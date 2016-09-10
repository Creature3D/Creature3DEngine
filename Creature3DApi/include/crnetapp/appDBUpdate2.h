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
#ifndef CRNETAPP_DBUPDATE2_H
#define CRNETAPP_DBUPDATE2_H 1

#include <CRNetApp/appExport.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRCore/crStreamBuf.h>
#include <string>

namespace CRNetApp{

class CRNETAPP_EXPORT crUpdateOnlineTime : public CRDataBase::crUpdateSql
{
public:
	crUpdateOnlineTime(){}
	void buildUpdateSql(int playerid);
};

class CRNETAPP_EXPORT crLockAccount : public CRDataBase::crUpdateSql
{
public:
	crLockAccount(){}
	void buildUpdateSql(int playerid,unsigned char locked);//0解锁，1玩家锁定，2官方锁定
};

class CRNETAPP_EXPORT crInsertGame : public CRDataBase::crUpdateSql
{
public:
	crInsertGame(){}
	void buildUpdateSql(const std::string &gamename, const std::string &uifile, const std::string &clientcfg, const std::string &basefilelist, unsigned short dbcount, unsigned char rolecount/*单个账号的最大角色数量*/);
};

class CRNETAPP_EXPORT crRemoveGame : public CRDataBase::crUpdateSql
{
public:
	crRemoveGame(){}
	void buildUpdateSql(int gameid);
};

class CRNETAPP_EXPORT crInsertRes : public CRDataBase::crUpdateSql
{
public:
	crInsertRes(){}
	void buildUpdateSql(const std::string &filename, const std::string &script);
};

class CRNETAPP_EXPORT crRemoveRes : public CRDataBase::crUpdateSql
{
public:
	crRemoveRes(){}
	void buildUpdateSql(int resid);
};

class CRNETAPP_EXPORT crInsertAbstractItem : public CRDataBase::crUpdateSql
{
public:
	crInsertAbstractItem(){}
	void buildUpdateSql(int resid, const std::string &name, const std::string &scriptfile);
};

class CRNETAPP_EXPORT crRemoveAbstractItem : public CRDataBase::crUpdateSql
{
public:
	crRemoveAbstractItem(){}
	void buildUpdateSql(int abstractitemid);
};

class CRNETAPP_EXPORT crInsertAbstractMetier : public CRDataBase::crUpdateSql
{
public:
	crInsertAbstractMetier(){}
	void buildUpdateSql(int gameid, const std::string &metiername, const std::string &introduce, int initsceneid, unsigned short initlayerid, int initx, int inity, unsigned short initr, short zoffset, const std::string &script, const std::string &tasktable);
};

class CRNETAPP_EXPORT crRemoveAbstractMetier : public CRDataBase::crUpdateSql
{
public:
	crRemoveAbstractMetier(){}
	void buildUpdateSql(int abstractmetierid);
};

class CRNETAPP_EXPORT crCreateInstanceItem : public CRDataBase::crUpdateSql
{
public:
	crCreateInstanceItem(){}
	void buildUpdateSql(int abstractitemid, unsigned char itemtype, const std::string &name, int sceneid, unsigned short layerid, int posx, int posy, short zoffset/*, CRCore::crStreamBuf *stream*/);
	void buildUpdateSql(int abstractitemid, unsigned char itemtype/*, CRCore::crStreamBuf *stream*/);
	void buildUpdateSql(int abstractitemid, unsigned char itemtype, const std::string &iname);
};

class CRNETAPP_EXPORT crUpdateInstanceItem : public CRDataBase::crUpdateSql
{
public:
	crUpdateInstanceItem(){}
	void buildUpdateSql(int instanceitemid,int sceneid,unsigned short layerid,int posx,int posy,float dirx,float diry,float dirz,CRCore::crStreamBuf *stream);
	void buildUpdateSql(int instanceitemid,CRCore::crStreamBuf *stream);
	void buildUpdateSql(int instanceitemid,const std::string &iname);
};

class CRNETAPP_EXPORT crRemoveInstanceItem : public CRDataBase::crUpdateSql
{
public:
	crRemoveInstanceItem(){}
	void buildUpdateSql(int instanceitemid);
};

class CRNETAPP_EXPORT crCreateRole : public CRDataBase::crUpdateSql
{
public:
	crCreateRole(){}
	void buildUpdateSql(int playerid, int instanceitemid, int abstractmetierid/*, CRCore::crStreamBuf *metierstream*/);
};

class CRNETAPP_EXPORT crUpdateRole : public CRDataBase::crUpdateSql
{
public:
	crUpdateRole(){}
	void buildUpdateSql(int roleid,CRCore::crStreamBuf *metierstream);
};

class CRNETAPP_EXPORT crRemoveRole : public CRDataBase::crUpdateSql
{
public:
	crRemoveRole(){}
	void buildUpdateSql(int roleid);
};

class CRNETAPP_EXPORT crCreateItemChild : public CRDataBase::crUpdateSql
{
public:
	crCreateItemChild(){}
	void buildUpdateSql(int instanceitemid,int parentitemid);
};

class CRNETAPP_EXPORT crRemoveItemChild : public CRDataBase::crUpdateSql
{
public:
	crRemoveItemChild(){}
	void buildUpdateSql(int itemchildid);
	void buildUpdateSql2(int parentitemid);
};

class CRNETAPP_EXPORT crInsertScene : public CRDataBase::crUpdateSql
{
public:
	crInsertScene(){}
	void buildUpdateSql(int gameid, const std::string &scenename, const std::string &filename,int birthx, int birthy, int birthr);
};

class CRNETAPP_EXPORT crRemoveScene : public CRDataBase::crUpdateSql
{
public:
	crRemoveScene(){}
	void buildUpdateSql(int sceneid);
};

class CRNETAPP_EXPORT crInsertPlayerGameDB : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerGameDB(){}
	void buildUpdateSql(int playerid, int serverid, unsigned short gamedbid);
};

