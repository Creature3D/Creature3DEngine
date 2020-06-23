/**********************************************************************
*
*	文件:	   appData.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/

#ifndef JXJ_APPDATA_H
#define JXJ_APPDATA_H
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crWchMsg.h>
#include <gnelib.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRNetApp/appDataClass.h>
#include <CREncapsulation/crTableIO.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appGameData.h>
#include <CRUI/crWidgetExtend.h>
#include <bitset>
namespace CRNetApp
{
	class crRole;
}
namespace JXJ{
/////////BattleID = unsigned char,unsigned char,unsigned short(攻势力id,守势力id,城池id)
class crJXJChengfangInfo : public CRCore::Referenced
{
public:
	crJXJChengfangInfo():m_qun(0)
	{
	}
	unsigned char getChengzhuShili();
	unsigned short getChengzhuChengfang();
	unsigned char changeShiliChengfang(unsigned char winshili,unsigned char lostshili);//返回值是城主势力id
	void setQunChengfeng(unsigned short chengfang){ m_qun = chengfang; }
	unsigned short getQunChengfang() { return m_qun; }
	typedef std::multimap< unsigned short,unsigned char,std::greater<unsigned short> > ShiliChengfangMap;//城防值,势力id
	ShiliChengfangMap getShiliChengfangMap(){ return m_shilichengfangmap; }
	void reset(unsigned char shili, unsigned short chengfang);
	void buildStream(CRCore::ref_ptr<CRCore::crStreamBuf> &stream);
	void setStream(CRCore::crStreamBuf *stream);
protected:
	GNE::Mutex m_shilichengfangmapMutex;
	ShiliChengfangMap m_shilichengfangmap;
	unsigned short m_qun;//群的城防
};
//typedef std::map<unsigned short, std::pair<unsigned char,unsigned short> >ChengchiMap;//城池id，势力id,城防值
typedef std::map<unsigned short, CRCore::ref_ptr<crJXJChengfangInfo> >ChengchiMap;//城池id，势力id,城防值
typedef std::deque<int> BattleDeque;
typedef std::map<_crInt32, BattleDeque> DefendDequeMap;//unsigned char,unsigned char,unsigned short(0,守势力id,城池id)
typedef std::map<_crInt32, BattleDeque> AttackDequeMap;//unsigned char,unsigned char,unsigned short(攻势力id,守势力id,城池id)
typedef std::list<int> BattlePlayer;
typedef std::pair<BattlePlayer,BattlePlayer> BattlePlayerPair;//攻方，守方
typedef std::multimap<_crInt32,std::pair<BattlePlayerPair,std::pair<int,float> >,std::less<int> > OpenBattleMap;//已经开启的战场,unsigned char,unsigned char,unsigned short(攻势力id,守势力id,城池id),roomid,战场倒计时
typedef std::map< unsigned char,std::pair<short,int> > ShiliScoreMap;//势力ID，分数，杀敌数量
typedef std::map<unsigned short, ShiliScoreMap> BattleScoreMap;//战场分数统计,城池id
typedef std::deque<_crInt64> CreateBattleDeque;
typedef std::map<int,std::pair<_crInt32,float> > BattleDropPlayerMap;//playerid,battleid,倒计时,对于战场掉线和提前退出战场的玩家的惩罚计时
typedef std::map< unsigned short, unsigned char >TroopsTechMap;//军备id，军备等级
typedef std::map< int, float >TechDevelopMap;//科技id，科技倒计时
typedef std::map<unsigned short,CRCore::ref_ptr<crJXJNewGuide>> JXJNewGuideMap;//引导id，crJXJNewGuide
typedef std::set<_crInt64> ClosedBattleSet;//battlerid,roomid
typedef std::set<unsigned short> NewFunctionOpenSet;//id(新功能开放提示)
typedef std::map<unsigned short, bool>CouldGetAwardMap;//可领取奖励（奖励ID， 是否可领取）;
typedef std::map< unsigned short, unsigned char >CardExtraTechMap;//组合ID，激活等级

template <class T,class C = std::greater<int>,class K = int >
class crJXJRankContainer 
{
public:

	typedef std::multiset<T,C>  RankSet;
	typedef std::multimap<K,T> RankMap;

	crJXJRankContainer():m_limitNum(100)
	{
		m_rankSet.clear();
		m_rankMap.clear();
	}
	void resize(unsigned int num)
	{
		m_limitNum = num;
		adjustSize();
	}
	int size () {return m_rankMap.size();}
	bool empty() {return m_rankMap.empty();}
	void updateRank(K key,T val)
	{
		RankMap::iterator it = m_rankMap.find(key);
		if (it != m_rankMap.end())
		{
			//C compare;
			//if ( !compare(it->second,val) )
			//{
				RankSet::iterator setItr = m_rankSet.begin();
				for (; setItr != m_rankSet.end(); ++ setItr)
				{
					if ( (*setItr )->getKey() == key )
					{
						m_rankSet.erase(setItr);
						m_rankSet.insert(val);
						it->second = val;
						break;
					}
				}
			//}
		}
		else
		{
			m_rankMap.insert(RankMap::value_type(key,val));
			m_rankSet.insert(val);
			adjustSize();
		}
	}
	void delRank(K key)
	{
		RankMap::iterator it = m_rankMap.find(key);
		if (it != m_rankMap.end())
		{
			RankSet::iterator setItr = m_rankSet.begin();
			for (; setItr != m_rankSet.end(); ++ setItr)
			{
				if ( (*setItr )->getKey() == key )
				{
					m_rankSet.erase(setItr);
					m_rankMap.erase(it);
					break;
				}
			}
		}
	}

	void adjustSize()
	{
		RankMap::reverse_iterator rItr ;
		if (m_rankMap.size() > m_limitNum)
		{
			rItr = m_rankMap.rbegin();
			for (; rItr != m_rankMap.rend() && m_rankMap.size() > m_limitNum;)
			{
				m_rankMap.erase( (++rItr).base() );
				rItr = m_rankMap.rbegin();
			}

			RankSet::reverse_iterator rItr2;
			if (m_rankSet.size() > m_limitNum)
			{
				rItr2 = m_rankSet.rbegin();
				for (; rItr2 != m_rankSet.rend() && m_rankSet.size() > m_limitNum;)
				{
					m_rankSet.erase( (++rItr2).base() );
					rItr2 = m_rankSet.rbegin();
				}
			}
		}
	}
	void clear()
	{
		m_rankSet.clear();
		m_rankMap.clear();
	}
	RankSet & getRankSet() {return m_rankSet;}
	RankMap & getRankMap() {return m_rankMap;}
	unsigned int getRankNum(K key)
	{
		unsigned int ret = 0;
		RankSet::iterator itr = m_rankSet.begin();
		for (; itr != m_rankSet.end(); ++ itr)
		{
			++ ret;
			if ((*itr)->getKey() == key)
				break;
		}
		if (!ret) ret = m_limitNum + 1;
		return ret;
	}

private:
	unsigned int m_limitNum;
	RankSet m_rankSet;
	RankMap m_rankMap;
};

//_crInt32 BattleID = unsigned char,unsigned char,unsigned short(攻势力id,守势力id,城池id)
///
//struct MailInfo
//{
//	int sender_id;
//	int receiver_id;
//	std::string sender_name;
//	std::string receiver_name;
//	std::string title;
//	std::string content;
//};
//struct MailQuery
//{
//	unsigned char state;
//	int querier_id;
//	int start_pos;
//	int query_count;
//};
//开房间式玩家数据
typedef std::map<int,int> TroopsMap;//abstractid,count
typedef std::map<unsigned short,int> TroopsEquipMap;//equipid,count
//typedef std::map<int,int> LordEquipMap;//equipid,count
//typedef std::map<int,int> JXJItemMap;//itemid,count//图纸、材料、将卡等
class crMailQuery : public CRCore::Referenced
{
public:
	crMailQuery():m_state(0),m_querier_id(0),m_start_pos(0),m_query_count(0),m_id(0){}
	void setState(unsigned char state){ m_state = state; }
	unsigned char getState() { return m_state; }
	void setQueryID(int querier_id){ m_querier_id = querier_id; }
	int getQueryID() { return m_querier_id; }
	void setStartPos(int start_pos){ m_start_pos = start_pos; }
	int getStartPos() { return m_start_pos; }
	void setQuerycount(int query_count){ m_query_count = query_count; }
	int getQuerycount() { return m_query_count; }
	void setID(int id){ m_id = id; }
	int getID() { return m_id; }
protected:
	unsigned char m_state;
	int m_querier_id;
	int m_start_pos;
	int m_query_count;
	int m_id;
};
typedef std::deque< CRCore::ref_ptr<crMailQuery> > MailQueryDeque;
class crMailInfo : public CRCore::Referenced
{
public:
	crMailInfo():m_state(0),m_senderid(0),m_recvid(0),m_dbID(0){}
	void setState(unsigned char state){ m_state = state; }
	unsigned char getState() { return m_state; }
	void setSenderName(const std::string &name) { m_sendername = name; }
	std::string &getSenderName() { return m_sendername; }
	void setRecvName(const std::string &name) { m_recvname = name; }
	std::string &getRecvName() { return m_recvname; }
	void setTitle(const std::string &title) { m_title = title; }
	std::string &getTitle() { return m_title; }
	void setCreateData(const std::string &data) { m_createdata = data; }
	std::string &getCreateData() { return m_createdata; }
	void setBody(CRCore::ref_ptr<CRCore::crStreamBuf> body) { m_body = body; }
	CRCore::ref_ptr<CRCore::crStreamBuf> &getBody() { return m_body; }
	//void setText(const std::string &text) { m_text = text; }
	//std::string &getText() { return m_text; }
	void setSenderID(int senderid){ m_senderid = senderid; }
	int getSenderID() { return m_senderid; }
	void setRecvID(int recvid){ m_recvid = recvid; }
	int getRecvID() { return m_recvid; }
	void setID(int id){ m_dbID = id; }
	int getID() { return m_dbID; }
protected:
	unsigned char m_state;
	std::string m_sendername;
	std::string m_recvname;
	std::string m_title;
	std::string m_createdata;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_body;
	//std::string m_text;
	int m_senderid;
	int m_recvid;
	int m_dbID;
};
typedef std::vector< CRCore::ref_ptr<crMailInfo> > MailInfoVec;
typedef std::deque< CRCore::ref_ptr<crMailInfo> > MailTaskDeque;
typedef std::vector< CRCore::ref_ptr<crMailInfo> > JXJRecvMailVec;
class crAppointCardData : public CRCore::Referenced
{
public:
	crAppointCardData():m_itemid(0),m_validtime(0){}
	void setItemID(int itemid){ m_itemid = itemid; }
	int getItemID() { return m_itemid; }
	void setValidTime(float validtime){m_validtime = validtime;}
	float getValidTime(){return m_validtime;}
protected:
	int m_itemid;
	float m_validtime;
};
#define EquipMagicSize 4
class crBagItemData : public CRCore::Referenced
{
public:
	crBagItemData():m_itemid(0),m_count(0),m_flg(0),m_equipmagic(0){}
	void setItemID(int itemid){ m_itemid = itemid; }
	int getItemID() { return m_itemid; }
	void setItemCount(unsigned char count){ m_count = count; }
	unsigned char getItemCount() { return m_count; }
	//void setJiangkaData(unsigned char jiangkadata){ m_jiangkadata = jiangkadata; }
	//unsigned char getJiangkaData() { return m_jiangkadata; }
	void setFlg(bool flg){ m_flg = flg; }
	bool getFlg() { return m_flg; }
	void setRefidID(unsigned char ref0,unsigned char ref1,unsigned char ref2)
	{
		_crInt16 hi16 = HIINT32(m_equipmagic);
		m_equipmagic = MAKEINT32(MAKEINT16(ref0,ref1),MAKEINT16(ref2,HIINT16(hi16)));
	}
	void setInlayID(unsigned char inlayid)
	{
		_crInt16 lo16 = LOINT32(m_equipmagic);
		_crInt16 hi16 = HIINT32(m_equipmagic);
		m_equipmagic = MAKEINT32(lo16,MAKEINT16(LOINT16(hi16),inlayid));
	}
	unsigned char getInlayID() { return HIINT16(HIINT32(m_equipmagic)); }
	void setEquipMagic(_crInt32 equipmagic){ m_equipmagic = equipmagic; }
	_crInt32 getEquipMagic(){ return m_equipmagic; }
	///////////临时数据
	void setItemDeductCount(int deductcount){ m_deductcount = deductcount; }
	int getItemDeductCount() { return m_deductcount; }
	
protected:
	int m_itemid;
	unsigned char m_count;
	//unsigned char m_jiangkadata;
	unsigned char m_flg;//0, 1:编制中
	_crInt32 m_equipmagic;//装备的(refid0,refid1,refid2,inlayid)
	int m_deductcount;//临时数据
	
};
class crStoreBagItemData:public crBagItemData
{
public:
	crStoreBagItemData():m_serverid(0){}
	//void setItemGameID(int gameid){m_gameid = gameid;}
	//int getItemGameID(){return m_gameid;}
	void setItemServerID(int serverid){m_serverid = serverid;}
	int getItemServerID(){return m_serverid;}
	void setItemHasCycle(char bcycle){m_isCycle = bcycle;}
	char getItemHasCycle(){return m_isCycle;}
private:
	//int m_gameid;
	int m_serverid;
	char m_isCycle;//0,1
};

typedef std::map< int,CRCore::ref_ptr<CRCore::crDataObject> > ItemObjectMap;//itemid,dataobject
typedef std::vector< CRCore::ref_ptr<crBagItemData> > JXJItemBagVec;//itemid,count//图纸、材料、装备等
typedef std::vector< CRCore::ref_ptr<crStoreBagItemData> > JXJStoreItemBagVec;//itemid,count//图纸、材料、装备等
typedef std::vector< CRCore::ref_ptr<crAppointCardData> > JXJAppointCardVec;//itemid,time//委任将卡列表
typedef std::vector< int > JXJItemBagVec2;//itemid//装备、将卡等
typedef std::vector< std::pair<int,_crInt32> > JXJLordEquipVec;//itemid,equipmagic领主身上装备
//typedef std::map< unsigned char,unsigned char > JXJLordSuitMap;//套装id，套装数量
class crSaleItemData : public CRCore::Referenced
{
public:
	crSaleItemData():m_itemid(0),m_count(0),m_time(0),m_equipmagic(0){}
	void setItemID(int itemid){ m_itemid = itemid; }
	int getItemID() { return m_itemid; }
	void setEquipMagic(_crInt32 equipmagic){ m_equipmagic = equipmagic; }
	_crInt32 getEquipMagic() { return m_equipmagic; }
	void setItemCount(unsigned char count){ m_count = count; }
	unsigned char getItemCount() { return m_count; }
	void setTime(unsigned short time){ m_time = time; }
	unsigned short getTime() { return m_time; }
protected:
	int m_itemid;
	_crInt32 m_equipmagic;//
	unsigned char m_count;
	unsigned short m_time;
};
typedef std::list< CRCore::ref_ptr<crSaleItemData> >JXJSaleItemList;
class crFormationInfo : public CRCore::Referenced
{
public:
	crFormationInfo():m_roleid(0),m_abstractid(0),m_count(0){}
	void setRoleID(int roleid){ m_roleid = roleid; }
	int getRoleID(){ return m_roleid; }
	void setAbstractID(int abstractid){ m_abstractid = abstractid; }
	int getAbstractID() { return m_abstractid; }
	void setCount(unsigned short count) { m_count = count; }
	unsigned short getCount() { return m_count; }
	void setEquips(const CRCore::crVector2i &equips) { m_equips = equips; }
	CRCore::crVector2i &getEquips() { return m_equips; }
protected:
	int m_roleid;
	int m_abstractid;
	unsigned short m_count;
	CRCore::crVector2i m_equips;//将卡itemid
};
typedef std::vector< CRCore::ref_ptr<crFormationInfo> >FormationInfoVec;
typedef std::vector< std::pair<std::string, FormationInfoVec> >SavedFormationInfoVec;//name，formationvec
class crTeamInfo : public CRCore::Referenced
{
public:
	crTeamInfo():m_leaderid(0),m_leaderlv(0),m_count(0){}
	void setLeaderID(int playerid){ m_leaderid = playerid; }
	int getLeaderID() { return m_leaderid; }
	void setLeaderName(const std::string &name) { m_leaderName = name; }
	std::string &getLeaderName() { return m_leaderName; }
	void setLeaderLevel(unsigned char lv){ m_leaderlv = lv; }
	unsigned char getLeaderLevel() { return m_leaderlv; }
	void setCount(char count) { m_count = count; }
	char getCount() { return m_count; }
protected:
	int m_leaderid;//队长ID
	std::string m_leaderName;
	unsigned char m_leaderlv;
	char m_count;//成员数量
};
typedef std::vector< CRCore::ref_ptr<crTeamInfo> >TeamVec;
class crTeamMember : public CRCore::Referenced
{
public:
	crTeamMember():m_playerid(0),m_level(0),m_isready(false){}
	void setIconID(unsigned char iconid){ m_iconid = iconid; }
	unsigned char getIconID() { return m_iconid; }
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
	void setName(const std::string &name) { m_name = name; }
	std::string &getName() { return m_name; }
	void setLevel(unsigned char lv){ m_level = lv; }
	unsigned char getLevel() { return m_level; }
	void setReady(bool bReady) {m_isready = bReady;}
	bool isReady(){return m_isready;}
protected:
	int m_playerid;
	std::string m_name;
	unsigned char m_level;
	unsigned char m_iconid;
	bool m_isready;
};
typedef std::list< CRCore::ref_ptr<crTeamMember> >TeamMemberVec;
class crFriend : public CRCore::Referenced
{
public:
	crFriend():m_playerid(0),m_level(0),m_ifonline(false){}
	crFriend(crFriend *fnd)
	{
		m_playerid = fnd->m_playerid;
		m_name = fnd->m_name;
		m_level = fnd->m_level;
		m_iconid = fnd->m_iconid;
		m_ifonline = fnd->m_ifonline;
	}
	void setIconID(unsigned char iconid){ m_iconid = iconid; }
	unsigned char getIconID() { return m_iconid; }
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
	void setName(const std::string &name) { m_name = name; }
	std::string &getName() { return m_name; }
	void setLevel(unsigned char lv){ m_level = lv; }
	unsigned char getLevel() { return m_level; }
	void setOnline(bool ifonline){ m_ifonline = ifonline; }
	bool getOnline() { return m_ifonline; }
protected:
	int m_playerid;
	std::string m_name;
	unsigned char m_level;
	unsigned char m_iconid;
	bool m_ifonline;/////客户端显示用
};
typedef std::list< CRCore::ref_ptr<crFriend> >FriendVec;
typedef std::set<std::string> BlackNameSet;
class crSale : public CRCore::Referenced
{
public:
	crSale():m_saleListID(-1),m_goodsType(0),m_goodsID(0),m_price(0),m_count(0),m_prepareTimer(0),m_saleid(0),m_saletimer(0),m_salestate(CRNetApp::Sale_Prepare),m_gaincash(0){}
	void setSaleListID(unsigned char id){ m_saleListID = id; }
	char getSaleListID(){ return m_saleListID; }
	void setGoodsType(int gtype){ m_goodsType = gtype; }
	int getGoodsType() { return m_goodsType; }
	void setGoodsID(int id){ m_goodsID = id; }
	int getGoodsID() { return m_goodsID; }
	void setPrice(int price){ m_price = price; }
	int getPrice() { return m_price; }
	void setCount(int count){ m_count = count; }
	int getCount() { return m_count; }
	short& prepareTimer(){ return m_prepareTimer; }

	void setSaleID(int saleid){ m_saleid = saleid; }
	int getSaleID(){ return m_saleid; }
	int& saleTimer(){ return m_saletimer; }
	void setSaleState(unsigned char s) { m_salestate = s; }
	unsigned char getSaleState() { return m_salestate; }
	void setGaincash(int cash){ m_gaincash = cash; }
	int getGaincash(){ return m_gaincash; }
protected:
	char m_saleListID;//
	int m_goodsType;
	int m_goodsID;
	int m_price;
	int m_count;
	short m_prepareTimer;//计时器
	int m_saleid;
	int m_saletimer;
	unsigned char m_salestate;
	int m_gaincash;
};
typedef std::list< CRCore::ref_ptr<crSale> > SaleList;
class crFubenInfo : public CRCore::Referenced
{
public:
	crFubenInfo():m_fubenprogress(0),m_fubencomplete(0),m_starcount(0),m_chapterid(0){}
	void setFubenProgress(short fubenprogress){ m_fubenprogress = fubenprogress; }
	short getFubenProgress() { return m_fubenprogress; }
	void setComplete(unsigned char fubencomplete){ m_fubencomplete = fubencomplete; }
	unsigned char getComplete() { return m_fubencomplete; }
	void setStarCount(unsigned char starcount){ m_starcount = starcount; }
	unsigned char getStarCount() { return m_starcount; }
	void setChapterId(int chapterid){ m_chapterid = chapterid; }
	int getChapterId() { return m_chapterid; }
protected:
	short m_fubenprogress;//副本进度
	unsigned char m_fubencomplete;//0.未通关，1.已通关次数
	unsigned char m_starcount;//星级
	int m_chapterid; //副本chapterid
};
typedef std::map< unsigned short, CRCore::ref_ptr<crFubenInfo> > FubenProgressMap;//副本ID，副本进度,通关标志符,星标
class crExtraProduce : public CRCore::Referenced
{
public:
	crExtraProduce(){}
	//void setType(unsigned char type){ m_type = type; }
	//unsigned char getType() { return m_type; }
	int &timer() { return m_timer; }
	void setPercent(short percent) { m_percent = percent; }
	short getPercent() { return m_percent; }
protected:
	//unsigned char m_type;//T_Food T_Wood T_Iron T_Horse T_Copper
	int           m_timer;
	short         m_percent;
};

enum JXJChatMode
{
	ChatAll,		//全部
	ChatInWorld,	//世界
	ChatInCountry,  //国聊
	ChatInGroup,    //团聊
	ChatInTeam,     //队聊
	ChatOneToOne,  //私聊
	ChatNull
};
class crJXJChatRecord : public CRCore::Referenced
{
public:
	crJXJChatRecord():m_shiliID(0){}
	void setSenderName(const std::string &name){ m_senderName = name; }
	const std::string& getSenderName(){ return m_senderName; }
	void setChatText(const std::string &chattext){ m_chattext = chattext; }
	const std::string& getChattext(){ return m_chattext; }
	void setSenderShiliID(const unsigned char shili){m_shiliID = shili;}
	unsigned char getSenderShiliID(){return m_shiliID;}
	void setTextColor(CRCore::crVector4 color){m_color = color;}
	CRCore::crVector4 getTextColor(){return m_color;}
	std::string & getChatChannelName(){return m_type;}
	void setChatChannelName(std::string type){m_type = type;}
	std::string &getReciverName(){return m_reciverName;}
	void setReciverName(std::string reciverName){m_reciverName = reciverName;}
protected:
	std::string m_senderName;
	std::string m_reciverName;
	std::string m_chattext;
	unsigned char m_shiliID;
	CRCore::crVector4 m_color;
	std::string  m_type;
};

class crJXJPersuadeInfo : public CRCore::Referenced
{
public:
	crJXJPersuadeInfo():m_fubenid(0),m_canFinishTime(-1),m_level(0){}
	void setFubenId(unsigned short fubenid){m_fubenid = fubenid;}
	unsigned short getFubenId(){return m_fubenid;}
	void setTime(_crInt64 time){m_canFinishTime = time;}
	_crInt64 getTime(){return m_canFinishTime;}
	void setLevel(unsigned char setlv){m_level = setlv;}
	unsigned char getLevel(){return m_level;}
	void setJKId(int setjkid){m_jkid = setjkid;}
	int getJKId(){return m_jkid;}
protected:
	unsigned short m_fubenid;
	//_crInt64 m_time;
	_crInt64 m_canFinishTime;//服务器端存储完成时间客户端存储倒计时时间
	unsigned char m_level;
	int m_jkid;
};
struct BattleReportThree
{
	int first;
	int second;
	int third;
};
struct PlayerBattleReport
{
	BattleReportThree info;
	int experience;//jingyan
	int exploit;//功勋
};
typedef std::map<std::string, PlayerBattleReport> PlayerBattleReportInfoMap;//playerName，总投入

struct strZhengzhaoInfo :public CRCore::Referenced
{
	unsigned char m_type; //征召令类型（国家或军团）
	std::string m_publisher; //发布者
	std::string m_chengchiName;
	_crInt64 m_battleroomid;
	int m_sceneid;
	unsigned short m_timer;
	std::string m_liuyan;
	unsigned char m_battletype; //战场类型
	strZhengzhaoInfo()
	{
		m_type = 0; 
		m_publisher = "";
		m_chengchiName = "";
		m_battleroomid = 0;
		m_sceneid = 0;
		m_timer = 0;
		m_liuyan = "";
		m_battletype = 0;
	}
};
typedef std::vector<CRCore::ref_ptr<strZhengzhaoInfo> > ZhengzhaolingVec;		// 征召令信息(在线使用)，不存数据库

class crJXJBattleReportInfo : public CRCore::Referenced
{
public:
	crJXJBattleReportInfo():m_battleid(0),m_isAttackWin(false){
		m_total.first = 0; m_total.second = 0; m_total.third = 0;
		m_totalDead.first = 0; m_totalDead.second = 0; m_totalDead.third = 0;
	}

	void setBattleId(int battleid){ m_battleid = battleid; }
	int getBattleId(){ return m_battleid; }

	void setAttackWin(bool win){ m_isAttackWin = win; }
	bool getAttackWin(){ return m_isAttackWin; }
	void setBattleName(const char * str){ m_strBattleName = str; }
	std::string & getBattleName(){ return m_strBattleName; }

	void setTotal(BattleReportThree total){ m_total = total; }
	BattleReportThree& getTotal(){ return m_total; }

	void seTotalDead(BattleReportThree &total){ m_totalDead = total; }
	BattleReportThree & geTotalDead(){ return m_totalDead; }

	PlayerBattleReportInfoMap & getReportPlayerInfoMap(){ return m_playerBattleReportInfo; }
	
protected:
	int m_battleid;
	bool m_isAttackWin;
	std::string m_strBattleName;
	BattleReportThree m_total;//总，攻防总，守方总
	BattleReportThree m_totalDead;//总死，攻防，守方
	PlayerBattleReportInfoMap m_playerBattleReportInfo;
};

class GroupCantonSpecialty : public CRCore::Referenced
{
public:
	GroupCantonSpecialty():m_itemtype(0),m_itemid(0),m_itemnum(0),m_contribute(0){}
	int m_itemtype;
	int m_itemid;			//镇守、驻守州的特产id
	int m_itemnum;		
	int m_contribute;
};
typedef std::vector<CRCore::ref_ptr<GroupCantonSpecialty>> GroupCantonSpecialtyVec;	//军团城池管理界面的8个物品

typedef std::map<int, crJXJBattleReportInfo> BattleReportInfoMap;//战报id

typedef std::map< unsigned char, CRCore::ref_ptr<crExtraProduce> > ExtraProduceMap;//JXJThingType,timer,Percent
typedef std::deque< std::pair< int,CRCore::ref_ptr<CRCore::crStreamBuf> > >ClientDesposeEventDeque;
typedef std::deque< CRCore::ref_ptr<CRCore::crStreamBuf>> ChatRecordDeque;
typedef std::map<JXJChatMode, ChatRecordDeque> ChatRecordMap;
typedef std::vector<unsigned short> RebelCityVec;
typedef std::map<unsigned short,CRCore::crVector2i> PersuadeTimeMap;//fubenid,将卡ID，倒计时时间
typedef std::deque< CRCore::ref_ptr<crJXJPersuadeInfo> > PersuadeInfoDeq;//说服信息
typedef std::vector<unsigned char> PlayerIconVec;
typedef std::set<int> SelectLeaderIdSet;									// 玩家投票的集合
typedef std::set<int> HadWonGroupFuBenSet;									// 玩家 已经通关的副本ID
typedef std::vector<int> SelectedIdVec;										// 玩家选择的候选人的集合，不存数据库
typedef std::map<int,_crInt64>	ItemUsedTimeMap;							// 物品最后使用时间Map
typedef std::map<unsigned char,unsigned char> AchieveShopMap;			// (achieveshop.tabid,已购买次数)成就商店物品购买次数记录，只记录有购买次数限制的商品
typedef std::map<unsigned char,unsigned char> GovShopMap;			// (itemid,已购买次数)商店物品购买次数记录，只记录有购买次数限制的商品

typedef std::deque < std::pair<int,std::pair<int,std::string> > > AddGroupMemberRequestDeq; // groupid ,playerid ,groupName
//typedef std::vector<CRCore::crVector3i> LunHuiRewardVec;		// id，magicid，count
typedef std::multimap<unsigned char,std::pair<unsigned char,CRCore::crVector3i>> LunHuiRewardMap; //等级，是否领取标识符（1领取，0为领取） 轮回奖励Vector、
typedef std::set<int> MyGroupApplySet;
typedef std::map<int, CRCore::crVector3i> TroopsAchieveMap; //玩家兵种相关成就信息,vector[0]为击杀玩家此兵种数量,vector[1]为玩家此兵种死亡数量,vector[2]为玩家此兵种击杀数量
class crPlayerRewardData;
struct InviteInfo
{
	unsigned char type;
	unsigned short id;
	int playerid;
};
struct CallForHelpInfo
{
	int secneid;
	int roomid;
};
typedef std::bitset<64>  BitSet64;

class crJXJPlayerDataMonitor : public CRCore::Referenced
{
public:
	crJXJPlayerDataMonitor();
	void setGoldingot(int goldingot);
	int getGoldingot();
	void setGiftgold(int giftgold);
	int getGiftgold();
	void setVipExp(int vipExp);
	int getVipExp();
	void setCoppercash(int coppercash);
	int getCoppercash();
	void setFood(int food);
	int getFood();
	void setWood(int wood);
	int getWood();
	void setIron(int iron);
	int getIron();
	void setHorse(int horse);
	int getHorse();
	void setLevel(unsigned char lv);
	unsigned char getLevel();
	void setExploit(int exploit);
	int getExploit();
	void setTroops(int troops);
	int getTroops();
	void setManorlv(unsigned short manorlv);
	unsigned short getManorlv();
protected:
	int m_goldingot;//元宝
	int m_giftgold;//礼金
	int m_vipExp;//vip经验
	int m_coppercash;//铜钱
	int m_food;//粮食
	int m_wood;//木材
	int m_iron;//铁
	int m_horse;//马匹
	unsigned char m_level;//等级
	int m_exploit;//功勋
	int m_troops;//兵力
	unsigned short m_manorlv;//建筑总等级
};
const int MAX_EQUIP_BIND = 4;//装备绑定上限;
class crJXJPlayerData : public CRCore::crData
{//server & client
public:
	crJXJPlayerData();
	crJXJPlayerData(const crJXJPlayerData& data);
	DataClass(JXJ, JXJPlayer)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);

protected:
	virtual ~crJXJPlayerData();
	int m_playerid;//Server用，不存数据库
	bool m_playersex;//0女，1男
	unsigned char m_playericonid;//头像ID
	unsigned char m_shiliid;//势力ID
	unsigned short m_homeid;//封地
	unsigned short m_chengchiid;//封地所在城池
	unsigned char	m_feteTimes;					// 国家界面祭祀次数
	unsigned char m_enrichType;						// 国家界面富国类型
	unsigned char m_enrichTimes;					// 国家界面富国次数

	unsigned char m_groupContributeiType;			// 军团捐献类型
	unsigned char m_groupContributeiTimes;			// 军团捐献次数
	int m_goldingot;//元宝
	int m_giftgold;//礼金
	//资源
	int m_coppercash;//铜钱
	int m_food;//粮食
	int m_wood;//木材
	int m_iron;//铁
	int m_horse;//马匹
	//驻地建筑等级
	unsigned char m_fuyalv;//府衙
	unsigned char m_bingyinglv;//兵营
	unsigned char m_jiusilv;//酒肆
	unsigned char m_gongfanglv;//工坊
	unsigned char m_shijilv;//市集
	unsigned char m_fukulv;//府库
	_crInt64 m_mingjulv;//民居unsigned char[8]
	_crInt64 m_nongtianlv;//农田unsigned char[8]
	_crInt32 m_muchanglv;//牧场unsigned char[4]
	_crInt32 m_lingdilv;//林地unsigned char[4]
	_crInt32 m_kuangshanlv;//矿山unsigned char[4]
	_crInt64 m_updateTimer;//上次数据更新记时,数据库记录，不需要传给客户端 time_t 秒
    CRCore::crVector4i m_buildingTimes;//秒
	CRCore::crVector4i m_recruitTimes;//秒
	unsigned short m_junling;//军令 /100
	//预备部队
	//unsigned char m_maxtroops;//预备部队编队总数
	TroopsMap m_troopsMap;
	TroopsEquipMap m_troopsEquipMap;
	// 背包列表
	JXJItemBagVec m_itembagvec;
	//科技等级
	//unsigned char m_infantrylv;//步兵科技等级
	//unsigned char m_cavalrylv;//骑兵科技等级
	//unsigned char m_archerlv;//弓箭兵科技等级
	//unsigned char m_siegelv;//攻城科技等级
	//unsigned char m_shieldlv;//盾等级
	//unsigned char m_guardlv;//护卫等级
	//unsigned char m_saddlelv;//马铠等级
	//unsigned char m_assaultlv;//突击等级
	//unsigned char m_assistlv;//协战等级
	//unsigned char m_penetratelv;//穿透等级
	//unsigned char m_manufacturelv;//制造等级
	//unsigned char m_gunpowderlv;//火药等级
	//unsigned char m_bravelv;//虎贲等级
	//unsigned char m_ironcavalrylv;//铁骑等级
	//unsigned char m_gearlv;//机关等级
	//unsigned char m_fireboltlv;//霹雳等级
	//unsigned char m_masterlv;//熟练兵等级
	//unsigned char m_juntunlv;//军屯等级
	//unsigned char m_reformlv;//编制改革等级
	////签到次数
	//unsigned char m_signin;
	FormationInfoVec m_formationInfoVec;
	//信息提示
	unsigned int m_goldcosthint;//消费提示
	//好友列表
	FriendVec m_friendList;
	//玩家组队
	TeamMemberVec m_myTeam;//跟玩家sceneid联系，玩家退出scene组队撤销
	TeamVec m_queryTeamVec;//客户端用
	int m_teamLeaderID;
	//char m_queryTeamIndex;
	////将卡刷新计时器
	//short m_halltimer;//大厅
	//int m_privatetimer;//雅间
	//int m_hallCard[5];
	//int m_privateCard[3];
	//征税
	unsigned char m_levyCount;
	//int m_levyTimer;
	/////在线奖励计时器
	unsigned char m_onlineRewardCount;//剩余在线奖励次数
	_crInt64 m_onlineRewardTimer;//上一次领取奖励的时间
	////客户端在线奖励临时变量
	int m_playerOnlineRewardTimer;
	//军令
	char m_buyJunlingCount;
	unsigned short m_buyJunlingTimer;
	//寄售
	SaleList m_saleList;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;
	GNE::Mutex m_dataMutex;
	static GNE::Mutex m_saveMutex;
	unsigned char m_utemp[8];
	////GameServerData
	bool m_openBattle;//开启城战
	int m_battleTimer;//不存数据库
	//bool m_openPass;// 开启关隘
	//short m_passTimer;
	ChengchiMap m_chengchiMap;//玩家刚登录的时候服务器发给玩家,只客户端有效
	//邀请者信息
	unsigned char m_inviteType;
	int m_inviteID;
	unsigned short m_inviteChengchiID;//城池id或者副本id
	//城战
	unsigned char m_fubenOrChengchiType;//0：无，1：城站 2：副本 3:世界战场
	unsigned short m_fubenOrChengchiID;//要进的副本ID或者城池ID
	//unsigned short m_battleChengchiID;//要进的城池ID，临时数据
	//unsigned short m_fubenID;//副本ID，临时数据
	_crInt64 m_battleRoomID;//battleid(攻防势力ID，守方势力ID，城池ID),roomid
	short m_curFubenProgress;//SceneServer运行时数据
	//已开启包裹的格子数
	unsigned char m_openbagnum;
	//最近出售列表
	JXJSaleItemList m_saleitemlist;
	// 将卡背包列表
	JXJItemBagVec m_jiangkabagvec;
	FubenProgressMap m_fubenProgressMap;
	//道具的DataObject，客户端服务器都有，不需要同步
	ItemObjectMap m_itemObjectMap;

	ExtraProduceMap m_extraProduceMap;//需要存服务器
	//unsigned char m_jingbinglv;//精兵
	//unsigned char m_jianjialv;//坚甲
	//unsigned char m_qiangrenlv;//强韧
	//unsigned char m_guimoulv;//鬼谋
	//unsigned char m_gongchenglv;//攻城
	//unsigned char m_yuangonglv;//远攻
	//unsigned char m_sugonglv;//速攻
	//unsigned char m_qiangxinglv;//强行
	//unsigned char m_zhaomulv;//招募
	//unsigned char m_rendelv;//仁德
	JXJAppointCardVec m_appointjiangkabagvec;
	//unsigned char m_luckycardcoppercost;//抽取将卡铜钱增加次数
	//unsigned char m_luckycardgoldcost;//抽取将卡元宝增加次数
	//unsigned char m_knowluckycard;//知天命元宝增加次数
	int m_luckycard[3];//抽取将卡
	//int m_jiusirefreshtime;//酒肆抽取将卡价格刷新时间
	unsigned char m_viplv;//军师等级
	int m_vipExp;//军师等级成长值
	CRCore::ref_ptr< crPlayerRewardData > m_fubenReward;//副本奖励数据
	unsigned char m_reliveCount; //副本补兵次数
	ClientDesposeEventDeque m_clienteventdeq;//
	int m_countrycontribute;//国家贡献
	ChatRecordMap m_chatRecordMap;//客户端聊天信息
	std::map<int,std::pair<int,int>> m_chatitemlinkIdMap;		//客户端聊天物品linkid
	std::map<int,InviteInfo> m_chatinviteIdMap;				//客户端聊天邀请linkid
	std::map<int, std::pair<std::string, int>>m_chatBattleReportIDMap;//客户端聊天战报林肯id
	std::map<int, int>m_chatPraiseIDMap;//客户端聊天点赞链接
	std::map<int, std::pair<int, int>> m_chatCallForHelpLinkIdMap;//客户端聊天求助连接
	unsigned short m_cardimpression;//抽将卡好感度
	int m_cardpieces;//将魂
	unsigned char m_freeCounterinsurgency;//免费平叛次数
	unsigned short m_RefineEquipFreeCount;//免费精炼次数 /100
	RebelCityVec m_rebelCityVec;//叛乱城池ID 客户端临时变量
	unsigned short m_rebelTimer;//客户端临时变量叛乱倒计时时间
	unsigned char m_NongTianShuiLilv;//农田水利
	unsigned char m_ZhiShuZaoLinlv;//植树造林
	unsigned char m_YouZhiCaoChanglv;//优质草场
	unsigned char m_JingKuangShenWalv;//精矿深挖
	unsigned char m_GuLiMaoYilv;//鼓励贸易
	unsigned char m_ZhengShouXinDelv;//征收心得
	unsigned char m_YanBingYueWulv;//演兵阅武
	unsigned char m_XunBingXinDelv;//训兵心得
	unsigned char m_ZongMiaoJiSilv;//宗庙祭祀
	unsigned char m_JiSiXinDelv;//祭祀心得
	int m_MarketBuyResourceMax;//市集每日已购买量
	_crInt32 m_refinemagicid;//精炼ID
	CRCore::ref_ptr<CRCore::crStreamBuf> m_cfgstream;//中央公告
	CRCore::ref_ptr<CRCore::crStreamBuf> m_oldcfgstream;//中央公告
	unsigned short m_FreeRollCardCount;//免费精炼次数 /100
	CRCore::ref_ptr<crJXJPlayerStore> m_playerStore;//轮回仓库
	short m_playerCycleCount;//玩家轮回次数此数据在playergamestore数据库表中玩家上线是查询同步给玩家
	unsigned short m_autosellcard;//自动出售将卡【白，绿，蓝】
	TroopsTechMap m_troopstechmap;//兵种科技||工坊科技
	CardExtraTechMap m_cardExtraTechMap;//组合将卡科技等级
	unsigned char m_CardImpression2CritTechLv;//好感度暴击科技//2
	unsigned char m_CardImpression4CritTechLv;//好感度暴击科技//4
	unsigned char m_CardImpression6CritTechLv;//好感度暴击科技//6
	unsigned char m_CardImpression10CritTechLv;//好感度暴击科技//10
	unsigned char m_FreeRollCardTechLv;//免费刷将科技16
	unsigned char m_OrangeCardUpgradeTechLv;//橙卡升星科技
	unsigned char m_CardShopOpenTechLv;//将卡商店开启科技
	unsigned char m_MakeEquipTechLv;//装备打造科技
	unsigned char m_EquipRefineTechLv;//装备精炼科技
	unsigned char m_EquipAttachTechLv;//附炼科技
	unsigned char m_SmeltEquipTechLv;//熔炼科技//开启装备熔炼
	unsigned char m_SmeltMaterialTechLv;//熔炼科技//开启材料熔炼
	unsigned char m_SmeltStoneTechLv;//熔炼科技//开启八卦石熔炼
	unsigned char m_EquipRefineMaxCount1TechLv;//装备精炼科技免费次数12
	unsigned char m_EquipRefineMaxCount2TechLv;//装备精炼科技免费次数16
	unsigned char m_EquipRefineMaxCount3TechLv;//装备精炼科技免费次数20
	unsigned char m_EquipRefineMaxCount4TechLv;//装备精炼科技免费次数24
	unsigned char m_EquipRefineProduce1TechLv;//装备精炼科技回复50%
	unsigned char m_EquipRefineProduce2TechLv;//装备精炼科技回复100%
	unsigned char m_EquipRefineProduce3TechLv;//装备精炼科技回复150%
	unsigned char m_EquipRefineProduce4TechLv;//装备精炼科技回复200%
	unsigned char m_FreeRollCard2TechLv;//免费刷将科技20
	unsigned char m_FreeRollCard3TechLv;//免费刷将科技24
	unsigned char m_FreeRollCardProduce1TechLv;//免费刷将回复科技
	unsigned char m_FreeRollCardProduce2TechLv;//免费刷将回复科技
	unsigned char m_FreeRollCardProduce3TechLv;//免费刷将回复科技
	unsigned char m_FreeRollCardProduce4TechLv;//免费刷将回复科技
	unsigned char m_FuKuMaxTechLv;//府库最大值科技20
	unsigned char m_FuKuMaxTech2Lv;//府库最大值科技5
	unsigned char m_FuKuMaxTech3Lv;//府库最大值科技10
	BlackNameSet m_blacknameset; 
	TechDevelopMap m_techdevelopmap;//研发中的科技
	bool m_bisCycling;//是否在轮回（客户端临时使用）
	int m_iClientCycleTime;//轮回时间（客户端临时使用）
	//PersuadeInfoDeq m_persuadeInfoDeq;//说服
	SavedFormationInfoVec m_savedformationinfovec;//编队保存切页
	unsigned char m_opensavedformationifo;//编队保存切页开启
	unsigned char m_changeShiliTimes;//主动叛国次数
	unsigned short m_seriesLandDays;//连续登录时间
	unsigned short m_loginDays;		// 登陆天数
	unsigned char m_LandRewardGetCount;//登陆抽奖次数
	CRCore::ref_ptr< crPlayerRewardData > m_lanRewardData;//登陆奖励服务器缓存（不存数据库，不发客户端）
	int m_nLandRewardTime;//登陆奖励客户端倒计时(只客户端使用)
	LunHuiRewardMap m_lunhuiRewardMap;
	//PlayerIconVec m_playerBuyIconVec;
	int m_nLunHuiRewardGiftGold;//轮回奖励礼金
	AddGroupMemberRequestDeq m_addGroupMemberRequestDeq;
	bool m_skipnewhand;//是否在轮回（客户端临时使用）

	int m_consortiaid;
	MyGroupApplySet m_myGroupApplySet;	// 申请的军团 id
	SelectLeaderIdSet m_selectLeaderIdSet;						//投票记录
	unsigned char m_savedformationindex;//编队保存切页页码
	unsigned char m_iftechbtnnew;//科技图标是否闪烁

	HadWonGroupFuBenSet m_hadWonGroupFuBenSet;					// 已经通关的军团副本id集合

	unsigned char m_inspireLv;//客户端和服务器临时数据不存数据库，玩家退出战场是此数据清除

	CRCore::ref_ptr<crJXJShili> m_myShili;// 客户端使用，从braindata移过来，防止进出副本被清掉

	BitSet64 m_chatChannelSetInfo;//玩家聊天信息设置信息
	//int m_serverVersion;			// 记录服务器版本号，用于【需求】玩家在版本更新后第一次登陆的时候弹出版本更新界面

	int m_flushTime;
	NewFunctionOpenSet m_newfunctionopenset;//新功能开启提示set，已提示过的在set中做记录
	BattleReportInfoMap m_battleReportInfoMap;//客户端存储玩家已查询的战报数据
	int m_battleReportQueryedId;//客户端存储玩家当前查询战报ID
	unsigned char m_lingqufengluflag;//领取俸禄标识符
	//unsigned char m_mainrolerefreshflag;//是否刷新mainrole（0点刷新使用）服务器用，不需要同步给客户端
	unsigned char m_tuntianlv;//屯田(粮食)
	unsigned char m_yelianlv;//冶炼（铁矿）
	unsigned char m_fanzhilv;//繁殖（马匹）
	unsigned char m_fanmaolv;//繁茂（木材）
	unsigned char m_qiaojianglv;//巧匠（建筑CD）
	unsigned char m_zhengshuilv;//征税
	unsigned char m_techfenglulv;//俸禄科技
	unsigned char m_techjiusibaojilv;//酒肆暴击科技
	unsigned char m_techworldtalklv;//世界发言科技
	unsigned char m_techbuyjunlinglv;//买军令的科技

	unsigned char m_foundCountryAward;	// 建国系统奖励领取情况，位存储
	int m_playertotalpaygold;//玩家累计充值元宝数

	unsigned char m_shuichelv;//水车：增加农田产量
	unsigned char m_ronglulv;//熔炉：增加铁矿产量
	unsigned char m_gongshelv;//工社：增加林地产量
	unsigned char m_mochuanlv;//秣船：增加牧场产量
	unsigned char m_7DaysAward;			// 7日登陆奖励领取情况，位存储
	unsigned char m_shouchong;//0:表示没有，1表示首充，2表示已经领取首充奖励

	//月卡
	unsigned short m_yuekaid;//0:表示当前没有月卡，每个玩家同一时期内只会有一种月卡
	_crInt64 m_yuekaendtime;
	_crInt64 m_lastgettime;//最近一次领取时间
	GroupCantonSpecialtyVec m_groupCantonSpecialtyVec;
	ItemUsedTimeMap m_itemusedtimeMap;	//物品最后一次使用时间	itemid , time

	bool m_hadwonjuntuanfubenT;	//今日是否通关过军团副本
	bool m_hadshowdismisstip;//是否提示过军团解散时间. 仅客户端使用
	_crInt64 m_kaoshangfutime; //最后一次使用犒赏令的时间戳
	unsigned char m_specialtyrefreshtimes;	//州郡收益刷新次数
	_crInt64 m_lastchongzhitime;	//连续充值活动 最后一次有效充值的时间
	_crInt64 m_lastzeroRefreshtime;	//最后一次零点刷新的时间
	int m_chongzhicountT;			//今天充值数量		

	//周卡
	int      m_gainedzhoukahuodongid;//我当前获得的周卡活动ID
	_crInt64 m_zhoukaendtime;
	unsigned char m_zhoukagetstatus;//周卡领取状态
	int m_zhoukagetdayidx;//周卡当前领取的是第几天

	int m_cZhengZhanQuan; //玩家拥有征战券数量
	int m_cGuoZhanJiFen;  //玩家拥有国战积分
	int m_treasonbuffertimer;   //叛国缓冲时间
	int m_treasonpunishtimer;   //叛国惩罚期
	int m_treasoncoolingtimer;  //叛国冷却期
	unsigned char m_selectedshili;   //玩家叛国时选择的势力
	unsigned char m_guanzhi;  //1 丞相，2 大司马，3 大将军
	std::map<int,unsigned char> m_buyshagnxianmap; //玩家每日在征战商店购买物品数量
	unsigned char m_timesshangwu;   //世界战场上午活动已领取次数
	unsigned char m_timesxiawu;   //世界战场下午活动已领取次数
	unsigned char m_timeswanshang;   //世界战场晚上活动已领取次数
	unsigned char m_praiselimit;   //玩家在展示中被赞的次数
	int m_praisedcount;   //玩家被赞的次数
	int m_praisecount;   //玩家赞别人的次数
	TroopsAchieveMap m_troopsachievemap; //玩家兵种相关成就信息,vector[0]为玩家击杀此兵种数量,vector[1]为玩家此兵种死亡数量,vector[2]为玩家此兵种击杀数量
	int m_killplayercount; //玩家杀死玩家数量
	int m_killnpccount; //玩家杀死npc数量
	int m_playerdeadcount; //玩家死亡数量
	unsigned short m_passmingjiangcount; //玩家通过名将之路次数
	CRCore::crVector4i m_ckilleachtroops;//玩家杀死各兵系兵数(弓步骑械)
	CRCore::crVector4i m_cdeadeachtroops;//玩家死亡各兵系兵数(弓步骑械)
	///每日杀敌数
	int m_dayplayerdeadcount; //每日玩家死亡数量
	int m_daykillplayercount; //每日玩家杀死玩家数量
	int m_daykillnpccount; //每日玩家杀死npc数量
	CRCore::crVector4i m_dayckilleachtroops;//每日玩家杀死各兵系兵数(弓步骑械)
	//CRCore::crVector4i m_daycdeadeachtroops;//每日玩家死亡各兵系兵数

    unsigned short m_Cpassworldfuben;    //玩家通过世界战场次数
	unsigned char m_cmingjiangfail;   //玩家在名将之路中失败的次数
	unsigned char m_cfinishdays;   //玩家连续完成活跃度的天数
	bool m_huoyuedusign;           //是否检测活跃度达到目标点标志
	int m_accomplishPointCache;		//成就点货币
	int m_usedachievepoint;           //已经使用的成就点
	unsigned char m_ctianxiafail;   //玩家在名将之路中失败的次数
	unsigned short m_Cpasstianxia;    //玩家通过征战天下次数
	CRCore::crVector4i m_danchangkill;//玩家单场国战击杀各兵系兵数
	CRCore::crVector4s m_killdoorboss;//玩家击杀的城门、正厅、箭塔和BOSS
	unsigned char m_havegotviplv;   //玩家已经领取的vip等级礼包

	CRCore::ref_ptr<crJXJPlayerDataMonitor> m_playerDataMonitor;//玩家数据异常监控
	//战场位置变化记录，超过5分钟无操作则弹出验证提示
	float m_battleActiveTimer;
	unsigned short m_battleValidateID;//验证
	bool m_elector;//不存储数据库，是否具备选举资格
	unsigned char m_vote;//国家官员选举投票权重，不存储数据库
	bool m_beElected;//不存储数据库，是否具备选举资格
	bool m_recommendSet;//玩家是否已自荐
	unsigned char m_electIcon; //选举图标显示时间控制
	int m_equipBindInfo;//装备绑定编队信息;
	unsigned char m_isYellow;//是否黄钻
	unsigned char m_isYearYellow;//是否年费黄钻
	unsigned char m_yellowLevel;//黄钻等级
	
	//char       |char     | char        | char
	//黄钻每日礼包|年费黄钻每日|黄钻新手 |等级礼包;
	int m_yellowAwardInfo;//黄钻礼包信息  ;
	SelectedIdVec m_SelectedIdVec;
	int m_cancelCantonTimer; //州郡管理取消驻扎冷却期
	unsigned short m_cardImpressionLevel;//好感度奖励等级;
	JXJLordEquipVec m_friendEquipVec;//当前查看的好友领主装备，不存数据库	
	unsigned char m_resourceCount;//购买集市资源次数;(每天重置)
	unsigned char m_freeBuy;//市集30级后每日一次免费购买;
    unsigned char m_goldRecruitCount;//战场元宝复活(补兵)次数;
	unsigned char m_zhengZhaoTimes;//当前使用的国家征召次数（凌晨5点刷新）
	bool m_myself; //是否刷新为自己的装备信息，不存数据库
	unsigned char m_pageNum;//当前显示征召令的页数
	ZhengzhaolingVec m_guanyuanlingVec;   //国战征召令
	ZhengzhaolingVec m_juntuanlingVec;	  //军团征召令
	ZhengzhaolingVec m_CurZhengzhaolingVec;//当前窗口显示的征召令
	int m_zhengZhaolingUpdateTimer; //征召令最近刷新时间，不存数据库
	int m_GuoJiaReleaseTimer; //上一次发布国家征召的时间,不存数据库
	int m_JunTuanReleaseTimer; //上一次发布军团征召的时间，不存数据库
	bool m_CheckYingzhengReward;//检查是否发放应征奖励，不存数据库
	unsigned char m_attackVictoryTimes;//国战攻城胜利次数（对应成就：拔得头筹，战争狂人）
	unsigned char m_defendVictoryTimes;//国战守城胜利次数（对应成就：固若金汤，坚如磐石）
	unsigned short m_countryBattleTimes;//完成国战次数;
	std::string m_releaseName;//玩家应征的征召令发布者
	bool m_autoBattle;//是否开启自动战斗
	unsigned char m_curPageNum_Introduction;//新手指南当前显示页数
	char m_7DayData[7][5];//7日目标
	bool m_hadGottenReward;//已经获取军团开服活动奖励
	unsigned char m_myRankVal;//我所在的军团在军团开服活动中获得名次
	unsigned short m_startedDays; //开服天数 
	CouldGetAwardMap m_couldGetAwardMap;//可领取奖励（目前包括四种：1.俸禄；2.赋税；3.活跃度；4.酒肆免费刷将）客户端数据
	bool m_gameIntroductionNoTips;// 是否隐藏领地界面游戏宝典图标
	bool m_hadShowFestivalHuodongCanvas;// 今日是否已经自动显示节日活动窗口
	int m_onlinetimer;//一天内的累计在线时长，0点清零，防沉迷系统用,服务器用，不传客户端
	AchieveShopMap m_achieveShopMap;//成就商店商品购买次数记录
	GovShopMap m_govShopMap;//道具商店商品购买次数记录
	bool m_autoBuilding;//自动建筑升级
	char m_buildCountryTimes;//每天可进行国家建设的剩余次数,-1表示无穷
	CRCore::crVector3i m_foreigncard;//异国将卡 
	std::vector<short> m_blackgoldshop;//黑店
	int m_blackShopRefreshTimer;//黑点刷新倒计时
	int m_superEquipCoin;//神器币
	int m_refineDiscountTimer;//精炼折扣时间
	unsigned char m_refineDiscount;//精炼折扣率，50表示50%
};

class crPlayerSaleRecord : public CRCore::Referenced
{
public:
	crPlayerSaleRecord():m_saleid(0),m_price(0),m_count(0){}
	void setSaleID(int id){ m_saleid = id; }
	int getSaleID(){ return m_saleid; }
	void setPlayerName(const std::string &name){ m_playerName = name; }
	const std::string& getPlayerName(){ return m_playerName; }
	void setPrice(int price){ m_price = price; }
	int getPrice() { return m_price; }
	void setCount(int count){ m_count = count; }
	int getCount() { return m_count; }
protected:
	int m_saleid;
	std::string m_playerName;
	int m_price;
	int m_count;
};
typedef std::vector< CRCore::ref_ptr<CRNetApp::crInstanceItem> > ItemVec;
typedef std::vector< CRCore::ref_ptr<crPlayerSaleRecord> > PlayerSaleRecordVec;
typedef std::deque < std::pair< int,CRCore::ref_ptr<crBagItemData> > > RecvBagItemDeq;//type,data
typedef std::vector< std::pair<CRCore::ref_ptr<CRCore::crCanvasNode>,CRCore::ref_ptr<CRCore::crCanvasNode>>> ItemCanvasPairVec;//itemname,itemicon
typedef std::deque < std::string > RecvFloatAnimationTextDeq;
typedef std::vector< CRCore::ref_ptr<CRCore::crCanvasNode> > FloatAnimationCanvasVec;//浮动用的canvas
typedef std::vector<int> RecruitArmyVec;

typedef std::map < int,CRCore::ref_ptr<crMailInfo>,std::greater<int>> MailCacheMap;//mailid,

class crJXJBrainData : public CRNetApp::crBrainData
{//client
public:
	crJXJBrainData();
	crJXJBrainData(const crJXJBrainData& data);
	DataClass(JXJ, JXJBrain)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	int m_curPlayerIconID;
	int m_curRecruitArmID;//RecruitArmWalk
	int m_curSelRecruitArmAID;
	int m_curFormationArmID;//FormationArmWalk
	int m_curSelFormationArmAID;
	int m_curCDHintID;
	int m_curTimesID;
	PlayerSaleRecordVec m_playerSaleRecordVec;
	int m_curSalePageNum;
	int m_saleTotal;
	int m_mycurSalePageNum;
	int m_mysaleTotal;
	GNE::Mutex m_dataMutex;
	unsigned short m_equipID;//打造装备
	int m_curFriendPageNum;//好友当前页
	int m_totalFriendPageNum;//好友总页数
	std::string m_addfriendname;
	unsigned char m_saleitemid; //所售物品在背包第几格
	int m_saleitemmax;
	int m_curJiangkaBagPageNum;//将卡背包当前页
	int m_totalJiangkaBagPageNum;//将卡背包总页数
	char m_JiangkaFormationRow;//将卡编队行
	float m_NewsCanvasStep;
	bool m_IfSetArmyNum;	
	CRCore::ref_ptr<CRCore::crCanvasNode> m_cur_canvas; // 当前打开canvas 名称
	CRCore::ref_ptr<CRUI::crImageBoxWidgetNode> m_rtt_widget; // 当前打开canvas rtt 控件名称
	bool m_rtt_refresh; // 确认是否从头开始RTT
	int m_autoequipscount; // 自动打造装备的数量
	int m_curinvitepagenum; // 邀请好友界面当前页
	int m_totalinvitepagenum; // 邀请好友界面总页数
	//unsigned char m_curinvitetype; // 邀请好友,城战or出征
	int m_CurForgeEquipID;//GongfangEquipWalk
	//int m_CurSelForgeEquipID;//GongfangSelectEquipID
	JXJItemBagVec m_salebagvec;
	int m_cursalecardpagenum; // 出售将卡界面当前页
	int m_totalsalecardpagenum; // 出售将卡界面总页数
	short m_salejiangkaindex; // 升级将卡index
	int m_newjiangkaid; // 升级新将卡id
	JXJRecvMailVec m_recvmailvec;//收取邮件列表
	unsigned char m_checkmailindex;
	MailCacheMap m_recvmailbodyMap;//收取邮件内容列表
	unsigned char m_deleteallmailflag;//是否删除所有邮件
	int m_curformationpagenum; // 编制界面当前页
	int m_totalformationpagenum; //编制界面总页数
	std::string m_chatReciverName; //私聊接收消息玩家
	int m_rankingpagenum;
	int m_rankingtotal;
	int m_BrainRecruitCount;//临时招兵数量
	int m_BrainResourcesCardCount;//临时资源卡数量
	int m_BrainRushTaskCount;//临时任务扫荡次数
	unsigned char m_BrainExtractJiangKa;//抽取将卡成功判断
	bool m_unfoldtasktype1; //该类型任务是否展开
	bool m_unfoldtasktype2; //该类型任务是否展开
	bool m_unfoldtasktype3; //该类型任务是否展开
	bool m_unfoldtasktype4; //该类型任务是否展开
	int m_curselecttaskid;//当前选择的任务id
	int m_GongfangLordPaperCount;//领主装备图纸数量
	RecvBagItemDeq m_itemDeq;//进入背包物品队列
	ItemCanvasPairVec m_canvasVec;
	unsigned char m_useitemremaincount;
	unsigned char m_rollcardremaincount;
	unsigned char m_PingpanRemainCount;	// 评判剩余次数

	CRCore::ref_ptr<CRCore::crNode> m_msOnBulidingNode;
	CRCore::ref_ptr<CREncapsulation::crTableIO> m_playdatatabIO;
	int m_goldcosthinttype;
	MailCacheMap m_mailMap;
	RecruitArmyVec m_recruitarmyvec;
	unsigned char m_friendorblack;//0加好友,1黑名单
	unsigned char m_ifEndSendPlayerData;
	unsigned char m_remainfubenrushcount;
	CRCore::crVector2ub m_queryloseshiliid;//注册时使用的，已灭国国家ID

	bool m_onlineDongHuaVisiable;// 可领取动画显示
	int m_hideModel;//战场屏蔽NPC和玩家模型
	int m_autoBattleSetting;//战场自动低配
	int m_newguideid;//新手引导执行ID
	int m_newguidestep;//新手引导执行到第几步
	std::string m_newguidecanvas;//新手引导点击后触发按钮的父窗口名称
	std::string m_newguidebtn;//新手引导点击后触发按钮名称
	int m_newguidebtnmessage;//新手引导点击后触发按钮的点击事件（如mousepush，mouserelease等）
	int m_totalrollcardcount;//客户端记录当前抽将总次数
	int m_totalupgradecardcount;//客户端记录当前升级将卡总次数
	int m_totalmakelordequipcount;//客户端记录当前打造装备总次数
	//float m_newhandtaskscrollbar;//任务栏滚动条value
	std::vector<int> m_newguidebtnparam;//新手引导触发按钮的重要参数
	int m_newguideemptyhypertext;//是否卡在了超文本界面

	bool m_showVersion;			// 是否显示版本信息面板
	CRCore::crVector3i m_mousePushPos;	// 鼠标最后一次按下的位置

	int m_totalexchangerescount;//客户端记录当前市集兑换次数
	int m_totallevycount;//客户端记录当前征税次数
	int m_delayHideWidgetTime;//延迟隐藏控件
	
};
//typedef std::map<unsigned short,bool> AccomplishMap;
typedef std::map<unsigned short,CRCore::ref_ptr<crJXJAccomplish> > AccomplishMap;
//typedef std::map<int, CRCore::ref_ptr<crJXJHuodong> > HuodongMap;
typedef std::map<int, CRCore::ref_ptr<crJXJNewHuodong> > NewHuodongMap;//huodongid
typedef std::map<unsigned short, CRCore::ref_ptr<crJXJHuoyuedu> > HuoyueduMap;
typedef std::set<unsigned short > HuoyueduRewardSet;
#define SuperEquipZhulingCount 10//神器加点条目总数
#define SuperEquipCount 8//神器总数
#define SuperEquipMaxLevel 40//神器最高等级
typedef std::vector<unsigned char>SuperEquipZhulinglvVec;
class SuperEquipData : public CRCore::Referenced
{
public:
	friend class crJXJMainRoleData;
	SuperEquipData(unsigned char equipid)
	{
		m_superEquipID = equipid;
		m_superEquipLevel = 1;
		m_superEquipExp = 0;
	}
	void setSuperEquipID(unsigned char id) { m_superEquipID = id; }
	unsigned char getSuperEquipID() { return m_superEquipID; }
	unsigned short openSuperEquip(unsigned char lvupPoint)
	{
		return levelup(lvupPoint);
	}
	unsigned short levelup(unsigned char lvupPoint);//返回获得的注灵点数
	unsigned char getSuperEquipLevel() { return m_superEquipLevel; }
	void setSuperEquipLevel(unsigned char lv) { m_superEquipLevel = lv; }
	void setSuperEquipExp(int exp)
	{
		m_superEquipExp = exp;
	}
	unsigned short addSuperEquipExp(int exp, unsigned char lvupPoint)
	{
		m_superEquipExp += exp;
		return levelup(lvupPoint);
	}
	int getSuperEquipExp() { return m_superEquipExp; }
private:
	unsigned char m_superEquipID;
	int m_superEquipExp;//神器经验
	unsigned char m_superEquipLevel;//0表示未开启
};
class SuperEquipZhulingData : public CRCore::Referenced
{
public:
	friend class crJXJMainRoleData;
	SuperEquipZhulingData()
	{
		m_zhulingPoint = 0;
		m_zhulinglvVec.resize(SuperEquipZhulingCount, 1);
	}
	void addZhulingPoint(short point) { m_zhulingPoint += point; }
	bool reduceZhulingPoint(short point) 
	{
		if (m_zhulingPoint >= point)
		{
			m_zhulingPoint -= point;
			return true;
		}
		return false;
	}
	unsigned short getZhulingPoint() { return m_zhulingPoint; }
	void setZhulingPoint(unsigned short point) { m_zhulingPoint = point; }
	void setZhulinglv(unsigned char index, unsigned char zhulinglv)
	{
		if (index < SuperEquipZhulingCount) m_zhulinglvVec[index] = zhulinglv;
	}
	unsigned char getZhulinglv(unsigned char index)
	{
		if (index < SuperEquipZhulingCount)
			return m_zhulinglvVec[index];
		return 0;
	}
	SuperEquipZhulinglvVec &getZhulinglvVec() { return m_zhulinglvVec; }
	void resetZhuling()
	{
		for (SuperEquipZhulinglvVec::iterator itr = m_zhulinglvVec.begin();
			itr != m_zhulinglvVec.end();
			++itr)
		{
			*itr = 1;
		}
	}
private:
	unsigned short m_zhulingPoint;
	SuperEquipZhulinglvVec m_zhulinglvVec;
};
typedef std::vector< CRCore::ref_ptr<SuperEquipData> >SuperEquipDataVec;
class crJXJMainRoleData : public CRNetApp::crRoleData
{//server & client
public:
	crJXJMainRoleData();
	crJXJMainRoleData(const crJXJMainRoleData& data);
	DataClass(JXJ, JXJMainRole)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	virtual ~crJXJMainRoleData();
	bool m_mainroleinited;//
	unsigned char m_level;//等级
	int m_experience;//经验
	int m_exploit;//功勋
	int m_achievement;//战功
	//int m_consortiaid;//军团ID
	unsigned char m_govpost;//官职
	unsigned char m_nobility;//爵位
	//unsigned char m_groupPos;// 军团官职
	//unsigned short m_campaignranking;//征战排名
	unsigned short m_bingfa;//兵法
	unsigned short m_zhenfa;//阵法
	unsigned short m_dongcha;//洞察
	unsigned short m_tongshuai;//统率

	//short m_soldierCount;//兵数
	//short m_combatpower;//战斗力
	//JXJLordSuitMap m_lordsuitmap;//需要去掉
	AccomplishMap m_accomplishMap;
	//ClientAccomplishMap m_clientAccomplishMap;//客户端MAP
	unsigned short m_accomplishPoint;//获得的成就点数量
	//HuodongMap m_huodongMap;
	NewHuodongMap m_newHuodongMap;
	JXJNewGuideMap m_newGuideMap;//新手引导
	HuoyueduMap m_huoyuedumap;//活跃度
	HuoyueduRewardSet m_huoyuedurewardset;//活跃度奖励
	//int m_mygroupapplyid;
	//MyGroupApplySet m_myGroupApplySet;	// 申请的军团 id

	//unsigned short m_attrPoint;//未分配的属性点,每升1级获得5点，注意避免数字溢出
	//unsigned short m_skillPoint;//未使用的技能点,每升1级获得2点，注意避免数字溢出
	std::vector<JXJLordEquipVec> m_equipvecs;//领主装备列表,装备换装;
	unsigned char m_currentEquipIndex;//当前装备索引;
	unsigned char m_openedEquipVec;//已开启的装备页;
	unsigned char m_appointColumnsVec;//已开启委任栏；
	_crInt64 m_changeNameTime;//上一次修改角色名时间
	int m_qiyutaskid;//今天接取的奇遇任务id，0点清零
	unsigned char m_qiyuCompleteCount;//奇遇任务完成次数
	SuperEquipDataVec m_superEquipDataVec;//神器
	unsigned char m_superEquipLvupPoint;//神器等级升级获得注灵点数（0,1,2,3,4）
	unsigned char m_expPelletBuyTimes;//经验丹购买次数
	CRCore::ref_ptr<SuperEquipZhulingData> m_superEquipZhulingData;
};
class crJXJMetierData : public CRCore::crData
{
public:
	crJXJMetierData();
	crJXJMetierData(const crJXJMetierData& data);
	DataClass(CRNetApp, JXJMetier)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	virtual ~crJXJMetierData();
	CRNetApp::GameTaskMap m_gameTaskMap;
	GNE::Mutex m_dataMutex;
	int m_currentTaskID;
	static GNE::Mutex m_saveMutex;
};
class crJXJTechUIData : public CRCore::crUIData
{//client
public:
	crJXJTechUIData();
	crJXJTechUIData(const crJXJTechUIData& data);
	DataClass(JXJ, JXJTechUI)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	GNE::Mutex m_dataMutex;
	int m_techid;
};
class crJXJRoleData : public CRNetApp::crRoleData
{//server & client
public:
	crJXJRoleData();
	crJXJRoleData(const crJXJRoleData& data);
	DataClass(JXJ, JXJRole)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	unsigned char m_level;
	/*unsigned short*/int m_jxjArmyCount;//
	/*unsigned short*/int m_rtArmyCount;
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_extraParasiticItem;//寄生者只存在于sceneserver寄生期间,宿主对己方及队友照成的伤害都是属于寄生者的
	CRCore::ref_ptr<CRNetApp::crInstanceItem> m_extraParasiticItem_rd;//寄生者只存在于sceneserver

	unsigned char m_troopType;//兵种类型
	unsigned char m_npcItemType;//npc类型
	short m_attackgainmp;		// 普通攻击获得的mp
	short m_damagegainmp;		// 受击增加MP
	short m_extraattackgainmp;	 // 攻击时额外的mp增量
	short m_extraattackgainmp_rd;
	short m_extrauseitemreducemp;	// 使用技能时消耗的mp
	short m_extrauseitemreducemp_rd;	// 使用技能时消耗的mp
	//bool m_isAddRTMP;				//是否增长士气
	unsigned char m_extraRevertType;		// 回复类型
	unsigned char m_extraRevertType_rd;		// 回复类型
	short m_oldExtraId; //之前拾取的随机buf ID (战场临时数据，不用发包)
	short m_extradamagegainmp;	 // 受伤害时额外的mp增量
	short m_extradamagegainmp_rd;
	unsigned char m_chenghao; //玩家的官职
	char m_gainDamageType;//-1,没有，JXJDamageType
	_crInt64 m_lastUseItemTime;//上一次使用技能或者攻击的时间
};
class crJXJBuildingItemData : public CRCore::crData
{//client
public:
	crJXJBuildingItemData();
	crJXJBuildingItemData(const crJXJBuildingItemData& data);
	DataClass(JXJ, JXJBuildingItem)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	GNE::Mutex m_dataMutex;
	unsigned char m_dataType;
	unsigned char m_itemState;
	int m_buildingid;
};
typedef std::map<int, double> TouchPlayerMap;
class crJXJItemData : public CRCore::crData
{//server & client
public:
	crJXJItemData();
	crJXJItemData(const crJXJItemData& data);
	DataClass(JXJ, JXJItem)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	unsigned char m_dataType;
	unsigned char m_itemState;
	unsigned int m_guiseState;//外形状态
	//CRCore::crVector2 m_halfLengths;//itemcollidesize,从脚本读取，不存数据库，不需要网络同步
	CRCore::crVector3 m_halfPhysicsBox;//itemcollidesize,从脚本读取，不存数据库，不需要网络同步
	CRNetApp::PatrolPointVec m_patrolPointVec;//巡逻点坐标
	char m_patrolIndex;//当前巡逻索引
	unsigned char m_patrolLoopMode;//0:不循环 1:LOOP 2:SWING
	unsigned char m_camp;//阵营//0中立（不可攻击,观察者） 1混战（所有人都可以对其攻击） 2组队ID（从2开始后面都是组队ID，不同组队间可以攻击）
	short m_sightRange;//视野 从脚本读取，不用记录到数据库
	short m_eyeHeight;//眼睛高度 从脚本读取，不用记录到数据库
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;
	GNE::Mutex m_dataMutex;
	unsigned char m_npcItemType;//npc类型
	TouchPlayerMap m_touchPlayerMap;
	float m_touchTime;//开宝箱需要的时间
};
class crJXJSkillData : public CRCore::crData
{//server & client
public:
	crJXJSkillData();
	crJXJSkillData(const crJXJSkillData& data);
	DataClass(JXJ, JXJSkill)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	GNE::Mutex m_dataMutex;
	unsigned char m_dataType;
	float m_cdtime;
	char m_skilltype;//技能类型,主动技能或被动技能

