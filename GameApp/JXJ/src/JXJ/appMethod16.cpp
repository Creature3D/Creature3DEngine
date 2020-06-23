/**********************************************************************
*
*	文件:	   appMethod16.cpp
*
*	描述:	   
*
*	作者:	   袁伟哲
*					
*
**********************************************************************/
#include <JXJ/appMethod16.h>
#include <JXJ/appMethod.h>
#include <crnetapp/appNetLogic.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <shellapi.h>
#include <CRIOManager/crFileUtils.h>
#include <crDataBase/crDataBaseManager.h>
#include <JXJ/appDBQuery.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CREncapsulation/crTableIO.h>
#include<wininet.h>
using namespace JXJ;
using namespace CRCore;
using namespace CRNetApp;
using namespace CRUI;
using namespace CRProducer;
using namespace CRNet;
using namespace CREncapsulation;
using namespace CRDataBase;
using namespace CREncapsulation;

enum TXMarketTaskType
{
	TXTask_Level = 1,
	TXTask_CountryBattle = 2
};

enum _7DayConditionType
{
	_7DayType_Level = 1, //玩家等级
	_7DayType_Technology = 2, //军备科技总等级
	_7DayType_Building = 3,//建筑总等级
	_7DayType_Death = 4,//国战死亡兵力
	_7DayType_Ectype = 5, //副本总星级
	_7DayType_Official = 6, //官职等级
	_7DayType_Card = 7 //将卡组合
};
bool searchCard(JXJItemBagVec *cardbagvec,crTableIO::DataVec& itemRecordVec,int itemidIndex,int qualityIndex, int qualityCondition,SavedFormationInfoVec *formationVec);

//黄钻新手礼包信息
 void getYellowAwardInfoByType(std::vector<CRCore::crVector4i>& awardData,YELLOW_AWARD_TYPE type,int level = 1)
{
	ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
	if (!huodongTab.valid())
		return ;
	crTableIO::StrVec huodongrecord;
	int id = 0;
	switch (type)
	{
	case YELLOW_NEWBIE:
		id = ACTIVITY_YELLOW_ID;
		break;
	case YELLOW_DAILY:
		id = ACTIVITY_YELLOW_ID+level;
		break;
	case YELLOW_YEAR_DAILY:
		id = ACTIVITY_YEAR_YELLOW;
		break;
	case YELLOW_UPGRADE_NORMAL:
		id = ACTIVITY_YELLOW_UPGRADE;
		break;
	case YELLOW_UPGRADE:
		id = ACTIVITY_YELLOW_UPGRADE+1;
		break;
	default:
		break;
	}
	if (huodongTab->queryOneRecord(0,crArgumentParser::appItoa(id),huodongrecord) >= 0)
	{
		CRCore::crVector4i awardVec;
		for (int i = 1 ; i <= MAX_ACITVITY_AWARD; ++i)
		{
			int awardIndex = huodongTab->getTitleIndex("奖励"+crArgumentParser::appItoa(i));
			if(awardIndex >= 0 && !huodongrecord[awardIndex].empty())
			{
				crArgumentParser::appAtoVec(huodongrecord[awardIndex],awardVec);
				awardData.push_back((awardVec));
			}
		}
	}
}

bool checkGetYellowReward(YELLOW_AWARD_TYPE type,crData* pData,int level = 0)
{
	void* param = NULL;
	pData->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
	int yellowAwardInfo = *(int*)param;
	pData->getParam(WCHDATA_JXJVipLv,param);
	unsigned char vipLevel = *(unsigned char *)param;
	int newbieLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJNewbieEndLevel,vipLevel).c_str());
	switch (type)
	{
	case YELLOW_NEWBIE:
		return (yellowAwardInfo & YellowAward_Newbie) == 0;
	case YELLOW_DAILY:
		return (yellowAwardInfo & YellowAward_Daily) == 0;
	case YELLOW_YEAR_DAILY:
		return (yellowAwardInfo & YellowAward_YearDaily) == 0;
	case YELLOW_UPGRADE:
		if(level< newbieLevel)
			return false;
		else
			return (yellowAwardInfo & YellowAward_LEVEL) < level;

	default:
		break;
	}
	return false;
}

char setRewardInfo(const CRCore::crVector4i& award,crImageBoxWidgetNode * imageBox,crStaticTextWidgetNode* input,CRCore::crMultiSwitch* cardcolorsw)
{//返回1表示card
	char rtncode = 0;
	std::string imageName = "";
	int count = 0;
	int itemId = 0;
	switch (award[0])
	{
	case T_Item:
		{
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec itemRecord;
			int iconIdx = itemtab->getTitleIndex("icon");
			int itemIdIdx = itemtab->getTitleIndex("itemid");
			int itemtypeindex = itemtab->getTitleIndex("类型");
			int itemcolorindex = itemtab->getTitleIndex("品质");
			if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(award[1]),itemRecord) >= 0)
			{
				imageName = itemRecord[iconIdx];
				count = award[3];
				itemId = atoi(itemRecord[itemIdIdx].c_str());
				if(atoi(itemRecord[itemtypeindex].c_str()) == IT_Jiangka)
				{
					rtncode = 1;
					int cardcolorindex = atoi(itemRecord[itemcolorindex].c_str());
					if(cardcolorsw)
						cardcolorsw->setActiveSwitchSet(cardcolorindex);
				}
			}
		}
		break;
	case T_Troops:
		{
			ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
			int idIndex = troopsTab->getTitleIndex("abstractid");
			int nameIndex = troopsTab->getTitleIndex("icon");
			crTableIO::StrVec record;
			if (troopsTab->queryOneRecord(idIndex,crArgumentParser::appItoa(award[1]),record) >= 0)
			{
				imageName = record[nameIndex];				
			}
			count = award[3];
			itemId = award[0];
		}
		break;
	default:
		{
			ref_ptr<crTableIO> resourceIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResourceIconTab);
			int idIndex = resourceIconTab->getTitleIndex("id");
			int nameIndex = resourceIconTab->getTitleIndex("name");
			crTableIO::StrVec record;
			if (resourceIconTab->queryOneRecord(idIndex,crArgumentParser::appItoa(award[0]),record) >= 0)
			{
				imageName = record[nameIndex];				
			}
			count = award[3];
			itemId = award[0];
		}
		break;
	}
	
	if(imageBox)
	{
		if(imageName.empty())
		{
			imageBox->setVisiable(false);
		}
		else
		{
			imageBox->setImageName(imageName);
			crData* uiData = imageBox->getDataClass();
			if (uiData)
			{
				uiData->inputParam(0,&itemId);
				uiData->inputParam(1,&count);
			}
			imageBox->setVisiable(true);
		}
	}
	if(input)
	{
		if(count>0)
		{
			input->setString(crArgumentParser::appItoa(count));
			input->setVisiable(true);
		}
		else
		{
			input->setVisiable(false);
		}
	}
	return rtncode;
}

void sendReward(std::vector<CRCore::crVector4i>& awardVec,crPlayerGameData* pPlayer,const std::string &command)
{
	JXJItemBagVec items;
	crRole* pMainRole = pPlayer->getMainRole();
	for (int i = 0; i < awardVec.size(); ++i)
	{
		CRCore::crVector4i& award = awardVec[i];
		switch (award[0])
		{
		case T_Food:
			pPlayer->doEvent(WCH_JXJRecvFood,MAKECREPARAM(award[3],NULL));
			break;
		case T_Wood:
			pPlayer->doEvent(WCH_JXJRecvWood,MAKECREPARAM(award[3],NULL));
			break;
		case T_Iron:
			pPlayer->doEvent(WCH_JXJRecvIron,MAKECREPARAM(award[3],NULL));
			break;
		case T_Horse:
			pPlayer->doEvent(WCH_JXJRecvHorse,MAKECREPARAM(award[3],NULL));
			break;
		case T_Copper:
			pPlayer->doEvent(WCH_JXJRecvCoppercash,MAKECREPARAM(award[3],NULL));
			break;
		case T_Exp:
			if(pMainRole)
				pMainRole->doEvent(WCH_JXJRecvExperience,MAKECREPARAM(award[3],NULL));
			break;
		case T_Achievement:
			if(pMainRole)
				pMainRole->doEvent(WCH_JXJRecvAchievement,MAKECREPARAM(award[3],NULL));
			break;
		case T_Giftgold:
			{
				MoneyChangeData  moneydata;
				moneydata.first = award[3];
				moneydata.second = command;
				pPlayer->doEvent(WCH_JXJRecvGiftgold,MAKECREPARAM(&moneydata,NULL));
			}
			break;
		case T_Exploit:
			if(pMainRole)
				pMainRole->doEvent(WCH_JXJRecvExploit,MAKECREPARAM(award[3],NULL));
			break;
		case T_Contribute:
			pPlayer->doEvent(WCH_JXJRecvCountryContribute,MAKECREPARAM(award[3],NULL));
			break;
		case T_JiangHun:
			pPlayer->doEvent(WCH_JXJRecvCardPieces,MAKECREPARAM(award[3],NULL));
			break;
		case T_Zhanquan:
			pPlayer->doEvent(WCH_JXJRecvZhengZanQuan,MAKECREPARAM(award[3],NULL));
			break;
		case T_VIPExp:
			pPlayer->doEvent(WCH_JXJAddVipExp,MAKECREPARAM(award[3],NULL));
			break;
		case T_CopperExtraProduce://征收铜钱额外加成ExtraProduceMap
		case T_FoodExtraProduce:
		case T_WoodExtraProduce:
		case T_IronExtraProduce:
		case T_HorseExtraProduce:
			{
				crVector3i vec3i(award[0],award[1],award[3]);
				pPlayer->doEvent(WCH_JXJAddExtraProduce,MAKECREPARAM(&vec3i,NULL));
			}
			break;
		case T_Item:
			{
				crBagItemData* pItem = new crBagItemData();
				pItem->setItemID(award[1]);
				//pItem->setEquipMagic(award[2]);
				pItem->setItemCount(award[3]);
				items.push_back(pItem);
			}
			break;
		case T_Troops:
			{
				crVector2i vec2i(award[1],award[3]);
				pPlayer->doEvent(WCH_JXJAddTroops,MAKECREPARAM(&vec2i,NULL));
			}
			break;
		case T_SuperEquipLvupPoint:
			{
				if (pMainRole && pMainRole->getDataClass())
				{
					crData *roledata = pMainRole->getDataClass();
					void *param;
					roledata->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
					unsigned char *lvuppoint = (unsigned char *)param;
					if (*lvuppoint < award[1])
					{
						pMainRole->doEvent(WCH_JXJGainSuperEquipLvupPoint, MAKECREPARAM(award[1], NULL));
					}					
				}
			}
			break;
		case T_SuperEquipCoin:
			pPlayer->doEvent(WCH_JXJRecvSuperEquipCoin, MAKECREPARAM(award[3], NULL));
			break;
		default:
			break;
		}
		
	}
	if (!items.empty())
	{
		pPlayer->doEvent(WCH_JXJRecvRewardItems,MAKECREPARAM(&items,0));
	}
	
}
/************************************************************************/
/* 购买装备页 UI                                                        */
/************************************************************************/
crJXJUIBuySavedEquipPageMethod::crJXJUIBuySavedEquipPageMethod():
	m_this(NULL)
{

}

crJXJUIBuySavedEquipPageMethod::crJXJUIBuySavedEquipPageMethod(const crJXJUIBuySavedEquipPageMethod& handle):
	crMethod(handle),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{

}


void crJXJUIBuySavedEquipPageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIBuySavedEquipPageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_promptCanvas = str;
		break;
	case 1:
		m_information = str;
		break;
	case 2:
		m_okbtn = str;
		break;
	}
}

void crJXJUIBuySavedEquipPageMethod::operator()(crHandle &handle)
{
	void *param = NULL;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			//crVector2i price;
			int ifhint = 1;
			crRole *mainRole = playerGameData->getMainRole();
			crData *mainroledata = mainRole?mainRole->getDataClass():NULL;
			crData *data = playerGameData->getDataClass();
			if (!mainroledata || !data)
			{
				return;
			}
			std::string buildingname;

			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			int price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuyEquipVecPrice,viplv).c_str());
			int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());
			mainroledata->getParam(WCHDATA_JXJSavedEquipVecs,param);
			unsigned char opensaveformationifo = *(unsigned char *)param;
			if(opensaveformationifo < maxpage)
			{
				int needgold = price;
				playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKECREPARAM(&needgold,NULL));
				if (needgold == 0)
				{
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));

					if (ifhint)
					{
						std::vector<float>v_i;
						ItemMap itemMap;
						rcfg::ConfigScript cfg_script(&itemMap);
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");
						//cfg_script.Add("Font",m_font);
						//cfg_script.Add("CharacterSize",m_characterSize);

						cfg_script.Add("Content");
						cfg_script.Push("Content",1);
						v_i.clear();
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","您确认要花费");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						v_i.clear();
						v_i.push_back(140.0f);
						v_i.push_back(40.0f);
						v_i.push_back(40.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						char tmpText[32];
						sprintf(tmpText,"%d礼金/元宝\0",price);
						cfg_script.Add("Text",tmpText);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						v_i.clear();
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","购买装备页吗？");
						cfg_script.Pop();

						cfg_script.Pop();
						if(information.valid())information->setHypertext(cfg_script);
					}

					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = canvas->getDataClass();
					int commandtype = CDP_Widget;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					if (ifhint)
					{
						crFilterRenderManager::getInstance()->doModal(canvas.get());
					}
					else
					{
						if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(2036,NULL));
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(2098,NULL));
			}

		}
	}

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
crJXJBuySavedEquipPageMethod::crJXJBuySavedEquipPageMethod():
	m_this(NULL)
{

}

crJXJBuySavedEquipPageMethod::crJXJBuySavedEquipPageMethod(const crJXJBuySavedEquipPageMethod& handle):
	crMethod(handle)
{

}

void crJXJBuySavedEquipPageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJBuySavedEquipPageMethod::addParam(int i, const std::string& str)
{

}

void crJXJBuySavedEquipPageMethod::operator ()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuySavedEquipPage,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
crJXJRecvBuySavedEquipPageMethod::crJXJRecvBuySavedEquipPageMethod():
	m_netType(GameClient_Game)
{

}

crJXJRecvBuySavedEquipPageMethod::crJXJRecvBuySavedEquipPageMethod(const crJXJRecvBuySavedEquipPageMethod& handle)
{

}

void crJXJRecvBuySavedEquipPageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvBuySavedEquipPageMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvBuySavedEquipPageMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			if(!gameServer)
				return;
			ref_ptr<crGameServerPlayerData> serverPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(!serverPlayerData.valid())
				return;
			crPlayerGameData* pPlayerData = serverPlayerData->getPlayerGameData();
			if(!pPlayerData)
				return;
			crRole * pMainRole = pPlayerData->getMainRole();
			if (!pMainRole)
				return;
			CRCore::crData* data = m_this->getDataClass();
			CRCore::crData* pRoleData = pMainRole->getDataClass();
			if(data && pRoleData)
			{
				char success = 0;
				void *param;
				bool needdeductgold = false;

				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuyEquipVecPrice,viplv).c_str());
				int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				pRoleData->getParam(WCHDATA_JXJSavedEquipVecs,param);
				unsigned char *opensaveequipnifo = (unsigned char *)param;
				do 
				{
					if (*opensaveequipnifo >= maxpage)
					{
						//装备页已达最大
						success = -1;
						break;
					}
					MoneyChangeData moneydata(needgold,"购买装备页");
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKECREPARAM(&moneydata,&needdeductgold));
					if (moneydata.first == 0)
					{
						success = 1;//购买切页成功
					}
					else
					{
						//元宝不够
						success = -2;
						break;
					}
					if (success == 1)
					{
						(*opensaveequipnifo) += 1;
					}
				} while (0);
				if (success == 1)
				{
					unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,0).c_str()));
					pRoleData->excHandle(MAKECREPARAM(WCH_LockData,1));
					pRoleData->getParam(WCHDATA_JXJLordEquipVecs, param);
					std::vector<JXJLordEquipVec> *openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
					openedEquipVec->resize(*opensaveequipnifo);
					JXJLordEquipVec equipVec;
					equipVec.resize(equiponmax);
					(*openedEquipVec)[(*opensaveequipnifo)-1] = equipVec;
					pRoleData->excHandle(MAKECREPARAM(WCH_LockData,0));

					if (needdeductgold)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
						callback->savePlayerData(serverPlayerData.get());
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(2);
					stream->_writeChar(success);
					stream->_writeUChar(*opensaveequipnifo);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuySavedEquipPage,stream.get());
				gameServer->getNetManager()->sendPacket(serverPlayerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_BuySavedEquipPage,logdata);//Log_ClearPunishTime
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream -> _readChar();
			int tipsindex = 0;
			if (success == 1)
			{
				tipsindex = 9155;
				unsigned char saveformationinfo = m_stream->_readUChar();
				crRole* pMainRole = m_this->getMainRole();
				if(!pMainRole)
					return;
				crData *data = pMainRole->getDataClass();
				if (!data)
					return;
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->inputParam(WCHDATA_JXJSavedEquipVecs,&saveformationinfo);
				data->getParam(WCHDATA_JXJLordEquipVecs, param);
				std::vector<JXJLordEquipVec> *openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
				unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,0).c_str()));
				openedEquipVec->resize(saveformationinfo);
				JXJLordEquipVec equipVec;
				equipVec.resize(equiponmax);
				(*openedEquipVec)[saveformationinfo-1] = equipVec;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			else
			{
				switch (success)
				{
				case -1://切页已最大
					tipsindex = 2098;
					break;
				case -2://元宝不足
					tipsindex = 2036;
					break;
				default:
					break;
				}
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(tipsindex,NULL));//
		}
	}
}

/************************************************************************/
/*         切换装备页(client)                                           */
/************************************************************************/
crJXJUISwitchEquipMethod::crJXJUISwitchEquipMethod():
	m_this(NULL)
{

}

crJXJUISwitchEquipMethod::crJXJUISwitchEquipMethod(const crJXJUISwitchEquipMethod& handle):
	crMethod(handle)
{

}

void crJXJUISwitchEquipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRUI::crRadioGroupWidgetNode *)param;
		break;
	}
}

void crJXJUISwitchEquipMethod::addParam(int i, const std::string& str)
{

}

