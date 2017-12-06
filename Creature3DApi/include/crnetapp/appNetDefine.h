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
#ifndef CRNETAPP_NETDEFINE_H
#define CRNETAPP_NETDEFINE_H 1

//WCHNETID = 5
#define WCHNET_APPSERVERREFUSE     WCHNETID+5
#define WCHNET_APPLOGOUT           WCHNETID+6

#define WCHNET_LOGIN               WCHNETID+7
#define WCHNET_LOGINCHAT           WCHNETID+8
#define WCHNET_LOGINGAME           WCHNETID+9
#define WCHNET_LOGINSCENE          WCHNETID+10
#define WCHNET_LOGINPARENT         WCHNETID+11
#define WCHNET_LOGINVOICE          WCHNETID+12
#define WCHNET_LOGINCAMERA         WCHNETID+13
#define WCHNET_LOGINDOWNLOAD       WCHNETID+14

#define WCHNET_DATASTREAM          WCHNETID+15
#define WCHNET_CHATSTATECHANGE     WCHNETID+16
#define WCHNET_CHATFRIENDLIST      WCHNETID+17
#define WCHNET_CHATGROUPLIST       WCHNETID+18
#define WCHNET_CHATGROUPMEMBERLIST WCHNETID+19
#define WCHNET_CHATDATATRANSFEREND WCHNETID+20

#define WCHNET_ADDCHATFRIEND       WCHNETID+21
#define WCHNET_REMOVECHATFRIEND    WCHNETID+22
#define WCHNET_CHAT                WCHNETID+23
#define WCHNET_GROUPCHAT           WCHNETID+24
#define WCHNET_APPLYJOINGROUP      WCHNETID+25
#define WCHNET_ACCEPTJOINGROUP     WCHNETID+26
#define WCHNET_NEWMEMBERCOMING     WCHNETID+27
#define WCHNET_ADDCHATGROUP        WCHNETID+28
#define WCHNET_ADDGROUPMEMBER      WCHNETID+29
#define WCHNET_EXITGROUP           WCHNETID+30
#define WCHNET_REMOVEGROUPMEMBER   WCHNETID+31
#define WCHNET_QUERYONLINEPLAYERLIST    WCHNETID+32
#define WCHNET_QUERYCHATGROUPLIST       WCHNETID+33

#define WCHNET_REGACCOUNT          WCHNETID+34
#define WCHNET_DELACCOUNT          WCHNETID+35
#define WCHNET_UPDPASSWORD         WCHNETID+36

#define WCHNET_CREATECHATGROUP     WCHNETID+37
#define WCHNET_CREATEDATASTREAM    WCHNETID+38