//class CRNETAPP_EXPORT crUpdatePlayerGameDB : public CRDataBase::crUpdateSql
//{
//public:
//	crUpdatePlayerGameDB(){}
//	void buildUpdateSql(int id,CRCore::crStreamBuf *stream);
//};

class CRNETAPP_EXPORT crRemovePlayerGameDB : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerGameDB(){}
	void buildUpdateSql(int playerid, int serverid);
};

class CRNETAPP_EXPORT crInsertPlayerGameData : public CRDataBase::crUpdateSql
{
public:
	crInsertPlayerGameData(){}
	void buildUpdateSql(int playerid,int mainroleid = 0);
};

class CRNETAPP_EXPORT crUpdatePlayerGameData : public CRDataBase::crUpdateSql
{
public:
	crUpdatePlayerGameData(){}
	void buildUpdateSql(int id,CRCore::crStreamBuf *stream);
};

class CRNETAPP_EXPORT crRemovePlayerGameData : public CRDataBase::crUpdateSql
{
public:
	crRemovePlayerGameData(){}
	void buildUpdateSql(int playerid);
};
class CRNETAPP_EXPORT crInsertCharacterName : public CRDataBase::crUpdateSql
{
public:
	crInsertCharacterName(){}
	void buildUpdateSql(int playerid, int serverid, int roleid, const std::string &name);
};
class CRNETAPP_EXPORT crUpdateCharacterName : public CRDataBase::crUpdateSql
{
public:
	crUpdateCharacterName(){}
	void buildUpdateSql(int playerid, int serverid, int roleid, const std::string &name);
};
class CRNETAPP_EXPORT crRemoveCharacterName : public CRDataBase::crUpdateSql
{
public:
	crRemoveCharacterName(){}
	void buildUpdateSql(int playerid, int gameid, int roleid);
};
class CRNETAPP_EXPORT crInsertSale : public CRDataBase::crUpdateSql
{
public:
	crInsertSale(){}
	void buildUpdateSql(int playerid, const std::string &playername, int goodstype, int goodsid, int price, int count, unsigned char validity, unsigned char salestate);
};

class CRNETAPP_EXPORT crUpdateSale : public CRDataBase::crUpdateSql
{
public:
	crUpdateSale(){}
	void buildUpdateSql(int id,unsigned char salestate,int gaincash);
	void buildUpdateSql(int id,int price, int count, unsigned char validity, unsigned char salestate);
};

