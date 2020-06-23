/**********************************************************************
*
*	文件:	   appMsg.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPMSG_H
#define JXJ_APPMSG_H

#define WCH_JXJMSG							WCH_USER+20000
#define WCH_JXJGameServerUpdate				WCH_JXJMSG+1
#define WCH_JXJBuildingLevelUpCheck			WCH_JXJMSG+2
#define WCH_JXJRecvBuildingLevelUp			WCH_JXJMSG+3
#define WCH_JXJBuildingLevelChange			WCH_JXJMSG+4
#define WCH_JXJTechLevelUpCheck				WCH_JXJMSG+5
#define WCH_JXJRecvTechLevelUp				WCH_JXJMSG+6
#define WCH_JXJRecvOnlineReward				WCH_JXJMSG+7
#define WCH_JXJRecruitCheck					WCH_JXJMSG+8
#define WCH_JXJRecvRecruit					WCH_JXJMSG+9
#define WCH_JXJRecvFormation				WCH_JXJMSG+10
#define WCH_JXJRecvRevocation				WCH_JXJMSG+11
#define WCH_JXJBuyCD						WCH_JXJMSG+12
#define WCH_JXJRecvBuyCD					WCH_JXJMSG+13
#define WCH_JXJRecvCDHint					WCH_JXJMSG+14
#define WCH_JXJBuyTimers					WCH_JXJMSG+15
#define WCH_JXJRecvBuyTimers				WCH_JXJMSG+16
#define WCH_JXJRecvBattleRMousePick			WCH_JXJMSG+17
#define WCH_JXJRecvCreateTeam				WCH_JXJMSG+18
#define WCH_JXJRecvQueryTeam				WCH_JXJMSG+19
#define WCH_JXJUpdateTeamList				WCH_JXJMSG+20
//#define WCH_JXJRecvHallJiangka				WCH_JXJMSG+21
//#define WCH_JXJRecvPrivateJiangka			WCH_JXJMSG+22
#define WCH_JXJRecvZeroRefresh				WCH_JXJMSG+23
#define WCH_JXJRecvLevy						WCH_JXJMSG+24
//#define WCH_JXJRecvImpress					WCH_JXJMSG+25
//#define WCH_JXJRecvHallRefresh				WCH_JXJMSG+26
//#define WCH_JXJRecvPrivateRefresh			WCH_JXJMSG+27
#define WCH_JXJRecvExploitRanking			WCH_JXJMSG+28
#define WCH_JXJSaleCountValidCheck   		WCH_JXJMSG+29
#define WCH_JXJRecvSale						WCH_JXJMSG+30
#define WCH_JXJRecvOnMarketMySalePage		WCH_JXJMSG+31//WCH_JXJRecvRefreshMySale
#define WCH_JXJRecvCancelSale				WCH_JXJMSG+32
#define WCH_JXJRecvSaleListTimer			WCH_JXJMSG+33
#define WCH_JXJRecvOnMarketPlayerSalePage   WCH_JXJMSG+34//WCH_JXJRecvQueryPlayerSale
#define WCH_JXJRecvPlayerSaleRecords		WCH_JXJMSG+35
#define WCH_JXJRecvBuyPlayerSale			WCH_JXJMSG+36
#define WCH_JXJRecvGoldBuy					WCH_JXJMSG+37
#define WCH_JXJRecvCopperSale				WCH_JXJMSG+38
#define WCH_JXJRecvMakeTroopsEquip			WCH_JXJMSG+39
#define WCH_JXJRecvInvite					WCH_JXJMSG+40
#define WCH_JXJClientRecvInvite				WCH_JXJMSG+41
#define WCH_JXJAcceptInvite					WCH_JXJMSG+42
#define WCH_JXJRefuseInvite					WCH_JXJMSG+43
#define WCH_JXJRecvAcceptInvite				WCH_JXJMSG+44
#define WCH_JXJRecvRefuseInvite				WCH_JXJMSG+45
#define WCH_JXJInvalidRequest				WCH_JXJMSG+46
#define WCH_JXJClientRecvRefuseInvite		WCH_JXJMSG+47
#define WCH_JXJRecvLeaveTeam				WCH_JXJMSG+48
#define WCH_JXJClientRecvLeaveTeam			WCH_JXJMSG+49
#define WCH_JXJRecvBirthPoint				WCH_JXJMSG+50
#define WCH_JXJRecvSetRoleFormationInfo		WCH_JXJMSG+51
#define WCH_JXJGetBingFa					WCH_JXJMSG+52
#define WCH_JXJGetZhenFa					WCH_JXJMSG+53
#define WCH_JXJGetDongCha					WCH_JXJMSG+54
#define WCH_JXJGetTongShuai					WCH_JXJMSG+55
#define WCH_JXJClientRecvExitBattleDeque	WCH_JXJMSG+56
#define WCH_JXJServerMaintenance			WCH_JXJMSG+57
#define WCH_JXJRecvFubenBirthPoint			WCH_JXJMSG+58
#define WCH_JXJRecvJoinTeam					WCH_JXJMSG+59
#define WCH_JXJRecvNewTeamMember			WCH_JXJMSG+60
#define WCH_JXJRecvTeamEnterRoom			WCH_JXJMSG+61
#define WCH_JXJRecvSceneDroped				WCH_JXJMSG+62
#define WCH_JXJRecvEnterBattleDeque			WCH_JXJMSG+63
#define WCH_JXJRecvEnterBattle				WCH_JXJMSG+64
#define WCH_JXJRecvBattleDequeWait			WCH_JXJMSG+65
#define WCH_JXJRecvExitBattleDeque			WCH_JXJMSG+66
#define WCH_JXJRecvRefuseEnterBattle		WCH_JXJMSG+67
#define WCH_JXJRecvAgreeEnterBattle			WCH_JXJMSG+68
#define WCH_JXJRecvBattleRemoved			WCH_JXJMSG+69
#define WCH_JXJClientRecvBattleRemoved		WCH_JXJMSG+70
#define WCH_JXJRecvBattleSettle				WCH_JXJMSG+71
#define WCH_JXJRecvCheckRoles				WCH_JXJMSG+72
#define WCH_JXJRecvCollideState				WCH_JXJMSG+73
#define WCH_JXJRecvFubenSettle				WCH_JXJMSG+74
#define WCH_JXJRecvFubenID					WCH_JXJMSG+75
#define WCH_JXJRecvSaveFubenProgress		WCH_JXJMSG+76
#define WCH_JXJSetInborn					WCH_JXJMSG+77
#define WCH_JXJGainGiftsPackKey				WCH_JXJMSG+78
#define WCH_JXJGetLeaderPower				WCH_JXJMSG+79
#define WCH_JXJGetResProductPercent			WCH_JXJMSG+80
//#define WCH_JXJRecvBuyJunlingCount			WCH_JXJMSG+81
#define WCH_JXJRecvJunling					WCH_JXJMSG+82
#define WCH_JXJRecvUseBagItem               WCH_JXJMSG+83
#define WCH_JXJUseBagItem					WCH_JXJMSG+84
#define WCH_JXJRecvOpenTreasureChest		WCH_JXJMSG+85
#define WCH_JXJRecvUseResPack				WCH_JXJMSG+86
#define WCH_JXJRecvUseTroopsEquipPack		WCH_JXJMSG+87
#define WCH_JXJGetRecruitSpeed				WCH_JXJMSG+88
#define WCH_JXJGetRecruitConsume			WCH_JXJMSG+89
#define WCH_JXJGetBuildingLevelUpSpeed		WCH_JXJMSG+90
#define WCH_JXJGetBuildingLevelUpConsume	WCH_JXJMSG+91
#define WCH_JXJGetRevenueIncrease			WCH_JXJMSG+92
#define WCH_JXJServerActiveTask				WCH_JXJMSG+93
#define WCH_JXJRecvCoppercash				WCH_JXJMSG+94
#define WCH_JXJClientRecvCoppercash			WCH_JXJMSG+95
#define WCH_JXJRecvActiveTaskVec			WCH_JXJMSG+96
#define WCH_JXJReferTask					WCH_JXJMSG+97
#define WCH_JXJAdvanceTask					WCH_JXJMSG+98
#define WCH_JXJAutoReferTask				WCH_JXJMSG+99
#define WCH_JXJRecvBattleClose				WCH_JXJMSG+100
#define WCH_JXJRecvBattleCaptured			WCH_JXJMSG+101
#define WCH_JXJRecvBattleCreateFaild		WCH_JXJMSG+102
#define WCH_JXJRecvLeaveScene				WCH_JXJMSG+103
#define WCH_JXJRecvBattlePunish				WCH_JXJMSG+104
#define WCH_JXJRecvCityInBattleInfo			WCH_JXJMSG+105
#define WCH_JXJRecvChengfangInfo			WCH_JXJMSG+106
#define WCH_JXJRecvGiftgold					WCH_JXJMSG+107
#define WCH_JXJClientRecvGiftgold			WCH_JXJMSG+108
#define WCH_JXJRecvChengchiChange			WCH_JXJMSG+109
#define WCH_JXJClientRecvExperience			WCH_JXJMSG+110
#define WCH_JXJRecvRevert					WCH_JXJMSG+111
#define WCH_JXJReturnRandomName				WCH_JXJMSG+112
#define WCH_JXJBuildingTaskEffect			WCH_JXJMSG+113
#define WCH_JXJRecvUseTroopsPack			WCH_JXJMSG+114
#define WCH_JXJDamageType					WCH_JXJMSG+115
#define WCH_JXJRecvDamageType				WCH_JXJMSG+116
//#define WCH_JXJRecvUseVipCard				WCH_JXJMSG+117
#define WCH_JXJVIPExtend					WCH_JXJMSG+118
#define WCH_JXJRecvAccomplishComplete		WCH_JXJMSG+119
#define WCH_JXJRecvUseProduceCard			WCH_JXJMSG+120
#define WCH_JXJCreateItemTips				WCH_JXJMSG+121
#define WCH_JXJServerCompleteCheck			WCH_JXJMSG+122
#define WCH_JXJRecvTeamClosed				WCH_JXJMSG+123
#define WCH_JXJCreateItemRefineTips			WCH_JXJMSG+124
#define WCH_JXJRecvCreateXumu				WCH_JXJMSG+125
#define WCH_JXJRecvLeaveMarket				WCH_JXJMSG+126
#define WCH_JXJRecvMoneyChange				WCH_JXJMSG+127
#define WCH_JXJRecvCountryContribute		WCH_JXJMSG+128
#define WCH_JXJClientRecvCountryContribute  WCH_JXJMSG+129
#define WCH_JXJLoginFuben					WCH_JXJMSG+130
#define WCH_JXJAgreeEnterBattle				WCH_JXJMSG+131
#define WCH_JXJRecvSearchBattle				WCH_JXJMSG+132
#define WCH_JXJRecvGuiseSync				WCH_JXJMSG+133
#define WCH_JXJRecvSyncFormation			WCH_JXJMSG+134
#define WCH_JXJRecvServerTimer				WCH_JXJMSG+135
#define WCH_JXJConnectWithStationCheck		WCH_JXJMSG+136
#define WCH_JXJRecvVipChange				WCH_JXJMSG+137
#define WCH_JXJResetTask					WCH_JXJMSG+138
#define WCH_JXJRecvHuodongComplete			WCH_JXJMSG+139
#define WCH_JXJRecvBattleCannotCreate		WCH_JXJMSG+140
#define WCH_JXJScenarioTimerEvent			WCH_JXJMSG+141
#define WCH_JXJRecvAddScenarioTimer			WCH_JXJMSG+142
#define WCH_JXJServerRecvPauseRoom			WCH_JXJMSG+143
#define WCH_JXJRecvRemoveScenarioTimer		WCH_JXJMSG+144
#define WCH_JXJRoomDoEvent					WCH_JXJMSG+145
#define WCH_JXJPlayerRelive					WCH_JXJMSG+146
#define WCH_JXJPlayerDataMonitor			WCH_JXJMSG+147
#define WCH_JXJRecvShouchongAward			WCH_JXJMSG+148
#define WCH_JXJRecvCardPieces				WCH_JXJMSG+149
#define WCH_JXJClientRecvCardPieces			WCH_JXJMSG+150
#define WCH_JXJBattleRecruitCheck			WCH_JXJMSG+151
#define WCH_JXJRecvAddArmyFaild				WCH_JXJMSG+152
#define WCH_JXJRecvBuyYueka					WCH_JXJMSG+153
#define WCH_JXJRecvYueKaRewards				WCH_JXJMSG+154
#define WCH_JXJRecvNewHuodongComplete		WCH_JXJMSG+155
#define WCH_JXJRecvNewHuodongActive			WCH_JXJMSG+156
#define WCH_JXJRecvNewHuodongStep			WCH_JXJMSG+157
#define WCH_JXJRecvBattleClosed				WCH_JXJMSG+158
#define WCH_JXJRecvRemoveExtra				WCH_JXJMSG+159
#define WCH_JXJRecvBattleValidateID			WCH_JXJMSG+160
#define WCH_JXJGainCountryCandidateVecT		WCH_JXJMSG+161
#define WCH_JXJRecvChangeName				WCH_JXJMSG+162
#define WCH_JXJRecvActiveCardExtra			WCH_JXJMSG+163
#define WCH_JXJRecvOpinion					WCH_JXJMSG+164
#define WCH_JXJRecvChildCheck				WCH_JXJMSG+165
#define WCH_JXJRecvQiYuTask 				WCH_JXJMSG+166
#define WCH_JXJAddExtraProduce				WCH_JXJMSG+167
#define	WCH_JXJRecvAddExtraProduce			WCH_JXJMSG+168
#define	WCH_JXJAddTroops					WCH_JXJMSG+169
#define	WCH_JXJRecvTroops					WCH_JXJMSG+170
#define	WCH_JXJRecvSetAutoBuilding			WCH_JXJMSG+171
#define	WCH_JXJRecvQueryBattleRankingMap	WCH_JXJMSG+172
#define	WCH_JXJRecvGetBattleRankingReward	WCH_JXJMSG+173
#define	WCH_JXJRecvRefreshForeignCard		WCH_JXJMSG+174
#define	WCH_JXJRecvExchangeForeignCard		WCH_JXJMSG+175
#define WCH_JXJRecvBlackShopRefresh			WCH_JXJMSG+176//黑店刷新
#define WCH_JXJRecvGlodShopBuy				WCH_JXJMSG+177//元宝商店
#define WCH_JXJRecvBlackShopBuy				WCH_JXJMSG+178//黑店
#define WCH_JXJRecvBlackShopRefreshTimer	WCH_JXJMSG+179
#define WCH_JXJRecvSuperEquipZhuling		WCH_JXJMSG+180//神器注灵
#define WCH_JXJRecvResetZhuling				WCH_JXJMSG+181//神器注灵
#define WCH_JXJRecvUseExpPellet				WCH_JXJMSG+182
#define WCH_JXJRecvBuyExpPellet				WCH_JXJMSG+183
#define WCH_JXJRecvKillWorldFubenNpc		WCH_JXJMSG+184//击杀世界副本任意小怪任务
#define WCH_JXJActiveSuperEquip				WCH_JXJMSG+185
#define WCH_JXJRecvActiveSuperEquip			WCH_JXJMSG+186
#define WCH_JXJRecvDistillSuperEquip		WCH_JXJMSG+187
#define WCH_JXJGainSuperEquipLvupPoint		WCH_JXJMSG+188
#define WCH_JXJRecvGainSuperEquipLvupPoint	WCH_JXJMSG+189
#define WCH_JXJRecvSuperEquipCoin			WCH_JXJMSG+190
#define WCH_JXJClientRecvSuperEquipCoin		WCH_JXJMSG+191
#define WCH_JXJRecvSuperEquipShopBuy		WCH_JXJMSG+192
#define WCH_JXJAddTaskProgress				WCH_JXJMSG+193
#define WCH_JXJRecvBuyFuben					WCH_JXJMSG+194
#define WCH_JXJRecvRefineDiscount			WCH_JXJMSG+195
#define WCH_JXJRecvRefineDiscountTimer		WCH_JXJMSG+196
#define WCH_JXJRecvQueryItemRTHP			WCH_JXJMSG+197
///////////////////////////////////////////////////////
#define WCH_JXJRecvUpdateGovPos				WCH_JXJMSG+300
#define WCH_JXJRecvGameChat					WCH_JXJMSG+301
#define WCH_JXJCheckResBuildingLevel		WCH_JXJMSG+302
#define WCH_JXJRecvGameAddFriend			WCH_JXJMSG+303
#define WCH_JXJRecvRequestAddFriend			WCH_JXJMSG+304
#define WCH_JXJRecvGameSendYesToAddFriend	WCH_JXJMSG+305
#define WCH_JXJRecvGameSendNoToAddFriend	WCH_JXJMSG+306
#define WCH_JXJRecvGameDeleteFriend			WCH_JXJMSG+307
#define WCH_JXJRecvGMOrder					WCH_JXJMSG+308
#define WCH_JXJRecvMakeLordEquip			WCH_JXJMSG+309
#define WCH_JXJRecvSaleItemEquip			WCH_JXJMSG+310
#define WCH_JXJRecvGameChatSuccess			WCH_JXJMSG+311
#define WCH_JXJServerNotice					WCH_JXJMSG+312
#define WCH_JXJRankingSave					WCH_JXJMSG+338
#define WCH_JXJHuodongExtUpdate				WCH_JXJMSG+354
#define WCH_JXJRecvTelhuodong				WCH_JXJMSG+355
#define WCH_JXJDeductHuodongUpdate			WCH_JXJMSG+356
//////////////////////////////////////////////////////////////////////////
#define WCH_JXJRecvBattleArmyCount			WCH_JXJMSG+360
#define WCH_JXJRecvSignalsPos				WCH_JXJMSG+361
#define WCH_JXJRecvCampBirthPointChange		WCH_JXJMSG+362			
//////////////////////////////////////////////////////////////////////////
#define WCH_JXJMailRequest					WCH_JXJMSG+312
#define WCH_JXJMailSend						WCH_JXJMSG+313
///////////////////////////////////////////////////////////
#define WCH_JXJRecvSplitItemEquip			WCH_JXJMSG+400
#define WCH_JXJRecvChangeItemPos			WCH_JXJMSG+401
#define WCH_JXJClientRecvAcceptInvite		WCH_JXJMSG+402
#define WCH_JXJRefreshSaleItemTime			WCH_JXJMSG+403
#define WCH_JXJRecvBuyKnapsackBag			WCH_JXJMSG+404
#define WCH_JXJRecvKnapsackBuyItem			WCH_JXJMSG+405
#define WCH_JXJRecvKickMemberFromTeam		WCH_JXJMSG+406
//#define WCH_JXJRecvAddJiangKa				WCH_JXJMSG+407
#define WCH_JXJRecvShowBattleGround			WCH_JXJMSG+408
#define WCH_JXJRecvPutOnEquip				WCH_JXJMSG+409
#define WCH_JXJRecvGetJiangKa				WCH_JXJMSG+410
#define WCH_JXJRecvSystemNews				WCH_JXJMSG+411
#define WCH_JXJRecvSellJiangkaOK			WCH_JXJMSG+412
#define WCH_JXJRecvUpgradeJiangka			WCH_JXJMSG+413
#define WCH_JXJRecvSystemMail				WCH_JXJMSG+414
#define WCH_JXJClientRecvSystemMail			WCH_JXJMSG+415
#define WCH_JXJMailBodyRequest				WCH_JXJMSG+416
#define WCH_JXJCheckOneMailSuccess			WCH_JXJMSG+417
#define WCH_JXJRecvDeleteMail				WCH_JXJMSG+418
#define WCH_JXJDeleteOneMail				WCH_JXJMSG+419
#define WCH_JXJRecvUpdateLordLvInfo			WCH_JXJMSG+420
#define WCH_JXJNewMailRequest				WCH_JXJMSG+421
//#define WCH_JXJRecvvCheckFubenNeed			WCH_JXJMSG+422
#define WCH_JXJRecvDeductJunling			WCH_JXJMSG+423
#define WCH_JXJRecvBuyJunling				WCH_JXJMSG+424
#define WCH_JXJFubenSelectChange			WCH_JXJMSG+425
#define WCH_JXJRecvUpdateTechLv				WCH_JXJMSG+426
#define WCH_JXJRecvArrangeKnapsackList		WCH_JXJMSG+427
#define WCH_JXJGetTechResProductPercent		WCH_JXJMSG+428
#define WCH_JXJRecvAppointOneJiangka		WCH_JXJMSG+429
#define WCH_JXJRecvAppointValidTime			WCH_JXJMSG+430
#define WCH_JXJRecvUndoAppointCard			WCH_JXJMSG+431
#define WCH_JXJRecvRollCards				WCH_JXJMSG+432
#define WCH_JXJRecvPickCards				WCH_JXJMSG+433
#define WCH_JXJShowluckyCards				WCH_JXJMSG+434
#define WCH_JXJUIKnowCards					WCH_JXJMSG+435
#define WCH_JXJRecvKnowCards				WCH_JXJMSG+436
#define WCH_JXJRecvSaleStateChange			WCH_JXJMSG+437//WCH_JXJRecvSaleCancelSuccess
#define WCH_JXJNewHandTask4					WCH_JXJMSG+438
#define WCH_JXJRecvJiusiCostCountRefresh	WCH_JXJMSG+439
#define WCH_JXJNewHandTask10				WCH_JXJMSG+440
//#define WCH_JXJCheckLordEquipSuit			WCH_JXJMSG+441
#define WCH_JXJNewHandTask16				WCH_JXJMSG+442
#define WCH_JXJNewHandTask					WCH_JXJMSG+443
#define WCH_JXJRecvUpdateFriendData			WCH_JXJMSG+444
#define WCH_JXJDeductLijinGold				WCH_JXJMSG+445
#define WCH_JXJRecvDeductLijinGold			WCH_JXJMSG+446
#define WCH_JXJRecvOnBattleUI				WCH_JXJMSG+447
#define WCH_JXJRecvLeaveBattleUI			WCH_JXJMSG+448
#define WCH_JXJRecvTakeOffLordEquip			WCH_JXJMSG+449
#define WCH_JXJRecvOnChengfangUI			WCH_JXJMSG+450
#define WCH_JXJRecvLeaveChengfangUI			WCH_JXJMSG+451
#define WCH_JXJRecvClearPunishTime			WCH_JXJMSG+452
#define WCH_JXJGetFukuLimitAdd				WCH_JXJMSG+453
#define WCH_JXJRecvCheckResBuildingLv		WCH_JXJMSG+454
#define WCH_JXJRecvFubenRushStart			WCH_JXJMSG+455
#define WCH_JXJRecvFubenRushPause			WCH_JXJMSG+456
#define WCH_JXJRecvFubenRushContinue		WCH_JXJMSG+457
#define WCH_JXJRecvFubenRushEnd				WCH_JXJMSG+458
#define WCH_JXJRecvFubenRushInfo			WCH_JXJMSG+459
#define WCH_JXJUIShowTipsCanvas				WCH_JXJMSG+460
#define WCH_JXJRecvCreateGroup				WCH_JXJMSG+461
#define WCH_JXJRecvQueryGroupList			WCH_JXJMSG+462
#define WCH_JXJCheckBagEmptyBag				WCH_JXJMSG+463
#define WCH_JXJClientRecvGroupList			WCH_JXJMSG+464
#define WCH_JXJRecvApplyJionGroup			WCH_JXJMSG+465
#define WCH_JXJRecvQueryGroupApplyList		WCH_JXJMSG+466
#define WCH_JXJRecvAgreeGroupApply			WCH_JXJMSG+467
#define WCH_JXJClientJoinGroup				WCH_JXJMSG+468
#define WCH_JXJClientLeaveGroup				WCH_JXJMSG+469
#define WCH_JXJRecvKickMemberOutofGroup		WCH_JXJMSG+470
#define WCH_JXJCheckLijinGold				WCH_JXJMSG+471
#define WCH_JXJRecvQueryMyGroupInfo			WCH_JXJMSG+472
#define WCH_JXJRecvDisAgreeGroupApply		WCH_JXJMSG+473
#define WCH_JXJRecvCancelApplyJionGroup		WCH_JXJMSG+474
#define WCH_JXJCheckInvalidWords			WCH_JXJMSG+475
#define WCH_JXJRecvQueryMyApplyGroupVec		WCH_JXJMSG+476
#define WCH_JXJRecvQueryGroupMemberList		WCH_JXJMSG+477
#define WCH_JXJClientRecvGroupMemberList	WCH_JXJMSG+478
#define WCH_JXJClientRecvMyGroupMemberList	WCH_JXJMSG+479
#define WCH_JXJRecvChangeGroupNotice		WCH_JXJMSG+480
#define WCH_JXJClientDesposeEvent			WCH_JXJMSG+481
#define WCH_JXJRecvIgnoreAllApply			WCH_JXJMSG+482
#define WCH_JXJRecvOnCountryBuildingPage	WCH_JXJMSG+483
#define WCH_JXJClientRecvCountryBuilding	WCH_JXJMSG+484
#define WCH_JXJRecvQueryMyCountryInfo		WCH_JXJMSG+485
#define WCH_JXJRecvBuildMyCountry			WCH_JXJMSG+486
#define WCH_JXJRecvClientNotice				WCH_JXJMSG+487
#define WCH_JXJRecvGovPostRanking			WCH_JXJMSG+488
#define WCH_JXJRecvAccomplishRanking		WCH_JXJMSG+489
#define WCH_JXJRecvTroopsRanking			WCH_JXJMSG+490
#define WCH_JXJRecvCountrycontributeRanking	WCH_JXJMSG+491
#define WCH_JXJRecvPlayerLevelRanking		WCH_JXJMSG+492
#define WCH_JXJPutItemInBag					WCH_JXJMSG+493
#define WCH_JXJProduceCardValidTime			WCH_JXJMSG+494
#define WCH_JXJRecvUseImpressionBuyCard		WCH_JXJMSG+495
#define WCH_JXJRecvPurChaseCard				WCH_JXJMSG+496
#define WCH_JXJCheckJiangkaBagEmpty			WCH_JXJMSG+498
#define WCH_JXJSeverPickLuckyCard			WCH_JXJMSG+499
//////////////////////////////////////////////////////////
//#define WCH_JXJBuildingUpdate				WCH_JXJMSG+501
///////////////////////////////////////////////////////////

#define WCH_JXJRecvAddArmy					WCH_JXJMSG+501
#define WCH_JXJRecvBattleReward				WCH_JXJMSG+502
#define WCH_JXJMouseOnRole					WCH_JXJMSG+503
#define WCH_JXJRecvAddArmySucess			WCH_JXJMSG+504
#define WCH_JXJRecvPlayerRewardInfo			WCH_JXJMSG+505
#define WCH_JXJCreateRewardMail				WCH_JXJMSG+506
#define WCH_JXJRecvExperience				WCH_JXJMSG+507
#define WCH_JXJRecvExploit					WCH_JXJMSG+508 //功勋
#define WCH_JXJRecvAchievement				WCH_JXJMSG+509 //战功
#define WCH_JXJRecvRewardItems				WCH_JXJMSG+510
#define WCH_JXJClientRecvExploit			WCH_JXJMSG+511
#define WCH_JXJClientRecvAchievement		WCH_JXJMSG+512
#define WCH_JXJClientRecvRewardItem			WCH_JXJMSG+513
#define WCH_JXJWriteRewardMail				WCH_JXJMSG+514
#define WCH_JXJCityBelongCheck				WCH_JXJMSG+515
#define WCH_JXJUIUpdateChatMessageList		WCH_JXJMSG+516
#define WCH_JXJRecvFuBenRestart				WCH_JXJMSG+517
#define WCH_JXJFuBenRoomRestart				WCH_JXJMSG+518
#define WCH_JXJRecvSceneItemDrop			WCH_JXJMSG+519
#define WCH_JXJRecvRewardEquips				WCH_JXJMSG+520
#define WCH_JXJGetRTHPPercent				WCH_JXJMSG+521
#define WCH_JXJRecvRoomChatMessage			WCH_JXJMSG+522
#define WCH_JXJRecvMailReward				WCH_JXJMSG+523
#define WCH_JXJRecvMemberReady				WCH_JXJMSG+524
#define WCH_JXJRecvMemberCancelReady		WCH_JXJMSG+525
#define WCH_JXJClientRecvDamage				WCH_JXJMSG+526
#define WCH_JXJRecvAddArmyChangeBirthPt		WCH_JXJMSG+527
#define WCH_JXJFubenDropItems				WCH_JXJMSG+528
#define WCH_JXJClientRecvFubenDrop			WCH_JXJMSG+529
#define WCH_JXJRecvClientChooseFubenDrop	WCH_JXJMSG+530
#define WCH_JXJFubenRushReward				WCH_JXJMSG+531
#define WCH_JXJClientRecvFubenRushReward	WCH_JXJMSG+532
#define WCH_JXJRecvFubenRelive				WCH_JXJMSG+533
#define WCH_JXJInitFubenPlayerReliveCount	WCH_JXJMSG+534
#define WCH_JXJFubenReliveCountHasUsed		WCH_JXJMSG+535
#define WCH_JXJFubenReliveCheck				WCH_JXJMSG+536
#define WCH_JXJClientUserRecvSomeValue		WCH_JXJMSG+537
#define WCH_JXJClientUserRecvSomeItems		WCH_JXJMSG+538
#define WCH_JXJClientRecvFood				WCH_JXJMSG+539
#define WCH_JXJClientRecvWood				WCH_JXJMSG+540
#define WCH_JXJClientRecvIron				WCH_JXJMSG+541
#define WCH_JXJClientRecvHorse				WCH_JXJMSG+542
/*#define WCH_JXJPlayerRecvJunling			WCH_JXJMSG+543*/
#define WCH_JXJRecvOnlineRewardQuery		WCH_JXJMSG+544
#define WCH_JXJClientRecvOnlineRewardTimer	WCH_JXJMSG+545
#define WCH_JXJRecvSystermMail				WCH_JXJMSG+546
#define WCH_JXJSystermSendMailToPlayer		WCH_JXJMSG+547
#define WCH_JXJSystermSendMailToSinglePlayer		WCH_JXJMSG+548
#define WCH_JXJSystermNoticeCreate			WCH_JXJMSG+549
#define WCH_JXJRecvVipUpdate				WCH_JXJMSG+550
#define WCH_JXJRecvVipExpUpdate				WCH_JXJMSG+551
#define WCH_JXJAddVipExp					WCH_JXJMSG+552
#define WCH_JXJCheckItemType				WCH_JXJMSG+553
#define WCH_JXJRecvFood						WCH_JXJMSG+554
#define WCH_JXJRecvWood						WCH_JXJMSG+555
#define WCH_JXJRecvIron						WCH_JXJMSG+556
#define WCH_JXJRecvHorse					WCH_JXJMSG+557
#define WCH_JXJTouchRoleShow				WCH_JXJMSG+558
#define WCH_JXJRecvGetRoomMemberInfo		WCH_JXJMSG+559
#define WCH_JXJRecvRandomRebelCity			WCH_JXJMSG+560
#define WCH_JXJRecvCounterinsurgency		WCH_JXJMSG+561
#define WCH_JXJRandomRebelCity				WCH_JXJMSG+562
#define WCH_JXJRecvStopCounterinsurgency	WCH_JXJMSG+563
#define WCH_JXJRecvInspire					WCH_JXJMSG+564
#define WCH_JXJSetRoleRelivePoint			WCH_JXJMSG+565
#define WCH_JXJQueryRebelTimer				WCH_JXJMSG+566
#define WCH_JXJGetRoleFightingPower			WCH_JXJMSG+567
#define WCH_JXJGetPlayerFightingPower		WCH_JXJMSG+568
#define WCH_JXJRecvPlayerFightPower			WCH_JXJMSG+569
#define WCH_JXJGetTotalLeaderPower			WCH_JXJMSG+570
#define WCH_JXJPlaySignalFx					WCH_JXJMSG+571
#define WCH_JXJExitBattleScene				WCH_JXJMSG+572
//#define WCH_JXJRecvServerTimer				WCH_JXJMSG+573
#define WCH_JXJDestroyWindow				WCH_JXJMSG+574
#define WCH_JXJRecvQueryBirthPoint			WCH_JXJMSG+575
#define WCH_JXJPutInChatRecordDeque			WCH_JXJMSG+576
#define WCH_JXJQueryMailShow				WCH_JXJMSG+577
#define WCH_JXJRecvMail						WCH_JXJMSG+578
#define WCH_JXJBattleInfoCenterNoticeCreate			WCH_JXJMSG+579
#define WCH_JXJQueryFubenPersuade			WCH_JXJMSG+579
#define WCH_JXJRecvFubenPersuade			WCH_JXJMSG+580
#define WCH_JXJRecvFubenPersuadeSucess			WCH_JXJMSG+581
#define WCH_JXJCheckCountryLose				WCH_JXJMSG+582
#define WCH_JXJChangeShili					WCH_JXJMSG+583
#define WCH_JXJRecvPlayerChangeShili		WCH_JXJMSG+584
#define WCH_JXJRecvGetLandReward			WCH_JXJMSG+585
#define WCH_JXJLandRewardDropClear			WCH_JXJMSG+586
#define WCH_JXJRecvSendLandRewardToPlayer	WCH_JXJMSG+587
#define WCH_JXJServerCycle					WCH_JXJMSG+588
#define WCH_JXJQueryPlayerGameStore			WCH_JXJMSG+589
#define WCH_JXJRecvPlayerStore				WCH_JXJMSG+590
#define WCH_JXJCheckGameServerCycleStart	WCH_JXJMSG+591
#define WCH_JXJRecvServerCycleStart			WCH_JXJMSG+592
#define WCH_JXJRecvFubenKillInfo			WCH_JXJMSG+593
#define WCH_JXJCreateCountryLoseNotice		WCH_JXJMSG+594
#define WCH_JXJRecvGetPlayerStoreFail		WCH_JXJMSG+595
#define WCH_JXJGetPlayerGameStore			WCH_JXJMSG+596
//#define WCH_JXJRecvPassStart				WCH_JXJMSG+601
//#define WCH_JXJPassTimerQuery				WCH_JXJMSG+602
#define WCH_JXJPassSendingRuntime			WCH_JXJMSG+603
//#define WCH_JXJRecvPassSettle				WCH_JXJMSG+604
#define WCH_JXJPassCanvasUpdate				WCH_JXJMSG+605
#define WCH_JXJChangeChengchiID				WCH_JXJMSG+606	
#define WCH_JXJChangeAllCityShili			WCH_JXJMSG+607
#define WCH_JXJRecvBattleBrocast			WCH_JXJMSG+608
#define WCH_JXJRecvSaveChatChannelSet		WCH_JXJMSG+609
#define WCH_JXJRecvBattleDisable			WCH_JXJMSG+610
#define WCH_JXJRecvTempClientNotice			WCH_JXJMSG+611
#define WCH_JXJChangeCurSelectRole			WCH_JXJMSG+612
#define WCH_JXJRecvQueryBattleReport		WCH_JXJMSG+613
#define WCH_JXJQueryBattleReport			WCH_JXJMSG+614
#define WCH_JXJWidgetAlphaflash				WCH_JXJMSG+615
#define WCH_JXJRecvFubenAddStar				WCH_JXJMSG+616
#define WCH_JXJSkillEffectRange				WCH_JXJMSG+617
#define WCH_JXJSkillEffectRectangle			WCH_JXJMSG+618
//#define WCH_JXJPlayerChengchiIDCheck		WCH_JXJMSG+619
#define WCH_JXJRecvPlayerCycleCount			WCH_JXJMSG+620
////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define WCH_JXJGongFangDZCheck				WCH_JXJMSG+801
#define WCH_JXJNewHandTask5					WCH_JXJMSG+802
#define WCH_JXJNewHandTask12				WCH_JXJMSG+803
#define WCH_JXJNewHandTask11				WCH_JXJMSG+804
#define WCH_JXJNewHandTask17				WCH_JXJMSG+805
//#define WCH_JXJNewHandTask23				WCH_JXJMSG+806
//#define WCH_JXJNewHandTask24				WCH_JXJMSG+807
#define WCH_JXJNewHandTask9					WCH_JXJMSG+808
#define WCH_JXJNewHandTask20				WCH_JXJMSG+809
#define WCH_JXJNewHandTask18				WCH_JXJMSG+810
#define WCH_JXJNewHandTask29				WCH_JXJMSG+811
#define WCH_JXJNewHandTask21				WCH_JXJMSG+812
#define WCH_JXJNewHandTask2101				WCH_JXJMSG+813
#define WCH_JXJNewHandDialogueClick			WCH_JXJMSG+814
#define WCH_JXJAutoAdvanceTask				WCH_JXJMSG+815
#define WCH_JXJRecvFuBenResetInfo			WCH_JXJMSG+816
#define WCH_JXJRecvGiveBuildingTime1		WCH_JXJMSG+817
#define WCH_JXJMouseOnBuildingItemEffect	WCH_JXJMSG+818	//建筑选中状态特效
//#define WCH_JXJUIXuMuDialogue				WCH_JXJMSG+819
#define WCH_JXJEndTalk						WCH_JXJMSG+820
///////////////////////////////////////////////////////////////////////
#define WCH_JXJUIMailTip					WCH_JXJMSG+900
//////////////////////////////////////////////////////////////
#define WCH_JXJRecvManorLvRanking			WCH_JXJMSG+1000
#define WCH_JXJRecvQueryCountryInfo			WCH_JXJMSG+1001
#define WCH_JXJRecvConsortialvRanking		WCH_JXJMSG+1002
#define WCH_JXJRecvConsortiaMemberRanking	WCH_JXJMSG+1003
#define WCH_JXJRecvPurChaseGovPostItem		WCH_JXJMSG+1004
#define WCH_JXJRecvSmelting					WCH_JXJMSG+1005
#define WCH_JXJRecvDeductCopperCash			WCH_JXJMSG+1006
#define WCH_JXJDeductCopperCash				WCH_JXJMSG+1007
#define WCH_JXJCheckCopperCash				WCH_JXJMSG+1008
#define WCH_JXJRecvAttachStore				WCH_JXJMSG+1009
#define WCH_JXJRecvEquipRefine				WCH_JXJMSG+1010
#define WCH_JXJRecvRefineFreeCountProduce	WCH_JXJMSG+1011
#define WCH_JXJRecvSaveEquipRefine			WCH_JXJMSG+1012
#define WCH_JXJRecvOneKeyBuildingLevelUp	WCH_JXJMSG+1013
#define WCH_JXJRecvChangeGoldCostHind		WCH_JXJMSG+1014
#define WCH_JXJRecvBuyNewBuilding			WCH_JXJMSG+1015
#define WCH_JXJRecvBuyLordEquipMaterials	WCH_JXJMSG+1016
#define WCH_JXJRecvUpgradeVipAffairs		WCH_JXJMSG+1017
#define WCH_JXJCheckMarketLimitBuy			WCH_JXJMSG+1018
#define WCH_JXJRecvCheckOnlineCount			WCH_JXJMSG+1019
#define WCH_JXJRecvPutItemInBag				WCH_JXJMSG+1020
#define WCH_JXJRecvCheckPlayerData			WCH_JXJMSG+1021
#define WCH_JXJFormationGetLeaderPower		WCH_JXJMSG+1022
#define WCH_JXJRecvChangePlayerData			WCH_JXJMSG+1023
#define WCH_JXJChangePlayerData				WCH_JXJMSG+1024
#define WCH_JXJCheckPlayerData				WCH_JXJMSG+1025
#define WCH_JXJClientRecvDeductItems		WCH_JXJMSG+1026
#define WCH_JXJRecvDeductItems				WCH_JXJMSG+1027
#define WCH_JXJRecvRollCardFreeCountProduce	WCH_JXJMSG+1028
#define WCH_JXJRecvChangeJiusiSet			WCH_JXJMSG+1029
#define WCH_JXJRecvUpgradeTroopsTech		WCH_JXJMSG+1030
#define WCH_JXJRecvAddBlackName				WCH_JXJMSG+1031
#define WCH_JXJRecvDeleteBlackName			WCH_JXJMSG+1032
#define WCH_JXJRecvTechDevelopUpdate		WCH_JXJMSG+1033
#define WCH_JXJRecvClearTechDevelopTime		WCH_JXJMSG+1034
#define WCH_JXJRecvBuySavedFormationPage	WCH_JXJMSG+1035
#define WCH_JXJCalculatePower				WCH_JXJMSG+1036//计算战斗力
#define WCH_JXJGMGetPlayerData				WCH_JXJMSG+1037//从数据库获取玩家数据
#define WCH_JXJRecvGMChangePlayerData		WCH_JXJMSG+1038//改变玩家数据
#define WCH_JXJRecvNewguideComplete			WCH_JXJMSG+1039//完成新手引导
#define WCH_JXJGetConsortiaName				WCH_JXJMSG+1040//获取玩家的军团名，传入参数playerid
#define WCH_JXJRecvMarketResexchange		WCH_JXJMSG+1041//市集交换资源
#define WCH_JXJRecvHuoyueduUpdate			WCH_JXJMSG+1042//活跃度完成
#define WCH_JXJRecvHuoyuedu					WCH_JXJMSG+1043//活跃度
#define WCH_JXJRecvHuoyueduReward			WCH_JXJMSG+1044//活跃度奖励
#define WCH_JXJRecvLingquFenglu				WCH_JXJMSG+1045//领取俸禄