#define WCHNET_QUERYGAMELIST       WCHNETID+50
#define WCHNET_DOWNLOADFILE        WCHNETID+51
#define WCHNET_REQUESTFILESTREAM   WCHNETID+52
//#define WCHNET_DOWNLOADFILEEND     WCHNETID+52
#define WCHNET_QUERYGAMEROLELIST   WCHNETID+53
#define WCHNET_CREATEROLE          WCHNETID+54
#define WCHNET_QUERYSCENEINFO      WCHNETID+55
#define WCHNET_SCENESERVERINIT     WCHNETID+56
#define WCHNET_QUERYSCENEITEM      WCHNETID+57
#define WCHNET_LOGINSCENE2         WCHNETID+58
#define WCHNET_QUERYROLEINFO       WCHNETID+59
#define WCHNET_QUERYROLEINFO2      WCHNETID+60
//#define WCHNET_GETROLEZ            WCHNETID+61
#define WCHNET_FILESTREAM          WCHNETID+61
#define WCHNET_RECVGAMELIST        WCHNETID+62
//#define WCHNET_PLAYERINRANGE       WCHNETID+61
//#define WCHNET_PLAYEROUTRANGE      WCHNETID+62
#define WCHNET_REQUESTPLAYER       WCHNETID+63
#define WCHNET_REGISTERSCENESERVER WCHNETID+64
#define WCHNET_ITEMINRANGE         WCHNETID+65
#define WCHNET_ITEMOUTRANGE        WCHNETID+66
//#define WCHNET_NPCINRANGE          WCHNETID+66
#define WCHNET_REQUESTITEM         WCHNETID+67
#define WCHNET_PLAYEREVENT         WCHNETID+68
#define WCHNET_REQUESTTASKAWARDINFO WCHNETID+69
//#define WCHNET_PLAYERCHILDITEMEVENT WCHNETID+69
#define WCHNET_ITEMEVENT           WCHNETID+70
#define WCHNET_REQUESTITEMCHILD	   WCHNETID+71
//#define WCHNET_CHILDITEMEVENT      WCHNETID+71
#define WCHNET_SETCLIENTRUNNING    WCHNETID+72
#define WCHNET_REMOVEPLAYER        WCHNETID+73
#define WCHNET_SAVEITEMDATA        WCHNETID+74
#define WCHNET_SAVEROLEDATA        WCHNETID+75
#define WCHNET_CREATEITEMCHILD     WCHNETID+76
#define WCHNET_PLAYERSERVEREVENT       WCHNETID+77
#define WCHNET_PLAYERSERVERMETIEREVENT WCHNETID+78
#define WCHNET_PLAYERMETIEREVENT       WCHNETID+79
#define WCHNET_REQUESTEMPORIUM         WCHNETID+80
//#define WCHNET_ADDDAMAGE               WCHNETID+78
//#define WCHNET_ITEMDEAD                WCHNETID+79
//#define WCHNET_NPCOUTRANGE           WCHNETID+81
#define WCHNET_REQUESTITEMRTDATA       WCHNETID+81
//#define WCHNET_ADDTARGETEXTRA          WCHNETID+81
#define WCHNET_QUERYABSTRACTITEM       WCHNETID+82
#define WCHNET_CREATETEMPORARYITEMCHILD WCHNETID+83
#define WCHNET_REMOVEITEMCHILD          WCHNETID+84
#define WCHNET_SCENECHAT                WCHNETID+85
#define WCHNET_GAMECHAT                 WCHNETID+86

