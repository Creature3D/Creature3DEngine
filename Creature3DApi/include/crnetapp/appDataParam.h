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
#ifndef CRNETAPP_ROLEDATAPARAM_H
#define CRNETAPP_ROLEDATAPARAM_H 1

#define WCHDATA_DataStream          0
#define WCHDATA_DataType            1
#define WCHDATA_SightRange          2
#define WCHDATA_ItemState           3
//mouse pick target
#define WCHDATA_TargetType          4
#define WCHDATA_TargetNode          5
#define WCHDATA_TargetID            6
#define WCHDATA_TargetPos           7
#define WCHDATA_FollowDistance      8
#define WCHDATA_Camp                9//阵营

#define WCHDATA_HP                  10
#define WCHDATA_MP                  11
#define WCHDATA_Attack              12
#define WCHDATA_Defend              13
#define WCHDATA_Speed               14
//#define WCHDATA_PlayerID            15
//#define WCHDATA_ItemID              15//如果是玩家取到的是playerid,如果是NPC取到的是itemid
#define WCHDATA_Item                15
#define WCHDATA_AttackSpeed         16
//#define WCHDATA_HasTalkEvent        17
#define WCHDATA_ItemPrice           17
#define WCHDATA_TouchDistance       18
//Character
#define WCHDATA_ViewHeight          19
#define WCHDATA_WalkableHeight      20
#define WCHDATA_TurnToIdleTime      21
#define WCHDATA_BotEffectStr        22
////Skill
//#define WCHDATA_DefaultAttackA        23
//#define WCHDATA_DefaultAttack         24
#define WCHDATA_AttackMap             23
#define WCHDATA_CurrentAttackID       24
#define WCHDATA_MetierSkillsA         25//str格式为:id1,id2,id3...
#define WCHDATA_SkillMap              26
#define WCHDATA_SkillLevel            27
#define WCHDATA_CDTime                28
#define WCHDATA_SkillType             29
#define WCHDATA_QuickKeyVec           30

//#define WCHDATA_EquipVec              31
//#define WCHDATA_ThingVec              32
#define WCHDATA_BackPackMap           31
#define WCHDATA_StorageMap            32
#define WCHDATA_EquipOnMap            33
#define WCHDATA_QuickList              34
#define WCHDATA_ItemCountMap          35
//#define WCHDATA_CurrentUseItem        36
#define WCHDATA_AboutToUseItemID      36
#define WCHDATA_FightMode             37
#define WCHDATA_GroupMemberSet        38

#define WCHDATA_Level                 39
#define WCHDATA_Experience            40
#define WCHDATA_Gold                  41
#define WCHDATA_Strength              42
#define WCHDATA_Agility               43
#define WCHDATA_Intellect             44

