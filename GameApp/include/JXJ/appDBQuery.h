/**********************************************************************
*
*	文件:	   appDBQuery.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*	HISTORY:		Created 2013.6.4
*
**********************************************************************/
#ifndef JXJ_DBQUERY_H
#define JXJ_DBQUERY_H 1

#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crStreamBuf.h>
#include <string>
#include <JXJ/appData.h>
namespace JXJ{
/////////////
class crMailQueryData : public CRDataBase::crQueryData
{
public:
	crMailQueryData();
	crMailQueryData(const crMailQueryData& data);
	DataClass(JXJ, MailQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int start,int count);
	int getID();
	const std::string &getSenderName() const;
	const std::string &getTitle() const;
	unsigned char getStats();
	const std::string &getCreateDate() const;
protected:
	int m_id;
	std::string m_sendername;
	std::string m_title;
	unsigned char m_stats;
	std::string m_createdate;
};

class crMailByStatsQueryData : public CRDataBase::crQueryData
{
public:
	crMailByStatsQueryData();
	crMailByStatsQueryData(const crMailByStatsQueryData& data);
	DataClass(JXJ, MailByStatsQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,unsigned char stats,int start,int count);
	int getID();
	const std::string &getSenderName() const;
	const std::string &getTitle() const;
	const std::string &getCreateDate() const;
protected:
	int m_id;
	std::string m_sendername;
	std::string m_title;
	std::string m_createdate;
};

class crCountMailQueryData : public CRDataBase::crQueryData
{
public:
	crCountMailQueryData();
	crCountMailQueryData(const crCountMailQueryData& data);
	DataClass(JXJ, CountMailQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	int getCount();
protected:
	int m_count;
};

class crCountMailByStatsQueryData : public CRDataBase::crQueryData
{
public:
	crCountMailByStatsQueryData();
	crCountMailByStatsQueryData(const crCountMailByStatsQueryData& data);
	DataClass(JXJ, CountMailByStatsQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,unsigned char stats);
	int getCount();
protected:
	int m_count;
};
class crMailBodyQueryData : public CRDataBase::crQueryData
{
public:
	crMailBodyQueryData();
	crMailBodyQueryData(const crMailBodyQueryData& data);
	DataClass(JXJ, MailBodyQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	CRCore::crStreamBuf *getBody();
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_body;
};
class crGiftsPackKeyValidQueryData : public CRDataBase::crQueryData
{//单组
public:
	crGiftsPackKeyValidQueryData();
	crGiftsPackKeyValidQueryData(const crGiftsPackKeyValidQueryData& data);
	DataClass(JXJ, GiftsPackKeyValidQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &key);
	int getCount();
protected:
	int m_count;
};
class crGiftsPackQueryData : public CRDataBase::crQueryData
{//单组
public:
	crGiftsPackQueryData();
	crGiftsPackQueryData(const crGiftsPackQueryData& data);
	DataClass(JXJ, GiftsPackQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &key);
	int getID();
	int getPlayerID();
	int getDistill();
	CRCore::crStreamBuf *getPack();
protected:
	int m_id;
	int m_playerid;
	int m_distill;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_pack;
};
class crGiftsPackOverdueQueryData : public CRDataBase::crQueryData
{//多组
public:
	crGiftsPackOverdueQueryData();
	crGiftsPackOverdueQueryData(const crGiftsPackOverdueQueryData& data);
	DataClass(JXJ, GiftsPackOverdueQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int overdue);
	int getID();
	const std::string &getKey()const;
protected:
	int m_id;
	std::string m_key;
};
class crMailOverdueQueryData : public CRDataBase::crQueryData
{//多组
public:
	crMailOverdueQueryData();
	crMailOverdueQueryData(const crMailOverdueQueryData& data);
	DataClass(JXJ, MailOverdueQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int overdue);
	int getID();
protected:
	int m_id;
};
class crConsortiaQueryData : public CRDataBase::crQueryData
{//多组
public:
	crConsortiaQueryData();
	crConsortiaQueryData(const crConsortiaQueryData& data);
	DataClass(JXJ, ConsortiaQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	int getID();
	CRCore::crStreamBuf *getStream();
	const std::string &getCreateDate();
	const std::string &getUpdateDate();
protected:
	int m_id;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	std::string m_createdate;
	std::string m_updateDate ;
};
class crChengchi2QueryData : public CRDataBase::crQueryData
{//多组
public:
	crChengchi2QueryData();
	crChengchi2QueryData(const crChengchi2QueryData& data);
	DataClass(JXJ, Chengchi2Query)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	unsigned short getID();
	//unsigned char getShili();
	//unsigned short getDefend();
	CRCore::crStreamBuf *getStream();
protected:
	unsigned short m_id;
	//unsigned char m_shili;
	//unsigned short m_defend;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
class crShiliQueryData : public CRDataBase::crQueryData
{//多组
public:
	crShiliQueryData();
	crShiliQueryData(const crShiliQueryData& data);
	DataClass(JXJ, ShiliQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	unsigned char getID();
	//int getKing();
	//const std::string& getKingName();
	//const std::string &getAffiche();
	CRCore::crStreamBuf *getStream();
protected:
	unsigned char m_id;
	//int m_king;
	//std::string m_kingname;
	//std::string m_affiche;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
/////////////////////////UpdateQuery
class crInsertMail : public CRDataBase::crUpdateSql
{
public:
	crInsertMail(){}
	void buildUpdateSql(int playerid, int senderid, const std::string &sendername, const std::string &title, CRCore::crStreamBuf *body,int stats = 0);//0=Mail_Recvd
};

class crUpdateMail : public CRDataBase::crUpdateSql
{
public:
	crUpdateMail(){}
	void buildUpdateSql(int id,unsigned char stats);
};

class crUpdateMailTitle : public CRDataBase::crUpdateSql
{
public:
	crUpdateMailTitle(){}
	void buildUpdateSql(int id,const std::string &title);
};

class crRemoveMail : public CRDataBase::crUpdateSql
{
public:
	crRemoveMail(){}
	void buildUpdateSql(int id);
	void buildUpdateSql_Overdue(int overdue);
};

class crInsertGiftsPack : public CRDataBase::crUpdateSql
{
public:
	crInsertGiftsPack(){}
	void buildUpdateSql(const std::string &key,CRCore::crStreamBuf *pack,int playerid=0);
};

class crUpdateGiftsPack : public CRDataBase::crUpdateSql
{
public:
	crUpdateGiftsPack(){}
	void buildUpdateSql(int id);
	void buildUpdateSql(const std::string &keyword);
};

class crRemoveGiftsPack : public CRDataBase::crUpdateSql
{
public:
	crRemoveGiftsPack(){}
	void buildUpdateSql(int id);
	void buildUpdateSql(const std::string &keyword);
	void buildUpdateSql_Overdue(int overdue);
};

class crInsertConsortia : public CRDataBase::crUpdateSql
{
public:
	crInsertConsortia(){}
	void buildUpdateSql(CRCore::crStreamBuf *stream);
};

class crUpdateConsortia : public CRDataBase::crUpdateSql
{
public:
	crUpdateConsortia(){}
	void buildUpdateSql(int id,CRCore::crStreamBuf *stream);
};

class crRemoveConsortia : public CRDataBase::crUpdateSql
{
public:
	crRemoveConsortia(){}
	void buildUpdateSql(int id);
};

class crUpdateChengchi2 : public CRDataBase::crUpdateSql
{
public:
	crUpdateChengchi2(){}
	//void buildUpdateSql(unsigned short id,unsigned char shili,unsigned short defend);
	void buildUpdateSql(unsigned short id,CRCore::crStreamBuf *stream);
};

class crUpdateShili : public CRDataBase::crUpdateSql
{
public:
	crUpdateShili(){}
	void buildUpdateSql(unsigned char id,CRCore::crStreamBuf *stream);
};
class crQueryPlayerData : public CRDataBase::crQueryData
{
public:
	crQueryPlayerData();
	crQueryPlayerData(const crQueryPlayerData& data);
	DataClass(JXJ, QueryPlayer)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	int getID();
	const std::string &getAccountName() const;
	const std::string &getLoginDate() const;
	const std::string &getCreateDate() const;
	const std::string &getOnlinetime() const;
	const std::string &getEmail() const;
protected:
	int m_id;
	std::string m_accountname;
	std::string m_logindate;
	std::string m_createdate;
	std::string m_onlinetime;
	std::string m_email;
};
class crPlayerGameStoreQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerGameStoreQueryData();
	crPlayerGameStoreQueryData(const crPlayerGameStoreQueryData& data);
	DataClass(JXJ, PlayerGameStoreQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int gameid);
	int getID();
	//CRCore::crStreamBuf *getStream();
	short getCyclecount();
	int getMoney();
	int getTotaladdmoney();
	JXJStoreItemBagVec &getCardBagVec();
	JXJStoreItemBagVec &getItemBagVec();
protected:
	int m_id;
	int m_money;
	int m_totaladdmoney;
	short m_cyclecount;
	//CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
	JXJStoreItemBagVec m_cardBagVec;
	JXJStoreItemBagVec m_itemBagVec;
};
class crPlayerGameStoreQuery2Data : public CRDataBase::crQueryData
{
public:
	crPlayerGameStoreQuery2Data();
	crPlayerGameStoreQuery2Data(const crPlayerGameStoreQuery2Data& data);
	DataClass(JXJ, PlayerGameStoreQuery2)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int gameid);
	int getID();
protected:
	int m_id;
};
class crPlayerGameStoreCycleCountQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerGameStoreCycleCountQueryData();
	crPlayerGameStoreCycleCountQueryData(const crPlayerGameStoreCycleCountQueryData& data);
	DataClass(JXJ, PlayerGameStoreCycleCountQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid, int gameid);
	short getCycleCount();
protected:
	short m_cycleCount;
};
// class crUpdatePlayerGameStoreCycleCount : public CRDataBase::crUpdateSql
// {
// public:
// 	crUpdatePlayerGameStoreCycleCount(){}
// 	void buildUpdateSql(int playerid, int gameid,int cyclecount);
// };

class crInsertPlayerGameStore : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerGameStore(){}
	void buildUpdateSql(int playerid, int gameid,JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec);
	void buildUpdateSql_Cycle(int playerid, int gameid, int cycleCountAdd, int money, int totaladdmoney);
};

class crInsertGameStoreDistillRecord : public CRDataBase::crUpdateSql
{
public:
	crInsertGameStoreDistillRecord(){}
	void buildUpdateSql(int playerid, int gameid, int money, int totaladdmoney, JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec);
};

class crUpdatePlayerGameStore : public CRDataBase::crUpdateSql
{
public:
	crUpdatePlayerGameStore(){}
	void buildUpdateSql(int id,int serverid,JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec);
	void buildUpdateSql_Cycle(int id,int serverid, int cycleCountAdd, int money, int totaladdmoney,JXJStoreItemBagVec & cardVec, JXJStoreItemBagVec & itemVec);
	void buildUpdateSql_Distill(int id,int distillmoney,int distilltotalmoney,std::vector<int>&cardVec,std::vector<int>&itemVec);
};

class crPeopleCycledQueryData : public CRDataBase::crQueryData
{
public:
	crPeopleCycledQueryData();
	crPeopleCycledQueryData(const crPeopleCycledQueryData& data);
	DataClass(JXJ, PeopleCycledQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	char getCycled();
protected:
	char m_cycled;
};
class crUpdatePeopleCycled : public CRDataBase::crUpdateSql
{
public:
	crUpdatePeopleCycled(){}
	void buildUpdateSql(int playerid);
};
class crPlayerTotalAddMoneyQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerTotalAddMoneyQueryData();
	crPlayerTotalAddMoneyQueryData(const crPlayerTotalAddMoneyQueryData& data);
	DataClass(JXJ, PlayerTotalAddMoneyQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int gameid);
	int getMoney();
protected:
	int m_money;
};
// class crUpdatePlayerStore : public CRDataBase::crUpdateSql
// {
// public:
// 	crUpdatePlayerStore(){}
// 	void buildUpdateSql(int id,CRCore::crStreamBuf *stream);
// };
// class crInsertPlayerStore : public CRDataBase::crUpdateSql
// {
// public:
// 	crInsertPlayerStore(){}
// 	void buildUpdateSql(int id,CRCore::crStreamBuf *stream);
// };
// class crRemovePlayerStore : public CRDataBase::crUpdateSql
// {
// public:
// 	crRemovePlayerStore(){}
// 	void buildUpdateSql(int id);
// };
// class crPlayerStoreQueryData : public CRDataBase::crQueryData
// {
// public:
// 	crPlayerStoreQueryData();
// 	crPlayerStoreQueryData(const crPlayerStoreQueryData& data);
// 	DataClass(JXJ, PlayerStoreQuery)
// 		virtual void addParam(int i, char *buf, int length);
// 	void buildQuerySql();
// 	void buildQuerySql(int playerid);
// 	unsigned char getID();
// 	int getPlayerid();
// 	CRCore::crStreamBuf *getStream();
// protected:
// 	unsigned char m_id;
// 	int m_playerid;
// 	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
// };
// class crPlayerStoreQuery2Data : public CRDataBase::crQueryData
// {
// public:
// 	crPlayerStoreQuery2Data();
// 	crPlayerStoreQuery2Data(const crPlayerStoreQuery2Data& data);
// 	DataClass(JXJ, PlayerStoreQuery2)
// 		virtual void addParam(int i, char *buf, int length);
// 	void buildQuerySql(int playerid);
// 	unsigned char getID();
// protected:
// 	unsigned char m_id;
// };
//class crQueryOnePlayerData : public CRDataBase::crQueryData
//{
//public:
//	crQueryOnePlayerData();
//	crQueryOnePlayerData(const crQueryOnePlayerData& data);
//	DataClass(JXJ, QueryOnePlayer)
//		virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int playerid);
//	int getID();
//	const std::string &getAccountName() const;
//	const std::string &getLoginDate() const;
//	const std::string &getCreateDate() const;
//	CRCore::crStreamBuf *getBody();
//protected:
//	int m_mainroleid;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
//class crQueryOnePlayerMainRoleData : public CRDataBase::crQueryData
//{
//public:
//	crQueryOnePlayerMainRoleData();
//	crQueryOnePlayerMainRoleData(const crQueryOnePlayerMainRoleData& data);
//	DataClass(JXJ, QueryOnePlayerMainRole)
//		virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int mainroleid);
//	const std::string &getCharacterName() const;
//	CRCore::crStreamBuf *getBody();
//protected:
//	std::string m_charactername;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//};
class crCheckRemainPlayerData : public CRDataBase::crQueryData
{
public:
	crCheckRemainPlayerData();
	crCheckRemainPlayerData(const crCheckRemainPlayerData& data);
	DataClass(JXJ, CheckRemainPlayer)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(std::string& selcolname,std::string& tabname,std::string& colname,int overdue);
	int getID();
protected:
	int m_id;
};
class crCheckLostPlayerData : public CRDataBase::crQueryData
{
public:
	crCheckLostPlayerData();
	crCheckLostPlayerData(const crCheckLostPlayerData& data);
	DataClass(JXJ, CheckLostPlayer)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int overdue);
	int getID();
protected:
	int m_id;
};
class crCheckTableCountData : public CRDataBase::crQueryData
{
public:
	crCheckTableCountData();
	crCheckTableCountData(const crCheckTableCountData& data);
	DataClass(JXJ, CheckTableCount)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(std::string& tabname);
	void buildQuerySql(std::string& tabname,std::string& colname,int overdue);
	int getTabCount();
protected:
	int m_count;
};
class crCheckMoneyRecordData : public CRDataBase::crQueryData
{
public:
	crCheckMoneyRecordData();
	crCheckMoneyRecordData(const crCheckMoneyRecordData& data);
	DataClass(JXJ, CheckMoneyRecord)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	int getPlayerID();
	int getMoneyCount();
	std::string &getStr();
	std::string &getCreatedate();
protected:
	int m_playerid;
	int m_moneycount;
	std::string m_str;
	std::string m_createdate;
};
//////////账户表相关
class crJXJLoginQueryData : public CRDataBase::crQueryData
{//根据userName查询Account表，单组
public:
	crJXJLoginQueryData();
	crJXJLoginQueryData(const crJXJLoginQueryData& data);
	DataClass(JXJ, JXJLoginQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string& name,const std::string& pwd);
	int getPlayerID();

