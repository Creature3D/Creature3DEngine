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
#ifndef CRNETAPP_MSG_H
#define CRNETAPP_MSG_H 1

#define WCH_ChatStateChange         WCH_USER+1//ÅóÓÑµÄÁÄÌì×´Ì¬¸Ä±ä
#define WCH_RecvedDataStream        WCH_USER+2
#define WCH_ChatDataTransferEnd     WCH_USER+3
#define WCH_BuildDataStream         WCH_USER+4
#define WCH_LockData                WCH_USER+5
#define WCH_RecvingFileStream		WCH_USER+6
#define WCH_RecvedFileStream		WCH_USER+7

#define	WCH_LoginChatReturn         WCH_USER+8
#define WCH_AddChatFriend           WCH_USER+9
#define WCH_FriendChat              WCH_USER+10
#define WCH_GroupChat               WCH_USER+11
#define WCH_ApplyJoinGroup          WCH_USER+12
#define WCH_AcceptJoinGroup         WCH_USER+13
#define WCH_AddGroupMember          WCH_USER+14
#define WCH_QueryOnlinePlayerList   WCH_USER+15
#define WCH_QueryGroupList          WCH_USER+16

#define WCH_ServerRefuse            WCH_USER+17
#define WCH_ChatConnectServerAbnormal      WCH_USER+18
#define WCH_CreateChatGroup         WCH_USER+19
#define WCH_AddChatGroup            WCH_USER+20

#define WCH_GameConnectServerAbnormal      WCH_USER+21
#define WCH_SceneServerAbnormal     WCH_USER+22

#define WCH_RecvGameList            WCH_USER+23
#define WCH_RecvGameRoleList        WCH_USER+24
#define WCH_DownloadedUpdatecfg     WCH_USER+25
#define WCH_LoginGame               WCH_USER+26
#define WCH_LoginGameReturn         WCH_USER+27
#define WCH_RecvSceneInfo           WCH_USER+28
#define	WCH_LoginReturn				WCH_USER+29
//#define WCH_DownloadFileEnd         WCH_USER+29
#define WCH_LoginScene              WCH_USER+30
#define WCH_LoginSceneReturn        WCH_USER+31
#define WCH_DownloadConnectServerAbnormal     WCH_USER+32
#define WCH_RecvRoleInfo            WCH_USER+33
#define WCH_StartGame               WCH_USER+34
#define WCH_PlayerInRangeTest       WCH_USER+35
#define WCH_DownloadItem            WCH_USER+36
#define WCH_LoadItem                WCH_USER+37
#define WCH_ItemInRangeTest         WCH_USER+38
#define WCH_ServerUpdate            WCH_USER+39

#define WCH_NetRoleDead             WCH_USER+40
#define WCH_NetControl              WCH_USER+41
#define WCH_NetMoveToTarget         WCH_USER+42
#define WCH_ItemCoordToNode         WCH_USER+43
#define WCH_RegisterAccountReturn   WCH_USER+44
//#define WCH_GetTarget               WCH_USER+44
//#define WCH_NetControl2             WCH_USER+45
//#define WCH_UploadPlayerCoord       WCH_USER+44
//#define WCH_RecvPlayerCoord         WCH_USER+45
#define WCH_RecvGameRoleListStream  WCH_USER+45
#define WCH_NetMoveToTarget2        WCH_USER+46
#define WCH_NetStopMove             WCH_USER+47
#define WCH_RecvStopMove            WCH_USER+48