class CRNETAPP_EXPORT crRemoveSale : public CRDataBase::crUpdateSql
{
public:
	crRemoveSale(){}
	void buildUpdateSql(int id);
};
class CRNETAPP_EXPORT crUpdateServerList : public CRDataBase::crUpdateSql
{
public:
	crUpdateServerList(){}
	void buildUpdateSql(int serverid, short remaincount,short onlinecount);
};
class CRNETAPP_EXPORT crInsertServerList : public CRDataBase::crUpdateSql
{
public:
	crInsertServerList(){}
	void buildUpdateSql(int gameid, int webserverid,const std::string &site, const std::string &serverid,const std::string &servername, unsigned char isp,short remaincount);
};
class CRNETAPP_EXPORT crUpdateConnectServers : public CRDataBase::crUpdateSql
{
public:
	crUpdateConnectServers(){}
	void buildUpdateSql(int serverid, const std::string &ip, const std::string &ip2, const std::string &ip3, unsigned short port, short remaincount);
	void buildUpdateSql2(int serverid, unsigned short port, short remaincount);
};
class CRNETAPP_EXPORT crInsertConnectServers : public CRDataBase::crUpdateSql
{
public:
	crInsertConnectServers(){}
	void buildUpdateSql(int serverid, const std::string &ip, const std::string &ip2,const std::string &ip3, unsigned short port,short remaincount);
};
class CRNETAPP_EXPORT crUpdateActiveCode : public CRDataBase::crUpdateSql
{
public:
	crUpdateActiveCode(){}
	void buildUpdateSql(int id);
};
class CRNETAPP_EXPORT crInsertActiveCode : public CRDataBase::crUpdateSql
{
public:
	crInsertActiveCode(){}
	void buildUpdateSql(const std::string &code);
	void buildUpdateSql(const std::string &code,const std::string &validdate);
};
class CRNETAPP_EXPORT crInsertPlayerMoney : public CRDataBase::crUpdateSql
{//玩家元宝，首次充值时自动创建
public:
	crInsertPlayerMoney(){}
	void buildUpdateSql(int playerid,int serverid,int money);
};
class CRNETAPP_EXPORT crUpdatePlayerMoney : public CRDataBase::crUpdateSql
{//玩家元宝更新
public:
	crUpdatePlayerMoney(){}
	void buildUpdateSql(int playerid,int serverid,int money);
};
//class CRNETAPP_EXPORT crInsertRechargerecord : public CRDataBase::crUpdateSql
//{//玩家充值
//public:
//	crInsertRechargerecord(){}
//	void buildUpdateSql(int playerid,int serverid,int money,const std::string &str);
//};
//class CRNETAPP_EXPORT crUpdateRechargerecord : public CRDataBase::crUpdateSql
//{//提取充值变成元宝
//public:
//	crUpdateRechargerecord(){}
//	void buildUpdateSql(int id);
//};
//class CRNETAPP_EXPORT crRemoveRechargerecord : public CRDataBase::crUpdateSql
//{//提取充值变成元宝
//public:
//	crRemoveRechargerecord(){}
//	void buildUpdateSql(int id);
//};
class CRNETAPP_EXPORT crInsertMoneyrecord : public CRDataBase::crUpdateSql
{//玩家元宝变动记录
public:
	crInsertMoneyrecord(){}
	void buildUpdateSql(int playerid,int serverid,int money,int oldmoney,const std::string &str);
};
class CRNETAPP_EXPORT crInsertGameLog : public CRDataBase::crUpdateSql
{
public:
	crInsertGameLog(){}
	void buildUpdateSql(int playerid,unsigned short logtype,const std::string &log);//playerid==0表示服务器日志
};
class CRNETAPP_EXPORT crRemoveGameLog : public CRDataBase::crUpdateSql
{
public:
	crRemoveGameLog(){}
	void buildUpdateSql(int id);
};
class CRNETAPP_EXPORT crInsertPlayerVip : public CRDataBase::crUpdateSql
{//
public:
	crInsertPlayerVip(){}
	void buildUpdateSql(int playerid,int serverid,char viplv,int vipexp);
};
class CRNETAPP_EXPORT crUpdatePlayerVip : public CRDataBase::crUpdateSql
{//
public:
	crUpdatePlayerVip(){}
	void buildUpdateSql(int playerid,int serverid,char viplv,int vipexp);
};
class CRNETAPP_EXPORT crUpdateCharacterOnlineState: public CRDataBase::crUpdateSql
{//
public:
	crUpdateCharacterOnlineState(){}
	void buildUpdateSql(int playerid,int serverid,int roleid,bool online);
};
}
#endif