//////////////////////////////////////////////////////
#define WCH_JXJNotInSightMsgData			WCH_JXJMSG+1100
#define WCH_JXJVIPRevenues					WCH_JXJMSG+1101
#define WCH_JXJVIPSacrifice					WCH_JXJMSG+1102
#define WCH_JXJVIPRecruit					WCH_JXJMSG+1103
#define WCH_JXJNotInSight2MsgData			WCH_JXJMSG+1104

//////////////////////////////////////////////////////
#define WCH_JXJRecvChangeGroupLeader			WCH_JXJMSG+1200		// 转让军团长
#define WCH_JXJRecvDismissGroup					WCH_JXJMSG+1201		// 解散军团
#define WCH_JXJRecvCancelDismissGroup			WCH_JXJMSG+1202		// 取消解散军团
#define WCH_JXJRecvAddGroupMember				WCH_JXJMSG+1203		// 主动添加军团成员
#define WCH_JXJRecvInvitedAddGroupMember		WCH_JXJMSG+1204		// 被邀请加入军团成员
#define WCH_JXJRecvAddGroupMemberResult			WCH_JXJMSG+1205		// 接收加军团的结果
#define WCH_JXJRecvChangeGroupName				WCH_JXJMSG+1206		// 军团改名
#define WCH_JXJRecvGroupContribute				WCH_JXJMSG+1207		// 军团捐献
#define WCH_JXJRecvGroupIntoFuBen				WCH_JXJMSG+1208		//  进入军团副本
#define WCH_JXJRecvGroupStartFuBen				WCH_JXJMSG+1209		//  进入军团副本
#define WCH_JXJRecvChangeMyChengChi				WCH_JXJMSG+1210// 
#define WCH_JXJRecvRemoveChengChiGroup			WCH_JXJMSG+1211//	
#define WCH_JXJRecvChengChiFenFeng				WCH_JXJMSG+1212//	
#define	WCH_JXJRecvAllocateEarnings				WCH_JXJMSG+1213
#define WCH_JXJRecvQueryMyCantonGroupMap			WCH_JXJMSG+1214
#define WCH_JXJRecvQueryMyGroupFenFengChengChiMap		WCH_JXJMSG+1215
#define WCH_JXJServerDayZero					WCH_JXJMSG+1216
#define WCH_JXJServerWeekZero					WCH_JXJMSG+1217
#define WCH_JXJServer2SecZero					WCH_JXJMSG+1218
#define WCH_JXJMyGroupIntoFuBen					WCH_JXJMSG+1219
#define WCH_JXJGroupNotExist					WCH_JXJMSG+1220		// 通知客户端军团已经不存在,将从数据库中删除
#define WCH_JXJGroupNotExitUISwitch				WCH_JXJMSG+1221
#define WCH_JXJGroupResetGroupId				WCH_JXJMSG+1222
#define WCH_JXJQueryGroupMemberList				WCH_JXJMSG+1223
#define WCH_JXJQueryGroupApplyerList			WCH_JXJMSG+1224
#define WCH_JXJRecvGetChengChiEarnings			WCH_JXJMSG+1225
#define WCH_JXJDeductResource					WCH_JXJMSG+1226
#define WCH_JXJRecvDeductResource				WCH_JXJMSG+1227
#define WCH_JXJRecvQueryMyCounrtyCantonChengChiINfo		WCH_JXJMSG+1228
#define WCH_JXJRecvQueryMyCounrtyCantonFenFengGroupList	WCH_JXJMSG+1229
#define WCH_JXJRecvCheckLostPlayerCount			WCH_JXJMSG+1230
#define WCH_JXJRecvCheckRemainPlayerCount		WCH_JXJMSG+1231
#define WCH_JXJRecvCheckTotalPlayerCount		WCH_JXJMSG+1232
#define WCH_JXJRecvSkipNewHandIntro				WCH_JXJMSG+1233
//#define WCH_JXJRecvGetShiliPeoPleMap			WCH_JXJMSG+1234
#define WCH_JXJRecvCheckMoneyRecord				WCH_JXJMSG+1235
#define WCH_JXJRecvResetTechNewBtnFlag			WCH_JXJMSG+1236
#define WCH_JXJRecvFubenRushQuickEnd			WCH_JXJMSG+1237
//#define WCH_JXJRecvQueryLoseCountry				WCH_JXJMSG+1238
#define WCH_JXJSelectCharacterPowerOnShow		WCH_JXJMSG+1239
#define WCH_JXJRecvChangeData					WCH_JXJMSG+1240//修改玩家数据
#define WCH_JXJRecvUpdateNewFunction			WCH_JXJMSG+1241//新功能开启
//#define WCH_JXJRecvMainroleZeroRefresh			WCH_JXJMSG+1242//Mainrole零点刷新
#define WCH_JXJRecvUpgradeResAddBuilding		WCH_JXJMSG+1243//资源加成建筑升级
#define WCH_JXJRecvBuyResAddBuildingTime		WCH_JXJMSG+1244//购买资源加成建筑时间
#define WCH_JXJRecvOneKeyResAddCard				WCH_JXJMSG+1245//一键使用资源加成卡
//////////////////////////////////////////////////////
#define WCH_JXJRecvChangeCountryNotice			WCH_JXJMSG+1500//发布国家公告
#define WCH_JXJRecvCountryHistory				WCH_JXJMSG+1501//查询国家历史信息
#define WCH_JXJRecvCountryToupiao				WCH_JXJMSG+1502//查询投票信息
#define WCH_JXJRecvCountryWillBeLeader			WCH_JXJMSG+1503//查询候选人信息
#define WCH_JXJRecvCountryCantonAllocate		WCH_JXJMSG+1504
#define WCH_JXJRecvSelectCountryLeader			WCH_JXJMSG+1505
#define WCH_JXJRecvCountryCanvasApplyStation			WCH_JXJMSG+1506 
#define WCH_JXJRecvQueryPeopleInfoList			WCH_JXJMSG+1507
#define WCH_JXJRecvPeopleInfoList				WCH_JXJMSG+1508
#define WCH_JXJRecvUIConfirmSmallAcc			WCH_JXJMSG+1509
#define WCH_JXJRecvQueryCountryBenefit			WCH_JXJMSG+1510
#define WCH_JXJRecvStartCountryBenefit			WCH_JXJMSG+1511
#define WCH_JXJRecvEnjoyCountryBenefit			WCH_JXJMSG+1512
#define WCH_JXJUIShowGroupCanvas				WCH_JXJMSG+1513
#define WCH_JXJRecvChatMsg						WCH_JXJMSG+2076
#define WCH_JXJPutIntoGroupChatMsgQueue			WCH_JXJMSG+2077
#define WCH_JXJGroupFundsUpgrade					WCH_JXJMSG+2078
#define WCH_JXJRecvQueryCountryPeople				WCH_JXJMSG+2079
#define WCH_JXJClientRecvCreateGroup			WCH_JXJMSG+2080
#define WCH_JXJJunTuanFuBenSettle				WCH_JXJMSG+2081
#define WCH_JXJJunTuanFuBenSettleClient			WCH_JXJMSG+2082
#define WCH_JXJRecvJunTuanFuBenSettleGetReward	WCH_JXJMSG+2083
#define WCH_JXJRecvJunTuanFuBenSettleDouble			WCH_JXJMSG+2084
#define WCH_JXJQueryMyCounrtyCantonChengChiINfo		WCH_JXJMSG+2085
#define WCH_JXJRecvQueryMyCantonInfo				WCH_JXJMSG+2086
#define WCH_JXJQueryMyCantonGroupMap				WCH_JXJMSG+2087
#define WCH_JXJQueryFenFengChengChiMap				WCH_JXJMSG+2088
#define WCH_JXJCheckNameLimit						WCH_JXJMSG+2089
#define WCH_JXJCountryCantonMapRefresh				WCH_JXJMSG+2090
//#define WCH_JXJRecvGroupAllocateChengChi				WCH_JXJMSG+2091
#define WCH_JXJGroupSetSelectedCantonID				WCH_JXJMSG+2092
#define WCH_JXJResetGroupChengChiIdVec				WCH_JXJMSG+2093
#define WCH_JXJMaintainShiliPeopleMap				WCH_JXJMSG+2094
#define WCH_JXJGroupSettleEarnings					WCH_JXJMSG+2095
#define WCH_JXJRecvPingMsg							WCH_JXJMSG+2096
#define WCH_JXJRecvPingRet							WCH_JXJMSG+2097
#define WCH_JXJSYNChengChiToCanton					WCH_JXJMSG+2098
#define WCH_JXJExitJunTuanFuBen						WCH_JXJMSG+2099
#define WCH_JXJRecvClearSkillCD						WCH_JXJMSG+2100
#define WCH_ClearTargetExtra						WCH_JXJMSG+2101
//#define WCH_JXJRecvHuodongChangeStatus				WCH_JXJMSG+2102
#define WCH_JXJRecvHuodongGetAward					WCH_JXJMSG+2103
#define WCH_JXJHongdongSynSetp						WCH_JXJMSG+2104
#define WCH_JXJRecvHuodongSyn						WCH_JXJMSG+2105
//#define WCH_JXJHuodongCanvasInit					WCH_JXJMSG+2106
#define WCH_JXJRecvQueryTabVersion					WCH_JXJMSG+2107
#define WCH_JXJDownloadFiles						WCH_JXJMSG+2108
#define WCH_JXJReloadHuodongTab						WCH_JXJMSG+2109
#define WCH_JXJHuodongTimeCheck						WCH_JXJMSG+2110
#define WCH_JXJReloadHuodongTabData					WCH_JXJMSG+2111
#define WCH_JXJSYNShiliData							WCH_JXJMSG+2112
#define WCH_JXJAddNoChatPlayer						WCH_JXJMSG+2113
#define WCH_JXJGetAwardsByActiveCode				WCH_JXJMSG+2114
//#define WCH_JXJRecvHuodongGetAwardByActiveCode		WCH_JXJMSG+2115
#define WCH_JXJGetAwardsPlaySound					WCH_JXJMSG+2116
#define WCH_JXJUpdateRankingData					WCH_JXJMSG+2117
#define WCH_JXJOnlineSynToRank						WCH_JXJMSG+2118
#define WCH_JXJRecvQueryCityMapChengChiInfo			WCH_JXJMSG+2119
#define WCH_JXJJXJShowScenarioTalk					WCH_JXJMSG+2120
#define WCH_JXJCountMonitorPacket					WCH_JXJMSG+2121
#define WCH_JXJRecvRestartServerCheck				WCH_JXJMSG+2122
#define WCH_JXJRecvCheckServerVersion				WCH_JXJMSG+2123
#define WCH_JXJRecvReduceRTMp						WCH_JXJMSG+2124
#define WCH_JXJCureRTMp								WCH_JXJMSG+2125
#define WCH_JXJGameServerGroupDayZero				WCH_JXJMSG+2126
#define WCH_JXJGameServerGroupWeekZero				WCH_JXJMSG+2127
#define WCH_JXJStatisticUpdate						WCH_JXJMSG+2128
#define WCH_JXJGameServerShiliDayZero				WCH_JXJMSG+2129
#define WCH_JXJQueryMyShiliData						WCH_JXJMSG+2130
#define WCH_JXJReduceRTMp							WCH_JXJMSG+2131
#define WCH_JXJRecvCureMP							WCH_JXJMSG+2132
#define WCH_JXJBuildPlayerDataSaveStreamTab			WCH_JXJMSG+2133
#define WCH_JXJBuildMainRoleDataSaveStreamTab		WCH_JXJMSG+2134
#define WCH_JXJGMOrderCountryVote					WCH_JXJMSG+2135
#define WCH_JXJPutFloatContentIntoDeq				WCH_JXJMSG+2136
#define WCH_JXJRecvFoundCountryGetAward				WCH_JXJMSG+2137
#define WCH_JXJNewerTaskHadAllCompleted				WCH_JXJMSG+2138
#define WCH_JXJGenerateCountryOfficial				WCH_JXJMSG+2139
#define WCH_JXJKictOutGroupMemberByGM				WCH_JXJMSG+2140
#define WCH_JXJOnlineSynToRank						WCH_JXJMSG+2141
#define WCH_JXJRecv7DaysGetAward					WCH_JXJMSG+2142
#define WCH_JXJGetSkillNeedRTMP						WCH_JXJMSG+2143
#define WCH_JXJGetGainRTMP							WCH_JXJMSG+2144

