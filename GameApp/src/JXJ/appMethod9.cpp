/**********************************************************************
*
*	文件:	   appMethod9.cpp
*
*	描述:	   
*
*	作者:	   刘超
*					
*
**********************************************************************/

#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod9.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMethod16.h>
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
#include <shellapi.h>
#include <sstream>
#include<wininet.h>
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
#define RE_VALUE(type, value) (*((type *)value))



/************************************************************************/
/*         crJXJChangeGroupLeaderMethod                                                             */
/************************************************************************/

void JXJ::crJXJChangeGroupLeaderMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_groupList= str;
		break;
	case 1:
		//m_validtime = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void JXJ::crJXJChangeGroupLeaderMethod::inputParam( int i, void *param )
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


JXJ::crJXJChangeGroupLeaderMethod::crJXJChangeGroupLeaderMethod():
	m_this(NULL)
{
}

JXJ::crJXJChangeGroupLeaderMethod::crJXJChangeGroupLeaderMethod( const crJXJChangeGroupLeaderMethod & handle ):
	crMethod(handle),
	m_groupList(handle.m_groupList),
	m_newLeader(handle.m_newLeader),
	m_this(NULL)
{
}

void crJXJChangeGroupLeaderMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor && canvas.valid())
		{
			ref_ptr<crTableWidgetNode> grouplist = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));

			crData * data = playerGameData->getDataClass();
			if( grouplist.valid() && data)
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					int curselectplayerid = grouplist->getSelectData();
					if (curselectplayerid != playerGameData->getPlayerID())
					{
						stream->createBuf(4);
						stream->_writeInt(curselectplayerid);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeGroupLeader,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
					else playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4062,NULL));
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvChangeGroupLeaderMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvChangeGroupLeaderMethod::crJXJRecvChangeGroupLeaderMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvChangeGroupLeaderMethod::crJXJRecvChangeGroupLeaderMethod( const crJXJRecvChangeGroupLeaderMethod & handle ):
	crMethod(handle),
	m_netType(handle.m_netType),
	m_stream(NULL),
	m_this(NULL)
{

}

void JXJ::crJXJRecvChangeGroupLeaderMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int newGroupLeader = m_stream->_readInt();

			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0 && playerid != newGroupLeader)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					ref_ptr<crJXJConsortia> consortia;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					if (itr != consortiamap->end())
					{
						consortia = itr->second;
					}
					if(consortia.valid())
					{
						//权限管理
						crData *consortiaData = consortia->getDataClass();
						if (consortiaData)
						{
							consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
							ConsortiaMemberMap * consortiaMemberMap = (ConsortiaMemberMap *) param;
							if (consortia->checkPermission(playerid,JXJGroup_ChangeLeader))
							{
								ConsortiaMemberMap::iterator it = consortiaMemberMap->find(newGroupLeader);
								ConsortiaMemberMap::iterator myIt = consortiaMemberMap->find(playerid);

								if (it != consortiaMemberMap->end() && myIt != consortiaMemberMap->end())
								{
									success = 1;
									myIt->second->setGroupPos(JXJGroup_Normal);
									it->second->setGroupPos(JXJGroup_FirstLeader);
									consortia->updateLeaderVec();

									//去除新军团长被分封的城池
									consortiaData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
									FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
									for (FenFengedChengChiMap::iterator ffccmitr = fenFengedChengChiMap->begin();ffccmitr != fenFengedChengChiMap->end();++ffccmitr)
									{
										if (ffccmitr->second == newGroupLeader)
										{
											fenFengedChengChiMap->erase(ffccmitr);
											break;
										}
									}

									consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&success);
									consortia->updateToRank(gsBrainData);

									ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									crTableIO::StrVec textrecord;
									char szNotice[256];
									int textid = 4050;
									if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
									{
										sprintf_s(szNotice,textrecord[1].c_str(), it->second->getName().c_str());
										CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
										crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									}
									//ref_ptr<JXJChangeGroupLeaderMsg > msg =  new JXJChangeGroupLeaderMsg;
									//msg->m_name = it->second->getName();
									//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(msg.get(),mygroupid));

									ref_ptr<crGameServerPlayerData> playerData2 =
										dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(it->first));

									stream->createBuf(2);
									stream->_writeUChar(success);
									stream->_writeUChar(JXJGroup_Normal);
									stream->_writeString(it->second->getName());
									stream->_writeInt(it->second->getLevel());
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeGroupLeader,stream.get());
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

									if (playerData2.valid())
									{
										stream->seekBegin();
										stream->_writeUChar(success);
										stream->_writeUChar(JXJGroup_FirstLeader);
										stream->_writeString(it->second->getName());
										stream->_writeInt(it->second->getLevel());
										crPlayerDataEventPacket::buildReplyPacket(packet,it->first,WCH_JXJRecvChangeGroupLeader,stream.get());
										gameServer->getNetManager()->sendPacket(playerData2->getPlayerConnectServerAddress(),packet);
									}
								}
							}
						}
					}
					
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_ChangeGroupLeader,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success)
			{
				unsigned char newGroupPos = m_stream->_readUChar();
				std::string newLeader = m_stream->_readString();
				int newLeaderLv = m_stream->_readInt();
				void * param;
				ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (canvas.valid())
				{
					crData *canvasdata = canvas->getDataClass();
					if (canvasdata)
					{
						CRCore::ScopedLock<crData> lock(*canvasdata);
						canvasdata ->getParam(WCHDATA_JXJMyGroupInfo,param);
						crJXJGroupInfo * groupInfo = (crJXJGroupInfo *)param;
						GroupLeaderInfoVec & leaderVec = groupInfo->getLeaderVec();
						if (leaderVec.empty() == false)
						{
							leaderVec[0].first = newLeader;
							leaderVec[0].second = newLeaderLv;
						}

						canvasdata ->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
						crJXJConsortiaMember * mySelf = (crJXJConsortiaMember *)param;
						if (mySelf)
						{
							mySelf->setGroupPos(newGroupPos);
						}
						m_this->doEvent(WCH_JXJQueryGroupMemberList);
					}
					canvas->doEvent(WCH_UI_UpdateData);
				}
			}
			else
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2027,NULL));//修改失败
			}
		}
	}
}

void JXJ::crJXJRecvChangeGroupLeaderMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvChangeGroupLeaderMethod::addParam( int i, const std::string& str )
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

/************************************************************************/
/*    crJXJRecvDismissGroupMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvDismissGroupMethod::crJXJRecvDismissGroupMethod():
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvDismissGroupMethod::crJXJRecvDismissGroupMethod( const crJXJRecvDismissGroupMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_netType(handle.m_netType),
	m_this(NULL),
	m_stream(NULL)
{
}

void JXJ::crJXJRecvDismissGroupMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;

			unsigned char type = m_stream->_readUChar();

			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					int dismissTime = -1;
					ref_ptr<crJXJConsortia> consortia;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					if (itr != consortiamap->end())
					{
						consortia = itr->second;
					}
					if(consortia.valid())
					{
						if (consortia->checkPermission(playerid,JXJGroup_DismissGroup))
						{
							crData * consortiaData = consortia->getDataClass();
							consortiaData->getParam(WCHDATA_JXJConsortiaDismissTime,param);
							int lastTime = *(int *)param;
							switch (type)
							{
							case JXJDismissGroupType_normal:
								if (-1 == lastTime)
								{
									dismissTime = 24 * 60 * 60;
									consortiaData->inputParam(WCHDATA_JXJConsortiaDismissTime,&dismissTime);
									success = 1;
								}
								break;
							case JXJDismissGroupType_immediately:
								{
									dismissTime = 0;
									consortiaData->inputParam(WCHDATA_JXJConsortiaDismissTime,&dismissTime);
									success = 1;
									//删除playerdata 里的 城池特产
									data->getParam(WCHDATA_JXJGroupCantonSpecialtyVec,param);
									GroupCantonSpecialtyVec *gcspecialtyVec = (GroupCantonSpecialtyVec *)param;
									for( GroupCantonSpecialtyVec::iterator gvitr = gcspecialtyVec->begin();
										gvitr != gcspecialtyVec->end();
										++gvitr )
									{
										*gvitr = NULL;
									}
									//删除已通关副本
									data->getParam(WCHDATA_JXJHadWonGroupFuBenSet,param);
									HadWonGroupFuBenSet * hadWonGroupFuBenSet = (HadWonGroupFuBenSet *)param;
									hadWonGroupFuBenSet->clear();
									break;
								}
							default:
								break;
							}
						}
					}
					int size = 1;
					if (success) 
					{
						size += 4;
					}

					stream->createBuf(size);
					stream->_writeUChar(success);
					if (1 == success)
					{
						stream->_writeInt(dismissTime);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvDismissGroup,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_DimissGroup,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success)
			{
				int dismissTime = m_stream->_readInt();

				void * param;
				ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (canvas.valid())
				{
					crData *canvasdata = canvas->getDataClass();
					if (canvasdata)
					{
						canvasdata ->getParam(WCHDATA_JXJMyGroupInfo,param);
						crJXJGroupInfo *groupinfo = (crJXJGroupInfo *)param;
						groupinfo->setDismissTime(dismissTime);

						if (dismissTime == 0)
						{
							crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
							crData * data = playerGameData->getDataClass();
							if( data)
							{
								int mygroupid = 0;
								data-> inputParam(WCHDATA_JXJConsortiaID,&mygroupid);
								playerGameData->doEvent(WCH_JXJUIShowGroupCanvas,NULL);
								canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
								StartedFubenMap *startedFubenMap = (StartedFubenMap *)param;
								startedFubenMap->clear();
								canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
								MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
								groupCantonVec->clear();
								canvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
								MyApplyCantonVec * myApplyCantonVec = (MyApplyCantonVec *)param;
								myApplyCantonVec->clear();
							}
						}

						//m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2026,NULL));//修改成功
					}
				}
			}
			else
			{
				//m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2027,NULL));//修改失败
			}
		}
	}
}

void JXJ::crJXJRecvDismissGroupMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvDismissGroupMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
	default:
		break;
	}
}

/************************************************************************/
/*         crJXJCancelDismissGroupMethod                                                             */
/************************************************************************/

void JXJ::crJXJCancelDismissGroupMethod::inputParam( int i, void *param )
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
		m_this =(crWidgetNode*)param;
		break;
	default:
		break;
	}
}

JXJ::crJXJCancelDismissGroupMethod::crJXJCancelDismissGroupMethod():
	m_netType(GameClient_Game )
{

}

JXJ::crJXJCancelDismissGroupMethod::crJXJCancelDismissGroupMethod( const crJXJCancelDismissGroupMethod & handle ):
	crMethod(handle),
	m_netType(GameClient_Game),
	m_btn(handle.m_btn)
{
}

void JXJ::crJXJCancelDismissGroupMethod::operator()( crHandle &handle )
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor && canvas.valid())
		{
			//ref_ptr<crTableWidgetNode>  grouplist = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));
			crData * data = playerGameData->getDataClass();
			if( data )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(mygroupid);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCancelDismissGroup,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}

void JXJ::crJXJCancelDismissGroupMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_btn = str;
		break;
	case 1:
		//m_validtime = atoi(str.c_str());
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJRecvCancelDismissGroupMethod                                                                  */
/************************************************************************/

inline JXJ::crJXJRecvCancelDismissGroupMethod::crJXJRecvCancelDismissGroupMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvCancelDismissGroupMethod::crJXJRecvCancelDismissGroupMethod( const crJXJRecvCancelDismissGroupMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(handle.m_netType),
	m_canvas(handle.m_canvas)

{
}

void JXJ::crJXJRecvCancelDismissGroupMethod::operator()( crHandle & handle )
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
			if (playerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					itr = consortiamap->find(mygroupid);
					int dismissTime = -1;
					if (itr != consortiamap->end())
					{
						if (itr->second->checkPermission(playerid,JXJGroup_CancelDismissGroup))
						{
							crData * consortiaData = itr->second->getDataClass();
							consortiaData->inputParam(WCHDATA_JXJConsortiaDismissTime,&dismissTime);
							success = 1;
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					stream->createBuf(1);
					stream->_writeUChar(success);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCancelDismissGroup,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_CancelDimissGroup,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success)
			{
				int dismissTime = -1;

				void * param;
				ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (canvas.valid())
				{
					crData *canvasdata = canvas->getDataClass();
					if (canvasdata)
					{
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata ->getParam(WCHDATA_JXJMyGroupInfo,param);
						crJXJGroupInfo *groupinfo = (crJXJGroupInfo *)param;
						groupinfo->setDismissTime(dismissTime);
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3033,NULL));//修改成功
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
			else
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2027,NULL));//修改失败
			}
		}
	}
}

void JXJ::crJXJRecvCancelDismissGroupMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvCancelDismissGroupMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJAddGroupMemberMethod                                                                  */
/************************************************************************/

JXJ::crJXJAddGroupMemberMethod::crJXJAddGroupMemberMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJAddGroupMemberMethod::crJXJAddGroupMemberMethod( const crJXJAddGroupMemberMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_input(handle.m_input),
	m_netType(GameClient_Game)
{

}

void JXJ::crJXJAddGroupMemberMethod::operator()( crHandle &handle )
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor && canvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> nameInput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
			std::wstring newMemberName  = nameInput->getString();
			crData * data = playerGameData->getDataClass();
			if(data)
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(newMemberName.length());
					std::string name = CRIOManager::convertUTF16toUTF8(newMemberName.c_str());
					stream->_writeString(name);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddGroupMember,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}

void JXJ::crJXJAddGroupMemberMethod::inputParam( int i, void *param )
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
		m_this = (CRCore::crCanvasNode *)param;
		break;
	default:
		break;
	}
}

void JXJ::crJXJAddGroupMemberMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJRecvAddGroupMemberMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvAddGroupMemberMethod::crJXJRecvAddGroupMemberMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvAddGroupMemberMethod::crJXJRecvAddGroupMemberMethod( const crJXJRecvAddGroupMemberMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJRecvAddGroupMemberMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			if(!m_this->getMainRole())
				return;
			std::string nickname = m_stream->_readString();
			int myplayerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			std::string myname = m_this->getMainRole()->getIName2();
			unsigned char result = JXJGroupAddMember_unknow	;
			int playerid2 = 0;
			void *param;
			ref_ptr<crGameServerPlayerData> playerData;
			std::string playername;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			if (myname != nickname)
			{
				ref_ptr<crGameServerPlayerData> playerData2;
				bool found = false;
				netDataManager->lockPlayerDataMap();//WCH 根据CharacterName找玩家
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();++itr )
				{
					playerData2 = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if(playerData2.valid() 
						&& playerData2->getCharacterName() == nickname)
					{
						found = true;
						break;
					}
				}
				netDataManager->unLockPlayerDataMap();

				do 
				{
					if ( !found 
						|| !(playerData2->getPlayerGameData()) 
						|| !(playerData2->getPlayerGameData()->getDataClass())
						)
					{
						result = JXJGroupAddMember_NotExist;
						break;
					}
					data->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myshiliId = *(unsigned char *) param;
					crData * data2 = playerData2->getPlayerGameData()->getDataClass();
					data2->getParam(WCHDATA_JXJShiliID,param);
					unsigned char shiliId = *(unsigned char *)param;
					if (shiliId != myshiliId)
					{
						result = JXJGroupAddMember_differCounty;
						break;
					}
					playerid2 = playerData2->getPlayerID();
					int groupid1 = 0;
					int groupid2 = 0;
					data->getParam(WCHDATA_JXJConsortiaID,param);
					groupid1 = *(int *)param;
					data2->getParam(WCHDATA_JXJConsortiaID,param);
					groupid2 = *(int *)param;

					if (!groupid1) break;

					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					ref_ptr<crData>consortiaData;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator it = consortiamap->find(groupid1);
					if (it == consortiamap->end()) break;
					if ( !(it->second->checkPermission(myplayerid,JXJGroup_AddGroupMember)) )
					{
						result = JXJGroupAddMember_NoPermission;
						break;
					}
					consortiaData = it->second->getDataClass();
					
					if (!consortiaData.valid()) break;
					consortiaData->getParam(WCHDATA_JXJConsortialv,param);
					unsigned char groupLv = *(unsigned char *)param;
					
					consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					ConsortiaMemberMap * consortiaMemberMap = (ConsortiaMemberMap *) param;
					ref_ptr<crTableIO>  groupUpgradeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);
					crTableIO::StrVec record;
					if (false == groupUpgradeTab.valid()) break;
					int memeberNumIdx = groupUpgradeTab->getTitleIndex("成员上限");
					if (groupUpgradeTab->queryOneRecord(0,crArgumentParser::appItoa(groupLv),record) < 0) break;

					if (groupid1 == groupid2)
					{
						result = JXJGroupAddMember_InSameGroup;
						break;
					}
					if (consortiaMemberMap->size() >= atoi(record[memeberNumIdx].c_str()))
					{
						result = JXJGroupAddMember_Full;
						break;
					}
					if (groupid2)
					{
						result = JXJGroupAddMember_HasGroup;
						break;
					}

					result = JXJGroupAddMember_Wait;
					gsBrainData->getParam(WCHDATA_JXJGroupAddMemberDisposeDeq,param);
					GroupAddMemberDeq * groupAddMemberDeq = (GroupAddMemberDeq *)param;

					GroupAddMemberDeq::iterator gamdIt = groupAddMemberDeq->begin();
					for (; gamdIt != groupAddMemberDeq->end(); ++ gamdIt )
					{
						if ((*gamdIt )[0] == playerid2 )
						{
							if ((*gamdIt)[1] == groupid1)
							{
								break;
							}
						}
					}
					if ( gamdIt == groupAddMemberDeq->end())
					{
						groupAddMemberDeq->push_back(crVector3(playerid2,groupid1,myplayerid));
					}
				} while (0);
			}
			else
			{
				result = JXJGroupAddMember_SameName;
			}

			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(myplayerid));
			if(playerData.valid())
			{
				crPlayerDataEventPacket recvpacket;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(result);
				crPlayerDataEventPacket::buildReplyPacket(recvpacket,myplayerid,WCH_JXJRecvAddGroupMember,stream.get());
				netManager->sendPacket(playerData->getPlayerConnectServerAddress(),recvpacket);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int msgid = 0;
			int result = m_stream->_readInt();
			switch (result)
			{
			case JXJGroupAddMember_Success:
				{
					crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
					if(playerGameData)
					{
						crData *data = playerGameData->getDataClass();
						if (data)
						{
							int mygroupid = m_stream->_readInt();
							data->inputParam(WCHDATA_JXJConsortiaID,&mygroupid);
							playerGameData->doEvent(WCH_JXJUIShowGroupCanvas);
						}
					}
				}
				break;
			case JXJGroupAddMember_SameName:
				msgid = 3035;
				break;
			case JXJGroupAddMember_Offline:
				msgid = 3036;
				break;
			case JXJGroupAddMember_NotExist:
				msgid = 3036;
				break;
			case JXJGroupAddMember_NoPermission:
				msgid = 3037;
				break;
			case JXJGroupAddMember_InSameGroup:
				msgid = 3038;
				break;
			case JXJGroupAddMember_Full:
				msgid = 3039;
				break;
			case JXJGroupAddMember_HasGroup:
				msgid = 3040;
				break;
			case JXJGroupAddMember_Wait:
				msgid = 3041;
				break;
			case JXJGroupAddMember_Disagree:
				msgid = 3042;
				break;
			case JXJGroupAddMember_Agree:
				msgid = 3043;
				break;
			case JXJGroupAddMember_differCounty:
				msgid = 4051;
				break;
			default:
				break;
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}

void JXJ::crJXJRecvAddGroupMemberMethod::inputParam( int i, void *param )
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
		m_this = (crPlayerGameData *)param;
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

void JXJ::crJXJRecvAddGroupMemberMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJAddGroupMemberDisposeMethod                                                                  */
/************************************************************************/

JXJ::crJXJAddGroupMemberDisposeMethod::crJXJAddGroupMemberDisposeMethod():
	m_this(NULL)
{

}

JXJ::crJXJAddGroupMemberDisposeMethod::crJXJAddGroupMemberDisposeMethod( const crJXJAddGroupMemberDisposeMethod& handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJAddGroupMemberDisposeMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJAddGroupMemberDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJAddGroupMemberDisposeMethod::operator()( crHandle &handle )
{
	crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
	if(dbConductor)
	{
		void *param;
		crData *data = m_this->getDataClass();

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetManager *netManager = gameServer->getNetManager();

		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJGroupAddMemberDisposeDeq,param);
		GroupAddMemberDeq *disposeDeq = (GroupAddMemberDeq *)param;
		data->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = ( ConsortiaMap *)param;
		if(!disposeDeq->empty())
		{
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData;
//			crPlayerGameData *playerGameData;
//			crData *gameData;

			int invitedplayerid = 0;		// 被邀请者的id
			int groupid = 0;
			int playerid = 0;				// 邀请者的id
			std::string groupName;
			crDBSavePacket packet;
			while(!disposeDeq->empty())
			{
				invitedplayerid = disposeDeq->front()[0];
				groupid = disposeDeq->front()[1];
				playerid = disposeDeq->front()[2];
				
				disposeDeq->pop_front();
				playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(invitedplayerid));
				if(playerData.valid())
				{
					ConsortiaMap::iterator findIt = consortiamap->find(groupid);
					if (findIt != consortiamap->end())
					{
						findIt->second->getName(groupName);
						crPlayerDataEventPacket recvpacket;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(28);
						stream->_writeInt(groupid);
						stream->_writeInt(playerid);		
						stream->_writeString(groupName);
						crPlayerDataEventPacket::buildReplyPacket(recvpacket,invitedplayerid,WCH_JXJRecvInvitedAddGroupMember,stream.get());
						netManager->sendPacket(playerData->getPlayerConnectServerAddress(),recvpacket);
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/************************************************************************/
/*    crJXJClientInvitedAddGroupMemberMethod                                                                  */
/************************************************************************/

JXJ::crJXJClientInvitedAddGroupMemberMethod::crJXJClientInvitedAddGroupMemberMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJClientInvitedAddGroupMemberMethod::crJXJClientInvitedAddGroupMemberMethod( const crJXJClientInvitedAddGroupMemberMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_agree(handle.m_agree),
	m_this(NULL)
{
}

void JXJ::crJXJClientInvitedAddGroupMemberMethod::operator()( crHandle & handle )
{	
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer && myPlayer->getDataClass())
	{
		crData *data = myPlayer->getDataClass();
		int groupid = 0;
		int playerid = 0;

		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (/*input && */netConductor)
		{
			std::string nickname,nameprompt;
			//nickname = input->getUTF8String();
			//crData *data = canvas->getDataClass();
			CRCore::ScopedLock<crData> lock(*data);
			data->getParam(WCHDATA_JXJAddGroupMemberRequestDeq,param);
			AddGroupMemberRequestDeq *addGroupMemberRequestDeq = (AddGroupMemberRequestDeq *)param;
			if (!addGroupMemberRequestDeq->empty())
			{
				groupid = addGroupMemberRequestDeq->front().first;
				playerid = addGroupMemberRequestDeq->front().second.first;
				addGroupMemberRequestDeq->pop_front();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(12);
				stream->_writeInt(groupid);
				stream->_writeInt(playerid);
				if (m_agree)
				{
					stream->_writeInt(JXJGroupAddMember_Agree);
				}
				else
				{
					stream->_writeInt(JXJGroupAddMember_Disagree);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddGroupMemberResult,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
	//handle.outputParam(0,&cancelTask);
}

void JXJ::crJXJClientInvitedAddGroupMemberMethod::inputParam( int i, void *param )
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

void JXJ::crJXJClientInvitedAddGroupMemberMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_agree = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJRecvInvitedAddGroupMemberMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvInvitedAddGroupMemberMethod::crJXJRecvInvitedAddGroupMemberMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJRecvInvitedAddGroupMemberMethod::crJXJRecvInvitedAddGroupMemberMethod( const crJXJRecvInvitedAddGroupMemberMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_lastname(handle.m_lastname),
	m_agree(handle.m_agree)

{
}

void JXJ::crJXJRecvInvitedAddGroupMemberMethod::operator()( crHandle & handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			bool canshow = false;
			int groupid= m_stream->_readInt();
			int playerid = m_stream->_readInt();
			std::string groupName = m_stream->_readString();
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (playerGameData && canvasNode.valid() && playerGameData->getDataClass())
			{
				if (canvasNode->getVisiable() && groupName.compare(m_lastname) != 0)
				{
					canshow = true;
					m_lastname = groupName;
				}
				if (!canvasNode->getVisiable())
				{
					canshow = true;
				}
				if (canshow)
				{
					//ref_ptr<crStaticTextWidgetNode> inputname = dynamic_cast<crStaticTextWidgetNode *>(canvasNode->getWidget(m_sendname));
					//ref_ptr<crStaticTextWidgetNode> inputtext = dynamic_cast<crStaticTextWidgetNode *>(canvasNode->getWidget(m_text));
					//std::string str = "请求加你为军团成员!";
					//if(inputname)inputname->setString("军团【"+ nickname+"】" + str);
					void *param;
					crData *data = playerGameData->getDataClass();
					CRCore::ScopedLock<crData> lock(*data);
					data->getParam(WCHDATA_JXJAddGroupMemberRequestDeq,param);
					AddGroupMemberRequestDeq *addGroupMemberRequestDeq = (AddGroupMemberRequestDeq *)param;
					addGroupMemberRequestDeq->push_back(std::make_pair(groupid,std::make_pair(playerid,groupName)));
					canvasNode->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

void JXJ::crJXJRecvInvitedAddGroupMemberMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvInvitedAddGroupMemberMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	}
}


/************************************************************************/
/*    crJXJAddGroupMemberRequestUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJAddGroupMemberRequestUpdateMethod::crJXJAddGroupMemberRequestUpdateMethod():
	m_this(NULL)	
{

}

JXJ::crJXJAddGroupMemberRequestUpdateMethod::crJXJAddGroupMemberRequestUpdateMethod( const crJXJAddGroupMemberRequestUpdateMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_text(handle.m_text)
{

}

void JXJ::crJXJAddGroupMemberRequestUpdateMethod::operator()( crHandle &handle )
{
	if(m_this)
	{
		void *param;
		int count = 0;

		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		//std::string str1 = "邀请加入军团";
		std::string str2 = "邀请你成为军团一员";
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid()&&  myPlayer && myPlayer->getDataClass())
		{
			ref_ptr<crHypertextWidgetNode> inputText = dynamic_cast<crHypertextWidgetNode*>(canvas->getWidget(m_text));

			crData *data = myPlayer->getDataClass();
			CRCore::ScopedLock<crData> lock(*data);
			data->getParam(WCHDATA_JXJAddGroupMemberRequestDeq,param);
			AddGroupMemberRequestDeq *addGroupMemberRequestDeq = (AddGroupMemberRequestDeq *)param;
			AddGroupMemberRequestDeq::iterator itr;
			if (!addGroupMemberRequestDeq->empty())
			{
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("DrawMode","TEXT");
				std::string groupName =  addGroupMemberRequestDeq->front().second.second;
				cfg_script.Add("Text","[军 团] " + groupName + " " + str2);
				cfg_script.Pop();
				cfg_script.Pop();
				inputText->setHypertext(cfg_script);
		
				bool ifshow = canvas->getVisiable();
				if(!ifshow)
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	}
}


void JXJ::crJXJAddGroupMemberRequestUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJAddGroupMemberRequestUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_text = str;
		break;
	}
}

/************************************************************************/
/*    crJXJRecvAddGroupMemberResultMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvAddGroupMemberResultMethod::crJXJRecvAddGroupMemberResultMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvAddGroupMemberResultMethod::crJXJRecvAddGroupMemberResultMethod( const crJXJRecvAddGroupMemberResultMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)

{
}

void JXJ::crJXJRecvAddGroupMemberResultMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			if(!m_this->getMainRole())
				return;
			crData *data = m_this->getDataClass();
			std::string myname = m_this->getMainRole()->getIName2();
			unsigned char result = 0;
			void *param;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData;

			int invitedplayerid = m_this->getPlayerID();
			int groupid	= m_stream->_readInt();
			int playerid = m_stream->_readInt();
			int agreeStat = m_stream->_readInt();
			std::string groupName ;

			CRCore::ScopedLock<crData> lock(*data);
			char tmp[64];
			switch (agreeStat)
			{
			case JXJGroupAddMember_Agree:
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					ref_ptr<crJXJConsortia> consortia;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator it = consortiamap->find(groupid);
					if (it != consortiamap->end())
					{
						consortia = it->second;
					}
					if(consortia.valid())
					{
						crData * consortiaData = consortia->getDataClass();
						consortia->getName(groupName);
						consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap * consortiaMemberMap = (ConsortiaMemberMap *) param;
						if (consortiaMemberMap)
						{
							if (consortiaMemberMap->size() >= 4)
							{
								result = JXJGroupAddMember_Full;
							}
							else
							{
								result = JXJGroupAddMember_Success;

								time_t t = time(0); 
								strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 

								crData * mainRoleData = m_this->getMainRole()->getDataClass();
								CRCore::ScopedLock<crData> lock(*mainRoleData);

								data->inputParam(WCHDATA_JXJConsortiaID,&groupid);

								mainRoleData->getParam(WCHDATA_Level,param);
								unsigned char mylv = *(unsigned char *)param;
								mainRoleData->getParam(WCHDATA_JXJGovPost,param);
								unsigned char govPost = *(unsigned char *)param;
								data->getParam(WCHDATA_JXJMyApplyGroupIDSet,param);
								MyGroupApplySet * myGroupApplySet = (MyGroupApplySet *) param;
								myGroupApplySet->clear();

								ref_ptr<crJXJConsortiaMember> consortiamemberinfo = new crJXJConsortiaMember(invitedplayerid);
								consortiamemberinfo->setLevel(mylv);
								consortiamemberinfo->setName(myname);
								consortiamemberinfo->setPos(govPost);
								consortiamemberinfo->setGroupPos(JXJGroup_Normal);
								consortiamemberinfo->setActiveValue(0);
								consortiamemberinfo->setTime(t);

								consortiaMemberMap->insert(ConsortiaMemberMap::value_type(invitedplayerid,consortiamemberinfo));

								bool modify = true;
								consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);

								ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
								crTableIO::StrVec textrecord;
								char szNotice[256];
								int textid = 3073;
								if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
								{
									sprintf_s(szNotice,textrecord[1].c_str(), myname.c_str());
									CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,groupid);
									gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
									crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
									gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
								}
								//ref_ptr<JXJGroupAddNewmemberMsg> chatMsg = new JXJGroupAddNewmemberMsg;
								//chatMsg->m_name = myname;
								//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(chatMsg.get(),groupid));
							}
						}
					}
					else
					{
						result = JXJGroupAddMember_NotExist;
					}
				}
				break;
			case JXJGroupAddMember_Disagree:
				{
					result = JXJGroupAddMember_Disagree;
				}
				break;
			default:
				break;
			}
			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(invitedplayerid));
			if (playerData.valid())
			{
				crPlayerDataEventPacket recvpacket;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				switch (result)
				{
				case JXJGroupAddMember_Success:
					{
						stream->createBuf(4);
						stream->_writeInt(result);
						stream->_writeInt(groupid);
					}
					break;
				case JXJGroupAddMember_NotExist:
				case JXJGroupAddMember_Full:
					{
						stream->createBuf(sizeof(result) + myname.length() + groupName.length());
						stream->_writeInt(result);
						stream->_writeString(myname);
						stream->_writeString(groupName);
					}
					break;
				default:
					break;
				}
				crPlayerDataEventPacket::buildReplyPacket(recvpacket,invitedplayerid,WCH_JXJRecvAddGroupMember,stream.get());
				netManager->sendPacket(playerData->getPlayerConnectServerAddress(),recvpacket);
			}
		}
	}
}

void JXJ::crJXJRecvAddGroupMemberResultMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvAddGroupMemberResultMethod::addParam( int i, const std::string& str )
{
}

/************************************************************************/
/*    crJXJClientChangeGroupNameMethod                                                                  */
/************************************************************************/

JXJ::crJXJClientChangeGroupNameMethod::crJXJClientChangeGroupNameMethod():
	m_validtime(5),
	m_lasttime(0)
{

}

JXJ::crJXJClientChangeGroupNameMethod::crJXJClientChangeGroupNameMethod( const crJXJClientChangeGroupNameMethod& handle ):
	crMethod(handle),
	m_nameedit(handle.m_nameedit),
	m_validtime(handle.m_validtime),
	m_lasttime(handle.m_lasttime)
{


}

void JXJ::crJXJClientChangeGroupNameMethod::inputParam( int i, void *param )
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

void JXJ::crJXJClientChangeGroupNameMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_nameedit = str;
		break;
	case 1:
		m_validtime = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJClientChangeGroupNameMethod::operator ()( crHandle & handle )
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = playerGameData->getDataClass();
	data-> getParam(WCHDATA_JXJConsortiaID,param);
	int mygroupid = *( int *)param;
	if (mygroupid > 0)
	{
		bool ifcansave = false;
		time_t t1 = time(0);
		if (t1 - m_lasttime >= m_validtime)
		{
			m_lasttime = t1;
			ifcansave = true;
		}
		else
		{
			playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2025,NULL));//过于频繁
		}
		if (ifcansave)
		{
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor && canvas.valid())
			{
				ref_ptr<crEditWidgetNode> editwidget = dynamic_cast<crEditWidgetNode *>(canvas -> getWidget(m_nameedit));
				if( editwidget.valid())
				{
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					std::string newGroupName = editwidget->getUTF8String();
					int maxsize = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupNameMaxSize,viplv).c_str());
					if(newGroupName.empty())
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(1099,NULL));//名字不能为空！
					}
					else if (newGroupName.length() > maxsize)
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2023,NULL));//名称过长
					}
					else
					{
						int success = 1;
						playerGameData->doEvent(WCH_JXJCheckInvalidWords,MAKEINT64(&success,&newGroupName));
						if (success == 1)
						{
							playerGameData->doEvent(WCH_JXJCheckNameLimit,MAKEINT64(&success,&newGroupName));
							if (success == 1)
							{
								//std::string name = crArgumentParser::trim(const_cast<char *>(newGroupName.c_str()));
								//if (name.empty())
								//{
								//	m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2021,NULL));//名称不能含有敏感词
								//}
								//else
								//{
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(newGroupName.length()+4);
									stream->_writeString(newGroupName);
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeGroupName,stream.get());
									netConductor->getNetManager()->sendPacket("all",packet);
								//}
							}
							else
							{
								m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2021,NULL));//名称不能含有敏感词
							}
						}
						else
						{
							m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2021,NULL));//名称不能含有敏感词
						}
					}
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvChangeGroupNameMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvChangeGroupNameMethod::crJXJRecvChangeGroupNameMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvChangeGroupNameMethod::crJXJRecvChangeGroupNameMethod( const crJXJRecvChangeGroupNameMethod& handle ):
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game),
	m_canvas(handle.m_canvas),
	crMethod(handle)
{
}

void JXJ::crJXJRecvChangeGroupNameMethod::operator ()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = JXJGroupChangeNameRetVal_failed_unkwon;
			std::string newGroupName = m_stream->_readString();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (data && playerData.valid() )
			{
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				success = JXJGroupChangeNameRetVal_failed_valid;
				do 
				{
					if(newGroupName.empty())
					{
						success = JXJGroupChangeNameRetVal_failed_empty;
						break;
					}
					int maxsize = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupNameMaxSize,viplv).c_str());
					if(newGroupName.length() > maxsize)
					{
						success = JXJGroupChangeNameRetVal_failed_size;
						break;
					}
					int validcheck = 1;
					m_this->doEvent(WCH_JXJCheckInvalidWords,MAKEINT64(&success,&newGroupName));
					if (validcheck == 1)
					{
						m_this->doEvent(WCH_JXJCheckNameLimit,MAKEINT64(&success,&newGroupName));
						if (validcheck == 1)
						{
							success = JXJGroupChangeNameRetVal_failed_unkwon;
						}
					}
				} while (0);

				std::string oldGroupName;
				int changeNameCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupChangeNameCost,viplv).c_str());
				data->getParam(WCHDATA_JXJGoldingot,param);
				int yuanBao = *(int *)param;

				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0 && success == JXJGroupChangeNameRetVal_failed_unkwon)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					ref_ptr<crJXJConsortia> consortia;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					bool sameName = false;
					ConsortiaMap::iterator cmIt = consortiamap->begin();
					std::string groupName;
					for (;cmIt != consortiamap->end(); ++ cmIt)
					{
						cmIt->second->getName(groupName);
						if (groupName == newGroupName)
						{
							sameName = true;
							break;
						}
					}
					if (sameName)
					{
						success = JXJGroupChangeNameRetVal_failed_repreat;
					}
					else 
					{
						ConsortiaMap::iterator itr;
						itr = consortiamap->find(mygroupid);
						if (itr != consortiamap->end())
						{
							consortia = itr->second;
						}
					}
					if(consortia.valid())
					{
						if (consortia->checkPermission(playerid,JXJGroup_ChangeName))
						{
							bool needdeductgold = false;
							MoneyChangeData moneydata(changeNameCost,"军团改名");
							m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
							if (-1 == moneydata.first)
							{
								success = JXJGroupChangeNameRetVal_failed_noYuanBao;
							}
							else if (0 == moneydata.first)
							{
								crData *consortiaData = consortia->getDataClass();
								if (consortiaData)
								{
									consortia->getName(oldGroupName);
									consortia->setName(newGroupName);
									consortia->updateToRank(gsBrainData);
									success = JXJGroupChangeNameRetVal_suc;
									bool modify = true;
									consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
								}	
							}
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(5);
					stream->_writeUChar(success);
					if (JXJGroupChangeNameRetVal_suc == success)
					{
						stream->_writeString(newGroupName);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeGroupName,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				if (JXJGroupChangeNameRetVal_suc == success)
				{
					///游戏日志
					std::string logdata = "军团："+oldGroupName+"改名为："+newGroupName;
					GameLogData gamelog(Log_ChangeGroupName,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			switch (success)
			{
			case JXJGroupChangeNameRetVal_suc :
				{
					std::string newGroupName = m_stream->_readString();
					void *param;
					ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if (groupCanvas.valid() && groupCanvas->getDataClass())
					{
						crData * groupData = groupCanvas->getDataClass();
						groupData->getParam(WCHDATA_JXJMyGroupInfo,param);
						crJXJGroupInfo * groupInfo = (crJXJGroupInfo *)param;
						groupInfo->setName(newGroupName);
						groupCanvas->doEvent(WCH_UI_UpdateData);
					}
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3030,NULL));//修改成功
				}
				break;
			case JXJGroupChangeNameRetVal_failed_repreat:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3031,NULL));//修改失败
				break;
			case JXJGroupChangeNameRetVal_failed_noYuanBao:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3032,NULL));
				break;
			case JXJGroupChangeNameRetVal_failed_empty:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(1099,NULL));
				break;
			case JXJGroupChangeNameRetVal_failed_size:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2023,NULL));
				break;
			case JXJGroupChangeNameRetVal_failed_valid:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2021,NULL));
				break;
			}
		}
	}
}

void JXJ::crJXJRecvChangeGroupNameMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvChangeGroupNameMethod::addParam( int i, const std::string& str )
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

/************************************************************************/
/*     crJXJDismissGroupMethod                                                                  */
/************************************************************************/

crJXJDismissGroupMethod ::crJXJDismissGroupMethod():
	m_this(NULL)
{
}

crJXJDismissGroupMethod ::crJXJDismissGroupMethod (const crJXJDismissGroupMethod & handle):
	crMethod(handle),
	m_btn(handle.m_btn),
	m_type(handle.m_type),
	m_this(NULL)
{
}

void crJXJDismissGroupMethod ::inputParam(int i, void *param)
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

void crJXJDismissGroupMethod ::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJDismissGroupMethod ::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor && canvas.valid())
		{
			crData *data = playerGameData->getDataClass();
			if( data )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(m_type);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvDismissGroup,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJChangeGroupNoticeMethod                                                                  */
/************************************************************************/
crJXJChangeGroupNoticeMethod::crJXJChangeGroupNoticeMethod():
	m_validtime(5),
	m_lasttime(0)
{
}

crJXJChangeGroupNoticeMethod::crJXJChangeGroupNoticeMethod(const crJXJChangeGroupNoticeMethod& handle):
	crMethod(handle),
	m_noticeedit(handle.m_noticeedit),
	m_validtime(handle.m_validtime),
	m_lasttime(handle.m_lasttime),
	m_noticetype(handle.m_noticetype)
{
}

void crJXJChangeGroupNoticeMethod::inputParam(int i, void *param)
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

void crJXJChangeGroupNoticeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeedit = str;
		break;
	case 1:
		m_validtime = atoi(str.c_str());
		break;
	case 2:
		m_noticetype = (unsigned char )atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJChangeGroupNoticeMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		bool ifcansave = false;
		time_t t1 = time(0);
		if (t1 - m_lasttime >= m_validtime)
		{
			m_lasttime = t1;
			ifcansave = true;
		}
		else
		{
			playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2025,NULL));//过于频繁
		}
		if (ifcansave)
		{
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor && canvas.valid())
			{
				ref_ptr<crEditWidgetNode> editwidget = dynamic_cast<crEditWidgetNode *>(canvas -> getWidget(m_noticeedit));
				crData *data = playerGameData->getDataClass();
				if( editwidget.valid() && data)
				{
					std::string noticestr = editwidget->getUTF8String();
					CRUI::crTextAttrWidgetNode::transformText(noticestr);
					if (noticestr.length() <= 400)
					{
						data-> getParam(WCHDATA_JXJConsortiaID,param);
						int mygroupid = *( int *)param;
						if (mygroupid > 0)
						{
							int success = 1;
							playerGameData->doEvent(WCH_JXJCheckInvalidWords,MAKEINT64(&success,&noticestr));
							if (success == 1)
							{
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(4+noticestr.length());
								stream->_writeUChar(m_noticetype);
								stream->_writeString(noticestr);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeGroupNotice,stream.get());
								netConductor->getNetManager()->sendPacket("all",packet);
							}
						}
						editwidget->setEnable(false);
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(5018,NULL));
					}
				}
			}
		}
	}
}

/************************************************************************/
/*   crJXJRecvChangeGroupNoticeMethodMethod                                                                   */
/************************************************************************/

crJXJRecvChangeGroupNoticeMethodMethod::crJXJRecvChangeGroupNoticeMethodMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game){}
crJXJRecvChangeGroupNoticeMethodMethod::crJXJRecvChangeGroupNoticeMethodMethod(const crJXJRecvChangeGroupNoticeMethodMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_canvas(handle.m_canvas)
{
}
void crJXJRecvChangeGroupNoticeMethodMethod::inputParam(int i, void *param)
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

void crJXJRecvChangeGroupNoticeMethodMethod::addParam(int i, const std::string& str)
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

void crJXJRecvChangeGroupNoticeMethodMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			unsigned char noticetype = m_stream->_readUChar();
			std::string noticestr = m_stream->_readString();
			if (noticestr.length() > 400)
			{
				noticestr = noticestr.substr(0,400);
			}
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if(mygroupid > 0)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					ref_ptr<crJXJConsortia> consortia;
					if (itr != consortiamap->end())
					{
						consortia = itr->second;
					}
					if(consortia.valid())
					{
						if (consortia->checkPermission(playerid,JXJGroup_PublishNotice))
						{
							success = 1;
							crData *consortiaData = consortia->getDataClass();
							if (consortiaData)
							{
								switch (noticetype)
								{
								case JXJGroupNotice_Normal:
									consortiaData->inputParam(WCHDATA_JXJConsortiaNormalNotice,&noticestr);
									break;
								case JXJGroupNotice_Contract:
									consortiaData->inputParam(WCHDATA_JXJConsortiaContactNotice,&noticestr);
									break;
								case JXJGroupNotice_Recruit:
									consortiaData->inputParam(WCHDATA_JXJConsortiaRecruitNotice	,&noticestr);
									break;
								default:
									break;
								}
								consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&success);
							}
						}
					}
					stream->createBuf(16);
					stream->_writeUChar(success);
					if (success)
					{
						std::string groupnotice = m_this->getCharacterName() + "修改了军团公告\n" + noticestr;
						CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", groupnotice,0,0,mygroupid);
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
						stream->_writeUChar(noticetype);
						stream->_writeString(noticestr);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeGroupNotice,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_ChangeGroupNotice,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success)
			{
				void *param;
				unsigned char noticeType = m_stream->_readUChar();
				std::string notice = m_stream->_readString();
				ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (groupCanvas.valid() && groupCanvas->getDataClass())
				{
					crData * groupData = groupCanvas->getDataClass();
					CRCore::ScopedLock<crData> lock(*groupData);
					groupData->getParam(WCHDATA_JXJMyGroupInfo,param);
					crJXJGroupInfo * groupInfo = (crJXJGroupInfo *)param;

					switch (noticeType)
					{
					case JXJGroupNotice_Normal:
						groupInfo->setNotice(notice);
						break;
					case JXJGroupNotice_Contract:
						groupInfo->setContactNotice(notice);
						break;
					case JXJGroupNotice_Recruit:
						groupInfo->setRecruitNotice(notice);
						break;
					default:
						break;
					}
					groupCanvas->doEvent(WCH_UI_UpdateData);
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2026,true));//修改成功
				}
			}
			else
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2027,NULL));//修改失败
			}
		}
	}
}


/************************************************************************/
/*      crJXJUIGroupListButtonUpdateMethod                                                                */
/************************************************************************/

crJXJUIGroupListButtonUpdateMethod::crJXJUIGroupListButtonUpdateMethod():
	m_this(NULL),
	m_checkselect(0)
{
}
crJXJUIGroupListButtonUpdateMethod::crJXJUIGroupListButtonUpdateMethod(const crJXJUIGroupListButtonUpdateMethod& handle):
	crMethod(handle),
	m_grouplist(handle.m_grouplist),
	m_groupcheck(handle.m_groupcheck),
	m_pageinput(handle.m_pageinput),
	m_farleftbtn(handle.m_farleftbtn),
	m_leftbtn(handle.m_leftbtn),
	m_rightbtn(handle.m_rightbtn),
	m_farrightbtn(handle.m_farrightbtn),
	m_checkselect(handle.m_checkselect)
{
	for (int i = 0; i < 10; i++)
	{
		m_applybtn[i] = handle.m_applybtn[i];
		m_disapplybtn[i] = handle.m_disapplybtn[i];
		m_checkinfobtn[i] = handle.m_checkinfobtn[i];
	}
}

void crJXJUIGroupListButtonUpdateMethod::inputParam(int i, void *param)
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
void crJXJUIGroupListButtonUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_applybtn[i] = str;
		break;
	case 1:
		m_applybtn[i] = str;
		break;
	case 2:
		m_applybtn[i] = str;
		break;
	case 3:
		m_applybtn[i] = str;
		break;
	case 4:
		m_applybtn[i] = str;
		break;
	case 5:
		m_applybtn[i] = str;
		break;
	case 6:
		m_applybtn[i] = str;
		break;
	case 7:
		m_applybtn[i] = str;
		break;
	case 8:
		m_applybtn[i] = str;
		break;
	case 9:
		m_applybtn[i] = str;
		break;
	case 10:
		m_disapplybtn[i - 10] = str;
		break;
	case 11:
		m_disapplybtn[i - 10] = str;
		break;
	case 12:
		m_disapplybtn[i - 10] = str;
		break;
	case 13:
		m_disapplybtn[i - 10] = str;
		break;
	case 14:
		m_disapplybtn[i - 10] = str;
		break;
	case 15:
		m_disapplybtn[i - 10] = str;
		break;
	case 16:
		m_disapplybtn[i - 10] = str;
		break;
	case 17:
		m_disapplybtn[i - 10] = str;
		break;
	case 18:
		m_disapplybtn[i - 10] = str;
		break;
	case 19:
		m_disapplybtn[i - 10] = str;
		break;
	case 20:
		m_checkinfobtn[i - 20] = str;
		break;
	case 21:
		m_checkinfobtn[i - 20] = str;
		break;
	case 22:
		m_checkinfobtn[i - 20] = str;
		break;
	case 23:
		m_checkinfobtn[i - 20] = str;
		break;
	case 24:
		m_checkinfobtn[i - 20] = str;
		break;
	case 25:
		m_checkinfobtn[i - 20] = str;
		break;
	case 26:
		m_checkinfobtn[i - 20] = str;
		break;
	case 27:
		m_checkinfobtn[i - 20] = str;
		break;
	case 28:
		m_checkinfobtn[i - 20] = str;
		break;
	case 29:
		m_checkinfobtn[i - 20] = str;
		break;
	case 30:
		m_grouplist = str;
		break;
	case 31:
		m_groupcheck = str;
		break;
	case 32:
		m_pageinput = str;
		break;
	case 33:
		m_farleftbtn = str;
		break;
	case 34:
		m_leftbtn = str;
		break;
	case 35:
		m_rightbtn = str;
		break;
	case 36:
		m_farrightbtn = str;
		break;
	}
}
void crJXJUIGroupListButtonUpdateMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		void *param;
		crData *data = playerGameData->getDataClass();
		if (canvas.valid() && data)
		{
			data-> getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *( int *)param;
			ref_ptr<crTableWidgetNode>  grouplist = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_grouplist));
			ref_ptr<crCheckBoxWidgetNode> groupcheck = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_groupcheck));
			ref_ptr<crStaticTextWidgetNode> pageinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_pageinput));
			ref_ptr<crButtonWidgetNode> farleftbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_farleftbtn));
			ref_ptr<crButtonWidgetNode> leftbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_leftbtn));
			ref_ptr<crButtonWidgetNode> rightbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_rightbtn));
			ref_ptr<crButtonWidgetNode> farrightbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_farrightbtn));
			ref_ptr<crButtonWidgetNode> applybtn[10] = {NULL};
			ref_ptr<crButtonWidgetNode> disapplybtn[10] = {NULL};
			ref_ptr<crButtonWidgetNode> checkinfobtn[10] = {NULL};
			for (int i = 0;i < 10;i++)
			{
				applybtn[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_applybtn[i]));
				disapplybtn[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_disapplybtn[i]));
				checkinfobtn[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_checkinfobtn[i]));
				if (applybtn[i].valid())applybtn[i]->setVisiable(false);
				if (disapplybtn[i].valid())disapplybtn[i]->setVisiable(false);
				if (checkinfobtn[i].valid())checkinfobtn[i]->setVisiable(false);
			}

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata && groupcheck.valid() && grouplist.valid())
			{
				canvasdata->getParam(WCHDATA_JXJCurSelectGroupID,param);
				int curselectgroupid = *( int *)param;
				canvasdata->getParam(WCHDATA_JXJGroupCurPage,param);
				int curpage = *( int *)param;
				canvasdata->getParam(WCHDATA_JXJGroupTotalPage,param);
				int totalpage = *( int *)param;
				canvasdata->getParam(WCHDATA_JXJGroupInfoVec,param);
				GroupInfoVec *groupinfovec = (GroupInfoVec *)param;
				canvasdata->getParam(WCHDATA_JXJMyApplyGroupInfoSet,param);
				MyGroupApplySet *myGroupApplySet= (MyGroupApplySet *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				unsigned char groupshilid = 0;
				if (curselectgroupid == 0)
				{
					curselectgroupid = grouplist->getData(0);
				}
				for( GroupInfoVec::iterator itr = groupinfovec->begin();
					itr != groupinfovec->end();
					++itr)
				{
					if (curselectgroupid == (*itr)->getID())
					{
						groupshilid = (*itr)->getShiliID();
					}

					if (myGroupApplySet && myGroupApplySet->find((*itr)->getID()) != myGroupApplySet->end())
					{
						if (applybtn[0].valid())applybtn[0]->setVisiable(true);
					}
					else
					{
						if (disapplybtn[0].valid())disapplybtn[0]->setVisiable(true);
					}

				}
				//int row = 0;
				//if (groupcheck->getSelect() == 0)
				//{
				//	if (!groupinfovec->empty())
				//	{
				//		if (checkinfobtn[0])checkinfobtn[0]->setVisiable(true);
				//		if (mygroupid == 0)
				//		{ 
				//			if(shiliid == groupshilid)
				//			{
				//				if (curselectgroupid != myapplygroupinfo->getID())
				//				{
				//					if (applybtn[0])applybtn[0]->setVisiable(true);
				//				}
				//				else
				//				{
				//					if (disapplybtn[0])disapplybtn[0]->setVisiable(true);
				//				}
				//			}
				//		}
				//	}
				//}
				//else
				//{
				//	totalpage = 1;
				//	if (totalpage < 1)
				//	{
				//		totalpage = 1;
				//	}
				//	canvasdata->inputParam(WCHDATA_JXJGroupTotalPage,&totalpage);
				//	if (myapplygroupinfo->getID() > 0)
				//	{
				//		if (checkinfobtn[0])checkinfobtn[0]->setVisiable(true);
				//		if (disapplybtn[0])disapplybtn[0]->setVisiable(true);
				//	}
				//}
				if (pageinput.valid())
				{
					pageinput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}
				//grouplist->selectRow(sel);
			}
		}
	}
}


/************************************************************************/
/*     crJXJMyGroupMemberInfoPageUpdateMethod                                                                 */
/************************************************************************/

crJXJMyGroupMemberInfoPageUpdateMethod::crJXJMyGroupMemberInfoPageUpdateMethod():
	m_this(NULL)
{
}

crJXJMyGroupMemberInfoPageUpdateMethod::crJXJMyGroupMemberInfoPageUpdateMethod(const crJXJMyGroupMemberInfoPageUpdateMethod& handle):
	crMethod(handle),
	m_btnsw(handle.m_btnsw),
	m_leavegroupbtn(handle.m_leavegroupbtn),
	m_changecreatorbtn(handle.m_changecreatorbtn),
	m_memberlist(handle.m_memberlist),
	m_pageinput(handle.m_pageinput),
	m_farleftbtn(handle.m_farleftbtn),
	m_leftbtn(handle.m_leftbtn),
	m_rightbtn(handle.m_rightbtn),
	m_farrightbtn(handle.m_farrightbtn),
	m_mygroupposinput(handle.m_mygroupposinput)
{
}

void crJXJMyGroupMemberInfoPageUpdateMethod::inputParam(int i, void *param)
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

void crJXJMyGroupMemberInfoPageUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_btnsw = str;
		break;
	case 1:
		m_leavegroupbtn = str;
		break;
	case 2:
		m_changecreatorbtn = str;
		break;
	case 3:
		m_memberlist = str;
		break;
	case 4:
		m_pageinput = str;
		break;
	case 5:
		m_farleftbtn = str;
		break;
	case 6:
		m_leftbtn = str;
		break;
	case 7:
		m_rightbtn = str;
		break;
	case 8:
		m_farrightbtn = str;
		break;
	case 9:
		m_mygroupposinput = str;
		break;
	default:
		break;
	}
}

void crJXJMyGroupMemberInfoPageUpdateMethod::operator()(crHandle &handle)
{
//	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//	ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
//	if(m_this && playerGameData && fubentab)
//	{
//		crMultiSwitch *btnsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_btnsw));
//		ref_ptr<crButtonWidgetNode> leavegroupbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leavegroupbtn));
//		ref_ptr<crButtonWidgetNode> changecreatorbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_changecreatorbtn));
//		ref_ptr<crTableWidgetNode> memberlist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_memberlist));
//		ref_ptr<crStaticTextWidgetNode> pageinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageinput));
//		ref_ptr<crButtonWidgetNode> farleftbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_farleftbtn));
//		ref_ptr<crButtonWidgetNode> leftbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leftbtn));
//		ref_ptr<crButtonWidgetNode> rightbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rightbtn));
//		ref_ptr<crButtonWidgetNode> farrightbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_farrightbtn));
//		ref_ptr<crStaticTextWidgetNode> mygroupposinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mygroupposinput));
//		
//		crData *canvasdata = m_this->getDataClass();
//		if (canvasdata)
//		{
//			if (btnsw)
//			{
//				btnsw->setActiveSwitchSet(2);
//			}
//			void *param;
//			std::string myname = playerGameData->getCharacterName();
//			crData *data = playerGameData->getDataClass();
//
//			ref_ptr<crTableIO> postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
//			crTableIO::StrVec record;
//			int govpostindex = postTab->getTitleIndex("govpost");
//			data->getParam(WCHDATA_JXJVipLv,param);
//			unsigned char viplv = *(unsigned char *)param;
//			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckGroupPerPageNum,viplv).c_str());
//			canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
//			crJXJGroupInfo *groupinfo = (crJXJGroupInfo *)param;
//			canvasdata->getParam(WCHDATA_JXJMyGroupMemberMap,param);
//			ConsortiaMemberSet *memberset= (ConsortiaMemberSet *)param;
//			canvasdata->getParam(WCHDATA_JXJMyGroupMemberCurPage,param);
//			int curpage = *(int *)param;
//			canvasdata->getParam(WCHDATA_JXJMyGroupMemberTotalPage,param);
//			int totalpage = *(int *)param;
//			if(pageinput)pageinput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
//			std::string str2 = "小喽喽";
//			if (groupinfo->getCreatorName().compare(myname) == 0)
//			{
//				str2 = "团长";
//				if (btnsw)
//				{
//					btnsw->setActiveSwitchSet(0);
//				}
//			}
//			mygroupposinput->setString(str2);
//			if (leavegroupbtn)
//			{
//				leavegroupbtn->setEnable(true);
//			}
//			if (changecreatorbtn)
//			{
//				changecreatorbtn->setEnable(false);
//			}
//			int row = 0;// -(curpage - 1) * perpagenum;
//			std::string str;
//			std::string onlinestr;
//			int selectrow =  memberlist->getSelect();
//			memberlist->clearData();
//			ref_ptr<crJXJConsortiaMember> member;
//			for (ConsortiaMemberSet::iterator itr = memberset->begin();
//				itr != memberset->end();
//				++itr,++row)
//			{
//				onlinestr = "离线";
//				str = "";
//				if(postTab->queryOneRecord(0,crArgumentParser::appItoa((*itr)->getPos()),record) >= 0)
//				{
//					member = *itr;
//					memberlist->addData(0,row,member->getName());
//					memberlist->addData(1,row,crArgumentParser::appItoa(member->getLevel()));
//
//					switch ((*itr)->getGroupPos())
//					{
//					case JXJGroup_FristLeader:
//						str = "团长";
//						break;
//					case JXJGroup_SecondLeader:
//						str = "副团长";
//						break;
//					default:
//						break;
//					}
//					memberlist->addData(2,row,str);
//					memberlist->addData(3,row,record[govpostindex]);
//					memberlist->addData(4,row,crArgumentParser::appItoa(member->getActiveVaule()));
//					memberlist->addData(5,row,crArgumentParser::appItoa(member->getTribute()));
//					if ((*itr)->getIfOnline() == 1)
//					{
//						onlinestr = "在线";
//					}
//					memberlist->addData(6,row,onlinestr);
//					memberlist->addData(row,member->getPlayerID());
//				}
//			}
//			memberlist -> selectRow(selectrow);
//		}
//	}
}



/************************************************************************/
/*    crJXJClientClickChangeGroupNoticeMethod                                                                  */
/************************************************************************/

JXJ::crJXJClientClickChangeGroupNoticeMethod::crJXJClientClickChangeGroupNoticeMethod():
	m_this(NULL)
{

}

JXJ::crJXJClientClickChangeGroupNoticeMethod::crJXJClientClickChangeGroupNoticeMethod( const crJXJClientClickChangeGroupNoticeMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_noticeedit(handle.m_noticeedit)
{

}

void JXJ::crJXJClientClickChangeGroupNoticeMethod::operator()( crHandle &handle )
{
	if  (m_this)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			ref_ptr<crEditWidgetNode> editwidget = dynamic_cast<crEditWidgetNode *>(canvas -> getWidget(m_noticeedit));
			if (editwidget.valid())
			{
				editwidget->setEnable(true);
			}
		}

	}

}

void JXJ::crJXJClientClickChangeGroupNoticeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJClientClickChangeGroupNoticeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_noticeedit = str;
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJUILookGroupUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUILookGroupUpdateMethod::crJXJUILookGroupUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJUILookGroupUpdateMethod::crJXJUILookGroupUpdateMethod( const crJXJUILookGroupUpdateMethod & handle ):
	crMethod(handle),
	m_apply				(handle.m_apply),
	m_cancelApply		(handle.m_cancelApply),
	m_pageinput			(handle.m_pageinput),
	m_leftbtn			(handle.m_leftbtn),
	m_rightbtn			(handle.m_rightbtn),
	m_memberList		(handle.m_memberList),
	m_canvas			(handle.m_canvas),		
	m_leaderName		(handle.m_leaderName),
	m_groupLevel		(handle.m_groupLevel),	
	m_groupRank			(handle.m_groupRank),
	m_fengyi			(handle.m_fengyi)	

{
}

void JXJ::crJXJUILookGroupUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_apply = str;
		break;
	case 1:
		m_cancelApply = str;
	case 2:
		m_pageinput = str;
		break;
	case 3:
		m_leftbtn = str;
		break;
	case 4:
		m_rightbtn = str;
	case 5:
		m_memberList = str;
		break;
	case 6:
		m_canvas = str;
		break;
	case 7:
		m_leaderName = str;
		break;
	case 8:
		m_groupLevel = str;
		break;
	case 9:
		m_groupRank = str;
		break;
	case 10:
		m_fengyi = str;
		break;
	default:
		break;
	}
}



void JXJ::crJXJUILookGroupUpdateMethod::operator()( crHandle &handle )
{	
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data )
		{
			CRCore::ScopedLock<crData> lock(*data);
			data-> getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *( int *)param;
			ref_ptr<crStaticTextWidgetNode> pageinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageinput));
			ref_ptr<crStaticTextWidgetNode>  leaderName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_leaderName));
			ref_ptr<crStaticTextWidgetNode>  fengyi = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_fengyi));
			ref_ptr<crStaticTextWidgetNode>  groupRank = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupRank));
			ref_ptr<crStaticTextWidgetNode>  groupLevel = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupLevel));

			ref_ptr<crButtonWidgetNode> applybtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_apply));
			if (applybtn.valid()) applybtn->setVisiable(false);
			ref_ptr<crButtonWidgetNode>  cancelApplyBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_cancelApply));
			if (cancelApplyBtn.valid() )cancelApplyBtn->setVisiable(false);
			ref_ptr<crTableWidgetNode>   memberList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_memberList));

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckGroupPerPageNum,viplv).c_str());
			ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (groupCanvas.valid() && groupCanvas->getDataClass() && m_this->getDataClass())
			{
				crData * groupCanvasData = groupCanvas->getDataClass();
				crData *canvasdata = m_this->getDataClass();

				groupCanvasData->getParam(WCHDATA_JXJCurSelectGroupID,param);
				int curselectgroupid = *( int *)param;
				groupCanvasData->getParam(WCHDATA_JXJGroupInfoVec,param);
				GroupInfoVec * groupInfoVec = (GroupInfoVec *)param;

				GroupInfoVec::iterator itr = groupInfoVec->begin();
				for (; itr != groupInfoVec->end(); ++ itr)
				{
					if ((*itr).valid())
					{
						if ( (*itr)->getID()  == curselectgroupid)
						{
							break;
						}
					}
				}

				if (groupInfoVec->end() != itr)
				{
					if (leaderName.valid())
					{
						leaderName->setString((*itr)->getCreatorName());
					}
					if (fengyi.valid())
					{
						fengyi->setString( (*itr)->getFengYi());
					}
					if (groupRank.valid())
					{
						groupRank->setString( crArgumentParser::appItoa((*itr)->getRank()));
					}
					if (groupLevel.valid())
					{
						groupLevel->setString( crArgumentParser::appItoa((*itr)->getLv()));
					}
				}
				groupCanvasData->getParam(WCHDATA_JXJMyApplyGroupInfoSet,param);
				MyGroupApplySet * myGroupApplySet = ( MyGroupApplySet *)param;

				canvasdata->getParam(WCHDATA_JXJCheckGroupCurPage,param);
				int curpage = *( int *)param;
				canvasdata->getParam(WCHDATA_JXJCheckGroupTotalPage	,param);
				int totalpage = *( int *)param;

				canvasdata->getParam(WCHDATA_JXJCheckGroupMemberVec,param);
				GroupMemberInfoVec * memberVec = (GroupMemberInfoVec * )param;


				memberList->clearData();

				ref_ptr<crTableIO> postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
				crTableIO::StrVec record;
				int govpostindex = postTab->getTitleIndex("govpost");

				GroupMemberInfoVec ::iterator it = memberVec->begin();
				int row = 0;
				//int startPoint = (curpage - 1) * perpagenum;
				for (; it != memberVec->end() && row < perpagenum; ++ it,++ row)
				{
						memberList->addData(0,row,(*it)->getName());
						std::string playerlevel = crArgumentParser::appItoa((*it)->getLevel());
						memberList->addData(1,row,playerlevel);
						int groupPos = (*it)->getGroupPos();
						std::string groupPosName;
						switch (groupPos)
						{
						case JXJGroup_FirstLeader:
							groupPosName = "军团长";
							break;
						case JXJGroup_SecondLeader:
							groupPosName = "副军团长";
							break;
						default:
							groupPosName = "普通成员";
							break;
						}
						memberList->addData(2,row,groupPosName);
						if (postTab->queryOneRecord(0,crArgumentParser::appItoa((*it)->getPos()),record) >= 0)
						{
							memberList->addData(3,row,record[govpostindex]);
						}
					}
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				unsigned char groupshilid = 0;
				if (myGroupApplySet && myGroupApplySet->find(curselectgroupid) !=  myGroupApplySet->end())
				{
					if (cancelApplyBtn.valid() )cancelApplyBtn->setVisiable(true);
				}
				else
				{
					if (applybtn.valid()) applybtn->setVisiable(true);
				}

				if (pageinput.valid())
				{
					pageinput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}
			}
		}
	}
}

void JXJ::crJXJUILookGroupUpdateMethod::inputParam( int i, void *param )
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

/************************************************************************/
/*    crJXJUIMyGroupInfoUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIMyGroupInfoUpdateMethod::crJXJUIMyGroupInfoUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIMyGroupInfoUpdateMethod::crJXJUIMyGroupInfoUpdateMethod( const crJXJUIMyGroupInfoUpdateMethod& handle ):
	crMethod(handle),
	m_groupName(handle.m_groupName),
	m_groupLevel(handle.m_groupLevel),
	m_memberCount(handle.m_memberCount),
	m_rank(handle.m_rank),
	m_changeNameBtn(handle.m_changeNameBtn),
	m_lookRankBtn(handle.m_lookRankBtn),
	m_gameList(handle.m_gameList),
	m_editNoticeBtn(handle.m_editNoticeBtn),
	m_publishNoticeBtn(handle.m_publishNoticeBtn),
	m_input(handle.m_input),
	m_tuiweiBtn(handle.m_tuiweiBtn),
	m_weirenBtn(handle.m_weirenBtn),
	m_leftBtn(handle.m_leftBtn),
	m_rightBtn(handle.m_rightBtn),
	m_yichuBtn(handle.m_yichuBtn),
	m_leavegroupBtn(handle.m_leavegroupBtn),
	m_sw(handle.m_sw),
	m_gonggao(handle.m_gonggao),
	m_jiechufutuanBtn(handle.m_jiechufutuanBtn)
{

}

void JXJ::crJXJUIMyGroupInfoUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_groupName = str;
		break;
	case 1:
		m_groupLevel = str;
		break;
	case 2:
		m_memberCount = str;
		break;
	case 3:
		m_rank = str;
		break;
	case 4:
		m_changeNameBtn = str;
		break;
	case 5:
		m_lookRankBtn = str;
		break;
	case 6:
		m_gameList = str;
		break;
	case 7:
		m_editNoticeBtn = str;
		break;
	case 8:
		m_publishNoticeBtn = str;
		break;
	case 9:
		m_input = str;
		break;
	case 10:
		m_tuiweiBtn = str;
		break;
	case 11:
		m_weirenBtn = str;
		break;
	case 12:
		m_leftBtn = str;
		break;
	case 13:
		m_rightBtn = str;
		break;
	case 14:
		m_yichuBtn = str;
		break;
	case 15:
		m_leavegroupBtn = str;
		break;
	case 16:
		m_sw = str;
		break;
	case 17:
		m_gonggao = str;
		break;
	case 18:
		m_jiechufutuanBtn = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIMyGroupInfoUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data )
		{
			ref_ptr<crStaticTextWidgetNode>  groupName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupName));
			ref_ptr<crStaticTextWidgetNode>  groupLv = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupLevel));
			ref_ptr<crStaticTextWidgetNode>  memberCount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_memberCount));
			ref_ptr<crStaticTextWidgetNode>  groupRank = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_rank));
			ref_ptr<crButtonWidgetNode>  changeGroupNameBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_changeNameBtn));
			ref_ptr<crButtonWidgetNode>  lookRankBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lookRankBtn));
			ref_ptr<crButtonWidgetNode>  editNoticeBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_editNoticeBtn));
			ref_ptr<crButtonWidgetNode>  publishNoticeBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_publishNoticeBtn));
			ref_ptr<crEditWidgetNode>  noticeText = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_gonggao));
			ref_ptr<crTableWidgetNode>   memberList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_gameList));
			crMultiSwitch *Sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
			if (Sw) Sw->setActiveSwitchSet(1);
			ref_ptr<crStaticTextWidgetNode>  pageInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input));
			ref_ptr<crButtonWidgetNode>  tuiweiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_tuiweiBtn));
			ref_ptr<crButtonWidgetNode>  jiechuBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_jiechufutuanBtn));
			ref_ptr<crButtonWidgetNode>  weirenBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_weirenBtn));
			ref_ptr<crButtonWidgetNode>  yichuBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_yichuBtn));
			ref_ptr<crButtonWidgetNode>  leftBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leftBtn));
			ref_ptr<crButtonWidgetNode>  rightBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rightBtn));
			ref_ptr<crButtonWidgetNode>  leavegroupBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leavegroupBtn));

			data-> getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *( int *)param;

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
			crData *canvasdata = m_this->getDataClass();
			if (canvasdata && mygroupid)
			{
				CRCore::ScopedLock<crData> lock(*canvasdata);
				ref_ptr<crTableIO>  groupUpgradeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);

				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo * myGroupInfo = (crJXJGroupInfo *) param;
				if (myGroupInfo)
				{
					if (groupName.valid()) groupName->setString(myGroupInfo->getName());
					if (groupRank.valid()) groupRank->setString(crArgumentParser::appItoa(myGroupInfo->getRank()));
					if (groupLv.valid()) groupLv->setString(crArgumentParser::appItoa(myGroupInfo->getLv()));

					if (memberCount.valid()) 
					{
						crTableIO::StrVec record;
						if (groupUpgradeTab->queryOneRecord(0,crArgumentParser::appItoa(myGroupInfo->getLv()),record) >= 0)
						{
							int numIdx = groupUpgradeTab->getTitleIndex("成员上限");
							memberCount->setString(crArgumentParser::appItoa(myGroupInfo->getMemberCount()) + "/" + record[numIdx]);
						}
					}
					if (noticeText.valid()) 
					{
						noticeText->setEnable(false);
						noticeText->setString(myGroupInfo->getNotice());
					}
				}
				if (pageInput.valid())
				{
					canvasdata->getParam(WCHDATA_JXJMyGroupMemberCurPage,param);
					int curpage = *(int *)param;
					canvasdata->getParam(WCHDATA_JXJMyGroupMemberTotalPage,param);
					int totalpage = *(int *)param;

					pageInput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
					pageInput->setAlignment(CRText::crText::CENTER_BASE_LINE);
				}

				unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
				canvasdata->getParam(WCHDATA_JXJMyGroupMemberVec,param);
				ConsortiaMemberVec *memberVec = (ConsortiaMemberVec *) param;
				int cursel = memberList->getSelect();
				memberList->clearData();

				ref_ptr<crTableIO> postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
				crTableIO::StrVec record;
				int govpostindex = postTab->getTitleIndex("govpost");
				time_t curtime = time(0);
				time_t logintime = 0;
				int leavedays = 0;
				ConsortiaMemberVec::iterator cmItr = memberVec->begin();
				for (unsigned char i = 0; cmItr != memberVec->end() && i < perpagenum; ++ i,++ cmItr)
				{
					memberList->addData(0,i,(*cmItr)->getName());
					memberList->addData(2,i,crArgumentParser::appItoa((*cmItr)->getLevel()));

					std::string groupPosName = "";
					switch ((*cmItr)->getGroupPos())
					{
					case JXJGroup_FirstLeader:
						groupPosName = "军团长";
						break;
					case JXJGroup_SecondLeader:
						groupPosName = "副军团长";
						break;
					default:
						groupPosName = "普通成员";
						break;
					}
					memberList->addData(1,i,groupPosName);

					if (postTab->queryOneRecord(0,crArgumentParser::appItoa((*cmItr)->getPos()),record) >= 0)
					{
						memberList->addData(3,i,record[govpostindex]);
					}
					memberList->addData(4,i,crArgumentParser::appItoa((*cmItr)->getGroupContribute()));
					logintime = (*cmItr)->getLoginTime();
					std::string online = "离线";
					leavedays = (curtime - logintime) / c_24hours;
					if ((*cmItr)->getIfOnline())
						online = "在线";
					else if (leavedays <= 0)
					{
						online = "离线";
					}
					else
					{
						online = "离线" + crArgumentParser::appItoa(leavedays) + "天";
					}
					memberList->addData(5,i,online);
					memberList->addData(i,(*cmItr)->getPlayerID());
				}
				if (!memberVec->empty())
				{
					memberList->selectRow(cursel);
				}

				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
				
				int memcount = 0;
				if (myGroupInfo)
				{
					memcount = myGroupInfo->getMemberCount();
				}

				if (myGroupPos == JXJGroup_Normal)
				{
					if (Sw)
					{
						Sw->setActiveSwitchSet(1);
					}
					if (leavegroupBtn.valid())
					{
						leavegroupBtn->setVisiable(true);
					}
				}
				else
				{
					if (Sw)
					{
						Sw->setActiveSwitchSet(0);
					}
					PermissionCheck checkPermission;
					if (changeGroupNameBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_ChangeName))
						{
							changeGroupNameBtn->setVisiable(true);						
						}
						else
						{
							changeGroupNameBtn->setVisiable(false);
						}
					}
					
					if (yichuBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_KickMember))
						{
							yichuBtn->setVisiable(true);
						}
						else
						{
							yichuBtn->setVisiable(false);
						}
					}
					
					if (lookRankBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_LookGroupRank))
						{
							lookRankBtn->setVisiable(true);
						}
						else
						{
							lookRankBtn->setVisiable(false);
						}
					}
					if (editNoticeBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_EditNotice))
						{
							editNoticeBtn->setVisiable(true);
						}
						else
						{
							editNoticeBtn->setVisiable(false);
						}
					}

					if (publishNoticeBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_PublishNotice))
						{
							publishNoticeBtn->setVisiable(true);
						}
						else
						{
							publishNoticeBtn->setVisiable(false);
						}
					}
					if (weirenBtn.valid() && jiechuBtn.valid() && tuiweiBtn.valid())
					{
						if (checkPermission(myGroupPos,JXJGroup_ChangeLeader))
						{
							weirenBtn->setVisiable(true);
							tuiweiBtn->setVisiable(true);
							std::wstring groupPosName ;
							bool z = memberList->getData(1,cursel,groupPosName);
							if (groupPosName == L"副军团长")
							{
								weirenBtn->setVisiable(false);
								jiechuBtn->setVisiable(true);
							}
							else if(groupPosName == L"普通成员")
							{
								weirenBtn->setVisiable(true);
								jiechuBtn->setVisiable(false);
								if((myGroupInfo->getLeaderVec()).size() >=3)
								{
									weirenBtn->setEnable(false);
								}
								else
								{
									weirenBtn->setEnable(true);
								}
							}
							else
							{
								weirenBtn->setVisiable(false);
								jiechuBtn->setVisiable(false);
							}
						}
						else
						{
							weirenBtn->setVisiable(false);
							jiechuBtn->setVisiable(false);
							tuiweiBtn->setVisiable(false);
						}
					}
					if (leavegroupBtn.valid())
					{
						if (JXJGroup_FirstLeader == myGroupPos)
						{
							if (1 == memcount)
							{
								leavegroupBtn->setVisiable(true);
							}
							else
							{
								leavegroupBtn->setVisiable(false);
							}
						}
						else
						{
							leavegroupBtn->setVisiable(true);
						}
					}
				}

			}




			

			//ref_ptr<crTableIO>  groupUpgradeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);

			//if (contractText.valid())
			//{
			//	contractText->setEnable(false);
			//}
			//if (noticeText.valid())
			//{
			//	noticeText->setEnable(false);
			//}

			//data-> getParam(WCHDATA_JXJConsortiaID,param);
			//int mygroupid = *( int *)param;

			//data->getParam(WCHDATA_JXJVipLv,param);
			//unsigned char viplv = *(unsigned char *)param;
			//unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
			//crData *canvasdata = m_this->getDataClass();
			//if (canvasdata && mygroupid)
			//{
			//	CRCore::ScopedLock<crData> lock(*canvasdata);
			//	canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			//	crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;

			//	int myGroupPos = JXJGroup_Normal;
			//	if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();

			//	if (myGroupPos == JXJGroup_Normal)
			//	{

			//	}

			//	
			//	canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
			//	crJXJGroupInfo * myGroupInfo = (crJXJGroupInfo *) param;
			//	if (groupName.valid()) groupName->setString(myGroupInfo->getName());
			//	if (memberCount.valid()) 
			//	{
			//		crTableIO::StrVec record;
			//		if (groupUpgradeTab->queryOneRecord(0,crArgumentParser::appItoa(myGroupInfo->getLv()),record) >= 0)
			//		{
			//			int numIdx = groupUpgradeTab->getTitleIndex("成员上限");
			//			memberCount->setString(crArgumentParser::appItoa(myGroupInfo->getMemberCount()) + "/" + record[numIdx]);
			//		}
			//	}

			//	if (groupRank.valid()) groupRank->setString(crArgumentParser::appItoa(myGroupInfo->getRank()));
			//	if (groupLevel.valid()) groupLevel->setString(crArgumentParser::appItoa(myGroupInfo->getLv()));
			//	if (curGroupFunds.valid())
			//	{
			//		crTableIO::StrVec record;
			//		if (groupUpgradeTab->queryOneRecord(0,crArgumentParser::appItoa(myGroupInfo->getLv()),record) >= 0)
			//		{
			//			int fundsIdx = groupUpgradeTab->getTitleIndex("升级资金");
			//			curGroupFunds->setString(crArgumentParser::appItoa(myGroupInfo->getExp())
			//				+ " / "
			//				+ record[fundsIdx]);
			//		}
			//	}
			//	
			//	GroupLeaderInfoVec &groupLeaderInfoVec = myGroupInfo->getLeaderVec();

			//	GroupLeaderInfoVec::iterator it = groupLeaderInfoVec.begin();
			//	for (unsigned char i = 0; it != groupLeaderInfoVec.end() && i < 3; ++ i,++ it)
			//	{
			//		if (leaderName[i].valid()) leaderName[i]->setString(groupLeaderInfoVec[i].first);
			//		if (leaderLevel[i].valid()) leaderLevel[i]->setString(crArgumentParser::appItoa(groupLeaderInfoVec[i].second));
			//	}

			//	crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);

			//	std::string notice = "";
			//	int mode = 2;
			//	noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			//	if(info.valid())
			//	{
			//		int texid = 3018;
			//		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			//		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			//		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			//		info->setString(notice);
			//	}

			//	notice = myGroupInfo->getContactNotice();
			//	if (!notice.length())
			//	{
			//		int texid = 3019;
			//		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			//		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			//		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			//	}
			//	if (contractText.valid()) contractText->setString(notice);
			//	if (noticeText.valid()) noticeText->setString(myGroupInfo->getNotice());

			//	if (billboard.valid()) 
			//	{
			//		notice = "";
			//		int texid = 3020;
			//		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			//		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			//		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			//		billboard->setString(notice);
			//	}

			//	canvasdata->getParam(WCHDATA_JXJMyGroupActiveMemberVec,param);
			//	GroupActiveMemberVec * memberSet = (GroupActiveMemberVec *)param;

			//	memberList->clearData();
			//	if (memberList.valid() && !memberSet->empty())
			//	{
			//		int row = 0;
			//		std::ostringstream name;
			//		name << "";
			//		for( GroupActiveMemberVec ::iterator itr = memberSet->begin();
			//			itr != memberSet->end()&&row<perpagenum;
			//			++itr)
			//		{						
			//			name.str("");
			//			name << ++ row ;
			//			name << "  ";
			//			name << itr->first;
			//			memberList->addData(0,row - 1,name.str());
			//			memberList->addData(1,row - 1,crArgumentParser::appItoa(itr->second));
			//		}
			//	}
			//}
		}
	}
}

void JXJ::crJXJUIMyGroupInfoUpdateMethod::inputParam( int i, void *param )
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

/************************************************************************/
/*   crJXJUIMyGroupManageUpdateMethod */
/************************************************************************/

JXJ::crJXJUIMyGroupManageUpdateMethod::crJXJUIMyGroupManageUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIMyGroupManageUpdateMethod::crJXJUIMyGroupManageUpdateMethod( const crJXJUIMyGroupManageUpdateMethod& handle ):
	crMethod(handle),
	m_switch(handle.m_switch),
	m_rightPage(handle.m_rightPage),
	m_publish(handle.m_publish),
	m_pageNum(handle.m_pageNum),
	m_notice(handle.m_notice),
	m_memberList(handle.m_memberList),
	m_leftPage(handle.m_leftPage),
	m_leaveGroup(handle.m_leaveGroup),
	m_kickOutGroup(handle.m_kickOutGroup),
	m_groupPos(handle.m_groupPos),
	m_editBtn(handle.m_editBtn),
	m_dismissGroup(handle.m_dismissGroup),
	m_dismissAtOnce(handle.m_dismissAtOnce),
	m_changeLeader(handle.m_changeLeader),
	m_cancelDismiss(handle.m_cancelDismiss),
	m_activeValue(handle.m_activeValue),
	m_this(NULL)
{
}

void JXJ::crJXJUIMyGroupManageUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_groupPos = str;
		break;
	case 1:
		m_activeValue = str;
		break;
	case 2:
		m_switch = str;
		break;
	case 3:
		m_notice = str;
		break;
	case 4:
		m_editBtn = str;
		break;
	case 5:
		m_publish = str;
		break;
	case 6:
		m_memberList = str;
		break;
	case 7:
		m_leftPage = str;
		break;
	case 8:
		m_pageNum = str;
		break;
	case 9:
		m_rightPage = str;
		break;
	case 10:
		m_changeLeader = str;
		break;
	case 11:
		m_leaveGroup = str;
		break;
	case 12:
		m_dismissGroup = str;
		break;
	case 13:
		m_kickOutGroup = str;
		break;
	case 14:
		m_dismissAtOnce = str;
		break;
	case 15:
		m_cancelDismiss = str;
		break;
	case 16:
		m_leaveGroup_sw = str;
		break;
	case 17:
		m_changeLeader_sw = str;
		break;
	case 18:
		m_dismissGroup_sw = str;
		break;
	case 19:
		m_kickOutGroup_sw = str;
		break;
	case 20:
		m_editBtn_sw = str;
		break;
	case 21:
		m_publish_sw = str;
		break;
	case 22:
		m_dismissAtOnce_sw = str;
		break;
	case 23:
		m_cancelDismiss_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIMyGroupManageUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data )
		{
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			ref_ptr<crStaticTextWidgetNode>  groupPos = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupPos));
			ref_ptr<crStaticTextWidgetNode>  activeValue = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_activeValue));
			crMultiSwitch *switchWidget = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch));

			crMultiSwitch * changeLeaderSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_cancelDismiss_sw));
			crMultiSwitch *leaveGroupSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_leaveGroup_sw));
			crMultiSwitch * dismissGroupSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_dismissGroup_sw));
			crMultiSwitch * kickOutSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_kickOutGroup_sw));
			crMultiSwitch * editSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_editBtn_sw));
			crMultiSwitch * publicNoticeSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_publish_sw));
			crMultiSwitch * dismissAtOnceSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_dismissAtOnce_sw));
			crMultiSwitch * cancelDismissSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_cancelDismiss_sw));
			ref_ptr<crStaticTextWidgetNode>  noticeText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_notice));

			ref_ptr<crStaticTextWidgetNode>  pageInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageNum));
			ref_ptr<crButtonWidgetNode>  changeLeaderBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_changeLeader));
			ref_ptr<crButtonWidgetNode>  leaveGroupBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leaveGroup));
			ref_ptr<crButtonWidgetNode>  dismissGroupBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_dismissGroup));
			ref_ptr<crButtonWidgetNode>  kickOutBtn	= dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_kickOutGroup));
			ref_ptr<crButtonWidgetNode>  dismissAtOnceBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_dismissAtOnce));
			ref_ptr<crButtonWidgetNode>  cancelDismissBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_cancelDismiss));
			ref_ptr<crButtonWidgetNode>  editBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_editBtn));
			ref_ptr<crButtonWidgetNode>  publicBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_publish));
			ref_ptr<crTableWidgetNode>   memberList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_memberList));

			crData *canvasdata = m_this->getDataClass();
			if (canvasdata)
			{
				CRCore::ScopedLock<crData> lock(*canvasdata);
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				
				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;

				unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
				canvasdata->getParam(WCHDATA_JXJMyGroupMemberVec,param);
				ConsortiaMemberVec *memberVec = (ConsortiaMemberVec *) param;
				memberList->clearData();

				ref_ptr<crTableIO> postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
				crTableIO::StrVec record;
				int govpostindex = postTab->getTitleIndex("govpost");

				ConsortiaMemberVec::iterator cmItr = memberVec->begin();

				for (unsigned char i = 0; cmItr != memberVec->end() && i < perpagenum; ++ i,++ cmItr)
				{
					memberList->addData(0,i,(*cmItr)->getName());
					memberList->addData(1,i,crArgumentParser::appItoa((*cmItr)->getLevel()));

					std::string groupPosName = "";
					switch ((*cmItr)->getGroupPos())
					{
					case JXJGroup_FirstLeader:
						groupPosName = "军团长";
						break;
					case JXJGroup_SecondLeader:
						groupPosName = "副军团长";
						break;
					default:
						groupPosName = "普通成员";
						break;
					}
					memberList->addData(2,i,groupPosName);

					if (postTab->queryOneRecord(0,crArgumentParser::appItoa((*cmItr)->getPos()),record) >= 0)
					{
						memberList->addData(3,i,record[govpostindex]);
					}
					memberList->addData(4,i,crArgumentParser::appItoa((*cmItr)->getTodayWeekActive()));
					std::string online = "离线";
					if ((*cmItr)->getIfOnline())
						online = "在线";
					memberList->addData(5,i,online);
					memberList->addData(i,(*cmItr)->getPlayerID());
				}
				if (!memberVec->empty())
				{
					memberList->selectRow(memberList->getSelect());
				}
				
				int myGroupPos = 0;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
				PermissionCheck checkPermission;
				int dismissTime = myGroup->getDismissTime();

				std::string groupPosName = "";
				switch (myGroupPos)
				{
				case JXJGroup_FirstLeader:
					groupPosName = "军团长";
					break;
				case JXJGroup_SecondLeader:
					groupPosName = "副军团长";
						break;
				default:
					groupPosName = "普通成员";
					break;
				}
				if (groupPos.valid())
				{
					groupPos->setString(groupPosName);
				}
				if (activeValue.valid())
				{
					if (mySelfInfo)
						activeValue->setString(crArgumentParser::appItoa((mySelfInfo->getTodayWeekActive())));
				}
				if (noticeText.valid())
				{
					noticeText->setEnable(false);
					noticeText->setString(myGroup->getRecruitNotice());
				}

				if (pageInput.valid())
				{
					canvasdata->getParam(WCHDATA_JXJMyGroupMemberCurPage,param);
					int curpage = *(int *)param;
					canvasdata->getParam(WCHDATA_JXJMyGroupMemberTotalPage,param);
					int totalpage = *(int *)param;
					
					pageInput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}

				if (-1 == dismissTime )
				{
					if (switchWidget) switchWidget->setActiveSwitchSet(0);
				}
				else
				{
					if (switchWidget) switchWidget->setActiveSwitchSet(1);
				}

				if (!checkPermission(myGroupPos,JXJGroup_ChangeLeader))
				{
					if (changeLeaderBtn.valid()) changeLeaderBtn->setEnable(false);
					if (changeLeaderSw) changeLeaderSw->setActiveSwitchSet(3);
				}
				else
				{
					if (changeLeaderBtn.valid()) changeLeaderBtn->setEnable(true);
					if (changeLeaderSw) changeLeaderSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_LeaveGroup))
				{
					if (leaveGroupBtn.valid()) leaveGroupBtn->setEnable(false);
					if (leaveGroupSw) leaveGroupSw->setActiveSwitchSet(3);
				}
				else
				{
					if (leaveGroupBtn.valid()) leaveGroupBtn->setEnable(true);
					if (leaveGroupSw) leaveGroupSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_KickMember))
				{
					if (kickOutBtn.valid()) kickOutBtn->setEnable(false);
					if (kickOutSw) kickOutSw->setActiveSwitchSet(3);
				}
				else
				{
					if (kickOutBtn.valid()) kickOutBtn->setEnable(true);
					if (kickOutSw) kickOutSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_DismissGroup))
				{
					if (dismissGroupBtn.valid()) dismissGroupBtn->setEnable(false);
					if (dismissGroupSw) dismissGroupSw->setActiveSwitchSet(3);
				}
				else
				{
					if (dismissGroupBtn.valid()) dismissGroupBtn->setEnable(true);
					if (dismissGroupSw) dismissGroupSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_DismissGroupImmediate))
				{
					if (dismissAtOnceBtn.valid()) dismissAtOnceBtn->setEnable(false);
					if (dismissAtOnceSw) dismissAtOnceSw->setActiveSwitchSet(3);
				}
				else
				{
					if (dismissAtOnceBtn.valid()) dismissAtOnceBtn->setEnable(true);
					if (dismissAtOnceSw) dismissAtOnceSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_CancelDismissGroup))
				{
					if (cancelDismissBtn.valid()) cancelDismissBtn->setEnable(false);
					if (cancelDismissSw) cancelDismissSw->setActiveSwitchSet(3);
				}
				else
				{
					if (cancelDismissBtn.valid()) cancelDismissBtn->setEnable(true);
					if (cancelDismissSw) cancelDismissSw->setActiveSwitchSet(0);
				}

				if (!checkPermission(myGroupPos,JXJGroup_EditNotice))
				{
					if (editBtn.valid()) editBtn->setEnable(false);
					if (editSw) editSw->setActiveSwitchSet(3);
				}
				else
				{
					if (editBtn.valid()) editBtn->setEnable(true);
					if (editSw) editSw->setActiveSwitchSet(0);
				}
				if (!checkPermission(myGroupPos,JXJGroup_PublishNotice))
				{
					if (publicBtn.valid()) publicBtn->setEnable(false);
					if (publicNoticeSw) publicNoticeSw->setActiveSwitchSet(3);
				}
				else
				{
					if (publicBtn.valid()) publicBtn->setEnable(true);
					if (publicNoticeSw) publicNoticeSw->setActiveSwitchSet(0);
				}
			}
		}
	}
}

void JXJ::crJXJUIMyGroupManageUpdateMethod::inputParam( int i, void *param )
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


/************************************************************************/
/*    crJXJUIMyGroupShenHeUpdateMethod                                                                  */
/************************************************************************/
JXJ::crJXJUIMyGroupShenHeUpdateMethod::crJXJUIMyGroupShenHeUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJUIMyGroupShenHeUpdateMethod::crJXJUIMyGroupShenHeUpdateMethod( const crJXJUIMyGroupShenHeUpdateMethod& handle ):
	crMethod(handle),
	m_applyMemberList(handle.m_applyMemberList),
	m_addOther(handle.m_addOther),
	m_allowApply(handle.m_allowApply),
	m_pageInput(handle.m_pageInput),
	m_rejectApply(handle.m_rejectApply),
	m_addOther_sw(handle.m_addOther_sw),
	m_allowApply_sw(handle.m_allowApply_sw),
	m_rejectApply_sw(handle.m_rejectApply_sw)
{
}

void JXJ::crJXJUIMyGroupShenHeUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_applyMemberList = str;
		break;
	case 1:
		m_addOther = str;
		break;
	case 2:
		m_pageInput = str;
		break;
	case 3:
		m_rejectApply = str;
		break;
	case 4:
		m_allowApply = str;
	case 5:
		m_addOther_sw = str;
		break;
	case 6:
		m_rejectApply_sw = str;
		break;
	case 7:
		m_allowApply_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIMyGroupShenHeUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data )
		{
			ref_ptr<crTableWidgetNode>   memberList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_applyMemberList));
			ref_ptr<crStaticTextWidgetNode>  pageInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pageInput));
			ref_ptr<crButtonWidgetNode>  addOtherBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_addOther));
			ref_ptr<crButtonWidgetNode>  rejectBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rejectApply));
			ref_ptr<crButtonWidgetNode>  allowBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_allowApply));

			ref_ptr<crMultiSwitch> addOther_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_addOther_sw));
			ref_ptr<crMultiSwitch> reject_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_rejectApply_sw));
			ref_ptr<crMultiSwitch> allow_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_allowApply_sw));

			crData *canvasdata = m_this->getDataClass();
			if (canvasdata)
			{
				CRCore::ScopedLock<crData> lock(*canvasdata);
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				
				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;

				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());

				int myGroupPos = 0;
				if (mySelfInfo)
					myGroupPos = mySelfInfo->getGroupPos();
				PermissionCheck checkPermission;

				if (checkPermission(myGroupPos , JXJGroup_AddGroupMember))
				{
					if (addOtherBtn.valid()) addOtherBtn->setEnable(true);
					if (addOther_sw.valid()) addOther_sw->setActiveSwitchSet(0);
				}
				else
				{
					if (addOtherBtn.valid()) addOtherBtn->setEnable(false);
					if (addOther_sw.valid()) addOther_sw->setActiveSwitchSet(3);
				}
				if (checkPermission(myGroupPos,JXJGroup_RejectGroupMember))
				{
					if (rejectBtn.valid()) rejectBtn->setEnable(true);
					if (reject_sw.valid()) reject_sw->setActiveSwitchSet(0);
				}
				else
				{
					if (rejectBtn.valid()) rejectBtn->setEnable(false);
					if (reject_sw.valid()) reject_sw->setActiveSwitchSet(3);
				}
				if (checkPermission(myGroupPos,JXJGroup_AgreeGroupMember))
				{
					if (allowBtn.valid()) allowBtn->setEnable(true);
					if (allow_sw.valid()) allow_sw->setActiveSwitchSet(0);
				}
				else
				{
					if (allowBtn.valid()) allowBtn->setEnable(false);
					if (allow_sw.valid()) allow_sw->setActiveSwitchSet(3);
				}

				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo * groupInfo = (crJXJGroupInfo *) param;
				//int dismissTime = groupInfo->getDismissTime();
				//if (-1 != dismissTime)
				//{
				//	if (addOtherBtn.valid()) addOtherBtn->setEnable(false);
				//	if (rejectBtn.valid()) rejectBtn->setEnable(false);
				//	if (allowBtn.valid()) allowBtn->setEnable(false);
				//	if (addOther_sw.valid()) addOther_sw->setActiveSwitchSet(3);
				//	if (reject_sw.valid()) reject_sw->setActiveSwitchSet(3);
				//	if (allow_sw.valid()) allow_sw->setActiveSwitchSet(3);
				//}
			
				canvasdata->getParam(WCHDATA_JXJConsortiaApplyVec,param);
				ConsortiaApplyVec *applyList = (ConsortiaApplyVec *) param;

				ref_ptr<crTableIO> postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
				crTableIO::StrVec record;
				int govpostindex = postTab->getTitleIndex("govpost");
		
				if (memberList.valid()) 
				{
					memberList->clearData();
					if (checkPermission(myGroupPos,JXJGroup_AgreeGroupMember))
					{
						if (!applyList->empty())
						{
							ConsortiaApplyVec::iterator it = applyList->begin();
							char tmp[64];
							long long t ;
							int level;
							for (int row = 0; it != applyList->end() && row < perpagenum; ++ row,++ it)
							{
								if ( (*it).valid())
								{
									t = (*it)->getTime();
									strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
									memberList->addData(0,row,tmp);
									memberList->addData(1,row,(*it)->getName());
									level = (*it)->getLevel();
									memberList->addData(2,row,crArgumentParser::appItoa(level));
									if(postTab->queryOneRecord(0,crArgumentParser::appItoa((*it)->getPos()),record) >= 0)
									{
										memberList->addData(3,row,record[govpostindex]);
									}
									memberList->addData(row,(*it)->getPlayerID());
								}
							}
							if (!applyList->empty())
							{
								memberList->selectRow(memberList->getSelect());
							}
						}
					}
				}
				if (pageInput.valid())
				{
					canvasdata->getParam(WCHDATA_JXJGroupApplyCurPage,param);
					int curpage = *(int *)param;
					canvasdata->getParam(WCHDATA_JXJGroupApplyTotalPage,param);
					int totalpage = *(int *)param;
					pageInput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
				}
			}
		}
	}
}

void JXJ::crJXJUIMyGroupShenHeUpdateMethod::inputParam( int i, void *param )
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

/************************************************************************/
/*    crJXJMyGroupCanvasRadioUpdateMethod                                                                  */
/************************************************************************/
JXJ::crJXJMyGroupCanvasRadioUpdateMethod::crJXJMyGroupCanvasRadioUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJMyGroupCanvasRadioUpdateMethod::crJXJMyGroupCanvasRadioUpdateMethod( const crJXJMyGroupCanvasRadioUpdateMethod & handle ):
	crMethod(handle),
	m_pagesw(handle.m_pagesw),
	m_pageradio(handle.m_pageradio)
{
}
void JXJ::crJXJMyGroupCanvasRadioUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJMyGroupCanvasRadioUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_pageradio = str;
		break;
	case 1:
		m_pagesw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJMyGroupCanvasRadioUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		ref_ptr<crCanvasNode> parentCanvas = m_this->getParentCanvas();
		ref_ptr<crRadioGroupWidgetNode> pageradio = dynamic_cast<crRadioGroupWidgetNode *>(parentCanvas->getWidget(m_pageradio));
		crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_pagesw));
		//void *param;
		int select = 0;
		if (pageradio.valid())
		{
			select = pageradio->getSelect();
		}
		if (pagesw)
		{
			pagesw->setActiveSwitchSet(select);
		}
	}
}

/************************************************************************/
/*    crJXJUIChangeGroupNameInitMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIChangeGroupNameInitMethod::crJXJUIChangeGroupNameInitMethod():
	m_this(NULL)
{

}

JXJ::crJXJUIChangeGroupNameInitMethod::crJXJUIChangeGroupNameInitMethod( const crJXJUIChangeGroupNameInitMethod& handle ):
	crMethod(handle),
	m_info(handle.m_info),
	m_confirmBtn(handle.m_confirmBtn),
	m_nameEdit(handle.m_nameEdit)
{
}

void JXJ::crJXJUIChangeGroupNameInitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIChangeGroupNameInitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_info = str;
		break;
	case 1:
		m_nameEdit = str;
		break;
	case 2:
		m_confirmBtn = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIChangeGroupNameInitMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		
		ref_ptr<crStaticTextWidgetNode>  info = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_info));
		ref_ptr<crEditWidgetNode>  nameInput = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_nameEdit));
		ref_ptr<crButtonWidgetNode>  confirmBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_confirmBtn));

		if (info.valid())
		{
			std::string infoStr = "改名需缴纳200礼金/元宝，请在下方输入名称：";
			info->setString(infoStr);
		}
		if (nameInput.valid())
		{
			nameInput->setEnable(true);
		}
		if (confirmBtn.valid())
		{
			confirmBtn->setVisiable(true);
		}
	}
}


JXJ::crJXJUIMyGroupFuLiUpdateMethod::crJXJUIMyGroupFuLiUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJUIMyGroupFuLiUpdateMethod::crJXJUIMyGroupFuLiUpdateMethod( const crJXJUIMyGroupFuLiUpdateMethod & handle ):
	crMethod(handle),
	m_groupLevel(handle.m_groupLevel),
	m_groupCurZijin(handle.m_groupCurZijin),
	m_groupLvUp(handle.m_groupLvUp),
	m_Crops3Raido(handle.m_Crops3Raido),
	m_Money(handle.m_Money),
	m_LiangShi(handle.m_LiangShi),
	m_MuCai(handle.m_MuCai),
	m_TieKuang(handle.m_TieKuang),
	m_MaPi(handle.m_MaPi),
	m_JuanXian(handle.m_JuanXian),
	m_Tishi1(handle.m_Tishi1),
	m_JunTuanFuBen_1(handle.m_JunTuanFuBen_1),
	m_JunTuanFuBenname(handle.m_JunTuanFuBenname),
	m_QianwangfubenBtn(handle.m_QianwangfubenBtn),
	m_Crops3_Radio(handle.m_Crops3_Radio),
	m_Tishi2(handle.m_Tishi2),
	m_sw135(handle.m_sw135),
	m_jiluBtn(handle.m_jiluBtn),
	m_cunbingBtn(handle.m_cunbingBtn),
	m_ListScroll1(handle.m_ListScroll1),
	m_Bar(handle.m_Bar),
	m_curgongxian(handle.m_curgongxian),
	m_shuaxinBtn(handle.m_shuaxinBtn),
	m_sw164(handle.m_sw164),
	m_sw163(handle.m_sw163)
{
	for (unsigned char i = 0; i < 8 ;++ i)
	{
		m_icon[i] = handle.m_icon[i];
		m_input1[i] = handle.m_input1[i];
		m_input2[i] = handle.m_input2[i];
		m_goumai[i] = handle.m_goumai[i];
		m_quchu[i] = handle.m_quchu[i];
		if (i<4)
		{
			m_icon2[i] = handle.m_icon2[i];
		}
	}
}

void JXJ::crJXJUIMyGroupFuLiUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIMyGroupFuLiUpdateMethod::addParam( int i, const std::string& str )
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
		m_icon[i] = str;
		break;
	case 8:
		m_groupLevel = str;
		break;
	case 9:
		m_groupCurZijin = str;
		break;
	case 10:
		m_groupLvUp = str;
		break;
	case 11:
		m_Crops3Raido = str;
		break;
	case 12:
		m_Money = str;
		break;
	case 13:
		m_LiangShi = str;
		break;
	case 14:
		m_MuCai = str;
		break;
	case 15:
		m_TieKuang = str;
		break;
	case 16:
		m_MaPi = str;
		break;
	case 17:
		m_JuanXian = str;
		break;
	case 18:
		m_Tishi1 = str;
		break;
	case 19:
		m_JunTuanFuBen_1 = str;
		break;
	case 20:
		m_JunTuanFuBenname = str;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
		m_icon2[i-21] = str;
		break;
	case 25:
		m_QianwangfubenBtn = str;
		break;
	case 26:
		m_Crops3_Radio = str;
		break;
	case 27:
		m_Tishi2 = str;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
		m_input1[i-28] = str;
		break;
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
		m_input2[i-36] = str;
		break;
	case 44:
		m_sw135 = str;
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
		m_goumai[i-45] = str;
		break;
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	case 60:
		m_quchu[i-53] = str;
		break;
	case 61:
		m_jiluBtn = str;
		break;
	case 62:
		m_cunbingBtn = str;
		break;
	case 63:
		m_ListScroll1 = str;
		break;
	case 64:
		m_Bar = str;
		break;
	case 65:
		m_curgongxian = str;
		break;
	case 66:
		m_shuaxinBtn = str;
		break;
	case 67:
		m_sw164 = str;
		break;
	case 68:
		m_sw163 = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIMyGroupFuLiUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data)
		{
			ref_ptr<crMultiSwitch> sw135 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw135));
			ref_ptr<crMultiSwitch> sw164 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw164)); 
			ref_ptr<crMultiSwitch> sw163 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw163)); 
			ref_ptr<crStaticTextWidgetNode>  groupLevel = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupLevel));
			ref_ptr<crStaticTextWidgetNode>  groupCurZijin = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_groupCurZijin));
			ref_ptr<crStaticTextWidgetNode>  curgongxian = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_curgongxian));
			ref_ptr<crStaticTextWidgetNode>  needgongxian[8];
			ref_ptr<crStaticTextWidgetNode>  num[8];

			ref_ptr<crButtonWidgetNode> groupLvUpBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_groupLvUp));
			ref_ptr<crButtonWidgetNode> JuanXianBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_JuanXian));
			ref_ptr<crButtonWidgetNode> QianwangfubenBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_QianwangfubenBtn));
			ref_ptr<crButtonWidgetNode> jiluBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_jiluBtn));
			ref_ptr<crButtonWidgetNode> cunbingBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_cunbingBtn));
			ref_ptr<crButtonWidgetNode> shuaxinBtn =  dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_shuaxinBtn));
			ref_ptr<crButtonWidgetNode> goumaiBtn[8];
			ref_ptr<crButtonWidgetNode> quchuBtn[8];
			ref_ptr<crImageBoxWidgetNode>imageBox[8];
			ref_ptr<crImageBoxWidgetNode>imageBox2[4];
			ref_ptr<crRadioGroupWidgetNode> Crops3_Radio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_Crops3_Radio));
			ref_ptr<crRadioGroupWidgetNode> Crops3Raido = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_Crops3Raido));
			ref_ptr<crImageBoxWidgetNode> fubenimage = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_JunTuanFuBen_1));
			ref_ptr<crStaticTextWidgetNode>  fubenname = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_JunTuanFuBenname));
			for (unsigned char i = 0;i<8;++i)
			{
				imageBox[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
				goumaiBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goumai[i]));
				quchuBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_quchu[i]));
				needgongxian[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1[i]));
				num[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2[i]));
				if (i<4)
				{
					imageBox2[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon2[i]));
				}
			}

			ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_ListScroll1));

			if (groupLvUpBtn.valid())
			{
				groupLvUpBtn->setVisiable(false);
			}
			if (JuanXianBtn.valid())
			{
				JuanXianBtn->setVisiable(true);
			}
			if (QianwangfubenBtn.valid())
			{
				QianwangfubenBtn->setVisiable(true);
			}
			if (jiluBtn.valid())
			{
				jiluBtn->setVisiable(true);
			}
			if (cunbingBtn.valid())
			{
				cunbingBtn->setVisiable(true);
			}
			if (shuaxinBtn.valid())
			{
				shuaxinBtn->setVisiable(true);
			}

			for (unsigned char i = 0;i<8;++i)
			{
				if (goumaiBtn[i].valid())
				{
					goumaiBtn[i]->setVisiable(false);
				}
				if (quchuBtn[i].valid())
				{
					quchuBtn[i]->setEnable(false);
					quchuBtn[i]->setVisiable(false);
				}
				if (imageBox[i].valid())
				{
					imageBox[i]->setImageName("UI_Army_Gong_N.img");
					imageBox[i]->setImageName("");
				}
				if (needgongxian[i].valid())
				{
					needgongxian[i]->setString(L"");
				}
				if (num[i].valid())
				{
					num[i]->setString(L"");
				}
				if (i<4 && imageBox2[i].valid())
				{
					imageBox2[i]->setVisiable(true);
					if (i % 2 == 0)
					{
						imageBox2[i]->setImageName("T_Icon_Task_Gifts.img");
					}
					else
					{
						imageBox2[i]->setImageName("T_Icon_Task_EXP.img");
					}
				}
			}
			if (fubenimage.valid())
			{
				fubenimage->setImageName("UI_Crops_Fuben1.img");
			}
			if (fubenname.valid())
			{
				fubenname->setString(L"军团副本");
			}

			if (Crops3_Radio.valid() && sw135.valid())
			{
				//sw135->setActiveSwitchSet(Crops3_Radio->getSelect());
				sw135->setActiveSwitchSet(1);
			}
			if (sw164.valid())
			{
				sw164->setActiveSwitchSet(1);
			}
			if (sw163.valid())
			{
				sw163->setActiveSwitchSet(1);
			}
			//data->getParam(WCHDATA_JXJGroupContributeiType,param);
			//unsigned char contributeType = *(unsigned char *)param;
			//switch (contributeType)
			//{
			//case JXJGroupContributeType_yuanbao1:
			//case JXJGroupContributeType_yuanbao2:
			//case JXJGroupContributeType_yuanbao3:
			//case JXJGroupContributeType_yuanbao4:
			//	{
			//		unsigned char idx = contributeType - 1;
			//		if (Crops3Raido.valid())
			//		{
			//			Crops3Raido->select(idx);
			//		}
			//	}
			//	break;
			//default:
			//	break;
			//}

			ref_ptr<crTableIO>  contributeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupContributeTab);
			ref_ptr<crTableIO>  groupUpgradeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);
			ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
			/*ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);*/
			crTableIO::StrVec record;

			crData *canvasdata = m_this->getDataClass();
			CRCore::ScopedLock<crData> lock(*canvasdata);

			if(contributeTab.valid()
				&& groupUpgradeTab.valid()
				&& troopsTab.valid()
				&& canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;
				if (groupLevel.valid())
				{
					groupLevel->setString(crArgumentParser::appItoa(myGroup->getLv()));
				}
				int fundsIdx = groupUpgradeTab->getTitleIndex("升级资金");
				if (groupUpgradeTab->queryOneRecord(0,crArgumentParser::appItoa(myGroup->getLv()),record) >= 0)
				{
					if (groupCurZijin.valid())
					{
						groupCurZijin->setString(crArgumentParser::appItoa(myGroup->getFunds()) + " / " + record[fundsIdx]);
					}
				}

				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				int curcontribute = 0;
				if (mySelfInfo) 
				{
					curcontribute = mySelfInfo->getGroupContribute();
					curgongxian->setString(crArgumentParser::appItoa(curcontribute));
					myGroupPos = mySelfInfo->getGroupPos();
				}
				if (myGroupPos != JXJGroup_Normal)
				{
					if (groupLvUpBtn.valid())
					{
						groupLvUpBtn->setVisiable(true);
						if (myGroup->getFunds() < atoi(record[fundsIdx].c_str()))
						{
							groupLvUpBtn->setEnable(false);
						}
						else
						{
							groupLvUpBtn->setEnable(true);
						}
					}
				}
				if (QianwangfubenBtn.valid())
				{
					canvasdata->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
					StartedFubenMap * startedFubenMap = (StartedFubenMap *)param;
					if (startedFubenMap->empty())
					{
						QianwangfubenBtn->setEnable(false);
					}
					else
					{
						QianwangfubenBtn->setEnable(true);
					}
				}
				

				if (contributeTab->queryOneRecord(0,crArgumentParser::appItoa(JXJGroupContributeType_yuanbao1),record) >= 0)
				{
					//data->getParam(WCHDATA_JXJGroupContributeiType,param);
					//unsigned char contributeType = *(unsigned char *)param;

					data->getParam(WCHDATA_JXJGroupContributeiTimes,param);
					unsigned char contributeTimes = *(unsigned char *)param;

					int timesPerDayIdx = contributeTab->getTitleIndex("每日次数上限");
					int a = atoi(record[timesPerDayIdx].c_str());
					if (contributeTimes < atoi(record[timesPerDayIdx].c_str()))
					{
						if (JuanXianBtn.valid())
						{
							JuanXianBtn->setEnable(true)	;
						}
					}
					else
					{
						if (JuanXianBtn.valid())
						{
							JuanXianBtn->setEnable(false);
						}
					}
				}

				crRole *mainRole = playerGameData->getMainRole();
				crData *mainroledata = mainRole->getDataClass();
				mainroledata->getParam(WCHDATA_JXJGovPost,param);
				unsigned char post = *(unsigned char*)param;
				ref_ptr<crTableIO> govpostTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
				crTableIO::StrVec record;
				int qubinglimitcount = 0;
				int cunbinglimitcount = 0;
				
				if (mySelfInfo)
				{
					if(govpostTab.valid())
					{
						int quLimitIdx = govpostTab->getTitleIndex("军团取兵上限");
						int cunLimitIdx = govpostTab->getTitleIndex("军团存兵上限");
						govpostTab->queryOneRecord(0,crArgumentParser::appItoa(post),record);
						qubinglimitcount = atoi(record[quLimitIdx].c_str()) - mySelfInfo->getTroopTakedNumT();
						cunbinglimitcount = atoi(record[cunLimitIdx].c_str()) - mySelfInfo->getTroopStoredNumT();
					}
				}
				if (0 == cunbinglimitcount && cunbingBtn.valid())
				{
					cunbingBtn->setEnable(false);
				}
				else
				{
					cunbingBtn->setEnable(true);
				}

				canvasdata->getParam(WCHDATA_JXJGroupTroopsMap,param);
				TroopsMap * grouptroopsmap = (TroopsMap *)param;

				int val = 0;
				int cRow = grouptroopsmap->size();
				if (scroll.valid())
				{
					if (cRow < 8)
					{
						scroll->setEnable(false);
					}
					else
					{
						scroll->setEnable(true);
						int crows = cRow%4==0? cRow/4 : cRow/4+1;
						scroll->setRange(0, crows-2);
						scroll->setLineValue(1.0f);
						scroll->setPageValue(1.0f);
					}
					val = scroll->getValue();
				}
				int _index = 4 * val;
	/*			ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				if(!troopsTab->validTab()) return;*/
				int iconIdx = troopsTab->getTitleIndex("icon");
				int gongxianIdx = troopsTab->getTitleIndex("花费军团贡献");
				int i = 0;
				int j = 0;
				int endindex = _index + 8;
				
				for( TroopsMap::iterator itr = grouptroopsmap->begin();
					itr != grouptroopsmap->end() && i< endindex;
					++itr,i++ )
				{
					if(i>=_index)
					{	
						if (itr->second <= 0)
						{
							++endindex;
							continue;
						}
						if (j < 8 && imageBox[j].valid() && num[j].valid() && needgongxian[j].valid())
						{
							if(troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>= 0)
							{
								if (!record.empty())
								{
									imageBox[j]->setImageName(record[iconIdx]);
									num[j]->setString(crArgumentParser::appItoa(itr->second));
									needgongxian[j]->setString(record[gongxianIdx]);
									int temp =atoi(record[gongxianIdx].c_str());
									if (quchuBtn[j].valid())
									{
										quchuBtn[j]->setVisiable(true);
										if (curcontribute >=temp && qubinglimitcount > 0 && itr->second > 0)
										{
											quchuBtn[j]->setEnable(true);
										}
									}
								}
							}
						}
						++j;
					}
				}
			}
		}
		ref_ptr<crRadioWidgetNode> cangku = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget("JunTuanCangKu"));
		if (cangku.valid())
		{
			cangku->setVisiable(false);
		}
	}
}


/************************************************************************/
/*    crJXJUIGroupContributeMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIGroupContributeMethod::crJXJUIGroupContributeMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIGroupContributeMethod::crJXJUIGroupContributeMethod( const crJXJUIGroupContributeMethod & handle ):
	crMethod(handle),
//	m_contributeType(handle.m_contributeType),
	m_resource  (handle.m_resource),
	m_yuanBao1  (handle.m_yuanBao1),
	m_yuanBao2  (handle.m_yuanBao2),
	m_yuanBao3  (handle.m_yuanBao3),
	m_yuanBao4  (handle.m_yuanBao4),
	m_canvas	(handle.m_canvas)
{
}

void JXJ::crJXJUIGroupContributeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIGroupContributeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_yuanBao1 = str;
		break;
	case 1:
		m_yuanBao2 = str;
		break;
	case 2:
		m_yuanBao3 = str;
		break;
	case 3:
		m_yuanBao4 = str;
		break;
	case 4:
		m_resource = str;
		break;
	case 5:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIGroupContributeMethod::operator()( crHandle &handle )
{
	void *param;
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = player->getDataClass();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	//unsigned char contributeType = crArgumentParser::appAtoI64(m_contributeType);
	unsigned char contributeType = 0;

	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() == false) return;
	/*ref_ptr<crRadioWidgetNode >  resourceRadio = dynamic_cast<crRadioWidgetNode*>(canvas->getWidget(m_resource));*/
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio1= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao1));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio2= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao2));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio3= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao3));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio4= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao4));
	/*if (resourceRadio->getSelect())
	contributeType = JXJGroupContributeType_resource;*/
	if (yuanBaoRadio1->getSelect())
		contributeType = JXJGroupContributeType_yuanbao1;
	if (yuanBaoRadio2->getSelect())
		contributeType = JXJGroupContributeType_yuanbao2;
	if (yuanBaoRadio3->getSelect())
		contributeType = JXJGroupContributeType_yuanbao3;
	if (yuanBaoRadio4->getSelect())
		contributeType = JXJGroupContributeType_yuanbao4;

	ref_ptr<crTableIO>  contributeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupContributeTab);
	crTableIO::StrVec record;
	bool suc = false;
	if (contributeTab->queryOneRecord(0,crArgumentParser::appItoa(contributeType),record) >= 0)
	{
		int yuanBaoIdx = contributeTab->getTitleIndex("消耗元宝");
		int yuanBao = atoi(record[yuanBaoIdx].c_str());

		data->getParam(WCHDATA_JXJGoldingot,param);
		int myYuanBao = *(int *)param;
		data->getParam(WCHDATA_JXJGiftGold,param);
		int myGiftGold = *(int *)param;
		switch (contributeType)
		{
			/*case JXJGroupContributeType_resource:
			{
			int copperIdx = 0, foodIdx = 0,woodIdx = 0,feIdx = 0,horseIdx = 0;
			copperIdx = contributeTab->getTitleIndex("消耗铜钱");
			foodIdx = contributeTab->getTitleIndex("消耗粮食");
			woodIdx = contributeTab->getTitleIndex("消耗木材");
			feIdx = contributeTab->getTitleIndex("消耗铁矿");
			horseIdx = contributeTab->getTitleIndex("消耗马匹");

			data->getParam(WCHDATA_JXJCoppercash,param);
			int copper = *(int *)param;
			data->getParam(WCHDATA_JXJFood,param);
			int food = *(int *)param;
			data->getParam(WCHDATA_JXJWood,param);
			int wood = *(int *)param;
			data->getParam(WCHDATA_JXJIron,param);
			int iron = *(int *)param;
			data->getParam(WCHDATA_JXJHorse,param);
			int horse = *(int *)param;

			if (
			copper >= atoi(record[copperIdx].c_str())
			&& food >= atoi(record[foodIdx].c_str())
			&& wood >= atoi(record[woodIdx].c_str())
			&& iron >= atoi(record[feIdx].c_str())
			&& horse >= atoi(record[horseIdx].c_str())
			)
			{
			suc = true;
			}
			else
			{
			int msgid = 3026;
			player->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
			}
			break;*/
		case JXJGroupContributeType_yuanbao1:
		case JXJGroupContributeType_yuanbao2:
		case JXJGroupContributeType_yuanbao3:
		case JXJGroupContributeType_yuanbao4:
			{
				if (myYuanBao >= yuanBao || myGiftGold >= yuanBao)
					suc = true;
				else
				{
					int msgid = 3025;
					player->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
				}
			}
			break;
		default:
			break;
		}
	}
	if (suc)
	{
		//switch (contributeType)
		//{
		//	/*case JXJGroupContributeType_resource:
		//	{
		//	if (yuanBaoRadio1.valid() && yuanBaoRadio2.valid() && yuanBaoRadio3.valid() && yuanBaoRadio4.valid())
		//	{
		//	yuanBaoRadio1->setEnable(false);
		//	yuanBaoRadio2->setEnable(false);
		//	yuanBaoRadio3->setEnable(false);
		//	yuanBaoRadio4->setEnable(false);
		//	}
		//	}
		//	break;*/
		//case JXJGroupContributeType_yuanbao1:
		//	{
		//		if (/*resourceRadio.valid() && */yuanBaoRadio2.valid() && yuanBaoRadio3.valid() && yuanBaoRadio4.valid())
		//		{
		//			/*resourceRadio->setEnable(false);*/
		//			yuanBaoRadio2->setEnable(false);
		//			yuanBaoRadio3->setEnable(false);
		//			yuanBaoRadio4->setEnable(false);
		//		}
		//	}
		//	break;
		//case JXJGroupContributeType_yuanbao2:
		//	{
		//		if (/*resourceRadio.valid() && */yuanBaoRadio1.valid() && yuanBaoRadio3.valid() && yuanBaoRadio4.valid())
		//		{
		//			/*resourceRadio->setEnable(false);*/
		//			yuanBaoRadio1->setEnable(false);
		//			yuanBaoRadio3->setEnable(false);
		//			yuanBaoRadio4->setEnable(false);
		//		}
		//	}
		//	break;
		//case JXJGroupContributeType_yuanbao3:
		//	{
		//		if (/*resourceRadio.valid() && */yuanBaoRadio1.valid() && yuanBaoRadio2.valid() && yuanBaoRadio4.valid())
		//		{
		//			/*resourceRadio->setEnable(false);*/
		//			yuanBaoRadio1->setEnable(false);
		//			yuanBaoRadio2->setEnable(false);
		//			yuanBaoRadio4->setEnable(false);
		//		}
		//	}
		//	break;
		//case JXJGroupContributeType_yuanbao4:
		//	{
		//		if (/*resourceRadio.valid() && */yuanBaoRadio1.valid() && yuanBaoRadio2.valid() && yuanBaoRadio3.valid())
		//		{
		//			/*resourceRadio->setEnable(false);*/
		//			yuanBaoRadio1->setEnable(false);
		//			yuanBaoRadio2->setEnable(false);
		//			yuanBaoRadio3->setEnable(false);
		//		}
		//	}
		//	break;
		//default:
		//	break;
		//}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(contributeType);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupContribute,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/************************************************************************/
/*    crJXJRecvGroupContributeMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvGroupContributeMethod::crJXJRecvGroupContributeMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvGroupContributeMethod::crJXJRecvGroupContributeMethod( const crJXJRecvGroupContributeMethod& handle ):
	m_this(NULL),
	m_stream(NULL),
	m_canvasName(handle.m_canvasName),
	crMethod(handle)
{
}

void JXJ::crJXJRecvGroupContributeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvGroupContributeMethod::addParam( int i, const std::string& str )
{
	switch (0)
	{
	case 0:
		m_canvasName = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvGroupContributeMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char contributeType = m_stream->_readUChar();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{

				ref_ptr<crTableIO>  contributeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupContributeTab);
				ref_ptr<crTableIO>  groupUpdateTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);
				crTableIO::StrVec record;

				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJGroupContributeiType,param);
				unsigned char type = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJGroupContributeiTimes,param);
				unsigned char times = *(unsigned char *)param;

				if (mygroupid > 0 && contributeTab->queryOneRecord(0,crArgumentParser::appItoa(contributeType),record) >= 0)
				{
					int yuanBaoIdx = contributeTab->getTitleIndex("消耗元宝");
					int yuanBao = atoi(record[yuanBaoIdx].c_str());
					int copperIdx = 0, foodIdx = 0,woodIdx = 0,feIdx = 0,horseIdx = 0;
					int groupAddFundsIdx = 0,mySelfAddIdx = 0,timesPerDayIdx = 0;

					timesPerDayIdx = contributeTab->getTitleIndex("每日次数上限");
					copperIdx = contributeTab->getTitleIndex("消耗铜钱");
					foodIdx = contributeTab->getTitleIndex("消耗粮食");
					woodIdx = contributeTab->getTitleIndex("消耗木材");
					feIdx = contributeTab->getTitleIndex("消耗铁矿");
					horseIdx = contributeTab->getTitleIndex("消耗马匹");
					groupAddFundsIdx = contributeTab->getTitleIndex("军团增加资金");
					mySelfAddIdx = contributeTab->getTitleIndex("个人收益");

					unsigned char suc = JXJGroupContributeReturnVal_failed_unkwon;
					ref_ptr<crData>consortiaData;
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					//gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					if(itr != consortiamap->end())
					{
						consortiaData = itr->second->getDataClass();
					}
					do 
					{
						//ref_ptr<crStreamBuf> stream = new crStreamBuf;
						do 
						{
							//if (itr == consortiamap->end()) break;
							//crData *consortiaData = itr->second->getDataClass();
							if (!consortiaData.valid()) break;
							consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
							ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
							ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->find(playerid);
							if (cmmIt == consortiaMemberMap->end()) break;
							if (! (cmmIt->second.valid()) ) break;

							//if (type && contributeType != type)
							//{
							//	suc = JXJGroupContributeReturnVal_failed_onlyOneKind;
							//	break;
							//}
							//if (times >= 1)
							//{

							//}
							if (times >= atoi(record[timesPerDayIdx].c_str()))
							{
								suc = JXJGroupContributeReturnVal_failed_noTimes;
								break;
							}

							
							suc = JXJGroupContributeReturnVal_suc;
						} while (0);
						//gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

						if (suc != JXJGroupContributeReturnVal_suc) break;
						suc = JXJGroupContributeReturnVal_failed_unkwon;

						data->getParam(WCHDATA_JXJCoppercash,param);
						int copper = *(int *)param;
						data->getParam(WCHDATA_JXJFood,param);
						int food = *(int *)param;
						data->getParam(WCHDATA_JXJWood,param);
						int wood = *(int *)param;
						data->getParam(WCHDATA_JXJIron,param);
						int iron = *(int *)param;
						data->getParam(WCHDATA_JXJHorse,param);
						int horse = *(int *)param;
						data->getParam(WCHDATA_JXJGoldingot,param);
						int myYuanBao = *(int *)param;
						switch (contributeType)
						{
						//case JXJGroupContributeType_resource:
						//	{
						//		int costCopper = atoi(record[copperIdx].c_str());
						//		int costFood = atoi(record[foodIdx].c_str());
						//		int costWood = atoi(record[woodIdx].c_str());
						//		int costIron = atoi(record[feIdx].c_str());
						//		int costHorse = atoi(record[horseIdx].c_str());

						//		if (
						//			copper >= costCopper 
						//			&& food >= costFood
						//			&& wood >= costWood
						//			&& iron >= costIron
						//			&& horse >= costHorse
						//			)
						//		{
						//			int copperCost = costCopper;
						//			m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&copperCost,NULL));

						//			ResourceChangeData foodData(T_Food,costFood);
						//			ResourceChangeData woodData(T_Wood,costWood);
						//			ResourceChangeData ironData(T_Iron,costIron);
						//			ResourceChangeData horseData(T_Horse,costHorse);
						//			m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&foodData,NULL));
						//			m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&woodData,NULL));
						//			m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&ironData,NULL));
						//			m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&horseData,NULL));

						//			suc = JXJGroupContributeReturnVal_suc;


						//			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
						//			crTableIO::StrVec textrecord;
						//			char szNotice[256];
						//			int textid = 4076;
						//			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
						//			{
						//				sprintf_s(szNotice,textrecord[1].c_str(),m_this->getCharacterName().c_str(), costFood,costWood,costIron,costHorse);
						//				CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);

						//				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						//				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
						//				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
						//			}
						//			//ref_ptr<JXJGroupContributeSourceMsg > msg =  new JXJGroupContributeSourceMsg ;
						//			//msg->m_name = m_this->getCharacterName();
						//			//msg->m_food = costFood;
						//			//msg->m_wood = costWood;
						//			//msg->m_iron = costIron;
						//			//msg->m_horse = costHorse;
						//			//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(msg.get(),mygroupid));
						//		}
						//		else
						//		{
						//			suc = JXJGroupContributeReturnVal_failed_res;
						//		}
						//	}
						//	break;
						case JXJGroupContributeType_yuanbao1:
						case JXJGroupContributeType_yuanbao2:
						case JXJGroupContributeType_yuanbao3:
						case JXJGroupContributeType_yuanbao4:
							{
								bool needdeductgold = false;
								int tmpCostYuanBao = yuanBao;
								MoneyChangeData moneydata(yuanBao,"军团捐献");
								m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
								if (moneydata.first == -1)
									suc = JXJGroupContributeReturnVal_failed_yuanbao;
								else if (moneydata.first == 0)
									suc = JXJGroupContributeReturnVal_suc;

								if (JXJGroupContributeReturnVal_suc == suc)
								{
									ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									crTableIO::StrVec textrecord;
									char szNotice[256];
									int textid = 4074;
									if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
									{
										sprintf_s(szNotice,textrecord[1].c_str(), m_this->getCharacterName().c_str(),tmpCostYuanBao);
										CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
										crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									}
									//ref_ptr<JXJGroupContributeYuanbaoMsg> msg =  new JXJGroupContributeYuanbaoMsg;
									//msg->m_name = m_this->getCharacterName();
									//msg->m_yuanbao = tmpCostYuanBao;
									//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(msg.get(),mygroupid));
								}
							}
							break;
						default:
							suc = JXJGroupContributeReturnVal_failed_unkwon; 
							break;
						}

					} while (0);
					int funds = 0;
					int addfunds = 0;
					int groupContribute  = 0;
					unsigned char groupLevel = 0;
					unsigned char contributeTimes = 0;
					if (JXJGroupContributeReturnVal_suc == suc)
					{
						// 增加活跃度
						m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_GroupContribution, 1));

						//crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
						//gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						//gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);

						//ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						//ConsortiaMap::iterator itr = consortiamap->find(mygroupid);;
						int addval = 0;
						if (consortiaData.valid())
						{
							//crData *consortiaData = itr->second->getDataClass();
							ref_ptr<crTableIO>  CoefficientTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCoefficientTab);
							crTableIO::StrVec corecord;
							if (CoefficientTab.valid())
							{
								crRole *mainRole = m_this->getMainRole();
								crData *roleData = mainRole->getDataClass();
								roleData->getParam(WCHDATA_Level,param);
								unsigned char rolelv = *(unsigned char *)param;
								int LvIdx = CoefficientTab->getTitleIndex("Level");
								int juanxianIdx = CoefficientTab->getTitleIndex("军团捐献系数");
								if (CoefficientTab->queryOneRecord(LvIdx,crArgumentParser::appItoa(rolelv),corecord) >= 0)
								{
									float xishu = atof(corecord[juanxianIdx].c_str());
									addfunds = atoi(record[groupAddFundsIdx].c_str());
									int testExp = addfunds;
									crServerBrainHandle::getInstance()->doEvent(WCH_JXJGroupFundsUpgrade,MAKEINT64(&testExp,mygroupid));

									consortiaData->getParam(WCHDATA_JXJConsortiaFunds,param);
									funds = *(int *)param;
									consortiaData->getParam(WCHDATA_JXJConsortialv,param);
									groupLevel = *(unsigned char *)param;
									addval = atoi(record[mySelfAddIdx].c_str()) * xishu;

									consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
									ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
									ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->find(playerid);
									if (cmmIt != consortiaMemberMap->end() && cmmIt->second.valid())
									{
										groupContribute = cmmIt->second->getGroupContribute() + addval;
										cmmIt->second->setGroupContribute(groupContribute);
									}
									if (addval > 0)
									{
										char szNotice[256];
										sprintf_s(szNotice,"获得%d点军团贡献！", addval);
										CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
										crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
										gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									}
									//consortiaData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
									//ConsortiaMemberMap *consortiaMemberMap  = (ConsortiaMemberMap *)param;
									//ConsortiaMemberMap::iterator cmmIt = consortiaMemberMap->find(playerid);

									//if (consortiaMemberMap->end() != cmmIt)
									//{
									//	type = contributeType;
									//	++ times;
									//	data->inputParam(WCHDATA_JXJGroupContributeiType,&type);
									//	data->inputParam(WCHDATA_JXJGroupContributeiTimes,&times);
									//	contributeTimes = times;
									//}
									bool modify = true;
									consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
								}
							}
						}
						//gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

						/*data->getParam(WCHDATA_JXJCountryContribute,param);
						countryContribute = *(int *)param;
						if (addval)
						{
						countryContribute += addval;
						}
						data->inputParam(WCHDATA_JXJCountryContribute,&countryContribute);*/

						type = contributeType;
						++times;
						data->inputParam(WCHDATA_JXJGroupContributeiType,&type);
						data->inputParam(WCHDATA_JXJGroupContributeiTimes,&times);
						contributeTimes = times;

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(5);
						stream->_writeUChar(suc);
						if (addfunds) 
						{
							stream->_writeInt(funds);
							stream->_writeUChar(groupLevel);
							stream->_writeUChar(contributeType);
							stream->_writeUChar(contributeTimes);
							stream->_writeInt(groupContribute);

							ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
							crTableIO::StrVec textrecord;
							char szNotice[256];
							int textid = 3074;
							if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
							{
								sprintf_s(szNotice,textrecord[1].c_str(), addfunds);
								CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
								gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
								crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
								gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
							}

							//ref_ptr<JXJGroupExpChatMsg> chatMsg = new JXJGroupExpChatMsg;
							//chatMsg->m_exp = addexp;
							//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(chatMsg.get(),mygroupid));

							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupContribute,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							///游戏日志
							std::string logdata = crArgumentParser::appItoa(suc);
							GameLogData gamelog(Log_GroupContribute,logdata);
							crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
						}
					}
					else
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(4);
						stream->_writeUChar(suc);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupContribute,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						///游戏日志
						std::string logdata = crArgumentParser::appItoa(suc);
						GameLogData gamelog(Log_GroupContribute,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc= m_stream->_readUChar();

			int msgid = 0;
			switch (suc)
			{
			case JXJGroupContributeReturnVal_failed_res:
				msgid = 3026;
				break;
			case JXJGroupContributeReturnVal_failed_yuanbao:
				msgid = 3025;
				break;
			case JXJGroupContributeReturnVal_failed_noTimes:
				msgid = 3024;
				break;
			case JXJGroupContributeReturnVal_failed_onlyOneKind:
				msgid = 3027;
				break;
			case JXJGroupContributeReturnVal_suc:
				{
					int exp = m_stream->_readInt();
					unsigned char groupLevel = m_stream->_readUChar();
					unsigned char contributeType = m_stream->_readUChar();
					unsigned char contributeTimes = m_stream->_readUChar();
					int groupContribute = m_stream->_readInt();
					void *param;
					ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
					if (groupCanvas.valid() && groupCanvas->getDataClass())
					{
						crData * canvasdata = groupCanvas->getDataClass();
						canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
						canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
						crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;
						myGroup->setFunds(exp);
						myGroup->setLv(groupLevel);
						canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
						crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
						if (mySelfInfo) 
						{
							mySelfInfo->setGroupContribute(groupContribute);
						}
						canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
					}

					crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
					if (player && player->getDataClass())
					{
						crData *data = player->getDataClass();
						data->inputParam(WCHDATA_JXJGroupContributeiType	,&contributeType);
						data->inputParam(WCHDATA_JXJGroupContributeiTimes,&contributeTimes);
						/*data->getParam(WCHDATA_JXJCountryContribute,param);
						int extra = countryContribute-(*(int *)param);
						if(extra!=0)
						{
						data->inputParam(WCHDATA_JXJCountryContribute,&countryContribute);
						crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Contribute,extra));
						}*/
					}
					groupCanvas->doEvent(WCH_UI_UpdateData);
					//msgid = 3023;
				}
				break;
			default:
				break;
			}
			//m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
		}
	}
}

/************************************************************************/
/*    crJXJUIGroupIntoFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIGroupIntoFuBenMethod::crJXJUIGroupIntoFuBenMethod():
	m_this(NULL)
{

}

JXJ::crJXJUIGroupIntoFuBenMethod::crJXJUIGroupIntoFuBenMethod( const crJXJUIGroupIntoFuBenMethod & handle ):
	m_canvas(handle.m_canvas),
	m_radio1(handle.m_radio1),
	crMethod(handle)
{

}

void JXJ::crJXJUIGroupIntoFuBenMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIGroupIntoFuBenMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radio1 = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIGroupIntoFuBenMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor && canvas.valid() && canvas->getDataClass())
		{
			int fubenId = 0;
			ref_ptr<crRadioGroupWidgetNode> radio1 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio1));
			if(radio1.valid())
			{
				int idx = radio1->getSelect();
				ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				crTableIO::DataVec  dataVec;
				int fubenType = 1;
				if (fubenTab.valid() )
				{
					dataVec.clear();
					fubenTab->queryRecords(2,crArgumentParser::appItoa(fubenType),dataVec);

					int fubenIdIdx = fubenTab->getTitleIndex("id");
					if (idx < dataVec.size())
					{
						fubenId = crArgumentParser::appAtoI64(dataVec[idx][fubenIdIdx]);
					}
				}
			}
			else
			{
				fubenId = 3000;
			}
			if (fubenId > 0)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(fubenId);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupIntoFuBen,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvGroupIntoFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvGroupIntoFuBenMethod::crJXJRecvGroupIntoFuBenMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvGroupIntoFuBenMethod::crJXJRecvGroupIntoFuBenMethod( const crJXJRecvGroupIntoFuBenMethod& handle ):
	m_this(NULL),
	m_stream(NULL),
	crMethod(handle)
{

}

void JXJ::crJXJRecvGroupIntoFuBenMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvGroupIntoFuBenMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJRecvGroupIntoFuBenMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int fubenId = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				unsigned char suc = JXJGroupIntoFuBenRetVal_failed_unkwon;
				if (mygroupid)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					ref_ptr<crData> groupData;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);;
					if (consortiamap->end() != itr)
					{
						groupData = itr->second->getDataClass();
					}
					if (groupData.valid())
					{
						/*groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenSet,param);
						StartedFubenSet * startedFubenSet  = (StartedFubenSet *)param;*/

						groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
						StartedFubenMap * startedFubenmap  = (StartedFubenMap *)param;

						StartedFubenMap::iterator it = startedFubenmap->find(fubenId);
						if (it != startedFubenmap->end())
						{

							suc = JXJGroupIntoFuBenRetVal_suc;
							// to do 进入副本
						}
						else
						{
							suc = JXJGroupIntoFuBenRetVal_failed_noStarted;
						}
					}
					else
					{
						suc = JXJGroupIntoFuBenRetVal_failed_noGroup;
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				else
				{
					suc = JXJGroupIntoFuBenRetVal_failed_noGroup;
				}
				//if (suc != JXJGroupContributeReturnVal_suc)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(suc);
					if (JXJGroupIntoFuBenRetVal_suc == suc)
					{
						stream->_writeInt(fubenId);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupIntoFuBen,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc = m_stream->_readUChar();
			switch (suc)
			{
				// to do 出错提示
			case JXJGroupIntoFuBenRetVal_failed_noGroup:
				break;
			case JXJGroupIntoFuBenRetVal_failed_noStarted:
				break;
			case JXJGroupIntoFuBenRetVal_failed_unkwon:
				break;
			case JXJGroupIntoFuBenRetVal_suc:
				{
					int fubenid = m_stream->_readInt();
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					if(myPlayer)
					{
						crData *data = myPlayer->getDataClass();
						unsigned char type = FT_Fuben;
						data->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
						data->inputParam(WCHDATA_JXJFubenOrChengchiID,&fubenid);
						myPlayer->doEvent(WCH_JXJMyGroupIntoFuBen);
					}
				}
				break;
			};
		}
	}
}

/************************************************************************/
/*    crJXJUIGroupFunBenStartMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIGroupFunBenStartMethod::crJXJUIGroupFunBenStartMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIGroupFunBenStartMethod::crJXJUIGroupFunBenStartMethod( const crJXJUIGroupFunBenStartMethod & handle ):
	crMethod(handle),
	m_radio1(handle.m_radio1),
	m_canvas(handle.m_canvas),
	m_type(handle.m_type)
{
}

void JXJ::crJXJUIGroupFunBenStartMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIGroupFunBenStartMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_type = (unsigned char )crArgumentParser::appAtoI64(str);
		break;
	case 2:
		m_radio1 = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIGroupFunBenStartMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(m_this && playerGameData && groupCanvas.valid() && groupCanvas->getDataClass())
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);

		if(netConductor && canvas.valid() && canvas->getDataClass())
		{
			do 
			{
				void *param;
				crData *canvasdata = groupCanvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();
				if (myGroupPos != JXJGroup_FirstLeader && myGroupPos != JXJGroup_SecondLeader) break;

				ref_ptr<crRadioGroupWidgetNode> radio1 = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio1));
				if(!radio1.valid()) break;
				int idx = radio1->getSelect();
				ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				crTableIO::DataVec  dataVec;
				crTableIO::StrVec strVec;
				int fubenType = 1;
				if (fubenTab.valid() )
				{
					//int expCostIdx = fubenTab->getTitleIndex("消耗资金");
					//int yuanBaoCostIdx = fubenTab->getTitleIndex("元宝");
					int fubenIdIdx = fubenTab->getTitleIndex("id");
					dataVec.clear();
					fubenTab->queryRecords(2,crArgumentParser::appItoa(fubenType),dataVec);

					if (idx< dataVec.size())
					{
						unsigned short fubenId = atoi(dataVec[idx][fubenIdIdx].c_str());
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(3);
						stream->_writeUChar(m_type);
						stream->_writeUShort(fubenId);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupStartFuBen,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
				}
			} while (0);
	
		}
	}
}

/************************************************************************/
/*    crJXJRecvGroupStartFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvGroupStartFuBenMethod::crJXJRecvGroupStartFuBenMethod():
	m_this(NULL)
{
}

JXJ::crJXJRecvGroupStartFuBenMethod::crJXJRecvGroupStartFuBenMethod( const crJXJRecvGroupStartFuBenMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_canvasname(handle.m_canvasname),
	m_chatinput(handle.m_chatinput),
	m_chatmodesw(handle.m_chatmodesw),
	m_sendbtn(handle.m_sendbtn),
	m_switch(handle.m_switch)
{
}

void JXJ::crJXJRecvGroupStartFuBenMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvGroupStartFuBenMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
	case 1:
		m_canvasname = str;
		break;
	case 2:
		m_chatmodesw = str;
		break;
	case 3:
		m_chatinput = str;
		break;
	case 4:
		m_sendbtn = str;
		break;
	case 5:
		m_switch = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvGroupStartFuBenMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			unsigned char startType = m_stream->_readUChar();
			unsigned short fubenId = m_stream->_readUShort();
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && data)
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				unsigned char suc = JXJGroupStartFuBenRetVal_failed_unkwon;
				int groupExp = 0;
				if (mygroupid)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					CRCore::ScopedLock<crData> lock(*gsBrainData);
					ref_ptr<crData> groupData;
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ConsortiaMap::iterator itr = consortiamap->find(mygroupid);
					if (consortiamap->end() != itr)
					{
						groupData = itr->second->getDataClass();
					}
					if (groupData.valid())
					{
						/*groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenSet,param);
						StartedFubenSet * startedFubenSet  = (StartedFubenSet *)param;*/
						groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator comsitr = membermap->find(playerid);
						if (comsitr != membermap->end())
						{
							if (comsitr->second.valid())
							{
								unsigned char pos = comsitr->second->getGroupPos();
								if (pos != JXJGroup_FirstLeader && pos != JXJGroup_SecondLeader )
								{
									return;
								}
							}
						}

						groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
						StartedFubenMap * startedFubenMap  = (StartedFubenMap *)param;

						StartedFubenMap::iterator sfmitr = startedFubenMap->find(fubenId);
						if (sfmitr == startedFubenMap->end())
						{
							data->getParam(WCHDATA_JXJShiliID,param);
							unsigned char myShili = *(unsigned char *)param;
							ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
							crTableIO::StrVec strVec;
							int fundsCostIdx = fubenTab->getTitleIndex("消耗资金");
							int zhenfazhiIdx = fubenTab->getTitleIndex("征伐值");
							int nameIdx = fubenTab->getTitleIndex("name");
							int sceneidIdx = fubenTab->getTitleIndex("sceneid");
							switch (startType)
							{
							case JXJGroupStartFuBenType_exp:
							case JXJGroupStartFuBenType_yuanbao:
								{
									//unsigned char maxGroupLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaMaxLevel).c_str());
									//if (itr->second->getLevel() >= maxGroupLevel)
									//{
									//	if (fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenId),strVec) >= 0)
									//	{ 
									//		int fundsCost = atoi(strVec[fundsCostIdx].c_str());
									//		int testCost = -fundsCost;
									//		crServerBrainHandle::getInstance()->doEvent(WCH_JXJGroupFundsUpgrade,MAKEINT64(&testCost,mygroupid));
									//		if (testCost == -1)
									//		{
									//			suc = JXJGroupStartFuBenRetVal_failed_noExp;
									//		}
									//		else
									//		{

									//			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									//			crTableIO::StrVec textrecord;
									//			char szNotice[256];
									//			int textid = 4069;
									//			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
									//			{
									//				sprintf_s(szNotice,textrecord[1].c_str(), fundsCost);
									//				CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
									//					szNotice,0,0,mygroupid);
									//				gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
									//				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
									//				gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									//			}
									//		
									//			bool create = false;
									//			
									//			int battleid = MAKEINT32(MAKEINT16(myShili,2),fubenId);
									//			
									//		
									//			crTableIO::StrVec record;
									//			if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenId),record)>=0)
									//			{
									//				int timeid = fubenTab->getTitleIndex("时间");
									//				int countid = fubenTab->getTitleIndex("玩家数量");
									//				int nameid = fubenTab->getTitleIndex("name");
									//				int sceneidIdx = fubenTab->getTitleIndex("sceneid");
									//				int sceneid = atoi(record[sceneidIdx].c_str());
									//				char battleMode = crRoom::Extern;
									//				int timeLimit = atoi(record[timeid].c_str());
									//				short playerCount = (short)(atoi(record[countid].c_str()));
									//				std::string battleName = record[nameid];
									//				bool friendlyFire = false;
									//				ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
									//				streamBuf->createBuf(28+battleName.length());
									//				streamBuf->_writeInt(playerid);
									//				streamBuf->_writeInt(mygroupid);
									//				streamBuf->_writeInt(sceneid);
									//				streamBuf->_writeInt(battleid);//8
									//				streamBuf->_writeChar(battleMode);//1
									//				streamBuf->_writeString(battleName);//16+4
									//				streamBuf->_writeInt(timeLimit);//4
									//				streamBuf->_writeShort(playerCount);//2
									//				streamBuf->_writeBool(friendlyFire);//1
									//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
									//				crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
									//				std::string serverAddress = netCallback->findSceneServerAddress(sceneid);
									//				if(!serverAddress.empty())
									//				{
									//					crJXJJuntuanCreateRoomPacket packet;
									//					crJXJJuntuanCreateRoomPacket::buildRequestPacket(packet,streamBuf.get());
									//					gameServer->getNetManager()->sendPacket(serverAddress,packet);
									//					create = true;
									//				}
									//			}

									//			if (create)
									//			{
									//				suc = JXJGroupStartFuBenRetVal_suc;

									//				bool modify = true;
									//				groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);

									//				//ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									//				//crTableIO::StrVec textrecord;
									//				//char szNotice[256];
									//				textid = 4075;
									//				if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
									//				{
									//					sprintf_s(szNotice,textrecord[1].c_str(),m_this->getCharacterName().c_str(), strVec[nameIdx].c_str());
									//					CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]", szNotice,0,0,mygroupid);
									//					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
									//					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
									//					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
									//				}
									//			}
									//			//ref_ptr<JXJGroupStartFuBenMsg> chatMsg2 = new JXJGroupStartFuBenMsg;
									//			//chatMsg2->m_name = m_this->getCharacterName();
									//			//chatMsg2->m_fubenName = strVec[nameIdx];
									//			//crServerBrainHandle::getInstance()->doEvent(WCH_JXJPutIntoGroupChatMsgQueue,MAKEINT64(chatMsg2.get(),mygroupid));
									//			
									//		}
									//	}
									//	if (suc == JXJGroupStartFuBenRetVal_suc)
									//	{
									//		break;
									//	}
									//}
									if (fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenId),strVec) >= 0)
									{ 
										int zhenfaCost = atoi(strVec[zhenfazhiIdx].c_str());

										groupData->getParam(WCHDATA_JXJConquest,param);
										unsigned char conquest = *(unsigned char *)param;
										if (conquest >= zhenfaCost)
										{
											conquest = conquest - zhenfaCost;
											groupData->inputParam(WCHDATA_JXJConquest,&conquest);
											bool create = false;								
											int battleid = MAKEINT32(MAKEINT16(myShili,2),fubenId);											

											crTableIO::StrVec record;
											if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenId),record)>=0)
											{
												int timeid = fubenTab->getTitleIndex("时间");
												int countid = fubenTab->getTitleIndex("玩家数量");
												int nameid = fubenTab->getTitleIndex("name");
												int sceneidIdx = fubenTab->getTitleIndex("sceneid");
												int sceneid = atoi(record[sceneidIdx].c_str());
												char battleMode = crRoom::Extern;
												int timeLimit = atoi(record[timeid].c_str());
												short playerCount = (short)(atoi(record[countid].c_str()));
												std::string battleName = record[nameid];
												bool friendlyFire = false;
												ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
												streamBuf->createBuf(28+battleName.length());
												streamBuf->_writeInt(playerid);
												streamBuf->_writeInt(mygroupid);
												streamBuf->_writeInt(sceneid);
												streamBuf->_writeInt(battleid);//8
												streamBuf->_writeChar(battleMode);//1
												streamBuf->_writeString(battleName);//16+4
												streamBuf->_writeInt(timeLimit);//4
												streamBuf->_writeShort(playerCount);//2
												streamBuf->_writeBool(friendlyFire);//1
												crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
												crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
												std::string serverAddress = netCallback->findSceneServerAddress(sceneid);
												if(!serverAddress.empty())
												{
													crJXJJuntuanCreateRoomPacket packet;
													crJXJJuntuanCreateRoomPacket::buildRequestPacket(packet,streamBuf.get());
													gameServer->getNetManager()->sendPacket(serverAddress,packet);
													create = true;
												}
											}
											if (create)
											{
												ConsortiaPlayerReliveCount countmap;
												int roomid = 0;
												startedFubenMap->insert(std::make_pair(fubenId,std::make_pair(roomid,countmap)));
												suc = JXJGroupStartFuBenRetVal_suc;
												bool modify = true;
												groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
											}
												
										}
										else
										{
											suc = JXJGroupStartFuBenRetVal_failed_noTaofaDian;
										}
									}
								}
								break;
							default:
								break;
							}
						}
						else if(sfmitr->second.first == 0)
						{
							suc = JXJGroupStartFuBenRetVal_failed_isstarting;
						}
						else
						{
							suc = JXJGroupStartFuBenRetVal_failed_alreadyStarted;
						}
							
					//	groupData->getParam(WCHDATA_JXJConsortiaFunds,param);
					//	groupExp = *(int *)param;
					}
					else
					{
						suc = JXJGroupStartFuBenRetVal_failed_noGroup;
					}
				}
				else
				{
					suc = JXJGroupStartFuBenRetVal_failed_noGroup;
				}
				if (suc != JXJGroupStartFuBenRetVal_suc)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(suc);
					/*	if (JXJGroupStartFuBenRetVal_suc == suc)
					{
					stream->_writeInt(fubenId);
					stream->_writeInt(groupExp);
					}*/

					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupStartFuBen,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc = m_stream->_readUChar();
			switch (suc)
			{
				// to do 出错提示
			case JXJGroupStartFuBenRetVal_failed_noGroup:
				break;
			case JXJGroupStartFuBenRetVal_failed_alreadyStarted:
				{
					int msgid = 5014;
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
				break;
			case JXJGroupStartFuBenRetVal_failed_unkwon:
				break;
			case JXJGroupStartFuBenRetVal_failed_noTaofaDian:
				{
					int msgid = 5013;
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
				break;
			case JXJGroupStartFuBenRetVal_failed_isstarting:
				{
					int msgid = 5014;
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
				break;
			case JXJGroupStartFuBenRetVal_suc:
				{
					int fubenid = m_stream->_readInt();
					//int groupExp = m_stream->_readInt();
					//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					//if (canvas.valid())
					//{
					//	crData * canvasData = canvas->getDataClass();
					//	if (canvasData)
					//	{
					//		canvasData->excHandle(MAKEINT64(WCH_LockData,1));
					//		void *param;

					//		/*	canvasData->getParam(WCHDATA_JXJMyGroupStartedFuBenSet,param);
					//		StartedFubenSet * fubnSet = (StartedFubenSet *)param;
					//		fubnSet->insert(fubenid);*/

					//		canvasData->getParam(WCHDATA_JXJMyGroupInfo,param);
					//		crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;
					//		myGroup->setFunds(groupExp);

					//		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
					//	}
					//	canvas->doEvent(WCH_UI_UpdateData);
					//}

					ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
					crTableIO::StrVec record;
					if (fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record) >= 0)
					{
						int nameid = fubenTab->getTitleIndex("name");
						ref_ptr<crCanvasNode>canvas1 = crFilterRenderManager::getInstance()->findCanvas(m_canvasname);
						if(!canvas1)return;
						ref_ptr<crRadioGroupWidgetNode> chatmodesw = dynamic_cast<crRadioGroupWidgetNode *>(canvas1->getWidget(m_chatmodesw));
						ref_ptr<crStaticTextWidgetNode> chatinput = dynamic_cast<crStaticTextWidgetNode *>(canvas1->getWidget(m_chatinput));
						ref_ptr<crButtonWidgetNode> sendbtn = dynamic_cast<crButtonWidgetNode *>(canvas1->getWidget(m_sendbtn));
						crMultiSwitch *mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas1->getChildNode(m_switch));
						if(!chatmodesw || !chatinput ||!sendbtn || !mulitSwitch)return;
						chatmodesw->select(ChatInGroup);
						chatmodesw->doEvent(WCH_UI_SelectChange);
						mulitSwitch->setActiveSwitchSet(ChatInGroup-1);
					
						int myplayerID = crMyPlayerData::getInstance()->getPlayerID();
						std::string strMessage;
						std::string name = record[nameid];
						strMessage += std::string("^4^")+std::string("军团副本[")+name+std::string("]开启，请快点加入吧！")+std::string("{(")+ \
							crArgumentParser::appItoa(3)+std::string(")(")+crArgumentParser::appItoa(myplayerID)+std::string(")(")+crArgumentParser::appItoa(fubenid)+std::string(")}");//{(type)(playerid)(fubenid)}
						if(!strMessage.empty())
						{
							chatinput->setString(strMessage);
							sendbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
						}
					}
				}
				break;
			}
		}
	}
}

/************************************************************************/
/*    crJXJCheckZhuZhaChengChiMethod                                                                  */
/************************************************************************/
JXJ::crJXJCheckCantonChengChiMethod::crJXJCheckCantonChengChiMethod()
{
}

JXJ::crJXJCheckCantonChengChiMethod::crJXJCheckCantonChengChiMethod( const crJXJCheckCantonChengChiMethod & handle ):
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJCheckCantonChengChiMethod::inputParam( int i, void *param )
{
}

void JXJ::crJXJCheckCantonChengChiMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJCheckCantonChengChiMethod::operator()( crHandle &handle )
{
	void * param;
	int _case = 0;

	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		crData *canvasdata = canvas->getDataClass();
		if (canvasdata)
		{
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,1));

			canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
			MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
			canvasdata ->getParam(WCHDATA_JXJMyApplyCantonVec,param);
			MyApplyCantonVec * chengChiVec = (MyApplyCantonVec *)param;
			_case = !chengChiVec->empty() || !myAllocatedCantonVec->empty();

			//_case = chengChiVec->size() ? 1: 0;
			//_case = 1; // to do test
			handle.outputParam(0,&_case);
			canvasdata ->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJMyGroupChengChiCanvasSwitchChangeMethod                                                                  */
/************************************************************************/

JXJ::crJXJMyGroupChengChiCanvasSwitchChangeMethod::crJXJMyGroupChengChiCanvasSwitchChangeMethod()
{
}

JXJ::crJXJMyGroupChengChiCanvasSwitchChangeMethod::crJXJMyGroupChengChiCanvasSwitchChangeMethod( const crJXJMyGroupChengChiCanvasSwitchChangeMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_switch(handle.m_switch),
	m_value (handle.m_value)
{
}

void JXJ::crJXJMyGroupChengChiCanvasSwitchChangeMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		crData * canvasData = canvas->getDataClass();

		crMultiSwitch *caseSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
		if(caseSwitch)
		{
			caseSwitch->setActiveSwitchSet(m_value);
			int preIdx = -1;
			canvasData->inputParam(WCHDATA_JXJPreSelectedIdx,&preIdx);
		}
	}
}

void JXJ::crJXJMyGroupChengChiCanvasSwitchChangeMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJMyGroupChengChiCanvasSwitchChangeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_switch = str;
		break;
	case 2:
		m_value = atoi(str.c_str());
		break;
	}
}

/************************************************************************/
/*    crJXJUIResetPageMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIResetPageMethod::crJXJUIResetPageMethod()
{

}

JXJ::crJXJUIResetPageMethod::crJXJUIResetPageMethod( const crJXJUIResetPageMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_curPage(handle.m_curPage),
	m_totalPage(handle.m_totalPage)
{

}

void JXJ::crJXJUIResetPageMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJUIResetPageMethod::addParam( int i, const std::string& str )
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_curPage = atoi(str.c_str());
		break;
	case 2:
		m_totalPage = atoi(str.c_str());
		break;
	};
}

void JXJ::crJXJUIResetPageMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		crData * canvasData = canvas->getDataClass();
		if (canvasData)
		{
			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
			int val = 1;
			canvasData->inputParam(m_curPage,&val);
			canvasData->inputParam(m_totalPage,&val);
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJQueryMyChengChiVecMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryMyCantonGroupMapMethod::crJXJQueryMyCantonGroupMapMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryMyCantonGroupMapMethod::crJXJQueryMyCantonGroupMapMethod( const crJXJQueryMyCantonGroupMapMethod & handle ):
	crMethod(handle),
	m_groupRadio(handle.m_groupRadio),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJQueryMyCantonGroupMapMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
				crData * canvasData = canvas->getDataClass();
				if (canvasData && groupradio.valid())
				{
					int idx = groupradio->getSelect();
					void *param;
					canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJMyGroupChengChiCurPage,param);
					int chengchiCurPage = *(int *)param;
					canvasData->getParam(WCHDATA_JXJMyApplyCantonVec,param);
					MyApplyCantonVec * myapplyCantonVec = (MyApplyCantonVec * )param;
					canvasData->getParam(WCHDATA_JXJAllocateCantonVec,param);
					MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
					std::vector<int> * vecPtr = NULL;

					if (myapplyCantonVec->empty())
					{
						vecPtr = myAllocatedCantonVec;
					}
					else if (myAllocatedCantonVec->empty())
					{
						vecPtr = myapplyCantonVec;
					}

					if (vecPtr && vecPtr->empty() == false)
					{
						if (idx > vecPtr->size())
						{
							idx = 0;
						}
						crPlayerDataEventPacket packet;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(8);
						stream->_writeInt(chengchiCurPage);
						stream->_writeInt((*vecPtr)[idx]); 

						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCantonGroupMap,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
					canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

void JXJ::crJXJQueryMyCantonGroupMapMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryMyCantonGroupMapMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
		break;
	default:
		break;
	}
}

/************************************************************************/
/*    crJXJRecvQueryMyChengChiVecMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryMyCantonGroupMapMethod::crJXJRecvQueryMyCantonGroupMapMethod():
	m_this(NULL)
{
}

JXJ::crJXJRecvQueryMyCantonGroupMapMethod::crJXJRecvQueryMyCantonGroupMapMethod( const crJXJRecvQueryMyCantonGroupMapMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}

void JXJ::crJXJRecvQueryMyCantonGroupMapMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyCantonGroupMapMethod::addParam( int i, const std::string& str )

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

void JXJ::crJXJRecvQueryMyCantonGroupMapMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();
			int curPage = m_stream->_readInt();
			int cantonId = m_stream->_readInt();

			crData *data = m_this->getDataClass();

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str()) - 1;
			if (playerData.valid())
			{
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				if (shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

						gsBrainData->getParam(WCHDATA_JXJConsortiaLvRank,param);
						ConsortialvRank *groupLvRank = (ConsortialvRank *)param;

						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;

						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if (shili && shili->getDataClass())
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
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(256);
									stream->_writeUChar(0);
									bool hasCantonGroup = false;
									int groupid = it->second->getConsortiaId();
									ConsortiaMap::iterator iter = consortiamap->find(groupid);;
									if (iter != consortiamap->end() && iter->second->getDataClass())
									{
										crData * groupData = iter->second->getDataClass();
										if (groupData)
										{
											hasCantonGroup = true;
											stream->_writeBool(true);
											stream->_writeInt(groupid);
											std::string groupName ; 
											iter->second->getName(groupName);
											std::string leaderName ;
											iter->second->getLeaderName(0,leaderName);
											stream->_writeString(groupName);
											stream->_writeString(leaderName);
											stream->_writeInt(iter->second->getLevel());
											stream->_writeInt(iter->second->getMemberCount());
											stream->_writeInt(iter->second->getTotalActiveVal());
											stream->_writeShort(it->second->reCalculateEarnings());
										}
									}
									if (!hasCantonGroup)
									{
										stream->_writeBool(false);
									}
									ref_ptr<crJXJConsortia> consortia;
									ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
									stream->_writeUChar(chengChiEarningsMap.size());
									ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.begin();
									int firstline = (curPage - 1) * perpagenum;
									int count = 0;
									int endline  = firstline + perpagenum; 
									for (int i = 0; ccemItr != chengChiEarningsMap.end() && count < perpagenum;++ ccemItr)
									{
										if (ccemItr->second.valid())
										{
											if (i >= firstline)
											{
												int groupid = ccemItr->second->getGroupId();
												consortia = NULL;
												ConsortiaMap::iterator iter = consortiamap->find(groupid);;
												if (iter != consortiamap->end())
												{
													consortia = iter->second;
												}
												if(consortia.valid())
												{
													crData * groupData = consortia->getDataClass();
													if (groupData)
													{
														int chengchiId = ccemItr->second->getChengChiId();
														short chengchiEarnings = ccemItr->second->getEarnings();

														stream->_writeInt(groupid);
														std::string groupName ; 
														consortia->getName(groupName);
														std::string leaderName ;
														consortia->getLeaderName(0,leaderName);
														stream->_writeInt(groupLvRank->getRankNum(groupid));
														stream->_writeString(groupName);
														stream->_writeString(leaderName);
														stream->_writeInt(consortia->getLevel());
														stream->_writeInt(consortia->getMemberCount());
														stream->_writeInt(consortia->getTotalActiveVal());
														stream->_writeInt(chengchiId);
														stream->_writeShort(chengchiEarnings);
														++ count;
													}
												}
											}
											++ i;
										}
									}
									int bufSize = stream->getBufSize();
									stream->seekBegin();
									stream->_writeUChar(count);
									stream->setBufSize(bufSize);
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyCantonGroupMap,stream.get());
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

					int count = m_stream->_readUChar();

					bool hasCantonGroup = m_stream->_readBool();

					canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec* cantonGroupVec = (CantonGroupVec *)param;
					cantonGroupVec->clear();

					CRCore::ref_ptr<MyGroupChengChi> chengchi = new MyGroupChengChi;
					if (hasCantonGroup)
					{
						chengchi->m_groupId = m_stream->_readInt();						
						chengchi->m_groupName = m_stream->_readString();
						chengchi->m_leaderName = m_stream->_readString();
						chengchi->m_level = m_stream->_readInt();
						chengchi->m_memberCount = m_stream->_readInt();
						chengchi->m_totoalActiveVal = m_stream->_readInt();
						chengchi->m_earnings = m_stream->_readShort();
						chengchi->m_UIearnings = chengchi->m_earnings;
					}
					cantonGroupVec->push_back( chengchi );

					int totalCount = m_stream->_readUChar() + 1;
					canvasdata->inputParam(WCHDATA_JXJMyGroupChengChiTotalPage,&totalCount);
					for (int i = 0; i < count; ++ i)
					{
						CRCore::ref_ptr<MyGroupChengChi> chengchi = new MyGroupChengChi;
						chengchi->m_groupId = m_stream->_readInt();
						chengchi->m_rank = m_stream->_readInt();
						chengchi->m_groupName = m_stream->_readString();
						chengchi->m_leaderName = m_stream->_readString();
						chengchi->m_level = m_stream->_readInt();
						chengchi->m_memberCount = m_stream->_readInt();
						chengchi->m_totoalActiveVal = m_stream->_readInt();
						chengchi->m_chengChiId = m_stream->_readInt();
						chengchi->m_earnings = m_stream->_readShort();
						chengchi->m_UIearnings = chengchi->m_earnings;
						cantonGroupVec->push_back( chengchi );
					}
					
					mygroupcanvas->doEvent(WCH_UI_UpdateData);
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJUIChangeMyChengChiMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIChangeMyChengChiMethod::crJXJUIChangeMyChengChiMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIChangeMyChengChiMethod::crJXJUIChangeMyChengChiMethod( const crJXJUIChangeMyChengChiMethod & handle ):
	crMethod(handle),
	m_groupRadio(handle.m_groupRadio),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJUIChangeMyChengChiMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIChangeMyChengChiMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
	default:
		break;
	}
}

void JXJ::crJXJUIChangeMyChengChiMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
				crData * canvasData = canvas->getDataClass();
				if (canvasData && groupradio.valid())
				{
					int idx = groupradio->getSelect();
					void *param;
					canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId	,param);
					int cantonId = *(int *)param;
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(cantonId);  

					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeMyChengChi,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvChangeMyChengChiMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvChangeMyChengChiMethod::crJXJRecvChangeMyChengChiMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvChangeMyChengChiMethod::crJXJRecvChangeMyChengChiMethod( const crJXJRecvChangeMyChengChiMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{
}

void JXJ::crJXJRecvChangeMyChengChiMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvChangeMyChengChiMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvChangeMyChengChiMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();

			int cantonId = m_stream->_readInt();

			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

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
						if (shili && shili->getDataClass())
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
									unsigned char suc = JXJGroupChangeChengChiRetVal_failed_unkwon;
									if (it->second->getConsortiaId() != mygroupid)
									{
										ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
										ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(mygroupid);
										if (ccemItr != chengChiEarningsMap.end())
										{
											chengChiEarningsMap.erase(ccemItr);
											ref_ptr<crData>groupData;
											ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
											if (cmItr != consortiamap->end())
											{
												groupData = cmItr->second->getDataClass();
											}
											if (groupData.valid())
											{
												ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
												crTableIO::StrVec record;
												int cantonIdx = 0;
												if (chengchiTab.valid()) cantonIdx = chengchiTab->getTitleIndex("州ID");

												groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
												FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
												FenFengedChengChiMap::iterator ffccmItr = fenFengedChengChiMap->begin();
												for (;ffccmItr != fenFengedChengChiMap->end();)
												{
													if (chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(ffccmItr->first),record) >= 0)
													{
														if (cantonId == atoi(record[cantonIdx].c_str()))
														{
															ffccmItr = fenFengedChengChiMap->erase(ffccmItr);
															continue;
														}
													}
													++ ffccmItr ;
												}
												groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
												ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
												ApplyCantonMap::iterator acmItr = applyCantonMap->find(cantonId);
												if (acmItr != applyCantonMap->end())
												{
													applyCantonMap->erase(acmItr);
												}
												bool modify = true;
												groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
												shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
											}
											suc = JXJGroupChangeChengChiRetVal_suc;
										}
										ref_ptr<crStreamBuf> stream = new crStreamBuf;
										stream->createBuf(1);
										stream->_writeUChar(suc);
										stream->_writeInt(cantonId);
										crPlayerDataEventPacket packet;
										crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeMyChengChi,stream.get());
										gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
									}
									else
									{
										suc = JXJGroupChangeChengChiRetVal_failed_isGuard;
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
					MyApplyCantonVec * myGroupChengChiVec  = (MyApplyCantonVec *)param;
					unsigned char suc = m_stream->_readUChar();
					int cantonId = m_stream->_readInt();

					switch (suc)
					{
					case JXJGroupChangeChengChiRetVal_failed_unkwon:
						break;
					case JXJGroupChangeChengChiRetVal_suc:
						{
							MyApplyCantonVec::iterator mgccvItr = myGroupChengChiVec->begin();
							for (; mgccvItr != myGroupChengChiVec->end(); ++ mgccvItr)
							{
								if (*mgccvItr == cantonId)
								{
									myGroupChengChiVec->erase(mgccvItr);
									break;
								}
							}
							mygroupcanvas->doEvent(WCH_UI_UpdateData);
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


/************************************************************************/
/*    crJXJUIRemoveChengChiGroupMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIRemoveChengChiGroupMethod::crJXJUIRemoveChengChiGroupMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIRemoveChengChiGroupMethod::crJXJUIRemoveChengChiGroupMethod( const crJXJUIRemoveChengChiGroupMethod& handle ):
	crMethod(handle),
	m_groupList(handle.m_groupList),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJUIRemoveChengChiGroupMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIRemoveChengChiGroupMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupList = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIRemoveChengChiGroupMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crTableWidgetNode>  groupList = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));
			
			if(canvas.valid() && groupList.valid())
			{
				//ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
				crData * canvasData = canvas->getDataClass();
				if (canvasData /*&& groupradio*/)
				{
					void *param;
					canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId,param);
					int cantonId = *(int *)param;
					int groupId = groupList->getSelectData();
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(8);
					stream->_writeInt(groupId); 
					stream->_writeInt(cantonId);

					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvRemoveChengChiGroup,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvRemoveChengChiGroupMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvRemoveChengChiGroupMethod::crJXJRecvRemoveChengChiGroupMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvRemoveChengChiGroupMethod::crJXJRecvRemoveChengChiGroupMethod( const crJXJRecvRemoveChengChiGroupMethod& handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_netType(handle.m_netType)
{
}

void JXJ::crJXJRecvRemoveChengChiGroupMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvRemoveChengChiGroupMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvRemoveChengChiGroupMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();
			int groupId = m_stream->_readInt();
			int cantonId  = m_stream->_readInt();

			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

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
									unsigned char suc = JXJGroupRemoveChengChiGroupRetVal_failed_unkwon;
									if (it->second->getConsortiaId() == mygroupid)
									{
										if (it->second->getCantonId() != groupId)
										{
											ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
											ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(groupId);
											if (ccemItr != chengChiEarningsMap.end())
											{
												int chengchiId = 0;
												if (ccemItr->second.valid())
												{
													chengchiId = ccemItr->second->getChengChiId();
												}
												chengChiEarningsMap.erase(ccemItr);
												ref_ptr<crData> groupData;
												ConsortiaMap::iterator cmItr = consortiamap->find(groupId);
												if (cmItr != consortiamap->end())
												{
													groupData = cmItr->second->getDataClass();
												}
												if (groupData.valid())
												{
													groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
													FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
													FenFengedChengChiMap::iterator ffccmItr = fenFengedChengChiMap->find(chengchiId);
													if (ffccmItr != fenFengedChengChiMap->end())
													{
														fenFengedChengChiMap->erase(ffccmItr);
													}
													groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
													ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
													ApplyCantonMap::iterator acmItr = applyCantonMap->find(cantonId);
													if (acmItr != applyCantonMap->end())
													{
														applyCantonMap->erase(acmItr);
													}
													bool modify = true;
													groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
												}
												suc = JXJGroupRemoveChengChiGroupRetVal_suc;
												bool modify = true;
												shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
											}
											ref_ptr<crStreamBuf> stream = new crStreamBuf;
											stream->createBuf(5);
											stream->_writeUChar(suc);
											stream->_writeInt(groupId);
											crPlayerDataEventPacket packet;
											crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangeMyChengChi,stream.get());
											gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
										}
										else
										{
											suc = JXJGroupRemoveChengChiGroupRetVal_failed_isGuard;
										}
									}
									else
									{
										suc = JXJGroupRemoveChengChiGroupRetVal_failed_notGuard;
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

					canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec* cantonGroupVec = (CantonGroupVec *)param;
					
					unsigned char suc = m_stream->_readUChar();
					int groupid = m_stream->_readInt();

					switch (suc)
					{
					case JXJGroupRemoveChengChiGroupRetVal_failed_unkwon:
						break;
					case JXJGroupRemoveChengChiGroupRetVal_failed_isGuard:
						{
							m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4064,NULL));
						}
						break;
					case JXJGroupRemoveChengChiGroupRetVal_suc:
						{
							CantonGroupVec::iterator cgvItr = cantonGroupVec->begin();
							for (; cgvItr != cantonGroupVec->end(); ++ cgvItr )
							{
								if (cgvItr->valid() && (*cgvItr)->m_groupId == groupid)
								{
									cantonGroupVec->erase(cgvItr);
									break;
								}
							}
							mygroupcanvas->doEvent(WCH_UI_UpdateData);
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

/************************************************************************/
/*    crJXJUIFenFengChengChiMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIFenFengChengChiMethod::crJXJUIFenFengChengChiMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIFenFengChengChiMethod::crJXJUIFenFengChengChiMethod( const crJXJUIFenFengChengChiMethod& handle ):
	crMethod(handle),
	m_type(handle.m_type),
	m_groupRadio(handle.m_groupRadio),
	m_groupList2(handle.m_groupList2),
	m_groupList1(handle.m_groupList1),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJUIFenFengChengChiMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIFenFengChengChiMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupList1 = str;
		break;
	case  2:
		m_groupList2 = str;
		break;
	case 3:
		m_groupRadio = str;
		break;
	case 4:
		m_type = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIFenFengChengChiMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crTableWidgetNode> groupList1 = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList1));
			ref_ptr<crTableWidgetNode> groupList2 = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList2));
			
			if(canvas.valid() && groupList1.valid() && groupList2.valid())
			{
				ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
				crData * canvasData = canvas->getDataClass();
				if (canvasData && groupradio.valid())
				{
					int idx = groupradio->getSelect();
					canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
					int groupId = groupList1->getSelectData();
					int chengchiId = groupList2->getSelectData();
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(9);
					stream->_writeInt(groupId); 
					stream->_writeInt(chengchiId); 
					stream->_writeUChar(m_type);
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChengChiFenFeng,NULL);
					netConductor->getNetManager()->sendPacket("all",packet);
					canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

JXJ::crJXJRecvChengChiFenFengMethod::crJXJRecvChengChiFenFengMethod():
	m_this(NULL),
	m_stream(NULL)
{
}

JXJ::crJXJRecvChengChiFenFengMethod::crJXJRecvChengChiFenFengMethod( const crJXJRecvChengChiFenFengMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_netType(handle.m_netType)
{
}

void JXJ::crJXJRecvChengChiFenFengMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvChengChiFenFengMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvChengChiFenFengMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char success = 0;
			int playerid = m_this->getPlayerID();

			int groupId = m_stream->_readInt();
			int chengchiId = m_stream->_readInt();
			unsigned char op = m_stream->_readUChar();

			int cantonId  = 0;

			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				if (mygroupid > 0 && shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						ref_ptr<crData>myGroupData;
						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
						if (cmItr != consortiamap->end())
						{
							myGroupData = cmItr->second->getDataClass();
						}
						if (myGroupData.valid())
						{
							myGroupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
							FenFengedCantonMap * fenFengedChengChiMap = (FenFengedCantonMap *)param;
							FenFengedCantonMap::iterator ffcmitr = fenFengedChengChiMap->begin();
							if (ffcmitr != fenFengedChengChiMap->end())
							{
								cantonId = ffcmitr->first;
							}
						}

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
									unsigned char suc = JXJGroupFenFengChengChiRetVal_failed_unkwon;
									ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
									ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(groupId);
									if (ccemItr != chengChiEarningsMap.end())
									{
										if (ccemItr->second.valid())
										{
											if (0 == op)
											{
												ccemItr->second->setChengChiId(0);
											}
											else if (1 == op)
											{
												ccemItr->second->setChengChiId(chengchiId);
											}
											//chengchiId = ccemItr->second->getChengChiId();
											suc = JXJGroupFenFengChengChiRetVal_suc;
										}
									//	ConsortiaMap::iterator cmItr = consortiamap->find(groupId);
									//	if (cmItr != consortiamap->end())
									//	{
									//		crData * groupData = cmItr->second->getDataClass();
									//		if (groupData)
									//		{
									//			groupData->getParam(WCH_JXJGroupApplyChengChiSet,param);
									//			GroupApplyChengChiSet * applyChengChiSet = (GroupApplyChengChiSet *)param;
									//			GroupApplyChengChiSet::iterator gaccsItr = applyChengChiSet->find(chengchiId);
									//			if (gaccsItr != applyChengChiSet->end())
									//			{
									//				applyChengChiSet->erase(gaccsItr);
									//			}
									//		}
									//	}
									//	it->second->reCalculateEarnings();
									}
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(9);
									stream->_writeUChar(suc);
									stream->_writeInt(groupId);
									stream->_writeInt(chengchiId);

									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChengChiFenFeng,stream.get());
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
					MyApplyCantonVec * myGroupChengChiVec  = (MyApplyCantonVec *)param;

					canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec * cantonGroupVec = (CantonGroupVec *)param;
					unsigned char suc = m_stream->_readUChar();
					int groupid = m_stream->_readInt();
					int chengchiId = m_stream->_readInt();

					//int cantonId = 0;
					//ref_ptr<crTableIO>  chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
					//crTableIO::StrVec record;
					//if (chengchiTab && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiId),record) >= 0)
					//{
					//	int cantonIdx = chengchiTab->getTitleIndex("州ID");
					//	cantonId = atoi(record[cantonIdx].c_str());
					//}

					switch (suc)
					{
					case JXJGroupFenFengChengChiRetVal_failed_unkwon:
						break;
					case JXJGroupFenFengChengChiRetVal_suc:
						{
							MyApplyCantonVec::iterator mgccvItr = myGroupChengChiVec->begin();
							for (; mgccvItr != myGroupChengChiVec->end(); ++ mgccvItr)
							{
								if (*mgccvItr == chengchiId)
								{
									CantonGroupVec::iterator cgvItr = cantonGroupVec->begin();
									for (; cgvItr != cantonGroupVec->end(); ++ cgvItr)
									{
										if (cgvItr->valid()  && (*cgvItr)->m_groupId == groupid)
										{
											(*cgvItr)->m_chengChiId = chengchiId; 
										}
									}
									break;
								}
							}
							mygroupcanvas->doEvent(WCH_UI_UpdateData);
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

/************************************************************************/
/*    crJXJUIAllocateEarningsMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIAllocateEarningsMethod::crJXJUIAllocateEarningsMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIAllocateEarningsMethod::crJXJUIAllocateEarningsMethod( const crJXJUIAllocateEarningsMethod& handle ):
	crMethod(handle),
	m_groupList(handle.m_groupList),
	m_canvas(handle.m_canvas),
	m_lock(handle.m_lock)
{
	for (unsigned char i = 0; i < 10; ++ i)
	{
		m_edit[i] = handle.m_edit[i];
	}
}

void JXJ::crJXJUIAllocateEarningsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIAllocateEarningsMethod::addParam( int i, const std::string& str )
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
		m_edit[i - 2] = str;
		break;
	case 3:
		m_edit[i - 2] = str;
		break;
	case 4:
		m_edit[i - 2] = str;
		break;
	case 5:
		m_edit[i - 2] = str;
		break;
	case 6:
		m_edit[i - 2] = str;
		break;
	case 7:
		m_edit[i - 2] = str;
		break;
	case 8:
		m_edit[i - 2] = str;
		break;
	case 9:
		m_edit[i - 2] = str;
		break;
	case 10:
		m_edit[i - 2] = str;
		break;
	case 11:
		m_edit[i - 2] = str;
		break;
	case 12:
		m_lock = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIAllocateEarningsMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid())
			{
				ref_ptr<crTableWidgetNode> groupList1 = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));

				crData * canvasData = canvas->getDataClass();
				if (canvasData )
				{
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(9);
					stream->_writeUChar(0);
					unsigned char count = 0;
					canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
					void *param;

					canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId,param);
					int cantonId = *(int *)param;
					stream->_writeInt(cantonId);
					canvasData->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec  cantonGroupVec = *(CantonGroupVec *)param;
					unsigned char size = cantonGroupVec.size();
					CantonGroupVec::iterator cgvItr = cantonGroupVec.begin(); 
					for (unsigned char i = 1; i < size; ++ i)
					{
						if (cantonGroupVec[i].valid() 
							&& cantonGroupVec[i]->m_earnings != cantonGroupVec[i]->m_UIearnings)
						{
							stream->_writeInt(cantonGroupVec[i]->m_groupId); 
							stream->_writeShort(cantonGroupVec[i]->m_UIearnings);
							++ count;
						}
					}
					if (count)
					{
						int bufSize = stream->getBufSize();
						stream->seekBegin();
						stream->_writeUChar(count);
						stream->setBufSize(bufSize);

						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAllocateEarnings,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
					canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}


/************************************************************************/
/*    crJXJRecvAllocateEarningsMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvAllocateEarningsMethod::crJXJRecvAllocateEarningsMethod():
	m_this(NULL)
{
}

JXJ::crJXJRecvAllocateEarningsMethod::crJXJRecvAllocateEarningsMethod( const crJXJRecvAllocateEarningsMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_netType(handle.m_netType)
{
}

void JXJ::crJXJRecvAllocateEarningsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvAllocateEarningsMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvAllocateEarningsMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		typedef std::vector<std::pair<int,short> > ChangeEarningVec;
		ChangeEarningVec changeEarningVec;
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();

			unsigned char size = m_stream->_readUChar();
			int cantonId  = m_stream->_readInt();
			int groupID ;
			short earnings;
			for (unsigned char i = 0; i < size; ++ i)
			{
				groupID = m_stream->_readInt();
				earnings = m_stream->_readShort();
				changeEarningVec.push_back(std::make_pair(groupID,earnings));
			}

			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

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
							unsigned char suc = JXJGroupAllocateEarningsRetVal_failed_unkwon;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(17);
							stream->_writeUChar(suc);

							crData *shilidata = shili->getDataClass();
							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
							shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
							CantonMap * cantonMap = (CantonMap *)param;
							CantonMap::iterator it = cantonMap->find(cantonId);
							if (it != cantonMap->end())
							{
								if (it->second.valid() && it->second->getConsortiaId() == mygroupid)
								{
									ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
									ChengChiEarningsMap::iterator ccemItr ;
									ChangeEarningVec::iterator cevItr = changeEarningVec.begin();
									short restEarnings = it->second->reCalculateEarnings();
									short changeVal;
									short testVal;

									stream->_writeUChar(0);
									unsigned char size = 0;
									for (; cevItr != changeEarningVec.end(); ++ cevItr)
									{
										ccemItr = chengChiEarningsMap.find(cevItr->first);
										if (ccemItr != chengChiEarningsMap.end())
										{
											changeVal = cevItr->second - ccemItr->second->getEarnings();
											testVal = restEarnings - changeVal;
											if (testVal < 0
												|| testVal > 1000)
											{
												continue;
											}
											ccemItr->second->setEarnings(cevItr->second);
											restEarnings -= changeVal;
											stream->_writeInt(cevItr->first);
											stream->_writeShort(cevItr->second);
											++ size;

											ref_ptr<crData> groupData;
											ConsortiaMap::iterator cmItr = consortiamap->find(cevItr->first);
											if (cmItr != consortiamap->end())
											{
												groupData = cmItr->second->getDataClass();
											}
											if(groupData.valid())
											{
												groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
												ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
												ApplyCantonMap::iterator ffcmItr = applyCantonMap->find(cantonId);
												if (ffcmItr != applyCantonMap->end())
												{
													ffcmItr->second = cevItr->second;
												}
												bool modify = true;
												groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
											}
										}
									}
									ref_ptr<crData> groupData;
									ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
									if (cmItr != consortiamap->end())
									{
										groupData = cmItr->second->getDataClass();
									}
									if(groupData.valid())
									{
										groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
										FenFengedCantonMap * fenFengedCantonMap = (FenFengedCantonMap *) param;
										FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->find(cantonId);
										if (ffcmItr != fenFengedCantonMap->end())
										{
											ffcmItr->second = it->second->reCalculateEarnings();
										}
										bool modify = true;
										groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
									}

									bool modify = true;
									shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);

									suc = JXJGroupAllocateEarningsRetVal_suc;
									stream->_writeInt(mygroupid);
									stream->_writeShort(it->second->reCalculateEarnings());

									int bufSize = stream->getBufSize();
									stream->seekBegin();
									stream->_writeUChar(suc);
									stream->_writeUChar(size);
									stream->setBufSize(bufSize);
								}
							}
							shilidata->excHandle(MAKEINT64(WCH_LockData,0));

							if (suc != JXJGroupAllocateEarningsRetVal_suc)
							{
								int bufSize = stream->getBufSize();
								stream->seekBegin();
								stream->_writeUChar(suc);
								stream->setBufSize(bufSize);
							}

							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAllocateEarnings,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					
					canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
					CantonGroupVec * cantonGroupVec = (CantonGroupVec *)param;

					unsigned char suc = m_stream->_readUChar();
					switch (suc)
					{
					case JXJGroupFenFengChengChiRetVal_failed_unkwon:
						break;
					case JXJGroupFenFengChengChiRetVal_suc:
						{
							changeEarningVec.clear();
							unsigned char size = m_stream->_readUChar();
							int groupId;
							short earnings;
							CantonGroupVec::iterator cgvItr ;
							for (unsigned char i = 0; i < size; ++ i)
							{
								groupId = m_stream->_readInt();
								earnings = m_stream->_readShort();
								cgvItr = cantonGroupVec->begin();
								for (; cgvItr != cantonGroupVec->end(); ++ cgvItr)
								{
									if (cgvItr->valid()  && (*cgvItr)->m_groupId == groupId)
									{
										(*cgvItr)->m_earnings = earnings ; 
										break;
									}
								}
							}
							groupId = m_stream->_readInt();
							earnings = m_stream->_readShort();
							if (cantonGroupVec->empty() == false)
							{
								if ((*cantonGroupVec)[0]->m_groupId == groupId)
								{
									(*cantonGroupVec)[0]->m_earnings = earnings;
								}
							}

							cgvItr = cantonGroupVec->begin();
							for (; cgvItr != cantonGroupVec->end(); ++ cgvItr)
							{
								(*cgvItr)->m_UIearnings = (*cgvItr)->m_earnings;
							}

							mygroupcanvas->doEvent(WCH_UI_UpdateData);
							m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2028,NULL));
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


/************************************************************************/
/*    crJXJUIChengChiGroupUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIChengChiGroupUpdateMethod::crJXJUIChengChiGroupUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIChengChiGroupUpdateMethod::crJXJUIChengChiGroupUpdateMethod( const crJXJUIChengChiGroupUpdateMethod & handle ):
	crMethod(handle),
	m_curgongxiantext(handle.m_curgongxiantext),
	m_shuaxinBtn(handle.m_shuaxinBtn),
	m_renmingBtn(handle.m_renmingBtn),
	m_Description(handle.m_Description),
	m_ZhoujuntechanyilanBtn(handle.m_ZhoujuntechanyilanBtn),
	m_strFoodImage(handle.m_strFoodImage),
	m_strWoodImage(handle.m_strWoodImage),
	m_strIronImage(handle.m_strIronImage),
	m_strHorseImage(handle.m_strHorseImage),
	m_strCopperImage(handle.m_strCopperImage),
	m_strGiftGoldImage(handle.m_strGiftGoldImage),
	m_strExpImage(handle.m_strExpImage),
	m_strAchievementImage(handle.m_strAchievementImage),
	m_strContributeImage(handle.m_strContributeImage),
	m_strExploitImage(handle.m_strExploitImage)
{
	unsigned char i = 0;
	for (; i < 8; ++ i)
	{
		m_icon[i] = handle.m_icon[i];
		m_input[i] = handle.m_input[i];
		m_goumaiBtn[i] = handle.m_goumaiBtn[i];
		m_itemcount[i] = handle.m_itemcount[i];
		if (i<3)
		{
			m_chengchiname[i] = handle.m_chengchiname[i];
			m_chakanBtn[i] = handle.m_chakanBtn[i];
		}
	}
}

void JXJ::crJXJUIChengChiGroupUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIChengChiGroupUpdateMethod::addParam( int i, const std::string& str )
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
		m_icon[i] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:		
	case 12:	
	case 13:	
	case 14:	
	case 15:
		m_input[i-8] = str;
		break;	
	case 16:	
	case 17:
	case 18:
	case 19:		
	case 20:	
	case 21:
	case 22:
	case 23:
		m_goumaiBtn[i-16] = str;
		break;
	case 24:
		m_curgongxiantext = str;
		break;
	case 25:
		m_shuaxinBtn = str;
		break;
	case 26:
	case 27:
	case 28:
		m_chengchiname[i-26] = str;
		break;
	case 29:
	case 30:
	case 31:
		m_chakanBtn[i-29] = str;
		break;
	case 32:
		m_renmingBtn = str;
		break;
	case 33:
		m_Description = str;
		break;
	case 34:
		m_ZhoujuntechanyilanBtn = str;
		break;
	case 35: 
		m_strFoodImage = str;//粮草
		break;
	case 36:
		m_strWoodImage = str;//木材
		break;
	case 37:
		m_strIronImage = str;//铁矿
		break;
	case 38: 
		m_strHorseImage = str;//马匹
		break;
	case 39: 
		m_strCopperImage = str;//铜钱
		break;
	case 40: 
		m_strExpImage = str;//经验
		break;
	case 41: 
		m_strAchievementImage = str;//功勋
		break;
	case 42: 
		m_strGiftGoldImage = str;//礼金
		break;
	case 43: 
		m_strContributeImage = str;//城防值
		break;
	case 44: 
		m_strExploitImage = str;//战功
		break;
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
		m_itemcount[i-45] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIChengChiGroupUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		if (data)
		{
			data->getParam(WCHDATA_JXJGoldingot,param);
			int goldingot = *(int*)param;
			data->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;
			ref_ptr<crButtonWidgetNode> shuaxinBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_shuaxinBtn));
			ref_ptr<crButtonWidgetNode> renmingBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_renmingBtn));
			ref_ptr<crButtonWidgetNode> ZhoujuntechanyilanBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_ZhoujuntechanyilanBtn));
			ref_ptr<crButtonWidgetNode> goumaiBtn[8];
			ref_ptr<crButtonWidgetNode> chakanBtn[3];
			ref_ptr<crStaticTextWidgetNode> description = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Description));
			ref_ptr<crStaticTextWidgetNode> chengchiname[3];
			ref_ptr<crStaticTextWidgetNode> num[8];
			ref_ptr<crStaticTextWidgetNode> itemcount[8];
			ref_ptr<crStaticTextWidgetNode> curgongxiantext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_curgongxiantext));
			ref_ptr<crImageBoxWidgetNode> image[8];

			for (unsigned char i = 0;i<8;++i)
			{
				goumaiBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goumaiBtn[i]));
				image[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
				num[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[i]));
				itemcount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_itemcount[i]));
				if (image[i].valid())
				{
					image[i]->setImageName("T_Icon_equipment_WuQi_W_24.img");
					image[i]->setImageName("");
				}
				if (goumaiBtn[i].valid())
				{
					goumaiBtn[i]->setEnable(false);
				}
				if (num[i].valid())
				{
					num[i]->setString(L"");
				}
				if (itemcount[i].valid()) 
				{
					itemcount[i]->setString(L"");
					itemcount[i]->setColor(FCOLOR_YELLOW);
				}
				if (i < 3)
				{
					chengchiname[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_chengchiname[i]));
					if (chengchiname[i].valid())
					{
						chengchiname[i]->setString(L"无");
					}
					chakanBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chakanBtn[i]));
					if (chakanBtn[i].valid())
					{
						chakanBtn[i]->setVisiable(false);
					}
				} 
			}

			if (shuaxinBtn.valid())
			{
				//shuaxinBtn->setVisiable(true);
				int RefreshCost = 0;
				data->getParam(WCHDATA_JXJSpecialtyRefreshTimes,param);
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
				if (giftgold + goldingot >= RefreshCost)
				{
					shuaxinBtn->setEnable(true);
				}
				else
				{
					shuaxinBtn->setEnable(false);
				}
			}
			if (renmingBtn.valid())
			{
				renmingBtn->setVisiable(false);
			}
			if (ZhoujuntechanyilanBtn.valid())
			{
				ZhoujuntechanyilanBtn->setVisiable(false);
			}

			crData *canvasdata = m_this->getDataClass();

			if (canvasdata)
			{
				CRCore::ScopedLock<crData> lock(*canvasdata);
				canvasdata->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				if (mySelfInfo) 
				{
					curgongxiantext->setString(crArgumentParser::appItoa(mySelfInfo->getGroupContribute()));
					myGroupPos = mySelfInfo->getGroupPos();
				}
				ref_ptr<crTableIO> cantontab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCantonInfoTab);
				ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				ref_ptr<crTableIO> equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
				if(!cantontab.valid() || !itemtab.valid() || !equipTab.valid()) return;
				crTableIO::StrVec record;
				int nameIdx = cantontab->getTitleIndex("名称");
				int iconIdx = itemtab->getTitleIndex("icon"); 
				canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
				MyAllocatedCantonVec *groupCantonVec = (MyAllocatedCantonVec *)param;
				bool isfenfenged = false;
				if (!groupCantonVec->empty())
				{
					int cantonId = (*groupCantonVec)[0];
					record.clear();
					if(cantontab->queryOneRecord(0,crArgumentParser::appItoa(cantonId),record)>=0)
					{
						if (chengchiname[0].valid()) chengchiname[0]->setString(record[nameIdx]);
						isfenfenged = true;
					}
				}
				if (myGroupPos == JXJGroup_FirstLeader && isfenfenged)
				{
					renmingBtn->setVisiable(true);
				}
				canvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
				MyApplyCantonVec * myApplyCantonVec = (MyApplyCantonVec *)param;
				unsigned char i = 1;
				if (!myApplyCantonVec->empty())
				{
					for (MyApplyCantonVec::iterator macvitr = myApplyCantonVec->begin();macvitr != myApplyCantonVec->end() && i<3;++macvitr)
					{
						int cantonId = *macvitr;
						if (cantontab->queryOneRecord(0,crArgumentParser::appItoa(cantonId),record)>=0)
						{
							if (i<3 && chengchiname[i].valid())
							{
								chengchiname[i]->setString(record[nameIdx]);
								++i;
							}
						}
					}
				}
				if (mySelfInfo)
				{
					GroupCantonSpecialtyVec & groupcantonSpecialtyVec = mySelfInfo->getGroupCantonSpecialtyVec();
					int _index = 0;
					for (GroupCantonSpecialtyVec::iterator gcsvitr = groupcantonSpecialtyVec.begin();gcsvitr != groupcantonSpecialtyVec.end() && _index <8;++gcsvitr)
					{
						//if (10 == gcsmitr->second.m_itemtype)
						//{
						//	if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(gcsmitr->second.m_itemid),record) >= 0)
						//	{
						//		if (_index >=0 && _index < 8 && image[_index].valid() && num[_index].valid() && gcsmitr->second.m_itemnum > 0)
						//		{
						//			image[_index]->setImageName(record[iconIdx]);
						//			num[_index]->setString(crArgumentParser::appItoa(gcsmitr->second.m_contribute));
						//			if (goumaiBtn[_index].valid())
						//			{
						//				goumaiBtn[_index]->setEnable(true);
						//			}
						//		}
						//	}
						//}
						if((*gcsvitr) == NULL)
						{
							++_index;
							continue;
						}
						std::string strImageName;
						std::string strItemCount;
						switch ((*gcsvitr)->m_itemtype)
						{
						case T_Food:
							strImageName = m_strFoodImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Wood:
							strImageName = m_strWoodImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Iron:
							strImageName = m_strIronImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Horse:
							strImageName = m_strHorseImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Copper:
							strImageName = m_strCopperImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Exp:
							strImageName = m_strExpImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Achievement:
							strImageName = m_strAchievementImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Giftgold:
							strImageName = m_strGiftGoldImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_TroopsEquip:
							{
								if(equipTab->queryOneRecord(0,crArgumentParser::appItoa((*gcsvitr)->m_itemid),record)>=0)
								{
									int eIconIndex = equipTab->getTitleIndex("icon");
									strImageName = record[eIconIndex];
									strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
								}											
							}
							break;
						case T_Item:
							{
								if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*gcsvitr)->m_itemid),record)>=0)
								{
									strImageName = record[iconIdx];
									strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
								}
							}
							break;
						case T_Contribute:
							strImageName = m_strContributeImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_Exploit:
							strImageName = m_strExploitImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						case T_GuoZhanJiFen:
							strImageName = m_strExploitImage;
							strItemCount = crArgumentParser::appItoa((*gcsvitr)->m_itemnum);
							break;
						}

						if (_index >=0 && _index < 8 && image[_index].valid() && num[_index].valid())
						{
							if ((*gcsvitr)->m_itemnum > 0)
							{
								image[_index]->setImageName(strImageName);
								num[_index]->setString(crArgumentParser::appItoa((*gcsvitr)->m_contribute));
								itemcount[_index]->setString(strItemCount);
								if (goumaiBtn[_index].valid())
								{
									goumaiBtn[_index]->setEnable(true);
								}
							}
							else
							{
								image[_index]->setImageName("UI_Icon_Shouji.img");
							}
						}
						++_index;
					}
				}
			}
			std::string notice;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			if(description.valid())
			{
				int mode = 2;
				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
				int texid = 5007;
				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
				noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				description->setString(notice);
			}

		}
	}
}


/************************************************************************/
/*    crJXJQueryFenFengChengChiSetMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryMyGroupFenFengChengChiMapMethod::crJXJQueryMyGroupFenFengChengChiMapMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryMyGroupFenFengChengChiMapMethod::crJXJQueryMyGroupFenFengChengChiMapMethod( const crJXJQueryMyGroupFenFengChengChiMapMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJQueryMyGroupFenFengChengChiMapMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryMyGroupFenFengChengChiMapMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJQueryMyGroupFenFengChengChiMapMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(playerGameData && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyGroupFenFengChengChiMap,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);	
	}
}

/************************************************************************/
/*    crJXJRecvQueryFenFengChengChiSetMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryMyGroupFenFengChengChiMapMethod::crJXJRecvQueryMyGroupFenFengChengChiMapMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryMyGroupFenFengChengChiMapMethod::crJXJRecvQueryMyGroupFenFengChengChiMapMethod( const crJXJRecvQueryMyGroupFenFengChengChiMapMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_mygroupcanvas(handle.m_mygroupcanvas),
	m_netType(handle.m_netType)
{
}

void JXJ::crJXJRecvQueryMyGroupFenFengChengChiMapMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyGroupFenFengChengChiMapMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvQueryMyGroupFenFengChengChiMapMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();				
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;
				if (mygroupid > 0 && shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						int count = 0;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(256);
						stream->_writeUChar(0);
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if (shili && shili->getDataClass())
						{
							crData *shilidata = shili->getDataClass();	
							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
							shilidata->getParam(WCHDATA_JXJPeopleMap,param);
							PeopleMap *peoplemap = (PeopleMap *)param;
							PeopleMap::iterator peoitr;
							std::string playername;
							ref_ptr<crData> groupdata;
							ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
							if (cmItr != consortiamap->end() && cmItr->second.valid())
							{
								groupdata = cmItr->second->getDataClass();
							}
							if (groupdata.valid())
							{
								groupdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
								FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
								for (FenFengedChengChiMap::iterator ffccmitr = fenFengedChengChiMap->begin();ffccmitr != fenFengedChengChiMap->end();++ffccmitr)
								{
									peoitr = peoplemap->find(ffccmitr->second);
									if (peoitr !=peoplemap->end())
									{
										playername = peoitr->second->getName();
									}
									stream->_writeInt(ffccmitr->first);
									stream->_writeString(playername);
									++count;
								}
							}

							int bufSize = stream->getBufSize();
							stream->seekBegin();
							stream->_writeUChar(count);
							stream->setBufSize(bufSize);
							/*if (count)
							{*/
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyGroupFenFengChengChiMap,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							/*}*/
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
					ClientFenFengedChengChiMap * fenFengChengChiMap = (ClientFenFengedChengChiMap *)param;
					fenFengChengChiMap->clear();
					unsigned char count = m_stream->_readUChar();
					int chengchiid = 0;
					std::string playername;
					for (unsigned char i = 0; i < count; ++ i)
					{
						chengchiid = m_stream->_readInt();
						playername = m_stream->_readString();
						fenFengChengChiMap->insert(std::make_pair(chengchiid,playername));
					}
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJUIAddGroupMemberInitMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIAddGroupMemberInitMethod::crJXJUIAddGroupMemberInitMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIAddGroupMemberInitMethod::crJXJUIAddGroupMemberInitMethod( const crJXJUIAddGroupMemberInitMethod& handle ):
	crMethod(handle),
	m_info(handle.m_info),
	m_nameEdit(handle.m_nameEdit)
{
}

void JXJ::crJXJUIAddGroupMemberInitMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIAddGroupMemberInitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_info = str;
		break;
	case 1:
		m_nameEdit = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJUIAddGroupMemberInitMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	if(playerGameData && netConductor && m_this)
	{
		
		ref_ptr<crStaticTextWidgetNode>  info = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_info));
		ref_ptr<crEditWidgetNode>  nameInput = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_nameEdit));
	//	ref_ptr<crButtonWidgetNode>  confirmBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_confirmBtn));

		if (info.valid())
		{
			std::string infoStr = "请在下方输入玩家名称：";
			info->setString(infoStr);
		}
		if (nameInput.valid())
		{
			nameInput->setEnable(true);
		}
	}
}

/************************************************************************/
/*    crJXJUIGroupChengChiEarningsLockMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIGroupChengChiEarningsLockMethod::crJXJUIGroupChengChiEarningsLockMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIGroupChengChiEarningsLockMethod::crJXJUIGroupChengChiEarningsLockMethod( const crJXJUIGroupChengChiEarningsLockMethod& handle ):
	crMethod(handle),
	m_groupList(handle.m_groupList),
	m_canvas(handle.m_canvas),
	m_confirm(handle.m_confirm),
	m_confirm_sw(handle.m_confirm_sw)
{
	for (unsigned char i = 0; i < 10; ++ i)
	{
		m_progressBar[i] = handle.m_progressBar[i];
		m_edit[i] = handle.m_edit[i];
	}
}

void JXJ::crJXJUIGroupChengChiEarningsLockMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIGroupChengChiEarningsLockMethod::addParam( int i, const std::string& str )
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
		m_edit[i - 2] = str;
		break;
	case 3:
		m_edit[i - 2] = str;
		break;
	case 4:
		m_edit[i - 2] = str;
		break;
	case 5:
		m_edit[i - 2] = str;
		break;
	case 6:
		m_edit[i - 2] = str;
		break;
	case 7:
		m_edit[i - 2] = str;
		break;
	case 8:
		m_edit[i - 2] = str;
		break;
	case 9:
		m_edit[i - 2] = str;
		break;
	case 10:
		m_edit[i - 2] = str;
		break;
	case 11:
		m_edit[i - 2] = str;
		break;
	case 12:
		m_progressBar[i - 12] = str;
		break;
	case 13:
		m_progressBar[i - 12] = str;
		break;
	case 14:
		m_progressBar[i - 12] = str;
		break;
	case 15:
		m_progressBar[i - 12] = str;
		break;
	case 16:
		m_progressBar[i - 12] = str;
		break;
	case 17:
		m_progressBar[i - 12] = str;
		break;
	case 18:
		m_progressBar[i - 12] = str;
		break;
	case 19:
		m_progressBar[i - 12] = str;
		break;
	case 20:
		m_progressBar[i - 12] = str;
		break;
	case 21:
		m_progressBar[i - 12] = str;
		break;
	case 22:
		m_confirm = str;
		break;
	case 23:
		m_confirm_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIGroupChengChiEarningsLockMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(canvas.valid() && netConductor && canvas->getDataClass())
		{
			ref_ptr<crTableWidgetNode> groupList1 = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));
			ref_ptr<crButtonWidgetNode> confirmBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_confirm));
			ref_ptr<crMultiSwitch > confirmBtn_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_confirm_sw));
			ref_ptr<crEditWidgetNode> inputEdit[10] = {NULL}; 
			ref_ptr<crProgressWidgetNode>  progressBar[10] = {NULL};

			unsigned char curRow = 0;
			if (groupList1.valid()) curRow = groupList1->getSelect();
			for (unsigned char i = 0; i < 10; ++ i)
			{
				inputEdit[i] = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_edit[i]));
				progressBar[i] = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progressBar[i]));
			}

			crData * canvasData = canvas->getDataClass();
			void * param;
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJCantonGroupVec,param);
			CantonGroupVec * cantonGroupVec = (CantonGroupVec *)param;
			
			if(groupList1.valid() && inputEdit[curRow].valid() && progressBar[curRow].valid())
			{
				ref_ptr<crCheckBoxWidgetNode>  lockWidget = dynamic_cast<crCheckBoxWidgetNode *>(m_this);
				if (lockWidget.valid())
				{
					if (lockWidget->getSelect())
					{
						confirmBtn->setVisiable(true);
						confirmBtn->setEnable(true);
						confirmBtn_sw->setActiveSwitchSet(0);
						progressBar[curRow]->setVisiable(false);
						inputEdit[curRow]->setVisiable(true);
						inputEdit[curRow]->setString(crArgumentParser::appFtoa((*cantonGroupVec)[curRow]->m_UIearnings / 10.0));
					}
					else
					{
						confirmBtn->setVisiable(true);
						confirmBtn->setEnable(false);
						confirmBtn_sw->setActiveSwitchSet(3);
						std::wstring text = inputEdit[curRow]->getString();
						std::string tmpText = CRIOManager::convertUTF16toUTF8(text.c_str());
						short earnings = atof(tmpText.c_str()) * 10;
						(*cantonGroupVec)[curRow]->m_UIearnings = earnings;
					
						progressBar[curRow]->setVisiable(true);
						progressBar[curRow]->setProgress(earnings / 1000.0);
						inputEdit[curRow]->setVisiable(false);
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJUIScrollChangePageMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIScrollChangePageMethod::crJXJUIScrollChangePageMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIScrollChangePageMethod::crJXJUIScrollChangePageMethod( const crJXJUIScrollChangePageMethod& handle ):
	crMethod(handle),
	m_totalPage(handle.m_totalPage),
	m_step(handle.m_step),
	m_perpageNum(handle.m_perpageNum),
	m_curPage(handle.m_curPage),
	m_canvas(handle.m_canvas)
{
}

void JXJ::crJXJUIScrollChangePageMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIScrollChangePageMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_step = atoi(str.c_str());
		break;
	case 2:
		m_curPage = atoi(str.c_str());
		break;
	case 3:
		m_totalPage = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJUIScrollChangePageMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		do 
		{
			if (!canvas || !(canvas->getDataClass()) )
			{
				break;
			}
			void *param;
			crData * canvasData = canvas->getDataClass();
			canvasData->getParam(m_curPage,param);
			int curPage =  *(int *)param;
			canvasData->getParam(m_totalPage,param);
			int totalPage = *(int *) param;

			curPage += m_step;
			if (curPage > totalPage)
				curPage = totalPage;
			if (curPage == 0)
				curPage = 1;
			canvasData->inputParam(m_curPage,&curPage);

		} while (0);
	}
}


/************************************************************************/
/*    crJXJResetScrollPagMethod                                                                  */
/************************************************************************/

JXJ::crJXJResetScrollPageMethod::crJXJResetScrollPageMethod():
	m_this(NULL)
{
}

JXJ::crJXJResetScrollPageMethod::crJXJResetScrollPageMethod( const crJXJResetScrollPageMethod& handle ):
	crMethod(handle),
	m_totalPage(handle.m_totalPage),
	m_perpageNum(handle.m_perpageNum),
	m_curPage(handle.m_curPage),
	m_canvas(handle.m_canvas),
	m_scroll(handle.m_scroll)
{
}

void JXJ::crJXJResetScrollPageMethod::inputParam( int i, void *param )
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

void JXJ::crJXJResetScrollPageMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_curPage = atoi(str.c_str());
		break;
	case 2:
		m_totalPage = atoi(str.c_str());
		break;
	case 3:
		m_perpageNum = atoi(str.c_str());
		break;
	case 4:
		m_scroll = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJResetScrollPageMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		do 
		{
			if (!canvas || !(canvas->getDataClass()) )
			{
				break;
			}
			ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scroll));
			if (scroll.valid()) break;

			void *param;
			crData * canvasData = canvas->getDataClass();
			canvasData->getParam(m_curPage,param);
			int curPage =  *(int *)param;
			canvasData->getParam(m_totalPage,param);
			int totalPage = *(int *) param;

			float val = scroll->getValue();
			m_curPage = (totalPage - m_perpageNum + 1) * val;

			canvasData->inputParam(m_curPage,&curPage);

		} while (0);
	}
}

/************************************************************************/
/*    crJXJSetRankUIRadioSelectMethod                                                                  */
/************************************************************************/

JXJ::crJXJSetRankUIRadioSelectMethod::crJXJSetRankUIRadioSelectMethod():
	m_this(NULL)
{
}

JXJ::crJXJSetRankUIRadioSelectMethod::crJXJSetRankUIRadioSelectMethod( const crJXJSetRankUIRadioSelectMethod& handle ):
	crMethod(handle),
	m_rankRadio(handle.m_rankRadio),
	m_rankCanvas(handle.m_rankCanvas),
	m_radioPage(handle.m_radioPage)
{
}

void JXJ::crJXJSetRankUIRadioSelectMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSetRankUIRadioSelectMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_rankCanvas = str;
		break;
	case 1:
		m_rankRadio = str;
		break;
	case 2:
		m_radioPage = atoi(str.c_str());
		break;
	case 3:
		m_page_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJSetRankUIRadioSelectMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_rankCanvas);
	if (canvas.valid())
	{
		crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_page_sw));
		ref_ptr<crRadioGroupWidgetNode>  rankRadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_rankRadio));
		if (rankRadio.valid()) rankRadio->select(m_radioPage);
		if (pagesw) pagesw->setActiveSwitchSet(m_radioPage);
	}
}

/************************************************************************/
/*    crJXJShowLeaveGroupConfirmMethod                                                                  */
/************************************************************************/

JXJ::crJXJShowGroupConfirmMethod::crJXJShowGroupConfirmMethod():
	m_this(NULL)
{
}

JXJ::crJXJShowGroupConfirmMethod::crJXJShowGroupConfirmMethod( const crJXJShowGroupConfirmMethod& handle ):
	crMethod(handle),
	m_msgbox(handle.m_msgbox),
	m_msg(handle.m_msg),
	m_ok(handle.m_ok),
	m_idx(handle.m_idx)
{
}

void JXJ::crJXJShowGroupConfirmMethod::inputParam( int i, void *param )
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

void JXJ::crJXJShowGroupConfirmMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_msgbox = str;
		break;
	case 1:
		m_msg = str;
		break;
	case 2:
		m_idx = atoi(str.c_str());
		break;
	}
}

void JXJ::crJXJShowGroupConfirmMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
		if (msgCanvas.valid())
		{
			ref_ptr<crHypertextWidgetNode > msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msg));
			std::string notice;
			
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			int mode = 2;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&m_idx);
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&notice);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);

			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content");
			cfg_script.Add("Text",notice);
			cfg_script.Pop();
			cfg_script.Pop();
			if (msg.valid())
			{
				msg->setHypertext(cfg_script);
			}
			crData *dlgData = msgCanvas->getDataClass();
			int commandtype = CDP_Widget;
			dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
			dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
	}
}


/************************************************************************/
/*    crJXJGameServerDayZeroMethod                                                                  */
/************************************************************************/

JXJ::crJXJGameServerDayZeroMethod::crJXJGameServerDayZeroMethod():
	m_this(NULL),
	m_lasttimer(0L)
{
}

JXJ::crJXJGameServerDayZeroMethod::crJXJGameServerDayZeroMethod( const crJXJGameServerDayZeroMethod & handle ):
	crMethod(handle),
	m_lasttimer(handle.m_lasttimer)
{
}


void JXJ::crJXJGameServerDayZeroMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGameServerDayZeroMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJGameServerDayZeroMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		time_t nowTime = time(0);
		struct  tm sTm = *localtime(&nowTime);

		void * param;
		crData * gsBrainData = m_this->getDataClass();
		
		// 军团每日刷新
		gsBrainData->getParam(WCHDATA_JXJGroupDayLastUpdate,param);
		time_t groupDayLastUpdate = *(time_t *)param;

		unsigned char hour = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTimeOfGetChengChiEarnings	).c_str());
		//if (nowTime - groupDayLastUpdate >= 86400)
		//{
		//	// 服务器重启时 检查一次，只会检查一次
		//	m_this->doEvent(WCH_JXJGameServerGroupDayZero);
		//}
		//else 
		if (sTm.tm_hour == hour && sTm.tm_min == 0 && nowTime - groupDayLastUpdate > 60)
		{
			// 1分钟内只执行一次
			m_this->doEvent(WCH_JXJGameServerGroupDayZero);
		}
		gsBrainData->getParam(WCHDATA_JXJGroupWeekLastUpdate,param);
		time_t groupWeekLastUpdate = *(time_t *)param;

		// 军团,势力每周刷新
		//if (nowTime - groupWeekLastUpdate >= 86400 * 7)
		//{
		//	// 服务器重启时 检查一次，只会检查一次
		//	m_this->doEvent(WCH_JXJGameServerGroupWeekZero);
		//}
		//else 
		if (sTm.tm_wday == 1 && sTm.tm_hour == 0 && sTm.tm_min == 0 && nowTime - groupWeekLastUpdate > 60)
		{
			// 周一凌晨
			m_this->doEvent(WCH_JXJGameServerGroupWeekZero);
		}

		//周常任务数据每周5点刷新
		if (sTm.tm_wday == 1 && sTm.tm_hour == 5 && sTm.tm_min == 0 && sTm.tm_sec == 0)
		{
			unsigned char count = 0;
			crJXJShili *shili;
			crData *shiliData;
			for( int i = 0; i < 3; ++ i)
			{
				gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();
				
				if(shiliData)
					shiliData->inputParam(WCHDATA_JXJZhanLingCityCount,&count);
			}
		}
		//军团开服活动刷新，开服第三天凌晨5点刷新
		int Deadline = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaPromotionDeadline, 0).c_str())); //活动截止到开服第几天
		int startedDays = 0;
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&startedDays, NULL));
		gsBrainData->getParam(WCHDATA_JXJConsortiaPromotionLastUpdateTime,param);
		time_t consortiaUpdate = *(time_t *)param;
		if (startedDays == Deadline - 1 && sTm.tm_hour == 5 && sTm.tm_min == 0 && nowTime - consortiaUpdate > 60)
		{
			m_this->doEvent(WCH_JXJConsortiaPromotionUpdate);
			//consortiaUpdate = nowTime;
			gsBrainData->inputParam(WCHDATA_JXJConsortiaPromotionLastUpdateTime, &nowTime);
		}

		// 势力每日刷新
		gsBrainData->getParam(WCHDATA_JXJShiliDayLastUpdate,param);
		time_t shiliDayUpdate = *(time_t *)param;
		int interval = nowTime - shiliDayUpdate;
		if (interval >= 86400)
		{
			gsBrainData->inputParam(WCHDATA_JXJShiliDayLastUpdate,&nowTime);
			// 服务器重启时 检查一次，只会检查一次
			m_this->doEvent(WCH_JXJGameServerShiliDayZero);
			////世界战场国家积分刷新
			crJXJShili *shili;
			crData *shiliData;
			for( int i = 0; i < 3; ++ i)
			{
				gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();

				if(shiliData)
					shiliData->inputParam(WCHDATA_JXJGuoJiaJiFen,&crVector3i(0,0,0));
			}
		}
		else if (sTm.tm_hour == 0 && sTm.tm_min == 0 && interval > 60)
		{
			gsBrainData->inputParam(WCHDATA_JXJShiliDayLastUpdate,&nowTime);
			// 1分钟内只执行一次
			m_this->doEvent(WCH_JXJGameServerShiliDayZero);

			int clandedplayer[3] = {0};
			unsigned char chavecityold[3] = {0};
			unsigned char chavecitynew[3] = {0};
			unsigned char cConqueredcitynew[3] = {0};
			std::set<int> * landedplayerset = NULL;
			
			////世界战场国家积分刷新
			crJXJShili *shili;
			crData *shiliData;
			for( int i = 0; i < 3; ++ i)
			{
				gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();

				if(shiliData)
				{
					shiliData->inputParam(WCHDATA_JXJGuoJiaJiFen,&crVector3i(0,0,0));

					shiliData->getParam(WCHDATA_JXJTodayHaveLandedPlayerSet,param);
					landedplayerset = (std::set<int> *)param;
					clandedplayer[i] = landedplayerset->size();

					shiliData->getParam(WCHDATA_JXJYesterdayConqueredTity,param);
					chavecityold[i] = *(unsigned char*)param;

					shiliData->getParam(WCHDATA_JXJTodayConqueredTity,param);
					cConqueredcitynew[i] = *(unsigned char*)param;
				}
			}

			//国家强弱刷新
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
			ChengchiMap *chengchimap = (ChengchiMap *)param;
			for( ChengchiMap::iterator itr = chengchimap->begin();
				itr != chengchimap->end();
				++itr )
			{
				if(itr->second->getChengzhuShili() == c_startShiliID)
					chavecitynew[0] += 1;
				else if(itr->second->getChengzhuShili() == c_startShiliID+1)
					chavecitynew[1] += 1;
				else if(itr->second->getChengzhuShili() == c_startShiliID+2)
					chavecitynew[2] += 1;
			}

			unsigned char shiliid = 0;
			float cDengji[3] = {0.0};
			gsBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
			LevelRankingMap * lvRankingmap = (LevelRankingMap *)param;
			LevelRankingMap::iterator itr = lvRankingmap->begin();
			int limitcount=0;
			for (; itr!=lvRankingmap->end()&&limitcount<50; limitcount++,itr++)
			{
				shiliid = itr->second->getShiliID();
				if(shiliid == 10)
					cDengji[0] = cDengji[0] + 1.0;
				else if(shiliid == 11)
					cDengji[1] = cDengji[1] + 1.0;
				else if(shiliid == 12)
					cDengji[2] = cDengji[2] + 1.0;
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			
			float alllanded = clandedplayer[0] + clandedplayer[1] + clandedplayer[2];
			float allconquered = cConqueredcitynew[0] + cConqueredcitynew[1] + cConqueredcitynew[2];
			float guolizhi[3] = {0.0};
			for( int i = 0; i < 3; ++ i)
			{
				if(alllanded <= 0 && allconquered <= 0)
					guolizhi[i] = (chavecitynew[i]-chavecityold[i]>0?0.3:0.0)*300.0 + cDengji[i]*200.0/(limitcount>0?(float)limitcount:1.0);
				else if(alllanded <= 0)
					guolizhi[i] = ((float)(cConqueredcitynew[i]*200))/allconquered + (chavecitynew[i]-chavecityold[i]>0?0.3:0.0)*300.0 + cDengji[i]*200.0/(limitcount>0?(float)limitcount:1.0);
				else if(allconquered <= 0)
					guolizhi[i] = ((float)(clandedplayer[i]*300))/alllanded + (chavecitynew[i]-chavecityold[i]>0?0.3:0.0)*300.0 + cDengji[i]*200.0/(limitcount>0?(float)limitcount:1.0);
				else
					guolizhi[i] = ((float)(clandedplayer[i]*300))/alllanded + ((float)(cConqueredcitynew[i]*200))/allconquered + (chavecitynew[i]-chavecityold[i]>0?0.3:0.0)*300.0 + cDengji[i]*200.0/(limitcount>0?(float)limitcount:1.0);
			}

			shiliid = 10;
			bool isLoseCountry = false;
			m_this->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
				guolizhi[0] = 0;

			shiliid = 11;
			isLoseCountry = false;
			m_this->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
				guolizhi[1] = 0;

			shiliid = 12;
			isLoseCountry = false;
			m_this->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
			if(isLoseCountry)
				guolizhi[2] = 0;
			
			float allguolizhi = guolizhi[0] + guolizhi[1] + guolizhi[2];
			bool shiliModify = true;
			for( int i = 0; i < 3; ++ i)
			{
				gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();
				if(shiliData)
				{
					shiliData->excHandle(MAKEINT64(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJTodayHaveLandedPlayerSet,param);
					landedplayerset = (std::set<int> *)param;
					landedplayerset->clear();

					shiliData->getParam(WCHDATA_JXJYesterdayConqueredTity,param);
					unsigned char *countold = (unsigned char*)param;
					*countold = chavecitynew[i];

					shiliData->getParam(WCHDATA_JXJTodayConqueredTity,param);
					unsigned char *countnew = (unsigned char*)param;
					*countnew = 0;

					shiliData->getParam(WCHDATA_JXJCountryStrength,param);
					unsigned char *countrystrength = (unsigned char*)param;

					float rato = 0.0;
					if (allguolizhi > 0)
					{
						rato = guolizhi[i]/allguolizhi;
						if(rato > 0.5)
							*countrystrength = 3;
						else if(rato >= 0.4)
							*countrystrength = 2;
						else if(rato >= 0.25)
							*countrystrength = 1;
						else
							*countrystrength = 0;
					}
					else
						*countrystrength = 1;

					//CRCore::notify(CRCore::ALWAYS)<<"本国登录人数:"+crArgumentParser::appFtoa(clandedplayer[i])+",三国总登录人数:"+crArgumentParser::appFtoa(alllanded)<<std::endl;
					//CRCore::notify(CRCore::ALWAYS)<<"本国今日城池数:"+crArgumentParser::appFtoa(chavecitynew[i])+",本国昨日城池数:"+crArgumentParser::appFtoa(chavecityold[i])<<std::endl;
					//CRCore::notify(CRCore::ALWAYS)<<"本国在后者中占的个数:"+crArgumentParser::appFtoa(cDengji[i])+",统计的世界等级数:"+crArgumentParser::appItoa(limitcount)<<std::endl;
					//CRCore::notify(CRCore::ALWAYS)<<"本国占领城池:"+crArgumentParser::appFtoa(cConqueredcitynew[i])+",三国总占领城池:"+crArgumentParser::appFtoa(allconquered)<<std::endl;
					//CRCore::notify(CRCore::ALWAYS)<<"本国国力值:"+crArgumentParser::appFtoa(guolizhi[i])+",总国力值:"+crArgumentParser::appFtoa(allguolizhi)<<std::endl;
					//CRCore::notify(CRCore::ALWAYS)<<"比例为:"+crArgumentParser::appFtoa(rato)<<std::endl<<std::endl<<std::endl;

					shiliData->inputParam(WCHDATA_JXJShiliModify,&shiliModify);
					shiliData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

			//显示选举图标
			if (sTm.tm_wday == 6 || sTm.tm_wday == 0 || sTm.tm_wday == 1)
			{
				m_this->doEvent(WCH_JXJElectIcon);
			}
			//周日0点检查国家官员候选人列表，将未自荐的玩家从列表中删除
			if (sTm.tm_wday == 0)
			{
				// 周日0点
				crJXJShili *shili;
				crData *shiliData;
				CandidateVec *candidateVecT;
				CandidateVec::iterator cvItr;
				int maxum = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCountryVotePeopleMaxNum).c_str()));
				//gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				bool modify = false;
				//gsBrainData->getParam(WCHDATA_JXJLevelRankingMap,param);
				//LevelRankingMap * lvRankingmap = (LevelRankingMap *)param;
				//gsBrainData->getParam(WCHDATA_JXJExpRankingMap,param);
				//ExpRankingMap * expRankingmap = (ExpRankingMap *)param;
				//gsBrainData->getParam(WCHDATA_JXJAccomplishRankingMap,param);
				//AccomplishRankingMap * accomplishRankingmap = (AccomplishRankingMap *)param;
				for( int i = 0; i < 3; ++ i)
				{
					gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
					shili = (crJXJShili *)param;
					shiliData = shili->getDataClass();
					shiliData->excHandle(MAKEINT64(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJCountryCandidateVecT,param);
					candidateVecT = (CandidateVec*)param;
					if (candidateVecT)
					{
						for (cvItr = candidateVecT->begin(); cvItr != candidateVecT->end();)
						{
							if (cvItr->valid())
							{
								if ((*cvItr)->nNumber == 0)
								{
									cvItr = candidateVecT->erase(cvItr);
									modify = true;
									continue;
								}
							}
							++ cvItr;
						}
					}
					if(modify) shiliData->inputParam(WCHDATA_JXJShiliModify,&modify);
					shiliData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				//gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}

		// 统计每日刷新
		if (sTm.tm_hour == 23 && sTm.tm_min == 59 && nowTime - m_lasttimer > 60)
		{
			// 统计
			m_this->doEvent( WCH_JXJStatisticUpdate);
			m_lasttimer = nowTime;
		}
		m_this->doEvent(WCH_JXJServer2SecZero);
	}
}

/************************************************************************/
/*    crJXJGameServerGroupDayZeroMethod                                                                  */
/************************************************************************/

JXJ::crJXJGameServerGroupZeroMethod::crJXJGameServerGroupZeroMethod():
	m_this(NULL)
{
}

JXJ::crJXJGameServerGroupZeroMethod::crJXJGameServerGroupZeroMethod( const crJXJGameServerGroupZeroMethod & handle ):
	crMethod(handle),
	m_zeroType(handle.m_zeroType)
{
}

void JXJ::crJXJGameServerGroupZeroMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGameServerGroupZeroMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_zeroType = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJGameServerGroupZeroMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * gsBrainData = m_this->getDataClass();
		if (gsBrainData)
		{
			void * param;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

			gsBrainData->getParam(WCHDATA_JXJDismissGroupMap,param);
			DismissGroupMap * dismissGroupMap = (DismissGroupMap *)param;

			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;

			ConsortiaMap::iterator it = consortiamap->begin();
			for (; it != consortiamap->end(); )
			{
				if (it->second.valid())
				{
					switch (m_zeroType)
					{
					case JXJGroupServerZero_2Sec:
						{
							it->second->twoSecZero();
							if (it->second->willDismiss())
							{
								dismissGroupMap->insert(DismissGroupMap::value_type(it->first,it->second));
								it = consortiamap->erase(it);
							}
							else
							{
								++ it;
							}
						}
						break;
					case JXJGroupServerZero_day:
						{
							it->second->dayZero();
							++ it;
						}
						break;
					case JXJGroupServerZero_week:
						{
							it->second->weekZero();
							/*it->second->updateLeaderVec();*/
							++ it;
						}
						break;
					case JXJGroupServerZero_settleEarnings:
						{
							it->second->settleEarngins();
							++ it;
						}
						break;
					default:
						break;
					}
				}
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

JXJ::crJXJMyGroupCanvasGlobalUpdateMethod::crJXJMyGroupCanvasGlobalUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJMyGroupCanvasGlobalUpdateMethod::crJXJMyGroupCanvasGlobalUpdateMethod( const crJXJMyGroupCanvasGlobalUpdateMethod & handle ):
	crMethod(handle),
	m_sw(handle.m_sw),
	m_title(handle.m_title),
	m_dongHua1(handle.m_dongHua1)
{
}

void JXJ::crJXJMyGroupCanvasGlobalUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJMyGroupCanvasGlobalUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_sw = str;
		break;
	case 1:
		m_title = str;
		break;
	case 2:
		m_dongHua1 = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJMyGroupCanvasGlobalUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * canvasData = m_this->getDataClass();
		crMultiSwitch * sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		ref_ptr<crStaticTextWidgetNode>  title = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title));
		crSequence *dongHua1 = dynamic_cast<crSequence*>(m_this->getChildNode(m_dongHua1));
		if (canvasData)
		{
			void * param;
			canvasData->getParam(WCHDATA_JXJMyGroupInfo,param);
			crJXJGroupInfo * groupInfo = (crJXJGroupInfo *) param;
			int dismissTime = groupInfo->getDismissTime();
			if (-1 != dismissTime)
			{
				sw->setActiveSwitchSet(1);

				char tmp[10];
				sprintf_s(tmp,"%02d:%02d\0",dismissTime / 3600,(dismissTime % 3600) / 60);
				std::string text(tmp);
				title->setString("人数<10，解散计时 " + text);
			}
			else
			{
				sw->setActiveSwitchSet(0);
			}
			if (dongHua1) dongHua1->setVisiable(false);
			canvasData->getParam(WCHDATA_JXJConsortiaHasNewApplyer,param);
			bool hasNewApplyer = *(bool *)param;
			canvasData->getParam(WCHDATA_JXJConsortiaHadOpenShenHe,param);
			bool hadOpenShenHe = *(bool *)param;
			canvasData->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			int myGroupPos = 0;
			if (mySelfInfo)
				myGroupPos = mySelfInfo->getGroupPos();
			PermissionCheck checkPermission;
			if (hasNewApplyer && !hadOpenShenHe && checkPermission(myGroupPos,JXJGroup_AgreeGroupMember))
			{
				if (dongHua1) 
				{
					dongHua1->setVisiable(true);
					dongHua1->setMode(crSequence::START);
					dongHua1->setDuration(0.1,5);
				}
			}
			else
			{
				if (dongHua1)
				{
					dongHua1->setVisiable(false);
					dongHua1->setMode(crSequence::STOP);
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJDismissTimerMethod                                                                  */
/************************************************************************/

JXJ::crJXJDismissTimerMethod::crJXJDismissTimerMethod():
	m_this(NULL)
{
}

JXJ::crJXJDismissTimerMethod::crJXJDismissTimerMethod( const crJXJDismissTimerMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{
}

void JXJ::crJXJDismissTimerMethod::inputParam( int i, void *param )
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

void JXJ::crJXJDismissTimerMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJDismissTimerMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * canvasData = m_this->getDataClass();
		if (canvasData)
		{
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			void * param;
			canvasData->getParam(WCHDATA_JXJMyGroupInfo,param);
			crJXJGroupInfo * groupInfo = (crJXJGroupInfo *) param;
			int dismissTime = groupInfo->getDismissTime();
			if (-1 != dismissTime && dismissTime)
			{
				-- dismissTime;
				groupInfo->setDismissTime(dismissTime);
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}


/************************************************************************/
/*    crJXJGameServerDismissGroupDisposeMethod                                                                  */
/************************************************************************/

JXJ::crJXJGameServerDismissGroupDisposeMethod::crJXJGameServerDismissGroupDisposeMethod():
	m_this(NULL)
{
}

JXJ::crJXJGameServerDismissGroupDisposeMethod::crJXJGameServerDismissGroupDisposeMethod( const crJXJGameServerDismissGroupDisposeMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{
}

void JXJ::crJXJGameServerDismissGroupDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGameServerDismissGroupDisposeMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJGameServerDismissGroupDisposeMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * gsBrainData = m_this->getDataClass();
		if (gsBrainData)
		{
			void * param;

			crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

			gsBrainData->getParam(WCHDATA_JXJDismissGroupMap,param);
			DismissGroupMap * dismissGroupMap = (DismissGroupMap *)param;
			if (!(dismissGroupMap->empty()) && gameServer && dbConductor) 
			{
				crPlayerDataEventPacket packet;

				CRCore::ref_ptr<crRemoveConsortia > removeGroup = new crRemoveConsortia;
				crDBSavePacket dbpacket;
				DismissGroupMap::iterator dgmIt = dismissGroupMap->begin();
				for (; dgmIt != dismissGroupMap->end(); ++ dgmIt)
				{
					if (!(dgmIt->second.valid())) continue;
					do 
					{
						crData * groupData = dgmIt->second->getDataClass();
						if (!groupData) break;

						groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
						FenFengedCantonMap  * fenFengedCantonMap = (FenFengedCantonMap *)param;
						int shiliid = dgmIt->second->getShiLi();

						if(shiliid < 10 || shiliid > 12) break;

						typedef std::vector<int> CantonVec;
						CantonVec cantonVec;

						if (fenFengedCantonMap->empty() == false)
						{
							FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->begin();
							for (;ffcmItr != fenFengedCantonMap->end(); ++ ffcmItr)
							{
								cantonVec.push_back(ffcmItr->first);
							}
						}

						groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
						ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
						if (applyCantonMap->empty() == false)
						{
							ApplyCantonMap::iterator acmItr = applyCantonMap->begin();
							for (; acmItr != applyCantonMap->end(); ++ acmItr)
							{
								cantonVec.push_back(acmItr->first);
							}
						}
						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						crData * shiliData = shili->getDataClass();

						shiliData->excHandle(MAKEINT64(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJShiliCantonMap,param);
						CantonMap * cantonMap = (CantonMap *)param;
						CantonMap::iterator cmItr ;
						CantonVec::iterator cvItr = cantonVec.begin();
						ChengChiEarningsMap::iterator ccemItr; 
						int groupId = dgmIt->second->getID();
						for (; cvItr != cantonVec.end(); ++ cvItr)
						{
							cmItr = cantonMap->find(*cvItr);
							if (cmItr != cantonMap->end())
							{
								if (cmItr->second->getConsortiaId() ==  groupId)
								{
									cmItr->second->setConsortiaId(0);
								}
								ChengChiEarningsMap & chengChiEarningsMap = cmItr->second->getChengChiEarningsMap();
								ccemItr = chengChiEarningsMap.find(groupId);
								if (ccemItr != chengChiEarningsMap.end())
								{
									chengChiEarningsMap.erase(ccemItr);
								}
							}
						}
						shiliData->excHandle(MAKEINT64(WCH_LockData,0));

						groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator cmmIt = memberMap->begin();
						for (;cmmIt != memberMap->end(); ++ cmmIt)
						{
							int playerid = cmmIt->second->getPlayerID();
							ref_ptr<crGameServerPlayerData> playerData = 
								dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
							if(playerData.valid()&&playerData->getPlayerGameData()&&playerData->getPlayerGameData()->getDataClass())
							{
								crData *dataPtr = playerData->getPlayerGameData()->getDataClass();
								int mygroupid = 0;
								dataPtr-> inputParam(WCHDATA_JXJConsortiaID,&mygroupid);
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJGroupNotExist,NULL);
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
						dgmIt->second->delFromRank(gsBrainData);
					} while (0);
					std::string groupName ;
					dgmIt->second->getName(groupName );
					std::string logdata = groupName;
					GameLogData gamelog(Log_DimissGroup,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(dgmIt->first,&gamelog));

					removeGroup->buildUpdateSql(dgmIt->first);
					crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,removeGroup.get());
					dbConductor->getNetManager()->sendPacket("all",dbpacket);

					
				}
				dismissGroupMap->clear();
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJGroupNotExistMethod                                                                  */
/************************************************************************/

JXJ::crJXJGroupNotExistMethod::crJXJGroupNotExistMethod():
	m_this(NULL)
{
}

JXJ::crJXJGroupNotExistMethod::crJXJGroupNotExistMethod( const crJXJGroupNotExistMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJGroupNotExistMethod::inputParam( int i, void *param )
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
		}
		break;
	}
}

void JXJ::crJXJGroupNotExistMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJGroupNotExistMethod::operator()( crHandle &handle )
{
	do 
	{
		if (!m_this) break;
		crData *data = m_this->getDataClass();
		if (!data) break;
		int mygroupid = 0;
		data-> inputParam(WCHDATA_JXJConsortiaID,&mygroupid);
		m_this->doEvent(WCH_JXJGroupNotExitUISwitch);
	} while (0);

	
}


/************************************************************************/
/*    crJXJSaveConsortiaLvRankMethod                                                                  */
/************************************************************************/

JXJ::crJXJSaveConsortiaLvRankMethod::crJXJSaveConsortiaLvRankMethod():
	m_this(NULL)
{
}

JXJ::crJXJSaveConsortiaLvRankMethod::crJXJSaveConsortiaLvRankMethod( const crJXJSaveConsortiaLvRankMethod & handle ):
	crMethod(handle),
	m_rankFile(handle.m_rankFile)
{
}

void JXJ::crJXJSaveConsortiaLvRankMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSaveConsortiaLvRankMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_rankFile = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJSaveConsortiaLvRankMethod::operator()( crHandle &handle )
{
	if(m_this)
	{
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		int expRankingCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking,0).c_str());
		ref_ptr<crTableIO> groupLvRankTab;
		crTableIO::StrVec record;
		crTableIO::StrVec title;
		title.push_back("groupid");
		title.push_back("groupName");
		title.push_back("leaderName");
		title.push_back("level");
		title.push_back("memberCount");
		title.push_back("troops");
		title.push_back("exploit");
		title.push_back("countryid");
		data->getParam(WCHDATA_JXJConsortiaLvRank,param);
		ConsortialvRank *groupLvRank = (ConsortialvRank *)param;
		ConsortialvRank::RankSet & rankSet = groupLvRank->getRankSet();
		groupLvRankTab = new crTableIO;
		groupLvRankTab->setTitleVec(title);
		std::string groupName;
		std::string leaderName;
		ConsortialvRank::RankSet::iterator itr = rankSet.begin();
		for(int j = 0; j < expRankingCount && itr != rankSet.end(); ++itr,++j )
		{
			if (itr->valid() == false) continue;
			record.clear();
			ConsortiaLvRankItem* consortia =(ConsortiaLvRankItem*) (itr->get());
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_id)));
			record.push_back(consortia->m_groupName);
			record.push_back(consortia->m_leaderName);
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_lv)));
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_memberCount)));
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_troops)));
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_exploit)));
			record.push_back(crArgumentParser::appItoa((int)(consortia->m_shili)));
			groupLvRankTab->addData(record);
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
		CRNet::crNetConductor * gameServerConductor = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServerConductor->getNetDataManager();
		CRNetApp::crGameServerCallback *netCallback = dynamic_cast<CRNetApp::crGameServerCallback *>(netDataManager->getNetCallback());
		std::string filepath = crArgumentParser::getFilePath(m_rankFile);
		std::string filename = crArgumentParser::getSimpleFileName(m_rankFile);
		std::string relFileName = filepath + "/" + netCallback->getServerName() + "/"+ filename;

		groupLvRankTab->saveToFileStreamNoCook(relFileName,SM_Record);
		
	}
}


/************************************************************************/
/*    crJXJGetChengChiEarningsMethod                                                                  */
/************************************************************************/

//JXJ::crJXJGetChengChiEarningsMethod::crJXJGetChengChiEarningsMethod():
//	m_this(NULL)
//{
//}
//
//JXJ::crJXJGetChengChiEarningsMethod::crJXJGetChengChiEarningsMethod( const crJXJGetChengChiEarningsMethod & handle ):
//	crMethod(handle)
//{
//}
//
//void JXJ::crJXJGetChengChiEarningsMethod::inputParam( int i, void *param )
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
//		m_this = (crWidgetNode *)param;
//		break;
//	}
//}
//
//void JXJ::crJXJGetChengChiEarningsMethod::addParam( int i, const std::string& str )
//{
//}
//
//void JXJ::crJXJGetChengChiEarningsMethod::operator()( crHandle &handle )
//{
//	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(m_this && playerGameData)
//	{
//		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//		if(netConductor)
//		{
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGetChengChiEarnings,NULL);
//			netConductor->getNetManager()->sendPacket("all",packet);
//		}
//	}
//}


/************************************************************************/
/*    crJXJRecvGetChengChiEarningsMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvGetChengChiEarningsMethod::crJXJRecvGetChengChiEarningsMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvGetChengChiEarningsMethod::crJXJRecvGetChengChiEarningsMethod( const crJXJRecvGetChengChiEarningsMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvGetChengChiEarningsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvGetChengChiEarningsMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJRecvGetChengChiEarningsMethod::operator()( crHandle &handle )
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data-> getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *( int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				unsigned char suc = JXJGroupGetEarningsRet_failed_unkwon;
				if (mygroupid > 0 && shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
						ChengchiMap *chengchiMap = (ChengchiMap *)param;
						ChengchiMap::iterator chItr;
						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
						ref_ptr<crData>groupData;
						ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
						if(cmItr!=consortiamap->end())
						{
							groupData = cmItr->second->getDataClass();
						}
						if(groupData.valid())
						{
							do 
							{
								groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
								ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
								ConsortiaMemberMap::iterator cmmIt = memberMap->find(playerid);
								if (cmmIt == memberMap->end()) break;
								if ( !(cmmIt->second.valid()) ) break;
								if (cmmIt->second->m_newer)
								{
									suc = JXJGroupGetEarningsRet_failed_newer;
									break;
								}
								GroupChengChiEarningMap &groupchengchiearningMap = cmmIt->second->getGroupChengChiEarningMap();
								GroupChengChiEarningMap::iterator gccemitr;
								/*	if ( cmmIt ->second->m_getEarningsYesterday) 
								{
								suc = JXJGroupGetEarningsRet_failed_hadGetted;
								break;
								}*/
							
								if (groupchengchiearningMap.empty())
								{
									ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
									if(!chengchitab.valid()) break;;
									crTableIO::StrVec record;
									crTableIO::DataVec chengchiVec;
									int idIdx = chengchitab->getTitleIndex("id");
									int earningsTypeIdx = chengchitab->getTitleIndex("收益类型");
									int earningsNumIdx = chengchitab->getTitleIndex("收益数量");
									int techanTypeIdx = chengchitab->getTitleIndex("特产类型");
									int techanNumIdx = chengchitab->getTitleIndex("特产数量");
									int cantonidIdx = chengchitab->getTitleIndex("州ID");
									int earningsnum = 0;
									int earningstype = 0;
									int techannum = 0;
									int techantype = 0;
									int chengchiNum = 0;
									if(cmmIt->second->getGroupPos() == JXJGroup_FirstLeader)
									{
										groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
										FenFengedCantonMap  * fenFengedCantonMap = (FenFengedCantonMap *)param;
										FenFengedCantonMap::iterator ffdcmitr = fenFengedCantonMap->begin();
										if (ffdcmitr == fenFengedCantonMap->end()) break;
										int cantonId = ffdcmitr->first;
										if(cantonId <= 0) break;
										chengchitab->queryRecords(cantonidIdx,crArgumentParser::appItoa(cantonId),chengchiVec);
										if(chengchiVec.empty()) break;;
										for (crTableIO::DataVec::iterator ccvitr = chengchiVec.begin();ccvitr != chengchiVec.end(); ++ccvitr)
										{
											chItr = chengchiMap->find(atoi((*ccvitr)[idIdx].c_str()));
											if (chItr != chengchiMap->end())
											{
												if (shiliid == chItr->second->getChengzhuShili())
												{
													earningsnum = 0;
													earningstype = 0;
													earningsnum = atoi((*ccvitr)[earningsNumIdx].c_str());
													earningstype = atoi((*ccvitr)[earningsTypeIdx].c_str());
													if (earningsnum >0 && earningstype > 0)
													{
														gccemitr = groupchengchiearningMap.find(earningstype);
														if (gccemitr == groupchengchiearningMap.end())
														{
															groupchengchiearningMap.insert(std::make_pair(earningstype,earningsnum));
														}
														else
														{
															groupchengchiearningMap[earningstype] = earningsnum + gccemitr->second;
														}
													}
													techannum = 0;
													techantype = 0;
													techannum = atoi((*ccvitr)[techanNumIdx].c_str());
													techantype = atoi((*ccvitr)[techanTypeIdx].c_str());
													gccemitr = groupchengchiearningMap.find(techantype);
													if (techantype > 0 && techannum > 0)
													{
														if (gccemitr == groupchengchiearningMap.end())
														{
															groupchengchiearningMap.insert(std::make_pair(techantype,techannum));
														}
														else
														{
															groupchengchiearningMap[techantype] = techannum >= (gccemitr->second) ? techannum:(gccemitr->second);
														}
													}
													++chengchiNum;
												}
											}
										}
										for (GroupChengChiEarningMap::iterator gccmitr = groupchengchiearningMap.begin(); gccmitr != groupchengchiearningMap.end(); ++gccmitr)
										{
											if (chengchiNum > 0 && gccmitr->first == T_Copper)
											{
												gccmitr->second = gccmitr->second / chengchiNum * 2;
											}
										}

									}
									else
									{
										bool isfenfenged =false;
										groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
										FenFengedChengChiMap * fenFengedChengChiMap = (FenFengedChengChiMap *)param;
										FenFengedChengChiMap::iterator ffccmItr = fenFengedChengChiMap->begin();
										for (;ffccmItr != fenFengedChengChiMap->end();++ffccmItr)
										{
											if(ffccmItr->second == cmmIt->first)
											{
												isfenfenged = true;
												break;
											}
										}
										if (isfenfenged)
										{
											if(chengchitab->queryOneRecord(0,crArgumentParser::appItoa(ffccmItr->first),record) >=0)
											{
												earningsnum = atoi(record[earningsNumIdx].c_str());
												earningstype = atoi(record[earningsTypeIdx].c_str());
												techannum = atoi(record[techanNumIdx].c_str());
												techantype = atoi(record[techanTypeIdx].c_str());
												if (earningsnum >0 && earningstype > 0)
												{
													groupchengchiearningMap[earningstype] = earningsnum;
												}
												if (techantype > 0 && techannum >0)
												{
													gccemitr = groupchengchiearningMap.find(techantype);
													if (gccemitr == groupchengchiearningMap.end())
													{
														groupchengchiearningMap[techantype] = techannum;
													}
													else
													{
														groupchengchiearningMap[techantype] = techannum+gccemitr->second;
													}
												}
											}
										}
									}
								}
								gccemitr = groupchengchiearningMap.begin();
								int techanType = 0;
								for (; gccemitr != groupchengchiearningMap.end(); ++ gccemitr)
								{
									techanType = gccemitr->first;
									switch (techanType)
									{
									case T_Food:
									case T_Horse:
									case T_Iron:
									case T_Wood:
										{
											ResourceChangeData resourceData(techanType,(int)(-gccemitr->second));
											m_this->doEvent(WCH_JXJDeductResource,MAKEINT64(&resourceData,NULL));
										}
										break;
									case T_Copper:
										{
											int copperCost = (int)(-gccemitr->second);
											m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&copperCost,NULL));
										}
										break;
									case T_Exploit:
										{
											int newExploit = (int)(gccemitr->second);
											crRole *mainRole = m_this->getMainRole();
											if (mainRole)
											{
												mainRole->doEvent(WCH_JXJRecvExploit,MAKEINT64(newExploit,NULL));
											}
										}
										break;
									case T_Giftgold:
										{
											std::string str = "领取俸禄城池特产";
											MoneyChangeData  moneydata;
											moneydata.first = (int)(gccemitr->second);
											moneydata.second = str;
											m_this->doEvent(WCH_JXJRecvGiftgold, MAKEINT64(&moneydata, NULL));
										}
										break;
									case T_Contribute:
										{
											int countrycontribute = (int)(gccemitr->second);
											m_this->doEvent(WCH_JXJRecvCountryContribute, MAKEINT64(countrycontribute, NULL));
										}
										break;
									case T_GroupContribute:
										{
											int groupcontribut = (int)(gccemitr->second);
											m_this->doEvent(WCH_JXJRecvGroupContribution,MAKEINT64(groupcontribut,NULL));
										}
										break;
									case T_Zhanquan:
										{
											int zhanquan = (int)(gccemitr->second);
											m_this->doEvent(WCH_JXJRecvZhengZanQuan,MAKEINT64(zhanquan,NULL));
										}
										break;
									case T_JiangHun:
										{
											int jianghun = (int)(gccemitr->second);
											m_this->doEvent(WCH_JXJRecvCardPieces,MAKEINT64(jianghun,NULL));
										}
										break;
									case T_AchievePoint:
										{
											int achievepoint = (int)(gccemitr->second);
											m_this->doEvent(WCH_JXJRecvAchievePoint,MAKEINT64(&achievepoint,NULL));
										}
										break;
									default:
										break;
									}
								
								}

								/*cmmIt->second->m_getEarningsYesterday = true;*/
								bool modify = true;
								groupchengchiearningMap.clear();
								groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
							} while (0);
						}
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				std::string logdata = playerData->getCharacterName() + "尝试领取军团收益 状态" + crArgumentParser::appItoa(suc);
				GameLogData gamelog(Log_GetGroupEarnings,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid ,&gamelog));
			}
		}

	}
}

/************************************************************************/
/*    crJXJDeductResourceMethod                                                                  */
/************************************************************************/

JXJ::crJXJDeductResourceMethod::crJXJDeductResourceMethod():
	m_this(NULL)
{
}

JXJ::crJXJDeductResourceMethod::crJXJDeductResourceMethod( const crJXJDeductResourceMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{
}


void JXJ::crJXJDeductResourceMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_resourceData = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_resourceData = (ResourceChangeData  *)(LOINT64(param64));
		}
		else
		{
			m_resourceData= NULL;
		}
		break;
	}
}

void JXJ::crJXJDeductResourceMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJDeductResourceMethod::operator()( crHandle &handle )
{
	bool success = false;
	if(m_this)
	{	
		// 0：扣除成功|增加成功
		//-1：扣除失败
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crData *data = m_this->getDataClass();
			void *param;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			int *resourceNum;
			int addcash = 0;
			int msg = 0;
			switch (m_resourceData->first)
			{
			case T_Food:
				msg = WCHDATA_JXJFood;
				break;
			case T_Wood:
				msg = WCHDATA_JXJWood;
				break;
			case T_Iron:
				msg = WCHDATA_JXJIron;
				break;
			case T_Horse:
				msg = WCHDATA_JXJHorse;
				break;
			case T_Giftgold:
				msg = WCHDATA_JXJGiftGold;
				break;
			default:
				break;
			}

			if (msg)
			{
				data->getParam(msg,param);
				resourceNum = (int *)param;
				if (m_resourceData->second > 0)
				{
					if (*resourceNum >= m_resourceData->second)
					{
						*resourceNum -= m_resourceData->second;
						success = true;
					}
				}
				else
				{
					addcash = - m_resourceData->second;
					INTLIMITADD(*resourceNum,addcash,INT_MAX);
					success = true;
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));

			if (success)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(m_resourceData->first);
				stream->_writeInt(*resourceNum);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvDeductResource,stream.get());
				if (playerData.valid())
				{
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				m_resourceData->first = 0;
			}
			else m_resourceData->first = -1;
		}
	}
}

/************************************************************************/
/*    crJXJRecvDeductResourceMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvDeductResourceMethod::crJXJRecvDeductResourceMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvDeductResourceMethod::crJXJRecvDeductResourceMethod( const crJXJRecvDeductResourceMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvDeductResourceMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvDeductResourceMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRecvDeductResourceMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int resourceType = m_stream->_readInt();
			int resourceNum = m_stream->_readInt();
			int msg = 0;
			crData *data = m_this->getDataClass();
			if (data)
			{
				switch (resourceType)
				{
				case T_Food:
						msg = WCHDATA_JXJFood;
					break;
				case T_Wood:
						msg = WCHDATA_JXJWood;
					break;
				case T_Iron:
						msg = WCHDATA_JXJIron;
					break;
				case T_Horse:
						msg = WCHDATA_JXJHorse;
					break;
				case T_Giftgold:
					msg = WCHDATA_JXJGiftGold;					
					break;
				default:
					break;
				}
				if (msg)
				{
					data->getParam(msg,param);
					int extra = resourceNum - *(int *)param;
					if(extra!=0)
					{
						data->inputParam(msg,&resourceNum);
						crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(resourceType,extra));
					}
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJFullOfCountryCanvasZhoujunMapMethod                                                                  */
/************************************************************************/

JXJ::crJXJFullOfCountryCanvasZhoujunMapMethod::crJXJFullOfCountryCanvasZhoujunMapMethod():
	m_this(NULL)
{
}

JXJ::crJXJFullOfCountryCanvasZhoujunMapMethod::crJXJFullOfCountryCanvasZhoujunMapMethod( const crJXJFullOfCountryCanvasZhoujunMapMethod & handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas)
{
}

void JXJ::crJXJFullOfCountryCanvasZhoujunMapMethod::inputParam( int i, void *param )
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

void JXJ::crJXJFullOfCountryCanvasZhoujunMapMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJFullOfCountryCanvasZhoujunMapMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	ref_ptr<crCanvasNode> countryCanvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
	if(playerGameData && netConductor && m_this && countryCanvas.get())
	{

		crData *canvasdata = countryCanvas->getDataClass();
		crData *data = playerGameData->getDataClass();
		void *param;
		if (!canvasdata || !data) return;

		data->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJCountryCanvasZhoujunMap,param);
		ZhoujunMap *zhoujunMap = (ZhoujunMap *)param;
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;
		if (chengchimap)
		{
			int i = 0;
			ZhoujunMap::iterator zItr;
			bool isOwn = false;
			zhoujunMap->clear();
			ref_ptr<crTableIO>  chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			crTableIO::StrVec record;
			int cantonIdx = chengchiTab->getTitleIndex("州ID");
			int cantonId;
			for (ChengchiMap::iterator it = chengchimap->begin(); it != chengchimap->end()&& i<20; ++ it)
			{
				isOwn = it->second->getChengzhuShili() == shiliid;
				if (chengchiTab.valid() && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(it->first),record) >= 0)
				{
					cantonId = atoi(record[cantonIdx].c_str());
					if (!cantonId) continue;
					zItr = zhoujunMap->find(cantonId);
					if (zItr != zhoujunMap->end()) 
					{
						zItr->second = zItr->second || isOwn;
					}
					else 
					{
						zhoujunMap->insert(ZhoujunMap::value_type(cantonId,isOwn));
						++ i;
					}
				}
			}
		}
		canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/************************************************************************/
/*    crJXJQueryMyCounrtyCantonChengChiINfoMethod                                                                  */
/************************************************************************/

JXJ::crJXJQueryMyCounrtyCantonChengChiINfoMethod::crJXJQueryMyCounrtyCantonChengChiINfoMethod():
	m_this(NULL)
{
}

JXJ::crJXJQueryMyCounrtyCantonChengChiINfoMethod::crJXJQueryMyCounrtyCantonChengChiINfoMethod( const crJXJQueryMyCounrtyCantonChengChiINfoMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_groupRadio(handle.m_groupRadio)
{
}

void JXJ::crJXJQueryMyCounrtyCantonChengChiINfoMethod::inputParam( int i, void *param )
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
		//m_this = (crWidgetNode *)param;
		break;
	}
}

void JXJ::crJXJQueryMyCounrtyCantonChengChiINfoMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupRadio = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJQueryMyCounrtyCantonChengChiINfoMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(canvas.valid())
		{
			ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_groupRadio));
			crData * canvasData = canvas->getDataClass();
			if (canvasData && groupradio.valid())
			{
				int idx = groupradio->getSelect();
				void *param;
				canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
				canvasData->getParam(WCHDATA_JXJCountryCanvasZhoujunMap,param);
				ZhoujunMap * zhoujunMap = (ZhoujunMap *)param;
				ZhoujunMap::iterator zItr = zhoujunMap->begin();
				for (unsigned char i = 0; zItr != zhoujunMap->end(); ++ i,++ zItr)
				{
					if (i == idx) break;
				}
				if (zItr != zhoujunMap->end() && zItr->second)
				{
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(zItr->first); 

					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCounrtyCantonChengChiINfo,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
				canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod::
	crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod( const crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod& handle ):
	crMethod(handle),
	m_myCountryCanvas(handle.m_myCountryCanvas),
	m_myCountry_Apply(handle.m_myCountry_Apply),
	m_myCountry_Cancle(handle.m_myCountry_Cancle),
	m_myCountry_Apply_sw(handle.m_myCountry_Apply_sw),
	m_myCountry_Cancle_sw(handle.m_myCountry_Cancle_sw)
{
}

void JXJ::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_myCountryCanvas = str;
		break;
	case 1:
		m_myCountry_Apply = str;
		break;
	case 2:
		m_myCountry_Cancle = str;
		break;
	case 3:
		m_myCountry_Apply_sw = str;
		break;
	case 4:
		m_myCountry_Cancle_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvQueryMyCounrtyCantonChengChiINfoMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			int cantonId = m_stream->_readInt();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				if (shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(256);

						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
						do 
						{
							gsBrainData->getParam(WCHDATA_JXJIsChengChiShiliSyn,param);
							bool isSyn = *(bool *)param;
							//if (!isSyn)
							{
								crServerBrainHandle::getInstance()->doEvent(WCH_JXJSYNChengChiToCanton,NULL	);
								isSyn = true;
								gsBrainData->inputParam(WCHDATA_JXJIsChengChiShiliSyn,&isSyn);
							}
							gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
							crJXJShili *shili = (crJXJShili *)param;
							if (!cantonId || !shili) break;
							if (shili->getDataClass() == NULL) break;

							crData *shilidata = shili->getDataClass();	
							if (!shilidata) break;
							CRCore::ScopedLock<crData> lock(*shilidata);
							gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
							ConsortiaMap *consortiamap = (ConsortiaMap *)param;
							ConsortiaMap::iterator cmItr ;

							shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
							CantonMap * cantonMap = (CantonMap *)param;
							CantonMap::iterator it = cantonMap->find(cantonId);

							if (it == cantonMap->end()) break;
							if (it->second.valid() == false) break;
							int groupid = it->second->getConsortiaId();

							cmItr = consortiamap->find(groupid);
							ref_ptr<crJXJConsortia>consortia;
							if(cmItr != consortiamap->end())
								consortia = cmItr->second;

							if (!consortia.valid())
								stream->_writeBool(false);
							else 
							{
								stream->_writeBool(true);
								std::string groupName;
								consortia->getName(groupName);
								stream->_writeString(groupName);
								std::string leaderName;
								consortia->getLeaderName(0,leaderName);
								stream->_writeString(leaderName);

								bool myCanton = false;
								data->getParam(WCHDATA_JXJConsortiaID,param);
								int mygroupid = *( int *)param;
								ref_ptr<crData>groupData;
								cmItr = consortiamap->find(mygroupid);
								if (cmItr != consortiamap->end() && cmItr->second.valid())
								{
									groupData = cmItr->second->getDataClass();
								}
								if(groupData.valid())
								{
									groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
									ApplyCantonMap * applyCantonMap = (ApplyCantonMap *)param;
									if (applyCantonMap->empty() == false)
									{
										ApplyCantonMap::iterator acmItr = applyCantonMap->begin();
										for (; acmItr != applyCantonMap->end(); ++ acmItr)
										{
											if (cantonId == acmItr->first)
											{
												myCanton = true;
											}
										}
									}
								}
								stream->_writeInt(myCanton);
							}
							//ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
							//ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.begin();

							//stream->_writeUChar(chengChiEarningsMap.size());
							//bool hasInfo;
							//for (; ccemItr != chengChiEarningsMap.end(); ++ ccemItr)
							//{
							//	hasInfo = false;
							//	do 
							//	{
							//		if (ccemItr->second.valid() == false) break;
							//		if (ccemItr->second->getChengChiId() == 0) break;
							//		cmItr = consortiamap->find(ccemItr->second->getGroupId());
							//		if (cmItr == consortiamap->end()) break;
							//		if (cmItr->second.valid() == false) break;
							//		stream->_writeBool(true);
							//		stream->_writeInt(ccemItr->second->getChengChiId());
							//		std::string groupName;
							//		cmItr->second->getName(groupName);
							//		stream->_writeString(groupName);
							//		std::string leaderName;
							//		cmItr->second->getLeaderName(0,leaderName);
							//		stream->_writeString(leaderName);
							//		hasInfo = true;
							//	} while (0);
							//	if (!hasInfo) stream->_writeBool(false);
							//}
						} while (0);

						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyCounrtyCantonChengChiINfo,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_myCountryCanvas);
			ref_ptr<crCanvasNode> Enfeoffcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_StateEnfeoff");
			crButtonWidgetNode* applyCanton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_myCountry_Apply));
			crButtonWidgetNode* cancleCanton = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_myCountry_Cancle));
			ref_ptr<crMultiSwitch> applyCanton_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_myCountry_Apply_sw));
			ref_ptr<crMultiSwitch> cancleCanton_sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_myCountry_Cancle_sw));
			do 
			{
				void *param;
				if (!canvas) break;
				crData * canvasData = canvas->getDataClass();
				if (!canvasData) break;
				std::string zhoumu = "";
				std::string zhenShouGroup = "";
				bool myCanton = false;
				canvasData->inputParam(WCHDATA_JXJCountryCanvasZhenShouGroup,&zhenShouGroup);
				canvasData->inputParam(WCHDATA_JXJCountryCanvasZhenShouGroupLeader,&zhoumu);
				if(m_stream->eof()) return;
				bool hasGroup = m_stream->_readBool();
				canvasData->inputParam(WCHDATA_JXJIsSelectCabtonFenFenged,&hasGroup);
				if (hasGroup)
				{
					zhenShouGroup = m_stream->_readString();
					zhoumu = m_stream->_readString();
					myCanton = m_stream->_readBool();
				}
				PermissionCheck permissionCheck;
				canvasData->getParam(WCHDATA_JXJCountryCanvasGroupPos,param);
				unsigned char groupPos = *(unsigned char *) param;
				if (!permissionCheck(groupPos,JXJGroup_ApplyStationCanton))
				{
					myCanton = false;
				}
				if (myCanton)
				{
					applyCanton->setVisiable(false);
					cancleCanton->setVisiable(true);
					cancleCanton->setEnable(true);
					cancleCanton_sw->setActiveSwitchSet(0);
				}
				else
				{
					cancleCanton->setVisiable(false);
					cancleCanton_sw->setActiveSwitchSet(3);
				}
				canvasData->inputParam(WCHDATA_JXJCountryCanvasZhenShouGroup,&zhenShouGroup);
				canvasData->inputParam(WCHDATA_JXJCountryCanvasZhenShouGroupLeader,&zhoumu);
				//void * param;
				//canvasData->excHandle(MAKEINT64(WCH_LockData,1));
				//canvasData->getParam(WCHDATA_JXJCountryCanvasChengchiInfoMap,param);
				//ChengchiInfoMap *chengchiInfoMap = (ChengchiInfoMap *)param;
				//unsigned char size = m_stream->_readUChar();
				//for (unsigned char i = 0; i < size; ++ i)
				//{
				//	if (m_stream->_readBool())
				//	{
				//		CRCore::ref_ptr<ChengchiInfo> info = new ChengchiInfo;
				//		info->m_id = m_stream->_readInt();
				//		info->m_groupName = m_stream->_readString();
				//		info->m_groupLeader = m_stream->_readString();
				//		chengchiInfoMap->insert(ChengchiInfoMap::value_type(info->m_id,info));
				//	}
				//}
				//canvasData->excHandle(MAKEINT64(WCH_LockData,0));
				canvas->doEvent(WCH_UI_UpdateData);
				if (Enfeoffcanvas.valid())
				{
					Enfeoffcanvas->doEvent(WCH_UI_UpdateData);
				}
			} while (0);
		}
	}
}

/************************************************************************/
/*    crJXJMyGroupCanvasDongHuaUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJMyGroupCanvasDongHuaUpdateMethod::crJXJMyGroupCanvasDongHuaUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJMyGroupCanvasDongHuaUpdateMethod::crJXJMyGroupCanvasDongHuaUpdateMethod( const crJXJMyGroupCanvasDongHuaUpdateMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJMyGroupCanvasDongHuaUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJMyGroupCanvasDongHuaUpdateMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJMyGroupCanvasDongHuaUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> parrentCanvas = m_this->getParentCanvas();
		crData * canvasData = parrentCanvas->getDataClass();
		bool hasNew = true;
		canvasData->inputParam(WCHDATA_JXJConsortiaHadOpenShenHe,&hasNew);
	}
}

/************************************************************************/
/*    crJXJClearGroupMemberRankMethod                                                                  */
/************************************************************************/

JXJ::crJXJClearGroupMemberRankMethod::crJXJClearGroupMemberRankMethod():
	m_this(NULL)
{
}

JXJ::crJXJClearGroupMemberRankMethod::crJXJClearGroupMemberRankMethod( const crJXJClearGroupMemberRankMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJClearGroupMemberRankMethod::inputParam( int i, void *param )
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

void JXJ::crJXJClearGroupMemberRankMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJClearGroupMemberRankMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * gsBrainData = m_this->getDataClass();
		if (gsBrainData)
		{
			void * param;
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			ref_ptr<crJXJConsortiaData> groupData;
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator it = consortiamap->begin();
			for (; it != consortiamap->end(); ++ it)
			{
				if (it->second.valid())
				{
					groupData = dynamic_cast<crJXJConsortiaData * >(it->second->getDataClass());
					if (groupData.valid())
						groupData->clearRankCache();
				}
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJRecvGroupChatMsgMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvChatMsgMethod::crJXJRecvChatMsgMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvChatMsgMethod::crJXJRecvChatMsgMethod( const crJXJRecvChatMsgMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvChatMsgMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvChatMsgMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRecvChatMsgMethod::operator()( crHandle &handle )
{
	if (m_stream.valid() && m_this)
	{
		if (m_netType == GameClient_Game)
		{
			unsigned char _type = m_stream->_readUChar();
			JXJGroupChatMsgManager creator;
			ref_ptr<JXJGroupChatMsgBase > msgPtr = creator(_type);
			if (msgPtr.valid())
			{
				msgPtr->readContent(m_stream);
				std::string chatText;
				msgPtr->getCompleteString(chatText);

				ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
				record->setChatChannelName("[军团]");
				record->setChatText(chatText);
				crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
			}
		}
	}
}

void JXJ::JXJGroupChatMsgBase::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	m_msgId = stream->_readInt();
}

void JXJ::JXJGroupChatMsgBase::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	stream->_writeUChar(m_infoType);
	stream->_writeInt(m_msgId);
}

CRCore::crHandle * JXJ::JXJGroupChatMsgBase::getNoticeHandle()
{
	CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
	if (noticeHandle)
	{
		int mode = 2;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&m_msgId);
	}
	return noticeHandle;
}

void JXJ::JXJGroupExpChatMsg::getCompleteString( std::string & outString)
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&crArgumentParser::appItoa(m_exp));
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";
}

void JXJ::JXJGroupExpChatMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_exp = stream->_readInt();
}

void JXJ::JXJGroupExpChatMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeInt(m_exp);
}

JXJ::JXJGroupExpChatMsg::JXJGroupExpChatMsg()
{
	m_infoType = JXJGroupChatMsgType_exp;
	m_msgId = 3074;
}

void JXJ::JXJGroupAddNewmemberMsg::getCompleteString( std::string & outString)
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&m_name);
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";

}

void JXJ::JXJGroupAddNewmemberMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_name = stream->_readString();
}

void JXJ::JXJGroupAddNewmemberMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_name);
}

JXJ::JXJGroupAddNewmemberMsg::JXJGroupAddNewmemberMsg()
{
	m_infoType = JXJGroupChatMsgType_addNewMember; 
	m_msgId = 3073;
}

/************************************************************************/
/*    crJXJPutInChatMsgQueueMethod                                                                  */
/************************************************************************/

JXJ::crJXJPutIntoGroupChatMsgQueueMethod::crJXJPutIntoGroupChatMsgQueueMethod():
	m_this(NULL)
{
}


JXJ::crJXJPutIntoGroupChatMsgQueueMethod::crJXJPutIntoGroupChatMsgQueueMethod( const crJXJPutIntoGroupChatMsgQueueMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJPutIntoGroupChatMsgQueueMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_msgPtr = NULL;
			m_groupId = 0;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_msgPtr = (JXJGroupChatMsgBase *)LOINT64(param64);
			m_groupId = HIINT64(param64);
		}
		else
		{
			m_msgPtr = NULL;
			m_groupId = 0;
		}
		break;
	}
}

void JXJ::crJXJPutIntoGroupChatMsgQueueMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJPutIntoGroupChatMsgQueueMethod::operator()( crHandle &handle )
{
	if (m_msgPtr.valid() && m_this)
	{
		crData * gsBrainData = m_this->getDataClass();
		if (gsBrainData)
		{
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			
			void *param;
			gsBrainData->getParam(WCHDATA_JXJGroupChatVec,param);
			GroupChatMsgVec * msgVec = (GroupChatMsgVec *)param;
			msgVec->push_back(std::make_pair(m_groupId,m_msgPtr));

			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}

	}
}
/************************************************************************/
/*    crJXJGroupChatMsgQueueDisposeMethod                                                                  */
/************************************************************************/

JXJ::crJXJGroupChatMsgQueueDisposeMethod::crJXJGroupChatMsgQueueDisposeMethod():
	m_this(NULL)
{
}

JXJ::crJXJGroupChatMsgQueueDisposeMethod::crJXJGroupChatMsgQueueDisposeMethod( const crJXJGroupChatMsgQueueDisposeMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJGroupChatMsgQueueDisposeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGroupChatMsgQueueDisposeMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJGroupChatMsgQueueDisposeMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData * gsBrainData = m_this->getDataClass();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if (gsBrainData && gameServer)
		{
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			
			void *param;

			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ConsortiaMap::iterator cmItr ;

			gsBrainData->getParam(WCHDATA_JXJGroupChatVec,param);
			GroupChatMsgVec * msgVec = (GroupChatMsgVec *)param;
			int groupid = 0;
			JXJGroupChatMsgBase * msgPtr = NULL;
			ConsortiaMemberMap * memberMap = NULL;
			ConsortiaMemberMap::iterator cmmItr ;
			ref_ptr<crData>groupData;

			crPlayerDataEventPacket packet;
			ref_ptr<crGameServerPlayerData> playerData; 
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(16);

			GroupChatMsgVec::iterator gcmItr = msgVec->begin();
			for (; gcmItr != msgVec->end(); ++ gcmItr)
			{
				groupid = gcmItr->first;
				msgPtr = gcmItr->second.get();
				groupData = NULL;
				cmItr = consortiamap->find(groupid);
				if (cmItr != consortiamap->end() && cmItr->second.valid())
				{
					groupData = cmItr->second->getDataClass();
				}
				if(groupData.valid())
				{
					groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
					memberMap = (ConsortiaMemberMap *)param;
					cmmItr = memberMap->begin();
					for (; cmmItr != memberMap->end(); ++ cmmItr)
					{
						playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(cmmItr->first));
						if(playerData.valid())
						{
							stream->resetBuf();
							gcmItr->second->writeContent(stream);
							crPlayerDataEventPacket::buildReplyPacket(packet,cmmItr->first,WCH_JXJRecvChatMsg,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
			}
			msgVec->clear();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJGroupFundsUpgradeMethod                                                                  */
/************************************************************************/

JXJ::crJXJGroupFundsUpgradeMethod::crJXJGroupFundsUpgradeMethod():
	m_this(NULL),
	m_funds(0),
	m_groupId(0)
{
}

JXJ::crJXJGroupFundsUpgradeMethod::crJXJGroupFundsUpgradeMethod( const crJXJGroupFundsUpgradeMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJGroupFundsUpgradeMethod::inputParam( int i, void *param )
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
			m_funds = (int *)LOINT64(param64);
			m_groupId = HIINT64(param64);
		}
		else
		{
			m_funds = NULL;
			m_groupId = 0;
		}
		break;
	}
}

void JXJ::crJXJGroupFundsUpgradeMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJGroupFundsUpgradeMethod::operator()( crHandle &handle )
{
	void *param;
	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	ref_ptr<crData>consortiaData;
	gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
	ConsortiaMap *consortiamap = (ConsortiaMap *)param;
	ConsortiaMap::iterator itr = consortiamap->find(m_groupId);;
	int addval = 0;
	if (itr != consortiamap->end())
	{
		consortiaData = itr->second->getDataClass();
	}
	if (consortiaData.valid())
	{
		consortiaData->getParam(WCHDATA_JXJConsortialv,param);
		int groupLevel = *(unsigned char *)param;
		ref_ptr<crTableIO>  groupUpdateTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupUpgradeTab);
		crTableIO::StrVec record;

		consortiaData->getParam(WCHDATA_JXJConsortiaFunds,param);
		int funds = *(int *)param;
		if (*m_funds < 0 && groupUpdateTab.valid() && groupUpdateTab->queryOneRecord(0,crArgumentParser::appItoa(groupLevel - 1),record) >= 0)
		{
			int upLevelFundsIdx = groupUpdateTab->getTitleIndex("升级资金");
			if (funds + *m_funds < atoi(record[upLevelFundsIdx].c_str()))
			{
				//exp = atoi(record[upLevelExpIdx].c_str());
				*m_funds = -1;
			}
			else 
			{
				funds += *m_funds;
				*m_funds = 0;
			}
		}
		if (*m_funds > 0)
		{
			funds += *m_funds;
			/*unsigned char maxGroupLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaMaxLevel,0).c_str());
			while (groupLevel < maxGroupLevel)
			{
			if (groupUpdateTab.valid() && groupUpdateTab->queryOneRecord(0,crArgumentParser::appItoa(groupLevel),record) >= 0)
			{
			int upLevelFundsIdx = groupUpdateTab->getTitleIndex("升级资金");
			if (funds >= atoi(record[upLevelFundsIdx].c_str()))
			{
			++ groupLevel ;
			consortiaData->inputParam(WCHDATA_JXJConsortialv,&groupLevel);
			}
			else break;
			}
			else break;
			}*/
			*m_funds = 0;
		}
		consortiaData->inputParam(WCHDATA_JXJConsortiaFunds,&funds);
		itr->second->updateToRank(gsBrainData);
		bool modify = true;
		consortiaData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
}

/************************************************************************/
/*    crJXJClientRecvCreateGroupMethod                                                                  */
/************************************************************************/

JXJ::crJXJClientRecvCreateGroupMethod::crJXJClientRecvCreateGroupMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJClientRecvCreateGroupMethod::crJXJClientRecvCreateGroupMethod( const crJXJClientRecvCreateGroupMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJClientRecvCreateGroupMethod::inputParam( int i, void *param )
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

void JXJ::crJXJClientRecvCreateGroupMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJClientRecvCreateGroupMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid() && m_netType == GameClient_Game)
	{
		if (m_this->getDataClass() == NULL) return;
		void *param;
		crData *data = m_this->getDataClass();
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		std::string playerName = m_this->getCharacterName();

		int groupid = m_stream->_readInt();
		std::string groupName = m_stream->_readString();

		data->inputParam(WCHDATA_JXJConsortiaID,param);

		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		std::string shiliName[] = {"魏国","蜀国","吴国"};
		std::string outString ;
		int mode = 2;
		int texid = 2000;
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&shiliName[shiliid - c_startShiliID]);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&playerName);
		noticeHandle->inputParam(WCHDATA_NoticeParam3,&groupName);
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);

		ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
		record->setChatChannelName("[国家]");
		record->setChatText(outString);
		crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
	}
}

/************************************************************************/
/*     crJXJUIGroupListNoticeUpdateMethod                                                                  */
/************************************************************************/

crJXJUIGroupListNoticeUpdateMethod ::crJXJUIGroupListNoticeUpdateMethod ():
	m_this(NULL)
{
}
crJXJUIGroupListNoticeUpdateMethod ::crJXJUIGroupListNoticeUpdateMethod (const crJXJUIGroupListNoticeUpdateMethod & handle):
	crMethod(handle),
	m_grouplist(handle.m_grouplist),
	m_recuitNotice(handle.m_recuitNotice) ,
	m_canvas(handle.m_canvas)
{
}
void crJXJUIGroupListNoticeUpdateMethod ::inputParam(int i, void *param)
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
void crJXJUIGroupListNoticeUpdateMethod ::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_grouplist = str;
		break;
	case 1:
		m_recuitNotice = str;
		break;
	case 2:
		m_canvas = str;
		break;
	}
}

void crJXJUIGroupListNoticeUpdateMethod ::operator()(crHandle &handle)
{	
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (data && canvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> recuitNotice = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_recuitNotice));
			ref_ptr<crTableWidgetNode> grouplist = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_grouplist));

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char perpagenum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGroupPerPageNum,viplv).c_str());
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJGroupInfoVec,param);
				GroupInfoVec *groupinfovec = (GroupInfoVec *)param;
				canvasdata->getParam(WCHDATA_JXJMyApplyGroupInfoSet,param);
				MyGroupApplySet * myGroupApplySet = ( MyGroupApplySet *)param;

				int sel = grouplist->getSelect();
				int row = 0;
				if (!groupinfovec->empty())
				{
					if (recuitNotice.valid())
					{
						recuitNotice->clearString();
					}
					for( GroupInfoVec::iterator itr = groupinfovec->begin();
						itr != groupinfovec->end()&&row<perpagenum;
						++itr,row++ )
					{
						if (row == sel)
						{
							if (recuitNotice.valid())
							{
								recuitNotice->setString((*itr)->getNotice());
							}
						}
					}
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJJunTuanFuBenSettleMethod                                                                  */
/************************************************************************/
JXJ::crJXJJunTuanFuBenSettleMethod::crJXJJunTuanFuBenSettleMethod():
	m_this(NULL),
	m_fubenPlayerInfo(NULL),
	m_circle(0.0f)
	//m_stream(NULL),
	//m_fuBenId(0)
{

}

JXJ::crJXJJunTuanFuBenSettleMethod::crJXJJunTuanFuBenSettleMethod( const crJXJJunTuanFuBenSettleMethod & handle ):
	crMethod(handle),
	m_fubenPlayerInfo(NULL),
	m_circle(handle.m_circle)
	//m_stream(handle.m_stream),
	//m_fuBenId(handle.m_fuBenId)
{
}

void JXJ::crJXJJunTuanFuBenSettleMethod::inputParam( int i, void *param )
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
			m_fubenPlayerInfo = (FuBenPlayerInfo *)LOINT64(param64);
			m_circle = *(float *)HIINT64(param64);
			//m_fuBenId = LOINT64(param64);
			//m_stream = (CRCore::crStreamBuf *)HIINT64(param64);
		}
		else
		{
			m_fubenPlayerInfo = NULL;
			m_circle = 0.0f;
			//m_fuBenId = 0;
			//m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJJunTuanFuBenSettleMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJJunTuanFuBenSettleMethod::operator()( crHandle &handle )
{
	crPlayerDataEventPacket packet;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	if (m_this && /*m_stream.valid() && m_fuBenId && */gameServer && m_this->getDataClass())
	{
		crData *gsBrainData = m_this->getDataClass();
		//bool win = m_stream->_readBool();
		//unsigned short mapsize = m_stream->_readUShort();
		int playerid = 0;
		//unsigned char reliveCount = 0;

		crTableIO::StrVec record;
		ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunTuanFuBenTab	);

		if (fubentab.valid() && 
			fubentab->queryOneRecord(0,crArgumentParser::appItoa(m_fubenPlayerInfo->fubenID),1,crArgumentParser::appItoa(0),record)>=0)
		{
			//int activeValIdx = fubentab->getTitleIndex("活跃值");
			int expIdx = fubentab->getTitleIndex("经验");
			//int zhanGongIdx = fubentab->getTitleIndex("战功");
			//int exploitIdx = fubentab->getTitleIndex("功勋");
			int junTuanFundsIdx = fubentab->getTitleIndex("军团资金");
			//int countryContributeIdx = fubentab->getTitleIndex("国家贡献");
			int groupContributeIdx = fubentab->getTitleIndex("军团贡献");
			CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(9);
			std::map< int, FubenKillAndStarInfo >::iterator itr_player = m_fubenPlayerInfo->playerMap->begin();
			//for ( int i = 0; i<mapsize; i++ )
			for (;itr_player != m_fubenPlayerInfo->playerMap->end(); ++ itr_player)
			{
				if (!m_fubenPlayerInfo->win) continue;				
				stream->resetBuf();
				playerid = itr_player->first;
				//playerid = m_stream->_readInt();//4
				//reliveCount = m_stream->_readUChar();//1
				stream->_writeInt(m_fubenPlayerInfo->fubenID);
				stream->_writeFloat(m_circle);

				ref_ptr<crGameServerPlayerData> playerData 
					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
				{
					crPlayerGameData *playerGameData = playerData->getPlayerGameData();
					crData * data = playerGameData->getDataClass();
					void *param;

					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJHadWonGroupFuBenSet,param);
					HadWonGroupFuBenSet * hadWonGroupFuBenSet = (HadWonGroupFuBenSet *)param;
					hadWonGroupFuBenSet->insert(m_fubenPlayerInfo->fubenID);
					data->excHandle(MAKEINT64(WCH_LockData,0));
					data->getParam(WCHDATA_JXJConsortiaID,param);
					int mygroupid = *(int *)param;
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
					ConsortiaMap *consortiamap = (ConsortiaMap *)param;
					ref_ptr<crData> groupData;
					ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
					if (cmItr != consortiamap->end())
					{
						groupData = cmItr->second->getDataClass();
					}
					if(groupData.valid())
					{
						groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
						ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
						ConsortiaMemberMap::iterator cmmIt = memberMap->find(playerid);
						if (cmmIt != memberMap->end() && cmmIt->second.valid())
						{
							cmmIt->second->clearJunTuanFuBenInfo();
							cmmIt->second->m_curFuBen = m_fubenPlayerInfo->fubenID;
							data->getParam(WCHDATA_JXJHadWonJunTuanFBT,param);
							bool hadwontoday = *(bool *)param;
							if (!hadwontoday)
							{
								//cmmIt->second->m_curFuBenAchievement = atoi(record[zhanGongIdx].c_str());
								//cmmIt->second->m_curFuBenActiveVal = atoi(record[activeValIdx].c_str());
								//cmmIt->second->m_curFuBenCountryContribute = atoi(record[countryContributeIdx].c_str());
								cmmIt->second->m_curFuBenExp = atoi(record[expIdx].c_str());
								//cmmIt->second->m_curFuBenExploit = atoi(record[exploitIdx].c_str());
								cmmIt->second->m_curFuBenGroupFunds = atoi(record[junTuanFundsIdx].c_str());
								cmmIt->second->m_curFuBenGroupContribute = atoi(record[groupContributeIdx].c_str());
								cmmIt->second->m_fubencircle = m_circle;
								hadwontoday = true;
								data->inputParam(WCHDATA_JXJHadWonJunTuanFBT,&hadwontoday);
							}
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJJunTuanFuBenSettleClient,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJJunTuanFuBenSettleClientMethod                                                                  */
/************************************************************************/
JXJ::crJXJJunTuanFuBenSettleClientMethod::crJXJJunTuanFuBenSettleClientMethod():
	m_this(NULL),
	m_netType(GameClient_Game),
	m_stream(NULL)
{
}

JXJ::crJXJJunTuanFuBenSettleClientMethod::crJXJJunTuanFuBenSettleClientMethod( const crJXJJunTuanFuBenSettleClientMethod& handle ):
	m_canvas(handle.m_canvas)
{

}

void JXJ::crJXJJunTuanFuBenSettleClientMethod::inputParam( int i, void *param )
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

void JXJ::crJXJJunTuanFuBenSettleClientMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJJunTuanFuBenSettleClientMethod::operator()( crHandle &handle )
{
	if (m_stream.valid() && m_this)
	{
		int fuBenId = m_stream->_readInt();
		float fubencircle = m_stream->_readFloat();

		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			crData * canvasData = canvas->getDataClass();
			if (canvasData)
			{
				unsigned char times = 0;
				canvasData->inputParam(WCHDATA_JXJJunTuanFuBenId	,&fuBenId);

				crTableIO::StrVec record;
				ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunTuanFuBenTab	);

				if (fubentab.valid() && 
					fubentab->queryOneRecord(0,crArgumentParser::appItoa(fuBenId),1,crArgumentParser::appItoa(0),record)>=0)
				{
					//int activeValIdx = fubentab->getTitleIndex("活跃值");
					int expIdx = fubentab->getTitleIndex("经验");
					//int zhanGongIdx = fubentab->getTitleIndex("战功");
					//int exploitIdx = fubentab->getTitleIndex("功勋");
					int junTuanFundsIdx = fubentab->getTitleIndex("军团资金");
					//int countryContributeIdx = fubentab->getTitleIndex("国家贡献");
					int groupContributeIdx = fubentab->getTitleIndex("军团贡献");

					//int achivement = atoi(record[zhanGongIdx].c_str());
					//int activeVal = atoi(record[activeValIdx].c_str());
					//int countryContribute = atoi(record[countryContributeIdx].c_str());
					int exp = atoi(record[expIdx].c_str());
					//int exploit  = atoi(record[exploitIdx].c_str());
					int groupFunds = atoi(record[junTuanFundsIdx].c_str());
					int groupContribute = atoi(record[groupContributeIdx].c_str());

					crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
					if (playerGameData)
					{
						crData *data = playerGameData->getDataClass();
						if (data)
						{
							void *param;
							data->getParam(WCHDATA_JXJHadWonJunTuanFBT,param);
							bool hadwontoday = *(bool *)param;
							if (hadwontoday)
							{
								exp = 0;
								groupFunds = 0;
								groupContribute = 0;
							}
						}
					}
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenCircle,&fubencircle);
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenTotalTimes,&times);
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenId	,&fuBenId);
					//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenActiveVal,&activeVal);
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenExp	,&exp);
					//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenAchivement	,&achivement);
					//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenExploit		,&exploit);
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenGroupFunds	,&groupFunds);
					//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenCountryContribute,&countryContribute);
					canvasData->inputParam(WCHDATA_JXJJunTuanFuBenGroupContribute,&groupContribute);		
					canvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJJunTuanFuBenSettleCanvasUpdateMethod                                                                  */
/************************************************************************/
JXJ::crJXJJunTuanFuBenSettleCanvasUpdateMethod::crJXJJunTuanFuBenSettleCanvasUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJJunTuanFuBenSettleCanvasUpdateMethod::crJXJJunTuanFuBenSettleCanvasUpdateMethod( const crJXJJunTuanFuBenSettleCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_input(handle.m_input),
	m_btn(handle.m_btn),
	m_btn_sw(handle.m_btn_sw),
	m_input2(handle.m_input2),
	m_input3(handle.m_input3),
	m_star_sw(handle.m_star_sw)
{
	for (unsigned char i = 0; i < 3; ++ i)
	{
		m_icon[i] = handle.m_icon[i];
		m_icon_image[i] = handle.m_icon_image[i];
	}
}

void JXJ::crJXJJunTuanFuBenSettleCanvasUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJJunTuanFuBenSettleCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_input2 = str;
		break;
	case 2:
		m_input3 = str;
		break;
	case 3:
		m_btn = str;
		break;
	case 4:
		m_btn_sw = str;
		break;
	case 5:
	case 6:
	case 7:
		m_icon[i - 5] = str;
		break;
	case 8:
	case 9:
	case 10:
		m_icon_image[i - 8]= str;
		break;
	case 11:
		m_star_sw = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJJunTuanFuBenSettleCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerGameData && m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();

		crData * canvasData = m_this->getDataClass();

		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input));
		ref_ptr<crStaticTextWidgetNode> input2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2));
		ref_ptr<crStaticTextWidgetNode> input3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input3));
		ref_ptr<crButtonWidgetNode>  button = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btn));
		//ref_ptr<crMultiSwitch>  button_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_btn_sw));
		ref_ptr<crMultiSwitch>  star_sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_star_sw));

		ref_ptr<crImageBoxWidgetNode>  icon[3] = {NULL};
		for (unsigned char i = 0; i < 3; ++ i)
		{
			icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
			if (icon[i].valid())
			{
				icon[i]->setImageName(m_icon_image[i]);
			}
		}
		if (canvasData && input.valid())
		{
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned char totalTimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenDoubleTimes	,viplv).c_str());

			void *param;
			canvasData->getParam(WCHDATA_JXJJunTuanFuBenTotalTimes,param);
			unsigned char times = *(unsigned char *)param;

			data->getParam(WCHDATA_JXJHadWonJunTuanFBT,param);
			bool hadwontoday = *(bool *)param;
			if (times >= totalTimes || hadwontoday)
			{
				if (button.valid()) button->setEnable(false);
				//if (button_sw.valid()) button_sw->setActiveSwitchSet(3);
			}
			else
			{
				if (button.valid()) button->setEnable(true);
			}

			if (star_sw.valid() && times >=0 && times < 5)
			{
				star_sw->setActiveSwitchSet(times);
			}
			//if (input3.valid())
			//{
			//	input3->setString(crArgumentParser::appItoa(times) + " / " + crArgumentParser::appItoa(totalTimes));
			//}
			//if (input2.valid())
			//{
			//	CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			//	if (noticeHandle)
			//	{
			//		int msgId = 4068;
			//		int mode = 2;
			//		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			//		noticeHandle->inputParam(WCHDATA_NoticeTextID,&msgId);
			//		std::string text;
			//		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&text);
			//		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			//		input2->setString(text);
			//	}
			//}
			/*canvasData->getParam(WCHDATA_JXJJunTuanFuBenActiveVal,param);
			int activeVal = *(int *)param;*/
			int exp = 0;
			int junTuanExp = 0;
			int groupContribute = 0;
			if (!hadwontoday)
			{
				canvasData->getParam(WCHDATA_JXJJunTuanFuBenExp	,param);
				exp = *(int *)param;
				//canvasData->getParam(WCHDATA_JXJJunTuanFuBenAchivement	,param);
				//int zhanGong = *(int *)param;
				/*canvasData->getParam(WCHDATA_JXJJunTuanFuBenExploit,param);
				int exploit = *(int *)param;*/
				canvasData->getParam(WCHDATA_JXJJunTuanFuBenGroupFunds	,param);
				junTuanExp  = *(int *)param;
				/*canvasData->getParam(WCHDATA_JXJJunTuanFuBenCountryContribute,param);
				int countryContribute  = *(int *)param;*/
				canvasData->getParam(WCHDATA_JXJJunTuanFuBenGroupContribute,param);
				groupContribute  = *(int *)param;
			}

			//std::ostringstream text;
			//text << "经验：" << exp  << " "
			//	//<< "战功： " << zhanGong  << " "
			//	<< "军团资金：" << junTuanExp  << " "
			//	<< "军团贡献：" << groupContribute ;
			float xishu = 0.0f;
			ref_ptr<crTableIO>  CoefficientTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCoefficientTab);
			crTableIO::StrVec corecord;
			if (CoefficientTab.valid())
			{
				crRole *mainRole = playerGameData->getMainRole();
				crData *roleData = mainRole->getDataClass();
				roleData->getParam(WCHDATA_Level,param);
				unsigned char rolelv = *(unsigned char *)param;
				int LvIdx = CoefficientTab->getTitleIndex("Level");
				int fubenxishuIdx = CoefficientTab->getTitleIndex("军团副本系数");
				if (CoefficientTab->queryOneRecord(LvIdx,crArgumentParser::appItoa(rolelv),corecord) >= 0)
				{
					xishu = atof(corecord[fubenxishuIdx].c_str());
				}
			}
			float circlexishu = 1.0f;
			if (data)
			{
				void *param;
				canvasData->getParam(WCHDATA_JXJJunTuanFuBenCircle,param);
				float fubencircle  = *(float *)param;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				float N = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenXiShuN,vipLevel).c_str());
				float K = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenXiShuK,vipLevel).c_str());
				circlexishu = fubencircle * N + K;
			}

			exp = exp * xishu * circlexishu;
			groupContribute = groupContribute * xishu * circlexishu;
			junTuanExp = junTuanExp;			
			input->setString(crArgumentParser::appItoa(exp));
			if (input2.valid())
			{
				input2->setString(crArgumentParser::appItoa(groupContribute));
			}
			if (input3.valid())
			{
				input3->setString(crArgumentParser::appItoa(junTuanExp));
			}
		}
	}
}

/************************************************************************/
/*    crJXJJunTuanFuBenSettleGetRewardMethod                                                                  */
/************************************************************************/
JXJ::crJXJJunTuanFuBenSettleGetRewardMethod::crJXJJunTuanFuBenSettleGetRewardMethod():
	m_this(NULL)
{
}

JXJ::crJXJJunTuanFuBenSettleGetRewardMethod::crJXJJunTuanFuBenSettleGetRewardMethod( const crJXJJunTuanFuBenSettleGetRewardMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJJunTuanFuBenSettleGetRewardMethod::inputParam( int i, void *param )
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

void JXJ::crJXJJunTuanFuBenSettleGetRewardMethod::addParam( int i, const std::string& str )
{
	
}

void JXJ::crJXJJunTuanFuBenSettleGetRewardMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor && canvas.valid() && canvas->getDataClass())
		{
			crData * canvasData = canvas->getDataClass();

			void *param;
			canvasData->getParam(WCHDATA_JXJJunTuanFuBenId,param);
			int fubenid = *(int *)param;
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeInt(fubenid); 

			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvJunTuanFuBenSettleGetReward,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/************************************************************************/
/*    crJXJRecvJunTuanFuBenSettleGetRewardMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvJunTuanFuBenSettleGetRewardMethod::crJXJRecvJunTuanFuBenSettleGetRewardMethod():
	m_this(NULL)
{
}

JXJ::crJXJRecvJunTuanFuBenSettleGetRewardMethod::crJXJRecvJunTuanFuBenSettleGetRewardMethod( const crJXJRecvJunTuanFuBenSettleGetRewardMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJRecvJunTuanFuBenSettleGetRewardMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvJunTuanFuBenSettleGetRewardMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvJunTuanFuBenSettleGetRewardMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int myplayerid = m_this->getPlayerID();
			unsigned char logRet = 0;
			int fubenid = m_stream->_readInt();
			if (m_this->getMainRole() == NULL)
			{
				logRet = 1;
				char gbuf[256];
				sprintf(gbuf,"%s 领取军团副本奖励 fubenID %d 领取状态 %d  %d|%d|%d\0",m_this->getCharacterName().c_str(),fubenid,logRet,0,0,0);
				std::string logdata(gbuf);
				GameLogData gamelog(Log_GetJunTuanFuBenAward,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(myplayerid,&gamelog));
				return;
			}
			crData * data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			void *param;
			data->getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *(int *)param;
			int exp = 0;
			//int exploit = 0;
			//int countryContribute = 0;
			int groupContribute = 0;
			int junTuanFunds = 0;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			do 
			{
				ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
				if (cmItr == consortiamap->end())
				{
					logRet = 2;
					break;
				}
				if ( ! (cmItr->second.valid()) )
				{
					logRet = 3;
					break;
				}
				ref_ptr<crData> groupData = cmItr->second->getDataClass();
				if (!groupData.valid())
				{
					logRet = 4;
					break;
				}

				groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
				ConsortiaMemberMap::iterator cmmIt = memberMap->find(myplayerid);
				if (cmmIt == memberMap->end())
				{
					logRet = 5;
					break;
				}
				if ( !(cmmIt->second.valid()) )
				{
					logRet = 6;
					break;
				}
				if (cmmIt->second->m_curFuBen != fubenid)
				{
					logRet = 7;
					/*break;*/
				}

				float xishu = 1.0f;
				ref_ptr<crTableIO>  CoefficientTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCoefficientTab);
				crTableIO::StrVec corecord;
				if (CoefficientTab.valid())
				{
					crRole *mainRole = m_this->getMainRole();
					crData *roleData = mainRole->getDataClass();
					roleData->getParam(WCHDATA_Level,param);
					unsigned char rolelv = *(unsigned char *)param;
					int LvIdx = CoefficientTab->getTitleIndex("Level");
					int fubenxishuIdx = CoefficientTab->getTitleIndex("军团副本系数");
					if (CoefficientTab->queryOneRecord(LvIdx,crArgumentParser::appItoa(rolelv),corecord) >= 0)
					{
						xishu = atof(corecord[fubenxishuIdx].c_str());
					}
				}

				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				float N = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenXiShuN,vipLevel).c_str());
				float K = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenXiShuK,vipLevel).c_str());
				float circlexishu = cmmIt->second->m_fubencircle * N + K;

				//int activeVal = cmmIt->second->m_curFuBenActiveVal;
				exp = cmmIt->second->m_curFuBenExp * xishu * circlexishu;
				//int zhanGong = cmmIt->second->m_curFuBenAchievement;
				//exploit = cmmIt->second->m_curFuBenExploit;
				//countryContribute = cmmIt->second->m_curFuBenCountryContribute;
				junTuanFunds = cmmIt->second->m_curFuBenGroupFunds;
				
				//cmmIt->second->m_todayActiveVal += activeVal;
				//cmmIt->second->m_curWeekActiveVal += activeVal;

				groupContribute = cmmIt->second->m_curFuBenGroupContribute * xishu * circlexishu;

				//groupData->getParam(WCHDATA_JXJConsortiaActiveRank,param);
				//ConsortiaActiveRank *activeRank = (ConsortiaActiveRank *)param;

				//CRCore::ref_ptr<ConsortiaActiveRankItem> rankItem; 
				//rankItem = new ConsortiaActiveRankItem(cmmIt->first);
				//rankItem->m_groupPos = cmmIt->second->getGroupPos();
				//rankItem->m_activeVal = cmmIt->second->getTodayWeekActive();
				//rankItem->m_name = cmmIt->second->getName();
				//activeRank->updateRank(rankItem->getKey(),rankItem);

				int testFunds = junTuanFunds;
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJGroupFundsUpgrade,MAKEINT64(&testFunds,mygroupid));
				cmmIt->second->clearJunTuanFuBenInfo();

				bool modify = true;
				groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);

				data->getParam(WCHDATA_JXJHadWonJunTuanFBT,param);
				bool hadwontoday = *(bool *)param;
				CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeBool(hadwontoday);
				stream->_writeInt(junTuanFunds);
				//stream->_writeInt(exp);
				////stream->_writeInt(zhanGong);
				//stream->_writeInt(exploit);
				//stream->_writeInt(countryContribute);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJRecvJunTuanFuBenSettleGetReward,stream.get());

				ref_ptr<crGameServerPlayerData> playerData 
					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myplayerid));
				if (playerData.valid())
				{
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			} while (0);
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

			if(exp!=0) m_this->getMainRole()->doEvent(WCH_JXJRecvExperience,MAKEINT64(exp,NULL));
			/*if(exploit!=0) m_this->getMainRole()->doEvent(WCH_JXJRecvExploit,MAKEINT64(exploit,NULL));
			if(countryContribute!=0) m_this->doEvent(WCH_JXJRecvCountryContribute,MAKEINT64(countryContribute,NULL));*/
			if(groupContribute != 0) m_this->doEvent(WCH_JXJRecvGroupContribution,MAKEINT64(groupContribute,NULL));

			char gbuf[256];
			sprintf(gbuf,"%s 领取军团副本奖励 fubenID %d 领取状态 %d  %d|%d|%d\0",m_this->getCharacterName().c_str(),fubenid,logRet,exp,junTuanFunds,groupContribute);
			std::string logdata(gbuf);
			GameLogData gamelog(Log_GetJunTuanFuBenAward,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(myplayerid,&gamelog));
		
		}
		else if (m_netType == GameClient_Game)
		{
			if (m_this->getMainRole() 
				&& m_this->getDataClass()
				&& m_this->getMainRole()->getDataClass())
			{
				crData * playerData = m_this->getDataClass();
				crData * mainRoleData = m_this->getMainRole()->getDataClass();
				//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if (/*canvas.valid() && canvas->getDataClass() && */playerData)
				{
					//crData * canvasData = canvas->getDataClass();
					bool hadwon = m_stream->_readBool();
					playerData->inputParam(WCHDATA_JXJHadWonJunTuanFBT,&hadwon);

					int extra = m_stream->_readInt(); 
					if(extra!=0)
						crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_GroupFunds,extra));	
					//int exp = m_stream->_readInt();
					////int zhanGong = m_stream->_readInt();
					//int exploit = m_stream->_readInt();
					//int countryContribute = m_stream->_readInt();

					//void *param;
					//canvasData->excHandle(MAKEINT64(WCH_LockData,1));
					//canvasData->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
					//crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
					//if (mySelfInfo)
					//{
					//	mySelfInfo->setActiveValue(mySelfInfo->getActiveVaule() + activeVal);
					//	mySelfInfo->setTodayWeekActive(mySelfInfo->getTodayWeekActive() + activeVal);
					//}
					//canvasData->excHandle(MAKEINT64(WCH_LockData,0));

					//playerData->excHandle(MAKEINT64(WCH_LockData,1));
					//mainRoleData->excHandle(MAKEINT64(WCH_LockData,1));
					//mainRoleData->getParam(WCHDATA_Experience,param);
					//int newExp = *(int *)param + exp;
					//mainRoleData->inputParam(WCHDATA_Experience,&newExp);
					//mainRoleData->getParam(WCHDATA_JXJExploit,param);
					//int newExploit = *(int *)param + exploit;
					//mainRoleData->inputParam(WCHDATA_JXJExploit,&newExploit);
					//mainRoleData->excHandle(MAKEINT64(WCH_LockData,0));

					//playerData->getParam(WCHDATA_JXJCountryContribute,param);
					//int newCountryContribute = *(int *)param + countryContribute;
					//playerData->inputParam(WCHDATA_JXJCountryContribute,&newCountryContribute);
					//playerData->excHandle(MAKEINT64(WCH_LockData,0));
				
					//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Exp,exp));
					//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Achievement,zhanGong));
					//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Exploit,exploit));
					//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Contribute,countryContribute));

					m_this->doEvent(WCH_JXJExitJunTuanFuBen);
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJJunTuanFuBenSettleDoubleMethod                                                                  */
/************************************************************************/
JXJ::crJXJJunTuanFuBenSettleDoubleMethod::crJXJJunTuanFuBenSettleDoubleMethod():
	m_this(NULL)
{
}

JXJ::crJXJJunTuanFuBenSettleDoubleMethod::crJXJJunTuanFuBenSettleDoubleMethod( const crJXJJunTuanFuBenSettleDoubleMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJJunTuanFuBenSettleDoubleMethod::inputParam( int i, void *param )
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

void JXJ::crJXJJunTuanFuBenSettleDoubleMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJJunTuanFuBenSettleDoubleMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor && canvas.valid() && canvas->getDataClass())
		{
			crData * canvasData = canvas->getDataClass();

			void *param;
			canvasData->getParam(WCHDATA_JXJJunTuanFuBenId,param);
			int fubenid = *(int *)param;
			canvasData->getParam(WCHDATA_JXJJunTuanFuBenTotalTimes,param);
			unsigned char times = *(unsigned char *)param;
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeInt(fubenid); 
			stream->_writeUChar(times + 1); 

			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvJunTuanFuBenSettleDouble,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/************************************************************************/
/*    crJXJRecvJunTuanFuBenSettleDoubleMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvJunTuanFuBenSettleDoubleMethod::crJXJRecvJunTuanFuBenSettleDoubleMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvJunTuanFuBenSettleDoubleMethod::crJXJRecvJunTuanFuBenSettleDoubleMethod( const crJXJRecvJunTuanFuBenSettleDoubleMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_animationCanvas(handle.m_animationCanvas)
{
	for (unsigned char i = 0; i < 2; ++ i)
	{
		m_animation[i] = handle.m_animation[i];
	}
}

void JXJ::crJXJRecvJunTuanFuBenSettleDoubleMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvJunTuanFuBenSettleDoubleMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_animationCanvas = str;
		break;
	case 2:
	case 3:
		m_animation[i - 2] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvJunTuanFuBenSettleDoubleMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int fubenid = m_stream->_readInt();
			unsigned char times = m_stream->_readUChar();
			crData * data = m_this->getDataClass();
			int myplayerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 

			void *param;
			data->getParam(WCHDATA_JXJConsortiaID,param);
			int mygroupid = *(int *)param;
			unsigned char suc = 0;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiamap = (ConsortiaMap *)param;
			ref_ptr<crData>groupData;
			ConsortiaMap::iterator cmItr = consortiamap->find(mygroupid);
			if(cmItr!=consortiamap->end())
			{
				groupData = cmItr->second->getDataClass();
			}
			do 
			{
				if (!groupData.valid()) break;
				groupData->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				ConsortiaMemberMap * memberMap = (ConsortiaMemberMap *)param;
				ConsortiaMemberMap::iterator cmmIt = memberMap->find(myplayerid);
				if (cmmIt == memberMap->end()) break;
				if ( !(cmmIt->second.valid()) ) break;
				if (cmmIt->second->m_curFuBen != fubenid) break;

				if (cmmIt->second->m_curFuBenTimes != times - 1)
					times = cmmIt->second->m_curFuBenTimes + 1;

				unsigned char totalTimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJunTuanFuBenDoubleTimes	).c_str());
				if (times > totalTimes)
				{
					suc = JXJGroupFuBenDoubleRet_failed_timesLimit;
					break;
				}

				crTableIO::StrVec record;
				ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunTuanFuBenTab);

				if (fubentab.valid())
				{
					int doubleCostIdx = fubentab->getTitleIndex("花费");
					int possiblityIdx = fubentab->getTitleIndex("概率");
					if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),1,crArgumentParser::appItoa(times),record)>=0)
					{
						int doubleCost = atoi(record[doubleCostIdx].c_str());
						bool needdeductgold = false;
						MoneyChangeData moneydata(doubleCost,"军团副本奖励翻倍");
						m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
						if (moneydata.first == -1)
							suc = JXJGroupFuBenDoubleRet_failed_noYuanBao;
						else if (moneydata.first == 0)
							suc = JXJGroupFuBenDoubleRet_suc;
						if (suc == JXJGroupFuBenDoubleRet_suc)
						{
							//int activeValIdx = fubentab->getTitleIndex("活跃值");
							int expIdx = fubentab->getTitleIndex("经验");
							//int zhanGongIdx = fubentab->getTitleIndex("战功");
							//int exploitIdx = fubentab->getTitleIndex("功勋");
							int junTuanFundsIdx = fubentab->getTitleIndex("军团资金");
							//int countryContributeIdx = fubentab->getTitleIndex("国家贡献");
							int groupContributeIdx = fubentab->getTitleIndex("军团贡献");

							//int activeValMax = 0;
							//int activeValMin = 0;
							int exp = 0;
							//int zhanGong = 0;
							//int exploit = 0;
							int groupFunds = 0;
							//int countryContribute = 0;
							//int groupcontriMax = 0;
							//int groupcontriMin = 0;
							int groupcontri = 0;
							std::vector<int> possiblityVec ;
							crArgumentParser::appAtoVec(record[possiblityIdx],possiblityVec );
							CRCore::rangei r(0,100);
							char select = r.get_random();
							bool doubleSuc = false;
							unsigned char idx = 1;
							if (select >= possiblityVec[1] && select <= possiblityVec[0])
							{
								doubleSuc = true;
								idx = 0;
							}
							std::vector<int> tmpVec ;
							//crArgumentParser::appAtoVec(record[activeValIdx],tmpVec);
							//activeValMax = tmpVec[0];
							//activeValMin = tmpVec[1];

							crArgumentParser::appAtoVec(record[groupContributeIdx],tmpVec);
							//groupcontriMax = tmpVec[0];
							//groupcontriMin = tmpVec[1];
							groupcontri = tmpVec[idx];
							crArgumentParser::appAtoVec(record[expIdx],tmpVec);
							exp = tmpVec[idx];

							//crArgumentParser::appAtoVec(record[exploitIdx],tmpVec);
							//exploit = tmpVec[idx];

							crArgumentParser::appAtoVec(record[junTuanFundsIdx],tmpVec);
							groupFunds = tmpVec[idx];

							//crArgumentParser::appAtoVec(record[countryContributeIdx],tmpVec);
							//countryContribute = tmpVec[idx];

							//r.set(activeValMin,activeValMax);
							//cmmIt->second->m_curFuBenActiveVal += r.get_random();
							cmmIt->second->m_curFuBenExp += exp;
							//cmmIt->second->m_curFuBenExploit += exploit;
							cmmIt->second->m_curFuBenGroupFunds += groupFunds;
							//cmmIt->second->m_curFuBenCountryContribute += countryContribute;
							cmmIt->second->m_curFuBenTimes ++;
							//r.set(groupcontriMin,groupcontriMax);
							cmmIt->second->m_curFuBenGroupContribute += groupcontri;

							CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(24);
							stream->_writeUChar(suc);
							stream->_writeBool(doubleSuc);
							stream->_writeInt(fubenid);
							stream->_writeUChar(cmmIt->second->m_curFuBenTimes);
							//stream->_writeInt(cmmIt->second->m_curFuBenActiveVal);
							stream->_writeInt(cmmIt->second->m_curFuBenExp);
							//stream->_writeInt(cmmIt->second->m_curFuBenExploit);
							stream->_writeInt(cmmIt->second->m_curFuBenGroupFunds);							
							//stream->_writeInt(cmmIt->second->m_curFuBenCountryContribute);
							stream->_writeInt(cmmIt->second->m_curFuBenGroupContribute);

							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJRecvJunTuanFuBenSettleDouble,stream.get());

							ref_ptr<crGameServerPlayerData> playerData 
								= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myplayerid));
							if (playerData.valid())
							{
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
				}
			} while(0);
			if (suc != JXJGroupFuBenDoubleRet_suc)
			{
				CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUChar(suc);
				stream->_writeUChar(times - 1);

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJRecvJunTuanFuBenSettleDouble,stream.get());

				ref_ptr<crGameServerPlayerData> playerData 
					= dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myplayerid));
				if (playerData.valid())
				{
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char suc = m_stream->_readUChar();
			switch (suc)
			{
			case JXJGroupFuBenDoubleRet_suc:
				{
					ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animationCanvas);
					if (canvas.valid() && animatinoCanvas.valid())
					{
						crData * canvasData = canvas->getDataClass();
						if (canvasData)
						{
							bool doubleSuc = m_stream->_readBool();
							int fuBenId = m_stream->_readInt();
							unsigned char times = m_stream->_readUChar();

							//int activeVal = m_stream->_readInt();
							int exp = m_stream->_readInt();
							//int achivement = m_stream->_readInt();
							//int exploit = m_stream->_readInt();
							int groupExp = m_stream->_readInt();
							//int countryContribute = m_stream->_readInt();
							int groupyContribute = m_stream->_readInt();
							canvasData->inputParam(WCHDATA_JXJJunTuanFuBenTotalTimes,&times);
							canvasData->inputParam(WCHDATA_JXJJunTuanFuBenId	,&fuBenId);
							//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenActiveVal,&activeVal);
							canvasData->inputParam(WCHDATA_JXJJunTuanFuBenExp	,&exp);
							//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenAchivement	,&achivement);
							//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenExploit		,&exploit);
							canvasData->inputParam(WCHDATA_JXJJunTuanFuBenGroupFunds	,&groupExp);
							//canvasData->inputParam(WCHDATA_JXJJunTuanFuBenCountryContribute,&countryContribute);
							canvasData->inputParam(WCHDATA_JXJJunTuanFuBenGroupContribute,&groupyContribute);
							ref_ptr<crSequence> sequence[2] = {NULL};

							unsigned char idx = (unsigned char)doubleSuc;
							for (unsigned char i = 0; i < 2; ++ i)
							{
								sequence[i] = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_animation[i]));
							}
							if (sequence[idx].valid())
							{
								crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
								sequence[idx]->startEffect();
							}

							canvas->doEvent(WCH_UI_UpdateData);
						}
					}
				}
				break;
			case JXJGroupFuBenDoubleRet_failed_noYuanBao:
				{
					unsigned char times = m_stream->_readUChar() - 1;
					ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if (canvas.valid() && canvas->getDataClass())
					{
						crData * canvasData = canvas->getDataClass();
						canvasData->inputParam(WCHDATA_JXJJunTuanFuBenTotalTimes,&times);
					}
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(3032,NULL));
				}
				break;
			case JXJGroupFuBenDoubleRet_failed_timesLimit:
				{
					unsigned char times = m_stream->_readUChar() - 1;
					ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if (canvas.valid() && canvas->getDataClass())
					{
						crData * canvasData = canvas->getDataClass();
						canvasData->inputParam(WCHDATA_JXJJunTuanFuBenTotalTimes,&times);
					}
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4073,NULL));
				}
				break;
			default:
				break;
			}
		}
	}

}

/************************************************************************/
/*    crJXJQueryMyCantonInfoMapMethod                                                                  */
/************************************************************************/
JXJ::crJXJQueryMyCantonInfoMethod::crJXJQueryMyCantonInfoMethod():
	m_this(NULL)
{

}

JXJ::crJXJQueryMyCantonInfoMethod::crJXJQueryMyCantonInfoMethod( const crJXJQueryMyCantonInfoMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJQueryMyCantonInfoMethod::inputParam( int i, void *param )
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

void JXJ::crJXJQueryMyCantonInfoMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJQueryMyCantonInfoMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQueryMyCantonInfo,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/************************************************************************/
/*    crJXJRecvQueryMyCantonInfoMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvQueryMyCantonInfoMethod::crJXJRecvQueryMyCantonInfoMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{
}

JXJ::crJXJRecvQueryMyCantonInfoMethod::crJXJRecvQueryMyCantonInfoMethod( const crJXJRecvQueryMyCantonInfoMethod & handle ):
	crMethod(handle),
	m_mygroupcanvas(handle.m_mygroupcanvas)
{

}

void JXJ::crJXJRecvQueryMyCantonInfoMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvQueryMyCantonInfoMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvQueryMyCantonInfoMethod::operator()( crHandle &handle )
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			crData *data = m_this->getDataClass();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() )
			{
				void *param;
				data->getParam(WCHDATA_JXJConsortiaID,param);
				int mygroupid = *(int *)param;

				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;

				if (shiliid >= c_startShiliID)
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					if (gsBrainData)
					{
						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
						ConsortiaMap *consortiamap = (ConsortiaMap *)param;

						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
						crJXJShili *shili = (crJXJShili *)param;
						if (shili && shili->getDataClass())
						{
							crData *shilidata = shili->getDataClass();	
							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
							shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
							CantonMap * cantonMap = (CantonMap *)param;
							CantonMap::iterator cantonitr;
							ref_ptr<crData> groupData;
							ConsortiaMap::iterator iter = consortiamap->find(mygroupid);;
							if (iter != consortiamap->end())
							{
								groupData = iter->second->getDataClass();
							}
							if (groupData.valid())
							{
								groupData->getParam(WCHDATA_JXJGroupFenFengedCantonMapT,param);
								FenFengedCantonMap *fenFengedCantonMap = (FenFengedCantonMap *)param;
								FenFengedCantonMap::iterator ffcmItr = fenFengedCantonMap->begin();
								if(ffcmItr != fenFengedCantonMap->end())
								{
									cantonitr = cantonMap->find(ffcmItr->first);
									if (cantonitr != cantonMap->end() && cantonitr->second.valid())
									{
										if(cantonitr->second->getConsortiaId() != mygroupid)
										{
											fenFengedCantonMap->clear();
											for (cantonitr = cantonMap->begin();cantonitr != cantonMap->end();++cantonitr)
											{
												if(cantonitr->second->getConsortiaId() == mygroupid)
												{
													fenFengedCantonMap->insert(FenFengedCantonMap::value_type(cantonitr->first,1000));
													break;
												}
											}
											if (fenFengedCantonMap->empty())
											{
												//去除分封出去的的城池
												groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
												FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
												fenFengedChengChiMap->clear();
											}
										}
									}
								}
								else
								{
									fenFengedCantonMap->clear();
									for (cantonitr = cantonMap->begin();cantonitr != cantonMap->end();++cantonitr)
									{
										if(cantonitr->second->getConsortiaId() == mygroupid)
										{
											fenFengedCantonMap->insert(FenFengedCantonMap::value_type(cantonitr->first,1000));
											break;
										}
									}
									if (fenFengedCantonMap->empty())
									{
										//去除分封出去的的城池
										groupData->getParam(WCHDATA_JXJFenFengedChengChiMap,param);
										FenFengedChengChiMap* fenFengedChengChiMap = (FenFengedChengChiMap*)param;
										fenFengedChengChiMap->clear();
									}
								}
								/*		for (; ffcmItr != fenFengedCantonMap->end();)
								{
								cantonitr = cantonMap->find(ffcmItr->first);
								if (cantonitr != cantonMap->end() && cantonitr->second.valid())
								{
								if(cantonitr->second->getConsortiaId() != mygroupid)
								{
								ffcmItr = fenFengedCantonMap->erase(ffcmItr);
								continue;
								}
								}
								++ ffcmItr;
								}*/

								groupData->getParam(WCHDATA_JXJGroupApplyCantonMapT,param);
								ApplyCantonMap *applyCantonMap = (ApplyCantonMap *)param;

								CRCore::ref_ptr<CRCore::crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(16);

								stream->_writeUChar(fenFengedCantonMap->size());

								ffcmItr = fenFengedCantonMap->begin();
								for (; ffcmItr != fenFengedCantonMap->end(); ++ ffcmItr )
								{
									stream->_writeInt(ffcmItr->first);
								}

								stream->_writeUChar(applyCantonMap ->size());

								ApplyCantonMap ::iterator acmItr = applyCantonMap ->begin();
								for (; acmItr != applyCantonMap ->end(); ++ acmItr)
								{
									stream->_writeInt(acmItr->first);
								}

								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryMyCantonInfo,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
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
			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_mygroupcanvas);
			if (mygroupcanvas.valid())
			{
				crData *canvasdata = mygroupcanvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

					canvasdata->getParam(WCHDATA_JXJAllocateCantonVec,param);
					MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
					myAllocatedCantonVec ->clear();
					unsigned char size = m_stream->_readUChar();
					unsigned char i = 0;
					for (; i < size; ++ i)
					{
						myAllocatedCantonVec->push_back(m_stream->_readInt());
					}

					canvasdata->getParam(WCHDATA_JXJMyApplyCantonVec,param);
					MyApplyCantonVec * myApplyCantonVec = (MyApplyCantonVec *)param;
					myApplyCantonVec ->clear();
					size = m_stream->_readUChar();
					for (i = 0; i < size; ++ i)
					{
						myApplyCantonVec->push_back(m_stream->_readInt());
					}
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
					/*m_this->doEvent(WCH_JXJGroupSetSelectedCantonID);*/
					/*m_this->doEvent(WCH_JXJResetGroupChengChiIdVec);*/
					/*m_this->doEvent(WCH_JXJQueryMyCantonGroupMap	);
					m_this->doEvent(WCH_JXJQueryFenFengChengChiMap);*/
					mygroupcanvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJNameLimitMethod                                                                  */
/************************************************************************/
JXJ::crJXJNameLimitMethod::crJXJNameLimitMethod():
	m_this(NULL)
{
}

JXJ::crJXJNameLimitMethod::crJXJNameLimitMethod( const crJXJNameLimitMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_length(handle.m_length)
{

}

void JXJ::crJXJNameLimitMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJNameLimitMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_length = (unsigned short) atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJNameLimitMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crEditWidgetNode>  nameInput = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_input));
	std::wstring text = nameInput->getString();
	std::string name = CRIOManager::convertUTF16toUTF8(text.c_str());

	//if (name.find_first_of(' ') != -1)
	//{
	//	name = name.substr(0,name.length() - 1);
	//}
	if (name.length() > m_length)
	{
		nameInput->setString(name.substr(0,m_length));
	}
}

/************************************************************************/
/*crJXJCheckNameLimitMethod  */
/************************************************************************/
crJXJCheckNameLimitMethod  ::crJXJCheckNameLimitMethod  ():
	m_this(NULL)
{
}
 crJXJCheckNameLimitMethod  ::crJXJCheckNameLimitMethod  (const crJXJCheckNameLimitMethod  & handle):
	crMethod(handle),
	m_checkstring(handle.m_checkstring)
{
}
void crJXJCheckNameLimitMethod  ::inputParam(int i, void *param)
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
			m_checkstring = *(std::string *)HIINT64(param64);
			m_success = (int *)LOINT64(param64);
		}
		else
		{
			m_checkstring.clear();
			*m_success = 0;
		}
		break;
	}
}
void crJXJCheckNameLimitMethod  ::addParam(int i, const std::string& str)
{
}
void crJXJCheckNameLimitMethod  ::operator()(crHandle &handle)
{	
	ref_ptr<crTableIO> InvalidNameTab;
	InvalidNameTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNameLimitTab);
	*m_success = 1;
	if (InvalidNameTab.valid())
	{
		std::wstring wtext = CRIOManager::convertUTF8toUTF16(m_checkstring.c_str());
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
					if(crGlobalHandle::isClient())
						m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4052,NULL));
					*m_success = 0;
					break;
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJGroupSetSelectedCantonIDMethod                                                                  */
/************************************************************************/
JXJ::crJXJGroupSetSelectedCantonIDMethod::crJXJGroupSetSelectedCantonIDMethod():
	m_this(NULL)
{

}

JXJ::crJXJGroupSetSelectedCantonIDMethod::crJXJGroupSetSelectedCantonIDMethod( const crJXJGroupSetSelectedCantonIDMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_radio(handle.m_radio)
{

}

void JXJ::crJXJGroupSetSelectedCantonIDMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGroupSetSelectedCantonIDMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radio = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJGroupSetSelectedCantonIDMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		do 
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() == false) break;
			crData * canvasData = canvas->getDataClass();
			if (!canvasData) break;
			ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio));

			int id = 0;
			canvasData->inputParam(WCHDATA_JXJGroupCanvasSelectedCantonId,&id);
			int idx = groupradio->getSelect();
			void *param;

			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));

			canvasData->getParam(WCHDATA_JXJMyApplyCantonVec,param);
			MyApplyCantonVec * myapplyCantonVec = (MyApplyCantonVec * )param;
			canvasData->getParam(WCHDATA_JXJAllocateCantonVec,param);
			MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
			std::vector<int> * vecPtr = NULL;

			if (myapplyCantonVec->empty())
			{
				vecPtr = myAllocatedCantonVec;
			}
			else if (myAllocatedCantonVec->empty())
			{
				vecPtr = myapplyCantonVec;
			}
			if (vecPtr && vecPtr->empty() == false)
			{
				if (idx > vecPtr->size())
				{
					idx = 0;
				}
				id = (*vecPtr)[idx];
				canvasData->inputParam(WCHDATA_JXJGroupCanvasSelectedCantonId,&id);
			}
			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));

		} while (0);
	}
}

/************************************************************************/
/*    crJXJGroupAllocateChengChiMethod                                                                  */
/************************************************************************/
//JXJ::crJXJGroupAllocateChengChiMethod::crJXJGroupAllocateChengChiMethod():
//	m_this(NULL)
//{
//
//}
//
//JXJ::crJXJGroupAllocateChengChiMethod::crJXJGroupAllocateChengChiMethod( const crJXJGroupAllocateChengChiMethod & handle ):
//	crMethod(handle),
//	m_offset(handle.m_offset),
//	m_canvas(handle.m_canvas),
//	m_table(handle.m_table),
//	m_type(handle.m_type)
//{
//}
//
//void JXJ::crJXJGroupAllocateChengChiMethod::inputParam( int i, void *param )
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
//		m_this = (crWidgetNode *)param;
//		break;
//	}
//}
//
//void JXJ::crJXJGroupAllocateChengChiMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	case 1:
//		m_offset = (unsigned char)atoi(str.c_str());
//		break;
//	case 2:
//		m_table = str;
//		break;
//	case 3:
//		m_type = (unsigned char )atoi(str.c_str());
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJGroupAllocateChengChiMethod::operator()( crHandle &handle )
//{
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//	if (m_this && netConductor )
//	{
//		do 
//		{
//			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//			if (canvas.valid() == false) break;
//			crData * canvasData = canvas->getDataClass();
//			if (!canvasData) break;
//			ref_ptr<crTableWidgetNode> tableWidget = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_table));
//			if (tableWidget .valid() == false) break;
//			void *param;
//			canvasData ->excHandle(MAKEINT64(WCH_LockData,1));
//			canvasData->getParam(WCHDATA_JXJGroupChengChiIdVec,param);
//			ChengChiIdVec * chengChiIdVec = (ChengChiIdVec *)param;
//			canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId,param);
//			int cantonId = *(int *)param;
//			if (m_offset < chengChiIdVec->size())
//			{
//				crPlayerDataEventPacket packet;
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				stream->createBuf(5);
//				stream->_writeUChar(m_type);
//				stream->_writeInt(tableWidget->getSelectData()); 
//				stream->_writeInt(cantonId);
//				stream->_writeInt((*chengChiIdVec)[m_offset]); 
//				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGroupAllocateChengChi,stream.get());
//				netConductor->getNetManager()->sendPacket("all",packet);
//			}
//			canvasData ->excHandle(MAKEINT64(WCH_LockData,0));
//		} while (0);
//	}
//}
//
///************************************************************************/
///*    crJXJRecvGroupAllocateChengChiMethod                                                                  */
///************************************************************************/
//JXJ::crJXJRecvGroupAllocateChengChiMethod::crJXJRecvGroupAllocateChengChiMethod():
//	m_this(NULL),
//	m_netType(GameClient_Game)
//{
//}
//
//JXJ::crJXJRecvGroupAllocateChengChiMethod::crJXJRecvGroupAllocateChengChiMethod( const crJXJRecvGroupAllocateChengChiMethod& handle ):
//	crMethod(handle)
//{
//
//}
//
//void JXJ::crJXJRecvGroupAllocateChengChiMethod::inputParam( int i, void *param )
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
//void JXJ::crJXJRecvGroupAllocateChengChiMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	default:
//		break;
//	}
//
//}
//
//void JXJ::crJXJRecvGroupAllocateChengChiMethod::operator()( crHandle &handle )
//{
//	if (m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			unsigned char op = m_stream->_readUChar();
//			int groupId = m_stream->_readInt();
//			int cantonId = m_stream->_readInt();
//			int chengChiId = m_stream->_readInt();
//			void *param;
//			unsigned char success = 0;
//			int playerid = m_this->getPlayerID();
//
//			crData *data = m_this->getDataClass();
//
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			if (playerData.valid())
//			{
//				data-> getParam(WCHDATA_JXJConsortiaID,param);
//				int mygroupid = *( int *)param;
//
//				data->getParam(WCHDATA_JXJShiliID,param);
//				unsigned char shiliid = *(unsigned char *)param;
//
//				data->getParam(WCHDATA_JXJVipLv,param);
//				unsigned char viplv = *(unsigned char *)param;
//				unsigned char maxNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCantonChengChiAllocateMaxNum,viplv).c_str());
//
//				if (mygroupid > 0 && shiliid >= c_startShiliID)
//				{
//					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
//					if (gsBrainData)
//					{
//						gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
//
//						gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
//						ConsortiaMap *consortiamap = (ConsortiaMap *)param;
//
//						gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
//						crJXJShili *shili = (crJXJShili *)param;
//						if (cantonId && shili && shili->getDataClass())
//						{
//							crData *shilidata = shili->getDataClass();	
//							shilidata->excHandle(MAKEINT64(WCH_LockData,1));
//							shilidata->getParam(WCHDATA_JXJShiliCantonMap,param);
//							CantonMap * cantonMap = (CantonMap *)param;
//							CantonMap::iterator it = cantonMap->find(cantonId);
//							if (it != cantonMap->end())
//							{
//								if (it->second.valid())
//								{
//									unsigned char suc = JXJGroupAllocateChengChiRet_failed_unkwon;
//									if (it->second->getConsortiaId() == mygroupid)
//									{
//										if (mygroupid != groupId)
//										{
//											ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
//											ChengChiEarningsMap::iterator ccemItr = chengChiEarningsMap.find(groupId);
//											if (ccemItr != chengChiEarningsMap.end())
//											{
//												int chengchiId_tmp = 0;
//												if (ccemItr->second.valid())
//												{
//													chengchiId_tmp = ccemItr->second->getChengChiId();
//												}
//												if (op)
//												{
//													if (chengchiId_tmp)
//													{
//														suc = JXJGroupAllocateChengChiRet_failed_hasOwn;
//													}
//													else
//													{
//														ConsortiaMap::iterator cmItr = consortiamap->find(groupId);
//														if (cmItr != consortiamap->end())
//														{
//															crData * groupData = cmItr->second->getDataClass();
//															if (groupData)
//															{
//																groupData->getParam(WCHDATA_JXJGroupFenFengedChengChiSet,param);
//																FenFengedChengChiSet* fenFengedChengChiSet = (FenFengedChengChiSet*)param;
//																if (fenFengedChengChiSet->size() >= maxNum )
//																{
//																	suc = JXJGroupAllocateChengChiRet_failed_full;
//																}
//																else
//																{
//																	FenFengedChengChiSet::iterator gaccsItr = fenFengedChengChiSet->find(chengChiId);
//																	if (gaccsItr == fenFengedChengChiSet->end())
//																	{
//																		fenFengedChengChiSet->insert(chengChiId);
//																		ccemItr->second->setChengChiId(chengChiId);
//																		suc = JXJGroupAllocateChengChiRet_suc;
//																	}
//																	bool modify = true;
//																	groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
//																}
//															}
//														}
//													}
//												}
//												else
//												{
//													if (chengchiId_tmp != chengChiId)
//													{
//														suc = JXJGroupAllocateChengChiRet_failed_hasOwn;
//													}
//													else
//													{
//														ConsortiaMap::iterator cmItr = consortiamap->find(groupId);
//														if (cmItr != consortiamap->end())
//														{
//															crData * groupData = cmItr->second->getDataClass();
//															if (groupData)
//															{
//																ccemItr->second->setChengChiId(0);
//																groupData->getParam(WCHDATA_JXJGroupFenFengedChengChiSet,param);
//																FenFengedChengChiSet* fenFengedChengChiSet = (FenFengedChengChiSet*)param;
//																FenFengedChengChiSet::iterator gaccsItr = fenFengedChengChiSet->find(chengChiId);
//																if (gaccsItr != fenFengedChengChiSet->end())
//																{
//																	fenFengedChengChiSet->erase(gaccsItr);
//																	suc = JXJGroupAllocateChengChiRet_suc;
//																}
//																bool modify = true;
//																groupData->inputParam(WCHDATA_JXJConsortiaModify,&modify);
//															}
//														}
//													}
//												}
//												bool modify = true;
//												shilidata->inputParam(WCHDATA_JXJShiliModify,&modify);
//											}
//										}
//										else
//										{
//											suc = JXJGroupAllocateChengChiRet_failed_isGuard;
//										}
//									}
//									else
//									{
//										suc = JXJGroupRemoveChengChiGroupRetVal_failed_notGuard;
//									}
//
//									ref_ptr<crStreamBuf> stream = new crStreamBuf;
//									stream->createBuf(10);
//									stream->_writeUChar(suc);
//									stream->_writeUChar(op);
//									stream->_writeInt(groupId);
//									stream->_writeInt(chengChiId);
//									crPlayerDataEventPacket packet;
//									crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGroupAllocateChengChi,stream.get());
//									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//								}
//							}
//							shilidata->excHandle(MAKEINT64(WCH_LockData,0));
//						}
//						gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
//					}
//				}
//			}
//
//		}
//		else if (m_netType == GameClient_Game)
//		{
//			void *param;
//			ref_ptr<crCanvasNode>mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//			if (mygroupcanvas.valid())
//			{
//				crData *canvasdata = mygroupcanvas->getDataClass();
//				if (canvasdata)
//				{
//					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
//
//					canvasdata->getParam(WCHDATA_JXJCantonGroupVec,param);
//					CantonGroupVec* cantonGroupVec = (CantonGroupVec *)param;
//
//					unsigned char suc = m_stream->_readUChar();
//					unsigned char op = m_stream->_readUChar();
//					int groupid = m_stream->_readInt();
//					int chengChiId = m_stream->_readInt();
//
//					switch (suc)
//					{
//					case JXJGroupAllocateChengChiRet_failed_unkwon:
//						break;
//					case JXJGroupAllocateChengChiRet_failed_isGuard:
//						break;
//					case JXJGroupAllocateChengChiRet_suc:
//						{
//
//							canvasdata->getParam(WCHDATA_JXJCantonFenFengChengChiSet,param);
//							CantonFenFengChengChiSet * fenFengSet = (CantonFenFengChengChiSet *)param;
//
//							CantonGroupVec::iterator cgvItr = cantonGroupVec->begin();
//							for (; cgvItr != cantonGroupVec->end(); ++ cgvItr )
//							{
//								if (cgvItr->valid() && (*cgvItr)->m_groupId == groupid)
//								{
//									if (op)
//									{
//										// 分封
//										(*cgvItr)->m_chengChiId = chengChiId;
//										fenFengSet->insert(chengChiId);
//									}
//									else
//									{
//										fenFengSet->erase(chengChiId);
//										// 解除
//										(*cgvItr)->m_chengChiId = 0;
//									}
//								}
//							}
//
//							mygroupcanvas->doEvent(WCH_UI_UpdateData);
//						}
//						break;
//					default:
//						break;
//					}
//					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
//				}
//			}
//		}
//	}
//}

/************************************************************************/
/*    crJXJResetGroupChengChiIdVecMethod                                                                  */
/************************************************************************/
//JXJ::crJXJResetGroupChengChiIdVecMethod::crJXJResetGroupChengChiIdVecMethod():
//	m_this(NULL)
//{
//}
//
//JXJ::crJXJResetGroupChengChiIdVecMethod::crJXJResetGroupChengChiIdVecMethod( const crJXJResetGroupChengChiIdVecMethod & handle ):
//	crMethod(handle),
//	m_canvas (handle.m_canvas),
//	m_scroll(handle.m_scroll)
//{
//}
//
//void JXJ::crJXJResetGroupChengChiIdVecMethod::inputParam( int i, void *param )
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
//		m_this = (crWidgetNode *)param;
//		break;
//	}
//}
//
//void JXJ::crJXJResetGroupChengChiIdVecMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	case 1:
//		m_scroll = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJResetGroupChengChiIdVecMethod::operator()( crHandle &handle )
//{
//	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if (playerGameData && m_this)
//	{
//		crData *data = playerGameData->getDataClass();
//		ref_ptr<crCanvasNode> mygroupcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//		if (mygroupcanvas.valid())
//		{
//			ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(mygroupcanvas->getWidget(m_scroll));
//			crData *canvasData = mygroupcanvas->getDataClass();
//			if (canvasData)
//			{
//				int cantonIdx = 0,chengchiId = 0;
//				ref_ptr<crTableIO>  chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
//				crTableIO::StrVec record;
//				crTableIO::DataVec dataVec;
//
//				int chengChiNameIdx = 0,chengChiIdIdx = 0;
//				if (chengchiTab.valid())
//				{
//					cantonIdx = chengchiTab->getTitleIndex("州ID");
//					chengChiIdIdx = chengchiTab->getTitleIndex("id");
//				}
//
//				canvasData->excHandle(MAKEINT64(WCH_LockData,1));
//				void *param;
//				canvasData->getParam(WCHDATA_JXJGroupCanvasSelectedCantonId,param);
//				int cantonId = *(int *)param;
//
//				canvasData->getParam(WCHDATA_JXJGroupChengChiIdVec,param);
//				ChengChiIdVec * chengChiIdVec = (ChengChiIdVec *)param;
//				chengChiIdVec->clear();
//				chengchiTab->queryRecords(cantonIdx,crArgumentParser::appItoa(cantonId),dataVec);
//
//				data->excHandle(MAKEINT64(WCH_LockData,1));
//
//				data->getParam(WCHDATA_JXJShiliID,param);
//				unsigned char shiliid = *(unsigned char *)param;
//				data->getParam(WCHDATA_JXJChengChiMap,param);
//				ChengchiMap *chengchimap = (ChengchiMap *)param;
//				ChengchiMap::iterator cmItr;
//
//				crTableIO::DataVec::iterator tdvIt = dataVec.begin();
//				crTableIO::DataVec::iterator tdvIt2 = tdvIt;
//				for (;  tdvIt != dataVec.end();++ tdvIt)
//				{
//					cmItr = chengchimap->find(atoi((*tdvIt)[chengChiIdIdx].c_str()));
//					if (cmItr != chengchimap->end() && cmItr->second->getChengzhuShili() == shiliid)
//					{
//						if (tdvIt2 != tdvIt)
//						{
//							tdvIt2->swap(*tdvIt);
//							++ tdvIt2 ;
//						}
//					}
//					//atoi((*tdvIt)[chengChiIdIdx].c_str());
//				}
//
//				data->excHandle(MAKEINT64(WCH_LockData,0));
//
//
//				unsigned char fenFengCurPage = 0;
//				if (scroll.valid())
//				{
//					int scrollPerpageNum = 7;
//					if (dataVec.size() > scrollPerpageNum)
//					{
//						fenFengCurPage = scroll->getValue();
//					}
//				}
//
//				tdvIt = dataVec.begin();
//				for (unsigned char i = 0,row = 0; row < 7 && tdvIt != dataVec.end() ;++i,++ tdvIt)
//				{
//					if (i >= fenFengCurPage)
//					{
//						chengChiIdVec->push_back(atoi((*tdvIt)[chengChiIdIdx].c_str()));
//						++ row;
//					}
//				}
//				canvasData->excHandle(MAKEINT64(WCH_LockData,0));
//			}
//		}
//	}
//}

/************************************************************************/
/*    crJXJMaintainShiliPeopleMapMethod                                                                  */
/************************************************************************/
JXJ::crJXJMaintainShiliPeopleMapMethod::crJXJMaintainShiliPeopleMapMethod():
	m_this(NULL)
{
}

JXJ::crJXJMaintainShiliPeopleMapMethod::crJXJMaintainShiliPeopleMapMethod( const crJXJMaintainShiliPeopleMapMethod& handle ):
	crMethod(handle)
{
}

void JXJ::crJXJMaintainShiliPeopleMapMethod::inputParam( int i, void *param )
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
	};
}

void JXJ::crJXJMaintainShiliPeopleMapMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_isOnline = (unsigned char ) atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJMaintainShiliPeopleMapMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crData *data = m_this->getDataClass();
		int playerid = m_this->getPlayerID();
		void *param;
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		data-> getParam(WCHDATA_JXJConsortiaID,param);
		int mygroupid = *( int *)param;

		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();

		time_t nowTime = time(0);
		bool hadInPeopleMap = false;

		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));

		gsBrainData->getParam(WCHDATA_JXJOnlineEveryDayPlayeridSet,param);
		OnlinePlayerSet * playerSet = (OnlinePlayerSet *)param;
		if (m_isOnline) playerSet->insert(playerid);

		gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = (ConsortiaMap *)param;
		ConsortiaMap::iterator cmItr;
		if (shiliid >= c_startShiliID)
		{
			for (int i = WCHDATA_JXJShiliWei; i <= WCHDATA_JXJShiliWu; ++ i)
			{
				if (i != shiliid - c_startShiliID + WCHDATA_JXJShiliWei)
				{
					gsBrainData->getParam(i,param);
					crJXJShili *shili = (crJXJShili *)param;
					if (shili && shili->getDataClass())
					{
						crData *shiliData = shili->getDataClass();	
						CRCore::ScopedLock<crData> lock(*shiliData);

						shiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
						ModifyPlayerSet * oldModifyPlayerSet = (ModifyPlayerSet *)param;
						shiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
						InsertPlayerSet * oldInsertPlayerSet = (InsertPlayerSet *)param;

						shiliData->getParam(WCHDATA_JXJPeopleMap,param);
						PeopleMap *peopleMap = (PeopleMap *)param;
						PeopleMap::iterator itr = peopleMap->find(playerid);
						if (itr != peopleMap->end())
						{
							hadInPeopleMap = true;
							peopleMap->erase(itr);
							if (oldModifyPlayerSet->find(playerid) != oldModifyPlayerSet->end())
							{
								oldModifyPlayerSet->erase(playerid);
							}
							if (oldInsertPlayerSet->find(playerid) != oldInsertPlayerSet->end())
							{
								oldInsertPlayerSet->erase(playerid);
							}
						}
					}
				}
			}
			gsBrainData->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
			crJXJShili *shili = (crJXJShili *)param;
			if (shili && shili->getDataClass())
			{
				crJXJShiliData * shiliData = dynamic_cast<crJXJShiliData * > (shili->getDataClass());
				//crData *shiliData = shili->getDataClass();	
				bool isModify = false;
				CRCore::ScopedLock<crData> lock(*shiliData);

				shiliData->getParam(WCHDATA_JXJTodayHaveLandedPlayerSet,param);
				std::set<int>*playerhavelanded = (std::set<int> *)param;
				playerhavelanded->insert(playerid);

				shiliData->getParam(WCHDATA_JXJCountryCandidateVecL,param);
				CandidateVec*candidateVecL = (CandidateVec*)param;

				for (CandidateVec::iterator cvLItr = candidateVecL->begin();
					cvLItr != candidateVecL->end();++ cvLItr)
				{
					if (cvLItr->valid() && (*cvLItr)->nPlayerID == m_this->getPlayerID())
					{
						(*cvLItr)->m_lastLogin = nowTime;
						isModify = true;
						break;
					}
				}
				

				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peopleMap = (PeopleMap *)param;
				PeopleMap::iterator itr = peopleMap->find(m_this->getPlayerID());
				if(itr == peopleMap->end())
				{
					CRCore::ref_ptr<PeopleInfo> item = new PeopleInfo(m_this->getPlayerID(),m_this->getCharacterName(),shiliid);							
					if (hadInPeopleMap)
					{
						item->needUpdate();
					}
					else
						item->needInsert();
					peopleMap->insert(PeopleMap::value_type(item->getID(),item));
					//isModify = true;
				}
				
				if (itr != peopleMap->end())
				{
					if (m_isOnline)
					{
						if (shiliData->getOfficialDes(m_this->getPlayerID()) != JXJCountryOfficial_normal)
						{
							ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
							crTableIO::StrVec textrecord;
							char szNotice[256];
							int textid = 4091;
							if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord) >= 0)
							{
								sprintf_s(szNotice,textrecord[1].c_str(),
									shiliData->getOfficialStr(m_this->getPlayerID()).c_str(),
									itr->second->getName().c_str());
								CRCore::ref_ptr<crJXJChatMessageInfo > msg = new crJXJChatMessageInfo("[通告]",
									szNotice,shiliid,shiliid);
								crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKEINT64(msg.get(),NULL));
							}
						}
					}
					if (!m_isOnline && m_this->getMainRole()
						&& m_this->getMainRole()->getDataClass())
					{
						// 下线时同步,刚上线未初始化
						crData *mainRoleData = m_this->getMainRole()->getDataClass();
						mainRoleData->getParam(WCHDATA_Level,param);
						unsigned char level = *(unsigned char *)param;
						if (itr->second->getLv()!= level)
						{
							itr->second->setLv(level);
							//itr->second->m_lv = level;
							//isModify = true;
						}
					}
					if (itr->second->getName() != m_this->getCharacterName()
						&& m_this->getCharacterName() != "")
					{
						itr->second->setName(m_this->getCharacterName());
						//itr->second->m_name = m_this->getCharacterName();
						//isModify = true;
					}
					data->getParam(WCHDATA_JXJBingyinglv,param);
					unsigned char bingyinglv = *(unsigned char *)param;
					if (itr->second->getTroopsLv() != bingyinglv)
					{
						itr->second->setTroopsLv(bingyinglv);
					}
					if (itr->second->getConsortiaId() != mygroupid)
					{
						itr->second->setConsortiaID(mygroupid);
						//isModify = true;
					}
					if (!m_isOnline)
					{
						data->getParam(WCHDATA_JXJGiftGold,param);
						int giftGold = *(int *)param;
						if (itr->second->getGiftGold() != giftGold)
						{
							itr->second->setGiftGold(giftGold);
						}
						data->getParam(WCHDATA_JXJPlayerTotalPayGold,param);
						int totalRecharge = *(int *)param;

						if (itr->second->getTotalRecharge() != totalRecharge)
						{
							itr->second->setTotalRecharge(totalRecharge);
						}
					}
					if(mygroupid>0)
					{
						cmItr = consortiamap->find(mygroupid);
						if (cmItr != consortiamap->end())
						{
							std::string newName ;
							cmItr->second->getName(newName );
							if (newName != itr->second->getConsortiaName())
							{
								itr->second->setConsortiaName(newName );
								//isModify = true;
							}
						}
						else
						{
							itr->second->setConsortiaID(0);
							itr->second->setConsortiaName("");
							//itr->second->m_consortiaName.clear();
							//itr->second->m_consortiaId = 0;
							//isModify = true;
						}
					}
					else
					{
						itr->second->setConsortiaName("");
						//itr->second->m_consortiaName.clear();
						//isModify = true;
					}
				}
				if(isModify) shiliData->inputParam(WCHDATA_JXJShiliModify,&isModify);
			}
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

JXJ::crJXJGroupFuLiRadioUpdateMethod::crJXJGroupFuLiRadioUpdateMethod():
	m_this(NULL)
{

}

JXJ::crJXJGroupFuLiRadioUpdateMethod::crJXJGroupFuLiRadioUpdateMethod( const crJXJGroupFuLiRadioUpdateMethod & handle ):
	crMethod(handle),
	m_canvas	(handle.m_canvas),
	m_contributeBtn(handle.m_contributeBtn),
	m_type(handle.m_type ),
	m_radioGroup(handle.m_radioGroup) 
{
	for (unsigned char i = 0; i < 5; ++ i)
	{
		m_contribute[i] = handle.m_contribute[i];
	}
}

void JXJ::crJXJGroupFuLiRadioUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGroupFuLiRadioUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		m_contribute[i - 1] = str;
		break;
	case 6:
		m_radioGroup = str;
		break;
	//case 6:
	//	m_contributeBtn = str;
	//	break;
	//case 7:
	//	m_contributeBtn_sw = str;
	//	break;
	//case 8:
	//	m_type = (unsigned char)(atoi(str.c_str()));
	//	break;
	//case 9:
	//case 10:
	//	m_contribute_sw[i - 6] = str;
	//	break;
	default:
		break;
	}
}

void JXJ::crJXJGroupFuLiRadioUpdateMethod::operator()( crHandle &handle )
{
	//ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	//if (canvas.valid() == false || canvas->getDataClass() == NULL) return;
	//crData * canvasData = canvas->getDataClass();

	//void *param;

	//canvasData->excHandle(MAKEINT64(WCH_LockData,1));
	//canvasData->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);			
	//crJXJConsortiaMember * mySelf = (crJXJConsortiaMember *)param;
	//ref_ptr<crRadioWidgetNode > contributeRadio[5]  = {NULL};
	//ref_ptr<crButtonWidgetNode> contributeBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_contributeBtn));
	//ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioGroup));
	////ref_ptr<crMultiSwitch> contributeBtn_sw = dynamic_cast<crMultiSwitch*>(canvas->getWidget(m_contributeBtn));
	////ref_ptr<crMultiSwitch>  contributeRadio_sw[5] = {NULL};
	//for (unsigned char i = 0; i < 5; i++)
	//{
	//	contributeRadio[i] = dynamic_cast<crRadioWidgetNode*>(canvas->getWidget(m_contribute[i]));
	//}
	//if (mySelf->getContributeType() != JXJGroupContributeType_all
	//	&& m_type == mySelf->getContributeType()
	//	)
	//{
	//	contributeRadio[m_type - 1]->select(true);
	//}
	////if (selectedNum > 1)
	////{
	////	if (contributeBtn.valid())
	////	{
	////		contributeBtn->setEnable(false);
	////		//if (contributeBtn_sw.valid())
	////		//{
	////		//	contributeBtn_sw->setActiveSwitchSet(3);
	////		//}
	////	}
	////}
	////else
	////{
	////	if (contributeBtn.valid())
	////	{
	////		contributeBtn->setEnable(true);
	////		if (contributeBtn_sw.valid())
	////		{
	////			contributeBtn_sw->setActiveSwitchSet(0);
	////		}
	////	}
	////}
	//canvasData->excHandle(MAKEINT64(WCH_LockData,0));
}

JXJ::crJXJCheckConsortiaDayZeroMethod::crJXJCheckConsortiaDayZeroMethod():
	m_this(NULL)
{
}

JXJ::crJXJCheckConsortiaDayZeroMethod::crJXJCheckConsortiaDayZeroMethod( const crJXJCheckConsortiaDayZeroMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJCheckConsortiaDayZeroMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCheckConsortiaDayZeroMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJCheckConsortiaDayZeroMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		// 服务器维护重启时 军团数据的一些维护
		crData * gsBrainData = m_this->getDataClass();
		void *param;
		std::string dateStr;
		std::string maxDateStr;
		short year,month,day;
		short hour,min,sec;

		time_t t = time(0);
		struct  tm  sTm = *localtime(&t);
		

		// 取距离现在最近的一次更新时间
		// 如果这个时间没有触发dayZero 相关的事件，则认为整个服务器都没有

		std::vector<std::string > vecStr;
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiaMap = (ConsortiaMap *)param;
		ConsortiaMap::iterator cmItr = consortiaMap->begin();
		for (; cmItr != consortiaMap->end(); ++ cmItr)
		{
			if (cmItr->second.valid())
			{
				dateStr = cmItr->second->getUpateDate();
				if (dateStr > maxDateStr)
					maxDateStr = dateStr;
			}
		}

		dateStr.clear();
		//std::cout << "---------------" << std::endl;
		//std::cout << maxDateStr << std::endl;
		year = month = day = 0;
		hour = min = sec = 0;
		vecStr.clear();
		crJXJConsortiaData::split(maxDateStr,"-",vecStr);

		if (vecStr.size() >= 3)
		{
			year = atoi(vecStr[0].c_str());
			month = atoi(vecStr[1].c_str());
			day = atoi(vecStr[2].c_str());
		}
		// 处理 00::00 更新
		if (sTm.tm_mon != month - 1
			|| sTm.tm_mday != day)
		{
			m_this->doEvent(WCH_JXJServerDayZero);
			if (sTm.tm_wday == 1)
			{
				m_this->doEvent(WCH_JXJServerWeekZero);
			}
		}

		int nPos = maxDateStr.find_first_of(' ');
		if (nPos != -1)
		{
			dateStr = maxDateStr.substr(nPos + 1,maxDateStr.length() - nPos - 1);
		}
		//std::cout << dateStr << std::endl;

		crJXJConsortiaData::split(dateStr,":",vecStr);

		if (vecStr.size() >= 3)
		{
			hour = atoi(vecStr[0].c_str());
			min = atoi(vecStr[1].c_str());
			sec = atoi(vecStr[2].c_str());
		}
		
		// 处理收益 非00::00 
		unsigned char needHour = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTimeOfGetChengChiEarnings	).c_str());
		if (
			(hour < needHour && sTm.tm_hour > needHour)
			|| (sTm.tm_hour == needHour && sTm.tm_min > 0)
			|| (sTm.tm_hour == needHour && sTm.tm_min == 0 && sTm.tm_sec > 2)
			)
		{
			m_this->doEvent(WCH_JXJGroupSettleEarnings);
		}

		//std::cout << year << std::endl;
		//std::cout << month << std::endl;
		//std::cout << day << std::endl;


		//std::cout << sTm->tm_year<< std::endl;
		//std::cout << sTm->tm_mon<< std::endl;
		//std::cout << sTm->tm_mday<< std::endl;

		//std::cout << hour << std::endl;
		//std::cout << min << std::endl;
		//std::cout << sec << std::endl;

		//std::cout << "xxxxxxxxxxxxxxx" << std::endl;

		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

JXJ::crJXJRecvPingMsgMethod::crJXJRecvPingMsgMethod():
	m_this(NULL)
{
}

JXJ::crJXJRecvPingMsgMethod::crJXJRecvPingMsgMethod( const crJXJRecvPingMsgMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{
}

void JXJ::crJXJRecvPingMsgMethod::operator()( crHandle &handle )
{
	if (m_stream.valid() && m_this && m_this->getDataClass())
	{
		if (m_netType == GameClient_Game)
		{
			std::string chatText = m_stream->_readString();
			std::string keyStr = "/jxjping";

			if (chatText.find(keyStr) == 0)
			{
				int nBegin = keyStr.length();
				int nEnd = chatText.find_first_of(' ');
				keyStr = chatText.substr(nBegin,nEnd - nBegin);
				chatText.erase(nBegin,nEnd - nBegin);

				CRCore::Timer_t start_tick  = crArgumentParser::appAtoI64(keyStr);
				CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();

			//	keyStr = " gateway ";
			//	nBegin = chatText.find(keyStr);
			//	if ( nBegin != std::string::npos)
			//	{
			//		nBegin = nBegin + keyStr.length();
			//		nEnd = chatText.length();
			//		keyStr = chatText.substr(nBegin ,nEnd - nBegin);
			//		chatText.erase(nBegin,nEnd - nBegin);

			//		std::string tmpStr = chatText;

			//		CRCore::Timer_t end_tick2 = crArgumentParser::appAtoI64(keyStr);

			//		ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
			//		record->setChatChannelName("[系统]");
			//		chatText = chatText + " up " + crArgumentParser::appFtoa (CRCore::Timer::instance()->delta_m(start_tick,end_tick2));
			//		record->setChatText(chatText);
			//		crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));

			//		chatText = tmpStr;
			//		record = new crJXJChatRecord();
			//		record->setChatChannelName("[系统]");
			//		chatText = chatText + " down " + crArgumentParser::appFtoa (CRCore::Timer::instance()->delta_m(end_tick2,end_tick));
			//		record->setChatText(chatText);
			//		crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));

			//		chatText = tmpStr;

			//	}
				chatText = chatText + " " + crArgumentParser::appFtoa (CRCore::Timer::instance()->delta_m(start_tick,end_tick));

				ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
				record->setChatChannelName("[系统]");
				record->setChatText(chatText);
				crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
			}
		}
	}
}

void JXJ::crJXJRecvPingMsgMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvPingMsgMethod::addParam( int i, const std::string& str )
{
	
}

/************************************************************************/
/*    crJXJStartFuBenConfirmMethod                                                                  */
/************************************************************************/
JXJ::crJXJStartFuBenConfirmMethod::crJXJStartFuBenConfirmMethod():
	m_this(NULL)
{
}

JXJ::crJXJStartFuBenConfirmMethod::crJXJStartFuBenConfirmMethod( const crJXJStartFuBenConfirmMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_groupCanvas(handle.m_groupCanvas),
	m_idx(handle.m_idx),
	m_canvas(handle.m_canvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{
}

void JXJ::crJXJStartFuBenConfirmMethod::inputParam( int i, void *param )
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

void JXJ::crJXJStartFuBenConfirmMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_groupCanvas = str;
		break;
	case 1:
		m_idx = (unsigned char )atoi(str.c_str());
		break;
	case 2:
		m_canvas = str;
		break;
	case 3:
		m_information = str;
		break;
	case 4:
		m_okbtn = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJStartFuBenConfirmMethod::operator()( crHandle &handle )
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_groupCanvas);
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (playerGameData && playerGameData->getDataClass() && groupCanvas.valid() && m_this && groupCanvas->getDataClass())
	{
		crData * data = playerGameData->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);

		crData * groupData = groupCanvas->getDataClass();
		CRCore::ScopedLock<crData> lock2(*groupData);

		void *param;
		groupData->getParam(WCHDATA_JXJMyGroupInfo,param);
		crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;

		data->getParam(WCHDATA_JXJGiftGold,param);
		int giftgold = *(int *)param;

		data->getParam(WCHDATA_Money,param);
		int money = *(int *)param;

		ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		crTableIO::StrVec strVec;
		int fundsCostIdx = fubenTab->getTitleIndex("消耗资金");
		int taoFaDianIdx = fubenTab->getTitleIndex("征伐值");
		unsigned char maxGroupLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaMaxLevel).c_str());
		if (myGroup->getLv() <= maxGroupLevel)
		{
			//bool isFullLevel = false;
			//if (myGroup->getLv() == maxGroupLevel)
			//{
			//	isFullLevel = true;
			//}
			crTableIO::DataVec  dataVec;
			int fubenType = 1;
			if (fubenTab.valid() )
			{
				int fubenIdIdx = fubenTab->getTitleIndex("id");
				int chapterIdx = fubenTab->getTitleIndex("chapterid");
				fubenTab->queryRecords(chapterIdx,crArgumentParser::appItoa(fubenType),dataVec);

				groupData->getParam(WCHDATA_JXJMyGroupFuBenCurPage,param);
				int fuBenCurPage = *(int *)param;

				if (fuBenCurPage + m_idx - 1 < dataVec.size())
				{
					int needTaoFaDian = atoi(dataVec[fuBenCurPage + m_idx - 1][taoFaDianIdx].c_str());
					int needFunds = atoi(dataVec[fuBenCurPage + m_idx - 1][fundsCostIdx].c_str());
					if (giftgold >= needTaoFaDian || money >= needTaoFaDian /*|| (isFullLevel && myGroup->getFunds() >= needFunds)*/)
					{
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
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
						cfg_script.Add("Text","您确认要花费");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(140.0f);
						v_i.push_back(40.0f);
						v_i.push_back(40.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						char tmpText[32];
						//if (isFullLevel)
						//{
						//	if (myGroup->getFunds()-needFunds>=6000)
						//	{
						//		sprintf(tmpText,"%d 军团资金\0",needFunds);
						//	}
						//	else
						//	{
						//		sprintf(tmpText,"%d 讨伐点\0",needTaoFaDian);
						//	}
						//	//sprintf(tmpText,"%d 军团资金 或者 %d 元宝\0",needExp,needMoney);

						//}
						//else
						//{
							sprintf(tmpText,"%d讨伐点\0",needTaoFaDian);
						//}
						cfg_script.Add("Text",tmpText);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						v_i.clear();
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","开启该副本吗？");
						cfg_script.Pop();

						cfg_script.Pop();

						if(information.valid())information->setHypertext(cfg_script);
						if(okbtn.valid())okbtn->setEnable(true);
						crData *dlgData = canvas->getDataClass();
						int commandtype = CDP_Widget;
						dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
						crFilterRenderManager::getInstance()->doModal(canvas.get());
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(4070,NULL));
					}
				}
			}
		}
	}
}


/************************************************************************/
/*  crJXJExtraCritPercentFixedMethod                                                                    */
/************************************************************************/
crJXJExtraCritPercentFixedMethod::crJXJExtraCritPercentFixedMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraCritPercentFixedMethod::crJXJExtraCritPercentFixedMethod(const crJXJExtraCritPercentFixedMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraCritPercentFixedMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraCritPercentFixedMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraCritPercentFixedMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_InbornCritPower,param);
		short* extra = (short *)param;
		//*extra += m_extraValue;
		*extra = m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
		//if(m_timer<0.0f)
		//{
		//	m_start = false;
		//	m_timer = 0.0f;
		//}
	}
}


/************************************************************************/
/*  crJXJExtraSightRangeMethod                                                                    */
/************************************************************************/
crJXJExtraSightRangeMethod::crJXJExtraSightRangeMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraSightRangeMethod::crJXJExtraSightRangeMethod(const crJXJExtraSightRangeMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraSightRangeMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraSightRangeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraSightRangeMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_ExtraSightRange,param);
		short* extra = (short *)param;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}


/************************************************************************/
/*   crJXJExtraStealthMethod                                                                   */
/************************************************************************/
crJXJExtraStealthMethod::crJXJExtraStealthMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraStealthMethod::crJXJExtraStealthMethod(const crJXJExtraStealthMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraStealthMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraStealthMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraStealthMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_ExtraStealth,param);
		short* extra = (short *)param;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}


/************************************************************************/
/*   crJXJExtraStealthSpyMethod                                                                   */
/************************************************************************/
crJXJExtraStealthSpyMethod::crJXJExtraStealthSpyMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraStealthSpyMethod::crJXJExtraStealthSpyMethod(const crJXJExtraStealthSpyMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraStealthSpyMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraStealthSpyMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraStealthSpyMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		//m_extraIncreesValue = 0;
	}
	if(m_timer>=0.0f)
	{
		//可叠加写法
		//m_extraIncreesValue+=m_extraValue;
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_ExtraStealthSpy,param);
		short* extra = (short *)param;
		//*extra += m_extraIncreesValue;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}


/************************************************************************/
/*   crJXJIgnoreDodgeMethod                                                                   */
/************************************************************************/
crJXJIgnoreDodgeMethod::crJXJIgnoreDodgeMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJIgnoreDodgeMethod::crJXJIgnoreDodgeMethod(const crJXJIgnoreDodgeMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJIgnoreDodgeMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJIgnoreDodgeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	//case 1:
	//	m_extraValue = (short)(atoi(str.c_str()));
	//	break;
	}
}

void crJXJIgnoreDodgeMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_InbornIgnoreDodge,param);
		unsigned char *ignoreDodge = (unsigned char *)param;
		if (*ignoreDodge == 0)
		{
			*ignoreDodge = 1;
		}
		thisData->excHandle(MAKEINT64(WCH_LockData,0));
		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}

/************************************************************************/
/*    crGetRadioGroupValue2Method                                                                  */
/************************************************************************/
JXJ::crGetRadioGroupValue2Method::crGetRadioGroupValue2Method()
{
}

JXJ::crGetRadioGroupValue2Method::crGetRadioGroupValue2Method( const crGetRadioGroupValue2Method& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_radioGroup(handle.m_radioGroup)
{

}

void JXJ::crGetRadioGroupValue2Method::inputParam( int i, void *param )
{

}

void JXJ::crGetRadioGroupValue2Method::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_radioGroup = str;
		break;
	default:
		break;
	}
}

void JXJ::crGetRadioGroupValue2Method::operator()( crHandle &handle )
{
	int value = 0;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		ref_ptr<crRadioGroupWidgetNode> groupradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioGroup));
		if (groupradio.valid())
		{
			value = groupradio->getSelect();
		}
	}
	handle.outputParam(0,&value);
}

///************************************************************************/
///*  crJXJExtraOverlapMethod                                                                    */
///************************************************************************/
//crJXJExtraOverlapMethod::crJXJExtraOverlapMethod():
//	m_times(0){}
//crJXJExtraOverlapMethod::crJXJExtraOverlapMethod(const crJXJExtraOverlapMethod& handle):
//	crMethod(handle),
//	m_times(0)
//{
//}
/************************************************************************/
/*  crJXJExtraAttackSpeedOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraAttackSpeedOverlapMethod::crJXJExtraAttackSpeedOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraAttackSpeedOverlapMethod::crJXJExtraAttackSpeedOverlapMethod(const crJXJExtraAttackSpeedOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraAttackSpeedOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraAttackSpeedOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraAttackSpeedOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraAttackSpeed,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}



/************************************************************************/
/*  crJXJExtraMoveSpeedOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraMoveSpeedPercentOverlapMethod::crJXJExtraMoveSpeedPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraMoveSpeedPercentOverlapMethod::crJXJExtraMoveSpeedPercentOverlapMethod(const crJXJExtraMoveSpeedPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraMoveSpeedPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraMoveSpeedPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraMoveSpeedPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraSpeedPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraAttackPercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraAttackPercentOverlapMethod::crJXJExtraAttackPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraAttackPercentOverlapMethod::crJXJExtraAttackPercentOverlapMethod(const crJXJExtraAttackPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraAttackPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraAttackPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraAttackPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraAttackPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}



/************************************************************************/
/*  crJXJExtraBuildingAttackPercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraBuildingAttackPercentOverlapMethod::crJXJExtraBuildingAttackPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraBuildingAttackPercentOverlapMethod::crJXJExtraBuildingAttackPercentOverlapMethod(const crJXJExtraBuildingAttackPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraBuildingAttackPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraBuildingAttackPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraBuildingAttackPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraBuildAttackPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}


/************************************************************************/
/*  crJXJExtraParrayPercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraParrayPercentOverlapMethod::crJXJExtraParrayPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraParrayPercentOverlapMethod::crJXJExtraParrayPercentOverlapMethod(const crJXJExtraParrayPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraParrayPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraParrayPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraParrayPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraParryPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraDodgePercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraDodgePercentOverlapMethod::crJXJExtraDodgePercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
	m_interval(1.0f){}
crJXJExtraDodgePercentOverlapMethod::crJXJExtraDodgePercentOverlapMethod(const crJXJExtraDodgePercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraDodgePercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraDodgePercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraDodgePercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraDodgePercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraHitRatioPercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraHitRatioPercentOverlapMethod::crJXJExtraHitRatioPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraHitRatioPercentOverlapMethod::crJXJExtraHitRatioPercentOverlapMethod(const crJXJExtraHitRatioPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraHitRatioPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraHitRatioPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraHitRatioPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_times+=1;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraHitratioPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraCritPercentOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraCritPercentOverlapMethod::crJXJExtraCritPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraCritPercentOverlapMethod::crJXJExtraCritPercentOverlapMethod(const crJXJExtraCritPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraCritPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraCritPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraCritPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_times+=1;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraCritPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraAttackDistOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraAttackDistOverlapMethod::crJXJExtraAttackDistOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraAttackDistOverlapMethod::crJXJExtraAttackDistOverlapMethod(const crJXJExtraAttackDistOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraAttackDistOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraAttackDistOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraAttackDistOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_times+=1;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraAttackDist,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraIgnoreDefendOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraIgnoreDefendOverlapMethod::crJXJExtraIgnoreDefendOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraIgnoreDefendOverlapMethod::crJXJExtraIgnoreDefendOverlapMethod(const crJXJExtraIgnoreDefendOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraIgnoreDefendOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraIgnoreDefendOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraIgnoreDefendOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
		//m_times = 0;
		//m_times+=1;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraIgnoreDefend,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraIgnoreMagicOverlapMethod                                                                    */
/************************************************************************/

crJXJExtraIgnoreMagicOverlapMethod::crJXJExtraIgnoreMagicOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraIgnoreMagicOverlapMethod::crJXJExtraIgnoreMagicOverlapMethod(const crJXJExtraIgnoreMagicOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraIgnoreMagicOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraIgnoreMagicOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraIgnoreMagicOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraIgnoreMagicDefend,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraAttackDamageBackOverlapMethod*/
/************************************************************************/

crJXJExtraAttackDamageBackOverlapMethod::crJXJExtraAttackDamageBackOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraAttackDamageBackOverlapMethod::crJXJExtraAttackDamageBackOverlapMethod(const crJXJExtraAttackDamageBackOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraAttackDamageBackOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraAttackDamageBackOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraAttackDamageBackOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraAttackDamageBack ,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraSkillDamageBackOverlapMethod*/
/************************************************************************/

crJXJExtraSkillDamageBackOverlapMethod::crJXJExtraSkillDamageBackOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraSkillDamageBackOverlapMethod::crJXJExtraSkillDamageBackOverlapMethod(const crJXJExtraSkillDamageBackOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraSkillDamageBackOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraSkillDamageBackOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraSkillDamageBackOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_times+=1;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraSkillDamageBack  ,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraSkillAbsorbHPOverlapMethod*/
/************************************************************************/

crJXJExtraSkillAbsorbHPOverlapMethod::crJXJExtraSkillAbsorbHPOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraSkillAbsorbHPOverlapMethod::crJXJExtraSkillAbsorbHPOverlapMethod(const crJXJExtraSkillAbsorbHPOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraSkillAbsorbHPOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraSkillAbsorbHPOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraSkillAbsorbHPOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraSkillAbsorbHP  ,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraAttackAbsorbHPOverlapMethod*/
/************************************************************************/

crJXJExtraAttackAbsorbHPOverlapMethod::crJXJExtraAttackAbsorbHPOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraAttackAbsorbHPOverlapMethod::crJXJExtraAttackAbsorbHPOverlapMethod(const crJXJExtraAttackAbsorbHPOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraAttackAbsorbHPOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraAttackAbsorbHPOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraAttackAbsorbHPOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_times+=1;
			//m_start = false;
			//m_timer = 0.0f;

		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraAttackAbsorbHP  ,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*  crJXJExtraMagicDefendOverlapMethod*/
/************************************************************************/

crJXJExtraMagicDefendPercentOverlapMethod::crJXJExtraMagicDefendPercentOverlapMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0),
m_interval(1.0f){}
crJXJExtraMagicDefendPercentOverlapMethod::crJXJExtraMagicDefendPercentOverlapMethod(const crJXJExtraMagicDefendPercentOverlapMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_addValue(0)
{
}
void crJXJExtraMagicDefendPercentOverlapMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraMagicDefendPercentOverlapMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraMagicDefendPercentOverlapMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
		m_addValue = 0;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_timer<0.0f)
		{
			m_dtrec += m_timer;
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec = 0.0f;
			//m_start = false;
			//m_timer = 0.0f;
			//m_times+=1;
		}
		else if(m_dtrec>=m_interval)
		{
			m_addValue += m_extraValue * m_dtrec;
			m_dtrec -= m_interval;
			//m_times+=1;
		}
		if(m_addValue!=0)
		{
			void *param;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_ExtraMagicDefendPercent,param);
			short* extra = (short *)param;
			*extra += m_addValue;
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJSetAndCheckPreSelectedUIEarningsMethod                                                                  */
/************************************************************************/
JXJ::crJXJSetAndCheckPreSelectedUIEarningsMethod::crJXJSetAndCheckPreSelectedUIEarningsMethod():
	m_this(NULL)
{
}

JXJ::crJXJSetAndCheckPreSelectedUIEarningsMethod::crJXJSetAndCheckPreSelectedUIEarningsMethod( const crJXJSetAndCheckPreSelectedUIEarningsMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_groupList(handle.m_groupList),
	m_confirm(handle.m_confirm),
	m_staticText(handle.m_staticText)
{
	for (unsigned char i = 0; i < 9; ++ i)
	{
		m_progressBar[i] = handle.m_progressBar[i];
		m_edit[i] = handle.m_edit[i];
	}
}

void JXJ::crJXJSetAndCheckPreSelectedUIEarningsMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSetAndCheckPreSelectedUIEarningsMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_groupList =str;
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
		m_edit[i - 2] = str;
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_progressBar[i - 11] = str;
		break;
	case 20:
		m_staticText = str;
		break;
	case 21:
		m_confirm = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJSetAndCheckPreSelectedUIEarningsMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && canvas->getDataClass())
		{
			ref_ptr<crEditWidgetNode> inputEdit[9] = {NULL}; 
			ref_ptr<crProgressWidgetNode>  progressBar[9] = {NULL};
			ref_ptr<crButtonWidgetNode> confirmBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_confirm));
			ref_ptr<crStaticTextWidgetNode> staticText = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_staticText));

			for (unsigned char i = 0; i < 9; ++ i)
			{
				inputEdit[i] = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_edit[i]));
				progressBar[i] = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progressBar[i]));
			}

			ref_ptr<crTableWidgetNode> groupList = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_groupList));

			do 
			{
				if (groupList.valid() == false) break;
				crData * canvasData = canvas->getDataClass();
				CRCore::ScopedLock<crData> lock2(*canvasData);

				void * param;
				canvasData->getParam(WCHDATA_JXJCantonGroupVec,param);
				CantonGroupVec & cantonGroupVec = *(CantonGroupVec *)param;

				if (cantonGroupVec.empty()) break;

				PermissionCheck check;

				canvasData->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
				crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
				int myGroupPos = JXJGroup_Normal;
				if (mySelfInfo) myGroupPos = mySelfInfo->getGroupPos();


				canvasData->getParam(WCHDATA_JXJAllocateCantonVec,param);
				MyAllocatedCantonVec * myAllocatedCantonVec = (MyAllocatedCantonVec *)param;
				if (myAllocatedCantonVec->empty()) break;

				if (!check(myGroupPos,JXJGroup_CantonManage)) break;

				canvasData->getParam(WCHDATA_JXJPreSelectedIdx,param);
				int preSelectedIdx = *(int *)param;
				int curSelectIdx = groupList->getSelect();

				if (preSelectedIdx == -1)
				{
					canvasData->inputParam(WCHDATA_JXJPreSelectedIdx,&curSelectIdx);
				}
				else 
				{
					do 
					{
						if ( preSelectedIdx < 0 || preSelectedIdx >= 9
							|| preSelectedIdx + 1 >= cantonGroupVec.size()
							|| inputEdit[preSelectedIdx]->getVisiable() == false) break;

						float inputVal = atof(inputEdit[preSelectedIdx]->getUTF8String().c_str());
						short changeVal = short (inputVal * 10) - cantonGroupVec[preSelectedIdx + 1]->m_UIearnings;

						if (cantonGroupVec[0]->m_UIearnings < changeVal
							|| cantonGroupVec[0]->m_UIearnings > 1000 + changeVal
							|| inputVal < 0.0f || inputVal > 100.0f)
						{
							break;
						}
						cantonGroupVec[0]->m_UIearnings -= changeVal;

						if (staticText.valid())
						{
							staticText->setString(crArgumentParser::appFtoa(cantonGroupVec[0]->m_UIearnings / 10.0) + "%");
						}

						cantonGroupVec[preSelectedIdx + 1]->m_UIearnings = inputVal * 10;

						if (inputEdit[preSelectedIdx].valid())
							inputEdit[preSelectedIdx]->setVisiable(false);
						if (progressBar[preSelectedIdx].valid())
						{
							progressBar[preSelectedIdx]->setVisiable(true);
							progressBar[preSelectedIdx]->setProgress(inputVal / 100);
						}
					} while (0);
				}

				if (curSelectIdx < 0 
					|| curSelectIdx >= 9
					|| curSelectIdx + 1 >= cantonGroupVec.size()
					|| (preSelectedIdx == curSelectIdx && inputEdit[curSelectIdx]->getVisiable()))
				{
					break;
				}
				canvasData->inputParam(WCHDATA_JXJPreSelectedIdx,&curSelectIdx);

				if (inputEdit[curSelectIdx ].valid())
				{
					inputEdit[curSelectIdx ]->setVisiable(true);
					inputEdit[curSelectIdx]->setString(crArgumentParser::appFtoa(cantonGroupVec[curSelectIdx + 1]->m_UIearnings / 10.0));
				}
				if (progressBar[curSelectIdx].valid())
				{
					progressBar[curSelectIdx]->setVisiable(false);
				}
				if (confirmBtn.valid())
				{
					confirmBtn->setVisiable(true);
					confirmBtn->setEnable(true);
				}
			} while (0);
		}
	}
}

/************************************************************************/
/*    crJXJUIFuBenSettleTimerUpdateMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIFuBenSettleTimerUpdateMethod::crJXJUIFuBenSettleTimerUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIFuBenSettleTimerUpdateMethod::crJXJUIFuBenSettleTimerUpdateMethod( const crJXJUIFuBenSettleTimerUpdateMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_time(handle.m_time),
	m_timerEdit(handle.m_timerEdit),
	m_op(handle.m_op )
{

}

void JXJ::crJXJUIFuBenSettleTimerUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIFuBenSettleTimerUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_timerEdit = str;
		break;
	case 2:
		m_op = (unsigned char)atoi(str.c_str());
		break;
	case 3:
		m_time = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJUIFuBenSettleTimerUpdateMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		crData * canvasData = canvas->getDataClass();
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		void *param;
		canvasData->getParam(WCHDATA_JXJGroupFuBenLoseTime,param);
		unsigned char loseTime = *(unsigned char *)param;
		if (m_op)
		{
			-- loseTime ;
		}
		else 
		{
			loseTime = m_time;
		}
		canvasData->inputParam(WCHDATA_JXJGroupFuBenLoseTime,&loseTime);
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));

		ref_ptr<crStaticTextWidgetNode > timeEdit = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_timerEdit));
		if (timeEdit.valid())
		{
			timeEdit->setString(crArgumentParser::appItoa(loseTime));
		}
	}
}

crJXJClearTargetExtraMethod::crJXJClearTargetExtraMethod():
	m_exceptBufType(-1)
{}
crJXJClearTargetExtraMethod::crJXJClearTargetExtraMethod(const crJXJClearTargetExtraMethod& handle):
	m_exceptBufType(handle.m_exceptBufType),
	crMethod(handle)
{
}
void crJXJClearTargetExtraMethod::inputParam(int i, void *param)
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
void crJXJClearTargetExtraMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_exceptBufType = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJClearTargetExtraMethod::operator()(crHandle &handle)
{
	if(m_hitParam.valid() && m_hitParam->m_fireItem.valid() && m_hitParam->m_hitItem.valid())
	{
		///ClearTargetExtra
		//m_hitParam->m_hitItem->clearExtra();
		///////清楚负面BUF
		void *param;
		crData *data = m_hitParam->m_hitItem->getDataClass();
		data->getParam(WCHDATA_ExtraData,param);
		if(param)
		{
			crData *extraData = (crData *)param;
			crHandle *handle;
			extraData->excHandle(MAKEINT64(WCH_LockData,1));

			ref_ptr<crTableIO> bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);
			crTableIO::StrVec record;
			int isNegativeIdx = -1;
			if (bufferAttrTab.valid()) isNegativeIdx = bufferAttrTab->getTitleIndex("Buf类型");

			extraData->getParam(WCHDATA_ExtraIDMap,param);
			ExtraIDMap *extraIDMap = (ExtraIDMap *)param;
			for( ExtraIDMap::iterator itr = extraIDMap->begin();
				itr != extraIDMap->end(); )
			{
				std::string extraValue = crArgumentParser::appItoa(itr->first)+std::string("|")+crArgumentParser::appItoa(HIINT16(itr->second));
				if(bufferAttrTab.valid() && bufferAttrTab->queryOneRecord(0,extraValue,record)>=0 && isNegativeIdx >= 0)
				{
					if (atoi(record[isNegativeIdx].c_str()) == m_exceptBufType)
					{
						if(crGlobalHandle::isClient())
						{
							handle = extraData->getHandle(MAKEINT64(WCH_DoExtra,itr->first));
							if(handle)
							{
								handle->releaseObjects(NULL);
							}
						}
						extraData->removeHandle(MAKEINT64(WCH_DoExtra,itr->first));
						itr = extraIDMap->erase(itr);
					}
					else
					{
						++itr;
					}
				}
				else ++ itr;
			}
			extraData->excHandle(MAKEINT64(WCH_LockData,0));

			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			if (sceneServerConductor)
			{
				crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
				crNetManager *netManager = sceneServerConductor->getNetManager();
				crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
				if(scene)
				{
					if(m_hitParam->m_hitItem->getItemtype() == crInstanceItem::Role)
					{
						int hitid = m_hitParam->m_hitItem->getID();
						crPlayerEventPacket clearExtraPacket;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1);
						stream->_writeChar(m_exceptBufType);
						crPlayerEventPacket::buildRequestPacket(clearExtraPacket,hitid,m_hitParam->m_hitItem.get(),WCH_ClearTargetExtra	,stream.get());
						ref_ptr<crSceneServerPlayerData> hitPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(hitid));
						if (hitPlayerData.valid())
						{
							netManager->sendPacket(hitPlayerData->getPlayerConnectServerAddress(),clearExtraPacket);
						}
						scene->sendPacketToItemNeighbor(m_hitParam->m_hitItem.get(),clearExtraPacket);
					}
				}
			}
		}
	}
}

/************************************************************************/
/*    crJXJGroupResetGroupIdMethod                                                                  */
/************************************************************************/
JXJ::crJXJGroupResetGroupIdMethod::crJXJGroupResetGroupIdMethod():
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

JXJ::crJXJGroupResetGroupIdMethod::crJXJGroupResetGroupIdMethod( const crJXJGroupResetGroupIdMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL),
	m_netType(GameClient_Game)
{

}

void JXJ::crJXJGroupResetGroupIdMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGroupResetGroupIdMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJGroupResetGroupIdMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid() && m_this->getDataClass())
	{
		crData * data = m_this->getDataClass();
		int newGroupId = m_stream->_readInt();
		data->inputParam(WCHDATA_JXJConsortiaID,&newGroupId);
	}
}

/************************************************************************/
/*   crJXJClearSkillCDMethod                                                                   */
/************************************************************************/
crJXJClearSkillCDMethod::crJXJClearSkillCDMethod():
m_this(NULL),
m_flg(1),
m_hitParam(NULL){}
crJXJClearSkillCDMethod::crJXJClearSkillCDMethod(const crJXJClearSkillCDMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_hitParam(NULL),
	m_flg(handle.m_flg),
	m_exceptVec(handle.m_exceptVec)
{
}
void crJXJClearSkillCDMethod::inputParam(int i, void *param)
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
void crJXJClearSkillCDMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_flg = atoi(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_exceptVec);
		break;
	default:
		break;
	}
}
void crJXJClearSkillCDMethod::operator()(crHandle &handle)
{
	if(m_hitParam.valid())
	{
		ref_ptr<crInstanceItem> item = m_flg == 0 ? m_hitParam->m_fireItem : m_hitParam->m_hitItem;
		if (item.valid() && item->getDataClass())
		{
			crData *data = item->getDataClass();
			void * param = NULL;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_EquipOnMap,param);
			EquipOnMap equipOnMap = *(EquipOnMap *)param;
			crItemChild *itemChild;
			crInstanceItem *equip;
			crData * equipData = NULL;
			int itemId = 0;
			_crInt64  lastUseTime = 0;
			std::vector<int> ClearVec;
			for( EquipOnMap::iterator itr = equipOnMap.begin();
				itr != equipOnMap.end();
				++itr )
			{
				itemChild = item->findChildItem(itr->second);
				if(itemChild && itemChild->isLoaded())
				{
					equip = itemChild->getInstanceItem();
					if (isExceptCard(equip->getAbstractItemID())) continue;
					equipData = equip->getDataClass();
					if (equipData)
					{
						equipData->inputParam(WCHDATA_LastUseTime,&lastUseTime);
						ClearVec.push_back(itr->second);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			if (!ClearVec.empty())
			{
				crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				if (sceneServerConductor)
				{
					crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
					crNetManager *netManager = sceneServerConductor->getNetManager();
					crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
					CRNetApp::crScene *scene = netCallback->findScene(item->getSceneID());
					if (scene)
					{
						if (item->getItemtype() == crInstanceItem::Role)
						{
							int hitid = item->getID();
							int count = ClearVec.size();
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(count * 4+1);
							stream->_writeUChar(count);
							for (std::vector<int>::iterator itr = ClearVec.begin(); itr != ClearVec.end(); ++itr)
							{
								stream->_writeInt(*itr);
							}
							crPlayerEventPacket clearCDPacket;
							crPlayerEventPacket::buildRequestPacket(clearCDPacket, hitid, item.get(), WCH_JXJRecvClearSkillCD, stream.get());
							ref_ptr<crSceneServerPlayerData> hitPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(hitid));
							if (hitPlayerData.valid())
							{
								netManager->sendPacket(hitPlayerData->getPlayerConnectServerAddress(), clearCDPacket);
							}
							scene->sendPacketToItemNeighbor(item.get(), clearCDPacket);
						}
					}
				}
			}
		}
	}
}

bool JXJ::crJXJClearSkillCDMethod::isExceptCard( int itemId )
{
	if (m_exceptVec.empty()) return false;

	for (ExceptCardVec::iterator ecItr = m_exceptVec.begin();
		ecItr != m_exceptVec.end(); 
		++ ecItr ) 
	{
		if (*ecItr == itemId)
		{
			return true;
		}
	}
	return false;
}

/************************************************************************/
/*    crJXJFPSAndPingUpdateMethod                                                                  */
/************************************************************************/
bool crJXJFPSAndPingUpdateMethod::s_recored = false;
JXJ::crJXJFPSAndPingUpdateMethod::crJXJFPSAndPingUpdateMethod():
	m_this(NULL)
{
}

JXJ::crJXJFPSAndPingUpdateMethod::crJXJFPSAndPingUpdateMethod( const crJXJFPSAndPingUpdateMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_fps(handle.m_fps),
	m_netPing(handle.m_netPing),
	m_url(handle.m_url),
	m_type(handle.m_type),
	m_xumusceneid(handle.m_xumusceneid)
{
}

void JXJ::crJXJFPSAndPingUpdateMethod::inputParam( int i, void *param )
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

void JXJ::crJXJFPSAndPingUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_fps = str;
		break;
	case 2:
		m_netPing = str;
		break;
	case 3:
		m_url = str;
		break;
	case 4:
		m_type = str;
		break;
	case 5:
		m_xumusceneid = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJFPSAndPingUpdateMethod::operator()( crHandle &handle )
{
	int curFPS = crFrameStamp::getInstance()->getFPS();
	float m_ping = 0.0f;
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	/*ref_ptr<crCanvasNode> canvas1 = crFilterRenderManager::getInstance()->findCanvas("UI_Show");
	ref_ptr<crCheckBoxWidgetNode >  settingBox = NULL;
	if(canvas1.valid())
	settingBox = dynamic_cast<crCheckBoxWidgetNode*>(canvas1->getWidget("ShowSetting"));
	if (settingBox.valid() && canvas.valid())
	{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
	void *param;
	crData *playerData = myPlayer->getDataClass();
	playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
	unsigned int goldcosthint = *(unsigned int *)param;
	int i = 20;
	if (goldcosthint & (1 << i))
	{
	settingBox->select(false);
	canvas->setVisiable(false);
	}
	else
	{
	settingBox->select(true);
	canvas->setVisiable(true);
	}
	}

	if(!settingBox->getSelect())
	canvas->setVisiable(false);
	else
	canvas->setVisiable(true);
	}*/
	
	if (canvas.valid() && canvas->getDataClass())
	{
		crData * canvasData = canvas->getDataClass();
		canvasData->getParam(WCHDATA_JXJPingVal,param);
		m_ping = *(float *)param;
		ref_ptr<crStaticTextWidgetNode > fps = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_fps));
		ref_ptr<crStaticTextWidgetNode > pingVal = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_netPing));
		if (fps.valid())
		{
			fps->setString("FPS: " + crArgumentParser::appItoa(curFPS));
		}
		if (pingVal.valid())
		{
			pingVal->setString("Ping: " + crArgumentParser::appItoa(m_ping) + " ms");
		}

		crJXJPingPacket packet;
		crJXJPingPacket::buildRequestPacket(packet);
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor) netConductor->getNetManager()->sendPacket("all",packet);

		CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
		if(!s_recored && scene && scene->getSceneID() == m_xumusceneid && curFPS>0 && m_ping>0 && crMyPlayerData::getInstance()->getSelectedGame()->getPlatform().find("QQ") != std::string::npos)
		{//QQ服序幕FPS记录
			s_recored = true;
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			int serverid = crMyPlayerData::getInstance()->getLastServerID();
			int playerID = myPlayer?myPlayer->getPlayerID():0;

			HINTERNET hIntSession = InternetOpen("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

			HINTERNET hHttpSession = InternetConnect(hIntSession, m_url.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
			std::string str = "index.php/index/gamerecord?playerid="+crArgumentParser::appItoa(playerID)+"&type="+m_type+"&value="+crArgumentParser::appItoa(curFPS)+"&serverid="+crArgumentParser::appItoa(serverid);
			HINTERNET hHttpRequest = HttpOpenRequest(
				hHttpSession, 
				"GET", 
				str.c_str(),
				0, 0, 0, INTERNET_FLAG_RELOAD, 0);

			char* szHeaders = "Content-Type: text/html";
			char szReq[64] = "";
			if( !HttpSendRequest(hHttpRequest, szHeaders, strlen(szHeaders), szReq, strlen(szReq))) 
			{
				DWORD dwErr = GetLastError();
				/// handle error
			}

			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hIntSession);
		}
	}
}

/************************************************************************/
/*    crJXJRecvPingRetMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvPingRetMethod::crJXJRecvPingRetMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvPingRetMethod::crJXJRecvPingRetMethod( const crJXJRecvPingRetMethod & handle ):
	crMethod(handle),
	m_manorCanvas(handle.m_manorCanvas),
	m_battleCanvas(handle.m_battleCanvas)
{

}

void JXJ::crJXJRecvPingRetMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvPingRetMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case  0:
		m_manorCanvas = str;
		break;
	case 1:
		m_battleCanvas = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvPingRetMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		ref_ptr<crCanvasNode> battleCanvas = crFilterRenderManager::getInstance()->findCanvas(m_battleCanvas);
		ref_ptr<crCanvasNode> manorCanvas = crFilterRenderManager::getInstance()->findCanvas(m_manorCanvas);
		ref_ptr<crCanvasNode> canvas = NULL;
		if (battleCanvas.valid() && battleCanvas->getDataClass())
		{
			canvas = battleCanvas;
		}
		else if (manorCanvas.valid() && manorCanvas->getDataClass())
		{
			canvas = manorCanvas;
		}
		if (canvas.valid())
		{
			crData * canvasData = canvas->getDataClass();
			
			CRCore::Timer_t start_tick  = m_stream->_readInt64();
			CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
			float pingVal = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
			canvasData->inputParam(WCHDATA_JXJPingVal,&pingVal);
		}
	}
}

/************************************************************************/
/*    crJXJRelocateFPSAndPingCanvasMethod                                                                  */
/************************************************************************/
JXJ::crJXJRelocateFPSAndPingCanvasMethod::crJXJRelocateFPSAndPingCanvasMethod():
	m_this(NULL)
{
}

JXJ::crJXJRelocateFPSAndPingCanvasMethod::crJXJRelocateFPSAndPingCanvasMethod( const crJXJRelocateFPSAndPingCanvasMethod & handle ):
	crMethod(handle),
	m_manorCanvas(handle.m_manorCanvas),
	m_battleCanvas(handle.m_battleCanvas)
{
}

void JXJ::crJXJRelocateFPSAndPingCanvasMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRelocateFPSAndPingCanvasMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_manorCanvas = str;
		break;
	case 1:
		m_battleCanvas = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJRelocateFPSAndPingCanvasMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> battleCanvas = crFilterRenderManager::getInstance()->findCanvas(m_battleCanvas);
		ref_ptr<crCanvasNode> manorCanvas = crFilterRenderManager::getInstance()->findCanvas(m_manorCanvas);
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Zhenglv");
		ref_ptr<crCanvasNode> canvas1 = crFilterRenderManager::getInstance()->findCanvas("UI_Zhenglv2");
		if (canvas.valid())
		{
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if(myPlayer)
			{
				char filePath[MAX_PATH];
				std::string strPath = crGlobalHandle::getInstance()->getGameDir();
				sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
				bool ischecked = ::GetPrivateProfileInt("frame","frameable",1,filePath);

				canvas->setVisiable(ischecked);
			}
		}
		if (canvas1.valid())
		{
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			if(myPlayer)
			{
				char filePath[MAX_PATH];
				std::string strPath = crGlobalHandle::getInstance()->getGameDir();
				sprintf(filePath,"%ssystem.ini\0",strPath.c_str());
				bool ischecked = ::GetPrivateProfileInt("frame","frameable",1,filePath);
				
				canvas1->setVisiable(ischecked);
			}
		}
		
		const crBoundingBox &thisBB = m_this->getBoundBox();
		float thisHeight = thisBB.yLength();
		float thisWidth = thisBB.xLength();
		crVector2f newCenter;
		newCenter[0] = 1.0f - thisWidth * 0.5f;
		float targetHeight  = 0.0f;
		if (manorCanvas.valid())
		{
			const crBoundingBox &manorBB = manorCanvas->getBoundBox();
			targetHeight = manorBB.yLength();
			if (targetHeight > 0.0f)
			{
				newCenter[1] = - (1.0f - targetHeight - thisHeight * 0.5f) ;

				m_this->setEnableMatrix(true);
				m_this->setMatrix(crMatrix ::translate(newCenter[0],newCenter[1],0.0f));
			}
		}
		//else if (battleCanvas.valid())
		//{
		//	const crBoundingBox &battleBB = battleCanvas->getBoundBox();
		//	targetHeight = battleBB.yLength();
		//}
		
	}
}

//void JXJ::JXJGroupKickeMemberMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
//{
//	Base::readContent(stream);
//	m_name = stream->_readString();
//}
//
//void JXJ::JXJGroupKickeMemberMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
//{
//	Base::writeContent(stream);
//	stream->_writeString(m_name);
//}

//void JXJ::JXJGroupKickeMemberMsg::getCompleteString( std::string & outString)
//{
//	CRCore::crHandle * noticeHandle = getNoticeHandle();
//	if (noticeHandle)
//	{
//		noticeHandle->inputParam(WCHDATA_NoticeParam1,&m_name);
//		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
//		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
//	}
//	else outString = "";
//}

/************************************************************************/
/*    crJXJUIFuBenSettleTimerCheckMethod                                                                  */
/************************************************************************/

JXJ::crJXJUIFuBenSettleTimerCheckMethod::crJXJUIFuBenSettleTimerCheckMethod():
	m_this(NULL)
{
}

JXJ::crJXJUIFuBenSettleTimerCheckMethod::crJXJUIFuBenSettleTimerCheckMethod( const crJXJUIFuBenSettleTimerCheckMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_receiveBtn(handle.m_receiveBtn)
{
}

void JXJ::crJXJUIFuBenSettleTimerCheckMethod::inputParam( int i, void *param )
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

void JXJ::crJXJUIFuBenSettleTimerCheckMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_receiveBtn = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJUIFuBenSettleTimerCheckMethod::operator()( crHandle &handle )
{
	bool ret = false;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crButtonWidgetNode> receiveBtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_receiveBtn));
	if (canvas.valid() && canvas->getDataClass() && canvas->getVisiable())
	{
		crData * canvasData = canvas->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJGroupFuBenLoseTime,param);
		unsigned char loseTime = *(unsigned char *)param;
		ret = loseTime == 0;
		if ((loseTime == 1) && receiveBtn.valid())
		{//loseTime 是用0初始化的，所以这里取在剩余1秒时就点领取按钮
			receiveBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
		}
	}
	handle.outputParam(0,&ret);
}

/************************************************************************/
/*    crJXJCheckIsJunTuanFuBenMethod                                                                  */
/************************************************************************/

JXJ::crJXJCheckIsJunTuanFuBenMethod::crJXJCheckIsJunTuanFuBenMethod()
{

}

JXJ::crJXJCheckIsJunTuanFuBenMethod::crJXJCheckIsJunTuanFuBenMethod( const crJXJCheckIsJunTuanFuBenMethod & handle ):
	crMethod(handle)
{
}

void JXJ::crJXJCheckIsJunTuanFuBenMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJCheckIsJunTuanFuBenMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJCheckIsJunTuanFuBenMethod::operator()( crHandle &handle )
{
	bool ret = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		unsigned short fubenid = *(unsigned short *)param;
		int fubenIdIdx = fubenTab->getTitleIndex("id");
		int chapterIdx = fubenTab->getTitleIndex("chapterid");
		crTableIO::StrVec record;
		if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
		{
			ret = atoi(record[chapterIdx].c_str()) == 1;
		}
	}
	handle.outputParam(0,&ret);
}

/************************************************************************/
/*    crJXJGroupCanvasInitMethod                                                                  */
/************************************************************************/
JXJ::crJXJGroupCanvasInitMethod::crJXJGroupCanvasInitMethod()
{

}

JXJ::crJXJGroupCanvasInitMethod::crJXJGroupCanvasInitMethod( const crJXJGroupCanvasInitMethod & handle )
{

}

void JXJ::crJXJGroupCanvasInitMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJGroupCanvasInitMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJGroupCanvasInitMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		crData * canvasData = canvas->getDataClass();
		if (canvasData)
		{
			int preSelectedIdx = -1;
			canvasData->inputParam(WCHDATA_JXJPreSelectedIdx,&preSelectedIdx);
		}
	}
}

///************************************************************************/
///*    crJXJHongdongUpdateMethod                                                                  */
///************************************************************************/
//JXJ::crJXJHuodongUpdateMethod::crJXJHuodongUpdateMethod():
//	m_dt(NULL)
//{
//
//}
//
//JXJ::crJXJHuodongUpdateMethod::crJXJHuodongUpdateMethod( const crJXJHuodongUpdateMethod & handle ):
//	m_dt(NULL),
//	m_canvas(handle.m_canvas)
//{
//
//}
//
//
//void JXJ::crJXJHuodongUpdateMethod::inputParam( int i, void *param )
//{
//	switch(i)
//	{
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			int lparam = LOINT64(param64);
//			m_dt = (float*)lparam;
//		}
//		else
//		{
//			m_dt = NULL;
//		}
//		break;
//	}
//}
//
//void JXJ::crJXJHuodongUpdateMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJHuodongUpdateMethod::operator()( crHandle &handle )
//{
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(myPlayer && myPlayer->getMainRole() && myPlayer->getMainRole()->getDataClass())
//	{
//		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//		if (canvas.valid() && canvas->getDataClass())
//		{
//			void *param;
//			crData *roleData = myPlayer->getMainRole()->getDataClass();
//			roleData->excHandle(MAKEINT64(WCH_LockData,1));
//			roleData->getParam(WCHDATA_JXJHuodongMap,param);
//			HuodongMap *huodongMap = (HuodongMap *)param;
//
//			//for(HuodongMap::iterator hmItr = huodongMap->begin();
//			//	huodongMap->end() != hmItr; ++ hmItr)
//			//{
//			//	if (hmItr != huodongMap->end())
//			//	{
//			//		hmItr->second->checkStartOrFinished(myPlayer->getPlayerID());
//			//		hmItr->second->doEvent(WCH_UPDATE,MAKEINT64(myPlayer,NULL));
//			//	}
//			//}
//			HuodongMap::iterator hmItr;
//			crData *canvasData = canvas->getDataClass();
//			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
//			canvasData->getParam(WCHDATA_JXJHuodongVec,param);
//			HuodongVec & huodongVec = *(HuodongVec *)param;
//			for (HuodongVec::iterator hvItr = huodongVec.begin();
//				hvItr != huodongVec.end(); ++ hvItr)
//			{
//				hmItr = huodongMap->find(*hvItr);
//				if (hmItr != huodongMap->end())
//				{
//					hmItr->second->checkStartOrFinished(myPlayer->getPlayerID());
//					hmItr->second->doEvent(WCH_UPDATE,MAKEINT64(myPlayer,NULL));
//				}
//			}
//			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
//
//			roleData->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//	}
//}
//
///************************************************************************/
///*    crJXJRecvHoudongChangeStatusMethod                                                                  */
///************************************************************************/
//
//JXJ::crJXJRecvHuodongChangeStatusMethod::crJXJRecvHuodongChangeStatusMethod():
//	m_time(5.0f),
//	m_netType(GameClient_Game),
//	m_this(NULL)
//{
//
//}
//
//JXJ::crJXJRecvHuodongChangeStatusMethod::crJXJRecvHuodongChangeStatusMethod( const crJXJRecvHuodongChangeStatusMethod & handle ):
//	crMethod(handle),
//	m_time(handle.m_time),
//	m_canvas(handle.m_canvas)
//{
//
//}
//
//void JXJ::crJXJRecvHuodongChangeStatusMethod::inputParam( int i, void *param )
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
//void JXJ::crJXJRecvHuodongChangeStatusMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJRecvHuodongChangeStatusMethod::operator()( crHandle &handle )
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			int playerid = m_this->getPlayerID();
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			crRole *mainRole = m_this->getMainRole();
//			if(playerData.valid() && mainRole && mainRole->getDataClass())
//			{
//				unsigned short accid = m_stream->_readShort();
//				void *param;
//				crData *data = mainRole->getDataClass();
//				data->excHandle(MAKEINT64(WCH_LockData,1));
//				data->getParam(WCHDATA_JXJHuodongMap	,param);
//				HuodongMap *huodongMap= (HuodongMap *)param;
//				HuodongMap ::iterator itr = huodongMap->find(accid);
//				if(itr != huodongMap->end())
//				{
//					itr->second->checkStartOrFinished(playerid);
//					itr->second->doEvent(WCH_JXJServerCompleteCheck,MAKEINT64(m_this,NULL));
//				}
//				data->excHandle(MAKEINT64(WCH_LockData,0));
//			}
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			crRole *mainRole = m_this->getMainRole();
//			if(mainRole && mainRole->getDataClass())
//			{
//				unsigned short accid = m_stream->_readUShort();
//				crJXJHuodong::CompleteStatus status = (crJXJHuodong::CompleteStatus) m_stream->_readUChar();
//				int tmpStep = m_stream->_readInt();
//				int tmpStepCount = m_stream->_readInt();
//				unsigned short usedTimes = m_stream->_readUShort();
//				void *param;
//				crData *data = mainRole->getDataClass();
//				data->excHandle(MAKEINT64(WCH_LockData,1));
//
//				data->getParam(WCHDATA_JXJHuodongMap,param);
//				HuodongMap * huodongMap = (HuodongMap *)param;
//				HuodongMap::iterator itr = huodongMap->find(accid);
//				if(itr != huodongMap->end())
//				{
//					itr->second->setComplete(status);
//					itr->second->setStep(tmpStep,m_this->getPlayerID(),false);
//					itr->second->setStepCount(tmpStepCount,m_this->getPlayerID(),false);
//					itr->second->setUsedTimes(usedTimes);
//
//					m_this->doEvent(WCH_JXJHuodongCanvasInit);
//					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//					if(canvas.valid())
//						canvas->doEvent(WCH_UI_UpdateData);
//
//					char gbuf[256];
//					sprintf(gbuf,"活动 %d 接收到服务器状态同步 complete %d, step %d, stepcount %d usedtimes %d\n\0",
//						accid,status,tmpStep,tmpStepCount,usedTimes);
//					gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
//				}
//				data->excHandle(MAKEINT64(WCH_LockData,0));
//			}
//		}
//	}
//}

/************************************************************************/
/*    crJXJRecvHoudongGetAwardMethod                                                                  */
/************************************************************************/
JXJ::crJXJRecvHuodongGetAwardMethod::crJXJRecvHuodongGetAwardMethod():
	m_netType(GameClient_Game),
	m_this(NULL)
{
}

JXJ::crJXJRecvHuodongGetAwardMethod::crJXJRecvHuodongGetAwardMethod( const crJXJRecvHuodongGetAwardMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_animation(handle.m_animation),
	m_sequence(handle.m_sequence) 
{

}

void JXJ::crJXJRecvHuodongGetAwardMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvHuodongGetAwardMethod::addParam( int i, const std::string& str )
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

void JXJ::crJXJRecvHuodongGetAwardMethod::operator()( crHandle &handle )
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
				unsigned short huodongID = m_stream->_readShort();

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

				CRCore::crVector3i awardVec;

				void *param;
				crData *thisData = m_this->getDataClass();
				crData *data = mainRole->getDataClass();
				thisData->excHandle(MAKEINT64(WCH_LockData,1));
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
							//do 
							//{
								//if (atoi(record[codeTypeIdx].c_str()))
								//{
								//	// 说明是领取的是激活码活动
								//	std::string activeCode = m_stream->_readString();
								//	ActiveCodeData activeCodeData ;
								//	activeCodeData.m_activeCode = activeCode;
								//	activeCodeData.m_huodongID = huodongID;
								//	m_this->doEvent(WCH_JXJGetAwardsByActiveCode,MAKEINT64(&activeCodeData,&logRet));
								//	if (JXJActiveCodeGetRet_suc != logRet)
								//	{
								//		break;
								//	}
								//}

								//RewardItemVec rewardItems;
								//rewardItems.reserve(5);
								//CRCore::ref_ptr<crBagItemData> itemPtr = NULL;
								//for (unsigned char i = 0; i < 5; ++ i)
								//{
								//	if (record[awardIdx[i]] == "") continue;
								//	crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
								//	itemPtr = new crBagItemData;
								//	itr->second->getStep();
								//	itemPtr->setItemID(awardVec[0]);
								//	itemPtr->setEquipMagic(awardVec[1]);
								//	itemPtr->setItemCount(awardVec[2]);
								//	rewardItems.push_back(itemPtr);
								//}
								std::vector<CRCore::crVector4i> awards;
								CRCore::crVector4i awardVec;
								for (unsigned char i = 0; i < 5; ++ i)
								{
									if (record[awardIdx[i]] == "") continue;
									crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
									awards.push_back(awardVec);

								}
								logRet = 1;
								sendReward(awards,m_this,"活动奖励");
								itr->second->setActivation(2,playerid);
								//m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
								//itr->second->setActivation(2,playerid);
							//} while (0);
							crPlayerDataEventPacket packet;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeChar(logRet);
							crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvHuodongGetAward,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				thisData->excHandle(MAKEINT64(WCH_LockData,0));

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
				default:

					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
			}
		}
	}
}

/************************************************************************/
/*    crJXJRecvClearSkillCDMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvClearSkillCDMethod::crJXJRecvClearSkillCDMethod():
	m_this(NULL),
	m_stream(NULL)
{
}

JXJ::crJXJRecvClearSkillCDMethod::crJXJRecvClearSkillCDMethod( const crJXJRecvClearSkillCDMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvClearSkillCDMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvClearSkillCDMethod::addParam( int i, const std::string& str )
{
}

void JXJ::crJXJRecvClearSkillCDMethod::operator()( crHandle &handle )
{
	if (crRunGameHandle::getInstance()->getRunningCode() == crRunGameHandle::NetGame)
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			unsigned char count = m_stream->_readUChar();
			std::vector<int> ClearIDVec;
			for (int i = 0; i < count; i++)
			{
				ClearIDVec.push_back(m_stream->_readInt());
			}
			void * param = NULL;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_EquipOnMap,param);
			EquipOnMap equipOnMap = *(EquipOnMap *)param;
			crItemChild *itemChild;
			crInstanceItem *equip;
			crData * equipData = NULL;
			int itemId = 0;
			_crInt64  lastUseTime = 0;
			for (std::vector<int>::iterator itr = ClearIDVec.begin();
				itr != ClearIDVec.end();
				++itr )
			{
				itemChild = m_this->findChildItem(*itr);
				if(itemChild && itemChild->isLoaded())
				{
					equip = itemChild->getInstanceItem();
					equipData = equip->getDataClass();
					if (equipData)
					{
						equipData->inputParam(WCHDATA_LastUseTime,&lastUseTime);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/************************************************************************/
/*    crJXJRecvClearTargetExtraMethod                                                                  */
/************************************************************************/

JXJ::crJXJRecvClearTargetExtraMethod::crJXJRecvClearTargetExtraMethod():
	m_this(NULL),
	m_stream(NULL)
{

}

JXJ::crJXJRecvClearTargetExtraMethod::crJXJRecvClearTargetExtraMethod( const crJXJRecvClearTargetExtraMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJRecvClearTargetExtraMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvClearTargetExtraMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJRecvClearTargetExtraMethod::operator()( crHandle &handle )
{
	void *param;
	crData *data = m_this->getDataClass();
	data->getParam(WCHDATA_ExtraData,param);
	if(param && m_stream.get())
	{
		char exceptBufType = m_stream->_readChar();
		crData *extraData = (crData *)param;
		crHandle *handle;
		extraData->excHandle(MAKEINT64(WCH_LockData,1));

		ref_ptr<crTableIO> bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);
		crTableIO::StrVec record;
		int isNegativeIdx = -1;
		if (bufferAttrTab.valid()) isNegativeIdx = bufferAttrTab->getTitleIndex("Buf类型");

		extraData->getParam(WCHDATA_ExtraIDMap,param);
		ExtraIDMap *extraIDMap = (ExtraIDMap *)param;
		for( ExtraIDMap::iterator itr = extraIDMap->begin();
			itr != extraIDMap->end(); )
		{
			std::string extraValue = crArgumentParser::appItoa(itr->first)+std::string("|")+crArgumentParser::appItoa(itr->second);
			if(bufferAttrTab.valid() && bufferAttrTab->queryOneRecord(0,extraValue,record)>=0 && isNegativeIdx >= 0)
			{
				if(atoi(record[isNegativeIdx].c_str()) == exceptBufType)
				{
					if(crGlobalHandle::isClient())
					{
						handle = extraData->getHandle(MAKEINT64(WCH_DoExtra,itr->first));
						if(handle)
						{
							handle->releaseObjects(NULL);
						}
					}
					extraData->removeHandle(MAKEINT64(WCH_DoExtra,itr->first));
					itr = extraIDMap->erase(itr);
				}
				else
				{
					++itr;
				}
			}
			else ++ itr;
		}
		extraData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

///************************************************************************/
///*    crJXJRecvHuodongSynMethod                                                                  */
///************************************************************************/
//
//JXJ::crJXJRecvHuodongSynMethod::crJXJRecvHuodongSynMethod():
//	m_this(NULL),
//	m_stream(NULL),
//	m_netType(GameServer)
//{
//
//}
//
//JXJ::crJXJRecvHuodongSynMethod::crJXJRecvHuodongSynMethod( const crJXJRecvHuodongSynMethod & handle ):
//	crMethod(handle),
//	m_this(handle.m_this),
//	m_stream(handle.m_stream),
//	m_netType(handle.m_netType)
//{
//
//}
//
//void JXJ::crJXJRecvHuodongSynMethod::inputParam( int i, void *param )
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
//void JXJ::crJXJRecvHuodongSynMethod::addParam( int i, const std::string& str )
//{
//
//}
//
//void JXJ::crJXJRecvHuodongSynMethod::operator()( crHandle &handle )
//{
//	if (m_this && m_stream.valid())
//	{
//		int playerid = m_this->getPlayerID();
//		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//		crRole *mainRole = m_this->getMainRole();
//		if(playerData.valid() && mainRole && mainRole->getDataClass())
//		{
//			unsigned short accid = m_stream->_readShort();
//
//			void *param;
//			crData *roleData = mainRole->getDataClass();
//			roleData->excHandle(MAKEINT64(WCH_LockData,1));
//			roleData->getParam(WCHDATA_JXJHuodongMap	,param);
//			HuodongMap *huodongMap= (HuodongMap *)param;
//			HuodongMap ::iterator itr = huodongMap->find(accid);
//			if(itr != huodongMap->end())
//			{
//				if(!itr->second->isComplete())
//				{
//					itr->second->doEvent(WCH_JXJHongdongSynSetp,MAKEINT64(m_this,NULL));
//				}
//			}
//			roleData->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//	}
//}

//**************************************************************************
//
//METHOD: crJXJHuodongCanvasInitMethod
//
//**************************************************************************/
crJXJHuodongCanvasInitMethod::crJXJHuodongCanvasInitMethod():
m_this(NULL)
{

}

crJXJHuodongCanvasInitMethod::crJXJHuodongCanvasInitMethod( const crJXJHuodongCanvasInitMethod & handle ):
	crMethod(handle)
{

}

void JXJ::crJXJHuodongCanvasInitMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJHuodongCanvasInitMethod::addParam( int i, const std::string& str )
{
}

void crJXJHuodongCanvasInitMethod::operator()( crHandle &handle )
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
			unsigned short serialNumber = 0; //节日类型活动在huodongVec中对应第几个索引,0表示没有该类型活动

			crData *roleData = mainRole->getDataClass();
			roleData->excHandle(MAKEINT64(WCH_LockData,1));

			crData *canvasData = m_this->getDataClass();
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJHuodongVec,param);
			HuodongVec & huodongVec = *(HuodongVec *)param;
			huodongVec.clear();
			roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			huodongVec.reserve(huodongMap->size());
			NewHuodongMap ::iterator itr = huodongMap->begin();
			for (; itr != huodongMap->end(); ++ itr)
			{
				if(itr->second->getActivation()==1)
				{
					if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
					{
						unsigned char displayValue = atoi(record[displayid].c_str());
						if(displayValue > 0 && displayValue != 2)
						{
							serialNumber++;
							huodongVec.push_back(itr->first);
							if (displayValue == 3)
							{//存在节日类型活动时，玩家每日首次登录自动弹出活动页面
								canvasData->inputParam(WCHDATA_JXJisFestivalHuodongExist,&serialNumber);
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

/**************************************************************************

METHOD: crJXJHuodongCanvasUpdateMethod 
 
**************************************************************************/
crJXJHuodongCanvasUpdateMethod::crJXJHuodongCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJHuodongCanvasUpdateMethod::crJXJHuodongCanvasUpdateMethod( const crJXJHuodongCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_sw (handle.m_sw),
	m_timeInput(handle.m_timeInput),
	m_contentInput(handle.m_contentInput),
	m_nullImage(handle.m_nullImage),
	m_hadGottenBtn(handle.m_hadGottenBtn),
	m_scroll(handle.m_scroll),
	m_code_sw(handle.m_code_sw),
	m_codeText(handle.m_codeText),
	m_Mzth_Input3(handle.m_Mzth_Input3),
	m_Mzth_Input4(handle.m_Mzth_Input4),
	m_Mzth_Input8(handle.m_Mzth_Input8),
	m_Mzth_Input10(handle.m_Mzth_Input10),
	m_Mzth_Icon1(handle.m_Mzth_Icon1),
	m_Mzth_Icon2(handle.m_Mzth_Icon2),
	m_Mzth_Donghua1(handle.m_Mzth_Donghua1),
	m_Mzth_Donghua2(handle.m_Mzth_Donghua2)
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
	}
}

void crJXJHuodongCanvasUpdateMethod::inputParam( int i, void *param )
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

void crJXJHuodongCanvasUpdateMethod::addParam( int i, const std::string& str )
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
		m_Mzth_Input3 = str;
		break;
	case 34:
		m_Mzth_Input4 = str;
		break;
	case 35:
		m_Mzth_Input8 = str;
		break;
	case 36:
		m_Mzth_Input10 = str;
		break;
	case 37:
		m_Mzth_Icon1 = str;
		break;
	case 38:
		m_Mzth_Icon2 =str;
		break;
	case 39:
		m_Mzth_Donghua1 = str;
		break;
	case 40:
		m_Mzth_Donghua2 = str;
		break;
	default:
		break;
	}
}

void crJXJHuodongCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crStaticTextWidgetNode > timeInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_timeInput));
		if (timeInput.valid())
			timeInput->clearString();
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
			int contentIdx = huodongTab->getTitleIndex("内容");
			int startTimeIdx = huodongTab->getTitleIndex("开始时间");
			int endTimeIdx = huodongTab->getTitleIndex("结束时间");
			int idIdx = huodongTab->getTitleIndex("id");
			int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");

			awardIdx[0] = huodongTab->getTitleIndex("奖励1");
			awardIdx[1] = huodongTab->getTitleIndex("奖励2");
			awardIdx[2] = huodongTab->getTitleIndex("奖励3");
			awardIdx[3] = huodongTab->getTitleIndex("奖励4");
			awardIdx[4] = huodongTab->getTitleIndex("奖励5");

			if (titleIdx < 0) break;
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
			for (unsigned char i = curPage; i < vecSize && j < 7; ++ i, ++ j)
			{
				if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongVec[i]),huoRecord) >= 0)
				{
					title[j]->setString(huoRecord[titleIdx]);
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
				if ( huodongID == Activation_WeekActivity)
				{
					jihuoma_sw->setActiveSwitchSet(2);
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					if (myPlayer && myPlayer->getDataClass())
					{
						crData * playerData = myPlayer->getDataClass();
						if (playerData)
						{
							playerData->getParam(WCHDATA_JXJVipLv,param);
							unsigned char vipLevel = *(unsigned char *)param;
							int huodongID = 0;
							int Cricles = 0;			
							ref_ptr<crStaticTextWidgetNode > Mzth_Input3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Mzth_Input3));
							ref_ptr<crStaticTextWidgetNode > Mzth_Input4 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Mzth_Input4));
							ref_ptr<crStaticTextWidgetNode > Mzth_Input8 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Mzth_Input8));
							ref_ptr<crStaticTextWidgetNode > Mzth_Input10 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_Mzth_Input10));
							ref_ptr<crButtonWidgetNode> icon1 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Mzth_Icon1));
							ref_ptr<crButtonWidgetNode> icon2 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_Mzth_Icon2));
							ref_ptr<crSequence> donghua1 = dynamic_cast<crSequence *>(m_this->getChildNode(m_Mzth_Donghua1));
							ref_ptr<crSequence> donghua2 = dynamic_cast<crSequence *>(m_this->getChildNode(m_Mzth_Donghua2));
							ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
							if (false == huodongTab.valid()) break;
							int idIdx = huodongTab->getTitleIndex("id");
							int cishuIdx = huodongTab->getTitleIndex("次数");
							int titleIdx = huodongTab->getTitleIndex("标题");
							ref_ptr<crRole> mainRole = myPlayer->getMainRole();
							if(mainRole.valid() && mainRole->getDataClass())
							{
								std::string cishu ="0";
								crVector4f coloryellow = (crVector4f(240.0f,200.0f,30.0f,255.0f)/255.0f);
								crData *roleData = mainRole->getDataClass();
								CRCore::ScopedLock<crData> lock(*roleData);
								roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
								NewHuodongMap *huodongMap= (NewHuodongMap *)param;
								NewHuodongMap ::iterator itr = huodongMap->end();
								crTableIO::DataVec zhouhuodongVec;
								std::string titletext("每周特惠累计");						
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
											cishu = (*siter)[cishuIdx];
											break;
										}
									}
								}
								if(itr != huodongMap->end())
								{
									Cricles = itr->second->getCircles();
									if(itr->second->isCanAdvance() && !itr->second->getComplete())
									{
										if (Mzth_Input4.valid())
										{
											Mzth_Input4->setVisiable(true);
											std::string text("点击可领取");
											Mzth_Input4->setString(text);
											Mzth_Input4->setColor(coloryellow);
										}
										if (icon2.valid())
										{
											icon2->setEnable(true);
										}
										if (donghua2.valid())
										{
											donghua2->setVisiable(true);
										}	
									}
									else
									{
										if (Mzth_Input4.valid())
										{
											Mzth_Input4->setVisiable(true);
											std::string text("可获赠");
											Mzth_Input4->setString(text);
											Mzth_Input4->setColor(FCOLOR_BLACK);
										}
										if (icon2.valid())
										{
											icon2->setEnable(false);
										}
										if (donghua2.valid())
										{
											donghua2->setVisiable(false);
										}
										
									}
									std::string inputtext = cishu + "/" + crArgumentParser::appItoa(Cricles);
									if (Cricles == 1 && itr->second->getComplete())
									{
										inputtext = cishu + "/0";
									}
									if (Mzth_Input10.valid())
									{
										Mzth_Input10->setString(inputtext);
									}
								}
								else
								{
									if (Mzth_Input4.valid() && icon2.valid() && Mzth_Input10.valid() && donghua2.valid())
									{
										Mzth_Input4->setVisiable(true);
										std::string text("可获赠");
										Mzth_Input4->setString(text);
										Mzth_Input4->setColor(FCOLOR_BLACK);
										icon2->setEnable(false);
										donghua2->setVisiable(false);
										text = cishu + "/0";
										Mzth_Input10->setString(text);
									}
								}
								cishu = "0";
								zhouhuodongVec.clear();
								titletext ="每周特惠单笔";
								itr = huodongMap->end();
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
											cishu = (*siter)[cishuIdx];
											break;
										}
									}
								}	
								Cricles = 0;
								if(itr != huodongMap->end())
								{
									Cricles = itr->second->getCircles();
									if(itr->second->isCanAdvance() && !itr->second->getComplete())
									{
										if (Mzth_Input3.valid())
										{
											Mzth_Input3->setVisiable(true);
											std::string text("点击可领取");
											Mzth_Input3->setString(text);
											Mzth_Input3->setColor(coloryellow);
										}
										if (icon1.valid())
										{
											icon1->setEnable(true);
										}
										if (donghua1.valid())
										{
											donghua1->setVisiable(true);
										}	
									}
									else
									{
										if (Mzth_Input3.valid())
										{
											Mzth_Input3->setVisiable(true);
											std::string text("可获赠");
											Mzth_Input3->setString(text);
											Mzth_Input3->setColor(FCOLOR_BLACK);
										}
										if (icon1.valid())
										{
											icon1->setEnable(false);
										}
										if (donghua1.valid())
										{
											donghua1->setVisiable(false);
										}

									}
									std::string inputtext = cishu + "/" + crArgumentParser::appItoa(Cricles);
									if (Cricles == 1 && itr->second->getComplete())
									{
										inputtext = cishu + "/0";
									}
									if (Mzth_Input8.valid())
									{
										Mzth_Input8->setString(inputtext);
									}
								}
								else
								{
									if (Mzth_Input3.valid() && icon1.valid() && Mzth_Input8.valid() && donghua1.valid())
									{
										Mzth_Input3->setVisiable(true);
										std::string text("可获赠");
										Mzth_Input3->setString(text);
										Mzth_Input3->setColor(FCOLOR_BLACK);
										icon1->setEnable(false);
										donghua1->setVisiable(false);
										text = cishu + "/0";
										Mzth_Input8->setString(text);
									}
								}
							}

						}
						
					}
					break;
				}
				else
				{
					jihuoma_sw->setActiveSwitchSet(0);
				}
			}

			typedef std::vector<int> Vec5i;
			Vec5i startTimeVec5i;
			Vec5i endTimeVec5i;
			startTimeVec5i.reserve(5);
			endTimeVec5i.reserve(5);
			crArgumentParser::appAtoVec(huoRecord[startTimeIdx],startTimeVec5i);
			crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endTimeVec5i);
			startTimeVec5i.resize(5,0);
			endTimeVec5i.resize(5,0);

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
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if (false == itemtab.valid()) break;
			crTableIO::StrVec itemRecord;
			int iconIdx = itemtab->getTitleIndex("icon");
			int itemIdIdx = itemtab->getTitleIndex("itemid");

			CRCore::crVector3i awardVec;
			for (unsigned char i = 0; i < 5; ++ i)
			{
				crArgumentParser::appAtoVec(huoRecord[awardIdx[i]],awardVec);
				if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(awardVec[0]),itemRecord) >= 0)
				{
					if (image[i].valid())
						image[i]->setImageName(itemRecord[iconIdx]);
					if (awardVec[2] > 1)
					{
						if (itemNum[i].valid())
							itemNum[i]->setString(crArgumentParser::appItoa(awardVec[2]));
					}
				}
				
			}

			if (getBtn.valid())
			{
				getBtn->setEnable(true);
			}

			//crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			//crRole *mainRole = playerData->getMainRole();
			//if(mainRole && mainRole->getDataClass())
			//{
			//	crData *roleData = mainRole->getDataClass();
			//	roleData->excHandle(MAKEINT64(WCH_LockData,1));
			//	roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
			//	NewHuodongMap *huodongMap= (NewHuodongMap *)param;
			//	NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
			//	if(itr != huodongMap->end())
			//	{
			//		if(itr->second->getActivation() == crJXJHuodong::HadGot)
			//		{
			//			if (getBtn.valid())
			//			{
			//				getBtn->setEnable(false);
			//				if (getBtn_sw.valid())
			//					getBtn_sw->setActiveSwitchSet(3);
			//			}
			//			if (hadGottenBtn.valid())
			//			{
			//				hadGottenBtn->setVisiable(true);
			//				hadGottenBtn->setEnable(false);
			//				if (hadGottenBtn_sw.valid())
			//				{
			//					hadGottenBtn_sw->setActiveSwitchSet(3);
			//				}
			//			}
			//		}
			//		else if (itr->second->getComplete() == crJXJHuodong::NotStart)
			//		{
			//			if (getBtn.valid())
			//			{
			//				getBtn->setEnable(false);
			//				if (getBtn_sw.valid())
			//					getBtn_sw->setActiveSwitchSet(3);
			//			}
			//		}
			//	}
			//	roleData->excHandle(MAKEINT64(WCH_LockData,0));
			//}
		} while (0);
	}

}

/**************************************************************************

METHOD: crJXJSetCurrentHuodongIdMethod
 
**************************************************************************/
JXJ::crJXJSetCurrentHuodongIdMethod::crJXJSetCurrentHuodongIdMethod():
	m_this(NULL),
	m_idx(0)
{

}

JXJ::crJXJSetCurrentHuodongIdMethod::crJXJSetCurrentHuodongIdMethod( const crJXJSetCurrentHuodongIdMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_idx(handle.m_idx)
{

}

void JXJ::crJXJSetCurrentHuodongIdMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSetCurrentHuodongIdMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_idx = atoi(str.c_str());
		break;
	case 2:
		m_scroll = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJSetCurrentHuodongIdMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (!canvas.valid() || !canvas->getDataClass()) return;

	ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scroll));
	crData * canvasData = canvas->getDataClass();
	canvasData->excHandle(MAKEINT64(WCH_LockData,1));

	void *param;
	canvasData->getParam(WCHDATA_JXJHuodongVec,param);
	HuodongVec & huodongVec = *(HuodongVec *)param;

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

	unsigned short idx = curPage + m_idx;
	canvasData->inputParam(WCHDATA_JXJHuodongSelectId	,&idx);
	canvasData->inputParam(WCHDATA_JXJHuodongSelectIdx	,&m_idx);

	canvasData->excHandle(MAKEINT64(WCH_LockData,0));

}

//JXJ::crJXJCheckHuodongTimeIntervalMethod::crJXJCheckHuodongTimeIntervalMethod():
//	m_this(NULL)
//{
//
//}
//
//JXJ::crJXJCheckHuodongTimeIntervalMethod::crJXJCheckHuodongTimeIntervalMethod( const crJXJCheckHuodongTimeIntervalMethod & handle ):
//	crMethod(handle),
//	m_this(NULL)
//{
//
//}
//
//void JXJ::crJXJCheckHuodongTimeIntervalMethod::inputParam( int i, void *param )
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
//		m_this = (crJXJHuodong *)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
//		}
//		else
//		{
//			m_playerGameData = NULL;
//		}
//		break;
//	}
//}
//
//void JXJ::crJXJCheckHuodongTimeIntervalMethod::addParam( int i, const std::string& str )
//{
//}
//
//void JXJ::crJXJCheckHuodongTimeIntervalMethod::operator()( crHandle &handle )
//{
//	do 
//	{
//		if (NULL == m_this) break;
//		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//		if (false == huodongTab.valid()) break;
//		crTableIO::StrVec huoRecord;
//		int isOpenIdx = huodongTab->getTitleIndex("开启");
//		int startTimeIdx = huodongTab->getTitleIndex("开始时间");
//		int endTimeIdx = huodongTab->getTitleIndex("结束时间");
//		int idIdx = huodongTab->getTitleIndex("id");
//
//		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_this->getID()),huoRecord) < 0) break;
//
//		typedef std::vector<int> Vec5i;
//		Vec5i startTimeVec5i;
//		Vec5i endTimeVec5i;
//		startTimeVec5i.reserve(5);
//		endTimeVec5i.reserve(5);
//		crArgumentParser::appAtoVec(huoRecord[startTimeIdx],startTimeVec5i);
//		crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endTimeVec5i);
//
//		startTimeVec5i.resize(5,0);
//		endTimeVec5i.resize(5,0);
//
//		time_t nowTime_t = time(0);
//		struct  tm nowTm = *localtime(&nowTime_t);
//		struct tm  startTm;
//		struct tm  endTm;
//		memset(&startTm,0,sizeof (startTm));
//		memset(&endTm,0,sizeof (endTm));
//
//		startTm.tm_year = startTimeVec5i[0] - 1900;
//		endTm.tm_year = endTimeVec5i[0] - 1900;
//		startTm.tm_mon = startTimeVec5i[1] - 1;
//		endTm.tm_mon = endTimeVec5i[1] - 1;
//		startTm.tm_mday = startTimeVec5i[2];
//		endTm.tm_mday = endTimeVec5i[2];
//		startTm.tm_hour = startTimeVec5i[3];
//		endTm.tm_hour = endTimeVec5i[3];
//		startTm.tm_min = startTimeVec5i[4];
//		endTm.tm_min = endTimeVec5i[4];
//		startTm.tm_isdst = nowTm.tm_isdst;
//		endTm.tm_isdst = nowTm.tm_isdst;
//
//		time_t startTime_t = mktime(&startTm);
//		time_t endTime_t = mktime(&endTm);
//		if (nowTime_t >= startTime_t && nowTime_t <= endTime_t)
//		{
//			if (!m_this->isOpen())
//			{
//				int id = m_playerGameData?m_playerGameData->getPlayerID():0;
//				m_this->changeComplete(crJXJHuodong::Started,id);
//			}
//		}
//		else
//		{
//			if (!m_this->isComplete())
//			{
//				crJXJHuodong::CompleteStatus status = m_this->getComplete();
//				switch (m_this->getComplete())
//				{
//				case crJXJHuodong::NotStart:
//				case crJXJHuodong::Started:
//				case crJXJHuodong::CannotGet:
//					status = crJXJHuodong::Finished_cannotGot;
//					break;
//				case crJXJHuodong::CanGet:
//					status = crJXJHuodong::Finished_notGot;
//					break;
//				case crJXJHuodong::HadGot:
//					status = crJXJHuodong::Finished_hadGot;
//					break;
//				default:
//					break;
//				}
//				int id = m_playerGameData?m_playerGameData->getPlayerID():0;
//				m_this->changeComplete(status,id);
//			}
//		}
//	} while (0);
//}

/**************************************************************************

METHOD: crJXJEquipAttackExtraTimeLimitTestMethod
 
**************************************************************************/
JXJ::crJXJEquipAttackExtraTimeLimitTestMethod::crJXJEquipAttackExtraTimeLimitTestMethod():
	m_this(NULL),
	m_hitParam(NULL),
	m_timeInterval(0),
	m_preTime_t(0),
	m_hitrate(0)
{

}

JXJ::crJXJEquipAttackExtraTimeLimitTestMethod::crJXJEquipAttackExtraTimeLimitTestMethod( const crJXJEquipAttackExtraTimeLimitTestMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_hitParam(NULL),
	m_timeInterval(0),
	m_preTime_t(0),
	m_hitrate(0)
{

}

void JXJ::crJXJEquipAttackExtraTimeLimitTestMethod::inputParam( int i, void *param )
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

void JXJ::crJXJEquipAttackExtraTimeLimitTestMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_hitrate = atoi(str.c_str());
		break;
	case 1:
		m_timeInterval = atoi(str.c_str());
		break;
	}
}

void JXJ::crJXJEquipAttackExtraTimeLimitTestMethod::operator()( crHandle &handle )
{
	bool valid = false;
	time_t nowTime_t = time(NULL);
	if (nowTime_t - m_preTime_t > m_timeInterval)
	{
		valid = CRCore::rangei(0,100).get_random()<m_hitrate;
		if (valid ) m_preTime_t = nowTime_t;
	}
	handle.outputParam(0,&valid);
}

///**************************************************************************
//
//METHOD: crJXJUIHuodongToolTipsMethod
// 
//**************************************************************************/
//
//JXJ::crJXJUIHuodongToolTipsMethod::crJXJUIHuodongToolTipsMethod():
//	m_this(NULL),
//	m_index(0),
//	m_festivalActivity_index(0)
//{
//}
//
//JXJ::crJXJUIHuodongToolTipsMethod::crJXJUIHuodongToolTipsMethod( const crJXJUIHuodongToolTipsMethod & handle ):
//	crMethod(handle),
//	m_tips (handle.m_tips),
//	m_name (handle.m_name),
//	m_info2 (handle.m_info2),
//	m_itemlv(handle.m_itemlv),
//	m_itemicon(handle.m_itemicon),
//	m_itemequipon(handle.m_itemequipon),
//	m_infocanvas(handle.m_infocanvas),
//	m_infobk(handle.m_infobk),
//	m_infodi(handle.m_infodi),
//	m_offsets (handle.m_offsets),
//	m_colorred (handle.m_colorred),
//	m_index (handle.m_index),
//	m_festivalActivity_index(handle.m_festivalActivity_index)
//{
//
//}
//
//void JXJ::crJXJUIHuodongToolTipsMethod::addParam( int i, const std::string& str )
//{
//	switch(i) 
//	{
//	case 0:
//		m_tips = str;
//		break;
//	case 1:
//		m_name = str;
//		break;
//	case 2:
//		m_info2 = str;
//		break;
//	case 3:
//		m_itemlv = str;
//		break;
//	case 4:
//		m_itemicon = str;
//		break;
//	case 5:
//		m_itemequipon = str;
//		break;
//	case 6:
//		m_infocanvas = str;
//		break;
//	case 7:
//		m_infobk = str;
//		break;
//	case 8:
//		m_infodi = str;
//		break;
//	case 9:
//		crArgumentParser::appAtoVec(str,m_offsets);
//		break;
//	case 10:
//		crArgumentParser::appAtoVec(str,m_colorred);//red
//		m_colorred /= 255.0f;
//		break;
//	case 11:
//		m_index = atoi(str.c_str());
//		break;
//	case 12:
//		m_festivalActivity_index = atoi(str.c_str());
//		break;
//	}
//}
//
//void JXJ::crJXJUIHuodongToolTipsMethod::inputParam( int i, void *param )
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
//	}
//}
//
//void JXJ::crJXJUIHuodongToolTipsMethod::operator()( crHandle &handle )
//{
//	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
//	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(tipsCanvas.valid() && playerData && infocanvas.valid())
//	{
//		void *param;
//		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
//		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
//		crTableIO::StrVec record,lordrecord;
//		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
//		crStaticTextWidgetNode *itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
//		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
//		crStaticTextWidgetNode *itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
//		//
//		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
//		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
//		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
//		if (name)
//		{
//			name->clearString();
//		}
//		if (info2)
//		{
//			info2->clearText();
//		}
//		if (itemlv)
//		{
//			itemlv->clearString();
//		}
//		if (itemicon)
//		{
//			itemicon->clearImage();
//		}
//		if (itemequipon)
//		{
//			itemequipon->clearString();
//		}
//		crRole *mainRole = playerData->getMainRole();
//		crData *roleData = mainRole->getDataClass();
//		roleData->getParam(WCHDATA_Level,param);
//		unsigned char rolelv = *(unsigned char *)param;
//		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
//		crData *data = crBrain::getInstance()->getDataClass();
//		crTableIO::DataVec recordVec;
//		crData *canvasdata = m_this->getParentCanvas()->getDataClass();
//		int itemid = 0;
//		int equipmagic = 0;
//		if (canvasdata)
//		{
//			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//			int idIdx = huodongTab->getTitleIndex("id");
//
//			int awardIdx[5] ;
//			awardIdx[0] = huodongTab->getTitleIndex("奖励1");
//			awardIdx[1] = huodongTab->getTitleIndex("奖励2");
//			awardIdx[2] = huodongTab->getTitleIndex("奖励3");
//			awardIdx[3] = huodongTab->getTitleIndex("奖励4");
//			awardIdx[4] = huodongTab->getTitleIndex("奖励5");
//			crTableIO::StrVec huoRecord;
//
//			canvasdata->getParam(WCHDATA_JXJHuodongSelectId	,param);
//			unsigned char selectID = *(unsigned char *)param;
//
//			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
//			canvasdata->getParam(WCHDATA_JXJHuodongVec,param);
//			HuodongVec & huodongVec = *(HuodongVec *)param;
//
//			int huodongID = huodongVec[selectID];
//			if (m_festivalActivity_index > 0)
//			{
//				huodongID += m_festivalActivity_index;
//			}
//
//			if (huodongVec.size() > selectID && huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID),huoRecord) >=0)
//			{
//				CRCore::crVector4i awardVec;
//				if (m_index < 5)
//				{
//					crArgumentParser::appAtoVec(huoRecord[awardIdx[m_index]],awardVec);
//					itemid = awardVec[0];
//					equipmagic = awardVec[1];
//				}
//			}
//			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
//		}
//		if (itemid > 0)
//		{
//			int iconindex = itemtab->getTitleIndex("icon");
//			int nameindex = itemtab->getTitleIndex("name");
//			int typeindex = itemtab->getTitleIndex("类型");
//			int colorindex = itemtab->getTitleIndex("品质");
//			int needlvindex = itemtab->getTitleIndex("人物等级");
//			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
//			{
//				int colormodeid = atoi(record[colorindex].c_str());
//				if(name)
//				{
//					name->setColor(crVector4(1,1,0,1));
//					name->setString(record[nameindex]);
//				}
//				if (itemlv)
//				{
//					if (rolelv < atoi(record[needlvindex].c_str()))
//					{
//						itemlv->setColor(m_colorred);
//					}
//					else
//					{
//						itemlv->setColor(crVector4(1,1,1,1));
//					}
//					itemlv->setString(record[needlvindex]);
//				}
//				if (itemicon)
//				{
//					itemicon->setImageName(record[iconindex]);
//				}
//				if (itemequipon)
//				{
//					std::string str;// = "未使用";
//					if (atoi(record[typeindex].c_str()) == IT_Equip)
//					{
//						if (m_index == 1)
//						{
//							str = "已装备";
//						}
//						else
//						{
//							str = "未装备";
//						}
//					}
//					itemequipon->setString(str);
//				}
//				if(info2)
//				{
//					ItemMap itemMap;
//					rcfg::ConfigScript outscript(&itemMap);
//					crVector2i item(itemid,equipmagic);
//					playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
//					info2->setHypertext(outscript);
//				}
//			}
//			/////////////////////////////////////////
//			float posx2,posy2;
//			float boxXlength = infobk->getChild(0)->getBoundBox().xLength();
//			///////////计算缩放
//			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
//			info2->updateData();
//			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
//			const crBoundingBox &bbox2 = info2->getBoundBox();
//			float texheight = 0.0f;
//			float needmove = 0.0f;
//			if(bbox.valid())
//			{
//				texheight = bbox.yLength();
//				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
//				if(texheight<boxYlength) texheight = boxYlength;
//				float scale = texheight/boxYlength;
//				float needmove ;
//				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
//				crVector3 centerTansTar = info2->getBound().center();
//
//				float tmpLength = bbox2.yLength();
//				if(infodi)
//				{
//					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
//					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
//					//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
//				}
//				else
//					centerTansTar[1] += 0.5f * (tmpLength - texheight);
//				// 源中心点
//				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
//				// 将源中心点平移到原点
//				crMatrix put_mat = crMatrix::translate(centerTansSrc);
//				crMatrix inv_put = crMatrix::inverse(put_mat);
//				crMatrix rotmat;
//				rotmat.postMult(inv_put);
//				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
//				rotmat.postMult(crMatrix::translate(centerTansTar));
//				if(infobk)infobk->setMatrix(rotmat);
//
//				if(infodi)
//				{
//					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
//					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
//					needmove = boxYlength - texheight + diYlength * 0.25;
//					//needmove = boxYlength - texheight;
//					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
//				}
//			}
//			else
//			{
//				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
//				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
//			}
//			/////////
//
//			/////////////////////////////////////////
//			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
//			const crBoundingBox &iconbox = m_this->getBoundBox();
//			const crBoundingBox &tip = tipsCanvas->getBoundBox();
//			float posx = (tip.m_max[0]-tip.m_min[0])*0.5;
//			float posy = (tip.m_max[1]-tip.m_min[1])*0.5;
//			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
//			pos = pos * mat;
//			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
//			pos2 = pos2 * mat;
//			/////////////////////////////////////////
//			if (pos2[0] >= 0)
//			{
//				boxXlength = -boxXlength;
//				posx2 = pos2[0] - posx;
//			}
//			else
//			{
//				posx2 = pos[0] + posx;
//			}
//			if (pos2[1] >= 0)
//			{
//				posy2 = pos[1] - posy;
//			}
//			else
//			{
//				posy2 = pos2[1] + posy - needmove;// + posinfoy;
//			}
//			crVector2 mouse(crVector2(posx2,posy2));
//
//			//float needmovex = 0;
//			//if (m_index == 1)
//			//{
//			//	needmovex += boxXlength;
//			//}
//			//tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f)/* * mat*/);
//			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
//			tipsCanvas->setCanFocus(false);
//			infocanvas->setCanFocus(false);
//			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
//			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
//			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
//		}
//		else
//		{
//			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
//			crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
//		}
//	}
//}

/**************************************************************************

METHOD: crJXJQueryTabVersionMethod
 
**************************************************************************/
crJXJQueryTabVersionMethod::crJXJQueryTabVersionMethod()
{
}

crJXJQueryTabVersionMethod::crJXJQueryTabVersionMethod(const crJXJQueryTabVersionMethod & handle) :
	crMethod(handle),
	m_tabids(handle.m_tabids)
{
}

void crJXJQueryTabVersionMethod::inputParam(int i, void *param)
{

}

void crJXJQueryTabVersionMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str, m_tabids,'|');
		break;
	}
}

void crJXJQueryTabVersionMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	int count = m_tabids.size();
	if(netConductor && count>0)
	{
		crPlayerDataEventPacket packet;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(count*8+1);
		stream->_writeUChar(count);
		ref_ptr<crTableIO> updateTab;
		int tabVersion = 0;
		int tabVersionIdx;
		for (std::vector<int>::iterator itr = m_tabids.begin();
			itr != m_tabids.end();
			++itr)
		{
			stream->_writeInt(*itr);
			tabVersion = 0;
			updateTab = crGlobalHandle::gData()->gGlobalTable(*itr);
			if (updateTab.valid())
			{
				tabVersionIdx = updateTab->getTitleIndex("TAB_VERSION");
				if (tabVersionIdx >= 0)
				{
					tabVersion = atoi((*updateTab)(0, tabVersionIdx).c_str());
				}
			}
			stream->_writeInt(tabVersion);
		}
		crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvQueryTabVersion, stream.get());
		netConductor->getNetManager()->sendPacket("all", packet);
	}
}

/**************************************************************************

METHOD: crJXJRecvQueryTabVersionMethod
 
**************************************************************************/
crJXJRecvQueryTabVersionMethod::crJXJRecvQueryTabVersionMethod() :
	m_this(NULL)
{
}

crJXJRecvQueryTabVersionMethod::crJXJRecvQueryTabVersionMethod(const crJXJRecvQueryTabVersionMethod& handle) :
	crMethod(handle)
{
}

void crJXJRecvQueryTabVersionMethod::inputParam(int i, void *param)
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

void crJXJRecvQueryTabVersionMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvQueryTabVersionMethod::operator()(crHandle &handle)
{
	if (m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				unsigned char count = m_stream->_readUChar();
				int id, clientversion, newTabVersion,tabVersionIdx;
				ref_ptr<crTableIO> updateTab;
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				for (int i = 0; i < count; i++)
				{
					id = m_stream->_readInt();
					clientversion = m_stream->_readInt();
					updateTab = crGlobalHandle::gData()->gGlobalTable(id);
					if (updateTab.valid())
					{
						tabVersionIdx = updateTab->getTitleIndex("TAB_VERSION");
						if (tabVersionIdx >= 0)
						{
							newTabVersion = atoi((*updateTab)(0, tabVersionIdx).c_str());
							if (newTabVersion != clientversion)
							{
								stream->seekBegin();
								stream->_writeInt(id);
								crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvQueryTabVersion, stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
							}
						}
					}
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
			if (data)
			{
				int id = m_stream->_readInt();
				std::string filename = crGlobalHandle::gData()->gGlobalTableFile(id);
				if (!filename.empty())
				{
					std::string tabName = crArgumentParser::getFileNameEliminateExt(filename) + ".cook";
					data->insertDownloadFile(id, tabName);
					data->startDownloadFile(id, GameClient_Game);
				}
			}
		}
	}
}

///**************************************************************************
//
//METHOD: crJXJReloadHuodongTabMethod
// 
//**************************************************************************/
//JXJ::crJXJClientReloadFileMethod::crJXJClientReloadFileMethod()
//{
//
//}
//
//JXJ::crJXJClientReloadFileMethod::crJXJClientReloadFileMethod( const crJXJClientReloadFileMethod & handle ):
//	crMethod(handle),
//	m_fileName(handle.m_fileName),
//	m_fileId(handle.m_fileId)
//{
//
//}
//
//void JXJ::crJXJClientReloadFileMethod::inputParam( int i, void *param )
//{
//
//}
//
//void JXJ::crJXJClientReloadFileMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_fileId = atoi(str.c_str());
//		break;
//	case 1:
//		m_fileName = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJClientReloadFileMethod::operator()( crHandle &handle )
//{
//	crGlobalHandle::gData()->reloadGlobalTable(m_fileId,m_fileName);
//}


/**************************************************************************

METHOD: crJXJCheckHuodongSingleRechargeMethod
 
**************************************************************************/
crJXJCheckHuodongSingleValueChangeMethod::crJXJCheckHuodongSingleValueChangeMethod():
	m_this(NULL),
	m_WCHDATA_ID(0),
	m_dataType(0),
	m_lastvalue(-1)
{

}

crJXJCheckHuodongSingleValueChangeMethod::crJXJCheckHuodongSingleValueChangeMethod( const crJXJCheckHuodongSingleValueChangeMethod & handle ):
	crMethod(handle),
	m_WCHDATA_ID(0),
	m_dataType(0),
	m_lastvalue(-1)
{

}

void crJXJCheckHuodongSingleValueChangeMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_playerGameData = NULL;
		}
		break;
	}
}

void crJXJCheckHuodongSingleValueChangeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_WCHDATA_ID = atoi(str.c_str());
		break;
	case 1:
		m_dataType = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void crJXJCheckHuodongSingleValueChangeMethod::operator()( crHandle &handle )
{
	do 
	{
		if (NULL == m_playerGameData || m_playerGameData->getDataClass() == NULL) break;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		crTableIO::StrVec huoRecord;
		int conditionIdx = huodongTab->getTitleIndex("条件");
		if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_this->getID()),huoRecord) < 0) break;

		crData * playerGameData = m_playerGameData->getDataClass();

		void *param = NULL;
		int valueData = -1;
		playerGameData ->getParam(m_WCHDATA_ID,param);
		switch (m_dataType)
		{
		case JXJDataType_Int	:
			valueData = *(int *)param;
			break;
		case JXJDataType_UInt	:
			valueData = *(unsigned int *)param;
			break;
		case JXJDataType_Short	:
			valueData = *(short *)param;
			break;
		case JXJDataType_Char	:
			valueData = *(char *)param;
			break;
		case JXJDataType_UChar	:
			valueData = *(unsigned char *)param;
			break;
		default:
			break;
		}
		if (-1 == valueData) break;

		if(m_lastvalue == -1)
		{
			m_lastvalue = valueData;
		}
		else if(!m_this->isCanAdvance())
		{
			int delta = valueData - m_lastvalue;
			m_lastvalue = valueData;
			if(delta>0)//不计消耗
				m_this->setStep(delta,m_playerGameData->getPlayerID());
		}
	} while (0);
}

/**************************************************************************

METHOD: crJXJClientHuodongGetAwardsMethod
 
**************************************************************************/

crJXJClientHuodongGetAwardsMethod::crJXJClientHuodongGetAwardsMethod():
	m_this(NULL),
	m_festivalHuodongIndex(0)
{

}

crJXJClientHuodongGetAwardsMethod::crJXJClientHuodongGetAwardsMethod( const crJXJClientHuodongGetAwardsMethod & handle ):
	crMethod(handle),
	m_infoCanvas(handle.m_infoCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_festivalHuodongIndex(handle.m_festivalHuodongIndex)
{

}

void crJXJClientHuodongGetAwardsMethod::inputParam( int i, void *param )
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

void crJXJClientHuodongGetAwardsMethod::addParam( int i, const std::string& str )
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
		m_festivalHuodongIndex = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void crJXJClientHuodongGetAwardsMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	
	if(netConductor)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && canvas->getDataClass())
		{
			bool valid = false;
			void *param;
			crData * canvasData = canvas->getDataClass();

			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJHuodongSelectId	,param);
			unsigned short selectID = *(unsigned short *)param;
			canvasData->getParam(WCHDATA_JXJHuodongVec,param);
			HuodongVec & huodongVec = *(HuodongVec *)param;
			unsigned short huodongID = 0;
			if (huodongVec.size() > selectID)
			{
				huodongID = huodongVec[selectID] + m_festivalHuodongIndex;
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			ref_ptr<crRole> mainRole = playerData->getMainRole();
			if(mainRole.valid() && mainRole->getDataClass() )
			{
				crData *roleData = mainRole->getDataClass();
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				NewHuodongMap ::iterator itr = huodongMap->find(huodongID);
				if(itr != huodongMap->end())
				{
					if(itr->second->getActivation()==1)
					{
						ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
						crTableIO::StrVec record;
						int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");
						int titleIdx = huodongTab->getTitleIndex("标题");
						if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongID),record) >= 0)
						{
							crData *data = playerData->getDataClass();
							if (data)
							{
								data->getParam(WCHDATA_JXJVipLv,param);
								unsigned char vipLevel = *(unsigned char *)param;
								std::string title = record[titleIdx];
								if (vipLevel == 0 && (title == "VIP每日礼包" || title== "VIP每周礼包"))
								{
									valid = false;
								}
								else
								{
									crPlayerDataEventPacket packet;
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									if (atoi(record[codeTypeIdx].c_str()))
									{
										//ref_ptr <crStaticTextWidgetNode> content = dynamic_cast<crStaticTextWidgetNode *>(m_this->getParentCanvas()->getWidget(m_input));
										//std::string activitycode;
										//if (content.valid() && !content->getString().empty())
										//{
										//	activitycode = content->getUTF8String();
										//	stream->createBuf(2+activitycode.length()+4);
										//	stream->_writeShort(huodongID);
										//	stream->_writeString(activitycode);
										//	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHuodongGetAward,stream.get());
										//	netConductor->getNetManager()->sendPacket("all",packet);
										//	valid = true;
										//}
									}
									else
									{
										stream->createBuf(2);
										stream->_writeShort(huodongID);
										crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvHuodongGetAward,stream.get());
										netConductor->getNetManager()->sendPacket("all",packet);
										valid = true;

										if (m_festivalHuodongIndex > 0)
										{//节日活动类型
											stream->createBuf(2);
											stream->_writeShort(huodongID - m_festivalHuodongIndex);
											crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckFestivalActivityComplete,stream.get());
											netConductor->getNetManager()->sendPacket("all",packet);
										}
									}
								}

							}
						}
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
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

METHOD: crJXJCheckHuodongAddUpValueChangeMethod
 
**************************************************************************/
crJXJCheckHuodongAddUpValueChangeMethod::crJXJCheckHuodongAddUpValueChangeMethod():
	m_this(NULL),
	m_WCHDATA_ID(0),
	m_dataType(0),
	m_lastdata(-1)
{

}

crJXJCheckHuodongAddUpValueChangeMethod::crJXJCheckHuodongAddUpValueChangeMethod( const crJXJCheckHuodongAddUpValueChangeMethod & handle ):
	crMethod(handle),
	m_WCHDATA_ID(0),
	m_dataType(0),
	m_lastdata(-1)
{

}

void crJXJCheckHuodongAddUpValueChangeMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_playerGameData = NULL;
		}
		break;
	}
}

void crJXJCheckHuodongAddUpValueChangeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_WCHDATA_ID = atoi(str.c_str());
		break;
	case 1:
		m_dataType = atoi(str.c_str());
		break;
	case 2:
		m_dataStruct = (JXJDataStruct)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJCheckHuodongAddUpValueChangeMethod::operator()( crHandle &handle )
{
	do 
	{
		if (NULL == m_playerGameData || m_playerGameData->getDataClass() == NULL
			|| m_playerGameData->getMainRole() == NULL) break;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		crTableIO::StrVec huoRecord;
		int conditionIdx = huodongTab->getTitleIndex("条件");

		if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_this->getID()),huoRecord) < 0) break;

		crData * playerGameData = m_playerGameData->getDataClass();

		crData * data = playerGameData;
		if (JXJDataStruct_mainRoleData == m_dataStruct)
		{
			data = m_playerGameData->getMainRole()->getDataClass();
		}
		if (NULL == data)
		{
			break;
		}

		void *param = NULL;
		int valueData = -1;
		data->getParam(m_WCHDATA_ID,param);
		switch (m_dataType)
		{
		case JXJDataType_Int	:
			valueData = *(int *)param;
			break;
		case JXJDataType_UInt	:
			valueData = *(unsigned int *)param;
			break;
		case JXJDataType_Short	:
			valueData = *(short *)param;
			break;
		case JXJDataType_Char	:
			valueData = *(char *)param;
			break;
		case JXJDataType_UChar	:
			valueData = *(unsigned char *)param;
			break;
		default:
			break;
		}
		if (-1 == valueData) break;

		if(m_lastdata == -1)
		{
			//m_lastdata = valueData-m_this->getStep();
			//if(m_this->isCanAdvance())
			//{
			//	m_lastdata += m_this->getStepCount();
			//}
			m_lastdata = valueData;
		}
		else/* if(!m_this->isCanAdvance())*/
		{
			int delta = valueData - m_lastdata;
			if(delta>0)//不计消耗
			{
				m_this->setStep(delta+m_this->getStep(),m_playerGameData->getPlayerID());
			}
			m_lastdata = valueData;
			//if(m_this->isCanAdvance())
			//{
			//	m_lastdata += m_this->getStepCount();
			//}
		}
		//else
		//{
		//	int delta = valueData - m_lastdata+m_this->getStepCount();
		//	if(delta>0)//不计消耗
		//		m_this->setStep(delta,m_playerGameData->getPlayerID());
		//	else
		//	{//消耗补偿
		//		m_lastdata += delta;
		//	}
		//}
	} while (0);
}

/**************************************************************************

METHOD: crJXJCheckHuodongIsValueChangeMethod
 
**************************************************************************/
crJXJCheckHuodongIsValueChangeMethod::crJXJCheckHuodongIsValueChangeMethod():
	m_this(NULL),
	m_WCHDATA_ID(0),
	m_dataType(0)
{

}

crJXJCheckHuodongIsValueChangeMethod::crJXJCheckHuodongIsValueChangeMethod( const crJXJCheckHuodongIsValueChangeMethod & handle ):
	crMethod(handle),
	m_WCHDATA_ID(0),
	m_dataType(0)
{

}

void crJXJCheckHuodongIsValueChangeMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_playerGameData = NULL;
		}
		break;
	}
}

void crJXJCheckHuodongIsValueChangeMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_WCHDATA_ID = atoi(str.c_str());
		break;
	case 1:
		m_dataType = atoi(str.c_str());
		break;
	case 2:
		m_dataStruct = (JXJDataStruct)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJCheckHuodongIsValueChangeMethod::operator()( crHandle &handle )
{
	if(m_this->isCanAdvance())
		return;
	do 
	{
		if (NULL == m_playerGameData || m_playerGameData->getDataClass() == NULL) break;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		crTableIO::StrVec huoRecord;
		int conditionIdx = huodongTab->getTitleIndex("条件");
		int idIdx = huodongTab->getTitleIndex("id");

		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_this->getID()),huoRecord) < 0) break;

		crData * playerGameData = m_playerGameData->getDataClass();

		crData * data = playerGameData;
		if (JXJDataStruct_mainRoleData == m_dataStruct)
		{
			data = m_playerGameData->getMainRole()->getDataClass();
		}
		if (NULL == data)
		{
			break;
		}
		void *param = NULL;
		int valueData = -1;
		data->getParam(m_WCHDATA_ID,param);
		switch (m_dataType)
		{
		case JXJDataType_Int	:
			valueData = *(int *)param;
			break;
		case JXJDataType_UInt	:
			valueData = *(unsigned int *)param;
			break;
		case JXJDataType_Short	:
			valueData = *(short *)param;
			break;
		case JXJDataType_Char	:
			valueData = *(char *)param;
			break;
		case JXJDataType_UChar	:
			valueData = *(unsigned char *)param;
			break;
		default:
			break;
		}
		m_this->setStep(valueData,m_playerGameData->getPlayerID());
	} while (0);
}
/**************************************************************************

METHOD: crJXJShiliOccupyCityCountMethod
 
**************************************************************************/
crJXJShiliOccupyCityCountMethod::crJXJShiliOccupyCityCountMethod():
	m_this(NULL)
{

}

crJXJShiliOccupyCityCountMethod::crJXJShiliOccupyCityCountMethod( const crJXJShiliOccupyCityCountMethod & handle ):
	crMethod(handle)
{

}

void crJXJShiliOccupyCityCountMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_playerGameData = NULL;
		}
		break;
	}
}

void crJXJShiliOccupyCityCountMethod::addParam( int i, const std::string& str )
{
}

void crJXJShiliOccupyCityCountMethod::operator()( crHandle &handle )
{
	if(m_this->isCanAdvance())
		return;
	do 
	{
		if (NULL == m_playerGameData || m_playerGameData->getDataClass() == NULL) break;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		crTableIO::StrVec huoRecord;
		int conditionIdx = huodongTab->getTitleIndex("条件");
		int idIdx = huodongTab->getTitleIndex("id");

		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_this->getID()),huoRecord) < 0) break;

		crData * playerData = m_playerGameData->getDataClass();
		void *param;
		//playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
		//serverData->excHandle(MAKEINT64(WCH_LockData,1));//mainrole 和 serverdata互锁，ChengchiMap不会变动，可以不用锁
		ref_ptr<crTableIO>tab_city = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		ref_ptr<crTableIO> foundCountryCityTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);
		int chengchitype = tab_city->getTitleIndex("类型");
		crTableIO::StrVec chengchiRec,foundRec;
		int chengchicount = 0;
		serverData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *map_city = (ChengchiMap *)param;
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
		//serverData->excHandle(MAKEINT64(WCH_LockData,0));
		//playerData->excHandle(MAKEINT64(WCH_LockData,0));

		m_this->setStep(chengchicount,m_playerGameData->getPlayerID());
	} while (0);
}
//JXJ::crJXJMainRoleHuodongInitMethod::crJXJMainRoleHuodongInitMethod():
//	m_playerGameData(NULL)
//{
//
//}
//
//JXJ::crJXJMainRoleHuodongInitMethod::crJXJMainRoleHuodongInitMethod( const crJXJMainRoleHuodongInitMethod & handle ):
//	crMethod(handle),
//	m_playerGameData(NULL)
//{
//
//}
//
//
//void JXJ::crJXJMainRoleHuodongInitMethod::inputParam( int i, void *param )
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_playerGameData = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crRole*)param;
//		break;
//	}
//}
//
//void JXJ::crJXJMainRoleHuodongInitMethod::addParam( int i, const std::string& str )
//{
//
//}
//void JXJ::crJXJMainRoleHuodongInitMethod::operator()( crHandle &handle )
//{
//	do 
//	{
//		m_playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//
//		if (NULL == m_playerGameData || m_playerGameData->getMainRole() == NULL )
//		{
//			break;
//		}
//		m_playerGameData->doEvent(WCH_JXJHuodongCanvasInit);
//	} while (0);
//
//}

crJXJReloadHuodongTabDataMethod::crJXJReloadHuodongTabDataMethod()
{

}

crJXJReloadHuodongTabDataMethod::crJXJReloadHuodongTabDataMethod( const crJXJReloadHuodongTabDataMethod & handle ):
	crMethod(handle)
{

}

void crJXJReloadHuodongTabDataMethod::inputParam( int i, void *param )
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

void crJXJReloadHuodongTabDataMethod::addParam( int i, const std::string& str )
{
}

void crJXJReloadHuodongTabDataMethod::operator()( crHandle &handle )
{
	ref_ptr<crRole> mainRole = m_this->getMainRole();
	if(mainRole.valid() && mainRole->getDataClass())
	{
		void *param;
		crData *roleData = mainRole->getDataClass();
		roleData->excHandle(MAKEINT64(WCH_LockData,1));
		roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
		NewHuodongMap *huodongMap= (NewHuodongMap *)param;

		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		ref_ptr<crJXJNewHuodong> huodong;
		crTableIO::DataVec hdDataVec;
		crTableIO::StrVec record;
		int openid = huodongTab->getTitleIndex("开启");
		int displayIdx = huodongTab->getTitleIndex("仅显示");
		int delFlagIDx = huodongTab->getTitleIndex("删除标记");
		//int delFlagIDx = huodongTab->getTitleIndex("删除标记");
		int huodongId = 0;
		if(crGlobalHandle::isClient() )
			huodongTab->queryRecords(openid,"1",hdDataVec);
		else 
		{
			huodongTab->queryRecords(openid,"1",displayIdx,"0",hdDataVec);
		}
		int delFlag;
		NewHuodongMap::iterator hitr;
		for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
			itr != hdDataVec.end();
			++itr )
		{
			huodongId = atoi((*itr)[0].c_str());
			hitr = huodongMap->find(huodongId);
			if (hitr == huodongMap->end())
			{
				huodong = new crJXJNewHuodong;
				huodong->setID(huodongId);
				huodong->loadData();
				//if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(huodongId),record)>=0)
				//{
				//	int tmpFlag = atoi(record[delFlagIDx].c_str());
				//	huodong->setDelFlag(tmpFlag);
				//}
				huodongMap->insert(std::make_pair(huodongId,huodong));
			}
			else
			{
				delFlag = atoi((*itr)[delFlagIDx].c_str());
				if(delFlag != hitr->second->getDelFlag())
				{
					hitr->second->loadData();
				}
				else
				{
					unsigned char circles = hitr->second->getCircles();
					hitr->second->loadData();
					hitr->second->setCircles(circles);
				}
			}
		}
		crTableIO::DataVec delDataVec;
		huodongTab->queryRecords(openid,"0",delDataVec);
		for( crTableIO::DataVec::iterator itr = delDataVec.begin();
			itr != delDataVec.end();
			++itr )
		{
			huodongId = atoi((*itr)[0].c_str());
			hitr = huodongMap->find(huodongId);
			if (hitr != huodongMap->end())
			{
				huodongMap->erase(hitr);
			}
		}
		//typedef std::vector<int> ReloadVec;
		//ReloadVec reloadVec;

		//for (HuodongMap::iterator hmItr = huodongMap->begin();
		//	hmItr != huodongMap->end(); ++ hmItr)
		//{				
		//	if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(hmItr->first),record)>=0)
		//	{
		//		int tmpFlag = atoi(record[delFlagIDx].c_str());
		//		if (tmpFlag != hmItr->second->getDelFlag())
		//		{
		//			// 说明策划在这个ID上配了新的活动
		//			reloadVec.push_back(hmItr->first);
		//		}
		//	}
		//}
		//// 清除旧活动的数据，加载新活动
		//for (ReloadVec::iterator rvItr = reloadVec.begin();
		//	rvItr != reloadVec.end(); ++ rvItr)
		//{
		//	HuodongMap::iterator hmItr = huodongMap->find(*rvItr);
		//	if (hmItr != huodongMap->end())
		//	{
		//		hmItr->second->clearData();
		//		hmItr->second->loadData();
		//		int tmpFlag = -1;
		//		if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(hmItr->first),record)>=0)
		//		{
		//			int tmpFlag = atoi(record[delFlagIDx].c_str());
		//			hmItr->second->setDelFlag(tmpFlag);
		//		}
		//	}
		//}
		roleData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

///**************************************************************************
//
//METHOD: crJXJReloadHuodongScriptMethod
// 
//**************************************************************************/
//JXJ::crJXJReloadHuodongScriptMethod::crJXJReloadHuodongScriptMethod()
//{
//
//}
//
//JXJ::crJXJReloadHuodongScriptMethod::crJXJReloadHuodongScriptMethod( const crJXJReloadHuodongScriptMethod & handle ):
//	crMethod(handle),
//	m_canvas(handle.m_canvas)
//{
//}
//
//void JXJ::crJXJReloadHuodongScriptMethod::inputParam( int i, void *param )
//{
//
//}
//
//void JXJ::crJXJReloadHuodongScriptMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_huodongId = atoi(str.c_str());
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJReloadHuodongScriptMethod::operator()( crHandle &handle )
//{
//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
//	crRole *mainRole = playerData->getMainRole();
//	if(mainRole && mainRole->getDataClass())
//	{
//		do 
//		{
//			ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//			if (false == huodongTab.valid()) break;
//
//			crTableIO::StrVec record;
//			int IDIdx = huodongTab->getTitleIndex("id");
//			if (huodongTab->queryOneRecord(IDIdx,crArgumentParser::appItoa(m_huodongId),record) < 0) break;
//
//			int scriptid = huodongTab->getTitleIndex("脚本");
//			if (-1 == scriptid) break;
//
//			void *param;
//			crData *roleData = mainRole->getDataClass();
//			roleData->excHandle(MAKEINT64(WCH_LockData,1));
//			roleData->getParam(WCHDATA_JXJHuodongMap	,param);
//			HuodongMap *huodongMap= (HuodongMap *)param;
//			HuodongMap::iterator hmItr = huodongMap->find(m_huodongId);
//			if (huodongMap->end() != hmItr)
//			{
//				std::string script = record[scriptid];
//				if(!script.empty())
//				{
//					std::string scriptName = crGlobalHandle::getInstance()->getScriptDir()+"huodong/"+script;
//					crScriptLoadManager::getInstance()->	eraseLoadedConfig(scriptName);
//					hmItr->second->loadData();
//				}
//			}
//			roleData->excHandle(MAKEINT64(WCH_LockData,0));
//		} while (0);
//	}
//}

/**************************************************************************

METHOD: crJXJReloadHuodongTabMethod
 
**************************************************************************/
crJXJReloadHuodongTabMethod::crJXJReloadHuodongTabMethod()
{

}

crJXJReloadHuodongTabMethod::crJXJReloadHuodongTabMethod( const crJXJReloadHuodongTabMethod & handle ):
	crMethod(handle)
{

}

void crJXJReloadHuodongTabMethod::inputParam( int i, void *param )
{

}

void crJXJReloadHuodongTabMethod::addParam( int i, const std::string& str )
{
}

void crJXJReloadHuodongTabMethod::operator()( crHandle &handle )
{
	do 
	{
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid()) break;
		int scriptid = huodongTab->getTitleIndex("脚本");
		if (-1 == scriptid) break;
		int rowCount = huodongTab->getRowCount();
		std::string script,scriptName;
		for (int i = 0; i < rowCount; ++ i)
		{
			script = (*huodongTab)(i,scriptid);
			if(!script.empty())
			{
				scriptName = crGlobalHandle::getInstance()->getScriptDir()+"huodong/"+script;
				crScriptLoadManager::getInstance()->eraseLoadedConfig(scriptName);
			}
		}
		crGlobalHandle::gData()->reloadGlobalTable(WCHDATA_JXJHuodongTab/*,"JXJ/script/table/huodong.crb"*/);
		if( crGlobalHandle::isClient() )
		{
			ref_ptr<crPlayerGameData> playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(playerData.valid())
			{
				playerData->doEvent(WCH_JXJReloadHuodongTabData);
				//playerData->doEvent(WCH_JXJHuodongCanvasInit);
			}
		}
		else
		{
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crNetManager *netManager = gameServer->getNetManager();
			ref_ptr<crGameServerPlayerData> playerData;
			netDataManager->lockPlayerDataMap();
			crNetDataManager::PlayerDataMap playerDataMap = netDataManager->getPlayerDataMap();
			netDataManager->unLockPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (playerData.valid())
				{
					playerData->getPlayerGameData()->doEvent(WCH_JXJReloadHuodongTabData);
				}
			}
		}
	}while (0);
}

/**************************************************************************

METHOD: crJXJCheckHuodongServerOpenDayMethod
 
**************************************************************************/
crJXJCheckHuodongServerOpenDayMethod::crJXJCheckHuodongServerOpenDayMethod()
{

}

crJXJCheckHuodongServerOpenDayMethod::crJXJCheckHuodongServerOpenDayMethod( const crJXJCheckHuodongServerOpenDayMethod & handle ):
	crMethod(handle)
{

}

void crJXJCheckHuodongServerOpenDayMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	}
}

void crJXJCheckHuodongServerOpenDayMethod::addParam( int i, const std::string& str )
{

}

void crJXJCheckHuodongServerOpenDayMethod::operator()( crHandle &handle )
{
	bool suc = false;
	do 
	{
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		if (false == huodongTab.valid())
		{
			break;
		}
		int condition2 = huodongTab->getTitleIndex("条件2");
		int idIdx = huodongTab->getTitleIndex("id");
		crTableIO::StrVec record;
		if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_this->getID()),record) >= 0)
		{
			int serverOpenDay = atoi(record[condition2].c_str());
			std::string dateStr = crGlobalHandle::gData()->gStartDate();
			typedef std::vector<int> Vec5i;
			Vec5i startTimeVec5i;
			startTimeVec5i.reserve(5);
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
			if (nowTime_t - startTime_t <= serverOpenDay * 86400)
			{
				suc = true;
			}
		}
	} while (0);

	handle.outputParam(0,&suc);
}

/**************************************************************************

METHOD: crJXJSYNShiliDataMethod
 
**************************************************************************/
JXJ::crJXJSYNShiliDataMethod::crJXJSYNShiliDataMethod():
	m_this(NULL)
{

}

JXJ::crJXJSYNShiliDataMethod::crJXJSYNShiliDataMethod( const crJXJSYNShiliDataMethod& handle ):
	crMethod(handle),
	m_this(NULL)
{
}

void JXJ::crJXJSYNShiliDataMethod::inputParam( int i, void *param )
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
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJSYNShiliDataMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJSYNShiliDataMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid() && m_this->getDataClass())
	{
		void *param;
		//crData *braindata = crBrain::getInstance()->getDataClass();
		//braindata->getParam(WCHDATA_JXJMyCountryData,param);
		crData * data = m_this->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJMyCountryData,param);
		crJXJShili *shili = (crJXJShili *)param;
		if (!shili)
		{
			shili = new crJXJShili;
			shili->loadData();
		}
		if (shili)
		{
			crData * shiliData = shili->getDataClass();
			if (shiliData)
			{
				shiliData->inputParam(WCHDATA_PlayerStream,m_stream.get());
				data->inputParam(WCHDATA_JXJMyCountryData,shili);
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/**************************************************************************

METHOD: JXJGroupContributeMsg
 
**************************************************************************/

void JXJ::JXJGroupContributeSourceMsg::getCompleteString( std::string & outString)
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&m_name);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&crArgumentParser::appItoa(m_copper));
		noticeHandle->inputParam(WCHDATA_NoticeParam3,&crArgumentParser::appItoa(m_food));
		noticeHandle->inputParam(WCHDATA_NoticeParam4,&crArgumentParser::appItoa(m_wood));
		noticeHandle->inputParam(WCHDATA_JXJNoticeParam5	,&crArgumentParser::appItoa(m_iron));
		noticeHandle->inputParam(WCHDATA_JXJNoticeParam6	,&crArgumentParser::appItoa(m_horse));
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";

}

void JXJ::JXJGroupContributeSourceMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_name = stream->_readString();
	m_copper = stream->_readInt();
	m_food = stream->_readInt();
	m_wood = stream->_readInt();
	m_iron = stream->_readInt();
	m_horse = stream->_readInt();
}

void JXJ::JXJGroupContributeSourceMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_name);
	stream->_writeInt(m_copper);
	stream->_writeInt(m_food);
	stream->_writeInt(m_wood);
	stream->_writeInt(m_iron);
	stream->_writeInt(m_horse);
}

/**************************************************************************

METHOD: JXJGroupContributeYuanbaoMsg
 
**************************************************************************/
void JXJ::JXJGroupContributeYuanbaoMsg::getCompleteString( std::string & outString)
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&m_name);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&crArgumentParser::appItoa(m_yuanbao));
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";

}

void JXJ::JXJGroupContributeYuanbaoMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_name = stream->_readString();
	m_yuanbao = stream->_readInt();
}

void JXJ::JXJGroupContributeYuanbaoMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_name);
	stream->_writeInt(m_yuanbao);
}

/**************************************************************************

METHOD: JXJGroupStartFuBenMsg
 
**************************************************************************/
void JXJ::JXJGroupStartFuBenMsg::getCompleteString( std::string & outString)
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&m_name);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&m_fubenName);
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";
}

void JXJ::JXJGroupStartFuBenMsg::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_name = stream->_readString();
	m_fubenName = stream->_readString();
}

void JXJ::JXJGroupStartFuBenMsg::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_name);
	stream->_writeString(m_fubenName);
}

/**************************************************************************

METHOD: crJXJAddNoChatPlayerMethod
 
**************************************************************************/
JXJ::crJXJAddNoChatPlayerMethod::crJXJAddNoChatPlayerMethod()
{

}

JXJ::crJXJAddNoChatPlayerMethod::crJXJAddNoChatPlayerMethod( const crJXJAddNoChatPlayerMethod & handle ):
	m_canvas(handle.m_canvas),
	m_nameInput(handle.m_nameInput),
	m_op(handle.m_op)
{

}

void JXJ::crJXJAddNoChatPlayerMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJAddNoChatPlayerMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_nameInput = str;
		break;
	case 2:
		m_op = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJAddNoChatPlayerMethod::operator()( crHandle &handle )
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && netConductor)
	{
		ref_ptr<crStaticTextWidgetNode > nameInput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_nameInput));
		if (nameInput.valid())
		{

			if (JXJGMOperation_downloadFile	== m_op)
			{
				crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
				if (data)
				{
					data->insertDownloadFile(WCHDATA_JXJGameServerLogFile,"../JXJDB/" + crMyPlayerData::getInstance()->getServerName() + "/gamelog.tab");
					data->startDownloadFile(WCHDATA_JXJGameServerLogFile,GameClient_Game);
					nameInput->setString(std::string("正在下载文件"));
					canvas->doEvent(WCH_UI_UpdateData);
				}
				return;
			}
			crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(12);
			stream->_writeString(nameInput->getUTF8String());
			stream->_writeUChar(m_op);
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJAddNoChatPlayer,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/**************************************************************************

METHOD: crJXJRecvAddNoChatPlayerMethod
 
**************************************************************************/
JXJ::crJXJRecvAddNoChatPlayerMethod::crJXJRecvAddNoChatPlayerMethod():
	m_this(NULL)
{

}

JXJ::crJXJRecvAddNoChatPlayerMethod::crJXJRecvAddNoChatPlayerMethod( const crJXJRecvAddNoChatPlayerMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_nameInput(handle.m_nameInput)
{

}

void JXJ::crJXJRecvAddNoChatPlayerMethod::inputParam( int i, void *param )
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

void JXJ::crJXJRecvAddNoChatPlayerMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_nameInput = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJRecvAddNoChatPlayerMethod::operator()( crHandle &handle )
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int myPlayerID = m_this->getPlayerID();
			std::string name = m_stream->_readString();
			unsigned char op = m_stream->_readUChar();

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(myPlayerID));
			if (playerData.valid())
			{
				int quanxian = crGlobalHandle::gData()->getGMAuthority(myPlayerID);
				//bool isGMer = crGlobalHandle::gData()->getGMAuthority(myPlayerID)== JXJGMPermission_manager;
				//CRCore::ref_ptr<CREncapsulation::crTableIO> GMTab = crGlobalHandle::gData()->gGMTable();
				//if (GMTab.valid())
				//{
				//	int permissionIdx = GMTab->getTitleIndex("权限");
				//	int idIdx = GMTab->getTitleIndex("playerid");

				//	crTableIO::StrVec record;
				//	// 判断是否是GMer
				//	for (int i = 0; i < GMTab->getRowCount(); ++i)
				//	{
				//		if (GMTab->queryOneRecord(idIdx,crArgumentParser::appItoa(playerData->getPlayerID()),record) >= 0)
				//		{
				//			if (atoi(record[permissionIdx].c_str()) == JXJGMPermission_manager)
				//			{
				//				isGMer = true;
				//				break;
				//			}
				//		}
				//	}
				//}
				unsigned char retVal = JXJGMOperationRet_opFailed	;		
				if (quanxian>0 && quanxian<JXJGMPermission_newer)
				{
					crJXJShili * shili = NULL;
					crData *shiliData = NULL;
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();

					int otherID = 0;

					if (name != "")
					{
						void *param;
						for (int shiliId = WCHDATA_JXJShiliWei; shiliId <= WCHDATA_JXJShiliWu	; ++ shiliId )
						{
							gsBrainData->getParam(shiliId,param);
							shili = (crJXJShili *)param;
							if (shili)
							{
								shiliData = shili->getDataClass();
								shiliData->excHandle(MAKEINT64(WCH_LockData,1));
								shiliData->getParam(WCHDATA_JXJPeopleMap,param);
								PeopleMap * peopleMap = (PeopleMap *)param;
								PeopleMap::iterator pmItr = peopleMap->begin();
								for (; pmItr != peopleMap->end(); ++ pmItr)
								{
									if (pmItr->second.valid())
									{
										if (pmItr->second->getName() == name)
										{
											otherID = pmItr->first;
											break;
										}
									}
								}
								shiliData->excHandle(MAKEINT64(WCH_LockData,0));
							}
							if (otherID)
							{
								break;
							}
						}
					}
					if (otherID)
					{
						switch (op)
						{
						case JXJGMOperation_addNoChat:
						case JXJGMOperation_delNoChat:	
							//{
							//	CRCore::ref_ptr<CREncapsulation::crTableIO> noChatTab = crJXJGameChatPacket::getNoChatTabPtr();

							//	bool find = false;
							//	bool modify = false;
							//	int row = 0;
							//	int idIdx = noChatTab->getTitleIndex("playerid");
							//	crTableIO::StrVec record;
							//	for (int i = 0; i < noChatTab->getRowCount(); ++i)
							//	{
							//		if (atoi(record[idIdx].c_str()) == otherID)
							//		{
							//			find = true;
							//			row = i;
							//			break;
							//		}
							//	}
							//	if (JXJGMOperation_addNoChat == op)
							//	{
							//		record.clear();
							//		record.resize(noChatTab->getColumnCount());
							//		record[idIdx] = crArgumentParser::appItoa(otherID);
							//		if (find || (modify = noChatTab->addData(record)))
							//		{
							//			retVal = JXJGMOperationRet_suc	;	

							//			std::string logdata = name + "被 " + playerData->getCharacterName() + "禁言";
							//			GameLogData gamelog(Log_GMAddNoChatPlayer,logdata);
							//			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(otherID,&gamelog));
							//		}
							//	}
							//	else if (JXJGMOperation_delNoChat == op)
							//	{
							//		if (find) 
							//		{
							//			noChatTab->removeRow(row);
							//			modify = true;
							//		}

							//		retVal = JXJGMOperationRet_suc	;	
							//		std::string logdata = name + "被 " + playerData->getCharacterName() + "解除禁言";
							//		GameLogData gamelog(Log_GMDelNoChatPlayer,logdata);
							//		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(otherID,&gamelog));
							//	}
							//	if (modify)
							//	{
							//		noChatTab->saveToFileStream("nochat.crb");
							//		//noChatTab->saveToFileNoCook("nochat.tab");
							//	}
							//}
							break;
						case JXJGMOperation_kickOff	:
							{
								crNetManager *netManager = gameServer->getNetManager();
								crNetDataManager *netDataManager = gameServer->getNetDataManager();
								ref_ptr<crGameServerPlayerData> connectedPlayerData  = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(myPlayerID));
								if(connectedPlayerData.valid())
								{
									crKickoffPlayerPacket kpacket;
									crKickoffPlayerPacket::buildReplyPacket(kpacket,otherID);
									netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
									if(!connectedPlayerData->getSceneServerAddress().empty())
									{
										netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
									}
									netDataManager->removePlayerData(otherID);
									retVal = JXJGMOperationRet_suc	;	
								}
								if (JXJGMOperationRet_suc == retVal)
								{
									std::string logdata = name + "被 " + playerData->getCharacterName() + "踢下线";
									GameLogData gamelog(Log_GMKickOffPlayer,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(otherID,&gamelog));
								}
							}
							break;
						case JXJGMOperation_lockPlayer:
							{
								crNetManager *netManager = gameServer->getNetManager();
								crNetDataManager *netDataManager = gameServer->getNetDataManager();
								ref_ptr<crGameServerPlayerData> connectedPlayerData  = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(myPlayerID));
								if(connectedPlayerData.valid())
								{
									crKickoffPlayerPacket kpacket;
									crKickoffPlayerPacket::buildReplyPacket(kpacket,otherID);
									netManager->sendPacket(connectedPlayerData->getPlayerConnectServerAddress(),kpacket);
									if(!connectedPlayerData->getSceneServerAddress().empty())
									{
										netManager->sendPacket(connectedPlayerData->getSceneServerAddress(),kpacket);
									}
									netDataManager->removePlayerData(otherID);
								}
								crDataBase *accountdb = crDataBaseManager::getInstance()->getAccountDB();
								ref_ptr<crDataBase> db_session = accountdb ->beginSession();
								ref_ptr<crJXJLockPlayerdata> lockPlayerData = new crJXJLockPlayerdata;
								lockPlayerData->buildUpdateSql(otherID);
								if (db_session->executeUpdate(lockPlayerData.get()))
								{
									db_session->commit();
									retVal = JXJGMOperationRet_suc;
								}
								else
								{
									db_session->rollback();
								}
								accountdb->endSession(db_session.get());

								if (JXJGMOperationRet_suc == retVal)
								{
									std::string logdata = name + "被 " + playerData->getCharacterName() + "锁住账号";
									GameLogData gamelog(Log_GMLockPlayer,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(otherID,&gamelog));
								}
							}
							break;
						case JXJGMOperation_unlockPlayer:
							{
								crDataBase *accountdb= crDataBaseManager::getInstance()->getAccountDB();
								ref_ptr<crDataBase> db_session = accountdb->beginSession();
								ref_ptr<crJXJUnLockPlayerdata> unlockPlayerData = new crJXJUnLockPlayerdata;
								unlockPlayerData->buildUpdateSql(otherID);
								if (db_session->executeUpdate(unlockPlayerData.get()))
								{
									db_session->commit();
									retVal = JXJGMOperationRet_suc;
								}
								else 
								{
									db_session->rollback();
								}
								accountdb->endSession(db_session.get());

								if (JXJGMOperationRet_suc == retVal)
								{
									std::string logdata = name + "被 " + playerData->getCharacterName() + "解锁账号";
									GameLogData gamelog(Log_GMUnlockPlayer,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(otherID,&gamelog));
								}
							}
							break;
						default:
							break;
						}
					}
					else
					{
						if (op == JXJGMOperation_kickAllPlayerOff)
						{
							crNetManager *netManager = gameServer->getNetManager();
							crNetDataManager *netDataManager = gameServer->getNetDataManager();

							typedef std::vector<int> DelPlayerIDVec;

							netDataManager->lockPlayerDataMap();
							crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
							DelPlayerIDVec delPlayerIDVec;
							delPlayerIDVec.reserve(playerDataMap.size());
							for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
								itr != playerDataMap.end();
								++itr )
							{
								ref_ptr<crGameServerPlayerData > otherData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
								if (otherData.valid() && playerData != otherData)
								{
									crKickoffPlayerPacket kpacket;
									crKickoffPlayerPacket::buildReplyPacket(kpacket,otherData->getPlayerID());
									netManager->sendPacket(otherData->getPlayerConnectServerAddress(),kpacket);
									// 不踢自己
									if(!otherData->getSceneServerAddress().empty())
									{
										netManager->sendPacket(otherData->getSceneServerAddress(),kpacket);
									}
									delPlayerIDVec.push_back(otherData->getPlayerID());
									retVal = JXJGMOperationRet_suc	;	
								}
							}
							for (DelPlayerIDVec::iterator itr = delPlayerIDVec.begin();
								delPlayerIDVec.end() != itr; ++ itr)
							{
								netDataManager->removePlayerData(*itr);
							}
							netDataManager->unLockPlayerDataMap();

							if (JXJGMOperationRet_suc == retVal)
							{
								std::string logdata = playerData->getCharacterName() + "将所有玩家踢下线";
								GameLogData gamelog(Log_GMKickOffPlayer,logdata);
								crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerData->getPlayerID(),&gamelog));
							}
						}
						else
							retVal = JXJGMOperationRet_noExit;	
					}
				}
				else
				{
					retVal = JXJGMOperationRet_notGMer;	
				}
				crPlayerDataEventPacket packet;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(retVal);
				crPlayerDataEventPacket::buildReplyPacket(packet,myPlayerID,WCH_JXJAddNoChatPlayer,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char retVal = m_stream->_readUChar();
			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid())
			{
				ref_ptr<crStaticTextWidgetNode > nameInput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_nameInput));
				if (nameInput.valid())
				{
					switch (retVal)
					{
					case JXJGMOperationRet_opFailed:
						nameInput->setString(std::string("操作失败"));
						break;
					case JXJGMOperationRet_suc	:	
						nameInput->setString(std::string("操作成功"));
						break;
					case JXJGMOperationRet_noExit:	
						nameInput->setString(std::string("玩家不存在"));
						break;
					case JXJGMOperationRet_notGMer:
						nameInput->setString(std::string("您没有GM权限"));
						break;
					default:
						break;
					}
				}
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJGetAwardsByActiveCodeMethod
 
**************************************************************************/
JXJ::crJXJGetAwardsByActiveCodeMethod::crJXJGetAwardsByActiveCodeMethod():
	m_this(NULL)
{

}

JXJ::crJXJGetAwardsByActiveCodeMethod::crJXJGetAwardsByActiveCodeMethod( const crJXJGetAwardsByActiveCodeMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJGetAwardsByActiveCodeMethod::inputParam( int i, void *param )
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

void JXJ::crJXJGetAwardsByActiveCodeMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJGetAwardsByActiveCodeMethod::operator()( crHandle &handle )
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
		//与活动配置的激活码类型不一致
		if( huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(m_activeCodeData->m_huodongID),record)<0
			|| atoi(record[codeTypeIdx].c_str()) != activeCodeQuery->getCodeType() )
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid;	
			break;
		}
		// 2,如果指定了获得激活码的玩家，并且该玩家不是拥有者
		/*	if (activeCodeQuery->getGetterID() != 0 
		&& activeCodeQuery->getGetterID() != m_this->getPlayerID())
		{
		*m_retVal = JXJActiveCodeGetRet_failed_notYours	;	
		break;
		}*/
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
				*m_retVal = JXJActiveCodeGetRet_failed_invalid;	
			}
		}
		else
		{
			*m_retVal = JXJActiveCodeGetRet_failed_invalid;
		}
		globalSession->releaseQuery();
		db->endSession(globalSession.get());
		if(*m_retVal != JXJActiveCodeGetRet_suc)
			break;

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

///**************************************************************************
//
//METHOD: crJXJRecvHuodongGetAwardByActiveCodeMethod
// 
//**************************************************************************/
//JXJ::crJXJRecvHuodongGetAwardByActiveCodeMethod::crJXJRecvHuodongGetAwardByActiveCodeMethod():
//	m_this(NULL),
//	m_stream(NULL)
//{
//
//}
//
//JXJ::crJXJRecvHuodongGetAwardByActiveCodeMethod::crJXJRecvHuodongGetAwardByActiveCodeMethod( const crJXJRecvHuodongGetAwardByActiveCodeMethod & handle ):
//	crMethod(handle),
//	m_this(NULL),
//	m_stream(NULL)
//{
//
//}
//
//void JXJ::crJXJRecvHuodongGetAwardByActiveCodeMethod::inputParam( int i, void *param )
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
//void JXJ::crJXJRecvHuodongGetAwardByActiveCodeMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_canvas = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void JXJ::crJXJRecvHuodongGetAwardByActiveCodeMethod::operator()( crHandle &handle )
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			int playerid = m_this->getPlayerID();
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			crRole *mainRole = m_this->getMainRole();
//			if(playerData.valid() && mainRole && mainRole->getDataClass())
//			{
//				unsigned short huodongID = m_stream->_readShort();
//				std::string activeCode = m_stream->_readString();
//
//				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
//				crTableIO::StrVec record;
//
//				int codeTypeIdx = huodongTab->getTitleIndex("激活码类型");
//
//				int awardIdx[5] ;
//				for (unsigned char i = 0; i < 5; ++ i)
//				{
//					awardIdx[i] = -1;
//				}
//				int idIdx = -1;
//				if (huodongTab.valid())
//				{
//					idIdx = huodongTab->getTitleIndex("id");
//					awardIdx[0] = huodongTab->getTitleIndex("奖励1");
//					awardIdx[1] = huodongTab->getTitleIndex("奖励2");
//					awardIdx[2] = huodongTab->getTitleIndex("奖励3");
//					awardIdx[3] = huodongTab->getTitleIndex("奖励4");
//					awardIdx[4] = huodongTab->getTitleIndex("奖励5");
//				}
//				CRCore::crVector3i awardVec;
//
//				char retVal = 0;
//				if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongID),record) >= 0)
//				{
//					if (atoi(record[codeTypeIdx].c_str()))
//					{
//						// 说明是领取的是激活码活动
//						ActiveCodeData activeCodeData ;
//						activeCodeData.m_activeCode = activeCode;
//						activeCodeData.m_huodongID = huodongID;
//						m_this->doEvent(WCH_JXJGetAwardsByActiveCode	,MAKEINT64(&activeCodeData,&retVal));
//						if (JXJActiveCodeGetRet_suc == retVal)
//						{
//							RewardItemVec rewardItems;
//							rewardItems.reserve(5);
//							CRCore::ref_ptr<crBagItemData> itemPtr = NULL;
//							for (unsigned char i = 0; i < 5; ++ i)
//							{
//								crArgumentParser::appAtoVec(record[awardIdx[i]],awardVec);
//								itemPtr = new crBagItemData;
//								itemPtr->setItemID(awardVec[0]);
//								itemPtr->setEquipMagic(awardVec[1]);
//								itemPtr->setItemCount(awardVec[2]);
//								rewardItems.push_back(itemPtr);
//							}
//							m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
//						}
//					}
//				}
//
//				crPlayerDataEventPacket packet;
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				stream->createBuf(1);
//				stream->_writeUChar(retVal);
//				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvHuodongGetAwardByActiveCode,stream.get());
//				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//			}
//		}
//		else if (m_netType == GameClient_Game)
//		{
//			unsigned char retVal = m_stream->_readUChar();
//			ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//			if (canvas.valid())
//			{
//				int msgid = 0;
//				switch (retVal)
//				{
//				case JXJActiveCodeGetRet_failed_hadGetten:
//					msgid = 4080;
//					break;
//				case JXJActiveCodeGetRet_failed_invalid	:
//					msgid = 4081;
//					break;
//				case JXJActiveCodeGetRet_failed_outOfDate:
//					msgid = 4082;
//					break;
//				case JXJActiveCodeGetRet_failed_databaseError:
//					msgid = 4083;
//					break;
//				default:
//					break;
//				}
//				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(msgid,NULL));
//			}
//		}
//	}
//}

/**************************************************************************

METHOD: crJXJCheckPlayerLoginDayMethod
 
**************************************************************************/
crJXJCheckPlayerLoginDayMethod::crJXJCheckPlayerLoginDayMethod():
	m_this(NULL)
{

}

crJXJCheckPlayerLoginDayMethod::crJXJCheckPlayerLoginDayMethod( const crJXJCheckPlayerLoginDayMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void crJXJCheckPlayerLoginDayMethod::inputParam( int i, void *param )
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
		m_this = (crJXJNewHuodong *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerGameData = (CRNetApp::crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_playerGameData = NULL;
		}
		break;
	}
}

void crJXJCheckPlayerLoginDayMethod::addParam( int i, const std::string& str )
{

}

void crJXJCheckPlayerLoginDayMethod::operator()( crHandle &handle )
{
	if(m_this->isCanAdvance())
		return;
	ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	int condition1 = huodongTab->getTitleIndex("条件");
	crTableIO::StrVec record;
	if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(m_this->getID()),record) >= 0)
	{
		int serverOpenDay = atoi(record[condition1].c_str());
		std::string dateStr = crGlobalHandle::gData()->gStartDate();
		typedef std::vector<int> Vec5i;
		Vec5i startTimeVec5i;
		startTimeVec5i.reserve(5);
		crArgumentParser::appAtoVec(dateStr,startTimeVec5i);

		startTimeVec5i.resize(5,0);

		time_t nowTime_t = time(0);
		struct  tm  nowTm = *localtime(&nowTime_t);
		struct tm  startTm ;
		memset(&startTm,0,sizeof (startTm));

		startTm.tm_year = startTimeVec5i[0] - 1900;
		startTm.tm_mon = startTimeVec5i[1] - 1;
		startTm.tm_mday = startTimeVec5i[2];
		startTm.tm_hour = 0;
		startTm.tm_min = 0;
		startTm.tm_isdst = nowTm.tm_isdst;

		time_t startTime_t = mktime(&startTm);
		if ( (nowTime_t - startTime_t) / 86400 == serverOpenDay - 1)
		{
			//m_this->setStepCount(serverOpenDay,m_playerGameData->getPlayerID(),true);
			m_this->setStep(serverOpenDay,m_playerGameData->getPlayerID());
		}
		//startTm = *localtime(&startTime_t);

		//if (nowTm.tm_year == startTm.tm_year)
		//{
		//	if (nowTm.tm_yday == startTm.tm_yday + serverOpenDay - 1 )
		//	{
		//		m_this->setStepCount(serverOpenDay,m_playerGameData->getPlayerID(),true);
		//	}
		//}
		//else if(nowTm.tm_year== startTm.tm_year + 1)
		//{
		//	if (startTm.tm_mon == 11 && 31 - startTm.tm_mday + nowTm.tm_yday + 1 == serverOpenDay)
		//	{
		//		m_this->setStepCount(serverOpenDay,m_playerGameData->getPlayerID(),true);
		//	}
		//}
	}
}

crJXJHuodongJumpMethod::crJXJHuodongJumpMethod()
{

}

crJXJHuodongJumpMethod::crJXJHuodongJumpMethod( const crJXJHuodongJumpMethod & handle ):
	m_canvas(handle.m_canvas),
	m_linkType(handle.m_linkType)
{

}

void crJXJHuodongJumpMethod::inputParam( int i, void *param )
{

}

void crJXJHuodongJumpMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_linkType = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJHuodongJumpMethod::operator()( crHandle &handle )
{

	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		bool valid = true;
		void *param;
		crData * canvasData = canvas->getDataClass();

		canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		canvasData->getParam(WCHDATA_JXJHuodongSelectId	,param);
		unsigned short selectID = *(unsigned short *)param;

		canvasData->getParam(WCHDATA_JXJHuodongVec,param);
		HuodongVec & huodongVec = *(HuodongVec *)param;

		int urlIdx = -1;
		ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		switch (m_linkType)
		{
		case JXJHuodongUrl_failedJumpLink	:	
			urlIdx = huodongTab->getTitleIndex("提示跳转链接");
			break;
		case JXJHuodongUrl_infoLink			:
			urlIdx = huodongTab->getTitleIndex("活动链接");
			break;
		case JXJHuodongUrl_officialLink		:
			urlIdx = huodongTab->getTitleIndex("官网链接");
			break;
		default:
			break;
		}
		int idIdx = huodongTab->getTitleIndex("id");
		crTableIO::StrVec record;
		if (huodongVec.size() > selectID && huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(huodongVec[selectID]),record) >= 0)
		{
			if (record[urlIdx] != "")
			{
				ShellExecute(NULL,"open",record[urlIdx].c_str(),NULL,NULL,SW_SHOW);
			}
		}
		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/**************************************************************************

METHOD: crJXJSetCurrentHuodongIdxMethod
 
**************************************************************************/

JXJ::crJXJSetCurrentHuodongIdxMethod::crJXJSetCurrentHuodongIdxMethod():
	m_this(NULL)
{

}

JXJ::crJXJSetCurrentHuodongIdxMethod::crJXJSetCurrentHuodongIdxMethod( const crJXJSetCurrentHuodongIdxMethod & handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{

}

void JXJ::crJXJSetCurrentHuodongIdxMethod::inputParam( int i, void *param )
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

void JXJ::crJXJSetCurrentHuodongIdxMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_scroll = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJSetCurrentHuodongIdxMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (!canvas.valid() || canvas->getDataClass() == NULL) return;
	ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scroll));
	crData * canvasData = canvas->getDataClass();
	canvasData->excHandle(MAKEINT64(WCH_LockData,1));

	void *param;
	canvasData->getParam(WCHDATA_JXJHuodongVec,param);
	HuodongVec & huodongVec = *(HuodongVec *)param;

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

	canvasData->getParam(WCHDATA_JXJHuodongSelectIdx	,param);
	unsigned short m_idx = *(unsigned short* )param;

	unsigned short idx = curPage + m_idx;
	canvasData->inputParam(WCHDATA_JXJHuodongSelectId	,&idx);

	canvasData->excHandle(MAKEINT64(WCH_LockData,0));
}

/**************************************************************************

METHOD: crJXJClientRecvServerLogMethod
 
**************************************************************************/
JXJ::crJXJClientRecvServerLogMethod::crJXJClientRecvServerLogMethod()
{

}

JXJ::crJXJClientRecvServerLogMethod::crJXJClientRecvServerLogMethod( const crJXJClientRecvServerLogMethod& handle ):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input)
{

}

void JXJ::crJXJClientRecvServerLogMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJClientRecvServerLogMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJClientRecvServerLogMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode > nameInput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
		if (nameInput.valid())
		{
			nameInput->setString(std::string("下载完成"));
		}
	}
}

/**************************************************************************

METHOD: crJXJClientDownLoadLogFilesMethod
 
**************************************************************************/
JXJ::crJXJClientDownLoadLogFilesMethod::crJXJClientDownLoadLogFilesMethod():
	m_msgid(0)
{

}

JXJ::crJXJClientDownLoadLogFilesMethod::crJXJClientDownLoadLogFilesMethod( const crJXJClientDownLoadLogFilesMethod & handle ):
	crMethod(handle),
	m_fileName(handle.m_fileName),
	m_msgid(handle.m_msgid)
{

}

void JXJ::crJXJClientDownLoadLogFilesMethod::inputParam( int i, void *param )
{

}

void JXJ::crJXJClientDownLoadLogFilesMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_msgid = atoi(str.c_str());
		break;
	case 1:
		m_fileName = str;
		break;
	default:
		break;
	}

}

void JXJ::crJXJClientDownLoadLogFilesMethod::operator()( crHandle &handle )
{
	std::string relFileName = "../JXJDB/" + crMyPlayerData::getInstance()->getServerName() + "/"+ m_fileName;
	crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
	if (data)
	{
		data->insertDownloadFile(m_msgid,relFileName);
		data->startDownloadFile(m_msgid,GameClient_Game);
	}
}

/**************************************************************************

METHOD: crJXJExtraCardAddRTMPMethod
 
**************************************************************************/
JXJ::crJXJExtraCardAddRTMPMethod::crJXJExtraCardAddRTMPMethod():
	m_this(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f)
{

}

JXJ::crJXJExtraCardAddRTMPMethod::crJXJExtraCardAddRTMPMethod( const crJXJExtraCardAddRTMPMethod & handle ):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{

}

void JXJ::crJXJExtraCardAddRTMPMethod::operator()( crHandle &handle )
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		if(m_timer<0.0f)
		{
			m_timer = m_duration;

			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));

			m_this->doEvent(MAKEINT64(WCH_JXJCureRTMp,0),MAKEINT64(&m_extraValue,NULL));
			thisData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

void JXJ::crJXJExtraCardAddRTMPMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			//m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}

}

void JXJ::crJXJExtraCardAddRTMPMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = atof(str.c_str());
		break;
	default:
		break;
	}
}