	unsigned short m_itemUseRange;//使用范围
	short m_itemUseScope;//作用范围
	int m_damage;
	unsigned short m_percentDamage;//普通攻击千分比伤害
	unsigned char m_damageType;

	unsigned short m_needMP;
	unsigned short m_needHP;
	unsigned short m_useMPLimit;	// 使用技能的最低mp限制
	_crInt32 m_targetExtraID;//id,优先级

	_crInt8 m_targetExtraMaxOverlapCount;//目标技能最大可叠加次数
	_crInt8 m_userExtraMaxOverlapCount;//目标技能最大可叠加次数

	_crInt32 m_userExtraID;//id,优先级
	_crInt32 m_equipExtraID;//id,优先级
	unsigned char m_targetType;//使用对象类型
	int m_belongPlayerID;//新加

	float m_useItemProtectTime;//使用保护时间,吟唱时间
	float m_chanttime;//0表示只吟唱一次，当m_chanttime<m_useItemProtectTime时，表示该法术是持续施法，将会在m_useItemProtectTime期间内每隔m_chanttime触发一次WCH_RecvItemUse
	float m_duration;//攻击有效时间
	//运行时刻数据
	_crInt64 m_lastUseTime;//上一次使用时间

	//short m_equiphp;
	//short m_equipmp;
	//short m_equipAttack;
	//short m_equipDefend;//附加状态对防御属性的影响值
	//short m_equipMagicDefend;//附加状态对防御属性的影响值
	//short m_equipSpeed;
	//short m_equipStealth;
	//short m_equipStealthSpy;
	//short m_equipAttackSpeed;//攻击速度提升百分比
	//short m_equipSkillSpeed;//施法速度提升百分比
	//short m_equipCrit;
	//short m_equipParry;
	//short m_equipDodge;
	//short m_equipBuildAttack;//攻城攻击力
	//short m_equipAttackDist;//攻击距离
	//short m_equipHitratio;//命中率
	//short m_equipIgnoreDefend;//穿透，护甲忽视百分比
	//short m_equipIgnoreMagicDefend;//法术穿透，护甲忽视百分比
	//short m_equipSightRange;//