#define WCHDATA_MaxAttack             45
#define WCHDATA_MaxSkill              46
#define WCHDATA_MaxBackPack           47
#define WCHDATA_MaxEquipon            48
#define WCHDATA_MaxQuick              49
#define WCHDATA_MaxStorage            50
//#define WCHDATA_PlayerEnmityMap       51
#define WCHDATA_EnmityMap			  51
#define WCHDATA_PatrolPointVec        52
#define WCHDATA_ItemUseRange          53
#define WCHDATA_PatrolIndex           54
#define WCHDATA_RTHP                  55
#define WCHDATA_RTMP                  56
#define WCHDATA_PhysicsDM             57//造成物理伤害
#define WCHDATA_MagicDM               58//造成魔法伤害
#define WCHDATA_NeedMP                59//施法需要魔法值
#define WCHDATA_NeedHP                60//施法需要血量
#define WCHDATA_ExtraData             61
#define WCHDATA_TargetExtraID         62//
#define WCHDATA_UserExtraID           63//
#define WCHDATA_ExtraIDMap            64//
#define WCHDATA_ExtraDefend           65//
#define WCHDATA_ExpToKiller           66//
#define WCHDATA_GoldToKiller          67//
#define WCHDATA_ExtraHP               68
#define WCHDATA_ExtraMP               69
#define WCHDATA_ExtraAttack           70
#define WCHDATA_PatrolLoopMode        71
//#define WCHDATA_ExtraSpeed            71
#define WCHDATA_ExtraAttackSpeed      72
#define WCHDATA_ExtraLevel            73
#define WCHDATA_ExtraStrength         74
#define WCHDATA_ExtraAgility          75
#define WCHDATA_ExtraIntellect        76
//WeaponItem
#define WCHDATA_WeaponActStr          77
#define WCHDATA_WeaponSpeedScale      78
#define WCHDATA_MetierSkillASet       79
#define WCHDATA_GameTaskMap           80
#define WCHDATA_ItemTalk              81
#define WCHDATA_ItemOption            82
#define WCHDATA_CurrentTaskID         83
#define WCHDATA_ItemInfoText          84
#define WCHDATA_StorageCountMap		  85
#define WCHDATA_CreateRoleTab		  86
#define WCHDATA_CurrentRoleTabIndex   87
//#define WCHDATA_GameTaskID            81
//#define WCHDATA_GameTaskName          82
//#define WCHDATA_GameTaskDes           83
//#define WCHDATA_GameTaskSteps         84
//#define WCHDATA_GameTaskExp           85
//#define WCHDATA_GameTaskGold          86
//#define WCHDATA_GameTaskItem          87
#define WCHDATA_ItemDropMap           88
#define WCHDATA_Zoffset               89
#define WCHDATA_BelongPlayerID        90
#define WCHDATA_ItemIcon              91
#define WCHDATA_SaleItemMap           92
#define WCHDATA_NetStream             93
#define WCHDATA_PlayerStream          94
#define WCHDATA_ExchangePlayerSet     95
#define WCHDATA_AttrPoint             96
#define WCHDATA_SkillPoint            97
#define WCHDATA_EquipComposeRequestVec 98
#define WCHDATA_ComposeEquipID        99
#define WCHDATA_UseItemProtectTime    100
//#define WCHDATA_CurUseItemProtectTime 101
#define WCHDATA_LoadedNode			  102
//#define WCHDATA_RoomID                102
//#define WCHDATA_Score                 102
#define WCHDATA_Kill                  103
#define WCHDATA_Dead                  104
#define WCHDATA_MeshFileName          105
#define WCHDATA_AnimFileName          106
#define WCHDATA_CameraBorrowTime      107
#define WCHDATA_CameraBorrowInterval  108
#define WCHDATA_ActStateMap			  109
#define WCHDATA_CurActState			  110
#define WCHDATA_ButtonID              111
#define WCHDATA_TextID                112
#define WCHDATA_CurrentFocusNode      113
#define WCHDATA_CurActDuration		  114
#define WCHDATA_ControllerFlg		  115
#define WCHDATA_GameTaskTab           116
#define WCHDATA_GuiseState            117
#define WCHDATA_SceneFile             118
#define WCHDATA_UIFile                119
#define WCHDATA_BrainScript           120
#define WCHDATA_CurrentSpeed          121
#define WCHDATA_Acceleration          122
#define WCHDATA_Damp                  123
#define WCHDATA_CurrentAngular        124
#define WCHDATA_AngularAccel          125
#define WCHDATA_AngularDamp           126
#define WCHDATA_MaxAngular            127
#define WCHDATA_AimID				  128
#define WCHDATA_UIAimRange			  129
#define WCHDATA_AttackAim			  130
#define WCHDATA_Integral			  131
#define WCHDATA_GameMoney			  132
#define WCHDATA_HalfPhysicsBox			  133//WCHDATA_HalfLengths
#define WCHDATA_FileName			  134
#define WCHDATA_ValidTime			  135
#define WCHDATA_AttachToThis	      136
#define WCHDATA_AttachToNode		  137
#define WCHDATA_AttachBoneName	      138
#define WCHDATA_AttachOffset	      139
#define WCHDATA_AttachScale	          140
#define WCHDATA_AttachRotAngle	      141
///GlobalData
#define WCHDATA_gUnitScale			  142
#define WCHDATA_gMaxLapover			  143
#define WCHDATA_gSaleRate			  144
#define WCHDATA_gRemoveOutRangeTime	  145
#define WCHDATA_gRecycleIDTime		  146
#define WCHDATA_gRecycleStreamIDTime_in	147
#define WCHDATA_gRecycleStreamIDTime_out 148
#define WCHDATA_gMaxgroupmember		  149
#define WCHDATA_gMaxSkillLevelLearn	  150
#define WCHDATA_gMinFollowDistance	  151
#define WCHDATA_gMinItemUseRange      152
#define WCHDATA_gItemVisiableDelay    153
#define WCHDATA_gMaxRecBufSize		  154
#define WCHDATA_gGameChatRecFile	  155
#define WCHDATA_gChatMsgRecFile	      156
#define WCHDATA_gNumGameChatShow	  157
#define WCHDATA_gNumGameChatClear	  158
#define WCHDATA_gMaxInRangePlayer	  159
#define WCHDATA_gMaxInRangeItem		  160
#define WCHDATA_gStreamCacheMaxTime	  161
#define WCHDATA_gMaxDisplayTaskTrace  162
#define WCHDATA_gRemoveItemChildTime  163
#define WCHDATA_gControllerTime		  164
#define WCHDATA_gLayerHeight		  165
#define WCHDATA_gMaxRoleCanJoin		  166
#define WCHDATA_gFirstGameSceneID	  167
#define WCHDATA_gGameID				  168
#define WCHDATA_gGameGlobalTable	  169
#define WCHDATA_gGlobalTables	      170
#define WCHDATA_gMainRoleAItemID	  171
#define WCHDATA_gMainRoleAMetierID	  172
#define WCHDATA_gShowDebugInfo		  173
#define WCHDATA_gEyePointRange		  174
#define WCHDATA_gEyePointHeight		  175
#define WCHDATA_gItemAdjustAngular	  176
#define WCHDATA_gFollowDelta		  177
#define WCHDATA_gPortScopeSqr		  178
#define WCHDATA_gItemVisiableInterval	179
#define WCHDATA_gLoginConnectCount	  180
#define WCHDATA_gDynamicDownloadTabName 181
#define WCHDATA_DynamicDownloadTab	  182
#define WCHDATA_gNewDynamicDownloadTab 183
//#define WCHDATA_gDynamicTryCount	  184
#define WCHDATA_gDynamicTryWait		  184
#define WCHDATA_gWalkHeight			  185
//#define WCHDATA_gGMTable			  186
#define WCHDATA_gEnableRobotTest	  187
#define WCHDATA_gStartDate			  188
#define WCHDATA_gSightRange			  189