	int getLastServerID();
	unsigned char getLocked();
protected:
	int m_id;
	int m_lastserverid;
	unsigned char m_locked;
};
class crJXJLoginKeyQueryData : public CRDataBase::crQueryData
{//根据playerid查询Account表，单组
public:
	crJXJLoginKeyQueryData();
	crJXJLoginKeyQueryData(const crJXJLoginKeyQueryData& data);
	DataClass(JXJ, JXJLoginKeyQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	const std::string& getKey() const;
	const std::string& getUserName() const;
	const std::string &getAdvertid() const;
protected:
	std::string m_key;
	std::string m_username;
	std::string m_advert_id;
};
class crJXJUpdateLoginKeyDate : public CRDataBase::crUpdateSql
{
public:
	crJXJUpdateLoginKeyDate(){}
	void buildUpdateSql(int playerid,const std::string &key,const std::string &ip);
};
class crJXJUpdateLoginServerID : public CRDataBase::crUpdateSql
{
public:
	crJXJUpdateLoginServerID(){}
	void buildUpdateSql(int playerid,int serverid);
};
class crJXJInsertAccount : public CRDataBase::crUpdateSql
{
public:
	crJXJInsertAccount(){}
	void buildUpdateSql(const std::string &name, const std::string &password, unsigned short chatdbid, const std::string &email, const std::string &nickname, const std::string &relname, char sex);
};

class crJXJRemoveAccount : public CRDataBase::crUpdateSql
{
public:
	crJXJRemoveAccount(){}
	void buildUpdateSql(const std::string &name);
};

class crJXJUpdatePassword : public CRDataBase::crUpdateSql
{
public:
	crJXJUpdatePassword(){}
	void buildUpdateSql(int playerid, const std::string &password);
};

class crJXJUpdateOnlineTime : public CRDataBase::crUpdateSql
{
public:
	crJXJUpdateOnlineTime(){}
	void buildUpdateSql(int playerid);
};

class crJXJLockAccount : public CRDataBase::crUpdateSql
{
public:
	crJXJLockAccount(){}
	void buildUpdateSql(int playerid,unsigned char locked);//0解锁，1玩家锁定，2官方锁定, 3未激活
};
class crJXJActivecodeQueryData : public CRDataBase::crQueryData
{
public:
	crJXJActivecodeQueryData();
	crJXJActivecodeQueryData(const crJXJActivecodeQueryData& data);
	DataClass(JXJ, JXJActivecodeQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &code);
	int getID();
	bool getUsed();
	const std::string &getValidDate();
protected:
	int m_id;
	bool m_used;
	std::string m_validdate;
};
class crJXJUpdateActiveCode : public CRDataBase::crUpdateSql
{
public:
	crJXJUpdateActiveCode(){}
	void buildUpdateSql(int id);
};
class crJXJInsertActiveCode : public CRDataBase::crUpdateSql
{
public:
	crJXJInsertActiveCode(){}
	void buildUpdateSql(const std::string &code);
	void buildUpdateSql(const std::string &code,const std::string &validdate);
};
//////////需要修改的玩家信息列表
class crJXJPlayerdataChangeQueryData : public CRDataBase::crQueryData
{//
public:
	crJXJPlayerdataChangeQueryData();
	crJXJPlayerdataChangeQueryData(const crJXJPlayerdataChangeQueryData& data);
	DataClass(JXJ, JXJPlayerdataChangeQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string& name,const std::string& pwd);
	int getPlayerID();

