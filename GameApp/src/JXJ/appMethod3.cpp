/**********************************************************************
*
*	文件:	   appMethod3.cpp
*
*	描述:	   
*
*	作者:		陆宾礼
*					
*
**********************************************************************/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// header files
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod3.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <CRUI/crWidgetExtend.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNodeCallbacks.h>
#include <CREncapsulation/crGameDices.h>
#include <JXJ/appData2.h>
#include <JXJ/appData3.h>
#include <stdlib.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crCookFile.h>
#include <sys/stat.h>
#include <CRIOManager/crConvertUTF.h>

using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRIOManager;
using namespace CRUI;
using namespace CRProducer;
using namespace CRIOManager;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// script method

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailShowMethod
// On  : Showing Mail Canvas
// Desp:	List mails
//			prepare widgets
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailShowMethod::crJXJUIOnMailShowMethod():
	m_this(NULL)
{}
JXJ::crJXJUIOnMailShowMethod::crJXJUIOnMailShowMethod(const crJXJUIOnMailShowMethod& handle):
	crMethod(handle), 
	m_this(handle.m_this),
	m_friend_canvas(handle.m_friend_canvas),
	m_switch(handle.m_switch),
	m_send_to(handle.m_send_to),
	m_title(handle.m_title),
	m_list(handle.m_list),
	m_mailsize(handle.m_mailsize),
	m_mailbody(handle.m_mailbody),
	m_deletebtn(handle.m_deletebtn),
	m_allselbtn(handle.m_allselbtn),
	m_pickupitem(handle.m_pickupitem),
	m_MailListScroll(handle.m_MailListScroll)
{
	for (int i = 0; i<5; i++)
	{
		m_mailchoice[i] = handle.m_mailchoice[i];
		m_mailinput[i] = handle.m_mailinput[i];
	}
	for (int i = 0; i<6; i++)
	{
		m_itemicon[i] = handle.m_itemicon[i];
	}
}
void JXJ::crJXJUIOnMailShowMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}
void JXJ::crJXJUIOnMailShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_friend_canvas = str;
		break;
	case 1:
		m_send_to = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_list = str;
		break;
	case 4:
		m_switch = str;
		break;
	case 5:
		m_mailchoice[0] = str;
		break;
	case 6:
		m_mailchoice[1] = str;
		break;
	case 7:
		m_mailchoice[2] = str;
		break;
	case 8:
		m_mailchoice[3] = str;
		break;
	case 9:
		m_mailchoice[4] = str;
		break;
	case 10:
		m_mailinput[0] = str;
		break;
	case 11:
		m_mailinput[1] = str;
		break;
	case 12:
		m_mailinput[2] = str;
		break;
	case 13:
		m_mailinput[3] = str;
		break;
	case 14:
		m_mailinput[4] = str;
		break;
	case 15:
		m_itemicon[0] = str;
		break;
	case 16:
		m_itemicon[1] = str;
		break;
	case 17:
		m_itemicon[2] = str;
		break;
	case 18:
		m_itemicon[3] = str;
		break;
	case 19:
		m_itemicon[4] = str;
		break;
	case 20:
		m_itemicon[5] = str;
		break;
	case 21:
		m_mailsize = str;
		break;
	case 22:
		m_mailbody = str;
		break;
	case 23:
		m_deletebtn = str;
		break;
	case 24:
		m_allselbtn = str;
		break;
	case 25:
		m_pickupitem = str;
		break;
	case 26:
		m_iconSwitch[0] = str;
		break;
	case 27:
		m_iconSwitch[1] = str;
		break;
	case 28:
		m_iconSwitch[2] = str;
		break;
	case 29:
		m_iconSwitch[3] = str;
		break;
	case 30:
		m_iconSwitch[4] = str;
		break;
	case 31:
		m_MailListScroll = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailShowMethod::operator()(crHandle &handle)
{
	// temp usage
	void *param;

	// 显示mail 窗口

	// 绑定显示好友列表
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	if (player)
	{
		//crCanvasNode *friend_list_node = crFilterRenderManager::getInstance()->findCanvas(m_friend_canvas);
		crWidgetNode *friend_list_node = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_friend_canvas));
		crMultiSwitch *pageswitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch));
		crEditWidgetNode *mailbody = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_mailbody));
		crButtonWidgetNode *deletebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_deletebtn));
		crButtonWidgetNode *allselbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_allselbtn));
		crButtonWidgetNode *pickupitem = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_pickupitem));
		crScrollBarWidgetNode *mailListScroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_MailListScroll));
		if (deletebtn)
		{
			deletebtn->setEnable(false);
		}
		if (allselbtn)
		{
			allselbtn->setEnable(false);
		}
		if (mailbody)
		{
			mailbody->clearString();
		}
		if (pageswitch)
		{
			pageswitch->setActiveSwitchSet(1);
		}
		if (pickupitem)
		{
			pickupitem->setVisiable(false);
			pickupitem->setEnable(false);
		}
		//if (mailListScroll)
		//{
		//	mailListScroll->setEnable(false);
		//}
		if(friend_list_node) 
		{
			friend_list_node->setVisiable(false);
			crTableWidgetNode *friend_table = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list));
			if (friend_table)
			{
				friend_table->clearData();
				crData *player_data = player->getDataClass();
				player_data->excHandle(MAKEINT64(WCH_LockData, 1));
				player_data->getParam(WCHDATA_JXJFriendList, param);
				FriendVec* friends = (FriendVec *)param;
				int i = 0;
				for(FriendVec::iterator itr = friends->begin(); itr != friends->end(); itr++, i++)
				{
					friend_table->addData(0, i, itr->get()->getName());
				}
				player_data->excHandle(MAKEINT64(WCH_LockData, 0));

				// show titles
				//CREncapsulation::crStaticTextWidgetNode *send_to_stxt = 
				//	dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(m_this->getWidget(m_send_to));
				//if (send_to_stxt)
				//{
				//	//send_to_stxt->setString(std::string("发件人:"));
				//	send_to_stxt->clearString();
				//	send_to_stxt->setVisiable(true);
				//}
				//CREncapsulation::crStaticTextWidgetNode *mail_title_stxt = 
				//	dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(m_this->getWidget(m_title));
				//if (mail_title_stxt)
				//{
				//	//mail_title_stxt->setString(std::string("主题:"));
				//	mail_title_stxt->clearString();
				//	mail_title_stxt->setVisiable(true);
				//}
			}
		}
	}
	crStaticTextWidgetNode *mailsize = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mailsize));
	if (mailsize)
	{
		mailsize->setString(std::string("0"));
	}
	crButtonWidgetNode *mailchoice[5] = {NULL};
	crStaticTextWidgetNode *mailinput[4] = {NULL};
	crImageBoxWidgetNode *itemicon[5] = {NULL};
	crMultiSwitch *bkIcon = NULL;
	for (int i = 0;i<5;i++)
	{
		bkIcon = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_iconSwitch[i]));
		if(bkIcon)
			bkIcon->setActiveSwitchSet(2);
	}
	crHypertextWidgetNode *bodyinput = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_mailinput[4]));
	if (bodyinput)
	{
		bodyinput->clearText();
	}
	if(m_this)
	{
		for (int i = 0; i< 5; i++)
		{
			if (i<4)
			{
				mailchoice[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_mailchoice[i]));
				if (mailchoice[i])
				{
					mailchoice[i]->setEnable(true);
				}
				mailinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mailinput[i]));
				if (mailinput[i])
				{
					mailinput[i]->clearString();
				}
			}
// 			itemicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_itemicon[i]));
// 			if (itemicon[i])
// 			{
// 				//itemicon[i]->setVisiable(false);
// 			}
			//
		}
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailFriendListMethod
// On  : Showing Mail Friend List
// Desp:	read friend list
//			display friend list
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailFriendListMethod::crJXJUIOnMailFriendListMethod():
	m_this(NULL)
{}
JXJ::crJXJUIOnMailFriendListMethod::crJXJUIOnMailFriendListMethod(const crJXJUIOnMailFriendListMethod& handle):
	crMethod(handle), m_this(handle.m_this), m_friend_canvas(handle.m_friend_canvas)
{}
void JXJ::crJXJUIOnMailFriendListMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}
void JXJ::crJXJUIOnMailFriendListMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_friend_canvas = str;
		break;

	case 1:
		m_friend_lbox = str;
	}
}
void JXJ::crJXJUIOnMailFriendListMethod::operator()(crHandle &handle)
{
	// toggle friend list canvas
	CRCore::crCanvasNode *canvas = m_this->getParentCanvas();
	CRCore::crNode *friend_node = canvas->getChildNode(m_friend_canvas); 
	friend_node->setVisiable(friend_node->getVisiable() ? 0 : 1);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailSendingMethod
// On  : Push Sending Button
// Desp:	check friend
//			send info to server
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailSendingMethod::crJXJUIOnMailSendingMethod():
	m_this(NULL)
{}
JXJ::crJXJUIOnMailSendingMethod::crJXJUIOnMailSendingMethod(const crJXJUIOnMailSendingMethod& handle):
	crMethod(handle), m_this(handle.m_this)
{}
void JXJ::crJXJUIOnMailSendingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}
void JXJ::crJXJUIOnMailSendingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_txt_friend = str;
		break;
	}
}
void JXJ::crJXJUIOnMailSendingMethod::operator()(crHandle &handle)
{
	// check friend name
	CRCore::crCanvasNode *canvas = m_this->getParentCanvas();
	CRCore::crWidgetNode *txt_friend = canvas->getWidget(this->m_txt_friend);

	// get send info
	// parse packet
	// send
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnChatFadeTimerMethod
// On  : 聊天窗口 Frame Update
// Desp: 时间到了 淡出
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnChatFadeTimerMethod::crJXJUIOnChatFadeTimerMethod():
	m_this(NULL), m_dt(NULL), m_fade_line(5.0f)
{}
JXJ::crJXJUIOnChatFadeTimerMethod::crJXJUIOnChatFadeTimerMethod(const crJXJUIOnChatFadeTimerMethod& handle):
	crMethod(handle), m_this(handle.m_this), m_dt(NULL), m_fade_line(5.0f)
{}
void JXJ::crJXJUIOnChatFadeTimerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
		}
		break;
	}
}
void JXJ::crJXJUIOnChatFadeTimerMethod::addParam(int i, const std::string& str)
{
}
void JXJ::crJXJUIOnChatFadeTimerMethod::operator()(crHandle &handle)
{
	// check needed
	if(!m_this->getVisiable()) return;

	// canvas data
	CRCore::crData* data = this->m_this->getDataClass();

	// update
	void *param;
	data->getParam(WCHDATA_JXJChatTimerFade, param);
	float *timer = (float *)param;
	*timer += *m_dt;

	// check reset 
	CRCore::crWidgetNode *focus_node = dynamic_cast<CRCore::crWidgetNode *>(CRCore::crFilterRenderManager::getInstance()->getFocusNode());
	if(focus_node && focus_node->getParentCanvas() && focus_node->getParentCanvas() == m_this)
	{
		*timer = 0.0f;
		return;
	}

	// check fade
	if(*timer >= m_fade_line)
	{
		*timer = 0.0f;
		CRCore::crFilterRenderManager::getInstance()->closeCanvas(m_this);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnFormationShowMethod
// On  : 打开编队界面
// Desp: 初始化编队界面
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnFormationShowMethod::crJXJUIOnFormationShowMethod()
{}
JXJ::crJXJUIOnFormationShowMethod::crJXJUIOnFormationShowMethod(const crJXJUIOnFormationShowMethod& handle):
	m_title_lead(handle.m_title_lead),
	m_title_used(handle.m_title_used),
	m_title_remain(handle.m_title_remain),
	m_pic(handle.m_pic),
	m_leftpage(handle.m_leftpage),
	m_rightpage(handle.m_rightpage),
	m_pic2(handle.m_pic2),
	m_radio(handle.m_radio),
	m_levelsw(handle.m_levelsw)
{
	for (int i = 0;i<9;i++)
	{
		if (i<8)
		{
			m_left_heads[i] = handle.m_left_heads[i];
			m_left_names[i] = handle.m_left_names[i];
			m_left_nums[i] = handle.m_left_nums[i];
		}
		if (i<3)
		{
			m_right_heads[i] = handle.m_right_heads[i];
			m_right_trp_total[i] = handle.m_right_trp_total[i];
			m_right_trp_used[i] = handle.m_right_trp_used[i];
			m_right_trp_plead[i] = handle.m_right_trp_plead[i];
			m_right_trp_ulead[i] = handle.m_right_trp_ulead[i];
			m_right_slide[i] = handle.m_right_slide[i];
			m_editbox[i] = handle.m_editbox[i];
		}
		if (i<6)
		{
			m_colorbox[i] = handle.m_colorbox[i];
		}
		m_right_eqs[i] = handle.m_right_eqs[i];
	}
}
void JXJ::crJXJUIOnFormationShowMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;

	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_left_heads[0] = str;
		break;
	case 1:
		m_left_heads[1] = str;
		break;
	case 2:
		m_left_heads[2] = str;
		break;
	case 3:
		m_left_heads[3] = str;
		break;
	case 4:
		m_left_heads[4] = str;
		break;
	case 5:
		m_left_heads[5] = str;
		break;
	case 6:
		m_left_heads[6] = str;
		break;
	case 7:
		m_left_heads[7] = str;
		break;
	case 8:
		m_left_names[0] = str;
		break;
	case 9:
		m_left_names[1] = str;
		break;
	case 10:
		m_left_names[2] = str;
		break;
	case 11:
		m_left_names[3] = str;
		break;
	case 12:
		m_left_names[4] = str;
		break;
	case 13:
		m_left_names[5] = str;
		break;
	case 14:
		m_left_names[6] = str;
		break;
	case 15:
		m_left_names[7] = str;
		break;
	case 16:
		m_left_nums[0] = str;
		break;
	case 17:
		m_left_nums[1] = str;
		break;
	case 18:
		m_left_nums[2] = str;
		break;
	case 19:
		m_left_nums[3] = str;
		break;
	case 20:
		m_left_nums[4] = str;
		break;
	case 21:
		m_left_nums[5] = str;
		break;
	case 22:
		m_left_nums[6] = str;
		break;
	case 23:
		m_left_nums[7] = str;
		break;
	case 24:
		m_title_lead = str;
		break;
	case 25:
		m_title_used = str;
		break;
	case 26:
		m_title_remain = str;
		break;
	case 27:
		m_right_heads[0] = str;
		break;
	case 28:
		m_right_heads[1] = str;
		break;
	case 29:
		m_right_heads[2] = str;
		break;
	case 30:
		m_right_trp_total[0] = str;
		break;
	case 31:
		m_right_trp_total[1] = str;
		break;
	case 32:
		m_right_trp_total[2] = str;
		break;
	case 33:
		m_right_trp_used[0] = str;
		break;
	case 34:
		m_right_trp_used[1] = str;
		break;
	case 35:
		m_right_trp_used[2] = str;
		break;
	case 36:
		m_right_trp_plead[0] = str;
		break;
	case 37:
		m_right_trp_plead[1] = str;
		break;
	case 38:
		m_right_trp_plead[2] = str;
		break;
	case 39:
		m_right_trp_ulead[0] = str;
		break;
	case 40:
		m_right_trp_ulead[1] = str;
		break;
	case 41:
		m_right_trp_ulead[2] = str;
		break;
	case 42:
		m_right_eqs[0] = str;
		break;
	case 43:
		m_right_eqs[1] = str;
		break;
	case 44:
		m_right_eqs[2] = str;
		break;
	case 45:
		m_right_eqs[3] = str;
		break;
	case 46:
		m_right_eqs[4] = str;
		break;
	case 47:
		m_right_eqs[5] = str;
		break;
	case 48:
		m_right_eqs[6] = str;
		break;
	case 49:
		m_right_eqs[7] = str;
		break;
	case 50:
		m_right_eqs[8] = str;
		break;
	case 51:
		m_right_slide[0] = str;
		break;
	case 52:
		m_right_slide[1] = str;
		break;
	case 53:
		m_right_slide[2] = str;
		break;
	case 54:
		m_pic = str;
		break;
	case 55:
		m_leftpage = str;
		break;
	case 56:
		m_rightpage = str;
		break;
	case 57:
		m_colorbox[0] = str;
		break;
	case 58:
		m_colorbox[1] = str;
		break;
	case 59:
		m_colorbox[2] = str;
		break;
	case 60:
		m_colorbox[3] = str;
		break;
	case 61:
		m_colorbox[4] = str;
		break;
	case 62:
		m_colorbox[5] = str;
		break;
	case 63:
		m_pic2 = str;
		break;
	case 64:
		m_radio = str;
		break;
	case 65:
		m_editbox[0] = str;
		break;
	case 66:
		m_editbox[1] = str;
		break;
	case 67:
		m_editbox[2] = str;
		break;
	case 68:
		m_levelsw = str;
		break;
	case 69:
		m_formationgamelist = str;
		break;
	case 70:
	case 71:
	case 72:
	case 73:
	case 74:
	case 75:
	case 76:
	case 77:
	case 78:
		m_newleftnum[i - 70] = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationShowMethod::operator()(crHandle &handle)
{
	//
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (!m_this && !playerData)
	{
		return;
	}
	crRole *role = playerData->getMainRole();
	if (!role)
	{
		return;
	}
	crData *RoleData = playerData->getMainRole()->getDataClass();
	crData *data = playerData->getDataClass();
	if (!RoleData || !data)
	{
		return;
	}
	RoleData->getParam(WCHDATA_Level,param);
	unsigned char lv = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	crMultiSwitch *multiswitch =dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_levelsw));
	if (multiswitch)
	{
		multiswitch -> setActiveSwitchSet(1);
		unsigned char firstlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation1stRowOpenLv,viplv).c_str()));
		unsigned char secondlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation2ndRowOpenLv,viplv).c_str()));
		
		if (lv >= secondlv)
		{
			multiswitch->setActiveSwitchSet(0);
		}
		else if (lv >= firstlv)
		{
			multiswitch->setActiveSwitchSet(2);
		}
	}

	crMultiSwitch *colorbox[6] = {NULL};
	for (int i = 0; i < 6; i++)
	{
		colorbox[i]=dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_colorbox[i]));
		if (colorbox[i])
		{
			colorbox[i]->setActiveSwitchSet(0);
		}
	}
	//
	////////////////////////////////////////////////统率影响单条领导力
	float totletongshuai = 0.0f;
	int perpagenum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormationOnePageNum,viplv).c_str()));
	int permaxlead = 0;//(int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLeadPerLine).c_str()));
	crVector3f factor;
	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),factor);
	role->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
	permaxlead = totletongshuai * factor[0] + factor[1];
	int total_lead = 0;
	playerData->doEvent(WCH_JXJFormationGetLeaderPower,MAKEINT64(&total_lead,NULL));
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// global usage
	// temp
	int									index = 1;	// dealing position
	crTableIO::StrVec	record;		// record extracting
	int									index_to_col = 0; // col and row indexing

	// datas
	crData *canvas_data = m_this->getDataClass();										// canvas
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	crData *player_data = player->getDataClass();										// player data
	crData *player_role_data = player->getMainRole()->getDataClass();						// player role data

	// get formations information
	//player_data->lock();
	//player_data->getParam(WCHDATA_JXJFormationInfoVec, param);
	//FormationInfoVec *formations = (FormationInfoVec *)param;
	// tables											// player troop
	canvas_data->getParam(WCHDATA_JXJTempFormationVec, param);
	FormationInfoVec *tempformationvec = (FormationInfoVec *)param;														// player troop
	canvas_data->getParam(WCHDATA_JXJArmsCells, param);
	int	*arms =	(int *)param;															// ui arms flag
	//canvas_data->getParam(WCHDATA_JXJForedCells, param);
	//int							*eqps			=	(int *)param;
	//canvas_data->getParam(WCHDATA_JXJForedHeadCells, param);
	//int							*heds			=	(int *)param;
	ref_ptr<CREncapsulation::crTableIO>troop_table =	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
	ref_ptr<CREncapsulation::crTableIO>gov_table	 =	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab); // govecap look-up table
	ref_ptr<CREncapsulation::crTableIO>equip_table =	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);	// equips look-up table

	crButtonWidgetNode *leftbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leftpage));
	crButtonWidgetNode *rightbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rightpage));
	crRadioGroupWidgetNode *radio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radio));
	crListControlWidgetNode *formationlist = dynamic_cast<crListControlWidgetNode *>(m_this->getWidget(m_formationgamelist));

	crEditWidgetNode *editbox[3] = {NULL};
	for (int i = 0; i<3;i++)
	{
		editbox[i] = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_editbox[i]));
		if (editbox[i])
		{
			editbox[i]->setEnable(false);
			editbox[i]->clearString();
		}
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - left side - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// get selecting
	//canvas_data->getParam(WCHDATA_JXJArmsSelecting, param);
	//char selcting = *((char*)param);
	int lead_index = troop_table->getTitleIndex("领导力");
	int troopsiconindex = troop_table->getTitleIndex("icon");
	char selcting = -1;
	if (radio)
	{
		if(radio->getSelect() != 0)
		{
			switch (radio->getSelect())
			{
			case 0:
				selcting = -1;
				break;
			case 1:
				selcting = 0;
				break;
			case 2:
				selcting = 2;
				break;
			case 3:
				selcting = 1;
				break;
			case 4:
				selcting = 3;
				break;
			default:
				break;
			}
		}
		
	}
	player_data->lock();
	player_data->getParam(WCHDATA_JXJTroopsMap, param);
	TroopsMap *troops =	(TroopsMap *)param;		
	int size = troops->size();
	player_data->unlock();
	crData *brainData = crBrain::getInstance()->getDataClass();
	brainData->excHandle(MAKEINT64(WCH_LockData,1));
	brainData->getParam(WCHDATA_JXJFormationCurPage,param);
	int *curpagenum = (int *)param;
	brainData->getParam(WCHDATA_JXJFormationTotalPage,param);
	int *totalpage = (int *)param;
	*totalpage = ceil((float)size/(float)perpagenum);
	if (*totalpage < 1 || selcting != -1)
	{
		*totalpage = 1;
	}
	if (*curpagenum > *totalpage)
	{
		*curpagenum = 1;
	}
	if (leftbtn)
	{
		leftbtn->setEnable(true);
		if (*curpagenum <= 1)
		{
			leftbtn->setEnable(false);
		}
	}
	if (rightbtn)
	{
		rightbtn->setEnable(true);
		if (*curpagenum >= *totalpage)
		{
			rightbtn->setEnable(false);
		}
	}
	brainData->excHandle(MAKEINT64(WCH_LockData,0));
	int count = 0;
	int row = -(*curpagenum - 1) * perpagenum;
	// show selecting arms
	int tempcount = 0;
	int maxformcount = 0;
	crStaticTextWidgetNode *bnum_node = NULL;
	if (formationlist)
	{
		formationlist->setListNodeCount(troop_table->getRowCount());
		//formationlist->setStartYPos(*curpagenum - 1);
		crListControlWidgetNode::ListNodeVec& listNodeVec = formationlist->getListNodeVec();
		player_data->lock();
		for(TroopsMap::iterator itr = troops->begin(); count<perpagenum && itr != troops->end(); ++itr)
		{
			if(troop_table->queryOneRecord(0, crArgumentParser::appItoa(itr->first), record) < 0)
				continue;
			//

			tempcount = itr->second;
			for (FormationInfoVec::iterator fitr = tempformationvec->begin(); fitr != tempformationvec->end();++fitr)
			{
				if (fitr->valid())
				{
					if ((*fitr)->getAbstractID() == itr->first)
					{
						tempcount += (*fitr)->getCount();
					}
				}
			}
			if (tempcount <= 0)
			{
				continue;
			}
			// arm type
			if(selcting != -1)
			{
				if(atoi(record[2].c_str()) != selcting)
				{
					continue;
				}
			}
			else
			{
				if(row<0)
				{
					row++;
					continue;
				}
			}
			count++;
			// insert information
			//crImageBoxWidgetNode *image_box = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_left_heads[index_to_col]));
			//image_box->setVisiable(true);
			//image_box->setImageName(record[10]);
			//crStaticTextWidgetNode *bname_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_left_names[index_to_col]));
			//bname_node->setString(record[1]);
			//bname_node->setVisiable(true);
			//crStaticTextWidgetNode *bnum_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_left_nums[index_to_col]));
			//bnum_node->setString(crArgumentParser::appItoa(tempcount));
			//bnum_node->setVisiable(true);
			bnum_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_newleftnum[index_to_col]));
			bnum_node->setString(crArgumentParser::appItoa(tempcount)/* + "/" + crArgumentParser::appItoa(maxformcount)*/);
			//listNodeVec[index_to_col]->setTitle("");
			//listNodeVec[index_to_col]->setTitle2("");
			listNodeVec[index_to_col]->setData(itr->first);
			listNodeVec[index_to_col]->setImageName(record[troopsiconindex]);
			listNodeVec[index_to_col]->setVisiable(true);
			listNodeVec[index_to_col]->setEnable(true);
			arms[index_to_col] = itr->first;
			index_to_col++;
		}
		player_data->unlock();
		for(; index_to_col < 9; index_to_col++)
		{
			listNodeVec[index_to_col]->setData(0);
			listNodeVec[index_to_col]->setImageName("");
			listNodeVec[index_to_col]->setVisiable(false);
			listNodeVec[index_to_col]->setEnable(false);
			bnum_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_newleftnum[index_to_col]));
			bnum_node -> clearString();
			//CRCore::crWidgetNode *image_box = m_this->getWidget(m_left_heads[index_to_col]);
			//image_box->setVisiable(false);
			//crStaticTextWidgetNode *bname_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_left_names[index_to_col]));
			//bname_node->setString(std::string(""));
			//crStaticTextWidgetNode *bnum_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_left_nums[index_to_col]));
			//bnum_node->setString(std::string(""));
			arms[index_to_col] = -1;
		}
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - right side - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// total lead
	crStaticTextWidgetNode *lead_cap_node = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title_lead));
	if(lead_cap_node)lead_cap_node->setString(crArgumentParser::appItoa(total_lead));
	//player_role_data->getParam(WCHDATA_JXJGovPost, param);
	//gov_table->queryOneRecord(0, crArgumentParser::appItoa(*((unsigned char*)param)), record);
	//lead_cap_node->setString(record[2]);


	//int					total_lead			= atoi(record[2].c_str());
	int					total_used_lead		= 0;
						index_to_col		= 0;
						index				= 0;

	// calculate total troops
	std::map<int, int> temp_check;
	std::map<int, int> usedtroop_check;
	int jj = 0;
	for(FormationInfoVec::iterator itr = tempformationvec->begin(); itr != tempformationvec->end(); itr++,jj++)
	{
		if ((*itr) != NULL)
		{
			if (editbox[jj])
			{
				editbox[jj]->setEnable(true);
				editbox[jj]->setString(crArgumentParser::appItoa((*itr)->getCount()));
			}
		}
		std::map<int, int>::iterator find_itr = temp_check.find((*itr).get()->getAbstractID());
		if(find_itr == temp_check.end())
		{
			temp_check.insert(std::pair<int, int>((*itr).get()->getAbstractID(), (*itr).get()->getCount()));
		}
		else
		{
			find_itr->second += (*itr).get()->getCount();
		}
		//////
		std::map<int, int>::iterator findused_itr = usedtroop_check.find((*itr).get()->getAbstractID());
		if (findused_itr == usedtroop_check.end())
		{
			usedtroop_check.insert(std::pair<int, int>((*itr).get()->getAbstractID(), (*itr).get()->getCount()));
		}
		else
		{
			findused_itr->second += (*itr).get()->getCount();
		}
	}
	player_data->lock();
	for(TroopsMap::iterator itr = troops->begin(); itr != troops->end(); itr++)
	{
		std::map<int, int>::iterator find_itr = temp_check.find((*itr).first);
		if(find_itr == temp_check.end())
		{
			temp_check.insert(std::pair<int, int>((*itr).first, (*itr).second));
		}
		else
		{
			find_itr->second += (*itr).second;
		}
	}
	player_data->unlock();
	int j = 0;
	//tempformationvec->clear();
	//tempformationvec->resize(3);
	//for (FormationInfoVec::iterator itr = tempformationvec->begin(); itr != tempformationvec->end() && j< 3; itr++,j++)
	//{
	//	if (itr->valid())
	//	{
	//		(*itr)->setAbstractID(-1);
	//		(*itr)->setCount(0);
	//		(*itr)->setEquips(crVector2i(-1,-1));
	//	}
	//	else
	//	{
	//		ref_ptr<crFormationInfo> newformation = new crFormationInfo;
	//		newformation->setAbstractID(-1);
	//		newformation->setCount(0);
	//		newformation->setEquips(crVector2i(-1,-1));
	//		(*itr) = newformation;
	//	}
	//}
	for(j = 0;j< 3;j++)
	{
		crImageBoxWidgetNode *image_box1 = 
			dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_right_heads[j]));
		if(image_box1)
		{
			image_box1->setVisiable(true);
			image_box1->setImageName("UI_Formation_Bingzhongdi.img");
		}

		crStaticTextWidgetNode *image_box2 = 
			dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_total[j]));
		if(image_box2)
		{
			image_box2->setVisiable(false);
			image_box2->clearString();
		}

		crStaticTextWidgetNode *image_box3 = 
			dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_used[j]));
		if(image_box3)
		{
			image_box3->setVisiable(false);
			image_box3->clearString();
		}

		crStaticTextWidgetNode *image_box4 = 
			dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_plead[j]));
		if(image_box4)
		{
			image_box4->setVisiable(false);
			image_box4->clearString();
		}

		crStaticTextWidgetNode *image_box5 = 
			dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_ulead[j]));
		if(image_box5)
		{
			image_box5->setVisiable(false);
			image_box5->clearString();
		}

		crScrollBarWidgetNode *slider = dynamic_cast<crScrollBarWidgetNode*>(m_this->getWidget(m_right_slide[j]));
		if(slider)
		{
			slider->setVisiable(false);
			slider->setValue(0);
		}
		crWidgetNode *eqp_head;
		for(int i = 0; i < 3; i++)
		{
			eqp_head = m_this->getWidget(m_right_eqs[j * 3 + i]);
			if (eqp_head)
			{
				CRCore::crStateSet *state_set =
					dynamic_cast<CRCore::crObject *>(( dynamic_cast<CRCore::crGroup *>(eqp_head->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
				if (state_set)
				{
					CRCore::crTexture2D *texture_2d = 
						dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
					if (texture_2d)
					{
						if (i == 0)
						{
							texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic,texture_2d));
						}
						else
						{
							texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic2,texture_2d));
						}
						//	eqp_head->setVisiable(false);
					}
				}
			}
		}
	}
	j = 0;
	for(FormationInfoVec::iterator itr = tempformationvec->begin(); itr != tempformationvec->end() && j< 3; itr++,j++)
	{
		int		total_troops	= 0;
		int		used_troops		= 0;
		int		remain_troops	= 0;
		int		p_lead			= 0;
		int		used_lead		= 0;

		// get formation info
		JXJ::crFormationInfo *formation = itr->get();
		//ref_ptr<crFormationInfo> newformation = new crFormationInfo;
		//newformation->setAbstractID(formation->getAbstractID());
		//newformation->setCount(formation->getCount());
		//newformation->setEquips(formation->getEquips());
		//(*tempformationvec)[j] = newformation;

		if(troop_table->queryOneRecord(0, crArgumentParser::appItoa(formation->getAbstractID()), record) >= 0)
		{
			int iconindex = troop_table->getTitleIndex("icon");
			//eqps[index_to_col] = formation->getAbstractID();

			// insert arm
			p_lead = atoi(record[lead_index].c_str());
			used_lead = p_lead * formation->getCount();
			total_used_lead += used_lead; // calculate used

			crImageBoxWidgetNode *head_box = 
				dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_right_heads[index_to_col]));
			// insert arm head
			if (head_box)
			{
				head_box->setImageName(record[iconindex]);
				head_box->setVisiable(true);
			}
		}

		// insert equips
		int cardcolor = 0;
		crVector2i equips = formation->getEquips();
		int color = equip_table->getTitleIndex("品质");
		crWidgetNode *eqp_box;
		for(int i = 0; i < 2; i++)
		{
		//	heds[index_to_col * 3 + i] = equips[i];
			int is_get = equip_table->queryOneRecord(0, crArgumentParser::appItoa(equips[i]), record);
			eqp_box = m_this->getWidget(m_right_eqs[index_to_col * 3 + i]);
			if (eqp_box)
			{
				if (i<2)
				{
					//crMultiSwitch *multiswitch =dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_colorbox[index_to_col * 2 + i]));
					if (colorbox[index_to_col * 2 + i])
					{
						colorbox[index_to_col * 2 + i]->setVisiable(true);
						if (is_get >= 0)
						{
							cardcolor = atoi(record[color].c_str());
							//if (cardcolor > CardColor_Orange)
							//{
							//	cardcolor = CardColor_Orange;
							//}
							colorbox[index_to_col * 2 + i]->setActiveSwitchSet(1 + cardcolor);
						}
					}
				}

				CRCore::crStateSet *state_set = dynamic_cast<CRCore::crObject *>(( dynamic_cast<CRCore::crGroup *>(eqp_box->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
				if (state_set)
				{
					CRCore::crTexture2D *texture_2d = dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
					eqp_box->setVisiable(true);
					if(is_get >= 0)
					{
						// TODO:
						int icon_id = equip_table->getTitleIndex("icon");
						texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(record[icon_id],texture_2d));
					}
					else
					{
						if (i == 0)
						{
							texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic,texture_2d));
						}
						else
						{
							texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic2,texture_2d));
						}
					}
				}
			}
		}

		// get nums
		if (formation->getAbstractID() > 0)
		{
			total_troops = temp_check.find(formation->getAbstractID())->second;
			used_troops = formation->getCount();
			remain_troops = total_troops - usedtroop_check.find(formation->getAbstractID())->second;

			permaxlead = total_lead / 3;
			int temptrooptotal = permaxlead / p_lead;
			//if (temptrooptotal > total_troops)
			//{
			//	temptrooptotal = total_troops;
			//}
			// insert nums
			crStaticTextWidgetNode *input_1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_total[index_to_col]));
			if (input_1)
			{
				input_1->setString(crArgumentParser::appItoa(temptrooptotal));
				input_1->setVisiable(true);
			}

			crStaticTextWidgetNode *input_2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_used[index_to_col]));
			if (input_2)
			{
				if (used_troops < temptrooptotal)
				{
					input_2->setColor(FCOLOR_RED);
				}
				else
				{
					input_2->setColor(FCOLOR_BLACK);
				}
				if (used_troops > temptrooptotal)
				{
					formation->setCount(temptrooptotal);
				}
				input_2->setString(crArgumentParser::appItoa(used_troops) + "/" + crArgumentParser::appItoa(temptrooptotal));
				input_2->setVisiable(true);
			}

			crStaticTextWidgetNode *input_3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_plead[index_to_col]));
			if (input_3)
			{
				input_3->setString(crArgumentParser::appItoa(p_lead));
				input_3->setVisiable(true);
			}

			crStaticTextWidgetNode *input_4 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_right_trp_ulead[index_to_col]));
			if (input_4)
			{
				input_4->setString(crArgumentParser::appItoa(used_lead));
				input_4->setVisiable(true);
			}

			// slide
			//crScrollBarWidgetNode *slider = dynamic_cast<crScrollBarWidgetNode*>(m_this->getWidget(m_right_slide[index_to_col]));
			//slider->setRange(0.0f, temptrooptotal);
			////slider->setPageValue(1.0f);
			//slider->setLineValue(1.0f);
			//slider->outerSetValue(itr->get()->getCount());
			//slider->setVisiable(true);
		}
		index_to_col++;
	}

	// calculate used lead
	crStaticTextWidgetNode *lead_used_node = 
		dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title_used));
	if(lead_used_node)lead_used_node->setString(crArgumentParser::appItoa(total_used_lead));
	crStaticTextWidgetNode *lead_red_node = 
		dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title_remain));
	if(lead_red_node)lead_red_node->setString(crArgumentParser::appItoa(total_lead - total_used_lead));

	//index = index_to_col;
	//for(; index < 3; index++)
	//{//wch?
	//	//eqps[index] = -1;
	//}

	// clear 
	// for heads
	//index = index_to_col;
	//for(index = 0; index < 3; index++)
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnEnterShowChatMethod
// On  : 在屏幕中输入Enter 
// Desp: 显示聊天框
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnFormationArmSelectingMethod::crJXJUIOnFormationArmSelectingMethod()
{
	m_tempformaionstr = "FormationRaido2";
}
JXJ::crJXJUIOnFormationArmSelectingMethod::crJXJUIOnFormationArmSelectingMethod(const crJXJUIOnFormationArmSelectingMethod& handle):
	m_leftbtn(handle.m_leftbtn),
	m_rightbtn(handle.m_rightbtn),
	m_formationradio(handle.m_formationradio),
	m_tempformaionstr(handle.m_tempformaionstr)
{
	for (int i = 0;i < 8; i++)
	{
		m_left_heads[i] = handle.m_left_heads[i];
		m_left_names[i] = handle.m_left_names[i];
		m_left_nums[i] = handle.m_left_nums[i];
	}
}
void JXJ::crJXJUIOnFormationArmSelectingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;

	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmSelectingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_left_heads[0] = str;
		break;
	case 1:
		m_left_heads[1] = str;
		break;
	case 2:
		m_left_heads[2] = str;
		break;
	case 3:
		m_left_heads[3] = str;
		break;
	case 4:
		m_left_heads[4] = str;
		break;
	case 5:
		m_left_heads[5] = str;
		break;
	case 6:
		m_left_heads[6] = str;
		break;
	case 7:
		m_left_heads[7] = str;
		break;
	case 8:
		m_left_names[0] = str;
		break;
	case 9:
		m_left_names[1] = str;
		break;
	case 10:
		m_left_names[2] = str;
		break;
	case 11:
		m_left_names[3] = str;
		break;
	case 12:
		m_left_names[4] = str;
		break;
	case 13:
		m_left_names[5] = str;
		break;
	case 14:
		m_left_names[6] = str;
		break;
	case 15:
		m_left_names[7] = str;
		break;
	case 16:
		m_left_nums[0] = str;
		break;
	case 17:
		m_left_nums[1] = str;
		break;
	case 18:
		m_left_nums[2] = str;
		break;
	case 19:
		m_left_nums[3] = str;
		break;
	case 20:
		m_left_nums[4] = str;
		break;
	case 21:
		m_left_nums[5] = str;
		break;
	case 22:
		m_left_nums[6] = str;
		break;
	case 23:
		m_left_nums[7] = str;
		break;
	case 24:
		m_selecting = atoi(str.c_str());
		break;
	case 25:
		m_leftbtn = str;
		break;
	case 26:
		m_rightbtn = str;
		break;
	case 27:
		m_formationradio = str;
		break;
	case 28:
		m_formationgamelist = str;
		break;
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		m_newleftnum[i - 29] = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmSelectingMethod::operator()(crHandle &handle)
{
	// global usage variable
	int									index = 1;	// dealing position
	void								*param;		// parameter extract
	CREncapsulation::crTableIO::StrVec	record;		// record extracting
	int									index_to_col = 0; // col and row indexing


	//	entities
	CRNetApp::crPlayerGameData		*player				= CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	CRCore::crCanvasNode			*formation_canvas	= m_this->getParentCanvas();									// formation canvas

	// datas
	CRCore::crData					*formation_data		= formation_canvas->getDataClass();								// canvas data
	CRCore::crData					*player_data		= player->getDataClass();										// player data
	CRCore::crData					*player_role_data	= player->getMainRole()->getDataClass();						// player role data

	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	int perpagenum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormationOnePageNum,viplv).c_str()));
	// tables
	player_data->getParam(WCHDATA_JXJTroopsMap, param);								// l-param
	TroopsMap						*troops				=	(TroopsMap *)param;		// ul-param										// player troop
	ref_ptr<CREncapsulation::crTableIO>troop_table		=	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
	formation_canvas->getDataClass()->getParam(WCHDATA_JXJArmsCells, param);
	int								*arms				=	(int *)param;
	crButtonWidgetNode *leftbtn = dynamic_cast<crButtonWidgetNode *>(formation_canvas->getWidget(m_leftbtn));
	crButtonWidgetNode *rightbtn = dynamic_cast<crButtonWidgetNode *>(formation_canvas->getWidget(m_rightbtn));
	crRadioGroupWidgetNode *formationradio = dynamic_cast<crRadioGroupWidgetNode *>(formation_canvas->getWidget(m_formationradio));
	crRadioGroupWidgetNode *tempformaionradio = dynamic_cast<crRadioGroupWidgetNode *>(formation_canvas->getWidget(m_tempformaionstr));
	crListControlWidgetNode *formationlist = dynamic_cast<crListControlWidgetNode *>(formation_canvas->getWidget(m_formationgamelist));
	if (!formationlist || !tempformaionradio)
	{
		return;
	}
	CRCore::ScopedLock<crData> lock(*player_data);
	int troopsiconindex = troop_table->getTitleIndex("icon");
	// check selecting
	//formation_data->addParam(WCHDATA_JXJArmsSelecting, &m_selecting);
	//int arm_type = m_selecting;
	int tempindex = tempformaionradio->getSelect();
	//formation_data->getParam(WCHDATA_JXJTempFormationVec, param);
	//FormationInfoVec *tempformation = (FormationInfoVec *)param;
	FormationInfoVec tempformation;
	player_data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
	SavedFormationInfoVec *savedtempformationvec = (SavedFormationInfoVec *)param;
	if (tempindex < savedtempformationvec->size())
	{
		tempformation = (*savedtempformationvec)[tempindex].second;
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - left side - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// get selecting
	char selcting = m_selecting;

	if (selcting == -2)
	{
		if (formationradio)
		{
			selcting = formationradio->getSelect();
			switch (selcting)
			{
			case 0:
				selcting = -1;
				break;
			case 1:
				selcting = 0;
				break;
			case 2:
				selcting = 2;
				break;
			case 3:
				selcting = 1;
				break;
			case 4:
				selcting = 4;
				break;
			default:
				break;
			}
		}
		else
		{
			selcting = 0;
		}
	}
	int size = 0;// = troops->size();
	crData *brainData = crBrain::getInstance()->getDataClass();
	brainData->excHandle(MAKEINT64(WCH_LockData,1));
	brainData->getParam(WCHDATA_JXJFormationCurPage,param);
	int *curpagenum = (int *)param;
	brainData->getParam(WCHDATA_JXJFormationTotalPage,param);
	int *totalpage = (int *)param;
	int count = 0;
	int row = -(*curpagenum - 1) * perpagenum;
	// show selecting arms
	int tempcount = 0;
	crStaticTextWidgetNode *bnum_node = NULL;
	crListControlWidgetNode::ListNodeVec& listNodeVec = formationlist->getListNodeVec();
	for(TroopsMap::iterator itr = troops->begin(); /*count<perpagenum && */itr != troops->end(); ++itr)
	{
		if(troop_table->queryOneRecord(0, crArgumentParser::appItoa(itr->first), record) < 0)
		{
			continue;
		}

		tempcount = itr->second;
		for (FormationInfoVec::iterator fitr = tempformation.begin(); fitr != tempformation.end();++fitr)
		{
			if (fitr->valid())
			{
				if ((*fitr)->getAbstractID() == itr->first)
				{
					tempcount += (*fitr)->getCount();
				}
			}
		}
		if (tempcount <= 0)
		{
			continue;
		}
		// arm type
		if(selcting != -1)
		{
			if(atoi(record[2].c_str()) != selcting)
			{
				continue;
			}
		}
		size++;
		if (count >= perpagenum)
		{
			continue;
		}
		if(row<0)
		{
			row++;
			continue;
		}
		count++;
		// insert information
		//crImageBoxWidgetNode *image_box = 
		//	dynamic_cast<crImageBoxWidgetNode *>(formation_canvas->getWidget(m_left_heads[index_to_col]));
		//image_box->setVisiable(true);
		//image_box->setImageName(record[10]);
		//crStaticTextWidgetNode *bname_node = 
		//	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_names[index_to_col]));
		//bname_node->setString(record[1]);
		//bname_node->setVisiable(true);
		//crStaticTextWidgetNode *bnum_node = 
		//	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_nums[index_to_col]));
		//bnum_node->setString(crArgumentParser::appItoa(tempcount));
		//bnum_node->setVisiable(true);
		bnum_node = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_newleftnum[index_to_col]));
		bnum_node->setString(crArgumentParser::appItoa(tempcount));
		listNodeVec[index_to_col]->setData(itr->first);
		listNodeVec[index_to_col]->setImageName(record[troopsiconindex]);
		listNodeVec[index_to_col]->setVisiable(true);
		listNodeVec[index_to_col]->setEnable(true);
		arms[index_to_col] = itr->first;
		index_to_col++;
	}

	for(; index_to_col < 9; index_to_col++)
	{
		bnum_node = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_newleftnum[index_to_col]));
		bnum_node->clearString();
		listNodeVec[index_to_col]->setData(0);
		listNodeVec[index_to_col]->setImageName("");
		listNodeVec[index_to_col]->setVisiable(false);
		listNodeVec[index_to_col]->setEnable(false);
		//CRCore::crWidgetNode *image_box = formation_canvas->getWidget(m_left_heads[index_to_col]);
		//image_box->setVisiable(false);
		//crStaticTextWidgetNode *bname_node = 
		//	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_names[index_to_col]));
		//bname_node->setString(std::string(""));
		//crStaticTextWidgetNode *bnum_node = 
		//	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_nums[index_to_col]));
		//bnum_node->setString(std::string(""));
		arms[index_to_col] = -1;
	}
	*totalpage = ceil((float)size/(float)perpagenum);
	if (*totalpage < 1/* || selcting != -1*/)
	{
		*totalpage = 1;
	}
	if (*curpagenum > *totalpage)
	{
		*curpagenum = 1;
	}
	if (leftbtn)
	{
		leftbtn->setEnable(true);
		if (*curpagenum <= 1)
		{
			leftbtn->setEnable(false);
		}
	}
	if (rightbtn)
	{
		rightbtn->setEnable(true);
		if (*curpagenum >= *totalpage)
		{
			rightbtn->setEnable(false);
		}
	}
	brainData->excHandle(MAKEINT64(WCH_LockData,0));
	//// show selecting arms
	//for(TroopsMap::iterator itr = troops->begin(); itr != troops->end(); ++itr)
	//{
	//	troop_table->queryOneRecord(0, crArgumentParser::appItoa(itr->first), record);

	//	// arm type
	//	if(m_selecting != -1)
	//	{
	//		if(atoi(record[2].c_str()) != arm_type)
	//		{
	//			continue;
	//		}
	//	}

	//	// insert information
	//	CREncapsulation::crImageBoxWidgetNode *image_box = 
	//		dynamic_cast<CREncapsulation::crImageBoxWidgetNode *>(formation_canvas->getWidget(m_left_heads[index_to_col]));
	//	image_box->setVisiable(true);
	//	image_box->setImageName(record[10]);
	//	CREncapsulation::crStaticTextWidgetNode *bname_node = 
	//		dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_names[index_to_col]));
	//	bname_node->setString(record[1]);
	//	bname_node->setVisiable(true);
	//	CREncapsulation::crStaticTextWidgetNode *bnum_node = 
	//		dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_nums[index_to_col]));
	//	bnum_node->setString(crArgumentParser::appItoa(itr->second));
	//	bnum_node->setVisiable(true);
	//	arms[index_to_col] = itr->first;
	//	index_to_col++;
	//}

	//for(; index_to_col < 8; index_to_col++)
	//{
	//	CRCore::crWidgetNode *image_box = formation_canvas->getWidget(m_left_heads[index_to_col]);
	//	image_box->setVisiable(false);
	//	CREncapsulation::crStaticTextWidgetNode *bname_node = 
	//		dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_names[index_to_col]));
	//	bname_node->setString(std::string(""));
	//	CREncapsulation::crStaticTextWidgetNode *bnum_node = 
	//		dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(formation_canvas->getWidget(m_left_nums[index_to_col]));
	//	bnum_node->setString(std::string(""));
	//	arms[index_to_col] = -1;
	//}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnFormationArmCancelMethod
