/**********************************************************************
*
*	文件:	   appPackets.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPPACKETS_H
#define JXJ_APPPACKETS_H 1

#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNet\crStreamPacket.h>
#include <CRNet\crNetContainer.h>
#include <CRNetApp/appExport.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRNetApp/appNetDefine.h>
#include <JXJ/appDefine.h>
#include <JXJ/appDataParam.h>
#include <CREncapsulation/crTableIO.h>
namespace JXJ{
class crBattleOpenPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crBattleOpenPacket> sptr;
	typedef GNE::WeakPtr<crBattleOpenPacket> wptr;
public :
	crBattleOpenPacket();
	crBattleOpenPacket( const crBattleOpenPacket &p );

	virtual ~crBattleOpenPacket() {}
public:
	static const int ID = WCHNET_JXJBattleOpen;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crBattleOpenPacket &packet,int playerid,bool openBattle/*,bool openPass*/);
	static void buildReplyPacket2(crBattleOpenPacket &packet,CRCore::crStreamBuf *stream);
};
class crQueryBattleTimerPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crQueryBattleTimerPacket> sptr;
	typedef GNE::WeakPtr<crQueryBattleTimerPacket> wptr;
public :
	crQueryBattleTimerPacket();
	crQueryBattleTimerPacket( const crQueryBattleTimerPacket &p );

	virtual ~crQueryBattleTimerPacket() {}
public:
	static const int ID = WCHNET_JXJQueryBattleTimer;
public:
	virtual void parsePacket(const std::string &sender);
	//static void buildRequestPacket(crQueryBattleTimerPacket &packet,unsigned char flg);
	static void buildRequestPacket2(crQueryBattleTimerPacket &packet,int playerid/*,CRCore::crStreamBuf *stream*/);
	static void buildReplyPacket(crQueryBattleTimerPacket &packet,int playerid,/*unsigned char flg,*/int time);
	static void buildReplyPacket2(crQueryBattleTimerPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJPlayerLoginGamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJPlayerLoginGamePacket> sptr;
	typedef GNE::WeakPtr<crJXJPlayerLoginGamePacket> wptr;
public :
	crJXJPlayerLoginGamePacket();
	crJXJPlayerLoginGamePacket( const crJXJPlayerLoginGamePacket &p );

	virtual ~crJXJPlayerLoginGamePacket() {}
public:
	static const int ID = WCHNET_JXJPlayerLoginGame;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJPlayerLoginGamePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJPlayerLoginGamePacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJSearchBattlePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJSearchBattlePacket> sptr;
	typedef GNE::WeakPtr<crJXJSearchBattlePacket> wptr;
public :
	crJXJSearchBattlePacket();
	crJXJSearchBattlePacket( const crJXJSearchBattlePacket &p );

	virtual ~crJXJSearchBattlePacket() {}
public:
	static const int ID = WCHNET_JXJSearchBattle;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJSearchBattlePacket &packet,int roomid);
	static void buildRequestPacket2(crJXJSearchBattlePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJSearchBattlePacket &packet,int playerid,CRCore::crStreamBuf *roomdata);
	static void buildReplyPacket2(crJXJSearchBattlePacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJSearchSceneRoomPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJSearchSceneRoomPacket> sptr;
	typedef GNE::WeakPtr<crJXJSearchSceneRoomPacket> wptr;
public :
	crJXJSearchSceneRoomPacket();
	crJXJSearchSceneRoomPacket( const crJXJSearchSceneRoomPacket &p );

	virtual ~crJXJSearchSceneRoomPacket() {}
public:
	static const int ID = WCHNET_JXJSearchSceneRoom;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJSearchSceneRoomPacket &packet,int sceneid);
	static void buildRequestPacket2(crJXJSearchSceneRoomPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};

class crJXJRandomPlayerNamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJRandomPlayerNamePacket> sptr;
	typedef GNE::WeakPtr<crJXJRandomPlayerNamePacket> wptr;
public :
	crJXJRandomPlayerNamePacket();
	crJXJRandomPlayerNamePacket( const crJXJRandomPlayerNamePacket &p );

	virtual ~crJXJRandomPlayerNamePacket() {}