#define WCHDATA_ControllerTime		  200
#define WCHDATA_DefaultKey			  201
#define WCHDATA_TargetRoleID		  202
#define WCHDATA_CameraMove  		  203
#define WCHDATA_SceneID				  204
#define WCHDATA_PlayerIconID		  205
#define WCHDATA_MousePosRec			  206
#define WCHDATA_SelectMyRoleVec		  207
#define WCHDATA_PlayerSex			  208
#define WCHDATA_CreateStream		  209
#define WCHDATA_UpdateTimer			  210
#define WCHDATA_UpdateStream		  211
#define WCHDATA_TargetItem			  212
#define WCHDATA_AbstractItemTab		  213
#define WCHDATA_AbstractMetierTab	  214
#define WCHDATA_ResTab				  215
#define WCHDATA_SceneTab			  216
#define WCHDATA_InstranceItemTab	  217
//#define WCHDATA_CommandDlgWidget	  218
//#define WCHDATA_CommandDlgPlayerGameData	  219
#define WCHDATA_CommandDlgParentType  218
#define WCHDATA_CommandDlgParent	  219

#define WCHDATA_SelectItem			  220
#define WCHDATA_SceneSaveStream		  221
//#define WCHDATA_WaypointRoot		  222
//#define WCHDATA_NpcEnmityMap		  222
#define WCHDATA_VisiableTime		  223
#define WCHDATA_IthisValid			  224
#define WCHDATA_CreateNodeName		  225
#define WCHDATA_RoleDataAttrTab		  226
//施法作用范围，0表示对点施法
#define WCHDATA_ItemUseScope		  227
#define WCHDATA_MoveToPos			  228
#define WCHDATA_EyeHeight			  229
#define WCHDATA_SkillDataAttrTab	  230
#define WCHDATA_LastUseTime			  231
#define WCHDATA_NpcDataAttrTab		  232
#define WCHDATA_ItemUseParam		  233
#define WCHDATA_UseItemRecordMap	  234
#define WCHDATA_Duration			  235
#define WCHDATA_RoomProgress		  236
#define WCHDATA_NpcPatrolTab		  237
#define WCHDATA_SkillSpeed			  238
#define WCHDATA_ExtraSkillSpeed		  239
#define WCHDATA_Stealth				  240
#define WCHDATA_ExtraStealth		  241
#define WCHDATA_StealthSpy			  242
#define WCHDATA_ExtraStealthSpy		  243
#define WCHDATA_ArmourType			  244
#define WCHDATA_Damage				  245
#define WCHDATA_DamageType			  246
#define WCHDATA_ExtraGuise			  247
#define WCHDATA_ExtraRadius			  248
#define WCHDATA_GuardRange			  249
#define WCHDATA_CameraRot			  250