//////////////////////////////////////////////////////
#define WCH_JXJRecvPlayerBuyIcon			        WCH_JXJMSG+1600
#define WCH_JXJPlayerUseIcon				        WCH_JXJMSG+1601
#define WCH_JXJRecvBattleRBuff					    WCH_JXJMSG+2600
#define WCH_JXJRecvCycleTimer					    WCH_JXJMSG+2601
#define WCH_JXJRecvUseJianZhuLing				    WCH_JXJMSG+2602
#define WCH_JXJRecvZhengZhanShopBuy				    WCH_JXJMSG+2603
#define WCH_JXJRecvZhengZanQuan                     WCH_JXJMSG+2604
#define WCH_JXJClientRecvZhengZanQuan               WCH_JXJMSG+2605
#define WCH_JXJRecvGuoZhanJiFen                     WCH_JXJMSG+2606
#define WCH_JXJClientRecvGuoZhanJiFen               WCH_JXJMSG+2607
#define WCH_JXJClientRecvZhanLingCityCount          WCH_JXJMSG+2608
#define WCH_JXJLoginWorldFuBen                      WCH_JXJMSG+2609
#define WCH_JXJRecvWorldFuBenBirthPoint             WCH_JXJMSG+2610
#define WCH_JXJInitWorldFubenPlayerReliveCount      WCH_JXJMSG+2611
#define WCH_JXJRecvFubenShowMonsterInfo				WCH_JXJMSG+2612
#define WCH_JXJRecvTreasonbuffTime				    WCH_JXJMSG+2613
#define WCH_JXJRecvTreasonpunishTime				WCH_JXJMSG+2614
#define WCH_JXJRecvTreasoncoolingTime				WCH_JXJMSG+2615
#define WCH_JXJRecvCancleChangShiLi				    WCH_JXJMSG+2616
#define WCH_JXJRecvClearTreasonBuffTime				WCH_JXJMSG+2617
#define WCH_JXJRecvPlayerInfo				        WCH_JXJMSG+2618
#define WCH_JXJNewRecvBattleBrocast				    WCH_JXJMSG+2619
#define WCH_JXJBattleOpenCenterNoticeCreate	        WCH_JXJMSG+2620
#define WCH_JXJRecvBattleOpenTiShi	                WCH_JXJMSG+2621
#define WCH_JXJFindCityInBattle	                    WCH_JXJMSG+2622
#define WCH_JXJRecvResetDailyTask	                WCH_JXJMSG+2623
#define WCH_JXJServerResetDailyTask	                WCH_JXJMSG+2624
#define WCH_JXJRecvJunZiArriveDesti	                WCH_JXJMSG+2625
#define WCH_JXJGameServerRecvCountryCapital	        WCH_JXJMSG+2626
#define WCH_JXJRecvBattDequeUpdata                  WCH_JXJMSG+2627
#define WCH_JXJServerBattDequeUpdata                WCH_JXJMSG+2628
#define WCH_JXJRecvOpenTimer                        WCH_JXJMSG+2629
#define WCH_JXJRecvWorldFuBenData                   WCH_JXJMSG+2630
#define WCH_JXJRecvWorldFuBenRankAward              WCH_JXJMSG+2631
#define WCH_JXJQueryWorldFuBenData                  WCH_JXJMSG+2632
#define WCH_JXJWorldFuBenStart                      WCH_JXJMSG+2633
#define WCH_JXJDirectGetAwardWindowPop              WCH_JXJMSG+2634
#define WCH_JXJButtonGetJiFenAward                  WCH_JXJMSG+2635
#define WCH_JXJHuoDongStartClearData                WCH_JXJMSG+2636
#define WCH_JXJWorldExecutePraise                   WCH_JXJMSG+2637
#define WCH_JXJWorldExecuteShow                     WCH_JXJMSG+2638
#define WCH_JXJWorldFuBenClientBrocast              WCH_JXJMSG+2639
#define WCH_JXJWorldEventRemainTime                 WCH_JXJMSG+2640
#define WCH_JXJGuoZhanAchievement                   WCH_JXJMSG+2641
#define WCH_JXJPassWorldFuben                       WCH_JXJMSG+2642
#define WCH_JXJContinueFinishHuYueDu                WCH_JXJMSG+2643
#define WCH_JXJZiYuanCheArriveAddPoint              WCH_JXJMSG+2644
#define WCH_JXJRecvAchieveShopBuy                   WCH_JXJMSG+2645
#define WCH_JXJRecvAchievePoint                     WCH_JXJMSG+2646
#define WCH_JXJClientRecvAchievePoint               WCH_JXJMSG+2647
#define WCH_JXJDisposeNewHandlGuoZhan               WCH_JXJMSG+2648
#define WCH_JXJClientDisposeNewHandlGuoZhan         WCH_JXJMSG+2649
#define WCH_JXJQueryCountryStrength                 WCH_JXJMSG+2650
#define WCH_JXJRecvUseLunHuiDaoJU				    WCH_JXJMSG+2651
#define WCH_JXJRecvQuChuLunHuiThing				    WCH_JXJMSG+2652
#define WCH_JXJPlayerGetViplvReward				    WCH_JXJMSG+2653