void crJXJUISwitchEquipMethod::operator ()(crHandle &handle)
{
	CRNetApp::crPlayerGameData		*player	= CRNetApp::crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	if (m_this && player)
	{
		void* param = NULL;
		CRCore::crData					*player_data		= player->getDataClass();		
		CRCore::crCanvasNode			*formation_canvas	= m_this->getParentCanvas();	
		crRole* pMainRole = player->getMainRole();
		if(!player_data || !formation_canvas || !pMainRole)
			return;
		crData* pRoleData = pMainRole->getDataClass();
		player_data->excHandle(MAKECREPARAM(WCH_LockData,1));
		player_data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		player_data->excHandle(MAKECREPARAM(WCH_LockData,0));
		int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());

		pRoleData->getParam(WCHDATA_JXJCurrentEquipVecIndex,param);
		unsigned char currentIndex = *(unsigned char*)param;

		unsigned char index = m_this->getSelect();
		if(index != currentIndex && index < maxpage)
		{
			CRNet::crNetConductor	*netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
			CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
			if(netConductor && stream.valid())
			{
				stream->createBuf(1);
				stream->_writeUChar(index);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSwitchEquipPage,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}

	}
}
/************************************************************************/
/*                 切换装备页(server);                                  */
/************************************************************************/
crJXJRecvSwitchEquipPageMethod::crJXJRecvSwitchEquipPageMethod():
	m_this(NULL)
{

}

crJXJRecvSwitchEquipPageMethod::crJXJRecvSwitchEquipPageMethod(const crJXJRecvSwitchEquipPageMethod& handle):
	crMethod(handle)
{

}

void crJXJRecvSwitchEquipPageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{
			//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (CRNetApp::crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (CRCore::crStreamBuf *)(LOCREPARAM(param64));
			m_net_type = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvSwitchEquipPageMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvSwitchEquipPageMethod::operator ()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	void* param = NULL;
	if(m_net_type == CRNetApp::GameServer)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(!gameServer)
			return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *data = m_this->getDataClass();
		crRole* pMainRole = m_this->getMainRole();
		if (!data || !pMainRole || !playerData.valid())
			return;
		crData* pRoleData = pMainRole->getDataClass();
		if(pRoleData)
		{
			unsigned char index = m_stream->_readUChar();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());
			//pRoleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			pRoleData->getParam(WCHDATA_JXJCurrentEquipVecIndex,param);
			unsigned char currentIndex = *(unsigned char *)param;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			if(index != currentIndex && index < maxpage)
			{
				pRoleData->inputParam(WCHDATA_JXJCurrentEquipVecIndex,&index);
				pRoleData->getParam(WCHDATA_JXJLordEquipVec,param);
				JXJLordEquipVec lordEquipVec = *(JXJLordEquipVec*)param;
				stream->createBuf(1);
				stream->_writeUChar(index);		
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSwitchEquipPage,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			//pRoleData->excHandle(MAKECREPARAM(WCH_LockData,0));	
		}
	}
	else
	{
		if(!m_stream)
			return;
		if (m_this)
		{
			crRole* pMainRole = m_this->getMainRole();
			if(!pMainRole)
				return;
			crData *pRoleData = pMainRole->getDataClass();
			unsigned char index = m_stream->_readUChar();
			pRoleData->inputParam(WCHDATA_JXJCurrentEquipVecIndex,&index);
		}
	}
}

/************************************************************************/
/*     绑定编队                                                         */
/************************************************************************/
crJXJUIFeudalLord_BangdingUpdateMethod::crJXJUIFeudalLord_BangdingUpdateMethod():
	m_this(NULL)
{

}

crJXJUIFeudalLord_BangdingUpdateMethod::crJXJUIFeudalLord_BangdingUpdateMethod(const crJXJUIFeudalLord_BangdingUpdateMethod& handle):
	crMethod(handle)
{
	for(int i = 0; i < 4; ++i)
	{
		m_bindInput[i] = handle.m_bindInput[i];
		m_equipInput[i] = handle.m_equipInput[i];
		m_equipButton[i] = handle.m_equipButton[i];
		m_gamelist[i] = handle.m_gamelist[i];
		m_combobox[i] = handle.m_combobox[i];
	}
}

void crJXJUIFeudalLord_BangdingUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJUIFeudalLord_BangdingUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_bindInput[i] = str;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_equipInput[i-4] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		m_equipButton[i-8] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		m_gamelist[i-12] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		m_combobox[i-16] = str;
		break;
	}
}

void crJXJUIFeudalLord_BangdingUpdateMethod::operator ()(crHandle &handle)
{
	if(!m_this)
		return;
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!myplayer)
		return;

	ref_ptr<crStaticTextWidgetNode> bindInput[4]={NULL};
	ref_ptr<crStaticTextWidgetNode> equipInput[4] = {NULL};
	ref_ptr<crButtonWidgetNode> equipButton[4] = {NULL};
	ref_ptr<crTableWidgetNode> gamelist[4] = {NULL};
	ref_ptr<crButtonWidgetNode> queding  = NULL;
	ref_ptr<crComboBoxWidgetNode> comboBox[4] = {NULL};
	crData *data = myplayer->getDataClass();
	crRole *pRole = myplayer->getMainRole();
	if(!data || !pRole)
		return;
	crData* pRoleData = pRole->getDataClass();
	if(!pRoleData)
		return;
	void* param = NULL;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJOpenSaveFormationInfo,param);
	unsigned char opensaveformationifo = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJEquipBindInfo,param);
	int equipBindInfo = *(int*)param;
	data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
	SavedFormationInfoVec savedformationVec = *(SavedFormationInfoVec *)param;
	data->excHandle(MAKECREPARAM(WCH_LockData,0));

	unsigned char index[4];//绑定的装备索引
	index[0] = LOINT16(LOINT32(equipBindInfo));
	index[1] = HIINT16(LOINT32(equipBindInfo));
	index[2] = LOINT16(HIINT32(equipBindInfo));
	index[3] = HIINT16(HIINT32(equipBindInfo));


	pRoleData->getParam(WCHDATA_JXJSavedEquipVecs,param);
	unsigned char savedEquips = *(unsigned char*)param;
	for(int i = 0; i < 4; ++i)//控件只有4个;
	{
		bindInput[i] = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_bindInput[i]));
		equipInput[i] = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_equipInput[i]));
		equipButton[i] = dynamic_cast<crButtonWidgetNode*>(m_this->getWidget(m_equipButton[i]));
		gamelist[i] = dynamic_cast<crTableWidgetNode*>(m_this->getWidget(m_gamelist[i]));
		comboBox[i] = dynamic_cast<crComboBoxWidgetNode*>(m_this->getWidget(m_combobox[i]));
		if(!bindInput[i].valid() || !equipInput[i].valid() || !equipButton[i].valid() || !gamelist[i].valid() || !comboBox[i].valid())
			continue;
		gamelist[i]->clearData();
		if (savedformationVec.size() > i)
		{
			std::string str = savedformationVec[i].first;
			if(str == "")
				str = std::string("编队")+crArgumentParser::appItoa(i+1);
			bindInput[i]->setString(str);
			for (int j = 0; j < savedEquips; ++j)
			{
				gamelist[i]->addData(0,j,std::string("装备页")+crArgumentParser::appItoa(j+1));
			}

			if (index[i] > 0)
			{
				comboBox[i]->select(index[i]);
			}else
			{
				comboBox[i]->select(0);
			}
		}
		else
		{
			bindInput[i]->setString(std::string("未开启"));
		}		
	}

}

/************************************************************************/
/*          保存绑定编队(UI)                                            */
/************************************************************************/

crJXJUISave_BindFormationMethod::crJXJUISave_BindFormationMethod()
{

}

crJXJUISave_BindFormationMethod::crJXJUISave_BindFormationMethod(const crJXJUISave_BindFormationMethod& handle):
	crMethod(handle)
{

}

void crJXJUISave_BindFormationMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUISave_BindFormationMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_combobox[i]=str;
		break;
	}
}

void crJXJUISave_BindFormationMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(canvas.valid() && myplayer)
	{
		void* param = NULL;
		crData* pData = myplayer->getDataClass();
		if (!pData)
			return;
		pData->getParam(WCHDATA_JXJEquipBindInfo,param);
		int equipBindInfo = *(int*)param;

		ref_ptr<crComboBoxWidgetNode>combobox[4] = {NULL};

		unsigned char index[4];
		for (int i = 0; i < 4; ++i)
		{
			combobox[i] = dynamic_cast<crComboBoxWidgetNode*>(canvas->getWidget(m_combobox[i]));
			index[i] = combobox[i]->getSelectIndex();
		}
		_crInt32 newInfo = MAKEINT32(MAKEINT16(index[0],index[1]),MAKEINT16(index[2],index[3]));
		if (newInfo != equipBindInfo)
		{
			CRNet::crNetConductor	*netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
			CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
			if(netConductor && stream.valid())
			{
				stream->createBuf(4);
				stream->_writeInt(newInfo);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSaveBindFormation,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
		crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
	}
}

/************************************************************************/
/*           保存绑定编队(server)                                       */
/************************************************************************/

crJXJRecvSave_BindFormationMethod::crJXJRecvSave_BindFormationMethod():
	m_netType(GameClient_Game)
{

}

crJXJRecvSave_BindFormationMethod::crJXJRecvSave_BindFormationMethod(const crJXJRecvSave_BindFormationMethod& handle)
{

}

void crJXJRecvSave_BindFormationMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvSave_BindFormationMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvSave_BindFormationMethod::operator ()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	void* param = NULL;
	if(m_netType == CRNetApp::GameServer)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(!gameServer)
			return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *data = m_this->getDataClass();
		crRole* pMainRole = m_this->getMainRole();
		if (!data || !pMainRole || !playerData.valid())
			return;
		crData* pRoleData = pMainRole->getDataClass();
		if(pRoleData)
		{
			int bindInfo = m_stream->_readInt();


			pRoleData->getParam(WCHDATA_JXJSavedEquipVecs,param);
			unsigned char savedEquip = *(unsigned char *)param;
			

			unsigned char index[4];//绑定的装备索引
			index[0] = LOINT16(LOINT32(bindInfo));
			index[1] = HIINT16(LOINT32(bindInfo));
			index[2] = LOINT16(HIINT32(bindInfo));
			index[3] = HIINT16(HIINT32(bindInfo));

			for(int i = 0; i < 4; ++i)
			{
				if (index[i] >= savedEquip)
				{
					return;
				}
			}

			data->inputParam(WCHDATA_JXJEquipBindInfo,&bindInfo);
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(bindInfo);		
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSaveBindFormation,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);



		}
	}
	else
	{
		if (m_this && m_stream.valid())
		{
			crData* pData = m_this->getDataClass();
			int bindInfo = m_stream->_readInt();
			pData->inputParam(WCHDATA_JXJEquipBindInfo,&bindInfo);
		}
	}
}


/************************************************************************/
/*         黄钻新手礼包界面初始化(ui)      ;                            */
/************************************************************************/
crJXJUIYellowNewbieMethod::crJXJUIYellowNewbieMethod():m_this(NULL)
{

}

crJXJUIYellowNewbieMethod::crJXJUIYellowNewbieMethod(const crJXJUIYellowNewbieMethod& handle):
	crMethod(handle)
{
	for(int i = 0; i < 4; ++i)
	{
		m_icons[i] = handle.m_icons[i];
	}
	m_lockSwitch = handle.m_lockSwitch;
	m_buttonSwitch = handle.m_buttonSwitch;
	m_input = handle.m_input;
}

void crJXJUIYellowNewbieMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}

void crJXJUIYellowNewbieMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_icons[i] = str;
		break;
	case 4:
		m_lockSwitch = str;
		break;
	case 5:
		m_buttonSwitch =  str;
		break;
	}
}

void crJXJUIYellowNewbieMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myplayer)
	{
		void* param = NULL;
		crData* pPlayerData = myplayer->getDataClass();
		if(!pPlayerData)
			return;

		ref_ptr<crImageBoxWidgetNode> icons[4] = {NULL};
		ref_ptr<crMultiSwitch> lockSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lockSwitch));;
		ref_ptr<crMultiSwitch> butnSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_buttonSwitch));;
		//CRCore::crVector3i awardVec;
		std::vector<CRCore::crVector4i> awardData ;
		getYellowAwardInfoByType(awardData,YELLOW_NEWBIE);
		for(int i = 0; i < 4; ++i)
		{
			icons[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i]));
			if (!icons[i].valid())
				continue;
			if (i<awardData.size())
			{
				icons[i]->setVisiable(true);
				setRewardInfo( awardData[i],icons[i].get());
			}else
			{
				icons[i]->setVisiable(false);
			}
		}

		pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;
		if (lockSwitch.valid()&&butnSwitch.valid())
		{
			if (isYellow == 1)
			{
				lockSwitch->setActiveSwitchSet(0);
				butnSwitch->setActiveSwitchSet(0);
			}else
			{
				lockSwitch->setActiveSwitchSet(1);
				butnSwitch->setActiveSwitchSet(1);
			}
		}
	}
}



/************************************************************************/
/* 领取黄钻新手礼包(ui);                                                */
/************************************************************************/

crJXJUIGetYellowNewbieAwardMethod::crJXJUIGetYellowNewbieAwardMethod()
{

}

crJXJUIGetYellowNewbieAwardMethod::crJXJUIGetYellowNewbieAwardMethod(const crJXJUIGetYellowNewbieAwardMethod& handle):
	crMethod(handle)
{

}

void crJXJUIGetYellowNewbieAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIGetYellowNewbieAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJUIGetYellowNewbieAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myplayer)
	{
		void* param = NULL;
		crData* pPlayerData = myplayer->getDataClass();
		if(!pPlayerData)
			return;
		pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;

		if (isYellow == 1 && checkGetYellowReward(YELLOW_NEWBIE,pPlayerData))
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor)
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJGetYellowNewbie,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/************************************************************************/
/* 领取黄钻礼包(server);                                                */
/************************************************************************/
crJXJRecvGetYellowNewbieAwardMethod::crJXJRecvGetYellowNewbieAwardMethod():
	m_netType(GameClient_Game)
{

}

crJXJRecvGetYellowNewbieAwardMethod::crJXJRecvGetYellowNewbieAwardMethod(const crJXJRecvGetYellowNewbieAwardMethod& handle)
{

}

void crJXJRecvGetYellowNewbieAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGetYellowNewbieAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGetYellowNewbieAwardMethod::operator ()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	void* param = NULL;
	if(m_netType == CRNetApp::GameServer)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(!gameServer)
			return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *data = m_this->getDataClass();
		if(!playerData.valid() || !data)
			return;
		
		data->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;
		
		if (isYellow == 1 && checkGetYellowReward(YELLOW_NEWBIE,data))
		{
			std::vector<CRCore::crVector4i> awardInfo;
			getYellowAwardInfoByType(awardInfo,YELLOW_NEWBIE);
			sendReward(awardInfo,m_this,"黄钻新手礼包");

			data->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
			int yellowInfo = *(int*)param;
			yellowInfo |= YellowAward_Newbie;
			data->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);

			crRole *mainRole = m_this->getMainRole();
			crData *mainroledata = mainRole?mainRole->getDataClass():NULL;
			mainroledata->excHandle(MAKECREPARAM(WCH_LockData,1));
			mainroledata->getParam(WCHDATA_JXJNewHuodongMap,param);

			NewHuodongMap *huodongMap = (NewHuodongMap *)param;
			NewHuodongMap::iterator hmIt = huodongMap->find(ACTIVITY_YELLOW_ID);
			if (hmIt != huodongMap->end())
			{
				hmIt->second->setActivation(2,playerid);
			}
			mainroledata->excHandle(MAKECREPARAM(WCH_LockData,0));

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(yellowInfo);		
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJGetYellowNewbie,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);


		}

	}
	else if(m_netType == GameClient_Game)
	{
		if (m_this && m_stream.valid())
		{
			crData* pData = m_this->getDataClass();
			int yellowInfo = m_stream->_readInt();
			pData->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);
		}
	}
}

/************************************************************************/
/*         黄钻充值续费(ui)                                             */
/************************************************************************/
crJXJUIYellowRechargeMethod::crJXJUIYellowRechargeMethod()
{

}

crJXJUIYellowRechargeMethod::crJXJUIYellowRechargeMethod(const crJXJUIYellowRechargeMethod& handle)
{

}

void crJXJUIYellowRechargeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIYellowRechargeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_url = str;
		break;
	default:
		break;
	}
}

void crJXJUIYellowRechargeMethod::operator ()(crHandle &handle)
{
	TCHAR* str = new TCHAR[256] ;
	wsprintf(str,(LPCTSTR)m_url.c_str());

	HWND  hwnd = CRNetApp::crRunGameHandle::getInstance()->getParentWnd();
	COPYDATASTRUCT data;
	data.cbData = _msize(str);
	data.lpData = str;
	if (hwnd > 0 )
	{
		::SendMessage(hwnd,WM_USER+603,NULL,(LPARAM)&data);
	}
	delete [] str;
}

/************************************************************************/
/* 年费黄钻充值                                                                     */
/************************************************************************/
crJXJUIYellowYearRechargeMethod::crJXJUIYellowYearRechargeMethod()
{

}

crJXJUIYellowYearRechargeMethod::crJXJUIYellowYearRechargeMethod(const crJXJUIYellowYearRechargeMethod& handle)
{

}

void crJXJUIYellowYearRechargeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIYellowYearRechargeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_url = str;
		break;;
	default:
		break;
	}
}

void crJXJUIYellowYearRechargeMethod::operator ()(crHandle &handle)
{
	TCHAR* str = new TCHAR[256] ;
	wsprintf(str,(LPCTSTR)m_url.c_str());

	HWND  hwnd = CRNetApp::crRunGameHandle::getInstance()->getParentWnd();
	COPYDATASTRUCT data;
	data.cbData = _msize(str);
	data.lpData = str;
	if (hwnd > 0 )
	{
		::SendMessage(hwnd,WM_USER+603,NULL,(LPARAM)&data);
	}
	delete [] str;
}
/************************************************************************/
/* 黄钻特权界面初始化(ui)                                               */
/************************************************************************/
crJXJUIYellowPrivilegeMethod::crJXJUIYellowPrivilegeMethod()
{

}

crJXJUIYellowPrivilegeMethod::crJXJUIYellowPrivilegeMethod(const crJXJUIYellowPrivilegeMethod& handle):
	crMethod(handle),
	m_yellowSwitch(handle.m_yellowSwitch)
{
	for (int i = 0; i < 43; ++i)
	{
		m_icons[i] = handle.m_icons[i];
		m_inputs[i] = handle.m_inputs[i]; 
	}
}

void crJXJUIYellowPrivilegeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}