public:
	static const int ID = WCHNET_JXJRandomPlayerName;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJRandomPlayerNamePacket &packet,unsigned char sex);
	static void buildRequestPacket2(crJXJRandomPlayerNamePacket &packet,int playerid, unsigned char sex);
	static void buildReplyPacket(crJXJRandomPlayerNamePacket &packet,int playerid,const std::string &name);
	static void buildReplyPacket2(crJXJRandomPlayerNamePacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJGameServerCreateRoomPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJGameServerCreateRoomPacket> sptr;
	typedef GNE::WeakPtr<crJXJGameServerCreateRoomPacket> wptr;
public :
	crJXJGameServerCreateRoomPacket();
	crJXJGameServerCreateRoomPacket( const crJXJGameServerCreateRoomPacket &p );

	virtual ~crJXJGameServerCreateRoomPacket() {}
public:
	static const int ID = WCHNET_JXJGameServerCreateRoom;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJGameServerCreateRoomPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJGameServerCreateRoomPacket &packet,bool sucess,int roomid,int sceneid,int battleid);
};
class crJXJJuntuanCreateRoomPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJJuntuanCreateRoomPacket> sptr;
	typedef GNE::WeakPtr<crJXJJuntuanCreateRoomPacket> wptr;
public :
	crJXJJuntuanCreateRoomPacket();
	crJXJJuntuanCreateRoomPacket( const crJXJJuntuanCreateRoomPacket &p );

	virtual ~crJXJJuntuanCreateRoomPacket() {}
public:
	static const int ID = WCHNET_JXJJuntuanCreateRoom;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJJuntuanCreateRoomPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJJuntuanCreateRoomPacket &packet,bool sucess,int roomid,int sceneid,int battleid,int groupid,int playerid);
};
class crJXJBattleRemovedPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleRemovedPacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleRemovedPacket> wptr;
public :
	crJXJBattleRemovedPacket();
	crJXJBattleRemovedPacket( const crJXJBattleRemovedPacket &p );

	virtual ~crJXJBattleRemovedPacket() {}
public:
	static const int ID = WCHNET_JXJBattleRemoved;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleRemovedPacket &packet,int battleid,int roomid);
};
class crJXJBattleSettlePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleSettlePacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleSettlePacket> wptr;
public :
	crJXJBattleSettlePacket();
	crJXJBattleSettlePacket( const crJXJBattleSettlePacket &p );

	virtual ~crJXJBattleSettlePacket() {}
public:
	static const int ID = WCHNET_JXJBattleSettle;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleSettlePacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJBattleSettlePacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJCountryCapitalAwardPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJCountryCapitalAwardPacket> sptr;
	typedef GNE::WeakPtr<crJXJCountryCapitalAwardPacket> wptr;
public :
	crJXJCountryCapitalAwardPacket();
	crJXJCountryCapitalAwardPacket( const crJXJCountryCapitalAwardPacket &p );

	virtual ~crJXJCountryCapitalAwardPacket() {}
public:
	static const int ID = WCHNET_JXJCountryCapital;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJCountryCapitalAwardPacket &packet,_crInt64 msg,CRCore::crStreamBuf *stream);
};
class crJXJBattleReportPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleReportPacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleReportPacket> wptr;
public:
	crJXJBattleReportPacket();
	crJXJBattleReportPacket(const crJXJBattleReportPacket &p);

	virtual ~crJXJBattleReportPacket() {}
public:
	static const int ID = WCHNET_JXJBattleReportPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleReportPacket &packet, CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJBattleReportPacket &packet, CRCore::crStreamBuf *stream);
};

class crJXJFubenSettlePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJFubenSettlePacket> sptr;
	typedef GNE::WeakPtr<crJXJFubenSettlePacket> wptr;
public :
	crJXJFubenSettlePacket();
	crJXJFubenSettlePacket( const crJXJFubenSettlePacket &p );

	virtual ~crJXJFubenSettlePacket() {}
public:
	static const int ID = WCHNET_JXJFubenSettle;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJFubenSettlePacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJFubenSettlePacket &packet,CRCore::crStreamBuf *stream);
};

