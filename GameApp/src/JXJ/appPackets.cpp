/**********************************************************************
*
*	文件:	   appPackets.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#include <JXJ/appPackets.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appNetGameData.h>
#include <CREncapsulation/crTableIO.h>
#include <CRDataBase/crDataBaseManager.h>
#include <JXJ/appDBQuery.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRNetApp/appNetPackets.h>

using namespace CRIOManager;
using namespace CRCore;
using namespace CRNetApp;
using namespace CRNet;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRDataBase;
using namespace JXJ;

/////////////////////////////////////////
//
//crBattleOpenPacket
//
/////////////////////////////////////////
crBattleOpenPacket::crBattleOpenPacket():
	crStreamPacket(crBattleOpenPacket::ID)
{
}
crBattleOpenPacket::crBattleOpenPacket( const crBattleOpenPacket &p ):
	crStreamPacket(crBattleOpenPacket::ID,p)
{
}
void crBattleOpenPacket::buildReplyPacket(crBattleOpenPacket &packet,int playerid,bool openBattle)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeBool(openBattle);
	//streamBuf->_writeBool(openPass);
}
void crBattleOpenPacket::buildReplyPacket2(crBattleOpenPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crBattleOpenPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crBattleOpenPacket packet;
			crBattleOpenPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			crData *data = gameData->getDataClass();
			bool openBattle = m_streamBuf->_readBool();
			//bool openPass = m_streamBuf->_readBool();
			data->inputParam(WCHDATA_JXJOpenBattle,&openBattle);
			//data->inputParam(WCHDATA_JXJOpenPass,&openPass);
			
		}
	}
}


/////////////////////////////////////////
//
//crQueryBattleTimerPacket
//
/////////////////////////////////////////
crQueryBattleTimerPacket::crQueryBattleTimerPacket():
	crStreamPacket(crQueryBattleTimerPacket::ID)
{
}
crQueryBattleTimerPacket::crQueryBattleTimerPacket( const crQueryBattleTimerPacket &p ):
	crStreamPacket(crQueryBattleTimerPacket::ID,p)
{
}
//void crQueryBattleTimerPacket::buildRequestPacket(crQueryBattleTimerPacket &packet/*,unsigned char flg*/)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(1);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeUChar(flg);
//}
void crQueryBattleTimerPacket::buildRequestPacket2(crQueryBattleTimerPacket &packet,int playerid/*,CRCore::crStreamBuf *stream*/)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4/* + stream->getRemainSize()*/);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	//streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crQueryBattleTimerPacket::buildReplyPacket(crQueryBattleTimerPacket &packet,int playerid,/*unsigned char flg,*/int time)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	//streamBuf->_writeUChar(flg);
	streamBuf->_writeInt(time);
}
void crQueryBattleTimerPacket::buildReplyPacket2(crQueryBattleTimerPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crQueryBattleTimerPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crQueryBattleTimerPacket packet;
				crQueryBattleTimerPacket::buildRequestPacket2(packet,playerData->getPlayerID()/*,m_streamBuf.get()*/);
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryBattleTimerPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		//unsigned char flg = m_streamBuf->_readUChar();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			void *param;
			crData *data = crServerBrainHandle::getInstance()->getDataClass();
			int time = 0;
			//if (flg == BT_CityBattle)
			//{
				data->getParam(WCHDATA_JXJBattleTimer,param);
				time = *(int *)param;
			//}
			//else
			//{
			//	data->getParam(WCHDATA_JXJPassTimer,param);
			//	time = *(short *)param;
			//}
			crQueryBattleTimerPacket packet;
			crQueryBattleTimerPacket::buildReplyPacket(packet,playerid,/*flg,*/time);
			m_netConductor->getNetManager()->sendPacket(netPlayerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crQueryBattleTimerPacket packet;
			crQueryBattleTimerPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			crData *data = gameData->getDataClass();
			//unsigned char flg = m_streamBuf->_readUChar();
			int battletimer = m_streamBuf->_readInt();
			//if (flg == BT_CityBattle)
			//{
				data->inputParam(WCHDATA_JXJBattleTimer,&battletimer);
			//}
			//else
			//{
			//	data->inputParam(WCHDATA_JXJPassTimer,&battletimer);
			//}
		}
	}
}


/////////////////////////////////////////
//
//crJXJPlayerLoginGamePacket
//
/////////////////////////////////////////
crJXJPlayerLoginGamePacket::crJXJPlayerLoginGamePacket():
	crStreamPacket(crJXJPlayerLoginGamePacket::ID)
{
}
crJXJPlayerLoginGamePacket::crJXJPlayerLoginGamePacket( const crJXJPlayerLoginGamePacket &p ):
	crStreamPacket(crJXJPlayerLoginGamePacket::ID,p)
{
}
void crJXJPlayerLoginGamePacket::buildReplyPacket(crJXJPlayerLoginGamePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJPlayerLoginGamePacket::buildReplyPacket2(crJXJPlayerLoginGamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJPlayerLoginGamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJPlayerLoginGamePacket packet;
			crJXJPlayerLoginGamePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			void *param;
			crTableIO::StrVec record;
			crData *data = gameData->getDataClass();
			if(data)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				crGlobalHandle::getInstance()->timer() = m_streamBuf->_readDouble();
				//char gbuf[256];
				//sprintf(gbuf,"crJXJPlayerLoginGamePacket servertime=%f\n\0",crGlobalHandle::getInstance()->timer());
				//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				bool openBattle = m_streamBuf->_readBool();
				data->inputParam(WCHDATA_JXJOpenBattle,&openBattle);
				//bool is_pass_opened = m_streamBuf->_readBool();
				//data->inputParam(WCHDATA_JXJOpenPass, &is_pass_opened);
				//if(openBattle)
				//{
				//	short battletimer = m_streamBuf->_readShort();
				//	data->inputParam(WCHDATA_JXJBattleTimer,&battletimer);
				//	//data->inputParam(WCHDATA_JXJOpenPassTimer,&battletimer);
				//}
				int size = (int)(m_streamBuf->_readShort());
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *chengchimap = (ChengchiMap *)param;
				chengchimap->clear();
				unsigned short id;
				//unsigned char shili;
				//unsigned short defend;
				ref_ptr<crJXJChengfangInfo> chengfangInfo;
				for (int i = 0; i < size; i++)
				{
					id = m_streamBuf->_readUShort();
					chengfangInfo = new crJXJChengfangInfo;
					chengfangInfo->setStream(m_streamBuf.get());
					chengchimap->insert(std::make_pair(id,chengfangInfo));
					//shili = m_streamBuf->_readUChar();
					//defend = m_streamBuf->_readUShort();
					//(*chengchimap)[id] = std::make_pair(shili,defend);
				}

// 				data->getParam(WCHDATA_JXJRebelCityVec,param);
// 				RebelCityVec *rebelCity = (RebelCityVec *)param;
// 				rebelCity->clear();
// 				int rebelSize = m_streamBuf->_readChar();
// 				for (int i = 0;i<rebelSize;++i)
// 				{
// 					rebelCity->push_back(m_streamBuf->_readUShort());
// 				}

				//unsigned short perTimer = m_streamBuf->_readUShort();

				//轮回信息
				bool isCycling = m_streamBuf->_readBool();
				float cycleTimer = m_streamBuf->_readFloat();
				//开服时间
				crGlobalHandle::gData()->gStartDate() = m_streamBuf->_readString();
				//////////////////////////////////////////////////////////////////////////
				if (m_streamBuf->getRemainSize() > 0)
				{
					ref_ptr<crStreamBuf> tempBufer = m_streamBuf.get();
					data->inputParam(WCHDATA_JXJCenterNotice, tempBufer.get());
					data->inputParam(WCHDATA_JXJOldCenterNotice, tempBufer.get());
				}

				//data->inputParam(WCHDATA_JXJRebelTimer,&perTimer);	

				data->inputParam(WCHDATA_JXJClientIsServerCycling,&isCycling);
				data->inputParam(WCHDATA_JXJClientCycleTimer,&cycleTimer);

				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJSearchBattlePacket
//
/////////////////////////////////////////
crJXJSearchBattlePacket::crJXJSearchBattlePacket():
	crStreamPacket(crJXJSearchBattlePacket::ID)
{
}
crJXJSearchBattlePacket::crJXJSearchBattlePacket( const crJXJSearchBattlePacket &p ):
	crStreamPacket(crJXJSearchBattlePacket::ID,p)
{
}
void crJXJSearchBattlePacket::buildRequestPacket(crJXJSearchBattlePacket &packet,int roomid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(roomid);
}
void crJXJSearchBattlePacket::buildRequestPacket2(crJXJSearchBattlePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJSearchBattlePacket::buildReplyPacket(crJXJSearchBattlePacket &packet,int playerid,CRCore::crStreamBuf *roomdata)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	packet.setStreamBuf(streamBuf);
	if(roomdata)
	{
		streamBuf->createBuf(5+roomdata->getBufSize());
		streamBuf->_writeInt(playerid);
		streamBuf->_writeBool(true);
		streamBuf->write(roomdata->getBuf(),roomdata->getBufSize());
	}
	else
	{
		streamBuf->createBuf(5);
		streamBuf->_writeInt(playerid);
		streamBuf->_writeBool(false);
	}
}
void crJXJSearchBattlePacket::buildReplyPacket2(crJXJSearchBattlePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJSearchBattlePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crJXJSearchBattlePacket packet;
				crJXJSearchBattlePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crJXJSearchBattlePacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		int roomid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid() && roomid!=0)
		{
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			crRoom *room = callback->findRoom(roomid);
			ref_ptr<crStreamBuf> roomData;
			if(room && room->isCanJoin())
			{
				room->getRoomData(roomData);
			}
			crJXJSearchBattlePacket packet;
			crJXJSearchBattlePacket::buildReplyPacket(packet,playerid,roomData.get());
			m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJSearchBattlePacket packet;
			crJXJSearchBattlePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		bool hasRoom = m_streamBuf->_readBool();
		if(hasRoom)
		{
			ref_ptr<crRoom> room = new crRoom(NULL);
			crMyPlayerData::getInstance()->lockRoomMap();
			RoomMap &roomMap = crMyPlayerData::getInstance()->getRoomMap();
			roomMap.clear();
			room->setRoomData(m_streamBuf.get());
			roomMap[room->getRoomID()] = room.get();
			crMyPlayerData::getInstance()->unlockRoomMap();
			crMyPlayerData::getInstance()->setSelectedRoom(room.get());
		}
		m_streamBuf->seekBegin();
		crBrain::getInstance()->doEvent(WCH_JXJRecvSearchBattle,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crJXJSearchSceneRoomPacket
//
/////////////////////////////////////////
crJXJSearchSceneRoomPacket::crJXJSearchSceneRoomPacket():
	crStreamPacket(crJXJSearchSceneRoomPacket::ID)
{
}
crJXJSearchSceneRoomPacket::crJXJSearchSceneRoomPacket( const crJXJSearchSceneRoomPacket &p ):
	crStreamPacket(crJXJSearchSceneRoomPacket::ID,p)
{
}
void crJXJSearchSceneRoomPacket::buildRequestPacket(crJXJSearchSceneRoomPacket &packet,int sceneid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(sceneid);
}
void crJXJSearchSceneRoomPacket::buildRequestPacket2(crJXJSearchSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJSearchSceneRoomPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crJXJSearchSceneRoomPacket packet;
				crJXJSearchSceneRoomPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crJXJSearchBattlePacket "<<std::endl;
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		int sceneid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid())
		{
			CRNetApp::crScene *scene = callback->findScene(sceneid);
			if(scene && scene->getMainRoomID()!=0)
			{
				crRoom *room = callback->findRoom(scene->getMainRoomID());
				ref_ptr<crStreamBuf> roomData;
				if(room && room->isCanJoin())
				{
					room->getRoomData(roomData);
				}
				crJXJSearchBattlePacket packet;
				crJXJSearchBattlePacket::buildReplyPacket(packet,playerid,roomData.get());
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRandomPlayerNamePacket
//
/////////////////////////////////////////
crJXJRandomPlayerNamePacket::crJXJRandomPlayerNamePacket():
	crStreamPacket(crJXJRandomPlayerNamePacket::ID)
{
}
crJXJRandomPlayerNamePacket::crJXJRandomPlayerNamePacket( const crJXJRandomPlayerNamePacket &p ):
	crStreamPacket(crJXJRandomPlayerNamePacket::ID,p)
{
}
void crJXJRandomPlayerNamePacket::buildRequestPacket(crJXJRandomPlayerNamePacket &packet,unsigned char sex)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(sex);
}
void crJXJRandomPlayerNamePacket::buildRequestPacket2(crJXJRandomPlayerNamePacket &packet,int playerid,unsigned char sex)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeUChar(sex);
}
void crJXJRandomPlayerNamePacket::buildReplyPacket(crJXJRandomPlayerNamePacket &packet,int playerid,const std::string &name)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+name.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeString(name);
}
void crJXJRandomPlayerNamePacket::buildReplyPacket2(crJXJRandomPlayerNamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJRandomPlayerNamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		unsigned char sex = m_streamBuf->_readUChar();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crJXJRandomPlayerNamePacket packet;
				crJXJRandomPlayerNamePacket::buildRequestPacket2(packet,playerData->getPlayerID(),sex);
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryBattleTimerPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		unsigned char sex = m_streamBuf->_readUChar();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
			ref_ptr<crTableIO>playerNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerNameTab);
			int tableLines = playerNameTab->getRowCount();
			CRCore::rangei r(0,tableLines);
			//获取名字
			bool success = false;
			std::string playName;
			crVector2i rndi;
			std::set<crVector2i> RndISet;
			int malindex = playerNameTab->getTitleIndex("男");
			int femaleindex = playerNameTab->getTitleIndex("女");
			int sexindex = 0;
			if (sex == 0)
			{
				sexindex = malindex;
			}
			else
			{
				sexindex = femaleindex;
			}
			ref_ptr<crTableIO>InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidWordTab);
			crTableIO::StrVec record;
			for( int i = 0; i<100; i++)
			{
				rndi[0] = r.get_random();  //姓 行号
				rndi[1] = r.get_random();  //名 行号
				if(RndISet.find(rndi) == RndISet.end())
				{
					success = true;
					playName = playerNameTab->getData(rndi[0],0);
					playName += playerNameTab->getData(rndi[1],sexindex);
					if (InvalidNameTab.valid())
					{
						for (int i = 0; i < InvalidNameTab->getRowCount();i++)
						{
							std::string &invalidstring = InvalidNameTab->getData(i,0);
							if(invalidstring.size() > 0 && playName.find(invalidstring) != std::string::npos)
							{
								success = false;
								break;
							}
						}
					}
					if(success)
						success = callback->insertCharacterName(playerid,playName);
					if(!success)
						RndISet.insert(rndi);
					else
					{//找到name
						break;
					}
				}
			}
			crJXJRandomPlayerNamePacket packet;
			crJXJRandomPlayerNamePacket::buildReplyPacket(packet,playerid,playName);
			m_netConductor->getNetManager()->sendPacket(netPlayerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJRandomPlayerNamePacket packet;
			crJXJRandomPlayerNamePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		//crGlobalHandle::getInstance()->doEvent(WCH_NetReturnStream,MAKEINT64(m_streamBuf.get(),NULL));
		crBrain::getInstance()->doEvent(WCH_JXJReturnRandomName,MAKEINT64(m_streamBuf.get(),NULL));
	}
}

/////////////////////////////////////////
//
//crJXJGameServerCreateRoomPacket
//
/////////////////////////////////////////
crJXJGameServerCreateRoomPacket::crJXJGameServerCreateRoomPacket():
	crStreamPacket(crJXJGameServerCreateRoomPacket::ID)
{
}
crJXJGameServerCreateRoomPacket::crJXJGameServerCreateRoomPacket( const crJXJGameServerCreateRoomPacket &p ):
	crStreamPacket(crJXJGameServerCreateRoomPacket::ID,p)
{
}
void crJXJGameServerCreateRoomPacket::buildRequestPacket(crJXJGameServerCreateRoomPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJGameServerCreateRoomPacket::buildReplyPacket(crJXJGameServerCreateRoomPacket &packet,bool sucess,int roomid,int sceneid,int battleid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(13);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(sucess);
	streamBuf->_writeInt(roomid);
	streamBuf->_writeInt(sceneid);
	streamBuf->_writeInt(battleid);
}
void crJXJGameServerCreateRoomPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int sceneid = m_streamBuf->_readInt();
		int battleid = m_streamBuf->_readInt();
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServer->getNetDataManager();
		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
		if(callback)
		{
			crJXJGameServerCreateRoomPacket packet;
			crRoom *room = callback->createRoom(sceneid,m_streamBuf.get());
			if(room)
			{
				crData *data = room->getDataClass();
				data->inputParam(WCHDATA_JXJBattleID,&battleid);
				crJXJGameServerCreateRoomPacket::buildReplyPacket(packet,true,room->getRoomID(),sceneid,battleid);
			}
			else
			{
				crJXJGameServerCreateRoomPacket::buildReplyPacket(packet,false,0,sceneid,battleid);
			}
			m_netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_GameServerCreateRoomReturn,MAKEINT64(m_streamBuf.get(),NULL));
	}
}


/////////////////////////////////////////
//
//crJXJJuntuanCreateRoomPacket
//
/////////////////////////////////////////
crJXJJuntuanCreateRoomPacket::crJXJJuntuanCreateRoomPacket():
	crStreamPacket(crJXJJuntuanCreateRoomPacket::ID)
{
}
crJXJJuntuanCreateRoomPacket::crJXJJuntuanCreateRoomPacket( const crJXJJuntuanCreateRoomPacket &p ):
	crStreamPacket(crJXJJuntuanCreateRoomPacket::ID,p)
{
}
void crJXJJuntuanCreateRoomPacket::buildRequestPacket(crJXJJuntuanCreateRoomPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJJuntuanCreateRoomPacket::buildReplyPacket(crJXJJuntuanCreateRoomPacket &packet,bool sucess,int roomid,int sceneid,int battleid,int groupid,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(17);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(sucess);
	streamBuf->_writeInt(roomid);
	streamBuf->_writeInt(sceneid);
	streamBuf->_writeInt(battleid);
	streamBuf->_writeInt(groupid);
	streamBuf->_writeInt(playerid);
}
void crJXJJuntuanCreateRoomPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		int groupid = m_streamBuf->_readInt();
		int sceneid = m_streamBuf->_readInt();
		int battleid = m_streamBuf->_readInt();
		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServer->getNetDataManager();
		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
		if(callback)
		{
			crJXJJuntuanCreateRoomPacket packet;
			crRoom *room = callback->createRoom(sceneid,m_streamBuf.get());
			if(room)
			{
				crData *data = room->getDataClass();
				data->inputParam(WCHDATA_JXJBattleID,&battleid);
				crJXJJuntuanCreateRoomPacket::buildReplyPacket(packet,true,room->getRoomID(),sceneid,battleid,groupid,playerid);
			}
			else
			{
				crJXJJuntuanCreateRoomPacket::buildReplyPacket(packet,false,0,sceneid,battleid,groupid,playerid);
			}
			m_netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
	else if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJJuntuanCreateRoom,MAKEINT64(m_streamBuf.get(),NULL));
	}
}



/////////////////////////////////////////
//
//crJXJBattleRemovedPacket
//
/////////////////////////////////////////
crJXJBattleRemovedPacket::crJXJBattleRemovedPacket():
	crStreamPacket(crJXJBattleRemovedPacket::ID)
{
}
crJXJBattleRemovedPacket::crJXJBattleRemovedPacket( const crJXJBattleRemovedPacket &p ):
	crStreamPacket(crJXJBattleRemovedPacket::ID,p)
{
}
void crJXJBattleRemovedPacket::buildRequestPacket(crJXJBattleRemovedPacket &packet,int battleid,int roomid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(battleid);
	streamBuf->_writeInt(roomid);
}
void crJXJBattleRemovedPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvBattleRemoved,MAKEINT64(m_streamBuf.get(),NULL));
	}
}


/////////////////////////////////////////
//
//crJXJBattleSettlePacket
//
/////////////////////////////////////////
crJXJBattleSettlePacket::crJXJBattleSettlePacket():
	crStreamPacket(crJXJBattleSettlePacket::ID)
{
}
crJXJBattleSettlePacket::crJXJBattleSettlePacket( const crJXJBattleSettlePacket &p ):
	crStreamPacket(crJXJBattleSettlePacket::ID,p)
{
}
void crJXJBattleSettlePacket::buildRequestPacket(crJXJBattleSettlePacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleSettlePacket::buildReplyPacket2(crJXJBattleSettlePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJBattleSettlePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvBattleSettle,MAKEINT64(m_streamBuf.get(),NULL));
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crJXJBattleSettlePacket packet;
				crJXJBattleSettlePacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			room->doEvent(WCH_JXJRecvBattleSettle,MAKEINT64(m_streamBuf.get(),NULL));
		}
	}
}

/////////////////////////////////////////
//
//crJXJCountryCapitalAwardPacket
//
/////////////////////////////////////////
crJXJCountryCapitalAwardPacket::crJXJCountryCapitalAwardPacket():
	crStreamPacket(crJXJCountryCapitalAwardPacket::ID)
{
}
crJXJCountryCapitalAwardPacket::crJXJCountryCapitalAwardPacket( const crJXJCountryCapitalAwardPacket &p ):
	crStreamPacket(crJXJCountryCapitalAwardPacket::ID,p)
{
}
void crJXJCountryCapitalAwardPacket::buildRequestPacket(crJXJCountryCapitalAwardPacket &packet,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(stream) streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJCountryCapitalAwardPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		_crInt64 msg = m_streamBuf->_readInt64();
		crServerBrainHandle::getInstance()->doEvent(msg,MAKEINT64(m_streamBuf.get(),NULL));
		//crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvBattleSettle,MAKEINT64(m_streamBuf.get(),NULL));
	}
}