// On  : 编制撤销
// Desp: 初始化编队兵种选择
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnFormationArmCancelMethod::crJXJUIOnFormationArmCancelMethod()
{}
JXJ::crJXJUIOnFormationArmCancelMethod::crJXJUIOnFormationArmCancelMethod(const crJXJUIOnFormationArmCancelMethod& handle):
	m_index(handle.m_index),
	m_title_lead(handle.m_title_lead),
	m_title_used(handle.m_title_used),
	m_title_remain(handle.m_title_remain),
	m_num_total(handle.m_num_total),
	m_num_used(handle.m_num_used),
	m_num_plead(handle.m_num_plead),
	m_num_ulead(handle.m_num_ulead),
	m_slide(handle.m_slide),
	m_arm_head(handle.m_arm_head),
	m_card(handle.m_card),
	m_pic(handle.m_pic),
	m_pic2(handle.m_pic2),
	m_tipcanvas(handle.m_tipcanvas),
	m_tiptitle(handle.m_tiptitle),
	m_tipcontent(handle.m_tipcontent)
{
	for (int i = 0;i<6;i++)
	{
		if (i<3)
		{
			m_eqs[i] = handle.m_eqs[i];
			m_editbox[i] = handle.m_editbox[i];
		}
		m_colorbox[i] = handle.m_colorbox[i];
	}
}
void JXJ::crJXJUIOnFormationArmCancelMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;

	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmCancelMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = str;
		break;
	case 1:
		m_title_lead = str;
		break;
	case 2:
		m_title_used = str;
		break;
	case 3:
		m_title_remain = str;
		break;
	case 4:
		m_num_total = str;
		break;
	case 5:
		m_num_used = str;
		break;
	case 6:
		m_num_plead = str;
		break;
	case 7:
		m_num_ulead = str;
		break;
	case 8:
		m_slide = str;
		break;
	case 9:
		m_arm_head = str;
		break;
	case 10:
		m_eqs[0] = str;
		break;
	case 11:
		m_eqs[1] = str;
		break;
	case 12:
		m_eqs[2] = str;
		break;
	case 13:
		m_card = str;
		break;
	case 14:
		m_pic = str;
		break;
	case 15:
		m_colorbox[0] = str;
		break;
	case 16:
		m_colorbox[1] = str;
		break;
	case 17:
		m_colorbox[2] = str;
		break;
	case 18:
		m_colorbox[3] = str;
		break;
	case 19:
		m_colorbox[4] = str;
		break;
	case 20:
		m_colorbox[5] = str;
		break;
	case 21:
		m_pic2 = str;
		break;
	case 22:
		m_editbox[0] = str;
		break;
	case 23:
		m_editbox[1] = str;
		break;
	case 24:
		m_editbox[2] = str;
		break;
	case 25:
		m_tipcanvas = str;
		break;
	case 26:
		m_tiptitle = str;
		break;
	case 27:
		m_tipcontent = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmCancelMethod::operator()(crHandle &handle)
{
	// global usage varialbe
	int									index = atoi(m_index.c_str());	// dealing position l-index
	void *param;

	//	entities
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crCanvasNode *card_canvas = crFilterRenderManager::getInstance()->findCanvas(m_card);
	crData *card_data = card_canvas->getDataClass();
	CRCore::crCanvasNode *formation_canvas	= m_this->getParentCanvas();									// formation canvas
	CRCore::crData * canvas_data = formation_canvas->getDataClass();
	//canvas_data->getParam(WCHDATA_JXJForedCells, param);
	//int							*eqps			=	(int *)param;
	//canvas_data->getParam(WCHDATA_JXJForedHeadCells, param);
	//int							*heds			=	(int *)param;	
	crData *data = myPlayer->getDataClass();
	canvas_data->getParam(WCHDATA_JXJTempFormationVec, param);
	FormationInfoVec *tempformationvec = (FormationInfoVec *)param;
	ref_ptr<crFormationInfo> formationinfo = (*tempformationvec)[index];
	if(!formationinfo.valid()/* || formationinfo->getAbstractID() <= 0*/)
	{
		return;
	}

	data->excHandle(MAKEINT64(WCH_LockData, 1));
	card_data->excHandle(MAKEINT64(WCH_LockData, 1));
	card_data->getParam(WCHDATA_JXJForJKVec,param);
	JXJItemBagVec *data_card= (JXJItemBagVec *)param;
	data->getParam(WCHDATA_JXJJiangkaBagVec,param);
	JXJItemBagVec *jiangkabagvec= (JXJItemBagVec *)param;
	int cardcount = 0;
	crVector2i formationcard = formationinfo -> getEquips();
	for (int cc = 0; cc < 2; cc++)
	{
		if (formationcard[cc]>0)
		{
			cardcount++;
		}
	}
	if (data_card->size()>0)
	{
		for (JXJItemBagVec::iterator carditr = data_card->begin();
			carditr!=data_card->end();
			++carditr)
		{
			if ((*carditr) == NULL)
			{
				cardcount--;
				if (cardcount <= 0)
				{
					cardcount = 0;
				}
			}
		}
	}
	else
	{
		for (JXJItemBagVec::iterator carditr = jiangkabagvec->begin();
			carditr!=jiangkabagvec->end();
			++carditr)
		{
			if ((*carditr) == NULL)
			{
				cardcount--;
				if (cardcount <= 0)
				{
					cardcount = 0;
				}
			}
		}
	}
	if (cardcount > 0)
	{
		std::string str ="将卡背包已满，请先整理将卡背包！";
		crCanvasNode *tipcanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipcanvas);
		if (tipcanvas)
		{
			crStaticTextWidgetNode *tiptitle = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tiptitle));
			crStaticTextWidgetNode *tipcontent = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tipcontent));
			if (tipcontent)
			{
				tipcontent->setString(str);
			}
			crFilterRenderManager::getInstance()->showCanvas(tipcanvas,true);
		}
		card_data->excHandle(MAKEINT64(WCH_LockData, 0));
		data->excHandle(MAKEINT64(WCH_LockData, 0));
		return;
	}
	////
	int newindex = 0;
	for (int i = 0;i<tempformationvec->size();i++)
	{
		if(i!=index && (*tempformationvec)[i].valid() && (*tempformationvec)[i]->getAbstractID() > 0)
		{
			newindex = i;
			break;
		}
	}
	canvas_data->getParam(WCHDATA_JXJCurFormation1stCardIndex, param);
	int *firstcardindex = (int *)param;
	canvas_data->getParam(WCHDATA_JXJCurFormation2ndCardIndex, param);
	int *secondcardindex = (int *)param;
	if (*firstcardindex == index)
	{
		*firstcardindex = newindex;
	}
	if (*secondcardindex == index)
	{
		*secondcardindex = newindex;
	}
	/////
	int total_lead = atoi(dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_lead))
		->getUTF8String().c_str());
	int used_lead = atoi(dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_used))
		->getUTF8String().c_str());
	int remaind_lead = 0;
	int p_lead = 0;
	int p_count = 0;


	crStaticTextWidgetNode *p_lead_txt = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_plead));
	p_lead = atoi(p_lead_txt->getUTF8String().c_str());
	crStaticTextWidgetNode *used_troop = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_used));
	p_count = atoi(used_troop->getUTF8String().c_str());


	// clearing
	// arm head
	// ul-temp
	crImageBoxWidgetNode *arm_head = 
		dynamic_cast<crImageBoxWidgetNode *>(formation_canvas->getWidget(m_arm_head));
	arm_head->setVisiable(false);

	// equips
	crWidgetNode *eqp_head;
	for(int i = 0; i < 3; i++)
	{
		eqp_head = formation_canvas->getWidget(m_eqs[i]);
		if (eqp_head)
		{
			CRCore::crStateSet *state_set =
				dynamic_cast<CRCore::crObject *>(( dynamic_cast<CRCore::crGroup *>(eqp_head->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
			CRCore::crTexture2D *texture_2d = 
				dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
			if (i == 0)
			{
				texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic,texture_2d));
			}
			else
			{
				texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(m_pic2,texture_2d));
			}
			//eqp_head->setVisiable(false);
		}
		if (i<2)
		{
			crMultiSwitch *multiswitch =dynamic_cast<crMultiSwitch *>(formation_canvas->getChildNode(m_colorbox[index * 2 + i]));
			if (multiswitch)
			{
				multiswitch->setActiveSwitchSet(0);
			}	

			for (JXJItemBagVec::iterator itr = data_card->begin();itr!=data_card->end();++itr)
			{
				if ((*itr) == NULL)
				{
					ref_ptr<crBagItemData> bagitemdata = new crBagItemData;
					bagitemdata->setItemID(formationinfo->getEquips()[i]);
					bagitemdata->setItemCount(1);
					(*itr) = bagitemdata;
					break;
				}
			}
		}

		// remove card back	

		//if(heds[index * 3 + i] >= 0 && card_canvas->getVisiable())
		//{
		//	int index_pos = -1;
		//	bool find_flag = false;
		//	// 查找将卡背包数据 加1
		//	for(int i_i = 0; i_i < data_card->size(); i_i++)
		//	{
		//		if(!((*data_card)[i_i].get()))
		//		{
		//			if(index_pos < 0) index_pos = i;
		//			continue;
		//		}

		//		if((*data_card)[i_i].get()->getItemID() == heds[index * 3 + i])
		//		{
		//			find_flag = true;
		//			(*data_card)[i_i].get()->setItemCount((*data_card)[i_i].get()->getItemCount() + 1);
		//			break;
		//		}
		//	}

		//	// 没有找到 插入背包
		//	if(!find_flag && index_pos >= 0 && index_pos < data_card->size())
		//	{
		//		(*data_card)[index_pos] = new crBagItemData;
		//		(*data_card)[index_pos]->setItemID(heds[index * 3 + i]);
		//		(*data_card)[index_pos]->setItemCount(1);
		//	}
		//}
		//heds[index * 3 + i] = -1;
	}
	//formationinfo->setEquips(crVector2i(-1,-1));
	card_canvas->doEvent(WCH_UI_UpdateData);
	card_data->excHandle(MAKEINT64(WCH_LockData, 0));

	// inputs
	crStaticTextWidgetNode *txt_input = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_total));
	txt_input->clearString();
	txt_input = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_used));
	txt_input->clearString();
	txt_input = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_plead));
	txt_input->clearString();
	txt_input = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_num_ulead));
	txt_input->clearString();

	// slides
	crScrollBarWidgetNode *slide = 
		dynamic_cast<crScrollBarWidgetNode *>(formation_canvas->getWidget(m_slide));
	slide->outerSetValue(0.0f);
	slide->setVisiable(false);

	// calculate lead
	used_lead -= p_lead * p_count;
	remaind_lead = total_lead - used_lead;
	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_used))->setString(crArgumentParser::appItoa(used_lead));
	dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_remain))->setString(crArgumentParser::appItoa(remaind_lead));

	//formationinfo->setAbstractID(-1);
	//formationinfo->setCount(0);
	if (index < tempformationvec->size())
	{
		if((*tempformationvec)[index] != NULL)
		{
			(*tempformationvec)[index]->setAbstractID(0);
			(*tempformationvec)[index]->setEquips(crVector2i(0,0));
		}
	}
	crEditWidgetNode *editbox = dynamic_cast<crEditWidgetNode *>(formation_canvas->getWidget(m_editbox[index]));
	if (editbox)
	{
		editbox->clearString();
		editbox->setEnable(false);
	}
	data->excHandle(MAKEINT64(WCH_LockData, 0));
	//eqps[index] = -1;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnFormationArmAddMethod
// On  : 兵种添加
// Desp: 判断编制最大 并加入编制
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnFormationArmAddMethod::crJXJUIOnFormationArmAddMethod():
	m_this(NULL),
	m_delayHideWidgetTime(0)
{}
JXJ::crJXJUIOnFormationArmAddMethod::crJXJUIOnFormationArmAddMethod(const crJXJUIOnFormationArmAddMethod& handle):
	m_title_lead(handle.m_title_lead),
	m_title_used(handle.m_title_used),
	m_title_remain(handle.m_title_remain),
	m_tip(handle.m_tip),
	m_tip_title(handle.m_tip_title),
	m_formationlist(handle.m_formationlist),
	m_delayHideWidgetTime(handle.m_delayHideWidgetTime),
	m_taskCanvas(handle.m_taskCanvas)
{
	for (int i = 0; i <8 ; i++)
	{
		m_left_heads[i] = handle.m_left_heads[i];
		m_left_names[i] = handle.m_left_names[i];
		m_left_nums[i] = handle.m_left_nums[i];
	}
	for (int i = 0; i <3 ; i++)
	{
		m_right_heads[i] = handle.m_right_heads[i];
		m_right_trp_total[i] = handle.m_right_trp_total[i];
		m_right_trp_used[i] = handle.m_right_trp_used[i];
		m_right_trp_plead[i] = handle.m_right_trp_plead[i];
		m_right_trp_ulead[i] = handle.m_right_trp_ulead[i];
		m_right_slide[i] = handle.m_right_slide[i];
		m_editbox[i] = handle.m_editbox[i];
	}
	for (int i = 0; i <9 ; i++)
	{
		m_right_eqs[i] = handle.m_right_eqs[i];
	}
}
void JXJ::crJXJUIOnFormationArmAddMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
			m_ea = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;

	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmAddMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_left_heads[0] = str;
		break;
	case 1:
		m_left_heads[1] = str;
		break;
	case 2:
		m_left_heads[2] = str;
		break;
	case 3:
		m_left_heads[3] = str;
		break;
	case 4:
		m_left_heads[4] = str;
		break;
	case 5:
		m_left_heads[5] = str;
		break;
	case 6:
		m_left_heads[6] = str;
		break;
	case 7:
		m_left_heads[7] = str;
		break;
	case 8:
		m_left_names[0] = str;
		break;
	case 9:
		m_left_names[1] = str;
		break;
	case 10:
		m_left_names[2] = str;
		break;
	case 11:
		m_left_names[3] = str;
		break;
	case 12:
		m_left_names[4] = str;
		break;
	case 13:
		m_left_names[5] = str;
		break;
	case 14:
		m_left_names[6] = str;
		break;
	case 15:
		m_left_names[7] = str;
		break;
	case 16:
		m_left_nums[0] = str;
		break;
	case 17:
		m_left_nums[1] = str;
		break;
	case 18:
		m_left_nums[2] = str;
		break;
	case 19:
		m_left_nums[3] = str;
		break;
	case 20:
		m_left_nums[4] = str;
		break;
	case 21:
		m_left_nums[5] = str;
		break;
	case 22:
		m_left_nums[6] = str;
		break;
	case 23:
		m_left_nums[7] = str;
		break;
	case 24:
		m_title_lead = str;
		break;
	case 25:
		m_title_used = str;
		break;
	case 26:
		m_title_remain = str;
		break;
	case 27:
		m_right_heads[0] = str;
		break;
	case 28:
		m_right_heads[1] = str;
		break;
	case 29:
		m_right_heads[2] = str;
		break;
	case 30:
		m_right_trp_total[0] = str;
		break;
	case 31:
		m_right_trp_total[1] = str;
		break;
	case 32:
		m_right_trp_total[2] = str;
		break;
	case 33:
		m_right_trp_used[0] = str;
		break;
	case 34:
		m_right_trp_used[1] = str;
		break;
	case 35:
		m_right_trp_used[2] = str;
		break;
	case 36:
		m_right_trp_plead[0] = str;
		break;
	case 37:
		m_right_trp_plead[1] = str;
		break;
	case 38:
		m_right_trp_plead[2] = str;
		break;
	case 39:
		m_right_trp_ulead[0] = str;
		break;
	case 40:
		m_right_trp_ulead[1] = str;
		break;
	case 41:
		m_right_trp_ulead[2] = str;
		break;
	case 42:
		m_right_eqs[0] = str;
		break;
	case 43:
		m_right_eqs[1] = str;
		break;
	case 44:
		m_right_eqs[2] = str;
		break;
	case 45:
		m_right_eqs[3] = str;
		break;
	case 46:
		m_right_eqs[4] = str;
		break;
	case 47:
		m_right_eqs[5] = str;
		break;
	case 48:
		m_right_eqs[6] = str;
		break;
	case 49:
		m_right_eqs[7] = str;
		break;
	case 50:
		m_right_eqs[8] = str;
		break;
	case 51:
		m_right_slide[0] = str;
		break;
	case 52:
		m_right_slide[1] = str;
		break;
	case 53:
		m_right_slide[2] = str;
		break;
	case 54:
		m_tip = str;
		break;
	case 55:
		m_tip_title = str;
		break;
	case 56:
		m_editbox[0] = str;
		break;
	case 57:
		m_editbox[1] = str;
		break;
	case 58:
		m_editbox[2] = str;
		break;
	case 59:
		m_index = atoi(str.c_str());
		break;
	case 60:
		m_formationlist = str;
		break;
	case 61:
		m_delayHideWidgetTime = atoi(str.c_str());
		break;
	case 62:
		m_taskCanvas = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationArmAddMethod::operator()(crHandle &handle)
{
	// global usage varialbe
	int									index = 1;	// dealing position
	void								*param;		// parameter extract
	CREncapsulation::crTableIO::StrVec	record;		// record extracting
	int									troops_count = 0; // col and row indexing
	int									troop_lead = 0;

	// entities
	CRCore::crCanvasNode			*formation_canvas	= m_this->getParentCanvas(); // formation canvas entity
	CRNetApp::crPlayerGameData		*player				= CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	
	// datas
	CRCore::crData					*canvas_data		= formation_canvas->getDataClass();
	canvas_data->getParam(WCHDATA_JXJArmsCells, param);
	int								*arms				= (int *)param;
	//canvas_data->getParam(WCHDATA_JXJForedCells, param);
	//int								*eqps				= (int *)param;
	canvas_data->getParam(WCHDATA_JXJTempFormationVec, param);
	FormationInfoVec *tempformation	= (FormationInfoVec *)param;
	CRCore::crData					*player_data		= player->getDataClass();
	player_data->getParam(WCHDATA_JXJTroopsMap, param);
	TroopsMap						*troops				= (TroopsMap *)param;														// player troop
	// tables
	ref_ptr<CREncapsulation::crTableIO>troop_table		= CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
	int lead_index = troop_table->getTitleIndex("领导力");
	int iconindex = troop_table->getTitleIndex("icon");
	crListControlWidgetNode *formationlist = dynamic_cast<crListControlWidgetNode *>(formation_canvas->getWidget(m_formationlist));
	int abstractid = 0;
	if(formationlist && formationlist->getSelectNode())abstractid = formationlist->getSelectNode()->getData();
	////////////////////////////////////////////////统率影响单条领导力
	crRole *role = player->getMainRole();
	float totletongshuai = 0.0f;
	int permaxlead = 0;//(int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLeadPerLine).c_str()));
	crVector3f factor;
	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),factor);
	role->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
	permaxlead = totletongshuai * factor[0] + factor[1];
	////////////////////////////////////////////////////////////////////
	// check add - select alid
	//if(arms[m_index] < 0)
	//{
	//	return;
	//}
	if (abstractid <= 0)
	{
		return;
	}
	// check add - insert valid
	//for(index = 0; index < 3; index++)
	//{
	//	if(eqps[index] < 0)
	//	{
	//		break;
	//	}
	//}
	index = 0;
	crWidgetNode *pick_item = NULL;
	if (m_ea)
	{
		crVector2 mouse(m_ea->getXnormalized(), m_ea->getYnormalized());
		crWidgetNode *capturer = crFilterRenderManager::getInstance()->getMouseCapturer();
		crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
		crFilterRenderManager::getInstance()->mousePick(mouse, pick_item);
		crFilterRenderManager::getInstance()->setMouseCapturer(capturer);
	}
	//crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
	//crWidgetNode *pick_item = NULL;
	//crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
	//crFilterRenderManager::getInstance()->mousePick(mouse, pick_item);
	//crFilterRenderManager::getInstance()->setMouseCapturer(m_this);
	//if(!pick_item) break;
	bool pickicon = false;
	unsigned char linelv[2];
	crData *mainroledata = role->getDataClass();
	mainroledata->getParam(WCHDATA_Level,param);
	unsigned char lordlv = *(unsigned char *)param;
	linelv[0] = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation1stRowOpenLv,viplv).c_str());
	linelv[1] = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation2ndRowOpenLv,viplv).c_str());
	unsigned char maxline = 1;
	for (int i = 0;i < 2; ++i)
	{
		if (lordlv >= linelv[i])
		{
			maxline++;
		}
	}
	for (int i = 0; i < maxline; i++)
	{
		if(pick_item && pick_item->getName() == m_right_heads[i])
		{
			pickicon = true;
			index = i;
			if (index < tempformation->size())
			{
				//equips = (*tempformation)[index]->getEquips();
				//ref_ptr<crFormationInfo> newformation = new crFormationInfo;
				//newformation->setAbstractID(abstractid);
				//newformation->setEquips(equips);
				//(*tempformation)[index] = newformation;
				(*tempformation)[index]->setAbstractID(abstractid);
				(*tempformation)[index]->setCount(0);
			}
		}
	}
	if (!pickicon)
	{
		for(FormationInfoVec::iterator itr = tempformation->begin(); itr!=tempformation->end(); itr++)
		{
			if ((*itr)!=NULL)
			{
				if ((*itr)->getAbstractID()>0)
				{
					index++;
				}
				else
				{
					//(*itr)->setAbstractID(arms[m_index]);
					(*itr)->setAbstractID(abstractid);
					(*itr)->setCount(0);
					break;
				}
			}
			else
			{
				ref_ptr<crFormationInfo> newformation = new crFormationInfo;
				//newformation->setAbstractID(arms[m_index]);
				newformation->setAbstractID(abstractid);
				newformation->setCount(0);
				(*tempformation)[index] = newformation;
				break;
			}
		}

		if(index >= maxline)
		{
			CRCore::crCanvasNode *note_canvas = dynamic_cast<CRCore::crCanvasNode *>(CRCore::crFilterRenderManager::getInstance()->findCanvas(m_tip));
			if (note_canvas)
			{
				//CRCore::crFilterRenderManager::getInstance()->doModal(note_canvas);
				crStaticTextWidgetNode *note = dynamic_cast<crStaticTextWidgetNode *>(note_canvas->getWidget(m_tip_title));
				if (note)
				{
					note->setString(std::string("编制已满，请先移除编队后重新加入！"));
					note->setVisiable(true);
				}
				ref_ptr<crCanvasNode> manorTaskBar_cavas = crFilterRenderManager::getInstance()->findCanvas(m_taskCanvas);
				if (manorTaskBar_cavas.valid())
				{
					const crBoundingBox &tipsbox = note_canvas->getBoundBox();
					const crBoundingBox &taskCavasbox = manorTaskBar_cavas->getBoundBox();
					note_canvas->setMatrix(crMatrix::translate(taskCavasbox.xMin() + tipsbox.xLength()*0.5f, taskCavasbox.yMax() - tipsbox.yLength()*0.5f, 0.0f));
					crFilterRenderManager::getInstance()->showCanvas(note_canvas,true);
				}
				crData *braindata = crBrain::getInstance()->getDataClass();
				braindata->inputParam(WCHDATA_JXJDelayHideWidgetTime, &m_delayHideWidgetTime);
			}
			return;
		}
	}

	// 
	//eqps[index] = arms[m_index];

	// get arm information
	if(troop_table->queryOneRecord(0, crArgumentParser::appItoa(abstractid/*arms[m_index]*/), record) < 0)
	{
		return;
	}

	// get player troop
	for(TroopsMap::iterator itr = troops->begin(); itr != troops->end(); ++itr)
	{
		if(itr->first == abstractid/*arms[m_index]*/)
		{
			troops_count = itr->second;
			break;
		}
	}

	// troop in formation
	player_data->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec		*troop_infors		= (FormationInfoVec *)param;
	for(FormationInfoVec::iterator itr = troop_infors->begin(); itr != troop_infors->end(); itr++)
	{
		if((*itr).get()->getAbstractID() == abstractid/*arms[m_index]*/)
		{
			troops_count += (*itr).get()->getCount();
		}
	}
	if (troops_count < 0)
	{
		return;
	}

	troop_lead = atoi(record[lead_index].c_str());

	if (troop_lead < 0)
	{
		return;
	}

	crEditWidgetNode *editbox = dynamic_cast<crEditWidgetNode *>(formation_canvas->getWidget(m_editbox[index]));
	if(editbox)
	{
		editbox->setEnable(true);
		editbox->setString(std::string("0"));
	}
	// insert head info
	crImageBoxWidgetNode *head_box = 
		dynamic_cast<crImageBoxWidgetNode *>(formation_canvas->getWidget(m_right_heads[index]));
	if(head_box)head_box->setImageName(record[iconindex]);
	if(head_box)head_box->setVisiable(true);

	int temptrooptotal = permaxlead / troop_lead;
	if (temptrooptotal > troops_count)
	{
		temptrooptotal = troops_count;
	}
	// set num
	crStaticTextWidgetNode *input_1 = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_total[index]));
	if(input_1)input_1->setString(crArgumentParser::appItoa(temptrooptotal));
	if(input_1)input_1->setVisiable(true);
	crStaticTextWidgetNode *input_2 = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[index]));
	if(input_2)input_2->setString(std::string("0"));
	if(input_2)input_2->setVisiable(true);

	crStaticTextWidgetNode *input_3 = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_plead[index]));
	if(input_3)input_3->setString(crArgumentParser::appItoa(troop_lead));
	if(input_3)input_3->setVisiable(true);

	crStaticTextWidgetNode *input_4 = 
		dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_ulead[index]));
	if(input_4)input_4->setString(std::string("0"));
	if(input_4)input_4->setVisiable(true);

	// slide
	//crScrollBarWidgetNode *slider = dynamic_cast<crScrollBarWidgetNode*>(formation_canvas->getWidget(m_right_slide[index]));
	//if (slider)
	//{
		//slider->setRange(0.0f, temptrooptotal);
		//slider->setVisiable(0.0f);
		//slider->setVisiable(true);
	//}

	// equips
	crWidgetNode *eqp_head;
	for(int i = 0; i < 3; i++)
	{
		eqp_head = formation_canvas->getWidget(m_right_eqs[index * 3 + i]);
		if(eqp_head)eqp_head->setVisiable(true);
	}
	canvas_data->inputParam(WCHDATA_JXJCurFormation1stCardIndex, &index);
	canvas_data->inputParam(WCHDATA_JXJCurFormation2ndCardIndex, &index);
	//crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnFormationStoreMethod
// On  : 编制保存
// Desp: 获取数据保存
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnFormationStoreMethod::crJXJUIOnFormationStoreMethod():
	m_index(0)
{}
JXJ::crJXJUIOnFormationStoreMethod::crJXJUIOnFormationStoreMethod(const crJXJUIOnFormationStoreMethod& handle):
	m_pageradio(handle.m_pageradio),
	m_index(handle.m_index)
{
	for (int i = 0; i < 3; ++i)
	{
		m_right_trp_used[i] = handle.m_right_trp_used[i];
	}
}
void JXJ::crJXJUIOnFormationStoreMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;

	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationStoreMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_right_trp_used[0] = str;
		break;
	case 1:
		m_right_trp_used[1] = str;
		break;
	case 2:
		m_right_trp_used[2] = str;
		break;
	case 3:
		m_pageradio = str;
		break;
	case 4:
		m_index = atoi(str.c_str());
		break;
	case 5:
	case 6:
	case 7:
	case 8:
		m_nameedit[i - 5] = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnFormationStoreMethod::operator()(crHandle &handle)
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// global
	bool is_cancel = true;

	// temp
	int									index = 1;			// dealing position
	void								*param;				// parameter extract
	CREncapsulation::crTableIO::StrVec	record;				// record extracting
	int									index_to_col = 1;	// col and row indexing

	// datas
	CRCore::crCanvasNode			*formation_canvas	= m_this->getParentCanvas();			
	CRCore::crData					*canvas_data		= formation_canvas->getDataClass();										// canvas
	CRNetApp::crPlayerGameData		*player				= CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	CRCore::crData					*player_data		= player->getDataClass();										// player data
	CRCore::crData					*player_role_data	= player->getMainRole()->getDataClass();						// player role data
	CRNet::crNetConductor			*netConductor		= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);

	// tables
	player_data->getParam(WCHDATA_JXJTroopsMap, param);
	TroopsMap *troops =	(TroopsMap *)param;														// player troop
	canvas_data->getParam(WCHDATA_JXJArmsCells, param);
	int	 *arms = (int *)param;															// ui arms flag												// player troop
	canvas_data->getParam(WCHDATA_JXJTempFormationVec, param);
	FormationInfoVec *tempformationvec = (FormationInfoVec *)param;	
	//canvas_data->getParam(WCHDATA_JXJForedCells, param);
	//int							*eqps			=	(int *)param;
	//canvas_data->getParam(WCHDATA_JXJForedHeadCells, param);
	//int							*heds			=	(int *)param;
	ref_ptr<CREncapsulation::crTableIO>troop_table	=	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
	ref_ptr<CREncapsulation::crTableIO>gov_table		=	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab); // govecap look-up table
	ref_ptr<CREncapsulation::crTableIO>equip_table	=	CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);	// equips look-up table
	
	// safe check
	if(!player || !netConductor || !player_data) return;

	// package data
	// create stream buffer
	unsigned char formationvecsize = 0;
	crStaticTextWidgetNode *input_1 = NULL;
	std::string pagename;
	char selectpage = -1;
	unsigned char battleselectpage = 0;
	crRadioGroupWidgetNode *pageradio = dynamic_cast<crRadioGroupWidgetNode *>(formation_canvas->getWidget(m_pageradio));
	if (pageradio)
	{
		if (m_index == 1)
		{
			selectpage = pageradio->getSelect();
			crEditWidgetNode *nameedit = dynamic_cast<crEditWidgetNode *>(formation_canvas->getWidget(m_nameedit[selectpage]));
			if (nameedit)
			{
				pagename = nameedit->getUTF8String();
			}
		}
		else
		{
			battleselectpage = pageradio->getSelect();
		}
	}
	std::string countstr;
	int finds;
	for(int i = 0; i < 3; i++)
	{
		if ((*tempformationvec)[i]!=NULL)
		{
			formationvecsize++;
			input_1 = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[i]));
			if (input_1)
			{
				countstr = input_1->getUTF8String();
				finds = countstr.find("/");
				if (finds >= 0)
				{
					countstr.erase(finds,countstr.size());
				}
				(*tempformationvec)[i]->setCount(atoi(countstr.c_str()));
			}
		}
		
	}
	//for(int i = 0; i < 3; i++) eqp_count = eqps[i] >= 0 ? eqp_count + 1 : eqp_count;
	CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
	if (selectpage >= 0)
	{
		stream->createBuf(formationvecsize * 14 + 6 + pagename.size());
	}
	else
	{
		stream->createBuf(formationvecsize * 14 + 3);
	}
	stream->_writeChar(selectpage);
	if (selectpage >= 0)
	{
		stream->_writeString(pagename);
	}
	else
	{
		stream->_writeUChar(battleselectpage);
	}
	stream->_writeUChar(formationvecsize);

	// input data
	for(int i = 0; i < 3; i++)
	{
		if((*tempformationvec)[i]==NULL/* || (*tempformationvec)[i]->getAbstractID() < 0*/) continue;

		// - - - - - - - - - - - - - - - - - - - - -
		// packet format
		// arm id
		// arm count
		// equip 1 id
		// equip 2 id
		// equip 3 id
		//CREncapsulation::crStaticTextWidgetNode *input_1 = 
		//	dynamic_cast<CREncapsulation::crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[i]));
		if ((*tempformationvec)[i]->getAbstractID() <= 0)
		{
			stream->_writeInt(0);
			stream->_writeUShort(0);
		}
		else
		{

			stream->_writeInt((*tempformationvec)[i]->getAbstractID());
			stream->_writeUShort((*tempformationvec)[i]->getCount());
		}
		stream->_writeVec2i((*tempformationvec)[i]->getEquips());
	}

	CRNetApp::crPlayerDataEventPacket packet;
	CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFormation,stream.get());
	netConductor->getNetManager()->sendPacket("all",packet);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnFormationStoreRecvMethod
// On  : 发包处理
// Desp: 获取数据保存，对照，保存
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
crJXJUIOnFormationStoreRecvMethod::crJXJUIOnFormationStoreRecvMethod()
{}
crJXJUIOnFormationStoreRecvMethod::crJXJUIOnFormationStoreRecvMethod(const crJXJUIOnFormationStoreRecvMethod& handle)
{}
void crJXJUIOnFormationStoreRecvMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (CRNetApp::crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (CRCore::crStreamBuf *)(LOINT64(param64));
			m_net_type = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJUIOnFormationStoreRecvMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formation_canvas = str;
		break;
	case 1:
		m_jkcnavase = str;
		break;
	default:
		break;
	}
}
void crJXJUIOnFormationStoreRecvMethod::operator()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	if(m_net_type == CRNetApp::GameServer)
	{
		int player_id = m_this->getPlayerID();
		CRCore::crData	*player_data		= m_this->getDataClass();
		CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(CRNetApp::GameServer); 		
		ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(player_id));
		if(!player_net_data.valid() || player_net_data->getSceneID()!=0)//已经准备要进入战场的时候不能改变编队
			return;
		crRole* mainrole = m_this->getMainRole();
		if(!mainrole)
			return;
		crData *mainroledata = mainrole->getDataClass();
		if(!mainroledata)
			return;
		void *param;
		unsigned char battleselectpage = 0;
		int leadnum;
		int leadmaxnum = 0;
		char is_valid = 0; // 检测是否通过
		// error code : 
		//		-1. 兵种信息出错
		//		-2. 兵种数量超额
		//		-3. 领导力超出
		//		-4. 将卡出错
		//      -5;//将卡背包已满
		FormationInfoVec formationtemp;
		std::map<int,int>ArmyNeed;
		std::map<int,int>EquipNeed;
		std::map<int,int>::iterator tmpitr;
		
		ref_ptr<crFormationInfo> formationinfo;
		crTableIO::StrVec	record,recordpost;
		ref_ptr<crTableIO>troop_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		ref_ptr<crTableIO>postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		int leadmaxindex = postTab->getTitleIndex("领导力");
		int leadindex = troop_table->getTitleIndex("领导力");
		char pageselect = m_stream->_readChar();
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int nameindex = itemtab->getTitleIndex("name");
		std::string pagename;
		player_data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		if (pageselect >= 0)
		{
			pagename = m_stream -> _readString();
		}
		else
		{
			battleselectpage = m_stream->_readUChar();
		}
		unsigned char formationsize = m_stream->_readUChar();

		unsigned char linelv[2] = {0};
		mainroledata->getParam(WCHDATA_Level,param);
		unsigned char lordlv = *(unsigned char *)param;
		linelv[0] = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation1stRowOpenLv,viplv).c_str());
		linelv[1] = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormation2ndRowOpenLv,viplv).c_str());
		unsigned char maxline = 1;
		for (int i = 0;i < 2; ++i)
		{
			if (lordlv >= linelv[i])
			{
				maxline++;
			}
		}
		if (formationsize > maxline)
		{
			formationsize = maxline;
		}
		////////////////////////////////////////////////单条领导力 = 总领导力/编队条数（3）
		unsigned short formationcount = 0;
		//float totletongshuai = 0.0f;
		int permaxlead = 0;//(int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLeadPerLine).c_str()));
		//crVector3f factor;
		//crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),factor);
		unsigned char formationmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxFormation,viplv).c_str()));
		//mainrole->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
		//permaxlead = totletongshuai * factor[0] + factor[1];
		int totalleaderpower = 0;
		m_this->doEvent(WCH_JXJFormationGetLeaderPower,MAKEINT64(&totalleaderpower,NULL));
		if (formationmax == 0)
		{
			formationmax = 1;
		}
		permaxlead = totalleaderpower / formationmax;
		for (int i = 0; i < (int)formationsize;i++)
		{
			formationinfo = new crFormationInfo;
			formationinfo->setAbstractID(m_stream->_readInt());
			// 兵种信息查找
			if(formationinfo->getAbstractID()>0 && troop_table->queryOneRecord(0, crArgumentParser::appItoa(formationinfo->getAbstractID()), record) < 0)
			{//兵种信息出错
				is_valid = -1;
				break;
			}
			leadnum = 0;
			formationcount = m_stream->_readUShort();
			formationinfo->setCount(formationcount);
			if(formationinfo->getAbstractID()>0 && formationcount>0)
			{
				//单队领导力检测
				leadnum = atoi(record[leadindex].c_str()) * formationcount;
				if (leadnum > permaxlead)
				{
					is_valid = -3;
					break;
				}
				leadmaxnum+=leadnum;
			}
			formationinfo->setEquips(m_stream->_readVec2i());
			formationtemp.push_back(formationinfo);
			if(formationinfo->getAbstractID()>0)
			{
				tmpitr = ArmyNeed.find(formationinfo->getAbstractID());
				if(tmpitr != ArmyNeed.end())
				{
					tmpitr->second += formationinfo->getCount();
				}
				else
				{
					ArmyNeed[formationinfo->getAbstractID()] = formationinfo->getCount();
				}
			}
			if(formationinfo->getEquips()[0]>0)
			{
				tmpitr = EquipNeed.find(formationinfo->getEquips()[0]);
				if(tmpitr != EquipNeed.end())
				{
					tmpitr->second += 1;
				}
				else
				{
					EquipNeed[formationinfo->getEquips()[0]] = 1;
				}
			}
			if(formationinfo->getEquips()[1]>0)
			{
				tmpitr = EquipNeed.find(formationinfo->getEquips()[1]);
				if(tmpitr != EquipNeed.end())
				{
					tmpitr->second += 1;
				}
				else
				{
					EquipNeed[formationinfo->getEquips()[1]] = 1;
				}
			}
		}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		if(is_valid==0)
		{
			///总领导力检查
			if (leadmaxindex > 0)
			{
				if (leadmaxnum > totalleaderpower)
				{
					is_valid = -3;
				}
			}
			//mainroledata->getParam(WCHDATA_JXJGovPost, param);
			//if(leadmaxindex > 0 && postTab->queryOneRecord(0, crArgumentParser::appItoa(*((unsigned char*)param)), recordpost))
			//{
			//	if(leadmaxnum > atoi(recordpost[leadmaxindex].c_str()))
			//	{
			//		is_valid = -3;
			//	}
			//}
		}
		if(is_valid!=0)
		{
			stream->createBuf(1);
			stream->_writeChar(is_valid);
			CRNetApp::crPlayerDataEventPacket packet;
			CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet,player_id,WCH_JXJRecvFormation,stream.get());
			server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
			return;
		}

		///////////////////////////////
		// data area
		player_data->excHandle(MAKEINT64(WCH_LockData,1));
		// tables
		// 编队信息
		// 兵种数量
		// 兵种信息表
		// 将卡背包数据
		// 官阶数据表
		FormationInfoVec formationinfovec;
		player_data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *troop_infors = (FormationInfoVec *)param;
		player_data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
		SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;
		player_data->getParam(WCHDATA_JXJTroopsMap,param);
		TroopsMap *troops = (TroopsMap*)param;
		if (pageselect == -1)
		{
			for (FormationInfoVec::iterator itr = troop_infors->begin();
				itr != troop_infors->end();
				++itr)
			{
				formationinfovec.push_back(*itr);
			}
		}
		else
		{
			if (pageselect >= 0 && pageselect < savedtroop_infors->size())
			{
				(*savedtroop_infors)[pageselect].first = pagename;
				for (FormationInfoVec::iterator itr = (*savedtroop_infors)[pageselect].second.begin();
					itr != (*savedtroop_infors)[pageselect].second.end();
					++itr)
				{
					formationinfovec.push_back(*itr);
				}
			}

		}
		//CREncapsulation::ref_ptr<crTableIO>troop_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
		//int lead_index = troop_table->getTitleIndex("领导力");
		player_data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *playerCardVec = (JXJItemBagVec *)param;
		//CREncapsulation::ref_ptr<crTableIO>gov_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab); // govecap look-up table

		std::map<int,int>ArmyOld;
		std::map<int,int>EquipOld;
		for( FormationInfoVec::iterator itr = formationinfovec.begin();
			 itr != formationinfovec.end();
			 ++itr )
		{
			formationinfo = itr->get();
			if(formationinfo->getAbstractID()>0)
			{
				tmpitr = ArmyOld.find(formationinfo->getAbstractID());
				if(tmpitr != ArmyOld.end())
				{
					tmpitr->second += formationinfo->getCount();
				}
				else
				{
					ArmyOld[formationinfo->getAbstractID()] = formationinfo->getCount();
				}
			}
			if(formationinfo->getEquips()[0]>0)
			{
				tmpitr = EquipOld.find(formationinfo->getEquips()[0]);
				if(tmpitr != EquipOld.end())
				{
					tmpitr->second += 1;
				}
				else
				{
					EquipOld[formationinfo->getEquips()[0]] = 1;
				}
			}
			if(formationinfo->getEquips()[1]>0)
			{
				tmpitr = EquipOld.find(formationinfo->getEquips()[1]);
				if(tmpitr != EquipOld.end())
				{
					tmpitr->second += 1;
				}
				else
				{
					EquipOld[formationinfo->getEquips()[1]] = 1;
				}
			}
		}
		/////兵种数量检查
		std::map<int, int> NewTroops;//新的预备部队信息变动
		std::map<int, int>::iterator armoldItr, ntItr;
		do 
		{
			std::vector<int> RemoveCardIndex;//要从将卡背包里移除的index
			std::vector<int> InsertCardID;//要从放入将卡背包的ID
			TroopsMap::iterator titr;
			int index = 0;
			int needcount = 0;
			for( tmpitr = ArmyNeed.begin();
				tmpitr != ArmyNeed.end();
				++tmpitr )
			{
				needcount = tmpitr->second;
				//现役
				armoldItr = ArmyOld.find(tmpitr->first);
				if(armoldItr != ArmyOld.end())
				{
					if(armoldItr->second>=needcount)
					{
						NewTroops[tmpitr->first] = armoldItr->second - needcount;
						ArmyOld.erase(armoldItr);
						continue;
					}
					else
					{
						needcount -= armoldItr->second;
						ArmyOld.erase(armoldItr);
					}
				}
				//预备役
				titr = troops->find(tmpitr->first);
				if(titr != troops->end())
				{
					if(titr->second>=needcount)
					{
						NewTroops[tmpitr->first] = -needcount;
						continue;
					}
					else
					{
						NewTroops[tmpitr->first] = titr->second;
						is_valid = -2;//兵种数量超额
						//break;
					}
				}
				else
				{
					NewTroops[tmpitr->first] = 0;
					is_valid = -2;//兵种数量超额
					//break;
				}
			}
			////////////////////////
			if(is_valid!=0)
				break;
			for( armoldItr = ArmyOld.begin();
				armoldItr != ArmyOld.end();
				++armoldItr )
			{
				ntItr = NewTroops.find(armoldItr->first);
				if(ntItr == NewTroops.end())
				{
					NewTroops[armoldItr->first] = armoldItr->second;
				}
				else
				{
					ntItr->second += armoldItr->second;
				}
			}
			//////////////将卡检查
			crTableIO::StrVec record;
			JXJItemBagVec::iterator jkItr;
			std::map<int,int>::iterator jkoldItr;
			std::vector<std::string> cardnamevec;
			std::vector<std::string>::iterator nameitr;
			std::string cardname;
			bool cardnamesame = false;
			needcount = 0;
			int remaincard;
			for( tmpitr = EquipNeed.begin();
				tmpitr != EquipNeed.end();
				++tmpitr )
			{
				//检测同名将卡
				if (tmpitr->second > 1)
				{
					cardnamesame = true;
				}
				else if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(tmpitr->first), record) >= 0)
				{
					cardname = record[nameindex];
					for (nameitr = cardnamevec.begin();
						nameitr != cardnamevec.end();
						++nameitr)
					{
						if ((*nameitr).compare(cardname) == 0)
						{
							cardnamesame = true;
							break;
						}
						
					}
				}
				if (cardnamesame)
				{
					is_valid = -6;//存在同名将卡
					break;
				}
				else
				{
					cardnamevec.push_back(cardname);
				}
				////////////////////////////////
				needcount = tmpitr->second;
				//现役
				jkoldItr = EquipOld.find(tmpitr->first);
				if(jkoldItr != EquipOld.end())
				{
					if(jkoldItr->second>=needcount)
					{
						remaincard = jkoldItr->second - needcount;
						for (int i=0; i<remaincard; i++)
						{
							InsertCardID.push_back(tmpitr->first);
						}
						EquipOld.erase(jkoldItr);
						continue;
					}
					else
					{
						needcount -= jkoldItr->second;
						EquipOld.erase(jkoldItr);
					}
				}
				//将卡背包
				index = 0;
				for( jkItr = playerCardVec->begin();
					 jkItr != playerCardVec->end();
					 ++jkItr,++index )
				{
					if(jkItr->valid() && (*jkItr)->getItemID() == tmpitr->first)
					{
						needcount -= (*jkItr)->getItemCount();
						//将卡不可叠加 (*jkItr)->getItemCount()=1
						RemoveCardIndex.push_back(index);
						if(needcount<=0)
							break;
					}
				}
				if(needcount>0)
				{
					is_valid = -4;//缺少将卡
					break;
				}
			}
			if(is_valid!=0)
				break;
			for( jkoldItr = EquipOld.begin();
				jkoldItr != EquipOld.end();
				++jkoldItr )
			{
				for (int i=0; i<jkoldItr->second; i++)
				{
					InsertCardID.push_back(jkoldItr->first);
				}
			}
			//检查将卡背包空余数量
			int remaincount = 0;
			for( jkItr = playerCardVec->begin();
				jkItr != playerCardVec->end();
				++jkItr,++index )
			{
				if(!jkItr->valid())
				{
					remaincount++;
				}
			}
			remaincount += RemoveCardIndex.size();
			if(InsertCardID.size()>remaincount)
			{
				is_valid = -5;//将卡背包已满
				break;
			}
			if (pageselect >= 0)
			{
				stream->createBuf(7+NewTroops.size()*8+RemoveCardIndex.size()+InsertCardID.size()*4+formationtemp.size()*14 + 4 + pagename.size());
			}
			else
			{
				stream->createBuf(7+NewTroops.size()*8+RemoveCardIndex.size()+InsertCardID.size()*4+formationtemp.size()*14);
			}
			stream->_writeChar(0);
			stream->_writeChar(pageselect);
			if (pageselect >= 0)
			{
				stream->_writeString(pagename);
			}
			///预备部队数量变化
			stream->_writeUChar(NewTroops.size());
			for( ntItr = NewTroops.begin();
				 ntItr != NewTroops.end();
				 ++ntItr )
			{
				titr = troops->find(ntItr->first);
				stream->_writeInt(ntItr->first);
				if(titr != troops->end())
				{
					titr->second += ntItr->second;
					stream->_writeInt(titr->second);
				}
				else
				{
					(*troops)[ntItr->first] = ntItr->second;
					stream->_writeInt(ntItr->second);
				}
			}
			//将卡背包删除
			std::vector<int>::iterator vItr;
			stream->_writeUChar(RemoveCardIndex.size());
			for( vItr = RemoveCardIndex.begin();
				 vItr != RemoveCardIndex.end();
				 ++vItr )
			{
				(*playerCardVec)[*vItr] = NULL;
				stream->_writeUChar(*vItr);
			}
			//放入将卡背包
			stream->_writeUChar(InsertCardID.size());
			for( vItr = InsertCardID.begin();
				vItr != InsertCardID.end();
				++vItr )
			{
				for( jkItr = playerCardVec->begin();
					jkItr != playerCardVec->end();
					++jkItr )
				{
					if(!(jkItr->valid()))
					{
						*jkItr = new crBagItemData;
						(*jkItr)->setItemID(*vItr);
						(*jkItr)->setItemCount(1);
						break;
					}
				}
				stream->_writeInt(*vItr);
			}
			stream->_writeUChar(formationtemp.size());
			for( FormationInfoVec::iterator itr = formationtemp.begin();
				itr != formationtemp.end();
				++itr )
			{
				formationinfo = itr->get();
				stream->_writeInt(formationinfo->getAbstractID());
				stream->_writeUShort(formationinfo->getCount());
				stream->_writeVec2i(formationinfo->getEquips());
			}
			//条件满足，执行编队
			if (pageselect == -1)
			{
				player_data->inputParam(WCHDATA_JXJSavedFormationIndex,&battleselectpage);
				stream->_writeUChar(battleselectpage);
				troop_infors->swap(formationtemp);
			}
			else
			{
				player_data->inputParam(WCHDATA_JXJSavedFormationIndex,&pageselect);
				stream->_writeUChar(pageselect);
				ref_ptr<crFormationInfo> newformation = NULL; 
				if (pageselect >= 0 && pageselect < savedtroop_infors->size())
				{
					troop_infors->clear();
					(*savedtroop_infors)[pageselect].second.clear();
					for( FormationInfoVec::iterator itr = formationtemp.begin();
						itr != formationtemp.end();
						++itr )
					{
						formationinfo = itr->get();
						for (int i = 0; i < 2; ++i)
						{
							newformation = new crFormationInfo;
							newformation->setAbstractID(formationinfo->getAbstractID());
							newformation->setCount(formationinfo->getCount());
							newformation->setEquips(formationinfo->getEquips());
							switch (i)
							{
							case 0:
								troop_infors->push_back(newformation);
								break;
							case 1:
								(*savedtroop_infors)[pageselect].second.push_back(newformation);
								break;
							default:
								break;
							}
						}
					}
				}
			}
		} while (0);
		player_data->excHandle(MAKEINT64(WCH_LockData,0));
		if (is_valid == -2)
		{
			stream->createBuf(70);
			stream->_writeChar(is_valid);
			///预备部队数量同步
			stream->_writeUChar(NewTroops.size());
			for (ntItr = NewTroops.begin();
				ntItr != NewTroops.end();
				++ntItr)
			{
				stream->_writeInt(ntItr->first);
				stream->_writeInt(ntItr->second);
			}
			//编队信息同步
			stream->_writeChar(pageselect);
			if (pageselect == -1)
			{
				stream->_writeUChar(troop_infors->size());
				for (FormationInfoVec::iterator itr = troop_infors->begin();
					itr != troop_infors->end();
					++itr)
				{
					formationinfo = itr->get();
					stream->_writeInt(formationinfo->getAbstractID());
					stream->_writeUShort(formationinfo->getCount());
					stream->_writeVec2i(formationinfo->getEquips());
				}
			}
			else
			{
				if (pageselect >= 0 && pageselect < savedtroop_infors->size())
				{
					stream->_writeUChar((*savedtroop_infors)[pageselect].second.size());
					for (FormationInfoVec::iterator itr = (*savedtroop_infors)[pageselect].second.begin();
						itr != (*savedtroop_infors)[pageselect].second.end();
						++itr)
					{
						formationinfo = itr->get();
						stream->_writeInt(formationinfo->getAbstractID());
						stream->_writeUShort(formationinfo->getCount());
						stream->_writeVec2i(formationinfo->getEquips());
					}
				}
			}
		}
		else if(is_valid!=0)
		{
			stream->createBuf(1);
			stream->_writeChar(is_valid);
		}
		CRNetApp::crPlayerDataEventPacket packet;
		CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet,player_id,WCH_JXJRecvFormation,stream.get());
		server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
		if(is_valid==0)
		{
			///游戏日志
			std::string logdata;
			crVector3i formationid;
			crVector3i formationcount;
			crVector3i cardidfirst;
			crVector3i cardidsecond;
			int i = 0;
			if (pageselect == -1)
			{
				for( FormationInfoVec::iterator itr = troop_infors->begin();
					itr != troop_infors->end();
					++itr,i++ )
				{
					formationinfo = itr->get();
					formationid[i] = formationinfo->getAbstractID();
					formationcount[i] = formationinfo->getCount();
					cardidfirst[i] = formationinfo->getEquips()[0];
					cardidsecond[i] = formationinfo->getEquips()[1];
				}
			}
			else
			{
				if (pageselect >= 0 && pageselect < savedtroop_infors->size())
				{
					for( FormationInfoVec::iterator itr = (*savedtroop_infors)[pageselect].second.begin();
						itr != (*savedtroop_infors)[pageselect].second.end();
						++itr,i++ )
					{
						formationinfo = itr->get();
						formationid[i] = formationinfo->getAbstractID();
						formationcount[i] = formationinfo->getCount();
						cardidfirst[i] = formationinfo->getEquips()[0];
						cardidsecond[i] = formationinfo->getEquips()[1];
					}
				}
			}
			logdata = "编队:兵种ID" + crArgumentParser::appVectoa(formationid) +",数量"+ crArgumentParser::appVectoa(formationcount) + ",天卡ID"+crArgumentParser::appVectoa(cardidfirst)+",副卡ID"+crArgumentParser::appVectoa(cardidsecond);
			GameLogData gamelog(Log_Formation,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(player_id,&gamelog));
		}
	}
	else if(m_net_type == CRNetApp::GameClient_Game)
	{
		char is_done = m_stream->_readChar();
		if(is_done < 0)
		{
			int texid = 0;
			switch (is_done)
			{
			case -1:
				texid = 1028;//兵种信息出错
				break;
			case -2:
				texid = 1029;//"兵种数量超额";
				{
					void *param;
					CRCore::crData	*player_data = m_this->getDataClass();
					player_data->excHandle(MAKEINT64(WCH_LockData, 1));
					player_data->getParam(WCHDATA_JXJFormationInfoVec, param);
					FormationInfoVec *troop_infors = (FormationInfoVec *)param;
					player_data->getParam(WCHDATA_JXJTroopsMap, param);
					TroopsMap *troops = (TroopsMap*)param;
					player_data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
					SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;
					std::map<int, int>::iterator ntItr;
					unsigned char size = m_stream->_readUChar();
					int id, count;
					for (int i = 0; i < size; i++)
					{
						id = m_stream->_readInt();
						count = m_stream->_readInt();
						(*troops)[id] = count;
					}
					char pageselect = m_stream->_readChar();
					size = m_stream->_readUChar();
					ref_ptr<crFormationInfo> formationinfo = NULL;
					ref_ptr<crFormationInfo> formationinfo2 = NULL;
					if (pageselect == -1)
					{
						troop_infors->clear();
					}
					else
					{
						troop_infors->clear();
						if (pageselect >= 0 && pageselect < savedtroop_infors->size())
						{
							(*savedtroop_infors)[pageselect].second.clear();
						}
					}
					for (int i = 0; i < size; i++)
					{
						formationinfo = new crFormationInfo;
						formationinfo->setAbstractID(m_stream->_readInt());
						formationinfo->setCount(m_stream->_readUShort());
						formationinfo->setEquips(m_stream->_readVec2i());
						if (pageselect == -1)
						{
							troop_infors->push_back(formationinfo);
						}
						else
						{
							formationinfo2 = new crFormationInfo;
							formationinfo2->setAbstractID(formationinfo->getAbstractID());
							formationinfo2->setCount(formationinfo->getCount());
							formationinfo2->setEquips(formationinfo->getEquips());

							troop_infors->push_back(formationinfo);
							if (pageselect >= 0 && pageselect < savedtroop_infors->size())
							{
								(*savedtroop_infors)[pageselect].second.push_back(formationinfo2);
							}
						}
					}
					player_data->excHandle(MAKEINT64(WCH_LockData, 0));
					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1_Formation");
					if (canvas.valid())
						canvas->doEvent(WCH_UI_UpdateData);
				}
				break;
			case -3:
				texid = 1030;//"领导力超出";
				break;
			case -4:
				texid = 1031;//"将卡出错";
				break;
			case -5:
				texid = 1032;//"将卡背包已满";
				break;
			case -6:
				texid = 2109;//"同名将卡只能上一张";
				break;
			}
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			return;
		}
		void *param;
		char pageselect = m_stream->_readChar();
		std::string pagename;
		if (pageselect >= 0)
		{
			pagename = m_stream->_readString();
		}
		CRCore::crData	*player_data		= m_this->getDataClass();
		player_data->excHandle(MAKEINT64(WCH_LockData,1));
		player_data->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *troop_infors = (FormationInfoVec *)param;
		player_data->getParam(WCHDATA_JXJTroopsMap,param);
		TroopsMap *troops = (TroopsMap*)param;
		player_data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *playerCardVec = (JXJItemBagVec *)param;
		player_data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
		SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;

		//stream->createBuf(5+NewTroops.size()*8+RemoveCardIndex.size()+InsertCardID.size()*4+troop_infors->size()*14);
		//预备部队数量变化
		std::map<int,int>::iterator ntItr;
		unsigned char size = m_stream->_readUChar();
		int id,count;
		for( int i = 0; i<size; i++ )
		{
			id = m_stream->_readInt();
			count = m_stream->_readInt();
			(*troops)[id] = count;
		}
		//将卡背包删除
		size = m_stream->_readUChar();
		for( int i = 0; i<size; i++ )
		{
			id = m_stream->_readUChar();
			(*playerCardVec)[id] = NULL;
		}
		//放入将卡背包
		size = m_stream->_readUChar();
		JXJItemBagVec::iterator jkItr;
		for( int i = 0; i<size; i++ )
		{
			id = m_stream->_readInt();
			for( jkItr = playerCardVec->begin();
				jkItr != playerCardVec->end();
				++jkItr )
			{
				if(!(jkItr->valid()))
				{
					*jkItr = new crBagItemData;
					(*jkItr)->setItemID(id);
					(*jkItr)->setItemCount(1);
					break;
				}
			}
		}
		size = m_stream->_readUChar();
		ref_ptr<crFormationInfo> formationinfo = NULL;
		ref_ptr<crFormationInfo> formationinfo2 = NULL;
		if (pageselect == -1)
		{
			troop_infors->clear();
		}
		else
		{
			troop_infors->clear();
			if (pageselect >= 0 && pageselect < savedtroop_infors->size())
			{
				(*savedtroop_infors)[pageselect].second.clear();
			}
		}
		for( int i = 0; i<size; i++ )
		{
			formationinfo = new crFormationInfo;
			formationinfo->setAbstractID(m_stream->_readInt());
			formationinfo->setCount(m_stream->_readUShort());
			formationinfo->setEquips(m_stream->_readVec2i());
			if (pageselect == -1)
			{
				troop_infors->push_back(formationinfo);
			}
			else
			{
				formationinfo2 = new crFormationInfo;
				formationinfo2->setAbstractID(formationinfo->getAbstractID());
				formationinfo2->setCount(formationinfo->getCount());
				formationinfo2->setEquips(formationinfo->getEquips());

				troop_infors->push_back(formationinfo);
				if (pageselect >= 0 && pageselect < savedtroop_infors->size())
				{
					(*savedtroop_infors)[pageselect].first = pagename;
					(*savedtroop_infors)[pageselect].second.push_back(formationinfo2);
				}
			}
		}
		unsigned char pageindex = m_stream->_readUChar();
		player_data->inputParam(WCHDATA_JXJSavedFormationIndex,&pageindex);
		player_data->excHandle(MAKEINT64(WCH_LockData,0));
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1_Formation");
		if(canvas.valid())
			canvas->doEvent(WCH_UI_UpdateData);
		//crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		//// data area
		//FormationInfoVec formationtemp;
		//unsigned char formationsize = m_stream->_readUChar();
		//for (int i = 0; i < (int)formationsize;i++)
		//{
		//	ref_ptr<crFormationInfo> formationinfo = new crFormationInfo;
		//	formationinfo->setAbstractID(m_stream->_readInt());
		//	formationinfo->setCount(m_stream->_readUShort());
		//	formationinfo->setEquips(m_stream->_readVec2i());
		//	formationtemp.push_back(formationinfo);
		//}

		/////////////////////////////////
		//// data area
		//void *param;
		//CRCore::crData	*player_data		= m_this->getDataClass();
		//player_data->excHandle(MAKEINT64(WCH_LockData,1)); // locked +++++++

		//CREncapsulation::crTableIO::StrVec	record; // record extracting

		//player_data->getParam(WCHDATA_JXJFormationInfoVec,param);
		//FormationInfoVec *troop_infors = (FormationInfoVec *)param;
		//player_data->getParam(WCHDATA_JXJTroopsMap,param);
		//TroopsMap *troops = (TroopsMap*)param;
		//CREncapsulation::ref_ptr<crTableIO>troop_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops look-up table 
		//int lead_index = troop_table->getTitleIndex("领导力");
		//player_data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		//JXJItemBagVec *player_card = (JXJItemBagVec *)param;
		//CREncapsulation::ref_ptr<crTableIO>gov_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab); // govecap look-up table


		//// check data
		//std::map<int, int> arm_check; // 兵种检测数据
		//std::map<int, int> equip_check; // 将卡检测数据
		//int equip_check_count = 0;
		//std::map<int, int> equip_need; // 将卡需求
		////std::vector<int> equip_need; // 将卡需求
		//m_this->getMainRole()->getDataClass()->getParam(WCHDATA_JXJGovPost, param);
		//gov_table->queryOneRecord(0, crArgumentParser::appItoa(*((unsigned char*)param)), record);
		//unsigned short lead_total = atoi(record[2].c_str()); // 总领导力
		//unsigned short lead_used = 0; // 计算领导力消耗

		//// 将原先编制数据放入 
		//// 兵种 和 将卡 检测数据中
		//std::map<int, int>::iterator find_itr;
		//for(FormationInfoVec::iterator itr = troop_infors->begin(); itr != troop_infors->end(); itr++)
		//{
		//	// 编队中的兵种数据
		//	find_itr = arm_check.find(itr->get()->getAbstractID());
		//	if(find_itr == arm_check.end())
		//	{
		//		arm_check.insert(std::pair<int, int>(itr->get()->getAbstractID(), itr->get()->getCount()));

		//		// 兵种数据库中的数据
		//		find_itr = troops->find(itr->get()->getAbstractID());
		//		if(find_itr != troops->end()) arm_check.find(itr->get()->getAbstractID())->second += find_itr->second;
		//	}
		//	else
		//	{
		//		find_itr->second += itr->get()->getCount();
		//	}

		//	// 将卡数据
		//	crVector2i equips = itr->get()->getEquips();
		//	for(int i = 0; i < 2; i++)
		//	{
		//		if(equips[i] < 0) continue;
		//		find_itr = equip_check.find(equips[i]);
		//		if(find_itr == equip_check.end()) equip_check.insert(std::pair<int, int>(equips[i], 1));
		//		else find_itr->second++;
		//		equip_check_count++;
		//	}
		//}

		//for(int i = 0; i < formationsize; i++)
		//{
		//	int arm_id = formationtemp[i]->getAbstractID(); // 编制兵种ID
		//	unsigned short arm_count = formationtemp[i]->getCount(); // 编制数量
		//	unsigned short arm_sum = 0; // 兵种总数
		//	unsigned short arm_plead = 0; // 编制兵种单兵消耗领导力

		//	if (arm_id > 0)
		//	{
		//		// 兵种信息查找
		//		if(!troop_table->queryOneRecord(0, crArgumentParser::appItoa(arm_id), record))
		//		{
		//			break;
		//		}
		//		arm_plead = atoi(record[lead_index].c_str());
		//		lead_used = arm_plead * arm_count;

		//		// 更新兵种检测数据
		//		find_itr = arm_check.find(arm_id);
		//		if(find_itr == arm_check.end())
		//		{
		//			find_itr = troops->find(arm_id);
		//			arm_check.insert(std::pair<int, int>(find_itr->first, find_itr->second));
		//			find_itr = arm_check.find(arm_id);
		//		}

		//		// 兵种数量不足
		//		find_itr->second -= arm_count;
		//		if(find_itr->second < 0) 
		//		{
		//			break;
		//		}

		//		for(int i_i = 0; i_i < 2; i_i++)
		//		{
		//			int eqp = formationtemp[i]->getEquips()[i_i];
		//			if(eqp <= 0) continue;
		//			find_itr = equip_check.find(eqp);
		//			if(find_itr != equip_check.end())
		//			{
		//				find_itr->second--;
		//				if(find_itr->second <= 0) equip_check.erase(find_itr);
		//				equip_check_count--;
		//			}
		//			else
		//			{
		//				find_itr = equip_need.find(eqp);
		//				if(find_itr != equip_need.end())
		//				{
		//					find_itr->second ++;
		//				}
		//				else
		//				{
		//					equip_need.insert(std::pair<int, int>(eqp, 1));
		//				}
		//			}
		//		}
		//	}
		//	else
		//	{
		//		for(int i_i = 0; i_i < 2; i_i++)
		//		{
		//			int eqp = formationtemp[i]->getEquips()[i_i];
		//			if(eqp <= 0) continue;
		//			find_itr = equip_check.find(eqp);
		//			if(find_itr != equip_check.end())
		//			{
		//				find_itr->second--;
		//				if(find_itr->second <= 0) equip_check.erase(find_itr);
		//				equip_check_count--;
		//			}
		//			else
		//			{
		//				find_itr = equip_need.find(eqp);
		//				if(find_itr != equip_need.end())
		//				{
		//					find_itr->second ++;
		//				}
		//				else
		//				{
		//					equip_need.insert(std::pair<int, int>(eqp, 1));
		//				}
		//			}
		//		}
		//	}
		//}


		//// 将卡检测
		//// 检测将卡需求 能否在背包中满足
		//std::vector<int> removes;
		//std::vector<int> plots; // 背包插入
		//for(int index = 0; index < player_card->size(); index++)
		//{
		//	if(!(*player_card)[index].get())
		//	{
		//		if(plots.size() < equip_check_count) plots.push_back(index);
		//	}
		//	else
		//	{
		//		find_itr = equip_need.find((*player_card)[index].get()->getItemID());
		//		if(find_itr != equip_need.end())
		//		{
		//			find_itr->second--;
		//			if(find_itr->second <= 0) equip_need.erase(find_itr);
		//			if(plots.size() < equip_check_count)
		//			{
		//				plots.push_back(index);
		//			}
		//			else
		//			{
		//				removes.push_back(index);
		//			}
		//		}
		//	}
		//}

		//troop_infors->clear();
		//for(int i = 0; i < formationsize; i++)
		//{
		//	CRCore::ref_ptr<JXJ::crFormationInfo> formation = new JXJ::crFormationInfo();
		//	formation->setAbstractID( formationtemp[i] -> getAbstractID());
		//	formation->setCount(formationtemp[i] ->getCount());
		//	formation->setEquips(formationtemp[i]->getEquips());
		//	troop_infors->push_back(formation);
		//}

		//// update troops
		//for(std::map<int, int>::iterator itr = arm_check.begin(); itr != arm_check.end(); itr++)
		//{
		//	TroopsMap::iterator tro_itr = troops->find(itr->first);
		//	if (tro_itr != troops->end())
		//	{
		//		tro_itr->second = itr->second;
		//	}
		//}

		//// update equips
		//for(int index = 0; index < removes.size(); index++)
		//{
		//	(*player_card)[removes[index]] = NULL;
		//}

		////int index = 0;
		////for(std::map<int, int>::iterator itr = equip_check.begin(); itr != equip_check.end(); itr++)
		////{
		////	for(int index_i = 0; index_i < itr->second; index_i++)
		////	{
		////		if(!(*player_card)[plots[index]].get())
		////		{
		////			(*player_card)[plots[index]] = new crBagItemData;
		////		}

		////		(*player_card)[plots[index]]->setItemID(itr->first);
		////		(*player_card)[plots[index]]->setItemCount(1);
		////		index++;
		////	}
		////}

		// 关闭窗口
		if (pageselect == -1)
		{
			crCanvasNode *formation_canvas = crFilterRenderManager::getInstance()->findCanvas(m_formation_canvas);
			crData *formation_canvas_data = formation_canvas->getDataClass();
			formation_canvas_data->excHandle(MAKEINT64(WCH_LockData,1));
			formation_canvas_data->addParam(WCHDATA_JXJFormationMove, "0");
			formation_canvas_data->excHandle(MAKEINT64(WCH_LockData,0));
			//crFilterRenderManager::getInstance()->closeCanvas(formation_canvas);
			//crFilterRenderManager::getInstance()->closeCanvas(m_jkcnavase);
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailPagingMethod
// On  : push
// Desp: 邮件翻页
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailPagingMethod::crJXJUIOnMailPagingMethod()
{}
JXJ::crJXJUIOnMailPagingMethod::crJXJUIOnMailPagingMethod(const crJXJUIOnMailPagingMethod& handle)
{}
void JXJ::crJXJUIOnMailPagingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailPagingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_dir = str;
		break;
	case 1:
		m_input_name = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailPagingMethod::operator()(crHandle &handle)
{
	void *param; // parameter extracting
//	char temp[20]; // int to string
	CRCore::crCanvasNode *mail_canvas = m_this->getParentCanvas();
	CRCore::crData *mail_canvas_data = mail_canvas->getDataClass();	
	mail_canvas_data->getParam(WCHDATA_JXJMailTotalPage, param);
	int total_page = *((int *)param);
	mail_canvas_data->getParam(WCHDATA_JXJMailCurPage, param);
	int cur_page = *((int *)param);


	if(m_dir == std::string("left"))
	{
		if (cur_page > 1)
		{
			cur_page -= 1;
		}
	}
	else
	{
		if(cur_page < total_page)
		{
			cur_page += 1;
		}
	}


	//_itoa(cur_page, temp, 10);
	mail_canvas_data->addParam(WCHDATA_JXJMailCurPage, crArgumentParser::appItoa(cur_page));

	// show
	crStaticTextWidgetNode *page_show = 
		dynamic_cast<crStaticTextWidgetNode *>(mail_canvas->getWidget(m_input_name));
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailPagingMethod
// On  : update data
// Desp: 更新数据
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailUpdateDataMethod::crJXJUIOnMailUpdateDataMethod()
{}
JXJ::crJXJUIOnMailUpdateDataMethod::crJXJUIOnMailUpdateDataMethod(const crJXJUIOnMailUpdateDataMethod& handle):
	m_this(handle.m_this),
	m_page_show(handle.m_page_show),
	m_statesw(handle.m_statesw),
	m_allselectbtn(handle.m_allselectbtn),
	m_deletebtn(handle.m_deletebtn),
	m_getbtn(handle.m_getbtn),
	m_scrollbar(handle.m_scrollbar)
{
	for (int i = 0; i < 15;i++)
	{
		if (i < 5)
		{
			m_checkbox[i] = handle.m_checkbox[i];
			m_rightinput[i] = handle.m_rightinput[i];
			m_righticon[i] = handle.m_righticon[i];
			m_righticonsw[i] = handle.m_righticonsw[i];
		}
		m_leftinput[i] = handle.m_leftinput[i];
	}
}
void JXJ::crJXJUIOnMailUpdateDataMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailUpdateDataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_page_show = str;
		break;
	case 1:
		m_statesw = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		m_leftinput[i - 2] = str;
		break;
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_checkbox[i - 17] = str;
		break;
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
		m_rightinput[i - 22] = str;
		break;
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		m_righticon[i - 27] = str;
		break;
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
		m_righticonsw[i - 32] = str;
		break;
	case 37:
		m_allselectbtn = str;
		break;
	case 38:
		m_deletebtn = str;
		break;
	case 39:
		m_getbtn = str;
		break;
	case 40:
		m_scrollbar = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailUpdateDataMethod::operator()(crHandle &handle)
{
	crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_pagesw));
	crStaticTextWidgetNode *leftinput[15] = {NULL};
	crCheckBoxWidgetNode *checkbox[5] = {NULL};
	crStaticTextWidgetNode *rightinput[4] = {NULL};
	crImageBoxWidgetNode *righticon[5] = {NULL};
	crMultiSwitch *righticonsw[5] = {NULL};
	for (int i = 0; i < 15; i++)
	{
		leftinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_leftinput[i]));
		if (leftinput[i])
		{
			leftinput[i]->clearString();
		}
		if (i < 5)
		{
			checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_checkbox[i]));
			righticon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_righticon[i]));
			righticonsw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_righticonsw[i]));
			if (checkbox[i])
			{
				checkbox[i]->select(false);
			}
			if (righticonsw[i])
			{
				righticonsw[i]->setActiveSwitchSet(2);
			}
			if (i < 4)
			{
				rightinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_rightinput[i]));
				if (rightinput[i])
				{
					rightinput[i]->clearString();
				}
			}
		}
	}
	crHypertextWidgetNode *rightinput5 = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_rightinput[4]));
	crButtonWidgetNode *allselectbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_allselectbtn));
	crButtonWidgetNode *deletebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_deletebtn));
	crButtonWidgetNode *getbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_getbtn));
	crScrollBarWidgetNode *scrollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scrollbar));
	if (rightinput5)
	{
		rightinput5->clearText();
	}
	if (allselectbtn)
	{
		allselectbtn->setEnable(false);
	}
	if (deletebtn)
	{
		deletebtn->setEnable(false);
	}
	if (getbtn)
	{
		getbtn->setEnable(false);
	}
	if (scrollbar)
	{
		scrollbar->setValue(0);
	}
	void *param;

	crData *player_data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	CRCore::crData *mail_canvas_data = m_this->getDataClass();
	mail_canvas_data->getParam(WCHDATA_JXJMailTotalPage, param);
	int total_page = *((int *)param);
	mail_canvas_data->getParam(WCHDATA_JXJMailCurPage, param);
	int cur_page = *((int *)param);

	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	int perpage = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMailPerPage,viplv).c_str()));

	crRadioGroupWidgetNode *statesw = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_statesw));
	unsigned char selecting = 0;
	if(statesw)
	{
		selecting = statesw->getSelect();
	}

	crStaticTextWidgetNode *page_show = 
		dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_page_show));
	std::string page_info = std::string();
	page_info.append(crArgumentParser::appItoa(cur_page));
	page_info.append("/");
	page_info.append(crArgumentParser::appItoa(total_page));
	if(page_show)page_show->setString(page_info);

	// parse packet
	
	MailStats state = Mail_All;
	switch (selecting)
	{
	case 1:
		state = Mail_Recvd;
		break;
	case 2:
		state = Mail_Readed;
		break;
	}
	//检查本地
	bool bNeedQuery = false;
	crData *brainData = crBrain::getInstance()->getDataClass();
	brainData->excHandle(MAKEINT64(WCH_LockData,1));