void crJXJUIYellowPrivilegeMethod::addParam(int i, const std::string& str)
{
	if (i>=0 && i <43)
	{
		m_icons[i] = str;
	}
	else if (i == 43)
	{
		m_yellowSwitch = str;
	}
	else if (i >43 && i < 87)
	{
		m_inputs[i-44] = str;
	}else if(i == 87)
	{
		m_iconSwitch = str;
	}else if(i == 88)
	{
		m_levelSwitch = str;
	}else if(i == 89)
	{
		m_yearSwitch = str;
	}
}

void crJXJUIYellowPrivilegeMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!m_this || !myplayer)
		return;

	void* param = NULL;
	crData* pPlayerData = myplayer->getDataClass();
	if(!pPlayerData)
		return;

	//每日礼包
	std::vector<crVector4i> awardData;
	for (int i = 0; i < MAX_YELLOW_LEVEL; ++i)
	{
		awardData.clear();
		getYellowAwardInfoByType(awardData,YELLOW_DAILY,i+1);
		for(int j = 0; j < 4; ++j)
		{
			ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[j+8+i*4]));
			ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputs[j+8+i*4]));
			if (!icon.valid() || !input.valid())
				continue;
			if (j<awardData.size())
			{
				icon->setVisiable(true);
				setRewardInfo(awardData[j],icon.get(),input.get());
			}
			else
			{
				icon->setVisiable(false);
			}
		}
	}

	//年费每日
	std::vector<crVector4i> yearAwardData;
	getYellowAwardInfoByType(yearAwardData,YELLOW_YEAR_DAILY);
	for (int i = 0; i < 3; ++i)
	{
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i+40]));
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputs[i+40]));
		if (!icon.valid() || !input.valid())
			continue;
		if (i<yearAwardData.size())
		{
			icon->setVisiable(true);
			setRewardInfo(yearAwardData[i],icon.get(),input.get());
		}else
		{
			icon->setVisiable(false);
		}
	}

	//新手礼包
	std::vector<crVector4i> newbieAwardData;
	getYellowAwardInfoByType(newbieAwardData,YELLOW_NEWBIE);
	for (int i = 0; i < 4; ++i)
	{
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i]));
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputs[i]));
		if (!icon.valid() || !input.valid())
			continue;
		if (i<newbieAwardData.size())
		{
			icon->setVisiable(true);
			setRewardInfo(newbieAwardData[i],icon.get(),input.get());
		}else
		{
			icon->setVisiable(false);
		}
	}

	//升级礼包
	std::vector<crVector4i> upgradeAwardData;
	getYellowAwardInfoByType(upgradeAwardData,YELLOW_UPGRADE);
	for (int i = 0; i < 4; ++i)
	{
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i+4]));
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputs[i+4]));
		if (!icon.valid() || !input.valid())
			continue;
		if (i<upgradeAwardData.size())
		{
			icon->setVisiable(true);
			setRewardInfo(upgradeAwardData[i],icon.get(),input.get());
		}else
		{
			icon->setVisiable(false);
		}
	}

	pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
	unsigned char isYellow = *(unsigned char*)param;

	pPlayerData->getParam(WCHDATA_JXJISYEARYELLOW,param);
	unsigned char isYearYellow = *(unsigned char*)param;

	pPlayerData->getParam(WCHDATA_JXJYELLOWLEVEL,param);
	unsigned char level = *(unsigned char*)param;
	ref_ptr<crMultiSwitch> yellowSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_yellowSwitch));
	ref_ptr<crMultiSwitch> iconSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_iconSwitch));
	ref_ptr<crMultiSwitch> levelSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_levelSwitch));
	ref_ptr<crMultiSwitch> yearSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_yearSwitch));
	if (yellowSwitch.valid() && iconSwitch.valid() && levelSwitch.valid() && yearSwitch.valid())
	{
		if (isYellow == 1)
		{
			yellowSwitch->setActiveSwitchSet(1);
			iconSwitch->setActiveSwitchSet(1);
			levelSwitch->setActiveSwitchSet(level-1);
		}else
		{
			yellowSwitch->setActiveSwitchSet(0);
			iconSwitch->setActiveSwitchSet(0);
		}
		if (isYearYellow == 1)
		{
			yearSwitch->setActiveSwitchSet(0);
		}
		else
		{
			yearSwitch->setActiveSwitchSet(1);
		}
	}


}
/************************************************************************/
/*      打开每日礼包界面(ui)                                            */
/************************************************************************/
crJXJUIDailyYellowShowMethod::crJXJUIDailyYellowShowMethod()
{

}

crJXJUIDailyYellowShowMethod::crJXJUIDailyYellowShowMethod(const crJXJUIDailyYellowShowMethod& handle):
	crMethod(handle)
{

}

void crJXJUIDailyYellowShowMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIDailyYellowShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_yellow = str;
		break;
	case 1:
		m_year_yellow = str;
		break;
	default:
		break;
	}
}

void crJXJUIDailyYellowShowMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!m_this || !myplayer)
		return;

	void* param = NULL;
	crData* pPlayerData = myplayer->getDataClass();
	if(!pPlayerData)
		return;
	ref_ptr<crCanvasNode> yellowCanvas = crFilterRenderManager::getInstance()->findCanvas(m_yellow);
	ref_ptr<crCanvasNode> yearYellowCanvas = crFilterRenderManager::getInstance()->findCanvas(m_year_yellow);
	if (!yearYellowCanvas.valid() && !yellowCanvas.valid())
		return;
	pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
	unsigned char isYellow = *(unsigned char*)param;

	pPlayerData->getParam(WCHDATA_JXJISYEARYELLOW,param);
	unsigned char isYearYellow = *(unsigned char*)param;

	if (isYearYellow)
	{
		crFilterRenderManager::getInstance()->showCanvas(yearYellowCanvas.get(),true);
	}
	else if(isYellow)
	{
		crFilterRenderManager::getInstance()->showCanvas(yellowCanvas.get(),true);
	}
}

/************************************************************************/
/* 黄钻每日礼包(ui)                                                     */
/************************************************************************/
crJXJUIDailyYellowAwardMethod::crJXJUIDailyYellowAwardMethod()
{

}

crJXJUIDailyYellowAwardMethod::crJXJUIDailyYellowAwardMethod(const crJXJUIDailyYellowAwardMethod& handle):
	crMethod(handle)
{
	for(int i = 0; i < 4; ++i)
	{
		m_icons[i] = handle.m_icons[i];
	}
	m_lockSwitch = handle.m_lockSwitch;
	m_buttonSwitch = handle.m_buttonSwitch;
	m_btnGet = handle.m_btnGet;
}

void crJXJUIDailyYellowAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}

void crJXJUIDailyYellowAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_icons[i] = str;
		break;
	case 4:
		m_lockSwitch = str;
		break;
	case 5:
		m_buttonSwitch =  str;
		break;
	case 6:
		m_btnGet = str;
		break;
	}
}

void crJXJUIDailyYellowAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myplayer)
	{
		void* param = NULL;
		crData* pPlayerData = myplayer->getDataClass();
		if(!pPlayerData)
			return;
		
		ref_ptr<crImageBoxWidgetNode> icons[4] = {NULL};
		ref_ptr<crMultiSwitch> lockSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lockSwitch));;
		ref_ptr<crMultiSwitch> butnSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_buttonSwitch));;
		CRCore::crVector3i awardVec;
		std::vector<CRCore::crVector4i> awardData;
		pPlayerData->getParam(WCHDATA_JXJYELLOWLEVEL,param);
		unsigned char yellowLevel = *(unsigned char*)param;
		getYellowAwardInfoByType(awardData,YELLOW_DAILY,yellowLevel);
		for(int i = 0; i < 4; ++i)
		{
			icons[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i]));
			if (!icons[i].valid())
				continue;

			if (i < awardData.size())
			{
				icons[i]->setVisiable(true);
				setRewardInfo(awardData[i],icons[i].get());
			}else
			{
				icons[i]->setVisiable(false);
			}
		}

		pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;
		if (lockSwitch.valid()&&butnSwitch.valid())
		{
			if (isYellow == 1)
			{
				lockSwitch->setActiveSwitchSet(0);
				butnSwitch->setActiveSwitchSet(0);
			}else
			{
				lockSwitch->setActiveSwitchSet(1);
				butnSwitch->setActiveSwitchSet(1);
			}
		}
		/*	pPlayerData->getParam(WCHDATA_JXJYELLOWLEVEL,param);
		unsigned char level = *(unsigned char*)param;
		ref_ptr<crButtonWidgetNode>btnGet = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btnGet));
		if (btnGet.valid())
		{
		if (checkGetYellowReward(YELLOW_DAILY,pPlayerData,level))
		{
		btnGet->setEnable(true);
		}
		else
		{
		btnGet->setEnable(false);
		}
		}*/
	}
}
/************************************************************************/
/* 年费黄钻每日礼包(ui)                                                 */
/************************************************************************/
crJXJUIDailyYearYellowAwardMethod::crJXJUIDailyYearYellowAwardMethod()
{

}

crJXJUIDailyYearYellowAwardMethod::crJXJUIDailyYearYellowAwardMethod(const crJXJUIDailyYearYellowAwardMethod& handle):
	crMethod(handle),
	m_lockSwitch(handle.m_lockSwitch)
{
	for(int i= 0; i < 8; ++i)
	{
		m_icons[i] = handle.m_icons[i];
	}
}

void crJXJUIDailyYearYellowAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}

void crJXJUIDailyYearYellowAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		m_icons[i] = str;
		break;
	case 8:
		m_lockSwitch = str;
		break;
	case 9:
		m_btnGet = str;
	default:
		break;
	}
}

void crJXJUIDailyYearYellowAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!m_this || !myplayer)
		return;

	void* param = NULL;
	crData* pPlayerData = myplayer->getDataClass();
	if(!pPlayerData)
		return;

	pPlayerData->getParam(WCHDATA_JXJISYEARYELLOW,param);
	unsigned char isYearYellow = *(unsigned char*)param;

	pPlayerData->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
	int* yellowInfo = (int*)param;

	pPlayerData->getParam(WCHDATA_JXJYELLOWLEVEL,param);
	unsigned char yellowLevel = *(unsigned char*)param;

	std::vector<crVector4i> normalAward;
	getYellowAwardInfoByType(normalAward,YELLOW_DAILY,yellowLevel);
	std::vector<crVector4i> yellowAward;
	getYellowAwardInfoByType(yellowAward,YELLOW_YEAR_DAILY);
	ref_ptr<crImageBoxWidgetNode> icons_normal[4] = {NULL};
	ref_ptr<crImageBoxWidgetNode> icons_year[4] = {NULL};
	for (int i = 0; i < 4; ++i)
	{
		icons_normal[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i]));
		icons_year[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i+4]));
		if (!icons_year[i].valid()||!icons_normal[i].valid())
			continue;
		crTableIO::StrVec itemRecord;
		if (i < normalAward.size())
		{
			icons_normal[i]->setVisiable(true);
			setRewardInfo(normalAward[i],icons_normal[i].get());
		}else
		{
			icons_normal[i]->setVisiable(false);
		}
		if (i < yellowAward.size())
		{
			icons_year[i]->setVisiable(true);
			setRewardInfo(yellowAward[i],icons_year[i].get());
		}else
		{
			icons_year[i]->setVisiable(false);
		}
	}

	ref_ptr<crMultiSwitch> lockSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lockSwitch));
	if (isYearYellow == 1)
	{
		lockSwitch->setActiveSwitchSet(1);
	}
	else
	{
		lockSwitch->setActiveSwitchSet(0);
	}

	/*ref_ptr<crButtonWidgetNode>btnGet = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btnGet));
	if (btnGet.valid())
	{
	if (checkGetYellowReward(YELLOW_YEAR_DAILY,pPlayerData))
	{
	btnGet->setEnable(true);
	}
	else
	{
	btnGet->setEnable(false);
	}
	}*/
	
}
/************************************************************************/
/* 领取每日礼包(client)                                                 */
/************************************************************************/
crJXJUIGetDailyYellowAwardMethod::crJXJUIGetDailyYellowAwardMethod()
{

}

crJXJUIGetDailyYellowAwardMethod::crJXJUIGetDailyYellowAwardMethod(const crJXJUIGetDailyYellowAwardMethod& handle):
	crMethod(handle)
{

}

void crJXJUIGetDailyYellowAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIGetDailyYellowAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = (unsigned char)atoi(str.c_str());
	default:
		break;
	}
}

void crJXJUIGetDailyYellowAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myplayer)
	{
		void* param = NULL;
		crData* pPlayerData = myplayer->getDataClass();
		if(!pPlayerData)
			return;
		pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;
		
		if (isYellow == 0)
			return;

		bool canGetAward = false;
		if(m_type==0)
		{
			canGetAward = checkGetYellowReward(YELLOW_DAILY,pPlayerData);
		}
		else
		{
			canGetAward = checkGetYellowReward(YELLOW_YEAR_DAILY,pPlayerData);
		}

		if (canGetAward)
		{
			CRNet::crNetConductor	*netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
			CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
			if(netConductor && stream.valid())
			{
				stream->createBuf(1);
				stream->_writeUChar(m_type);
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJGetDailyYellowAward,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}

		}

	}
}

/************************************************************************/
/* 领取每日礼包(server)                                                 */
/************************************************************************/
crJXJRecvGetDailyYellowAwardMethod::crJXJRecvGetDailyYellowAwardMethod():
	m_netType(GameClient_Game)
{

}

crJXJRecvGetDailyYellowAwardMethod::crJXJRecvGetDailyYellowAwardMethod(const crJXJRecvGetDailyYellowAwardMethod& handle):
	crMethod(handle)
{

}

void crJXJRecvGetDailyYellowAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGetDailyYellowAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGetDailyYellowAwardMethod::operator ()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	void* param = NULL;
	if(m_netType == CRNetApp::GameServer)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(!gameServer)
			return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *data = m_this->getDataClass();
		if(!playerData.valid() || !data)
			return;

		unsigned char type = m_stream->_readUChar();
		YELLOW_AWARD_TYPE yellowType = type==0?YELLOW_DAILY:YELLOW_YEAR_DAILY;

		data->getParam(WCHDATA_JXJYELLOWLEVEL,param);
		unsigned char yellowLevel = *(unsigned char*)param;
		if(checkGetYellowReward(yellowType,data))
		{
			std::vector<CRCore::crVector4i> awards ;
			getYellowAwardInfoByType(awards,yellowType,yellowLevel);
			
			if (yellowType == YELLOW_YEAR_DAILY)
			{
				std::vector<CRCore::crVector4i> yellowAwards;
				getYellowAwardInfoByType(yellowAwards,YELLOW_DAILY,yellowLevel);
				awards.insert(awards.end(),yellowAwards.begin(),yellowAwards.end());
			}
			sendReward(awards,m_this,"黄钻日常礼包");

			data->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
			int yellowInfo = *(int*)param;
			if (yellowType == YELLOW_DAILY)
			{
				yellowInfo |= YellowAward_Daily; 
			}
			else if (yellowType == YELLOW_YEAR_DAILY)
			{
				yellowInfo |= YellowAward_YearDaily | YellowAward_Daily; 
			}
			data->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(yellowInfo);		
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJGetDailyYellowAward,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

			///游戏日志
			std::string logdata = crArgumentParser::appItoa(yellowType);
			GameLogData gamelog(Log_YellowAward,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
			



		}
	}
	else if(m_netType == GameClient_Game)
	{
		if (m_this && m_stream.valid())
		{
			crData* pData = m_this->getDataClass();
			int yellowInfo = m_stream->_readInt();
			pData->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);
		}
	}


}

/************************************************************************/
/* 黄钻升级礼包(ui)                                                     */
/************************************************************************/
crJXJUIYellowUpgradeAwardMethod::crJXJUIYellowUpgradeAwardMethod()
{

}

crJXJUIYellowUpgradeAwardMethod::crJXJUIYellowUpgradeAwardMethod(const crJXJUIYellowUpgradeAwardMethod& handle):
	crMethod(handle),
	m_lockSwitch(handle.m_lockSwitch),
	m_buttonSwitch(handle.m_buttonSwitch)
{
	for(int i= 0; i < 8; ++i)
	{
		m_icons[i] = handle.m_icons[i];
	}
}

void crJXJUIYellowUpgradeAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crCanvasNode *)param;
		break;
	}
}

void crJXJUIYellowUpgradeAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		m_icons[i] = str;
		break;
	case 8:
		m_lockSwitch = str;
		break;
	case 9:
		m_buttonSwitch = str;
		break;
	default:
		break;
	}
}

void crJXJUIYellowUpgradeAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!m_this || !myplayer)
		return;

	void* param = NULL;
	crData* pPlayerData = myplayer->getDataClass();
	if(!pPlayerData)
		return;

	pPlayerData->getParam(WCHDATA_JXJISYELLOW,param);
	unsigned char isYellow = *(unsigned char*)param;


	std::vector<crVector4i> normalAward;
	std::vector<crVector4i> yellowAward; 
	getYellowAwardInfoByType(normalAward,YELLOW_UPGRADE_NORMAL);
	getYellowAwardInfoByType(yellowAward,YELLOW_UPGRADE);
	ref_ptr<crImageBoxWidgetNode> icons_normal[4] = {NULL};
	ref_ptr<crImageBoxWidgetNode> icons_year[4] = {NULL};
	for (int i = 0; i < 4; ++i)
	{
		icons_normal[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i]));
		icons_year[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i+4]));
		if (!icons_year[i].valid()||!icons_normal[i].valid())
			continue;
		crTableIO::StrVec itemRecord;
		if (i < normalAward.size())
		{
			icons_normal[i]->setVisiable(true);
			setRewardInfo(normalAward[i],icons_normal[i].get());
		}
		else
		{
			icons_normal[i]->setVisiable(false);
		}
		if (i < yellowAward.size())
		{
			icons_year[i]->setVisiable(true);
			setRewardInfo(yellowAward[i],icons_year[i].get());
		}else
		{
			icons_year[i]->setVisiable(false);
		}
	}

	ref_ptr<crMultiSwitch> lockSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_lockSwitch));
	ref_ptr<crMultiSwitch> btnSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_buttonSwitch));
	if (isYellow == 1)
	{
		lockSwitch->setActiveSwitchSet(1);
		btnSwitch->setActiveSwitchSet(1);
	}
	else
	{
		lockSwitch->setActiveSwitchSet(0);
		btnSwitch->setActiveSwitchSet(0);
	}

}

/************************************************************************/
/*         领取升级礼包       (client)                                  */
/************************************************************************/
crJXJUIGetYellowUpgradeAwardMethod::crJXJUIGetYellowUpgradeAwardMethod()
{

}

crJXJUIGetYellowUpgradeAwardMethod::crJXJUIGetYellowUpgradeAwardMethod(const crJXJUIGetYellowUpgradeAwardMethod& handle):
	crMethod(handle)
{

}

void crJXJUIGetYellowUpgradeAwardMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUIGetYellowUpgradeAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJUIGetYellowUpgradeAwardMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myplayer)
	{
		void* param = NULL;
		crData* pPlayerData = myplayer->getDataClass();
		if(!pPlayerData)
			return;

		crRole* pMainRole = myplayer->getMainRole();
		crData* pRoleData = pMainRole?pMainRole->getDataClass():NULL;
		if (!pRoleData)
			return;
		pRoleData->getParam(WCHDATA_Level,param);
		unsigned char level = *(unsigned char*)param;
		if (checkGetYellowReward(YELLOW_UPGRADE,pPlayerData,level))
		{
			CRNet::crNetConductor	*netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
			if(netConductor)
			{
				CRNetApp::crPlayerDataEventPacket packet;
				CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJGetYellowUpgradeAward,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}

		}

	}
}


/************************************************************************/
/*                     领取升级礼包(server)                             */
/************************************************************************/
crJXJRecvGetYellowUpgradeAwardMethod::crJXJRecvGetYellowUpgradeAwardMethod()
{

}

crJXJRecvGetYellowUpgradeAwardMethod::crJXJRecvGetYellowUpgradeAwardMethod(const crJXJRecvGetYellowUpgradeAwardMethod& handle):
	crMethod(handle)
{

}

void crJXJRecvGetYellowUpgradeAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGetYellowUpgradeAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGetYellowUpgradeAwardMethod::operator ()(crHandle &handle)
{
	if(!m_this || !m_stream.valid()) return;
	void* param = NULL;
	if(m_netType == CRNetApp::GameServer)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(!gameServer)
			return;
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		crData *data = m_this->getDataClass();
		crRole* pRole = m_this->getMainRole();
		if(!playerData.valid() || !data || !pRole)
			return;
		crData* pRoleData = pRole->getDataClass();
		if (!playerData)
			return;
		pRoleData->getParam(WCHDATA_Level,param);
		unsigned char level = *(unsigned char*)param;

		if (checkGetYellowReward(YELLOW_UPGRADE,data,level))
		{
			data->getParam(WCHDATA_JXJISYELLOW,param);
			unsigned char isYellow = *(unsigned char*)param;

			data->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
			int yellowInfo = *(int*)param;

			std::vector<CRCore::crVector4i> awardInfo;
			getYellowAwardInfoByType(awardInfo,YELLOW_UPGRADE_NORMAL);
			if (isYellow == 1 )
			{ 
				std::vector<CRCore::crVector4i> yellowAward;
				getYellowAwardInfoByType(yellowAward,YELLOW_UPGRADE);
				awardInfo.insert(awardInfo.end(),yellowAward.begin(),yellowAward.end());
			}
			sendReward(awardInfo,m_this,"黄钻升级礼包");

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int newbieLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJNewbieEndLevel,vipLevel).c_str());
			unsigned char awardLv = (yellowInfo & YellowAward_LEVEL);
			awardLv = awardLv<newbieLevel?newbieLevel-1:awardLv;
			yellowInfo = (yellowInfo&0xffffff00) | (awardLv+1);
			data->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(yellowInfo);		
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJGetYellowNewbie,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);


		}
	}
	else if(m_netType == GameClient_Game)
	{
		if (m_this && m_stream.valid())
		{
			crData* pData = m_this->getDataClass();
			int yellowInfo = m_stream->_readInt();
			pData->inputParam(WCHDATA_JXJYELLOWAWARDINFO,&yellowInfo);
		}
	}
}
/************************************************************************/
/* crJXJUIShowCanvasMethod                                              */
/************************************************************************/
crJXJUIShowCanvasMethod::crJXJUIShowCanvasMethod()
{

}

crJXJUIShowCanvasMethod::crJXJUIShowCanvasMethod(const crJXJUIShowCanvasMethod& handle)
{

}

void crJXJUIShowCanvasMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIShowCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJUIShowCanvasMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		ref_ptr<crCanvasNode>canvas2 = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(canvas.valid() && canvas2.valid())
		{
			canvas->setMatrix(crMatrix::translate(-0.3,0,0.0f));
			const crBoundingBox &bbox = canvas->getBoundBox();

			const crBoundingBox &bbox2 = canvas2->getBoundBox();
			canvas2->setMatrix(crMatrix::translate(bbox.m_max[0] + 0.5*(bbox2.m_max[0] - bbox2.m_min[0]),0,0.0f));
			crFilterRenderManager::getInstance()->showCanvas(canvas2.get(),true);
			
		}
	}
}

///////////////////////////////////////////
////
////crBuyGoldURLMethod
////
///////////////////////////////////////////
//crJXJUIGoToForumMethod::crJXJUIGoToForumMethod()
//{
//}
//crJXJUIGoToForumMethod::crJXJUIGoToForumMethod(const crJXJUIGoToForumMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJUIGoToForumMethod::inputParam(int i, void *param)
//{
//}
//void crJXJUIGoToForumMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0 :
//		m_txUrl = str;
//		break;
//	case 1:
//		m_url = str;
//		break;
//	default:
//		break;
//	}
//}
//void crJXJUIGoToForumMethod::operator()(crHandle &handle)
//{
//	std::string url;
//	//if (CRIOManager::fileExists("web.ini"))
//	if(crMyPlayerData::getInstance()->getSelectedGame()->getPlatform().find("QQ") != std::string::npos)
//	{//tx
//		if(CRCore::crDisplaySettings::instance()->getRunMode()==3)
//		{
//			TCHAR* str = new TCHAR[256] ;
//			wsprintf(str,(LPCTSTR)m_txUrl.c_str());
//
//			HWND  hwnd = CRNetApp::crRunGameHandle::getInstance()->getParentWnd();
//			COPYDATASTRUCT data;
//			data.cbData = _msize(str);
//			data.lpData = str;
//			if (hwnd > 0 )
//			{
//				::SendMessage(hwnd,WM_USER+603,NULL,(LPARAM)&data);
//			}
//			delete [] str;
//		}
//		else
//		{
//			url = m_txUrl;
//			ShellExecute(::GetActiveWindow(),"open",url.c_str(),NULL,NULL,SW_HIDE);
//		}
//	}
//	else
//	{
//		url = m_url;
//		ShellExecute(::GetActiveWindow(),"open",url.c_str(),NULL,NULL,SW_HIDE);
//	}
//}
/**************************************************************************

crJXJShowYellowUpgradeCanvasMethod
 
**************************************************************************/
JXJ::crJXJShowYellowUpgradeCanvasMethod::crJXJShowYellowUpgradeCanvasMethod():
	m_this(NULL)
{

}

JXJ::crJXJShowYellowUpgradeCanvasMethod::crJXJShowYellowUpgradeCanvasMethod( const crJXJShowYellowUpgradeCanvasMethod & handle ):
	crMethod(handle),
	m_this(NULL)
{

}

void JXJ::crJXJShowYellowUpgradeCanvasMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crBrain *)param;
		break;
	}
}

void JXJ::crJXJShowYellowUpgradeCanvasMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_yellowUpgrade = str;
		break;
	}
}

void JXJ::crJXJShowYellowUpgradeCanvasMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && m_this->getDataClass() && myPlayer)
	{
		crRole *mainRole = myPlayer->getMainRole();
		crData *maindata = mainRole ? mainRole->getDataClass():NULL;
		crData *pPlayerData = myPlayer->getDataClass();
		if (maindata)
		{
			void* param = NULL;
			maindata->getParam(WCHDATA_Level,param);
			unsigned char mylv = *(unsigned char *)param;
			pPlayerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int newbieLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJNewbieEndLevel,vipLevel).c_str());
			if(crMyPlayerData::getInstance()->getSelectedGame()->getPlatform().find("QQ") != std::string::npos)
			{
				if (/*CRIOManager::fileExists("web.ini") && */mylv>=newbieLevel && !m_yellowUpgrade.empty() && checkGetYellowReward(YELLOW_UPGRADE,pPlayerData,mylv))
				{
					bool needShow = false;
					if(crRunGameHandle::getInstance()->isInManor())
					{
						ref_ptr<crCanvasNode> yellowUpgrade = crFilterRenderManager::getInstance()->findCanvas(m_yellowUpgrade);
						if(yellowUpgrade.valid())
							crFilterRenderManager::getInstance()->showCanvas(yellowUpgrade.get(),true);
					}
				}
			}
		}
	}
}

/************************************************************************/
/*         腾讯平台充值(ui)                                             */
/************************************************************************/
crJXJUITXRechargeMethod::crJXJUITXRechargeMethod()
{

}

crJXJUITXRechargeMethod::crJXJUITXRechargeMethod(const crJXJUITXRechargeMethod& handle)
{

}

void crJXJUITXRechargeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
		}
		break;

	case 1:
		m_this = (CRCore::crWidgetNode *)param;
		break;
	}
}

void crJXJUITXRechargeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = str;
		break;
	//case 1:
	//	m_url = str;
	//	break;
	}
}

void crJXJUITXRechargeMethod::operator ()(crHandle &handle)
{
	ref_ptr<crTableIO>rechargeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTXRechargeTab);
	crTableIO::StrVec record;
	if (rechargeTab.valid())
	{
		int idIndex = rechargeTab->getTitleIndex("itemid");
		int priceIndex = rechargeTab->getTitleIndex("price");
		int nameIndex = rechargeTab->getTitleIndex("name");
		int descIndex = rechargeTab->getTitleIndex("desc");
		int iconIndex = rechargeTab->getTitleIndex("icon");
		if (rechargeTab->queryOneRecord(0,m_index,record) >= 0)
		{
			ref_ptr<crTableIO> sitetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSiteTab);
			crTableIO::StrVec siterecord;
			if(sitetab->queryOneRecord(0,"QQ",siterecord)>=0)
			{
				int index = sitetab->getTitleIndex("Recharge");
				std::string siteurl = siterecord[index];

				char tmp[32];
				int serverid = crMyPlayerData::getInstance()->getLastServerID();
				sprintf(tmp,"&serverid=%d",serverid);
				std::string str = siteurl+ record[idIndex]+"*"+record[priceIndex]+"*1&goodsmeta="+record[nameIndex]+"*"+record[descIndex]+"&goodsurl="+record[iconIndex]+tmp;
				//ShellExecute(::GetActiveWindow(),"open","crExplorer.exe",url.c_str(),NULL,SW_HIDE);
				HWND  hwnd = CRNetApp::crRunGameHandle::getInstance()->getParentWnd();
				//hwnd = ::FindWindow("CcreActiveXCtrl",NULL);
				if (hwnd != NULL )
				{
					TCHAR* url = new TCHAR[512] ;
					wsprintf(url,(LPCTSTR)str.c_str());
					COPYDATASTRUCT data;
					data.cbData = _msize(url);
					data.lpData = url;
					::SendMessage(hwnd,WM_USER+602,NULL,(LPARAM)&data);
					delete [] url;
				}
				else
				{
					int playerid = crMyPlayerData::getInstance()->getPlayerID();
					std::string url = str + "&playerid=" + crArgumentParser::appItoa(playerid);
					ShellExecute(NULL,"open",url.c_str(),NULL,NULL,SW_SHOW);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGoldingotBuyUpdateNewMethod
//
/////////////////////////////////////////
crJXJUIGoldingotBuyUpdateNewMethod::crJXJUIGoldingotBuyUpdateNewMethod()
{
}
crJXJUIGoldingotBuyUpdateNewMethod::crJXJUIGoldingotBuyUpdateNewMethod(const crJXJUIGoldingotBuyUpdateNewMethod& handle):
	crMethod(handle)
{
	for (int i = 0; i < 4; i++)
	{
		m_goodCount[i] = handle.m_goodCount[i];
		m_goodPrice[i] = handle.m_goodPrice[i];
		m_buyBtn[i] = handle.m_buyBtn[i];
		m_switch[i] = handle.m_switch[i];
		m_discountSwitch[i] = handle.m_discountSwitch[i];
	}
}
void crJXJUIGoldingotBuyUpdateNewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJUIGoldingotBuyUpdateNewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_goodCount[i] = str;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_goodPrice[i-4] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		m_buyBtn[i-8] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		m_switch[i-12] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		m_discountSwitch[i-16] = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
		m_buyCount[i-20] = str;
		break;
	}
}
void crJXJUIGoldingotBuyUpdateNewMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = m_this->getDataClass();

	if(data && playerGameData)
	{
		void *param;

		crStaticTextWidgetNode *goodsCount[4];
		crStaticTextWidgetNode *goodsPrice[4];
		crButtonWidgetNode* buyBtn[4];
		crMultiSwitch* multiSwitch[4];
		crMultiSwitch* discountSwitch[4];
		crStaticTextWidgetNode *buyCount[4];
		for (int i = 0; i < 4; ++i)
		{
			goodsCount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goodCount[i]));
			goodsPrice[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goodPrice[i]));
			buyBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_buyBtn[i]));
			multiSwitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch[i]));
			discountSwitch[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_discountSwitch[i]));
			buyCount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_buyCount[i]));
		}

		ref_ptr<crTableIO>shijiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShijiTab);
		int addIndex = shijiTab->getTitleIndex("市集增加量");
		ref_ptr<crTableIO>shiji2Tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiji2Tab);
		int goldIndex = shiji2Tab->getTitleIndex("元宝");
		int discountIndex = shiji2Tab->getTitleIndex("折扣");
		ref_ptr<crTableIO>resbuildingtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResBuildingTab);
		int foodproid = resbuildingtab->getTitleIndex("粮食");
		int woodproid = resbuildingtab->getTitleIndex("木材");
		int ironproid = resbuildingtab->getTitleIndex("铁矿");
		int horseproid = resbuildingtab->getTitleIndex("马匹");
		crTableIO::StrVec record,fukuRec,shijirecord,viprecord;
		int goldsum = 0;
		std::string str;
		crData *myData = playerGameData->getDataClass();
		myData -> getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;


		myData->getParam(WCHDATA_JXJShijilv,param);
		unsigned char shijilv = *(unsigned char *)param;
		float addResource;
		if (shijiTab->queryOneRecord(0,crArgumentParser::appItoa(shijilv),record)>=0)
		{
			addResource = atof(record[addIndex].c_str());
		}

		myData->getParam(WCHDATA_JXJBuyResourceCount,param);
		unsigned char count = *(unsigned char*)param;
		myData->getParam(WCHDATA_JXJFreeBuyInfo,param);
		unsigned char freeInfo = *(unsigned char *)param;

		crVector4f pricevec4f;
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
		float priceParam;
		int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceCount,viplv).c_str());
		for( int i = 0; i<4; i++)
		{
			if (buyCount[i])
			{
				buyCount[i]->setString(crArgumentParser::appItoa(maxCount-count+1) + "/" + crArgumentParser::appItoa(maxCount));
				buyCount[i]->setCanFocus(false);
			}
			if (count > maxCount)
			{
				buyBtn[i]->setEnable(false);
				continue;
			}else
			{
				buyBtn[i]->setEnable(true);
			}
			

			int startid = 0;
			int titleIndex ;
			int len = 4;
			switch (i)
			{
			case 0:
				startid = WCHDATA_JXJNongtianlv1;
				titleIndex = foodproid;
				len = 8;
				break;
			case 1:
				startid = WCHDATA_JXJLingdilv1;
				titleIndex = woodproid;
				break;
			case 2:
				startid = WCHDATA_JXJKuangshanlv1;
				titleIndex = ironproid;
				break;
			case 3:
				startid = WCHDATA_JXJMuchanglv1;
				titleIndex = horseproid;
				break;
			}
			float product = 0;
			priceParam = pricevec4f[i];
			for (int j = 0;j < len;j++,startid++)
			{
				myData->getParam(startid,param);
				unsigned char lv = *(unsigned char *)param;
				if (lv>0)
				{
					if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
					{
						product += atof(record[titleIndex].c_str()) ;
					}
				}
			}
			float marketParam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceParam,viplv).c_str());
			float total = marketParam*product+addResource/priceParam;
			if (int(total/100000) > 0)
			{
				goodsCount[i]->setString(crArgumentParser::appItoa(total/10000)+"万");
			}else
			{
				goodsCount[i]->setString(crArgumentParser::appItoa(total));
			}


			

			int needGold ;
			int discount = 0;
			if(shiji2Tab->queryOneRecord(0,crArgumentParser::appItoa(count),record)>=0)
			{
				needGold = atoi(record[goldIndex].c_str());
				discount = atoi(record[discountIndex].c_str());
			}
			
			if (freeInfo == 0)
			{
				needGold = 0;
			}

			if (discount == 0 || freeInfo == 0)
			{
				multiSwitch[i]->setActiveSwitchSet(1);
			}
			else
			{
				multiSwitch[i]->setActiveSwitchSet(0);
				discountSwitch[i]->setActiveSwitchSet(discount-1);
			}

			if (needGold)
			{
				goodsPrice[i]->setString(crArgumentParser::appItoa(needGold));
			}else
			{
				goodsPrice[i]->setString(std::string("免费"));
			}

			
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGoldBuyMethod
//
/////////////////////////////////////////
crJXJUIGoldBuyNewMethod::crJXJUIGoldBuyNewMethod():
	m_this(NULL){}