	int getLastServerID();
	unsigned char getLocked();
protected:
	int m_id;
	int m_lastserverid;
	unsigned char m_locked;
};
class  crJXJInsertPlayerdataChangeList : public CRDataBase::crUpdateSql
{//增加需要修改的玩家数据列
public:
	crJXJInsertPlayerdataChangeList(){}
	void buildUpdateSql(int playerid, int serverid, int gmid, int changetype, int itemid, int count, const std::string &info);
};
class  crJXJUpdatePlayerdataChangeList : public CRDataBase::crUpdateSql
{//提取需要修改的玩家数据列
public:
	crJXJUpdatePlayerdataChangeList(){}
	void buildUpdateSql(int id);
};
class  crJXJRemovePlayerdataChangeList : public CRDataBase::crUpdateSql
{//删除需要修改的玩家数据列
public:
	crJXJRemovePlayerdataChangeList(){}
	void buildUpdateSql(int id);
};
class  crJXJInsertPlayerdataChangeRecord : public CRDataBase::crUpdateSql
{//玩家数据变动记录
public:
	crJXJInsertPlayerdataChangeRecord(){}
	void buildUpdateSql(int playerid, int serverid, int gmid, int changetype, int itemid, int count, const std::string &info);
};
class  crPlayerdataChangeListQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerdataChangeListQueryData();
	crPlayerdataChangeListQueryData(const crPlayerdataChangeListQueryData& data);
	DataClass(CRNetApp, PlayerdataChangeListQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,int count);
	int getID();
	int getPlayerID();
	int getType();
	int getItemID();
	int getCount();
	int getGmid();
	const std::string& getInformation();
protected:
	int m_id;
	int m_playerid;
	int m_type;
	int m_itemid;
	int m_count;
	int m_gmid;
	std::string m_information;
};
class  crJXJLockPlayerdata : public CRDataBase::crUpdateSql
{//锁定玩家帐号
public:
	crJXJLockPlayerdata(){}
	void buildUpdateSql(int id);
};
class  crJXJUnLockPlayerdata : public CRDataBase::crUpdateSql
{//解锁玩家帐号
public:
	crJXJUnLockPlayerdata(){}
	void buildUpdateSql(int id);
};

class crJXJActiveCodeQueryData : public CRDataBase::crQueryData
{//
public:
	crJXJActiveCodeQueryData ();
	crJXJActiveCodeQueryData (const crJXJActiveCodeQueryData & data);
	DataClass(JXJ, JXJActiveCodeQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string & activeCode);
	char getUsed();
	short getCodeType();
	int getServerID();
	int getGetterID();
	int getUserID();
protected:
	char m_used;
	short m_codeType;
	int m_serverID;
	int m_getterID;
	int m_userID;
};

class crJXJCodeTypeQueryData : public CRDataBase::crQueryData
{//
public:
	crJXJCodeTypeQueryData ();
	crJXJCodeTypeQueryData (const crJXJCodeTypeQueryData & data);
	DataClass(JXJ, JXJCodeTypeQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(short codeType);
	const std::string & getValidDate();
	const std::string & getOverDate();
protected:
	std::string m_validDate;
	std::string m_overDate;
};

class  crJXJActiveCodeUpdate: public CRDataBase::crUpdateSql
{//
public:
	crJXJActiveCodeUpdate(){}
	void buildUpdateSql(std::string & activeCode,int playerID);
};

class  crJXJUpdatePlayerPassword : public CRDataBase::crUpdateSql
{//修改玩家密码
public:
	crJXJUpdatePlayerPassword(){}
	void buildUpdateSql(int id, std::string &newpassword);
};

class crJXJCheckPasswordQueryData : public CRDataBase::crQueryData
{//根据playerid,password 查询Account表，单组
public:
	crJXJCheckPasswordQueryData ();
	crJXJCheckPasswordQueryData (const crJXJCheckPasswordQueryData & data);
	DataClass(JXJ, JXJCheckPasswordQuery)
	void buildQuerySql(int playerid,std::string& password);
};
class crCheckPayMoneyCountData : public CRDataBase::crQueryData
{
public:
	crCheckPayMoneyCountData();
	crCheckPayMoneyCountData(const crCheckPayMoneyCountData& data);
	DataClass(JXJ, CheckPayMoneyCount)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int serverid);
	int getCount();
protected:
	int m_count;
};
class crInsertPlayerGiftGold : public CRDataBase::crUpdateSql
{//玩家礼金获得和消费
public:
	crInsertPlayerGiftGold(){}
	void buildUpdateSql(int playerid,int serverid,int money,const std::string &str,const std::string &advert_id);
};
//////////////////////////////////

class crGetTodayLoginPlayerData : public CRDataBase::crQueryData
{
	// 得到今日登录并且注册的玩家
public:
	crGetTodayLoginPlayerData ();
	crGetTodayLoginPlayerData (const crGetTodayLoginPlayerData & data);
	DataClass(JXJ, GetTodayLoginPlayer)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,int overdue,int permission);
	int getCount();
protected:
	int m_count;
};

class crGetTodayLoginGMData : public CRDataBase::crQueryData
{
	// 得到今日登录并且注册的GM
public:
	crGetTodayLoginGMData ();
	crGetTodayLoginGMData (const crGetTodayLoginGMData & data);
	DataClass(JXJ, GetTodayLoginGM)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,int overdue,int permission);
	int getCount();
protected:
	int m_count;
};

