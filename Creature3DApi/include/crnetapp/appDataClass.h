/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRNETAPP_CRDATACLASS_H
#define CRNETAPP_CRDATACLASS_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataParam.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRGUI/crImageStage.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRNetApp/appNetChatData.h>
#include <CREncapsulation/crTableIO.h>
#include <CRCore/crArgumentParser.h>
#include <CRCore/crWidgetNode.h>
#include <gnelib.h>
#include <vector>
#include <list>
#include <string>
#include <map>
namespace CRNetApp{
class crInstanceItem;
class crGameTask;
class crWaypoint;
struct ItemUseParam;
typedef std::map< int,CRCore::ref_ptr<CREncapsulation::crTableIO> >TableIOMap;
typedef std::map< int,std::string >TableFileMap;
typedef std::map< int, unsigned char > GMMap;//playerid,权限
typedef std::map< int, float > WordValueMap;//字典表
class CRNETAPP_EXPORT crGlobalData : public CRCore::crData
{
public:
	crGlobalData();
	crGlobalData(const crGlobalData& data);
	DataClass(CRNetApp, Global)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);

	inline std::string& gClientCfg() { return g_clientfile; }//服务器启动日期
	inline std::string& gStartDate() { return g_startDate; }//服务器启动日期
	inline float gUnitScale() { return g_unitScale; }
	inline unsigned char gMaxLapover() { return g_maxLapover; }
	inline float gSalerate() { return g_salerate; }
	inline float gRemoveOutRangeTime() { return g_removeOutRangeTime; }
	inline float gRecycleIDTime() { return g_recycleIDTime; }
	inline float gRecycleStreamIDTime_in() { return g_recycleStreamIDTime_in; }
	inline float gRecycleStreamIDTime_out() { return g_recycleStreamIDTime_out; }
	inline unsigned char gMaxgroupmember() { return g_maxgroupmember; }
	inline unsigned char gMaxSkillLevelLearn() { return g_maxSkillLevelLearn; }
	inline float gMinFollowDistance() { return g_minFollowDistance; }
	inline float gMinItemUseRange() { return g_minItemUseRange; }
	inline unsigned char gItemVisiableDelay() { return g_itemVisiableDelay; }
	inline int gMaxRecBufSize() { return g_maxRecBufSize; }
	inline std::string gGameChatRecFile() { return g_gameChatRecFile; }
	inline std::string gChatMsgRecFile() { return g_chatMsgRecFile; }
	inline int gNumGameChatShow() { return g_numGameChatShow; }
	inline int gNumGameChatClear() { return g_numGameChatClear; }
	inline unsigned short gMaxInRangePlayer() { return g_maxInRangePlayer; }
	inline unsigned short gMaxInRangeItem() { return g_maxInRangeItem; }
	inline float gStreamCacheMaxTime() { return g_streamCacheMaxTime; }
	inline unsigned char gMaxDisplayTaskTrace() { return g_maxDisplayTaskTrace; }
	inline float gRemoveItemChildTime() { return g_removeItemChildTime; }
	inline float gControllerTime() { return g_controllerTime; }
	inline float gLayerHeight() { return g_layerHeight; }
	inline float gWalkHeight() { return g_walkHeight; }
	inline unsigned char gMaxRoleCanJoin() { return g_maxRoleCanJoin; }
	inline short gFirstGameSceneID() { return g_firstGameSceneID; }
	inline int gGameID() { return g_gameid; }
	inline int gMainRoleAItemID() { return g_mainRoleAItemID; }
	inline int gMainRoleAMetierID() { return g_mainRoleAMetierID; }
	inline unsigned char gLoginConnectCount() { return g_loginConnectCount; }
	inline const std::string& gDynamicDownloadTabName() { return g_dynamicDownloadTabName; }
	inline CRCore::ref_ptr<CREncapsulation::crTableIO> gGameGlobalTable() { return g_gameGlobalTable.get(); }
	inline CRCore::ref_ptr<CREncapsulation::crTableIO> gNewDynamicDownloadTable() { return g_newDynamicDownloadTable.get(); }
	//inline CRCore::ref_ptr<CREncapsulation::crTableIO> gGMTable() { return g_gmTable.get(); }
	inline CRCore::ref_ptr<CREncapsulation::crTableIO> gGlobalTable(int id)
	{ TableIOMap::iterator itr = g_globalTables.find(id); return itr != g_globalTables.end()?itr->second.get():NULL; }
	inline std::string gGlobalTableFile(int id)
	{
		TableFileMap::iterator itr = g_globalTableFileMap.find(id); return itr != g_globalTableFileMap.end() ? itr->second : "";
	}
	inline std::string gGameGlobalValue(int id,int viplv = 0)
	{
		CREncapsulation::crTableIO::StrVec record;
		if(g_gameGlobalTable->queryOneRecord(0,CRCore::crArgumentParser::appItoa(id),record)>=0)
		{
			if(viplv+1<record.size())
			{
				if(record[viplv+1].empty())
					return record[1];
				else
					return record[viplv+1];
			}
		}
		return "NULL";
	}
	inline float gWorldValue(int id)
	{
		WordValueMap::iterator itr = g_wordValueMap.find(id);
		return itr != g_wordValueMap.end() ? itr->second : 0.0f;
	}
	void UpdateWorldValue(int id, float value);
	inline bool gShowDebugInfo() { return g_showDebugInfo; }
	inline bool gEnableRobotTest() { return g_enableRobotTest; }
	inline float gSightRange() { return g_sightRange; }
	inline short gEyePointRange() { return g_eyePointRange; }
	inline short gEyePointHeight() { return g_eyePointHeight; }
	inline float gItemAdjustAngular() { return g_itemAdjustAngular; }
	inline float gFollowDelta() { return g_followDelta; }
	inline float gPortScopeSqr() { return g_portScopeSqr; }
	inline float gItemVisiableInterval() { return g_itemVisiableInterval; }
	inline float gDynamicTryWait() { return g_dynamicTryWait; }
	//void reloadGMTable(const std::string &filename);
	void reloadGameGlobalTable(const std::string &filename);
	void reloadGlobalTable(int id);
	void reloadGlobalTable(int id,const std::string &filename);
	void lockGMMap();
	GMMap &getGMMap();
	short getGMAuthority(int playerid);//-1表示非GM
	void unlockGMMap();
protected:
	////制作场景的时候注意坐标0,0,0点为不可站立点。
	std::string g_clientfile;//client.cfg文件名
	std::string g_startDate;//服务器启动时间
	float g_unitScale;
	unsigned char g_maxLapover;
	float g_salerate;
	float g_removeOutRangeTime;//100s
	float g_recycleIDTime;//110s
	float g_recycleStreamIDTime_in;//服务器间(局域网内)回收时间
	float g_recycleStreamIDTime_out;//外网回收时间
	unsigned char g_maxgroupmember;
	unsigned char g_maxSkillLevelLearn;//可学到的最高技能等级
	float g_minFollowDistance;
	float g_minItemUseRange;//武器的最小攻击距离
	float g_followDelta;
	unsigned char g_itemVisiableDelay;//ms
	int g_maxRecBufSize;//0.5MB
	std::string g_gameChatRecFile;
	std::string g_chatMsgRecFile; 
	int g_numGameChatShow;
	int g_numGameChatClear;
	unsigned short g_maxInRangePlayer;//32;
	unsigned short g_maxInRangeItem;
	float g_streamCacheMaxTime;
	unsigned char g_maxDisplayTaskTrace;
	float g_removeItemChildTime;//20s
	float g_controllerTime;//0.5s
	float g_layerHeight;//2.5米
	float g_walkHeight;//0.8
	unsigned char  g_maxRoleCanJoin;//可以参战的最大角色数量
	short g_firstGameSceneID;
	int g_gameid;
	int g_mainRoleAItemID;
	int g_mainRoleAMetierID;
	unsigned char g_loginConnectCount;//登录接入服务器数量
	std::string g_dynamicDownloadTabName;
	CRCore::ref_ptr<CREncapsulation::crTableIO> g_gameGlobalTable;
	CRCore::ref_ptr<CREncapsulation::crTableIO> g_newDynamicDownloadTable;
	//CRCore::ref_ptr<CREncapsulation::crTableIO> g_gmTable;
	TableIOMap g_globalTables;
	TableFileMap g_globalTableFileMap;
	////////DebugInfo
	bool g_showDebugInfo;
	bool g_enableRobotTest;
	float g_sightRange;
	short g_eyePointRange;//
	short g_eyePointHeight;
	float g_itemAdjustAngular;
	float g_portScopeSqr;
	float g_itemVisiableInterval;
	float g_dynamicTryWait;//item动态碰撞寻路尝试等待时间
	CRCore::ref_ptr<CREncapsulation::crTableIO> g_wordValueTable;
	WordValueMap g_wordValueMap;//高速缓存
	GNE::Mutex m_gmMapMutex;
	GMMap m_gmMap;
};
class crRecvFileStream;
class CRNETAPP_EXPORT crGameClientData : public crGlobalData
{
public:
	crGameClientData();
	crGameClientData(const crGameClientData& data);
    DataClass(CRNetApp, GameClient)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);

	typedef std::map< CRCore::ref_ptr<CRGUI::crImageStage>,  CRCore::ref_ptr<crNetChatData::crChatFriend> > FriendChatMap;
	void insertFriendChatMap(CRGUI::crImageStage *stage, crNetChatData::crChatFriend *chatFriend);
	crNetChatData::crChatFriend *getChatFriend(CRGUI::crImageStage *stage);
	CRGUI::crImageStage *getChatFriendStage(crNetChatData::crChatFriend *chatFriend);
	//DownloadServer动态下载
	typedef std::deque<std::string> DownloadFileDeque;//filename，下载文件队列
	typedef std::map<int,DownloadFileDeque> DownloadFileTaskMap;//下载完成后要触发的消息
	bool insertDownloadFile(int msgparam, const std::string &file);//返回false表示该文件已经存在于下载队列
	void startDownloadFile(int msgparam, short netType);
	void recvedFileStream(crRecvFileStream *fileStream);//下载完成
	void fileNotExist(unsigned short downloadFlg,const std::string &file,short netType);//文件不存在

	//HttpDown 动态下载
	//void lockHttpDownload();
	//void unlockHttpDownload();
	void insertHttpDownloadFile(int msgparam, const std::string &file);//返回false表示该文件已经存在于下载队列
	void startHttpDownloadTask(int msgparam);
	void downloadUpdate();