	//unsigned char m_equipIgnoreDodge;//无视目标闪避 不可叠加
	//short m_equipAttackDamageBack;//物理伤害反弹
	//short m_equipSkillDamageBack;//技能伤害反弹
	//short m_equipAttackAbsorbHP;//物理吸血
	//short m_equipSkillAbsorbHP;//技能吸血
	//unsigned char m_equipCritSword;//暴击剑 不可叠加（暴击杀伤力从200%变为250%）
	//_crInt32 m_equipSputtering;//溅射伤害（溅射半径(unsigned short)，伤害百分比（unsigned short））
	//short m_equipAttackDamage;//加深伤害或者伤害减免
	//short m_equipSkillDamage;
	char m_damageCount;//伤害个数控制，0表示不限,负值表示可以对同一个目标产生多次伤害
};
class crPlayerRanking : public CRCore::Referenced
{
public:
	crPlayerRanking():m_playerid(0),m_exploit(0),m_govpost(1),m_shiliid(0),m_nobility(0),m_accomplishPoint(0),m_troops(0),m_manorlv(0),m_countrycontribute(0){}
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
	void setName(const std::string &name) { m_name = name; }
	std::string &getName() { return m_name; }
	void setExploit(int exploit){ m_exploit = exploit; }
	int getExploit() { return m_exploit; }
	void setGovpost(unsigned char post){ m_govpost = post; }
	unsigned char getGovpost() { return m_govpost; }
	void setShiliID(unsigned char shili){ m_shiliid = shili; }
	unsigned char getShiliID() { return m_shiliid; }
	void setLevel(unsigned char lv){ m_level = lv; }
	unsigned char getLevel() { return m_level; }
	void setNobility(unsigned char nobility){ m_nobility = nobility; }
	unsigned char getNobility() { return m_nobility; }
	void setAccomplishPoint(unsigned short point){ m_accomplishPoint = point; }
	unsigned short getAccomplishPoint() { return m_accomplishPoint; }
	void setTroops(int troops){ m_troops = troops; }
	int getTroops() { return m_troops; }
	void setManorlv(unsigned short lv){ m_manorlv = lv; }
	unsigned short getManorlv() { return m_manorlv; }
	void setCountrycontribute(int countrycontribute){ m_countrycontribute = countrycontribute; }
	int getCountrycontribute() { return m_countrycontribute; } 
	void setPlayerRank(int playerrank) {m_playerrank = playerrank; }
	int getPlayerRank() { return m_playerrank;}
protected:
	int m_playerid;
	std::string m_name;//characterName
	int m_exploit;//功勋
	unsigned char m_govpost;
	unsigned char m_shiliid;//势力ID
	unsigned char m_level;//等级
	unsigned char m_nobility;//爵位
	unsigned short m_accomplishPoint;//成就点数
	int m_troops;
	unsigned short m_manorlv;
	int m_countrycontribute;//国家贡献
	int m_playerrank;
};
//typedef std::map<int,CRCore::ref_ptr<crPlayerRanking> >ExpRankingWaitMap;//playerid
typedef std::multimap<int,CRCore::ref_ptr<crPlayerRanking>,std::greater<int> >ExpRankingMap;//exploit
typedef std::multimap<unsigned char,CRCore::ref_ptr<crPlayerRanking>,std::greater<unsigned char> >LevelRankingMap;//level
typedef std::multimap<_crInt64,CRCore::ref_ptr<crPlayerRanking>,std::greater<_crInt64> >GovpostRankingMap;//exploit,govpost
typedef std::multimap<unsigned short,CRCore::ref_ptr<crPlayerRanking>,std::greater<unsigned short> >AccomplishRankingMap;//accomplishPoint
typedef std::multimap<int,CRCore::ref_ptr<crPlayerRanking>,std::greater<int> >TroopsRankingMap;//兵力
typedef std::multimap<int,CRCore::ref_ptr<crPlayerRanking>,std::greater<int> >CountrycontributeRankingMap;//国家贡献
typedef std::multimap<unsigned short,CRCore::ref_ptr<crPlayerRanking>,std::greater<unsigned short> >ManorlvRankingMap;//领地发展