class crSelectCountCharacternameExceptGMData : public CRDataBase::crQueryData
{
	// 得到 charactername 记录数目 排除gm
public:
	crSelectCountCharacternameExceptGMData ();
	crSelectCountCharacternameExceptGMData (const crSelectCountCharacternameExceptGMData & data);
	DataClass(JXJ, SelectCountCharacternameExceptGM)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,int permission);
	int getCount();
protected:
	int m_count;
};

class crSelectCountTableData : public CRDataBase::crQueryData
{
	// 得到 得到某个表的数目
public:
	crSelectCountTableData ();
	crSelectCountTableData (const crSelectCountTableData & data);
	DataClass(JXJ, SelectCountTable)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(std::string &tabName);
	int getCount();
protected:
	int m_count;
};

class crJXJInsertMoneyrecord : public CRDataBase::crUpdateSql
{//玩家元宝变动记录
public:
	crJXJInsertMoneyrecord(){}
	void buildUpdateSql(int playerid, int serverid, int money, int oldmoney, const std::string &str, const std::string &consortia, const std::string &name,const std::string &advert_id,const std::string &ordernumber);
};

class  crDataQueryTaskData : public CRDataBase::crQueryData
{
public:
	crDataQueryTaskData ();
	crDataQueryTaskData (const crDataQueryTaskData & data);
	DataClass(JXJ, DataQueryTask)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(std::string tabname,int limitCount,int serverid);
	int getID();
	int getPlayerID();
	int getQueryID();
	int getQueryType();
	int getGMID();
protected:
	int m_id;
	int m_playerid;
	int m_queryID;
	int m_queryType;
	int m_gmID;
};