/////////////////////////////////////////
//
//crJXJBattleReportPacket
//
/////////////////////////////////////////
crJXJBattleReportPacket::crJXJBattleReportPacket() :
crStreamPacket(crJXJBattleReportPacket::ID)
{
}
crJXJBattleReportPacket::crJXJBattleReportPacket(const crJXJBattleReportPacket &p) :
crStreamPacket(crJXJBattleReportPacket::ID, p)
{
}
void crJXJBattleReportPacket::buildRequestPacket(crJXJBattleReportPacket &packet, CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleReportPacket::buildReplyPacket2(crJXJBattleReportPacket &packet, CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(), stream->getRemainSize());
}
void crJXJBattleReportPacket::parsePacket(const std::string &sender)
{
	if (!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if (netType == GameServer)
	{
		void *param;
		crServerBrainHandle * serverBrain = crServerBrainHandle::getInstance();
		crData *serverData = serverBrain->getDataClass();
		if (serverData)
		{
			ref_ptr<crBattleReportInfo> battleReportInfo = new crBattleReportInfo;
			//battleReportInfo->setTextInfo();
			//battleReportInfo->setMailTitle();

			int battleid = m_streamBuf->_readInt();
			battleReportInfo->setBattleID(battleid);
			std::string roomName = m_streamBuf->_readString();//4+
			bool isAttackWin = m_streamBuf->_readBool();//1
			unsigned short size = m_streamBuf->_readUShort();//2*

			ref_ptr<crStreamBuf> battleData = new crStreamBuf;
			battleData->createBuf(roomName.size() + 23 + size*40);

			battleData->_writeString(roomName);//4
			battleData->_writeBool(isAttackWin);//1
			battleData->_writeUShort(size);//2

			unsigned char attackshili = (unsigned char)(LOINT16(LOINT32(battleid)));
			ref_ptr<crTableIO> battleRewardTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBattleRewardTab);
			int exploitindex = battleRewardTable->getTitleIndex("功勋");
			crTableIO::StrVec record_reward_base,record_reward_extra;
			int winnermingci = 0;
			int losermingci = 100;
			float playerScore = 0.0f;
			std::multimap<float,std::vector<std::string>,std::greater<float>> winermap;       
			std::multimap<float,std::vector<std::string>,std::greater<float>> losermap;

			for (int i = 0; i < size; i++)
			{
				std::vector<std::string> info;

				//battleReportInfo->getPlayerVec().push_back(m_streamBuf->_readInt());

				//battleData->_writeInt(m_streamBuf->_readInt());//4 killcount
				//battleData->_writeInt(m_streamBuf->_readInt());//4 deadCount

				//unsigned char myshili = m_streamBuf->_readUChar();
			
				//battleData->_writeFloat(m_streamBuf->_readFloat());//4 killscore
				//battleData->_writeFloat(m_streamBuf->_readFloat());//4 experience
				//battleData->_writeString(m_streamBuf->_readString());//20+4

				info.push_back(crArgumentParser::appItoa(m_streamBuf->_readInt()));
				info.push_back(crArgumentParser::appItoa(m_streamBuf->_readInt()));
				info.push_back(crArgumentParser::appItoa(m_streamBuf->_readInt()));
				unsigned char myshili = m_streamBuf->_readUChar();
				float score = m_streamBuf->_readFloat();
				info.push_back(crArgumentParser::appItoa((int)(m_streamBuf->_readFloat())));
				info.push_back(m_streamBuf->_readString());

				info.push_back(crArgumentParser::appItoa(m_streamBuf->_readInt()));

				if(myshili == attackshili)
				{
					if(isAttackWin)
						winermap.insert(std::make_pair(score,info));
					else
						losermap.insert(std::make_pair(score,info));	
				}
				else
				{
					if(isAttackWin)
						losermap.insert(std::make_pair(score,info));
						
					else 
						winermap.insert(std::make_pair(score,info));
				}

			}

			int index = 1;
			int my_index = -1;
			for (std::multimap<float,std::vector<std::string>,std::greater<float>>::iterator itr=winermap.begin();itr!=winermap.end();itr++)
			{
				battleReportInfo->getPlayerVec().push_back(atoi((itr->second)[0].c_str()));

				battleData->_writeInt(atoi((itr->second)[1].c_str()));//4 killcount
				battleData->_writeInt(atoi((itr->second)[2].c_str()));//4 deadCount

				my_index = index;
				if(index>20) my_index = 21;
				if(battleRewardTable->queryOneRecord(0,crArgumentParser::appItoa(0),record_reward_base)>=0 && battleRewardTable->queryOneRecord(0,crArgumentParser::appItoa(my_index),record_reward_extra)>=0)
					battleData->_writeFloat(float(atoi(record_reward_base[exploitindex].c_str()))+float(atoi(record_reward_extra[exploitindex].c_str()))+ itr->first);

				battleData->_writeFloat(float(atoi((itr->second)[3].c_str())));//4 experience
				battleData->_writeString((itr->second)[4]);//20+4
				battleData->_writeInt(atoi((itr->second)[5].c_str()));//部队数

				index++;
			}

			index = 1;
			for (std::multimap<float,std::vector<std::string>,std::greater<float>>::iterator itr=losermap.begin();itr!=losermap.end();itr++)
			{
				battleReportInfo->getPlayerVec().push_back(atoi((itr->second)[0].c_str()));

				battleData->_writeInt(atoi((itr->second)[1].c_str()));//4 killcount
				battleData->_writeInt(atoi((itr->second)[2].c_str()));//4 deadCount

				my_index = index+100;
				if(index>20) my_index = 121;
				if(battleRewardTable->queryOneRecord(0,crArgumentParser::appItoa(100),record_reward_base)>=0 && battleRewardTable->queryOneRecord(0,crArgumentParser::appItoa(my_index),record_reward_extra)>=0)
					battleData->_writeFloat(float(atoi(record_reward_base[exploitindex].c_str()))+float(atoi(record_reward_extra[exploitindex].c_str()))+ itr->first);

				battleData->_writeFloat(float(atoi((itr->second)[3].c_str())));//4 experience
				battleData->_writeString((itr->second)[4]);//20+4
				battleData->_writeInt(atoi((itr->second)[5].c_str()));//部队数

				index++;
			}

			int ashiliRtCount = m_streamBuf->_readInt();
			int ashiliDeadCount = m_streamBuf->_readInt();
			int dshiliRtCount = m_streamBuf->_readInt();
			int dshiliDeadCount = m_streamBuf->_readInt();

			battleData->_writeInt(ashiliRtCount);//4
			battleData->_writeInt(ashiliDeadCount);//4
			battleData->_writeInt(dshiliRtCount);//4
			battleData->_writeInt(dshiliDeadCount);//4
			//压缩数据
			battleData->seekBegin();
			CRNet::creCompress(battleData);
			battleReportInfo->setBattleData(battleData);

			//生成文本信息
			crTableIO::StrVec record,record2;
			std::string aShiliName,dShiliName,winShiliName;
			unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));
			unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
			ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			if (playershilitab.valid() && \
				playershilitab->queryOneRecord(0, crArgumentParser::appItoa(ashili), record) >= 0 && \
				playershilitab->queryOneRecord(0, crArgumentParser::appItoa(dshili), record2) >= 0)
			{
				int shilinameindex = playershilitab->getTitleIndex("name");
				if (shilinameindex >= 0)
				{
					aShiliName = record[shilinameindex];
					dShiliName = record2[shilinameindex];
					winShiliName = isAttackWin ? aShiliName : dShiliName;
				}
			}

			if (!aShiliName.empty() && !dShiliName.empty() && !roomName.empty())
			{
				char text[128];
				sprintf_s(text,128, "[%s]国与[%s]在[%s]中进行了激烈战斗，最终[%s]获胜！\0", aShiliName.c_str(), \
					dShiliName.c_str(), roomName.c_str(), winShiliName.c_str());
				battleReportInfo->setTextInfo(text);
				if (isAttackWin)
				{
					sprintf_s(text, 128 ,"[%s][%s][%s]\0", aShiliName.c_str(), dShiliName.c_str(), roomName.c_str());
				}
				else
				{
					sprintf_s(text,128 ,"[%s][%s][%s]\0", dShiliName.c_str(), aShiliName.c_str(), roomName.c_str());
				}
				battleReportInfo->setMailTitle(text);
			}

			//插入到数据操作队列
			serverData->excHandle(MAKEINT64(WCH_LockData, 1));
			serverData->getParam(WCHDATA_JXJInsertBattleReportDeq, param);
			InsertBattleReportDeq *disposeDeq = (InsertBattleReportDeq *)param;
			disposeDeq->push_back(battleReportInfo);
			serverData->excHandle(MAKEINT64(WCH_LockData, 0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJFubenSettlePacket
//
/////////////////////////////////////////
crJXJFubenSettlePacket::crJXJFubenSettlePacket():
	crStreamPacket(crJXJFubenSettlePacket::ID)
{
}
crJXJFubenSettlePacket::crJXJFubenSettlePacket( const crJXJFubenSettlePacket &p ):
	crStreamPacket(crJXJFubenSettlePacket::ID,p)
{
}
void crJXJFubenSettlePacket::buildRequestPacket(crJXJFubenSettlePacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJFubenSettlePacket::buildReplyPacket2(crJXJFubenSettlePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJFubenSettlePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvFubenSettle,MAKEINT64(m_streamBuf.get(),NULL));
	}
	//else if(netType == GameConnectServerClient_Scene)
	//{
	//	int recvid = m_streamBuf->_readInt();
	//	crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
	//	if(connectServerConductor)
	//	{
	//		crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
	//		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
	//		if(playerData.valid())
	//		{
	//			crJXJFubenSettlePacket packet;
	//			crJXJFubenSettlePacket::buildReplyPacket2(packet,m_streamBuf.get());
	//			connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
	//			//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
	//		}	
	//	}
	//}
	//else if(netType == GameClient_Game)
	//{//
	//	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	//	if(room)
	//	{
			//room->doEvent(WCH_JXJRecvFubenSettle,MAKEINT64(m_streamBuf.get(),NULL));
	//	}
	//}
}

///////////////////////////////////////////
////
////crJXJPassSettlePacket
////
///////////////////////////////////////////
//crJXJPassSettlePacket::crJXJPassSettlePacket():
//	crStreamPacket(crJXJPassSettlePacket::ID)
//{
//}
//crJXJPassSettlePacket::crJXJPassSettlePacket( const crJXJPassSettlePacket &p ):
//	crStreamPacket(crJXJPassSettlePacket::ID,p)
//{
//}
//void crJXJPassSettlePacket::buildRequestPacket(crJXJPassSettlePacket &packet,CRCore::crStreamBuf *stream)
//{
//	packet.setStreamBuf(stream);
//}
//void crJXJPassSettlePacket::buildReplyPacket2(crJXJPassSettlePacket &packet,CRCore::crStreamBuf *stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crJXJPassSettlePacket::parsePacket(const std::string &sender)
//{
//	if(!m_netConductor) return;
//
//	int netType = m_netConductor->getNetType();
//	if(netType == GameServer)
//	{
//		crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvPassSettle,MAKEINT64(m_streamBuf.get(),NULL));
//	}
//	else if(netType == GameConnectServerClient_Scene)
//	{
//		int recvid = m_streamBuf->_readInt();
//		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServerConductor)
//		{
//			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
//			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
//			if(playerData.valid())
//			{
//				crJXJPassSettlePacket packet;
//				crJXJPassSettlePacket::buildReplyPacket2(packet,m_streamBuf.get());
//				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
//				//CRCore::notify(CRCore::ALWAYS)<<"crStartRoomGamePacket"<<std::endl;
//			}	
//		}
//	}
//	else if(netType == GameClient_Game)
//	{//
//		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
//		if(room)
//		{
//			room->doEvent(WCH_JXJRecvPassSettle,MAKEINT64(m_streamBuf.get(),NULL));
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJBattleTotalArmyCountPacket
//
/////////////////////////////////////////
crJXJBattleTotalArmyCountPacket::crJXJBattleTotalArmyCountPacket():
	crStreamPacket(crJXJBattleTotalArmyCountPacket::ID)
{
}
crJXJBattleTotalArmyCountPacket::crJXJBattleTotalArmyCountPacket( const crJXJBattleTotalArmyCountPacket &p ):
	crStreamPacket(crJXJBattleTotalArmyCountPacket::ID,p)
{
}
void crJXJBattleTotalArmyCountPacket::buildRequestPacket(crJXJBattleTotalArmyCountPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleTotalArmyCountPacket::buildReplyPacket2(crJXJBattleTotalArmyCountPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJBattleTotalArmyCountPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crJXJBattleTotalArmyCountPacket packet;
				crJXJBattleTotalArmyCountPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			room->doEvent(WCH_JXJRecvBattleArmyCount,MAKEINT64(m_streamBuf.get(),NULL));
		}
	}
}
/////////////////////////////////////////
//
//crJXJdFormationInfoPacket
//
/////////////////////////////////////////
crJXJBattleAddArmyInfoPacket::crJXJBattleAddArmyInfoPacket():
	crStreamPacket(crJXJBattleAddArmyInfoPacket::ID)
{
}
crJXJBattleAddArmyInfoPacket::crJXJBattleAddArmyInfoPacket( const crJXJBattleAddArmyInfoPacket &p ):
	crStreamPacket(crJXJBattleAddArmyInfoPacket::ID,p)
{
}
void crJXJBattleAddArmyInfoPacket::buildRequestPacket(crJXJBattleAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleAddArmyInfoPacket::buildReplyPacket(crJXJBattleAddArmyInfoPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crJXJBattleAddArmyInfoPacket::buildReplyPacket2(crJXJBattleAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJBattleAddArmyInfoPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *serverConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = serverConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
		{
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			if(roleMap.empty())
				return;
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			CRNetApp::crScene *scene = callback->findScene(playerData->getSceneID());
			crRoom *room = callback->findRoom(playerData->getRoomID());
			if(scene && room)
			{
				crRoomPlayer *roomplayer = room->getMember(playerid);
				if(roomplayer)
				{
					void *param;
					crData *data = playerData->getPlayerGameData()->getDataClass();
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJFormationInfoVec,param);
					FormationInfoVec *formationVec = (FormationInfoVec *)param;
					int roleid;
					unsigned short count;
					char size = m_streamBuf->_readChar();
					for(char i =0;i<size;i++)
					{
						roleid = m_streamBuf->_readInt();
						count = m_streamBuf->_readUShort();
						for( FormationInfoVec::iterator itr = formationVec->begin();
							itr != formationVec->end();
							++itr )
						{
							if((*itr)->getRoleID() == roleid)
							{
								(*itr)->setCount(count);
							}
						}
					}
					data->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myShili = *(unsigned char *)param;
					data->excHandle(MAKEINT64(WCH_LockData,0));
					unsigned char inspireLv = m_streamBuf->_readUChar();
					
					crData *roleData;
					float scale = crGlobalHandle::gData()->gUnitScale();
					crRole *role = roleMap.begin()->second.get();
					crVector2f deadPos(role->getPosx(),role->getPosy());
					deadPos *= scale;
					crSceneLayer *sceneLayer = scene->getSceneLayer(roleMap.begin()->second.get()->getLayerID());
					const crSceneLayer::BirthPointArray& pointVec = sceneLayer->getBirthPointArray();
					crData *roomData = room->getDataClass();
					roomData->getParam(WCHDATA_JXJBattleID,param);
					_crInt32 battleid = *(_crInt32 *)param;
					unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));
					unsigned short chengchiid = HIINT32(battleid);
					ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
					crTableIO::StrVec record;
					int chusindex = chengchiTab->getTitleIndex("出生点数");
					if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0)
					{
						crVector2i bpointvec;
						crArgumentParser::appAtoVec(record[chusindex],bpointvec);
						std::set<crVector3f> BirthPointSet;

						if(!pointVec.empty())
						{
							if(myShili == attackShili)
							{//攻击方
								for( int i = 0; i<bpointvec[0]; i++)
								{
									BirthPointSet.insert(pointVec[i]);
								}
							}
							else
							{
								for( int i = bpointvec[0]; i<bpointvec[0]+bpointvec[1]; i++)
								{
									BirthPointSet.insert(pointVec[i]);
								}
							}
						}
						roomData->excHandle(MAKEINT64(WCH_LockData,1));
						roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
						JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
						JXJCampBirthpoint& birthpointSet = (*campMap)[myShili];
						for( JXJCampBirthpoint::iterator bitr = birthpointSet.begin();
							bitr != birthpointSet.end();
							++bitr )
						{
							BirthPointSet.insert(bitr->first);
						}
						roomData->excHandle(MAKEINT64(WCH_LockData,0)); 
						std::set<crVector3f>::iterator bpitr = BirthPointSet.begin();
						float mindist = (crVector2f((*bpitr)[0],(*bpitr)[1])*scale - deadPos).length();
						float dist;
						crVector3f nearBirthPoint = *bpitr;
						for( ++bpitr;
							bpitr != BirthPointSet.end();
							++bpitr )
						{
							dist = (crVector2f((*bpitr)[0], (*bpitr)[1])*scale - deadPos).length();
							if(dist<mindist)
							{
								mindist = dist;
								nearBirthPoint = *bpitr;
							}
						}
						crVector3f birthPoint;
						//rangei range(-nearBirthPoint[2],nearBirthPoint[2]);
						birthPoint[0] = nearBirthPoint[0]*scale;// + range.get_random();
						birthPoint[1] = nearBirthPoint[1]*scale;// + range.get_random();

						int id = 0;
						switch (inspireLv)
						{
						case 1:
							id = JXJExt_Inspire;
							break;
						case 2:
							id = JXJExt_Inspire_2;
							break;
						case 3:
							id = JXJExt_Inspire_3;
							break;
						case 4:
							id = JXJExt_Inspire_4;
							break;
						case 5:
							id = JXJExt_Inspire_5;
							break;
						case 6:
							id = JXJExt_Inspire_6;
							break;
						case 7:
							id = JXJExt_Inspire_7;
							break;
						case 8:
							id = JXJExt_Inspire_8;
							break;
						case 9:
							id = JXJExt_Inspire_9;
							break;
						case 10:
							id = JXJExt_Inspire_10;
							break;
						case 11:
							id = JXJExt_Inspire_11;
							break;
						case 12:
							id = JXJExt_Inspire_12;
							break;
						case 13:
							id = JXJExt_Inspire_13;
							break;
						case 14:
							id = JXJExt_Inspire_14;
							break;
						case 15:
							id = JXJExt_Inspire_15;
							break;
						}
						//
						int rbuff = 0;
						roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
						crVector2i battleRbuffids = *(crVector2i *)param;
						if(battleRbuffids[0] == playerid || battleRbuffids[1] == playerid)
						{
							rbuff = JXJExt_BattleBuff_1;
						}

						crVector3 pos;
						for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
							itr != roleMap.end();
							++itr )
						{
							role = itr->second.get();
							//birthPoint[0] = nearBirthPoint[0] + range.get_random();
							//birthPoint[1] = nearBirthPoint[1] + range.get_random();
							//z = scene->getPosZ(role->getLayerID(),birthPoint[0]*scale,birthPoint[1]*scale, role->getZoffset() * scale);
							if(!scene->findWalkablePos(role,crVector2(birthPoint[0],birthPoint[1]),c_walkableSearchRange,pos))
								CRCore::notify(CRCore::ALWAYS)<<"crJXJBattleAddArmyInfoPacket findWalkablePos失败 "<<birthPoint<<std::endl;
							else
							{
								pos/=scale;
								role->setPosxy(pos[0],pos[1]);
								role->setPosz(pos[2]);
							}
							roleData = role->getDataClass();
							roleData->excHandle(MAKEINT64(WCH_InitData,role));
							scene->itemRelive(itr->second.get());
							if(id>0)
								role->doEvent(WCH_ExtraHandle,MAKEINT64(MAKEINT32(id,-1),NULL));
							if(rbuff>0)
								role->doEvent(WCH_ExtraHandle,MAKEINT64(MAKEINT32(rbuff,-1),NULL));
						}
						crJXJBattleAddArmyInfoPacket packet_scene;
						crJXJBattleAddArmyInfoPacket::buildReplyPacket(packet_scene,playerid);
						serverConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet_scene);
					}
					
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crJXJBattleAddArmyInfoPacket packet;
				crJXJBattleAddArmyInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		crMyPlayerData::MyRoleNpcMap &roleNpcMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
		crRole *role;
		float dt = 0.1f;
		for( crMyPlayerData::MyRoleNpcMap::iterator itr = roleNpcMap.begin();
			itr != roleNpcMap.end();
			++itr )
		{
			role = itr->second.first.get();
			if(role)
			{
				role->doEvent(WCH_ChangeActState,MAKEINT64(ACT_Idle,&dt));
			}
		}
		crBrain::getInstance()->doEvent(WCH_JXJRecvAddArmySucess);
	}
}