//#define WCH_RecvRoleStream1         WCH_USER+49
#define WCH_ServerCheckItemData     WCH_USER+49//SceneServer
#define WCH_NodeInRange             WCH_USER+50
#define WCH_NodeOutRange            WCH_USER+51
#define WCH_RecvItemRTData          WCH_USER+52
#define WCH_DownloadItemChild       WCH_USER+53
#define WCH_LoadItemChild           WCH_USER+54
#define WCH_LoginDownloadReturn     WCH_USER+55
//#define WCH_CheckPlayerData         WCH_USER+55//GameClient
#define WCH_UseItem                 WCH_USER+56
#define WCH_CheckNetData            WCH_USER+57
#define WCH_NodeCollideWithItem     WCH_USER+58
#define WCH_GetDamage               WCH_USER+59
#define WCH_GetDefend               WCH_USER+60
#define WCH_AddDamage               WCH_USER+61
#define WCH_GetEquipAttack          WCH_USER+62
#define WCH_GetEquipDefend          WCH_USER+63
#define WCH_RecvDamage              WCH_USER+64
#define WCH_RecvItemDead            WCH_USER+65
#define WCH_ItemDead                WCH_USER+66
#define WCH_AddExperience           WCH_USER+67
//#define WCH_SetCurrentUseItem       WCH_USER+66
//#define WCH_ChangeTarget            WCH_USER+67
#define WCH_Patrol                  WCH_USER+68
//#define WCH_SwitchToDefault         WCH_USER+69
#define WCH_AddGold                 WCH_USER+69
#define WCH_GetSpeed                WCH_USER+70
#define WCH_GetHP                   WCH_USER+71
#define WCH_GetMP                   WCH_USER+72
#define WCH_GetEquipSpeed           WCH_USER+73
#define WCH_GetEquipHP              WCH_USER+74
#define WCH_GetEquipMP              WCH_USER+75
#define WCH_GetHPRevert             WCH_USER+76
#define WCH_GetMPRevert             WCH_USER+77
#define WCH_GetEquipHPRevert        WCH_USER+78
#define WCH_GetEquipMPRevert        WCH_USER+79
#define WCH_GetSkillCD              WCH_USER+80
#define WCH_TargetExtra             WCH_USER+81
#define WCH_RecvTargetExtra         WCH_USER+82
#define WCH_UserExtra               WCH_USER+83
#define WCH_RecvUserExtra           WCH_USER+84
#define WCH_ResetExtra              WCH_USER+85
#define WCH_DoExtra                 WCH_USER+86
#define WCH_InitData                WCH_USER+87
#define WCH_ItemUse                 WCH_USER+88
#define WCH_GetItemRTData           WCH_USER+89
#define WCH_RecvExperience          WCH_USER+90
#define WCH_RecvGold                WCH_USER+91
#define WCH_LvUp                    WCH_USER+92
#define WCH_RecvLvUp                WCH_USER+93
#define WCH_DeadEvent               WCH_USER+94
#define WCH_GetEquipStrength        WCH_USER+95
#define WCH_GetEquipLv              WCH_USER+96
#define WCH_GetEquipAgility         WCH_USER+97
#define WCH_GetEquipIntellect       WCH_USER+98
#define WCH_GetEquipMagicDamage     WCH_USER+99
#define WCH_GetEquipMagicDefend     WCH_USER+100
#define WCH_GetEquipAttackSpeed     WCH_USER+101
#define WCH_Sink                    WCH_USER+102
#define WCH_ResetBot                WCH_USER+103
#define WCH_ShowStage               WCH_USER+104
#define WCH_RelivePlayer            WCH_USER+105
#define WCH_SetKeyboardMouseMode    WCH_USER+106
#define WCH_EnemyCheck              WCH_USER+107
#define WCH_RequestItemRTData       WCH_USER+108
//#define WCH_GetRoleZ                WCH_USER+109
#define WCH_QueryRoleInfo           WCH_USER+109
#define WCH_Transport               WCH_USER+110
#define WCH_ExitScene               WCH_USER+111
#define WCH_NodeCoordToItem         WCH_USER+112
#define WCH_GetNextLvExp            WCH_USER+113
#define WCH_RecvGetTask             WCH_USER+114
#define WCH_RecvAdvanceTask         WCH_USER+115
#define WCH_RecvReferTask           WCH_USER+116
#define WCH_ExtraHandle				WCH_USER+117
#define WCH_AttackExtraTest			WCH_USER+118
//#define WCH_RecvNetReferTask        WCH_USER+117
//#define WCH_NetReferTaskReturn      WCH_USER+118
//#define WCH_UpdateQuickList         WCH_USER+117
#define WCH_ServerCheckRoleData     WCH_USER+119
#define WCH_CorrectPosition         WCH_USER+120