protected:
	FriendChatMap m_friendChatMap;
    CRCore::crMutex m_friendChatMapMutex;

	DownloadFileTaskMap m_downloadFileTaskMap;
	GNE::Mutex m_downloadFileMutex;

	//HttpDown 动态下载
	DownloadFileTaskMap m_perhttpDownloadTaskMap;
	DownloadFileTaskMap m_httpDownloadTaskMap;
	GNE::Mutex m_perhttpDownloadMutex;
	GNE::Mutex m_httpDownloadMutex;
};
//crRole Data
typedef std::vector<int> IntVec;//技能实例ID，技能等级存放在技能Data里
//typedef std::vector<int> EquipVec;//装备栏，实例ID 
//typedef std::vector<int> ThingVec;//物品栏，实例ID，物品数量存放在物品Data里
//typedef std::vector<int> EquipOnVec;//装备在身上的，实例ID
//typedef std::vector<int> QuickVec;//快捷栏，实例ID，可存放技能或可使用的物品
//typedef std::vector<int> Storage;//仓库，实例ID
typedef std::set<int> GroupMemberSet;//队友的playerid, 不需要储存
//typedef std::vector<int> FriendVec;//好友playerid
typedef std::set<int> MetierSkillASet;//职业技能抽象ID

typedef std::map<int,int> AttackMap;//普通攻击抽象ID，实例ID，技能等级存放在技能Data里
typedef std::map<int,int> SkillMap;//技能抽象ID，实例ID，技能等级存放在技能Data里
typedef std::vector<CRCore::crVector2f> PatrolPointVec;//米为单位
//typedef std::map<_crInt64,float> PlayerEnmityMap;//playerid,roleid,仇恨度
//typedef std::map<int,float> NpcEnmityMap;//itemid,仇恨度
typedef std::multimap< CRCore::ref_ptr<crInstanceItem>,int > EnmityMap;//item,enmity
typedef std::multimap< float, CRCore::ref_ptr<crInstanceItem>, std::less<float> > InPatrolEnemyMap;//dist(米为单位),item
typedef std::map<int,float> ItemDropMap;//item abstractid,可能性 装备掉落被玩家拾取后发crCreateItemChildPacket
typedef std::map<int, CRCore::ref_ptr<crGameTask> >GameTaskMap;//taskid
typedef std::multimap<int,int> BackPackMap;//抽象ID,实例ID
typedef std::multimap<int,int> StorageMap;//仓库 抽象ID,实例ID
typedef std::multimap<int,int> EquipOnMap;//抽象ID,实例ID
//typedef std::map<int,int> QuickMap;//抽象ID,实例ID
typedef std::list< std::pair<int,int> > QuickList;//抽象ID,实例ID
typedef std::multimap<int,unsigned char> ItemCountMap;//抽象ID,count
//typedef std::map<int,int> NpcSaleItemMap;//抽象ID,实例ID
//Npc售卖加入物品数量，可以限制系统投放物品的数量。数量-1表示无穷，0表示没有库存。系统每隔一个时间段或者其它方式为NPC补充库存
//typedef std::pair<short,int> CountPricePair;//数量,售价 售价0表示价格从itemdata里取
//typedef std::pair<int,CountPricePair> SaleItemPair;//实例ID,数量,售价
typedef std::multimap<int,CRCore::crVector3i> SaleItemMap;//抽象ID,实例ID,数量,售价
typedef std::set<int> ExchangePlayerSet;//playerid
typedef std::list< std::pair<char,int> > QuickKeyVec;//key,抽象ID
typedef std::vector<int> EquipComposeRequestVec;//abstractid
typedef std::map<unsigned short,std::string> ActStateMap;//状态ID,动作名  角色的特殊动作模式 对话状态、坐着的状态、特殊交互状态等，禁用物理，并保持动作。
class CRNETAPP_EXPORT crRoleData : public CRCore::crData
{
public:
	crRoleData();
	crRoleData(const crRoleData& data);
	DataClass(CRNetApp, Role)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	void serverCheckData(crInstanceItem *item, _crInt64 completeMsg/*int& count*/);
	void checkItemChild(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,std::set<int>&AbstractIDSet,int &count);
	void checkItemChildMultiMap(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,std::set<int>&TemporaryAbstractIDSet,int &count);
	void clientCheckData(crInstanceItem *item);
	void clientCheckItemChild(crInstanceItem *item,int first,int &second);
	void clientCheckItemChildMultiMap(crInstanceItem *item,int first,int &second,std::set<int>&TemporaryItemIDSet);
	void gameServerCheckData(crInstanceItem *item);
	void gameServerCheckItemChild(crInstanceItem *item,int first,int &second,unsigned short gamedbid);
	void gameServerCheckItemChildMultiMap(crInstanceItem *item,int first,int &second);
	