/////////////////////////////////////////
//
//crJXJCampAddArmyInfoPacket
//
/////////////////////////////////////////
crJXJCampAddArmyInfoPacket::crJXJCampAddArmyInfoPacket():
	crStreamPacket(crJXJCampAddArmyInfoPacket::ID)
{
}
crJXJCampAddArmyInfoPacket::crJXJCampAddArmyInfoPacket( const crJXJCampAddArmyInfoPacket &p ):
	crStreamPacket(crJXJCampAddArmyInfoPacket::ID,p)
{
}
void crJXJCampAddArmyInfoPacket::buildRequestPacket(crJXJCampAddArmyInfoPacket &packet)
{
}
void crJXJCampAddArmyInfoPacket::buildRequestPacket2(crJXJCampAddArmyInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	packet.setStreamBuf(streamBuf);
	if(stream)
	{
		streamBuf->createBuf(4+stream->getBufSize());
		streamBuf->_writeInt(playerid);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->createBuf(4);
		streamBuf->_writeInt(playerid);
	}
}
void crJXJCampAddArmyInfoPacket::buildReplyPacket(crJXJCampAddArmyInfoPacket &packet,int playerid,char code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeChar(code);
}
void crJXJCampAddArmyInfoPacket::buildReplyPacket2(crJXJCampAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJCampAddArmyInfoPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;
	void *param;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crJXJCampAddArmyInfoPacket packet;
				crJXJCampAddArmyInfoPacket::buildRequestPacket2(packet,playerData->getPlayerID(),NULL);
				conductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	else if(netType == SceneServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *serverConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = serverConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
		{
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			if(roleMap.empty())
				return;
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			CRNetApp::crScene *scene = callback->findScene(playerData->getSceneID());
			crRoom *room = callback->findRoom(playerData->getRoomID());
			if(scene && room)
			{
				crRoomPlayer *roomplayer = room->getMember(playerid);
				if(roomplayer)
				{
					crSceneServerPlayerData::RoleMap & roleMap = playerData->getRoleMap();
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
						if (rtCount == 0)
						{
							addArmyInfoVec.push_back(std::make_pair(itr_role->second->getAbstractItemID(),std::make_pair(itr_role->second->getRoleID(),count-rtCount)));
						}
					}
					if(!addArmyInfoVec.empty())
					{
						//crData *roleData;
						float scale = crGlobalHandle::gData()->gUnitScale();
						crRole *role = roleMap.begin()->second.get();
						crVector2f deadPos(role->getPosx(),role->getPosy());
						deadPos *= scale;
						crSceneLayer *sceneLayer = scene->getSceneLayer(roleMap.begin()->second.get()->getLayerID());
						const crSceneLayer::BirthPointArray& pointVec = sceneLayer->getBirthPointArray();
						crData *roomData = room->getDataClass();
						roomData->getParam(WCHDATA_JXJBattleID,param);
						_crInt32 battleid = *(_crInt32 *)param;
						unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));
						unsigned short chengchiid = HIINT32(battleid);
						ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
						crTableIO::StrVec record;
						int chusindex = chengchiTab->getTitleIndex("出生点数");
						if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0)
						{
							crData *data = playerData->getPlayerGameData()->getDataClass();
							data->getParam(WCHDATA_JXJShiliID,param);
							unsigned char myShili = *(unsigned char *)param;

							crVector2i bpointvec;
							crArgumentParser::appAtoVec(record[chusindex],bpointvec);
							std::set<crVector3f> BirthPointSet;
							if(!pointVec.empty())
							{
								if(myShili == attackShili)
								{//攻击方
									for( int i = 0; i<bpointvec[0]; i++)
									{
										BirthPointSet.insert(pointVec[i]);
									}
								}
								else
								{
									for( int i = bpointvec[0]; i<bpointvec[0]+bpointvec[1]; i++)
									{
										BirthPointSet.insert(pointVec[i]);
									}
								}
							}
							roomData->excHandle(MAKEINT64(WCH_LockData,1));
							roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
							JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
							JXJCampBirthpoint& birthpointSet = (*campMap)[myShili];
							for( JXJCampBirthpoint::iterator bitr = birthpointSet.begin();
								bitr != birthpointSet.end();
								++bitr )
							{
								BirthPointSet.insert(bitr->first);
							}
							roomData->excHandle(MAKEINT64(WCH_LockData,0)); 
							std::set<crVector3f>::iterator bpitr = BirthPointSet.begin();
							float mindist = (crVector2f((*bpitr)[0],(*bpitr)[1])*scale - deadPos).length();
							float dist;
							crVector3f nearBirthPoint = *bpitr;
							for( ++bpitr;
								bpitr != BirthPointSet.end();
								++bpitr )
							{
								dist = (crVector2f((*bpitr)[0], (*bpitr)[1])*scale - deadPos).length();
								if(dist<mindist)
								{
									mindist = dist;
									nearBirthPoint = *bpitr;
								}
							}
							crVector3 camppos = role->getPosition();
							crVector3f birthPoint;
							//rangei range(-nearBirthPoint[2],nearBirthPoint[2]);
							birthPoint[0] = nearBirthPoint[0];// + range.get_random();
							birthPoint[1] = nearBirthPoint[1];// + range.get_random();
							if(!scene->findWalkablePos(role,crVector2(birthPoint[0]*scale,birthPoint[1]*scale),c_walkableSearchRange,camppos))
							{
								CRCore::notify(CRCore::ALWAYS)<<"crJXJCampAddArmyInfoPacket findWalkablePos失败 "<<birthPoint<<std::endl;
							}

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
							stream->_writeVec3(camppos);//12 camppos
							crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
							crJXJCampAddArmyInfoPacket packet;
							crJXJCampAddArmyInfoPacket::buildRequestPacket2(packet,playerData->getPlayerID(),stream.get());
							gameServerConductor->getNetManager()->sendPacket("all",packet);
						}
					}
				}
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));		
		if(playerData.valid() && playerData->getPlayerGameData())
		{
			unsigned char troopssize = m_streamBuf->_readUChar();
			std::vector< std::pair< int,std::pair< int,/*unsigned short*/int > > > addArmyInfoVec;
			TroopsMap recruitTroops;
			int abstractid,roleid;
			/*unsigned short*/int count;
			std::string addarmstr="sceneid"+crArgumentParser::appItoa(playerData->getSceneID())+"|";
			char code = -1;
			for( int i = 0; i<troopssize; i++ )
			{
				abstractid = m_streamBuf->_readInt();
				roleid = m_streamBuf->_readInt();
				count = m_streamBuf->_readInt();
				addArmyInfoVec.push_back(std::make_pair(abstractid,std::make_pair(roleid,count)));
				if(recruitTroops.find(abstractid)!=recruitTroops.end())
				{
					recruitTroops[abstractid]+=count;
				}
				else
				{
					recruitTroops[abstractid]=count;
				}
				addarmstr += crArgumentParser::appVectoa(crVector2i(abstractid,count));
			}
			
			crPlayerGameData *playerGameData = playerData->getPlayerGameData();
			crData *data = playerGameData->getDataClass();
			bool needdeductgold = false;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJTroopsMap,param);
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
			BattleRecruitCheckResult recruitCheck;
			playerGameData->doEvent(WCH_JXJBattleRecruitCheck,MAKEINT64(&recruitTroops,&recruitCheck));
			if(recruitCheck.m_code>=0)
			{//补兵成功
				for( TroopsMap::iterator itr = recruitTroops_copy.begin();
					itr != recruitTroops_copy.end();
					)
				{
					itr_troops = troops->find(itr->first);
					if (itr_troops != troops->end())
					{
						if(itr_troops->second>=itr->second)
						{
							itr_troops->second-=itr->second;
							//itr = recruitTroops.erase(itr);
							//continue;
						}
						else
						{
							//itr->second-=itr_troops->second;
							itr_troops->second = 0;
						}
					}
					++itr;
				}
				code = recruitCheck.m_code;

				data->getParam(WCHDATA_JXJCoppercash,param);
				int* mycopper = (int *)param;
				data->getParam(WCHDATA_JXJFood,param);
				int* myfood = (int *)param;
				data->getParam(WCHDATA_JXJWood,param);
				int* mywood = (int *)param;
				data->getParam(WCHDATA_JXJIron,param);
				int* myiron = (int *)param;
				data->getParam(WCHDATA_JXJHorse,param);
				int* myhorse = (int *)param;
				data->getParam(WCHDATA_MarketBuyResourceCount,param);
				int *MarketBuyResourceCount = (int*)param;
				data->getParam(WCHDATA_JXJRecruitTimes,param);
				crVector4i *recruitTimes = (crVector4i *)param;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;

				if(recruitCheck.m_copperneed>0)
					*mycopper -= recruitCheck.m_copperneed;
				if(recruitCheck.m_foodneed>0)
					*myfood -= recruitCheck.m_foodneed;
				if(recruitCheck.m_woodneed>0)
					*mywood -= recruitCheck.m_woodneed;
				if(recruitCheck.m_ironneed>0)
					*myiron -= recruitCheck.m_ironneed;
				if(recruitCheck.m_horseneed>0)
					*myhorse -= recruitCheck.m_horseneed;
				if(recruitCheck.m_timeneed>0)
				{//cd
					short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime,viplv).c_str()));
					int credittime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime,viplv).c_str()));
					int t;
					for(int i = 0; recruitCheck.m_timeneed>0 && i<4; i++)
					{
						if((*recruitTimes)[i]>=0 && (*recruitTimes)[i]<overTimes)
						{
							t = overTimes + credittime - (*recruitTimes)[i];
							if(recruitCheck.m_timeneed>t)
							{
								(*recruitTimes)[i] += t;
								recruitCheck.m_timeneed -= t;
							}
							else
							{
								(*recruitTimes)[i] += recruitCheck.m_timeneed;
								recruitCheck.m_timeneed = 0;
							}
						}
					}
					if(recruitCheck.m_timeneed>0)
					{
						for(int i = 0; i<4; i++)
						{
							if((*recruitTimes)[i]>=0 && (*recruitTimes)[i]<=overTimes)
							{
								(*recruitTimes)[i] += recruitCheck.m_timeneed;						
								break;
							}
						}
					}
				}
				if(recruitCheck.m_shijibuycount>0)
				{//市场够买上限
					*MarketBuyResourceCount += recruitCheck.m_shijibuycount;
				}
				data->getParam(WCHDATA_JXJGoldRecruitCount,param);
				unsigned char* goldRecruitCount = (unsigned char*)param;
				if(recruitCheck.m_goldneed>0)
				{//扣除元宝
					MoneyChangeData moneydata(recruitCheck.m_goldneed,"战场补兵");
					playerGameData->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
					//元宝复活计数递增;
					(*goldRecruitCount)++;
				}

				data->getParam(WCHDATA_JXJInspireLevel, param);
				unsigned char inspireLv = *(unsigned char *)param;

				crVector3 campPos = m_streamBuf->_readVec3();
				crJXJCampAddArmyInfoPacket packet;
				//crJXJCampAddArmyInfoPacket::buildReplyPacket(packet,playerData->getPlayerID(),_case);
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(19 + addArmyInfoVec.size() * 4);
				packet.setStreamBuf(streamBuf.get());
				streamBuf->_writeInt(playerid);
				streamBuf->_writeChar(recruitCheck.m_code);
				streamBuf->_writeVec3(campPos);
				streamBuf->_writeUChar(inspireLv);

				streamBuf->_writeChar(addArmyInfoVec.size());
				std::vector< std::pair< int,std::pair< int,/*unsigned short*/int > > >::iterator itr_need = addArmyInfoVec.begin();
				while (itr_need!=addArmyInfoVec.end())
				{
					streamBuf->_writeInt(itr_need->second.first);
					itr_need++;
				}
				m_netConductor->getNetManager()->sendPacket(sender,packet);

				ref_ptr<crStreamBuf> streamBufToClient = new crStreamBuf;
				streamBufToClient->createBuf(streamBuf->getBufSize()+42+recruitTroops_copy.size()*6);
				packet.setStreamBuf(streamBufToClient.get());
				streamBufToClient->write(streamBuf->getBuf(),streamBuf->getBufSize());
				streamBufToClient->_writeUChar(*goldRecruitCount);
				streamBufToClient->_writeChar(recruitTroops_copy.size());
				for( TroopsMap::iterator itr = recruitTroops_copy.begin();
					 itr != recruitTroops_copy.end();
					 ++itr )
				{
					streamBufToClient->_writeInt(itr->first);
					streamBufToClient->_writeInt(itr->second);
				}
				switch (recruitCheck.m_code)
				{
				case 0:
					streamBufToClient->_writeInt(*myfood);
					break;
				case 1:
					break;
				case 2:
					streamBufToClient->_writeInt(*mycopper);
					streamBufToClient->_writeInt(*myfood);
					streamBufToClient->_writeInt(*mywood);
					streamBufToClient->_writeInt(*myiron);
					streamBufToClient->_writeInt(*myhorse);
					break;
				case 3:
					streamBufToClient->_writeInt(*mycopper);
					streamBufToClient->_writeInt(*myfood);
					streamBufToClient->_writeInt(*mywood);
					streamBufToClient->_writeInt(*myiron);
					streamBufToClient->_writeInt(*myhorse);
					streamBufToClient->_writeInt(*MarketBuyResourceCount);
					streamBufToClient->_writeVec4i(*recruitTimes);
					break;
				}
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			else
			{
				crJXJCampAddArmyInfoPacket packet;
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(5);
				packet.setStreamBuf(streamBuf.get());
				streamBuf->_writeInt(playerid);
				streamBuf->_writeChar(recruitCheck.m_code);
				m_netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			if (needdeductgold)
			{
				crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
				callback->savePlayerData(playerData.get());
			}
			///游戏日志
			std::string logdata = "战场补兵0扣粮食1扣兵力2扣资源3扣元宝:" + crArgumentParser::appItoa(code)+addarmstr;;
			GameLogData gamelog(Log_SupplementArmy, logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
		}
//			//
//			int code = 0;
//			playerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_JXJVIPExtend,WCH_JXJRecvAddArmy),MAKEINT64(&code,NULL));
//			unsigned char vipLevel = 0;
//			char _case = -1;
//			int troopRTcount =0;
//			int copperneed=0,
//				woodneed = 0,
//				ironneed = 0,
//				horseneed = 0,
//				foodneed = 0,
//				timeneed = 0;
//			int	equipneed = 0,
//				equipid = 0;
//			int percopperneed = 0;
//			int perwoodneed = 0;
//			int perironneed = 0;
//			int perhorseneed = 0;
//			int perfoodneed = 0;
//			int pertimeneed = 0;
//			int perleaderneed = 0;
//			int totalLeaderCount = 0;
//			bool isTroopMapArmyEnough = true;
//
//			crVector3 campPos;
//			unsigned char inspireLv = 0;
//			std::vector< std::pair< int,std::pair< int,unsigned short > > > addArmyInfoVec;//abstractid,roleid,count
//			std::vector<CRCore::crVector4i> addArmyInfo;//roleid,roleabstractid,补兵数量,需要招募的数量
//			TroopsMap temptroops;
//			ref_ptr<crTableIO>equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
//			ref_ptr<crTableIO>troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
//
//			if(playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
//			{
//				crData *addArmyPlayerData = playerData->getPlayerGameData()->getDataClass();
//
//				addArmyPlayerData->getParam(WCHDATA_JXJVipLv,param);
//				unsigned char vipLevel = *(unsigned char *)param;
//				short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime, vipLevel).c_str()));
//				int credittime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime, vipLevel).c_str());
//				int addArmyExtraFood = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleAddArmyExtraFoodNeed, vipLevel).c_str());
//				addArmyPlayerData->excHandle(MAKEINT64(WCH_LockData,1));
//				//此处需要获取VIP等级
//				addArmyPlayerData->getParam(WCHDATA_JXJVipLv,param);
//				vipLevel = *(unsigned char *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJCoppercash,param);
//				int *mycopper = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJWood,param);
//				int *mywood = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJIron,param);
//				int *myiron = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJHorse,param);
//				int *myhorse = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJFood,param);
//				int *myfood = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJGoldingot,param);
//				int *myGold = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJGiftGold,param);
//				int *myGiftGold = (int *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJTroopsMap,param);
//				TroopsMap *troops = (TroopsMap*)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJFormationInfoVec,param);
//				FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJRecruitTimes,param);
//				crVector4i *recruitTimes = (crVector4i *)param;
//				addArmyPlayerData->getParam(WCHDATA_JXJInspireLevel, param);
//				inspireLv = *(unsigned char *)param;
//
//				unsigned char size = m_streamBuf->_readUChar();
//
//				int abstractid = 0,roleid = 0;
//				unsigned addArmyCount = 0;
//				for (unsigned char i = 0;i<size;i++)
//				{
//					abstractid = m_streamBuf->_readInt();
//					roleid = m_streamBuf->_readInt();
//					addArmyCount = m_streamBuf->_readUShort();
//
//					addArmyInfoVec.push_back(std::make_pair(abstractid,std::make_pair(roleid,addArmyCount)));
//
//					TroopsMap::iterator itr_troopsmap = troops->find(abstractid);
//					if(itr_troopsmap!=troops->end())
//						temptroops[abstractid] = itr_troopsmap->second;
//				}
//
//				//据点坐标
//				campPos = m_streamBuf->_readVec3();
//
//				std::vector< std::pair< int,std::pair< int,unsigned short > > >::iterator itr_need = addArmyInfoVec.begin();
//				for(;itr_need!=addArmyInfoVec.end();itr_need++)
//				{
//					int needcount = itr_need->second.second;
//					TroopsMap::iterator itr_tempmap = temptroops.find(itr_need->first);
//					if(itr_tempmap!=temptroops.end())
//					{
//						troopRTcount = itr_tempmap->second;
//						if(troopRTcount<needcount)
//						{
//							addArmyInfo.push_back(crVector4i(itr_need->second.first,itr_need->first,needcount,needcount-troopRTcount));
//							isTroopMapArmyEnough = false;
//							itr_tempmap->second = 0;
//						}
//						else
//						{
//							addArmyInfo.push_back(crVector4i(itr_need->second.first,itr_need->first,needcount,0));
//							itr_tempmap->second-=needcount;
//						}
//					}
//				}
//				if(isTroopMapArmyEnough)
//				{
//					if(vipLevel == 0)
//					{
//						if(*myfood>=addArmyExtraFood)
//							_case = 0;
//					}
//					else
//					{
//						_case = 1;
//					}
//				}
//				else
//				{
//					if(vipLevel > 1)
//					{
//						crTableIO::StrVec record,record2;
//
//// 						int troopcopperindex = troopTab->getTitleIndex("铜钱");
//// 						int troopfoodindex = troopTab->getTitleIndex("粮食");
//// 						int trooptimeIndex = troopTab->getTitleIndex("时间");
//// 						int troopequipidindex = troopTab->getTitleIndex("装备ID");
//// 						int troopequipcountindex = troopTab->getTitleIndex("装备数量");
//						int leaderneedindex = troopTab->getTitleIndex("领导力");
//
//						int equipcopperindex = equipTab->getTitleIndex("铜钱");
//						int equipfoodindex = equipTab->getTitleIndex("粮食");
//						int equiphorseindex = equipTab->getTitleIndex("马匹");
//						int equipironindex = equipTab->getTitleIndex("铁矿");
//						int equipwoodindex = equipTab->getTitleIndex("木材");
//						std::vector<CRCore::crVector4i>::iterator itr = addArmyInfo.begin();
//						while (itr!=addArmyInfo.end())
//						{
//							if(troopTab->queryOneRecord(0,crArgumentParser::appItoa((*itr)[1]),record)>=0)
//							{
//// 								if(troopcopperindex>0)
//// 									percopperneed = atoi(record[troopcopperindex].c_str());
//// 								if(troopfoodindex>0)
//// 									perfoodneed = atoi(record[troopfoodindex].c_str());
//// 								if(trooptimeIndex>0)
//// 									pertimeneed = atoi(record[trooptimeIndex].c_str());
//// 								if(troopequipidindex>0)
//// 									equipid = atoi(record[troopequipidindex].c_str());
//// 								if(troopequipcountindex)
//// 									equipneed = atoi(record[troopequipcountindex].c_str());
//								if(leaderneedindex>0)
//									perleaderneed = atoi(record[leaderneedindex].c_str());
//							}
//							RecruitCheckResult recruitCheck;
//							playerData->getPlayerGameData()->doEvent(WCH_JXJRecruitCheck,MAKEINT64((*itr)[1],&recruitCheck));
//
//							copperneed += (*itr)[3]*recruitCheck.m_needresourcevec[T_Copper - 1];
//							foodneed += (*itr)[3]*recruitCheck.m_needresourcevec[T_Food - 1];
//							woodneed += (*itr)[3]*recruitCheck.m_needresourcevec[T_Wood - 1];
//							ironneed += (*itr)[3]*recruitCheck.m_needresourcevec[T_Iron - 1];
//							horseneed += (*itr)[3]*recruitCheck.m_needresourcevec[T_Horse - 1];
//							timeneed += (*itr)[3]*recruitCheck.m_timeNeed;
//
//							if(equipTab->queryOneRecord(0,crArgumentParser::appItoa(recruitCheck.m_equipid),record2)>=0)
//							{
//								if(equipcopperindex>0)
//									percopperneed = atoi(record2[equipcopperindex].c_str());
//								if(equipfoodindex>0)
//									perfoodneed = atoi(record2[equipfoodindex].c_str());
//								if(equiphorseindex>0)
//									perhorseneed = atoi(record2[equiphorseindex].c_str());
//								if(equipironindex>0)
//									perironneed = atoi(record2[equipironindex].c_str());
//								if(equipwoodindex)
//									perwoodneed = atoi(record2[equipwoodindex].c_str());
//							}
//							copperneed += percopperneed * recruitCheck.m_equipCount *(*itr)[3];
//							woodneed += perwoodneed * recruitCheck.m_equipCount *(*itr)[3];
//							ironneed += perironneed * recruitCheck.m_equipCount * (*itr)[3];
//							horseneed += perhorseneed * recruitCheck.m_equipCount * (*itr)[3];
//							foodneed += perfoodneed * recruitCheck.m_equipCount *(*itr)[3];
//
//							itr++;
//						}
//						int timeRemain = 0;
//						for(int i = 0; i<4; i++)
//						{
//							if((*recruitTimes)[i]>=0 && (*recruitTimes)[i] < overTimes)
//								timeRemain += overTimes - (*recruitTimes)[i] + credittime;
//						}
//						if(copperneed > *mycopper || woodneed > *mywood || ironneed > *myiron || horseneed > *myhorse || foodneed > *myfood || timeneed > timeRemain)
//						{
//							if(vipLevel>=3)
//							{
//								int goldneed = totalLeaderCount/100+10;
//								if(goldneed<= *myGold + *myGiftGold)
//								{
//									_case = 3;
//								}
//							}
//						}
//						else
//						{
//							_case = 2;
//						}
//					}
//				}
//
//				std::vector<crVector4i>::iterator itr = addArmyInfo.begin();
//				while (itr!=addArmyInfo.end())
//				{
//					int count = (*itr)[2] - (*itr)[3];//补兵数量－招募数量
//					TroopsMap::iterator itr_troopsmap = troops->find((*itr)[1]);
//					if(itr_troopsmap!=troops->end())
//						itr_troopsmap->second-=count;
//
//					FormationInfoVec::iterator itr_for = formationInfoVec->begin();
//					while (itr_for!=formationInfoVec->end())
//					{
//						if((*itr_for)->getRoleID()==(*itr)[0])
//						{
//							(*itr_for)->setCount(itr_for->get()->getCount() + (*itr)[2]);
//							break;
//						}
//						itr_for++;
//					}
//					itr++;
//				}
//				addArmyPlayerData->excHandle(MAKEINT64(WCH_LockData,0));
//
//				//发包给客户端资源扣除
//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				//std::vector<CRCore::crVector4i>::iterator itr = addArmyInfo.begin();
//				if(_case==0)
//				{
//					//粮食扣除
//					(*myfood)-=addArmyExtraFood;
//					//发包
//					stream->createBuf(6+addArmyInfo.size()*8);
//					stream->_writeChar(_case);
//					stream->_writeChar(addArmyInfo.size());
//					stream->_writeInt(*myfood);
//					std::vector<CRCore::crVector4i>::iterator itr = addArmyInfo.begin();
//					while (itr!=addArmyInfo.end())
//					{
//						stream->_writeInt((*itr)[0]);
//						stream->_writeUShort((*itr)[2]);
//						stream->_writeUShort((*itr)[3]);
//						itr++;
//					}
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddArmy,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);						
//				}
//				else if(_case ==1)
//				{
//					//只扣兵力
//					stream->createBuf(2+addArmyInfo.size()*8);
//					stream->_writeChar(_case);
//					stream->_writeChar(addArmyInfo.size());
//					std::vector<CRCore::crVector4i>::iterator itr = addArmyInfo.begin();
//					while (itr!=addArmyInfo.end())
//					{
//						stream->_writeInt((*itr)[0]);
//						stream->_writeUShort((*itr)[2]);
//						stream->_writeUShort((*itr)[3]);
//						itr++;
//					}
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddArmy,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//				}
//				else if(_case == 2)
//				{
//					//扣除资源
//					*mycopper-=copperneed;
//					*myfood-=foodneed;
//					*myiron-=ironneed;
//					*mywood-=woodneed;
//					*myhorse-=horseneed;
//
//					for(int i = 0; timeneed>0 && i<4; i++)
//					{
//						if((*recruitTimes)[i]>=0 && (*recruitTimes)[i]<overTimes)
//						{
//							int t = overTimes + credittime - (*recruitTimes)[i];
//							if(timeneed>t)
//							{
//								(*recruitTimes)[i] += t;
//								timeneed -= t;
//							}
//							else
//							{
//								(*recruitTimes)[i] += timeneed;
//								timeneed = 0;
//							}
//						}
//					}
//
//					stream->createBuf(38+addArmyInfo.size()*8);
//					stream->_writeChar(_case);
//					stream->_writeChar(addArmyInfo.size());
//					stream->_writeInt(*mycopper);
//					stream->_writeInt(*myfood);
//					stream->_writeInt(*myiron);
//					stream->_writeInt(*mywood);
//					stream->_writeInt(*myhorse);
//					stream->_writeVec4i(*recruitTimes);
//					std::vector<CRCore::crVector4i>::iterator itr = addArmyInfo.begin();
//					while (itr!=addArmyInfo.end())
//					{
//						stream->_writeInt((*itr)[0]);
//						stream->_writeUShort((*itr)[2]);
//						stream->_writeUShort((*itr)[3]);
//						itr++;
//					}
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddArmy,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//				}
//				else if(_case == 3)
//				{
//					//元宝扣除
//					int goldneed = totalLeaderCount/100+10;
//					bool needdeductgold = false;
//					MoneyChangeData moneydata(goldneed,"战场据点补兵");
//					playerData->getPlayerGameData()->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
//
//					////元宝扣除增加成长值
//					//addArmyPlayerData->getParam(WCHDATA_JXJVipLv,param);
//					//unsigned char vipLevel = *(unsigned char *)param;
//					//int goldToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGoldToVipExpFactor, vipLevel).c_str());
//					//playerData->getPlayerGameData()->doEvent(WCH_JXJAddVipExp,MAKEINT64(goldneed * goldToVipExpFactor,NULL));
//					//发包
//					stream->createBuf(2+addArmyInfo.size()*8);
//					stream->_writeChar(_case);
//					stream->_writeChar(addArmyInfo.size());
//					itr = addArmyInfo.begin();
//					while (itr!=addArmyInfo.end())
//					{
//						stream->_writeInt((*itr)[0]);
//						stream->_writeUShort((*itr)[2]);
//						stream->_writeUShort((*itr)[3]);
//						itr++;
//					}
//					crPlayerDataEventPacket packet;
//					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddArmy,stream.get());
//					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//
//					if (needdeductgold)
//					{
//						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
//						callback->savePlayerData(playerData);
//					}
//				}
//			}
	}
	else if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		char code = m_streamBuf->_readChar();
		crVector3 compos = m_streamBuf->_readVec3();
		unsigned char inspireLv = m_streamBuf->_readUChar();

		char size = m_streamBuf->_readChar();
		std::set<int> roleSet;
		for (int i = 0;i<size;i++)
		{
			roleSet.insert(m_streamBuf->_readInt());
		}

		int id = 0;
		int rbuff = 0;
		switch (inspireLv)
		{
		case 1:
			id = JXJExt_Inspire;
			break;
		case 2:
			id = JXJExt_Inspire_2;
			break;
		case 3:
			id = JXJExt_Inspire_3;
			break;
		case 4:
			id = JXJExt_Inspire_4;
			break;
		case 5:
			id = JXJExt_Inspire_5;
			break;
		case 6:
			id = JXJExt_Inspire_6;
			break;
		case 7:
			id = JXJExt_Inspire_7;
			break;
		case 8:
			id = JXJExt_Inspire_8;
			break;
		case 9:
			id = JXJExt_Inspire_9;
			break;
		case 10:
			id = JXJExt_Inspire_10;
			break;
		case 11:
			id = JXJExt_Inspire_11;
			break;
		case 12:
			id = JXJExt_Inspire_12;
			break;
		case 13:
			id = JXJExt_Inspire_13;
			break;
		case 14:
			id = JXJExt_Inspire_14;
			break;
		case 15:
			id = JXJExt_Inspire_15;
			break;
		}
		//crVector3i birthPoint;
		float scale = crGlobalHandle::gData()->gUnitScale();
		//crVector3i nearBirthPoint = compos * scale;
		//rangei range(nearBirthPoint[2], nearBirthPoint[2]);

		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));		
		if(playerData.valid())
		{
			//
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
			CRNetApp::crScene *scene = callback->findScene(playerData->getSceneID());
			crRoom *room = callback->findRoom(playerData->getRoomID());

			if(code>=0)
			{
				if(room)
				{
					crData *roomData = room->getDataClass();
					if(roomData)
					{
						roomData->excHandle(MAKEINT64(WCH_LockData,1));
						roomData->getParam(WCHDATA_JXJCampAddArmyTimerMap,param);
						CampAddArmyTimerMap *timerMap = (CampAddArmyTimerMap *)param;

						time_t t = time(0);
						(*timerMap)[playerid] = t;

						roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
						crVector2i battleRbuffids = *(crVector2i *)param;
						if(battleRbuffids[0] == playerid || battleRbuffids[1] == playerid)
						{
							rbuff = JXJExt_BattleBuff_1;
						}

						roomData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}

				//补充兵力
				crSceneServerPlayerData::RoleMap & roleMap = playerData->getRoleMap();
				crData * roleData = NULL;
				/*unsigned short*/int rtCount = 0;
				/*unsigned short*/int count = 0;
				
				//编队补充
				crPlayerGameData *playerGameData = playerData->getPlayerGameData();
				if(playerGameData && playerGameData->getDataClass())
				{
					crData * data = playerGameData->getDataClass();
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJFormationInfoVec,param);
					FormationInfoVec *formationVec = (FormationInfoVec *)param;
					
					crSceneServerPlayerData::RoleMap::iterator itr_role;
					std::set<int>::iterator itr_add_role;
					FormationInfoVec::iterator itr = formationVec->begin();
					for (;itr!=formationVec->end();itr++)
					{
						itr_role = roleMap.find(itr->get()->getRoleID());
						if(itr_role == roleMap.end()) continue;;
						itr_add_role = roleSet.find(itr->get()->getRoleID());
						if(itr_add_role == roleSet.end()) continue;
						
						roleData = itr_role->second->getDataClass();
						if(!roleData) continue;
						roleData->getParam(WCHDATA_JXJArmyCount,param);
						count = *(/*unsigned short*/int *)param;
						roleData->getParam(WCHDATA_JXJRTArmyCount,param);
						rtCount = *(/*unsigned short*/int *)param;
						if(rtCount>=count) continue;

						roleData->inputParam(WCHDATA_JXJRTArmyCount, &count);
						itr->get()->setCount(count);

						if (rtCount <= 0)
						{
							//复活	
							
							itr_role->second->setPosxy(compos[0]/scale, compos[1]/scale);
							itr_role->second->setPosz(compos[2]/scale);

							roleData->excHandle(MAKEINT64(WCH_InitData, itr_role->second.get()));
							scene->itemRelive(itr_role->second.get());
							if (id > 0)
								itr_role->second->doEvent(WCH_ExtraHandle, MAKEINT64(MAKEINT32(id,-1), NULL));
							if (rbuff > 0)
								itr_role->second->doEvent(WCH_ExtraHandle, MAKEINT64(MAKEINT32(rbuff,-1), NULL));
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crJXJCampAddArmyInfoPacket packet;
				crJXJCampAddArmyInfoPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}	
		}
	}
	else if(netType == GameClient_Game)
	{//
		char code = m_streamBuf->_readChar();
		crVector3 compos = m_streamBuf->_readVec3();
		unsigned char inspireLv = m_streamBuf->_readUChar();
		char size = m_streamBuf->_readChar();
		std::set<int> roleSet;
		for (int i = 0;i<size;i++)
		{
			roleSet.insert(m_streamBuf->_readInt());
		}
		if(code>=0)
		{
			crMyPlayerData::getInstance()->lockMyRoleNpcMap();
			crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
			
			ref_ptr<crData>data;
			ref_ptr<crPlayerGameData>playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(playerGameData.valid())
			{
				data = playerGameData->getDataClass();
				unsigned char goldRecruitCount  = m_streamBuf->_readUChar();
				data->inputParam(WCHDATA_JXJGoldRecruitCount,&goldRecruitCount);
				crData * roleData = NULL;
				/*unsigned short*/int rtCount = 0;
				/*unsigned short*/int count = 0;
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJFormationInfoVec,param);
				FormationInfoVec *formationVec = (FormationInfoVec *)param;
				crMyPlayerData::MyRoleNpcMap::iterator itr_role = myRoles.begin();
				std::set<int>::iterator itr ;
				int roleid;
				float scale = crGlobalHandle::gData()->gUnitScale();
				for (;itr_role!=myRoles.end();itr_role++)
				{
					roleid = itr_role->second.first->getRoleID();
					itr = roleSet.find(roleid);
					if(itr == roleSet.end()) continue;

					roleData = itr_role->second.first->getDataClass();
					if(!roleData) continue;
					roleData->getParam(WCHDATA_JXJArmyCount,param);
					count = *(/*unsigned short*/int *)param;
					roleData->getParam(WCHDATA_JXJRTArmyCount,param);
					rtCount = *(/*unsigned short*/int *)param;
					if(rtCount>=count) continue;

					roleData->inputParam(WCHDATA_JXJRTArmyCount,&count);
					if (rtCount <= 0)
					{
						itr_role->second.first->setPosxy(compos[0]/scale, compos[1]/scale);
						itr_role->second.first->setPosz(compos[2]/scale);
						float dt = 0.1f;
						itr_role->second.first->doEvent(WCH_ChangeActState, MAKEINT64(ACT_Idle, &dt));
					}
					for( FormationInfoVec::iterator fitr = formationVec->begin();
						 fitr!=formationVec->end();
						 fitr++ )
					{
						if((*fitr)->getRoleID() == roleid)
						{
							(*fitr)->setCount(count);
							break;
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			
			crMyPlayerData::getInstance()->unlockMyRoleNpcMap();

			ref_ptr<crCamera> camera = CRPhysics::crCameraManager::getInstance()->getMainCamera();
			if(camera.valid())
			{
				crData *cameraData = camera->getAttachedNode()->getDataClass();
				float addArmyInterval = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCampAddArmyInterval, 0).c_str())+1.0f;
				if(cameraData)
					cameraData->inputParam(WCHDATA_JXJCampAddArmyTimer,&addArmyInterval);
			}
			///扣除资源
			if(data.valid())
			{
				void *param;
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJTroopsMap,param);
				TroopsMap *troops = (TroopsMap *)param;
				TroopsMap::iterator itr_troops;
				char size = m_streamBuf->_readChar();
				int abstractid;
				unsigned short count;
				for( int i = 0; i<size; i++ )
				{
					abstractid = m_streamBuf->_readInt();
					count = m_streamBuf->_readInt();
					itr_troops = troops->find(abstractid);
					if(itr_troops!=troops->end())
					{
						if(itr_troops->second>count)
							itr_troops->second -= count;
						else
						{
							itr_troops->second = 0;
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				switch (code)
				{
				case 0:
					{
						int food = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJFood, param);
						int extra = food - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Food, extra));
						data->inputParam(WCHDATA_JXJFood, &food);
					}
					break;
				case 1:
					break;
				case 2:
					{
						int copper = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJCoppercash, param);
						int extra = copper - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Copper, extra));
						data->inputParam(WCHDATA_JXJCoppercash, &copper);
						
						int food = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJFood, param);
						extra = food - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Food, extra));
						data->inputParam(WCHDATA_JXJFood, &food);

						int wood = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJWood, param);
						extra = wood - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Wood, extra));
						data->inputParam(WCHDATA_JXJWood, &wood);

						int iron = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJIron, param);
						extra = iron - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Iron, extra));
						data->inputParam(WCHDATA_JXJIron, &iron);

						int horse = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJHorse, param);
						extra = horse - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Horse, extra));
						data->inputParam(WCHDATA_JXJHorse, &horse);
					}
					break;
				case 3:
					{
						int copper = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJCoppercash, param);
						int extra = copper - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Copper, extra));
						data->inputParam(WCHDATA_JXJCoppercash, &copper);

						int food = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJFood, param);
						extra = food - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Food, extra));
						data->inputParam(WCHDATA_JXJFood, &food);

						int wood = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJWood, param);
						extra = wood - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Wood, extra));
						data->inputParam(WCHDATA_JXJWood, &wood);

						int iron = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJIron, param);
						extra = iron - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Iron, extra));
						data->inputParam(WCHDATA_JXJIron, &iron);

						int horse = m_streamBuf->_readInt();
						data->getParam(WCHDATA_JXJHorse, param);
						extra = horse - (*(int *)param);
						if (extra != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_Horse, extra));
						data->inputParam(WCHDATA_JXJHorse, &horse);

						int MarketBuyResourceCount = m_streamBuf->_readInt();
						data->inputParam(WCHDATA_MarketBuyResourceCount, &MarketBuyResourceCount);
						crVector4i recruitTimes = m_streamBuf->_readVec4i();
						data->inputParam(WCHDATA_JXJRecruitTimes, &recruitTimes);
					}
					break;
				}
			}
			crBrain::getInstance()->doEvent(WCH_JXJRecvAddArmySucess);
		}
		else
		{
			crBrain::getInstance()->doEvent(WCH_JXJRecvAddArmyFaild);
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerMetierEventPacket
//
/////////////////////////////////////////
crJXJServerMetierEventPacket::crJXJServerMetierEventPacket():
	crStreamPacket(crJXJServerMetierEventPacket::ID)
{
}
crJXJServerMetierEventPacket::crJXJServerMetierEventPacket( const crJXJServerMetierEventPacket &p ):
	crStreamPacket(crJXJServerMetierEventPacket::ID,p)
{
}
void crJXJServerMetierEventPacket::buildRequestPacket(crJXJServerMetierEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crJXJServerMetierEventPacket::buildRequestPacket2(crJXJServerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJServerMetierEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crJXJServerMetierEventPacket packet;
				crJXJServerMetierEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerMetierEventPacket "<<playerData->getPlayerID()<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		_crInt64 msg = m_streamBuf->_readInt64();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(m_netConductor->getNetDataManager()->getPlayerData(playerid));
		if(netPlayerData.valid() && netPlayerData->getPlayerGameData())
		{
			crRole *role = netPlayerData->getPlayerGameData()->getMainRole();
			if (role) role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			//CRCore::notify(CRCore::ALWAYS)<<"crPlayerServerMetierEventPacket "<<std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerMetierEventPacket
//
/////////////////////////////////////////
crJXJPlayerMetierEventPacket::crJXJPlayerMetierEventPacket():
	crStreamPacket(crJXJPlayerMetierEventPacket::ID)
{
}

crJXJPlayerMetierEventPacket::crJXJPlayerMetierEventPacket( const crJXJPlayerMetierEventPacket &p ):
	crStreamPacket(crJXJPlayerMetierEventPacket::ID,p)
{
}

void crJXJPlayerMetierEventPacket::buildRequestPacket(crJXJPlayerMetierEventPacket &packet, _crInt64 msg, CRCore::crStreamBuf *param)
{//client send to server
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crJXJPlayerMetierEventPacket::buildRequestPacket(crJXJPlayerMetierEventPacket &packet, int recvid, int playerid, _crInt64 msg, CRCore::crStreamBuf *param)
{//server send to client
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(16+(param?param->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	if(param) streamBuf->write(param->getBuf(),param->getBufSize());
}
void crJXJPlayerMetierEventPacket::buildRequestPacket2(crJXJPlayerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crJXJPlayerMetierEventPacket::buildReplyPacket(crJXJPlayerMetierEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(12+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeInt64(msg);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}

void crJXJPlayerMetierEventPacket::buildReplyPacket2(crJXJPlayerMetierEventPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crJXJPlayerMetierEventPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getSceneServerLineConductor();
			if(conductor)
			{
				crJXJPlayerMetierEventPacket packet;
				crJXJPlayerMetierEventPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crPlayerMetierEventPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid() && netPlayerData->getPlayerGameData())
		{
			crRole *role = netPlayerData->getPlayerGameData()->getMainRole();
			if(role)
			{
				_crInt64 msg = m_streamBuf->_readInt64();
				role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crPlayerMetierEventPacket "<<std::endl;
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJPlayerMetierEventPacket packet;
			crJXJPlayerMetierEventPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		_crInt64 msg = m_streamBuf->_readInt64();
		crRole *role = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		if(role)
			role->doMetierEvent(msg,MAKEINT64(m_streamBuf.get(),netType));
	}
}

/////////////////////////////////////////
//
//crJXJDropedItemPacket
//
/////////////////////////////////////////
crJXJDropedItemPacket::crJXJDropedItemPacket():
	crStreamPacket(crJXJDropedItemPacket::ID)
{
}
crJXJDropedItemPacket::crJXJDropedItemPacket( const crJXJDropedItemPacket &p ):
	crStreamPacket(crJXJDropedItemPacket::ID,p)
{
}
void crJXJDropedItemPacket::buildRequestPacket(crJXJDropedItemPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJDropedItemPacket::buildReplyPacket2(crJXJDropedItemPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJDropedItemPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJRecvSceneItemDrop,MAKEINT64(m_streamBuf.get(),NULL));
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int recvid = m_streamBuf->_readInt();
		crNetConductor *connectServerConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServerConductor)
		{
			crNetDataManager *netDataManager = connectServerConductor->getNetDataManager();
			ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerData(recvid));
			if(playerData.valid())
			{
				crJXJDropedItemPacket packet;
				crJXJDropedItemPacket::buildReplyPacket2(packet,m_streamBuf.get());
				connectServerConductor->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJGameChatPacket
//
/////////////////////////////////////////
//CRCore::ref_ptr<CREncapsulation::crTableIO> crJXJGameChatPacket::s_noChatTab = NULL;
//CRCore::ref_ptr<CREncapsulation::crTableIO> crJXJGameChatPacket::s_ADTab = NULL;

crJXJGameChatPacket::crJXJGameChatPacket():
	crStreamPacket(crJXJGameChatPacket::ID)
{
}
crJXJGameChatPacket::crJXJGameChatPacket( const crJXJGameChatPacket &p ):
	crStreamPacket(crJXJGameChatPacket::ID,p)
{
}
void crJXJGameChatPacket::buildRequestPacket(crJXJGameChatPacket &packet,char chatmode,const std::string &chattext)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5+chattext.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(chatmode);
	streamBuf->_writeString(chattext);
}
void crJXJGameChatPacket::buildRequestPacket(crJXJGameChatPacket &packet,char chatmode,const std::string &recvname,const std::string &chattext)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9+chattext.length() + recvname.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(chatmode);
	streamBuf->_writeString(recvname);
	streamBuf->_writeString(chattext);
}
void crJXJGameChatPacket::buildRequestPacket2(crJXJGameChatPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJGameChatPacket::buildReplyPacket(crJXJGameChatPacket &packet,int recvid,char chatmode,const std::string &sendername,const std::string &chattext)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(13+chattext.length()+sendername.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(chatmode);
	streamBuf->_writeString(sendername);
	streamBuf->_writeString(chattext);
}
void crJXJGameChatPacket::buildReplyPacket(crJXJGameChatPacket &packet,int recvid,char chatmode,unsigned char sendershili,const std::string &sendername,const std::string &chattext)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(14+chattext.length()+sendername.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(chatmode);
	streamBuf->_writeUChar(sendershili);
	streamBuf->_writeString(sendername);
	streamBuf->_writeString(chattext);
}
void crJXJGameChatPacket::buildReplyPacket2(crJXJGameChatPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJGameChatPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			//if (s_noChatTab.valid() == false)
			//{
			//	reloadNoChatTab();
			//}
			//if (s_ADTab.valid() == false)
			//{
			//	s_ADTab = crTableIO::openFile("AD.crb");
			//}
			//if(s_noChatTab.valid())
			//{
			//	crTableIO::StrVec record;
			//	if (s_noChatTab->queryOneRecord(0,crArgumentParser::appItoa(playerData->getPlayerID()),record) >= 0)
			//	{
			//		return;
			//	}
			//}
			///////解析聊天内容,GM指令判断及广告词截取
			int bufSize = m_streamBuf->getBufSize();
			char chatmode = m_streamBuf->_readChar();
			if (chatmode == ChatOneToOne)
			{
				std::string recvname;
				std::string chat_text;
				recvname = m_streamBuf->_readString();
				chat_text = m_streamBuf->_readString();

				//if(chat_text.find("/reloadnochattab") == 0)
				//{
				//	// 刷新禁言tab
				//	if (isGMer) noChatTab = crTableIO::openFile("nochat.crb");
				//}
				//else if (chat_text.find("/reloadADTab") == 0)
				//{
				//	// 刷新广告屏蔽tab
				//	if (isGMer) ADTab = crTableIO::openFile("AD.crb");
				//}
				//else if (chat_text.find("/reloadGMerTab") == 0)
				//{
				//	// 刷新Gmtab
				//	if (isGMer) crGlobalHandle::gData()->reloadGMTable("gm.tab");
				//}
				if (chat_text.find("/jxjping") == 0)
				{
					//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
					//int permissionIdx = gmTab->getTitleIndex("权限");
					//int idIdx = gmTab->getTitleIndex("playerid");
					//crTableIO::StrVec record;
					//bool isGMer = false;
					//if (gmTab->queryOneRecord(idIdx,crArgumentParser::appItoa(playerData->getPlayerID()),record) >= 0)
					//{
					//	if (atoi(record[permissionIdx].c_str()) == JXJGMPermission_manager)
					//	{
					//		isGMer = true;
					//	}
					//}

					//if (isGMer)
					//{
						CRCore::Timer_t end_tick2 = CRCore::Timer::instance()->tick();

						//chat_text = chat_text + " gateway " + crArgumentParser::appI64toa(end_tick2);
						chat_text = chat_text + " gateway ";
						ref_ptr<crStreamBuf > streamBuf = new crStreamBuf;
						streamBuf->createBuf(4 + chat_text.size());
						streamBuf->_writeString(chat_text);
						streamBuf->seekBegin();

						crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
						crJXJPingPacket packet;
						crJXJPingPacket::buildReplyPacket2(packet,streamBuf.get());
						connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
					//}
					//else 
					//{
					//	return;
					//}
	
					//crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
					//crJXJGameChatPacket packet;
					//crJXJGameChatPacket::buildReplyPacket2(packet,streamBuf.get());

					//crPlayerDataEventPacket packet;
					//crPlayerDataEventPacket::buildReplyPacket(packet,playerData->getPlayerID(),WCH_JXJRecvPingMsg,streamBuf.get());
					//connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);

					//crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
					//crNetManager *netManager = m_netConductor->getNetManager();

					//chat_text = chat_text + " gateway ";
					//ref_ptr<crStreamBuf > streamBuf = new crStreamBuf;
					//streamBuf->createBuf(4 + chat_text.size());
					//streamBuf->_writeString(chat_text);

					//// 回给客户端
					//crPlayerDataEventPacket packet2;
					//crPlayerDataEventPacket::buildReplyPacket(packet2,playerData->getPlayerID(),WCH_JXJRecvPingMsg,streamBuf.get());
					//netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet2);
				}
			}
			//else chat_text = m_streamBuf->_readString();
			//if (s_ADTab.valid())
			//{
			//	for (int i = 0; i < s_ADTab->getRowCount(); ++i)
			//	{
			//		if (chat_text.find((*s_ADTab)(i,0)) == 0)
			//		{
			//			return ;
			//		}
			//	}
			//}
			m_streamBuf->seekBegin();
			m_streamBuf->setBufSize(bufSize);

			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crJXJGameChatPacket packet;
				crJXJGameChatPacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryBattleTimerPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crNetManager *netManager = m_netConductor->getNetManager();
		ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
		void *param;
		serverData->excHandle(MAKEINT64(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJNoChatSet,param);
		NoChatSet *nochatset = (NoChatSet *)param;
		if(nochatset->find(playerid)!=nochatset->end())
		{//向玩家回复被禁言
			if(senderPlayerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5);
				stream->_writeInt(playerid);
				stream->_writeChar(-1);
				crJXJGameChatPacket packet;
				packet.setStreamBuf(stream.get());
				netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet);
			}
			serverData->excHandle(MAKEINT64(WCH_LockData,0));
			return;
		}
		serverData->excHandle(MAKEINT64(WCH_LockData,0));
		char chatmode = m_streamBuf->_readUChar();
		std::string recvname;
		if (chatmode == ChatOneToOne)
		{
			recvname = m_streamBuf->_readString();
		}
		std::string chattext = m_streamBuf->_readString();
		ref_ptr<crTableIO> invalidTextTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidTextTab);
		if (invalidTextTab.valid())
		{//广告词屏蔽
			int count = invalidTextTab->getRowCount();
			for (int i = 0; i < count; ++i)
			{
				if (chattext.find((*invalidTextTab)(i,0)) == 0)
				{
					return ;
				}
			}
		}

		if(senderPlayerData.valid() && senderPlayerData->getPlayerGameData() && senderPlayerData->getPlayerGameData()->getDataClass())
		{
			crData *data;
			crPlayerGameData *playerGameData = senderPlayerData->getPlayerGameData();
			data = playerGameData->getDataClass();
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char sendershili = *(unsigned char *)param;
			//聊天数据包,给其它玩家发包
			//crJXJGameChatPacket packet;
			//if(chatmode == ChatInWorld)
			//{
			//	crJXJGameChatPacket::buildReplyPacket(packet,0,chatmode,sendershili,senderPlayerData->getCharacterName(),chattext);
			//}
			//else
			//{
			//	crJXJGameChatPacket::buildReplyPacket(packet,0,chatmode,senderPlayerData->getCharacterName(),chattext);
			//}
			//crStreamBuf *pkgstream = packet.getStreamBuf();
			//int pkgsize = pkgstream->getBufSize();
			///////////////////////////////////////////////////
			//unsigned char done = 0;
			if(chatmode == ChatInWorld)
			{
				//done = 1;
				//netDataManager->lockPlayerDataMap();
				//crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				//for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				//	itr != playerDataMap.end();
				//	++itr )
				//{
				//	if(itr->first == playerid) continue;
				//	recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				//	if (recvPlayerData.valid())
				//	{
				//		pkgstream->seekBegin();
				//		pkgstream->_writeInt(recvPlayerData->getPlayerID());
				//		pkgstream->setBufSize(pkgsize);
				//		netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
				//	}
				//}
				//netDataManager->unLockPlayerDataMap();
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int chatprice = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldChatPrice, viplv).c_str());
				data->getParam(WCHDATA_JXJTechWorldTalkPriceLv, param);
				unsigned char techlv = *(unsigned char *)param;
				if (techlv > 0)
				{
					int chatpricereducepercent = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTechWorldTalkPriceReduce, viplv).c_str());
					chatprice = chatprice * (100 - chatpricereducepercent) / 100;
				}
				playerGameData->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&chatprice,NULL));
				if (chatprice == 0)
				{
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo(senderPlayerData->getCharacterName(),chattext,sendershili,0);
					chatInfo->setSenderID(playerid);
					crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
					serverData->excHandle(MAKEINT64(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(chatInfo.get(),NULL));
					serverData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			if (chatmode == ChatInCountry)
			{
				//done = 1;
				ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo(senderPlayerData->getCharacterName(),chattext,sendershili,sendershili);
				chatInfo->setSenderID(playerid);
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				serverData->excHandle(MAKEINT64(WCH_LockData,1));
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(chatInfo.get(),NULL));
				serverData->excHandle(MAKEINT64(WCH_LockData,0));
				//netDataManager->lockPlayerDataMap();
				//crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				//for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				//	itr != playerDataMap.end();
				//	++itr )
				//{
				//	if(itr->first == playerid) continue;
				//	recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				//	if (recvPlayerData.valid())
				//	{
				//		playerGameData = recvPlayerData->getPlayerGameData();
				//		if(playerGameData && playerGameData->getDataClass())
				//		{
				//			data = playerGameData->getDataClass();
				//			data->getParam(WCHDATA_JXJShiliID,param);
				//			unsigned char recvshili = *(unsigned char *)param;

				//			if(recvshili == sendershili)
				//			{
				//				pkgstream->seekBegin();
				//				pkgstream->_writeInt(recvPlayerData->getPlayerID());
				//				pkgstream->setBufSize(pkgsize);
				//				netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
				//			}
				//		}
				//	}
				//}
				//netDataManager->unLockPlayerDataMap();
			}
			if (chatmode == ChatOneToOne)
			{
				if (chattext.find("/jxjping") == 0)
				{
					chattext = chattext + " gameserver ";
					ref_ptr<crStreamBuf > streamBuf = new crStreamBuf;
					streamBuf->createBuf(4 + chattext.size());
					streamBuf->_writeString(chattext);

					crPlayerDataEventPacket packet2;
					crPlayerDataEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvPingMsg,streamBuf.get());
					netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet2);
				}
				else
				{
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo(senderPlayerData->getCharacterName(),chattext,sendershili,sendershili);
					chatInfo->setRecvName(recvname);
					chatInfo->setSenderID(playerid);
					crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
					serverData->excHandle(MAKEINT64(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(chatInfo.get(),NULL));
					serverData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			else if (chatmode == ChatInTeam)
			{
				ref_ptr<crGameServerPlayerData> recvPlayerData;
				data->getParam(WCHDATA_JXJTeamLeaderID,param);
				int leaderid = *(int *)param;
				if (leaderid > 0)
				{
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo(senderPlayerData->getCharacterName(),chattext,sendershili,sendershili);
					chatInfo->setLeaderID(leaderid);
					chatInfo->setSenderID(playerid);
					crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
					serverData->excHandle(MAKEINT64(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(chatInfo.get(),NULL));
					serverData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				else
				{
					crPlayerDataEventPacket packet2;
					crPlayerDataEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvGameChat,NULL);
					netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet2);
				}
			}
			else if (chatmode == ChatInGroup)
			{
				if (playerGameData->getMainRole() && playerGameData->getMainRole()->getDataClass())
				{
					//crData *mainroledata = playerGameData->getMainRole()->getDataClass();
					crData *data = playerGameData->getDataClass();
					data->getParam(WCHDATA_JXJConsortiaID,param);
					int groupid = *(int *)param;
					if (groupid > 0)
					{
						//done = 1;
						ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo(senderPlayerData->getCharacterName(),chattext,sendershili,sendershili,groupid);
						chatInfo->setSenderID(playerid);
						crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
						serverData->excHandle(MAKEINT64(WCH_LockData,1));
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(chatInfo.get(),NULL));
						serverData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					else
					{
						crPlayerDataEventPacket packet2;
						crPlayerDataEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvGameChat,NULL);
						netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet2);
					}
				}
			}
			//if (chatmode!=ChatInRoomOneToOne)
			//{
			//	ref_ptr<crStreamBuf> streamBuf2 = new crStreamBuf;
			//	streamBuf2->createBuf(1);
			//	streamBuf2->_writeUChar(done);
			//	crPlayerDataEventPacket packet2;
			//	crPlayerDataEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvGameChat,streamBuf2.get());
			//	netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet2);
			//}
			//
		}
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJGameChatPacket packet;
			crJXJGameChatPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJGameChatPacket packet;
			crJXJGameChatPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		//std::string chat_text = m_streamBuf->_readString();
		//m_streamBuf->seekBegin();
		//if (chat_text.find("/jxjping") == 0)
		//{
		//	//int bufSize = m_streamBuf->getBufSize();
		//	//m_streamBuf->seekBegin();
		//	//m_streamBuf->setBufSize(bufSize);
		//	crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJRecvPingMsg,MAKEINT64(m_streamBuf.get(),GameClient_Game));
		//}
		//else
		{
			if(crRunGameHandle::getInstance()->isInManor() || crRunGameHandle::getInstance()->isInBattle())
				crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJRecvGameChat,MAKEINT64(m_streamBuf.get(),NULL));
		}
	}	
}
//void JXJ::crJXJGameChatPacket::reloadNoChatTab()
//{
//	CRIOManager::crScriptLoadManager::getInstance()->eraseLoadedRefeanced("nochat.crb");
//	s_noChatTab = crTableIO::openFile("nochat.crb");
//}
//
//CREncapsulation::crTableIO* JXJ::crJXJGameChatPacket::getNoChatTabPtr()
//{
//	if (!s_noChatTab.valid())
//	{
//		s_noChatTab = crTableIO::openFile("nochat.crb");
//	}
//	return s_noChatTab.get();
//}

/////////////////////////////////////////
//
//crJXJBattleInspirePacket
//
/////////////////////////////////////////
crJXJBattleInspirePacket::crJXJBattleInspirePacket():
	crStreamPacket(crJXJBattleInspirePacket::ID)
{
}
crJXJBattleInspirePacket::crJXJBattleInspirePacket( const crJXJBattleInspirePacket &p ):
	crStreamPacket(crJXJBattleInspirePacket::ID,p)
{
}
void crJXJBattleInspirePacket::buildRequestPacket(crJXJBattleInspirePacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleInspirePacket::buildReplyPacket(crJXJBattleInspirePacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crJXJBattleInspirePacket::buildReplyPacket(crJXJBattleInspirePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJBattleInspirePacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		char inspireLv = m_streamBuf->_readChar();//0
		crNetConductor *serverConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(serverConductor)
		{
			crNetDataManager *netDataManager = serverConductor->getNetDataManager();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid())
			{
				int id = 0;
				switch (inspireLv)
				{
				case 1:
					id = JXJExt_Inspire;
					break;
				case 2:
					id = JXJExt_Inspire_2;
					break;
				case 3:
					id = JXJExt_Inspire_3;
					break;
				case 4:
					id = JXJExt_Inspire_4;
					break;
				case 5:
					id = JXJExt_Inspire_5;
					break;
				case 6:
					id = JXJExt_Inspire_6;
					break;
				case 7:
					id = JXJExt_Inspire_7;
					break;
				case 8:
					id = JXJExt_Inspire_8;
					break;
				case 9:
					id = JXJExt_Inspire_9;
					break;
				case 10:
					id = JXJExt_Inspire_10;
					break;
				case 11:
					id = JXJExt_Inspire_11;
					break;
				case 12:
					id = JXJExt_Inspire_12;
					break;
				case 13:
					id = JXJExt_Inspire_13;
					break;
				case 14:
					id = JXJExt_Inspire_14;
					break;
				case 15:
					id = JXJExt_Inspire_15;
					break;
				}
				if(id>0)
				{
					crPlayerGameData *playerGameData = playerData->getPlayerGameData();
					if(playerGameData)
					{
						crData *data = playerGameData->getDataClass();
						if(data)
						{
							crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
							crSceneServerPlayerData::RoleMap::iterator itr_role = roleMap.begin();
							for (;itr_role!=roleMap.end();itr_role++)
							{
								itr_role->second->doEvent(WCH_ExtraHandle,MAKEINT64(MAKEINT32(id,-1),NULL));
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
//crJXJBattleRBuffPacket
//
/////////////////////////////////////////
crJXJBattleRBuffPacket::crJXJBattleRBuffPacket():
	crStreamPacket(crJXJBattleRBuffPacket::ID)
{
}
crJXJBattleRBuffPacket::crJXJBattleRBuffPacket( const crJXJBattleRBuffPacket &p ):
	crStreamPacket(crJXJBattleRBuffPacket::ID,p)
{
}
void crJXJBattleRBuffPacket::buildRequestPacket(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream)
{
	packet.setStreamBuf(stream);
}
void crJXJBattleRBuffPacket::buildReplyPacket(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJBattleRBuffPacket::buildReplyPacket2(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJBattleRBuffPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		char rbuff = m_streamBuf->_readChar();//0
		crNetConductor *serverConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(serverConductor)
		{
			crNetDataManager *netDataManager = serverConductor->getNetDataManager();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				int roomid = playerData->getRoomID();
				ref_ptr<crSceneServerCallback>callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
				ref_ptr<crRoom>room = callback->findRoom(roomid);
				if(room.valid())
				{
					void *param;
					crData *roomdata = room->getDataClass();
					int id = 0;
					if (rbuff = 1)
					{
						//crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
						//crRole *role = roleMap.begin()->second.get();
						//ref_ptr<crInstanceItem>* itemptr = NULL;
						//roomdata->excHandle(MAKEINT64(WCH_LockData,1));
						//if(myShili == ashili)
						//{
						//	roomdata->getParam(WCHDATA_JXJAttackRBuffItem,param);
						//	itemptr = (ref_ptr<crInstanceItem>*)param;
						//}
						//else
						//{
						//	roomdata->getParam(WCHDATA_JXJDefendRBuffItem,param);
						//	itemptr = (ref_ptr<crInstanceItem>*)param;
						//}
						//roomdata->excHandle(MAKEINT64(WCH_LockData,0));

						//if(itemptr)
						//{
						//	id = JXJExt_BattleBuff_1;
						//	*itemptr = new crInstanceItem;
						//	(*itemptr)->setInstanceItemID(crGlobalHandle::gainTemporaryItemID());
						//	(*itemptr)->setDropItemTimer(room->timeRemain());
						//	(*itemptr)->setItemtype(crInstanceItem::InstanceItem);
						//	(*itemptr)->setSceneID(room->getSceneID());
						//	(*itemptr)->setRoomID(room->getRoomID());
						//	(*itemptr)->setLayerID(role->getLayerID());

						//	ref_ptr<crAbstractItem> abstractitem = new crAbstractItem;
						//	abstractitem->setScript("JXJ/script/item/rbuffitem.cfg");
						//	(*itemptr)->setAbstractItem(abstractitem.get());
						//	(*itemptr)->loadItemData(0);
						//	crData *itemdata = (*itemptr)->getDataClass();
						//	if(itemdata)
						//	{
						//		(*itemptr)->setDataClass(itemdata);
						//		itemdata->inputParam(WCHDATA_Camp,&myShili);
						//		int itemcount = 1;
						//		itemdata->inputParam(WCHDATA_JXJArmyCount,&itemcount);
						//		(*itemptr)->setSightInfo(role->getSightInfo());

						//		room->getScene()->insertSceneItem((*itemptr).get());
						//		room->getScene()->itemRelive((*itemptr).get());
						//	}
						//	(*itemptr)->doEvent(WCH_ExtraHandle,MAKEINT64(MAKEINT32(id,-1),NULL));
						//}
						id = JXJExt_BattleBuff_1;
						crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
						crSceneServerPlayerData::RoleMap::iterator itr_role = roleMap.begin();
						for (;itr_role!=roleMap.end();itr_role++)
						{
							itr_role->second->doEvent(WCH_ExtraHandle,MAKEINT64(MAKEINT32(id,-1),NULL));
						}

						roomdata->getParam(WCHDATA_JXJBattleRBuffId,param);
						crVector2i battleRbuffids = *(crVector2i *)param;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(12);
						stream->_writeInt(0);
						stream->_writeVec2i(battleRbuffids);

						crJXJBattleRBuffPacket packet;
						crJXJBattleRBuffPacket::buildReplyPacket(packet,stream.get());
						room->sendPacketToAll(packet);
					}
					else
					{
						roomdata->getParam(WCHDATA_JXJBattleID,param);
						_crInt32 battleid = *(_crInt32 *)param;
						unsigned char ashili = LOINT16(LOINT32(battleid));
						unsigned char dshili = HIINT16(LOINT32(battleid));//(*chengchimap)[chengchiid]->getChengzhuShili();
						crPlayerGameData *playerGameData = playerData->getPlayerGameData();
						crData *mydata = playerGameData->getDataClass();
						mydata->getParam(WCHDATA_JXJShiliID,param);
						unsigned char myShili = *(unsigned char *)param;

						roomdata->excHandle(MAKEINT64(WCH_LockData,1));
						roomdata->getParam(WCHDATA_JXJBattleRBuffId,param);
						crVector2i* battleRbuffids = (crVector2i *)param;
						if(myShili == ashili)
						{
							(*battleRbuffids)[0] = 0;
						}
						else
						{
							(*battleRbuffids)[1] = 0;
						}
						roomdata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
	}
	else if(netType == GameConnectServerClient_Scene)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJBattleRBuffPacket packet;
			crJXJBattleRBuffPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		ref_ptr<crRoom>room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room.valid() && room->getDataClass())
		{
			void *param;
			crData *roomData = room->getDataClass();
			roomData->excHandle(MAKEINT64(WCH_LockData,1));
			roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
			crVector2i *battlebuff = (crVector2i *)param;
			*battlebuff = m_streamBuf->_readVec2i();
			roomData->excHandle(MAKEINT64(WCH_LockData,0));
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_CopyBattle_Name");
			canvas->doEvent(WCH_UI_UpdateData);
		}

	}	
}
/////////////////////////////////////////
//
//crJXJSaveLogPacket
//
/////////////////////////////////////////
crJXJSaveLogPacket::crJXJSaveLogPacket():
	crStreamPacket(crJXJSaveLogPacket::ID)
{
}

crJXJSaveLogPacket::crJXJSaveLogPacket( const crJXJSaveLogPacket &p ):
	crStreamPacket(crJXJSaveLogPacket::ID,p)
{
}
void crJXJSaveLogPacket::buildRequestPacket(crJXJSaveLogPacket &packet,int playerid,unsigned short logtype,const std::string &logdata)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(10+logdata.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeUShort(logtype);
	streamBuf->_writeString(logdata.c_str());
}
void crJXJSaveLogPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == DBServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crDBServerCallback *dbcallback = dynamic_cast<crDBServerCallback *>(netDataManager->getNetCallback());
		if(dbcallback)
		{
			int playerid = m_streamBuf->_readInt();
			unsigned short logtype = m_streamBuf->_readUShort();
			std::string logdata = m_streamBuf->_readString();
			dbcallback->saveGameLog(sender,playerid,logtype,logdata);
		}
	}
	else if(netType == GameServer)
	{
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		dbConductor->getNetManager()->sendPacket("all",*this);
	}
}
/////////////////////////////////////////
//
//crJXJSendPlayerDataPacket
//
/////////////////////////////////////////
crJXJSendPlayerDataPacket::crJXJSendPlayerDataPacket():
	crStreamPacket(crJXJSendPlayerDataPacket::ID)
{
}
crJXJSendPlayerDataPacket::crJXJSendPlayerDataPacket( const crJXJSendPlayerDataPacket &p ):
	crStreamPacket(crJXJSendPlayerDataPacket::ID,p)
{
}
void crJXJSendPlayerDataPacket::buildReplyPacket(crJXJSendPlayerDataPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getBuf(),stream->getBufSize());
}
void crJXJSendPlayerDataPacket::buildReplyPacket2(crJXJSendPlayerDataPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJSendPlayerDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJSendPlayerDataPacket packet;
			crJXJSendPlayerDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			void *param;
			crData *braindata = crBrain::getInstance()->getDataClass();
			braindata->getParam(WCHDATA_JXJCheckPlayDataTable,param);
			ref_ptr<crTableIO>table = (crTableIO *)param;
			
			typedef std::vector<int> TechLvVec;
			typedef std::vector<int> BuildingLvVec;
			int _playerid = 0;
			std::string _account;//帐号
			std::string _nickname;//昵称
			std::string _createaccount;//注册时间
			std::string _createdate;//创建角色时间
			std::string _logindate;//最后登陆时间
			std::string _onlinetime;//在线时长
			std::string _email;//Email地址
			unsigned short _serieslanddays = 0;//连续登录天数
			unsigned char _shiliid = 0;//势力
			int _gold = 0;//元宝
			int _lijin = 0;//礼金
			unsigned char _buildingtimebarcount = 0;//建筑时间条数
			unsigned char _recuittimebarcount = 0;//招募时间条
			unsigned char _lordlv = 0;//领主等级
			unsigned char _govpost = 0;//官职
			int _groupid = 0;//军团ID
			int _exploit = 0;//功勋
			unsigned short _junling = 0;//军令
			int _achieve = 0;//战功
			int _countrycontri = 0;//国家贡献
			int _copper = 0;//铜钱
			int _food = 0;//粮食
			int _wood = 0;//木材
			int _iron = 0;//铁矿
			int _horse = 0;//马匹
			TechLvVec _techlvvec;//科技
			BuildingLvVec _buildinglvvec;//建筑等级
			FubenProgressMap _fubenprogressmap;//副本map
			JXJItemBagVec _itembagvec;//背包
			JXJItemBagVec2 _lordequipvec;//领主装备
			JXJItemBagVec _jiangkabagvec;//将卡背包
			//ref_ptr<crTableIO> table = new crTableIO;
			//table->openFileNoCook("玩家信息列表.tab");
			crTableIO::StrVec strVec;
			crTableIO::StrVec strVec2;
			strVec.resize(42);
			strVec2.resize(42);
			int index = 0;
			if (!table)
			{
				return;
				//table = new crTableIO;
				//braindata->getParam(WCHDATA_JXJCheckPlayDataTable,param);
				//ref_ptr<crTableIO>table = (ref_ptr<crTableIO>)param;
			}
			if(!table->getRowCount())
			{
				strVec[index++] = "playerid";
				strVec[index++] = "帐号id";
				strVec[index++] = "角色名称";
				strVec[index++] = "注册时间";
				strVec[index++] = "角色创建时间";
				strVec[index++] = "最后一次上线时间";
				strVec[index++] = "在线时长";
				strVec[index++] = "邮件地址";
				strVec[index++] = "连续登录天数";
				strVec[index++] = "势力ID";
				strVec[index++] = "元宝";
				strVec[index++] = "礼金";
				strVec[index++] = "建筑时间条数量";
				strVec[index++] = "招兵时间条数量";
				strVec[index++] = "等级";
				strVec[index++] = "官职";
				strVec[index++] = "所属军团";
				strVec[index++] = "功勋";
				strVec[index++] = "军令";
				strVec[index++] = "战功";
				strVec[index++] = "国家贡献";
				strVec[index++] = "铜钱";
				strVec[index++] = "粮草";
				strVec[index++] = "木材";
				strVec[index++] = "铁矿";
				strVec[index++] = "马匹";
				strVec[index++] = "科技等级";
				strVec[index++] = "各建筑等级";
				strVec[index++] = "最后一个通关副本和星级";
				strVec[index++] = "拥有的物品";
				strVec[index++] = "将卡";
				strVec[index++] = "成就";
				strVec[index++] = "所有部队";
				strVec[index++] = "三组编队";
				table->setTitleVec(strVec);
			}
			char tmp[64];
			//int mapsize = m_stream->_readInt();
//			unsigned char ifremain;
			time_t t = time(0);
			strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
			std::string servername;
			int playercount = 0;
			//for (int i = 0; i < mapsize; ++i)
			{
				//ifremain = m_streamBuf->_readUChar();
				_playerid = m_streamBuf->_readInt();
				if (_playerid > 0)
				{
					_account = m_streamBuf->_readString();//帐号
					_nickname = m_streamBuf->_readString();//昵称
					_createaccount = m_streamBuf->_readString();//注册时间
					_createdate = m_streamBuf->_readString();//创建角色时间
					_logindate = m_streamBuf->_readString();//最后登陆时间
					_onlinetime = m_streamBuf->_readString();//在线时长
					_email = m_streamBuf->_readString();//Email
					_serieslanddays = m_streamBuf->_readUShort();//连续登录天数
					_shiliid = m_streamBuf->_readUChar();//势力ID
					_gold = m_streamBuf->_readInt();//元宝
					_lijin = m_streamBuf->_readInt();//礼金
					_buildingtimebarcount = m_streamBuf->_readUChar();//建筑时间条数
					_recuittimebarcount = m_streamBuf->_readUChar();//招募时间条
					_lordlv = m_streamBuf->_readUChar();//领主等级
					_govpost = m_streamBuf->_readUChar();//官职
					_groupid = m_streamBuf->_readInt();//军团ID
					_exploit = m_streamBuf->_readInt();//功勋
					_junling = m_streamBuf->_readUShort();//军令
					_achieve = m_streamBuf->_readInt();//战功
					_countrycontri = m_streamBuf->_readInt();//国家贡献
					_copper = m_streamBuf->_readInt();//铜钱
					_food = m_streamBuf->_readInt();//粮食
					_wood = m_streamBuf->_readInt();//木材
					_iron = m_streamBuf->_readInt();//铁矿
					_horse = m_streamBuf->_readInt();//马匹
					_techlvvec;//科技
					_buildinglvvec;//建筑等级
					_fubenprogressmap;//副本map
					_itembagvec;//背包
					_lordequipvec;//领主装备
					_jiangkabagvec;//将卡背包
					index = 0;
					strVec2[index++] = crArgumentParser::appItoa(_playerid);
					strVec2[index++] = _account;
					strVec2[index++] = _nickname;
					strVec2[index++] = _createaccount;
					strVec2[index++] = _createdate;
					strVec2[index++] = _logindate;
					strVec2[index++] = _onlinetime;
					strVec2[index++] = _email;
					strVec2[index++] = crArgumentParser::appItoa(_serieslanddays);
					strVec2[index++] = crArgumentParser::appItoa(_shiliid);
					strVec2[index++] = crArgumentParser::appItoa(_gold);
					strVec2[index++] = crArgumentParser::appItoa(_lijin);
					strVec2[index++] = crArgumentParser::appItoa(_buildingtimebarcount);
					strVec2[index++] = crArgumentParser::appItoa(_recuittimebarcount);
					strVec2[index++] = crArgumentParser::appItoa(_lordlv);
					strVec2[index++] = crArgumentParser::appItoa(_govpost);
					strVec2[index++] = crArgumentParser::appItoa(_groupid);
					strVec2[index++] = crArgumentParser::appItoa(_exploit);
					strVec2[index++] = crArgumentParser::appItoa(_junling);
					strVec2[index++] = crArgumentParser::appItoa(_achieve);
					strVec2[index++] = crArgumentParser::appItoa(_countrycontri);
					strVec2[index++] = crArgumentParser::appItoa(_copper);
					strVec2[index++] = crArgumentParser::appItoa(_food);
					strVec2[index++] = crArgumentParser::appItoa(_wood);
					strVec2[index++] = crArgumentParser::appItoa(_iron);
					strVec2[index++] = crArgumentParser::appItoa(_horse);
					table->addData(strVec2);
				}
			}
			//if (!ifremain/* || table->getRowCount() % 100 == 0*/)
			//{
			//	std::string tabletime = tmp;
			//	table->saveToFileNoCook("玩家信息列表.tab");
			//}
		}
		//crNetConductor *mainConductor = crNetContainer::getInstance()->getNetConductor(crNetContainer::getInstance()->getMainNetType());
		//if(!m_init)
		//{
		//	m_filename = mainConductor->getKeyStr() + ".tab";
		//	crTableIO::StrVec titleVec;
		//	titleVec.push_back("时间");
		//	titleVec.push_back("在线玩家数量");
		//	titleVec.push_back("CPU使用率");
		//	titleVec.push_back("房间数量");
		//	m_columncount = titleVec.size();
		//	if(!CRIOManager::fileExists(m_filename))
		//	{
		//		std::ofstream fout(m_filename.c_str(), std::ios::out);
		//		char tab = 9;
		//		char n = '\n';
		//		int i;
		//		for(i = 0; i<m_columncount-1; ++i)
		//		{
		//			fout.write(titleVec[i].c_str(),titleVec[i].length());
		//			fout.write(&tab,1);
		//		}
		//		fout.write(titleVec[i].c_str(),titleVec[i].length());
		//		fout.write(&n,1);
		//		fout.close();
		//	}
		//	m_serverInfoData.reserve(m_writeCount);
		//	m_init = true;
		//}
		//int playercount = mainConductor->getNetDataManager()->getPlayerCount();
		//if(playercount>=m_minCount)
		//{
		//	time_t t = time( 0 ); 
		//	char tmp[20];b 
		//	strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
		//	crTableIO::StrVec record;
		//	record.resize(m_columncount);
		//	record[0] = tmp;
		//	record[1] = crArgumentParser::appItoa(playercount);
		//	record[2] = crArgumentParser::appItoa(get_cpu_usage());
		//	crNetDataManager *netDataManager = mainConductor->getNetDataManager();
		//	crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		//	if(netCallback)
		//		record[3] = crArgumentParser::appItoa(netCallback->getRoomCount());
		//	m_serverInfoData.push_back(record);
		//	int infosize = m_serverInfoData.size();
		//	if(infosize>=m_writeCount)
		//	{
		//		std::ofstream fout(m_filename.c_str(), std::ios::binary|std::ios::app);
		//		if(fout)
		//		{
		//			char tab = 9;
		//			char n = '\n';
		//			int i,j;
		//			for(i = 0; i<infosize;++i)
		//			{
		//				for(j=0; j<m_columncount-1; ++j)
		//				{
		//					fout.write(m_serverInfoData[i][j].c_str(),m_serverInfoData[i][j].length());
		//					fout.write(&tab,1); 
		//				}
		//				fout.write(m_serverInfoData[i][j].c_str(),m_serverInfoData[i][j].length());
		//				fout.write(&n,1);
		//			}
		//			fout.close();
		//		}
		//		m_serverInfoData.resize(0);
		//	}
		//	//char strbuf[64];
		//	//sprintf(strbuf," 在线玩家数量：%d  CPU使用率: %d\0",playercount,get_cpu_usage());
		//	//std::string str = tmp;
		//	//str += strbuf;
		//	//m_serverInfo.push_back(str);
		//	//if(m_serverInfo.size()>=m_writeCount)
		//	//{
		//	//	std::ofstream fout(m_filename.c_str(), std::ios::binary|std::ios::app);
		//	//	if(fout)
		//	//	{
		//	//		for( ServerInfoVec::iterator itr = m_serverInfo.begin();
		//	//			itr != m_serverInfo.end();
		//	//			++itr )
		//	//		{
		//	//			fout<<*itr<<std::endl;
		//	//		}
		//	//		fout.close();
		//	//	}
		//	//	m_serverInfo.resize(0);
		//	//}
		//}
	}
}
/////////////////////////////////////////
//
//crJXJEndSendPlayerDataPacket
//
/////////////////////////////////////////
crJXJEndSendPlayerDataPacket::crJXJEndSendPlayerDataPacket():
	crStreamPacket(crJXJEndSendPlayerDataPacket::ID)
{
}
crJXJEndSendPlayerDataPacket::crJXJEndSendPlayerDataPacket( const crJXJEndSendPlayerDataPacket &p ):
	crStreamPacket(crJXJEndSendPlayerDataPacket::ID,p)
{
}
void crJXJEndSendPlayerDataPacket::buildReplyPacket(crJXJEndSendPlayerDataPacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if (stream)
	{
		streamBuf->createBuf(4+stream->getBufSize());
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeInt(playerid);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->createBuf(4);
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeInt(playerid);
	}
}
void crJXJEndSendPlayerDataPacket::buildReplyPacket2(crJXJEndSendPlayerDataPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJEndSendPlayerDataPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJEndSendPlayerDataPacket packet;
			crJXJEndSendPlayerDataPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *gameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(gameData)
		{
			void *param;
			crData *braindata = crBrain::getInstance()->getDataClass();
			braindata->getParam(WCHDATA_JXJCheckPlayDataTable,param);
			ref_ptr<crTableIO>table = (crTableIO *)param;
			if (table.valid())
			{
				table->saveToFileNoCook("玩家信息列表.tab");
				unsigned char endsend = 1;
				braindata->inputParam(WCHDATA_JXJIFEndSendPlayerData,&endsend);
				//braindata->inputParam(WCHDATA_JXJCheckPlayDataTable,NULL);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRandomZhenYingTellClientIdPacket
//
/////////////////////////////////////////
crJXJRandomZhenYingTellClientIdPacket::crJXJRandomZhenYingTellClientIdPacket():
	crStreamPacket(crJXJRandomZhenYingTellClientIdPacket::ID)
{
}
crJXJRandomZhenYingTellClientIdPacket::crJXJRandomZhenYingTellClientIdPacket( const crJXJRandomZhenYingTellClientIdPacket &p ):
	crStreamPacket(crJXJRandomZhenYingTellClientIdPacket::ID,p)
{
}
void crJXJRandomZhenYingTellClientIdPacket::buildReplyPacket(crJXJRandomZhenYingTellClientIdPacket &packet, int playerid,unsigned char shiliId, unsigned short chengchiId, unsigned short homeId, int giftGold)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(13);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeUChar(shiliId);
	streamBuf->_writeUShort(homeId);
	streamBuf->_writeUShort(chengchiId);
	streamBuf->_writeInt(giftGold);
}
void crJXJRandomZhenYingTellClientIdPacket::buildReplyPacket2(crJXJRandomZhenYingTellClientIdPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJRandomZhenYingTellClientIdPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJRandomZhenYingTellClientIdPacket packet;
			crJXJRandomZhenYingTellClientIdPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			//这边读取出来，设置到游戏中去
			unsigned char shiliId = m_streamBuf->_readUChar();
			unsigned short homeId = m_streamBuf->_readUShort();
			unsigned short chengchiId = m_streamBuf->_readUShort();
			int giftCount = m_streamBuf->_readInt();
			playerData->inputParam(WCHDATA_JXJShiliID, &shiliId);
			playerData->inputParam(WCHDATA_JXJHomeID, &homeId);
			playerData->inputParam(WCHDATA_JXJChengchiID, &chengchiId);
			playerData->inputParam(WCHDATA_JXJGiftGold, &giftCount);
			//sceneid 确定
			ref_ptr<crTableIO>homeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHomeTab);
			crTableIO::StrVec record;
			if(homeTab->queryOneRecord(0,crArgumentParser::appItoa(homeId),record)>=0)
			{
				crData *runGameData = crRunGameHandle::getInstance()->getDataClass();
				int sceneid = atoi(record[homeTab->getTitleIndex("sceneid")].c_str());
				runGameData->inputParam(WCHDATA_SceneID,&sceneid);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJQueryLoseCountryPacket
//
/////////////////////////////////////////
crJXJQueryLoseCountryPacket::crJXJQueryLoseCountryPacket():
	crStreamPacket(crJXJQueryLoseCountryPacket::ID)
{
}
crJXJQueryLoseCountryPacket::crJXJQueryLoseCountryPacket( const crJXJQueryLoseCountryPacket &p ):
	crStreamPacket(crJXJQueryLoseCountryPacket::ID,p)
{
}
void crJXJQueryLoseCountryPacket::buildRequestPacket(crJXJQueryLoseCountryPacket &packet)
{
	//crStreamBuf *streamBuf = new crStreamBuf;
	//streamBuf->createBuf(1);
	//packet.setStreamBuf(streamBuf);
	//streamBuf->_writeUChar(sex);
}
void crJXJQueryLoseCountryPacket::buildRequestPacket2(crJXJQueryLoseCountryPacket &packet,int playerid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
}
void crJXJQueryLoseCountryPacket::buildReplyPacket(crJXJQueryLoseCountryPacket &packet,int playerid,CRCore::crVector2ub& loseshiliid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(6);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->_writeVec2ub(loseshiliid);
}
void crJXJQueryLoseCountryPacket::buildReplyPacket2(crJXJQueryLoseCountryPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJQueryLoseCountryPacket::record(const std::string &servername)
{
	static GNE::Mutex mutex;
	GNE::LockMutex lock( mutex );
	std::string filename = "../JXJDB/" + servername + "/SelectCharacter.tab";
	ref_ptr<crTableIO> tab = new crTableIO;
	bool found = tab->openFileNoCook(filename);
	time_t t = time( 0 );
	char tmp[20];//20150520
	strftime( tmp, sizeof(tmp), "%Y%m%d\0", localtime(&t) );
	crTableIO::StrVec record;
	if(!found)
	{
		CREncapsulation::crTableIO::StrVec title;
		title.push_back("角色选择界面人数");
		title.push_back("日期");
		tab->setTitleVec(title);
		record.push_back("1");
		record.push_back(tmp);
		tab->addData(record);
	}
	else
	{
		int count = 1;
		int row = tab->queryOneRecord(1,tmp,record);
		if(row>=0)
		{
			count = atoi(record[0].c_str()) + 1;
			(*tab)(row,0) = crArgumentParser::appItoa(count);
		}
		else
		{
			record.push_back("1");
			record.push_back(tmp);
			tab->addData(record);
		}
	}
	tab->saveToFileNoCook(filename);
}
void crJXJQueryLoseCountryPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crJXJQueryLoseCountryPacket packet;
				crJXJQueryLoseCountryPacket::buildRequestPacket2(packet,playerData->getPlayerID());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryBattleTimerPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			unsigned char shiliid = 0;
			//unsigned char loseShiliid = 0;
			crVector2ub loseshiliids;
			int j=0;
			bool isLoseCountry = false;
			for (int i = 0; i < 3; ++i)
			{
				shiliid = c_startShiliID + i;
				isLoseCountry = false;
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
				if (isLoseCountry)
				{
					loseshiliids[j++] = shiliid;
				}
			}
			crJXJQueryLoseCountryPacket packet;
			crJXJQueryLoseCountryPacket::buildReplyPacket(packet,playerid,loseshiliids);
			m_netConductor->getNetManager()->sendPacket(netPlayerData->getPlayerConnectServerAddress(),packet);
		}
		////加统计进入到角色选择页面的人数
		//CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());
		//record(netCallback->getServerName());
		GameLogData gamelog(Log_SelectCharacter,"选择角色");
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJQueryLoseCountryPacket packet;
			crJXJQueryLoseCountryPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crVector2ub loseshiliid = m_streamBuf->_readVec2ub();
		crData *brainData = crBrain::getInstance()->getDataClass();
		brainData->inputParam(WCHDATA_JXJQueryLoseShiliID,&loseshiliid);
		ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_SelectCharacter");
		if (canvasNode.valid())
		{
			canvasNode->doEvent(WCH_JXJSelectCharacterPowerOnShow);
		}
	}
}


/////////////////////////////////////////
//
//crJXJPingPacket
//
/////////////////////////////////////////
crJXJPingPacket::crJXJPingPacket():
	crStreamPacket(crJXJPingPacket::ID)
{
}
crJXJPingPacket::crJXJPingPacket( const crJXJPingPacket &p ):
	crStreamPacket(crJXJPingPacket::ID,p)
{
}
void crJXJPingPacket::buildRequestPacket(crJXJPingPacket &packet)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt64(CRCore::Timer::instance()->tick());
}
void crJXJPingPacket::buildReplyPacket(crJXJPingPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(JXJPingType_atuo);
	streamBuf->write(stream->getPtr(),stream->getBufSize());
}


void crJXJPingPacket::buildReplyPacket2(crJXJPingPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getBufSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeUChar(JXJPingType_man);
	streamBuf->write(stream->getPtr(),stream->getBufSize());
}

void crJXJPingPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if (playerData.valid())
		{
			crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
			crJXJPingPacket packet;
			crJXJPingPacket::buildReplyPacket(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		unsigned char pingType = m_streamBuf->_readUChar();
		if (pingType == JXJPingType_atuo)
		{
			crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJRecvPingRet,MAKEINT64(m_streamBuf.get(),GameClient_Game));
		}
		else if (pingType == JXJPingType_man)
		{
			crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJRecvPingMsg,MAKEINT64(m_streamBuf.get(),GameClient_Game));
		}
	}	
}

/////////////////////////////////////////
//
//crJXJLoginPacket
//
/////////////////////////////////////////
crJXJLoginPacket::crJXJLoginPacket():
	crStreamPacket(crJXJLoginPacket::ID)
{
}

crJXJLoginPacket::crJXJLoginPacket( const crJXJLoginPacket &p ):
	crStreamPacket(crJXJLoginPacket::ID,p)
{
}

void crJXJLoginPacket::buildRequestPacket(crJXJLoginPacket &packet,std::string &userName,std::string &userPassword)
{//Client->ConnectServer ConnectServerClient_Login->LoginServer
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(256);
	packet.setStreamBuf(streamBuf);
	//crJXJLoginPacket::s_success = 0;
	//if(userName.size()>20)
	//	userName = userName.substr(0,20);
	streamBuf->_writeString(userName);//name---最大20+4
	//if(userPassword.size()>32)
	//	userPassword = userPassword.substr(0,32);
	streamBuf->_writeString(userPassword);//name---最大20+4
}

void crJXJLoginPacket::buildRequestPacket2(crJXJLoginPacket &packet,const std::string &address,CRCore::crStreamBuf *stream)
{//Client->ConnectServer ConnectServerClient_Login->LoginServer
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crJXJLoginPacket::buildReplyPacket(crJXJLoginPacket &packet,const std::string &address,char loginCode,int playerid,int lastserverid,const std::string &key)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(address.length()+key.length()+17);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->_writeChar(loginCode);
	if(loginCode>0)
	{
		streamBuf->_writeInt(playerid);
		streamBuf->_writeInt(lastserverid);
		streamBuf->_writeString(key);
	}
}
void crJXJLoginPacket::buildReplyPacket2(crJXJLoginPacket &packet,CRCore::crStreamBuf *stream)
{//LoginServer->ConnectServerClient_Login
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void crJXJLoginPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到LoginServer
		crJXJLoginPacket packet;
		crJXJLoginPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//LoginServer发回到LoginConnectServer
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		int playerid = 0;
		int lastserverid = 0;
		unsigned char locked = 0;
		std::string userName,userPassword,playerAddress,key;
		char loginCode = 0;//0登录失败，用户名密码不正确，1登录成功,2帐号被锁定
		playerAddress = m_streamBuf->_readString();
		userName = m_streamBuf->_readString();
		//if(userName.size()>16)
		//	userName = userName.substr(0,16);
		userPassword = m_streamBuf->_readString();
		//if(userPassword.size()>16)
		//	userPassword = userPassword.substr(0,16);

		//数据库检索登陆账户，如果密码正确，则返回账户信息
		//ref_ptr<crAccountData> accountData;
		crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
		ref_ptr<crDataBase> accountSession = accountdb->beginSession();

		ref_ptr<crJXJLoginQueryData> loginQueryData = new crJXJLoginQueryData;
		loginQueryData->buildQuerySql(userName,userPassword);

		accountSession->executeQuery(loginQueryData.get());
		crDataBase::QueryResultVec &loginQueryResultVec = accountSession->getQueryResult();
		if(!loginQueryResultVec.empty())
		{
			loginQueryData = dynamic_cast<crJXJLoginQueryData *>(loginQueryResultVec[0].get());
			playerid = loginQueryData->getPlayerID();
			lastserverid = loginQueryData->getLastServerID();
			locked = loginQueryData->getLocked();
			switch (locked)
			{
			case 0:
				loginCode = 1;
				break;
			case 1:
				loginCode = 2;
				break;
			case 2:
				loginCode = 3;
				break;
			case 3:
				loginCode = 4;
				break;
			}
		}
		accountSession->releaseQuery();

		if(loginCode==1)
		{//登陆成功
			key = CRNet::gainCode();//CRNet::gainKey(/*basecode*/);
			std::string::size_type i = playerAddress.find_last_of(':');
			std::string ip = std::string(playerAddress.begin(),playerAddress.begin()+i+1);

			ref_ptr<crJXJUpdateLoginKeyDate> updateLoginDate = new crJXJUpdateLoginKeyDate;
			updateLoginDate->buildUpdateSql(loginQueryData->getPlayerID(),key,ip);
			if(accountSession->executeUpdate(updateLoginDate.get()))
			{//插入成功
				accountSession->commit();
			}
		}
		accountdb->endSession(accountSession.get());
		crJXJLoginPacket packet;
		crJXJLoginPacket::buildReplyPacket(packet,playerAddress,loginCode,playerid,lastserverid,/*userName,*/key);
		netManager->sendPacket(sender,packet);
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginPacket "<<userName<<" "<<(int)loginCode<<std::endl;
	}
	else if(netType == LoginConnectServerClient_Login)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crJXJLoginPacket packet;
			crJXJLoginPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginPacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{
		char loginCode = m_streamBuf->_readChar();
		if(loginCode==1)
		{//登陆成功，返回账户信息。
			crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
			myPlayerData->setPlayerID(m_streamBuf->_readInt());
			myPlayerData->setLastServerID(m_streamBuf->_readInt());
			myPlayerData->setKey(m_streamBuf->_readString());

		}
		crGlobalHandle::getInstance()->doEvent(WCH_LoginReturn,MAKEINT64(loginCode,NULL));
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginPacket "<<loginCode<<std::endl;
	}
}
/////////////////////////////////////////
//
//crJXJRegisterAccountPacket
//
/////////////////////////////////////////
crJXJRegisterAccountPacket::crJXJRegisterAccountPacket():
	crStreamPacket(crJXJRegisterAccountPacket::ID)
{
}

crJXJRegisterAccountPacket::crJXJRegisterAccountPacket( const crJXJRegisterAccountPacket &p ):
	crStreamPacket(crJXJRegisterAccountPacket::ID,p)
{
}

void crJXJRegisterAccountPacket::buildRequestPacket(crJXJRegisterAccountPacket &packet, std::string &name, std::string &password, std::string &email, std::string &nickname, std::string &relname, char sex,std::string &code)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(256);
	packet.setStreamBuf(streamBuf);
	//crJXJRegisterAccountPacket::s_success = 0;
	//if(name.size()>16)
	//	name = name.substr(0,16);
	streamBuf->_writeString(name);
	//if(password.size()>16)
	//	password = password.substr(0,16);
	streamBuf->_writeString(password);
	//if(email.size()>20)
	//	email = email.substr(0,20);
	streamBuf->_writeString(email);
	//if(nickname.size()>16)
	//	nickname = nickname.substr(0,16);
	streamBuf->_writeString(nickname);
	//if(relname.size()>16)
	//	relname = relname.substr(0,16);
	streamBuf->_writeString(relname);
	streamBuf->_writeChar(sex);//0女性，1男性
	if(!code.empty())
	{
		//if(code.size()>10)
		//	code = code.substr(0,10);
		streamBuf->_writeString(code);
	}
}

void crJXJRegisterAccountPacket::buildRequestPacket2(crJXJRegisterAccountPacket &packet, const std::string &address, crStreamBuf *buf)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(buf->getRemainSize() + address.length()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);
	streamBuf->write(buf->getPtr(),buf->getRemainSize());
}