class  crJXJRemoveDataQueryTaskRecord:public CRDataBase::crUpdateSql
{//
public:
	crJXJRemoveDataQueryTaskRecord(){}
	void buildUpdateSql(int id);
};

class crJXJInsertDataQueryRecord: public CRDataBase::crUpdateSql
{//
public:
	crJXJInsertDataQueryRecord(){}
	void buildUpdateSql(int playerid,int serverid,int queryid,int gmid,int querytype,unsigned char result,const std::string & retStr = "");
};

class crJXJRechargerecordQueryData : public CRDataBase::crQueryData
{
public:
	crJXJRechargerecordQueryData();
	crJXJRechargerecordQueryData(const crJXJRechargerecordQueryData& data);
	DataClass(JXJ, JXJRechargerecordQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid, int count);
	int getID();
	int getPlayerID();
	int getMoney();
	const std::string &getOrderNumber();
	const std::string &getType_id();
protected:
	int m_id;
	int m_playerid;
	int m_money;
	std::string m_ordernumber;
};
class crJXJInsertRechargerecord : public CRDataBase::crUpdateSql
{//玩家充值
public:
	crJXJInsertRechargerecord(){}
	void buildUpdateSql(int playerid,int serverid,int money,const std::string &str);
};
class crJXJUpdateRechargerecord : public CRDataBase::crUpdateSql
{//提取充值变成元宝
public:
	crJXJUpdateRechargerecord(){}
	void buildUpdateSql(int id);
};
class crJXJRemoveRechargerecord : public CRDataBase::crUpdateSql
{//提取充值变成元宝
public:
	crJXJRemoveRechargerecord(){}
	void buildUpdateSql(int id);
};
class crBattleReportsInsert : public CRDataBase::crUpdateSql
{
public:
	crBattleReportsInsert();
	void buildUpdateSql(int battleid,CRCore::ref_ptr<CRCore::crStreamBuf> stream);
};
class crBattleReportsQueryData : public CRDataBase::crQueryData
{
public:
	crBattleReportsQueryData();
	crBattleReportsQueryData(const crBattleReportsQueryData& data);
	DataClass(JXJ, BattleReportsQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	CRCore::crStreamBuf *getStream();
	int getBattleID();
protected:
	int m_battleid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};
////////////////////
//crJXJGmQueryData
class crJXJGmQueryData : public CRDataBase::crQueryData
{
public:
	crJXJGmQueryData();
	crJXJGmQueryData(const crJXJGmQueryData& data);
	DataClass(JXJ, JXJGmQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid);
	int getPlayerID();
	unsigned char getAuthorityid();
protected:
	int m_playerid;
	unsigned char m_authorityid;
};
////////////////////
//crJXJNoChatQueryData
class crJXJNoChatQueryData : public CRDataBase::crQueryData
{
public:
	crJXJNoChatQueryData();
	crJXJNoChatQueryData(const crJXJNoChatQueryData& data);
	DataClass(JXJ, JXJNoChatQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid);
	int getPlayerID();
protected:
	int m_playerid;
};

class crJXJInsertPeopleMapRecord: public CRDataBase::crUpdateSql
{//
public:
	crJXJInsertPeopleMapRecord(){}
	void buildUpdateSql(int playerid,unsigned char shili,const std::string & name,
		unsigned char lv,unsigned char troopsLv,int consortiaID,
		unsigned char ismallacc,int killNum,int newerStep,int deadTroopsNum,int giftGold,
		int totalRecharge,int troopsnum, int exploit);
};

class  crJXJUpdatePeopleMapRecord:public CRDataBase::crUpdateSql
{//
public:
	crJXJUpdatePeopleMapRecord(){}
	void buildUpdateSql(int playerid,unsigned char shili,const std::string & name,
		unsigned char lv,unsigned char troopsLv,int consortiaID,
		unsigned char ismallacc,int killNum,int newerStep,int deadTroopsNum,int giftGold,
		int totalRecharge,int troopsnum, int exploit);
};

class crJXJPeopleMapQueryData : public CRDataBase::crQueryData
{
public:
	crJXJPeopleMapQueryData ();
	crJXJPeopleMapQueryData (const crJXJPeopleMapQueryData & data);
	DataClass(JXJ, JXJPeopleMapQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int start,int count);
	int getPlayerID();
	int getShili();
	const std::string & getName();
	unsigned char getLv();
	unsigned char getTroopsLv();
	int getConsortiaID();
	unsigned char getSmallAcc();
	int getKillNum();
	int getNewerStep();
	int getDeadTroopsNum();
	int getGiftGold();
	int getRecharge();
	int gettroopsnum();
	int getexploit();

protected:

	int m_playerid;
	unsigned char m_shili;
	std::string  m_name;
	unsigned char m_lv;
	unsigned char m_troopsLv;
	int m_consortiaID;
	unsigned char m_ismallacc;
	int m_killNum;
	int m_newerStep;
	int m_deadTroopsNum;
	int m_giftGold;
	int m_totalRecharge;
	int m_troopsnum;
	int m_exploit;
};

class crJXJSelectGMIDData : public CRDataBase::crQueryData
{
public:
	crJXJSelectGMIDData ();
	crJXJSelectGMIDData (const crJXJSelectGMIDData & data);
	DataClass(JXJ, JXJSelectGMID)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,int permission);
	int getPlayerID();
protected:
	int m_playerid;
};
//////////
//GM邮件//
/////////
class  crJXJRemoveGMMail :public CRDataBase::crUpdateSql
{//
public:
	crJXJRemoveGMMail(){}
	void buildUpdateSql(int id);
};

class crJXJInsertGMMailRecord : public CRDataBase::crUpdateSql
{//
public:
	crJXJInsertGMMailRecord(){}
	void buildUpdateSql(int gmid,int serverid,unsigned char mailtype, int playerid, const std::string& playername, const std::string& title, const std::string& content, const std::string &item1,const std::string& item2,const std::string& item3,const std::string& item4);
};