	unsigned char m_maxattack;
	unsigned char m_maxskill;
	unsigned char m_maxbackpack;//背包的最大容量
	unsigned char m_maxstorage;
	unsigned char m_maxequipon;//从脚本读取，不用记录到数据库
	unsigned char m_maxquick;//从脚本读取，不用记录到数据库

	AttackMap m_attackMap;//
	SkillMap m_skillMap;//已学到的技能
	//BackPackMap m_backPackMap;
	//StorageMap m_storageMap;//仓库放在NPC/ITEM处，不放在玩家身上
	//ItemCountMap m_storageCountMap;
	EquipOnMap m_equipOnMap;
	//QuickList   m_quickList;
	//QuickKeyVec m_quickKeyVec;//快捷键设置
	//ItemCountMap m_itemCountMap;

	//ItemDropMap m_itemDropMap;
	//SaleItemMap m_saleItemMap;//需要网络传输
	//4 m_exchangePlayerSet;//运行时刻产生的数据，只在服务器，不需要同步给客户端，当m_saleItemMap有变化时，同步给这里的玩家

	short m_sightRange;//视野 从脚本读取，不用记录到数据库
	short m_guardRange;//警戒半径
	short m_eyeHeight;//眼睛高度 从脚本读取，不用记录到数据库
	int m_hp;
	int m_mp;
	int m_attack;
	int m_defend;
	short m_magicDefend;//法防
	short m_speed;//移动速度
	short m_stealth;//隐身距离，对方的视野-隐身距离
	short m_stealthspy;//反隐距离
	short m_hitratio;//命中率
	//float m_attackSpeed;//攻速
	short m_attackSpeed;//攻击速度提升千分比
	short m_skillSpeed;//施法速度提升千分比
	unsigned char m_armourType;//盔甲类型
	//运行时刻数据
	unsigned char m_targetType;
	//CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	int m_targetID;
	int m_targetRoleID;//m_targetType=role时有效
	CRCore::crVector3 m_targetPosition;
	CRCore::crVector3 m_moveToPosition;//IS_MoveToPos IS_MoveToUseSkill IS_AttackToPos
	unsigned char m_itemState;
	unsigned char m_itemState_rec;
	unsigned int m_guiseState;//外形状态
	unsigned char m_camp;//阵营//0中立（不可攻击,观察者） 1混战（所有人都可以对其攻击） 2组队ID（从2开始后面都是组队ID，不同组队间可以攻击）
	unsigned char m_fightmode;//战斗模式
	float m_followDistance;//跟随目标，与目标保持的距离
	CRNetApp::PatrolPointVec m_patrolPointVec;//巡逻点坐标
	char m_patrolIndex;//当前巡逻索引
	unsigned char m_patrolLoopMode;//0:不循环 1:LOOP 2:SWING
	//int m_baseEnmity;//基础仇恨度
	//CRNetApp::PlayerEnmityMap m_playerEnmityMap;//对玩家的仇恨度
	//CRNetApp::NpcEnmityMap m_npcEnmityMap;//对NPC的仇恨度
	EnmityMap m_enmityMap;
	InPatrolEnemyMap m_inpatrolEnemyMap;