// 
// 	brainData->getParam(WCHDATA_JXJMailMap,param);
// 	MailMap *mailMap = (MailMap *)param;
// 	
// 	MailMap::iterator itr_map = mailMap->find(state);
// 	if(itr_map!=mailMap->end())
// 	{
// 		if(itr_map->second.size() < cur_page*perpage)
// 			bNeedQuery = true;
// 	}
// 	else
// 	{
// 		bNeedQuery = true;
// 	}
	
	//发送查询包
	//if(bNeedQuery)
	{
		CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
		stream->createBuf(2);
		stream->_writeUChar(state);
		stream->_writeUChar(cur_page);
		CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
		CRNetApp::crPlayerDataEventPacket packet;
		CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJMailRequest, stream.get());
		if(netConductor)
			netConductor->getNetManager()->sendPacket("all",packet);
	}
// 	else
// 	{
// 		brainData->getParam(WCHDATA_JXJRecvMailVec,param);
// 		JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
// 		if(mailvec)
// 			mailvec->clear();
// 
// 		int index = 0;
// 		int startPos = (cur_page-1)*perpage;
// 		for (;index<itr_map->second.size()&&index<perpage;index++)
// 		{
// 			if(index<startPos) continue;
// 			mailvec->push_back(itr_map->second[index]);
// 		}
// 
// 		m_this->doEvent(WCH_JXJQueryMailShow);
// 	}
	brainData->excHandle(MAKEINT64(WCH_LockData,0));
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIOnMailTabSelectingMethod
// On  : tab select
// Desp: 数据更新
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIOnMailTabSelectingMethod::crJXJUIOnMailTabSelectingMethod()
{}
JXJ::crJXJUIOnMailTabSelectingMethod::crJXJUIOnMailTabSelectingMethod(const crJXJUIOnMailTabSelectingMethod& handle)
{}
void JXJ::crJXJUIOnMailTabSelectingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailTabSelectingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_selecting = str;
	default:
		break;
	}
}
void JXJ::crJXJUIOnMailTabSelectingMethod::operator()(crHandle &handle)
{
	CRCore::crCanvasNode *mail_canvas = dynamic_cast<CRCore::crCanvasNode *>(m_this->getParentCanvas());
	CRCore::crData *mail_canvas_data = mail_canvas->getDataClass();
	mail_canvas_data->addParam(WCHDATA_JXJMailTabSel, m_selecting);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJOnMailRequestMethod
// On  : 邮件更新
// Desp: 数据数据和请求封装、解析
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJOnMailRequestMethod::crJXJOnMailRequestMethod()
{}
JXJ::crJXJOnMailRequestMethod::crJXJOnMailRequestMethod(const crJXJOnMailRequestMethod& handle):
	m_mail_canvas(handle.m_mail_canvas)
{
}
void JXJ::crJXJOnMailRequestMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (CRNetApp::crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (CRCore::crStreamBuf *)(LOINT64(param64));
			m_net_type = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void JXJ::crJXJOnMailRequestMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mail_canvas = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJOnMailRequestMethod::operator()(crHandle &handle)
{
	if(!m_this)
	{
		return;
	}

	if(m_net_type == CRNetApp::GameServer)
	{
		unsigned char state = m_stream->_readUChar();

		int cur_page = m_stream->_readUChar();
		int count = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMailPerPage,0).c_str()));


		// make query 
		void *param;
		crData *server_data = crServerBrainHandle::getInstance()->getDataClass();
		server_data->excHandle(MAKEINT64(WCH_LockData,1));
		server_data->getParam(WCHDATA_JXJMailQueryList, param);
		MailQueryDeque *query_deque = (MailQueryDeque *)param;

		// check de remove new query to end
		bool found = false;
		for(MailQueryDeque::iterator itr = query_deque->begin(); itr != query_deque->end(); ++itr)
		{
			if((*itr)->getQueryID() == m_this->getPlayerID())
			{
				(*itr)->setState(state);
				(*itr)->setQuerycount(count);
				(*itr)->setStartPos((cur_page - 1) * (*itr)->getQuerycount());
				found = true;
				break;
			}
		}
		if(!found)
		{
			CRCore::ref_ptr<crMailQuery> query = new crMailQuery;
			query->setQueryID(m_this->getPlayerID());
			query->setState(state);
			query->setQuerycount(count);
			query->setStartPos((cur_page - 1) * query->getQuerycount());
			query_deque->push_back(query);
		}
		server_data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	else if(m_net_type == CRNetApp::GameClient_Game)
	{
		void *param;
		int mailcount = m_stream->_readInt();
		unsigned char size = m_stream->_readUChar();
		int index = 0;
		crData *data = m_this->getDataClass();

		crCanvasNode *mail_canvas = crFilterRenderManager::getInstance()->findCanvas(m_mail_canvas);
		if (mail_canvas)
		{
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			//crStaticTextWidgetNode *inputpage = dynamic_cast<crStaticTextWidgetNode *>(mail_canvas->getWidget(m_input_page));
			int perpage = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMailPerPage,viplv).c_str()));
			//
			crData *canvasdata = mail_canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJMailTotalPage,param);
			int *totalPage = (int *)param;
			if (mailcount == 0)
			{
				*totalPage = 1;
			}
			else
			{
				*totalPage = (ceil)((float)mailcount/(float)perpage);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			
			crData *brainData = crBrain::getInstance()->getDataClass();
			brainData->excHandle(MAKEINT64(WCH_LockData,1));
			brainData->getParam(WCHDATA_JXJRecvMailVec,param);
			JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
			if(mailvec)
				mailvec->clear();
  			brainData->getParam(WCHDATA_JXJRecvMailCacheMap,param);
  			MailCacheMap *mailcacheMap = (MailCacheMap *)param;
			for(; index < size; index ++)
			{
				int dbid = m_stream->_readInt();
				unsigned char state = m_stream->_readUChar();
				std::string title = m_stream->_readString();
				std::string sender = m_stream->_readString();
				std::string createtime = m_stream->_readString();
				//
				ref_ptr<crMailInfo> recvmaildata = new crMailInfo;
				recvmaildata->setID(dbid);
				recvmaildata->setState(state);
				recvmaildata->setTitle(title);
				recvmaildata->setSenderName(sender);
				recvmaildata->setCreateData(createtime);

				if(mailvec)
					mailvec->push_back(recvmaildata);

				if(mailcacheMap)
				{
					MailCacheMap::iterator itr = mailcacheMap->find(dbid);
					if(itr!=mailcacheMap->end())
					{
						itr->second->setState(state);
						itr->second->setTitle(title);
						itr->second->setSenderName(sender);
						itr->second->setCreateData(createtime);
					}
					else
					{
						mailcacheMap->insert(std::make_pair(dbid,recvmaildata));
					}
				}
			}
			brainData->excHandle(MAKEINT64(WCH_LockData,0));

			mail_canvas->doEvent(WCH_JXJQueryMailShow);
		}
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJOnMailRecvMethod
// On  : mail send 
// Desp: 数据数据和请求封装、解析
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJOnMailRecvMethod::crJXJOnMailRecvMethod()
{}
JXJ::crJXJOnMailRecvMethod::crJXJOnMailRecvMethod(const crJXJOnMailRecvMethod& handle):
	m_mailcanvas(handle.m_mailcanvas),
	m_receiver(handle.m_receiver),
	m_title(handle.m_title),
	m_content(handle.m_content),
	m_tipcanvas(handle.m_tipcanvas),
	m_tiptitle(handle.m_tiptitle),
	m_tipcontent(handle.m_tipcontent),
	m_contentsize(handle.m_contentsize)
{}
void JXJ::crJXJOnMailRecvMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (CRNetApp::crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (CRCore::crStreamBuf *)(LOINT64(param64));
			m_net_type = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void JXJ::crJXJOnMailRecvMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailcanvas = str;
		break;
	case 1:
		m_receiver = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_content = str;
		break;
	case 4:
		m_contentsize = str;
		break;
	case 5:
		m_tipcanvas = str;
		break;
	case 6:
		m_tiptitle = str;
		break;
	case 7:
		m_tipcontent = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJOnMailRecvMethod::operator()(crHandle &handle)
{
	if(!m_this)
	{
		return;
	}

	if(m_net_type == CRNetApp::GameServer)
	{
		// parse mail data
		ref_ptr<crMailInfo> mail = new crMailInfo; 
		mail->setRecvName(m_stream->_readString());
		mail->setTitle(m_stream->_readString());
		mail->setBody(m_stream.get());
		//MailInfo mail;
		//mail.receiver_name = m_stream->_readString();
		//mail.title = m_stream->_readString();
		//mail.content = m_stream->_readString();
		bool is_sended = false;
		int receiver_id = 0;

		// get receiver id
		CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer); // packet replay and tip 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(server_net->getNetDataManager()->getNetCallback());
		receiver_id = CRNetApp::crGlobalHandle::getInstance()->gameServerQueryPlayerID(mail->getRecvName(),callback->getServerID());

		crNetDataManager *netDataManager = server_net->getNetDataManager();
		//netDataManager->lockPlayerDataMap();
		//crNetDataManager::PlayerDataMap &player_map = netDataManager->getPlayerDataMap();
		//for(crNetDataManager::PlayerDataMap::iterator itr = player_map.begin(); itr != player_map.end(); itr++)
		//{
		//	ref_ptr<crGameServerPlayerData> send_to = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
		//	if (send_to.valid())
		//	{
		//		if (send_to->getCharacterName() == mail->getRecvName())
		//		{
		//			receiver_id = send_to->getPlayerID();
		//		}
		//	}
		//}
		//netDataManager->unLockPlayerDataMap();
		// check and parse result

		int player_id = m_this->getPlayerID();
		if(receiver_id > 0)
		{
			// parse data
			mail->setSenderID(player_id);
			mail->setRecvID(receiver_id);
			mail->setSenderName(m_this->getCharacterName());

			// input into sending queue
			crData *server_data = crServerBrainHandle::getInstance()->getDataClass();
			server_data->excHandle(MAKEINT64(WCH_LockData,1));
			void *param;
			server_data->getParam(WCHDATA_JXJMailTaskList, param);
			MailTaskDeque *mail_deque = (MailTaskDeque *)param;
			mail_deque->push_back(mail);
			server_data->excHandle(MAKEINT64(WCH_LockData,0));
			is_sended = true;

			//发送到接收玩家
			//ref_ptr<crStreamBuf> query_result = new crStreamBuf;
			//query_result->createBuf(1);
			//query_result->_writeBool(is_sended);
			crPlayerDataEventPacket packet;
			packet.buildReplyPacket(packet,receiver_id,WCH_JXJNewMailRequest,NULL);
			CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(receiver_id));
			if(player_net_data.valid())
			{
				server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
			}
		}

		// send back
		CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
		stream->createBuf(1);
		stream->_writeBool(is_sended);
		crNetConductor *db_net = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB); // database operations
		ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(player_id));
		if (player_net_data.valid())
		{
			CRNetApp::crPlayerDataEventPacket packet;
			CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet,player_id,WCH_JXJMailSend,stream.get());
			server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
		}
		///游戏日志
		std::string logdata = crArgumentParser::appItoa(is_sended);
		GameLogData gamelog(Log_MailSend,logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(player_id,&gamelog));
	}
	else if(m_net_type == CRNetApp::GameClient_Game)
	{
		bool is_sended = m_stream->_readBool();
		int i = 0;
		std::string tipstr;
		// TODO: tips
		if (is_sended)
		{
			tipstr = "邮件发送成功！";
			crCanvasNode *mail_canvas = crFilterRenderManager::getInstance()->findCanvas(m_mailcanvas);
			crEditWidgetNode *receiver_etxt = 
				dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_receiver));
			crEditWidgetNode *title_etxt = 
				dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_title));
			crEditWidgetNode *content_etxt = 
				dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_content));
			crStaticTextWidgetNode *content_size = 
				dynamic_cast<crStaticTextWidgetNode *>(mail_canvas->getWidget(m_contentsize));

			if (receiver_etxt)
			{
				receiver_etxt->clearString();
			}
			if (title_etxt)
			{
				title_etxt->clearString();
			}
			if (content_etxt)
			{
				content_etxt->clearString();
			}
			if (content_size)
			{
				content_size->setString(std::string("0"));
			}
		}
		else
		{
			tipstr = "邮件发送失败！";
		}
		crCanvasNode *tipcanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipcanvas);
		if (tipcanvas)
		{
			//crStaticTextWidgetNode *tiptitle = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tiptitle));
			//if (tiptitle)
			//{
				//tiptitle->setString(std::string("发送邮件"));
			//}
			crStaticTextWidgetNode *tipcontent = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tipcontent));
			if (tipcontent)
			{
				tipcontent->setString(tipstr);
			}
			crFilterRenderManager::getInstance()->showCanvas(tipcanvas,true);
		}
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJOnMailSendingMethod
// On  : mail send 
// Desp: 数据数据和请求封装、解析
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJOnMailSendingMethod::crJXJOnMailSendingMethod()
{}
JXJ::crJXJOnMailSendingMethod::crJXJOnMailSendingMethod(const crJXJOnMailSendingMethod& handle):
	m_this(NULL),
	m_receiver(handle.m_receiver),
	m_title(handle.m_title),
	m_content(handle.m_content),
	m_tipcanvas(handle.m_tipcanvas),
	m_tiptitle(handle.m_tiptitle),
	m_tipcontent(handle.m_tipcontent)
{}
void JXJ::crJXJOnMailSendingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJOnMailSendingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_receiver = str;
		break;
	case 1:
		m_title = str;
		break;
	case 2:
		m_content = str;
		break;
	case 3:
		m_tipcanvas = str;
		break;
	case 4:
		m_tiptitle = str;
		break;
	case 5:
		m_tipcontent = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJOnMailSendingMethod::operator()(crHandle &handle)
{
	void *param;
	bool result = true;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerGameData->getDataClass();
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	int maxsize = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPlayerNameSizeMax,viplv).c_str()));
	int titlemaxsize = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMailTitleMaxSize,viplv).c_str()));
	// - - - - - - - - - - - - - - - - - - global definitions - - - - - - - - - - - - - - - - - - 
	// entities
	CRCore::crCanvasNode *mail_canvas = m_this->getParentCanvas();
	crEditWidgetNode *receiver_etxt = 
		dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_receiver));
	crEditWidgetNode *title_etxt = 
		dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_title));
	crEditWidgetNode *content_etxt = 
		dynamic_cast<crEditWidgetNode *>(mail_canvas->getWidget(m_content));
	
	// parse format 
	// receiver
	// title
	// content
	std::string receivier;
	std::string title;
	std::string content;
	if (receiver_etxt)
	{
		receivier = receiver_etxt->getUTF8String();
	}
	if (title_etxt)
	{
		title = title_etxt->getUTF8String();
	}
	if (content_etxt)
	{
		content = content_etxt->getUTF8String();
	}

	std::string tipstr;
	ref_ptr<crTableIO>InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidWordTab);
	if (InvalidNameTab.valid())
	{
		std::wstring wtext[2]; 
		wtext[0] = CRIOManager::convertUTF8toUTF16(content.c_str());
		wtext[1] = CRIOManager::convertUTF8toUTF16(title.c_str());
		std::wstring invalidstring;
		int ipos = 0;
		int invalidwordsize;
		std::string replacestr = "****";
		for (int i = 0; i < InvalidNameTab->getRowCount();i++)
		{
			//invalidstring = InvalidNameTab->getData(i,0);
			invalidstring = CRIOManager::convertUTF8toUTF16(InvalidNameTab->getData(i,0).c_str());
			invalidwordsize = invalidstring.length();
			for (int i = 0;i < 2; i++)
			{
				if (invalidwordsize > 0 && invalidwordsize <=  wtext[i].length())
				{
					ipos = wtext[i].find(invalidstring);
					if(ipos != std::string::npos)
					{
						wtext[i].replace(ipos,invalidwordsize,std::wstring(CRIOManager::convertUTF8toUTF16(replacestr.c_str())));
						result = false;
						tipstr = "不能含有敏感词汇！";
						//for(j = 0; j < invalidwordsize;++j)
						//	chat_text[ipos+j]='*';
						//this->display_message(std::string("不能含有敏感词汇！"));
						//return;
					}
				}
			}
		}
		content =  CRIOManager::convertUTF16toUTF8(wtext[0].c_str());
		title = CRIOManager::convertUTF16toUTF8(wtext[1].c_str());
		if (content_etxt)
		{
			content_etxt->setString(content);
		}
		if (title_etxt)
		{
			title_etxt->setString(title);
		}
	}
	// TODO - input check
	if (content.size() == 0)
	{
		result = false;
		tipstr = "【内容】不能为空！";
	}
	////
	if (title.size() == 0)
	{
		result = false;
		tipstr = "【主题】不能为空！";
	}
	else if (title.size() > titlemaxsize)
	{
		result = false;
		tipstr = "【主题】不能超过" + crArgumentParser::appItoa(titlemaxsize) + "个字符！";
	}
	////
	if (receivier.empty())
	{
		result = false;
		tipstr = "【收件人】不能为空！";
	}
	else if(receivier.size()>maxsize)
	{
		result = false;
		tipstr = "【收件人】不超过6个字！";
	}
	////send
	if (result)
	{
		CRUI::crTextAttrWidgetNode::transformText(content);
		tipstr = "正在发送邮件……";
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");
		//cfg_script.Add("Font",m_font);
		//cfg_script.Add("CharacterSize",14);
		v_i.clear();
		v_i.push_back(255.0);
		v_i.push_back(255.0);
		v_i.push_back(255.0);
		v_i.push_back(255.0);
		cfg_script.Add("Color",v_i);

		cfg_script.Add("Content");
		cfg_script.Push("Content",1);
		cfg_script.Add("Text",content);
		cfg_script.Pop();

		cfg_script.Pop();
		CRCore::ref_ptr<CRCore::crStreamBuf> bodystream = new CRCore::crStreamBuf;
		crGlobalHandle::getInstance()->getScriptStream(cfg_script,bodystream);
		// packet
		CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
		stream->createBuf(12 + receivier.length() + title.length() + bodystream->getRemainSize());
		stream->_writeString(receivier);
		stream->_writeString(title);
		stream->write(bodystream->getPtr(),bodystream->getRemainSize());
		CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
		CRNetApp::crPlayerDataEventPacket packet;
		CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJMailSend,stream.get());
		net_conductor->getNetManager()->sendPacket("all",packet);
	}
	////
	crCanvasNode *tipcanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipcanvas);
	if (tipcanvas)
	{
		//crStaticTextWidgetNode *tiptitle = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tiptitle));
		//if (tiptitle)
		//{
			//tiptitle->setString(std::string("发送邮件"));
		//}
		crStaticTextWidgetNode *tipcontent = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tipcontent));
		if (tipcontent)
		{
			tipcontent->setString(tipstr);
		}
		crFilterRenderManager::getInstance()->showCanvas(tipcanvas,true);
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJMailInsertingMethod
// On  : mail inserting into database
// Desp: 邮件数据库操作
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJMailInsertingMethod::crJXJMailInsertingMethod():
	m_taskcount(10)
{}
JXJ::crJXJMailInsertingMethod::crJXJMailInsertingMethod(const crJXJMailInsertingMethod& handle):
	m_taskcount(handle.m_taskcount)
{}
void JXJ::crJXJMailInsertingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJMailInsertingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}

}
void JXJ::crJXJMailInsertingMethod::operator()(crHandle &handle)
{
	m_taskVec.resize(0);
	m_taskVec.reserve(m_taskcount);

	crData *server_data = m_this->getDataClass();
	void *param;
	CRCore::ref_ptr<crMailInfo> query;
	server_data->excHandle(MAKEINT64(WCH_LockData,1));
	server_data->getParam(WCHDATA_JXJMailTaskList, param);
	MailTaskDeque *query_deque = (MailTaskDeque *)param;
	int i = 0;
	while(!query_deque->empty() && i<m_taskcount)
	{
		query = query_deque->front();
		query_deque->pop_front();
		m_taskVec.push_back(query);
		i++;
	}
	server_data->excHandle(MAKEINT64(WCH_LockData,0));

	//crData *server_data = m_this->getDataClass();
	//void *param;
	//server_data->excHandle(MAKEINT64(WCH_LockData,1));
	//server_data->getParam(WCHDATA_JXJMailTaskList, param);
	//MailTaskDeque *mail_deque = (MailTaskDeque *)param;

	//if(mail_deque->empty())
	//{
	//	server_data->excHandle(MAKEINT64(WCH_LockData,0));
	//	return;
	//}


	//// timer
	//const Timer& timer = *Timer::instance();
	//Timer_t start_tick = timer.tick();
	//float elapsed_time = 0.0f;
	if(!m_taskVec.empty())
	{
		// database
		crNetConductor *db_net = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		ref_ptr<crInsertMail> insert_query = new crInsertMail;
		//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		//ref_ptr<crDataBase> db_session = global_db->beginSession();
		crDBSavePacket packet;
		//ref_ptr<crMailInfo> mail;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		for( std::vector<CRCore::ref_ptr<crMailInfo> >::iterator titr = m_taskVec.begin();
			titr != m_taskVec.end();
			++titr )
		{
			query = *titr;
			//mail = mail_deque->front();
			//mail_deque->pop_front();
			stream->createBuf(query->getBody()->getRemainSize());
			stream->write(query->getBody()->getPtr(),query->getBody()->getRemainSize());
			insert_query->buildUpdateSql( query->getRecvID(), query->getSenderID(), \
				query->getSenderName(), query->getTitle(), stream.get(),query->getState());
			//if(db_session->executeUpdate(insert_query.get()))
			//{
			//	db_session->commit();
			//}
			crDBSavePacket::buildRequestPacket(packet,GameGlobalDB,insert_query.get());
			db_net->getNetManager()->sendPacket("all",packet);
			//packet.buildRequestPacket(packet,GameGlobalDB,insert_query.get());
			//db_net->getNetManager()->sendPacket("all", packet);
			//elapsed_time += timer.delta_s(start_tick,timer.tick());
		}
		//global_db->endSession(db_session.get());
		//server_data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJMailInsertingMethod
// On  : mail inserting into database
// Desp: 邮件数据库操作
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJMailQueryingMethod::crJXJMailQueryingMethod():
	m_taskcount(10)
{}
JXJ::crJXJMailQueryingMethod::crJXJMailQueryingMethod(const crJXJMailQueryingMethod& handle):
	m_taskcount(handle.m_taskcount)
{}
void JXJ::crJXJMailQueryingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle *)param;
		break;
	}
}
void JXJ::crJXJMailQueryingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}
}
void JXJ::crJXJMailQueryingMethod::operator()(crHandle &handle)
{
	m_taskVec.resize(0);
	m_taskVec.reserve(m_taskcount);
	// get waiting deque
	crData *server_data = m_this->getDataClass();
	void *param;
	CRCore::ref_ptr<crMailQuery> query;
	server_data->excHandle(MAKEINT64(WCH_LockData,1));
	server_data->getParam(WCHDATA_JXJMailQueryList, param);
	MailQueryDeque *query_deque = (MailQueryDeque *)param;
	int i = 0;
	while(!query_deque->empty() && i<m_taskcount)
	{
		query = query_deque->front();
		query_deque->pop_front();
		m_taskVec.push_back(query);
		i++;
	}
	server_data->excHandle(MAKEINT64(WCH_LockData,0));

	// timer
	//const Timer& timer = *Timer::instance();
	//Timer_t start_tick = timer.tick();
	//float elapsed_time = 0.0f;
	// database
	if(!m_taskVec.empty())
	{
		int mailcount = 0;
		crNetConductor *db_net = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crCountMailQueryData> count_query = new crCountMailQueryData;
		ref_ptr<crCountMailByStatsQueryData> count_query2 = new crCountMailByStatsQueryData;
		ref_ptr<crMailByStatsQueryData> search_query = new crMailByStatsQueryData;
		ref_ptr<crMailQueryData> search_query2 = new crMailQueryData;
		CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crDataBase> db_session = global_db->beginSession();
		for( std::vector<CRCore::ref_ptr<crMailQuery> >::iterator titr = m_taskVec.begin();
			titr != m_taskVec.end();
			++titr )
		{
			query = *titr;
			mailcount = 0;
			//ref_ptr<crMailBodyQueryData> body_query = new crMailBodyQueryData;
			//CRCore::ref_ptr<crMailQuery> query = query_deque->front();
			//query_deque->pop_front();
			if (query->getState() == Mail_All)
			{
				count_query->buildQuerySql(query->getQueryID());
				db_session->executeQuery(count_query.get());
				crDataBase::QueryResultVec &count_reuslt_vec = db_session->getQueryResult();
				if(!count_reuslt_vec.empty())
				{
					crCountMailQueryData *count_record = dynamic_cast<crCountMailQueryData *>(count_reuslt_vec[0].get());
					if (count_record)
					{
						mailcount = count_record->getCount();
					}
				}
				db_session->releaseQuery();
				//if(mailcount == 0)
				//	continue;
				search_query2->buildQuerySql(query->getQueryID(), query->getStartPos(), query->getQuerycount());
				db_session->executeQuery(search_query2.get());
			}
			else
			{
				count_query2->buildQuerySql(query->getQueryID(),query->getState());
				db_session->executeQuery(count_query2.get());
				crDataBase::QueryResultVec &count_reuslt_vec2 = db_session->getQueryResult();
				if(!count_reuslt_vec2.empty())
				{
					crCountMailByStatsQueryData *count_record2 = dynamic_cast<crCountMailByStatsQueryData *>(count_reuslt_vec2[0].get());
					if (count_record2)
					{
						mailcount = count_record2->getCount();
					}
				}
				db_session->releaseQuery();
				//if(mailcount == 0)
				//	continue;
				search_query->buildQuerySql(query->getQueryID(), query->getState(), query->getStartPos(), query->getQuerycount());
				db_session->executeQuery(search_query.get());
			}

			crDataBase::QueryResultVec &query_reuslt_vec = db_session->getQueryResult();
			// parse and return
			ref_ptr<crStreamBuf> query_result = new crStreamBuf;
			//ref_ptr<crStreamBuf> query_body = new crStreamBuf;
			//m_mailbodyvec.
			int size = 1;
			// calculate return size
			crMailQueryData *query_record;
			crMailByStatsQueryData *query_record2;
			for( crDataBase::QueryResultVec::iterator itr = query_reuslt_vec.begin();
				itr != query_reuslt_vec.end();
				++itr )
			{
				//
				if (query->getState() == Mail_All)
				{
					query_record = dynamic_cast<crMailQueryData *>(itr->get());
					if (query_record)
					{
						//body_query->buildQuerySql(query_record->getID());
						size += (17 + query_record->getSenderName().length() + query_record->getTitle().length() + query_record->getCreateDate().length());
					}
				}
				else
				{
					query_record2 = dynamic_cast<crMailByStatsQueryData *>(itr->get());
					if (query_record2)
					{
						//body_query->buildQuerySql(query_record->getID());
						size += (17 + query_record2->getSenderName().length() + query_record2->getTitle().length() + query_record2->getCreateDate().length());
					}
				}
			}
			//	db_session->executeQuery(body_query.get());
			//	crDataBase::QueryResultVec &body_reuslt_vec = db_session->getQueryResult();
			//	crMailBodyQueryData *body_record = dynamic_cast<crMailBodyQueryData *>(body_reuslt_vec[0].get());
			//	if (body_record)
			//	{
			//		query_body = body_record->getBody();
			//		CRCore::ref_ptr<CRCore::crStreamBuf> bodytext = query_body;
			//		m_mailbodyvec.push_back(bodytext);
			//		size += (4 + bodytext->getBufSize());
			//	}

			//	if (query->getState() == Mail_All)
			//	{
			//		db_session->executeQuery(search_query2.get());
			//	}
			//	else
			//	{
			//		db_session->executeQuery(search_query.get());
			//	}
			//}
			// parse data
			query_result->createBuf(size+5);
			query_result->_writeInt(mailcount);
			query_result->_writeUChar(query_reuslt_vec.size());
			for( crDataBase::QueryResultVec::iterator itr = query_reuslt_vec.begin();
				itr != query_reuslt_vec.end();
				++itr )
			{
				if (query->getState() == Mail_All)
				{
					query_record = dynamic_cast<crMailQueryData *>(itr->get());
					if (query_record)
					{
						query_result->_writeInt(query_record->getID());
						query_result->_writeUChar(query_record->getStats());
						query_result->_writeString(query_record->getTitle());
						query_result->_writeString(query_record->getSenderName());
						query_result->_writeString(query_record->getCreateDate());
					}
				}
				else
				{
					query_record2 = dynamic_cast<crMailByStatsQueryData *>(itr->get());
					if (query_record2)
					{
						query_result->_writeInt(query_record2->getID());
						query_result->_writeUChar(query->getState());
						query_result->_writeString(query_record2->getTitle());
						query_result->_writeString(query_record2->getSenderName());
						query_result->_writeString(query_record2->getCreateDate());
					}
				}
				//query_result->_writeString((*m_mailbodyvec)[i]);
				//query_result->write(m_mailbodyvec[i]->getPtr(),m_mailbodyvec[i]->getRemainSize());
			}
			db_session->releaseQuery();
			// send back
			crPlayerDataEventPacket packet;
			packet.buildReplyPacket(packet,query->getQueryID(),WCH_JXJMailRequest,query_result.get());
			ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(query->getQueryID()));
			if(player_net_data.valid())
			{
				server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
			}
			//elapsed_time += timer.delta_s(start_tick,timer.tick());
		}
		global_db->endSession(db_session.get());
	}
	//server_data->excHandle(MAKEINT64(WCH_LockData,0));
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJFormationSlidingMethod
// On  : sliding
// Desp: 更新数据
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJFormationSlidingMethod::crJXJFormationSlidingMethod()
{}
JXJ::crJXJFormationSlidingMethod::crJXJFormationSlidingMethod(const crJXJFormationSlidingMethod& handle):
	m_title_lead(handle.m_title_lead),
	m_title_used(handle.m_title_used),
	m_title_remain(handle.m_title_remain),
	m_editbox(handle.m_editbox)
{
	for (int i = 0; i < 3;i++)
	{
		m_right_trp_total[i] = handle.m_right_trp_total[i];
		m_right_trp_used[i] = handle.m_right_trp_used[i];
		m_right_trp_plead[i] = handle.m_right_trp_plead[i];
		m_right_trp_ulead[i] = handle.m_right_trp_ulead[i];
	}
}
void JXJ::crJXJFormationSlidingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crScrollBarWidgetNode *)param;
		break;
	default:
		break;
	}
}
void JXJ::crJXJFormationSlidingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_title_lead = str;
		break;
	case 2:
		m_title_used = str;
		break;
	case 3:
		m_title_remain = str;
		break;
	case 4:
		m_right_trp_total[0] = str;//可分配兵力
		break;
	case 5:
		m_right_trp_total[1] = str;
		break;
	case 6:
		m_right_trp_total[2] = str;
		break;
	case 7:
		m_right_trp_used[0] = str;//已分配兵力
		break;
	case 8:
		m_right_trp_used[1] = str;
		break;
	case 9:
		m_right_trp_used[2] = str;
		break;
	case 10:
		m_right_trp_plead[0] = str;//单兵领导力
		break;
	case 11:
		m_right_trp_plead[1] = str;
		break;
	case 12:
		m_right_trp_plead[2] = str;
		break;
	case 13:
		m_right_trp_ulead[0] = str; //消耗领导力
		break;
	case 14:
		m_right_trp_ulead[1] = str;
		break;
	case 15:
		m_right_trp_ulead[2] = str;
		break;
	case 16:
		m_editbox = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJFormationSlidingMethod::operator()(crHandle &handle)
{				

	// temp
	void *param;
	int index = m_index;
	// PlayerGameData
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	crData *player_data = player->getDataClass();										// player data
	crData *player_role_data = player->getMainRole()->getDataClass();						// player role data

	// get formations information
	player_data->getParam(WCHDATA_JXJFormationInfoVec, param);
	FormationInfoVec *formations = (FormationInfoVec *)param;
	// tables
	player_data->getParam(WCHDATA_JXJTroopsMap, param);
	TroopsMap *troops =	(TroopsMap *)param;			

	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	// entities
	crCanvasNode *formation_canvas = m_this->getParentCanvas();
	crData *formation_data = formation_canvas->getDataClass();
	crStaticTextWidgetNode *stxt_total;
//	crStaticTextWidgetNode *stxt_othertotal;
	crStaticTextWidgetNode *stxt_otheruse;
	crStaticTextWidgetNode *stxt_used;
	crStaticTextWidgetNode *stxt_plead;
	crStaticTextWidgetNode *stxt_tlead;

	short perMaxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPerFormationMaxCount,viplv).c_str());
	//// check troops limit
	int total_lead =atoi((dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_lead)))->getUTF8String().c_str());
	stxt_total = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_total[m_index]));

	int cur_trp_total = 0;//atoi(stxt_total->getUTF8String().c_str());
	
	int cur_trp_other = 0; // other row formation used troops
	//formation_data->getParam(WCHDATA_JXJForedCells, param);
	//int *heds = (int *)param;
	formation_data->getParam(WCHDATA_JXJTempFormationVec, param);
	FormationInfoVec *tempformation = (FormationInfoVec *)param;
	// check index
	if ((*tempformation)[index] == NULL)
	{
		return;
	}
	if((*tempformation)[index]->getAbstractID() < 0)
	{
		return;
	}
	TroopsMap::iterator troopitr = troops->find((*tempformation)[index]->getAbstractID());
	if (troopitr != troops->end())
	{
		cur_trp_total = troopitr->second;
	}
	int cur_trp_selected = int(m_this->getValue());
	int fsize = formations->size();
	for(int i = 0; i< 3; i++)
	{
		if ((*tempformation)[i]!=NULL)
		{
			if(i != index && (*tempformation)[i]->getAbstractID() == (*tempformation)[index]->getAbstractID())
			{
				stxt_otheruse = 
					dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[i]));
				cur_trp_other += atoi(stxt_otheruse->getUTF8String().c_str());
			}
		}
		if (i<fsize && (*formations)[i]!=NULL)
		{
			if((*formations)[i]->getAbstractID() == (*tempformation)[index]->getAbstractID())
			{
				cur_trp_total += (*formations)[i]->getCount();
			}
		}
	}

	//stxt_total ->setString(crArgumentParser::appItoa(cur_trp_total - cur_trp_other));
	//for(int i = 0; i< 3; i++)
	//{
	//	if ((*tempformation)[i]!=NULL)
	//	{
	//		if(i != index && (*tempformation)[i]->getAbstractID() == (*tempformation)[index]->getAbstractID())
	//		{
	//			stxt_othertotal = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[i]));
	//			stxt_othertotal->setString(crArgumentParser::appItoa(cur_trp_total - cur_trp_other));
	//		}
	//	}
	//}
	if(cur_trp_other + cur_trp_selected > cur_trp_total)
	{
		cur_trp_selected = cur_trp_total - cur_trp_other;
	}
	if (cur_trp_selected < 0)
	{
		cur_trp_selected = 0;
	}

	if(cur_trp_selected>perMaxCount)
		cur_trp_selected = perMaxCount;
	int cur_plead = 0;
	int oth_lead_used = 0;
	for(int i = 0; i < 3; i++)
	{
		if(i == index)
		{
			stxt_plead = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_plead[i]));
			cur_plead = atoi(stxt_plead->getUTF8String().c_str());
		}
		else
		{
			stxt_tlead = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_ulead[i]));
			oth_lead_used += atoi(stxt_tlead->getUTF8String().c_str());
		}
	}

	if(cur_plead * cur_trp_selected + oth_lead_used > total_lead)
	{
		cur_trp_selected = int((total_lead - oth_lead_used) / cur_plead);
	}

	m_this->outerSetValue(cur_trp_selected);

	// show data
	// selected data
	stxt_used = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_used[m_index]));
	if (stxt_used)
	{
		stxt_used->setString(crArgumentParser::appItoa(cur_trp_selected));
		stxt_used->setVisiable(true);
	}

	// per used lead
	stxt_used = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_right_trp_ulead[m_index]));
	if (stxt_used)
	{
		stxt_used->setString(crArgumentParser::appItoa(cur_trp_selected * cur_plead));
		stxt_used->setVisiable(true);
	}

	// total used lead
	stxt_used = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_used));
	if (stxt_used)
	{
		stxt_used->setString(crArgumentParser::appItoa(cur_trp_selected * cur_plead + oth_lead_used));
		stxt_used->setVisiable(true);
	}

	// remaind lead
	stxt_used = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_title_remain));
	if (stxt_used)
	{
		stxt_used->setString(crArgumentParser::appItoa(total_lead - cur_trp_selected * cur_plead - oth_lead_used));
		stxt_used->setVisiable(true);
	}

	//editbox
	stxt_used = dynamic_cast<crStaticTextWidgetNode *>(formation_canvas->getWidget(m_editbox));
	if (stxt_used)
	{
		stxt_used->setString(crArgumentParser::appItoa(cur_trp_selected));
		stxt_used->setVisiable(true);
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJBattleTimterRuntimeMethod
// On  : Battle Timer Runtime
// Desp: 更新战场计时器
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
crJXJBattleTimterRuntimeMethod::crJXJBattleTimterRuntimeMethod()
{}
crJXJBattleTimterRuntimeMethod::crJXJBattleTimterRuntimeMethod(const crJXJBattleTimterRuntimeMethod& handle):
	m_canvas(handle.m_canvas)
{}
void crJXJBattleTimterRuntimeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = *(float *)(LOINT64(param64));
		}
		break;
	}
}
void crJXJBattleTimterRuntimeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}
void crJXJBattleTimterRuntimeMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	CRNetApp::crPlayerGameData *player = CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData();
	if(player && canvas)
	{
		void *param;
		//crData *canvas_data = canvas->getDataClass();
		//canvas_data->getParam(WCHDATA_JXJBattleUIType, param);
		//if (canvas_data)
		//{
			//int select = *((int *)param);
			crData *player_data = player->getDataClass();
			player_data->excHandle(MAKEINT64(WCH_LockData,1));
			//if (select == BT_CityBattle)
			//{
				player_data->getParam(WCHDATA_JXJOpenBattle, param);
				bool is_battle_opened = *((bool *)param);
				if(is_battle_opened)
				{
					player_data->getParam(WCHDATA_JXJBattleTimer, param);
					int *timer = (int *)param;
					if (*timer>0)
					{
						*timer -= 1;
					}
				}
			//}
			//else if(select == BT_GateBattle)
			//{
			//	player_data->getParam(WCHDATA_JXJOpenPass, param);
			//	bool is_opened = *((bool *)param);
			//	if(is_opened)
			//	{
			//		player_data->getParam(WCHDATA_JXJPassTimer, param);
			//		short *timer = (short *)param;
			//		if (*timer>0)
			//		{
			//			*timer -= 1;
			//		}
			//	}
			//}
			player_data->excHandle(MAKEINT64(WCH_LockData,0));
		//}
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJBattleTimerQueryMethod
// On  : Query Battle Timer
// Desp: 更新战场计时器
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//int crJXJChatReshowMethod::m_slinkIndex = 10;
JXJ::crJXJChatReshowMethod::crJXJChatReshowMethod():m_textRow(0)
{}
JXJ::crJXJChatReshowMethod::crJXJChatReshowMethod(const crJXJChatReshowMethod& handle):crMethod(handle),m_textRow(handle.m_textRow)
{}
void JXJ::crJXJChatReshowMethod::inputParam(int i, void *param)
{
}
void JXJ::crJXJChatReshowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_chat_canvas = str;
		break;
	case 1:
		m_chat_show = str;
		break;
	case 2:
		m_chat_mode = str;
		break;
	case 3:
		m_chat_cfg = str;
		break;
	case 4:
		m_input = str;
		break;
	case 5:
		m_scroll = str;
		break;
	case 6:
		m_RoomChatCanvas  = str;
	}
}
void JXJ::crJXJChatReshowMethod::operator()(crHandle &handle)
{
	void *param;
	// widgets
	crCanvasNode *chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_chat_canvas);
	if(!chat_canvas) chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_RoomChatCanvas);

	if(!chat_canvas) return;
	// 	crData *chat_canvas_data = chat_canvas->getDataClass();
	// 	if(!chat_canvas_data) return;
	crHypertextWidgetNode *chat_show = dynamic_cast<crHypertextWidgetNode *>(chat_canvas->getWidget(m_chat_show));
	crHypertextWidgetNode *chat_show2 = dynamic_cast<crHypertextWidgetNode *>(chat_canvas->getWidget(m_chat_show + "2"));
	crRadioGroupWidgetNode *chat_select = dynamic_cast<crRadioGroupWidgetNode *>(chat_canvas->getChildNode(m_chat_mode));
	crEditWidgetNode * chat_input = dynamic_cast<crEditWidgetNode *>(chat_canvas->getChildNode(m_input));
	crScrollBarWidgetNode *scrollbar = dynamic_cast<crScrollBarWidgetNode *>(chat_canvas->getWidget(m_scroll));

	CRNetApp::crPlayerGameData *player = CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData();
	crData *player_data = player->getDataClass();
	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	if(chat_show)
		chat_show->setDefaultScrollPos(1);
	if(chat_show2)
		chat_show2->setDefaultScrollPos(1);
	int max_row = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine,viplv).c_str());
	if (chat_select)
	{
		std::string myname = player->getCharacterName();
		JXJChatMode chat_mode = JXJChatMode(chat_select->getSelect());
		void *param;

		crMyPlayerData *me = crMyPlayerData::getInstance();
		if(me && me->getPlayerGameData())
		{
			crData *playerData = me->getPlayerGameData()->getDataClass();
			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJChatRecordMap,param);
				ChatRecordMap *chat_records = (ChatRecordMap *)param;
				playerData->getParam(WCHDATA_JXJChatItemLinkMap,param);
				std::map<int,std::pair<int,int>> * itemLinkMap = (std::map<int,std::pair<int,int>> *)param;
				playerData->getParam(WCHDATA_JXJChatInviteLinkMap,param);
				std::map<int,InviteInfo>  * invitelinkMap = (std::map<int,InviteInfo> *)param;

				playerData->getParam(WCHDATA_JXJChatBattleReportLinkMap, param);
				std::map<int, std::pair<std::string, int>>  * battleReportlinkMap = (std::map<int, std::pair<std::string, int>> *)param;
				playerData->getParam(WCHDATA_JXJChatCallForHelpLinkMap, param);
				std::map<int, std::pair<int, int>>  * callForHelplinkMap = (std::map<int, std::pair<int, int>> *)param;

				playerData->getParam(WCHDATA_JXJChatPraiseLinkMap, param);
				std::map<int, int>  * praiselinkMap = (std::map<int, int> *)param;

				if(chat_records)
				{
					if(chat_show) chat_show->clearText();
					if(chat_show2) chat_show2->clearText();

					rcfg::ConfigScript cfg_script;
					ChatRecordDeque & chatDeque = (*chat_records)[chat_mode];
					ChatRecordDeque::iterator itr_deque = chatDeque.begin();
					for(;itr_deque!=chatDeque.end();itr_deque++)
					{
						crGlobalHandle::getInstance()->getStreamScript(itr_deque->get(),cfg_script);
						if(chat_show)chat_show->setHypertext(cfg_script,true);
						if(chat_show2) chat_show2->setHypertext(cfg_script,true);
					}

					ref_ptr<crData> chatData;
					ref_ptr<crData> chatData2;
					crHandle *linkhandle = NULL;
					crHandle *clonelink = NULL;
					if(chat_show) chatData = chat_show->getDataClass();
					if(chat_show2) chatData2 = chat_show2->getDataClass();

					if(chatData.valid())
					{
						std::map<int,std::pair<int,int>>::iterator itr_link = itemLinkMap->begin(); 
						for(;itr_link!=itemLinkMap->end();itr_link++)
						{
							linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown,2));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3,&itr_link->second.first);
							clonelink->inputParam(4,&itr_link->second.second);
							chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown,itr_link->first),clonelink);
						}

						std::map<int,InviteInfo>::iterator itr_link_i = invitelinkMap->begin(); 
						for(;itr_link_i!=invitelinkMap->end();itr_link_i++)
						{
							linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown,4));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3,&itr_link_i->second.type);
							clonelink->inputParam(4,&itr_link_i->second.playerid);
							clonelink->inputParam(5,&itr_link_i->second.id);
							chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown,itr_link_i->first),clonelink);
						}
						std::map<int, std::pair<std::string, int>>::iterator itr_link_re = battleReportlinkMap->begin();
						for (; itr_link_re != battleReportlinkMap->end(); itr_link_re++)
						{
							linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 5));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_re->second.first);
							clonelink->inputParam(4, &itr_link_re->second.second);
							chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_re->first), clonelink);
						}
						std::map<int, int>::iterator itr_link_pre = praiselinkMap->begin();
						for (; itr_link_pre != praiselinkMap->end(); itr_link_pre++)
						{
							linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 7));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_pre->second);
							chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_pre->first), clonelink);
						}
						std::map<int, std::pair<int, int>>::iterator itr_link_help = callForHelplinkMap->begin();
						for (; itr_link_help != callForHelplinkMap->end(); itr_link_help++)
						{
							linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 6));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_help->second.first);
							clonelink->inputParam(4, &itr_link_help->second.second);
							chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_help->first), clonelink);
						}
					}

					if(chatData2.valid())
					{
						std::map<int,std::pair<int,int>>::iterator itr_link = itemLinkMap->begin(); 
						for(;itr_link!=itemLinkMap->end();itr_link++)
						{
							linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown,2));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3,&itr_link->second.first);
							clonelink->inputParam(4,&itr_link->second.second);
							chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown,itr_link->first),clonelink);
						}

						std::map<int,InviteInfo>::iterator itr_link_i = invitelinkMap->begin(); 
						for(;itr_link_i!=invitelinkMap->end();itr_link_i++)
						{
							linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown,4));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3,&itr_link_i->second.type);
							clonelink->inputParam(4,&itr_link_i->second.playerid);
							clonelink->inputParam(5,&itr_link_i->second.id);
							chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown,itr_link_i->first),clonelink);
						}

						std::map<int, std::pair<std::string, int>>::iterator itr_link_re = battleReportlinkMap->begin();
						for (; itr_link_re != battleReportlinkMap->end(); itr_link_re++)
						{
							linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 5));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_re->second.first);
							clonelink->inputParam(4, &itr_link_re->second.second);
							chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_re->first), clonelink);
						}
						std::map<int, int>::iterator itr_link_pre = praiselinkMap->begin();
						for (; itr_link_pre != praiselinkMap->end(); itr_link_pre++)
						{
							linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 7));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_pre->second);
							chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_pre->first), clonelink);
						}
						std::map<int, std::pair<int, int>>::iterator itr_link_help = callForHelplinkMap->begin();
						for (; itr_link_help != callForHelplinkMap->end(); itr_link_help++)
						{
							linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 6));
							clonelink = linkhandle->clone();
							clonelink->inputParam(3, &itr_link_help->second.first);
							clonelink->inputParam(4, &itr_link_help->second.second);
							chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_help->first), clonelink);
						}
					}
				}
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJBattleTimerQueryMethod
// On  : Query Battle Timer
// Desp: 更新战场计时器
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJChatParserMethod::crJXJChatParserMethod()
{}
JXJ::crJXJChatParserMethod::crJXJChatParserMethod(const crJXJChatParserMethod& handle)
{}
void JXJ::crJXJChatParserMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}
void JXJ::crJXJChatParserMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_chat_show = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJChatParserMethod::operator()(crHandle &handle)
{}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJBattleChatShowMethod
// On  : chat canvas show in battle
// Desp: 战场聊天框初始化
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJBattleChatShowMethod::crJXJBattleChatShowMethod()
{}
JXJ::crJXJBattleChatShowMethod::crJXJBattleChatShowMethod(const crJXJBattleChatShowMethod& handle)
{}
void JXJ::crJXJBattleChatShowMethod::inputParam(int i, void *param)
{}
void JXJ::crJXJBattleChatShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJBattleChatShowMethod::operator()(crHandle &handle)
{
	int  a  = 1;
}