crJXJUIGoldBuyNewMethod::crJXJUIGoldBuyNewMethod(const crJXJUIGoldBuyNewMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_msgbox(handle.m_msgbox),
	m_title(handle.m_title),
	m_msg(handle.m_msg),
	m_ok(handle.m_ok),
	m_goldColor(handle.m_goldColor),
	m_index(handle.m_index)
{
}
void crJXJUIGoldBuyNewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIGoldBuyNewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msgbox = str;
		break;
	case 1:
		m_title = str;
		break;
	case 2:
		m_msg = str;
		break;
	case 3:
		m_ok = str;
		break;
	case 4:
		crArgumentParser::appAtoVec(str,m_goldColor);
		break;
	case 5:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJUIGoldBuyNewMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData* pPlayerData = myPlayer?myPlayer->getDataClass():NULL;
	void* param = NULL;
	crTableIO::StrVec record,fukuRec;
	if(pPlayerData && m_index>=0 && m_index<4)
	{
		ref_ptr<crTableIO>shijiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShijiTab);
		int addIndex = shijiTab->getTitleIndex("市集增加量");
		ref_ptr<crTableIO>shiji2Tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiji2Tab);
		int goldIndex = shiji2Tab->getTitleIndex("元宝");
		ref_ptr<crTableIO>resbuildingtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResBuildingTab);
		int foodproid = resbuildingtab->getTitleIndex("粮食");
		int woodproid = resbuildingtab->getTitleIndex("木材");
		int ironproid = resbuildingtab->getTitleIndex("铁矿");
		int horseproid = resbuildingtab->getTitleIndex("马匹");
		pPlayerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;

		pPlayerData->getParam(WCHDATA_JXJShijilv,param);
		unsigned char shijilv = *(unsigned char *)param;
		float addResource;
		if (shijiTab->queryOneRecord(0,crArgumentParser::appItoa(shijilv),record)>=0)
		{
			addResource = atof(record[addIndex].c_str());
		}

		int startid = 0;
		int titleIndex ;
		int len = 4;
		switch (m_index)
		{
		case 0:
			startid = WCHDATA_JXJNongtianlv1;
			titleIndex = foodproid;
			len = 8;
			break;
		case 1:
			startid = WCHDATA_JXJLingdilv1;
			titleIndex = woodproid;
			break;
		case 2:
			startid = WCHDATA_JXJKuangshanlv1;
			titleIndex = ironproid;
			break;
		case 3:
			startid = WCHDATA_JXJMuchanglv1;
			titleIndex = horseproid;
			break;
		}
		float product = 0.0f;
		for (int i = 0;i < len;i++,startid++)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			if (lv>0)
			{
				if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
				{
					product += atof(record[titleIndex].c_str()) ;
				}
			}
		}

		float marketParam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceParam,viplv).c_str());
		crVector4f pricevec4f;
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
		float priceParam = pricevec4f[m_index];
		int total = ceil(marketParam*product+addResource/priceParam);

		pPlayerData->getParam(WCHDATA_JXJBuyResourceCount,param);
		unsigned char count = *(unsigned char*)param;

		int needGold ;
		int discount = 0;
		if(shiji2Tab->queryOneRecord(0,crArgumentParser::appItoa(count),record)>=0)
		{
			needGold = atoi(record[goldIndex].c_str());
		}
		std::string goodsName;
		switch (m_index)
		{
		case 0:
			goodsName = "粮食";
			break;
		case 1:
			goodsName = "木材";
			break;
		case 2:
			goodsName = "铁矿";
			break;
		case 3:
			goodsName = "马匹";
			break;
		default:
			break;
		}
		int textId = 0;
		do 
		{
			int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceCount,viplv).c_str());
			if (count > maxCount)
			{
				textId = 2038;
				break;
			}

			pPlayerData->getParam(WCHDATA_JXJGoldingot,param);
			int mygold = *(int *)param;
			pPlayerData->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;
			mygold += giftgold;
			pPlayerData->getParam(WCHDATA_JXJFreeBuyInfo,param);
			unsigned char freeInfo = *(unsigned char *)param;
			if (freeInfo == 0)
			{
				needGold = 0;
			}
			if (mygold < needGold)
			{
				textId = 3032;
				break;
			}

			pPlayerData->getParam(WCHDATA_JXJFood+m_index,param);
			int mycount = *(int *)param;
			pPlayerData->getParam(WCHDATA_JXJFukulv,param);
			int fukulv = (int)(*(unsigned char *)param);
			ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);
			fukutab->queryOneRecord(0,crArgumentParser::appItoa(fukulv),fukuRec);
			float fukulimitaddpercent = 0.0f;
			myPlayer->doEvent(WCH_JXJGetFukuLimitAdd,MAKECREPARAM(&fukulimitaddpercent,NULL));
			float limit = atof(fukuRec[m_index+1].c_str()) * (1.0f + fukulimitaddpercent);
			if (mycount + total >limit)
			{
				textId = 2104;
				break;
			}

			pPlayerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			unsigned int ifhint = *(unsigned int *)param;
			int hinttype = JXJGoldCostType_MarketBuyResource;
			ifhint &= JXJGoldCostType_MarketBuyResource;

			ref_ptr<crCanvasNode> msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
			if(msgCanvas.valid())
			{
				crData *braindata = crBrain::getInstance()->getDataClass();
				braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

				crStaticTextWidgetNode *title = dynamic_cast<crStaticTextWidgetNode *>(msgCanvas->getWidget(m_title));
				crHypertextWidgetNode *msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msg));
				crButtonWidgetNode *okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_ok));
				
				std::vector<float>v_i;
				ItemMap itemMap;
				if(okBtn)okBtn->setEnable(true);
				if (ifhint)
				{
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					int nContent = 1;
					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					cfg_script.Add("Text","您确认要花费");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					v_i.clear();
					v_i.push_back(/*m_goldColor[0]*/140.0);
					v_i.push_back(/*m_goldColor[1]*/40.0);
					v_i.push_back(/*m_goldColor[2]*/40.0);
					v_i.push_back(/*m_goldColor[3]*/255.0);
					cfg_script.Add("Color",v_i);
					char tmpText[32];
					sprintf(tmpText,"%d礼金/元宝\0",needGold);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					cfg_script.Add("Text","来购买资源吗?\n\0");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					cfg_script.Add("Text","您将获得\0");
					cfg_script.Pop();

					sprintf(tmpText,"(%d)%s\0",total,goodsName.c_str());
					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",nContent++);
					cfg_script.Add("Text","实际以服务器计算为准。\0");
					cfg_script.Pop();
					cfg_script.Pop();
					msg->setHypertext(cfg_script);					
				}
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				
				if (ifhint)
				{
					crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				}
				else
				{
					if(okBtn)okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
				
			}
		} while (false);

		if(textId>0)
		{
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&textId);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		}

	}
}
/////////////////////////////////////////
//
//crJXJUIGoldBuy2Method
//
/////////////////////////////////////////
crJXJUIGoldBuy2NewMethod::crJXJUIGoldBuy2NewMethod():
	m_this(NULL)
{
}
crJXJUIGoldBuy2NewMethod::crJXJUIGoldBuy2NewMethod(const crJXJUIGoldBuy2NewMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{
}
void crJXJUIGoldBuy2NewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIGoldBuy2NewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJUIGoldBuy2NewMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeChar(m_index);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGoldBuy,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);

	}
}
/////////////////////////////////////////
//
//crJXJRecvGoldBuyNewMethod
//
/////////////////////////////////////////
crJXJRecvGoldBuyNewMethod::crJXJRecvGoldBuyNewMethod():
	m_netType(GameClient_Game){}
crJXJRecvGoldBuyNewMethod::crJXJRecvGoldBuyNewMethod(const crJXJRecvGoldBuyNewMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGoldBuyNewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGoldBuyNewMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGoldBuyNewMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		int playerid = m_this->getPlayerID();
		crData *data = m_this->getDataClass();
		crRole *mainRole = m_this->getMainRole();
		void* param = NULL;
		if(m_netType == GameServer)
		{
			int index = (int)(m_stream->_readChar());
			if (index <0 || index >3)
			{
				return;
			}

			crTableIO::StrVec record,fukuRec;
			ref_ptr<crTableIO>shijiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShijiTab);
			int addIndex = shijiTab->getTitleIndex("市集增加量");
			ref_ptr<crTableIO>shiji2Tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiji2Tab);
			int goldIndex = shiji2Tab->getTitleIndex("元宝");
			ref_ptr<crTableIO>resbuildingtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResBuildingTab);
			int foodproid = resbuildingtab->getTitleIndex("粮食");
			int woodproid = resbuildingtab->getTitleIndex("木材");
			int ironproid = resbuildingtab->getTitleIndex("铁矿");
			int horseproid = resbuildingtab->getTitleIndex("马匹");

			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJFood+index,param);
			int* mycount = (int *)param;
			data->getParam(WCHDATA_JXJFukulv,param);
			int fukulv = (int)(*(unsigned char *)param);
			data->getParam(WCHDATA_JXJBuyResourceCount,param);
			unsigned char count = *(unsigned char*)param;
			data->getParam(WCHDATA_JXJGoldingot,param);
			int curgold = *(int *)param;
			data->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;

			int needGold ;
			float total =0.0f;
			bool needdeductgold = false;
			data->getParam(WCHDATA_JXJFreeBuyInfo,param);
			unsigned char freeInfo = *(unsigned char*)param;
			int code = 0;
			do 
			{
				//购买次数
				int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceCount,viplv).c_str());
				if (count > maxCount)
				{
					code = 1;//达到购买次数上限
					break;
				}

				//元宝检查
				curgold += giftgold;
				if(shiji2Tab->queryOneRecord(0,crArgumentParser::appItoa(count),record)>=0)
				{
					needGold = atoi(record[goldIndex].c_str());
				}
				if (needGold>curgold && freeInfo)
				{
					code = 2;//元宝不足
					break;
				}

				//府库上限检查
				ref_ptr<crTableIO>fukutab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFukuTab);				
				fukutab->queryOneRecord(0,crArgumentParser::appItoa(fukulv),fukuRec);
				float fukulimitaddpercent = 0.0f;
				m_this->doEvent(WCH_JXJGetFukuLimitAdd,MAKECREPARAM(&fukulimitaddpercent,NULL));
				float limit = atof(fukuRec[index+1].c_str()) * (1.0f + fukulimitaddpercent);

				int startid = 0;
				int titleIndex ;
				int len = 4;
				switch (index)
				{
				case 0:
					startid = WCHDATA_JXJNongtianlv1;
					titleIndex = foodproid;
					len = 8;
					break;
				case 1:
					startid = WCHDATA_JXJLingdilv1;
					titleIndex = woodproid;
					break;
				case 2:
					startid = WCHDATA_JXJKuangshanlv1;
					titleIndex = ironproid;
					break;
				case 3:
					startid = WCHDATA_JXJMuchanglv1;
					titleIndex = horseproid;
					break;
				}
				float product = 0.0f;
				for (int i = 0;i < len;i++,startid++)
				{
					data->getParam(startid,param);
					unsigned char lv = *(unsigned char *)param;
					if (lv>0)
					{
						if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
						{
							product += atof(record[titleIndex].c_str()) ;
						}
					} 
				}
				data->getParam(WCHDATA_JXJShijilv,param);
				unsigned char shijilv = *(unsigned char *)param;
				float addResource;
				if (shijiTab->queryOneRecord(0,crArgumentParser::appItoa(shijilv),record)>=0)
				{
					addResource = atof(record[addIndex].c_str());
				}
				float marketParam = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceParam,viplv).c_str());
				crVector4f pricevec4f;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResourcePriceVec, 0).c_str(), pricevec4f);
				float priceParam = pricevec4f[index];

				total = marketParam*product+addResource/priceParam;
				if (*mycount + total >limit)
				{
					code = 3;//超府库上限
					break;
				}

			} while (0);


			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(code == 0 && playerData.valid())
			{				
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(7);
				stream->_writeUChar(index);//1
				*mycount += ceil(total);
				stream->_writeInt(*mycount);//4

				//增加活跃度
				m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKECREPARAM(HYD_UpgradeCard, 1));

				if (freeInfo == 0)
				{
					stream->_writeBool(true);
					freeInfo = 1;
					data->inputParam(WCHDATA_JXJFreeBuyInfo,&freeInfo);
					stream->_writeUChar(freeInfo);
					needGold = 0;
				}
				else
				{
					stream->_writeBool(false);
					count++;
					data->inputParam(WCHDATA_JXJBuyResourceCount,&count);
					stream->_writeUChar(count);
				}

				if (needGold > 0)
				{
					MoneyChangeData moneydata(needGold,"市集购买资源");
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKECREPARAM(&moneydata,&needdeductgold));
				}

				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				if (needdeductgold)
				{
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGoldBuy,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = "元宝购买资源（id，type，count）：" + crArgumentParser::appVectoa(crVector3i(0,WCHDATA_JXJFood+index,total));
				GameLogData gamelog(Log_MarketGoldBuy,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
			}
			else
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int index = (int)m_stream->_readChar();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFood+index,param);
			int* myCount = (int *)param;
			int serverCount = m_stream->_readInt();
			int gainCount = serverCount - *myCount;
			*myCount = serverCount;

			crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Food+index,gainCount));

			bool isFree = m_stream->_readBool();
			if (isFree)
			{
				unsigned char freeInfo = m_stream->_readUChar();
				data->inputParam(WCHDATA_JXJFreeBuyInfo,&freeInfo);
			}else
			{
				unsigned char count = m_stream->_readUChar();
				data->inputParam(WCHDATA_JXJBuyResourceCount,&count);
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJBattleRecruitCheckNewMethod
//
/////////////////////////////////////////
crJXJBattleRecruitCheckNewMethod::crJXJBattleRecruitCheckNewMethod():
	m_this(NULL),
	m_result(NULL)
{
}

crJXJBattleRecruitCheckNewMethod::crJXJBattleRecruitCheckNewMethod(const crJXJBattleRecruitCheckNewMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_result(NULL)
{
}

void crJXJBattleRecruitCheckNewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_recruitTroopsMap = (TroopsMap*)(LOCREPARAM(param64));
			m_result = (BattleRecruitCheckResult *)(HICREPARAM(param64));
		}
		else
		{
			m_recruitTroopsMap = NULL;
			m_result = NULL;
		}
		break;
	}
}

void crJXJBattleRecruitCheckNewMethod::addParam(int i, const std::string& str)
{
}