#define WCHDATA_EquipDataAttrTab	  251
#define WCHDATA_EquipHP				  252
#define WCHDATA_EquipMP				  253
#define WCHDATA_EquipAttack			  254
#define WCHDATA_EquipDefend			  255
#define WCHDATA_EquipSpeed			  256
#define WCHDATA_EquipStealth		  257
#define WCHDATA_EquipStealthSpy		  258
#define WCHDATA_EquipAttackSpeed	  259
#define WCHDATA_EquipSkillSpeed  	  260
#define WCHDATA_EquipCrit  			  261
#define WCHDATA_EquipParry  		  262
#define WCHDATA_EquipDodge  		  263
#define WCHDATA_EquipBuildAttack  	  264
//#define WCHDATA_EquipBuildDefend  	  265
#define WCHDATA_EquipAttackDist  	  266
#define WCHDATA_EquipHitratio  	      267
#define WCHDATA_EquipIgnoreDefend  	  268
#define	WCHDATA_EquipHPRevert         269
#define	WCHDATA_EquipMPRevert         270
#define WCHDATA_Hitratio 			  271
#define WCHDATA_ExtraHitratioPercent  272
#define WCHDATA_ExtraIgnoreDefend 	  273
#define WCHDATA_Crit				  274
#define WCHDATA_Parry				  275
#define WCHDATA_Dodge				  276
#define WCHDATA_ExtraCrit			  277
#define WCHDATA_ExtraParry			  278
#define WCHDATA_ExtraDodge			  279
#define WCHDATA_IsCrit				  280
#define WCHDATA_IsParry				  281
#define WCHDATA_IsDodge				  282
#define WCHDATA_IsMiss				  283
#define WCHDATA_InbornHPPercent		  284
#define WCHDATA_InbornMPPercent		  285
#define WCHDATA_InbornAttackPercent	  286
#define WCHDATA_InbornDefendPercent	  287
#define WCHDATA_InbornSpeed			  288
#define WCHDATA_InbornAttackSpeedPercent 289
#define WCHDATA_InbornSkillSpeedPercent 290
#define WCHDATA_InbornHitratioPercent 291
#define WCHDATA_InbornBuildAttackPercent 292
#define WCHDATA_ExtraSightRange		  293
#define WCHDATA_EquipSightRange		  294
#define WCHDATA_InbornSightRange	  295
#define WCHDATA_AttachDir			  296

#define WCHDATA_ExtraAttackPercent    297
#define WCHDATA_ExtraSpeedPercent     298
#define WCHDATA_ExtraDefendPercent    299
#define WCHDATA_ExtraBuildAttackPercent 300
#define WCHDATA_ExtraCritPercent	  301
#define WCHDATA_ExtraParryPercent	  302
#define WCHDATA_ExtraDodgePercent	  303
#define WCHDATA_FireItem			  304

#define WCHDATA_ExtraAttackDist		  305
#define WCHDATA_InbornAttackDist	  306
#define WCHDATA_ExtraAttackAbsorbHP   307
#define WCHDATA_ExtraSkillAbsorbHP    308
#define WCHDATA_ExtraAttackDamageBack 309
#define WCHDATA_ExtraSkillDamageBack  310
#define WCHDATA_ExtraRTHP			  311

#define WCHDATA_EquipIgnoreDodge	  312
#define WCHDATA_EquipAttackDamageBack 313
#define WCHDATA_EquipSkillDamageBack  314
#define WCHDATA_EquipAttackAbsorbHP   315
#define WCHDATA_EquipSkillAbsorbHP    316
#define WCHDATA_EquipCritSword		  317
#define WCHDATA_EquipSputtering		  318