class crJXJGMMailQueryData : public CRDataBase::crQueryData
{
public:
	crJXJGMMailQueryData();
	crJXJGMMailQueryData(const crJXJGMMailQueryData& data);
	DataClass(JXJ, JXJGMMailQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int count,int serverid);
	int getID();
	int getGMID();
	unsigned char getType();
	const std::string & getPlayerName();
	const std::string & getTitle();
	const std::string & getContent();
	//std::vector<int> getItemIDVec();
	//std::vector<int> getItemCountVec();
	const std::string & getItem1();
	const std::string & getItem2();
	const std::string & getItem3();
	const std::string & getItem4();
protected:
	int m_id;
	int m_gmid;
	unsigned char m_type;
	std::string m_playername;
	std::string m_title;
	std::string m_content;
	//std::vector<int> m_itemidvec;
	//std::vector<int> m_itemcountvec;
	std::string m_item1;
	std::string m_item2;
	std::string m_item3;
	std::string m_item4;
};
//////////
//GM公告//
/////////
//class  crJXJRemoveGMNotice :public CRDataBase::crUpdateSql
//{//
//public:
//	crJXJRemoveGMNotice(){}
//	void buildUpdateSql(int id);
//};
//
//class crJXJInsertGMNoticeRecord : public CRDataBase::crUpdateSql
//{//
//public:
//	crJXJInsertGMNoticeRecord(){}
//	void buildUpdateSql(int gmid, std::string& content);
//};

class crJXJGMNoticeQueryData : public CRDataBase::crQueryData
{
public:
	crJXJGMNoticeQueryData();
	crJXJGMNoticeQueryData(const crJXJGMNoticeQueryData& data);
	DataClass(JXJ, JXJGMNoticeQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid);
	int getID();
	const std::string & getContent();
protected:
	int m_id;
	std::string m_content;
};

class crJXJSelectAllAccountByCreateDateData : public CRDataBase::crQueryData
{
	// 得到某天注册的玩家数
public:
	crJXJSelectAllAccountByCreateDateData ();
	crJXJSelectAllAccountByCreateDateData (const crJXJSelectAllAccountByCreateDateData & data);
	DataClass(JXJ, JXJSelectAllAccountByCreateDate)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int diffDay);
	int getCount();
protected:
	int m_count;
};