//class crJXJPassSettlePacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crJXJPassSettlePacket> sptr;
//	typedef GNE::WeakPtr<crJXJPassSettlePacket> wptr;
//public :
//	crJXJPassSettlePacket();
//	crJXJPassSettlePacket( const crJXJPassSettlePacket &p );
//
//	virtual ~crJXJPassSettlePacket() {}
//public:
//	static const int ID = WCHNET_JXJPassSettle;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crJXJPassSettlePacket &packet,CRCore::crStreamBuf *stream);
//	static void buildReplyPacket2(crJXJPassSettlePacket &packet,CRCore::crStreamBuf *stream);
//};
////////////////////////////////////////////
class crJXJBattleTotalArmyCountPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleTotalArmyCountPacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleTotalArmyCountPacket> wptr;
public :
	crJXJBattleTotalArmyCountPacket();
	crJXJBattleTotalArmyCountPacket( const crJXJBattleTotalArmyCountPacket &p );

	virtual ~crJXJBattleTotalArmyCountPacket() {}
public:
	static const int ID = WCHNET_JXJBattleArmyCountPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleTotalArmyCountPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJBattleTotalArmyCountPacket &packet,CRCore::crStreamBuf *stream);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleAddArmyInfoPacket//兵力不足，补充兵力包
//
////////////////////////////////////////////
class crJXJBattleAddArmyInfoPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleAddArmyInfoPacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleAddArmyInfoPacket> wptr;
public :
	crJXJBattleAddArmyInfoPacket();
	crJXJBattleAddArmyInfoPacket( const crJXJBattleAddArmyInfoPacket &p );

	virtual ~crJXJBattleAddArmyInfoPacket() {}
public:
	static const int ID = WCHNET_JXJBattleAddArmyInfoPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJBattleAddArmyInfoPacket &packet,int playerid);
	static void buildReplyPacket2(crJXJBattleAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJCampAddArmyInfoPacket//据点兵，补充兵力包
//
////////////////////////////////////////////
class crJXJCampAddArmyInfoPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJCampAddArmyInfoPacket> sptr;
	typedef GNE::WeakPtr<crJXJCampAddArmyInfoPacket> wptr;
public :
	crJXJCampAddArmyInfoPacket();
	crJXJCampAddArmyInfoPacket( const crJXJCampAddArmyInfoPacket &p );

	virtual ~crJXJCampAddArmyInfoPacket() {}
public:
	static const int ID = WCHNET_JXJCampAddArmyInfoPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJCampAddArmyInfoPacket &packet);
	static void buildRequestPacket2(crJXJCampAddArmyInfoPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJCampAddArmyInfoPacket &packet,int playerid,char code);
	static void buildReplyPacket2(crJXJCampAddArmyInfoPacket &packet,CRCore::crStreamBuf *stream);
};
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class crJXJServerMetierEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJServerMetierEventPacket> sptr;
	typedef GNE::WeakPtr<crJXJServerMetierEventPacket> wptr;
public :
	crJXJServerMetierEventPacket();
	crJXJServerMetierEventPacket( const crJXJServerMetierEventPacket &p );

	virtual ~crJXJServerMetierEventPacket() {}
public:
	static const int ID = WCHNET_JXJServerMetierEvent;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJServerMetierEventPacket &packet,_crInt64 msg,CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crJXJServerMetierEventPacket &packet,int playerid,CRCore::crStreamBuf *stream);
};

class crJXJPlayerMetierEventPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJPlayerMetierEventPacket> sptr;
	typedef GNE::WeakPtr<crJXJPlayerMetierEventPacket> wptr;
public :
	crJXJPlayerMetierEventPacket();
	crJXJPlayerMetierEventPacket( const crJXJPlayerMetierEventPacket &p );

	virtual ~crJXJPlayerMetierEventPacket() {}