#define WCHDATA_InbornStealth		  319
#define WCHDATA_InbornStealthSpy	  320
#define WCHDATA_InbornCrit			  321
#define WCHDATA_InbornParry			  322
#define WCHDATA_InbornDodge			  323
#define WCHDATA_InbornIgnoreDefend	  324
#define WCHDATA_InbornIgnoreDodge	   325
#define WCHDATA_InbornAttackDamageBack 326
#define WCHDATA_InbornSkillDamageBack  327
#define WCHDATA_InbornAttackAbsorbHP   328
#define WCHDATA_InbornSkillAbsorbHP    329
#define WCHDATA_InbornCritPower		   330
#define WCHDATA_InbornSputtering	   331
#define WCHDATA_InbornAttackDamage     332
#define WCHDATA_InbornSkillDamage	   333
#define WCHDATA_ExtraAttackDamage      334
#define WCHDATA_ExtraSkillDamage       335
#define WCHDATA_EquipAttackDamage      336
#define WCHDATA_EquipSkillDamage       337
#define WCHDATA_NpcEquipsTab		   338
#define WCHDATA_NpcSkillSelectTab	   339
#define WCHDATA_InPatrolEnemyMap	   340

#define WCHDATA_ExtraHP_RD					342
#define WCHDATA_ExtraMP_RD					343
#define WCHDATA_ExtraAttack_RD				344
#define WCHDATA_ExtraDefend_RD				345
//#define WCHDATA_ExtraSpeed_RD				346
#define WCHDATA_ExtraStealth_RD				347
#define WCHDATA_ExtraStealthSpy_RD			348
#define WCHDATA_ExtraHitratioPercent_RD		349
#define WCHDATA_ExtraAttackSpeed_RD			350
#define WCHDATA_ExtraSkillSpeed_RD			351
#define WCHDATA_ExtraGuise_RD				352
#define WCHDATA_ExtraRadius_RD				353
#define WCHDATA_ExtraIgnoreDefend_RD		354
#define WCHDATA_ExtraSightRange_RD			355
#define WCHDATA_ExtraCrit_RD				356
#define WCHDATA_ExtraParry_RD				357
#define WCHDATA_ExtraDodge_RD				358
#define WCHDATA_ExtraAttackPercent_RD		359
#define WCHDATA_ExtraSpeedPercent_RD		360
#define WCHDATA_ExtraDefendPercent_RD		361
#define WCHDATA_ExtraBuildAttackPercent_RD	362
#define WCHDATA_ExtraCritPercent_RD			363
#define WCHDATA_ExtraParryPercent_RD		364
#define WCHDATA_ExtraDodgePercent_RD		365
#define WCHDATA_ExtraAttackDist_RD			366
#define WCHDATA_ExtraAttackAbsorbHP_RD		367
#define WCHDATA_ExtraSkillAbsorbHP_RD		368
#define WCHDATA_ExtraAttackDamageBack_RD	369
#define WCHDATA_ExtraSkillDamageBack_RD		370
#define WCHDATA_ExtraRTHP_RD				371
#define WCHDATA_ExtraAttackDamage_RD		372
#define WCHDATA_ExtraSkillDamage_RD			373

#define WCHDATA_RoleInbornAttrTab			374
#define WCHDATA_NpcInbornAttrTab			375

#define WCHDATA_MagicDefend					376
#define WCHDATA_ExtraMagicDefendPercent		377
#define WCHDATA_ExtraMagicDefendPercent_RD	378
#define WCHDATA_InbornMagicDefendPercent	379
#define WCHDATA_EquipMagicDefend			380
#define WCHDATA_IgnoreMagicDefend			381
#define WCHDATA_ExtraIgnoreMagicDefend		382
#define WCHDATA_ExtraIgnoreMagicDefend_RD	383
#define WCHDATA_InbornIgnoreMagicDefend		384
#define WCHDATA_EquipIgnoreMagicDefend		385
#define WCHDATA_DamageCount					386

#define WCHDATA_NoticeTextID				387
#define WCHDATA_NoticeMode					388
#define WCHDATA_NoticeString				389
#define WCHDATA_NoticeParam1				390
#define WCHDATA_NoticeParam2				391
#define WCHDATA_NoticeParam3				392
#define WCHDATA_NoticeParam4				393
#define WCHDATA_NoticeOutPut				394