void crJXJRegisterAccountPacket::buildReplyPacket(crJXJRegisterAccountPacket &packet,const std::string &address,char ret,const std::string &validdate)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	if(ret == -11)
	{
		streamBuf->createBuf(address.length()+9+validdate.length());
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(address);
		streamBuf->_writeChar(ret);//1
		streamBuf->_writeString(validdate);
	}
	else
	{
		streamBuf->createBuf(address.length()+5);
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(address);
		streamBuf->_writeChar(ret);//1
	}
}

void crJXJRegisterAccountPacket::buildReplyPacket2(crJXJRegisterAccountPacket &packet,CRCore::crStreamBuf* buf)
{//CenterServer->WaiterConnectServerClient_Center
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(buf->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(buf->getPtr(),buf->getRemainSize());
}

void crJXJRegisterAccountPacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(connectServer)
		{
			crJXJRegisterAccountPacket packet;
			crJXJRegisterAccountPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket("all",packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJRegisterAccountPacket "<<std::endl;
		}
	}
	else if(netType == LoginServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		std::string address = m_streamBuf->_readString();
		std::string name = m_streamBuf->_readString();
		std::string validdate;
		char ret = 0;
		do 
		{
			if(name.empty()/* || name.find("GM")!=std::string::npos || name.find("gm")!=std::string::npos || name.find("Gm")!=std::string::npos || name.find("gM")!=std::string::npos*/)
			{
				ret = -1;//帐号名格式不正确
				break;
			}
			if(name.size()>20)
			{
				ret = -2;//帐号名字符数量超限
				break;
			}
			std::string password = m_streamBuf->_readString();
			if(password.empty() || password.size()>32)
			{
				ret = -3;//密码不正确
				break;
			}
			std::string email = m_streamBuf->_readString();
			if(email.size()>50)
			{
				ret = -4;//邮箱地址字符数量超限
				break;
			}
			std::string nickname = m_streamBuf->_readString();
			if(nickname.size()>20)
			{
				ret = -5;//昵称字符数量超限
				break;
			}
			std::string relname = m_streamBuf->_readString();
			if(relname.size()>20)
			{
				ret = -6;//真实名字字符数量超限
				break;
			}

			char sex = m_streamBuf->_readChar();
			if(!m_streamBuf->eof())
			{
				std::string code = m_streamBuf->_readString();
				if(!code.empty())
				{
					int id = 0;
					bool used = true;
					crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
					ref_ptr<crDataBase> globalSession = db->beginSession();

					ref_ptr<crJXJActivecodeQueryData> activeCodeQuery = new crJXJActivecodeQueryData;
					activeCodeQuery->buildQuerySql(code);
					globalSession->executeQuery(activeCodeQuery.get());
					crDataBase::QueryResultVec& activeCodeQueryResultVec = globalSession->getQueryResult();
					if(!activeCodeQueryResultVec.empty())
					{
						activeCodeQuery = dynamic_cast<crJXJActivecodeQueryData *>(activeCodeQueryResultVec[0].get());
						id = activeCodeQuery->getID();
						used = activeCodeQuery->getUsed();
						validdate = activeCodeQuery->getValidDate();
					}
					globalSession->releaseQuery();
					db->endSession(globalSession.get());
					if(id == 0)
					{
						ret = -7;//激活码输入错误
					}
					else if(used)
					{
						ret = -8;//该激活码已经失效
					}
					else
					{
						if(!validdate.empty())
						{
							time_t t = time( 0 ); 
							char tmp[20];
							strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0", localtime(&t) );
							if(strcmp(tmp,validdate.c_str())<0)
							{
								ret = -11;//未到激活码有效期
							}
						}
						if(ret == 0)
						{
							crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
							ref_ptr<crDataBase> accountSession = accountdb->beginSession();
							ref_ptr<crJXJInsertAccount> insertAccount = new crJXJInsertAccount;
							insertAccount->buildUpdateSql(name,password,0,email,nickname,relname,sex);
							if(accountSession->executeUpdate(insertAccount.get()))
							{//插入成功
								accountdb->endSession(accountSession.get());
								globalSession = db->beginSession();
								ref_ptr<crJXJUpdateActiveCode> updateActiveCode = new crJXJUpdateActiveCode;
								updateActiveCode->buildUpdateSql(id);
								globalSession->executeUpdate(updateActiveCode.get());
								globalSession->commit();
								db->endSession(globalSession.get());
							}
							else
							{
								accountdb->endSession(accountSession.get());
								ret = -9;//帐号冲突
							}
						}
					}
				}
				else
				{
					ret = -10;//激活码不能为空
				}
			}
			else
			{
				ref_ptr<crJXJInsertAccount> insertAccount = new crJXJInsertAccount;
				insertAccount->buildUpdateSql(name,password,0,email,nickname,relname,sex);
				crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
				ref_ptr<crDataBase> accountSession = accountdb->beginSession();
				if(accountSession->executeUpdate(insertAccount.get()))
				{//插入成功
					accountSession->commit();
				}
				else
				{
					ret = -9;//帐号冲突
				}
				accountdb->endSession(accountSession.get());
			}
		} while (0);
		crJXJRegisterAccountPacket packet;
		crJXJRegisterAccountPacket::buildReplyPacket(packet,address,ret,validdate);
		netManager->sendPacket(sender,packet);
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJRegisterAccountPacket SendTo ConnectServer "<<std::endl;
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crJXJRegisterAccountPacket packet;
			crJXJRegisterAccountPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJRegisterAccountPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJRegisterAccountPacket "<<std::endl;
		//bool sucess = m_streamBuf->_readBool();
		//s_success = sucess?1:-1;
		crGlobalHandle::getInstance()->doEvent(WCH_RegisterAccountReturn,MAKEINT64(m_streamBuf.get(),NULL));
	}
}
/////////////////////////////////////////
//
//crJXJLoginGamePacket
//
/////////////////////////////////////////
crJXJLoginGamePacket::crJXJLoginGamePacket():
	crStreamPacket(crJXJLoginGamePacket::ID)
{
}