public:
	static const int ID = WCHNET_JXJPlayerMetierEvent;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJPlayerMetierEventPacket &packet,_crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket(crJXJPlayerMetierEventPacket &packet, int recvid, int playerid, _crInt64 msg, CRCore::crStreamBuf *param);
	static void buildRequestPacket2(crJXJPlayerMetierEventPacket &packet, int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJPlayerMetierEventPacket &packet,int playerid,_crInt64 msg,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJPlayerMetierEventPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJDropedItemPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJDropedItemPacket> sptr;
	typedef GNE::WeakPtr<crJXJDropedItemPacket> wptr;
public :
	crJXJDropedItemPacket();
	crJXJDropedItemPacket( const crJXJDropedItemPacket &p );

	virtual ~crJXJDropedItemPacket() {}
public:
	static const int ID = WCHNET_JXJDropedItem;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJDropedItemPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJDropedItemPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJGameChatPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJGameChatPacket> sptr;
	typedef GNE::WeakPtr<crJXJGameChatPacket> wptr;
public :
	crJXJGameChatPacket();
	crJXJGameChatPacket( const crJXJGameChatPacket &p );

	virtual ~crJXJGameChatPacket() {}
public:
	static const int ID = WCHNET_JXJGameChatPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJGameChatPacket &packet,char chatmode,const std::string &chattext);
	static void buildRequestPacket(crJXJGameChatPacket &packet,char chatmode,const std::string &recvname,const std::string &chattext);
	static void buildRequestPacket2(crJXJGameChatPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJGameChatPacket &packet,int playerid,char chatmode,const std::string &sendername,const std::string &chattext);
	static void buildReplyPacket(crJXJGameChatPacket &packet,int playerid,char chatmode,unsigned char sendershili,const std::string &sendername,const std::string &chattext);
	static void buildReplyPacket2(crJXJGameChatPacket &packet,CRCore::crStreamBuf *stream);

	//static CREncapsulation::crTableIO * getNoChatTabPtr();
	//static void reloadNoChatTab();
protected:
	//static CRCore::ref_ptr<CREncapsulation::crTableIO> s_noChatTab ;
	//static CRCore::ref_ptr<CREncapsulation::crTableIO> s_ADTab; 
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleInspirePacket//兵力不足，补充兵力包
//
////////////////////////////////////////////
class crJXJBattleInspirePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleInspirePacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleInspirePacket> wptr;
public :
	crJXJBattleInspirePacket();
	crJXJBattleInspirePacket( const crJXJBattleInspirePacket &p );

	virtual ~crJXJBattleInspirePacket() {}
public:
	static const int ID = WCHNET_JXJBattleInspirePacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleInspirePacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJBattleInspirePacket &packet,int playerid);
	static void buildReplyPacket(crJXJBattleInspirePacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJSaveLogPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJSaveLogPacket> sptr;
	typedef GNE::WeakPtr<crJXJSaveLogPacket> wptr;
public :
	crJXJSaveLogPacket();
	crJXJSaveLogPacket( const crJXJSaveLogPacket &p );

	virtual ~crJXJSaveLogPacket() {}
public:
	static const int ID = WCHNET_JXJSaveLog;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJSaveLogPacket &packet,int playerid,unsigned short logtype,const std::string &logdata);
};
class crJXJSendPlayerDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJSendPlayerDataPacket> sptr;
	typedef GNE::WeakPtr<crJXJSendPlayerDataPacket> wptr;
public :
	crJXJSendPlayerDataPacket();
	crJXJSendPlayerDataPacket( const crJXJSendPlayerDataPacket &p );

	virtual ~crJXJSendPlayerDataPacket() {}
public:
	static const int ID = WCHNET_JXJSendPlayerData;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJSendPlayerDataPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJSendPlayerDataPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJEndSendPlayerDataPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJEndSendPlayerDataPacket> sptr;
	typedef GNE::WeakPtr<crJXJEndSendPlayerDataPacket> wptr;
public :
	crJXJEndSendPlayerDataPacket();
	crJXJEndSendPlayerDataPacket( const crJXJEndSendPlayerDataPacket &p );

	virtual ~crJXJEndSendPlayerDataPacket() {}
public:
	static const int ID = WCHNET_JXJEndSendPlayerData;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJEndSendPlayerDataPacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJEndSendPlayerDataPacket &packet,CRCore::crStreamBuf *stream);
};
//随机选择阵营的时候，服务器告诉客户端homeid和m_chengchiid的消息
class crJXJRandomZhenYingTellClientIdPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJRandomZhenYingTellClientIdPacket> sptr;
	typedef GNE::WeakPtr<crJXJRandomZhenYingTellClientIdPacket> wptr;
