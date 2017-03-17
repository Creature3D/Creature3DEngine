/**********************************************************************
*
*	文件:	   appMethod13.cpp
*
*	描述:
*
*	作者:	   宋健
*
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod13.h>
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
#include <CREncapsulation/crGameDices.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crLoadManager.h>
#include <CRCore/crBlockDetectThread.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRDownload/crDownload.h>
#include <sstream>
#include <JXJ/appDBQuery.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace CRIOManager;
using namespace rbody;
using namespace JXJ;
using namespace CRUI;
using namespace CRDownload;
/////////////////////////////////////////
//
//crJXJShouchongCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJShouchongCanvasUpdateMethod::crJXJShouchongCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJShouchongCanvasUpdateMethod::crJXJShouchongCanvasUpdateMethod( const crJXJShouchongCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_chongzhi(handle.m_chongzhi),
	m_lingqu(handle.m_lingqu)
{
	for (unsigned char i = 0; i < 6; ++ i)
	{
		m_icon[i] = handle.m_icon[i];
	}
}

void crJXJShouchongCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJShouchongCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_icon[i] = str;
		break;
	case 6:
		m_chongzhi = str;
		break;
	case 7:
		m_lingqu = str;
		break;
	}

}

void crJXJShouchongCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getDataClass())
	{
		crButtonWidgetNode *chongzhi = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chongzhi));
		crButtonWidgetNode *lingqu = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqu));

		crData * playerData = myPlayer->getDataClass();
		void *param;
		playerData->getParam(WCHDATA_JXJShouchong,param);
		unsigned char shouchong = *(unsigned char *)param;
		if(chongzhi && lingqu)
		{
			if(shouchong == 0)
			{
				chongzhi->setVisiable(true);
				lingqu->setVisiable(false);
			}
			else if(shouchong == 1)
			{
				chongzhi->setVisiable(false);
				lingqu->setVisiable(true);
				lingqu->setEnable(true);
			}
			else
			{
				chongzhi->setVisiable(false);
				lingqu->setVisiable(true);
				lingqu->setEnable(false);
			}
		}
		ref_ptr<crImageBoxWidgetNode> icon[6] = {NULL};
		crVector3i vec3;			
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> shouchongAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShouchongAwardTab);
		crTableIO::StrVec itemRecord;
		int iconindex = itemtab->getTitleIndex("icon");
		for( int i = 0; i<6; i++)
		{
			icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			if(icon[i].valid())
			{
				if(crArgumentParser::appAtoVec((*shouchongAwardTab)(i,1),vec3))
				{
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(vec3[0]),itemRecord) >= 0)
					{
						icon[i]->setImageName(itemRecord[iconindex]);
					}
				}
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJFubenRushEndMethod
//
/////////////////////////////////////////
crJXJFubenRushEnd0Method::crJXJFubenRushEnd0Method()
{
}

crJXJFubenRushEnd0Method::crJXJFubenRushEnd0Method(const crJXJFubenRushEnd0Method& handle):
	crMethod(handle),
	m_leftswitch(handle.m_leftswitch),
	m_rightswitch(handle.m_rightswitch),
	m_rushradio(handle.m_rushradio),
	m_rushcountedit(handle.m_rushcountedit),
	m_rushjunlinginput(handle.m_rushjunlinginput)
{
}

void crJXJFubenRushEnd0Method::inputParam(int i, void *param)
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
	}
}

void crJXJFubenRushEnd0Method::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_leftswitch = str;
		break;
	case 1:
		m_rightswitch = str;
		break;
	case 2:
		m_rushradio = str;
		break;
	case 3:
		m_rushcountedit = str;
		break;
	case 4:
		m_rushjunlinginput = str;
		break;
	case 5:
		m_list = str;
		break;
	default:
		break;
	}
}

void crJXJFubenRushEnd0Method::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		//crData *data = playerGameData->getDataClass();
		//data->inputParam(WCHDATA_JXJFubenOrChengchiType,NULL);
		//data->inputParam(WCHDATA_JXJFubenOrChengchiID,NULL);
		
		if (m_this->getDataClass())
		{
			crData *canvasData = m_this->getDataClass();
			if(canvasData)
				canvasData->inputParam(WCHDATA_JXJFubenRushRewardItemMap,NULL);
			ref_ptr<crTableWidgetNode> settleList= dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list));
			if(settleList.valid())
				settleList->clearData();
			crMultiSwitch *leftsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_leftswitch));
			crMultiSwitch *rightsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_rightswitch));
			if (leftsw)
			{
				leftsw->setActiveSwitchSet(FubenRushType_Ready);
			}
			if(rightsw)
			{
				rightsw->setActiveSwitchSet(FubenRushType_Ready);
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJCheckJoinWarTaskMethod
//
/////////////////////////////////////////
crJXJCheckJoinWarTaskMethod::crJXJCheckJoinWarTaskMethod():
	m_trueExecCount(1)
{
}
crJXJCheckJoinWarTaskMethod::crJXJCheckJoinWarTaskMethod(const crJXJCheckJoinWarTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckJoinWarTaskMethod::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckJoinWarTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckJoinWarTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room && playerGameData)
		{
			void *param;
			crData *data = playerGameData->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				unsigned char type = *(unsigned char *)param;
				if(type == FT_Chengchi)
				{
					check = true;
					if(m_trueExecCount > 0)
						m_trueExecCount--;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}




/////////////////////////////////////////
//
//crJXJCheckJoinWarTask1Method
//
/////////////////////////////////////////
crJXJCheckJoinWarTask1Method::crJXJCheckJoinWarTask1Method():
	m_trueExecCount(1)
{
}
crJXJCheckJoinWarTask1Method::crJXJCheckJoinWarTask1Method(const crJXJCheckJoinWarTask1Method& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckJoinWarTask1Method::inputParam(int i, void *param)
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
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJCheckJoinWarTask1Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckJoinWarTask1Method::operator()(crHandle &handle)
{
	bool check = false;
	if (m_trueExecCount!=0)
	{
	
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(!room)
		{
			check = true;
			if(m_trueExecCount > 0)
				m_trueExecCount--;
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJIsChengZhanMethod
//
/////////////////////////////////////////
crJXJIsChengZhanMethod::crJXJIsChengZhanMethod()
{
}
crJXJIsChengZhanMethod::crJXJIsChengZhanMethod(const crJXJIsChengZhanMethod& handle)
{
}
void crJXJIsChengZhanMethod::inputParam(int i, void *param)
{
}
void crJXJIsChengZhanMethod::addParam(int i, const std::string& str)
{
}
void crJXJIsChengZhanMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
		unsigned char getAward = *(unsigned char *)param;
		if (getAward == (unsigned char)-1)
		{
			check= true;
		}
		else
		{
			check=false;
		}
	}
	handle.outputParam(0,&check);
}


/////////////////////////////////////////
//
//crJXJMonitorPingPacketParseMethod
//
/////////////////////////////////////////
crJXJMonitorPingPacketParseMethod::crJXJMonitorPingPacketParseMethod()
{
}
crJXJMonitorPingPacketParseMethod::crJXJMonitorPingPacketParseMethod(const crJXJMonitorPingPacketParseMethod& handle)
{
}
void crJXJMonitorPingPacketParseMethod::inputParam(int i, void *param)
{
}
void crJXJMonitorPingPacketParseMethod::addParam(int i, const std::string& str)
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

void crJXJMonitorPingPacketParseMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		crData * canvasData = canvas->getDataClass();
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		// 重新发包
		crJXJMonitorPingPacket pkg;
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",pkg);
		}

		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}

}


/////////////////////////////////////////
//
//crJXJCheckTechIsCompleteMethod
//
/////////////////////////////////////////
crJXJCheckTechIsCompleteMethod::crJXJCheckTechIsCompleteMethod()
{
}
crJXJCheckTechIsCompleteMethod::crJXJCheckTechIsCompleteMethod(const crJXJCheckTechIsCompleteMethod& handle)
{
}
void crJXJCheckTechIsCompleteMethod::inputParam(int i, void *param)
{
}
void crJXJCheckTechIsCompleteMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_techtype = atoi(str.c_str());
		break;
	case 1:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJCheckTechIsCompleteMethod::operator()(crHandle &handle)
{
	bool check = false;
	if(m_trueExecCount != 0)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerData && playerData->getDataClass())
		{
			void *param;
			crData *data = playerData->getDataClass();
			if(!data)return;
//			data->getParam(WCHDATA_JXJEquipAttachTechLv,param);
			data->getParam(m_techtype,param);
			unsigned char EquipAttachTechLv = *(unsigned char *)param;
			if(EquipAttachTechLv>0)
			{
				check = true;
				if(m_trueExecCount>0)
					m_trueExecCount--;
			}
		}
		
	}
	handle.outputParam(0,&check);	
}


/////////////////////////////////////////
//
//crJXJIsSelectMyRolesMethod
//
/////////////////////////////////////////
crJXJIsSelectMyRolesMethod::crJXJIsSelectMyRolesMethod():
	m_this(NULL){}
crJXJIsSelectMyRolesMethod::crJXJIsSelectMyRolesMethod(const crJXJIsSelectMyRolesMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJIsSelectMyRolesMethod::inputParam(int i, void *param)
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

void crJXJIsSelectMyRolesMethod::addParam(int i, const std::string& str)
{
}

void crJXJIsSelectMyRolesMethod::operator()(crHandle &handle)
{
	bool selected = true;
	void *param;
	crData *cameraData = m_this->getDataClass();
	if(!cameraData) return;
	CRCore::ScopedLock<crData> lock(*cameraData);

	cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
	SelectMyRoleVec *selemyroles = (SelectMyRoleVec*)param;
	if(selemyroles->empty()) selected = false;

	handle.outputParam(0,&selected);
}


/////////////////////////////////////////
//
//crJXJBuyYuekaMethod
//
/////////////////////////////////////////
crJXJBuyYuekaMethod::crJXJBuyYuekaMethod():
	m_type(0)
{
}

crJXJBuyYuekaMethod::crJXJBuyYuekaMethod( const crJXJBuyYuekaMethod & handle ):
	crMethod(handle),
	m_type(handle.m_type)
{
}

void crJXJBuyYuekaMethod::inputParam( int i, void *param )
{
}

void crJXJBuyYuekaMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJBuyYuekaMethod::operator()( crHandle &handle )
{
	//
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(m_type);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuyYueka,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJRecvBuyYuekaMethod
//
/////////////////////////////////////////
crJXJRecvBuyYuekaMethod::crJXJRecvBuyYuekaMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{

}
crJXJRecvBuyYuekaMethod::crJXJRecvBuyYuekaMethod( const crJXJRecvBuyYuekaMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client),
	m_canvas(handle.m_canvas)
{
}
void crJXJRecvBuyYuekaMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void crJXJRecvBuyYuekaMethod::addParam( int i, const std::string& str )
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

void crJXJRecvBuyYuekaMethod::operator()( crHandle &handle )
{
	if(m_netType == GameServer)
	{
		unsigned char type = m_stream->_readUChar();
		int yuekaid = 1;
		int playerid = m_this->getPlayerID();		
		ref_ptr<crData> data = m_this->getDataClass();
		void *param;
		unsigned char ret = 0;
		data->getParam(WCHDATA_JXJYueKaEndTime,param);
		time_t yuekaendtime = *(_crInt64 *)param;
		data->getParam(WCHDATA_JXJYueKaLastGetTime,param);
		time_t yuekalastgetime = *(_crInt64 *)param;
		bool needdeductgold = false;
		
		if (type != 1)
		{
			data->getParam(WCHDATA_JXJGoldingot,param);
			int *goldingot = (int *)param;	
			time_t curtime = time(0);
			int curday = (int)(curtime/86400);
			int endday = (int)(yuekaendtime/86400);
			ref_ptr<crTableIO> yuekaTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJYueKaTab);
			crTableIO::StrVec record;
			yuekaTab->queryOneRecord(0,crArgumentParser::appItoa(yuekaid),record);
		
			if(endday -curday <= 5)
			{
				int priceindex = yuekaTab->getTitleIndex("售价");
				int price = 0;
				if (priceindex > 0)
				{
					price = atoi(record[priceindex].c_str());
				}
			
				if (price > 0 && *goldingot >= price)
				{
					std::string logdata = "购买月卡： ";
					logdata += crArgumentParser::appItoa(yuekaid);
					if (yuekaTab.valid())
					{				
						int lijinindex = yuekaTab->getTitleIndex("赠送礼金");
						int keepdaysindex = yuekaTab->getTitleIndex("持续天数");
						int lijin = 0;
						int keepdays = 0;
						if (lijinindex > 0 && keepdaysindex > 0)
						{
							lijin = atoi(record[lijinindex].c_str());
							keepdays = atoi(record[keepdaysindex].c_str());
						}
						if (lijin >0 && keepdays >0)
						{
							MoneyChangeData moneydata(-price,"购买月卡");
							m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
							if(needdeductgold == true)
							{
								int tempday = (curday >endday)?curday:endday;
								yuekaendtime = (tempday + keepdays)*86400;
								data->excHandle(MAKEINT64(WCH_LockData,1));
								data->inputParam(WCHDATA_JXJYueKaEndTime,&yuekaendtime);
								data->inputParam(WCHDATA_JXJYueKaID,&yuekaid);
								data->excHandle(MAKEINT64(WCH_LockData,0));
								moneydata.first = lijin;
								moneydata.second = "购买月卡礼金奖励";
								m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
								ret = 1;
							
								GameLogData gamelog(Log_BuyYueka,logdata);
								crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog)); 
							}
						}
					}
				}
				else
				{
					ret = 3;	//元宝不足
				}

			}
			else
			{
				ret = 2;		//剩余天数大于5不能续费
			}
		}
		else
		{
			ret = 4;
		}
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(18);
			stream->_writeUChar(ret);
			stream->_writeInt64(yuekaendtime);
			stream->_writeInt64(yuekalastgetime);
			stream->_writeUChar((unsigned char)yuekaid);	
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvBuyYueka,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
		if (needdeductgold)
		{
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
			callback->savePlayerData(playerData.get());
		}
	}
	else if (m_netType == GameClient_Game)
	{
		unsigned char ret = m_stream->_readUChar();	
		time_t yuekaendtime = m_stream->_readInt64();
		time_t yuekalastgettime = m_stream->_readInt64();
		unsigned char yuekaid = m_stream->_readUChar();
		int msgid = 0;
		if (ret ==1)
		{
			msgid = 4110;
		}
		else if(ret==2)
		{
			msgid = 4107;
		}
		else if (ret == 3)
		{
			msgid =	4108;
		}
		else
		{
			msgid = 4109;
		}
		ref_ptr<crData> data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->inputParam(WCHDATA_JXJYueKaEndTime,&yuekaendtime);
		data->inputParam(WCHDATA_JXJYueKaID,&yuekaid);
		data->inputParam(WCHDATA_JXJYueKaLastGetTime,&yuekalastgettime);
		data->excHandle(MAKEINT64(WCH_LockData,0));
		if (ret != 4)
		{
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvasNode.valid())
			{
				canvasNode->doEvent(WCH_UI_UpdateData);
			}
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		}
		
	}
}


/////////////////////////////////////////
//
//crJXJYueKaCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJYueKaCanvasUpdateMethod::crJXJYueKaCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJYueKaCanvasUpdateMethod::crJXJYueKaCanvasUpdateMethod( const crJXJYueKaCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_Goumai(handle.m_Goumai),
	m_Lingqu(handle.m_Lingqu),
	m_Xufei(handle.m_Xufei),
	m_icon(handle.m_icon),
	m_sw2(handle.m_sw2),
	m_sw4(handle.m_sw4),
	m_sw9(handle.m_sw9),
	m_imagename(handle.m_imagename)
{

}

void crJXJYueKaCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJYueKaCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_Goumai = str;
		break;
	case 1:
		m_Lingqu = str;
		break;
	case 2:
		m_Xufei = str;
		break;
	case 3:
		m_icon = str;
		break;
	case 4:
		m_sw2 = str;
		break;
	case 5:
		m_sw4 = str;
		break;
	case 6:
		m_sw9 = str;
		break;
	case 7:
		m_imagename = str;
		break;
	default:
		break;
	}

}

void crJXJYueKaCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crMultiSwitch>  button_sw2 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw2));
		ref_ptr<crMultiSwitch>  button_sw4 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw4));
		ref_ptr<crMultiSwitch>	button_sw9 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw9));
		ref_ptr<crButtonWidgetNode> goumaiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Goumai));
		ref_ptr<crButtonWidgetNode> lingquBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Lingqu));
		ref_ptr<crButtonWidgetNode> xufeiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Xufei));
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon));

		crData * playerData = myPlayer->getDataClass();
		void *param;
		playerData->getParam(WCHDATA_JXJYueKaEndTime,param);
		time_t yuekaendtime = *(_crInt64 *)param;
		playerData->getParam(WCHDATA_JXJYueKaLastGetTime,param);
		time_t yuekalastgettime = *(_crInt64 *)param;
		time_t curtime = time(0);
		int endday = (int)(yuekaendtime/86400);
		int curday = (int)(curtime/86400);
		int lastgetday = (int)(yuekalastgettime/86400);
		if(button_sw2.valid() && button_sw4.valid()
			&& goumaiBtn.valid() && lingquBtn.valid() 
			&& xufeiBtn.valid() && icon.valid() && button_sw9.valid())
		{
			icon->setEnable(true);
			icon->setImageName(m_imagename);
			if (endday - curday <= 0)
			{
				button_sw9->setActiveSwitchSet(0);
				goumaiBtn->setVisiable(true);
				xufeiBtn->setVisiable(false);
				lingquBtn->setVisiable(false);
				button_sw2->setActiveSwitchSet(0);
				goumaiBtn->setEnable(true);
			}
			else if (endday - curday <=5)
			{
				button_sw9->setActiveSwitchSet(2);
				button_sw2->setActiveSwitchSet(1);
				goumaiBtn->setVisiable(false);
				xufeiBtn->setVisiable(true);
				if (lastgetday < curday)
				{
					lingquBtn->setVisiable(true);
					lingquBtn->setEnable(true);
				}
				else
				{
				//	lingquBtn->setVisiable(false);
					lingquBtn->setEnable(false);
				}		
				button_sw4->setActiveSwitchSet(endday-curday);
				xufeiBtn->setEnable(true);
			}
			else
			{
				button_sw9->setActiveSwitchSet(1);
				button_sw2->setActiveSwitchSet(1);
				goumaiBtn->setVisiable(false);
				xufeiBtn->setVisiable(false);
				if (lastgetday < curday)
				{
					lingquBtn->setVisiable(true);
					lingquBtn->setEnable(true);
				}
				else
				{
				//	lingquBtn->setVisiable(false);
					lingquBtn->setEnable(false);
				}
				button_sw4->setActiveSwitchSet(endday-curday);
			}
		}

	}
}


/////////////////////////////////////////
//
//crJXJYueKaRewardsMethod
//
/////////////////////////////////////////
crJXJYueKaRewardsMethod::crJXJYueKaRewardsMethod()
{
}

crJXJYueKaRewardsMethod::crJXJYueKaRewardsMethod( const crJXJYueKaRewardsMethod & handle ):
	crMethod(handle)
{
}

void crJXJYueKaRewardsMethod::inputParam( int i, void *param )
{
}

void crJXJYueKaRewardsMethod::addParam( int i, const std::string& str )
{
}

void crJXJYueKaRewardsMethod::operator()( crHandle &handle )
{
	//
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvYueKaRewards,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}


/////////////////////////////////////////
//
//crJXJRecvYueKaRewardsMethod
//
/////////////////////////////////////////
crJXJRecvYueKaRewardsMethod::crJXJRecvYueKaRewardsMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{

}
crJXJRecvYueKaRewardsMethod::crJXJRecvYueKaRewardsMethod( const crJXJRecvYueKaRewardsMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{
}
void crJXJRecvYueKaRewardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void crJXJRecvYueKaRewardsMethod::addParam( int i, const std::string& str )
{

}

void crJXJRecvYueKaRewardsMethod::operator()( crHandle &handle )
{
	if(m_netType == GameServer)
	{
		int yuekaid = 1;
		int playerid = m_this->getPlayerID();		
		ref_ptr<crData> data = m_this->getDataClass();
		void *param;
		unsigned char ret = 0;
		data->getParam(WCHDATA_JXJYueKaEndTime,param);
		time_t yuekaendtime = *(_crInt64 *)param;
		data->getParam(WCHDATA_JXJYueKaLastGetTime,param);
		time_t yuekalastgetime = *(_crInt64 *)param;	
		time_t curtime = time(0);
		int curday = (int)(curtime/86400);
		int endday = (int)(yuekaendtime/86400);
		int yuekalastgetday = (int)(yuekalastgetime/86400);

		ref_ptr<crTableIO> yuekaTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJYueKaTab);
		crTableIO::StrVec record;
		yuekaTab->queryOneRecord(0,crArgumentParser::appItoa(yuekaid),record);

		if (endday > curday)
		{
			if (yuekalastgetday < curday)
			{
				int dailyrewardindex = yuekaTab->getTitleIndex("每日领取");
				if (dailyrewardindex > 0)
				{
					yuekalastgetime = curtime;
					std::string logdata = "领取每日礼金： "; 
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->inputParam(WCHDATA_JXJYueKaLastGetTime,&yuekalastgetime);
					data->excHandle(MAKEINT64(WCH_LockData,0));
					int lijin = atoi(record[dailyrewardindex].c_str());
					logdata += crArgumentParser::appItoa(lijin);

					MoneyChangeData moneydata(lijin,"月卡每日赠送礼金");
					m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
					ret = 1;
					GameLogData gamelog(Log_GetYuekalijin,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog)); 
				}
			}
			else
			{
				ret = 3;	// 今天已经领取过了
			}
		}
		else
		{
			ret = 2;	//月卡已过期
		}

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(9);
			stream->_writeUChar(ret);
			stream->_writeInt64(yuekalastgetime);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvYueKaRewards,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if (m_netType == GameClient_Game)
	{
		unsigned char ret = m_stream->_readUChar();	
		time_t yuekalastgettime = m_stream->_readInt64();
		int msgid = 0;
		if (ret ==2)
		{
			msgid = 4111;
		}
		else if(ret==3)
		{
			msgid = 4112;
		}
		ref_ptr<crData> data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->inputParam(WCHDATA_JXJYueKaLastGetTime,&yuekalastgettime);
		data->excHandle(MAKEINT64(WCH_LockData,0));
		if (ret == 2 || ret == 3)
		{
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		}

	}
}


/////////////////////////////////////////
//
//crJXJZiXuanLiBaoCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJZiXuanLiBaoCanvasUpdateMethod::crJXJZiXuanLiBaoCanvasUpdateMethod():
	m_this(NULL),
	m_lasthuodongID(0)
{

}

crJXJZiXuanLiBaoCanvasUpdateMethod::crJXJZiXuanLiBaoCanvasUpdateMethod( const crJXJZiXuanLiBaoCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_chongzhi(handle.m_chongzhi),
	m_lingqu(handle.m_lingqu),
	m_sw13(handle.m_sw13),
	m_nullImage(handle.m_nullImage),
	m_LingQuCiShu(handle.m_LingQuCiShu),
	m_yichongzhi(handle.m_yichongzhi),
	m_remainingtimes(handle.m_remainingtimes),
	m_remainingtime(handle.m_remainingtime),
	m_radioGroup(handle.m_radioGroup),
	m_yuanbaonum(handle.m_yuanbaonum),
	m_lasthuodongID(handle.m_lasthuodongID)
{
	for (unsigned char i = 0; i < 16; ++ i)
	{
		m_icon[i] = handle.m_icon[i];
		m_input[i] = handle.m_input[i];
		m_donghua[i] = handle.m_donghua[i];
		if ( i < 4)
		{
			m_swnum[i] = handle.m_swnum[i];
		}
	}
}

void crJXJZiXuanLiBaoCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJZiXuanLiBaoCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_chongzhi = str;
		break;
	case 1:
		m_lingqu = str;
		break;
	case 2:
		m_sw13 = str;
		break;
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
	case 17:
	case 18:
		m_icon[i-3] = str;
		break;
	case 19:
		m_nullImage = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
		m_input[i-20] = str;
		break;
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		m_donghua[i-36] = str;
		break;
	case 52:
		m_radioGroup = str;
		break;
	case 53:
		m_LingQuCiShu = str;
		break;
	case 54:
		m_yichongzhi = str;
		break;
	case 55:
		m_remainingtimes = str;
		break;
	case 56:
		m_remainingtime = str;
		break;
	case 57:
		m_yuanbaonum = str;
		break;
	case 58:
	case 59:
	case 60:
	case 61:
		m_swnum[ i - 58] = str;
		break;
	default:
		break;
	}

}

void crJXJZiXuanLiBaoCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		/*int zixuanhuodongID = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJZiXuanLibaoHuodongID, 1).c_str());*/
		int zixuanhuodongID = 0;
		ref_ptr<crMultiSwitch>  button_sw13 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw13));
		ref_ptr<crMultiSwitch>  yuanbaonum = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_yuanbaonum));
		ref_ptr<crButtonWidgetNode> chongzhiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chongzhi));
		ref_ptr<crButtonWidgetNode> lingquBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqu));
		ref_ptr<crStaticTextWidgetNode> LingQuCiShuText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_LingQuCiShu));
		ref_ptr<crStaticTextWidgetNode> yichongzhiText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_yichongzhi));
		ref_ptr<crStaticTextWidgetNode> remainingtimesText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_remainingtimes));
		ref_ptr<crStaticTextWidgetNode> remainingtimeText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_remainingtime));
		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radioGroup));
		ref_ptr<crMultiSwitch>  swnum[4];
		int value = 0;
		if (groupradio.valid())
		{
			value = groupradio->getSelect();
		}
		std::string rewardiconbkname[16];
		std::string itemNumbkname[16];
		ref_ptr<crImageBoxWidgetNode> image[16];
		ref_ptr<crStaticTextWidgetNode> itemNum[16];
		ref_ptr<crSequence> sequence[16];
		ref_ptr<crNode> rewardiconbk[16];
		ref_ptr<crNode> itemNumbk[16];

		for (int i = 0; i < 16; ++i)
		{
			rewardiconbkname[i] = "IconBackboard" + crArgumentParser::appItoa(i + 1);
			itemNumbkname[i] = "InputBackboard" + crArgumentParser::appItoa(i + 1);
				
		}
		for (unsigned char i = 0; i < 16; ++ i)
		{
			
			image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			if (image[i].valid())
			{
				image[i]->setImageName(m_nullImage);
				image[i]->setVisiable(false);
			}
			itemNum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
			if (itemNum[i].valid())
			{
				itemNum[i]->clearString();
				itemNum[i]->setVisiable(false);
			}
			sequence[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua[i]));
			if (sequence[i].valid())
			{
				sequence[i]->setVisiable(false);
			}	
			rewardiconbk[i] = m_this->getChildNode(rewardiconbkname[i]);
			if (rewardiconbk[i].valid())
			{
				rewardiconbk[i]->setVisiable(false);
			}
			itemNumbk[i] = m_this->getChildNode(itemNumbkname[i]);
			if (itemNumbk[i].valid())
			{
				itemNumbk[i]->setVisiable(false);
			}
			if ( i < 4)
			{
				swnum[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_swnum[i]));
			}
		}

		/*crData * playerData = myPlayer->getDataClass();*/
		do 
		{
			void *param;
			crData * canvasData = m_this->getDataClass();
			if (!canvasData) break;
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if (false == huodongTab.valid()) break;
			crTableIO::StrVec huoRecord;
			int awardIdx[4] ;
			int titleIdx = huodongTab->getTitleIndex("标题");
			int opendayIdx = huodongTab->getTitleIndex("开服天数");
			int endTimeIdx = huodongTab->getTitleIndex("结束时间");
			int idIdx = huodongTab->getTitleIndex("id");
			int tiaojianIdx = huodongTab->getTitleIndex("条件");
			int cishuIdx = huodongTab->getTitleIndex("次数");

			awardIdx[0] = huodongTab->getTitleIndex("奖励1");
			awardIdx[1] = huodongTab->getTitleIndex("奖励2");
			awardIdx[2] = huodongTab->getTitleIndex("奖励3");
			awardIdx[3] = huodongTab->getTitleIndex("奖励4");
			ref_ptr<crRole> mainRole = myPlayer->getMainRole();
			if(mainRole.valid() && mainRole->getDataClass())
			{
				crData *data = mainRole->getDataClass();	
				CRCore::ScopedLock<crData> lock(*data);
				data->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->end();
				crTableIO::DataVec huodongIDVec;
				std::string titletext("自选礼包");
				huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
				if (!huodongIDVec.empty()) 
				{
					int huodongid = 0;
					for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
					{
						huodongid = atoi((*siter)[idIdx].c_str());
						itr = huodongMap->find(huodongid);	
						if(itr != huodongMap->end())
						{
							if(itr->second->isActivation())
							{
								zixuanhuodongID = huodongid;
								m_lasthuodongID = huodongid;
								break;
							}
						}
					}
					
				}
				if(zixuanhuodongID == 0)
				{
					zixuanhuodongID = m_lasthuodongID;
				}
				if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(zixuanhuodongID),huoRecord) < 0) break;

				int Conditions = 0;
				if (tiaojianIdx > 0)
				{
					Conditions = atoi(huoRecord[tiaojianIdx].c_str());
					int tmp = Conditions;
					if (yuanbaonum.valid())
					{
						int lenth = crArgumentParser::appItoa(tmp).length();
						if (lenth >= 1)
						{
							yuanbaonum->setActiveSwitchSet(lenth - 1);
						}
						for (unsigned char ii = 0; ii < lenth && ii < 4; ++ii)
						{
							if (swnum[ii].valid())
							{
								swnum[ii]->setVisiable(true);
								swnum[ii]->setActiveSwitchSet((tmp % 10));
							}
							tmp = tmp / 10;
						}
					}
				}

				CRCore::crVector2i opendayVec2i;
				crArgumentParser::appAtoVec(huoRecord[opendayIdx],opendayVec2i);
				time_t endtime = 0;
				time_t nowTime_t = time(0);
				if (opendayVec2i[0] != 0)
				{
					typedef std::vector<int> Vec5i;
					Vec5i startTimeVec5i;
					std::string dateStr = crGlobalHandle::gData()->gStartDate();
					crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
					startTimeVec5i.resize(5,0);
					struct  tm  nowTm = *localtime(&nowTime_t);
					struct tm  startTm;
					memset(&startTm,0,sizeof (startTm));
					startTm.tm_year = startTimeVec5i[0] - 1900;
					startTm.tm_mon = startTimeVec5i[1] - 1;
					startTm.tm_mday = startTimeVec5i[2];
					startTm.tm_hour = startTimeVec5i[3];
					startTm.tm_min = startTimeVec5i[4];
					startTm.tm_isdst = nowTm.tm_isdst;
					time_t startTime_t = mktime(&startTm);
					if (opendayVec2i[1] != 0)
					{
						endtime = startTime_t + opendayVec2i[1] * c_24hours;
					}
					else
					{
						endtime = startTime_t + opendayVec2i[0] * c_24hours;
					}
				}
				else
				{
					CRCore::crVector3i endtimeVec;
					crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
					struct tm tm_endtime;
					memset(&tm_endtime,0,sizeof(tm_endtime));
					tm_endtime.tm_year = endtimeVec[0] - 1900;
					tm_endtime.tm_mon = endtimeVec[1] - 1;
					tm_endtime.tm_mday = endtimeVec[2];
					tm_endtime.tm_hour = 23;
					tm_endtime.tm_min = 59;
					tm_endtime.tm_sec = 59;
					endtime = mktime(&tm_endtime);
				}
				time_t remainingtime = endtime - nowTime_t;
				//if (nowtime < starttime || nowtime > endtime)	break;
			
				int _hour = remainingtime /3600;
				int _min = (remainingtime  - 3600*_hour)/60;
				int _sec = (remainingtime  - 3600*_hour -60*_min);
				std::string remaining;
				if (_sec >= 0)
				{
					remaining =  crArgumentParser::appItoa(_hour) 
						+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
				}
				else
				{
					remaining = "0:00:00";
				}
				if (remainingtimeText.valid())
				{
					remainingtimeText->clearString();
					remainingtimeText->setString(remaining);
				}
			
				itr = huodongMap->find(zixuanhuodongID);	
				if(itr != huodongMap->end())
				{

					int shengyucishu = itr->second->getCircles();
					if (yichongzhiText.valid())
					{
						int zongcishu = atoi(huoRecord[cishuIdx].c_str());
						
						if (shengyucishu==1 && itr->second->getComplete() && itr->second->getStep() !=0)
						{
							yichongzhiText->setString(crArgumentParser::appItoa(itr->second->getStep()-Conditions + Conditions * zongcishu));
						}
						else
						{	
							yichongzhiText->setString(crArgumentParser::appItoa(itr->second->getStep() + Conditions * (zongcishu - shengyucishu)));
						}			
					}
					if (remainingtimesText.valid())
					{
						if (shengyucishu!=1 || !itr->second->getComplete())
						{
							remainingtimesText->setString(crArgumentParser::appItoa(shengyucishu));	
						}
						else
						{
							remainingtimesText->setString(crArgumentParser::appItoa(shengyucishu-1));		
						}
							
					}
					if(itr->second->isCanAdvance() && !itr->second->getComplete())
					{
						if (button_sw13.valid() && chongzhiBtn.valid() && lingquBtn.valid() && LingQuCiShuText.valid())
						{
							button_sw13->setActiveSwitchSet(1);
							chongzhiBtn->setVisiable(false);
							lingquBtn->setVisiable(true);
							int linqucishu = 0;
							if (Conditions > 0)
							{
								linqucishu = (itr->second->getStep() - itr->second->getStepCount())/Conditions + 1;
							}
							linqucishu = linqucishu < shengyucishu?linqucishu:shengyucishu;
							LingQuCiShuText->setVisiable(true);
							LingQuCiShuText->setString("[" + crArgumentParser::appItoa(linqucishu) + "]");
						}
					}
					else
					{
						if (button_sw13.valid() && chongzhiBtn.valid() && lingquBtn.valid() && LingQuCiShuText.valid())
						{
							if (shengyucishu==1 && itr->second->getComplete())
							{
								button_sw13->setActiveSwitchSet(1);
								chongzhiBtn->setVisiable(false);
								lingquBtn->setVisiable(false);
								LingQuCiShuText->setVisiable(false);
							}
							else
							{
								button_sw13->setActiveSwitchSet(0);
								chongzhiBtn->setVisiable(true);
								lingquBtn->setVisiable(false);
							}	
						}
					}
				}
			
			}

			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if (false == itemtab.valid()) break;
			crTableIO::StrVec itemRecord;
			int iconIdx = itemtab->getTitleIndex("icon");
			int itemIdIdx = itemtab->getTitleIndex("itemid");
			std::vector<int> awardVec;
			crVector4i award;
			std::string tmpstr;
			int awardnum;
			for (unsigned char i = 0;i<4;++i)
			{		
				crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
				if (!awardVec.empty())
				{
					awardnum = awardVec.size();
					for (unsigned char j=0;j<awardnum/4;++j)
					{
						if (j*4+3 < awardnum)
						{
							tmpstr = crArgumentParser::appItoa(awardVec[j*4]) + "|" + 
								crArgumentParser::appItoa(awardVec[j*4+1]) + "|" +
								crArgumentParser::appItoa(awardVec[j*4+2]) + "|" + 
								crArgumentParser::appItoa(awardVec[j*4+3]);
						}
						crArgumentParser::appAtoVec(tmpstr,award);
						if (image[i*4+j].valid())
						{
							image[i*4+j]->setVisiable(true);
							setRewardInfo(award,image[i*4+j].get());
						}
						if (rewardiconbk[i * 4 + j].valid())
						{
							rewardiconbk[i * 4 + j]->setVisiable(true);
						}
						if (itemNumbk[i * 4 + j].valid())
						{
							itemNumbk[i * 4 + j]->setVisiable(true);
						}
						if (award[3] >= 1)
						{
							if (itemNum[i*4+j].valid())
							{
								itemNum[i*4+j]->setVisiable(true);
								itemNum[i*4+j]->setString(crArgumentParser::appItoa(award[3]));
							}
						}
						if (sequence[i*4+j].valid() && i==value)
						{
							sequence[i*4+j]->setVisiable(true);
						}	
					}
				}	
			}

		} while (0);
	}
}


/**************************************************************************

METHOD: crJXJClientLibaoHuodongGetAwardsMethod
 
**************************************************************************/

crJXJClientLibaoHuodongGetAwardsMethod::crJXJClientLibaoHuodongGetAwardsMethod()
{

}

crJXJClientLibaoHuodongGetAwardsMethod::crJXJClientLibaoHuodongGetAwardsMethod( const crJXJClientLibaoHuodongGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_radioGroup(handle.m_radioGroup)
{

}

void crJXJClientLibaoHuodongGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJClientLibaoHuodongGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_input = str;
		break;
	case 4:
		m_radioGroup = str;
		break;
	default:
		break;
	}

}

void crJXJClientLibaoHuodongGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		/*int zixuanhuodongID = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJZiXuanLibaoHuodongID, 1).c_str());*/
		int zixuanhuodongID = 0;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && canvas->getDataClass())
		{
			bool valid = false;
			void *param;
			crData * canvasData = canvas->getDataClass();

			ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioGroup));
			unsigned char value = 0;
			if (groupradio.valid())
			{
				value = (unsigned char)groupradio->getSelect();
			}

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			ref_ptr<crRole> mainRole = playerData->getMainRole();
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if(mainRole.valid() && mainRole->getDataClass() )
			{
				int titleIdx = huodongTab->getTitleIndex("标题");
				int idIdx = huodongTab->getTitleIndex("id");
				crTableIO::DataVec huodongIDVec;
				std::string titletext("自选礼包");

				crData *roleData = mainRole->getDataClass();	
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->end();

				huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
				if (!huodongIDVec.empty()) 
				{	
					int huodongid = 0;
					for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
					{
						huodongid = atoi((*siter)[idIdx].c_str());
						itr = huodongMap->find(huodongid);	
						if(itr != huodongMap->end())
						{
							if(itr->second->isActivation())
							{
								zixuanhuodongID = huodongid;
								break;
							}
						}
					}
					
				}
				if(zixuanhuodongID > 0)
				{
					itr = huodongMap->find(zixuanhuodongID);
					if(itr != huodongMap->end())
					{
						if(itr->second->getActivation()==1)
						{
							crPlayerDataEventPacket packet;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(3);
							stream->_writeUShort(zixuanhuodongID);
							stream->_writeUChar(value);
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvLibaoHuodongGetAward,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
							valid = true;
						
						}
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			if (!valid)
			{
				do 
				{
					
					crTableIO::StrVec record;
					if (false == huodongTab.valid()) break;
					int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
					int IDIdx = huodongTab->getTitleIndex("id");
					if (-1 == failedInfoIdx || -1 == IDIdx) break;

					if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(zixuanhuodongID),record) < 0) break;

					ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
					if (infocanvas.valid() == false) break;
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",record[failedInfoIdx]);
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = infocanvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
				} while (0);
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvLibaoHuodongGetAwardMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvLibaoHuodongGetAwardMethod::crJXJRecvLibaoHuodongGetAwardMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvLibaoHuodongGetAwardMethod::crJXJRecvLibaoHuodongGetAwardMethod( const crJXJRecvLibaoHuodongGetAwardMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvLibaoHuodongGetAwardMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvLibaoHuodongGetAwardMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvLibaoHuodongGetAwardMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				unsigned short huodongID = m_stream->_readUShort();
				unsigned char value = m_stream->_readUChar();
				if(value<0||value>4) return;
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[5] ;
				for (unsigned char i = 0; i < 5; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");

				std::vector<int> awardVec;
				void *param;
				crData *playerGameData = m_this->getDataClass();
				crData *data = mainRole->getDataClass();
				playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
				if(itr != huodongMap->end())
				{
					if(itr->second->isCanAdvance() && !itr->second->getComplete())
					{
						if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
						{	
							if (!record.empty())
							{
								std::vector<CRCore::crVector4i> awards;
								crVector4i award;
								crArgumentParser::appAtoVec(record[awardIdx[value]],awardVec);
								int awardnum = awardVec.size();
								std::string tmpstr;
								for (unsigned char i =0;i+3 < awardnum;i+=4)
								{
									tmpstr.clear();
									if (i+3 < awardnum)
									{
										tmpstr = crArgumentParser::appItoa(awardVec[i]) + "|" + 
											crArgumentParser::appItoa(awardVec[i+1]) + "|" +
											crArgumentParser::appItoa(awardVec[i+2]) + "|" + 
											crArgumentParser::appItoa(awardVec[i+3]);
									}
									crArgumentParser::appAtoVec(tmpstr,award);
									awards.push_back(award);
								}

								logRet = 1;
								sendReward(awards,m_this,"活动奖励");
								itr->second->setActivation(2,playerid);

								crPlayerDataEventPacket packet;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(1);
								stream->_writeChar(logRet);
								crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvLibaoHuodongGetAward,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}		
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

				char gbuf[256];
				sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),huodongID,logRet);
				std::string logdata(gbuf);
				GameLogData gamelog(Log_HuodongGet,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}
			
		}
	}
}


/**************************************************************************

METHOD: crJXJNewGetAwardsByActiveCodeMethod
 
**************************************************************************/
JXJ::crJXJNewGetAwardsByActiveCodeMethod::crJXJNewGetAwardsByActiveCodeMethod():
	m_this(NULL)
{

}

JXJ::crJXJNewGetAwardsByActiveCodeMethod::crJXJNewGetAwardsByActiveCodeMethod( const crJXJNewGetAwardsByActiveCodeMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJNewGetAwardsByActiveCodeMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_activeCodeData = (ActiveCodeData *)(LOINT64(param64));
			m_retVal = (char *)(HIINT64(param64));
		}
		else
		{
			m_activeCodeData = NULL;
			m_retVal = NULL;
		}
		break;
	}
}

void JXJ::crJXJNewGetAwardsByActiveCodeMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJNewGetAwardsByActiveCodeMethod::operator()( crHandle &handle )
{
	if (NULL == m_this || NULL == m_activeCodeData || NULL == m_retVal)
		return;

	*m_retVal = JXJActiveCodeGetRet_suc;
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();

	// 0,查询该激活码的相关信息
	ref_ptr<crJXJActiveCodeQueryData > activeCodeQuery = new crJXJActiveCodeQueryData ;
	activeCodeQuery->buildQuerySql(m_activeCodeData->m_activeCode);
	globalSession->executeQuery(activeCodeQuery.get());

	bool found = false;
	crDataBase::QueryResultVec &queryResultVec = globalSession->getQueryResult();
	if(!queryResultVec.empty())
	{
		activeCodeQuery = dynamic_cast<crJXJActiveCodeQueryData *>(queryResultVec[0].get());
		found = true;
	}
	globalSession->releaseQuery();
	db->endSession(globalSession.get());

	ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	int idIdx = huodongTab->getTitleIndex("id");
	int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");
	crTableIO::StrVec record;
	
	do 
	{
		// 1,激活码不存在
		if(!found)
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid;
			break;
		}
		// 3,如果激活码已经用过了
		if (activeCodeQuery->getUsed()>0)
		{
			*m_retVal = JXJActiveCodeGetRet_failed_hadGetten;
			break;
		}
	
		// 4,该激活码在该区不可用
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		if (activeCodeQuery->getServerID()>0 && activeCodeQuery->getServerID() != callback->getServerID())
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid	;	
			break;
		}
		// 5,判断该类型的活动是否已经过期了
		ref_ptr<crJXJCodeTypeQueryData > codeTypeQuery = new crJXJCodeTypeQueryData ;
		codeTypeQuery->buildQuerySql(activeCodeQuery->getCodeType());

		globalSession = db->beginSession();
		globalSession->executeQuery(codeTypeQuery.get());
		crDataBase::QueryResultVec &codeTypeQueryResultVec = globalSession->getQueryResult();
		if(!codeTypeQueryResultVec.empty())
		{
			codeTypeQuery = dynamic_cast<crJXJCodeTypeQueryData *>(codeTypeQueryResultVec[0].get());
			time_t nowTime = time(0);
			char curDate[64];
			strftime( curDate, sizeof(curDate), "%Y-%m-%d %H:%M:%S",localtime(&nowTime)); 

			if (std::string(curDate) < codeTypeQuery->getValidDate() 
				|| std::string(curDate) > codeTypeQuery->getOverDate())
			{
				*m_retVal = JXJActiveCodeGetRet_failed_invalid	;	
			}
		}
		else
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid	;
		}
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		if(*m_retVal != JXJActiveCodeGetRet_suc)
			break;

		crTableIO::DataVec huodongIDVec;
	
		crRole *mainRole = m_this->getMainRole();

		if (mainRole && mainRole->getDataClass())
		{
			void *param;
			crData *data = mainRole->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator itr;	
			huodongTab->queryRecords(codeTypeIdx,crArgumentParser::appItoa(activeCodeQuery->getCodeType()),huodongIDVec);
			int huodongid = 0;
			if (!huodongIDVec.empty()) 
			{
				for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				{
					huodongid = atoi((*siter)[idIdx].c_str());
					itr = huodongMap->find(huodongid);	
					if(itr != huodongMap->end())
					{
						//if(itr->second->isCanAdvance() && !itr->second->getComplete())
						if(itr->second->isActivation())
						{
							m_activeCodeData->m_huodongID = huodongid;
							break;
						}
					}
				}
				if (m_activeCodeData->m_huodongID == 0)
				{
					*m_retVal = JXJActiveCodeGetRet_failed_activityhadGetten;
				}
			}
			else
			{
				*m_retVal = JXJActiveCodeGetRet_failed_invalid;
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			if(*m_retVal != JXJActiveCodeGetRet_suc)
				break;
		}
		else
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid;
			break;
		}

		// 6,条件均合法时，update数据库
		globalSession = db->beginSession();
		ref_ptr<crJXJActiveCodeUpdate> activeCodeUpdate = new crJXJActiveCodeUpdate;
		activeCodeUpdate->buildUpdateSql(m_activeCodeData->m_activeCode,m_this->getPlayerID());
		if (globalSession->executeUpdate(activeCodeUpdate.get()))
		{
			globalSession->commit();
		}
		else 
		{
			globalSession->rollback();
			*m_retVal = JXJActiveCodeGetRet_failed_databaseError;
		}
		db->endSession(globalSession.get());
	} while (0);
}

/**************************************************************************

METHOD: crJXJClientCodeHuodongGetAwardsMethod
 
**************************************************************************/

crJXJClientCodeHuodongGetAwardsMethod::crJXJClientCodeHuodongGetAwardsMethod()
{

}

crJXJClientCodeHuodongGetAwardsMethod::crJXJClientCodeHuodongGetAwardsMethod( const crJXJClientCodeHuodongGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{

}

void crJXJClientCodeHuodongGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJClientCodeHuodongGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_input = str;
		break;
	}

}

void crJXJClientCodeHuodongGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && canvas->getDataClass())
		{
			bool valid = false;

			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;

			ref_ptr <crEditWidgetNode> content = dynamic_cast<crEditWidgetNode *>(m_this->getParentCanvas()->getWidget(m_input));
			std::string activitycode;
			if (content.valid() && !content->getString().empty())
			{
				activitycode = content->getUTF8String();
				int codelenth = activitycode.length();
				if (codelenth < 32)
				{
					stream->createBuf(codelenth+4);
					stream->_writeString(activitycode);
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCodeHuodongGetAward,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
					valid = true;
				}
			}			
		
			if (!valid)
			{
				do 
				{
					int huodongID = Activation_CodeActivity;
					ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
					crTableIO::StrVec record;
					if (false == huodongTab.valid()) break;
					int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
					int IDIdx = huodongTab->getTitleIndex("id");
					if (-1 == failedInfoIdx || -1 == IDIdx) break;

					if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(huodongID),record) < 0) break;

					ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
					if (infocanvas.valid() == false) break;
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",record[failedInfoIdx]);
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = infocanvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
				} while (0);
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvCodeHuodongGetAwardMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvCodeHuodongGetAwardMethod::crJXJRecvCodeHuodongGetAwardMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvCodeHuodongGetAwardMethod::crJXJRecvCodeHuodongGetAwardMethod( const crJXJRecvCodeHuodongGetAwardMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvCodeHuodongGetAwardMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvCodeHuodongGetAwardMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvCodeHuodongGetAwardMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[5] ;
				for (unsigned char i = 0; i < 5; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");
				int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");

				CRCore::crVector4i awardVec;

				void *param;
				crData *playerGameData = m_this->getDataClass();
				crData *data = mainRole->getDataClass();
				playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
				data->excHandle(MAKEINT64(WCH_LockData,1));
				ActiveCodeData activeCodeData ;
				do 
				{
					
					// 说明是领取的是激活码活动
					std::string activeCode = m_stream->_readString();
					if(activeCode.empty() || activeCode.size()>32)
					{
						logRet = JXJActiveCodeGetRet_failed_invalid;
						break;
					}
					activeCodeData.m_activeCode = activeCode;
					activeCodeData.m_huodongID = 0;
					m_this->doEvent(WCH_JXJNewGetAwardsByActiveCode,MAKEINT64(&activeCodeData,&logRet));
					if (JXJActiveCodeGetRet_suc != logRet)
					{
						break;
					}

					data->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->find(activeCodeData.m_huodongID);	
					if (itr != huodongMap->end() && huodongTab->queryOneRecord(0,crArgumentParser::appItoa(activeCodeData.m_huodongID),record) >= 0)
					{
						std::vector<CRCore::crVector4i> awards;
						for (unsigned char i = 0; i < 5; ++ i)
						{
							if (record[awardIdx[i]] == "") continue;
							crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
							awards.push_back(awardVec);

						}
						logRet = 1;
						sendReward(awards,m_this,"活动奖励");
						itr->second->setActivation(2,playerid);
					}
				} while (0);
				data->excHandle(MAKEINT64(WCH_LockData,0));
				playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(logRet);
				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvCodeHuodongGetAward,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

				if (activeCodeData.m_huodongID!=0)
				{
					char gbuf[256];
					sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),activeCodeData.m_huodongID,logRet);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}
			else
			{
				int msgid = 4084;
				switch (_case)
				{
				case -1:
					break;
				case JXJActiveCodeGetRet_failed_hadGetten:
					msgid = 4080;
					break;
				case JXJActiveCodeGetRet_failed_invalid	:
					msgid = 4081;
					break;
				case JXJActiveCodeGetRet_failed_outOfDate:
					msgid = 4082;
					break;
				case JXJActiveCodeGetRet_failed_databaseError:
					msgid = 4083;
					break;
				case JXJActiveCodeGetRet_failed_activityhadGetten:
					msgid = 5000;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
		}
	}
}



/**************************************************************************

METHOD: crJXJClientWeeklySpecialGetAwardsMethod
 
**************************************************************************/

crJXJClientWeeklySpecialGetAwardsMethod::crJXJClientWeeklySpecialGetAwardsMethod()
{

}

crJXJClientWeeklySpecialGetAwardsMethod::crJXJClientWeeklySpecialGetAwardsMethod( const crJXJClientWeeklySpecialGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{

}

void crJXJClientWeeklySpecialGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJClientWeeklySpecialGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_huodongtype = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}

}

void crJXJClientWeeklySpecialGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		unsigned short huodongID = 0;
		void *param;
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		std::string titletext;
		unsigned char vipLevel = *(unsigned char *)param;
		if (m_huodongtype == 2)
		{
			titletext = "每周特惠累计";
		}
		else if(m_huodongtype == 1)
		{
			titletext = "每周特惠单笔";
		}
				
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && canvas->getDataClass())
		{
			bool valid = false;
			crData * canvasData = canvas->getDataClass();

			
			ref_ptr<crRole> mainRole = playerData->getMainRole();
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if(mainRole.valid() && mainRole->getDataClass() )
			{
				crData *roleData = mainRole->getDataClass();
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->end();
				crTableIO::DataVec zhouhuodongVec;
				int idIdx = huodongTab->getTitleIndex("id");
				int titleIdx = huodongTab->getTitleIndex("标题");
				huodongTab->queryRecords(titleIdx,titletext,zhouhuodongVec);	
				for(crTableIO::DataVec::iterator siter = zhouhuodongVec.begin(); siter!=zhouhuodongVec.end();++siter)
				{
					huodongID = atoi((*siter)[idIdx].c_str());
					NewHuodongMap ::iterator tmpitr = huodongMap->find(huodongID);	
					if(tmpitr != huodongMap->end())
					{
						if(tmpitr->second->isActivation())
						{
							itr = tmpitr;
							break;
						}
					}
				}
				if(itr != huodongMap->end())
				{
					if(itr->second->getActivation()==1)
					{
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeUShort(huodongID);
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvWeeklySpecialGetAward,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
						valid = true;
						
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			if (!valid)
			{
				do 
				{
					
					crTableIO::StrVec record;
					if (false == huodongTab.valid()) break;
					int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
					int IDIdx = huodongTab->getTitleIndex("id");
					if (-1 == failedInfoIdx || -1 == IDIdx) break;

					if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(huodongID),record) < 0) break;

					ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
					if (infocanvas.valid() == false) break;
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",record[failedInfoIdx]);
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = infocanvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
				} while (0);
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJRecvWeeklySpecialGetAwardsMethod
 
**************************************************************************/
JXJ::crJXJRecvWeeklySpecialGetAwardMethod::crJXJRecvWeeklySpecialGetAwardMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvWeeklySpecialGetAwardMethod::crJXJRecvWeeklySpecialGetAwardMethod( const crJXJRecvWeeklySpecialGetAwardMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvWeeklySpecialGetAwardMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvWeeklySpecialGetAwardMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvWeeklySpecialGetAwardMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				unsigned short huodongID = m_stream->_readUShort();
				

				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[5] ;
				for (unsigned char i = 0; i < 5; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");

				if (awardIdx[0] >=0 && awardIdx[1] >=0 && awardIdx[2] >=0 && awardIdx[3] >=0 && awardIdx[4] >=0)
				{
					crVector4i awardVec;
					void *param;
					crData *playerGameData = m_this->getDataClass();
					crData *data = mainRole->getDataClass();
					playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
					if(itr != huodongMap->end())
					{
						if(itr->second->isCanAdvance() && !itr->second->getComplete())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
							{
								std::vector<CRCore::crVector4i> awards;
								for (unsigned char i = 0; i < 5; ++ i)
								{
									if (record[awardIdx[i]] == "") continue;
									crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
									awards.push_back(awardVec);
								}
								sendReward(awards,m_this,"活动奖励");
								//RewardItemVec rewardItems;
								//rewardItems.reserve(5);
								//CRCore::ref_ptr<crBagItemData> itemPtr = NULL;
								//for (unsigned char i = 0; i < 5; ++ i)
								//{
								//	if (record[awardIdx[i]] == "") continue;
								//	crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
								//	itemPtr = new crBagItemData;
								//	itemPtr->setItemID(awardVec[0]);
								//	itemPtr->setEquipMagic(awardVec[1]);
								//	itemPtr->setItemCount(awardVec[2]);
								//	rewardItems.push_back(itemPtr);
								//}

								logRet = 1;
								//m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
								itr->second->setActivation(2,playerid);

								crPlayerDataEventPacket packet;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(1);
								stream->_writeChar(logRet);
								crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvWeeklySpecialGetAward,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
					playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

					char gbuf[256];
					sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),huodongID,logRet);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJLeijichongzhiCanvasUpdateMethod
//
///////////////////////////////////////
crJXJLeijichongzhiCanvasUpdateMethod::crJXJLeijichongzhiCanvasUpdateMethod():
	m_this(NULL),
	m_isxiaofei(0),
	m_curselectid(0)
{

}

crJXJLeijichongzhiCanvasUpdateMethod::crJXJLeijichongzhiCanvasUpdateMethod( const crJXJLeijichongzhiCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_lingqu(handle.m_lingqu),
	m_radio(handle.m_radio),
	m_input6(handle.m_input6),
	m_swshuzi(handle.m_swshuzi),
	m_sw11(handle.m_sw11),
	m_chongzhi(handle.m_chongzhi),
	m_isxiaofei(handle.m_isxiaofei),
	m_sw12(handle.m_sw12),
	m_sw13(handle.m_sw13),
	m_curselectid(handle.m_curselectid),
	m_hdCanvas(handle.m_hdCanvas)
{
	for (unsigned char i = 0;i < 15;++i)
	{
		m_icon[i] = handle.m_icon[i];
		m_input[i] = handle.m_input[i];
		if (i < 7)
		{
			m_swy[i] = handle.m_swy[i];
			m_donghuay[i] = handle.m_donghuay[i];
		}
		if (i<5)
		{
			m_inputd[i] = handle.m_inputd[i];
			m_donghua[i] = handle.m_donghua[i];
		}
	}

}

void crJXJLeijichongzhiCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJLeijichongzhiCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
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
		m_icon[i] = str;
		break;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		m_input[i-15] = str;
		break;
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		m_donghua[i-30] = str;
		break;
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		m_inputd[i-35] = str;
		break;
	case 40:
		m_input6 = str;
		break;
	case 41:
		m_lingqu = str;
		break;
	case 42:
		m_radio = str;
		break;
	case 43:
		m_swshuzi = str;
		break;
	case 44:
		m_sw11 = str;
		break;
	case 45:
		m_chongzhi = str;
		break;
	case 46:
		m_isxiaofei = (unsigned char)(atoi(str.c_str()));
		break;
	case 47:
		m_sw12 = str;
		break;
	case 48:
		m_sw13 = str;
		break;
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
		m_swy[i-49] = str;
		break;
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
	case 62:
		m_donghuay[i-56] = str;
		break;
	case 63:
		m_hdCanvas = str;
		break;
	default:
		break;
	}

}

void crJXJLeijichongzhiCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if (!itemtab.valid()) return;
		crTableIO::StrVec itemRecord;
		int iconIdx = itemtab->getTitleIndex("icon");
		int itemIdIdx = itemtab->getTitleIndex("itemid");
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (!huodongTab.valid()) return;
		crTableIO::DataVec zhouhuodongVec;
		int idIdx = huodongTab->getTitleIndex("id");
		int titleIdx = huodongTab->getTitleIndex("标题");
		std::string titletext("累计充值回馈");
		if (m_isxiaofei == 1)
		{
			titletext = "累计消费回馈";
			ref_ptr<crButtonWidgetNode> chongzhi = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chongzhi));
			if (chongzhi.valid())
			{
				chongzhi->setVisiable(false);
			}
		}
		huodongTab->queryRecords(titleIdx,titletext,zhouhuodongVec);	
		ref_ptr<crMultiSwitch> swshuzi = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_swshuzi));
		ref_ptr<crMultiSwitch> sw11 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw11));
		ref_ptr<crMultiSwitch> sw12 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw12));
		ref_ptr<crMultiSwitch> sw13 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw13));
		ref_ptr<crButtonWidgetNode> lingquBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqu));
		ref_ptr<crStaticTextWidgetNode> input6 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input6));
		ref_ptr<crRadioGroupWidgetNode> radio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radio));
		crRadioGroupWidgetNode::RadioGroup &radioGroup = radio->getRadioGroup();
		ref_ptr<crImageBoxWidgetNode> image[15];
		ref_ptr<crStaticTextWidgetNode> input[15];
		ref_ptr<crSequence> seqQ[5];
		ref_ptr<crStaticTextWidgetNode> inputd[5];	
		ref_ptr<crSequence> seqQY[7];
		ref_ptr<crMultiSwitch> swY[7];
		ref_ptr<crCanvasNode> huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_hdCanvas);
		if(!huodongcanvas.valid()) return;
		crData *canvasData = huodongcanvas->getDataClass();
		for (unsigned char i = 0; i < 15;++i)
		{
			image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
			if (i < 7)
			{
				seqQY[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghuay[i]));
				swY[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_swy[i]));
			}
			if (i < 5)
			{
				seqQ[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua[i]));
				inputd[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputd[i]));
			}
		}
		
		for (unsigned char i = 0;i < 7;++i)
		{
			if (i < 5)
			{
				if (seqQ[i].valid())
				{
					seqQ[i]->setVisiable(false);
				}
			}
			if (seqQY[i].valid())
			{
				seqQY[i]->setVisiable(false);
			}
			if (swY[i].valid())
			{
				swY[i]->setActiveSwitchSet(0);
				swY[i]->setVisiable(false);
			}
		}

		int awardIdx[5] ;
		for (unsigned char i = 0; i < 5; ++ i)
		{
			awardIdx[i] = -1;
		}
		awardIdx[0] = huodongTab->getTitleIndex("奖励1");
		awardIdx[1] = huodongTab->getTitleIndex("奖励2");
		awardIdx[2] = huodongTab->getTitleIndex("奖励3");
		awardIdx[3] = huodongTab->getTitleIndex("奖励4");
		awardIdx[4] = huodongTab->getTitleIndex("奖励5");
		int yuanbaonumIdx = huodongTab->getTitleIndex("条件");
		if (awardIdx[0] >=0 && awardIdx[1] >=0 && awardIdx[2] >=0 && awardIdx[3] >=0 && awardIdx[4] >=0)
		{
			crTableIO::StrVec huoRecord;
			ref_ptr<crRole> mainRole = myPlayer->getMainRole();
			if (mainRole.valid() && mainRole->getDataClass())
			{
				void *param;
				crData *roleData = mainRole->getDataClass();
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator tmpitr = huodongMap->end();
				int huodongID = 0;
				int radioGroupsize = radioGroup.size();
				int vecsize = zhouhuodongVec.size();
				bool isallfinished = true;		//是否都完成
				bool isoverdue = false;			//是否过期
				crTableIO::DataVec tmpVec;
				for(unsigned char i = 0; i < vecsize;i += radioGroupsize)
				{
					isallfinished = true;
					isoverdue = false;
					for (unsigned char j = 0; j + i < vecsize && j < radioGroupsize;++j)
					{
						huodongID = atoi(zhouhuodongVec[i+j][idIdx].c_str());
						tmpitr = huodongMap->find(huodongID);
						if(tmpitr != huodongMap->end())
						{
							if(tmpitr->second->getActivation() == 0)
							{
								isoverdue = true;
							}
							if (tmpitr->second->getActivation() != 2)
							{
								isallfinished = false;
							}
						}
					}
					if (!isallfinished && !isoverdue)
					{
						unsigned char _index = 0;
						for (unsigned char m = i; m < vecsize && _index<radioGroupsize;++m)
						{
							tmpVec.push_back(zhouhuodongVec[m]);
							++_index;
						}
						break;
					}
				}
				int Vecsize = tmpVec.size()<=radioGroupsize?tmpVec.size():radioGroupsize;
				int selectid = -1;
				if (radio.valid())
				{ 
					selectid = radio->getSelect();
				}
				if (selectid < 0 || selectid >= Vecsize)
				{
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
					return;
				}
				
				std::vector<std::string> itemsVec;
				crVector4i awardVec;
				std::string iconfile1;
				crMultiSwitch *radioButton;
				crStateSet *ss1;
				crTexture2D *tex2d;
				crRadioGroupWidgetNode::RadioGroup::iterator ritr = radioGroup.begin();
				
				int itemNum = 0;
				int _Idx = 0;
				int inputIdx = 0;
				int yuanbaonum = 0;
				int index = 0;
				for(crTableIO::DataVec::iterator siter = tmpVec.begin(); siter!=tmpVec.end() && ritr!=radioGroup.end();++siter)
				{
					huodongID = atoi((*siter)[idIdx].c_str());
					tmpitr = huodongMap->find(huodongID);
					if(tmpitr != huodongMap->end())
					{
						if(tmpitr->second->getActivation() > 0)
						{
							if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID),huoRecord) < 0) continue;
							if (selectid == index)
							{
								if (canvasData)
								{
									canvasData->inputParam(WCHDATA_JXJSelectLeiJiHuoDongID,&huodongID);
								}
								itemNum = 0;
								itemsVec.clear();
								for (unsigned char j = 0;j<5;++j)
								{
									if (huoRecord[awardIdx[j]] != "")
									{
										itemsVec.push_back(huoRecord[awardIdx[j]]);
										++itemNum;
									}
								}
								if (sw12.valid() && sw13.valid())
								{
									if (itemNum > 0 && itemNum <=5)
									{
										sw12->setActiveSwitchSet(itemNum -1);
										sw13->setActiveSwitchSet(itemNum -1);
									}
								}
								yuanbaonum = atoi(huoRecord[yuanbaonumIdx].c_str());
								switch (itemNum)
								{
								case 1:
									_Idx = 4;	
									crArgumentParser::appAtoVec(itemsVec[0],awardVec);			
									if (image[_Idx].valid())
									{
										image[_Idx]->setVisiable(true);
										setRewardInfo(awardVec,image[_Idx].get());
									}
									if (input[_Idx].valid())
									{
										input[_Idx]->setString(crArgumentParser::appItoa(awardVec[3]));
									}	
									break;
								case 2:
									_Idx = 5;
									for (unsigned char j = 0;j<itemNum;++j)
									{	
										crArgumentParser::appAtoVec(itemsVec[j],awardVec);			
										if (image[_Idx].valid())
										{
											image[_Idx]->setVisiable(true);
											setRewardInfo(awardVec,image[_Idx].get());
										}
										if (input[_Idx].valid())
										{
											input[_Idx]->setString(crArgumentParser::appItoa(awardVec[3]));
										}
										++_Idx;
									}
									break;
								case 3:
									_Idx = 7;	
									for (unsigned char j = 0;j<itemNum;++j)
									{	
										crArgumentParser::appAtoVec(itemsVec[j],awardVec);			
										if (image[_Idx].valid())
										{
											image[_Idx]->setVisiable(true);
											setRewardInfo(awardVec,image[_Idx].get());
										}
										if (input[_Idx].valid())
										{
											input[_Idx]->setString(crArgumentParser::appItoa(awardVec[3]));
										}
										++_Idx;						
									}
									break;
								case 4:
									_Idx = 0;
									for (unsigned char j = 0;j<itemNum;++j)
									{	
										crArgumentParser::appAtoVec(itemsVec[j],awardVec);			
										if (image[_Idx].valid())
										{
											image[_Idx]->setVisiable(true);
											setRewardInfo(awardVec,image[_Idx].get());
										}
										if (input[_Idx].valid())
										{
											input[_Idx]->setString(crArgumentParser::appItoa(awardVec[3]));
										}
										++_Idx;			
									}
									break;
								case 5:
									_Idx = 10;
									for (unsigned char j = 0;j<itemNum;++j)
									{	
										crArgumentParser::appAtoVec(itemsVec[j],awardVec);			
										if (image[_Idx].valid())
										{
											image[_Idx]->setVisiable(true);
											setRewardInfo(awardVec,image[_Idx].get());
										}
										if (input[_Idx].valid())
										{
											input[_Idx]->setString(crArgumentParser::appItoa(awardVec[3]));
										}
										++_Idx;			
									}
									break;
								default:
									break;
								}
								if (lingquBtn.valid() && sw11.valid())
								{
									if (tmpitr->second->isActivation() && tmpitr->second->isCanAdvance())
									{
										sw11->setActiveSwitchSet(1);
										lingquBtn->setEnable(true);
									}
									else if(tmpitr->second->getComplete())
									{
										sw11->setActiveSwitchSet(1);
										lingquBtn->setEnable(false);
									}
									else
									{
										sw11->setActiveSwitchSet(0);
									}
								}

							}

							crArgumentParser::appAtoVec(huoRecord[awardIdx[0]],awardVec);
							 if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(awardVec[1]),itemRecord) >= 0)
							 {
								 iconfile1 = itemRecord[iconIdx];
								 radioButton = ritr->second->getButton();
								 radioButton->setVisiable(true);
								 ss1 = dynamic_cast<crObject *>(radioButton->getChild(0))->getDrawable(0)->getStateSet();

								 tex2d = dynamic_cast<crTexture2D *>(ss1->getTextureAttribute(0,crStateAttribute::TEXTURE));
								 if(tex2d)
								 {
									 if(tex2d->getImageNameID().compare(iconfile1)!=0)
										 tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
								 }
								 else
								 {
									 tex2d = new crTexture2D;
									 tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
									 tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
									 tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
									 tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
									 tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
									 tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
									 ss1->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
								 }
							 }
							 ++ritr;

							if (seqQ[index].valid() && inputd[index].valid())
							{
								if (tmpitr->second->isActivation() && tmpitr->second->isCanAdvance())
								{
									seqQ[index]->setVisiable(true);	
								}
								else
								{
									seqQ[index]->setVisiable(false);
								}
								inputd[index]->setString(crArgumentParser::appItoa(tmpitr->second->getStepCount()) + "元宝");
								++index;
							}
							if (tmpitr->second->isActivation())
							{
								if (input6.valid())
								{
									input6->setString(crArgumentParser::appItoa(tmpitr->second->getStep()));
								}
							}
						}
					}

					if (index >= Vecsize)
					{
						break;
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
				if (swshuzi.valid())
				{
					swshuzi->setActiveSwitchSet(0);
					unsigned char _index = (7 - crArgumentParser::appItoa(yuanbaonum).length()) / 2;
					while (yuanbaonum > 0 && _index < 7 && _index >=0)
					{
						if (swY[_index].valid())
						{
							swY[_index]->setVisiable(true);
							swY[_index]->setActiveSwitchSet((yuanbaonum % 10));
						}
						yuanbaonum = yuanbaonum / 10;
						++_index;
					}
					for (unsigned char ii = 0; ii <= _index && ii <7 ;++ii)
					{
						if (seqQY[ii].valid())
						{
							if (selectid == m_curselectid)
							{
								seqQY[ii]->setVisiable(false);
							}
							else
							{
								swshuzi->setActiveSwitchSet(1);
								seqQY[ii]->setVisiable(true);
								seqQY[ii]->startEffect();
							}
						}
					}
				}
				m_curselectid = selectid;
			}	
		}	

	}
}

/**************************************************************************

METHOD: crJXJLeijiChongzhiGetAwardsMethod
 
**************************************************************************/

crJXJLeijiChongzhiGetAwardsMethod::crJXJLeijiChongzhiGetAwardsMethod():
	m_isxiaofei(0)
{

}

crJXJLeijiChongzhiGetAwardsMethod::crJXJLeijiChongzhiGetAwardsMethod( const crJXJLeijiChongzhiGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_radioGroup(handle.m_radioGroup),
	m_isxiaofei(handle.m_isxiaofei),
	m_hdCanvas(handle.m_hdCanvas)
{

}

void crJXJLeijiChongzhiGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJLeijiChongzhiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_input = str;
		break;
	case 4:
		m_radioGroup = str;
		break;
	case 5:
		m_isxiaofei =(unsigned char)(atoi(str.c_str()));
		break;
	case 6:
		m_hdCanvas = str;
		break;
	default:
		break;
	}

}

void crJXJLeijiChongzhiGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		int leijichongzhiID = 0;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && canvas->getDataClass())
		{
			bool valid = false;
			void *param;
			ref_ptr<crCanvasNode> huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_hdCanvas);
			if(!huodongcanvas.valid()) return;
			crData *canvasData = huodongcanvas->getDataClass();
			ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioGroup));
			unsigned char value = 0;
			if (groupradio.valid())
			{
				value = (unsigned char)groupradio->getSelect();
			}
			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			ref_ptr<crRole> mainRole = playerData->getMainRole();
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if(mainRole.valid() && mainRole->getDataClass() )
			{
				//int titleIdx = huodongTab->getTitleIndex("标题");
				//int idIdx = huodongTab->getTitleIndex("id");
				//crTableIO::DataVec huodongIDVec;
				//std::string titletext("累计充值回馈");
				//if (m_isxiaofei == 1)
				//{
				//	titletext = "累计消费回馈";
				//}
				//huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
				//if (!huodongIDVec.empty()) 
				//{	
				//	int huodongid = 0;
				//	int index = 0;
				//	for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				//	{
				//		huodongid = atoi((*siter)[idIdx].c_str());
				//		itr = huodongMap->find(huodongid);	
				//		if(itr != huodongMap->end())
				//		{
				//			if(itr->second->getActivation() > 0)
				//			{
				//				if (index == value)
				//				{
				//					leijichongzhiID = huodongid;
				//					break;
				//				}
				//				++index;
				//			}
				//		}
				//	}
				//	
				//}
				if (canvasData)
				{
					canvasData->getParam(WCHDATA_JXJSelectLeiJiHuoDongID,param);
					leijichongzhiID = *(int *)param;
				}
				if(leijichongzhiID > 0)
				{
					crData *roleData = mainRole->getDataClass();	
					if (roleData)
					{
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *huodongMap= (NewHuodongMap *)param;
						NewHuodongMap ::iterator itr = huodongMap->end();
						itr = huodongMap->find(leijichongzhiID);
						if(itr != huodongMap->end())
						{
							if(itr->second->getActivation()==1)
							{
								crPlayerDataEventPacket packet;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(3);
								stream->_writeUShort(leijichongzhiID);
								stream->_writeUChar(value);
								crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvLeijiChongzhiGetAward,stream.get());
								netConductor->getNetManager()->sendPacket("all",packet);
								valid = true;

							}
						}
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
			if (!valid)
			{
				do 
				{	
					crTableIO::StrVec record;
					if (false == huodongTab.valid()) break;
					int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
					int IDIdx = huodongTab->getTitleIndex("id");
					if (-1 == failedInfoIdx || -1 == IDIdx) break;

					if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(leijichongzhiID),record) < 0) break;

					ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
					if (infocanvas.valid() == false) break;
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",record[failedInfoIdx]);
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = infocanvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
				} while (0);
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvLeijiChongzhiGetAwardsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvLeijiChongzhiGetAwardsMethod::crJXJRecvLeijiChongzhiGetAwardsMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvLeijiChongzhiGetAwardsMethod::crJXJRecvLeijiChongzhiGetAwardsMethod( const crJXJRecvLeijiChongzhiGetAwardsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence)
{

}

void JXJ::crJXJRecvLeijiChongzhiGetAwardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvLeijiChongzhiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvLeijiChongzhiGetAwardsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				unsigned short huodongID = m_stream->_readUShort();
				unsigned char value = m_stream->_readUChar();
				if(value<0||value>4) return;
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[5] ;
				for (unsigned char i = 0; i < 5; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");

				if (awardIdx[0] >=0 && awardIdx[1] >=0 && awardIdx[2] >=0 && awardIdx[3] >=0 && awardIdx[4] >=0)
				{
					crVector4i awardVec;
					void *param;
					crData *playerGameData = m_this->getDataClass();
					crData *data = mainRole->getDataClass();
					playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
					if(itr != huodongMap->end())
					{
						if(itr->second->isCanAdvance() && !itr->second->getComplete())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
							{
								std::vector<CRCore::crVector4i> awards;
								for (unsigned char i = 0; i < 5; ++ i)
								{
									if (record[awardIdx[i]] == "") continue;
									crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
									awards.push_back(awardVec);
								}

								logRet = 1;
								sendReward(awards,m_this,"活动奖励");
								itr->second->setActivation(2,playerid);

								crPlayerDataEventPacket packet;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(1);
								stream->_writeChar(logRet);
								crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvLeijiChongzhiGetAward,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
					playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

					char gbuf[256];
					sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),huodongID,logRet);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}

		}
	}
}


/////////////////////////////////////////
//
//crJXJDanbichongzhiCanvasUpdateMethod
//
///////////////////////////////////////
crJXJDanbichongzhiCanvasUpdateMethod::crJXJDanbichongzhiCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJDanbichongzhiCanvasUpdateMethod::crJXJDanbichongzhiCanvasUpdateMethod( const crJXJDanbichongzhiCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_remainingtime(handle.m_remainingtime)
{
	for (unsigned char i = 0;i < 16;++i)
	{
		m_icon[i]= handle.m_icon[i];
		m_donghua[i]=handle.m_donghua[i];
		if (i<4)
		{
			m_input1[4]=handle.m_input1[4];
			m_input2[4]=handle.m_input2[4];
			m_lingqubtn[4]=handle.m_lingqubtn[4];
		}
	}

}

void crJXJDanbichongzhiCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJDanbichongzhiCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
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
		m_icon[i] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		m_input1[i-16] = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
		m_input2[i-20] = str;
		break;
	case 24:
	case 25:
	case 26:
	case 27:
		m_lingqubtn[i-24] = str;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
		m_donghua[i-28] = str;
		break;
	case 44:
		m_remainingtime = str;
		break;
	default:
		break;
	}

}

void crJXJDanbichongzhiCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crButtonWidgetNode> lingquBtn[4];
		ref_ptr<crImageBoxWidgetNode> image[16];
		ref_ptr<crSequence> seqQ[16];
		ref_ptr<crStaticTextWidgetNode> input1[4];
		ref_ptr<crStaticTextWidgetNode> input2[4];
		for (unsigned char i=0;i<16;++i)
		{
			image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));

			seqQ[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua[i]));
			if (i<4)
			{
				lingquBtn[i]= dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqubtn[i]));
				input1[i]= dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1[i]));
				input2[i]= dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2[i]));
			}
		}
		ref_ptr<crStaticTextWidgetNode> remainingtimetext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_remainingtime));
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if (!itemtab.valid()) return;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (!huodongTab.valid()) return;
		crTableIO::StrVec itemRecord;
		int iconIdx = itemtab->getTitleIndex("icon");
		int itemIdIdx = itemtab->getTitleIndex("itemid");	
		crTableIO::DataVec danbihuodongVec;
		int idIdx = huodongTab->getTitleIndex("id");
		int titleIdx = huodongTab->getTitleIndex("标题");
		int tiaojianIdx = huodongTab->getTitleIndex("条件2");
		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
		std::string titletext("单笔充值回馈");
		huodongTab->queryRecords(titleIdx,titletext,danbihuodongVec);	
		
		for (unsigned char i = 0;i < 16;++i)
		{
			if (seqQ[i].valid())
			{
				seqQ[i]->setVisiable(false);
			}
			if (i<4)
			{
				lingquBtn[i]->setEnable(false);
			}
		}

		int awardIdx[4] ;
		for (unsigned char i = 0; i < 4; ++ i)
		{
			awardIdx[i] = -1;
		}
		awardIdx[0] = huodongTab->getTitleIndex("奖励1");
		awardIdx[1] = huodongTab->getTitleIndex("奖励2");
		awardIdx[2] = huodongTab->getTitleIndex("奖励3");
		awardIdx[3] = huodongTab->getTitleIndex("奖励4");
	
		ref_ptr<crRole> mainRole = myPlayer->getMainRole();
		if (mainRole.valid() && mainRole->getDataClass())
		{
			int huodongID = 0;
			int index = 0;
			int Vecsize = danbihuodongVec.size()<=4?danbihuodongVec.size():4;		
			crVector4i awardVec;
			void *param;
			crData *roleData = mainRole->getDataClass();
			roleData->excHandle(MAKEINT64(WCH_LockData,1));
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator tmpitr = huodongMap->end();
			crTableIO::StrVec huoRecord;
			std::vector <int> Vec;
			std::string tmpstr;
			for(crTableIO::DataVec::iterator siter = danbihuodongVec.begin(); siter!=danbihuodongVec.end();++siter)
			{
				huodongID = atoi((*siter)[idIdx].c_str());
				tmpitr = huodongMap->find(huodongID);
				
				if(tmpitr != huodongMap->end())
				{
					if(tmpitr->second->getActivation() > 0)
					{
						if (tiaojianIdx > 0)
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),huoRecord) >= 0)
							{
								crArgumentParser::appAtoVec(huoRecord[tiaojianIdx],Vec);
								if (Vec.size() >=2 && input1[index].valid())
								{
									input1[index]->setString(crArgumentParser::appItoa(Vec[0]) /*+ "~" + crArgumentParser::appItoa(Vec[1])*/);
								}
							}
						}
						if (input2[index].valid())
						{
							if (tmpitr->second->getComplete())
							{
								input2[index]->setString(crArgumentParser::appItoa(0));
							}
							else
							{
								input2[index]->setString(crArgumentParser::appItoa(tmpitr->second->getCircles()));
							}
						}
						if (tmpitr->second->isCanAdvance() && !tmpitr->second->getComplete())
						{
							if (lingquBtn[index].valid())
							{
								lingquBtn[index]->setEnable(true);
							}
						}
						for (unsigned char i = 0;i<4 ;i++)
						{
							crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
							int _idx = i+index*4;
							if (_idx < 16 && _idx >=0)
							{
								if (image[_idx].valid())
								{
									image[_idx]->setVisiable(true);
									setRewardInfo(awardVec,image[_idx].get());
								}
								if (tmpitr->second->isCanAdvance() && !tmpitr->second->getComplete())
								{
									if (seqQ[_idx].valid())
									{
										seqQ[_idx]->setVisiable(true);
									}
								}
							}			
						}
						
					}
				}
				++index;
				if (index >= Vecsize)
				{
					break;
				}
			}
			roleData->excHandle(MAKEINT64(WCH_LockData,0));

			if (huodongID > 0 && !huoRecord.empty())
			{
				time_t nowtime = time(0);
				CRCore::crVector3i starttimeVec;
				CRCore::crVector3i endtimeVec;
				crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
				crArgumentParser::appAtoVec(huoRecord[startTimeIdx],starttimeVec);
				struct tm tm_endtime;
				memset(&tm_endtime,0,sizeof(tm_endtime));
				struct tm tm_starttime;
				memset(&tm_starttime,0,sizeof(tm_starttime));
				tm_endtime.tm_year = endtimeVec[0] - 1900;
				tm_endtime.tm_mon = endtimeVec[1] - 1;
				tm_endtime.tm_mday = endtimeVec[2];
				tm_endtime.tm_hour = 23;
				tm_endtime.tm_min = 59;
				tm_endtime.tm_sec = 59;
				time_t endtime = mktime(&tm_endtime);
				tm_starttime.tm_year = starttimeVec[0] - 1900;
				tm_starttime.tm_mon = starttimeVec[1] - 1;
				tm_starttime.tm_mday = starttimeVec[2];
				tm_starttime.tm_hour = 0;
				tm_starttime.tm_min = 0;
				tm_starttime.tm_sec = 0;
				time_t starttime = mktime(&tm_starttime);
				time_t remainingtime = endtime - nowtime;

				int _hour = remainingtime /3600;
				int _min = (remainingtime  - 3600*_hour)/60;
				int _sec = (remainingtime  - 3600*_hour -60*_min);
				std::string remaining;
				if (_sec >= 0)
				{
					remaining =  crArgumentParser::appItoa(_hour) 
						+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
				}
				else
				{
					remaining = "0:00:00";
				}
				if (remainingtimetext.valid())
				{
					remainingtimetext->clearString();
					remainingtimetext->setString(remaining);
				}
			}
		}	

	}
}


/**************************************************************************

METHOD: crJXJDanbiChongzhiGetAwardsMethod
 
**************************************************************************/

crJXJDanbiChongzhiGetAwardsMethod::crJXJDanbiChongzhiGetAwardsMethod()
{

}

crJXJDanbiChongzhiGetAwardsMethod::crJXJDanbiChongzhiGetAwardsMethod( const crJXJDanbiChongzhiGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_index(handle.m_index)
{

}

void crJXJDanbiChongzhiGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJDanbiChongzhiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_input = str;
		break;
	case 4:
		m_index = str;
		break;
	default:
		break;
	}

}

void crJXJDanbiChongzhiGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		unsigned short danbichongzhiID = 0;

		bool valid = false;
		void *param;
		int value = atoi(m_index.c_str());
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crRole> mainRole = playerData->getMainRole();
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if(mainRole.valid() && mainRole->getDataClass() )
		{
			int titleIdx = huodongTab->getTitleIndex("标题");
			int idIdx = huodongTab->getTitleIndex("id");
			crTableIO::DataVec huodongIDVec;
			std::string titletext("单笔充值回馈");

			crData *roleData = mainRole->getDataClass();	
			roleData->excHandle(MAKEINT64(WCH_LockData,1));
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator itr = huodongMap->end();

			huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
			if (!huodongIDVec.empty()) 
			{	
				int huodongid = 0;
				int index = 0;
				for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				{
					huodongid = atoi((*siter)[idIdx].c_str());
					itr = huodongMap->find(huodongid);	
					if(itr != huodongMap->end())
					{
						if(itr->second->getActivation() > 0)
						{
							if (index == value)
							{
								danbichongzhiID = huodongid;
								break;
							}
							++index;
						}
					}
				}		
			}
			if(danbichongzhiID > 0)
			{
				itr = huodongMap->find(danbichongzhiID);
				if(itr != huodongMap->end())
				{
					if(itr->second->getActivation()==1)
					{
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(3);
						stream->_writeUShort(danbichongzhiID);
						stream->_writeUChar(value);
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvDanbiChongzhiGetAward,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
						valid = true;
						
					}
				}
			}
			roleData->excHandle(MAKEINT64(WCH_LockData,0));

			if (!valid)
			{
				do 
				{	
					crTableIO::StrVec record;
					if (false == huodongTab.valid()) break;
					int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
					int IDIdx = huodongTab->getTitleIndex("id");
					if (-1 == failedInfoIdx || -1 == IDIdx) break;

					if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(danbichongzhiID),record) < 0) break;

					ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
					if (infocanvas.valid() == false) break;
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",record[failedInfoIdx]);
					cfg_script.Pop();

					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = infocanvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
				} while (0);
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvDanbiChongzhiGetAwardsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvDanbiChongzhiGetAwardsMethod::crJXJRecvDanbiChongzhiGetAwardsMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvDanbiChongzhiGetAwardsMethod::crJXJRecvDanbiChongzhiGetAwardsMethod( const crJXJRecvDanbiChongzhiGetAwardsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvDanbiChongzhiGetAwardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvDanbiChongzhiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvDanbiChongzhiGetAwardsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				unsigned short huodongID = m_stream->_readUShort();
				unsigned char value = m_stream->_readUChar();
				if(value<0||value>4) return;
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[5] ;
				for (unsigned char i = 0; i < 5; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");

				if (awardIdx[0] >=0 && awardIdx[1] >=0 && awardIdx[2] >=0 && awardIdx[3] >=0 && awardIdx[4] >=0)
				{
					crVector4i awardVec;
					void *param;
					crData *playerGameData = m_this->getDataClass();
					crData *data = mainRole->getDataClass();
					playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
					if(itr != huodongMap->end())
					{
						if(itr->second->isCanAdvance() && !itr->second->getComplete())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
							{
								std::vector<CRCore::crVector4i> awards;
								for (unsigned char i = 0; i < 5; ++ i)
								{
									if (record[awardIdx[i]] == "") continue;
									crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
									awards.push_back(awardVec);
								}

								logRet = 1;
								sendReward(awards,m_this,"活动奖励");
								itr->second->setActivation(2,playerid);

								crPlayerDataEventPacket packet;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(1);
								stream->_writeChar(logRet);
								crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvDanbiChongzhiGetAward,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
					playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

					char gbuf[256];
					sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),huodongID,logRet);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
				
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}

		}
	}
}


/////////////////////////////////////////
//
//crJXJZhouKaCanvasUpdateMethod
//
///////////////////////////////////////
crJXJZhouKaCanvasUpdateMethod::crJXJZhouKaCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJZhouKaCanvasUpdateMethod::crJXJZhouKaCanvasUpdateMethod( const crJXJZhouKaCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_chongzhi(handle.m_chongzhi),
	m_remainingtime(handle.m_remainingtime),
	m_ingotnum(handle.m_ingotnum),
	m_lingqubtn(handle.m_lingqubtn),
	m_sw18(handle.m_sw18)
{
	for (unsigned char i = 0;i < 14;++i)
	{
		m_input[i]= handle.m_input[i];
		if (i < 7)
		{
			m_icon[i] = handle.m_icon[i];
			m_donghua[i] = handle.m_donghua[i];
			m_sw[i] = handle.m_sw[i];
		}
	}

}

void crJXJZhouKaCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJZhouKaCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_icon[i] = str;
		break;
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
	case 17:
	case 18:
	case 19:
	case 20:
		m_input[i-7] = str;
		break;
	case 21:
		m_ingotnum = str;
		break;
	case 22:
		m_remainingtime = str;
		break;
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		m_donghua[i-23] = str;
		break;
	case 30:
		m_chongzhi = str;
		break;
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		m_sw[i-31] = str;
		break;
	case 38:
		m_lingqubtn = str;
		break;
	case 39:
		m_sw18 = str;
		break;
	default:
		break;
	}

}

void crJXJZhouKaCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJZhouKaGetStatus,param);
		unsigned char zhoukagetstatus = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJZhouKaEndTime,param);
		_crInt64 zhoukaendtime = *(_crInt64 *)param;
		data->getParam(WCHDATA_JXJGainedZhouKaID,param);
		int zhoukaID = *(int *)param;
		ref_ptr<crButtonWidgetNode> chongzhiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chongzhi));
		ref_ptr<crButtonWidgetNode> lingquBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqubtn));
		ref_ptr<crMultiSwitch> sw18 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw18));
		ref_ptr<crSequence> seqQ[7];
		ref_ptr<crButtonWidgetNode> iconbtn[7];
		ref_ptr<crStaticTextWidgetNode> inputnum[7];
		ref_ptr<crStaticTextWidgetNode> inputstatus[7];
		ref_ptr<crStaticTextWidgetNode> remainingtimetext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_remainingtime));
		ref_ptr<crStaticTextWidgetNode> ingotnumtext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_ingotnum));
		ref_ptr<crMultiSwitch> sw[7];
		std::string textstring;
		for (unsigned char i=0;i<14;++i)
		{
			if (i<7)
			{
				iconbtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_icon[i]));
				seqQ[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua[i]));
				inputstatus[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
				sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw[i]));
			}
			else
			{
				inputnum[i-7] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
			}
		}

		for (unsigned char i = 0;i < 7;++i)
		{
				if (seqQ[i].valid())
				{
					seqQ[i]->setVisiable(false);
				}
				if (iconbtn[i].valid())
				{
					iconbtn[i]->setEnable(false);
				}
				
				if (sw[i].valid())
				{
					sw[i]->setActiveSwitchSet((zhoukagetstatus >> i) & 1);
				}
		}
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (!huodongTab.valid()) return;
			
		crTableIO::DataVec zhoukahuodongVec;
		int idIdx = huodongTab->getTitleIndex("id");
		int titleIdx = huodongTab->getTitleIndex("标题");
		int opendayIdx = huodongTab->getTitleIndex("开服天数");
		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
		int awardIdx = huodongTab->getTitleIndex("奖励1");
		std::string titletext("小周卡");
		huodongTab->queryRecords(titleIdx,titletext,zhoukahuodongVec);	

		ref_ptr<crRole> mainRole = myPlayer->getMainRole();
		if (mainRole.valid() && mainRole->getDataClass())
		{
			int huodongID = 0;	
			crTableIO::StrVec huoRecord;
			crData *roleData = mainRole->getDataClass();
			roleData->excHandle(MAKEINT64(WCH_LockData,1));
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator tmpitr = huodongMap->end();
			if (zhoukaID > 0)
			{
				huodongID = zhoukaID;
				tmpitr = huodongMap->find(huodongID);
				if(tmpitr != huodongMap->end())
				{	
					if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),huoRecord) >= 0)
					{
						if (ingotnumtext.valid())
						{
							ingotnumtext->setString(crArgumentParser::appItoa(tmpitr->second->getStep()));
						}
					}		
				}
				if (chongzhiBtn.valid() && sw18.valid() && lingquBtn.valid())
				{
					chongzhiBtn->setVisiable(false);
					lingquBtn->setVisiable(true);
					lingquBtn->setEnable(false);
					sw18->setActiveSwitchSet(1);
				}
			}
			else
			{	
				if (chongzhiBtn.valid() && sw18.valid() && lingquBtn.valid())
				{
					chongzhiBtn->setVisiable(true);
					lingquBtn->setVisiable(false);
					sw18->setActiveSwitchSet(0);
				}
				for(crTableIO::DataVec::iterator siter = zhoukahuodongVec.begin(); siter!=zhoukahuodongVec.end();++siter)
				{
					huodongID = atoi((*siter)[idIdx].c_str());
					tmpitr = huodongMap->find(huodongID);

					if(tmpitr != huodongMap->end())
					{
						if(tmpitr->second->isActivation())
						{
							if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),huoRecord) >= 0)
							{
								if (ingotnumtext.valid())
								{
									ingotnumtext->setString(crArgumentParser::appItoa(tmpitr->second->getStep()));
								}
								break;
							}
						}
					}
				}
			}
			roleData->excHandle(MAKEINT64(WCH_LockData,0));

			if (huodongID >0 && !huoRecord.empty())
			{
				std::vector <int> awardVec;
				crArgumentParser::appAtoVec(huoRecord[awardIdx],awardVec);
				if(awardVec.size() < 7) return;
				for (unsigned char i=0;i<7;++i) 
				{
					inputnum[i]->setString(crArgumentParser::appItoa(awardVec[i]));
				}
				time_t nowtime = time(0);
				/*if (zhoukaendtime >= nowtime)
				{*/
					/*int endday = (int)((zhoukaendtime + 28800)/c_24hours);
					int curday = (int)((nowtime + 28800)/c_24hours);
					int dayIdx = 7 - (endday - curday);*/
					data->getParam(WCHDATA_JXJZhouKaGetDayIdx,param);
					int dayIdx = *(int *)param;
					if (dayIdx >= 0 && dayIdx < 7 && iconbtn[dayIdx].valid() && seqQ[dayIdx].valid() && lingquBtn.valid())
					{
						if (((zhoukagetstatus >> dayIdx) & 1) == 0)
						{
							iconbtn[dayIdx]->setEnable(true);
							seqQ[dayIdx]->setVisiable(true);
							lingquBtn->setEnable(true);
						}
					}
				/*}*/
				CRCore::crVector2i opendayVec2i;
				crArgumentParser::appAtoVec(huoRecord[opendayIdx],opendayVec2i);
				time_t endtime = 0;
				if (opendayVec2i[0] != 0)
				{
					typedef std::vector<int> Vec5i;
					Vec5i startTimeVec5i;
					std::string dateStr = crGlobalHandle::gData()->gStartDate();
					crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
					startTimeVec5i.resize(5,0);
					struct  tm  nowTm = *localtime(&nowtime);
					struct tm  startTm;
					memset(&startTm,0,sizeof (startTm));
					startTm.tm_year = startTimeVec5i[0] - 1900;
					startTm.tm_mon = startTimeVec5i[1] - 1;
					startTm.tm_mday = startTimeVec5i[2];
					startTm.tm_hour = startTimeVec5i[3];
					startTm.tm_min = startTimeVec5i[4];
					startTm.tm_isdst = nowTm.tm_isdst;
					time_t startTime_t = mktime(&startTm);
					if (opendayVec2i[1] != 0)
					{
						endtime = startTime_t + opendayVec2i[1] * c_24hours;
					}
					else
					{
						endtime = startTime_t + opendayVec2i[0] * c_24hours;
					}
				}
				else
				{
					CRCore::crVector3i endtimeVec;
					crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
					struct tm tm_endtime;
					memset(&tm_endtime,0,sizeof(tm_endtime));
					tm_endtime.tm_year = endtimeVec[0] - 1900;
					tm_endtime.tm_mon = endtimeVec[1] - 1;
					tm_endtime.tm_mday = endtimeVec[2];
					tm_endtime.tm_hour = 23;
					tm_endtime.tm_min = 59;
					tm_endtime.tm_sec = 59;
					endtime = mktime(&tm_endtime);
				}
				time_t remainingtime = endtime - nowtime;
				int _hour = remainingtime /3600;
				int _min = (remainingtime  - 3600*_hour)/60;
				int _sec = (remainingtime  - 3600*_hour -60*_min);
				std::string remaining;
				if (_sec >= 0)
				{
					remaining =  crArgumentParser::appItoa(_hour) 
						+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
				}
				else
				{
					remaining = "0:00:00";
				}
				if (remainingtimetext.valid())
				{
					remainingtimetext->clearString();
					remainingtimetext->setString(remaining);
				}
			}
			
		}	

	}
}


/////////////////////////////////////////
//
//crJXJRecvUpdateZhouKaStatusMethod
//
/////////////////////////////////////////
crJXJRecvUpdateZhouKaStatusMethod::crJXJRecvUpdateZhouKaStatusMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{

}
crJXJRecvUpdateZhouKaStatusMethod::crJXJRecvUpdateZhouKaStatusMethod( const crJXJRecvUpdateZhouKaStatusMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{
}
void crJXJRecvUpdateZhouKaStatusMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void crJXJRecvUpdateZhouKaStatusMethod::addParam( int i, const std::string& str )
{
	
}

void crJXJRecvUpdateZhouKaStatusMethod::operator()( crHandle &handle )
{
	if(m_netType == GameServer)
	{
		int playerid = m_this->getPlayerID();		
		ref_ptr<crData> data = m_this->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJZhouKaEndTime,param);
		time_t zhoukaendtime = *(_crInt64 *)param;
		data->getParam(WCHDATA_JXJZhouKaGetStatus,param);
		unsigned char zhoukastatus = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJGainedZhouKaID,param);
		int zhoukaID = *(int *)param;
		data->getParam(WCHDATA_JXJZhouKaGetDayIdx,param);
		int zhoukaIdx = *(int *)param;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(13);
			stream->_writeInt64(zhoukaendtime);
			stream->_writeUChar(zhoukastatus);	
			stream->_writeInt(zhoukaID);
			stream->_writeInt(zhoukaIdx);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvUpdateZhouKaStatus,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if (m_netType == GameClient_Game)
	{		
		time_t zhoukaendtime = m_stream->_readInt64();
		unsigned char zhoukastatus = m_stream->_readUChar();
		int zhoukaID = m_stream->_readInt();
		int zhoukaIdx = m_stream->_readInt();
		ref_ptr<crData> data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->inputParam(WCHDATA_JXJZhouKaEndTime,&zhoukaendtime);
		data->inputParam(WCHDATA_JXJZhouKaGetStatus,&zhoukastatus);
		data->inputParam(WCHDATA_JXJGainedZhouKaID,&zhoukaID);
		data->inputParam(WCHDATA_JXJZhouKaGetDayIdx,&zhoukaIdx);
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJZhouKaGetAwardsMethod
 
**************************************************************************/

crJXJZhouKaGetAwardsMethod::crJXJZhouKaGetAwardsMethod()
{

}

crJXJZhouKaGetAwardsMethod::crJXJZhouKaGetAwardsMethod( const crJXJZhouKaGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{

}

void crJXJZhouKaGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJZhouKaGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn =str;
		break;
	case 3:
		m_input = str;
		break;
	default:
		break;
	}

}

void crJXJZhouKaGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		bool valid = false;
		void *param;	
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crData> data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJGainedZhouKaID,param);
		int zhoukaID = *(int *)param;	
		if(zhoukaID > 0)
		{			
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvJXJZhouKaGetAwards,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
			valid = true;	
		}
			
		if (!valid)
		{
			do 
			{	
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;
				if (false == huodongTab.valid()) break;
				int failedInfoIdx = huodongTab->getTitleIndex("失败提示");
				int IDIdx = huodongTab->getTitleIndex("id");
				if (-1 == failedInfoIdx || -1 == IDIdx) break;

				if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(zhoukaID),record) < 0) break;

				ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
				if (infocanvas.valid() == false) break;
				ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_information));
				ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(infocanvas->getWidget(m_okbtn));
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content",1);
				v_i.clear();
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				cfg_script.Add("Text",record[failedInfoIdx]);
				cfg_script.Pop();

				cfg_script.Pop();

				if(information.valid())information->setHypertext(cfg_script);
				if(okbtn.valid())okbtn->setEnable(true);
				crData *dlgData = infocanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				crFilterRenderManager::getInstance()->doModal(infocanvas.get());
			} while (0);
		}
		
	}
}


/************************************************************************/
/*    crJXJRecvZhouKaGetAwardsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvZhouKaGetAwardsMethod::crJXJRecvZhouKaGetAwardsMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvZhouKaGetAwardsMethod::crJXJRecvZhouKaGetAwardsMethod( const crJXJRecvZhouKaGetAwardsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvZhouKaGetAwardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvZhouKaGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvZhouKaGetAwardsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int zhoukaID = 0;
			char logRet = 0;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			int playerid = m_this->getPlayerID();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				ref_ptr<crData> data = m_this->getDataClass();
				void *param;
				data->getParam(WCHDATA_JXJZhouKaEndTime,param);
				time_t zhoukaendtime = *(_crInt64 *)param;
				data->getParam(WCHDATA_JXJZhouKaGetStatus,param);
				unsigned char zhoukastatus = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJGainedZhouKaID,param);
				int zhoukaID = *(int *)param;
				int _status = -1;
				time_t nowtime = time(0);
				if (zhoukaendtime >= nowtime)
				{
					int endday = (int)((zhoukaendtime + 28800)/c_24hours);
					int curday = (int)((nowtime + 28800)/c_24hours);
					int dayIdx = 7 - (endday - curday);
					if (dayIdx >= 0 && dayIdx < 7)
					{
						_status = (zhoukastatus >> dayIdx) & 1;
					}
					if (_status == 1)
					{
						logRet = 2; //已领取
					}
					else if(_status == 0)
					{
						ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
						crTableIO::StrVec record;
						if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(zhoukaID),record) >= 0)
						{
							int awardIdx = huodongTab->getTitleIndex("奖励1");
							std::vector<int> lijinVec;
							crArgumentParser::appAtoVec(record[awardIdx],lijinVec);
							if (lijinVec.size() >= 7)
							{

								ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
								reward_info->setGiftgold(lijinVec[dayIdx]);
								reward_info->setType(GP_RewardXiaozhouka);
								m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward_info.get(),NULL));
								zhoukastatus = zhoukastatus | (1 << dayIdx);
								data->excHandle(MAKEINT64(WCH_LockData,1));
								data->inputParam(WCHDATA_JXJZhouKaGetStatus,&zhoukastatus);
								data->excHandle(MAKEINT64(WCH_LockData,0));
								m_this->doEvent(WCH_JXJRecvUpdateZhouKaStatus,MAKEINT64(NULL,m_netType));
								logRet = 1;
							}
							
						}
					}
				}

				char gbuf[256];
				sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),zhoukaID,logRet);
				std::string logdata(gbuf);
				GameLogData gamelog(Log_HuodongGet,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJSetBillboardPageMethod
//
///////////////////////////////////////
crJXJSetBillboardPageMethod::crJXJSetBillboardPageMethod():
	m_this(NULL)
{

}

crJXJSetBillboardPageMethod::crJXJSetBillboardPageMethod( const crJXJSetBillboardPageMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_billboardraido(handle.m_billboardraido),
	m_num(handle.m_num)
{

}

void crJXJSetBillboardPageMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJSetBillboardPageMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_billboardraido = str;
		break;
	case 2:
		m_num = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void crJXJSetBillboardPageMethod::operator()( crHandle &handle )
{
	
	CRCore::ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		ref_ptr<crRadioGroupWidgetNode> BillboardRaido = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getChildNode(m_billboardraido));
		if (BillboardRaido.valid())
		{	
			BillboardRaido->select(m_num);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckMyApplyGroupIDSetMethod
//
/////////////////////////////////////////
crJXJCheckMyApplyGroupIDSetMethod::crJXJCheckMyApplyGroupIDSetMethod():
	m_type(0)
{
}

crJXJCheckMyApplyGroupIDSetMethod::crJXJCheckMyApplyGroupIDSetMethod( const crJXJCheckMyApplyGroupIDSetMethod & handle ):
	crMethod(handle),
	m_type(handle.m_type)
{
}

void crJXJCheckMyApplyGroupIDSetMethod::inputParam( int i, void *param )
{
}

void crJXJCheckMyApplyGroupIDSetMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJCheckMyApplyGroupIDSetMethod::operator()( crHandle &handle )
{
	//
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && playerGameData->getDataClass())
	{
		crData *playerData = playerGameData->getDataClass();
		if(playerData)
		{
			void *param;
			playerData->getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *(int *)param;
			if (mygroupid <= 0)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if (netConductor)
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJCheckMyApplyGroupIDSet,NULL);
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
	
}


/////////////////////////////////////////
//
//crJXJRecvCheckMyApplyGroupIDSetMethod
//
/////////////////////////////////////////
crJXJRecvCheckMyApplyGroupIDSetMethod::crJXJRecvCheckMyApplyGroupIDSetMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{

}
crJXJRecvCheckMyApplyGroupIDSetMethod::crJXJRecvCheckMyApplyGroupIDSetMethod( const crJXJRecvCheckMyApplyGroupIDSetMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{
}
void crJXJRecvCheckMyApplyGroupIDSetMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void crJXJRecvCheckMyApplyGroupIDSetMethod::addParam( int i, const std::string& str )
{

}

void crJXJRecvCheckMyApplyGroupIDSetMethod::operator()( crHandle &handle )
{
	if(m_netType == GameServer)
	{
		int playerid = m_this->getPlayerID();		
		ref_ptr<crData> data = m_this->getDataClass();
		void *param;
		if (data.valid())
		{
			data->getParam(WCHDATA_JXJMyApplyGroupIDSet,param);
			MyGroupApplySet* myGroupApplySet = (MyGroupApplySet *)param;
			if (myGroupApplySet)
			{
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator itr = consortiamap->end();
				MyGroupApplySet::iterator mitr = myGroupApplySet->begin();
				crData *groupdata = NULL;
				bool changed;
				for (;mitr!=myGroupApplySet->end();)
				{
					itr = consortiamap->find(*mitr);
					if (itr != consortiamap->end())
					{
						if (itr->second.valid())
						{
							groupdata = itr->second->getDataClass();
							if (groupdata)
							{
								groupdata->getParam(WCHDATA_JXJConsortiaApplyVec,param);
								ConsortiaApplyVec *applyvec = (ConsortiaApplyVec *)param;
								changed = true;
								for (ConsortiaApplyVec::iterator citr = applyvec->begin();
									citr != applyvec->end(); ++citr)
								{
									if ((*citr)->getPlayerID() == playerid)
									{
										changed = false;
										break;
									}
								}
								if (changed)
								{
									mitr = myGroupApplySet->erase(mitr);
									continue;
								}	
							}
						}
					}
					mitr++;
				}
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJResurrectionCountMethod
//
/////////////////////////////////////////
crJXJFubenResurrectionCountMethod::crJXJFubenResurrectionCountMethod()
{

}
crJXJFubenResurrectionCountMethod::crJXJFubenResurrectionCountMethod(const crJXJFubenResurrectionCountMethod& handle):
	crMethod(handle)	
{
}
void crJXJFubenResurrectionCountMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJFubenResurrectionCountMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJFubenResurrectionCountMethod::operator()(crHandle &handle)
{
	int _case = 0;
	if (m_this)
	{
		void *param;
		ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>
			(sceneServer->getNetDataManager()->getPlayerData(m_this->getRoomHost()));
		if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
		{
			crData *data =  playerData->getPlayerGameData()->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			unsigned char type = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short fubenid = *(unsigned short *)param;	
			data->getParam(WCHDATA_JXJFubenReliveCount,param);
			unsigned char count = (unsigned char )param;
			data->excHandle(MAKEINT64(WCH_LockData,0));
			crTableIO::StrVec record;
			if(type == FT_Fuben && fubenid>0 && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
			{
				int index = fubentab->getTitleIndex("允许补兵次数");
				unsigned char totalcount = atoi(record[index].c_str());
				_case = totalcount - count;
			}
		}
	}		
	handle.outputParam(0,&_case);
}


/************************************************************************/
/*    crJXJClickLinkIntoFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJClickLinkIntoFuBenMethod::crJXJClickLinkIntoFuBenMethod():
	m_this(NULL)
{

}

JXJ::crJXJClickLinkIntoFuBenMethod::crJXJClickLinkIntoFuBenMethod( const crJXJClickLinkIntoFuBenMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJClickLinkIntoFuBenMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJClickLinkIntoFuBenMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJClickLinkIntoFuBenMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		void *param;
		crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();

		data->getParam(WCHDATA_JXJInviteChengChiID,param);
		unsigned short chengchiID = *(unsigned short *)param;

		int fubenId = (int)chengchiID;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeInt(fubenId);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupIntoFuBen,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
	
}

/////////////////////////////////////////
//
//crJXJJuntuanCreateRoomReturnMethod
//
/////////////////////////////////////////
crJXJJuntuanCreateRoomReturnMethod::crJXJJuntuanCreateRoomReturnMethod(){}
crJXJJuntuanCreateRoomReturnMethod::crJXJJuntuanCreateRoomReturnMethod(const crJXJJuntuanCreateRoomReturnMethod& handle):
	crMethod(handle)
{
}
void crJXJJuntuanCreateRoomReturnMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJJuntuanCreateRoomReturnMethod::addParam(int i, const std::string& str)
{
}
void crJXJJuntuanCreateRoomReturnMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		bool success = m_stream->_readBool();
		int roomid = m_stream->_readInt();
		int sceneid = m_stream->_readInt();
		_crInt32 battleid = m_stream->_readInt();
		int groupid = m_stream->_readInt();
		int playerid = m_stream->_readInt();
		unsigned short fubenid = HIINT32(battleid);
		_crInt64 battleroomid = MAKEINT64(battleid,roomid);
		void *param;
		unsigned char suc = JXJGroupStartFuBenRetVal_failed_unkwon;
		crData *data = m_this->getDataClass();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(!gameServer) return;

		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = (ConsortiaMap *)param;
		ConsortiaMap::iterator itr = consortiamap->find(groupid);
		ref_ptr<crData> groupData;
		if (itr != consortiamap->end())
		{
			groupData = itr->second->getDataClass();
		}
		if (groupData.valid())
		{
			groupData->excHandle(MAKEINT64(WCH_LockData,1));
			groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
			StartedFubenMap * startedFubenMap  = (StartedFubenMap *)param;
			if (success)
			{
				if (startedFubenMap->find(fubenid) == startedFubenMap->end())
				{
					ConsortiaPlayerReliveCount countmap;
					startedFubenMap->insert(std::make_pair(fubenid,std::make_pair(roomid,countmap)));
				}
				else
				{
					(*startedFubenMap)[fubenid].first = roomid;
				}
				if (playerData.valid() && playerData->getPlayerGameData())
				{
					crPlayerGameData *pdata = playerData->getPlayerGameData();
					pdata->doEvent(WCH_JXJRecvQueryMyGroupInfo,MAKEINT64(NULL,GameServer));
					groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap *consortiamembermap = (ConsortiaMemberMap *)param;
					int olplayerid = 0;
					ref_ptr<crGameServerPlayerData> OlplayerData ;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(12);
					stream->_writeInt(sceneid);
					stream->_writeInt64(battleroomid);	
					crPlayerDataEventPacket packet;
					for (ConsortiaMemberMap::iterator itr = consortiamembermap->begin();itr != consortiamembermap->end();++itr)
					{
						olplayerid = itr->second->getPlayerID();
						OlplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(olplayerid));
						if(OlplayerData.valid() && OlplayerData->getPlayerGameData() && OlplayerData->getPlayerGameData()->getMainRole())
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,olplayerid,WCH_JXJClientStartJuntuanFubenNotification,stream.get());
							gameServer->getNetManager()->sendPacket(OlplayerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
				suc = JXJGroupStartFuBenRetVal_suc;
			}
			else	//创建失败
			{
				suc = JXJGroupStartFuBenRetVal_failed_unkwon;
				if (startedFubenMap->find(fubenid) != startedFubenMap->end())
				{
					startedFubenMap->erase(fubenid);
				}
				//ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				//crTableIO::StrVec strVec;
				//int zhenfazhiIdx = fubenTab->getTitleIndex("征伐值");
				//if (fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),strVec) >= 0)
				//{ 
				//	int zhenfaCost = atoi(strVec[zhenfazhiIdx].c_str());
				//	groupData->getParam(WCHDATA_JXJConquest,param);
				//	unsigned char conquest = *(unsigned char *)param;
				//	conquest += zhenfaCost;
				//	groupData->inputParam(WCHDATA_JXJConquest,&conquest);	//返还讨伐点
				//}
				//if (playerData.valid() && playerData->getPlayerGameData())
				//{
				//	crPlayerGameData *pdata = playerData->getPlayerGameData();
				//	pdata->doEvent(WCH_JXJRecvQueryMyGroupInfo,MAKEINT64(NULL,GameServer));
				//}
			}
			groupData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		//////////////////////////////////////////////////////////////////////////
		if (playerData.valid())
		{
			ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
			stream1->createBuf(5);
			stream1->_writeUChar(suc);
			stream1->_writeInt(fubenid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupStartFuBen,stream1.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	
	}
}


/************************************************************************/
/*    crJXJClickIntoJunTuanMultiFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJClickIntoJunTuanMultiFuBenMethod::crJXJClickIntoJunTuanMultiFuBenMethod()
{

}

JXJ::crJXJClickIntoJunTuanMultiFuBenMethod::crJXJClickIntoJunTuanMultiFuBenMethod( const crJXJClickIntoJunTuanMultiFuBenMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{

}

void JXJ::crJXJClickIntoJunTuanMultiFuBenMethod::inputParam( int i, void *param )
{
}

void JXJ::crJXJClickIntoJunTuanMultiFuBenMethod::addParam( int i, const std::string& str )
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	}
}

void JXJ::crJXJClickIntoJunTuanMultiFuBenMethod::operator()( crHandle &handle )
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
	if (mygroupcanvas.valid() && playerGameData && playerGameData->getDataClass())
	{
		crData * playerData = playerGameData->getDataClass();
		crData *canvasdata = mygroupcanvas->getDataClass();
		if (canvasdata)
		{
			CRCore::ScopedLock<crData> lock(*canvasdata);

			playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short fubenid = *(unsigned short *)param;

			ref_ptr<crTableIO>funbenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec record;
			if(fubenid>0 && funbenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
			{
				int sceneidIdx = funbenTab->getTitleIndex("sceneid");
				int sceneid = atoi(record[sceneidIdx].c_str());

				canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
				StartedFubenMap * startedFubenMap = (StartedFubenMap *)param;
				StartedFubenMap::iterator itr = startedFubenMap->find(fubenid);

				if (itr!=startedFubenMap->end())
				{
					int roomid = itr->second.first;
					_crInt64 battleroomid = MAKEINT64(NULL, roomid);
					playerData->inputParam(WCHDATA_JXJBattleRoomID, &battleroomid);

					crLoginScenePacket packet;
					crLoginScenePacket::buildRequestPacket(packet, sceneid, WCH_JXJAgreeEnterBattle);
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					netConductor->getNetManager()->sendPacket("all", packet);
				}
			}

		}
	}
}


/**************************************************************************

METHOD: crJXJClientStartJuntuanFubenNotificationMethod
 
**************************************************************************/

JXJ::crJXJClientStartJuntuanFubenNotificationMethod::crJXJClientStartJuntuanFubenNotificationMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJClientStartJuntuanFubenNotificationMethod::crJXJClientStartJuntuanFubenNotificationMethod( const crJXJClientStartJuntuanFubenNotificationMethod& handle ):
	crMethod(handle),
	m_noticeCanvas(handle.m_noticeCanvas),
	m_textWidget(handle.m_textWidget),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_netType(handle.m_netType),
	m_this(NULL),
	m_stream(NULL)
{

}

void JXJ::crJXJClientStartJuntuanFubenNotificationMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJClientStartJuntuanFubenNotificationMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_textWidget = str;
		break;
	case 2:
		m_mygroupcanvas = str;
		break;
	}
}

void JXJ::crJXJClientStartJuntuanFubenNotificationMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			int sceneid = m_stream->_readInt();
			_crInt64 battleroomid = m_stream->_readInt64();
			_crInt32 battleid = LOINT64(battleroomid);
			int roomid = HIINT64(battleroomid);
			unsigned short fubenid = HIINT32(battleid);
			//void *param;
			crData *data = m_this->getDataClass();
			ref_ptr<crTableIO>funbenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec record;
			if(/*type == FT_Chengchi && */fubenid>0 && funbenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
			{
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
				if(canvas.valid())
				{
					if (canvas->getVisiable() == false)
					{
						unsigned char type = FT_Fuben;
						data->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
						data->inputParam(WCHDATA_JXJFubenOrChengchiID,&fubenid);
						int nameid = funbenTab->getTitleIndex("name");

						int texid = 5012;//"是否进入城战%s"
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 2;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						noticeHandle->inputParam(WCHDATA_NoticeParam1,&(record[nameid]));
						std::string output;
						noticeHandle->inputParam(WCHDATA_NoticeOutPut,&output);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);

						data->inputParam(WCHDATA_JXJBattleRoomID, &battleroomid);
						ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
						crData *canvasData = canvas->getDataClass();
						canvasData->inputParam(WCHDATA_JXJEnterSceneID,&sceneid);
// 						unsigned char index = 1; //值为1时表示当前提示窗口是军团副本开启提示框，而非国战进入提示框
// 						canvasData->inputParam(WCHDATA_JXJCanvasIndex, &index);
						crStaticTextWidgetNode *textWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_textWidget));
						if(textWidget)
						{
							textWidget->setString(output);
						}
						crFilterRenderManager::getInstance()->doModal(canvas.get());
					}
				}
				ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
				if (mygroupcanvas.valid() && mygroupcanvas->getDataClass())
				{
					void *param = NULL;
					crData *canvasdata = mygroupcanvas->getDataClass();
					canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
					StartedFubenMap * startedFubenMap = (StartedFubenMap *)param;
					if (startedFubenMap->find(fubenid) == startedFubenMap->end())
					{
						ConsortiaPlayerReliveCount countmap;
						startedFubenMap->insert(std::make_pair(fubenid,std::make_pair(roomid,countmap)));
					}
					else
					{
						(*startedFubenMap)[fubenid].first = roomid;
					}
				}

			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJAppointedSecondGroupLeaderMethod
//
/////////////////////////////////////////
crJXJAppointedSecondGroupLeaderMethod::crJXJAppointedSecondGroupLeaderMethod()
{
}

crJXJAppointedSecondGroupLeaderMethod::crJXJAppointedSecondGroupLeaderMethod(const crJXJAppointedSecondGroupLeaderMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_groupList(handle.m_groupList),
	m_type(handle.m_type)
{
}

void crJXJAppointedSecondGroupLeaderMethod::inputParam(int i, void *param)
{
}

void crJXJAppointedSecondGroupLeaderMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupList = str;
		break;
	case 2:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJAppointedSecondGroupLeaderMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			ref_ptr<crTableWidgetNode>  memberList = dynamic_cast<crTableWidgetNode *> (canvas->getWidget(m_groupList));
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata && memberList.valid())
			{
				void * param;
				PermissionCheck checkPermission;
				CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
				stream->createBuf(5);				
				int curId = memberList->getSelectData();
				std::wstring groupPosName ;
				bool z = memberList->getData(1,memberList->getRowCount()-1,groupPosName);
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;
				int myGroupPos = 0;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();			
				if (curId != playerGameData->getPlayerID())
				{
					if (( groupPosName != L"副军团长" && groupPosName != L"军团长") || (checkPermission(myGroupPos,JXJGroup_ChangeLeader)))
					{
						stream->_writeUChar(m_type);
						stream->_writeInt(curId);
						CRNetApp::crPlayerDataEventPacket packet;
						CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAppointedSecondGroupLeader, stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(5002,NULL));
					}
				}
				else
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4062,NULL));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvAppointedSecondGroupLeaderMethod
//
/////////////////////////////////////////
crJXJRecvAppointedSecondGroupLeaderMethod::crJXJRecvAppointedSecondGroupLeaderMethod():
	m_netType(GameClient_Game){}
crJXJRecvAppointedSecondGroupLeaderMethod::crJXJRecvAppointedSecondGroupLeaderMethod(const crJXJRecvAppointedSecondGroupLeaderMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvAppointedSecondGroupLeaderMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvAppointedSecondGroupLeaderMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvAppointedSecondGroupLeaderMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				unsigned char _type = m_stream->_readUChar();
				int Appointplayerid = m_stream->_readInt();
				unsigned int permission = JXJGroup_ChangeLeader;
				
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				crData *groupdata = NULL;
				ref_ptr<crJXJConsortia> consortia;
				ConsortiaMap::iterator cmitr = consortiamap->find(mygroupid);
				if (cmitr != consortiamap->end())
				{
					consortia = cmitr->second.get();
				}
				if(consortia.valid())
				{
					groupdata = consortia->getDataClass();
					int mygrouppos = consortia->getUserGroupPos(playerid);
					int Appointgrouppos = consortia->getUserGroupPos(Appointplayerid);
					if (groupdata && consortia->checkPermission(playerid,permission) && mygrouppos == JXJGroup_FirstLeader)
					{
						if ( Appointgrouppos != JXJGroup_FirstLeader)
						{		
							groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
							ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
							groupdata->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
							GroupLeaderVec *groupLeaderVec = (GroupLeaderVec *)param;
							ConsortiaMemberMap::iterator itr ;
							itr = membermap->find(Appointplayerid);
							if (itr != membermap->end() && itr->second.valid())
							{
								if (_type == 1 && Appointgrouppos != JXJGroup_SecondLeader && groupLeaderVec->size() < 3)
								{
									itr->second->setGroupPos(JXJGroup_SecondLeader);
									std::string groupnotice = itr->second->getName() + "成为副军团长";
									CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", groupnotice,0,0,mygroupid);
									gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
									crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
									gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
								}
								else if(_type == 0 && Appointgrouppos == JXJGroup_SecondLeader)
								{
									itr->second->setGroupPos(JXJGroup_Normal);
								}

								consortia->updateLeaderVec();
								bool modify = true;
								groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);

								consortia->updateLeaderVec();
								consortia->updateToRank(gsBrainData);
								if (playerid != Appointplayerid)
								{
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAppointedSecondGroupLeader,NULL);
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

									crPlayerDataEventPacket::buildReplyPacket(packet,Appointplayerid,WCH_JXJRecvAppointedSecondGroupLeader,NULL);
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
								}
							}
						}
					}
				}
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(mygroupid,Appointplayerid)) + "|" + crArgumentParser::appItoa(_type);
				GameLogData gamelog(Log_ChangeGroupSecondLeader,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{	
			m_this->doEvent(WCH_JXJQueryGroupMemberList);
		}
	}
}

/////////////////////////////////////////
//
//crJXJGroupLevelUpMethod
//
/////////////////////////////////////////
crJXJGroupLevelUpMethod::crJXJGroupLevelUpMethod()
{
}

crJXJGroupLevelUpMethod::crJXJGroupLevelUpMethod(const crJXJGroupLevelUpMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{
}

void crJXJGroupLevelUpMethod::inputParam(int i, void *param)
{
}

void crJXJGroupLevelUpMethod::addParam(int i, const std::string& str)
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

void crJXJGroupLevelUpMethod::operator()(crHandle &handle)
{
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(netConductor)
	{	
		CRNetApp::crPlayerDataEventPacket packet;
		CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupLevelUp,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}


/////////////////////////////////////////
//
//crJXJRecvGroupLevelUpMethod
//
/////////////////////////////////////////
crJXJRecvGroupLevelUpMethod::crJXJRecvGroupLevelUpMethod():
	m_netType(GameClient_Game){}
crJXJRecvGroupLevelUpMethod::crJXJRecvGroupLevelUpMethod(const crJXJRecvGroupLevelUpMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGroupLevelUpMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGroupLevelUpMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvGroupLevelUpMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
				ref_ptr<crData> consortiaData = itr->second->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

				consortiaData->getParam(WCHDATA_JXJConsortialv,param);
				int groupLevel = *(unsigned char *)param;
				consortiaData->getParam(WCHDATA_JXJConsortiaFunds,param);
				int funds = *(int *)param;

				ref_ptr<crTableIO>  groupUpdateTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);
				crTableIO::StrVec record;
				unsigned char maxGroupLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaMaxLevel,0).c_str());
				unsigned char ret = 0;
				if(groupLevel < maxGroupLevel)
				{
					if (groupUpdateTab.valid() && groupUpdateTab->queryOneRecord(0,crArgumentParser::appItoa(groupLevel),record) >= 0)
					{
						int upLevelFundsIdx = groupUpdateTab->getTitleIndex("升级资金");
						int needfunds = atoi(record[upLevelFundsIdx].c_str());
						if (funds >= needfunds)
						{
							++ groupLevel ;
							consortiaData->inputParam(WCHDATA_JXJConsortialv,&groupLevel);
							funds = funds - needfunds;
							consortiaData->inputParam(WCHDATA_JXJConsortiaFunds,&funds);
							m_this->doEvent(WCH_JXJRecvQueryMyGroupInfo,MAKEINT64(NULL,GameServer));
						}
						else
						{
							ret = 1;	//资金不足
						}
					}
					else
					{
						ret = 2;		//
					}
				}
				else
				{
					ret = 3;			//已经满级
				}
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(1);
				streamBuf->_writeUChar(ret);
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupLevelUp,streamBuf.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

			}
		}
		else if (m_netType = GameClient_Game)
		{
			if (m_stream.valid())
			{
				unsigned char ret = m_stream->_readUChar();	
				int msgid = 0;
				if (ret == 1)
				{
					msgid = 5003;
				}
				else if(ret==3)
				{
					msgid = 5004;
				}

				if (ret == 1 || ret == 3)
				{
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
			}
		}
	}
}



/************************************************************************/
/*    crJXJGroupEnfeoffChengChiMethod                                                                  */
/************************************************************************/
JXJ::crJXJGroupEnfeoffChengChiMethod::crJXJGroupEnfeoffChengChiMethod():
	m_this(NULL)
{

}

JXJ::crJXJGroupEnfeoffChengChiMethod::crJXJGroupEnfeoffChengChiMethod( const crJXJGroupEnfeoffChengChiMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_table(handle.m_table),
	m_type(handle.m_type)
{
}

void JXJ::crJXJGroupEnfeoffChengChiMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJGroupEnfeoffChengChiMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_table = str;
		break;
	case 2:
		m_type = (unsigned char )atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJGroupEnfeoffChengChiMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (m_this && netConductor )
	{
		do 
		{
			ref_ptr<crCanvasNode> groupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (groupcanvas.valid() == false) break;
			ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
			if(!canvas.valid()) break;
			crData * canvasData = groupcanvas->getDataClass();
			if (!canvasData) break;
			void *param;
			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId,param);
			int chengchiid = *(int *)param;
			canvasData->getParam(WCHDATA_JXJAllocateCantonVec,param);
			MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
			if(groupCantonVec->empty()) break;
			int cantonid = (*groupCantonVec)[0];
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(13);
			stream->_writeUChar(m_type);
			stream->_writeInt(chengchiid);
			stream->_writeInt(cantonid); 
			if (m_type)
			{
				ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_table));
				if (tableWidget .valid() == false) break;
				stream->_writeInt(tableWidget->getSelectData()); 
			}
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_RecvGroupEnfeoffChengChi,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
			
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
		} while (0);
	}
}

/************************************************************************/
/*    crJXJRecvGroupEnfeoffChengChiMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvGroupEnfeoffChengChiMethod::crJXJRecvGroupEnfeoffChengChiMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvGroupEnfeoffChengChiMethod::crJXJRecvGroupEnfeoffChengChiMethod( const crJXJRecvGroupEnfeoffChengChiMethod& handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvGroupEnfeoffChengChiMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvGroupEnfeoffChengChiMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvGroupEnfeoffChengChiMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = 0;
			unsigned char op = m_stream->_readUChar();
			int chengChiId = m_stream->_readInt();
			int cantonId = m_stream->_readInt();
			if (op && !m_stream->eof())
			{
				playerid = m_stream->_readInt();
			}
			void *param;
			unsigned char success = 0;
			int myplayerid = m_this->getPlayerID();

			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> myplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myplayerid));
			if (myplayerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				unsigned char maxNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCantonChengChiAllocateMaxNum,viplv).c_str());

				if (mygroupid > 0 && shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;

						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if (cantonId && shili && shili->getDataClass())
						{
							crData *shilidata = shili->getDataClass();	
							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
							shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
							CantonMap * cantonMap = (CantonMap *)param;
							CantonMap::iterator it = cantonMap->find(cantonId);
							if (it != cantonMap->end())
							{
								if (it->second.valid())
								{
									unsigned char suc = JXJGroupAllocateChengChiRet_failed_unkwon;
									if (it->second->getConsortiaId() == mygroupid)
									{
										if (myplayerid != playerid)
										{
											ref_ptr<crJXJConsortia> consortia;
											ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
											if (cmItr != consortiamap->end() && cmItr->second.valid())
											{
												consortia = cmItr->second;
											}
											if(consortia.valid() && consortia->getUserGroupPos(myplayerid) == JXJGroup_FirstLeader)
											{
												crData * groupData = consortia->getDataClass();
												if (groupData)
												{
													groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
													FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
													FenFengedChengChiMap::iterator ffccmitr = fenFengedChengChiMap->find(chengChiId);
													if (op)
													{
														if (ffccmitr != fenFengedChengChiMap->end())
														{
															suc = JXJGroupAllocateChengChiRet_failed_hasOwn;
														}
														else
														{
															if(consortia->isUserNewer(playerid) == false)
															{
																bool isfenfenged =false;
																for (ffccmitr = fenFengedChengChiMap->begin();ffccmitr != fenFengedChengChiMap->end();++ffccmitr)
																{
																	if(ffccmitr->second == playerid)
																	{
																		isfenfenged = true;
																		break;
																	}
																}
																if (isfenfenged)
																{
																	suc = JXJGroupAllocateChengChiRet_failed_full;
																}
																else
																{
																	fenFengedChengChiMap->insert(std::make_pair(chengChiId,playerid));
																	suc = JXJGroupAllocateChengChiRet_suc;
																}
															}
														}
													}
													else
													{
														if (ffccmitr != fenFengedChengChiMap->end())
														{
															/*	if (ffccmitr->second == playerid)
															{*/
															playerid = ffccmitr->second;
															fenFengedChengChiMap->erase(ffccmitr);
															suc = JXJGroupAllocateChengChiRet_suc;
														/*	}*/
														}
													}
													bool modify = true;
													groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
													
												}

												bool modify = true;
												shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
											}
										}
										else
										{
											suc = JXJGroupAllocateChengChiRet_failed_isGuard;
										}
									}
									else
									{
										suc = JXJGroupRemoveChengChiGroupRetVal_failed_notGuard;
									}
									shilidata->getParam(WCHDATA_JXJPeopleMap,param);
									PeopleMap *peoplemap = (PeopleMap *)param;
									PeopleMap::iterator peoitr = peoplemap->find(playerid);
									std::string playername;
									if (peoitr != peoplemap->end() && peoitr->second.valid())
									{
										playername = peoitr->second->getName();
									}

									if (op && suc == JXJGroupAllocateChengChiRet_suc)
									{
										ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
										if (chengchitab.valid())
										{
											crTableIO::StrVec record;
											int chengweiIdx = chengchitab->getTitleIndex("称谓");
											int chengchiname = chengchitab->getTitleIndex("名字");
											if(chengchitab->queryOneRecord(0,crArgumentParser::appItoa(chengChiId),record)>=0)
											{
												std::string groupnotice = playername + "被任命为" + record[chengchiname] + record[chengweiIdx];
												CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", groupnotice,0,0,mygroupid);
												gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
												crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
												gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

											}
										}
									}

									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(14 + playername.length());
									stream->_writeUChar(suc);
									stream->_writeUChar(op);
									stream->_writeString(playername);
									stream->_writeInt(chengChiId);
									stream->_writeInt(mygroupid);
									
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_RecvGroupEnfeoffChengChi,stream.get());
									gameServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);

									ref_ptr<crGameServerPlayerData> playerData = 
										dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
									if (playerData.valid())
									{
										crPlayerDataEventPacket packet;
										crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_RecvGroupEnfeoffChengChi,stream.get());
										gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
									}
								}
							}
							shilidata->excHandle(MAKEINT64(WCH_LockData,0));
						}
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}

		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

					/*canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec* cantonGroupVec = (CantonGroupVec *)param;*/

					unsigned char suc = m_stream->_readUChar();
					unsigned char op = m_stream->_readUChar();
					std::string playername = m_stream->_readString();
					int chengChiId = m_stream->_readInt();
					int mygroupid = m_stream->_readInt();

					switch (suc)
					{
					case JXJGroupAllocateChengChiRet_failed_unkwon:
						break;
					case JXJGroupAllocateChengChiRet_failed_isGuard:
						break;
					case JXJGroupAllocateChengChiRet_suc:
						{

							canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
							ClientFenFengedChengChiMap * fenFengMap = (ClientFenFengedChengChiMap *)param;
			
							if (op)
							{
								// 分封
								if (fenFengMap->find(chengChiId) != fenFengMap->end())
								{
									(*fenFengMap)[chengChiId] = playername;
								}
								else
								{
									fenFengMap->insert(std::make_pair(chengChiId,playername));
								}
							}
							else
							{
								fenFengMap->erase(chengChiId);
								// 解除
							}
					
							mygroupcanvas->doEvent(WCH_UI_UpdateData);
							ref_ptr<crCanvasNode> renmingcanvas = crFilterRenderManager::getInstance()->findCanvas("Corps2_Chengchirenming");
							if (renmingcanvas.valid())
							{
								renmingcanvas->doEvent(WCH_UI_UpdateData);
							}
						}
						break;
					default:
						break;
					}
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}



/////////////////////////////////////////
//
//crJXJTaoFaCanvasUpdateMethod
//
///////////////////////////////////////
crJXJTaoFaCanvasUpdateMethod::crJXJTaoFaCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJTaoFaCanvasUpdateMethod::crJXJTaoFaCanvasUpdateMethod( const crJXJTaoFaCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_fubenname(handle.m_fubenname),
	m_radio1(handle.m_radio1),
	m_radio2(handle.m_radio2),
	m_times(handle.m_times),
	m_tuijiandengdi(handle.m_tuijiandengdi),
	m_taofadian(handle.m_taofadian),
	m_canvas(handle.m_canvas),
	m_taofaBtn(handle.m_taofaBtn),
	m_quanxiansw(handle.m_quanxiansw),
	m_jinruBtn(handle.m_jinruBtn)
{
	for (unsigned char i = 0;i<5;++i)
	{
		m_nandu[i] = handle.m_nandu[i];
		if (i<4)
		{
			m_jiangliicon[i] = handle.m_jiangliicon[i];
		}
	}
}

void crJXJTaoFaCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJTaoFaCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_fubenname = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_nandu[i-1] = str;
		break;
	case 6:
		m_radio1 = str;
		break;
	case 7:
		m_times = str;
		break;
	case 8:
		m_tuijiandengdi = str;
		break;
	case 9:
		m_radio2 = str;
		break;
	case 10:
		m_taofadian = str;
		break;
	case 11:
		m_taofaBtn = str;
		break;
	case 12:
		m_canvas = str;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
		m_jiangliicon[i-13] = str;
		break;
	case 17:
		m_quanxiansw = str;
		break;
	case 18:
		m_jinruBtn = str;
		break;
	default:
		break;
	}

}

void crJXJTaoFaCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (myPlayer && myPlayer->getDataClass() && mygroupcanvas.valid())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		ref_ptr<crStaticTextWidgetNode> nandu[5];
		ref_ptr<crImageBoxWidgetNode> image[4];
		ref_ptr<crStaticTextWidgetNode> fubenname = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_fubenname));
		ref_ptr<crStaticTextWidgetNode> tuijiandengdi = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_tuijiandengdi));
		ref_ptr<crStaticTextWidgetNode> times = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_times));
		ref_ptr<crStaticTextWidgetNode> taofadian = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_taofadian));
		ref_ptr<crButtonWidgetNode> taofaBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_taofaBtn)); 
		ref_ptr<crRadioGroupWidgetNode> radio1 = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radio1));
		ref_ptr<crRadioGroupWidgetNode> radio2 = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radio2));
		ref_ptr<crMultiSwitch> quanxiansw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_quanxiansw));
		ref_ptr<crButtonWidgetNode> jinruBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_jinruBtn));
		if(!radio1.valid()) return;
		if(!radio2.valid()) return;
		for (unsigned char i = 0;i<5;++i)
		{
			nandu[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_nandu[i]));
			if (i<4)
			{
				image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_jiangliicon[i]));
			}
		}
		for (unsigned char i = 0;i<5;++i)
		{
			if (nandu[i].valid())
			{
				std::string text("难度");
				nandu[i]->setString(text + crArgumentParser::appItoa(i+1));
			}
			if (i<4 && image[i].valid())
			{
				if (i % 2 == 0)
				{
					image[i]->setImageName("T_Icon_Task_Gifts.img");
				}
				else
				{
					image[i]->setImageName("T_Icon_Task_EXP.img");
				}
			}
		}
		if (times.valid())
		{
			times->setString(L"99");
		}

		crData *canvasdata = mygroupcanvas->getDataClass();

		if(canvasdata)
		{
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
			StartedFubenMap *startedFubenMap = (StartedFubenMap *)param;
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			canvasdata->getParam(WCHDATA_JXJHadWonGroupFuBenSet,param);
			HadWonGroupFuBenSet *hadWonGroupFuBenSet = (HadWonGroupFuBenSet *)param;
			int myGroupPos = JXJGroup_Normal;
			if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
			ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::DataVec  dataVec;
			int fubenType = 1;
			unsigned short fubenid = 0;
			unsigned char needtaofadian = 0;
			if (fubenTab.valid() &&fubenname.valid() && times.valid() && tuijiandengdi.valid())
			{
				dataVec.clear();
				unsigned char idx = 0;
				fubenTab->queryRecords(2,crArgumentParser::appItoa(fubenType),dataVec);
				int fubenidIdx = fubenTab->getTitleIndex("id");
				int fubennameIdx = fubenTab->getTitleIndex("name");
				int timesIdx = fubenTab->getTitleIndex("敌军波数");
				int tuijiandengjiIdx = fubenTab->getTitleIndex("说明");
				int taofadianIdx = fubenTab->getTitleIndex("征伐值");
				StartedFubenMap::iterator sfmitr = startedFubenMap->begin();
				if (sfmitr != startedFubenMap->end())
				{
					fubenid = sfmitr->first;
					crTableIO::DataVec::iterator dvitr = dataVec.begin();
					for (;dvitr!=dataVec.end();++dvitr,++idx)
					{
						if(fubenid == atoi((*dvitr)[fubenidIdx].c_str()))
						{
							fubenname->setString((*dvitr)[fubennameIdx]);
							times->setString((*dvitr)[timesIdx]);
							tuijiandengdi->setString((*dvitr)[tuijiandengjiIdx]);
							break;
						}
					}
					radio1->select(idx);
				}
				else
				{
					if (myGroupPos == JXJGroup_FirstLeader || myGroupPos == JXJGroup_SecondLeader)
					{
						idx = radio1->getSelect();
					}
					else
					{
						idx = 0;
						radio1->select(idx);
					}
					if (idx < dataVec.size())
					{
						fubenname->setString(dataVec[idx][fubennameIdx]);
						times->setString(dataVec[idx][timesIdx]);
						tuijiandengdi->setString(dataVec[idx][tuijiandengjiIdx]);
						fubenid = atoi(dataVec[idx][fubenidIdx].c_str());
						needtaofadian = atoi(dataVec[idx][taofadianIdx].c_str());
					}
				}
			}
			unsigned char curtaofadian = 0;
			canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
			crJXJGroupInfo *groupinfo = (crJXJGroupInfo *)param;
			if (taofadian.valid() && groupinfo)
			{
				curtaofadian = groupinfo->getConquest();
				unsigned char maxtaofadian = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxTaoFaDian).c_str());
				taofadian->setString(crArgumentParser::appItoa(curtaofadian) + "/" + crArgumentParser::appItoa(maxtaofadian));
			}
			if (taofaBtn.valid() && jinruBtn.valid() && quanxiansw.valid())
			{
				if (myGroupPos == JXJGroup_FirstLeader || myGroupPos == JXJGroup_SecondLeader)
				{
					quanxiansw->setActiveSwitchSet(0);
					taofaBtn->setVisiable(true);
					taofaBtn->setEnable(false);
					if (startedFubenMap->find(fubenid) != startedFubenMap->end())
					{
						quanxiansw->setActiveSwitchSet(1);
						taofaBtn->setVisiable(false);
						jinruBtn->setVisiable(true);
						jinruBtn->setEnable(true);
					}
					else if (fubenid == 3000 || (hadWonGroupFuBenSet->find(fubenid -1) != hadWonGroupFuBenSet->end()))
					{
						quanxiansw->setActiveSwitchSet(0);
						jinruBtn->setVisiable(false);
						taofaBtn->setVisiable(true);
						taofaBtn->setEnable(true);
					}
				}
				else
				{
					quanxiansw->setActiveSwitchSet(1);
					taofaBtn->setVisiable(false);
					jinruBtn->setVisiable(true);
					jinruBtn->setEnable(false);
					if (startedFubenMap->find(fubenid) != startedFubenMap->end())
					{
						jinruBtn->setEnable(true);
					}
					else
					{
						jinruBtn->setEnable(false);
					}
				}
			}
			//if (taofaBtn.valid() && (curtaofadian < needtaofadian))
			//{
			//	taofaBtn->setEnable(false);
			//}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJCunBingCanvasUpdateMethod
//
///////////////////////////////////////
crJXJCunBingCanvasUpdateMethod::crJXJCunBingCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJCunBingCanvasUpdateMethod::crJXJCunBingCanvasUpdateMethod( const crJXJCunBingCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_limitnum(handle.m_limitnum),
	m_querenBtn(handle.m_querenBtn),
	m_scroll(handle.m_scroll),
	m_canvas(handle.m_canvas)
{
	for (unsigned char i = 0;i<3;++i)
	{
		m_icon[i] = handle.m_icon[i];
		m_ownnum[i] = handle.m_ownnum[i];
		m_cunrunum[i] = handle.m_cunrunum[i];
		m_numscroll[i] = handle.m_numscroll[i];
	}
}

void crJXJCunBingCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJCunBingCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
		m_icon[i] = str;
		break;
	case 3:
	case 4:
	case 5:
		m_ownnum[i-3] = str;
		break;
	case 6:
	case 7:
	case 8:
		m_cunrunum[i-6] = str;
		break;
	case 9:
	case 10:
	case 11:
		m_numscroll[i-9] = str;
		break;
	case 12:
		m_scroll = str;
		break;
	case 13:
		m_limitnum = str;
		break;
	case 14:
		m_querenBtn = str;
		break;
	case 15:
		m_canvas = str;
		break;
	default:
		break;
	}

}

void crJXJCunBingCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (myPlayer && myPlayer->getDataClass() &&mygroupcanvas.valid())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		ref_ptr<crImageBoxWidgetNode> icon[3];
		ref_ptr<crStaticTextWidgetNode> ownnum[3];
		ref_ptr<crEditWidgetNode> cunrunum[3];
		ref_ptr<crScrollBarWidgetNode> numscroll[3];
		ref_ptr<crScrollBarWidgetNode> scroll1 = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));

		ref_ptr<crStaticTextWidgetNode> limitcountText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_limitnum));
		ref_ptr<crButtonWidgetNode> querenBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_querenBtn));
		
		for (unsigned char i=0;i<3;++i)
		{
			icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			ownnum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_ownnum[i]));
			cunrunum[i] = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_cunrunum[i]));
			numscroll[i] = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_numscroll[i]));
		}
		for (unsigned char i = 0;i<3;++i)
		{
			if (icon[i].valid())
			{
				icon[i]->setImageName("UI_Army_Gong_N.img");
				icon[i]->setImageName("");
			}
			if (numscroll[i].valid())
			{
				numscroll[i]->setEnable(false);
			}
			if (cunrunum[i].valid())
			{
				cunrunum[i]->setString(L"");
				cunrunum[i]->setEnable(false);
			}
			if (ownnum[i].valid())
			{
				ownnum[i]->setString(L"");
			}
		}

		crData *canvasdata = mygroupcanvas->getDataClass();

		crRole *mainRole = myPlayer->getMainRole();
		crData *mainroledata = mainRole->getDataClass();
		mainroledata->getParam(WCHDATA_JXJGovPost,param);
		unsigned char post = *(unsigned char*)param;
		ref_ptr<crTableIO> govpostTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		crTableIO::StrVec record;
		int limitcount = 0;
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			if (mySelfInfo)
			{
				if(govpostTab.valid())
				{
					int cunLimitIdx = govpostTab->getTitleIndex("军团存兵上限");
					govpostTab->queryOneRecord(0,crArgumentParser::appItoa(post),record);
					limitcount = atoi(record[cunLimitIdx].c_str()) - mySelfInfo->getTroopStoredNumT();
					if (limitcountText.valid())
					{
						limitcountText->setString(crArgumentParser::appItoa(limitcount));
					}
				}
			}
		}

		int troopsnumtmp = 0;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJTroopsMap,param);
		TroopsMap *troopsMap = (TroopsMap *)param;
		data->excHandle(MAKEINT64(WCH_LockData,0));

		int val = 0;
		int cRow = troopsMap->size();
		if (scroll1.valid())
		{
			if (cRow < 3)
			{
				scroll1->setEnable(false);
			}
			else
			{
				scroll1->setEnable(true);
				int crows = cRow%3==0? cRow/3 : cRow/3+1;
				scroll1->setRange(0, crows-1);
				scroll1->setLineValue(1.0f);
				scroll1->setPageValue(1.0f);
			}
			val = scroll1->getValue();
		}
		int _index = 3 * val;
		ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		if(!troopsTab->validTab()) return;
		int iconIdx = troopsTab->getTitleIndex("icon");
		int i = 0;
		int j = 0;
		int endindex = _index+3;
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJCunruTroopsMap,param);
			TroopsMap *cunruTroopmap = (TroopsMap *)param;
			for( TroopsMap::iterator itr = troopsMap->begin();
				itr != troopsMap->end() && i<endindex;
				++itr,i++ )
			{
				if(i>=_index)
				{	
					if (j < 3 && icon[j].valid() && ownnum[j].valid() && cunrunum[j].valid() && numscroll[j].valid())
					{
						cunrunum[j]->setEnable(true);
						numscroll[j]->setEnable(true);
						troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record);
						if (!record.empty())
						{
							icon[j]->setImageName(record[iconIdx]);
						}
						TroopsMap::iterator cunitr = cunruTroopmap->find(itr->first);
						troopsnumtmp = 0;
						if (cunitr != cunruTroopmap->end())
						{

							troopsnumtmp = cunitr->second;
							troopsnumtmp = CRCore::minimum(troopsnumtmp, itr->second);
							troopsnumtmp = CRCore::minimum(troopsnumtmp, limitcount);
							
						}
						ownnum[j]->setString(crArgumentParser::appItoa(itr->second));
						numscroll[j]->setRange(0.0f,(float)(CRCore::minimum(itr->second, limitcount)));
						numscroll[j]->setLineValue(1.0f);
						numscroll[j]->outerSetValue(troopsnumtmp);
						cunrunum[j]->setString(crArgumentParser::appItoa(troopsnumtmp));
					}
					++j;
				}
			}
		}
		
	}
}

////////////////////////////////////////////
//
//crJXJCunBingSetCountScrolPosMethod
//
////////////////////////////////////////////
crJXJCunBingSetCountScrolPosMethod::crJXJCunBingSetCountScrolPosMethod() :
	m_this(NULL)
{

}

crJXJCunBingSetCountScrolPosMethod::crJXJCunBingSetCountScrolPosMethod(const crJXJCunBingSetCountScrolPosMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_type(handle.m_type),
	m_canvas(handle.m_canvas),
	m_limitnum(handle.m_limitnum)
{
	for (unsigned char i=0;i<3;++i)
	{
		m_input[i] = handle.m_input[i];
		m_icon[i] = handle.m_icon[i];
	}
}

void crJXJCunBingSetCountScrolPosMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crScrollBarWidgetNode*)param;
		break;
	}
}
void crJXJCunBingSetCountScrolPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	case 1:
	case 2:
	case 3:
		m_icon[i-1] = str;
		break;
	case 4:
	case 5:
	case 6:
		m_input[i-4] = str;
		break;
	case 7:
		m_canvas = str;
		break;
	case 8:
		m_limitnum = str;
		break;
	default:
		break;	
	}
}
void crJXJCunBingSetCountScrolPosMethod::operator ()(crHandle &handle)
{	
	if (m_this)
	{
		if(m_type <0 || m_type >2) return;
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && mygroupcanvas.valid() && mygroupcanvas->getDataClass())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,1));
			ref_ptr<crEditWidgetNode> inputtext = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_input[m_type]));
			int limitnum = 0;
			ref_ptr<crStaticTextWidgetNode> limitcountText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_limitnum));
			if (limitcountText.valid())
			{
				limitnum = atoi((limitcountText->getUTF8String()).c_str());
			}

			ref_ptr<crImageBoxWidgetNode> image = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_icon[m_type]));
			if (image.valid() && inputtext.valid())
			{
				ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				if(troopsTab->validTab())
				{
					crTableIO::StrVec record;
					int iconIdx = troopsTab->getTitleIndex("icon");
					int abstractIdx = troopsTab->getTitleIndex("abstractid");
					troopsTab->queryOneRecord(iconIdx,image->getImageName(),record);
					int abstractid = atoi(record[abstractIdx].c_str());

					int _value = m_this->getValue();
					int alreadytosave = 0;
					void *param;
					canvasdata->getParam(WCHDATA_JXJCunruTroopsMap,param);
					TroopsMap *cunruTroopmap = (TroopsMap *)param;
					for (TroopsMap::iterator tmitr = cunruTroopmap->begin();tmitr != cunruTroopmap->end();++tmitr)
					{
						if (abstractid != tmitr->first)
						{
							alreadytosave += tmitr->second;
						}
					}
					if (limitnum >= alreadytosave)
					{
						limitnum -= alreadytosave;
					}
					else
					{
						limitnum = 0;
					}
					_value = CRCore::minimum(_value,limitnum);
					if (_value >= 0)
					{		
						inputtext->setString(crArgumentParser::appItoa(_value));
						if (cunruTroopmap->find(abstractid) != cunruTroopmap->end())
						{
							(*cunruTroopmap)[abstractid] = _value;
						}
						else
						{
							cunruTroopmap->insert(std::make_pair(abstractid,_value));
						}
					}
				}
			}
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}

}


////////////////////////////////////////////
//
//crJXJCunBingSetCountinputTextMethod
//
////////////////////////////////////////////
crJXJCunBingSetCountinputTextMethod::crJXJCunBingSetCountinputTextMethod() :
	m_this(NULL)
{

}

crJXJCunBingSetCountinputTextMethod::crJXJCunBingSetCountinputTextMethod(const crJXJCunBingSetCountinputTextMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_type(handle.m_type),
	m_canvas(handle.m_canvas),
	m_limitnum(handle.m_limitnum)
{
	for (unsigned char i=0;i<3;++i)
	{
		m_icon[i] = handle.m_icon[i];
		m_numscroll[i] = handle.m_numscroll[i];
	}
}

void crJXJCunBingSetCountinputTextMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crStaticTextWidgetNode*)param;
		break;
	}
}
void crJXJCunBingSetCountinputTextMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	case 1:
	case 2:
	case 3:
		m_icon[i-1] = str;
		break;
	case 4:
	case 5:
	case 6:
		m_numscroll[i-4] = str;
		break;
	case 7:
		m_canvas = str;
		break;
	case 8:
		m_limitnum = str;
		break;
	default:
		break;	
	}
}
void crJXJCunBingSetCountinputTextMethod::operator ()(crHandle &handle)
{	
	if (m_this)
	{
		if(m_type <0 || m_type >2) return;
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);

		if (canvas.valid() && mygroupcanvas.valid() && mygroupcanvas.valid())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,1));
			ref_ptr<crScrollBarWidgetNode> numscroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_numscroll[m_type]));
			ref_ptr<crImageBoxWidgetNode> image = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_icon[m_type]));

			int limitnum = 0;
			ref_ptr<crStaticTextWidgetNode> limitcountText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_limitnum));
			if (limitcountText.valid())
			{
				limitnum = atoi((limitcountText->getUTF8String()).c_str());
			}

			if (image.valid())
			{
				ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				if(troopsTab->validTab())
				{
					void *param;
					crTableIO::StrVec record;
					int iconIdx = troopsTab->getTitleIndex("icon");
					int abstractIdx = troopsTab->getTitleIndex("abstractid");
					troopsTab->queryOneRecord(iconIdx,image->getImageName(),record);
					int abstractid = atoi(record[abstractIdx].c_str());
					canvasdata->getParam(WCHDATA_JXJCunruTroopsMap,param);
					TroopsMap *cunruTroopmap = (TroopsMap *)param;
					int alreadytosave = 0;
					for (TroopsMap::iterator tmitr = cunruTroopmap->begin();tmitr != cunruTroopmap->end();++tmitr)
					{
						if (abstractid != tmitr->first)
						{
							alreadytosave += tmitr->second;
						}
					}
					if (limitnum >= alreadytosave)
					{
						limitnum -= alreadytosave;
					}
					else
					{
						limitnum = 0;
					}

					int _value = 0;
					if (numscroll.valid())
					{
						_value = atoi((m_this->getUTF8String()).c_str());
						_value = CRCore::minimum(_value,limitnum);
						numscroll->outerSetValue(_value);
						int maxrange = (int)numscroll->getRange()[1];
						_value = CRCore::minimum(_value,maxrange);
						m_this->setString(crArgumentParser::appItoa(_value));
					}

					if (_value >= 0)
					{
						if (cunruTroopmap->find(abstractid) != cunruTroopmap->end())
						{
							(*cunruTroopmap)[abstractid] = _value;
						}
						else
						{
							cunruTroopmap->insert(std::make_pair(abstractid,_value));
						}
					}
				}
			}
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,0));
		}

	}

}


/************************************************************************/
/*    crJXJConfirmdonateTroopsMethod                                                                  */
/************************************************************************/
JXJ::crJXJConfirmdonateTroopsMethod::crJXJConfirmdonateTroopsMethod():
	m_this(NULL)
{

}

JXJ::crJXJConfirmdonateTroopsMethod::crJXJConfirmdonateTroopsMethod( const crJXJConfirmdonateTroopsMethod & handle ):
	crMethod(handle),
	m_limitnum(handle.m_limitnum)
{
}

void JXJ::crJXJConfirmdonateTroopsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJConfirmdonateTroopsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_limitnum = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJConfirmdonateTroopsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (m_this && netConductor )
	{
		do 
		{
			ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
			if(!canvas.valid()) break;
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (mygroupcanvas.valid() == false) break;
			crData * canvasData = mygroupcanvas->getDataClass();
			if (!canvasData) break;
			ref_ptr<crStaticTextWidgetNode> limitcountText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_limitnum));
			if (!limitcountText.valid()) break;
			int limitnum = atoi((limitcountText->getUTF8String()).c_str());
			void *param;
			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJCunruTroopsMap,param);
			TroopsMap *cunrutroopmap = (TroopsMap *)param;
			unsigned char size = cunrutroopmap->size();
			unsigned char suc = 0;
			if (size > 0)
			{
				int totalnum = 0;
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1 + 8 * size );
				stream->_writeUChar(size);	
				for (TroopsMap::iterator itr = cunrutroopmap->begin();itr != cunrutroopmap->end();++itr)
				{
					stream->_writeInt(itr->first);
					stream->_writeInt(itr->second);
					totalnum += itr->second;
				}
				if (totalnum <= limitnum)
				{
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvConfirmdonateTroops,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
					suc = 1;
				}
				if (suc != 1)
				{
					int msgid = 5009;
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
			}
			
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
		} while (0);
	}
}


/************************************************************************/
/*    crJXJRecvConfirmdonateTroopsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvConfirmdonateTroopsMethod::crJXJRecvConfirmdonateTroopsMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvConfirmdonateTroopsMethod::crJXJRecvConfirmdonateTroopsMethod( const crJXJRecvConfirmdonateTroopsMethod& handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvConfirmdonateTroopsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvConfirmdonateTroopsMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvConfirmdonateTroopsMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			unsigned char size = m_stream->_readUChar();
			TroopsMap tmptroopmap;
			int abstractid = 0;
			int count = 0;
			int totalcount = 0;
			for (unsigned i = 0; i< size;++i)
			{
				abstractid = m_stream->_readInt();
				count = m_stream->_readInt();
				tmptroopmap.insert(std::make_pair(abstractid,count));
				totalcount += count;
			}
			void *param;
			unsigned char ret = 0;
			int playerid = m_this->getPlayerID();
			std::string playername = m_this->getCharacterName();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && data)
			{
				CRCore::ScopedLock<crData> lock(*data);
				data->getParam(WCHDATA_JXJTroopsMap,param);
				TroopsMap *troopsMap = (TroopsMap *)param;
				TroopsMap::iterator titr;
				TroopsMap::iterator gitr;
				TroopsMap realtroopsmap;
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				CRCore::ScopedLock<crData> lock1(*gsBrainData);
				ref_ptr<crData> groupdata;
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator conitr = consortiamap->find(mygroupid);
				if (conitr != consortiamap->end())
				{
					groupdata = conitr->second->getDataClass();
				}
				if (groupdata.valid())
				{
					TroopsMap *grouptroopmap = NULL;
					ConsortiaMemberMap *consortiamembermap = NULL;
					ConsortiaMemberMap::iterator consitr;
					std::string groupnotice = playername + "存入: ";

					groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					consortiamembermap = (ConsortiaMemberMap *)param;
					if (consortiamembermap)
					{
						consitr = consortiamembermap->find(playerid);
						if (consitr != consortiamembermap->end())
						{
							groupdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
							grouptroopmap = (TroopsMap *)param;
							if(!grouptroopmap) return;
							ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
							crTableIO::StrVec record;
							int nameIdx = troopsTab->getTitleIndex("name");
							for (TroopsMap::iterator tmpitr = tmptroopmap.begin(); tmpitr!=tmptroopmap.end();++tmpitr)
							{
								abstractid = tmpitr->first;
								count = tmpitr->second;
								if(0 == count) continue;
								titr = troopsMap->find(abstractid);
								gitr = grouptroopmap->find(abstractid);
								if (titr != troopsMap->end())
								{
									if (titr->second >= count)
									{
										titr->second = titr->second - count;
										if (gitr != grouptroopmap->end())
										{
											gitr->second += count;
										}
										else
										{
											(*grouptroopmap)[abstractid] = count;
										}
										realtroopsmap.insert(std::make_pair(abstractid, count));
										consitr->second->setTroopStoredNumT(consitr->second->getTroopStoredNumT() + count);
										if (troopsTab.valid())
										{
											if(troopsTab->queryOneRecord(0,crArgumentParser::appItoa(abstractid),record)>= 0 && !record.empty())
											{
												groupdata->getParam(WCHDATA_JXJConsortiaRecordsDeq,param);
												ConsortiaRecordsDeq *consortiarecordsDeq = (ConsortiaRecordsDeq *)param;
												if (consortiarecordsDeq)
												{
													while(consortiarecordsDeq->size() >= 200)
													{
														consortiarecordsDeq->pop_front();
													}
													std::string troopname = record[nameIdx];
													int _begin = 0;
													_begin = troopname.find(" ");
													while (_begin != -1)
													{
														troopname.replace(_begin,1,"");
														_begin = troopname.find(" ");
													}
													char szNotice[256] = "\0";
													char timeStr[64] = "\0";
													Timer_t _time = time(0);
													memset(timeStr,0,sizeof(timeStr));
													strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S \0",localtime(&_time) ); 
													sprintf_s(szNotice,"%s玩家%s存入%d%s", timeStr, playername.c_str(), count,troopname.c_str());
													consortiarecordsDeq->push_back(szNotice);
													groupnotice += crArgumentParser::appItoa(count) + troopname + ", ";
													ret = 2;
												}
											}
										}
									}
									else
									{
										ret = 1;	//兵力不足
										break;
									}
								}
								else
								{
									ret = 1;
									break;
								}
							}

						}
					}
					if (ret == 2)
					{
						CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", groupnotice,0,0,mygroupid);
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
						bool modify = true;
						groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2 + (realtroopsmap.size())* 8);
						stream->_writeUChar(ret);
						stream->_writeUChar(realtroopsmap.size());
						TroopsMap::iterator rlmItr = realtroopsmap.begin();
						for (; rlmItr != realtroopsmap.end(); ++ rlmItr)
						{
							stream->_writeInt(rlmItr->first);
							stream->_writeInt(rlmItr->second);
						}	
						crPlayerDataEventPacket packet;
						if (playerData.valid())
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvConfirmdonateTroops,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					
					}
				}	
			}

		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc = m_stream->_readUChar();
			if (suc == 2)
			{
				void *param;
				ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
				if (mygroupcanvas.valid() && myPlayer)
				{
					crData *data = m_this->getDataClass();
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata && data)
					{
						CRCore::ScopedLock<crData> lock(*data);
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJCunruTroopsMap,param);
						TroopsMap *cunrutroopmap = (TroopsMap *)param;
						cunrutroopmap->clear();
						canvasdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
						TroopsMap * grouptroopsmap = (TroopsMap *)param;
						TroopsMap::iterator itr;
						data->getParam(WCHDATA_JXJTroopsMap,param);
						TroopsMap *troopsMap = (TroopsMap *)param;
						int size = m_stream->_readUChar();
						int abstractid = 0;
						int count = 0;
						for (unsigned char i = 0; i< size; ++i)
						{
							abstractid = m_stream->_readInt();
							count = m_stream->_readInt();
							itr = grouptroopsmap->find(abstractid);
							if (itr != grouptroopsmap->end())
							{
								itr->second += count;
							}
							else
							{
								grouptroopsmap->insert(std::make_pair(abstractid,count));
							}
							itr = troopsMap->find(abstractid);
							if (itr != troopsMap->end())
							{
								if(itr->second >= count)
									itr->second -= count;
								else
									itr->second = 0;
							}
						}
						for (unsigned char i = 0; i < size; ++ i)
						{
							abstractid = m_stream->_readInt();
							count = m_stream->_readInt();
							grouptroopsmap->insert(std::make_pair(abstractid,count));
						}
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));	
					}
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJConfirmTakeTroopsMethod                                                                  */
/************************************************************************/
JXJ::crJXJConfirmTakeTroopsMethod::crJXJConfirmTakeTroopsMethod():
	m_this(NULL)
{

}

JXJ::crJXJConfirmTakeTroopsMethod::crJXJConfirmTakeTroopsMethod( const crJXJConfirmTakeTroopsMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJConfirmTakeTroopsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJConfirmTakeTroopsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_icon = str;
		break;
	case 1:
		m_input = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJConfirmTakeTroopsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (m_this && netConductor )
	{
		do 
		{
			ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
			ref_ptr<crImageBoxWidgetNode> tmpimage = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_icon));
			ref_ptr<crStaticTextWidgetNode> numtext = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
			if(!tmpimage.valid()) break;
			if(!numtext.valid()) break;
			std::string imagename = tmpimage->getImageName();
			ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
			crTableIO::StrVec record;
			if(!troopsTab.valid()) break;
			int iconIdx = troopsTab->getTitleIndex("icon");	
			int abstractIdx = troopsTab->getTitleIndex("abstractid");
			troopsTab->queryOneRecord(iconIdx,imagename,record);
			if(record.empty()) break;
			int abstractid = atoi(record[abstractIdx].c_str());
			int count = atoi(numtext->getUTF8String().c_str());
			if(0 == count) break;
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(8);
			stream->_writeInt(abstractid);
			stream->_writeInt(count);			
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvConfirmTakeTroops,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
			
		} while (0);
	}
}


/************************************************************************/
/*    crJXJRecvConfirmTakeTroopsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvConfirmTakeTroopsMethod::crJXJRecvConfirmTakeTroopsMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvConfirmTakeTroopsMethod::crJXJRecvConfirmTakeTroopsMethod( const crJXJRecvConfirmTakeTroopsMethod& handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvConfirmTakeTroopsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvConfirmTakeTroopsMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvConfirmTakeTroopsMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			
			int abstractid = m_stream->_readInt();
			int count = m_stream->_readInt();
			
			void *param;
			unsigned char ret = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && data)
			{
				CRCore::ScopedLock<crData> lock(*data);
				ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				crTableIO::StrVec record;
				if(!troopsTab.valid()) return;
				int gongxianIdx = troopsTab->getTitleIndex("花费军团贡献");
				int abstractIdx = troopsTab->getTitleIndex("abstractid");
				int nameIdx = troopsTab->getTitleIndex("name");
				troopsTab->queryOneRecord(abstractIdx,crArgumentParser::appItoa(abstractid),record);
				if(record.empty()) return;
				int needgongxian = count * atoi(record[gongxianIdx].c_str());
				data->getParam(WCHDATA_JXJTroopsMap,param);
				TroopsMap *troopsMap = (TroopsMap *)param;
				TroopsMap::iterator titr;
				TroopsMap::iterator gitr;
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				CRCore::ScopedLock<crData> lock1(*gsBrainData);
				ref_ptr<crData> groupdata;
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator conitr = consortiamap->find(mygroupid);
				if (conitr != consortiamap->end())
				{
					groupdata = conitr->second->getDataClass();
				}
				if (groupdata.valid())
				{
					TroopsMap *grouptroopmap = NULL;
					ConsortiaMemberMap *consortiamembermap = NULL;
					ConsortiaMemberMap::iterator consitr;
					groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					consortiamembermap = (ConsortiaMemberMap *)param;
					if (consortiamembermap)
					{
						consitr = consortiamembermap->find(playerid);
						if (consitr != consortiamembermap->end())
						{
							int totalgongxian = consitr->second->getGroupContribute();
							if (totalgongxian >= needgongxian)
							{
								groupdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
								grouptroopmap = (TroopsMap *)param;
								if (grouptroopmap)
								{
									titr = troopsMap->find(abstractid);
									gitr = grouptroopmap->find(abstractid);
									if (gitr != grouptroopmap->end())
									{
										if (gitr->second >= count)
										{
											gitr->second = gitr->second - count;
											if (titr != troopsMap->end())
											{
												titr->second += count;
											}
											else
											{
												(*troopsMap)[abstractid] = count;
											}
											consitr->second->setGroupContribute(totalgongxian - needgongxian);
											consitr->second->setTroopTakedNumT(consitr->second->getTroopTakedNumT() + count);

											if (troopsTab.valid())
											{
												if(troopsTab->queryOneRecord(0,crArgumentParser::appItoa(abstractid),record)>= 0 && !record.empty())
												{
													groupdata->getParam(WCHDATA_JXJConsortiaRecordsDeq,param);
													ConsortiaRecordsDeq *consortiarecordsDeq = (ConsortiaRecordsDeq *)param;
													if (consortiarecordsDeq)
													{
														while(consortiarecordsDeq->size() >= 200)
														{
															consortiarecordsDeq->pop_front();
														}
														std::string troopname = record[nameIdx];
														int _begin = 0;
														_begin = troopname.find(" ");
														while (_begin != -1)
														{
															troopname.replace(_begin,1,"");
															_begin = troopname.find(" ");
														}
														char szNotice[256] = "\0";
														char timeStr[64] = "\0";
														Timer_t _time = time(0);
														std::string playername = m_this->getCharacterName();
														memset(timeStr,0,sizeof(timeStr));
														strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S \0",localtime(&_time) ); 
														sprintf_s(szNotice,"%s玩家%s取出%d%s", timeStr, playername.c_str(), count,troopname.c_str());
														consortiarecordsDeq->push_back(szNotice);
														sprintf_s(szNotice,"%s取出%d%s", playername.c_str(), count,troopname.c_str());
														CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
														gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
														crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
														gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
														ret = 2;
													}
												}
											}
										}
										else
										{
											ret = 1; //兵力不足
										}
									}
									else
									{
										ret = 1; //兵力不足
									}
								}
							}
							else
							{
								ret = 3; //贡献不足
							}
						}
					}
				}

				if (ret == 2)
				{
					bool modify = true;
					groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(9);
					stream->_writeUChar(ret);	
					stream->_writeInt(abstractid);
					stream->_writeInt(count);
					crPlayerDataEventPacket packet;
					if (playerData.valid())
					{
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvConfirmTakeTroops,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}	
			}

		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc = m_stream->_readUChar();
			if (suc == 2)
			{
				void *param;
				ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
				if (mygroupcanvas.valid() && myPlayer)
				{
					crData *data = m_this->getDataClass();
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata && data)
					{
						int abstractid = m_stream->_readInt();
						int count = m_stream->_readInt();
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
						TroopsMap * grouptroopsmap = (TroopsMap *)param;
						TroopsMap::iterator itr;
						itr = grouptroopsmap->find(abstractid);
						if (itr != grouptroopsmap->end())
						{
							if(itr->second >= count)
								itr->second -= count;
							else
								itr->second = 0;
						}
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
						data->excHandle(MAKEINT64(WCH_LockData,1));
						data->getParam(WCHDATA_JXJTroopsMap,param);
						TroopsMap *troopsMap = (TroopsMap *)param;
						itr = troopsMap->find(abstractid);
						if (itr != troopsMap->end())
						{
							itr->second += count;
						}
						else
						{
							troopsMap->insert(std::make_pair(abstractid, count));
						}
						data->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJSetTakeTroopsIndexMethod                                                                  */
/************************************************************************/
JXJ::crJXJSetTakeTroopsIndexMethod::crJXJSetTakeTroopsIndexMethod():
	m_this(NULL),
	m_idx(0)
{

}

JXJ::crJXJSetTakeTroopsIndexMethod::crJXJSetTakeTroopsIndexMethod( const crJXJSetTakeTroopsIndexMethod & handle ):
	crMethod(handle),
	m_idx(handle.m_idx)
{
}

void JXJ::crJXJSetTakeTroopsIndexMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJSetTakeTroopsIndexMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_idx = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void JXJ::crJXJSetTakeTroopsIndexMethod::operator()( crHandle &handle )
{
	if (m_this )
	{
		do 
		{
			ref_ptr<crCanvasNode> mygroupcanvas = m_this->getParentCanvas();
			if (mygroupcanvas.valid() == false) break;
			crData * canvasData = mygroupcanvas->getDataClass();
			if (!canvasData) break;
			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->inputParam(WCHDATA_JXJQuBingBtnIdx,&m_idx);
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
		} while (0);
	}
}


/////////////////////////////////////////
//
//crJXJQuBingCanvasUpdateMethod
//
///////////////////////////////////////
crJXJQuBingCanvasUpdateMethod::crJXJQuBingCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJQuBingCanvasUpdateMethod::crJXJQuBingCanvasUpdateMethod( const crJXJQuBingCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_icon(handle.m_icon),
	m_quchunum(handle.m_quchunum),
	m_numscroll(handle.m_numscroll),
	m_limitnum(handle.m_limitnum),
	m_querenBtn(handle.m_querenBtn),
	m_canvas(handle.m_canvas),
	m_totalgongxian(handle.m_totalgongxian),
	m_curowngongxian(handle.m_curowngongxian)
{
	
}

void crJXJQuBingCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJQuBingCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_icon = str;
		break;
	case 1:
		m_quchunum = str;
		break;
	case 2:
		m_totalgongxian = str;
		break;
	case 3:
		m_numscroll = str;
		break;
	case 4:
		m_limitnum = str;
		break;
	case 5:
		m_querenBtn = str;
		break;
	case 6:
		m_canvas = str;
		break;
	case 7:
		m_curowngongxian = str;
		break;
	default:
		break;
	}

}

void crJXJQuBingCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (myPlayer && myPlayer->getDataClass() &&mygroupcanvas.valid())
	{
		std::string iconstr("UI_Crops3_Icon");
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		ref_ptr<crScrollBarWidgetNode> numscroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_numscroll));
		ref_ptr<crStaticTextWidgetNode> limitcountText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_limitnum));
		ref_ptr<crButtonWidgetNode> querenBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_querenBtn));
		ref_ptr<crImageBoxWidgetNode> image = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon));
		ref_ptr<crStaticTextWidgetNode> quchunum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_quchunum));
		ref_ptr<crStaticTextWidgetNode> totalgongxian = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_totalgongxian));
		ref_ptr<crStaticTextWidgetNode> curowngongxiantext = dynamic_cast<crStaticTextWidgetNode *>(mygroupcanvas->getWidget(m_curowngongxian));
		if (image.valid())
		{
			image->setImageName("UI_Army_Gong_N.img");
			image->setImageName("");
		}

		if (totalgongxian.valid())
		{
			totalgongxian->setString(L"");
		}
		if (quchunum.valid())
		{
			quchunum->setString(L"");
		}
		int curowngongxian = 0;
		if (curowngongxiantext.valid())
		{
			curowngongxian = atoi((curowngongxiantext->getUTF8String()).c_str());
		}
		
		crData *canvasdata = mygroupcanvas->getDataClass();

		crRole *mainRole = myPlayer->getMainRole();
		crData *mainroledata = mainRole->getDataClass();
		mainroledata->getParam(WCHDATA_JXJGovPost,param);
		unsigned char post = *(unsigned char*)param;
		ref_ptr<crTableIO> govpostTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		crTableIO::StrVec record;
		int limitcount = 0;
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			if (mySelfInfo)
			{
				if(govpostTab.valid())
				{
					int cunLimitIdx = govpostTab->getTitleIndex("军团取兵上限");
					govpostTab->queryOneRecord(0,crArgumentParser::appItoa(post),record);
					limitcount = atoi(record[cunLimitIdx].c_str()) - mySelfInfo->getTroopTakedNumT();
					if (limitcountText.valid())
					{
						limitcountText->setString(crArgumentParser::appItoa(limitcount));
					}
				}
			}
		}

		if (canvasdata)
		{
			do 
			{
				canvasdata->getParam(WCHDATA_JXJQuBingBtnIdx,param);
				unsigned char idx = *(unsigned char *)param;
				ref_ptr<crImageBoxWidgetNode> tmpimage = dynamic_cast<crImageBoxWidgetNode *>(mygroupcanvas->getWidget(iconstr + crArgumentParser::appItoa(idx)));
				if(!tmpimage.valid()) break;
				std::string imagename = tmpimage->getImageName();
				ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				if(!troopsTab.valid()) break;
				int iconIdx = troopsTab->getTitleIndex("icon");	
				int abstractIdx = troopsTab->getTitleIndex("abstractid");
				int gongxianIdx = troopsTab->getTitleIndex("花费军团贡献");
				troopsTab->queryOneRecord(iconIdx,imagename,record);
				if(record.empty()) break;
				int abstractid = atoi(record[abstractIdx].c_str());

				canvasdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
				TroopsMap *groupTroopmap = (TroopsMap *)param;
				TroopsMap::iterator gmitr =  groupTroopmap->find(abstractid);
				int tmpnum = 0;
				if ( gmitr != groupTroopmap->end())
				{
					if (image.valid())
					{
						image->setImageName(imagename);
					}
					if (numscroll.valid() && quchunum.valid() && totalgongxian.valid())
					{		
						numscroll->setEnable(true);
						int needgongxian = atoi(record[gongxianIdx].c_str());
						tmpnum = numscroll->getValue();
						tmpnum = CRCore::minimum(tmpnum,gmitr->second);
						tmpnum = CRCore::minimum(tmpnum,limitcount);
						tmpnum = CRCore::minimum(tmpnum,(int)(curowngongxian / needgongxian));
						numscroll->setRange(0.0f,(float)(gmitr->second));
						numscroll->setLineValue(1.0f);
						totalgongxian->setString(crArgumentParser::appItoa(tmpnum * needgongxian));
						quchunum->setString(crArgumentParser::appItoa(tmpnum));
					}
				}				
			} while (0);

		}

	}
}

/////////////////////////////////////////
//
//crJXJRenMingCanvasUpdateMethod
//
///////////////////////////////////////
crJXJRenMingCanvasUpdateMethod::crJXJRenMingCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJRenMingCanvasUpdateMethod::crJXJRenMingCanvasUpdateMethod( const crJXJRenMingCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_renmingTab(handle.m_renmingTab),
	m_ListScroll(handle.m_ListScroll),
	m_canvas(handle.m_canvas)
{
	for (unsigned char i = 0;i<10;++i)
	{
		m_renmingBtn[i] = handle.m_renmingBtn[i];
		m_jiechuBtn[i] = handle.m_jiechuBtn[i];
	}
}

void crJXJRenMingCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJRenMingCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_renmingBtn[i] = str;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_jiechuBtn[i - 10] = str;
		break;
	case 20:
		m_renmingTab = str;
		break;
	case 21:
		m_ListScroll = str;
		break;
	case 22:
		m_canvas = str;
		break;
	default:
		break;
	}

}

void crJXJRenMingCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (myPlayer && myPlayer->getDataClass() &&mygroupcanvas.valid())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchiMap = (ChengchiMap *)param;
		ref_ptr<crButtonWidgetNode> renmingBtn[10];
		ref_ptr<crButtonWidgetNode> mianchuBtn[10];
		ref_ptr<crTableWidgetNode> renmingTab = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_renmingTab));
		ref_ptr<crScrollBarWidgetNode> scroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_ListScroll));

		if (renmingTab.valid())
		{
			renmingTab->clearData();
		}

		for (unsigned char i = 0;i<10;++i)
		{
			renmingBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_renmingBtn[i]));
			mianchuBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_jiechuBtn[i]));
		}
		for (unsigned char i = 0;i<10;++i)
		{
			if (renmingBtn[i].valid())
			{
				renmingBtn[i]->setVisiable(false);
			}
			if (mianchuBtn[i].valid())
			{
				mianchuBtn[i]->setVisiable(false);
			}
		}
		crData *canvasdata = mygroupcanvas->getDataClass();
		
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
			MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
			if (!groupCantonVec->empty())
			{
				int cantonId = (*groupCantonVec)[0];
				ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				crTableIO::DataVec recordVec;
				int chengchiidIdx = chengchitab->getTitleIndex("id");
				int cantonIdx = chengchitab->getTitleIndex("州ID");
				int chengchinameIdx = chengchitab->getTitleIndex("名字");
				int junnameIdx = chengchitab->getTitleIndex("郡名");
				chengchitab->queryRecords(cantonIdx,crArgumentParser::appItoa(cantonId),recordVec);
				if(recordVec.empty()) return;
				ChengchiMap::iterator ccmitr;
				for (crTableIO::DataVec::iterator ritr = recordVec.begin();ritr != recordVec.end();)
				{
					ccmitr = chengchiMap->find(atoi((*ritr)[chengchiidIdx].c_str()));
					if (ccmitr != chengchiMap->end())
					{
						if (shiliid != ccmitr->second->getChengzhuShili())
						{
							ritr = recordVec.erase(ritr);
							continue;
						}
					}
					++ritr;
				}
				if (renmingTab.valid())
				{
					int val = 0;
					int cRow = recordVec.size();
					if (scroll.valid())
					{
						if (cRow <= 10)
						{
							scroll->setEnable(false);
						}
						else
						{
							scroll->setEnable(true);
							/*int crows = cRow%10==0? cRow/10 : cRow/10+1;*/
							scroll->setRange(0, cRow - 10);
							scroll->setLineValue(1.0f);
							scroll->setPageValue(1.0f);
						}
						val = scroll->getValue();
					}
					int _index = 0;
					int endindex = val+10;
					int row = 0;
					int i = 0;
					canvasdata->getParam(WCHDATA_JXJGroupChengChiIdVec,param);
					ChengChiIdVec * chengChiIdVec = (ChengChiIdVec *)param;
					chengChiIdVec->clear();
					canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
					ClientFenFengedChengChiMap * fenFengMap = (ClientFenFengedChengChiMap *)param;
					ClientFenFengedChengChiMap::iterator ffccmitr;
					for (crTableIO::DataVec::iterator itr = recordVec.begin();itr != recordVec.end() && row < endindex;++itr,++row)
					{
						if (row >= val)
						{
							int chengchiid = atoi((*itr)[chengchiidIdx].c_str());
							renmingTab->addData(0,_index,(*itr)[junnameIdx]);
							renmingTab->addData(1,_index,(*itr)[chengchinameIdx]);
							
							if (renmingBtn[i].valid() && mianchuBtn[i].valid())
							{
								ffccmitr = fenFengMap->find(chengchiid);
								if(ffccmitr != fenFengMap->end())
								{
									renmingBtn[i]->setVisiable(false);
									mianchuBtn[i]->setVisiable(true);
									renmingTab->addData(2,_index,ffccmitr->second);
								}
								else
								{
									renmingBtn[i]->setVisiable(true);
									mianchuBtn[i]->setVisiable(false);
									renmingTab->addData(2,_index,"----");
								}
							}
							++_index;
							++i;
							chengChiIdVec->push_back(chengchiid);
						}
					}
				}
		

			}
		}


	}
}

////////////////////////////////////////////
//
//crJXJEditWidgetSizeLimitMethod
//
////////////////////////////////////////////
crJXJEditWidgetSizeLimitMethod::crJXJEditWidgetSizeLimitMethod() :
	m_this(NULL)
{

}

crJXJEditWidgetSizeLimitMethod::crJXJEditWidgetSizeLimitMethod(const crJXJEditWidgetSizeLimitMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_limit(handle.m_limit)
{

}

void crJXJEditWidgetSizeLimitMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crEditWidgetNode*)param;
		break;
	}
}
void crJXJEditWidgetSizeLimitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_limit = (unsigned short)(atoi(str.c_str()));
		break;
	default:
		break;	
	}
}
void crJXJEditWidgetSizeLimitMethod::operator ()(crHandle &handle)
{	
	std::wstring str = m_this->getString();
	if (str.length() > m_limit)
	{
		m_this->setString(str.substr(0,m_limit-1));
	}
}

/////////////////////////////////////////
//
//crJXJCorps2Tips2UpdateMethod
//
///////////////////////////////////////
crJXJCorps2Tips2UpdateMethod::crJXJCorps2Tips2UpdateMethod():
	m_this(NULL)
{

}

crJXJCorps2Tips2UpdateMethod::crJXJCorps2Tips2UpdateMethod( const crJXJCorps2Tips2UpdateMethod & handle ):
	crMethod(handle),
	m_input2(handle.m_input2),
	m_input1(handle.m_input1)
{

}

void crJXJCorps2Tips2UpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJCorps2Tips2UpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_input2 = str;
		break;
	case 1:
		m_textid = (int)(atoi(str.c_str()));
		break;
	case 2:
		m_input1 = str;
		break;
	default:
		break;
	}

}

void crJXJCorps2Tips2UpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crStaticTextWidgetNode> input1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1));
		if (input1.valid())
		{
			input1->setString(L"说明");
		}
		ref_ptr<crStaticTextWidgetNode> input2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2));
		if(input2.valid())
		{
			std::string notice;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			if (noticeHandle)
			{
				int mode = 2;
				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
				int texid = m_textid;
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
				noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				input2->setString(notice);
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJISGroupFuBenOpenMethod
//
/////////////////////////////////////////
crJXJISGroupFuBenOpenMethod::crJXJISGroupFuBenOpenMethod():
	m_this(NULL)
{
}
crJXJISGroupFuBenOpenMethod::crJXJISGroupFuBenOpenMethod(const crJXJISGroupFuBenOpenMethod& handle):
	crMethod(handle),
	m_radio1(handle.m_radio1),
	m_canvas(handle.m_canvas)
{
}
void crJXJISGroupFuBenOpenMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJISGroupFuBenOpenMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_radio1 = str;
		break;
	case 1:
		m_canvas = str;
		break;
	default:
		break;
	}
}
void crJXJISGroupFuBenOpenMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(m_this && playerGameData && mygroupcanvas.valid())
	{

		crData *canvasdata = mygroupcanvas->getDataClass();
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid() && canvas->getDataClass() && canvasdata)
		{
			do 
			{
				void *param;
				ref_ptr<crRadioGroupWidgetNode> radio1 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio1));
				if(!radio1.valid()) break;
				int idx = radio1->getSelect();
				ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				crTableIO::DataVec  dataVec;
				crTableIO::StrVec strVec;
				int fubenType = 1;
				if (fubenTab.valid() )
				{
					int fubenIdIdx = fubenTab->getTitleIndex("id");
					dataVec.clear();
					fubenTab->queryRecords(2,crArgumentParser::appItoa(fubenType),dataVec);

					if (idx< dataVec.size())
					{
						unsigned short fubenId = atoi(dataVec[idx][fubenIdIdx].c_str());
						canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
						StartedFubenMap *startedfubenMap = (StartedFubenMap *)param;
						if (startedfubenMap->find(fubenId) != startedfubenMap->end())
						{
							check = true;
						}
						
					}
				}
			} while (0);
		}
	}
	handle.outputParam(0,&check);
}


/////////////////////////////////////////
//
//crJXJISUIButtonEnableMethod
//
/////////////////////////////////////////
crJXJISUIButtonEnableMethod::crJXJISUIButtonEnableMethod():
	m_this(NULL)
{
}
crJXJISUIButtonEnableMethod::crJXJISUIButtonEnableMethod(const crJXJISUIButtonEnableMethod& handle):
	crMethod(handle)
{
}
void crJXJISUIButtonEnableMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJISUIButtonEnableMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJISUIButtonEnableMethod::operator()(crHandle &handle)
{
	bool check = false;
	if (m_this)
	{
		check = m_this->getEnable();
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJRecvCountryContributeMethod
//
/////////////////////////////////////////
crJXJRecvGroupContributionMethod::crJXJRecvGroupContributionMethod():
	m_this(NULL),
	m_groupcontribution(0)
{
}

crJXJRecvGroupContributionMethod::crJXJRecvGroupContributionMethod(const crJXJRecvGroupContributionMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_groupcontribution(0)
{
}

void crJXJRecvGroupContributionMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_groupcontribution = LOINT64(param64);
		}
		else
		{
			m_groupcontribution = 0;
		}
		break;
	}
}

void crJXJRecvGroupContributionMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvGroupContributionMethod::operator()(crHandle &handle)
{
	if(m_groupcontribution!=0 && m_this)
	{	
		void *param;
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			if(m_this->getDataClass())
			{
				int myplayerid = m_this->getPlayerID();
				crData * data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *(int *)param;

				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				do 
				{
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
					if (cmItr == consortiamap->end() || !(cmItr->second.valid())) 
					{
						break;
					}
					ref_ptr<crData> groupData = cmItr->second->getDataClass();
					if (groupData == NULL) break;
					groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
					ConsortiaMemberMap::iterator cmmIt = memberMap->find(myplayerid);
					if (cmmIt == memberMap->end()) break;
					if ( !(cmmIt->second.valid()) ) break;
					int *groupcontribute = &(cmmIt->second->m_groupcontribute);
					INTLIMITADD(*groupcontribute,m_groupcontribution,INT_MAX)
					bool modify = true;
					groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(m_groupcontribution);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvGroupContribution,stream.get());
					if (playerData.valid())
					{
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}

				} while (0);
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJRecvClientGroupContributionMethod
//
/////////////////////////////////////////
crJXJClientRecvGroupContributionMethod::crJXJClientRecvGroupContributionMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvGroupContributionMethod::crJXJClientRecvGroupContributionMethod(const crJXJClientRecvGroupContributionMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvGroupContributionMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientRecvGroupContributionMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvGroupContributionMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			int extra = m_stream->_readInt(); 
			if(extra!=0)
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_GroupContribute,extra));	
		}
	}
}

/////////////////////////////////////////
//
//crJXJIsGroupFirstLeaderMethod
//
/////////////////////////////////////////
crJXJIsGroupFirstLeaderMethod::crJXJIsGroupFirstLeaderMethod():
	m_this(NULL),
	m_type(0)
{
}
crJXJIsGroupFirstLeaderMethod::crJXJIsGroupFirstLeaderMethod(const crJXJIsGroupFirstLeaderMethod& handle):
	crMethod(handle),
	m_type(handle.m_type)
{
}
void crJXJIsGroupFirstLeaderMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJIsGroupFirstLeaderMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}
void crJXJIsGroupFirstLeaderMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Crops1");
	if (m_this && playerGameData && playerGameData->getDataClass())
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJConsortiaID,param);
		int mygroupid = *( int *)param;
		if (mygroupcanvas.valid() && mygroupcanvas->getDataClass() && mygroupid)
		{
			ref_ptr<crData> canvasdata = mygroupcanvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			int myGroupPos = JXJGroup_Normal;
			if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
			if (m_type == 1)
			{
				if (JXJGroup_FirstLeader == myGroupPos || JXJGroup_SecondLeader == myGroupPos)
				{
					check = true;
				}
			}
			else
			{
				if (JXJGroup_FirstLeader == myGroupPos)
				{
					check = true;
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

/////////////////////////////////////////
//
//crJXJJiLuCanvasUpdateMethod
//
///////////////////////////////////////
crJXJJiLuCanvasUpdateMethod::crJXJJiLuCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJJiLuCanvasUpdateMethod::crJXJJiLuCanvasUpdateMethod( const crJXJJiLuCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_input(handle.m_input),
	m_scroll(handle.m_scroll),
	m_canvas(handle.m_canvas)
{

}

void crJXJJiLuCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJJiLuCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_scroll = str;
		break;
	case 2:
		m_canvas = str;
		break;
	default:
		break;
	}

}

void crJXJJiLuCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crScrollBarWidgetNode> scroll  = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));
		ref_ptr<crHypertextWidgetNode> history = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_input));
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (mygroupcanvas.valid() && mygroupcanvas->getDataClass())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			void *param;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

			canvasdata ->getParam(WCHDATA_JXJMyGroupInfo,param);
			crJXJGroupInfo * groupInfo = (crJXJGroupInfo *)param;
			ConsortiaRecordsDeq & conrecordDeq = groupInfo->getConsortiaRecordsDeq();

			std::string text;
			history->clearText();
			if (!conrecordDeq.empty())
			{
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				int Deqsize = conrecordDeq.size();
				for (unsigned short i = 0;i < Deqsize;++i)
				{
					text = conrecordDeq.at(Deqsize - i - 1);
					cfg_script.Add("Content");
					cfg_script.Push("Content",i + 1);
					cfg_script.Add("Text",text + " /n");
					cfg_script.Pop();
				}
				cfg_script.Pop();
				history->setHypertext(cfg_script);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJQueryGroupJiLuMethod
//
/////////////////////////////////////////
crJXJQueryGroupJiLuMethod::crJXJQueryGroupJiLuMethod()
{
}

crJXJQueryGroupJiLuMethod::crJXJQueryGroupJiLuMethod(const crJXJQueryGroupJiLuMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryGroupJiLuMethod::inputParam(int i, void *param)
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
	}
}

void crJXJQueryGroupJiLuMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryGroupJiLuMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryGroupJiLu,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvQueryGroupJiLuMethod
//
/////////////////////////////////////////
crJXJRecvQueryGroupJiLuMethod::crJXJRecvQueryGroupJiLuMethod():
	m_netType(GameClient_Game){}
crJXJRecvQueryGroupJiLuMethod::crJXJRecvQueryGroupJiLuMethod(const crJXJRecvQueryGroupJiLuMethod& handle):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_canvas(handle.m_canvas)
{
}
void crJXJRecvQueryGroupJiLuMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvQueryGroupJiLuMethod::addParam(int i, const std::string& str)
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	case 1:
		m_canvas = str;
		break;
	}
}

void crJXJRecvQueryGroupJiLuMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			//int curgold = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				//crData *mainroledata = m_this->getMainRole()->getDataClass();
				//mainroledata -> getParam(WCHDATA_JXJConsortiaID,param);
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					int bufsize = 0;
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crData> groupdata;
					if(itr != consortiamap->end())
					{
						groupdata = itr->second->getDataClass();
					}
					if (groupdata.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2048);
						groupdata->getParam(WCHDATA_JXJConsortiaRecordsDeq,param);
						ConsortiaRecordsDeq *consortiarecordsDeq = (ConsortiaRecordsDeq *)param;
						if (consortiarecordsDeq)
						{
							unsigned short csize = consortiarecordsDeq->size();
							stream->_writeUShort(csize);
							for (unsigned short i = 0;i < csize; ++i)
							{
								stream->_writeString(consortiarecordsDeq->at(i));
							}
							crPlayerDataEventZipPacket packet;
							crPlayerDataEventZipPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryGroupJiLu,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			if (m_stream.valid())
			{
				void *param;
				unsigned short csize = m_stream->_readUShort();
				ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
				ref_ptr<crCanvasNode> jilucanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				crJXJGroupInfo *groupinfo = NULL;
				if (mygroupcanvas.valid())
				{
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata)
					{
						CRCore::ScopedLock<crData> lock(*canvasdata);
						canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
						groupinfo = (crJXJGroupInfo *)param;
						if (groupinfo)
						{
							ConsortiaRecordsDeq &consortiarecordsDeq = groupinfo->getConsortiaRecordsDeq();
							consortiarecordsDeq.clear();
							for (unsigned short i = 0; i< csize; ++i)
							{
								if (consortiarecordsDeq.size() > 200)
								{
									consortiarecordsDeq.pop_front();
								}
								consortiarecordsDeq.push_back(m_stream->_readString());
							}
						}			

					}
				}
				if (jilucanvas.valid())
				{
					jilucanvas->doEvent(WCH_UI_UpdateData);
				}
			}

		}
	}
}


/////////////////////////////////////////
//任命canvas
//crJXJFenFengCanvasUpdateMethod
//
///////////////////////////////////////
crJXJFenFengCanvasUpdateMethod::crJXJFenFengCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJFenFengCanvasUpdateMethod::crJXJFenFengCanvasUpdateMethod( const crJXJFenFengCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_gamelist(handle.m_gamelist),
	m_input1(handle.m_input1),
	m_canvas(handle.m_canvas),
	m_querenBtn(handle.m_querenBtn)
{

}

void crJXJFenFengCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJFenFengCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_gamelist = str;
		break;
	case 1:
		m_input1 = str;
		break;
	case 2:
		m_querenBtn = str;
		break;
	case 3:
		m_canvas = str;
		break;
	default:
		break;
	}

}

void crJXJFenFengCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crTableWidgetNode>  memberList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_gamelist));
		ref_ptr<crStaticTextWidgetNode> pageInput  = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1));
		ref_ptr<crButtonWidgetNode>  quedingBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_querenBtn));
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (mygroupcanvas.valid() && mygroupcanvas->getDataClass())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			void *param;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJMyGroupFenFengCurPage,param);
			int curpage = *(int *)param;
			if (memberList.valid())
			{
				memberList->clearData();
				int rowcount = 8;
				int _index = 0;
				canvasdata->getParam(WCHDATA_JXJGroupCanFengFenmemVec,param);
				ConsortiaMemberVec *memberVec = (ConsortiaMemberVec *) param;
				ConsortiaMemberVec::iterator cmItr = memberVec->begin();
				canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
				ClientFenFengedChengChiMap * fenFengMap = (ClientFenFengedChengChiMap *)param;
				ClientFenFengedChengChiMap::iterator ffccmitr;
				std::string playername;
				bool isfenfenged = false;
				for (;cmItr != memberVec->end();++cmItr)
				{
					playername = (*cmItr)->getName();
					ffccmitr = fenFengMap->begin();
					isfenfenged = false;
					for (;ffccmitr != fenFengMap->end();++ffccmitr)
					{
						if (ffccmitr->second == playername)
						{
							isfenfenged = true;
						}
					}
					if (!isfenfenged)
					{
						if (_index >= (curpage -1) * rowcount && _index < curpage * rowcount)
						{
							memberList->addData(0,_index - (curpage -1) * rowcount,playername);
							memberList->addData(1,_index - (curpage -1) * rowcount,crArgumentParser::appItoa((*cmItr)->getLevel()));
							memberList->addData(_index - (curpage -1) * rowcount,(*cmItr)->getPlayerID());
						}
						++_index;
					}	
				}
				if (pageInput.valid())
				{
					int totalpage = ceil((float)_index/(float)rowcount) ;
					if (totalpage < 1)
					{
						totalpage = 1;
					}
					canvasdata->inputParam(WCHDATA_JXJMyGroupFenFengTotalPage,&totalpage);
					/*canvasdata->getParam(WCHDATA_JXJMyGroupFenFengTotalPage,param);*/
					pageInput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}
				if (quedingBtn.valid())
				{
					int playerid = memberList->getSelectData();
					if (playerid > 0)
					{
						quedingBtn->setEnable(true);
					}
					else
					{
						quedingBtn->setEnable(false);
					}
				}
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJSetGroupFenFengChengChiIDMethod                                                                  */
/************************************************************************/
JXJ::crJXJSetGroupFenFengChengChiIDMethod::crJXJSetGroupFenFengChengChiIDMethod():
	m_this(NULL),
	m_idx(0)
{

}

JXJ::crJXJSetGroupFenFengChengChiIDMethod::crJXJSetGroupFenFengChengChiIDMethod( const crJXJSetGroupFenFengChengChiIDMethod & handle ):
	crMethod(handle),
	m_idx(handle.m_idx),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJSetGroupFenFengChengChiIDMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJSetGroupFenFengChengChiIDMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_idx = (unsigned char)(atoi(str.c_str()));
		break;
	case 1:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJSetGroupFenFengChengChiIDMethod::operator()( crHandle &handle )
{
	if (m_this )
	{
		do 
		{
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (mygroupcanvas.valid() == false) break;
			crData * canvasData = mygroupcanvas->getDataClass();
			if (!canvasData) break;
			void *param;
			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJGroupChengChiIdVec,param);
			ChengChiIdVec * chengChiIdVec = (ChengChiIdVec *)param;
			if (m_idx < chengChiIdVec->size())
			{
				int chengchiid = (*chengChiIdVec)[m_idx];
				canvasData->inputParam(WCHDATA_JXJGroupCanvasSelectedCantonId,&chengchiid);
			}
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
		} while (0);
	}
}


/**************************************************************************

METHOD: crJXJRefreshGroupCantonSpecialtyMethod
 
**************************************************************************/
JXJ::crJXJRefreshGroupCantonSpecialtyMethod::crJXJRefreshGroupCantonSpecialtyMethod():
	m_this(NULL)
{

}

JXJ::crJXJRefreshGroupCantonSpecialtyMethod::crJXJRefreshGroupCantonSpecialtyMethod( const crJXJRefreshGroupCantonSpecialtyMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJRefreshGroupCantonSpecialtyMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_groupid = (int *)(LOINT64(param64));
			m_playerid = (int *)(HIINT64(param64));
		}
		else
		{
			m_groupid = NULL;
			m_playerid = NULL;
		}
		break;
	}
}

void JXJ::crJXJRefreshGroupCantonSpecialtyMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRefreshGroupCantonSpecialtyMethod::operator()( crHandle &handle )
{
	if (m_groupid && m_playerid && m_this)
	{
		void *param;
		int _playerid = m_this->getPlayerID();
		crData *mainRoleData = m_this->getMainRole()->getDataClass();
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		if (mainRoleData && gsBrainData)
		{
			mainRoleData->getParam(WCHDATA_Level,param);
			unsigned char mylevel = *(unsigned char *)param;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator itr = consortiamap->find(*m_groupid);
			ref_ptr<crData>groupdata;
			if(itr != consortiamap->end())
			{
				groupdata = itr->second->getDataClass();
			}
			if(groupdata.valid())
			{
				ref_ptr<crTableIO> chengchiProTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiProTab);
				ref_ptr<crTableIO> juntuanProTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunTuanProTab);
				if (chengchiProTab.valid() && juntuanProTab.valid())
				{
					crTableIO::DataVec recordVec;
					CRCore::crVector2i LvVec;
					int techanidIdx = chengchiProTab->getTitleIndex("特产id");
					int techantypeIdx = chengchiProTab->getTitleIndex("特产类型");
					int techannumIdx = chengchiProTab->getTitleIndex("特产数量");
					int contributeIdx = chengchiProTab->getTitleIndex("需求贡献");
					int cantonidIdx = chengchiProTab->getTitleIndex("州ID");
					int playerlvIdx = chengchiProTab->getTitleIndex("玩家等级");
					groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
					ConsortiaMemberMap::iterator comsitr = membermap->find(*m_playerid);
					if (comsitr != membermap->end() && comsitr->second.valid())
					{
						if (_playerid != *m_playerid)
						{
							mylevel = comsitr->second->getLevel();
						}
						std::srand(time(0));
						comsitr->second->clearGroupcantonspecialtyVec();
						GroupCantonSpecialtyVec & groupcantonspecialtyVec = comsitr->second->getGroupCantonSpecialtyVec();
						ref_ptr<GroupCantonSpecialty>groupcantonspecial;
						//镇守军团
						groupdata->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
						FenFengedCantonMap * fenFengedChengChiMap = (FenFengedCantonMap *)param;
						FenFengedCantonMap::iterator ffcmitr = fenFengedChengChiMap->begin();
						int _index = 0;
						if (ffcmitr != fenFengedChengChiMap->end())
						{
							recordVec.clear();
							chengchiProTab->queryRecords(cantonidIdx,crArgumentParser::appItoa(ffcmitr->first),recordVec);
							std::random_shuffle(recordVec.begin(),recordVec.end());
							_index = 4;
							for (crTableIO::DataVec::iterator dvitr = recordVec.begin();dvitr !=recordVec.end() && _index <6; ++dvitr)
							{
								crArgumentParser::appAtoVec((*dvitr)[playerlvIdx],LvVec);
								if (mylevel >= LvVec[0] && mylevel <= LvVec[1])
								{
									groupcantonspecial = new GroupCantonSpecialty;
									groupcantonspecial->m_contribute = atoi((*dvitr)[contributeIdx].c_str());
									groupcantonspecial->m_itemid = atoi((*dvitr)[techanidIdx].c_str());
									groupcantonspecial->m_itemnum = atoi((*dvitr)[techannumIdx].c_str());
									groupcantonspecial->m_itemtype = atoi((*dvitr)[techantypeIdx].c_str());
									groupcantonspecialtyVec[_index] = groupcantonspecial;
									++_index;
								}
							}
						}
						//驻守军团
						groupdata->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
						ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
						_index = 6;
						for (ApplyCantonMap::iterator acmitr = applyCantonMap->begin();acmitr != applyCantonMap->end() && _index < 8;++acmitr)
						{
							recordVec.clear();
							chengchiProTab->queryRecords(cantonidIdx,crArgumentParser::appItoa(acmitr->first),recordVec);
							std::random_shuffle(recordVec.begin(),recordVec.end());
							for (crTableIO::DataVec::iterator dvitr = recordVec.begin();dvitr !=recordVec.end(); ++dvitr)
							{
								crArgumentParser::appAtoVec((*dvitr)[playerlvIdx],LvVec);
								if (mylevel >= LvVec[0] && mylevel <= LvVec[1])
								{
									groupcantonspecial = new GroupCantonSpecialty;
									groupcantonspecial->m_contribute = atoi((*dvitr)[contributeIdx].c_str());
									groupcantonspecial->m_itemid = atoi((*dvitr)[techanidIdx].c_str());
									groupcantonspecial->m_itemnum = atoi((*dvitr)[techannumIdx].c_str());
									groupcantonspecial->m_itemtype = atoi((*dvitr)[techantypeIdx].c_str());
									groupcantonspecialtyVec[_index] = groupcantonspecial;
									++_index;
									break;
								}
							}

						}
						playerlvIdx = juntuanProTab->getTitleIndex("玩家等级");
						techantypeIdx = juntuanProTab->getTitleIndex("特产类型");
						techanidIdx = juntuanProTab->getTitleIndex("特产id");
						techannumIdx = juntuanProTab->getTitleIndex("特产数量");
						contributeIdx = juntuanProTab->getTitleIndex("需求贡献");
						_index = 0;
						recordVec.clear();
						recordVec = juntuanProTab->getDataVec();
						std::random_shuffle(recordVec.begin(),recordVec.end());
						for (crTableIO::DataVec::iterator dvitr = recordVec.begin();dvitr !=recordVec.end() && _index < 4; ++dvitr)
						{
							crArgumentParser::appAtoVec((*dvitr)[playerlvIdx],LvVec);
							if (mylevel >= LvVec[0] && mylevel <= LvVec[1])
							{
								groupcantonspecial = new GroupCantonSpecialty;
								groupcantonspecial->m_contribute = atoi((*dvitr)[contributeIdx].c_str());
								groupcantonspecial->m_itemid = atoi((*dvitr)[techanidIdx].c_str());
								groupcantonspecial->m_itemnum = atoi((*dvitr)[techannumIdx].c_str());
								groupcantonspecial->m_itemtype = atoi((*dvitr)[techantypeIdx].c_str());
								groupcantonspecialtyVec[_index] = groupcantonspecial;
								++_index;
							}
						}
						bool modify = true;
						groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);
						crData *mdata = m_this->getDataClass();
						if (mdata)
						{
							mdata->getParam(WCHDATA_JXJGroupCantonSpecialtyVec,param);
							GroupCantonSpecialtyVec *gcspecialVec = (GroupCantonSpecialtyVec *)param;
							unsigned char i = 0;
							for (GroupCantonSpecialtyVec::iterator gitr = groupcantonspecialtyVec.begin();gitr != groupcantonspecialtyVec.end() && i<8;++gitr)
							{
								(*gcspecialVec)[i] = (*gitr);
								++i;
							}
						}
					}
				}

			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}

	}
}

/////////////////////////////////////////
//
//crJXJQueryCantonSpecialtyMethod
//
/////////////////////////////////////////
crJXJQueryCantonSpecialtyMethod::crJXJQueryCantonSpecialtyMethod()
{
}

crJXJQueryCantonSpecialtyMethod::crJXJQueryCantonSpecialtyMethod(const crJXJQueryCantonSpecialtyMethod& handle):
	crMethod(handle)
{
}

void crJXJQueryCantonSpecialtyMethod::inputParam(int i, void *param)
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
	}
}

void crJXJQueryCantonSpecialtyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_isrefresh = (bool)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}

void crJXJQueryCantonSpecialtyMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeBool(m_isrefresh);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryCantonSpecialty,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvQueryCantonSpecialtyMethod
//
/////////////////////////////////////////
crJXJRecvQueryCantonSpecialtyMethod::crJXJRecvQueryCantonSpecialtyMethod():
	m_netType(GameClient_Game){}
crJXJRecvQueryCantonSpecialtyMethod::crJXJRecvQueryCantonSpecialtyMethod(const crJXJRecvQueryCantonSpecialtyMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvQueryCantonSpecialtyMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvQueryCantonSpecialtyMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvQueryCantonSpecialtyMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			//char ret = 0;
			bool isrefresh = m_stream->_readBool();
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crData> groupdata;
					if(itr != consortiamap->end())
					{
						groupdata = itr->second->getDataClass();
					}
					if (groupdata.valid())
					{
						groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator comsitr = membermap->find(playerid);
						if (comsitr != membermap->end() && comsitr->second.valid())
						{
							GroupCantonSpecialtyVec & groupcantonspecialtyVec = comsitr->second->getGroupCantonSpecialtyVec();
							
							if (comsitr->second->isGroupcantonspecialtyVecempty() || isrefresh)
							{
								if (isrefresh)
								{
									bool needdeductgold = false;
									data->getParam(WCHDATA_JXJSpecialtyRefreshTimes,param);
									unsigned char times = *(unsigned char *)param;
									int totalprice = 0;
									int rowcounts = 0;
									ref_ptr<crTableIO>  SpecialtyRefreshTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSpecialtyRefreshTab);
									crTableIO::StrVec record;
									if (SpecialtyRefreshTab.valid())
									{
										rowcounts = SpecialtyRefreshTab->getRowCount();
										int timesIdx = SpecialtyRefreshTab->getTitleIndex("次数");
										int costIdx = SpecialtyRefreshTab->getTitleIndex("消耗元宝");
										if (timesIdx >=0 && costIdx >= 0)
										{
											if (SpecialtyRefreshTab->queryOneRecord(timesIdx,crArgumentParser::appItoa(times + 1),record) >= 0)
											{
												totalprice = atoi(record[costIdx].c_str());
											}
										}
									}

									if (totalprice > 0)
									{
										MoneyChangeData moneydata(totalprice,"州郡特产刷新");
										m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
										if (0 == moneydata.first)
										{
											m_this->doEvent(WCH_JXJRefreshGroupCantonSpecialty,MAKEINT64(&mygroupid,&playerid));
											if (times +1 < rowcounts )
											{
												++times;
												data->inputParam(WCHDATA_JXJSpecialtyRefreshTimes,&times);
											}
											//ret = 1;
										}
										//else
										//{
										//ret = -1;
										//}
									}
								}
								else
								{
									data->getParam(WCHDATA_JXJGroupCantonSpecialtyVec,param);
									GroupCantonSpecialtyVec *gcspecialtyVec = (GroupCantonSpecialtyVec *)param;
									bool isempty = true;
									for( GroupCantonSpecialtyVec::iterator itr = gcspecialtyVec->begin();
										itr != gcspecialtyVec->end();
										++itr )
									{
										if(*itr != NULL)
										{
											isempty = false;
											break;
										}
									}
									if (isempty)
									{
										m_this->doEvent(WCH_JXJRefreshGroupCantonSpecialty,MAKEINT64(&mygroupid,&playerid));
									}
									else
									{
										unsigned char i = 0;
										for (GroupCantonSpecialtyVec::iterator gitr = gcspecialtyVec->begin();gitr != gcspecialtyVec->end() && i<8;++gitr)
										{
											(groupcantonspecialtyVec)[i] = (*gitr);
											++i;
										}
									}
								}
							}
							m_this->doEvent(WCH_JXJRecvUpdateCantonSpecialty,MAKEINT64(NULL,GameServer));
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJRecvUpdateCantonSpecialtyMethod
//
/////////////////////////////////////////
crJXJRecvUpdateCantonSpecialtyMethod::crJXJRecvUpdateCantonSpecialtyMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client)
{

}
crJXJRecvUpdateCantonSpecialtyMethod::crJXJRecvUpdateCantonSpecialtyMethod( const crJXJRecvUpdateCantonSpecialtyMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(CRNetApp::Client),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}
void crJXJRecvUpdateCantonSpecialtyMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_netType = Client;
			m_stream = NULL;
		}
		break;
	}

}

void crJXJRecvUpdateCantonSpecialtyMethod::addParam( int i, const std::string& str )
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvUpdateCantonSpecialtyMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crData> groupdata;
					if(itr != consortiamap->end())
					{
						groupdata = itr->second->getDataClass();
					}
					if (groupdata.valid())
					{
						groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator comsitr = membermap->find(playerid);
						if (comsitr != membermap->end() && comsitr->second.valid())
						{
							GroupCantonSpecialtyVec & groupcantonspecialtyVec = comsitr->second->getGroupCantonSpecialtyVec();
							if (!(comsitr->second->isGroupcantonspecialtyVecempty()))
							{
								unsigned char gsize = groupcantonspecialtyVec.size();
								unsigned char realsize = 0;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(20 * gsize + 1);
								stream->_writeUChar(gsize);
								int _index = 0;
								for (GroupCantonSpecialtyVec::iterator gcsvitr = groupcantonspecialtyVec.begin();gcsvitr != groupcantonspecialtyVec.end();++gcsvitr)
								{
									if ((*gcsvitr) != NULL)
									{
										stream->_writeInt(_index);
										stream->_writeInt((*gcsvitr)->m_contribute);
										stream->_writeInt((*gcsvitr)->m_itemid);
										stream->_writeInt((*gcsvitr)->m_itemnum);
										stream->_writeInt((*gcsvitr)->m_itemtype);
										++realsize;
									}
									++_index;
								}
								data->getParam(WCHDATA_JXJSpecialtyRefreshTimes,param);
								unsigned char times = *(unsigned char *)param;
								stream->_writeUChar(times);
								int bsize = stream->getBufSize();
								stream->seekBegin();
								stream->_writeUChar(realsize);
								stream->setBufSize(bsize);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUpdateCantonSpecialty,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			if (m_stream.valid())
			{
				void *param;
				unsigned char gsize = m_stream->_readUChar();
				ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
				if (mygroupcanvas.valid())
				{
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata)
					{
						CRCore::ScopedLock<crData> lock(*canvasdata);
						canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
						crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
						if (mySelfInfo)
						{
							GroupCantonSpecialtyVec & groupcantonSpecialtyVec = mySelfInfo->getGroupCantonSpecialtyVec();
							mySelfInfo->clearGroupcantonspecialtyVec();
							int _index = 0;
							ref_ptr<GroupCantonSpecialty> groupcantonSpecialty;
							for (unsigned char i = 0; i< gsize; ++i)
							{
								groupcantonSpecialty = new GroupCantonSpecialty;
								_index = m_stream->_readInt();
								groupcantonSpecialty->m_contribute = m_stream->_readInt();
								groupcantonSpecialty->m_itemid = m_stream->_readInt();
								groupcantonSpecialty->m_itemnum = m_stream->_readInt();
								groupcantonSpecialty->m_itemtype = m_stream->_readInt();
								groupcantonSpecialtyVec[_index] = groupcantonSpecialty;
								
							}
							unsigned char times = m_stream->_readUChar();
							crData *data = m_this->getDataClass();
							if (data)
							{
								data->inputParam(WCHDATA_JXJSpecialtyRefreshTimes,&times);
							}
						}
					}
					mygroupcanvas->doEvent(WCH_UI_UpdateData);
				}
			}

		}
	}
}

/////////////////////////////////////////
//
//crJXJBuyCantonSpecialtyMethod
//
/////////////////////////////////////////
crJXJBuyCantonSpecialtyMethod::crJXJBuyCantonSpecialtyMethod() :
	m_this(NULL)
{
}
crJXJBuyCantonSpecialtyMethod::crJXJBuyCantonSpecialtyMethod(const crJXJBuyCantonSpecialtyMethod& handle) :
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJBuyCantonSpecialtyMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyCantonSpecialtyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}
}
void crJXJBuyCantonSpecialtyMethod::operator()(crHandle &handle)
{

	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{			
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(m_index);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvBuyCantonSpecialty, stream.get());
		netConductor->getNetManager()->sendPacket("all", packet);
	
	}
}

/////////////////////////////////////////
//
//crJXJRecvBuyCantonSpecialtyMethod
//
/////////////////////////////////////////
crJXJRecvBuyCantonSpecialtyMethod::crJXJRecvBuyCantonSpecialtyMethod() :
	m_netType(GameClient_Game){}
crJXJRecvBuyCantonSpecialtyMethod::crJXJRecvBuyCantonSpecialtyMethod(const crJXJRecvBuyCantonSpecialtyMethod& handle):
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}
void crJXJRecvBuyCantonSpecialtyMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvBuyCantonSpecialtyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvBuyCantonSpecialtyMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			char ret = 0;
			unsigned char _index = m_stream->_readUChar();
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crData> groupdata;
					if(itr != consortiamap->end())
					{
						groupdata = itr->second->getDataClass();
					}
					if (groupdata.valid())
					{
						ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
						groupdata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator comsitr = membermap->find(playerid);
						if (itemTab.valid() && comsitr != membermap->end() && comsitr->second.valid())
						{
							CRCore::ref_ptr<crPlayerRewardData> rewardData = new crPlayerRewardData;
							RewardItemVec rewarditemvec;
							ref_ptr<crBagItemData> itemdata;
							std::vector<std::pair<int,int> > SpecialtyResVec;//resid,count
							std::vector<std::pair<unsigned short,int> > SpecialtyEquipVec;//equipid,count
							GroupCantonSpecialtyVec & groupcantonspecialtyVec = comsitr->second->getGroupCantonSpecialtyVec();
							int mygroupcontribute = comsitr->second->getGroupContribute();
							data->getParam(WCHDATA_JXJGroupCantonSpecialtyVec,param);
							GroupCantonSpecialtyVec *gcspecialtyVec = (GroupCantonSpecialtyVec *)param;
							CRCore::ref_ptr<GroupCantonSpecialty> gcspecialty = (*gcspecialtyVec)[_index];
							CRCore::ref_ptr<GroupCantonSpecialty> groupcantonSpecialty = groupcantonspecialtyVec[_index];
							if (groupcantonSpecialty != NULL)
							{
								int count = groupcantonSpecialty->m_itemnum;
								if (count > 0)
								{
									int needcontribute = groupcantonSpecialty->m_contribute;
									if (needcontribute <= mygroupcontribute)
									{
										int itemtype = groupcantonSpecialty->m_itemtype;
										switch (itemtype)
										{
										case T_Food:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJFood,count));
											break;
										case T_Wood:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJWood,count));
											break;
										case T_Iron:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJIron,count));
											break;
										case T_Horse:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJHorse,count));
											break;
										case T_Copper:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJCoppercash,count));
											break;
										case T_Exp:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_Experience,count));
											break;
										case T_Achievement:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJAchievement,count));
											break;
										case T_Giftgold:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJGiftGold,count));
											break;
										case T_TroopsEquip:
											{
												SpecialtyEquipVec.push_back(std::make_pair(groupcantonSpecialty->m_itemid,count));
											}
											break;
										case T_Item:
											{
												itemdata = new crBagItemData;
												itemdata->setItemID(groupcantonSpecialty->m_itemid);
												itemdata->setItemCount(count);
												rewarditemvec.push_back(itemdata);												
											}
											break;
										case T_Contribute:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJCountryContribute,count));
											break;
										case T_Exploit:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJExploit,count));
											break;
										case T_GuoZhanJiFen:
											SpecialtyResVec.push_back(std::make_pair(WCHDATA_JXJGuoZhanJiFen,count));
											break;
										}
										std::vector<std::pair<int,int> >::iterator itr_res = SpecialtyResVec.begin();
										for (;itr_res!=SpecialtyResVec.end();itr_res++)
										{
											switch (itr_res->first)
											{
											case WCHDATA_JXJFood:
												rewardData->setFood(itr_res->second);
												break;
											case WCHDATA_JXJWood:
												rewardData->setWood(itr_res->second);
												break;
											case WCHDATA_JXJIron:
												rewardData->setIron(itr_res->second);
												break;
											case WCHDATA_JXJHorse:
												rewardData->setHorse(itr_res->second);
												break;
											case WCHDATA_JXJCoppercash:
												rewardData->setCoopercash(itr_res->second);
												break;
											case WCHDATA_Experience:
												rewardData->setExperience(itr_res->second);
												break;
											case WCHDATA_JXJAchievement:
												rewardData->setAchievement(itr_res->second);
												break;
											case WCHDATA_JXJGiftGold:
												rewardData->setGiftgold(itr_res->second);
												break;
											case WCHDATA_JXJCountryContribute:
												rewardData->setCountryContribution(itr_res->second);
												break;
											case WCHDATA_JXJExploit:
												rewardData->setExploit(itr_res->second );
												break;
											}
										}
										RewardItemVec::iterator itr_item = rewarditemvec.begin();
										for (;itr_item!=rewarditemvec.end();itr_item++)
										{
											itr_item->get()->setItemCount(itr_item->get()->getItemCount());
											rewardData->getRewardItemsVec().push_back(itr_item->get());
										}
										std::vector<std::pair<unsigned short,int> > ::iterator itr_equip = SpecialtyEquipVec.begin();
										for (;itr_equip!=SpecialtyEquipVec.end();itr_equip++)
										{
											rewardData->getEquipsVec().push_back(std::make_pair(itr_equip->first,itr_equip->second));
										}
										if(rewardData.valid())
										{
											m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(rewardData.get(),NULL));
										}
										ret = 1;
										comsitr->second->setGroupContribute(mygroupcontribute - needcontribute);
										groupcantonSpecialty->m_itemnum = 0;
										if (gcspecialty.valid() && gcspecialty != NULL)
										{
											gcspecialty->m_itemnum = 0;
										}
										bool modify = true;
										groupdata->inputParam(WCHDATA_JXJConsortiaModify,&modify);
										m_this->doEvent(WCH_JXJRecvUpdateCantonSpecialty,MAKEINT64(NULL,GameServer));
									}
									else
									{
										ret = 3;	//军团贡献不足
									}
								}
								else
								{
									ret = 2;	//物品已售罄
								}
							}
							else
							{
								ret = 2;	//
							}
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeChar(ret);
							stream->_writeInt(comsitr->second->getGroupContribute());
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyCantonSpecialty,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			int groupcontribute = m_stream->_readInt();
			int tipsindex = 0;
			if (success == 1)
			{
				void *param = NULL;
				ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
				if (mygroupcanvas.valid())
				{
					crData *canvasdata = mygroupcanvas->getDataClass();
					if (canvasdata)
					{
						canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
						crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
						mySelfInfo->setGroupContribute(groupcontribute);
					}
					mygroupcanvas->doEvent(WCH_UI_UpdateData);
				}
			}
			else
			{
				switch (success)
				{
				case 2://已售罄
					tipsindex = 5023;
					break;
				case 3://贡献不足
					tipsindex = 5022;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(tipsindex, NULL));//
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJChengchiziyuanCanvasUpdateMethod
//
///////////////////////////////////////
crJXJChengchiziyuanCanvasUpdateMethod::crJXJChengchiziyuanCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJChengchiziyuanCanvasUpdateMethod::crJXJChengchiziyuanCanvasUpdateMethod( const crJXJChengchiziyuanCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_position(handle.m_position),
	m_output(handle.m_output),
	m_canvas(handle.m_canvas),
	m_baguanBtn(handle.m_baguanBtn)
{
	for (unsigned char i = 0 ;i< 4;++i)
	{
		m_specialty[i] = handle.m_specialty[i];
	}
}

void crJXJChengchiziyuanCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJChengchiziyuanCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_position = str;
		break;
	case 1:
		m_output = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		m_specialty[i-2] = str;
		break;
	case 6:
		m_baguanBtn = str;
		break;
	case 7:
		m_canvas = str;
		break;
	default:
		break;
	}

}

void crJXJChengchiziyuanCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!playerGameData) return;
		crData * data = playerGameData->getDataClass();
		if(!data) return;
		ref_ptr<crButtonWidgetNode> baguan = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_baguanBtn));
		ref_ptr<crStaticTextWidgetNode> specialty[4];
		ref_ptr<crStaticTextWidgetNode> position  = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_position));
		ref_ptr<crStaticTextWidgetNode> cointext  = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_output));
		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		for (unsigned char i=0;i<4;++i)
		{
			specialty[i] =  dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_specialty[i]));
			if (specialty[i].valid())
			{
				specialty[i]->setString(L"");
			}
		}
		if (position.valid())
		{
			position->setString(L"");
		}
		if (cointext.valid())
		{
			cointext->setString(L"");
		}
		if (baguan.valid())
		{
			baguan->setEnable(false);
		}
		if (mygroupcanvas.valid() && mygroupcanvas->getDataClass())
		{
			crData *canvasdata = mygroupcanvas->getDataClass();
			if (canvasdata)
			{
				void *param;
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *chengchiMap = (ChengchiMap *)param;
				ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
				ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				if(!cantontab.valid() && !chengchitab.valid()) return;
				GroupChengChiEarningMap groupchengchiearningmap;
				GroupChengChiEarningMap::iterator gccemitr;
				crTableIO::StrVec record;
				crTableIO::DataVec chengchiVec;
				int chengchiidIdx = chengchitab->getTitleIndex("id");
				int cantonnameIdx = cantontab->getTitleIndex("名称");
				int chengweiIdx = chengchitab->getTitleIndex("称谓");
				int chengchiname = chengchitab->getTitleIndex("名字");
				int earningsTypeIdx = chengchitab->getTitleIndex("收益类型");
				int earningsNumIdx = chengchitab->getTitleIndex("收益数量");
				int techanTypeIdx = chengchitab->getTitleIndex("特产类型");
				int techanNumIdx = chengchitab->getTitleIndex("特产数量");
				int cantonidIdx = chengchitab->getTitleIndex("州ID");
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
				if (myGroupPos == JXJGroup_FirstLeader)
				{
					canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
					MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
					if (!groupCantonVec->empty())
					{
						int cantonId = (*groupCantonVec)[0];
						if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(cantonId),record)>=0)
						{
							if (position.valid()) position->setString(record[cantonnameIdx] + "州牧");
						}
						chengchitab->queryRecords(cantonidIdx,crArgumentParser::appItoa(cantonId),chengchiVec);
						int earningsNum = 0;
						int techannum = 0;
						int techantype = 0;
						int chengchiNum = 0;
						ChengchiMap::iterator ccmitr;
						for (crTableIO::DataVec::iterator ccvitr = chengchiVec.begin();ccvitr != chengchiVec.end(); ++ccvitr)
						{
							ccmitr = chengchiMap->find(atoi((*ccvitr)[chengchiidIdx].c_str()));
							if (ccmitr != chengchiMap->end())
							{
								if (shiliid == ccmitr->second->getChengzhuShili())
								{
									earningsNum += atoi((*ccvitr)[earningsNumIdx].c_str());
									techannum = 0;
									techantype = 0;
									techannum = atoi((*ccvitr)[techanNumIdx].c_str());
									techantype = atoi((*ccvitr)[techanTypeIdx].c_str());
									gccemitr = groupchengchiearningmap.find(techantype);
									if (techantype > 0 && techannum > 0)
									{
										if (gccemitr == groupchengchiearningmap.end())
										{
											groupchengchiearningmap.insert(std::make_pair(techantype,techannum));
										}
										else
										{
											groupchengchiearningmap[techantype] = techannum >= (gccemitr->second) ? techannum:(gccemitr->second);
										}
									}
									++chengchiNum;
								}
							}
						}
						gccemitr = groupchengchiearningmap.find(T_Copper);
						if (earningsNum > 0 && chengchiNum > 0)
						{
							earningsNum = earningsNum / chengchiNum * 2;
							if (gccemitr == groupchengchiearningmap.end())
							{
								groupchengchiearningmap.insert(std::make_pair(T_Copper,earningsNum));
							}
							else
							{
								groupchengchiearningmap[T_Copper] = earningsNum;
							}
						}
						
					}
				}
				else
				{
					if (baguan.valid())
					{
						baguan->setEnable(true);
					}
					
					canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
					ClientFenFengedChengChiMap *fenFengMap = (ClientFenFengedChengChiMap *)param;
					if (playerGameData)
					{
						int chengchiid = 0;
						std::string playername = playerGameData->getCharacterName();
						ClientFenFengedChengChiMap::iterator cffccm;
						int earningsnum = 0;
						int earningstype = 0;
						int techannum = 0;
						int techantype = 0;
						for (cffccm = fenFengMap->begin();cffccm != fenFengMap->end();++cffccm)
						{
							if (playername == cffccm->second)
							{
								chengchiid = cffccm->first;
								if(chengchitab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0)
								{
									if (position.valid()) position->setString(record[chengchiname] + record[chengweiIdx]);
									earningsnum = atoi(record[earningsNumIdx].c_str());
									earningstype = atoi(record[earningsTypeIdx].c_str());
									techannum = atoi(record[techanNumIdx].c_str());
									techantype = atoi(record[techanTypeIdx].c_str());
									if (earningsnum >0 && earningstype > 0)
									{
										gccemitr = groupchengchiearningmap.find(earningstype);
										if (gccemitr == groupchengchiearningmap.end())
										{
											groupchengchiearningmap.insert(std::make_pair(earningstype,earningsnum));
										}
										else
										{
											groupchengchiearningmap[earningstype] = earningsnum;
										}
									}
									if (techantype > 0 && techannum >0)
									{
										gccemitr = groupchengchiearningmap.find(techantype);
										if (gccemitr == groupchengchiearningmap.end())
										{
											groupchengchiearningmap.insert(std::make_pair(techantype,techannum));
										}
										else
										{
											groupchengchiearningmap[techantype] = techannum;
										}
									}
								}
								break;
							}
						}
					}	
				}
				std::string teChanName[] = 
				{
					"",
					"粮草","木材","铁矿","马匹","铜钱",
					"经验","战功","礼金","兵力材料","道具",
					"兵力","功勋","国家贡献","元宝","将魂","将卡好感度"
				};
				unsigned char i = 0;
				for (GroupChengChiEarningMap::iterator itr = groupchengchiearningmap.begin();itr != groupchengchiearningmap.end() && i < 4; ++ itr)
				{
					if (itr->first != T_Copper)
					{
						if (specialty[i].valid())
						{
							specialty[i]->setString(teChanName[itr->first] + ": " + crArgumentParser::appItoa(itr->second));
							++i;
						}
					}
					else
					{
						if (cointext.valid())
						{
							cointext->setString(crArgumentParser::appItoa(itr->second));
						}
					}
				}

				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

///////////////////////////////////////////
////
////crJXJUIGroupCantonSpecialtyTipsMethod
////
///////////////////////////////////////////
crJXJUIGroupCantonSpecialtyTipsMethod::crJXJUIGroupCantonSpecialtyTipsMethod():
	m_ea(NULL),
	m_this(NULL),
	m_imageindex(0)
{
}
crJXJUIGroupCantonSpecialtyTipsMethod::crJXJUIGroupCantonSpecialtyTipsMethod(const crJXJUIGroupCantonSpecialtyTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_colorred(handle.m_colorred),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJUIGroupCantonSpecialtyTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
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
	}
}
void crJXJUIGroupCantonSpecialtyTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemicon = str;
		break;
	case 4:
		m_infocanvas = str;
		break;
	case 5:
		m_infobk = str;
		break;
	case 6:
		m_infodi = str;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 9:
		m_imageindex = atoi(str.c_str());
		break;
	}
}
void crJXJUIGroupCantonSpecialtyTipsMethod::operator()(crHandle &handle)
{
	if(m_imageindex < 0) return;
	ref_ptr<crImageBoxWidgetNode> imagenode = dynamic_cast<crImageBoxWidgetNode *>(m_this);
	if (imagenode.valid())
	{
		std::string imagename = imagenode->getImageName();
		if(imagename == "UI_Icon_Shouji.img") return;
	}
	
	ref_ptr<crCanvasNode> groupcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid() && groupcanas.valid())
	{
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		if (info2)
		{
			info2->clearText();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}

		crVector4f colormode[5];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i =0; i<5;i++)
		{
			if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
			{
				if (qualityindex>=0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int nameindex = itemtab->getTitleIndex("name");
		int colorindex = itemtab->getTitleIndex("品质");

		crData *canvasdata = groupcanas->getDataClass();
		int itemid = 0;
		if (canvasdata)
		{
			void *param;
			canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			if (mySelfInfo)
			{
				GroupCantonSpecialtyVec & groupcantonSpecialtyVec = mySelfInfo->getGroupCantonSpecialtyVec();
				if (!groupcantonSpecialtyVec.empty())
				{
					if (m_imageindex < groupcantonSpecialtyVec.size())
					{
						ref_ptr<GroupCantonSpecialty> ptr = groupcantonSpecialtyVec[m_imageindex];
						if(ptr != NULL && (ptr->m_itemtype == T_Item))
						{
							itemid = groupcantonSpecialtyVec[m_imageindex]->m_itemid;
						}
					}
				}
			}
		}
		if(itemid <= 0) return;
		if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
		{
			int colormodeid = atoi(record[colorindex].c_str());
			if(name)
			{
				name->setColor(colormode[colormodeid]);
				name->setString(record[nameindex]);
			}
			if (itemicon)
			{
				itemicon->setImageName(record[iconindex]);
			}
			if(info2)
			{
				ItemMap itemMap;
				rcfg::ConfigScript outscript(&itemMap);
				crVector2i item(itemid,0);
				playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
				info2->setHypertext(outscript);
			}

			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//centerTansTar[1] += 0.5f * (tmpLength - texheight);
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(infobk)infobk->setMatrix(rotmat);

				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2,posy2;
			const crBoundingBox &infos = infobk->getBoundBox();
			float posinfox = (infos.m_max[0]-infos.m_min[0]);
			float posinfoy = (infos.m_max[1]-infos.m_min[1]);
			if (pos2[0] >= 0)
			{
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + 2.0f * posy - needmove;
			}
			if((abs(posy2)+posy)>1.0f)
			{
				if (posy2 >= 0)
				{
					posy2 -= posy;
				}
				else
				{
					posy2 += posy;
				}
			}
			crVector2 mouse(crVector2(posx2,posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUpdateExploitandTroopnumInPeopleMapMethod
//
/////////////////////////////////////////
crJXJUpdateExploitandTroopnumInPeopleMapMethod::crJXJUpdateExploitandTroopnumInPeopleMapMethod()
{
}

crJXJUpdateExploitandTroopnumInPeopleMapMethod::crJXJUpdateExploitandTroopnumInPeopleMapMethod(const crJXJUpdateExploitandTroopnumInPeopleMapMethod& handle):
	crMethod(handle)
{
}

void crJXJUpdateExploitandTroopnumInPeopleMapMethod::inputParam(int i, void *param)
{
}

void crJXJUpdateExploitandTroopnumInPeopleMapMethod::addParam(int i, const std::string& str)
{
}

void crJXJUpdateExploitandTroopnumInPeopleMapMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUpdateExploitandTroopnumInPeopleMap,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		playerGameData->doEvent(WCH_JXJQueryGroupMemberList);
	}
}

/////////////////////////////////////////
//
//crJXJRecvpdateExploitandTroopnumInPeopleMapMethod
//
/////////////////////////////////////////
crJXJRecvpdateExploitandTroopnumInPeopleMapMethod::crJXJRecvpdateExploitandTroopnumInPeopleMapMethod():
	m_netType(GameClient_Game){}
crJXJRecvpdateExploitandTroopnumInPeopleMapMethod::crJXJRecvpdateExploitandTroopnumInPeopleMapMethod(const crJXJRecvpdateExploitandTroopnumInPeopleMapMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvpdateExploitandTroopnumInPeopleMapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvpdateExploitandTroopnumInPeopleMapMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvpdateExploitandTroopnumInPeopleMapMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			if (data && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *mainroledata = m_this->getMainRole()->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJTroopsMap,param);
				int totalcount = 0;
				TroopsMap *troops = (TroopsMap*)param;
				for (TroopsMap::iterator tmitr = troops->begin();tmitr != troops->end(); ++tmitr)
				{
					totalcount += tmitr->second;
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				mainroledata->getParam(WCHDATA_JXJExploit,param);
				int *exploit = (int *)param;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				if (gsBrainData)
				{
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

					gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					crJXJShili *shili = (crJXJShili *)param;
					if (shili && shili->getDataClass())
					{
						crData *shilidata = shili->getDataClass();	
						shilidata->getParam(WCHDATA_JXJPeopleMap,param);
						PeopleMap *peoplemap = (PeopleMap *)param;
						PeopleMap::iterator peoitr = peoplemap->find(playerid);
						std::string playername;
						if (peoitr != peoplemap->end() && peoitr->second.valid())
						{
							peoitr->second->setTroopsnum(totalcount);
							peoitr->second->setExploit(*exploit);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvConsortiaExploitRankingMethod
//
/////////////////////////////////////////
crJXJRecvConsortiaExploitRankingMethod::crJXJRecvConsortiaExploitRankingMethod():
	m_netType(GameClient_Game){}
crJXJRecvConsortiaExploitRankingMethod::crJXJRecvConsortiaExploitRankingMethod(const crJXJRecvConsortiaExploitRankingMethod& handle):
	crMethod(handle),
	m_canvasName(handle.m_canvasName)
{
}
void crJXJRecvConsortiaExploitRankingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvConsortiaExploitRankingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	}
}

void crJXJRecvConsortiaExploitRankingMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				unsigned short start = m_stream->_readUShort();
				unsigned char querycount = m_stream->_readUChar();
				unsigned char shiliindex = m_stream->_readUChar();
				std::string name;
				ConsortiaLvRankItem *rankItem;
				void *param;
				crData *data = m_this->getDataClass();
				//crData *mainroledata = m_this->getMainRole()->getDataClass();
				data->getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *(int *)param;
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				serverData->getParam(WCHDATA_JXJConsortiaLvRank,param);
				ConsortiaExploitRank* consortiaExploitRank = (ConsortiaExploitRank*)param;
				ConsortiaExploitRank tmpconsortiaExploitRank = *consortiaExploitRank;

				ConsortiaExploitRank::RankSet & ExploitRankSet = consortiaExploitRank->getRankSet();
				int count = CRCore::minimum((int)querycount,(int)(ExploitRankSet.size()-start));

				ConsortiaExploitRank::RankSet::iterator itr = ExploitRankSet.begin();
				for(;itr != ExploitRankSet.end(); ++itr)
				{
					if (itr->valid() == false) continue;
					rankItem = (ConsortiaLvRankItem *)( (*itr).get() );
					CRCore::ref_ptr<ConsortiaLvRankItem > tmprankItem = new ConsortiaLvRankItem(rankItem->m_id);
					tmprankItem->m_groupName = rankItem->m_groupName;
					tmprankItem->m_leaderName = rankItem->m_leaderName;
					tmprankItem->m_lv = rankItem->m_lv;
					tmprankItem->m_memberCount = rankItem->m_memberCount;
					tmprankItem->m_exploit = rankItem->m_exploit;
					tmprankItem->m_troops = rankItem->m_troops;
					tmprankItem->m_shili = rankItem->m_shili;
					tmpconsortiaExploitRank.updateRank(tmprankItem->getKey(),tmprankItem);
				}
				ConsortiaExploitRank::RankSet & tmpExploitRankSet = tmpconsortiaExploitRank.getRankSet();
				int index = 0;
				if (!m_stream->eof())
				{
					name = m_stream->_readString();
					itr = tmpExploitRankSet.begin();
					for(;itr != tmpExploitRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem *)( (*itr).get() );
						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if (name == rankItem->m_groupName)
							{
								start = index;
								count = 1;
								break;
							}
							index++;		
						}
					}

				}
				if(count>0)
				{
					short myranking = -1;
					int recordcount = 0;
					int i = 0;
					unsigned short totalrecord = 0;
					unsigned char groupshiliid = 0;
					itr = tmpExploitRankSet.begin(); 

					for(;itr != tmpExploitRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem *)( (*itr).get() );

						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if(i >= start && i < start + count) recordcount ++;
							if (rankItem->m_id == mygroupid) myranking = i;
							++i;
							++totalrecord;
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					crPlayerDataEventPacket packet;
					stream->createBuf(132);
					stream->_writeUChar((unsigned char)recordcount);
					itr = tmpExploitRankSet.begin(); 
					for ( i = 0;i < start + count && itr != tmpExploitRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem * )(itr->get());
						if (rankItem == NULL) continue;
						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if(i>=start)
							{
								stream->_writeString(rankItem->m_groupName);//20
								stream->_writeString(rankItem->m_leaderName);//20
								stream->_writeUChar(rankItem->m_lv);//1
								stream->_writeUShort(rankItem->m_memberCount);//2
								stream->_writeInt(rankItem->m_troops);//4
								stream->_writeInt(rankItem->m_exploit);//4
								stream->_writeUChar(rankItem->m_shili);//1
								stream->_writeInt(i+1); //排名
							}
							++i;
						}
					}

					stream->_writeUShort(totalrecord);
					stream->_writeUShort(unsigned short(myranking + 1));
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvConsortiaExploitRanking,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int pageCount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRankingCount,viplv).c_str()));
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJConsortiaExploitRankingMap,param);
			GroupExploitRankingMap *rankingMap = (GroupExploitRankingMap *)param;
			rankingMap->clear();
			int count = (int)(m_stream->_readUChar());
			ref_ptr<crJXJGroupInfo> expRanking;
			for (int i = 0; i<count; i++)
			{
				expRanking = new crJXJGroupInfo;
				expRanking->setName(m_stream->_readString());
				expRanking->setCreatorName(m_stream->_readString());
				expRanking->setLv(m_stream->_readUChar());
				expRanking->setMemberCount(m_stream->_readUShort());
				expRanking->setGroupTroops(m_stream->_readInt());
				expRanking->setGroupExploit(m_stream->_readInt());
				expRanking->setShiliID(m_stream->_readUChar());
				expRanking->setRank(m_stream->_readInt());

				rankingMap->insert(std::make_pair(expRanking->getGroupExploit(),expRanking));
			}
			unsigned short mapsize = m_stream->_readUShort();
			unsigned short myranking = m_stream->_readUShort();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			int totalPage = ceil((float)mapsize/(float)pageCount);
			if(totalPage == 0) totalPage = 1;
			canvasdata->inputParam(WCHDATA_JXJRankingTotal,&totalPage);
			canvasdata->inputParam(WCHDATA_JXJMyRanking,&myranking);
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvConsortiaTroopsRankingMethod
//
/////////////////////////////////////////
crJXJRecvConsortiaTroopsRankingMethod::crJXJRecvConsortiaTroopsRankingMethod():
	m_netType(GameClient_Game){}
crJXJRecvConsortiaTroopsRankingMethod::crJXJRecvConsortiaTroopsRankingMethod(const crJXJRecvConsortiaTroopsRankingMethod& handle):
	crMethod(handle),
	m_canvasName(handle.m_canvasName)
{
}
void crJXJRecvConsortiaTroopsRankingMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvConsortiaTroopsRankingMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	}
}

void crJXJRecvConsortiaTroopsRankingMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				unsigned short start = m_stream->_readUShort();
				unsigned char querycount = m_stream->_readUChar();
				unsigned char shiliindex = m_stream->_readUChar();
				std::string name;
				ConsortiaLvRankItem *rankItem;
				void *param;
				crData *data = m_this->getDataClass();
				//crData *mainroledata = m_this->getMainRole()->getDataClass();
				data->getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *(int *)param;
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));

				serverData->getParam(WCHDATA_JXJConsortiaLvRank,param);
				ConsortiaTroopsRank* consortiaTroopsRank = (ConsortiaTroopsRank*)param;
				ConsortiaTroopsRank tmpconsortiaTroopsRank = *consortiaTroopsRank;

				ConsortiaTroopsRank::RankSet & TroopsRankSet = consortiaTroopsRank->getRankSet();

				int count = CRCore::minimum((int)querycount,(int)(TroopsRankSet.size()-start));
				ConsortiaTroopsRank::RankSet::iterator itr = TroopsRankSet.begin();

				for(;itr != TroopsRankSet.end(); ++itr)
				{
					if (itr->valid() == false) continue;
					rankItem = (ConsortiaLvRankItem *)( (*itr).get() );
					CRCore::ref_ptr<ConsortiaLvRankItem > tmprankItem = new ConsortiaLvRankItem(rankItem->m_id);
					tmprankItem->m_groupName = rankItem->m_groupName;
					tmprankItem->m_leaderName = rankItem->m_leaderName;
					tmprankItem->m_lv = rankItem->m_lv;
					tmprankItem->m_memberCount = rankItem->m_memberCount;
					tmprankItem->m_exploit = rankItem->m_exploit;
					tmprankItem->m_troops = rankItem->m_troops;
					tmprankItem->m_shili = rankItem->m_shili;
					tmpconsortiaTroopsRank.updateRank(tmprankItem->getKey(),tmprankItem);
				}
				ConsortiaTroopsRank::RankSet & tmpTroopsRankSet = tmpconsortiaTroopsRank.getRankSet();

				int index = 0;
				if (!m_stream->eof())
				{
					name = m_stream->_readString();
					itr = tmpTroopsRankSet.begin();
					for(;itr != tmpTroopsRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem *)( (*itr).get() );
						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if (name == rankItem->m_groupName)
							{
								start = index;
								count = 1;
								break;
							}
							index++;		
						}
					}

				}
				if(count>0)
				{
					short myranking = -1;
					int recordcount = 0;
					int i = 0;
					unsigned short totalrecord = 0;
					unsigned char groupshiliid = 0;
					itr = tmpTroopsRankSet.begin(); 

					for(;itr != tmpTroopsRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem *)( (*itr).get() );

						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if(i >= start && i < start + count) recordcount ++;
							if (rankItem->m_id == mygroupid) myranking = i;
							++i;
							++totalrecord;
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					crPlayerDataEventPacket packet;
					stream->createBuf(132);
					stream->_writeUChar((unsigned char)recordcount);
					itr = tmpTroopsRankSet.begin(); 
					for ( i = 0;i < start + count && itr != tmpTroopsRankSet.end(); ++itr)
					{
						if (itr->valid() == false) continue;
						rankItem = (ConsortiaLvRankItem * )(itr->get());
						if (rankItem == NULL) continue;
						if (shiliindex == 0 || rankItem->m_shili == shiliindex)
						{
							if(i>=start)
							{
								stream->_writeString(rankItem->m_groupName);//20
								stream->_writeString(rankItem->m_leaderName);//20
								stream->_writeUChar(rankItem->m_lv);//1
								stream->_writeUShort(rankItem->m_memberCount);//2
								stream->_writeInt(rankItem->m_troops);//4
								stream->_writeInt(rankItem->m_exploit);//4
								stream->_writeUChar(rankItem->m_shili);//1
								stream->_writeInt(i+1); //排名
							}
							++i;
						}
					}

					stream->_writeUShort(totalrecord);
					stream->_writeUShort(unsigned short(myranking + 1));
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvConsortiaTroopsRanking,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int pageCount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRankingCount,viplv).c_str()));
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJConsortiaTroopsRankingMap,param);
			GroupTroopsRankingMap *rankingMap = (GroupTroopsRankingMap *)param;
			rankingMap->clear();
			int count = (int)(m_stream->_readUChar());
			ref_ptr<crJXJGroupInfo> expRanking;
			for (int i = 0; i<count; i++)
			{
				expRanking = new crJXJGroupInfo;
				expRanking->setName(m_stream->_readString());
				expRanking->setCreatorName(m_stream->_readString());
				expRanking->setLv(m_stream->_readUChar());
				expRanking->setMemberCount(m_stream->_readUShort());
				expRanking->setGroupTroops(m_stream->_readInt());
				expRanking->setGroupExploit(m_stream->_readInt());
				expRanking->setShiliID(m_stream->_readUChar());
				expRanking->setRank(m_stream->_readInt());

				rankingMap->insert(std::make_pair(expRanking->getGroupTroops(),expRanking));
			}
			unsigned short mapsize = m_stream->_readUShort();
			unsigned short myranking = m_stream->_readUShort();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			int totalPage = ceil((float)mapsize/(float)pageCount);
			if(totalPage == 0) totalPage = 1;
			canvasdata->inputParam(WCHDATA_JXJRankingTotal,&totalPage);
			canvasdata->inputParam(WCHDATA_JXJMyRanking,&myranking);
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCiQuGuanZhiMethod
//
/////////////////////////////////////////
crJXJCiQuGuanZhiMethod::crJXJCiQuGuanZhiMethod()
{
}

crJXJCiQuGuanZhiMethod::crJXJCiQuGuanZhiMethod(const crJXJCiQuGuanZhiMethod& handle):
	crMethod(handle)
{
}

void crJXJCiQuGuanZhiMethod::inputParam(int i, void *param)
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
	}
}

void crJXJCiQuGuanZhiMethod::addParam(int i, const std::string& str)
{
}

void crJXJCiQuGuanZhiMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvRecvCiQuGuanZhi,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/************************************************************************/
/*    crJXJRecvCiQuGuanZhiMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvCiQuGuanZhiMethod::crJXJRecvCiQuGuanZhiMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvCiQuGuanZhiMethod::crJXJRecvCiQuGuanZhiMethod( const crJXJRecvCiQuGuanZhiMethod& handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvCiQuGuanZhiMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvCiQuGuanZhiMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvCiQuGuanZhiMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int myplayerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> myplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myplayerid));
			if (myplayerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						unsigned char suc = JXJGroupAllocateChengChiRet_failed_unkwon;
						ref_ptr<crData>groupData;
						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
						int chengchiid = 0;
						if (cmItr != consortiamap->end() && cmItr->second.valid())
						{
							groupData = cmItr->second->getDataClass();
						}
						if (groupData.valid())
						{
							groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
							FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
							for (FenFengedChengChiMap::iterator ffccmitr = fenFengedChengChiMap->begin();ffccmitr != fenFengedChengChiMap->end();++ffccmitr)
							{
								if (ffccmitr->second == myplayerid)
								{
									chengchiid = ffccmitr->first;
									fenFengedChengChiMap->erase(ffccmitr);
									suc = JXJGroupAllocateChengChiRet_suc;
									bool modify = true;
									groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
									break;
								}
							}
						}
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

						if (suc == JXJGroupAllocateChengChiRet_suc)
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(5);
							stream->_writeUChar(suc);
							stream->_writeInt(chengchiid);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJRecvRecvCiQuGuanZhi,stream.get());
							gameServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
			}

		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

					unsigned char suc = m_stream->_readUChar();
					int chengChiId = m_stream->_readInt();
			
					switch (suc)
					{
					case JXJGroupAllocateChengChiRet_suc:
						{
							canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
							ClientFenFengedChengChiMap * fenFengMap = (ClientFenFengedChengChiMap *)param;
							fenFengMap->erase(chengChiId);
							ref_ptr<crCanvasNode> ziyuancanvas = crFilterRenderManager::getInstance()->findCanvas("Corps2_Chengchiziyuan");
							if (ziyuancanvas.valid())
							{
								ziyuancanvas->doEvent(WCH_UI_UpdateData);
							}
						}
						break;
					default:
						break;
					}
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGroupDismissNotificationMethod
//
/////////////////////////////////////////
crJXJGroupDismissNotificationMethod::crJXJGroupDismissNotificationMethod()
{
}

crJXJGroupDismissNotificationMethod::crJXJGroupDismissNotificationMethod(const crJXJGroupDismissNotificationMethod& handle):
	crMethod(handle)
{
}

void crJXJGroupDismissNotificationMethod::inputParam(int i, void *param)
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
	}
}

void crJXJGroupDismissNotificationMethod::addParam(int i, const std::string& str)
{
}

void crJXJGroupDismissNotificationMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupDismissNotification,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvGroupDismissNotificationMethod
//
/////////////////////////////////////////
crJXJRecvGroupDismissNotificationMethod::crJXJRecvGroupDismissNotificationMethod()
{
}

crJXJRecvGroupDismissNotificationMethod::crJXJRecvGroupDismissNotificationMethod(const crJXJRecvGroupDismissNotificationMethod& handle):
	crMethod(handle)
{
}

void crJXJRecvGroupDismissNotificationMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGroupDismissNotificationMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGroupDismissNotificationMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param = NULL;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *( int *)param;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crData>groupdata;
					if(itr != consortiamap->end() && itr->second.valid())
					{
						groupdata = itr->second->getDataClass();
					}
					if (groupdata.valid())
					{
						groupdata->getParam(WCHDATA_JXJConsortiaDismissTime,param);
						int dismissTime = *(int *)param;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(4);
						stream->_writeInt(dismissTime);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupDismissNotification,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			int dismisstime = m_stream->_readInt();
			int h = dismisstime / (60 * 60);
			int m = (dismisstime - 60 * 60 * h) / 60;
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_JXJHadShowDismissTip,param);
			bool hadshow = *(bool *)param;
			if (dismisstime > 0 && hadshow == false)
			{
				int texid = 5029;
				std::string hour = crArgumentParser::appItoa(h);
				std::string min = crArgumentParser::appItoa(m);
				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				int mode = 1;
				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
				noticeHandle->inputParam(WCHDATA_NoticeParam1,&hour);
				noticeHandle->inputParam(WCHDATA_NoticeParam2,&min);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				hadshow = true;
				data->inputParam(WCHDATA_JXJHadShowDismissTip,&hadshow);
			}
			
		}
	}
}

/////////////////////////////////////////
//
//crJXJShowChengChiRenmingTipsMethod
//
///////////////////////////////////////
crJXJShowChengChiRenmingTipsMethod::crJXJShowChengChiRenmingTipsMethod():
	m_this(NULL)
{

}

crJXJShowChengChiRenmingTipsMethod::crJXJShowChengChiRenmingTipsMethod( const crJXJShowChengChiRenmingTipsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input1(handle.m_input1),
	m_input2(handle.m_input2)
{

}

void crJXJShowChengChiRenmingTipsMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJShowChengChiRenmingTipsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input1 = str;
		break;
	case 2:
		m_input2 = str;
		break;
	default:
		break;
	}

}

void crJXJShowChengChiRenmingTipsMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		ref_ptr<crStaticTextWidgetNode> input1 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input1));
		if (input1.valid())
		{
			input1->setString(L"说明");
		}
		ref_ptr<crStaticTextWidgetNode> input2 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input2));
		if (input2.valid())
		{
			std::string notice;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			if (noticeHandle)
			{
				int mode = 2;
				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
				int texid = 5030;
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
				noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				input2->setString(notice);
			}
		}
	}
}


/**************************************************************************

METHOD: crJXJ7DayHuodongCanvasUpdateMethod 
 
**************************************************************************/
crJXJ7DayHuodongCanvasUpdateMethod::crJXJ7DayHuodongCanvasUpdateMethod():
	m_this(NULL),
	m_3starstimes(0)
{

}

crJXJ7DayHuodongCanvasUpdateMethod::crJXJ7DayHuodongCanvasUpdateMethod( const crJXJ7DayHuodongCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_sw (handle.m_sw),
	m_timeInput(handle.m_timeInput),
	m_contentInput(handle.m_contentInput),
	m_nullImage(handle.m_nullImage),
	m_hadGottenBtn(handle.m_hadGottenBtn),
	m_scroll(handle.m_scroll),
	m_code_sw(handle.m_code_sw),
	m_codeText(handle.m_codeText),
	m_huodongprocess(handle.m_huodongprocess),
	m_huodongtitle(handle.m_huodongtitle),
	m_3starstimes(handle.m_3starstimes),
	m_festival_Introduction(handle.m_festival_Introduction),
	m_festival_Deadline(handle.m_festival_Deadline),
	m_chengchiNum(handle.m_chengchiNum),
	m_telephoneIcon(handle.m_telephoneIcon),
	m_telephoneGet(handle.m_telephoneGet)
{
	for (unsigned char i = 0; i < 7; ++ i)
	{
		m_title[i] = handle.m_title[i];
		m_title_sw[i] = handle.m_title_sw[i];
		if (i < 5)
		{
			m_imageBox[i] = handle.m_imageBox[i];
			m_num[i] = handle.m_num[i];
		}
		if (i < 3)
		{
			m_festival_Icon[i] = handle.m_festival_Icon[i];
			m_festival_Button[i] = handle.m_festival_Button[i];
			m_festival_buttonSwitch[i] = handle.m_festival_buttonSwitch[i];
		}
	}
}

void crJXJ7DayHuodongCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJ7DayHuodongCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_sw = str;
		break;
	case 1:
		m_timeInput = str;
		break;
	case 2:
		m_contentInput = str;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_title[i - 3] = str;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		m_imageBox[i - 10] = str;
		break;
	case 15:
		m_getBtn = str;
		break;
	case 16:
		m_nullImage = str;
		break;
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_num[i - 17] = str;
		break;
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
		m_title_sw[i - 22] = str;
		break;
	case 29:
		m_hadGottenBtn = str;
		break;
	case 30:
		m_scroll = str;
		break;
	case 31:
		m_code_sw = str;
		break;
	case 32:
		m_codeText = str;
		break;
	case 33:
		m_huodongtitle = str;
		break;
	case 34:
		m_huodongprocess = str;
		break;
	case 35:
	case 36:
	case 37:
		m_festival_Icon[i-35] = str;
		break;
	case 38:
	case 39:
	case 40:
		m_festival_Button[i-38] = str;
		break;
	case 41:
		m_festival_Introduction = str;
		break;
	case 42:
		m_festival_Deadline = str;
		break;
	case 43:
	case 44:
	case 45:
		m_festival_buttonSwitch[i-43] = str;
		break;
	case 46:
		m_chengchiNum = str;
		break;
	case 47:
	case 48:
	case 49:
	case 50:
		m_qiangguoIcon[i-47] = str;
		break;
	case 51:
	case 52:
	case 53:
	case 54:
		m_qiangguoNum[i-51] = str;
		break;
	case 55:
	case 56:
	case 57:
	case 58:
		m_qiangguoStep[i-55] = str;
		break;
	case 59:
	case 60:
	case 61:
	case 62:
		m_qiangguoGet[i-59] = str;
		break;
	case 63:
		m_telephoneIcon = str;
		break;
	case 64:
		m_telephoneGet = str;
		break;
	default:
		break;
	}
}

void crJXJ7DayHuodongCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crStaticTextWidgetNode > timeInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_timeInput));
		if (timeInput.valid())
			timeInput->clearString();
		ref_ptr<crStaticTextWidgetNode > huodongtitle = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_huodongtitle));
		if (huodongtitle.valid())
			huodongtitle->clearString();
		ref_ptr<crStaticTextWidgetNode > huodongprocess = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_huodongprocess));
		if (huodongprocess.valid())
			huodongprocess->clearString();
		ref_ptr<crStaticTextWidgetNode > content = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_contentInput));
		if (content.valid())
			content->clearString();
		ref_ptr<crButtonWidgetNode> getBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_getBtn));
		ref_ptr<crButtonWidgetNode> hadGottenBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_hadGottenBtn));
		if (hadGottenBtn.valid())
			hadGottenBtn->setVisiable(false);
		if (getBtn.valid())
			getBtn->setVisiable(true);
		ref_ptr<crStaticTextWidgetNode > itemNum[5] = {NULL};
		ref_ptr<crStaticTextWidgetNode > title[7] = {NULL};
		ref_ptr<crMultiSwitch> title_sw[7] = {NULL};
		ref_ptr<crImageBoxWidgetNode > image[5] = {NULL};
		ref_ptr<crImageBoxWidgetNode > qiangruo_image[4] = {NULL};

		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));

		
		for (unsigned char i = 0; i < 7; ++ i)
		{
			title[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title[i]));
			title_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_title_sw[i]));
			if (title_sw[i].valid())
			{
				title_sw[i]->setActiveSwitchSet(0);
			}
			if (title[i].valid())
			{
				title[i]->clearString();
			}
			if (i < 5)
			{
				image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox[i]));
				if (image[i].valid())
					image[i]->setImageName(m_nullImage);
				itemNum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_num[i]));
				if (itemNum[i].valid())
				{
					itemNum[i]->clearString();
				}
			}
		}
		ref_ptr<crMultiSwitch> jihuoma_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_code_sw));
		do 
		{
			crData * canvasData = m_this->getDataClass();
			if (!canvasData) break;
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if (false == huodongTab.valid()) break;
			crTableIO::StrVec huoRecord;
			int awardIdx[5] ;
			int titleIdx = huodongTab->getTitleIndex("标题");
			int titlexianshiIdx = huodongTab->getTitleIndex("标题显示");
			int contentIdx = huodongTab->getTitleIndex("内容");
			int startTimeIdx = huodongTab->getTitleIndex("开始时间");
			int endTimeIdx = huodongTab->getTitleIndex("结束时间");
			int idIdx = huodongTab->getTitleIndex("id");
			int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");
			int opendayIdx = huodongTab->getTitleIndex("开服天数");
			int conditionIndex = huodongTab->getTitleIndex("条件");
			int displayIndex = huodongTab->getTitleIndex("活动显示");

			awardIdx[0] = huodongTab->getTitleIndex("奖励1");
			awardIdx[1] = huodongTab->getTitleIndex("奖励2");
			awardIdx[2] = huodongTab->getTitleIndex("奖励3");
			awardIdx[3] = huodongTab->getTitleIndex("奖励4");
			awardIdx[4] = huodongTab->getTitleIndex("奖励5");

			if (titleIdx < 0) break;
			if (titlexianshiIdx < 0) break;
			
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if (false == itemtab.valid()) break;
			crTableIO::StrVec itemRecord;
			int iconIdx = itemtab->getTitleIndex("icon");
			int itemIdIdx = itemtab->getTitleIndex("itemid");
			CRCore::crVector4i awardVec;

			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			void *param;

			canvasData->getParam(WCHDATA_JXJHuodongVec,param);
			HuodongVec & huodongVec = *(HuodongVec *)param;
			
			int huodongID = 0;
			canvasData->getParam(WCHDATA_JXJHuodongSelectId	,param);
			unsigned short selectID = *(unsigned short *)param;

			int scrollPerpageNum = 7;
			int curPage = 0;
			if (scroll.valid())
			{
				scroll->setLineValue(1.0);
				scroll->setPageValue(1.0);
				if (huodongVec.size() > scrollPerpageNum)
				{
					scroll->setRange(0,huodongVec.size() - scrollPerpageNum);
					scroll->setEnable(true);
					curPage = scroll->getValue();
				}
				else 
				{
					scroll->setEnable(false);
				}
			}
			if (huodongVec.empty())
			{
				canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				break;
			}

			if (huodongVec.size() > selectID)
			{
				huodongID = huodongVec[selectID];
			}
			//else selectID = 0;

			unsigned char vecSize = huodongVec.size();
			unsigned char j = 0;
			std::string huodongtitlestr;
			for (unsigned char i = curPage; i < vecSize && j < 7; ++ i, ++ j)
			{
				if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongVec[i]),huoRecord) >= 0)
				{
					title[j]->setString(huoRecord[titlexianshiIdx]);
					if (huodongVec[i] == huodongID && huodongtitle.valid())
					{
						huodongtitlestr = huoRecord[titleIdx];
						huodongtitle->setString(huoRecord[titlexianshiIdx]);	
					}
				}
				if (huodongVec[i] == huodongID)
				{
					if (title_sw[j].valid())
					{
						title_sw[j]->setActiveSwitchSet(1);
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
			if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID),huoRecord) < 0) break;

			//切页
			if (jihuoma_sw.valid())
			{
				if ( huodongID == Activation_CodeActivity)
				{
					jihuoma_sw->setActiveSwitchSet(1);
					ref_ptr<crEditWidgetNode> codeText = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_codeText));
					std::string text("在此输入激活码序列号");
					if (codeText.valid())
					{
						codeText->clearString();
						codeText->setString(text);
					}
					break;
				}
				else if ( huodongID == Activation_WeakShili)
				{//弱国奖励
					jihuoma_sw->setActiveSwitchSet(3);
					for (unsigned char i = 0; i < 5; ++ i)
					{
						crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
						setRewardInfo(awardVec,image[i].get(),itemNum[i].get());
					}
					break;
				}
				else if ( huodongID == Activation_Telephone)
				{//手机绑定
					jihuoma_sw->setActiveSwitchSet(4);
					ref_ptr<crImageBoxWidgetNode> telephoneIcon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_telephoneIcon));
					ref_ptr<crButtonWidgetNode> telephoneGet = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_telephoneGet));
					crArgumentParser::appAtoVec(huoRecord[awardIdx[0]],awardVec);
					setRewardInfo(awardVec,telephoneIcon.get());
					//检查完成进度，设置领取按钮状态
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					ref_ptr<crRole> mainRole = myPlayer->getMainRole();
					if(mainRole.valid() && mainRole->getDataClass())
					{
						CRCore::crVector3i conditionVec;
						crArgumentParser::appAtoVec(huoRecord[conditionIndex],conditionVec);
						crData *roleData = mainRole->getDataClass();
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *huodongMap= (NewHuodongMap *)param;
						NewHuodongMap::iterator hitr = huodongMap->find(huodongID);
						if (hitr != huodongMap->end())
						{
							if(!hitr->second->getComplete())
							{
								if (hitr->second->isCanAdvance())
								{
									telephoneGet->setVisiable(true);
									telephoneGet->setEnable(true);
								}
								else
								{	
									telephoneGet->setVisiable(true);
									telephoneGet->setEnable(false);
								}
							}
							else
								telephoneGet->setEnable(false);
						}
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					break;
				}
				else if (atoi(huoRecord[displayIndex].c_str()) == Huodong_Festival)
				{//节日类型活动
					jihuoma_sw->setActiveSwitchSet(2);
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					if (myPlayer && myPlayer->getDataClass())
					{
						crData * playerData = myPlayer->getDataClass();
						if (playerData)
						{
							int Cricles = 0;
							ref_ptr<crButtonWidgetNode>   festival_Button[3] = {NULL};
							ref_ptr<crMultiSwitch> festival_buttonSwitch[3] = {NULL};
							ref_ptr<crStaticTextWidgetNode> festival_Introduction = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_festival_Introduction));
							ref_ptr<crStaticTextWidgetNode> festival_Deadline = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_festival_Deadline));

							//活动内容介绍
							if (festival_Introduction.valid())
							{
								festival_Introduction->setString(huoRecord[contentIdx]);
							}

							//设置活动持续时间
							typedef std::vector<int> Vec5i;
							Vec5i startTimeVec5i;
							Vec5i endTimeVec5i;
							startTimeVec5i.reserve(5);
							endTimeVec5i.reserve(5);

							crArgumentParser::appAtoVec(huoRecord[startTimeIdx],startTimeVec5i);
							crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endTimeVec5i);

							startTimeVec5i.resize(5,0);
							endTimeVec5i.resize(5,0);
							endTimeVec5i[3] = 24;

							char startTimeStr[64];
							memset(startTimeStr,0,sizeof(startTimeIdx));
							sprintf_s(startTimeStr,"%d.%02d.%02d %02d:%02d - %d.%02d.%02d %02d:%02d\0",
								startTimeVec5i[0],startTimeVec5i[1],startTimeVec5i[2],startTimeVec5i[3],startTimeVec5i[4],
								endTimeVec5i[0],endTimeVec5i[1],endTimeVec5i[2],endTimeVec5i[3],endTimeVec5i[4]);

							if (festival_Deadline.valid())
							{
								festival_Deadline->setString(std::string(startTimeStr));
							}

							//设置奖品信息
							crTableIO::StrVec festivalRecord;
							//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
							//if (false == itemtab.valid()) break;
							//crTableIO::StrVec itemRecord;
							//int iconIdx = itemtab->getTitleIndex("icon");
							//int itemIdIdx = itemtab->getTitleIndex("itemid");

							//CRCore::crVector3i awardVec;
							for (unsigned char i = 0; i < 3; ++ i)
							{
								festival_Button[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_festival_Button[i]));
								//festival_buttonSwitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_festival_buttonSwitch[i]));
								if (festival_Button[i].valid())
								{//领取按钮默认隐藏
									festival_Button[i]->setVisiable(false);
								}
								if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID + 1 + i),festivalRecord) >= 0)
								{//分别设置三个节日对应的奖励物品(每个节日对应一件item)
									ref_ptr<crImageBoxWidgetNode> festival_Icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_festival_Icon[i]));
									crArgumentParser::appAtoVec(festivalRecord[awardIdx[0]],awardVec);
									setRewardInfo(awardVec,festival_Icon.get());
									//if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(awardVec[0]),itemRecord) >= 0)
									//{
									//	if (festival_Icon.valid())
									//		festival_Icon->setImageName(itemRecord[iconIdx]);
									//}
								}
							}

							//检查完成进度，设置领取按钮状态
							ref_ptr<crRole> mainRole = myPlayer->getMainRole();
							if(mainRole.valid() && mainRole->getDataClass())
							{
								CRCore::crVector3i conditionVec;
								crArgumentParser::appAtoVec(huoRecord[conditionIndex],conditionVec);
								crData *roleData = mainRole->getDataClass();
								roleData->excHandle(MAKEINT64(WCH_LockData,1));
								roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
								NewHuodongMap *huodongMap= (NewHuodongMap *)param;
								for (unsigned char i = 0; i < 3 && festival_Button[i].valid(); ++i)
								{
									NewHuodongMap::iterator hitr = huodongMap->find(huodongID + 1 + i);
									if (hitr != huodongMap->end())
									{
										if(hitr->second->isCanAdvance())
										{
											if (!hitr->second->getComplete())
											{
												festival_Button[i]->setVisiable(true);
												festival_Button[i]->setEnable(true);
											}
											else
											{	
												festival_Button[i]->setVisiable(true);
												festival_Button[i]->setEnable(false);
											}
										}
										else
											festival_Button[i]->setVisiable(false);
									}
								}
								roleData->excHandle(MAKEINT64(WCH_LockData,0));
							}
						}
					}
					break;
				}
				else if (atoi(huoRecord[displayIndex].c_str()) == Huodong_Qiangguo)
				{//强国之路，国家占领城池奖励
					jihuoma_sw->setActiveSwitchSet(5);
					ref_ptr<crStaticTextWidgetNode > chengchiNum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_chengchiNum));
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					if (myPlayer && myPlayer->getDataClass())
					{
						crData * playerData = myPlayer->getDataClass();
						playerData->excHandle(MAKEINT64(WCH_LockData,1));
						playerData->getParam(WCHDATA_JXJShiliID,param);
						unsigned char shiliid = *(unsigned char *)param;
						playerData->getParam(WCHDATA_JXJChengChiMap,param);
						ChengchiMap *map_city = (ChengchiMap *)param;
						ref_ptr<crTableIO>tab_city = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
						ref_ptr<crTableIO> foundCountryCityTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);
						int chengchitype = tab_city->getTitleIndex("类型");
						crTableIO::StrVec chengchiRec,foundRec;
						int chengchicount = 0;
						for( ChengchiMap::iterator itr = map_city->begin(); itr != map_city->end(); ++itr)
						{
							if(shiliid == itr->second->getChengzhuShili())
							{
								if (tab_city->queryOneRecord(0,crArgumentParser::appItoa((int)itr->first),chengchiRec)>=0)
								{
									if(chengchiRec[chengchitype] == "1")
									{//城池个数,去除关隘和建国城池
										if(foundCountryCityTab->queryOneRecord(0,chengchiRec[0],foundRec)<0)
											chengchicount++;
									}
								}
							}
						}
						playerData->excHandle(MAKEINT64(WCH_LockData,0));
						if(chengchiNum.valid())
						{
							chengchiNum->setString(crArgumentParser::appItoa(chengchicount));
						}
						//设置奖品信息
						crTableIO::StrVec qrec;
						ref_ptr<crButtonWidgetNode> QgetBut[4];
						ref_ptr<crImageBoxWidgetNode> Qicon[4];
						ref_ptr<crStaticTextWidgetNode> QNum[4];
						ref_ptr<crStaticTextWidgetNode> QStep[4];
						std::string step;
						for (int i = 0; i < 4; ++ i)
						{
							QgetBut[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_qiangguoGet[i]));
							Qicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_qiangguoIcon[i]));
							QNum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_qiangguoNum[i]));
							QStep[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_qiangguoStep[i]));
							if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID + 1 + i),qrec) >= 0)
							{//分别设置四个攻占城池对应的奖励物品(每个对应一件item)
								crArgumentParser::appAtoVec(qrec[awardIdx[0]],awardVec);
								setRewardInfo(awardVec,Qicon[i].get(),QNum[i].get());
								step = qrec[conditionIndex]+"座";
								QStep[i]->setString(step);
							}
						}
						//检查完成进度，设置领取按钮状态
						ref_ptr<crRole> mainRole = myPlayer->getMainRole();
						if(mainRole.valid() && mainRole->getDataClass())
						{
							crData *roleData = mainRole->getDataClass();
							roleData->excHandle(MAKEINT64(WCH_LockData,1));
							roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
							NewHuodongMap *huodongMap= (NewHuodongMap *)param;
							for (int i = 0; i < 4 && QgetBut[i].valid(); ++i)
							{
								NewHuodongMap::iterator hitr = huodongMap->find(huodongID + 1 + i);
								if (hitr != huodongMap->end())
								{
									if(!hitr->second->getComplete())
									{
										if (hitr->second->isCanAdvance())
										{
											QgetBut[i]->setVisiable(true);
											QgetBut[i]->setEnable(true);
										}
										else
										{	
											QgetBut[i]->setVisiable(true);
											QgetBut[i]->setEnable(false);
										}
									}
									else
										QgetBut[i]->setVisiable(false);
								}
							}
							roleData->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
					break;
				}
				jihuoma_sw->setActiveSwitchSet(0);			
			}

			typedef std::vector<int> Vec5i;
			Vec5i startTimeVec5i;
			Vec5i endTimeVec5i;
			startTimeVec5i.reserve(5);
			endTimeVec5i.reserve(5);

			CRCore::crVector2i opendayVec2i;
			crArgumentParser::appAtoVec(huoRecord[opendayIdx],opendayVec2i);
			if (opendayVec2i[0] != 0)
			{
				std::string dateStr = crGlobalHandle::gData()->gStartDate();
				crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
				startTimeVec5i.resize(5,0);
				time_t nowTime_t = time(0);
				struct  tm  nowTm = *localtime(&nowTime_t);
				struct tm  startTm;
				memset(&startTm,0,sizeof (startTm));
				startTm.tm_year = startTimeVec5i[0] - 1900;
				startTm.tm_mon = startTimeVec5i[1] - 1;
				startTm.tm_mday = startTimeVec5i[2];
				startTm.tm_hour = startTimeVec5i[3];
				startTm.tm_min = startTimeVec5i[4];
				startTm.tm_isdst = nowTm.tm_isdst;
				time_t startTime_t = mktime(&startTm);
				char tmp1[20];
				char tmp2[20];
				time_t t1 = 0;
				time_t t2 = 0;
				if (opendayVec2i[1] == 0)
				{
					t1 = startTime_t + (opendayVec2i[0] - 1) * c_24hours;
					strftime( tmp1, sizeof(tmp1), "%Y|%m|%d\0", localtime(&t1) );
					t2 = startTime_t + (opendayVec2i[0] - 1) * c_24hours;
					strftime( tmp2, sizeof(tmp2), "%Y|%m|%d\0", localtime(&t2) );
				}
				else
				{
					t1 = startTime_t + (opendayVec2i[0] - 1) * c_24hours;
					strftime( tmp1, sizeof(tmp1), "%Y|%m|%d\0", localtime(&t1) );
					t2 = startTime_t + (opendayVec2i[1] - 1) * c_24hours;
					strftime( tmp2, sizeof(tmp2), "%Y|%m|%d\0", localtime(&t2) );
				}
				crArgumentParser::appAtoVec(tmp1,startTimeVec5i);
				crArgumentParser::appAtoVec(tmp2,endTimeVec5i);
			}
			else
			{
				crArgumentParser::appAtoVec(huoRecord[startTimeIdx],startTimeVec5i);
				crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endTimeVec5i);
			}

			startTimeVec5i.resize(5,0);
			endTimeVec5i.resize(5,0);
			endTimeVec5i[3] = 24;

			char startTimeStr[64];
			memset(startTimeStr,0,sizeof(startTimeIdx));
			sprintf_s(startTimeStr,"%d.%02d.%02d %02d:%02d - %d.%02d.%02d %02d:%02d\0",
				startTimeVec5i[0],startTimeVec5i[1],startTimeVec5i[2],startTimeVec5i[3],startTimeVec5i[4],
				endTimeVec5i[0],endTimeVec5i[1],endTimeVec5i[2],endTimeVec5i[3],endTimeVec5i[4]);

			if (timeInput.valid())
			{
				timeInput->setString(std::string(startTimeStr));
			}
			if (content.valid())
			{
				content->setString(huoRecord[contentIdx]);
			}
			for (unsigned char i = 0; i < 5; ++ i)
			{
				crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
				setRewardInfo(awardVec,image[i].get(),itemNum[i].get());
			}
			//CRCore::crVector3i awardVec;
			//for (unsigned char i = 0; i < 5; ++ i)
			//{
			//	crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
			//	if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(awardVec[0]),itemRecord) >= 0)
			//	{
			//		if (image[i].valid())
			//			image[i]->setImageName(itemRecord[iconIdx]);
			//		if (awardVec[2] > 1)
			//		{
			//			if (itemNum[i].valid())
			//				itemNum[i]->setString(crArgumentParser::appItoa(awardVec[2]));
			//		}
			//	}
			//	
			//}
			if (huodongprocess.valid())
			{
				crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
				ref_ptr<crRole> mainRole = playerData->getMainRole();
				if(mainRole.valid() && mainRole->getDataClass() )
				{
					crData *roleData = mainRole->getDataClass();
					roleData->excHandle(MAKEINT64(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap::iterator hitr = huodongMap->find(huodongID);
					if (hitr != huodongMap->end())
					{
						if(hitr->second->getStepCount() > 0)
						{
							if (huodongtitlestr == "征战天下")
							{
								if(hitr->second->getStep() > 0)
								{
									m_3starstimes = hitr->second->getStep();
								}
								huodongprocess->setString("通关详情:" + crArgumentParser::appItoa(m_3starstimes) + "/" 
									+ "70");
							}
							else
							{
								huodongprocess->setString("通关详情:" + crArgumentParser::appItoa(hitr->second->getStep()) + "/" 
									+ crArgumentParser::appItoa(hitr->second->getStepCount()));
							}
						}
					}
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

			if (getBtn.valid())
			{
				getBtn->setEnable(true);
			}
		} while (0);
	}
}

/////////////////////////////////////////
//
//crJXJRecvUpdateJoinWarTimesMethod
// 7日征兵活动刷新
/////////////////////////////////////////
crJXJUpdate7DayHuoDongMethod::crJXJUpdate7DayHuoDongMethod():
	m_this(NULL),
	m_type(-1),
	m_chapterid(-1)
{

}
crJXJUpdate7DayHuoDongMethod::crJXJUpdate7DayHuoDongMethod( const crJXJUpdate7DayHuoDongMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_type(-1),
	m_chapterid(-1)
{
}
void crJXJUpdate7DayHuoDongMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_type = LOINT64(param64);
			m_chapterid = HIINT64(param64);
		}
		else
		{
			m_chapterid = -1;
			m_type = -1;
		}
		break;
	}

}

void crJXJUpdate7DayHuoDongMethod::addParam( int i, const std::string& str )
{

}

void crJXJUpdate7DayHuoDongMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *mainroledata = m_this->getMainRole()->getDataClass();
			mainroledata->excHandle(MAKEINT64(WCH_LockData,1));
			/////////////活动update
			mainroledata->getParam(WCHDATA_JXJNewHuodongMap,param);
			NewHuodongMap huodongMap = *(NewHuodongMap *)param;
			mainroledata->excHandle(MAKEINT64(WCH_LockData,0));
			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			int _index = huodongTab->getTitleIndex("标题");
			int idIdx = huodongTab->getTitleIndex("id");
			crTableIO::StrVec huoRecord;
			std::string text("");
			switch (m_type)
			{
			case 0:
				text = "征战天下";
				break;
			case 1:
				text = "名将之路";
				break;
			case 2:
				text = "攻城掠地";
				break;
			case 3:
				text = "血战沙场";
				break;
			case 4:
				text = "视死如归";
				break;
			default:
				break;
			}
			crTableIO::DataVec huodongIDVec;
			NewHuodongMap ::iterator itr = huodongMap.end();
			huodongTab->queryRecords(_index,text,huodongIDVec);
			if (!huodongIDVec.empty()) 
			{	
				int huodongid = 0;
				for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				{
					huodongid = atoi((*siter)[idIdx].c_str());
					itr = huodongMap.find(huodongid);	
					if(itr != huodongMap.end())
					{
						if(itr->second->isActivation())
						{	
							if (m_type == 0)
							{
								itr->second->doEvent(WCH_JXJUpdate7DayHuoDong,MAKEINT64(NULL,m_this->getPlayerID()));
							}
							else if (m_type > 0 && m_type <3)
							{
								itr->second->doEvent(WCH_JXJUpdate7DayHuoDong,MAKEINT64(NULL,m_this->getPlayerID()));
							}
							else if(m_type >= 3 && m_type < 5)
							{
								crData *data = m_this->getDataClass();
								data->excHandle(MAKEINT64(WCH_LockData,1));
								data->getParam(WCHDATA_JXJShiliID,param);
								unsigned char shiliid = *(unsigned char *)param;
								int deadcounts = 0;
								int killedcounts = 0;
								if (shiliid >= c_startShiliID)
								{
									crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
									if (gsBrainData)
									{
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
										gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
										crJXJShili *shili = (crJXJShili *)param;
										if (shili && shili->getDataClass())
										{
											crData *shilidata = shili->getDataClass();	
											shilidata->excHandle(MAKEINT64(WCH_LockData,1));
											shilidata->getParam(WCHDATA_JXJPeopleMap,param);
											PeopleMap *peoplemap = (PeopleMap *)param;
											PeopleMap::iterator peoitr = peoplemap->find(playerid);
											if (peoitr != peoplemap->end() && peoitr->second.valid())
											{
												deadcounts = peoitr->second->getDeadTroopsNum();
												killedcounts = peoitr->second->getKillNum();
											}
								
											shilidata->excHandle(MAKEINT64(WCH_LockData,0));
										}
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									}
								}
								data->excHandle(MAKEINT64(WCH_LockData,0));
								if (m_type == 3)
								{
									itr->second->doEvent(WCH_JXJUpdate7DayHuoDong,MAKEINT64(killedcounts,m_this->getPlayerID()));
								}
								else if(m_type == 4)
								{
									itr->second->doEvent(WCH_JXJUpdate7DayHuoDong,MAKEINT64(deadcounts,m_this->getPlayerID()));
								}
							}
						}
					}
				}

			}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseKaoShangFuMethod
//
/////////////////////////////////////////
crJXJServerUseKaoShangFuMethod::crJXJServerUseKaoShangFuMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseKaoShangFuMethod::crJXJServerUseKaoShangFuMethod(const crJXJServerUseKaoShangFuMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseKaoShangFuMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseKaoShangFuMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseKaoShangFuMethod::operator()(crHandle &handle)
{
	int playerid = m_user->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (playerData.valid())
	{
		crData *data = m_user->getDataClass();
		time_t t = time(0);
		if (data)
		{
			data->inputParam(WCHDATA_JXJKaoShangFuUsedTime,&t);
			*m_useResult = 1;//使用成功
		}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(8);
		stream->_writeInt64(t);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvKaoShangFuUsedTime,stream.get());
		gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		
	}
}

/////////////////////////////////////////
//
//crJXJRecvKaoShangFuUsedTimeMethod
//
/////////////////////////////////////////
crJXJRecvKaoShangFuUsedTimeMethod::crJXJRecvKaoShangFuUsedTimeMethod():
	m_netType(GameClient_Game){}
crJXJRecvKaoShangFuUsedTimeMethod::crJXJRecvKaoShangFuUsedTimeMethod(const crJXJRecvKaoShangFuUsedTimeMethod& handle):
	m_tipcanvas(handle.m_tipcanvas),
	m_tipcontent(handle.m_tipcontent)
{
}
void crJXJRecvKaoShangFuUsedTimeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvKaoShangFuUsedTimeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tipcanvas = str;
		break;
	case 1:
		m_tipcontent = str;
		break;
	default:
		break;
	}
}

void crJXJRecvKaoShangFuUsedTimeMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			_crInt64 t = m_stream -> _readInt64();
			std::string tipstr/* = "犒赏符使用成功,在接下来的2小时内，您在战场中进行战场犒赏时将不消耗元宝。"*/;

			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			std::string str;
			int textid = 5034;		//犒赏符使用成功,在接下来的2小时内，您在战场中进行战场犒赏时将不消耗元宝。
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),record)>=0)
			{
				tipstr = record[1];
			}

			crData *data = m_this->getDataClass();
			if (data)
			{
				data->inputParam(WCHDATA_JXJKaoShangFuUsedTime,&t);
				ref_ptr<crCanvasNode>tipcanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipcanvas);
				if (tipcanvas.valid())
				{
					ref_ptr<crStaticTextWidgetNode> tipcontent = dynamic_cast<crStaticTextWidgetNode *>(tipcanvas->getWidget(m_tipcontent));
					if (tipcontent.valid())
					{
						tipcontent->setString(tipstr);
					}
					crFilterRenderManager::getInstance()->showCanvas(tipcanvas.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseQieYeFuMethod
//
/////////////////////////////////////////
crJXJServerUseQieYeFuMethod::crJXJServerUseQieYeFuMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseQieYeFuMethod::crJXJServerUseQieYeFuMethod(const crJXJServerUseQieYeFuMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseQieYeFuMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseQieYeFuMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseQieYeFuMethod::operator()(crHandle &handle)
{
	int playerid = m_user->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (playerData.valid() && m_user->getMainRole())
	{
		crData *data = m_user->getDataClass();
		crData* RoleData = m_user->getMainRole()->getDataClass();
		if (data && RoleData)
		{
			void *param;
			//data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = 15;
			RoleData->getParam(WCHDATA_JXJSavedEquipVecs,param);
			unsigned char opensaveequipnifo = *(unsigned char *)param;
			int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());
			if (opensaveequipnifo >= maxpage)
			{
				//装备页已达最大
				*m_useResult = USE_QieYeFu_Error;
			}
			else
			{
				opensaveequipnifo += 1;
				unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,viplv).c_str()));
				RoleData->inputParam(WCHDATA_JXJSavedEquipVecs,&opensaveequipnifo);
				RoleData->excHandle(MAKEINT64(WCH_LockData,1));
				RoleData->getParam(WCHDATA_JXJLordEquipVecs, param);
				std::vector<JXJLordEquipVec> *openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
				openedEquipVec->resize(opensaveequipnifo);
				JXJLordEquipVec equipVec;
				equipVec.resize(equiponmax);
				(*openedEquipVec)[opensaveequipnifo-1] = equipVec;
				RoleData->excHandle(MAKEINT64(WCH_LockData,0));
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
				callback->savePlayerData(playerData.get());
				*m_useResult = 1;//使用成功
			}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			if (*m_useResult == 1)
			{
				stream->createBuf(2);
				stream->_writeChar(*m_useResult);
				stream->_writeUChar(opensaveequipnifo);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuySavedEquipPage,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			//else
			//{
			//	stream->createBuf(1);
			//	stream->_writeChar(*m_useResult);
			//}
			///游戏日志
			std::string logdata = crArgumentParser::appItoa(*m_useResult);
			GameLogData gamelog(Log_BuySavedEquipPage,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));

		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseXuLieFuMethod
//
/////////////////////////////////////////
crJXJServerUseXuLieFuMethod::crJXJServerUseXuLieFuMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseXuLieFuMethod::crJXJServerUseXuLieFuMethod(const crJXJServerUseXuLieFuMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseXuLieFuMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseXuLieFuMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseXuLieFuMethod::operator()(crHandle &handle)
{
	int playerid = m_user->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (playerData.valid() )
	{
		crData *data = m_user->getDataClass();
		if (data)
		{
			void *param;
			//data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = 15;
			int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,viplv).c_str());
			//data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJOpenSaveFormationInfo,param);
			unsigned char opensaveformationifo = *(unsigned char *)param;
			if (opensaveformationifo >= maxpage)
			{
				//编队切页已达最大
				*m_useResult = USE_QieYeFu_Error;		
			}
			else
			{
				opensaveformationifo += 1;
				data->inputParam(WCHDATA_JXJOpenSaveFormationInfo,&opensaveformationifo);
				data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
				SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
				savedformationvec->resize(opensaveformationifo);
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
				callback->savePlayerData(playerData.get());	
				*m_useResult = 1;//使用成功
			}
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			if (*m_useResult == 1)
			{
				stream->createBuf(2);
				stream->_writeChar(*m_useResult);
				stream->_writeUChar(opensaveformationifo);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuySavedFormationPage,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			//data->excHandle(MAKEINT64(WCH_LockData,0));
			///游戏日志
			std::string logdata = crArgumentParser::appItoa(*m_useResult);
			GameLogData gamelog(Log_BuySavedEquipPage,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));

		}
	}
}

/////////////////////////////////////////
//
//crJXJRefreshZhengZhanTianHuoDongStepsMethod
//
/////////////////////////////////////////
crJXJRefreshZhengZhanTianHuoDongStepsMethod::crJXJRefreshZhengZhanTianHuoDongStepsMethod()
{
}

crJXJRefreshZhengZhanTianHuoDongStepsMethod::crJXJRefreshZhengZhanTianHuoDongStepsMethod(const crJXJRefreshZhengZhanTianHuoDongStepsMethod& handle):
	crMethod(handle)
{
}

void crJXJRefreshZhengZhanTianHuoDongStepsMethod::inputParam(int i, void *param)
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
	}
}

void crJXJRefreshZhengZhanTianHuoDongStepsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRefreshZhengZhanTianHuoDongStepsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvZhengZhanTianHuoDongSteps,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}


/////////////////////////////////////////
//
//crJXJRecvZhengZhanTianHuoDongStepsMethod
//
/////////////////////////////////////////
crJXJRecvZhengZhanTianHuoDongStepsMethod::crJXJRecvZhengZhanTianHuoDongStepsMethod()
{
}

crJXJRecvZhengZhanTianHuoDongStepsMethod::crJXJRecvZhengZhanTianHuoDongStepsMethod(const crJXJRecvZhengZhanTianHuoDongStepsMethod& handle):
	crMethod(handle)
{
}

void crJXJRecvZhengZhanTianHuoDongStepsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvZhengZhanTianHuoDongStepsMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvZhengZhanTianHuoDongStepsMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			//int playerid = m_this->getPlayerID();
			//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			//if (playerData.valid())
			//{	
				int _type = 0;	//征战天下活动
				m_this->doEvent(WCH_JXJUpdate7DayHuoDong,MAKEINT64(_type,NULL));
				//crPlayerDataEventPacket packet;
				//crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvZhengZhanTianHuoDongSteps,NULL);
				//gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);	
			//}
		}
	}
}



//////////////////////////////////////////////////////////////////////////
//crJXJUISpecialtyRefreshMethod
//////////////////////////////////////////////////////////////////////////
crJXJUISpecialtyRefreshMethod::crJXJUISpecialtyRefreshMethod()
{}
crJXJUISpecialtyRefreshMethod::crJXJUISpecialtyRefreshMethod(const crJXJUISpecialtyRefreshMethod& handle)
{

}
void crJXJUISpecialtyRefreshMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUISpecialtyRefreshMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}
void crJXJUISpecialtyRefreshMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	crMyPlayerData *me = crMyPlayerData::getInstance();
	int RefreshCost = 0;
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJSpecialtyRefreshTimes,param);
			unsigned char times = *(unsigned char *)param;
			ref_ptr<crTableIO>  SpecialtyRefreshTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSpecialtyRefreshTab);
			crTableIO::StrVec record;
			if (SpecialtyRefreshTab.valid())
			{
				int timesIdx = SpecialtyRefreshTab->getTitleIndex("次数");
				int costIdx = SpecialtyRefreshTab->getTitleIndex("消耗元宝");
				if (timesIdx >=0 && costIdx >= 0)
				{
					if (SpecialtyRefreshTab->queryOneRecord(timesIdx,crArgumentParser::appItoa(times + 1),record) >= 0)
					{
						RefreshCost = atoi(record[costIdx].c_str());
					}
				}
			}
		}
	}
	if(!msgCanvas)return;
	std::string strInfo ;
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	if(RefreshCost>0)
	{
		int hinttype = JXJGoldCostType_SpecialtyRefresh;
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

		crData * data = me->getPlayerGameData()->getDataClass();
		data->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_SpecialtyRefresh;
		if (ifhint)
		{
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text","您确认要花费");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(140.0);
			v_i.push_back(40.0);
			v_i.push_back(40.0);
			v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			strInfo=crArgumentParser::appItoa(RefreshCost) + std::string("礼金/元宝") ;
			cfg_script.Add("Text",strInfo);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			strInfo = crArgumentParser::appItoa(RefreshCost);
			cfg_script.Add("Text","刷新特产吗？");
			cfg_script.Pop();

			cfg_script.Pop();
			msg->setHypertext(cfg_script);
		}
		ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
		crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		crData *dlgData = msgCanvas->getDataClass();
		int commandtype = CDP_Widget;
		dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
		dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
		if (ifhint)
		{
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{
			if(okBtn.valid())okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
		}
	}
}

/////////////////////////////////////////
//
//crJXJBaoXiangProgressFrameUpdateMethod
//
/////////////////////////////////////////
crJXJBaoXiangProgressFrameUpdateMethod::crJXJBaoXiangProgressFrameUpdateMethod():
	m_dt(NULL)
{
}
crJXJBaoXiangProgressFrameUpdateMethod::crJXJBaoXiangProgressFrameUpdateMethod(const crJXJBaoXiangProgressFrameUpdateMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_progress(handle.m_progress)
{
}
void crJXJBaoXiangProgressFrameUpdateMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
		}
		break;
	}
}
void crJXJBaoXiangProgressFrameUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_progress = str;
		break;
	}
}
void crJXJBaoXiangProgressFrameUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crProgressWidgetNode> progress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_progress));
	crData *canvasdata = m_this->getDataClass();
	if (progress.valid() && canvasdata)
	{
		canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJTouchItem,param);
		crInstanceItem *touchitem = (crInstanceItem *)param;
		if(m_curItem != touchitem)
		{
			m_curItem = touchitem;
			canvasdata->getParam(WCHDATA_JXJTouchInterval,param);
			float interval = *(float *)param;
			m_speed = 1.0f/interval;
			progress->setProgress(0.0f);
		}
		float newpro = progress->getProgress()+(*m_dt) * m_speed;
		progress->setProgress(newpro);
		if(newpro>=1.0f)
		{
			crFilterRenderManager::getInstance()->showCanvas(m_this,false);
			crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor && myplayer)
			{
				if (m_curItem.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(myplayer->getPlayerID());//
					crPlayerServerEventPacket packet;
					crPlayerServerEventPacket::buildRequestPacket(packet,WCH_RecvTouch,m_curItem.get(),stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
					canvasdata->inputParam(WCHDATA_JXJTouchItem,NULL);
					if(m_curItem->getDataClass())
					{
						crData *itemData = m_curItem->getDataClass();
						unsigned char itemState = IS_Dead;
						itemData->inputParam(WCHDATA_ItemState,&itemState);
					}
					m_curItem = NULL;
				}
			}
		}
		canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
	else
	{
		crFilterRenderManager::getInstance()->showCanvas(m_this,false);
	}
}

/////////////////////////////////////////
//
//crJXJTouchSceneItemProgressMethod
//
/////////////////////////////////////////
crJXJTouchSceneItemProgressMethod::crJXJTouchSceneItemProgressMethod():
m_interval(2.0f){}
crJXJTouchSceneItemProgressMethod::crJXJTouchSceneItemProgressMethod(const crJXJTouchSceneItemProgressMethod& handle):
	crMethod(handle),
	m_interval(handle.m_interval)
{
}
void crJXJTouchSceneItemProgressMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	}
}

void crJXJTouchSceneItemProgressMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJTouchSceneItemProgressMethod::operator()(crHandle &handle)
{
	void *param;
	crData *itemData = m_this->getDataClass();
	if(itemData)
	{
		itemData->excHandle(MAKEINT64(WCH_LockData,1));
		itemData->getParam(WCHDATA_ItemState,param);
		unsigned char *itemState = (unsigned char *)param;
		if(*itemState!=IS_Dead)
		{
			ref_ptr<crCanvasNode>progresscanvas = dynamic_cast<crCanvasNode*>(crFilterRenderManager::getInstance()->findCanvas("UI_bxjdt"));
			if (progresscanvas.valid() && progresscanvas->getDataClass())
			{
				crData *canvasdata = progresscanvas->getDataClass();
				canvasdata->inputParam(WCHDATA_JXJTouchInterval,&m_interval);
				canvasdata->inputParam(WCHDATA_JXJTouchItem,m_this);
				crFilterRenderManager::getInstance()->doModal(progresscanvas.get());
			}		
			//*itemState = IS_Dead;
		}
		itemData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJUpdateNewHuodongMapMethod
//
/////////////////////////////////////////
crJXJUpdateNewHuodongMapMethod::crJXJUpdateNewHuodongMapMethod()
{
}

crJXJUpdateNewHuodongMapMethod::crJXJUpdateNewHuodongMapMethod(const crJXJUpdateNewHuodongMapMethod& handle):
	crMethod(handle)
{
}

void crJXJUpdateNewHuodongMapMethod::inputParam(int i, void *param)
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
	}
}

void crJXJUpdateNewHuodongMapMethod::addParam(int i, const std::string& str)
{
}

void crJXJUpdateNewHuodongMapMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvNewHuodongMap,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvNewHuodongMapMethod
//
/////////////////////////////////////////
crJXJRecvNewHuodongMapMethod::crJXJRecvNewHuodongMapMethod():
	m_netType(GameClient_Game){}
crJXJRecvNewHuodongMapMethod::crJXJRecvNewHuodongMapMethod(const crJXJRecvNewHuodongMapMethod& handle):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_canvas(handle.m_canvas)
{
}
void crJXJRecvNewHuodongMapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvNewHuodongMapMethod::addParam(int i, const std::string& str)
{
	switch	(i)
	{
	case 0:
		m_mygroupcanvas = str;
		break;
	case 1:
		m_canvas = str;
		break;
	}
}

void crJXJRecvNewHuodongMapMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			ref_ptr<crRole> mainrole = m_this->getMainRole();
			if(mainrole.valid() && mainrole->getDataClass())
			{
				void *param;
				int playerid = m_this->getPlayerID();
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
				{
					crData *roleData = mainrole->getDataClass();
					roleData->excHandle(MAKEINT64(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJNewHuodongMap,param);
					NewHuodongMap *huodongMap = (NewHuodongMap *)param;
					if (huodongMap)
					{
						unsigned short size = huodongMap->size();
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(size * 8);
						stream->_writeUShort(size);
						unsigned short relsize = 0;
						for( NewHuodongMap::iterator hmItr = huodongMap->begin();
							hmItr != huodongMap->end(); 
							++hmItr )
						{
							if (hmItr->second.valid())
							{
								stream->_writeUShort(hmItr->second->getID());
								stream->_writeUChar(hmItr->second->getActivation());
								stream->_writeUChar(hmItr->second->getCircles());
								stream->_writeInt(hmItr->second->getStep());
								++relsize;
							}
							//hmItr->second->activationCheck(m_this->getPlayerID());
							//if(hmItr->second->getActivation()==1/* && !hmItr->second->isCanAdvance()*/)
							//{
							//	hmItr->second->doEvent(WCH_UPDATE,MAKEINT64(m_this,NULL));
							//}
						}
						int bufSize = stream->getBufSize();
						stream->seekBegin();
						stream->_writeUShort(relsize);
						stream->setBufSize(bufSize);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvNewHuodongMap,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			if (m_stream.valid())
			{
				unsigned short csize = m_stream->_readUShort();
				ref_ptr<crPlayerGameData> playerdata = crMyPlayerData::getInstance()->getPlayerGameData();
				if(playerdata.valid())
				{
					ref_ptr<crRole> mainrole = playerdata->getMainRole();
					if(mainrole.valid() && mainrole->getDataClass())
					{
						void *param;
						crData *roledata = mainrole->getDataClass();
						roledata->excHandle(MAKEINT64(WCH_LockData,1));
						roledata->getParam(WCHDATA_JXJNewHuodongMap,param);
						NewHuodongMap *huodongmap = (NewHuodongMap *)param;
						NewHuodongMap::iterator itr = huodongmap->end();
						unsigned short id = 0;
						unsigned char activation = 0;
						unsigned char circles = 0;
						int step = 0;
						for (unsigned short i = 0;i< csize; ++i)
						{
							id = m_stream->_readUShort();
							activation = m_stream->_readUChar();
							circles = m_stream->_readUChar();
							step = m_stream->_readInt();
							itr = huodongmap->find(id);
							if(itr != huodongmap->end() && itr->second.valid())
							{
								itr->second->setCircles(circles);
								itr->second->setActivation(activation,0);
								itr->second->setStep(step,0);
							}
						}
						roledata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIShuoMingTipInfoMethod
//
/////////////////////////////////////////
crJXJUIShuoMingTipInfoMethod::crJXJUIShuoMingTipInfoMethod():
	m_ea(NULL),
	m_this(NULL)
{
}
crJXJUIShuoMingTipInfoMethod::crJXJUIShuoMingTipInfoMethod(const crJXJUIShuoMingTipInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJUIShuoMingTipInfoMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
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
	}
}
void crJXJUIShuoMingTipInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_header = str;
		break;
	case 1:
		m_trunk = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_text = str;
		break;
	case 4:
		m_textbk = str;
		break;
	case 5:
		m_textdi = str;
		break;
	case 6:
		m_titleid = atoi(str.c_str());
		break;
	case 7:
		m_textid = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIShuoMingTipInfoMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> headerCanvas = crFilterRenderManager::getInstance()->findCanvas(m_header);
	ref_ptr<crCanvasNode> trunkCanvas = crFilterRenderManager::getInstance()->findCanvas(m_trunk);
	if (headerCanvas.valid() && trunkCanvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode > title = dynamic_cast<crStaticTextWidgetNode *>(headerCanvas->getWidget(m_title));
		ref_ptr<crHypertextWidgetNode> text = dynamic_cast<crHypertextWidgetNode *>(trunkCanvas->getWidget(m_text));
		if(!text.valid()) return;
		crMatrixTransform *textbk = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textbk));
		if(!textbk) return;
		crMatrixTransform *textdi = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textdi));
		if(!textdi) return;

		std::string titlestr;
		std::string notice;
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		if (noticeHandle)
		{
			ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_textid),record) >= 0)
			{
				notice = record[1];
			}
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_titleid),record) >= 0)
			{
				titlestr = record[1];
			}

			/*if (m_textid==0 && m_titleid==0)
			{
			titlestr = "国力值各参数";
			ref_ptr<crCanvasNode> lunhuiCanvas = m_this->getParentCanvas();
			if (lunhuiCanvas.valid()&&lunhuiCanvas->getDataClass())
			{
			void *param;
			int index = 0;
			std::string widgetname = m_this->getName();
			if(widgetname == "UI_Treason_Shu")
			index = 1;
			else if(widgetname == "UI_Treason_Wu")
			index = 2;

			ref_ptr<crData> canvasdata = lunhuiCanvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJCountryPowerComputeFormula,param);
			std::string *strformula = (std::string*)param;
			notice = strformula[index];
			}
			}*/
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content",1);
			cfg_script.Add("Text",notice + " /n");
			cfg_script.Pop();
			cfg_script.Pop();
			text->setHypertext(cfg_script);
		}
		if (title.valid())
		{
			title->setString(titlestr);
		}
		
		///////////计算缩放
		float boxYlength = textbk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
		text->updateData();
		const crBoundingBox &bbox = text->getTextObject()->getBoundBox();
		const crBoundingBox &bbox2 = text->getBoundBox();
		float texheight = 0.0f;
		float needmove = 0.0f;
		if(bbox.valid())
		{
			texheight = bbox.yLength();
			texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
			if(texheight<boxYlength) texheight = boxYlength;
			float scale = texheight/boxYlength;

			float needmove ;
			const crBoundingBox &tipsbb = trunkCanvas->getBoundBox();
			crVector3 centerTansTar = text->getBound().center();

			float tmpLength = bbox2.yLength();
			if(textdi)
			{
				float diYlength = textdi->getChild(0)->getBoundBox().yLength();
				centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
			}
			else
				centerTansTar[1] += 0.5f * (tmpLength - texheight);
			// 源中心点
			crVector3 centerTansSrc = textbk->getChild(0)->getBoundBox().center();
			// 将源中心点平移到原点
			crMatrix put_mat = crMatrix::translate(centerTansSrc);
			crMatrix inv_put = crMatrix::inverse(put_mat);
			crMatrix rotmat;
			rotmat.postMult(inv_put);
			rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
			rotmat.postMult(crMatrix::translate(centerTansTar));
			if(textbk)textbk->setMatrix(rotmat);

			if(textdi)
			{
				float diYlength = textdi->getChild(0)->getBoundBox().yLength();
				//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
				needmove = boxYlength - texheight + diYlength * 0.25f;
				//needmove = boxYlength - texheight;
				textdi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
			}
		}
		else
		{
			textbk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
			if(textdi) textdi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
		}
		///////////////////////////////////////
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		const crBoundingBox &iconbox = m_this->getBoundBox();
		const crBoundingBox &tips = trunkCanvas->getBoundBox();
		float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
		float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
		crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
		/////////////////////////////////////////
		crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
		pos = pos * mat;
		pos2 = pos2 * mat;
		float posx2,posy2;
		/////////////////////////////////////////                             
		if (pos2[0] >= 0)
		{
			posx2 = pos2[0] - posx;
		}
		else
		{
			posx2 = pos[0] + posx;
		}
		if (pos2[1] >= 0)
		{
			posy2 = pos[1] - posy;
		}
		else
		{
			posy2 = pos2[1] + 2.0f * posy - needmove;
		}
		if((abs(posy2)+posy)>1.0f)
		{
			if (posy2 >= 0)
			{
				posy2 -= posy;
			}
			else
			{
				posy2 += posy;
			}
		}
		crVector2 mouse(crVector2(posx2,posy2));
		//textbk->setMatrix(crMatrix::scale(1.0f,m_expandnum,1.0f) * crMatrix::translate(0,(textbk->getChild(0)->getBoundBox().yLength()*0.5f),0.0f));
		headerCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));//(crMatrix::translate(mouse[0],mouse[1]-posy*2.0f-trunkCanvas->getBoundBox().yLength()*0.5f,0.0f));

		/////////////////////////////////////////
		trunkCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
		trunkCanvas->setCanFocus(false);
		headerCanvas->setCanFocus(false);

		bool ifshowcanvas = true;

		crFilterRenderManager::getInstance()->showCanvas(headerCanvas.get(),true);
		crFilterRenderManager::getInstance()->showCanvas(trunkCanvas.get(),true);
	}
}


/////////////////////////////////////////
//
//crJXJMeiRiHaoLiCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJMeiRiHaoLiCanvasUpdateMethod::crJXJMeiRiHaoLiCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJMeiRiHaoLiCanvasUpdateMethod::crJXJMeiRiHaoLiCanvasUpdateMethod( const crJXJMeiRiHaoLiCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_LingquBtn(handle.m_LingquBtn),
	m_sw(handle.m_sw),
	m_Text(handle.m_Text)
{
	for (unsigned char i = 0; i< 4;++i)
	{
		m_icon[i] = handle.m_icon[i];
	}
}

void crJXJMeiRiHaoLiCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJMeiRiHaoLiCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_LingquBtn = str;
		break;
	case 1:
		m_Text = str;
		break;
	case 2:
		m_sw = str;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		m_icon[i - 3] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
		m_input[i -7] = str;
		break;
	default:
		break;
	}

}

void crJXJMeiRiHaoLiCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crButtonWidgetNode>  LingquBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_LingquBtn));
		ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		ref_ptr<crStaticTextWidgetNode> text = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Text));
		ref_ptr<crImageBoxWidgetNode> icon[4];
		ref_ptr<crStaticTextWidgetNode> input[4];
		int meirihaoliID = 0;
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crRole> mainRole = playerData->getMainRole();
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if(mainRole.valid() && mainRole->getDataClass() )
		{
			int textIdx = huodongTab->getTitleIndex("内容");
			int titleIdx = huodongTab->getTitleIndex("标题");
			int idIdx = huodongTab->getTitleIndex("id");
			crTableIO::DataVec huodongIDVec;
			std::string titletext("每日十元");
			crData *roleData = mainRole->getDataClass();	
			void *param;
			CRCore::ScopedLock<crData> lock(*roleData);
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator itr = huodongMap->end();
			huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
			unsigned char ret = 0;
			std::string textstr;
			if (!huodongIDVec.empty()) 
			{	
				int huodongid = 0;
				for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				{
					huodongid = atoi((*siter)[idIdx].c_str());
					itr = huodongMap->find(huodongid);	
					if(itr != huodongMap->end() && itr->second.valid())
					{
						if(itr->second->isActivation())
						{
							textstr = (*siter)[textIdx];
							meirihaoliID = huodongid;
							if (itr->second->isCanAdvance() && !itr->second->getComplete())
							{
								ret = 1;
							}
							break;
						}
					}
				}
			}
			if (itr != huodongMap->end() && itr->second.valid())
			{
				if (itr->second->getComplete())
				{
					ret = 2;
				}
			}
			if (text.valid())
			{
				text->setString(textstr);
			}
			if (sw.valid())
			{
				sw->setActiveSwitchSet(ret);
			}
			std::vector<CRCore::crVector4i> awardData;
			crTableIO::StrVec huodongrecord;
			if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(meirihaoliID),huodongrecord) >= 0)
			{
				CRCore::crVector4i awardVec;
				for (int i = 1 ; i <= MAX_ACITVITY_AWARD; ++i)
				{
					int awardIndex = huodongTab->getTitleIndex("奖励"+crArgumentParser::appItoa(i));
					if(awardIndex >= 0 && !huodongrecord[awardIndex].empty())
					{
						crArgumentParser::appAtoVec(huodongrecord[awardIndex],awardVec);
						awardData.push_back((awardVec));
					}
				}
			}
			for(int i = 0; i < 4; ++i)
			{
				icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
				input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
				if (!icon[i].valid())
					continue;
				if (i<awardData.size())
				{
					icon[i]->setVisiable(true);
					setRewardInfo( awardData[i],icon[i].get());
				}else
				{
					icon[i]->setVisiable(false);
				}
				if (!input[i].valid())
					continue;
				if (i < awardData.size())
				{
					input[i]->setString(crArgumentParser::appItoa(awardData[i][3]));
				}
			}
		}


	}
}


/**************************************************************************

METHOD: crJXJMeiRiHaoLiGetAwardsMethod
 
**************************************************************************/

crJXJMeiRiHaoLiGetAwardsMethod::crJXJMeiRiHaoLiGetAwardsMethod()
{

}

crJXJMeiRiHaoLiGetAwardsMethod::crJXJMeiRiHaoLiGetAwardsMethod( const crJXJMeiRiHaoLiGetAwardsMethod & handle ):
	crMethod(handle)
{

}

void crJXJMeiRiHaoLiGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJMeiRiHaoLiGetAwardsMethod::addParam( int i, const std::string& str )
{

}

void crJXJMeiRiHaoLiGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);	
	if(netConductor)
	{			
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvMeiRiHaoLiGetAwards,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}


/************************************************************************/
/*    crJXJRecvMeiRiHaoLiGetAwardsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvMeiRiHaoLiGetAwardsMethod::crJXJRecvMeiRiHaoLiGetAwardsMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvMeiRiHaoLiGetAwardsMethod::crJXJRecvMeiRiHaoLiGetAwardsMethod( const crJXJRecvMeiRiHaoLiGetAwardsMethod & handle ):
	crMethod(handle),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvMeiRiHaoLiGetAwardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvMeiRiHaoLiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvMeiRiHaoLiGetAwardsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			char logRet = 0;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				int meirihaoliID = 0;
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				int titleIdx = huodongTab->getTitleIndex("标题");
				int idIdx = huodongTab->getTitleIndex("id");
				crTableIO::DataVec huodongIDVec;
				std::string titletext("每日十元");
				crData *roleData = mainRole->getDataClass();	
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				void *param;
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->end();
				huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
				if (!huodongIDVec.empty()) 
				{	
					int huodongid = 0;
					for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
					{
						huodongid = atoi((*siter)[idIdx].c_str());
						itr = huodongMap->find(huodongid);	
						if(itr != huodongMap->end())
						{
							if(itr->second->isCanAdvance() && !itr->second->getComplete())
							{
								meirihaoliID = huodongid;
								break;
							}
						}
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
				std::vector<CRCore::crVector4i> awardData;
				crTableIO::StrVec huodongrecord;
				if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(meirihaoliID),huodongrecord) >= 0)
				{
					CRCore::crVector4i awardVec;
					for (int i = 1 ; i <= MAX_ACITVITY_AWARD; ++i)
					{
						int awardIndex = huodongTab->getTitleIndex("奖励"+crArgumentParser::appItoa(i));
						if(awardIndex >= 0 && !huodongrecord[awardIndex].empty())
						{
							crArgumentParser::appAtoVec(huodongrecord[awardIndex],awardVec);
							awardData.push_back((awardVec));
						}
					}
				}
				if (!awardData.empty())
				{
					crData *thisData = m_this->getDataClass();
					thisData->excHandle(MAKEINT64(WCH_LockData,1));
					sendReward(awardData,m_this,"活动奖励");
					thisData->excHandle(MAKEINT64(WCH_LockData,0));
					if (itr != huodongMap->end() && itr->second.valid())
					{
						itr->second->setActivation(2,playerid);
					}
					logRet = 1;
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeChar(logRet);
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvMeiRiHaoLiGetAwards,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				char gbuf[256];
				sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),meirihaoliID,logRet);
				std::string logdata(gbuf);
				GameLogData gamelog(Log_HuodongGet,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));


			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}

		}
	}
}


/**************************************************************************

METHOD: crJXJQueryCanFenFengGroupMemberListMethod
 
**************************************************************************/

crJXJQueryCanFenFengGroupMemberListMethod::crJXJQueryCanFenFengGroupMemberListMethod()
{

}

crJXJQueryCanFenFengGroupMemberListMethod::crJXJQueryCanFenFengGroupMemberListMethod( const crJXJQueryCanFenFengGroupMemberListMethod & handle ):
	crMethod(handle)
{

}

void crJXJQueryCanFenFengGroupMemberListMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJQueryCanFenFengGroupMemberListMethod::addParam( int i, const std::string& str )
{

}

void crJXJQueryCanFenFengGroupMemberListMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);	
	if(netConductor)
	{			
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCanFenFengGroupMemberList,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}


/////////////////////////////////////////
//
//crJXJRecvCanFenFengGroupMemberListMethod
//
/////////////////////////////////////////
crJXJRecvCanFenFengGroupMemberListMethod::crJXJRecvCanFenFengGroupMemberListMethod():
	m_netType(GameClient_Game){}
crJXJRecvCanFenFengGroupMemberListMethod::crJXJRecvCanFenFengGroupMemberListMethod(const crJXJRecvCanFenFengGroupMemberListMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_renmingcanvas(handle.m_renmingcanvas)
{
}
void crJXJRecvCanFenFengGroupMemberListMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvCanFenFengGroupMemberListMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_renmingcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvCanFenFengGroupMemberListMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			int groupid = 0;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJConsortiaID,param);
			groupid = *(int *)param;
			if(groupid == 0)
				return;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData;
			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(!playerData.valid())
				return;
			ref_ptr<crGameServerPlayerData> memberplayerData;
			ref_ptr<crStreamBuf> stream;
			ref_ptr<crData> condata;
			ConsortiaMemberMap *membermap;
			ConsortiaMemberMap::iterator memitr;
			ConsortiaMap::iterator mapitr;
			int bufsize = 0;
			crPlayerDataEventZipPacket packet;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = ( ConsortiaMap *)param;
			mapitr = consortiamap->find(groupid);
			if (mapitr != consortiamap->end() && mapitr->second.valid())
			{
				condata = mapitr->second->getDataClass();
			}
			if(condata.valid())
			{
				condata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				membermap = (ConsortiaMemberMap *)param;
				condata->getParam(WCHDATA_JXJConsortiaMemberRank,param);
				ConsortiaMemberRank *rank = (ConsortiaMemberRank *)param;

				rank->clear();
				ConsortiaMemberMap::iterator memitr = membermap->begin();
				for (; memitr != membermap->end() ; ++memitr)
				{
					if (memitr->second.valid())
					{
						memberplayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(memitr->second->getPlayerID()));
						if(memberplayerData.valid() && memberplayerData->getPlayerGameData()
							&& memberplayerData->getPlayerGameData()->getMainRole())
						{
							crData * mainRoleData = memberplayerData->getPlayerGameData()->getMainRole()->getDataClass();
							if (mainRoleData)
							{
								mainRoleData->getParam(WCHDATA_Level,param);
								unsigned char level = *(unsigned char *)param;
								memitr->second->setLevel(level);

								mainRoleData->getParam(WCHDATA_JXJGovPost,param);
								unsigned char govPost = *(unsigned char *)param;
								memitr->second->setPos(govPost);
							}
						}
						CRCore::ref_ptr<ConsortiaMemberRankItem> rankItem = new ConsortiaMemberRankItem(0);
						rankItem->fullOfData(memitr->second.get());
						rank->updateRank(rankItem->getKey(),rankItem);
					}
				}
				stream = new crStreamBuf;
				stream->createBuf(1024);
				stream->_writeUChar(0);
				unsigned char size = 0;
				time_t curtime = time(0);
				ConsortiaMemberRank::RankSet rankSet = rank->getRankSet();
				ConsortiaMemberRank::RankSet::iterator rsItr = rankSet.begin();
				for (; rsItr != rankSet.end(); ++ rsItr)
				{
					if ((*rsItr).valid())
					{
						memitr = membermap->find((*rsItr)->m_id);
						if (memitr != membermap->end() && memitr->second.valid())
						{
							if((*rsItr)->m_groupPos != JXJGroup_FirstLeader
								&& (*rsItr)->m_level >= 35 
								&& (curtime - memitr->second->getLoginTime() < 3 * c_24hours)
								&& memitr->second->getNewer() == false)
							{
								stream->_writeInt((*rsItr)->m_id);//玩家ID
								stream->_writeString((*rsItr)->m_name);//玩家名称	
								stream->_writeUChar(memitr->second->getLevel());
								++ size;
							}
						}
					}	
				}
				bufsize = stream->getBufSize();
				stream->seekBegin();
				stream->_writeUChar(size);
				stream->setBufSize(bufsize);
				stream->seekEnd();
			}
			if (playerData.valid() && stream.valid())
			{
				crPlayerDataEventZipPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCanFenFengGroupMemberList,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid())
			{
				crData *canvasdata = canvas->getDataClass();
				if (canvasdata)
				{
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					unsigned char perpagenum = 8;
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata ->getParam(WCHDATA_JXJGroupCanFengFenmemVec,param);
					ConsortiaMemberVec *groupMemberVec = (ConsortiaMemberVec *)param;
					groupMemberVec->clear();

					unsigned char  size = m_stream->_readUChar();
					int curplayerid = 0;
					for (int i = 0; i < size; i++)
					{
						curplayerid = m_stream -> _readInt();
						ref_ptr<crJXJConsortiaMember> memberInfo = new crJXJConsortiaMember(curplayerid);
						memberInfo->setName(m_stream -> _readString());
						memberInfo->setLevel(m_stream->_readUChar());
						groupMemberVec->push_back(memberInfo);
					}
					int totalpage = ceil((float)size/(float)perpagenum) ;
					if (totalpage < 1)
					{
						totalpage = 1;
					}
					canvasdata ->inputParam(WCHDATA_JXJMyGroupFenFengTotalPage,&totalpage);
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
					//canvas->doEvent(WCH_UI_UpdateData);
					ref_ptr<crCanvasNode> renmingcanvas = crFilterRenderManager::getInstance()->findCanvas(m_renmingcanvas);
					if (renmingcanvas.valid())
					{
						renmingcanvas->doEvent(WCH_UI_UpdateData);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJWorldFubenSynchronouscoordMethod
//
/////////////////////////////////////////
crJXJWorldFubenSynchronouscoordMethod::crJXJWorldFubenSynchronouscoordMethod()
{

}
crJXJWorldFubenSynchronouscoordMethod::crJXJWorldFubenSynchronouscoordMethod(const crJXJWorldFubenSynchronouscoordMethod& handle):
	crMethod(handle)	
{
}
void crJXJWorldFubenSynchronouscoordMethod::addParam(int i, const std::string& str)
{

}
void crJXJWorldFubenSynchronouscoordMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJWorldFubenSynchronouscoordMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		unsigned char shiliid = 0;
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetManager *netManager = sceneServer->getNetManager();
		ref_ptr<crSceneServerPlayerData> playerData;
		ref_ptr<crPlayerDataSceneEventPacket> packet[3];
		ref_ptr<crStreamBuf> streambuf[3];
		ref_ptr<crPlayerGameData> mplayerdata;
		CRCore::ref_ptr<crRole> mainrole;
		unsigned char count[3] = {0};
		int playerid = 0;
		int roleid = 0;
		for (unsigned char i = 0; i < 3; ++i)
		{
			streambuf[i] = new crStreamBuf;
			streambuf[i]->createBuf(1024);
			streambuf[i]->_writeUChar(0);
		}
		m_this->lockPlayerList();
		crRoom::PlayerList &playerlist = m_this->getPlayerList();
		for (crRoom::PlayerList::iterator plitr = playerlist.begin();plitr != playerlist.end(); ++plitr)
		{
			if (plitr->get() && plitr->get()->getPlayerGameData() && plitr->get()->getPlayerGameData()->getMainRole())
			{
				shiliid = plitr->get()->getGroupID();
				mplayerdata = plitr->get()->getPlayerGameData();
				mainrole = mplayerdata->getMainRole();
				playerid = mplayerdata->getPlayerID();
				playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
				if (playerData.valid())
				{
					crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
					if (shiliid == 10 && count[0] < 100)
					{
						for (crSceneServerPlayerData::RoleMap::iterator rmitr = roleMap.begin();rmitr != roleMap.end();++rmitr)
						{
							if (rmitr->second.valid())
							{
								playerid = rmitr->second->getPlayerID();
								roleid = rmitr->second->getRoleID();
								++count[0];
								streambuf[0]->_writeInt64(MAKEINT64(playerid,roleid));
								streambuf[0]->_writeFloat(rmitr->second->getPosx());
								streambuf[0]->_writeFloat(rmitr->second->getPosy());
							}
						}
					}
					else if(shiliid == 11 && count[1] < 100)
					{
						for (crSceneServerPlayerData::RoleMap::iterator rmitr = roleMap.begin();rmitr != roleMap.end();++rmitr)
						{
							if (rmitr->second.valid())
							{
								playerid = rmitr->second->getPlayerID();
								roleid = rmitr->second->getRoleID();
								++count[1];
								streambuf[1]->_writeInt64(MAKEINT64(playerid,roleid));
								streambuf[1]->_writeFloat(rmitr->second->getPosx());
								streambuf[1]->_writeFloat(rmitr->second->getPosy());
							}
						}
					}
					else if(shiliid == 12 && count[2] < 100)
					{
						for (crSceneServerPlayerData::RoleMap::iterator rmitr = roleMap.begin();rmitr != roleMap.end();++rmitr)
						{
							if (rmitr->second.valid())
							{
								playerid = rmitr->second->getPlayerID();
								roleid = rmitr->second->getRoleID();
								++count[2];
								streambuf[2]->_writeInt64(MAKEINT64(playerid,roleid));
								streambuf[2]->_writeFloat(rmitr->second->getPosx());
								streambuf[2]->_writeFloat(rmitr->second->getPosy());
							}
						}
					}
				}
			}
			if (count[0] >= 100 && count[1] >= 100 && count[2] >= 100)
			{
				break;
			}
		}
		m_this->unlockPlayerList();
		int bufsize = 0;
		for (unsigned char i = 0; i < 3; ++i)
		{
			packet[i] = new crPlayerDataSceneEventPacket;
			bufsize = streambuf[i]->getBufSize();
			streambuf[i]->seekBegin();
			streambuf[i]->_writeUChar(count[i]);
			streambuf[i]->setBufSize(bufsize);
			crPlayerDataSceneEventPacket::buildReplyPacket(*packet[i],0,WCH_JXJRecvWorldFubenSynchronouscoord,streambuf[i].get());
			m_this->sendRoomMessage(packet[i].get(),c_startShiliID + i);
		}
	}		
}

/////////////////////////////////////////
//
//crJXJRecvWorldFubenSynchronouscoordMethod
//
/////////////////////////////////////////
crJXJRecvWorldFubenSynchronouscoordMethod::crJXJRecvWorldFubenSynchronouscoordMethod():
	m_netType(GameClient_Game){}
crJXJRecvWorldFubenSynchronouscoordMethod::crJXJRecvWorldFubenSynchronouscoordMethod(const crJXJRecvWorldFubenSynchronouscoordMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvWorldFubenSynchronouscoordMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvWorldFubenSynchronouscoordMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvWorldFubenSynchronouscoordMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			unsigned char count = m_stream->_readUChar();
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			crData *roomdata = room?room->getDataClass():NULL;
			if (roomdata)
			{
				void *param;
				roomdata->excHandle(MAKEINT64(WCH_LockData,1));
				roomdata->getParam(WCHDATA_JXJPlayerCoordMap,param);
				PlayerCoordMap *playercoordMap = (PlayerCoordMap *)param;
				playercoordMap->clear();
				_crInt64 tmp = 0;
				float x = 0.0f;
				float y = 0.0f;
				for (unsigned char i = 0; i < count; ++i)
				{
					tmp = m_stream->_readInt64();
					x = m_stream->_readFloat();
					y = m_stream->_readFloat();
					playercoordMap->insert(std::make_pair(tmp,CRCore::crVector2f(x,y)));
				}
				roomdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

//**************************************************************************
//
//METHOD: crJXJNewHuiKuiCanvasInitMethod
//
//**************************************************************************/
crJXJNewHuiKuiCanvasInitMethod::crJXJNewHuiKuiCanvasInitMethod():
	m_this(NULL)
{

}

crJXJNewHuiKuiCanvasInitMethod::crJXJNewHuiKuiCanvasInitMethod( const crJXJNewHuiKuiCanvasInitMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJNewHuiKuiCanvasInitMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJNewHuiKuiCanvasInitMethod::addParam( int i, const std::string& str )
{
}

void crJXJNewHuiKuiCanvasInitMethod::operator()( crHandle &handle )
{
	do 
	{
		void *param;
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		crRole *mainRole = playerData->getMainRole();
		if(mainRole && mainRole->getDataClass())
		{
			crTableIO::StrVec record;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			int displayid = huodongTab->getTitleIndex("活动显示");
			int titleid = huodongTab->getTitleIndex("标题");
			crData *roleData = mainRole->getDataClass();
			roleData->excHandle(MAKEINT64(WCH_LockData,1));

			crData *canvasData = m_this->getDataClass();
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJHuiKuiHuoDongMap,param);
			HuiKuiHuoDongMap & HKhuodongmap = *(HuiKuiHuoDongMap *)param;
			HuiKuiHuoDongMap::iterator HKmitr;
			HKhuodongmap.clear();
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			NewHuodongMap ::iterator itr = huodongMap->begin();
			for (; itr != huodongMap->end(); ++ itr)
			{
				//if(itr->second->isIntimeCheck())
				if(itr->second->getActivation()==1 || (itr->second->getComplete() && itr->second->isIntimeCheck()))
				{
					if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
					{
						if(atoi(record[displayid].c_str()) == 2)
						{
							HKmitr = HKhuodongmap.find(record[titleid]);
							if (HKmitr != HKhuodongmap.end())
							{
								HKmitr->second.insert(itr->second->getID());
							}
							else
							{
								std::set<unsigned short> tmpset;
								tmpset.insert(itr->second->getID());
								HKhuodongmap.insert(std::make_pair(record[titleid],tmpset));
							}
						}
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
			roleData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	} while (0);
}

/////////////////////////////////////////
//
//crJXJNewHuiKuiCanvasUpdateMethod
//
///////////////////////////////////////
crJXJNewHuiKuiCanvasUpdateMethod::crJXJNewHuiKuiCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJNewHuiKuiCanvasUpdateMethod::crJXJNewHuiKuiCanvasUpdateMethod( const crJXJNewHuiKuiCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_scroll(handle.m_scroll),
	m_nullImage(handle.m_nullImage),
	m_Dintu_sw(handle.m_Dintu_sw),
	m_shengyushijianText(handle.m_shengyushijianText),
	m_yichongzhi(handle.m_yichongzhi),
	m_guojia_sw(handle.m_guojia_sw)
{
	for (unsigned char i = 0; i< 18;++i)
	{
		if (i < 10)
		{
			m_titleBtn[i] = handle.m_titleBtn[i];
			m_title[i] = handle.m_title[i];
			m_name_sw[i] = handle.m_name_sw[i];
			m_numinput[i] = handle.m_numinput[i];
		}
		m_icon[i] = handle.m_icon[i];
		m_title[i] = handle.m_title[i];
		m_iconplane[i] = handle.m_iconplane[i];
		if ( i < 3)
		{
			m_title1[i] = handle.m_title1[i];
			m_lingquBtn[i] = handle.m_lingquBtn[i];
			m_sw[i] = handle.m_sw[i];
			m_node[i] = handle.m_node[i];
			m_circles[i] = handle.m_circles[i];
			m_lingqu_sw[i] = handle.m_lingqu_sw[i];
			m_sequence[i] = handle.m_sequence[i];
		}
	}
}

void crJXJNewHuiKuiCanvasUpdateMethod::inputParam( int i, void *param )
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
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJNewHuiKuiCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_titleBtn[i] = str;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_title[i - 10] = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		m_icon[i - 20] = str;
		break;
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
		m_input[i - 38] = str;
		break;
	case 56:
	case 57:
	case 58:
		m_title1[i - 56] = str;
		break;
	case 59:
		m_scroll = str;
		break;
	case 60:
	case 61:
	case 62:
		m_lingquBtn[i - 60] = str;
		break;
	case 63:
		m_nullImage = str;
		break;
	case 64:
	case 65:
	case 66:
		m_sw[i - 64] = str;
		break;
	case 67:
	case 68:
	case 69:
		m_goumaiBtn[i - 67] = str;
		break;
	case 70:
		m_Dintu_sw = str;
		break;
	case 71:
		m_shengyushijianText = str;
		break;
	case 72:
		m_yichongzhi = str;
		break;
	case 73:
	case 74:
	case 75:
		m_node[i - 73] = str;
		break;
	case 76:
	case 77:
	case 78:
		m_circles[i - 76] = str;
		break;
	case 79:
	case 80:
	case 81:
		m_lingqu_sw[i - 79] = str;
		break;
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
	case 95:
	case 96:
	case 97:
	case 98:
	case 99:
		m_iconplane[i - 82] = str;
		break;
	case 100:
		m_guojia_sw = str;
		break;
	case 101:
	case 102:
	case 103:
	case 104:
	case 105:
	case 106:
	case 107:
	case 108:
	case 109:
	case 110:
		m_name_sw[i - 101] = str;
		break;
	case 111:
	case 112:
	case 113:
	case 114:
	case 115:
	case 116:
	case 117:
	case 118:
	case 119:
	case 120:
		m_numinput[i - 111] = str;
		break;
	case 121:
	case 122:
	case 123:
		m_sequence[i - 121] = str;
		break;
	default:
		break;
	}

}

std::string crJXJNewHuiKuiCanvasUpdateMethod::getremainingtime(CRCore::crVector3i endtimeVec)
{
	struct tm tm_endtime;
	memset(&tm_endtime,0,sizeof(tm_endtime));
	tm_endtime.tm_year = endtimeVec[0] - 1900;
	tm_endtime.tm_mon = endtimeVec[1] - 1;
	tm_endtime.tm_mday = endtimeVec[2];
	tm_endtime.tm_hour = 23;
	tm_endtime.tm_min = 59;
	tm_endtime.tm_sec = 59;
	time_t endtime = mktime(&tm_endtime);
	time_t curtime = time(0);
	time_t remainingtime = endtime - curtime;
	int _hour = remainingtime /3600;
	int _min = (remainingtime  - 3600*_hour)/60;
	int _sec = (remainingtime  - 3600*_hour -60*_min);
	std::string remaining;
	if (_hour >= 24)
	{
		remaining = crArgumentParser::appItoa(_hour/24) + "天" + crArgumentParser::appItoa(_hour - (_hour / 24) * 24 ) 
			+ "时" + crArgumentParser::appItoa(_min) +"分";
	}
	else if (_sec >= 0)
	{
		remaining =  crArgumentParser::appItoa(_hour) 
			+ "时" + crArgumentParser::appItoa(_min) +"分" + crArgumentParser::appItoa(_sec) + "秒";
	}
	else
	{
		remaining = "0:00:00";
	}
	return remaining;
}

void crJXJNewHuiKuiCanvasUpdateMethod::setTitleText(int tiaojian1,int tiaojian2, int title, int neirong,int cishu, const CREncapsulation::crTableIO::StrVec &record, crHypertextWidgetNode * hyp)
{
	std::string type = record[title];
	std::vector<std::string> strVec;
	crArgumentParser::appAtoVec(record[neirong],strVec);
	if (type.compare(std::string("限时特卖")) == 0)
	{
		if (strVec.size() >= 3)
		{
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",strVec[0]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(250.0);
			v_i.push_back(0.0);
			v_i.push_back(0.0);
			v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",record[tiaojian1]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			cfg_script.Add("Text",strVec[1]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",4);
			//v_i.clear();
			//v_i.push_back(140.0);
			//v_i.push_back(40.0);
			//v_i.push_back(40.0);
			//v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",record[cishu]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",5);
			cfg_script.Add("Text",strVec[2]);
			cfg_script.Pop();

			cfg_script.Pop();
			hyp->setHypertext(cfg_script);
		}
	}
	else if(type.compare(std::string("今日充值")) == 0 || type.compare(std::string("多日充值")) == 0)
	{
		if (strVec.size() >= 2)
		{
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",strVec[0]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(255.0);
			v_i.push_back(0.0);
			v_i.push_back(0.0);
			v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",record[tiaojian1]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			cfg_script.Add("Text",strVec[1]);
			cfg_script.Pop();

			cfg_script.Pop();
			hyp->setHypertext(cfg_script);
		}
	}
	else if(type.compare(std::string("连续充值豪礼")) == 0)
	{
		if (strVec.size() >= 3)
		{
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",strVec[0]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(255.0);
			v_i.push_back(0.0);
			v_i.push_back(0.0);
			v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",record[tiaojian2]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			cfg_script.Add("Text",strVec[1]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",4);
			//v_i.clear();
			//v_i.push_back(140.0);
			//v_i.push_back(40.0);
			//v_i.push_back(40.0);
			//v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",record[tiaojian1]);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",5);
			cfg_script.Add("Text",strVec[2]);
			cfg_script.Pop();

			cfg_script.Pop();
			hyp->setHypertext(cfg_script);
		}
	}
}

void crJXJNewHuiKuiCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crImageBoxWidgetNode> icon[18];
		ref_ptr<crStaticTextWidgetNode> input[18];
		ref_ptr<crRadioWidgetNode> titleBtn[10];
		ref_ptr<crStaticTextWidgetNode> title[10];
		ref_ptr<crHypertextWidgetNode> titl1[3];
		ref_ptr<crButtonWidgetNode> lingquBtn[3];
		ref_ptr<crMultiSwitch>  button_sw[3];
		ref_ptr<crButtonWidgetNode> goumaiBtn[3];
		ref_ptr<crNode> node[3];
		ref_ptr<crMultiSwitch>  Dintu_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_Dintu_sw));
		ref_ptr<crMultiSwitch>  guojia_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_guojia_sw));
		ref_ptr<crStaticTextWidgetNode> shengyusjText =dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_shengyushijianText));
		ref_ptr<crStaticTextWidgetNode> yichongzhiText =dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_yichongzhi));
		ref_ptr<crStaticTextWidgetNode> circles[3];
		ref_ptr<crNode> iconplane[18];
		ref_ptr<crMultiSwitch> lingqu_sw[3];
		ref_ptr<crMultiSwitch> name_sw[10];
		ref_ptr<crStaticTextWidgetNode> numinput[10];
		ref_ptr<crSequence> sequence[3];
		//ref_ptr<crNode> numplant[10];
		if (shengyusjText.valid())
		{
			shengyusjText->clearString();
		}
		if (yichongzhiText.valid())
		{
			yichongzhiText->clearString();
		}
		std::string numplantstr = "Plane216_";
		for (unsigned char i = 0; i < 18; ++i)
		{
			if (i < 10)
			{
				titleBtn[i] = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_titleBtn[i]));
				title[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title[i]));
				if (titleBtn[i].valid())
				{
					titleBtn[i]->setVisiable(false);
				}
				if (title[i].valid())
				{
					title[i]->clearString();
				}
				name_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_name_sw[i]));
				if (name_sw[i].valid())
				{
					name_sw[i]->setActiveSwitchSet(0);
				}
				numinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_numinput[i]));
				if (numinput[i].valid())
				{
					numinput[i]->setVisiable(false);
				}

				//numplant[i] = dynamic_cast<crNode *>(m_this->getChildNode(numplantstr + crArgumentParser::appItoa(i + 1)));
				//if (numplant[i].valid())
				//{
				//	numplant[i]->setVisiable(false);
				//}
			}
			if (i < 3)
			{
				titl1[i] = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_title1[i]));
				if (titl1[i].valid())
				{
					titl1[i]->clearText();
				}
				lingquBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingquBtn[i]));
				if (lingquBtn[i].valid())
				{
					lingquBtn[i]->setVisiable(false);
					lingquBtn[i]->setEnable(false);
				}
				goumaiBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goumaiBtn[i]));
				if (goumaiBtn[i].valid())
				{
					goumaiBtn[i]->setVisiable(false);
				}
				button_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw[i]));
				if (button_sw[i].valid())
				{
					button_sw[i]->setActiveSwitchSet(1);
				}
				node[i] = dynamic_cast<crNode *>(m_this->getChildNode(m_node[i]));
				if (node[i].valid())
				{
					node[i]->setVisiable(false);
				}
				circles[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_circles[i]));
				if (circles[i].valid())
				{
					circles[i]->clearString();
				}
				lingqu_sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lingqu_sw[i]));
				if (lingqu_sw[i].valid())
				{
					lingqu_sw[i]->setActiveSwitchSet(0);
				}
				sequence[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_sequence[i]));
				if (sequence[i].valid())
				{
					sequence[i]->setVisiable(false);
				}
			}
			icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			if (icon[i].valid())
			{
				icon[i]->setImageName(m_nullImage);
			}
			input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
			if (input[i].valid())
			{
				input[i]->clearString();
			}
			iconplane[i] = dynamic_cast<crNode *>(m_this->getChildNode(m_iconplane[i]));
			if (iconplane[i].valid())
			{
				iconplane[i]->setVisiable(false);
			}
		}
		ref_ptr<crScrollBarWidgetNode> scroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));

		crRole *mainRole = myPlayer->getMainRole();
		if(mainRole && mainRole->getDataClass())
		{
			crData *roledata = mainRole->getDataClass();
			crData *roleData = mainRole->getDataClass();
			void *param;
			roleData->excHandle(MAKEINT64(WCH_LockData,1));
			do 
			{
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap::iterator hdmitr;
				crData * canvasData = m_this->getDataClass();
				if (!canvasData) break;
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				if (false == huodongTab.valid()) break;
				int titleIdx = huodongTab->getTitleIndex("标题");
				int titlexianshiIdx = huodongTab->getTitleIndex("标题显示");
				int contentIdx = huodongTab->getTitleIndex("内容");
				int endtimeIdx = huodongTab->getTitleIndex("结束时间");
				int circlesIdx = huodongTab->getTitleIndex("次数");
				int tiaojian1Idx = huodongTab->getTitleIndex("条件");
				int tiaojian2Idx = huodongTab->getTitleIndex("条件2");
				int idIdx = huodongTab->getTitleIndex("id");
				std::string titlestr;
				canvasData->excHandle(MAKEINT64(WCH_LockData,1));
				canvasData->getParam(WCHDATA_JXJHuiKuiHuoDongMap,param);
				HuiKuiHuoDongMap & HKhuodongmap = *(HuiKuiHuoDongMap *)param;
				canvasData->getParam(WCHDATA_JXJHuodongSelectIdx,param);
				unsigned short hdIdx = *(unsigned short *)param;
				unsigned short _index = 0;
				unsigned short num = 0;
				unsigned short numIdx = 0;
				unsigned short iconIdx = 0;
				std::set<unsigned short>::iterator sitr;
				std::vector<CRCore::crVector4i> awardData;
				crTableIO::StrVec huodongrecord;
				int huodongid = 0;
				CRCore::crVector4i awardVec;
				std::string hdtitle;
				CRCore::crVector3i endtimeVec;
				if (hdIdx >= 0 && hdIdx < 10)
				{
					if (name_sw[hdIdx].valid())
					{
						name_sw[hdIdx]->setActiveSwitchSet(1);
					}
				}
				int count = 0;
				for (HuiKuiHuoDongMap::iterator hkmitr = HKhuodongmap.begin(); hkmitr != HKhuodongmap.end() && _index < 10; ++hkmitr,++_index)
				{
					if (!hkmitr->second.empty())
					{
						if (titleBtn[_index].valid() && title[_index].valid())
						{
							sitr = hkmitr->second.begin();
							if (sitr != hkmitr->second.end())
							{
								if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(*sitr),huodongrecord) >= 0)
								{
									titleBtn[_index]->setVisiable(true);
									titlestr = huodongrecord[titlexianshiIdx];
									title[_index]->setString(titlestr);
								}
							}
							if (numinput[_index].valid() /*&& numplant[_index].valid()*/)
							{
								count = 0;
								for (;sitr != hkmitr->second.end();++sitr)
								{
									hdmitr = huodongMap->find(*sitr);
									if (hdmitr != huodongMap->end() && hdmitr->second.valid() 
										&& hdmitr->second->isActivation() && hdmitr->second->isCanAdvance())
									{
										++count;
									}
								}
								if (count > 0)
								{
									numinput[_index]->setString(crArgumentParser::appItoa(count));
									numinput[_index]->setVisiable(true);
									//numplant[_index]->setVisiable(true);
								}
								else
								{
									numinput[_index]->setVisiable(false);
									/*numplant[_index]->setVisiable(false);*/
								}
							}
							
						}
						if (hdIdx == _index)
						{
							num = hkmitr->second.size();
							numIdx = 0;
							int val = 0;
							if (scroll.valid())
							{
								if (num <= 3)
								{
									scroll->setEnable(false);
								}
								else
								{
									scroll->setEnable(true);
									scroll->setRange(0, num - 3);
									scroll->setLineValue(1.0f);
									scroll->setPageValue(1.0f);
								}
								val = scroll->getValue();
							}
							iconIdx = 0;

							int alreadycharge = 0;
							if (hkmitr->first == "今日充值" || hkmitr->first == "多日充值")
							{
								for (sitr = hkmitr->second.begin();sitr != hkmitr->second.end(); ++sitr)
								{
									huodongid = *sitr;
									hdmitr = huodongMap->find(huodongid);
									if (hdmitr != huodongMap->end())
									{
										if (alreadycharge >= hdmitr->second->getStep())
											continue;
										else
											alreadycharge = hdmitr->second->getStep();
									}
								}
							}

							for (sitr = hkmitr->second.begin();sitr != hkmitr->second.end(); ++sitr,++numIdx)
							{
								if (numIdx >= val && iconIdx < 3)
								{
									huodongid = *sitr;
									hdmitr = huodongMap->find(huodongid);
									if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongid),huodongrecord) >= 0)
									{
										if (node[iconIdx].valid())
										{
											node[iconIdx]->setVisiable(true);
										}
										if (titleIdx >= 0 && contentIdx >= 0)
										{
											int cishu = atoi(huodongrecord[circlesIdx].c_str());
											if (titl1[iconIdx].valid())
											{
												setTitleText(tiaojian1Idx,tiaojian2Idx,titleIdx,contentIdx,circlesIdx,huodongrecord,titl1[iconIdx].get());
											}
											hdtitle = huodongrecord[titleIdx];
											if(hdtitle.compare(std::string("限时特卖")) == 0)
											{
												if (button_sw[iconIdx].valid() && goumaiBtn[iconIdx].valid())
												{
													goumaiBtn[iconIdx]->setVisiable(true);
													button_sw[iconIdx]->setActiveSwitchSet(0);
													if (hdmitr != huodongMap->end())
													{
														if (hdmitr->second->getCircles() == 0)
														{
															goumaiBtn[iconIdx]->setEnable(false);
															button_sw[iconIdx]->setActiveSwitchSet(2);
														}
														else
														{
															goumaiBtn[iconIdx]->setEnable(true);
														}
													}
													crData* uiData = goumaiBtn[iconIdx]->getDataClass();
													if (uiData)
													{
														uiData->inputParam(0,&huodongid);
													}
												}
												if (Dintu_sw.valid())
												{
													Dintu_sw->setActiveSwitchSet(2);
												}
												if (circles[iconIdx].valid())
												{
													if (hdmitr != huodongMap->end())
													{
														circles[iconIdx]->setString("次数:" + crArgumentParser::appItoa(cishu - hdmitr->second->getCircles()) 
															+ "/" + crArgumentParser::appItoa(cishu));
													}
												}
											}
											else
											{
												if (button_sw[iconIdx].valid() && lingquBtn[iconIdx].valid())
												{
													lingquBtn[iconIdx]->setVisiable(true);
													if(sequence[iconIdx].valid()) sequence[iconIdx]->setVisiable(true);
													button_sw[iconIdx]->setActiveSwitchSet(1);
													crData* uiData = lingquBtn[iconIdx]->getDataClass();
													if (uiData)
													{
														huodongid = *sitr;
														uiData->inputParam(0,&huodongid);
													}
													if (hdmitr != huodongMap->end())
													{
														if(hdmitr->second->isCanAdvance() && !hdmitr->second->getComplete())
														{
															lingquBtn[iconIdx]->setEnable(true);
															if(sequence[iconIdx].valid()) sequence[iconIdx]->setVisiable(true);
														}
														else /*if(hdmitr->second->getComplete())*/
														{
															lingquBtn[iconIdx]->setEnable(false);
															if(sequence[iconIdx].valid()) sequence[iconIdx]->setVisiable(false);
														}
													}
												}
											}
											if (hdtitle.compare(std::string("今日充值")) == 0 || hdtitle.compare(std::string("多日充值")) == 0)
											{
												crData *data = myPlayer->getDataClass();
												data->getParam(WCHDATA_JXJShiliID,param);
												unsigned char shiliid = *(unsigned char *)param;
												if (guojia_sw.valid())
												{
													guojia_sw->setActiveSwitchSet(shiliid - c_startShiliID);
												}
												if (hdmitr != huodongMap->end())
												{
													if (hdmitr->second->getComplete())
													{
														if (lingqu_sw[iconIdx].valid())
														{
															lingqu_sw[iconIdx]->setActiveSwitchSet(4);
														}
													}

												}
												if (Dintu_sw.valid())
												{
													Dintu_sw->setActiveSwitchSet(0);
												}
												if (yichongzhiText.valid())
												{
													if (hdmitr != huodongMap->end())
													{
														yichongzhiText->setString(crArgumentParser::appItoa(/*hdmitr->second->getStep()*/alreadycharge) + "元宝");
													}	
												}
											}
											else if (hdtitle.compare(std::string("连续充值豪礼")) == 0)
											{
												if (Dintu_sw.valid())
												{
													Dintu_sw->setActiveSwitchSet(1);
												}
												if (circles[iconIdx].valid())
												{
													if (hdmitr != huodongMap->end())
													{
														if (hdmitr->second->getComplete())
														{
															circles[iconIdx]->setString(std::string("已完成"));
															if (lingqu_sw[iconIdx].valid())
															{
																lingqu_sw[iconIdx]->setActiveSwitchSet(4);
															}
														}
														else
														{
															circles[iconIdx]->setString("次数:" + crArgumentParser::appItoa(hdmitr->second->getStep()) 
																+ "/" + crArgumentParser::appItoa(hdmitr->second->getStepCount()));
														}
													}
												}
												if (yichongzhiText.valid())
												{
													if (hdmitr != huodongMap->end())
													{
														yichongzhiText->setString(crArgumentParser::appItoa(hdmitr->second->getStep()) + "次");
													}	
												}
											}
											if (shengyusjText.valid())
											{
												int opendayIdx = huodongTab->getTitleIndex("开服天数");
												CRCore::crVector2i opendayVec2i;
												crArgumentParser::appAtoVec(huodongrecord[opendayIdx],opendayVec2i);
												if (opendayVec2i[0] != 0)
												{
													typedef std::vector<int> Vec5i;
													Vec5i startTimeVec5i;
													std::string dateStr = crGlobalHandle::gData()->gStartDate();
													crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
													startTimeVec5i.resize(5,0);
													time_t nowTime_t = time(0);
													struct  tm  nowTm = *localtime(&nowTime_t);
													struct tm  startTm;
													memset(&startTm,0,sizeof (startTm));
													startTm.tm_year = startTimeVec5i[0] - 1900;
													startTm.tm_mon = startTimeVec5i[1] - 1;
													startTm.tm_mday = startTimeVec5i[2];
													startTm.tm_hour = startTimeVec5i[3];
													startTm.tm_min = startTimeVec5i[4];
													startTm.tm_isdst = nowTm.tm_isdst;
													time_t startTime_t = mktime(&startTm);
													char tmp[20];
													time_t t = 0;
													if (opendayVec2i[1] != 0)
													{
														t = startTime_t + (opendayVec2i[1] - 1) * c_24hours;
														strftime( tmp, sizeof(tmp), "%Y|%m|%d\0", localtime(&t) );
													}
													else
													{
														t = startTime_t + (opendayVec2i[0] - 1) * c_24hours;
														strftime( tmp, sizeof(tmp), "%Y|%m|%d\0", localtime(&t) );
													}
													crArgumentParser::appAtoVec(tmp,endtimeVec);

												}
												else
												{
													crArgumentParser::appAtoVec(huodongrecord[endtimeIdx],endtimeVec);
												}
												shengyusjText->setVisiable(true);
												shengyusjText->setString(getremainingtime(endtimeVec));
											}
										}
										awardData.clear();
										for (int i = 1 ; i <= MAX_ACITVITY_AWARD; ++i)
										{
											int awardIndex = huodongTab->getTitleIndex("奖励"+crArgumentParser::appItoa(i));
											if(awardIndex >= 0 && !huodongrecord[awardIndex].empty())
											{
												crArgumentParser::appAtoVec(huodongrecord[awardIndex],awardVec);
												awardData.push_back((awardVec));
											}
										}
										for(int i = iconIdx * 6; i <(1+iconIdx) *6 && i < 18; ++i)
										{
											if (icon[i].valid() && input[i].valid() && iconplane[i].valid())
											{
												if (i - iconIdx * 6 < awardData.size())
												{
													icon[i]->setVisiable(true);
													setRewardInfo( awardData[i - iconIdx * 6],icon[i].get());
													input[i]->setString(crArgumentParser::appItoa(awardData[i - iconIdx * 6][3]));
													iconplane[i]->setVisiable(true);
												}
												else
												{
													icon[i]->setVisiable(false);
													input[i]->clearString();
													iconplane[i]->setVisiable(false);
												}
											}
										}
										++iconIdx;
									}
								}
							}
						}
					}
				}
				canvasData->excHandle(MAKEINT64(WCH_LockData,0));
			} while (0);
			roleData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/**************************************************************************

METHOD: crJXJSetCurrentHuiKuiHdIdMethod
 
**************************************************************************/
JXJ::crJXJSetCurrentHuiKuiHdIdMethod::crJXJSetCurrentHuiKuiHdIdMethod():
	m_this(NULL),
	m_idx(0)
{

}

JXJ::crJXJSetCurrentHuiKuiHdIdMethod::crJXJSetCurrentHuiKuiHdIdMethod( const crJXJSetCurrentHuiKuiHdIdMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_idx(handle.m_idx)
{

}

void JXJ::crJXJSetCurrentHuiKuiHdIdMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}

}

void JXJ::crJXJSetCurrentHuiKuiHdIdMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_idx = (unsigned short)(atoi(str.c_str()));
		break;
	default:
		break;
	}

}

void JXJ::crJXJSetCurrentHuiKuiHdIdMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (!canvas.valid() || !canvas->getDataClass()) return;
	crData * canvasData = canvas->getDataClass();
	canvasData->inputParam(WCHDATA_JXJHuodongSelectIdx	,&m_idx);
}

/**************************************************************************

METHOD: crJXJClientNewHuiKuiGetAwardsMethod
 
**************************************************************************/

crJXJClientNewHuiKuiGetAwardsMethod::crJXJClientNewHuiKuiGetAwardsMethod():
	m_isgoumai(0)
{

}

crJXJClientNewHuiKuiGetAwardsMethod::crJXJClientNewHuiKuiGetAwardsMethod( const crJXJClientNewHuiKuiGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_isgoumai(handle.m_isgoumai)
{

}

void crJXJClientNewHuiKuiGetAwardsMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJClientNewHuiKuiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_infoCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn = str;
		break;
	case 3:
		m_isgoumai = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}

}

void crJXJClientNewHuiKuiGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crData *uidata = m_this->getDataClass();
		unsigned short huodongID = 0;
		void *param;
		if (uidata)
		{
			uidata->getParam(0,param);
			huodongID = *(unsigned short *)param;
		}
		if (huodongID > 0)
		{
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(3);
			stream->_writeUShort(huodongID);
			stream->_writeUChar(m_isgoumai);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvNewHuiKuiGetAwards,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}		
	}
}

/************************************************************************/
/*    crJXJRecvClientNewHuiKuiGetAwardsMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvClientNewHuiKuiGetAwardsMethod::crJXJRecvClientNewHuiKuiGetAwardsMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvClientNewHuiKuiGetAwardsMethod::crJXJRecvClientNewHuiKuiGetAwardsMethod( const crJXJRecvClientNewHuiKuiGetAwardsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence)
{

}

void JXJ::crJXJRecvClientNewHuiKuiGetAwardsMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJRecvClientNewHuiKuiGetAwardsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_animation = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvClientNewHuiKuiGetAwardsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			bool needdeductgold = false;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			crRole *mainRole = m_this->getMainRole();
			if(playerData.valid() && mainRole && mainRole->getDataClass())
			{
				char logRet = 0;
				unsigned short huodongID = m_stream->_readUShort();
				unsigned char isgoumai = m_stream->_readUChar();
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				crTableIO::StrVec record;

				int awardIdx[6] ;
				for (unsigned char i = 0; i < 6; ++ i)
				{
					awardIdx[i] = -1;
				}
				awardIdx[0] = huodongTab->getTitleIndex("奖励1");
				awardIdx[1] = huodongTab->getTitleIndex("奖励2");
				awardIdx[2] = huodongTab->getTitleIndex("奖励3");
				awardIdx[3] = huodongTab->getTitleIndex("奖励4");
				awardIdx[4] = huodongTab->getTitleIndex("奖励5");
				awardIdx[5] = huodongTab->getTitleIndex("奖励6");

				int tiaojianIdx = huodongTab->getTitleIndex("条件2");

				if (awardIdx[0] >=0 && awardIdx[1] >=0 && awardIdx[2] >=0 && awardIdx[3] >=0 && awardIdx[4] >=0 && tiaojianIdx >=0)
				{
					crVector4i awardVec;
					void *param;
					crData *playerGameData = m_this->getDataClass();
					crData *data = mainRole->getDataClass();
					playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
					if(itr != huodongMap->end())
					{
						if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
						{
							if (isgoumai == 1)
							{
								if (itr->second->isActivation())
								{
									int totalprice =itr->second->getStepCount();
									if (totalprice > 0)
									{
										if (record[tiaojianIdx].compare(std::string("1")) == 0)
										{
											MoneyChangeData moneydata(-totalprice,"限时特卖活动");
											m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
											if(needdeductgold == true)
											{
												itr->second->setStep(totalprice,m_this->getPlayerID());
											}
											else
											{
												logRet = -1;
											}
										}
										else
										{
											MoneyChangeData moneydata(totalprice,"限时特卖活动");
											m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
											if (0 == moneydata.first)
											{

												itr->second->setStep(totalprice,m_this->getPlayerID());
											}
											else
											{
												logRet = -1;
											}
										}
									}
								}
							}
							if(itr->second->isCanAdvance() && !itr->second->getComplete())
							{
								std::vector<CRCore::crVector4i> awards;
								for (unsigned char i = 0; i < 6; ++ i)
								{
									if (record[awardIdx[i]] == "") continue;
									crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
									awards.push_back(awardVec);
								}

								logRet = 1;
								sendReward(awards,m_this,"活动奖励");
								itr->second->setActivation(2,playerid);							
							}
						}
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1);
						stream->_writeChar(logRet);
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvNewHuiKuiGetAwards,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
					playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

					char gbuf[256];
					sprintf(gbuf,"%s 尝试领取活动奖励 活动ID %d 领取状态 %d\0",playerData->getCharacterName().c_str(),huodongID,logRet);
					std::string logdata(gbuf);
					GameLogData gamelog(Log_HuodongGet,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			switch (_case)
			{
			case -1:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2036, NULL));//元宝不足
				break;
			default:
				break;
			}
			if(_case == 1)
			{
				// 播放领取成功的动画
				ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound);
			}

		}
	}
}

////////////////////////////////
//
//crCheckHuiKuiHuoDongCompleteMethod
//回馈活动提示
///////////////////////////////////
crCheckHuiKuiHuoDongCompleteMethod::crCheckHuiKuiHuoDongCompleteMethod() :
	m_this(NULL)
{}

crCheckHuiKuiHuoDongCompleteMethod::crCheckHuiKuiHuoDongCompleteMethod(const crCheckHuiKuiHuoDongCompleteMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_huodongcanvas(handle.m_huodongcanvas),
	m_countbackboard(handle.m_countbackboard),
	m_countinput(handle.m_countinput),
	m_huikuibtn(handle.m_huikuibtn)
{
}

void crCheckHuiKuiHuoDongCompleteMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crCheckHuiKuiHuoDongCompleteMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_huodongcanvas = str;
		break;
	case 1:
		m_countbackboard = str;
		break;
	case 2:
		m_countinput = str;
		break;
	case 3:
		m_huikuibtn = str;
		break;
	case 4:
		m_donghua = str;
		break;
	}
}

void crCheckHuiKuiHuoDongCompleteMethod::operator()(crHandle &handle)
{
	if (crGlobalHandle::isClient())
	{
		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode>huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
		if (huodongcanvas.valid())
		{
			ref_ptr<crSequence> donghua = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua));
			ref_ptr<crButtonWidgetNode> huikuibtn = dynamic_cast<crButtonWidgetNode *>(huodongcanvas->getWidget(m_huikuibtn));
			ref_ptr<crStaticTextWidgetNode> countinput = dynamic_cast<crStaticTextWidgetNode *>(huodongcanvas->getWidget(m_countinput));
			ref_ptr<crNode> countbackboard = dynamic_cast<crNode *>(huodongcanvas->getChildNode(m_countbackboard));
			if (countbackboard.valid())
			{
				countbackboard->setVisiable(false);
			}
			if (countinput.valid())
			{
				countinput->clearString();
			}
			int cangetcount = 0;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if(!huodongTab.valid()) return;
			int displayid = huodongTab->getTitleIndex("活动显示");
			if (huikuibtn.valid() && huikuibtn->getVisiable())
			{
				crRole *mainRole = myPlayer->getMainRole();
				if(mainRole && mainRole->getDataClass())
				{
					crTableIO::StrVec record;
					crData *roleData = mainRole->getDataClass();
					roleData->excHandle(MAKEINT64(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->begin();
					for (; itr != huodongMap->end(); ++ itr)
					{
						if(itr->second->getActivation()==1 &&
							itr->second->isCanAdvance() &&
							!itr->second->getComplete())
						{
							if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
							{
								if(atoi(record[displayid].c_str()) == 2)
								{
									++cangetcount;
								}
							}
						}
					}
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
				}


				if (cangetcount > 0)
				{
					if (countbackboard.valid())
					{
						countbackboard->setVisiable(true);
					}
					if (countinput.valid())
					{
						countinput->setString(crArgumentParser::appItoa(cangetcount));
					}
				}

				//ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_JueSe");
				/*if (canvas.valid() && canvas->getDataClass())
				{*/
					/*ref_ptr<crData> canvasdata = canvas->getDataClass();
					canvasdata->getParam(WCHDATA_JXJIsClickHongDongBtn,param);
					crVector2s sign = *(crVector2s *)param;*/
					if(donghua.valid())
					{
						unsigned char isclickhuodong = crJXJSetIsClickHuoDongSignMethod::getIsclickhudongbtn(1);
						if (cangetcount <= 0 && isclickhuodong==1)
							donghua->setVisiable(false);
						else
							donghua->setVisiable(true);
					}
				//}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJServerOpenGroupFuBenMethod
 
**************************************************************************/
JXJ::crJXJServerOpenGroupFuBenMethod::crJXJServerOpenGroupFuBenMethod():
	m_this(NULL)
{

}

JXJ::crJXJServerOpenGroupFuBenMethod::crJXJServerOpenGroupFuBenMethod( const crJXJServerOpenGroupFuBenMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_lasttimer(0L)
{

}

void JXJ::crJXJServerOpenGroupFuBenMethod::inputParam( int i, void *param )
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

void JXJ::crJXJServerOpenGroupFuBenMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJServerOpenGroupFuBenMethod::operator()( crHandle &handle )
{
	time_t t = time(0);
	//if (m_lasttimer == 0)
	//{
	//	m_lasttimer = t;
	//}
	int dt = t - m_lasttimer;
	if(dt > 3600)
	{
		std::string opentime1 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenOpenTime1, 0);
		std::string opentime2 = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenOpenTime2, 0);
		char tmp[20];
		strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
		crVector3i opentm1,opentm2,curtm;
		crArgumentParser::appTimetoVec(opentime1,opentm1);
		crArgumentParser::appTimetoVec(opentime2,opentm2);
		crArgumentParser::appTimetoVec(tmp,curtm);
		if( (curtm[0] == opentm1[0] && (curtm[1] / 10) == (opentm1[1] / 10)) || ( curtm[0] == opentm2[0] && (curtm[1] / 10) == (opentm2[1] / 10 )))
		{
			m_lasttimer = t;
			//开启副本
			ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec strVec;
			void *param;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			if (gsBrainData && fubenTab.valid() && gameServer)
			{
				int timeid = fubenTab->getTitleIndex("时间");
				int countid = fubenTab->getTitleIndex("玩家数量");
				int nameid = fubenTab->getTitleIndex("name");
				int sceneidIdx = fubenTab->getTitleIndex("sceneid");
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				unsigned short fubenId = 3000;
				crTableIO::StrVec record;
				if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenId),record)>=0)
				{
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap consortiamap = *(ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap.begin();
					crData * groupData = NULL;
					bool create = false;
					int battleid = 0;
					char battleMode = crRoom::Extern;
					unsigned char shiliid = 0;
					bool friendlyFire = false;
					int roomid = 0;
					bool modify = true;
					int playerid = 0;
					ConsortiaPlayerReliveCount countmap;
					GroupLeaderVec* leaderVec;
					ConsortiaMemberMap* consortiamemberMap;
					ConsortiaMemberMap::iterator cmmitr;
					StartedFubenMap* startedFubenMap;

					int sceneid = atoi(record[sceneidIdx].c_str());
					int timeLimit = atoi(record[timeid].c_str());
					short playerCount = (short)(atoi(record[countid].c_str()));
					std::string battleName = record[nameid];
					crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					std::string serverAddress = netCallback->findSceneServerAddress(sceneid);
					for (;itr != consortiamap.end();++itr)
					{
						groupData = itr->second->getDataClass();
						if (groupData)
						{
							groupData->excHandle(MAKEINT64(WCH_LockData,1));
							groupData->getParam(WCHDATA_JXJConsortiaShiliID,param);
							shiliid = *(unsigned char *)param;
							groupData->getParam(WCHDATA_JXJConsortiaLeaderVec,param);
							leaderVec = (GroupLeaderVec *)param;
							ref_ptr<crGameServerPlayerData> playerData;
							if (!leaderVec->empty())
							{
								playerid = (*leaderVec)[0];
								playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
							}
							if(!playerData.valid())
							{
								groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
								consortiamemberMap = (ConsortiaMemberMap *)param;
								cmmitr = consortiamemberMap->begin();
								for(;cmmitr !=consortiamemberMap->end();++cmmitr)
								{
									playerid = cmmitr->first;
									playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
									if (playerData.valid())
									{
										break;
									}
								}
							}
							if (playerid > 0)
							{
								create = false;								
								battleid = MAKEINT32(MAKEINT16(shiliid,2),fubenId);											
								ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
								streamBuf->createBuf(28+battleName.length());
								streamBuf->_writeInt(playerid);
								streamBuf->_writeInt(itr->first);
								streamBuf->_writeInt(sceneid);
								streamBuf->_writeInt(battleid);//8
								streamBuf->_writeChar(battleMode);//1
								streamBuf->_writeString(battleName);//16+4
								streamBuf->_writeInt(timeLimit);//4
								streamBuf->_writeShort(playerCount);//2
								streamBuf->_writeBool(friendlyFire);//1
								if(!serverAddress.empty())
								{
									crJXJJuntuanCreateRoomPacket packet;
									crJXJJuntuanCreateRoomPacket::buildRequestPacket(packet,streamBuf.get());
									gameServer->getNetManager()->sendPacket(serverAddress,packet);
									create = true;
								}
								if (create)
								{
									roomid = 0;
									groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
									startedFubenMap = (StartedFubenMap *)param;
									startedFubenMap->insert(std::make_pair(fubenId,std::make_pair(roomid,countmap)));
									groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
								}
							}
							groupData->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			
		}
	}	

}

//////////////////////////////////////////////////////////////////////////
//crJXJUIXianShiTeMaiTipMethod
//////////////////////////////////////////////////////////////////////////
crJXJUIXianShiTeMaiTipMethod::crJXJUIXianShiTeMaiTipMethod()
{}
crJXJUIXianShiTeMaiTipMethod::crJXJUIXianShiTeMaiTipMethod(const crJXJUIXianShiTeMaiTipMethod& handle)
{

}
void crJXJUIXianShiTeMaiTipMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUIXianShiTeMaiTipMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}
void crJXJUIXianShiTeMaiTipMethod::operator()(crHandle &handle)
{
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	crData *uidata = m_this->getDataClass();
	unsigned short huodongID = 0;
	void *param;
	int Cost = 0;
	if (uidata)
	{
		uidata->getParam(0,param);
		huodongID = *(unsigned short *)param;
	}
	ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	crTableIO::StrVec record;
	if (huodongID > 0 && huodongTab.valid())
	{
		int tiaojianIdx = huodongTab->getTitleIndex("条件");
		if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
		{
			Cost = atoi(record[tiaojianIdx].c_str());
		}
	}
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(!msgCanvas)return;
	std::string strInfo ;
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	if(Cost > 0 && me)
	{
		//int hinttype = JXJGoldCostType_MeiRiXianGouHint;
		//crData *braindata = crBrain::getInstance()->getDataClass();
		//braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

		crData * data = me->getPlayerGameData()->getDataClass();
		data->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_MeiRiXianGouHint;
		if (ifhint)
		{
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text","您确认要花费");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(140.0);
			v_i.push_back(40.0);
			v_i.push_back(40.0);
			v_i.push_back(255.0);
			cfg_script.Add("Color",v_i);
			strInfo=crArgumentParser::appItoa(Cost) + std::string("元宝") ;
			cfg_script.Add("Text",strInfo);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			strInfo = crArgumentParser::appItoa(Cost);
			cfg_script.Add("Text","购买这些物品吗？");
			cfg_script.Pop();

			cfg_script.Pop();
			msg->setHypertext(cfg_script);
		}
		ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
		crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		crData *dlgData = msgCanvas->getDataClass();
		int commandtype = CDP_Widget;
		dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
		dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
		if (ifhint)
		{
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{
			if(okBtn.valid())okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
		}
	}
}

/**************************************************************************

METHOD: crJXJJunTuanFuBenXinHideMethod
 
**************************************************************************/

crJXJJunTuanFuBenXinHideMethod::crJXJJunTuanFuBenXinHideMethod():
	m_ishide(0)
{

}

crJXJJunTuanFuBenXinHideMethod::crJXJJunTuanFuBenXinHideMethod( const crJXJJunTuanFuBenXinHideMethod & handle ):
	crMethod(handle),
	m_xinxicanvas(handle.m_xinxicanvas),
	m_canvas(handle.m_canvas),
	m_ishide(handle.m_ishide)
{

}

void crJXJJunTuanFuBenXinHideMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJJunTuanFuBenXinHideMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_xinxicanvas = str;
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_ishide = (unsigned char)(atoi(str.c_str()));
		break;
	default:
		break;
	}

}

void crJXJJunTuanFuBenXinHideMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode>  Canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);

	if (Canvas.valid())
	{
		if (m_ishide == 1)
		{
			crFilterRenderManager::getInstance()->showCanvas(m_canvas,false);
			crFilterRenderManager::getInstance()->showCanvas(m_xinxicanvas,true);
		}
		else
		{
			crFilterRenderManager::getInstance()->showCanvas(m_canvas,true);
			crFilterRenderManager::getInstance()->showCanvas(m_xinxicanvas,false);
		}
	}
}

////////////////////////////////
//
//crCheck7DayHuoDongCompleteMethod
//回馈活动提示
///////////////////////////////////
crCheck7DayHuoDongCompleteMethod::crCheck7DayHuoDongCompleteMethod() :
	m_this(NULL)
{}

crCheck7DayHuoDongCompleteMethod::crCheck7DayHuoDongCompleteMethod(const crCheck7DayHuoDongCompleteMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_huodongcanvas(handle.m_huodongcanvas),
	m_countbackboard(handle.m_countbackboard),
	m_countinput(handle.m_countinput),
	m_7DayhuodongBtn(handle.m_7DayhuodongBtn),
	m_hddonghua(handle.m_hddonghua)
{
}

void crCheck7DayHuoDongCompleteMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crCheck7DayHuoDongCompleteMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_huodongcanvas = str;
		break;
	case 1:
		m_countbackboard = str;
		break;
	case 2:
		m_countinput = str;
		break;
	case 3:
		m_7DayhuodongBtn = str;
		break;
	case 4:
		m_hddonghua = str;
		break;
	default:
		break;
	}
}

void crCheck7DayHuoDongCompleteMethod::operator()(crHandle &handle)
{
	if (crGlobalHandle::isClient())
	{
		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode>huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
		if (huodongcanvas.valid() && myPlayer)
		{
			ref_ptr<crData> data = myPlayer->getDataClass();
			ref_ptr<crButtonWidgetNode> _7Dayhuodongbtn = dynamic_cast<crButtonWidgetNode *>(huodongcanvas->getWidget(m_7DayhuodongBtn));
			ref_ptr<crStaticTextWidgetNode> countinput = dynamic_cast<crStaticTextWidgetNode *>(huodongcanvas->getWidget(m_countinput));
			ref_ptr<crNode> countbackboard = dynamic_cast<crNode *>(huodongcanvas->getChildNode(m_countbackboard));
			ref_ptr<crSequence> hddonghua = dynamic_cast<crSequence *>(m_this->getChildNode(m_hddonghua));
			if (countbackboard.valid())
			{
				countbackboard->setVisiable(false);
			}
			if (countinput.valid())
			{
				countinput->clearString();
			}
			if (hddonghua.valid())
			{
				hddonghua->setVisiable(false);
			}
			int cangetcount = -1;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if(!huodongTab.valid()) return;
			int displayid = huodongTab->getTitleIndex("活动显示");
			int titleIdx = huodongTab->getTitleIndex("标题");
			std::string str1("VIP每日礼包");
			std::string str2("VIP每周礼包");
			if (_7Dayhuodongbtn.valid() && _7Dayhuodongbtn->getVisiable() && data.valid())
			{
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char myviplv = *(unsigned char *)param;
				crRole *mainRole = myPlayer->getMainRole();
				if(mainRole && mainRole->getDataClass())
				{
					crTableIO::StrVec record;
					crData *roleData = mainRole->getDataClass();
					roleData->excHandle(MAKEINT64(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator nhmitr = huodongMap->begin();
					for (; nhmitr != huodongMap->end(); ++ nhmitr)
					{
						if (nhmitr != huodongMap->end() && nhmitr->second.valid())
						{
							if(nhmitr->second->getActivation()==1 &&
								nhmitr->second->isCanAdvance() &&
								!nhmitr->second->getComplete())
							{
								if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(nhmitr->first),record)>=0)
								{
									if(atoi(record[displayid].c_str()) == 1)
									{
										if (myviplv != 0 || (record[titleIdx].compare(str1) != 0 && record[titleIdx].compare(str2) != 0))
										{
											++cangetcount;
										}
									}
								}
							}
						}
					}
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			
				if (cangetcount > 0)
				{
					if (countbackboard.valid())
					{
						countbackboard->setVisiable(true);
					}
					if (countinput.valid())
					{
						countinput->setString(crArgumentParser::appItoa(cangetcount));
					}
					/*if (hddonghua.valid())
					{
						hddonghua->setVisiable(true);
					}*/
				}
				//ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_JueSe");
				/*if (canvas.valid() && canvas->getDataClass())
				{
					ref_ptr<crData> canvasdata = canvas->getDataClass();
					canvasdata->getParam(WCHDATA_JXJIsClickHongDongBtn,param);
					crVector2s sign = *(crVector2s *)param;*/
					if(hddonghua.valid())
					{
						unsigned char isclickhuodong = crJXJSetIsClickHuoDongSignMethod::getIsclickhudongbtn(0);
						if (cangetcount <= 0 && isclickhuodong==1)
							hddonghua->setVisiable(false);
						else
							hddonghua->setVisiable(true);
					}
				//}
			}
		}
	}
}