void crJXJBattleRecruitCheckNewMethod::operator()(crHandle &handle)
{
	if(m_result && m_recruitTroopsMap)
	{
		m_result->m_code = -1;//不能补兵
		void *param;
		crData *data = m_this->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJFood,param);
		int myfood = *(int *)param;

		if(m_recruitTroopsMap->empty())
		{
			if(vipLevel == 0)
			{
				int addArmyNeedExtraFood = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleAddArmyExtraFoodNeed, vipLevel).c_str()));
				if(myfood>=addArmyNeedExtraFood)
				{
					m_result->m_code = 0;//兵源充足VIP0补兵需要额外粮草
					m_result->m_foodneed = addArmyNeedExtraFood;
					//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&addArmyNeedExtraFood);
				}
				else
					m_result->m_code = -2;//粮草不足
			}
			else
			{
				m_result->m_code = 1;//兵源充足
			}
		}
		else
		{
			data->getParam(WCHDATA_JXJCoppercash,param);
			int mycopper = *(int *)param;
			data->getParam(WCHDATA_JXJWood,param);
			int mywood = *(int *)param;
			data->getParam(WCHDATA_JXJIron,param);
			int myiron = *(int *)param;
			data->getParam(WCHDATA_JXJHorse,param);
			int myhorse = *(int *)param;
			data->getParam(WCHDATA_JXJGoldingot,param);
			int myGold = *(int *)param;
			data->getParam(WCHDATA_JXJGiftGold,param);
			myGold+=(*(int *)param);
			data->getParam(WCHDATA_JXJTroopsMap,param);
			TroopsMap *troops = (TroopsMap *)param;

			if(vipLevel > 1)
			{
				int copperneed=0,
					woodneed = 0,
					ironneed = 0,
					horseneed = 0,
					foodneed = 0,
					timeneed = 0;
				int	equipneed = 0,
					equipid = 0;
				int percopperneed = 0;
				int perwoodneed = 0;
				int perironneed = 0;
				int perhorseneed = 0;
				int perfoodneed = 0;
				int pertimeneed = 0;

				ref_ptr<crTableIO>troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
				ref_ptr<crTableIO>equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
				short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime, vipLevel).c_str()));
				int credittime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime, vipLevel).c_str()));

				crTableIO::StrVec record,record2;
				TroopsMap::iterator itr = m_recruitTroopsMap->begin();
				int leaderneedindex = troopTab->getTitleIndex("领导力");

				int equipcopperindex = equipTab->getTitleIndex("铜钱");
				int equipfoodindex = equipTab->getTitleIndex("粮食");
				int equiphorseindex = equipTab->getTitleIndex("马匹");
				int equipironindex = equipTab->getTitleIndex("铁矿");
				int equipwoodindex = equipTab->getTitleIndex("木材");
				while (itr!=m_recruitTroopsMap->end())
				{
					RecruitCheckResult recruitCheck;
					m_this->doEvent(WCH_JXJRecruitCheck,MAKECREPARAM(itr->first,&recruitCheck));
					if(recruitCheck.m_code!=1)
					{
						m_result->m_code = -12;//不能招募
						break;
					}
					copperneed += ceil((float)(itr->second)*recruitCheck.m_needresourcevec[T_Copper - 1]);
					foodneed += ceil((float)(itr->second)*recruitCheck.m_needresourcevec[T_Food - 1]);
					woodneed += ceil((float)(itr->second)*recruitCheck.m_needresourcevec[T_Wood - 1]);
					ironneed += ceil((float)(itr->second)*recruitCheck.m_needresourcevec[T_Iron - 1]);
					horseneed += ceil((float)(itr->second)*recruitCheck.m_needresourcevec[T_Horse - 1]);
					timeneed += ceil((float)(itr->second)*recruitCheck.m_timeNeed);
					//timeneed += itr->second*pertimeneed;
					//totalLeaderCount += itr->second*perleaderneed;

					if(equipTab->queryOneRecord(0,crArgumentParser::appItoa(recruitCheck.m_equipid),record2)>=0)
					{
						if(equipcopperindex>0)
							percopperneed = atoi(record2[equipcopperindex].c_str());
						if(equipfoodindex>0)
							perfoodneed = atoi(record2[equipfoodindex].c_str());
						if(equiphorseindex>0)
							perhorseneed = atoi(record2[equiphorseindex].c_str());
						if(equipironindex>0)
							perironneed = atoi(record2[equipironindex].c_str());
						if(equipwoodindex)
							perwoodneed = atoi(record2[equipwoodindex].c_str());
					}
					copperneed += percopperneed * recruitCheck.m_equipCount *itr->second;
					woodneed += perwoodneed * recruitCheck.m_equipCount *itr->second;
					ironneed += perironneed * recruitCheck.m_equipCount * itr->second;
					horseneed += perhorseneed * recruitCheck.m_equipCount * itr->second;
					foodneed += perfoodneed * recruitCheck.m_equipCount *itr->second;

					itr++;
				}
				if(m_result->m_code!=-12)
				{//不能招募
					data->getParam(WCHDATA_JXJRecruitTimes,param);
					crVector4i recruitTimes = *(crVector4i *)param;

					int i = 0;
					int timeRemain = 0;
					for(; i<4; i++)
					{
						if(recruitTimes[i]>=0 && recruitTimes[i] < overTimes)
							timeRemain += overTimes - recruitTimes[i] + credittime;
					}
					int needVIP = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGoldRecruitVIPLevel,vipLevel).c_str());
					if(copperneed > mycopper)
					{//铜钱不足
						m_result->m_code = -6;//铜钱不足
					}
					else if(woodneed > mywood || ironneed > myiron || horseneed > myhorse || foodneed > myfood || timeneed > timeRemain)
					{
						if(vipLevel>= needVIP)
						{
							int goldneed = 0;
							//够买时间
							if(timeneed>timeRemain)
							{//
								ref_ptr<crTableIO>hintTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHintTab);
								crTableIO::StrVec record;
								hintTable->queryOneRecord(0,crArgumentParser::appItoa(WCHDATA_JXJRecruitCDHint),record);
								goldneed += ceil((float)(timeneed - timeRemain) * atof(record[2].c_str()));

							}
							//够买资源
							int needbuycount = woodneed > mywood?woodneed - mywood:0;
							needbuycount += ironneed > myiron?ironneed - myiron:0;
							needbuycount += horseneed > myhorse?horseneed - myhorse:0;
							needbuycount += foodneed > myfood?foodneed - myfood:0;
							if(needbuycount>0)
							{
								ref_ptr<crTableIO>resbuildingtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResBuildingTab);
								int foodproid = resbuildingtab->getTitleIndex("粮食");
								int woodproid = resbuildingtab->getTitleIndex("木材");
								int ironproid = resbuildingtab->getTitleIndex("铁矿");
								int horseproid = resbuildingtab->getTitleIndex("马匹");

								float marketParam = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceParam,vipLevel).c_str());

								int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGoldRecruitLimit,vipLevel).c_str());
								data->getParam(WCHDATA_JXJGoldRecruitCount,param);
								unsigned char goldRecruitCount = *(unsigned char*)param;

								if(goldRecruitCount>=maxCount)
								{
									m_result->m_code = -7;//今日可购资源量不足，不能购买！ 元宝复活次数已用完
								}
								else
								{//购买资源，需要做元宝修正
									do 
									{
										ref_ptr<crTableIO>shiji2Tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiji2Tab);
										int goldIndex = shiji2Tab->getTitleIndex("元宝");
										int discountIndex = shiji2Tab->getTitleIndex("折扣");
										int unitPrice = 100;
										if(shiji2Tab->queryOneRecord(discountIndex,"0",record) >=0)
										{
											unitPrice = atoi(record[goldIndex].c_str());
										}
										ref_ptr<crTableIO>goldRecruitTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGoldRecruitTab);
										goldIndex = goldRecruitTab->getTitleIndex("元宝");
										int addPrice = 0;
										if(goldRecruitTab->queryOneRecord(0,crArgumentParser::appItoa(goldRecruitCount),record) >=0)
										{
											addPrice = atoi(record[goldIndex].c_str());
										}
										int startid = 0;
										float product = 0.0f;
										float total = 0.0f;
										float buycount = foodneed - myfood;
										if(buycount>0)
										{
											startid = WCHDATA_JXJNongtianlv1;
											for (int j = 0;j < 8;j++,startid++)
											{
												data->getParam(startid,param);
												unsigned char lv = *(unsigned char *)param;
												if (lv>0)
												{
													if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
													{
														product += atof(record[foodproid].c_str()) ;
													}
												}
											}

											total = marketParam*product;
											goldneed += ceil(buycount/total *(float)(unitPrice+addPrice));
										}

										buycount = woodneed - mywood;
										product = 0.0f;
										total = 0.0f;
										if(buycount>0)
										{
											startid = WCHDATA_JXJLingdilv1;
											for (int j = 0;j < 4;j++,startid++)
											{
												data->getParam(startid,param);
												unsigned char lv = *(unsigned char *)param;
												if (lv>0)
												{
													if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
													{
														product += atof(record[woodproid].c_str()) ;
													}
												}
											}
											total = marketParam*product;
											goldneed += ceil(buycount/total *(float)(unitPrice+addPrice));
										}
										buycount = ironneed - myiron;
										product = 0.0f;
										total = 0.0f;
										if(buycount>0)
										{
											startid = WCHDATA_JXJKuangshanlv1;
											for (int j = 0;j < 4;j++,startid++)
											{
												data->getParam(startid,param);
												unsigned char lv = *(unsigned char *)param;
												if (lv>0)
												{
													if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
													{
														product += atof(record[ironproid].c_str()) ;
													}
												}
											}
											total = marketParam*product;
											goldneed += ceil(buycount/total *(float)(unitPrice+addPrice));
										}
										buycount = horseneed - myhorse;
										product = 0.0f;
										total = 0.0f;
										if(buycount>0)
										{
											startid = WCHDATA_JXJMuchanglv1;
											for (int j = 0;j < 4;j++,startid++)
											{
												data->getParam(startid,param);
												unsigned char lv = *(unsigned char *)param;
												if (lv>0)
												{
													if(resbuildingtab->queryOneRecord(0,crArgumentParser::appItoa((int)lv),record) >= 0)
													{
														product += atof(record[horseproid].c_str()) ;
													}
												}
											}
											total = marketParam*product;
											goldneed += ceil(buycount/total *(float)(unitPrice+addPrice));
										}
									} while (0);
									if(m_result->m_code!=-8)
									{
										int Min = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleRecruitGoldMin,vipLevel).c_str());
										int Max = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleRecruitGoldMax,vipLevel).c_str());
										if (goldneed < Min && goldneed > 0)
										{
											goldneed = Min;
										}
										if (goldneed > Max)
										{
											goldneed = Max;
										}
										if(goldneed<=myGold)
										{
											m_result->m_code = 3;//元宝补兵
											m_result->m_goldneed = goldneed;
											m_result->m_shijibuycount = needbuycount;

											m_result->m_copperneed = copperneed;
											m_result->m_foodneed = foodneed > myfood?myfood:foodneed;
											m_result->m_woodneed = woodneed > mywood?mywood:woodneed;
											m_result->m_ironneed = ironneed > myiron?myiron:ironneed;
											m_result->m_horseneed = horseneed > myhorse?myhorse:horseneed;
											m_result->m_timeneed = timeneed>timeRemain?timeRemain:timeneed;

											//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedGold,&goldneed);
										}
										else
											m_result->m_code = -3;//元宝不足
									}
								}
							}
						}
						else
							m_result->m_code = -4;//资源不足
					}
					else
					{
						m_result->m_code = 2;//兵力不足，资源充足，直接扣除资源补兵；
						m_result->m_copperneed = copperneed;
						m_result->m_foodneed = foodneed;
						m_result->m_woodneed = woodneed;
						m_result->m_ironneed = ironneed;
						m_result->m_horseneed = horseneed;
						//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedCopper,&copperneed);
						//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&foodneed);
						//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedWood,&woodneed);
						//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedIron,&ironneed);
						//canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedHorse,&horseneed);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIMarketBuyLimitTipsMethod
//
/////////////////////////////////////////
crJXJUIMarketBuyLimitTipsMethod::crJXJUIMarketBuyLimitTipsMethod()
{
}
crJXJUIMarketBuyLimitTipsMethod::crJXJUIMarketBuyLimitTipsMethod(const crJXJUIMarketBuyLimitTipsMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void crJXJUIMarketBuyLimitTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIMarketBuyLimitTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	}
}

void crJXJUIMarketBuyLimitTipsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData* pPlayerData = myPlayer?myPlayer->getDataClass():NULL;
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(pPlayerData && tips.valid())
	{
		tips->setCanFocus(false);
		crStaticTextWidgetNode *tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipsTextWidget));
		std::string str;
		///////////

		void *param;
		pPlayerData->getParam(WCHDATA_JXJBuyResourceCount,param);
		unsigned char count = *(unsigned char*)param;
		pPlayerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		int maxCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMarketResourceCount,vipLevel).c_str());
		if (count > maxCount)
		{
			str = "今日购买次数已耗尽";
		}else
		{
			str = "提高vip等级可提升购买次数";
		}

		tipsText->setString(str);
		tipsText->setColor(FCOLOR_RED);

		if(!str.empty())
		{
			const crBoundingBox &btnbox = m_this->getBoundBox();
			//
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1]+1.5f*(btnbox.m_max[1]-btnbox.m_min[1]));
			//
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
		}
	}
}


/////////////////////////////////////////
//
//crJXJTXMarketTaskDisposeMethod
//
/////////////////////////////////////////
crJXJTXMarketTaskDisposeMethod::crJXJTXMarketTaskDisposeMethod():
	m_this(NULL),
	m_count(50){}
crJXJTXMarketTaskDisposeMethod::crJXJTXMarketTaskDisposeMethod(const crJXJTXMarketTaskDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_count(handle.m_count)
{
}
void crJXJTXMarketTaskDisposeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	}
}
void crJXJTXMarketTaskDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	}
}
void crJXJTXMarketTaskDisposeMethod::operator()(crHandle &handle)
{
	bool success = false;
	ref_ptr<crGameServerPlayerData> playerData;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	int serverid = callback->getServerID();
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crTXMarketTaskQueryData> marketTaskQuery = new crTXMarketTaskQueryData;
	marketTaskQuery->buildQuerySql(serverid,m_count);
	globalSession->executeQuery(marketTaskQuery.get());
	crDataBase::QueryResultVec marketTaskQueryResultVec = globalSession->getQueryResult();
	globalSession->releaseQuery();
	db->endSession(globalSession.get());

	ref_ptr<crCheckPayMoneyCountData> checkpaymoneycount;
	crDataBase::QueryResultVec checkpaymoneycountResultVec;
	if(!marketTaskQueryResultVec.empty())
	{
		ref_ptr<crTableIO> txTaskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTXMarketTaskTab);
		int rewardIndex = txTaskTab->getTitleIndex("奖励");
		crTableIO::StrVec record;
		CRCore::crVector4i rewardVec;
		int playerid = 0;
		int id = 0;
		int step = 0;
		std::string taskID;
		std::string ordernumber;
		crPlayerDataEventPacket packet;
		ref_ptr<crStreamBuf>stream;
		ref_ptr<crJXJRemoveTXMarketTask> removeTXTask = new crJXJRemoveTXMarketTask;
		ref_ptr<crJXJInsertTXTaskrecord> insertTXTaskrecord = new crJXJInsertTXTaskrecord;

		for( crDataBase::QueryResultVec::iterator itr = marketTaskQueryResultVec.begin();
			itr != marketTaskQueryResultVec.end();
			++itr )
		{
			marketTaskQuery = dynamic_cast<crTXMarketTaskQueryData *>(itr->get());
			playerid = marketTaskQuery->getPlayerID();
			step = marketTaskQuery->getStep();
			id = marketTaskQuery->getID();
			taskID = marketTaskQuery->getTaskID();
			
			globalSession = db->beginSession();
			insertTXTaskrecord->buildUpdateSql(playerid,step,taskID,serverid,2);
			removeTXTask->buildUpdateSql(id);
			if (globalSession->executeUpdate(insertTXTaskrecord.get()) && globalSession->executeUpdate(removeTXTask.get()))
			{
				globalSession->commit();
				success = true;
			}
			else
			{
				globalSession->rollback();
				success = false;
			}
			db->endSession(globalSession.get());

			rewardVec.set(0,0,0,0);
			if (txTaskTab->queryOneRecord(0,taskID,1,crArgumentParser::appItoa(step),record) >= 0)
			{
				crArgumentParser::appAtoVec(record[rewardIndex],rewardVec);
			}
			else
				continue;

			if (success)
			{
				ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
				ref_ptr<crBagItemData> itemdata;
				switch (rewardVec[0])
				{
				case T_Item:
					{
						itemdata = new crBagItemData;
						itemdata->setItemID(rewardVec[1]);
						itemdata->setEquipMagic(rewardVec[2]);
						itemdata->setItemCount(rewardVec[3]);
						reward_info->getRewardItemsVec().push_back(itemdata);
					}
					break;
				default:
					break;
				}
				reward_info->setPlayerID(playerid);
				reward_info->setType(GP_RewardTxMarketTask);

				playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
				if(playerData.valid() && playerData->getPlayerGameData())
				{
					crData *pData = playerData->getPlayerGameData()->getDataClass();
					pData->excHandle(MAKECREPARAM(WCH_LockData,1));
					playerData->getPlayerGameData()->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
					pData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				else
				{
					m_this->doEvent(WCH_JXJCreateRewardMail,MAKECREPARAM(reward_info.get(),NULL));
				}
			}
			///游戏日志
			std::string logdata = "集市任务奖励（taskid,step）："+taskID+","+crArgumentParser::appItoa(step);
			GameLogData gamelog(Log_TXMarket,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUpdateTXMarketTaskMethod
//
/////////////////////////////////////////
crJXJUpdateTXMarketTaskMethod::crJXJUpdateTXMarketTaskMethod()
{}
crJXJUpdateTXMarketTaskMethod::crJXJUpdateTXMarketTaskMethod(const crJXJUpdateTXMarketTaskMethod& handle)
{
}
void crJXJUpdateTXMarketTaskMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	}
}
void crJXJUpdateTXMarketTaskMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = str;
		break;
	default:
		break;
	}
}
void crJXJUpdateTXMarketTaskMethod::operator()(crHandle &handle)
{
	void *param;
	crRole *mainRole = m_this->getMainRole();
	crData* pData = m_this->getDataClass();
	if (mainRole && mainRole->getDataClass() && pData)
	{
		crData *maindata = mainRole->getDataClass();

		ref_ptr<crTableIO> txTaskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTXMarketTaskTab);
		int conditionIndex = txTaskTab->getTitleIndex("条件");
		int stepIndex  = txTaskTab->getTitleIndex("步骤");
		int typeIndex = txTaskTab->getTitleIndex("类型");
		crTableIO::StrVec record;

		crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
		ref_ptr<crDataBase> globalSession;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int serverid = callback->getServerID();
		int playerid = m_this->getPlayerID();

		crTableIO::DataVec dataVec;
		txTaskTab->queryRecords(typeIndex,m_type,dataVec);
		int condition;
		bool flag,found;
		int type = atoi(m_type.c_str());
		int step;
		ref_ptr<crTXMarketTaskRecordQueryData> marketRecordTaskQuery = new crTXMarketTaskRecordQueryData;
		ref_ptr<crJXJInsertTXTaskrecord> insertTXTaskrecord = new crJXJInsertTXTaskrecord;
		for (int i = 0; i < dataVec.size(); i++)
		{
			record = dataVec[i];
			condition = atoi(record[conditionIndex].c_str());
			flag = false;
			switch (type)
			{
			case TXTask_Level:
				{
					maindata->getParam(WCHDATA_Level,param);
					unsigned char mylv = *(unsigned char *)param;
					if (mylv >= condition)
					{
						flag = true;
					}
				}
				break;
			case TXTask_CountryBattle:
				{
					pData->getParam(WCHDATA_JXJCountryBattleTimes,param);
					unsigned short battleTimes = *(unsigned short*)param;
					if (battleTimes >= condition)
					{
						flag = true;
					}
				}
				break;
			default:
				break;
			}
			if (flag)
			{
				globalSession = db->beginSession();
				marketRecordTaskQuery->buildQuerySql(playerid,serverid,record[0]);
				globalSession->executeQuery(marketRecordTaskQuery.get());
				crDataBase::QueryResultVec& marketTaskRecordQueryResultVec = globalSession->getQueryResult();
				if (!marketTaskRecordQueryResultVec.empty())
				{//表示有这个任务，但是该步骤没有的时候插入
					found = false;
					step = atoi(record[stepIndex].c_str());
					for( crDataBase::QueryResultVec::iterator itr = marketTaskRecordQueryResultVec.begin();
						 itr != marketTaskRecordQueryResultVec.end();
						 ++itr )
					{
						marketRecordTaskQuery = dynamic_cast<crTXMarketTaskRecordQueryData *>(itr->get());
						if(marketRecordTaskQuery->getStep() == step)
						{
							found = true;
							break;
						}
					}
					if(!found)
					{
						insertTXTaskrecord->buildUpdateSql_task(record[0],playerid,serverid,step);
						if (globalSession->executeUpdate(insertTXTaskrecord.get()))
						{
							globalSession->commit();
						}
					}
				}
				globalSession->releaseQuery();
				db->endSession(globalSession.get());
			}
		}
	}

}

//////////////////////////////////////////////////////////////////////////
//crJXJUIHttpRecordMethod
//////////////////////////////////////////////////////////////////////////

crJXJUIHttpRecordMethod::crJXJUIHttpRecordMethod()
{

}

crJXJUIHttpRecordMethod::crJXJUIHttpRecordMethod(const crJXJUIHttpRecordMethod& handle)
{

}

void crJXJUIHttpRecordMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIHttpRecordMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = str;
		break;
	case 1:
		m_param = str;
		break;
	case 2:
		m_url = str;
		break;
	default:
		break;
	}
}

void crJXJUIHttpRecordMethod::operator ()(crHandle &handle)
{
	//if (!CRIOManager::fileExists("web.ini"))
	if(crMyPlayerData::getInstance()->getSelectedGame()->getPlatform().find("QQ") == std::string::npos)
		return;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	int serverid = crMyPlayerData::getInstance()->getLastServerID();
	int playerID = myPlayer?myPlayer->getPlayerID():0;
	
	HINTERNET hIntSession = InternetOpen("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	HINTERNET hHttpSession = InternetConnect(hIntSession, m_url.c_str(), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);
	std::string str = "index.php/index/gamerecord?playerid="+crArgumentParser::appItoa(playerID)+"&type="+m_type+"&value="+m_param+"&serverid="+crArgumentParser::appItoa(serverid);
	HINTERNET hHttpRequest = HttpOpenRequest(
		hHttpSession, 
		"GET", 
		str.c_str(),
		0, 0, 0, INTERNET_FLAG_RELOAD, 0);

	char* szHeaders = "Content-Type: text/html";
	char szReq[64] = "";
	if( !HttpSendRequest(hHttpRequest, szHeaders, strlen(szHeaders), szReq, strlen(szReq))) 
	{
		DWORD dwErr = GetLastError();
		/// handle error
	}

	::InternetCloseHandle(hHttpRequest);
	::InternetCloseHandle(hHttpSession);
	::InternetCloseHandle(hIntSession);
	
}

/////////////////////////////////////////
//
//crJXJCountryBattleTimesMethod
//
/////////////////////////////////////////
crJXJCountryBattleTimesMethod::crJXJCountryBattleTimesMethod():
	m_this(NULL)
{
}
crJXJCountryBattleTimesMethod::crJXJCountryBattleTimesMethod(const crJXJCountryBattleTimesMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCountryBattleTimesMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	}
}
void crJXJCountryBattleTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJCountryBattleTimesMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		crData *data = myPlayer->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJCountryBattleTimes,param);
		unsigned short battleTimes = *(unsigned short *)param;
		m_this->setStep(battleTimes);
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