public :
	crJXJRandomZhenYingTellClientIdPacket();
	crJXJRandomZhenYingTellClientIdPacket( const crJXJRandomZhenYingTellClientIdPacket &p );

	virtual ~crJXJRandomZhenYingTellClientIdPacket() {}
public:
	static const int ID = WCHNET_JXJRandomZhenYingTellClientId;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJRandomZhenYingTellClientIdPacket &packet, int playerid, unsigned char shiliId, unsigned short chengchiId, unsigned short homeId, int giftGold);
	static void buildReplyPacket2(crJXJRandomZhenYingTellClientIdPacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJPingPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJPingPacket > sptr;
	typedef GNE::WeakPtr<crJXJPingPacket > wptr;
public :
	crJXJPingPacket ();
	crJXJPingPacket ( const crJXJPingPacket &p );

	virtual ~crJXJPingPacket () {}
public:
	static const int ID = WCHNET_JXJPingPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJPingPacket &packet);
	static void buildReplyPacket(crJXJPingPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJPingPacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJQueryLoseCountryPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJQueryLoseCountryPacket> sptr;
	typedef GNE::WeakPtr<crJXJQueryLoseCountryPacket> wptr;
public :
	crJXJQueryLoseCountryPacket();
	crJXJQueryLoseCountryPacket( const crJXJQueryLoseCountryPacket &p );

	virtual ~crJXJQueryLoseCountryPacket() {}
public:
	static const int ID = WCHNET_JXJQueryLoseCountry;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJQueryLoseCountryPacket &packet);
	static void buildRequestPacket2(crJXJQueryLoseCountryPacket &packet,int playerid);
	static void buildReplyPacket(crJXJQueryLoseCountryPacket &packet,int playerid,CRCore::crVector2ub& loseshiliid);
	static void buildReplyPacket2(crJXJQueryLoseCountryPacket &packet,CRCore::crStreamBuf *stream);
private:
	static void record(const std::string &servername);
};
///////crJXJLoginPacket
class crJXJLoginPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJLoginPacket> sptr;
	typedef GNE::WeakPtr<crJXJLoginPacket> wptr;
public :
	crJXJLoginPacket();
	crJXJLoginPacket( const crJXJLoginPacket &p );

	virtual ~crJXJLoginPacket() {}
public:
	static const int ID = WCHNET_JXJLOGIN;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJLoginPacket &packet,std::string &userName,std::string &userPassword);
	static void buildRequestPacket2(crJXJLoginPacket &packet,const std::string &address,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJLoginPacket &packet,const std::string &address,char loginCode,int playerid,int lastserverid,const std::string &key);
	static void buildReplyPacket2(crJXJLoginPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJRegisterAccountPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJRegisterAccountPacket> sptr;
	typedef GNE::WeakPtr<crJXJRegisterAccountPacket> wptr;
public :
	crJXJRegisterAccountPacket();
	crJXJRegisterAccountPacket( const crJXJRegisterAccountPacket &p );

	virtual ~crJXJRegisterAccountPacket() {}
public:
	static const int ID = WCHNET_JXJREGACCOUNT;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJRegisterAccountPacket &packet, std::string &name, std::string &password, std::string &email, std::string &nickname, std::string &relname, char sex,std::string &code);
	static void buildRequestPacket2(crJXJRegisterAccountPacket &packet,const std::string &address, CRCore::crStreamBuf *buf);
	static void buildReplyPacket(crJXJRegisterAccountPacket &packet,const std::string &address,char ret,const std::string &validdate);
	static void buildReplyPacket2(crJXJRegisterAccountPacket &packet,CRCore::crStreamBuf* buf);
};
class crJXJLoginGamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJLoginGamePacket> sptr;
	typedef GNE::WeakPtr<crJXJLoginGamePacket> wptr;
public :
	crJXJLoginGamePacket();
	crJXJLoginGamePacket( const crJXJLoginGamePacket &p );

	virtual ~crJXJLoginGamePacket() {}