	float m_rthp;
	float m_rtmp;
	//float m_currentUseItemProtectTime;//当前的技能保护时间
	int m_currentAttackID;//当前选择的普通攻击itemid
	int m_aboutToUseItemID;//将要使用的itemid，比如即将要施放技能，使用过之后需置为0
	//附加状态对属性的影响值，对相同属性的附加影响，后者会覆盖前者
	short m_extrahp;
	short m_extrahp_rd;//read
	short m_extramp;
	short m_extramp_rd;
	short m_extraAttack;
	short m_extraAttack_rd;
	short m_extraDefend;//附加状态对防御属性的影响值
	short m_extraDefend_rd;
	//short m_extraSpeed;
	//short m_extraSpeed_rd;
	short m_extraStealth;
	short m_extraStealth_rd;
	short m_extraStealthSpy;
	short m_extraStealthSpy_rd;
	short m_extraHitratioPercent;//命中率百分比
	short m_extraHitratioPercent_rd;
	short m_extraAttackSpeed;//攻击速度提升百分比
	short m_extraAttackSpeed_rd;
	short m_extraSkillSpeed;//施法速度提升百分比
	short m_extraSkillSpeed_rd;
	unsigned int m_extraGuise;//外形状态
	unsigned int m_extraGuise_rd;
	short m_extraRadius;//碰撞半径百分比
	short m_extraRadius_rd;
	short m_extraIgnoreDefend;//穿透，护甲忽视百分比
	short m_extraIgnoreDefend_rd;
	short m_extraIgnoreMagicDefend;//法术穿透，护甲忽视百分比
	short m_extraIgnoreMagicDefend_rd;
	short m_extraSightRange;
	short m_extraSightRange_rd;

	short m_crit;//暴击
	short m_parry;//格挡
	short m_dodge;//闪避
	short m_extraCrit;
	short m_extraCrit_rd;
	short m_extraParry;
	short m_extraParry_rd;
	short m_extraDodge;
	short m_extraDodge_rd;
	bool m_isCrit;//
	bool m_isParry;
	bool m_isDodge;//
	bool m_isMiss;//是否未命中

