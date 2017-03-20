/**********************************************************************
*
*	文件:	   appMethod10.cpp
*
*	描述:	   
*
*	作者:	   于毅欣
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod11.h>
#include <JXJ/appMethod2.h>
#include <JXJ/appMethod.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <CRUI/crWidgetExtend.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appDBUpdate.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRIOManager/crConvertUTF.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crReadFile.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <sys/stat.h>
#include <JXJ/appData6.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace rbody;
using namespace JXJ;
using namespace CRIOManager;
using namespace CRUI;


///////////////////////////////////////////
////crJXJUIImageDecorateMethod
///////////////////////////////////////////
//crJXJUIImageDecorateUpdateMethod::crJXJUIImageDecorateUpdateMethod(const crJXJUIImageDecorateUpdateMethod& handle):
//	crMethod(handle)
//	, m_this(NULL)
//{	
//}
//void crJXJUIImageDecorateUpdateMethod::inputParam(int i, void *param)
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
//		m_this = (crCanvasNode*)param;
//		break;
//	}
//}
//
//void crJXJUIImageDecorateUpdateMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_Dialog = str;
//		break;
//	case 1:
//		m_Radio1 = str;
//		break;
//	case 2:
//		m_Radio2 = str;
//		break;
//	}
//}
//void crJXJUIImageDecorateUpdateMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData * playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if (NULL == playerGameData)
//		return ;
//	crData *playerData = playerGameData->getDataClass();
//	if (NULL == playerData)
//		return ;
//
//	//找到面板
//	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_Dialog);
//	if (!canvas.valid())
//		return ;
//
//	void *param;
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	crData *data = myPlayer->getDataClass();
//	data->getParam(WCHDATA_JXJVipLv,param);
//	unsigned char viplv = *(unsigned char *)param;
//	int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,viplv).c_str());
//
//	//付费头像和背景是一个面板
//	ref_ptr<crTableIO>playerIconTab = NULL;
//	int nCurSelectSex = 0;	//这里要读取下，0或1
//	int nTypeSelect = 0;	//这里要读取下，0,1,2
//	crRadioGroupWidgetNode * pRadio1 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_Radio1));
//	if (pRadio1)
//		nCurSelectSex = pRadio1->getSelect();
//	crRadioGroupWidgetNode * pRadio2 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_Radio2));
//	if (pRadio2)
//		nTypeSelect = pRadio2->getSelect();
//	crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("UI_Visualize_Label"));
//	if (pagesw)
//		pagesw->setActiveSwitchSet(0);
//	if (0 == nTypeSelect)
//	{
//		playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//	}
//	else
//	{
//		if (pagesw)
//			pagesw->setActiveSwitchSet(1);
//		if (1 == nTypeSelect)
//			playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//		else
//		{
//			playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconBackTable);
//		}
//	}
//
//	if (!playerIconTab.valid())
//		return ;
//
//	int rowCount = playerIconTab->getRowCount();
//	int indexCount = 0;
//	int sexIndex = playerIconTab->getTitleIndex("性别");
//	int iconIndex = playerIconTab->getTitleIndex("icon");
//	int priceIndex = playerIconTab->getTitleIndex("价格");
//	std::string strValue;
//	char szBuf[128] = {0};
//	for (int i = 0; i < rowCount && indexCount < 10; i++)
//	{
//		//判定是否符合当前性别，如果符合则设置-有性别项
//		strValue = (*playerIconTab)(i, sexIndex);
//		if (nCurSelectSex == atoi(strValue.c_str()))
//		{
//			strValue = (*playerIconTab)(i, iconIndex);
//			//设置图片
//			if (0 == nTypeSelect)
//				sprintf(szBuf, "UI_Visualize_SamsaraPortraits_%d", indexCount +1);
//			else
//				sprintf(szBuf, "UI_Visualize_ChargePortraits_%d", indexCount +1);
//			crImageBoxWidgetNode *iconImageBox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(szBuf));
//			if (iconImageBox)
//			{
//				iconImageBox->setVisiable(true);
//				iconImageBox->setImageName(strValue);
//			}
//
//			indexCount ++;
//		}
//	}
//
//	//付费和背景面板的滚动条先隐藏
//	crWidgetNode *scroll = dynamic_cast<crWidgetNode *>(canvas->getWidget("UI_Visualize_Charge_ListScroll"));
//	if (scroll)
//		scroll->setVisiable(false);
//
//	//购买的头像设置成使用，没买的设置成购买
//	playerData->getParam(WCHDATA_PlayerSex,param);
//	bool bSex = *((bool *)param);	//0是女，1是男
//	if (0 == nTypeSelect)
//	{
//		playerData->getParam(WCHDATA_JXJVipLv,param);
//		unsigned char vipLv = *((unsigned char*)param);	//军师等级
//
//		//玩家当前选择的头像灰掉
//		playerData->getParam(WCHDATA_PlayerIconID,param);
//		unsigned char playerCurIconId = *((unsigned char *)param);
//
//		//根据轮回的变量来判断显示哪些购买或使用按钮-策划后8个弄的一样的
//		//两个默认可以使用的头像的按钮显示
//		crButtonWidgetNode *iconButton = NULL;
//		sprintf(szBuf, "UI_Visualize_Samsara_Use_1");
//		iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//		if (iconButton)
//		{
//			iconButton->setVisiable(true);
//			if (1 == playerCurIconId)	//当前的脸如果设置了之后，要设置成灰色的
//				iconButton->setEnable(false);
//		}
//
//		sprintf(szBuf, "UI_Visualize_Samsara_Use_2");
//		iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//		if (iconButton)
//		{
//			iconButton->setVisiable(true);
//			if (2 == playerCurIconId)	//当前的脸如果设置了之后，要设置成灰色的
//				iconButton->setEnable(false);
//		}
//
//		for (int i=2; i<10; i++)
//		{
//			sprintf(szBuf, "sw67_%d", i -1);
//			crMultiSwitch *iconsw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(szBuf));
//			if (vipLv >= i + 1)	//是否这项当前不可用
//			{
//				if (iconsw)
//					iconsw->setActiveSwitchSet(1);
//
//				//显示使用按钮
//				sprintf(szBuf, "UI_Visualize_Samsara_Use_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//				{
//					iconButton->setVisiable(true);
//					if (i+1 == playerCurIconId)	//当前的脸如果设置了之后，要设置成灰色的
//						iconButton->setEnable(false);
//				}
//			}
//			else
//			{
//				if (iconsw)
//					iconsw->setActiveSwitchSet(0);
//
//				sprintf(szBuf, "UI_Visualize_SamsaraPortraits_%d", i +1);
//				crImageBoxWidgetNode *iconImageBox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//				{
//					iconImageBox->setVisiable(false);
//				}
//
//				//关闭按钮
//				sprintf(szBuf, "UI_Visualize_Samsara_Use_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//					iconButton->setVisiable(false);
//				sprintf(szBuf, "UI_Visualize_Samsara_Buy_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//					iconButton->setVisiable(false);
//			}
//		}
//
//		//判断性别，如果不相同就设置按钮不显示
//		if (nCurSelectSex == (int)bSex)	//值不同时才是相同性别
//		{
//			for (int i=0; i<10; i++)
//			{
//				sprintf(szBuf, "UI_Visualize_Samsara_Use_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//					iconButton->setVisiable(false);
//				sprintf(szBuf, "UI_Visualize_Samsara_Buy_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//					iconButton->setVisiable(false);
//			}
//		}
//	}
//	else if (1 == nTypeSelect)
//	{
//		//取到数据
//		crData *data = m_this->getDataClass();
//		void *param = NULL;
//		JXJ::IconShopMap * iconShopMap = NULL;
//		data->excHandle(MAKEINT64(WCH_LockData,1));
//		data->getParam(WCHDATA_JXJPlayerIconShopMap,param);
//		iconShopMap = (JXJ::IconShopMap *)param;
//
//		//读取头像表，找到付费的头像显示出来
//		int pay4IconIndex = playerIconTab->getTitleIndex("是否付费道具"), id = playerIconTab->getTitleIndex("id");
//		std::string strIspay4Icon, strIconID, strPrice;
//		indexCount = 0;
//		unsigned char iconList[128];
//		for (int i = 0; i < rowCount && indexCount < nMaxBuyIconCount; i++)
//		{
//			//判定是否符合当前性别，如果符合则设置-有性别项
//			strValue = (*playerIconTab)(i, sexIndex);
//			strIspay4Icon = (*playerIconTab)(i, pay4IconIndex);
//			strIconID = (*playerIconTab)(i, id);
//			if (nCurSelectSex == atoi(strValue.c_str()) && atoi(strIspay4Icon.c_str()))
//			{
//				strValue = (*playerIconTab)(i, iconIndex);
//				//设置图片
//				sprintf(szBuf, "UI_Visualize_ChargePortraits_%d", indexCount +1);
//				crImageBoxWidgetNode *iconImageBox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//				{
//					iconImageBox->setVisiable(true);
//					iconImageBox->setImageName(strValue);
//				}
//
//				sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", indexCount +1);
//				ref_ptr<crWidgetNode> pBuy = dynamic_cast<crWidgetNode *>(canvas->getWidget(szBuf));
//				(*iconShopMap)[pBuy] = atoi(strIconID.c_str());
//				sprintf(szBuf, "UI_Visualize_Charge_Use_%d", indexCount +1);
//				ref_ptr<crWidgetNode> pUse = dynamic_cast<crWidgetNode *>(canvas->getWidget(szBuf));
//				(*iconShopMap)[pUse] = atoi(strIconID.c_str());
//
//				iconList[indexCount] = atoi(strIconID.c_str());
//
//				//设置价钱
//				strPrice = (*playerIconTab)(i, priceIndex);
//				sprintf(szBuf, "UI_Visualize_Charge_Input_%d", indexCount +1);
//				ref_ptr<crStaticTextWidgetNode> pPrice = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(szBuf));
//				if (pPrice.valid())
//				{
//					sprintf(szBuf, "价格：%s", strPrice.c_str());
//					pPrice->setString(std::string(szBuf));
//				}
//
//				indexCount ++;
//			}
//		}
//		data->excHandle(MAKEINT64(WCH_LockData, 0));
//
//		//设置购买的头像为使用，没有购买的头像为购买
//		crButtonWidgetNode *iconImageBox = NULL;
//		playerData->getParam(WCHDATA_JXJMainRoleBuyIcon, param);
//		PlayerIconVec * playerBuyIcon = (PlayerIconVec *)param;
//		for (int i=0; i<nMaxBuyIconCount; i++)
//		{
//			if (nCurSelectSex != (int)bSex)	//值不同时才是相同性别
//			{
//				//查找这个头像是否被购买过了，可以直接使用
//				bool bIconBuy = false;
//				for (int mm=0; mm<(*playerBuyIcon).size(); mm++)
//				{
//					if (iconList[i] == (*playerBuyIcon)[mm])
//					{
//						bIconBuy = true;
//						break;
//					}
//				}
//				if (bIconBuy)
//				{
//					sprintf(szBuf, "UI_Visualize_Charge_Use_%d", i+1);
//					iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//					if (iconImageBox)
//						iconImageBox->setVisiable(true);
//					sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", i+1);
//					iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//					if (iconImageBox)
//						iconImageBox->setVisiable(false);
//				}
//				else
//				{
//					sprintf(szBuf, "UI_Visualize_Charge_Use_%d", i+1);
//					iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//					if (iconImageBox)
//						iconImageBox->setVisiable(false);
//					sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", i+1);
//					iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//					if (iconImageBox)
//						iconImageBox->setVisiable(true);
//				}
//			}
//			else
//			{
//				sprintf(szBuf, "UI_Visualize_Charge_Use_%d", i+1);
//				iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//					iconImageBox->setVisiable(false);
//				sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", i+1);
//				iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//					iconImageBox->setVisiable(false);
//			}
//		}
//	}
//	else if (2 == nTypeSelect)
//	{
//		for (int i=0; i<nMaxBuyIconCount; i++)
//		{
//			//暂时不做的功能先关闭按钮
//			crButtonWidgetNode *iconButton = NULL;
//			sprintf(szBuf, "UI_Visualize_Charge_Use_%d", i+1);
//			iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//			if (iconButton)
//				iconButton->setVisiable(false);
//			sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", i+1);
//			iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//			if (iconButton)
//				iconButton->setVisiable(false);
//
//			//暂时不做的关闭面板
//			sprintf(szBuf, "UI_Visualize_ChargePortraits_%d", i +1);
//			crImageBoxWidgetNode *iconImageBox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(szBuf));
//			if (iconImageBox)
//			{
//				iconImageBox->setVisiable(false);
//			}
//
//			sprintf(szBuf, "UI_Visualize_Charge_Input_%d", i +1);
//			ref_ptr<crStaticTextWidgetNode> pPrice = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(szBuf));
//			if (pPrice.valid())
//			{
//				pPrice->setString(std::string(""));
//			}
//		}
//	}
//}
///////////////////////////////////////////
////
////crJXJSendPlayerBuyIconMethod
////
///////////////////////////////////////////
//crJXJSendPlayerBuyIconMethod::crJXJSendPlayerBuyIconMethod():
//	m_this(NULL),
//	m_buyIconIndex(0)
//{
//}
//crJXJSendPlayerBuyIconMethod::crJXJSendPlayerBuyIconMethod(const crJXJSendPlayerBuyIconMethod& handle):
//	crMethod(handle),
//	m_this(NULL),
//	m_buyIconIndex(0)
//{
//}
//void crJXJSendPlayerBuyIconMethod::inputParam(int i, void *param)
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
//		m_this = (crCanvasNode*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//void crJXJSendPlayerBuyIconMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_buyIconIndex = atoi(str.c_str());
//		break;
//	}
//}
//void crJXJSendPlayerBuyIconMethod::operator()(crHandle &handle)
//{
//	//是否已经购买了最大的
//	void *param;
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	crData *pTmpData = myPlayer->getDataClass();
//	pTmpData->getParam(WCHDATA_JXJVipLv,param);
//	unsigned char viplv = *(unsigned char *)param;
//	int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,viplv).c_str());
//
//	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
//	playerData->excHandle(MAKEINT64(WCH_LockData,1));
//	playerData->getParam(WCHDATA_JXJMainRoleBuyIcon, param);
//	PlayerIconVec * playerBuyIcon = (PlayerIconVec *)param;
//	if ((*playerBuyIcon).size() >= nMaxBuyIconCount)
//	{
//		bool bFound = false;
//		for (int i=0; i<nMaxBuyIconCount; i++)
//		{
//			if (0 == (*playerBuyIcon)[i])
//			{
//				bFound = true;
//				break;
//			}
//		}
//		if ( ! bFound)
//		{
//			playerData->excHandle(MAKEINT64(WCH_LockData,0));
//			return ;
//		}
//	}
//
//	//取到数据
//	crData *data = m_this->getParentCanvas()->getDataClass();
//	JXJ::BtnIconShopMap * btnIconShopMap = NULL;
//	data->getParam(WCHDATA_JXJPlayerBtnIconShopMap,param);
//	btnIconShopMap = (JXJ::BtnIconShopMap *)param;
//	JXJ::IconShopMap * iconShopMap = NULL;
//	data->getParam(WCHDATA_JXJPlayerIconShopMap,param);
//	iconShopMap = (JXJ::IconShopMap *)param;
//
//	//找到IconID
//	//JXJ::BtnIconShopMap::iterator IterBtn = btnIconShopMap->find(m_this);
//	//if (btnIconShopMap->end() == IterBtn)
//	//	return ;
//	JXJ::IconShopMap::iterator IterImage = iconShopMap->find(m_this);
//	if (iconShopMap->end() == IterImage)
//	{
//		playerData->excHandle(MAKEINT64(WCH_LockData,0));
//		return ;
//	}
//	unsigned char iconIndex = IterImage->second;
//
//	ref_ptr<crTableIO> playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//	int priceIndex = playerIconTab->getTitleIndex("价格");
//	crTableIO::StrVec record;
//	if (playerIconTab->queryOneRecord(0, crArgumentParser::appItoa(iconIndex), record)<0)
//	{
//		playerData->excHandle(MAKEINT64(WCH_LockData,0));
//		return ;
//	}
//
//	int price = atoi(record[7].c_str());	//商品价格
//
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//
//	playerData->getParam(WCHDATA_JXJGoldingot,param);	//当前的钱
//	int goldingot = *(int*)param;
//	playerData->getParam(WCHDATA_JXJGiftGold,param);
//	int giftgold = *(int*)param;
//	if(price<=goldingot + giftgold)
//	{
//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
//		stream->createBuf(1);
//		stream->_writeUChar(iconIndex);
//
//		crPlayerDataEventPacket packet;
//		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerBuyIcon,stream.get());
//		netConductor->getNetManager()->sendPacket("all",packet);
//	}
//	else
//	{//提示元宝不够
//		myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2002,NULL));//元宝不足
//	}
//	playerData->excHandle(MAKEINT64(WCH_LockData,0));
//}
//
///////////////////////////////////////////
////
////crJXJRecvPlayerBuyIconMethod
////
///////////////////////////////////////////
//crJXJRecvPlayerBuyIconMethod::crJXJRecvPlayerBuyIconMethod():
//	m_netType(GameClient_Game){}
//crJXJRecvPlayerBuyIconMethod::crJXJRecvPlayerBuyIconMethod(const crJXJRecvPlayerBuyIconMethod& handle)
//	: m_buyIconIndex(0)
//	, m_this(NULL)
//{
//}
//void crJXJRecvPlayerBuyIconMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//			m_netType = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void crJXJRecvPlayerBuyIconMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crJXJRecvPlayerBuyIconMethod::operator()(crHandle &handle)
//{
//
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			void *param;
//			crData *data = m_this->getDataClass();
//			data->getParam(WCHDATA_JXJVipLv,param);
//			unsigned char viplv = *(unsigned char *)param;
//			int sexIconOffset = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFemaleIconOffset,viplv).c_str());	//女性在男性头像增加的偏移
//			int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,viplv).c_str());
//			//读取种类，设置到玩家身上
//			int playerid = m_this->getPlayerID();
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			if(playerData.valid())
//			{
//				unsigned char buyIconIndex = m_stream->_readUChar();
//
//				//从表中读取这个物品的金钱
//				int price = 0;
//				ref_ptr<crTableIO>playerIconTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//				crTableIO::StrVec record;
//				if(playerIconTable->queryOneRecord(0,crArgumentParser::appItoa(buyIconIndex),record)>=0)
//				{
//					if (7 < record.size())
//						price = atoi(record[7].c_str());
//				}
//
//				//修改并储存购买的Icon的值
//				void *param;
//				m_this->getDataClass()->excHandle(MAKEINT64(WCH_LockData,1));
//				m_this->getDataClass()->getParam(WCHDATA_JXJMainRoleBuyIcon, param);
//				PlayerIconVec * playerBuyIcon = (PlayerIconVec *)param;
//				if ((*playerBuyIcon).size() >= nMaxBuyIconCount)
//				{
//					bool bFound = false;
//					for (int i=0; i<nMaxBuyIconCount; i++)
//					{
//						if (0 == (*playerBuyIcon)[i])
//						{
//							(*playerBuyIcon)[i] = buyIconIndex;
//							break;
//						}
//					}
//					if ( ! bFound)
//					{
//						m_this->getDataClass()->excHandle(MAKEINT64(WCH_LockData, 0));
//						return ;
//					}
//				}
//
//				//读取金钱，验证是否拥有那么多的金钱
//				m_this->getDataClass()->getParam(WCHDATA_JXJGoldingot,param);
//				int *goldingot = (int*)param;
//				m_this->getDataClass()->getParam(WCHDATA_JXJGiftGold,param);
//				int *giftgold = (int*)param;
//				if(price> ((*goldingot)+ (*giftgold)))
//				{
//					ref_ptr<crStreamBuf> stream = new crStreamBuf;
//					stream->createBuf(1);
//					stream->_writeUChar(2);	//1是成功，2是失败
//
//					crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvPlayerBuyIcon,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//
//					m_this->getDataClass()->excHandle(MAKEINT64(WCH_LockData, 0));
//					return ;
//				}
//
//				//扣除元宝
//				bool needdeductgold = false;
//				MoneyChangeData moneydata(price,"购买付费形象");
//				m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
//
//				(*playerBuyIcon).push_back(buyIconIndex);	//增加
//
//				//反馈玩家购买成功
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				stream->createBuf(2);
//				stream->_writeUChar(1);	//1是成功，2是失败
//				stream->_writeUChar(buyIconIndex);	//付费头像增加100
//
//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//				crPlayerDataEventPacket packet;
//				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvPlayerBuyIcon,stream.get());
//				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//				m_this->getDataClass()->excHandle(MAKEINT64(WCH_LockData, 0));
//			}
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			void *param;
//			crData *data = m_this->getDataClass();
//			data->getParam(WCHDATA_JXJVipLv,param);
//			unsigned char viplv = *(unsigned char *)param;
//			int sexIconOffset = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFemaleIconOffset,viplv).c_str());	//女性在男性头像增加的偏移
//			int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,viplv).c_str());
//
//			unsigned char success = m_stream->_readUChar();
//
//			if (1 == success)
//			{
//				//修改本地数据
//				crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
//				playerData->getParam(WCHDATA_JXJMainRoleBuyIcon,param);
//				PlayerIconVec * playerBuyIcon = (PlayerIconVec *)param;
//				unsigned char buyIconIndex = m_stream->_readUChar();
//				bool bInsert = false;
//				playerData->excHandle(MAKEINT64(WCH_LockData, 1));
//				for (int i=0; i<playerBuyIcon->size(); i++)
//				{
//					if (0 == (*playerBuyIcon)[i])
//					{
//						bInsert = true;
//						(*playerBuyIcon)[i] = buyIconIndex;
//						break;
//					}
//				}
//				if ( ! bInsert)
//					(*playerBuyIcon).push_back(buyIconIndex);
//				playerData->excHandle(MAKEINT64(WCH_LockData, 0));
//
//				//找到这个Icon，讲这个Icon的按钮来设置成“使用”
//				int nIndexOffset = buyIconIndex - 100;
//				playerData->getParam(WCHDATA_PlayerSex,param);
//				bool bSex = *(bool *)param;
//				if ( ! bSex)
//					nIndexOffset -= sexIconOffset;
//				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Visualize");
//				if (!canvas.valid())
//					return ;
//
//				char szBuf[128] = {0};
//				sprintf(szBuf, "UI_Visualize_Charge_Use_%d", nIndexOffset);
//				crButtonWidgetNode * iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//					iconImageBox->setVisiable(true);
//				sprintf(szBuf, "UI_Visualize_Charge_Buy_%d", nIndexOffset);
//				iconImageBox = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconImageBox)
//					iconImageBox->setVisiable(false);
//			}
//			else if (2 == success)
//			{
//				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
//			}
//		}
//	}
//}
//
///////////////////////////////////////////
////
////crJXJPlayerUseIconMethod
////
///////////////////////////////////////////
//crJXJPlayerUseIconMethod::crJXJPlayerUseIconMethod():
//	m_netType(GameClient_Game), m_useIconIndex(0){}
//crJXJPlayerUseIconMethod::crJXJPlayerUseIconMethod(const crJXJPlayerUseIconMethod& handle):
//	crMethod(handle), m_useIconIndex(0)
//{
//}
//void crJXJPlayerUseIconMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crCanvasNode*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//			m_netType = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void crJXJPlayerUseIconMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_useIconIndex = atoi(str.c_str());
//		break;
//	}
//}
//
//void crJXJPlayerUseIconMethod::operator()(crHandle &handle)
//{
//	//取到数据
//	crData *data = m_this->getParentCanvas()->getDataClass();
//	void *param = NULL;
//	JXJ::BtnIconShopMap * btnIconShopMap = NULL;
//	data->getParam(WCHDATA_JXJPlayerBtnIconShopMap,param);
//	btnIconShopMap = (JXJ::BtnIconShopMap *)param;
//	JXJ::IconShopMap * iconShopMap = NULL;
//	data->getParam(WCHDATA_JXJPlayerIconShopMap,param);
//	iconShopMap = (JXJ::IconShopMap *)param;
//
//	//找到IconID
//	//JXJ::BtnIconShopMap::iterator IterBtn = btnIconShopMap->find(m_this);
//	//if (btnIconShopMap->end() == IterBtn)
//	//	return ;
//	JXJ::IconShopMap::iterator IterImage = iconShopMap->find(m_this);
//	unsigned char iconIndex = 0;
//	if (iconShopMap->end() != IterImage)
//		iconIndex = IterImage->second;
//	else if (m_useIconIndex)
//		iconIndex = m_useIconIndex;
//	else
//		return;
//
//	int sexIconOffset = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFemaleIconOffset,0).c_str());	//女性在男性头像增加的偏移
//
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//	if(m_this && m_stream.valid())
//	{
//		void *param;
//		crData *playerData = myPlayer->getDataClass();
//		playerData->getParam(WCHDATA_PlayerSex,param);
//		bool bSex = *(bool *)param;
//		unsigned char useIconIndex = iconIndex;
//		if ( ! bSex)
//			useIconIndex += sexIconOffset;
//
//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
//		stream->createBuf(1);
//		stream->_writeUChar(useIconIndex);
//
//		crPlayerDataEventPacket packet;
//		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJPlayerUseIcon,stream.get());
//		netConductor->getNetManager()->sendPacket("all",packet);
//
//
//		//设置按钮状态
//		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Visualize");
//		if (!canvas.valid())
//			return ;
//		char szBuf[128] = {0};
//		crButtonWidgetNode *iconButton = NULL;
//		if (useIconIndex < 100)
//		{
//			for (int i=0; i<10; i++)
//			{
//				sprintf(szBuf, "UI_Visualize_Samsara_Use_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//				{
//					if (iconIndex == i+1)	//当前的脸如果设置了之后，要设置成灰色的
//						iconButton->setEnable(false);
//					else
//						iconButton->setEnable(true);
//				}
//			}
//		}
//		else
//		{
//			for (int i=0; i<10; i++)
//			{
//				sprintf(szBuf, "UI_Visualize_Charge_Use_%d", i+1);
//				iconButton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(szBuf));
//				if (iconButton)
//				{
//					if (iconIndex == i+1 + 100)	//当前的脸如果设置了之后，要设置成灰色的
//						iconButton->setEnable(false);
//					else
//						iconButton->setEnable(true);
//				}
//			}
//		}
//	}
//}
//
///////////////////////////////////////////
////
////crJXJProcPlayerUseIconMethod
////
///////////////////////////////////////////
//crJXJProcPlayerUseIconMethod::crJXJProcPlayerUseIconMethod():
//	m_netType(GameClient_Game){}
//crJXJProcPlayerUseIconMethod::crJXJProcPlayerUseIconMethod(const crJXJProcPlayerUseIconMethod& handle):
//	crMethod(handle),
//	m_useIconIndex(0)
//{
//}
//void crJXJProcPlayerUseIconMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//			m_netType = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void crJXJProcPlayerUseIconMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_useIconIndex = atoi(str.c_str());
//		break;
//	case 1:
//		break;
//	}
//}
//
//void crJXJProcPlayerUseIconMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			unsigned char selectIndex = m_stream->_readUChar();
//// 			crData *mainRoleData = m_this->getMainRole()->getDataClass();
//			crData *data = m_this->getDataClass();
//			data->inputParam(WCHDATA_PlayerIconID,&selectIndex);
//
//			int playerid = m_this->getPlayerID();
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//
//			//通知客户端
//			ref_ptr<crStreamBuf> stream = new crStreamBuf;
//			stream->createBuf(1);
//			stream->_writeUChar(selectIndex);
//
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJPlayerUseIcon,stream.get());
//			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//			//处理服务器的反馈
//			crData *data = crBrain::getInstance()->getDataClass();
//			unsigned char selectIndex = m_stream->_readUChar();
//			data->inputParam(WCHDATA_JXJCurPlayerIconID,(void *)&selectIndex);
//
//			crData *playerData = myPlayer->getDataClass();
//			playerData->inputParam(WCHDATA_PlayerIconID,(void *)&selectIndex);
//
//			//设置“修改人物属性面板”中的头像
//			crTableIO::StrVec record;
//			ref_ptr<crTableIO>playerIconTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//			if(playerIconTable.valid() && playerIconTable->queryOneRecord(0,crArgumentParser::appItoa(selectIndex),record)>=0)
//			{
//				//找到面板
//				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Beta2_FeudalLord");
//				if (!canvas.valid())
//					return ;
//
//				//头像
//				int iconindex = playerIconTable->getTitleIndex("icon2");
//				crImageBoxWidgetNode *image = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget("UI_Beta2_FeudalLord_TouXiang"));
//				if(image && iconindex>=0 )image->setImageName(record[iconindex]);
//			}
//
//// 			playerData->getParam(WCHDATA_PlayerSex,param);
//// 			bool bSex = *(bool *)param;
//
//// 			//找到图片名
//// 			ref_ptr<crTableIO> playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
//// 			int sexIndex = bSex;
//// 			std::string strValue = (*playerIconTab)(selectIndex, sexIndex);
//// 
//// 			//设置图片
//// 			crButtonWidgetNode *playerFaceWidget = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget("TouXiang"));
//// 			if (playerFaceWidget)
//// 			{
//// 				crStateSet *state_set = dynamic_cast<crObject *>(( dynamic_cast<crGroup *>(playerFaceWidget->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
//// 				crTexture2D *texture_2d = dynamic_cast<crTexture2D *>(state_set->getTextureAttribute(0,crStateAttribute::TEXTURE));
//// 				if (texture_2d)
//// 				{
//// 					texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(strValue,texture_2d));
//// 					playerFaceWidget->setVisiable(true);
//// 				}
//// 			}
//		}
//	}
//}
///////////////////////////////////////////
////
////crJXJPlayerIconShopInitMethod
////
///////////////////////////////////////////
//crJXJPlayerIconShopInitMethod::crJXJPlayerIconShopInitMethod():
//	m_this(NULL){}
//crJXJPlayerIconShopInitMethod::crJXJPlayerIconShopInitMethod(const crJXJPlayerIconShopInitMethod& handle):
//	crMethod(handle),m_this(NULL)
//{
//}
//void crJXJPlayerIconShopInitMethod::inputParam(int i, void *param)
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
//		m_this = (CRCore::crCanvasNode * )param;
//		break;
//	}
//}
//
//void crJXJPlayerIconShopInitMethod::addParam(int i, const std::string& str)
//{
//	if (i % 2)
//	{
//// 		crWidgetNode * pWidget = m_this->getWidget(str.c_str());
//		m_ImageName.push_back(str);
//	}
//	else
//	{
//		m_ButtonName.push_back(str);
//	}
//}
//
//void crJXJPlayerIconShopInitMethod::operator()(crHandle &handle)
//{
//	crData *data = m_this->getDataClass();
//
//	//取到数据
//	void *param = NULL;
//	JXJ::BtnIconShopMap * btnIconShopMap = NULL;
//	data->getParam(WCHDATA_JXJPlayerBtnIconShopMap,param);
//	btnIconShopMap = (JXJ::BtnIconShopMap *)param;
//	//JXJ::IconShopMap * iconShopMap = NULL;
//	//data->getParam(WCHDATA_JXJPlayerIconShopMap,param);
//	//iconShopMap = (JXJ::IconShopMap *)param;
//
//	for (int i = 0; i < m_ButtonName.size() && i<m_ImageName.size(); i++)
//	{
//		ref_ptr<crWidgetNode> pBtnWidget = m_this->getWidget(m_ButtonName[i].c_str());
//
//		ref_ptr<crWidgetNode> pImageWidget = m_this->getWidget(m_ImageName[i].c_str());
//
//		(*btnIconShopMap)[pBtnWidget] = pImageWidget;
//	}
//}

/////////////////////////////////////////
//
//crJXJRecvPlayerGameDataCreateStreamMethod
//
/////////////////////////////////////////
crJXJRecvPlayerGameDataCreateStreamMethod::crJXJRecvPlayerGameDataCreateStreamMethod():
	m_flag(false)
{}
crJXJRecvPlayerGameDataCreateStreamMethod::crJXJRecvPlayerGameDataCreateStreamMethod(const crJXJRecvPlayerGameDataCreateStreamMethod& handle)
	: m_this(NULL),
	m_flag(handle.m_flag)
{
}
void crJXJRecvPlayerGameDataCreateStreamMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
		break;
	}
}

void crJXJRecvPlayerGameDataCreateStreamMethod::addParam(int i, const std::string& str)
{
}

unsigned int crJXJRecvPlayerGameDataCreateStreamMethod::getCountryPlayerCount(int nOffset)
{
	int rtn = 0;
	void *param = NULL;
	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	gsBrainData->getParam(WCHDATA_JXJShiliWei + nOffset, param);
	crJXJShili *shili = (crJXJShili *)param;
	if (shili)
	{
		crData *shilidata = shili->getDataClass();
		shilidata->excHandle(MAKEINT64(WCH_LockData,1));
		shilidata->getParam(WCHDATA_JXJPeopleMap, param);
		PeopleMap * peopleMap = (PeopleMap *)param;
		if (peopleMap)
		{
			rtn = peopleMap->size();
		}
		shilidata->excHandle(MAKEINT64(WCH_LockData,0));
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData, 0));
	return rtn;
}

void crJXJRecvPlayerGameDataCreateStreamMethod::operator()(crHandle &handle)
{
	CRCore::crData * playerData = m_this->getDataClass();
	if(m_this && playerData && !m_flag)
	{
		m_flag = true;
		unsigned char shiliid = 0;
		unsigned short chengchiid = 0, homeid = 0;
		void * param = NULL;
		playerData->getParam(WCHDATA_JXJHomeID, param);
		homeid = *(unsigned short *)param;
		if (0 == homeid)	//随机势力
		{
			//读取三个国家的人数，然后计算得到概率
			unsigned int WeiPeopleCount = getCountryPlayerCount(0);	//魏国的人数
			unsigned int ShuPeopleCount = getCountryPlayerCount(1);	//蜀国的人数
			unsigned int WuPeopleCount = getCountryPlayerCount(2);	//吴国的人数
			//double WeiValue = pow((100.0 / WeiPeopleCount), 4);
			//double ShuValue = pow((100.0 / ShuPeopleCount), 4);
			//double WuValue = pow((100.0 / WuPeopleCount), 4);
			int WeiRate = 10;
			bool isLoseCountry = false;
			shiliid = c_startShiliID;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
			{
				WeiRate = 0;
			}
			else if (WeiPeopleCount < ShuPeopleCount && WeiPeopleCount < WuPeopleCount)
			{
				WeiRate = 80;
			}

			int ShuRate = 10;
			isLoseCountry = false;
			shiliid = c_startShiliID+1;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
			{
				ShuRate = 0;
			}
			else if (ShuPeopleCount < WeiPeopleCount && ShuPeopleCount < WuPeopleCount)
			{
				ShuRate = 80;
			}
			int WuRate = 10;
			isLoseCountry = false;
			shiliid = c_startShiliID+2;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
			{
				WuRate = 0;
			}
			else if (WuPeopleCount < WeiPeopleCount && WuPeopleCount < ShuPeopleCount)
			{
				WuRate = 80;
			}
			int rat = WeiRate+ShuRate+WuRate;
			int nRandomValue = rangei(0,rat).get_random();
			int shiliOffset = 0;
			if (WeiRate >= nRandomValue)
			{
				shiliOffset = 0;	//魏国
				////判定是否亡国
				//int myshiliid = shiliOffset + c_startShiliID;
				//
				//crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
				//if (isLoseCountry)
				//{
				//	if (WuPeopleCount > ShuPeopleCount)
				//		shiliOffset = 1;
				//	else
				//		shiliOffset = 2;
				//}
			}
			else if (WeiRate + ShuRate >= nRandomValue)
			{
				shiliOffset = 1;	//蜀国
				////判定是否亡国
				//int myshiliid = shiliOffset + c_startShiliID;
				//bool isLoseCountry = false;
				//crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
				//if (isLoseCountry)
				//{
				//	if (WeiPeopleCount > WuPeopleCount)
				//		shiliOffset = 2;
				//	else
				//		shiliOffset = 0;
				//}
			}
			else
			{
				shiliOffset = 2;	//吴国
				////判定是否亡国
				//int myshiliid = shiliOffset + c_startShiliID;
				//bool isLoseCountry = false;
				//crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
				//if (isLoseCountry)
				//{
				//	if (WeiPeopleCount > ShuPeopleCount)
				//		shiliOffset = 0;
				//	else
				//		shiliOffset = 1;
				//}
			}
			ref_ptr<crTableIO>shiliTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			ref_ptr<crTableIO>homeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHomeTab);
			crTableIO::StrVec record;
			if (shiliTab->queryOneRecord(0,crArgumentParser::appItoa(shiliOffset + c_startShiliID),record)>=0)
			{
				shiliid = (unsigned char)(atoi(record[0].c_str()));
				chengchiid = (unsigned short)(atoi(record[shiliTab->getTitleIndex("默认城池")].c_str()));

				if (homeTab->queryOneRecord(homeTab->getTitleIndex("chengchiid"),crArgumentParser::appItoa(chengchiid),record)>=0)
				{
					homeid = atoi(record[0].c_str());
				}

				if (homeid != 0)
				{
					playerData->inputParam(WCHDATA_JXJShiliID, &shiliid);
					playerData->inputParam(WCHDATA_JXJHomeID, &homeid);
					playerData->inputParam(WCHDATA_JXJChengchiID, &chengchiid);

					//点券增加
					int playerid = m_this->getPlayerID();
					playerData->excHandle(MAKEINT64(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJGiftGold, param);
					int *giftGold = (int *)param;
					int nOffset = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRandomCountryGiftCount,0).c_str());
					INTLIMITADD(*giftGold, nOffset,INT_MAX);
					playerData->excHandle(MAKEINT64(WCH_LockData,0));

					//将shiliid，chengchiid，homeid告诉给客户端
					crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
					int playerID = m_this->getPlayerID();
					ref_ptr<crGameServerPlayerData> player = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
					if(player.valid())
					{
						crJXJRandomZhenYingTellClientIdPacket suijiCreateRole;
						crJXJRandomZhenYingTellClientIdPacket::buildReplyPacket(suijiCreateRole, playerID, shiliid, chengchiid, homeid, *giftGold);
						gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(),suijiCreateRole);
					}
					///游戏日志
					std::string logdata = "随机势力选择,获得礼金：" + crArgumentParser::appItoa(nOffset) + "势力：" + crArgumentParser::appItoa(shiliid);
					GameLogData gamelog(Log_RandShili,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
	}
}