#define WCH_Touch                   WCH_USER+121
#define WCH_RecvNetPickItem         WCH_USER+122
#define WCH_UnTouch                 WCH_USER+123
//#define WCH_NetPickItemReturn       WCH_USER+123
#define WCH_InsertItemToBackPack    WCH_USER+124
#define WCH_UISaleItem              WCH_USER+125
#define WCH_RecvSaleItem            WCH_USER+126
#define WCH_RemoveItemFromBackPack  WCH_USER+127
#define WCH_RecvDropItem            WCH_USER+128
#define WCH_ShowSaleItem            WCH_USER+129
#define WCH_RecvBuyItem             WCH_USER+130
#define WCH_GetSaleItemMap          WCH_USER+131
#define WCH_RecvSaleItemMap         WCH_USER+132
#define WCH_RecvDownLoadItemChilds  WCH_USER+133
#define WCH_RecvBeginExchange       WCH_USER+134
#define WCH_RecvEndExchange         WCH_USER+135
#define WCH_RecvSaleItemMapChange   WCH_USER+136
#define WCH_BuildSaleItemList       WCH_USER+137
#define WCH_UINotify                WCH_USER+138
#define WCH_RecvRefashSaleMap       WCH_USER+139
#define WCH_RecvLearnSkill          WCH_USER+140
#define WCH_GetPrice                WCH_USER+141
#define WCH_RecvAttrChange          WCH_USER+142
#define WCH_LearnSkillCreater       WCH_USER+143
#define WCH_RecvLearnSkillCreater   WCH_USER+144
#define WCH_RecvEquipOnItem         WCH_USER+145
#define WCH_BuildEquipOnList        WCH_USER+146
#define WCH_BuildSkillList          WCH_USER+147
#define WCH_BuildBackPackList       WCH_USER+148
#define WCH_BuildQuickList          WCH_USER+149
#define WCH_RecvUnEquipItem         WCH_USER+150
#define WCH_RecvEquipQuick          WCH_USER+151
#define WCH_RecvUnEquipQuick        WCH_USER+152
#define WCH_RecvSetQuickKey         WCH_USER+153
#define WCH_GetItemInfo             WCH_USER+154
#define WCH_ComposeEquipCreater     WCH_USER+155
#define WCH_RecvComposeEquipCreater WCH_USER+156
#define WCH_NetUseThing             WCH_USER+157
#define WCH_RecvNetUseThing         WCH_USER+158
#define WCH_ThingUse                WCH_USER+159
#define WCH_CureHP                  WCH_USER+160
#define WCH_CureMP                  WCH_USER+161
#define WCH_QuickKeyPressed         WCH_USER+162
#define WCH_ShowTargetInfo          WCH_USER+163
#define WCH_RecvCureHP              WCH_USER+164
#define WCH_RecvCureMP              WCH_USER+165
#define WCH_RecvGameChat            WCH_USER+166
#define WCH_ChatSystemMsg           WCH_USER+167
#define WCH_RecveDataStream         WCH_USER+168
#define WCH_RecvSceneList           WCH_USER+169
#define WCH_RecvSceneRooms          WCH_USER+170
#define WCH_RecvCreateSceneRoom     WCH_USER+171
#define WCH_RecvEnterSceneRoom      WCH_USER+172
#define WCH_RecvStartRoomGame       WCH_USER+173
#define WCH_RecvEndRoomGame         WCH_USER+174
#define WCH_CheckAndRackEquipOnMesh WCH_USER+175
#define WCH_CheckAndLoadQuickAnim   WCH_USER+176
#define WCH_BorrowCamera            WCH_USER+177
#define WCH_TransportCommand		WCH_USER+178
#define WCH_Start            		WCH_USER+179
#define WCH_RecvFindPathTo			WCH_USER+180
#define WCH_RecvItemState			WCH_USER+181
#define WCH_RecvTarget				WCH_USER+182
#define WCH_ITouch					WCH_USER+183
#define WCH_RecvGuiseState			WCH_USER+184
#define WCH_GetItemCountInBackPack	WCH_USER+185
#define WCH_UIShowTask				WCH_USER+186
#define WCH_PlayerMoveTo			WCH_USER+187
#define WCH_RecvRemoveRoomGame		WCH_USER+188
#define WCH_LearnedSkill			WCH_USER+189
#define WCH_ITransport				WCH_USER+190
#define WCH_RecvTaskAwardItem   	WCH_USER+191
#define WCH_GetItemChildData        WCH_USER+192
#define WCH_RecvItemChildData       WCH_USER+193
#define WCH_GetStorageItemMap       WCH_USER+194
#define WCH_RecvStorageItemMap		WCH_USER+195
#define WCH_BuildStorageItemList	WCH_USER+196
#define WCH_RecvDepositItem			WCH_USER+197
#define WCH_RecvTakeoutItem			WCH_USER+198
#define WCH_InsertItemToStorage		WCH_USER+199
#define WCH_RemoveItemFromStorage	WCH_USER+200
#define WCH_NetControlTank			WCH_USER+201
#define WCH_SetSelectRolePosition	WCH_USER+202
#define WCH_RecvCreateRole			WCH_USER+203
#define WCH_RecvRemoveRole			WCH_USER+204
#define WCH_RecvAttack			    WCH_USER+205
#define WCH_RecvRoomPlayerGameDataUpdate WCH_USER+206
#define WCH_RecvAboutToUseItemID    WCH_USER+207
#define WCH_NetReturnStream			WCH_USER+208
#define WCH_RecvCreateMainRole		WCH_USER+209
#define WCH_GameServerCheckData		WCH_USER+210
#define WCH_PlayerLogin				WCH_USER+211
#define WCH_BuildPlayerLoginStream	WCH_USER+212
#define WCH_CameraFocus				WCH_USER+213
#define WCH_RecvRoleInfo2			WCH_USER+214
#define WCH_ServerLoad				WCH_USER+215
#define WCH_ChangeActState			WCH_USER+216
#define WCH_RecvCreateSight			WCH_USER+217
#define WCH_InPatrolTest			WCH_USER+218
#define WCH_LoadDataAttrFromTab		WCH_USER+219
#define WCH_RecvCloseTarget			WCH_USER+220
#define WCH_PlayerJoinRoom			WCH_USER+221
#define WCH_RecvPathFinding         WCH_USER+222
#define WCH_UseItemCheck			WCH_USER+223
#define WCH_RecvPositionSync		WCH_USER+224
#define WCH_SceneSavePlayerData		WCH_USER+225
#define WCH_MissCritTest			WCH_USER+226
#define WCH_RecvMissCrit			WCH_USER+227
#define WCH_DodgeParryTest			WCH_USER+228
#define WCH_RecvDodgeParry			WCH_USER+229
//#define WCH_CritTest				WCH_USER+226
//#define WCH_ParryTest				WCH_USER+227
//#define WCH_DodgeTest				WCH_USER+228
//#define WCH_RecvCrit				WCH_USER+229
//#define WCH_RecvParry				WCH_USER+230
//#define WCH_RecvDodge				WCH_USER+231
#define WCH_RoomStart				WCH_USER+232
#define WCH_RoomUpdate				WCH_USER+233
#define WCH_RoomEnd					WCH_USER+234
#define WCH_AdvanceRoomProgress		WCH_USER+235
#define WCH_RecvAdvanceRoomProgress	WCH_USER+236
#define WCH_ReturnCamera            WCH_USER+237
#define WCH_ServerDroped            WCH_USER+238
#define WCH_RoomCreate				WCH_USER+240
#define WCH_RoomRemoved				WCH_USER+241
#define WCH_GameServerCreateRoomReturn WCH_USER+242
#define WCH_RoomKillStats			WCH_USER+243
#define WCH_RoomPlayerLeave			WCH_USER+244
#define WCH_RecvServerList			WCH_USER+245
#define WCH_RecvConnectServer		WCH_USER+246
#define WCH_DownloadTaskDone		WCH_USER+247
#define WCH_GetEquipSkillSpeed		WCH_USER+248
#define WCH_TargetNearTest			WCH_USER+249
#define WCH_GetStealth				WCH_USER+250
#define WCH_GetEquipStealth			WCH_USER+251
#define WCH_GetStealthSpy			WCH_USER+252
#define WCH_GetEquipStealthSpy		WCH_USER+253
#define WCH_DynamicCollide			WCH_USER+254
#define WCH_GetGuiseState			WCH_USER+255
#define WCH_GetRadius				WCH_USER+256
#define WCH_DynamicBlock			WCH_USER+257
#define WCH_RoomUpdate2				WCH_USER+258
#define WCH_HaloExtra               WCH_USER+259
//#define WCH_RecvPlayerLeaveScene	WCH_USER+259
//#define WCH_ServerTick				WCH_USER+209
//#define WCH_ClientTick				WCH_USER+210