/////////////////////////////////////////////////////////
#define WCH_JXJGetStartedDays						WCH_JXJMSG+2800
#define WCH_JXJElectIcon							WCH_JXJMSG+2801
#define WCH_JXJRecvBattleCheckIdentifyingCode		WCH_JXJMSG+2802
#define WCH_JXJRecvBuyAppointColunms				WCH_JXJMSG+2803
#define WCH_JXJRecvReleaseGuoJiaZhengZhao			WCH_JXJMSG+2804
#define	WCH_JXJRecvReleaseJunTuanZhengZhao			WCH_JXJMSG+2805
#define WCH_JXJShowYingZhengCanvas					WCH_JXJMSG+2806
#define WCH_JXJUIBuyCD								WCH_JXJMSG+2807
#define WCH_JXJYingzhengReward						WCH_JXJMSG+2808
#define WCH_JXJSetGuoZhanVictoryTimes				WCH_JXJMSG+2809
#define WCH_JXJRecvRandomExtraCollide				WCH_JXJMSG+2810
#define WCH_JXJReliveItemsFromTab					WCH_JXJMSG+2811
#define WCH_JXJSyncScenarioVariable					WCH_JXJMSG+2812
#define WCH_JXJConsortiaPromotionUpdate				WCH_JXJMSG+2813
#define	WCH_JXJRecvQueryConsortiaPromotionData		WCH_JXJMSG+2814
#define WCH_JXJRecvGetConsortiaPromotionReward		WCH_JXJMSG+2815
//#define WCH_JXJRecvBackeToBirthPoint				WCH_JXJMSG+2816
#define WCH_JXJRecvHideGameIntroductionIcon			WCH_JXJMSG+2817
#define WCH_JXJRecvCheckFestivalActivityComplete	WCH_JXJMSG+2818
//////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
#define WCH_JXJGetYellowNewbie						WCH_JXJMSG+2900
#define WCH_JXJGetDailyYellowAward					WCH_JXJMSG+2901
#define WCH_JXJGetYellowUpgradeAward				WCH_JXJMSG+2902
#define WCH_JXJRecvBuySavedEquipPage				WCH_JXJMSG+2903
#define WCH_JXJRecvSwitchEquipPage					WCH_JXJMSG+2904
#define WCH_JXJRecvSaveBindFormation				WCH_JXJMSG+2905
#define WCH_JXJCheckPresentSoldier					WCH_JXJMSG+2906//升级送兵符;
#define WCH_JXJRecvPresentSoldier					WCH_JXJMSG+2907//升级送兵符;
#define WCH_JXJCheck7Day							WCH_JXJMSG+2908//检查7日目标;
#define WCH_JXJRecvGet7DayAward						WCH_JXJMSG+2909//7日目标奖励;
/////////////////////////////////////////////////////////
#define WCH_JXJRecvLibaoHuodongGetAward				WCH_JXJMSG+5000
#define WCH_JXJNewGetAwardsByActiveCode				WCH_JXJMSG+5001
#define WCH_JXJRecvCodeHuodongGetAward				WCH_JXJMSG+5002
#define WCH_JXJRecvWeeklySpecialGetAward			WCH_JXJMSG+5003
#define WCH_JXJRecvLeijiChongzhiGetAward			WCH_JXJMSG+5004			
#define WCH_JXJRecvDanbiChongzhiGetAward			WCH_JXJMSG+5005
#define WCH_JXJRecvUpdateZhouKaStatus				WCH_JXJMSG+5006
#define WCH_JXJRecvJXJZhouKaGetAwards				WCH_JXJMSG+5007
#define WCH_JXJCheckMyApplyGroupIDSet				WCH_JXJMSG+5008
#define WCH_JXJClientStartJuntuanFubenNotification	WCH_JXJMSG+5009
#define WCH_JXJRecvAppointedSecondGroupLeader		WCH_JXJMSG+5010
#define WCH_JXJJuntuanCreateRoom					WCH_JXJMSG+5011
#define WCH_JXJRecvGroupLevelUp						WCH_JXJMSG+5012
#define WCH_JXJRecvConfirmdonateTroops				WCH_JXJMSG+5013
#define WCH_JXJRecvConfirmTakeTroops				WCH_JXJMSG+5014
#define WCH_RecvGroupEnfeoffChengChi				WCH_JXJMSG+5015
#define WCH_JXJRecvGroupContribution			    WCH_JXJMSG+5016
#define WCH_JXJClientRecvGroupContribution			WCH_JXJMSG+5017
#define WCH_JXJRecvQueryGroupJiLu					WCH_JXJMSG+5018
#define WCH_JXJRefreshGroupCantonSpecialty			WCH_JXJMSG+5019
#define WCH_JXJRecvQueryCantonSpecialty				WCH_JXJMSG+5020
#define WCH_JXJRecvUpdateCantonSpecialty			WCH_JXJMSG+5021
#define WCH_JXJRecvBuyCantonSpecialty				WCH_JXJMSG+5022
#define WCH_JXJRecvUpdateExploitandTroopnumInPeopleMap	WCH_JXJMSG+5023
#define WCH_JXJRecvConsortiaExploitRanking			WCH_JXJMSG+5024
#define WCH_JXJRecvConsortiaTroopsRanking			WCH_JXJMSG+5025
#define WCH_JXJRecvRecvCiQuGuanZhi					WCH_JXJMSG+5026
#define WCH_JXJRecvGroupDismissNotification			WCH_JXJMSG+5027
#define WCH_JXJUpdate7DayHuoDong			    	WCH_JXJMSG+5028
#define WCH_JXJRecvKaoShangFuUsedTime				WCH_JXJMSG+5029
#define WCH_JXJUseBagItemConfirm					WCH_JXJMSG+5030
#define WCH_JXJRecvZhengZhanTianHuoDongSteps		WCH_JXJMSG+5031
#define WCH_JXJRecvNewHuodongMap					WCH_JXJMSG+5032
#define WCH_JXJRecvMeiRiHaoLiGetAwards				WCH_JXJMSG+5033
#define WCH_JXJRecvCanFenFengGroupMemberList		WCH_JXJMSG+5034
#define WCH_JXJRecvWorldFubenSynchronouscoord		WCH_JXJMSG+5035
#define WCH_JXJRecvNewHuiKuiGetAwards				WCH_JXJMSG+5036


