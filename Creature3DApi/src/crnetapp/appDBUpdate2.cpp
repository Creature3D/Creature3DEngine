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
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appNetDefine.h>
using namespace CRNetApp;
using namespace CRCore;
using namespace CRDataBase;

/////////////////////////////////////////
//
//crUpdateOnlineTime
//
/////////////////////////////////////////
void crUpdateOnlineTime::buildUpdateSql(int playerid)
{
	char sql[128];
	sprintf(sql,"update account set onlinetime=addtime(onlinetime,timediff(now(),logindate)),logoutdate=now() where id=%d\0",playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crLockAccount
//
/////////////////////////////////////////
void crLockAccount::buildUpdateSql(int playerid,unsigned char locked)
{
	char sql[128];
	sprintf(sql,"update account set locked=%d,logoutdate=now() where id=%d\0",playerid,locked);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertGame
//
/////////////////////////////////////////
void crInsertGame::buildUpdateSql(const std::string &gamename, const std::string &uifile, const std::string &clientcfg, const std::string &basefilelist, unsigned short dbcount, unsigned char rolecount)
{
	char sql[512];
	int size = gamename.size();
	char escgamename[32];
	memset(escgamename,0,32);
	crDataBaseManager::getInstance()->escapeString(escgamename,gamename.c_str(),size);

	char escuifile[128];
	size = uifile.size();
	memset(escuifile,0,128);
	crDataBaseManager::getInstance()->escapeString(escuifile,uifile.c_str(),size);

	char escclientcfg[128];
	size = clientcfg.size();
	memset(escclientcfg,0,128);
	crDataBaseManager::getInstance()->escapeString(escclientcfg,clientcfg.c_str(),size);

	char escbasefile[128];
	size = basefilelist.size();
	memset(escbasefile,0,128);
	crDataBaseManager::getInstance()->escapeString(escbasefile,basefilelist.c_str(),size);

	sprintf(sql,"insert into game(gamename,uifile,clientcfg,basefilelist,dbcount,rolecount,createdate) values('%s','%s','%s','%s',%d,%d,now())\0",escgamename,escuifile,escclientcfg,escbasefile,(short)dbcount,(char)rolecount);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveGame
//
/////////////////////////////////////////
void crRemoveGame::buildUpdateSql(int gameid)
{
	char sql[128];
	sprintf(sql,"delete from game where id=%d\0",gameid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertRes
//
/////////////////////////////////////////
void crInsertRes::buildUpdateSql(const std::string &filename, const std::string &script)
{
	char sql[256];
	int size = filename.size();
	char escfilename[100];
	memset(escfilename,0,100);
	crDataBaseManager::getInstance()->escapeString(escfilename,filename.c_str(),size);

	char escscript[100];
	size = script.size();
	memset(escscript,0,100);
	crDataBaseManager::getInstance()->escapeString(escscript,script.c_str(),size);

	sprintf(sql,"insert into res(filename,script,createdate) values('%s','%s','%s',now())\0",escfilename,escscript);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveRes
//
/////////////////////////////////////////
void crRemoveRes::buildUpdateSql(int resid)
{
	char sql[128];
	sprintf(sql,"delete from res where id=%d\0",resid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertAbstractItem
//
/////////////////////////////////////////
void crInsertAbstractItem::buildUpdateSql(int resid, const std::string &name, const std::string &scriptfile)
{
	char sql[256];
	
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	size = scriptfile.size();
	char escscriptfile[128];
	memset(escscriptfile,0,128);
	crDataBaseManager::getInstance()->escapeString(escscriptfile,scriptfile.c_str(),size);

	sprintf(sql,"insert into abstractitem(resid,name,script,createdate) values(%d,'%s','%s',now())\0",resid,escname,escscriptfile);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveAbstractItem
//
/////////////////////////////////////////
void crRemoveAbstractItem::buildUpdateSql(int abstractitemid)
{
	char sql[128];
	sprintf(sql,"delete from abstractitem where id=%d\0",abstractitemid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertAbstractMetier
//
/////////////////////////////////////////
void crInsertAbstractMetier::buildUpdateSql(int gameid, const std::string &metiername, const std::string &introduce, int initsceneid, unsigned short initlayerid, int initx, int inity, unsigned short initr, short zoffset, const std::string &script, const std::string &tasktable)
{
	char sql[768];

	int size = metiername.size();
	char escmetiername[32];
	memset(escmetiername,0,32);
	crDataBaseManager::getInstance()->escapeString(escmetiername,metiername.c_str(),size);

	size = introduce.size();
	char escintroduce[256];
	memset(escintroduce,0,256);
	crDataBaseManager::getInstance()->escapeString(escintroduce,introduce.c_str(),size);

	size = script.size();
	char escscript[128];
	memset(escscript,0,128);
	crDataBaseManager::getInstance()->escapeString(escscript,script.c_str(),size);

	size = tasktable.size();
	char esctasktable[128];
	memset(esctasktable,0,128);
	crDataBaseManager::getInstance()->escapeString(esctasktable,tasktable.c_str(),size);

	sprintf(sql,"insert into abstractmetier(gameid,metiername,introduce,initsceneid,initlayerid,initx,inity,initr,zoffset,script,tasktable,createdate) values(%d,'%s','%s',%d,%d,%d,%d,%d,%d,'%s','%s',now())\0",gameid,escmetiername,escintroduce,initsceneid,(short)initlayerid,initx,inity,(short)initr,zoffset,escscript,esctasktable);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveAbstractMetier
//
/////////////////////////////////////////
void crRemoveAbstractMetier::buildUpdateSql(int abstractmetierid)
{
	char sql[128];
	sprintf(sql,"delete from abstractmetier where id = %d\0",abstractmetierid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crCreateInstanceItem
//
/////////////////////////////////////////
void crCreateInstanceItem::buildUpdateSql(int abstractitemid, unsigned char itemtype, const std::string &iname, int sceneid, unsigned short layerid, int posx, int posy, short zoffset/*, CRCore::crStreamBuf *stream*/)
{
	int size = iname.size();
	char esciname[32];
	memset(esciname,0,32);
	crDataBaseManager::getInstance()->escapeString(esciname,iname.c_str(),size);

	//char *escstream = NULL;
	//size = stream->getBufSize();
	//int escsize = 0;
	//if(size>0)
	//{
	//	escstream = new char[size+128];
	//	memset(escstream,0,size+128);
	//	crDataBase::escapeString(escstream,stream->getBuf(),size);
	//	escsize = strlen(escstream);
	//}
	//char *sql = new char[escsize+256];
	//sprintf(sql,"insert into instanceitem (abstractitemid, itemtype, iname, sceneid, layerid, posx, posy, posz, stream, level, createdate) values (%d, %d, '%s', %d, %d, %d, %d, %d, '%s', 1, now())\0",abstractitemid,(int)itemtype,esciname,sceneid,(int)layerid,posx,posy,posz,escstream);
	//setUpdateSql(sql);
	//delete escstream;
	//delete sql;

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(/*stream->getBufSize()+*/256);
	char sql[256];
	sprintf(sql,"insert into instanceitem(abstractitemid,itemtype,iname,sceneid,layerid,posx,posy,zoffset,createdate) values(%d,%d,'%s',%d,%d,%d,%d,%d,now())\0",abstractitemid,(char)itemtype,esciname,sceneid,(short)layerid,posx,posy,zoffset);
	m_sqlstream->write(sql,strlen(sql));
	//m_sqlstream->write(stream->getBuf(),stream->getBufSize());
	//sprintf(sql,"', now())\0");
	//m_sqlstream->write(sql,strlen(sql));
}
void crCreateInstanceItem::buildUpdateSql(int abstractitemid, unsigned char itemtype)
{
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(128);
	char sql[128];
	sprintf(sql,"insert into instanceitem(abstractitemid,itemtype,createdate) values(%d,%d,now())\0",abstractitemid,(char)itemtype);
	m_sqlstream->write(sql,strlen(sql));
}
void crCreateInstanceItem::buildUpdateSql(int abstractitemid, unsigned char itemtype, const std::string &iname)
{
	int size = iname.size();
	char esciname[32];
	memset(esciname,0,32);
	crDataBaseManager::getInstance()->escapeString(esciname,iname.c_str(),size);

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(128);
	char sql[128];
	sprintf(sql,"insert into instanceitem(abstractitemid,itemtype,iname,createdate) values(%d,%d,'%s',now())\0",abstractitemid,(char)itemtype,esciname);
	m_sqlstream->write(sql,strlen(sql));
}
/////////////////////////////////////////
//
//crUpdateInstanceItem
//
/////////////////////////////////////////
void crUpdateInstanceItem::buildUpdateSql(int instanceitemid,int sceneid,unsigned short layerid,int posx,int posy,float dirx,float diry,float dirz,CRCore::crStreamBuf *stream)
{
	//char *escstream = NULL;
	//int size = stream->getRemainSize();
	//int escsize = 0;
	//if(size>0)
	//{
	//	escstream = new char[size+128];
	//	memset(escstream,0,size+128);
	//	crDataBase::escapeString(escstream,stream->getPtr(),size);
	//	escsize = strlen(escstream);
	//}
	//CRCore::notify(CRCore::ALWAYS)<<"crUpdateInstanceItem::buildUpdateSql size = "<<size<<" escsize = "<<escsize<<std::endl;
	//char *sql = new char[escsize+256];
	//sprintf(sql,"update instanceitem set sceneid = %d, layerid = %d, posx = %d, posy = %d, posz = %d, dirx = %f, diry = %f, dirz = %f, stream = '%s', updatedate = now() where id = %d\0",sceneid,(int)layerid,posx,posy,posz,dirx,diry,dirz,escstream,instanceitemid);
	//setUpdateSql(sql);
	//delete escstream;
	//delete sql;
	int streamSize = 0;
	if(stream) streamSize = stream->getRemainSize();

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"update instanceitem set sceneid=%d,layerid=%d,posx=%d,posy=%d,dirx=%f,diry=%f,dirz=%f,stream=NULL\0",sceneid,(short)layerid,posx,posy,dirx,diry,dirz);
	    m_sqlstream->write(sql,strlen(sql));
		sprintf(sql,",updatedate=now() where id=%d\0",instanceitemid);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"update instanceitem set sceneid=%d,layerid=%d,posx=%d,posy=%d,dirx=%f,diry=%f,dirz=%f,stream='\0",sceneid,(short)layerid,posx,posy,dirx,diry,dirz);
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"',updatedate=now() where id=%d\0",instanceitemid);
		m_sqlstream->write(sql,strlen(sql));
	}
}
void crUpdateInstanceItem::buildUpdateSql(int instanceitemid,CRCore::crStreamBuf *stream)
{
	int streamSize = 0;
	if(stream) streamSize = stream->getRemainSize();

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"update instanceitem set stream=NULL\0");
		m_sqlstream->write(sql,strlen(sql));
		sprintf(sql,",updatedate=now() where id=%d\0",instanceitemid);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"update instanceitem set stream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"',updatedate=now() where id=%d\0",instanceitemid);
		m_sqlstream->write(sql,strlen(sql));
	}
}
void crUpdateInstanceItem::buildUpdateSql(int instanceitemid, const std::string &iname)
{
	int size = iname.size();
	char esciname[32];
	memset(esciname,0,32);
	crDataBaseManager::getInstance()->escapeString(esciname,iname.c_str(),size);

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(128);
	char sql[128];
	sprintf(sql,"update instanceitem set iname='%s',updatedate=now() where id=%d\0",esciname,instanceitemid);
	m_sqlstream->write(sql,strlen(sql));
}
/////////////////////////////////////////
//
//crRemoveInstanceItem
//
/////////////////////////////////////////
void crRemoveInstanceItem::buildUpdateSql(int instanceitemid)
{
	char sql[128];
	sprintf(sql,"delete from instanceitem where id=%d\0",instanceitemid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crCreateRole
//
/////////////////////////////////////////
void crCreateRole::buildUpdateSql(int playerid, int instanceitemid, int abstractmetierid/*, CRCore::crStreamBuf *metierstream*/)
{
	//char *escstream = NULL;
	//int size = metierstream->getBufSize();
	//int escsize = 0;
	//if(size>0)
	//{
	//	escstream = new char[size+128];
	//	memset(escstream,0,size+128);
	//	crDataBase::escapeString(escstream,metierstream->getBuf(),size);
	//	escsize = strlen(escstream);
	//}
	//char *sql = new char[escsize+128];
	//sprintf(sql,"insert into role (playerid, instanceitemid, metierid, metierstream, createdate) values (%d, %d, %d, '%s', now())\0",playerid,instanceitemid,abstractmetierid,escstream);
	//setUpdateSql(sql);
	//delete escstream;
	//delete sql;

	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(/*metierstream->getBufSize()+*/128);
	char sql[128];
	sprintf(sql,"insert into role(playerid,instanceitemid,metierid,createdate) values(%d,%d,%d,now())\0",playerid,instanceitemid,abstractmetierid);
	m_sqlstream->write(sql,strlen(sql));
	//m_sqlstream->write(metierstream->getBuf(),metierstream->getBufSize());
	//sprintf(sql,"', now())\0");
	//m_sqlstream->write(sql,strlen(sql));
}
/////////////////////////////////////////
//
//crUpdateRole
//
/////////////////////////////////////////
void crUpdateRole::buildUpdateSql(int roleid,CRCore::crStreamBuf *metierstream)
{
	//char *escstream = NULL;
	//int size = metierstream->getRemainSize();
	//int escsize = 0;
	//if(size>0)
	//{
	//	escstream = new char[size+128];
	//	memset(escstream,0,size+128);
	//	crDataBase::escapeString(escstream,metierstream->getPtr(),size);
	//	escsize = strlen(escstream);
	//}
	//char *sql = new char[escsize+128];
	//sprintf(sql,"update role set metierstream = '%s', updatedate = now() where id = %d\0",escstream,roleid);
	//setUpdateSql(sql);
	//delete escstream;
	//delete sql;

	int streamSize = 0;
	if(metierstream) streamSize = metierstream->getRemainSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+128);
	char sql[128];
	if(streamSize == 0)
	{
		sprintf(sql,"update role set metierstream=NULL,updatedate=now() where id=%d\0",roleid);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"update role set metierstream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(metierstream->getPtr(),metierstream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,metierstream->getPtr(),metierstream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"',updatedate=now() where id=%d\0",roleid);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crRemoveRole
//
/////////////////////////////////////////
void crRemoveRole::buildUpdateSql(int roleid)
{
	char sql[128];
	sprintf(sql,"delete from role where id=%d\0",roleid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crCreateItemChild
//
/////////////////////////////////////////
void crCreateItemChild::buildUpdateSql(int instanceitemid,int parentitemid)
{
	char sql[128];
	sprintf(sql,"insert into itemchild(instanceitemid,parentitemid,createdate) values(%d,%d,now())\0",instanceitemid,parentitemid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveItemChild
//
/////////////////////////////////////////
void crRemoveItemChild::buildUpdateSql(int itemchildid)
{
	char sql[128];
	sprintf(sql,"delete from itemchild where id=%d\0",itemchildid);
	setUpdateSql(sql);
}
void crRemoveItemChild::buildUpdateSql2(int parentitemid)
{
	char sql[128];
	sprintf(sql,"delete from itemchild where parentitemid=%d\0",parentitemid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertScene
//
/////////////////////////////////////////
void crInsertScene::buildUpdateSql(int gameid, const std::string &scenename, const std::string &filename, int birthx, int birthy, int birthr)
{
	char sql[256];
	int size = scenename.size();
	char escscenename[32];
	memset(escscenename,0,32);
	crDataBaseManager::getInstance()->escapeString(escscenename,scenename.c_str(),size);

	char escfilename[100];
	size = filename.size();
	memset(escfilename,0,100);
	crDataBaseManager::getInstance()->escapeString(escfilename,filename.c_str(),size);

	sprintf(sql,"insert into scene(gameid,scenename,filename,birthx,birthy,birthr,createdate) values(%d,'%s','%s','%s','%s',%d,%d,%d,now())\0",gameid,escscenename,escfilename,birthx,birthy,birthr);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveScene
//
/////////////////////////////////////////
void crRemoveScene::buildUpdateSql(int sceneid)
{
	char sql[128];
	sprintf(sql,"delete from scene where id = %d\0",sceneid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerGameDB
//
/////////////////////////////////////////
void crInsertPlayerGameDB::buildUpdateSql(int playerid, int serverid, unsigned short gamedbid)
{
	char sql[128];
	sprintf(sql,"insert into playergamedb(playerid,serverid,gamedbid,createdate) values(%d,%d,%d,now())\0",playerid,serverid,(short)gamedbid);
	setUpdateSql(sql);
}
///////////////////////////////////////////
////
////crUpdatePlayerGameDB
////
///////////////////////////////////////////
//void crUpdatePlayerGameDB::buildUpdateSql(int id,CRCore::crStreamBuf *stream)
//{
//	int streamSize = 0;
//	if(stream) streamSize = stream->getRemainSize();
//	m_sqlstream = new crStreamBuf;
//	m_sqlstream->createBuf(streamSize+128);
//	char sql[128];
//	if(streamSize == 0)
//	{
//		sprintf(sql,"update playergamedb set stream = NULL, updatedate = now() where id = %d\0",id);
//		m_sqlstream->write(sql,strlen(sql));
//	}
//	else
//	{
//		sprintf(sql,"update playergamedb set stream = '\0");
//		m_sqlstream->write(sql,strlen(sql));
//		m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
//		sprintf(sql,"', updatedate = now() where id = %d\0",id);
//		m_sqlstream->write(sql,strlen(sql));
//	}
//}
/////////////////////////////////////////
//
//crRemovePlayerGameDB
//
/////////////////////////////////////////
void crRemovePlayerGameDB::buildUpdateSql(int playerid, int serverid)
{
	char sql[128];
	sprintf(sql,"delete from playergamedb where playerid=%d and serverid=%d\0",playerid,serverid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerGameData
//
/////////////////////////////////////////
void crInsertPlayerGameData::buildUpdateSql(int playerid,int mainroleid)
{
	char sql[128];
	sprintf(sql,"insert into playergamedata(playerid,mainroleid,createdate) values(%d,%d,now())\0",playerid,mainroleid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdatePlayerGameData
//
/////////////////////////////////////////
void crUpdatePlayerGameData::buildUpdateSql(int id,CRCore::crStreamBuf *stream)
{
	int streamSize = 0;
	if(stream) streamSize = stream->getRemainSize();
	m_sqlstream = new crStreamBuf;
	m_sqlstream->createBuf(streamSize+256);
	char sql[256];
	if(streamSize == 0)
	{
		sprintf(sql,"update playergamedata set stream=null,updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
	else
	{
		sprintf(sql,"update playergamedata set stream='\0");
		m_sqlstream->write(sql,strlen(sql));
		//m_sqlstream->write(stream->getPtr(),stream->getRemainSize());
		char* escstream = new char[streamSize*2+1];
		unsigned long len = crDataBaseManager::getInstance()->escapeString(escstream,stream->getPtr(),stream->getRemainSize());
		m_sqlstream->write(escstream,len);
		delete [] escstream;
		sprintf(sql,"', updatedate=now() where id=%d\0",id);
		m_sqlstream->write(sql,strlen(sql));
	}
}
/////////////////////////////////////////
//
//crRemovePlayerGameData
//
/////////////////////////////////////////
void crRemovePlayerGameData::buildUpdateSql(int playerid)
{
	char sql[64];
	sprintf(sql,"delete from playergamedata where playerid=%d\0",playerid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertCharacterNameData
//
/////////////////////////////////////////
void crInsertCharacterName::buildUpdateSql(int playerid, int serverid, int roleid, const std::string &name)
{
	char sql[256];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	sprintf(sql,"insert into charactername(playerid,serverid,roleid,name,createdate) values(%d,%d,%d,'%s',now())\0",playerid,serverid,roleid,escname);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateCharacterName
//
/////////////////////////////////////////
void crUpdateCharacterName::buildUpdateSql(int playerid, int serverid, int roleid, const std::string &name)
{
	char sql[256];
	int size = name.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,name.c_str(),size);

	sprintf(sql,"update charactername set name='%s',updatedate=now() where playerid=%d and serverid=%d and roleid=%d\0",escname,playerid,serverid,roleid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveCharacterNameData
//
/////////////////////////////////////////
void crRemoveCharacterName::buildUpdateSql(int playerid, int gameid, int roleid)
{
	char sql[128];
	sprintf(sql,"delete from charactername where playerid=%d and gameid=%d and roleid=%d\0",playerid,gameid,roleid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertSale
//
/////////////////////////////////////////
void crInsertSale::buildUpdateSql(int playerid, const std::string &playername, int goodstype, int goodsid, int price, int count, unsigned char validterm, unsigned char salestate)
{
	char sql[256];
	int size = playername.size();
	char escname[32];
	memset(escname,0,32);
	crDataBaseManager::getInstance()->escapeString(escname,playername.c_str(),size);

	sprintf(sql,"insert into sale(playerid,playername,goodstype,goodsid,price,salecount,validterm,salestate,createdate) values(%d,'%s',%d,%d,%d,%d,%d,%d,now())\0",playerid,escname,goodstype,goodsid,price,count,(char)validterm,(char)salestate);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateSale
//
/////////////////////////////////////////
void crUpdateSale::buildUpdateSql(int id,unsigned char salestate,int gaincash)
{
	char sql[256];
	sprintf(sql,"update sale set salestate=%d,gaincash=%d,updatedate=now() where id=%d and salestate=%d\0",(char)salestate,gaincash,id,Sale_OnSale);
	setUpdateSql(sql);
}
void crUpdateSale::buildUpdateSql(int id,int price, int count, unsigned char validterm, unsigned char salestate)
{
	char sql[256];
	sprintf(sql,"update sale set price=%d,salecount=%d,validterm=%d,salestate=%d,updatedate=now() where id=%d\0",price,count,(char)validterm,(char)salestate,id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveSale
//
/////////////////////////////////////////
void crRemoveSale::buildUpdateSql(int id)
{
	char sql[64];
	sprintf(sql,"delete from sale where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateServerList
//
/////////////////////////////////////////
void crUpdateServerList::buildUpdateSql(int serverid, short remaincount,short onlinecount)
{
	char sql[256];
	sprintf(sql,"update serverlist set remaincount=%d,onlinecount=%d,updatedate=now() where id=%d and remaincount>=-1000\0",remaincount,onlinecount,serverid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertServerList
//
/////////////////////////////////////////
void crInsertServerList::buildUpdateSql(int gameid, int webserverid,const std::string &site,const std::string &serverid,const std::string &servername, unsigned char isp,short remaincount)
{
	char sql[256];
	//int size = servername.size();
	//char escname[32];
	//memset(escname,0,32);
	//crDataBase::escapeString(escname,servername.c_str(),size);
	if(site.empty())
	{
		sprintf(sql,"insert into serverlist(gameid,webserverid,serverid,servername,isp,remaincount,createdate) values(%d,%d,,'%s','%s',%d,%d,now())\0",gameid,webserverid,serverid.c_str(),servername.c_str(),(char)isp,remaincount);
	}
	else
	{
		sprintf(sql,"insert into serverlist(gameid,webserverid,site,serverid,servername,isp,remaincount,createdate) values(%d,%d,'%s','%s','%s',%d,%d,now())\0",gameid,webserverid,site.c_str(),serverid.c_str(),servername.c_str(),(char)isp,remaincount);
	}
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateConnectServers
//
/////////////////////////////////////////
void crUpdateConnectServers::buildUpdateSql(int serverid, const std::string &ip, const std::string &ip2, const std::string &ip3, unsigned short port, short remaincount)
{
	char sql[256];
	//int size = ip.size();
	//char escip[32];
	//memset(escip,0,32);
	//crDataBase::escapeString(escip,ip.c_str(),size);
	//char escip2[32];
	//memset(escip2,0,32);
	//crDataBase::escapeString(escip2,ip2.c_str(),size);
	sprintf(sql,"update connectservers set remaincount=%d,ip='%s',ip2='%s',ip3='%s',updatedate=now() where serverid=%d and port=%d\0",remaincount,ip.c_str(),ip2.c_str(),ip3.c_str(),serverid,(short)port);
	setUpdateSql(sql);
}
void crUpdateConnectServers::buildUpdateSql2(int serverid, unsigned short port, short remaincount)
{
	char sql[256];
	sprintf(sql,"update connectservers set remaincount=%d,updatedate=now() where serverid=%d and port=%d\0",remaincount,serverid,(short)port);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertConnectServers
//
/////////////////////////////////////////
void crInsertConnectServers::buildUpdateSql(int serverid, const std::string &ip, const std::string &ip2, const std::string &ip3, unsigned short port, short remaincount)
{
	char sql[256];
	//int size = ip.size();
	//char escip[32];
	//memset(escip,0,32);
	//crDataBase::escapeString(escip,ip.c_str(),size);
	//char escip2[32];
	//memset(escip2,0,32);
	//crDataBase::escapeString(escip2,ip2.c_str(),size);
	sprintf(sql,"insert into connectservers(serverid,ip,ip2,ip3,port,remaincount,createdate) values(%d,'%s','%s','%s',%d,%d,now())\0",serverid,ip.c_str(),ip2.c_str(),ip3.c_str(),(short)port,remaincount);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateActiveCode
//
/////////////////////////////////////////
void crUpdateActiveCode::buildUpdateSql(int id)
{
	char sql[128];
	sprintf(sql,"update activecode set used=1,updatedate=now() where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertActiveCode
//
/////////////////////////////////////////
void crInsertActiveCode::buildUpdateSql(const std::string &code)
{
	char sql[256];
	sprintf(sql,"insert into activecode(code,used) values('%s',0)\0",code.c_str());
	setUpdateSql(sql);
}
void crInsertActiveCode::buildUpdateSql(const std::string &code,const std::string &validdate)
{
	if(validdate.empty())
	{
		buildUpdateSql(code);
	}
	else
	{
		char sql[256];
		sprintf(sql,"insert into activecode(code,used,validdate) values('%s',0,'%s')\0",code.c_str(),validdate.c_str());
		setUpdateSql(sql);
	}
}
/////////////////////////////////////////
//
//crInsertPlayerMoney
//
/////////////////////////////////////////
void crInsertPlayerMoney::buildUpdateSql(int playerid,int serverid,int money)
{
	char sql[256];
	sprintf(sql,"insert into playermoney(playerid,serverid,money,createdate) values(%d,%d,%d,now())\0",playerid,serverid,money);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdatePlayerMoney
//
/////////////////////////////////////////
void crUpdatePlayerMoney::buildUpdateSql(int playerid,int serverid, int money)
{
	char sql[256];
	sprintf(sql,"update playermoney set money=money+%d,updatedate=now() where playerid=%d and serverid=%d\0",money,playerid,serverid);
	setUpdateSql(sql);
}
///////////////////////////////////////////
////
////crInsertRechargerecord
////
///////////////////////////////////////////
//void crInsertRechargerecord::buildUpdateSql(int playerid,int serverid,int money,const std::string &str)
//{
//	char sql[256];
//	sprintf(sql,"insert into Rechargerecord(playerid,serverid,money,str,stats,createdate) values(%d,%d,%d,'%s',1,now())\0",playerid,serverid,money,str.c_str());//stats1表示充值，0表示充值已经提取
//	setUpdateSql(sql);
//}
///////////////////////////////////////////
////
////crUpdateRechargerecord
////
///////////////////////////////////////////
//void crUpdateRechargerecord::buildUpdateSql(int id)
//{
//	char sql[128];
//	sprintf(sql,"update Rechargerecord set stats=0 where id=%d\0",id);//stats1表示充值，0表示充值已经提取
//	setUpdateSql(sql);
//}
///////////////////////////////////////////
////
////crRemoveRechargerecord
////
///////////////////////////////////////////
//void crRemoveRechargerecord::buildUpdateSql(int id)
//{
//	char sql[128];
//	sprintf(sql,"delete from Rechargerecord where id=%d\0",id);
//	setUpdateSql(sql);
//}
/////////////////////////////////////////
//
//crInsertMoneyrecord
//
/////////////////////////////////////////
void crInsertMoneyrecord::buildUpdateSql(int playerid,int serverid,int money,int oldmoney,const std::string &str)
{
	char sql[256];
	sprintf(sql,"insert into moneyrecord(playerid,serverid,money,oldmoney,str,createdate) values(%d,%d,%d,%d,'%s',now())\0",playerid,serverid,money,str.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertLog
//
/////////////////////////////////////////
void crInsertGameLog::buildUpdateSql(int playerid,unsigned short logtype,const std::string &log)
{
	char sql[384];
	sprintf(sql,"insert into gamelog(playerid,logtype,logdata,createdate) values(%d,%d,'%s',now())\0",playerid,logtype,log.c_str());
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crRemoveGameLog
//
/////////////////////////////////////////
void crRemoveGameLog::buildUpdateSql(int id)
{
	char sql[64];
	sprintf(sql,"delete from gamelog where id=%d\0",id);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crInsertPlayerVip
//
/////////////////////////////////////////
void crInsertPlayerVip::buildUpdateSql(int playerid,int serverid,char viplv,int vipexp)
{
	char sql[256];
	sprintf(sql,"insert into playervip(playerid,serverid,viplv,vipexp,createdate) values(%d,%d,%d,%d,now())\0",playerid,serverid,(int)viplv,vipexp);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdatePlayerMoney
//
/////////////////////////////////////////
void crUpdatePlayerVip::buildUpdateSql(int playerid,int serverid,char viplv,int vipexp)
{
	char sql[256];
	sprintf(sql,"update playervip set viplv=viplv+%d,vipexp=vipexp+%d,updatedate=now() where playerid=%d and serverid=%d\0",(int)viplv,vipexp,playerid,serverid);
	setUpdateSql(sql);
}
/////////////////////////////////////////
//
//crUpdateCharacterOnlineState
//
/////////////////////////////////////////
void crUpdateCharacterOnlineState::buildUpdateSql(int playerid,int serverid,int roleid,bool online)
{
	char sql[256];
	sprintf(sql,"update charactername set onlinestate=%d where playerid=%d and serverid=%d and roleid=%d\0",online?1:0,playerid,serverid,roleid);
	setUpdateSql(sql);
}