class crJXJSelectGMAccountByCreateDateData : public CRDataBase::crQueryData
{
	// 得到某天注册的GM玩家数
public:
	crJXJSelectGMAccountByCreateDateData ();
	crJXJSelectGMAccountByCreateDateData (const crJXJSelectGMAccountByCreateDateData & data);
	DataClass(JXJ, JXJSelectGMAccountByCreateDate)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverID,int diffDay,int permission);
	int getCount();
protected:
	int m_count;
};

class crJXJSelectAllCharacterByCreateDateData : public CRDataBase::crQueryData
{
	// 得到某天创角的玩家数
public:
	crJXJSelectAllCharacterByCreateDateData ();
	crJXJSelectAllCharacterByCreateDateData (const crJXJSelectAllCharacterByCreateDateData & data);
	DataClass(JXJ, JXJSelectAllCharacterByCreateDate)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverID,int diffDay);
	int getCount();
protected:
	int m_count;
};

class crJXJSelectGMCharacterByCreateDateData : public CRDataBase::crQueryData
{
	// 得到某天注册的GM玩家数
public:
	crJXJSelectGMCharacterByCreateDateData ();
	crJXJSelectGMCharacterByCreateDateData (const crJXJSelectGMCharacterByCreateDateData & data);
	DataClass(JXJ, JXJSelectGMCharacterByCreateDate)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverID,int permission,int diffDay);
	int getCount();
protected:
	int m_count;
};
/////////////////////////
class crInsertDatamonitor : public CRDataBase::crUpdateSql
{//
public:
	crInsertDatamonitor(){}
	void buildUpdateSql(int playerid, int serverid, std::string& str);
};

//黄钻信息查询;
class crJXJYellowInfoQueryData : public CRDataBase::crQueryData
{
public:
	crJXJYellowInfoQueryData();
	crJXJYellowInfoQueryData(const crJXJYellowInfoQueryData& data);
	DataClass(JXJ, JXJYellowInfoQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	unsigned char getIsYellow(){return m_isyellow;}
	unsigned char getIsYearYellow(){return m_isyearyellow;}
	unsigned char getYellowLevel(){return m_yellowlevel;}
protected:
	unsigned char m_isyellow;
	unsigned char m_isyearyellow;
	unsigned char m_yellowlevel;
};

class  crTXMarketTaskQueryData : public CRDataBase::crQueryData
{
public:
	crTXMarketTaskQueryData ();
	crTXMarketTaskQueryData (const crTXMarketTaskQueryData & data);
	DataClass(JXJ, TXMarketTaskQuery)
		virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid, int count);
	int getID();
	int getPlayerID();
	int getStep();
	const std::string& getTaskID();
protected:
	int m_id;
	int m_playerid;
	int m_step;
	std::string m_taskid;
};

class crJXJRemoveTXMarketTask : public CRDataBase::crUpdateSql
{//
public:
	crJXJRemoveTXMarketTask(){}
	void buildUpdateSql(int id);
};

class  crTXMarketTaskRecordQueryData : public CRDataBase::crQueryData
{
public:
	crTXMarketTaskRecordQueryData ();
	crTXMarketTaskRecordQueryData (const crTXMarketTaskRecordQueryData & data);
	DataClass(JXJ, TXMarketTaskRecordQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid, int serverid,const std::string& taskid);
	int getStep();
protected:
	int m_step;
};

class crJXJInsertTXTaskrecord : public CRDataBase::crUpdateSql
{
public:
	crJXJInsertTXTaskrecord(){}
	void buildUpdateSql(int playerid, int step,const std::string& taskid, int serverid, int state);
	void buildUpdateSql_task(const std::string& taskid,int playerid,int serverid,int step);
};

class crJXJInsertTaskrecord : public CRDataBase::crUpdateSql
{//玩家任务记录
public:
	crJXJInsertTaskrecord(){}
	void buildUpdateSql(int playerid, int serverid, const std::string &name, int taskid, char status, int progress);
};
class crJXJInsertOpinions : public CRDataBase::crUpdateSql
{//玩家意见征集记录
public:
	crJXJInsertOpinions(){}
	void buildUpdateSql(int playerid, int serverid, const std::string &name, const std::string &opinion);
};
class crJXJUpdateCharacterLevel : public CRDataBase::crUpdateSql
{//
public:
	crJXJUpdateCharacterLevel(){}
	void buildUpdateSql(int playerid, int serverid, int roleid, int lv);
};
class crJXJChildQueryData : public CRDataBase::crQueryData
{
public:
	crJXJChildQueryData();
	crJXJChildQueryData(const crJXJChildQueryData& data);
	DataClass(JXJ, JXJChildQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	char getChild(){return m_child;}
protected:
	char m_child;
};
class crJXJTelQueryData : public CRDataBase::crQueryData
{
public:
	crJXJTelQueryData();
	crJXJTelQueryData(const crJXJTelQueryData& data);
	DataClass(JXJ, JXJTelQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	const std::string& getTel(){return m_tel;}
protected:
	std::string m_tel;
};
class crJXJUpdateTel : public CRDataBase::crUpdateSql
{//
public:
	crJXJUpdateTel(){}
	void buildUpdateSql(int playerid, const std::string &tel);
};
}
#endif