	short m_extraAttackPercent;
	short m_extraAttackPercent_rd;
	short m_extraSpeedPercent;
	short m_extraSpeedPercent_rd;
	short m_extraDefendPercent;
	short m_extraDefendPercent_rd;
	short m_extraMagicDefendPercent;//附加状态对防御属性的影响值
	short m_extraMagicDefendPercent_rd;
	short m_extraBuildAttackPercent;
	short m_extraBuildAttackPercent_rd;
	short m_extraCritPercent;
	short m_extraCritPercent_rd;
	short m_extraParryPercent;
	short m_extraParryPercent_rd;
	short m_extraDodgePercent;
	short m_extraDodgePercent_rd;
	short m_extraAttackDist;//厘米
	short m_extraAttackDist_rd;//厘米
	short m_extraAttackAbsorbHP;
	short m_extraAttackAbsorbHP_rd;
	short m_extraSkillAbsorbHP;
	short m_extraSkillAbsorbHP_rd;
	short m_extraAttackDamageBack;
	short m_extraAttackDamageBack_rd;
	short m_extraSkillDamageBack;
	short m_extraSkillDamageBack_rd;
	float m_extraRTHP;
	float m_extraRTHP_rd;

	short m_inbornHPPercent;
	short m_inbornMPPercent;
	short m_inbornAttackPercent;
	short m_inbornDefendPercent;
	short m_inbornMagicDefendPercent;
	short m_inbornBuildAttackPercent;
	short m_inbornSpeed;
	short m_inbornAttackSpeedPercent;
	short m_inbornSkillSpeedPercent;
	short m_inbornSightRange;
	short m_inbornAttackDist;//厘米
	///
	short m_inbornStealth;
	short m_inbornStealthSpy;
	short m_inbornCrit;
	short m_inbornParry;
	short m_inbornDodge;
	short m_inbornIgnoreDefend;
	short m_inbornIgnoreMagicDefend;

	unsigned char m_inbornIgnoreDodge;//无视目标闪避 不可叠加
	short m_inbornAttackDamageBack;//物理伤害反弹
	short m_inbornSkillDamageBack;//技能伤害反弹
	short m_inbornAttackAbsorbHP;//物理吸血
	short m_inbornSkillAbsorbHP;//技能吸血
	short m_inbornCritPower;//暴击剑 不可叠加（暴击杀伤力从200%变为250%）->暴击倍率,百分比
	_crInt32 m_inbornSputtering;//溅射伤害（溅射半径(unsigned short)，伤害百分比（unsigned short））

	short m_inbornAttackDamage;//加深伤害或者伤害减免
	short m_inbornSkillDamage;
	short m_extraAttackDamage;
	short m_extraAttackDamage_rd;
	short m_extraSkillDamage;
	short m_extraSkillDamage_rd;

	short m_inbornSkillDamageIncrease;//法伤加成
	short m_extraSkillDamageIncrease;//法伤加成
	short m_extraSkillDamageIncrease_rd;//法伤加成
	short m_inbornAttackDamageIncrease;//物伤加成
	short m_extraAttackDamageIncrease;//物伤加成
	short m_extraAttackDamageIncrease_rd;//物伤加成

	unsigned char m_extraIgnoreDodge;
	unsigned char m_extraIgnoreDodge_rd;
	short m_extraCritPower;
	short m_extraCritPower_rd;
	_crInt32 m_extraSputtering;//溅射伤害（溅射半径(unsigned short)，伤害百分比（unsigned short））
	_crInt32 m_extraSputtering_rd;//溅射伤害（溅射半径(unsigned short)，伤害百分比（unsigned short））

	short m_inbornHitratioPercent;//命中率百分比

	//short m_cureEffectPercent;//回复效果百分比
	short m_extraCureEffectPercent;//回复效果百分比
	short m_extraCureEffectPercent_rd;
	short m_inbornCureEffectPercent;

	CRNetApp::ActStateMap m_actStateMap;
	unsigned short m_currentActState;//0表示未处于特殊状态 客户端根据事件会改变该指，不需要网络同步
	float m_currentActDuration;//客户端根据事件会改变该值，不需要网络同步
	//CRCore::crVector2 m_halfLengths;//itemcollidesize,从脚本读取，不存数据库，不需要网络同步,碰撞半径，攻击半径
	CRCore::crVector3 m_halfPhysicsBox;//itemcollidesize,从脚本读取，不存数据库，不需要网络同步,碰撞半径，攻击半径
	CRCore::ref_ptr<ItemUseParam> m_itemUseParam;//技能释放
	CRCore::ref_ptr<ItemUseParam> m_attackUseParam;//普通攻击
	/////////////////////////////////////
	//附加状态信息
	CRCore::ref_ptr<CRCore::crData> m_extraDataClass;//附加状态的同步问题
	//使用技能时的附加状态只在使用时候同步给附近玩家
	//装备的附加状态，各端自行设置。
	////////////////////////

	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;//==crRole::m_stream
	GNE::Mutex m_dataMutex;
	static GNE::Mutex m_saveMutex;
};
class CRNETAPP_EXPORT crRpgGameRoleData : public crRoleData
{
public:
	crRpgGameRoleData();
	crRpgGameRoleData(const crRpgGameRoleData& data);
	DataClass(CRNetApp, RpgGameRole)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	//void serverCheckData(crInstanceItem *item, int &count);
	//void checkItemChild(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,int &count);
	//void checkItemChildMultiMap(CRNet::crNetConductor *netConductor,crInstanceItem *item,int first,int &second,std::set<int>&TemporaryAbstractIDSet,int &count);
	//void clientCheckItemChild(crInstanceItem *item,int first,int &second);
	//void clientCheckItemChildMultiMap(crInstanceItem *item,int first,int &second,std::set<int>&TemporaryItemIDSet);
	//short m_sightRange;//视野 从脚本读取，不用记录到数据库
	//int m_hp;
	//int m_mp;
	//short m_attack;
	//short m_defend;
	//short m_speed;
	//float m_attackSpeed;//攻速