#define WCH_GetEquipCrit			WCH_USER+260
#define WCH_GetEquipParry			WCH_USER+261
#define WCH_GetEquipDodge			WCH_USER+262
#define WCH_GetEquipBuildAttack     WCH_USER+263
#define WCH_GetEquipHitratio		WCH_USER+264
#define WCH_GetHitratio				WCH_USER+265
#define WCH_GetEquipIgnoreDefend	WCH_USER+266
#define WCH_GetIgnoreDefend			WCH_USER+267
#define WCH_GetEquipIgnoreMagicDefend	WCH_USER+268
//#define WCH_HitratioTest			WCH_USER+268
#define WCH_GetCrit					WCH_USER+269
#define WCH_GetParry				WCH_USER+270
#define WCH_GetDodge				WCH_USER+271
#define WCH_GetEquipAttackDist	    WCH_USER+272
//#define WCH_RecvMiss				WCH_USER+272
//#define WCH_GetAttackRangeExalt		WCH_USER+273
#define WCH_GetSightRange			WCH_USER+274
#define WCH_GetEquipSightRange		WCH_USER+275
#define WCH_GetAttackDist			WCH_USER+276

#define WCH_GetIgnoreDodge				WCH_USER+277//ÎÞÊÓÄ¿±êÉÁ±Ü
#define WCH_GetAttackDamageBack			WCH_USER+278//·´µ¯ÆÕÍ¨¹¥»÷ÉËº¦
#define WCH_GetSkillDamageBack			WCH_USER+279//·´µ¯¼¼ÄÜ¹¥»÷ÉËº¦
//#define WCH_GetAbsorbDamage				WCH_USER+280//»¤¶ÜBufÎüÊÕHpÉËº¦
#define WCH_GetAttackAbsorbHP			WCH_USER+281//ÆÕÍ¨¹¥»÷ÎüÑª
#define WCH_GetSkillAbsorbHP			WCH_USER+282//¼¼ÄÜÎüÑª
//#define WCH_GetSkillIgnore				WCH_USER+283//¼¼ÄÜµÖµ²£¨ÁÖ¿Ï£©