public:
	static const int ID = WCHNET_JXJLOGINGAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJLoginGamePacket &packet);
	static void buildRequestPacket2(crJXJLoginGamePacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJLoginGamePacket &packet,const std::string &address,char loginCode,int webserverid,const std::string &site,const std::string &serverName,CRNetApp::crGameServerPlayerData *playerData);
	static void buildReplyPacket2(crJXJLoginGamePacket &packet,char loginCode,CRCore::crStreamBuf *stream);
	void moneyAndVipQuery(int playerid, int serverid,CRCore::crData *data/*,CRDataBase::crDataBase* globalSession*/);
	void qqYellowQuery(int playerid, CRCore::crData* data/*,CRDataBase::crDataBase* accountSession*/);
};
class crJXJConnectServerQueryPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJConnectServerQueryPacket> sptr;
	typedef GNE::WeakPtr<crJXJConnectServerQueryPacket> wptr;
public :
	crJXJConnectServerQueryPacket();
	crJXJConnectServerQueryPacket( const crJXJConnectServerQueryPacket &p );

	virtual ~crJXJConnectServerQueryPacket() {}
public:
	static const int ID = WCHNET_JXJCONNECTSERVERQUERY;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJConnectServerQueryPacket &packet,int gameid);
	static void buildRequestPacket2(crJXJConnectServerQueryPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJConnectServerQueryPacket &packet,const std::string &sender,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJConnectServerQueryPacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJMonitorPingPacket: public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJMonitorPingPacket> sptr;
	typedef GNE::WeakPtr<crJXJMonitorPingPacket> wptr;
public :
	crJXJMonitorPingPacket();
	crJXJMonitorPingPacket( const crJXJMonitorPingPacket&p );

	virtual ~crJXJMonitorPingPacket() {}
public:
	static const int ID = WCHNET_JXJMonitorPingPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJMonitorPingPacket &packet,int gameid);
	static void buildRequestPacket2(crJXJMonitorPingPacket &packet,const std::string &sender);
	static void buildReplyPacket(crJXJMonitorPingPacket &packet,const std::string &sender,const std::string& servername);
	static void buildReplyPacket2(crJXJMonitorPingPacket &packet,CRCore::crStreamBuf *stream);
};

class crJXJRestartServerPacket: public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJRestartServerPacket> sptr;
	typedef GNE::WeakPtr<crJXJRestartServerPacket> wptr;
public :
	crJXJRestartServerPacket();
	crJXJRestartServerPacket( const crJXJRestartServerPacket&p );

	virtual ~crJXJRestartServerPacket() {}
public:
	static const int ID = WCHNET_JXJRestartServer			;
public:
	virtual void parsePacket(const std::string &sender);
};



class crJXJQueryAbstractItemPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJQueryAbstractItemPacket> sptr;
	typedef GNE::WeakPtr<crJXJQueryAbstractItemPacket> wptr;
public :
	crJXJQueryAbstractItemPacket();
	crJXJQueryAbstractItemPacket( const crJXJQueryAbstractItemPacket &p );

	virtual ~crJXJQueryAbstractItemPacket() {}
public:
	static const int ID = WCHNET_JXJQueryAbstractItem;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJQueryAbstractItemPacket &packet,int abstractid,CRNetApp::crInstanceItem *item,unsigned char camp,/*unsigned short*/int itemcount,float interval,short bufid=0,unsigned char npctype=NPC_Common);
	static void buildReplyPacket(crJXJQueryAbstractItemPacket &packet,bool sucess,int streamid,int count);
};
////////////////////////////////////////////////////////////////////////////
////
////crJXJChangeBattleStatePacket//根据攻防出生点数量更改战场状态
////
//////////////////////////////////////////////
//class crJXJChangeBattleStatePacket : public CRNet::crStreamPacket
//{
//public:
//	typedef GNE::SmartPtr<crJXJChangeBattleStatePacket> sptr;
//	typedef GNE::WeakPtr<crJXJChangeBattleStatePacket> wptr;
//public :
//	crJXJChangeBattleStatePacket();
//	crJXJChangeBattleStatePacket( const crJXJChangeBattleStatePacket &p );
//
//	virtual ~crJXJChangeBattleStatePacket() {}
//public:
//	static const int ID = WCHNET_JXJBattleChangeStatePacket;
//public:
//	virtual void parsePacket(const std::string &sender);
//	static void buildRequestPacket(crJXJChangeBattleStatePacket &packet,int battleid,int roomid,bool bClose);
//	static void buildReplyPacket(crJXJChangeBattleStatePacket &packet,CRCore::crStreamBuf *stream);
//};
//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryGmMapPacket 
//
////////////////////////////////////////////
class crJXJQueryGmMapPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJQueryGmMapPacket> sptr;
	typedef GNE::WeakPtr<crJXJQueryGmMapPacket> wptr;