	//Tank交互
	short m_currentSpeed;//当前速度，m_speed = 最大速度
	short m_acceleration;//加速度
	short m_damp;//阻尼 阻力 = m_currentSpeed * m_damp;
	float m_currentAngular;//当前角速度,单位是弧度
	short m_angularAccel;//角加速度，单位是角度/s
	short m_angularDamp;//角速度衰减，单位是角度/s
	short m_maxAngular;//最大角速度，单位是角度/s

	unsigned char m_level;
	int m_experience;
	int m_gold;
	short m_strength;
	short m_agility;
    short m_intellect;
	//unsigned char m_camp;

	//unsigned char m_maxattack;
	//unsigned char m_maxskill;
	//unsigned char m_maxbackpack;//背包的最大容量
	//unsigned char m_maxstorage;
	//unsigned char m_maxequipon;//从脚本读取，不用记录到数据库
	//unsigned char m_maxquick;//从脚本读取，不用记录到数据库

	//AttackMap m_attackMap;//
	//SkillMap m_skillMap;//已学到的技能
	//BackPackMap m_backPackMap;
 //   StorageMap m_storageMap;//仓库放在NPC/ITEM处，不放在玩家身上
	//ItemCountMap m_storageCountMap;
 //   EquipOnMap m_equipOnMap;
 //   QuickList   m_quickList;
	//QuickKeyVec m_quickKeyVec;//快捷键设置
	//ItemCountMap m_itemCountMap;

	//以下数据从脚本读取，无需记录到数据库
	std::string m_itemIcon;//图标
	//PatrolPointVec m_patrolPointVec;//巡逻点坐标
	short m_expToKiller;
	short m_goldToKiller;
 //   ItemDropMap m_itemDropMap;
	//SaleItemMap m_saleItemMap;//需要网络传输
	//ExchangePlayerSet m_exchangePlayerSet;//运行时刻产生的数据，只在服务器，不需要同步给客户端，当m_saleItemMap有变化时，同步给这里的玩家

    unsigned short m_attrPoint;//未分配的属性点,每升1级获得5点，注意避免数字溢出
	unsigned short m_skillPoint;//未使用的技能点,每升1级获得2点，注意避免数字溢出

	//以下为运行时刻产生的数据，不记录到datastream
	//mouse pick packet
	//需要网络同步的数据 NetControl Data
	//unsigned char m_targetType;
	//CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	//int m_targetID;
	//int m_targetRoleID;//m_targetType=role时有效
	//CRCore::crVector3 m_targetPosition;
	//unsigned char m_itemState;
	//unsigned char m_guiseState;//外形状态
	//float m_followDistance;//跟随目标，与目标保持的距离
    //运行时刻数据
	//float m_rthp;
	//float m_rtmp;
	short m_kill;//当前战役杀敌数
	short m_dead;//当前战役死亡次数
 //   /////////////////////////////////////
	////附加状态信息
	//CRCore::ref_ptr<CRCore::crData> m_extraDataClass;//附加状态的同步问题
	////使用技能时的附加状态只在使用时候同步给附近玩家
	////装备的附加状态，各端自行设置。
	//////////////////////////
	//float m_currentUseItemProtectTime;//当前的技能保护时间

    /////主机端数据
	//int m_currentAttackID;//当前选择的普通攻击itemid
	//int m_aboutToUseItemID;//将要使用的itemid，比如即将要施放技能，使用过之后需置为0
	//unsigned char m_fightmode;//战斗模式
	GroupMemberSet m_groupMemberSet;
	//unsigned char m_patrolIndex;//当前巡逻索引
	//PlayerEnmityMap m_playerEnmityMap;//对玩家的仇恨度
	//NpcEnmityMap m_npcEnmityMap;//对NPC的仇恨度
	//附加状态对属性的影响值，对相同属性的附加影响，后者会覆盖前者
	//short m_extrahp;
	//short m_extramp;
	//short m_extraAttack;
	//short m_extraDefend;//附加状态对防御属性的影响值
	//short m_extraSpeed;
	//float m_extraAttackSpeed;
    char m_extraLevel;
    short m_extraStrength;
    short m_extraAgility;
	short m_extraIntellect;

	//SpecialActStateMap m_specialActStateMap;
	//unsigned short m_currentSpecialActState;//0表示未处于特殊状态 客户端根据事件会改变该指，不需要网络同步
	//float m_currentActDuration;//客户端根据事件会改变该指，不需要网络同步
	unsigned char m_controllerFlg;//0:stopmove, 1:moveup, 2:movedown, 3:moveleft, 4:moveright
	float m_controllerTime;//控制的有效时间
};
//crRole MetierData
class CRNETAPP_EXPORT crRpgGameMetierData : public CRCore::crData
{
public:
	crRpgGameMetierData();
	crRpgGameMetierData(const crRpgGameMetierData& data);
	DataClass(CRNetApp, RpgGameMetier)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	virtual ~crRpgGameMetierData();
	MetierSkillASet m_metierSkillASet;//职业技能抽象ID列表,从脚本读取，不用存储到数据库
	GameTaskMap m_gameTaskMap;
	//CRCore::ref_ptr<crGameTask> m_addTask;//load时候用到
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;//==crRole::m_metierStream
	GNE::Mutex m_dataMutex;
	int m_currentTaskID;
	static GNE::Mutex m_saveMutex;
};
//crViewMatterObject Data
class CRNETAPP_EXPORT crRpgGameBotData : public CRCore::crNodeData
{
public:
	crRpgGameBotData();
	crRpgGameBotData(const crRpgGameBotData& data);
	DataClass(CRNetApp, RpgGameBot)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	GNE::Mutex m_dataMutex;
	CRCore::ref_ptr<crInstanceItem> m_item;
    
