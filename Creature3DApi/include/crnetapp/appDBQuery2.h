/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#ifndef CRNETAPP_DBQUERY2_H
#define CRNETAPP_DBQUERY2_H 1

#include <CRNetApp/appExport.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crStreamBuf.h>
#include <string>

namespace CRNetApp{

///////GlobalDB
class CRNETAPP_EXPORT crGameListLimitQueryData : public CRDataBase::crQueryData
{//查询game表，多组
public:
	crGameListLimitQueryData();
	crGameListLimitQueryData(const crGameListLimitQueryData& data);
	DataClass(CRNetApp, GameListLimitQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int rows,int offset);
	int getID();
	const std::string& getGameName() const;
	const std::string& getCName() const;
	unsigned char getRoleCount();
protected:
	int m_id;
	std::string m_gamename;
	std::string m_cname;
	unsigned char m_rolecount;
};

class CRNETAPP_EXPORT crLoginKeyQueryData : public CRDataBase::crQueryData
{//根据playerid查询account表，单组
public:
	crLoginKeyQueryData();
	crLoginKeyQueryData(const crLoginKeyQueryData& data);
	DataClass(CRNetApp, LoginKeyQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	const std::string& getKey() const;
	const std::string &getUserName() const;
protected:
	std::string m_key;
	std::string m_username;
};

class CRNETAPP_EXPORT crPlayerGameDBQueryData : public CRDataBase::crQueryData
{//查询playergamedb表，单组
public:
	crPlayerGameDBQueryData();
	crPlayerGameDBQueryData(const crPlayerGameDBQueryData& data);
	DataClass(CRNetApp, PlayerGameDBQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int serverid);
	unsigned short getGameDBID();
protected:
	unsigned short m_gamedbid;
};

class CRNETAPP_EXPORT crPlayerGameDataQueryData : public CRDataBase::crQueryData
{//查询playergamedata表，单组
public:
	crPlayerGameDataQueryData();
	crPlayerGameDataQueryData(const crPlayerGameDataQueryData& data);
	DataClass(CRNetApp, PlayerGameDataQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	int getID();
	int getMainRoleID();
	CRCore::crStreamBuf *getStream();
protected:
	int m_id;
	int m_mainroleid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class CRNETAPP_EXPORT crCharacterNameValidQueryData : public CRDataBase::crQueryData
{//单组
public:
	crCharacterNameValidQueryData();
	crCharacterNameValidQueryData(const crCharacterNameValidQueryData& data);
	DataClass(CRNetApp, CharacterNameValidQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &name, int serverid);
	int getPlayeID();
protected:
	int m_playerid;
};

class CRNETAPP_EXPORT crCharacterNameIDQueryData : public CRDataBase::crQueryData
{//单组
public:
	crCharacterNameIDQueryData();
	crCharacterNameIDQueryData(const crCharacterNameIDQueryData& data);
	DataClass(CRNetApp, CharacterNameIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid, int serverid, int roleid);
	int getID();
protected:
	int m_id;
};

class CRNETAPP_EXPORT crCharacterNameQueryData : public CRDataBase::crQueryData
{//单组
public:
	crCharacterNameQueryData();
	crCharacterNameQueryData(const crCharacterNameQueryData& data);
	DataClass(CRNetApp, CharacterNameQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int serverid,int roleid);
	const std::string& getName() const;
protected:
	std::string m_name;
};

class CRNETAPP_EXPORT crCharacterNamePlayerIDQueryData : public CRDataBase::crQueryData
{//单组
public:
	crCharacterNamePlayerIDQueryData();
	crCharacterNamePlayerIDQueryData(const crCharacterNamePlayerIDQueryData& data);
	DataClass(CRNetApp, CharacterNamePlayerIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &name, int serverid);
	int getPlayerID();
protected:
	int m_playerid;
};

class CRNETAPP_EXPORT crGameDBCountQueryData : public CRDataBase::crQueryData
{//查询game表，单组
public:
	crGameDBCountQueryData();
	crGameDBCountQueryData(const crGameDBCountQueryData& data);
	DataClass(CRNetApp, GameDBCountQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid);
	unsigned short getGameDBCount();
protected:
	unsigned short m_gamedbcount;
};

class CRNETAPP_EXPORT crGameRoleCountQueryData : public CRDataBase::crQueryData
{//查询game表，单组
public:
	crGameRoleCountQueryData();
	crGameRoleCountQueryData(const crGameRoleCountQueryData& data);
	DataClass(CRNetApp, GameRoleCountQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid);
	unsigned char getRoleCount();
protected:
	unsigned char m_rolecount;
};

class CRNETAPP_EXPORT crGameRoleQueryData : public CRDataBase::crQueryData
{//查询role表，多组
public:
	crGameRoleQueryData();
	crGameRoleQueryData(const crGameRoleQueryData& data);
	DataClass(CRNetApp, GameRoleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	int getID();
	int getInstanceItemID();
	int getMetierID();
protected:
	int m_id;
	int m_instanceitemid;
	int m_metierid;
};

class CRNETAPP_EXPORT crRoleCountQueryData : public CRDataBase::crQueryData
{//查询role表，多组
public:
	crRoleCountQueryData();
	crRoleCountQueryData(const crRoleCountQueryData& data);
	DataClass(CRNetApp, RoleCountQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid);
	int getCount();
protected:
	int m_count;
};

class CRNETAPP_EXPORT crRoleStreamQueryData : public CRDataBase::crQueryData
{
public:
	crRoleStreamQueryData();
	crRoleStreamQueryData(const crRoleStreamQueryData& data);
	DataClass(CRNetApp, RoleStreamQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int roleid);
	CRCore::crStreamBuf *getStream();
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class CRNETAPP_EXPORT crRoleMetierStreamQueryData : public CRDataBase::crQueryData
{
public:
	crRoleMetierStreamQueryData();
	crRoleMetierStreamQueryData(const crRoleMetierStreamQueryData& data);
	DataClass(CRNetApp, RoleMetierStreamQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int roleid);
	CRCore::crStreamBuf *getMetierStream();
protected:
	CRCore::ref_ptr<CRCore::crStreamBuf> m_metierStream;
};

//class CRNETAPP_EXPORT crSceneInfoQueryData : public CRDataBase::crQueryData
//{
//public:
//	crSceneInfoQueryData();
//	crSceneInfoQueryData(const crSceneInfoQueryData& data);
//	DataClass(CRNetApp, SceneInfoQuery)
//	virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int sceneid);
//	const std::string& getSceneName() const;
//	const std::string& getFileName() const;
//	//const std::string& getUpdatecfg() const;
//	const std::string& getCollideFile() const;
//protected:
//	std::string m_scenename;
//	std::string m_filename;
//	//std::string m_updatecfg;
//	std::string m_collidefile;
//};

class CRNETAPP_EXPORT crGameIDQueryData : public CRDataBase::crQueryData
{//查询game表，单组
public:
	crGameIDQueryData();
	crGameIDQueryData(const crGameIDQueryData& data);
	DataClass(CRNetApp, GameIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(const std::string &gamename);
	int getID();
protected:
	int m_id;
};

class CRNETAPP_EXPORT crScene2QueryData : public CRDataBase::crQueryData
{//查询scene表获得gameid，单组
public:
	crScene2QueryData();
	crScene2QueryData(const crScene2QueryData& data);
	DataClass(CRNetApp, Scene2Query)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getGameID();
protected:
	int m_gameid;
};

class CRNETAPP_EXPORT crSceneIDQueryData : public CRDataBase::crQueryData
{
public:
	crSceneIDQueryData();
	crSceneIDQueryData(const crSceneIDQueryData& data);
	DataClass(CRNetApp, SceneIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid,const std::string &scenename);
	int getID();
protected:
	int m_id;
};

class CRNETAPP_EXPORT crSceneItemQueryData : public CRDataBase::crQueryData
{
public:
	crSceneItemQueryData();
	crSceneItemQueryData(const crSceneItemQueryData& data);
	DataClass(CRNetApp, SceneItemQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int sceneid);
	int getID();
	int getAbstractItemID();
	unsigned char getItemtype();
	const std::string& getIName() const;
    unsigned short getLayerid();
	int getPosx();
	int getPosy();
	short getZoffset();
	float getDirx();
	float getDiry();
	float getDirz();
    CRCore::crStreamBuf *getStream();
protected:
	int m_id;
	int m_abstractitemid;
	unsigned char m_itemtype;
	std::string m_iname;
	unsigned short m_layerid;
	int m_posx;
	int m_posy;
	short m_zoffset;
	float m_dirx;
	float m_diry;
	float m_dirz;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class CRNETAPP_EXPORT crItemChildQueryData : public CRDataBase::crQueryData
{
public:
	crItemChildQueryData();
	crItemChildQueryData(const crItemChildQueryData& data);
	DataClass(CRNetApp, ItemChildQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int parentitemid);
	int getID();
	int getInstanceitemid();
protected:
	int m_id;
	int m_instanceitemid;
};

class CRNETAPP_EXPORT crItemChild2QueryData : public CRDataBase::crQueryData
{
public:
	crItemChild2QueryData();
	crItemChild2QueryData(const crItemChild2QueryData& data);
	DataClass(CRNetApp, ItemChild2Query)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int parentitemid);
	int getInstanceitemid();
protected:
	int m_instanceitemid;
};

class CRNETAPP_EXPORT crInstanceItemQueryData : public CRDataBase::crQueryData
{
public:
	crInstanceItemQueryData();
	crInstanceItemQueryData(const crInstanceItemQueryData& data);
	DataClass(CRNetApp, InstanceItemQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getAbstractItemID();
	unsigned char getItemtype();
	const std::string& getIName() const;
	int getSceneID();
	unsigned short getLayerid();
	int getPosx();
	int getPosy();
	int getZoffset();
	float getDirx();
	float getDiry();
	float getDirz();
	CRCore::crStreamBuf *getStream();
protected:
	int m_abstractitemid;
	unsigned char m_itemtype;
	std::string m_iname;
	int m_sceneid;
	unsigned short m_layerid;
	int m_posx;
	int m_posy;
	int m_zoffset;
	float m_dirx;
	float m_diry;
	float m_dirz;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class CRNETAPP_EXPORT crInstanceItem2QueryData : public CRDataBase::crQueryData
{
public:
	crInstanceItem2QueryData();
	crInstanceItem2QueryData(const crInstanceItem2QueryData& data);
	DataClass(CRNetApp, InstanceItem2Query)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getAbstractItemID();
	const std::string& getIName() const;
	int getSceneID();
	unsigned short getLayerid();
	CRCore::crStreamBuf *getStream();
protected:
	int m_abstractitemid;
	std::string m_iname;
	int m_sceneid;
	unsigned short m_layerid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
};

class CRNETAPP_EXPORT crRoleQueryData : public CRDataBase::crQueryData
{
public:
	crRoleQueryData();
	crRoleQueryData(const crRoleQueryData& data);
	DataClass(CRNetApp, RoleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int roleid);
	int getInstanceItemID();
	int getAbstractMetierID();
	CRCore::crStreamBuf *getMetierStream();
protected:
	int m_instanceitemid;
	int m_abstractmetierid;
	CRCore::ref_ptr<CRCore::crStreamBuf> m_metierstream;
};

class crRoleQuery2Data : public CRDataBase::crQueryData
{
public:
	crRoleQuery2Data();
	crRoleQuery2Data(const crRoleQuery2Data& data);
	DataClass(CRNetApp, RoleQuery2)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int roleid);
	int getInstanceItemID();
protected:
	int m_instanceitemid;
};

class CRNETAPP_EXPORT crAbstractItemQueryData : public CRDataBase::crQueryData
{
public:
	crAbstractItemQueryData();
	crAbstractItemQueryData(const crAbstractItemQueryData& data);
	DataClass(CRNetApp, AbstractItemQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getResID();
	const std::string& getName() const;
	const std::string& getScript() const;
protected:
	int m_resid;
	std::string m_name;
	std::string m_script;
};

//class CRNETAPP_EXPORT crAbstractItem2QueryData : public CRDataBase::crQueryData
//{
//public:
//	crAbstractItem2QueryData();
//	crAbstractItem2QueryData(const crAbstractItem2QueryData& data);
//	DataClass(CRNetApp, AbstractItem2Query)
//	virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int id);
//	//int getResID();
//	const std::string& getScript() const;
//protected:
//	//int m_resid;
//	std::string m_script;
//};

class CRNETAPP_EXPORT crResQueryData : public CRDataBase::crQueryData
{
public:
	crResQueryData();
	crResQueryData(const crResQueryData& data);
	DataClass(CRNetApp, ResQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	const std::string& getFileName() const;
	const std::string& getScript() const;
protected:
	std::string m_filename;
	std::string m_script;
};

class CRNETAPP_EXPORT crRes2QueryData : public CRDataBase::crQueryData
{
public:
	crRes2QueryData();
	crRes2QueryData(const crRes2QueryData& data);
	DataClass(CRNetApp, Res2Query)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	const std::string& getScript() const;
protected:
	std::string m_script;
};

class CRNETAPP_EXPORT crAbstractMetierQueryData : public CRDataBase::crQueryData
{
public:
	crAbstractMetierQueryData();
	crAbstractMetierQueryData(const crAbstractMetierQueryData& data);
	DataClass(CRNetApp, AbstractMetierQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getGameID();
	const std::string& getMetierName() const;
	int getInitSceneID();
	unsigned short getInitLayerID();
	short getZoffset();
	const std::string& getScript() const;
	const std::string& getTaskTable() const;
protected:
	int m_gameid;
	std::string m_metiername;
	int m_initsceneid;
	unsigned short m_initlayerid;
	short m_zoffset;
	std::string m_script;
	std::string m_tasktable;
};

//class CRNETAPP_EXPORT crAbstractMetier2QueryData : public CRDataBase::crQueryData
//{
//public:
//	crAbstractMetier2QueryData();
//	crAbstractMetier2QueryData(const crAbstractMetier2QueryData& data);
//	DataClass(CRNetApp, AbstractMetier2Query)
//	virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int id/*, int gameid*/);
//	int getInitSceneID();
//	unsigned short getInitLayerID();
//	//int getInitx();
//	//int getInity();
//	//unsigned short getInitR();
//	short getZoffset();
//	//const std::string& getScript() const;
//	//const std::string& getTaskTable() const;
//protected:
//	int m_initsceneid;
//	unsigned short m_initlayerid;
//	//int m_initx;
//	//int m_inity;
//	//unsigned short m_initr;
//	short m_zoffset;
//	//std::string m_script;
//	//std::string m_tasktable;
//};

class CRNETAPP_EXPORT crSceneListQueryData : public CRDataBase::crQueryData
{//查询scene表
public:
	crSceneListQueryData();
	crSceneListQueryData(const crSceneListQueryData& data);
	DataClass(CRNetApp, SceneListQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid,int rows, int offset);
	int getSceneID();
	const std::string& getSceneName() const;
	const std::string& getFileName() const;
protected:
	int m_sceneid;
	std::string m_scenename;
};
class CRNETAPP_EXPORT crRemoveRoleQueryData : public CRDataBase::crQueryData
{
public:
	crRemoveRoleQueryData();
	crRemoveRoleQueryData(const crRemoveRoleQueryData& data);
	DataClass(CRNetApp, RemoveRoleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int roleid);
	int getPlayerID();
	int getInstanceItemID();
protected:
	int m_playerid;
	int m_instanceitemid;
};
class CRNETAPP_EXPORT crPlayerSaleQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerSaleQueryData();
	crPlayerSaleQueryData(const crPlayerSaleQueryData& data);
	DataClass(CRNetApp, PlayerSaleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int goodstype,int goodsid,unsigned char salestate,int start,int count);
	int getID();
	const std::string &getPlayerName() const;
	int getPrice();
	int getSaleCount();
protected:
	int m_id;
	std::string m_playername;
	int m_price;
	int m_salecount;
};
class CRNETAPP_EXPORT crCountPlayerSaleQueryData : public CRDataBase::crQueryData
{
public:
	crCountPlayerSaleQueryData();
	crCountPlayerSaleQueryData(const crCountPlayerSaleQueryData& data);
	DataClass(CRNetApp, CountPlayerSaleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int goodstype,int goodsid);
	int getCount();
protected:
	int m_count;
};
class CRNETAPP_EXPORT crBuySaleQueryData : public CRDataBase::crQueryData
{
public:
	crBuySaleQueryData();
	crBuySaleQueryData(const crBuySaleQueryData& data);
	DataClass(CRNetApp, BuySaleQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	int getGoodsType();
	int getGoodsID();
	int getPrice();
	int getSaleCount();
protected:
	int m_goodstype;
	int m_goodsid;
	int m_price;
	int m_salecount;
};
class CRNETAPP_EXPORT crMySaleStateQueryData : public CRDataBase::crQueryData
{
public:
	crMySaleStateQueryData();
	crMySaleStateQueryData(const crMySaleStateQueryData& data);
	DataClass(CRNetApp, MySaleStateQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int id);
	unsigned char getSaleState();
	int getGaincash();
protected:
	unsigned char m_salestate;
	int m_gaincash;
};
class CRNETAPP_EXPORT crServerListIDQueryData : public CRDataBase::crQueryData
{
public:
	crServerListIDQueryData();
	crServerListIDQueryData(const crServerListIDQueryData& data);
	DataClass(CRNetApp, ServerListIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid,const std::string &serverid);
	int getID();
	int getWebServerID();
	const std::string &getSite() const;
protected:
	int m_id;
	int m_webserverid;
	std::string m_site;
};
class CRNETAPP_EXPORT crServerListQueryData : public CRDataBase::crQueryData
{
public:
	crServerListQueryData();
	crServerListQueryData(const crServerListQueryData& data);
	DataClass(CRNetApp, ServerListQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid);
	void buildQuerySql_gm(int gameid);
	int getID();
	const std::string &getServerName() const;
	unsigned char getISP();
	short getRemainCount();
protected:
	int m_id;
	std::string m_servername;
	unsigned char m_isp;
	short m_remaincount;
};
class CRNETAPP_EXPORT crConnectServerIDQueryData : public CRDataBase::crQueryData
{
public:
	crConnectServerIDQueryData();
	crConnectServerIDQueryData(const crConnectServerIDQueryData& data);
	DataClass(CRNetApp, ConnectServerIDQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid,unsigned short port);
	int getID();
protected:
	int m_id;
};
class CRNETAPP_EXPORT crConnectServersQueryData : public CRDataBase::crQueryData
{
public:
	crConnectServersQueryData();
	crConnectServersQueryData(const crConnectServersQueryData& data);
	DataClass(CRNetApp, ConnectServersQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int serverid);
	const std::string &getIP() const;
	const std::string &getIP2() const;
	const std::string &getIP3() const;
	unsigned short getPort();
	short getRemainCount();
protected:
	std::string m_ip;//电信
	std::string m_ip2;//联通
	std::string m_ip3;//移动
	unsigned short m_port;
	short m_remaincount;
};
class CRNETAPP_EXPORT crActivecodeQueryData : public CRDataBase::crQueryData
{
public:
	crActivecodeQueryData();
	crActivecodeQueryData(const crActivecodeQueryData& data);
	DataClass(CRNetApp, ActivecodeQuery)
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
//class CRNETAPP_EXPORT crPlayerMoneyExistQueryData : public CRDataBase::crQueryData
//{
//public:
//	crPlayerMoneyExistQueryData();
//	crPlayerMoneyExistQueryData(const crPlayerMoneyExistQueryData& data);
//	DataClass(CRNetApp, PlayerMoneyExistQuery)
//	virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int playerid,int serverid);
//};
class CRNETAPP_EXPORT crPlayerMoneyQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerMoneyQueryData();
	crPlayerMoneyQueryData(const crPlayerMoneyQueryData& data);
	DataClass(CRNetApp, PlayerMoneyQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int serverid);
	int getMoney();
protected:
	int m_money;
};
//class CRNETAPP_EXPORT crRechargerecordQueryData : public CRDataBase::crQueryData
//{
//public:
//	crRechargerecordQueryData();
//	crRechargerecordQueryData(const crRechargerecordQueryData& data);
//	DataClass(CRNetApp, RechargerecordQuery)
//	virtual void addParam(int i, char *buf, int length);
//	void buildQuerySql(int serverid,int count);
//	int getID();
//	int getPlayerID();
//	int getMoney();
//protected:
//	int m_id;
//	int m_playerid;
//	int m_money;
//};
class CRNETAPP_EXPORT crGamelogOverdueQueryData : public CRDataBase::crQueryData
{//多组
public:
	crGamelogOverdueQueryData();
	crGamelogOverdueQueryData(const crGamelogOverdueQueryData& data);
	DataClass(CRNetApp, GamelogOverdueQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int overdue);
	int getID();
protected:
	int m_id;
};
class CRNETAPP_EXPORT crPlayerVipQueryData : public CRDataBase::crQueryData
{
public:
	crPlayerVipQueryData();
	crPlayerVipQueryData(const crPlayerVipQueryData& data);
	DataClass(CRNetApp, PlayerVipQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int playerid,int serverid);
	unsigned char getViplv();
	int getVipexp();
protected:
	unsigned char m_viplv;
	int m_vipexp;
};
class CRNETAPP_EXPORT crGameInfoQueryData : public CRDataBase::crQueryData
{
public:
	crGameInfoQueryData();
	crGameInfoQueryData(const crGameInfoQueryData& data);
	DataClass(CRNetApp, GameInfoQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql(int gameid);
	const std::string &getCName() const;
	const std::string &getPlatform() const;
protected:
	std::string m_cname;
	std::string m_platform;
};
////////////////////
//crGmQueryData
class crGmQueryData : public CRDataBase::crQueryData
{
public:
	crGmQueryData();
	crGmQueryData(const crGmQueryData& data);
	DataClass(CRNetApp, GmQuery)
	virtual void addParam(int i, char *buf, int length);
	void buildQuerySql();
	int getPlayerID();
	unsigned char getAuthorityid();
protected:
	int m_playerid;
	unsigned char m_authorityid;
};
}
#endif