public :
	crJXJQueryGmMapPacket();
	crJXJQueryGmMapPacket( const crJXJQueryGmMapPacket &p );

	virtual ~crJXJQueryGmMapPacket() {}
public:
	static const int ID = WCHNET_JXJQueryGmMapPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJQueryGmMapPacket &packet,CRCore::crStreamBuf *stream);
};
//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryNoChatSetPacket 
//
////////////////////////////////////////////
class crJXJQueryNoChatSetPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJQueryNoChatSetPacket> sptr;
	typedef GNE::WeakPtr<crJXJQueryNoChatSetPacket> wptr;
public :
	crJXJQueryNoChatSetPacket();
	crJXJQueryNoChatSetPacket( const crJXJQueryNoChatSetPacket &p );

	virtual ~crJXJQueryNoChatSetPacket() {}
public:
	static const int ID = WCHNET_JXJQueryNoChatSetPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildReplyPacket(crJXJQueryNoChatSetPacket &packet,CRCore::crStreamBuf *stream);
};
class crJXJCheckPlayerGameNamePacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJCheckPlayerGameNamePacket> sptr;
	typedef GNE::WeakPtr<crJXJCheckPlayerGameNamePacket> wptr;
public :
	crJXJCheckPlayerGameNamePacket();
	crJXJCheckPlayerGameNamePacket( const crJXJCheckPlayerGameNamePacket &p );

	virtual ~crJXJCheckPlayerGameNamePacket() {}
public:
	static const int ID = WCHNET_JXJCHECKPLAYERGAMENAME;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJCheckPlayerGameNamePacket &packet,std::string &name);
	static void buildRequestPacket2(crJXJCheckPlayerGameNamePacket &packet,int playerid,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJCheckPlayerGameNamePacket &packet,int recvid,char code);
	static void buildReplyPacket2(crJXJCheckPlayerGameNamePacket &packet,CRCore::crStreamBuf *stream);
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleRBuffPacket//犒赏失败包
//
////////////////////////////////////////////
class crJXJBattleRBuffPacket : public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJBattleRBuffPacket> sptr;
	typedef GNE::WeakPtr<crJXJBattleRBuffPacket> wptr;
public :
	crJXJBattleRBuffPacket();
	crJXJBattleRBuffPacket( const crJXJBattleRBuffPacket &p );

	virtual ~crJXJBattleRBuffPacket() {}
public:
	static const int ID = WCHNET_JXJBattleRBuffPacket;
public:
	virtual void parsePacket(const std::string &sender);
	static void buildRequestPacket(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream);
	static void buildReplyPacket2(crJXJBattleRBuffPacket &packet,CRCore::crStreamBuf *stream);
};

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleRBuffPacket// senceserver reloadtable
//
////////////////////////////////////////////
class crJXJSceneServerReloadTabPacket: public CRNet::crStreamPacket
{
public:
	typedef GNE::SmartPtr<crJXJSceneServerReloadTabPacket> sptr;
	typedef GNE::WeakPtr<crJXJSceneServerReloadTabPacket> wptr;
public :
	crJXJSceneServerReloadTabPacket();
	crJXJSceneServerReloadTabPacket( const crJXJSceneServerReloadTabPacket&p );

	virtual ~crJXJSceneServerReloadTabPacket() {}
public:
	static const int ID = WCHNET_JXJSceneServerReloadTab;
public:
	virtual void parsePacket(const std::string &sender);
	static void crJXJSceneServerReloadTabPacket::buildRequestPacket(crJXJSceneServerReloadTabPacket &packet, int tableid);
};
}
#endif