	float m_viewHeight;
	float m_walkableHeight;
	float m_turnToIdleTime;
	std::string m_botEffectStr;
    //主角参数
	//float m_cameraBorrowTime;//相机借出时间
	//float m_cameraBorrowInterval;//相机借用时间间隔
};
//crInstanceItem Data
class CRNETAPP_EXPORT crRpgGameItemData : public CRCore::crData
{
public:
	crRpgGameItemData();
	crRpgGameItemData(const crRpgGameItemData& data);
	DataClass(CRNetApp, RpgGameItem)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	unsigned char m_dataType;
	unsigned char m_itemState;
	unsigned int m_guiseState;//外形状态
	unsigned short m_touchDistance;
	short m_zoffset;
	int m_belongPlayerID;
	int m_itemPrice;
	float m_rthp;
	float m_useItemProtectTime;//使用保护时间
	std::string m_itemIcon;
	//EquipComposeRequestVec m_equipComposeRequestVec;
	//int m_composeEquipID;//抽象id
	//SaleItemMap m_saleItemMap;
	//ExchangePlayerSet m_exchangePlayerSet;//运行时刻产生的数据，只在服务器，不需要同步给客户端，当m_saleItemMap有变化时，同步给这里的玩家
	GNE::Mutex m_dataMutex;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;

	//Equip
	std::string m_meshFileName;//蒙皮filename
	//Skill or Thing
	std::string m_animFileName;//动作filename
	///只需要客户端存在
	std::string m_itemTalk;
	std::string m_itemOption;
	std::string m_itemInfoText;
	//CRCore::crVector2 m_halfLengths;//itemcollidesize,从脚本读取，不存数据库，不需要网络同步
	CRCore::crVector3 m_halfPhysicsBox;
	char m_defaultkey;
};
//crNode Data
class CRNETAPP_EXPORT crRpgGameNodeData : public CRCore::crNodeData
{
public:
	crRpgGameNodeData();
	crRpgGameNodeData(const crRpgGameNodeData& data);
	DataClass(CRNetApp, RpgGameNode)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	CRCore::ref_ptr<crInstanceItem> m_item;
};

class CRNETAPP_EXPORT crRpgGameWeaponData : public CRCore::crNodeData
{
public:
	crRpgGameWeaponData();
	crRpgGameWeaponData(const crRpgGameWeaponData& data);
	DataClass(CRNetApp, RpgGameWeapon)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	CRCore::ref_ptr<crInstanceItem> m_item;
	std::string m_weaponActStr;
	float m_weaponSpeedScale;
};

class CRNETAPP_EXPORT crRpgGameSkillData : public CRCore::crData
{
public:
	crRpgGameSkillData();
	crRpgGameSkillData(const crRpgGameSkillData& data);
	DataClass(CRNetApp, RpgGameSkill)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	unsigned char m_dataType;
	unsigned char m_skilllevel;
	float m_cdtime;
	char m_skilltype;//技能类型,主动技能或被动技能

	unsigned short m_itemUseRange;//使用范围
	short m_itemUseScope;//作用范围
	CRCore::crVector2s m_physicsDM;
	CRCore::crVector2s m_magicDM;
	unsigned short m_needMP;
	unsigned short m_needHP;
	int m_targetExtraID;
	int m_userExtraID;
	unsigned char m_targetType;//使用对象类型
	int m_belongPlayerID;//新加
	int m_itemPrice;//1级的价格，升级价格 ＝ skilllevel * itemPrice
	std::string m_itemIcon;

	float m_useItemProtectTime;//使用保护时间
	float m_duration;//持续时间
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;
	GNE::Mutex m_dataMutex;
	///只需要客户端存在
	std::string m_meshFileName;//蒙皮filename 新加
	std::string m_animFileName;//动作filename 新加
	std::string m_itemInfoText;
	short m_aimid;
	CRCore::crVector2 m_uiAimRange;
	float m_attackAim;
	static GNE::Mutex m_saveMutex;
	char m_defaultkey;
	//运行时刻数据
	_crInt64 m_lastUseTime;//上一次使用时间
};
typedef std::map<short,short> ExtraIDMap;//id,(叠加次数,优先级)
class CRNETAPP_EXPORT crRpgGameExtraData : public CRCore::crData
{
public:
	crRpgGameExtraData();
	crRpgGameExtraData(const crRpgGameExtraData& data);
	DataClass(CRNetApp, RpgGameExtra)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
	virtual void clear();
protected:
    ExtraIDMap m_extraIDMap;
	GNE::Mutex m_dataMutex;
};