////////////交叉锁注意
//1
//crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
//serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
//crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
//serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
//WCH_JXJSystermNoticeCreate必需在serverbraindata lock里
/////////////////////////////////
//2
//crData *data = m_this->getDataClass();
//data->excHandle(MAKECREPARAM(WCH_LockData,1));
//reward_info->setType(GP_RewardFubenRush);
//m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
//data->excHandle(MAKECREPARAM(WCH_LockData,0));
//WCH_JXJRecvPlayerRewardInfo 必需在playergamedata lock里
///////////////////
//3 玩家锁在serverData锁外面
//playerdatalock->serverdatalock
///////////
//4 CRNetApp::crScene 里的m_roomItemMutex和m_roomRoleMutex在roomdatalock外面
//m_roomItemMutex->roomdatalock
//m_roomRoleMutex->roomdatalock
//////////////////DB beginSession锁
//DB beginSession锁内不要加crServerBrainHandle锁
//如果无法避免，应该是crServerBrainHandle锁->DB beginSession锁
//锁的关系顺序：玩家锁->crServerBrainHandle锁->DB beginSession锁
//如果有shilidatalock，DB beginSession锁->shilidatalock
///////////////////lockPlayerDataMap
//lockPlayerDataMap不要和其它锁交叉
//5 playerdatalock 和mainroledatalock注意
//playerdatalock->mainroledatalock
///////////////////////
//crData *data = m_this->getDataClass();
//data->excHandle(MAKECREPARAM(WCH_LockData,1));
//m_this->getMainRole()->doEvent(WCH_JXJHuodongExtUpdate,MAKECREPARAM(deductgiftgold+needdeductgold,NULL));
//data->excHandle(MAKECREPARAM(WCH_LockData,0));
#endif