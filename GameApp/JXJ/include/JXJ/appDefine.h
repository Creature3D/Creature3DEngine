/**********************************************************************
*
*	文件:	   appDefine.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPDEFINE_H
#define JXJ_APPDEFINE_H 1

#include <CRNetApp/appNetDefine.h>

#define WCHNET_JXJID						WCHNETID+2000
#define WCHNET_JXJBattleOpen				WCHNET_JXJID+1
#define WCHNET_JXJQueryBattleTimer			WCHNET_JXJID+2
#define WCHNET_JXJPlayerLoginGame			WCHNET_JXJID+3
#define WCHNET_JXJSearchBattle				WCHNET_JXJID+4
#define WCHNET_JXJRandomPlayerName			WCHNET_JXJID+5	
#define WCHNET_JXJGameServerCreateRoom		WCHNET_JXJID+6
#define WCHNET_JXJBattleRemoved				WCHNET_JXJID+7
#define WCHNET_JXJBattleSettle				WCHNET_JXJID+8
#define WCHNET_JXJFubenSettle				WCHNET_JXJID+9
#define WCHNET_JXJServerMetierEvent			WCHNET_JXJID+10
#define WCHNET_JXJPlayerMetierEvent			WCHNET_JXJID+11
#define WCHNET_JXJPassSettle				WCHNET_JXJID+12
#define WCHNET_JXJDropedItem				WCHNET_JXJID+13
#define WCHNET_JXJSaveLog					WCHNET_JXJID+14
#define WCHNET_JXJLOGIN						WCHNET_JXJID+15
#define WCHNET_JXJREGACCOUNT				WCHNET_JXJID+16
#define WCHNET_JXJLOGINGAME					WCHNET_JXJID+17
#define	WCHNET_JXJCONNECTSERVERQUERY		WCHNET_JXJID+18
#define	WCHNET_JXJQueryAbstractItem			WCHNET_JXJID+19
#define	WCHNET_JXJQueryGmMapPacket			WCHNET_JXJID+20
#define	WCHNET_JXJQueryNoChatSetPacket		WCHNET_JXJID+21
#define	WCHNET_JXJCHECKPLAYERGAMENAME		WCHNET_JXJID+22
#define	WCHNET_JXJSearchSceneRoom			WCHNET_JXJID+23
/////////////////////////////////////////////
#define WCHNET_JXJGameChatPacket			WCHNET_JXJID+50
#define WCHNET_JXJBattleArmyCountPacket		WCHNET_JXJID+51
#define WCHNET_JXJBattleAddArmyInfoPacket	WCHNET_JXJID+52
#define WCHNET_JXJBattleInspirePacket		WCHNET_JXJID+53
#define WCHNET_JXJSendPlayerData			WCHNET_JXJID+54
#define WCHNET_JXJEndSendPlayerData			WCHNET_JXJID+55
#define WCHNET_JXJCampAddArmyInfoPacket		WCHNET_JXJID+56
#define WCHNET_JXJRandomZhenYingTellClientId		WCHNET_JXJID+57
#define WCHNET_JXJQueryLoseCountry			WCHNET_JXJID+58
#define WCHNET_JXJPingPacket				WCHNET_JXJID+59
#define WCHNET_JXJMonitorPingPacket			WCHNET_JXJID+60
#define WCHNET_JXJRestartServer				WCHNET_JXJID+61
#define WCHNET_JXJBattleChangeStatePacket	WCHNET_JXJID+62
#define WCHNET_JXJBattleReportPacket		WCHNET_JXJID+63
#define WCHNET_JXJBattleRBuffPacket			WCHNET_JXJID+64
#define WCHNET_JXJJuntuanCreateRoom			WCHNET_JXJID+70
#define WCHNET_JXJCountryCapital		    WCHNET_JXJID+71
#define WCHNET_JXJSceneServerReloadTab		WCHNET_JXJID+72
//		范围锁使用 CRCore::ScopedLock<crData> lock(*shilidata);
namespace JXJ{
enum JXJAppNetType
{
	JXJMonitor1 = CRNetApp::Net_User+1,
	JXJMonitor2,
	JXJMonitor3,
	JXJMonitor4,
	JXJMonitor5,
	JXJMonitor6,
	JXJMonitor7,
	JXJMonitor8,
	JXJMonitor9,
	JXJMonitor10,
};
}
#endif