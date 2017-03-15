/**********************************************************************
*
*	文件:	   appData.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#include <JXJ/appData.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appPackets.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
#include <crnetapp/appNetDefine.h>
#include <rcfg/ConfigScriptOO.h>
#include <CRIOManager/crFileUtils.h>
#include <CREncapsulation/crLoader.h>
#include <windows.h>
#include <JXJ/appMsg.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace CRIOManager;
using namespace JXJ;
/////////////////////////////////////////
//
//crJXJChengfangInfo
//
/////////////////////////////////////////
unsigned char crJXJChengfangInfo::getChengzhuShili()
{
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	if(m_qun>0)
		return 2;
	else if(!m_shilichengfangmap.empty())
	{
		return m_shilichengfangmap.begin()->second;
	}
	return 2;//不应该执行到这里
}
unsigned short crJXJChengfangInfo::getChengzhuChengfang()
{
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	if(m_qun>0)
		return m_qun;
	else if(!m_shilichengfangmap.empty())
	{
		return m_shilichengfangmap.begin()->first;
	}
	return m_qun;//不应该执行到这里
}
unsigned char crJXJChengfangInfo::changeShiliChengfang(unsigned char winshili,unsigned char lostshili)
{
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	unsigned char chengzhu = getChengzhuShili();
	if(winshili == chengzhu || winshili<c_startShiliID || winshili == lostshili)
		return chengzhu;
	unsigned short chengfang;
	if(lostshili<c_startShiliID)
		m_qun -= 100;
	ShiliChengfangMap newmap;
	bool found = false;
	for( ShiliChengfangMap::iterator itr = m_shilichengfangmap.begin();
		itr != m_shilichengfangmap.end();
		++itr )
	{
		chengfang = itr->first;
		if(lostshili == itr->second)
		{
			chengfang -= 100;
		}
		else if(winshili == itr->second)
		{
			 chengfang += 100;
			 found = true;
		}
		newmap.insert(std::make_pair(chengfang,itr->second));
	}
	if(!found)
	{
		newmap.insert(std::make_pair(100,winshili));
	}
	m_shilichengfangmap.swap(newmap);
	return getChengzhuShili();
}
void crJXJChengfangInfo::reset(unsigned char shili, unsigned short chengfang)
{
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	m_shilichengfangmap.clear();
	if(shili<c_startShiliID)
		m_qun = chengfang;
	else
	{
		m_qun = 0;
		m_shilichengfangmap.insert(std::make_pair(chengfang,shili));
	}
}
void crJXJChengfangInfo::buildStream(CRCore::ref_ptr<CRCore::crStreamBuf> &stream)
{//12
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	stream->_writeUShort(m_qun);//2
	stream->_writeUChar(m_shilichengfangmap.size());//1
	for( ShiliChengfangMap::iterator itr = m_shilichengfangmap.begin();
		 itr != m_shilichengfangmap.end();
		 ++itr )
	{
		stream->_writeUShort(itr->first);
		stream->_writeUChar(itr->second);
	}
}
void crJXJChengfangInfo::setStream(CRCore::crStreamBuf *stream)
{
	GNE::LockMutex lock( m_shilichengfangmapMutex );
	if(stream)
	{
		m_qun = stream->_readUShort();
		unsigned char size = stream->_readUChar();
		m_shilichengfangmap.clear();
		unsigned short chengfang;
		unsigned char shili;
		for( int i = 0; i<size; i++ )
		{
			chengfang = stream->_readUShort();
			shili = stream->_readUChar();
			m_shilichengfangmap.insert(std::make_pair(chengfang,shili));
		}
	}
}
/////////////////////////////////////////
//
//crJXJPlayerDataMonitor
//
/////////////////////////////////////////
crJXJPlayerDataMonitor::crJXJPlayerDataMonitor()
{
}
void crJXJPlayerDataMonitor::setGoldingot(int goldingot)
{
	m_goldingot = goldingot;
}
int crJXJPlayerDataMonitor::getGoldingot()
{
	return m_goldingot;
}
void crJXJPlayerDataMonitor::setGiftgold(int giftgold)
{
	m_giftgold = giftgold;
}
int crJXJPlayerDataMonitor::getGiftgold()
{
	return m_giftgold;
}
void crJXJPlayerDataMonitor::setVipExp(int vipExp)
{
	m_vipExp = vipExp;
}
int crJXJPlayerDataMonitor::getVipExp()
{
	return m_vipExp;
}
void crJXJPlayerDataMonitor::setCoppercash(int coppercash)
{
	m_coppercash = coppercash;
}
int crJXJPlayerDataMonitor::getCoppercash()
{
	return m_coppercash;
}
void crJXJPlayerDataMonitor::setFood(int food)
{
	m_food = food;
}
int crJXJPlayerDataMonitor::getFood()
{
	return m_food;
}
void crJXJPlayerDataMonitor::setWood(int wood)
{
	m_wood = wood;
}
int crJXJPlayerDataMonitor::getWood()
{
	return m_wood;
}
void crJXJPlayerDataMonitor::setIron(int iron)
{
	m_iron = iron;
}
int crJXJPlayerDataMonitor::getIron()
{
	return m_iron;
}
void crJXJPlayerDataMonitor::setHorse(int horse)
{
	m_horse = horse;
}
int crJXJPlayerDataMonitor::getHorse()
{
	return m_horse;
}
void crJXJPlayerDataMonitor::setLevel(unsigned char lv)
{
	m_level = lv;
}
unsigned char crJXJPlayerDataMonitor::getLevel()
{
	return m_level;
}
void crJXJPlayerDataMonitor::setExploit(int exploit)
{
	m_exploit = exploit;
}
int crJXJPlayerDataMonitor::getExploit()
{
	return m_exploit;
}
void crJXJPlayerDataMonitor::setTroops(int troops)
{
	m_troops = troops;
}
int crJXJPlayerDataMonitor::getTroops()
{
	return m_troops;
}
void crJXJPlayerDataMonitor::setManorlv(unsigned short manorlv)
{
	m_manorlv = manorlv;
}
unsigned short crJXJPlayerDataMonitor::getManorlv()
{
	return m_manorlv;
}
/////////////////////////////////////////
//
//crJXJPlayerData
//
/////////////////////////////////////////
GNE::Mutex crJXJPlayerData::m_saveMutex;
crJXJPlayerData::crJXJPlayerData():
m_playerid(0),
m_playersex(0),
m_playericonid(0),
m_shiliid(0),
m_homeid(0),
m_chengchiid(0),
m_goldingot(0),
m_giftgold(0),
m_coppercash(0),
m_food(0),
m_wood(0),
m_iron(0),
m_horse(0),
m_fuyalv(1),
m_bingyinglv(1),
m_jiusilv(1),
m_gongfanglv(1),
m_shijilv(1),
m_fukulv(1),
m_mingjulv(0),
m_nongtianlv(0),
m_muchanglv(0),
m_lingdilv(0),
m_kuangshanlv(0),
m_updateTimer(0L),
m_junling(5500),
//m_maxtroops(255),
//m_jingbinglv(0),
//m_jianjialv(0),
//m_qiangrenlv(0),
//m_guimoulv(0),
//m_gongchenglv(0),
//m_yuangonglv(0),
//m_sugonglv(0),
//m_qiangxinglv(0),
//m_zhaomulv(0),
//m_rendelv(0),
//m_infantrylv(0),
//m_cavalrylv(0),
//m_archerlv(0),
//m_siegelv(0),
//m_shieldlv(0),
//m_guardlv(0),
//m_saddlelv(0),
//m_assaultlv(0),
//m_assistlv(0),
//m_penetratelv(0),
//m_manufacturelv(0),
//m_gunpowderlv(0),
//m_bravelv(0),
//m_ironcavalrylv(0),
//m_gearlv(0),
//m_fireboltlv(0),
//m_masterlv(0),
//m_juntunlv(0),
//m_reformlv(0),
//m_signin(0),
m_goldcosthint(0xFFFFFFFF),
//m_queryTeamIndex(0),
//m_halltimer(0),
//m_privatetimer(0),
m_levyCount(0),
//m_levyTimer(0),
m_onlineRewardCount(0),
m_onlineRewardTimer(0),
m_playerOnlineRewardTimer(0),
m_buyJunlingCount(0),
m_buyJunlingTimer(0),
m_openBattle(0),
m_battleTimer(0),
//m_openPass(0),
//m_passTimer(0),
m_inviteType(Invite_None),
m_inviteID(0),
m_inviteChengchiID(0),
m_fubenOrChengchiType(0),
m_fubenOrChengchiID(0),
m_teamLeaderID(0),
m_battleRoomID(0),
m_curFubenProgress(0),
m_openbagnum(0),
//m_luckycardcoppercost(0),
//m_luckycardgoldcost(0),
//m_knowluckycard(0),
//m_jiusirefreshtime(0),
m_viplv(0),
m_vipExp(0),
m_fubenReward(NULL),
m_reliveCount(0),
m_countrycontribute(0),
m_cardimpression(0),
m_cardpieces(0),
m_freeCounterinsurgency(0),
m_RefineEquipFreeCount(0),
m_rebelTimer(0),
m_NongTianShuiLilv(0),
m_ZhiShuZaoLinlv(0),
m_YouZhiCaoChanglv(0),
m_JingKuangShenWalv(0),
m_GuLiMaoYilv(0),
m_ZhengShouXinDelv(0),
m_YanBingYueWulv(0),
m_XunBingXinDelv(0),
m_ZongMiaoJiSilv(0),
m_JiSiXinDelv(0),
m_MarketBuyResourceMax(0),
m_refinemagicid(0),
m_cfgstream(NULL),
m_oldcfgstream(NULL),
m_FreeRollCardCount(0),
m_autosellcard(0),
m_CardImpression2CritTechLv(0),
m_CardImpression4CritTechLv(0),
m_CardImpression6CritTechLv(0),
m_CardImpression10CritTechLv(0),
m_FreeRollCardTechLv(0),
m_OrangeCardUpgradeTechLv(0),
m_CardShopOpenTechLv(0),
m_MakeEquipTechLv(0),
m_EquipRefineTechLv(0),
m_EquipAttachTechLv(0),
m_SmeltEquipTechLv(0),
m_SmeltMaterialTechLv(0),
m_SmeltStoneTechLv(0),
m_EquipRefineMaxCount1TechLv(0),
m_EquipRefineMaxCount2TechLv(0),
m_EquipRefineMaxCount3TechLv(0),
m_EquipRefineMaxCount4TechLv(0),
m_EquipRefineProduce1TechLv(0),
m_EquipRefineProduce2TechLv(0),
m_EquipRefineProduce3TechLv(0),
m_EquipRefineProduce4TechLv(0),
m_FreeRollCard2TechLv(0),
m_FreeRollCard3TechLv(0),
m_FreeRollCardProduce1TechLv(0),
m_FreeRollCardProduce2TechLv(0),
m_FreeRollCardProduce3TechLv(0),
m_FreeRollCardProduce4TechLv(0),
m_FuKuMaxTechLv(0),
m_bisCycling(false),
m_iClientCycleTime(0),
m_opensavedformationifo(1),
m_changeShiliTimes(0),
m_seriesLandDays(1),
m_LandRewardGetCount(1),
m_lanRewardData(NULL),
m_nLandRewardTime(0),
m_feteTimes(0),
m_enrichTimes(0),
m_enrichType(0),
m_nLunHuiRewardGiftGold(0),
m_consortiaid(0),
m_savedformationindex(0),
m_skipnewhand(false),
m_iftechbtnnew(0),
m_groupContributeiType(0),
m_groupContributeiTimes(0),
m_FuKuMaxTech2Lv(0),
m_FuKuMaxTech3Lv(0),
m_inspireLv(0),
//m_serverVersion (0),
m_flushTime(-1),
m_battleReportQueryedId(0),
m_lingqufengluflag(0),
//m_mainrolerefreshflag(0),
m_tuntianlv(0),
m_fanzhilv(0),
m_yelianlv(0),
m_fanmaolv(0),
m_qiaojianglv(0),
m_zhengshuilv(0),
m_techfenglulv(0),
m_techjiusibaojilv(0),
m_techworldtalklv(0),
m_techbuyjunlinglv(0),
m_foundCountryAward(0),
m_playertotalpaygold(0),
m_playerCycleCount(0),
m_shuichelv(1),
m_ronglulv(1),
m_gongshelv(1),
m_mochuanlv(1),
m_loginDays(0),
m_7DaysAward(0),
m_shouchong(0),
m_cZhengZhanQuan(0),
m_killplayercount(0),
m_killnpccount(0),
m_dayplayerdeadcount(0),
m_playerdeadcount(0),
m_daykillplayercount(0),
m_daykillnpccount(0),
m_usedachievepoint(0),
m_passmingjiangcount(0),
m_Cpassworldfuben(0),
m_cmingjiangfail(0),
m_ctianxiafail(0),
m_havegotviplv(0),
m_Cpasstianxia(0),
m_cfinishdays(0),
m_accomplishPointCache(0),
m_praisedcount(0),
m_praisecount(0),
m_cGuoZhanJiFen(0),
m_treasonbuffertimer(0),
m_treasonpunishtimer(0),
m_treasoncoolingtimer(0),
m_selectedshili(10),
m_timesshangwu(0),
m_timesxiawu(0),
m_timeswanshang(0),
m_praiselimit(0),
m_guanzhi(0),
m_yuekaid(0),
m_yuekaendtime(0),
m_lastgettime(0),
m_kaoshangfutime(0),
m_specialtyrefreshtimes(0),
m_lastchongzhitime(0),
m_lastzeroRefreshtime(0),
m_chongzhicountT(0),
m_gainedzhoukahuodongid(0),
m_zhoukaendtime(0),
m_zhoukagetstatus(0),
m_zhoukagetdayidx(-1),
m_battleValidateID(0),
m_elector(false),
m_vote(0),
m_beElected(false),
m_recommendSet(false),
m_electIcon(0),
m_equipBindInfo(0),
m_isYellow(0),
m_isYearYellow(0),
m_yellowLevel(0),
m_yellowAwardInfo(0),
m_cancelCantonTimer(0),
m_hadwonjuntuanfubenT(false),
m_huoyuedusign(true),
m_hadshowdismisstip(false),
m_cardImpressionLevel(1),
m_resourceCount(1),
m_freeBuy(1),
m_goldRecruitCount(0),
m_zhengZhaoTimes(0),
m_myself(false),
m_pageNum(0),
m_zhengZhaolingUpdateTimer(0),
m_GuoJiaReleaseTimer(0),
m_JunTuanReleaseTimer(0),
m_CheckYingzhengReward(false),
m_attackVictoryTimes(0),
m_defendVictoryTimes(0),
m_countryBattleTimes(0),
m_autoBattle(false),
m_curPageNum_Introduction(0),
m_hadGottenReward(false),
m_myRankVal(0),
m_startedDays(0),
m_gameIntroductionNoTips(false),
m_hadShowFestivalHuodongCanvas(false),
m_onlinetimer(0),
m_autoBuilding(false),
m_buildCountryTimes(-1)
{
	//memset(m_hallCard,0,sizeof(m_hallCard));
	//memset(m_privateCard,0,sizeof(m_privateCard));
	for (int i = 0; i < 3; i++)
	{
		m_luckycard[i] = 0;
	}
	m_itemusedtimeMap.clear();
	m_chatRecordMap.clear();
	m_chatitemlinkIdMap.clear();
	m_chatinviteIdMap.clear();
	m_savedformationinfovec.resize(1);
	m_chatChannelSetInfo = (_ULONGLONG)0x0004220140A05C00;
	m_battleActiveTimer = c_battleActiveInterval;
	m_groupCantonSpecialtyVec.resize(8);
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			m_7DayData[i][j] = 0;
		}
	}
}
crJXJPlayerData::crJXJPlayerData(const crJXJPlayerData& data):
crData(data),
m_playerid(data.m_playerid),
m_playersex(data.m_playersex),
m_playericonid(data.m_playericonid),
m_shiliid(data.m_shiliid),
m_homeid(data.m_homeid),
m_chengchiid(data.m_chengchiid),
m_goldingot(data.m_goldingot),
m_giftgold(data.m_giftgold),
m_coppercash(data.m_coppercash),
m_food(data.m_food),
m_wood(data.m_wood),
m_iron(data.m_iron),
m_horse(data.m_horse),
m_fuyalv(data.m_fuyalv),
m_bingyinglv(data.m_bingyinglv),
m_jiusilv(data.m_jiusilv),
m_gongfanglv(data.m_gongfanglv),
m_shijilv(data.m_shijilv),
m_fukulv(data.m_fukulv),
m_mingjulv(data.m_mingjulv),
m_nongtianlv(data.m_nongtianlv),
m_muchanglv(data.m_muchanglv),
m_lingdilv(data.m_lingdilv),
m_kuangshanlv(data.m_kuangshanlv),
m_updateTimer(data.m_updateTimer),
m_buildingTimes(data.m_buildingTimes),
m_recruitTimes(data.m_recruitTimes),
m_ckilleachtroops(data.m_ckilleachtroops),
m_cdeadeachtroops(data.m_cdeadeachtroops),
m_danchangkill(data.m_danchangkill),
m_killdoorboss(data.m_killdoorboss),
m_junling(data.m_junling),
//m_maxtroops(data.m_maxtroops),
m_troopsMap(data.m_troopsMap),
m_troopsEquipMap(data.m_troopsEquipMap),
m_itembagvec(data.m_itembagvec),
//m_jingbinglv(data.m_jingbinglv),
//m_jianjialv(data.m_jianjialv),
//m_qiangrenlv(data.m_qiangrenlv),
//m_guimoulv(data.m_guimoulv),
//m_gongchenglv(data.m_gongchenglv),
//m_yuangonglv(data.m_yuangonglv),
//m_sugonglv(data.m_sugonglv),
//m_qiangxinglv(data.m_qiangxinglv),
//m_zhaomulv(data.m_zhaomulv),
//m_rendelv(data.m_rendelv),
//m_infantrylv(data.m_infantrylv),
//m_cavalrylv(data.m_cavalrylv),
//m_archerlv(data.m_archerlv),
//m_siegelv(data.m_siegelv),
//m_shieldlv(data.m_shieldlv),
//m_guardlv(data.m_guardlv),
//m_saddlelv(data.m_saddlelv),
//m_assaultlv(data.m_assaultlv),
//m_assistlv(data.m_assistlv),
//m_penetratelv(data.m_penetratelv),
//m_manufacturelv(data.m_manufacturelv),
//m_gunpowderlv(data.m_gunpowderlv),
//m_bravelv(data.m_bravelv),
//m_ironcavalrylv(data.m_ironcavalrylv),
//m_gearlv(data.m_gearlv),
//m_fireboltlv(data.m_fireboltlv),
//m_masterlv(data.m_masterlv),
//m_juntunlv(data.m_juntunlv),
//m_reformlv(data.m_reformlv),
//m_signin(data.m_signin),
m_formationInfoVec(data.m_formationInfoVec),
m_goldcosthint(data.m_goldcosthint),
m_myTeam(data.m_myTeam),
m_friendList(data.m_friendList),
m_queryTeamVec(data.m_queryTeamVec),
//m_queryTeamIndex(data.m_queryTeamIndex),
//m_halltimer(data.m_halltimer),
//m_privatetimer(data.m_privatetimer),
m_levyCount(data.m_levyCount),
//m_levyTimer(data.m_levyTimer),
m_onlineRewardCount(data.m_onlineRewardCount),
m_onlineRewardTimer(data.m_onlineRewardTimer),
m_buyJunlingCount(data.m_buyJunlingCount),
m_buyJunlingTimer(data.m_buyJunlingTimer),
m_saleList(data.m_saleList),
m_openBattle(data.m_openBattle),
m_battleTimer(data.m_battleTimer),
//m_openPass(data.m_openPass),
//m_passTimer(data.m_passTimer),
m_inviteType(data.m_inviteType),
m_inviteID(data.m_inviteID),
m_inviteChengchiID(data.m_inviteChengchiID),
m_fubenOrChengchiType(data.m_fubenOrChengchiType),
m_fubenOrChengchiID(data.m_fubenOrChengchiID),
m_teamLeaderID(0),
m_battleRoomID(0),
m_curFubenProgress(0),
m_openbagnum(data.m_openbagnum),
m_jiangkabagvec(data.m_jiangkabagvec),
m_fubenProgressMap(data.m_fubenProgressMap),
m_itemObjectMap(data.m_itemObjectMap),
m_extraProduceMap(data.m_extraProduceMap),
m_appointjiangkabagvec(data.m_appointjiangkabagvec),
//m_luckycardcoppercost(data.m_luckycardcoppercost),
//m_luckycardgoldcost(data.m_luckycardgoldcost),
//m_knowluckycard(data.m_knowluckycard),
//m_jiusirefreshtime(data.m_jiusirefreshtime),
m_viplv(data.m_viplv),
m_vipExp(data.m_vipExp),
m_fubenReward(data.m_fubenReward),
m_reliveCount(data.m_reliveCount),
m_countrycontribute(data.m_countrycontribute),
m_playerOnlineRewardTimer(data.m_playerOnlineRewardTimer),
m_chatRecordMap(data.m_chatRecordMap),
m_chatitemlinkIdMap(data.m_chatitemlinkIdMap),
m_chatinviteIdMap(data.m_chatinviteIdMap),
m_cardimpression(data.m_cardimpression),
m_cardpieces(data.m_cardpieces),
m_freeCounterinsurgency(data.m_freeCounterinsurgency),
m_RefineEquipFreeCount(data.m_RefineEquipFreeCount),
m_rebelTimer(data.m_rebelTimer),
m_NongTianShuiLilv(data.m_NongTianShuiLilv),
m_ZhiShuZaoLinlv(data.m_ZhiShuZaoLinlv),
m_YouZhiCaoChanglv(data.m_YouZhiCaoChanglv),
m_JingKuangShenWalv(data.m_JingKuangShenWalv),
m_GuLiMaoYilv(data.m_GuLiMaoYilv),
m_ZhengShouXinDelv(data.m_ZhengShouXinDelv),
m_YanBingYueWulv(data.m_YanBingYueWulv),
m_XunBingXinDelv(data.m_XunBingXinDelv),
m_ZongMiaoJiSilv(data.m_ZongMiaoJiSilv),
m_JiSiXinDelv(data.m_JiSiXinDelv),
m_MarketBuyResourceMax(data.m_MarketBuyResourceMax),
m_refinemagicid(data.m_refinemagicid),
m_FreeRollCardCount(data.m_FreeRollCardCount),
m_autosellcard(data.m_autosellcard),
m_cfgstream(NULL),
m_oldcfgstream(NULL),
m_troopstechmap(data.m_troopstechmap),
m_cardExtraTechMap(data.m_cardExtraTechMap),
m_CardImpression2CritTechLv(data.m_CardImpression2CritTechLv),
m_CardImpression4CritTechLv(data.m_CardImpression4CritTechLv),
m_CardImpression6CritTechLv(data.m_CardImpression6CritTechLv),
m_CardImpression10CritTechLv(data.m_CardImpression10CritTechLv),
m_FreeRollCardTechLv(data.m_FreeRollCardTechLv),
m_OrangeCardUpgradeTechLv(data.m_OrangeCardUpgradeTechLv),
m_CardShopOpenTechLv(data.m_CardShopOpenTechLv),
m_MakeEquipTechLv(data.m_MakeEquipTechLv),
m_EquipRefineTechLv(data.m_EquipRefineTechLv),
m_EquipAttachTechLv(data.m_EquipAttachTechLv),
m_blacknameset(data.m_blacknameset),
m_SmeltEquipTechLv(data.m_SmeltEquipTechLv),
m_SmeltMaterialTechLv(data.m_SmeltMaterialTechLv),
m_SmeltStoneTechLv(data.m_SmeltStoneTechLv),
m_EquipRefineMaxCount1TechLv(data.m_EquipRefineMaxCount1TechLv),
m_EquipRefineMaxCount2TechLv(data.m_EquipRefineMaxCount2TechLv),
m_EquipRefineMaxCount3TechLv(data.m_EquipRefineMaxCount3TechLv),
m_EquipRefineMaxCount4TechLv(data.m_EquipRefineMaxCount4TechLv),
m_EquipRefineProduce1TechLv(data.m_EquipRefineProduce1TechLv),
m_EquipRefineProduce2TechLv(data.m_EquipRefineProduce2TechLv),
m_EquipRefineProduce3TechLv(data.m_EquipRefineProduce3TechLv),
m_EquipRefineProduce4TechLv(data.m_EquipRefineProduce4TechLv),
m_FreeRollCard2TechLv(data.m_FreeRollCard2TechLv),
m_FreeRollCard3TechLv(data.m_FreeRollCard3TechLv),
m_FreeRollCardProduce1TechLv(data.m_FreeRollCardProduce1TechLv),
m_FreeRollCardProduce2TechLv(data.m_FreeRollCardProduce2TechLv),
m_FreeRollCardProduce3TechLv(data.m_FreeRollCardProduce3TechLv),
m_FreeRollCardProduce4TechLv(data.m_FreeRollCardProduce4TechLv),
m_FuKuMaxTechLv(data.m_FuKuMaxTechLv),
m_techdevelopmap(data.m_techdevelopmap),
m_bisCycling(false),
m_iClientCycleTime(0),
//m_persuadeInfoDeq(data.m_persuadeInfoDeq),
m_opensavedformationifo(data.m_opensavedformationifo),
m_savedformationindex(data.m_savedformationindex),
m_savedformationinfovec(data.m_savedformationinfovec),
m_changeShiliTimes(0),
m_seriesLandDays(data.m_seriesLandDays),
m_LandRewardGetCount(data.m_LandRewardGetCount),
m_lanRewardData(data.m_lanRewardData),
m_nLandRewardTime(0),
m_feteTimes(data.m_feteTimes),
m_enrichTimes(data.m_enrichTimes),
m_enrichType(data.m_enrichType),
m_lunhuiRewardMap(data.m_lunhuiRewardMap),
m_consortiaid(data.m_consortiaid),
m_skipnewhand(data.m_skipnewhand),
m_iftechbtnnew(data.m_iftechbtnnew),
m_groupContributeiType(data.m_groupContributeiType),
m_groupContributeiTimes(data.m_groupContributeiTimes),
m_FuKuMaxTech2Lv(data.m_FuKuMaxTech2Lv),
m_FuKuMaxTech3Lv(data.m_FuKuMaxTech3Lv),
m_myShili(data.m_myShili),
//m_serverVersion (data.m_serverVersion),
m_inspireLv(data.m_inspireLv),
m_flushTime(-1),
m_battleReportQueryedId(0),
m_lingqufengluflag(0),
//m_mainrolerefreshflag(0),
m_tuntianlv(data.m_tuntianlv),
m_fanzhilv(data.m_fanzhilv),
m_yelianlv(data.m_yelianlv),
m_fanmaolv(data.m_fanmaolv),
m_qiaojianglv(data.m_qiaojianglv),
m_zhengshuilv(data.m_zhengshuilv),
m_techfenglulv(data.m_techfenglulv),
m_techjiusibaojilv(data.m_techjiusibaojilv),
m_techworldtalklv(data.m_techworldtalklv),
m_techbuyjunlinglv(data.m_techbuyjunlinglv),
m_foundCountryAward(data.m_foundCountryAward),
m_playertotalpaygold(data.m_playertotalpaygold),
m_playerStore(data.m_playerStore),
m_playerCycleCount(data.m_playerCycleCount),
m_shuichelv(data.m_shuichelv),
m_ronglulv(data.m_ronglulv),
m_gongshelv(data.m_gongshelv),
m_mochuanlv(data.m_mochuanlv),
m_loginDays(data.m_loginDays),
m_7DaysAward(data.m_7DaysAward),
m_shouchong(data.m_shouchong),
m_cZhengZhanQuan(data.m_cZhengZhanQuan),
m_killplayercount(data.m_killplayercount),
m_killnpccount(data.m_killnpccount),
m_dayplayerdeadcount(data.m_dayplayerdeadcount),
m_playerdeadcount(data.m_playerdeadcount),
m_daykillplayercount(data.m_daykillplayercount),
m_daykillnpccount(data.m_daykillnpccount),
m_dayckilleachtroops(data.m_dayckilleachtroops),
m_usedachievepoint(data.m_usedachievepoint),
m_passmingjiangcount(data.m_passmingjiangcount),
m_Cpassworldfuben(data.m_Cpassworldfuben),
m_cmingjiangfail(data.m_cmingjiangfail),
m_ctianxiafail(data.m_ctianxiafail),
m_havegotviplv(data.m_havegotviplv),
m_Cpasstianxia(data.m_Cpasstianxia),
m_cfinishdays(data.m_cfinishdays),
m_accomplishPointCache(data.m_accomplishPointCache),
m_praisedcount(data.m_praisedcount),
m_praisecount(data.m_praisecount),
m_cGuoZhanJiFen(data.m_cGuoZhanJiFen),
m_treasonbuffertimer(data.m_treasonbuffertimer),
m_treasonpunishtimer(data.m_treasonpunishtimer),
m_treasoncoolingtimer(data.m_treasoncoolingtimer),
m_selectedshili(data.m_selectedshili),
m_timesshangwu(data.m_timesshangwu),
m_timesxiawu(data.m_timesxiawu),
m_timeswanshang(data.m_timeswanshang),
m_praiselimit(data.m_praiselimit),
m_guanzhi(data.m_guanzhi),
m_yuekaid(data.m_yuekaid),
m_yuekaendtime(data.m_yuekaendtime),
m_lastgettime(data.m_lastgettime),
m_kaoshangfutime(data.m_kaoshangfutime),
m_specialtyrefreshtimes(data.m_specialtyrefreshtimes),
m_lastchongzhitime(data.m_lastchongzhitime),
m_lastzeroRefreshtime(data.m_lastzeroRefreshtime),
m_chongzhicountT(data.m_chongzhicountT),
m_gainedzhoukahuodongid(data.m_gainedzhoukahuodongid),
m_zhoukaendtime(data.m_zhoukaendtime),
m_zhoukagetstatus(data.m_zhoukagetstatus),
m_zhoukagetdayidx(data.m_zhoukagetdayidx),
m_itemusedtimeMap(data.m_itemusedtimeMap),
m_battleValidateID(0),
m_elector(data.m_elector),
m_vote(data.m_vote),
m_beElected(data.m_beElected),
m_recommendSet(data.m_recommendSet),
m_electIcon(data.m_electIcon),
m_equipBindInfo(data.m_equipBindInfo),
m_isYellow(data.m_isYellow),
m_isYearYellow(data.m_isYearYellow),
m_yellowLevel(data.m_yellowLevel),
m_yellowAwardInfo(data.m_yellowAwardInfo),
m_groupCantonSpecialtyVec(data.m_groupCantonSpecialtyVec),
m_cancelCantonTimer(data.m_cancelCantonTimer),
m_hadwonjuntuanfubenT(data.m_hadwonjuntuanfubenT),
m_huoyuedusign(data.m_huoyuedusign),
m_hadshowdismisstip(data.m_hadshowdismisstip),
m_cardImpressionLevel(data.m_cardImpressionLevel),
m_buyshagnxianmap(data.m_buyshagnxianmap),
m_troopsachievemap(data.m_troopsachievemap),
m_resourceCount(data.m_resourceCount),
m_freeBuy(data.m_freeBuy),
m_goldRecruitCount(data.m_goldRecruitCount),
m_zhengZhaoTimes(data.m_zhengZhaoTimes),
m_myself(data.m_myself),
m_pageNum(data.m_pageNum),
m_zhengZhaolingUpdateTimer(data.m_zhengZhaolingUpdateTimer),
m_GuoJiaReleaseTimer(data.m_GuoJiaReleaseTimer),
m_JunTuanReleaseTimer(data.m_JunTuanReleaseTimer),
m_CheckYingzhengReward(data.m_CheckYingzhengReward),
m_attackVictoryTimes(data.m_attackVictoryTimes),
m_defendVictoryTimes(data.m_defendVictoryTimes),
m_countryBattleTimes(data.m_countryBattleTimes),
m_releaseName(data.m_releaseName),
m_autoBattle(data.m_autoBattle),
m_curPageNum_Introduction(data.m_curPageNum_Introduction),
m_hadGottenReward(data.m_hadGottenReward),
m_myRankVal(data.m_myRankVal),
m_startedDays(data.m_startedDays),
m_couldGetAwardMap(data.m_couldGetAwardMap),
m_gameIntroductionNoTips(data.m_gameIntroductionNoTips),
m_hadShowFestivalHuodongCanvas(data.m_hadShowFestivalHuodongCanvas),
m_onlinetimer(data.m_onlinetimer),
m_autoBuilding(data.m_autoBuilding),
m_buildCountryTimes(data.m_buildCountryTimes),
m_achieveShopMap(data.m_achieveShopMap),
m_govShopMap(data.m_govShopMap),
m_foreigncard(data.m_foreigncard)
{
	//memcpy(m_hallCard,data.m_hallCard,sizeof(m_hallCard));
	//memcpy(m_privateCard,data.m_privateCard,sizeof(m_privateCard));
	for (int i = 0;i < 3;i++)
	{
		m_luckycard[i] = data.m_luckycard[i];
	}
	m_chatChannelSetInfo = (_ULONGLONG)0x0004220140A05C00;
	m_battleActiveTimer = c_battleActiveInterval;
	//????
	//m_troopsMap.insert(std::make_pair(20012,300000));
	//m_troopsMap.insert(std::make_pair(20018,300000));
	//m_troopsMap.insert(std::make_pair(20017,300000));
	//m_troopsMap.insert(std::make_pair(20006,300000));
	//m_troopsMap.insert(std::make_pair(20020,300000));
	//m_troopsMap.insert(std::make_pair(20022,300000));
	//m_troopsMap.insert(std::make_pair(20015,300000));
	//m_troopsMap.insert(std::make_pair(20016,300000));

	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			m_7DayData[i][j] = data.m_7DayData[i][j];
		}
	}
}
crJXJPlayerData::~crJXJPlayerData()
{
	GNE::LockMutex lock( m_dataMutex );
	if(m_playerStore.valid())
	{
		m_playerStore->clearData();
		m_playerStore = NULL;
	}
}
void crJXJPlayerData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	//int maxpage = 0;
	switch(_type) 
	{
	case WCHDATA_PlayerSex:
		m_playersex = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_PlayerIconID:
		m_playericonid = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJMainRoleBuyIcon:

	//	//m_playerBuyIconList的维护方式不同，这里没用设置内容

	//	break;
	case WCHDATA_JXJShiliID:
		m_shiliid = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJEquipBindInfo:
		m_equipBindInfo = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJYELLOWAWARDINFO:
		m_yellowAwardInfo = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJHomeID:
		m_homeid = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJChengchiID:
		m_chengchiid = (unsigned short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJGoldingot:
	//	m_goldingot = atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJGiftGold:
		m_giftgold = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCoppercash:
		m_coppercash = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFood:
		m_food = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJWood:
		m_wood = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJIron:
		m_iron = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJHorse:
		m_horse = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFuyalv:
		m_fuyalv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJBingyinglv:
		m_bingyinglv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJJiusilv:
		m_jiusilv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGongfanglv:
		m_gongfanglv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJShijilv:
		m_shijilv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFukulv:
		m_fukulv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJMingjulv:
		m_mingjulv = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJMingjulv1:
	case WCHDATA_JXJMingjulv2:
	case WCHDATA_JXJMingjulv3:
	case WCHDATA_JXJMingjulv4:
	case WCHDATA_JXJMingjulv5:
	case WCHDATA_JXJMingjulv6:
	case WCHDATA_JXJMingjulv7:
	case WCHDATA_JXJMingjulv8:
		{
			_crInt32 lo32 = LOINT64(m_mingjulv);
			_crInt32 hi32 = HIINT64(m_mingjulv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			unsigned char mingju[8];
			mingju[0] = (unsigned char)LOINT16(lo16);
			mingju[1] = (unsigned char)HIINT16(lo16);
			mingju[2] = (unsigned char)LOINT16(hi16);
			mingju[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			mingju[4] = (unsigned char)LOINT16(lo16);
			mingju[5] = (unsigned char)HIINT16(lo16);
			mingju[6] = (unsigned char)LOINT16(hi16);
			mingju[7] = (unsigned char)HIINT16(hi16);
			mingju[_type-(WCHDATA_JXJMingjulv1)] = (unsigned char)atoi(relStr.c_str());
			m_mingjulv = MAKEINT64(MAKEINT32(MAKEINT16(mingju[0],mingju[1]),MAKEINT16(mingju[2],mingju[3])),MAKEINT32(MAKEINT16(mingju[4],mingju[5]),MAKEINT16(mingju[6],mingju[7])));
		}
		break;
	case WCHDATA_JXJNongtianlv:
		m_nongtianlv = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJNongtianlv1:
	case WCHDATA_JXJNongtianlv2:
	case WCHDATA_JXJNongtianlv3:
	case WCHDATA_JXJNongtianlv4:
	case WCHDATA_JXJNongtianlv5:
	case WCHDATA_JXJNongtianlv6:
	case WCHDATA_JXJNongtianlv7:
	case WCHDATA_JXJNongtianlv8:
		{
			_crInt32 lo32 = LOINT64(m_nongtianlv);
			_crInt32 hi32 = HIINT64(m_nongtianlv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			unsigned char nongtian[8];
			nongtian[0] = (unsigned char)LOINT16(lo16);
			nongtian[1] = (unsigned char)HIINT16(lo16);
			nongtian[2] = (unsigned char)LOINT16(hi16);
			nongtian[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			nongtian[4] = (unsigned char)LOINT16(lo16);
			nongtian[5] = (unsigned char)HIINT16(lo16);
			nongtian[6] = (unsigned char)LOINT16(hi16);
			nongtian[7] = (unsigned char)HIINT16(hi16);
			nongtian[_type-(WCHDATA_JXJNongtianlv1)] = (unsigned char)atoi(relStr.c_str());
			m_nongtianlv = MAKEINT64(MAKEINT32(MAKEINT16(nongtian[0],nongtian[1]),MAKEINT16(nongtian[2],nongtian[3])),MAKEINT32(MAKEINT16(nongtian[4],nongtian[5]),MAKEINT16(nongtian[6],nongtian[7])));
		}
		break;
	case WCHDATA_JXJMuchanglv:
		m_muchanglv = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJMuchanglv1:
	case WCHDATA_JXJMuchanglv2:
	case WCHDATA_JXJMuchanglv3:
	case WCHDATA_JXJMuchanglv4:
		{
			_crInt16 lo16 = LOINT32(m_muchanglv);
			_crInt16 hi16 = HIINT32(m_muchanglv);
			unsigned char muchang[4];
			muchang[0] = (unsigned char)LOINT16(lo16);
			muchang[1] = (unsigned char)HIINT16(lo16);
			muchang[2] = (unsigned char)LOINT16(hi16);
			muchang[3] = (unsigned char)HIINT16(hi16);
			muchang[_type-(WCHDATA_JXJMuchanglv1)] = (unsigned char)atoi(relStr.c_str());
			m_muchanglv = MAKEINT32(MAKEINT16(muchang[0],muchang[1]),MAKEINT16(muchang[2],muchang[3]));
		}
		break;
	case WCHDATA_JXJLingdilv:
		m_lingdilv = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJLingdilv1:
	case WCHDATA_JXJLingdilv2:
	case WCHDATA_JXJLingdilv3:
	case WCHDATA_JXJLingdilv4:
		{
			_crInt16 lo16 = LOINT32(m_lingdilv);
			_crInt16 hi16 = HIINT32(m_lingdilv);
			unsigned char lingdi[4];
			lingdi[0] = (unsigned char)LOINT16(lo16);
			lingdi[1] = (unsigned char)HIINT16(lo16);
			lingdi[2] = (unsigned char)LOINT16(hi16);
			lingdi[3] = (unsigned char)HIINT16(hi16);
			lingdi[_type-(WCHDATA_JXJLingdilv1)] = (unsigned char)atoi(relStr.c_str());
			m_lingdilv = MAKEINT32(MAKEINT16(lingdi[0],lingdi[1]),MAKEINT16(lingdi[2],lingdi[3]));
		}
		break;
	case WCHDATA_JXJKuangshanlv:
		m_kuangshanlv = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJKuangshanlv1:
	case WCHDATA_JXJKuangshanlv2:
	case WCHDATA_JXJKuangshanlv3:
	case WCHDATA_JXJKuangshanlv4:
		{
			_crInt16 lo16 = LOINT32(m_kuangshanlv);
			_crInt16 hi16 = HIINT32(m_kuangshanlv);
			unsigned char kuangshan[4];
			kuangshan[0] = (unsigned char)LOINT16(lo16);
			kuangshan[1] = (unsigned char)HIINT16(lo16);
			kuangshan[2] = (unsigned char)LOINT16(hi16);
			kuangshan[3] = (unsigned char)HIINT16(hi16);
			kuangshan[_type-(WCHDATA_JXJKuangshanlv1)] = (unsigned char)atoi(relStr.c_str());
			m_kuangshanlv = MAKEINT32(MAKEINT16(kuangshan[0],kuangshan[1]),MAKEINT16(kuangshan[2],kuangshan[3]));
		}
		break;
	//case WCHDATA_JXJVipLv:
	//	m_viplv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	case WCHDATA_UpdateTimer:
		m_updateTimer = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJBuildingTimes:
		crArgumentParser::appAtoVec(relStr,m_buildingTimes);
		break;
	case WCHDATA_JXJRecruitTimes:
		crArgumentParser::appAtoVec(relStr,m_recruitTimes);
		break;
	case WCHDATA_JXJKillEachTroopsCount:
		crArgumentParser::appAtoVec(relStr,m_ckilleachtroops);
		break;
	case WCHDATA_JXJDeadEachTroopsCount:
		crArgumentParser::appAtoVec(relStr,m_cdeadeachtroops);
		break;
	case WCHDATA_JXJDanDhangKillCount:
		crArgumentParser::appAtoVec(relStr,m_danchangkill);
		break;
	case WCHDATA_JXJKillDoorBossCount:
		crArgumentParser::appAtoVec(relStr,m_killdoorboss);
		break;
	case WCHDATA_JXJJunling:
		m_junling = (unsigned short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJMaxTroops:
	//	m_maxtroops = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechJingBinglv:
	//	m_jingbinglv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechJianJialv:
	//	m_jianjialv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechQiangRenlv:
	//	m_qiangrenlv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechGuiMoulv:
	//	m_guimoulv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechGongChenglv:
	//	m_gongchenglv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechYuanGonglv:
	//	m_yuangonglv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechSuGonglv:
	//	m_sugonglv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechQiangXinglv:
	//	m_qiangxinglv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechZhaoMulv:
	//	m_zhaomulv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJTechRenDelv:
	//	m_rendelv = (unsigned char)(atoi(relStr.c_str()));
	//	break;
		//case WCHDATA_JXJCavalrylv:
		//	m_cavalrylv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJArcherlv:
		//	m_archerlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJSiegelv:
		//	m_siegelv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJShieldlv:
		//	m_shieldlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJGuardlv:
		//	m_guardlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJSaddlelv:
		//	m_saddlelv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJAssaultlv:
		//	m_assaultlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJAssistlv:
		//	m_assistlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJPenetratelv:
		//	m_penetratelv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJManufacturelv:
		//	m_manufacturelv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJGunpowderlv:
		//	m_gunpowderlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJBravelv:
		//	m_bravelv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJIroncavalrylv:
		//	m_ironcavalrylv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJGearlv:
		//	m_gearlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJFireboltlv:
		//	m_fireboltlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJMasterlv:
		//	m_masterlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJJuntunlv:
		//	m_juntunlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
		//case WCHDATA_JXJReformlv:
		//	m_reformlv = (unsigned char)(atoi(relStr.c_str()));
		//	break;
	//case WCHDATA_JXJSignin:
	//	m_signin = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	case WCHDATA_JXJGoldCostHintInt:
		m_goldcosthint = (unsigned int)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJHalltimer:
	//	m_halltimer = (short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJPrivatetimer:
	//	m_privatetimer = atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJLevyCount:
		m_levyCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJLevyTimer:
	//	m_levyTimer = atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJOnlineRewardCount:
		m_onlineRewardCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJOnlineRewardTimer:
		m_onlineRewardTimer = atoi(relStr.c_str());
		{
			time_t nowtime = time(0);
			m_onlineRewardTimer = nowtime - m_onlineRewardTimer;
		}
		break;
	case WCHDATA_JXJBuyJunlingCount:
		m_buyJunlingCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJBuyJunlingTimer:
		m_buyJunlingTimer = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJOpenBagNum:
		m_openbagnum = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJLuckyCardCopperCost:
	//	m_luckycardcoppercost = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJLuckyCardGoldCost:
	//	m_luckycardgoldcost = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJKnowLuckyCard:
	//	m_knowluckycard = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJJiusiRefreshCostTime:
	//	m_jiusirefreshtime = (int)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJVipExp:
	//	m_vipExp = atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJCountryContribute:
		m_countrycontribute = (int)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCardImpression:
		m_cardimpression = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCardImpressionLevel:
		m_cardImpressionLevel = (unsigned short)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJBuyResourceCount:
		m_resourceCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJFreeBuyInfo:
		m_freeBuy = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGoldRecruitCount:
		m_goldRecruitCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCardPieces:
		m_cardpieces = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFreeCounterinsurgencyCount:
		m_freeCounterinsurgency = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJRefineEquipFreeCount:
		m_RefineEquipFreeCount = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPNongTianShuiLi:
		m_NongTianShuiLilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPZhiShuZaoLin:
		m_ZhiShuZaoLinlv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPYouZhiCaoChang:
		m_YouZhiCaoChanglv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPJingKuangShenWa:
		m_JingKuangShenWalv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPGuLiMaoYi:
		m_GuLiMaoYilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPZhengShouXinDe:
		m_ZhengShouXinDelv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPYanBingYueWu:
		m_YanBingYueWulv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPXunBingXinDe:
		m_XunBingXinDelv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPZongMiaoJiSi:
		m_ZongMiaoJiSilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJVIPJiSiXinDe:
		m_JiSiXinDelv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_MarketBuyResourceCount:
		m_MarketBuyResourceMax = (int)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJRollCardFreeCount:
		m_FreeRollCardCount = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJAutoSellCard:
		m_autosellcard = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJCardImpression2CritTechLv:
		m_CardImpression2CritTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJCardImpression4CritTechLv:
		m_CardImpression4CritTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJCardImpression6CritTechLv:
		m_CardImpression6CritTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJCardImpression10CritTechLv:
		m_CardImpression10CritTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCardTechLv:
		m_FreeRollCardTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJOrangeCardUpgradeTechLv:
		m_OrangeCardUpgradeTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJCardShopOpenTechLv:
		m_CardShopOpenTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJMakeEquipTechLv:
		m_MakeEquipTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJEquipRefineTechLv:
		m_EquipRefineTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJEquipAttachTechLv:
		m_EquipAttachTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJSmeltEquipTechLv:
		m_SmeltEquipTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJSmeltMaterialTechLv:
		m_SmeltMaterialTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJSmeltStoneTechLv:
		m_SmeltStoneTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeCount1TechLv:
		m_EquipRefineMaxCount1TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeCount2TechLv:
		m_EquipRefineMaxCount2TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeCount3TechLv:
		m_EquipRefineMaxCount3TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeCount4TechLv:
		m_EquipRefineMaxCount4TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeProduce1TechLv:
		m_EquipRefineProduce1TechLv = (unsigned char)(atoi(relStr.c_str())); 
		break;	
	case WCHDATA_JXJAddRefineFreeProduce2TechLv:
		m_EquipRefineProduce2TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeProduce3TechLv:
		m_EquipRefineProduce3TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJAddRefineFreeProduce4TechLv:
		m_EquipRefineProduce4TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCard2TechLv:
		m_FreeRollCard2TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCard3TechLv:
		m_FreeRollCard3TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCardProduce1TechLv:
		m_FreeRollCardProduce1TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCardProduce2TechLv:
		m_FreeRollCardProduce2TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCardProduce3TechLv:
		m_FreeRollCardProduce3TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFreeRollCardProduce4TechLv:
		m_FreeRollCardProduce4TechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJFuKuMaxTechLv:
		m_FuKuMaxTechLv = (unsigned char)(atoi(relStr.c_str()));
		break;	
	case WCHDATA_JXJOpenSaveFormationInfo:
		m_opensavedformationifo = (unsigned char)(atoi(relStr.c_str()));
		//maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,m_viplv).c_str());
		//if (m_opensavedformationifo > maxpage)
		//{
		//	m_opensavedformationifo = maxpage;
		//}
		if (m_opensavedformationifo < 1)
		{
			m_opensavedformationifo = 1;
		}
		break;	
	case WCHDATA_JXJPlayerChangeShiliTimes:
		m_changeShiliTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJSeriesLandDay:
		m_seriesLandDays = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJLandRewardGetCount:
		m_LandRewardGetCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCountryBenefitFeteTimes	:
		m_feteTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCountryBenefitEnrichType:
		m_enrichType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCountryBenefitEnrichTimes:
		m_enrichTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJSkipNewHand:
		m_skipnewhand = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJConsortiaID:
		m_consortiaid = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJSavedFormationIndex:
		m_savedformationindex = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJIfTechBtnNew:
		m_iftechbtnnew = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFuKuMaxTech2Lv:
		m_FuKuMaxTech2Lv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFuKuMaxTech3Lv:
		m_FuKuMaxTech3Lv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJChatChannelSetInfo:
		m_chatChannelSetInfo = (_ULONGLONG)_atoi64(relStr.c_str());
		break;
	case WCHDATA_JXJLingqufengluFlag:
		m_lingqufengluflag = (unsigned char)atoi(relStr.c_str());
		break;
	//case WCHDATA_JXJMainRoleRefreshFlag:
	//	m_mainrolerefreshflag = (unsigned char)atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJTechFoodlv:
		m_tuntianlv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechHorselv:
		m_fanzhilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechIronlv:
		m_yelianlv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechWoodlv:
		m_fanmaolv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechBuildingCDlv:
		m_qiaojianglv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechZhengShuilv:
		m_zhengshuilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechFengluLv:
		m_techfenglulv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechJiusibaojiLv:
		m_techjiusibaojilv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechWorldTalkPriceLv:
		m_techworldtalklv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTechBuyJunlingLv:
		m_techbuyjunlinglv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJFoundCountryGetAward			:
		m_foundCountryAward = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJPlayerTotalPayGold:
		m_playertotalpaygold = (int)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJShuichelv:
		m_shuichelv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJRonglulv:
		m_ronglulv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGongshelv:
		m_gongshelv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJMochuanlv:
		m_mochuanlv = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJServerVersion:
	//	m_serverVersion = atoi(relStr.c_str());
	//	break;
	case WCHDATA_JXJPlayerLoginDays:
		m_loginDays = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJ7DaysGetAward:
		m_7DaysAward = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJShouchong:
		m_shouchong = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJZhengZhanQuan:
		m_cZhengZhanQuan = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJKillPlayerCount:
		m_killplayercount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJKillNpcCount:
		m_killnpccount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDayPlayerDeadCount:
		m_dayplayerdeadcount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerDeadCount:
		m_playerdeadcount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDayKillPlayerCount:
		m_daykillplayercount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDayKillNPCCount:
		m_daykillnpccount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDayKillEachTroops:
		crArgumentParser::appAtoVec(relStr,m_dayckilleachtroops);
		break;
	case WCHDATA_JXJUsedAchievePoints:
		m_usedachievepoint = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPassMingJiangCount:
		m_passmingjiangcount = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPassWorldFuBenCount:
		m_Cpassworldfuben = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJMingJiangFailCount:
		m_cmingjiangfail = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJZhengZhanTianXiaFailCount:
		m_ctianxiafail = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerHaveGotViplvReward:
		m_havegotviplv = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPassZhengZhanTianXiaCount:
		m_Cpasstianxia = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJFinishHuoYueDuDayCount:
		m_cfinishdays = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJAchievePointsCount:
		m_accomplishPointCache = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJHavePraiseCounts:
		m_praisedcount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPraiseOtherCounts:
		m_praisecount = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGuoZhanJiFen:
		m_cGuoZhanJiFen = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJTreasonBufferTimer:
		m_treasonbuffertimer = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJTreasonPunishTimer:
		m_treasonpunishtimer = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJTreasonCoolingTimer:
		m_treasoncoolingtimer = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerSelectedShiLi:
		m_selectedshili = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJAmHuoDongGetTimes:
		m_timesshangwu = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPmHuoDongGetTimes:
		m_timesxiawu = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJNightHuoDongGetTimes:
		m_timeswanshang = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerPraiseLimit:
		m_praiselimit = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerGuanZhi:
		m_guanzhi = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJYueKaID:
		m_yuekaid = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJYueKaEndTime:
		m_yuekaendtime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJYueKaLastGetTime:
		m_lastgettime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJKaoShangFuUsedTime:
		m_kaoshangfutime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJSpecialtyRefreshTimes:
		m_specialtyrefreshtimes = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJLastChongZhiTime:
		m_lastchongzhitime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJLastZeroRefreshTime:
		m_lastzeroRefreshtime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJChongZhiCountT:
		m_chongzhicountT = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJHadWonJunTuanFBT:
		m_hadwonjuntuanfubenT = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJIsCheckHuoYueDuSign:
		m_huoyuedusign = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGainedZhouKaID:
		m_gainedzhoukahuodongid = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJZhouKaEndTime:
		m_zhoukaendtime = (_crInt64)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJZhouKaGetStatus:
		m_zhoukagetstatus = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJZhouKaGetDayIdx:
		m_zhoukagetdayidx = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGroupContributeiType:
		m_groupContributeiType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGroupContributeiTimes:
		m_groupContributeiTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCountryRecommendSet:
		m_recommendSet = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCancelCantonTimer:
		m_cancelCantonTimer = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJUsedZhengZhaoTimes:
		m_zhengZhaoTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGuoZhanAttackVictoryTimes:
		m_attackVictoryTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGuoZhanDefendVictoryTimes:
		m_defendVictoryTimes = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJCountryBattleTimes:
		m_countryBattleTimes = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJAcceptZhengZhaoEnterBroadcast:
		m_releaseName = relStr;
		break;
	case WCHDATA_JXJHadGottenConsortiaPromotionReward:
		m_hadGottenReward = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJConsortiaPromotionMyRankVal:
		m_myRankVal = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJGameStartedDays:
		m_startedDays = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGameIntroductionNoTipsCheck:
		m_gameIntroductionNoTips = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJHadShowFestivalHuodongCanvas:
		m_hadShowFestivalHuodongCanvas = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJOnlineTimer:
		m_onlinetimer = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJAutoBuilding:
		m_autoBuilding = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJBuildCountryTimes:
		m_buildCountryTimes = (char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJForeignCard:
		crArgumentParser::appAtoVec(relStr, m_foreigncard);
		break;
	default:
		break;
	}
}
void crJXJPlayerData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf*)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[256];
		cfg_script.Add("VERSION",JXJVERSION);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJVipLv,(int)m_viplv);
		//cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_PlayerSex,(int)m_playersex);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_PlayerIconID,(int)m_playericonid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJShiliID,(int)m_shiliid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHomeID,(int)m_homeid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJChengchiID,(int)m_chengchiid);
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJGoldingot,m_goldingot);
		//cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGiftGold,m_giftgold);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCoppercash,m_coppercash);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFood,m_food);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJWood,m_wood);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJIron,m_iron);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHorse,m_horse);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFuyalv,(int)m_fuyalv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBingyinglv,(int)m_bingyinglv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJJiusilv,(int)m_jiusilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGongfanglv,(int)m_gongfanglv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJShijilv,(int)m_shijilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFukulv,(int)m_fukulv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJMingjulv,crArgumentParser::appI64toa(m_mingjulv).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJNongtianlv,crArgumentParser::appI64toa(m_nongtianlv).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJMuchanglv,m_muchanglv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJLingdilv,m_lingdilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJKuangshanlv,m_kuangshanlv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_UpdateTimer,crArgumentParser::appI64toa(m_updateTimer).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJBuildingTimes,crArgumentParser::appVectoa(m_buildingTimes).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJRecruitTimes,crArgumentParser::appVectoa(m_recruitTimes).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJKillEachTroopsCount,crArgumentParser::appVectoa(m_ckilleachtroops).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDeadEachTroopsCount,crArgumentParser::appVectoa(m_cdeadeachtroops).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDanDhangKillCount,crArgumentParser::appVectoa(m_danchangkill).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJKillDoorBossCount,crArgumentParser::appVectoa(m_killdoorboss).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJJunling,(int)m_junling);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryBenefitFeteTimes,m_feteTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryBenefitEnrichType,m_enrichType);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryBenefitEnrichTimes,m_enrichTimes);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortiaID,m_consortiaid);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJFoundCountryGetAward,m_foundCountryAward);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerLoginDays,m_loginDays);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJ7DaysGetAward,m_7DaysAward);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJShouchong,m_shouchong);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJZhengZhanQuan,m_cZhengZhanQuan);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJKillPlayerCount,m_killplayercount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJKillNpcCount,m_killnpccount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDayPlayerDeadCount,m_dayplayerdeadcount);
		cfg_script.Add("Param",buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJPlayerDeadCount, m_playerdeadcount);
		cfg_script.Add("Param", buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDayKillPlayerCount,m_daykillplayercount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDayKillNPCCount,m_daykillnpccount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDayKillEachTroops,crArgumentParser::appVectoa(m_dayckilleachtroops));
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJUsedAchievePoints,m_usedachievepoint);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPassMingJiangCount,(int)m_passmingjiangcount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPassWorldFuBenCount,(int)m_Cpassworldfuben);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJMingJiangFailCount,(int)m_cmingjiangfail);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJZhengZhanTianXiaFailCount,(int)m_ctianxiafail);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerHaveGotViplvReward,(int)m_havegotviplv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPassZhengZhanTianXiaCount,(int)m_Cpasstianxia);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFinishHuoYueDuDayCount,(int)m_cfinishdays);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAchievePointsCount,m_accomplishPointCache);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHavePraiseCounts,m_praisedcount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPraiseOtherCounts,m_praisecount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGuoZhanJiFen,m_cGuoZhanJiFen);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJTreasonBufferTimer,m_treasonbuffertimer);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJTreasonPunishTimer,m_treasonpunishtimer);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJTreasonCoolingTimer,m_treasoncoolingtimer);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerSelectedShiLi,m_selectedshili);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAmHuoDongGetTimes,m_timesshangwu);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPmHuoDongGetTimes,m_timesxiawu);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJNightHuoDongGetTimes,m_timeswanshang);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerPraiseLimit,m_praiselimit);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJYueKaID,m_yuekaid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJYueKaEndTime,crArgumentParser::appI64toa(m_yuekaendtime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJYueKaLastGetTime,crArgumentParser::appI64toa(m_lastgettime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJKaoShangFuUsedTime,crArgumentParser::appI64toa(m_kaoshangfutime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSpecialtyRefreshTimes,(int)m_specialtyrefreshtimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJLastChongZhiTime,crArgumentParser::appI64toa(m_lastchongzhitime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJLastZeroRefreshTime,crArgumentParser::appI64toa(m_lastzeroRefreshtime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJChongZhiCountT,m_chongzhicountT);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHadWonJunTuanFBT,(int)m_hadwonjuntuanfubenT);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJIsCheckHuoYueDuSign,(int)m_huoyuedusign);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGainedZhouKaID,m_gainedzhoukahuodongid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJZhouKaEndTime,crArgumentParser::appI64toa(m_zhoukaendtime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJZhouKaGetStatus,m_zhoukagetstatus);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJZhouKaGetDayIdx,m_zhoukagetdayidx);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryRecommendSet,m_recommendSet);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJEquipBindInfo,m_equipBindInfo);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJYELLOWAWARDINFO,m_yellowAwardInfo);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCancelCantonTimer,m_cancelCantonTimer);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHadGottenConsortiaPromotionReward,(int)m_hadGottenReward);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortiaPromotionMyRankVal,(int)m_myRankVal);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGameStartedDays,(int)m_startedDays);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGameIntroductionNoTipsCheck,(int)m_gameIntroductionNoTips);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJHadShowFestivalHuodongCanvas,(int)m_hadShowFestivalHuodongCanvas);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJOnlineTimer,m_onlinetimer);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAutoBuilding,(int)m_autoBuilding);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBuildCountryTimes,(int)m_buildCountryTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf, "%d,%s\0", WCHDATA_JXJForeignCard, crArgumentParser::appVectoa(m_foreigncard).c_str());
		cfg_script.Add("Param", buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJServerVersion,m_serverVersion);
		//cfg_script.Add("Param",buf);

		////预备部队
		std::vector<std::string> v_str;
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJMaxTroops,(int)m_maxtroops);
		//cfg_script.Add("Param",buf);
		if(!m_troopsMap.empty())
		{
			cfg_script.Add("TroopsMap");
			cfg_script.Push("TroopsMap");
			v_str.resize(2);
			for( TroopsMap::iterator itr = m_troopsMap.begin();
				itr != m_troopsMap.end();
				++itr )
			{
				if(itr->second>0)
				{
					v_str[0] = crArgumentParser::appItoa(itr->first);
					v_str[1] = crArgumentParser::appItoa(itr->second);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		if(!m_troopsEquipMap.empty())
		{
			cfg_script.Add("TroopsEquipMap");
			cfg_script.Push("TroopsEquipMap");
			v_str.resize(2);
			for( TroopsEquipMap::iterator itr = m_troopsEquipMap.begin();
				itr != m_troopsEquipMap.end();
				++itr )
			{
				if(itr->second>0)
				{
					v_str[0] = crArgumentParser::appItoa(itr->first);
					v_str[1] = crArgumentParser::appItoa(itr->second);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		/////////////////背包已开格子数
		sprintf(buf,"%d,%d\0",WCHDATA_JXJOpenBagNum,(int)m_openbagnum);
		cfg_script.Add("Param",buf);
		/////////背包列表
		if(!m_itembagvec.empty())
		{
			int openbagnum = m_itembagvec.size();
			cfg_script.Add("JXJItemBagVec");
			cfg_script.Push("JXJItemBagVec");
			v_str.resize(3);
			for( int i = 0; i < openbagnum; i++ )
			{
				if((m_itembagvec)[i].valid() && (m_itembagvec)[i]->getItemCount()>0)
				{
					v_str[0] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemID());
					v_str[1] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemCount());
					v_str[2] = crArgumentParser::appItoa((m_itembagvec)[i]->getEquipMagic());
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
					v_str[2] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		//科技等级
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechJingBinglv,(int)m_jingbinglv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechJianJialv,(int)m_jianjialv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechQiangRenlv,(int)m_qiangrenlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechGuiMoulv,(int)m_guimoulv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechGongChenglv,(int)m_gongchenglv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechYuanGonglv,(int)m_yuangonglv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechSuGonglv,(int)m_sugonglv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechQiangXinglv,(int)m_qiangxinglv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechZhaoMulv,(int)m_zhaomulv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJTechRenDelv,(int)m_rendelv);
		//cfg_script.Add("Param",buf);

		//sprintf(buf,"%d,%d\0",WCHDATA_JXJInfantrylv,(int)m_infantrylv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJCavalrylv,(int)m_cavalrylv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJArcherlv,(int)m_archerlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJSiegelv,(int)m_siegelv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJShieldlv,(int)m_shieldlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJGuardlv,(int)m_guardlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJSaddlelv,(int)m_saddlelv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJAssaultlv,(int)m_assaultlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJAssistlv,(int)m_assistlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJPenetratelv,(int)m_penetratelv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJManufacturelv,(int)m_manufacturelv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJGunpowderlv,(int)m_gunpowderlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJBravelv,(int)m_bravelv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJIroncavalrylv,(int)m_ironcavalrylv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJGearlv,(int)m_gearlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJFireboltlv,(int)m_fireboltlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJMasterlv,(int)m_masterlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJJuntunlv,(int)m_juntunlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJReformlv,(int)m_reformlv);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJSignin,(int)m_signin);
		//cfg_script.Add("Param",buf);
		//////FormationInfo
		//if(!m_formationInfoVec.empty())
		//{
		//	cfg_script.Add("FormationInfo");
		//	cfg_script.Push("FormationInfo");
		//	v_str.resize(4);
		//	for( FormationInfoVec::iterator itr = m_formationInfoVec.begin();
		//		itr != m_formationInfoVec.end();
		//		++itr )
		//	{
		//		v_str[0] = crArgumentParser::appItoa((*itr)->getAbstractID());
		//		v_str[1] = crArgumentParser::appItoa((*itr)->getCount());
		//		v_str[2] = crArgumentParser::appItoa((*itr)->getEquips()[0]);
		//		v_str[3] = crArgumentParser::appItoa((*itr)->getEquips()[1]);
		//		//v_i.push_back((*itr)->getEquips()[2]);
		//		cfg_script.Add("Param",v_str);
		//	}
		//	cfg_script.Pop();
		//}
		//消费提示
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGoldCostHintInt,(int)m_goldcosthint);
		cfg_script.Add("Param",buf);
		//////好友列表
		if(!m_friendList.empty())
		{
			int nFriend = 1;
			for( FriendVec::iterator itr = m_friendList.begin();
				itr != m_friendList.end();
				++itr )
			{
				cfg_script.Add("FriendList");
				cfg_script.Push("FriendList",nFriend++);
				cfg_script.Add("Param",(*itr)->getPlayerID());
				cfg_script.Add("Param",(*itr)->getName());
				cfg_script.Add("Param",(int)((*itr)->getLevel()));
				cfg_script.Add("Param",(int)(*itr)->getIconID());
				cfg_script.Pop();
			}
		}
		//将卡刷新计时器
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJHalltimer,(int)m_halltimer);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJPrivatetimer,m_privatetimer);
		//cfg_script.Add("Param",buf);
		//cfg_script.Add("HallCard");
		//cfg_script.Push("HallCard");
		//v_i.clear();
		//for(int i = 0; i<5; i++)
		//{
		//	v_i.push_back(m_hallCard[i]);
		//}
		//cfg_script.Add("Param",v_i);
		//cfg_script.Pop();
		//cfg_script.Add("PrivateCard");
		//cfg_script.Push("PrivateCard");
		//v_i.clear();
		//for(int i = 0; i<3; i++)
		//{
		//	v_i.push_back(m_privateCard[i]);
		//}
		//cfg_script.Add("Param",v_i);
		//cfg_script.Pop();
		////征税
		sprintf(buf,"%d,%d\0",WCHDATA_JXJLevyCount,(int)m_levyCount);
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJLevyTimer,m_levyTimer);
		//cfg_script.Add("Param",buf);
		////在线奖励
		sprintf(buf,"%d,%d\0",WCHDATA_JXJOnlineRewardCount,(int)m_onlineRewardCount);
		cfg_script.Add("Param",buf);
		time_t nowtime = time(0);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJOnlineRewardTimer,crArgumentParser::appItoa(nowtime-m_onlineRewardTimer).c_str());
		cfg_script.Add("Param",buf);
		////军令购买次数
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBuyJunlingCount,(int)m_buyJunlingCount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBuyJunlingTimer,(int)m_buyJunlingTimer);
		cfg_script.Add("Param",buf);
		//寄售         
		if(!m_saleList.empty())
		{
			int nSale = 1;
			for( SaleList::iterator itr = m_saleList.begin();
				 itr != m_saleList.end();
				 ++itr )
			{
				if((*itr)->getSaleState() != Sale_Canceled)
				{
					cfg_script.Add("SaleList");
					cfg_script.Push("SaleList",nSale++);
					cfg_script.Add("Param",(*itr)->getSaleListID());
					cfg_script.Add("Param",(*itr)->getGoodsType());
					cfg_script.Add("Param",(*itr)->getGoodsID());
					cfg_script.Add("Param",(*itr)->getPrice());
					cfg_script.Add("Param",(*itr)->getCount());
					cfg_script.Add("Param",(*itr)->prepareTimer());
					cfg_script.Add("Param",(*itr)->getSaleID());
					cfg_script.Add("Param",(*itr)->saleTimer());
					cfg_script.Add("Param",(int)((*itr)->getSaleState()));
					cfg_script.Add("Param",(*itr)->getGaincash());
					cfg_script.Pop();
				}
			}
		}
		//////最近出售列表
		if(!m_saleitemlist.empty())
		{
			cfg_script.Add("SaleItemDeq");
			cfg_script.Push("SaleItemDeq");
			v_str.resize(4);
			for( JXJSaleItemList::iterator itr = m_saleitemlist.begin();
				itr != m_saleitemlist.end();
				++itr )
			{
				v_str[0] = crArgumentParser::appItoa((*itr)->getItemID());
				v_str[1] = crArgumentParser::appItoa((int)(*itr)->getItemCount());
				v_str[2] = crArgumentParser::appItoa((int)(*itr)->getTime());
				v_str[3] = crArgumentParser::appItoa((*itr)->getEquipMagic());
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////将卡背包列表
		//unsigned char bagnummax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJiangkaBagSize,m_viplv).c_str()));
		if(!m_jiangkabagvec.empty())
		{
			int bagcount = m_jiangkabagvec.size();
			cfg_script.Add("JXJJiangkaItemBagVec");
			cfg_script.Push("JXJJiangkaItemBagVec");
			v_str.resize(2);
			for( int i = 0;i<bagcount ; i++)
			{
				if(m_jiangkabagvec[i].valid() && m_jiangkabagvec[i]->getItemCount()>0)
				{
					v_str[0] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getItemID());
					v_str[1] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getItemCount());
					//v_str[2] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getEquipMagic());
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
					//v_str[2] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////将卡派遣列表
		unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,m_viplv).c_str()));
		if(!m_appointjiangkabagvec.empty())
		{
			cfg_script.Add("JXJAppointJiangkaItemBagVec");
			cfg_script.Push("JXJAppointJiangkaItemBagVec");
			v_str.resize(2);
			for( int i = 0;i<bagnummax2 ; i++)
			{
				if(m_appointjiangkabagvec[i].valid())
				{
					v_str[0] = crArgumentParser::appItoa(m_appointjiangkabagvec[i]->getItemID());
					v_str[1] = crArgumentParser::appItoa(m_appointjiangkabagvec[i]->getValidTime());
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////FubenProgressMap
		if(!m_fubenProgressMap.empty())
		{
			cfg_script.Add("FubenProgressMap");
			cfg_script.Push("FubenProgressMap");
			v_str.resize(4);
			for( FubenProgressMap::iterator itr = m_fubenProgressMap.begin();
				itr != m_fubenProgressMap.end();
				++itr )
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second->getFubenProgress());
				v_str[2] = crArgumentParser::appItoa(itr->second->getComplete());
				v_str[3] = crArgumentParser::appItoa(itr->second->getStarCount());
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////ExtraProduceMap
		if(!m_extraProduceMap.empty())
		{
			//unsigned char extramaxcount = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJExtraProduceMaxCount).c_str()));
			cfg_script.Add("ExtraProduceMap");
			cfg_script.Push("ExtraProduceMap");
			int i = 0;
			v_str.resize(3);
			for( ExtraProduceMap::iterator itr = m_extraProduceMap.begin();
				itr != m_extraProduceMap.end()/* && i<extramaxcount*/;
				++itr,++i )
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second->timer());
				v_str[2] = crArgumentParser::appItoa(itr->second->getPercent());
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJLuckyCardCopperCost,(int)m_luckycardcoppercost);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJLuckyCardGoldCost,(int)m_luckycardgoldcost);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJKnowLuckyCard,(int)m_knowluckycard);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJJiusiRefreshCostTime,(int)m_jiusirefreshtime);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJVipExp,m_vipExp);
		//cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryContribute,(int)m_countrycontribute);
		cfg_script.Add("Param",buf);
		//////////////////////////////////////////////////////////////////////////
		cfg_script.Add("7DayData");
		cfg_script.Push("7DayData");
		for (int i = 0 ; i < 7; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				cfg_script.Add("Param",crArgumentParser::appItoa(m_7DayData[i][j]));
			}
		}
		cfg_script.Pop();
		////////////////////////////////////
		cfg_script.Add("LuckyCard");
		cfg_script.Push("LuckyCard");
		cfg_script.Add("Param",m_luckycard[0]);
		cfg_script.Add("Param",m_luckycard[1]);
		cfg_script.Add("Param",m_luckycard[2]);
		cfg_script.Pop();
		/////////////////////////////////////
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpression,(int)m_cardimpression);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardPieces,m_cardpieces);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeCounterinsurgencyCount,m_freeCounterinsurgency);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJRefineEquipFreeCount,(unsigned short)m_RefineEquipFreeCount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPNongTianShuiLi,(int)m_NongTianShuiLilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPZhiShuZaoLin,(int)m_ZhiShuZaoLinlv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPYouZhiCaoChang,(int)m_YouZhiCaoChanglv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPJingKuangShenWa,(int)m_JingKuangShenWalv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPGuLiMaoYi,(int)m_GuLiMaoYilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPZhengShouXinDe,(int)m_ZhengShouXinDelv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPYanBingYueWu,(int)m_YanBingYueWulv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPXunBingXinDe,(int)m_XunBingXinDelv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPZongMiaoJiSi,(int)m_ZongMiaoJiSilv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJVIPJiSiXinDe,(int)m_JiSiXinDelv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_MarketBuyResourceCount,m_MarketBuyResourceMax);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJRollCardFreeCount,m_FreeRollCardCount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAutoSellCard,(int)m_autosellcard);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpressionLevel,(int)m_cardImpressionLevel);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBuyResourceCount,(int)m_resourceCount);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeBuyInfo,(int)m_freeBuy);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGoldRecruitCount,(int)m_goldRecruitCount);
		cfg_script.Add("Param",buf);
		/////////军备map（兵种科技）
		if(!m_troopstechmap.empty())
		{
			cfg_script.Add("JXJTroopsTechMap");
			cfg_script.Push("JXJTroopsTechMap");
			v_str.resize(2);
			for(TroopsTechMap::iterator itr = m_troopstechmap.begin();
				itr != m_troopstechmap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		////////组合将卡科技等级
		if(!m_cardExtraTechMap.empty())
		{
			cfg_script.Add("CardExtraTechMap");
			cfg_script.Push("CardExtraTechMap");
			v_str.resize(2);
			for(CardExtraTechMap::iterator itr = m_cardExtraTechMap.begin();
				itr != m_cardExtraTechMap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////玩家在征战商店每日可购买物品上限
		if(!m_buyshagnxianmap.empty())
		{
			cfg_script.Add("JXJBuyShangXianMap");
			cfg_script.Push("JXJBuyShangXianMap");
			v_str.resize(2);
			for(std::map<int,unsigned char>::iterator itr = m_buyshagnxianmap.begin();
				itr != m_buyshagnxianmap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		/////////玩家兵种成就相关信息
		if(!m_troopsachievemap.empty())
		{
			cfg_script.Add("JXJTroopsAchieveMap");
			cfg_script.Push("JXJTroopsAchieveMap");
			v_str.resize(4);
			for(std::map<int,CRCore::crVector3i>::iterator itr = m_troopsachievemap.begin();
				itr != m_troopsachievemap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second[0]);
				v_str[2] = crArgumentParser::appItoa(itr->second[1]);
				v_str[3] = crArgumentParser::appItoa(itr->second[2]);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		///////新科技
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpression2CritTechLv,(int)m_CardImpression2CritTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpression4CritTechLv,(int)m_CardImpression4CritTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpression6CritTechLv,(int)m_CardImpression6CritTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardImpression10CritTechLv,(int)m_CardImpression10CritTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCardTechLv,(int)m_FreeRollCardTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJOrangeCardUpgradeTechLv,(int)m_OrangeCardUpgradeTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCardShopOpenTechLv,(int)m_CardShopOpenTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJMakeEquipTechLv,(int)m_MakeEquipTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJEquipRefineTechLv,(int)m_EquipRefineTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJEquipAttachTechLv,(int)m_EquipAttachTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSmeltEquipTechLv,(int)m_SmeltEquipTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSmeltMaterialTechLv,(int)m_SmeltMaterialTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSmeltStoneTechLv,(int)m_SmeltStoneTechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeCount1TechLv,(int)m_EquipRefineMaxCount1TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeCount2TechLv,(int)m_EquipRefineMaxCount2TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeCount3TechLv,(int)m_EquipRefineMaxCount3TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeCount4TechLv,(int)m_EquipRefineMaxCount4TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeProduce1TechLv,(int)m_EquipRefineProduce1TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeProduce2TechLv,(int)m_EquipRefineProduce2TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeProduce3TechLv,(int)m_EquipRefineProduce3TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAddRefineFreeProduce4TechLv,(int)m_EquipRefineProduce4TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCard2TechLv,(int)m_FreeRollCard2TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCard3TechLv,(int)m_FreeRollCard3TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCardProduce1TechLv,(int)m_FreeRollCardProduce1TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCardProduce2TechLv,(int)m_FreeRollCardProduce2TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCardProduce3TechLv,(int)m_FreeRollCardProduce3TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFreeRollCardProduce4TechLv,(int)m_FreeRollCardProduce4TechLv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFuKuMaxTechLv,(int)m_FuKuMaxTechLv);
		cfg_script.Add("Param",buf);

		//////////////////////////////////////////////////////////////////////////////////
		sprintf(buf,"%d,%d\0",WCHDATA_JXJOpenSaveFormationInfo,(int)m_opensavedformationifo);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerChangeShiliTimes,(int)m_changeShiliTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSeriesLandDay,(int)m_seriesLandDays);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJLandRewardGetCount,(int)m_LandRewardGetCount);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJGroupContributeiType,(int)m_groupContributeiType);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGroupContributeiTimes,(int)m_groupContributeiTimes);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%I64d\0",WCHDATA_JXJChatChannelSetInfo,m_chatChannelSetInfo.to_ullong());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJUsedZhengZhaoTimes,(int)m_zhengZhaoTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGuoZhanAttackVictoryTimes,(int)m_attackVictoryTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGuoZhanDefendVictoryTimes,(int)m_defendVictoryTimes);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryBattleTimes,(int)m_countryBattleTimes);
		cfg_script.Add("Param",buf);
		//////黑名单
		if(!m_blacknameset.empty())
		{
			cfg_script.Add("JXJBlackNameSet");
			cfg_script.Push("JXJBlackNameSet");
			v_str.resize(1);
			for(BlackNameSet::iterator itr = m_blacknameset.begin();
				itr != m_blacknameset.end();
				++itr)
			{
				v_str[0] = (*itr);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		////////研发中的科技map
		if(!m_techdevelopmap.empty())
		{
			cfg_script.Add("JXJTechDevelopMap");
			cfg_script.Push("JXJTechDevelopMap");
			v_str.resize(2);
			for(TechDevelopMap::iterator itr = m_techdevelopmap.begin();
				itr != m_techdevelopmap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);//???Int??
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		////说服队列
		//if(!m_persuadeInfoDeq.empty())
		//{
		//	cfg_script.Add("JXJPersuadeDeq");
		//	cfg_script.Push("JXJPersuadeDeq");
		//	v_str.resize(4);
		//	for(PersuadeInfoDeq::iterator itr = m_persuadeInfoDeq.begin();
		//		itr != m_persuadeInfoDeq.end();
		//		++itr)
		//	{
		//		v_str[0] = crArgumentParser::appItoa(itr->get()->getFubenId());
		//		v_str[1] = crArgumentParser::appItoa(itr->get()->getJKId());
		//		v_str[2] = crArgumentParser::appI64toa(itr->get()->getTime());
		//		v_str[3] = crArgumentParser::appItoa(itr->get()->getLevel());
		//		cfg_script.Add("Param",v_str);
		//	}
		//	cfg_script.Pop();
		//}
		//////SavedFormationInfo
		if(!m_savedformationinfovec.empty())
		{
			cfg_script.Add("SavedFormationInfo");
			cfg_script.Push("SavedFormationInfo");
			v_str.resize(4);
			for( SavedFormationInfoVec::iterator itr = m_savedformationinfovec.begin();
				itr != m_savedformationinfovec.end();
				++itr )
			{
				cfg_script.Add("Param",itr->first);
				for( FormationInfoVec::iterator fitr = itr->second.begin();
					fitr != itr->second.end();
					++fitr )
				{
					v_str[0] = crArgumentParser::appItoa((*fitr)->getAbstractID());
					v_str[1] = crArgumentParser::appItoa((*fitr)->getCount());
					v_str[2] = crArgumentParser::appItoa((*fitr)->getEquips()[0]);
					v_str[3] = crArgumentParser::appItoa((*fitr)->getEquips()[1]);
					//v_i.push_back((*itr)->getEquips()[2]);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		////////GroupCantonSpecialtyVec
		if(!m_groupCantonSpecialtyVec.empty())
		{
			cfg_script.Add("GroupCantonSpecialtyVec");
			cfg_script.Push("GroupCantonSpecialtyVec");
			v_str.resize(4);
			for( int i = 0;i < 8 ; i++)
			{
				if(m_groupCantonSpecialtyVec[i].valid())
				{
					v_str[0] = crArgumentParser::appItoa(m_groupCantonSpecialtyVec[i]->m_contribute);
					v_str[1] = crArgumentParser::appItoa(m_groupCantonSpecialtyVec[i]->m_itemid);
					v_str[2] = crArgumentParser::appItoa(m_groupCantonSpecialtyVec[i]->m_itemnum);
					v_str[3] = crArgumentParser::appItoa(m_groupCantonSpecialtyVec[i]->m_itemtype);
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
					v_str[2] = "";
					v_str[3] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		////////物品最后使用时间map
		if(!m_itemusedtimeMap.empty())
		{
			cfg_script.Add("ItemUsedTimeMap");
			cfg_script.Push("ItemUsedTimeMap");
			v_str.resize(2);
			//v_str.clear();
			for(ItemUsedTimeMap::iterator itr = m_itemusedtimeMap.begin();
				itr != m_itemusedtimeMap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appI64toa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}

		////////轮回福利map
		if(!m_lunhuiRewardMap.empty())
		{
			cfg_script.Add("JXJLunHuiRewardMap");
			cfg_script.Push("JXJLunHuiRewardMap");
			//v_str.resize(2);
			v_str.clear();
			for(LunHuiRewardMap::iterator itr = m_lunhuiRewardMap.begin();
				itr != m_lunhuiRewardMap.end();
				++itr)
			{
				cfg_script.Add("Param",crArgumentParser::appItoa(itr->first));
				cfg_script.Add("Param",crArgumentParser::appItoa(itr->second.first));
				cfg_script.Add("Param",crArgumentParser::appVectoa(itr->second.second));				
			}
			cfg_script.Pop();
		}
		////////////////////////////////////////////////
		//////////////////////////////////// 玩家购买头像列表 ////////////////////////////////////////
		//if(!m_playerBuyIconVec.empty())
		//{
		//	int openbagnum = m_playerBuyIconVec.size();
		//	cfg_script.Add("JXJPlayerBuyIconVec");
		//	cfg_script.Push("JXJPlayerBuyIconVec");
		//	for( int i = 0; i < openbagnum; i++ )
		//	{
		//		cfg_script.Add("Param", m_playerBuyIconVec[i]);
		//	}
		//	cfg_script.Pop();
		//}
		//////////////投票记录
		if (!m_selectLeaderIdSet.empty())
		{
			cfg_script.Add("SelectLeaderIdSet");
			cfg_script.Push("SelectLeaderIdSet");
			SelectLeaderIdSet::iterator slsItr = m_selectLeaderIdSet.begin();
			for (;slsItr != m_selectLeaderIdSet.end(); ++ slsItr)
			{
				cfg_script.Add("Param", *slsItr);
			}
			cfg_script.Pop();
		}


		//MyGroupApplySet
		if(!m_myGroupApplySet.empty())
		{
			cfg_script.Add("MyGroupApplySet");
			cfg_script.Push("MyGroupApplySet");
			for( MyGroupApplySet::iterator itr = m_myGroupApplySet.begin();
				itr != m_myGroupApplySet.end(); ++itr )
			{
				cfg_script.Add("Param",*itr);
			}
			cfg_script.Pop();
		}

		// 已经通关的军团副本id
		if (!m_hadWonGroupFuBenSet.empty())
		{
			cfg_script.Add("HadWonGroupFuBenSet");
			cfg_script.Push("HadWonGroupFuBenSet");
			HadWonGroupFuBenSet::iterator hwgfItr = m_hadWonGroupFuBenSet.begin();
			for (;hwgfItr != m_hadWonGroupFuBenSet.end(); ++ hwgfItr )
			{
				cfg_script.Add("Param", *hwgfItr );
			}
			cfg_script.Pop();
		}

		////////////////////////////////////////////////////
		//跳过新手引导
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSkipNewHand,(int)m_skipnewhand);
		cfg_script.Add("Param",buf);
		//保存编队索引
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSavedFormationIndex,(int)m_savedformationindex);
		cfg_script.Add("Param",buf);
		//科技图标是否闪烁
		sprintf(buf,"%d,%d\0",WCHDATA_JXJIfTechBtnNew,(int)m_iftechbtnnew);
		cfg_script.Add("Param",buf);
		//府库最大值科技
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFuKuMaxTech2Lv,(int)m_FuKuMaxTech2Lv);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJFuKuMaxTech3Lv,(int)m_FuKuMaxTech3Lv);
		cfg_script.Add("Param", buf);
		// 新功能开启提示set，已提示过的在set中做记录
		if (!m_newfunctionopenset.empty())
		{
			cfg_script.Add("NewFunctionOpenSet");
			cfg_script.Push("NewFunctionOpenSet");
			NewFunctionOpenSet::iterator newfuncItr = m_newfunctionopenset.begin();
			for (; newfuncItr != m_newfunctionopenset.end(); ++newfuncItr)
			{
				cfg_script.Add("Param", (int)(*newfuncItr));
			}
			cfg_script.Pop();
		}
		//领取俸禄标识符
		sprintf(buf, "%d,%d\0", WCHDATA_JXJLingqufengluFlag, (int)m_lingqufengluflag);
		cfg_script.Add("Param", buf);
		////是否刷新mainrole
		//sprintf(buf, "%d,%d\0", WCHDATA_JXJMainRoleRefreshFlag, (int)m_mainrolerefreshflag);
		//cfg_script.Add("Param", buf);
		//九鼎科技。科技
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechFoodlv, (int)m_tuntianlv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechHorselv, (int)m_fanzhilv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechIronlv, (int)m_yelianlv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechWoodlv, (int)m_fanmaolv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechBuildingCDlv, (int)m_qiaojianglv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechZhengShuilv, (int)m_zhengshuilv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechFengluLv, (int)m_techfenglulv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechJiusibaojiLv, (int)m_techjiusibaojilv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechWorldTalkPriceLv, (int)m_techworldtalklv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJTechBuyJunlingLv, (int)m_techbuyjunlinglv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJPlayerTotalPayGold, m_playertotalpaygold);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJShuichelv, (int)m_shuichelv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJRonglulv, (int)m_ronglulv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJGongshelv, (int)m_gongshelv);
		cfg_script.Add("Param", buf);
		sprintf(buf, "%d,%d\0", WCHDATA_JXJMochuanlv, (int)m_mochuanlv);
		cfg_script.Add("Param", buf);

		////////成就商店商品购买次数记录map
		if(!m_achieveShopMap.empty())
		{
			cfg_script.Add("AchieveShopMap");
			cfg_script.Push("AchieveShopMap");
			v_str.resize(2);
			//v_str.clear();
			for(AchieveShopMap::iterator itr = m_achieveShopMap.begin();
				itr != m_achieveShopMap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		////////道具商店商品购买次数记录map
		if(!m_govShopMap.empty())
		{
			cfg_script.Add("GovShopMap");
			cfg_script.Push("GovShopMap");
			v_str.resize(2);
			//v_str.clear();
			for(GovShopMap::iterator itr = m_govShopMap.begin();
				itr != m_govShopMap.end();
				++itr)
			{
				v_str[0] = crArgumentParser::appItoa(itr->first);
				v_str[1] = crArgumentParser::appItoa(itr->second);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		//////////////////////////////////////////////////////////////////////////
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		//void *param;
		//crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		//crData *data = myPlayer->getDataClass();
		//data->getParam(WCHDATA_JXJVipLv,param);
		//unsigned char viplv = *(unsigned char *)param;
		//int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,m_viplv).c_str());

		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		//m_dataStream = new crStreamBuf;
		stream->createBuf(8192);
		///////
		stream->_writeUChar(m_viplv);
		stream->_writeBool(m_playersex);
		stream->_writeUChar(m_playericonid);
		//stream->_writeUChar((unsigned char)(nMaxBuyIconCount));
		//for (int i=0; i<nMaxBuyIconCount; i++)
		//{
		//	if (i >= m_playerBuyIconVec.size())
		//	{
		//		stream->_writeUChar(0);
		//	}
		//	else
		//	{
		//		stream->_writeUChar(m_playerBuyIconVec[i]);
		//	}
		//}
		stream->_writeUChar(m_shiliid);
		stream->_writeUShort(m_homeid);
		stream->_writeUShort(m_chengchiid);
		stream->_writeInt(m_goldingot);
		stream->_writeInt(m_giftgold);
		stream->_writeInt(m_coppercash);
		stream->_writeInt(m_food);
		stream->_writeInt(m_wood);
		stream->_writeInt(m_iron);
		stream->_writeInt(m_horse);
		stream->_writeUChar(m_fuyalv);
		stream->_writeUChar(m_bingyinglv);
		stream->_writeUChar(m_jiusilv);
		stream->_writeUChar(m_gongfanglv);
		stream->_writeUChar(m_shijilv);
		stream->_writeUChar(m_fukulv);
		stream->_writeInt64(m_mingjulv);
		stream->_writeInt64(m_nongtianlv);
		stream->_writeInt(m_muchanglv);
		stream->_writeInt(m_lingdilv);
		stream->_writeInt(m_kuangshanlv);
		//预备部队
		//stream->_writeUChar(m_maxtroops);
		stream->_writeUChar(m_troopsMap.size());
		for( TroopsMap::iterator itr = m_troopsMap.begin();
			 itr != m_troopsMap.end();
			 ++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		stream->_writeUChar(m_troopsEquipMap.size());
		for( TroopsEquipMap::iterator itr = m_troopsEquipMap.begin();
			itr != m_troopsEquipMap.end();
			++itr )
		{
			stream->_writeUShort(itr->first);
			stream->_writeInt(itr->second);
		}
		//背包格子数
		stream->_writeUChar(m_openbagnum);
		if(m_itembagvec.size()<m_openbagnum)
			m_itembagvec.resize(m_openbagnum);
		//背包列表
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int itemtypeindex= itemtab->getTitleIndex("类型");
		int itemtype;
		crTableIO::StrVec itemrecord;
		for( int i = 0; i < m_openbagnum; i++)
		{
			if(m_itembagvec[i].valid() && m_itembagvec[i]->getItemCount()>0)
			{
				stream->_writeUChar(m_itembagvec[i]->getItemCount());
				stream->_writeInt(m_itembagvec[i]->getItemID());
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(m_itembagvec[i]->getItemID()),itemrecord)>=0)
				{
					itemtype = atoi(itemrecord[itemtypeindex].c_str());
					if(itemtype == IT_Equip)
						stream->_writeInt(m_itembagvec[i]->getEquipMagic());
				}
			}
			else
			{
				stream->_writeUChar(0);
			}
		}
		//科技等级
/*		stream->_writeUChar(m_infantrylv);
		stream->_writeUChar(m_cavalrylv);
		stream->_writeUChar(m_archerlv);
		stream->_writeUChar(m_siegelv);
		stream->_writeUChar(m_shieldlv);
		stream->_writeUChar(m_guardlv);
		stream->_writeUChar(m_saddlelv);
		stream->_writeUChar(m_assaultlv);
		stream->_writeUChar(m_assistlv);
		stream->_writeUChar(m_penetratelv);
		stream->_writeUChar(m_manufacturelv);
		stream->_writeUChar(m_gunpowderlv);
		stream->_writeUChar(m_bravelv);
		stream->_writeUChar(m_ironcavalrylv);
		stream->_writeUChar(m_gearlv);
		stream->_writeUChar(m_fireboltlv);
		stream->_writeUChar(m_masterlv);
		stream->_writeUChar(m_juntunlv);
		stream->_writeUChar(m_reformlv);	*/	
		//stream->_writeUChar(m_jingbinglv);
		//stream->_writeUChar(m_jianjialv);
		//stream->_writeUChar(m_qiangrenlv);
		//stream->_writeUChar(m_guimoulv);
		//stream->_writeUChar(m_gongchenglv);
		//stream->_writeUChar(m_yuangonglv);
		//stream->_writeUChar(m_sugonglv);
		//stream->_writeUChar(m_qiangxinglv);
		//stream->_writeUChar(m_zhaomulv);
		//stream->_writeUChar(m_rendelv);
		//stream->_writeUChar(m_signin);
		////FormationInfo
		stream->_writeUChar(m_formationInfoVec.size());
		for( FormationInfoVec::iterator itr = m_formationInfoVec.begin();
			itr != m_formationInfoVec.end();
			++itr )
		{
			stream->_writeInt((*itr)->getRoleID());//SceneServer需要用到
			stream->_writeInt((*itr)->getAbstractID());
			stream->_writeUShort((*itr)->getCount());
			stream->_writeVec2i((*itr)->getEquips());
		}
		
		stream->_writeUInt(m_goldcosthint);
		//好友列表
		stream->_writeUChar(m_friendList.size());
		for( FriendVec::iterator itr = m_friendList.begin();
			itr != m_friendList.end();
			++itr )
		{
			stream->_writeInt((*itr)->getPlayerID());
			stream->_writeString((*itr)->getName());
			stream->_writeUChar((*itr)->getLevel());
			stream->_writeUChar((*itr)->getIconID());
			stream->_writeBool((*itr)->getOnline());
		}
		//stream->_writeShort(m_halltimer);
		//stream->_writeInt(m_privatetimer);
		//for(int i = 0; i<5; i++)
		//{
		//	stream->_writeInt(m_hallCard[i]);
		//}
		//for(int i = 0; i<3; i++)
		//{
		//	stream->_writeInt(m_privateCard[i]);
		//}
		////征税
		stream->_writeUChar(m_levyCount);
		//stream->_writeInt(m_levyTimer);
		////在线奖励
		stream->_writeUChar(m_onlineRewardCount);
		stream->_writeInt(m_onlineRewardTimer);
		////军令
		stream->_writeUChar(m_buyJunlingCount);
		stream->_writeUShort(m_buyJunlingTimer);
		//寄售
		stream->_writeUChar(m_saleList.size());
		for( SaleList::iterator itr = m_saleList.begin();
			 itr != m_saleList.end();
			 ++itr )
		{
			stream->_writeChar((*itr)->getSaleListID());
			stream->_writeInt((*itr)->getGoodsType());
			stream->_writeInt((*itr)->getGoodsID());
			stream->_writeInt((*itr)->getPrice());
			stream->_writeInt((*itr)->getCount());
			stream->_writeShort((*itr)->prepareTimer());
			stream->_writeInt((*itr)->getSaleID());
			stream->_writeInt((*itr)->saleTimer());
			stream->_writeUChar((*itr)->getSaleState());
			stream->_writeInt((*itr)->getGaincash());
		}

		//最近出售列表
		stream->_writeUChar(m_saleitemlist.size());
		for( JXJSaleItemList::iterator itr = m_saleitemlist.begin();
			itr != m_saleitemlist.end();
			++itr )
		{
			if((*itr)->getItemCount()>0 && (*itr)->getItemID()>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itr)->getItemID()),itemrecord)>=0)
			{
				stream->_writeUChar((*itr)->getItemCount());
				stream->_writeUShort((*itr)->getTime());
				stream->_writeInt((*itr)->getItemID());
				itemtype = atoi(itemrecord[itemtypeindex].c_str());
				if(itemtype == IT_Equip)
					stream->_writeInt((*itr)->getEquipMagic());
			}
			else
			{
				stream->_writeUChar(0);
			}
		}
		//将卡背包列表
		unsigned char bagnummax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJiangkaBagSize,m_viplv).c_str()));
		if(m_jiangkabagvec.size()<bagnummax)
			m_jiangkabagvec.resize(bagnummax);
		stream->_writeUChar(bagnummax);
		for( int i = 0; i < bagnummax;i++ )
		{
			if(m_jiangkabagvec[i].valid() && m_jiangkabagvec[i]->getItemCount()>0)
			{
				stream->_writeUChar(m_jiangkabagvec[i]->getItemCount());
				stream->_writeInt(m_jiangkabagvec[i]->getItemID());
				//stream->_writeUChar(LOINT16(LOINT32(m_jiangkabagvec[i]->getEquipMagic())));
			}
			else
			{
				stream->_writeUChar(0);
			}
		}
		//将卡派遣列表
		unsigned char bagnummax2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,m_viplv).c_str()));
		m_appointjiangkabagvec.resize(bagnummax2);
		stream->_writeUChar(m_appointjiangkabagvec.size());
		for( int i = 0; i < bagnummax2;i++ )
		{
			if(m_appointjiangkabagvec[i].valid())
			{
				stream->_writeInt(m_appointjiangkabagvec[i]->getItemID());
				stream->_writeUShort(m_appointjiangkabagvec[i]->getValidTime());
			}
			else
			{
				stream->_writeInt(0);
			}
		}
		//FubenProgressMap
		stream->_writeUChar(m_fubenOrChengchiType);
		stream->_writeUShort(m_fubenOrChengchiID);
		stream->_writeUShort(m_fubenProgressMap.size());
		for( FubenProgressMap::iterator itr = m_fubenProgressMap.begin();
			itr != m_fubenProgressMap.end();
			++itr )
		{
			stream->_writeUShort(itr->first);
			stream->_writeShort(itr->second->getFubenProgress());
			stream->_writeUChar(itr->second->getComplete());
			stream->_writeUChar(itr->second->getStarCount());
		}
		//时间队列
		stream->_writeVec4i(m_buildingTimes);//16
		stream->_writeVec4i(m_recruitTimes);//16
		stream->_writeVec4i(m_ckilleachtroops);//16
		stream->_writeVec4i(m_cdeadeachtroops);//16
		stream->_writeVec4i(m_danchangkill);
		stream->_writeVec4s(m_killdoorboss);
		stream->_writeUShort(m_junling);
		//ExtraProduceMap
		stream->_writeUChar(m_extraProduceMap.size());
		for( ExtraProduceMap::iterator itr = m_extraProduceMap.begin();
			itr != m_extraProduceMap.end();
			++itr )
		{
			stream->_writeUChar(itr->first);
			stream->_writeInt(itr->second->timer());
			stream->_writeShort(itr->second->getPercent());
		}
		//stream->_writeUChar(m_luckycardcoppercost);
		//stream->_writeUChar(m_luckycardgoldcost);
		//stream->_writeUChar(m_knowluckycard);
		//stream->_writeInt(m_jiusirefreshtime);
		stream->_writeInt(m_vipExp);
		stream->_writeInt(m_countrycontribute);
		for (int i = 0; i < 3;i++)
		{
			stream->_writeInt(m_luckycard[i]);
		}
		stream->_writeUShort(m_cardimpression);
		stream->_writeInt(m_cardpieces);
		stream->_writeUChar(m_freeCounterinsurgency);
		stream->_writeUShort(m_RefineEquipFreeCount);
		stream->_writeUChar(m_NongTianShuiLilv);
		stream->_writeUChar(m_ZhiShuZaoLinlv);
		stream->_writeUChar(m_YouZhiCaoChanglv);
		stream->_writeUChar(m_JingKuangShenWalv);
		stream->_writeUChar(m_GuLiMaoYilv);
		stream->_writeUChar(m_ZhengShouXinDelv);
		stream->_writeUChar(m_YanBingYueWulv);
		stream->_writeUChar(m_XunBingXinDelv);
		stream->_writeUChar(m_ZongMiaoJiSilv);
		stream->_writeUChar(m_JiSiXinDelv);
		stream->_writeInt(m_MarketBuyResourceMax);
		stream->_writeUShort(m_FreeRollCardCount);
		stream->_writeUShort(m_autosellcard);
		//军备map（兵种科技）
		stream->_writeUChar(m_troopstechmap.size());
		for(TroopsTechMap::iterator itr = m_troopstechmap.begin();
			itr != m_troopstechmap.end();
			++itr)
		{
			stream->_writeUShort(itr->first);
			stream->_writeUChar(itr->second);
		}
		//组合将卡科技等级
		stream->_writeUChar(m_cardExtraTechMap.size());
		for(CardExtraTechMap::iterator itr = m_cardExtraTechMap.begin();
			itr != m_cardExtraTechMap.end();
			++itr)
		{
			stream->_writeUShort(itr->first);
			stream->_writeUChar(itr->second);
		}
		//玩家在征战商店每日可购买物品上限
		stream->_writeUChar(m_buyshagnxianmap.size());
		for(std::map<int,unsigned char>::iterator itr = m_buyshagnxianmap.begin();
			itr != m_buyshagnxianmap.end();
			++itr)
		{
			stream->_writeInt(itr->first);
			stream->_writeUChar(itr->second);
		}
		//玩家兵种成就相关信息
		stream->_writeUChar(m_troopsachievemap.size());
		for(std::map<int,CRCore::crVector3i>::iterator itr = m_troopsachievemap.begin();
			itr != m_troopsachievemap.end();
			++itr)
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second[0]);
			stream->_writeInt(itr->second[1]);
			stream->_writeInt(itr->second[2]);
		}
		//新版科技
		stream->_writeUChar(m_CardImpression2CritTechLv);
		stream->_writeUChar(m_CardImpression4CritTechLv);
		stream->_writeUChar(m_CardImpression6CritTechLv);
		stream->_writeUChar(m_CardImpression10CritTechLv);
		stream->_writeUChar(m_FreeRollCardTechLv);
		stream->_writeUChar(m_OrangeCardUpgradeTechLv);
		stream->_writeUChar(m_CardShopOpenTechLv);
		stream->_writeUChar(m_MakeEquipTechLv);
		stream->_writeUChar(m_EquipRefineTechLv);
		stream->_writeUChar(m_EquipAttachTechLv);
		stream->_writeUChar(m_SmeltEquipTechLv);
		stream->_writeUChar(m_SmeltMaterialTechLv);
		stream->_writeUChar(m_SmeltStoneTechLv);
		stream->_writeUChar(m_EquipRefineMaxCount1TechLv);
		stream->_writeUChar(m_EquipRefineMaxCount2TechLv);
		stream->_writeUChar(m_EquipRefineMaxCount3TechLv);
		stream->_writeUChar(m_EquipRefineMaxCount4TechLv);
		stream->_writeUChar(m_EquipRefineProduce1TechLv);
		stream->_writeUChar(m_EquipRefineProduce2TechLv);
		stream->_writeUChar(m_EquipRefineProduce3TechLv);
		stream->_writeUChar(m_EquipRefineProduce4TechLv);
		stream->_writeUChar(m_FreeRollCard2TechLv);
		stream->_writeUChar(m_FreeRollCard3TechLv);
		stream->_writeUChar(m_FreeRollCardProduce1TechLv);
		stream->_writeUChar(m_FreeRollCardProduce2TechLv);
		stream->_writeUChar(m_FreeRollCardProduce3TechLv);
		stream->_writeUChar(m_FreeRollCardProduce4TechLv);
		stream->_writeUChar(m_FuKuMaxTechLv);
		////////////////////////////////////
		stream->_writeUChar(m_opensavedformationifo);
		stream->_writeUChar(m_changeShiliTimes);
		stream->_writeUShort(m_seriesLandDays);
		stream->_writeUChar(m_LandRewardGetCount);
		stream->_writeInt64(m_chatChannelSetInfo.to_ullong());

		stream->_writeUChar(m_blacknameset.size());
		//黑名单
		for(BlackNameSet::iterator itr = m_blacknameset.begin();
			itr != m_blacknameset.end();
			++itr)
		{
			stream->_writeString((*itr));
		}

		//研发中的科技map
		stream->_writeUChar(m_techdevelopmap.size());
		for(TechDevelopMap::iterator itr = m_techdevelopmap.begin();
			itr != m_techdevelopmap.end();
			++itr)
		{
			stream->_writeInt(itr->first);
			stream->_writeFloat(itr->second);
		}

		////说服队列
		//int timeOffset = 0;
		//time_t t = time(0);
		//stream->_writeUChar(m_persuadeInfoDeq.size());
		//for(PersuadeInfoDeq::iterator itr = m_persuadeInfoDeq.begin();
		//	itr != m_persuadeInfoDeq.end();
		//	++itr)
		//{
		//	stream->_writeUShort(itr->get()->getFubenId());
		//	stream->_writeInt(itr->get()->getJKId());
		//	timeOffset = itr->get()->getTime() - t;
		//	timeOffset = (timeOffset>0)?timeOffset:0;
 	//		stream->_writeInt(timeOffset);
		//	stream->_writeUChar(itr->get()->getLevel());
		//}
		//////SavedFormationInfo
		stream->_writeUChar(m_savedformationinfovec.size());

		for( SavedFormationInfoVec::iterator itr = m_savedformationinfovec.begin();
			itr != m_savedformationinfovec.end();
			++itr )
		{
			stream->_writeString(itr->first);
			stream->_writeUChar(itr->second.size());
			for( FormationInfoVec::iterator fitr = itr->second.begin();
				fitr != itr->second.end();
				++fitr )
			{
				stream->_writeInt((*fitr)->getRoleID());//SceneServer需要用到
				stream->_writeInt((*fitr)->getAbstractID());
				stream->_writeUShort((*fitr)->getCount());
				stream->_writeVec2i((*fitr)->getEquips());
			}
		}
		////////轮回福利map
		stream->_writeUChar(m_lunhuiRewardMap.size());
		for(LunHuiRewardMap::iterator itr = m_lunhuiRewardMap.begin();
			itr != m_lunhuiRewardMap.end();
			++itr)
		{

			stream->_writeUChar(itr->first);
			stream->_writeUChar(itr->second.first);
			stream->_writeVec3i(itr->second.second);
// 			for (LunHuiRewardVec::iterator itr_vec = itr->second.second.begin();\
// 				itr_vec!=itr->second.second.end();itr_vec++)
// 			{
// 				stream->_writeInt((*itr_vec)[0]);
// 				stream->_writeInt((*itr_vec)[1]);
// 				stream->_writeInt((*itr_vec)[2]);
// 			}
		}
		stream->_writeBool(m_skipnewhand);
		stream->_writeInt(m_consortiaid);
		stream->_writeUChar(m_savedformationindex);
		stream->_writeUChar(m_iftechbtnnew);
		stream->_writeUChar(m_FuKuMaxTech2Lv);
		stream->_writeUChar(m_FuKuMaxTech3Lv);
		////////新功能开启提示
		stream->_writeUChar(m_newfunctionopenset.size());
		for (NewFunctionOpenSet::iterator itr = m_newfunctionopenset.begin();
			itr != m_newfunctionopenset.end();
			++itr)
		{

			stream->_writeUShort(*itr);
		}
		stream->_writeUChar(m_lingqufengluflag);
		stream->_writeUChar(m_tuntianlv);
		stream->_writeUChar(m_yelianlv);
		stream->_writeUChar(m_fanzhilv);
		stream->_writeUChar(m_fanmaolv);
		stream->_writeUChar(m_qiaojianglv);
		stream->_writeUChar(m_zhengshuilv);
		stream->_writeUChar(m_techfenglulv);
		stream->_writeUChar(m_techjiusibaojilv);
		stream->_writeUChar(m_techworldtalklv);
		stream->_writeUChar(m_techbuyjunlinglv);
		stream->_writeUChar(m_foundCountryAward);
		stream->_writeInt(m_playertotalpaygold);
		stream->_writeUChar(m_shuichelv);
		stream->_writeUChar(m_ronglulv);
		stream->_writeUChar(m_gongshelv);
		stream->_writeUChar(m_mochuanlv);
		stream->_writeUShort(m_loginDays);
		stream->_writeUChar(m_7DaysAward);
		stream->_writeUChar(m_shouchong);
		stream->_writeInt(m_cZhengZhanQuan);
		stream->_writeInt(m_killplayercount);
		stream->_writeInt(m_killnpccount);
		stream->_writeInt(m_dayplayerdeadcount);
		stream->_writeInt(m_playerdeadcount);
		stream->_writeInt(m_usedachievepoint);
		stream->_writeUShort(m_passmingjiangcount);
		stream->_writeUShort(m_Cpassworldfuben);
		stream->_writeUChar(m_cmingjiangfail);
		stream->_writeUChar(m_ctianxiafail);
		stream->_writeUChar(m_havegotviplv);
		stream->_writeUShort(m_Cpasstianxia);
		stream->_writeUChar(m_cfinishdays);
		stream->_writeInt(m_accomplishPointCache);
		stream->_writeInt(m_praisedcount);
		stream->_writeInt(m_praisecount);
		stream->_writeInt(m_cGuoZhanJiFen);
		stream->_writeInt(m_treasonbuffertimer);
		stream->_writeInt(m_treasonpunishtimer);
		stream->_writeInt(m_treasoncoolingtimer);
		stream->_writeUChar(m_selectedshili);
		stream->_writeUChar(m_timesshangwu);
		stream->_writeUChar(m_timesxiawu);
		stream->_writeUChar(m_timeswanshang);
		stream->_writeUChar(m_praiselimit);
		stream->_writeShort(m_yuekaid);
		stream->_writeInt64(m_yuekaendtime);
		stream->_writeInt64(m_lastgettime);
		stream->_writeInt64(m_kaoshangfutime);
		stream->_writeUChar(m_specialtyrefreshtimes);
		stream->_writeInt64(m_lastchongzhitime);
		stream->_writeBool(m_hadwonjuntuanfubenT);
		stream->_writeBool(m_huoyuedusign);
		stream->_writeInt(m_gainedzhoukahuodongid);
		stream->_writeInt64(m_zhoukaendtime);
		stream->_writeUChar(m_zhoukagetstatus);
		stream->_writeInt(m_zhoukagetdayidx);
		stream->_writeShort(m_playerCycleCount);
		stream->_writeInt(m_playerOnlineRewardTimer);
		stream->_writeBool(m_recommendSet);
		stream->_writeInt(m_equipBindInfo);
		stream->_writeUChar(m_isYellow);
		stream->_writeUChar(m_isYearYellow);
		stream->_writeUChar(m_yellowLevel);
		stream->_writeInt(m_yellowAwardInfo);
		stream->_writeInt(m_cancelCantonTimer);
		stream->_writeUChar(m_guanzhi);
		stream->_writeUShort(m_cardImpressionLevel);
		stream->_writeUChar(m_resourceCount);
		stream->_writeUChar(m_freeBuy);
		stream->_writeUChar(m_goldRecruitCount);
		stream->_writeUChar(m_zhengZhaoTimes);
		stream->_writeUChar(m_attackVictoryTimes);
		stream->_writeUChar(m_defendVictoryTimes);
		stream->_writeString(m_releaseName);
		stream->_writeUShort(m_countryBattleTimes);
		stream->_writeBool(m_hadGottenReward);
		stream->_writeUChar(m_myRankVal);
		stream->_writeUShort(m_startedDays);
		stream->_writeBool(m_gameIntroductionNoTips);
		stream->_writeBool(m_hadShowFestivalHuodongCanvas);
		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				stream->_writeChar(m_7DayData[i][j]);
			}
		}
		////////成就商店商品购买次数记录
		stream->_writeUChar(m_achieveShopMap.size());
		for (AchieveShopMap::iterator itr = m_achieveShopMap.begin();
			itr != m_achieveShopMap.end();
			++itr)
		{

			stream->_writeUChar(itr->first);
			stream->_writeUChar(itr->second);
		}
		////////道具商店商品购买次数记录
		stream->_writeUChar(m_govShopMap.size());
		for (GovShopMap::iterator itr = m_govShopMap.begin();
			itr != m_govShopMap.end();
			++itr)
		{

			stream->_writeUChar(itr->first);
			stream->_writeUChar(itr->second);
		}
		stream->_writeInt(m_daykillplayercount);
		stream->_writeInt(m_daykillnpccount);
		stream->_writeVec4i(m_dayckilleachtroops);
		stream->_writeBool(m_autoBuilding);
		stream->_writeChar(m_buildCountryTimes);
		stream->_writeVec3i(m_foreigncard);
		stream->seekBegin();
		CRNet::creCompress(stream);
	}
	else if(LOINT64(msg) == WCH_BuildNetStream)
	{
		GNE::LockMutex lock( m_dataMutex );

		//void *param;
		//crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		//crData *data = myPlayer->getDataClass();
		//data->getParam(WCHDATA_JXJVipLv,param);
		//unsigned char viplv = *(unsigned char *)param;
		//int nMaxBuyIconCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxBuyIconCount,m_viplv).c_str());

		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(512);
		///////
		stream->_writeUChar(m_viplv);
		stream->_writeBool(m_playersex);
		stream->_writeUChar(m_playericonid);
		stream->_writeUChar(m_shiliid);
		stream->_writeUShort(m_homeid);
		stream->_writeUShort(m_chengchiid);
		stream->_writeUChar(m_guanzhi);
		//stream->_writeUChar(m_foundCountryAward);
		//stream->_writeUChar(m_7DaysAward);
		//////FormationInfo
		stream->_writeUChar(m_formationInfoVec.size());
		for( FormationInfoVec::iterator itr = m_formationInfoVec.begin();
			itr != m_formationInfoVec.end();
			++itr )
		{
			stream->_writeInt((*itr)->getAbstractID());
			stream->_writeUShort((*itr)->getCount());
			stream->_writeVec2i((*itr)->getEquips());
		}
		////军备map（兵种科技）
		//stream->_writeUChar(m_troopstechmap.size());
		//for(TroopsTechMap::iterator itr = m_troopstechmap.begin();
		//	itr != m_troopstechmap.end();
		//	++itr)
		//{
		//	stream->_writeUShort(itr->first);
		//	stream->_writeUChar(itr->second);
		//}
		//stream->_writeUChar(m_fubenOrChengchiType);
		//stream->_writeUShort(m_fubenOrChengchiID);
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_BuildCreateStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(7);
		///////
		stream->_writeBool(m_playersex);
		stream->_writeUChar(m_playericonid);
		stream->_writeUChar(m_shiliid);
		stream->_writeUShort(m_homeid);
		stream->_writeUShort(m_chengchiid);

		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_BuildUpdateStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(54 + m_extraProduceMap.size() * 7 + m_techdevelopmap.size() * 8);
		///////
		//资源派发
		stream->_writeInt(m_coppercash);
		stream->_writeInt(m_food);
		stream->_writeInt(m_wood);
		stream->_writeInt(m_iron);
		stream->_writeInt(m_horse);
		//时间队列
		stream->_writeVec4i(m_buildingTimes);//16
		stream->_writeVec4i(m_recruitTimes);//16
		//ExtraProduceMap
		stream->_writeUChar(m_extraProduceMap.size());
		for( ExtraProduceMap::iterator itr = m_extraProduceMap.begin();
			itr != m_extraProduceMap.end();
			++itr )
		{
			stream->_writeUChar(itr->first);
			stream->_writeInt(itr->second->timer());
			stream->_writeShort(itr->second->getPercent());
		}
		//TechDevelopMap
		stream->_writeUChar(m_techdevelopmap.size());
		for( TechDevelopMap::iterator itr = m_techdevelopmap.begin();
			itr != m_techdevelopmap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeFloat(itr->second);
		}
		
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_BuildSceneSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		int size = 3+m_formationInfoVec.size()*2;
		stream->createBuf(size);
		///////
		//////FormationInfo
		stream->_writeUChar(m_formationInfoVec.size());
		for( FormationInfoVec::iterator itr = m_formationInfoVec.begin();
			itr != m_formationInfoVec.end();
			++itr )
		{//2
			stream->_writeUShort((*itr)->getCount());
		}
		//stream->_writeShort(m_curFubenProgress);

		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_InitData)
	{
		//if(HIINT64(msg) == ID_LoginGameServer)
		//{
			//m_onlineRewardCount = 0;
			//ref_ptr<crTableIO>onlineReward = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
			//crTableIO::StrVec record;
			//onlineReward->queryOneRecord(0,crArgumentParser::appItoa(m_onlineRewardCount),record);
			//m_onlineRewardTimer = atoi(record[1].c_str());
		//}
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crJXJPlayerData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJ7DayData:
		if (param == NULL)
		{
			for (int i = 0; i < 7; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					m_7DayData[i][j] = 0;
				}
			}
		}
		break;
	case WCHDATA_JXJPlayerID:
		m_playerid = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_PlayerSex:
		m_playersex = param==NULL?NULL:*((bool*)param);
		break;
	case WCHDATA_PlayerIconID:
		m_playericonid = param==NULL?NULL:*((unsigned char*)param);
		break;
	//case WCHDATA_JXJMainRoleBuyIcon:

	//	//m_playerBuyIconList的修改使用读取出数组统一修改的方法，所以这里不需要改

	//	break;
	case WCHDATA_JXJShiliID:
		m_shiliid = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJHomeID:
		m_homeid = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJChengchiID:
		m_chengchiid = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJGoldingot:
		m_goldingot = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJGiftGold:
		m_giftgold = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCoppercash:
		m_coppercash = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJFood:
		m_food = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJWood:
		m_wood = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJEquipBindInfo:
		m_equipBindInfo = param?*((int*)param):NULL;
		break;
	case  WCHDATA_JXJISYELLOW:
		m_isYellow = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJISYEARYELLOW:
		m_isYearYellow = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJYELLOWLEVEL:
		m_yellowLevel = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJYELLOWAWARDINFO:
		m_yellowAwardInfo = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJIron:
		m_iron = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJHorse:
		m_horse = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJFuyalv:
		m_fuyalv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJBingyinglv:
		m_bingyinglv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJJiusilv:
		m_jiusilv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJGongfanglv:
		m_gongfanglv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJShijilv:
		m_shijilv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJFukulv:
		m_fukulv = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJMingjulv:
		m_mingjulv = param==NULL?NULL:*((_crInt64*)param);
		break;
	case WCHDATA_JXJMingjulv1:
	case WCHDATA_JXJMingjulv2:
	case WCHDATA_JXJMingjulv3:
	case WCHDATA_JXJMingjulv4:
	case WCHDATA_JXJMingjulv5:
	case WCHDATA_JXJMingjulv6:
	case WCHDATA_JXJMingjulv7:
	case WCHDATA_JXJMingjulv8:
		{
			_crInt32 lo32 = LOINT64(m_mingjulv);
			_crInt32 hi32 = HIINT64(m_mingjulv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			unsigned char mingju[8];
			mingju[0] = (unsigned char)LOINT16(lo16);
			mingju[1] = (unsigned char)HIINT16(lo16);
			mingju[2] = (unsigned char)LOINT16(hi16);
			mingju[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			mingju[4] = (unsigned char)LOINT16(lo16);
			mingju[5] = (unsigned char)HIINT16(lo16);
			mingju[6] = (unsigned char)LOINT16(hi16);
			mingju[7] = (unsigned char)HIINT16(hi16);
			mingju[i-(WCHDATA_JXJMingjulv1)] = param?*(unsigned char*)param:0;
			m_mingjulv = MAKEINT64(MAKEINT32(MAKEINT16(mingju[0],mingju[1]),MAKEINT16(mingju[2],mingju[3])),MAKEINT32(MAKEINT16(mingju[4],mingju[5]),MAKEINT16(mingju[6],mingju[7])));
		}
		break;
	case WCHDATA_JXJNongtianlv:
		m_nongtianlv = param==NULL?NULL:*((_crInt64*)param);
		break;
	case WCHDATA_JXJNongtianlv1:
	case WCHDATA_JXJNongtianlv2:
	case WCHDATA_JXJNongtianlv3:
	case WCHDATA_JXJNongtianlv4:
	case WCHDATA_JXJNongtianlv5:
	case WCHDATA_JXJNongtianlv6:
	case WCHDATA_JXJNongtianlv7:
	case WCHDATA_JXJNongtianlv8:
		{
			_crInt32 lo32 = LOINT64(m_nongtianlv);
			_crInt32 hi32 = HIINT64(m_nongtianlv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			unsigned char nongtian[8];
			nongtian[0] = (unsigned char)LOINT16(lo16);
			nongtian[1] = (unsigned char)HIINT16(lo16);
			nongtian[2] = (unsigned char)LOINT16(hi16);
			nongtian[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			nongtian[4] = (unsigned char)LOINT16(lo16);
			nongtian[5] = (unsigned char)HIINT16(lo16);
			nongtian[6] = (unsigned char)LOINT16(hi16);
			nongtian[7] = (unsigned char)HIINT16(hi16);
			nongtian[i-(WCHDATA_JXJNongtianlv1)] = param?*(unsigned char*)param:0;
			m_nongtianlv = MAKEINT64(MAKEINT32(MAKEINT16(nongtian[0],nongtian[1]),MAKEINT16(nongtian[2],nongtian[3])),MAKEINT32(MAKEINT16(nongtian[4],nongtian[5]),MAKEINT16(nongtian[6],nongtian[7])));
		}
		break;
	case WCHDATA_JXJMuchanglv:
		m_muchanglv = param==NULL?NULL:*((_crInt32*)param);
		break;
	case WCHDATA_JXJMuchanglv1:
	case WCHDATA_JXJMuchanglv2:
	case WCHDATA_JXJMuchanglv3:
	case WCHDATA_JXJMuchanglv4:
		{
			_crInt16 lo16 = LOINT32(m_muchanglv);
			_crInt16 hi16 = HIINT32(m_muchanglv);
			unsigned char muchang[4];
			muchang[0] = (unsigned char)LOINT16(lo16);
			muchang[1] = (unsigned char)HIINT16(lo16);
			muchang[2] = (unsigned char)LOINT16(hi16);
			muchang[3] = (unsigned char)HIINT16(hi16);
			muchang[i-(WCHDATA_JXJMuchanglv1)] = param?*(unsigned char*)param:0;
			m_muchanglv = MAKEINT32(MAKEINT16(muchang[0],muchang[1]),MAKEINT16(muchang[2],muchang[3]));
		}
		break;
	case WCHDATA_JXJLingdilv:
		m_lingdilv = param==NULL?NULL:*((_crInt32*)param);
		break;
	case WCHDATA_JXJLingdilv1:
	case WCHDATA_JXJLingdilv2:
	case WCHDATA_JXJLingdilv3:
	case WCHDATA_JXJLingdilv4:
		{
			_crInt16 lo16 = LOINT32(m_lingdilv);
			_crInt16 hi16 = HIINT32(m_lingdilv);
			unsigned char lingdi[4];
			lingdi[0] = (unsigned char)LOINT16(lo16);
			lingdi[1] = (unsigned char)HIINT16(lo16);
			lingdi[2] = (unsigned char)LOINT16(hi16);
			lingdi[3] = (unsigned char)HIINT16(hi16);
			lingdi[i-(WCHDATA_JXJLingdilv1)] = param?*(unsigned char*)param:0;
			m_lingdilv = MAKEINT32(MAKEINT16(lingdi[0],lingdi[1]),MAKEINT16(lingdi[2],lingdi[3]));
		}
		break;
	case WCHDATA_JXJKuangshanlv:
		m_kuangshanlv = param==NULL?NULL:*((_crInt32*)param);
		break;
	case WCHDATA_JXJKuangshanlv1:
	case WCHDATA_JXJKuangshanlv2:
	case WCHDATA_JXJKuangshanlv3:
	case WCHDATA_JXJKuangshanlv4:
		{
			_crInt16 lo16 = LOINT32(m_kuangshanlv);
			_crInt16 hi16 = HIINT32(m_kuangshanlv);
			unsigned char kuangshan[4];
			kuangshan[0] = (unsigned char)LOINT16(lo16);
			kuangshan[1] = (unsigned char)HIINT16(lo16);
			kuangshan[2] = (unsigned char)LOINT16(hi16);
			kuangshan[3] = (unsigned char)HIINT16(hi16);
			kuangshan[i-(WCHDATA_JXJKuangshanlv1)] = param?*(unsigned char*)param:0;
			m_kuangshanlv = MAKEINT32(MAKEINT16(kuangshan[0],kuangshan[1]),MAKEINT16(kuangshan[2],kuangshan[3]));
		}
		break;
	case WCHDATA_UpdateTimer:
		m_updateTimer = param==NULL?NULL:*((_crInt64*)param);
		break;
	case WCHDATA_JXJBuildingTimes:
		m_buildingTimes = param==NULL?crVector4i(0,0,0,0):*((crVector4i*)param);
		break;
	case WCHDATA_JXJBuildingTime0:
		m_buildingTimes[0] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJBuildingTime1:
		m_buildingTimes[1] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJBuildingTime2:
		m_buildingTimes[2] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJBuildingTime3:
		m_buildingTimes[3] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJRecruitTimes:
		m_recruitTimes = param==NULL?crVector4i(0,0,0,0):*((crVector4i*)param);
		break;
	case WCHDATA_JXJRecruitTime0:
		m_recruitTimes[0] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJRecruitTime1:
		m_recruitTimes[1] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJRecruitTime2:
		m_recruitTimes[2] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJRecruitTime3:
		m_recruitTimes[3] = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJKillEachTroopsCount:
		m_ckilleachtroops = param==NULL?crVector4i(0,0,0,0):*((crVector4i*)param);
		break;
	case WCHDATA_JXJDeadEachTroopsCount:
		m_cdeadeachtroops = param==NULL?crVector4i(0,0,0,0):*((crVector4i*)param);
		break;
	case WCHDATA_JXJDanDhangKillCount:
		m_danchangkill = param==NULL?crVector4i(0,0,0,0):*((crVector4i*)param);
		break;
	case WCHDATA_JXJKillDoorBossCount:
		m_killdoorboss = param==NULL?crVector4s(0,0,0,0):*((crVector4s*)param);
		break;
	case WCHDATA_JXJJunling:
		m_junling = param==NULL?NULL:*((unsigned short*)param);
		break;
	//case WCHDATA_JXJMaxTroops:
	//	m_maxtroops = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	//case WCHDATA_JXJSignin:
	//	m_signin = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	case WCHDATA_JXJGoldCostHintInt:
		m_goldcosthint = param==NULL?NULL:*((unsigned int*)param);
		break;
	//case WCHDATA_JXJQueryTeamIndex:
	//	m_queryTeamIndex = param?*((char*)param):NULL;
	//	break;
	//case WCHDATA_JXJHalltimer:
	//	m_halltimer = param?*((short*)param):NULL;
	//	break;
	//case WCHDATA_JXJPrivatetimer:
	//	m_privatetimer = param?*((int*)param):NULL;
	//	break;
	case WCHDATA_JXJLevyCount:
		m_levyCount = param?*((unsigned char*)param):NULL;
		break;
	//case WCHDATA_JXJLevyTimer:
	//	m_levyTimer = param?*((int*)param):NULL;
	//	break;
	case WCHDATA_JXJOnlineRewardCount:
		m_onlineRewardCount = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJOnlineRewardTimer:
		m_onlineRewardTimer = param==NULL?NULL:*((_crInt64*)param);
		break;
	case WCHDATA_JXJPlayerOnlineRewardTimer:
		m_playerOnlineRewardTimer = param?*(int *)param:0;
		break;
	//case WCHDATA_JXJClientTimer:
	//	m_clientTimer = param?*(int *)param:0;
	//	break;
	case WCHDATA_JXJBuyJunlingCount:
		m_buyJunlingCount = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJBuyJunlingTimer:
		m_buyJunlingTimer = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJOpenBattle:
		m_openBattle = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJBattleTimer:
		m_battleTimer = param?*((int*)param):NULL;
		break;
	//case WCHDATA_JXJOpenPass:
	//	m_openPass = param?*((bool*)param):NULL;
	//	break;
	//case WCHDATA_JXJPassTimer:
	//	m_passTimer = param?*((short*)param):NULL;
	//	break;
	case WCHDATA_JXJInviteType:
		m_inviteType = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJInviteID:
		m_inviteID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJInviteChengChiID:
		m_inviteChengchiID = param?*(unsigned short *)param:NULL;
		break;
	case WCHDATA_JXJFubenOrChengchiType:
		m_fubenOrChengchiType = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFubenOrChengchiID:
		m_fubenOrChengchiID = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJOpenBagNum:
		m_openbagnum = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJTeamLeaderID:
		m_teamLeaderID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRoomID:
		m_battleRoomID = param?*((_crInt64*)param):NULL;
		//m_battleChengchiID = HIINT32(LOINT64(m_battleRoomID));
		break;
	case WCHDATA_JXJCurFubenProgress:
		m_curFubenProgress = param?*((short*)param):NULL;
		break;
	//case WCHDATA_JXJLuckyCardCopperCost:
	//	m_luckycardcoppercost = param?*((unsigned char*)param):NULL;
	//	break;
	//case WCHDATA_JXJLuckyCardGoldCost:
	//	m_luckycardgoldcost = param?*((unsigned char*)param):NULL;
	//	break;
	case WCHDATA_JXJLuckyCard:
		if(param == NULL)
		{
			for (int i = 0;i<3;i++)
			{
				m_luckycard[i] = 0;
			}
		}
		break;
	//case WCHDATA_JXJKnowLuckyCard:
	//	m_knowluckycard = param?*((unsigned char*)param):NULL;
	//	break;
	//case WCHDATA_JXJJiusiRefreshCostTime:
	//	m_jiusirefreshtime = param?*((int*)param):NULL;
	//	break;
	case WCHDATA_JXJVipLv:
		m_viplv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVipExp:
		m_vipExp = param?*(int *)param:NULL;
		break;
	case WCHDATA_JXJCountryContribute:
		m_countrycontribute = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCardImpression:
		m_cardimpression = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJCardImpressionLevel:
		m_cardImpressionLevel = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJBuyResourceCount:
		m_resourceCount = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeBuyInfo:
		m_freeBuy = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJGoldRecruitCount:
		m_goldRecruitCount = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJCardPieces:
		m_cardpieces = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJVIPNongTianShuiLi:
		m_NongTianShuiLilv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPZhiShuZaoLin:
		m_ZhiShuZaoLinlv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPYouZhiCaoChang:
		m_YouZhiCaoChanglv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPJingKuangShenWa:
		m_JingKuangShenWalv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPGuLiMaoYi:
		m_GuLiMaoYilv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPZhengShouXinDe:
		m_ZhengShouXinDelv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPYanBingYueWu:
		m_YanBingYueWulv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPXunBingXinDe:
		m_XunBingXinDelv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPZongMiaoJiSi:
		m_ZongMiaoJiSilv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJVIPJiSiXinDe:
		m_JiSiXinDelv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_MarketBuyResourceCount:
		m_MarketBuyResourceMax = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJRefineMagicID:
		m_refinemagicid = param?*((_crInt32*)param):NULL;
		break;
	case WCHDATA_JXJCenterNotice:
		m_cfgstream = param?(crStreamBuf*)param:NULL;
		break;
	case WCHDATA_JXJOldCenterNotice:
		m_oldcfgstream = param ? (crStreamBuf*)param : NULL;
		break;
	case WCHDATA_JXJAutoSellCard:
		m_autosellcard = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJCardImpression2CritTechLv:
		m_CardImpression2CritTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJCardImpression4CritTechLv:
		m_CardImpression4CritTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJCardImpression6CritTechLv:
		m_CardImpression6CritTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJCardImpression10CritTechLv:
		m_CardImpression10CritTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCardTechLv:
		m_FreeRollCardTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJOrangeCardUpgradeTechLv:
		m_OrangeCardUpgradeTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJCardShopOpenTechLv:
		m_CardShopOpenTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJMakeEquipTechLv:
		m_MakeEquipTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJEquipRefineTechLv:
		m_EquipRefineTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJEquipAttachTechLv:
		m_EquipAttachTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJSmeltEquipTechLv:
		m_SmeltEquipTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJSmeltMaterialTechLv:
		m_SmeltMaterialTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJSmeltStoneTechLv:
		m_SmeltStoneTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeCount1TechLv:
		m_EquipRefineMaxCount1TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeCount2TechLv:
		m_EquipRefineMaxCount2TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeCount3TechLv:
		m_EquipRefineMaxCount3TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeCount4TechLv:
		m_EquipRefineMaxCount4TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeProduce1TechLv:
		m_EquipRefineProduce1TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeProduce2TechLv:
		m_EquipRefineProduce2TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeProduce3TechLv:
		m_EquipRefineProduce3TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJAddRefineFreeProduce4TechLv:
		m_EquipRefineProduce4TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCard2TechLv:
		m_FreeRollCard2TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCard3TechLv:
		m_FreeRollCard3TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCardProduce1TechLv:
		m_FreeRollCardProduce1TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCardProduce2TechLv:
		m_FreeRollCardProduce2TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCardProduce3TechLv:
		m_FreeRollCardProduce3TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFreeRollCardProduce4TechLv:
		m_FreeRollCardProduce4TechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFuKuMaxTechLv:
		m_FuKuMaxTechLv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJOpenSaveFormationInfo:
		m_opensavedformationifo = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJPlayerChangeShiliTimes:
		m_changeShiliTimes = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJSeriesLandDay:
		m_seriesLandDays = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJLandRewardGetCount:
		m_LandRewardGetCount = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJLandRewardDataInfo:
		m_lanRewardData = param?(crPlayerRewardData *)param:NULL;
		break;
	case WCHDATA_JXJCountryBenefitFeteTimes		:
		m_feteTimes = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJCountryBenefitEnrichType			:
		m_enrichType = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJCountryBenefitEnrichTimes:
		m_enrichTimes = param?*((unsigned char *)param):NULL;
		break;
	case WCHDATA_JXJSkipNewHand:
		m_skipnewhand = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJSavedFormationIndex:
		m_savedformationindex = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFuKuMaxTech2Lv:
		m_FuKuMaxTech2Lv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFuKuMaxTech3Lv:
		m_FuKuMaxTech3Lv = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJChatChannelSetInfo:
		m_chatChannelSetInfo = param?*(BitSet64 *)param:NULL;
		break;
	case WCHDATA_JXJTechFoodlv:
		m_tuntianlv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechHorselv:
		m_fanzhilv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechIronlv:
		m_yelianlv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechWoodlv:
		m_fanmaolv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechBuildingCDlv:
		m_qiaojianglv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechZhengShuilv:
		m_zhengshuilv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechFengluLv:
		m_techfenglulv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechJiusibaojiLv:
		m_techjiusibaojilv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechWorldTalkPriceLv:
		m_techworldtalklv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJTechBuyJunlingLv:
		m_techbuyjunlinglv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJShuichelv:
		m_shuichelv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJRonglulv:
		m_ronglulv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJGongshelv:
		m_gongshelv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_JXJMochuanlv:
		m_mochuanlv = param ? *((unsigned char*)param) : NULL;
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;

			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJPlayerData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			int version = 0;
			cfg_script.Get("VERSION",version);
			std::string str;
			std::vector<float> v_i;
			std::vector<std::string> v_str;
			int int1;
			int nParam = 1;
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}
			//if(version<JXJVERSION_0012)
			//{//
			//	//m_playerid
			//	crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
			//	serverData->getParam(WCHDATA_JXJAccomplishRankingMap,param);
			//	AccomplishRankingMap *accomplishRankingMap = (AccomplishRankingMap *)param;
			//	AccomplishRankingMap::iterator mapiter = accomplishRankingMap->begin();
			//	unsigned short accomplishpoint = 0;
			//	for (; mapiter != accomplishRankingMap->end(); mapiter++)
			//	{
			//		if(mapiter->second->getPlayerID() == m_playerid)
			//		{
			//			accomplishpoint = mapiter->first;
			//			break;
			//		}
			//	}
			//	m_accomplishPointCache = m_accomplishPointCache+accomplishpoint-m_usedachievepoint;
			//}
			////////////////
			//预备部队
			if(cfg_script.Push("TroopsMap"))
			{
				m_troopsMap.clear();
				nParam = 1;
				if(version<JXJVERSION_0001)
				{
					while (cfg_script.Get("Param", v_i,nParam++))
					{
						m_troopsMap.insert(std::make_pair((int)(v_i[0]),(int)(v_i[1])));
					}
				}
				else
				{
					while (cfg_script.Get("Param", v_str,nParam++))
					{
						m_troopsMap.insert(std::make_pair(atoi(v_str[0].c_str()),atoi(v_str[1].c_str())));
					}
				}
				cfg_script.Pop();
			}
			if(cfg_script.Push("TroopsEquipMap"))
			{
				m_troopsEquipMap.clear();
				nParam = 1;
				if(version<JXJVERSION_0001)
				{
					while (cfg_script.Get("Param", v_i,nParam++))
					{
						m_troopsEquipMap.insert(std::make_pair((unsigned short)(v_i[0]),(int)(v_i[1])));
					}
				}
				else
				{
					while (cfg_script.Get("Param", v_str,nParam++))
					{
						m_troopsEquipMap.insert(std::make_pair(atoi(v_str[0].c_str()),atoi(v_str[1].c_str())));
					}
				}
				cfg_script.Pop();
			}
			//背包列表
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int itemtypeindex= itemtab->getTitleIndex("类型");
			int itemtype;
			crTableIO::StrVec itemrecord;
			//m_itembagvec.clear();
			//int bagnummax = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax).c_str()));
			m_itembagvec.resize(m_openbagnum);
			if(cfg_script.Push("JXJItemBagVec"))
			{
				int nBagVec = 1;
				ref_ptr<crBagItemData> bag;
				if(version<JXJVERSION_0001)
				{
					while(cfg_script.Get("Param", v_i,nBagVec++))
					{
						if((int)v_i[0] != 0 && (int)v_i[1]>0)
						{
							if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[0]),itemrecord)>=0)
							{
								itemtype = atoi(itemrecord[itemtypeindex].c_str());
								bag = new crBagItemData;
								bag->setItemID((int)v_i[0]);
								bag->setItemCount((unsigned char)v_i[1]);
								if(itemtype == IT_Equip && v_i.size()>=3)
									bag->setEquipMagic((int)v_i[2]);//装备附魔
								m_itembagvec[nBagVec-2] = bag;
							}
						}
					}
				}
				else
				{
					int itemid,itemcount;
					while(cfg_script.Get("Param", v_str,nBagVec++))
					{
						itemid = atoi(v_str[0].c_str());
						itemcount = atoi(v_str[1].c_str());
						if(itemid != 0 && itemcount>0 && nBagVec-2 < m_openbagnum)
						{
							if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
							{
								itemtype = atoi(itemrecord[itemtypeindex].c_str());
								bag = new crBagItemData;
								bag->setItemID(itemid);
								bag->setItemCount(itemcount);
								if(itemtype == IT_Equip && v_str.size()>=3)
									bag->setEquipMagic(atoi(v_str[2].c_str()));//装备附魔
								m_itembagvec[nBagVec-2] = bag;
							}
						}
					}
				}
				cfg_script.Pop();
			}
			////////////////
			//FormationInfo
			//if(cfg_script.Push("FormationInfo"))
			//{
			//	m_formationInfoVec.clear();
			//	nParam = 1;
			//	ref_ptr<crFormationInfo> fi;
			//	if(version<JXJVERSION_0001)
			//	{
			//		while (cfg_script.Get("Param", v_i,nParam++))
			//		{
			//			fi = new crFormationInfo;
			//			fi->setAbstractID((int)(v_i[0]));
			//			fi->setCount((unsigned short)(v_i[1]));
			//			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[2]),itemrecord)<0)
			//				v_i[2] = 0;
			//			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[3]),itemrecord)<0)
			//				v_i[3] = 0;
			//			fi->setEquips(crVector2i((int)(v_i[2]),(int)(v_i[3])));
			//			m_formationInfoVec.push_back(fi);
			//		}
			//	}
			//	else
			//	{
			//		crVector2i equip;
			//		while (cfg_script.Get("Param", v_str,nParam++))
			//		{
			//			fi = new crFormationInfo;
			//			fi->setAbstractID(atoi(v_str[0].c_str()));
			//			fi->setCount(atoi(v_str[1].c_str()));
			//			if (itemtab->queryOneRecord(0,v_str[2],itemrecord)>=0)
			//				equip[0] = atoi(v_str[2].c_str());
			//			else
			//			{
			//				equip[0] = 0;
			//			}
			//			if (itemtab->queryOneRecord(0,v_str[3],itemrecord)>=0)
			//				equip[1] = atoi(v_str[3].c_str());
			//			else
			//			{
			//				equip[1] = 0;
			//			}
			//			fi->setEquips(equip);
			//			m_formationInfoVec.push_back(fi);
			//		}
			//	}
			//	cfg_script.Pop();
			//}
			//好友列表
			m_friendList.clear();
			int nFriend = 1;
			ref_ptr<crFriend> fd;
			while(cfg_script.Push("FriendList",nFriend++))
			{
				fd = new crFriend;
				cfg_script.Get("Param", int1,1);
				fd->setPlayerID(int1);
				cfg_script.Get("Param", str,2);
				fd->setName(str);
				cfg_script.Get("Param", int1,3);
				fd->setLevel((unsigned char)int1);
				cfg_script.Get("Param", int1,4);
				fd->setIconID((unsigned char)int1);

				m_friendList.push_back(fd);
				cfg_script.Pop();
			}
			//if(cfg_script.Push("HallCard"))
			//{
			//	cfg_script.Get("Param", v_i);
			//	for (int i = 0; i<5; i++)
			//	{
			//		m_hallCard[i] = v_i[i];
			//	}
			//	cfg_script.Pop();
			//}
			//if(cfg_script.Push("PrivateCard"))
			//{
			//	cfg_script.Get("Param", v_i);
			//	for (int i = 0; i<3; i++)
			//	{
			//		m_privateCard[i] = v_i[i];
			//	}
			//	cfg_script.Pop();
			//}
			//寄售
			m_saleList.clear();
			int nSale = 1;
			ref_ptr<crSale> sale;
			while(cfg_script.Push("SaleList",nSale++))
			{
				sale = new crSale;
				cfg_script.Get("Param", int1,1);
				sale->setSaleListID(int1);
				cfg_script.Get("Param", int1,2);
				sale->setGoodsType(int1);
				cfg_script.Get("Param", int1,3);
				sale->setGoodsID(int1);
				cfg_script.Get("Param", int1,4);
				sale->setPrice(int1);
				cfg_script.Get("Param", int1,5);
				sale->setCount(int1);
				cfg_script.Get("Param", int1,6);
				sale->prepareTimer() = int1;
				cfg_script.Get("Param", int1,7);
				sale->setSaleID(int1);
				cfg_script.Get("Param", int1,8);
				sale->saleTimer() = int1;
				cfg_script.Get("Param", int1,9);
				sale->setSaleState((unsigned char)int1);
				cfg_script.Get("Param", int1,10);
				sale->setGaincash(int1);
				m_saleList.push_back(sale);
				cfg_script.Pop();
			}
			//出售列表
			m_saleitemlist.clear();

			if(cfg_script.Push("SaleItemDeq"))
			{
				int nSaleitem = 1;
				ref_ptr<crSaleItemData> saleitem;
				if(version<JXJVERSION_0001)
				{
					while(cfg_script.Get("Param", v_i,nSaleitem++))
					{
						saleitem = new crSaleItemData;
						saleitem->setItemID((int)v_i[0]);
						saleitem->setItemCount((unsigned char)v_i[1]);
						saleitem->setTime((short)v_i[2]);

						m_saleitemlist.push_back(saleitem);
					}
				}
				else
				{
					int itemid,itemcount;
					while(cfg_script.Get("Param", v_str,nSaleitem++))
					{
						itemid = atoi(v_str[0].c_str());
						itemcount = atoi(v_str[1].c_str());
						if(itemid != 0 && itemcount>0)
						{
							if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
							{
								itemtype = atoi(itemrecord[itemtypeindex].c_str());
								saleitem = new crSaleItemData;
								saleitem->setItemID(itemid);
								saleitem->setItemCount(itemcount);
								saleitem->setTime(atoi(v_str[2].c_str()));
								if(itemtype == IT_Equip && v_str.size()>=4)
									saleitem->setEquipMagic(atoi(v_str[3].c_str()));//装备附魔
								m_saleitemlist.push_back(saleitem);
							}
						}
					}
				}
				cfg_script.Pop();
			}
			//将卡背包列表
			//m_itembagvec.clear();
			unsigned char bagmaxnum = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJJiangkaBagSize,m_viplv).c_str()));
			m_jiangkabagvec.resize(bagmaxnum);
			if(cfg_script.Push("JXJJiangkaItemBagVec"))
			{
				int nJkBagVec = 1;
				ref_ptr<crBagItemData> jkbag;
				if(version<JXJVERSION_0001)
				{
					while(cfg_script.Get("Param", v_i,nJkBagVec++))
					{
						if((int)v_i[0] != 0 && (int)v_i[1]>0 && nJkBagVec-2<bagmaxnum)
						{
							if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[0]),itemrecord)>=0)
							{
								jkbag = new crBagItemData;
								jkbag->setItemID((int)v_i[0]);
								jkbag->setItemCount((unsigned char)v_i[1]);
								//jkbag->setEquipMagic((int)v_i[2]);
								m_jiangkabagvec[nJkBagVec-2] = jkbag;
							}
						}
					}
				}
				else
				{
					int itemid,itemcount;
					while(cfg_script.Get("Param", v_str,nJkBagVec++))
					{
						itemid = atoi(v_str[0].c_str());
						itemcount = atoi(v_str[1].c_str());
						if(itemid != 0 && itemcount>0 && nJkBagVec-2<bagmaxnum)
						{
							if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
							{
								jkbag = new crBagItemData;
								jkbag->setItemID(itemid);
								jkbag->setItemCount(itemcount);
								m_jiangkabagvec[nJkBagVec-2] = jkbag;
							}
						}
					}
				}
				cfg_script.Pop();
			}
			//将卡派遣列表
			//m_itembagvec.clear();
			unsigned char bagmaxnum2 = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAppointCardMaxNum,m_viplv).c_str()));
			m_appointjiangkabagvec.resize(bagmaxnum2);
			if(cfg_script.Push("JXJAppointJiangkaItemBagVec"))
			{
				int nJkBagVec = 1;
				ref_ptr<crAppointCardData> jkbag2;
				if(version<JXJVERSION_0001)
				{
					while(cfg_script.Get("Param", v_i,nJkBagVec++))
					{
						if((int)v_i[0] != 0)
						{
							if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[0]),itemrecord)>=0)
							{
								jkbag2 = new crAppointCardData;
								jkbag2->setItemID((int)v_i[0]);
								jkbag2->setValidTime((unsigned short)v_i[1]);
								m_appointjiangkabagvec[nJkBagVec-2] = jkbag2;
							}
						}
					}
				}
				else
				{
					int itemid;
					while(cfg_script.Get("Param", v_str,nJkBagVec++))
					{
						itemid = atoi(v_str[0].c_str());
						if(itemid != 0 && nJkBagVec-2 < bagmaxnum2)
						{
							if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
							{
								jkbag2 = new crAppointCardData;
								jkbag2->setItemID(itemid);
								jkbag2->setValidTime(atoi(v_str[1].c_str()));
								m_appointjiangkabagvec[nJkBagVec-2] = jkbag2;
							}
						}
					}
				}
				cfg_script.Pop();
			}
			//
			///////FubenProgressMap
			if(cfg_script.Push("FubenProgressMap"))
			{
				m_fubenProgressMap.clear();
				nParam = 1;
				ref_ptr<crFubenInfo> fubeninfo;
				if(version<JXJVERSION_0001)
				{
					while (cfg_script.Get("Param", v_i,nParam++))
					{
						if(v_i.size()>=4)
						{
							fubeninfo = new crFubenInfo;
							fubeninfo->setFubenProgress((short)(v_i[1]));
							fubeninfo->setComplete((unsigned char)(v_i[2]));
							fubeninfo->setStarCount((unsigned char)(v_i[3]));
							m_fubenProgressMap.insert(std::make_pair((unsigned short)(v_i[0]),fubeninfo));
						}
					}
				}
				else
				{
					while (cfg_script.Get("Param", v_str,nParam++))
					{
						if(v_str.size()>=4)
						{
							fubeninfo = new crFubenInfo;
							fubeninfo->setFubenProgress(atoi(v_str[1].c_str()));
							fubeninfo->setComplete(atoi(v_str[2].c_str()));
							fubeninfo->setStarCount(atoi(v_str[3].c_str()));
							m_fubenProgressMap.insert(std::make_pair(atoi(v_str[0].c_str()),fubeninfo));
						}
					}
				}
				cfg_script.Pop();
			}
			///////ExtraProduceMap
			if(cfg_script.Push("ExtraProduceMap"))
			{
				m_extraProduceMap.clear();
				nParam = 1;
				ref_ptr<crExtraProduce> extraproduce;
				if(version<JXJVERSION_0001)
				{
					while (cfg_script.Get("Param", v_i,nParam++))
					{
						if(v_i.size()>=3)
						{
							extraproduce = new crExtraProduce;
							extraproduce->timer() = (int)(v_i[1]);
							extraproduce->setPercent((short)(v_i[2]));
							m_extraProduceMap[(unsigned char)(v_i[0])]=extraproduce;
						}
					}
				}
				else
				{
					while (cfg_script.Get("Param", v_str,nParam++))
					{
						if(v_str.size()>=3)
						{
							extraproduce = new crExtraProduce;
							extraproduce->timer() = atoi(v_str[1].c_str());
							extraproduce->setPercent(atoi(v_str[2].c_str()));
							m_extraProduceMap[atoi(v_str[0].c_str())]=extraproduce;
						}
					}
				}
				cfg_script.Pop();
			}
			////7DayData
			if (cfg_script.Push("7DayData"))
			{
				nParam = 1;
				std::string str7Day;
				for (int i = 0; i < 7; ++i)
				{
					for (int j = 0; j < 5; ++j)
					{
						cfg_script.Get("Param",str7Day,nParam++);
						m_7DayData[i][j] = atoi(str7Day.c_str());
					}
				}
				cfg_script.Pop();
			}
			///////LuckyCard
			if(cfg_script.Push("LuckyCard"))
			{
				if(version<JXJVERSION_0001)
				{
					for (int i = 0; i < 3; i++)
					{
						m_luckycard[i] = 0;
					}
					nParam = 1;
					while (cfg_script.Get("Param", v_i,nParam++))
					{
						if(v_i.size()>=3)
						{
							for (int i = 0; i < 3; i++)
							{
								if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[i]),itemrecord)>=0)
									m_luckycard[i] = (int)(v_i[i]);
							}
						}
					}
				}
				else
				{
					nParam = 1;
					cfg_script.Get("Param",m_luckycard[0],nParam++);
					cfg_script.Get("Param",m_luckycard[1],nParam++);
					cfg_script.Get("Param",m_luckycard[2],nParam++);
				}
				cfg_script.Pop();
			}
			//军备map（兵种科技）
			if(cfg_script.Push("JXJTroopsTechMap"))
			{
				nParam = 1;
				//unsigned short id;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					m_troopstechmap.insert(std::make_pair((unsigned short)atoi(v_str[0].c_str()),(unsigned char)atoi(v_str[1].c_str())));
				}
			
				cfg_script.Pop();
			}
			//组合将卡科技等级
			if(cfg_script.Push("CardExtraTechMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					m_cardExtraTechMap.insert(std::make_pair((unsigned short)atoi(v_str[0].c_str()),(unsigned char)atoi(v_str[1].c_str())));
				}

				cfg_script.Pop();
			}
			//玩家在征战商店每日可购买物品上限
			if(cfg_script.Push("JXJBuyShangXianMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					if((int)atoi(v_str[0].c_str()) != 0)
					{
						m_buyshagnxianmap.insert(std::make_pair((int)atoi(v_str[0].c_str()),(unsigned char)atoi(v_str[1].c_str())));
					}
				}

				cfg_script.Pop();
			}
			//玩家兵种成就相关信息
			if(cfg_script.Push("JXJTroopsAchieveMap"))
			{
				CRCore::crVector3i countinfo;
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					if((int)atoi(v_str[0].c_str()) != 0)
					{
						countinfo[0] = atoi(v_str[1].c_str());
						countinfo[1] = atoi(v_str[2].c_str());
						countinfo[2] = atoi(v_str[3].c_str());
						m_troopsachievemap.insert(std::make_pair((int)atoi(v_str[0].c_str()),countinfo));
					}
				}

				cfg_script.Pop();
			}
			//黑名单
			if(cfg_script.Push("JXJBlackNameSet"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					if(!v_str[0].empty())
					{
						m_blacknameset.insert(v_str[0].c_str());
					}
				}

				cfg_script.Pop();
			}
			//研发中科技map
			if(cfg_script.Push("JXJTechDevelopMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					if(atoi(v_str[0].c_str()) != 0)
					{
						m_techdevelopmap.insert(std::make_pair((int)atoi(v_str[0].c_str()),(int)atoi(v_str[1].c_str())));
					}
				}

				cfg_script.Pop();
			}
			////说服列表
			//CRCore::ref_ptr<crJXJPersuadeInfo> jxjPersuadeInfo;
			//if(cfg_script.Push("JXJPersuadeDeq"))
			//{
			//	nParam = 1;
			//	while(cfg_script.Get("Param", v_str,nParam++))
			//	{
			//		if(atoi(v_str[0].c_str()) != 0)
			//		{
			//			jxjPersuadeInfo = new crJXJPersuadeInfo;
			//			jxjPersuadeInfo->setFubenId((unsigned short)atoi(v_str[0].c_str()));
			//			jxjPersuadeInfo->setJKId(atoi(v_str[1].c_str()));
			//			jxjPersuadeInfo->setTime(crArgumentParser::appAtoI64(v_str[2].c_str()));
			//			jxjPersuadeInfo->setLevel((unsigned char)atoi(v_str[3].c_str()));
			//			m_persuadeInfoDeq.push_back(jxjPersuadeInfo);
			//		}
			//	}

			//	cfg_script.Pop();
			//}
			////////////////
			//SavedFormationInfo
			unsigned char formationmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxFormation,m_viplv).c_str()));
			unsigned char savedformationmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,m_viplv).c_str()));

			m_savedformationinfovec.clear();
			m_savedformationinfovec.resize(m_opensavedformationifo);
			if(cfg_script.Push("SavedFormationInfo"))
			{
				nParam = 1;
				ref_ptr<crFormationInfo> fi;
				FormationInfoVec tformationifovec;
				crVector2i equip;
				std::string formationname;
				for (int i = 0; i < m_savedformationinfovec.size(); ++i)
				{
					formationname.clear();
					tformationifovec.clear();
					//获取编队名称
					if (cfg_script.Get("Param", str,nParam++))
						formationname = str.c_str();
					//获取编队信息
					for (int j = 0; j < formationmax; ++j)
					{
						if (cfg_script.Get("Param", v_str,nParam++))
						{
							fi = new crFormationInfo;
							fi->setAbstractID(atoi(v_str[0].c_str()));
							fi->setCount(atoi(v_str[1].c_str()));
							if (itemtab->queryOneRecord(0,v_str[2],itemrecord)>=0)
								equip[0] = atoi(v_str[2].c_str());
							else
							{
								equip[0] = 0;
							}
							if (itemtab->queryOneRecord(0,v_str[3],itemrecord)>=0)
								equip[1] = atoi(v_str[3].c_str());
							else
							{ 
								equip[1] = 0;
							}
							fi->setEquips(equip);
							tformationifovec.push_back(fi);
						}
					}

					m_savedformationinfovec[i] = std::make_pair(formationname,tformationifovec);
				}
				cfg_script.Pop();
			}

			if(cfg_script.Push("GroupCantonSpecialtyVec"))
			{
				nParam = 1;
				unsigned char _index = 0;
				int contribute,itemid,itemnum,itemtype;
				ref_ptr<GroupCantonSpecialty> specialty;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					contribute = atoi(v_str[0].c_str());
					itemid = atoi(v_str[1].c_str());
					itemnum = atoi(v_str[2].c_str());
					itemtype = atoi(v_str[3].c_str());
					if(contribute >= 0 && itemtype > 0)
					{
						specialty = new GroupCantonSpecialty;
						specialty->m_contribute = contribute;
						specialty->m_itemid = itemid;
						specialty->m_itemnum = itemnum;
						specialty->m_itemtype = itemtype;
						m_groupCantonSpecialtyVec[_index] = specialty;
					}
					else
					{
						m_groupCantonSpecialtyVec[_index] = NULL;
					}
					++_index;
				}
				cfg_script.Pop();
			}


			if(cfg_script.Push("ItemUsedTimeMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					if((int)atoi(v_str[0].c_str()) != 0)
					{
						m_itemusedtimeMap.insert(std::make_pair((int)atoi(v_str[0].c_str()),crArgumentParser::appAtoI64(v_str[1].c_str())));
					}
				}

				cfg_script.Pop();
			}

			////购买的头像相关的
			//if(cfg_script.Push("JXJPlayerBuyIconVec"))
			//{
			//	m_playerBuyIconVec.clear();
			//	nParam = 1;
			//	int nTmp = 0;
			//	while (cfg_script.Get("Param", nTmp,nParam++))
			//	{
			//		m_playerBuyIconVec.push_back(nTmp);
			//	}
			//	cfg_script.Pop();
			//}

			////////轮回福利map
			int maxlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,m_viplv).c_str());
			m_lunhuiRewardMap.clear();
			//m_lunhuiRewardMap.resize(maxlv);
			if(cfg_script.Push("JXJLunHuiRewardMap"))
			{
				nParam = 1;
				crVector3i tempItemInfo;
				//LunHuiRewardVec tempLunHuivec;
				unsigned char level;
				unsigned char bGet;
				//unsigned char vecSize;
				//std::string formationname;
				for (int i = 0; i < maxlv; ++i)
				{
					//等级
					if (cfg_script.Get("Param", str,nParam++))
						level = (unsigned char)atoi(str.c_str());
					if (cfg_script.Get("Param", str,nParam++))
						bGet = (unsigned char)atoi(str.c_str());
					if (cfg_script.Get("Param", str,nParam++))
						crArgumentParser::appAtoVec(str,tempItemInfo);
					//获得奖励物品

					m_lunhuiRewardMap.insert(std::make_pair(level, std::make_pair(bGet,tempItemInfo)));
				}
				cfg_script.Pop();
			}

			////MyGroupApplySet
			if(cfg_script.Push("MyGroupApplySet"))
			{
				m_myGroupApplySet.clear();
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					m_myGroupApplySet.insert(int1);
				}
				cfg_script.Pop();
			}

			//////////玩家投票记录
			m_selectLeaderIdSet.clear();
			if(cfg_script.Push("SelectLeaderIdSet"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", int1,nParam ++))
				{
					m_selectLeaderIdSet.insert(int1);
				}
				cfg_script.Pop();
			}

			// 已经通关的军团副本id
			m_hadWonGroupFuBenSet.clear();
			if(cfg_script.Push("HadWonGroupFuBenSet"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", int1,nParam ++))
				{
					m_hadWonGroupFuBenSet.insert(int1);
				}
				cfg_script.Pop();
			}

			// 新功能开启提示set，已提示过的在set中做记录
			m_newfunctionopenset.clear();
			if (cfg_script.Push("NewFunctionOpenSet"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1, nParam++))
				{
					m_newfunctionopenset.insert((unsigned short)int1);
				}
				cfg_script.Pop();
			}
			// 成就商店商品购买次数记录
			if(cfg_script.Push("AchieveShopMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					m_achieveShopMap.insert(std::make_pair(atoi(v_str[0].c_str()),atoi(v_str[1].c_str())));
				}
				cfg_script.Pop();
			}
			// 道具商店商品购买次数记录
			if(cfg_script.Push("GovShopMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", v_str,nParam++))
				{
					m_govShopMap.insert(std::make_pair(atoi(v_str[0].c_str()),atoi(v_str[1].c_str())));
				}
				cfg_script.Pop();
			}
			////////////////////////////////////////////////
			cfg_script.Close();
			DeleteFileA(file);
			//m_dataStream = NULL;
			if(m_savedformationindex < m_savedformationinfovec.size())
				m_formationInfoVec = m_savedformationinfovec[m_savedformationindex].second;
			else
			{
				m_savedformationindex = 0;
				m_formationInfoVec.clear();
			}
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJPlayerData PlayerStream UnCompress error"<<std::endl;
			}
			m_viplv = dataStream->_readUChar();
			m_playersex = dataStream->_readBool();
			m_playericonid = dataStream->_readUChar();
			//int nMaxBuyIconCount = dataStream->_readUChar();
			//m_playerBuyIconVec.resize(nMaxBuyIconCount);
			//for (int i=0; i<nMaxBuyIconCount; i++)
			//{
			//	m_playerBuyIconVec[i] = dataStream->_readUChar();
			//}
			m_shiliid = dataStream->_readUChar();
			m_homeid = dataStream->_readUShort();
			m_chengchiid = dataStream->_readUShort();
			m_goldingot = dataStream->_readInt();
			m_giftgold = dataStream->_readInt();
			m_coppercash = dataStream->_readInt();
			m_food = dataStream->_readInt();
			m_wood = dataStream->_readInt();
			m_iron = dataStream->_readInt();
			m_horse = dataStream->_readInt();
			m_fuyalv = dataStream->_readUChar();
			m_bingyinglv = dataStream->_readUChar();
			m_jiusilv = dataStream->_readUChar();
			m_gongfanglv = dataStream->_readUChar();
			m_shijilv = dataStream->_readUChar();
			m_fukulv = dataStream->_readUChar();
			m_mingjulv = dataStream->_readInt64();
			m_nongtianlv = dataStream->_readInt64();
			m_muchanglv = dataStream->_readInt();
			m_lingdilv = dataStream->_readInt();
			m_kuangshanlv = dataStream->_readInt();
			//预备部队
			int size;
			int i;
			int troopsaid;
			int troopsCount;
			unsigned short tequipid;
			int equipCount;
			//m_maxtroops = dataStream->_readUChar();
			size = dataStream->_readUChar();
			m_troopsMap.clear();
			for(i = 0; i<size; i++)
			{
				troopsaid = dataStream->_readInt();
				troopsCount = dataStream->_readInt();
				m_troopsMap.insert(std::make_pair(troopsaid,troopsCount));
			}
			size = dataStream->_readUChar();
			m_troopsEquipMap.clear();
			for(i = 0; i<size; i++)
			{
				tequipid = dataStream->_readUShort();
				equipCount = dataStream->_readInt();
				m_troopsEquipMap.insert(std::make_pair(tequipid,equipCount));
			}
			//背包格子数
			m_openbagnum = dataStream->_readUChar();
			//背包
			//int bagnummax = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax).c_str()));
			//背包列表
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int itemtypeindex= itemtab->getTitleIndex("类型");
			int itemtype;
			crTableIO::StrVec itemrecord;
			int itemid;
			m_itembagvec.resize(m_openbagnum);
			ref_ptr<crBagItemData> itemdata;
			unsigned char bagitemcount;
			for(i = 0; i<m_openbagnum; i++)
			{
				bagitemcount = dataStream->_readUChar();
				if(bagitemcount>0)
				{
					itemid = dataStream->_readInt();
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
					{
						itemtype = atoi(itemrecord[itemtypeindex].c_str());
						itemdata = new crBagItemData;
						itemdata->setItemID(itemid);
						itemdata->setItemCount(bagitemcount);
						if(itemtype == IT_Equip)
							itemdata->setEquipMagic(dataStream->_readInt());//装备附魔
						m_itembagvec[i] = itemdata;
					}
				}
			}
			//科技等级			m_infantrylv = dataStream->_readUChar();
			//m_jingbinglv = dataStream->_readUChar();
			//m_jianjialv = dataStream->_readUChar();
			//m_qiangrenlv = dataStream->_readUChar();
			//m_guimoulv = dataStream->_readUChar();
			//m_gongchenglv = dataStream->_readUChar();
			//m_yuangonglv = dataStream->_readUChar();
			//m_sugonglv = dataStream->_readUChar();
			//m_qiangxinglv = dataStream->_readUChar();
			//m_zhaomulv = dataStream->_readUChar();
			//m_rendelv = dataStream->_readUChar();
			//m_infantrylv = dataStream->_readUChar();
			//m_cavalrylv = dataStream->_readUChar();
			//m_archerlv = dataStream->_readUChar();
			//m_siegelv = dataStream->_readUChar();
			//m_shieldlv = dataStream->_readUChar();
			//m_guardlv = dataStream->_readUChar();
			//m_saddlelv = dataStream->_readUChar();
			//m_assaultlv = dataStream->_readUChar();
			//m_assistlv = dataStream->_readUChar();
			//m_penetratelv = dataStream->_readUChar();
			//m_manufacturelv = dataStream->_readUChar();
			//m_gunpowderlv = dataStream->_readUChar();
			//m_bravelv = dataStream->_readUChar();
			//m_ironcavalrylv = dataStream->_readUChar();
			//m_gearlv = dataStream->_readUChar();
			//m_fireboltlv = dataStream->_readUChar();
			//m_masterlv = dataStream->_readUChar();
			//m_juntunlv = dataStream->_readUChar();
			//m_reformlv = dataStream->_readUChar();
			//m_signin = dataStream->_readUChar();
			////FormationInfo
			size = dataStream->_readUChar();
			m_formationInfoVec.clear();
			ref_ptr<crFormationInfo> fi;
			for(i = 0; i<size; i++)
			{
				fi = new crFormationInfo;
				fi->setRoleID(dataStream->_readInt());//SceneServer需要用到
				fi->setAbstractID(dataStream->_readInt());
				fi->setCount(dataStream->_readUShort());
				fi->setEquips(dataStream->_readVec2i());
				m_formationInfoVec.push_back(fi);
			}
			//消费提示
			m_goldcosthint = dataStream->_readUInt();
			//////好友列表
			size = dataStream->_readUChar();
			m_friendList.clear();
			ref_ptr<crFriend> fd;
			for(i = 0; i<size; i++)
			{
				fd = new crFriend;
				fd->setPlayerID(dataStream->_readInt());
				fd->setName(dataStream->_readString());
				fd->setLevel(dataStream->_readUChar());
				fd->setIconID(dataStream->_readUChar());
				fd->setOnline(dataStream->_readBool());
				m_friendList.push_back(fd);
			}
			//将卡刷新
			//m_halltimer = dataStream->_readShort();
			//m_privatetimer = dataStream->_readInt();
			//for (int i = 0; i<5; i++)
			//{
			//	m_hallCard[i] = dataStream->_readInt();
			//}
			//for (int i = 0; i<3; i++)
			//{
			//	m_privateCard[i] = dataStream->_readInt();
			//}
			//征税
			m_levyCount = dataStream->_readUChar();
			//m_levyTimer = dataStream->_readInt();
			m_onlineRewardCount = dataStream->_readUChar();
			m_onlineRewardTimer = dataStream->_readInt();
			m_buyJunlingCount = dataStream->_readUChar();
			m_buyJunlingTimer = dataStream->_readUShort();
			//寄售
			size = dataStream->_readUChar();
			m_saleList.clear();
			ref_ptr<crSale> sale;
			for(i = 0; i<size; i++)
			{
				sale = new crSale;
				sale->setSaleListID(dataStream->_readChar());
				sale->setGoodsType(dataStream->_readInt());
				sale->setGoodsID(dataStream->_readInt());
				sale->setPrice(dataStream->_readInt());
				sale->setCount(dataStream->_readInt());
				sale->prepareTimer() = dataStream->_readShort();
				sale->setSaleID(dataStream->_readInt());
				sale->saleTimer() = dataStream->_readInt();
				sale->setSaleState(dataStream->_readUChar());
				sale->setGaincash(dataStream->_readInt());
				m_saleList.push_back(sale);
			}
			//////最近出售列表
			size = dataStream->_readUChar();
			m_saleitemlist.clear();
			ref_ptr<crSaleItemData> saleitemdata;
			int itemcount;
			for(i = 0; i<size; i++)
			{
				itemcount = dataStream->_readUChar();
				if(itemcount>0)
				{
					saleitemdata = new crSaleItemData;
					saleitemdata->setItemCount(itemcount);
					saleitemdata->setTime(dataStream->_readUShort());
					saleitemdata->setItemID(dataStream->_readInt());
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(saleitemdata->getItemID()),itemrecord)>=0)
					{
						itemtype = atoi(itemrecord[itemtypeindex].c_str());
						if(itemtype == IT_Equip)
							saleitemdata->setEquipMagic(dataStream->_readInt());
					}
					m_saleitemlist.push_back(saleitemdata);
				}

			}
			//将卡背包
			//int bagnummax = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax).c_str()));
			size = dataStream->_readUChar();
			m_jiangkabagvec.resize(size);
			ref_ptr<crBagItemData> jkitemdata;
			for(i = 0; i<size; i++)
			{
				bagitemcount = dataStream->_readUChar();
				if(bagitemcount>0)
				{
					jkitemdata = new crBagItemData;
					jkitemdata->setItemCount(bagitemcount);
					jkitemdata->setItemID(dataStream->_readInt());
					//jkitemdata->setEquipMagic(dataStream->_readUChar());
					m_jiangkabagvec[i] = jkitemdata;
				}
			}
			//将卡派遣列表
			//int bagnummax = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax).c_str()));
			size = dataStream->_readUChar();
			m_appointjiangkabagvec.resize(size);
			ref_ptr<crAppointCardData> jkitemdata2;
			for(i = 0; i<size; i++)
			{
				int bagitemid = dataStream->_readInt();
				if(bagitemid>0)
				{
					jkitemdata2 = new crAppointCardData;
					jkitemdata2->setItemID(bagitemid);
					jkitemdata2->setValidTime(dataStream->_readUShort());
					m_appointjiangkabagvec[i] = jkitemdata2;
				}
			}
			///////////FubenProgressMap
			unsigned short fubensize;
			m_fubenOrChengchiType = dataStream->_readUChar();
			m_fubenOrChengchiID = dataStream->_readUShort();
			fubensize = dataStream->_readUShort();
			m_fubenProgressMap.clear();
			unsigned short fubenid;
			short fubenProgress;
			unsigned char fubenPass;
			unsigned char fubenstarcount;
			ref_ptr<crFubenInfo> fubeninfo;
			for(i = 0; i<fubensize; i++)
			{
				fubenid = dataStream->_readUShort();
				fubenProgress = dataStream->_readShort();
				fubenPass = dataStream->_readUChar();
				fubenstarcount = dataStream->_readUChar();
				fubeninfo = new crFubenInfo;
				fubeninfo->setFubenProgress(fubenProgress);
				fubeninfo->setComplete(fubenPass);
				fubeninfo->setStarCount(fubenstarcount);
				m_fubenProgressMap.insert(std::make_pair(fubenid,fubeninfo));
			}
			///////////////////////////
			//时间队列
			m_buildingTimes = dataStream->_readVec4i();
			m_recruitTimes = dataStream->_readVec4i();
			m_ckilleachtroops = dataStream->_readVec4i();
			m_cdeadeachtroops = dataStream->_readVec4i();
			m_danchangkill = dataStream->_readVec4i();
			m_killdoorboss = dataStream->_readVec4s();
			m_junling = dataStream->_readUShort();
			/////////////////////////ExtraProduceMap
			m_extraProduceMap.clear();
			ref_ptr<crExtraProduce> extraproduce;
			size = dataStream->_readUChar();
			unsigned char type;
			for(i = 0; i<size; i++)
			{
				extraproduce = new crExtraProduce;
				type = dataStream->_readUChar();
				extraproduce->timer() = dataStream->_readInt();
				extraproduce->setPercent(dataStream->_readShort());
				m_extraProduceMap[type] = extraproduce;
			}
			//m_luckycardcoppercost = dataStream->_readUChar();
			//m_luckycardgoldcost = dataStream->_readUChar();
			//m_knowluckycard = dataStream->_readUChar();
			//m_jiusirefreshtime = dataStream->_readInt();
			m_vipExp = dataStream->_readInt();
			m_countrycontribute = dataStream->_readInt();
			for (int i = 0; i < 3; i++)
			{
				m_luckycard[i]  = dataStream->_readInt();
			}
			m_cardimpression = dataStream->_readUShort();
			m_cardpieces = dataStream->_readInt();
			m_freeCounterinsurgency = dataStream->_readUChar();
			m_RefineEquipFreeCount = dataStream->_readUShort();
			m_NongTianShuiLilv = dataStream->_readUChar();
			m_ZhiShuZaoLinlv = dataStream->_readUChar();
			m_YouZhiCaoChanglv = dataStream->_readUChar();
			m_JingKuangShenWalv = dataStream->_readUChar();
			m_GuLiMaoYilv = dataStream->_readUChar();
			m_ZhengShouXinDelv = dataStream->_readUChar();
			m_YanBingYueWulv = dataStream->_readUChar();
			m_XunBingXinDelv = dataStream->_readUChar();
			m_ZongMiaoJiSilv = dataStream->_readUChar();
			m_JiSiXinDelv = dataStream->_readUChar();
			m_MarketBuyResourceMax = dataStream->_readInt();
			m_FreeRollCardCount = dataStream->_readUShort();
			m_autosellcard  = dataStream->_readUShort();
			//军备map  
			m_troopstechmap.clear();
			size = dataStream->_readUChar();
			unsigned short troopstechid = 0;
			unsigned char troopstechlv = 0;
			for(i = 0; i<size; i++)
			{
				troopstechid = dataStream->_readUShort();
				troopstechlv = dataStream->_readUChar();
				m_troopstechmap.insert(std::make_pair(troopstechid,troopstechlv));
			}
			//组合将卡科技等级  
			m_cardExtraTechMap.clear();
			size = dataStream->_readUChar();
			unsigned short cardextraid = 0;
			unsigned char cardextralv = 0;
			for(i = 0; i<size; i++)
			{
				cardextraid = dataStream->_readUShort();
				cardextralv = dataStream->_readUChar();
				m_cardExtraTechMap.insert(std::make_pair(cardextraid,cardextralv));
			}
			//玩家在征战商店每日可购买物品上限 
			m_buyshagnxianmap.clear();
			size = dataStream->_readUChar();
			int wupinid = 0;
			unsigned char shuliang = 0;
			for(i = 0; i<size; i++)
			{
				wupinid = dataStream->_readInt();
				shuliang = dataStream->_readUChar();
				m_buyshagnxianmap.insert(std::make_pair(wupinid,shuliang));
			}
			//玩家兵种成就相关信息
			m_troopsachievemap.clear();
			size = dataStream->_readUChar();
			int troopsid = 0;
			CRCore::crVector3i countinfo;
			for(i = 0; i<size; i++)
			{
				troopsid = dataStream->_readInt();
				countinfo[0] = dataStream->_readInt();
				countinfo[1] = dataStream->_readInt();
				countinfo[2] = dataStream->_readInt();
				m_troopsachievemap.insert(std::make_pair(troopsid,countinfo));
			}
			m_CardImpression2CritTechLv = dataStream->_readUChar();
			m_CardImpression4CritTechLv = dataStream->_readUChar();
			m_CardImpression6CritTechLv = dataStream->_readUChar();
			m_CardImpression10CritTechLv = dataStream->_readUChar();
			m_FreeRollCardTechLv = dataStream->_readUChar();
			m_OrangeCardUpgradeTechLv = dataStream->_readUChar();
			m_CardShopOpenTechLv = dataStream->_readUChar();
			m_MakeEquipTechLv = dataStream->_readUChar();
			m_EquipRefineTechLv = dataStream->_readUChar();
			m_EquipAttachTechLv = dataStream->_readUChar();
			m_SmeltEquipTechLv = dataStream->_readUChar();
			m_SmeltMaterialTechLv = dataStream->_readUChar();
			m_SmeltStoneTechLv = dataStream->_readUChar();
			m_EquipRefineMaxCount1TechLv = dataStream->_readUChar();
			m_EquipRefineMaxCount2TechLv = dataStream->_readUChar();
			m_EquipRefineMaxCount3TechLv = dataStream->_readUChar();
			m_EquipRefineMaxCount4TechLv = dataStream->_readUChar();
			m_EquipRefineProduce1TechLv = dataStream->_readUChar();
			m_EquipRefineProduce2TechLv = dataStream->_readUChar();
			m_EquipRefineProduce3TechLv = dataStream->_readUChar();
			m_EquipRefineProduce4TechLv = dataStream->_readUChar();
			m_FreeRollCard2TechLv = dataStream->_readUChar();
			m_FreeRollCard3TechLv = dataStream->_readUChar();
			m_FreeRollCardProduce1TechLv = dataStream->_readUChar();
			m_FreeRollCardProduce2TechLv = dataStream->_readUChar();
			m_FreeRollCardProduce3TechLv = dataStream->_readUChar();
			m_FreeRollCardProduce4TechLv = dataStream->_readUChar();
			m_FuKuMaxTechLv = dataStream->_readUChar();
			////////////////////////////////////////////
			m_opensavedformationifo = dataStream->_readUChar();
			m_changeShiliTimes = dataStream->_readUChar();
			m_seriesLandDays = dataStream->_readUShort();
			m_LandRewardGetCount = dataStream->_readUChar();
			m_chatChannelSetInfo = (_ULONGLONG)dataStream->_readInt64();

			std::string blackname;
			size = dataStream->_readUChar();
			for(i = 0; i<size; i++)
			{
				blackname = dataStream->_readString();
				m_blacknameset.insert(blackname);
			}
			//研发中科技map  
			m_techdevelopmap.clear();
			size = dataStream->_readUChar();
			int developtechid = 0;
			float developtechtime = 0.0f;
			for(i = 0; i<size; i++)
			{
				developtechid = dataStream->_readInt();
				developtechtime = dataStream->_readFloat();
				m_techdevelopmap.insert(std::make_pair(developtechid,developtechtime));
			}
			////说服队列  
			//CRCore::ref_ptr<crJXJPersuadeInfo> jxjPersuadeInfo;
			//m_persuadeInfoDeq.clear();
			//size = dataStream->_readUChar();
			//for(i = 0; i<size; i++)
			//{
			//	jxjPersuadeInfo = new crJXJPersuadeInfo;
			//	jxjPersuadeInfo->setFubenId(dataStream->_readUShort());
			//	jxjPersuadeInfo->setJKId(dataStream->_readInt());
			//	jxjPersuadeInfo->setTime(dataStream->_readInt());
			//	jxjPersuadeInfo->setLevel(dataStream->_readUChar());

			//	m_persuadeInfoDeq.push_back(jxjPersuadeInfo);
			//}
			//SavedFormationInfoVec
			unsigned char formationsize = 0;
			size = dataStream->_readUChar();
			//ref_ptr<crFormationInfo> fi = NULL;
			FormationInfoVec tformationifovec;
			std::string formationname;
			m_savedformationinfovec.clear();
			for(i = 0; i<size; i++)
			{
				formationname = dataStream->_readString();
				formationsize = dataStream->_readUChar();
				tformationifovec.clear();
				tformationifovec.resize(formationsize);
				for (int ii = 0; ii < formationsize;++ii)
				{
					fi = new crFormationInfo;
					fi->setRoleID(dataStream->_readInt());//SceneServer需要用到
					fi->setAbstractID(dataStream->_readInt());
					fi->setCount(dataStream->_readUShort());
					fi->setEquips(dataStream->_readVec2i());
					tformationifovec[ii] = fi;
				}
				m_savedformationinfovec.push_back(std::make_pair(formationname,tformationifovec));
			}
			//轮回福利
			size = dataStream->_readUChar();
			unsigned char level;
			unsigned char bget;
			crVector3i tempVec3;
			m_lunhuiRewardMap.clear();
			for(i = 0; i<size; i++)
			{
				level = dataStream->_readUChar();
				bget = dataStream->_readUChar();
				tempVec3 = dataStream->_readVec3i();
				m_lunhuiRewardMap.insert(std::make_pair(level,std::make_pair(bget,tempVec3)));
			}
			m_skipnewhand = dataStream->_readBool();

			m_consortiaid = dataStream->_readInt();
			m_savedformationindex = dataStream->_readUChar();
			m_iftechbtnnew = dataStream->_readUChar();
			m_FuKuMaxTech2Lv = dataStream->_readUChar();
			m_FuKuMaxTech3Lv = dataStream->_readUChar();

			//新功能开启提示
			unsigned short newfunctionid = 0;
			size = dataStream->_readUChar();
			m_newfunctionopenset.clear();
			for (i = 0; i < size; i++)
			{
				newfunctionid = dataStream->_readUShort();
				m_newfunctionopenset.insert(newfunctionid);
			}
			m_lingqufengluflag = dataStream->_readUChar();
			m_tuntianlv = dataStream->_readUChar();
			m_yelianlv = dataStream->_readUChar();
			m_fanzhilv = dataStream->_readUChar();
			m_fanmaolv = dataStream->_readUChar();
			m_qiaojianglv = dataStream->_readUChar();
			m_zhengshuilv = dataStream->_readUChar();
			m_techfenglulv = dataStream->_readUChar();
			m_techjiusibaojilv = dataStream->_readUChar();
			m_techworldtalklv = dataStream->_readUChar();
			m_techbuyjunlinglv = dataStream->_readUChar();
			m_foundCountryAward = dataStream->_readUChar();
			m_playertotalpaygold = dataStream->_readInt();
			m_shuichelv = dataStream->_readUChar();
			m_ronglulv = dataStream->_readUChar();
			m_gongshelv = dataStream->_readUChar();
			m_mochuanlv = dataStream->_readUChar();
			m_loginDays = dataStream->_readUShort();
			m_7DaysAward = dataStream->_readUChar();
			m_shouchong = dataStream->_readUChar();
			m_cZhengZhanQuan = dataStream->_readInt();
			m_killplayercount = dataStream->_readInt();
			m_killnpccount = dataStream->_readInt();
			m_dayplayerdeadcount = dataStream->_readInt();
			m_playerdeadcount = dataStream->_readInt();
			m_usedachievepoint = dataStream->_readInt();
			m_passmingjiangcount = dataStream->_readUShort();
			m_Cpassworldfuben = dataStream->_readUShort();
			m_cmingjiangfail = dataStream->_readUChar();
			m_ctianxiafail = dataStream->_readUChar();
			m_havegotviplv = dataStream->_readUChar();
			m_Cpasstianxia = dataStream->_readUShort();
			m_cfinishdays = dataStream->_readUChar();
			m_accomplishPointCache = dataStream->_readInt();
			m_praisedcount = dataStream->_readInt();
			m_praisecount = dataStream->_readInt();
			m_cGuoZhanJiFen = dataStream->_readInt();
			m_treasonbuffertimer = dataStream->_readInt();
			m_treasonpunishtimer = dataStream->_readInt();
			m_treasoncoolingtimer = dataStream->_readInt();
			m_selectedshili = dataStream->_readUChar();
			m_timesshangwu = dataStream->_readUChar();
			m_timesxiawu = dataStream->_readUChar();
			m_timeswanshang = dataStream->_readUChar();
			m_praiselimit = dataStream->_readUChar();
			m_yuekaid = dataStream->_readShort();
			m_yuekaendtime = dataStream->_readInt64();
			m_lastgettime = dataStream->_readInt64();
			m_kaoshangfutime = dataStream->_readInt64();
			m_specialtyrefreshtimes = dataStream->_readUChar();
			m_lastchongzhitime = dataStream->_readInt64();
			m_hadwonjuntuanfubenT = dataStream->_readBool();
			m_huoyuedusign = dataStream->_readBool();
			m_gainedzhoukahuodongid = dataStream->_readInt();
			m_zhoukaendtime = dataStream->_readInt64();
			m_zhoukagetstatus = dataStream->_readUChar();
			m_zhoukagetdayidx = dataStream->_readInt();
			m_playerCycleCount = dataStream->_readShort();
			m_playerOnlineRewardTimer = dataStream->_readInt();
			m_recommendSet = dataStream->_readBool();
			m_equipBindInfo = dataStream->_readInt();
			m_isYellow = dataStream->_readUChar();
			m_isYearYellow = dataStream->_readUChar();
			m_yellowLevel = dataStream->_readUChar();
			m_yellowAwardInfo = dataStream->_readInt();
			m_cancelCantonTimer = dataStream->_readInt();
			m_guanzhi = dataStream->_readUChar();
			m_cardImpressionLevel = dataStream->_readUShort();
			m_resourceCount = dataStream->_readUChar();
			m_freeBuy = dataStream->_readUChar();
			m_goldRecruitCount = dataStream->_readUChar();
			m_zhengZhaoTimes = dataStream->_readUChar();
			m_attackVictoryTimes = dataStream->_readUChar();
			m_defendVictoryTimes = dataStream->_readUChar();
			m_releaseName = dataStream->_readString();
			m_countryBattleTimes = dataStream->_readUShort();
			m_hadGottenReward = dataStream->_readBool();
			m_myRankVal = dataStream->_readUChar();
			m_startedDays = dataStream->_readUShort();
			m_gameIntroductionNoTips = dataStream->_readBool();
			m_hadShowFestivalHuodongCanvas = dataStream->_readBool();
			for (int i = 0; i < 7; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					m_7DayData[i][j] = dataStream->_readChar();
				}
			}
			//成就商店商品购买次数记录
			unsigned char achieveid = 0;
			unsigned char achievebuycount = 0;
			size = dataStream->_readUChar();
			m_achieveShopMap.clear();
			for (i = 0; i < size; i++)
			{
				achieveid = dataStream->_readUChar();
				achievebuycount = dataStream->_readUChar();
				m_achieveShopMap[achieveid] = achievebuycount;
			}
			//道具商店商品购买次数记录
			unsigned char govshopid = 0;
			unsigned char itembuycount = 0;
			size = dataStream->_readUChar();
			m_govShopMap.clear();
			for (i = 0; i < size; i++)
			{
				govshopid = dataStream->_readUChar();
				itembuycount = dataStream->_readUChar();
				m_govShopMap[govshopid] = itembuycount;
			}
			m_daykillplayercount = dataStream->_readInt();
			m_daykillnpccount = dataStream->_readInt();
			m_dayckilleachtroops = dataStream->_readVec4i();
			m_autoBuilding = dataStream->_readBool();
			m_buildCountryTimes = dataStream->_readChar();
			m_foreigncard = dataStream->_readVec3i();
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_NetStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_viplv = dataStream->_readUChar();
			m_playersex = dataStream->_readBool();
			m_playericonid = dataStream->_readUChar();

			m_shiliid = dataStream->_readUChar();
			m_homeid = dataStream->_readUShort();
			m_chengchiid = dataStream->_readUShort();
			m_guanzhi = dataStream->_readUChar();
			//m_foundCountryAward = dataStream->_readUChar();
			//m_7DaysAward = dataStream->_readUChar();
			//////FormationInfo
			unsigned char size = dataStream->_readUChar();
			m_formationInfoVec.clear();
			ref_ptr<crFormationInfo> fi;
			for(int j = 0; j<size; j++)
			{
				fi = new crFormationInfo;
				fi->setAbstractID(dataStream->_readInt());
				fi->setCount(dataStream->_readUShort());
				fi->setEquips(dataStream->_readVec2i());
				m_formationInfoVec.push_back(fi);
			}
			////军备map  
			//m_troopstechmap.clear();
			//size = dataStream->_readUChar();
			//unsigned short troopstechid = 0;
			//unsigned char troopstechlv = 0;
			//for(i = 0; i<size; i++)
			//{
			//	troopstechid = dataStream->_readUShort();
			//	troopstechlv = dataStream->_readUChar();
			//	m_troopstechmap.insert(std::make_pair(troopstechid,troopstechlv));
			//}
			//m_fubenOrChengchiType = dataStream->_readUChar();
			//m_fubenOrChengchiID = dataStream->_readUShort();
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_CreateStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_playersex = dataStream->_readBool();
			m_playericonid = dataStream->_readUChar();
			m_shiliid = dataStream->_readUChar();

			m_homeid = dataStream->_readUShort();
			m_chengchiid = dataStream->_readUShort();
		}
		break;
	case WCHDATA_UpdateStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			///////
			//资源派发
			m_coppercash = dataStream->_readInt();
			m_food = dataStream->_readInt();
			m_wood = dataStream->_readInt();
			m_iron = dataStream->_readInt();
			m_horse = dataStream->_readInt();
			//时间队列
			m_buildingTimes = dataStream->_readVec4i();
			m_recruitTimes = dataStream->_readVec4i();
			//ExtraProduceMap
			m_extraProduceMap.clear();
			ref_ptr<crExtraProduce> extraproduce;
			unsigned char size = dataStream->_readUChar();
			unsigned char type;
			for(i = 0; i<size; i++)
			{
				extraproduce = new crExtraProduce;
				type = dataStream->_readUChar();
				extraproduce->timer() = dataStream->_readInt();
				extraproduce->setPercent(dataStream->_readShort());
				m_extraProduceMap[type] = extraproduce;
			}
			//TechDevelopMap
			m_techdevelopmap.clear();
			size = dataStream->_readUChar();
			int techid = 0;
			float techcd = 0.0f;
			for(i = 0; i<size; i++)
			{
				techid = dataStream->_readInt();
				techcd = dataStream->_readFloat();
				m_techdevelopmap.insert(std::make_pair(techid,techcd));
			}
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_SceneSaveStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			//////FormationInfo
			unsigned char size = dataStream->_readUChar();
			int fsize = m_formationInfoVec.size();
			for(int i = 0; i<size; i++)
			{
				if(i<fsize)
				{
					m_formationInfoVec[i]->setCount(dataStream->_readUShort());
				}
			}
			//if(m_savedformationindex < m_savedformationinfovec.size())
			//	m_savedformationinfovec[m_savedformationindex].second = m_formationInfoVec;
			//m_curFubenProgress = m_dataStream->_readShort();
			//if(m_curFubenProgress>0)
			//{
			//	if(m_fubenOrChengchiType==FT_Fuben && m_fubenOrChengchiID>0)
			//	{
			//		ref_ptr<crTableIO>tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			//		crTableIO::StrVec record;
			//		if(tab->queryOneRecord(0,crArgumentParser::appItoa(m_fubenOrChengchiID),record))
			//		{
			//			ref_ptr<crFubenInfo> fubeninfo;
			//			FubenProgressMap::iterator itr = m_fubenProgressMap.find(m_fubenOrChengchiID);
			//			if(itr != m_fubenProgressMap.end())
			//			{
			//				fubeninfo = itr->second.get();
			//				fubeninfo->setFubenProgress(m_curFubenProgress);
			//			}
			//			else
			//			{
			//				fubeninfo = new crFubenInfo;
			//				fubeninfo->setFubenProgress(m_curFubenProgress);
			//				fubeninfo->setComplete(0);
			//				m_fubenProgressMap[m_fubenOrChengchiID] = fubeninfo;
			//			}
			//			if(m_curFubenProgress>0)
			//			{
			//				unsigned char curcompletecount = 0;
			//				int progressid = tab->getTitleIndex("结束进度");
			//				crVector4s closevec;
			//				crArgumentParser::appAtoVec(record[progressid],closevec);
			//				for(int i = 0; i<4; i++)
			//				{
			//					if(m_curFubenProgress==closevec[i])
			//					{//副本结束，进度重置，并设置完成标识符
			//						curcompletecount = m_fubenProgressMap[m_fubenOrChengchiID]->getComplete();
			//						if (curcompletecount == 0)
			//						{
			//							curcompletecount += 2;
			//						}
			//						else
			//						{
			//							curcompletecount += 1;
			//						}
			//						if (curcompletecount > c_MaxFubenCompleteCount)
			//						{
			//							curcompletecount = c_MaxFubenCompleteCount;
			//						}
			//						m_curFubenProgress = 0;
			//						fubeninfo->setFubenProgress(m_curFubenProgress);
			//						fubeninfo->setComplete(curcompletecount);
			//						break;
			//					}
			//				}
			//			}
			//		}
			//	}
			//}
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_JXJFubenRewardInfo:
		m_fubenReward = param?(crPlayerRewardData *)param:NULL;
		break;
	case WCHDATA_JXJFubenReliveCount:
		m_reliveCount = param?*(unsigned char *)param:0;
		break;
	case WCHDATA_JXJFreeCounterinsurgencyCount:
		m_freeCounterinsurgency = param?*(unsigned char *)param:0;
		break;
	case WCHDATA_JXJRefineEquipFreeCount:
		m_RefineEquipFreeCount = param?*(unsigned short *)param:0;
		break;
	case WCHDATA_JXJRebelTimer:
		m_rebelTimer = param?*(unsigned short *)param:0;
		break;
	case WCHDATA_JXJRollCardFreeCount:
		m_FreeRollCardCount = param?*(unsigned short *)param:0;
		break;
	case WCHDATA_JXJPlayerStore:
		m_playerStore = param?(crJXJPlayerStore *)param:NULL;
		break;
	case WCHDATA_JXJPlayerCycleCount:
		m_playerCycleCount = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJClientIsServerCycling:
		m_bisCycling = param?*(bool *)param:false;
		break;
	case WCHDATA_JXJClientCycleTimer:
		m_iClientCycleTime = param?*(int *)param:0;
		break;
	case WCHDATA_JXJLandRewardClientTime:
		m_nLandRewardTime = param?*(int *)param:0;
		break;
	case WCHDATA_JXJConsortiaID:
		m_consortiaid = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_JXJIfTechBtnNew:
		m_iftechbtnnew = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJGroupContributeiType			:
		m_groupContributeiType = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJGroupContributeiTimes			:
		m_groupContributeiTimes = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJInspireLevel:
		m_inspireLv = param?*(unsigned char *)param:NULL;
		break;
	case WCHDATA_JXJMyCountryData:
		m_myShili = param?(crJXJShili*)param:NULL;
		break;
	//case WCHDATA_JXJServerVersion				:
	//	m_serverVersion = param ? *(int *)param : NULL;
	//	break;
	case WCHDATA_JXJCenterNoticeRefershTime:
		m_flushTime = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJCurQueryBattleReportID:
		m_battleReportQueryedId = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJLingqufengluFlag:
		m_lingqufengluflag = param ? *((unsigned char*)param) : NULL;
		break;
	//case WCHDATA_JXJMainRoleRefreshFlag:
	//	m_mainrolerefreshflag = param ? *((unsigned char*)param) : NULL;
	//	break;
	case WCHDATA_JXJFoundCountryGetAward			:
		m_foundCountryAward = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerLoginDays:
		m_loginDays = param ? *(unsigned short *) param : NULL;
		break;
	case WCHDATA_JXJ7DaysGetAward			:
		m_7DaysAward = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJShouchong:
		m_shouchong = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJZhengZhanQuan:
		m_cZhengZhanQuan = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJKillPlayerCount:
		m_killplayercount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJKillNpcCount:
		m_killnpccount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJDayPlayerDeadCount:
		m_dayplayerdeadcount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerDeadCount:
		m_playerdeadcount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJDayKillPlayerCount:
		m_daykillplayercount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJDayKillNPCCount:
		m_daykillnpccount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJDayKillEachTroops:
		m_dayckilleachtroops = param ? *((crVector4i *)param) : crVector4i(0,0,0,0);
		break;
	case WCHDATA_JXJUsedAchievePoints:
		m_usedachievepoint = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJPassMingJiangCount:
		m_passmingjiangcount = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJPassWorldFuBenCount:
		m_Cpassworldfuben = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJMingJiangFailCount:
		m_cmingjiangfail = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJZhengZhanTianXiaFailCount:
		m_ctianxiafail = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerHaveGotViplvReward:
		m_havegotviplv = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPassZhengZhanTianXiaCount:
		m_Cpasstianxia = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJFinishHuoYueDuDayCount:
		m_cfinishdays = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJAchievePointsCount:
		m_accomplishPointCache = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJHavePraiseCounts:
		m_praisedcount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJPraiseOtherCounts:
		m_praisecount = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJGuoZhanJiFen:
		m_cGuoZhanJiFen = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJTreasonBufferTimer:
		m_treasonbuffertimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJTreasonPunishTimer:
		m_treasonpunishtimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJTreasonCoolingTimer:
		m_treasoncoolingtimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerSelectedShiLi:
		m_selectedshili = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJAmHuoDongGetTimes:
		m_timesshangwu = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPmHuoDongGetTimes:
		m_timesxiawu = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJNightHuoDongGetTimes:
		m_timeswanshang = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerPraiseLimit:
		m_praiselimit = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJPlayerGuanZhi:
		m_guanzhi = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJYueKaID:
		m_yuekaid = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJYueKaEndTime:
		m_yuekaendtime = param ? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJYueKaLastGetTime:
		m_lastgettime = param? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJKaoShangFuUsedTime:
		m_kaoshangfutime = param? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJSpecialtyRefreshTimes:
		m_specialtyrefreshtimes = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJLastChongZhiTime:
		m_lastchongzhitime = param ? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJLastZeroRefreshTime:
		m_lastzeroRefreshtime = param ? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJChongZhiCountT:
		m_chongzhicountT = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJHadWonJunTuanFBT:
		m_hadwonjuntuanfubenT = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJIsCheckHuoYueDuSign:
		m_huoyuedusign = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJGainedZhouKaID:
		m_gainedzhoukahuodongid = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJZhouKaEndTime:
		m_zhoukaendtime = param ? *((_crInt64 *)param) : NULL;
		break;
	case WCHDATA_JXJZhouKaGetStatus:
		m_zhoukagetstatus = param? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJZhouKaGetDayIdx:
		m_zhoukagetdayidx = param? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJBattleActiveTimer:
		m_battleActiveTimer = param ? *((float *)param) : NULL;
		break;
	case WCHDATA_JXJBattleValidateID:
		m_battleValidateID = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJElector:
		m_elector = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJVoteTimes:
		m_vote = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJCandidate:
		m_beElected = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJCountryRecommendSet:
		m_recommendSet = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJElectIconTimeControl:
		m_electIcon = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJCancelCantonTimer:
		m_cancelCantonTimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJHadShowDismissTip:
		m_hadshowdismisstip = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJUsedZhengZhaoTimes:
		m_zhengZhaoTimes = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJCheckIsMyself:
		m_myself = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJCurZhengzhaolingPage:
		m_pageNum = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJZhengzhaolingUpdateTimer:
		m_zhengZhaolingUpdateTimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJGuoJiaReleaseTimer:
		m_GuoJiaReleaseTimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJJunTuanReleaseTimer:
		m_JunTuanReleaseTimer = param ? *((int *)param) : NULL;
		break;
	case WCHDATA_JXJCheckYingzhengReward:
		m_CheckYingzhengReward = param ? ((bool *)param) : NULL;
		break;
	case WCHDATA_JXJGuoZhanAttackVictoryTimes:
		m_attackVictoryTimes = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJGuoZhanDefendVictoryTimes:
		m_defendVictoryTimes = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJCountryBattleTimes:
		m_countryBattleTimes = param ? *((unsigned short*)param) : NULL;
		break;
	case WCHDATA_JXJAcceptZhengZhaoEnterBroadcast:
		m_releaseName = param ? *((std::string *)param) : NULL;
		break;
	case WCHDATA_JXJChooseAutoBattleMode:
		m_autoBattle = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJGameIntroductionCurPageNum:
		m_curPageNum_Introduction = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJHadGottenConsortiaPromotionReward:
		m_hadGottenReward = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJConsortiaPromotionMyRankVal:
		m_myRankVal = param ? *((unsigned char *)param) : NULL;
		break;
	case WCHDATA_JXJGameStartedDays:
		m_startedDays = param ? *((unsigned short *)param) : NULL;
		break;
	case WCHDATA_JXJGameIntroductionNoTipsCheck:
		m_gameIntroductionNoTips = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJHadShowFestivalHuodongCanvas:
		m_hadShowFestivalHuodongCanvas = param ? *((bool *)param) : NULL;
		break;
	case WCHDATA_JXJOnlineTimer:
		m_onlinetimer = param ? *((int *)param) : 0;
		break;
	case WCHDATA_JXJAutoBuilding:
		m_autoBuilding = param ? *((bool *)param) : 0;
		break;
	case WCHDATA_JXJBuildCountryTimes:
		m_buildCountryTimes = param ? *((char *)param) : 0;
		break;
	case WCHDATA_JXJForeignCard:
		m_foreigncard = param ? *((crVector3i *)param) : crVector3i(0, 0, 0);
		break;
	default:
		break;
	}
}
void crJXJPlayerData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_DataStream:
	//	if(m_dataStream.valid())
	//	{
	//		m_dataStream->seekBegin();
	//		param = &m_dataStream;
	//	}
	//	else
	//	{
	//		param = NULL;
	//	}
	//	break;
	case WCHDATA_JXJPlayerID:
		param = &m_playerid;
		break;
	case WCHDATA_PlayerSex:
		param = &m_playersex;
		break;
	case WCHDATA_PlayerIconID:
		param = &m_playericonid;
		break;
	//case WCHDATA_JXJMainRoleBuyIcon:
 //		param = &m_playerBuyIconVec;
	//	break;
	case WCHDATA_JXJShiliID:
		param = &m_shiliid;
		break;
	case WCHDATA_JXJHomeID:
		param = &m_homeid;
		break;
	case WCHDATA_JXJChengchiID:
		param = &m_chengchiid;
		break;
	case WCHDATA_JXJGoldingot:
		param = &m_goldingot;
		break;
	case WCHDATA_JXJGiftGold:
		param = &m_giftgold;
		break;
	case WCHDATA_JXJCoppercash:
		param = &m_coppercash;
		break;
	case WCHDATA_JXJFood:
		param = &m_food;
		break;
	case WCHDATA_JXJWood:
		param = &m_wood;
		break;
	case WCHDATA_JXJIron:
		param = &m_iron;
		break;
	case WCHDATA_JXJHorse:
		param = &m_horse;
		break;
	case WCHDATA_JXJFuyalv:
		param = &m_fuyalv;
		break;
	case WCHDATA_JXJBingyinglv:
		param = &m_bingyinglv;
		break;
	case WCHDATA_JXJJiusilv:
		param = &m_jiusilv;
		break;
	case WCHDATA_JXJGongfanglv:
		param = &m_gongfanglv;
		break;
	case WCHDATA_JXJShijilv:
		param = &m_shijilv;
		break;
	case WCHDATA_JXJFukulv:
		param = &m_fukulv;
		break;
	case WCHDATA_JXJMingjulv:
		param = &m_mingjulv;
		break;
	case WCHDATA_JXJEquipBindInfo:
		param = &m_equipBindInfo;
		break;
	case WCHDATA_JXJISYELLOW:
		param = &m_isYellow;
		break;
	case WCHDATA_JXJISYEARYELLOW:
		param = &m_isYearYellow;
		break;
	case WCHDATA_JXJYELLOWLEVEL:
		param = &m_yellowLevel;
		break;
	case WCHDATA_JXJYELLOWAWARDINFO:
		param = &m_yellowAwardInfo;
		break;
	case WCHDATA_JXJMingjulv1:
	case WCHDATA_JXJMingjulv2:
	case WCHDATA_JXJMingjulv3:
	case WCHDATA_JXJMingjulv4:
	case WCHDATA_JXJMingjulv5:
	case WCHDATA_JXJMingjulv6:
	case WCHDATA_JXJMingjulv7:
	case WCHDATA_JXJMingjulv8:
		{//使用时候注意不能修改
			_crInt32 lo32 = LOINT64(m_mingjulv);
			_crInt32 hi32 = HIINT64(m_mingjulv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			m_utemp[0] = (unsigned char)LOINT16(lo16);
			m_utemp[1] = (unsigned char)HIINT16(lo16);
			m_utemp[2] = (unsigned char)LOINT16(hi16);
			m_utemp[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			m_utemp[4] = (unsigned char)LOINT16(lo16);
			m_utemp[5] = (unsigned char)HIINT16(lo16);
			m_utemp[6] = (unsigned char)LOINT16(hi16);
			m_utemp[7] = (unsigned char)HIINT16(hi16);
			param = &(m_utemp[i-(WCHDATA_JXJMingjulv1)]);
		}
		break;
	case WCHDATA_JXJNongtianlv:
		param = &m_nongtianlv;
		break;
	case WCHDATA_JXJNongtianlv1:
	case WCHDATA_JXJNongtianlv2:
	case WCHDATA_JXJNongtianlv3:
	case WCHDATA_JXJNongtianlv4:
	case WCHDATA_JXJNongtianlv5:
	case WCHDATA_JXJNongtianlv6:
	case WCHDATA_JXJNongtianlv7:
	case WCHDATA_JXJNongtianlv8:
		{//使用时候注意不能修改
			_crInt32 lo32 = LOINT64(m_nongtianlv);
			_crInt32 hi32 = HIINT64(m_nongtianlv);
			_crInt16 lo16 = LOINT32(lo32);
			_crInt16 hi16 = HIINT32(lo32);
			m_utemp[0] = (unsigned char)LOINT16(lo16);
			m_utemp[1] = (unsigned char)HIINT16(lo16);
			m_utemp[2] = (unsigned char)LOINT16(hi16);
			m_utemp[3] = (unsigned char)HIINT16(hi16);
			lo16 = LOINT32(hi32);
			hi16 = HIINT32(hi32);
			m_utemp[4] = (unsigned char)LOINT16(lo16);
			m_utemp[5] = (unsigned char)HIINT16(lo16);
			m_utemp[6] = (unsigned char)LOINT16(hi16);
			m_utemp[7] = (unsigned char)HIINT16(hi16);
			param = &(m_utemp[i-(WCHDATA_JXJNongtianlv1)]);
		}
		break;
	case WCHDATA_JXJMuchanglv:
		param = &m_muchanglv;
		break;
	case WCHDATA_JXJMuchanglv1:
	case WCHDATA_JXJMuchanglv2:
	case WCHDATA_JXJMuchanglv3:
	case WCHDATA_JXJMuchanglv4:
		{//使用时候注意不能修改
			_crInt16 lo16 = LOINT32(m_muchanglv);
			_crInt16 hi16 = HIINT32(m_muchanglv);
			m_utemp[0] = (unsigned char)LOINT16(lo16);
			m_utemp[1] = (unsigned char)HIINT16(lo16);
			m_utemp[2] = (unsigned char)LOINT16(hi16);
			m_utemp[3] = (unsigned char)HIINT16(hi16);
			param = &(m_utemp[i-(WCHDATA_JXJMuchanglv1)]);
		}
		break;
	case WCHDATA_JXJLingdilv:
		param = &m_lingdilv;
		break;
	case WCHDATA_JXJLingdilv1:
	case WCHDATA_JXJLingdilv2:
	case WCHDATA_JXJLingdilv3:
	case WCHDATA_JXJLingdilv4:
		{//使用时候注意不能修改
			_crInt16 lo16 = LOINT32(m_lingdilv);
			_crInt16 hi16 = HIINT32(m_lingdilv);
			m_utemp[0] = (unsigned char)LOINT16(lo16);
			m_utemp[1] = (unsigned char)HIINT16(lo16);
			m_utemp[2] = (unsigned char)LOINT16(hi16);
			m_utemp[3] = (unsigned char)HIINT16(hi16);
			param = &(m_utemp[i-(WCHDATA_JXJLingdilv1)]);
		}
		break;
	case WCHDATA_JXJKuangshanlv:
		param = &m_kuangshanlv;
		break;
	case WCHDATA_JXJKuangshanlv1:
	case WCHDATA_JXJKuangshanlv2:
	case WCHDATA_JXJKuangshanlv3:
	case WCHDATA_JXJKuangshanlv4:
		{//使用时候注意不能修改
			_crInt16 lo16 = LOINT32(m_kuangshanlv);
			_crInt16 hi16 = HIINT32(m_kuangshanlv);
			m_utemp[0] = (unsigned char)LOINT16(lo16);
			m_utemp[1] = (unsigned char)HIINT16(lo16);
			m_utemp[2] = (unsigned char)LOINT16(hi16);
			m_utemp[3] = (unsigned char)HIINT16(hi16);
			param = &(m_utemp[i-(WCHDATA_JXJKuangshanlv1)]);
		}
		break;
	case WCHDATA_UpdateTimer:
		param = &m_updateTimer;
		break;
	case WCHDATA_JXJBuildingTimes:
		param = &m_buildingTimes;
		break;
	case WCHDATA_JXJBuildingTime0:
		param = &m_buildingTimes[0];
		break;
	case WCHDATA_JXJBuildingTime1:
		param = &m_buildingTimes[1];
		break;
	case WCHDATA_JXJBuildingTime2:
		param = &m_buildingTimes[2];
		break;
	case WCHDATA_JXJBuildingTime3:
		param = &m_buildingTimes[3];
		break;
	case WCHDATA_JXJRecruitTimes:
		param = &m_recruitTimes;
		break;
	case WCHDATA_JXJRecruitTime0:
		param = &m_recruitTimes[0];
		break;
	case WCHDATA_JXJRecruitTime1:
		param = &m_recruitTimes[1];
		break;
	case WCHDATA_JXJRecruitTime2:
		param = &m_recruitTimes[2];
		break;
	case WCHDATA_JXJRecruitTime3:
		param = &m_recruitTimes[3];
		break;
	case WCHDATA_JXJKillEachTroopsCount:
		param = &m_ckilleachtroops;
		break;
	case WCHDATA_JXJDeadEachTroopsCount:
		param = &m_cdeadeachtroops;
		break;
	case WCHDATA_JXJDanDhangKillCount:
		param = &m_danchangkill;
		break;
	case WCHDATA_JXJKillDoorBossCount:
		param = &m_killdoorboss;
		break;
	case WCHDATA_JXJJunling:
		param = &m_junling;
		break;
	//case WCHDATA_JXJMaxTroops:
	//	param = &m_maxtroops;
	//	break;
	case WCHDATA_JXJTroopsMap:
		param = &m_troopsMap;
		break;
	case WCHDATA_JXJTroopsEquipMap:
		param = &m_troopsEquipMap;
		break;
	case WCHDATA_JXJItemBagVec:
		param = &m_itembagvec;
		break;
	//case WCHDATA_JXJTechJingBinglv:
	//	param = &m_jingbinglv;
	//	break;
	//case WCHDATA_JXJTechJianJialv:
	//	param = &m_jianjialv;
	//	break;
	//case WCHDATA_JXJTechQiangRenlv:
	//	param = &m_qiangrenlv;
	//	break;
	//case WCHDATA_JXJTechGuiMoulv:
	//	param = &m_guimoulv;
	//	break;
	//case WCHDATA_JXJTechGongChenglv:
	//	param = &m_gongchenglv;
	//	break;
	//case WCHDATA_JXJTechYuanGonglv:
	//	param = &m_yuangonglv;
	//	break;
	//case WCHDATA_JXJTechSuGonglv:
	//	param = &m_sugonglv;
	//	break;
	//case WCHDATA_JXJTechQiangXinglv:
	//	param = &m_qiangxinglv;
	//	break;
	//case WCHDATA_JXJTechZhaoMulv:
	//	param = &m_zhaomulv;
	//	break;
	//case WCHDATA_JXJTechRenDelv:
	//	param = &m_rendelv;
	//	break;
	//case WCHDATA_JXJTechlv2Tab:
	//	param = &m_masterlv;
	//	break;
	//case WCHDATA_JXJInfantrylv:
	//	param = &m_infantrylv;
	//	break;
	//case WCHDATA_JXJCavalrylv:
	//	param = &m_cavalrylv;
	//	break;
	//case WCHDATA_JXJArcherlv:
	//	param = &m_archerlv;
	//	break;
	//case WCHDATA_JXJSiegelv:
	//	param = &m_siegelv;
	//	break;
	//case WCHDATA_JXJShieldlv:
	//	param = &m_shieldlv;
	//	break;
	//case WCHDATA_JXJGuardlv:
	//	param = &m_guardlv;
	//	break;
	//case WCHDATA_JXJSaddlelv:
	//	param = &m_saddlelv;
	//	break;
	//case WCHDATA_JXJAssaultlv:
	//	param = &m_assaultlv;
	//	break;
	//case WCHDATA_JXJAssistlv:
	//	param = &m_assistlv;
	//	break;
	//case WCHDATA_JXJPenetratelv:
	//	param = &m_penetratelv;
	//	break;
	//case WCHDATA_JXJManufacturelv:
	//	param = &m_manufacturelv;
	//	break;
	//case WCHDATA_JXJGunpowderlv:
	//	param = &m_gunpowderlv;
	//	break;
	//case WCHDATA_JXJBravelv:
	//	param = &m_bravelv;
	//	break;
	//case WCHDATA_JXJIroncavalrylv:
	//	param = &m_ironcavalrylv;
	//	break;
	//case WCHDATA_JXJGearlv:
	//	param = &m_gearlv;
	//	break;
	//case WCHDATA_JXJFireboltlv:
	//	param = &m_fireboltlv;
	//	break;
	//case WCHDATA_JXJMasterlv:
	//	param = &m_masterlv;
	//	break;
	//case WCHDATA_JXJJuntunlv:
	//	param = &m_juntunlv;
	//	break;
	//case WCHDATA_JXJReformlv:
	//	param = &m_reformlv;
	//	break;
	//case WCHDATA_JXJSignin:
	//	param = &m_signin;
	//	break;
	case WCHDATA_JXJFormationInfoVec:
		param = &m_formationInfoVec;
		break;
	case WCHDATA_JXJGoldCostHintInt:
		param = &m_goldcosthint;
		break;
	case WCHDATA_JXJFriendList:
		param = &m_friendList;
		break;
	case WCHDATA_JXJMyTeam:
		param = &m_myTeam;
		break;
	case WCHDATA_JXJQueryTeamVec:
		param = &m_queryTeamVec;
		break;
	//case WCHDATA_JXJQueryTeamIndex:
	//	param = &m_queryTeamIndex;
	//	break;
	//case WCHDATA_JXJHalltimer:
	//	param = &m_halltimer;
	//	break;
	//case WCHDATA_JXJPrivatetimer:
	//	param = &m_privatetimer;
	//	break;
	//case WCHDATA_JXJHallCard:
	//	param = &m_hallCard[0];
	//	break;
	//case WCHDATA_JXJPrivateCard:
	//	param = &m_privateCard[0];
	//	break;
	case WCHDATA_JXJLevyCount:
		param = &m_levyCount;
		break;
	//case WCHDATA_JXJLevyTimer:
	//	param = &m_levyTimer;
	//	break;
	case WCHDATA_JXJOnlineRewardCount:
		param = &m_onlineRewardCount;
		break;
	case WCHDATA_JXJOnlineRewardTimer:
		param = &m_onlineRewardTimer;
		break;
	case WCHDATA_JXJPlayerOnlineRewardTimer:
		param = &m_playerOnlineRewardTimer;
		break;
	//case WCHDATA_JXJClientTimer:
	//	param = &m_clientTimer;
	//	break;
	case WCHDATA_JXJBuyJunlingCount:
		param = &m_buyJunlingCount;
		break;
	case WCHDATA_JXJBuyJunlingTimer:
		param = &m_buyJunlingTimer;
		break;
	case WCHDATA_JXJSaleList:
		param = &m_saleList;
		break;
	case WCHDATA_JXJOpenBattle:
		param = &m_openBattle;
		break;
	case WCHDATA_JXJBattleTimer:
		param = &m_battleTimer;
		break;
	//case WCHDATA_JXJOpenPass:
	//	param = &m_openPass;
	//	break;
	//case WCHDATA_JXJPassTimer:
	//	param = &m_passTimer;
	//	break;
	case WCHDATA_JXJChengChiMap:
		param = &m_chengchiMap;
		break;
	case WCHDATA_JXJInviteType:
		param = &m_inviteType;
		break;
	case WCHDATA_JXJInviteID:
		param = &m_inviteID;
		break;
	case WCHDATA_JXJInviteChengChiID:
		param = &m_inviteChengchiID;
		break;
	case WCHDATA_JXJFubenOrChengchiType:
		param = &m_fubenOrChengchiType;
		break;
	case WCHDATA_JXJFubenOrChengchiID:
		param = &m_fubenOrChengchiID;
		break;
	case WCHDATA_JXJSaleItemList:
		param = &m_saleitemlist;
		break;
	case WCHDATA_JXJOpenBagNum:
		param = &m_openbagnum;
		break;
	case WCHDATA_JXJTeamLeaderID:
		param = &m_teamLeaderID;
		break;
	case WCHDATA_JXJBattleRoomID:
		param = &m_battleRoomID;
		break;
	case WCHDATA_JXJCurFubenProgress:
		param = &m_curFubenProgress;
		break;
	//case WCHDATA_JXJLuckyCardCopperCost:
	//	param = &m_luckycardcoppercost;
	//	break;
	//case WCHDATA_JXJLuckyCardGoldCost:
	//	param = &m_luckycardgoldcost;
	//	break;
	case WCHDATA_JXJJiangkaBagVec:
		param = &m_jiangkabagvec;
		break;
	case WCHDATA_JXJFubenProgressMap:
		param = &m_fubenProgressMap;
		break;
	case WCHDATA_JXJItemObjectMap:
		param = &m_itemObjectMap;
		break;
	case WCHDATA_JXJExtraProduceMap:
		param = &m_extraProduceMap;
		break;
	case WCHDATA_JXJAppointJiangkaBagVec:
		param = &m_appointjiangkabagvec;
		break;
	case WCHDATA_JXJ7DayData:
		param = &m_7DayData;
		break;
	case WCHDATA_JXJLuckyCard:
		param = &m_luckycard[0];
		break;
	//case WCHDATA_JXJKnowLuckyCard:
	//	param = &m_knowluckycard;
	//	break;
	//case WCHDATA_JXJJiusiRefreshCostTime:
	//	param = &m_jiusirefreshtime;
	//	break;
	case WCHDATA_JXJVipLv:
		param = &m_viplv;
		break;
	case WCHDATA_JXJVipExp:
		param = &m_vipExp;
		break;
	case WCHDATA_JXJFubenRewardInfo:
		param = m_fubenReward.get();
		break;
	case WCHDATA_JXJFubenReliveCount:
		param = &m_reliveCount;
		break;
	case WCHDATA_JXJClientDesposeEventDeque:
		param = &m_clienteventdeq;
		break;
	case WCHDATA_JXJCountryContribute:
		param = &m_countrycontribute;
		break;
	case WCHDATA_JXJCardImpression:
		param = &m_cardimpression;
		break;
	case WCHDATA_JXJCardImpressionLevel:
		param = &m_cardImpressionLevel;
		break;
	case WCHDATA_JXJBuyResourceCount:
		param = &m_resourceCount;
		break;
	case WCHDATA_JXJFreeBuyInfo:
		param = &m_freeBuy;
		break;
	case WCHDATA_JXJGoldRecruitCount:
		param = &m_goldRecruitCount;
		break;
	case WCHDATA_JXJCardPieces:
		param = &m_cardpieces;
		break;
	case WCHDATA_JXJChatRecordMap:
		param = &m_chatRecordMap;
		break;
	case WCHDATA_JXJChatItemLinkMap:
		param = &m_chatitemlinkIdMap;
		break;
	case WCHDATA_JXJChatInviteLinkMap:
		param = &m_chatinviteIdMap;
		break;
	case WCHDATA_JXJChatBattleReportLinkMap:
		param = &m_chatBattleReportIDMap;
		break;
	case WCHDATA_JXJChatPraiseLinkMap:
		param = &m_chatPraiseIDMap;
		break;
	case WCHDATA_JXJChatCallForHelpLinkMap:
		param = &m_chatCallForHelpLinkIdMap;
		break;
	case WCHDATA_JXJFreeCounterinsurgencyCount:
		param = &m_freeCounterinsurgency;
		break;
	case WCHDATA_JXJRefineEquipFreeCount:
		param = &m_RefineEquipFreeCount;
		break;
	case WCHDATA_JXJRebelCityVec:
		param = &m_rebelCityVec;
		break;
	case WCHDATA_JXJRebelTimer:
		param = &m_rebelTimer;
		break;
	case WCHDATA_JXJVIPNongTianShuiLi:
		param = &m_NongTianShuiLilv;
		break;
	case WCHDATA_JXJVIPZhiShuZaoLin:
		param = &m_ZhiShuZaoLinlv;
		break;
	case WCHDATA_JXJVIPYouZhiCaoChang:
		param = &m_YouZhiCaoChanglv;
		break;
	case WCHDATA_JXJVIPJingKuangShenWa:
		param = &m_JingKuangShenWalv;
		break;
	case WCHDATA_JXJVIPGuLiMaoYi:
		param = &m_GuLiMaoYilv;
		break;
	case WCHDATA_JXJVIPZhengShouXinDe:
		param = &m_ZhengShouXinDelv;
		break;
	case WCHDATA_JXJVIPYanBingYueWu:
		param = &m_YanBingYueWulv;
		break;
	case WCHDATA_JXJVIPXunBingXinDe:
		param = &m_XunBingXinDelv;
		break;
	case WCHDATA_JXJVIPZongMiaoJiSi:
		param = &m_ZongMiaoJiSilv;
		break;
	case WCHDATA_JXJVIPJiSiXinDe:
		param = &m_JiSiXinDelv;
		break;
	case WCHDATA_MarketBuyResourceCount:
		param = &m_MarketBuyResourceMax;
		break;
	case WCHDATA_JXJRefineMagicID:
		param = &m_refinemagicid;
		break;
	case WCHDATA_JXJCenterNotice:
		param = m_cfgstream.get();
		break;
	case WCHDATA_JXJOldCenterNotice:
		param = m_oldcfgstream.get();
		break;
	case WCHDATA_JXJRollCardFreeCount:
		param = &m_FreeRollCardCount;
		break;
	case WCHDATA_JXJPlayerStore:
		param = m_playerStore.get();
		break;
	case WCHDATA_JXJPlayerCycleCount:
		param = &m_playerCycleCount;
		break;
	case WCHDATA_JXJAutoSellCard:
		param = &m_autosellcard;
		break;	
	case WCHDATA_JXJTroopsTechMap:
		param = &m_troopstechmap;
		break;
	case WCHDATA_JXJCardExtraTechMap:
		param = &m_cardExtraTechMap;
		break;
	case WCHDATA_JXJBuyShangXianMap:
		param = &m_buyshagnxianmap;
		break;
	case WCHDATA_JXJTroopsAchieveInfo:
		param = &m_troopsachievemap;
		break;
	case WCHDATA_JXJCardImpression2CritTechLv:
		param = &m_CardImpression2CritTechLv;
		break;
	case WCHDATA_JXJCardImpression4CritTechLv:
		param = &m_CardImpression4CritTechLv;
		break;
	case WCHDATA_JXJCardImpression6CritTechLv:
		param = &m_CardImpression6CritTechLv;
		break;
	case WCHDATA_JXJCardImpression10CritTechLv:
		param = &m_CardImpression10CritTechLv;
		break;
	case WCHDATA_JXJFreeRollCardTechLv:
		param = &m_FreeRollCardTechLv;
		break;
	case WCHDATA_JXJOrangeCardUpgradeTechLv:
		param = &m_OrangeCardUpgradeTechLv;
		break;
	case WCHDATA_JXJCardShopOpenTechLv:
		param = &m_CardShopOpenTechLv;
		break;
	case WCHDATA_JXJMakeEquipTechLv:
		param = &m_MakeEquipTechLv;
		break;
	case WCHDATA_JXJEquipRefineTechLv:
		param = &m_EquipRefineTechLv;
		break;
	case WCHDATA_JXJEquipAttachTechLv:
		param = &m_EquipAttachTechLv;
		break;
	case WCHDATA_JXJSmeltEquipTechLv:
		param = &m_SmeltEquipTechLv;
		break;
	case WCHDATA_JXJSmeltMaterialTechLv:
		param = &m_SmeltMaterialTechLv;
		break;
	case WCHDATA_JXJSmeltStoneTechLv:
		param = &m_SmeltStoneTechLv;
		break;
	case WCHDATA_JXJAddRefineFreeCount1TechLv:
		param = &m_EquipRefineMaxCount1TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeCount2TechLv:
		param = &m_EquipRefineMaxCount2TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeCount3TechLv:
		param = &m_EquipRefineMaxCount3TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeCount4TechLv:
		param = &m_EquipRefineMaxCount4TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeProduce1TechLv:
		param = &m_EquipRefineProduce1TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeProduce2TechLv:
		param = &m_EquipRefineProduce2TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeProduce3TechLv:
		param = &m_EquipRefineProduce3TechLv;
		break;
	case WCHDATA_JXJAddRefineFreeProduce4TechLv:
		param = &m_EquipRefineProduce4TechLv;
		break;
	case WCHDATA_JXJFreeRollCard2TechLv:
		param = &m_FreeRollCard2TechLv;
		break;
	case WCHDATA_JXJFreeRollCard3TechLv:
		param = &m_FreeRollCard3TechLv;
		break;
	case WCHDATA_JXJFreeRollCardProduce1TechLv:
		param = &m_FreeRollCardProduce1TechLv;
		break;
	case WCHDATA_JXJFreeRollCardProduce2TechLv:
		param = &m_FreeRollCardProduce2TechLv;
		break;
	case WCHDATA_JXJFreeRollCardProduce3TechLv:
		param = &m_FreeRollCardProduce3TechLv;
		break;
	case WCHDATA_JXJFreeRollCardProduce4TechLv:
		param = &m_FreeRollCardProduce4TechLv;
		break;
	case WCHDATA_JXJFuKuMaxTechLv:
		param = &m_FuKuMaxTechLv;
		break;
		///////////////////////////////////
	case WCHDATA_JXJOpenSaveFormationInfo:
		param = &m_opensavedformationifo;
		break;
	case WCHDATA_JXJPlayerChangeShiliTimes:
		param = &m_changeShiliTimes;
		break;
	case WCHDATA_JXJSeriesLandDay:
		param = &m_seriesLandDays;
		break;
	case WCHDATA_JXJLandRewardGetCount:
		param = &m_LandRewardGetCount;
		break;
	case WCHDATA_JXJLandRewardDataInfo:
		param = m_lanRewardData.get();
		break;
	case WCHDATA_JXJBlackNameSet:
		param = &m_blacknameset;
		break;
	case WCHDATA_JXJTechDevelopMap:
		param = &m_techdevelopmap;
		break;
	case WCHDATA_JXJClientIsServerCycling:
		param = &m_bisCycling;
		break;
	case WCHDATA_JXJClientCycleTimer:
		param = &m_iClientCycleTime;
		break;
	//case WCHDATA_JXJPersuadeDeq:
	//	param = &m_persuadeInfoDeq;
	//	break;
	case WCHDATA_JXJSavedFormationInfoVec:
		param = &m_savedformationinfovec;
		break;
	case WCHDATA_JXJLandRewardClientTime:
		param = &m_nLandRewardTime;
		break;
	case WCHDATA_JXJCountryBenefitFeteTimes		:
		param = & m_feteTimes;
		break;
	case WCHDATA_JXJCountryBenefitEnrichType		:
		param = & m_enrichType;
		break;
	case WCHDATA_JXJCountryBenefitEnrichTimes	:
		param = & m_enrichTimes;
		break;
	case WCHDATA_JXJLunHuiRewardMap:
		param = &m_lunhuiRewardMap;
		break;
	case WCHDATA_JXJAddGroupMemberRequestDeq:
		param = & m_addGroupMemberRequestDeq;
		break;
	case WCHDATA_JXJSkipNewHand:
		param = & m_skipnewhand;
		break;
	case WCHDATA_JXJSelectLeaderSet:
		param = & m_selectLeaderIdSet;
		break;
	case WCHDATA_JXJConsortiaID:
		param = &m_consortiaid;
		break;
	case WCHDATA_JXJMyApplyGroupIDSet:
		param = &m_myGroupApplySet;
		break;
	case WCHDATA_JXJSavedFormationIndex:
		param = &m_savedformationindex;
		break;
	case WCHDATA_JXJIfTechBtnNew:
		param = &m_iftechbtnnew;
		break;
	case WCHDATA_JXJHadWonGroupFuBenSet	:
		param = & m_hadWonGroupFuBenSet;
		break;
	case WCHDATA_JXJGroupContributeiType:
		param = & m_groupContributeiType;
		break;
	case WCHDATA_JXJGroupContributeiTimes:
		param = & m_groupContributeiTimes;
		break;
	case WCHDATA_JXJFuKuMaxTech2Lv:
		param = & m_FuKuMaxTech2Lv;
		break;
	case WCHDATA_JXJFuKuMaxTech3Lv:
		param = & m_FuKuMaxTech3Lv;
		break;
	case WCHDATA_JXJInspireLevel:
		param = & m_inspireLv;
		break;

	case WCHDATA_JXJMyCountryData:
		param = m_myShili.get();
		break;
	case WCHDATA_JXJChatChannelSetInfo:
		param = & m_chatChannelSetInfo;
		break;
	//case WCHDATA_JXJServerVersion				:
	//	param = & m_serverVersion;
	//	break;
	case WCHDATA_JXJCenterNoticeRefershTime:
		param  = &m_flushTime;
		break;
	case WCHDATA_JXJNewFunctionOpenSet:
		param = &m_newfunctionopenset;
		break;
	case WCHDATA_JXJQueryedBattleReportInfo:
		param = &m_battleReportInfoMap;
		break;
	case WCHDATA_JXJCurQueryBattleReportID:
		param = &m_battleReportQueryedId;
		break;
	case WCHDATA_JXJLingqufengluFlag:
		param = &m_lingqufengluflag;
		break;
	//case WCHDATA_JXJMainRoleRefreshFlag:
	//	param = &m_mainrolerefreshflag;
	//	break;
	case WCHDATA_JXJTechFoodlv:
		param = &m_tuntianlv;
		break;
	case WCHDATA_JXJTechHorselv:
		param = &m_fanzhilv;
		break;
	case WCHDATA_JXJTechIronlv:
		param = &m_yelianlv;
		break;
	case WCHDATA_JXJTechWoodlv:
		param = &m_fanmaolv;
		break;
	case WCHDATA_JXJTechBuildingCDlv:
		param = &m_qiaojianglv;
		break;
	case WCHDATA_JXJTechZhengShuilv:
		param = &m_zhengshuilv;
		break;
	case WCHDATA_JXJTechFengluLv:
		param = &m_techfenglulv;
		break;
	case WCHDATA_JXJTechJiusibaojiLv:
		param = &m_techjiusibaojilv;
		break;
	case WCHDATA_JXJTechWorldTalkPriceLv:
		param = &m_techworldtalklv;
		break;
	case WCHDATA_JXJTechBuyJunlingLv:
		param = &m_techbuyjunlinglv;
		break;
	case WCHDATA_JXJFoundCountryGetAward			:
		param = &m_foundCountryAward;
		break;
	case WCHDATA_JXJPlayerTotalPayGold:
		param = &m_playertotalpaygold;
		break;
	case WCHDATA_JXJShuichelv:
		param = &m_shuichelv;
		break;
	case WCHDATA_JXJRonglulv:
		param = &m_ronglulv;
		break;
	case WCHDATA_JXJGongshelv:
		param = &m_gongshelv;
		break;
	case WCHDATA_JXJMochuanlv:
		param = &m_mochuanlv;
		break;
	case WCHDATA_JXJPlayerLoginDays:
		param = & m_loginDays;
		break;
	case WCHDATA_JXJ7DaysGetAward:
		param = & m_7DaysAward;
		break;
	case WCHDATA_JXJShouchong:
		param = & m_shouchong;
		break;
	case WCHDATA_JXJZhengZhanQuan:
		param = & m_cZhengZhanQuan;
		break;
	case WCHDATA_JXJKillPlayerCount:
		param = & m_killplayercount;
		break;
	case WCHDATA_JXJKillNpcCount:
		param = & m_killnpccount;
		break;
	case WCHDATA_JXJDayPlayerDeadCount:
		param = & m_dayplayerdeadcount;
		break;
	case WCHDATA_JXJPlayerDeadCount:
		param = &m_playerdeadcount;
		break;
	case WCHDATA_JXJDayKillPlayerCount:
		param = & m_daykillplayercount;
		break;
	case WCHDATA_JXJDayKillNPCCount:
		param = & m_daykillnpccount;
		break;
	case WCHDATA_JXJDayKillEachTroops:
		param = & m_dayckilleachtroops;
		break;
	case WCHDATA_JXJUsedAchievePoints:
		param = & m_usedachievepoint;
		break;
	case WCHDATA_JXJPassMingJiangCount:
		param = & m_passmingjiangcount;
		break;
	case WCHDATA_JXJPassWorldFuBenCount:
		param = & m_Cpassworldfuben;
		break;
	case WCHDATA_JXJMingJiangFailCount:
		param = & m_cmingjiangfail;
		break;
	case WCHDATA_JXJZhengZhanTianXiaFailCount:
		param = & m_ctianxiafail;
		break;
	case WCHDATA_JXJPlayerHaveGotViplvReward:
		param = & m_havegotviplv;
		break;
	case WCHDATA_JXJPassZhengZhanTianXiaCount:
		param = & m_Cpasstianxia;
		break;
	case WCHDATA_JXJFinishHuoYueDuDayCount:
		param = & m_cfinishdays;
		break;
	case WCHDATA_JXJAchievePointsCount:
		param = & m_accomplishPointCache;
		break;
	case WCHDATA_JXJHavePraiseCounts:
		param = & m_praisedcount;
		break;
	case WCHDATA_JXJPraiseOtherCounts:
		param = & m_praisecount;
		break;
	case WCHDATA_JXJGuoZhanJiFen:
		param = & m_cGuoZhanJiFen;
		break;
	case WCHDATA_JXJTreasonBufferTimer:
		param = & m_treasonbuffertimer;
		break;
	case WCHDATA_JXJTreasonPunishTimer:
		param = & m_treasonpunishtimer;
		break;
	case WCHDATA_JXJTreasonCoolingTimer:
		param = & m_treasoncoolingtimer;
		break;
	case WCHDATA_JXJPlayerSelectedShiLi:
		param = & m_selectedshili;
		break;
	case WCHDATA_JXJAmHuoDongGetTimes:
		param = & m_timesshangwu;
		break;
	case WCHDATA_JXJPmHuoDongGetTimes:
		param = & m_timesxiawu;
		break;
	case WCHDATA_JXJNightHuoDongGetTimes:
		param = & m_timeswanshang;
		break;
	case WCHDATA_JXJPlayerPraiseLimit:
		param = & m_praiselimit;
		break;
	case WCHDATA_JXJPlayerGuanZhi:
		param = & m_guanzhi;
		break;
	case WCHDATA_JXJYueKaID:
		param = & m_yuekaid;
		break;
	case WCHDATA_JXJYueKaEndTime:
		param = & m_yuekaendtime;
		break;
	case WCHDATA_JXJYueKaLastGetTime:
		param = & m_lastgettime;
		break;
	case WCHDATA_JXJKaoShangFuUsedTime:
		param = & m_kaoshangfutime;
		break;
	case WCHDATA_JXJSpecialtyRefreshTimes:
		param = & m_specialtyrefreshtimes;
		break;
	case WCHDATA_JXJLastChongZhiTime:
		param = & m_lastchongzhitime;
		break;
	case WCHDATA_JXJLastZeroRefreshTime:
		param = & m_lastzeroRefreshtime;
		break;
	case WCHDATA_JXJChongZhiCountT:
		param = & m_chongzhicountT;
		break;
	case WCHDATA_JXJHadWonJunTuanFBT:
		param = & m_hadwonjuntuanfubenT;
		break;
	case WCHDATA_JXJIsCheckHuoYueDuSign:
		param = & m_huoyuedusign;
		break;
	case WCHDATA_JXJGainedZhouKaID:
		param = & m_gainedzhoukahuodongid;
		break;
	case WCHDATA_JXJZhouKaEndTime:
		param = & m_zhoukaendtime;
		break;
	case WCHDATA_JXJZhouKaGetStatus:
		param = & m_zhoukagetstatus;
		break;
	case WCHDATA_JXJZhouKaGetDayIdx:
		param = & m_zhoukagetdayidx;
		break;
	case WCHDATA_JXJItemEndUsedTime:
		param = & m_itemusedtimeMap;
		break;
	case WCHDATA_JXJGroupCantonSpecialtyVec:
		param = &m_groupCantonSpecialtyVec;
		break;
	case WCHDATA_JXJPlayerDataMonitor:
		{
			if(!m_playerDataMonitor.valid())
				m_playerDataMonitor = new crJXJPlayerDataMonitor;
			param = m_playerDataMonitor.get();
		}
		break;
	case WCHDATA_JXJBattleActiveTimer:
		param = &m_battleActiveTimer;
		break;
	case WCHDATA_JXJBattleValidateID:
		param = &m_battleValidateID;
		break;
	case WCHDATA_JXJElector:
		param = &m_elector;
		break;
	case WCHDATA_JXJVoteTimes:
		param = &m_vote;
		break;
	case WCHDATA_JXJCandidate:
		param = &m_beElected;
		break;
	case WCHDATA_JXJCountryRecommendSet:
		param = & m_recommendSet;
		break;
	case WCHDATA_JXJElectIconTimeControl:
		param = &m_electIcon;
		break;
	case WCHDATA_JXJSelectedIdVec:
		param = &m_SelectedIdVec;
		break;
	case WCHDATA_JXJCancelCantonTimer:
		param = &m_cancelCantonTimer;
		break;
	case WCHDATA_JXJHadShowDismissTip:
		param = &m_hadshowdismisstip;
		break;
	case WCHDATA_JXJMyFriendLordEquipVec:
		param = &m_friendEquipVec;
		break;
	case WCHDATA_JXJUsedZhengZhaoTimes:
		param = &m_zhengZhaoTimes;
		break;
	case WCHDATA_JXJCheckIsMyself:
		param = &m_myself;
		break;
	case WCHDATA_JXJGuoZhanZhengzhaolingVec:
		param = &m_guanyuanlingVec;
		break;
	case WCHDATA_JXJJunTuanZhengzhaolingVec:
		param = &m_juntuanlingVec;
		break;
	case WCHDATA_JXJCurZhengzhaolingVec:
		param = &m_CurZhengzhaolingVec;
		break;
	case WCHDATA_JXJCurZhengzhaolingPage:
		param = &m_pageNum;
		break;
	case WCHDATA_JXJZhengzhaolingUpdateTimer:
		param = &m_zhengZhaolingUpdateTimer;
		break;
	case WCHDATA_JXJGuoJiaReleaseTimer:
		param = &m_GuoJiaReleaseTimer;
		break;
	case WCHDATA_JXJJunTuanReleaseTimer:
		param = &m_JunTuanReleaseTimer;
		break;
	case WCHDATA_JXJCheckYingzhengReward:
		param = &m_CheckYingzhengReward;
		break;
	case WCHDATA_JXJGuoZhanAttackVictoryTimes:
		param = &m_attackVictoryTimes;
		break;
	case WCHDATA_JXJGuoZhanDefendVictoryTimes:
		param = &m_defendVictoryTimes;
		break;
	case WCHDATA_JXJCountryBattleTimes:
		param = &m_countryBattleTimes;
		break;
	case WCHDATA_JXJAcceptZhengZhaoEnterBroadcast:
		param = &m_releaseName;
		break;
	case WCHDATA_JXJChooseAutoBattleMode:
		param = &m_autoBattle;
		break;
	case WCHDATA_JXJGameIntroductionCurPageNum:
		param = &m_curPageNum_Introduction;
		break;
	case WCHDATA_JXJHadGottenConsortiaPromotionReward:
		param = &m_hadGottenReward;
		break;
	case WCHDATA_JXJConsortiaPromotionMyRankVal:
		param = &m_myRankVal;
		break;
	case WCHDATA_JXJGameStartedDays:
		param = &m_startedDays;
		break;
	case WCHDATA_JXJCouldGetAwardMap:
		param = &m_couldGetAwardMap;
		break;
	case WCHDATA_JXJGameIntroductionNoTipsCheck:
		param = &m_gameIntroductionNoTips;
		break;
	case WCHDATA_JXJHadShowFestivalHuodongCanvas:
		param = &m_hadShowFestivalHuodongCanvas;
		break;
	case WCHDATA_JXJOnlineTimer:
		param = &m_onlinetimer;
		break;
	case WCHDATA_JXJAutoBuilding:
		param = &m_autoBuilding;
		break;
	case WCHDATA_JXJBuildCountryTimes:
		param = &m_buildCountryTimes;
		break;
	case WCHDATA_JXJAchieveShopMap:
		param = &m_achieveShopMap;
		break;
	case WCHDATA_JXJGovShopMap:
		param = &m_govShopMap;
		break;
	case WCHDATA_JXJForeignCard:
		param = &m_foreigncard;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crJXJBrainData
//
/////////////////////////////////////////
crJXJBrainData::crJXJBrainData():
	m_curPlayerIconID(0),
	m_curRecruitArmID(0),
	m_curSelRecruitArmAID(0),
	m_curFormationArmID(0),
	m_curSelFormationArmAID(0),
	m_curCDHintID(0),
	m_curTimesID(0),
	m_curSalePageNum(0),
	m_saleTotal(0),
	m_mycurSalePageNum(1),
	m_mysaleTotal(0),
	m_saleitemid(0),
	m_saleitemmax(0),
	m_curFriendPageNum(1),
	m_totalFriendPageNum(1),
	m_curJiangkaBagPageNum(1),
	m_totalJiangkaBagPageNum(1),
	m_JiangkaFormationRow(-1),
	m_NewsCanvasStep(-1.0f),
	m_IfSetArmyNum(true),
	m_rtt_refresh(true),
	m_cur_canvas(NULL),
	m_autoequipscount(0),
	m_curinvitepagenum(1),
	m_totalinvitepagenum(1),
	//m_curinvitetype(0),
	m_CurForgeEquipID(0),
	//m_CurSelForgeEquipID(0),
	m_cursalecardpagenum(1),
	m_totalsalecardpagenum(1),
	m_salejiangkaindex(-1),
	m_newjiangkaid(0),
	m_checkmailindex(0),
	m_deleteallmailflag(0),
	m_curformationpagenum(1),
	///
	m_rankingpagenum(1),
	m_rankingtotal(1),
	m_BrainRecruitCount(0),
	m_BrainResourcesCardCount(0),
	m_BrainRushTaskCount(0),
	m_totalformationpagenum(1),
	m_unfoldtasktype1(false),
	m_unfoldtasktype2(false),
	m_unfoldtasktype3(false),
	m_unfoldtasktype4(false),
	m_curselecttaskid(0),
	m_GongfangLordPaperCount(0),
	m_useitemremaincount(0),
	m_PingpanRemainCount(0),
	m_rollcardremaincount(0),
	m_msOnBulidingNode(NULL),
	m_goldcosthinttype(-1),
	m_friendorblack(0),
	m_ifEndSendPlayerData(0),
	m_remainfubenrushcount(0),
	m_hideModel(0),
	m_onlineDongHuaVisiable(false),
	m_autoBattleSetting(0),
	m_newguidestep(0),
	m_newguideid(0),
	m_totalrollcardcount(0),
	m_totalupgradecardcount(0),
	m_totalmakelordequipcount(0),
	//m_newhandtaskscrollbar(0),
	m_showVersion(false),
	m_totalexchangerescount(0),
	m_totallevycount(0),
	m_delayHideWidgetTime(0)
{
}
crJXJBrainData::crJXJBrainData(const crJXJBrainData& data):
	crBrainData(data),
	m_curPlayerIconID(data.m_curPlayerIconID),
	m_curRecruitArmID(data.m_curRecruitArmID),
	m_curSelRecruitArmAID(data.m_curSelRecruitArmAID),
	m_curFormationArmID(data.m_curFormationArmID),
	m_curSelFormationArmAID(data.m_curSelFormationArmAID),
	m_curCDHintID(data.m_curCDHintID),
	m_curTimesID(data.m_curTimesID),
	m_curSalePageNum(data.m_curSalePageNum),
	m_saleTotal(data.m_saleTotal),
	m_mycurSalePageNum(data.m_mycurSalePageNum),
	m_mysaleTotal(data.m_mysaleTotal),
	m_equipID(data.m_equipID),
	m_curFriendPageNum(data.m_curFriendPageNum),
	m_totalFriendPageNum(data.m_totalFriendPageNum),
	m_addfriendname(data.m_addfriendname),
	m_saleitemid(data.m_saleitemid),
	m_saleitemmax(data.m_saleitemmax),
	m_curJiangkaBagPageNum(data.m_curJiangkaBagPageNum),
	m_totalJiangkaBagPageNum(data.m_totalJiangkaBagPageNum),
	m_JiangkaFormationRow(data.m_JiangkaFormationRow),
	m_NewsCanvasStep(data.m_NewsCanvasStep),
	m_IfSetArmyNum(data.m_IfSetArmyNum),
	m_rtt_refresh(data.m_rtt_refresh),
	m_rtt_widget(data.m_rtt_widget),
	m_cur_canvas(data.m_cur_canvas),
	m_autoequipscount(data.m_autoequipscount),
	m_curinvitepagenum(data.m_curinvitepagenum),
	m_totalinvitepagenum(data.m_totalinvitepagenum),
	//m_curinvitetype(data.m_curinvitetype),
	m_CurForgeEquipID(data.m_CurForgeEquipID),
	//m_CurSelForgeEquipID(data.m_CurSelForgeEquipID),
	m_salebagvec(data.m_salebagvec),
	m_cursalecardpagenum(data.m_cursalecardpagenum),
	m_totalsalecardpagenum(data.m_totalsalecardpagenum),
	m_salejiangkaindex(data.m_salejiangkaindex),
	m_newjiangkaid(data.m_newjiangkaid),
	m_checkmailindex(data.m_checkmailindex),
	m_recvmailvec(data.m_recvmailvec),
	m_recvmailbodyMap(data.m_recvmailbodyMap),
	m_deleteallmailflag(data.m_deleteallmailflag),
	m_curformationpagenum(data.m_curformationpagenum),
	m_totalformationpagenum(data.m_totalformationpagenum),
	///////////
	m_rankingpagenum(data.m_rankingpagenum),
	m_rankingtotal(data.m_rankingtotal),
	m_chatReciverName(data.m_chatReciverName),
	m_BrainRecruitCount(data.m_BrainRecruitCount),
	m_BrainResourcesCardCount(data.m_BrainResourcesCardCount),
	m_BrainRushTaskCount(data.m_BrainRushTaskCount),
	m_BrainExtractJiangKa(data.m_BrainExtractJiangKa),
	m_unfoldtasktype1(data.m_unfoldtasktype1),
	m_unfoldtasktype2(data.m_unfoldtasktype2),
	m_unfoldtasktype3(data.m_unfoldtasktype3),
	m_unfoldtasktype4(data.m_unfoldtasktype4),
	m_curselecttaskid(data.m_curselecttaskid),
	m_GongfangLordPaperCount(data.m_GongfangLordPaperCount),
	m_canvasVec(data.m_canvasVec),
	m_useitemremaincount(data.m_useitemremaincount),
	m_PingpanRemainCount(data.m_PingpanRemainCount),
	m_rollcardremaincount(data.m_rollcardremaincount),
	m_msOnBulidingNode(data.m_msOnBulidingNode),
	m_goldcosthinttype(data.m_goldcosthinttype),
	m_playdatatabIO(data.m_playdatatabIO),
	m_recruitarmyvec(data.m_recruitarmyvec),
	m_friendorblack(data.m_friendorblack),
	m_ifEndSendPlayerData(data.m_ifEndSendPlayerData),
	m_remainfubenrushcount(data.m_remainfubenrushcount),
	m_queryloseshiliid(data.m_queryloseshiliid),
	m_hideModel(data.m_hideModel),
	m_onlineDongHuaVisiable(data.m_onlineDongHuaVisiable),
	m_autoBattleSetting(data.m_autoBattleSetting),
	m_newguidestep(data.m_newguidestep),
	m_newguidecanvas(data.m_newguidecanvas),
	m_newguidebtn(data.m_newguidebtn),
	m_newguideid(data.m_newguideid),
	m_totalrollcardcount(data.m_totalrollcardcount),
	m_totalupgradecardcount(data.m_totalupgradecardcount),
	m_totalmakelordequipcount(data.m_totalmakelordequipcount),
	//m_newhandtaskscrollbar(data.m_newhandtaskscrollbar),
	m_newguidebtnmessage(data.m_newguidebtnmessage),
	m_newguidebtnparam(data.m_newguidebtnparam),
	m_newguideemptyhypertext(data.m_newguideemptyhypertext),
	m_showVersion(data.m_showVersion),
	m_totalexchangerescount(data.m_totalexchangerescount),
	m_totallevycount(data.m_totallevycount),
	m_delayHideWidgetTime(data.m_delayHideWidgetTime)
{}
void crJXJBrainData::addParam(int i, const std::string& str)
{
	crBrainData::addParam(i,str);
}
void crJXJBrainData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crBrainData::excHandle(msg); 
	}
}
void crJXJBrainData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJCurPlayerIconID:
		m_curPlayerIconID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurRecruitArmID:
		m_curRecruitArmID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurSelRecruitArmAID:
		m_curSelRecruitArmAID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurFormationArmID:
		m_curFormationArmID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurSelFormationArmAID:
		m_curSelFormationArmAID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurCDHintID:
		m_curCDHintID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurTimesID:
		m_curTimesID = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCurSalePageNum:
		m_curSalePageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJSaleTotal:
		m_saleTotal = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJMyCurSalePageNum:
		m_curSalePageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJMySaleTotal:
		m_saleTotal = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJEquipID:
		m_equipID = param?*((unsigned short*)param):NULL;
		break;
	case WCHDATA_JXJMyCurFriendPageNum:
		m_curFriendPageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJMyFriendPageTotal:
		m_totalFriendPageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJAddFriendName:
		m_addfriendname = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJSaleItemID:
		m_saleitemid = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJSaleItemMax:
		m_saleitemmax = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJCurJiangkaBagPageNum:
		m_curJiangkaBagPageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTotalJiangkaBagPageNum:
		m_totalJiangkaBagPageNum = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJJiangkaFormationRow:
		m_JiangkaFormationRow = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewsCanvasStep:
		m_NewsCanvasStep = param?*(float*)param:NULL;
		break;
	case WCHDATA_JXJIfSetArmyNum:
		m_IfSetArmyNum = param?*(bool*)param:NULL;
		break;
	case WCHDATA_JXJBrainCurOpened:
		m_cur_canvas = param ? (CRCore::crCanvasNode *)param : NULL;
		break;
	case WCHDATA_JXJBrainCurRTTName:
		m_rtt_widget = param ? (CRUI::crImageBoxWidgetNode *)param : NULL;
		break;
	case WCHDATA_JXJBrainCurRTTRF:
		m_rtt_refresh = param ? *(bool *)param : NULL;
		break;
	case WCHDATA_JXJRankingPageNum:
		m_rankingpagenum = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJRankingTotal:
		m_rankingtotal = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJAutoMakeEquipCount:
		m_autoequipscount = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJBrainRecruitCount:
		m_BrainRecruitCount = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJBrainResourcesCardCount:
		m_BrainResourcesCardCount = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJBrainRushTaskCount:
		m_BrainRushTaskCount = param ? *(int *)param : NULL;
		break;
	case WCHDATA_JXJBrainExtractJiangKa:
		m_BrainExtractJiangKa = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJCurInvitePageNum:
		m_curinvitepagenum = param ? *(int *)param : NULL;
		break;	
	case WCHDATA_JXJTotalInvitePageNum:
		m_totalinvitepagenum = param ? *(int *)param : NULL;
		break;
	//case WCHDATA_JXJCurInviteType:
	//	m_curinvitetype = param ? *(unsigned char *)param : NULL;
	//	break;
	case WCHDATA_JXJCurForgeEquipID:
		m_CurForgeEquipID = param?*((unsigned short*)param):NULL;
		break;
	//case WCHDATA_JXJCurSelForgeEquipID:
	//	m_CurSelForgeEquipID = param?*((int*)param):NULL;
	//	break;
	case WCHDATA_JXJCurSaleCardPageNum:
		m_cursalecardpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJTotalSaleCardPageNum:
		m_totalsalecardpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJUpgradeJiangkaIndex:
		m_salejiangkaindex = param?*((short*)param):NULL;
		break;
	case WCHDATA_JXJUpgradeNewJiangkaId:
		m_newjiangkaid = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJCheckMailIndex:
		m_checkmailindex = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJDeleteAllMailFlag:
		m_deleteallmailflag = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJFormationCurPage:
		m_curformationpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJFormationTotalPage:
		m_totalformationpagenum = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJChatOneToOneReciverName:
		m_chatReciverName = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJUnfoldTaskType1:
		m_unfoldtasktype1 = param?*(bool*)param:NULL;
		break;
	case WCHDATA_JXJUnfoldTaskType2:
		m_unfoldtasktype2 = param?*(bool*)param:NULL;
		break;
	case WCHDATA_JXJUnfoldTaskType3:
		m_unfoldtasktype3 = param?*(bool*)param:NULL;
		break;
	case WCHDATA_JXJUnfoldTaskType4:
		m_unfoldtasktype4 = param?*(bool*)param:NULL;
		break;
	case WCHDATA_JXJCurChooseTaskID:
		m_curselecttaskid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJGongfangLordPaperCount:
		m_GongfangLordPaperCount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJRecvBagItemDeq:
		if(param)
			m_itemDeq = *(RecvBagItemDeq *)param;
		else
			m_itemDeq.clear();
		break;
	case WCHDATA_JXJBagItemIconNodeVec:
		if(param)
			m_canvasVec = *(ItemCanvasPairVec *)param;
		else
		{
			m_canvasVec.clear();
		}
		break;
	case WCHDATA_JXJUseItemRemainCount:
		m_useitemremaincount = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJBatchRebelCount:
		m_PingpanRemainCount = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJRollCardRemainCount:
		m_rollcardremaincount = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJMouseOneBulidingNode:
		m_msOnBulidingNode = param?(CRCore::crNode *)param:NULL;
		break;
	case WCHDATA_JXJCurGoldHintType:
		m_goldcosthinttype = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJCheckPlayDataTable:
		m_playdatatabIO = param?(CREncapsulation::crTableIO*)param:NULL;
		break;
	case WCHDATA_JXJAddFriendOrBlack:
		m_friendorblack = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJIFEndSendPlayerData:
		m_ifEndSendPlayerData = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJRemainFubenRushCount:
		m_remainfubenrushcount = param?*(unsigned char*)param:NULL;
		break;
	case WCHDATA_JXJQueryLoseShiliID:
		m_queryloseshiliid = param?*(crVector2ub*)param:crVector2ub(0,0);
		break;
	case WCHDATA_JXJHideModel:
		m_hideModel = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJAutoBattleSetting:
		m_autoBattleSetting = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJOnlineGiftDonghua	:
		m_onlineDongHuaVisiable = param == NULL? false : *(bool *)param;
		break;
	case WCHDATA_JXJNewGuideID:
		m_newguideid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewGuideStep:
		m_newguidestep = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewGuideCanvas:
		m_newguidecanvas = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJNewGuideBtn:
		m_newguidebtn = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJNewGuideBtnMessage:
		m_newguidebtnmessage = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewguideRollCardCount:
		m_totalrollcardcount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewguideUpgradeCardCount:
		m_totalupgradecardcount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewguideMakeLordEquipCount:
		m_totalmakelordequipcount = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJNewGuideEmptyHyperText:
		m_newguideemptyhypertext = param?*(int*)param:NULL;
		break;
	//case WCHDATA_JXJNewHandTaskScrollBar:
	//	m_newhandtaskscrollbar = param?*(float*)param:NULL;
	//	break;
	//case WCHDATA_JXJShowVersion					:
	//	m_showVersion = param ? *(bool *) param : false;
	//	break;
	case WCHDATA_JXJTotalExchangeResCount:
		m_totalexchangerescount = param ? *(int*)param : NULL;
		break;
	case WCHDATA_JXJTotalLevyCount:
		m_totallevycount = param ? *(int*)param : NULL;
		break;
	case WCHDATA_JXJDelayHideWidgetTime:
		m_delayHideWidgetTime = param ? *(int*)param : NULL;
		break;
	default:
		crBrainData::inputParam(i,param);
	}
}
void crJXJBrainData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJCurPlayerIconID:
		param = &m_curPlayerIconID;
		break;
	case WCHDATA_JXJCurRecruitArmID:
		param = &m_curRecruitArmID;
		break;
	case WCHDATA_JXJCurSelRecruitArmAID:
		param = &m_curSelRecruitArmAID;
		break;
	case WCHDATA_JXJCurFormationArmID:
		param = &m_curFormationArmID;
		break;
	case WCHDATA_JXJCurSelFormationArmAID:
		param = &m_curSelFormationArmAID;
		break;
	case WCHDATA_JXJCurCDHintID:
		param = &m_curCDHintID;
		break;
	case WCHDATA_JXJCurTimesID:
		param = &m_curTimesID;
		break;
	case WCHDATA_JXJPlayerSaleRecordVec:
		param = &m_playerSaleRecordVec;
		break;
	case WCHDATA_JXJCurSalePageNum:
		param = &m_curSalePageNum;
		break;
	case WCHDATA_JXJSaleTotal:
		param = &m_saleTotal;
		break;
	case WCHDATA_JXJMyCurSalePageNum:
		param = &m_curSalePageNum;
		break;
	case WCHDATA_JXJMySaleTotal:
		param = &m_saleTotal;
		break;
	case WCHDATA_JXJEquipID:
		param = &m_equipID;
		break;
	case WCHDATA_JXJMyCurFriendPageNum:
		param = &m_curFriendPageNum;
		break;
	case WCHDATA_JXJMyFriendPageTotal:
		param = &m_totalFriendPageNum;
		break;
	case WCHDATA_JXJAddFriendName:
		param = &m_addfriendname;
		break;
	case WCHDATA_JXJSaleItemID:
		param = &m_saleitemid;
		break;
	case WCHDATA_JXJSaleItemMax:
		param = &m_saleitemmax;
		break;
	case WCHDATA_JXJCurJiangkaBagPageNum:
		param = &m_curJiangkaBagPageNum;
		break;
	case WCHDATA_JXJTotalJiangkaBagPageNum:
		param = &m_totalJiangkaBagPageNum;
		break;
	case WCHDATA_JXJJiangkaFormationRow:
		param = &m_JiangkaFormationRow;
		break;
	case WCHDATA_JXJNewsCanvasStep:
		param = &m_NewsCanvasStep;
		break;
	case WCHDATA_JXJIfSetArmyNum:
		param = &m_IfSetArmyNum;
		break;	
	case WCHDATA_JXJBrainCurOpened:
		param = m_cur_canvas.get();
		break;
	case WCHDATA_JXJBrainCurRTTName:
		param = m_rtt_widget.get();
		break;
	case WCHDATA_JXJBrainCurRTTRF:
		param = &m_rtt_refresh;
		break;	
	case WCHDATA_JXJRankingPageNum:
		param = &m_rankingpagenum;
		break;	
	case WCHDATA_JXJRankingTotal:
		param = &m_rankingtotal;
		break;	
	case WCHDATA_JXJAutoMakeEquipCount:
		param = &m_autoequipscount;
		break;	
	case WCHDATA_JXJBrainRecruitCount:
		param = &m_BrainRecruitCount;
		break;
	case WCHDATA_JXJBrainResourcesCardCount:
		param = &m_BrainResourcesCardCount;
		break;
	case WCHDATA_JXJBrainRushTaskCount:
		param = &m_BrainRushTaskCount;
		break;
	case WCHDATA_JXJBrainExtractJiangKa:
		param = &m_BrainExtractJiangKa;
		break;
	case WCHDATA_JXJCurInvitePageNum:
		param = &m_curinvitepagenum;
		break;	
	case WCHDATA_JXJTotalInvitePageNum:
		param = &m_totalinvitepagenum;
		break;
	//case WCHDATA_JXJCurInviteType:
	//	param = &m_curinvitetype;
	//	break;
	case WCHDATA_JXJCurForgeEquipID:
		param = &m_CurForgeEquipID;
		break;
	//case WCHDATA_JXJCurSelForgeEquipID:
	//	param = &m_CurSelForgeEquipID;
	//	break;
	case WCHDATA_JXJSaleBagVec:
		param = &m_salebagvec;
		break;
	case WCHDATA_JXJCurSaleCardPageNum:
		param = &m_cursalecardpagenum;
		break;
	case WCHDATA_JXJTotalSaleCardPageNum:
		param = &m_totalsalecardpagenum;
		break;
	case WCHDATA_JXJUpgradeJiangkaIndex:
		param = &m_salejiangkaindex;
		break;
	case WCHDATA_JXJUpgradeNewJiangkaId:
		param = &m_newjiangkaid;
		break;
	case WCHDATA_JXJRecvMailVec:
		param = &m_recvmailvec;
		break;
	case WCHDATA_JXJCheckMailIndex:
		param = &m_checkmailindex;
		break;
	case WCHDATA_JXJRecvMailCacheMap:
		param = &m_recvmailbodyMap;
		break;
	case WCHDATA_JXJDeleteAllMailFlag:
		param = &m_deleteallmailflag;
		break;
	case WCHDATA_JXJFormationCurPage:
		param = &m_curformationpagenum;
		break;
	case WCHDATA_JXJFormationTotalPage:
		param = &m_totalformationpagenum;
		break;
	case WCHDATA_JXJChatOneToOneReciverName:
		param = &m_chatReciverName;
		break;
	case WCHDATA_JXJUnfoldTaskType1:
		param = &m_unfoldtasktype1;
		break;
	case WCHDATA_JXJUnfoldTaskType2:
		param = &m_unfoldtasktype2;
		break;
	case WCHDATA_JXJUnfoldTaskType3:
		param = &m_unfoldtasktype3;
		break;
	case WCHDATA_JXJUnfoldTaskType4:
		param = &m_unfoldtasktype4;
		break;
	case WCHDATA_JXJCurChooseTaskID:
		param = &m_curselecttaskid;
		break;
	case WCHDATA_JXJGongfangLordPaperCount:
		param = &m_GongfangLordPaperCount;
		break;
	case WCHDATA_JXJRecvBagItemDeq:
		param = &m_itemDeq;
		break;
	case WCHDATA_JXJBagItemIconNodeVec:
		param = &m_canvasVec;
		break;
	case WCHDATA_JXJUseItemRemainCount:
		param = &m_useitemremaincount;
		break;
	case WCHDATA_JXJBatchRebelCount:
		param = &m_PingpanRemainCount;
		break;
	case WCHDATA_JXJRollCardRemainCount:
		param = &m_rollcardremaincount;
		break;
	case WCHDATA_JXJMouseOneBulidingNode:
		param = m_msOnBulidingNode.get();
		break;
	case WCHDATA_JXJCurGoldHintType:
		param = &m_goldcosthinttype;
		break;
	case WCHDATA_JXJRecruitArmyVec:
		param = &m_recruitarmyvec;
		break;
	case WCHDATA_JXJAddFriendOrBlack:
		param = &m_friendorblack;
		break;
	case WCHDATA_JXJIFEndSendPlayerData:
		param = &m_ifEndSendPlayerData;
		break;
	case WCHDATA_JXJRemainFubenRushCount:
		param = &m_remainfubenrushcount;
		break;
	case WCHDATA_JXJQueryLoseShiliID:
		param = &m_queryloseshiliid;
		break;
	case WCHDATA_JXJHideModel:
		param = &m_hideModel;
		break;
	case WCHDATA_JXJAutoBattleSetting:
		param = &m_autoBattleSetting;
		break;
	case WCHDATA_JXJNewGuideID:
		param = &m_newguideid;
		break;
	case WCHDATA_JXJNewGuideStep:
		param = &m_newguidestep;
		break;
	case WCHDATA_JXJNewGuideCanvas:
		param = &m_newguidecanvas;
		break;
	case WCHDATA_JXJNewGuideBtn:
		param = &m_newguidebtn;
		break;
	case WCHDATA_JXJNewGuideBtnMessage:
		param = &m_newguidebtnmessage;
		break;
	case WCHDATA_JXJNewguideRollCardCount:
		param = &m_totalrollcardcount;
		break;
	case WCHDATA_JXJNewguideUpgradeCardCount:
		param = &m_totalupgradecardcount;
		break;
	case WCHDATA_JXJNewguideMakeLordEquipCount:
		param = &m_totalmakelordequipcount;
		break;
	//case WCHDATA_JXJNewHandTaskScrollBar:
	//	param = &m_newhandtaskscrollbar;
	//	break;
	case WCHDATA_JXJCheckPlayDataTable:
		if (!m_playdatatabIO.valid())
		{
			m_playdatatabIO = new crTableIO;
		}
		param = m_playdatatabIO.get();
		break;
	case WCHDATA_JXJOnlineGiftDonghua:
		param = & m_onlineDongHuaVisiable;
		break;
	case WCHDATA_JXJNewGuideBtnParamVec:
		param = & m_newguidebtnparam;
		break;
	case WCHDATA_JXJNewGuideEmptyHyperText:
		param = & m_newguideemptyhypertext;
		break;
	//case WCHDATA_JXJShowVersion					:
	//	param = & m_showVersion;
	//	break;
	case WCHDATA_JXJTotalExchangeResCount:
		param = &m_totalexchangerescount;
		break;
	case WCHDATA_JXJTotalLevyCount:
		param = &m_totallevycount;
		break;
	case WCHDATA_JXJDelayHideWidgetTime:
		param = &m_delayHideWidgetTime;
		break;
	default:
		crBrainData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJMainRoleData
//
/////////////////////////////////////////
crJXJMainRoleData::crJXJMainRoleData():
m_mainroleinited(false),
m_level(1),
m_experience(0),
m_exploit(0),
m_achievement(0),
m_govpost(1),
m_nobility(1),
//m_campaignranking(0),
m_bingfa(0),
m_zhenfa(0),
m_dongcha(0),
m_tongshuai(0),
m_accomplishPoint(0),
m_currentEquipIndex(0),
m_openedEquipVec(1),
m_appointColumnsVec(3),
m_changeNameTime(0L),
m_qiyutaskid(0),
m_qiyuCompleteCount(0)
//m_mygroupapplyid(0)
//m_soldierCount(0),
//m_combatpower(0)
{
}
crJXJMainRoleData::crJXJMainRoleData(const crJXJMainRoleData& data):
crRoleData(data),
m_mainroleinited(data.m_mainroleinited),
m_level(data.m_level),
m_experience(data.m_experience),
m_exploit(data.m_exploit),
m_achievement(data.m_achievement),
m_govpost(data.m_govpost),
m_nobility(data.m_nobility),
//m_campaignranking(data.m_campaignranking),
m_bingfa(data.m_bingfa),
m_zhenfa(data.m_zhenfa),
m_dongcha(data.m_dongcha),
m_tongshuai(data.m_tongshuai),
//m_soldierCount(data.m_soldierCount),
//m_combatpower(data.m_combatpower),
//m_lordsuitmap(data.m_lordsuitmap),
m_accomplishMap(data.m_accomplishMap),
m_accomplishPoint(data.m_accomplishPoint),
//m_huodongMap(data.m_huodongMap),
m_newHuodongMap(data.m_newHuodongMap),
m_newGuideMap(data.m_newGuideMap),
m_huoyuedumap(data.m_huoyuedumap),
m_huoyuedurewardset(data.m_huoyuedurewardset),
m_equipvecs(data.m_equipvecs),
m_currentEquipIndex(data.m_currentEquipIndex),
m_openedEquipVec(data.m_openedEquipVec),
m_appointColumnsVec(data.m_appointColumnsVec),
m_changeNameTime(data.m_changeNameTime),
m_qiyutaskid(data.m_qiyutaskid),
m_qiyuCompleteCount(data.m_qiyuCompleteCount)
//m_mygroupapplyid(data.m_mygroupapplyid)
{
}
crJXJMainRoleData::~crJXJMainRoleData()
{
	GNE::LockMutex lock( m_dataMutex );
	for( AccomplishMap::iterator itr = m_accomplishMap.begin();
		 itr != m_accomplishMap.end();
		 ++itr )
	{
		itr->second->clearData();
	}
	m_accomplishMap.clear();
	for( JXJNewGuideMap::iterator itr = m_newGuideMap.begin();
		itr != m_newGuideMap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_newGuideMap.clear();
	for( HuoyueduMap::iterator itr = m_huoyuedumap.begin();
		itr != m_huoyuedumap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_huoyuedumap.clear();
	for( NewHuodongMap::iterator itr = m_newHuodongMap.begin();
		itr != m_newHuodongMap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_newHuodongMap.clear();
}
void crJXJMainRoleData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_Level:
		m_level = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Experience:
		m_experience = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJExploit:
		m_exploit = atoi(relStr.c_str());
		break;
	//case WCHDATA_JXJJunling:
	//	m_junling = (unsigned char)(atoi(relStr.c_str()));
	//	break;
	case WCHDATA_JXJAchievement:
		m_achievement = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGovPost:
		m_govpost = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJNobility:
		m_nobility = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJAccomplishPoint:
		m_accomplishPoint = (unsigned short)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCurrentEquipVecIndex:
		m_currentEquipIndex = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJSavedEquipVecs:
		m_openedEquipVec = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJAppointColumnsVec:
		m_appointColumnsVec = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJChangeNameTime:
		m_changeNameTime = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJQiyuTaskID:
		m_qiyutaskid = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJQiyuCompleteCount:
		m_qiyuCompleteCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_JXJMyApplyGroupIDVec:
	//	m_mygroupapplyid = (int)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJCampaignRanking:
	//	m_campaignranking = (unsigned short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJForce:
	//	m_force = (unsigned short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJIntellect:
	//	m_intellect = (unsigned short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJCommand:
	//	m_command = (unsigned short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJGlamour:
	//	m_glamour = (unsigned short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJSoldierCount:
	//	m_soldierCount = (short)(atoi(relStr.c_str()));
	//	break;
	//case WCHDATA_JXJCombatPower:
	//	m_combatpower = (short)(atoi(relStr.c_str()));
	//	break;
	default:
		crRoleData::addParam(i,str);
		break;
	}
}
void crJXJMainRoleData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[128];
		cfg_script.Add("VERSION",JXJVERSION);
		sprintf(buf,"%d,%d\0",WCHDATA_Level,(int)m_level);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_Experience,m_experience);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJExploit,m_exploit);
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJJunling,(int)m_junling);
		//cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAchievement,m_achievement);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGovPost,(int)m_govpost);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJNobility,(int)m_nobility);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAccomplishPoint,(int)m_accomplishPoint);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCurrentEquipVecIndex,(int)m_currentEquipIndex);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSavedEquipVecs,(int)m_openedEquipVec);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJAppointColumnsVec,(int)m_appointColumnsVec);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJChangeNameTime,crArgumentParser::appI64toa(m_changeNameTime).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJQiyuTaskID,m_qiyutaskid);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJQiyuCompleteCount,m_qiyuCompleteCount);
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJMyApplyGroupID,(int)m_mygroupapplyid);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJCampaignRanking,(int)m_campaignranking);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJForce,(int)m_force);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJIntellect,(int)m_intellect);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJCommand,(int)m_command);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJGlamour,(int)m_glamour);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJSoldierCount,m_soldierCount);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJCombatPower,m_combatpower);
		//cfg_script.Add("Param",buf);
		//道具
		std::vector<std::string> v_str;
		//领主已装备列表
		if(!m_equipvecs.empty())
		{
			cfg_script.Add("JXJEquipVecs");
			cfg_script.Push("JXJEquipVecs");
			int len = m_equipvecs.size();
			
			for (int i = 0 ; i < len; ++i)
			{
				cfg_script.Add("JXJEquipVec");
				cfg_script.Push("JXJEquipVec",i+1);
				v_str.resize(2);
				const JXJLordEquipVec& equipVec = m_equipvecs[i];
				for(int j = 0 ; j<equipVec.size(); ++j)
				{
					v_str[0] = crArgumentParser::appItoa(equipVec[j].first);
					v_str[1] = crArgumentParser::appItoa(equipVec[j].second);
					cfg_script.Add("Param",v_str);
				}
				cfg_script.Pop();
			}
			cfg_script.Pop();
		}
		//AccomplishMap
		if(!m_accomplishMap.empty())
		{
			cfg_script.Add("AccomplishMap");
			cfg_script.Push("AccomplishMap");
			for( AccomplishMap::iterator itr = m_accomplishMap.begin();
				itr != m_accomplishMap.end();
				++itr )
			{
				if(itr->second->getComplete())
				{
					cfg_script.Add("Param",itr->first);
				}
				//v_i.clear();
				//v_i.push_back(itr->first);
				//v_i.push_back((itr->second->getComplete())?1:0);
				//cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();

			/*cfg_script.Add("AccomplishMapStep");
			cfg_script.Push("AccomplishMapStep");
			for( AccomplishMap::iterator itr = m_accomplishMap.begin();
			itr != m_accomplishMap.end();
			++itr )
			{
			v_str.resize(2);
			if(!itr->second->getComplete())
			{
			v_str[0] = crArgumentParser::appItoa(itr->first);
			v_str[1] = crArgumentParser::appItoa(itr->second->getStep());
			cfg_script.Add("Param",v_str);
			}
			}
			cfg_script.Pop();*/
		}
		//HuodongMap
		//if(!m_huodongMap.empty())
		//{
		//	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		//	crTableIO::StrVec hdrecord;
		//	cfg_script.Add("HuodongMap");
		//	cfg_script.Push("HuodongMap");
		//	v_str.resize(6);
		//	for( HuodongMap::iterator itr = m_huodongMap.begin();
		//		itr != m_huodongMap.end();
		//		++itr )
		//	{
		//		if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),hdrecord)>=0)
		//		{
		//			if (itr->second->needSave())
		//			{
		//				v_str[0] = crArgumentParser::appItoa(itr->first);
		//				v_str[1] = crArgumentParser::appItoa(itr->second->getStep());
		//				v_str[2] = crArgumentParser::appItoa(itr->second->getComplete());
		//				v_str[3] = crArgumentParser::appItoa(itr->second->getStepCount());
		//				v_str[4] = crArgumentParser::appItoa(itr->second->getUsedTimes());
		//				v_str[5] = crArgumentParser::appItoa(itr->second->getDelFlag());
		//				cfg_script.Add("Param",v_str);
		//				//cfg_script.Add("Param",itr->first);
		//				////cfg_script.Add("Param",itr->second->getStartTime());
		//				////cfg_script.Add("Param",itr->second->getEndTime());
		//				//cfg_script.Add("Param",itr->second->getStep());
		//				//cfg_script.Add("Param",itr->second->getComplete());
		//				//cfg_script.Add("Param",itr->second->getStepCount());
		//				//cfg_script.Add("Param",itr->second->getUsedTimes());
		//				//cfg_script.Add("Param",itr->second->getDelFlag());
		//			}
		//		}
		//	}
		//	cfg_script.Pop();
		//}
		if(!m_newHuodongMap.empty())
		{
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if (huodongTab.valid())
			{
				crTableIO::StrVec hdrecord;
				cfg_script.Add("NewHuodongMap");
				cfg_script.Push("NewHuodongMap");
				v_str.resize(5);
				for( NewHuodongMap::iterator itr = m_newHuodongMap.begin();
					itr != m_newHuodongMap.end();
					++itr )
				{
					if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),hdrecord)>=0)
					{
						if (itr->second->isNeedSave())
						{
							v_str[0] = crArgumentParser::appItoa(itr->first);
							v_str[1] = crArgumentParser::appItoa(itr->second->getStep());
							v_str[2] = crArgumentParser::appItoa(itr->second->getActivation());
							//v_str[3] = crArgumentParser::appItoa(itr->second->getStepCount());
							v_str[3] = crArgumentParser::appItoa(itr->second->getCircles());
							v_str[4] = crArgumentParser::appItoa(itr->second->getDelFlag());
							cfg_script.Add("Param",v_str);
							//cfg_script.Add("Param",itr->first);
							////cfg_script.Add("Param",itr->second->getStartTime());
							////cfg_script.Add("Param",itr->second->getEndTime());
							//cfg_script.Add("Param",itr->second->getStep());
							//cfg_script.Add("Param",itr->second->getComplete());
							//cfg_script.Add("Param",itr->second->getStepCount());
							//cfg_script.Add("Param",itr->second->getUsedTimes());
							//cfg_script.Add("Param",itr->second->getDelFlag());
						}
					}
				}
				cfg_script.Pop();
			}
		}
		//JXJNewGuideMap
		if(!m_newGuideMap.empty())
		{
			cfg_script.Add("JXJNewGuideMap");
			cfg_script.Push("JXJNewGuideMap");
			for( JXJNewGuideMap::iterator itr = m_newGuideMap.begin();
				itr != m_newGuideMap.end();
				++itr )
			{
				if(itr->second->getComplete())
				{
					cfg_script.Add("Param",itr->first);
				}
				//v_i.clear();
				//v_i.push_back(itr->first);
				//v_i.push_back((itr->second->getComplete())?1:0);
				//cfg_script.Add("Param",v_i);
			}
			cfg_script.Pop();
		}
		//HuoyueduMap活跃度
		if (!m_huoyuedumap.empty())
		{
			cfg_script.Add("HuoyueduMap");
			cfg_script.Push("HuoyueduMap");
			v_str.resize(2);
			ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
			crTableIO::StrVec Huoyuedurecord;
			for (HuoyueduMap::iterator itr = m_huoyuedumap.begin();
				itr != m_huoyuedumap.end();
				++itr)
			{
				if (HuoyueduTab->queryOneRecord(0, crArgumentParser::appItoa(itr->first), Huoyuedurecord) >= 0)
				{
					if (itr->second->getStep() > 0/* || itr->second->getComplete()*/)
					{
						v_str[0] = crArgumentParser::appItoa(itr->first);
						v_str[1] = crArgumentParser::appItoa(itr->second->getStep());
						cfg_script.Add("Param", v_str);
						//cfg_script.Add("Param", (int)itr->second->getComplete());
					}
				}
			}
			cfg_script.Pop();
		}
		//HuoyueduRewardSet
		if (!m_huoyuedurewardset.empty())
		{
			cfg_script.Add("HuoyueduRewardSet");
			cfg_script.Push("HuoyueduRewardSet");
			for (HuoyueduRewardSet::iterator itr = m_huoyuedurewardset.begin();
				itr != m_huoyuedurewardset.end();
				++itr)
			{
				cfg_script.Add("Param", (int)(*itr));
			}
			cfg_script.Pop();
		}
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,0).c_str()));

		stream->createBuf(8192);

		stream->_writeUChar(m_level);
		stream->_writeInt(m_experience);//4*4
		stream->_writeInt(m_exploit);
		//stream->_writeUChar(m_junling);
		stream->_writeInt(m_achievement);
		stream->_writeUChar(m_govpost);
		stream->_writeUChar(m_nobility);
		stream->_writeUShort(m_accomplishPoint);
		stream->_writeUChar(m_appointColumnsVec);
		stream->_writeInt64(m_changeNameTime);
		//stream->_writeInt(m_mygroupapplyid);
		//stream->_writeUShort(m_campaignranking);//2*7
		//stream->_writeUShort(m_force);
		//stream->_writeUShort(m_intellect);
		//stream->_writeUShort(m_command);
		//stream->_writeUShort(m_glamour);
		//stream->_writeShort(m_soldierCount);
		//stream->_writeShort(m_combatpower);
		//道具
		//领主已装备列表
		stream->_writeUChar(m_currentEquipIndex);
		stream->_writeUChar(m_openedEquipVec);
		m_equipvecs.resize(m_openedEquipVec);
		for(int i = 0; i < m_openedEquipVec; ++i)
		{
			JXJLordEquipVec& equipVec = m_equipvecs[i];
			equipVec.resize(equiponmax);
			stream->_writeUChar(equipVec.size());
			for (int j = 0; j < equiponmax; j++)
			{
				stream->_writeInt(equipVec[j].first);
				if(equipVec[j].first>0)
					stream->_writeInt(equipVec[j].second);
			}

		}
		
		////领主套装列表
		//stream->_writeUChar(m_lordsuitmap.size());
		//for( JXJLordSuitMap::iterator itr = m_lordsuitmap.begin();
		//	itr != m_lordsuitmap.end();
		//	++itr )
		//{
		//	if(itr->first > 0)
		//	{
		//		stream->_writeUChar(itr->first);
		//		stream->_writeUChar(itr->second);
		//	}
		//	else
		//	{
		//		stream->_writeUChar(0);
		//	}
		//}
		//AccomplishMap
		char *ptr = stream->getPtr();
		stream->_writeUShort(m_accomplishMap.size());
		int relsize = 0;
		for( AccomplishMap::iterator itr = m_accomplishMap.begin();
			 itr != m_accomplishMap.end();
			 ++itr )
		{
			if(itr->second->getComplete())
			{
				stream->_writeUShort(itr->first);
				relsize++;
			}
			//m_dataStream->_writeBool(itr->second);
		}
		char *nowptr = stream->getPtr();
		int bufsize = stream->getBufSize();
		stream->seek(ptr-nowptr);
		stream->_writeUShort(relsize);
		stream->setBufSize(bufsize);
		stream->seekEnd();

		//ptr = stream->getPtr();
		//stream->_writeUShort(m_accomplishMap.size());
		//relsize = 0;
		//for( AccomplishMap::iterator itr = m_accomplishMap.begin();
		//	itr != m_accomplishMap.end();
		//	++itr )
		//{
		//	if(!itr->second->getComplete())
		//	{
		//		stream->_writeUShort(itr->first);
		//		stream->_writeUChar(itr->second->getStep());
		//		relsize++;
		//	}
		//	//m_dataStream->_writeBool(itr->second);
		//}
		//nowptr = stream->getPtr();
		//bufsize = stream->getBufSize();
		//stream->seek(ptr-nowptr);
		//stream->_writeUShort(relsize);
		//stream->setBufSize(bufsize);
		//stream->seekEnd();
		////HuodongMap
		//ptr = stream->getPtr();
		//stream->_writeUChar(m_huodongMap.size());
		//relsize = 0;
		//for( HuodongMap::iterator itr = m_huodongMap.begin();
		//	itr != m_huodongMap.end();
		//	++itr )
		//{
		//	if (itr->second->getOpenFlag())
		//	{
		//		stream->_writeUShort(itr->first);
		//		stream->_writeInt(itr->second->getStep());
		//		stream->_writeUChar(itr->second->getComplete());
		//		stream->_writeInt(itr->second->getStepCount());
		//		stream->_writeUShort(itr->second->getUsedTimes());
		//		stream->_writeInt(itr->second->getDelFlag());
		//		relsize++;
		//	}
		//}
		//nowptr = stream->getPtr();
		//bufsize = stream->getBufSize();
		//stream->seek(ptr-nowptr);
		//stream->_writeUChar(relsize);
		//stream->setBufSize(bufsize);
		//stream->seekEnd();

		//NewHuodongMap
		ptr = stream->getPtr();
		stream->_writeUShort(m_newHuodongMap.size());
		relsize = 0;
		for( NewHuodongMap::iterator itr = m_newHuodongMap.begin();
			itr != m_newHuodongMap.end();
			++itr )
		{
			if(itr->second->isNeedSave())
			{
				stream->_writeUShort(itr->first);
				stream->_writeUChar(itr->second->getActivation());
				stream->_writeInt(itr->second->getStep());
				stream->_writeUChar(itr->second->getCircles());
				stream->_writeUChar(itr->second->getDelFlag());
				relsize++;
			}
		}
		nowptr = stream->getPtr();
		bufsize = stream->getBufSize();
		stream->seek(ptr-nowptr);
		stream->_writeUShort(relsize);
		stream->setBufSize(bufsize);
		stream->seekEnd();

		//JXJNewGuideMap
		ptr = stream->getPtr();
		stream->_writeUShort(m_newGuideMap.size());
		relsize = 0;
		for( JXJNewGuideMap::iterator itr = m_newGuideMap.begin();
			itr != m_newGuideMap.end();
			++itr )
		{
			if(itr->second->getComplete())
			{
				stream->_writeUShort(itr->first);
				relsize++;
			}
			//m_dataStream->_writeBool(itr->second);
		}
		nowptr = stream->getPtr();
		bufsize = stream->getBufSize();
		stream->seek(ptr-nowptr);
		stream->_writeUShort(relsize);
		stream->setBufSize(bufsize);
		stream->seekEnd();
		//

		//HuoyueduMap
		relsize = 0;
		ptr = stream->getPtr();
		stream->_writeUChar(m_huoyuedumap.size());
		for (HuoyueduMap::iterator itr = m_huoyuedumap.begin();
			itr != m_huoyuedumap.end();
			++itr)
		{
			if (itr->second->getStep() > 0)
			{
				stream->_writeUShort(itr->first);
				stream->_writeUChar(itr->second->getStep());
				//stream->_writeBool(itr->second->getComplete());
				relsize++;
			}
		}
		nowptr = stream->getPtr();
		bufsize = stream->getBufSize();
		stream->seek(ptr - nowptr);
		stream->_writeUChar(relsize);
		stream->setBufSize(bufsize);
		stream->seekEnd();
		////
		//HuoyueduRewardSet
		stream->_writeUChar(m_huoyuedurewardset.size());
		for (HuoyueduRewardSet::iterator itr = m_huoyuedurewardset.begin();
			itr != m_huoyuedurewardset.end();
			++itr)
		{
			stream->_writeUShort(*itr);
		}
		////
		stream->_writeInt(m_qiyutaskid);
		stream->_writeUChar(m_qiyuCompleteCount);

		///end
		stream->seekBegin();
		//role数据传输走数据流,不在这里做压缩
	}
	else if(LOINT64(msg) == WCH_BuildNetStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		//m_dataStream = new crStreamBuf;
		stream->createBuf(6);
		///////
		stream->_writeUChar(m_level);
		//m_dataStream->_writeInt(m_experience);//4*4
		//m_dataStream->_writeInt(m_exploit);
		stream->_writeUChar(m_govpost);
		//stream->_writeUShort(m_campaignranking);//2*7
		//stream->_writeUShort(m_force);
		//stream->_writeUShort(m_intellect);
		//stream->_writeUShort(m_command);
		//stream->_writeUShort(m_glamour);
		//stream->_writeShort(m_soldierCount);
		//stream->_writeShort(m_combatpower);
		//stream->_writeInt(m_coppercash);
		//stream->_writeInt(m_food);
		//stream->_writeInt(m_wood);
		//stream->_writeInt(m_iron);
		//stream->_writeInt(m_horse);
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_ServerCheckRoleData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
		serverCheckData(item,WCH_ServerCheckRoleData);
		//item->beginCreateItemChild(WCH_ServerCheckRoleData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_ServerCheckItemData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
		serverCheckData(item,WCH_ServerCheckItemData);
		//item->beginCreateItemChild(WCH_ServerCheckItemData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_CheckNetData/*WCH_CheckPlayerData*/)
	{
		//GNE::LockMutex lock( m_dataMutex );避免与crGlobalHandle的互斥信号产生互锁现象
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		clientCheckData(item);
	}
	else if(LOINT64(msg) == WCH_GameServerCheckData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		gameServerCheckData(item);
	} 
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_InitData)
	{
		//if(!m_extraDataClass.valid())
		//	m_extraDataClass = new crRpgGameExtraData;
		//else
		//	m_extraDataClass->clear();
		ref_ptr<crTableIO>lordlvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordlvTab);
		crTableIO::StrVec record;
		if(lordlvTab->queryOneRecord(0,crArgumentParser::appItoa(m_level),record)>=0)
		{
			int bingfaid = lordlvTab->getTitleIndex("兵法");
			int zhenfaid = lordlvTab->getTitleIndex("阵法");
			int dongchaid = lordlvTab->getTitleIndex("洞察");
			int tongshuaiid = lordlvTab->getTitleIndex("统率");
			m_bingfa = atoi(record[bingfaid].c_str());
			m_zhenfa = atoi(record[zhenfaid].c_str());
			m_dongcha = atoi(record[dongchaid].c_str());
			m_tongshuai = atoi(record[tongshuaiid].c_str());
		}

		//if (m_huodongMap.empty())
		//{
		//	//HuodongMap
		//	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
		//	crTableIO::DataVec hdDataVec;
		//	int openid = huodongTab->getTitleIndex("开启");
		//	int displayId = huodongTab->getTitleIndex("仅显示");
		//	huodongTab->queryRecords(openid,"1",displayId,"0",hdDataVec);
		//	ref_ptr<crJXJHuodong> huodong;
		//	for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
		//		itr != hdDataVec.end();
		//		++itr )
		//	{
		//		huodong = new crJXJHuodong;
		//		huodong->setID(atoi((*itr)[0].c_str()));
		//		m_huodongMap[huodong->getID()] = huodong;
		//		huodong->loadData();
		//	}
		//}
		if (m_newHuodongMap.empty())
		{
			//NewHuodongMap
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			if (huodongTab.valid())
			{
				crTableIO::DataVec hdDataVec;
				int openid = huodongTab->getTitleIndex("开启");
				int displayId = huodongTab->getTitleIndex("仅显示");
				huodongTab->queryRecords(openid,"1",displayId,"0",hdDataVec);
				ref_ptr<crJXJNewHuodong> huodong;
				for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
					itr != hdDataVec.end();
					++itr )
				{
					huodong = new crJXJNewHuodong;
					huodong->setID(atoi((*itr)[0].c_str()));
					m_newHuodongMap[huodong->getID()] = huodong;
					huodong->loadData();
				}
			}
		}

		if (m_accomplishMap.empty())
		{
			//AccomplishMap
			bool complete = false;
			ref_ptr<crTableIO>accomplishTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
			ref_ptr<crJXJAccomplish> accomplish;
			int rowcount = accomplishTab->getRowCount();
			int col = 0;
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				accomplish = new crJXJAccomplish;
				accomplish->setID(atoi((*accomplishTab)(i,col++).c_str()));
				m_accomplishMap[accomplish->getID()] = accomplish;
				accomplish->loadData();
			}
		}
		if (m_newGuideMap.empty())
		{
			ref_ptr<crTableIO>newguidetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTab);
			ref_ptr<crJXJNewGuide> jxjnewguide;
			int rowcount = newguidetab->getRowCount();
			int col = 0;
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				jxjnewguide = new crJXJNewGuide;
				jxjnewguide->setID(atoi((*newguidetab)(i,col++).c_str()));
				m_newGuideMap[jxjnewguide->getID()] = jxjnewguide;
				jxjnewguide->loadData();
			}
		}
		if (m_huoyuedumap.empty())
		{
			//HuoyueduMap
			ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
			ref_ptr<crJXJHuoyuedu> JXJhuoyuedu;
			int rowcount = HuoyueduTab->getRowCount();
			int col = 0;
			for (int i = 0; i < rowcount; ++i)
			{
				col = 0;
				JXJhuoyuedu = new crJXJHuoyuedu;
				JXJhuoyuedu->setID(atoi((*HuoyueduTab)(i, col++).c_str()));
				m_huoyuedumap[JXJhuoyuedu->getID()] = JXJhuoyuedu;
				JXJhuoyuedu->loadData();
			}
		}
	}
	else if(msg == WCH_ResetExtra)
	{
	}
	else
	{
		crRoleData::excHandle(msg); 
	}
}
void crJXJMainRoleData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJMainRoleInited:
		m_mainroleinited = param==NULL?false:*((bool*)param);
		break;
	case WCHDATA_Level:
		m_level = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_Experience:
		m_experience = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_JXJExploit:
		m_exploit = param==NULL?NULL:*((int*)param);
		break;
	//case WCHDATA_JXJJunling:
	//	m_junling = param==NULL?NULL:*((unsigned char*)param);
	//	break;
	case WCHDATA_JXJAchievement:
		m_achievement = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_JXJGovPost:
		m_govpost = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJNobility:
		m_nobility = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJAccomplishPoint:
		m_accomplishPoint = param==NULL?NULL:*((unsigned short*)param);
		break;
	//case WCHDATA_JXJMyApplyGroupIDVec:
	//	m_mygroupapplyid = param==NULL?NULL:*((int*)param);
	//	break;
	//case WCHDATA_JXJCampaignRanking:
	//	m_campaignranking = param==NULL?NULL:*((unsigned short*)param);
	//	break;
	case WCHDATA_JXJBingFa:
		m_bingfa = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJZhenFa:
		m_zhenfa = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJDongCha:
		m_dongcha = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_JXJTongShuai:
		m_tongshuai = param==NULL?NULL:*((unsigned short*)param);
		break;
	//case WCHDATA_JXJSoldierCount:
	//	m_soldierCount = param==NULL?NULL:*((short*)param);
	//	break;
	//case WCHDATA_JXJCombatPower:
	//	m_combatpower = param==NULL?NULL:*((short*)param);
	//	break;
	case WCHDATA_JXJCurrentEquipVecIndex:
		m_currentEquipIndex = param==NULL?NULL:*((unsigned char*)param);
		if(m_currentEquipIndex >= m_equipvecs.size())
		{
			m_currentEquipIndex = 0;
		}
		break;
	case WCHDATA_JXJSavedEquipVecs:
		m_openedEquipVec = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			// to do 赋值玩家权限
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJMainRoleData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			std::vector<float> v_i;
			std::vector<std::string> v_str;
			int nParam = 1;
			int version = 0;
			cfg_script.Get("VERSION",version);
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}
			
			unsigned char maxlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,0).c_str()));
			if(m_level>maxlv) m_level = maxlv;
			////////////////
			//领主已装备列表
			ref_ptr<crTableIO>equiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
			crTableIO::StrVec record;
			int int1;
			unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,0).c_str()));
			JXJLordEquipVec equipvec;
			equipvec.resize(equiponmax);
			if(cfg_script.Push("JXJEquipVec"))
			{
				//int nequipBagVec = 1;
				//while(cfg_script.Get("Param", int1,nequipBagVec++))
				//{//兼容老版本
				//	m_equipvec[nequipBagVec-2].first = int1;
				//	m_equipvec[nequipBagVec-2].second = 0;
				//}
				int nequipBagVec = 1;
				if(version<JXJVERSION_0001)
				{
					while(cfg_script.Get("Param", v_i,nequipBagVec++))
					{
						if(equiptab->queryOneRecord(0,crArgumentParser::appItoa((int)v_i[0]),record)>=0)
						{
							equipvec[nequipBagVec-2].first = v_i[0];
							equipvec[nequipBagVec-2].second = v_i[1];
						}
					}
				}
				else
				{
					while(cfg_script.Get("Param", v_str,nequipBagVec++))
					{
						if(equiptab->queryOneRecord(0,v_str[0],record)>=0)
						{
							equipvec[nequipBagVec-2].first = atoi(v_str[0].c_str());
							equipvec[nequipBagVec-2].second = atoi(v_str[1].c_str());
						}
					}
				}
				cfg_script.Pop();
			}
			if (cfg_script.Push("JXJEquipVecs"))
			{
				int nCount = 1;
				m_equipvecs.resize(m_openedEquipVec);
				while(cfg_script.Push("JXJEquipVec",nCount++))
				{
					JXJLordEquipVec equipVec(equiponmax);
					int nVec = 1;
					while(cfg_script.Get("Param", v_str,nVec++))
					{
						if(equiptab->queryOneRecord(0,v_str[0],record)>=0)
						{
							equipVec[nVec-2].first = atoi(v_str[0].c_str());
							equipVec[nVec-2].second = atoi(v_str[1].c_str());
						}
					}
					m_equipvecs[nCount-2] = equipVec;
					cfg_script.Pop();
				}
				if (m_currentEquipIndex >= m_equipvecs.size())
				{
					m_currentEquipIndex = 0;
				}
				cfg_script.Pop();
			}
			else if(version<JXJVERSION_0011)
			{
				m_equipvecs.resize(m_openedEquipVec);
				m_equipvecs[0]=equipvec;
			}
			//AccomplishMap
			unsigned short accid = 0;
			bool complete = false;
			ref_ptr<crTableIO>accomplishTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
			ref_ptr<crJXJAccomplish> accomplish;
			int rowcount = accomplishTab->getRowCount();
			int col = 0;
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				accomplish = new crJXJAccomplish;
				accomplish->setID(atoi((*accomplishTab)(i,col++).c_str()));
				m_accomplishMap[accomplish->getID()] = accomplish;
				accomplish->loadData();
			}
			AccomplishMap::iterator aitr;
			if(cfg_script.Push("AccomplishMap"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					aitr = m_accomplishMap.find(int1);
					if(aitr != m_accomplishMap.end())
					{
						accomplish = aitr->second.get();
						accomplish->setComplete(true);
					}
				}
				cfg_script.Pop();
			}

			/*if(cfg_script.Push("AccomplishMapStep"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					aitr = m_accomplishMap.find(atoi(v_str[0].c_str()));
					if(aitr != m_accomplishMap.end())
					{
						accomplish = aitr->second.get();
						accomplish->setStep(atoi(v_str[1].c_str()));
					}
				}
				cfg_script.Pop();
			}*/
			////HuodongMap
			//int huodongid = 0;
			//complete = false;
			//ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			//crTableIO::DataVec hdDataVec;
			//int openid = huodongTab->getTitleIndex("开启");
			//int displayId = huodongTab->getTitleIndex("仅显示");
			////int delFlagIDx = huodongTab->getTitleIndex("删除标记");

			//huodongTab->queryRecords(openid,"1",displayId,"0",hdDataVec);
			//ref_ptr<crJXJHuodong> huodong;
			//for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
			//	 itr != hdDataVec.end();
			//	 ++itr )
			//{
			//	huodong = new crJXJHuodong;
			//	huodong->setID(atoi((*itr)[0].c_str()));
			//	m_huodongMap[huodong->getID()] = huodong;
			//	huodong->loadData();
			//}
			//HuodongMap::iterator hitr;
			//if(cfg_script.Push("HuodongMap"))
			//{
			//	nParam = 1;
			//	int huodongID = 0;
			//	int step = 0;
			//	int complete = 0;
			//	int stepCount = 0;
			//	int usedTimes = 0;
			//	int delFlag = 0;
			//	std::string startTime;
			//	std::string endTime;

			//	if (version >= JXJVERSION_0009)
			//	{
			//		v_str.resize(6);
			//		while (cfg_script.Get("Param",v_str,nParam ++))
			//		{
			//			huodongID = atoi(v_str[0].c_str());
			//			step = atoi(v_str[1].c_str());
			//			complete = atoi(v_str[2].c_str());
			//			stepCount = atoi(v_str[3].c_str());
			//			usedTimes = atoi(v_str[4].c_str());
			//			delFlag = atoi(v_str[5].c_str());

			//			hitr = m_huodongMap.find(huodongID);
			//			if(hitr != m_huodongMap.end())
			//			{
			//				huodong = hitr->second.get();

			//				//int tmpFlag = -1;
			//				//if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(hitr->first),record)>=0)
			//				//	tmpFlag = atoi(record[delFlagIDx].c_str());
			//				//huodong->setDelFlag(tmpFlag);
			//				if (delFlag != huodong->getDelFlag())
			//				{
			//					// 该活动ID 已经被策划重用，判定为新的活动，旧的数据删除
			//					continue;
			//				}
			//				huodong->setStep(step,0,false);
			//				huodong->setComplete( (crJXJHuodong::CompleteStatus) complete);
			//				huodong->setStepCount(stepCount,0,false);
			//				huodong->setUsedTimes(usedTimes);
			//			}
			//		}
			//	}
			//	else
			//	{
			//		while (cfg_script.Get("Param", huodongID,nParam++))
			//		{
			//			startTime.clear();
			//			endTime.clear();
			//			delFlag = 0;
			//			hitr = m_huodongMap.find(huodongID);
			//			if(version<JXJVERSION_0002)
			//			{
			//				cfg_script.Get("Param", complete,nParam++);
			//			}
			//			else
			//			{
			//				if (version >= JXJVERSION_0003)
			//				{
			//					cfg_script.Get("Param",startTime,nParam ++);
			//					cfg_script.Get("Param",endTime,nParam ++);
			//				}
			//				cfg_script.Get("Param", step,nParam++);
			//				cfg_script.Get("Param", complete,nParam++);
			//				cfg_script.Get("Param", stepCount,nParam++);
			//				cfg_script.Get("Param", usedTimes,nParam++);
			//				if (version >= JXJVERSION_0007)
			//				{
			//					cfg_script.Get("Param", delFlag,nParam++);
			//				}
			//			}


			//			if(hitr != m_huodongMap.end())
			//			{
			//				huodong = hitr->second.get();

			//				//int tmpFlag = -1;
			//				//if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(hitr->first),record)>=0)
			//				//	tmpFlag = atoi(record[delFlagIDx].c_str());
			//				//huodong->setDelFlag(tmpFlag);
			//				if (delFlag != huodong->getDelFlag())
			//				{
			//					// 该活动ID 已经被策划重用，判定为新的活动，旧的数据删除
			//					continue;
			//				}

			//				if (version < JXJVERSION_0002)
			//				{
			//					huodong->setComplete( (crJXJHuodong::CompleteStatus) complete);
			//				}
			//				else
			//				{
			//					huodong->setStep(step,0,false);
			//					huodong->setComplete( (crJXJHuodong::CompleteStatus) complete);
			//					huodong->setStepCount(stepCount,0,false);
			//					huodong->setUsedTimes(usedTimes);
			//				}
			//			}
			//		}
			//	}
			//	cfg_script.Pop();
			//}

			//NewHuodongMap
			int huodongid = 0;
			complete = false;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			crTableIO::DataVec hdDataVec;
			int openid = huodongTab->getTitleIndex("开启");
			int displayId = huodongTab->getTitleIndex("仅显示");
			//int delFlagIDx = huodongTab->getTitleIndex("删除标记");

			huodongTab->queryRecords(openid,"1",displayId,"0",hdDataVec);
			ref_ptr<crJXJNewHuodong> huodong;
			for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
				 itr != hdDataVec.end();
				 ++itr )
			{
				huodong = new crJXJNewHuodong;
				huodong->setID(atoi((*itr)[0].c_str()));
				m_newHuodongMap[huodong->getID()] = huodong;
				huodong->loadData();
			}
			NewHuodongMap::iterator hitr;
			if(cfg_script.Push("NewHuodongMap"))
			{
				nParam = 1;
				unsigned short huodongID = 0;
				int step = 0;
				unsigned char activation = 0;
				unsigned char circles = 0;
				unsigned char delFlag = 0;

				v_str.resize(5);
				while (cfg_script.Get("Param",v_str,nParam ++))
				{
					huodongID = atoi(v_str[0].c_str());
					step = atoi(v_str[1].c_str());
					activation = atoi(v_str[2].c_str());
					circles = atoi(v_str[3].c_str());
					delFlag = atoi(v_str[4].c_str());

					hitr = m_newHuodongMap.find(huodongID);
					if(hitr != m_newHuodongMap.end())
					{
						huodong = hitr->second.get();
						if (delFlag != huodong->getDelFlag())
						{
							// 该活动ID 已经被策划重用，判定为新的活动，旧的数据删除
							continue;
						}
						huodong->setDelFlag(delFlag);
						huodong->setCircles(circles);
						//if(activation == 0 && huodongID == 300)
						//{
						//	if(huodong->isIntimeCheck())
						//		activation = 1;
						//}
						huodong->setActivation(activation,0);
						huodong->setStep(step,0);
					}
				}
				cfg_script.Pop();
			}
			//JXJNewGuideMap
			complete = false;
			ref_ptr<crTableIO>newguidetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTab);
			ref_ptr<crJXJNewGuide> jxjnewguide;
			rowcount = newguidetab->getRowCount();
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				jxjnewguide = new crJXJNewGuide;
				jxjnewguide->setID(atoi((*newguidetab)(i,col++).c_str()));
				m_newGuideMap[jxjnewguide->getID()] = jxjnewguide;
				jxjnewguide->loadData();
			}
			JXJNewGuideMap::iterator ngitr;
			if(cfg_script.Push("JXJNewGuideMap"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					ngitr = m_newGuideMap.find(int1);
					if(ngitr != m_newGuideMap.end())
					{
						jxjnewguide = ngitr->second.get();
						jxjnewguide->setComplete(true);
					}
				}
				cfg_script.Pop();
			}
			//HuoyueduMap
			complete = false;
			ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
			ref_ptr<crJXJHuoyuedu> jxjhuoyuedu;
			rowcount = HuoyueduTab->getRowCount();
			col = 0;
			for (int i = 0; i < rowcount; ++i)
			{
				col = 0;
				jxjhuoyuedu = new crJXJHuoyuedu;
				jxjhuoyuedu->setID(atoi((*HuoyueduTab)(i, col++).c_str()));
				m_huoyuedumap[jxjhuoyuedu->getID()] = jxjhuoyuedu;
				jxjhuoyuedu->loadData();
			}
			HuoyueduMap::iterator hyditr;
			if (cfg_script.Push("HuoyueduMap"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", v_str, nParam++))
				{
					hyditr = m_huoyuedumap.find(atoi(v_str[0].c_str()));
					if (hyditr != m_huoyuedumap.end())
					{
						jxjhuoyuedu = hyditr->second.get();
						jxjhuoyuedu->setStep(atoi(v_str[1].c_str()));
						//jxjhuoyuedu->setComplete(atoi(v_str[2].c_str()));
					}
				}
				cfg_script.Pop();
			}
			//HuoyueduRewardSet
			if (cfg_script.Push("HuoyueduRewardSet"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1, nParam++))
				{
					m_huoyuedurewardset.insert((unsigned short)int1);
				}
				cfg_script.Pop();
			}
			//
			cfg_script.Close();
			DeleteFileA(file);
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_level = dataStream->_readUChar();
			m_experience = dataStream->_readInt();
			m_exploit = dataStream->_readInt();
			//m_junling = dataStream->_readUChar();
			m_achievement = dataStream->_readInt();
			m_govpost = dataStream->_readUChar();
			m_nobility = dataStream->_readUChar();
			m_accomplishPoint = dataStream->_readUShort();
			m_appointColumnsVec = dataStream->_readUChar();
			m_changeNameTime = dataStream->_readInt64();
			//m_mygroupapplyid = dataStream->_readInt();
			//m_campaignranking = dataStream->_readUShort();
			//m_force = dataStream->_readUShort();
			//m_intellect = dataStream->_readUShort();
			//m_command = dataStream->_readUShort();
			//m_glamour = dataStream->_readUShort();
			//m_soldierCount = dataStream->_readShort();
			//m_combatpower = dataStream->_readShort();

			int size;
			//领主已装备列表
			//int bagnummax = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax).c_str()));
			m_currentEquipIndex = dataStream->_readUChar();
			m_openedEquipVec = dataStream->_readUChar();
			m_equipvecs.resize(m_openedEquipVec);
			for(int i = 0; i < m_openedEquipVec; ++i)
			{
				size = dataStream->_readUChar();
				JXJLordEquipVec equipVec(size);
				
				for (int j = 0; j < size; ++j)
				{
					equipVec[j].first = dataStream->_readInt();
					if(equipVec[j].first>0)
					{
						equipVec[j].second = dataStream->_readInt();
					}
				}
				m_equipvecs[i]=equipVec;
			}
			if (m_currentEquipIndex >= m_equipvecs.size())
			{
				m_currentEquipIndex = 0;
			}
			
			//领主套装列表
			//size = dataStream->_readUChar();
			//m_lordsuitmap.clear();
			//unsigned char lordsuitid = 0;
			//unsigned char lordsuitnum = 0;
			//for(i = 0; i<size; i++)
			//{
			//	lordsuitid = dataStream->_readUChar();
			//	if(lordsuitid>0)
			//	{
			//		lordsuitnum = dataStream->_readUChar();
			//		m_lordsuitmap.insert(std::make_pair(lordsuitid,lordsuitnum));
			//	}
			//}
			//AccomplishMap
			unsigned short accid = 0;
			bool complete = false;
			ref_ptr<crTableIO>accomplishTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
			ref_ptr<crJXJAccomplish> accomplish;
			int rowcount = accomplishTab->getRowCount();
			int col = 0;
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				accomplish = new crJXJAccomplish;
				accomplish->setID(atoi((*accomplishTab)(i,col++).c_str()));
				m_accomplishMap[accomplish->getID()] = accomplish;
				accomplish->loadData();
			}
			AccomplishMap::iterator aitr;
			size = dataStream->_readUShort();
			for(int i = 0; i<size; i++)
			{
				accid = dataStream->_readUShort();
				aitr = m_accomplishMap.find(accid);
				if(aitr != m_accomplishMap.end())
				{
					accomplish = aitr->second.get();
					accomplish->setComplete(true);
				}
			}
			/*unsigned char completestep = 0;
			size = dataStream->_readUShort();
			for(int i = 0; i<size; i++)
			{
				accid = dataStream->_readUShort();
				completestep = dataStream->_readUChar();
				aitr = m_accomplishMap.find(accid);
				if(aitr != m_accomplishMap.end())
				{
					accomplish = aitr->second.get();
					accomplish->setStep(completestep);
				}
			}*/
			////HuodongMap
			//int hdid = 0;
			//complete = false;
			//ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			//ref_ptr<crJXJHuodong> huodong;
			//crTableIO::DataVec hdDataVec;
			//int openid = huodongTab->getTitleIndex("开启");
			//huodongTab->queryRecords(openid,"1",hdDataVec);
			//for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
			//	itr != hdDataVec.end();
			//	++itr )
			//{
			//	huodong = new crJXJHuodong;
			//	huodong->setID(atoi((*itr)[0].c_str()));
			//	m_huodongMap[huodong->getID()] = huodong;
			//	huodong->loadData();
			//}
			//HuodongMap::iterator hitr;
			//int param2 = 0,param3 = 0;
			//int step = 0;
			//unsigned char completeSta = 0;
			//int stepCount = 0;
			//unsigned short usedTimes = 0;
			//int delFlag = 0;
			//size = dataStream->_readUChar();
			//for(i = 0; i<size; i++)
			//{
			//	hdid = dataStream->_readUShort();
			//	step = dataStream->_readInt();
			//	completeSta = dataStream->_readUChar();
			//	stepCount = dataStream->_readInt();
			//	usedTimes = dataStream->_readUShort();
			//	delFlag = dataStream->_readInt();

			//	hitr = m_huodongMap.find(hdid);
			//	if(hitr != m_huodongMap.end())
			//	{
			//		huodong = hitr->second.get();
			//		huodong->setStep(step,0,false);
			//		huodong->setComplete( (crJXJHuodong::CompleteStatus) completeSta );
			//		huodong->setStepCount(stepCount,0,false);
			//		huodong->setUsedTimes(usedTimes);
			//		huodong->setDelFlag(delFlag);

			//		//if(crGlobalHandle::isClient())
			//		//{
			//		//	char gbuf[256];
			//		//	sprintf(gbuf,"活动 %d 接收到服务器状态初始化同步 complete %d, step %d, stepcount %d usedtimes %d\n\0",
			//		//		hdid,completeSta ,step,stepCount,usedTimes);
			//		//	gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			//		//}
			//	}
			//}
			////int huodongId = 0;
			////huodongTab->queryRecords(openid,"0",hdDataVec);
			////for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
			////	itr != hdDataVec.end();
			////	++itr )
			////{
			////	hitr = m_huodongMap.find(atoi((*itr)[0].c_str()));
			////	if (m_huodongMap.end() != hitr)
			////	{
			////		m_huodongMap.erase(hitr);
			////	}
			////}

			//NewHuodongMap
			int hdid = 0;
			complete = false;
			ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
			ref_ptr<crJXJNewHuodong> huodong;
			crTableIO::DataVec hdDataVec;
			int openid = huodongTab->getTitleIndex("开启");
			huodongTab->queryRecords(openid,"1",hdDataVec);
			for( crTableIO::DataVec::iterator itr = hdDataVec.begin();
				itr != hdDataVec.end();
				++itr )
			{
				huodong = new crJXJNewHuodong;
				huodong->setID(atoi((*itr)[0].c_str()));
				m_newHuodongMap[huodong->getID()] = huodong;
				huodong->loadData();
			}
			NewHuodongMap::iterator hitr;
			int step = 0;
			unsigned char activation = 0;
			unsigned char circles = 0;
			unsigned char delFlag = 0;
			size = dataStream->_readUShort();
			for(int i = 0; i<size; i++)
			{
				hdid = dataStream->_readUShort();
				activation = dataStream->_readUChar();
				step = dataStream->_readInt();
				circles = dataStream->_readUChar();
				delFlag = dataStream->_readUChar();

				hitr = m_newHuodongMap.find(hdid);
				if(hitr != m_newHuodongMap.end())
				{
					huodong = hitr->second.get();
					huodong->setDelFlag(delFlag);
					huodong->setCircles(circles);
					huodong->setActivation(activation,0);
					huodong->setStep(step,0);
				}
			}

			//JXJNewGuideMap
			unsigned short guideid = 0;
			complete = false;
			ref_ptr<crTableIO>newguidetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJNewguideTab);
			ref_ptr<crJXJNewGuide> jxjnewguide;
			rowcount = newguidetab->getRowCount();
			for(int i = 0; i<rowcount; ++i)
			{
				col = 0;
				jxjnewguide = new crJXJNewGuide;
				jxjnewguide->setID(atoi((*newguidetab)(i,col++).c_str()));
				m_newGuideMap[jxjnewguide->getID()] = jxjnewguide;
				jxjnewguide->loadData();
			}
			JXJNewGuideMap::iterator ngitr;
			size = dataStream->_readUShort();
			for(int i = 0; i<size; i++)
			{
				guideid = dataStream->_readUShort();
				ngitr = m_newGuideMap.find(guideid);
				if(ngitr != m_newGuideMap.end())
				{
					jxjnewguide = ngitr->second.get();
					jxjnewguide->setComplete(true);
				}
			}
			//HuoyueduMap活跃度
			unsigned short hydid = 0;
			unsigned char hydstep = 0;
			bool hydcomplete = false;
			ref_ptr<crTableIO>HuoyueduTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuoyueduTab);
			ref_ptr<crJXJHuoyuedu> huoyuedu;
			rowcount = HuoyueduTab->getRowCount();
			col = 0;
			for (int i = 0; i < rowcount; ++i)
			{
				col = 0;
				huoyuedu = new crJXJHuoyuedu;
				huoyuedu->setID(atoi((*HuoyueduTab)(i, col++).c_str()));
				m_huoyuedumap[huoyuedu->getID()] = huoyuedu;
				huoyuedu->loadData();
			}
			HuoyueduMap::iterator hyditr;
			size = dataStream->_readUChar();
			for (int i = 0; i < size; i++)
			{
				hydid = dataStream->_readUShort();
				hydstep = dataStream->_readUChar();
				//hydcomplete = dataStream->_readBool();
				hyditr = m_huoyuedumap.find(hydid);
				if (hyditr != m_huoyuedumap.end())
				{
					huoyuedu = hyditr->second.get();
					huoyuedu->setStep(hydstep);
					//huoyuedu->setComplete(hydcomplete);
				}
			}
			//HuoyueduRewardSet
			unsigned short hydrewardid = 0;
			size = dataStream->_readUChar();
			for (int i = 0; i < size; i++)
			{
				hydrewardid = dataStream->_readUShort();
				m_huoyuedurewardset.insert(hydrewardid);
			}
			m_qiyutaskid = dataStream->_readInt();
			m_qiyuCompleteCount = dataStream->_readUChar();
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_NetStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_level = dataStream->_readUChar();
			m_govpost= dataStream->_readUChar();
			//m_nobility= dataStream->_readUChar();
			//m_campaignranking= dataStream->_readUShort();
			//m_force = dataStream->_readUShort();
			//m_intellect = dataStream->_readUShort();
			//m_command = dataStream->_readUShort();
			//m_glamour = dataStream->_readUShort();
			//m_combatpower = dataStream->_readShort();

			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_JXJAppointColumnsVec:
		m_appointColumnsVec = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_JXJChangeNameTime:
		m_changeNameTime = param==NULL?0L:*((_crInt64*)param);
		break;
	case WCHDATA_JXJQiyuTaskID:
		m_qiyutaskid = param==NULL?0L:*((int*)param);
		break;
	case WCHDATA_JXJQiyuCompleteCount:
		m_qiyutaskid = param==NULL?0L:*((unsigned char*)param);
		break;
	default:
		crRoleData::inputParam(i,param);
	}
}
void crJXJMainRoleData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJMainRoleInited:
		param = &m_mainroleinited;
		break;
	case WCHDATA_Level:
		param = &m_level;
		break;
	case WCHDATA_Experience:
		param = &m_experience;
		break;
	case WCHDATA_JXJExploit:
		param = &m_exploit;
		break;
	//case WCHDATA_JXJJunling:
	//	param = &m_junling;
	//	break;
	case WCHDATA_JXJAchievement:
		param = &m_achievement;
		break;
	case WCHDATA_JXJGovPost:
		param = &m_govpost;
		break;
	case WCHDATA_JXJNobility:
		param = &m_nobility;
		break;
	case WCHDATA_JXJAccomplishPoint:
		param = &m_accomplishPoint;
		break;
	//case WCHDATA_JXJCampaignRanking:
	//	param = &m_campaignranking;
	//	break;
	case WCHDATA_JXJBingFa:
		param = &m_bingfa;
		break;
	case WCHDATA_JXJZhenFa:
		param = &m_zhenfa;
		break;
	case WCHDATA_JXJDongCha:
		param = &m_dongcha;
		break;
	case WCHDATA_JXJTongShuai:
		param = &m_tongshuai;
		break;
	//case WCHDATA_JXJSoldierCount:
	//	param = &m_soldierCount;
	//	break;
	//case WCHDATA_JXJCombatPower:
	//	param = &m_combatpower;
	//	break;
	case WCHDATA_JXJLordEquipVec:
		param = &m_equipvecs[m_currentEquipIndex];
		break;
	case WCHDATA_JXJLordEquipVecs:
		param = &m_equipvecs;
		break;
	case WCHDATA_JXJCurrentEquipVecIndex:
		param = &m_currentEquipIndex;
		break;
	case WCHDATA_JXJSavedEquipVecs:
		param = &m_openedEquipVec;
		break;
	//case WCHDATA_JXJLordSuitMap:
	//	param = &m_lordsuitmap;
	//	break;
	case WCHDATA_JXJAccomplishMap:
		param = &m_accomplishMap;
		break;
	case WCHDATA_JXJNewHuodongMap:
		param = &m_newHuodongMap;
		break;
	case WCHDATA_JXJNewGuideMap:
		param = &m_newGuideMap;
		break;
	case WCHDATA_JXJHuoyueduMap:
		param = &m_huoyuedumap;
		break;
	case WCHDATA_JXJHuoyueduRewardSet:
		param = &m_huoyuedurewardset;
		break;
	case WCHDATA_JXJAppointColumnsVec:
		param = &m_appointColumnsVec;
		break;
	case WCHDATA_JXJChangeNameTime:
		param = &m_changeNameTime;
		break;
	case WCHDATA_JXJQiyuTaskID:
		param = &m_qiyutaskid;
		break;
	case WCHDATA_JXJQiyuCompleteCount:
		param = &m_qiyuCompleteCount;
		break;
	default:
		crRoleData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJMetierData
//
/////////////////////////////////////////
GNE::Mutex crJXJMetierData::m_saveMutex;
crJXJMetierData::crJXJMetierData():
	m_currentTaskID(0)
{
}
crJXJMetierData::crJXJMetierData(const crJXJMetierData& data):
	crData(data),
	m_gameTaskMap(data.m_gameTaskMap),
	m_currentTaskID(0)
{
}
crJXJMetierData::~crJXJMetierData()
{
	GNE::LockMutex lock( m_dataMutex );
	for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
		itr != m_gameTaskMap.end();
		++itr )
	{
		itr->second->clearData();
	}
	m_gameTaskMap.clear();
}
void crJXJMetierData::addParam(int i, const std::string& str)
{
}
void crJXJMetierData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[128];

		ref_ptr<crGameTask> gameTask;
		std::vector<float> v_i;
		int nGameTask = 1;
		for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
			itr != m_gameTaskMap.end();
			++itr )
		{
			if(itr->second->isNeedSave())
			{
				cfg_script.Add("GameTaskMap");
				cfg_script.Push("GameTaskMap",nGameTask++);

				gameTask = itr->second.get();
				sprintf(buf,"%d\0",(int)(gameTask->getTaskID()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getActivationFlg()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getTaskProgress()));
				cfg_script.Add("Param",buf);
				sprintf(buf,"%d\0",(int)(gameTask->getCircles()));
				cfg_script.Add("Param",buf);

				cfg_script.Pop();
			}
		}

		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		///////
		std::vector< ref_ptr<crGameTask> > GameTaskVec;
		ref_ptr<crGameTask> gameTask;
		for( GameTaskMap::iterator itr = m_gameTaskMap.begin();
			itr != m_gameTaskMap.end();
			++itr )
		{
			gameTask = itr->second.get();
			if(gameTask->isNeedSave())
			{
				GameTaskVec.push_back(gameTask.get());
			}
		}
		stream->createBuf(10*GameTaskVec.size()+1);
		stream->_writeUShort(GameTaskVec.size());
		for( std::vector< ref_ptr<crGameTask> >::iterator itr = GameTaskVec.begin();
			itr != GameTaskVec.end();
			++itr )
		{
			gameTask = itr->get();
			stream->_writeInt(gameTask->getTaskID());
			stream->_writeChar(gameTask->getActivationFlg());
			stream->_writeInt(gameTask->getTaskProgress());
			stream->_writeUChar(gameTask->getCircles());
		}
		stream->seekBegin();
		//传输走数据流,不在这里做压缩
	}
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crJXJMetierData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_GameTaskTab:
		{
			std::string *file = (std::string *)param;
			ref_ptr<crTableIO> tasktab = crTableIO::openFile(*file);
			if(tasktab.valid())
			{
				ref_ptr<crGameTask> gameTask;
				int rowcount = tasktab->getRowCount();
				//crVector2 awardItem;
				crVector2s lvRange;
				int col = 0;
				crVector4b taskStep;
				std::string datafile;
				int overdue = tasktab->getTitleIndex("过期");
				for(int i = 0; i<rowcount; ++i)
				{
					col = 0;
					if(overdue>=0 && atoi((*tasktab)(i,overdue).c_str())==1)
						continue;
					gameTask = new crGameTask;
					gameTask->setTaskID(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setTaskName((*tasktab)(i,col++));
					gameTask->setHyperText((*tasktab)(i,col++));
					gameTask->setCircles(atoi((*tasktab)(i,col++).c_str()));
					if(crArgumentParser::appAtoVec((*tasktab)(i,col++),lvRange))
						gameTask->setLvRange(lvRange);
					gameTask->setPreTask(atoi((*tasktab)(i,col++).c_str()));
					taskStep.set(0,0,0,0);
					crArgumentParser::appAtoVec((*tasktab)(i,col++),taskStep);
					gameTask->setTaskSteps(taskStep);
					gameTask->setTaskStepType((unsigned char)(atoi((*tasktab)(i,col++).c_str())));
					gameTask->setExp(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setExp2(atoi((*tasktab)(i,col++).c_str()));
					gameTask->setGold(atoi((*tasktab)(i,col++).c_str()));
					if(crGlobalHandle::isClient())
					{
						datafile = (*tasktab)(i,col++);
						if(!datafile.empty())
							gameTask->setDataClass(CREncapsulation::loadData(crGlobalHandle::getInstance()->getScriptDir()+datafile.c_str()));
					}
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//gameTask->addAwardItemMap(atoi((*tasktab)(i,col++).c_str()));
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					//if(crArgumentParser::appAtoVec((*tasktab)(i,col++),awardItem))
					//	gameTask->addAwardItemMap(awardItem[0],awardItem[1]);
					m_gameTaskMap[gameTask->getTaskID()] = gameTask.get();
				}
			}
		}
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJMetierData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			int nParam = 1;
			int nGameTask = 1;
			ref_ptr<crGameTask> gameTask;
			int gametaskid;
			GameTaskMap::iterator taskitr;
			while(cfg_script.Push("GameTaskMap",nGameTask++))
			{
				nParam = 1;
				cfg_script.Get("Param", str,nParam++);
				gametaskid = atoi(str.c_str());
				taskitr = m_gameTaskMap.find(gametaskid);
				if(taskitr != m_gameTaskMap.end())
				{
					gameTask = taskitr->second.get();
					cfg_script.Get("Param", str,nParam++);
					gameTask->setActivationFlg(atoi(str.c_str()));
					cfg_script.Get("Param", str,nParam++);
					gameTask->setTaskProgress(atoi(str.c_str()));
					cfg_script.Get("Param", str,nParam++);
					gameTask->setCircles(atoi(str.c_str()));
				}
				cfg_script.Pop();
			}
			cfg_script.Close();
			DeleteFileA(file);
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			unsigned short size = dataStream->_readUShort();
			int i;
			ref_ptr<crGameTask> gameTask;
			int gametaskid;
			GameTaskMap::iterator itr;
			for(i = 0; i<size; i++)
			{
				gametaskid = dataStream->_readInt();
				itr = m_gameTaskMap.find(gametaskid);
				if(itr != m_gameTaskMap.end())
				{
					gameTask = itr->second.get();
					if(gameTask.valid())
					{
						gameTask->setActivationFlg(dataStream->_readChar());
						gameTask->setTaskProgress(dataStream->_readInt());
						gameTask->setCircles(dataStream->_readUChar());
					}
				}
				else
				{
					dataStream->seek(6);
				}
			}
			//m_dataStream = NULL;
		}
	case WCHDATA_CurrentTaskID:
		m_currentTaskID = param==NULL?NULL:*((int*)param);
		break;
	}
}
void crJXJMetierData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_GameTaskMap:
		param = &m_gameTaskMap;
		break;
	case WCHDATA_CurrentTaskID:
		param = &m_currentTaskID;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crJXJTechUIData
//
/////////////////////////////////////////
crJXJTechUIData::crJXJTechUIData():
	m_techid(0)
{
}
crJXJTechUIData::crJXJTechUIData(const crJXJTechUIData& data):
	crUIData(data),
	m_techid(data.m_techid)
{
}
void crJXJTechUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJTechID:
		m_techid = param==NULL?NULL:*((int*)param);
		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJTechUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJTechID:
		param = &m_techid;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJRoleData
//
/////////////////////////////////////////
crJXJRoleData::crJXJRoleData():
	m_level(1),
	m_jxjArmyCount(1),
	m_rtArmyCount(1),
	m_troopType(JXJTroopsType_Bubing),
	m_npcItemType(NPC_Common),
	m_attackgainmp(0),
	m_damagegainmp(0),
	m_extraattackgainmp(0),
	m_extraattackgainmp_rd(0),
	m_extrauseitemreducemp(0),
	m_extrauseitemreducemp_rd(0),
	m_extradamagegainmp(0),
	m_extradamagegainmp_rd(0),
	m_extraRevertType(0),
	m_extraRevertType_rd(0),
	m_oldExtraId(0),
	m_chenghao(ChengHao_No)
{
}
crJXJRoleData::crJXJRoleData(const crJXJRoleData& data):
	crRoleData(data),
	m_level(data.m_level),
	m_jxjArmyCount(data.m_jxjArmyCount),
	m_rtArmyCount(data.m_rtArmyCount),
	m_troopType(data.m_troopType),
	m_npcItemType(data.m_npcItemType),
	m_attackgainmp(data.m_attackgainmp),
	m_damagegainmp(data.m_damagegainmp),
	m_extraattackgainmp(0),
	m_extraattackgainmp_rd(0),
	m_extrauseitemreducemp(0),
	m_extrauseitemreducemp_rd(0),
	m_extradamagegainmp(0),
	m_extradamagegainmp_rd(0),
	m_extraRevertType(0),
	m_extraRevertType_rd(0),
	m_oldExtraId(data.m_oldExtraId),
	m_chenghao(data.m_chenghao)
{
}
void crJXJRoleData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_Level:
		m_level = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJArmyCount:
		m_jxjArmyCount = (/*unsigned short*/int)(atoi(relStr.c_str()));
		break;
	default:
		crRoleData::addParam(i,str);
		break;
	}
}
void crJXJRoleData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		//ItemMap itemMap;
		//rcfg::ConfigScript cfg_script(&itemMap);
		//char buf[64];
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJArmyCount,(int)m_jxjArmyCount);
		//cfg_script.Add("Param",buf);
		//{
		//	GNE::LockMutex lock( m_saveMutex );
		//	sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
		//	cfg_script.Write(buf,"");
		//	cfg_script.Close();

		//	m_dataStream = new crStreamBuf;
		//	m_dataStream->loadFromFile2(buf);
		//	DeleteFileA(buf);
		//}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		//m_dataStream = new crStreamBuf;
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(1024);

		stream->_writeInt(m_jxjArmyCount);
		stream->_writeInt(m_rtArmyCount);
		stream->_writeShort(m_crit);
		stream->_writeShort(m_parry);
		stream->_writeShort(m_dodge);

		stream->_writeShort(m_sightRange);
		stream->_writeShort(m_guardRange);
		stream->_writeShort(m_eyeHeight);
		stream->_writeInt(m_hp);
		stream->_writeInt(m_mp);
		stream->_writeUChar(m_level);
		stream->_writeInt(m_attack);
		stream->_writeInt(m_defend);//2*7
		stream->_writeShort(m_magicDefend);//2*7
		stream->_writeShort(m_speed);
		stream->_writeShort(m_stealth);
		stream->_writeShort(m_stealthspy);
		stream->_writeShort(m_hitratio);
		stream->_writeShort(m_attackSpeed);
		stream->_writeShort(m_skillSpeed);
		stream->_writeUChar(m_armourType);
		//道具
		stream->_writeUChar(m_attackMap.size());
		for( AttackMap::iterator itr = m_attackMap.begin();
			itr != m_attackMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_skillMap.size());
		//for( SkillMap::iterator itr = m_skillMap.begin();
		//	itr != m_skillMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		stream->_writeUChar(m_equipOnMap.size());
		for( EquipOnMap::iterator itr = m_equipOnMap.begin();
			itr != m_equipOnMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		stream->_writeUChar(m_camp);
		//天赋
		stream->_writeShort(m_inbornHPPercent);
		stream->_writeShort(m_inbornMPPercent);
		stream->_writeShort(m_inbornAttackPercent);
		stream->_writeShort(m_inbornDefendPercent);
		stream->_writeShort(m_inbornMagicDefendPercent);
		stream->_writeShort(m_inbornBuildAttackPercent);
		stream->_writeShort(m_inbornSpeed);
		stream->_writeShort(m_inbornAttackSpeedPercent);
		stream->_writeShort(m_inbornSkillSpeedPercent);
		stream->_writeShort(m_inbornSightRange);
		stream->_writeShort(m_inbornAttackDist);

		stream->_writeShort(m_inbornStealth);
		stream->_writeShort(m_inbornStealthSpy);
		stream->_writeShort(m_inbornCrit);
		stream->_writeShort(m_inbornParry);
		stream->_writeShort(m_inbornDodge);
		stream->_writeShort(m_inbornIgnoreDefend);
		stream->_writeShort(m_inbornIgnoreMagicDefend);
		stream->_writeUChar(m_inbornIgnoreDodge);
		stream->_writeShort(m_inbornAttackDamageBack);
		stream->_writeShort(m_inbornSkillDamageBack);
		stream->_writeShort(m_inbornAttackAbsorbHP);
		stream->_writeShort(m_inbornSkillAbsorbHP);
		stream->_writeShort(m_inbornCritPower);
		stream->_writeInt(m_inbornSputtering);
		stream->_writeShort(m_inbornAttackDamage);
		stream->_writeShort(m_inbornSkillDamage);
		stream->_writeShort(m_inbornHitratioPercent);
		stream->_writeUChar(m_npcItemType);
		stream->_writeShort(m_attackgainmp);
		stream->_writeShort(m_damagegainmp);
		stream->_writeShort(m_extraattackgainmp);
		stream->_writeShort(m_extraattackgainmp_rd);
		stream->_writeShort(m_extrauseitemreducemp);
		stream->_writeShort(m_extrauseitemreducemp_rd);
		stream->_writeUChar(m_extraRevertType);
		stream->_writeUChar(m_extraRevertType_rd);
		stream->_writeShort(m_extradamagegainmp);
		stream->_writeShort(m_extradamagegainmp_rd);
		stream->_writeUChar(m_chenghao);

		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_BuildNetStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		crStreamBuf *stream = (crStreamBuf*)(HIINT64(msg));
		stream->createBuf(1024);
		///////
		stream->_writeInt(m_jxjArmyCount);
		stream->_writeInt(m_rtArmyCount);
		stream->_writeShort(m_crit);
		stream->_writeShort(m_parry);
		stream->_writeShort(m_dodge);

		stream->_writeShort(m_sightRange);
		//stream->_writeShort(m_guardRange);
		stream->_writeShort(m_eyeHeight);
		stream->_writeInt(m_hp);
		stream->_writeInt(m_mp);
		stream->_writeUChar(m_level);
		stream->_writeInt(m_attack);
		stream->_writeInt(m_defend);//2*7
		stream->_writeShort(m_magicDefend);
		stream->_writeShort(m_speed);
		stream->_writeShort(m_attackSpeed);
		stream->_writeShort(m_skillSpeed);
		stream->_writeUChar(m_armourType);
		//道具
		stream->_writeUChar(m_attackMap.size());
		for( AttackMap::iterator itr = m_attackMap.begin();
			itr != m_attackMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		//stream->_writeUChar(m_skillMap.size());
		//for( SkillMap::iterator itr = m_skillMap.begin();
		//	itr != m_skillMap.end();
		//	++itr )
		//{
		//	stream->_writeInt(itr->first);
		//	stream->_writeInt(itr->second);
		//}
		stream->_writeUChar(m_equipOnMap.size());
		for( EquipOnMap::iterator itr = m_equipOnMap.begin();
			itr != m_equipOnMap.end();
			++itr )
		{
			stream->_writeInt(itr->first);
			stream->_writeInt(itr->second);
		}
		stream->_writeUChar(m_camp);
		//天赋
		stream->_writeShort(m_inbornHPPercent);
		stream->_writeShort(m_inbornMPPercent);
		stream->_writeShort(m_inbornAttackPercent);
		stream->_writeShort(m_inbornDefendPercent);
		stream->_writeShort(m_inbornMagicDefendPercent);
		stream->_writeShort(m_inbornBuildAttackPercent);
		stream->_writeShort(m_inbornSpeed);
		stream->_writeShort(m_inbornAttackSpeedPercent);
		stream->_writeShort(m_inbornSkillSpeedPercent);
		stream->_writeShort(m_inbornSightRange);
		stream->_writeShort(m_inbornAttackDist);

		stream->_writeShort(m_inbornStealth);
		stream->_writeShort(m_inbornStealthSpy);
		stream->_writeShort(m_inbornCrit);
		stream->_writeShort(m_inbornParry);
		stream->_writeShort(m_inbornDodge);
		stream->_writeShort(m_inbornIgnoreDefend);
		stream->_writeShort(m_inbornIgnoreMagicDefend);
		stream->_writeUChar(m_inbornIgnoreDodge);
		stream->_writeShort(m_inbornAttackDamageBack);
		stream->_writeShort(m_inbornSkillDamageBack);
		stream->_writeShort(m_inbornAttackAbsorbHP);
		stream->_writeShort(m_inbornSkillAbsorbHP);
		stream->_writeShort(m_inbornCritPower);
		stream->_writeInt(m_inbornSputtering);
		stream->_writeShort(m_inbornAttackDamage);
		stream->_writeShort(m_inbornSkillDamage);
		stream->_writeShort(m_inbornHitratioPercent);
		stream->_writeUChar(m_npcItemType);
		stream->_writeUChar(m_chenghao);
		stream->seekBegin();
	}
	else if(LOINT64(msg) == WCH_ServerCheckRoleData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
		serverCheckData(item,WCH_ServerCheckRoleData);
		//item->beginCreateItemChild(WCH_ServerCheckRoleData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_ServerCheckItemData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		//item->getCreateItemChildLock();
		//int count = 0;
		serverCheckData(item,WCH_ServerCheckItemData);
		//item->beginCreateItemChild(WCH_ServerCheckItemData,count);
		//if(count == 0)
		//{
		//	item->completeCreateItemChild();
		//}
	}
	else if(LOINT64(msg) == WCH_CheckNetData/*WCH_CheckPlayerData*/)
	{
		//GNE::LockMutex lock( m_dataMutex );避免与crGlobalHandle的互斥信号产生互锁现象
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		clientCheckData(item);
	}
	else if(LOINT64(msg) == WCH_GameServerCheckData)
	{
		//GNE::LockMutex lock( m_dataMutex );
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		gameServerCheckData(item);
	} 
	else if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_InitData)
	{
		crInstanceItem *item = (crInstanceItem *)(HIINT64(msg));
		float hp = 0.0f;
		item->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&hp,NULL));
		{
			if(m_guiseState & GS_StaticUnVisiable || m_guiseState & GS_Static || m_guiseState & GS_StaticNoneBlock)
				m_guiseState = GS_Static;
			else
				m_guiseState = GS_Normal;
			if(m_guiseState == GS_Normal)
			{//GS_Static没有buf
				if(!m_extraDataClass.valid())
					m_extraDataClass = new crRpgGameExtraData;
				else
					m_extraDataClass->clear();//ExtraData lock > datalock
			}
			//GNE::LockMutex lock( m_dataMutex );
			m_dataMutex.acquire();
			m_targetType = Target_None;
			//m_targetNode = NULL;
			m_targetID = 0;
			m_targetRoleID = 0;
			m_targetPosition.set(0,0,0);
			m_moveToPosition.set(0,0,0);
			m_itemState = IS_Relive;

			m_oldExtraId = 0;

			m_followDistance = crGlobalHandle::gData()->gMinFollowDistance();
			m_rtArmyCount = m_jxjArmyCount;
			m_rthp = hp * m_rtArmyCount;
			//float mp = 0.0f;
			//item->doEvent(MAKEINT64(WCH_GetMP,NULL),MAKEINT64(&mp,NULL));
			m_rtmp = 0;//mp * m_rtArmyCount;

			m_currentAttackID = 0;
			m_aboutToUseItemID = 0;
			m_fightmode = FM_Peace;
			m_patrolIndex = 0;
			m_enmityMap.clear();
			m_inpatrolEnemyMap.clear();

			m_extrahp = 0;
			m_extramp = 0;
			m_extraAttack = 0;
			m_extraDefend = 0;
			//m_extraSpeed = 0;
			m_extraStealth = 0;
			m_extraStealthSpy = 0;
			m_extraHitratioPercent = 0;
			m_extraAttackSpeed = 0.0f;
			m_extraSkillSpeed = 0.0f;
			m_extraGuise = GS_Normal;
			m_extraRadius = 100;
			m_extraIgnoreDefend = 0;
			m_extraIgnoreMagicDefend = 0;
			m_extraSightRange = 0;

			m_extraCrit = 0;
			m_extraParry = 0;
			m_extraDodge = 0;
			m_isCrit = false;
			m_isParry = false;
			m_isDodge = false;
			m_isMiss = false;

			m_extraAttackPercent = 0;
			m_extraSpeedPercent = 0;
			m_extraDefendPercent = 0;
			m_extraMagicDefendPercent = 0;
			m_extraBuildAttackPercent = 0;
			m_extraCritPercent = 0;
			m_extraParryPercent = 0;
			m_extraDodgePercent = 0;
			m_extraAttackDist = 0;
			m_extraAttackAbsorbHP = 0;
			m_extraSkillAbsorbHP = 0;
			m_extraAttackDamageBack = 0;
			m_extraSkillDamageBack = 0;
			m_extraRTHP = 0.0f;
			m_extraAttackDamage = 0;
			m_extraSkillDamage = 0;
			m_extraIgnoreDodge = 0;
			m_extraCritPower = 0;
			m_extraSputtering = 0;
			m_extraattackgainmp = 0;
			m_extraattackgainmp_rd = 0;
			m_extrauseitemreducemp = 0;
			m_extrauseitemreducemp_rd = 0;
			m_extradamagegainmp = 0;
			m_extradamagegainmp_rd = 0;

			m_extraRevertType = 0;
			m_extraRevertType_rd = 0;
			
			m_extraParasiticItem = NULL;
			m_extraParasiticItem_rd = NULL;
			m_dataMutex.release();
		}
	}
	else if(msg == WCH_ResetExtra)
	{
		GNE::LockMutex lock( m_dataMutex );
		m_extrahp_rd = m_extrahp;
		m_extrahp = 0;
		m_extramp_rd = m_extramp;
		m_extramp = 0;
		m_extraAttack_rd = m_extraAttack;
		m_extraAttack = 0;
		m_extraDefend_rd = m_extraDefend;
		m_extraDefend = 0;
		//m_extraSpeed_rd = m_extraSpeed;
		//m_extraSpeed = 0;
		m_extraStealth_rd = m_extraStealth;
		m_extraStealth = 0;
		m_extraStealthSpy_rd = m_extraStealthSpy;
		m_extraStealthSpy = 0;
		m_extraHitratioPercent_rd = m_extraHitratioPercent;
		m_extraHitratioPercent = 0;
		m_extraAttackSpeed_rd = m_extraAttackSpeed;
		m_extraAttackSpeed = 0;
		m_extraSkillSpeed_rd = m_extraSkillSpeed;
		m_extraSkillSpeed = 0;
		m_extraGuise_rd = m_extraGuise;
		m_extraGuise = GS_Normal;
		m_extraRadius_rd = m_extraRadius;
		m_extraRadius = 100;

		m_extraIgnoreDefend_rd = m_extraIgnoreDefend;
		m_extraIgnoreDefend = 0;
		m_extraIgnoreMagicDefend_rd = m_extraIgnoreMagicDefend;
		m_extraIgnoreMagicDefend = 0;
		m_extraSightRange_rd = m_extraSightRange;
		m_extraSightRange = 0;

		m_extraCrit_rd = m_extraCrit;
		m_extraCrit = 0;
		m_extraParry_rd = m_extraParry;
		m_extraParry = 0;
		m_extraDodge_rd = m_extraDodge;
		m_extraDodge = 0;

		m_extraAttackPercent_rd = m_extraAttackPercent;
		m_extraAttackPercent = 0;
		m_extraSpeedPercent_rd = m_extraSpeedPercent;
		m_extraSpeedPercent = 0;
		m_extraDefendPercent_rd = m_extraDefendPercent;
		m_extraDefendPercent = 0;
		m_extraMagicDefendPercent_rd = m_extraMagicDefendPercent;
		m_extraMagicDefendPercent = 0;
		m_extraBuildAttackPercent_rd = m_extraBuildAttackPercent;
		m_extraBuildAttackPercent = 0;
		m_extraCritPercent_rd = m_extraCritPercent;
		m_extraCritPercent = 0;
		m_extraParryPercent_rd = m_extraParryPercent;
		m_extraParryPercent = 0;
		m_extraDodgePercent_rd = m_extraDodgePercent;
		m_extraDodgePercent = 0;
		m_extraAttackDist_rd = m_extraAttackDist;
		m_extraAttackDist = 0;
		m_extraAttackAbsorbHP_rd = m_extraAttackAbsorbHP;
		m_extraAttackAbsorbHP = 0;
		m_extraSkillAbsorbHP_rd = m_extraSkillAbsorbHP;
		m_extraSkillAbsorbHP = 0;
		m_extraAttackDamageBack_rd = m_extraAttackDamageBack;
		m_extraAttackDamageBack = 0;
		m_extraSkillDamageBack_rd = m_extraSkillDamageBack;
		m_extraSkillDamageBack = 0;
		//m_extraRTHP_rd = m_extraRTHP;
		//m_extraRTHP = 0.0f;
		m_extraAttackDamage_rd = m_extraAttackDamage;
		m_extraAttackDamage = 0;
		m_extraSkillDamage_rd = m_extraSkillDamage;
		m_extraSkillDamage = 0;
		m_extraIgnoreDodge_rd = m_extraIgnoreDodge;
		m_extraIgnoreDodge = 0;
		m_extraCritPower_rd = m_extraCritPower;
		m_extraCritPower = 0;
		m_extraSputtering_rd = m_extraSputtering;
		m_extraSputtering = 0;

		m_extraParasiticItem_rd = m_extraParasiticItem;
		m_extraParasiticItem = 0;

		m_extraattackgainmp_rd = m_extraattackgainmp ;
		m_extraattackgainmp  = 0;
		m_extrauseitemreducemp_rd = m_extrauseitemreducemp;
		m_extrauseitemreducemp = 0;

		m_extradamagegainmp_rd = m_extradamagegainmp;
		m_extradamagegainmp = 0;

		m_extraRevertType_rd = m_extraRevertType;
		m_extraRevertType = 0;
	}
	else
	{
		crRoleData::excHandle(msg); 
	}
}
void crJXJRoleData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_Level:
		m_level = param?*((unsigned char*)param):NULL;
		break;
	case WCHDATA_JXJArmyCount:
		m_jxjArmyCount = param?*((/*unsigned short*/int*)param):NULL;
		break;
	case WCHDATA_JXJRTArmyCount:
		m_rtArmyCount = param?*((/*unsigned short*/int*)param):NULL;
		break;
	case WCHDATA_JXJExtraParasiticItem:
		m_extraParasiticItem = (crInstanceItem*)param;
		break;
	case WCHDATA_JXJAttackGainMP:
		m_attackgainmp = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJDamageGainMP:
		m_damagegainmp = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJAttackExtraGainMp	:
		m_extraattackgainmp = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJAttackExtraGainMp_RD:
		m_extraattackgainmp_rd = param ? *(short *)param : NULL;
		break;

	case WCHDATA_JXJExtraUseItemReduceMp:
		m_extrauseitemreducemp = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJExtraUseItemReduceMp_RD:
		m_extrauseitemreducemp_rd = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJExtraRevertType:
		m_extraRevertType = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJExtraRevertType_RD:
		m_extraRevertType_rd = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJOldRandomExtraId:
		m_oldExtraId = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJDamageExtraGainMp	:
		m_extradamagegainmp = param ? *(short *)param : NULL;
		break;
	case WCHDATA_JXJDamageExtraGainMp_RD:
		m_extradamagegainmp_rd = param ? *(short *)param : NULL;
		break;
	case WCHDATA_DataStream:
		//if(param)
		//{
		//	GNE::LockMutex lock( m_saveMutex );
		//	m_dataStream =(crStreamBuf*)param;
		//	char file[64];
		//	sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
		//	m_dataStream->saveToFile2(file);
		//	rcfg::ConfigScript cfg_script;
		//	cfg_script.Open(file,"");
		//	std::string str;
		//	std::vector<float> v_i;
		//	int nParam = 1;
		//	while (cfg_script.Get("Param", str,nParam++))
		//	{
		//		addParam(nParam-2,str);
		//	}
		//	cfg_script.Close();
		//	DeleteFileA(file);
		//	m_dataStream = NULL;
		//}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_jxjArmyCount = dataStream->_readInt();
			m_rtArmyCount = dataStream->_readInt();
			m_crit = dataStream->_readShort();
			m_parry = dataStream->_readShort();
			m_dodge = dataStream->_readShort();

			m_sightRange = dataStream->_readShort();
			m_guardRange = dataStream->_readShort();
			m_eyeHeight = dataStream->_readShort();
			m_hp = dataStream->_readInt();
			m_mp = dataStream->_readInt();
			m_level = dataStream->_readUChar();
			m_attack = dataStream->_readInt();
			m_defend = dataStream->_readInt();//2*7
			m_magicDefend = dataStream->_readShort();
			m_speed = dataStream->_readShort();
			m_stealth = dataStream->_readShort();
			m_stealthspy = dataStream->_readShort();
			m_hitratio = dataStream->_readShort();
			m_attackSpeed = dataStream->_readShort();
			m_skillSpeed = dataStream->_readShort();
			m_armourType = dataStream->_readUChar();

			unsigned char size;
			unsigned char i;
			int int1,int2;
			size = dataStream->_readUChar();
			m_attackMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_attackMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_skillMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_skillMap.insert(std::make_pair(int1,int2));
			//}
			size = dataStream->_readUChar();
			m_equipOnMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_equipOnMap.insert(std::make_pair(int1,int2));
			}
			m_camp = dataStream->_readUChar();
			//天赋
			m_inbornHPPercent = dataStream->_readShort();
			m_inbornMPPercent = dataStream->_readShort();
			m_inbornAttackPercent = dataStream->_readShort();
			m_inbornDefendPercent = dataStream->_readShort();
			m_inbornMagicDefendPercent = dataStream->_readShort();
			m_inbornBuildAttackPercent = dataStream->_readShort();
			m_inbornSpeed = dataStream->_readShort();
			m_inbornAttackSpeedPercent = dataStream->_readShort();
			m_inbornSkillSpeedPercent = dataStream->_readShort();
			m_inbornSightRange = dataStream->_readShort();
			m_inbornAttackDist = dataStream->_readShort();

			m_inbornStealth = dataStream->_readShort();
			m_inbornStealthSpy = dataStream->_readShort();
			m_inbornCrit = dataStream->_readShort();
			m_inbornParry = dataStream->_readShort();
			m_inbornDodge = dataStream->_readShort();
			m_inbornIgnoreDefend = dataStream->_readShort();
			m_inbornIgnoreMagicDefend = dataStream->_readShort();
			m_inbornIgnoreDodge = dataStream->_readUChar();
			m_inbornAttackDamageBack = dataStream->_readShort();
			m_inbornSkillDamageBack = dataStream->_readShort();
			m_inbornAttackAbsorbHP = dataStream->_readShort();
			m_inbornSkillAbsorbHP = dataStream->_readShort();
			m_inbornCritPower = dataStream->_readShort();
			m_inbornSputtering = dataStream->_readInt();
			m_inbornAttackDamage = dataStream->_readShort();
			m_inbornSkillDamage = dataStream->_readShort();
			m_inbornHitratioPercent = dataStream->_readShort();
			//m_dataStream = NULL;

			m_npcItemType = dataStream->_readUChar();

			m_attackgainmp = dataStream->_readShort();
			m_damagegainmp = dataStream->_readShort();
			m_extraattackgainmp = dataStream->_readShort();
			m_extraattackgainmp_rd = dataStream->_readShort();
			m_extrauseitemreducemp = dataStream->_readShort();
			m_extrauseitemreducemp_rd = dataStream->_readShort();
			m_extraRevertType = dataStream->_readUChar();
			m_extraRevertType_rd = dataStream->_readUChar();
			m_extradamagegainmp = dataStream->_readShort();
			m_extradamagegainmp_rd = dataStream->_readShort();
			m_chenghao = dataStream->_readUChar();
		}
		break;
	case WCHDATA_NetStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_jxjArmyCount = dataStream->_readInt();
			m_rtArmyCount = dataStream->_readInt();
			m_crit = dataStream->_readShort();
			m_parry = dataStream->_readShort();
			m_dodge = dataStream->_readShort();

			m_sightRange = dataStream->_readShort();
			//m_guardRange = dataStream->_readShort();
			m_eyeHeight = dataStream->_readShort();
			m_hp = dataStream->_readInt();
			m_mp = dataStream->_readInt();
			m_level = dataStream->_readUChar();
			m_attack = dataStream->_readInt();
			m_defend = dataStream->_readInt();//2*7
			m_magicDefend = dataStream->_readShort();
			m_speed = dataStream->_readShort();
			m_attackSpeed = dataStream->_readShort();
			m_skillSpeed = dataStream->_readShort();
			m_armourType = dataStream->_readUChar();

			unsigned char size;
			unsigned char i;
			int int1,int2;
			size = dataStream->_readUChar();
			m_attackMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_attackMap.insert(std::make_pair(int1,int2));
			}
			//size = dataStream->_readUChar();
			//m_skillMap.clear();
			//for(i = 0; i<size; i++)
			//{
			//	int1 = dataStream->_readInt();
			//	int2 = dataStream->_readInt();
			//	m_skillMap.insert(std::make_pair(int1,int2));
			//}
			size = dataStream->_readUChar();
			m_equipOnMap.clear();
			for(i = 0; i<size; i++)
			{
				int1 = dataStream->_readInt();
				int2 = dataStream->_readInt();
				m_equipOnMap.insert(std::make_pair(int1,int2));
			}
			m_camp = dataStream->_readUChar();
			//天赋
			m_inbornHPPercent = dataStream->_readShort();
			m_inbornMPPercent = dataStream->_readShort();
			m_inbornAttackPercent = dataStream->_readShort();
			m_inbornDefendPercent = dataStream->_readShort();
			m_inbornMagicDefendPercent = dataStream->_readShort();
			m_inbornBuildAttackPercent = dataStream->_readShort();
			m_inbornSpeed = dataStream->_readShort();
			m_inbornAttackSpeedPercent = dataStream->_readShort();
			m_inbornSkillSpeedPercent = dataStream->_readShort();
			m_inbornSightRange = dataStream->_readShort();
			m_inbornAttackDist = dataStream->_readShort();

			m_inbornStealth = dataStream->_readShort();
			m_inbornStealthSpy = dataStream->_readShort();
			m_inbornCrit = dataStream->_readShort();
			m_inbornParry = dataStream->_readShort();
			m_inbornDodge = dataStream->_readShort();
			m_inbornIgnoreDefend = dataStream->_readShort();
			m_inbornIgnoreMagicDefend = dataStream->_readShort();
			m_inbornIgnoreDodge = dataStream->_readUChar();
			m_inbornAttackDamageBack = dataStream->_readShort();
			m_inbornSkillDamageBack = dataStream->_readShort();
			m_inbornAttackAbsorbHP = dataStream->_readShort();
			m_inbornSkillAbsorbHP = dataStream->_readShort();
			m_inbornCritPower = dataStream->_readShort();
			m_inbornSputtering = dataStream->_readInt();
			m_inbornAttackDamage = dataStream->_readShort();
			m_inbornSkillDamage = dataStream->_readShort();
			m_inbornHitratioPercent = dataStream->_readShort();
			//m_dataStream = NULL;
			m_npcItemType = dataStream->_readUChar();
			m_chenghao = dataStream->_readUChar();
		}
		break;
	case WCHDATA_JXJTroopType:
		m_troopType = param?*(unsigned char*)param:JXJTroopsType_Bubing;
		break;
	case WCHDATA_JXJRoleChengHao:
		m_chenghao = param?*(unsigned char*)param:ChengHao_No;
		break;
	case WCHDATA_JXJNpcItemType:
		m_npcItemType = param?*(unsigned char*)param:NPC_Common;
		break;
	default:
		crRoleData::inputParam(i,param);
	}
}
void crJXJRoleData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_Level:
		param = &m_level;
		break;
	case WCHDATA_JXJArmyCount:
		param = &m_jxjArmyCount;
		break;
	case WCHDATA_JXJRTArmyCount:
		param = &m_rtArmyCount;
		break;
	case WCHDATA_JXJExtraParasiticItem:
		param = m_extraParasiticItem.get();
		break;
	case WCHDATA_JXJExtraParasiticItem_RD:
		param = m_extraParasiticItem_rd.get();
		break;
	case WCHDATA_JXJTroopType:
		param = &m_troopType;
		break;
	case WCHDATA_JXJRoleChengHao:
		param = &m_chenghao;
		break;
	case WCHDATA_JXJNpcItemType:
		param = &m_npcItemType;
		break;
	case WCHDATA_JXJAttackGainMP:
		param = &m_attackgainmp;
		break;
	case WCHDATA_JXJDamageGainMP:
		param = &m_damagegainmp;
		break;
	case WCHDATA_JXJAttackExtraGainMp	:
		param = & m_extraattackgainmp;
		break;
	case WCHDATA_JXJAttackExtraGainMp_RD:
		param = & m_extraattackgainmp_rd;
		break;
	case WCHDATA_JXJExtraUseItemReduceMp:
		param = &m_extrauseitemreducemp;
		break;
	case WCHDATA_JXJExtraUseItemReduceMp_RD:
		param = &m_extrauseitemreducemp_rd;
		break;
	case WCHDATA_JXJExtraRevertType:
		param = &m_extraRevertType;
		break;
	case WCHDATA_JXJExtraRevertType_RD:
		param = &m_extraRevertType_rd;
		break;
	case WCHDATA_JXJOldRandomExtraId:
		param = &m_oldExtraId;
		break;
	case WCHDATA_JXJDamageExtraGainMp	:
		param = & m_extradamagegainmp;
		break;
	case WCHDATA_JXJDamageExtraGainMp_RD:
		param = & m_extradamagegainmp_rd;
		break;
	default:
		crRoleData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJBuildingItemData
//
/////////////////////////////////////////
crJXJBuildingItemData::crJXJBuildingItemData():
	m_dataType(DT_MatrixNode),
	m_itemState(0),
	m_buildingid(0)
{
}
crJXJBuildingItemData::crJXJBuildingItemData(const crJXJBuildingItemData& data):
	crData(data),
	m_dataType(data.m_dataType),
	m_itemState(data.m_itemState),
	m_buildingid(data.m_buildingid)
{
}
void crJXJBuildingItemData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_DataType:
		m_dataType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJBuildingID:
		m_buildingid = atoi(relStr.c_str());
		break;
	}
}
void crJXJBuildingItemData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_ItemState:
		m_itemState = param==NULL?NULL:*((unsigned char*)param);
		break;
	}
}
void crJXJBuildingItemData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_DataType:
		param = &m_dataType;
		break;
	case WCHDATA_ItemState:
		param = &m_itemState;
		break;
	case WCHDATA_JXJBuildingID:
		param = &m_buildingid;
		break;
	default:
		crData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJItemData
//
/////////////////////////////////////////
crJXJItemData::crJXJItemData():
	m_dataType(DT_SceneItem),
	m_itemState(0),
	m_guiseState(GS_Static),
	m_patrolIndex(0),
	m_patrolLoopMode(PL_Swing),
	m_camp(0),
	m_sightRange(2500),
	m_eyeHeight(150),
	m_npcItemType(NPC_Common)
{
}
crJXJItemData::crJXJItemData(const crJXJItemData& data):
	crData(data),
	m_dataType(data.m_dataType),
	m_itemState(data.m_itemState),
	m_guiseState(data.m_guiseState),
	m_halfPhysicsBox(data.m_halfPhysicsBox),
	m_patrolPointVec(data.m_patrolPointVec),
	m_patrolIndex(data.m_patrolIndex),
	m_patrolLoopMode(data.m_patrolLoopMode),
	m_camp(data.m_camp),
	m_sightRange(data.m_sightRange),
	m_eyeHeight(data.m_eyeHeight),
	m_npcItemType(data.m_npcItemType)
{
}
void crJXJItemData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_DataType:
		m_dataType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_GuiseState:
		m_guiseState = (unsigned int)(atoi(relStr.c_str()));
		break;
	case WCHDATA_HalfPhysicsBox:
		crArgumentParser::appAtoVec(relStr,m_halfPhysicsBox);
		break;
	case WCHDATA_PatrolLoopMode:
		m_patrolLoopMode = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_Camp:
		m_camp = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_SightRange:
		{
			float range = atof(relStr.c_str());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_sightRange = (short)range;
		}
		break;
	case WCHDATA_EyeHeight:
		{
			float height = atof(relStr.c_str());
			height /= crGlobalHandle::gData()->gUnitScale();
			m_eyeHeight = (short)height;
		}
		break;
	case WCHDATA_JXJNpcItemType:
		m_npcItemType = (unsigned char)(atoi(relStr.c_str()));
		break;
	}
}
void crJXJItemData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_ItemState:
		m_itemState = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_GuiseState:
		m_guiseState = param==NULL?NULL:*((unsigned int*)param);
		break;
	case WCHDATA_HalfPhysicsBox:
		m_halfPhysicsBox = param?*(crVector3*)param:crVector3(0,0,0);
		break;
	case WCHDATA_PatrolPointVec:
		if(param)
		{
			crVector2 vec2 = *((crVector2*)param);
			m_patrolPointVec.push_back(vec2);
		}
		break;
	case WCHDATA_PatrolIndex:
		m_patrolIndex = param==NULL?NULL:*((char*)param);
		break;
	case WCHDATA_PatrolLoopMode:
		m_patrolLoopMode = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_Camp:
		m_camp = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_SightRange:
		m_sightRange = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_EyeHeight:
		m_eyeHeight = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_JXJNpcItemType:
		m_npcItemType = param == NULL ? NULL : *(unsigned char *)param;
		break;
	}
}
void crJXJItemData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_DataType:
		param = &m_dataType;
		break;
	case WCHDATA_ItemState:
		param = &m_itemState;
		break;
	case WCHDATA_GuiseState:
		param = &m_guiseState;
		break;
	case WCHDATA_HalfPhysicsBox:
		param = &m_halfPhysicsBox;
		break;
	case WCHDATA_PatrolPointVec:
		param = &m_patrolPointVec;
		break;
	case WCHDATA_PatrolIndex:
		param = &m_patrolIndex;
		break;
	case WCHDATA_PatrolLoopMode:
		param = &m_patrolLoopMode;
		break;
	case WCHDATA_Camp:
		param = &m_camp;
		break;
	case WCHDATA_SightRange:
		param = &m_sightRange;
		break;
	case WCHDATA_EyeHeight:
		param = &m_eyeHeight;
		break;
	case WCHDATA_JXJNpcItemType:
		param = &m_npcItemType;
		break;
	default:
		crData::getParam(i,param);
		break;
	}
}
void crJXJItemData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_InitData)
	{
		m_itemState = IS_Stop;
		if(m_guiseState & GS_StaticUnVisiable || m_guiseState & GS_Static || m_guiseState & GS_StaticNoneBlock)
			m_guiseState = GS_Static;
		else
			m_guiseState = GS_Normal;
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
/////////////////////////////////////////
//
//crJXJSkillData
//
/////////////////////////////////////////
crJXJSkillData::crJXJSkillData():
	m_dataType(DT_Equip),
	m_cdtime(0),
	m_skilltype(ST_Initiative),
	m_itemUseRange(10),
	m_itemUseScope(0),
	m_damage(0),
	m_percentDamage(0),
	m_damageType(0),
	m_needMP(0),
	m_needHP(0),
	m_targetExtraID(0),
	m_userExtraID(0),
	m_equipExtraID(0),
	m_targetType(Target_Bot),
	m_belongPlayerID(0),
	m_useItemProtectTime(0.0f),
	m_chanttime(0.0f),
	m_lastUseTime(0L),
	m_duration(1.0f),
	//m_equiphp(0),
	//m_equipmp(0),
	//m_equipAttack(0),
	//m_equipDefend(0),
	//m_equipMagicDefend(0),
	//m_equipSpeed(0),
	//m_equipStealth(0),
	//m_equipStealthSpy(0),
	//m_equipAttackSpeed(0),
	//m_equipSkillSpeed(0),
	//m_equipCrit(0),
	//m_equipParry(0),
	//m_equipDodge(0),
	//m_equipBuildAttack(0),
	//m_equipAttackDist(0),
	//m_equipHitratio(0),
	//m_equipIgnoreDefend(0),
	//m_equipIgnoreMagicDefend(0),
	//m_equipSightRange(0),
	//m_equipIgnoreDodge(0),
	//m_equipAttackDamageBack(0),
	//m_equipSkillDamageBack(0),
	//m_equipAttackAbsorbHP(0),
	//m_equipSkillAbsorbHP(0),
	//m_equipCritSword(0),
	//m_equipSputtering(0),
	//m_equipAttackDamage(0),
	//m_equipSkillDamage(0),
	m_damageCount(1),
	m_targetExtraMaxOverlapCount(0),
	m_userExtraMaxOverlapCount(0),
	m_useMPLimit (0)
{
}
crJXJSkillData::crJXJSkillData(const crJXJSkillData& data):
	crData(data),
	m_dataType(data.m_dataType),
	m_cdtime(data.m_cdtime),
	m_skilltype(data.m_skilltype),
	m_itemUseRange(data.m_itemUseRange),
	m_itemUseScope(data.m_itemUseScope),
	m_damage(data.m_damage),
	m_percentDamage(data.m_percentDamage),
	m_damageType(data.m_damageType),
	m_needMP(data.m_needMP),
	m_needHP(data.m_needHP),
	m_targetExtraID(data.m_targetExtraID),
	m_userExtraID(data.m_userExtraID),
	m_equipExtraID(data.m_equipExtraID),
	m_targetType(data.m_targetType),
	m_belongPlayerID(0),
	m_useItemProtectTime(data.m_useItemProtectTime),
	m_chanttime(data.m_chanttime),
	m_lastUseTime(0L),
	m_duration(data.m_duration),
	//m_equiphp(data.m_equiphp),
	//m_equipmp(data.m_equipmp),
	//m_equipAttack(data.m_equipAttack),
	//m_equipDefend(data.m_equipDefend),
	//m_equipMagicDefend(data.m_equipMagicDefend),
	//m_equipSpeed(data.m_equipSpeed),
	//m_equipStealth(data.m_equipStealth),
	//m_equipStealthSpy(data.m_equipStealthSpy),
	//m_equipAttackSpeed(data.m_equipAttackSpeed),
	//m_equipSkillSpeed(data.m_equipSkillSpeed),
	//m_equipCrit(data.m_equipCrit),
	//m_equipParry(data.m_equipParry),
	//m_equipDodge(data.m_equipDodge),
	//m_equipBuildAttack(data.m_equipBuildAttack),
	//m_equipAttackDist(data.m_equipAttackDist),
	//m_equipHitratio(data.m_equipHitratio),
	//m_equipIgnoreDefend(data.m_equipIgnoreDefend),
	//m_equipIgnoreMagicDefend(data.m_equipIgnoreMagicDefend),
	//m_equipSightRange(data.m_equipSightRange),
	//m_equipIgnoreDodge(data.m_equipIgnoreDodge),
	//m_equipAttackDamageBack(data.m_equipAttackDamageBack),
	//m_equipSkillDamageBack(data.m_equipSkillDamageBack),
	//m_equipAttackAbsorbHP(data.m_equipAttackAbsorbHP),
	//m_equipSkillAbsorbHP(data.m_equipSkillAbsorbHP),
	//m_equipCritSword(data.m_equipCritSword),
	//m_equipSputtering(data.m_equipSputtering),
	//m_equipAttackDamage(data.m_equipAttackDamage),
	//m_equipSkillDamage(data.m_equipSkillDamage),
	m_damageCount(data.m_damageCount),
	m_targetExtraMaxOverlapCount(data.m_targetExtraMaxOverlapCount),
	m_userExtraMaxOverlapCount(data.m_userExtraMaxOverlapCount),
	m_useMPLimit (data.m_useMPLimit)
{
}
void crJXJSkillData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_DataType:
		m_dataType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_CDTime:
		m_cdtime = atof(relStr.c_str());
		break;
	case WCHDATA_SkillType:
		m_skilltype = (char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ItemUseRange:
		{
			float range = atof(relStr.c_str());
			range = CRCore::maximum(range,crGlobalHandle::gData()->gMinItemUseRange());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_itemUseRange = (unsigned short)range;
		}
		//m_itemUseRange = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_ItemUseScope:
		{
			float range = atof(relStr.c_str());
			//range = CRCore::maximum(range,crGlobalHandle::gData()->gMinItemUseRange());
			range /= crGlobalHandle::gData()->gUnitScale();
			m_itemUseScope = (short)range;
		}
		break;
	case WCHDATA_Damage:
		m_damage = (int)(atoi(str.c_str()));
		break;
	case WCHDATA_JXJPercentDamage:
		m_percentDamage = (unsigned short)(atoi(str.c_str()));
		break;
	case WCHDATA_DamageType:
		m_damageType = (unsigned char)(atoi(str.c_str()));
		break;
	case WCHDATA_NeedMP:
		m_needMP = (unsigned short)(atoi(relStr.c_str()));
		break;
	case WCHDATA_NeedHP:
		m_needHP = (unsigned short)(atoi(relStr.c_str()));
		break;
	//case WCHDATA_TargetExtraID:
	//	m_targetExtraID = atoi(relStr.c_str());
	//	break;
	//case WCHDATA_UserExtraID:
	//	m_userExtraID = atoi(relStr.c_str());
	//	break;
	case WCHDATA_TargetType:
		m_targetType = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_UseItemProtectTime:
		m_useItemProtectTime = atof(relStr.c_str());
		break;
	case WCHDATA_ChantTime:
		m_chanttime = atof(relStr.c_str());
		break;
	case WCHDATA_Duration:
		m_duration = atof(relStr.c_str());
		break;
	case WCHDATA_DamageCount:
		m_damageCount = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJTargetOverlapMaxCount:
		m_targetExtraMaxOverlapCount = (_crInt8)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJUserOverlapMaxCount:
		m_userExtraMaxOverlapCount = (_crInt8)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJUseMpValLimit:
		m_useMPLimit = (unsigned short)(atoi(relStr.c_str()));
		break;
	}
}
void crJXJSkillData::excHandle(_crInt64 msg)
{
	crData::excHandle(getHandle(msg)); 
}
void crJXJSkillData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_DataType:
		m_dataType = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_CDTime:
		m_cdtime = param==NULL?0:*((float*)param);
		break;
	case WCHDATA_SkillType:
		m_skilltype = param==NULL?0:*((char*)param);
		break;
	case WCHDATA_ItemUseRange:
		m_itemUseRange = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_ItemUseScope:
		m_itemUseScope = param==NULL?NULL:*((short*)param);
		break;
	case WCHDATA_Damage:
		if(param) m_damage = *((int*)param);
		break;
	case WCHDATA_JXJPercentDamage:
		if(param) m_percentDamage = *((unsigned short*)param);
		break;
	case WCHDATA_DamageType:
		if(param) m_damageType = *((unsigned char*)param);
		break;
	case WCHDATA_NeedMP:
		m_needMP = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_NeedHP:
		m_needHP = param==NULL?NULL:*((unsigned short*)param);
		break;
	case WCHDATA_TargetType:
		m_targetType = param==NULL?NULL:*((unsigned char*)param);
		break;
	case WCHDATA_BelongPlayerID:
		m_belongPlayerID = param==NULL?NULL:*((int*)param);
		break;
	case WCHDATA_UseItemProtectTime:
		m_useItemProtectTime = param?*((float*)param):NULL;
		break;
	case WCHDATA_ChantTime:
		m_chanttime = param?*((float*)param):NULL;
		break;
	case WCHDATA_LastUseTime:
		m_lastUseTime = param?*(_crInt64 *)param:0L;
		break;
	case WCHDATA_Duration:
		m_duration = param?*(float *)param:0.0f;
		break;
	//case WCHDATA_EquipHP:
	//	m_equiphp = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipMP:
	//	m_equipmp = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipAttack:
	//	m_equipAttack = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipDefend:
	//	m_equipDefend = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipMagicDefend:
	//	m_equipMagicDefend = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSpeed:
	//	m_equipSpeed = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipStealth:
	//	m_equipStealth = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipStealthSpy:
	//	m_equipStealthSpy = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipAttackSpeed:
	//	m_equipAttackSpeed = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSkillSpeed:
	//	m_equipSkillSpeed = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipCrit:
	//	m_equipCrit = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipParry:
	//	m_equipParry = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipDodge:
	//	m_equipDodge = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipBuildAttack:
	//	m_equipBuildAttack = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipAttackDist:
	//	m_equipAttackDist = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipHitratio:
	//	m_equipHitratio = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipIgnoreDefend:
	//	m_equipIgnoreDefend = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipIgnoreMagicDefend:
	//	m_equipIgnoreMagicDefend = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSightRange:
	//	m_equipSightRange = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipIgnoreDodge:
	//	m_equipIgnoreDodge = param?*(unsigned char *)param:0;
	//	break;
	//case WCHDATA_EquipAttackDamageBack:
	//	m_equipAttackDamageBack = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSkillDamageBack:
	//	m_equipSkillDamageBack = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipAttackAbsorbHP:
	//	m_equipAttackAbsorbHP = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSkillAbsorbHP:
	//	m_equipSkillAbsorbHP = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipCritSword:
	//	m_equipCritSword = param?*(unsigned char *)param:0;
	//	break;
	//case WCHDATA_EquipSputtering:
	//	m_equipSputtering = param?*(_crInt32 *)param:0;
	//	break;
	//case WCHDATA_EquipAttackDamage:
	//	m_equipAttackDamage = param?*(short *)param:0;
	//	break;
	//case WCHDATA_EquipSkillDamage:
	//	m_equipSkillDamage = param?*(short *)param:0;
	//	break;
	case WCHDATA_TargetExtraID:
		m_targetExtraID = param?*(_crInt32 *)param:0;
		break;
	case WCHDATA_UserExtraID:
		m_userExtraID = param?*(_crInt32 *)param:0;
		break;
	case WCHDATA_EquipExtraID:
		m_equipExtraID = param?*(_crInt32 *)param:0;
		break;
	case WCHDATA_DamageCount:
		m_damageCount = param?*(unsigned char *)param:0;
		break;
	case WCHDATA_JXJTargetOverlapMaxCount:
		m_targetExtraMaxOverlapCount = param?*(_crInt8 *)param:0;
		break;
	case WCHDATA_JXJUserOverlapMaxCount:
		m_userExtraMaxOverlapCount = param?*(_crInt8 *)param:0;
		break;
	case WCHDATA_JXJUseMpValLimit:
		m_useMPLimit = param==NULL?NULL:*((unsigned short*)param);
		break;
	}
}

void crJXJSkillData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_DataType:
		param = &m_dataType;
		break;
	case WCHDATA_CDTime:
		param = &m_cdtime;
		break;
	case WCHDATA_SkillType:
		param = &m_skilltype;
		break;
	case WCHDATA_ItemUseRange:
		param = &m_itemUseRange;
		break;
	case WCHDATA_ItemUseScope:
		param = &m_itemUseScope;
		break;
	case WCHDATA_Damage:
		param = &m_damage;
		break;
	case WCHDATA_JXJPercentDamage:
		param = &m_percentDamage;
		break;
	case WCHDATA_DamageType:
		param = &m_damageType;
		break;
	case WCHDATA_NeedMP:
		param = &m_needMP;
		break;
	case WCHDATA_NeedHP:
		param = &m_needHP;
		break;
	case WCHDATA_TargetExtraID:
		param = &m_targetExtraID;
		break;
	case WCHDATA_JXJTargetOverlapMaxCount:
		param = &m_targetExtraMaxOverlapCount;
		break;
	case WCHDATA_JXJUserOverlapMaxCount:
		param = &m_userExtraMaxOverlapCount;
		break;
	case WCHDATA_UserExtraID:
		param = &m_userExtraID;
		break;
	case WCHDATA_EquipExtraID:
		param = &m_equipExtraID;
		break;
	case WCHDATA_TargetType:
		param = &m_targetType;
		break;
	case WCHDATA_BelongPlayerID:
		param = &m_belongPlayerID;
		break;
	case WCHDATA_UseItemProtectTime:
		param = &m_useItemProtectTime;
		break;
	case WCHDATA_ChantTime:
		param = &m_chanttime;
		break;
	case WCHDATA_LastUseTime:
		param = &m_lastUseTime;
		break;
	case WCHDATA_Duration:
		param = &m_duration;
		break;
	//case WCHDATA_EquipHP:
	//	param = &m_equiphp;
	//	break;
	//case WCHDATA_EquipMP:
	//	param = &m_equipmp;
	//	break;
	//case WCHDATA_EquipAttack:
	//	param = &m_equipAttack;
	//	break;
	//case WCHDATA_EquipDefend:
	//	param = &m_equipDefend;
	//	break;
	//case WCHDATA_EquipMagicDefend:
	//	param = &m_equipMagicDefend;
	//	break;
	//case WCHDATA_EquipSpeed:
	//	param = &m_equipSpeed;
	//	break;
	//case WCHDATA_EquipStealth:
	//	param = &m_equipStealth;
	//	break;
	//case WCHDATA_EquipStealthSpy:
	//	param = &m_equipStealthSpy;
	//	break;
	//case WCHDATA_EquipAttackSpeed:
	//	param = &m_equipAttackSpeed;
	//	break;
	//case WCHDATA_EquipSkillSpeed:
	//	param = &m_equipSkillSpeed;
	//	break;
	//case WCHDATA_EquipCrit:
	//	param = &m_equipCrit;
	//	break;
	//case WCHDATA_EquipParry:
	//	param = &m_equipParry;
	//	break;
	//case WCHDATA_EquipDodge:
	//	param = &m_equipDodge;
	//	break;
	//case WCHDATA_EquipBuildAttack:
	//	param = &m_equipBuildAttack;
	//	break;
	//case WCHDATA_EquipAttackDist:
	//	param = &m_equipAttackDist;
	//	break;
	//case WCHDATA_EquipHitratio:
	//	param = &m_equipHitratio;
	//	break;
	//case WCHDATA_EquipIgnoreDefend:
	//	param = &m_equipIgnoreDefend;
	//	break;
	//case WCHDATA_EquipIgnoreMagicDefend:
	//	param = &m_equipIgnoreMagicDefend;
	//	break;
	//case WCHDATA_EquipSightRange:
	//	param = &m_equipSightRange;
	//	break;
	//case WCHDATA_EquipIgnoreDodge:
	//	param = &m_equipIgnoreDodge;
	//	break;
	//case WCHDATA_EquipAttackDamageBack:
	//	param = &m_equipAttackDamageBack;
	//	break;
	//case WCHDATA_EquipSkillDamageBack:
	//	param = &m_equipSkillDamageBack;
	//	break;
	//case WCHDATA_EquipAttackAbsorbHP:
	//	param = &m_equipAttackAbsorbHP;
	//	break;
	//case WCHDATA_EquipSkillAbsorbHP:
	//	param = &m_equipSkillAbsorbHP;
	//	break;
	//case WCHDATA_EquipCritSword:
	//	param = &m_equipCritSword;
	//	break;
	//case WCHDATA_EquipSputtering:
	//	param = &m_equipSputtering;
	//	break;
	//case WCHDATA_EquipAttackDamage:
	//	param = &m_equipAttackDamage;
	//	break;
	//case WCHDATA_EquipSkillDamage:
	//	param = &m_equipSkillDamage;
	//	break;
	case WCHDATA_DamageCount:
		param = &m_damageCount;
		break;
	case WCHDATA_JXJUseMpValLimit:
		param = &m_useMPLimit;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crJXJGameServerData
//
/////////////////////////////////////////
crJXJGameServerData::crJXJGameServerData():
	m_openBattle(0),
	m_battleTimer(0),
	m_bIsOpenRebel(false),
	m_nContinuedRebelTimer(0),
	m_perRebelTimer(0),
	m_checkplayerdataplayerid(0),
	m_cfgstream(NULL),
	m_isCycling(false),
	isChengChiShiliSyn(false),
	m_cycleTimer(0),
	m_groupDayUpdate(0L),
	m_groupWeekUpdate(0L),
	m_shiliDayUpdate(0L),
	m_systemnoticeid(0),
	m_consortiaPromotion(0L),
	m_battleRankingVerWei(0),
	m_battleRankingVerShu(0),
	m_battleRankingVerWu(0),
	m_battleRankingSaveTime(0)
{
}
crJXJGameServerData::crJXJGameServerData(const crJXJGameServerData& data):
	crData(data),
	m_openBattle(0),
	m_battleTimer(0),
	m_bIsOpenRebel(false),
	m_nContinuedRebelTimer(0),
	m_perRebelTimer(0),
	m_checkplayerdataplayerid(0),
	m_cfgstream(NULL),
	m_isCycling(false),
	isChengChiShiliSyn(false),
	m_cycleTimer(0),
	m_groupDayUpdate(0L),
	m_groupWeekUpdate(0L),
	m_shiliDayUpdate (0L),
	m_systemnoticeid(0),
	m_consortiaPromotion(0L),
	m_battleRankingVerWei(0),
	m_battleRankingVerShu(0),
	m_battleRankingVerWu(0),
	m_battleRankingSaveTime(0)
{
	for (int i = 0; i<6; i++)
	{
		m_fubenrushmap[i].reserve(10);
	}
	unsigned short countermaxtime = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, 0).c_str());
	for (int i = 0; i<countermaxtime + 1; i++)
	{
		m_jxjcounterMap[i].reserve(10);
	}
	
}
void crJXJGameServerData::addParam(int i, const std::string& str)
{
	//std::string type,relStr;
	//std::string::size_type comma = str.find_first_of(',');
	//type = std::string(str.begin(),str.begin()+comma);
	//relStr = std::string(str.begin()+comma + 1,str.end());
	//int _type = atoi(type.c_str());
	//switch(_type) 
	//{
	////case WCHDATA_JXJExpRankingMap1:
	////case WCHDATA_JXJExpRankingMap2:
	////case WCHDATA_JXJExpRankingMap3:
	////case WCHDATA_JXJExpRankingMap4:
	////case WCHDATA_JXJExpRankingMap5:
	////case WCHDATA_JXJExpRankingMap6:
	//case WCHDATA_JXJExpRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_shiliExpRanking.insert(std::make_pair(rank->getExploit(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJLevelRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_lvRanking.insert(std::make_pair(rank->getLevel(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJGovpostRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_govpostRanking.insert(std::make_pair(rank->getGovpost(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJAccomplishRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_accomplishRanking.insert(std::make_pair(rank->getAccomplishPoint(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJTroopsRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_troopsRanking.insert(std::make_pair(rank->getTroops(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJCountrycontributeRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_countrycontributeRankingMap.insert(std::make_pair(rank->getCountrycontribute(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJManorlvRankingMap:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<crPlayerRanking> rank;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				rank = new crPlayerRanking;
	//				col = 0;
	//				rank->setPlayerID(atoi((*table)(i,col++).c_str()));
	//				rank->setExploit(atoi((*table)(i,col++).c_str()));
	//				rank->setName((*table)(i,col++));
	//				rank->setGovpost(atoi((*table)(i,col++).c_str()));
	//				rank->setShiliID(atoi((*table)(i,col++).c_str()));
	//				rank->setLevel(atoi((*table)(i,col++).c_str()));
	//				rank->setNobility(atoi((*table)(i,col++).c_str()));
	//				rank->setAccomplishPoint(atoi((*table)(i,col++).c_str()));
	//				rank->setTroops(atoi((*table)(i,col++).c_str()));
	//				rank->setManorlv(atoi((*table)(i,col++).c_str()));
	//				rank->setCountrycontribute(atoi((*table)(i,col++).c_str()));
	//				//m_shiliExpRanking[_type - (WCHDATA_JXJExpRankingMap1)].insert(std::make_pair(rank->getExploit(),rank));
	//				m_manorlvRankingMap.insert(std::make_pair(rank->getManorlv(),rank));
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	//case WCHDATA_JXJConsortiaLvRank:
	//	{
	//		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	//		if(table.valid())
	//		{
	//			ref_ptr<ConsortiaLvRankItem> rankItem;
	//			int rowcount = table->getRowCount();
	//			int col = 0;
	//			for(int i = 0; i<rowcount; ++i)
	//			{
	//				col = 0;
	//				rankItem = new ConsortiaLvRankItem(atoi((*table)(i,col++).c_str()));
	//				rankItem->m_groupName = (*table)(i,col++);
	//				rankItem->m_leaderName = (*table)(i,col++);
	//				rankItem->m_lv = atoi((*table)(i,col++).c_str());
	//				rankItem->m_memberCount = atoi((*table)(i,col++).c_str());
	//				rankItem->m_troops = atoi((*table)(i,col++).c_str());
	//				rankItem->m_exploit = atoi((*table)(i,col++).c_str());
	//				rankItem->m_shili = atoi((*table)(i,col++).c_str());
	//				m_consortiaLvRank.updateRank(rankItem->getKey(),rankItem);
	//			}
	//		}
	//		else
	//		{
	//			makeDirectoryForFile(relStr);
	//		}
	//	}
	//	break;
	////case WCHDATA_JXJChengChiMap:
	////	{
	////		ref_ptr<crTableIO> table = crTableIO::openFile(relStr);
	////		if(table.valid())
	////		{
	////			int rowcount = table->getRowCount();
	////			int col = 0;
	////			crVector4i neighbours;
	////			unsigned short id;
	////			unsigned char shili;
	////			unsigned short defend;
	////			for(int i = 0; i<rowcount; ++i)
	////			{
	////				col = 0;
	////				id = (unsigned short)(atoi((*table)(i,col++).c_str()));
	////				shili = (unsigned char)(atoi((*table)(i,col++).c_str()));
	////				defend = (unsigned short)(atoi((*table)(i,col++).c_str()));
	////				m_chengchiMap[id] = std::make_pair(shili,defend);
	////			}
	////		}
	////	}
	////	break;
	//}
}
void crJXJGameServerData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJOpenBattle:
		m_openBattle = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJBattleTimer:
		m_battleTimer = param?*((int*)param):NULL;
		break;
	//case WCHDATA_JXJOpenPass:
	//	m_openPass = param?*((bool*)param):NULL;
	//	break;
	//case WCHDATA_JXJPassTimer:
	//	m_passTimer = param?*((short*)param):NULL;
	//	break;
	case WCHDATA_JXJChengchiModifySet:
		if(param)
			m_chengchiModifySet.insert(*((unsigned short*)param));
		break;
	case WCHDATA_JXJShiliWei:
		m_shiliWei = param?(crJXJShili*)param:NULL;
		break;
	case WCHDATA_JXJShiliShu:
		m_shiliShu = param?(crJXJShili*)param:NULL;
		break;
	case WCHDATA_JXJShiliWu:
		m_shiliWu = param?(crJXJShili*)param:NULL;
		break;
	case WCHDATA_JXJOpenRebel:
		m_bIsOpenRebel = param?*(bool *)param:NULL;
		break;
	case WCHDATA_JXJContinuedRebelTimer:
		m_nContinuedRebelTimer = param?*(int *)param:NULL;
		break;
	case WCHDATA_JXJPerRebelTimer:
		m_perRebelTimer = param?*(unsigned short *)param:NULL;
		break;
	case WCHDATA_JXJCheckPlayerDataPlayerID:
		m_checkplayerdataplayerid = param?*(int *)param:NULL;
		break;
	case WCHDATA_JXJCenterNotice:
		m_cfgstream = param?(crStreamBuf*)param:NULL;
		break;
	case WCHDATA_JXJIsServerCycling:
		m_isCycling = param?*(bool *)param:false;
		break;
	case WCHDATA_JXJServerCycleTimer:
		m_cycleTimer = param?*(float *)param:0;
		break;
	case WCHDATA_JXJIsChengChiShiliSyn			:
		isChengChiShiliSyn = *(bool *)param;
		break;
	case WCHDATA_JXJGroupDayLastUpdate				:
		m_groupDayUpdate = param ? *(time_t *) param : NULL;
		break;
	case WCHDATA_JXJGroupWeekLastUpdate				:
		m_groupWeekUpdate = param ? *(time_t *)param : NULL;
		break;
	case WCHDATA_JXJShiliDayLastUpdate			:
		m_shiliDayUpdate = param ? *(time_t *)param : NULL;
		break;
	case WCHDATA_JXJSystemNoticeID:
		m_systemnoticeid = param ? *(int *)param : 0;
		break;
	case WCHDATA_JXJConsortiaPromotionLastUpdateTime:
		m_consortiaPromotion = param ? *(time_t *)param :NULL;
		break;
	case WCHDATA_JXJBattleRankingVerWei:
		m_battleRankingVerWei = param ? *(char *)param :NULL;
		break;
	case WCHDATA_JXJBattleRankingVerShu:
		m_battleRankingVerShu = param ? *(char *)param :NULL;
		break;
	case WCHDATA_JXJBattleRankingVerWu:
		m_battleRankingVerWu = param ? *(char *)param :NULL;
		break;
	case WCHDATA_JXJBattleRankingSaveTime:
		m_battleRankingSaveTime = param ? *(time_t *)param :NULL;
		break;
	default:
		break;
	}
}
void crJXJGameServerData::getParam(int i, void*& param)
{
	switch(i) 
	{
	//case WCHDATA_JXJExpRankingWaitMap:
	//	param = &m_expRankingWaitMap;
	//	break;
	case WCHDATA_JXJExpRankingMap:
		param = &m_shiliExpRanking;
		break;
	//case WCHDATA_JXJLevelRankingWaitMap:
	//	param = &m_lvRankingWaitMap;
	//	break;
	case WCHDATA_JXJLevelRankingMap:
		param = &m_lvRanking;
		break;
	//case WCHDATA_JXJGovpostRankingWaitMap:
	//	param = &m_govpostRankingWaitMap;
	//	break;
	case WCHDATA_JXJGovpostRankingMap:
		param = &m_govpostRanking;
		break;
	//case WCHDATA_JXJAccomplishRankingWaitMap:
	//	param = &m_accomplishRankingWaitMap;
	//	break;
	case WCHDATA_JXJAccomplishRankingMap:
		param = &m_accomplishRanking;
		break;
	//case WCHDATA_JXJTroopsRankingWaitMap:
	//	param = &m_troopsRankingWaitMap;
	//	break;
	case WCHDATA_JXJTroopsRankingMap:
		param = &m_troopsRanking;
		break;
	//case WCHDATA_JXJCountrycontributeRankingWaitMap:
	//	param = &m_countrycontributeRankingWaitMap;
	//	break;
	case WCHDATA_JXJCountrycontributeRankingMap:
		param = &m_countrycontributeRankingMap;
		break;
	//case WCHDATA_JXJManorlvRankingWaitMap:
	//	param = &m_manorlvRankingWaitMap;
	//	break;
	case WCHDATA_JXJManorlvRankingMap:
		param = &m_manorlvRankingMap;
		break;
	case WCHDATA_JXJConsortialvRankingMap:
		param = &m_consortialvRankingMap;
		break;
	case WCHDATA_JXJConsortiaMemberRankingMap:
		param = &m_consortiaMemberCountRank;
		break;
	//case WCHDATA_JXJExpRankingMap1:
	//case WCHDATA_JXJExpRankingMap2:
	//case WCHDATA_JXJExpRankingMap3:
	//	param = &(m_shiliExpRanking[i-(WCHDATA_JXJExpRankingMap1)]);
	//	break;
	case WCHDATA_JXJSaleDisposeDeq:
		param = &m_saleDisposeDeq;
		break;
	case WCHDATA_JXJQuerySaleDeque:
		param = &m_querySaleDeq;
		break;
	case WCHDATA_JXJBuySaleDeque:
		param = &m_buySaleDeq;
		break;
	case WCHDATA_JXJOpenBattle:
		param = &m_openBattle;
		break;
	case WCHDATA_JXJBattleTimer:
		param = &m_battleTimer;
		break;
	//case WCHDATA_JXJOpenPass:
	//	param = &m_openPass;
	//	break;
	//case WCHDATA_JXJPassTimer:
	//	param = &m_passTimer;
	//	break;
	case WCHDATA_JXJChengChiMap:
		param = &m_chengchiMap;
		break;
	case WCHDATA_JXJDefendDequeMap:
		param = &m_defendDequeMap;
		break;
	case WCHDATA_JXJAttackDequeMap:
		param = &m_attackDequeMap;
		break;
	case WCHDATA_JXJOpenBattleMap:
		param = &m_openBattleMap;
		break;
	//case WCHDATA_JXJCloseBattleSet:
	//	param = &m_closeBattleSet;
	//	break;
	case WCHDATA_JXJBattleScoreMap:
		param = &m_battleScoreMap;
		break;
	case WCHDATA_JXJCreateBattleDeque:
		param = &m_createBattleDeque;
		break;
	case WCHDATA_JXJBattleDropPlayerMap:
		param = &m_battleDropPlayerMap;
		break;
	case WCHDATA_JXJMailTaskList:
		param = &m_mail_sending;
		break;
	case WCHDATA_JXJMailQueryList:
		param = &m_mail_query;
		break;
	case WCHDATA_JXJMailBodyQueryList:
		param = &m_mailbody_query;
		break;
	case WCHDATA_JXJMailDeleteList:
		param = &m_maildeletedeq;
		break;
	case WCHDATA_JXJGiftsPackKeySet:
		param = &m_giftsPackKeySet;
		break;
	case WCHDATA_JXJNewMailQueryList:
		param = &m_newmailquerylist;
		break;
	case WCHDATA_JXJCreateGiftsPackDeque:
		param = &m_createGiftsPackDeque;
		break;
	case WCHDATA_JXJQueryGiftsPackDeque:
		param = &m_queryGiftsPacketDeque;
		break;
	case WCHDATA_JXJOnBattleUIPlayerSet:
		param = &m_onBattleUIPlayerSet;
		break;
	case WCHDATA_JXJOnChengfangUIPlayerMap:
		param = &m_onChengfangUIPlayerMap;
		break;
	case WCHDATA_JXJChengchiModifySet:
		param = &m_chengchiModifySet;
		break;
	case WCHDATA_JXJConsortiaMap:
		param = &m_consortiaMap;
		break;
	case WCHDATA_JXJFubenRushMap:
		param = &m_fubenrushmap;
		break;
	case WCHDATA_JXJCreateGroupDisposeDeq:
		param = &m_creategroupdeq;
		break;
	case WCHDATA_JXJGroupAddMemberDisposeDeq:
		param = &m_groupAddMemberDeq;
		break;
	case WCHDATA_JXJQueryGroupDisposeDeq:
		param = &m_querygroupdeq;
		break;
	case WCHDATA_JXJQueryGroupMemberDisposeDeq:
		param = &m_querygroupmemberdeq;
		break;
	case WCHDATA_JXJShiliWei:
		param = m_shiliWei.get();
		break;
	case WCHDATA_JXJShiliShu:
		param = m_shiliShu.get();
		break;
	case WCHDATA_JXJShiliWu:
		param = m_shiliWu.get();
		break;
	case WCHDATA_JXJOnCountryQueryMap:
		param = &m_oncountryquerymap;
		break;
	case WCHDATA_JXJSystermMailInfoDeq:
		param = &m_sysMailDeq;
		break;
	//case WCHDATA_JXJImmediatelySavePlayerSet:
	//	param = &m_immediatelySavePlayerSet;
	//	break;
	case WCHDATA_JXJGSTeamMap:
		param = &m_gsteamMap;
		break;
	case WCHDATA_JXJRebelCityMap:
		param = &m_rebelCityMap;
		break;
	case WCHDATA_JXJOpenRebel:
		param = &m_bIsOpenRebel;
		break;
	case WCHDATA_JXJContinuedRebelTimer:
		param = &m_nContinuedRebelTimer;
		break;
	case WCHDATA_JXJCounterinsurgencyInfoMap:
		//param  = &m_counterMap;
		param  = &m_jxjcounterMap;
		break;
	case WCHDATA_JXJPerRebelTimer:
		param = &m_perRebelTimer;
		break;
	case WCHDATA_JXJBroadcastMessageDeq:
		param = &m_broadcastMessageDeq;
		break;
	case WCHDATA_JXJGroupMessageDeq:
		param = &m_groupMessageDeq;
		break;
	case WCHDATA_JXJTeamMessageDeq:
		param = &m_teamMessageDeq;
		break;
	case WCHDATA_JXJPlayerMessageDeq:
		param = &m_playerMessageDeq;
		break;
	case WCHDATA_JXJSysBroadcastMessageDeq:
		param = &m_sysbroadcastMessageDeq;
		break;
	case WCHDATA_JXJCheckPlayerDataMap:
		param = &m_checkplayerdatamap;
		break;
	case WCHDATA_JXJCheckPlayerDataPlayerID:
		param = &m_checkplayerdataplayerid;
		break;
	case WCHDATA_JXJCenterNotice:
		param = &m_cfgstream;
		break;
	//case WCHDATA_JXJGameServerTimer:
	//	param = &m_serverTimer;
	//	break;
	case WCHDATA_JXJSystemMailDeq:
		param = &m_systemMailDeq;
		break;
	case WCHDATA_JXJIsServerCycling:
		param = &m_isCycling;
		break;
	case WCHDATA_JXJServerCycleTimer:
		param = &m_cycleTimer;
		break;
	case WCHDATA_JXJDismissGroupMap:					
		param = &m_dismissGroupMap;
		break;
	case WCHDATA_JXJConsortiaLvRank:
		param = &m_consortiaLvRank;
		break;
	case WCHDATA_JXJCreatePlayerStoreInfoDeque:
		param = &m_playerStoreInfos;
		break;
	case WCHDATA_JXJCreateGameStoreDistillDeque:
		param = &m_playerStoreRecordInfos;
		break;
	case WCHDATA_JXJCountryUIQueryPeopleInfoDeque:
		param = & m_queryPeopleInfoMap;
		break;
	case WCHDATA_JXJGroupChatVec:						
		param = & m_groupChatVec;
		break;
	case WCHDATA_JXJIsChengChiShiliSyn			:
		param = & isChengChiShiliSyn;
		break;
	case WCHDATA_JXJGroupDayLastUpdate				:
		param = & m_groupDayUpdate ;
		break;
	case WCHDATA_JXJGroupWeekLastUpdate				:
		param = & m_groupWeekUpdate;
		break;
	case WCHDATA_JXJShiliDayLastUpdate			:
		param = & m_shiliDayUpdate;
		break;
	case WCHDATA_JXJInsertBattleReportDeq:
		param = &m_insertBattleReportDeq;
		break;
	case WCHDATA_JXJQueryBattleReportDeq:
		param = &m_queryBattleReportDeq;
		break;
	case WCHDATA_JXJNoChatSet:
		param = &m_noChatSet;
		break;
	case WCHDATA_JXJSystemNoticeID:
		param = &m_systemnoticeid;
		break;
	case WCHDATA_JXJOnlineEveryDayPlayeridSet:
		param = &m_onlinePlayerSet;
		break;
	case WCHDATA_JXJConsortiaExploitRankingMap:
		param = &m_consortiaexploitRank;
		break;
	case WCHDATA_JXJConsortiaTroopsRankingMap:
		param = &m_consortiatroopsRank;
		break;
	case WCHDATA_JXJConsortiaPromotionLastUpdateTime:
		param = &m_consortiaPromotion;
		break;
	case WCHDATA_JXJBattleRankingMapWei:
		param = &m_battleRankingMapWei;
		break;
	case WCHDATA_JXJBattleRankingMapShu:
		param = &m_battleRankingMapShu;
		break;
	case WCHDATA_JXJBattleRankingMapWu:
		param = &m_battleRankingMapWu;
		break;
	case WCHDATA_JXJBattleRankingVerWei:
		param = &m_battleRankingVerWei;
		break;
	case WCHDATA_JXJBattleRankingVerShu:
		param = &m_battleRankingVerShu;
		break;
	case WCHDATA_JXJBattleRankingVerWu:
		param = &m_battleRankingVerWu;
		break;
	case WCHDATA_JXJBattleRankingSaveTime:
		param = &m_battleRankingSaveTime;
		break;
	default:
		crData::getParam(i,param);
		break;
	}
}
void crJXJGameServerData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else if(LOINT64(msg) == WCH_BuildPlayerLoginStream)
	{
	
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		unsigned char shili = stream->_readUChar();
		if (m_cfgstream.valid())
		{
			stream->createBuf(16+m_chengchiMap.size() * 5 + m_cfgstream->getBufSize());
		}
		else
		{
			stream->createBuf(16+m_chengchiMap.size() * 14);
		}
		stream->_writeDouble(crGlobalHandle::getInstance()->timer());//8
		stream->_writeBool(m_openBattle);					//1
		stream->_writeShort(m_chengchiMap.size());			//2
		for( ChengchiMap::iterator itr = m_chengchiMap.begin();
			 itr != m_chengchiMap.end();
			 ++itr )
		{
			stream->_writeUShort(itr->first);				//2
			itr->second->buildStream(stream);
			//stream->_writeUChar(itr->second.first);		//1
			//stream->_writeUShort(itr->second.second);		//2
		}

		//stream->_writeBool(m_bIsOpenRebel);
// 		stream->_writeChar(m_rebelCityMap[shili].size());	//1
// 		std::set<unsigned short>::iterator itr_rebel = m_rebelCityMap[shili].begin();
// 		for (;itr_rebel != m_rebelCityMap[shili].end();itr_rebel++)
// 		{
// 			stream->_writeUShort(*itr_rebel);				//2
// 		}
		//stream->_writeUShort(m_perRebelTimer);				//2
		//服务器轮回信息
		stream->_writeBool(m_isCycling);
		stream->_writeFloat(m_cycleTimer);
		//开服时间
		stream->_writeString(crGlobalHandle::gData()->gStartDate());
		if (m_cfgstream.valid())
		{
			stream->write(m_cfgstream->getPtr(),m_cfgstream->getRemainSize());
		}
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
/////////////////////////////////////////
//
//crJXJSceneServerData
//
/////////////////////////////////////////
crJXJSceneServerData::crJXJSceneServerData()
{
}
crJXJSceneServerData::crJXJSceneServerData(const crJXJSceneServerData& data):
	crSceneServerData(data)
{
}
void crJXJSceneServerData::excHandle(_crInt64 msg)
{
	crSceneServerData::excHandle(msg);
}
void crJXJSceneServerData::inputParam(int i, void *param)
{
	crSceneServerData::inputParam(i,param);
}
void crJXJSceneServerData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJNoChatSet:
		param = &m_noChatSet;
		break;
	default:
		crSceneServerData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJQueryRankingUIData
//
/////////////////////////////////////////
crJXJQueryRankingUIData::crJXJQueryRankingUIData()
{
}
crJXJQueryRankingUIData::crJXJQueryRankingUIData(const crJXJQueryRankingUIData& data):
	crUIData(data)
{
}
void crJXJQueryRankingUIData::inputParam(int i, void *param)
{
	crUIData::inputParam(i,param);
}
void crJXJQueryRankingUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGovpostRankingMap:
		param = &m_govpostRankingMap;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
void crJXJQueryRankingUIData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crUIData::excHandle(getHandle(msg)); 
	}
}
/////////////////////////////////////////
//
//crJXJGoodsRadioUIData
//
/////////////////////////////////////////
crJXJGoodsRadioUIData::crJXJGoodsRadioUIData()
{
}
crJXJGoodsRadioUIData::crJXJGoodsRadioUIData(const crJXJGoodsRadioUIData& data):
	crUIData(data),
	m_goodsVec(data.m_goodsVec)
{
}
void crJXJGoodsRadioUIData::addParam(int i, const std::string& str)
{
	crVector2i vec;
	crArgumentParser::appAtoVec(str,vec);
	m_goodsVec.push_back(vec);
}
void crJXJGoodsRadioUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGoodsVec:
		param = &m_goodsVec;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJMarketUIData
//
/////////////////////////////////////////
crJXJMarketUIData::crJXJMarketUIData()
{
}
crJXJMarketUIData::crJXJMarketUIData(const crJXJMarketUIData& data):
	crUIData(data),
	m_goldbuySteps(data.m_goldbuySteps),
	m_coppersaleSteps(data.m_coppersaleSteps)
{
}
void crJXJMarketUIData::addParam(int i, const std::string& str)
{
}
void crJXJMarketUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGoldBuySteps:
		param = &m_goldbuySteps;
		break;
	case WCHDATA_JXJCopperSaleSteps:
		param = &m_coppersaleSteps;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

/////////////////////////////////////////
//
//crJXJBattleCameraData
//
/////////////////////////////////////////
crJXJBattleCameraData::crJXJBattleCameraData():	
	m_cameraOperator(Cam_Default),
	m_timer(-1),
	m_othertimer(-1),
	m_isShow(false),
	m_range(0),
	m_isFollow(false),
	m_ea(NULL),
	m_campAddArmyTimer(0),
	m_cameraSpeed(40.0f),
	m_battleCameraMovable(true), m_effectTargetType(0),
	m_delayTimer(-1)
{
}
crJXJBattleCameraData::crJXJBattleCameraData(const crJXJBattleCameraData& data):
	crCameraData(data),
	m_cameraOperator(Cam_Default),
	m_timer(-1),
	m_othertimer(-1),
	m_isShow(false),
	m_range(data.m_range),
	m_useItemPair(data.m_useItemPair),
	m_isFollow(data.m_isFollow),
	m_ea(NULL),
	m_campAddArmyTimer(data.m_campAddArmyTimer),
	m_cameraSpeed(40.0f),
	m_battleCameraMovable(true),
	m_namedSignalMap(data.m_namedSignalMap),
	m_effectTargetType(data.m_effectTargetType),
	m_delayTimer(-1)
{
}
void crJXJBattleCameraData::addParam(int i, const std::string& str)
{
}
void crJXJBattleCameraData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJUseSkillDecal:
		m_useSkillDecal = param?(crNode*)param:NULL;
		break;
	case WCHDATA_JXJUseSkillRangeNodeMap:
		if(param)
			m_useSkillRangeNodeMap = *(SkillRangeNodeMap *)param;
		break;
	case WCHDATA_JXJCameraOperator:
		m_cameraOperator = param?*(unsigned short*)param:NULL;
		break;
	case WCHDATA_JXJSignalsMap:
		if(param)
			m_signalDeque = *(SignalNodeMap*)param;
		break;
	case WCHDATA_JXJNamedSignalNodeMap:
		if (param)
			m_namedSignalMap = *(NamedSignalNodeMap *)param;
		break;
	case WCHDATA_JXJBattleChoosePointTimer:
		if(param)
			m_timer = *(short*)param;
		break;
	case WCHDATA_JXJTimer:
		if(param)
			m_othertimer = *(short*)param;
		break;
	case WCHDATA_JXJBattleMouseOnRoleVec:
		if(param)
			m_mouseonitemVec =*(ItemVec*)param;
		break;
	case WCHDATA_JXJIsSelectRoleInfoTipsShow:
		if(param)
			m_isShow = *(bool *)param;
		break;
	case WCHDATA_JXJPreUseSkillRangeDecal:
		m_usePreSkillRangeDecal = param?(crNode*)param:NULL;
		break;
	case WCHDATA_JXJPreUseSkillRange:
		if(param)
			m_range = *(unsigned short*)param;
		break;
	case WCHDATA_JXJPreUseSkillRole:
		m_role = param?(crRole *)param:NULL;
		break;
	case WCHDATA_JXJRoleAboutToUseItemID:
		if(param)
			m_useItemPair = *(AboutUseItemPair *)param;
		else
		{
			m_useItemPair.first = NULL;
			m_useItemPair.second = 0;
		}
		break;
	case WCHDATA_JXJPreUseSkillDecal:
		m_usePreSkillDecal = param?(crNode*)param:NULL;
		break;
	case WCHDATA_JXJPreUseSkillScope:
		m_scope = param?*(short *)param:NULL;
		break;
	case WCHDATA_JXJPreUseSkillEffectTargetType:
		m_effectTargetType = param ? *(unsigned char *)param : NULL;
		break;
	case WCHDATA_JXJCameraFollow:
		m_isFollow = param?*(bool *)param:true;
		break;
	case WCHDATA_JXJBattleTargetRoleVec:
		if(param)
			m_targetItemVec = *(TargetItemVec *)param;
		else
		{
			m_targetItemVec.clear();
		}
		break;
	case WCHDATA_JXJKeyDownEventInfo:
		m_ea = param?(CRProducer::crGUIEventAdapter *)param:NULL;
		break;
	case WCHDATA_JXJBirthPointButtonVec:
		if(param)
			m_btVec = *(ButtonVector *)param;
		else
		{
			m_btVec.clear();
		}
		break;
	case WCHDATA_JXJMouseOnShuiJingNode:
		m_msOnShuiJingNode = param?(crNode*)param:NULL;
		break;
	case WCHDATA_JXJMouseOnJianTaNode:
		m_msOnJianTaNode = param?(crNode*)param:NULL;
		break;
	case WCHDATA_JXJCampAddArmyTimer:
		m_campAddArmyTimer = param?*(float*)param:NULL;
		break;
	case WCHDATA_JXJCameraMoveSpeed:
		m_cameraSpeed = param?*(float *)param:40.0f;
		break;
	case WCHDATA_JXJBattleCameraMovable:
		m_battleCameraMovable = param?*(bool *)param:true;
		break;
	case WCHDATA_JXJDelayCanvasCloseTime:
		if(param)
			m_delayTimer = *(short*)param;
		break;
	default:
		crCameraData::inputParam(i,param);
		break;
	}
}
void crJXJBattleCameraData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJUseSkillDecal:
		param = m_useSkillDecal.get();
		break;
	case WCHDATA_JXJUseSkillRangeNodeMap:
		param = &m_useSkillRangeNodeMap;
		break;
	case WCHDATA_JXJBattleFormation:
		param = &m_battleFormationMap;
		break;
	case WCHDATA_JXJCameraOperator:
		param = &m_cameraOperator;
		break;
	case WCHDATA_JXJSignalsMap:
		param = &m_signalDeque;
		break;
	case WCHDATA_JXJNamedSignalNodeMap:
		param = &m_namedSignalMap;
		break;
	case WCHDATA_JXJBattleChoosePointTimer:
		param = &m_timer;
		break;
	case WCHDATA_JXJTimer:
		param = &m_othertimer;
		break;
	case WCHDATA_JXJBattleMouseOnRoleVec:
		param = &m_mouseonitemVec;
		break;
	case WCHDATA_JXJIsSelectRoleInfoTipsShow:
		param = &m_isShow;
		break;
	case WCHDATA_JXJPreUseSkillRangeDecal:
		param = m_usePreSkillRangeDecal.get();
		break;
	case WCHDATA_JXJPreUseSkillRange:
		param = &m_range;
		break;
	case WCHDATA_JXJPreUseSkillRole:
		param = m_role.get();
		break;
	case WCHDATA_JXJRoleAboutToUseItemID:
		param = &m_useItemPair;
		break;
	case WCHDATA_JXJPreUseSkillDecal:
		param = m_usePreSkillDecal.get();
		break;
	case WCHDATA_JXJPreUseSkillScope:
		param = &m_scope;
		break;
	case WCHDATA_JXJPreUseSkillEffectTargetType:
		param = &m_effectTargetType;
		break;
	case WCHDATA_JXJRecvDamageMap:
		param = &m_widgetNodeMap;
		break;
	//case WCHDATA_JXJRecvDamageShowDeque:
	//	param = &m_widgetNodeDeque;
	//	break;
	case WCHDATA_JXJCameraFollow:
		param = &m_isFollow;
		break;
	case WCHDATA_JXJBattleTargetRoleVec:
		param = &m_targetItemVec;
		break;
	case WCHDATA_JXJKeyDownEventInfo:
		param = m_ea.get();
		break;
	case WCHDATA_JXJBirthPointButtonVec:
		param = &m_btVec;
		break;
	case WCHDATA_JXJMouseOnShuiJingNode:
		param = m_msOnShuiJingNode.get();
		break;
	case WCHDATA_JXJMouseOnJianTaNode:
		param = m_msOnJianTaNode.get();
		break;
	case WCHDATA_JXJAttackIconWidgetMap:
		param = &m_sceneNodeMap;
		break;
	case WCHDATA_JXJCampAddArmyTimer:
		param = &m_campAddArmyTimer;
		break;
	case WCHDATA_JXJCameraMoveSpeed:
		param = &m_cameraSpeed;
		break;
	case WCHDATA_JXJBattleCameraMovable:
		param = &m_battleCameraMovable;
		break;
	case WCHDATA_JXJDelayCanvasCloseTime:
		param = &m_delayTimer;
		break;
	default:
		crCameraData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJEnterBattleUIData
//
/////////////////////////////////////////
crJXJEnterBattleUIData::crJXJEnterBattleUIData():
	m_sceneid(0),
	m_timer(30)
/*	m_index(0)*/
{
}
crJXJEnterBattleUIData::crJXJEnterBattleUIData(const crJXJEnterBattleUIData& data):
	crUIData(data),
	m_sceneid(data.m_sceneid),
	m_timer(data.m_timer)
/*	m_index(data.m_index)*/
{
}
void crJXJEnterBattleUIData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJEnterBattleTimer:
		m_timer = atoi(relStr.c_str());
		break;
	}
}
void crJXJEnterBattleUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJEnterSceneID:
		m_sceneid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJEnterBattleTimer:
		m_timer = param?*(int*)param:NULL;
		break;
// 	case WCHDATA_JXJCanvasIndex:
// 		m_index = param?*(unsigned char *)param:NULL;
// 		break;
	default:
		crUIData::inputParam(i,param);
	}
}
void crJXJEnterBattleUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJEnterSceneID:
		param = &m_sceneid;
		break;
	case WCHDATA_JXJEnterBattleTimer:
		param = &m_timer;
		break;
// 	case WCHDATA_JXJCanvasIndex:
// 		param = &m_index;
// 		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}

/////////////////////////////////////////
//
//crJXJBattleRoomData
//
/////////////////////////////////////////
crJXJBattleRoomData::crJXJBattleRoomData():
	m_battleid(0),
	m_roomType(1),
	m_getFirstBloodPlayerId(0),
	m_temhuodong(4),
	m_weipoints(0),
	m_shupoints(0),
	m_wupoints(0)
	//m_red_remain(0), 
	//m_blue_remain(0),
	//m_modified(false),
	//m_is_init(false)
{
}
crJXJBattleRoomData::crJXJBattleRoomData(const crJXJBattleRoomData& data):
	crRoomData(data),
	m_battleid(data.m_battleid),
	m_roomType(data.m_roomType),
	//m_checks_tabname(data.m_checks_tabname),
	//m_red_remain(data.m_red_remain),
	//m_blue_remain(data.m_blue_remain),
	//m_checks(data.m_checks),
	//m_modified(data.m_modified),
	//m_is_init(data.m_is_init),
	m_reliveItemMap(data.m_reliveItemMap),
	m_saveProgressSet(data.m_saveProgressSet),
	m_getFirstBloodPlayerId(data.m_getFirstBloodPlayerId),
	m_totalNpcCount(data.m_totalNpcCount),
	m_scenarioVariableMap(data.m_scenarioVariableMap),
	m_battleRbuffId(data.m_battleRbuffId),
	m_playerCoordMap(data.m_playerCoordMap),
	m_praisedid(data.m_praisedid),
	m_temhuodong(data.m_temhuodong),
	m_weipoints(data.m_weipoints),
	m_shupoints(data.m_shupoints),
	m_wupoints(data.m_wupoints)
{
}
void crJXJBattleRoomData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	//case WCHDATA_JXJStartPoint:
	//	m_red_remain = m_blue_remain = atoi(relStr.c_str());
	//	break;
	//case WCHDATA_JXJChecksTab:
	//	m_checks_tabname = relStr;
	//	break;
	case WCHDATA_JXJReliveItemMap:
		{
			CRCore::ref_ptr<CREncapsulation::crTableIO> table = crTableIO::openFile(crGlobalHandle::getInstance()->getScenarioTableDir() + relStr);
			m_reliveItemMap.clear();
			if(table.valid())
			{
				int rowcount = table->getRowCount();
				int col = 0;
				short progress;
				ref_ptr<crJXJReliveItem> reliveItem;
				int colcount = table->getColumnCount();
				for(int i = 0; i<rowcount; ++i)
				{
					col = 0;
					reliveItem = new crJXJReliveItem;
					progress = (short)(atoi((*table)(i,col++).c_str()));
					reliveItem->setInstanceID(atoi((*table)(i,col++).c_str()));
					reliveItem->setName((*table)(i,col++));
					reliveItem->setCount((unsigned short)(atoi((*table)(i,col++).c_str())));
					reliveItem->setHPScale(atof((*table)(i,col++).c_str()));
					reliveItem->setAttackScale(atof((*table)(i,col++).c_str()));
					if(col<colcount)
						reliveItem->setCamp(atoi((*table)(i,col++).c_str()));
					else
						reliveItem->setCamp(1);

					if(col<colcount)
						reliveItem->setNpcItemType((unsigned char)atoi((*table)(i,col++).c_str()));
					else
						reliveItem->setNpcItemType(NPC_Common);

					m_reliveItemMap.insert(std::make_pair(progress,reliveItem.get()));
				}
			}
		}
		break;
	case WCHDATA_JXJSaveProgressSet:
		{
			m_saveProgressSet.clear();
			std::vector<short> shortarray;
			crArgumentParser::appAtoVec(relStr,shortarray);
			for( std::vector<short>::iterator itr = shortarray.begin();
				 itr != shortarray.end();
				 ++itr )
			{
				m_saveProgressSet.insert(*itr);
			}
			//CRCore::ref_ptr<CREncapsulation::crTableIO> table = crTableIO::openFile(crGlobalHandle::getInstance()->getScenarioTableDir() + relStr);
			//m_saveProgressSet.clear();
			//if(table.valid())
			//{
			//	int rowcount = table->getRowCount();
			//	short progress;
			//	for(int i = 0; i<rowcount; ++i)
			//	{
			//		progress = (short)(atoi((*table)(i,0).c_str()));
			//		m_saveProgressSet.insert(progress);
			//	}
			//}
		}
		break;
	case WCHDATA_JXJRoomType:
		m_roomType = atoi(relStr.c_str());
		break;
	default:
		crRoomData::addParam(i,str);
		break;
	}
}
void crJXJBattleRoomData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJBattleID:
		m_battleid = param?*(int*)param:NULL;
		break;
	//case WCHDATA_JXJPassRedRemain:
	//	m_red_remain = param ? *(int*)param : NULL;
	//	break;
	//case WCHDATA_JXJPassBlueRemain:
	//	m_blue_remain = param ? *(int*)param : NULL;
	//	break;
	//case WCHDATA_JXJRoomDataFlag:
	//	m_modified = param ? *(char *)param : NULL;
	//	break;
	//case WCHDATA_JXJPassCaptureCheck:
	//	m_isPassCapture = param? *(bool *)param :NULL;
	//	break;
	default:
		crRoomData::inputParam(i,param);
		break;
	}
}
void crJXJBattleRoomData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJBattleID:
		param = &m_battleid;
		break;
	case WCHDATA_JXJBattleStatsMap:
		param = &m_battleStatsMap;
		break;
	//case WCHDATA_JXJPassRedRemain:
	//	param = &m_red_remain;
	//	break;
	//case WCHDATA_JXJPassBlueRemain:
	//	param = &m_blue_remain;
	//	break;
	//case WCHDATA_JXJCheckPointVec:
	//	if(!m_is_init && !m_checks_tabname.empty())
	//	{
	//		// 读取table 数据
	//		CRCore::ref_ptr<CREncapsulation::crTableIO> tab_checkpoint = crTableIO::openFile(crGlobalHandle::getInstance()->getScenarioTableDir() + m_checks_tabname);
	//		if(tab_checkpoint.valid())
	//		{
	//			// 形成监测点数据
	//			CREncapsulation::crTableIO::StrVec record;
	//			int id_index = tab_checkpoint->getTitleIndex("城池ID");
	//			m_checks.clear();
	//			unsigned short city_id = (unsigned short)(HIINT32(m_battleid));
	//			if(city_id != 0)
	//			{
	//				tab_checkpoint->queryOneRecord(id_index, crArgumentParser::appItoa(city_id), record);

	//				for(int i = 1; i < record.size(); i++)
	//				{
	//					if(record[i].size() <= 5) continue;
	//					crVector3 temp_vec;
	//					crArgumentParser::appAtoVec(record[i], temp_vec);
	//					ref_ptr<PassCheck> temp = new PassCheck;
	//					temp->setPosX(temp_vec[0]) ;
	//					temp->setPosY(temp_vec[1]) ;
	//					temp->setArea(temp_vec[2]) ;
	//					temp->setScore(0);
	//					if(i==(record.size()-1))
	//						temp->setType(1);
	//					else
	//						temp->setType(0);
	//					m_checks.push_back(temp);
	//				}
	//			}
	//		}
	//		m_is_init = true;
	//	}
	//	param = &m_checks;
	//	break;
	//case WCHDATA_JXJRoomDataFlag:
	//	param = &m_modified;
	//	break;
	case WCHDATA_JXJReliveItemMap:
		param = &m_reliveItemMap;
		break;
	case WCHDATA_JXJSaveProgressSet:
		param = &m_saveProgressSet;
		break;
	//case WCHDATA_JXJPassCaptureCheck:
	//	param = &m_isPassCapture;
	//	break;
	case WCHDATA_JXJRoomType:
		param = &m_roomType;
		break;
	case WCHDATA_JXJCampBirthPointMap:
		param = &m_roomCampMap;
		break;
	case WCHDATA_JXJRoomFistBloodPlayer:
		param = &m_getFirstBloodPlayerId;
		break;
	case WCHDATA_JXJCampAddArmyTimerMap:
		param = &m_campTimerMap;
		break;
	case WCHDATA_JXJScenarioTimerMap:
		param = &m_scenarioTimerMap;
		break;
	case WCHDATA_JXJFubenStarsMap:
		param = &m_fubenStarMap;
		break;
	case WCHDATA_JXJBattleTotalNpcCount:
		param = &m_totalNpcCount;
		break;
	case WCHDATA_JXJScenarioVariableMap:
		param = &m_scenarioVariableMap;
		break;
	case WCHDATA_JXJBattleRBuffId:
		param = &m_battleRbuffId;
		break;
	case WCHDATA_JXJPlayerCoordMap:
		param = &m_playerCoordMap;
		break;
	case WCHDATA_JXJWeiGuoPoints:
		param = &m_weipoints;
		break;
	case WCHDATA_JXJShuGuoPoints:
		param = &m_shupoints;
		break;
	case WCHDATA_JXJWuGuoPoints:
		param = &m_wupoints;
		break;
	case WCHDATA_JXJWorldPraisedIdMap:
		param = &m_praisedid;
		break;
	case WCHDATA_JXJWorldTemHuoDong:
		param = &m_temhuodong;
		break;
	default:
		crRoomData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crPlayerRewardData
//
/////////////////////////////////////////
void crPlayerRewardData::buildGiftStream(CRCore::ref_ptr<crStreamBuf> &stream)
{
	ItemMap giftItemMap;
	rcfg::ConfigScript gift_script(&giftItemMap);
	if(m_coopercash>0)
	{
		gift_script.Add("T_Copper",m_coopercash);
	}
	if(m_exploit>0)
	{
		gift_script.Add("T_Exploit",m_exploit);
	}
	if(m_huoyuedu>0)
	{
		gift_script.Add("T_HuoYueDu",m_huoyuedu);
	}
	if(m_warcounts==true)
	{
		gift_script.Add("T_GuoZhanShuLiang",m_warcounts);
	}
	if(m_score>0)
	{
		gift_script.Add("T_GuoZhanJiFen",m_score);
	}
	if(m_exprience>0)
	{
		gift_script.Add("T_Exp",m_exprience);
	}
	if(m_achievement>0)
	{
		gift_script.Add("T_Achievement",m_achievement);
	}
	if(m_food>0)
	{
		gift_script.Add("T_Food",m_food);
	}
	if(m_wood>0)
	{
		gift_script.Add("T_Wood",m_wood);
	}
	if(m_iron>0)
	{
		gift_script.Add("T_Iron",m_iron);
	}
	if(m_horse>0)
	{
		gift_script.Add("T_Horse",m_horse);
	}
	if(m_giftGold>0)
	{
		gift_script.Add("T_Giftgold",m_giftGold);
	}
	if(m_countrycontribution>0)
	{
		gift_script.Add("T_Contribute",m_countrycontribution);
	}
	std::vector<std::string> v_str;
	v_str.resize(2);
	gift_script.Add("RewardEquipsVec");
	gift_script.Push("RewardEquipsVec");
	for( RewardEquipsVec::iterator itr = m_equips.begin();
		itr != m_equips.end();
		++itr )
	{
		v_str[0] = crArgumentParser::appItoa(itr->first);
		v_str[1] = crArgumentParser::appItoa(itr->second);
		gift_script.Add("Param",v_str);
	}
	gift_script.Pop();
	v_str.resize(3);
	gift_script.Add("RewardItemVec");
	gift_script.Push("RewardItemVec");
	CRCore::ref_ptr<crBagItemData> bagitem;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if (itemtab.valid())
	{
		crTableIO::StrVec record;
		for( RewardItemVec::iterator itr = m_rewardItems.begin();
			itr != m_rewardItems.end();
			++itr )
		{
			bagitem = itr->get();
			v_str[0] = crArgumentParser::appItoa(bagitem->getItemID());
			if(itemtab->queryOneRecord(0,v_str[0],record)>=0)
			{
				v_str[1] = crArgumentParser::appItoa(bagitem->getEquipMagic());
				v_str[2] = crArgumentParser::appItoa(bagitem->getItemCount());
				gift_script.Add("Param",v_str);
			}
		}
	}
	gift_script.Pop();
	crGlobalHandle::getInstance()->getScriptStream(gift_script,stream);
}
void crPlayerRewardData::setGiftDataStream(crStreamBuf *stream)
{
	int huoyudu=0;
	int warcount=0;
	rcfg::ConfigScript gift_script;
	crGlobalHandle::getInstance()->getStreamScript(stream,gift_script);
	gift_script.Get("T_Copper",m_coopercash);
	gift_script.Get("T_Exploit",m_exploit);
	gift_script.Get("T_HuoYueDu",huoyudu);
	gift_script.Get("T_GuoZhanShuLiang",warcount);
	gift_script.Get("T_GuoZhanJiFen",m_score);
	gift_script.Get("T_Exp",m_exprience);
	gift_script.Get("T_Achievement",m_achievement);
	gift_script.Get("T_Food",m_food);
	gift_script.Get("T_Wood",m_wood);
	gift_script.Get("T_Iron",m_iron);
	gift_script.Get("T_Horse",m_horse);
	gift_script.Get("T_Giftgold",m_giftGold);
	gift_script.Get("T_Contribute",m_countrycontribution);
	std::vector<std::string> v_str;
	int nParam = 1;
	if(gift_script.Push("RewardEquipsVec"))
	{
		m_equips.clear();
		while (gift_script.Get("Param", v_str,nParam++))
		{
			m_equips.push_back(std::make_pair(atoi(v_str[0].c_str()),atoi(v_str[1].c_str())));
		}
		gift_script.Pop();
	}
	CRCore::ref_ptr<crBagItemData> bagitem;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if (itemtab.valid())
	{
		crTableIO::StrVec record;
		if(gift_script.Push("RewardItemVec"))
		{
			m_rewardItems.clear();
			nParam = 1;
			while (gift_script.Get("Param", v_str,nParam++))
			{
				if(itemtab->queryOneRecord(0,v_str[0],record)>=0)
				{
					bagitem = new crBagItemData;
					bagitem->setItemID(atoi(v_str[0].c_str()));
					bagitem->setEquipMagic(atoi(v_str[1].c_str()));
					bagitem->setItemCount(atoi(v_str[2].c_str()));
					m_rewardItems.push_back(bagitem);
				}
			}
			gift_script.Pop();
		}
	}
	m_huoyuedu = huoyudu;
	m_warcounts = warcount;
}
void crPlayerRewardData::parseRewardItemCount(int &cardcount,int &itemcount)
{
	cardcount = 0;
	itemcount = 0;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if (itemtab.valid())
	{
		int typeindex = itemtab->getTitleIndex("类型");
		crTableIO::StrVec record;
		for( RewardItemVec::iterator itr = m_rewardItems.begin();
			itr != m_rewardItems.end();
			++itr )
		{
			if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itr)->getItemID()),record)>=0)
			{
				if (atoi(record[typeindex].c_str()) == IT_Jiangka)
				{
					cardcount++;
				}
				else
				{
					itemcount++;
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIGiftsPackMailHyperData
//
/////////////////////////////////////////
crJXJUIGiftsPackMailHyperData::crJXJUIGiftsPackMailHyperData()
{
	m_packItemVec.clear();
}
crJXJUIGiftsPackMailHyperData::crJXJUIGiftsPackMailHyperData(const crJXJUIGiftsPackMailHyperData& data):
	crUIData(data),
	m_keyWords(data.m_keyWords),
	m_packItemVec(data.m_packItemVec),
	m_reportId(0)
{
}
void crJXJUIGiftsPackMailHyperData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJGiftsPackKey:
		if(relStr!=m_keyWords) m_packItemVec.clear();
		m_keyWords = relStr;
		break;
	case WCHDATA_JXJGiftsPackItemInfo:
		{
			crVector3i vec3;
			crArgumentParser::appAtoVec(relStr,vec3);
			//m_packItemVec.push_back(std::make_pair(packitem[0],packitem[1]));
			ref_ptr<crBagItemData> itemdata = new crBagItemData;
			itemdata->setItemID(vec3[0]);
			itemdata->setEquipMagic(vec3[1]);
			itemdata->setItemCount(vec3[2]);
			m_packItemVec.push_back(itemdata);
		}
		break;
	case WCHDATA_JXJBattleReportID:
		m_reportId = atoi(relStr.c_str());
		break;
	}
}
void crJXJUIGiftsPackMailHyperData::inputParam(int i, void *param)
{
}
void crJXJUIGiftsPackMailHyperData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJGiftsPackKey:
		param = &m_keyWords;
		break;
	case WCHDATA_JXJGiftsPackItemInfo:
		param = &m_packItemVec;
		break;
	case WCHDATA_JXJBattleReportID:
		param = &m_reportId;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJNewHandUIData
//
/////////////////////////////////////////
crJXJNewHandUIData::crJXJNewHandUIData():
	m_newHandTaskid(0)
{
}
crJXJNewHandUIData::crJXJNewHandUIData(const crJXJNewHandUIData& data):
	crUIData(data),
	m_newHandTaskid(data.m_newHandTaskid),
	m_targetCanvasName(data.m_targetCanvasName),
	m_targetWidgetName(data.m_targetWidgetName)
{
}
void crJXJNewHandUIData::addParam(int i, const std::string& str)
{
}
void crJXJNewHandUIData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJNewHandTaskID:
		m_newHandTaskid = param?*(int*)param:NULL;
		break;
	case WCHDATA_JXJTargetCanvasName:
		m_targetCanvasName = param?*(std::string*)param:NULL;
		break;
	case WCHDATA_JXJTargetWidgetName:
		m_targetWidgetName = param?*(std::string*)param:NULL;
		break;
	}
}
void crJXJNewHandUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJNewHandTaskID:
		param = &m_newHandTaskid;
		break;
	case WCHDATA_JXJTargetCanvasName:
		param = &m_targetCanvasName;
		break;
	case WCHDATA_JXJTargetWidgetName:
		param = &m_targetWidgetName;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJConsortiaData
//
/////////////////////////////////////////
GNE::Mutex crJXJConsortiaData::m_saveMutex;
crJXJConsortiaData::crJXJConsortiaData():
	m_lv(1),
	m_manorlevel(1),
	m_barrackslevel(1),
	m_shiliid(0),
	m_funds(0),
	m_dismissTime(-1),
	m_modify(false),
	m_earningsScaleT(0),
	m_earningsScaleY(0),
	m_totalActiveValY(0),
	m_dayLastUpdate(0),
	m_yesterdayIsCantonGroup(false),
	m_weekLastUpdate(0),
	m_conquest(0)
{

	m_groupLeaderVec.clear();
	/*m_earningsVec.clear();*/
	m_fenFengedChengChiSet.clear();
	m_consortiaMemberMap.clear();
	m_applyvec.clear();
	m_fenFengedCantonMapY.clear();
	m_fenFengedCantonMapT.clear();
	m_applyCantonMapY.clear();
	m_applyCantonMapT.clear();
	m_activeRank.clear();
	m_activeRank.resize(10);
	/*m_startedFuBenSet.clear();*/
	m_startedFuBenMap.clear();
	m_grouptroopsMap.clear();
	m_fenFengedChengChiMap.clear();
	m_consortiarecordsDeq.clear();
}

crJXJConsortiaData::crJXJConsortiaData(const crJXJConsortiaData& data):
	crData(data),
	m_lv(data.m_lv),
	m_manorlevel(data.m_manorlevel),
	m_barrackslevel(data.m_barrackslevel),
	m_shiliid(data.m_shiliid),
	m_modify(data.m_modify),
	m_funds(data.m_funds),
	m_dismissTime(data.m_dismissTime),
	m_yesterdayIsCantonGroup(data.m_yesterdayIsCantonGroup),
	m_earningsScaleY(data.m_earningsScaleY),
	m_earningsScaleT(data.m_earningsScaleT),
	m_totalActiveValY(data.m_totalActiveValY),
	m_dayLastUpdate(data.m_dayLastUpdate),
	m_weekLastUpdate(data.m_weekLastUpdate),
	m_conquest(data.m_conquest)
{
	m_groupLeaderVec.clear();
	/*m_earningsVec.clear();*/
	m_fenFengedChengChiSet.clear();
	m_consortiaMemberMap.clear();
	m_applyvec.clear();
	m_fenFengedCantonMapY.clear();
	m_fenFengedCantonMapT.clear();
	m_applyCantonMapY.clear();
	m_applyCantonMapT.clear();
	m_activeRank.clear();
	m_activeRank.resize(10);
	/*m_startedFuBenSet.clear();*/
	m_startedFuBenMap.clear();
	m_grouptroopsMap.clear();
	m_fenFengedChengChiMap.clear();
	m_consortiarecordsDeq.clear();
}

void crJXJConsortiaData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	//std::cout << _type << "  :  " << relStr << std::endl; 
	switch(_type) 
	{
	case WCHDATA_JXJConsortialv:
		m_lv = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJManorlevel:
		m_manorlevel = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJBarrackslevel:
		m_barrackslevel = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJConsortiaShiliID:
		m_shiliid = (unsigned char)(atoi(relStr.c_str()));
		break;
	case WCHDATA_JXJConsortiaFunds:
		m_funds = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJConsortiaDismissTime:
		m_dismissTime = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJConsortiaRecruitNotice:
		m_recruitNotice = relStr;
		break;
	case WCHDATA_JXJConsortiaContactNotice:
		m_contactNotice = relStr;
		break;
	case WCHDATA_JXJConsortiaNormalNotice:		
		m_notice = relStr;
		break;
	case WCHDATA_JXJConsortiaName:
		m_groupName = relStr;
		break;
	case WCHDATA_JXJConsortiaFengYi:
		m_fengyi = relStr;
		break;
	case WCHDATA_JXJGroupEarningsScaleY:
		m_earningsScaleY = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGroupEarningsScaleT:
		m_earningsScaleT = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGroupTotalActiveValY:
		m_totalActiveValY = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGroupDayLastUpdate				:
		m_dayLastUpdate = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJGroupWeekLastUpdate				:
		m_weekLastUpdate = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJConquest:
		m_conquest = (unsigned char)(atoi(relStr.c_str()));
		break;
	}
}

void crJXJConsortiaData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)HIINT64(msg);
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[512];
		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortialv,m_lv);
		cfg_script.Add("Param",buf);
		
		sprintf(buf,"%d,%d\0",WCHDATA_JXJManorlevel,(int)m_manorlevel);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJBarrackslevel,(int)m_barrackslevel);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortiaShiliID,(int)m_shiliid);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortiaFunds,(int)m_funds);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJConsortiaDismissTime,(int)m_dismissTime);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%s\0",WCHDATA_JXJConsortiaRecruitNotice	,m_recruitNotice.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJConsortiaContactNotice	,m_contactNotice.c_str());
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%s\0",WCHDATA_JXJConsortiaName,m_groupName.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJConsortiaFengYi,m_fengyi.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJConsortiaNormalNotice,m_notice.c_str());
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJGroupEarningsScaleY,m_earningsScaleY);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJGroupEarningsScaleY,m_earningsScaleT);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJGroupTotalActiveValY,m_totalActiveValY);
		cfg_script.Add("Param",buf);

		cfg_script.Add("VERSION",JXJVERSION);

		sprintf(buf,"%d,%s\0",WCHDATA_JXJGroupDayLastUpdate	,crArgumentParser::appI64toa(m_dayLastUpdate).c_str());
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%s\0",WCHDATA_JXJGroupWeekLastUpdate,crArgumentParser::appI64toa(m_weekLastUpdate).c_str());
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJConquest,(int)m_conquest);
		cfg_script.Add("Param",buf);


		/////////////
		if(!m_consortiaMemberMap.empty())
		{
			cfg_script.Add("ConsortiaMemberMap");
			cfg_script.Push("ConsortiaMemberMap");
			for( ConsortiaMemberMap::iterator cmItr = m_consortiaMemberMap.begin();
				 cmItr != m_consortiaMemberMap.end();
				 ++cmItr )
			{
				cfg_script.Add("Param",cmItr->first);
				cfg_script.Add("Param",cmItr->second->getName());
				cfg_script.Add("Param",(int)cmItr->second->getLevel());
				cfg_script.Add("Param",(int)cmItr->second->getPos());

				cfg_script.Add("Param",(int)cmItr->second->getGroupPos());
				cfg_script.Add("Param",(int)cmItr->second->getActiveVaule());
				cfg_script.Add("Param",crArgumentParser::appI64toa(cmItr->second->getTime()));
				cfg_script.Add("Param",(int)cmItr->second->getLastWeekActive());
				cfg_script.Add("Param",(int)cmItr->second->getTodayWeekActive());
				cfg_script.Add("Param",(int)cmItr->second->getEarningsYesterday());

				//
				cfg_script.Add("Param",(int)cmItr->second->getYesterdayActive());
				cfg_script.Add("Param",(int)cmItr->second->getNewer());
				cfg_script.Add("Param",crArgumentParser::appI64toa(cmItr->second->getLoginTime()));
				cfg_script.Add("Param",(int)cmItr->second->getGroupContribute());
				cfg_script.Add("Param",(int)cmItr->second->getTroopStoredNumT());
				cfg_script.Add("Param",(int)cmItr->second->getTroopTakedNumT());
			}
			cfg_script.Pop();
		}

		if(!m_activeRank.empty())
		{
			cfg_script.Add("ConsortiaActiveRank");
			cfg_script.Push("ConsortiaActiveRank");
			ConsortiaActiveRank::RankSet & rankSet = m_activeRank.getRankSet();
			ConsortiaActiveRank::RankSet::iterator csItr = rankSet.begin();
			for(; csItr != rankSet.end(); ++csItr )
			{
				if (csItr->valid())
				{
					cfg_script.Add("Param",(int)((*csItr)->m_id));
					cfg_script.Add("Param",(int)( (*csItr)->m_groupPos ));
					cfg_script.Add("Param",(int)((*csItr)->m_activeVal) );
					cfg_script.Add("Param",(*csItr)->m_name);
				}
			}
			cfg_script.Pop();
		}
		///////////////////
		if(!m_applyvec.empty())
		{
			cfg_script.Add("ConsortiaApplyVec");
			cfg_script.Push("ConsortiaApplyVec");
			for( ConsortiaApplyVec::iterator csItr = m_applyvec.begin();
				csItr != m_applyvec.end();
				++csItr )
			{
				cfg_script.Add("Param",(*csItr)->getPlayerID());
				cfg_script.Add("Param",(*csItr)->getName());
				cfg_script.Add("Param",(int)(*csItr)->getLevel());
				cfg_script.Add("Param",(int)(*csItr)->getPos());
				cfg_script.Add("Param",crArgumentParser::appI64toa((*csItr)->getTime()));
			}
			cfg_script.Pop();
		}
		/*if (!m_startedFuBenSet.empty())
		{
			cfg_script.Add("StartedFubenSet");
			cfg_script.Push("StartedFubenSet");
			StartedFubenSet::iterator sfsItr = m_startedFuBenSet.begin();
			for( ; sfsItr != m_startedFuBenSet.end(); ++ sfsItr )
			{
				cfg_script.Add("Param",*sfsItr );
			}
			cfg_script.Pop();
		}*/
		if (!m_fenFengedCantonMapT.empty())
		{
			cfg_script.Add("FenFengedCantonMapT");
			cfg_script.Push("FenFengedCantonMapT");
			FenFengedCantonMap::iterator ffcmItr = m_fenFengedCantonMapT.begin();
			for( ; ffcmItr != m_fenFengedCantonMapT.end(); ++ ffcmItr)
			{
				cfg_script.Add("Param",ffcmItr->first);
				cfg_script.Add("Param",ffcmItr->second);
			}
			cfg_script.Pop();
		}

		if (!m_fenFengedCantonMapY.empty())
		{
			cfg_script.Add("FenFengedCantonMapY");
			cfg_script.Push("FenFengedCantonMapY");
			FenFengedCantonMap::iterator ffcmItr = m_fenFengedCantonMapY.begin();
			for( ; ffcmItr != m_fenFengedCantonMapY.end(); ++ ffcmItr)
			{
				cfg_script.Add("Param",ffcmItr->first);
				cfg_script.Add("Param",ffcmItr->second);
			}
			cfg_script.Pop();
		}

		if (!m_grouptroopsMap.empty())
		{
			cfg_script.Add("GroupTroopsMap");
			cfg_script.Push("GroupTroopsMap");
			TroopsMap::iterator gtcmItr = m_grouptroopsMap.begin();
			for( ; gtcmItr != m_grouptroopsMap.end(); ++ gtcmItr)
			{
				cfg_script.Add("Param",gtcmItr->first);
				cfg_script.Add("Param",gtcmItr->second);
			}
			cfg_script.Pop();
		}

		if (!(m_applyCantonMapT.empty()))
		{
			cfg_script.Add("ApplyCantonMapT");
			cfg_script.Push("ApplyCantonMapT");
			ApplyCantonMap::iterator acmItr = m_applyCantonMapT.begin();
			for( ; acmItr != m_applyCantonMapT.end(); ++ acmItr )
			{
				cfg_script.Add("Param",acmItr->first);
				cfg_script.Add("Param",acmItr->second);
			}
			cfg_script.Pop();
		}

		if (!(m_applyCantonMapY.empty()))
		{
			cfg_script.Add("ApplyCantonMapY");
			cfg_script.Push("ApplyCantonMapY");
			ApplyCantonMap::iterator acmItr = m_applyCantonMapY.begin();
			for( ; acmItr != m_applyCantonMapY.end(); ++ acmItr )
			{
				cfg_script.Add("Param",acmItr->first);
				cfg_script.Add("Param",acmItr->second);
			}
			cfg_script.Pop();
		}

		if (!(m_fenFengedChengChiMap.empty()))
		{
			cfg_script.Add("FenFengedChengChiMap");
			cfg_script.Push("FenFengedChengChiMap");
			FenFengedChengChiMap::iterator ffccmItr = m_fenFengedChengChiMap.begin();
			for( ; ffccmItr != m_fenFengedChengChiMap.end(); ++ ffccmItr )
			{
				cfg_script.Add("Param",ffccmItr->first);
				cfg_script.Add("Param",ffccmItr->second);
			}
			cfg_script.Pop();
		}

		if (!m_fenFengedChengChiSet.empty())
		{
			cfg_script.Add("FenFengedChengChiSet");
			cfg_script.Push("FenFengedChengChiSet");
			FenFengedChengChiSet::iterator ffccsItr = m_fenFengedChengChiSet.begin();
			for( ; ffccsItr != m_fenFengedChengChiSet.end(); ++ ffccsItr)
			{
				cfg_script.Add("Param",*ffccsItr);
			}
			cfg_script.Pop();
		}
		
		//if (!(m_earningsVec.empty()))
		//{
		//	cfg_script.Add("GroupEarningsVec");
		//	cfg_script.Push("GroupEarningsVec");
		//	GroupEarningsVec::iterator gevItr = m_earningsVec.begin();
		//	for( ; gevItr != m_earningsVec.end(); ++ gevItr)
		//	{
		//		if (gevItr->valid())
		//		{
		//			cfg_script.Add("Param",(*gevItr)->m_type);
		//			cfg_script.Add("Param",(*gevItr)->m_num);
		//			cfg_script.Add("Param",(*gevItr)->m_earningsScale);
		//		}
		//	}
		//	cfg_script.Pop();
		//}
		if (!(m_consortiarecordsDeq.empty()))
		{
			cfg_script.Add("ConsortiaRecordsDeq");
			cfg_script.Push("ConsortiaRecordsDeq");
			ConsortiaRecordsDeq::iterator crdItr = m_consortiarecordsDeq.begin();
			for (;crdItr != m_consortiarecordsDeq.end(); ++ crdItr)
			{
				cfg_script.Add("Param", *crdItr);
			}
			cfg_script.Pop();

		}
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream||LOINT64(msg) == WCH_BuildNetStream)
	{
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		//m_dataStream = new crStreamBuf;
		stream->createBuf(11+(m_consortiaMemberMap.size()+m_applyvec.size())*34);
		stream->_writeUChar(m_lv);
		stream->_writeUChar(m_shiliid);
		stream->_writeUChar(m_manorlevel);
		stream->_writeUChar(m_barrackslevel);
		stream->_writeInt(m_funds);
		stream->_writeInt(m_dismissTime);

		stream->_writeString(m_recruitNotice);
		stream->_writeString(m_contactNotice);

		stream->_writeShort(m_consortiaMemberMap.size());
		for( ConsortiaMemberMap::iterator itr = m_consortiaMemberMap.begin();
			 itr != m_consortiaMemberMap.end();
			 ++itr )
		{
			stream->_writeInt(itr->first);//4
			stream->_writeString(itr->second->getName());//20
			stream->_writeUChar(itr->second->getLevel());
			stream->_writeUChar(itr->second->getPos());

			stream->_writeUChar(itr->second->getGroupPos());
			stream->_writeInt(itr->second->getActiveVaule());

			stream->_writeInt64(itr->second->getTime());
		}
		stream->_writeUChar(m_applyvec.size());
		for (ConsortiaApplyVec::iterator itr = m_applyvec.begin();
			itr != m_applyvec.end();
			++itr)
		{
			stream->_writeInt((*itr)->getPlayerID());
			stream->_writeString((*itr)->getName());
			stream->_writeUChar((*itr)->getLevel());
			stream->_writeUChar((*itr)->getPos());
			stream->_writeInt64((*itr)->getTime());
		}

		stream->seekBegin();
		CRNet::creCompress(stream);
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crJXJConsortiaData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJConsortialv:
		m_lv = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_JXJManorlevel:
		m_manorlevel = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_JXJBarrackslevel:
		m_barrackslevel = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_JXJConsortiaModify:
		m_modify = param==NULL?false:*((bool*)param);
		break;
	case WCHDATA_JXJConsortiaShiliID:
		m_shiliid = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_JXJConsortiaFunds:				
		m_funds = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJConsortiaDismissTime:
		m_dismissTime = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJConsortiaRecruitNotice:
		m_recruitNotice = *(std::string *)param;
		break;
	case WCHDATA_JXJConsortiaContactNotice:
		m_contactNotice = *(std::string *)param;
		break;
	case WCHDATA_JXJConsortiaName:
		m_groupName = *(std::string *)param;
		break;
	case WCHDATA_JXJConsortiaFengYi:
		m_fengyi = *(std::string *)param;
		break;
	case WCHDATA_JXJConsortiaNormalNotice:
		m_notice = *(std::string *)param;
		break;
	case WCHDATA_JXJGroupEarningsScaleY:					
		m_earningsScaleY = *(int *)param;
		break;
	case WCHDATA_JXJGroupEarningsScaleT:					
		m_earningsScaleT = *(int *)param;
		break;
	case WCHDATA_JXJGroupDayLastUpdate			:
		m_dayLastUpdate = *(time_t *)param;
		break;
	case WCHDATA_JXJGroupWeekLastUpdate:
		m_weekLastUpdate = *(time_t *)param;
		break;
	case WCHDATA_JXJConquest:
		m_conquest = param==NULL?0:*((unsigned char*)param);
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJConsortiaData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			int nParam = 1;
			int nParm_up = 1;
			int int1;
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}

			int version = 0;
			cfg_script.Get("VERSION",version);

			int curplayerid = 0;
			m_consortiaMemberMap.clear();
			m_groupLeaderVec.clear();
			m_groupLeaderVec.resize(1);
			ref_ptr<crJXJConsortiaMember> memberinfo;
			if(cfg_script.Push("ConsortiaMemberMap"))
			{
				nParam = 1;
				bool isFirstLeader = false;
				bool isSecondLeader = false;
				unsigned char idx = 0;
				while(cfg_script.Get("Param", curplayerid,nParam++))
				{
					memberinfo = new crJXJConsortiaMember(curplayerid);
					cfg_script.Get("Param", str,nParam++);
					memberinfo->setName(str);
					cfg_script.Get("Param", int1,nParam++);
					memberinfo->setLevel((unsigned char)int1);
					cfg_script.Get("Param", int1,nParam++);
					memberinfo->setPos((unsigned char)int1);

					cfg_script.Get("Param", int1,nParam++);
					memberinfo->setGroupPos((unsigned char)int1);
					cfg_script.Get("Param", int1,nParam++);
					memberinfo->setActiveValue(int1);

					cfg_script.Get("Param", str,nParam++);
					memberinfo->setTime(crArgumentParser::appAtoI64(str));

					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setLastWeekActive(int1);
					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setTodayWeekActive(int1);

					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setEarningsYesterday((bool)int1);

					if (version >= JXJVERSION_0005)
					{
						cfg_script.Get("Param",int1,nParam ++);
						memberinfo->setYesterdayActive(int1);
						cfg_script.Get("Param",int1,nParam ++);
						memberinfo->setNewer((bool)int1);
					}
					if (version >= JXJVERSION_0008)
					{
						cfg_script.Get("Param", str,nParam++);
						memberinfo->setLoginTime(crArgumentParser::appAtoI64(str));
					}
					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setGroupContribute(int1);
					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setTroopStoredNumT(int1);
					cfg_script.Get("Param",int1,nParam ++);
					memberinfo->setTroopTakedNumT(int1);

					CRCore::ref_ptr<ConsortiaMemberRankItem> rankItem = new ConsortiaMemberRankItem(0);
					rankItem->fullOfData(memberinfo.get());
					m_memberRank.updateRank(rankItem->getKey(),rankItem);

					if (memberinfo->getGroupPos() == JXJGroup_FirstLeader)
						m_groupLeaderVec[0] = memberinfo->getPlayerID();
					if (memberinfo->getGroupPos() == JXJGroup_SecondLeader)
						m_groupLeaderVec.push_back(memberinfo->getPlayerID());
					m_consortiaMemberMap.insert(std::make_pair(curplayerid,memberinfo));
				}
				cfg_script.Pop();
			}

			curplayerid = 0;
			if(cfg_script.Push("ConsortiaActiveRank"))
			{
				m_activeRank.clear();
				nParam = 1;
				while(cfg_script.Get("Param", curplayerid,nParam++))
				{
					CRCore::ref_ptr<ConsortiaActiveRankItem> rankItem = new ConsortiaActiveRankItem(curplayerid);
					cfg_script.Get("Param", int1,nParam++);
					rankItem->m_groupPos = (unsigned char )int1;
					cfg_script.Get("Param", int1,nParam++);
					rankItem->m_activeVal = int1;
					cfg_script.Get("Param", str,nParam++);
					rankItem->m_name = str;
					m_activeRank.updateRank(rankItem->getKey(),rankItem);
				}
				cfg_script.Pop();
			}

			curplayerid = 0;
			m_applyvec.clear();
			ref_ptr<crJXJConsortiaApplyMember> applymemberinfo;
			if(cfg_script.Push("ConsortiaApplyVec"))
			{
				nParam = 1;
				while(cfg_script.Get("Param", curplayerid,nParam++))
				{
					applymemberinfo = new crJXJConsortiaApplyMember(curplayerid);
					cfg_script.Get("Param", str,nParam++);
					applymemberinfo->setName(str);
					cfg_script.Get("Param", int1,nParam++);
					applymemberinfo->setLevel((unsigned char)int1);
					cfg_script.Get("Param", int1,nParam++);
					applymemberinfo->setPos((unsigned char)int1);
					cfg_script.Get("Param", str,nParam++);
					applymemberinfo->setTime(crArgumentParser::appAtoI64(str));

					m_applyvec.push_back(applymemberinfo);
				}
				cfg_script.Pop();
			}


		/*	int fubenid = 0;
			if (cfg_script.Push("StartedFubenSet"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",fubenid,nParam++))
				{
					m_startedFuBenSet.insert(fubenid);
				}
				cfg_script.Pop();
			}*/

			int cantonid = 0;
			int earnings = 0;
			nParm_up = 1;
			if (cfg_script.Push("FenFengedCantonMapT"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",cantonid,nParam++))
				{
					cfg_script.Get("Param",earnings,nParam ++);
					m_fenFengedCantonMapT.insert(FenFengedCantonMap::value_type(cantonid,earnings));
				}
				cfg_script.Pop();
			}
			if (cfg_script.Push("FenFengedCantonMapY"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",cantonid,nParam++))
				{
					cfg_script.Get("Param",earnings,nParam ++);
					m_fenFengedCantonMapY.insert(FenFengedCantonMap::value_type(cantonid,earnings));
				}
				cfg_script.Pop();
			}

			int count = 0;
			int abstractid = 0;
			if (cfg_script.Push("GroupTroopsMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",abstractid,nParam++))
				{
					cfg_script.Get("Param",count,nParam ++);
					m_grouptroopsMap.insert(TroopsMap::value_type(abstractid,count));
				}
				cfg_script.Pop();
			}

			nParm_up = 1;
			if (cfg_script.Push("ApplyCantonMapT"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",cantonid,nParam++))
				{
					cfg_script.Get("Param",earnings,nParam ++);
					m_applyCantonMapT.insert(FenFengedCantonMap::value_type(cantonid,earnings));
				}
				cfg_script.Pop();
			}
			if (cfg_script.Push("ApplyCantonMapY"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",cantonid,nParam++))
				{
					cfg_script.Get("Param",earnings,nParam ++);
					m_applyCantonMapY.insert(FenFengedCantonMap::value_type(cantonid,earnings));
				}
				cfg_script.Pop();
			}

			int chengchiid = 0;
			int playerid = 0;
			if (cfg_script.Push("FenFengedChengChiMap"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",chengchiid,nParam++))
				{
					cfg_script.Get("Param",playerid,nParam ++);
					m_fenFengedChengChiMap.insert(FenFengedChengChiMap::value_type(chengchiid,playerid));
				}
				cfg_script.Pop();
			}

			chengchiid = 0;
			if (cfg_script.Push("FenFengedChengChiSet"))
			{
				nParam = 1;
				while(cfg_script.Get("Param",chengchiid ,nParam++))
				{
					m_fenFengedChengChiSet.insert(chengchiid);
				}
				cfg_script.Pop();
			}

			//if (cfg_script.Push("GroupEarningsVec"))
			//{
			//	nParam = 1;
			//	int type,num;
			//	while(cfg_script.Get("Param",type,nParam++))
			//	{
			//		CRCore::ref_ptr<GroupEarnings> earnings = new GroupEarnings;
			//		earnings->m_type = type;
			//		cfg_script.Get("Param",num,nParam ++);
			//		earnings->m_num = num;
			//		cfg_script.Get("Param",num,nParam ++);
			//		earnings->m_earningsScale = num;
			//		m_earningsVec.push_back(earnings);
			//	}
			//	cfg_script.Pop();
			//}
			if (cfg_script.Push("ConsortiaRecordsDeq"))
			{
				nParam = 1;
				while (cfg_script.Get("Param",str,nParam++))
				{
					m_consortiarecordsDeq.push_back(str);
				}

			}

			cfg_script.Close();			
			DeleteFileA(file);
		}
		break;
	case WCHDATA_NetStream:
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJConsortiaData PlayerStream UnCompress error"<<std::endl;
			}
			m_lv = dataStream->_readUChar();
			m_shiliid = dataStream->_readUChar();
			m_manorlevel = dataStream->_readUChar();
			m_barrackslevel = dataStream->_readUChar();
			m_funds = dataStream->_readInt();
			m_dismissTime = dataStream->_readInt();

			m_recruitNotice = dataStream->_readString();
			m_contactNotice = dataStream->_readString();

			int i = 0;
			int count = dataStream->_readShort();
			m_consortiaMemberMap.clear();
			int playerid;
			ref_ptr<crJXJConsortiaMember> memberinfo;
			for( i=0; i<count; i++ )
			{//
				playerid = dataStream->_readInt();
				memberinfo = new crJXJConsortiaMember(playerid);
				memberinfo->setName(dataStream->_readString());
				memberinfo->setLevel(dataStream->_readUChar());
				memberinfo->setPos(dataStream->_readUChar());

				memberinfo->setGroupPos(dataStream->_readUChar());
				memberinfo->setActiveValue(dataStream->_readInt());

				memberinfo->setTime(dataStream->_readInt64());
				m_consortiaMemberMap.insert(std::make_pair(playerid,memberinfo));
			}
			count = dataStream->_readUChar();
			m_applyvec.clear();
			ref_ptr<crJXJConsortiaApplyMember> applymemberinfo;
			for (int i = 0;i<count;i++)
			{
				playerid = dataStream->_readInt();
				applymemberinfo = new crJXJConsortiaApplyMember(playerid);
				applymemberinfo->setName(dataStream->_readString());
				applymemberinfo->setLevel(dataStream->_readUChar());
				applymemberinfo->setPos(dataStream->_readUChar());
				applymemberinfo->setTime(dataStream->_readInt64());
				m_applyvec.push_back(applymemberinfo);
			}
			//m_dataStream = NULL;
		}
		break;
	}
}

void crJXJConsortiaData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJConsortialv:
		param = &m_lv;
		break;
	case WCHDATA_JXJManorlevel:
		param = &m_manorlevel;
		break;
	case WCHDATA_JXJBarrackslevel:
		param = &m_barrackslevel;
		break;
	case WCHDATA_JXJConsortiaMemberMap:
		param = &m_consortiaMemberMap;
		break;
	case WCHDATA_JXJConsortiaLeaderVec:
		param = &m_groupLeaderVec;
		break;
	case WCHDATA_JXJConsortiaApplyVec:
		param = &m_applyvec;
		break;
	case WCHDATA_JXJConsortiaModify:
		param = &m_modify;
		break;
	case WCHDATA_JXJConsortiaShiliID:
		param = &m_shiliid;
		break;
	case WCHDATA_JXJConsortiaFunds:
		param = &m_funds;
		break;
	case WCHDATA_JXJConsortiaDismissTime:
		param = &m_dismissTime;
		break;
	case WCHDATA_JXJConsortiaRecruitNotice:
		param = &m_recruitNotice;
		break;
	case WCHDATA_JXJConsortiaContactNotice:
		param = &m_contactNotice;
		break;
	case WCHDATA_JXJConsortiaNormalNotice:
		param = &m_notice;
		break;
	case WCHDATA_JXJConsortiaName:
		param = &m_groupName;
		break;
	case WCHDATA_JXJConsortiaFengYi:
		param = &m_fengyi;
		break;
		/*case WCHDATA_JXJGroupFenFengedChengChiSet:
		param = &m_fenFengedChengChiSet;
		break;*/
	case WCHDATA_JXJGroupFenFengedCantonMapT:
		param = &m_fenFengedCantonMapT;
		break;
	case WCHDATA_JXJGroupFenFengedCantonMapY:
		param = &m_fenFengedCantonMapY;
		break;
	//case WCHDATA_JXJMyGroupStartedFuBenSet:
	//	param = &m_startedFuBenSet;
	//	break;
	case WCHDATA_JXJGroupEarningsScaleY:
		param = &m_earningsScaleY;
		break;
	case WCHDATA_JXJGroupEarningsScaleT:
		param = &m_earningsScaleT;
		break;
	//case WCHDATA_JXJGroupEarningsVec:				
	//	param = & m_earningsVec;
	//	break;
	case WCHDATA_JXJGroupTotalActiveValY:
		param = & m_totalActiveValY;
		break;
	case WCHDATA_JXJGroupApplyCantonMapY:
		param = & m_applyCantonMapY;
		break;
	case WCHDATA_JXJGroupApplyCantonMapT:
		param = & m_applyCantonMapT;
		break;
	case WCHDATA_JXJConsortiaActiveRank:
		param = & m_activeRank;
		break;
	case WCHDATA_JXJConsortiaMemberRank:
		param = & m_memberRank;
		break;
	case WCHDATA_JXJGroupDayLastUpdate			:
		param = &m_dayLastUpdate;
		break;
	case WCHDATA_JXJGroupWeekLastUpdate:
		param = & m_weekLastUpdate;
		break;
	case WCHDATA_JXJConquest:
		param = & m_conquest;
		break;
	case WCHDATA_JXJMyGroupStartedFuBenMap:
		param = & m_startedFuBenMap;
		break;
	case WCHDATA_JXJGroupTroopsMap:
		param = & m_grouptroopsMap;
		break;
	case WCHDATA_JXJFenFengedChengChiMap:
		param = & m_fenFengedChengChiMap;
		break;
	case WCHDATA_JXJConsortiaRecordsDeq:
		param = & m_consortiarecordsDeq;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crJXJShiliData
//
/////////////////////////////////////////
GNE::Mutex crJXJShiliData::m_saveMutex;
crJXJShiliData::crJXJShiliData():
	m_chengxiang(0),
	m_chengxiangName(""),
	m_dasima(0),
	m_dasimaName(""),
	m_dajiangjun(0),
	m_dajiangjunName(""),
	m_notice(""),
	m_sijintai(0),
	m_shennongtai(0),
	m_simajian(0),
	m_lubansi(0),
	m_dianjiangtai(0),
	m_modify(false),
	m_startedFete(false),
	m_startedEnrichCountry(false),
	m_smallAccLimit(0),
	m_chengxiangLastLogin(0L),
	m_dajiangjunLastLogin(0L),
	m_dasimaLastLogin(0L),
	m_shiliDayLastZero(0L),
	m_version(0),
	m_winCity(0),
	m_cZhanLingChengChi(0),
	m_cyesterdayconqueredcity(0),
	m_countrystrength(1),
	m_ctodayconqueredcity(0),
	m_guojiazijn(0),
	m_destroycountryid1(-1),
	m_destroycountryid2(-1)
{
	m_cantonMap.clear();
}
crJXJShiliData::crJXJShiliData(const crJXJShiliData& data):
	crData(data),
	m_chengxiang(data.m_chengxiang),
	m_chengxiangName(data.m_chengxiangName),
	m_dasima(data.m_dasima),
	m_dasimaName(data.m_dasimaName),
	m_dajiangjun(data.m_dajiangjun),
	m_dajiangjunName(data.m_dajiangjunName),
	m_notice(data.m_notice),
	m_sijintai(data.m_sijintai),
	m_shennongtai(data.m_shennongtai),
	m_simajian(data.m_simajian),
	m_lubansi(data.m_lubansi),
	m_dianjiangtai(data.m_dianjiangtai),
	m_modify(data.m_modify),
	m_startedFete(data.m_startedFete),
	m_startedEnrichCountry(data.m_startedEnrichCountry),
	m_chengxiangLastLogin(data.m_chengxiangLastLogin),
	m_dajiangjunLastLogin(data.m_dajiangjunLastLogin),
	m_dasimaLastLogin(data.m_dajiangjunLastLogin),
	m_version(data.m_version),
	m_winCity(data.m_winCity),
	m_cZhanLingChengChi(data.m_cZhanLingChengChi),
	m_cyesterdayconqueredcity(data.m_cyesterdayconqueredcity),
	m_countrystrength(data.m_countrystrength),
	m_ctodayconqueredcity(data.m_ctodayconqueredcity),
	m_smallAccLimit(data.m_smallAccLimit),
	m_guojiazijn(data.m_guojiazijn),
	m_guojiajifen(data.m_guojiajifen),
	m_destroycountryid1(data.m_destroycountryid1),
	m_destroycountryid2(data.m_destroycountryid2),
	m_todayhavelandedplayer(data.m_todayhavelandedplayer)
{
}
void crJXJShiliData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_JXJChengxiang:
		m_chengxiang = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJChengxiangName:
		m_chengxiangName = relStr;
		break;
	case WCHDATA_JXJDasima:
		m_dasima = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDasimaName:
		m_dasimaName = relStr;
		break;
	case WCHDATA_JXJDajiangjun:
		m_dajiangjun = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDajiangjunName:
		m_dajiangjunName = relStr;
		break;
	case WCHDATA_JXJCountryNotice:
		m_notice = relStr;
		break;
	case WCHDATA_JXJSijintai:
		m_sijintai = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJShenNongMiao:
		m_shennongtai = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJSiMaJian:
		m_simajian = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJShangLinYuan:
		m_lubansi = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJTaiMiao:
		m_dianjiangtai = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCountryStartedFete:
		m_startedFete = (bool)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCountryStartedEnrichCountry:
		m_startedEnrichCountry = (bool)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJChengxiangLastLogin			:
		m_chengxiangLastLogin = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJDasimaLastLogin				:
		m_dasimaLastLogin = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJDajiangjunLastLogin			:	
		m_dajiangjunLastLogin = crArgumentParser::appAtoI64(relStr);
		break;
	case WCHDATA_JXJShiliDayLastUpdate:
		{
			m_shiliDayLastZero = crArgumentParser::appAtoI64(relStr);
			time_t nowTime = time(0);
			if(m_shiliDayLastZero>nowTime)
			{
				m_shiliDayLastZero = nowTime;
			}
		}
		break;
	case WCHDATA_JXJShiliFoundCountryWinCity:
		m_winCity = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJZhanLingCityCount:
		m_cZhanLingChengChi = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJYesterdayConqueredTity:
		m_cyesterdayconqueredcity = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCountryStrength:
		m_countrystrength = (unsigned char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJCountryCapital:
		m_guojiazijn = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJGuoJiaJiFen:
		crArgumentParser::appAtoVec(relStr,m_guojiajifen);
		break;
	case WCHDATA_JXJDestroyCountryid1:
		m_destroycountryid1 = (char)atoi(relStr.c_str());
		break;
	case WCHDATA_JXJDestroyCountryid2:
		m_destroycountryid2 = (char)atoi(relStr.c_str());
		break;
	default:
		break;
	}
}
void crJXJShiliData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		void *param;
		crData *brainData = crServerBrainHandle::getInstance()->getDataClass();
		brainData->getParam(WCHDATA_JXJShiliDayLastUpdate,param);
		time_t lastupdate = *(time_t *)param;
		m_shiliDayLastZero = lastupdate;
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		char buf[512];
		sprintf(buf,"%d,%d\0",WCHDATA_JXJChengxiang,m_chengxiang);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJChengxiangName,m_chengxiangName.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDasima,m_dasima);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDasimaName,m_dasimaName.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDajiangjun,m_dajiangjun);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDajiangjunName,m_dajiangjunName.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf, "%d,%s\0",WCHDATA_JXJCountryNotice,m_notice.c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSijintai,m_sijintai);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJShenNongMiao,m_shennongtai);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJSiMaJian,m_simajian);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJShangLinYuan,m_lubansi);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJTaiMiao,m_dianjiangtai);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryStartedFete,m_startedFete);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryStartedEnrichCountry,m_startedEnrichCountry);
		cfg_script.Add("Param",buf);


		sprintf(buf,"%d,%s\0",WCHDATA_JXJChengxiangLastLogin,crArgumentParser::appI64toa(m_chengxiangLastLogin).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDasimaLastLogin,crArgumentParser::appI64toa(m_dasimaLastLogin).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJDajiangjunLastLogin,crArgumentParser::appI64toa(m_dajiangjunLastLogin).c_str());
		cfg_script.Add("Param",buf);


		sprintf(buf,"%d,%s\0",WCHDATA_JXJShiliDayLastUpdate,crArgumentParser::appI64toa(m_shiliDayLastZero).c_str());
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJShiliFoundCountryWinCity,m_winCity);
		cfg_script.Add("Param",buf);

		sprintf(buf,"%d,%d\0",WCHDATA_JXJZhanLingCityCount,m_cZhanLingChengChi);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJYesterdayConqueredTity,m_cyesterdayconqueredcity);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryStrength,m_countrystrength);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJCountryCapital,m_guojiazijn);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%s\0",WCHDATA_JXJGuoJiaJiFen,crArgumentParser::appVectoa(m_guojiajifen).c_str());
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDestroyCountryid1,m_destroycountryid1);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJDestroyCountryid2,m_destroycountryid2);
		cfg_script.Add("Param",buf);

		///////////////////
		cfg_script.Add("VERSION",JXJVERSION);
		if(!m_lostcitydeq.empty())
		{
			cfg_script.Add("LostCityDeq");
			cfg_script.Push("LostCityDeq");
			for( ShiliCityChangeDeq::iterator csItr = m_lostcitydeq.begin();
				csItr != m_lostcitydeq.end();
				++csItr )
			{
				cfg_script.Add("Param",(int)(*csItr));
			}
			cfg_script.Pop();
		}
		///////////////////
		//if(!m_peopleMap.empty())
		//{
		//	cfg_script.Add("PeopleMap");
		//	cfg_script.Push("PeopleMap");
		//	v_str.resize(9);
		//	for( PeopleMap::iterator itr = m_peopleMap.begin();
		//		itr != m_peopleMap.end();
		//		++itr )
		//	{
		//		if (itr->second.valid())
		//		{
		//			v_str[0] = crArgumentParser::appItoa(itr->second->m_isSmallAcc);
		//			v_str[1] = crArgumentParser::appItoa(itr->second->m_lv);
		//			v_str[2] = crArgumentParser::appItoa(itr->second->m_troopsLv);
		//			v_str[3] = crArgumentParser::appItoa(itr->second->m_id);
		//			v_str[4] = crArgumentParser::appItoa(itr->second->m_consortiaId);
		//			v_str[5] = crArgumentParser::appItoa(itr->second->m_killNum);
		//			v_str[6] = itr->second->m_name;
		//			v_str[7] = crArgumentParser::appItoa(itr->second->m_newerStep);
		//			v_str[8] = crArgumentParser::appItoa(itr->second->m_deadTroopsNum);

		//			cfg_script.Add("Param",v_str);
		//		}
		//	}
		//	cfg_script.Pop();
		//}

		///////////////////
		std::vector<std::string> v_str;
		if(!m_cantonMap.empty())
		{
			cfg_script.Add("CantonMap");
			cfg_script.Push("CantonMap");
			int nParam2 = 1;

			for (CantonMap::iterator it = m_cantonMap.begin(); it != m_cantonMap.end(); ++ it)
			{
				if (it->second.valid())
				{
					cfg_script.Add("CantonConsortia");
					cfg_script.Push("CantonConsortia",nParam2 ++);
					v_str.resize(2);
					v_str[0] = crArgumentParser::appItoa(it->first);
					v_str[1] = crArgumentParser::appItoa(it->second->getConsortiaId());
					cfg_script.Add("Param",v_str);

					cfg_script.Add("ChengChiEarningsMap");
					cfg_script.Push("ChengChiEarningsMap");
					v_str.resize(3);
					ChengChiEarningsMap & chengChiEarningsMap = it->second->getChengChiEarningsMap();
					ChengChiEarningsMap ::iterator itr = chengChiEarningsMap.begin();
					for (; itr != chengChiEarningsMap.end(); ++ itr)
					{
						if (itr->second.valid())
						{
							v_str[0] = crArgumentParser::appItoa(itr->second->getGroupId());
							v_str[1] = crArgumentParser::appItoa(itr->second->getChengChiId());
							v_str[2] = crArgumentParser::appItoa(itr->second->getEarnings());
							cfg_script.Add("Param",v_str);
						}
					}
					cfg_script.Pop();
					cfg_script.Pop();
				}
			}
			cfg_script.Pop();
		}
		///////////////////已投票玩家
		if (!m_votersSet.empty())
		{
			cfg_script.Add("VotersSet");
			cfg_script.Push("VotersSet");
			//v_str.resize(2);
			for( VotersSet::iterator itr = m_votersSet.begin();itr != m_votersSet.end();++itr )
			{
				cfg_script.Add("Param",crArgumentParser::appItoa(*itr));
			}
			cfg_script.Pop();
		}
		///////////////////军团开服活动满足资格玩家
		if (!m_consortiaStartPromotionVecT.empty())
		{
			cfg_script.Add("ConsortiaStartPromotionVecT");
			cfg_script.Push("ConsortiaStartPromotionVecT");
			v_str.resize(4);
			for( ConsortiaStartPromotionVec::iterator itr = m_consortiaStartPromotionVecT.begin();itr != m_consortiaStartPromotionVecT.end();++itr )
			{
				if (itr->valid())
				{
					v_str[0] = crArgumentParser::appItoa((*itr)->nPlayerId);
					v_str[1] = crArgumentParser::appItoa((*itr)->rankNum);
					v_str[2] = crArgumentParser::appItoa((*itr)->isConsortiaLeader);
					v_str[3] = crArgumentParser::appItoa((*itr)->groupId);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		///////////////////军团开服活动入围军团
		if (!m_consortiaPromotionRankVecT.empty())
		{
			cfg_script.Add("ConsortiaPromotionRankVecT");
			cfg_script.Push("ConsortiaPromotionRankVecT");
			v_str.resize(2);
			for( ConsortiaStartPromotionRankVec::iterator itr = m_consortiaPromotionRankVecT.begin();itr != m_consortiaPromotionRankVecT.end();++itr )
			{
				if (itr->valid())
				{
					v_str[0] = crArgumentParser::appItoa((*itr)->rankNum);
					v_str[1] = crArgumentParser::appItoa((*itr)->groupId);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		/////////////////////本周 投票候选人
		if (!m_candidateVecT.empty())
		{
			cfg_script.Add("CandidateVecT");
			cfg_script.Push("CandidateVecT");
			v_str.resize(7);
			for( CandidateVec::iterator itr = m_candidateVecT.begin();itr != m_candidateVecT.end();++itr )
			{
				if (itr->valid())
				{
					v_str[0] = crArgumentParser::appItoa((*itr)->nPlayerID);
					v_str[1] = (*itr)->playerName;
					v_str[2] = crArgumentParser::appItoa((*itr)->nJuntuanID);
					v_str[3] = (*itr)->juntuanName;
					v_str[4] = crArgumentParser::appItoa((*itr)->m_rankVal);
					v_str[5] = crArgumentParser::appItoa((*itr)->nNumber);
					v_str[6] = crArgumentParser::appI64toa((*itr)->m_lastLogin);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		/////////////////////上周 投票候选人
		if (!m_candidateVecL.empty())
		{
			cfg_script.Add("CandidateVecL");
			cfg_script.Push("CandidateVecL");
			v_str.resize(7);
			for( CandidateVec::iterator itr = m_candidateVecL.begin();itr != m_candidateVecL.end();++itr )
			{
				if (itr->valid())
				{
					v_str[0] = crArgumentParser::appItoa((*itr)->nPlayerID);
					v_str[1] = (*itr)->playerName;
					v_str[2] = crArgumentParser::appItoa((*itr)->nJuntuanID);
					v_str[3] = (*itr)->juntuanName;
					v_str[4] = crArgumentParser::appItoa((*itr)->m_rankVal);
					v_str[5] = crArgumentParser::appItoa((*itr)->nNumber);
					v_str[6] = crArgumentParser::appI64toa((*itr)->m_lastLogin);
					cfg_script.Add("Param",v_str);
				}
			}
			cfg_script.Pop();
		}
		////////////////////国家历史
		if (!m_countryHistory.empty())
		{
			cfg_script.Add("CountryHistory");
			cfg_script.Push("CountryHistory");
			v_str.resize(4);
			for (CountryHistory::iterator itr = m_countryHistory.begin();itr != m_countryHistory.end();++itr)
			{
				(*itr)->buildSaveStream(v_str);
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		// 小号列表 begin
		if (!m_blacklistSet.empty())
		{
			cfg_script.Add("ShiliBlacklistSet");
			cfg_script.Push("ShiliBlacklistSet");
			for( ShiliBlacklistSet::iterator itr = m_blacklistSet.begin();itr != m_blacklistSet.end();++itr )
			{
				cfg_script.Add("Param",crArgumentParser::appItoa(*itr));
			}
			cfg_script.Pop();
		}
		// 小号列表 end
		///////////////////
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream||LOINT64(msg) == WCH_BuildNetStream)
	{
		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(1024);
		stream->_writeInt(m_chengxiang);
		stream->_writeString(m_chengxiangName);
		stream->_writeInt(m_dasima);
		stream->_writeString(m_dasimaName);
		stream->_writeInt(m_dajiangjun);
		stream->_writeString(m_dajiangjunName);
		stream->_writeString(m_notice);
		stream->_writeInt(m_sijintai);
		stream->_writeInt(m_shennongtai);
		stream->_writeInt(m_simajian);
		stream->_writeInt(m_lubansi);
		stream->_writeInt(m_dianjiangtai);
		stream->_writeUChar(m_winCity);
		stream->_writeUChar(m_cZhanLingChengChi);
		stream->_writeUChar(m_cyesterdayconqueredcity);
		stream->_writeUChar(m_countrystrength);
		stream->_writeInt(m_guojiazijn);
		stream->_writeVec3i(m_guojiajifen);
		stream->_writeChar(m_destroycountryid1);
		stream->_writeChar(m_destroycountryid2);

		///////////////////////
		stream->_writeUChar(m_lostcitydeq.size());//maxsize = 10 WCHDATA_JXJCityChangeDeqMaxCount
		for (ShiliCityChangeDeq::iterator itr = m_lostcitydeq.begin();itr != m_lostcitydeq.end(); ++itr)
		{
			stream->_writeUShort((*itr));
		}
		stream->seekBegin();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crJXJShiliData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_JXJChengxiang:
		m_chengxiang = param==NULL?0:*(int*)param;
		break;
	case WCHDATA_JXJChengxiangName:
		m_chengxiangName = param == NULL ? "":*(std::string *)param;
		break;
	case WCHDATA_JXJDasima:
		m_dasima = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJDasimaName:
		m_dasimaName = param == NULL ? "":*(std::string *)param;
		break;
	case WCHDATA_JXJDajiangjun:
		m_dajiangjun = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJDajiangjunName:
		m_dajiangjunName = param == NULL ? "":*(std::string *)param;
		break;
	case WCHDATA_JXJCountryNotice:
		m_notice = param == NULL ? "":*(std::string *)param;
		break;
	case WCHDATA_JXJSijintai:
		m_sijintai = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJShenNongMiao:
		m_shennongtai = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJSiMaJian:
		m_simajian = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJShangLinYuan:
		m_lubansi = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJTaiMiao:
		m_dianjiangtai = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJShiliModify:
		m_modify = param==NULL?false:*((bool*)param);
		break;
	case WCHDATA_JXJCountryStartedFete:
		m_startedFete = *(bool *)param;
		break;
	case WCHDATA_JXJCountryStartedEnrichCountry:
		m_startedEnrichCountry = *(bool *)param;
		break;
	case WCHDATA_JXJCountrySmallAccNum :
		m_smallAccLimit = *(short *)param;
		break;
	case WCHDATA_JXJChengxiangLastLogin			:
		m_chengxiangLastLogin = param == NULL ? 0:*(_crInt64*)param;
		break;
	case WCHDATA_JXJDasimaLastLogin				:
		m_dasimaLastLogin = param == NULL ? 0:*(_crInt64*)param;
		break;
	case WCHDATA_JXJDajiangjunLastLogin			:	
		m_dajiangjunLastLogin = param == NULL ? 0:*(_crInt64*)param;
		break;
	case WCHDATA_JXJShiliDayLastUpdate			:
		m_shiliDayLastZero = param == NULL ? 0: *(_crInt64 *)param;
		break;
	case WCHDATA_JXJShiliFoundCountryWinCity:
		m_winCity = param == NULL ? 0 : *(unsigned char *)param;
		break;
	case WCHDATA_JXJZhanLingCityCount:
		m_cZhanLingChengChi = param == NULL ? 0 : *(unsigned char *)param;
		break;
	case WCHDATA_JXJYesterdayConqueredTity:
		m_cyesterdayconqueredcity = param == NULL ? 0 : *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryStrength:
		m_countrystrength = param == NULL ? 1 : *(unsigned char *)param;
		break;
	case WCHDATA_JXJCountryCapital:
		m_guojiazijn = param==NULL?0:*((int*)param);
		break;
	case WCHDATA_JXJGuoJiaJiFen:
		m_guojiajifen = param==NULL?crVector3i(0,0,0):*((crVector3i*)param);
		break;
	case WCHDATA_JXJDestroyCountryid1:
		m_destroycountryid1 = param == NULL ? 0 : *(char *)param;
		break;
	case WCHDATA_JXJDestroyCountryid2:
		m_destroycountryid2 = param == NULL ? 0 : *(char *)param;
		break;
	
	//case WCHDATA_JXJCountryToupiaoMap:
	//	m_toupiaoData = *(ToupiaoData*)param;
	//	break;
	//case WCHDATA_JXJCountryToupiaoLast:
	//	m_toupiaoResultLastweek = *(ToupiaoData*)param;
	//	break;
	//case WCHDATA_JXJCountryToupiaoPeople:
	//	m_peopleToupiaoMap = *(PeopleMap*)param;
	//	break;
	//case WCHDATA_JXJCountryHistory:
	//	m_countryHistory = *(CountryHistory*)param;
	//	break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
			if(!CRNet::creUnCompress(dataStream))
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJShiliData creUnCompress error"<<std::endl;
			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			int nParam = 1;
			int int1;
			std::vector<std::string> v_str;
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}

			int version = 0;
			cfg_script.Get("VERSION",version);
			m_version = version;

			m_lostcitydeq.clear();
			if(cfg_script.Push("LostCityDeq"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					m_lostcitydeq.push_back(int1);
				}
				cfg_script.Pop();
			}

			if (version < JXJVERSION_0010)
			{
				m_peopleMap.clear();
				m_smallAccLimit = 0;
				if(cfg_script.Push("PeopleMap"))
				{
					nParam = 1;
					v_str.resize(9);
					while (cfg_script.Get("Param", v_str,nParam++))
					{
						CRCore::ref_ptr<PeopleInfo> people = new PeopleInfo(atoi(v_str[3].c_str()),v_str[6],0);
						people->m_isSmallAcc =				atoi(v_str[0].c_str());
						people->m_lv =						atoi(v_str[1].c_str());
						people->m_troopsLv =					atoi(v_str[2].c_str());
						people->m_consortiaId=				atoi(v_str[4].c_str());
						people->m_killNum =					atoi(v_str[5].c_str());
						if (version >= JXJVERSION_0004)
						{
							people->m_newerStep =			atoi(v_str[7].c_str());
						}
						if (version >= JXJVERSION_0006)
						{
							people->m_deadTroopsNum =		atoi(v_str[8].c_str());
						}
						if (people->m_isSmallAcc)
						{
							++ m_smallAccLimit;
						}
						m_peopleMap.insert(PeopleMap::value_type(people->m_id,people));
					}
					cfg_script.Pop();
				}
			}

			m_cantonMap.clear();
			if (cfg_script.Push("CantonMap"))
			{
				int cantonId = 0;
				int consortiaId = 0;
				nParam = 1;
				v_str.resize(3);

				while (cfg_script.Push("CantonConsortia",nParam ++))
				{
					cfg_script.Get("Param",v_str);
					cantonId = atoi(v_str[0].c_str());
					consortiaId = atoi(v_str[1].c_str());

					CRCore::ref_ptr<Canton> canton = new Canton;
					canton->setCantonId(cantonId);
					canton->setConsortiaId(consortiaId);

					int groupid = 0,chengchiId = 0;
					short earnings = 0;
					ChengChiEarningsMap & chengChiEarningsMap = canton->getChengChiEarningsMap();
					if (cfg_script.Push("ChengChiEarningsMap") )
					{
						int nParam2 = 1;
						while (cfg_script.Get("Param",v_str,nParam2 ++))
						{
							groupid = atoi(v_str[0].c_str());
							chengchiId = atoi(v_str[1].c_str());
							earnings = atoi(v_str[2].c_str());

							CRCore::ref_ptr<ChengChiEarnings> chengChiEarnings = new ChengChiEarnings(groupid);
							chengChiEarnings->setChengChiId(chengchiId);
							chengChiEarnings->setEarnings(earnings);
							chengChiEarningsMap.insert(ChengChiEarningsMap::value_type(groupid,chengChiEarnings));
						}
						cfg_script.Pop();
					}
					cfg_script.Pop();
					m_cantonMap.insert(CantonMap::value_type(cantonId,canton));
				}
				cfg_script.Pop();
			}

			m_votersSet.clear();
			if(cfg_script.Push("VotersSet"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					m_votersSet.insert(int1);
				}
				cfg_script.Pop();
			}

			m_consortiaStartPromotionVecT.clear();
			if(cfg_script.Push("ConsortiaStartPromotionVecT"))
			{
				nParam = 1;
				v_str.resize(4);
				ref_ptr<strConsortiaHuoDongInfo> pStrInfo;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					pStrInfo = new strConsortiaHuoDongInfo;
					pStrInfo->nPlayerId = atoi(v_str[0].c_str());
					pStrInfo->rankNum = atoi(v_str[1].c_str());
					pStrInfo->isConsortiaLeader = atoi(v_str[2].c_str());
					pStrInfo->groupId = atoi(v_str[3].c_str());
					m_consortiaStartPromotionVecT.push_back(pStrInfo);
				}
				cfg_script.Pop();
			}

			m_consortiaPromotionRankVecT.clear();
			if(cfg_script.Push("ConsortiaPromotionRankVecT"))
			{
				nParam = 1;
				v_str.resize(2);
				ref_ptr<strConsortiaHuoDongInfo> pStrInfo;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					pStrInfo = new strConsortiaHuoDongInfo;
					pStrInfo->rankNum = atoi(v_str[0].c_str());
					pStrInfo->groupId = atoi(v_str[1].c_str());
					m_consortiaPromotionRankVecT.push_back(pStrInfo);
				}
				cfg_script.Pop();
			}

			m_candidateVecT.clear();
			if(cfg_script.Push("CandidateVecT"))
			{
				nParam = 1;
				v_str.resize(7);
				ref_ptr<strToupiaoInfo> pStrInfo;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					pStrInfo = new strToupiaoInfo;
					pStrInfo->nPlayerID = atoi(v_str[0].c_str());
					pStrInfo->playerName = v_str[1];
					pStrInfo->nJuntuanID = atoi(v_str[2].c_str());
					pStrInfo->juntuanName = v_str[3];
					pStrInfo->m_rankVal = atoi(v_str[4].c_str());
					pStrInfo->nNumber = atoi(v_str[5].c_str());
					if (version >= JXJVERSION_0006)
					{
						pStrInfo->m_lastLogin = crArgumentParser::appAtoI64(v_str[6]);
					}
					m_candidateVecT.push_back(pStrInfo);
				}
				cfg_script.Pop();
			}

			m_candidateVecL.clear();
			if(cfg_script.Push("CandidateVecL"))
			{
				nParam = 1;
				v_str.resize(7);
				ref_ptr<strToupiaoInfo>pStrInfo;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					pStrInfo = new strToupiaoInfo;
					pStrInfo->nPlayerID = atoi(v_str[0].c_str());
					pStrInfo->playerName = v_str[1];
					pStrInfo->nJuntuanID = atoi(v_str[2].c_str());
					pStrInfo->juntuanName = v_str[3];
					pStrInfo->m_rankVal = atoi(v_str[4].c_str());
					pStrInfo->nNumber = atoi(v_str[5].c_str());
					if (version >= JXJVERSION_0006)
					{
						pStrInfo->m_lastLogin = crArgumentParser::appAtoI64(v_str[6]);
					}
					
					m_candidateVecL.push_back(pStrInfo);
				}
				cfg_script.Pop();
			}

			m_blacklistSet.clear();
			if(cfg_script.Push("ShiliBlacklistSet"))
			{
				nParam = 1;
				while (cfg_script.Get("Param", int1,nParam++))
				{
					m_blacklistSet.insert(int1);
				}
				cfg_script.Pop();
			}

			m_countryHistory.clear();
			if(cfg_script.Push("CountryHistory"))
			{
				nParam = 1;
				v_str.resize(10);
				CountryHistoryManager creator;
				while (cfg_script.Get("Param", v_str,nParam++))
				{
					CRCore::ref_ptr<CountryHistoryInfoBase> item ;
					creator(atoi(v_str[0].c_str()),item);
					if (item.valid())
					{
						item->getSaveStream(v_str);
						m_countryHistory.push_back(item);
					}

					//m_countryHistory.insert(std::make_pair(atoi(v_str[0].c_str()),std::make_pair(atoi(v_str[1].c_str()), v_str[3])));
				}
				cfg_script.Pop();
			}

			m_blacklistSet.clear();
			cfg_script.Close();
			DeleteFileA(file);
		}
		break;
	case WCHDATA_NetStream:
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_chengxiang = dataStream->_readInt();
			m_chengxiangName = dataStream->_readString();
			m_dasima = dataStream->_readInt();
			m_dasimaName = dataStream->_readString();
			m_dajiangjun = dataStream->_readInt();
			m_dajiangjunName = dataStream->_readString();
			m_notice = dataStream->_readString();
			m_sijintai = dataStream->_readInt();
			m_shennongtai = dataStream->_readInt();
			m_simajian = dataStream->_readInt();
			m_lubansi = dataStream->_readInt();
			m_dianjiangtai = dataStream->_readInt();
			m_winCity = dataStream->_readUChar();
			m_cZhanLingChengChi = dataStream->_readUChar();
			m_cyesterdayconqueredcity = dataStream->_readUChar();
			m_countrystrength = dataStream->_readUChar();
			m_guojiazijn = dataStream->_readInt();
			m_guojiajifen = dataStream->_readVec3i();
			m_destroycountryid1 = dataStream->_readChar();
			m_destroycountryid2 = dataStream->_readChar();

			int count = dataStream->_readUChar();
			m_lostcitydeq.clear();
			for( i=0; i<count; i++ )
			{
				m_lostcitydeq.push_back(dataStream->_readUShort());
			}
		}
		break;
	}
}

void crJXJShiliData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJChengxiang:
		param = &m_chengxiang;
		break;
	case WCHDATA_JXJChengxiangName:
		param = &m_chengxiangName;
		break;
	case WCHDATA_JXJDasima:
		param = &m_dasima;
		break;
	case WCHDATA_JXJDasimaName:
		param = &m_dasimaName;
		break;
	case WCHDATA_JXJDajiangjun:
		param = &m_dajiangjun;
		break;
	case WCHDATA_JXJDajiangjunName:
		param = &m_dajiangjunName;
		break;
	case WCHDATA_JXJCountryNotice:
		param = &m_notice;
		break;
	case WCHDATA_JXJSijintai:
		param = &m_sijintai;
		break;
	case WCHDATA_JXJShenNongMiao:
		param = &m_shennongtai;
		break;
	case WCHDATA_JXJSiMaJian:
		param = &m_simajian;
		break;
	case WCHDATA_JXJShangLinYuan:
		param = &m_lubansi;
		break;
	case WCHDATA_JXJTaiMiao:
		param = &m_dianjiangtai;
		break;
	case WCHDATA_JXJShiliWinCityDeq:
		param = &m_wincitydeq;
		break;
	case WCHDATA_JXJShiliLostCityDeq:
		param = &m_lostcitydeq;
		break;
	case WCHDATA_JXJPeopleMap:
		param = &m_peopleMap;
		break;
	case WCHDATA_JXJShiliModify:
		param  = &m_modify;
		break;
	case WCHDATA_JXJShiliCantonMap:
		param = &m_cantonMap;
		break;
	//case WCHDATA_JXJCountryToupiaoMap:
	//	param = &m_toupiaoData;
	//	break;
	case WCHDATA_JXJCountryCandidateVecL:
		param = &m_candidateVecL;
		break;
	case WCHDATA_JXJCountryVotersSet:
		param = & m_votersSet;
		//param = &m_peopleToupiaoMap;
		break;
	case WCHDATA_JXJCountryCandidateVecT:
		param = &m_candidateVecT;
		break;
	case WCHDATA_JXJCountryHistory:
		param = &m_countryHistory;
		break;
	case WCHDATA_JXJShiliDataTempPeopleVec:
		param = & m_tempPeopleVec;
		break;
	case WCHDATA_JXJShiliBlacklistSet:
		param = & m_blacklistSet;
		break;
	case WCHDATA_JXJQueryFindByMinLvMap:
		param = & m_queryFindbyMinLvMap;
		break;
	case WCHDATA_JXJQueryFindByMaxLvMap:
		param = & m_queryFindbyMaxLvMap;
		break;
	case WCHDATA_JXJCountryStartedFete:
		param = & m_startedFete;
		break;
	case WCHDATA_JXJCountryStartedEnrichCountry:
		param = & m_startedEnrichCountry;
		break;
	case WCHDATA_JXJCountrySmallAccNum :
		param = & m_smallAccLimit;
		break;
	case WCHDATA_JXJChengxiangLastLogin			:
		param = &m_chengxiangLastLogin;
		break;
	case WCHDATA_JXJDasimaLastLogin				:
		param = &m_dasimaLastLogin ;
		break;
	case WCHDATA_JXJDajiangjunLastLogin			:	
		param = & m_dajiangjunLastLogin ;
		break;
	case WCHDATA_JXJShiliDayLastUpdate			:
		param = & m_shiliDayLastZero;
		break;
	case WCHDATA_JXJModifyPlayerSet:
		param = & m_modifyPlayerSet;
		break;
	case WCHDATA_JXJInsertPlayerSet:
		param = &m_insertPlayerSet;
		break;
	case WCHDATA_JXJShiliVersion:
		param = &m_version;
		break;
	case WCHDATA_JXJShiliFoundCountryWinCity:
		param = &m_winCity;
		break;
	case WCHDATA_JXJZhanLingCityCount:
		param = &m_cZhanLingChengChi;
		break;
	case WCHDATA_JXJYesterdayConqueredTity:
		param = &m_cyesterdayconqueredcity;
		break;
	case WCHDATA_JXJCountryStrength:
		param = &m_countrystrength;
		break;
	case WCHDATA_JXJTodayConqueredTity:
		param = &m_ctodayconqueredcity;
		break;
	case WCHDATA_JXJCountryCapital:
		param = &m_guojiazijn;
		break;
	case WCHDATA_JXJGuoJiaJiFen:
		param = &m_guojiajifen;
		break;
	case WCHDATA_JXJDestroyCountryid1:
		param = &m_destroycountryid1;
		break;
	case WCHDATA_JXJDestroyCountryid2:
		param = &m_destroycountryid2;
		break;
	case WCHDATA_JXJConsortiaPromotionPlayerVecT:
		param = &m_consortiaStartPromotionVecT;
		break;
	case WCHDATA_JXJConsortiaPromotionRankVecT:
		param = &m_consortiaPromotionRankVecT;
		break;
	case WCHDATA_JXJTodayHaveLandedPlayerSet:
		param = &m_todayhavelandedplayer;
		break;
	default:
		param = NULL;
		break;
	}
}
/////////////////////////////////////////
//
//crJXJSelectServerUIData
//
/////////////////////////////////////////
crJXJSelectServerUIData::crJXJSelectServerUIData():
	m_curpage(1),
	m_totalpage(1)
{
}
crJXJSelectServerUIData::crJXJSelectServerUIData(const crJXJSelectServerUIData& data):
	crUIData(data),
	m_curpage(data.m_curpage),
	m_totalpage(data.m_totalpage)
{
}
void crJXJSelectServerUIData::addParam(int i, const std::string& str)
{
}
void crJXJSelectServerUIData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_JXJSelectServerUICurPage:
		m_curpage = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJSelectServerUITotalPage:
		m_totalpage = param?*((int*)param):NULL;
		if (m_totalpage < 1)
		{
			m_totalpage = 1;
		}
		break;
	}
}
void crJXJSelectServerUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJSelectServerUICurPage:
		param = &m_curpage;
		break;
	case WCHDATA_JXJSelectServerUITotalPage:
		param = &m_totalpage;
		break;
	case WCHDATA_JXJServerListMap:
		param = &m_serverListMap;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
/////////////////////////////////////////
//
//crJXJPlayerStoreData
//
/////////////////////////////////////////
GNE::Mutex crJXJPlayerStoreData::m_saveMutex;
crJXJPlayerStoreData::crJXJPlayerStoreData():
	m_money(0),
	m_playerUsedMoney(0),
	m_cyclecount(0)
	//m_itembagnum(0),
	//m_jiangkabagnum(0)
{
}
crJXJPlayerStoreData::crJXJPlayerStoreData(const crJXJPlayerStoreData& data):
	crData(data),
	m_money(data.m_money),
	m_playerUsedMoney(data.m_playerUsedMoney),
	m_cyclecount(data.m_cyclecount)
	//m_itembagnum(data.m_itembagnum),
	//m_jiangkabagnum(data.m_jiangkabagnum)
{
}
void crJXJPlayerStoreData::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_Money:
		m_money = atoi(relStr.c_str());
		break;
	case WCHDATA_JXJPlayerTotalAddMoney:
		m_playerUsedMoney = atoi(relStr.c_str());
		break;
// 	case WCHDATA_JXJItemBagNum:
// 		m_itembagnum = atoi(relStr.c_str());
// 		break;
// 	case WCHDATA_JXJJiangkaBagNum:
// 		m_jiangkabagnum = atoi(relStr.c_str());
// 		break;
	}
}
void crJXJPlayerStoreData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_BuildSaveStream)
	{
		GNE::LockMutex lock( m_dataMutex );
		ref_ptr<crStreamBuf> stream = (crStreamBuf *)(HIINT64(msg));
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		std::vector<std::string> v_str;
		char buf[128];
		sprintf(buf,"%d,%d\0",WCHDATA_Money,m_money);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerTotalAddMoney,m_playerUsedMoney);
		cfg_script.Add("Param",buf);
		sprintf(buf,"%d,%d\0",WCHDATA_JXJPlayerCycleCount,m_cyclecount);
		cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJItemBagNum,m_itembagnum);
		//cfg_script.Add("Param",buf);
		//sprintf(buf,"%d,%d\0",WCHDATA_JXJJiangkaBagNum,m_jiangkabagnum);
		//cfg_script.Add("Param",buf);
		/////////背包列表
		if(!m_itembagvec.empty())
		{
			int count = m_itembagvec.size();
			cfg_script.Add("JXJItemStoreBagVec");
			cfg_script.Push("JXJItemStoreBagVec");
			v_str.resize(5);
			for( int i = 0; i < count; i++ )
			{
				if((m_itembagvec)[i].valid() && (m_itembagvec)[i]->getItemCount()>0)
				{
					v_str[0] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemID());
					v_str[1] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemCount());
					v_str[2] = crArgumentParser::appItoa((m_itembagvec)[i]->getEquipMagic());
					v_str[3] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemServerID());
					v_str[4] = crArgumentParser::appItoa((m_itembagvec)[i]->getItemHasCycle());
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
					v_str[2] = "";
					v_str[3] = "";
					v_str[4] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		///////////将卡背包列表
		if(!m_jiangkabagvec.empty())
		{
			int bagcount = m_jiangkabagvec.size();
			cfg_script.Add("JXJJiangkaItemBagVec");
			cfg_script.Push("JXJJiangkaItemBagVec");
			v_str.resize(4);
			for( int i = 0;i<bagcount ; i++)
			{
				if(m_jiangkabagvec[i].valid() && m_jiangkabagvec[i]->getItemCount()>0)
				{
					v_str[0] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getItemID());
					v_str[1] = crArgumentParser::appItoa(m_jiangkabagvec[i]->getItemCount());
					v_str[2] = crArgumentParser::appItoa((m_jiangkabagvec)[i]->getItemServerID());
					v_str[3] = crArgumentParser::appItoa((m_jiangkabagvec)[i]->getItemHasCycle());
				}
				else
				{
					v_str[0] = "";
					v_str[1] = "";
					v_str[2] = "";
					v_str[3] = "";
					//v_str[2] = "";
				}
				cfg_script.Add("Param",v_str);
			}
			cfg_script.Pop();
		}
		///////////////////
		{
			GNE::LockMutex lock( m_saveMutex );
			sprintf(buf,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			cfg_script.Write(buf,"");
			cfg_script.Close();

			//m_dataStream = new crStreamBuf;
			stream->loadFromFile2(buf);
			stream->seekBegin();
			CRNet::creCompress(stream);
			DeleteFileA(buf);
		}
	}
	else if(LOINT64(msg) == WCH_BuildPlayerStream)
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int itemtypeindex= itemtab->getTitleIndex("类型");
		//int itemtype;
		crTableIO::StrVec itemrecord;
		CRNet::crNetConductor *gameServer = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		CRNet::crNetDataManager *netDataManager = gameServer->getNetDataManager();
		CRNetApp::crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int serverid = callback->getServerID();

		crStreamBuf *stream = (crStreamBuf *)(HIINT64(msg));
		stream->createBuf(1024);
		stream->_writeInt(m_money);
		stream->_writeInt(m_playerUsedMoney);
		stream->_writeUShort(m_cyclecount);
		//背包列表
		stream->_writeUChar(m_itembagvec.size());
		for( int i = 0; i < m_itembagvec.size(); i++)
		{
			if(m_itembagvec[i].valid() && m_itembagvec[i]->getItemCount()>0)
			{
				stream->_writeUChar(m_itembagvec[i]->getItemCount());
				stream->_writeInt(m_itembagvec[i]->getItemID());
				stream->_writeInt(m_itembagvec[i]->getEquipMagic());
				
				if(m_itembagvec[i]->getItemServerID()==serverid)
				{
					stream->_writeInt(1);
				}
				else
				{
					stream->_writeInt(0);
				}
				stream->_writeUChar(m_itembagvec[i]->getItemHasCycle());
			}
			else
			{
				stream->_writeUChar(0);
			}
		}
		//将卡背包列表
		stream->_writeUChar(m_jiangkabagvec.size());
		for( int i = 0; i < m_jiangkabagvec.size();i++ )
		{
			if(m_jiangkabagvec[i].valid() && m_jiangkabagvec[i]->getItemCount()>0)
			{
				unsigned char itemcount = m_jiangkabagvec[i]->getItemCount();
				stream->_writeUChar(itemcount);
				stream->_writeInt(m_jiangkabagvec[i]->getItemID());
				if(m_jiangkabagvec[i]->getItemServerID()==serverid)
				{
					stream->_writeInt(1);
				}
				else
				{
					stream->_writeInt(0);
				}
				stream->_writeUChar(m_jiangkabagvec[i]->getItemHasCycle());
			}
			else
			{
				stream->_writeUChar(0);
			}
		}

		stream->seekBegin();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}
void crJXJPlayerStoreData::inputParam(int i, void *param)
{
	GNE::LockMutex lock( m_dataMutex );
	switch(i) 
	{
	case WCHDATA_Money:
		m_money = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJPlayerTotalAddMoney:
		m_playerUsedMoney = param?*((int*)param):NULL;
		break;
	case WCHDATA_DataStream:
		if(param)
		{
			GNE::LockMutex lock( m_saveMutex );
			ref_ptr<crStreamBuf> dataStream =(crStreamBuf*)param;
// 			if(!CRNet::creUnCompress(dataStream))
// 			{
// 				CRCore::notify(CRCore::ALWAYS)<<"crJXJPlayerStoreData creUnCompress error"<<std::endl;
// 			}
			char file[128];
			sprintf(file,"%s%d%d.tmp\0",_name(),GetCurrentThreadId(),time(0));
			dataStream->saveToFile2(file);
			rcfg::ConfigScript cfg_script;
			cfg_script.Open(file,"");
			std::string str;
			int nParam = 1;
			std::vector<std::string> v_str;
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}
			while (cfg_script.Get("Param", str,nParam++))
			{
				addParam(nParam-2,str);
			}
			//背包列表
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int itemtypeindex= itemtab->getTitleIndex("类型");
			int itemtype;
			crTableIO::StrVec itemrecord;
			
			m_itembagvec.clear();
			if(cfg_script.Push("JXJItemStoreBagVec"))
			{
				int nBagVec = 1;
				ref_ptr<crStoreBagItemData> bag;
				int itemid,itemcount;
				while(cfg_script.Get("Param", v_str,nBagVec++))
				{
					itemid = atoi(v_str[0].c_str());
					itemcount = atoi(v_str[1].c_str());
					if(itemid != 0 && itemcount>0 /*&& nBagVec-2<m_itembagnum*/)
					{
						if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
						{
							itemtype = atoi(itemrecord[itemtypeindex].c_str());
							bag = new crStoreBagItemData;
							bag->setItemID(itemid);
							bag->setItemCount(itemcount);
							if(itemtype == IT_Equip && v_str.size()>=3)
								bag->setEquipMagic(atoi(v_str[2].c_str()));//装备附魔
							bag->setItemServerID(atoi(v_str[3].c_str()));
							bag->setItemHasCycle(atoi(v_str[4].c_str()));

							m_itembagvec.push_back(bag);
						}
					}
				}
				cfg_script.Pop();
			}
			//将卡背包列表
			//m_jiangkabagvec.resize(m_jiangkabagnum);
			m_jiangkabagvec.clear();
			if(cfg_script.Push("JXJJiangkaItemBagVec"))
			{
				int nJkBagVec = 1;
				ref_ptr<crStoreBagItemData> jkbag;
				int itemid,itemcount;
				while(cfg_script.Get("Param", v_str,nJkBagVec++))
				{
					itemid = atoi(v_str[0].c_str());
					itemcount = atoi(v_str[1].c_str());
					if(itemid != 0 && itemcount>0 /*&& nJkBagVec-2<m_jiangkabagnum*/)
					{
						if (itemtab->queryOneRecord(0,v_str[0],itemrecord)>=0)
						{
							jkbag = new crStoreBagItemData;
							jkbag->setItemID(itemid);
							jkbag->setItemCount(itemcount);
							jkbag->setItemServerID(atoi(v_str[2].c_str()));
							jkbag->setItemHasCycle(atoi(v_str[3].c_str()));
							m_jiangkabagvec.push_back(jkbag);
						}
					}
				}
				cfg_script.Pop();
			}

			cfg_script.Close();
			DeleteFileA(file);
			//m_dataStream = NULL;
		}
		break;
	case WCHDATA_PlayerStream:
		if(param)
		{
			ref_ptr<crStreamBuf>dataStream =(crStreamBuf*)param;
			m_money = dataStream->_readInt();
			m_playerUsedMoney = dataStream->_readInt();
			m_cyclecount = dataStream->_readUShort();
			//背包格子数
			unsigned char itembagnum = dataStream->_readUChar();
			//背包列表
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int itemtypeindex= itemtab->getTitleIndex("类型");
			int itemtype;
			crTableIO::StrVec itemrecord;
			int itemid;
			m_itembagvec.resize(itembagnum);
			ref_ptr<crStoreBagItemData> itemdata;
			unsigned char bagitemcount;
			unsigned char i;
			for(i = 0; i<itembagnum; i++)
			{
				bagitemcount = dataStream->_readUChar();
				if(bagitemcount>0)
				{
					itemid = dataStream->_readInt();
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
					{
						itemtype = atoi(itemrecord[itemtypeindex].c_str());
						itemdata = new crStoreBagItemData;
						itemdata->setItemID(itemid);
						itemdata->setItemCount(bagitemcount);
						itemdata->setEquipMagic(dataStream->_readInt());//装备附魔
						itemdata->setItemServerID(dataStream->_readInt());
						itemdata->setItemHasCycle(dataStream->_readUChar());
						m_itembagvec[i] = itemdata;
					}
				}
			}
			////将卡背包
			unsigned char jiangkabagnum = dataStream->_readUChar();
			m_jiangkabagvec.resize(jiangkabagnum);
			ref_ptr<crStoreBagItemData> jkitemdata;
			for(i = 0; i<jiangkabagnum; i++)
			{
				bagitemcount = dataStream->_readUChar();
				if(bagitemcount>0)
				{
					jkitemdata = new crStoreBagItemData;
					jkitemdata->setItemID(dataStream->_readInt());
					jkitemdata->setItemCount(bagitemcount);
					jkitemdata->setItemServerID(dataStream->_readInt());
					jkitemdata->setItemHasCycle(dataStream->_readUChar());
					m_jiangkabagvec[i] = jkitemdata;
				}
			}
		}
		break;
	}
}

void crJXJPlayerStoreData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_Money:
		param = &m_money;
		break;
	case WCHDATA_JXJPlayerTotalAddMoney:
		param = &m_playerUsedMoney;
		break;
// 	case WCHDATA_JXJItemBagNum:
// 		param = &m_itembagnum;
// 		break;
// 	case WCHDATA_JXJJiangkaBagNum:
// 		param = &m_jiangkabagnum;
// 		break;
	case WCHDATA_JXJItemBagVec:
		param = &m_itembagvec;
		break;
	case WCHDATA_JXJJiangkaBagVec:
		param = &m_jiangkabagvec;
		break;
	default:
		param = NULL;
		break;
	}
}

JXJ::ChengChiEarnings::ChengChiEarnings(int groupid):
	m_cantonGroupId(groupid)
{
	m_earnings = 0;
	m_chengChiId = 0;
}

JXJ::Canton::Canton()
{
	m_consortiaID = 0;
	m_cantonId = 0;
	m_chengChiEarningsMap.clear();
}

short JXJ::Canton::reCalculateEarnings()
{
	if (! m_chengChiEarningsMap.empty() )
	{
		short totalEarngins = 1000;
		ChengChiEarningsMap ::iterator it = m_chengChiEarningsMap.begin();
		for (; it != m_chengChiEarningsMap.end(); ++ it)
		{
			if (it->second.valid() )
			{
				if (it->first != m_consortiaID)
				{
					totalEarngins -= it->second->getEarnings();
				}
			}
		}
		if (totalEarngins < 0)
		{
			totalEarngins = 0;
		}
		return totalEarngins;
	}
	return 1000;
}

void JXJ::crJXJConsortiaData::dayZero()
{
	bool willZero = false;
	unsigned char hour = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTimeOfGetChengChiEarnings	).c_str());

	time_t nowTime = time(NULL);
	struct tm sTm = *localtime(&nowTime);
	if (nowTime - m_dayLastUpdate >= 86400)
	{
		// 1
		willZero = true;
	}
	else if (sTm.tm_hour == hour && sTm.tm_min == 0 && nowTime - m_dayLastUpdate > 60)
	{
		// 1分钟内只执行一次
		willZero = true;
	}

	if (willZero)
	{
		m_dayLastUpdate = nowTime;

		m_applyvec.clear();
		/*m_totalActiveValY = 0;*/
		CRNet::crNetConductor *gameServer = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer); 	
		CRNet::crNetDataManager *netDataManager = gameServer->getNetDataManager();
		ref_ptr<crGameServerPlayerData> playerData;
		int groupid = 0;
		int playerid = 0;
		crPlayerGameData *myPlayer = NULL;
		crData *data = NULL;
		void *param;
		ConsortiaMemberMap::iterator it = m_consortiaMemberMap.begin();
		for (; it != m_consortiaMemberMap.end(); ++ it)
		{
			if (it->second.valid())
			{
				it->second->dayZero();
				playerid = it->first;
				playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
				if (playerData.valid() && playerData->getPlayerGameData())
				{
					myPlayer = playerData->getPlayerGameData();
					if (myPlayer->getDataClass())
					{
						data = myPlayer->getDataClass();
						data-> getParam(WCHDATA_JXJConsortiaID,param);
						groupid = *( int *)param;
						myPlayer->doEvent(WCH_JXJRefreshGroupCantonSpecialty,MAKEINT64(&groupid,&playerid));
						myPlayer->doEvent(WCH_JXJRecvUpdateCantonSpecialty,MAKEINT64(NULL,GameServer));
					}
				}
					
				/*m_totalActiveValY += it->second->m_yesterdayActiveVal;*/
			}
		}
		settleEarnings();
		unsigned char maxtaofadian = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxTaoFaDian).c_str());
		if (m_conquest < maxtaofadian)
		{
			m_conquest += 1;
		}
		m_modify = true;
	}

	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
	gsBrainData->inputParam(WCHDATA_JXJGroupDayLastUpdate,&nowTime);
}

void JXJ::crJXJConsortiaData::weekZero()
{
	time_t nowTime = time(NULL);
	struct  tm sTm = * localtime(&nowTime);
	bool willZero = false;
	if (nowTime - m_weekLastUpdate >= 86400 * 7)
	{
		willZero = true;
	}
	else if (sTm.tm_wday == 1 && sTm.tm_hour == 0 && sTm.tm_min == 0 && nowTime - m_weekLastUpdate > 60)
	{
		// 周一凌晨
		willZero = true;
	}

	if (willZero)
	{
		m_weekLastUpdate = nowTime;

		//ConsortiaMemberMap::iterator it = m_consortiaMemberMap.begin();
		//for (; it != m_consortiaMemberMap.end(); ++ it)
		//{
		//	if (it->second.valid())
		//	{
		//		it->second->weekZero();
		//		// 首先清空上周的副军团长
		//		if (it->second->getGroupPos() == JXJGroup_SecondLeader)
		//		{
		//			it->second->setGroupPos(JXJGroup_Normal);
		//		}
		//	}
		//}

		//m_groupLeaderVec.clear();
		//int playerId = 0;
		//unsigned char maxSecLeader = 2;
		//ConsortiaActiveRank::RankSet & rankSet = m_activeRank.getRankSet();
		//ConsortiaActiveRank::RankSet::iterator rsItr = rankSet.begin(); 
		//for (; rsItr != rankSet.end() && maxSecLeader; ++ rsItr)
		//{
		//	if (rsItr->valid())
		//	{
		//		playerId = (*rsItr)->m_id;
		//		it = m_consortiaMemberMap.find(playerId);
		//		if (it != m_consortiaMemberMap.end() && it->second->getGroupPos() != JXJGroup_FirstLeader)
		//		{
		//			it->second->setGroupPos(JXJGroup_SecondLeader);
		//			-- maxSecLeader;
		//		}
		//	}
		//}
		/*m_activeRank.clear();*/
		/*m_startedFuBenSet.clear();*/
		/*m_startedFuBenMap.clear();*/
		m_modify = true;
	}
	// 备份到serverData
	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
	gsBrainData->inputParam(WCHDATA_JXJGroupWeekLastUpdate,&nowTime);
}

void JXJ::crJXJConsortiaData::twoSecZero()
{
	if (m_dismissTime > 0)
	{
		m_dismissTime -= 2;
		if (m_dismissTime < 0)
			m_dismissTime = 0;
	}
}

JXJ::JXJGroupPos JXJ::crJXJConsortiaData::getGroupPos( int playerid )
{
	GroupLeaderVec::iterator glvItr = m_groupLeaderVec.begin(); 
	int idx = 0;
	for (; glvItr != m_groupLeaderVec.end(); ++ glvItr)
	{
		if (playerid == *glvItr)  break;
		++ idx;
	}
	if (glvItr != m_groupLeaderVec.end())
	{
		if (idx == 0) return JXJGroup_FirstLeader;
		else if (idx <= 2) return JXJGroup_SecondLeader;
	}
	return JXJGroup_Normal;
}

void JXJ::crJXJConsortiaData::clearRankCache()
{
	m_memberRank.clear();
}

void JXJ::crJXJConsortiaData::settleEarnings()
{
	//ref_ptr<crTableIO> chengchiTab;
	//crTableIO::StrVec record;
	//int cantonIdx ;
	//int cantonId;

	//FenFengedCantonMap groupCantonMap;
	//if ( !(m_fenFengedCantonMapT.empty()) )
	//{
	//	FenFengedCantonMap::iterator gcsItr = m_fenFengedCantonMapT.begin();
	//	for (; gcsItr != m_fenFengedCantonMapT.end(); ++ gcsItr)
	//	{
	//		groupCantonMap.insert(*gcsItr);
	//	}
	//	m_yesterdayIsCantonGroup = true;
	//}
	//else if ( !(m_applyCantonMapT.empty()) )
	//{
	//	ApplyCantonMap ::iterator gacsItr = m_applyCantonMapT.begin();
	//	for (; gacsItr != m_applyCantonMapT.end(); ++ gacsItr)
	//	{
	//		groupCantonMap.insert(*gacsItr);
	//	}
	//	m_yesterdayIsCantonGroup = false;
	//}
	//else m_yesterdayIsCantonGroup = false;
	//do 
	//{
	//	if (groupCantonMap.empty()) break;
	//	void * param;
	//	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();

	//	CRCore::ScopedLock<crData> lock(*gsBrainData);

	//	chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	//	if (!chengchiTab.valid()) break;
	//	int earningsTypeIdx = chengchiTab->getTitleIndex("收益类型");
	//	int earningsNumIdx = chengchiTab->getTitleIndex("收益数量");
	//	int techanTypeIdx = chengchiTab->getTitleIndex("特产类型");
	//	int techanNumIdx = chengchiTab->getTitleIndex("特产数量");
	//	cantonIdx = chengchiTab->getTitleIndex("州ID");

	//	m_totalChengChiNum = 0;
	//	gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
	//	CRCore::ref_ptr<GroupEarnings> earnings ;
	//	ChengchiMap *chengchiMap = (ChengchiMap *)param;
	//	ChengchiMap::iterator ccItr = chengchiMap->begin();
	//	/*m_earningsVec.clear();*/
	//	FenFengedCantonMap::iterator gcsItr;
	//	for (; ccItr != chengchiMap->end(); ++ ccItr)
	//	{
	//		if (m_shiliid == ccItr->second->getChengzhuShili())
	//		{
	//			cantonId = 0;
	//			if (chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(ccItr->first),record)>=0)
	//			{
	//				cantonId = atoi(record[cantonIdx].c_str());
	//			}
	//			else continue;
	//			gcsItr = groupCantonMap.find(cantonId);
	//			if (gcsItr != groupCantonMap.end())
	//			{
	//				earnings = new GroupEarnings;
	//				earnings->m_earningsScale = gcsItr->second;
	//				earnings->m_type = atoi(record[earningsTypeIdx].c_str());
	//				earnings->m_num = atoi(record[earningsNumIdx].c_str());
	//				m_earningsVec.push_back(earnings);
	//				earnings = new GroupEarnings;
	//				earnings->m_earningsScale = gcsItr->second;
	//				earnings->m_type = atoi(record[techanTypeIdx].c_str());
	//				earnings->m_num = atoi(record[techanNumIdx].c_str());
	//				m_earningsVec.push_back(earnings);
	//			}
	//		}
	//	}
	//} while (0);
	void *param;
	crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
	CRCore::ScopedLock<crData> lock(*gsBrainData);
	gsBrainData->getParam(WCHDATA_JXJChengChiMap,param);
	ChengchiMap *chengchiMap = (ChengchiMap *)param;
	ChengchiMap::iterator chItr;
	ref_ptr<crTableIO> chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	if(!chengchitab.valid()) return;
	crTableIO::StrVec record;
	crTableIO::DataVec chengchiVec;
	int chengweiIdx = chengchitab->getTitleIndex("称谓");
	int idIdx = chengchitab->getTitleIndex("id");
	int earningsTypeIdx = chengchitab->getTitleIndex("收益类型");
	int earningsNumIdx = chengchitab->getTitleIndex("收益数量");
	int techanTypeIdx = chengchitab->getTitleIndex("特产类型");
	int techanNumIdx = chengchitab->getTitleIndex("特产数量");
	int cantonidIdx = chengchitab->getTitleIndex("州ID");
	GroupChengChiEarningMap::iterator gccmitr;
	FenFengedCantonMap::iterator ffcmItr;
	FenFengedChengChiMap::iterator ffccmItr;
	ffcmItr = m_fenFengedCantonMapT.begin();
	int cantonId = 0;
	if (ffcmItr != m_fenFengedCantonMapT.end())
	{
		cantonId = ffcmItr->first;
	}
	if(cantonId <= 0) return;
	int earningsnum = 0;
	int earningstype = 0;
	int techannum = 0;
	int techantype = 0;
	int chengchiNum = 0;
	for (ConsortiaMemberMap::iterator cmmitr = m_consortiaMemberMap.begin();cmmitr != m_consortiaMemberMap.end();++cmmitr)
	{
		GroupChengChiEarningMap &groupchengchiearningMap = cmmitr->second->getGroupChengChiEarningMap();
		groupchengchiearningMap.clear();
		if(cmmitr->second->getGroupPos() == JXJGroup_FirstLeader)
		{
			chengchitab->queryRecords(cantonidIdx,crArgumentParser::appItoa(cantonId),chengchiVec);
			if(chengchiVec.empty()) continue;
			for (crTableIO::DataVec::iterator ccvitr = chengchiVec.begin();ccvitr != chengchiVec.end(); ++ccvitr)
			{
				chItr = chengchiMap->find(atoi((*ccvitr)[idIdx].c_str()));
				if (chItr != chengchiMap->end())
				{
					if (m_shiliid == chItr->second->getChengzhuShili())
					{
						earningsnum = 0;
						earningstype = 0;
						earningsnum = atoi((*ccvitr)[earningsNumIdx].c_str());
						earningstype = atoi((*ccvitr)[earningsTypeIdx].c_str());
						if (earningsnum >0 && earningstype > 0)
						{
							gccmitr = groupchengchiearningMap.find(earningstype);
							if (gccmitr == groupchengchiearningMap.end())
							{
								groupchengchiearningMap.insert(std::make_pair(earningstype,earningsnum));
							}
							else
							{
								groupchengchiearningMap[earningstype] = earningsnum + gccmitr->second;
							}
						}
						techannum = 0;
						techantype = 0;
						techannum = atoi((*ccvitr)[techanNumIdx].c_str());
						techantype = atoi((*ccvitr)[techanTypeIdx].c_str());
						gccmitr = groupchengchiearningMap.find(techantype);
						if (techantype > 0 && techannum > 0)
						{
							if (gccmitr == groupchengchiearningMap.end())
							{
								groupchengchiearningMap.insert(std::make_pair(techantype,techannum));
							}
							else
							{
								groupchengchiearningMap[techantype] = techannum >= (gccmitr->second) ? techannum:(gccmitr->second);
							}
						}
						++chengchiNum;
					}
				}
			}
			for (GroupChengChiEarningMap::iterator gccemitr = groupchengchiearningMap.begin(); gccemitr != groupchengchiearningMap.end(); ++gccemitr)
			{
				if (chengchiNum > 0 && gccemitr->first == T_Copper)
				{
					gccemitr->second = gccemitr->second / chengchiNum * 2;
				}
			}
		}
		else
		{
			bool isfenfenged =false;
			for (ffccmItr = m_fenFengedChengChiMap.begin();ffccmItr != m_fenFengedChengChiMap.end();++ffccmItr)
			{
				if(ffccmItr->second == cmmitr->first)
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
						gccmitr = groupchengchiearningMap.find(earningstype);
						if (gccmitr == groupchengchiearningMap.end())
						{
							groupchengchiearningMap.insert(std::make_pair(earningstype,earningsnum));
						}
						else
						{
							groupchengchiearningMap[earningstype] = earningsnum;
						}
					}
					if (techantype > 0 && techannum >0)
					{
						gccmitr = groupchengchiearningMap.find(techantype);
						if (gccmitr == groupchengchiearningMap.end())
						{
							groupchengchiearningMap.insert(std::make_pair(techantype,techannum));
						}
						else
						{
							groupchengchiearningMap[techantype] = techannum;
						}
					}
				}
			}
		}
	}

	m_fenFengedCantonMapY.clear();
	ffcmItr = m_fenFengedCantonMapT.begin();
	for (;ffcmItr != m_fenFengedCantonMapT.end(); ++ ffcmItr)
	{
		m_fenFengedCantonMapY.insert(*ffcmItr);
	}
	m_applyCantonMapY.clear();
	ApplyCantonMap::iterator acmItr = m_applyCantonMapT.begin();
	for (; acmItr != m_applyCantonMapT.end(); ++ acmItr)
	{
		m_applyCantonMapY.insert(*acmItr);
	}
	m_modify = true;
}

void  JXJ::crJXJConsortiaData::split( std::string str,std::string pattern ,std::vector<std::string> & resultVec)
{
    std::string::size_type pos;
    str+=pattern;
    int size=str.size();
	resultVec.clear();
	resultVec.reserve( (size + 1) / 2);
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            resultVec.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
}

void JXJ::crJXJConsortiaMember::dayZero()
{
	//m_curWeekActiveVal += m_todayActiveVal;
	//m_yesterdayActiveVal = m_todayActiveVal;
	//m_todayActiveVal = 0;
	m_getEarningsYesterday = false;
	m_newer = false;
	m_troopstorednumT = 0;
	m_trooptakednumT = 0;
	clearGroupcantonspecialtyVec();
}

void JXJ::crJXJConsortiaMember::weekZero()
{
	m_lastWeekActiveVal = m_curWeekActiveVal;
	m_curWeekActiveVal = 0;
}

JXJ::JXJCountryOfficial JXJ::crJXJShiliData::getOfficialDes( int id )
{
	if (id == m_chengxiang) return JXJCountryOfficial_first;
	if (id == m_dasima) return JXJCountryOfficial_second;
	if (id == m_dajiangjun) return JXJCountryOfficial_third;
	return JXJCountryOfficial_normal;
}

std::string  JXJ::crJXJShiliData::getOfficialStr( int id)
{
	if (id == m_chengxiang) return std::string("丞相");
	if (id == m_dasima) return std::string("大司马");
	if (id == m_dajiangjun) return std::string("大将军");
	return std::string("");
}

CRCore::crHandle * JXJ::CountryHistoryInfoBase::getNoticeHandle()
{
	CRCore::crHandle *noticeHandle = CRNetApp::crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
	if (noticeHandle)
	{
		int mode = 2;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&m_infoId);
	}
	return noticeHandle;
}

unsigned char JXJ::CountryHistoryInfoBase::getSaveStream( std::vector<std::string> & strVec )
{
	m_infoType = (unsigned char) atoi(strVec[0].c_str());
	m_infoId = atoi(strVec[1].c_str());
	m_time = crArgumentParser::appAtoI64(strVec[2].c_str() );
	return 3;
}

unsigned char JXJ::CountryHistoryInfoBase::buildSaveStream( std::vector<std::string> & strVec )
{
	strVec.resize(3);
	strVec[0] = crArgumentParser::appItoa(m_infoType);
	strVec[1] = crArgumentParser::appItoa(m_infoId);
	strVec[2] = crArgumentParser::appI64toa(m_time);
	return 3;
}

void JXJ::CountryHistoryInfoBase::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	stream->_writeUChar(m_infoType);
	stream->_writeInt(m_infoId);
	stream->_writeInt64(m_time);
}

void JXJ::CountryHistoryInfoBase::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	//m_infoType = stream->_readUChar();
	m_infoId = stream->_readInt();
	m_time = stream->_readInt64();
}

void JXJ::CountryHistoryInfoBase::setInfoId( int id )
{
	m_infoId = id;
}

void JXJ::CountryHistoryInfoBase::setTime( time_t time )
{
	m_time = time;
}

unsigned char JXJ::CountryHistoryInfoBase::getInfoType()
{
	return m_infoType;
}

JXJ::CountryHistoryInfoBase::CountryHistoryInfoBase()
{
	m_infoType = 0,m_infoId= 0;
	m_time = time(0);
}

unsigned char JXJ::CountryHistoryOfficalChange::getSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::getSaveStream(strVec);
	m_playerName = strVec[size];
	m_officalName = strVec[size + 1];
	return strVec.size();
}

unsigned char JXJ::CountryHistoryOfficalChange::buildSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::buildSaveStream(strVec);
	strVec.resize(size + 2);
	strVec[size] = m_playerName;
	strVec[size + 1] = m_officalName;
	return strVec.size();
}

void JXJ::CountryHistoryOfficalChange::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_playerName);
	stream->_writeString(m_officalName);
}

void JXJ::CountryHistoryOfficalChange::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_playerName = stream->_readString();
	m_officalName = stream->_readString();
}

void JXJ::CountryHistoryOfficalChange::getCompleteString( std::string & outString )
{

	ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	crTableIO::StrVec textrecord;
	char szNotice[256];
	memset(szNotice,0,sizeof(szNotice));

	if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(m_infoId),textrecord) >= 0)
	{
		char timeStr[64]; 
		memset(timeStr,0,sizeof(timeStr));
		strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S \0",localtime(&m_time) ); 
		sprintf_s(szNotice,textrecord[1].c_str(), timeStr, m_playerName.c_str(),m_officalName.c_str());
		outString = szNotice;
	}
	else outString = "";
}

JXJ::CountryHistoryOfficalChange::CountryHistoryOfficalChange()
{
	m_infoType = JXJCountryHistoryType_Offical;
	m_infoId = 3068;
}

unsigned char JXJ::CountryHistoryBenefitChange::getSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::getSaveStream(strVec);
	m_playerName = strVec[size];
	m_officalName = strVec[size + 1];
	return strVec.size();
}

unsigned char JXJ::CountryHistoryBenefitChange::buildSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::buildSaveStream(strVec);
	strVec.resize(size + 2);
	strVec[size] = m_playerName;
	strVec[size + 1] = m_officalName;
	return strVec.size();
}

void JXJ::CountryHistoryBenefitChange::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeString(m_playerName);
	stream->_writeString(m_officalName);
}

void JXJ::CountryHistoryBenefitChange::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_playerName = stream->_readString();
	m_officalName = stream->_readString();
}

void JXJ::CountryHistoryBenefitChange::getCompleteString( std::string & outString )
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S ",localtime(&m_time) ); 
		std::string timeStr = tmp;
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&timeStr);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&m_officalName);
		noticeHandle->inputParam(WCHDATA_NoticeParam3,&m_playerName);
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";
}

JXJ::CountryHistoryBenefitChange::CountryHistoryBenefitChange()
{
	m_infoType = JXJCountryHistoryType_Benefit;
}


JXJ::CountryHistoryChengChiChange::CountryHistoryChengChiChange()
{
	m_infoType = JXJCountryHistoryType_Chengchi;
}

void JXJ::CountryHistoryChengChiChange::getCompleteString( std::string & outString )
{
	CRCore::crHandle * noticeHandle = getNoticeHandle();
	if (noticeHandle)
	{
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S ",localtime(&m_time) ); 
		std::string timeStr = tmp;
		
		std::string shiliName[] = {"魏国","蜀国","吴国"};

		ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crTableIO::StrVec record;
		int nameIdx = 0;
		std::string chengchiName;
		if (chengchiTab.valid())
		{
			nameIdx = chengchiTab->getTitleIndex("名字");
			crTableIO::StrVec record;
			if (nameIdx >= 0 
				&& chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(m_chengchiId),record) >= 0)
			{
				chengchiName  = record[nameIdx];
			}
		}
		std::string shili;
		if (m_shiliid >= c_startShiliID)
		{
			shili = shiliName[m_shiliid - c_startShiliID];
		}

		noticeHandle->inputParam(WCHDATA_NoticeParam1,&timeStr);
		noticeHandle->inputParam(WCHDATA_NoticeParam2,&chengchiName);
		if (m_winOrlose)
		{
			noticeHandle->inputParam(WCHDATA_NoticeParam3,&chengchiName);
			noticeHandle->inputParam(WCHDATA_NoticeParam4,&shili);
		}
		else
		{
			noticeHandle->inputParam(WCHDATA_NoticeParam3,&shili);
		}
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&outString);
		CRNetApp::crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	}
	else outString = "";
}

void JXJ::CountryHistoryChengChiChange::readContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::readContent(stream);
	m_winOrlose = stream->_readBool();
	m_chengchiId = stream->_readInt();
	m_shiliid = stream->_readInt();
}

void JXJ::CountryHistoryChengChiChange::writeContent( CRCore::ref_ptr<CRCore::crStreamBuf> & stream )
{
	Base::writeContent(stream);
	stream->_writeBool(m_winOrlose);
	stream->_writeInt(m_chengchiId);
	stream->_writeInt(m_shiliid);
}

unsigned char JXJ::CountryHistoryChengChiChange::buildSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::buildSaveStream(strVec);
	strVec.resize(size + 3);
	strVec[size] = crArgumentParser::appItoa(m_chengchiId);
	strVec[size + 1] = crArgumentParser::appItoa(m_shiliid);
	strVec[size + 2] = crArgumentParser::appItoa(m_winOrlose);

	return strVec.size();
}

unsigned char JXJ::CountryHistoryChengChiChange::getSaveStream( std::vector<std::string> & strVec )
{
	unsigned char size = Base::getSaveStream(strVec);
	m_chengchiId = atoi(strVec[size].c_str());
	m_shiliid = atoi(strVec[size + 1].c_str());
	m_winOrlose = (bool)atoi(strVec[size + 2].c_str());
	return strVec.size();
}

void JXJ::PeopleInfo::needUpdate()
{
	if(!crGlobalHandle::isClient())
	{
		crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
		serverData->excHandle(MAKEINT64(WCH_LockData,1));
		if (m_shili >= c_startShiliID)
		{
			void *param;
			serverData->getParam(WCHDATA_JXJShiliWei+m_shili-c_startShiliID,param);
			crJXJShili *shili = (crJXJShili *)param;

			crData * shiliData = shili->getDataClass();
			if (shiliData) 
			{
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
				ModifyPlayerSet * modifyPlayerSet = (ModifyPlayerSet *)param;
				shiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
				InsertPlayerSet * insertPlayerSet = (InsertPlayerSet *)param;
				if (insertPlayerSet && insertPlayerSet->find(m_id) == insertPlayerSet->end())
				{
					if (modifyPlayerSet && modifyPlayerSet->find(m_id) == modifyPlayerSet->end())
					{
						modifyPlayerSet->insert(m_id);
					}
				}

				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		serverData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

void JXJ::PeopleInfo::needInsert()
{
	if(!crGlobalHandle::isClient())
	{
		crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
		serverData->excHandle(MAKEINT64(WCH_LockData,1));
		if (m_shili >= c_startShiliID)
		{
			void *param;
			serverData->getParam(WCHDATA_JXJShiliWei+m_shili-c_startShiliID,param);
			crJXJShili *shili = (crJXJShili *)param;

			crData * shiliData = shili->getDataClass();
			if (shiliData) 
			{
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJModifyPlayerSet,param);
				ModifyPlayerSet * modifyPlayerSet = (ModifyPlayerSet *)param;
				shiliData->getParam(WCHDATA_JXJInsertPlayerSet,param);
				InsertPlayerSet * insertPlayerSet = (InsertPlayerSet *)param;
				if (modifyPlayerSet && modifyPlayerSet->find(m_id) != modifyPlayerSet->end())
				{
					modifyPlayerSet->erase(m_id);
				}
				if (insertPlayerSet && insertPlayerSet->find(m_id) == insertPlayerSet->end())
				{
					insertPlayerSet->insert(m_id);
				}
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		serverData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJExitSceneServerUIData
//
/////////////////////////////////////////
crJXJExitSceneServerUIData::crJXJExitSceneServerUIData():
	m_codetext(1096),//战场尚未结束，是否确定退出？\n（%d秒后自动退出）
	m_exittimer(60)
{
}
crJXJExitSceneServerUIData::crJXJExitSceneServerUIData(const crJXJExitSceneServerUIData& data):
	crUIData(data),
	m_codetext(data.m_codetext),
	m_exittimer(data.m_exittimer)
{
}
void crJXJExitSceneServerUIData::addParam(int i, const std::string& str)
{
}
void crJXJExitSceneServerUIData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_JXJExitSceneCodeText:
		m_codetext = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJExitSceneTimer:
		m_exittimer = param?*((short*)param):NULL;
		break;
	}
}
void crJXJExitSceneServerUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJExitSceneCodeText:
		param = &m_codetext;
		break;
	case WCHDATA_JXJExitSceneTimer:
		param = &m_exittimer;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJTipsUIData
//
//////////////////////////////////////////////////////////////////////////
crJXJTipsUIData::crJXJTipsUIData():
	m_id(0),
	m_count(0)
{

}

crJXJTipsUIData::crJXJTipsUIData(const crJXJTipsUIData& data):
	m_id(data.m_id),
	m_count(data.m_count)
{

}
void crJXJTipsUIData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		m_id = param?*((int*)param):NULL;
		break;
	case 1:
		m_count = param?*((int*)param):NULL;
		break;
	}
}

void crJXJTipsUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case 0:
		param = &m_id;
		break;
	case 1:
		param = &m_count;
		break;
	default:
		crUIData::getParam(i,param);
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleRankingUIData
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleRankingUIData::crJXJBattleRankingUIData():
	m_mypos(-1),
	m_killtotal(0),
	m_deadtotal(0),
	m_myJifen(0),
	m_myKill(0),
	m_myDead(0),
	m_myExploit(0),
	m_myLingqu(false),
	m_battlerankingVer(-1)
{
}

crJXJBattleRankingUIData::crJXJBattleRankingUIData(const crJXJBattleRankingUIData& data):
	m_mypos(data.m_mypos),
	m_killtotal(data.m_killtotal),
	m_deadtotal(data.m_deadtotal),
	m_battleRankingMap(data.m_battleRankingMap),
	m_myJifen(data.m_myJifen),
	m_myKill(data.m_myKill),
	m_myDead(data.m_myDead),
	m_myExploit(data.m_myExploit),
	m_myLingqu(data.m_myLingqu),
	m_battlerankingVer(-1)
{
}
void crJXJBattleRankingUIData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case WCHDATA_JXJUIBattleRankingMyPos:
		m_mypos = param?*((short*)param):NULL;
		break;
	case WCHDATA_JXJUIBattleRankingKill:
		m_killtotal = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJUIBattleRankingDead:
		m_deadtotal = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRankingMyJifen:
		m_myJifen = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRankingMyKill:
		m_myKill = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRankingMyDead:
		m_myDead = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRankingMyExploit:
		m_myExploit = param?*((int*)param):NULL;
		break;
	case WCHDATA_JXJBattleRankingMyLingqu:
		m_myLingqu = param?*((bool*)param):NULL;
		break;
	case WCHDATA_JXJUIBattleRankingVer:
		m_battlerankingVer = param?*((unsigned char*)param):NULL;
		break;
	}
}

void crJXJBattleRankingUIData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case WCHDATA_JXJUIBattleRankingMap:
		param = &m_battleRankingMap;
		break;
	case WCHDATA_JXJUIBattleRankingMyPos:
		param = &m_mypos;
		break;
	case WCHDATA_JXJUIBattleRankingKill:
		param = &m_killtotal;
		break;
	case WCHDATA_JXJUIBattleRankingDead:
		param = &m_deadtotal;
		break;
	case WCHDATA_JXJBattleRankingMyJifen:
		param = &m_myJifen;
		break;
	case WCHDATA_JXJBattleRankingMyKill:
		param = &m_myKill;
		break;
	case WCHDATA_JXJBattleRankingMyDead:
		param = &m_myDead;
		break;
	case WCHDATA_JXJBattleRankingMyExploit:
		param = &m_myExploit;
		break;
	case WCHDATA_JXJBattleRankingMyLingqu:
		param = &m_myLingqu;
		break;
	case WCHDATA_JXJUIBattleRankingVer:
		param = &m_battlerankingVer;
		break;
	}
}
void crJXJBattleRankingUIData::excHandle(_crInt64 msg)
{
	if(LOINT64(msg) == WCH_LockData)
	{
		if(HIINT64(msg))
			m_dataMutex.acquire();
		else
			m_dataMutex.release();
	}
	else
	{
		crData::excHandle(getHandle(msg)); 
	}
}