crJXJLoginGamePacket::crJXJLoginGamePacket( const crJXJLoginGamePacket &p ):
	crStreamPacket(crJXJLoginGamePacket::ID,p)
{
}

void crJXJLoginGamePacket::buildRequestPacket(crJXJLoginGamePacket &packet)
{
	//crQueryGameListData *gameData = crMyPlayerData::getInstance()->getSelectedGame();
	//   if(gameData)
	//{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(68);
	packet.setStreamBuf(streamBuf);
	crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
	streamBuf->_writeInt(myPlayerData->getPlayerID());
	//streamBuf->_writeString(myPlayerData->getPlayerName());
	streamBuf->_writeString(myPlayerData->getKey());
	streamBuf->_writeInt(myPlayerData->getLastServerID());
	streamBuf->_writeInt(crGlobalHandle::getVersion());
	//}
}

void crJXJLoginGamePacket::buildRequestPacket2(crJXJLoginGamePacket &packet,const std::string &sender,CRCore::crStreamBuf *stream)
{
	if(stream)
	{
		crStreamBuf *streamBuf = new crStreamBuf;
		streamBuf->createBuf(sender.length()+4+stream->getRemainSize());
		packet.setStreamBuf(streamBuf);
		streamBuf->_writeString(sender);
		streamBuf->write(stream->getPtr(),stream->getRemainSize());
	}
}