#define WCHDATA_ExtraIgnoreDodge			400
#define WCHDATA_ExtraCritPower				401
#define WCHDATA_ExtraIgnoreDodge_RD			402
#define WCHDATA_ExtraCritPower_RD			403
#define WCHDATA_Money						404
#define WCHDATA_VipLv						405
#define WCHDATA_VipExp						406
#define WCHDATA_AttackUseParam				407
#define WCHDATA_EquipExtraID				408
#define WCHDATA_ExtraParam					409
#define WCHDATA_WaypointNodeMap				410
#define WCHDATA_ExtraSputtering				411
#define WCHDATA_ExtraSputtering_RD			412
#define WCHDATA_ResetFX						413
#define WCHDATA_ActFX						414
#define WCHDATA_FXItem						415
#define WCHDATA_ChantTime					416
#define WCHDATA_ItemState_rec				417
#define WCHDATA_gWordValueTab				418
#define WCHDATA_Word_AutoSelectTargetMode	419
#define WCHDATA_Word_SelectTargetMode		420
#define WCHDATA_Word_DisplayDamageMode		421

#define WCHDATA_InbornSkillDamageIncrease	   423
#define WCHDATA_ExtraSkillDamageIncrease	   424
#define WCHDATA_ExtraSkillDamageIncrease_RD	   425
#define WCHDATA_InbornAttackDamageIncrease	   426
#define WCHDATA_ExtraAttackDamageIncrease	   427
#define WCHDATA_ExtraAttackDamageIncrease_RD   428

//#define WCHDATA_CureEffectPercent				429
#define WCHDATA_InbornCureEffectPercent			430
#define WCHDATA_ExtraCureEffectPercent			431
#define WCHDATA_ExtraCureEffectPercent_RD		432