typedef std::deque<int>SaleDisposeDeq;//playerid
class crQuerySale : public CRCore::Referenced
{
public:
	crQuerySale(int playerid):m_playerid(playerid),m_start(0){}
	int getPlayerID(){ return m_playerid; }
	void setGoods(CRCore::crVector2i &goods){ m_goods = goods; }
	const CRCore::crVector2i& getGoods(){ return m_goods; }
	void setStart(int start){ m_start = start; }
	int getStart() { return m_start; }
protected:
	int m_playerid;
	CRCore::crVector2i m_goods;
	int m_start;
};
typedef std::deque< CRCore::ref_ptr<crQuerySale> > QuerySaleDeque;
typedef std::deque< std::pair<int,int> >BuySaleDeque;//saleid,playerid
typedef std::set< std::string >GiftsPackKeySet;

class crQueryGiftsPack : public CRCore::Referenced
{
public:
	crQueryGiftsPack(int playerid,std::string keyWord):m_playerid(playerid),m_keyWord(keyWord),m_mailID(0){}
	int getPlayerID(){ return m_playerid; }
	std::string getKey(){ return m_keyWord; }
	int getMailID(){return m_mailID;}
	void setMailID(int id){m_mailID = id;}
protected:
	int m_playerid;
	std::string m_keyWord;
	int m_mailID;
};
typedef std::deque< CRCore::ref_ptr<crQueryGiftsPack> > QueryGiftsPackDeque;
class crCreateGiftsPack : public CRCore::Referenced
{
public:
	crCreateGiftsPack(int playerid,std::string key,CRCore::crStreamBuf *packet):m_playerid(playerid),m_key(key),m_pack(packet){}
	int getPlayerID(){ return m_playerid; }
	std::string getKey(){return m_key;}
	CRCore::crStreamBuf *getStreamBuf(){return m_pack.get();}
protected:
	std::string m_key;
	int m_playerid;
	CRCore::ref_ptr<CRCore::crStreamBuf>m_pack;
};
class crRushPlayerInfo : public CRCore::Referenced
{
public:
	crRushPlayerInfo():m_state(0),m_rushcount(0),m_fubenid(0),m_playerid(0){}
	void setRushCount(unsigned char rushcount){ m_rushcount = rushcount; }
	unsigned char getRushCount() { return m_rushcount; }
	void setAllRushCount(unsigned char allrushcount){ m_allrushcount = allrushcount; }
	unsigned char getAllRushCount() { return m_allrushcount; }
	void setState(unsigned char state){ m_state = state; }
	unsigned char getState() { return m_state; }
	void setFubenID(unsigned short fubenid){ m_fubenid = fubenid; }
	unsigned short getFubenID() { return m_fubenid; }
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
protected:
	unsigned char m_rushcount;
	unsigned char m_allrushcount;
	unsigned char m_state;
	unsigned short m_fubenid;
	int m_playerid;
};
class crCounterPlayerInfo : public CRCore::Referenced
{//平叛
public:
	crCounterPlayerInfo():m_state(0),m_rushcount(0),m_chengchiid(0),m_playerid(0){}
	void setTotalCount(unsigned char allrushcount){ m_allrushcount = allrushcount; }
	unsigned char getTotalCount() { return m_allrushcount; }
	void setRemainCount(unsigned char rushcount){ m_rushcount = rushcount; }
	unsigned char getRemainCount() { return m_rushcount; }
	void setState(unsigned char state){ m_state = state; }
	unsigned char getState() { return m_state; }
	void setChengChiID(unsigned short chengchiid){ m_chengchiid = chengchiid; }
	unsigned short getChengChiID() { return m_chengchiid; }
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
protected:
	unsigned char m_rushcount;
	unsigned char m_allrushcount;
	unsigned char m_state;
	unsigned short m_chengchiid;
	int m_playerid;
};
class crQueryGroupMemberInfo : public CRCore::Referenced
{
public:
	crQueryGroupMemberInfo():m_querytype(0),m_startpage(0),m_perpagenum(0),m_groupid(0),m_playerid(0){}
	void setQueryType(unsigned char querytype){ m_querytype = querytype; }
	unsigned char getQueryType() { return m_querytype; }
	void setStartPage(unsigned char startpage){ m_startpage = startpage; }
	unsigned char getStartPage() { return m_startpage; }
	void setPerPageNum(unsigned char perpagenum){ m_perpagenum = perpagenum; }
	unsigned char getPerPageNum() { return m_perpagenum; }
	void setGroupID(unsigned short groupid){ m_groupid = groupid; }
	unsigned short getGroupID() { return m_groupid; }
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
protected:
	unsigned char m_querytype;
	unsigned char m_startpage;
	unsigned char m_perpagenum;
	int m_groupid;
	int m_playerid;
};
class crJXJSystermMailInfo : public CRCore::Referenced
{
public:
	crJXJSystermMailInfo(){}
	void setSysMailTitle(std::string mailTitle){m_mailTitle = mailTitle;}
	const std::string& getSysMailTitle(){return m_mailTitle;}
	void setSysMailContent(std::string mailContent){m_mailContent = mailContent;}
	const std::string& getSysMailContent(){return m_mailContent;}
	void setSysMailItems(const CRCore::crVector3i &mailItems1,const CRCore::crVector3i &mailItems2,const CRCore::crVector3i &mailItems3,const CRCore::crVector3i &mailItems4)
	{ m_rewardItems1 = mailItems1; m_rewardItems2 = mailItems2; m_rewardItems3 = mailItems3; m_rewardItems4 = mailItems4; }
	const CRCore::crVector3i& getSysRewardItems1(){return m_rewardItems1;}
	const CRCore::crVector3i& getSysRewardItems2(){return m_rewardItems2;}
	const CRCore::crVector3i& getSysRewardItems3(){return m_rewardItems3;}
	const CRCore::crVector3i& getSysRewardItems4(){return m_rewardItems4;}
	void setSysMailType(unsigned char type){m_mailType = type;}
	unsigned char getSysMailType(){return m_mailType;}
	void setPlayerCharacterName(std::string name){m_characterName = name;}
	const std::string& getPlayerCharacterName(){return m_characterName;}
protected:
	std::string m_mailTitle;
	std::string m_mailContent;
	std::string m_characterName;
	//CRCore::crVector4i  m_rewardItems;
	CRCore::crVector3i  m_rewardItems1;
	CRCore::crVector3i  m_rewardItems2;
	CRCore::crVector3i  m_rewardItems3;
	CRCore::crVector3i  m_rewardItems4;
	unsigned char m_mailType;
};
class crJXJCounterinsurgencyInfo : public CRCore::Referenced
{
public:
	crJXJCounterinsurgencyInfo():m_ucTotalCount(0),m_usChengChiID(0){}
	unsigned char & getTotalCount(){return m_ucTotalCount;}
	unsigned short & getChengChiID(){return m_usChengChiID;}
	unsigned short & getTimes(){return m_times;}
protected:
	unsigned char m_ucTotalCount;
	unsigned short m_usChengChiID;
	unsigned short m_times;
};