void crJXJLoginGamePacket::buildReplyPacket(crJXJLoginGamePacket &packet,const std::string &address,char loginCode,int webserverid,const std::string &site,const std::string &serverName,crGameServerPlayerData *playerData)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	ref_ptr<crStreamBuf> stream;
	//std::string playerGameName;
	crPlayerGameData *gameData = NULL;
	if(playerData)
	{
		gameData = playerData->getPlayerGameData();
		if(gameData && gameData->getDataClass())
		{
			//playerGameName = gameData->getName();
			crData *data = gameData->getDataClass();
			stream = new crStreamBuf;
			data->excHandle(MAKEINT64(WCH_BuildPlayerStream,stream.get()));
			//void *param;
			//data->getParam(WCHDATA_DataStream,param);
			//if(param)
			//{
			//	stream = *((ref_ptr<crStreamBuf>*)param);
			//}
		}
	}
	if(loginCode==1)
	{
		streamBuf->createBuf(128 + (stream.valid()?stream->getBufSize():0)/* + playerGameName.length()*/);
	}
	else
	{
		streamBuf->createBuf(26);
	}
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(address);//25
	streamBuf->_writeChar(loginCode);//1
	if(loginCode==1)
	{
		streamBuf->_writeInt(playerData->getPlayerID());//4
		streamBuf->_writeString(playerData->getUserName());//50
		streamBuf->_writeInt(webserverid);//4
		streamBuf->_writeString(site);//24
		streamBuf->_writeString(serverName);//24
		streamBuf->_writeBool(gameData!=NULL);
		if(gameData)
		{
			//streamBuf->_writeString(playerGameName);
			streamBuf->_writeBool(gameData->getDBValid());
			streamBuf->_writeInt(gameData->getMainRoleID());
			if(stream.valid())
			{
				streamBuf->write(stream->getBuf(),stream->getBufSize());
			}
		}
	}
}