////////////////////////////////////////////////
//
//crJXJCheckGuoZhanAttackVictoryTimesMethod
//国战攻城胜利次数
///////////////////////////////////////////////
crJXJCheckCountryBattleTimesMethod::crJXJCheckCountryBattleTimesMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJCheckCountryBattleTimesMethod::crJXJCheckCountryBattleTimesMethod(const crJXJCheckCountryBattleTimesMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJCheckCountryBattleTimesMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_player = NULL;
		}
		break;
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_player = (crPlayerGameData *)(LOCREPARAM(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJCheckCountryBattleTimesMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckCountryBattleTimesMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		crData *data = m_player->getDataClass();
		void *param;
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJCountryBattleTimes,param);
		unsigned short battleTimes = *(unsigned short *)param;
		if (!m_this->isComplete())
		{
			m_this->setStep(battleTimes);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJUITrainningCopyCheckMethod
//
/////////////////////////////////////////
crJXJUITrainningCopyCheckMethod::crJXJUITrainningCopyCheckMethod():
	m_this(NULL)
{
}
crJXJUITrainningCopyCheckMethod::crJXJUITrainningCopyCheckMethod(const crJXJUITrainningCopyCheckMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJUITrainningCopyCheckMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUITrainningCopyCheckMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJUITrainningCopyCheckMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData* pPlayerData = myPlayer->getDataClass();
		crRole* pMainRole = myPlayer->getMainRole();
		crData *myMetierData = pMainRole?pMainRole->getMetierDataClass():NULL;
		if (pPlayerData && myMetierData)
		{
			int taskID = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCityBattleTaskID,0).c_str());
			myMetierData->excHandle(MAKECREPARAM(WCH_LockData,1));
			myMetierData->getParam(WCHDATA_GameTaskMap,param);
			GameTaskMap *gameTaskMap = (GameTaskMap *)param;
			GameTaskMap::iterator it = gameTaskMap->find(taskID);
			unsigned short id = 0;
			unsigned char type = FT_Fuben;
			bool cancelTask = true;
			if (it == gameTaskMap->end() || !it->second->getActivation())
			{
				cancelTask = false;
			}
			myMetierData->excHandle(MAKECREPARAM(WCH_LockData,0));
			if (cancelTask)
			{
				id = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTrainingCopyID,0).c_str());
				pPlayerData->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
				pPlayerData->inputParam(WCHDATA_JXJFubenOrChengchiID,&id);
			}
			handle.outputParam(0,&cancelTask);
		}
	}
}

/////////////////////////////////////////
//
//crJXJNewGuideTrainningCopyMethod
//
/////////////////////////////////////////
crJXJNewGuideTrainningCopyMethod::crJXJNewGuideTrainningCopyMethod()
{
}
crJXJNewGuideTrainningCopyMethod::crJXJNewGuideTrainningCopyMethod(const crJXJNewGuideTrainningCopyMethod& handle):
	crMethod(handle)
{
}
void crJXJNewGuideTrainningCopyMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crGameTask*)param;
		break;
	}
}
void crJXJNewGuideTrainningCopyMethod::addParam(int i, const std::string& str)
{

}
void crJXJNewGuideTrainningCopyMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	crData* pPlayerData = player?player->getDataClass():NULL;
	if(player && pPlayerData)
	{
		int copyID = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTrainingCopyID,0).c_str());

		pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
		pPlayerData->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
		FubenProgressMap::iterator it = fubenProgress->find(copyID);
		if (it!=fubenProgress->end() && it->second->getComplete())
		{
			player->doEvent(WCH_JXJAdvanceTask,MAKECREPARAM(m_this->getTaskID(),0));
		}

		pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));

	}
}

/////////////////////////////////////////
//
//crJXJCheckPresentSoldierMethod
//
/////////////////////////////////////////
crJXJCheckPresentSoldierMethod::crJXJCheckPresentSoldierMethod():
	m_nowlv(0),
	m_lastlv(0)
{
}
crJXJCheckPresentSoldierMethod::crJXJCheckPresentSoldierMethod(const crJXJCheckPresentSoldierMethod& handle):
	crMethod(handle)
{
}
void crJXJCheckPresentSoldierMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_lastlv = LOCREPARAM(param64);
			m_nowlv = HICREPARAM(param64);
			
		}
		else
		{
			m_lastlv = 0;
			m_nowlv = 0;
		}
		break;
	}
}
void crJXJCheckPresentSoldierMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckPresentSoldierMethod::operator()(crHandle &handle)
{
	void *param;
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> player = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(player.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
	{
		crData *playerData = m_this->getDataClass();
		crRole *mainRole = m_this->getMainRole();
		crData *mainRoleData = mainRole->getDataClass();
		mainRoleData->getParam(WCHDATA_Level,param);
		unsigned char mylv = *(unsigned char *)param;
		crTableIO::StrVec record;
		std::vector<int> praiselvvec;
		int praiselv = 0;
		ref_ptr<crTableIO>presentSoldierTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPresentSoldierTab);
		int rowcount = presentSoldierTab->getRowCount();
		for (int i=0; i<rowcount; i++)
		{
			praiselv = atoi(presentSoldierTab->getData(i,0).c_str());
			if(praiselv > m_lastlv && praiselv <= m_nowlv)
				praiselvvec.push_back(praiselv);
		}

		if (praiselvvec.size() > 0)
		{
			std::string logdata="等级升级奖励：lv"+crArgumentParser::appItoa(mylv);
			for (int i=0; i<praiselvvec.size(); i++)
			{
				if(presentSoldierTab.valid() && presentSoldierTab->queryOneRecord(0,crArgumentParser::appItoa(praiselvvec[i]),record) >= 0 )
				{
					logdata += record[0];
					CRCore::crVector3i awardVec;
					JXJItemBagVec items;
		
					for (int i = 1 ; i <= 4; ++i)
					{
						int soldierIndex = presentSoldierTab->getTitleIndex("奖励"+crArgumentParser::appItoa(i));
						if(soldierIndex >= 0 && !record[soldierIndex].empty())
						{
							crArgumentParser::appAtoVec(record[soldierIndex],awardVec);
							crBagItemData* pItem = new crBagItemData();
							pItem->setItemID(awardVec[0]);
							pItem->setEquipMagic(awardVec[1]);
							pItem->setItemCount(awardVec[2]);
							items.push_back(pItem);
						}
					}
			
					if (!items.empty())
					{
						m_this->doEvent(WCH_JXJRecvRewardItems,MAKECREPARAM(&items,0));
					}
			
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(items.size()*8 + 2);
					stream->_writeUChar((unsigned char)m_nowlv);
					stream->_writeChar(items.size());
					for (int i = 0; i < items.size(); ++i)
					{
						stream->_writeInt(items[i]->getItemID());
						stream->_writeInt(items[i]->getItemCount());
					}
			
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvPresentSoldier,stream.get());
					gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(),packet);
				}
			}
			///
			///游戏日志
			GameLogData gamelog(Log_PresentSoldierAward, logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvPresentSoldierMethod
//
/////////////////////////////////////////
crJXJRecvPresentSoldierMethod::crJXJRecvPresentSoldierMethod():
	m_this(NULL)
{
}
crJXJRecvPresentSoldierMethod::crJXJRecvPresentSoldierMethod(const crJXJRecvPresentSoldierMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPresentSoldierMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvPresentSoldierMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_switch = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		m_icons[i-3] = str;
		break;
	case 7:
	case 8:
	case 9 :
	case 10:
		m_inputs[i-7] = str;
		break;
	default:
		break;
	}
}
void crJXJRecvPresentSoldierMethod::operator()(crHandle &handle)
{	
	if(m_this && m_netType == GameClient_Game)
	{
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if (!itemtab.valid())
			return;
		ref_ptr<crCanvasNode>Canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!Canvas.valid())
			return;
		void *param = NULL;
		crData *pPlayerData = m_this->getDataClass();
		crRole *mainRole = m_this->getMainRole();
		crData *roleData = mainRole?mainRole->getDataClass():NULL;
		if (!roleData)
			return;
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;

		pPlayerData->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		rolelv = m_stream->_readUChar();

		ref_ptr<crStaticTextWidgetNode> title = dynamic_cast<crStaticTextWidgetNode *>(Canvas->getWidget(m_title));
		ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		crTableIO::StrVec record;
		if (textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(9188),record) >= 0 && title.valid())
		{
			char buf[64];
			sprintf(buf,record[1].c_str(),rolelv);
			title->setString(std::string(buf));
		}

		ref_ptr<crMultiSwitch> lordSwitch = dynamic_cast<crMultiSwitch *>(Canvas->getChildNode(m_switch));
		if (lordSwitch.valid())
		{
			lordSwitch->setActiveSwitchSet(shiliid - c_startShiliID);
		}

		ref_ptr<crImageBoxWidgetNode> icons[4];
		ref_ptr<crStaticTextWidgetNode> inputs[4];
		
		crData *data = m_this->getDataClass();
		char size = m_stream->_readChar();
		for (int i = 0; i < size,i<4; ++i)
		{
			icons[i]= dynamic_cast<crImageBoxWidgetNode *>(Canvas->getWidget(m_icons[i]));
			inputs[i] = dynamic_cast<crStaticTextWidgetNode *>(Canvas->getWidget(m_inputs[i]));
			int itemId = m_stream->_readInt();
			int count = m_stream->_readInt();
			crTableIO::StrVec itemRecord;
			std::string imageName = "";
			int iconIdx = itemtab->getTitleIndex("icon");
			int itemIdIdx = itemtab->getTitleIndex("itemid");
			if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(itemId),itemRecord) >= 0)
			{
				imageName = itemRecord[iconIdx];
			}
			icons[i]->setImageName(imageName);
			inputs[i]->setString(crArgumentParser::appItoa(count));
		}
		crFilterRenderManager::getInstance()->showCanvas(Canvas.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJ7DhuodongCanvasUpdateMethod
//
///////////////////////////////////////
crJXJ7DhuodongCanvasUpdateMethod::crJXJ7DhuodongCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJ7DhuodongCanvasUpdateMethod::crJXJ7DhuodongCanvasUpdateMethod( const crJXJ7DhuodongCanvasUpdateMethod & handle ):
	crMethod(handle)
{
	
}

void crJXJ7DhuodongCanvasUpdateMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}

}

void crJXJ7DhuodongCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_tag = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:

	case 6:
	case 7:
	case 8:
	case 9:

	case 10:
	case 11:
	case 12:
	case 13:

	case 14:
	case 15:
	case 16:
	case 17:

	case 18:
	case 19:
	case 20:
	case 21:
		m_icons[(i-2)/4][(i-2)%4] = str;
		break;
	case 22:
	case 23:
	case 24:
	case 25:

	case 26:
	case 27:
	case 28:
	case 29:

	case 30:
	case 31:
	case 32:
	case 33:

	case 34:
	case 35:
	case 36:
	case 37:

	case 38:
	case 39:
	case 40:
	case 41:
		m_inputs[(i-22)/4][(i-22)%4] = str;
		break;
	
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
		m_btnget[i-42] = str;
		break;
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		m_title[i-47] =  str;
		break;
	case 52:
	case 53:
	case 54:
	case 55:
		m_carddesc[i-52] = str;
		break;
    case 56:
    case 57:
    case 58:
    case 59:

    case 60:
    case 61:
    case 62:
    case 63:

    case 64:
    case 65:
    case 66:
    case 67:
        
    case 68:
    case 69:
    case 70:
    case 71:

    case 72:
    case 73:
    case 74:
    case 75:
        m_planes[(i-56)/4][(i-56)%4] = str;
        break;
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 81:
        m_desc[i-76] = str;
        break;
	default:
		break;
	}
}
void crJXJ7DhuodongCanvasUpdateMethod::operator()( crHandle &handle )
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();	
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (myPlayer && myPlayer->getDataClass() &&canvas.valid())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		void *param;
		ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_tag));
		int selected = radioGroup->getSelect();

		crTableIO::StrVec record;
		std::vector<int> awardVec;
		std::vector<std::string> titleVec;
		ref_ptr<crTableIO> _7DayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DayTab);
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int nameIndex = itemtab->getTitleIndex("name");
		int itemidIndex = itemtab->getTitleIndex("itemid");
		int qualityIndex = itemtab->getTitleIndex("品质");
		if (_7DayTab.valid() && _7DayTab->queryOneRecord(0,crArgumentParser::appItoa(selected+1),record) >=0)
		{
			std::string imageName;
			std::string strCount,desstr,condition;
			char state;
            int itemId;
            int count;
			int titleIndex = _7DayTab->getTitleIndex("标题");
			int descIndex  = _7DayTab->getTitleIndex("描述");
			int conditionTypeIndex,conditionIndex;
			int conditionType;
			ref_ptr<crStaticTextWidgetNode> title;
			ref_ptr<crStaticTextWidgetNode> desc;
			crRole *mainRole = myPlayer->getMainRole();
			crData *roleData = mainRole->getDataClass();
			std::string output;
			for (int i = 0; i < 5; ++i)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJ7DayData,param);
				char *_7DayData = (char *)param;
				state = *(_7DayData + selected*5+i);
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				ref_ptr<crButtonWidgetNode>  btnget = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btnget[i]));
				if (btnget.valid())
				{
                    if (state == 1)
                    {
                        btnget->setEnable(true);
                        btnget->setVisiable(true);
                    }
                    else if (state == 2)
                    {
                        btnget->setVisiable(false);
                    }
                    else
                    {
                        btnget->setVisiable(true);
                        btnget->setEnable(false);
                    }
				}
				
				int awardIndex = _7DayTab->getTitleIndex("奖励" + crArgumentParser::appItoa(i));
				title = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title[i]));
                desc = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_desc[i]));
                
				crArgumentParser::appAtoVec(record[titleIndex],titleVec);
				if (title.valid())
				{
					title->setString(titleVec[i]);
				}
                crArgumentParser::appAtoVec(record[descIndex],titleVec,'|');
                if (desc.valid())
                {//
					desstr = titleVec[i];
					if(i>0&&i<4)
					{
						conditionTypeIndex = _7DayTab->getTitleIndex("条件类型" + crArgumentParser::appItoa(i));
						conditionIndex = _7DayTab->getTitleIndex("条件" + crArgumentParser::appItoa(i));
						conditionType = atoi(record[conditionTypeIndex].c_str());
						condition = record[conditionIndex];
						switch (conditionType)
						{
						case _7DayType_Level:
							{//玩家等级
								roleData->getParam(WCHDATA_Level,param);
								unsigned char rolelv = *(unsigned char *)param;

								getDesstring(desstr,crArgumentParser::appItoa(rolelv),condition,output);
								desstr = output;
							}
							break;
						case _7DayType_Technology:
							{
								//军备科技
								data->excHandle(MAKECREPARAM(WCH_LockData,1));
								data->getParam(WCHDATA_JXJTroopsTechMap,param);
								TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
								int totaltec = 0;
								for (TroopsTechMap::iterator it = troopstechmap->begin(); it != troopstechmap->end(); ++it)
								{
									totaltec += it->second;
								}
								data->excHandle(MAKECREPARAM(WCH_LockData,0));

								getDesstring(desstr,crArgumentParser::appItoa(totaltec),condition,output);
								desstr = output;
							}
							break;
						case _7DayType_Building:
							{
								//建筑总等级
								int totalBuilding = 0;
								data->getParam(WCHDATA_JXJFuyalv,param);
								unsigned char fuyalv = *(unsigned char *)param;
								totalBuilding += fuyalv;
								data->getParam(WCHDATA_JXJBingyinglv,param);
								unsigned char bingyinglv = *(unsigned char *)param;
								totalBuilding += bingyinglv;
								data->getParam(WCHDATA_JXJJiusilv,param);
								unsigned char jiusilv = *(unsigned char *)param;
								totalBuilding += jiusilv;
								data->getParam(WCHDATA_JXJGongfanglv,param);
								unsigned char gongfanglv = *(unsigned char *)param;
								totalBuilding += gongfanglv;
								data->getParam(WCHDATA_JXJShijilv,param);
								unsigned char shijilv = *(unsigned char *)param;
								totalBuilding += shijilv;
								data->getParam(WCHDATA_JXJFukulv,param);
								unsigned char fukulv = *(unsigned char *)param;
								totalBuilding += fukulv;

								int startid = WCHDATA_JXJNongtianlv1;
								for (int i = 0;i < 8;++i,++startid)
								{
									data->getParam(startid,param);
									unsigned char lv = *(unsigned char *)param;
									totalBuilding += lv;
								}
								startid = WCHDATA_JXJMingjulv1;
								for (int i = 0;i < 8;++i,++startid)
								{
									data->getParam(startid,param);
									unsigned char lv = *(unsigned char *)param;
									totalBuilding += lv;
								}
								startid = WCHDATA_JXJLingdilv1;
								for (int i = 0;i < 4;++i,++startid)
								{
									data->getParam(startid,param);
									unsigned char lv = *(unsigned char *)param;
									totalBuilding += lv;
								}
								startid = WCHDATA_JXJKuangshanlv1;
								for (int i = 0;i < 4;++i,++startid)
								{
									data->getParam(startid,param);
									unsigned char lv = *(unsigned char *)param;
									totalBuilding += lv;
								}
								startid = WCHDATA_JXJMuchanglv1;
								for (int i = 0;i < 4;++i,++startid)
								{
									data->getParam(startid,param);
									unsigned char lv = *(unsigned char *)param;
									totalBuilding += lv;
								}

								getDesstring(desstr,crArgumentParser::appItoa(totalBuilding),condition,output);
								desstr = output;
							}
							break;
						case _7DayType_Death:
							{
								//战场死兵
								data->getParam(WCHDATA_JXJDayPlayerDeadCount,param);
								int death = *(int *)param;

								getDesstring(desstr,crArgumentParser::appItoa(death),condition,output);
								desstr = output;
							}
							break;
						case _7DayType_Ectype:
							{
								//副本总星级
								data->excHandle(MAKECREPARAM(WCH_LockData,1));
								data->getParam(WCHDATA_JXJFubenProgressMap,param);
								FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
								int totalstar = 0;
								for (FubenProgressMap::iterator it = fubenProgress->begin(); it != fubenProgress->end(); ++it)
								{
									totalstar += it->second->getStarCount();
								}
								data->excHandle(MAKECREPARAM(WCH_LockData,0));

								getDesstring(desstr,crArgumentParser::appItoa(totalstar),condition,output);
								desstr = output;
							}
							break;
						}
					}
                    desc->setString(desstr);
                }
				if (i == 4)//将卡组合
				{
                    ref_ptr<crStaticTextWidgetNode> carddesc = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_desc[i+1]));
                    if (carddesc.valid())
                    {
                        carddesc->setString(titleVec[i+1]);
                    }
					data->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->getParam(WCHDATA_JXJJiangkaBagVec,param);
					JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
                    data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
                    SavedFormationInfoVec *savedformationVec = (SavedFormationInfoVec *)param;
					int conditionIndex = _7DayTab->getTitleIndex("条件4");
					std::vector<std::string> cardVec;
					crTableIO::DataVec itemRecordVec;
					crArgumentParser::appAtoVec(record[conditionIndex],cardVec);
					int qualityCondition = atoi(cardVec[cardVec.size()-1].c_str());
					for (int j = 0; j < 4; ++j)
					{
						ref_ptr<crHypertextWidgetNode> carddesc = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_carddesc[j]));
						std::vector<float>v_i;
						ItemMap itemMap;
						rcfg::ConfigScript cfg_script(&itemMap);
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");
						//cfg_script.Add("Font",m_font);
						//cfg_script.Add("CharacterSize",m_characterSize);
						
						cfg_script.Add("Content");
						cfg_script.Push("Content",1);
						v_i.clear();
						v_i.push_back(255.0f);
						v_i.push_back(235.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						switch (atoi(cardVec[0+j*3].c_str()))
						{
						case 2://群
							cfg_script.Add("Text","群:");
							break;
						case 10:
							cfg_script.Add("Text","魏:");
							break;
						case 11:
							cfg_script.Add("Text","蜀:");
							break;
						case 12:
							cfg_script.Add("Text","吴:");
							break;
						default:
							break;
						}
						cfg_script.Pop();

						

						cfg_script.Add("Content");
						cfg_script.Push("Content",2);
						itemtab->queryRecords(nameIndex,cardVec[1+j*3],itemRecordVec);
						v_i.clear();
						if (searchCard(cardbagvec,itemRecordVec,itemidIndex,qualityIndex,qualityCondition,savedformationVec))
						{
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
						}
						else
						{
							v_i.push_back(255.0f);
							v_i.push_back(235.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
						}
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text",cardVec[1+j*3]);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",3);
						v_i.clear();
						v_i.push_back(255.0f);
						v_i.push_back(235.0f);
						v_i.push_back(0.0f);
						v_i.push_back(255.0f);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text","+");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",4);
						itemtab->queryRecords(nameIndex,cardVec[2+j*3],itemRecordVec);
						v_i.clear();
						if (searchCard(cardbagvec,itemRecordVec,itemidIndex,qualityIndex,qualityCondition,savedformationVec))
						{
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
						}
						else
						{
							v_i.push_back(255.0f);
							v_i.push_back(235.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
						}
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text",cardVec[2+j*3]);
						cfg_script.Pop();

						cfg_script.Pop();

						if(carddesc.valid())carddesc->setHypertext(cfg_script);

					}
					data->excHandle(MAKECREPARAM(WCH_LockData,0));
					
				}

				crArgumentParser::appAtoVec(record[awardIndex],awardVec);
				for (int j = 0; j < 4; ++j)
				{
					ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icons[i][j]));
					ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputs[i][j]));
                    ref_ptr<crNode> plane = m_this->getChildNode(m_planes[i][j]);
                    
					imageName = "";
					strCount = "";
					if (j < awardVec.size()/3)
					{
						crTableIO::StrVec itemRecord;
						int iconIdx = itemtab->getTitleIndex("icon");
						int itemIdIdx = itemtab->getTitleIndex("itemid");
                        itemId = awardVec[0+j*3];
                        count = awardVec[2+j*3];
						if (itemtab->queryOneRecord(itemIdIdx,crArgumentParser::appItoa(itemId),itemRecord) >= 0)
						{
							imageName = itemRecord[iconIdx];
							strCount = crArgumentParser::appItoa(count);
						}
                        crData* uiData = icon->getDataClass();
                        if (uiData)
                        {
                            uiData->inputParam(0,&itemId);
                            uiData->inputParam(1,&count);
                        }
                        plane->setVisiable(true);
                        icon->setVisiable(true);
                        input->setVisiable(true);
                        icon->setImageName(imageName);
                        input->setString(strCount);
					}
                    else
                    {
                        plane->setVisiable(false);
                        icon->setVisiable(false);
                        input->setVisiable(false);
                    }
				}
			}
		}
	}
}