class QueryGroupDeqItem : public CRCore::Referenced
{
public:
	QueryGroupDeqItem()
	{
		startPage = 0;
		pageNum = 0;
		playerid = 0;
	}
	unsigned char startPage;
	unsigned char pageNum;
	int playerid;
};

typedef std::deque<CRCore::ref_ptr<crQueryGroupMemberInfo> > QueryGroupMemberDeq;
typedef std::deque< CRCore::ref_ptr<crCreateGiftsPack> > CreateGiftsPackDeque;
typedef std::set<int> OnBattleUIPlayerSet;//playerid
typedef std::set<int> OnChengfangUIPlayerSet;//playerid
typedef std::map<unsigned short,OnChengfangUIPlayerSet > OnChengfangUIPlayerMap;//chengchiid,playeridset
//typedef std::map<int,CRCore::ref_ptr<crRushPlayerInfo>> JXJFubenRushPlayerMap;//playerid,fubenid,rushcount,(1.rushing,2.pause)
typedef std::vector<CRCore::ref_ptr<crRushPlayerInfo> > JXJFubenRushVec;
typedef std::map<char,JXJFubenRushVec> JXJFubenRushMap;//0,1,2,3,4,5 每秒执行一次，0秒的做扫荡，并将结束的退出，依次调用swap交换（5－>4->3->2->1->0－>5)。新加入的首先在队列中寻找有没有相同的playerid，没有则加入到5里

typedef std::vector<CRCore::ref_ptr<crCounterPlayerInfo> > JXJCounterVec;
typedef std::map<char,JXJCounterVec> JXJCounterMap;//0,1,2,3,4,5... 每秒执行一次，0秒的做平叛，并将结束的退出，依次调用swap交换（5－>4->3->2->1->0->5...)。新加入的首先在队列中寻找有没有相同的playerid，没有则加入到time里

typedef std::map< int,CRCore::ref_ptr<crJXJConsortia> > ConsortiaMap;//Consortia id
/////////军团排行榜
typedef std::multimap<unsigned char,CRCore::ref_ptr<crJXJConsortia>,std::greater<unsigned char> >ConsortialvRankingMap;//军团等级
//typedef std::multimap<unsigned short,CRCore::ref_ptr<crJXJConsortia>,std::greater<unsigned short> >ConsortiaMemberRankingMap;//军团人数

typedef std::set< unsigned short > ChengchiModifySet;//chengchiid
typedef std::deque<std::pair<int,std::string> > CreateGroupDeq;//playerid,groupname
//typedef std::deque<std::pair<int,std::pair<unsigned char,unsigned char>> > QueryGroupDeq;//playerid,startid,count
typedef std::deque<CRCore::ref_ptr<QueryGroupDeqItem> > QueryGroupDeq;
typedef std::deque< int > OnCountryQueryDeq;//playerid
typedef std::map< int,OnCountryQueryDeq > OnCountryQueryMap;//shiliid
typedef std::deque< CRCore::crVector3i >	GroupAddMemberDeq;			// invitedplayerid, groupid,playerid
//typedef std::map< int,unsigned char > OnlineRewardMap;//playerid,rewardCount 玩家ID剩余奖励次数
typedef std::deque < CRCore::ref_ptr<crJXJSystermMailInfo> >  SystermMailInfoDeq;
class crJXJChatMessageInfo : public CRCore::Referenced
{
public:
	crJXJChatMessageInfo(const std::string& sender,const std::string &text,unsigned char senderShili,unsigned char broadcastShili,int groupid = 0):
	m_sender(sender),m_text(text),m_senderShili(senderShili),m_broadcastShili(broadcastShili),m_groupid(groupid),m_leaderid(0),m_senderplayerid(0){}
	const std::string &getSender(){ return m_sender; }
	const std::string &getText(){ return m_text; }
	unsigned char getSenderShiliID(){ return m_senderShili; }
	unsigned char getBroadcastShiliID(){ return m_broadcastShili; }
	int getGroupID(){ return m_groupid; }
	void setLeaderID(int leaderid){ m_leaderid = leaderid; }
	int getLeaderID(){ return m_leaderid; }
	void setRecvName(const std::string &name){ m_recvName = name; }
	const std::string &getRecvName(){ return m_recvName; }
	void setSenderID(int playerid){ m_senderplayerid = playerid; }
	int getSenderID(){ return m_senderplayerid; }
protected:
	std::string m_sender;
	std::string m_text;
	unsigned char m_senderShili;
	unsigned char m_broadcastShili;
	int m_groupid;//军团聊天
	int m_leaderid;//组队聊天
	std::string m_recvName;//私聊
	int m_senderplayerid;
};

class crJXJPlayerStoreInfo : public CRCore::Referenced
{
public:
	crJXJPlayerStoreInfo() :m_playerID(0), m_money(0), m_totalmoney(0), m_playerCycleCount(0), m_vipexp(0){}
	int getPlayerId(){return m_playerID;}
	void setPlayerId(int playerid){m_playerID = playerid;}

	int getPlayerMoney(){ return m_money; }
	void setPlayerMoney(int money){ m_money = money; }

	int getTotalMoney(){ return m_totalmoney; }
	void setTotalMoney(int totalmoney){ m_totalmoney = totalmoney; }

	int getVipExp(){ return m_vipexp; }
	void setVipExp(int vipexp){ m_vipexp = vipexp; }

	char getPlayerCycleCount(){ return m_playerCycleCount; }
	void setPlayerCycleCount(short cycleCount){ m_playerCycleCount = cycleCount; }

	JXJStoreItemBagVec & getStoreItemVec(){ return m_itemBagVec; }
	JXJStoreItemBagVec & getStoreCardVec(){ return m_cradBagVec; }
	void setSuper(std::vector<int>&super) { m_super = super; }
	std::vector<int>& getSuper() { return m_super; }
	std::string getSuperStr() { return CRCore::crArgumentParser::appVectoa(m_super); }
protected:
	int m_playerID;
	short m_playerCycleCount;
	int m_money;
	int m_totalmoney;
	int m_vipexp;
	JXJStoreItemBagVec m_cradBagVec;
	JXJStoreItemBagVec m_itemBagVec;
	std::vector<int> m_super;
};

class crPlayerDataQuery : public CRCore::Referenced
{
public:
	crPlayerDataQuery():m_id(0){}
	void setPlayerID(int id){ m_id = id; }
	int getPlayerID() { return m_id; }
	void setAccountName(const std::string &accountname) { m_accountname = accountname; }
	const std::string &getAccountName() const{ return m_accountname; };
	void setLoginDate(const std::string &logindate) { m_logindate = logindate; }
	const std::string &getLoginDate() const{ return m_logindate; };
	void setCreateDate(const std::string &createdate) { m_createdate = createdate; }
	const std::string &getCreateDate() const{ return m_createdate; };
	void setOnlinetime(const std::string &onlinetime) { m_onlinetime = onlinetime; }
	const std::string &getOnlinetime() const{ return m_onlinetime; };
	void setEmail(const std::string &email) { m_email = email; }
	const std::string &getEmail() const{ return m_email; };
protected:
	std::string m_accountname;
	std::string m_logindate;
	std::string m_createdate;
	std::string m_onlinetime;
	std::string m_email;
	int m_id;
};
class crPlayerPayQuery : public CRCore::Referenced
{
public:
	crPlayerPayQuery():m_id(0),m_moneycount(0){}
	void setPlayerID(int id){ m_id = id; }
	int getPlayerID() { return m_id; }
	void setMoneyCount(int moneycount){ m_moneycount = moneycount; }
	int getMoneyCount() { return m_moneycount; }
	void setStr(const std::string &str) { m_str = str; }
	const std::string &getStr() const{ return m_str; };
	void setCreateDate(const std::string &createdate) { m_createdate = createdate; }
	const std::string &getCreateDate() const{ return m_createdate; };
protected:
	std::string m_str;//说明
	std::string m_createdate;
	int m_id;
	int m_moneycount;
};

typedef std::map<int,CRCore::ref_ptr<crPlayerPayQuery>> PlayerPayMap;//playerid,充值记录
typedef std::vector<int> TechLvVec;
typedef std::vector<int> BuildingLvVec;
typedef std::map<int,CRCore::ref_ptr<crPlayerDataQuery>> PlayerMap;
typedef std::deque < CRCore::ref_ptr<crJXJChatMessageInfo> > BroadcastMessageDeq;//全服和国家聊天
typedef std::deque < CRCore::ref_ptr<crJXJChatMessageInfo> > GroupMessageDeq;//军团聊天
typedef std::deque < CRCore::ref_ptr<crJXJChatMessageInfo> > TeamMessageDeq;//组队聊天
typedef std::deque < CRCore::ref_ptr<crJXJChatMessageInfo> > PlayerMessageDeq;//私聊
//typedef std::set <int> ImmediatelySavePlayerSet;
typedef std::multimap< _crInt32,int >GSTeamMap;//(FubenOrChengchiID,FubenOrChengchiType,shiliid),leaderid
typedef std::map<unsigned char,std::set<unsigned short>> RebelCityMap;//shili,chengchiid

typedef std::map< int,CRCore::ref_ptr<crJXJCounterinsurgencyInfo> > CounterInfoMap; // 平叛信息map 1.playerid 2.平叛信息
typedef std::pair<int,CRCore::ref_ptr<crJXJSystermMailInfo>> SystemMailPair;//playerid,
typedef std::deque< SystemMailPair > SystemMailDeque;
typedef std::map <int ,CRCore::ref_ptr<crJXJConsortia>> DismissGroupMap;		// <groupid,dismisstime>

typedef std::pair<int,int> ResourceChangeData;			// 资源类型，消耗数量

typedef std::deque<CRCore::ref_ptr<crJXJPlayerStoreInfo>> PlayerStoreInfoDeq;//临时存储信息

class UIQueryPeopleInfoByLevel : public CRCore::Referenced
{
public:
	UIQueryPeopleInfoByLevel() :m_playerid(0),m_minLv(-1),m_maxLv(-1),m_leftPtr(-1),m_rightPtr(-1)
	{
	}
	int m_playerid;
	unsigned char m_shili;
	unsigned char m_minLv;
	unsigned char m_maxLv;
	unsigned short m_leftPtr;
	unsigned short m_rightPtr;
	unsigned char m_curPage;
	unsigned int m_queryTime;				// 上次查询时间

};

typedef std::deque<CRCore::ref_ptr<UIQueryPeopleInfoByLevel> > UIQueryPeopleInfoDeq;
typedef std::map<unsigned char,unsigned short > UIQueryFindByMinLvMap;
typedef std::map<unsigned char,unsigned short > UIQueryFindByMaxLvMap;

class ConsortiaLvRankItem : public CRCore::Referenced
{
public:
	ConsortiaLvRankItem(int id):
		m_id(id),m_lv(0), m_shili(0),m_memberCount(0),
		m_troops(0),m_exploit(0)
	{}
	int getKey() const {return m_id;}
	unsigned char m_lv;
	int m_id;
	int m_shili;
	int m_memberCount;
	int m_troops;
	int m_exploit;
	std::string m_groupName;
	std::string m_leaderName;
};

class ConsortiaLvCompare
{
public:
	bool operator() (const CRCore::ref_ptr<ConsortiaLvRankItem > & left,
		const CRCore::ref_ptr<ConsortiaLvRankItem > & right)
	{
		if (left->m_lv == right->m_lv)
		{
			if (left->m_memberCount == right->m_memberCount)
			{
				if (left->m_exploit == right->m_exploit)
				{
					return left->m_troops > right->m_troops;
				}
				return left->m_exploit > right->m_exploit;
			}
			return left->m_memberCount > right->m_memberCount;
		}
		return left->m_lv > right->m_lv;
	}
};

class ConsortiaMemberCountCompare
{
public:
	bool operator() (const CRCore::ref_ptr<ConsortiaLvRankItem > & left,
		const CRCore::ref_ptr<ConsortiaLvRankItem > & right)
	{
		if (left->m_memberCount == right->m_memberCount)
		{
			if (left->m_lv == right->m_lv)
			{
				if (left->m_exploit == right->m_exploit)
				{
					return left->m_troops > right->m_troops;
				}
				return left->m_exploit > right->m_exploit;
			}
			return left->m_lv > right->m_lv;
		}
		return left->m_memberCount > right->m_memberCount;
	}
};

class ConsortiaExploitCompare
{
public:
	bool operator() (const CRCore::ref_ptr<ConsortiaLvRankItem > & left,
		const CRCore::ref_ptr<ConsortiaLvRankItem > & right)
	{
		if (left->m_exploit == right->m_exploit)
		{
			if (left->m_lv == right->m_lv)
			{
				if (left->m_memberCount == right->m_memberCount)
				{
					return left->m_troops > right->m_troops;
				}
				return left->m_memberCount > right->m_memberCount;
			}
			return left->m_lv > right->m_lv;
		}
		return left->m_exploit > right->m_exploit;
	}
};

class ConsortiaTroopsCompare
{
public:
	bool operator() (const CRCore::ref_ptr<ConsortiaLvRankItem > & left,
		const CRCore::ref_ptr<ConsortiaLvRankItem > & right)
	{
		if (left->m_troops == right->m_troops)
		{
			if (left->m_lv == right->m_lv)
			{
				if (left->m_memberCount == right->m_memberCount)
				{
					return left->m_exploit > right->m_exploit;
				}
				return left->m_memberCount > right->m_memberCount;
			}
			return left->m_lv > right->m_lv;
		}
		return left->m_troops > right->m_troops;
	}
};

typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaLvRankItem>,ConsortiaLvCompare> ConsortialvRank;
typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaLvRankItem>,ConsortiaMemberCountCompare> ConsortiaMemberCountRank;
typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaLvRankItem>,ConsortiaExploitCompare> ConsortiaExploitRank;
typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaLvRankItem>,ConsortiaTroopsCompare> ConsortiaTroopsRank;

class JXJGroupChatMsgBase :public CRCore::Referenced
{
public:
	typedef JXJGroupChatMsgBase Base;
	virtual void getCompleteString(std::string &) = 0;
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	inline CRCore::crHandle * getNoticeHandle();
	unsigned char m_infoType;
	int m_msgId;
};

typedef std::vector<std::pair<int,CRCore::ref_ptr<JXJGroupChatMsgBase> > > GroupChatMsgVec;


class crBattleReportInfo : public CRCore::Referenced
{
public:
	crBattleReportInfo(){}
	void setBattleID(int id){ m_battleid = id; }
	int getBattleID(){ return m_battleid; }

	void setMailTitle(const char * str){ m_strMailTitle= str; }
	std::string &getMailTitle(){ return m_strMailTitle; }

	void setTextInfo(const char * str){ m_strTextInfo = str; }
	std::string & getTextInfo(){ return m_strTextInfo; }

	void setBattleData(CRCore::ref_ptr<CRCore::crStreamBuf>& buffer){ m_dataBuffer = buffer; }
	CRCore::ref_ptr<CRCore::crStreamBuf>& getBattleData(){ return m_dataBuffer; }

	std::vector<int> & getPlayerVec(){ return m_playerVec; }
protected:
	int m_battleid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_dataBuffer;//战场相关数据
	std::vector<int> m_playerVec;
	std::string m_strTextInfo;
	std::string m_strMailTitle;
};

typedef std::set<int> OnlinePlayerSet;				// 每日登陆的玩家id
typedef std::deque< CRCore::ref_ptr<crBattleReportInfo> > InsertBattleReportDeq; //battleid，战场数据
typedef std::deque< std::pair<int,int> > QueryBattleReportDeq;//playerid，战报id
//typedef std::map< int,PersuadeInfoDeq > PersuadeInfoMap;//playerid,说服信息
typedef std::set< int > NoChatSet;//playerid 禁言

class crBattleRanking : public CRCore::Referenced
{
public:
	crBattleRanking():m_playerid(0),m_jifeng(0),m_killcount(0),m_deadcount(0),m_exploit(0),m_lingqu(false){}
	void setPlayerID(int playerid){ m_playerid = playerid; }
	int getPlayerID() { return m_playerid; }
	void setName(const std::string &name) { m_name = name; }
	std::string &getName() { return m_name; }
	void addJifeng(int jifeng){ m_jifeng += jifeng; }
	int getJifeng() { return m_jifeng; }
	void addKillCount(int killcount){ m_killcount += killcount; }
	int getKillCount() { return m_killcount; }
	void addDeadCount(int deadcount){ m_deadcount += deadcount; }
	int getDeadCount() { return m_deadcount; }
	void addExploit(int exploit){ m_exploit += exploit; }
	int getExploit() { return m_exploit; }
	void setLingqu(bool lingqu){ m_lingqu = lingqu; }
	int getLingqu() { return m_lingqu; }
protected:
	int m_playerid;
	std::string m_name;//characterName
	int m_jifeng;//总积分
	int m_killcount;//杀敌总数
	int m_deadcount;//死亡总数
	int m_exploit;//总功勋
	bool m_lingqu;//奖励是否已领取标识
};
typedef std::multimap<int,CRCore::ref_ptr<crBattleRanking>,std::greater<int> >BattleRankingMap;//国战积分排行榜,按国战总积分排
class crJXJGameServerData : public CRCore::crData
{//server
public:
	crJXJGameServerData();
	crJXJGameServerData(const crJXJGameServerData& data);
	DataClass(JXJ, JXJGameServer)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	//ExpRankingWaitMap m_expRankingWaitMap;
	//ExpRankingMap m_expRankingWaitMap;
	ExpRankingMap m_shiliExpRanking;//[JXJ_SHILICOUNT];
	//LevelRankingMap m_lvRankingWaitMap;
	LevelRankingMap m_lvRanking;
	//GovpostRankingMap m_govpostRankingWaitMap;
	GovpostRankingMap m_govpostRanking;
	//AccomplishRankingMap m_accomplishRankingWaitMap;
	AccomplishRankingMap m_accomplishRanking;
	//TroopsRankingMap m_troopsRankingWaitMap;
	TroopsRankingMap m_troopsRanking;
	//CountrycontributeRankingMap m_countrycontributeRankingWaitMap;
	CountrycontributeRankingMap m_countrycontributeRankingMap;
	//ManorlvRankingMap m_manorlvRankingWaitMap;
	ManorlvRankingMap m_manorlvRankingMap;
	///军团排行榜
	ConsortialvRankingMap m_consortialvRankingMap;//
	//ConsortiaMemberRankingMap m_consortiaMemberRankingMap;//
	ConsortialvRank m_consortiaLvRank;
	ConsortiaMemberCountRank m_consortiaMemberCountRank;
	ConsortiaExploitRank m_consortiaexploitRank;
	ConsortiaTroopsRank m_consortiatroopsRank;

	SaleDisposeDeq m_saleDisposeDeq;
	QuerySaleDeque m_querySaleDeq;
	BuySaleDeque m_buySaleDeq;
	bool m_openBattle;//开启城战
	int m_battleTimer;//不存数据库
	//bool m_openPass; // 开启关隘
	//short m_passTimer; // 关隘计时器
	ChengchiMap m_chengchiMap;
	//bool m_chengchiMapModify;
	DefendDequeMap m_defendDequeMap;
	AttackDequeMap m_attackDequeMap;
	OpenBattleMap m_openBattleMap;
	ClosedBattleSet m_closeBattleSet;//记录战场开启状态
	BattleScoreMap m_battleScoreMap;
	CreateBattleDeque m_createBattleDeque;
	BattleDropPlayerMap m_battleDropPlayerMap;//
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;//玩家登录的时候需要将有些信息同步给玩家
	GNE::Mutex m_dataMutex;
	MailTaskDeque m_mail_sending;
	MailQueryDeque m_mail_query;
	GiftsPackKeySet m_giftsPackKeySet;
	MailQueryDeque m_mailbody_query;
	MailQueryDeque m_maildeletedeq;
	MailQueryDeque m_newmailquerylist;
	CreateGiftsPackDeque m_createGiftsPackDeque;
	QueryGiftsPackDeque m_queryGiftsPacketDeque;
	OnBattleUIPlayerSet m_onBattleUIPlayerSet;
	OnChengfangUIPlayerMap m_onChengfangUIPlayerMap;
	ConsortiaMap m_consortiaMap;
	JXJFubenRushMap m_fubenrushmap;
	ChengchiModifySet m_chengchiModifySet;
	CreateGroupDeq m_creategroupdeq;
	GroupAddMemberDeq m_groupAddMemberDeq;		
	QueryGroupDeq m_querygroupdeq;
	QueryGroupMemberDeq m_querygroupmemberdeq;
	CRCore::ref_ptr<crJXJShili> m_shiliWei;//势力id 10
	CRCore::ref_ptr<crJXJShili> m_shiliShu;//势力id 11
	CRCore::ref_ptr<crJXJShili> m_shiliWu;//势力id 12
	OnCountryQueryMap m_oncountryquerymap;
	SystermMailInfoDeq	m_sysMailDeq;
	//ImmediatelySavePlayerSet m_immediatelySavePlayerSet;
	GSTeamMap m_gsteamMap;
	RebelCityMap m_rebelCityMap;
	bool m_bIsOpenRebel;
	int m_nContinuedRebelTimer;
	//CounterInfoMap m_counterMap;
	unsigned short m_perRebelTimer;
	BroadcastMessageDeq m_sysbroadcastMessageDeq;
	BroadcastMessageDeq m_broadcastMessageDeq;
	GroupMessageDeq m_groupMessageDeq;
	TeamMessageDeq m_teamMessageDeq;
	PlayerMessageDeq m_playerMessageDeq;
	PlayerMap m_checkplayerdatamap;
	int m_checkplayerdataplayerid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_cfgstream;//中央公告
	JXJCounterMap m_jxjcounterMap;
	SystemMailDeque m_systemMailDeq;
	float m_cycleTimer;//轮回缓冲期倒计时 单位是s
	bool m_isCycling;//轮回是否开启
	DismissGroupMap m_dismissGroupMap;
	PlayerStoreInfoDeq m_playerStoreInfos;
	PlayerStoreInfoDeq m_playerStoreRecordInfos;