void crJXJLoginGamePacket::buildReplyPacket2(crJXJLoginGamePacket &packet,char loginCode,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(1+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeChar(loginCode);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJLoginGamePacket::moneyAndVipQuery(int playerid, int serverid,CRCore::crData *data/*,CRDataBase::crDataBase* globalSession*/)
{
	int money = 0;
	unsigned char viplv = 0;
	int vipexp = 0;
	ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
	playerMoneyQuery->buildQuerySql(playerid,serverid);

	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	globalSession->executeQuery(playerMoneyQuery.get());
	crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
	if(!playerMoneyQueryResultVec.empty())
	{
		playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
		money = playerMoneyQuery->getMoney();
	}
	globalSession->releaseQuery();

	ref_ptr<crPlayerVipQueryData> playerVipQuery = new crPlayerVipQueryData;
	playerVipQuery->buildQuerySql(playerid,serverid);//要设置为区ID

	globalSession->executeQuery(playerVipQuery.get());
	crDataBase::QueryResultVec &playerVipQueryResultVec = globalSession->getQueryResult();
	if(!playerVipQueryResultVec.empty())
	{
		playerVipQuery = dynamic_cast<crPlayerVipQueryData *>(playerVipQueryResultVec[0].get());
		viplv = playerVipQuery->getViplv();
		vipexp = playerVipQuery->getVipexp();
	}
	globalSession->releaseQuery();
	db->endSession(globalSession.get());

	data->inputParam(WCHDATA_Money,&money);
	data->inputParam(WCHDATA_VipLv,&viplv);
	data->inputParam(WCHDATA_VipExp,&vipexp);
}
void crJXJLoginGamePacket::qqYellowQuery(int playerid, CRCore::crData* data/*,CRDataBase::crDataBase* accountSession*/)
{
	unsigned char isYellow = 0;
	unsigned char isYearYellow = 0;
	unsigned char yellowLevle = 0;
	ref_ptr<crJXJYellowInfoQueryData> playerYellowQuery = new crJXJYellowInfoQueryData;
	playerYellowQuery->buildQuerySql(playerid);

	crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
	ref_ptr<crDataBase> accountSession = accountdb->beginSession();
	accountSession->executeQuery(playerYellowQuery.get());
	crDataBase::QueryResultVec &playerYellowQueryResultVec = accountSession->getQueryResult();
	if(!playerYellowQueryResultVec.empty())
	{
		playerYellowQuery = dynamic_cast<crJXJYellowInfoQueryData *>(playerYellowQueryResultVec[0].get());
		isYearYellow = playerYellowQuery->getIsYearYellow();
		isYellow = playerYellowQuery->getIsYellow();
		yellowLevle = playerYellowQuery->getYellowLevel();
	}
	accountSession->releaseQuery();
	accountdb->endSession(accountSession.get());

	data->inputParam(WCHDATA_JXJISYELLOW,&isYellow);
	data->inputParam(WCHDATA_JXJISYEARYELLOW,&isYearYellow);
	data->inputParam(WCHDATA_JXJYELLOWLEVEL,&yellowLevle);
}

void crJXJLoginGamePacket::parsePacket(const std::string &sender)
{//id name
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//转发到GameServer
		if(m_streamBuf.valid())
		{
			crJXJLoginGamePacket packet;
			crJXJLoginGamePacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Game);
			if(netConductor)
			{
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket SendTo GameServer"<<std::endl;
		}
	}
	else if(netType == GameServer)
	{
		std::string playerAddress = m_streamBuf->_readString();
		int playerid = m_streamBuf->_readInt();
		//std::string name = m_streamBuf->_readString();
		std::string key = m_streamBuf->_readString();
		int serverid = m_streamBuf->_readInt();
		int version = 0;
		if(!m_streamBuf->eof())
			version = m_streamBuf->_readInt();
		//check gameid
		crNetManager *netManager = m_netConductor->getNetManager();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		char loginCode = 0;//0游戏登陆失败，1游戏登陆成功，2帐号已经在线，3有人在尝试登陆你的帐号，4客户端版本信息不正确,5重新创建角色，6区服务器信息错误,7您刚与该区服务器断开,请稍后在登录,或者选择其它区登录
		if(!netCallback->checkServer(serverid))
		{
			loginCode = 6;
			crJXJLoginGamePacket packet;
			crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),NULL);
			netManager->sendPacket(sender,packet);
			return;
		}
		/////断线重连check
		ref_ptr<crGameServerPlayerData> dropedPlayerData;
		int remuse = netCallback->remuseDropedPlayer(playerid,key,dropedPlayerData);
		if(remuse == -1)
		{//您刚与该区服务器断开,请稍后在登录,或者选择其它区登录
			loginCode = 7;
			crJXJLoginGamePacket packet;
			crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),NULL);
			netManager->sendPacket(sender,packet);
			return;
		}
		////维护期间只能GM账号登陆
		bool canlogin = true;
		if(!crGlobalHandle::gOpenGameLogin())
		{
			if(crGlobalHandle::gData()->getGMAuthority(playerid)<0)
				canlogin = false;//非GM帐号
			//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
			//if(gmTab.valid())
			//{
			//	crTableIO::StrVec record;
			//	if(gmTab->queryOneRecord(0,crArgumentParser::appItoa(playerid),record)<0)
			//		canlogin = false;//非GM帐号
			//}
		}
		else
		{//已经开放，判断版本号
			if(version!=crGlobalHandle::getVersion())
			{//玩家版本号与服务器不符
				loginCode = 4;
				crJXJLoginGamePacket packet;
				crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),NULL);
				netManager->sendPacket(sender,packet);
				return;
			}
		}
		if(canlogin)
		{
			if(remuse==1 && dropedPlayerData.valid() && dropedPlayerData->getPlayerGameData() && dropedPlayerData->getPlayerGameData()->getDataClass())
			{
				dropedPlayerData->setPlayerAdress(playerAddress);
				dropedPlayerData->setPlayerConnectServerAddress(sender);
				dropedPlayerData->setSceneID(0);
				dropedPlayerData->setSceneServerAddress("");

				crData *data = dropedPlayerData->getPlayerGameData()->getDataClass();
				moneyAndVipQuery(playerid,serverid,data/*,globalSession.get()*/);

				//黄钻信息查询
				qqYellowQuery(playerid,data/*,accountSession.get()*/);

				netDataManager->insertPlayerData(dropedPlayerData.get());
				dropedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,dropedPlayerData.get()));
				//游戏登陆成功
				loginCode = 1;
				crJXJLoginGamePacket packet;
				crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),dropedPlayerData.get());
				netManager->sendPacket(sender,packet);
				crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(dropedPlayerData.get(),NULL));
			}
			else
			{
				bool keyCheck = false;
				if(crGlobalHandle::gData()->gEnableRobotTest())
				{
					if(key.compare("Creature3DRobot$@#") == 0)
					{//只有机器人帐号可以这样登录
						if(crGlobalHandle::gData()->getGMAuthority(playerid)==JXJGMPermission_Robot)
							keyCheck = true;
						//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
						//if(gmTab.valid())
						//{
						//	crTableIO::StrVec record;
						//	if(gmTab->queryOneRecord(0,crArgumentParser::appItoa(playerid),record)>=0 && atoi(record[1].c_str()) == 1)
						//		keyCheck = true;//
						//}
					}
				}
				//if(!keyCheck)
				//{
					std::string username,advertid;
					crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
					ref_ptr<crDataBase> accountSession = accountdb->beginSession();

					ref_ptr<crJXJLoginKeyQueryData> loginGameQuery = new crJXJLoginKeyQueryData;
					loginGameQuery->buildQuerySql(playerid);

					accountSession->executeQuery(loginGameQuery.get());
					crDataBase::QueryResultVec loginGameQueryResultVec = accountSession->getQueryResult();
					accountSession->releaseQuery();
					if(!loginGameQueryResultVec.empty())
					{
						loginGameQuery = dynamic_cast<crJXJLoginKeyQueryData *>(loginGameQueryResultVec[0].get());
						const std::string& dbkey = loginGameQuery->getKey();
						//std::string key1;
						//std::string::size_type i = dbkey.find_first_of(':');
						//ip = std::string(dbkey.begin(),dbkey.begin()+i);
						//key1 = std::string(dbkey.begin()+i+1,dbkey.end());
						//i = playerAddress.find_last_of(':');
						//std::string playerip = std::string(playerAddress.begin(),playerAddress.begin()+i);
						if(/*playerip.compare(ip) == 0 && */key.compare(dbkey) == 0)
						{//已经做了有效时间校验，这里不再做IP校验了，有些IP是易变的
							keyCheck = true;
							username = loginGameQuery->getUserName();
							advertid = loginGameQuery->getAdvertid();
						}
					}
					accountdb->endSession(accountSession.get());
				//}
				if(keyCheck)
				{
					if(dropedPlayerData.valid() && dropedPlayerData.valid() && dropedPlayerData->getPlayerGameData() && dropedPlayerData->getPlayerGameData()->getDataClass())
					{
						dropedPlayerData->setPlayerAdress(playerAddress);
						dropedPlayerData->setPlayerConnectServerAddress(sender);
						dropedPlayerData->setSceneID(0);
						dropedPlayerData->setSceneServerAddress("");
						dropedPlayerData->setKey(key,username);
						dropedPlayerData->getPlayerGameData()->setAdvertid(advertid);

						crData *data = dropedPlayerData->getPlayerGameData()->getDataClass();
						//crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
						//ref_ptr<crDataBase> globalSession = db->beginSession();
						moneyAndVipQuery(playerid,serverid,data/*,globalSession.get()*/);
						//db->endSession(globalSession.get());

						//黄钻信息查询
						//crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
						//ref_ptr<crDataBase> accountSession = accountdb->beginSession();
						qqYellowQuery(playerid,data/*,accountSession.get()*/);
						//accountdb->endSession(accountSession.get());

						netDataManager->insertPlayerData(dropedPlayerData.get());
						dropedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,dropedPlayerData.get()));
						//游戏登陆成功
						loginCode = 1;
						crJXJLoginGamePacket packet;
						crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),dropedPlayerData.get());
						netManager->sendPacket(sender,packet);
						crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(dropedPlayerData.get(),NULL));
					}
					else
					{
						ref_ptr<crGameServerPlayerData> connectedPlayerData  = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
						if(connectedPlayerData.valid())
						{//
							if(connectedPlayerData->isPlayerGameDataValid() && connectedPlayerData->checkKey(key))
							{//断线重连
								crKickoffPlayerPacket kpacket;
								crKickoffPlayerPacket::buildReplyPacket(kpacket,playerid);
								netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
								if(!connectedPlayerData->getSceneServerAddress().empty())
								{////同时踢下线
									netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
								}
								connectedPlayerData->setPlayerAdress(playerAddress);
								connectedPlayerData->setPlayerConnectServerAddress(sender);
								connectedPlayerData->setSceneID(0);
								connectedPlayerData->setSceneServerAddress("");

								connectedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,connectedPlayerData.get()));
								//游戏登陆成功
								loginCode = 1;
								crJXJLoginGamePacket packet;
								crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),connectedPlayerData.get());
								netManager->sendPacket(sender,packet);
								crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(connectedPlayerData.get(),NULL));
							}
							else
							{
								//loginCode = 2;//通知客户端，帐号已经处于登陆状态
								////同时通知已经登陆的玩家，有人在尝试登陆你的帐号
								crPlayerGameData *playerGameData = connectedPlayerData->getPlayerGameData();
								if( playerGameData&&playerGameData->getDataClass() )
								{
									crPlayerDataEventPacket ppacket;
									crPlayerDataEventPacket::buildReplyPacket(ppacket,playerid,WCH_SomeOneTestLogin,NULL);
									netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),ppacket);
								}

								//if(!connectedPlayerData->getSceneServerAddress().empty())
								//{////同时踢下线
								//	crKickoffPlayerPacket kpacket;
								//	crKickoffPlayerPacket::buildReplyPacket(kpacket,playerid);
								//	netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
								//}
								////netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
								//connectedPlayerData->setSceneID(0);
								//connectedPlayerData->setSceneServerAddress("");
								//netDataManager->removePlayerData(playerid);

								//crJXJLoginGamePacket packet;
								//crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getServerName(),NULL);
								//netManager->sendPacket(sender,packet);
								///
								crKickoffPlayerPacket kpacket;
								crKickoffPlayerPacket::buildReplyPacket(kpacket,playerid);
								netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
								if(!connectedPlayerData->getSceneServerAddress().empty())
								{////同时踢下线
									netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
								}
								connectedPlayerData->setPlayerAdress(playerAddress);
								connectedPlayerData->setPlayerConnectServerAddress(sender);
								connectedPlayerData->setSceneID(0);
								connectedPlayerData->setSceneServerAddress("");

								connectedPlayerData->getPlayerGameData()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,connectedPlayerData.get()));
								//游戏登陆成功
								loginCode = 1;
								crJXJLoginGamePacket packet;
								crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),connectedPlayerData.get());
								netManager->sendPacket(sender,packet);
								crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(connectedPlayerData.get(),NULL));
							}
						}
						else
						{
							crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
							ref_ptr<crDataBase> globalSession = db->beginSession();

							ref_ptr<crGameServerPlayerData> playerData;
							playerData = new crGameServerPlayerData;
							playerData->setKey(key,username);
							playerData->saveTimer() = netCallback->getSaveInterval();
							playerData->setPlayerID(playerid);
							//playerData->setPlayerName(name);
							playerData->setPlayerAdress(playerAddress);
							playerData->setPlayerConnectServerAddress(sender);
							//playerData->setNickName(loginGameQuery->getNickName());

							//ref_ptr<crUpdateGameState> updateGameState = new crUpdateGameState;
							//updateGameState->buildUpdateSql(playerid,gameid);
							//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
							//crDBSavePacket dbpacket;
							//crDBSavePacket::buildRequestPacket(dbpacket,updateGameState.get());
							//dbConductor->getNetManager()->sendPacket("all",dbpacket);
							//if(globalSession->executeUpdate(updateGameState.get()))
							//{//插入成功
							//	globalSession->commit();
							//}

							unsigned short gamedbid = 1;
							bool found = false;

							ref_ptr<crPlayerGameDBQueryData> playergamedbQuery = new crPlayerGameDBQueryData;
							playergamedbQuery->buildQuerySql(playerid,serverid);

							globalSession->executeQuery(playergamedbQuery.get());
							crDataBase::QueryResultVec &playergamedbQueryResultVec = globalSession->getQueryResult();
							if(!playergamedbQueryResultVec.empty())
							{
								playergamedbQuery = dynamic_cast<crPlayerGameDBQueryData *>(playergamedbQueryResultVec[0].get());
								gamedbid = playergamedbQuery->getGameDBID();
								found = true;
							}
							globalSession->releaseQuery();

							if(!found)
							{
								unsigned short dbcount = 0;

								ref_ptr<crGameDBCountQueryData> gamedbcountQuery = new crGameDBCountQueryData;
								gamedbcountQuery->buildQuerySql(netCallback->getGameID());

								globalSession->executeQuery(gamedbcountQuery.get());
								crDataBase::QueryResultVec &gamedbcountQueryResultVec = globalSession->getQueryResult();
								if(!gamedbcountQueryResultVec.empty())
								{//0是系统所有，玩家从1开始
									gamedbcountQuery = dynamic_cast<crGameDBCountQueryData *>(gamedbcountQueryResultVec[0].get());
									dbcount = gamedbcountQuery->getGameDBCount();
									if(dbcount>0) gamedbid = playerid%dbcount+1;//CRCore::rangei(1,dbcount).get_random();
								}
								globalSession->releaseQuery();

								ref_ptr<crInsertPlayerGameDB> insertPlayerGameDB = new crInsertPlayerGameDB;
								insertPlayerGameDB->buildUpdateSql(playerid,serverid,gamedbid);
								//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
								//crDBSavePacket packet;
								//crDBSavePacket::buildRequestPacket(packet,insertPlayerGameDB.get());
								//dbConductor->getNetManager()->sendPacket("all",packet);
								if(globalSession->executeUpdate(insertPlayerGameDB.get()))
								{//插入成功
									globalSession->commit();
									found = true;
								}
							}
							if(found)
							{
								playerData->setGameDBID(gamedbid);
								////////playerGameData查询
								ref_ptr<crPlayerGameData> playerGameData;
								std::string script = netCallback->getGameName() + "/playergamedata.cfg";
								ref_ptr<crData> data = CREncapsulation::loadData(script);
								if(data.valid())
								{
									bool dbexist = false;
									playerGameData = new crPlayerGameData;
									playerGameData->setPlayerID(playerid);
									playerGameData->setAdvertid(advertid);
									playerGameData->setDataClass(data.get());
									//playerData->setPlayerGameData(playerGameData.get());
									db->endSession(globalSession.get());
									moneyAndVipQuery(playerid,serverid,data.get()/*,globalSession.get()*/);

									//黄钻信息查询
									//crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
									//ref_ptr<crDataBase> accountSession = accountdb->beginSession();
									qqYellowQuery(playerid,data.get()/*,accountSession.get()*/);
									//accountdb->endSession(accountSession.get());

									crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
									if(gamedb)
									{
										ref_ptr<crDataBase> gameSession = gamedb->beginSession();
										ref_ptr<crPlayerGameDataQueryData> playerGameDataQuery = new crPlayerGameDataQueryData;
										playerGameDataQuery->buildQuerySql(playerid);

										gameSession->executeQuery(playerGameDataQuery.get());
										crDataBase::QueryResultVec &playerGameDataQueryResultVec = gameSession->getQueryResult();
										if(!playerGameDataQueryResultVec.empty())
										{
											playerGameDataQuery = dynamic_cast<crPlayerGameDataQueryData *>(playerGameDataQueryResultVec[0].get());
											playerGameData->setID(playerGameDataQuery->getID());
											playerGameData->setStream(playerGameDataQuery->getStream());
											int roleid = playerGameDataQuery->getMainRoleID();
											if(roleid > 0)
											{//查询并设置mainrole
												playerGameData->setMainRoleID(roleid);
											}
											playerGameData->setDBValid(true);
											dbexist = true;
										}
										//else
										//{//insert
										//	ref_ptr<crInsertPlayerGameData> insertPlayerGameData = new crInsertPlayerGameData;
										//	insertPlayerGameData->buildUpdateSql(playerid);
										//	if(gameSession->executeUpdate(insertPlayerGameData.get()))
										//	{
										//		gameSession->commit();
										//	}
										//}
										gameSession->releaseQuery();
										gamedb->endSession(gameSession.get());
										if(playerGameData->getStream())
										{
											data->inputParam(WCHDATA_JXJPlayerID,&playerid);
											data->inputParam(WCHDATA_DataStream,playerGameData->getStream());
											playerGameData->setStream(NULL);
										}
										if(dbexist)
										{
											void *param;
											data->getParam(WCHDATA_JXJHomeID,param);
											unsigned short homeid = *(unsigned short *)param;
											if(homeid==0)
											{//角色创建过程中服务器重启，导致角色为存储，给一个随机势力
												playerGameData->doEvent(WCH_RecvPlayerGameDataCreateStream);
											}
										}
										playerData->setPlayerGameData(playerGameData.get());
										data->excHandle(MAKEINT64(WCH_InitData,ID_LoginGameServer));

										//crNetConductor *parentServer = crNetContainer::getInstance()->getNetConductor(GameServerClient_Parent);
										//if(parentServer)
										//{
										//	crLoginParentPacket loginParentPacket;
										//	crLoginParentPacket::buildRequestPacket(loginParentPacket,playerData->getPlayerID(),playerAddress);
										//	parentServer->getNetManager()->sendPacket("all",loginParentPacket);
										//	playerData->setParentServerLineConductor(parentServer);
										//}
										netDataManager->insertPlayerData(playerData.get());
										playerGameData->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(NULL,playerData.get()));
										//游戏登陆成功
										loginCode = 1;
										crJXJLoginGamePacket packet;
										crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),playerData.get());
										netManager->sendPacket(sender,packet);
										crServerBrainHandle::getInstance()->doEvent(MAKEINT64(WCH_PlayerLogin,GameServer),MAKEINT64(playerData.get(),NULL));
									}
									else
									{
										CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket Error 没有找到GameDB dbid = "<<playerData->getGameDBID()<<std::endl;
									}
								}
								else
								{
									db->endSession(globalSession.get());
								}
							}
							else
							{
								db->endSession(globalSession.get());
							}
						}
					}
				}
			}
		}
		if(loginCode == 0)
		{//游戏登陆失败
			crJXJLoginGamePacket packet;
			crJXJLoginGamePacket::buildReplyPacket(packet,playerAddress,loginCode,netCallback->getWebServerID(),netCallback->getSite(),netCallback->getServerName(),NULL);
			netManager->sendPacket(sender,packet);
		}

		char log[128];
		sprintf(log," 进行登录 状态码 %d（0，表示失败 1,表示登陆成功 2，表示重复登陆）ip:%s\0",loginCode,playerAddress.c_str());
		std::string logdata(log);
		GameLogData gamelog(Log_LoginLog,logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket "<<loginCode<<std::endl;
	}
	else if(netType == GameConnectServerClient_Game)
	{//转发到Client
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			char loginCode = m_streamBuf->_readChar();
			if(loginCode==1)
			{
				int playerid = m_streamBuf->_readInt();

				crNetDataManager *netDataManager = connectServer->getNetDataManager();
				ref_ptr<crGameConnectServerCallback> callback = dynamic_cast<crGameConnectServerCallback *>(netDataManager->getNetCallback());
				ref_ptr<crGameConnectServerPlayerData> playerdata;
				callback->remuseDropedPlayer(playerid,playerdata);
				if(playerdata.valid())
				{
					playerdata->setPlayerAdress(address);
					playerdata->setGameServerLineConductor(m_netConductor);
					playerdata->setSceneServerLineConductor(NULL);
					netDataManager->insertPlayerData(playerdata.get());
				}
				else
				{
					playerdata = new crGameConnectServerPlayerData;
					playerdata->setPlayerID(playerid);
					//playerData->setPlayerName(m_streamBuf->_readString());
					playerdata->setPlayerAdress(address);
					playerdata->setGameServerLineConductor(m_netConductor);

					netDataManager->insertPlayerData(playerdata.get());
				}
			}
			crJXJLoginGamePacket packet;
			crJXJLoginGamePacket::buildReplyPacket2(packet,loginCode,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket SendTo GameClient"<<std::endl;
		}
	}
	else if(netType == GameClient_Game)
	{//id name
		char loginCode = m_streamBuf->_readChar();
		if(loginCode==1)
		{
			std::string username = m_streamBuf->_readString();
			int webserverid = m_streamBuf->_readInt();
			std::string site = m_streamBuf->_readString();
			std::string serverName = m_streamBuf->_readString();
			crMyPlayerData::getInstance()->setUserName(username);
			crMyPlayerData::getInstance()->setWebServerID(webserverid);
			crMyPlayerData::getInstance()->setSite(site);
			crMyPlayerData::getInstance()->setServerName(serverName);
			if(crDisplaySettings::instance()->getRunMode()<crDisplaySettings::Robot)
			{
				crMyPlayerData *myPlayerData = crMyPlayerData::getInstance();
				char buf[128];
				memset(buf,0,128);
				sprintf_s(buf,"bin\\Jiangxingjue.exe -WebLogin %s|%d|%d|%d\0",myPlayerData->getKey().c_str(),
					myPlayerData->getPlayerID(),myPlayerData->getLastServerID(),myPlayerData->getISP());//key|playerid|serverid|isp
				CRNetApp::crGlobalHandle::argvstr() = buf;
			}

			crGlobalHandle::getInstance()->setStartupLoadingProgress(0.89f);
			const std::string &gameName = crMyPlayerData::getInstance()->getSelectedGame()->getGameName();
			crData *globalData = crGlobalHandle::getInstance()->getDataClass();
			std::string globalDataFile = gameName + "/GlobalData.cfg";
			CREncapsulation::loadIncludeData(globalDataFile,globalData);
			short sceneid = CRNetApp::crGlobalHandle::getInstance()->gData()->gFirstGameSceneID();
			crData *data = crRunGameHandle::getInstance()->getDataClass();
			data->inputParam(WCHDATA_SceneID,&sceneid);
			crGlobalHandle::getInstance()->setStartupLoadingProgress(0.99f);
			if(m_streamBuf->_readBool())
			{
				crPlayerGameData *playerGameData = new crPlayerGameData;
				//playerGameData->setName(m_streamBuf->_readString());
				playerGameData->setDBValid(m_streamBuf->_readBool());
				playerGameData->setMainRoleID(m_streamBuf->_readInt());
				crMyPlayerData::getInstance()->setPlayerGameData(playerGameData);
				std::string script = crMyPlayerData::getInstance()->getSelectedGame()->getGameName() + "/playergamedata.cfg";
				ref_ptr<crData> data = CREncapsulation::loadData(script);
				if(data.valid())
				{
					playerGameData->setPlayerID(crMyPlayerData::getInstance()->getPlayerID());
					playerGameData->setDataClass(data.get());
					if(m_streamBuf->getRemainSize()>0)
					{
						data->inputParam(WCHDATA_PlayerStream,m_streamBuf.get());
					}
					data->excHandle(MAKEINT64(WCH_InitData,playerGameData));
					playerGameData->doEvent(WCH_PlayerLogin,NULL);
					//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket doEvent WCH_PlayerLogin"<<std::endl;
					//void *param;
					//data->getParam(WCHDATA_HomeID,param);
					//if(param)
					//{
					//	int sceneID = *(int *)param;
					//	if(sceneID>0)
					//	{//如果有homescene,玩家登陆有直接进入homescene
					//		crData *runGameData = crRunGameHandle::getInstance()->getDataClass();
					//		runGameData->inputParam(WCHDATA_SceneID,&sceneID);
					//	}
					//}
				}
				int mainRoleID = playerGameData->getMainRoleID();
				if(mainRoleID>0)
				{
					crMyPlayerData::getInstance()->setLoadMainRoleCase(1);
					crQueryRoleInfoPacket packet;
					crQueryRoleInfoPacket::buildRequestPacket(packet,mainRoleID);
					m_netConductor->getNetManager()->sendPacket("all",packet);
					return;
				}
			}
			crGlobalHandle::getInstance()->setStartupLoadingProgress(1.0f);
		}
		else
		{
			crNetContainer::getInstance()->removeDynamicNetConductor(GameClient_Game);
		}
		crGlobalHandle::getInstance()->doEvent(WCH_LoginGameReturn,MAKEINT64(loginCode,NULL));
	}
}
/////////////////////////////////////////
//
//crJXJConnectServerQueryPacket
//
/////////////////////////////////////////
crJXJConnectServerQueryPacket::crJXJConnectServerQueryPacket():
	crStreamPacket(crJXJConnectServerQueryPacket::ID)
{
}

crJXJConnectServerQueryPacket::crJXJConnectServerQueryPacket( const crJXJConnectServerQueryPacket &p ):
	crStreamPacket(crJXJConnectServerQueryPacket::ID,p)
{
}