#define WCHDATA_OverlapParam				21377
//#define WCHDATA_BaseEnmity					411
//#define WCHDATA_CommandDlgItem				405
//#define WCHDATA_CommandDlgBase				406
//#define WCHDATA_CommandDlgDataObject		407
#include <string>
namespace CRNetApp{
////制作场景的时候注意坐标0,0,0点为不可站立点。
//const float crGlobalHandle::gData()->gUnitScale() = 0.01f;
//const float c_safeDistance = 200;
//const unsigned char c_maxLapover = 99;
//const float c_salerate = 0.25;
//const float c_removeOutRangeTime = 600.0f;//100s
//const float c_recycleIDTime = 620.0f;//110s
//const float c_recycleStreamIDTime_in = 60.0f;//服务器间(局域网内)回收时间
//const float c_recycleStreamIDTime_out = 180.0f;//外网回收时间
//const unsigned char c_maxgroupmember = 16;
//const unsigned char c_maxSkillLevelLearn = 20;//可学到的最高技能等级
//const float c_minFollowDistance = 1.0f;
//const float c_minItemUseRange = 3.0f;//武器的最小攻击距离
//const unsigned char crGlobalHandle::gData()->gItemVisiableDelay() = 200;//ms
//const int c_maxRecBufSize = 524288;//0.5MB
//const std::string c_gameChatRecFile = "rec/gamechat.cfg";
//const std::string c_chatMsgRecFile = "rec/chatmsg.cfg";
//const int c_numGameChatShow = 50;
//const int c_numGameChatClear = 30;
//const unsigned short c_maxInRangePlayer = 100;//32;
//const unsigned short c_maxInRangeItem = 100;
//const float c_streamCacheMaxTime = 600;
//const unsigned char c_maxDisplayTaskTrace = 5;
//const float c_removeItemChildTime = 20.0f;//20s

enum TargetType
{
	Target_None = 0x00,
	Target_Coord = 0x01,//地面 1
	Target_Role = 0x02,
	Target_Npc = 0x04,
	Target_Item = 0x08,
	Target_Self = 0x10,//自身单位 16
	Target_Friend = 0x20,
	Target_Enemy = 0x40,
	Target_StaticNpc = 0x80,//建筑128
	Target_Bot = Target_Role|Target_Npc|Target_Self,//所有玩家和自身单位和NPC 22
	Target_Instance = Target_Bot|Target_Item,//所有玩家和自身单位和和场景道具 30
	Target_All = Target_Coord|Target_Instance,//地面和所有对象 31
	Target_FriendPlayer = Target_Friend | Target_Role | Target_Self,//友军玩家和自身单位 50
	Target_FriendNPC = Target_Friend | Target_Npc,//友军NPC 36
	Target_FriendBot = Target_FriendPlayer | Target_FriendNPC,//友军玩家和友军NPC和自身单位 54
	Target_EnemyPlayer = Target_Enemy | Target_Role,//敌对玩家 66
	Target_EnemyNPC = Target_Enemy | Target_Npc,//敌对NPC 68
	Target_EnemyBot = Target_EnemyPlayer | Target_EnemyNPC,//敌对玩家和敌对NPC 70
	Target_EnemyAll = Target_EnemyBot | Target_StaticNpc,//敌对玩家和敌对NPC和静态建筑 198
	Target_FriendAll = Target_FriendBot | Target_StaticNpc,//友军玩家和友军NPC和自身单位和静态建筑 182
	Target_All_Static = Target_All|Target_StaticNpc,//地面和所有对象和建筑 159
};

enum DataType
{
	DT_Bot,
	DT_NPC,
	DT_MatrixNode,//crMatrixTransform
	DT_Attack,
	DT_Skill,
	DT_Equip,
	DT_Thing,
	DT_Box,//crBoxMatterObject
	DT_Sphere,//crSphereMatterObject
	DT_AttackThing,
	DT_SceneItem,
};

enum ItemState
{
	IS_None,
	IS_UI,
	IS_Stop,
	IS_Dead,
	IS_Relive,
	IS_ItemLoad,
	IS_UseItem,
	IS_Move,
	IS_Patrol,
	IS_MoveToPos,
	IS_MoveToUseSkill,
	IS_CloseTarget,
	IS_CloseTargetUseSkill,
	IS_CloseTargetTouch,
	IS_AttackToPos,
	IS_Defend,//防御，不追击目标
	IS_RecvItemUse,
	//IS_AutoFight,
};

enum GuiseState
{
	GS_Normal			= 0x00000000,
	GS_Static			= 0x00000001,//用于表示静态NPC，如箭塔类		0
	GS_StaticUnVisiable = 0x00000002,//用于表示静态NPC，如箭塔类		1
	GS_StaticNoneBlock	= 0x00000004,//静态item可穿，如门处于打开状态	2
	GS_UnVisiable		= 0x00000008,//不可见，死亡造成					3
	GS_Hide				= 0x00000010,//隐身								4
	GS_Sheep			= 0x00000020,//变羊，可移动，不可攻击,不可被攻击5
	GS_OnHorse			= 0x00000040,//骑马、骑鸟						6
	GS_Metamorphose		= 0x00000080,//变身								7
	GS_NoneBlock		= 0x00000100,//相位，可穿人						8
	GS_Stagnate			= 0x00000200,//凝滞，不可选中，不可移动，不可攻击	9
	GS_Blind			= 0x00000400,//盲								10
	GS_Fetter			= 0x00000800,//束缚/定身，不可移动,可攻击		11
	GS_Swoon			= 0x00001000,//击晕，不可移动，不可攻击			12
	GS_Taunt			= 0x00002000,//嘲讽，不可选中，攻击嘲讽目标		13
	GS_immunoPhysics	= 0x00004000,//物理免疫							14
	GS_immunoSkill		= 0x00008000,//技能免疫							15
	GS_Nihility			= 0x00010000,//虚无状态							16
	GS_Garrison			= 0x00020000,//驻守，不可移动					17
	GS_TrueSight		= 0x00040000,//真视								18
	GS_UnHide			= 0x00080000,//隐身失效							19
	GS_Disarm			= 0x00100000,//缴械,不可攻击					20
	GS_Confusion		= 0x00200000,//混乱,不可选中,不可攻击,四处乱走	21
	GS_Scare			= 0x00400000,//惊吓,不可选中,不可攻击			22
	GS_Silent			= 0x00800000,//沉默,不可使用技能				23
	GS_Stop				= 0x01000000,//停止当前吟唱技能					24
	GS_ExtraAttack		= 0x02000000,//额外攻击状态对应部分需要激活的WCH_AttackExtraTest 25
	GS_ExtraShifting	= 0x04000000,//不转身,不播动作,不可移动,不可攻击 26
	GS_Backstab			= 0x08000000,//背刺			 27
	GS_NoAttack			= 0x10000000,//不可被攻击 不可被别人选中	 28
	GS_NoCure			= 0x20000000,//不可回血						 29
	GS_SuperArmor		= 0x40000000,//霸体,不受状态系技能影响		 30
};

enum SkillType
{
	ST_Initiative,
	ST_Passive,
};

enum FightMode
{
    FM_Peace,//和平
    FM_Group,//组队
	FM_All,//全体
	FM_Defence//自卫
};

enum UseResult
{
	UR_None,
	UR_Succeed,
	UR_InCD,//技能冷却
	UR_MPLack,//魔法不够
	UR_HPLack,//血量不够
	UR_TargetInvalid,//对象无效
	UR_TargetDead,//对象死亡
	UR_UseItemProtect,
	UR_OverRange
};

enum ItemReliveMode
{
	IR_ServerRefash,
	IR_PlayerEvent,
	IR_ItemEvent
};

enum Camp
{
	Evil,
    Nonaligned,
	Just,
};

enum TransportMode
{
	TM_None,
	TM_SwitchLayer,
	TM_SwitchScene,
};

enum ExitCode
{
	EC_ExitGame,
	EC_ExitScene,
	EC_SwitchScene,
	EC_EnterRoom,
};

enum BuyItemReturnCode
{
	BC_Success,
	BC_Error,
	BC_SaleOut,
	BC_MoneyNotEnough,
	BC_BackpackIsFull,
	BC_InsertBackpackError,
};

enum LearnSkillReturnCode
{
	LC_Success,
	LC_Error,
	LC_CantLearn,
	LC_SkillPointLack,
	LC_MoneyNotEnough,
	LC_SkillMapFull,
};

enum EquipItemReturnCode
{
	EC_Success,
	EC_Error,
	EC_DataTypeError,
	EC_EquipOnMapFull,
	EC_NotInBackPack,
    EC_FoundComposeEquip,
	EC_CreateComposeEquip,
	//EquipQuick
	QC_Success,
	QC_Error,
	QC_DataTypeError,
	QC_QuickMapFull,
	QC_NotInBackPack,
	QC_NotInSkillMap,
	QC_NotInAttackMap,
};

enum UnEquipItemReturnCode
{
	UC_Success,
	UC_Error,
	UC_NotInEquip,
	UC_BackpackIsFull,
	UC_InsertBackpackError,
};

enum UnEquipQuickReturnCode
{
	UQ_Success,
	UQ_Error,
	UQ_NotInQuick,
	UQ_BackpackIsFull,
};
enum ReturnCode
{
	RC_Success,
	RC_Error
};

enum DamageOrDefendType
{
    DM_Physics,
	DM_Magic,
};

enum ControllerFlg
{
	CF_Stop = 0,
	CF_Up = 0x01,
	CF_Down = 0x02,
	CF_Left = 0x04,
	CF_Right = 0x08
};

enum MapProtected
{//<0.1为系统保留区域
	MP_NoPass,//0
	MP_NoAutoFight,//0.01
	MP_NoPK,//0.02
	MP_Normal,
};

enum TaskStepType
{
	TS_Zero,
	TS_One,
	TS_Two,
	TS_Three,
	TS_Four,
};

enum SaleState
{
	Sale_Prepare,
	Sale_PrepareOff,
	Sale_OnSale,
	Sale_Saled,
	Sale_Off,
	Sale_Cancel,
	Sale_Canceled,//已取消
	Sale_Receive,
	Sale_Received,//已提走铜钱
};

enum ActState
{
	ACT_None,
	ACT_Idle,
	ACT_Move,
	ACT_Hurt,
	ACT_Dead,
	ACT_Attack,
	ACT_CritAttack,
	ACT_Skill,
	ACT_Parry,
	ACT_Dodge,
	ACT_NearAttack,
	ACT_User,
};
enum ExtraID
{
	Ext_Block	= 1,
	Ext_TrueSight = 2,
	Ext_Relive = 3,
	Ext_User	= 100,
};
enum PatrolLoopMode
{
	PL_NoLoop = 0,
	PL_Loop	= 1,
	PL_Swing = 2,
};
enum CommandDlgParentType
{
	CDP_Widget,
	CDP_PlayerGameData,
	CDP_InstanceItem,
	CDP_Base,
	CDP_DataObject,
};
enum CollideIgnoreMode
{
	IG_None,//所有都做检测
	IG_Me,//忽略自己
	IG_Friends,//忽略队友
};
typedef std::pair<int,std::string> MoneyChangeData;
typedef std::pair<unsigned short,std::string> GameLogData;
typedef std::pair<char,int> VipChangeData;
}
#endif