//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJFormationCloseMethod
// On  : reset flag
// Desp: 编队关闭
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJFormationCloseMethod::crJXJFormationCloseMethod()
{}
JXJ::crJXJFormationCloseMethod::crJXJFormationCloseMethod(const crJXJFormationCloseMethod& handle)
{}
void JXJ::crJXJFormationCloseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}
void JXJ::crJXJFormationCloseMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_cards = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJFormationCloseMethod::operator()(crHandle &handle)
{
	crCanvasNode *formation_canvas = m_this->getParentCanvas();
	crData *formation_canvas_data = formation_canvas->getDataClass();
	formation_canvas_data->excHandle(MAKEINT64(WCH_LockData,1));
	formation_canvas_data->addParam(WCHDATA_JXJFormationMove, "0");
	formation_canvas_data->excHandle(MAKEINT64(WCH_LockData,0));
	crFilterRenderManager::getInstance()->closeCanvas(formation_canvas);
	//crFilterRenderManager::getInstance()->showCanvas(m_cards, 0);
}



//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIJangkaListShowMethod
// On  : update card canvas
// Desp: 更新将卡数据
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIJangkaListShowMethod::crJXJUIJangkaListShowMethod()
{}
JXJ::crJXJUIJangkaListShowMethod::crJXJUIJangkaListShowMethod(const crJXJUIJangkaListShowMethod& handle):
	m_this(handle.m_this),
	m_cardtyperadio(handle.m_cardtyperadio),
	m_colorradio(handle.m_colorradio),
	m_bagtyperadio(handle.m_bagtyperadio),
	m_formation(handle.m_formation),
	m_arrangebtn(handle.m_arrangebtn)
{}
void JXJ::crJXJUIJangkaListShowMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}
void JXJ::crJXJUIJangkaListShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_colorradio = str;
		break;
	case 1:
		m_cardtyperadio = str;
		break;
	case 2:
		m_bagtyperadio = str;
		break;
	case 3:
		m_formation = str;
		break;
	case 4:
		m_arrangebtn = str;
		break;
	//case 1:
	//	m_for_equips[0] = str;
	//	break;
	//case 2:
	//	m_for_equips[1] = str;
	//	break;
	//case 3:
	//	m_for_equips[2] = str;
	//	break;
	//case 4:
	//	m_for_equips[3] = str;
	//	break;
	//case 5:
	//	m_for_equips[4] = str;
	//	break;
	//case 6:
	//	m_for_equips[5] = str;
	//	break;
	//case 7:
	//	m_for_equips[6] = str;
	//	break;
	//case 8:
	//	m_for_equips[7] = str;
	//	break;
	//case 9:
	//	m_for_equips[8] = str;
	//	break;
	default:
		break;
	}
}
void JXJ::crJXJUIJangkaListShowMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		// temp use
		void *param;

		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->excHandle(MAKEINT64(WCH_LockData,1));
		braindata->getParam(WCHDATA_JXJUpgradeJiangkaIndex,param);
		short *upgradejiangkaindex = (short *)param;
		braindata->getParam(WCHDATA_JXJUpgradeNewJiangkaId,param);
		int *newjiangkaindex = (int *)param;
		*upgradejiangkaindex = -1;
		*newjiangkaindex = 0;
		braindata->excHandle(MAKEINT64(WCH_LockData,0));

		//crRadioGroupWidgetNode *bagtyperadio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_bagtyperadio));
		//if (bagtyperadio)
		//{
		//	//bagtyperadio->select(0);
		//}
		crButtonWidgetNode *arrangebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_arrangebtn));
		if (arrangebtn)
		{
			arrangebtn->setVisiable(true);
			//arrangebtn->setEnable(true);
		}
		//crRadioGroupWidgetNode *colorradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_colorradio));
		//if (colorradio)
		//{
		//	colorradio->select(0);
		//}
		//crRadioGroupWidgetNode *cardtyperadio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_cardtyperadio));
		//if (cardtyperadio)
		//{
		//	cardtyperadio->select(0);
		//}
		// widget
		crCanvasNode *can_formation = crFilterRenderManager::getInstance()->findCanvas(m_formation);
		if (can_formation)
		{
			crData *data = m_this->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			int can_jk_type = can_formation->getVisiable() ? 1 : 0;
			data->inputParam(WCHDATA_JXJForJKType, &can_jk_type);

			// card list in player card bag
			CRNetApp::crPlayerGameData *player = CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData();
			crData *player_data = player->getDataClass();
			CRCore::ScopedLock<crData> lock(*player_data);
			player_data->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec *player_card = (JXJItemBagVec *)param;
			data->getParam(WCHDATA_JXJForJKVec,param);
			JXJItemBagVec *data_card= (JXJItemBagVec *)param;
			//data_card->clear();
			//*data_card = *player_card;
			data_card->clear();
			for(int i = 0; i < player_card->size(); i++)
			{
				ref_ptr<crBagItemData> temp = NULL;
				if((*player_card)[i].get())
				{
					temp = new crBagItemData;
					temp->setItemID((*player_card)[i]->getItemID());
					temp->setItemCount((*player_card)[i]->getItemCount());
				}
				data_card->push_back(temp);
			}

			braindata->excHandle(MAKEINT64(WCH_LockData,1));
			braindata->getParam(WCHDATA_JXJSaleBagVec,param);
			JXJItemBagVec *salebagvec = (JXJItemBagVec *)param;
			if (salebagvec->size()>0)
			{
				salebagvec->clear();
			}
			braindata->excHandle(MAKEINT64(WCH_LockData,0));

			// calculate jk vector
			ref_ptr<crBagItemData> newcard = NULL;
			if(can_jk_type == 1)
			{
				// add formation info
			//	player_data->getParam(WCHDATA_JXJFormationInfoVec,param);
			//	FormationInfoVec *troop_infors = (FormationInfoVec *)param;

			//	for(FormationInfoVec::iterator itr = troop_infors->begin(); itr != troop_infors->end(); itr++)
			//	{
			//		crVector2i equips = itr->get()->getEquips();
			//		for(int i = 0; i < 2; i++)
			//		{
			//			// 没有将卡
			//			if(equips[i] < 0) continue;

			//			// 背包查找
			//			//bool find_flag = false;
			//			for(JXJItemBagVec::iterator itr = data_card->begin(); itr != data_card->end(); itr++)
			//			{
			//				// empty cell 
			//				if(!itr->get())
			//				{
			//					newcard = new crBagItemData;
			//					newcard->setItemID(equips[i]);
			//					newcard->setItemCount(1);
			//					(*itr) = newcard;
			//					break;
			//				}
			//				//continue;

			//				//if(itr->get()->getItemID() == equips[i])
			//				//{
			//				//	find_flag = true;
			//				//	itr->get()->setItemCount(itr->get()->getItemCount() + 1);
			//				//	break;
			//				//}
			//			}
			//			// 发现背包将卡
			//			//if(find_flag) continue;

			//			//// 没法背包将卡
			//			//// 插入将卡
			//			//for(int i_i = 0; i_i < data_card->size(); i_i++)
			//			//{
			//			//	if(!(*data_card)[i_i].get())
			//			//	{
			//			//		(*data_card)[i_i] = new crBagItemData;
			//			//		(*data_card)[i_i].get()->setItemID(equips[i]);
			//			//		(*data_card)[i_i].get()->setItemCount(1);
			//			//		break;
			//			//	}
			//			//}
			//		}
			//	}

				std::vector<int> cardvec;
				std::vector<int>::iterator carditr;
				// remove data in formation canvas
				crData *formation_data = can_formation->getDataClass();
				//formation_data->getParam(WCHDATA_JXJForedCells, param);
				//int *right_arms = (int *)param;
				//formation_data->getParam(WCHDATA_JXJForedHeadCells, param);
				//int *right_eqps = (int *)param;
				int savedformationselect = 0;
				ref_ptr<crFormationInfo> formation;
				ref_ptr<crRadioGroupWidgetNode> FormationRaido2 = dynamic_cast<crRadioGroupWidgetNode *>(can_formation->getWidget("FormationRaido2"));
				if (FormationRaido2.valid())
				{
					savedformationselect = FormationRaido2->getSelect();
				}
				formation_data->getParam(WCHDATA_JXJTempFormationVec, param);
				FormationInfoVec *tempfomationvec = (FormationInfoVec *)param;
				FormationInfoVec::iterator savedfitr;
				FormationInfoVec savedvec;
				player_data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
				SavedFormationInfoVec *savedformationVec = (SavedFormationInfoVec *)param;
				int savedformationindex = 0;
				crVector2i equips;
				if (savedformationselect < savedformationVec->size())
				{
					savedvec = (*savedformationVec)[savedformationselect].second;
					for (FormationInfoVec::iterator itr = savedvec.begin(); itr != savedvec.end(); itr++)
					{
						equips = itr->get()->getEquips();
						for (int i = 0; i < 2; i++)
						{
							// 没有将卡
							if (equips[i] <= 0) continue;

							// 背包查找
							//bool find_flag = false;
							for (JXJItemBagVec::iterator itr = data_card->begin(); itr != data_card->end(); itr++)
							{
								// empty cell 
								if (!itr->get())
								{
									newcard = new crBagItemData;
									newcard->setItemID(equips[i]);
									newcard->setItemCount(1);
									(*itr) = newcard;
									break;
								}
							}
						}
					}
				}
				//for (SavedFormationInfoVec::iterator sfitr = savedformationVec->begin();
				//	sfitr != savedformationVec->end(); ++savedformationindex,++sfitr)
				//{
				//	if (savedformationindex != savedformationselect)
				//	{
				//		for (savedfitr = sfitr->second.begin(); savedfitr != sfitr->second.end(); ++savedfitr)
				//		{
				//			if (savedfitr->get())
				//			{
				//				for (int col = 0; col < 2; col++)
				//				{
				//					if (savedfitr->get()->getEquips()[col] <= 0) continue;
				//					cardvec.push_back(savedfitr->get()->getEquips()[col]);
				//				}
				//			}
				//		}
				//	}
				//}
				for(int row = 0; row < 3; row++)
				{
					// 无效编制行
					if((*tempfomationvec)[row] == NULL/* || (*tempfomationvec)[row]->getAbstractID() < 0*/) continue;
					formation = (*tempfomationvec)[row];
					for(int col = 0; col < 2; col++)
					{
						// 没有编制将卡
						if(formation->getEquips()[col] <= 0) continue;
						cardvec.push_back(formation->getEquips()[col]);
					}
				}
				// 减除
				for (carditr = cardvec.begin(); carditr != cardvec.end(); ++carditr)
				{
					for (JXJItemBagVec::iterator itr = data_card->begin(); itr != data_card->end(); itr++)
					{
						if (!itr->get()) continue;

						if (itr->get()->getItemID() == (*carditr) && itr->get()->getItemCount() > 0)
						{
							(*itr) = NULL;
							break;
						}
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
		//crRadioGroupWidgetNode *bagtyperadio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_bagtyperadio));
		//if (bagtyperadio)
		//{
		//	bagtyperadio->select(JKBT_Jiangka);
		//}
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIBattleOpenMethod
// On  : 
// Desp: 客户端计时器更新
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIBattleOpenMethod::crJXJUIBattleOpenMethod()
	:m_select(0)
{}
JXJ::crJXJUIBattleOpenMethod::crJXJUIBattleOpenMethod(const crJXJUIBattleOpenMethod& handle)
	:m_select(handle.m_select),
	m_jr1switch(handle.m_jr1switch),
	m_zoom(handle.m_zoom),
	m_enlarge(handle.m_enlarge)
{
	for (int i = 0;i <6;i++)
	{
		m_canvas[i] = handle.m_canvas[i];
	}
}
void JXJ::crJXJUIBattleOpenMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}
void JXJ::crJXJUIBattleOpenMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_select = atoi(str.c_str());
		break;
	case 1:
		m_canvas[0] = str;
		break;
	case 2:
		m_canvas[1] = str;
		break;
	case 3:
		m_canvas[2] = str;
		break;
	case 4:
		m_canvas[3] = str;
		break;
	case 5:
		m_canvas[4] = str;
		break;
	case 6:
		m_canvas[5] = str;
		break;
	case 7:
		m_canvas[6] = str;
		break;
	case 8:
		m_canvas[7] = str;
		break;
	case 9:
		m_jr1switch = str;
		break;
	case 10:
		m_zoom = str;
		break;
	case 11:
		m_enlarge = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIBattleOpenMethod::operator()(crHandle &handle)
{
	for (int i = 0; i <8;i ++)
	{
		crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas[i]);
		if (!canvas)
		{
			continue;
		}
		if (i == 1)
		{
			crMultiSwitch *jr1switch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_jr1switch));
			crButtonWidgetNode *zoom = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_zoom));
			crButtonWidgetNode *enlarge = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_enlarge));
			if(jr1switch)jr1switch->setActiveSwitchSet(0);
			if(zoom)zoom->setVisiable(true);
			if(enlarge)enlarge->setVisiable(false);

			//crData *canvas_data = canvas->getDataClass();
			//canvas_data->inputParam(WCHDATA_JXJBattleUIType, &m_select);
		}
		//*((int *)param) = m_select;
		crFilterRenderManager::getInstance()->showCanvas(canvas, true);
	}
}