	UIQueryPeopleInfoDeq m_queryPeopleInfoMap;
	GroupChatMsgVec m_groupChatVec;
	bool isChengChiShiliSyn;		// chengChimap 是否同步到 CantonMap		不保存
	time_t m_groupDayUpdate;
	time_t m_groupWeekUpdate;
	time_t m_shiliDayUpdate;

	InsertBattleReportDeq m_insertBattleReportDeq;//战报插入队列
	QueryBattleReportDeq m_queryBattleReportDeq;//战报查询队列

	NoChatSet m_noChatSet;//
	int m_systemnoticeid;
	OnlinePlayerSet m_onlinePlayerSet;
	time_t m_consortiaPromotion; //军团活动刷新时间
	BattleRankingMap m_battleRankingMapWei;//国战积分排行榜
	BattleRankingMap m_battleRankingMapShu;//国战积分排行榜
	BattleRankingMap m_battleRankingMapWu;//国战积分排行榜
	char m_battleRankingVerWei;
	char m_battleRankingVerShu;
	char m_battleRankingVerWu;
	time_t m_battleRankingSaveTime;
};
class crJXJSceneServerData : public CRNetApp::crSceneServerData
{//sceneserver
public:
	crJXJSceneServerData();
	crJXJSceneServerData(const crJXJSceneServerData& data);
	DataClass(JXJ, JXJSceneServer)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	NoChatSet m_noChatSet;//
};
class crJXJQueryRankingUIData : public CRCore::crUIData
{//client
public:
	crJXJQueryRankingUIData();
	crJXJQueryRankingUIData(const crJXJQueryRankingUIData& data);
	DataClass(JXJ, JXJQueryRankingUI)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	//_crInt64 m_lastQueryTime;
	GovpostRankingMap m_govpostRankingMap;
	GNE::Mutex m_dataMutex;
};
typedef std::vector<CRCore::crVector2i> GoodsVec;
class crJXJGoodsRadioUIData : public CRCore::crUIData
{//client
public:
	crJXJGoodsRadioUIData();
	crJXJGoodsRadioUIData(const crJXJGoodsRadioUIData& data);
	DataClass(JXJ, JXJGoodsRadioUI)
	virtual void addParam(int i, const std::string& str);
	virtual void getParam(int i, void*& param);
protected:
	GoodsVec m_goodsVec;
};
typedef std::map<CRCore::crVector2i,int> GoodsStepMap;
class crJXJMarketUIData : public CRCore::crUIData
{//client
public:
	crJXJMarketUIData();
	crJXJMarketUIData(const crJXJMarketUIData& data);
	DataClass(JXJ, JXJMarketUI)
	virtual void addParam(int i, const std::string& str);
	virtual void getParam(int i, void*& param);
protected:
	GoodsStepMap m_goldbuySteps;
	GoodsStepMap m_coppersaleSteps;
};

typedef std::map<char,ItemVec> BattleFormationMap;
typedef std::pair<CRCore::ref_ptr<CRCore::crWidgetNode>, CRCore::crVector3> WidgetNodePair;
typedef std::map< CRCore::ref_ptr<CRCore::crWidgetNode>, float >SignalNodeMap;
typedef std::multimap< std::string, WidgetNodePair >NamedSignalNodeMap;//
//typedef std::vector<CRCore::crVector3> StopSignalVec;

struct NotInSightNode
{
	CRCore::ref_ptr<CRCore::crWidgetNode> m_widget;
	float m_deltaTime;
	int m_id;
	bool m_isEnable;
	NotInSightNode()
	{
		m_id = -1;
		m_deltaTime= 0.0f;
		m_isEnable = false;
	}
};
typedef std::vector<NotInSightNode> SceneNodesVec;
//typedef	std::map<CRCore::ref_ptr<CRCore::crWidgetNode>, float> SceneNodeMap;

typedef std::pair<CRCore::ref_ptr<CRNetApp::crRole>,int> AboutUseItemPair;
typedef std::multimap< char,CRCore::ref_ptr<CRCore::crCanvasNode> > WidgetNodeMap;
typedef std::map< std::string,CRCore::ref_ptr<CRCore::crNode> > SkillRangeNodeMap;
typedef std::vector < CRCore::ref_ptr<CRNetApp::crInstanceItem> > TargetItemVec;
typedef std::vector < CRCore::ref_ptr<CRCore::crWidgetNode> > ButtonVector;
typedef	std::deque <  CRCore::ref_ptr<CRCore::crCanvasNode> > WidgetNodeDeque;
typedef std::map< CRCore::ref_ptr<CRNetApp::crInstanceItem>, float > QueryItemRTHPMap;
class crJXJBattleCameraData : public CRNetApp::crCameraData
{//client
public:
	crJXJBattleCameraData();
	crJXJBattleCameraData(const crJXJBattleCameraData& data);
	DataClass(JXJ, JXJBattleCamera)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_useSkillDecal;
	//CRCore::ref_ptr<CRCore::crNode> m_useSkillRangeDecal;
	SkillRangeNodeMap m_useSkillRangeNodeMap;
	CRCore::ref_ptr<CRCore::crNode> m_usePreSkillRangeDecal;
	CRCore::ref_ptr<CRCore::crNode> m_usePreSkillDecal;
	BattleFormationMap m_battleFormationMap;
	unsigned short m_cameraOperator;
	SignalNodeMap m_signalDeque;
	NamedSignalNodeMap m_namedSignalMap;
	short m_timer;
	short m_othertimer;
	ItemVec m_mouseonitemVec;
	bool m_isShow;
	unsigned short m_range;
	short m_scope;
	unsigned char m_effectTargetType;
	CRCore::ref_ptr<CRNetApp::crRole> m_role;
	AboutUseItemPair m_useItemPair;
	WidgetNodeMap m_widgetNodeMap;
	//WidgetNodeDeque m_widgetNodeDeque;
	//bool m_bHeadCanvasShow;
	bool m_isFollow;//相机跟随
	TargetItemVec m_targetItemVec;
	CRCore::ref_ptr< CRProducer::crGUIEventAdapter > m_ea;//记录按键状态
	ButtonVector m_btVec;
	CRCore::ref_ptr<CRCore::crNode> m_msOnShuiJingNode;
	CRCore::ref_ptr<CRCore::crNode> m_msOnJianTaNode;

	SceneNodesVec m_sceneNodeMap;
	float m_campAddArmyTimer;//据点补兵时间 不存数据库，玩家战场补兵时间间隔限制
	//bool m_isChangeSpeed;
	float m_cameraSpeed;
	bool m_battleCameraMovable;
	bool m_isSkillKeyDown;
	short m_delayTimer;//按键提示窗口延时关闭时间，不存数据库
	QueryItemRTHPMap m_queryItemRTHPMap;
};
class crJXJEnterBattleUIData : public CRCore::crUIData
{//client
public:
	crJXJEnterBattleUIData();
	crJXJEnterBattleUIData(const crJXJEnterBattleUIData& data);
	DataClass(JXJ, JXJEnterBattleUI)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	GNE::Mutex m_dataMutex;
	int m_sceneid;
	int m_timer;
/*	unsigned char m_index;*/
};

typedef std::map< int,int > KillArmyInfoMap;//abstractid , count
class crJXJBattleStats : public CRCore::Referenced
{
public:
	crJXJBattleStats(int playerid,unsigned char shiliid):m_playerid(playerid),m_shiliid(shiliid),m_stats(1),m_kill(0),m_npckill(0),m_bosskill(0),m_bulidingkill(0),m_shadidengji(0),m_deaddengji(0),m_deadbuduic(0),
		m_playerpoints(0),m_zhanshi(true),m_Killedtimes(0),m_zhengtingkill(0),m_towerkill(0),m_doorkill(0){}
	int getPlayerID(){ return m_playerid; }
	unsigned char getShiliID(){ return m_shiliid; }
	void setStats(char stats){ m_stats = stats; }
	char getStats(){ return m_stats; }
	void setKillDengji(char dengji){ m_shadidengji = dengji; }
	char getKillDengji(){ return m_shadidengji; }
	void setDeadDengji(char dengji){ m_deaddengji = dengji; }
	char getDeadDengji(){ return m_deaddengji; }
	void addDeadBuDuiC(int deadbuduic){ m_deadbuduic += deadbuduic; }
	int getDeadBuDuiC(){ return m_deadbuduic; }
	void setPoints(unsigned short points){ m_playerpoints = points; }
	void addPoints(unsigned short points){ m_playerpoints += points; }
	unsigned short getPoint(){ return m_playerpoints; }
	void setShow(bool mark){m_zhanshi = mark; }
	bool getShow(){return m_zhanshi; }
	void addkilledtimes(unsigned char mark){m_Killedtimes += mark; }
	unsigned char getkilledtimes(){return m_Killedtimes; }
	void addKillZhengTing(unsigned char points){ m_zhengtingkill += points; }
	unsigned char getKillZhengTing(){ return m_zhengtingkill; }
	void addKillTower(unsigned char points){ m_towerkill += points; }
	unsigned char getKillTower(){ return m_towerkill; }
	void addKillDoor(unsigned char points){ m_doorkill += points; }
	unsigned char getKillDoor(){ return m_doorkill; }
	void addKillArmyCount(int abstractid,int kill)
	{
		KillArmyInfoMap::iterator itr_info = m_killArmyInfoMap.find(abstractid);
		if(itr_info!=m_killArmyInfoMap.end())
		{
			itr_info->second+=kill;
		}
		else
			m_killArmyInfoMap[abstractid] = kill;
	}
	KillArmyInfoMap & getKillArmyInfoMap(){ return m_killArmyInfoMap; }
	void addKillCount(int kill){m_kill += kill;}
	int getKillCount(){return m_kill;}
	void addDeadMap(int abstractid,int dead)
	{ 
		KillArmyInfoMap::iterator itr_info = m_deadInfoMap.find(abstractid);
		if(itr_info!=m_deadInfoMap.end())
		{
			itr_info->second+=dead;
		}
		else
			m_deadInfoMap[abstractid] = dead;
	}
	KillArmyInfoMap & getDeadCount(){ return m_deadInfoMap; }
	void addNpcKillCount(int npckill){ m_npckill += npckill; }
	int getNpcKillCount(){ return m_npckill; }
	void addBossKillCount(int bossCount){m_bosskill += bossCount;}
	int getBosskillCount(){return m_bosskill;}
	void addNpcKillArmyCount(int abstractid,int npckill)
	{ 
		KillArmyInfoMap::iterator itr_info = m_killNpcArmyMap.find(abstractid);
		if(itr_info!=m_killNpcArmyMap.end())
		{
			itr_info->second+=npckill;
		}
		else
			m_killNpcArmyMap[abstractid] = npckill;
	}
	KillArmyInfoMap & getNpcKillArmyMap(){ return m_killNpcArmyMap; }

	void addBuildingKillArmyCount(int abstractid,int count){ 

		KillArmyInfoMap::iterator itr_info = m_killBuildingArmyMap.find(abstractid);
		if(itr_info!=m_killBuildingArmyMap.end())
		{
			itr_info->second+=count;
		}
		else
			m_killBuildingArmyMap[abstractid] = count; 
	}
	KillArmyInfoMap & getBuildingKillArmyMap(){ return m_killBuildingArmyMap; }

	void addTroopsKillCount(int abstractid,int count){

		KillArmyInfoMap::iterator itr_info = m_bingzhongkillmap.find(abstractid);
		if(itr_info!=m_bingzhongkillmap.end())
		{
			itr_info->second+=count;
		}
		else
			m_bingzhongkillmap[abstractid] = count; 
	}
	KillArmyInfoMap & getTroopsKillMap(){ return m_bingzhongkillmap; }

	void addBuildingKillCount(int kill){m_bulidingkill+=kill;}
	int getBuildingKillCount(){return m_bulidingkill;}
	std::string getPlayerName(){return m_playername;}
	void  setCharacterName(std::string &name){m_playername = name;}
protected:
	int m_playerid;
	unsigned char m_shiliid;
	char m_stats;//0表示离线，1表示在线，2表示死亡离线
	int m_kill;//杀玩家部队数量
	KillArmyInfoMap m_deadInfoMap;//死亡数量
	int m_npckill;//杀死NPC数量
	int m_bosskill;//杀死boss数
	KillArmyInfoMap m_killBuildingArmyMap;//摧毁建筑数量
	int m_bulidingkill;//摧毁建筑单位个数 
	KillArmyInfoMap m_killArmyInfoMap;//杀死玩家兵数
	KillArmyInfoMap m_killNpcArmyMap;//杀死NPC兵数
	std::string m_playername;//20
	char m_shadidengji;
	char m_deaddengji;
	int m_deadbuduic; //玩家死亡部队数量
	unsigned short m_playerpoints; //世界战场中玩家的积分
	bool m_zhanshi;                //世界战场中排行榜能否展示标志
	unsigned char m_Killedtimes;   //被击杀的次数
	unsigned char m_zhengtingkill;   //击破正厅数量
	unsigned char m_towerkill;   //击破箭塔数量
	unsigned char m_doorkill;   //击破城门数量
	KillArmyInfoMap m_bingzhongkillmap; //玩家兵种击杀数量
};
typedef std::map< int,CRCore::ref_ptr<crJXJBattleStats> > BattleStatsMap;//playerid

struct RecruitCheckResult
{
	RecruitCheckResult():
		m_code(0),
		m_bingyinglvNeed(0),
		m_stationidNeed(0),
		m_timeNeed(0.0f),
		m_equipid(0),
		m_equipCount(0),
		//m_coppercashNeed(0),
		//m_foodNeed(0),
		m_maxRecruitCount(0)
	{}
	char m_code;//0不能招募，1正常，-1部队人数上限, -2队伍数量上限，-3军备等级不足， -4驻地条件, -5没有招募时间，-6装备不足，-7粮食不足，-8木材不足，-9铁矿不足，-10马匹不足，-11铜钱不足，-12不可招募
	unsigned char m_bingyinglvNeed;
	int m_stationidNeed;
	float m_timeNeed;
	unsigned short m_equipid;
	int m_equipCount;
	//int m_coppercashNeed;
	//int m_foodNeed;
	int m_maxRecruitCount;//最大可招募人数
	std::vector<int> m_needjunbeivec;//需要军备科技条件
	std::vector<float> m_needresourcevec;//需要资源
};

// 关隘中的checkpoint 粮仓的数据
class PassCheck : public CRCore::Referenced
{
public:
	PassCheck():m_type(-1),m_score(0),m_pos_x(0.0f),m_pos_y(0.0f),m_area(0.0f){}
	void setType(char type){m_type = type;}
	char getType(){return m_type;}
	void setScore(short score){m_score = score;}
	short getScore(){return m_score;}
	void setPosX(float x){m_pos_x = x;}
	float getPosX(){return m_pos_x;}
	void setPosY(float y){m_pos_y = y;}
	float getPosY(){return m_pos_y;}
	void setArea(float area){m_area = area;}
	float getArea(){return m_area;}
protected:
	virtual ~PassCheck(){}
	char  m_type;	//据点类型---0.粮仓 1. 关隘
	short m_score; // 当前占领值 -limit ---- limit 负数为攻击方 正数为防守方
	float m_pos_x; // 坐标点x
	float m_pos_y; // 坐标点y
	float m_area; // 作用范围
};

typedef std::vector< CRCore::ref_ptr<PassCheck> > PassCheckVec;
class crJXJReliveItem : public CRCore::Referenced
{
public:
	crJXJReliveItem():m_instanceid(0),m_count(0),m_hpscale(1.0f),m_attackscale(1.0f),m_camp(1){}
	void setInstanceID(int instanceid) { m_instanceid = instanceid; }
	int getInstanceID(){ return m_instanceid; }
	void setCount(unsigned short count){ m_count = count; }
	unsigned short getCount(){ return m_count; }
	void setHPScale(float hpscale){ m_hpscale = hpscale; }
	float getHPScale(){ return m_hpscale; }
	void setAttackScale(float attackscale){ m_attackscale = attackscale; }
	float getAttackScale(){ return m_attackscale; }
	void setName(std::string name){m_name = name;}
	std::string getName(){return m_name;}
	void setCamp(unsigned char camp){ m_camp = camp; }
	unsigned char getCamp(){ return m_camp; }
	void setNpcItemType(unsigned char type){m_npcItemType = type;}
	unsigned char getNpcItemType(){return m_npcItemType;}
protected:
	int m_instanceid;
	std::string m_name;
	unsigned short m_count;
	float m_hpscale;
	float m_attackscale;
	unsigned char m_camp;//0:中立，1：守，2：攻
	unsigned char m_npcItemType;
};
typedef std::multimap< short,CRCore::ref_ptr<crJXJReliveItem> > ReliveItemMap;
typedef std::set<short> SaveProgressSet;