#define WCH_GetEquipIgnoreDodge			WCH_USER+284//ÎÞÊÓÄ¿±êÉÁ±Ü
#define WCH_GetEquipAttackDamageBack	WCH_USER+285//·´µ¯ÆÕÍ¨¹¥»÷ÉËº¦
#define WCH_GetEquipSkillDamageBack		WCH_USER+286//·´µ¯¼¼ÄÜ¹¥»÷ÉËº¦
#define WCH_GetEquipAttackAbsorbHP		WCH_USER+287//ÆÕÍ¨¹¥»÷ÎüÑª
#define WCH_GetEquipSkillAbsorbHP		WCH_USER+288//¼¼ÄÜÎüÑª
//#define WCH_GetEquipSkillIgnore			WCH_USER+290//¼¼ÄÜµÖµ²£¨ÁÖ¿Ï£©

#define WCH_GetRTHP						WCH_USER+291
#define WCH_GetRTMP						WCH_USER+292
#define WCH_GetCritSword				WCH_USER+293//±©»÷½££¬±©»÷ÉËº¦ÌáÊ¾µ½2.5±¶
#define WCH_GetEquipCritSword			WCH_USER+294//±©»÷½££¬±©»÷ÉËº¦ÌáÊ¾µ½2.5±¶
#define WCH_GetSputtering				WCH_USER+295//½¦ÉäÉËº¦
#define WCH_GetEquipSputtering			WCH_USER+296
#define WCH_GetAttackDamage				WCH_USER+297
#define WCH_GetSkillDamage				WCH_USER+298
#define WCH_GetEquipAttackDamage		WCH_USER+299
#define WCH_GetEquipSkillDamage			WCH_USER+300
#define WCH_MainRoleInit				WCH_USER+301
#define WCH_IsTaskCanActive				WCH_USER+302
#define WCH_DisplayTaskGoal				WCH_USER+303
#define WCH_PlayActFx					WCH_USER+304