//// - - - - - - - - - - - - - - - - - - - - - - - - 关隘战Battle Field - - - - - - - - - - - - - - - - - - - - - - - - 
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassSendingRuntimeMethod
//// On  : 
//// Desp: 服务器定时向客户端发送数据
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJPassSendingRuntimeMethod::crJXJPassSendingRuntimeMethod()
//{}
//JXJ::crJXJPassSendingRuntimeMethod::crJXJPassSendingRuntimeMethod(const crJXJPassSendingRuntimeMethod& handle)
//{}
//void JXJ::crJXJPassSendingRuntimeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (CRNetApp::crRoom *)param;
//		break;
//	}
//}
//void JXJ::crJXJPassSendingRuntimeMethod::addParam(int i, const std::string& str)
//{}
//void JXJ::crJXJPassSendingRuntimeMethod::operator()(crHandle &handle)
//{
//	// parse
//	// remain scores
//	// check point states
//	
//	// server entities
//	crNetConductor *scene_server = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	crNetDataManager *scene_datamgr = scene_server->getNetDataManager();
//
//	// parse package
//	// red remains
//	// blue remains
//	// size of checkpoints
//	// check point scores lists
//	void *param;
//	crData *room_data = m_this->getDataClass();
//	room_data->excHandle(MAKEINT64(WCH_LockData,1));
//	room_data->getParam(WCHDATA_JXJRoomDataFlag, param);
//	if(!*(bool *)param) return;
//
//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
//	room_data->getParam(WCHDATA_JXJCheckPointVec, param);
//	PassCheckVec *checks = (PassCheckVec *)param;
//	stream->createBuf( 4 + 3*checks->size());
//	room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//	stream->_writeShort(*(short *)param);
//	room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//	stream->_writeShort(*(short *)param);
//	for(PassCheckVec::iterator itr = checks->begin(); itr != checks->end(); itr++)
//	{
//		stream->_writeShort(itr->get()->getScore());
//		stream->_writeChar(itr->get()->getType());
//	}
//
//	// boardcast send
//	m_this->lockPlayerList();
//	crRoom::PlayerList &players = m_this->getPlayerList();
//	for(crRoom::PlayerList::iterator itr = players.begin(); itr != players.end(); itr++)
//	{
//		ref_ptr<crSceneServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crSceneServerPlayerData *>(scene_server->getNetDataManager()->getPlayerData(itr->get()->getPlayerID()));
//		if(player_net_data.valid())
//		{
//			CRNetApp::crPlayerDataEventPacket packet;
//			CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet, itr->get()->getPlayerID(), WCH_JXJPassSendingRuntime,stream.get());
//			scene_server->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(), packet);
//		}
//	}
//	m_this->unlockPlayerList();
//	room_data->excHandle(MAKEINT64(WCH_LockData,0));
//}


//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassMainRuntimeMethod
//// On  : 
//// Desp: 游戏判断逻辑
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJPassMainRuntimeMethod::crJXJPassMainRuntimeMethod()
//{}
//JXJ::crJXJPassMainRuntimeMethod::crJXJPassMainRuntimeMethod(const crJXJPassMainRuntimeMethod& handle)
//{}
//void JXJ::crJXJPassMainRuntimeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (CRNetApp::crRoom *)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_dt = (float*)(LOINT64(param64));
//		}
//		else
//		{
//			m_dt = NULL;
//		}
//		break;
//	}
//}
//void JXJ::crJXJPassMainRuntimeMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		//m_checks = crTableIO::openFile(crGlobalHandle::getInstance()->getTableDir() + str);
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJPassMainRuntimeMethod::operator()(crHandle &handle)
//{
//	// extract pass field information
//	crData *room_data =m_this->getDataClass();
//	room_data->excHandle(MAKEINT64(WCH_LockData,1));
//	void *param;
//	room_data->getParam(WCHDATA_JXJCheckPointVec, param);
//	PassCheckVec *check_points = (PassCheckVec *)param;
//	room_data->getParam(WCHDATA_JXJBattleID,param);
//	int battle_id = *(int *)param;
//	room_data->getParam(WCHDATA_JXJRoomDataFlag, param);
//	bool *is_modifed = (bool *) param;
//	unsigned char deffence = (unsigned char)(LOINT16(LOINT32(battle_id)));
//	CREncapsulation::crTableIO::StrVec	record;
//	if(check_points->empty()) return;
//	int check_span = m_checks->getTitleIndex("兵力差异");
//	int check_speed = m_checks->getTitleIndex("占领速度");
//
//	m_this->lockPlayerList();
//	crRoom::PlayerList &players = m_this->getPlayerList();
//
//	// check every check point
//	for(PassCheckVec::iterator sitr = check_points->begin(); sitr != check_points->end(); sitr++)
//	{
//		int red_arms = 0;
//		int blue_arms = 0;
//		//检测是否为关隘
//		//if(sitr->type == 1) continue;
//
//		// check evry player
//		for(crRoom::PlayerList::iterator pitr = players.begin(); pitr != players.end(); pitr++)
//		{
//			// check every arms
//			ref_ptr<crSceneServerPlayerData> player_data = dynamic_cast<crSceneServerPlayerData *>
//				(crNetContainer::getInstance()->getNetConductor(SceneServer)->getNetDataManager()->getPlayerData(pitr->get()->getPlayerID()));
//			if (player_data.valid() && player_data->getPlayerGameData() && player_data->getPlayerGameData()->getDataClass())
//			{ 
//				player_data->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJShiliID, param);
//				unsigned char side_id = *(int *)param;
//				crSceneServerPlayerData::RoleMap &arms = player_data->getRoleMap();
//
//				for(crSceneServerPlayerData::RoleMap::iterator ritr = arms.begin(); ritr != arms.end(); ritr++)
//				{
//					// calculations
//					crData*	data = ritr->second->getDataClass();
//					data->getParam(WCHDATA_JXJRTArmyCount,param);
//					int armyRTCount = *(unsigned short *)param;
//					crVector3f postion = ritr->second->getPosition();
//					float dis_sq = (postion[0] - sitr->get()->getPosX()) * (postion[0] - sitr->get()->getPosX()) + (postion[1] - sitr->get()->getPosY()) * (postion[1] - sitr->get()->getPosY()); // 距离的平方
//					if(dis_sq <= sitr->get()->getArea() * sitr->get()->getArea())
//					{
//						red_arms += (side_id == deffence ? armyRTCount : 0);
//						blue_arms += (side_id == deffence ? 0 : armyRTCount);
//					}
//				}
//			}
//		}
//
//		// take position
//		short check_limit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckPointLimit).c_str());
//
//		int dif = red_arms - blue_arms;
//		dif = (abs(dif)<10)?10:(abs(dif)/10)*10;
//		if(dif>m_checks->getRowCount()*10)
//			dif = m_checks->getRowCount()*10;
//
//		if(red_arms == blue_arms)
//		{
//			continue;
//		}
//
//		if(red_arms > blue_arms && sitr->get()->getScore() <= -check_limit)
//		{
//			sitr->get()->setScore(-check_limit);
//			*is_modifed = true;
//			continue;
//		}
//
//		if(red_arms < blue_arms && sitr->get()->getScore() >= check_limit)
//		{
//			*is_modifed = true;
//			sitr->get()->setScore(check_limit);
//			continue;
//		}
//
//		CREncapsulation::crTableIO::StrVec	record;
//		if(m_checks->queryOneRecord(check_span, crArgumentParser::appItoa(dif), record))
//		{
//			*is_modifed = true;
//			int dir = (red_arms>=blue_arms)?1:-1;
//			short score = sitr->get()->getScore();
//			score -= dir*atoi(record[check_speed].c_str());	
//			sitr->get()->setScore(score);
//		}
//	}
//	m_this->unlockPlayerList();
//	room_data->excHandle(MAKEINT64(WCH_LockData,0));
//}
//
//
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassCheckGameOverMethod
//// On  : 
//// Desp: 判断游戏是否结束 - 关隘一方资源消耗为零就结束
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJPassCheckGameOverMethod::crJXJPassCheckGameOverMethod()
//{}
//JXJ::crJXJPassCheckGameOverMethod::crJXJPassCheckGameOverMethod(const crJXJPassCheckGameOverMethod& handle)
//{}
//void JXJ::crJXJPassCheckGameOverMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (CRNetApp::crRoom *)param;
//		break;
//	}
//}
//void JXJ::crJXJPassCheckGameOverMethod::addParam(int i, const std::string& str)
//{}
//void JXJ::crJXJPassCheckGameOverMethod::operator()(crHandle &handle)
//{
//	// extract pass field information
//	// define
//	bool is_game_over = false;
//	void *param;
//	crData *room_data = m_this->getDataClass();
//	room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//	short red_remain = *(short *)param;
//	room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//	short blue_remain = *(short *)param;
//
//	if(red_remain <= 0 || blue_remain <= 0)
//	{
//		is_game_over = true;
//	}
//	handle.outputParam(0, &is_game_over);
//}
//
//
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassScoreRuntimeMethod
//// On  : 
//// Desp: 关隘中根据
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJPassScoreRuntimeMethod::crJXJPassScoreRuntimeMethod()
//{}
//JXJ::crJXJPassScoreRuntimeMethod::crJXJPassScoreRuntimeMethod(const crJXJPassScoreRuntimeMethod& handle)
//{}
//void JXJ::crJXJPassScoreRuntimeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (CRNetApp::crRoom *)param;
//		break;
//	}
//}
//void JXJ::crJXJPassScoreRuntimeMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		{
//			std::string::size_type bindex = 0;
//			std::string::size_type eindex = str.size();
//
//			while( bindex < eindex)
//			{
//				std::string::size_type findex = str.find_first_of(',', bindex);
//				if(findex == std::string::npos) findex = str.size() - 1;
//				m_downs.push_back(atoi(str.substr(bindex, findex - bindex + 1).c_str()));
//				bindex = findex + 1;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJPassScoreRuntimeMethod::operator()(crHandle &handle)
//{
//	// get checkpoint infor
//	void *param;
//	crData *room_data = m_this->getDataClass();
//	room_data->excHandle(MAKEINT64(WCH_LockData,1));
//	room_data->getParam(WCHDATA_JXJCheckPointVec, param);
//	PassCheckVec *check_points = (PassCheckVec *)param;
//
//	// get room score data
//	room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//	short *red_remain = (short *)param;
//	room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//	short *blue_remain = (short *)param;
//	room_data->getParam(WCHDATA_JXJRoomDataFlag, param);
//	bool *is_modifed = (bool *) param;
//
//	room_data->getParam(WCHDATA_JXJPassCaptureCheck,param);
//	char *isPassCapture = (char *)param;
//	// cal score
//	int mid_span = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckPointLimit).c_str());
//	int red_checks = 0;
//	int blue_checks = 0;
//	char pass_check = 0;//关隘是否被占领 0为中立 -1为红方占领 1为蓝方占领
//	for(PassCheckVec::iterator itr = check_points->begin(); itr != check_points->end(); itr++)
//	{
//		if(itr->get()->getType() == 1)
//		{
//			if(itr->get()->getScore() <= -mid_span )
//				pass_check = -1;
//			else if(itr->get()->getScore() >=mid_span)
//				pass_check = 1;
//		}
//		else
//		{
//			int score = itr->get()->getScore();
//			red_checks += score > 0 ? 0 : (score > -mid_span ? 0 : 1);
//			blue_checks += score < 0 ? 0 : (score < mid_span ? 0 : 1);
//		}
//	}
//	if(pass_check != 0)
//	{
//		if(pass_check == -1 && (*isPassCapture != pass_check))
//		{// 红方占领关隘
//			*blue_remain-=500;
//			*blue_remain = *blue_remain < 0 ? 0 : *blue_remain;
//			*is_modifed = true;
//			*isPassCapture = pass_check;
//			pass_check = 0;
//		}
//		else if(pass_check == 1 && (*isPassCapture != pass_check)) //蓝方占领关隘
//		{
//			*red_remain-=500;
//			*red_remain = *red_remain < 0 ? 0 : *red_remain;
//			*is_modifed = true;
//			*isPassCapture = pass_check;
//			pass_check = 0;
//		}
//	}
//
//	if(blue_checks > 0)
//	{
//		*red_remain -= (m_downs[blue_checks - 1]);
//		*red_remain = *red_remain < 0 ? 0 : *red_remain;
//		*is_modifed = true;
//	}
//
//	if(red_checks > 0)
//	{
//		*blue_remain -= (m_downs[red_checks - 1]);
//		*blue_remain = *blue_remain < 0 ? 0 : *blue_remain;
//		*is_modifed = true;
//	}
//	room_data->excHandle(MAKEINT64(WCH_LockData,0));
//}
//
//
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassScoreRuntimeMethod
//// On  : 
//// Desp: 关隘中根据
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJRecvPassServerSendingMethod::crJXJRecvPassServerSendingMethod()
//{}
//JXJ::crJXJRecvPassServerSendingMethod::crJXJRecvPassServerSendingMethod(const crJXJRecvPassServerSendingMethod& handle)
//{}
//void JXJ::crJXJRecvPassServerSendingMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{
//			//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (CRNetApp::crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (CRCore::crStreamBuf *)(LOINT64(param64));
//			m_net_type = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//void JXJ::crJXJRecvPassServerSendingMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	}
//}
//void JXJ::crJXJRecvPassServerSendingMethod::operator()(crHandle &handle)
//{
//	if(m_net_type == GameClient_Game)
//	{
//		// get room info
//		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
//		crData *room_data = room->getDataClass();
//		void *param;
//		if(room_data)
//		{
//			room_data->excHandle(MAKEINT64(WCH_LockData,1));
//			room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//			short *red_remain = (short *)param;
//			*red_remain = m_stream->_readShort();
//			room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//			short *blue_remain = (short *)param;
//			*blue_remain = m_stream->_readShort();
//
//			
//			room_data->getParam(WCHDATA_JXJCheckPointVec, param);
//			PassCheckVec *checks = (PassCheckVec *)param;
//			for(PassCheckVec::iterator itr = checks->begin(); itr != checks->end(); itr++)
//			{
//				itr->get()->setScore(m_stream->_readShort());
//				itr->get()->setType(m_stream->_readChar());
//				//m_stream->_readFloat();
//				//m_stream->_readChar();
//			}
//			crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//			if(canvas)
//			{
//				canvas->doEvent(WCH_JXJPassCanvasUpdate); //crJXJPassCanavsUIUpdateMethod
//			}
//			room_data->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//	}
//	else if(m_net_type == SceneServer)
//	{
//		// parse
//		// remain scores
//		// check point states
//
//		// server entities
//		crNetConductor *scene_server = crNetContainer::getInstance()->getNetConductor(SceneServer);
//		crNetDataManager *scene_datamgr = scene_server->getNetDataManager();
//		ref_ptr<crSceneServerPlayerData> player_scdata = dynamic_cast<crSceneServerPlayerData *>(scene_datamgr->getPlayerData(m_this->getPlayerID()));
//
//		// parse package
//		// red remains
//		// blue remains
//		// size of checkpoints
//		// check point scores lists
//		if (player_scdata.valid())
//		{
//			void *param;
//			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(scene_datamgr->getNetCallback());
//			CRNetApp::crRoom* room = callback->findRoom(player_scdata->getRoomID());
//			crData *room_data = room->getDataClass();
//			if(!room_data) return;
//			room_data->excHandle(MAKEINT64(WCH_LockData,1));
//			room_data->getParam(WCHDATA_JXJRoomDataFlag, param);
//			if(!*(bool *)param) return;
//
//			ref_ptr<crStreamBuf> stream = new crStreamBuf;
//			room_data->getParam(WCHDATA_JXJCheckPointVec, param);
//			PassCheckVec *checks = (PassCheckVec *)param;
//			stream->createBuf( 4 + 3*checks->size());
//			room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//			stream->_writeShort(*(short *)param);
//			room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//			stream->_writeShort(*(short *)param);
//			for(PassCheckVec::iterator itr = checks->begin(); itr != checks->end(); itr++)
//			{
//				stream->_writeShort(itr->get()->getScore());
//				stream->_writeChar(itr->get()->getType());
//			}
//
//			room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//			stream->_writeShort(*(short *)param);
//
//			room_data->excHandle(MAKEINT64(WCH_LockData,0));
//			CRNetApp::crPlayerDataEventPacket packet;
//			CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJPassSendingRuntime,stream.get());
//			scene_server->getNetManager()->sendPacket(player_scdata->getPlayerConnectServerAddress(), packet);
//		}
//	}else
//	{
//		return;
//	}
//}
//
//
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJPassArmRemoveMethod
//// On  : 
//// Desp: 损失部队减少粮草
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJPassArmRemoveMethod::crJXJPassArmRemoveMethod()
//{}
//JXJ::crJXJPassArmRemoveMethod::crJXJPassArmRemoveMethod(const crJXJPassArmRemoveMethod& handle)
//{}
//void JXJ::crJXJPassArmRemoveMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJPassArmRemoveMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_factor = atoi(str.c_str());
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJPassArmRemoveMethod::operator()(crHandle &handle)
//{
//	// get room info
//	if(!m_this) return;
//	CRNet::crNetConductor *scene_server = crNetContainer::getInstance()->getNetConductor(SceneServer);
//	crNetDataManager *scene_datamgr = scene_server->getNetDataManager();
//	crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(scene_datamgr->getNetCallback());
//	crRoom *room =  callback->findRoom(m_this->getRoomID());
//	crData *room_data = room->getDataClass();
//	void *param;
//
//	// TODO : 关隘占领
//	if(false)
//	{
//	}
//
//	if(dynamic_cast<crRole *>(m_this))
//	{
//		int player_id = m_this->getID();
//		ref_ptr<crSceneServerPlayerData> player_scdata = dynamic_cast<crSceneServerPlayerData *>(scene_datamgr->getPlayerData(player_id));
//		if (player_scdata.valid())
//		{
//			crData *player_data = player_scdata->getPlayerGameData()->getDataClass();
//			player_data->getParam(WCHDATA_JXJShiliID, param);
//			unsigned char side_id = *(unsigned char *) param;
//			room_data->getParam(WCHDATA_JXJBattleID,param);
//			int battle_id = *(int *)param;
//			unsigned char offense = (unsigned char)(LOINT16(LOINT32(battle_id)));
//			room_data->getParam(WCHDATA_JXJPassRedRemain,param);
//			short *red_remain = (short *)param;
//			room_data->getParam(WCHDATA_JXJPassBlueRemain,param);
//			short *blue_remain = (short *)param;
//
//			(*red_remain) -= offense == side_id ? m_factor : 0;
//			(*blue_remain) -= offense == side_id ? 0 : m_factor;
//			(*red_remain) = (*red_remain) <= 0 ? 0 : (*red_remain);
//			(*blue_remain) = (*blue_remain) <= 0 ? 0 : (*blue_remain);
//		}
//	}
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJSetBuildCanvasMethod
// On  : 
// Desp: canvas 显示
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
crJXJSetBuildCanvasMethod::crJXJSetBuildCanvasMethod()
{}
crJXJSetBuildCanvasMethod::crJXJSetBuildCanvasMethod(const crJXJSetBuildCanvasMethod& handle)
	:m_rtt_widget(handle.m_rtt_widget)
{}
void crJXJSetBuildCanvasMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	default:
		break;
	}
}
void crJXJSetBuildCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rtt_widget = str;
		break;
	}
}
void crJXJSetBuildCanvasMethod::operator()(crHandle &handle)
{
	crData *brain_data = crBrain::getInstance()->getDataClass();
	void *param;
	brain_data->getParam(WCHDATA_JXJBrainCurOpened, param);
	CRCore::crCanvasNode* cur_opened = (CRCore::crCanvasNode *)param;
	if(cur_opened && cur_opened!=m_this)
	{
		crFilterRenderManager::getInstance()->closeCanvas(cur_opened);
	}
	brain_data->inputParam(WCHDATA_JXJBrainCurOpened, m_this);
	crImageBoxWidgetNode *rtt_widget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_rtt_widget));
	brain_data->inputParam(WCHDATA_JXJBrainCurRTTName, rtt_widget);
	bool needRefresh = true;
	brain_data->inputParam(WCHDATA_JXJBrainCurRTTRF, &needRefresh);
}
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJUIShowCanvasMethod
//// On  : 
//// Desp: canvas 显示
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//crJXJUIShowCanvasMethod::crJXJUIShowCanvasMethod()
//{}
//crJXJUIShowCanvasMethod::crJXJUIShowCanvasMethod(const crJXJUIShowCanvasMethod& handle)
//	:m_canvas(handle.m_canvas),
//	m_param(handle.m_param),
//	m_rtt_widget(handle.m_rtt_widget)
//{}
//void crJXJUIShowCanvasMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	default:
//		break;
//	}
//}
//void crJXJUIShowCanvasMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	case 1:
//		m_param = atoi(str.c_str());
//		break;
//	case 2:
//		m_rtt_widget = str;
//		break;
//	default:
//		break;
//	}
//}
//void crJXJUIShowCanvasMethod::operator()(crHandle &handle)
//{
//	crData *brain_data = crBrain::getInstance()->getDataClass();
//	//brain_data->excHandle(MAKEINT64(WCH_LockData,1));
//	void *param;
//	brain_data->getParam(WCHDATA_JXJBrainCurOpened, param);
//	ref_ptr<CRCore::crCanvasNode> cur_opened = (CRCore::crCanvasNode *)param;
//	crCanvasNode *wor_canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//	if(!wor_canvas) return;
//
//	if(m_param == 0)
//	{
//		crFilterRenderManager::getInstance()->showCanvas(wor_canvas, 0);
//		brain_data->inputParam(WCHDATA_JXJBrainCurOpened, NULL);
//	}
//	else
//	{
//		if(cur_opened.valid()) crFilterRenderManager::getInstance()->showCanvas(cur_opened.get(), false);
//		if (wor_canvas->getVisiable())
//		{
//			crFilterRenderManager::getInstance()->showCanvas(wor_canvas, false);
//		}
//		else
//		{
//			crFilterRenderManager::getInstance()->showCanvas(wor_canvas, true);
//		}
//		brain_data->inputParam(WCHDATA_JXJBrainCurOpened, wor_canvas);
//		//crData *wor_canvas_data = wor_canvas->getDataClass();
//		//wor_canvas_data->inputParam(WCHDATA_JXJCanvasBuildingItem, m_this);
//		//brain_data->getParam(WCHDATA_JXJBrainCurRTTName, param);
//		//crImageBoxWidgetNode **rtt_widget = (crImageBoxWidgetNode **)param;
//		crImageBoxWidgetNode *rtt_widget = dynamic_cast<crImageBoxWidgetNode *>(wor_canvas->getWidget(m_rtt_widget));
//		brain_data->inputParam(WCHDATA_JXJBrainCurRTTName, rtt_widget);
//		bool needRefresh = true;
//		brain_data->inputParam(WCHDATA_JXJBrainCurRTTRF, &needRefresh);
//	}
//	//brain_data->excHandle(MAKEINT64(WCH_LockData,0));
//}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIManiorRttRuntimeMethod
// On  : 
// Desp: 主界面RTT显示
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
crJXJUIManiorRttRuntimeMethod::crJXJUIManiorRttRuntimeMethod():
	m_updInterval(0.2f),
	m_rotspeed(100.0f),
	m_rot(0.0f),
	m_rtt(NULL)
{
	m_distScale.set(1.5f,1.5f);
}
crJXJUIManiorRttRuntimeMethod::crJXJUIManiorRttRuntimeMethod(const crJXJUIManiorRttRuntimeMethod& handle):
	m_backModel(handle.m_backModel),
	m_texSize(handle.m_texSize),
	m_updInterval(handle.m_updInterval),
	m_distScale(handle.m_distScale),
	m_rotspeed(handle.m_rotspeed),
	m_rot(0.0f),
	m_rtt(NULL)
{}
void crJXJUIManiorRttRuntimeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	}
}
void crJXJUIManiorRttRuntimeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_backModel = str;
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_texSize);
		break;
	case 2:
		m_updInterval = atof(str.c_str());
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_distScale);
		break;
	case 4:
		m_rotspeed = CRCore::DegreesToRadians(atof(str.c_str()));
		break;
	default:
		break;
	}
}
void crJXJUIManiorRttRuntimeMethod::operator()(crHandle &handle)
{
	// get  widgets
	crData *brain_data = crBrain::getInstance()->getDataClass();
	if(!brain_data) return;
	void *param;

	// current opened canvas
	brain_data->getParam(WCHDATA_JXJBrainCurOpened, param);
	crCanvasNode* canvas = (CRCore::crCanvasNode *)param;
	brain_data->getParam(WCHDATA_JXJBrainCurRTTName, param);
	crImageBoxWidgetNode *rtt_widget = (crImageBoxWidgetNode *)param;
	if(!canvas || !canvas->getDataClass() || !rtt_widget) 
	{
		return;
	}
	crNode *select_node = NULL;
	canvas->getDataClass()->getParam(WCHDATA_JXJCanvasBuildingItem, param);
	crInstanceItem *select_item = (crInstanceItem *)param;
	if(!select_item)
		return;
	// refresh mode
	brain_data->getParam(WCHDATA_JXJBrainCurRTTRF, param);
	bool need_refresh = *(bool *)param;
	
	// input item

	// create rtt
	if(!m_rtt)
	{
		m_rtt = new crRenderToTexture;
		m_rtt->setName("JXJManiorRTT");
		m_rtt->setMode(crRenderToTexture::RenderToTextureMode(crRenderToTexture::RenderToTexture|crRenderToTexture::RenderColorMap|crRenderToTexture::RealTimeUpd|crRenderToTexture::ExternMVP));
		m_rtt->setMapSize(m_texSize[0],m_texSize[1]);
		m_rtt->setUpdateInterval(m_updInterval);

		crGroup *adapter = new crGroup;
		adapter->setName("#adapter");
		m_rtt->addRenderNode(adapter);
		if(m_backModel.compare("NULL")!=0) CRIOManager::crLoadManager::getInstance()->requestNode(m_backModel,m_rtt->getRender(),true);
		m_rtt->createTexturedRoot();//(rtt_widget->getImageObject());
		const crBoundingBox &bbox = rtt_widget->getBoundBox();
		m_rtt->setProjectionMatrix(crMatrix::perspective(50.0f,bbox.xLength()/bbox.yLength(),1.0f,1000.0f));
		m_rtt->init(1.0f);
		if(crBrain::getInstance()->getFog()==1)
		{
			crStateSet *ss = m_rtt->getRender()->getOrCreateStateSet();
			crUniform *uniform = ss->getOrCreateUniform("fog_params",CRCore::crUniform::FLOAT_VEC4);
			uniform->setDataVariance(crBase::STATIC);
			uniform->set(crVector4(0,0,0,0));
		}
		crRenderToTextureManager::getInstance()->addRenderToTexture(m_rtt);
	}
	
	// get select item
	if(select_item->getRelNode()) select_node = dynamic_cast<crGroup *>(select_item->getRelNode())->getChild(0);

	// add none
	crGroup *adapter = dynamic_cast<crGroup *>(m_rtt->findRenderNode("#adapter"));
	if(!select_node || !select_node->getBound().valid()) return;
	if(adapter->getNumChildren() == 0)
	{
		CRIOManager::crLoadManager::getInstance()->requestAddNode(adapter,select_node,false);
	}

	if(select_node != adapter->getChild(0))
	{
		//m_rtt->setEnable(false);
		//CRIOManager::crLoadManager::getInstance()->requestRemoveNode(m_rtt->getTextured(),m_rtt->getTextured()->getChild(0));
		//CRIOManager::crLoadManager::getInstance()->requestAddNode(m_rtt->getTextured(),rtt_widget->getImageObject(),false);
		//m_rtt->removeTexturedNode(0);
		//m_rtt->addTexturedNode(rtt_widget->getImageObject());
		rtt_widget->getImageObject()->setStateSet(m_rtt->getTexturedRoot()->getStateSet());
		const crBoundingBox &bbox = rtt_widget->getBoundBox();
		m_rtt->setProjectionMatrix(crMatrix::perspective(50.0f,bbox.xLength()/bbox.yLength(),1.0f,1000.0f));
		//m_rtt->init(1.0f);
		//m_rtt->setEnable(true);

		CRIOManager::crLoadManager::getInstance()->requestRemoveNode(adapter,adapter->getChild(0));
		CRIOManager::crLoadManager::getInstance()->requestAddNode(adapter,select_node,false);
		crBoundingSphere &bs = select_node->getBound();
		crVector2 dist = m_distScale * bs.radius();
		//crMatrix viewMatrix = crMatrix::lookAt(bs.center()  + crVector3(-dist, m_rotate, m_height), bs.center(), Z_AXIS);
		crMatrix viewMatrix = crMatrix::lookAt(bs.center()  + crVector3(0.0,-dist[0],dist[1]), bs.center(), Z_AXIS);
		m_rtt->setViewMatrix(viewMatrix);
		m_rtt->setEnable(true);
		m_boundSphere = bs;
	}
	else
	{
		//crBoundingSphere &bs = select_node->getBound();
		crVector2 dist = m_distScale * m_boundSphere.radius();
		m_rot += m_rotspeed * crFrameStamp::getInstance()->getFrameInterval();
		if(m_rot>CRCore::PI * 2.0f)
			m_rot -= CRCore::PI * 2.0f;
		crVector3 rotpos = crVector3(0.0,-dist[0],dist[1]) * crMatrix::rotate(m_rot,Z_AXIS);
		crMatrix viewMatrix = crMatrix::lookAt(m_boundSphere.center() + rotpos, m_boundSphere.center(), Z_AXIS);
		m_rtt->setViewMatrix(viewMatrix);
	}
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Clas: crJXJUIBattleShowChatMethod
// On  : 
// Desp: 战斗界面 聊天框显示
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
JXJ::crJXJUIBattleShowChatMethod::crJXJUIBattleShowChatMethod()
{}
JXJ::crJXJUIBattleShowChatMethod::crJXJUIBattleShowChatMethod(const crJXJUIBattleShowChatMethod& handle)
	:m_chat(handle.m_chat),
	m_minmap(handle.m_minmap)
{}
void JXJ::crJXJUIBattleShowChatMethod::inputParam(int i, void *param)
{
}
void JXJ::crJXJUIBattleShowChatMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_chat = str;
		break;
	case 1:
		m_minmap = str;
		break;
	case 2:
		m_y_offset = atof(str.c_str());
		break;
	case 3:
		m_face = str;
		break;
	default:
		break;
	}
}
void JXJ::crJXJUIBattleShowChatMethod::operator()(crHandle &handle)
{
	crCanvasNode *minmap_cannvas = crFilterRenderManager::getInstance()->findCanvas(m_minmap);
	crCanvasNode *chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_chat);
	if(!minmap_cannvas || !chat_canvas)return;

	CRCore::crBoundingBox bbox = minmap_cannvas->getBoundBox();
	crFilterRenderManager::getInstance()->showCanvas(chat_canvas, 1);
	chat_canvas->setMatrix(chat_canvas->getMatrix() * crMatrix::translate(0.0f, bbox.yLength() + m_y_offset, 0.0f));

	// hide the face list
	crWidgetNode *face_list = chat_canvas->getWidget(m_face);
	if(!face_list) return;
	face_list->setVisiable(false);
	crFilterRenderManager::getInstance()->showCanvas(chat_canvas,false);
}