bool searchCard(JXJItemBagVec *cardbagvec,crTableIO::DataVec& itemRecordVec,int itemidIndex,int qualityIndex, int qualityCondition,SavedFormationInfoVec  *formationVec)
{
	int itemid;
	int quality;
	for (int i = 0; i < itemRecordVec.size(); ++i)
	{
		itemid = atoi(itemRecordVec[i][itemidIndex].c_str());
		quality = atoi(itemRecordVec[i][qualityIndex].c_str());
		if (quality >= qualityCondition)
		{
			for (int j = 0; j < cardbagvec->size(); ++j)
			{
				if ((*cardbagvec)[j].valid() && (*cardbagvec)[j]->getItemID() == itemid)
				{
					return true;
				}
			}
            for (int i = 0 ; i < formationVec->size(); ++i)
            {
                FormationInfoVec& formation = (*formationVec)[i].second;
                for (FormationInfoVec::iterator it = formation.begin(); it != formation.end(); ++it)
                {
                    if ((*it).valid() && ((*it)->getEquips()[0] == itemid || (*it)->getEquips()[1] == itemid))
                    {
                        return true;
                    }
                }
            }
		}
	}
    
    
	return false;
}

/////////////////////////////////////////
//
//crJXJCheck7DayMethod
//
/////////////////////////////////////////
crJXJCheck7DayMethod::crJXJCheck7DayMethod():
	m_this(NULL){}
crJXJCheck7DayMethod::crJXJCheck7DayMethod(const crJXJCheck7DayMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheck7DayMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	}
}
void crJXJCheck7DayMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJCheck7DayMethod::operator()(crHandle &handle)
{
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> player = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(player.valid() && m_this->getDataClass() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
	{
		void* param = NULL;
		crData* pPlayerData = m_this->getDataClass();
        pPlayerData->getParam(WCHDATA_JXJPlayerLoginDays,param);
        unsigned short loginDays = *(unsigned short *)param;
        if (loginDays > 7)
            return;
		crTableIO::StrVec record;
		crTableIO::DataVec recordVec;
		ref_ptr<crTableIO> _7DayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DayTab);
		recordVec = _7DayTab->getDataVec();
		int conditionTypeIndex[4];
		int conditionIndex[4];
		int conditionType;
		int condition;
		for (int i = 0; i < 4; ++i)
		{
			conditionTypeIndex[i] = _7DayTab->getTitleIndex("条件类型" + crArgumentParser::appItoa(i+1));
			conditionIndex[i] = _7DayTab->getTitleIndex("条件" + crArgumentParser::appItoa(i+1));
		}
		crRole *mainRole = m_this->getMainRole();
		crData *roleData = mainRole->getDataClass();
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;
		//官职
		roleData->getParam(WCHDATA_JXJGovPost,param);
		unsigned char govpost = *(unsigned char *)param;


		pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
		pPlayerData->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
		pPlayerData->getParam(WCHDATA_JXJ7DayData,param);
		char *_7DayData = (char *)param;
        //战场死兵
        
        pPlayerData->getParam(WCHDATA_JXJDayPlayerDeadCount,param);
        int death = *(int *)param;
		//军备科技
		pPlayerData->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		int totaltec = 0;
		for (TroopsTechMap::iterator it = troopstechmap->begin(); it != troopstechmap->end(); ++it)
		{
			totaltec += it->second;
		}
		//建筑总等级
		int totalBuilding = 0;
		pPlayerData->getParam(WCHDATA_JXJFuyalv,param);
		unsigned char fuyalv = *(unsigned char *)param;
		totalBuilding += fuyalv;
		pPlayerData->getParam(WCHDATA_JXJBingyinglv,param);
		unsigned char bingyinglv = *(unsigned char *)param;
		totalBuilding += bingyinglv;
		pPlayerData->getParam(WCHDATA_JXJJiusilv,param);
		unsigned char jiusilv = *(unsigned char *)param;
		totalBuilding += jiusilv;
		pPlayerData->getParam(WCHDATA_JXJGongfanglv,param);
		unsigned char gongfanglv = *(unsigned char *)param;
		totalBuilding += gongfanglv;
		pPlayerData->getParam(WCHDATA_JXJShijilv,param);
		unsigned char shijilv = *(unsigned char *)param;
		totalBuilding += shijilv;
		pPlayerData->getParam(WCHDATA_JXJFukulv,param);
		unsigned char fukulv = *(unsigned char *)param;
		totalBuilding += fukulv;

		int startid = WCHDATA_JXJNongtianlv1;
		for (int i = 0;i < 8;++i,++startid)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			totalBuilding += lv;
		}
		startid = WCHDATA_JXJMingjulv1;
		for (int i = 0;i < 8;++i,++startid)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			totalBuilding += lv;
		}
		startid = WCHDATA_JXJLingdilv1;
		for (int i = 0;i < 4;++i,++startid)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			totalBuilding += lv;
		}
		startid = WCHDATA_JXJKuangshanlv1;
		for (int i = 0;i < 4;++i,++startid)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			totalBuilding += lv;
		}
		startid = WCHDATA_JXJMuchanglv1;
		for (int i = 0;i < 4;++i,++startid)
		{
			pPlayerData->getParam(startid,param);
			unsigned char lv = *(unsigned char *)param;
			totalBuilding += lv;
		}
		//副本总星级
		pPlayerData->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
		int totalstar = 0;
		for (FubenProgressMap::iterator it = fubenProgress->begin(); it != fubenProgress->end(); ++it)
		{
			totalstar += it->second->getStarCount();
		}
		
		std::vector<std::string> strVec;
		std::string cardName1;
		std::string cardName2;
		char *state = NULL;
		bool bChange = false;
		crTableIO::DataVec itemRecords;
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int itemidIndex = itemtab->getTitleIndex("itemid");
		int nameIndex = itemtab->getTitleIndex("name");
		int qualityIndex = itemtab->getTitleIndex("品质");
		for (int i = 0 ; i < recordVec.size(),i < 7; ++i)
		{
			record = recordVec[i];
			for (int j = 0; j < 5; ++j)
			{
				state = _7DayData + i*5+j;
				if (*state != 0)
				{
					continue;
				}
				if (loginDays-1 != i)
				{
					continue;
				}
				
				if (j == 0)
				{//登录有奖
					*state = 1;
					bChange = true;
				}
				else if (j == 4)
				{//将卡组合
                    pPlayerData->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
                    SavedFormationInfoVec *savedformationVec = (SavedFormationInfoVec *)param;

					crArgumentParser::appAtoVec(record[conditionIndex[j-1]],strVec);
					int quality = atoi(strVec[strVec.size()-1].c_str());
					for (int k = 0; k < 4; ++k)
					{
						cardName1 = strVec[1+k*3];
						cardName2 = strVec[2+k*3];                        
						itemtab->queryRecords(nameIndex,cardName1,itemRecords);
                        if(searchCard(cardbagvec,itemRecords,itemidIndex,qualityIndex,quality,savedformationVec))
                        {
                            itemtab->queryRecords(nameIndex,cardName2,itemRecords);
                            if(searchCard(cardbagvec,itemRecords,itemidIndex,qualityIndex,quality,savedformationVec))
                            {
                                *state = 1;
                                bChange = true;
                                break;
                            }
                        }
					}
				}
				else
				{
					conditionType = atoi(record[conditionTypeIndex[j-1]].c_str());
					condition = atoi(record[conditionIndex[j-1]].c_str());
					switch (conditionType)
					{
					case _7DayType_Level:
						if(rolelv >= condition)
						{
							*state = 1;
							bChange = true;
						}
						break;
					case _7DayType_Technology:
						if (totaltec >= condition)
						{
							*state = 1;
							bChange = true;
						}
						break;
					case _7DayType_Building:
						if (totalBuilding >= condition)
						{
							*state = 1;
							bChange = true;
						}
						break;
					case _7DayType_Death:
                        if (death >= condition)
                        {
                            *state = 1;
                            bChange = true;
                        }
						break;
					case _7DayType_Ectype:
						if (totalstar >= condition)
						{
							*state = 1;
							bChange = true;
						}
						break;
					case _7DayType_Official:
						if (govpost >= condition)
						{
							*state = 1;
							bChange = true;
						}
						break;
					case _7DayType_Card:

						break;
					default:
						break;
					}
				}
			}
		}
		pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));

		if (bChange)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5*7);
			for (int i = 0; i < 7; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					stream->_writeChar(*(_7DayData + i*5 + j));
				}
			}
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGet7DayAward,stream.get());
			gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(),packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIGet7DayAwardMethod
//
/////////////////////////////////////////
crJXJUIGet7DayAwardMethod::crJXJUIGet7DayAwardMethod():
	m_this(NULL)
{
}
crJXJUIGet7DayAwardMethod::crJXJUIGet7DayAwardMethod(const crJXJUIGet7DayAwardMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{
}
void crJXJUIGet7DayAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUIGet7DayAwardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tag = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJUIGet7DayAwardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_tag));
	
	if(myPlayer && netConductor && radioGroup.valid())
	{
		int selected = radioGroup->getSelect();
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeChar(selected*5+m_index);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGet7DayAward,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

/////////////////////////////////////////
//
//crJXJRecvGet7DayAwardMethod
//
/////////////////////////////////////////
crJXJRecvGet7DayAwardMethod::crJXJRecvGet7DayAwardMethod():
	m_netType(GameClient_Game){}
crJXJRecvGet7DayAwardMethod::crJXJRecvGet7DayAwardMethod(const crJXJRecvGet7DayAwardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGet7DayAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
			m_netType = HICREPARAM(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGet7DayAwardMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGet7DayAwardMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		int playerid = m_this->getPlayerID();
		crData *pPlayerData = m_this->getDataClass();
		void* param = NULL;
		if(m_netType == GameServer)
		{
			char index = m_stream->_readChar();
			bool success = false;
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			pPlayerData->getParam(WCHDATA_JXJ7DayData,param);
			char *_7DayData = (char *)param;
			if (*(_7DayData+index) == 1)
			{
                *(_7DayData+index) = 2;
				success = true;
				crTableIO::StrVec record;
				std::vector<std::string> awardVec;
				JXJItemBagVec items;
				ref_ptr<crTableIO> _7DayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJ7DayTab);
				if(_7DayTab.valid() && _7DayTab->queryOneRecord(0,crArgumentParser::appItoa(index/5 + 1),record) >= 0)
				{
					int awardIndex = _7DayTab->getTitleIndex("奖励"+crArgumentParser::appItoa(index%5));
					crArgumentParser::appAtoVec(record[awardIndex],awardVec);
					for (int i = 0; i < awardVec.size()/3; ++i)
					{
						crBagItemData* pItem = new crBagItemData();
						pItem->setItemID(atoi(awardVec[0+i*3].c_str()));
						pItem->setEquipMagic(atoi(awardVec[1+i*3].c_str()));
						pItem->setItemCount(atoi(awardVec[2+i*3].c_str()));
						items.push_back(pItem);
					}
					
					m_this->doEvent(WCH_JXJRecvRewardItems,MAKECREPARAM(&items,0));
				}
			}
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));
			if (success)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5*7);
				for (int i = 0; i < 7; ++i)
				{
					for (int j = 0; j < 5; ++j)
					{
						stream->_writeChar(*(_7DayData + i*5 + j));
					}
				}
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> player = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(player.valid())
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGet7DayAward,stream.get());
					gameServer->getNetManager()->sendPacket(player->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			pPlayerData->getParam(WCHDATA_JXJ7DayData,param);
			char *_7DayData = (char *)param;
			for (int i = 0; i < 7; ++i)
			{
				for (int j = 0; j < 5; ++j)
				{
					*(_7DayData+i*5+j) = m_stream->_readChar();
				}
			}
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

////////////////////////////////
//
//crCheck7DayCompleteMethod
///////////////////////////////////
crCheck7DayCompleteMethod::crCheck7DayCompleteMethod() :
	m_this(NULL)
{}

crCheck7DayCompleteMethod::crCheck7DayCompleteMethod(const crCheck7DayCompleteMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_huodongcanvas(handle.m_huodongcanvas),
	m_countinput(handle.m_countinput)
{
}

void crCheck7DayCompleteMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crCheck7DayCompleteMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_huodongcanvas = str;
		break;
	case 1:
		m_countinput = str;
		break;
    case 2:
        m_countbackboard = str;
        break;
	}
}

void crCheck7DayCompleteMethod::operator()(crHandle &handle)
{
	if (crGlobalHandle::isClient())
	{
		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode>huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
		if (huodongcanvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> countinput = dynamic_cast<crStaticTextWidgetNode *>(huodongcanvas->getWidget(m_countinput));
            ref_ptr<crNode> countbackboard = dynamic_cast<crNode *>(huodongcanvas->getChildNode(m_countbackboard));
			crData *pPlayerData = myPlayer->getDataClass();
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			pPlayerData->getParam(WCHDATA_JXJ7DayData,param);
			char *_7DayData = (char *)param;
			int count = 0; 
			if (countinput.valid() && countbackboard.valid())
			{
                countbackboard->setVisiable(false);
				countinput->clearString();
				for (int i = 0; i < 7; ++i)
				{
					for (int j = 0; j < 5; ++j)
					{
						if (*(_7DayData + i*5+j) == 1)
						{
							++count;
						}
					}
				}
				if (count)
				{
					countinput->setString(crArgumentParser::appItoa(count));
                    countbackboard->setVisiable(true);
				}
            }
			pPlayerData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUI7DayInitMethod
//
/////////////////////////////////////////
crJXJUI7DayInitMethod::crJXJUI7DayInitMethod():
    m_this(NULL)
{
}
crJXJUI7DayInitMethod::crJXJUI7DayInitMethod(const crJXJUI7DayInitMethod& handle):
    crMethod(handle),
    m_this(NULL)
{
}
void crJXJUI7DayInitMethod::inputParam(int i, void *param)
{
    switch(i) 
    {
    case 0:
        if(param == 0)
        {//释放
            m_this = NULL;
        }
        break;
    case 1:
        m_this = (crCanvasNode*)param;
        break;
    }
}
void crJXJUI7DayInitMethod::addParam(int i, const std::string& str)
{
    switch (i)
    {
    case 0:
        m_radioGroup = str;
        break;
    default:
        break;
    }
}
void crJXJUI7DayInitMethod::operator()(crHandle &handle)
{
    crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
    if(m_this &&myPlayer)
    {
        void *param;
        crData *pPlayerData = myPlayer->getDataClass();
        pPlayerData->getParam(WCHDATA_JXJPlayerLoginDays,param);
        unsigned short loginDays = *(unsigned short *)param;

        if (loginDays <= 7)
        {
            ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radioGroup));
            if(radioGroup.valid())
            {
                radioGroup->select(loginDays-1);
            }
        }
    }
}