typedef std::map<CRCore::crVector3i,std::string> JXJCampBirthpoint;
typedef std::map<unsigned char,JXJCampBirthpoint> JXJCampBirthpointMap;//shiliid
typedef std::map<int,char> BattleInspireMap;		//playerid,鼓舞次数
typedef std::map<int,_crInt64> CampAddArmyTimerMap;//playerid,时间点
struct crJXJScenarioTimer : public CRCore::Referenced
{
	crJXJScenarioTimer(int msg,float interval,int times):m_msg(msg),m_interval(interval),m_times(times),m_timer(interval){}
	int m_msg;
	float m_interval;
	float m_timer;//倒计时
	int m_times;//-1表示无穷
};
//typedef std::multimap<int, crJXJScenarioTimer >ScenarioTimerMap;//id,msg,时间，触发次数（－1表示无穷,0表示未启用）
typedef std::map<int, CRCore::ref_ptr<crJXJScenarioTimer> >ScenarioTimerMap;//id,msg,时间，触发次数（－1表示无穷,0表示未启用）
typedef std::map<int, unsigned char> FubenStarMap;//playerid,stars
typedef std::map<unsigned char, int> NpcTotalCountMap;//shiliid,count
typedef std::map<std::string, float>ScenarioVariableMap;//nameid,value
//typedef std::set<int> PlayerSet;//多人PVP阵营数据
//typedef std::map<unsigned char,PlayerSet> PVPCampMap;//多人PVP阵营数据
typedef std::map<_crInt64, CRCore::crVector2f>PlayerCoordMap;//世界战场同势力队友坐标位置，playerid,roleid,坐标x,y
class crJXJBattleRoomData : public CRNetApp::crRoomData
{//client
public:
	crJXJBattleRoomData();
	crJXJBattleRoomData(const crJXJBattleRoomData& data);
	DataClass(JXJ, JXJBattleRoom)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	int m_battleid;//攻防势力ID，守方势力ID，城池ID，副本时（攻防势力ID，群势力ID，fubenid）
	BattleStatsMap m_battleStatsMap;
	//std::string m_checks_tabname;
	//short m_red_remain; // 红方粮草积分
	//short m_blue_remain; // 蓝方粮草积分
	//PassCheckVec m_checks; // 监测点 - 粮仓数据
	//bool m_modified;
	//bool m_is_init;
	//char m_isPassCapture;		//关隘是否被占领 0中立 -1 红方占领 1蓝方占领
	ReliveItemMap m_reliveItemMap;
	SaveProgressSet m_saveProgressSet;
	unsigned char m_roomType;//1:城站，2:副本，3:世界战场
	JXJCampBirthpointMap m_roomCampMap;
	BattleInspireMap m_battleInspireMap;
	int m_getFirstBloodPlayerId;//获得一血玩家
	CampAddArmyTimerMap m_campTimerMap;
	ScenarioTimerMap m_scenarioTimerMap;
	FubenStarMap m_fubenStarMap;
	NpcTotalCountMap m_totalNpcCount;//战场总NPC数
	ScenarioVariableMap m_scenarioVariableMap;
	CRCore::crVector2i m_battleRbuffId;//攻方犒赏玩家ID,守方犒赏玩家ID
	//PVPCampMap m_pvpcampmap;//阵营ID,  阵营playerid SET
	PlayerCoordMap m_playerCoordMap;//只是世界战场客户端有这个数据，世界战场同势力队友坐标位置
	unsigned short m_weipoints;  //只是世界战场有，魏国积分
	unsigned short m_shupoints;  //只是世界战场有，蜀国积分
	unsigned short m_wupoints;   //只是世界战场有，吴国积分
	std::map<int,std::set<int>> m_praisedid;  //世界战场中被本玩家赞过的其他玩家
	unsigned char m_temhuodong;   //当前活动
};
//////////////////////////////////////////////////////////////////////////
typedef std::vector< CRCore::ref_ptr<crBagItemData> > RewardItemVec;//int,unsigned char,unsigned char
typedef std::map<int, CRCore::ref_ptr<crBagItemData> > DeductItemMap;//int,unsigned char,unsigned char
//typedef std::map< int,CRCore::ref_ptr<crBagItemData> > RewardItemMap;
typedef std::vector< std::pair<unsigned short,int> > RewardEquipsVec;
typedef std::vector< std::pair<unsigned char, CRCore::crVector3i> > FubenDropRewardVec;//typeid,itemid/equipid/resid,equipmagic,count 从奖池中掉落，选取其中一样
class crPlayerRewardData : public CRCore::Referenced
{//client
public:
	crPlayerRewardData():m_type(GP_RewardGameEnd),m_playerid(0),m_coopercash(0),m_exploit(0),m_exprience(0),m_achievement(0),m_food(0),
		m_wood(0), m_iron(0), m_horse(0), m_giftGold(0), m_junling(0), m_countrycontribution(0), m_freerollcardcount(0), m_freerefinecount(0),m_score(0),m_huoyuedu(0),
		m_warcounts(false), m_groupcontribute(0)
	{
		//m_rewardItems.resize(0);
		//m_equips.resize(0);
	}
	crPlayerRewardData(crPlayerRewardData* data):
		m_type(data->m_type),m_playerid(data->m_playerid),m_coopercash(data->m_coopercash),m_exploit(data->m_exploit),m_exprience(data->m_exprience),
		m_achievement(data->m_achievement),m_food(data->m_food),m_wood(data->m_wood),m_iron(data->m_iron),m_horse(data->m_horse),m_giftGold(data->m_giftGold),
		m_junling(data->m_junling),m_countrycontribution(data->m_countrycontribution),
		m_rewardItems(data->m_rewardItems),m_equips(data->m_equips),m_keyWord(data->m_keyWord),
		m_fubenDropVec(data->m_fubenDropVec),
		m_freerollcardcount(data->m_freerollcardcount),
		m_freerefinecount(data->m_freerefinecount),
		m_score(data->m_score),
		m_huoyuedu(data->m_huoyuedu),
		m_warcounts(data->m_warcounts),
		m_groupcontribute(data->m_groupcontribute)
	{
	}
	void setPlayerID(int playerid){m_playerid = playerid;}
	int getPlayerID(){return m_playerid;}
	void setCoopercash(int coopercash){m_coopercash = coopercash;}
	int getCooperCashReward(){return m_coopercash;}
	void setExploit(int exploit){m_exploit = exploit;}
	int getExploit(){return m_exploit;}
	void setExperience(int experience){m_exprience = experience;}
	int getExperience(){return m_exprience;}
	void setAchievement(int achievement){m_achievement = achievement;}
	int getAchievement(){return m_achievement;}
	//void setRewadItemsVec(RewardItemVec& rewardItem){m_rewardItems = rewardItem;}
	RewardItemVec& getRewardItemsVec(){return m_rewardItems;}
	void setType(unsigned char type){m_type = type;}
	unsigned char getType(){return m_type;}
	void setKeyWord(std::string key){m_keyWord = key;}
	std::string &getKeyWord(){return m_keyWord;}
	void setFood(int food ){m_food = food;}
	int getFood(){return m_food;}
	void setWood(int wood){m_wood = wood;}
	int getWood(){return m_wood;}
	void setIron(int iron){m_iron = iron;}
	int getIron(){return m_iron;}
	void setHorse(int horse){m_horse = horse;}
	int getHorse(){return m_horse;}
	void setGiftgold(int giftgold){m_giftGold = giftgold;}
	int getGiftgold(){return m_giftGold;}
	//void setEquipsVec(RewardEquipsVec equips){m_equips = equips;}
	RewardEquipsVec& getEquipsVec(){return m_equips;}
	//void setFubenDropItems(FubenDropRewardVec dropitem){m_fubenDropItems = dropitem;}
	FubenDropRewardVec &getFubenDropVec(){return m_fubenDropVec;}
	void setJunling(int junling){m_junling = junling;}
	unsigned char getJunling(){return m_junling;}
	void setCountryContribution(int countrycontribution){m_countrycontribution = countrycontribution;}
	int getCountryContribution(){return m_countrycontribution;}
	void setGroupContribution(int value){ m_groupcontribute = value; }
	int getGroupContribution(){ return m_groupcontribute; }
	void buildGiftStream(CRCore::ref_ptr<CRCore::crStreamBuf> &stream);
	void setGiftDataStream(CRCore::crStreamBuf *stream);
	void parseRewardItemCount(int &cardcount,int &itemcount);
	void setTimer(time_t time){m_startTimer = time;}
	time_t getTimer(){return m_startTimer;}
	std::string &getDescribtion(){ return m_describtion; }
	void setFreerollcardcount(int freerollcardcount){ m_freerollcardcount = freerollcardcount; }
	unsigned char getFreerollcardcount(){ return m_freerollcardcount; }
	void setFreerefinecount(int freerefinecount){ m_freerefinecount = freerefinecount; }
	unsigned char getFreerefinecount(){ return m_freerefinecount; }
	void setScore(int score){m_score = score;}
	int getScore(){return m_score;}
	void setHuoyuedu(char huoyuedu){m_huoyuedu = huoyuedu;}
	char getHuoyuedu(){return m_huoyuedu;}
	void setwarcounts(bool var){m_warcounts = var;}
	bool getwarcounts(){return m_warcounts;}
protected:
	unsigned char m_type;			//奖励类型
	int m_playerid;					//玩家ID
	int m_coopercash;				//金钱
	int m_exploit;					//功勋
	int m_exprience;				//经验
	int m_achievement;				//战功
	int m_food;						//食物
	int m_wood;						//木材
	int m_iron;						//铁矿
	int m_horse;					//马匹
	int m_giftGold;					//礼金
	unsigned char m_junling;		//军令//需要去掉
	int m_countrycontribution;		//国家贡献
	RewardItemVec m_rewardItems;	//奖励物品
	RewardEquipsVec m_equips;		//T_TroopsEquip
	std::string m_keyWord;			//礼包KEY
	FubenDropRewardVec m_fubenDropVec;//副本掉落奖池,临时数据
	time_t m_startTimer;				//服务器奖励数据生成时间
	std::string m_describtion;		//奖励描述
	unsigned char m_freerollcardcount;//免费抽将卡次数
	unsigned char m_freerefinecount;//免费精炼次数
	int  m_score;                   //国战积分
	char m_huoyuedu;
	bool m_warcounts;				//国战次数是否+1
	int m_groupcontribute;//军团贡献
};

class crJXJUIGiftsPackMailHyperData :  public CRCore::crUIData
{
public:
	crJXJUIGiftsPackMailHyperData();
	crJXJUIGiftsPackMailHyperData(const crJXJUIGiftsPackMailHyperData& data);
	DataClass(JXJ, JXJUIGiftsPackMailHyper)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	std::string m_keyWords;
	RewardItemVec m_packItemVec;
	int m_reportId;
};
class crJXJNewHandUIData :  public CRCore::crUIData
{//GameClient Method
public:
	crJXJNewHandUIData();
	crJXJNewHandUIData(const crJXJNewHandUIData& data);
	DataClass(JXJ, JXJNewHandUI)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	GNE::Mutex m_dataMutex;
	int m_newHandTaskid;
	std::string m_targetCanvasName;
	std::string m_targetWidgetName;
};

typedef std::map<int,int> GroupChengChiEarningMap;					//军团收益 类型,数量

class crJXJConsortiaMember : public CRCore::Referenced
{
public:
	crJXJConsortiaMember(int playerid):m_playerid(playerid),m_lv(0),m_pos(0),m_grouppos(0),m_ifonline(0),
		m_curWeekActiveVal(0),m_lastWeekActiveVal(0),m_todayActiveVal(0),m_tribute(0),
		m_yesterdayActiveVal(0),m_getEarningsYesterday(true),
		m_curFuBen(0),m_curFuBenTimes(0),m_curFuBenValidTimes(0),m_curFuBenAchievement(0),
		m_curFuBenActiveVal(0),m_curFuBenCountryContribute(0),m_curFuBenExp(0),
		m_curFuBenExploit(0),m_curFuBenGroupFunds(0),m_newer(true),m_loginTime(time(0)),
		m_groupcontribute(0),m_troopstorednumT(0),m_trooptakednumT(0),m_curFuBenGroupContribute(0),
		m_fubencircle(0.0f)
	{ m_groupcantonspecialtyVec.resize(8); }
	unsigned char getGroupPos() const {return m_grouppos;}
	unsigned char getIfOnline() const {return m_ifonline;}
	unsigned char getLevel() const {return m_lv;}
	unsigned char getPos() const{return m_pos;}
	//unsigned char getContributeTimes() {return m_contributeTimes;}
	//unsigned char getContributeType() const {return m_contributeType;}

	void setName(const std::string &name){m_playername = name;}
	void setLevel(unsigned char lv){m_lv = lv;}
	void setPos(unsigned char pos){m_pos = pos;}
	void setTime(_crInt64 applytime){m_applytime = applytime;}
	void setGroupPos(unsigned char grouppos){m_grouppos = grouppos;}
	void setIfOnlie(unsigned char ifonline){m_ifonline = ifonline;}
	void setTribute(int tribute){m_tribute = tribute;}
	void setActiveValue(int activeValue) {m_todayActiveVal = activeValue;}
	void setLastWeekActive(int val) {m_lastWeekActiveVal = val;}
	void setTodayWeekActive(int val) {m_curWeekActiveVal = val;}
	void setYesterdayActive(int val) {m_yesterdayActiveVal = val;}
	void setEarningsYesterday(bool val) {m_getEarningsYesterday = val;}
	void setNewer(bool val) {m_newer = val;}
	void setLoginTime(time_t time) {m_loginTime = time;}
	void setGroupContribute(int groupcontribute) {m_groupcontribute = groupcontribute;}
	void setTroopStoredNumT(int num) {m_troopstorednumT = num;}
	void setTroopTakedNumT(int num) {m_trooptakednumT = num;}
	void dayZero();
	void weekZero();
	void clearJunTuanFuBenInfo() 
	{
		m_curFuBen = 0,m_curFuBenAchievement = 0,m_curFuBenActiveVal = 0,
		m_curFuBenCountryContribute = 0,m_curFuBenExp = 0,
		m_curFuBenExploit = 0,m_curFuBenGroupFunds = 0,
		m_curFuBenTimes = 0,m_curFuBenValidTimes = 0,m_curFuBenGroupContribute = 0;
		m_fubencircle = 0.0f;
	}

	_crInt64 getTime()const {return m_applytime;}
	time_t getLoginTime() {return m_loginTime;}
	int getTribute() const {return m_tribute;}
	int getPlayerID() const { return m_playerid; }
	int getActiveVaule( ) const {return m_todayActiveVal;}
	int getLastWeekActive() const {return m_lastWeekActiveVal;};
	int getTodayWeekActive() {return m_curWeekActiveVal ;}
	int getYesterdayActive() {return m_yesterdayActiveVal;}
	bool getEarningsYesterday() {return m_getEarningsYesterday;}
	bool getNewer() {return m_newer;}
	int getGroupContribute() {return m_groupcontribute; }
	int getTroopStoredNumT() {return m_troopstorednumT; }
	int getTroopTakedNumT() {return m_trooptakednumT; }
	GroupCantonSpecialtyVec & getGroupCantonSpecialtyVec() {return m_groupcantonspecialtyVec;}
	void clearGroupcantonspecialtyVec()
	{
		for( GroupCantonSpecialtyVec::iterator itr = m_groupcantonspecialtyVec.begin();
			itr != m_groupcantonspecialtyVec.end();
			++itr )
		{
			*itr = NULL;
		}
	}
	bool isGroupcantonspecialtyVecempty()
	{
		for( GroupCantonSpecialtyVec::iterator itr = m_groupcantonspecialtyVec.begin();
			itr != m_groupcantonspecialtyVec.end();
			++itr )
		{
			if(*itr != NULL)
				return false;
		}
		return true;
	}
	GroupChengChiEarningMap & getGroupChengChiEarningMap() {return m_groupchengchiearningMap;}

	std::string &getName() {return m_playername;}

	bool m_getEarningsYesterday;			// 昨日收益是否已经领取
	unsigned char m_lv;//等级
	unsigned char m_pos;//职位官职
	unsigned char m_grouppos;//团内职位 
	unsigned char m_ifonline;//在线状态,只客户端上使用
	//unsigned char m_contributeType;			// 捐献类型
	//unsigned char m_contributeTimes;		// 捐献次数
	unsigned char m_curFuBenTimes;			// 当前结算副本翻倍次数。
	unsigned char m_curFuBenValidTimes;		// 当前结算副本翻倍有效次数。

	int m_playerid;
	int m_curWeekActiveVal;					// 本周到昨天的活跃值总和
	int m_todayActiveVal;					// 今天的活跃值
	int m_yesterdayActiveVal;				// 昨日的活跃值
	int m_lastWeekActiveVal;				// 上周的活跃值总和
	int m_curFuBen;							// 当前结算的军团副本
	int m_curFuBenActiveVal;				// 当前副本奖励活跃值
	int m_curFuBenExp;						// 当前副本奖励经验
	int m_curFuBenAchievement;				// 当前副本奖励战功
	int m_curFuBenExploit;					// 当前副本奖励功勋
	int m_curFuBenGroupFunds;				// 当前副本奖励军团资金
	int m_curFuBenCountryContribute;		// 当前副本奖励国家贡献
	int m_curFuBenGroupContribute;			// 当前副本奖励军团贡献
	float m_fubencircle;					// 当前副本通关波数
	int m_tribute;//上贡资金
	_crInt64 m_applytime;
	int m_groupcontribute;					//军团贡献
	int m_troopstorednumT;					//今日已存放军团兵库兵的数量
	int m_trooptakednumT;					//今日已取出兵的数量 
	GroupCantonSpecialtyVec m_groupcantonspecialtyVec;	//军团管理界面的8个物品
	GroupChengChiEarningMap m_groupchengchiearningMap;	//军团城池收益

	std::string m_playername;
	bool m_newer;							// 今天刚加入
	time_t m_loginTime;		
};

class crJXJConsortiaApplyMember : public CRCore::Referenced
{
public:
	crJXJConsortiaApplyMember(int playerid):m_playerid(playerid),m_lv(0),m_pos(0){}
	int getPlayerID(){ return m_playerid; }
	void setName(const std::string &name){m_playername = name;}
	std::string &getName(){return m_playername;}
	unsigned char getLevel(){return m_lv;}
	void setLevel(unsigned char lv){m_lv = lv;}
	unsigned char getPos(){return m_pos;}
	void setPos(unsigned char pos){m_pos = pos;}
	void setTime(_crInt64 applytime){m_applytime = applytime;}
	_crInt64 getTime(){return m_applytime;}
protected:
	int m_playerid;
	std::string m_playername;
	_crInt64 m_applytime;
	unsigned char m_lv;//等级
	unsigned char m_pos;//职位
};



//class GroupEarnings : public CRCore::Referenced
//{
//public:
//	GroupEarnings():m_type(0),m_num(0){}
//	~GroupEarnings(){}
//	//short m_earningsScale;	// 收益比例
//	//int m_cantonId;			// 所在州
//	int m_type;				//  收益类型
//	int m_num;				//  收益数量
//};

typedef std::vector< CRCore::ref_ptr<crJXJConsortiaApplyMember> > ConsortiaApplyVec;
typedef std::vector < int > GroupLeaderVec;
typedef std::map<int,CRCore::ref_ptr<crJXJConsortiaMember> ,std::greater<int> > ConsortiaMemberMap;//playerid,
typedef std::vector<CRCore::ref_ptr<crJXJConsortiaMember> > ConsortiaMemberVec;
typedef std::map<int,short> ApplyCantonMap;					//  军团申请驻扎的州id和收益
typedef std::map<int,short> FenFengedCantonMap;				//  军团被分封的州的id和收益
typedef std::set<int> FenFengedChengChiSet;					// 军团被分封的城池
typedef std::map<int,int> FenFengedChengChiMap;				//军团分封给成员的城池	chengchiid和 playerid
//typedef std::set<int> StartedFubenSet;
//typedef std::vector< CRCore::ref_ptr<GroupEarnings> > GroupEarningsVec;				// 军团收益
typedef std::map<int,unsigned char> ConsortiaPlayerReliveCount;//playerid,复活次数
typedef std::map<unsigned short,std::pair<int,ConsortiaPlayerReliveCount> > StartedFubenMap;//副本id 和 roomid，玩家复活次数

class ConsortiaActiveRankItem : public CRCore::Referenced
{
public:
	ConsortiaActiveRankItem(int id):
	m_groupPos(0),m_id(id),m_activeVal(0)
	{}
	~ConsortiaActiveRankItem(){}
	int getKey() const {return m_id;}
	unsigned char m_groupPos;
	int m_id;
	int m_activeVal;
	std::string m_name;
};

class GroupActiveValueCompare
{
public:
	bool operator ()(const CRCore::ref_ptr<ConsortiaActiveRankItem> & left,
		const CRCore::ref_ptr<ConsortiaActiveRankItem> & right) 
	{
		if (left->m_activeVal == right->m_activeVal)
			return left->m_groupPos > right->m_groupPos;
		return left->m_activeVal > right->m_activeVal;
	}
};

typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaActiveRankItem>,GroupActiveValueCompare> ConsortiaActiveRank;

class ConsortiaMemberRankItem: public ConsortiaActiveRankItem
{
public:
	ConsortiaMemberRankItem(int id):ConsortiaActiveRankItem(id),m_level(0){}
	void fullOfData(crJXJConsortiaMember * memberinfo)
	{
		m_id = memberinfo->getPlayerID();
		m_activeVal = memberinfo->getTodayWeekActive();
		m_groupPos = memberinfo->getGroupPos();
		m_level = memberinfo->getLevel();
		m_name = memberinfo->getName();
		m_govPos = memberinfo->getPos();
	}
	unsigned char m_level;
	int m_govPos;
};

class GroupMemberRankCompare
{
public:
	bool operator () (const CRCore::ref_ptr<ConsortiaMemberRankItem > & left,
		const CRCore::ref_ptr<ConsortiaMemberRankItem > & right)
	{
		if (left->m_groupPos == right->m_groupPos)
		{
			if (left->m_level == right->m_level)
			{
				return left->m_activeVal > right->m_activeVal;
			}
			return left->m_level > right->m_level;
		}
		return left->m_groupPos > right->m_groupPos;
	}
};
typedef crJXJRankContainer<CRCore::ref_ptr<ConsortiaMemberRankItem>,GroupMemberRankCompare > ConsortiaMemberRank;
typedef std::deque<std::string> ConsortiaRecordsDeq;

class crJXJConsortiaData :  public CRCore::crData
{//GameServer & GameClient Method
public:
	crJXJConsortiaData();
	crJXJConsortiaData(const crJXJConsortiaData& data);
	DataClass(JXJ, JXJConsortia)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
	JXJGroupPos getGroupPos(int playerid);
	void dayZero();
	void weekZero();
	void twoSecZero();
	void clearRankCache();
	void settleEarnings();

	static  void split( std::string str,std::string pattern ,std::vector<std::string> &);
protected:
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;
	bool m_modify;//不存数据库
	bool m_yesterdayIsCantonGroup;			// 昨日是否是镇守军团
	unsigned char m_lv;//军团等级
	unsigned char m_manorlevel;//领地等级
	unsigned char m_barrackslevel;//兵营等级
	unsigned char m_shiliid;//军团势力ID
	short m_earningsScaleY;					// 昨日收益比例
	short m_earningsScaleT;					// 今日收益比例
	short m_totalChengChiNum;				// 总的被占领城池数
	
	int m_funds;		// 军团资金
	int m_dismissTime;				// -1 未解散 >=0 解散倒计时
	int m_totalActiveValY;				// 昨日成员活跃值总和

	std::string m_recruitNotice;		// 招募公告
	std::string m_contactNotice;		// 联系公告
	std::string m_notice;				// 一般公告
	std::string m_groupName;			// 军团名称
	std::string m_fengyi;				// 军团封邑

	static GNE::Mutex m_saveMutex;
	GNE::Mutex m_dataMutex;
	ConsortiaMemberMap m_consortiaMemberMap;
	ConsortiaActiveRank m_activeRank;				// 周活跃值排行
	ConsortiaApplyVec m_applyvec;							//申请列表
	GroupLeaderVec m_groupLeaderVec;				// 军团长，副团长 0 军团长，1 副团长，2 副团长
	/*StartedFubenSet m_startedFuBenSet;*/				// 已经开启的副本
	StartedFubenMap m_startedFuBenMap;				////副本id 和 roomid，玩家复活次数

	FenFengedCantonMap m_fenFengedCantonMapT;		// 镇守的州 和 收益 今日
	FenFengedCantonMap m_fenFengedCantonMapY;		// 镇守的州 和 收益 昨日 
	ApplyCantonMap m_applyCantonMapT;				// 申请驻扎的州，和收益 今日
	ApplyCantonMap m_applyCantonMapY;				// 申请驻扎的州，和收益 昨日 
	FenFengedChengChiSet m_fenFengedChengChiSet;	// 被分封的城池
	/*GroupEarningsVec m_earningsVec;	*/				// 军团收益
	ConsortiaMemberRank m_memberRank;				// 用于显示，不存储
	_crInt64 m_dayLastUpdate;							// 上次更新时间
	_crInt64 m_weekLastUpdate;						// 上次周更新时间
	unsigned char m_conquest;						//军团征伐值 上限为7
	TroopsMap m_grouptroopsMap;						//军团兵库
	FenFengedChengChiMap m_fenFengedChengChiMap;	//军团分封给成员的 城池id 和playerid
	ConsortiaRecordsDeq m_consortiarecordsDeq;		//军团存取兵记录
};

