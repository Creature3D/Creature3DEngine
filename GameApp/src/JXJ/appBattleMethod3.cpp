/**********************************************************************
*
*	文件:	   appBattleMethod3.cpp
*
*	描述:	   
*
*	作者:	   曹霖麟
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appBattleMethod3.h>
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
#include <CRProducer/crViewer.h>
#include <CRIOManager/crCookFile.h>
#include <JXJ/appData3.h>
#include <JXJ/appMethod.h>
#include <sys/stat.h>
#include <imm.h>
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
using namespace std;

//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryBattleBirthPointMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJQueryBattleBirthPointMethod::crJXJQueryBattleBirthPointMethod(){}
crJXJQueryBattleBirthPointMethod::crJXJQueryBattleBirthPointMethod(const crJXJQueryBattleBirthPointMethod& handle):
	crMethod(handle)
{
}
void crJXJQueryBattleBirthPointMethod::inputParam(int i, void *param)
{
}

void crJXJQueryBattleBirthPointMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryBattleBirthPointMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataSceneEventPacket packet;
		crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryBirthPoint,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

//////////////////////////////////////////////////////////////////////////
//在线奖励玩
//crJXJRecvQueryBirthPointMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvQueryBirthPointMethod::crJXJRecvQueryBirthPointMethod(){}
crJXJRecvQueryBirthPointMethod::crJXJRecvQueryBirthPointMethod(const crJXJRecvQueryBirthPointMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvQueryBirthPointMethod::inputParam(int i, void *param)
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

void crJXJRecvQueryBirthPointMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas_1 = str;
		break;
	case 1:
		m_strCanvas_2 = str;
		break;
	case 2:
		m_strCanvas_3 = str;
		break;
	default:
		break;
	}
}

void crJXJRecvQueryBirthPointMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == SceneServer)
		{
			bool success = false;
			void *param;
			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crNetManager *netManager = sceneServerConductor->getNetManager();
			crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());

			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				crRoom *room = netCallback->findRoom(playerData->getRoomID());
				if(room)
				{
					crData *roomData = room->getDataClass();
					if(roomData)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						roomData->excHandle(MAKEINT64(WCH_LockData,1));

						roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
						JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
						JXJCampBirthpointMap::iterator itr_map = campMap->begin();
						JXJCampBirthpoint::iterator itr_set;
						itr_map = campMap->begin();
						unsigned char mapSize = campMap->size();
						stream->createBuf(1+mapSize*130+8);

						stream->_writeUChar(mapSize);
						while (itr_map!=campMap->end())
						{
							stream->_writeUChar(itr_map->first);					//1
							stream->_writeUChar(itr_map->second.size());			//1
							itr_set = itr_map->second.begin();
							while (itr_set!=itr_map->second.end())
							{
								stream->_writeVec3i(itr_set->first);		//12
								stream->_writeString(itr_set->second);//20
								itr_set++;
							}
							itr_map++;
						}

						roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
						crVector2i battleRbuffids = *(crVector2i *)param;
						stream->_writeVec2i(battleRbuffids);

						crPlayerDataSceneEventPacket packet;
						crPlayerDataSceneEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvQueryBirthPoint,stream.get());
						netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

						roomData->excHandle(MAKEINT64(WCH_LockData,0));
						success = true;
					}
				}
				if(!success)
				{//战场已经结束
					crPlayerDataSceneEventPacket packet;
					crPlayerDataSceneEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvBattleClosed,NULL);
					netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
			{
				crData *roomData = room->getDataClass();
				if(roomData)
				{
					roomData->excHandle(MAKEINT64(WCH_LockData,1));
					roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
					JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
					campMap->clear();
					unsigned char mapSize = m_stream->_readUChar();
					unsigned shiliid = 0,setSize = 0;
					JXJCampBirthpoint campBirthPoint;
					std::string instanceName;
					crVector3 instancePoint;
					for (int i = 0; i < mapSize; i++)
					{
						campBirthPoint.clear();
						shiliid = m_stream->_readUChar();
						setSize = m_stream->_readUChar();
						for (int j = 0; j < setSize; j++)
						{
							instancePoint = m_stream->_readVec3i();
							instanceName = m_stream->_readString();
							campBirthPoint.insert(std::make_pair(instancePoint,instanceName));
						}
						campMap->insert(make_pair(shiliid,campBirthPoint));
					}

					ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas_1);
					ref_ptr<crCanvasNode>canvas_1 = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas_2);
					ref_ptr<crCanvasNode>canvas_2 = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas_3);
					if(canvas.valid() && canvas_1.valid() && canvas_2.valid() && !canvas->getVisiable() && !canvas_1->getVisiable() && !canvas_2->getVisiable())
					{
						const crVector2i &size = crBrain::getInstance()->getMapSize();
						if(size.x()>size.y())
						{
							crFilterRenderManager::getInstance()->doModal(canvas_1.get());
						}
						else if(size.x()<size.y())
						{
							crFilterRenderManager::getInstance()->doModal(canvas_2.get());
						}
						else
						{
							crFilterRenderManager::getInstance()->doModal(canvas.get());
						}
					}

					crVector2i battleRbuffids = m_stream->_readVec2i();
					roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
					crVector2i *battlebuff = (crVector2i *)param;
					*battlebuff = battleRbuffids;

					roomData->excHandle(MAKEINT64(WCH_LockData,0));
					//ref_ptr<crCanvasNode> kaocanvas = crFilterRenderManager::getInstance()->findCanvas("UI_CopyBattle_Name");
					//kaocanvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryGameServerTimerMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJQueryGameServerTimerMethod::crJXJQueryGameServerTimerMethod(){}
crJXJQueryGameServerTimerMethod::crJXJQueryGameServerTimerMethod(const crJXJQueryGameServerTimerMethod& handle):
	crMethod(handle)
{
}
void crJXJQueryGameServerTimerMethod::inputParam(int i, void *param)
{
}

void crJXJQueryGameServerTimerMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryGameServerTimerMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvServerTimer,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJInviteLinkClickMethod
//
/////////////////////////////////////////
crJXJInviteLinkClickMethod::crJXJInviteLinkClickMethod():
	m_ea(NULL),
	m_this(NULL),
	m_inviteType(0),
	m_invitePlayerId(0),
	m_inviteid(0)
{
}
crJXJInviteLinkClickMethod::crJXJInviteLinkClickMethod(const crJXJInviteLinkClickMethod& handle):
	crMethod(handle),
	m_ea(NULL),
	m_this(NULL),
	m_inviteType(0),
	m_invitePlayerId(0),
	m_inviteid(0)
{
}
void crJXJInviteLinkClickMethod::inputParam(int i, void *param)
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
			//m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_inviteType = *(unsigned char*)param;
		break;
	case 4:
		m_invitePlayerId = *(int*)param;
		break;
	case 5:
		m_inviteid = *(unsigned short *)param;
		break;
	}
}
void crJXJInviteLinkClickMethod::addParam(int i, const std::string& str)
{
}

void crJXJInviteLinkClickMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();

	if(m_inviteType>0 && m_invitePlayerId>0 && playerGameData && playerGameData->getDataClass())
	{
		crData *playerData = playerGameData->getDataClass();
		playerData->inputParam(WCHDATA_JXJInviteType,&m_inviteType);
		playerData->inputParam(WCHDATA_JXJInviteID,&m_invitePlayerId);
		playerData->inputParam(WCHDATA_JXJInviteChengChiID,&m_inviteid);
	}
}

/////////////////////////////////////////
//
//crJXJEnterBattleDequeClickMethod
//
/////////////////////////////////////////
crJXJEnterBattleDequeClickMethod::crJXJEnterBattleDequeClickMethod():
	m_ea(NULL),
	m_this(NULL),
	m_time(0L)
{
}
crJXJEnterBattleDequeClickMethod::crJXJEnterBattleDequeClickMethod(const crJXJEnterBattleDequeClickMethod& handle):
	crMethod(handle),
	m_ea(NULL),
	m_this(NULL),
	m_time(handle.m_time)
{
}
void crJXJEnterBattleDequeClickMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
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
	case 3:
		m_strCityName = *(std::string *)param;
		break;
	}
}
void crJXJEnterBattleDequeClickMethod::addParam(int i, const std::string& str)
{
}

void crJXJEnterBattleDequeClickMethod::operator ()(crHandle &handle)
{
	//根据m_strCityName查找cityid
	time_t nowtime  = time(0);
	if(m_time == 0L || nowtime-m_time > 1)
	{
		m_time = nowtime;
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(chengchiTab.valid() && netConductor)
		{
			int idIndex = chengchiTab->getTitleIndex("id");
			int nameIndex = chengchiTab->getTitleIndex("名字");
			crTableIO::StrVec record;
			if(idIndex>=0 && nameIndex>=0 && \
				chengchiTab->queryOneRecord(nameIndex,m_strCityName,record)>=0)
			{
				int chengchiid = atoi(record[idIndex].c_str());

				if(chengchiid>0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(3);
					stream->_writeUShort(chengchiid);
					stream->_writeUChar(0);//要防守的势力
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvEnterBattleDeque,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Clas: crJXJPutInChatRecordDequeMethod
//
//////////////////////////////////////////////////////////////////////////
int crJXJPutInChatRecordDequeMethod::m_slinkIndex = 10;
JXJ::crJXJPutInChatRecordDequeMethod::crJXJPutInChatRecordDequeMethod():m_textRow(0),m_characterSize(16)
{}
JXJ::crJXJPutInChatRecordDequeMethod::crJXJPutInChatRecordDequeMethod(const crJXJPutInChatRecordDequeMethod& handle):crMethod(handle),m_textRow(handle.m_textRow),
	m_characterSize(16)
{}
void JXJ::crJXJPutInChatRecordDequeMethod::inputParam(int i, void *param)
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
		m_this = (crBrain *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_chatInfo = (crJXJChatRecord *)LOINT64(param64);
		}
		else
		{
			m_chatInfo = NULL;
		}
		break;
	}
}
void JXJ::crJXJPutInChatRecordDequeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strChatSetCanvas = str;
		break;
	case 1:
		m_chat_canvas = str;
		break;
	case 2:
		m_chat_show = str;
		break;
	case 3:
		m_chat_mode = str;
		break;
	case 4:
		m_chat_cfg = str;
		break;
	case 5:
		m_input = str;
		break;
	case 6:
		m_scroll = str;
		break;
	case 7:
		m_RoomChatCanvas  = str;
		break;
	case 8:
		m_characterSize = atoi(str.c_str());
		break;
	}
}
void JXJ::crJXJPutInChatRecordDequeMethod::operator()(crHandle &handle)
{
	void *param;
	// widgets
	//if(!crRunGameHandle::getInstance()->isInManor() && !crRunGameHandle::getInstance()->isInBattle()) return;
	//ref_ptr<crCanvasNode>chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_chat_canvas);
	//if(!chat_canvas.valid()) chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_RoomChatCanvas);
	////if(!chat_canvas.valid()) return;
	//ref_ptr<crCanvasNode> chatSetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strChatSetCanvas);

	ref_ptr<crCanvasNode>chat_canvas;
	ref_ptr<crCanvasNode> chatSetCanvas;
	ref_ptr<crHypertextWidgetNode>chat_show;
	ref_ptr<crHypertextWidgetNode>chat_show2;
	ref_ptr<crRadioGroupWidgetNode>chat_select;
	ref_ptr<crEditWidgetNode> chat_input;
	ref_ptr<crScrollBarWidgetNode>scrollbar;
	if(crRunGameHandle::getInstance()->isInManor() || crRunGameHandle::getInstance()->isInBattle())
	{
		chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_chat_canvas);
		if(!chat_canvas.valid()) chat_canvas = crFilterRenderManager::getInstance()->findCanvas(m_RoomChatCanvas);
		chatSetCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strChatSetCanvas);
		if(chat_canvas.valid())
		{
			chat_show = dynamic_cast<crHypertextWidgetNode *>(chat_canvas->getWidget(m_chat_show));
			chat_show2 = dynamic_cast<crHypertextWidgetNode *>(chat_canvas->getWidget(m_chat_show + "2"));
			chat_select = dynamic_cast<crRadioGroupWidgetNode *>(chat_canvas->getChildNode(m_chat_mode));
			chat_input = dynamic_cast<crEditWidgetNode *>(chat_canvas->getChildNode(m_input));
			scrollbar = dynamic_cast<crScrollBarWidgetNode *>(chat_canvas->getWidget(m_scroll));
		}
	}

	CRNetApp::crPlayerGameData *player = CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData();
	crData *player_data = player->getDataClass();
	player_data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	if(chat_show.valid())
		chat_show->setDefaultScrollPos(1);
	if(chat_show2.valid())
		chat_show2->setDefaultScrollPos(1);
	int max_row = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine,viplv).c_str());
	//if (chat_select.valid())
	{
		std::string myname = player->getCharacterName();
		JXJChatMode chat_mode = ChatAll;
		if (chat_select.valid())
		{
			chat_mode = JXJChatMode(chat_select->getSelect());
		}
		void *param;

		crMyPlayerData *me = crMyPlayerData::getInstance();
		if(me && me->getPlayerGameData())
		{
			crData *playerData = me->getPlayerGameData()->getDataClass();
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(playerData && itemtab.valid())
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				
				playerData->getParam(WCHDATA_JXJChatItemLinkMap,param);
				std::map<int,std::pair<int,int>> * itemLinkMap = (std::map<int,std::pair<int,int>> *)param;
				playerData->getParam(WCHDATA_JXJChatInviteLinkMap,param);
				std::map<int,InviteInfo>  * invitelinkMap = (std::map<int,InviteInfo> *)param;
				
				playerData->getParam(WCHDATA_JXJChatBattleReportLinkMap, param);
				std::map<int, std::pair<std::string, int>>  * battleReportlinkMap = (std::map<int, std::pair<std::string, int>> *)param;

				playerData->getParam(WCHDATA_JXJChatPraiseLinkMap, param);
				std::map<int, int>  * praiselinkMap = (std::map<int, int> *)param;

				playerData->getParam(WCHDATA_JXJChatCallForHelpLinkMap, param);
				std::map<int, std::pair<int, int>>  * callForHelplinkMap = (std::map<int, std::pair<int, int>> *)param;

				rcfg::ConfigScript cfg_script;
				if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_chat_cfg,true,""))
				{
					CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJChatReshowMethod(): file open error "<<m_chat_cfg<<std::endl;
					//sprintf(gDebugInfo->buf(),"crJXJChatReshowMethod(): file open error %s\n\0",m_chat_cfg.c_str());
					//gDebugInfo->debugInfo(CRCore::FATAL);
					playerData->excHandle(MAKEINT64(WCH_LockData,0));
					return;
				}
				std::vector<float>v_i;
				// draw
				m_linkIdMap.clear();
				m_inviteIdMap.clear();
				m_noticeIdMap.clear();
				m_reportIdMap.clear();
				m_praiseIdMap.clear();
				m_achieveIdMap.clear();
				int index = 1;
				cfg_script.Push("Hypertext");
				cfg_script.Add("CharacterSize",m_characterSize);
				m_strVec.clear();
				std::string tempStr = std::string("");
				std::string chatChannel;
				crVector4f color = crVector4f(255.0f,255.0f,255.0f,255.0f);
				crVector4f colorRed = crVector4f(255.0f,0.0f,0.0f,255.0f);

				ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
				crTableIO::StrVec colorrecord;
				crTableIO::StrVec itemrecord,record2;
				int colorindex = qualitytab->getTitleIndex("chatcolor");
				int qualityindex = itemtab->getTitleIndex("品质");
				int typeindex = itemtab->getTitleIndex("类型");

				int maxDequeSize = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine, viplv).c_str());

				//if(chat_mode == ChatInWorld)
				//{
				//	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInWorldColor,viplv),color);
				//}
				//else if(chat_mode == ChatInCountry)
				//{
				//	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInCountryColor,viplv),color);
				//}
				//else if(chat_mode == ChatInGroup)
				//{
				//	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInGroupColor,viplv),color);
				//}
				//else if(chat_mode == ChatInTeam)
				//{
				//	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInTeamColor,viplv),color);
				//}
				//else if(chat_mode== ChatOneToOne)
				//{
				//	crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatOneToOneColor,viplv),color);
				//}

				//for (ChatRecordDeque::iterator itr = deque.begin();itr!=deque.end();itr++)
				
				v_i.clear();
				std::string chatText = m_chatInfo->getChattext();
				std::string chatName = m_chatInfo->getSenderName();
				std::string chatReciverName = m_chatInfo->getReciverName();
				std::string sendername = chatName;
				if (chatText.length())
				{
					chatChannel = m_chatInfo->getChatChannelName();
					if(chatChannel.compare("[世界]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInWorldColor,viplv),color);
					}
					else if(chatChannel.compare("[国家]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInCountryColor,viplv),color);
					}
					else if(chatChannel.compare("[军团]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInGroupColor,viplv),color);
					}
					else if(chatChannel.compare("[组队]")==0 || chatChannel.compare("[战场]")==0 )
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatInTeamColor,viplv),color);
					}
					else if(chatChannel.compare("[私聊]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatOneToOneColor,viplv),color);
					}
					else if(chatChannel.compare("[系统]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatDefaultColor,viplv),color);
					}
					else if(chatChannel.compare("[通告]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatDefaultColor,viplv),color);
					}
					else if(chatChannel.compare("[游戏]")==0)
					{
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChatDefaultColor,viplv),color);
					}


					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					v_i.resize(0);
					for (int i = 0; i<4;i++)
					{
						v_i.push_back(color[i]);
					}
					cfg_script.Add("Color",v_i);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("Text",chatChannel);
					cfg_script.Pop();

					if(/*chat_mode == ChatAll ||*/ chatChannel.compare("[世界]")==0)
					{
						unsigned char shili = m_chatInfo->getSenderShiliID();

						ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
						if(playershilitab.valid() && playershilitab->queryOneRecord(0,crArgumentParser::appItoa(shili),record2)>=0)
						{
							int shilinameindex = playershilitab->getTitleIndex("name");
							if(shilinameindex>=0 ) 
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{ 
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",std::string("[")+record2[shilinameindex]+std::string("]"));

								cfg_script.Pop();
							}
						}
					}

					if(chatName.length())
					{
						if(player->getCharacterName().compare(chatName)==0 && chatChannel.compare("[私聊]")==0)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("Text",std::string("我对"));
							cfg_script.Pop();
							chatName = chatReciverName;
						}

						cfg_script.Add("Content");
						cfg_script.Push("Content",index++);
						v_i.resize(0);

						if(player->getCharacterName().compare(chatName)!=0) 
						{
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color",v_i);

							cfg_script.Add("DrawMode","TEXT|UNDERLINE");
						}
						else
						{
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color",v_i);	
							cfg_script.Add("DrawMode","TEXT");
						}
						cfg_script.Add("Text",chatName);
						if(player->getCharacterName().compare(chatName)!=0)
						{
							cfg_script.Add("Link",1);
						}
						cfg_script.Pop();
						chatName = m_chatInfo->getSenderName();

						if(player->getCharacterName().compare(chatName)!=0 && chatChannel.compare("[私聊]")==0)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("DrawMode","TEXT");
							cfg_script.Add("Text",std::string("对我说"));
							cfg_script.Pop();
						}
						if(player->getCharacterName().compare(chatName)==0 && chatChannel.compare("[私聊]")==0)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("DrawMode","TEXT");
							cfg_script.Add("Text",std::string("说"));
							cfg_script.Pop();
						}
					}

					analysisChatText(chatText);
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("Text",std::string(":"));
					cfg_script.Pop();
					crVector4f colorItem = color;
					if(!m_strVec.empty())
					{
						ChatItemInfoVec::iterator itr_str = m_strVec.begin();
						for (;itr_str!=m_strVec.end();itr_str++)
						{
							if(itr_str->first.first>0)
							{
								if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itr_str->first.first),itemrecord)>=0)
								{
									if(qualityindex>=0)
									{
										if(atoi(itemrecord[typeindex].c_str()) == IT_Jiangka && atoi(itemrecord[qualityindex].c_str())>CardColor_Orange)
										{
											if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(CardColor_Orange),colorrecord)>=0)
											{
												if (colorindex>=0)
												{
													crArgumentParser::appAtoVec(colorrecord[colorindex],colorItem);
												}
											}
										}
										else
										{
											if (qualitytab->queryOneRecord(0,itemrecord[qualityindex],colorrecord)>=0)
											{
												if (colorindex>=0)
												{
													crArgumentParser::appAtoVec(colorrecord[colorindex],colorItem);
												}
											}
										}
									}
								}

								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(colorItem[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",std::string("["));
								cfg_script.Pop();

								if(itr_str->second.length()<=2)
									tempStr = itr_str->second;
								else
									tempStr = itr_str->second.substr(1,itr_str->second.length()-2);
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(colorItem[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT|UNDERLINE");
								cfg_script.Add("Text",tempStr);
								cfg_script.Add("Link",m_slinkIndex);
								m_linkIdMap.insert(std::make_pair(m_slinkIndex,itr_str->first));
								itemLinkMap->insert(std::make_pair(m_slinkIndex,itr_str->first));
								cfg_script.Pop();
								m_slinkIndex++;

								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(colorItem[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",std::string("]"));
								cfg_script.Pop();
							}
							else
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",itr_str->second);
								cfg_script.Pop();
							}
						}
					}
					if(!m_strNoticeVec.empty())
					{
						SystemChatInfoVec::iterator itr_str = m_strNoticeVec.begin();
						for (;itr_str!=m_strNoticeVec.end();itr_str++)
						{
							if(itr_str->first == 3)
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT|UNDERLINE");
								cfg_script.Add("Text",itr_str->second);
								cfg_script.Add("Link",m_slinkIndex);
								cfg_script.Pop();

								m_noticeIdMap.insert(std::make_pair(m_slinkIndex,itr_str->second));
								m_slinkIndex++;
								
							}
							else
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",itr_str->second);
								cfg_script.Pop();
							}
						}
					}
					if(!m_strInviteVec.empty())
					{
						InviteChatInfoVec::iterator itr_str = m_strInviteVec.begin();
						for (;itr_str!=m_strInviteVec.end();itr_str++)
						{
							if(itr_str->first.type > 0 && itr_str->first.playerid > 0)
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT|UNDERLINE");
								cfg_script.Add("Text",itr_str->second);
								cfg_script.Add("Link",m_slinkIndex);
								cfg_script.Pop();

								m_inviteIdMap.insert(std::make_pair(m_slinkIndex,itr_str->first));
								invitelinkMap->insert(std::make_pair(m_slinkIndex,itr_str->first));
								m_slinkIndex++;
							}
							else
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content",index++);
								v_i.resize(0);
								for (int i = 0; i<4;i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color",v_i);
								cfg_script.Add("DrawMode","TEXT");
								cfg_script.Add("Text",itr_str->second);
								cfg_script.Pop();
							}
						}
					}
					if (!m_strReportVec.empty())
					{
						BattleReportInfoVec::iterator itr_report = m_strReportVec.begin();
						for (; itr_report != m_strReportVec.end();itr_report++)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content", index++);
							v_i.resize(0);
							for (int i = 0; i<4; i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color", v_i);
							cfg_script.Add("DrawMode", "TEXT");
							cfg_script.Add("Text", itr_report->second);
							cfg_script.Pop();

							if (itr_report->first.second > 0)
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content", index++);
								v_i.resize(0);
								for (int i = 0; i < 4; i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color", v_i);
								cfg_script.Add("DrawMode", "TEXT|UNDERLINE");
								cfg_script.Add("Text", std::string("详细战报"));
								cfg_script.Add("Link", m_slinkIndex);
								cfg_script.Pop();

								m_reportIdMap.insert(std::make_pair(m_slinkIndex, \
									std::make_pair(itr_report->first.first,itr_report->first.second)));
								battleReportlinkMap->insert(std::make_pair(m_slinkIndex, \
									std::make_pair(itr_report->first.first, itr_report->first.second)));
								m_slinkIndex++;
							}
						}
					}
					if (!m_strPraiseVec.empty())
					{
						PraiseInfoVec::iterator itr_report = m_strPraiseVec.begin();
						for (; itr_report != m_strPraiseVec.end();itr_report++)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content", index++);
							v_i.resize(0);
							for (int i = 0; i<4; i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color", v_i);
							cfg_script.Add("DrawMode", "TEXT");
							cfg_script.Add("Text", itr_report->second);
							cfg_script.Pop();

							if (itr_report->first > 0)
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content", index++);
								v_i.resize(0);
								crVector4f preasecolor = crVector4f(0.0f,255.0f,0.0f,255.0f);
								for (int i = 0; i < 4; i++)
								{
									v_i.push_back(preasecolor[i]);
								}
								cfg_script.Add("Color", v_i);
								cfg_script.Add("DrawMode", "TEXT");
								cfg_script.Add("Text", std::string("[点赞]"));
								cfg_script.Add("Link", m_slinkIndex);
								cfg_script.Pop();

								m_praiseIdMap.insert(std::make_pair(m_slinkIndex, itr_report->first));
								praiselinkMap->insert(std::make_pair(m_slinkIndex, itr_report->first));
								m_slinkIndex++;
							}
							cfg_script.Add("Content");
							cfg_script.Push("Content", index++);
							v_i.resize(0);
							for (int i = 0; i<4; i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color", v_i);
							cfg_script.Add("DrawMode", "TEXT");
							cfg_script.Add("Text", std::string("吧"));
							cfg_script.Pop();
						}
					}
					if(!m_strAchieveVec.empty())
					{
						colorItem = crVector4f(255.0f,160.0f,60.0f,255.0f);
						ChatAchieveInfoVec::iterator itr_str = m_strAchieveVec.begin();
						for (;itr_str!=m_strAchieveVec.end();itr_str++)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(colorItem[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("DrawMode","TEXT");
							cfg_script.Add("Text",std::string("["));
							cfg_script.Pop();

							if(itr_str->second.length()<=2)
								tempStr = itr_str->second;
							else
								tempStr = itr_str->second.substr(1,itr_str->second.length()-2);
							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(colorItem[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("DrawMode","TEXT|UNDERLINE");
							cfg_script.Add("Text",tempStr);
							cfg_script.Add("Link",m_slinkIndex);
							m_achieveIdMap.insert(std::make_pair(m_slinkIndex,itr_str->first));
							//itemLinkMap->insert(std::make_pair(m_slinkIndex,itr_str->first));
							cfg_script.Pop();
							m_slinkIndex++;

							cfg_script.Add("Content");
							cfg_script.Push("Content",index++);
							v_i.resize(0);
							for (int i = 0; i<4;i++)
							{
								v_i.push_back(colorItem[i]);
							}
							cfg_script.Add("Color",v_i);
							cfg_script.Add("DrawMode","TEXT");
							cfg_script.Add("Text",std::string("]"));
							cfg_script.Pop();
						}
					}
					if (!m_strCallForHelpVec.empty())
					{
						CallForHelpInfoVec::iterator itr_help = m_strCallForHelpVec.begin();
						for (; itr_help != m_strCallForHelpVec.end(); itr_help++)
						{
							cfg_script.Add("Content");
							cfg_script.Push("Content", index++);
							v_i.resize(0);
							for (int i = 0; i<4; i++)
							{
								v_i.push_back(color[i]);
							}
							cfg_script.Add("Color", v_i);
							cfg_script.Add("DrawMode", "TEXT");
							cfg_script.Add("Text", itr_help->second);
							cfg_script.Pop();

							if (itr_help->first.roomid != 0 && \
								itr_help->first.secneid !=0)
							{
								cfg_script.Add("Content");
								cfg_script.Push("Content", index++);
								v_i.resize(0);
								for (int i = 0; i < 4; i++)
								{
									v_i.push_back(color[i]);
								}
								cfg_script.Add("Color", v_i);
								cfg_script.Add("DrawMode", "TEXT|UNDERLINE");
								cfg_script.Add("Text", std::string("点击进入"));
								cfg_script.Add("Link", m_slinkIndex);
								cfg_script.Pop();

								m_helpIdMap.insert(std::make_pair(m_slinkIndex, \
									std::make_pair(itr_help->first.roomid, itr_help->first.secneid)));
								callForHelplinkMap->insert(std::make_pair(m_slinkIndex, \
									std::make_pair(itr_help->first.roomid, itr_help->first.secneid)));
								m_slinkIndex++;
							}
						}
					}
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("Text",std::string(" /n"));
					cfg_script.Pop();
				}
				
				cfg_script.Pop();

				ref_ptr<crData> chatData;
				ref_ptr<crData> chatData2;
				if(chat_show.valid())
				{
					if(chat_show->getValidTextSize()>max_row*2)
						chat_show->popTexts(max_row/2);
					chatData = chat_show->getDataClass();
				}
				if(chat_show2.valid())
				{
					if(chat_show2->getValidTextSize()>max_row*2)
						chat_show2->popTexts(max_row/2);
					chatData2 = chat_show2->getDataClass();
				}

				if(itemLinkMap->size()>6*max_row)
				{			
					std::map<int,std::pair<int,int>>::iterator itr_link;
					for (int i = 0;i<max_row;i++)
					{
						itr_link = itemLinkMap->begin();
						itemLinkMap->erase(itr_link);
					}
				}

				if(invitelinkMap->size()>6*max_row)
				{			
					std::map<int,InviteInfo>::iterator itr_link;
					for (int i = 0;i<max_row;i++)
					{
						itr_link = invitelinkMap->begin();
						invitelinkMap->erase(itr_link);
					}
				}
				playerData->getParam(WCHDATA_JXJChatRecordMap,param);
				ChatRecordMap *chatRecordMap = (ChatRecordMap *)param;
				
				ref_ptr<crStreamBuf> rushinfoBuffer = new crStreamBuf;
				crGlobalHandle::getInstance()->getScriptStream(cfg_script,rushinfoBuffer);

				playerData->getParam(WCHDATA_JXJChatChannelSetInfo,param);
				BitSet64 & userSet = *(BitSet64 *)param;

				BitSet64 chatChannelSet = userSet;
				if(chatSetCanvas.valid())
				{
					crData * canvasData = chatSetCanvas->getDataClass();
					if(canvasData)
					{
						canvasData->excHandle(MAKEINT64(WCH_LockData,1));

						canvasData->getParam(WCHDATA_JXJChatChannelDefaultSet,param);
						BitSet64 & defaultSet = *(BitSet64 *)param;
						
						chatChannelSet |= defaultSet;
						canvasData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
								
				//插入聊天存储队列

				int channelIndex = 0;
				for (int i = ChatAll;i<ChatNull;i++)
				{
					ChatRecordDeque & chatDeque = (*chatRecordMap)[(JXJChatMode)i];
					channelIndex = 0;
					if(i == ChatInWorld)
					{
						channelIndex = 9;
					}
					else if(i == ChatInCountry)
					{
						channelIndex = 2*9;
					}
					else if(i == ChatInGroup)
					{
						channelIndex = 3*9;
					}
					else if(i == ChatInTeam)
					{
						if(crRunGameHandle::getInstance()->isInManor())
						{
							channelIndex = 4*9;
						}
						else if(crRunGameHandle::getInstance()->isInBattle())
						{
							channelIndex = 5*9;
						}
					}
					else if(i == ChatOneToOne)
					{
						channelIndex = 6*9;
					}

					if(chatDeque.size()>=maxDequeSize)
					{
						chatDeque.pop_front();
					}
					
					//分类存储
					bool canAdd = false;
					if(chatChannel.compare("[世界]")==0 && chatChannelSet[channelIndex])
					{					
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[国家]")==0 && chatChannelSet[channelIndex+1])
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[军团]")==0 && chatChannelSet[channelIndex+2])
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[组队]")==0 && chatChannelSet[channelIndex+3])
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[战场]")==0 && chatChannelSet[channelIndex+4])
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[私聊]")==0 && chatChannelSet[channelIndex+5])
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[系统]")==0 && (i == ChatAll || chatChannelSet[channelIndex+6]))
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[通告]")==0 && (i == ChatAll || chatChannelSet[channelIndex+7]))
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}
					else if(chatChannel.compare("[游戏]")==0 && (i == ChatAll || chatChannelSet[channelIndex+8]))
					{
						chatDeque.push_back(rushinfoBuffer.get());
						canAdd = true;
					}

					//插入当前队列
					if(canAdd && chat_mode == i)
					{
						if(chat_show.valid())
							chat_show->setHypertext(cfg_script,true);
						if(chat_show2.valid())
							chat_show2->setHypertext(cfg_script,true);
					}
				}

				crHandle *linkhandle = NULL;
				crHandle *clonelink = NULL;
				if (chatData.valid())
				{
					std::map<int, std::pair<int, int>>::iterator itr_link = m_linkIdMap.begin();
					for (; itr_link != m_linkIdMap.end(); itr_link++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 2));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link->second.first);
						clonelink->inputParam(4, &itr_link->second.second);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link->first), clonelink);
					}

					std::map<int, InviteInfo>::iterator itr_link_i = m_inviteIdMap.begin();
					for (; itr_link_i != m_inviteIdMap.end(); itr_link_i++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 4));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_i->second.type);
						clonelink->inputParam(4, &itr_link_i->second.playerid);
						clonelink->inputParam(5, &itr_link_i->second.id);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_i->first), clonelink);
					}

					std::map<int, std::string>::iterator itr_link_n = m_noticeIdMap.begin();
					for (; itr_link_n != m_noticeIdMap.end(); itr_link_n++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 3));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_n->second);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_n->first), clonelink);
					}

					std::map<int, std::pair<std::string, int>>::iterator itr_link_re = m_reportIdMap.begin();
					for (; itr_link_re != m_reportIdMap.end(); itr_link_re++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 5));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_re->second.first);
						clonelink->inputParam(4, &itr_link_re->second.second);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_re->first), clonelink);
					}

					std::map<int, int>::iterator itr_link_pre = m_praiseIdMap.begin();
					for (; itr_link_pre != m_praiseIdMap.end(); itr_link_pre++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 7));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_pre->second);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_pre->first), clonelink);
					}

					std::map<int, std::pair<int, int>>::iterator itr_link_ach = m_achieveIdMap.begin();
					for (; itr_link_ach != m_achieveIdMap.end(); itr_link_ach++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 8));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_ach->second.first);
						clonelink->inputParam(4, &itr_link_ach->second.second);
						clonelink->inputParam(5, &sendername);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_ach->first), clonelink);
					}

					std::map<int, std::pair<int, int>>::iterator itr_link_help = m_helpIdMap.begin();
					for (; itr_link_help != m_helpIdMap.end(); itr_link_help++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 6));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_help->second.first);
						clonelink->inputParam(4, &itr_link_help->second.second);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_help->first), clonelink);
					}
				}

				if (chatData2.valid())
				{
					std::map<int, std::pair<int, int>>::iterator itr_link = m_linkIdMap.begin();
					for (; itr_link != m_linkIdMap.end(); itr_link++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 2));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link->second.first);
						clonelink->inputParam(4, &itr_link->second.second);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link->first), clonelink);
					}
					std::map<int, InviteInfo>::iterator itr_link_i = m_inviteIdMap.begin();
					for (; itr_link_i != m_inviteIdMap.end(); itr_link_i++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 4));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_i->second.type);
						clonelink->inputParam(4, &itr_link_i->second.playerid);
						clonelink->inputParam(5, &itr_link_i->second.id);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_i->first), clonelink);
					}

					std::map<int, std::string>::iterator itr_link_n = m_noticeIdMap.begin();
					for (; itr_link_n != m_noticeIdMap.end(); itr_link_n++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 3));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_n->second);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_n->first), clonelink);
					}
					std::map<int, std::pair<std::string, int>>::iterator itr_link_re = m_reportIdMap.begin();
					for (; itr_link_re != m_reportIdMap.end(); itr_link_re++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 5));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_re->second.first);
						clonelink->inputParam(4, &itr_link_re->second.second);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_re->first), clonelink);
					}
					std::map<int, int>::iterator itr_link_pre = m_praiseIdMap.begin();
					for (; itr_link_pre != m_praiseIdMap.end(); itr_link_pre++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 7));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_pre->second);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_pre->first), clonelink);
					}
					std::map<int, std::pair<int, int>>::iterator itr_link_ach = m_achieveIdMap.begin();
					for (; itr_link_ach != m_achieveIdMap.end(); itr_link_ach++)
					{
						linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown, 8));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_ach->second.first);
						clonelink->inputParam(4, &itr_link_ach->second.second);
						clonelink->inputParam(5, &sendername);
						chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_ach->first), clonelink);
					}
					std::map<int, std::pair<int, int>>::iterator itr_link_help = m_helpIdMap.begin();
					for (; itr_link_help != m_helpIdMap.end(); itr_link_help++)
					{
						linkhandle = chatData2->getHandle(MAKEINT64(WCH_UI_LBtnDown, 6));
						clonelink = linkhandle->clone();
						clonelink->inputParam(3, &itr_link_help->second.first);
						clonelink->inputParam(4, &itr_link_help->second.second);
						chatData2->insertHandle(MAKEINT64(WCH_UI_LBtnDown, itr_link_help->first), clonelink);
					}
				}
				//cfg_script.Write("12306.cfg");
				cfg_script.Close();
				//crBrain::getInstance()->doEvent(WCH_JXJUIUpdateChatMessageList);
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}
void crJXJPutInChatRecordDequeMethod::analysisChatText(std::string text)
{
	m_strVec.clear();
	m_strNoticeVec.clear();
	m_strInviteVec.clear();
	m_strReportVec.clear();
	m_strCallForHelpVec.clear();
	m_strPraiseVec.clear();
	m_strAchieveVec.clear();
	int indexFront = 0;
	unsigned char invitetype = 0;
	int inviteplayerid = 0;
	unsigned short fubenOrChengchiID = 0;
	int indexBack = text.length();
	std::string strFront;
	std::string strMid;
	std::string strItemName;
	std::string strItemIds;
	std::string strItemIDPair;
	std::string strInviteInfo;
	std::string strTempInviteInfo;
	std::pair<std::pair<int,int>,std::string> strPairFront;
	std::pair<std::pair<int,int>,std::string> strPair;
	std::pair<unsigned char,std::string> strLinkPair;
	std::pair<InviteInfo,std::string> strInvitePair;
	//std::pair< std::pair<std:::string, int>, std::string > strBattleReport;
	std::pair<std::pair<std::string, int>, std::string> strBattleReport;
	std::pair<int,std::string> strpraiseReport;
	std::pair<std::pair<int,int>,std::string> strAchieveInfo;
	std::pair<CallForHelpInfo, std::string> strCallForHelpInfo;
	ShowItemIDVec itemIdVec;
	crTableIO::StrVec itemrecord;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	int nameindex = itemtab->getTitleIndex("name");

	int typeIndexF = text.find_first_of("^");
	int typeIndexB = text.find_last_of("^");
	int chatIndexValue = atoi(text.substr(typeIndexF+1, typeIndexB-typeIndexF-1).c_str());
	if(typeIndexF>=0 && typeIndexB >0 && 3 == chatIndexValue)//解析城战通告
	{
		text = text.substr(typeIndexB+1);
		int indexFront = text.find_first_of("[");
		int indexBack = text.find_first_of("]");
		strFront = text.substr(0,indexFront+1);
		if(!strFront.empty())
		{
			strLinkPair.first = 0;
			strLinkPair.second = strFront;
			m_strNoticeVec.push_back(strLinkPair);
		}
		strMid = text.substr(indexFront+1,indexBack-indexFront-1);
		if(!strMid.empty())
		{
			strLinkPair.first = 3;
			strLinkPair.second = strMid;
			m_strNoticeVec.push_back(strLinkPair);
		}
		text = text.substr(indexBack);
		strLinkPair.first = 0;
		strLinkPair.second = text;
		m_strNoticeVec.push_back(strLinkPair);
	}
	else if(typeIndexF>=0 && typeIndexB >0 && 4 == chatIndexValue)//解析招募信息
	{
		text = text.substr(typeIndexB+1);
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if(itemIDIndexF>0 && itemIDIndexB>0 /*== text.length()-1*/)
		{
			strInviteInfo = text.substr(itemIDIndexF+1,itemIDIndexB-itemIDIndexF-1);
			strTempInviteInfo = strInviteInfo;
			text = text.substr(0,itemIDIndexF);

			if(!strInviteInfo.empty())
			{
				indexFront = strTempInviteInfo.find_first_of("(");
				indexBack = strTempInviteInfo.find_first_of(")");
				if(indexFront>0 || indexBack >0)
				{
					invitetype = (unsigned char)atoi(strTempInviteInfo.substr(indexFront+1,indexBack-1).c_str());
				}
				strTempInviteInfo = strTempInviteInfo.substr(indexBack+1);
				indexFront = strTempInviteInfo.find_first_of("(");
				indexBack = strTempInviteInfo.find_first_of(")");
				if(indexFront>0 || indexBack >0)
				{
					inviteplayerid = atoi(strTempInviteInfo.substr(indexFront+1,indexBack-1).c_str());
				}

				strTempInviteInfo = strTempInviteInfo.substr(indexBack+1);
				indexFront = strTempInviteInfo.find_first_of("(");
				indexBack = strTempInviteInfo.find_first_of(")");
				if(indexFront>0 || indexBack >0)
				{
					fubenOrChengchiID = (unsigned short)atoi(strTempInviteInfo.substr(indexFront+1,indexBack-1).c_str());
				}
				// 				if(invitetype>0 && inviteplayerid>0)
				// 				{
				// 					playerData->inputParam(WCHDATA_JXJInviteType,&invitetype);
				// 					playerData->inputParam(WCHDATA_JXJInviteID,&inviteplayerid);
				// 				}
				if(invitetype<=0 || inviteplayerid<=0)
				{
					strInvitePair.first.type = 0;
					strInvitePair.first.id = 0;
					strInvitePair.first.playerid = 0;
					strInvitePair.second = text/*+strInviteInfo*/;
					m_strInviteVec.push_back(strInvitePair);
				}

				int indexFront = text.find_first_of("[");
				int indexBack = text.find_first_of("]");
				strFront = text.substr(0,indexFront+1);
				if(!strFront.empty())
				{
					strInvitePair.first.type = 0;
					strInvitePair.first.id = 0;
					strInvitePair.first.playerid = 0;
					strInvitePair.second = strFront;
					m_strInviteVec.push_back(strInvitePair);
				}
				strMid = text.substr(indexFront+1,indexBack-indexFront-1);
				if(!strMid.empty())
				{
					strInvitePair.first.type = invitetype;
					strInvitePair.first.playerid = inviteplayerid;
					strInvitePair.first.id = fubenOrChengchiID;
					strInvitePair.second = strMid;
					m_strInviteVec.push_back(strInvitePair);
				}
				text = text.substr(indexBack);
				strInvitePair.first.type = 0;
				strInvitePair.first.id = 0;
				strInvitePair.first.playerid = 0;
				strInvitePair.second = text;
				m_strInviteVec.push_back(strInvitePair);
			}
			else
			{
				strInvitePair.first.type = 0;
				strInvitePair.first.id = 0;
				strInvitePair.first.playerid = 0;
				strInvitePair.second = text/*+strInviteInfo*/;
				m_strInviteVec.push_back(strInvitePair);
			}
		}
		else
		{
			strInvitePair.first.type = 0;
			strInvitePair.first.id = 0;
			strInvitePair.first.playerid = 0;
			strInvitePair.second = text;
			m_strInviteVec.push_back(strInvitePair);
		}
	}
	else if (typeIndexF >= 0 && typeIndexB >0 && 5 == chatIndexValue)//解析详细战报
	{
		text = text.substr(typeIndexB + 1);
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if (itemIDIndexF > 0 && itemIDIndexB > 0 /*== text.length()-1*/)
		{
			std::string strReportId = text.substr(itemIDIndexF + 1, itemIDIndexB - itemIDIndexF - 1);
			text = text.substr(0, itemIDIndexF);
			//strTempInviteInfo = strInviteInfo;
			int idIndex = strReportId.find_first_of(",");
			std::string playerName = strReportId.substr(0, idIndex);
			int reportid = atoi(strReportId.substr(idIndex + 1).c_str());

			if (reportid > 0)
			{
				strBattleReport.first.first = playerName;
				strBattleReport.first.second = reportid;
				strBattleReport.second = text;
				m_strReportVec.push_back(strBattleReport);
			}
			else
			{
				strBattleReport.first.first.clear();
				strBattleReport.first.second = 0;
				strBattleReport.second = text;
				m_strReportVec.push_back(strBattleReport);
			}
		}
		else
		{
			strBattleReport.first.first.clear();
			strBattleReport.first.second = 0;
			strBattleReport.second = text;
			m_strReportVec.push_back(strBattleReport);
		}
	}
	else if (typeIndexF >= 0 && typeIndexB > 0 && 6 == chatIndexValue)//救援消息解析
	{
		text = text.substr(typeIndexB + 1);
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if (itemIDIndexF > 0 && itemIDIndexB > 0 /*== text.length()-1*/)
		{
			std::string strReportId = text.substr(itemIDIndexF + 1, itemIDIndexB - itemIDIndexF - 1);
			text = text.substr(0, itemIDIndexF);
			//strTempInviteInfo = strInviteInfo;
			int idIndex = strReportId.find_first_of("|");
			int roomid = atoi(strReportId.substr(0, idIndex).c_str());
			int sceneid = atoi(strReportId.substr(idIndex + 1).c_str());

			if (roomid != 0 && sceneid != 0)
			{
				strCallForHelpInfo.first.roomid = roomid;
				strCallForHelpInfo.first.secneid = sceneid;
				strCallForHelpInfo.second = text;
				m_strCallForHelpVec.push_back(strCallForHelpInfo);
			}
			else
			{
				strCallForHelpInfo.first.roomid = 0;
				strCallForHelpInfo.first.secneid = 0;
				strCallForHelpInfo.second = text;
				m_strCallForHelpVec.push_back(strCallForHelpInfo);
			}
		}
		else
		{
			strCallForHelpInfo.first.roomid = 0;
			strCallForHelpInfo.first.secneid = 0;
			strCallForHelpInfo.second = text;
			m_strCallForHelpVec.push_back(strCallForHelpInfo);
		}
	}
	else if (typeIndexF >= 0 && typeIndexB > 0 && 7 == chatIndexValue)//赞消息解析
	{
		text = text.substr(typeIndexB + 1);
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if (itemIDIndexF > 0 && itemIDIndexB > 0 /*== text.length()-1*/)
		{
			std::string strPlayerId = text.substr(itemIDIndexF + 1, itemIDIndexB - itemIDIndexF - 1);
			text = text.substr(0, itemIDIndexF);

			strpraiseReport.first = atoi(strPlayerId.c_str());
			strpraiseReport.second = text;
			m_strPraiseVec.push_back(strpraiseReport);
		}
	}
	else if (typeIndexF >= 0 && typeIndexB > 0 && 8 == chatIndexValue)//成就展示消息解析
	{
		text = text.substr(typeIndexB + 1);
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if(itemIDIndexF>0 && itemIDIndexB == text.length()-1)
		{
			strItemIds = text.substr(itemIDIndexF+1,itemIDIndexB-itemIDIndexF-1);
			text = text.substr(0,itemIDIndexF);
			while (!text.empty())
			{
				int indexFront = text.find_first_of("[");
				int indexBack = text.find_first_of("]");
				
				strMid = text.substr(indexFront,indexBack-indexFront+1);
				strPair.first.first = -1;
				strPair.first.second = -1;
				strPair.second = strMid;
				m_strAchieveVec.push_back(strPair);
				text = text.substr(indexBack+1);
			}

			int indexFront = -1;
			int indexBack = -1;
			int tempIndex = 0;
			int itemID = 0;
			int itemEquipMagic = 0;
			while (!strItemIds.empty())
			{
				indexFront = strItemIds.find_first_of("(");
				indexBack = strItemIds.find_first_of(")");
				if(indexFront<0 || indexBack <0) break;

				strItemIDPair = strItemIds.substr(indexFront+1,indexBack-1);
				tempIndex = strItemIDPair.find_first_of("|");
				if(tempIndex<=0) break;
				std::string strItemID = strItemIDPair.substr(0,tempIndex);
				std::string strEquipMagic = strItemIDPair.substr(tempIndex+1);
				itemID = atoi(strItemID.c_str());
				itemEquipMagic = atoi(strEquipMagic.c_str());
				itemIdVec.push_back(std::make_pair(itemID,itemEquipMagic));
				strItemIds = strItemIds.substr(indexBack+1);
			}

			if (m_strAchieveVec.size() == itemIdVec.size())
			{
				for (int i=0; i<m_strAchieveVec.size(); i++)
				{
					m_strAchieveVec[i].first = itemIdVec[i];
				}
			}
		}
		else
		{
			strAchieveInfo.first.first = 0;
			strAchieveInfo.first.second = 0;
			strAchieveInfo.second = text;
			m_strAchieveVec.push_back(strAchieveInfo);
		}
	}
	else
	{
		int itemIDIndexF = text.find_last_of("{");
		int itemIDIndexB = text.find_last_of("}");
		if(itemIDIndexF>0 && itemIDIndexB == text.length()-1)
		{
			strItemIds = text.substr(itemIDIndexF+1,itemIDIndexB-itemIDIndexF-1);
			text = text.substr(0,itemIDIndexF);
			while (!text.empty())
			{
				int indexFront = text.find_first_of("[");
				int indexBack = text.find_first_of("]");
				if(indexFront<0 || indexBack <0)
				{
					if(!text.empty())
					{
						strPairFront.first.first = 0;
						strPairFront.first.second = 0;
						strPairFront.second = text;
						m_strVec.push_back(strPairFront);
					}
					break;
				}
				strFront = text.substr(0,indexFront);
				if(!strFront.empty())
				{
					strPairFront.first.first = 0;
					strPairFront.first.second = 0;
					strPairFront.second = strFront;
					m_strVec.push_back(strPairFront);
				}
				strMid = text.substr(indexFront,indexBack-indexFront+1);
				strPair.first.first = -1;
				strPair.first.second = -1;
				strPair.second = strMid;
				m_strVec.push_back(strPair);
				text = text.substr(indexBack+1);
			}

			int indexFront = -1;
			int indexBack = -1;
			int tempIndex = 0;
			int itemID = 0;
			int itemEquipMagic = 0;
			while (!strItemIds.empty())
			{
				indexFront = strItemIds.find_first_of("(");
				indexBack = strItemIds.find_first_of(")");
				if(indexFront<0 || indexBack <0) break;

				strItemIDPair = strItemIds.substr(indexFront+1,indexBack-1);
				tempIndex = strItemIDPair.find_first_of("|");
				if(tempIndex<=0) break;
				std::string strItemID = strItemIDPair.substr(0,tempIndex);
				std::string strEquipMagic = strItemIDPair.substr(tempIndex+1);
				itemID = atoi(strItemID.c_str());
				itemEquipMagic = atoi(strEquipMagic.c_str());
				itemIdVec.push_back(std::make_pair(itemID,itemEquipMagic));
				strItemIds = strItemIds.substr(indexBack+1);
			}
			if(itemIdVec.empty())
			{
				strPairFront.first.first = 0;
				strPairFront.first.second = 0;
				strPairFront.second = text;
				m_strVec.push_back(strPairFront);
			}
			else
			{
				int index= 0;
				ChatItemInfoVec::iterator itr_str = m_strVec.begin();
				for (;itr_str!=m_strVec.end();itr_str++)
				{
					if(itr_str->first.first == -1)
					{
						if(index>=itemIdVec.size())
							break;
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemIdVec[index].first),itemrecord)>=0)
						{
							if(nameindex>0)
							{
								if(itr_str->second.compare(std::string("[")+itemrecord[nameindex]+std::string("]"))==0)
									itr_str->first = itemIdVec[index];
							}	
						}
						index++;
					}
				}
			}
		}
		else
		{
			strPairFront.first.first = 0;
			strPairFront.first.second = 0;
			strPairFront.second = text;
			m_strVec.push_back(strPairFront);
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewMailSwitchControlMethod
//
/////////////////////////////////////////
crJXJNewMailSwitchControlMethod::crJXJNewMailSwitchControlMethod()
{
}
crJXJNewMailSwitchControlMethod::crJXJNewMailSwitchControlMethod(const crJXJNewMailSwitchControlMethod& handle):
	crMethod(handle)
{}
void crJXJNewMailSwitchControlMethod::inputParam(int i, void *param)
{
}
void crJXJNewMailSwitchControlMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_mailplay = str;
		break;
	}
}
void crJXJNewMailSwitchControlMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crMultiSwitch>  mailplay = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_mailplay));
		if(mailplay.valid() && mailplay->getActiveSwitchSet()==1)
		{
			//void *param;
			//crData *brainData = crBrain::getInstance()->getDataClass();
			//brainData->excHandle(MAKEINT64(WCH_LockData,1));
			////brainData->getParam(WCHDATA_JXJRecvMailVec,param);
			////JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
			////if(mailvec)
			////	mailvec->clear();
			//brainData->getParam(WCHDATA_JXJRecvMailCacheMap,param);
			//MailCacheMap *mailcacheMap = (MailCacheMap *)param;
			//MailCacheMap::iterator itr = mailcacheMap->begin();
			//for (;itr!=mailcacheMap->end();itr++)
			//{
			//	if(itr->second->getState() == Mail_Recvd)
			//		break;
			//}
			//if(itr == mailcacheMap->end())
			//{
			//	mailplay->setActiveSwitchSet(0);
			//}
			//brainData->excHandle(MAKEINT64(WCH_LockData,0));
			mailplay->setActiveSwitchSet(0);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCampAddArmyMethod
//
/////////////////////////////////////////
crJXJCampAddArmyMethod::crJXJCampAddArmyMethod()
{
}
crJXJCampAddArmyMethod::crJXJCampAddArmyMethod(const crJXJCampAddArmyMethod& handle):
	crMethod(handle)
{}
void crJXJCampAddArmyMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJCampAddArmyMethod::addParam(int i, const std::string& str)
{

}
void crJXJCampAddArmyMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
	if(parentCanvas.valid())
	{
		crData *canvasData = parentCanvas->getDataClass();
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJBattleAddArmyInfoMap,param);
		BattleAddArmyInfoVec *addArmyInfoVec = (BattleAddArmyInfoVec *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyCase,param);
		char _case = *(char *)param;
		canvasData->getParam(WCHDATA_JXJAddArmyCampItem,param);
		crInstanceItem * touchCamp = (crInstanceItem *)param;
		if(_case>=0 && !addArmyInfoVec->empty())
		{
			crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if(myplayer)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(myplayer->getPlayerID());//
					crPlayerServerEventPacket packet;
					crPlayerServerEventPacket::buildRequestPacket(packet,WCH_RecvTouch,touchCamp,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}

			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedCopper,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedWood,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedIron,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedHorse,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedGold,NULL);
			canvasData->inputParam(WCHDATA_JXJBattleAddArmyCase,NULL);
			addArmyInfoVec->clear();
		}
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJCheckCampAddArmyEnableMethod
//
/////////////////////////////////////////
crJXJCheckCampAddArmyEnableMethod::crJXJCheckCampAddArmyEnableMethod():m_distance(10.0f)
{
}
crJXJCheckCampAddArmyEnableMethod::crJXJCheckCampAddArmyEnableMethod(const crJXJCheckCampAddArmyEnableMethod& handle):
	crMethod(handle),
	m_distance(handle.m_distance)
{}
void crJXJCheckCampAddArmyEnableMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem *)param;
		break;
	}
}
void crJXJCheckCampAddArmyEnableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_distance = atof(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJCheckCampAddArmyEnableMethod::operator()(crHandle &handle)
{
	void *param;
	crRole *role = NULL;
	//AddArmyInfoVec armyinfo;
	char _case = -1;//-1,补兵失败，0-》VIP0粮草充足并且兵力充足补兵成功，1-》VIP1兵力充足，2-》兵力不足，资源充足，直接扣除资源补兵；3-》资源不足，扣除元宝补兵
	crMyPlayerData *me = crMyPlayerData::getInstance();
	ref_ptr<crTableIO>equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
	ref_ptr<crTableIO>troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
	crPlayerGameData *myPlayer =me->getPlayerGameData();
	CRNetApp::crScene *scene = me->getScene();
	if(myPlayer && crRunGameHandle::getInstance()->isInGame() && scene && scene->getExternSceneType() == Ext_Battle)
	{
		crData *myPlayerData = myPlayer->getDataClass();
		if(myPlayerData)
		{
			//myPlayerData->getParam(WCHDATA_JXJVipLv,param);
			//unsigned char vipLevel = *(unsigned char *)param;
			//int addArmyNeedExtraFood = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleAddArmyExtraFoodNeed, vipLevel).c_str()));
			//short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime, vipLevel).c_str()));
			//int credittime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime, vipLevel).c_str()));
			////float validDistance = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCampAddArmyDistance, vipLevel).c_str());
			crRole *role = NULL;
			
			char isEnemy = 0;
			me->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKEINT64(m_this,&isEnemy));
			//////////////////////////////////////////////////////////////////////////
			ref_ptr<crCanvasNode>addArmyCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);

			if((isEnemy==1) && addArmyCanvas.valid())
			{
				//crHypertextWidgetNode *hyperText = dynamic_cast<crHypertextWidgetNode *>(addArmyCanvas->getWidget(m_strHyperText));
				crData *canvasData = addArmyCanvas->getDataClass();
				crPlayerGameData *myPlayer = me->getPlayerGameData();
				if( myPlayer && canvasData)
				{
					canvasData->excHandle(MAKEINT64(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJBattleAddArmyInfoMap,param);
					BattleAddArmyInfoVec *addArmyInfoVec = (BattleAddArmyInfoVec *)param;
					addArmyInfoVec->clear();
					crData * myPlayerData = myPlayer->getDataClass();
					ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
					if(myPlayerData && camera.valid())
					{
						crData *cameraData =  camera->getAttachedNode()->getDataClass();
						cameraData->getParam(WCHDATA_JXJCampAddArmyTimer,param);
						float campAddArmytimer = *(float *)param;
						//if(campAddArmytimer>0.0f)
						//{
						//	_case = -4;//据点补兵CD中
						//}
						//else
						{
							myPlayerData->excHandle(MAKEINT64(WCH_LockData,1));

							TroopsMap recruitTroops;
							crVector3 itempos = m_this->getPosition();
							crVector3 mypos;
							/*unsigned short*/int rtCount,needCount;
							crData *roledata;
							bool isalldead = true;
							for(int i = 0;i<3;i++)
							{
								role = me->getLoadedRoleByIndex(i);
								if(!role)
									continue;
								roledata = role->getDataClass();
								int abstractid = role->getAbstractItemID();
								int roleid = role->getRoleID();
								mypos = role->getPosition();

								if(roledata)
								{
									roledata->getParam(WCHDATA_JXJArmyCount,param);
									/*unsigned short*/int count = *(/*unsigned short*/int *)param;
									roledata->getParam(WCHDATA_JXJRTArmyCount,param);
									rtCount = *(/*unsigned short*/int *)param;
									if(rtCount>0)
										isalldead = false;
									if(rtCount>=count) continue;

									if (rtCount == 0 || (rtCount>0 && (mypos - itempos).length() < m_distance))
									{
										//needCount = count - rtCount;
										//TroopsMap::iterator itr_troops = tempTroopsMap.find(abstractid);
										//if (itr_troops != tempTroopsMap.end())
										//{
										//	troopRTcount = itr_troops->second;
										//}
										//if (troopRTcount>needCount)
										//{
										//	itr_troops->second -= needCount;
										//}
										//if (needCount > troopRTcount)
										//{
										//	if(recruitTroops.find(abstractid) == recruitTroops.end())
										//		recruitTroops[abstractid] = needCount - troopRTcount;
										//	else
										//	{
										//		recruitTroops[abstractid] += needCount - troopRTcount;
										//	}
										//	//armyinfo.push_back(make_pair(abstractid, needCount - troopRTcount));
										//}
										//addArmyInfoVec->push_back(make_pair(abstractid, make_pair(roleid, needCount)));
										needCount = count-rtCount;
										addArmyInfoVec->push_back(std::make_pair(abstractid, std::make_pair(roleid, needCount)));
										if(recruitTroops.find(abstractid)!=recruitTroops.end())
										{
											recruitTroops[abstractid]+=needCount;
										}
										else
										{
											recruitTroops[abstractid]=needCount;
										}
									}
								}			
							}
							if(addArmyInfoVec->empty())
							{
								_case = -5;//全死光，或者兵力满员,不需要补兵
								myPlayerData->excHandle(MAKEINT64(WCH_LockData,0));
							}
							else if(!isalldead && campAddArmytimer>0.0f)
							{
								_case = -4;//据点补兵CD中
								myPlayerData->excHandle(MAKEINT64(WCH_LockData,0));
							}
							else
							{
								myPlayerData->getParam(WCHDATA_JXJTroopsMap,param);
								TroopsMap *troops = (TroopsMap *)param;
								TroopsMap::iterator itr_troops;
								TroopsMap recruitTroops_copy = recruitTroops;
								for( TroopsMap::iterator itr = recruitTroops.begin();
									itr != recruitTroops.end();
									)
								{
									itr_troops = troops->find(itr->first);
									if (itr_troops != troops->end())
									{
										if(itr_troops->second>=itr->second)
										{
											//itr_troops->second-=itr->second;
											itr = recruitTroops.erase(itr);
											continue;
										}
										else
										{
											itr->second-=itr_troops->second;
											//itr_troops->second = 0;
										}
									}
									++itr;
								}
								myPlayerData->excHandle(MAKEINT64(WCH_LockData,0));

								BattleRecruitCheckResult recruitCheck;
								myPlayer->doEvent(WCH_JXJBattleRecruitCheck,MAKEINT64(&recruitTroops,&recruitCheck));

								_case = recruitCheck.m_code;
								if(_case == 0)
								{
									int addArmyNeedExtraFood = recruitCheck.m_foodneed;
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&addArmyNeedExtraFood);
								}
								else
								{
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedGold,&recruitCheck.m_goldneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedCopper,&recruitCheck.m_copperneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&recruitCheck.m_foodneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedWood,&recruitCheck.m_woodneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedIron,&recruitCheck.m_ironneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedHorse,&recruitCheck.m_horseneed);
								}
							}
						}
					}
					canvasData->inputParam(WCHDATA_JXJBattleAddArmyCase,&_case);
					canvasData->inputParam(WCHDATA_JXJAddArmyCampItem,m_this);
					canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				//显示提示窗口
				if(_case != -5 && _case != -4)
				{
					crFilterRenderManager::getInstance()->showCanvas(addArmyCanvas.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJChangeCameraFollowStateMethod
//
/////////////////////////////////////////
crJXJChangeCameraFollowStateMethod::crJXJChangeCameraFollowStateMethod():m_needChange(false)
{
}
crJXJChangeCameraFollowStateMethod::crJXJChangeCameraFollowStateMethod(const crJXJChangeCameraFollowStateMethod& handle):
	crMethod(handle),
	m_needChange(false)
{}
void crJXJChangeCameraFollowStateMethod::inputParam(int i, void *param)
{
// 	switch(i) 
// 	{
// 	case 0:
// 		if(param == 0)
// 		{//释放
// 			m_this = NULL;
// 		}
// 		break;
// 	case 1:
// 		m_this = (crMatrixTransform*)param;
// 		break;
// 	}
}
void crJXJChangeCameraFollowStateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_needChange = atoi(str.c_str());
		break;
	case 1:
		m_strCanvas = str;
		break;
	case 2:
		m_strCheckBox = str;
		break;
	case 3:
		m_newCanvas = str;
		break;
	case 4:
		m_newCanvas_sw = str;
		break;
	default:
		break;
	}
}
void crJXJChangeCameraFollowStateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(camera->getAttachedNode());
		crData *cameraData = cameraNode->getDataClass();
		char filePath[MAX_PATH];
		std::string strPath = crGlobalHandle::getInstance()->getGameDir();
		sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
		bool cameraFollow = ::GetPrivateProfileInt("mouse","Lockable",0,filePath);
		cameraFollow = !cameraFollow;
		std::string isCheck = crArgumentParser::appItoa(cameraFollow);
		::WritePrivateProfileString("mouse","Lockable",isCheck.c_str(),filePath);
		cameraData->inputParam(WCHDATA_JXJCameraFollow,&cameraFollow);
		if(m_needChange)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if(canvas.valid())
			{
				ref_ptr<crCheckBoxWidgetNode>checkBox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_strCheckBox));
				if(checkBox.valid())
				{
					checkBox->select(cameraFollow);
				}
			}
		}
		ref_ptr<crCanvasNode> newcanvas = crFilterRenderManager::getInstance()->findCanvas(m_newCanvas);
		if (newcanvas.valid() && cameraFollow)
		{
			ref_ptr<crMultiSwitch> newcanvas_sw = dynamic_cast<crMultiSwitch *>(newcanvas->getChildNode(m_newCanvas_sw));
			if (newcanvas_sw.valid())
			{
				newcanvas_sw->setActiveSwitchSet(0);
			}
			crFilterRenderManager::getInstance()->showCanvas(newcanvas.get(), true);
			ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
			if(camera.valid())
			{
				void *param;
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				if(myPlayer)
				{
					crData *myPlayerData = myPlayer->getDataClass();
					if(myPlayerData)
					{
						myPlayerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						crData *camadata = camera->getAttachedNode()->getDataClass();
						short timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJDelayTimer, vipLevel).c_str()); // 按键提示窗口显示3秒之后自动关闭
						camadata->inputParam(WCHDATA_JXJDelayCanvasCloseTime,&timer);
					}
				}
			}
		}
		std::string str;
		int textid;
		ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		crTableIO::StrVec record;
		if(cameraFollow) textid = 3014;
		else textid = 3015;
		if(textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),record)>=0)
		{
			str = record[1];
		}
		ref_ptr<crCanvasNode >tipsCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Prompt_Notice");
		if (tipsCanvas.valid())
		{
			tipsCanvas->setCanFocus(false);
			float time = 3.0f;
			crData *canvasdata = tipsCanvas->getDataClass();
			if(canvasdata)canvasdata->inputParam(WCHDATA_JXJVisiableTime,&time);
			ref_ptr<crStaticTextWidgetNode>input = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget("UI_Prompt"));
			if(input.valid())input->setString(str);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJGetCameraFollowStateMethod
//
/////////////////////////////////////////
crJXJGetCameraFollowStateMethod::crJXJGetCameraFollowStateMethod()
{
}
crJXJGetCameraFollowStateMethod::crJXJGetCameraFollowStateMethod(const crJXJGetCameraFollowStateMethod& handle):
	crMethod(handle)
{}
void crJXJGetCameraFollowStateMethod::inputParam(int i, void *param)
{
}
void crJXJGetCameraFollowStateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strCheckBox = str;
		break;
	default:
		break;
	}
}
void crJXJGetCameraFollowStateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(camera->getAttachedNode());
		crData *cameraData = cameraNode->getDataClass();

		char filePath[MAX_PATH];
		std::string strPath = crGlobalHandle::getInstance()->getGameDir();
		sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
		bool cameraFollow = ::GetPrivateProfileInt("mouse","Lockable",0,filePath);

		cameraData->inputParam(WCHDATA_JXJCameraFollow,&cameraFollow);
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if(canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_strCheckBox));
			if(checkBox.valid())
			{
				checkBox->select(cameraFollow);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGetCameraSpeedConfigMethod
//
/////////////////////////////////////////
crJXJGetCameraSpeedConfigMethod::crJXJGetCameraSpeedConfigMethod()
{
}
crJXJGetCameraSpeedConfigMethod::crJXJGetCameraSpeedConfigMethod(const crJXJGetCameraSpeedConfigMethod& handle):
	crMethod(handle)
{}
void crJXJGetCameraSpeedConfigMethod::inputParam(int i, void *param)
{
}
void crJXJGetCameraSpeedConfigMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJGetCameraSpeedConfigMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> cam = crCameraManager::getInstance()->getMainCamera();
	if(cam.valid())
	{
		crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(cam->getAttachedNode());
		if(cameraNode)
		{
			crData *cameraData = cameraNode->getDataClass();
			if(cameraData)
			{
				char speedValue[16];
				char filePath[MAX_PATH];
				std::string strPath = crGlobalHandle::getInstance()->getGameDir();
				sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
				::GetPrivateProfileString("mouse","MoveSpeed","40",speedValue,16,filePath);
				float cameraSpeed = atof(speedValue);
				cameraData->inputParam(WCHDATA_JXJCameraMoveSpeed,&cameraSpeed);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCampAddArmyCanvasOnshowMethod
//
/////////////////////////////////////////
crJXJCampAddArmyCanvasOnshowMethod::crJXJCampAddArmyCanvasOnshowMethod()
{
}
crJXJCampAddArmyCanvasOnshowMethod::crJXJCampAddArmyCanvasOnshowMethod(const crJXJCampAddArmyCanvasOnshowMethod& handle):
	crMethod(handle)
{}
void crJXJCampAddArmyCanvasOnshowMethod::inputParam(int i, void *param)
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
void crJXJCampAddArmyCanvasOnshowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strtext = str;
		break;
	default:
		break;
	}
}
void crJXJCampAddArmyCanvasOnshowMethod::operator()(crHandle &handle)
{
	ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strtext));
	if(!hyperText) return;
	std::string strinfo;
	void *param;
	crTableIO::StrVec record;
	ref_ptr<crTableIO>troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
	crData *canvasData = m_this->getDataClass();
	if(canvasData && troopTab.valid())
	{
		int troopnameindex = troopTab->getTitleIndex("name");
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJBattleAddArmyInfoMap,param);
		BattleAddArmyInfoVec *addArmyInfoVec = (BattleAddArmyInfoVec *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyCase,param);
		char _case = *(char *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedCopper,param);
		int needCopper = *(int *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedFood,param);
		int needFood = *(int *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedWood,param);
		int needWood = *(int *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedIron,param);
		int needIron = *(int *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedHorse,param);
		int needHorse = *(int *)param;
		canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedGold,param);
		int needGold = *(int *)param;

		//生成超文本
		switch (_case)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			strinfo = std::string("你确定要补充：");
			break;
		case -1:
			strinfo = std::string("兵力不足，补兵失败！点击确定后退出");
			break;
		case -2:
			strinfo = std::string("资源不足，补兵失败！点击确定后退出");
			break;
		case -3:
			strinfo = std::string("礼金/元宝不足，补兵失败！点击确定后退出");
			break;
		default:
			strinfo = std::string("补兵失败！点击确定后退出");
			break;
		}
		std::vector<float>v_i;
		ItemMap itemMap;
		int index = 1;
		rcfg::ConfigScript cfg_script(&itemMap);
		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content",index++);
		cfg_script.Add("Text",strinfo);
		cfg_script.Pop();

		if(_case>=0)
		{
			for (BattleAddArmyInfoVec::iterator itr_info =addArmyInfoVec->begin();
				itr_info!=addArmyInfoVec->end();itr_info++)
			{
				if(troopTab->queryOneRecord(0,crArgumentParser::appItoa(itr_info->first),record)>=0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",record[troopnameindex]);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(itr_info->second.second));
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",",");
					cfg_script.Pop();
				}
			}

			if(_case!=1)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Add("Text","花费:");
				cfg_script.Pop();

				if(needGold>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","礼金/元宝:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needGold));
					cfg_script.Pop();
				}

				if(needCopper>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","铜钱:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needCopper));
					cfg_script.Pop();
				}

				if(needFood>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","粮草:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needFood));
					cfg_script.Pop();
				}

				if(needWood>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","木材:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needWood));
					cfg_script.Pop();
				}

				if(needIron>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","铁矿:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needIron));
					cfg_script.Pop();
				}

				if(needHorse>0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text","马匹:");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",index++);
					cfg_script.Add("Text",crArgumentParser::appItoa(needHorse));
					cfg_script.Pop();
				}
			}
		}

		cfg_script.Pop();

		hyperText->setHypertext(cfg_script);
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJPlayerCampAddArmyTimerUpdateMethod
//
/////////////////////////////////////////
crJXJPlayerCampAddArmyTimerUpdateMethod::crJXJPlayerCampAddArmyTimerUpdateMethod():
	m_dt(NULL)
{
}
crJXJPlayerCampAddArmyTimerUpdateMethod::crJXJPlayerCampAddArmyTimerUpdateMethod(const crJXJPlayerCampAddArmyTimerUpdateMethod& handle):
	crMethod(handle),
	m_dt(NULL)
{}
void crJXJPlayerCampAddArmyTimerUpdateMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJPlayerCampAddArmyTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strInput = str;
		break;
	default:
		break;
	}
}
void crJXJPlayerCampAddArmyTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *cameraData = m_this->getDataClass();
	if(cameraData)
	{
		int _timer = 0;
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJCampAddArmyTimer,param);
			float * timer = (float *)param;
			if(*timer>0.0f)
				*timer -=*m_dt;
			_timer = *timer;
		}
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if(canvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> timeText = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_strInput));
			if(timeText.valid())
			{
				if(_timer>0)
				{
					int minute = (_timer)/60;
					int second = (_timer)%60;
					char text[16];
					sprintf(text,"%02d:%02d\0",minute,second);
					timeText->setString(std::string(text));
				}
				else
				{
					timeText->setString(std::string("可补兵"));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvTouchCampMethod
//
/////////////////////////////////////////
crJXJRecvTouchCampMethod::crJXJRecvTouchCampMethod():
	m_range(10.0f){}
crJXJRecvTouchCampMethod::crJXJRecvTouchCampMethod(const crJXJRecvTouchCampMethod& handle):
	crMethod(handle),
	m_range(handle.m_range)
{
}
void crJXJRecvTouchCampMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvTouchCampMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_range = atoi(str.c_str());
		break;
	}
}

void crJXJRecvTouchCampMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		//crData *itemData = m_this->getDataClass();
		int playerid = m_stream->_readInt();
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerSceneServerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));		
		if(playerSceneServerData.valid())
		{
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			CRNetApp::crScene *scene = callback->findScene(playerSceneServerData->getSceneID());
			if(!scene || scene->getExternSceneType() != Ext_Battle)
				return;
			void *param;
			crRoom *room = callback->findRoom(playerSceneServerData->getRoomID());
			bool canAddArmy = false;
			if(room)
			{
				crData *roomData = room->getDataClass();
				if(roomData)
				{
					int addArmyInterval = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCampAddArmyInterval, 0).c_str());
					roomData->excHandle(MAKEINT64(WCH_LockData,1));
					roomData->getParam(WCHDATA_JXJCampAddArmyTimerMap,param);
					CampAddArmyTimerMap *timerMap = (CampAddArmyTimerMap *)param;
					CampAddArmyTimerMap::iterator itr_timer = timerMap->find(playerid);

					if(itr_timer!= timerMap->end())
					{
						time_t t = time(0);
						if(t-itr_timer->second > addArmyInterval)
						{
							canAddArmy = true;
						}
					}
					else
					{
						canAddArmy = true;
					}
					roomData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			if(canAddArmy)
			{
				crVector3 camppos = m_this->getPosition();
				crSceneServerPlayerData::RoleMap & roleMap = playerSceneServerData->getRoleMap();
				std::vector< std::pair< int,std::pair< int,/*unsigned short*/int > > > addArmyInfoVec; //abstractid,roleid,count ;
				//TroopsMap recruitTroopsMap;
				crData * roleData = NULL;
				/*unsigned short*/int rtCount = 0;
				/*unsigned short*/int count = 0;
				//int abstractid;
				crSceneServerPlayerData::RoleMap::iterator itr_role = roleMap.begin();
				for (;itr_role!=roleMap.end();itr_role++)
				{
					roleData = itr_role->second->getDataClass();
					roleData->getParam(WCHDATA_JXJArmyCount,param);
					count = *(/*unsigned short*/int *)param;
					roleData->getParam(WCHDATA_JXJRTArmyCount,param);
					rtCount = *(/*unsigned short*/int *)param;
					if(rtCount>=count) continue;

					crVector3f mypos = itr_role->second->getPosition();

					if (rtCount == 0 || (rtCount>0 && (mypos - camppos).length() < m_range))
					{
						//abstractid = itr_role->second->getAbstractItemID();
						//if(recruitTroopsMap.find(abstractid)!=recruitTroopsMap.end())
						//{
						//	recruitTroopsMap[abstractid]=count-rtCount;
						//}
						//else
						//{
						//	recruitTroopsMap[abstractid]+=count-rtCount;
						//}
						addArmyInfoVec.push_back(std::make_pair(itr_role->second->getAbstractItemID(),std::make_pair(itr_role->second->getRoleID(),count-rtCount)));
					}
				}
				if(!addArmyInfoVec.empty())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(13+addArmyInfoVec.size()*10);
					stream->_writeUChar(addArmyInfoVec.size());
					//std::vector< std::pair< int,std::pair< int,unsigned short > > >::iterator itr_info = addArmyInfoVec.begin();
					for( std::vector< std::pair< int,std::pair< int,/*unsigned short*/int > > >::iterator itr = addArmyInfoVec.begin();
						 itr!=addArmyInfoVec.end();
						 itr++)
					{
						stream->_writeInt(itr->first);
						stream->_writeInt(itr->second.first);
						stream->_writeInt(itr->second.second);
					}
					stream->_writeVec3(camppos);//12
					crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
					crJXJCampAddArmyInfoPacket packet;
					crJXJCampAddArmyInfoPacket::buildRequestPacket2(packet,playerSceneServerData->getPlayerID(),stream.get());
					gameServerConductor->getNetManager()->sendPacket("all",packet);
				}			
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJBattleInfoCenterNoticeCreateMethod
//
/////////////////////////////////////////
crJXJBattleInfoCenterNoticeCreateMethod::crJXJBattleInfoCenterNoticeCreateMethod():m_battleid(0),m_iswin(false){}
crJXJBattleInfoCenterNoticeCreateMethod::crJXJBattleInfoCenterNoticeCreateMethod(const crJXJBattleInfoCenterNoticeCreateMethod& handle):
	crMethod(handle),
	m_battleid(handle.m_battleid),
	m_iswin(handle.m_iswin)
{
}
void crJXJBattleInfoCenterNoticeCreateMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_battleid = LOINT64(param64);
			m_iswin = *(bool *)(HIINT64(param64));
		}
		else
		{
			m_battleid = 0;
			m_iswin = false;
		}
		break;
	}
}

void crJXJBattleInfoCenterNoticeCreateMethod::addParam(int i, const std::string& str)
{
}

void crJXJBattleInfoCenterNoticeCreateMethod::operator()(crHandle &handle)
{
	if(m_this!=NULL && m_battleid!=0)
	{
		crData *gsbraindata = m_this->getDataClass();
		if(gsbraindata)
		{
			std::string strAttack,strDefend,strWin,strLose,strChengchiName,strInfo;
			crTableIO::StrVec record;
			void *param;
			unsigned char ashili = (unsigned char)(LOINT16(LOINT32(m_battleid)));
			unsigned char dshili = (unsigned char)(HIINT16(LOINT32(m_battleid)));
			unsigned short chengchiid = HIINT32(m_battleid);
			ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			if(chengchiTab.valid() && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0 && chengchiTab->getTitleIndex("名字")>0 )
			{
				strChengchiName = record[chengchiTab->getTitleIndex("名字")];
			}
			gsbraindata->getParam(WCHDATA_JXJShiliWei,param);
			crJXJShili *shiliWei = (crJXJShili *)param;
			gsbraindata->getParam(WCHDATA_JXJShiliShu,param);
			crJXJShili *shiliShu = (crJXJShili *)param;
			gsbraindata->getParam(WCHDATA_JXJShiliWu,param);
			crJXJShili *shiliWu = (crJXJShili *)param;
			int weiID = shiliWei->getID();
			int shuID = shiliShu->getID();
			int wuID = shiliWu->getID();

			if(ashili == weiID)
			{
				strAttack = "魏";
			}
			else if(ashili == shuID)
			{
				strAttack = "蜀";
			}
			else if(ashili == wuID)
			{
				strAttack = "吴";
			}
			else
			{
				strAttack = "群雄";
			}
			if(dshili == weiID)
			{
				strDefend = "魏";
			}
			else if(dshili == shuID)
			{
				strDefend = "蜀";
			}
			else if(dshili == wuID)
			{
				strDefend = "吴";
			}
			else
			{
				strDefend = "群雄";
			}

			if(m_iswin)
			{
				strWin = strAttack;
				strLose = strDefend;
				strInfo = std::string("攻下此城!");
			}
			else
			{
				strWin = strDefend;
				strLose = strAttack;
				strInfo = std::string("守住此城!");
			}

			std::vector<float>v_i;
			ItemMap itemMap;
			int index = 1;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			cfg_script.Push("Character",16);
			cfg_script.Add("Text","恭喜");
			cfg_script.Pop();
			
			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			if(strWin.compare("群雄")==0)
			{
				cfg_script.Add("Text",std::string("[")+strWin+std::string("]"));
			}
			else
			{
				cfg_script.Add("Text",std::string("[")+strWin+std::string("]国"));
			}
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			cfg_script.Add("Text",std::string("在[")+strChengchiName+std::string("]之战中"));
			cfg_script.Pop();

			
			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			cfg_script.Add("Text","战胜");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			if(strLose.compare("群雄")==0)
			{
				cfg_script.Add("Text",std::string("[")+strLose+std::string("]"));
			}
			else
			{
				cfg_script.Add("Text",std::string("[")+strLose+std::string("]国"));
			}
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			cfg_script.Add("Text",strInfo);
			cfg_script.Pop();

			cfg_script.Pop();

			ref_ptr<crStreamBuf> streamBuffer = new crStreamBuf;
			crGlobalHandle::getInstance()->getScriptStream(cfg_script,streamBuffer);

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(streamBuffer->getRemainSize());
			stream->write(streamBuffer->getPtr(),streamBuffer->getRemainSize());

			crPlayerDataEventPacket packetpk;
			crPlayerDataEventPacket::buildReplyPacket(packetpk, 0, WCH_JXJRecvTempClientNotice, stream.get());
			stream->seekBegin();
			//gsbraindata->inputParam(WCHDATA_JXJCenterNotice,stream.get());
			crStreamBuf *pkgstream = packetpk.getStreamBuf();
			int size = pkgstream->getBufSize();
			ref_ptr<crGameServerPlayerData> recvPlayerData;
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crNetManager *netManager = gameServer->getNetManager();
			if(netDataManager && netManager)
			{
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();
					++itr )
				{
					recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if (recvPlayerData.valid())
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(recvPlayerData->getPlayerID());
						pkgstream->setBufSize(size);
						gameServer->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packetpk);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISelectPersuadeFubenMethod
//
/////////////////////////////////////////
crJXJUISelectPersuadeFubenMethod::crJXJUISelectPersuadeFubenMethod():
	m_this(NULL){}
crJXJUISelectPersuadeFubenMethod::crJXJUISelectPersuadeFubenMethod(const crJXJUISelectPersuadeFubenMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_strCanvas(handle.m_strCanvas)
{
}
void crJXJUISelectPersuadeFubenMethod::inputParam(int i, void *param)
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

void crJXJUISelectPersuadeFubenMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJUISelectPersuadeFubenMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(!canvas.valid())return;
	ref_ptr<crCanvasNode>persuadeCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);

	crData *canvasdata = canvas->getDataClass();
	void *param;
	canvasdata->getParam(WCHDATA_JXJSelFubenID,param);
	unsigned short fubenid = *(unsigned short *)param;	

	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
		PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;

		if(persuadeCanvas.valid() && persuadeInfoDeq->size()<3)
		{
			crData * persuadeCanvasData = persuadeCanvas->getDataClass();
			if(persuadeCanvasData)
			{
				persuadeCanvasData->inputParam(WCHDATA_JXJPersuadeFubenID,&fubenid);
			}
			crFilterRenderManager::getInstance()->showCanvas(persuadeCanvas.get(),true);
		}

		if(persuadeInfoDeq->size()>=3)
		{
			//提示说服已到最大数量限制
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
//////////////////////////////////////////////////////////////////////////
//将卡拖动至说服界面
////////////////////////////////////////////////////////////////////////// 
//crJXJFubenPersuadeCanvasShowMethod::crJXJFubenPersuadeCanvasShowMethod(){}
//crJXJFubenPersuadeCanvasShowMethod::crJXJFubenPersuadeCanvasShowMethod(const crJXJFubenPersuadeCanvasShowMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJFubenPersuadeCanvasShowMethod::inputParam(int i, void *param)
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
//void crJXJFubenPersuadeCanvasShowMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_strDesText = str;
//		break;
//	case 1:
//		m_strPersuadeSw_level = str;
//		break;
//	case 2:
//		m_strPersuadeSw_jk = str;
//		break;
//	case 3:
//		m_strPersuadeSw_Time = str;
//		break;
//	case 4:
//		m_strJkImageBox = str;
//		break;
//	case 5:
//		m_strBt = str;
//		break;
//	case 6:
//		m_strImgaeBk = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJFubenPersuadeCanvasShowMethod::operator()(crHandle &handle)
//{
//	void *param;
//	crTableIO::StrVec recordVec;
//	crData *data = m_this->getDataClass();
//	ref_ptr<crStaticTextWidgetNode>textNode = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strDesText));
//	ref_ptr<crMultiSwitch>timeMulitSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strPersuadeSw_Time));
//	ref_ptr<crMultiSwitch>jkMulitSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strPersuadeSw_jk));
//	ref_ptr<crMultiSwitch>lvMulitSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strPersuadeSw_level));
//	ref_ptr<crButtonWidgetNode>btImageCard = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strJkImageBox));
//	ref_ptr<crButtonWidgetNode>btConfirm = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strBt));
//	//初始化状态
//	if(btConfirm.valid()) btConfirm->setEnable(true);
//	//if(btImageCard) btImageCard->setEnable(false);
//	if(timeMulitSwitch.valid()) timeMulitSwitch->setActiveSwitchSet(0);
//	if(jkMulitSwitch.valid()) jkMulitSwitch->setActiveSwitchSet(5);
//	if(lvMulitSwitch.valid()) lvMulitSwitch->setActiveSwitchSet(0);
//
//	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
//	if(data && playerData)
//	{
//		data->getParam(WCHDATA_JXJPersuadeFubenID,param);
//		unsigned short fubenid = *(unsigned short *)param;
//
//		ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
//		ref_ptr<crTableIO>itemTab = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//		int qindex = itemTab->getTitleIndex("品质");
//		int iconIndex = itemTab->getTitleIndex("icon");
//		if(fubentab.valid())
//		{
//			int desIndex = fubentab->getTitleIndex("说服描述");
//			if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),recordVec)>=0 && desIndex>=0)
//			{
//				if(textNode.valid())
//				{
//					textNode->setString(recordVec[desIndex]);
//				}
//				
//				playerData->excHandle(MAKEINT64(WCH_LockData,1));
//				playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
//				PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;
//				PersuadeInfoDeq::iterator itr_info = persuadeInfoDeq->begin();
//				for (;itr_info!=persuadeInfoDeq->end();)
//				{
//					if(itr_info->get()->getFubenId()==fubenid)
//					{
//						btConfirm->setEnable(false);
//
//						if(timeMulitSwitch.valid())
//							timeMulitSwitch->setActiveSwitchSet(1);
//						if(lvMulitSwitch.valid())
//							lvMulitSwitch->setActiveSwitchSet(itr_info->get()->getLevel());
//
//						if(itemTab->queryOneRecord(0, crArgumentParser::appItoa(itr_info->get()->getJKId()), recordVec)>=0)
//						{
//							if(jkMulitSwitch.valid())
//							{
//								//if(atoi(recordVec[qindex].c_str())<=CardColor_Orange)
//									jkMulitSwitch->setActiveSwitchSet(atoi(recordVec[qindex].c_str()));
//								//else if(atoi(recordVec[qindex].c_str())>CardColor_Orange)
//								//	jkMulitSwitch->setActiveSwitchSet(CardColor_Orange);
//							}
//								
//							if(btImageCard.valid())
//							{
//								CRCore::crStateSet *state_set = dynamic_cast<CRCore::crObject *>(( dynamic_cast<CRCore::crGroup *>(btImageCard->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
//								CRCore::crTexture2D *texture_2d = 	dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
//								texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(recordVec[iconIndex],texture_2d));
//
//								//btImageCard->setEnable(false);
//								
//							}
//						}
//
//						break;
//					}
//					itr_info++;
//				}
//
//				if(itr_info == persuadeInfoDeq->end()) 
//				{
//					btImageCard->setEnable(true);
//					CRCore::crStateSet *state_set = dynamic_cast<CRCore::crObject *>(( dynamic_cast<CRCore::crGroup *>(btImageCard->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
//					CRCore::crTexture2D *texture_2d = 	dynamic_cast<CRCore::crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
//					texture_2d->setImage(0,texture_2d->getImageDataRequestHandler()->requestImageFile(m_strImgaeBk,texture_2d));
//				}
//				playerData->excHandle(MAKEINT64(WCH_LockData,0));
//			}
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJShowJKPacketCanvasMethod
//
/////////////////////////////////////////
crJXJShowJKPacketCanvasMethod::crJXJShowJKPacketCanvasMethod():
	m_this(NULL){}
crJXJShowJKPacketCanvasMethod::crJXJShowJKPacketCanvasMethod(const crJXJShowJKPacketCanvasMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_strCanvas(handle.m_strCanvas)
{
}
void crJXJShowJKPacketCanvasMethod::inputParam(int i, void *param)
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

void crJXJShowJKPacketCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJShowJKPacketCanvasMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>JKCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	if(!canvas.valid())return;
	crData *data = canvas->getDataClass();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	bool bShow = true;
	if(data && playerData)
	{
		data->getParam(WCHDATA_JXJPersuadeFubenID,param);
		unsigned short fubenid = *(unsigned short *)param;
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
		PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;
		PersuadeInfoDeq::iterator itr_info = persuadeInfoDeq->begin();
		for (;itr_info!=persuadeInfoDeq->end();itr_info++)
		{
			if(itr_info->get()->getFubenId()==fubenid)
			{
				bShow = false;
				break;
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	
	if(canvas.valid() && JKCanvas.valid() && bShow)
	{
		const crBoundingBox & bbox = canvas->getBoundBox();
		const crBoundingBox & jkbox = JKCanvas->getBoundBox();

		JKCanvas->setMatrix(crMatrix::translate(crVector3(bbox.xMax()+jkbox.xLength()*0.5f,bbox.center().y(),0.0f)));

		crFilterRenderManager::getInstance()->showCanvas(JKCanvas.get(),true);
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			canvasData->inputParam(WCHDATA_JXJPersuadeNeedCanvas,JKCanvas.get());
		}
	}
}

/////////////////////////////////////////
//
//crJXJClosePersuadeCanvasMethod
//
/////////////////////////////////////////
crJXJClosePersuadeCanvasMethod::crJXJClosePersuadeCanvasMethod():
	m_this(NULL){}
crJXJClosePersuadeCanvasMethod::crJXJClosePersuadeCanvasMethod(const crJXJClosePersuadeCanvasMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJClosePersuadeCanvasMethod::inputParam(int i, void *param)
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

void crJXJClosePersuadeCanvasMethod::addParam(int i, const std::string& str)
{
}

void crJXJClosePersuadeCanvasMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(canvas.valid())
	{
		crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			canvasData->getParam(WCHDATA_JXJPersuadeNeedCanvas,param);
			ref_ptr<crCanvasNode>JKCanvas = (crCanvasNode *)param;
			if(JKCanvas.valid())
			{
				crFilterRenderManager::getInstance()->closeCanvas(JKCanvas.get());
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJDoFubenPersuadeMethod
//
/////////////////////////////////////////
crJXJDoFubenPersuadeMethod::crJXJDoFubenPersuadeMethod(){}
crJXJDoFubenPersuadeMethod::crJXJDoFubenPersuadeMethod(const crJXJDoFubenPersuadeMethod& handle):
	crMethod(handle)
{
}
void crJXJDoFubenPersuadeMethod::inputParam(int i, void *param)
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

void crJXJDoFubenPersuadeMethod::addParam(int i, const std::string& str)
{
}

void crJXJDoFubenPersuadeMethod::operator()(crHandle &handle)
{
	//判断说服是否合法
	/************************************************************************/
	/*2.说服队列是否已满													*/
	/*3.说服将卡是否存在													*/
	/************************************************************************/
	// 如果合法将卡从背包中清除放入临时背包
	// 发送fubenid和将卡id到服务器，如果服务器判断可以说服，返回说服时间，说服倒计时开始
	// 否则提示
	void *param;
	crPlayerGameData * myPlayerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			crData *canvasData = canvas->getDataClass();
			crData *playerData = myPlayerGameData->getDataClass();
			if(playerData && canvasData)
			{
				canvasData->getParam(WCHDATA_JXJPersuadeFubenID,param);
				unsigned short fubenid = *(unsigned short *)param;
				canvasData->getParam(WCHDATA_JXJPersuadeJKID,param);
				int persuadeJkid = *(int *)param;

				playerData->excHandle(MAKEINT64(WCH_LockData,1));

				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeMaxCount,vipLevel).c_str());

				bool bNotInPersuade = true,bHasCard = false;
				playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
				PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;

				for (PersuadeInfoDeq::iterator itr_info = persuadeInfoDeq->begin();
					itr_info!=persuadeInfoDeq->end();itr_info++)
				{
					if(itr_info->get()->getFubenId() == fubenid)
					{
						bNotInPersuade = false;//该城池正在进行说服
						break;
					}
				}
				playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
				JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
				for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
					itr!=jiangkabagvec->end();
					++itr)
				{
					if(!itr->valid()) continue;
					if (itr->get()->getItemID() == persuadeJkid)
					{
						bHasCard = true;
						break;
					}
				}

				if(bNotInPersuade && bHasCard && persuadeInfoDeq->size()<maxCount)
				{
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					if(netConductor)
					{
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(6);
						stream->_writeUShort(fubenid);//2
						stream->_writeInt(persuadeJkid);//4
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenPersuade,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
						m_this->setEnable(false);
					}
				}
				else
				{
					//提示失败
				}
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvFubenPersuadeMethod
//
/////////////////////////////////////////
//crJXJRecvFubenPersuadeMethod::crJXJRecvFubenPersuadeMethod(){}
//crJXJRecvFubenPersuadeMethod::crJXJRecvFubenPersuadeMethod(const crJXJRecvFubenPersuadeMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJRecvFubenPersuadeMethod::inputParam(int i, void *param)
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
//void crJXJRecvFubenPersuadeMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_strCanvas = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJRecvFubenPersuadeMethod::operator()(crHandle &handle)
//{
//	if(m_netType == GameServer)
//	{
//		void *param; 
//		crTableIO::DataVec infoVec;
//		crTableIO::StrVec recordVec;
//		bool bHasCard = false;
//		//获取副本ID，将卡ID,根据副本ID查找说服相关信息
//		ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
//		ref_ptr<crTableIO>fubenCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPersuadeFubenCardTab);
//		ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//		unsigned short fubenid = m_stream->_readUShort();
//		int jkid = m_stream->_readInt();
//		int playerid = m_this->getPlayerID();
//		crData *playerData = m_this->getDataClass();
//		char _case = 0;//-1;队列已满，-2副本不允许说服；-3，委派将卡不存在,-4副本正在说服中
//		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//		crNetDataManager *netDataManager = gameServer->getNetDataManager();
//		ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
//		playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//		if(playerData && playerGameServerPlayerData.valid())
//		{
//			playerData->excHandle(MAKEINT64(WCH_LockData,1));
//			playerData->getParam(WCHDATA_JXJVipLv,param);
//			unsigned char vipLevel = *(unsigned char *)param;
//
//			playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
//			JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
//			for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
//				itr!=jiangkabagvec->end();
//				++itr)
//			{
//				if(!itr->valid()) continue;
//				if ((*itr)->getItemID() == jkid)
//				{
//					bHasCard = true;
//					break;
//				}
//			}
//
//
//			bool bCanPushBack = false,useCardSucess = false,bNotInPersuade = true;
//
//			if(bHasCard)
//			{
//				int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeMaxCount,vipLevel).c_str());
//				int time_w = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_W,vipLevel).c_str());				
//				playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
//				PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;
//			
//				if(persuadeInfoDeq->size()<maxCount)
//				{
//					bCanPushBack = true;
//				}
//				else
//				{
//					bCanPushBack = false;
//					_case = -1;//队列已满
//				}
//
//				for (PersuadeInfoDeq::iterator itr_info = persuadeInfoDeq->begin();
//					itr_info!=persuadeInfoDeq->end();itr_info++)
//				{
//					if(itr_info->get()->getFubenId() == fubenid)
//					{
//						bNotInPersuade = false;
//						_case = -4;//该城池正在进行说服
//						break;
//					}
//				}
//				if(bCanPushBack && bNotInPersuade)
//				{
//					time_t t = time(0);
//					CRCore::ref_ptr<crJXJPersuadeInfo> persuadeInfo = new crJXJPersuadeInfo;
//					persuadeInfo->setFubenId(fubenid);
//					if(fubentab.valid() && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),recordVec)>=0)
//					{
//						int canbeIndex = fubentab->getTitleIndex("允许说服");
//						bool isCanbe = atoi(recordVec[canbeIndex].c_str());
//						if(isCanbe)
//						{
//							int needTime = time_w;
//							if(itemTab.valid() && itemTab->queryOneRecord(0,crArgumentParser::appItoa(jkid),recordVec)>=0)
//							{
//								int qIndex = itemTab->getTitleIndex("品质");
//								switch (atoi(recordVec[qIndex].c_str()))
//								{
//								case 1:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_G,vipLevel).c_str());
//									persuadeInfo->setLevel(1);
//									break;
//								case 2:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_B,vipLevel).c_str());
//									persuadeInfo->setLevel(2);
//									break;
//								case 3:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_P,vipLevel).c_str());
//									persuadeInfo->setLevel(3);
//									break;
//								case 4:
//								case 5:
//								case 6:
//								case 7:
//								case 8:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_O,vipLevel).c_str());
//									persuadeInfo->setLevel(4);
//									break;
//								default:
//									persuadeInfo->setLevel(0);
//									needTime = time_w;
//									break;
//								}
//							}
//							if(fubenCardTab.valid())
//							{
//								int itemIndex = fubenCardTab->getTitleIndex("itemid");
//								int timeIndex = fubenCardTab->getTitleIndex("time");
//								fubenCardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),infoVec);
//								for(int i = 0;i<infoVec.size();i++)
//								{
//									if(infoVec[itemIndex][i].compare(crArgumentParser::appItoa(jkid)) == 0)
//									{
//										needTime = atoi(infoVec[timeIndex][i].c_str());
//										persuadeInfo->setLevel(5);
//										break;
//									}
//								}
//							}
//
//							persuadeInfo->setTime(t+needTime);
//							persuadeInfo->setJKId(jkid);
//							useCardSucess = true;
//							_case = 1;
//							persuadeInfoDeq->push_back(persuadeInfo);
//						}
//						else
//						{
//							//不允许说服
//							_case = -2;
//						}
//					}
//				}
//				//委派将卡
//				if(useCardSucess)
//				{
//					playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
//					JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
//					for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
//						itr!=jiangkabagvec->end();
//						++itr)
//					{
//						if(!itr->valid()) continue;
//						if ((*itr)->getItemID() == jkid)
//						{
//							(*itr) = NULL;
//							break;
//						}
//					}
//				}
//			}
//			else
//			{
//				//将卡不存在
//				_case = -3;
//			}
//
//			playerData->excHandle(MAKEINT64(WCH_LockData,0));
//
//			ref_ptr<crStreamBuf> stream = new crStreamBuf;
//			stream->createBuf(1);
//			stream->_writeChar(_case);//1
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvFubenPersuade,stream.get());
//			gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
//		}
//	}
//	else if(m_netType == GameClient_Game)
//	{
//		char _case = m_stream->_readChar();
//		crTableIO::StrVec recordVec;
//		crTableIO::DataVec infoVec;
//		ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
//		ref_ptr<crTableIO>fubenCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPersuadeFubenCardTab);
//		ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//
//		//int jkid = m_stream->_readInt();
//		void *param;
//		if(_case == 1)
//		{
//			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
//			if(canvas.valid())
//			{
//				crData *canvasData = canvas->getDataClass();
//				if(canvasData)
//				{
//					canvasData->getParam(WCHDATA_JXJPersuadeFubenID,param);
//					unsigned short persuadeFubenId = *(unsigned short *)param;
//					canvasData->getParam(WCHDATA_JXJPersuadeJKID,param);
//					int jkid = *(int *)param;
//
//					crData * playerData = m_this->getDataClass();
//					if(playerData)
//					{
//						playerData->excHandle(MAKEINT64(WCH_LockData,1));
//
//						playerData->getParam(WCHDATA_JXJVipLv,param);
//						unsigned char vipLevel = *(unsigned char *)param;
//
//						playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
//						PersuadeInfoDeq * persuadeInfoDeq = (PersuadeInfoDeq *)param;
//
//						//int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeMaxCount,vipLevel).c_str());
//						int time_w = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_W,vipLevel).c_str());	
//
//						//处理添加说服信息
//						CRCore::ref_ptr<crJXJPersuadeInfo> persuadeInfo = new crJXJPersuadeInfo;
//						persuadeInfo->setFubenId(persuadeFubenId);
//						if(fubentab.valid() && fubentab->queryOneRecord(0,crArgumentParser::appItoa(persuadeFubenId),recordVec)>=0)
//						{
//							int canbeIndex = fubentab->getTitleIndex("允许说服");
//							bool isCanbe = atoi(recordVec[canbeIndex].c_str());
//
//							int needTime = time_w;
//							if(itemTab.valid() && itemTab->queryOneRecord(0,crArgumentParser::appItoa(jkid),recordVec)>=0)
//							{
//								int qIndex = itemTab->getTitleIndex("品质");
//								switch (atoi(recordVec[qIndex].c_str()))
//								{
//								case 1:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_G,vipLevel).c_str());
//									persuadeInfo->setLevel(1);
//									break;
//								case 2:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_B,vipLevel).c_str());
//									persuadeInfo->setLevel(2);
//									break;
//								case 3:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_P,vipLevel).c_str());
//									persuadeInfo->setLevel(3);
//									break;
//								case 4:
//								case 5:
//								case 6:
//								case 7:
//								case 8:
//									needTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPersuadeTime_O,vipLevel).c_str());
//									persuadeInfo->setLevel(4);
//									break;
//								default:
//									persuadeInfo->setLevel(0);
//									needTime = time_w;
//									break;
//								}
//							}
//							if(fubenCardTab.valid())
//							{
//								int itemIndex = fubenCardTab->getTitleIndex("itemid");
//								int timeIndex = fubenCardTab->getTitleIndex("time");
//								fubenCardTab->queryRecords(0,crArgumentParser::appItoa(persuadeFubenId),infoVec);
//								for(int i = 0;i<infoVec.size();i++)
//								{
//									if(infoVec[itemIndex][i].compare(crArgumentParser::appItoa(jkid)) == 0)
//									{
//										needTime = atoi(infoVec[timeIndex][i].c_str());
//										persuadeInfo->setLevel(5);
//										break;
//									}
//								}
//							}
//							persuadeInfo->setTime(needTime);
//							persuadeInfo->setJKId(jkid);
//							persuadeInfoDeq->push_back(persuadeInfo);
//						}
//						//处理将卡
//						playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
//						JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
//						for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
//							itr!=jiangkabagvec->end();
//							++itr)
//						{
//							if(!itr->valid()) continue;
//							if ((*itr)->getItemID() == jkid)
//							{
//								(*itr) = NULL;
//								break;
//							}
//						}
//						playerData->excHandle(MAKEINT64(WCH_LockData,0));
//					}
//				}
//			}
//		}
//	}
//}


/////////////////////////////////////////
//
//crJXJRecvFubenPersuadeSucessMethod
//
/////////////////////////////////////////
crJXJRecvFubenPersuadeSucessMethod::crJXJRecvFubenPersuadeSucessMethod(){}
crJXJRecvFubenPersuadeSucessMethod::crJXJRecvFubenPersuadeSucessMethod(const crJXJRecvFubenPersuadeSucessMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvFubenPersuadeSucessMethod::inputParam(int i, void *param)
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

void crJXJRecvFubenPersuadeSucessMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strTaskCanvas = str;
		break;
	case 1:
		m_strPlay = str;
		break;
	case 2:
		m_strPersuadeRewardCanvas = str;
		break;
	case 3:
		m_strText1 = str;
		break;
	case 4:
		m_strText2 = str;
		break;
	default:
		break;
	}
}

void crJXJRecvFubenPersuadeSucessMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		void *param; 
		crVector3i vec3;
		bool hasNext = false,isBagEmpty = false,isSucess = false;
		char _case = 0;
		crTableIO::DataVec rewardVec;
		ref_ptr<crTableIO>persuadeRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPersuadeRewardTab);
		unsigned short fubenid = m_stream->_readUShort();
		int jkid = 0;
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *playerData = m_this->getDataClass();
		if(playerData && playerGameServerPlayerData.valid())
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;

			playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
			for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
				itr!=jiangkabagvec->end();
				++itr)
			{
				if ((*itr) == NULL)
				{
					isBagEmpty = true;
					break;
				}
			}

			if(isBagEmpty)
			{
				playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
				PersuadeInfoDeq * persuadeDeq = (PersuadeInfoDeq *)param;
				if(!persuadeDeq->empty())
				{
					ref_ptr<crJXJPersuadeInfo> persuadeInfo = persuadeDeq->front();
					if(persuadeInfo.valid())
					{
						time_t t = time(0);
						if(persuadeInfo->getTime()<t)
						{
							//发放奖励
							unsigned short fubenid = persuadeInfo->getFubenId();
							unsigned char level = persuadeInfo->getLevel();
							if(persuadeRewardTab.valid())
							{
								int levelIndex = persuadeRewardTab->getTitleIndex("说服品质");
								int foodIndex = persuadeRewardTab->getTitleIndex("粮食");
								int copperIndex = persuadeRewardTab->getTitleIndex("铜钱");
								int woodIndex = persuadeRewardTab->getTitleIndex("木材");
								int ironIndex = persuadeRewardTab->getTitleIndex("铁矿");
								int horseIndex = persuadeRewardTab->getTitleIndex("马匹");
								int itemIndex = persuadeRewardTab->getTitleIndex("物品");
								persuadeRewardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),rewardVec);
								if(level>5)level = 5;


								ref_ptr<crPlayerRewardData> rewardInfo = new crPlayerRewardData;
								if(!rewardVec.empty())
								{
									rewardInfo->setCoopercash(atoi(rewardVec[copperIndex][level].c_str()));
									rewardInfo->setFood(atoi(rewardVec[foodIndex][level].c_str()));
									rewardInfo->setWood(atoi(rewardVec[woodIndex][level].c_str()));
									rewardInfo->setIron(atoi(rewardVec[ironIndex][level].c_str()));
									rewardInfo->setHorse(atoi(rewardVec[horseIndex][level].c_str()));

									if(crArgumentParser::appAtoVec(rewardVec[itemIndex][level],vec3) && vec3[0]>0)
									{
										ref_ptr<crBagItemData> itemdata = new crBagItemData;
										itemdata->setItemID(vec3[0]);
										itemdata->setEquipMagic(vec3[1]);
										itemdata->setItemCount(vec3[2]);
										rewardInfo->getRewardItemsVec().push_back(itemdata);
									}
								}
								
								isSucess = true;
								_case = 1;
								jkid = persuadeInfo->getJKId();
								rewardInfo->setType(GP_RewardShuofu);
								m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(rewardInfo.get(),NULL));
							}
						}
					}

					//返还委派说服将卡
					if(isSucess && jkid>0)
					{
						playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
						JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;

						ref_ptr<crBagItemData> carddata = new crBagItemData;
						carddata->setItemID(jkid);
						carddata->setItemCount(1);
						for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
							itr!=jiangkabagvec->end();
							++itr)
						{
							if ((*itr) == NULL)
							{
								*itr = carddata.get();
								break;
							}
						}

						//完成说服
						persuadeDeq->pop_front();
					}
				}
			}
			else
			{
				//将卡背包满//无法返还将卡
				_case = -1;
			}	

			playerData->excHandle(MAKEINT64(WCH_LockData,0));

			stream->createBuf(1);
			stream->_writeChar(_case);//1
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvFubenPersuadeSucess,stream.get());
			gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
			
		}
	}
	else if(m_netType == GameClient_Game)
	{
		char _case = m_stream->_readChar();
		if(_case == 1)
		{
			//气泡消失
			ref_ptr<crSequence>seqBubble = NULL;
			ref_ptr<crCanvasNode>taskbarCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strTaskCanvas);
			if(taskbarCanvas.valid())
			{
				seqBubble = dynamic_cast<crSequence*>(taskbarCanvas->getChildNode(m_strPlay));
				if(seqBubble.valid())
				{
					seqBubble->setVanishWhenStop(true);
				}
			}
			crData *playerData = m_this->getDataClass();
			if(playerData)
			{
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
				PersuadeInfoDeq * persuadeDeq = (PersuadeInfoDeq *)param;
				CRCore::ref_ptr<crJXJPersuadeInfo> persuadeInfo = persuadeDeq->front();

				//返还将卡
				playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
				JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;

				if(persuadeInfo.valid())
				{
					ref_ptr<crBagItemData> carddata = new crBagItemData;
					carddata->setItemID(persuadeInfo->getJKId());
					carddata->setItemCount(1);
					for (JXJItemBagVec::iterator itr = jiangkabagvec->begin();
						itr!=jiangkabagvec->end();
						++itr)
					{
						if ((*itr) == NULL)
						{
							*itr = carddata.get();
							break;
						}
					}

					//显示奖励提示界面
					ref_ptr<crCanvasNode>rewardCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strPersuadeRewardCanvas);
					if(rewardCanvas.valid())
					{
						ref_ptr<crStaticTextWidgetNode>text_up = dynamic_cast<crStaticTextWidgetNode *>(rewardCanvas->getWidget(m_strText1));
						ref_ptr<crStaticTextWidgetNode>text_down = dynamic_cast<crStaticTextWidgetNode *>(rewardCanvas->getWidget(m_strText2));

						crTableIO::DataVec rewardVec;
						crVector3i vec3;
						ref_ptr<crTableIO>persuadeRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPersuadeRewardTab);
						unsigned short fubenid = persuadeInfo->getFubenId();
						unsigned char level = persuadeInfo->getLevel();
						std::string strRewardInfo;
						if(persuadeRewardTab.valid())
						{
							int levelIndex = persuadeRewardTab->getTitleIndex("说服品质");
							int foodIndex = persuadeRewardTab->getTitleIndex("粮食");
							int copperIndex = persuadeRewardTab->getTitleIndex("铜钱");
							int woodIndex = persuadeRewardTab->getTitleIndex("木材");
							int ironIndex = persuadeRewardTab->getTitleIndex("铁矿");
							int horseIndex = persuadeRewardTab->getTitleIndex("马匹");
							int itemIndex = persuadeRewardTab->getTitleIndex("物品");
							persuadeRewardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),rewardVec);
							if(level>5)level = 5;


							ref_ptr<crPlayerRewardData> rewardInfo = new crPlayerRewardData;
							if(!rewardVec.empty())
							{
								int copoper = atoi(rewardVec[copperIndex][level].c_str());
								int food = atoi(rewardVec[foodIndex][level].c_str());
								int wood = atoi(rewardVec[woodIndex][level].c_str());
								int iron = atoi(rewardVec[ironIndex][level].c_str());
								int horse = atoi(rewardVec[horseIndex][level].c_str());
								if(copoper>0)
									strRewardInfo+=std::string("铜钱：")+rewardVec[copperIndex][level]+std::string(" ");
								if(food>0)
									strRewardInfo+=std::string("粮草：")+rewardVec[foodIndex][level]+std::string(" ");
								if(wood>0)
									strRewardInfo+=std::string("木材：")+rewardVec[woodIndex][level]+std::string(" ");
								if(iron>0)
									strRewardInfo+=std::string("铁矿：")+rewardVec[ironIndex][level]+std::string(" ");
								if(horse>0)
									strRewardInfo+=std::string("马匹：")+rewardVec[horseIndex][level]+std::string(" ");

								if(crArgumentParser::appAtoVec(rewardVec[itemIndex][level],vec3) && vec3[0]>0)
								{
									ref_ptr<crBagItemData> itemdata = new crBagItemData;
									int itemid = vec3[0];
									int magicid = vec3[1];
									unsigned char count = (unsigned char)vec3[2];
									
									ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
									crTableIO::StrVec itemrecord;
									if(itemtab.valid())
									{
										int name_index = itemtab->getTitleIndex("name");
										if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0 && name_index>=0)
										{
											strRewardInfo+=string("[")+itemrecord[name_index]+string("]")+string("*")+crArgumentParser::appItoa(count);
										}
									}
								}
							}
						}
						if(!strRewardInfo.empty() && text_down.valid())
						{
							text_down->setString(strRewardInfo);
						}
						if(text_up.valid())
						{
							text_up->setString(std::string("说服成功获得大量贡品："));
						}
						crFilterRenderManager::getInstance()->doModal(rewardCanvas.get());
					}
				}
				
				persuadeDeq->pop_front();

				if(seqBubble.valid())
				{
					seqBubble->setMode(crSequence::STOP);
				}

				if(!persuadeDeq->empty())
				{
					ref_ptr<crJXJPersuadeInfo> persuadeInfo = persuadeDeq->front();
					
					if(persuadeInfo->getTime()<=0)
					{
						//气泡显示
						if(seqBubble.valid())
						{
							seqBubble->setVisiable(true);
							seqBubble->setMode(crSequence::START);
						}
					}
				}

				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else
		{
			//提示整理将卡背包
		}
	}
}

/////////////////////////////////////////
//
//crJXJFubenPersuadeTimeUpdateMethod
//
/////////////////////////////////////////
crJXJFubenPersuadeTimeUpdateMethod::crJXJFubenPersuadeTimeUpdateMethod():m_persuadeSize(0){}
crJXJFubenPersuadeTimeUpdateMethod::crJXJFubenPersuadeTimeUpdateMethod(const crJXJFubenPersuadeTimeUpdateMethod& handle):
	crMethod(handle),m_persuadeSize(0)
{
}
void crJXJFubenPersuadeTimeUpdateMethod::inputParam(int i, void *param)
{
}

void crJXJFubenPersuadeTimeUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strTaskCanvas = str;
		break;
	case 1:
		m_strPlay = str;
		break;
	default:
		break;
	}
}

void crJXJFubenPersuadeTimeUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
		PersuadeInfoDeq *persuadeInfoDeq = (PersuadeInfoDeq *)param;

		PersuadeInfoDeq::iterator itr = persuadeInfoDeq->begin();
		int time = 0;
		for (;itr!=persuadeInfoDeq->end();itr++)
		{
			time = itr->get()->getTime();
			if(time>0)
			{
				time-=1;
				itr->get()->setTime(time);
			}
			else if(time == 0 && m_persuadeSize!=persuadeInfoDeq->size())
			{
				//
				m_persuadeSize = persuadeInfoDeq->size();
				//显示气泡
				ref_ptr<crSequence>seqBubble = NULL;
				ref_ptr<crCanvasNode>taskbarCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strTaskCanvas);
				if(taskbarCanvas.valid())
				{
					seqBubble = dynamic_cast<crSequence*>(taskbarCanvas->getChildNode(m_strPlay));
					if(seqBubble.valid())
					{
						seqBubble->setVisiable(true);
						seqBubble->setMode(crSequence::START);
					}
				}

// 				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
// 				if(netConductor)
// 				{
// 					crPlayerDataEventPacket packet;
// 					ref_ptr<crStreamBuf> stream = new crStreamBuf;
// 					stream->createBuf(2);
// 					stream->_writeUShort(itr->get()->getFubenId());//2
// 					//stream->_writeInt(persuadeJkid);//4
// 					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenPersuadeSucess,stream.get());
// 					netConductor->getNetManager()->sendPacket("all",packet);
// 				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJFubenPersuadeSucessMethod
//
/////////////////////////////////////////
crJXJFubenPersuadeSucessMethod::crJXJFubenPersuadeSucessMethod(){}
crJXJFubenPersuadeSucessMethod::crJXJFubenPersuadeSucessMethod(const crJXJFubenPersuadeSucessMethod& handle):
	crMethod(handle)
{
}
void crJXJFubenPersuadeSucessMethod::inputParam(int i, void *param)
{
}

void crJXJFubenPersuadeSucessMethod::addParam(int i, const std::string& str)
{
}

void crJXJFubenPersuadeSucessMethod::operator()(crHandle &handle)
{
	void *param;
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
		PersuadeInfoDeq *persuadeInfoDeq = (PersuadeInfoDeq *)param;
		if(!persuadeInfoDeq->empty())
		{
			CRCore::ref_ptr<crJXJPersuadeInfo> persuadeInfo = persuadeInfoDeq->front();
			if(persuadeInfo.valid() && persuadeInfo->getTime()<=0)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(2);
					stream->_writeUShort(persuadeInfo->getFubenId());//2
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenPersuadeSucess,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJFubenPersuadeUIUpdateMethod
//
/////////////////////////////////////////
crJXJFubenPersuadeUIUpdateMethod::crJXJFubenPersuadeUIUpdateMethod(){}
crJXJFubenPersuadeUIUpdateMethod::crJXJFubenPersuadeUIUpdateMethod(const crJXJFubenPersuadeUIUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJFubenPersuadeUIUpdateMethod::inputParam(int i, void *param)
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

void crJXJFubenPersuadeUIUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strTimeText = str;
		break;
	default:
		break;
	}
}

void crJXJFubenPersuadeUIUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	crData *canvasData = m_this->getDataClass();
	ref_ptr<crStaticTextWidgetNode>   timeTextWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strTimeText));
	if(playerData && canvasData)
	{
		canvasData->getParam(WCHDATA_JXJPersuadeFubenID,param);
		unsigned short fubenid = *(unsigned short *)param;

		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPersuadeDeq,param);
		PersuadeInfoDeq *persuadeInfoDeq = (PersuadeInfoDeq *)param;

		PersuadeInfoDeq::iterator itr = persuadeInfoDeq->begin();
		int time = 0;
		for (;itr!=persuadeInfoDeq->end();itr++)
		{
			if(itr->get()->getFubenId() == fubenid)
			{
				time = itr->get()->getTime();

				int hour = time/3600;
				int minute = (time%3600)/60;
				int second = (time%3600)%60;

				char timetext[128];
				sprintf(timetext,"%02d:%02d:%02d\0",hour,minute,second);

				if(timeTextWidget.valid())
					timeTextWidget->setString(std::string(timetext));
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJCheckGameServerCycleStartMethod
//
/////////////////////////////////////////
crJXJCheckGameServerCycleStartMethod::crJXJCheckGameServerCycleStartMethod(){}
crJXJCheckGameServerCycleStartMethod::crJXJCheckGameServerCycleStartMethod(const crJXJCheckGameServerCycleStartMethod& handle):
	crMethod(handle),
	m_cityLose(0)
{
}
void crJXJCheckGameServerCycleStartMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle *)param;
		break;
	case 2:
		if(param)
		{
			_crInt16 param16 = *(_crInt16*)param;
			m_cityLose = (char)(LOINT16(param16));
		}
		else
		{
			m_cityLose = 0;
		}
		break;
	}
}

void crJXJCheckGameServerCycleStartMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckGameServerCycleStartMethod::operator()(crHandle &handle)
{
	void *param;
	crData *brainData = m_this->getDataClass();
	if(brainData)
	{		
		crJXJShili *shili[3]  = {NULL};
		brainData->excHandle(MAKEINT64(WCH_LockData,1));
		brainData->getParam(WCHDATA_JXJShiliWei,param);
		shili[0] = (crJXJShili *)param;
		brainData->getParam(WCHDATA_JXJShiliShu,param);
		shili[1] = (crJXJShili *)param;
		brainData->getParam(WCHDATA_JXJShiliWu,param);
		shili[2] = (crJXJShili *)param;
		brainData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;

		int shiliid[3];
		shiliid[0] = shili[0]->getID();
		shiliid[1] = shili[1]->getID();
		shiliid[2] = shili[2]->getID();

		int loseshiliid = 0;
		int lastshiliid = 0;
		bool isLoseCountry = false;
		int countryLoseCount = 0;
		int changecityCount = 0;
		ChengchiMap::iterator itr;
		for (int i = 0;i<3;i++)
		{
			changecityCount = 0;
			itr = chengchimap->begin();
			isLoseCountry = false;
			m_this->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid[i]));
			if(isLoseCountry)
			{
				countryLoseCount++;
				////将灭国的城池变为野城
				//while (itr!=chengchimap->end())
				//{
				//	if(itr->second.first == shiliid[i])
				//	{
				//		itr->second.first = c_npcShiliID;
				//		changecityCount++;
				//	}
				//	itr++;
				//}
				////////////////////////////////////////////////////////////////////////////
				////通知所有玩家客户端变野城
				//if(changecityCount>0)
				//{
				//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//	stream->createBuf(1);
				//	stream->_writeUChar(shiliid[i]);
				//	crPlayerDataEventPacket packet;
				//	crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJChangeAllCityShili,stream.get());
				//	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
				//	crNetDataManager *netDataManager = netConductor->getNetDataManager();
				//	netDataManager->sendPacketToAllPlayer(packet);
				//}
				//////////////////////////////////////////////////////////////////////////
				loseshiliid = shiliid[i];
			}
			else
			{
				lastshiliid = shiliid[i];
			}
		}
		if(countryLoseCount == 1 && m_cityLose == -1)
		{
			bool bStart = false;
			if(loseshiliid>0)
				m_this->doEvent(WCH_JXJCreateCountryLoseNotice,MAKEINT64(loseshiliid,&bStart));
		}
		else if(countryLoseCount>=2 && m_cityLose == -1)
		{
			//开始轮回
			bool bStart = true;
			float cycleTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJServerCycleTotalTime,0).c_str());
			brainData->inputParam(WCHDATA_JXJIsServerCycling,&bStart);
			brainData->inputParam(WCHDATA_JXJServerCycleTimer,&cycleTime);
			
			//通知所有玩家
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeBool(bStart);
			stream->_writeInt(cycleTime);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJRecvServerCycleStart,stream.get());
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = netConductor->getNetDataManager();
			netDataManager->sendPacketToAllPlayer(packet);
			if(lastshiliid>0)
				m_this->doEvent(WCH_JXJCreateCountryLoseNotice,MAKEINT64(lastshiliid,&bStart));
		}
		brainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJRecvChangeAllCityShiliMethod
//
/////////////////////////////////////////
crJXJRecvChangeAllCityShiliMethod::crJXJRecvChangeAllCityShiliMethod(){}
crJXJRecvChangeAllCityShiliMethod::crJXJRecvChangeAllCityShiliMethod(const crJXJRecvChangeAllCityShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvChangeAllCityShiliMethod::inputParam(int i, void *param)
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

void crJXJRecvChangeAllCityShiliMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvChangeAllCityShiliMethod::operator()(crHandle &handle)
{
	if(m_netType == GameClient_Game)
	{
		//void *param;
		//crData * playerData = m_this->getDataClass();
		//if(playerData)
		//{
		//	playerData->excHandle(MAKEINT64(WCH_LockData,1));
		//	playerData->getParam(WCHDATA_JXJChengChiMap,param);
		//	ChengchiMap *chengchimap = (ChengchiMap *)param;
		//	unsigned char loseShili = m_stream->_readUChar();
		//	ChengchiMap::iterator itr = chengchimap->begin();
		//	while (itr!=chengchimap->end())
		//	{
		//		if(itr->second.first == loseShili)
		//		{
		//			itr->second.first = c_npcShiliID;
		//		}
		//		itr++;
		//	}
		//	playerData->excHandle(MAKEINT64(WCH_LockData,0));
		//}
	}
}

/////////////////////////////////////////
//
//crJXJRecvServerCycleStartMethod
//
/////////////////////////////////////////
crJXJRecvServerCycleStartMethod::crJXJRecvServerCycleStartMethod(){}
crJXJRecvServerCycleStartMethod::crJXJRecvServerCycleStartMethod(const crJXJRecvServerCycleStartMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvServerCycleStartMethod::inputParam(int i, void *param)
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

void crJXJRecvServerCycleStartMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvServerCycleStartMethod::operator()(crHandle &handle)
{
	//void *param;
	if(m_netType == GameClient_Game)
	{
		bool isCycle = m_stream->_readBool();
		int cycleTime = m_stream->_readInt();
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->inputParam(WCHDATA_JXJClientIsServerCycling,&isCycle);
			playerData->inputParam(WCHDATA_JXJClientCycleTimer,&cycleTime);
		}
	}
}
/////////////////////////////////////////
//
//crJXJGameServerCycleTimerUpdateMethod
//
/////////////////////////////////////////
crJXJGameServerCycleTimerUpdateMethod::crJXJGameServerCycleTimerUpdateMethod():
m_dt(NULL){}
crJXJGameServerCycleTimerUpdateMethod::crJXJGameServerCycleTimerUpdateMethod(const crJXJGameServerCycleTimerUpdateMethod& handle):
	crMethod(handle),
	m_dt(NULL)
{
}
void crJXJGameServerCycleTimerUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crServerBrainHandle *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			if(lparam)
				m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJGameServerCycleTimerUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJGameServerCycleTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *brainData = m_this->getDataClass();
	if(brainData)
	{
		brainData->getParam(WCHDATA_JXJIsServerCycling,param);
		bool isCycling = *(bool *)param;
		if(isCycling)
		{
			brainData->excHandle(MAKEINT64(WCH_LockData,1));
			brainData->getParam(WCHDATA_JXJServerCycleTimer,param);
			float * cycleTimer = (float *)param;
			if(*cycleTimer>0.0f)
				(*cycleTimer)-=*m_dt;
			else
			{
				//关服断连
				m_this->doEvent(WCH_JXJServerCycle);

				isCycling = false;
				brainData->inputParam(WCHDATA_JXJIsServerCycling,&isCycling);
			}
			brainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else
		{//是否大于100天了，如果是就自动开启轮回
			int tianshu = 0;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			if(tianshu>=100)
			{
				bool bStart = true;
				float cycleTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJServerCycleTotalTime,0).c_str());
				brainData->inputParam(WCHDATA_JXJIsServerCycling,&bStart);
				brainData->inputParam(WCHDATA_JXJServerCycleTimer,&cycleTime);

				//通知所有玩家
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5);
				stream->_writeBool(bStart);
				stream->_writeInt(cycleTime);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJRecvServerCycleStart,stream.get());
				crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
				crNetDataManager *netDataManager = netConductor->getNetDataManager();
				netDataManager->sendPacketToAllPlayer(packet);
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJGameClientCycleTimerUpdateMethod
//
/////////////////////////////////////////
crJXJGameClientCycleTimerUpdateMethod::crJXJGameClientCycleTimerUpdateMethod(){}
crJXJGameClientCycleTimerUpdateMethod::crJXJGameClientCycleTimerUpdateMethod(const crJXJGameClientCycleTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJGameClientCycleTimerUpdateMethod::inputParam(int i, void *param)
{

}

void crJXJGameClientCycleTimerUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJGameClientCycleTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;

	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->getParam(WCHDATA_JXJClientIsServerCycling,param);
		bool isCycling = *(bool *)param;
		if(isCycling)
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJClientCycleTimer,param);
			int * cycleTimer = (int *)param;
			if(cycleTimer>0)
				(*cycleTimer)-=1;
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJTreasonCountryCanvasShowMethod
//
/////////////////////////////////////////
crJXJTreasonCountryCanvasShowMethod::crJXJTreasonCountryCanvasShowMethod(){}
crJXJTreasonCountryCanvasShowMethod::crJXJTreasonCountryCanvasShowMethod(const crJXJTreasonCountryCanvasShowMethod& handle):
	crMethod(handle)
{
}
void crJXJTreasonCountryCanvasShowMethod::inputParam(int i, void *param)
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

void crJXJTreasonCountryCanvasShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch = str;
		break;
	case 1:
		m_strRadioGroup = str;
		break;
	case 2:
		m_strRadioWidget_wei = str;
		break;
	case 3:
		m_strRadioWidget_shu = str;
		break;
	case 4:
		m_strRadioWidget_wu = str;
		break;
	case 5:
		m_strSwitch_wei = str;
		break;
	case 6:
		m_strSwitch_shu = str;
		break;
	case 7:
		m_strSwitch_wu = str;
		break;
	case 8:
		m_strText = str;
		break;
	case 9:
		m_powersw_wei = str;
		break;
	case 10:
		m_powersw_shu = str;
		break;
	case 11:
		m_powersw_wu = str;
		break;
	}
}

void crJXJTreasonCountryCanvasShowMethod::operator()(crHandle &handle)
{
	//检查三个势力那个灭亡切换switch
	ref_ptr<crMultiSwitch>  countrypowersw[3] = {NULL};
	ref_ptr<crMultiSwitch>  switchShili[3] = {NULL};
	crRadioWidgetNode *radioWidget[3] = {NULL};
	ref_ptr<crMultiSwitch>  switchTitle = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch));
	ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_strRadioGroup));
	radioWidget[0] = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_strRadioWidget_wei));
	radioWidget[1] = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_strRadioWidget_shu));
	radioWidget[2] = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_strRadioWidget_wu));
	switchShili[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch_wei));
	switchShili[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch_shu));
	switchShili[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch_wu));
	countrypowersw[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_powersw_wei));
	countrypowersw[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_powersw_shu));
	countrypowersw[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_powersw_wu));
	//curshili[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[0]));
	//curshili[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[1]));
	//curshili[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[2]));
	//curshili[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[0]));
	//curshili[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[1]));
	//curshili[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strShiLiS[2]));
	ref_ptr<crStaticTextWidgetNode> timesTextWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strText));
	if(!switchTitle || !radioGroup || !radioWidget[0] || !radioWidget[1] || !radioWidget[2] \
		|| !switchShili[0] || !switchShili[1] || !switchShili[2]) 
		return;
	//检查本势力是否灭亡
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *playerData = myPlayer->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myShiliid = *(unsigned char *)param;

			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;

			playerData->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
			unsigned char changeTimes = *(unsigned char *)param;
			int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiliMaxCount,vipLevel).c_str());

			if(timesTextWidget.valid() && maxCount>0)
			{
				timesTextWidget->setString(crArgumentParser::appItoa(changeTimes)+std::string("/")+crArgumentParser::appItoa(maxCount));
			}
			//bool isLoseCountry = false;
			bool islose = false;
			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&islose,&myShiliid));
			if(islose){
				switchTitle->setActiveSwitchSet(1);
			}else{
				switchTitle->setActiveSwitchSet(0);
			}

			ref_ptr<crData> canvasdata = m_this->getDataClass();
			canvasdata->getParam(WCHDATA_JXJCountryStrength,param);
			crVector3i vcountrypower = *(crVector3i *)param;
			unsigned char shili = 0;
			for (int i = 0;i<3;i++){
				islose = false;
				shili = c_startShiliID+i;

				myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&islose,&shili));

				if(islose){
					switchShili[i]->setActiveSwitchSet(2);
					radioWidget[i]->setEnable(false);
					if(countrypowersw[i].valid())
						countrypowersw[i]->setActiveSwitchSet(4);
				}else{
					if(myShiliid == shili){
						radioWidget[i]->setEnable(false);
						switchShili[i]->setActiveSwitchSet(1);
					}
					else
					{
						radioWidget[i]->setEnable(true);
						switchShili[i]->setActiveSwitchSet(0);
					}

					if(countrypowersw[i].valid())
						countrypowersw[i]->setActiveSwitchSet(3-vcountrypower[i]);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerTryToChangeShiliMethod
//
/////////////////////////////////////////
crJXJPlayerTryToChangeShiliMethod::crJXJPlayerTryToChangeShiliMethod(){}
crJXJPlayerTryToChangeShiliMethod::crJXJPlayerTryToChangeShiliMethod(const crJXJPlayerTryToChangeShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJPlayerTryToChangeShiliMethod::inputParam(int i, void *param)
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

void crJXJPlayerTryToChangeShiliMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strRadioGroup = str;
		break;
	case 1:
		m_strMsgCanvas = str;
		break;
	case 2:
		m_strMsg = str;
		break;
	case 3:
		m_citywei = str;
		break;
	case 4:
		m_cityshu = str;
		break;
	case 5:
		m_citywu = str;
		break;
	}
}

void crJXJPlayerTryToChangeShiliMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
	void *param;
	if(canvas.valid()){
		ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_strRadioGroup));
		if(radioGroup.valid()){
			unsigned char shiliid = c_startShiliID + (unsigned char)radioGroup->getSelect();

			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if(myPlayer){
				crData *playerData = myPlayer->getDataClass();
				if(playerData){
					playerData->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myShiliid = *(unsigned char *)param;

					bool isLoseCountry = false;
					myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myShiliid));

					if(!isLoseCountry)
					{
						playerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						playerData->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
						unsigned char *changeTimes = (unsigned char *)param;
						playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
						unsigned short changchiid = *(unsigned short *)param;
						if(myShiliid!=shiliid){
							crRole *mainRole = myPlayer->getMainRole();
							if(mainRole)
							{
								crData *mainroledata = mainRole->getDataClass();
								mainroledata->getParam(WCHDATA_Level,param);
								unsigned char rolelv = *(unsigned char *)param;
								playerData-> getParam(WCHDATA_JXJConsortiaID,param);
								int mygroupid = *( int *)param;

								unsigned char needlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedRoleLv,vipLevel).c_str()));
								if(changchiid !=0)
								{
									//在国战排队中
									int texid = 4017;
									crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
									int mode = 1;
									noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
									noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
									crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
								}
								else
								{
									if(needlv>rolelv)
									{
										//提示等级不足
										int texid = 4002;
										crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
										int mode = 1;
										noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
										noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
										crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
									}
									else
									{
										if(mygroupid!=0)
										{
											//提示退出军团
											int texid = 4003;
											crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
											int mode = 1;
											noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
											noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
											crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
										}
										else
										{
											unsigned char needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedGold,vipLevel).c_str());
											playerData->getParam(WCHDATA_JXJGoldingot,param);
											int *goldingot = (int*)param;
											playerData->getParam(WCHDATA_JXJGiftGold,param);
											int *giftgold = (int*)param;

											if(needgold> ((*goldingot)+ (*giftgold)))
											{
												//提示元宝不足
												int texid = 4004;
												crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
												int mode = 1;
												noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
												noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
												std::string param1 = crArgumentParser::appItoa(needgold);
												noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
												crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
											}
											else
											{
												/*std::string city;
												bool flag = false;
												playerData->getParam(WCHDATA_JXJChengChiMap,param);
												ChengchiMap *map_city = (ChengchiMap *)param;
												if (myShiliid == 10)
												city = m_citywei;
												else if(myShiliid == 11)
												city = m_cityshu;
												else
												city = m_citywu;

												std::vector<string> strCity;
												std::vector<int> index;
												for (int i=0;i!=city.size();i++)
												{
												if(city[i] == '|')
												index.push_back(i);
												}
												for (int i=0;i<=index.size();i++)
												{
												if(i == 0)
												strCity.push_back(city.substr(0,index[0]));
												else if (i == index.size())
												strCity.push_back(city.substr(index[i-1]+1,city.size()-index[i-1]-1));
												else
												strCity.push_back(city.substr(index[i-1]+1,index[i]-index[i-1]-1));
												}
												for (int i=0;i!=strCity.size();i++)
												{
												ChengchiMap::iterator itr = map_city->find(atoi(strCity[i].c_str()));
												if (itr != map_city->end())
												if (myShiliid != itr->second->getChengzhuShili())
												{
												flag = true;
												break;
												}
												}*/

												if (false)
												{
													//提示建国之路城池被攻占
													int texid = 9148;
													crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
													int mode = 1;
													noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
													noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
													std::string param1 = crArgumentParser::appItoa(needgold);
													noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
													crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
												}
												else
												{
													OfficalVec official;
													ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
													if (canvas.valid())
													{
														crData *canvasdata = canvas->getDataClass();
														canvasdata->getParam(WCHDATA_JXJCountryCanvasOfficalVec,param);
														official = *(OfficalVec *)param;
													}
													int myid = myPlayer->getPlayerID();

													if (myid == official[0].first || myid == official[1].first || myid == official[2].first)
													{
														//提示官员不能叛国
														int texid = 9149;
														crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
														int mode = 1;
														noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
														noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
														std::string param1 = crArgumentParser::appItoa(needgold);
														noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
														crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
													}
													else
													{
														//提示在叛国冷却期
														playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
														int coolingtime = *(int*)param;
														if (coolingtime > 0)
														{
															int texid = 9153;
															crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
															int mode = 1;
															noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
															noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
															std::string param1 = crArgumentParser::appItoa(needgold);
															noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
															crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
														}
														else
														{
															//提示在选举日
															time_t nowTime_t = time(0);
															struct  tm  nowTm = *localtime(&nowTime_t);
															if (nowTm.tm_wday==0 || nowTm.tm_wday==6)
															{
																int texid = 9154;
																crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
																int mode = 1;
																noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
																noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
																std::string param1 = crArgumentParser::appItoa(needgold);
																noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
																crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
															} 
															else
															{
																ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strMsgCanvas);
																char szText[256];
																unsigned char needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedGold,vipLevel).c_str());
																ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
																crTableIO::StrVec txtrecord;
																std::string str;
																if(textTab->queryOneRecord(0,crArgumentParser::appItoa(/*4016*/9150),txtrecord)>=0)
																{
																	str = txtrecord[1];
																	sprintf_s(szText,sizeof(szText),str.c_str(),needgold);
																}

																ItemMap itemMap;
																rcfg::ConfigScript cfg_script(&itemMap);
																cfg_script.Add("Hypertext");
																cfg_script.Push("Hypertext");
																cfg_script.Add("Content");
																cfg_script.Push("Content");
																cfg_script.Add("Text",/*string(szText)*/str);
																cfg_script.Pop();
																cfg_script.Pop();

																if(msgCanvas.valid())
																{
																	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_strMsg));
																	if(msg.valid())
																		msg->setHypertext(cfg_script);
																	crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
																	crData *dlgData = msgCanvas->getDataClass();
																	int commandtype = CDP_Widget;
																	dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
																	dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}

						}else{
							//提示不能选同势力
							int texid = 4006;
							crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
							int mode = 1;
							noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
							noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
							crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);	
						}
					}
					else
					{
						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if(netConductor){
							crPlayerDataEventPacket packet;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(2);
							stream->_writeUChar(shiliid);//1
							stream->_writeUChar(2);
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerChangeShili,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
						}	
					}
				}
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerChangeShiliMethod
//
/////////////////////////////////////////
crJXJPlayerChangeShiliMethod::crJXJPlayerChangeShiliMethod(){}
crJXJPlayerChangeShiliMethod::crJXJPlayerChangeShiliMethod(const crJXJPlayerChangeShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJPlayerChangeShiliMethod::inputParam(int i, void *param)
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

void crJXJPlayerChangeShiliMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strRadioGroup = str;
		break;
	}
}

void crJXJPlayerChangeShiliMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
	//void *param;
	if(canvas.valid()){
		ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_strRadioGroup));
		if(radioGroup.valid()){
			unsigned char shiliid = c_startShiliID + (unsigned char)radioGroup->getSelect();
		
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor){
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUChar(shiliid);//1
				stream->_writeUChar(1);//1
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerChangeShili,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvPlayerChangeShiliMethod
//
/////////////////////////////////////////
crJXJRecvPlayerChangeShiliMethod::crJXJRecvPlayerChangeShiliMethod():
	m_punishtime(0),
	m_coolingtime(0)
{}
crJXJRecvPlayerChangeShiliMethod::crJXJRecvPlayerChangeShiliMethod(const crJXJRecvPlayerChangeShiliMethod& handle):
	crMethod(handle),
	m_punishtime(handle.m_punishtime),
	m_coolingtime(handle.m_coolingtime)
{
}
void crJXJRecvPlayerChangeShiliMethod::inputParam(int i, void *param)
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

void crJXJRecvPlayerChangeShiliMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strJueSeCanvas = str;
		break;
	case 1:
		m_strCountry = str;
		break;
	case 2:
		m_strtaskCanvas = str;
		break;
	case 3:
		m_strSwitch = str;
		break;
	case 4:
		m_strWidget = str;
		break;
	case 5:
		m_strCountryCanvas = str;
		break;
	case 6:
		m_huodongcanvas = str;
		break;
	case 7:
		m_panguoCanvas = str;
		break;
	case 8:
		m_citywei = str;
		break;
	case 9:
		m_cityshu = str;
		break;
	case 10:
		m_citywu = str;
		break;
	case 11:
		m_punishtime = atoi(str.c_str());
		break;
	case 12:
		m_coolingtime = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJRecvPlayerChangeShiliMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		unsigned char shiliid = m_stream->_readUChar();
		unsigned char type = m_stream->_readUChar();
		char _case = 0;
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			bool isLoseCountry = false;
			bool isDestroy = false;
			
			playerData->excHandle(MAKEINT64(WCH_LockData,1));

			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myshiliid = *(unsigned char *)param;
			
			playerData->getParam(WCHDATA_JXJChengchiID,param);
			unsigned short * mychengchiID = (unsigned short *)param;

			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;

			playerData->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
			unsigned char *changeTimes = (unsigned char *)param;

			playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short changchiid = *(unsigned short *)param;

			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
			//cerr<<isLoseCountry<<endl;
			if(isLoseCountry && type == 3)
			{
				_case = 2;
				m_this->doEvent(WCH_JXJChangeShili,MAKEINT64(&shiliid,NULL));

				ref_ptr<crTableIO>shiliTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				crTableIO::StrVec record;
				if(shiliTab->queryOneRecord(0,crArgumentParser::appItoa(shiliid),record)>=0)
				{
					int chengchiindex = shiliTab->getTitleIndex("默认城池");
					*mychengchiID = (unsigned short)(atoi(record[chengchiindex].c_str()));
				}	
			}
			else
			{
				int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiliMaxCount,vipLevel).c_str());
				do 
				{
					//判断所选势力是否已灭亡
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isDestroy,&shiliid));
					if(isDestroy)
					{
						_case = -6;//所选择势力已经灭亡
						break;
					}
					//检查叛国次数
					if(changchiid != 0)
					{
						_case = -5;//在组队或者战场排队中
						break;
					}
					if(*changeTimes>=maxCount && type == 1)
					{
						_case = -1;//叛国次数已到
						break;
					}

					//std::string city;
					//bool flag = false;
					//playerData->getParam(WCHDATA_JXJChengChiMap,param);
					//ChengchiMap *map_city = (ChengchiMap *)param;
					//if (myshiliid == 10)
					//	city = m_citywei;
					//else if(myshiliid == 11)
					//	city = m_cityshu;
					//else
					//	city = m_citywu;

					//std::vector<string> strCity;
					//std::vector<int> index;
					//for (int i=0;i!=city.size();i++)
					//{
					//	if(city[i] == '|')
					//		index.push_back(i);
					//}
					//for (int i=0;i<=index.size();i++)
					//{
					//	if(i == 0)
					//		strCity.push_back(city.substr(0,index[0]));
					//	else if (i == index.size())
					//		strCity.push_back(city.substr(index[i-1]+1,city.size()-index[i-1]-1));
					//	else
					//		strCity.push_back(city.substr(index[i-1]+1,index[i]-index[i-1]-1));
					//}
					//for (int i=0;i!=strCity.size();i++)
					//{
					//	ChengchiMap::iterator itr = map_city->find(atoi(strCity[i].c_str()));
					//	if (itr != map_city->end())
					//		if (myshiliid != itr->second->getChengzhuShili())
					//		{
					//			flag = true;
					//			break;
					//		}
					//}
					//if (type==1 && flag)
					//{
					//	_case = -8;//建国之路城池被攻占
					//	break;
					//}

					int myid = m_this->getPlayerID();
					crData *serdata = crServerBrainHandle::getInstance()->getDataClass();
					crJXJShili *shili;
					crData *shiliData;
					serdata->getParam(WCHDATA_JXJShiliWei+myshiliid-c_startShiliID,param);
					shili = (crJXJShili *)param;
					unsigned char count = 0;
					int chengxiangid = 0;
					int dasimaid = 0;
					int dajiangjunid = 0;
					unsigned char mycountrypower = 1;
					unsigned char targetcountrypower = 1;
					if(shili && shili->getDataClass())
					{
						shiliData = shili->getDataClass();
						
						shiliData->getParam(WCHDATA_JXJChengxiang,param);
						chengxiangid = *(int*)param;
						shiliData->getParam(WCHDATA_JXJDasima,param);
						dasimaid = *(int*)param;
						shiliData->getParam(WCHDATA_JXJDajiangjun,param);
						dajiangjunid = *(int*)param;
						shiliData->getParam(WCHDATA_JXJCountryStrength,param);
						mycountrypower = *(unsigned char*)param;
					}

					serdata->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
					shili = (crJXJShili *)param;
					if(shili && shili->getDataClass())
					{
						shiliData = shili->getDataClass();
						shiliData->getParam(WCHDATA_JXJCountryStrength,param);
						targetcountrypower = *(unsigned char*)param;
					}

					if (mycountrypower == 0 && !isLoseCountry)
					{
						_case = -12;//弱国不能叛国
						break;
					}
					if (targetcountrypower == 3 && !isLoseCountry)
					{
						_case = -13;//不能叛向帝国
						break;
					}

					if (type == 1 && (myid==chengxiangid || myid==dasimaid || myid==dajiangjunid))
					{
						_case = -9;//官员不能叛国
						break;
					}

					playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
					int coolingtime = *(int*)param;
					if (type==1 && coolingtime > 0)
					{
						_case = -10;//叛国冷却期
						break;
					}

					time_t nowTime_t = time(0);
					struct  tm  nowTm = *localtime(&nowTime_t);
					if (type==1 && (nowTm.tm_wday==0 || nowTm.tm_wday==6))
					{
						_case = -11;//在选举日
						break;
					}

					crRole *mainRole = m_this->getMainRole();
					if(mainRole)
					{
						crData *mainroledata = mainRole->getDataClass();
						if(mainroledata)
						{
							mainroledata->getParam(WCHDATA_Level,param);
							unsigned char rolelv = *(unsigned char *)param;
							playerData-> getParam(WCHDATA_JXJConsortiaID,param);
							int mygroupid = *( int *)param;

							unsigned char needlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedRoleLv,vipLevel).c_str()));
							if(type == 1 && rolelv<needlv)
							{
								_case = -2;//等级不足
								break;
							}
							if(type == 1 && mygroupid!=0)
							{
								_case = -3;//在军团中
								break;
							}
								
							unsigned char needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedGold,vipLevel).c_str());
							playerData->getParam(WCHDATA_JXJGoldingot,param);
							int *goldingot = (int*)param;
							playerData->getParam(WCHDATA_JXJGiftGold,param);
							int *giftgold = (int*)param;

							if(type==1 && needgold> ((*goldingot)+ (*giftgold)))
							{
								_case = -4;//元宝不足
								break;
							}
							else
							{
								//if (type == 1)
								//{
								//	//保存选择的势力
								//	playerData->getParam(WCHDATA_JXJPlayerSelectedShiLi,param);
								//	unsigned char *selectedshili = (unsigned char *)param; 
								//	*selectedshili = shiliid;
								//	//进入缓冲期扣除元宝扣除叛国次数
								//	bool needdeductgold = false;
								//	MoneyChangeData moneydata(needgold,"主动叛国");
								//	m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
								//	*changeTimes+=1;
								//	//开启缓冲期
								//	playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
								//	int *buffertime = (int *)param; 
								//	*buffertime = 21600;
								//	//开启冷却期
								//	playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
								//	int *coolingtime = (int *)param; 
								//	*coolingtime = 3*24*60*60;
								//	_case = -7;
								//}
								//else if(type == 2)
								//{
									//叛国
									//开启惩罚期
									playerData->getParam(WCHDATA_JXJTreasonPunishTimer,param);
									int *punishtime = (int *)param; 
									*punishtime = m_punishtime;

									playerData->getParam(WCHDATA_JXJExtraProduceMap,param);
									ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
									unsigned char type = T_Panguo;
									ExtraProduceMap::iterator itr = extraProduceMap->find(type);
									if (itr != extraProduceMap->end())
									{
										itr->second->timer() = m_punishtime;
									}
									else
									{
										ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
										extraProduce->timer() = m_punishtime;
										extraProduceMap->insert(make_pair(type,extraProduce));
									}

									//开启冷却期
									playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
									int *coolingtime = (int *)param; 
									*coolingtime = m_coolingtime;

									if(!isLoseCountry)
									{
										bool needdeductgold = false;
										MoneyChangeData moneydata(needgold,"主动叛国");
										m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
										*changeTimes+=1;
									}

									////结束缓冲期
									//playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
									//int *buffertime = (int *)param; 
									//*buffertime = 0;

									//清除兵力
									playerData->getParam(WCHDATA_JXJTroopsMap,param);
									TroopsMap *troopsMap = (TroopsMap *)param;
									troopsMap->clear();

									m_this->doEvent(WCH_JXJChangeShili,MAKEINT64(&shiliid,NULL));
									_case = 1;
									ref_ptr<crTableIO>shiliTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
									crTableIO::StrVec record;
									if(shiliTab->queryOneRecord(0,crArgumentParser::appItoa(shiliid),record)>=0)
									{
										int chengchiindex = shiliTab->getTitleIndex("默认城池");
										*mychengchiID = (unsigned short)(atoi(record[chengchiindex].c_str()));
									}
								//}
								break;
							}	
						}
					}
					break;
				} while (0);
			}
			//
			playerData->excHandle(MAKEINT64(WCH_LockData,0));

			//playerData->getParam(WCHDATA_JXJCardPieces,param);
			//int curCardpieces = *(int *)param;

			//发包客户端
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if(_case == 1 || _case ==2)
				{
					stream->createBuf(5);
					stream->_writeChar(_case);//1
					stream->_writeUChar(shiliid);//1
					stream->_writeUShort(*mychengchiID);//2
					//stream->_writeInt(curCardpieces);//4
					stream->_writeUChar(*changeTimes);//1
				}
				else
				{
					stream->createBuf(3);
					stream->_writeChar(_case);
					stream->_writeUChar(shiliid);
					stream->_writeUChar(*changeTimes);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvPlayerChangeShili,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(m_netType == GameClient_Game)
	{
		//界面刷新
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strJueSeCanvas);
		ref_ptr<crMultiSwitch>  switch_country = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strCountry));
		ref_ptr<crCanvasNode> taskCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strtaskCanvas);
		
		char _case = m_stream->_readChar();
		unsigned char vipLevel = 0;
		if(_case == 1 || _case == 2)
		{
			unsigned char shiliid = m_stream->_readUChar();
			unsigned short chengchiid = m_stream->_readUShort();
			//int cardpiece = m_stream->_readInt();
			//unsigned char changeTimes = m_stream->_readUChar();
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if(myPlayer)
			{
				crData *playerData = myPlayer->getDataClass();
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				if(playerData && itemtab.valid())
				{
					if (_case == 1)
					{
						int punishtime = m_punishtime; 
						playerData->inputParam(WCHDATA_JXJTreasonPunishTimer,&punishtime);
						ref_ptr<crCanvasNode> punishcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
						if (punishcanvas.valid())
							crFilterRenderManager::getInstance()->showCanvas(punishcanvas.get(),true);

						/*int bufftime = 0; 
						playerData->inputParam(WCHDATA_JXJTreasonBufferTimer,&bufftime);
						ref_ptr<crCanvasNode> panguobuffcanvas = crFilterRenderManager::getInstance()->findCanvas(m_panguoCanvas);
						if(panguobuffcanvas.valid())
						crFilterRenderManager::getInstance()->showCanvas(panguobuffcanvas.get(),false);*/

						//清除兵力
						playerData->excHandle(MAKEINT64(WCH_LockData,1));
						playerData->getParam(WCHDATA_JXJTroopsMap,param);
						TroopsMap *troopsMap = (TroopsMap *)param;
						troopsMap->clear();

						//开启冷却期
						playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
						int *coolingtime = (int *)param; 
						*coolingtime = m_coolingtime;

						playerData->getParam(WCHDATA_JXJExtraProduceMap,param);
						ExtraProduceMap *extraProduceMap = (ExtraProduceMap *)param;
						unsigned char type = T_Panguo;
						ExtraProduceMap::iterator itr = extraProduceMap->find(type);
						if (itr != extraProduceMap->end())
						{
							itr->second->timer() = m_punishtime;
						}
						else
						{
							ref_ptr<crExtraProduce> extraProduce = new crExtraProduce;
							extraProduce->timer() = m_punishtime;
							extraProduceMap->insert(make_pair(type,extraProduce));
						}

						playerData->excHandle(MAKEINT64(WCH_LockData,0));

						
					}

					unsigned char changeTimes = m_stream->_readUChar();
					playerData->inputParam(WCHDATA_JXJPlayerChangeShiliTimes,&changeTimes);

					playerData->getParam(WCHDATA_JXJShiliID,param);
					unsigned char *myshiliid = (unsigned char *)param;
					playerData->inputParam(WCHDATA_JXJChengchiID,&chengchiid);
					if(switch_country.valid())
						switch_country->setActiveSwitchSet(shiliid-c_startShiliID);
					playerData->getParam(WCHDATA_JXJVipLv,param);
					vipLevel = *(unsigned char *)param;

					playerData->getParam(WCHDATA_JXJFriendList,param);
					FriendVec *myfriend = (FriendVec *)param;
					myfriend->clear();

					//playerData->getParam(WCHDATA_JXJCardPieces,param);
					//int *curCardpieces = (int *)param;

					//playerData->inputParam(WCHDATA_JXJPlayerChangeShiliTimes,&changeTimes);

					//int cardpiecesoffset = cardpiece - *curCardpieces;
					//playerData->inputParam(WCHDATA_JXJCardPiecesOffset,&cardpiecesoffset);
					//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_JiangHun,cardpiecesoffset));
					//*curCardpieces = cardpiece;
					//*curCardpieces = cardpiece;
					//清理将卡背包和编队将卡
					crTableIO::StrVec record;
					//int priceid = itemtab->getTitleIndex("回收价格");
					int cardshili = itemtab->getTitleIndex("国家");
					playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
					JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;
					//处理委任中的将卡
					playerData->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
					JXJAppointCardVec *appointcardvec = (JXJAppointCardVec *)param;

					//int coppergot = 0;
					int itemid= 0;
					unsigned char jkshili = 0;

					//委任中的将卡
					for (int appindex = 0; appindex<appointcardvec->size();appindex++)
					{
						if((*appointcardvec)[appindex]!=NULL)
						{
							itemid = (*appointcardvec)[appindex]->getItemID();
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
							{
								jkshili = (unsigned char)atoi(record[cardshili].c_str());
								if(jkshili == (*myshiliid))
								{
									(*appointcardvec)[appindex] = NULL;
								}
							}
						}
					}
					//将卡背包中的将卡

					for (int bagindex = 0; bagindex<jiangkabagvec->size();bagindex++)
					{
						if((*jiangkabagvec)[bagindex]!=NULL)
						{
							itemid = (*jiangkabagvec)[bagindex]->getItemID();
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
							{
								jkshili = (unsigned char)atoi(record[cardshili].c_str());
								if(jkshili == (*myshiliid))
								{
									(*jiangkabagvec)[bagindex] = NULL;
								}
							}
						}
					}
					//清理编队中将卡
					playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
					FormationInfoVec *troop_infors = (FormationInfoVec *)param;
					playerData->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
					SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;
					for (SavedFormationInfoVec::iterator itr_saved = savedtroop_infors->begin();\
						itr_saved!=savedtroop_infors->end();itr_saved++)
					{
						for (FormationInfoVec::iterator itr_troop = itr_saved->second.begin();\
							itr_troop!=itr_saved->second.end();itr_troop++)
						{
							for (int i = 0;i<2;i++)
							{
								itemid = (itr_troop->get()->getEquips())[i];
								if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
								{
									jkshili = (unsigned char)atoi(record[cardshili].c_str());
									if(jkshili == (*myshiliid))
									{
										(itr_troop->get()->getEquips())[i] = NULL;
									}
								}
							}
						}
					}

					for (FormationInfoVec::iterator itr_troop = troop_infors->begin();\
						itr_troop!=troop_infors->end();itr_troop++)
					{
						for (int i = 0;i<2;i++)
						{
							itemid = (itr_troop->get()->getEquips())[i];
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
							{
								jkshili = (unsigned char)atoi(record[cardshili].c_str());
								if(jkshili == (*myshiliid))
								{
									(itr_troop->get()->getEquips())[i] = NULL;
								}
							}
						}
					}

					//清除待征召将卡列表
					playerData->inputParam(WCHDATA_JXJLuckyCard,NULL);					

					//
					*myshiliid = shiliid;
// 					if(taskCanvas.valid())
// 					{
// 						ref_ptr<crMultiSwitch> btSwitch = dynamic_cast<crMultiSwitch *>(taskCanvas->getChildNode(m_strSwitch));
// 
// 						if(btSwitch.valid()) btSwitch->setActiveSwitchSet(1);
// 						ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(taskCanvas->getChildNode(m_strWidget));
// 						if(sequence.valid()) sequence->startEffect();
// 					}
				}
				ref_ptr<crCanvasNode> canvasCountry = crFilterRenderManager::getInstance()->findCanvas(m_strCountryCanvas);
				if(canvasCountry.valid() && canvasCountry->getVisiable() == true)
				{
					canvasCountry->doEvent(WCH_UI_UpdateData);
				}
			}
		}

		unsigned char needlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedRoleLv,vipLevel).c_str()));
		unsigned char needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedGold,vipLevel).c_str());

		int texid = -1;
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		int mode = 1;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		std::string param1;
		switch (_case)
		{
		case 1:
			//成功
			texid = 4000;
			break;
		case 2:
			//成功
			texid = 4000;
			break;
		case -1:
			texid = 4001;
			//次数过多
			break;
		case -2:
			texid = 4002;
			param1 = crArgumentParser::appItoa(needlv);
			noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
			//等级不足
			break;
		case -3:
			texid = 4003;
			//未退出军团
			break;
		case -4:
			texid = 4004;
			param1 = crArgumentParser::appItoa(needgold);
			noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
			//元宝不足
			break;
		case -5://在战场排队或者组队中
			texid = 4017;
			break;
		case -6:
			texid = 4018;
			break;
		case -7:
			{
				unsigned char selectedshili = m_stream->_readUChar();
				unsigned char changeTimes = m_stream->_readUChar();
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				if(myPlayer && myPlayer->getDataClass())
				{
					crData *playerData = myPlayer->getDataClass();
					playerData->inputParam(WCHDATA_JXJPlayerChangeShiliTimes,&changeTimes);

					playerData->inputParam(WCHDATA_JXJPlayerSelectedShiLi,&selectedshili);

					//开启缓冲期
					playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
					int *buffertime = (int *)param; 
					*buffertime = 21600;
				}

				/*ref_ptr<crCanvasNode> huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
				if (huodongcanvas.valid())
				{
				ref_ptr<crButtonWidgetNode> chengzhan = dynamic_cast<crButtonWidgetNode *>(huodongcanvas->getWidget(m_chenzhanbtn));
				if(chengzhan.valid())
				{
				chengzhan->setEnable(false);
				chengzhan->setVisiable(false);
				}
				}*/
				ref_ptr<crCanvasNode> panguobuffcanvas = crFilterRenderManager::getInstance()->findCanvas(m_panguoCanvas);
				if(panguobuffcanvas.valid())
					crFilterRenderManager::getInstance()->showCanvas(panguobuffcanvas.get(),true);
				texid = 9151;
			}
			break;
		case -8:
			texid = 9148;
			break;
		case -9:
			texid = 9149;
			break;
		case -10:
			texid = 9153;
			break;
		case -11:
			texid = 9154;
			break;
		case -12:
			texid = 9195;
			break;
		case -13:
			texid = 9196;
			break;
		default:
			texid = 4005;
			break;
		}	
		if(texid>0)
		{
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		}		
	}
}

/////////////////////////////////////////
//
//crJXJChangeShiliMethod
//
/////////////////////////////////////////
crJXJChangeShiliMethod::crJXJChangeShiliMethod(){}
crJXJChangeShiliMethod::crJXJChangeShiliMethod(const crJXJChangeShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJChangeShiliMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_shiliid = 0;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_shiliid = *(unsigned char *)(LOINT64(param64));
		}
		else
		{
			m_shiliid = 0;
		}
		break;
	}
}

void crJXJChangeShiliMethod::addParam(int i, const std::string& str)
{
}

void crJXJChangeShiliMethod::operator()(crHandle &handle)
{
	void *param;
	crData *playerData = m_this->getDataClass();
	int playerid = m_this->getPlayerID();
	crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(playerData && serverData && itemtab.valid())
	{
		serverData->getParam(WCHDATA_JXJShiliWei,param);
		crJXJShili *shiliWei = (crJXJShili *)param;
		serverData->getParam(WCHDATA_JXJShiliShu,param);
		crJXJShili *shiliShu = (crJXJShili *)param;
		serverData->getParam(WCHDATA_JXJShiliWu,param);
		crJXJShili *shiliWu = (crJXJShili *)param;
		int weiID = shiliWei->getID();
		int shuID = shiliShu->getID();
		int wuID = shiliWu->getID();
		//处理将卡
		int priceid = itemtab->getTitleIndex("回收价格");
		int cardshili = itemtab->getTitleIndex("国家");
		crTableIO::StrVec record;
		CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		//ref_ptr<crGameServerPlayerData> playerGameData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		float convertFactor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCardConvertFactor,vipLevel).c_str());

		playerData->getParam(WCHDATA_JXJShiliID,param);
		unsigned char * shiliid = (unsigned char *)param;

		//playerData->getParam(WCHDATA_JXJCardPieces,param);
		//int *curCardpieces = (int *)param;
		//处理委任中的将卡
		if(*shiliid!=m_shiliid && (m_shiliid == weiID || m_shiliid == shuID || m_shiliid == wuID))
		{
			//处理委任中的将卡
			playerData->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
			JXJAppointCardVec *appointcardvec = (JXJAppointCardVec *)param;
			
			playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec *jiangkabagvec = (JXJItemBagVec *)param;

			
			int coppergot = 0;
			int itemid= 0;
			unsigned char jkshili = 0;

			//处理委任中的将卡
			for (int appIndex = 0;appIndex<appointcardvec->size();appIndex++)
			{
				if((*appointcardvec)[appIndex]!=NULL)
				{
					itemid = (*appointcardvec)[appIndex]->getItemID();
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
					{
						jkshili = (unsigned char)atoi(record[cardshili].c_str());
						if(jkshili == (*shiliid))
						{
							coppergot += atoi(record[priceid].c_str());
							(*appointcardvec)[appIndex] = NULL;
						}
					}
				}
			}
			//处理将卡背包中的将卡
			for (int bagindex = 0; bagindex<jiangkabagvec->size();bagindex++)
			{
				if((*jiangkabagvec)[bagindex]!=NULL)
				{
					itemid = (*jiangkabagvec)[bagindex]->getItemID();
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
					{
						jkshili = (unsigned char)atoi(record[cardshili].c_str());
						if(jkshili == (*shiliid))
						{
							coppergot += atoi(record[priceid].c_str());
							(*jiangkabagvec)[bagindex] = NULL;
						}
					}
				}
			}

			playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
			FormationInfoVec *troop_infors = (FormationInfoVec *)param;
			playerData->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
			SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;
			for (SavedFormationInfoVec::iterator itr_saved = savedtroop_infors->begin();\
				itr_saved!=savedtroop_infors->end();itr_saved++)
			{
				for (FormationInfoVec::iterator itr_troop = itr_saved->second.begin();\
					itr_troop!=itr_saved->second.end();itr_troop++)
				{
					for (int i = 0;i<2;i++)
					{
						itemid = (itr_troop->get()->getEquips())[i];
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
						{
							jkshili = (unsigned char)atoi(record[cardshili].c_str());
							if(jkshili == (*shiliid))
							{
								coppergot += atoi(record[priceid].c_str());
								(itr_troop->get()->getEquips())[i] = NULL;
							}
						}
					}
				}
			}

			for (FormationInfoVec::iterator itr_troop = troop_infors->begin();\
				itr_troop!=troop_infors->end();itr_troop++)
			{
				for (int i = 0;i<2;i++)
				{
					itemid = (itr_troop->get()->getEquips())[i];
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
					{
						jkshili = (unsigned char)atoi(record[cardshili].c_str());
						if(jkshili == (*shiliid))
						{
							//coppergot += atoi(record[priceid].c_str());
							(itr_troop->get()->getEquips())[i] = NULL;
						}
					}
				}
			}
			if (coppergot > 0)
			{
				coppergot = (int)(convertFactor*(float)coppergot);	
				m_this->doEvent(WCH_JXJChangePlayerData,MAKEINT64(&coppergot,T_JiangHun));
				//INTLIMITADD(*curCardpieces,coppergot,INT_MAX);
			}
			//清理好友
			playerData->getParam(WCHDATA_JXJFriendList,param);
			FriendVec *friendVec = (FriendVec *)param;
			friendVec->clear();

			//转换势力

			//处理国民列表
			crData *shiliData = NULL;
			crData *newshiliData = NULL;
			if(*shiliid == weiID)
			{
				shiliData = shiliWei->getDataClass();
			}
			else if(*shiliid == shuID)
			{
				shiliData = shiliShu->getDataClass();
			}
			else if(*shiliid == wuID)
			{
				shiliData = shiliWu->getDataClass();
			}
			if(m_shiliid == weiID)
			{
				newshiliData = shiliWei->getDataClass();
			}
			else if(m_shiliid == shuID)
			{
				newshiliData = shiliShu->getDataClass();
			}
			else if(m_shiliid == wuID)
			{
				newshiliData = shiliWu->getDataClass();
			}
			if(shiliData && newshiliData)
			{
				//CRCore::ref_ptr<PeopleInfo > peopleInfo;
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				PeopleMap::iterator itrInfo = peopleMap->find(playerid);
				if(itrInfo!=peopleMap->end())
				{
					itrInfo->second->setShili(m_shiliid);
					itrInfo->second->setSamllAcc(false,false);
					newshiliData->excHandle(MAKEINT64(WCH_LockData,1));
					newshiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap *newpeopleMap = (PeopleMap *)param;
					if(newpeopleMap)
						newpeopleMap->insert(std::make_pair(playerid,itrInfo->second));
					newshiliData->excHandle(MAKEINT64(WCH_LockData,0));

					peopleMap->erase(playerid);
				}

				shiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
				ModifyPlayerSet * oldModifyPlayerSet = (ModifyPlayerSet *)param;
				shiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
				InsertPlayerSet * oldInsertPlayerSet = (InsertPlayerSet *)param;

				newshiliData->excHandle(MAKEINT64(WCH_LockData,1));
				newshiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
				ModifyPlayerSet * newModifyPlayerSet = (ModifyPlayerSet *)param;
				newshiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
				InsertPlayerSet * newInsertPlayerSet = (InsertPlayerSet *)param;

				if (oldModifyPlayerSet->find(playerid) != oldModifyPlayerSet->end())
				{
					oldModifyPlayerSet->erase(playerid);
					newModifyPlayerSet->insert(playerid);
				}
				if (oldInsertPlayerSet->find(playerid) != oldInsertPlayerSet->end())
				{
					oldInsertPlayerSet->erase(playerid);
					newInsertPlayerSet->insert(playerid);
				}
				newshiliData->excHandle(MAKEINT64(WCH_LockData,0));

				// 如果在投票列表中
				shiliData->getParam(WCHDATA_JXJCountryCandidateVecT,param);
				CandidateVec *candidateVec  = (CandidateVec *) param;
				CandidateVec::iterator cvItr = candidateVec->begin();
				for (; cvItr != candidateVec->end(); ++ cvItr)
				{
					if (cvItr->valid() && (*cvItr)->nPlayerID == playerid)
					{
						candidateVec->erase(cvItr);
						break;
					}
				}

				shiliData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
				CandidateVec*candidateVecL = (CandidateVec*)param;
				for (CandidateVec::iterator cvLItr = candidateVecL->begin();
					cvLItr != candidateVecL->end(); ++ cvLItr)
				{
					if (cvLItr->valid() && (*cvLItr)->nPlayerID == playerid)
					{
						char log[128];
						memset(log,0,sizeof(log));
						sprintf(log,"%d %s 叛国 从国家官员候补列表中删除\0",(*cvLItr)->nPlayerID,(*cvLItr)->playerName.c_str());

						std::string logdata(log);
						GameLogData gamelog(Log_RemoveFromCandidateL,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64((*cvLItr)->nPlayerID,&gamelog));

						candidateVecL->erase(cvLItr);
						break;
					}
				}
				////////////////小号列表清除
				shiliData->getParam(WCHDATA_JXJShiliBlacklistSet,param);
				ShiliBlacklistSet *blacklistSet = (ShiliBlacklistSet*)param;
				blacklistSet->erase(playerid);

				bool modify = true;
				shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			//清除待征召将卡列表
			playerData->inputParam(WCHDATA_JXJLuckyCard,NULL);
			//清除玩家官职
			time_t nowTime = 1;
			shiliData->getParam(WCHDATA_JXJChengxiang, param);
			int chengxiangID = *(int *)param;
			shiliData->getParam(WCHDATA_JXJDasima, param);
			int dasimaID = *(int *)param;
			shiliData->getParam(WCHDATA_JXJDajiangjun, param);
			int dajiangjunID = *(int *)param;
			if (playerid == chengxiangID)
			{
				shiliData->inputParam(WCHDATA_JXJChengxiang, NULL);
				shiliData->inputParam(WCHDATA_JXJChengxiangName, NULL);
				shiliData->inputParam(WCHDATA_JXJChengxiangLastLogin, &nowTime);
			}
			else if (playerid == dasimaID)
			{
				shiliData->inputParam(WCHDATA_JXJDasima, NULL);
				shiliData->inputParam(WCHDATA_JXJDasimaName, NULL);
				shiliData->inputParam(WCHDATA_JXJDasimaLastLogin, &nowTime);
			}
			else if (playerid == dajiangjunID)
			{
				shiliData->inputParam(WCHDATA_JXJDajiangjun, NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjunName, NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjunLastLogin, &nowTime);
			}

			*shiliid = m_shiliid;
			ref_ptr<crStreamBuf> shiliStream = new crStreamBuf;
			newshiliData->excHandle(MAKEINT64(WCH_BuildPlayerStream,shiliStream.get()));

			ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid)); 
			if(serverPlayerData.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJSYNShiliData,shiliStream.get());
				gameServer->getNetManager()->sendPacket(serverPlayerData ->getPlayerConnectServerAddress(),packet);
			}
		}
		else
		{
			//势力有问题
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));


	}
}

/////////////////////////////////////////
//
//crJXJCheckCountryLoseMethod
//
/////////////////////////////////////////
crJXJCheckCountryLoseMethod::crJXJCheckCountryLoseMethod():m_bLoseCountry(NULL){}
crJXJCheckCountryLoseMethod::crJXJCheckCountryLoseMethod(const crJXJCheckCountryLoseMethod& handle):
	crMethod(handle),
	m_bLoseCountry(handle.m_bLoseCountry)
{
}
void crJXJCheckCountryLoseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_bLoseCountry = NULL;
			m_shiliid = 0;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_bLoseCountry = (bool *)(LOINT64(param64));
			m_shiliid = *(unsigned char *)(HIINT64(param64));
		}
		else
		{
			m_bLoseCountry = NULL;
			m_shiliid = 0;
		}
		break;
	}
}

void crJXJCheckCountryLoseMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckCountryLoseMethod::operator()(crHandle &handle)
{
	void *param;
	*m_bLoseCountry = false;
	char _case = 0;//0.未灭国，1，国都失陷，2，陪都失陷
	crData *brainData = m_this->getDataClass();
	if(brainData)
	{
		ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		brainData->excHandle(MAKEINT64(WCH_LockData,1));
		brainData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;
		
		//查询国都陪都
		if(playershilitab.valid())
		{
			crTableIO::StrVec record;
			int guoduIndex = playershilitab->getTitleIndex("国都");
			int peiduIndex = playershilitab->getTitleIndex("陪都");

			if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(m_shiliid),record)>=0 && guoduIndex>=0 && peiduIndex>=0)
			{
				unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
				unsigned short peiduid = (unsigned short)(atoi(record[peiduIndex].c_str()));

				ChengchiMap::iterator itr_info = chengchimap->begin();
				for (;itr_info!=chengchimap->end() && _case<2;itr_info++)
				{
					if((itr_info->first == guoduid || itr_info->first == peiduid) && 
						itr_info->second->getChengzhuShili() != m_shiliid)
					{
						_case++;
					}
				}

				if(_case>=2)
				{
					*m_bLoseCountry = true;
				}
			}
		}
				
		brainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJClientCheckCountryLoseMethod
//
/////////////////////////////////////////
crJXJClientCheckCountryLoseMethod::crJXJClientCheckCountryLoseMethod():m_bLoseCountry(NULL){}
crJXJClientCheckCountryLoseMethod::crJXJClientCheckCountryLoseMethod(const crJXJClientCheckCountryLoseMethod& handle):
	crMethod(handle),
	m_bLoseCountry(handle.m_bLoseCountry)
{
}
void crJXJClientCheckCountryLoseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_bLoseCountry = NULL;
			m_shiliid = 0;
		}
		break;
	case 1:
		m_this = (crPlayerGameData *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_bLoseCountry = (bool *)(LOINT64(param64));
			m_shiliid = *(unsigned char *)(HIINT64(param64));
		}
		else
		{
			m_bLoseCountry = NULL;
			m_shiliid = 0;
		}
		break;
	}
}

void crJXJClientCheckCountryLoseMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientCheckCountryLoseMethod::operator()(crHandle &handle)
{
	void *param;

	char _case = 0;//0.未灭国，1，国都失陷，2，陪都失陷
	crData *playerData = m_this->getDataClass();
	if(playerData)
	{
		ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;

		//查询国都陪都
		if(playershilitab.valid())
		{
			crTableIO::StrVec record;
			int guoduIndex = playershilitab->getTitleIndex("国都");
			int peiduIndex = playershilitab->getTitleIndex("陪都");

			if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(m_shiliid),record)>=0 && guoduIndex>=0 && peiduIndex>=0)
			{
				unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
				unsigned short peiduid = (unsigned short)(atoi(record[peiduIndex].c_str()));

				ChengchiMap::iterator itr_info = chengchimap->begin();
				for (;itr_info!=chengchimap->end() && _case<2;itr_info++)
				{
					if((itr_info->first == guoduid || itr_info->first == peiduid) && 
						itr_info->second->getChengzhuShili() != m_shiliid)
					{
						_case++;
					}
				}

				if(_case>=2)
				{
					*m_bLoseCountry = true;
				}
			}
		}

		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJIsMyCountryLoseMethod
//
/////////////////////////////////////////
crJXJIsMyCountryLoseMethod::crJXJIsMyCountryLoseMethod(){}
crJXJIsMyCountryLoseMethod::crJXJIsMyCountryLoseMethod(const crJXJIsMyCountryLoseMethod& handle):
	crMethod(handle)
{
}
void crJXJIsMyCountryLoseMethod::inputParam(int i, void *param)
{
}

void crJXJIsMyCountryLoseMethod::addParam(int i, const std::string& str)
{
}

void crJXJIsMyCountryLoseMethod::operator()(crHandle &handle)
{
	void *param;
	bool isLoseCountry = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *playerData = myPlayer->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;

			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
		}

	}

	handle.outputParam(0,&isLoseCountry);
}
/////////////////////////////////////////
//
//crJXJClientShowChangeShiliCanvasMethod
//
/////////////////////////////////////////
crJXJClientShowChangeShiliCanvasMethod::crJXJClientShowChangeShiliCanvasMethod(){}
crJXJClientShowChangeShiliCanvasMethod::crJXJClientShowChangeShiliCanvasMethod(const crJXJClientShowChangeShiliCanvasMethod& handle):
	crMethod(handle)
{
}
void crJXJClientShowChangeShiliCanvasMethod::inputParam(int i, void *param)
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
		m_this = (crBrain*)param;
		break;
	}
}

void crJXJClientShowChangeShiliCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJClientShowChangeShiliCanvasMethod::operator()(crHandle &handle)
{
	//判断玩家是否灭国
	/************************************************************************/
	/* 读取chengchimap判断玩家所在势力国都陪都是否沦陷                      */
	/************************************************************************/
	void *param;
	bool isLoseCountry = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *playerData = myPlayer->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;

			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
		}
		
	}
	//如果灭国就提示叛国
	if(isLoseCountry)
	{
		crFilterRenderManager::getInstance()->doModal(m_strCanvas);
	}
}

/////////////////////////////////////////
//
//crJXJRecvGetLandRewardMethod
//
/////////////////////////////////////////
crJXJRecvGetLandRewardMethod::crJXJRecvGetLandRewardMethod(){}
crJXJRecvGetLandRewardMethod::crJXJRecvGetLandRewardMethod(const crJXJRecvGetLandRewardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGetLandRewardMethod::inputParam(int i, void *param)
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

void crJXJRecvGetLandRewardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strSwitch = str;
		break;
	default:
		break;
	}
}

void crJXJRecvGetLandRewardMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		/************************************************************************/
		/* 1.随机物品
		/* 2.随机倍率（如果随机倍率大于物品最大允许倍率，按最小来）
		 * 3.随机物品放入存临时数据动画播放完后发放
		/************************************************************************/

		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJLandRewardGetCount,param);
			unsigned char * lanRewardCount = (unsigned char  *)param;

			CRCore::ref_ptr<crPlayerRewardData> rewardData = new crPlayerRewardData;
			rewardData->setPlayerID(m_this->getPlayerID());
			rewardData->setType(GP_DailyLandReward);
			if(*lanRewardCount > 0)
			{
				//随机物品
				crTableIO::DataVec recordVec,mulitRecordVec;
				crTableIO::StrVec itemrecord;
				ref_ptr<crTableIO>landDropTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLandRewardTab);
				ref_ptr<crTableIO>landMultiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLandRewardMultiTab);
				ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				if(landDropTab.valid() && landMultiTab.valid()){
					landDropTab->queryRecords(0,crArgumentParser::appItoa(1),recordVec);
					landMultiTab->queryRecords(0,crArgumentParser::appItoa(1),mulitRecordVec);
				}
				if(!recordVec.empty() && !mulitRecordVec.empty())
				{
					int dropcountid = landDropTab->getTitleIndex("掉落种数");
					int droptypeid = landDropTab->getTitleIndex("掉落类型");
					int dropidindex = landDropTab->getTitleIndex("dropid");
					int oddsid = landDropTab->getTitleIndex("掉落几率");
					int countid = landDropTab->getTitleIndex("数量");
					int dropcount = atoi(recordVec[0][dropcountid].c_str());
					int itemMulitIndex = itemTab->getTitleIndex("最高倍率");
					int multiIndex = landMultiTab->getTitleIndex("倍率");
					int multioddsIndex = landMultiTab->getTitleIndex("几率");
					//检查背包空格数量
					playerData->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
					bool hasspace = false;
					int packetRemainSize = 0;
					for( JXJItemBagVec::iterator itr = itemBagVec->begin();
						itr != itemBagVec->end();
						++itr )
					{
						if(!(itr->valid()))
						{
							packetRemainSize++;
							if(packetRemainSize>=dropcount)
							{
								hasspace = true;
								break;
							}
						}
					}
					if(hasspace)
					{
						int odds = 0;
						int multiodds = 0;
						unsigned char multirate = 1;
						//bool isdroped;
						unsigned char droptype;
						int count;
						crVector2i countrange;
						CRCore::rangei counrnd;
						RewardItemVec rewarditemvec;
						ref_ptr<crBagItemData> itemdata;
						std::vector<std::pair<int,int> > DropedResVec;//resid,count
						std::vector<std::pair<unsigned short,int> > DropedEquipVec;//equipid,count

						//crRole *mainRole = m_this->getMainRole();

						int recordCount = recordVec.size();
						std::vector< std::pair<int,int> >DroprndVec,multiRndVec;
						int i,j,k;
						int dropodds;
						for (i = 0; i < recordCount; i++)
						{
							dropodds = atoi(recordVec[i][oddsid].c_str());
							odds += dropodds;
							DroprndVec.push_back(std::make_pair(odds,dropodds));
						}
						for (i = 0; i < mulitRecordVec.size(); i++)
						{
							dropodds = atoi(mulitRecordVec[i][multioddsIndex].c_str());
							multiodds += dropodds;
							multiRndVec.push_back(std::make_pair(multiodds,dropodds));
						}
						crVector2i vec2;
						rangei rndi(0,odds);
						rangei rndi2(0,multiodds);
						int r;
						r = rndi2.get_random();
						for(j = 0; j<mulitRecordVec.size(); j++)
						{
							if(r<=multiRndVec[j].first)
							{//该倍率掉出
								multirate = (unsigned char)atoi(mulitRecordVec[j][multiIndex].c_str());
								break;
							}
						}
						for(i = 0; i<dropcount;i++)
						{
							r = rndi.get_random();
							for(j = 0; j<recordCount; j++)
							{
								if(r<=DroprndVec[j].first)
								{//该物品掉出
									DroprndVec[j].first = -1;
									odds -= DroprndVec[j].second;
									rndi.set(0,odds);
									for (k = j+1; k < recordCount; k++)
									{
										DroprndVec[k].first -= DroprndVec[j].second;
									}
									droptype = atoi(recordVec[j][droptypeid].c_str());

									crArgumentParser::appAtoVec(recordVec[j][countid],countrange);
									counrnd.set(countrange[0],countrange[1]);
									count = counrnd.get_random();
									if(count>0)
									{
										droptype = atoi(recordVec[j][droptypeid].c_str());
										switch (droptype)
										{
										case T_Food:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJFood,count));
											break;
										case T_Wood:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJWood,count));
											break;
										case T_Iron:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJIron,count));
											break;
										case T_Horse:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJHorse,count));
											break;
										case T_Copper:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJCoppercash,count));
											break;
										case T_Exp:
											DropedResVec.push_back(std::make_pair(WCHDATA_Experience,count));
											break;
										case T_Achievement:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJAchievement,count));
											break;
										case T_Giftgold:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJGiftGold,count));
											break;
										case T_TroopsEquip:
											{
												DropedEquipVec.push_back(std::make_pair(atoi(recordVec[j][dropidindex].c_str()),count));
											}
											break;
										case T_Item:
											{
												crArgumentParser::appAtoVec(recordVec[j][dropidindex],vec2);

												if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(vec2[0]),itemrecord)>=0)
												{
													char maxMulit = (char)atoi(itemrecord[itemMulitIndex].c_str());
													if(multirate > maxMulit)
														multirate = maxMulit;
													if(multirate <= 0)
														multirate = 1;
												}

												itemdata = new crBagItemData;
												itemdata->setItemID(vec2[0]);
												itemdata->setEquipMagic(vec2[1]);
												itemdata->setItemCount(count);
												rewarditemvec.push_back(itemdata);												
											}
											break;
										case T_Contribute:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJCountryContribute,count));
											break;
										case T_Exploit:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJExploit,count));
											break;
										case T_GuoZhanJiFen:
											DropedResVec.push_back(std::make_pair(WCHDATA_JXJGuoZhanJiFen,count));
											break;
										}
									}
									break;
								}
							}
						}
						
						//可领取次数-1
						*lanRewardCount-=1;
						///游戏日志
						std::string logdata = "登陆抽奖" + crArgumentParser::appItoa(*lanRewardCount);
						GameLogData gamelog(Log_LandReward,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(m_this->getPlayerID(),&gamelog));
						//服务器端缓存奖励信息，客户端动画播放完毕发放
						std::vector<std::pair<int,int> >::iterator itr_res = DropedResVec.begin();
						for (;itr_res!=DropedResVec.end();itr_res++)
						{
							switch (itr_res->first)
							{
							case WCHDATA_JXJFood:
								rewardData->setFood(itr_res->second * multirate);
								break;
							case WCHDATA_JXJWood:
								rewardData->setWood(itr_res->second * multirate);
								break;
							case WCHDATA_JXJIron:
								rewardData->setIron(itr_res->second * multirate);
								break;
							case WCHDATA_JXJHorse:
								rewardData->setHorse(itr_res->second * multirate);
								break;
							case WCHDATA_JXJCoppercash:
								rewardData->setCoopercash(itr_res->second * multirate);
								break;
							case WCHDATA_Experience:
								rewardData->setExperience(itr_res->second * multirate);
								break;
							case WCHDATA_JXJAchievement:
								rewardData->setAchievement(itr_res->second * multirate);
								break;
							case WCHDATA_JXJGiftGold:
								rewardData->setGiftgold(itr_res->second * multirate);
								break;
							case WCHDATA_JXJCountryContribute:
								rewardData->setCountryContribution(itr_res->second * multirate);
								break;
							case WCHDATA_JXJExploit:
								rewardData->setExploit(itr_res->second * multirate);
								break;
							}
						}
						RewardItemVec::iterator itr_item = rewarditemvec.begin();
						for (;itr_item!=rewarditemvec.end();itr_item++)
						{
							itr_item->get()->setItemCount(itr_item->get()->getItemCount()*multirate);
							rewardData->getRewardItemsVec().push_back(itr_item->get());
						}
						std::vector<std::pair<unsigned short,int> > ::iterator itr_equip = DropedEquipVec.begin();
						for (;itr_equip!=DropedEquipVec.end();itr_equip++)
						{
							itr_equip->second*=multirate;	
							rewardData->getEquipsVec().push_back(std::make_pair(itr_equip->first,itr_equip->second));
						}
						playerData->inputParam(WCHDATA_JXJLandRewardDataInfo,rewardData.get());
						//发送到客户端
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(5+DropedResVec.size()*8+rewarditemvec.size()*5+DropedEquipVec.size()*6);

						stream->_writeUChar(*lanRewardCount);
						stream->_writeUChar(multirate);//1

						stream->_writeChar(DropedResVec.size());//1
						itr_res = DropedResVec.begin();
						for (;itr_res!=DropedResVec.end();itr_res++)
						{
							stream->_writeInt(itr_res->first);//4
							stream->_writeInt(itr_res->second);//4
						}
						stream->_writeChar(rewarditemvec.size());//1
						itr_item = rewarditemvec.begin();
						for (;itr_item!=rewarditemvec.end();itr_item++)
						{
							stream->_writeInt(itr_item->get()->getItemID());//4
							stream->_writeUChar(itr_item->get()->getItemCount());//1
						}
						stream->_writeChar(DropedEquipVec.size());//1
						itr_equip = DropedEquipVec.begin();
						for (;itr_equip!=DropedEquipVec.end();itr_equip++)
						{
							stream->_writeUShort(itr_equip->first);//2
							stream->_writeInt(itr_equip->second);//4
						}

						crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
						crNetDataManager *netDataManager = gameServer->getNetDataManager();
						ref_ptr<crGameServerPlayerData> playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
						if(playerGameServerPlayerData.valid())
						{
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvGetLandReward,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(), packet);
							m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_DailyLand, 1));
						}
					}
				}
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
	else if(m_netType == GameClient_Game)
	{
		ref_ptr<crCanvasNode>   landRewardCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if(landRewardCanvas.valid())
		{
			bool isHas = true;
			crData *canvasData = landRewardCanvas->getDataClass();
			
			canvasData->getParam(WCHDATA_JXJLandRewardMap,param);
			LandRewardMap *landRewardMap = (LandRewardMap *)param;

			//存入播放抽奖动画相关data；
			//LandRewardMap rewardMap;//类型,id

			unsigned char canGetCount = m_stream->_readUChar();
			unsigned char mulitRate = m_stream->_readUChar();

			char resSize = m_stream->_readChar();
			for (int i = 0;i<resSize;i++)
			{
				int resType = m_stream->_readInt();
				int count = m_stream->_readInt();
				switch (resType)
				{
				case WCHDATA_JXJFood:
					landRewardMap->insert(std::make_pair(T_Food,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJWood:
					landRewardMap->insert(std::make_pair(T_Wood,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJIron:
					landRewardMap->insert(std::make_pair(T_Iron,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJHorse:
					landRewardMap->insert(std::make_pair(T_Horse,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJCoppercash:
					landRewardMap->insert(std::make_pair(T_Copper,std::make_pair(0,count)));
					break;
				case WCHDATA_Experience:
					landRewardMap->insert(std::make_pair(T_Exp,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJAchievement:
					landRewardMap->insert(std::make_pair(T_Achievement,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJGiftGold:
					landRewardMap->insert(std::make_pair(T_Giftgold,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJCountryContribute:
					landRewardMap->insert(std::make_pair(T_Contribute,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJExploit:
					landRewardMap->insert(std::make_pair(T_Exploit,std::make_pair(0,count)));
					break;
				case WCHDATA_JXJGuoZhanJiFen:
					landRewardMap->insert(std::make_pair(T_GuoZhanJiFen,std::make_pair(0,count)));
					break;
				}
			}

			char itemSize = m_stream->_readChar();
			for (int i = 0;i<itemSize;i++)
			{
				int itemid = m_stream->_readInt();
				unsigned char count = m_stream->_readUChar();
				landRewardMap->insert(std::make_pair(T_Item,std::make_pair(itemid,count)));
			}

			char equipSize = m_stream->_readChar();
			for (int i = 0;i<equipSize;i++)
			{
				unsigned short equipid = m_stream->_readUShort();
				int count = m_stream->_readInt();
				landRewardMap->insert(std::make_pair(T_TroopsEquip,std::make_pair(equipid,count)));
			}

			canvasData->inputParam(WCHDATA_JXJLandRewardMulti,&mulitRate);
			canvasData->inputParam(WCHDATA_JXJClientHasLandReward,&isHas);
			crData *playerData = m_this->getDataClass();

			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				int landReardTime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLandRewardMaxTime,vipLevel).c_str());
				playerData->inputParam(WCHDATA_JXJLandRewardClientTime,&landReardTime);
				playerData->inputParam(WCHDATA_JXJLandRewardGetCount,&canGetCount);
			}
			ref_ptr<crMultiSwitch>  multiSwitch = dynamic_cast<crMultiSwitch *>(landRewardCanvas->getChildNode(m_strSwitch));
			if(multiSwitch.valid())
				multiSwitch->setActiveSwitchSet(canGetCount);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvSendLandRewardToPlayerMethod
//
/////////////////////////////////////////
crJXJRecvSendLandRewardToPlayerMethod::crJXJRecvSendLandRewardToPlayerMethod(){}
crJXJRecvSendLandRewardToPlayerMethod::crJXJRecvSendLandRewardToPlayerMethod(const crJXJRecvSendLandRewardToPlayerMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvSendLandRewardToPlayerMethod::inputParam(int i, void *param)
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

void crJXJRecvSendLandRewardToPlayerMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvSendLandRewardToPlayerMethod::operator()(crHandle &handle)
{
	if(m_netType == GameServer)
	{
		//如果玩家没有等到播放完动画直接退出也发放
		m_this->doEvent(WCH_JXJLandRewardDropClear);
// 		void *param;
// 		crData *playerData = m_this->getDataClass();
// 		if(playerData)
// 		{
// 			playerData->excHandle(MAKEINT64(WCH_LockData,1));
// 			playerData->getParam(WCHDATA_JXJLandRewardDataInfo,param);
// 			CRCore::ref_ptr<crPlayerRewardData> reward = (crPlayerRewardData *)param;
// 
// 			if(!reward.valid())
// 			{
// 				m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward.get(),NULL));
// 
// 				playerData->inputParam(WCHDATA_JXJLandRewardDataInfo,NULL);//清除奖励信息
// 			}
// 			playerData->excHandle(MAKEINT64(WCH_LockData,0));
// 		}
	}
}

/////////////////////////////////////////
//
//crJXJLandRewardDropClearMethod
//
/////////////////////////////////////////
crJXJLandRewardDropClearMethod::crJXJLandRewardDropClearMethod(){}
crJXJLandRewardDropClearMethod::crJXJLandRewardDropClearMethod(const crJXJLandRewardDropClearMethod& handle):
	crMethod(handle)
{
}
void crJXJLandRewardDropClearMethod::inputParam(int i, void *param)
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

void crJXJLandRewardDropClearMethod::addParam(int i, const std::string& str)
{
}

void crJXJLandRewardDropClearMethod::operator()(crHandle &handle)
{
	//如果玩家没有等到播放完动画直接退出也发放
	void *param;
	crData *playerData = m_this->getDataClass();
	if(playerData)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));

		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJLandRewardDataInfo,param);
		CRCore::ref_ptr<crPlayerRewardData> reward = (crPlayerRewardData *)param;

		if(reward.valid())
		{
			if(gsplayerData.valid())
			{
				m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward.get(),NULL));
			}
			else
			{
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJCreateRewardMail,MAKEINT64(reward.get(),NULL));
			}
			playerData->inputParam(WCHDATA_JXJLandRewardDataInfo,NULL);//清除奖励信息
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJLandRewardCanvasShowMethod
//
/////////////////////////////////////////
crJXJLandRewardCanvasShowMethod::crJXJLandRewardCanvasShowMethod(){}
crJXJLandRewardCanvasShowMethod::crJXJLandRewardCanvasShowMethod(const crJXJLandRewardCanvasShowMethod& handle):
	crMethod(handle)
{
}
void crJXJLandRewardCanvasShowMethod::inputParam(int i, void *param)
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

void crJXJLandRewardCanvasShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strTitle = str;
		break;
	case 1:
		m_strSeq[0] = str;
		break;
	case 2:
		m_strSeq[1] = str;
		break;
	case 3:
		m_strSeq[2] = str;
		break;
	case 4:
		m_strImage_1 = str;
		break;
	case 5:
		m_strImage_2 = str;
		break;
	case 6:
		m_strMultiImage = str;
		break;
	case 7:
		m_strStopSw[0] = str;
		break;
	case 8:
		m_strStopSw[1] = str;
		break;
	case 9:
		m_strStopSw[2] = str;
		break;
	case 10:
		m_strName[0] = str;
		break;
	case 11:
		m_strName[1] = str;
		break;
	case 12:
		m_strName[2] = str;
		break;
	case 13:
		m_strNumber[0] = str;
		break;
	case 14:
		m_strNumber[1] = str;
		break;
	case 15:
		m_strNumber[2] = str;
		break;
	case 16:
		m_strSwitch = str;
		break;
	case 17:
		m_strButton = str;
		break;
	}
}

void crJXJLandRewardCanvasShowMethod::operator()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	bool bHasReward = false;
	if(canvasData)
	{
		canvasData->getParam(WCHDATA_JXJClientHasLandReward,param);
		bHasReward = *(bool *)param;
	}
	
	if(!bHasReward)
	{
		crSequence * seq[3] = {NULL};
		ref_ptr<crStaticTextWidgetNode>  name[3] = {NULL};
		ref_ptr<crStaticTextWidgetNode>  number[3] = {NULL};
		ref_ptr<crMultiSwitch>  stopSwitch[3] = {NULL};
		ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strTitle));
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();


		for (int i = 0;i<3;i++)
		{
			seq[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_strSeq[i]));
			if(seq[i]){
				seq[i]->setVanishWhenStop(true);
				seq[i]->setMode(crSequence::STOP);
			}

			name[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strName[i]));
			number[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strNumber[i]));

			if(name[i]!=NULL){
				name[i]->clearString();
			}
			if(number[i]!=NULL){
				number[i]->clearString();
			}

			stopSwitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strStopSw[i]));
			if(stopSwitch[i].valid()){
				stopSwitch[i]->setActiveSwitchSet(0);
			}
		}

		ref_ptr<crButtonWidgetNode>  btConfirm = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strButton));
		ref_ptr<crMultiSwitch>   multiSwitchNum = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch));
		if(myPlayer){
			crData *playerData = myPlayer->getDataClass();
			if(playerData){
				playerData->getParam(WCHDATA_JXJLandRewardGetCount,param);
				unsigned char * lanGetCount = (unsigned char *)param;

				if(*lanGetCount > 0){
					if(btConfirm.valid())
						btConfirm->setEnable(true);
					if(multiSwitchNum.valid())
						multiSwitchNum->setActiveSwitchSet(*lanGetCount);
				}else{
					if(btConfirm.valid())
						btConfirm->setEnable(false);
					if(multiSwitchNum.valid())
						multiSwitchNum->setActiveSwitchSet(0);
				}
			}
		}

		ref_ptr<crImageBoxWidgetNode>  imageBox_1 = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strImage_1));
		ref_ptr<crImageBoxWidgetNode>  imageBox_2 = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strImage_2));
		ref_ptr<crMultiSwitch>  imageSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strMultiImage));
		if(imageBox_1.valid()) 
		{
			imageBox_1->setVisiable(false);
		}
		if(imageBox_2.valid()) 
		{
			imageBox_2->setVisiable(false);
		}
		if(imageSwitch.valid()) 
			imageSwitch->setActiveSwitchSet(4);
	}
}

/////////////////////////////////////////
//
//crJXJClientRequestLandRewardMethod
//
/////////////////////////////////////////
crJXJClientRequestLandRewardMethod::crJXJClientRequestLandRewardMethod(){}
crJXJClientRequestLandRewardMethod::crJXJClientRequestLandRewardMethod(const crJXJClientRequestLandRewardMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRequestLandRewardMethod::inputParam(int i, void *param)
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

void crJXJClientRequestLandRewardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
		m_strSeq[i] = str;
		break;
	case 3:
	case 4:
	case 5:
		m_strStopSw[i-3] = str;
		break;
	case 6:
	case 7:
	case 8:
		m_strName[i-6] = str;
		break;
	case 9:
	case 10:
	case 11:
		m_strNumber[i-9]=str;
		break;
	case 12:
		m_strSwitch = str;
		break;
	}
}

void crJXJClientRequestLandRewardMethod::operator()(crHandle &handle)
{
	//第一个动画开始播放
	void *param;
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
		bool hasspace = false;
		int packetRemainSize = 0;
		for( JXJItemBagVec::iterator itr = itemBagVec->begin();
			itr != itemBagVec->end();
			++itr )
		{
			if(!(itr->valid()))
			{
				packetRemainSize++;
				if(packetRemainSize>=2)
				{
					hasspace = true;
					break;
				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));

		if(hasspace)
		{
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			if(canvas.valid()){
				int repeatCount = 2;
				rangei rndi(1,2);
				crSequence *seq[3] = {NULL};
				ref_ptr<crMultiSwitch>  multiSwitch[3] = {NULL};
				ref_ptr<crStaticTextWidgetNode>  textName[3] = {NULL};
				ref_ptr<crStaticTextWidgetNode>  textNumber[3] = {NULL};
				ref_ptr<crMultiSwitch> mulitNewSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strSwitch));
				if(mulitNewSwitch.valid())
					mulitNewSwitch->setActiveSwitchSet(4);
				for (int i = 0;i<3;i++)
				{
					seq[i] = dynamic_cast<crSequence *>(canvas->getChildNode(m_strSeq[i]));
					multiSwitch[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strStopSw[i]));
					textName[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strName[i]));
					textNumber[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strNumber[i]));
					if(seq[i]){
						seq[i]->setVisiable(true);
						seq[i]->setMode(crSequence::START);
						seq[i]->setDuration(0.01,repeatCount);
						repeatCount += rndi.get_random();
					}
					if(multiSwitch[i].valid())
					{
						multiSwitch[i]->setActiveSwitchSet(0);
					}
					if(textName[i].valid()){
						textName[i]->clearString();
					}
					if(textNumber[i].valid()){
						textNumber[i]->clearString();
					}

				}
			}
			//
			m_this->setEnable(false);
			//提交抽奖到服务器
			//
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor){
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGetLandReward,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
		else
		{
			//请先整理背包
			crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2008,NULL));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUILandRewardUpdateMethod
//
/////////////////////////////////////////
crJXJUILandRewardUpdateMethod::crJXJUILandRewardUpdateMethod(){}
crJXJUILandRewardUpdateMethod::crJXJUILandRewardUpdateMethod(const crJXJUILandRewardUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJUILandRewardUpdateMethod::inputParam(int i, void *param)
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

void crJXJUILandRewardUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSeq[0] = str;
		break;
	case 1:
		m_strSeq[1] = str;
		break;
	case 2:
		m_strSeq[2] = str;
		break;
	case 3:
		m_strImage_1 = str;
		break;
	case 4:
		m_strImage_2 = str;
		break;
	case 5:
		m_strMultiImage = str;
		break;
	case 6:
		m_strStopSw[0] = str;
		break;
	case 7:
		m_strStopSw[1] = str;
		break;
	case 8:
		m_strStopSw[2] = str;
		break;
	case 9:
		m_strName[0] = str;
		break;
	case 10:
		m_strName[1] = str;
		break;
	case 11:
		m_strName[2] = str;
		break;
	case 12:
		m_strNumber[0] = str;
		break;
	case 13:
		m_strNumber[1] = str;
		break;
	case 14:
		m_strNumber[2] = str;
		break;
	case 15: 
		m_strFoodImage = str;//粮草
		break;
	case 16:
		m_strWoodImage = str;//木材
		break;
	case 17:
		m_strIronImage = str;//铁矿
		break;
	case 18: 
		m_strHorseImage = str;//马匹
		break;
	case 19: 
		m_strCopperImage = str;//铜钱
		break;
	case 20: 
		m_strExpImage = str;//经验
		break;
	case 21: 
		m_strAchievementImage = str;//功勋
		break;
	case 22: 
		m_strGiftGoldImage = str;//礼金
		break;
	case 23: 
		m_strContributeImage = str;//城防值
		break;
	case 24: 
		m_strExploitImage = str;//战功
		break;
	case 25:
		m_strBt = str;
		break;
	}
}

void crJXJUILandRewardUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_this){
		crData *canvasData = m_this->getDataClass();
		if(canvasData){
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJLandRewardMap,param);
			LandRewardMap * lanRewardMap = (LandRewardMap *)param;
			
			canvasData->getParam(WCHDATA_JXJLandRewardMulti,param);
			unsigned char multiRate = *(unsigned char *)param;

			canvasData->getParam(WCHDATA_JXJClientHasLandReward,param);
			bool bHasReward = *(bool *)param;
			ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(bHasReward && itemTab.valid())
			{
				//LandRewardMap tempMap = *lanRewardMap;
				crSequence *seq[3] = {NULL};
				ref_ptr<crMultiSwitch>   stopSw[3] = {NULL};
				ref_ptr<crStaticTextWidgetNode>  name[3] = {NULL};
				ref_ptr<crStaticTextWidgetNode>  number[3] = {NULL};

				for (int i = 0;i<3;i++)	{
					seq[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_strSeq[i]));
					stopSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strStopSw[i]));
					name[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strName[i]));
					number[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strNumber[i]));
				}
				ref_ptr<crImageBoxWidgetNode>  imageBox_1 = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strImage_1));
				ref_ptr<crImageBoxWidgetNode>  imageBox_2 = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strImage_2));
				ref_ptr<crMultiSwitch>  imageSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strMultiImage));
				ref_ptr<crTableIO>equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
				ref_ptr<crButtonWidgetNode>  btNode = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strBt));

				crTableIO::StrVec record;
				int eNameIndex = equipTab->getTitleIndex("name");
				int eIconIndex = equipTab->getTitleIndex("icon");
				int iNameIndex = itemTab->getTitleIndex("name");
				int iIconIndex = itemTab->getTitleIndex("icon");
				bool isAllStop = true;
				bool isStop = false;
				std::string strItemName;
				string strImageName;
				string strItemCount;
				LandRewardMap::iterator itr_begin = lanRewardMap->begin();
				for (int i = 0;i<3;i++)
				{
					if(seq[i] && stopSw[i].valid() && name[i].valid() && number[i].valid())
					{
						if(crSequence::STOP == seq[i]->getMode())
						{
							isStop = true;
							int xy = stopSw[i]->getActiveSwitchSet();
							if( xy == 0)
							{
								stopSw[i]->setActiveSwitchSet(1);
								if(i<2){
									switch (itr_begin->first)
									{
									case T_Food:
										strItemName = string("粮草");
										strImageName = m_strFoodImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Wood:
										strItemName = string("木材");
										strImageName = m_strWoodImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Iron:
										strItemName = string("铁矿");
										strImageName = m_strIronImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Horse:
										strItemName = string("马匹");
										strImageName = m_strHorseImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Copper:
										strItemName = string("铜钱");
										strImageName = m_strCopperImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Exp:
										strItemName = string("经验");
										strImageName = m_strExpImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Achievement:
										strItemName = string("功勋");
										strImageName = m_strAchievementImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Giftgold:
										strItemName = string("礼金");
										strImageName = m_strGiftGoldImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_TroopsEquip:
										{
											if(equipTab->queryOneRecord(0,crArgumentParser::appItoa(itr_begin->second.first),record)>=0)
											{
												strItemName = record[eNameIndex];
												strImageName = record[eIconIndex];
												strItemCount = crArgumentParser::appItoa(itr_begin->second.second/multiRate);
											}											
										}
										break;
									case T_Item:
										{
											if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(itr_begin->second.first),record)>=0)
											{
												strItemName = record[iNameIndex];
												strImageName = record[iIconIndex];
												strItemCount = crArgumentParser::appItoa(itr_begin->second.second/multiRate);
											}
										}
										break;
									case T_Contribute:
										strItemName = string("国家贡献");
										strImageName = m_strContributeImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_Exploit:
										strItemName = string("战功");
										strImageName = m_strExploitImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									case T_GuoZhanJiFen:
										strItemName = string("国战积分");
										strImageName = m_strExploitImage;
										strItemCount = crArgumentParser::appItoa(itr_begin->second.second/*/multiRate*/);
										break;
									}

									name[i]->setString(strItemName);
									number[i]->setString(strItemCount);

									if(i == 0 && imageBox_1.valid()){
										imageBox_1->setVisiable(true);
										imageBox_1->setImageName(strImageName);
									}else if(i == 1){
										imageBox_2->setVisiable(true);
										imageBox_2->setImageName(strImageName);
									}
									itr_begin = lanRewardMap->erase(itr_begin);
								}else{
									strItemName = std::string("倍率");
									strItemCount = std::string("x")+crArgumentParser::appItoa(multiRate);

									name[i]->setString(strItemName);
									number[i]->setString(strItemCount);

									if(imageSwitch.valid())
									{
										int tempRate = multiRate;
										int index = 0;
										tempRate/=2;
										while (tempRate)
										{
											tempRate/=2;
											index++;
										}
										imageSwitch->setActiveSwitchSet(index);
									}
								}	
							}
							else{
								continue;
							}
						}else{
							isStop = false;
						}

						isAllStop  = isAllStop && isStop;
					}
				}

				if(isAllStop)
				{
					if(btNode.valid())
					{
						crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
						if(playerData)
						{
							playerData->getParam(WCHDATA_JXJLandRewardGetCount,param);
							unsigned char count = *(unsigned char *)param;
							if(count>0)
							{
								btNode->setEnable(true);
							}
							else
							{
								btNode->setEnable(false);
							}
						}	
					}
					//提交发放奖励
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					if(netConductor){
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSendLandRewardToPlayer,NULL);
						netConductor->getNetManager()->sendPacket("all",packet);
						//清除CanvasData
						canvasData->inputParam(WCHDATA_JXJLandRewardMap,NULL);
						canvasData->inputParam(WCHDATA_JXJLandRewardMulti,NULL);
						canvasData->inputParam(WCHDATA_JXJClientHasLandReward,NULL);
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJGameClientLandRewardTimerUpdateMethod
//
/////////////////////////////////////////
crJXJGameClientLandRewardTimerUpdateMethod::crJXJGameClientLandRewardTimerUpdateMethod(){}
crJXJGameClientLandRewardTimerUpdateMethod::crJXJGameClientLandRewardTimerUpdateMethod(const crJXJGameClientLandRewardTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJGameClientLandRewardTimerUpdateMethod::inputParam(int i, void *param)
{
	
}

void crJXJGameClientLandRewardTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJGameClientLandRewardTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJLandRewardClientTime,param);
		int * cycleTimer = (int *)param;
		if(*cycleTimer>0)
			(*cycleTimer)-=1;
		else
		{
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if(canvas.valid() && canvas->getVisiable() == false){
				crData *canvasData = canvas->getDataClass();
				if(canvasData)
				{
					canvasData->excHandle(MAKEINT64(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJLandRewardMap,param);
					LandRewardMap *landRewardMap = (LandRewardMap *)param;

					if(!landRewardMap->empty()){
						//提交发放奖励
						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if(netConductor){
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSendLandRewardToPlayer,NULL);
							netConductor->getNetManager()->sendPacket("all",packet);
							//清除CanvasData
							canvasData->inputParam(WCHDATA_JXJLandRewardMap,NULL);
							canvasData->inputParam(WCHDATA_JXJLandRewardMulti,NULL);
							canvasData->inputParam(WCHDATA_JXJClientHasLandReward,NULL);
						}
					}
					canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJResetLunHuiPageMethod
//
/////////////////////////////////////////
crJXJResetLunHuiPageMethod::crJXJResetLunHuiPageMethod(){}
crJXJResetLunHuiPageMethod::crJXJResetLunHuiPageMethod(const crJXJResetLunHuiPageMethod& handle):
	crMethod(handle)
{
}
void crJXJResetLunHuiPageMethod::inputParam(int i, void *param)
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

void crJXJResetLunHuiPageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch = str;
		break;
	default:
		break;
	}
}

void crJXJResetLunHuiPageMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
	ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this);

	if(parentCanvas.valid() && radioGroup.valid()){
		ref_ptr<crMultiSwitch>  switchNode = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_strSwitch));
		if(switchNode.valid())
		{
			switchNode->setActiveSwitchSet(radioGroup->getSelect());
		}
	}
}

/////////////////////////////////////////
//
//crJXJLunHuiCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJLunHuiCanvasUpdateMethod::crJXJLunHuiCanvasUpdateMethod(){}
crJXJLunHuiCanvasUpdateMethod::crJXJLunHuiCanvasUpdateMethod(const crJXJLunHuiCanvasUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJLunHuiCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJLunHuiCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strRadioGroup = str;
		break;
	case 1:
		m_strSwitch = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_strItemName[i-2] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		m_strItemNum[i-7] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
		m_strItemImg[i-12] = str;
		break;
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_strFinishSwitch[i-17] = str;
		break;
	case 22:
		m_strListScroll = str;
		break;
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
		m_strCardShowSwitch[i-23] = str;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		m_strItemShowSwitch[i-28] = str;
		break;
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
		m_strStoreCardImage[i-33] = str;
		break;
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
		m_strCardSwitch[i-39] = str;
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
		m_strStoreItem[i-45] = str;
		break;
	case 51:
		m_strBtCard = str;
		break;
	case 52:
		m_strBtItem = str;
		break;
	case 53:
		m_strGetImage = str;
		break;
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
	case 61:
		m_strNumberSwitch[i-54] = str;
		break;
	case 62:
	case 63:
		m_strlvSwitch[i-62] = str;
		break;
	case 64:
		m_strDefaultCardImg = str;
		break;
	case 65:
		m_strDefaultItemImg = str;
		break;
	default:
		break;
	}
}

void crJXJLunHuiCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_strRadioGroup));
	crRole *mainRole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	playerData->getParam(WCHDATA_JXJVipLv,param);
	unsigned char vipLevel = *(unsigned char *)param;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	//int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiBtVisiableLv,vipLevel).c_str());
	
	if(radioGroup.valid() && mainRole && itemtab.valid()){
		crData *mainRoleData = mainRole->getDataClass();
		mainRoleData->getParam(WCHDATA_Level,param);
		unsigned char mylv = *(unsigned char *)param;
		if(radioGroup->getSelect() == 2){
			crTableIO::StrVec itemrecord;
			crStaticTextWidgetNode *itemName[5] = {NULL};
			crStaticTextWidgetNode *itemNum[5] = {NULL};
			crImageBoxWidgetNode * iteImg[5] = {NULL};
			crMultiSwitch * finishSw[5] = {NULL};
			crScrollBarWidgetNode *scrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_strListScroll));
			for (int i = 0;i<5;i++)
			{
				itemName[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strItemName[i]));
				itemNum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strItemNum[i]));
				iteImg[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strItemImg[i]));
				finishSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strFinishSwitch[i]));
				if(itemName[i])
					itemName[i]->clearString();
				if(itemNum[i])
					itemNum[i]->clearString();
				if(iteImg[i])
					iteImg[i]->setVisiable(false);
				if(finishSw[i])
					finishSw[i]->setActiveSwitchSet(0);
			}
			int nameIndex = itemtab->getTitleIndex("name");
			int iconIndex = itemtab->getTitleIndex("icon");
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJLunHuiRewardMap,param);
			LunHuiRewardMap *lunhuiRewardMap = (LunHuiRewardMap*)param;
			int beginIndex = 0;
			
			if(scrollBar)
			{
				//scrollBar->setRange(0,lunhuiRewardMap->size());
				beginIndex = (int)scrollBar->getValue()*lunhuiRewardMap->size();
			}
			int index = beginIndex;

			for (LunHuiRewardMap::iterator itr = lunhuiRewardMap->begin();\
				itr!=lunhuiRewardMap->end() && index<beginIndex+5;itr++,index++)
			{
				if(itemName[index-beginIndex])
					itemName[index-beginIndex]->setString(std::string("开放等级:")+crArgumentParser::appItoa(itr->first)+std::string("级"));
				if(itemNum[index-beginIndex])
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itr->second.second[0]),itemrecord)>=0)
					{
						itemNum[index-beginIndex]->setString(itemrecord[nameIndex]+std::string("*")+crArgumentParser::appItoa(itr->second.second[2]));
						if(iteImg[index-beginIndex])
						{
							iteImg[index-beginIndex]->setVisiable(true);
							iteImg[index-beginIndex]->setImageName(itemrecord[iconIndex]);
						}
					}
				}
				if(finishSw[index-beginIndex])
					finishSw[index-beginIndex]->setActiveSwitchSet(itr->second.first);
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));

		}else if(radioGroup->getSelect() == 0){
			crMultiSwitch *cardlvSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strlvSwitch[0]));
			crMultiSwitch *itemlvSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strlvSwitch[1]));
			if(cardlvSwitch && itemlvSwitch)
			{
				//if(mylv>=openlv){
					cardlvSwitch->setActiveSwitchSet(1);
					itemlvSwitch->setActiveSwitchSet(1);
					//显示界面
					crMultiSwitch * enableCardSwitch = NULL;
					crMultiSwitch * enableItemSwitch = NULL;
					int openCardCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiCardPacketOpenNum,vipLevel).c_str());
					int openItemCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiItemPacketOpenNum,vipLevel).c_str());
					//openCount-=1;
					for (int i = 0;i<5;i++)
					{
						enableCardSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strCardShowSwitch[i]));
						enableItemSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strItemShowSwitch[i]));
						if(enableCardSwitch)
						{
							if(i<openCardCount-1)
							{
								enableCardSwitch->setActiveSwitchSet(0);
							}
							else
							{
								enableCardSwitch->setActiveSwitchSet(1);
							}
						}
						if(enableItemSwitch)
						{
							if(i<openItemCount-1)
							{
								enableItemSwitch->setActiveSwitchSet(0);
							}
							else
							{
								enableItemSwitch->setActiveSwitchSet(1);
							}
						}
					}
					//显示轮回将卡背包内容
					crImageBoxWidgetNode *imageCard[6] = {NULL};
					crImageBoxWidgetNode *imageItem[6] = {NULL};
					crMultiSwitch * cardQulitySw[6] = {NULL};
					for (int i= 0;i<6;i++)
					{
						imageCard[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strStoreCardImage[i]));
						cardQulitySw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strCardSwitch[i]));
						imageItem[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strStoreItem[i]));

						if(imageCard[i])
						{
							if(i<openCardCount)
							{
								imageCard[i]->setVisiable(true);
								imageCard[i]->setImageName(m_strDefaultCardImg);
							}
							else
							{
								imageCard[i]->setVisiable(false);
 							}
						}
						if(imageItem[i])
						{
							if(i<openItemCount)
							{
								imageItem[i]->setVisiable(true);
								imageItem[i]->setImageName(m_strDefaultItemImg);
							}
							else
							{
								imageItem[i]->setVisiable(false);
							}
						}
					}
					playerData->getParam(WCHDATA_JXJPlayerStore,param);
					ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
					if(playerStore.valid())
					{
						crData *playerStoreData = playerStore->getDataClass();
						if(playerStoreData)
						{
							playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
							playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
							JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;

							playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
							JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;

							for (int i= 0;i<6;i++)
							{
								if(imageCard[i])
								{
									if(i<cardVec->size())
									{
										imageCard[i]->setVisiable(true);
										crTableIO::StrVec itemrecord;
										if(itemtab.valid() && (*cardVec)[i].valid() && \
											itemtab->queryOneRecord(0,crArgumentParser::appItoa((*cardVec)[i]->getItemID()),itemrecord)>=0)
										{
											int iconIndex = itemtab->getTitleIndex("icon");
											int qualityIndex = itemtab->getTitleIndex("品质");

											imageCard[i]->setImageName(itemrecord[iconIndex]);

											int q = atoi(itemrecord[qualityIndex].c_str());
											if(cardQulitySw[i])
											{
												cardQulitySw[i]->setActiveSwitchSet(0);
												//if(q<=CardColor_Orange)
												//{
													cardQulitySw[i]->setActiveSwitchSet(q+1);
												//}
												//else if(q>CardColor_Orange)
												//{
												//	cardQulitySw[i]->setActiveSwitchSet(CardColor_Orange+1);
												//}
											}
											
										}
									}
									else
									{
										imageCard[i]->setImageName(m_strDefaultCardImg);
									}
								}
								if(imageItem[i])
								{
									if(i<itemVec->size())
									{
										imageItem[i]->setVisiable(true);
										crTableIO::StrVec itemrecord;
										if(itemtab.valid() && (*itemVec)[i].valid() && \
											itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itemVec)[i]->getItemID()),itemrecord)>=0)
										{
											int iconIndex = itemtab->getTitleIndex("icon");
											//int qualityIndex = itemtab->getTitleIndex("品质");

											imageItem[i]->setImageName(itemrecord[iconIndex]);
										}
									}
									else
									{
										imageItem[i]->setImageName(m_strDefaultItemImg);
									}
								}
							}

							playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				//}else{
				//	cardlvSwitch->setActiveSwitchSet(0);
				//	itemlvSwitch->setActiveSwitchSet(0);
				//}
			}	
		}
		else if(radioGroup->getSelect() == 1)
		{
			crImageBoxWidgetNode * imgBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strGetImage));
			if(imgBox)
				imgBox->setVisiable(false);

			int money = 0;
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			if(playerStore.valid())
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					//playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					playerStoreData->getParam(WCHDATA_Money,param);
					money = *(int *)param;
					//playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

			std::list<int> numList;
			while (money!=0)
			{
				numList.push_front(money%10);
				money/=10;
			}

			crMultiSwitch *numberSw[8] = {NULL};			
			for (int i = 0; i < 8; i++)
			{
				numberSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strNumberSwitch[i]));
				if(numberSw[i])
				{
					numberSw[i]->setActiveSwitchSet(10);
				}
			}
			int i = 0;
			for (std::list<int>::iterator itr_num = numList.begin();
				itr_num !=numList.end();itr_num++,i++)
			{
				if(i>8) break;
				numberSw[i]->setActiveSwitchSet(*itr_num);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGameServerCycleMethod
//
/////////////////////////////////////////
crJXJGameServerCycleMethod::crJXJGameServerCycleMethod():m_validCycleLv(60){}
crJXJGameServerCycleMethod::crJXJGameServerCycleMethod(const crJXJGameServerCycleMethod& handle):
	crMethod(handle),
	m_validCycleLv(handle.m_validCycleLv)
{
}
void crJXJGameServerCycleMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle *)param;
		break;
	}
}

void crJXJGameServerCycleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_validCycleLv = atoi(str.c_str());
		break;
	}
}

void crJXJGameServerCycleMethod::operator()(crHandle &handle)
{
	//1.处理玩家数据写入stream
	//1.1查询国家系统国民列表循环国民列表插入玩家等级，玩家
	// 处理数据库
	void *param;
	crGlobalHandle::gSetOpenGameLogin(0);//服务器进入维护
// 	crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
// 	ref_ptr<crDataBase> gameSession = gamedb->beginSession();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	int gameid = callback->getGameID();
	int serverid = callback->getServerID();
	crJXJShili *shili[3] = {NULL};
	crData *gsData = m_this->getDataClass();
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if(gsData && dbConductor)
	{
		gsData->excHandle(MAKEINT64(WCH_LockData,1));

		//gsData->getParam(WCHDATA_JXJCreatePlayerStoreInfoDeque,param);
		//PlayerStoreInfoDeq *playerStoreInfoDeq = (PlayerStoreInfoDeq *)param;

		gsData->getParam(WCHDATA_JXJShiliWei,param);
		shili[0] = (crJXJShili *)param;
		gsData->getParam(WCHDATA_JXJShiliShu,param);
		shili[1] = (crJXJShili *)param;
		gsData->getParam(WCHDATA_JXJShiliWu,param);
		shili[2] = (crJXJShili *)param;

		ref_ptr<crGameServerPlayerData> playerdata;
		int money = 0;
		int totalMoney = 0;
		unsigned char viplv = 0;
		int vipexp  = 0;
		int playerid = 0;
		ref_ptr<crPeopleCycledQueryData> peopleCycledQuery;
		ref_ptr<crUpdatePeopleCycled> updatePeopleCycled;
		ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery;
		ref_ptr<crPlayerTotalAddMoneyQueryData> playerTotalAddMoneyQuery;
		//ref_ptr<crPlayerVipQueryData> playerVipQuery;
		ref_ptr<crPlayerGameStoreQueryData> playerStoreQuery;
		//ref_ptr<crPlayerGameStoreCycleCountQueryData> playerCycleCountQuery;
		ref_ptr<crJXJPlayerStore> playerStore;
		crData *playerStoreData = NULL;
		JXJStoreItemBagVec::iterator itr;
		int maxCount = 1;
		int cycleCountAdd = 0;
		CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreInfo ;
		//CRCore::ref_ptr<CRCore::crStreamBuf> streamBuffer;
		
		//ref_ptr<crPlayerGameStoreQuery2Data> playerStoreQuery2 = new crPlayerGameStoreQuery2Data;
		CRCore::ref_ptr<crUpdatePlayerGameStore > updatePlayerStore = new crUpdatePlayerGameStore;
		CRCore::ref_ptr<crInsertPlayerGameStore > insertPlayerStore = new crInsertPlayerGameStore;
		crDBSavePacket packet;
		crKickoffPlayerPacket kpacket;

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
		crDataBase *gameglobaldb = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crDataBase> globalSession;// = db->beginSession();
		ref_ptr<crDataBase> accountSession;// = accountdb->beginSession();
		ref_ptr<crDataBase> gameGlobalSession;// = gameglobaldb->beginSession();
		for (int i = 0;i<3;i++)
		{
			if(!shili[i]) continue;
			crData *shiliData = shili[i]->getDataClass();
			if(shiliData)
			{
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				PeopleMap::iterator itr_pl = peopleMap->begin();
				for (;itr_pl!=peopleMap->end();itr_pl++)
				{
					playerid = itr_pl->first;
					//把在线玩家踢下线
					playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
					crKickoffPlayerPacket::buildReplyPacket(kpacket,playerid);
					if(playerdata.valid())
					{
						gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),kpacket);
						netDataManager->removePlayerData(playerid);
					}

					peopleCycledQuery = new crPeopleCycledQueryData;
					peopleCycledQuery->buildQuerySql(playerid);
					gameGlobalSession = gameglobaldb->beginSession();
					gameGlobalSession->executeQuery(peopleCycledQuery.get());
					crDataBase::QueryResultVec &peopleCycledQueryResultVec = gameGlobalSession->getQueryResult();
					if(!peopleCycledQueryResultVec.empty())
					{
						peopleCycledQuery = dynamic_cast<crPeopleCycledQueryData *>(peopleCycledQueryResultVec[0].get());
						if(peopleCycledQuery->getCycled() == 1)
						{//该玩家已经轮回过了
							gameGlobalSession->releaseQuery();
							continue;
						}
					}
					gameGlobalSession->releaseQuery();

					playerTotalAddMoneyQuery = new crPlayerTotalAddMoneyQueryData;
					playerTotalAddMoneyQuery->buildQuerySql(playerid, serverid);

					gameGlobalSession->executeQuery(playerTotalAddMoneyQuery.get());
					crDataBase::QueryResultVec &playerTotalAddMoneyQueryResultVec = gameGlobalSession->getQueryResult();
					totalMoney = 0;
					if (!playerTotalAddMoneyQueryResultVec.empty())
					{
						playerTotalAddMoneyQuery = dynamic_cast<crPlayerTotalAddMoneyQueryData *>(playerTotalAddMoneyQueryResultVec[0].get());
						totalMoney = playerTotalAddMoneyQuery->getMoney();
					}
					gameGlobalSession->releaseQuery();
					gameglobaldb->endSession(gameGlobalSession.get());

					//处理玩家数据查询玩家money
					playerMoneyQuery = new crPlayerMoneyQueryData;
					playerMoneyQuery->buildQuerySql(playerid,serverid);

					globalSession = db->beginSession();
					globalSession->executeQuery(playerMoneyQuery.get());
					crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
					money = 0;
					if(!playerMoneyQueryResultVec.empty())
					{
						playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
						money = playerMoneyQuery->getMoney();
					}
					globalSession->releaseQuery();

					//处理玩家VIP数据
// 					playerVipQuery = new crPlayerVipQueryData;
// 					playerVipQuery->buildQuerySql(itr_pl->first,gameid);
// 
// 					globalSession->executeQuery(playerVipQuery.get());
// 					crDataBase::QueryResultVec &playerVipQueryResultVec = globalSession->getQueryResult();
// 					if(!playerVipQueryResultVec.empty())
// 					{
// 						playerVipQuery = dynamic_cast<crPlayerVipQueryData *>(playerVipQueryResultVec[0].get());
// 						viplv = playerVipQuery->getViplv();
// 						vipexp = playerVipQuery->getVipexp();
// 					}
// 					globalSession->releaseQuery();
					//处理玩家轮回背包数据
					//char cycleCount = 0;
					//playerCycleCountQuery = new crPlayerGameStoreCycleCountQueryData;
					//playerCycleCountQuery->buildQuerySql(itr_pl->first, gameid);
					//accountSession->executeQuery(playerCycleCountQuery.get());
					//crDataBase::QueryResultVec &playerStoreResultVec = accountSession->getQueryResult();

					//if (!playerStoreResultVec.empty())
					//{
					//	playerCycleCountQuery = dynamic_cast<crPlayerGameStoreCycleCountQueryData *>(playerStoreResultVec[0].get());
					//	cycleCount = playerCycleCountQuery->getCycleCount();
					//}
					//accountSession->releaseQuery();
					cycleCountAdd = itr_pl->second->getLv()>=m_validCycleLv?1:0;

					playerStoreQuery = new crPlayerGameStoreQueryData;
					playerStoreQuery->buildQuerySql(playerid,gameid);
					accountSession = accountdb->beginSession();
					accountSession->executeQuery(playerStoreQuery.get());
					crDataBase::QueryResultVec &playerStoreResultVec = accountSession->getQueryResult();
					if(!playerStoreResultVec.empty())
					{
						//更新
						playerStoreQuery = dynamic_cast<crPlayerGameStoreQueryData *>(playerStoreResultVec[0].get());
						JXJStoreItemBagVec &cardVec = playerStoreQuery->getCardBagVec();
						JXJStoreItemBagVec &itemVec = playerStoreQuery->getItemBagVec();
						for( JXJStoreItemBagVec::iterator itr = cardVec.begin();
							itr != cardVec.end();
							++itr )
						{
							if(itr->valid() && (*itr)->getItemServerID() == serverid)
							{
								(*itr)->setItemHasCycle(1);
							}
						}
						for( JXJStoreItemBagVec::iterator itr = itemVec.begin();
							itr != itemVec.end();
							++itr )
						{
							if(itr->valid() && (*itr)->getItemServerID() == serverid)
							{
								(*itr)->setItemHasCycle(1);
							}
						}
						updatePlayerStore->buildUpdateSql_Cycle(playerStoreQuery->getID(),serverid,cycleCountAdd,money,totalMoney,cardVec,itemVec);
						crDBSavePacket::buildRequestPacket(packet,updatePlayerStore.get(),-1);
					}
					else
					{
						//插入记录
						insertPlayerStore->buildUpdateSql_Cycle(playerid,gameid,cycleCountAdd,money,totalMoney);
						crDBSavePacket::buildRequestPacket(packet,insertPlayerStore.get(),-1);
					}
					dbConductor->getNetManager()->sendPacket("all",packet);

					updatePeopleCycled = new crUpdatePeopleCycled;
					updatePeopleCycled->buildUpdateSql(playerid);
					crDBSavePacket::buildRequestPacket(packet,GameGlobalDB,updatePeopleCycled.get());
					dbConductor->getNetManager()->sendPacket("all",packet);

					accountSession->releaseQuery();
					accountdb->endSession(accountSession.get());
				}
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}	
		gsData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	//2。清除相关数据库表
}

/////////////////////////////////////////
//
//crJXJRecvPlayerStoreMethod
//
/////////////////////////////////////////
crJXJRecvPlayerStoreMethod::crJXJRecvPlayerStoreMethod(){}
crJXJRecvPlayerStoreMethod::crJXJRecvPlayerStoreMethod(const crJXJRecvPlayerStoreMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPlayerStoreMethod::inputParam(int i, void *param)
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

void crJXJRecvPlayerStoreMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	}
}

void crJXJRecvPlayerStoreMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();
		int serverid = callback->getServerID();
		//接收item
		ref_ptr<crTableIO>itemTable = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec	record;
		//获取提交类型0.将卡1.装备
		unsigned char uctype = m_stream->_readChar();
		unsigned char size = m_stream->_readUChar();
		std::string logdata;
		crData *playerData = m_this->getDataClass();
		int playerid = m_this->getPlayerID();
		std::map<unsigned char,ref_ptr<crStoreBagItemData>> tempStoreMap;
		if(playerData && itemTable.valid())
		{
			int colorindex = itemTable->getTitleIndex("品质");
			int iconIndex = itemTable->getTitleIndex("icon");
			int maxCountIndex = itemTable->getTitleIndex("叠加数量");
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			crJXJPlayerStore *playerStore = (crJXJPlayerStore *)param;
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;

			JXJItemBagVec *bagVec = NULL;
			JXJStoreItemBagVec *playerStoreBagVec = NULL;
			if(uctype == 1)
			{
				playerData->getParam(WCHDATA_JXJItemBagVec,param);
				bagVec = (JXJItemBagVec *)param;
			}
			else if(uctype == 0)
			{
				playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
				bagVec = (JXJItemBagVec *)param;
			}
			
			char _case = 1;
			int maxCount = 0;
			
			if(bagVec && playerStore)
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					if(uctype == 1)
					{
						playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
						playerStoreBagVec = (JXJStoreItemBagVec *)param;
						maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiItemPacketOpenNum,vipLevel).c_str());
						logdata+="轮回仓库道具：";
					}
					else if(uctype == 0)
					{
						playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
						playerStoreBagVec = (JXJStoreItemBagVec *)param;
						maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiCardPacketOpenNum,vipLevel).c_str());
						logdata+="轮回仓库将卡：";
					}

					char index = 0;
					int itemID = 0;
					int magicID = 0;
					ref_ptr<crStoreBagItemData> tempitemData;
					
					crVector3i itemOrCard[10];
					//bool isOverCount = false;//检测是否超出最大数量
					//bool isHaveAnotherServer = false;//检测是否有要替换其他server的物品
				
					for (int i = 0; i < size; i++)
					{
						index = m_stream->_readChar();
						itemID = m_stream->_readInt();
						if (uctype == 1)//装备
							magicID = m_stream->_readInt();
						if (index>maxCount)
						{
							_case = -1;//物品超出最大数量
							break;
						}

						if (index < playerStoreBagVec->size() && \
							(*playerStoreBagVec)[index]->getItemServerID() != serverid)
						{
							_case = -2;//要替换物品中有其他区的物品
							break;
						}
						if (itemTable->queryOneRecord(0, crArgumentParser::appItoa(itemID), record) < 0)
						{
							_case = -3;//提交物品中含有非法物品
							break;
						}

						itemOrCard[index][0] = itemID;
						itemOrCard[index][1] = magicID;
						itemOrCard[index][2] = atoi(record[maxCountIndex].c_str());//最大可叠加数
						logdata += crArgumentParser::appVectoa(itemOrCard[index],'|');
					}

					if (_case == 1)//可以存储
					{
						for (int i = 0; i < 10; i++)
						{
							if (itemOrCard[i] == crVector3i(0,0,0))
								continue;
							//index = i;
							//itemID = itemOrCard[i][0];
							//magicID = itemOrCard[i][1];itemOrCard[i][2]

							JXJItemBagVec::iterator itr_item = bagVec->begin();
							for (; itr_item != bagVec->end(); itr_item++)
							{
								if (!itr_item->valid()) continue;
								if (itr_item->get()->getItemID() == itemOrCard[i][0] && \
									itr_item->get()->getEquipMagic() == itemOrCard[i][1])
								{
									tempitemData = new crStoreBagItemData;
									tempitemData->setItemID(itemOrCard[i][0]);
									tempitemData->setEquipMagic(itemOrCard[i][1]);
									tempitemData->setItemCount(1);
									tempitemData->setItemServerID(serverid);
									tempitemData->setItemHasCycle(0);
									if (i < playerStoreBagVec->size())
									{
										//处理背包
										if (uctype == 1)
										{
											//bool isNeedAddNew = true;
											//for (JXJItemBagVec::iterator itr_bagItem = bagVec->begin(); \
											//	itr_bagItem != bagVec->end(); itr_bagItem++)
											//{
											//	if (!itr_bagItem->valid()) continue;
											//	if ((*itr_bagItem)->getItemID() == itemOrCard[i][0])
											//	{
											//		if (itr_bagItem->get()->getItemCount() == 1)
											//		{
											//			*itr_bagItem = NULL;
											//		}
											//		else if (itr_bagItem->get()->getItemCount() > 1)
											//		{
											//			itr_bagItem->get()->setItemCount(itr_bagItem->get()->getItemCount() - 1);
											//		}
											//		break;
											//	}
											//}
											if (itr_item->get()->getItemCount() == 1)
											{
												*itr_item = NULL;
											}
											else if (itr_item->get()->getItemCount() > 1)
											{
												itr_item->get()->setItemCount(itr_item->get()->getItemCount() - 1);
											}
											//for (JXJItemBagVec::iterator itr_bagItem = bagVec->begin(); \
											//	itr_bagItem != bagVec->end(); itr_bagItem++)
											//{
											//	if (!itr_bagItem->valid()) continue;
											//	if ((*itr_bagItem)->getItemID() == (*playerStoreBagVec)[i]->getItemID() && \
											//		(*itr_bagItem)->getEquipMagic() == (*playerStoreBagVec)[i]->getEquipMagic() && \
											//		itr_bagItem->get()->getItemCount() < itemOrCard[i][2])
											//	{
											//		itr_bagItem->get()->setItemCount(itr_bagItem->get()->getItemCount() + 1);
											//		isNeedAddNew = false;
											//		break;
											//	}
											//}
											//if (isNeedAddNew)
											//{
											//	for (JXJItemBagVec::iterator itr_bagItem = bagVec->begin(); \
											//		itr_bagItem != bagVec->end(); itr_bagItem++)
											//	{
											//		if ((*itr_bagItem) == NULL)
											//		{
											//			ref_ptr<crBagItemData> temp = new crBagItemData;
											//			temp->setItemID((*playerStoreBagVec)[i]->getItemID());
											//			temp->setEquipMagic((*playerStoreBagVec)[i]->getEquipMagic());
											//			temp->setItemCount(1);
											//			*itr_bagItem = temp;
											//			break;
											//		}
											//	}
											//}
										}
										else if (uctype == 0)
										{
											for (JXJItemBagVec::iterator itr_bagItem = bagVec->begin(); \
												itr_bagItem != bagVec->end(); itr_bagItem++)
											{
												if (!itr_bagItem->valid()) continue;
												if ((*itr_bagItem)->getItemID() == itemOrCard[i][0])
												{
													//ref_ptr<crBagItemData> temp = new crBagItemData;
													//temp->setItemID((*playerStoreBagVec)[i]->getItemID());
													//temp->setEquipMagic((*playerStoreBagVec)[i]->getEquipMagic());
													//temp->setItemCount(1);
													//*itr_bagItem = temp;
													*itr_bagItem = NULL;
													break;
												}
											}
										}
										tempStoreMap.insert(make_pair(i, tempitemData));
										(*playerStoreBagVec)[i] = tempitemData;
									}
									else
									{
										tempStoreMap.insert(make_pair(playerStoreBagVec->size(), tempitemData));

										playerStoreBagVec->push_back(tempitemData);
										//处理将卡背包

										for (JXJItemBagVec::iterator itr_bagItem = bagVec->begin(); \
											itr_bagItem != bagVec->end(); itr_bagItem++)
										{
											if (!itr_bagItem->valid()) continue;
											if (itr_bagItem->get()->getItemID() == itemOrCard[i][0] &&
												itr_bagItem->get()->getEquipMagic() == itemOrCard[i][1])
											{
												if (itr_bagItem->get()->getItemCount() == 1)
												{
													*itr_bagItem = NULL;
												}
												else if (itr_bagItem->get()->getItemCount() > 1)
												{
													itr_bagItem->get()->setItemCount(itr_bagItem->get()->getItemCount() - 1);
												}

												break;
											}
										}

									}
									break;
								}
							}
						}
					}
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));

			if(_case == 1)
			{
				//update数据库数据playerstore
				crServerBrainHandle * gameServerBrain = crServerBrainHandle::getInstance();
				playerData->getParam(WCHDATA_JXJPlayerStore,param);
				crJXJPlayerStore *playerStore = (crJXJPlayerStore *)param;
				//playerData->getParam(WCHDATA_JXJVipLv,param);
				//unsigned char vipLevel = *(unsigned char *)param;
				//playerData->getParam(WCHDATA_JXJPlayerCycleCount, param);
				//char cycleCount = *(char *)param;
				//if (cycleCount == -1) cycleCount = 0;
				if( playerStore)
				{
					crData *gsData = gameServerBrain->getDataClass();
					crData *playerStoreData = playerStore->getDataClass();
					if(gsData && playerStoreData)
					{
						gsData->excHandle(MAKEINT64(WCH_LockData,1));

						gsData->getParam(WCHDATA_JXJCreatePlayerStoreInfoDeque,param);
						PlayerStoreInfoDeq *playerStoreInfoDeq = (PlayerStoreInfoDeq *)param;

						playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));

						CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreInfo = new crJXJPlayerStoreInfo;
						//
						int playerid = m_this->getPlayerID();
						playerStoreInfo->setPlayerId(playerid);
						//
						//playerStoreData->getParam(WCHDATA_Money, param);
						//playerStoreInfo->setPlayerMoney(*(int *)param);
						//
						//playerStoreData->getParam(WCHDATA_JXJPlayerTotalAddMoney, param);
						//playerStoreInfo->setPlayerUsedMoney(*(int *)param);
						//
						playerStoreData->getParam(WCHDATA_JXJItemBagVec, param);
						playerStoreInfo->getStoreItemVec() = *(JXJStoreItemBagVec *)param;
						//
						playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec, param);
						playerStoreInfo->getStoreCardVec() = *(JXJStoreItemBagVec *)param;

						//playerStoreInfo->setPlayerCycleCount(cycleCount);
						playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));

						if(playerStoreInfoDeq)
							playerStoreInfoDeq->push_back(playerStoreInfo.get());
						gsData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			if(_case == 1)
			{
				stream->createBuf(3+tempStoreMap.size()*14);
				stream->_writeChar(_case);
				stream->_writeUChar(uctype);
				stream->_writeUChar(tempStoreMap.size());
				for (std::map<unsigned char,ref_ptr<crStoreBagItemData>>::iterator itr_map = tempStoreMap.begin();
					itr_map!=tempStoreMap.end();itr_map++)
				{
					stream->_writeUChar(itr_map->first);				//1
					stream->_writeInt(itr_map->second->getItemID());//4
					stream->_writeInt(itr_map->second->getEquipMagic());//4
					stream->_writeUChar(itr_map->second->getItemCount());//1
					stream->_writeInt(serverid);//4
				}
				///游戏日志
				GameLogData gamelog(Log_RecvPlayerStore,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
			else
			{
				stream->createBuf(2);
				stream->_writeBool(_case);
				stream->_writeUChar(uctype);
			}
			
			ref_ptr<crGameServerPlayerData> playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
			if(playerdata.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(), WCH_JXJRecvPlayerStore, stream.get());
				gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if (m_netType == GameClient_Game)
	{
		char _case = m_stream->_readChar();
		unsigned char ucType = m_stream->_readUChar();
		ref_ptr<crCanvasNode>canvasNode  = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		ref_ptr<crTableIO>itemTable = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(!itemTable.valid()) return;
		crTableIO::StrVec	record;
		int colorindex = itemTable->getTitleIndex("品质");
		int iconIndex = itemTable->getTitleIndex("icon");
		int maxCountIndex = itemTable->getTitleIndex("叠加数量");
		if(canvasNode.valid())
		{
			JXJItemBagVec *bagvec = NULL;
			JXJStoreItemBagVec *storeVec = NULL;
			crData *canvasData = canvasNode->getDataClass();
			crData *playerData = m_this->getDataClass();
			if(canvasData)
			{
				canvasData->excHandle(MAKEINT64(WCH_LockData,1));
				ItemOrCardMap * itemOrCardMap = NULL;
				ItemSwitchMap *switchmap = NULL;

				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				if (ucType == 0)
				{
					playerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
					bagvec = (JXJItemBagVec *)param;
					canvasData->getParam(WCHDATA_JXJLunHuiCardMap, param);
					itemOrCardMap = (ItemOrCardMap *)param;
					canvasData->getParam(WCHDATA_JXJTempCardSwitchMap, param);
					switchmap = (ItemSwitchMap *)param;
				}
				else if (ucType == 1)
				{
					playerData->getParam(WCHDATA_JXJItemBagVec,param);
					bagvec = (JXJItemBagVec *)param;
					canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
					itemOrCardMap = (ItemOrCardMap *)param;
					canvasData->getParam(WCHDATA_JXJTempItemSwitchMap, param);
					switchmap = (ItemSwitchMap *)param;
				}
				canvasData->getParam(WCHDATA_JXJPlayerLunHuiReplacecount,param);
				char *replacecount = (char *)param;

				playerData->getParam(WCHDATA_JXJPlayerStore,param);
				crJXJPlayerStore * playerStore = (crJXJPlayerStore *)param;
				if(playerStore)
				{
					crData *playerStoreData = playerStore->getDataClass();
					if(playerStoreData)
					{
						playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
						if (ucType == 0)
						{
							playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
							storeVec = (JXJStoreItemBagVec *)param;
						}
						else if (ucType == 1)
						{
							playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
							storeVec = (JXJStoreItemBagVec *)param;
						}
						
						int texid = 4011;
						if(_case!=1)
						{
							//处理将卡背包数据
							if(bagvec && storeVec)
							{
								//归还bagvec
								ref_ptr<crBagItemData> temp;
								ref_ptr<crStoreBagItemData> tempStore;
								ItemOrCardMap::iterator itr_coi = itemOrCardMap->begin();

								JXJItemBagVec::iterator itr_bag;// = bagvec->begin();
								for (; itr_coi != itemOrCardMap->end(); itr_coi++)
								{
									if(itemTable->queryOneRecord(0, crArgumentParser::appItoa(itr_coi->second->getItemID()), record)>=0)
									{
										int maxCount = atoi(record[maxCountIndex].c_str());
										bool needNew = true;
										itr_bag = bagvec->begin();
										for (;itr_bag!=bagvec->end();itr_bag++)
										{
											if (!itr_bag->valid()) continue;
											if((*itr_bag)->getItemID()==itr_coi->second->getItemID() && \
											   (*itr_bag)->getEquipMagic()==itr_coi->second->getEquipMagic() && \
												itr_bag->get()->getItemCount()< maxCount)
											{
// 												temp = new crBagItemData;
// 												temp->setItemID(itr_coi->second->getItemID());
// 												temp->setEquipMagic(itr_coi->second->getEquipMagic());
// 												temp->setItemCount(1);

												itr_bag->get()->setItemCount(itr_bag->get()->getItemCount()+1);
												needNew = false;
												break;
											}
										}
										if(needNew)
										{
											itr_bag = bagvec->begin();
											for (;itr_bag!=bagvec->end();itr_bag++)
											{
												if((*itr_bag)==NULL)
												{
													temp = new crBagItemData;
													temp->setItemID(itr_coi->second->getItemID());
													temp->setEquipMagic(itr_coi->second->getEquipMagic());
													temp->setItemCount(1);

													*itr_bag = temp;

													break;
												}
											}
										}
									}
								}
								//归还storeVec
								ItemSwitchMap::iterator itr_switch = switchmap->begin();
								for (;itr_switch!=switchmap->end();itr_switch++)
								{
									if(itr_switch->first < storeVec->size())
									{
										//tempStore = new crStoreBagItemData;
										//tempStore->setItemID(itr_switch->second->getItemID());

										(*storeVec)[itr_switch->first] =  itr_switch->second;
									}
								}
							}
							//提示存储失败
							if(_case==-1)
							{
								//数量过多
								texid = 4012;
							}
							else if(_case == -2)
							{
								//其他区物品
								texid = 4013;
							}
							else if(_case == -3)
							{
								//非法物品
								texid = 4013;
							}
						}
						//else
						//else
						//{
						//	//提示存储成功
						//}
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 1;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						//noticeHandle->inputParam(WCHDATA_NoticeParam1,&lastTotalCount);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
						
						//清理临时数据
						if (itemOrCardMap)
							itemOrCardMap->clear();
						if (switchmap)
							switchmap->clear();
						*replacecount = 0;

						//更新数据
						unsigned char index;
						ref_ptr<crStoreBagItemData> tempStore;
						unsigned char size = m_stream->_readUChar();
						int serverid;
						for (int i = 0;i<size;i++)
						{
							index = m_stream->_readUChar();
							tempStore  = new crStoreBagItemData;
							tempStore->setItemID(m_stream->_readInt());
							tempStore->setEquipMagic(m_stream->_readInt());
							tempStore->setItemCount(m_stream->_readUChar());
							serverid = m_stream->_readInt();
							tempStore->setItemServerID(serverid);
							if(index>=storeVec->size())
							{
								storeVec->push_back(tempStore);
							}
							else
							{
								(*storeVec)[index] = tempStore;
							}
						}
						
						playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					playerData->excHandle(MAKEINT64(WCH_LockData,0));
					canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}

		//刷新将卡背包及界面
	}
}

/////////////////////////////////////////
//
//crJXJCreatePlayerStoreDisposeMethod
//
/////////////////////////////////////////
crJXJCreatePlayerStoreDisposeMethod::crJXJCreatePlayerStoreDisposeMethod():
	m_this(NULL),
	m_taskcount(5){}
crJXJCreatePlayerStoreDisposeMethod::crJXJCreatePlayerStoreDisposeMethod(const crJXJCreatePlayerStoreDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_taskcount(handle.m_taskcount)
{
}
void crJXJCreatePlayerStoreDisposeMethod::inputParam(int i, void *param)
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

void crJXJCreatePlayerStoreDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}
}

void crJXJCreatePlayerStoreDisposeMethod::operator()(crHandle &handle)
{
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if(dbConductor)
	{
		m_taskVec.resize(0);
		m_taskVec.reserve(m_taskcount);
		CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreInfo;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();
		int serverid = callback->getServerID();
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJCreatePlayerStoreInfoDeque,param);
		PlayerStoreInfoDeq *query_deque = (PlayerStoreInfoDeq *)param;
		int i = 0;
		while(!query_deque->empty() && i<m_taskcount)
		{
			playerStoreInfo = query_deque->front();
			query_deque->pop_front();
			m_taskVec.push_back(playerStoreInfo);
			i++;
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));

		if(!m_taskVec.empty())
		{
			//const CRCore::Timer& timer = *CRCore::Timer::instance();
			//CRCore::Timer_t start_tick = timer.tick();
			//float elapsedTime = 0.0f;
			crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
			ref_ptr<crDataBase> dbSession = accountdb->beginSession();
			ref_ptr<crPlayerGameStoreQuery2Data> playerStoreQuery2 = new crPlayerGameStoreQuery2Data;
			
			CRCore::ref_ptr<crUpdatePlayerGameStore > updatePlayerStore = new crUpdatePlayerGameStore;
			CRCore::ref_ptr<crInsertPlayerGameStore > insertPlayerStore = new crInsertPlayerGameStore;

			crDBSavePacket packet;
		
			CRCore::ref_ptr<crStreamBuf> streamBuf;
			for( std::vector<CRCore::ref_ptr<crJXJPlayerStoreInfo> >::iterator titr = m_taskVec.begin();
				titr != m_taskVec.end();
				++titr )
			{
				playerStoreInfo = *titr;
				
				playerStoreQuery2->buildQuerySql(playerStoreInfo->getPlayerId(),gameid);
				dbSession->executeQuery(playerStoreQuery2.get());
				crDataBase::QueryResultVec &playerStoreResultVec = dbSession->getQueryResult();

				if(!playerStoreResultVec.empty())
				{
					//更新
					playerStoreQuery2 = dynamic_cast<crPlayerGameStoreQuery2Data *>(playerStoreResultVec[0].get());

					updatePlayerStore->buildUpdateSql(playerStoreQuery2->getID(),serverid,\
						playerStoreInfo->getStoreCardVec(),playerStoreInfo->getStoreItemVec());
					crDBSavePacket::buildRequestPacket(packet,updatePlayerStore.get(),-1);
				}
				else
				{
					//插入记录
					insertPlayerStore->buildUpdateSql(playerStoreInfo->getPlayerId(), gameid,\
						playerStoreInfo->getStoreCardVec(), playerStoreInfo->getStoreItemVec() );
					crDBSavePacket::buildRequestPacket(packet,insertPlayerStore.get(),-1);
				}
				dbConductor->getNetManager()->sendPacket("all",packet);
				dbSession->releaseQuery();
				//elapsedTime = timer.delta_s(start_tick,timer.tick());
			}
			accountdb->endSession(dbSession.get());
		}
	}
}

/////////////////////////////////////////
//
//crJXJCreatePlayerStoreRecordDisposeMethod
//
/////////////////////////////////////////
crJXJCreatePlayerStoreRecordDisposeMethod::crJXJCreatePlayerStoreRecordDisposeMethod() :
m_this(NULL),
m_taskcount(5){}
crJXJCreatePlayerStoreRecordDisposeMethod::crJXJCreatePlayerStoreRecordDisposeMethod(const crJXJCreatePlayerStoreRecordDisposeMethod& handle) :
crMethod(handle),
m_this(NULL),
m_taskcount(handle.m_taskcount)
{
}
void crJXJCreatePlayerStoreRecordDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}

void crJXJCreatePlayerStoreRecordDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}
}

void crJXJCreatePlayerStoreRecordDisposeMethod::operator()(crHandle &handle)
{
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if (dbConductor)
	{
		m_taskVec.resize(0);
		m_taskVec.reserve(m_taskcount);
		CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreInfo;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData, 1));
		data->getParam(WCHDATA_JXJCreateGameStoreDistillDeque, param);
		PlayerStoreInfoDeq *gameStoreDistillDeque = (PlayerStoreInfoDeq *)param;
		int i = 0;
		while (!gameStoreDistillDeque->empty() && i < m_taskcount)
		{
			playerStoreInfo = gameStoreDistillDeque->front();
			gameStoreDistillDeque->pop_front();
			m_taskVec.push_back(playerStoreInfo);
			i++;
		}
		data->excHandle(MAKEINT64(WCH_LockData, 0));

		if (!m_taskVec.empty())
		{
			crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
			ref_ptr<crDataBase> dbSession = accountdb->beginSession();
			CRCore::ref_ptr<crInsertGameStoreDistillRecord > insertPlayerStoreRecordRecord = new crInsertGameStoreDistillRecord;

			crDBSavePacket packet;

			CRCore::ref_ptr<crStreamBuf> streamBuf;
			for (std::vector<CRCore::ref_ptr<crJXJPlayerStoreInfo> >::iterator titr = m_taskVec.begin();
				titr != m_taskVec.end();
				++titr)
			{
				playerStoreInfo = *titr;
								
				//插入记录
				insertPlayerStoreRecordRecord->buildUpdateSql(playerStoreInfo->getPlayerId(), gameid, playerStoreInfo->getPlayerMoney(), playerStoreInfo->getTotalMoney(), \
					playerStoreInfo->getStoreCardVec(), playerStoreInfo->getStoreItemVec());
				crDBSavePacket::buildRequestPacket(packet, insertPlayerStoreRecordRecord.get(), -1);
				
				dbConductor->getNetManager()->sendPacket("all", packet);
				dbSession->releaseQuery();
			}
			accountdb->endSession(dbSession.get());
		}
	}
}
/////////////////////////////////////////
//
//crJXJCreatePlayerStoreDisposeMethod
//
/////////////////////////////////////////
crJXJCanvasOnShowQueryMethod::crJXJCanvasOnShowQueryMethod():
	m_this(NULL){}
crJXJCanvasOnShowQueryMethod::crJXJCanvasOnShowQueryMethod(const crJXJCanvasOnShowQueryMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCanvasOnShowQueryMethod::inputParam(int i, void *param)
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

void crJXJCanvasOnShowQueryMethod::addParam(int i, const std::string& str)
{
}

void crJXJCanvasOnShowQueryMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor){
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJQueryPlayerGameStore,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJQueryPlayerGameStoreMethod
//
/////////////////////////////////////////
crJXJQueryPlayerGameStoreMethod::crJXJQueryPlayerGameStoreMethod(){}
crJXJQueryPlayerGameStoreMethod::crJXJQueryPlayerGameStoreMethod(const crJXJQueryPlayerGameStoreMethod& handle):
	crMethod(handle)
{
}
void crJXJQueryPlayerGameStoreMethod::inputParam(int i, void *param)
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

void crJXJQueryPlayerGameStoreMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJQueryPlayerGameStoreMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		//查询玩家playergamestore背包数据
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		if(dbConductor)
		{
			int playerid = m_this->getPlayerID();
			crData *playerData = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
			int gameid = callback->getGameID();
			int serverid = callback->getServerID();

			crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
			ref_ptr<crDataBase> dbSession = accountdb->beginSession();
			ref_ptr<crPlayerGameStoreQueryData> playerStoreQuery = new crPlayerGameStoreQueryData;
			playerStoreQuery->buildQuerySql(playerid,gameid);
			dbSession->executeQuery(playerStoreQuery.get());
			crDataBase::QueryResultVec playerStoreResultVec = dbSession->getQueryResult();
			dbSession->releaseQuery();
			accountdb->endSession(dbSession.get());
			if(!playerStoreResultVec.empty())
			{
				playerStoreQuery = dynamic_cast<crPlayerGameStoreQueryData *>(playerStoreResultVec[0].get());
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJPlayerStore,param);
				ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
				crData *playerStoreData = NULL;
				if(!playerStore)
				{
					playerStore = new crJXJPlayerStore;
					playerStore->setID(playerStoreQuery->getID());
					//playerStore->setStream(playerStoreQuery->getStream());
					playerStore->loadData();
					playerData->inputParam(WCHDATA_JXJPlayerStore,playerStore.get());
				}
				playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					int money = playerStoreQuery->getMoney();
					playerStoreData->inputParam(WCHDATA_Money,&money);
					int totalMoney = playerStoreQuery->getTotaladdmoney();
					playerStoreData->inputParam(WCHDATA_JXJPlayerTotalAddMoney,&totalMoney);
					playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
					JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;
					*itemVec = playerStoreQuery->getItemBagVec();
					playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
					JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;
					*cardVec = playerStoreQuery->getCardBagVec();
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));

					ref_ptr<crStreamBuf> dataStream = new crStreamBuf;
					playerStoreData->excHandle(MAKEINT64(WCH_BuildPlayerStream,dataStream.get()));
					ref_ptr<crGameServerPlayerData> playerGamedata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
					if(playerGamedata.valid())
					{
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid, WCH_JXJQueryPlayerGameStore, dataStream.get());
						gameServer->getNetManager()->sendPacket(playerGamedata->getPlayerConnectServerAddress(),packet);
					}
				}
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJPlayerStore,param);
				ref_ptr<crJXJPlayerStore>playerStore = (crJXJPlayerStore *)param;
				if(!playerStore)
				{
					playerStore = new crJXJPlayerStore;
					playerStore->setStream(NULL);
					playerStore->loadData();
					playerData->inputParam(WCHDATA_JXJPlayerStore,playerStore.get());
				}
				
				ref_ptr<crGameServerPlayerData> playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
				crData *playerStoreData = playerStore->getDataClass();
				if(playerdata.valid() && playerStoreData)
				{
					ref_ptr<crStreamBuf> dataStream = new crStreamBuf;
					playerStoreData->excHandle(MAKEINT64(WCH_BuildPlayerStream,dataStream.get()));
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid, WCH_JXJQueryPlayerGameStore, dataStream.get());
					gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
				}

				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
	else if(m_netType == GameClient_Game)
	{
		crData *playerData = m_this->getDataClass();
		//playerData->excHandle(MAKEINT64(WCH_LockData,1));
		//playerData->getParam(WCHDATA_JXJPlayerStore,param);
		ref_ptr<crJXJPlayerStore> playerStore ;//= (crJXJPlayerStore *)param;
				
		ref_ptr<crJXJPlayerStoreData> playerStoreData = new crJXJPlayerStoreData;
		CRCore::ref_ptr<CRCore::crStreamBuf> streamBuffer = new crStreamBuf;
		playerStoreData->inputParam(WCHDATA_PlayerStream,m_stream.get());
		//playerStoreData->excHandle(MAKEINT64(WCH_BuildSaveStream,streamBuffer.get()));

		playerStore = new crJXJPlayerStore;
		//playerStore->setStream(streamBuffer.get());
		playerStore->setDataClass(playerStoreData.get());

		playerData->inputParam(WCHDATA_JXJPlayerStore,playerStore.get());


		//playerData->excHandle(MAKEINT64(WCH_LockData,0));

		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if(canvas.valid())
		{
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJGetPlayerGameStoreMethod
//
/////////////////////////////////////////
crJXJGetPlayerGameStoreMethod::crJXJGetPlayerGameStoreMethod(){}
crJXJGetPlayerGameStoreMethod::crJXJGetPlayerGameStoreMethod(const crJXJGetPlayerGameStoreMethod& handle):
	crMethod(handle)
{
}
void crJXJGetPlayerGameStoreMethod::inputParam(int i, void *param)
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

void crJXJGetPlayerGameStoreMethod::addParam(int i, const std::string& str)
{
}

void crJXJGetPlayerGameStoreMethod::operator()(crHandle &handle)
{
	//1.读取数据库轮回背包数据到玩家背包、、并处理数据库轮回背包数据
	void *param;
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	int playerid = m_this->getPlayerID();
	bool bSucess = false;
	crData *gsData = crServerBrainHandle::getInstance()->getDataClass();
	if(dbConductor && gsData)
	{
		std::string logdata = "提取轮回仓库:";
		crData *playerData = m_this->getDataClass();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();
		int serverid = callback->getServerID();

		crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
		ref_ptr<crDataBase> dbSession = accountdb->beginSession();
		ref_ptr<crPlayerGameStoreQueryData> playerStoreQuery = new crPlayerGameStoreQueryData;
		playerStoreQuery->buildQuerySql(playerid,gameid);
		dbSession->executeQuery(playerStoreQuery.get());
		crDataBase::QueryResultVec playerStoreResultVec = dbSession->getQueryResult();
		dbSession->releaseQuery();
		accountdb->endSession(dbSession.get());

		if(!playerStoreResultVec.empty())
		{
			playerStoreQuery = dynamic_cast<crPlayerGameStoreQueryData *>(playerStoreResultVec[0].get());
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore>playerStore = (crJXJPlayerStore *)param;
			crData *playerStoreData = NULL;
			if(!playerStore)
			{
				playerStore = new crJXJPlayerStore;
				playerStore->setID(playerStoreQuery->getID());
				//playerStore->setStream(playerStoreQuery->getStream());
				playerStore->loadData();
				playerData->inputParam(WCHDATA_JXJPlayerStore,playerStore.get());
			}
			playerStoreData = playerStore->getDataClass();
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(playerStoreData && itemtab.valid())
			{
				playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
				int qmoney = playerStoreQuery->getMoney();
				playerStoreData->inputParam(WCHDATA_Money,&qmoney);
				int totalMoney = playerStoreQuery->getTotaladdmoney();
				playerStoreData->inputParam(WCHDATA_JXJPlayerTotalAddMoney,&totalMoney);
				short cyclecount = playerStoreQuery->getCyclecount();
				playerData->inputParam(WCHDATA_JXJPlayerCycleCount,&cyclecount);
				playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
				JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;
				*itemVec = playerStoreQuery->getItemBagVec();
				playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
				JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;
				*cardVec = playerStoreQuery->getCardBagVec();
				//playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));

				int distillmoney = 0;
				int distillTotalMoney = 0;
				CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreRecordInfo = new crJXJPlayerStoreInfo;
				playerStoreRecordInfo->setPlayerId(playerid);
				//playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
				playerStoreData->getParam(WCHDATA_Money,param);
				int *money = (int *)param;
				//领取元宝
				bool goldsuccess = false;
				MoneyChangeData moneyChangeData(*money,"轮回获得元宝");
				m_this->doEvent(WCH_MoneyChange,MAKEINT64(&moneyChangeData,&goldsuccess));
				if(goldsuccess)//领取元宝成功
				{
					playerStoreRecordInfo->setPlayerMoney(*money);
					distillmoney = *money;
					*money = 0;
				}
				playerStoreData->getParam(WCHDATA_JXJPlayerTotalAddMoney,param);
				int *totalmoney = (int *)param;
				//轮回获得礼金
				float factor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiGiftGoldFactor,0).c_str());
				int giftgold = (float)(*totalmoney) *factor;
				MoneyChangeData  moneydata;
				moneydata.first = giftgold;
				moneydata.second = "轮回获得礼金";
				m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
				playerStoreRecordInfo->setTotalMoney(*totalmoney);
				distillTotalMoney = *totalmoney;
				*totalmoney = 0;
				//轮回次数记录
				//playerData->getParam(WCHDATA_JXJPlayerCycleCount, param);
				//char cycleCount = *(char *)param;
				playerStoreRecordInfo->setPlayerCycleCount(cyclecount);

				logdata+=crArgumentParser::appVectoa(crVector3(distillmoney,distillTotalMoney,cyclecount),'|');
				///物品轮回
				typedef std::vector<int> AwardVec;
				AwardVec distillItemVec,distillCardVec;
				RewardItemVec rewardItems;
				CRCore::ref_ptr<crBagItemData> itemPtr;
				playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
				JXJStoreItemBagVec * playerStoreItemBagVec = (JXJStoreItemBagVec *)param;
				int i = 0;
				int itemid;
				crTableIO::StrVec record;
				int priceid = itemtab->getTitleIndex("回收价格");
				int cardshili = itemtab->getTitleIndex("国家");
				JXJStoreItemBagVec &itemBagRec = playerStoreRecordInfo->getStoreItemVec();
				itemBagRec.resize(playerStoreItemBagVec->size());
				for( JXJStoreItemBagVec::iterator itr = playerStoreItemBagVec->begin();
					 itr!=playerStoreItemBagVec->end();i++,++itr)
				{
					if(itr->valid() && (*itr)->getItemHasCycle() == 1)
					{
						itemid = (*itr)->getItemID();
						logdata+="道具:"+crArgumentParser::appVectoa(crVector3i((*itr)->getItemID(),(*itr)->getEquipMagic(),(*itr)->getItemCount()));
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
						{
							itemPtr = new crBagItemData;
							itemPtr->setItemID((*itr)->getItemID());
							itemPtr->setEquipMagic((*itr)->getEquipMagic());
							itemPtr->setItemCount((*itr)->getItemCount());
							rewardItems.push_back(itemPtr);
						}
						else
						{
							logdata+="已不再使用，";
						}
						itemBagRec[i] = *itr;
						*itr = NULL;
						distillItemVec.push_back(i);
					}
				}
				playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
				JXJStoreItemBagVec * playerStoreCardBagVec = (JXJStoreItemBagVec *)param;
				i = 0;
				JXJStoreItemBagVec &cardBagRec = playerStoreRecordInfo->getStoreCardVec();
				cardBagRec.resize(playerStoreCardBagVec->size());
				//处理将卡
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				float convertFactor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCardCycleFactor,vipLevel).c_str());
				//playerData->getParam(WCHDATA_JXJCardPieces,param);
				//int *curCardpieces = (int *)param;
				playerData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				unsigned char jkshili;
				int coppergot = 0;
				for( JXJStoreItemBagVec::iterator itr = playerStoreCardBagVec->begin();
					 itr!=playerStoreCardBagVec->end();i++,itr++)
				{
					if(itr->valid() && (*itr)->getItemHasCycle() == 1)
					{
						itemid = (*itr)->getItemID();
						logdata+="将卡:"+crArgumentParser::appVectoa(crVector3i((*itr)->getItemID(),(*itr)->getEquipMagic(),(*itr)->getItemCount()));
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
						{
							jkshili = (unsigned char)atoi(record[cardshili].c_str());
							if (jkshili != 2 && jkshili != 20 && jkshili != shiliid)
							{
								coppergot += atoi(record[priceid].c_str());
								logdata+="转换成将魂"+record[priceid];
							}
							else
							{
								itemPtr = new crBagItemData;
								itemPtr->setItemID((*itr)->getItemID());
								itemPtr->setEquipMagic((*itr)->getEquipMagic());
								itemPtr->setItemCount((*itr)->getItemCount());
								rewardItems.push_back(itemPtr);
							}
						}
						else
						{
							logdata+="已不再使用，";
						}
						cardBagRec[i] = *itr;
						*itr = NULL;
						distillCardVec.push_back(i);
					}
				}
				if (coppergot > 0)
				{
					coppergot = (int)(convertFactor*(float)coppergot);
					m_this->doEvent(WCH_JXJChangePlayerData,MAKEINT64(&coppergot,T_JiangHun));
					//INTLIMITADD(*curCardpieces,coppergot,INT_MAX);
				}
				//将轮回仓库内剩余不轮回数据写会数据库
				//CRCore::ref_ptr<crJXJPlayerStoreInfo> playerStoreInfo = new crJXJPlayerStoreInfo;
				//CRCore::ref_ptr<CRCore::crStreamBuf> streamBuffer = new crStreamBuf;
				////playerStoreData->excHandle(MAKEINT64(WCH_BuildSaveStream,streamBuffer.get()));
				//playerStoreInfo->setPlayerId(playerid);
				//playerStoreInfo->getStoreCardVec() = *playerStoreCardBagVec;
				//playerStoreInfo->getStoreItemVec() = *playerStoreItemBagVec;
				playerStoreData->excHandle(MAKEINT64(WCH_LockData, 0));
				
				//更新数据库
				CRCore::ref_ptr<crUpdatePlayerGameStore > updatePlayerStore = new crUpdatePlayerGameStore;
				updatePlayerStore->buildUpdateSql_Distill(playerStoreQuery->getID(),distillmoney,distillTotalMoney,distillCardVec,distillItemVec);
				crDBSavePacket packet;
				crDBSavePacket::buildRequestPacket(packet,updatePlayerStore.get(),-1);
				dbConductor->getNetManager()->sendPacket("all",packet);

				gsData->excHandle(MAKEINT64(WCH_LockData,1));
				//gsData->getParam(WCHDATA_JXJCreatePlayerStoreInfoDeque,param);
				//PlayerStoreInfoDeq *playerStoreInfoDeq = (PlayerStoreInfoDeq *)param;
				//playerStoreInfoDeq->push_back(playerStoreInfo);

				gsData->getParam(WCHDATA_JXJCreateGameStoreDistillDeque, param);
				PlayerStoreInfoDeq *playerStoreRecordInfoDeq = (PlayerStoreInfoDeq *)param;
				playerStoreRecordInfoDeq->push_back(playerStoreRecordInfo);

				gsData->excHandle(MAKEINT64(WCH_LockData,0));

				///////发放轮回物品
				m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,MAKEINT32(0,1)));

				bSucess = true;	
				//else
				//{
				//	//提醒玩家整理背包
				//	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
				//	ref_ptr<crGameServerPlayerData> playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
				//	if (playerdata.valid())
				//	{
				//		crPlayerDataEventPacket packet;
				//		crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJRecvGetPlayerStoreFail, NULL);
				//		gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(), packet);
				//	}
				//}
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		///游戏日志
		logdata += bSucess ? "成功" : "失败";
		GameLogData gamelog(Log_GetPlayerGameStore, logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
	}
}

/////////////////////////////////////////
//
//crJXJRecvGetPlayerStoreFailMethod
//
/////////////////////////////////////////
crJXJRecvGetPlayerStoreFailMethod::crJXJRecvGetPlayerStoreFailMethod(){}
crJXJRecvGetPlayerStoreFailMethod::crJXJRecvGetPlayerStoreFailMethod(const crJXJRecvGetPlayerStoreFailMethod& handle) :
crMethod(handle)
{
}
void crJXJRecvGetPlayerStoreFailMethod::inputParam(int i, void *param)
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

void crJXJRecvGetPlayerStoreFailMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvGetPlayerStoreFailMethod::operator()(crHandle &handle)
{
	if (m_netType == GameClient_Game)
	{
		int textid = 4105;
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);

		noticeHandle->inputParam(WCHDATA_NoticeTextID, &textid);
		int mode = 1;
		noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
}

/////////////////////////////////////////
//
//crJXJDoStorePlayerInfoMethod
//
/////////////////////////////////////////
crJXJDoStorePlayerInfoMethod::crJXJDoStorePlayerInfoMethod(){}
crJXJDoStorePlayerInfoMethod::crJXJDoStorePlayerInfoMethod(const crJXJDoStorePlayerInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJDoStorePlayerInfoMethod::inputParam(int i, void *param)
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

void crJXJDoStorePlayerInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = (char)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJDoStorePlayerInfoMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(canvas.valid())
	{
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			ItemOrCardMap *cardOrItemmap = NULL;
			if (m_index == 0)
			{
				canvasData->getParam(WCHDATA_JXJLunHuiCardMap, param);
				cardOrItemmap = (ItemOrCardMap *)param;
			}
			if (m_index == 1)
			{
				canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
				cardOrItemmap = (ItemOrCardMap *)param;
			}
			
			if (cardOrItemmap && !cardOrItemmap->empty())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2 + cardOrItemmap->size() * 9);
				stream->_writeChar(m_index);
				stream->_writeUChar(cardOrItemmap->size());
				for (ItemOrCardMap::iterator itr = cardOrItemmap->begin();
					itr != cardOrItemmap->end(); itr++)
				{
					stream->_writeChar(itr->first);
					stream->_writeInt(itr->second->getItemID());
					if(m_index == 1)
					{
						stream->_writeInt(itr->second->getEquipMagic());
					}
				}

				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor){
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerStore,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
				
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIAddItemDragMethod
//
/////////////////////////////////////////
crJXJUIAddItemDragMethod::crJXJUIAddItemDragMethod()
{
}
crJXJUIAddItemDragMethod::crJXJUIAddItemDragMethod(const crJXJUIAddItemDragMethod& handle):
	crMethod(handle)
{
}
void crJXJUIAddItemDragMethod::inputParam(int i, void *param)
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
		m_this = (crListControlWidgetNode*)param;
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
void crJXJUIAddItemDragMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_strImgItem[i-1] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		m_strSwIsOpen[i-7] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
		m_strImgItem[i-12] = str;
		break;
	}
}
void crJXJUIAddItemDragMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!m_this || !myPlayer) return;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	crCanvasNode *lunhuiCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	if(!m_ea || !canvas || !lunhuiCanvas) return;
	//crData *itemBagCanvasData = canvas->getDataClass();
	crData *playerData = myPlayer->getDataClass();
	if(playerData)
	{
		playerData -> getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;

		crListControlNode *dragNode = m_this->getSelectNode();
		if(!dragNode || !dragNode->getVisiable()) return;

		ref_ptr<crTableIO>itemTab = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec	record, record2;
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crWidgetNode *pickItemNode = NULL; 
		crWidgetNode *capturer = crFilterRenderManager::getInstance()->getMouseCapturer();
		crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
		crFilterRenderManager::getInstance()->mousePick(mouse, pickItemNode);
		crFilterRenderManager::getInstance()->setMouseCapturer(capturer);

		if(pickItemNode && pickItemNode->getParentCanvas() == lunhuiCanvas && itemTab.valid())
		{
			//获取playerstore中已有如果serverid相等则可以操作，否则不能操作

			bool isCanbe = true;
			for (int i = 0;i<10;i++)
			{
				if(pickItemNode->getName() == m_strImgItem[i])
				{
					/*if(i>0)
					{
					crMultiSwitch *multiSwitch = dynamic_cast<crMultiSwitch *>(lunhuiCanvas->getChildNode(m_strSwIsOpen[i-1]));
					if(multiSwitch && multiSwitch->getActiveSwitchSet()==0)
					{
					isCanbe = true;
					}
					}
					else
					{
					isCanbe = true;
					}*/
					if(isCanbe)
					{
						crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
						if(playerData)
						{
							playerData->excHandle(MAKEINT64(WCH_LockData,1));
							playerData->getParam(WCHDATA_JXJPlayerStore,param);
							crJXJPlayerStore * playerStore = (crJXJPlayerStore *)param;
							if(playerStore)
							{
								crData *playerStoreData = playerStore->getDataClass();
								if(playerStoreData)
								{
									playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
									playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
									JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;

									if(i >= itemVec->size() || \
										(i<itemVec->size() && (*itemVec)[i]->getItemServerID() == 1))
									{
										//
										//crMultiSwitch *multiswitch_level =dynamic_cast<crMultiSwitch *>(lunhuiCanvas->getChildNode(m_strSwCardLv[i]));
										crImageBoxWidgetNode *imageBox = dynamic_cast<crImageBoxWidgetNode *>(lunhuiCanvas->getChildNode(m_strImgItem[i]));
										if(imageBox)
										{
											int colorindex = itemTab->getTitleIndex("品质");
											int iconIndex = itemTab->getTitleIndex("icon");
											int maxCountIndex = itemTab->getTitleIndex("叠加数量");
											int dragIndex = dragNode->getData();
											//itemBagCanvasData->excHandle(MAKEINT64(WCH_LockData,1));
											playerData->getParam(WCHDATA_JXJItemBagVec, param);
											JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
											int item_id = (*itemBagVec)[dragIndex]->getItemID();
											int magicID = (*itemBagVec)[dragIndex]->getEquipMagic();
											if(itemTab->queryOneRecord(0, crArgumentParser::appItoa(item_id), record)>=0)
											{
												imageBox->setVisiable(true);
												imageBox->setImageName(record[iconIndex]);
												int maxCount = atoi(record[maxCountIndex].c_str());
												//存入canvasData
												crData *canvasData = lunhuiCanvas->getDataClass();
												if(canvasData)
												{
													canvasData->excHandle(MAKEINT64(WCH_LockData,1));
													canvasData->getParam(WCHDATA_JXJLunHuiItemMap,param);
													ItemOrCardMap *cardmap = (ItemOrCardMap *)param;
													canvasData->getParam(WCHDATA_JXJTempItemSwitchMap,param);
													ItemSwitchMap *switchmap = (ItemSwitchMap *)param;

													if(cardmap)
													{
														int index = 0;
														for (JXJItemBagVec::iterator itr = itemBagVec->begin();
															itr != itemBagVec->end();
															++itr, index++)
														{
															if (!itr->valid()) continue;
															if (index == dragIndex)
															{
																if (itr->get()->getItemCount() == 1)
																{
																	*itr = NULL;
																}
																else if (itr->get()->getItemCount() > 1)
																{
																	itr->get()->setItemCount(itr->get()->getItemCount() - 1);
																}
																break;
															}
														}

														//处理wupin与背包显示
														ItemOrCardMap::iterator itr_item = cardmap->find(i);
														if(itr_item!=cardmap->end())
														{
															ref_ptr<crBagItemData> temp = new crBagItemData;
															temp->setItemID(itr_item->second->getItemID());
															temp->setEquipMagic(itr_item->second->getEquipMagic());
															temp->setItemCount(1);
															////playerData->getParam(WCHDATA_JXJItemBagVec,param);
															////JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
															//bool isNeedAddNew = true;
															//
															//for (JXJItemBagVec::iterator itr = itemBagVec->begin();
															//	itr!=itemBagVec->end();
															//	++itr)
															//{
															//	if(!itr->valid()) continue;
															//	if ((*itr)->getItemID() == itr_item->second->getItemID() && \
															//		itr->get()->getItemCount()<maxCount)
															//	{
															//		itr->get()->setItemCount(itr->get()->getItemCount()+1);
															//		isNeedAddNew = false;
															//		break;
															//	}
															//}

															//if(isNeedAddNew = true)
															//{
															//	for (JXJItemBagVec::iterator itr = itemBagVec->begin();
															//		itr!=itemBagVec->end();
															//		++itr)
															//	{
															//		if (itr->get() == NULL)
															//		{
															//			*itr = temp.get();
															//			break;
															//		}
															//	}
															//}
															temp = new crBagItemData;
															temp->setItemID(item_id);
															temp->setEquipMagic(magicID);
															temp->setItemCount(1);
															(*cardmap)[i] = temp;
														}
														else
														{
															if(i<itemVec->size())
															{
																ref_ptr<crBagItemData> temp = new crBagItemData;
																temp->setItemID((*itemVec)[i]->getItemID());
																temp->setEquipMagic((*itemVec)[i]->getEquipMagic());
																temp->setItemCount(1);

																//放入临时转换背包
																switchmap->insert(std::make_pair(i,(*itemVec)[i]));

																////playerData->getParam(WCHDATA_JXJItemBagVec,param);
																////JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
																//for (JXJItemBagVec::iterator itr = itemBagVec->begin();
																//	itr!=itemBagVec->end();
																//	++itr)
																//{
																//	if ((*itr) == NULL)
																//	{
																//		*itr = temp;
																//		break;
																//	}
																//}
															}
													
															ref_ptr<crBagItemData> temp = new crBagItemData;
															temp->setItemID(item_id);
															temp->setEquipMagic(magicID);
															temp->setItemCount(1);
															cardmap->insert(std::make_pair(i,temp));
														}														
													}
													canvasData->excHandle(MAKEINT64(WCH_LockData,0));
												}
											}
											//itemBagCanvasData->excHandle(MAKEINT64(WCH_LockData,0));
										}
									}

									playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
								}
							}
							playerData->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUILunHuiCanvasCloseMethod
//
/////////////////////////////////////////
crJXJUILunHuiCanvasCloseMethod::crJXJUILunHuiCanvasCloseMethod()
{
}
crJXJUILunHuiCanvasCloseMethod::crJXJUILunHuiCanvasCloseMethod(const crJXJUILunHuiCanvasCloseMethod& handle):
	crMethod(handle)
{
}
void crJXJUILunHuiCanvasCloseMethod::inputParam(int i, void *param)
{
// 	switch(i) 
// 	{
// 	case 0:
// 		if(param == 0)
// 		{//释放
// 			m_this = NULL;
// 		}
// 		break;
// 	case 1:
// 		m_this = (crCanvasNode*)param;
// 		break;
// 	}
}
void crJXJUILunHuiCanvasCloseMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_strJKSwitch[i-1] = str;
		break;
	default:
		break;
	}
}
void crJXJUILunHuiCanvasCloseMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO>itemTable = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemTable.valid()) return;
	crTableIO::StrVec	record;
	int colorindex = itemTable->getTitleIndex("品质");
	int iconIndex = itemTable->getTitleIndex("icon");
	int maxCountIndex = itemTable->getTitleIndex("叠加数量");
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	
	crMultiSwitch * mulitSwitch = NULL;
	for (int i = 0; i < 6;i++)
	{
		mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strJKSwitch[i]));
		if (mulitSwitch)
		{
			mulitSwitch->setActiveSwitchSet(0);
		}
	}
	crData *canvasData = canvas->getDataClass();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(canvasData)
	{
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
		ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
		canvasData->getParam(WCHDATA_JXJTempCardSwitchMap,param);
		ItemSwitchMap *itemSwitchMap = (ItemSwitchMap *)param;

		canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
		ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
		canvasData->getParam(WCHDATA_JXJTempItemSwitchMap, param);
		ItemSwitchMap *cardSwitchMap = (ItemSwitchMap *)param;

		canvasData->getParam(WCHDATA_JXJPlayerLunHuiReplacecount,param);
		char *replacecount = (char *)param;

		playerData->excHandle(MAKEINT64(WCH_LockData,1));
	
		playerData->getParam(WCHDATA_JXJJiangkaBagVec, param);
		JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
		playerData->getParam(WCHDATA_JXJItemBagVec, param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		
		playerData->getParam(WCHDATA_JXJPlayerStore,param);
		crJXJPlayerStore * playerStore = (crJXJPlayerStore *)param;
		if(playerStore)
		{
			crData *playerStoreData = playerStore->getDataClass();
			if(playerStoreData)
			{
				playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
				
				playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec, param);
				JXJStoreItemBagVec * storeCardVec = (JXJStoreItemBagVec *)param;
				playerStoreData->getParam(WCHDATA_JXJItemBagVec, param);
				JXJStoreItemBagVec * storeItemVec = (JXJStoreItemBagVec *)param;
				
				//处理将卡背包数据
				if (cardbagvec && storeCardVec)
				{
					//归还bagvec
					ref_ptr<crBagItemData> temp;
					ref_ptr<crStoreBagItemData> tempStore;
					ItemOrCardMap::iterator itr_coi = cardMap->begin();

					JXJItemBagVec::iterator itr_bag;// = bagvec->begin();
					for (; itr_coi != cardMap->end(); itr_coi++)
					{
						if(itemTable->queryOneRecord(0, crArgumentParser::appItoa(itr_coi->second->getItemID()), record)>=0)
						{
							int maxCount = atoi(record[maxCountIndex].c_str());
							bool needNew = true;
							itr_bag = cardbagvec->begin();
							for (; itr_bag != cardbagvec->end(); itr_bag++)
							{
								if (!itr_bag->valid())continue;
								if((*itr_bag)->getItemID()==itr_coi->second->getItemID() && \
									itr_bag->get()->getItemCount()< maxCount)
								{
									itr_bag->get()->setItemCount(itr_bag->get()->getItemCount()+1);
									needNew = false;
									break;
								}
							}
							if(needNew)
							{
								itr_bag = cardbagvec->begin();
								for (; itr_bag != cardbagvec->end(); itr_bag++)
								{
									if((*itr_bag)==NULL)
									{
										temp = new crBagItemData;
										temp->setItemID(itr_coi->second->getItemID());
										//temp->setEquipMagic(itr_coi->second->getEquipMagic());
										temp->setItemCount(1);

										*itr_bag = temp;

										break;
									}
								}
							}
						}
					}							
				}

				//处理物品背包数据
				if (itembagvec && storeItemVec)
				{
					//归还bagvec
					ref_ptr<crBagItemData> temp;
					ref_ptr<crStoreBagItemData> tempStore;
					ItemOrCardMap::iterator itr_coi = itemMap->begin();

					JXJItemBagVec::iterator itr_bag;// = bagvec->begin();
					for (; itr_coi != itemMap->end(); itr_coi++)
					{
						if (itemTable->queryOneRecord(0, crArgumentParser::appItoa(itr_coi->second->getItemID()), record) >= 0)
						{
							int maxCount = atoi(record[maxCountIndex].c_str());
							bool needNew = true;
							itr_bag = itembagvec->begin();
							for (; itr_bag != itembagvec->end(); itr_bag++)
							{
								if (!itr_bag->valid()) continue;
								if ((*itr_bag)->getItemID() == itr_coi->second->getItemID() && \
									(*itr_bag)->getEquipMagic() == itr_coi->second->getEquipMagic() && \
									itr_bag->get()->getItemCount() < maxCount)
								{
									itr_bag->get()->setItemCount(itr_bag->get()->getItemCount() + 1);
									needNew = false;
									break;
								}
							}
							if (needNew)
							{
								itr_bag = itembagvec->begin();
								for (; itr_bag != itembagvec->end(); itr_bag++)
								{
									if ((*itr_bag) == NULL)
									{
										temp = new crBagItemData;
										temp->setItemID(itr_coi->second->getItemID());
										temp->setEquipMagic(itr_coi->second->getEquipMagic());
										temp->setItemCount(1);

										*itr_bag = temp;

										break;
									}
								}
							}
						}
					}
				}

				//清理临时数据
				cardMap->clear();
				itemMap->clear();
				itemSwitchMap->clear();
				cardSwitchMap->clear();
				*replacecount = 0;

				playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJBattleAddFriendNewMethod
//
/////////////////////////////////////////
crJXJBattleAddFriendNewMethod::crJXJBattleAddFriendNewMethod()
{
}
crJXJBattleAddFriendNewMethod::crJXJBattleAddFriendNewMethod(const crJXJBattleAddFriendNewMethod& handle):
	crMethod(handle)
{
}
void crJXJBattleAddFriendNewMethod::inputParam(int i, void *param)
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
void crJXJBattleAddFriendNewMethod::addParam(int i, const std::string& str)
{

}
void crJXJBattleAddFriendNewMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode *parentCanvas = m_this->getParentCanvas();
	if(parentCanvas)
	{
		crData *canvasData = parentCanvas->getDataClass();
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
		RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
		canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
		crInstanceItem* curSelect = (crInstanceItem *)param;

		int currentSelPlayerId = curSelect->getID();
		//检测是否为同一势力
		//检测是否在黑名单
		//检查是否为自己
		char isEnemy = 0;
		crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKEINT64(curSelect,&isEnemy));
		if(isEnemy == 1)
		{

// 			crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
// 			if(playerData)
// 			{
// 				playerData->excHandle(MAKEINT64(WCH_LockData,1));
// 				playerData->getParam(WCHDATA_JXJBlackNameSet,param);
// 				BlackNameSet *blacknameset = (BlackNameSet*)param;
				RoomMemberInfoMap::iterator itr_member = infoMap->find(currentSelPlayerId);
				if(itr_member!=infoMap->end())
				{
					std::string characterName = itr_member->second->getCharacterName();
					std::string myname = crMyPlayerData::getInstance()->getCharacterName();
					if(!characterName.empty() && myname.compare(characterName)!=0)
					{
						//BlackNameSet::iterator itr_name = blacknameset->find(characterName);
// 						if(itr_name == blacknameset->end())
// 						{
							crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(4+characterName.length());
							stream->_writeString(characterName);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGameAddFriend,stream.get());
							if(netConductor)
								netConductor->getNetManager()->sendPacket("all",packet);
// 						}
// 						else
// 						{
// 							//在黑名单中
// 						}
					}
					else
					{
						//自己
						crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4009,NULL));
					}
				}

				//playerData->excHandle(MAKEINT64(WCH_LockData,0));
			//}
		}
		else
		{
			//提示势力不一样
			crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4010,NULL));
		}
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

////////////////////////////////////////
//
//crJXJTaskBarCanvasOnShowMethod
//
/////////////////////////////////////////
crJXJTaskBarCanvasOnShowMethod::crJXJTaskBarCanvasOnShowMethod()
{
}
crJXJTaskBarCanvasOnShowMethod::crJXJTaskBarCanvasOnShowMethod(const crJXJTaskBarCanvasOnShowMethod& handle):
	crMethod(handle)
{
}
void crJXJTaskBarCanvasOnShowMethod::inputParam(int i, void *param)
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
void crJXJTaskBarCanvasOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch = str;
		break;
	case 1:
		m_strSequence = str;
		break;
	default:
		break;
	}
}
void crJXJTaskBarCanvasOnShowMethod::operator()(crHandle &handle)
{
	//void *param;
	ref_ptr<crMultiSwitch> multiSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch));
	ref_ptr<crSequence> seq = dynamic_cast<crSequence *>(m_this->getChildNode(m_strSequence));
	crData *playerdata = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerdata)
	{
		//playerdata->excHandle(MAKEINT64(WCH_LockData,1));
		//playerdata->getParam(WCHDATA_JXJCardPieces,param);
		//int *cardPiece = (int *)param;
		//playerdata->getParam(WCHDATA_JXJCardPiecesOffset,param);
		int cardPieceoffset = 0;//*(int *)param;
		if(cardPieceoffset>0)
		{
			if(multiSwitch.valid())
				multiSwitch->setActiveSwitchSet(1);
			if(seq.valid())
				seq->startEffect();

			//*cardPiece += cardPieceoffset;

			//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Exp,exp));
		}
		//playerdata->excHandle(MAKEINT64(WCH_LockData,1));
	}
}

////////////////////////////////////////
//
//crJXJGetJiangHunShowMethod
//
/////////////////////////////////////////
crJXJGetJiangHunShowMethod::crJXJGetJiangHunShowMethod()
{
}
crJXJGetJiangHunShowMethod::crJXJGetJiangHunShowMethod(const crJXJGetJiangHunShowMethod& handle):
	crMethod(handle)
{
}
void crJXJGetJiangHunShowMethod::inputParam(int i, void *param)
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
void crJXJGetJiangHunShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch = str;
		break;
	case 1:
		m_strSequence = str;
		break;
	default:
		break;
	}
}
void crJXJGetJiangHunShowMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
	if(parentCanvas.valid())
	{
		ref_ptr<crMultiSwitch> multiSwitch = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_strSwitch));
		ref_ptr<crSequence> seq = dynamic_cast<crSequence *>(parentCanvas->getChildNode(m_strSequence));
		crData *playerdata = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		if(playerdata)
		{
			playerdata->excHandle(MAKEINT64(WCH_LockData,1));
			playerdata->getParam(WCHDATA_JXJCardPieces,param);
			int *cardPiece = (int *)param;
			//playerdata->getParam(WCHDATA_JXJCardPiecesOffset,param);
			int cardPieceoffset = *(int *)param;
			if(cardPieceoffset>0)
			{
				*cardPiece += cardPieceoffset;

				//cardPieceoffset = 0;
				//playerdata->inputParam(WCHDATA_JXJCardPiecesOffset,NULL);
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_JiangHun,cardPieceoffset));
			}

			if(multiSwitch.valid())
				multiSwitch->setActiveSwitchSet(0);
			if(seq.valid())
				seq->stopEffect();

			playerdata->excHandle(MAKEINT64(WCH_LockData,0));
			int texid = 4015;
			std::string param1 = crArgumentParser::appItoa(cardPieceoffset);
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);	
		}
	}
}

////////////////////////////////////////
//
//crJXJSkillBufferUpdateMethod
//
/////////////////////////////////////////
crJXJSkillBufferUpdateMethod::crJXJSkillBufferUpdateMethod()
{
}
crJXJSkillBufferUpdateMethod::crJXJSkillBufferUpdateMethod(const crJXJSkillBufferUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJSkillBufferUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJSkillBufferUpdateMethod::addParam(int i, const std::string& str)
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
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		m_strImgBox[i / 8][i % 8] = str;
		break;
	}
}
void crJXJSkillBufferUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	bool bshow = false;
	crData *canvasData = m_this->getDataClass();
	ref_ptr<crImageBoxWidgetNode>  bufferImage[3][8];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bufferImage[i][j] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strImgBox[i][j]));
			if (bufferImage[i][j].valid()) bufferImage[i][j]->setVisiable(false);
		}
	}
	ref_ptr<crTableIO> bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);

	canvasData->excHandle(MAKEINT64(WCH_LockData, 1));
	canvasData->getParam(WCHDATA_JXJBufferTimerMap, param);
	BufferInfoMap * bufferInfoMap = (BufferInfoMap *)param;

	int index = 0;
	crTableIO::StrVec record;
	crData *widgetData = NULL;
	BufferInfoMap::iterator itr_map;
	for (int i = 0; i < 3;i++)
	{
		index = 0;
		itr_map = bufferInfoMap[i].begin();
		for (; itr_map != bufferInfoMap[i].end() && index<8;)
		{
			if (itr_map->second.second < 0.0f)
			{
				itr_map = bufferInfoMap[i].erase(itr_map);
				if (bufferImage[i][index].valid())
				{
					widgetData = bufferImage[i][index]->getDataClass();
					if (widgetData)
					{
						bool isFlash = false;
						widgetData->inputParam(WCHDATA_JXJBufferWidgetIsFlash, &isFlash);//停止
						bufferImage[i][index]->setVisiable(false);
					}
				}
			}
			else
			{
				if (bufferImage[i][index].valid() && bufferAttrTab.valid())
				{
					int iconIndex = bufferAttrTab->getTitleIndex("BufferIcon");
					std::string extraValue = crArgumentParser::appItoa(itr_map->first) + std::string("|") \
						+ crArgumentParser::appItoa(itr_map->second.first);
					if (bufferAttrTab->queryOneRecord(0, extraValue, record) >= 0 && iconIndex >= 0)
					{
						bufferImage[i][index]->setVisiable(true);
						bufferImage[i][index]->setImageName(record[iconIndex]);

						widgetData = bufferImage[i][index]->getDataClass();
						if (widgetData)
						{
// 							widgetData->getParam(WCHDATA_JXJBufferWidgetIsFlash, param);
// 							bool isFlash = *(bool *)param;

							if (itr_map->second.second <= 5.0f)
							{
								bool isFlash = true;
								widgetData->inputParam(WCHDATA_JXJBufferWidgetIsFlash, &isFlash);//开始
							}
							else
							{
								bool isFlash = false;
								widgetData->inputParam(WCHDATA_JXJBufferWidgetIsFlash, &isFlash);
							}
						}
					}
				}

				itr_map->second.second -= *m_dt;
				itr_map++;
				index++;
			}
		}
	}
	canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
}

/////////////////////////////////////////
//
//crJXJWidgetAlphaflashMethod
//
/////////////////////////////////////////
crJXJWidgetAlphaflashMethod::crJXJWidgetAlphaflashMethod() :m_curState(false), m_value(0.0f)
{
}
crJXJWidgetAlphaflashMethod::crJXJWidgetAlphaflashMethod(const crJXJWidgetAlphaflashMethod& handle) :
crMethod(handle),
m_curState(false),
m_value(0.0f)
{
}
void crJXJWidgetAlphaflashMethod::inputParam(int i, void *param)
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
		m_this = (crImageBoxWidgetNode*)param;
	}
}
void crJXJWidgetAlphaflashMethod::addParam(int i, const std::string& str)
{
}
void crJXJWidgetAlphaflashMethod::operator()(crHandle &handle)
{
	void *param;
	crData *widgetData = m_this->getDataClass();
	if (widgetData)
	{
		widgetData->getParam(WCHDATA_JXJBufferWidgetIsFlash, param);
		bool isFlash = *(bool *)param;

		if (isFlash == false && m_curState != isFlash)
		{
			//stop
			ref_ptr<crGeometry> thisGeometry = m_this->getImageGeometry();
			crVector4 & color = thisGeometry->getColor();
			thisGeometry->setColor(crVector4(color.r(), color.g(), color.b(), 1.0f));
			m_value = 0.0f;
			m_curState = isFlash;
		}
		else if (isFlash)
		{
			//start
			m_curState = isFlash;

			ref_ptr<crGeometry> thisGeometry = m_this->getImageGeometry();
			crVector4 & color = thisGeometry->getColor();
			thisGeometry->setColor(crVector4(color.r(), color.g(), color.b(), abs(sinf(m_value))));
			m_value += 1.0f;
		}
	}
}
////////////////////////////////////////
//
//crJXJAddBlackName2Method
//
/////////////////////////////////////////
crJXJAddBlackName2Method::crJXJAddBlackName2Method():
	m_this(NULL)
{
}
crJXJAddBlackName2Method::crJXJAddBlackName2Method(const crJXJAddBlackName2Method& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJAddBlackName2Method::inputParam(int i, void *param)
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
void crJXJAddBlackName2Method::addParam(int i, const std::string& str)
{
}
void crJXJAddBlackName2Method::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	void *param;
	crData *brainData = crBrain::getInstance()->getDataClass();

	if(myPlayer && netConductor && brainData)
	{
		brainData->getParam(WCHDATA_JXJAddFriendName,param);
		std::string blackname = *(std::string *)param;
		crData *data = myPlayer->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data -> getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		int blacknamemax = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBlackNameSetMax,viplv).c_str());
		data->getParam(WCHDATA_JXJBlackNameSet,param);
		BlackNameSet *blacknameset = (BlackNameSet *)param;
		BlackNameSet::iterator finditr = blacknameset->find(blackname);
		if (blacknameset->size() < blacknamemax)
		{
			if (finditr == blacknameset->end())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4 + blackname.size());
				stream->_writeString(blackname);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddBlackName,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
				blacknameset->insert(blackname);
			}
			else
			{
				myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,2084);//已在黑名单中
			}
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,2086);//黑名单已满
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
////////////////////////////////////////
//
//crJXJChangeMulitswitchValueMethod
//
/////////////////////////////////////////
crJXJChangeMulitswitchValueMethod::crJXJChangeMulitswitchValueMethod():
	m_this(NULL)
{
}
crJXJChangeMulitswitchValueMethod::crJXJChangeMulitswitchValueMethod(const crJXJChangeMulitswitchValueMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJChangeMulitswitchValueMethod::inputParam(int i, void *param)
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
void crJXJChangeMulitswitchValueMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strMulitswitch = str;
		break;
	case 1:
		m_value = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJChangeMulitswitchValueMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	if(canvas.valid())
	{
		crMultiSwitch *mulitswitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strMulitswitch));
		if(mulitswitch)
			mulitswitch->setActiveSwitchSet(m_value);
	}
}

////////////////////////////////////////
//
//crJXJBattleTalkWithFriendMethod
//
/////////////////////////////////////////
crJXJBattleTalkWithFriendMethod::crJXJBattleTalkWithFriendMethod():
	m_this(NULL)
{
}
crJXJBattleTalkWithFriendMethod::crJXJBattleTalkWithFriendMethod(const crJXJBattleTalkWithFriendMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJBattleTalkWithFriendMethod::inputParam(int i, void *param)
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
void crJXJBattleTalkWithFriendMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strChatCanvas = str;
		break;
	case 1:
		m_strMulitswitch = str.c_str();
		break;
	case 2:
		m_strChatInput = str;
		break;
	default:
		break;
	}
}
void crJXJBattleTalkWithFriendMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strChatCanvas);
	if(chatCanvas.valid() && parentCanvas.valid())
	{
		ref_ptr<crMultiSwitch>  canvasSwitch = dynamic_cast<crMultiSwitch *>(chatCanvas->getChildNode(m_strMulitswitch));
		if(canvasSwitch.valid())
			canvasSwitch->setActiveSwitchSet(1);
		crData *canvasData = chatCanvas->getDataClass();
		if(canvasData)
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJChatCanvasAutoHideTimer,param);
			float *timer = (float *)param;
			*timer = 1.0f;
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));

			ref_ptr<crEditWidgetNode> editWidget = dynamic_cast<crEditWidgetNode *>(chatCanvas->getWidget(m_strChatInput));
			if(editWidget.valid())
			{
				crData *parentcanvasData = parentCanvas->getDataClass();
				if(parentcanvasData)
				{
					CRCore::ScopedLock<crData> lock(*parentcanvasData);

					parentcanvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
					RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
					parentcanvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
					crInstanceItem* curSelect = (crInstanceItem *)param;

					//char isEnemy = 0;
					//crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKEINT64(curSelect,&isEnemy));
					//if(isEnemy == 1)
					{
						RoomMemberInfoMap::iterator itr_info = infoMap->find(curSelect->getID());
						if(itr_info!=infoMap->end())
						{
							std::string nickname = itr_info->second->getCharacterName();
							std::string myname = crMyPlayerData::getInstance()->getCharacterName();

							if(!nickname.empty() && nickname.length()<13 && myname!=nickname)
							{
								editWidget->setString(std::string("\\")+nickname+std::string(" "));
							}
						}
					}
				}				
			}
		}
	}
}

////////////////////////////////////////
//
//crJXJClearChengChiOrFubenIDMethod
//
/////////////////////////////////////////
crJXJClearChengChiOrFubenIDMethod::crJXJClearChengChiOrFubenIDMethod()
{
}
crJXJClearChengChiOrFubenIDMethod::crJXJClearChengChiOrFubenIDMethod(const crJXJClearChengChiOrFubenIDMethod& handle):
	crMethod(handle)
{
}
void crJXJClearChengChiOrFubenIDMethod::inputParam(int i, void *param)
{
}
void crJXJClearChengChiOrFubenIDMethod::addParam(int i, const std::string& str)
{
}
void crJXJClearChengChiOrFubenIDMethod::operator()(crHandle &handle)
{
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->inputParam(WCHDATA_JXJFubenOrChengchiType,NULL);
		playerData->inputParam(WCHDATA_JXJFubenOrChengchiID,NULL);
	}
}

////////////////////////////////////////
//
//crJXJRecvFubenKillInfoMethod
//
/////////////////////////////////////////
crJXJRecvFubenKillInfoMethod::crJXJRecvFubenKillInfoMethod()
{
}
crJXJRecvFubenKillInfoMethod::crJXJRecvFubenKillInfoMethod(const crJXJRecvFubenKillInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvFubenKillInfoMethod::inputParam(int i, void *param)
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
		m_this = (crRoom*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			crVector2i nettypepair = *(crVector2i *)(HIINT64(param64));
			m_netType = nettypepair[0];
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvFubenKillInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
// 	case 1:
// 		m_strSwitch[0] = str;
// 		break;
// 	case 2:
// 		m_strSwitch[1] = str;
// 		break;
// 	case 3:
// 		m_strSwitch[2] = str;
// 		break;
// 	case 4:
// 		m_strInput[0] = str;
// 		break;
// 	case 5:
// 		m_strInput[1] = str;
// 		break;
// 	case 6:
// 		m_strInput[2] = str;
// 		break;
// 	case 7:
// 		m_strSeq[0] = str;
// 		break;
// 	case 8:
// 		m_strSeq[1] = str;
// 		break;
// 	case 9:
// 		m_strSeq[2] = str;
// 		break;
	}
}
void crJXJRecvFubenKillInfoMethod::operator()(crHandle &handle)
{
	if(m_netType == GameClient_Game)
	{
		void * param;
		int npcCount = m_stream->_readInt();
		int bossCount = m_stream->_readInt();

		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);

		if(canvas.valid())
		{

			//////////////////////////////////////////////////////////////////////////
			crData *canvasData = canvas->getDataClass();
			if (canvasData)
			{
				canvasData->inputParam(WCHDATA_JXJStarInfoNPCKillCount, &npcCount);
				canvasData->inputParam(WCHDATA_JXJStarInfoBossKillCount, &bossCount);

				canvasData->excHandle(MAKEINT64(WCH_LockData, 1));
				canvasData->getParam(WCHDATA_JXJCanvasStarInfoVec, param);
				crVector3i * vec = (crVector3i *)param;
				if (vec)
				{
					for (int i = 0; i < 3;i++)
					{
						if (vec[i][1] == 1 || vec[i][2] == 2)
						{
							canvas->doEvent(WCH_UI_UpdateData);
							break;
						}
					}
				}
				canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
			}
			//////////////////////////////////////////////////////////////////////////

// 
// 			crData * playerData = m_this->getDataClass();
// 			playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
// 			unsigned short fubenid = *(unsigned short *)param;
// 
// 			int todoIndex = fubentab->getTitleIndex("完成目标");
// 			crTableIO::StrVec record;
// 			crVector3i vecTodo;
// 			if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && todoIndex>=0)
// 			{
// 				crArgumentParser::appAtoVec(record[todoIndex],vecTodo);
// 			}
// 			
// 			if(mulitSwitch[0].valid() && seq[0].valid() && npcCount>0 && vecTodo[0]>0)
// 			{
// 				if(npcCount >= vecTodo[0] && seq[0]->getMode() == crSequence::STOP)
// 				{
// 					mulitSwitch[0]->setActiveSwitchSet(1);
// 					seq[0]->setVisiable(true);
// 					seq[0]->startEffect();
// 				}
// 			}
// 			
// 			if(mulitSwitch[1].valid() && seq[1].valid() && bossCount>0 && vecTodo[1]>0)
// 			{
// 				if(bossCount >= vecTodo[1] && seq[1]->getMode() == crSequence::STOP)
// 				{
// 					mulitSwitch[1]->setActiveSwitchSet(1);
// 					seq[1]->setVisiable(true);
// 					seq[1]->startEffect();
// 				}
// 			}
// 			
// 			if(mulitSwitch[2].valid() && seq[2].valid())
// 			{
// 				mulitSwitch[2]->setActiveSwitchSet(0);
// 				seq[2]->stopEffect();
// 				seq[2]->setVanishWhenStop(true);
// 			}
// 
// 			char text[128];
// 
// 			if(vecTodo[0]>0)
// 			{
// 				if(npcCount>vecTodo[0]) npcCount = vecTodo[0];
// 				sprintf(text,"打倒敌军：%d/%d",npcCount,vecTodo[0]);
// 				if(input[0].valid() && npcCount>0)
// 					input[0]->setString(std::string(text));
// 			}
// 			
// 
// 			if(vecTodo[1]>0)
// 			{
// 				if(bossCount>vecTodo[1]) bossCount = vecTodo[1];
// 				sprintf(text,"打倒守将：%d/%d",bossCount,vecTodo[1]);
// 				if(input[1].valid() && bossCount>0)
// 					input[1]->setString(std::string(text));
// 			}
// 			
// // 			if(vecTodo[2]>0)
// // 			{
// // 				sprintf(text,"摧毁政厅：%d/%d",0,vecTodo[2]);
// // 				if(input[2].valid())
// // 					input[2]->setString(std::string(text));
// // 			}
		}
	}
}

////////////////////////////////////////
//
//crJXJChangeBattleStarUIMethod
//
/////////////////////////////////////////
crJXJChangeBattleStarUIMethod::crJXJChangeBattleStarUIMethod():
	m_this(NULL)
{
}
crJXJChangeBattleStarUIMethod::crJXJChangeBattleStarUIMethod(const crJXJChangeBattleStarUIMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJChangeBattleStarUIMethod::inputParam(int i, void *param)
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
void crJXJChangeBattleStarUIMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strMulitswitch = str;
		break;
	}
}
void crJXJChangeBattleStarUIMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	crCheckBoxWidgetNode * check = dynamic_cast<crCheckBoxWidgetNode *>(m_this);
	if(canvas.valid())
	{
		crMultiSwitch *mulitswitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strMulitswitch));
		if(mulitswitch)
		{
			//mulitswitch->setActiveSwitchSet(check->getSelect());

			ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);

			crData * playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short fubenid = *(unsigned short *)param;

			int todoIndex = fubentab->getTitleIndex("完成目标");
			crTableIO::StrVec record;
			crVector3i vecTodo;
			if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && todoIndex>=0)
			{
				crArgumentParser::appAtoVec(record[todoIndex],vecTodo);
			}

			int count = 0;
			for (int i = 0;i<3;i++)
			{
				if(vecTodo[i]>0) count++;
			}

			if(check->getSelect())
			{
				if(count == 1)
				{
					mulitswitch->setActiveSwitchSet(3);
				}
				else if(count == 2)
				{
					mulitswitch->setActiveSwitchSet(2);
				}
				else
				{
					mulitswitch->setActiveSwitchSet(0);
				}
			}
			else
			{
				mulitswitch->setActiveSwitchSet(1);
			}
		}
	}
}

////////////////////////////////////////
//
//crJXJBattleStarCanvasOnShowMethod
//
/////////////////////////////////////////
crJXJBattleStarCanvasOnShowMethod::crJXJBattleStarCanvasOnShowMethod():
	m_this(NULL)
{
}
crJXJBattleStarCanvasOnShowMethod::crJXJBattleStarCanvasOnShowMethod(const crJXJBattleStarCanvasOnShowMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJBattleStarCanvasOnShowMethod::inputParam(int i, void *param)
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
void crJXJBattleStarCanvasOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCheckBox = str;
		break;
	case 1:
		m_strMulitswitch = str;
		break;
	case 2:
		m_strSwitch[0] = str;
		break;
	case 3:
		m_strSwitch[1] = str;
		break;
	case 4:
		m_strSwitch[2] = str;
		break;
	case 5:
		m_strInput[0] = str;
		break;
	case 6:
		m_strInput[1] = str;
		break;
	case 7:
		m_strInput[2] = str;
		break;
	case 8:
		m_strSeq[0] = str;
		break;
	case 9:
		m_strSeq[1] = str;
		break;
	case 10:
		m_strSeq[2] = str;
		break;
	}
}
void crJXJBattleStarCanvasOnShowMethod::operator()(crHandle &handle)
{
	void * param;
	ref_ptr<crMultiSwitch> mulitsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strMulitswitch));
	ref_ptr<crMultiSwitch> mulitStar[3];
	ref_ptr<crStaticTextWidgetNode> input[3];
	ref_ptr<crSequence> seq[3];

	for (int i = 0;i<3;i++)
	{
		mulitStar[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch[i]));
		if(mulitStar[i].valid()) mulitStar[i]->setActiveSwitchSet(0);
		input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strInput[i]));
		if(input[i].valid()) input[i]->clearString();
		seq[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_strSeq[i]));
		if(seq[i].valid()) seq[i]->stopEffect();
	}

// 	ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
// 
// 	crData * playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
// 	playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
// 	unsigned short fubenid = *(unsigned short *)param;
// 
// 	int todoIndex = fubentab->getTitleIndex("完成目标");
// 	crTableIO::StrVec record;
// 	crVector3i vecTodo;
// 	if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && todoIndex>=0)
// 	{
// 		crArgumentParser::appAtoVec(record[todoIndex],vecTodo);
// 	}
// 
// 	char text[128];
// 	int visiableCount = 0;
// 	if(vecTodo[0]>0)
// 	{
// 		sprintf(text,"打倒敌军：%d/%d",0,vecTodo[0]);
// 		if(input[0].valid())
// 			input[0]->setString(std::string(text));
// 		visiableCount++;
// 	}
// 	
// 	if(vecTodo[1]>0)
// 	{
// 		sprintf(text,"打倒守将：%d/%d",0,vecTodo[1]);
// 		if(input[1].valid())
// 			input[1]->setString(std::string(text));
// 		visiableCount++;
// 	}
// 	
// 	if(vecTodo[2]>0)
// 	{
// 		sprintf(text,"摧毁政厅");
// 		if(input[2].valid())
// 			input[2]->setString(std::string(text));
// 		visiableCount++;
// 	}
// 	
// 	if(visiableCount == 0)
// 	{
// 		crFilterRenderManager::getInstance()->closeCanvas(m_this);
// 	}
// 	else
// 	{
// 		
// 		if (visiableCount == 1)
// 		{
// 			mulitsw->setActiveSwitchSet(2);
// 		}
// 		else if (visiableCount == 2)
// 		{
// 			mulitsw->setActiveSwitchSet(1);
// 		}
// 		

	crData *canvasData = m_this->getDataClass();
	if (canvasData)
	{
		ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		crTableIO::StrVec record;
		std::string str,strNum;

		canvasData->excHandle(MAKEINT64(WCH_LockData, 1));
		canvasData->getParam(WCHDATA_JXJCanvasStarInfoVec, param);
		crVector3i * vec = (crVector3i *)param;
		char text[128];
		int visiableCount = 0;
		for (int i = 0; i < 3;i++)
		{
			if (vec && input[i].valid())
			{
				if (vec[i]!=crVector3i(0,0,0) && \
					textTab->queryOneRecord(0, crArgumentParser::appItoa(vec[i][0]), record) >= 0)
				{
					str = record[1];
					strNum = crArgumentParser::appItoa(vec[i][2]);
					if (vec[i][1] == 1 || vec[i][1] == 2)
					{
						sprintf_s(text, 128, str.c_str(), "0", strNum.c_str());
						input[i]->setString(std::string(text));
					}
					else
					{
						input[i]->setString(str);
					}
					visiableCount++;
				}
			}
		}
		
		canvasData->excHandle(MAKEINT64(WCH_LockData, 0));

		if (visiableCount == 0)
		{
			crFilterRenderManager::getInstance()->closeCanvas(m_this);
		}
		else if (visiableCount == 1)
		{
			mulitsw->setActiveSwitchSet(2);
		}
		else if (visiableCount == 2)
		{
			mulitsw->setActiveSwitchSet(1);
		}
		
	}
}
////////////////////////////////////////
//
//crJXJBattleStarCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJBattleStarCanvasUpdateMethod::crJXJBattleStarCanvasUpdateMethod() :
m_this(NULL)
{
}
crJXJBattleStarCanvasUpdateMethod::crJXJBattleStarCanvasUpdateMethod(const crJXJBattleStarCanvasUpdateMethod& handle) :
crMethod(handle),
m_this(NULL)
{
}
void crJXJBattleStarCanvasUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJBattleStarCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch[0] = str;
		break;
	case 1:
		m_strSwitch[1] = str;
		break;
	case 2:
		m_strSwitch[2] = str;
		break;
	case 3:
		m_strInput[0] = str;
		break;
	case 4:
		m_strInput[1] = str;
		break;
	case 5:
		m_strInput[2] = str;
		break;
	case 6:
		m_strSeq[0] = str;
		break;
	case 7:
		m_strSeq[1] = str;
		break;
	case 8:
		m_strSeq[2] = str;
		break;
	}
}
void crJXJBattleStarCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crMultiSwitch> mulitSwitch[3];
	ref_ptr<crStaticTextWidgetNode> input[3];
	ref_ptr<crSequence> seq[3];

	for (int i = 0; i < 3; i++)
	{
		mulitSwitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch[i]));
		input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strInput[i]));
		seq[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_strSeq[i]));
	}
	crData *canvasData = m_this->getDataClass();
	ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	if (canvasData && textTab.valid())
	{
		canvasData->getParam(WCHDATA_JXJCanvasStarInfoVec, param);
		crVector3i * vec = (crVector3i *)param;
		canvasData->getParam(WCHDATA_JXJStarInfoNPCKillCount, param);
		int npckillCount = *(int *)param;
		canvasData->getParam(WCHDATA_JXJStarInfoBossKillCount, param);
		int bosskillCount = *(int *)param;

// 		canvasData->getParam(WCHDATA_JXJCanvasStarInfoVec, param);
// 		crVector3i * vec = (crVector3i *)param;
		if (vec)
		{
			std::string str,strnpc,strtotal;
			char text[128];
			crTableIO::StrVec record;
			for (int i = 0; i < 3; i++)
			{
				if (vec && input[i].valid())
				{
					if (vec[i] != crVector3i(0, 0, 0) && \
						textTab->queryOneRecord(0, crArgumentParser::appItoa(vec[i][0]), record) >= 0)
					{
						str = record[1];

						if (vec[i][1] == 1)
						{
							strnpc = crArgumentParser::appItoa(npckillCount);
							strtotal = crArgumentParser::appItoa(vec[i][2]);
							sprintf_s(text, 128, str.c_str(), strnpc.c_str(), strtotal.c_str());
							input[i]->setString(std::string(text));
							if (npckillCount >= vec[i][2] && seq[i]->getMode() == crSequence::STOP)
							{
								mulitSwitch[i]->setActiveSwitchSet(1);
								seq[i]->setVisiable(true);
								seq[i]->startEffect();
							}
						}
						else if (vec[i][1] == 2)
						{
							strnpc = crArgumentParser::appItoa(bosskillCount);
							strtotal = crArgumentParser::appItoa(vec[i][2]);

							sprintf_s(text, 128, str.c_str(), strnpc.c_str(), strtotal.c_str());
							input[i]->setString(std::string(text));
							if (bosskillCount >= vec[i][2] && seq[i]->getMode() == crSequence::STOP)
							{
								mulitSwitch[i]->setActiveSwitchSet(1);
								seq[i]->setVisiable(true);
								seq[i]->startEffect();
							}
						}
						else
						{
							input[i]->setString(str);
						}
					}
				}
			}
		}
	}
}
////////////////////////////////////////
//
//crJXJBattleStarCanvasInitMethod
//
/////////////////////////////////////////
crJXJBattleStarCanvasInitMethod::crJXJBattleStarCanvasInitMethod()
{
}
crJXJBattleStarCanvasInitMethod::crJXJBattleStarCanvasInitMethod(const crJXJBattleStarCanvasInitMethod& handle) :
crMethod(handle)
{
}
void crJXJBattleStarCanvasInitMethod::inputParam(int i, void *param)
{
}
void crJXJBattleStarCanvasInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		crArgumentParser::appAtoVec(str, m_vec[0]);
		break;
	case 2:
		crArgumentParser::appAtoVec(str, m_vec[1]);
		break;
	case 3:
		crArgumentParser::appAtoVec(str, m_vec[2]);
		break;
	}
}
void crJXJBattleStarCanvasInitMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	if (canvas)
	{
		crData *canvasData = canvas->getDataClass();
		if (canvasData)
		{
			ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crData * playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			playerData->getParam(WCHDATA_JXJFubenOrChengchiID, param);
			unsigned short fubenid = *(unsigned short *)param;
			int todoIndex = fubentab->getTitleIndex("完成目标");
			crTableIO::StrVec record;
			crVector3i vecTodo;
			if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && todoIndex>=0)
			{
			 	crArgumentParser::appAtoVec(record[todoIndex],vecTodo);

				canvasData->excHandle(MAKEINT64(WCH_LockData, 1));

				canvasData->getParam(WCHDATA_JXJCanvasStarInfoVec, param);
				crVector3i * vec = (crVector3i *)param;
				if (vec)
				{
					for (int i = 0; i < 3;i++)
					{
						vec[i][0] = m_vec[i][0];
						vec[i][1] = m_vec[i][1];
						if (m_vec[i][1] == 1)
						{
							vec[i][2] = vecTodo[0];
						}
						else if (m_vec[i][1] == 2)
						{
							vec[i][2] = vecTodo[1];
						}
					}
				}

				canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
			}			

			crFilterRenderManager::getInstance()->showCanvas(canvas, true);
		}
	}
}
////////////////////////////////////////
//
//crJXJSetShiLiWidgetEnableMethod
//
/////////////////////////////////////////
crJXJSetShiLiWidgetEnableMethod::crJXJSetShiLiWidgetEnableMethod():
	m_this(NULL),
	m_visiable(true)
{
}
crJXJSetShiLiWidgetEnableMethod::crJXJSetShiLiWidgetEnableMethod(const crJXJSetShiLiWidgetEnableMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_visiable(true)
{
}
void crJXJSetShiLiWidgetEnableMethod::inputParam(int i, void *param)
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
void crJXJSetShiLiWidgetEnableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strbt[0] = str;
		break;
	case 1:
		m_strbt[1] = str;
		break;
	case 2:
		m_strbt[2] = str;
		break;
	case 3:
		m_visiable = atoi(str.c_str());
		break;
	}
}
void crJXJSetShiLiWidgetEnableMethod::operator()(crHandle &handle)
{
	crButtonWidgetNode *bt[3] = {NULL};
	for (int i = 0;i<3;i++)
	{
		bt[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strbt[i]));
		if(bt[i])
			bt[i]->setEnable(m_visiable);
	}	
}

////////////////////////////////////////
//
//crJXJFubenRushItemShowMethod
//
/////////////////////////////////////////
crJXJFubenRushItemShowMethod::crJXJFubenRushItemShowMethod():
	m_this(NULL)
{
}
crJXJFubenRushItemShowMethod::crJXJFubenRushItemShowMethod(const crJXJFubenRushItemShowMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJFubenRushItemShowMethod::inputParam(int i, void *param)
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
void crJXJFubenRushItemShowMethod::addParam(int i, const std::string& str)
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
		m_strNameInput[i] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		m_strCountInput[i-7] = str;
		break;
	}
}
void crJXJFubenRushItemShowMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	crScrollBarWidgetNode * scrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_this);
	if(canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode> name[7];
		ref_ptr<crStaticTextWidgetNode> count[7];
		for (int i = 0;i<7;i++)
		{
			name[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strNameInput[i]));
			if(name[i].valid()) name[i]->clearString();
			count[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strCountInput[i]));
			if(count[i].valid()) count[i]->clearString();
		}

		float barvalue = scrollBar->getValue();

		crData * canvasData = canvas->getDataClass();
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(canvasData && itemtab.valid())
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJFubenRushRewardItemMap,param);
			FubenRushItemMap *itemmap = (FubenRushItemMap *)param;

			int index = int((float)(itemmap->size()-7) * barvalue);
			if(index<0)index = 0;
			int i = 0;
			int itemid = 0;
			_crInt32 equipmagic = 0;
			std::string itemName;
			std::string itemCount;
			crTableIO::StrVec itemrecord;
			int nameIndex = itemtab->getTitleIndex("name");
			int qualityIndex = itemtab->getTitleIndex("品质");
			ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
			int colorIndex = qualitytab->getTitleIndex("color");

			FubenRushItemMap::iterator itr_item = itemmap->begin();
			for (;itr_item!=itemmap->end();itr_item++,i++)
			{
				if(i<index) continue;
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itr_item->first),itemrecord)>=0)
				{
					if(nameIndex >= 0 && i-index < 7)
					{
						itemName = itemrecord[nameIndex];
						itemCount = string("*")+crArgumentParser::appItoa(itr_item->second);
						if (qualityIndex > 0 && colorIndex > 0)
						{
							crTableIO::StrVec quality;
							CRCore::crVector4 colorVec;
							if (qualitytab->queryOneRecord(0, itemrecord[qualityIndex].c_str(), quality)>=0)
							{
								crArgumentParser::appAtoVec(quality[colorIndex], colorVec);
								colorVec.x() = colorVec.x()/255.0f;
								colorVec.y() = colorVec.y()/255.0f;
								colorVec.z() = colorVec.z()/255.0f;
								colorVec.w() = colorVec.w()/255.0f;
								if(name[i-index].valid())
								{
									name[i-index]->setColor(colorVec);
									name[i-index]->setString(itemName);
									name[i-index]->setVisiable(true);
								}
								if(count[i-index].valid())
								{
									count[i-index]->setColor(colorVec);
									count[i-index]->setString(itemCount);
									count[i-index]->setVisiable(true);
								}
							}
						}
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJUIBattleInspireMethod
//////////////////////////////////////////////////////////////////////////
crJXJUIBattleInspireMethod::crJXJUIBattleInspireMethod():
	m_this(NULL),
	m_itemId(0)
{}
crJXJUIBattleInspireMethod::crJXJUIBattleInspireMethod(const crJXJUIBattleInspireMethod& handle)
{

}
void crJXJUIBattleInspireMethod::inputParam(int i, void *param)
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
void crJXJUIBattleInspireMethod::addParam(int i, const std::string& str)
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
	case 4:
		m_itemId = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIBattleInspireMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	crMyPlayerData *me = crMyPlayerData::getInstance();
	int inspireCost = 0;
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			playerData->getParam(WCHDATA_JXJInspireLevel,param);
			unsigned char inspireLv = *(unsigned char *)param;

			inspireCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireCost, inspireLv+1).c_str());
		}
	}
	if(!msgCanvas)return;
	std::string strInfo ;
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	if(inspireCost>0)
	{
		int hinttype = JXJGoldCostType_BattleInspire;
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

		crData * data = me->getPlayerGameData()->getDataClass();
		data->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_BattleInspire;
		if (ifhint)
		{
			//检测背包里现有的鼓舞令数量
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJItemBagVec, param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			int itemcount = 0;
			for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
				itemitr != itembagvec->end();
				++itemitr)
			{
				if (itemitr->get())
				{
					if (m_itemId == itemitr->get()->getItemID())
					{
						itemcount = (int)itemitr->get()->getItemCount();
						break;
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			//str = "消耗元宝" + crArgumentParser::appItoa(inspireCost) + "。\n提升血量攻击力" + crArgumentParser::appItoa(inspireExtra/10.0f) + "%";
			char tmpText[32];
			if (itemcount >= 1)
			{
				sprintf(tmpText, "1个奋进鼓\0");
			}
			else
			{
				sprintf(tmpText, "%d礼金/元宝\0",inspireCost);
			}

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
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", tmpText);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			cfg_script.Add("Text","进行战场鼓舞吗？");
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

//////////////////////////////////////////////////////////////////////////
//crJXJChatChannelDisableMethod
//////////////////////////////////////////////////////////////////////////
crJXJChatChannelDisableMethod::crJXJChatChannelDisableMethod()
{}
crJXJChatChannelDisableMethod::crJXJChatChannelDisableMethod(const crJXJChatChannelDisableMethod& handle)
{

}
void crJXJChatChannelDisableMethod::inputParam(int i, void *param)
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
void crJXJChatChannelDisableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCountryBt = str;
		break;
	case 1:
		m_strJunTuanBt = str;
		break;
	case 2:
		m_strCountryRd = str;
		break;
	case 3:
		m_strJunTuanRd = str;
		break;
	}
}
void crJXJChatChannelDisableMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crButtonWidgetNode> btCountry = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strCountryBt));
	ref_ptr<crButtonWidgetNode> btJunTuan = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strJunTuanBt));

	ref_ptr<crRadioWidgetNode>  rdCountry = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_strCountryRd));
	ref_ptr<crRadioWidgetNode>  rdJunTuan = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_strJunTuanRd));
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer){
		crData *playerData = myPlayer->getDataClass();
		if(playerData){
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myShiliid = *(unsigned char *)param;

			bool isLoseCountry = false;
			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myShiliid));
			if(isLoseCountry)
			{
				if(btCountry.valid())
					btCountry->setEnable(false);
				if(btJunTuan.valid())
					btJunTuan->setEnable(false);
				if(rdCountry.valid())
					rdCountry->setEnable(false);
				if(rdJunTuan.valid())
					rdJunTuan->setEnable(false);
			}
			else
			{
				if(btCountry.valid())
					btCountry->setEnable(true);
				if(btJunTuan.valid())
					btJunTuan->setEnable(true);
				if(rdCountry.valid())
					rdCountry->setEnable(true);
				if(rdJunTuan.valid())
					rdJunTuan->setEnable(true);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJServerUseJunlingCardMethod
//
/////////////////////////////////////////
crJXJServerUseJunlingCardMethod::crJXJServerUseJunlingCardMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseJunlingCardMethod::crJXJServerUseJunlingCardMethod(const crJXJServerUseJunlingCardMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseJunlingCardMethod::inputParam(int i, void *param)
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

void crJXJServerUseJunlingCardMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseJunlingCardMethod::operator()(crHandle &handle)
{
	if(m_user && m_itemid > 0)
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			ref_ptr<crTableIO>vipCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunlingCardTab);
			crTableIO::StrVec record;
			if(vipCardTab->queryOneRecord(0,crArgumentParser::appItoa(m_itemid),record)>=0)
			{
				int countid = vipCardTab->getTitleIndex("count");
				int count = atoi(record[countid].c_str()) * 100;
				
				void *param;
				crData *data = m_user->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				unsigned short limit = _UI16_MAX;//(unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxJunling,viplv).c_str()));
				data->getParam(WCHDATA_JXJJunling,param);
				unsigned short * junling = (unsigned short *)param;

				
				if(count<=limit && *junling <= limit-count)
				{
					*junling += count;
					*m_useResult = 1;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(2);
					stream->_writeUShort(*junling);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvJunling,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				else
				{
					*m_useResult = USE_JunlingCard_Error;
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseAddFreeCountItemMethod
//
/////////////////////////////////////////
crJXJServerUseAddFreeCountItemMethod::crJXJServerUseAddFreeCountItemMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL),
	m_index(0){}
crJXJServerUseAddFreeCountItemMethod::crJXJServerUseAddFreeCountItemMethod(const crJXJServerUseAddFreeCountItemMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL),
	m_index(handle.m_index)
{
}
void crJXJServerUseAddFreeCountItemMethod::inputParam(int i, void *param)
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

void crJXJServerUseAddFreeCountItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());//0,免费抽将卡次数，1，免费精炼次数
		break;
	}
}

void crJXJServerUseAddFreeCountItemMethod::operator()(crHandle &handle)
{
	if(m_user && m_itemid > 0)
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			ref_ptr<crTableIO>vipCardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunlingCardTab);
			crTableIO::StrVec record;
			void *param;
			if(vipCardTab->queryOneRecord(0,crArgumentParser::appItoa(m_itemid),record)>=0)
			{
				int countid = vipCardTab->getTitleIndex("count");
				int count = atoi(record[countid].c_str()) * 100;
				crData *data = m_user->getDataClass();
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;

				unsigned short limit = 0;
				int dataId = 0;
				int messageId = 0;
				char errorCode = *m_useResult;
				switch (m_index)
				{
				case 0:
					dataId = WCHDATA_JXJRollCardFreeCount;
					limit = _UI16_MAX;//(unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountMax,viplv).c_str()));
					messageId = WCH_JXJRecvRollCardFreeCountProduce;
					errorCode = USE_AddFreeRollCardCount_Error;
					break;
				case 1:
					dataId = WCHDATA_JXJRefineEquipFreeCount;
					limit = _UI16_MAX;//(unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipFreeCountMax,viplv).c_str()));
					messageId = WCH_JXJRecvRefineFreeCountProduce;
					errorCode = USE_AddFreeRefineEquipCount_Error;
					break;
				}
				if(limit>0 && dataId>0 && messageId>0)
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(dataId,param);
					unsigned short *freecount = (unsigned short *)param;//注意此处数据类型unsigned char

					if(count<=limit && *freecount <= limit-count)
					{
						*freecount += count;
						*m_useResult = 1;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeUShort(*freecount);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,messageId,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					else
					{
						*m_useResult = errorCode;
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crBigMapMoveToMyCityPosMethod
//
//////////////////////////////////////////////////////////////////////////
crBigMapMoveToMyCityPosMethod::crBigMapMoveToMyCityPosMethod():
	m_this(NULL)
{

}
crBigMapMoveToMyCityPosMethod::crBigMapMoveToMyCityPosMethod(const crBigMapMoveToMyCityPosMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crBigMapMoveToMyCityPosMethod::inputParam(int i, void *param)
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
void crBigMapMoveToMyCityPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strBigMapImage = str;
		break;
	case 1:
		m_strOtherMapCanvas = str;
		break;
	case 2:
		m_strOtherMapImage = str;
		break;
	case 3:
		m_strFrame = str;
		break;
	default:
		break;
	}
}
void crBigMapMoveToMyCityPosMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>  miniMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strOtherMapCanvas);
	//ref_ptr<crCanvasNode>  bigMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strBigMapCanvas);
	if(!miniMapCanvas.valid()) return;
	crData *canvasData = m_this->getDataClass();
	if(!canvasData) return;
	ref_ptr<crImageBoxWidgetNode> miniimagebox = dynamic_cast<crImageBoxWidgetNode *>(miniMapCanvas->getWidget(m_strOtherMapImage));
	ref_ptr<crWidgetNode> bigimagebox = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_strBigMapImage));
	ref_ptr<crWidgetNode>  frameWeidget = miniMapCanvas->getWidget(m_strFrame);
	if(!frameWeidget.valid()) 
	{
		canvasData->getParam(WCHDATA_JXJMiniMapFrameWidget,param);
		frameWeidget = (crWidgetNode *)param;
	}
	if(!miniimagebox.valid() || !bigimagebox.valid() || !frameWeidget.valid()) return;

	const crBoundingBox& bigmapbox = bigimagebox->getBoundBox();
	const crBoundingBox& minimapImageboxBox = miniimagebox->getBoundBox();
	crVector3 minimapCenter = minimapImageboxBox.center();
	const crBoundingBox& frameBox = frameWeidget->getBoundBox();
	
	crVector2 curOffset;
	canvasData->getParam(WCHDATA_JXJCityCenterOffset,param);
	crVector2 & oriOffset = *(crVector2 *)param;

	const float EPSINON = 0.000001;
	if ((oriOffset[0] >= - EPSINON && oriOffset[0] <= EPSINON) && ( oriOffset[1] >= - EPSINON && oriOffset[1] <= EPSINON ))
	{
		ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crTableIO::StrVec record;
		//获取城池id
		crPlayerGameData *me = crMyPlayerData::getInstance()->getPlayerGameData();
		if(me && chengchiTab.valid())
		{
			int buttonid = chengchiTab->getTitleIndex("buttonname");
			crData * playerData = me->getDataClass();
			if(playerData)
			{
				playerData->getParam(WCHDATA_JXJChengchiID,param);
				unsigned short mychengchiID = *(unsigned short *)param;
				ref_ptr<crTableIO> playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				if(playershilitab.valid())
				{
					playerData->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myshiliid = *(unsigned char *)param;
					crTableIO::StrVec record;
					int guoduIndex = playershilitab->getTitleIndex("国都");
					//int peiduIndex = playershilitab->getTitleIndex("陪都");
					if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(myshiliid),record)>=0 && guoduIndex>=0)
					{
						unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
						playerData->getParam(WCHDATA_JXJChengChiMap,param);
						ChengchiMap *chengchimap = (ChengchiMap *)param;
						ChengchiMap::iterator ccmitr = chengchimap->find(guoduid);
						if (ccmitr != chengchimap->end() && ccmitr->second.valid())
						{
							if(ccmitr->second->getChengzhuShili() == myshiliid)
							{
								mychengchiID = guoduid;
							}
						}
					}
				}
				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(mychengchiID),record)>=0 && buttonid>=0)
				{
					crRadioWidgetNode * bt = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(record[buttonid]));
					if(bt)
					{
						const crBoundingBox& btBox = bt->getBoundBox();
						curOffset[0] = -btBox.center().x();
						curOffset[1] = -btBox.center().y();

						canvasData->getParam(WCHDATA_JXJCityMapFrameScale,param);
						crVector3 & framescale = *(crVector3 *)param;

						canvasData->getParam(WCHDATA_JXJCityMapOffsetMax,param);
						crVector2 maxoffset = *(crVector2 *)param;

						crVector2 scaleVec(minimapImageboxBox.xLength()/bigmapbox.xLength(),minimapImageboxBox.yLength()/bigmapbox.yLength());
						crVector3 frameOffset(-curOffset.x()*scaleVec.x(),-curOffset.y()*scaleVec.y(),0.0f);

						if(curOffset[0]<-maxoffset[0])
							curOffset[0] = -maxoffset[0];
						else if(curOffset[0]>maxoffset[0])
							curOffset[0] = maxoffset[0];
						if(curOffset[1]<-maxoffset[1])
							curOffset[1] = -maxoffset[1];
						else if(curOffset[1]>maxoffset[1])
							curOffset[1] = maxoffset[1];

						m_this->setMatrix(crMatrix::translate(curOffset[0],curOffset[1],0.0f));
						frameWeidget->setMatrix(crMatrix::scale(framescale) * crMatrix::translate(frameOffset[0],frameOffset[1],0.0f));
					}
				}
			}
		}
	}
	else
	{
		curOffset = oriOffset;
	}
	canvasData->inputParam(WCHDATA_JXJCityCenterOffset,&curOffset);
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJXingjiTipsMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJXingjiTipsMethod::crJXJXingjiTipsMethod():
	m_this(NULL),
	m_index(0)
{

}
crJXJXingjiTipsMethod::crJXJXingjiTipsMethod(const crJXJXingjiTipsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{

}
void crJXJXingjiTipsMethod::inputParam(int i, void *param)
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
void crJXJXingjiTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strTipsCanvas = str;
		break;
	case 1:
		m_strinput = str;
		break;
	case 2:
		m_strSw = str;
		break;
	case 3:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJXingjiTipsMethod::operator ()(crHandle &handle)
{
	void * param;
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strTipsCanvas);
	crCanvasNode *parentCanvas = m_this->getParentCanvas();
	crPlayerGameData * me = crMyPlayerData::getInstance()->getPlayerGameData();
	if(canvas.valid() && parentCanvas&& me)
	{
		crData *playerData = me->getDataClass();
		crData * canvasData = parentCanvas->getDataClass();
		ref_ptr<crMultiSwitch> mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strSw));
		if(mulitSwitch.valid()) mulitSwitch->setActiveSwitchSet(2);
		ref_ptr<crHypertextWidgetNode> textInput = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_strinput));
		if(textInput.valid() && playerData && canvasData)
		{
			//////////////////////////////////////////////////////////////////////////
			//playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			
			//在此处添加副本id获取代码
			//////////////////////////////////////////////////////////////////////////
			canvasData->getParam(WCHDATA_JXJCanvasFubenIDVec,param);
			std::vector<int> &fubenidvec = *(std::vector<int> *)param;
			if(m_index<fubenidvec.size())
			{
				unsigned short fubenid = fubenidvec[m_index];
				if(fubenid!=0)
				{
					crTableIO::DataVec recordVec;
					ref_ptr<crTableIO>fubenRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
					fubenRewardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),recordVec);
					int copperIndex = fubenRewardTab->getTitleIndex("铜钱");
					int expIndex = fubenRewardTab->getTitleIndex("经验");
					int gongxunIndex = fubenRewardTab->getTitleIndex("功勋");
					int giftgoldIndex = fubenRewardTab->getTitleIndex("礼券");

					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					int textindex = 1;

					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					string text;
					for (int index = 1;index<4;index++)
					{
						std::string rewardText;
						if(atoi(recordVec[index][copperIndex].c_str())>0)
							rewardText+=recordVec[index][copperIndex]+std::string("铜钱 ");
						if(atoi(recordVec[index][expIndex].c_str())>0)
							rewardText+=recordVec[index][expIndex]+std::string("经验 ");
						if(atoi(recordVec[index][gongxunIndex].c_str())>0)
							rewardText+=recordVec[index][gongxunIndex]+std::string("功勋 ");
						if(atoi(recordVec[index][giftgoldIndex].c_str())>0)
							rewardText+=recordVec[index][giftgoldIndex]+std::string("礼券 ");

						switch (index)
						{
						case 1:
							text+=std::string("一星可获得掉落物品和:");
							text+=rewardText;
							//text+=std::string(",另可获得掉落物品!");
							break;
						case 2:
							text+=std::string("首次二星额外获得:");
							text+=rewardText;
							break;
						case 3:
							text+=std::string("首次三星额外获得:");
							text+=rewardText;
							break;
						}
					}

					cfg_script.Add("Content");
					cfg_script.Push("Content",textindex++);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("Text",text);
					cfg_script.Pop();

					cfg_script.Pop();

					textInput->setHypertext(cfg_script);

					const crBoundingBox & bbox = m_this->getBoundBox();
					const crBoundingBox & canvasBox = canvas->getBoundBox();

					canvas->setMatrix(crMatrix::translate(bbox.center().x(),bbox.center().y()-canvasBox.yLength()*0.5f,0.0f));
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCreateCountryLoseNoticeMethod
//
/////////////////////////////////////////
crJXJCreateCountryLoseNoticeMethod::crJXJCreateCountryLoseNoticeMethod():m_shiliid(0),m_isCycle(false){}
crJXJCreateCountryLoseNoticeMethod::crJXJCreateCountryLoseNoticeMethod(const crJXJCreateCountryLoseNoticeMethod& handle):
	crMethod(handle),
	m_shiliid(handle.m_shiliid),
	m_isCycle(handle.m_isCycle)
{
}
void crJXJCreateCountryLoseNoticeMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_shiliid = LOINT64(param64);
			m_isCycle = *(bool *)(HIINT64(param64));
		}
		else
		{
			m_shiliid = 0;
			m_isCycle = false;
		}
		break;
	}
}

void crJXJCreateCountryLoseNoticeMethod::addParam(int i, const std::string& str)
{
}

void crJXJCreateCountryLoseNoticeMethod::operator()(crHandle &handle)
{
	if(m_this!=NULL && m_shiliid!=0)
	{
		crData *gsbraindata = m_this->getDataClass();
		if(gsbraindata)
		{
			std::string strShiliName,strInfo;
			crTableIO::StrVec record;
			void *param;
			
			gsbraindata->getParam(WCHDATA_JXJShiliWei,param);
			crJXJShili *shiliWei = (crJXJShili *)param;
			gsbraindata->getParam(WCHDATA_JXJShiliShu,param);
			crJXJShili *shiliShu = (crJXJShili *)param;
			gsbraindata->getParam(WCHDATA_JXJShiliWu,param);
			crJXJShili *shiliWu = (crJXJShili *)param;
			int weiID = shiliWei->getID();
			int shuID = shiliShu->getID();
			int wuID = shiliWu->getID();

			if(m_shiliid == weiID)
			{
				strShiliName = "魏";
			}
			else if(m_shiliid == shuID)
			{
				strShiliName = "蜀";
			}
			else if(m_shiliid == wuID)
			{
				strShiliName = "吴";
			}
			
			// 在魏蜀吴三国争霸中Y（国家名）国已经灭国，三国鼎立的时代已经结束！
			// 恭贺X国（国家名）一统天下,游戏进入轮回阶段!
			std::vector<float>v_i;
			ItemMap itemMap;
			int index = 1;
			rcfg::ConfigScript cfg_script(&itemMap);
			if(m_isCycle)
			{
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Push("Character",16);
				cfg_script.Add("Text","恭贺");
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Add("Text",strShiliName);
				cfg_script.Pop();


				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Add("Text","国一统天下,游戏进入轮回阶段!");
				cfg_script.Pop();

				cfg_script.Pop();
			}
			else
			{
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Push("Character",16);
				cfg_script.Add("Text","在魏蜀吴三国争霸中");
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Add("Text",strShiliName);
				cfg_script.Pop();


				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Add("Text","国已经灭国，三国鼎立的时代已经结束！");
				cfg_script.Pop();

				cfg_script.Pop();
			}
			
			ref_ptr<crStreamBuf> streamBuffer = new crStreamBuf;
			crGlobalHandle::getInstance()->getScriptStream(cfg_script,streamBuffer);

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(streamBuffer->getRemainSize());
			stream->write(streamBuffer->getPtr(),streamBuffer->getRemainSize());

			crPlayerDataEventPacket packetpk;
			crPlayerDataEventPacket::buildReplyPacket(packetpk,0,WCH_JXJRecvClientNotice,stream.get());
			stream->seekBegin();
			//gsbraindata->inputParam(WCHDATA_JXJCenterNotice,stream.get());
			crStreamBuf *pkgstream = packetpk.getStreamBuf();
			int size = pkgstream->getBufSize();
			ref_ptr<crGameServerPlayerData> recvPlayerData;
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crNetManager *netManager = gameServer->getNetManager();
			if(netDataManager && netManager)
			{
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();
					++itr )
				{
					recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if (recvPlayerData.valid())
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(recvPlayerData->getPlayerID());
						pkgstream->setBufSize(size);
						gameServer->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packetpk);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJChangeChengchiIDMethod
//
/////////////////////////////////////////
crJXJChangeChengchiIDMethod::crJXJChangeChengchiIDMethod(){}
crJXJChangeChengchiIDMethod::crJXJChangeChengchiIDMethod(const crJXJChangeChengchiIDMethod& handle) :
	crMethod(handle)
{
}
void crJXJChangeChengchiIDMethod::inputParam(int i, void *param)
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

void crJXJChangeChengchiIDMethod::addParam(int i, const std::string& str)
{
}

void crJXJChangeChengchiIDMethod::operator()(crHandle &handle)
{
	if(m_netType == GameClient_Game)
	{
		unsigned short chengchiid = m_stream->_readUShort();
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->inputParam(WCHDATA_JXJChengchiID, &chengchiid);
		}
	}
}

/////////////////////////////////////////
//
//crJXJPlayerChengchiIDCheckMethod
//
/////////////////////////////////////////
crJXJPlayerChengchiIDCheckMethod::crJXJPlayerChengchiIDCheckMethod(){}
crJXJPlayerChengchiIDCheckMethod::crJXJPlayerChengchiIDCheckMethod(const crJXJPlayerChengchiIDCheckMethod& handle) :
	crMethod(handle)
{
}
void crJXJPlayerChengchiIDCheckMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
		break;
	}
}

void crJXJPlayerChengchiIDCheckMethod::addParam(int i, const std::string& str)
{
}

void crJXJPlayerChengchiIDCheckMethod::operator()(crHandle &handle)
{
	void * param;
	crData * playerData = m_this->getDataClass();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameServer);
	if (playerData && netConductor)
	{
		playerData->getParam(WCHDATA_JXJShiliID, param);
		unsigned char shili = *(unsigned char *)param;
		playerData->getParam(WCHDATA_JXJChengchiID, param);
		unsigned short *mychengchiID = (unsigned short *)param;

		//playerData->getParam(WCHDATA_JXJChengChiMap, param);
		//ChengchiMap *chengchiMap = (ChengchiMap *)param;
		//unsigned short tempID = *mychengchiID;
		bool changed = false;
		ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		if (playershilitab.valid())
		{
			crTableIO::StrVec record;
			int guoduIndex = playershilitab->getTitleIndex("国都");
			int peiduIndex = playershilitab->getTitleIndex("陪都");

			if (playershilitab->queryOneRecord(0, crArgumentParser::appItoa(shili), record) >= 0 && guoduIndex >= 0 && peiduIndex >= 0)
			{
				unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
				unsigned short peiduid = (unsigned short)(atoi(record[peiduIndex].c_str()));

				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *chengchiMap = (ChengchiMap *)param;

				ChengchiMap::iterator itr_curchengchi = chengchiMap->find(*mychengchiID);
				if (*mychengchiID == peiduid || 
					(itr_curchengchi != chengchiMap->end() && \
					itr_curchengchi->second->getChengzhuShili() != shili))
				{//我所在驻地是陪都，或者驻地丢失
					ChengchiMap::iterator itr_guodu = chengchiMap->find(guoduid);
					if (itr_guodu != chengchiMap->end() && itr_guodu->second->getChengzhuShili() == shili)
					{//如果国都是我的
						*mychengchiID = guoduid;
						changed = true;
					}
					else
					{
						ChengchiMap::iterator itr_peidu = chengchiMap->find(peiduid);
						if (*mychengchiID == guoduid && itr_peidu != chengchiMap->end() \
							&& itr_peidu->second->getChengzhuShili() == shili)
						{
							*mychengchiID = peiduid;
							changed = true;
						}
					}
				}
				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}

		if (changed)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			stream->_writeUChar(*mychengchiID);
			ref_ptr<crGameServerPlayerData> playerGameData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerGameData.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJChangeChengchiID, stream.get());
				netConductor->getNetManager()->sendPacket(playerGameData->getPlayerConnectServerAddress(), packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJHideAddArmyTimerMethod
//
/////////////////////////////////////////
crJXJHideAddArmyTimerMethod::crJXJHideAddArmyTimerMethod(){}
crJXJHideAddArmyTimerMethod::crJXJHideAddArmyTimerMethod(const crJXJHideAddArmyTimerMethod& handle):
	crMethod(handle)
{
}
void crJXJHideAddArmyTimerMethod::inputParam(int i, void *param)
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

void crJXJHideAddArmyTimerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSw = str;
		break;
	default:
		break;
	}
}

void crJXJHideAddArmyTimerMethod::operator()(crHandle &handle)
{
	ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSw));
	if(!sw.valid()) return;
	void *param;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me)
	{
		crRoom *room = me->getSelectedRoom();
		if(room)
		{
			crData *roomData = room->getDataClass();
			if(roomData)
			{
				roomData->getParam(WCHDATA_JXJRoomType,param);
				unsigned char type = *(unsigned char *)param;
				if(type == 1)//guozhan
					sw->setActiveSwitchSet(0);
				else if(type == 2)//fuben
					sw->setActiveSwitchSet(1);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJSetBattleCameraMoveableMethod
//
/////////////////////////////////////////
crJXJSetBattleCameraMoveableMethod::crJXJSetBattleCameraMoveableMethod(){}
crJXJSetBattleCameraMoveableMethod::crJXJSetBattleCameraMoveableMethod(const crJXJSetBattleCameraMoveableMethod& handle):
	crMethod(handle)
{
}
void crJXJSetBattleCameraMoveableMethod::inputParam(int i, void *param)
{
}

void crJXJSetBattleCameraMoveableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_bEnable = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJSetBattleCameraMoveableMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		CRCore::crNode * cameraNode = camera->getAttachedNode();
		if(cameraNode)
		{
			crData *cameraData = cameraNode->getDataClass();
			if(cameraData)
			{
				cameraData->inputParam(WCHDATA_JXJBattleCameraMovable,&m_bEnable);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCritExtraAttackMethod
//
/////////////////////////////////////////
crJXJCritExtraAttackMethod::crJXJCritExtraAttackMethod(){}
crJXJCritExtraAttackMethod::crJXJCritExtraAttackMethod(const crJXJCritExtraAttackMethod& handle):
	crMethod(handle)
{
}
void crJXJCritExtraAttackMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_hitParam = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_hitParam = (HitParam *)(LOINT64(param64));
		}
		else
		{
			m_hitParam = NULL;
		}
		break;
	}
}
void crJXJCritExtraAttackMethod::addParam(int i, const std::string& str)
{
}
void crJXJCritExtraAttackMethod::operator()(crHandle &handle)
{
	void * param;
	bool cancelTask = true;
	if(m_hitParam->m_fireItem.valid() && m_hitParam->m_fireItem->getDataClass())
	{	
		crData *fireItemData = m_hitParam->m_fireItem->getDataClass();
		//unsigned int guisestate = GS_Normal;
		//m_hitParam->m_fireItem->doEvent(MAKEINT64(WCH_GetGuiseState,0),MAKEINT64(&guisestate,NULL));

		fireItemData->getParam(WCHDATA_IsCrit,param);
		bool isCrit = *(bool *)param;
		if(isCrit/* && (guisestate & GS_ExtraAttack )*/)
		{
			cancelTask = false;
		}
	}
	handle.outputParam(0,&cancelTask);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleBrocastMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleBrocastMethod::crJXJRecvBattleBrocastMethod(){}
crJXJRecvBattleBrocastMethod::crJXJRecvBattleBrocastMethod(const crJXJRecvBattleBrocastMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvBattleBrocastMethod::inputParam(int i, void *param)
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

void crJXJRecvBattleBrocastMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvBattleBrocastMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char _case = m_stream->_readUChar();

			crPlayerGameData * player = crMyPlayerData::getInstance()->getPlayerGameData();
			if (player && player->getDataClass())
			{
				crData *playerData = player->getDataClass();
				playerData->getParam(WCHDATA_JXJShiliID, param);
				unsigned char myShili = *(unsigned char *)param;

				std::string text,playerName,itemName, guojia,zhiwei,text1,head,middle,tail;
				if (_case == 5)
				{
					std::string releaseName = m_stream->_readString();
					std::string playerName = m_stream->_readString();
					text1 = "[公告]：玩家" + playerName + "通过" + releaseName + "发布的征召令进入战场";
				}
				if (_case == 4)
				{
					char type = m_stream->_readChar();
					playerName = m_stream->_readString();
					itemName = m_stream->_readString();
					bool isAttack = true;
					crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
					if (room)
					{
						crData *roomData = room->getDataClass();
						if (roomData)
						{
							roomData->getParam(WCHDATA_JXJBattleID, param);
							int battleid = *(int *)param;
							unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));

							if (myShili == defenceShili)
								isAttack = false;
						}
					}

					if (isAttack)
					{
						if (type == 0)
							text = std::string("恭喜玩家[") + playerName + std::string("]成功击破[") + itemName + std::string("]");
						else if (type == 1)
							text = std::string("恭喜玩家[") + playerName + std::string("]成功杀死[") + itemName + std::string("]");
					}
					else
					{
						if (type == 0)
						{
							text = std::string("[") + itemName + std::string("]已被玩家[") + playerName + std::string("]击破！");
							//text1 = "[公告]：" + itemName  + "被击破";
						}
						else if (type == 1)
						{
							text = std::string("[") + itemName + std::string("]已被玩家[") + playerName + std::string("]杀死！");
							//text1 = "[公告]：" + itemName  + "被击杀";
						}
					}
				}
				else if(_case == 3)
				{
					unsigned char shili = m_stream->_readUChar();
					unsigned char guanzhi = m_stream->_readUChar();
					playerName = m_stream->_readString();

					if(shili == 10)
						guojia = "魏国";
					else if(shili == 11)
						guojia = "蜀国";
					else if(shili == 12)
						guojia = "吴国";
					else if(shili == 13)
						guojia = "群";

					if(guanzhi == 1)
						zhiwei = "丞相";
					else if(guanzhi == 2)
						zhiwei = "大司马";
					else if(guanzhi == 3)
						zhiwei = "大将军";

					if(shili == myShili)
						guojia = "本国";

					text1 = "[公告]：" + guojia + zhiwei + playerName + "离开战场";

					head = "[公告]：了事拂衣去，深藏身与名！ ";
					middle  = guojia + zhiwei + playerName;
					tail = " 率领他的军队离开战场！";
				}
				else if(_case == 2)
				{
					unsigned char shili = m_stream->_readUChar();
					unsigned char guanzhi = m_stream->_readUChar();
					playerName = m_stream->_readString();

					if(shili == 10)
						guojia = "魏国";
					else if(shili == 11)
						guojia = "蜀国";
					else if(shili == 12)
						guojia = "吴国";
					else if(shili == 13)
						guojia = "群";

					if(guanzhi == 1)
						zhiwei = "丞相";
					else if(guanzhi == 2)
						zhiwei = "大司马";
					else if(guanzhi == 3)
						zhiwei = "大将军";

					if(shili == myShili)
						guojia = "本国";

					text1 = "[公告]：" + guojia + zhiwei + playerName + "进入战场";

					head = "[公告]：星沉地动，风虎云龙！ ";
					middle  = guojia + zhiwei + playerName;
					tail = " 率领他的军队进入战场！";
				}
				else if(_case == 1)
				{
					unsigned char shili = m_stream->_readUChar();
					unsigned char guanzhi = m_stream->_readUChar();
					unsigned char deadtimes = m_stream->_readUChar();
					playerName = m_stream->_readString();
					itemName = m_stream->_readString();
					if(shili == 10)
						guojia = "魏国";
					else if(shili == 11)
						guojia = "蜀国";
					else if(shili == 12)
						guojia = "吴国";
					else if(shili == 13)
						guojia = "群";

					if(guanzhi == 1)
						zhiwei = "丞相";
					else if(guanzhi == 2)
						zhiwei = "大司马";
					else if(guanzhi == 3)
						zhiwei = "大将军";

					if (myShili != shili)
					{
						if(playerName != "npc")
							text1 = std::string("[公告]：我国[") + playerName + std::string("]击杀") + guojia + zhiwei + std::string("[") + itemName + std::string("]");

						head = "[战报]：";
						middle  = guojia + zhiwei + itemName;
						tail = " 倒在我军铁蹄之下，剑锋所指，日月臣服！";

						if(deadtimes > 1)
							tail = " 再次倒在我军铁蹄之下，剑锋所指，日月臣服！";

						/*text = std::string("恭喜玩家[") + playerName + std::string("]成功击杀[") + guojia + zhiwei + itemName + std::string("]");
						if(playerName == "npc")
						text.clear();*/
					}
					/*else
					{
					text = std::string("[我国")  + zhiwei + itemName + std::string("]已被玩家[") + playerName + std::string("]击杀！");
					text1 = "[公告]：本国" + zhiwei + itemName  + "被击杀";
					if(playerName == "npc")
					text.clear();
					}*/
				}
				

				if (!text1.empty())
				{
					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("Can_Chanchanxinxi");
					if (canvas.valid())
					{
						ref_ptr<crData> canvasdata  = canvas->getDataClass();
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJBattleBrocastqeque,param);
						std::list<std::string> * tonggaoqueue = (std::list<std::string> *)param;
						if (tonggaoqueue->size() >= 5)
						{
							std::list<std::string> list_tem(++tonggaoqueue->begin(),tonggaoqueue->end());
							//tonggaoqueue->erase(tonggaoqueue->begin());
							tonggaoqueue->clear();
							*tonggaoqueue = list_tem;
							tonggaoqueue->push_back(text1);
						}
						else
							tonggaoqueue->push_back(text1);

						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
						if(tonggaoqueue->size() == 1)
							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);

						canvas->doEvent(WCH_UI_UpdateData);
					}
				}
				if (!text.empty())
				{
					ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
					record->setChatText(text);
					//record->setTextColor(COLOR_GREEN);
					record->setChatChannelName("[战场]");
					crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
				}

				if (_case==1 || _case==2 || _case==3)
				{
					if (!head.empty() && !middle.empty() && !tail.empty())
					{
						ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("Can_Zhanbiaoxian");
						if (canvas.valid())
						{
							ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("UI_Zhanbiaoxian"));
							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);

							std::vector<float>v_i;
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);

							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");

							cfg_script.Add("Content");
							cfg_script.Push("Content", 1);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", head);
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 2);
							v_i.clear();
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Push("Character",18);
							cfg_script.Add("Text", middle);
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content", 3);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							cfg_script.Add("Text", tail);
							cfg_script.Pop();

							cfg_script.Pop();
							if(contentText.valid()) contentText->setHypertext(cfg_script);

							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleBrocastMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCheckInspireBtEnableMethod::crJXJCheckInspireBtEnableMethod(){}
crJXJCheckInspireBtEnableMethod::crJXJCheckInspireBtEnableMethod(const crJXJCheckInspireBtEnableMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJCheckInspireBtEnableMethod::inputParam(int i, void *param)
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

void crJXJCheckInspireBtEnableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strBt = str;
		break;
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
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		m_strimg[i - 1] = str;
		break;
	case 25:
		m_canvas = str;
		break;
	case 26:
		m_strBt_defend = str;
		break;
	default:
		break;
	}
}

void crJXJCheckInspireBtEnableMethod::operator ()(crHandle &handle)
{
	ref_ptr<crImageBoxWidgetNode> imgBox;
	for (int i = 0; i < 24;i++)
	{
		imgBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strimg[i]));
		if (imgBox.valid())
		{
			imgBox->setVisiable(false);
		}
	}

	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid()) return;  //鼓舞按钮parentCanvas调整
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me)
	{
		void *param;
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if (room && room->getDataClass())
			{
				crData *roomData = room->getDataClass();
				roomData->getParam(WCHDATA_JXJBattleID, param);
				int battleid = *(int *)param;
				playerData->getParam(WCHDATA_JXJShiliID, param);
				unsigned char myShili = *(unsigned char *)param;
				unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
				ref_ptr<crButtonWidgetNode> inspireBt = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strBt));
				if (myShili == dshili)
				{//防守方
					if (inspireBt.valid()) inspireBt->setVisiable(false); //隐藏攻防小鼓舞按钮
					inspireBt = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strBt_defend));
				}
				if(inspireBt.valid())
				{
					playerData->getParam(WCHDATA_JXJVipLv,param);
					unsigned char vipLevel = *(unsigned char *)param;
					int canInspireCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireMaxCount, vipLevel).c_str());
					if(canInspireCount<=0) inspireBt->setEnable(false);
					else inspireBt->setEnable(true);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJChatChannelSetInitMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatChannelSetInitMethod::crJXJChatChannelSetInitMethod(){}
crJXJChatChannelSetInitMethod::crJXJChatChannelSetInitMethod(const crJXJChatChannelSetInitMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJChatChannelSetInitMethod::inputParam(int i, void *param)
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

void crJXJChatChannelSetInitMethod::addParam(int i, const std::string& str)
{

}

void crJXJChatChannelSetInitMethod::operator ()(crHandle &handle)
{
	crData * canvasData = m_this->getDataClass();
	if(canvasData)
	{
		void *param;
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJChatChannelDefaultSet,param);
		BitSet64 * bitSet = (BitSet64 *)param;

		char chatChannelSet[64];
		char filePath[MAX_PATH];
		std::string strPath = crGlobalHandle::getInstance()->getGameDir();
		sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
		::GetPrivateProfileString("ChatChannel",\
			"Value",\
			"000100000000011000000011000000010100000010010000010001111111111" \
			,chatChannelSet,64,filePath);


		BitSet64 temp(chatChannelSet);

		*bitSet = temp;

		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJChatChannelSetCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatChannelSetCanvasMethod::crJXJChatChannelSetCanvasMethod(){}
crJXJChatChannelSetCanvasMethod::crJXJChatChannelSetCanvasMethod(const crJXJChatChannelSetCanvasMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJChatChannelSetCanvasMethod::inputParam(int i, void *param)
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

void crJXJChatChannelSetCanvasMethod::addParam(int i, const std::string& str)
{
}

void crJXJChatChannelSetCanvasMethod::operator ()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	crData * playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(canvasData && playerData)
	{
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJChatChannelTempSet,param);
		BitSet64 * tempSet = (BitSet64 *)param;
		playerData->getParam(WCHDATA_JXJChatChannelSetInfo,param);
		BitSet64 & userSet = *(BitSet64 *)param;

		*tempSet = userSet;

		m_this->doEvent(WCH_UI_UpdateData);
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJChatChannelSetCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatChannelSetCanvasUpdateMethod::crJXJChatChannelSetCanvasUpdateMethod(){}
crJXJChatChannelSetCanvasUpdateMethod::crJXJChatChannelSetCanvasUpdateMethod(const crJXJChatChannelSetCanvasUpdateMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJChatChannelSetCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJChatChannelSetCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strChatChannelRadio = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		m_strCheckBox[i-1] = str;
		break;
	default:
		break;
	}
}

void crJXJChatChannelSetCanvasUpdateMethod::operator ()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	if(canvasData)
	{
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
		canvasData->getParam(WCHDATA_JXJChatChannelDefaultSet,param);
		BitSet64 & defaultSet = *(BitSet64*)param;
		canvasData->getParam(WCHDATA_JXJChatChannelTempSet,param);
		BitSet64 & tempSet = *(BitSet64 *)param;

		ref_ptr<crCheckBoxWidgetNode> checkBox[9] = {NULL};
		ref_ptr<crRadioGroupWidgetNode>radio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_strChatChannelRadio));

		for (int i = 0;i<9;i++)
		{
			checkBox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_strCheckBox[i]));
		}

		if(radio.valid())
		{
			int beginIndex = radio->getSelect() * 9;
			int endIndex = beginIndex+9;


			for (int i = beginIndex;\
				i<tempSet.size() && i< defaultSet.size() && i<endIndex;\
				i++)
			{
				if(checkBox[i-beginIndex].valid())
				{
					checkBox[i-beginIndex]->select(tempSet[i]);
					if(defaultSet[i])
						checkBox[i-beginIndex]->setEnable(false);
					else
						checkBox[i-beginIndex]->setEnable(true);
				}
			}
		}
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJChatChannelSetCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatChannelValueChangeMethod::crJXJChatChannelValueChangeMethod(){}
crJXJChatChannelValueChangeMethod::crJXJChatChannelValueChangeMethod(const crJXJChatChannelValueChangeMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJChatChannelValueChangeMethod::inputParam(int i, void *param)
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

void crJXJChatChannelValueChangeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strRadio = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJChatChannelValueChangeMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>parentCanvas = m_this->getParentCanvas();
	ref_ptr<crCheckBoxWidgetNode> checkBox = dynamic_cast<crCheckBoxWidgetNode *>(m_this);
	if(parentCanvas.valid() && checkBox.valid())
	{
		ref_ptr<crRadioGroupWidgetNode>radio = dynamic_cast<crRadioGroupWidgetNode *>(parentCanvas->getWidget(m_strRadio));
		crData *canvasData = parentCanvas->getDataClass();
		if(canvasData && radio.valid())
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJChatChannelTempSet,param);
			BitSet64 & tempSet = *(BitSet64 *)param;

			int pos = radio->getSelect() * 9 + m_index;
			if(pos<tempSet.size())
				tempSet[pos] = checkBox->getSelect();
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJSaveChatChannelSetMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSaveChatChannelSetMethod::crJXJSaveChatChannelSetMethod(){}
crJXJSaveChatChannelSetMethod::crJXJSaveChatChannelSetMethod(const crJXJSaveChatChannelSetMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJSaveChatChannelSetMethod::inputParam(int i, void *param)
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

void crJXJSaveChatChannelSetMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJSaveChatChannelSetMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	crPlayerGameData * player = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(canvas.valid() && player && netConductor)
	{
		crData *canvasData = canvas->getDataClass();
		crData * playerData = player->getDataClass();
		if(canvasData)
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJChatChannelTempSet,param);
			BitSet64 * tempSet = (BitSet64 *)param;
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJChatChannelSetInfo,param);
			BitSet64 * userSet = (BitSet64 *)param;

			if(*tempSet!=*userSet)
			{
				*userSet = *tempSet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(8);
				stream->_writeInt64(tempSet->to_ullong());
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSaveChatChannelSet,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJDefaultChatChannelSetMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJDefaultChatChannelSetMethod::crJXJDefaultChatChannelSetMethod(){}
crJXJDefaultChatChannelSetMethod::crJXJDefaultChatChannelSetMethod(const crJXJDefaultChatChannelSetMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJDefaultChatChannelSetMethod::inputParam(int i, void *param)
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

void crJXJDefaultChatChannelSetMethod::addParam(int i, const std::string& str)
{

}

void crJXJDefaultChatChannelSetMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(canvas.valid())
	{
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJChatChannelTempSet,param);
			BitSet64 * tempSet = (BitSet64 *)param;

			*tempSet = (_ULONGLONG)0x0004220140A05C00;	

			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvSaveChatChannelSetMethod
//
/////////////////////////////////////////
crJXJRecvSaveChatChannelSetMethod::crJXJRecvSaveChatChannelSetMethod(){}
crJXJRecvSaveChatChannelSetMethod::crJXJRecvSaveChatChannelSetMethod(const crJXJRecvSaveChatChannelSetMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvSaveChatChannelSetMethod::inputParam(int i, void *param)
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

void crJXJRecvSaveChatChannelSetMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvSaveChatChannelSetMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		_ULONGLONG channelSet = m_stream->_readInt64();
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJChatChannelSetInfo,param);
			BitSet64 *userSet = (BitSet64 *)param;

			*userSet = channelSet;

			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvBattleDisableMethod
//
/////////////////////////////////////////
crJXJRecvBattleDisableMethod::crJXJRecvBattleDisableMethod():
	m_netType(GameClient_Game){}
crJXJRecvBattleDisableMethod::crJXJRecvBattleDisableMethod(const crJXJRecvBattleDisableMethod& handle):
	crMethod(handle),
	m_noticeCanvas(handle.m_noticeCanvas),
	m_textWidget(handle.m_textWidget)
{
}
void crJXJRecvBattleDisableMethod::inputParam(int i, void *param)
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
			m_netType = HIINT64(param64);
		}
		break;
	}
}
void crJXJRecvBattleDisableMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_textWidget = str;
		break;
	}
}
void crJXJRecvBattleDisableMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
			if(canvas.valid())
			{
				void *param;
				ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				crTableIO::StrVec record;
				crData * data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
				unsigned char type = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short selectID = *(unsigned short *)param;
				if(type == FT_Chengchi && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(selectID),record)>=0)
				{
					int nameindex = chengchiTab->getTitleIndex("名字");
					crStaticTextWidgetNode *textWidget = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_textWidget));
					if(textWidget)
					{
						int texid = 4029;//"[%s]出生点已经全部失守，在出生点夺回之前无法进入！"
						crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
						noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
						int mode = 2;
						noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
						noticeHandle->inputParam(WCHDATA_NoticeParam1,&(record[nameindex]));
						std::string output;
						noticeHandle->inputParam(WCHDATA_NoticeOutPut,&output);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
						textWidget->setString(output);
					}
					if(!canvas->getVisiable())
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvTempClientNoticeMethod
//
/////////////////////////////////////////
crJXJRecvTempClientNoticeMethod::crJXJRecvTempClientNoticeMethod() :
m_netType(GameClient_Game),m_time(180){}
crJXJRecvTempClientNoticeMethod::crJXJRecvTempClientNoticeMethod(const crJXJRecvTempClientNoticeMethod& handle) :
crMethod(handle),
m_canvas(handle.m_canvas),
m_time(180)
{
}
void crJXJRecvTempClientNoticeMethod::inputParam(int i, void *param)
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

void crJXJRecvTempClientNoticeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_strChatCanvas = str;
		break;
	case 2:
		m_strBattleChatCanvas = str;
		break;
	case 3:
		m_strHyperTxt = str;
		break;
	case 4:
		m_time = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJRecvTempClientNoticeMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid())
			{
				crData *data = m_this->getDataClass();
				// 				rcfg::ConfigScript cfg_script;
				// 				crGlobalHandle::getInstance()->getStreamScript(m_stream.get(),cfg_script);
				// 				cfg_script.Write("232425.cfg");
				data->inputParam(WCHDATA_JXJCenterNotice, m_stream.get());

				//int time = 180;
				data->inputParam(WCHDATA_JXJCenterNoticeRefershTime, &m_time);
			}


			//插入聊天队列
			std::vector<float>v_i;
			ItemMap itemMap, itemMap2;
			rcfg::ConfigScript cfg_script(&itemMap);
			rcfg::ConfigScript cfg_script2(&itemMap2);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("DrawMode", "TEXT");
			cfg_script.Add("CharacterSize", 16);
			cfg_script.Add("Text", "[游戏]");
			cfg_script.Pop();

			cfg_script.Pop();

			cfg_script2.Add("Hypertext");
			cfg_script2.Push("Hypertext");

			cfg_script2.Add("Content");
			cfg_script2.Push("Content");
			cfg_script2.Add("DrawMode", "TEXT");
			cfg_script2.Add("Text", " /n");
			cfg_script2.Pop();

			cfg_script2.Pop();

			ref_ptr<crStreamBuf> bufferFront = new crStreamBuf;
			ref_ptr<crStreamBuf> bufferBack = new crStreamBuf;
			crGlobalHandle::getInstance()->getScriptStream(cfg_script, bufferFront);
			crGlobalHandle::getInstance()->getScriptStream(cfg_script2, bufferBack);

			crMyPlayerData *me = crMyPlayerData::getInstance();
			if (me && me->getPlayerGameData())
			{
				void *param;
				crData *playerData = me->getPlayerGameData()->getDataClass();
				if (playerData)
				{
					playerData->excHandle(MAKEINT64(WCH_LockData, 1));
					playerData->getParam(WCHDATA_JXJVipLv, param);
					unsigned char viplv = *(unsigned char *)param;

					playerData->getParam(WCHDATA_JXJChatRecordMap, param);
					ChatRecordMap *chatRecordMap = (ChatRecordMap *)param;

					int maxDequeSize = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine, viplv).c_str());

					if (chatRecordMap)
					{
						for (int chatMode = ChatAll; chatMode < ChatNull; chatMode++)
						{
							ChatRecordDeque & chatDeque = (*chatRecordMap)[(JXJChatMode)chatMode];
							if (chatDeque.size() >= maxDequeSize - 2)
							{
								chatDeque.pop_front();
							}
							chatDeque.push_back(bufferFront.get());
							chatDeque.push_back(m_stream.get());
							chatDeque.push_back(bufferBack.get());
						}
					}
					playerData->excHandle(MAKEINT64(WCH_LockData, 0));

					ref_ptr<crCanvasNode> chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strChatCanvas);
					ref_ptr<crCanvasNode> battlechatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strBattleChatCanvas);

					if (chatCanvas.valid())
					{
						rcfg::ConfigScript cfg_script3;
						crGlobalHandle::getInstance()->getStreamScript(m_stream.get(), cfg_script3);
						ref_ptr<crHypertextWidgetNode> chatShow = dynamic_cast<crHypertextWidgetNode *>(chatCanvas->getWidget(m_strHyperTxt));
						ref_ptr<crHypertextWidgetNode> chatShow2 = dynamic_cast<crHypertextWidgetNode *>(chatCanvas->getWidget(m_strHyperTxt + "2"));

						if (chatShow.valid())
						{
							if (chatShow->getValidTextSize() > maxDequeSize * 2)
								chatShow->popTexts(maxDequeSize / 2);
							chatShow->setHypertext(cfg_script, true);
							chatShow->setHypertext(cfg_script3, true);
							chatShow->setHypertext(cfg_script2, true);
						}
						if (chatShow2.valid())
						{
							if (chatShow2->getValidTextSize() > maxDequeSize * 2)
								chatShow2->popTexts(maxDequeSize / 2);
							chatShow2->setHypertext(cfg_script, true);
							chatShow2->setHypertext(cfg_script3, true);
							chatShow2->setHypertext(cfg_script2, true);
						}
					}
					if (battlechatCanvas.valid())
					{
						rcfg::ConfigScript cfg_script3;
						crGlobalHandle::getInstance()->getStreamScript(m_stream.get(), cfg_script3);

						ref_ptr<crHypertextWidgetNode> chatShow = dynamic_cast<crHypertextWidgetNode *>(battlechatCanvas->getWidget(m_strHyperTxt));
						ref_ptr<crHypertextWidgetNode> chatShow2 = dynamic_cast<crHypertextWidgetNode *>(battlechatCanvas->getWidget(m_strHyperTxt + "2"));

						if (chatShow.valid())
						{
							if (chatShow->getValidTextSize() > maxDequeSize * 2)
								chatShow->popTexts(maxDequeSize / 2);
							chatShow->setHypertext(cfg_script, true);
							chatShow->setHypertext(cfg_script3, true);
							chatShow->setHypertext(cfg_script2, true);
						}
						if (chatShow2.valid())
						{
							if (chatShow2->getValidTextSize() > maxDequeSize * 2)
								chatShow2->popTexts(maxDequeSize / 2);
							chatShow2->setHypertext(cfg_script, true);
							chatShow2->setHypertext(cfg_script3, true);
							chatShow2->setHypertext(cfg_script2, true);
						}
					}
					//if(crRunGameHandle::getInstance()->isInManor()||crRunGameHandle::getInstance()->isInBattle())
					//	crBrain::getInstance()->doEvent(WCH_JXJUIUpdateChatMessageList);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJSycnCenterNoticeMethod
//
/////////////////////////////////////////
crJXJSycnCenterNoticeMethod::crJXJSycnCenterNoticeMethod(){}
crJXJSycnCenterNoticeMethod::crJXJSycnCenterNoticeMethod(const crJXJSycnCenterNoticeMethod& handle) :
crMethod(handle)
{
}
void crJXJSycnCenterNoticeMethod::inputParam(int i, void *param)
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

void crJXJSycnCenterNoticeMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJSycnCenterNoticeMethod::operator()(crHandle &handle)
{
	void *param;

	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	if (player)
	{
		crData *playerData = player->getDataClass();
		if (playerData)
		{
			playerData->getParam(WCHDATA_JXJCenterNoticeRefershTime, param);
			int *time = (int *)param;
			if (*time > 0)
				*time -= 1;

			if (*time == 0)
			{
				std::string file = "helloplayer.cfg";
				playerData->getParam(WCHDATA_JXJOldCenterNotice, param);
				crStreamBuf *filestream = (crStreamBuf *)param;
				if (filestream)
				{
					playerData->inputParam(WCHDATA_JXJCenterNotice, filestream);
				}
				else if (!file.empty())
				{
					file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
					if (!file.empty())
					{
						rcfg::ConfigScript cfg_script;
						if (crScriptLoadManager::getInstance()->loadConfigScript(cfg_script, file))
						{
							ref_ptr<crStreamBuf> tempStream;
							crGlobalHandle::getInstance()->getScriptStream(cfg_script, tempStream);
							playerData->inputParam(WCHDATA_JXJCenterNotice, tempStream.get());
							playerData->inputParam(WCHDATA_JXJOldCenterNotice, tempStream.get());
						}
					}
				}
				*time -= 1;

				m_this->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJPlayMiniMapSignalMethod
//
/////////////////////////////////////////
crJXJPlayMiniMapSignalMethod::crJXJPlayMiniMapSignalMethod() : m_time(0.0f)
{}
crJXJPlayMiniMapSignalMethod::crJXJPlayMiniMapSignalMethod(const crJXJPlayMiniMapSignalMethod& handle) :
crMethod(handle),
m_vecPos(handle.m_vecPos),
m_time(handle.m_time)
{
}
void crJXJPlayMiniMapSignalMethod::inputParam(int i, void *param)
{

}

void crJXJPlayMiniMapSignalMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strMinimap = str;
		break;
	case 2:
		m_strWidget = str;
		break;
	case 3:
		m_strRotateMap = str;
		break;
	case 4:
		crArgumentParser::appAtoVec(str.c_str(), m_vecPos);
		break;
	case 5:
		m_time = atof(str.c_str());
		break;
	case 6:
		m_strWidgetName = str;
		break;
	default:
		break;
	}
}

void crJXJPlayMiniMapSignalMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if (camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ref_ptr<CRCore::crWidgetNode> icon;
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJSignalsMap, param);
		if (param)
		{
			SignalNodeMap *signaldeque = (SignalNodeMap *)param;

			cameraData->getParam(WCHDATA_JXJNamedSignalNodeMap, param);
			NamedSignalNodeMap *namedSignalMap = (NamedSignalNodeMap *)param;

			const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
			const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
			ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if (mainCanvas.valid() && canvas.valid())
			{
				ref_ptr<crWidgetNode>  miniMap = canvas->getWidget(m_strMinimap);
				ref_ptr<crWidgetNode>  signalWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_strWidget));
				ref_ptr<crWidgetNode>  rotMapWidget = canvas->getWidget(m_strRotateMap);
				if (miniMap.valid() && signalWidget.valid() && rotMapWidget.valid())
				{
					crVector2 pos;
					ref_ptr<crWidgetNode>  iconWidget;
					int maxIndex = m_vecPos.size() / 2;
					for (int i = 0; i < maxIndex; i++)
					{
						pos[0] = m_vecPos[i * 2 + 0];
						pos[1] = m_vecPos[i * 2 + 1];
						//pos[2] = m_vecPos[i * 3 + 2];
						if (m_strWidgetName.empty())
						{
							SignalNodeMap::iterator itr_s = signaldeque->begin();
							while (itr_s != signaldeque->end())
							{
								// 							if(signaldeque->size()>=20 && itr_s->first->getName().compare(m_strWidget)==0)
								// 							{
								// 								iconWidget = itr_s->first;
								// 								itr_s->second = m_time;
								// 								break;
								// 							}
								if (itr_s->first->getVisiable() == false && \
									itr_s->first->getName().compare(m_strWidget) == 0)
								{
									iconWidget = itr_s->first;
									itr_s->second = m_time;
									break;
								}
								itr_s++;
							}
							if (itr_s == signaldeque->end() && signaldeque->size() <= 20)
							{
								iconWidget = dynamic_cast<crWidgetNode *>(signalWidget->clone(crCopyOp::DEEP_COPY_NODES));
								signaldeque->insert(std::make_pair(iconWidget, m_time));
								crLoadManager::getInstance()->requestAddNode(canvas.get(), iconWidget.get(), false);
							}

							if (iconWidget.valid())
							{
								const crBoundingBox &bbox = miniMap->getBoundBox();
								crVector3 centerCoord = bbox.center();
								crVector2 mapWidgetSize(bbox.xLength(), bbox.yLength());
								crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);

								float px = pos.x() * mapFactor[0] * crGlobalHandle::gData()->gUnitScale();
								float py = pos.y() * mapFactor[1] * crGlobalHandle::gData()->gUnitScale();

								crVector3 pos = centerCoord + crVector3(px, py, 0.0f);
								pos = pos*rotMapWidget->getMatrix();
								iconWidget->setMatrix(scaleMat*invScaleMat*crMatrix::translate(pos));
								iconWidget->setVisiable(true);
							}
						}
						else
						{
							iconWidget = dynamic_cast<crWidgetNode *>(signalWidget->clone(crCopyOp::DEEP_COPY_NODES));
							
							crLoadManager::getInstance()->requestAddNode(canvas.get(), iconWidget.get(), false);
							
							if (iconWidget.valid())
							{
								const crBoundingBox &bbox = miniMap->getBoundBox();
								crVector3 centerCoord = bbox.center();
								crVector2 mapWidgetSize(bbox.xLength(), bbox.yLength());
								crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);

								float px = pos.x() * mapFactor[0] * crGlobalHandle::gData()->gUnitScale();
								float py = pos.y() * mapFactor[1] * crGlobalHandle::gData()->gUnitScale();

								crVector3 mappos = centerCoord + crVector3(px, py, 0.0f);
								mappos = mappos*rotMapWidget->getMatrix();
								iconWidget->setMatrix(scaleMat*invScaleMat*crMatrix::translate(mappos));
								iconWidget->setVisiable(true);
								crVector3 hitPos;
								hitPos[0] = pos[0]*crGlobalHandle::gData()->gUnitScale();
								hitPos[1] = pos[1]*crGlobalHandle::gData()->gUnitScale();
								namedSignalMap->insert(std::make_pair(m_strWidgetName, std::make_pair(iconWidget, hitPos)));
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
//crJXJStopMiniMapSignalMethod
//
/////////////////////////////////////////
crJXJStopMiniMapSignalMethod::crJXJStopMiniMapSignalMethod()
{}
crJXJStopMiniMapSignalMethod::crJXJStopMiniMapSignalMethod(const crJXJStopMiniMapSignalMethod& handle) :
crMethod(handle)
{
}
void crJXJStopMiniMapSignalMethod::inputParam(int i, void *param)
{

}

void crJXJStopMiniMapSignalMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strWidgetName = str;
		break;
	}
}

void crJXJStopMiniMapSignalMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if (camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ref_ptr<CRCore::crWidgetNode> icon;
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJNamedSignalNodeMap, param);
		NamedSignalNodeMap *namedSignalMap = (NamedSignalNodeMap *)param;

		NamedSignalNodeMap::iterator itr = namedSignalMap->begin();
		while (itr!=namedSignalMap->end())
		{
			if (itr->first.compare(m_strWidgetName) == 0)
			{
				itr->second.first->setVisiable(false);
			}
			itr++;
		}
	}
}

/////////////////////////////////////////
//
//crJXJJuQingCanvasOnShowMethod
//
/////////////////////////////////////////
crJXJJuQingCanvasOnShowMethod::crJXJJuQingCanvasOnShowMethod()
{}
crJXJJuQingCanvasOnShowMethod::crJXJJuQingCanvasOnShowMethod(const crJXJJuQingCanvasOnShowMethod& handle) :
crMethod(handle)
{
}
void crJXJJuQingCanvasOnShowMethod::inputParam(int i, void *param)
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

void crJXJJuQingCanvasOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strJuQingImg = str;
		break;
	case 1:
		m_strJuQingName = str;
		break;
	case 2:
		m_strJuQingInfo = str;
		break;
	default:
		break;
	}
}

void crJXJJuQingCanvasOnShowMethod::operator()(crHandle &handle)
{
	void *param;
	crData *canvasData = m_this->getDataClass();
	if (canvasData)
	{
		ref_ptr<crImageBoxWidgetNode> imageWidget = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strJuQingImg));
		ref_ptr<crStaticTextWidgetNode> nameWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strJuQingName));
		ref_ptr<crStaticTextWidgetNode> infoWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strJuQingInfo));
		
		if (!imageWidget || !nameWidget || !infoWidget) return;

		canvasData->excHandle(MAKEINT64(WCH_LockData, 1));

		canvasData->getParam(WCHDATA_JXJJuQingImg, param);
		std::string & image = *(std::string *)param;
		canvasData->getParam(WCHDATA_JXJJuQingName, param);
		std::string & name = *(std::string *)param;
		canvasData->getParam(WCHDATA_JXJJuQingInfo, param);
		std::string & info = *(std::string *)param;

		if (image.empty())
		{
			imageWidget->setVisiable(false);
		}
		else
		{
			imageWidget->setVisiable(true);
			imageWidget->setImageName(image);
		}
		
		nameWidget->setString(name);
		infoWidget->setString(info);

		canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
	}
}

/////////////////////////////////////////
//
//crJXJJuQingCanvasFrameMethod
//
/////////////////////////////////////////
crJXJJuQingCanvasFrameMethod::crJXJJuQingCanvasFrameMethod() :
m_this(NULL),
m_dt(NULL)
{}
crJXJJuQingCanvasFrameMethod::crJXJJuQingCanvasFrameMethod(const crJXJJuQingCanvasFrameMethod& handle) :
crMethod(handle),
m_this(NULL),
m_dt(NULL)
{
}
void crJXJJuQingCanvasFrameMethod::inputParam(int i, void *param)
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
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = (float*)lparam;
		}
		break;
	}
}

void crJXJJuQingCanvasFrameMethod::addParam(int i, const std::string& str)
{
}

void crJXJJuQingCanvasFrameMethod::operator()(crHandle &handle)
{
	void *param;
	crData *canvasData = m_this->getDataClass();
	if (canvasData)
	{
		CRCore::ScopedLock<crData> lock(*canvasData);
		canvasData->getParam(WCHDATA_JXJScenarioTalkAutoCloseTime, param);
		float autoclose = *(float *)param;
		if (autoclose > 0.0f)
		{
			canvasData->getParam(WCHDATA_JXJVisiableTime, param);
			float *timer = (float *)param;
			if (*timer <= 0.0f)
			{
				canvasData->getParam(WCHDATA_JXJScenarioTalkStep, param);
				int *curstep = (int *)param;
				*curstep += 1;
				std::string str = m_this->getName();
				m_this->doEvent(WCH_JXJJXJShowScenarioTalk, MAKEINT64(&str, NULL));
				*timer = autoclose;
				//m_this->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
				//crFilterRenderManager::getInstance()->showCanvas(m_this, false);
			}
			else
			{
				*timer -= *m_dt;
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJShowJuQingCanvasMethod
//
/////////////////////////////////////////
crJXJShowJuQingCanvasMethod::crJXJShowJuQingCanvasMethod():m_time(0.0f)
{}
crJXJShowJuQingCanvasMethod::crJXJShowJuQingCanvasMethod(const crJXJShowJuQingCanvasMethod& handle) :
crMethod(handle),
m_time(handle.m_time)
{
}
void crJXJShowJuQingCanvasMethod::inputParam(int i, void *param)
{

}

void crJXJShowJuQingCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strImg = str;
		break;
	case 2:
		m_strName = str;
		break;
	case 3:
		m_strInfo = str;
		break;
	case 4:
		m_time = atof(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJShowJuQingCanvasMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);

	if (canvas)
	{
		crData *canvasData = canvas->getDataClass();
		if (canvasData)
		{
			canvasData->inputParam(WCHDATA_JXJJuQingImg, &m_strImg);
			canvasData->inputParam(WCHDATA_JXJJuQingName, &m_strName);
			canvasData->inputParam(WCHDATA_JXJJuQingInfo, &m_strInfo);
			canvasData->inputParam(WCHDATA_JXJJuQingCanvasShowTimer, &m_time);


			crFilterRenderManager::getInstance()->showCanvas(canvas, true);
		}
	}
	
}

/////////////////////////////////////////
//
//crJXJCameraMouseMoveMethod
//
/////////////////////////////////////////
crJXJMouseOnUnwalkableCroodMethod::crJXJMouseOnUnwalkableCroodMethod()
{
}
crJXJMouseOnUnwalkableCroodMethod::crJXJMouseOnUnwalkableCroodMethod(const crJXJMouseOnUnwalkableCroodMethod& handle) :
crMethod(handle)
{
}
void crJXJMouseOnUnwalkableCroodMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJMouseOnUnwalkableCroodMethod::addParam(int i, const std::string& str)
{

}
void crJXJMouseOnUnwalkableCroodMethod::operator()(crHandle &handle)
{
	bool mouseOnEdge = false;
	if (m_ea)
	{
		float x = m_ea->getXnormalized();
		float y = m_ea->getYnormalized();

		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		crSceneLayer *scenelayer = crMyPlayerData::getInstance()->getSceneLayer();
		if (scenelayer && bindview && bindview->isInited() && crFilterRenderManager::getInstance()->getFocusNode() == NULL)
		{
			CRCore::crIntersectVisitor::HitList hits;
			bindview->computeIntersections(m_ea->getXnormalized(), m_ea->getYnormalized(), crSceneManager::getInstance()->getStaticRoot(), hits,~AutoTransMask);
			if (!hits.empty())
			{
				CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
				crVector2s coord = scenelayer->getCoord(crVector2(pickPoint[0], pickPoint[1]));
				mouseOnEdge = !scenelayer->walkability(coord[0], coord[1]);
			}
		}
	}
	handle.outputParam(0, &mouseOnEdge);
}

/////////////////////////////////////////
//
//crJXJCameraMouseMoveMethod
//
/////////////////////////////////////////
crJXJInitCurrentInstanceMethod::crJXJInitCurrentInstanceMethod()
{
}
crJXJInitCurrentInstanceMethod::crJXJInitCurrentInstanceMethod(const crJXJInitCurrentInstanceMethod& handle) :
crMethod(handle)
{
}
void crJXJInitCurrentInstanceMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJInitCurrentInstanceMethod::addParam(int i, const std::string& str)
{

}
void crJXJInitCurrentInstanceMethod::operator()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	if (canvasData)
	{
		canvasData->excHandle(MAKEINT64(WCH_LockData, 1));
		canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap, param);
		RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;

		ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
		if (camera.valid())
		{
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			ref_ptr<crJXJRoomMemberInfo> roomMemberInfo = new crJXJRoomMemberInfo;
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_SelectMyRoleVec, param);
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			if (playerGameData && playerGameData->getMainRole())
			{
				roomMemberInfo->setValid(true);
				crData *playerData = playerGameData->getDataClass();
				crData *mainRoleData = playerGameData->getMainRole()->getDataClass();
				if (playerData && mainRoleData)
				{
					playerData->getParam(WCHDATA_PlayerIconID, param);
					unsigned char playerIconID = *(unsigned char*)param;
					roomMemberInfo->setPlayerIconID(playerIconID);

					mainRoleData->getParam(WCHDATA_Level, param);
					unsigned char playerLv = *(unsigned char *)param;
					roomMemberInfo->setPlayerLv(playerLv);

					float playerBF = 0;
					playerGameData->getMainRole()->doEvent(WCH_JXJGetBingFa, MAKEINT64(&playerBF, NULL));
					roomMemberInfo->setBingFa((unsigned short)playerBF);

					float playerZF = 0;
					playerGameData->getMainRole()->doEvent(WCH_JXJGetZhenFa, MAKEINT64(&playerZF, NULL));
					roomMemberInfo->setZhenFa((unsigned short)playerZF);

					float playerDC = 0;
					playerGameData->getMainRole()->doEvent(WCH_JXJGetDongCha, MAKEINT64(&playerDC, NULL));
					roomMemberInfo->setDongCha((unsigned short)playerDC);

					mainRoleData->getParam(WCHDATA_JXJLordEquipVec, param);
					JXJLordEquipVec *equipvec = (JXJLordEquipVec *)param;
					roomMemberInfo->getEquipVec() = *equipvec;

					mainRoleData->getParam(WCHDATA_JXJGovPost, param);//官职
					unsigned char govpost = *(unsigned char *)param;
					roomMemberInfo->setPlayerGovPos(govpost);

					mainRoleData->getParam(WCHDATA_JXJExploit, param);//功勋
					int exploit = *(int *)param;
					roomMemberInfo->setPlayerExploit(exploit);

					float tongshuai = 0;
					m_this->doEvent(WCH_JXJGetTongShuai, MAKEINT64(&tongshuai, NULL));
					roomMemberInfo->setPlayerLeaderPower((short)tongshuai);

					roomMemberInfo->setCharacterName(playerGameData->getCharacterName());

					//int output = 0;
					//playerGameData->doEvent(WCH_JXJGetPlayerFightingPower, MAKEINT64(&output, NULL));
					//roomMemberInfo->setFightPower(output);

					infoMap->insert(make_pair(playerGameData->getPlayerID(), roomMemberInfo.get()));

					ref_ptr<crInstanceItem> role = crMyPlayerData::getInstance()->getLoadedRoleByIndex(0);
					canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect, role.get());
				}
			}
			
		}

		canvasData->excHandle(MAKEINT64(WCH_LockData, 0));
	}
}

/////////////////////////////////////////
//
//crJXJChangeCurSelectRoleMethod
//
/////////////////////////////////////////
crJXJChangeCurSelectRoleMethod::crJXJChangeCurSelectRoleMethod()
{
}
crJXJChangeCurSelectRoleMethod::crJXJChangeCurSelectRoleMethod(const crJXJChangeCurSelectRoleMethod& handle) :
crMethod(handle)
{
}
void crJXJChangeCurSelectRoleMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform *)param;
		break;
	}
}
void crJXJChangeCurSelectRoleMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}
void crJXJChangeCurSelectRoleMethod::operator()(crHandle &handle)
{
	void *param;
	crData *cameraData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_SelectMyRoleVec, param);
	SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;

	if (!selectMyRoles->empty())
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if (canvas.valid())
		{
			crData * canvasData = canvas->getDataClass();
			if (canvasData)
			{
				canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect, (*selectMyRoles)[0].get());
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJInsertBattleReportDisposeMethod
//
/////////////////////////////////////////
crJXJInsertBattleReportDisposeMethod::crJXJInsertBattleReportDisposeMethod() :
m_this(NULL)
{
}
crJXJInsertBattleReportDisposeMethod::crJXJInsertBattleReportDisposeMethod(const crJXJInsertBattleReportDisposeMethod& handle) :
crMethod(handle),
m_this(NULL)
{
}
void crJXJInsertBattleReportDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}

void crJXJInsertBattleReportDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailCfg = str;
		break;
	}
}

void crJXJInsertBattleReportDisposeMethod::operator()(crHandle &handle)
{
	//插入数据库 如果插入成功，根据返回keyValue 生成通告战报和邮件战报
	void *param;
	crData *serverData = m_this->getDataClass();
	serverData->excHandle(MAKEINT64(WCH_LockData, 1));
	serverData->getParam(WCHDATA_JXJInsertBattleReportDeq, param);
	InsertBattleReportDeq *disposeDeq = (InsertBattleReportDeq *)param;
	serverData->getParam(WCHDATA_JXJMailTaskList, param);
	MailTaskDeque *mailDeq = (MailTaskDeque *)param;

	if (!disposeDeq->empty())
	{
		ref_ptr<crBattleReportInfo> battleReport = disposeDeq->front();
		disposeDeq->pop_front();
		//数据库中插入文件

		ref_ptr<crBattleReportsInsert> insertBattleReport = new crBattleReportsInsert;
		crDataBase *gameglobaldb = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crDataBase> dbSession = gameglobaldb->beginSession();
		int insertid = 0;
		std::string createname;
		//crDBSavePacket packet;

		insertBattleReport->buildUpdateSql(battleReport->getBattleID(), battleReport->getBattleData());
		if (dbSession->executeUpdate(insertBattleReport.get()))
		{
			insertid = dbSession->getInsertID();
			dbSession->commit();
		}
		gameglobaldb->endSession(dbSession.get());

		if (insertid > 0)
		{
			//生成邮件
			rcfg::ConfigScript cfg_script;
			if (crScriptLoadManager::getInstance()->loadConfigScript(cfg_script, m_mailCfg, true, ""))
			{
				char textBuf[64];
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content", 1);
				cfg_script.Add("DrawMode", "TEXT");
				cfg_script.Add("Text", battleReport->getTextInfo());
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content", 2);
				cfg_script.Add("DrawMode", "TEXT|UNDERLINE");
				cfg_script.Add("Text", std::string("详细战报"));
				cfg_script.Add("Link", 3);
				cfg_script.Pop();
				cfg_script.Pop();

				//cfg_script.Add("Data");
				cfg_script.Push("Data");
				sprintf(textBuf, "%d,%d\0", WCHDATA_JXJBattleReportID, insertid);
				cfg_script.Add("Param", textBuf);

				cfg_script.Pop();

				//cfg_script.Write("JXJ/script/giftstest.cfg");

				ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
				ref_ptr<crMailInfo> mailInfo; 
				crGlobalHandle::getInstance()->getScriptStream(cfg_script, mailBuffer);

				for (auto itr = battleReport->getPlayerVec().begin(); \
					itr != battleReport->getPlayerVec().end();itr++)
				{
					mailInfo = new crMailInfo;
					mailInfo->setRecvID(*itr);
					mailInfo->setSenderName("系统");
					mailInfo->setSenderID(0);
					mailInfo->setTitle(battleReport->getMailTitle());
					mailInfo->setBody(mailBuffer.get());

					mailDeq->push_back(mailInfo.get());
				}
					
				cfg_script.Close();
			}
			//生成通告信息
			int battleid = battleReport->getBattleID();
			unsigned char aShiliid = (unsigned char)(LOINT16(LOINT32(battleid)));
			unsigned char dShiliid = (unsigned char)(HIINT16(LOINT32(battleid)));

			std::string strMessage = std::string("^5^") + battleReport->getTextInfo() + std::string("{") + \
				crArgumentParser::appItoa(0) + std::string(",") + crArgumentParser::appItoa(insertid) + std::string("}");

			ref_ptr<crJXJChatMessageInfo> achatInfo = new crJXJChatMessageInfo("[通告]", strMessage, 0, aShiliid);
			ref_ptr<crJXJChatMessageInfo> dchatInfo = new crJXJChatMessageInfo("[通告]", strMessage, 0, dShiliid);
			m_this->doEvent(WCH_JXJSystermNoticeCreate, MAKEINT64(achatInfo.get(), NULL));
			m_this->doEvent(WCH_JXJSystermNoticeCreate, MAKEINT64(dchatInfo.get(), NULL));
		}
	}

	serverData->excHandle(MAKEINT64(WCH_LockData, 0));
}

/////////////////////////////////////////
//
//crJXJQueryBattleReportDisposeMethod
//
/////////////////////////////////////////
crJXJQueryBattleReportDisposeMethod::crJXJQueryBattleReportDisposeMethod() :
m_this(NULL),
m_taskcount(5)
{
}
crJXJQueryBattleReportDisposeMethod::crJXJQueryBattleReportDisposeMethod(const crJXJQueryBattleReportDisposeMethod& handle) :
crMethod(handle),
m_this(NULL),
m_taskcount(handle.m_taskcount)
{
}
void crJXJQueryBattleReportDisposeMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}

void crJXJQueryBattleReportDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}
}

void crJXJQueryBattleReportDisposeMethod::operator()(crHandle &handle)
{
	m_taskVec.resize(0);
	m_taskVec.reserve(m_taskcount);
	std::pair<int, int> playerQueryPair;
	void *param;
	crData *gameData = m_this->getDataClass();
	gameData->excHandle(MAKEINT64(WCH_LockData, 1));
	gameData->getParam(WCHDATA_JXJQueryBattleReportDeq, param);
	QueryBattleReportDeq *queryDeq = (QueryBattleReportDeq *)param;
	int i = 0;
	while(!queryDeq->empty() && i<m_taskcount)
	{
		playerQueryPair = queryDeq->front();
		queryDeq->pop_front();
		m_taskVec.push_back(playerQueryPair);
		i++;
	}
	gameData->excHandle(MAKEINT64(WCH_LockData, 0));

	if (!m_taskVec.empty())
	{
		crDataBase *globaldb = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
		ref_ptr<crDataBase> dbSession;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();

		ref_ptr<crGameServerPlayerData> player;
		for( std::vector< std::pair<int,int> >::iterator titr = m_taskVec.begin();
			titr != m_taskVec.end();
			++titr )
		{
			playerQueryPair = *titr;
			player = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerQueryPair.first));
			if (player.valid() && player->getPlayerGameData())
			{
				crPlayerGameData *playerGameData = player->getPlayerGameData();
				crData * playerData = playerGameData->getDataClass();
				if (playerData)
				{
					dbSession = globaldb->beginSession();
					ref_ptr<crBattleReportsQueryData> battleReportQuery = new crBattleReportsQueryData;
					battleReportQuery->buildQuerySql(playerQueryPair.second);
					if (dbSession->executeQuery(battleReportQuery.get()))
					{
						crDataBase::QueryResultVec &queryResults = dbSession->getQueryResult();
						if (!queryResults.empty())
						{
							crBattleReportsQueryData *battleReport = dynamic_cast<crBattleReportsQueryData *>(queryResults[0].get());
							if (battleReport)
							{
								//发送到客户端
								ref_ptr<crStreamBuf> tempStream = battleReport->getStream();
								int length = tempStream->getBufSize();

								ref_ptr<crStreamBuf> resultStream = new crStreamBuf;
								resultStream->createBuf(4 + length);
								resultStream->_writeInt(battleReport->getBattleID());
								resultStream->write(tempStream->getBuf(),length);

								crPlayerDataEventPacket packet;
								packet.buildReplyPacket(packet, playerQueryPair.first, WCH_JXJRecvQueryBattleReport, resultStream.get());
								gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(), packet);
							}
						}
					}
					dbSession->releaseQuery();
					globaldb->endSession(dbSession.get());
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJMailQueryBattleReportMethod
//
/////////////////////////////////////////
crJXJMailQueryBattleReportMethod::crJXJMailQueryBattleReportMethod() :m_strBattleReportTab("BattleReports.crb")
{
}

crJXJMailQueryBattleReportMethod::crJXJMailQueryBattleReportMethod(const crJXJMailQueryBattleReportMethod& handle) :
crMethod(handle),
m_strCanvas(handle.m_strCanvas),
m_strBattleReportTab("BattleReports.crb")
{
}

void crJXJMailQueryBattleReportMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crHypertextWidgetNode*)param;
		break;
	case 2:
		if (param)
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

void crJXJMailQueryBattleReportMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strBattleReportTab = str;
		break;
	default:
		break;
	}
}

void crJXJMailQueryBattleReportMethod::operator()(crHandle &handle)
{
	if (m_ea && m_this)
	{
		void *param;
		//获取data
		crData *data = m_this->getDataClass();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (data)
		{
			data->getParam(WCHDATA_JXJBattleReportID, param);
			int battleReportID = *(int *)param;
			int playerid = crMyPlayerData::getInstance()->getPlayerID();
			crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			if (playerData)
			{
				playerData->inputParam(WCHDATA_JXJCurQueryBattleReportID, &battleReportID);
			}

			crCanvasNode * canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if (canvas)
			{
				crData * canvasData = canvas->getDataClass();
				if (canvasData)
				{
					std::string playerName = crMyPlayerData::getInstance()->getCharacterName();
					canvasData->inputParam(WCHDATA_JXJNeedQueryPlayerName, &playerName);
				}
			}
			
			bool showCanvas = false;
			playerData->excHandle(MAKEINT64(WCH_LockData, 1));

			playerData->getParam(WCHDATA_JXJQueryedBattleReportInfo, param);
			BattleReportInfoMap * battleInfoMap = (BattleReportInfoMap *)param;

			if(battleInfoMap->find(battleReportID) == battleInfoMap->end())
			{
				//查找本地文件
				std::string fileName = crGlobalHandle::getInstance()->getGameDir() + m_strBattleReportTab;
				ref_ptr<crTableIO> battleReportTab = crTableIO::openFile(fileName);
				crTableIO::StrVec record, reportInfo;
				string strTemp;
				int index = 0;
				int battleid = 0;

				if (battleReportTab.valid() && battleReportTab->queryOneRecord(0, crArgumentParser::appItoa(battleReportID), record)>=0)
				{
					int infoindex = battleReportTab->getTitleIndex("battleinfo");
					int idIndex = battleReportTab->getTitleIndex("battleid");
					if (infoindex >= 0 && idIndex >= 0)
					{
						battleid = atoi(record[idIndex].c_str());

						strTemp = record[infoindex];
						index = strTemp.find_first_of("|");
						while (index != string::npos)
						{
							reportInfo.push_back(strTemp.substr(0, index));
							strTemp = strTemp.substr(index + 1);
							index = strTemp.find_first_of("|");
						}
						reportInfo.push_back(strTemp);
					}
				}

				if (!reportInfo.empty())
				{
					index = 0;

					crJXJBattleReportInfo tempBattleReport;
					tempBattleReport.setBattleId(battleid);

					std::string strBattleInfo;
					std::string battleName = reportInfo[index]; index++;
					tempBattleReport.setBattleName(battleName.c_str());
					bool isAttackWin = atoi(reportInfo[index].c_str()); index++;
					tempBattleReport.setAttackWin(isAttackWin);

					unsigned short size = atoi(reportInfo[index].c_str()); index++;

					PlayerBattleReport  playerReportInfo;
					std::string playerName;
					for (int i = 0; i < size; i++)
					{
						playerReportInfo.info.first = 0;
						int killcount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.second = killcount;
						int deadcount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.third = deadcount;
						float exploit = atof(reportInfo[index].c_str()); index++;
						playerReportInfo.exploit = (int)exploit;
						float exp = atof(reportInfo[index].c_str()); index++;
						playerReportInfo.experience = (int)exp;
						playerName = reportInfo[index]; index++;

						int buduicount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.first = buduicount;

						tempBattleReport.getReportPlayerInfoMap().insert(std::make_pair(playerName, playerReportInfo));
					}
					int ashiliTotalRtCount = atoi(reportInfo[index].c_str()); index++;
					int ashiliTotalDeadCount = atoi(reportInfo[index].c_str()); index++;
					int dshiliTotalRtCount = atoi(reportInfo[index].c_str()); index++;
					int dshiliTotalDeadCount = atoi(reportInfo[index].c_str()); index++;

					BattleReportThree totalInfo, totalDeadInfo;
					totalInfo.second = ashiliTotalDeadCount + ashiliTotalRtCount;
					totalInfo.third = dshiliTotalDeadCount + dshiliTotalRtCount;
					totalInfo.first = totalInfo.second + totalInfo.third;

					totalDeadInfo.second = ashiliTotalDeadCount;
					totalDeadInfo.third = dshiliTotalDeadCount;
					totalDeadInfo.first = totalDeadInfo.second + totalDeadInfo.third;

					tempBattleReport.setTotal(totalInfo);
					tempBattleReport.seTotalDead(totalDeadInfo);

					battleInfoMap->insert(std::make_pair(battleReportID, tempBattleReport));
					showCanvas = true;
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
					streamBuf->createBuf(4);
					streamBuf->_writeInt(battleReportID);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvQueryBattleReport, streamBuf.get());
					netConductor->getNetManager()->sendPacket("all", packet);
				}
			}
			else
			{
				showCanvas = true;
			}
			playerData->excHandle(MAKEINT64(WCH_LockData, 0));

			
			if (canvas && showCanvas == true)
				crFilterRenderManager::getInstance()->showCanvas(canvas, true);
			
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvQueryBattleReportMethod
//
/////////////////////////////////////////
crJXJRecvQueryBattleReportMethod::crJXJRecvQueryBattleReportMethod()
{
}

crJXJRecvQueryBattleReportMethod::crJXJRecvQueryBattleReportMethod(const crJXJRecvQueryBattleReportMethod& handle) :
crMethod(handle)
{
}

void crJXJRecvQueryBattleReportMethod::inputParam(int i, void *param)
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

void crJXJRecvQueryBattleReportMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strTabFileName = str;
		break;
	default:
		break;
	}
}

void crJXJRecvQueryBattleReportMethod::operator()(crHandle &handle)
{
	if (m_netType == GameServer)
	{
		int playerId = m_this->getPlayerID();
		int queryId = m_stream->_readInt();

		crServerBrainHandle * gameSerevr = crServerBrainHandle::getInstance();
		crData * serverData = gameSerevr->getDataClass();
		if (serverData)
		{
			void *param;
			serverData->excHandle(MAKEINT64(WCH_LockData, 1));
			serverData->getParam(WCHDATA_JXJQueryBattleReportDeq, param);
			QueryBattleReportDeq *queryDeq = (QueryBattleReportDeq *)param;

			queryDeq->push_back(std::make_pair(playerId, queryId));
			serverData->excHandle(MAKEINT64(WCH_LockData, 0));
		}
	}
	else if (m_netType == GameClient_Game)
	{
		void *param;
		crData * playerData = m_this->getDataClass();
		if (playerData)
		{
			crJXJBattleReportInfo tempBattleReport;
			crTableIO::StrVec dataVec;
			playerData->getParam(WCHDATA_JXJCurQueryBattleReportID, param);
			int curBattleReportId = *(int *)param;
			dataVec.push_back(crArgumentParser::appItoa(curBattleReportId));
			//PlayerBattleReport = 
			int battleid = m_stream->_readInt();
			tempBattleReport.setBattleId(battleid);
			dataVec.push_back(crArgumentParser::appItoa(battleid));
// 			int remainSize = m_stream->getRemainSize();
// 			ref_ptr<crStreamBuf> tempStream = new crStreamBuf;
// 			tempStream->createBuf(m_stream->getRemainSize());
// 			char * buffer = new char[remainSize];
// 			m_stream->read(buffer, remainSize);
// 			tempStream->write(m_stream->getPtr(), m_stream->getRemainSize());

			creUnCompress(m_stream);

			std::string strBattleInfo;
			std::string battleName = m_stream->_readString(); strBattleInfo += battleName + std::string("|");
			tempBattleReport.setBattleName(battleName.c_str());
			bool isAttackWin = m_stream->_readBool(); strBattleInfo += crArgumentParser::appItoa(isAttackWin) + std::string("|");
			tempBattleReport.setAttackWin(isAttackWin);

			unsigned short size = m_stream->_readUShort(); 
			strBattleInfo += crArgumentParser::appItoa(size) + std::string("|");

			PlayerBattleReport  playerReportInfo;
			std::string playerName;
			for (int i = 0; i < size;i++)
			{
				playerReportInfo.info.first = 0;
				int killcount = m_stream->_readInt();
				strBattleInfo += crArgumentParser::appItoa(killcount) + std::string("|");
				playerReportInfo.info.second = killcount;
				int deadcount = m_stream->_readInt();
				strBattleInfo += crArgumentParser::appItoa(deadcount) + std::string("|");
				playerReportInfo.info.third = deadcount;
				float exploit = m_stream->_readFloat();
				strBattleInfo += crArgumentParser::appFtoa(exploit) + std::string("|");
				playerReportInfo.exploit = (int)exploit;
				float exp = m_stream->_readFloat();
				strBattleInfo += crArgumentParser::appFtoa(exp) + std::string("|");
				playerReportInfo.experience = (int)exp;
				playerName = m_stream->_readString();
				strBattleInfo += playerName + std::string("|");

				int buduishu = m_stream->_readInt();
				playerReportInfo.info.first = buduishu;
				strBattleInfo += crArgumentParser::appItoa(buduishu) + std::string("|");

				tempBattleReport.getReportPlayerInfoMap().insert(std::make_pair(playerName,playerReportInfo));
			}
			int ashiliTotalRtCount = m_stream->_readInt();
			strBattleInfo += crArgumentParser::appItoa(ashiliTotalRtCount) + std::string("|");
			int ashiliTotalDeadCount = m_stream->_readInt();
			strBattleInfo += crArgumentParser::appItoa(ashiliTotalDeadCount) + std::string("|");
			int dshiliTotalRtCount = m_stream->_readInt();
			strBattleInfo += crArgumentParser::appItoa(dshiliTotalRtCount) + std::string("|");
			int dshiliTotalDeadCount = m_stream->_readInt();
			strBattleInfo += crArgumentParser::appItoa(dshiliTotalDeadCount);
			dataVec.push_back(strBattleInfo);

			BattleReportThree totalInfo,totalDeadInfo;
			totalInfo.second = ashiliTotalDeadCount + ashiliTotalRtCount;
			totalInfo.third = dshiliTotalDeadCount + dshiliTotalRtCount;
			totalInfo.first = totalInfo.second + totalInfo.third;

			totalDeadInfo.second = ashiliTotalDeadCount;
			totalDeadInfo.third = dshiliTotalDeadCount;
			totalDeadInfo.first = totalDeadInfo.second + totalDeadInfo.third;

			tempBattleReport.setTotal(totalInfo);
			tempBattleReport.seTotalDead(totalDeadInfo);

			playerData->excHandle(MAKEINT64(WCH_LockData, 1));
			playerData->getParam(WCHDATA_JXJCurQueryBattleReportID, param);
			int curQueryID = *(int *)param;
			playerData->getParam(WCHDATA_JXJQueryedBattleReportInfo, param);
			BattleReportInfoMap * battleInfoMap = (BattleReportInfoMap *)param;

			battleInfoMap->insert(std::make_pair(curQueryID, tempBattleReport));
			playerData->excHandle(MAKEINT64(WCH_LockData, 0));

			crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if (canvas)
			{
				crFilterRenderManager::getInstance()->showCanvas(canvas, true);
// 				crData *canvasData = canvas->getDataClass();
// 				if (canvasData)
// 				{
// 					canvasData->inputParam(WCHDATA_JXJNeedQueryPlayerName, NULL);
// 					playerData->inputParam(WCHDATA_JXJCurQueryBattleReportID, NULL);
// 				}
			}

			//将stream存入本地文件
			ref_ptr<crTableIO> battleReportTab = crTableIO::openFile(crGlobalHandle::getInstance()->getGameDir() + m_strTabFileName);
			if (battleReportTab.valid())
			{
				battleReportTab->addData(dataVec);
				battleReportTab->saveToFileStream(crGlobalHandle::getInstance()->getGameDir() + m_strTabFileName);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJShowBattleReportLinkClickMethod
//
/////////////////////////////////////////
crJXJShowBattleReportLinkClickMethod::crJXJShowBattleReportLinkClickMethod() :
m_ea(NULL),
m_this(NULL),
m_reportID(0)
{
}
crJXJShowBattleReportLinkClickMethod::crJXJShowBattleReportLinkClickMethod(const crJXJShowBattleReportLinkClickMethod& handle) :
crMethod(handle),
m_this(handle.m_this),
m_ea(handle.m_ea),
m_linkInfo(handle.m_linkInfo),
m_playerName(handle.m_playerName),
m_reportID(handle.m_reportID),
m_strReportCanvas(handle.m_strReportCanvas),
m_strBattleReportTab(handle.m_strBattleReportTab)
{
}
void crJXJShowBattleReportLinkClickMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
			m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_playerName = *(std::string*)param;
		break;
	case 4:
		m_reportID = *(int*)param;
		break;
	}
}
void crJXJShowBattleReportLinkClickMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strReportCanvas = str;
		break;
	case 1:
		m_strBattleReportTab = str;
		break;
	}
}
void crJXJShowBattleReportLinkClickMethod::operator()(crHandle &handle)
{
	if (m_reportID == 0) return;

	crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strReportCanvas);
	if (canvas)
	{
		crData *canvasData = canvas->getDataClass();
		if (canvasData)
		{
			if (m_playerName.empty() || \
				m_playerName.compare("0") == 0)
			{
				m_playerName = crMyPlayerData::getInstance()->getCharacterName();
			}
			
			canvasData->inputParam(WCHDATA_JXJNeedQueryPlayerName, &m_playerName);			
			
			//发送查询包到服务器
			void *param;
			bool bShowCanvas = false;
			crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			playerData->excHandle(MAKEINT64(WCH_LockData, 1));
			playerData->inputParam(WCHDATA_JXJCurQueryBattleReportID, &m_reportID);
			playerData->getParam(WCHDATA_JXJQueryedBattleReportInfo, param);
			BattleReportInfoMap * battleInfoMap = (BattleReportInfoMap *)param;

			if(battleInfoMap->find(m_reportID) == battleInfoMap->end())
			{
				//查找本地文件
				ref_ptr<crTableIO> battleReportTab = crTableIO::openFile(crGlobalHandle::getInstance()->getGameDir() + m_strBattleReportTab);
				crTableIO::StrVec record,reportInfo;
				string strTemp;
				int index = 0;
				int battleid = 0;

				if (battleReportTab.valid() && battleReportTab->queryOneRecord(0, crArgumentParser::appItoa(m_reportID), record)>=0)
				{
					int infoindex = battleReportTab->getTitleIndex("battleinfo");
					int idIndex = battleReportTab->getTitleIndex("battleid");
					if (infoindex >= 0 && idIndex>=0)
					{
						battleid = atoi(record[idIndex].c_str());

						strTemp = record[infoindex];
						index = strTemp.find_first_of("|");
						while (index != string::npos)
						{
							reportInfo.push_back(strTemp.substr(0, index));
							strTemp = strTemp.substr(index+1);
							index = strTemp.find_first_of("|");
						}
						reportInfo.push_back(strTemp);
					}
				}

				if (!reportInfo.empty())
				{
					index = 0;

					crJXJBattleReportInfo tempBattleReport;
					tempBattleReport.setBattleId(battleid);

					std::string strBattleInfo;
					std::string battleName = reportInfo[index]; index++;
					tempBattleReport.setBattleName(battleName.c_str());
					bool isAttackWin = atoi(reportInfo[index].c_str()); index++;
					tempBattleReport.setAttackWin(isAttackWin);

					unsigned short size = atoi(reportInfo[index].c_str()); index++;
					
					PlayerBattleReport  playerReportInfo;
					std::string playerName;
					for (int i = 0; i < size; i++)
					{
						playerReportInfo.info.first = 0;
						int killcount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.second = killcount;
						int deadcount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.third = deadcount;
						float exploit = atof(reportInfo[index].c_str()); index++;
						playerReportInfo.exploit = (int)exploit;
						float exp = atof(reportInfo[index].c_str()); index++;
						playerReportInfo.experience = (int)exp;
						playerName = reportInfo[index]; index++;

						int buduicount = atoi(reportInfo[index].c_str()); index++;
						playerReportInfo.info.first = buduicount;

						tempBattleReport.getReportPlayerInfoMap().insert(std::make_pair(playerName, playerReportInfo));
					}
					int ashiliTotalRtCount = atoi(reportInfo[index].c_str()); index++;
					int ashiliTotalDeadCount = atoi(reportInfo[index].c_str()); index++;
					int dshiliTotalRtCount = atoi(reportInfo[index].c_str()); index++;
					int dshiliTotalDeadCount = atoi(reportInfo[index].c_str()); index++;

					BattleReportThree totalInfo, totalDeadInfo;
					totalInfo.second = ashiliTotalDeadCount + ashiliTotalRtCount;
					totalInfo.third = dshiliTotalDeadCount + dshiliTotalRtCount;
					totalInfo.first = totalInfo.second + totalInfo.third;

					totalDeadInfo.second = ashiliTotalDeadCount;
					totalDeadInfo.third = dshiliTotalDeadCount;
					totalDeadInfo.first = totalDeadInfo.second + totalDeadInfo.third;

					tempBattleReport.setTotal(totalInfo);
					tempBattleReport.seTotalDead(totalDeadInfo);

					battleInfoMap->insert(std::make_pair(m_reportID, tempBattleReport));
					bShowCanvas = true;
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
					streamBuf->createBuf(4);
					streamBuf->_writeInt(m_reportID);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvQueryBattleReport, streamBuf.get());
					netConductor->getNetManager()->sendPacket("all", packet);
				}				
			}
			else
			{
				bShowCanvas = true;
			}
			playerData->excHandle(MAKEINT64(WCH_LockData, 0));

			if (canvas && bShowCanvas == true)
			{
				crFilterRenderManager::getInstance()->showCanvas(canvas, true);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIZhanBaoUpdateMethod
//
/////////////////////////////////////////
crJXJUIZhanBaoUpdateMethod::crJXJUIZhanBaoUpdateMethod()
{
}
crJXJUIZhanBaoUpdateMethod::crJXJUIZhanBaoUpdateMethod(const crJXJUIZhanBaoUpdateMethod& handle) :
crMethod(handle)
{
}
void crJXJUIZhanBaoUpdateMethod::inputParam(int i, void *param)
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
void crJXJUIZhanBaoUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strAttackShiliSw = str;
		break;
	case 1:
		m_strDefendShiliSw = str;
		break;
	case 2:
		m_strWinSw = str;
		break;
	case 3:
		m_strBattleName = str;
		break;
	case 4:
		m_strTotal = str;
		break;
	case 5:
		m_strShiliTotal = str;
		break;
	case 6:
		m_strDeadTotal = str;
		break;
	case 7:
		m_strshiliDeadTotal = str;
		break;
	case 8:
		m_strkillList = str;
		break;
	case 9:
		m_strDeadList = str;
		break;
	case 10:
		m_strplayerTotal = str;
		break;
	case 11:
		m_strplayerKill = str;
		break;
	case 12:
		m_strplayerDead = str;
		break;
	case 13:
		m_strexp = str;
		break;
	case 14:
		m_strExploit = str;
		break;
	case 15:
		m_strExpIcon = str;
		break;
	case 16:
		m_strWxploitIcon = str;
		break;
	case 17:
		m_strexpImgName = str;
		break;
	case 18:
		m_strexploitImgName = str;
		break;
	case 19:
		m_strShowBt = str;
		break;
	case 20:
		m_strPlayerName = str;
		break;
	}
}
void crJXJUIZhanBaoUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();

	if (canvasData && playerData)
	{
		canvasData->getParam(WCHDATA_JXJNeedQueryPlayerName, param);
		std::string strQueryName = *(string *)param;

		ref_ptr<crButtonWidgetNode> btShow = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strShowBt));
		if (btShow.valid())
		{
			if (crMyPlayerData::getInstance()->getCharacterName().compare(strQueryName) == 0)
			{
				btShow->setVisiable(true);
			}
			else
			{
				btShow->setVisiable(false);
			}
		}
		ref_ptr<crMultiSwitch> attackSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strAttackShiliSw));
		ref_ptr<crMultiSwitch> defendkSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strDefendShiliSw));
		ref_ptr<crMultiSwitch> winSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strWinSw));
		ref_ptr<crStaticTextWidgetNode> textBattleName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strBattleName));

		ref_ptr<crTableWidgetNode> killList = dynamic_cast<crTableWidgetNode *>(m_this->getChildNode(m_strkillList));
		ref_ptr<crTableWidgetNode> deadList = dynamic_cast<crTableWidgetNode *>(m_this->getChildNode(m_strDeadList));
		if (killList.valid())killList->clearData();
		if (deadList.valid())deadList->clearData();

		ref_ptr<crStaticTextWidgetNode> textplayerTotal = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strplayerTotal));
		ref_ptr<crStaticTextWidgetNode> textplayerKill = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strplayerKill));
		ref_ptr<crStaticTextWidgetNode> textplayerDead = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strplayerDead));
		ref_ptr<crStaticTextWidgetNode> textplayerExp = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strexp));
		ref_ptr<crStaticTextWidgetNode> textplayerExploit = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strExploit));

		ref_ptr<crStaticTextWidgetNode> textTotalCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strTotal));
		ref_ptr<crStaticTextWidgetNode> textTotalShiliCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strShiliTotal));
		ref_ptr<crStaticTextWidgetNode> textTotalDeadCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strDeadTotal));
		ref_ptr<crStaticTextWidgetNode> textTotalShiliDeadCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strshiliDeadTotal));
		ref_ptr<crStaticTextWidgetNode> textPlayerName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strPlayerName));


		ref_ptr<crImageBoxWidgetNode> imgExp = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strExpIcon));
		ref_ptr<crImageBoxWidgetNode> imgExploit = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strWxploitIcon));
		if (imgExp.valid()) imgExp->setImageName(m_strexpImgName);
		if (imgExploit.valid()) imgExploit->setImageName(m_strexploitImgName);
		if(textPlayerName.valid()) textPlayerName->setString(strQueryName);

		playerData->getParam(WCHDATA_JXJCurQueryBattleReportID, param);
		int curQueryID = *(int *)param;

		playerData->excHandle(MAKEINT64(WCH_LockData, 1));
		playerData->getParam(WCHDATA_JXJQueryedBattleReportInfo, param);
		BattleReportInfoMap * battleInfoMap = (BattleReportInfoMap *)param;

		BattleReportInfoMap::iterator itr_map = battleInfoMap->find(curQueryID);
		if (itr_map != battleInfoMap->end())
		{
			int battleid = itr_map->second.getBattleId();
			unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
			unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));
			if (attackSw.valid())
				attackSw->setActiveSwitchSet(attackShili - c_startShiliID);
			if (defendkSw.valid())
			{
				if (defenceShili < c_startShiliID)
				{
					defendkSw->setActiveSwitchSet(3);
				}
				else
				{
					defendkSw->setActiveSwitchSet(defenceShili - c_startShiliID);
				}
			}
			
			if (winSw.valid())
				winSw->setActiveSwitchSet(!itr_map->second.getAttackWin());

			if (textBattleName.valid())
			{
				textBattleName->setString(itr_map->second.getBattleName());
			}

			PlayerBattleReportInfoMap::iterator itr_player = itr_map->second.getReportPlayerInfoMap().find(strQueryName);
			if (itr_player != itr_map->second.getReportPlayerInfoMap().end())
			{
				textplayerTotal->setString(crArgumentParser::appItoa(itr_player->second.info.first));
				textplayerKill->setString(crArgumentParser::appItoa(itr_player->second.info.second));
				textplayerDead->setString(crArgumentParser::appItoa(itr_player->second.info.third));
				textplayerExp->setString(crArgumentParser::appItoa(itr_player->second.experience));
				textplayerExploit->setString(crArgumentParser::appItoa(itr_player->second.exploit));
			}
			//
			ReportPlayerSet killSet, deadSet;
			ReportPlayerInfo infokill,infodead;
			itr_player = itr_map->second.getReportPlayerInfoMap().begin();
			for (; itr_player != itr_map->second.getReportPlayerInfoMap().end(); itr_player++)
			{
				infokill.count = itr_player->second.info.second;
				infodead.count = itr_player->second.info.third;
				infokill.name = itr_player->first;
				infodead.name = itr_player->first;

				killSet.insert(infokill);
				deadSet.insert(infodead);
			}

			if (killList.valid())
			{
				ReportPlayerSet::iterator itr = killSet.begin();
				for (int i = 0; i < 3, itr != killSet.end();itr++,i++)
				{
					killList->addData(0, i,itr->name);
					killList->addData(1, i, crArgumentParser::appItoa(itr->count));
				}
			}
			if (deadList.valid())
			{
				ReportPlayerSet::iterator itr = deadSet.begin();
				for (int i = 0; i < 3, itr != deadSet.end(); itr++, i++)
				{
					deadList->addData(0, i, itr->name);
					deadList->addData(1, i, crArgumentParser::appItoa(itr->count));
				}
			}

			crTableIO::StrVec record, record2;
			std::string aShiliName, dShiliName;
			ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			if (playershilitab.valid() && \
				playershilitab->queryOneRecord(0, crArgumentParser::appItoa(attackShili), record) >= 0 && \
				playershilitab->queryOneRecord(0, crArgumentParser::appItoa(defenceShili), record2) >= 0)
			{
				int shilinameindex = playershilitab->getTitleIndex("name");
				if (shilinameindex >= 0)
				{
					aShiliName = record[shilinameindex];
					dShiliName = record2[shilinameindex];
				}
			}
			//
			if (textTotalCount.valid())
				textTotalCount->setString(crArgumentParser::appItoa(itr_map->second.getTotal().first));
			if (textTotalShiliCount.valid() && !aShiliName.empty() && !dShiliName.empty())
			{
				std::string str = aShiliName + std::string("国:") + crArgumentParser::appItoa(itr_map->second.getTotal().second) + \
					std::string("   ") + dShiliName + std::string("国:") + crArgumentParser::appItoa(itr_map->second.getTotal().third);
				textTotalShiliCount->setString(str);
			}

			if (textTotalDeadCount.valid())
				textTotalDeadCount->setString(crArgumentParser::appItoa(itr_map->second.geTotalDead().first));
			if (textTotalShiliDeadCount.valid() && !aShiliName.empty() && !dShiliName.empty())
			{
				std::string str = aShiliName + std::string("国:") + crArgumentParser::appItoa(itr_map->second.geTotalDead().second) + \
					std::string("   ") + dShiliName + std::string("国:") + crArgumentParser::appItoa(itr_map->second.geTotalDead().third);
				textTotalShiliDeadCount->setString(str);
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData, 0));
	}
	
}

/////////////////////////////////////////
//
//crJXJUIShowBattleReportInfoMethod
//
/////////////////////////////////////////
crJXJUIShowBattleReportInfoMethod::crJXJUIShowBattleReportInfoMethod()
{
}

crJXJUIShowBattleReportInfoMethod::crJXJUIShowBattleReportInfoMethod(const crJXJUIShowBattleReportInfoMethod& handle) :
crMethod(handle),
m_canvasname(handle.m_canvasname),
m_chatmodesw(handle.m_chatmodesw),
m_chatinput(handle.m_chatinput),
m_sendbtn(handle.m_sendbtn)
{
}

void crJXJUIShowBattleReportInfoMethod::inputParam(int i, void *param)
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

void crJXJUIShowBattleReportInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasname = str;
		break;
	case 1:
		m_chatmodesw = str;
		break;
	case 2:
		m_chatinput = str;
		break;
	case 3:
		m_sendbtn = str;
		break;
	case 4:
		m_switch = str;
		break;
	case 5:
		m_strBattleCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJUIShowBattleReportInfoMethod::operator()(crHandle &handle)
{
	void *param;
	if (!m_this)return;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (!myPlayer)return;
	crData *data = myPlayer->getDataClass();
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
	if (!canvas) canvas = crFilterRenderManager::getInstance()->findCanvas(m_strBattleCanvas);
	if (!canvas) return;
	ref_ptr<crRadioGroupWidgetNode> chatmodesw = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_chatmodesw));
	ref_ptr<crStaticTextWidgetNode> chatinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_chatinput));
	ref_ptr<crButtonWidgetNode> sendbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_sendbtn));
	crMultiSwitch *mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
	if (!chatmodesw || !chatinput || !sendbtn || !mulitSwitch)return;
	ref_ptr<crTableIO>chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	//chatmodesw->select(ChatInCountry);
	//chatmodesw->doEvent(WCH_UI_SelectChange);
	//int curSelect = mulitSwitch->getActiveSwitchSet()+1;

	crTableIO::StrVec record;
	std::string name;
	std::string strMessage;
	int textid = 0;
	std::string myplayerName = crMyPlayerData::getInstance()->getCharacterName();

	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if (playerData)
	{
		playerData->getParam(WCHDATA_JXJCurQueryBattleReportID, param);
		int curQueryID = *(int *)param;

		playerData->excHandle(MAKEINT64(WCH_LockData, 1));
		playerData->getParam(WCHDATA_JXJQueryedBattleReportInfo, param);
		BattleReportInfoMap * battleInfoMap = (BattleReportInfoMap *)param;

		
		BattleReportInfoMap::iterator itr_map = battleInfoMap->find(curQueryID);
		PlayerBattleReportInfoMap & playerInfoMap = itr_map->second.getReportPlayerInfoMap();
		if (itr_map != battleInfoMap->end() && playerInfoMap.find(myplayerName)!=playerInfoMap.end())
		{			
			strMessage = std::string("^5^") + std::string("玩家[") + myplayerName + std::string("]展示了他在[") + itr_map->second.getBattleName() +std::string("]之战中的战报。")\
				+ std::string("{") + myplayerName + std::string(",") + crArgumentParser::appItoa(curQueryID) + std::string("}");
		}

		playerData->excHandle(MAKEINT64(WCH_LockData, 0));
	}


	if (!strMessage.empty())
	{
		chatinput->setString(strMessage);
		sendbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
		chatinput->clearString();
	}
}

/////////////////////////////////////////
//
//crJXJAddStarToPlayerMethod
//
/////////////////////////////////////////
crJXJAddStarToPlayerMethod::crJXJAddStarToPlayerMethod() :
m_star(0),m_index(crVector3b(0,0,0)){}
crJXJAddStarToPlayerMethod::crJXJAddStarToPlayerMethod(const crJXJAddStarToPlayerMethod& handle) :
crMethod(handle),
m_star(handle.m_star),
m_index(handle.m_index)
{
}
void crJXJAddStarToPlayerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_star = (unsigned char)(atoi(str.c_str()));
		break;
	case 1:
		crArgumentParser::appAtoVec(str, m_index);
		break;
	}
}
void crJXJAddStarToPlayerMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJAddStarToPlayerMethod::operator()(crHandle &handle)
{
	void *param;
	int playerid = 0; 
	FubenStarMap::iterator itr_star;
	crData * roomData = m_this->getDataClass();
	if (roomData)
	{
		roomData->excHandle(MAKEINT64(WCH_LockData, 1));
		roomData->getParam(WCHDATA_JXJFubenStarsMap, param);
		FubenStarMap * starMap = (FubenStarMap *)param;

		m_this->lockPlayerList();
		crRoom::PlayerList &playerList = m_this->getPlayerList();
		for (crRoom::PlayerList::iterator itr = playerList.begin();
			itr != playerList.end();
			++itr)
		{
			playerid = itr->get()->getPlayerID();
			itr_star = starMap->find(playerid);
			if (itr_star != starMap->end())
			{
				itr_star->second += m_star;
			}
			else
			{
				(*starMap)[playerid] = m_star;
			}

			//发消息给客户端
		}
		m_this->unlockPlayerList();

		roomData->excHandle(MAKEINT64(WCH_LockData, 0));

		if (m_index != crVector3b(0, 0, 0))
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeVec3b(m_index);
			stream->_writeUChar(m_star);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet, 0, WCH_JXJRecvFubenAddStar, stream.get());
			m_this->sendPacketToAll(packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvFubenAddStarMethod
//
/////////////////////////////////////////
crJXJRecvFubenAddStarMethod::crJXJRecvFubenAddStarMethod()
{
}

crJXJRecvFubenAddStarMethod::crJXJRecvFubenAddStarMethod(const crJXJRecvFubenAddStarMethod& handle) :
crMethod(handle)
{
}

void crJXJRecvFubenAddStarMethod::inputParam(int i, void *param)
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

void crJXJRecvFubenAddStarMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strSwitch[0] = str;
		break;
	case 2:
		m_strSwitch[1] = str;
		break;
	case 3:
		m_strSwitch[2] = str;
		break;
	case 4:
		m_strSeq[0] = str;
		break;
	case 5:
		m_strSeq[1] = str;
		break;
	case 6:
		m_strSeq[2] = str;
		break;
	default:
		break;
	}
}

void crJXJRecvFubenAddStarMethod::operator()(crHandle &handle)
{
	if (m_netType == GameClient_Game)
	{
		crVector3b index = m_stream->_readVec3b();
		unsigned char starCount = m_stream->_readUChar();


		crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if (canvas)
		{
			ref_ptr<crMultiSwitch>mulitSwitch[3];
			ref_ptr<crSequence>seq[3];

			for (int i = 0; i < 3; i++)
			{
				mulitSwitch[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strSwitch[i]));
				seq[i] = dynamic_cast<crSequence *>(canvas->getChildNode(m_strSeq[i]));
			}

			if (mulitSwitch[0].valid() && seq[0].valid()/* && index[0] == 1*/)
			{
				//if (seq[0]->getMode() == crSequence::STOP && starCount>0)
				if(index[0] == 1)
				{
					mulitSwitch[0]->setActiveSwitchSet(1);
					seq[0]->setVisiable(true);
					seq[0]->startEffect();
				}
				else if(index[0] == -1)// if (seq[0]->getMode() == crSequence::START && starCount <0)
				{
					mulitSwitch[0]->setActiveSwitchSet(0);
					seq[0]->setVisiable(false);
					seq[0]->stopEffect();
				}
			}

			if (mulitSwitch[1].valid() && seq[1].valid()/* && index[1] == 1*/)
			{
				//if (seq[1]->getMode() == crSequence::STOP)
				if(index[1] == 1)
				{
					mulitSwitch[1]->setActiveSwitchSet(1);
					seq[1]->setVisiable(true);
					seq[1]->startEffect();
				}
				else if(index[1] == -1)// if (seq[1]->getMode() == crSequence::START && starCount < 0)
				{
					mulitSwitch[1]->setActiveSwitchSet(0);
					seq[1]->setVisiable(false);
					seq[1]->stopEffect();
				}
			}

			if (mulitSwitch[2].valid() && seq[2].valid()/* && index[2] == 1*/)
			{
				//if (seq[2]->getMode() == crSequence::STOP)
				if(index[2] == 1)
				{
					mulitSwitch[2]->setActiveSwitchSet(1);
					seq[2]->setVisiable(true);
					seq[2]->startEffect();
				}
				else if(index[2] == -1)// if (seq[2]->getMode() == crSequence::START && starCount < 0)
				{
					mulitSwitch[2]->setActiveSwitchSet(0);
					seq[2]->setVisiable(false);
					seq[2]->stopEffect();
				}
			}
		}		
	}
}


/////////////////////////////////////////
//
//crJXJClickHeadCanvasSelectRoleMethod
//
/////////////////////////////////////////
crJXJClickHeadCanvasSelectRoleMethod::crJXJClickHeadCanvasSelectRoleMethod()
{
}

crJXJClickHeadCanvasSelectRoleMethod::crJXJClickHeadCanvasSelectRoleMethod(const crJXJClickHeadCanvasSelectRoleMethod& handle) :
crMethod(handle)
{
}

void crJXJClickHeadCanvasSelectRoleMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if (param)
		{
			m_param = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(m_param));
		}
		else
		{
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	}
}

void crJXJClickHeadCanvasSelectRoleMethod::addParam(int i, const std::string& str)
{

}

void crJXJClickHeadCanvasSelectRoleMethod::operator()(crHandle &handle)
{
	void *param;
	crData * canvasData = m_this->getDataClass();
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if (canvasData && camera.valid())
	{
		canvasData->getParam(WCHDATA_JXJHeadCanvasBindInstance, param);
		crInstanceItem * item = (crInstanceItem *)param;
		if (item != NULL)
		{
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJCameraOperator, param);
			unsigned short attackToPos = *(unsigned short *)param;

			unsigned char targetitemtype = item->getItemtype();
			if (targetitemtype == crInstanceItem::Npc || targetitemtype == crInstanceItem::Role)
			{
				int key = m_ea->getModKeyMask();
				if (!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_SHIFT) && \
					(attackToPos != Cam_AttackToPos) && (attackToPos != Cam_MoveToPos))
				{//shift多选
					//清除之前选中的自己人
					cameraData->getParam(WCHDATA_SelectMyRoleVec, param);
					crNode *fxNode;
					crDecalUpdateCallback *callback;
					//crSelectNodeVisitor selectNode(false);
					SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
					for (SelectMyRoleVec::iterator itr = selectMyRoles->begin();
						itr != selectMyRoles->end();
						++itr)
					{
						//bot = (*itr)->getRelNode();
						//bot->accept(selectNode);
						fxNode = (*itr)->findSceneFxNode("$SelectDecal");
						if (fxNode)
						{
							callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
							if (callback)
							{
								callback->setVisiable(false);
							}
						}
					}
					selectMyRoles->resize(0);
				}
			}

			item->doEvent(WCH_Touch, MAKEINT64(m_ea, NULL));
		}
	}
}

/////////////////////////////////////////
//
//crJXJFubenCallForHelpMethod
//
/////////////////////////////////////////
crJXJFubenCallForHelpMethod::crJXJFubenCallForHelpMethod()
{
}

crJXJFubenCallForHelpMethod::crJXJFubenCallForHelpMethod(const crJXJFubenCallForHelpMethod& handle) :
crMethod(handle),
m_canvasname(handle.m_canvasname),
m_chatmodesw(handle.m_chatmodesw),
m_chatinput(handle.m_chatinput),
m_sendbtn(handle.m_sendbtn)
{
}

void crJXJFubenCallForHelpMethod::inputParam(int i, void *param)
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

void crJXJFubenCallForHelpMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasname = str;
		break;
	case 1:
		m_chatmodesw = str;
		break;
	case 2:
		m_chatinput = str;
		break;
	case 3:
		m_sendbtn = str;
		break;
	case 4:
		m_switch = str;
		break;
	default:
		break;
	}
}

void crJXJFubenCallForHelpMethod::operator()(crHandle &handle)
{
	void *param;
	if (!m_this)return;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (!myPlayer)return;
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
	if (!canvas)return;
	ref_ptr<crRadioGroupWidgetNode> chatmodesw = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_chatmodesw));
	ref_ptr<crStaticTextWidgetNode> chatinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_chatinput));
	ref_ptr<crButtonWidgetNode> sendbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_sendbtn));
	crMultiSwitch *mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
	if (!chatmodesw || !chatinput || !sendbtn || !mulitSwitch)return;
	ref_ptr<crTableIO>chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	chatmodesw->select(ChatInCountry);
	chatmodesw->doEvent(WCH_UI_SelectChange);
	int curSelect = mulitSwitch->getActiveSwitchSet()+1;

	crTableIO::StrVec record;
	std::string name;
	std::string strMessage;
	int textid = 0;
	std::string myplayerName = crMyPlayerData::getInstance()->getCharacterName();

	crData *playerData = myPlayer->getDataClass();
	if (playerData)
	{
		//playerData->getParam(WCHDATA_JXJBattleRoomID, param);
		int roomid = crMyPlayerData::getInstance()->getSelectedRoom()->getRoomID();
		playerData->getParam(WCHDATA_JXJFubenOrChengchiType, param);
		unsigned char type = *(unsigned char *)param;
		
		playerData->getParam(WCHDATA_JXJFubenOrChengchiID, param);
		unsigned short fubenid = *(unsigned short *)param;
		crQuerySceneInfoData *sceneInfoData = crMyPlayerData::getInstance()->getSceneInfoData();
		int sceneid = sceneInfoData->getSceneID();

		//int battleid = LOINT64(battleroomid);
		//unsigned short fubenid = (unsigned short)(HIINT32(battleid));

		crTableIO::StrVec record;
		ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		string fubenName;
		if (fubenTab.valid())
		{
			int index = fubenTab->getTitleIndex("name");
			if (fubenTab->queryOneRecord(0, crArgumentParser::appItoa(fubenid), record) >= 0)
			{
				if (index >= 0)
					fubenName = record[index];
			}
		}

		if (!fubenName.empty())
		{
			strMessage = std::string("^6^") + std::string("玩家[") + myplayerName + std::string("]发起[") + fubenName  + std::string("]救援，望各位将军助其一臂之力。")\
				+ std::string("{") + crArgumentParser::appItoa(roomid) + std::string("|") + crArgumentParser::appItoa(sceneid) + std::string("}");
		}
	}

	if (!strMessage.empty())
	{
		chatinput->setString(strMessage);
		sendbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON, crGUIEventAdapter::RELEASE));
		chatinput->clearString();
	}
}

/////////////////////////////////////////
//
//crJXJCallForHelpLinkClickMethod
//
/////////////////////////////////////////
crJXJCallForHelpLinkClickMethod::crJXJCallForHelpLinkClickMethod() :
m_ea(NULL),
m_this(NULL),
m_roomid(0),
m_sceneId(0)
{
}
crJXJCallForHelpLinkClickMethod::crJXJCallForHelpLinkClickMethod(const crJXJCallForHelpLinkClickMethod& handle) :
crMethod(handle),
m_this(handle.m_this),
m_ea(handle.m_ea),
m_linkInfo(handle.m_linkInfo),
m_roomid(handle.m_roomid),
m_sceneId(handle.m_sceneId)
{
}
void crJXJCallForHelpLinkClickMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
			m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_roomid = *(int*)param;
		break;
	case 4:
		m_sceneId = *(int*)param;
		break;
	}
}
void crJXJCallForHelpLinkClickMethod::addParam(int i, const std::string& str)
{
}
void crJXJCallForHelpLinkClickMethod::operator()(crHandle &handle)
{
	if (m_sceneId != 0 && m_roomid != 0)
	{
		crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		if (playerData)
		{
			_crInt64 battleroomid = MAKEINT64(NULL, m_roomid);
			playerData->inputParam(WCHDATA_JXJBattleRoomID, &battleroomid);

			//crMyPlayerData::getInstance()->setLoginGameSceneMode(1);
			//crGlobalHandle::getInstance()->doEvent(WCH_UINotify, MAKEINT64(1060, NULL));
			crLoginScenePacket packet;
			crLoginScenePacket::buildRequestPacket(packet, m_sceneId, WCH_JXJAgreeEnterBattle);
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			netConductor->getNetManager()->sendPacket("all", packet);
		}
		
		//taskCanceled = false;
	}
}

/************************************************************************/
/*    crJXJSaveServerCycleInfoMethod                                                                  */
/************************************************************************/

JXJ::crJXJSaveServerCycleInfoMethod::crJXJSaveServerCycleInfoMethod() :
m_this(NULL)
{
}

JXJ::crJXJSaveServerCycleInfoMethod::crJXJSaveServerCycleInfoMethod(const crJXJSaveServerCycleInfoMethod & handle) :
crMethod(handle),
m_strInfoFile(handle.m_strInfoFile)
{
}

void JXJ::crJXJSaveServerCycleInfoMethod::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	}
}

void JXJ::crJXJSaveServerCycleInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strInfoFile = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJSaveServerCycleInfoMethod::operator()(crHandle &handle)
{
	void *param;
	CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
	CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
	CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());
	std::string filepath = crArgumentParser::getFilePath(m_strInfoFile);
	std::string filename = crArgumentParser::getSimpleFileName(m_strInfoFile);
	std::string relFileName = filepath + "/" + netCallback->getServerName() + "/" + filename;

	crData *serverBrainData = m_this->getDataClass();
	if (serverBrainData && !relFileName.empty())
	{
		serverBrainData->getParam(WCHDATA_JXJIsServerCycling, param);
		bool isCycle = *(bool *)param;
		serverBrainData->getParam(WCHDATA_JXJServerCycleTimer, param);
		float cycleTimer = *(float *)param;
		
		ref_ptr<CRCore::crStreamBuf> streamBuf = new crStreamBuf;
		streamBuf->createBuf(5);
		streamBuf->_writeBool(isCycle);
		streamBuf->_writeFloat(cycleTimer);
		streamBuf->seekBegin();

		streamBuf->saveToFile2(relFileName);
	}
}

/////////////////////////////////////////
//
//crJXJSkillEffectRangeMethod
//
/////////////////////////////////////////
crJXJSkillEffectRangeMethod::crJXJSkillEffectRangeMethod()
{
}
crJXJSkillEffectRangeMethod::crJXJSkillEffectRangeMethod(const crJXJSkillEffectRangeMethod& handle) :
crMethod(handle)
{

}
void crJXJSkillEffectRangeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_centerPos = crVector3f(0.0f,0.0f,0.0f);
			m_range = 0.0f;
			m_targetType = 0;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_centerPos = *(crVector3f *)(LOINT64(param64));
			crVector2f vec = *(crVector2f *)(HIINT64(param64));
			m_range = vec[0];
			m_targetType = (unsigned char)vec[1];
		}
		else
		{
			m_centerPos = crVector3f(0.0f, 0.0f, 0.0f);
			m_range = 0.0f;
			m_targetType = 0;
		}
		break;
	}
}
void crJXJSkillEffectRangeMethod::addParam(int i, const std::string& str)
{
}
void crJXJSkillEffectRangeMethod::operator()(crHandle &handle)
{
	crRole *role = NULL;
	crData *roleData = NULL;
	char isEnemy = 0;

	if (m_targetType & Target_Self)
	{
		crMyPlayerData::getInstance()->lockMyRoleNpcMap();
		crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for (crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
			itr != myRoles.end();
			++itr)
		{
			role = itr->second.first.get();
			/*bot = dynamic_cast<crViewMatterObject *>(player->getRelNode());*/
			roleData = role->getDataClass();
			if (!roleData) continue;

			if (checkPointInRange(role->getPosition(), m_centerPos, m_range))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Green);
				role->getRelNode()->accept(selectNodeVisitor);
			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				role->getRelNode()->accept(selectNodeVisitor);
			}
		}
		crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
	}
	
	crMyPlayerData::getInstance()->lockInRangePlayerMap();
	crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
	for (crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr)
	{
		role = itr->second.first.get();
		roleData = role->getDataClass();
		if (!roleData) continue;

		if (checkPointInRange(role->getPosition(), m_centerPos, m_range))
		{
			crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck, MAKEINT64(role, &isEnemy));
			if (isEnemy == -1 && (m_targetType & Target_Enemy))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Red);
				role->getRelNode()->accept(selectNodeVisitor);
			}
			else if (isEnemy == 1 && (m_targetType & Target_Friend))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Blue);
				role->getRelNode()->accept(selectNodeVisitor);

			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				role->getRelNode()->accept(selectNodeVisitor);
			}
		}
		else
		{
			crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
			role->getRelNode()->accept(selectNodeVisitor);
		}
	}
	crMyPlayerData::getInstance()->unlockInRangePlayerMap();

	crInstanceItem *item;
	unsigned char npcItemType = 0;
	void *param;
	crMyPlayerData::getInstance()->lockInRangeNpcMap();
	crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
	for (crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
		itr != npcMap.end();
		++itr)
	{
		item = itr->second.first.get();
		roleData = item->getDataClass();
		if (!roleData) continue;

		roleData->getParam(WCHDATA_JXJNpcItemType, param);
		npcItemType = *(unsigned char *)param;
		if (NPC_JianTa == npcItemType || NPC_ChengMen == npcItemType ||\
			NPC_JuDian == npcItemType || NPC_ZhengTing == npcItemType) continue;

		if (checkPointInRange(item->getPosition(), m_centerPos, m_range))
		{
			crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck, MAKEINT64(item, &isEnemy));
			if (isEnemy == -1 && (m_targetType & Target_Enemy))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Red);
				item->getRelNode()->accept(selectNodeVisitor);
			}
			else if (isEnemy == 1 && (m_targetType & Target_Friend))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Blue);
				item->getRelNode()->accept(selectNodeVisitor);
			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				item->getRelNode()->accept(selectNodeVisitor);
			}
		}
		else
		{
			crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
			item->getRelNode()->accept(selectNodeVisitor);
		}
	}
	crMyPlayerData::getInstance()->unlockInRangeNpcMap();
}
bool crJXJSkillEffectRangeMethod::checkPointInRange(const crVector3f &point,const crVector3f &center, float range)
{
	if ((point[0] - center[0])*(point[0] - center[0]) + (point[1] - center[1])*(point[1] - center[1]) < range*range)
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////
//
//crJXJSkillEffectRectangleMethod
//
/////////////////////////////////////////
crJXJSkillEffectRectangleMethod::crJXJSkillEffectRectangleMethod()
{
}
crJXJSkillEffectRectangleMethod::crJXJSkillEffectRectangleMethod(const crJXJSkillEffectRectangleMethod& handle) :
crMethod(handle)
{

}
void crJXJSkillEffectRectangleMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_matrixVolume = NULL;
			m_targetType = 0;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_matrixVolume = (crMatrixVolumeNode *)(LOINT64(param64));
			m_targetType = *(unsigned char *)(HIINT64(param64));
		}
		else
		{
			m_matrixVolume = NULL;
			m_targetType = 0;
		}
		break;
	}
}
void crJXJSkillEffectRectangleMethod::addParam(int i, const std::string& str)
{
}
void crJXJSkillEffectRectangleMethod::operator()(crHandle &handle)
{
	crRole *role = NULL;
	crData *roleData = NULL;
	char isEnemy = 0;

	if (m_targetType & Target_Self)
	{
		crMyPlayerData::getInstance()->lockMyRoleNpcMap();
		crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for (crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
			itr != myRoles.end();
			++itr)
		{
			role = itr->second.first.get();
			/*bot = dynamic_cast<crViewMatterObject *>(player->getRelNode());*/
			roleData = role->getDataClass();
			if (!roleData) continue;

			if (checkPointInRect(role->getPosition(), m_matrixVolume.get()))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Green);
				role->getRelNode()->accept(selectNodeVisitor);
			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				role->getRelNode()->accept(selectNodeVisitor);
			}
		}
		crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
	}

	crMyPlayerData::getInstance()->lockInRangePlayerMap();
	crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
	for (crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr)
	{
		role = itr->second.first.get();
		roleData = role->getDataClass();
		if (!roleData) continue;

		if (checkPointInRect(role->getPosition(), m_matrixVolume.get()))
		{
			crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck, MAKEINT64(role, &isEnemy));
			if (isEnemy == -1 && (m_targetType & Target_Enemy))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Red);
				role->getRelNode()->accept(selectNodeVisitor);
			}
			else if (isEnemy == 1 && (m_targetType & Target_Friend))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Blue);
				role->getRelNode()->accept(selectNodeVisitor);

			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				role->getRelNode()->accept(selectNodeVisitor);
			}
		}
		else
		{
			crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
			role->getRelNode()->accept(selectNodeVisitor);
		}
	}
	crMyPlayerData::getInstance()->unlockInRangePlayerMap();

	crInstanceItem *item;
	unsigned char npcItemType = 0;
	void *param;
	crMyPlayerData::getInstance()->lockInRangeNpcMap();
	crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
	for (crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
		itr != npcMap.end();
		++itr)
	{
		item = itr->second.first.get();
		roleData = item->getDataClass();
		if (!roleData) continue;

		roleData->getParam(WCHDATA_JXJNpcItemType, param);
		npcItemType = *(unsigned char *)param;
		if (NPC_JianTa == npcItemType || NPC_ChengMen == npcItemType || \
			NPC_JuDian == npcItemType || NPC_ZhengTing == npcItemType) continue;

		if (checkPointInRect(item->getPosition(), m_matrixVolume.get()))
		{
			crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck, MAKEINT64(item, &isEnemy));
			if (isEnemy == -1 && (m_targetType & Target_Enemy))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Red);
				item->getRelNode()->accept(selectNodeVisitor);
			}
			else if (isEnemy == 1 && (m_targetType & Target_Friend))
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Blue);
				item->getRelNode()->accept(selectNodeVisitor);
			}
			else
			{
				crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
				item->getRelNode()->accept(selectNodeVisitor);
			}
		}
		else
		{
			crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
			item->getRelNode()->accept(selectNodeVisitor);
		}
	}
	crMyPlayerData::getInstance()->unlockInRangeNpcMap();
}
bool crJXJSkillEffectRectangleMethod::checkPointInRect(const crVector3f &point, crMatrixVolumeNode * matrixVolume)
{
	//crVector2f dirVec(widthlenDir[0] * widthlenDir[1])

	if (matrixVolume->intersects(point))
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////
//
//crJXJCancelSkillEffectMethod
//
/////////////////////////////////////////
crJXJCancelSkillEffectMethod::crJXJCancelSkillEffectMethod()
{
}
crJXJCancelSkillEffectMethod::crJXJCancelSkillEffectMethod(const crJXJCancelSkillEffectMethod& handle) :
crMethod(handle)
{

}
void crJXJCancelSkillEffectMethod::inputParam(int i, void *param)
{
}
void crJXJCancelSkillEffectMethod::addParam(int i, const std::string& str)
{
}
void crJXJCancelSkillEffectMethod::operator()(crHandle &handle)
{
	crRole *role = NULL;
	
	crMyPlayerData::getInstance()->lockMyRoleNpcMap();
	crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	for (crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
		itr != myRoles.end();
		++itr)
	{
		role = itr->second.first.get();
		crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
		role->getRelNode()->accept(selectNodeVisitor);
		
	}
	crMyPlayerData::getInstance()->unlockMyRoleNpcMap();

	crMyPlayerData::getInstance()->lockInRangePlayerMap();
	crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
	for (crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
		itr != playerMap.end();
		++itr)
	{
		role = itr->second.first.get();

		crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
		role->getRelNode()->accept(selectNodeVisitor);

	}
	crMyPlayerData::getInstance()->unlockInRangePlayerMap();

	crInstanceItem *item;
	crMyPlayerData::getInstance()->lockInRangeNpcMap();
	crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
	for (crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
		itr != npcMap.end();
		++itr)
	{
		item = itr->second.first.get();
		crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
		item->getRelNode()->accept(selectNodeVisitor);
	}
	crMyPlayerData::getInstance()->unlockInRangeNpcMap();
}

/////////////////////////////////////////
//
//crJXJNamedSignalUpdateMethod
//
/////////////////////////////////////////
crJXJNamedSignalUpdateMethod::crJXJNamedSignalUpdateMethod()
{
}
crJXJNamedSignalUpdateMethod::crJXJNamedSignalUpdateMethod(const crJXJNamedSignalUpdateMethod& handle) :
crMethod(handle)
{
}
void crJXJNamedSignalUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}
void crJXJNamedSignalUpdateMethod::addParam(int i, const std::string& str)
{
}
void crJXJNamedSignalUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *cameraData = m_this->getDataClass();
	if (cameraData)
	{
		int range = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSignalRange,0).c_str());
		
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJNamedSignalNodeMap, param);
		NamedSignalNodeMap & namedSignalMap = *(NamedSignalNodeMap *)param;

		ref_ptr<crViewMatterObject> bot;
		ref_ptr<crRole> player;
		crVector3 pos;

		NamedSignalNodeMap::iterator itr_signal = namedSignalMap.begin();
		crMyPlayerData::getInstance()->lockMyRoleNpcMap();
		crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for (crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
			itr != myRoles.end();
			++itr)
		{
			player = itr->second.first.get();
			bot = dynamic_cast<crViewMatterObject *>(player->getRelNode());
			pos = bot->getTrans();
			pos[2] = 0.0f;
			itr_signal = namedSignalMap.begin();
			while (itr_signal != namedSignalMap.end())
			{
				if ((pos - itr_signal->second.second).length() < range)
				{
					itr_signal->second.first->setVisiable(false);
					itr_signal = namedSignalMap.erase(itr_signal);
				}
				else
					itr_signal++;
			}
		}
		crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
	}
	
}
/////////////////////////////////////////
//
//crJXJPlayerCycleCountMethod
//
/////////////////////////////////////////
crJXJPlayerCycleCountMethod::crJXJPlayerCycleCountMethod(){}
crJXJPlayerCycleCountMethod::crJXJPlayerCycleCountMethod(const crJXJPlayerCycleCountMethod& handle) :
crMethod(handle)
{
}
void crJXJPlayerCycleCountMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
		break;
	}
}

void crJXJPlayerCycleCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJPlayerCycleCountMethod::operator()(crHandle &handle)
{
	//void * param;
	crData * playerData = m_this->getDataClass();
	int playerid = m_this->getPlayerID();
	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
	if (playerData && netConductor)
	{
		short cycleCount = 0;

		crNetDataManager *netDataManager = netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int gameid = callback->getGameID();

		crDataBase *acountdb = crDataBaseManager::getInstance()->getAccountDB();
		ref_ptr<crDataBase> dbSession = acountdb->beginSession();
		ref_ptr<crPlayerGameStoreCycleCountQueryData> playerStoreQuery = new crPlayerGameStoreCycleCountQueryData;
		playerStoreQuery->buildQuerySql(playerid, gameid);
		dbSession->executeQuery(playerStoreQuery.get());
		crDataBase::QueryResultVec &playerStoreResultVec = dbSession->getQueryResult();

		if (!playerStoreResultVec.empty())
		{
			playerStoreQuery = dynamic_cast<crPlayerGameStoreCycleCountQueryData *>(playerStoreResultVec[0].get());
			if(playerStoreQuery.valid()) cycleCount = playerStoreQuery->getCycleCount();
		}
		dbSession->releaseQuery();
		acountdb->endSession(dbSession.get());

		playerData->inputParam(WCHDATA_JXJPlayerCycleCount, &cycleCount);
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//stream->createBuf(2);
		//stream->_writeShort(*cycleCount);
		//ref_ptr<crGameServerPlayerData> playerGameData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
		//crPlayerDataEventPacket packet;
		//crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJRecvPlayerCycleCount, stream.get());
		//netConductor->getNetManager()->sendPacket(playerGameData->getPlayerConnectServerAddress(), packet);
	}
}

/////////////////////////////////////////
//
//crJXJRecvPlayerCycleCountMethod
//
/////////////////////////////////////////
crJXJRecvPlayerCycleCountMethod::crJXJRecvPlayerCycleCountMethod(){}
crJXJRecvPlayerCycleCountMethod::crJXJRecvPlayerCycleCountMethod(const crJXJRecvPlayerCycleCountMethod& handle) :
crMethod(handle)
{
}
void crJXJRecvPlayerCycleCountMethod::inputParam(int i, void *param)
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

void crJXJRecvPlayerCycleCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvPlayerCycleCountMethod::operator()(crHandle &handle)
{
	if (m_netType == GameClient_Game)
	{
		short cycleCount = m_stream->_readShort();

		crData *playerData = m_this->getDataClass();
		if (playerData)
		{
			playerData->inputParam(WCHDATA_JXJPlayerCycleCount, &cycleCount);
		}
	}
}