//VR_ICBC
class CRNETAPP_EXPORT crTalkChooseUIData : public CRCore::crUIData
{
public:
	crTalkChooseUIData();
	crTalkChooseUIData(const crTalkChooseUIData& data);
	DataClass(CRNetApp, TalkChooseUI)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	unsigned short m_buttonid;
	unsigned short m_textid;
};

class CRNETAPP_EXPORT crBrainData : public CRCore::crData
{
public:
	crBrainData();
	crBrainData(const crBrainData& data);
	DataClass(CRNetApp, Brain)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	CRCore::crNode* m_currentFocusNode;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_createRoleTab;
	int m_currentRoleTabIndex;
	CRCore::crNode* m_roleAttachToNode;
	std::string m_attachBoneName;
	CRCore::crVector3 m_attachOffset;
	CRCore::crVector3 m_attachScale;
	float m_attachRotAngle;
    CRCore::crNode* m_loadedNode;
};
class crRole;
typedef std::vector< CRCore::ref_ptr<crRole> > SelectMyRoleVec    ;
typedef std::map< CRCore::ref_ptr<CRCore::crMatrixTransform>, CRCore::ref_ptr<CRNetApp::crWaypoint> > WaypointNodeMap;
class CRNETAPP_EXPORT crCameraData : public CRCore::crNodeData
{
public:
	crCameraData();
	crCameraData(const crCameraData& data);
	DataClass(CRNetApp, Camera)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
	enum CameraMove
	{
		None = 0x0,
		MoveLeft = 0x1,
		MoveRight = 0x2,
		MoveUp = 0x4,
		MoveDown = 0x8,
	};
	enum CameraRot
	{
		RotLeft = 0x1,
		RotRight = 0x2,
	};
protected:
	virtual ~crCameraData();
	unsigned char m_cameraMove;
	unsigned char m_cameraRot;
	unsigned char m_targetType;
	double m_cameraBorrowTime;//相机借出时间
	float m_cameraBorrowInterval;//相机借用时间间隔
	CRCore::ref_ptr<CRCore::crNode> m_targetNode;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_targetItem;
	CRCore::crVector3 m_targetPosition;
	CRCore::crVector3i m_mousePosRec;//x,y, z=0表示无效，1表示有效
	SelectMyRoleVec m_selectMyRoles;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_selectItem;
	
	CRCore::ref_ptr<CRNetApp::crWaypoint> m_waypointroot;//Editor用
	WaypointNodeMap m_waypointNodeMap;//Editor
	GNE::Mutex m_dataMutex;
};
class crPlayerGameData;
class crCommandDlgData : public CRCore::crUIData
{
public:
	crCommandDlgData();
	crCommandDlgData(const crCommandDlgData& data);
	DataClass(CRCore, CommandDlg)
	virtual void inputParam(int i, void *param);
	virtual void excHandle(_crInt64 msg);
protected:
	int m_parentType;
	void *m_parent;
	//CRCore::ref_ptr<CRCore::crWidgetNode> m_commandDlgWidget;
	//CRCore::ref_ptr<crPlayerGameData> m_commandDlgPlayerGameData;
};
typedef std::map<CRCore::ref_ptr<crInstanceItem>,float> HitMap;//hit,rate
class CRNETAPP_EXPORT crUseItemRecord : public CRCore::Referenced
{
public:
	crUseItemRecord():m_duration(0.0f),m_damageCount(1){}
	void setItem(crInstanceItem *item) { m_item = item; }
	crInstanceItem *getItem() { return m_item.get(); }
	void setUser(crInstanceItem *user) { m_user = user; }
	crInstanceItem *getUser() { return m_user.get(); }
	void setTarget(crInstanceItem *target) { m_target = target; }
	crInstanceItem *getTarget() { return m_target.get(); }
	void setDuration(float duration) { m_duration = duration; }
	float& duration(){ return m_duration; }
	HitMap &getHitMap(){ return m_hitMap; }
	void setDamageCount(char count){ m_damageCount = count; }
	char getDamageCount(){ return m_damageCount; }
	bool getHitValid(crInstanceItem *hititem, bool npcfire);
protected:
	CRCore::ref_ptr<crInstanceItem> m_item;//bulletitem
	CRCore::ref_ptr<crInstanceItem> m_user;
	CRCore::ref_ptr<crInstanceItem> m_target;
	HitMap m_hitMap;//击中者记录，防止重复击中
	float m_duration;
	char m_damageCount;
};
typedef std::multimap< CRCore::ref_ptr<crInstanceItem>,CRCore::ref_ptr<crUseItemRecord> > UseItemRecordMap;//user,record
typedef std::multimap< float,CRCore::ref_ptr<crUseItemRecord>,std::less<float> > HitTestMap;//duration,record
class CRNETAPP_EXPORT crSceneServerData : public CRCore::crData
{
public:
	crSceneServerData();
	crSceneServerData(const crSceneServerData& data);
	DataClass(CRNetApp, SceneServer)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	UseItemRecordMap m_useItemRecordMap;
	GNE::Mutex m_dataMutex;
};
class CRNETAPP_EXPORT crRoomData : public CRCore::crData
{
public:
	crRoomData();
	crRoomData(const crRoomData& data);
	DataClass(CRNetApp, Room)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(_crInt64 msg);
protected:
	short m_progress;
	GNE::Mutex m_dataMutex;
};
}
#endif