class ChengChiEarnings : public CRCore::Referenced
{
public:
	ChengChiEarnings(int groupid);
	int getGroupId() {return m_cantonGroupId;}
	int getChengChiId() {return m_chengChiId;}
	void setChengChiId(int id) {m_chengChiId = id;}
	short getEarnings() {return m_earnings;}
	void setEarnings(short earnings) {m_earnings = earnings;}

	int m_cantonGroupId;				// 驻扎的军团id
	int m_chengChiId;			// 城池 id 可以为0 说明没有分配 城池
	short m_earnings;				// 收益比例
};

typedef std::map<int,CRCore::ref_ptr<ChengChiEarnings>> ChengChiEarningsMap;//consortiaID

class Canton:public CRCore::Referenced
{
public:
	Canton();
	void setCantonId(int id) {m_cantonId = id;}
	int getCantonId() {return m_cantonId;}
	void setConsortiaId(int id) {m_consortiaID = id;}
	int getConsortiaId() {return m_consortiaID;}
	short reCalculateEarnings();
	ChengChiEarningsMap & getChengChiEarningsMap() {return m_chengChiEarningsMap;}
	int m_cantonId;
	int m_consortiaID;//州长军团
	ChengChiEarningsMap m_chengChiEarningsMap;
};
typedef std::vector<int> SuperEquipExpVec;
class PeopleInfo : public CRCore::Referenced
{
public:
	PeopleInfo(int id,const std::string & name,unsigned char shili):m_id(id),m_name(name),m_shili(shili)
	{
		m_lv = 1;
		m_troopsLv = 0;
		m_consortiaId = 0;
		m_isSmallAcc = false;
		m_killNum = 0;
		m_newerStep = 0;
		m_deadTroopsNum = 0;
		m_giftGold = 0;
		m_totalRecharge = 0;
		m_troopsnum = 0;
		m_exploit = 0;
	}
	PeopleInfo()
	{
		m_id = 0;
		m_lv = 1;
		m_troopsLv = 0;
		m_consortiaId = 0;
		m_isSmallAcc = false;
		m_killNum = 0;
		m_newerStep = 0;
		m_deadTroopsNum = 0;
		m_shili = 0;
		m_giftGold = 0;
		m_totalRecharge = 0;
		m_troopsnum = 0;
		m_exploit = 0;
	}
	PeopleInfo( const PeopleInfo & info)
	{
		m_isSmallAcc = info.m_isSmallAcc;
		m_lv = info.m_lv;
		m_troopsLv = info.m_troopsLv;
		m_id = info.m_id;
		m_consortiaId = info.m_consortiaId;
		m_killNum = info.m_killNum;
		m_name = info.m_name;
		m_consortiaName = info.m_consortiaName;
		m_deadTroopsNum = info.m_deadTroopsNum;
		m_shili = info.m_shili;
		m_giftGold = info.m_giftGold;
		m_troopsnum = info.m_troopsnum;
		m_exploit = info.m_exploit;
	}

	void setSamllAcc(bool smallAcc,bool forceModify = true) { if(m_isSmallAcc == smallAcc) return; m_isSmallAcc = smallAcc; if (forceModify) needUpdate(); }
	void setLv(unsigned char lv,bool forceModify = true) { if(m_lv == lv) return; m_lv = lv; if (forceModify) needUpdate();}
	void setTroopsLv(unsigned char lv,bool forceModify = true) { if(m_troopsLv == lv) return; m_troopsLv = lv; if (forceModify) needUpdate();}
	void setConsortiaID(int id,bool forceModify = true) { if(m_consortiaId == id) return; m_consortiaId = id; if (forceModify) needUpdate();}
	void setKillNum(int num,bool forceModify = true) { if(m_killNum == num) return; m_killNum = num; if (m_killNum < 0) m_killNum = 0;if (forceModify) needUpdate();}
	void setName(const std::string &name,bool forceModify = true) { if(m_name == name) return; m_name = name;if (forceModify) needUpdate();}
	void setConsortiaName(const std::string &name) { if(m_consortiaName == name) return; m_consortiaName = name;}
	void setNewrStep(int step,bool forceModify = true) { if(m_newerStep == step) return; m_newerStep = step; if (forceModify) needUpdate();}
	void setDeadTroopsNum(int num,bool forceModify = true) { if(m_deadTroopsNum == num) return; m_deadTroopsNum = num; if (forceModify) needUpdate();}
	void setID(int id) {m_id = id;}

	void setShili(unsigned char shili) {m_shili = shili;}
	void setGiftGold(int giftGold,bool forceModify = true) { if(m_giftGold == giftGold) return; m_giftGold = giftGold;if (forceModify) needUpdate();}
	void setTotalRecharge(int recharge,bool forceModify = true) { if(m_totalRecharge == recharge) return; m_totalRecharge = recharge;if (forceModify) needUpdate();}
	void setTroopsnum(int num) {m_troopsnum = num; }
	void setExploit(int expolit) {m_exploit = expolit; }
	void setSuperEquipLvupPoint(unsigned char point, bool forceModify = true) { if (m_superEquipLvupPoint == point) return; m_superEquipLvupPoint = point; if (forceModify) needUpdate(); }
	void setSuperEquipExpVec(SuperEquipExpVec &superEquipExpVec, bool forceModify = true) { if (m_superEquipExpVec == superEquipExpVec) return; m_superEquipExpVec = superEquipExpVec; if (forceModify) needUpdate(); }

	bool getSmallAcc() {return m_isSmallAcc;}
	unsigned char getLv() {return m_lv;}
	unsigned char getTroopsLv() {return m_troopsLv;}
	int getID() {return m_id;}
	int getConsortiaId() {return m_consortiaId;}
	int getKillNum() {return m_killNum;}
	const std::string & getName() {return m_name;}
	const std::string & getConsortiaName() {return m_consortiaName;}
	int getNewerStep() {return m_newerStep;}
	int getDeadTroopsNum() {return m_deadTroopsNum;}
	unsigned char getShili() {return m_shili;}
	int getGiftGold() {return m_giftGold;}
	int getTotalRecharge() {return m_totalRecharge;}
	int getTroopsnum() {return m_troopsnum;}
	int getExploit() {return m_exploit;}
	unsigned char getSuperEquipLvupPoint() { return m_superEquipLvupPoint; }
	SuperEquipExpVec& getSuperEquipExpVec() { return m_superEquipExpVec; }
	std::string getSuper()
	{
		return CRCore::crArgumentParser::appItoa(m_superEquipLvupPoint) + "|" + CRCore::crArgumentParser::appVectoa(m_superEquipExpVec,'|');
	}
	void needUpdate();
	void needInsert();

	friend class crJXJShiliData ;
private:
	

	bool m_isSmallAcc;						// 是否被标记成小号
	unsigned char m_lv;						// 等级
	unsigned char m_troopsLv;				// 所在兵营等级
	int m_id;								// 玩家id
	int m_consortiaId;						// 军团id
	int m_killNum;							// 国战杀敌数
	std::string m_name;						// 名字
	std::string m_consortiaName;			// 军团名称		// 不存储
	int m_newerStep;						// 已经完成的新手步骤
	int m_deadTroopsNum;					// 玩家在所有战斗中死亡的兵力数量.
	int m_giftGold;							// 玩家礼金
	int m_totalRecharge;					// 玩家累计充值
	int m_troopsnum;						// 玩家当前兵力
	int m_exploit;							// 玩家当前功勋
	unsigned char m_superEquipLvupPoint;//神器等级升级获得注灵点数（1,2,3,4）
	SuperEquipExpVec m_superEquipExpVec;//神器经验

	unsigned char m_shili;					
};

class PeopleInfoCmp
{
public:
	bool operator() (CRCore::ref_ptr<PeopleInfo> & left,CRCore::ref_ptr<PeopleInfo> right)
	{
		return left->getLv() < right->getLv();
	}
};

typedef std::map <int,CRCore::ref_ptr<Canton> > CantonMap;				// 州，城池 军团 收益map
typedef std::deque< unsigned short > ShiliCityChangeDeq;//城池id
typedef std::map<int,CRCore::ref_ptr<PeopleInfo > > PeopleMap;//playerid
typedef std::vector<CRCore::ref_ptr<PeopleInfo> > PeopleVec;
typedef std::deque< int > PeopleDeq;//playerid
struct strToupiaoInfo :public CRCore::Referenced
{
	int m_rankVal;
	int nPlayerID;
	std::string playerName;
	int nJuntuanID;
	std::string juntuanName;
	int nNumber;//票数
	int m_level;
	time_t m_lastLogin;
	strToupiaoInfo()
	{
		m_rankVal = 0;
		nPlayerID = 0;
		playerName = "";
		nJuntuanID = 0;
		juntuanName = "";
	//	nRankNumber = 0;
		nNumber = 0;
		m_level = 0;
		m_lastLogin = 0;
	}
};

class strToupiaoInfoCmp 
{
public:
	bool operator() (const CRCore::ref_ptr<strToupiaoInfo> & left,const CRCore::ref_ptr<strToupiaoInfo> & right)
	{
		if (left->nNumber == right->nNumber)
		{
			//return left->m_lastLogin > left->m_lastLogin;
			return left->m_rankVal < right->m_rankVal;
		}
		return left->nNumber > right->nNumber;
	}
};

//typedef std::map<int, CRCore::ref_ptr<strToupiaoInfo> > ToupiaoData;//投票数据
//struct strInfo :public CRCore::Referenced
//{
//	int m_rankVal;
//	int nPlayerID;
//	std::string playerName;
//	int nJuntuanID;
//	std::string junTuanName;
//	strInfo()
//	{
//		m_rankVal = 0;
//		nPlayerID = 0;
//		playerName = "";
//		nJuntuanID = 0;
//		junTuanName = "";
//	}
//};

class CountryHistoryInfoBase :public CRCore::Referenced
{
public:
	typedef CountryHistoryInfoBase Base;
	CountryHistoryInfoBase();;
	inline unsigned char getInfoType();
	inline void setTime( time_t time );
	inline void setInfoId(int id);

	virtual void getCompleteString(std::string &) = 0;
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual unsigned char buildSaveStream(std::vector<std::string> & strVec);
	virtual unsigned char getSaveStream(std::vector<std::string> & strVec);
	inline CRCore::crHandle * getNoticeHandle();

	unsigned char m_infoType;
	int m_infoId;
	time_t m_time;
};

class CountryHistoryOfficalChange : public CountryHistoryInfoBase
{
public:
	CountryHistoryOfficalChange();
	virtual void getCompleteString(std::string & outString );
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual unsigned char buildSaveStream(std::vector<std::string> & strVec);
	virtual unsigned char getSaveStream(std::vector<std::string> & strVec);
	std::string m_playerName;
	std::string m_officalName;
};

class CountryHistoryBenefitChange : public CountryHistoryInfoBase
{
public:
	CountryHistoryBenefitChange();
	virtual void getCompleteString(std::string & outString );
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual unsigned char buildSaveStream(std::vector<std::string> & strVec);
	virtual unsigned char getSaveStream(std::vector<std::string> & strVec);
	std::string m_officalName;
	std::string m_playerName;
};


class CountryHistoryChengChiChange : public CountryHistoryInfoBase
{
public:
	CountryHistoryChengChiChange ();
	virtual void getCompleteString(std::string & outString );
	virtual void readContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual void writeContent(CRCore::ref_ptr<CRCore::crStreamBuf> & stream);
	virtual unsigned char buildSaveStream(std::vector<std::string> & strVec);
	virtual unsigned char getSaveStream(std::vector<std::string> & strVec);
	bool m_winOrlose;					// true -> win,false -> lose
	int m_chengchiId;
	int m_shiliid;
	//std::string m_officalName;
	//std::string m_playerName;
};

class CountryHistoryManager
{
public:
	void operator()(unsigned char type,CRCore::ref_ptr<CountryHistoryInfoBase> & newBase)
	{
		switch (type)
		{
		case JXJCountryHistoryType_Offical:
			newBase =  new CountryHistoryOfficalChange;
			break;
		case JXJCountryHistoryType_Benefit:
			newBase = new CountryHistoryBenefitChange;
			break;
		case JXJCountryHistoryType_Chengchi:
			newBase = new CountryHistoryChengChiChange ;
			break;
		default:
			newBase = NULL;
			break;
		}
	}
};

typedef std::deque<CRCore::ref_ptr<CountryHistoryInfoBase > > CountryHistory;		
typedef std::vector<CRCore::ref_ptr<strToupiaoInfo> > CandidateVec;//投票候选人
typedef std::set<int> VotersSet;								// 投过票的玩家id
typedef std::set<int> ShiliBlacklistSet;						// 小号标记

typedef std::set<int> InsertPlayerSet;							// 新加入国民列表的玩家id
typedef std::set<int> ModifyPlayerSet;							// 有数据修改的玩家id

struct strConsortiaHuoDongInfo:public CRCore::Referenced 
{
	int nPlayerId;
	unsigned char rankNum;
	bool isConsortiaLeader;
	int groupId;
	strConsortiaHuoDongInfo(){
		nPlayerId = 0;
		rankNum = 0;
		isConsortiaLeader = false;
		groupId = 0;
	}
};
typedef std::vector<CRCore::ref_ptr<strConsortiaHuoDongInfo> > ConsortiaStartPromotionVec;				// 军团开服活动满足资格玩家
typedef std::vector<CRCore::ref_ptr<strConsortiaHuoDongInfo> > ConsortiaStartPromotionRankVec;			// 军团开服活动入围军团

class crJXJShiliData :  public CRCore::crData
{//GameServer & GameClient Method
public:
	crJXJShiliData();
	crJXJShiliData(const crJXJShiliData& data);
	DataClass(JXJ, JXJShili)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
	JXJCountryOfficial getOfficialDes(int id);					// 得到官职
	std::string getOfficialStr( int id );
protected:
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_dataStream;
	static GNE::Mutex m_saveMutex;
	GNE::Mutex m_dataMutex;
	bool m_modify;//不存数据库
	bool m_startedFete;								// 是否开启祭祀	
	bool m_startedEnrichCountry;					// 是否开启富国
	short m_smallAccLimit;							// 小号保护上限
	int m_chengxiang;//丞相
	int m_dasima;//大司马
	int m_dajiangjun;//大将军
	int m_sijintai;//司金台
	int m_shennongtai;//神农台
	int m_simajian;//司马监
	int m_lubansi;//鲁班祠
	int m_dianjiangtai;//点将台
	int m_version;		// 不存储
	unsigned char m_winCity;	// 位标记 ，建国之路八个城市

	_crInt64 m_chengxiangLastLogin;
	_crInt64 m_dasimaLastLogin;
	_crInt64 m_dajiangjunLastLogin;

	_crInt64 m_shiliDayLastZero;//仅存取，等于GameServerData里的m_shiliDayUpdate

	std::string m_chengxiangName;
	std::string m_dasimaName;
	std::string m_dajiangjunName;
	std::string m_notice;

	CantonMap m_cantonMap;// 州，城池 军团 收益map
	ShiliCityChangeDeq m_wincitydeq;
	ShiliCityChangeDeq m_lostcitydeq;
	PeopleMap m_peopleMap;//国民列表
	ShiliBlacklistSet m_blacklistSet;				// 小号列表
	VotersSet m_votersSet;							// 投票者id set
	//PeopleMap m_peopleToupiaoMap;//已投票玩家ID
	//CandidateVec m_toupiaoData;//本周投票数据
	//CandidateVec m_toupiaoResultLastweek;//上周投票结果
	CandidateVec m_candidateVecT;					//本周官员候选人列表
	CandidateVec m_candidateVecL;					//上周官员候选人列表
	CountryHistory m_countryHistory;

	PeopleVec m_tempPeopleVec;						// 缓存国民列表,不存入数据库， 以等级为索引 ，供前端查询
	UIQueryFindByMinLvMap m_queryFindbyMinLvMap;
	UIQueryFindByMaxLvMap m_queryFindbyMaxLvMap;
	
	ModifyPlayerSet m_modifyPlayerSet;
	InsertPlayerSet m_insertPlayerSet;			// 国民列表中新增的玩家id
	unsigned char m_cZhanLingChengChi;          //国家每周占领城池数
	int m_guojiazijn;                           //国家资金
	CRCore::crVector3i m_guojiajifen;           //国家积分0、上午，1、下午、2、晚上
	char m_destroycountryid1;                   //本势力灭亡的国家id
	char m_destroycountryid2;					//本势力灭亡的国家id
	ConsortiaStartPromotionVec m_consortiaStartPromotionVecT;    //军团开服活动满足要求玩家id
	ConsortiaStartPromotionRankVec m_consortiaPromotionRankVecT;  //军团开服活动入围军团
	std::set<int> m_todayhavelandedplayer;     //当日登陆的玩家集合  不存储
	unsigned char m_cyesterdayconqueredcity;         //昨天占领的城池数量
	unsigned char m_ctodayconqueredcity;         //今天占领的城池数量 不存储
	unsigned char m_countrystrength;            //国家的强弱,0弱国，1普通，2强国，3帝国
};

class crJXJSelectServerUIData :  public CRCore::crUIData
{//GameClient Method
public:
	crJXJSelectServerUIData();
	crJXJSelectServerUIData(const crJXJSelectServerUIData& data);
	DataClass(JXJ, JXJSelectServerUI)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	CRNetApp::ServerListMap m_serverListMap;
	int m_curpage;
	int m_totalpage;
};
typedef std::vector<int> SuperEquipCycleExpVec;
class crJXJPlayerStoreData :  public CRCore::crData
{//GameServer & GameClient Method
public:
	crJXJPlayerStoreData();
	crJXJPlayerStoreData(const crJXJPlayerStoreData& data);
	DataClass(JXJ, JXJPlayerStore)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	static GNE::Mutex m_saveMutex;
	GNE::Mutex m_dataMutex;
	int m_money;//轮回仓库里的元宝数量
	// 背包列表
	//unsigned char m_itembagnum;
	JXJStoreItemBagVec m_itembagvec;
	//// 将卡背包列表
	//unsigned char m_jiangkabagnum;
	JXJStoreItemBagVec m_jiangkabagvec;
	int m_playerUsedMoney;//该角色总共消费的元宝
	unsigned short m_cyclecount;
	int m_vipexp;//vip经验
	unsigned char m_superEquipCycleLvupPoint;//轮回神器等级升级获得注灵点数（0,1,2,3,4）
	SuperEquipCycleExpVec m_superEquipCycleExpVec;//神器轮回经验
};

struct BattleRecruitCheckResult
{
	BattleRecruitCheckResult():
		m_code(-1),
		m_copperneed(0),
		m_woodneed(0),
		m_ironneed(0),
		m_horseneed(0),
		m_foodneed(0),
		m_timeneed(0),
		m_goldneed(0),
		m_shijibuycount(0)
	{}
	//0:兵源充足VIP0补兵需要额外粮草 1:兵源充足 2:兵力不足，资源充足，直接扣除资源补兵 3:资源不足，扣除元宝补兵
	//-1:不能补兵 -2：粮草不足 -3:元宝不足 -4:资源不足 -6:铜钱不足 -7:今日可购资源量不足，不能购买！ -8:资源够买失败
	//-12:不能招募
	char m_code;
	int m_copperneed;
	int m_foodneed;
	int m_woodneed;
	int m_ironneed;
	int m_horseneed;
	int m_timeneed;
	int m_goldneed;
	int m_shijibuycount;
};
class crJXJExitSceneServerUIData :  public CRCore::crUIData
{//GameClient Method
public:
	crJXJExitSceneServerUIData();
	crJXJExitSceneServerUIData(const crJXJExitSceneServerUIData& data);
	DataClass(JXJ, JXJExitSceneServerUI)
	virtual void addParam(int i, const std::string& str);
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	int m_codetext;
	short m_exittimer;
};

class crJXJTipsUIData : public CRCore::crUIData
{//client
public:
	crJXJTipsUIData();
	crJXJTipsUIData(const crJXJTipsUIData& data);
	DataClass(JXJ, JXJTipsUI)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	int m_id;
	int m_count;
};
class crJXJBattleRankingUIData : public CRCore::crUIData
{//client
public:
	crJXJBattleRankingUIData();
	crJXJBattleRankingUIData(const crJXJBattleRankingUIData& data);
	DataClass(JXJ, JXJBattleRankingUI)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
	virtual void excHandle(CREPARAM msg);
protected:
	GNE::Mutex m_dataMutex;
	BattleRankingMap m_battleRankingMap;
	short m_mypos;
	int m_killtotal;
	int m_deadtotal;
	int m_myJifen;
	int m_myKill;
	int m_myDead;
	int m_myExploit;
	bool m_myLingqu;
	char m_battlerankingVer;
};
class crJXJUIIDData : public CRCore::crUIData
{//client
public:
	crJXJUIIDData();
	crJXJUIIDData(const crJXJUIIDData& data);
	DataClass(JXJ, JXJUIID)
	virtual void inputParam(int i, void *param);
	virtual void getParam(int i, void*& param);
protected:
	int m_id;
};
}
#endif