//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//// Clas: crJXJUIHeadItemInputMethod
//// On  : 
//// Desp: 输入item 数据 RTT 显示
//// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//JXJ::crJXJUIHeadItemInputMethod::crJXJUIHeadItemInputMethod()
//{}
//JXJ::crJXJUIHeadItemInputMethod::crJXJUIHeadItemInputMethod(const crJXJUIHeadItemInputMethod& handle)
//	:m_target_canvas(handle.m_target_canvas)
//{}
//void JXJ::crJXJUIHeadItemInputMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crWidgetNode*)param;
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJUIHeadItemInputMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_target_canvas = str;
//		break;
//	default:
//		break;
//	}
//}
//void JXJ::crJXJUIHeadItemInputMethod::operator()(crHandle &handle)
//{
//	crCanvasNode *target_canvas = crFilterRenderManager::getInstance()->findCanvas(m_target_canvas);
//	void *param;
//	m_this->getDataClass()->getParam(WCHDATA_JXJCanvasBuildingItem, param);
//	crInstanceItem *temp = (crInstanceItem *)param;
//	if(target_canvas) target_canvas->getDataClass()->inputParam(WCHDATA_JXJCanvasBuildingItem, param);
//}

/////////////////////////////////////////
//
//crJXJCountryKingUpdateMethod
//
/////////////////////////////////////////
crJXJCountryKingUpdateMethod::crJXJCountryKingUpdateMethod():
	m_this(NULL){}
crJXJCountryKingUpdateMethod::crJXJCountryKingUpdateMethod(const crJXJCountryKingUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCountryKingUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	}
}

void crJXJCountryKingUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJCountryKingUpdateMethod::operator()(crHandle &handle)
{
	//void *param;
	//crData *serverData = m_this->getDataClass();
	//serverData->excHandle(MAKEINT64(WCH_LockData,1));
	//serverData->getParam(WCHDATA_JXJGovpostRankingMap,param);
	//GovpostRankingMap * rankingMap = (GovpostRankingMap *)param;
	//serverData->getParam(WCHDATA_JXJShiliWei,param);
	//crJXJShili *shiliWei = (crJXJShili *)param;
	//serverData->getParam(WCHDATA_JXJShiliShu,param);
	//crJXJShili *shiliShu = (crJXJShili *)param;
	//serverData->getParam(WCHDATA_JXJShiliWu,param);
	//crJXJShili *shiliWu = (crJXJShili *)param;
	//if(rankingMap && shiliWei && shiliShu && shiliWu)
	//{
	//	std::map<_crInt64,CRCore::ref_ptr<crPlayerRanking>,std::greater<_crInt64>>TempMap[3];//功勋,govpos
	//	std::map<_crInt64,CRCore::ref_ptr<crPlayerRanking>,std::greater<_crInt64>>::iterator titr;
	//	bool shilifound[3] = {false,false,false};
	//	for( GovpostRankingMap::iterator itr = rankingMap->begin();
	//			itr != rankingMap->end();
	//			++itr )
	//	{
	//		if(shilifound[itr->second->getShiliID()-c_startShiliID])
	//			continue;
	//		if(TempMap[itr->second->getShiliID()-c_startShiliID].empty())
	//			TempMap[itr->second->getShiliID()-c_startShiliID].insert(std::make_pair(MAKEINT64(itr->second->getExploit(),itr->second->getGovpost()),itr->second));
	//		else if(TempMap[itr->second->getShiliID()-c_startShiliID].begin()->second->getGovpost()==itr->second->getGovpost())
	//		{
	//			TempMap[itr->second->getShiliID()-c_startShiliID].insert(std::make_pair(MAKEINT64(itr->second->getExploit(),itr->second->getGovpost()),itr->second));
	//		}
	//		else
	//		{
	//			shilifound[itr->second->getShiliID()-c_startShiliID] = true;
	//			if(shilifound[0] && shilifound[1] && shilifound[2])
	//				break;
	//		}
	//	}
	//	if(!TempMap[0].empty())
	//	{
	//		titr = TempMap[0].begin();
	//		if(titr->second.valid())
	//		{
	//			crData *data = shiliWei->getDataClass();
	//			if(data)
	//			{
	//				int iPlayerID = titr->second->getPlayerID();
	//				std::string strPlayName = titr->second->getName();
	//				data->inputParam(WCHDATA_JXJChengxiang,&iPlayerID);
	//				data->inputParam(WCHDATA_JXJChengxiangName,&strPlayName);
	//				bool modify = true;
	//				data->inputParam(WCHDATA_JXJShiliModify,&modify);
	//			}
	//		}
	//	}
	//	if(!TempMap[1].empty())
	//	{
	//		titr = TempMap[1].begin();
	//		if(titr->second.valid())
	//		{
	//			crData *data = shiliShu->getDataClass();
	//			if(data)
	//			{
	//				int iPlayerID = titr->second->getPlayerID();
	//				std::string strPlayName = titr->second->getName();
	//				data->inputParam(WCHDATA_JXJChengxiang,&iPlayerID);
	//				data->inputParam(WCHDATA_JXJChengxiangName,&strPlayName);
	//				bool modify = true;
	//				data->inputParam(WCHDATA_JXJShiliModify,&modify);
	//			}
	//		}
	//	}
	//	if(!TempMap[2].empty())
	//	{
	//		titr = TempMap[2].begin();
	//		if(titr->second.valid())
	//		{
	//			crData *data = shiliWu->getDataClass();
	//			if(data)
	//			{
	//				int iPlayerID = titr->second->getPlayerID();
	//				std::string strPlayName = titr->second->getName();
	//				data->inputParam(WCHDATA_JXJChengxiang,&iPlayerID);
	//				data->inputParam(WCHDATA_JXJChengxiangName,&strPlayName);
	//				bool modify = true;
	//				data->inputParam(WCHDATA_JXJShiliModify,&modify);
	//			}
	//		}
	//	}
	//}
	//serverData->excHandle(MAKEINT64(WCH_LockData,0));
}

JXJ::crJXJQueryMailShowMethod::crJXJQueryMailShowMethod()
{}
JXJ::crJXJQueryMailShowMethod::crJXJQueryMailShowMethod(const crJXJQueryMailShowMethod& handle):
	m_input_page(handle.m_input_page),
	m_allselbtn(handle.m_allselbtn),
	m_deletebtn(handle.m_deletebtn),
	m_mailname(handle.m_mailname),
	m_mailtitle(handle.m_mailtitle),
	m_mailtext(handle.m_mailtext)
{
	for (int i = 0;i<5;i++)
	{
		m_inputname[i] = handle.m_inputname[i];
		m_inputdata[i] = handle.m_inputdata[i];
		m_inputtitle[i] = handle.m_inputtitle[i];
		m_mailchoice[i] = handle.m_mailchoice[i];
		m_mailflag[i] = handle.m_mailflag[i];
	}
}
void JXJ::crJXJQueryMailShowMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}
void JXJ::crJXJQueryMailShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailchoice[0] = str;
		break;
	case 1:
		m_mailchoice[1] = str;
		break;
	case 2:
		m_mailchoice[2] = str;
		break;
	case 3:
		m_mailchoice[3] = str;
		break;
	case 4:
		m_mailchoice[4] = str;
		break;
	case 5:
		m_inputname[0] = str;
		break;
	case 6:
		m_inputname[1] = str;
		break;
	case 7:
		m_inputname[2] = str;
		break;
	case 8:
		m_inputname[3] = str;
		break;
	case 9:
		m_inputname[4] = str;
		break;
	case 10:
		m_inputdata[0] = str;
		break;
	case 11:
		m_inputdata[1] = str;
		break;
	case 12:
		m_inputdata[2] = str;
		break;
	case 13:
		m_inputdata[3] = str;
		break;
	case 14:
		m_inputdata[4] = str;
		break;
	case 15:
		m_inputtitle[0] = str;
		break;
	case 16:
		m_inputtitle[1] = str;
		break;
	case 17:
		m_inputtitle[2] = str;
		break;
	case 18:
		m_inputtitle[3] = str;
		break;
	case 19:
		m_inputtitle[4] = str;
		break;
	case 20:
		m_mailflag[0] = str;
		break;
	case 21:
		m_mailflag[1] = str;
		break;
	case 22:
		m_mailflag[2] = str;
		break;
	case 23:
		m_mailflag[3] = str;
		break;
	case 24:
		m_mailflag[4] = str;
		break;
	case 25:
		m_input_page = str;
		break;
	case 26:
		m_allselbtn = str;
		break;
	case 27:
		m_deletebtn = str;
		break;
	case 28:
		m_mailname = str;
		break;
	case 29:
		m_mailtitle = str;
		break;
	case 30:
		m_mailtext = str;
		break;
	}
}
void JXJ::crJXJQueryMailShowMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		void *param;
		crStaticTextWidgetNode *inputname[5] = {NULL};
		crStaticTextWidgetNode *inputdata[5] = {NULL};
		crStaticTextWidgetNode *inputtitle[5] = {NULL};
		for (int i = 0; i < 5; i++)
		{
			inputname[i] = dynamic_cast<crStaticTextWidgetNode *> (m_this->getWidget(m_inputname[i]));
			inputdata[i] = dynamic_cast<crStaticTextWidgetNode *> (m_this->getWidget(m_inputdata[i]));
			inputtitle[i] = dynamic_cast<crStaticTextWidgetNode *> (m_this->getWidget(m_inputtitle[i]));
			if (inputname[i])inputname[i]->clearString();
			if (inputdata[i])inputdata[i]->clearString();
			if (inputtitle[i])inputtitle[i]->clearString();
		}
		crButtonWidgetNode *allselbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_allselbtn));
		crButtonWidgetNode *deletebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_deletebtn));
		crStaticTextWidgetNode *mailname = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mailname));
		crStaticTextWidgetNode *mailtitle = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mailtitle));
		crHypertextWidgetNode *mailtext = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_mailtext));
		if (mailname)mailname->clearString();
		if (mailtitle)mailtitle->clearString();
		if (mailtext)mailtext->clearText();

		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData)
		{
			crData *playerData = playerGameData->getDataClass();
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crStaticTextWidgetNode *inputpage = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input_page));
			int perpage = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMailPerPage,viplv).c_str()));
			//
			crData *canvasdata = m_this->getDataClass();
			canvasdata->getParam(WCHDATA_JXJMailTotalPage,param);
			int totalPage = *(int *)param;
			canvasdata->getParam(WCHDATA_JXJMailCurPage,param);
			int curpage = *(int *)param;
			if (inputpage)
			{
				inputpage->setString(crArgumentParser::appItoa(curpage)+"/"+crArgumentParser::appItoa(totalPage));
			}
		}
		
		
		crData *brainData = crBrain::getInstance()->getDataClass();
		brainData->excHandle(MAKEINT64(WCH_LockData,1));
		brainData->getParam(WCHDATA_JXJRecvMailVec,param);
		JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
		int index = 0;
		if(mailvec)
		{
			int size = mailvec->size();
			for(JXJRecvMailVec::iterator itr_mail = mailvec->begin()
				; itr_mail!=mailvec->end(); itr_mail++,index++)
			{
				if (inputname[index])
				{
					inputname[index]->setString(itr_mail->get()->getSenderName());
					inputname[index]->setVisiable(true);
				}
				if (inputdata[index])
				{
					inputdata[index]->setString(itr_mail->get()->getCreateData());
					inputdata[index]->setVisiable(true);
				}
				if (inputtitle[index])
				{
					std::string title = itr_mail->get()->getTitle();
					if(itr_mail->get()->getState()==Mail_BeenGetted)
					{
						inputtitle[index]->setColor(FCOLOR_GRAY);
						title += std::string("(已领取)");
					}
					else
					{
						if (itr_mail->get()->getState() == Mail_HasEx)
						{
							title += std::string("(附件)");
							inputtitle[index]->setColor(FCOLOR_YELLOW);
						}
						else
						{
							inputtitle[index]->setColor(FCOLOR_GRAY);
						}
						
					}
					inputtitle[index]->setString(title);
					inputtitle[index]->setVisiable(true);
				}
				crRadioWidgetNode *mailchoice = dynamic_cast<crRadioWidgetNode *> 
					(m_this->getWidget(m_mailchoice[index]));
				if (mailchoice)
				{
					mailchoice->setVisiable(true);
					if(index == 0)
					{
						mailchoice->select(true);
						mailchoice->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::PUSH));
					}
				}
				crCheckBoxWidgetNode *mailflag = dynamic_cast<crCheckBoxWidgetNode *> 
					(m_this->getWidget(m_mailflag[index]));
				if (mailflag)
				{
					mailflag->setVisiable(true);
					if (itr_mail->get()->getState() == Mail_Recvd)
					{
						mailflag->select(true);
					}
					else
					{
						mailflag->select(false);
					}
				}
			}

			if (allselbtn && deletebtn)
			{
				if (size > 0)
				{
					allselbtn -> setEnable(true);
					deletebtn -> setEnable(true);
				}
				else
				{
					allselbtn -> setEnable(false);
					deletebtn -> setEnable(false);
				}
			}
		}
		brainData->excHandle(MAKEINT64(WCH_LockData,0));

// 		index = 0;
// 		for(; index < 5; index ++)
// 		{
// 			crStaticTextWidgetNode *input_sender = dynamic_cast<crStaticTextWidgetNode *> 
// 				(m_this->getWidget(m_inputname[index]));
// 			if (input_sender)
// 			{
// 				input_sender->clearString();
// 				input_sender->setVisiable(true);
// 			}
// 			crStaticTextWidgetNode *input_time = dynamic_cast<crStaticTextWidgetNode *> 
// 				(m_this->getWidget(m_inputdata[index]));
// 			if (input_time)
// 			{
// 				input_time->clearString();
// 				input_time->setVisiable(true);
// 			}
// 			crStaticTextWidgetNode *input_title = dynamic_cast<crStaticTextWidgetNode *> 
// 				(m_this->getWidget(m_inputtitle[index]));
// 			if (input_title)
// 			{
// 				input_title->clearString();
// 				input_title->setVisiable(true);
// 			}
// 			crRadioWidgetNode *mailchoice = dynamic_cast<crRadioWidgetNode *> 
// 				(m_this->getWidget(m_mailchoice[index]));
// 			if (mailchoice)
// 			{
// 				mailchoice->setVisiable(false);
// 			}
// 			crCheckBoxWidgetNode *mailflag = dynamic_cast<crCheckBoxWidgetNode *> 
// 				(m_this->getWidget(m_mailflag[index]));
// 			if (mailflag)
// 			{
// 				mailflag->setVisiable(false);
// 			}
// 		}
	}
}