void crJXJConnectServerQueryPacket::buildRequestPacket(crJXJConnectServerQueryPacket &packet,int serverid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(serverid);
	streamBuf->_writeInt(crMyPlayerData::getInstance()->getPlayerID());
}
void crJXJConnectServerQueryPacket::buildRequestPacket2(crJXJConnectServerQueryPacket &packet,const std::string &playerAddress,crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJConnectServerQueryPacket::buildReplyPacket(crJXJConnectServerQueryPacket &packet,const std::string &playerAddress,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+5+(stream?stream->getBufSize():0));
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	if(stream)
	{
		streamBuf->_writeBool(true);
		streamBuf->write(stream->getBuf(),stream->getBufSize());
	}
	else
	{
		streamBuf->_writeBool(false);
	}
}
void crJXJConnectServerQueryPacket::buildReplyPacket2(crJXJConnectServerQueryPacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJConnectServerQueryPacket::parsePacket(const std::string &sender)
{
	if(!m_netConductor) return;

	int netType = m_netConductor->getNetType();
	if(netType == LoginConnectServer)
	{//转发到GameServer
		crJXJConnectServerQueryPacket packet;
		crJXJConnectServerQueryPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(LoginConnectServerClient_Login);
		if(netConductor)
		{
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crJXJConnectServerQueryPacket SendTo LoginServer"<<std::endl;
	}
	else if(netType == LoginServer)
	{//
		std::string playerAddress = m_streamBuf->_readString();
		int serverid = m_streamBuf->_readInt();
		int playerid = 0;
		if(!m_streamBuf->eof())
			playerid = m_streamBuf->_readInt();
		bool success = false;
		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession = db->beginSession();
		ref_ptr<crConnectServersQueryData> query = new crConnectServersQueryData;
		query->buildQuerySql(serverid);
		globalSession->executeQuery(query.get());
		crDataBase::QueryResultVec& queryResultVec = globalSession->getQueryResult();
		crJXJConnectServerQueryPacket packet;
		if(!queryResultVec.empty())
		{
			ref_ptr<crQueryConnectServerData> connectServerData;
			ConnectServerMap connectServerMap;
			for( crDataBase::QueryResultVec::iterator itr = queryResultVec.begin();
				itr != queryResultVec.end();
				++itr )
			{
				query = dynamic_cast<crConnectServersQueryData *>(itr->get());
				if(query->getRemainCount()>0)
				{
					connectServerData = new crQueryConnectServerData;
					connectServerData->setIP(query->getIP());//19
					connectServerData->setIP2(query->getIP2());//19
					connectServerData->setIP3(query->getIP3());//19
					connectServerData->setPort(query->getPort());//2
					connectServerData->setRemainCount(query->getRemainCount());
					connectServerMap.insert(std::make_pair(connectServerData->getRemainCount(),connectServerData.get()));
				}
			}
			if(!connectServerMap.empty())
			{
				const std::string &ip = connectServerMap.begin()->second->getIP();
				const std::string &ip2 = connectServerMap.begin()->second->getIP2();
				const std::string &ip3 = connectServerMap.begin()->second->getIP3();
				unsigned short port = connectServerMap.begin()->second->getPort();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(ip.length()+ip2.length()+ip3.length()+14);
				stream->_writeString(ip);
				stream->_writeString(ip2);
				stream->_writeString(ip3);
				stream->_writeUShort(port);
				crJXJConnectServerQueryPacket::buildReplyPacket(packet,playerAddress,stream.get());
				success = true;
			}
			m_netConductor->getNetManager()->sendPacket(sender,packet);
		}
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		if(!success)
		{
			crJXJConnectServerQueryPacket::buildReplyPacket(packet,playerAddress,NULL);
		}
		else if(playerid>0)
		{
			crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
			ref_ptr<crDataBase> accountSession = accountdb->beginSession();
			ref_ptr<crJXJUpdateLoginServerID> updateLoginServerID = new crJXJUpdateLoginServerID;
			updateLoginServerID->buildUpdateSql(playerid,serverid);
			if(accountSession->executeUpdate(updateLoginServerID.get()))
			{//插入成功
				accountSession->commit();
			}
			accountdb->endSession(accountSession.get());
		}
	}
	else if(netType == LoginConnectServerClient_Login)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(LoginConnectServer);
		if(connectServer)
		{
			std::string address = m_streamBuf->_readString();
			crJXJConnectServerQueryPacket packet;
			crJXJConnectServerQueryPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(address,packet);
			//CRCore::notify(CRCore::ALWAYS)<<"crJXJConnectServerQueryPacket Reply"<<std::endl;
		}
	}
	else if(netType == GameClient_Login)
	{//
		crGlobalHandle::getInstance()->doEvent(WCH_RecvConnectServer,MAKEINT64(m_streamBuf.get(),NULL));
	}
}

////////////////////////监控服务器包
//void crJXJMonitorPingPacket::parsePacket(const std::string &sender)
//{
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//	if(netType == GameConnectServer)
//	{
//		if(m_streamBuf.valid())
//		{
//			crJXJMonitorPingPacket packet;
//			crJXJMonitorPingPacket::buildRequestPacket2(packet,sender,m_streamBuf.get());
//			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Game);
//			if(netConductor)
//			{
//				netConductor->getNetManager()->sendPacket("all",packet);
//			}
//			GameConnectServerClient_Scene
//			//CRCore::notify(CRCore::ALWAYS)<<"crJXJLoginGamePacket SendTo GameServer"<<std::endl;
//		}
//	}
//	else if(netType == GameServer)
//	{
//		std::string playerAddress = m_streamBuf->_readString();
//		crJXJMonitorPingPacket packet;
//		crJXJMonitorPingPacket::buildReplyPacket(packet,playerAddress,loginCode,NULL);
//		netManager->sendPacket(sender,packet);
//	}
//	else if (netType == GameConnectServerClient_Game)
//	{
//		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
//		if(connectServer)
//		{
//			std::string playerAddress = m_streamBuf->_readString();
//			crJXJMonitorPingPacket packet;
//			crJXJMonitorPingPacket::buildReplyPacket2(packet,loginCode,m_streamBuf.get());
//			connectServer->getNetManager()->sendPacket(playerAddress,packet);
//		}
//	}
//	else if(netType == SceneServer)
//	{
//
//	}
//	else if (netType == GameConnectServerClient_Scene)
//	{
//	}
//	else if(netType == JXJMonitor1)
//	{
//		
//	}
//	else if(netType == JXJMonitor2)
//	{
//
//	}
//	else if(netType == JXJMonitor3)
//	{
//
//	}
//	else if(netType == JXJMonitor4)
//	{
//
//	}
//}

/////////////////////////////////////////
//
//crJXJQueryAbstractItemPacket
//
/////////////////////////////////////////
crJXJQueryAbstractItemPacket::crJXJQueryAbstractItemPacket():
	crStreamPacket(crJXJQueryAbstractItemPacket::ID)
{
}
crJXJQueryAbstractItemPacket::crJXJQueryAbstractItemPacket( const crJXJQueryAbstractItemPacket &p ):
	crStreamPacket(crJXJQueryAbstractItemPacket::ID,p)
{
}
void crJXJQueryAbstractItemPacket::buildRequestPacket(crJXJQueryAbstractItemPacket &packet,int abstractid,crInstanceItem *item,unsigned char camp,/*unsigned short*/int itemcount,float interval,short bufid,unsigned char npctype)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(64);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(item->getInstanceItemID());
	streamBuf->_writeInt(abstractid/*item->getAbstractItemID()*/);
	streamBuf->_writeInt(item->getSceneID());
	streamBuf->_writeInt(item->getRoomID());
	streamBuf->_writeInt(item->getLayerID());
	streamBuf->_writeInt(item->getPosx());
	streamBuf->_writeInt(item->getPosy());
	streamBuf->_writeUChar(item->getItemtype());
	streamBuf->_writeUChar(camp);
	streamBuf->_writeInt(itemcount);
	streamBuf->_writeFloat(interval);
	streamBuf->_writeVec3(item->getDir());
	streamBuf->_writeInt64(item->getOwnerID());
	streamBuf->_writeShort(bufid);
	streamBuf->_writeUChar(npctype);
}
void crJXJQueryAbstractItemPacket::buildReplyPacket(crJXJQueryAbstractItemPacket &packet,bool sucess,int streamid,int count)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(9);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeBool(sucess);
	streamBuf->_writeInt(streamid);
	streamBuf->_writeInt(count);
}
void crJXJQueryAbstractItemPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		int itemid = m_streamBuf->_readInt();
		int abstractid = m_streamBuf->_readInt();
		crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
		if(!aitem)
		{
			crJXJQueryAbstractItemPacket packet;
			crJXJQueryAbstractItemPacket::buildReplyPacket(packet,false,m_streamBuf->_readInt(),itemid);//无效的abstractid，通知sceneServer回收itemid
			CRCore::notify(CRCore::ALWAYS)<<"crJXJQueryAbstractItemPacket没有找到AbstractItemID="<<abstractid<<std::endl;
			return;
		}
		int count = 0;
		std::vector< ref_ptr<crStreamBuf> > BufVec;

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(MTUBUFFERSIZE);//(MAXBUFFERSIZE-12);
		stream->_writeInt(itemid);
		stream->write(m_streamBuf->getPtr(),m_streamBuf->getRemainSize());
		writeAbstractItem(aitem,stream,BufVec,count);
		if(stream->getBufSize()>0)
		{
			count += stream->getBufSize();
			BufVec.push_back(stream.get());
		}
		if(count>0)
		{
			int streamid = crDataStreamTransfer::getInstance()->gainStreamID();
			crJXJQueryAbstractItemPacket packet;
			crJXJQueryAbstractItemPacket::buildReplyPacket(packet,true,streamid,count);
			m_netConductor->getNetManager()->sendPacket(sender,packet);
			crDataStreamPacket streamPacket;
			for( std::vector< ref_ptr<crStreamBuf> >::iterator itr = BufVec.begin();
				itr != BufVec.end();
				++itr )
			{
				crDataStreamPacket::buildRequestPacket(streamPacket,0,streamid,crJXJQueryAbstractItemPacket::ID,itr->get());
				m_netConductor->getNetManager()->sendPacket(sender,streamPacket);
			}
			crDataStreamTransfer::getInstance()->recycleStreamID(streamid,crGlobalHandle::gData()->gRecycleStreamIDTime_in());
		}
	}
	else if(netType == SceneServerClient_Game)
	{
		bool sucess = m_streamBuf->_readBool();
		if(sucess)
		{
			int streamid = m_streamBuf->_readInt();
			int count = m_streamBuf->_readInt();
			crDataStreamTransfer::getInstance()->createTransferingStream(streamid,crJXJQueryAbstractItemPacket::ID,count);
		}
		else
		{
			int sceneid = m_streamBuf->_readInt();
			int itemid = m_streamBuf->_readInt();
			//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
			//CRNetApp::crScene *scene = netCallback->findScene(sceneid);
			crGlobalHandle::recycleItemID(itemid);
		}
	}
}

JXJ::crJXJMonitorPingPacket::crJXJMonitorPingPacket():
	crStreamPacket(crJXJMonitorPingPacket::ID)
{

}

JXJ::crJXJMonitorPingPacket::crJXJMonitorPingPacket( const crJXJMonitorPingPacket&p ):
	crStreamPacket(crJXJMonitorPingPacket::ID,p)
{

}


void JXJ::crJXJMonitorPingPacket::buildRequestPacket( crJXJMonitorPingPacket &packet,int gameid )
{

}

void JXJ::crJXJMonitorPingPacket::buildRequestPacket2(crJXJMonitorPingPacket &packet,const std::string &playerAddress)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
}

void JXJ::crJXJMonitorPingPacket::buildReplyPacket(crJXJMonitorPingPacket  &packet,const std::string &playerAddress,const std::string& servername)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(playerAddress.length()+servername.length()+8);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(playerAddress);
	streamBuf->_writeString(servername);
}

void JXJ::crJXJMonitorPingPacket::buildReplyPacket2(crJXJMonitorPingPacket  &packet,CRCore::crStreamBuf *stream )
{
		crStreamBuf *streamBuf = new crStreamBuf;
		streamBuf->createBuf(stream->getRemainSize());
		packet.setStreamBuf(streamBuf);
		streamBuf->write(stream->getPtr(),stream->getRemainSize());
}

void JXJ::crJXJMonitorPingPacket::parsePacket( const std::string &sender )
{
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		//if(m_streamBuf.valid())
		
			crJXJMonitorPingPacket packet;
			crJXJMonitorPingPacket::buildRequestPacket2(packet,sender);
			
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameConnectServerClient_Game);
			if(netConductor)
			{
				// 发给 gameserver
				netConductor->getNetManager()->sendPacket("all",packet);
			}

			std::vector< crNetConductor *> netConductorVec;
			crNetContainer::getInstance()->getNetConductorVec(GameConnectServerClient_Scene,netConductorVec);

			for( std::vector< crNetConductor* >::iterator iter = netConductorVec.begin();
				 iter!=netConductorVec.end();
				 iter++ )
			{
				// 发给sceneserver
				(*iter)->getNetManager()->sendPacket("all",packet);
			}
		
	}
	else if(netType == GameServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		if (netManager)
		{
			std::string playerAddress = m_streamBuf->_readString();
			crJXJMonitorPingPacket packet;
			crJXJMonitorPingPacket::buildReplyPacket(packet,playerAddress,m_netConductor->getName());
			netManager->sendPacket(sender,packet);
		}
	}
	else if (netType == GameConnectServerClient_Game
		|| netType == GameConnectServerClient_Scene)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		if(connectServer)
		{
			// 发回给client
			std::string playerAddress = m_streamBuf->_readString();
			crJXJMonitorPingPacket packet;
			crJXJMonitorPingPacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerAddress,packet);
		}
	}
	else if(netType == SceneServer)
	{
		crNetManager *netManager = m_netConductor->getNetManager();
		if (netManager)
		{
			std::string playerAddress = m_streamBuf->_readString();
			crJXJMonitorPingPacket packet;
			crJXJMonitorPingPacket::buildReplyPacket(packet,playerAddress,m_netConductor->getName());
			netManager->sendPacket(sender,packet);
		}
	}
	//else if (netType == GameConnectServerClient_Scene)
	//{
	//	crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
	//	if(connectServer)
	//	{
	//		// 发回给client
	//		std::string playerAddress = m_streamBuf->_readString();
	//		crJXJMonitorPingPacket packet;
	//		crJXJMonitorPingPacket::buildReplyPacket2(packet,m_streamBuf.get());
	//		connectServer->getNetManager()->sendPacket(playerAddress,packet);
	//	}
	//}
	else if(netType == GameClient_Game)
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if (playerData)
		{
			playerData->doEvent(WCH_JXJCountMonitorPacket,MAKEINT64(m_streamBuf.get(),netType));
		}
	}
}

/**************************************************************************

METHOD: crJXJRestartServerPacket
 
**************************************************************************/

JXJ::crJXJRestartServerPacket::crJXJRestartServerPacket():
	crStreamPacket(crJXJRestartServerPacket::ID)
{

}

JXJ::crJXJRestartServerPacket::crJXJRestartServerPacket( const crJXJRestartServerPacket&p ):
	crStreamPacket(crJXJRestartServerPacket::ID,p)
{

}

void JXJ::crJXJRestartServerPacket::parsePacket( const std::string &sender )
{

	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		connectServer->getNetManager()->getNetBridge()->netErrorLog(0,sender);
		__asm int 3;
	}
}


///////////////////////////////////////////
////
////crJXJChangeBattleStatePacket
////
///////////////////////////////////////////
//crJXJChangeBattleStatePacket::crJXJChangeBattleStatePacket():
//	crStreamPacket(crJXJChangeBattleStatePacket::ID)
//{
//}
//crJXJChangeBattleStatePacket::crJXJChangeBattleStatePacket( const crJXJChangeBattleStatePacket &p ):
//	crStreamPacket(crJXJChangeBattleStatePacket::ID,p)
//{
//}
//void crJXJChangeBattleStatePacket::buildRequestPacket(crJXJChangeBattleStatePacket &packet,int battleid,int roomid,bool bClose)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(9);
//	packet.setStreamBuf(streamBuf);
//	streamBuf->_writeInt(battleid);
//	streamBuf->_writeInt(roomid);
//	streamBuf->_writeBool(bClose);
//}
//void crJXJChangeBattleStatePacket::buildReplyPacket(crJXJChangeBattleStatePacket &packet,CRCore::crStreamBuf * stream)
//{
//	crStreamBuf *streamBuf = new crStreamBuf;
//	streamBuf->createBuf(stream->getRemainSize());
//	packet.setStreamBuf(streamBuf);
//	streamBuf->write(stream->getPtr(),stream->getRemainSize());
//}
//void crJXJChangeBattleStatePacket::parsePacket(const std::string &sender)
//{//
//	if(!m_netConductor) return;
//	int netType = m_netConductor->getNetType();
//	void * param;
//	if(netType == GameServer)
//	{
//		int battleid =  m_streamBuf->_readInt();
//		int roomid = m_streamBuf->_readInt();
//		bool bclose = m_streamBuf->_readBool();
//		_crInt64 battleroomid = MAKEINT64(battleid,roomid);
//		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
//		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
//
//		gsBrainData->getParam(WCHDATA_JXJOpenBattleMap,param);
//		OpenBattleMap *openBattleMap = (OpenBattleMap *)param;
//		gsBrainData->getParam(WCHDATA_JXJCloseBattleSet,param);
//		ClosedBattleSet *closeBattleSet = (ClosedBattleSet *)param;
//
//		OpenBattleMap::iterator itr = openBattleMap->find(battleid);
//		if(itr!=openBattleMap->end())
//		{
//			if(bclose)
//			{
//				closeBattleSet->insert(battleroomid);
//			}
//			else
//			{
//				ClosedBattleSet::iterator itr_close = closeBattleSet->find(battleroomid);
//				if(itr_close!=closeBattleSet->end())
//				{
//					closeBattleSet->erase(itr_close);
//				}
//			}
//		}
//
//		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
//	}
//}

/////////////////////////////////////////
//
//crJXJQueryGmMapPacket
//
/////////////////////////////////////////
crJXJQueryGmMapPacket::crJXJQueryGmMapPacket():
	crStreamPacket(crJXJQueryGmMapPacket::ID)
{
}
crJXJQueryGmMapPacket::crJXJQueryGmMapPacket( const crJXJQueryGmMapPacket &p ):
	crStreamPacket(crJXJQueryGmMapPacket::ID,p)
{
}
void crJXJQueryGmMapPacket::buildReplyPacket(crJXJQueryGmMapPacket &packet,CRCore::crStreamBuf * stream)
{
	packet.setStreamBuf(stream);
}
void crJXJQueryGmMapPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		crGlobalHandle::gData()->lockGMMap();
		GMMap &gmMap = crGlobalHandle::gData()->getGMMap();
		stream->createBuf(5*gmMap.size()+2);
		stream->_writeUShort(gmMap.size());
		for( GMMap::iterator itr = gmMap.begin();
			 itr != gmMap.end();
			 ++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeUChar(itr->second);
		}
		crGlobalHandle::gData()->unlockGMMap();
		crJXJQueryGmMapPacket packet;
		crJXJQueryGmMapPacket::buildReplyPacket(packet,stream.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == SceneServerClient_Game)
	{
		unsigned short count = m_streamBuf->_readUShort();
		crGlobalHandle::gData()->lockGMMap();
		GMMap &gmMap = crGlobalHandle::gData()->getGMMap();
		gmMap.clear();
		int playerid;
		unsigned char authority;
		for (int i = 0; i<count; i++)
		{
			playerid = m_streamBuf->_readInt();
			authority = m_streamBuf->_readUChar();
			gmMap[playerid] = authority;
		}
		crGlobalHandle::gData()->unlockGMMap();
	}
}
/////////////////////////////////////////
//
//crJXJQueryNoChatSetPacket
//
/////////////////////////////////////////
crJXJQueryNoChatSetPacket::crJXJQueryNoChatSetPacket():
	crStreamPacket(crJXJQueryNoChatSetPacket::ID)
{
}
crJXJQueryNoChatSetPacket::crJXJQueryNoChatSetPacket( const crJXJQueryNoChatSetPacket &p ):
	crStreamPacket(crJXJQueryNoChatSetPacket::ID,p)
{
}
void crJXJQueryNoChatSetPacket::buildReplyPacket(crJXJQueryNoChatSetPacket &packet,CRCore::crStreamBuf * stream)
{
	packet.setStreamBuf(stream);
}
void crJXJQueryNoChatSetPacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameServer)
	{
		crData *gameData = crServerBrainHandle::getInstance()->getDataClass();
		gameData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		gameData->getParam(WCHDATA_JXJNoChatSet,param);
		NoChatSet *noChatSet = (NoChatSet *)param;

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		
		stream->createBuf(4*noChatSet->size()+2);
		stream->_writeUShort(noChatSet->size());
		for( NoChatSet::iterator itr = noChatSet->begin();
			itr != noChatSet->end();
			++itr )
		{
			stream->_writeInt(*itr);
		}
		gameData->excHandle(MAKEINT64(WCH_LockData,0));
		crJXJQueryNoChatSetPacket packet;
		crJXJQueryNoChatSetPacket::buildReplyPacket(packet,stream.get());
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == SceneServerClient_Game)
	{
		unsigned short count = m_streamBuf->_readUShort();

		crData *gameData = crServerBrainHandle::getInstance()->getDataClass();
		gameData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		gameData->getParam(WCHDATA_JXJNoChatSet,param);
		NoChatSet *noChatSet = (NoChatSet *)param;
		noChatSet->clear();
		for (int i = 0; i<count; i++)
		{
			noChatSet->insert(m_streamBuf->_readInt());
		}
		gameData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJCheckPlayerGameNamePacket
//
/////////////////////////////////////////
crJXJCheckPlayerGameNamePacket::crJXJCheckPlayerGameNamePacket():
	crStreamPacket(crJXJCheckPlayerGameNamePacket::ID)
{
}

crJXJCheckPlayerGameNamePacket::crJXJCheckPlayerGameNamePacket( const crJXJCheckPlayerGameNamePacket &p ):
	crStreamPacket(crJXJCheckPlayerGameNamePacket::ID,p)
{
}

void crJXJCheckPlayerGameNamePacket::buildRequestPacket(crJXJCheckPlayerGameNamePacket &packet, std::string &name)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	if(name.size()>20)
		name = name.substr(0,20);
	streamBuf->createBuf(4+name.length());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeString(name);
}

void crJXJCheckPlayerGameNamePacket::buildRequestPacket2(crJXJCheckPlayerGameNamePacket &packet,int playerid,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4+stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(playerid);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJCheckPlayerGameNamePacket::buildReplyPacket(crJXJCheckPlayerGameNamePacket &packet,int recvid, char code)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(5);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(recvid);
	streamBuf->_writeChar(code);
}

void crJXJCheckPlayerGameNamePacket::buildReplyPacket2(crJXJCheckPlayerGameNamePacket &packet,CRCore::crStreamBuf *stream)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(stream->getRemainSize());
	packet.setStreamBuf(streamBuf);
	streamBuf->write(stream->getPtr(),stream->getRemainSize());
}
void crJXJCheckPlayerGameNamePacket::parsePacket(const std::string &sender)
{//
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == GameConnectServer)
	{//
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(netDataManager->getPlayerDataByAddress(sender));		
		if(playerData.valid())
		{
			crNetConductor *conductor = playerData->getGameServerLineConductor();
			if(conductor)
			{
				crJXJCheckPlayerGameNamePacket packet;
				crJXJCheckPlayerGameNamePacket::buildRequestPacket2(packet,playerData->getPlayerID(),m_streamBuf.get());
				conductor->getNetManager()->sendPacket("all",packet);
				//CRCore::notify(CRCore::ALWAYS)<<"crQueryGameRoleListPacket "<<std::endl;
			}
		}
	}
	else if(netType == GameServer)
	{
		char code = 0;
		int playerid = m_streamBuf->_readInt();
		crNetDataManager *netDataManager = m_netConductor->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		ref_ptr<crGameServerPlayerData> netPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if(netPlayerData.valid())
		{
			std::string name = m_streamBuf->_readString();
			if(!name.empty()&&name.size()<20)
			{
				bool valid = true;
				if(name.find_first_of("\'\"") != std::string::npos)
					valid = false;
				if(valid)
				{
					ref_ptr<crTableIO> InvalidNameTab;
					InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNameLimitTab);
					if (InvalidNameTab.valid())
					{
						std::wstring wtext = CRIOManager::convertUTF8toUTF16(name.c_str());
						std::wstring invalidstring;
						int ipos = 0;
						int invalidwordsize;
						for (int i = 0; i < InvalidNameTab->getRowCount();i++)
						{
							invalidstring = CRIOManager::convertUTF8toUTF16(InvalidNameTab->getData(i,0).c_str());
							invalidwordsize = invalidstring.length();
							if (invalidwordsize <=  wtext.length())
							{
								ipos = wtext.find(invalidstring);
								if(ipos != std::string::npos)
								{
									valid = false;
									break;
								}
							}
						}
					}
					if(valid)
					{
						InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidWordTab);
						if (InvalidNameTab.valid())
						{
							std::string invalidstring;
							for (int i = 0; i < InvalidNameTab->getRowCount();i++)
							{
								invalidstring = InvalidNameTab->getData(i,0);
								if(invalidstring.size() > 0 && name.find(invalidstring) != std::string::npos)
								{
									valid = false;
									break;
								}
							}
						}
					}
				}
				if(valid && callback->insertCharacterName(playerid,name))
					code = 1;
			}
		}
		crJXJCheckPlayerGameNamePacket packet;
		crJXJCheckPlayerGameNamePacket::buildReplyPacket(packet,playerid,code);
		m_netConductor->getNetManager()->sendPacket(sender,packet);
	}
	else if(netType == GameConnectServerClient_Game)
	{
		int playerid = m_streamBuf->_readInt();
		crNetConductor *connectServer = crNetContainer::getInstance()->getNetConductor(GameConnectServer);
		ref_ptr<crGameConnectServerPlayerData> playerData = dynamic_cast<crGameConnectServerPlayerData *>(connectServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crJXJCheckPlayerGameNamePacket packet;
			crJXJCheckPlayerGameNamePacket::buildReplyPacket2(packet,m_streamBuf.get());
			connectServer->getNetManager()->sendPacket(playerData->getPlayerAdress(),packet);
		}
	}
	else if(netType == GameClient_Game)
	{
		crBrain::getInstance()->doEvent(WCH_RecvCheckPlayerGameName,MAKEINT64(m_streamBuf.get(),NULL));
	}
}


crJXJSceneServerReloadTabPacket::crJXJSceneServerReloadTabPacket():
	crStreamPacket(crJXJSceneServerReloadTabPacket::ID)
{

}

crJXJSceneServerReloadTabPacket::crJXJSceneServerReloadTabPacket( const crJXJSceneServerReloadTabPacket&p ):
	crStreamPacket(crJXJSceneServerReloadTabPacket::ID,p)
{

}
void crJXJSceneServerReloadTabPacket::buildRequestPacket(crJXJSceneServerReloadTabPacket &packet, int tableid)
{
	crStreamBuf *streamBuf = new crStreamBuf;
	streamBuf->createBuf(4);
	packet.setStreamBuf(streamBuf);
	streamBuf->_writeInt(tableid);
}
void crJXJSceneServerReloadTabPacket::parsePacket( const std::string &sender )
{
	if(!m_netConductor) return;
	int netType = m_netConductor->getNetType();
	if(netType == SceneServerClient_Game)
	{
		int tableid = m_streamBuf->_readInt();
		crGlobalHandle::gData()->reloadGlobalTable(tableid);
	}
}