#define WCHNET_QUERYSCENELIST           WCHNETID+87
#define WCHNET_QUERYSCENEROOMS          WCHNETID+88
#define WCHNET_CREATESCENEROOM          WCHNETID+89
#define WCHNET_PREENTERSCENEROOM        WCHNETID+90
//#define WCHNET_ROOMPLAYERDATA           WCHNETID+91
#define WCHNET_REMOVEROOMGAME			WCHNETID+91
#define WCHNET_SAVEPLAYERGAMEDATA       WCHNETID+92
#define WCHNET_STARTROOMGAME            WCHNETID+93
#define WCHNET_ENDROOMGAME              WCHNETID+94
#define WCHNET_LOGOUTSCENE              WCHNETID+95
#define WCHNET_REMOVEROLE				WCHNETID+96
#define WCHNET_ROOMGAMETIME				WCHNETID+97
#define WCHNET_ENTERSCENEROOM           WCHNETID+98
#define WCHNET_ROOMPLAYEREVENT          WCHNETID+99
#define WCHNET_ROOMADDMEMBER		    WCHNETID+100
#define WCHNET_ROOMREMOVEMEMBER	        WCHNETID+101
#define WCHNET_ROOMSETPLAYERGAMEDATA    WCHNETID+102
#define WCHNET_ROOMPLAYERGAMEDATAUPDATE WCHNETID+103
#define WCHNET_CHECKPLAYERGAMENAME		WCHNETID+103
#define WCHNET_CREATEPLAYERGAMEDATA		WCHNETID+104
#define WCHNET_PLAYERDATACREATESTREAM   WCHNETID+105
#define WCHNET_PLAYERDATAUPDATESTREAM   WCHNETID+106
#define WCHNET_PLAYERDATAEVENT          WCHNETID+107
#define WCHNET_SAVEITEMSTREAM			WCHNETID+108
#define WCHNET_PLAYERDATASCENEEVENT     WCHNETID+109
#define WCHNET_LOGINSCENE3				WCHNETID+110
#define WCHNET_ROOMPLAYERREADY			WCHNETID+111
#define WCHNET_EYEPOINTS				WCHNETID+112
#define WCHNET_CHECKPLAYERDELAYTIME		WCHNETID+113
#define WCHNET_STOPMYROLES				WCHNETID+114
#define WCHNET_DBSAVE					WCHNETID+115
#define WCHNET_SERVERLISTUPDATE			WCHNETID+116
#define WCHNET_SERVERLISTQUERY			WCHNETID+117
#define WCHNET_CONNECTSERVERINIT		WCHNETID+118
#define WCHNET_CONNECTSERVERUPDATE		WCHNETID+119
#define WCHNET_CONNECTSERVERQUERY		WCHNETID+120
#define WCHNET_ROOMEVENT				WCHNETID+121
#define WCHNET_KICKOFFPLAYER			WCHNETID+122
#define WCHNET_SETGAMELOGPATH			WCHNETID+123
#define WCHNET_THROWSERVER				WCHNETID+124
#define WCHNET_PAUSEROOMGAME			WCHNETID+125
#define WCHNET_SERVERNEEDRESTART		WCHNETID+126
#define WCHNET_CLIENTRECONNECT			WCHNETID+127
#define WCHNET_PLAYERDATAEVENTZIP		WCHNETID+128
#define WCHNET_PLAYERDATASCENEEVENTZIP     WCHNETID+129
#define WCHNET_SCENESERVERINITEND		WCHNETID+130
#define WCHNET_QUERYGAMEINFO		WCHNETID+131
#define WCHNET_SHUTDOWNSERVER		WCHNETID+132
//#define WCHNET_GAMESERVERCREATEROOM		WCHNETID+116

namespace CRNetApp{

enum AppNetType
{
	Client = -1,
	ParentServer,
	ParentServerClient_Parent,
	LoginServer,
	LoginConnectServer,
	LoginConnectServerClient_Login,
	ChatServer,
	ChatServerClient_Parent,
	VoiceServer,
	VoiceServerClient_Parent,
	CameraServer,
	CameraServerClient_Parent,
	ChatConnectServer,//x
	ChatConnectServerClient_Chat,//x
	ChatConnectServerClient_Voice,//x
	ChatConnectServerClient_Camera,//x
	DownloadServer,
	DownloadConnectServer,
	DownloadConnectServerClient_Download,
	GameServer,
	GameServerClient_Parent,
	GameConnectServer,
	GameConnectServerClient_Game,
	GameConnectServerClient_Download,
	GameConnectServerClient_Scene,
	GameConnectServerClient_Chat,
	GameConnectServerClient_Voice,
	GameConnectServerClient_Camera,
	SceneServer,
	SceneServerClient_Game,
	//SceneConnectServer,//x
	//SceneConnectServerClient_Scene,//x
	GameClient_Login,
	GameClient_Chat,//x
	GameClient_Download,
	GameClient_Game,
	//GameClient_Game,//x
	MonitorServer,
	MonitorServerClient_Monitor,
	MonitorClient_Monitor,
	DBServer,
	GameServerClient_DB,
	GameClient_CrossGame,//与跨服游戏服务器连接
	Net_User,
};
#define DBPingInterval 30
#define DBTimeOut	90
//DBMapVec
static const int GameDB = 0;//instanceitem,itemchild,npc(0)   gamedb 0是游戏数据库，用户数据库从1开始
static const int ChatDB = 1;

//DBVec
static const int ResDB = 0;//res,abstractitem,abstractmetier,scene
static const int GameGlobalDB = 1;//sale
//static const int InstanceDB = 1;//instanceitem,itemchild,npc,scene
}

#endif