#define WCH_RecvTouch					WCH_USER+305
#define WCH_ClientRecvTouch				WCH_USER+306
#define WCH_TrackFireMiss				WCH_USER+307
#define WCH_ServerInit					WCH_USER+308
#define WCH_SomeOneTestLogin			WCH_USER+309
#define WCH_GlobalNotice				WCH_USER+310
#define	WCH_MoneyChange					WCH_USER+311
#define	WCH_RecvTriggerSetRoomProgress  WCH_USER+312
#define	WCH_GameLog						WCH_USER+313
#define	WCH_ItemRelive					WCH_USER+314
#define WCH_ServerExtraHandle			WCH_USER+315
#define WCH_ClientExtraHandle			WCH_USER+316
#define WCH_RecvExtraHandle				WCH_USER+317
#define WCH_RecvCheckPlayerGameName		WCH_USER+318
#define WCH_GainBirthPoint				WCH_USER+319
//////StreamType
#define WCH_BuildSaveStream				WCH_USER+320
#define WCH_BuildPlayerStream			WCH_USER+321
#define WCH_BuildNetStream				WCH_USER+322
#define WCH_BuildCreateStream			WCH_USER+323
#define WCH_BuildUpdateStream			WCH_USER+324
#define WCH_BuildSceneSaveStream		WCH_USER+325
#define	WCH_RecvRoomProgressEvent		WCH_USER+326
#define	WCH_TalkEnd						WCH_USER+327
#define WCH_RoomPlayerDead				WCH_USER+328
#define WCH_RoomItemDead				WCH_USER+329
#define WCH_RoomPlayerDamage			WCH_USER+330
#define WCH_RoomItemDamage				WCH_USER+331
#define WCH_RecvScenarioEvent			WCH_USER+332
//.....                                 WCH_USER+330

//#define WCH_AdjustBulletSpeed			WCH_USER+331
//#define WCH_RecvItemUse					WCH_USER+332
#define	WCH_VIPChange					WCH_USER+333
#define	WCH_RecvPlayerGameDataCreateStream		WCH_USER+334
#define WCH_RecvResetTask				WCH_USER+335
#define WCH_EquipExtra                  WCH_USER+336
#define WCH_RecvEquipExtra              WCH_USER+337
#define WCH_SetGamelogPath				WCH_USER+338
#define WCH_RecvPauseRoom				WCH_USER+339
#define WCH_HitExtraTest				WCH_USER+340
#define WCH_SkillHitExtraTest			WCH_USER+341
#define WCH_UseSkillExtraTest			WCH_USER+342
#define WCH_DeadExtraTest				WCH_USER+343
#define WCH_KillExtraTest				WCH_USER+344
#define WCH_ServerNeedRestart			WCH_USER+345
#define WCH_AddPlayer					WCH_USER+346
#define WCH_AdjustBulletSpeed			WCH_USER+347
#define WCH_RecvItemUse					WCH_USER+348
#define WCH_ServerInitSuccess			WCH_USER+349
#define WCH_PlayUseItemFx				WCH_USER+350
#define WCH_SkillExtraTest				WCH_USER+351
#define WCH_RecvTaskProgress			WCH_USER+352
#define WCH_GetAttackDamageIncrease		WCH_USER+353
#define WCH_GetSkillDamageIncrease		WCH_USER+354
#define WCH_GetCureEffectPercent		WCH_USER+355
//WCH_USER16
#define WCH_CutTalk         		WCH_USER16+1
#define WCH_CutScene                WCH_USER16+2
#define WCH_ShowText        		WCH_USER16+3
//H MsgParam
#define WCH_Server                  0
#define WCH_Client                  1

#define UI_GameUI                  0
#define UI_RoleDead                1
#define UI_ExitGame                2
#define UI_Body                    3
#define UI_Quick                   4
#define UI_Map                     5

enum StreamType
{
	SaveStream,
	PlayerStream,
	NetStream,
	CreateStream,
	UpdateStream,
	SceneSaveStream,
};

enum UseItemType
{
	UT_Client,
	UT_Server,
	UT_NetClient,
	UT_NetServer,
};

enum ChatDefine
{
	CD_AddChatFriend,
	CD_ApplyAddChatFriend,
};

enum InitData
{
	ID_None,
	ID_LoginGameServer,
};
//#define WCHExtra_ReduceDefend       2
//#define WCHExtra_IncreaseDefend     3
enum Extra
{
	Ex_None = 0,
	Ex_ReduceDefend = 1,
	Ex_IncreaseDefend = 2,
};
enum HttpDownload
{
	HttpD_None,
	HttpD_FirstSceneFile,
	HttpD_EnterScene,
	HttpD_EnterSceneRoom,
	HttpD_DownLoadItem,
	HttpD_EnterSceneSuccess,
	HttpD_DownLoadItemChild,
	HttpD_Script,
	HttpD_User,
};
#endif