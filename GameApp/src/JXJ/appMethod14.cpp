/**********************************************************************
*
*	文件:	   appMethod14.cpp
*
*	描述:	   
*
*	作者:	   吴桐
*					  
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod14.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <CRUI/crWidgetExtend.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crSceneHandler.h>
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRDataBase/crDataBaseManager.h>
#include <CRNetApp/appDBUpdate2.h>
#include <CRNetApp/appDBQuery2.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNodeCallbacks.h>
#include <CREncapsulation/crGameDices.h>
#include <JXJ/appData5.h>
#include <stdlib.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crConvertUTF.h>
#include <JXJ/appMethod16.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CREncapsulation;
using namespace JXJ;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRUI;
using namespace CRIOManager;
using namespace CRProducer;

/////////////////////////////////////////
//
// crJXJUIQiangZhengTipMethod 
//强征提示
//
/////////////////////////////////////////
 crJXJUIQiangZhengTipMethod :: crJXJUIQiangZhengTipMethod ()
{
}
 crJXJUIQiangZhengTipMethod :: crJXJUIQiangZhengTipMethod (const  crJXJUIQiangZhengTipMethod & handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget),
	m_id(handle.m_id)
{
}
void  crJXJUIQiangZhengTipMethod ::inputParam(int i, void *param)
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
void  crJXJUIQiangZhengTipMethod ::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	case 2:
		m_id = str;
		break;
	}
}

void crJXJUIQiangZhengTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		int goldneed = 0;
		int expneed = 0;
		std::string str;
		void *param;
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);

		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content", 1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);

		if (m_id == "qiangzheng")
		{
		
			crData *data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJLevyCount,param);
			unsigned char levyCount = *(unsigned char *)param;
			crVector2i maxlevycount;
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,viplv).c_str(),maxlevycount);
			if(levyCount>0 && levyCount <= maxlevycount[LevyType_Gold])
			{
				//检查元宝是否够
				int impress = maxlevycount[LevyType_Gold] - levyCount + 1;
				ref_ptr<crTableIO>impressTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJImpressTab);
				crTableIO::StrVec record;
				if(impressTab->queryOneRecord(0,crArgumentParser::appItoa(impress),record)>=0)
				{
					 goldneed = atoi(record[1].c_str());
				}
			}

			cfg_script.Add("Text", "         花费");
			cfg_script.Pop();
			str = "进行强征";
			
		}
		if (m_id == "taobing")
		{
			int needgold= 0;
			//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			crData *playerData = myPlayer->getDataClass();
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			if(canvas.valid() && canvas->getDataClass())
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int curgoldmin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCurPunishMinCostGold,viplv).c_str());
				crData *canvasdata = canvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJBattlePunishTime,param);
				short punishtime = *(short *)param;
				needgold = (1 + punishtime/60) * curgoldmin;
				goldneed = needgold;
			}
			cfg_script.Add("Text", "       花费");
			cfg_script.Pop();
			str = "清除惩罚时间" ;
		}
		if (m_id =="miaokeji_0" || m_id =="miaokeji_1" || m_id =="miaokeji_2" || m_id =="miaokeji_3" || m_id =="miaokeji_4" || m_id =="miaokeji_5" || m_id =="miaokeji_6")
		{
			cfg_script.Add("Text", "         花费");
			cfg_script.Pop();
			int index = atoi(m_id.substr(9,1).c_str());
			int needgold= 0;
			//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			crData *data = myPlayer->getDataClass();
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			if(canvas.valid() && canvas->getDataClass())
			{
				int techid = 0;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int curgoldmin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCurTechCDMinCostGold,viplv).c_str());
				crData *canvasdata = canvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJShowTechVec,param);
				ShowTechVec *showtechvec = (ShowTechVec *)param;
				if (index < showtechvec->size())
				{
					techid = (*showtechvec)[index];
				}
				if (techid > 0)
				{
					data->getParam(WCHDATA_JXJTechDevelopMap,param);
					TechDevelopMap *developmap = (TechDevelopMap *)param;
					TechDevelopMap::iterator fitr = developmap->find(techid);
					if (fitr != developmap->end())
					{
						int cleartime = fitr->second;
						needgold = (1 + cleartime/60) * curgoldmin;
						canvasdata->inputParam(WCHDATA_JXJClearTechDevelopTimeNeedGold,&needgold);
						canvasdata->inputParam(WCHDATA_JXJClearTechDevelopID,&techid);

						goldneed = needgold;
					}
				}
			}
			str = "立刻完成";
		}
		if (m_id == "fete")
		{
			cfg_script.Add("Text", "       花费");
			cfg_script.Pop();
			goldneed = 10;
			str = "祭拜获得资源";
		}
		if (m_id =="goumaicailiao_0" || m_id =="goumaicailiao_1" || m_id =="goumaicailiao_2")
		{
			cfg_script.Add("Text", "        花费");
			cfg_script.Pop();

			int index = atoi(m_id.substr(14,1).c_str());
			int itemid = 0;
			int itemcount = 0;
			int needgold = 0;
			int needgoldtemp = 0;
			crData *data = myPlayer->getDataClass();
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			if (canvas.valid())
			{
				crData *canvasdata = canvas->getDataClass();
				if (canvasdata)
				{
					canvasdata->getParam(WCHDATA_JXJGongFangMaterialVec,param);
					GongFangMaterialVec *matevec = (GongFangMaterialVec *)param;
					canvasdata->inputParam(WCHDATA_JXJGongFangMaterialIndex,&index);
					if (index < matevec->size())
					{
						itemid = (*matevec)[index]->getItemID();
						itemcount = (*matevec)[index]->getItemDeductCount();
					}
				}
			}
			ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record;
			int shoppriceindex = itemTab->getTitleIndex("商店价格");
			if (itemid > 0 && itemcount > 0)
			{
				if (itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0 && shoppriceindex > 0)
				{
					needgold = atof(record[shoppriceindex].c_str()) * (float)itemcount;
				}
			}
			goldneed = needgold;
			str = "购买材料";
		}
		
		if (m_id == "chongzhimingjiangzhilu_UI_Input5")
		{
			std::string chongzhiinput = m_id.substr(23,9);
			crCanvasNode* canvas = m_this->getParentCanvas();
			//ref_ptr<crStaticTextWidgetNode> chongzhiinputc = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(chongzhiinput));
			ref_ptr<crMultiSwitch>  jiangliSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("sw131"));
			//std::string strl = chongzhiinputc->getUTF8String();
			if(/*strl == "0/1"*/0==jiangliSwitch->getActiveSwitchSet())
				return;

			cfg_script.Add("Text", "        花费");
			cfg_script.Pop();
			goldneed = 100;
			str = "重置副本";
		}
		if (m_id == "clearpanguobuff")
		{
			cfg_script.Add("Text", "花费");
			cfg_script.Pop();
			goldneed = 10;
			str = "清除叛国缓冲时间立即叛国";
		}
		if (m_id =="chexiaoweiren_0" || m_id =="chexiaoweiren_1" || m_id =="chexiaoweiren_2")
		{
			cfg_script.Add("Text", "         花费");
			cfg_script.Pop();

			int index = atoi(m_id.substr(14,1).c_str());

			crData *data = myPlayer->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			void *param;
			//data->getParam(WCHDATA_JXJJiangkaBagVec,param);
			//JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			data->getParam(WCHDATA_JXJAppointJiangkaBagVec,param);
			JXJAppointCardVec *appointcardvec = (JXJAppointCardVec *)param;
			data->excHandle(MAKEINT64(WCH_LockData,0));

			float validtime = 0;
			crVector2i undocost;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJUndoAppointCardCost,viplv).c_str(),undocost);
			int coststep = undocost[0];
			int costmax = undocost[1];
			if (index < appointcardvec->size())
			{
				if ((*appointcardvec)[index] != NULL)
				{
					validtime = (*appointcardvec)[index]->getValidTime();

					if (validtime > 0)
					{
						int needgold = ceil((float)validtime/3600.0f)*coststep;
						goldneed = needgold;
						str = "撤销委任";
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}
			}
			
		}
		if (m_id == "reward" || m_id == "zhengzhanquan" || m_id == "praisepoints" || m_id == "achievejifen" || m_id == "jiangkatianmingfu")
		{
			if (m_id == "reward")
			{
				cfg_script.Add("Text", "  犒赏三军，提高战场所有友军战斗力");
				cfg_script.Pop();
			}
			else if(m_id == "zhengzhanquan")
			{
				cfg_script.Add("Text", "   可通过每日任务和国战获取征战券");
				cfg_script.Pop();
			}
			else if(m_id == "achievejifen")
			{
				cfg_script.Add("Text", "1:每获得1成就点可以获得1成就积分。2:成就点只供成就排行榜排名用,成就积分可在成就商店买物品");
				cfg_script.Pop();
			}
			else if(m_id == "jiangkatianmingfu")
			{
				std::string text;
				ref_ptr<crTableIO>codtextTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
				crTableIO::StrVec precord;
				if(codtextTab.valid()&&codtextTab->queryOneRecord(0,crArgumentParser::appItoa(9222),precord) >= 0)
					text = "   "+precord[1];

				cfg_script.Add("Text", text);
				cfg_script.Pop();
			}
			else
			{
				crData *data = myPlayer->getDataClass();
				data->getParam(WCHDATA_JXJHavePraiseCounts,param);
				int counts = *(int *)param;
				std::string strrr = "         当前拥有赞数：" + crArgumentParser::appItoa(counts);

				cfg_script.Add("Text", strrr);
				cfg_script.Pop();
			}

			cfg_script.Pop();
			//cfg_script.Write("tipstest.cfg");
			if(tipsText.valid()) tipsText->setHypertext(cfg_script);

			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
			////////////
			crVector2 mouse(crVector2(posx,posy));
			tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);

			return;
		}
		cfg_script.Add("Content");
		cfg_script.Push("Content", 2);
		v_i.clear();
		v_i.push_back(140.0f);
		v_i.push_back(40.0f);
		v_i.push_back(40.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		char tmpText[32];
		sprintf(tmpText, "%d礼金/元宝\0", goldneed);
		cfg_script.Add("Text", tmpText);
		cfg_script.Pop();

		cfg_script.Add("Content");
		cfg_script.Push("Content", 3);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("Text", str);
		cfg_script.Pop();

		cfg_script.Pop();
		//cfg_script.Write("tipstest.cfg");
		if(tipsText.valid()) tipsText->setHypertext(cfg_script);

		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}

}

/////////////////////////////////////////
//
//crJXJUIMingJiangZhiLuJiangLiTipMethod
//名将之路奖励提示
//
/////////////////////////////////////////
crJXJUIMingJiangZhiLuJiangLiTipMethod :: crJXJUIMingJiangZhiLuJiangLiTipMethod ()
{
}
crJXJUIMingJiangZhiLuJiangLiTipMethod :: crJXJUIMingJiangZhiLuJiangLiTipMethod (const  crJXJUIMingJiangZhiLuJiangLiTipMethod & handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
}
void  crJXJUIMingJiangZhiLuJiangLiTipMethod ::inputParam(int i, void *param)
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
void  crJXJUIMingJiangZhiLuJiangLiTipMethod ::addParam(int i, const std::string& str)
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

void crJXJUIMingJiangZhiLuJiangLiTipMethod::operator()(crHandle &handle)
{

	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		//ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		//ref_ptr<crMultiSwitch>  jiangliSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("sw131"));


		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);

		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content", 1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);

		/*if (0 == jiangliSwitch->getActiveSwitchSet())
		{
		cfg_script.Add("Text", "        通关副本可获得奖励");
		cfg_script.Pop();
		}
		else
		{
		cfg_script.Add("Text", "          今日已获取奖励");
		cfg_script.Pop();
		}*/
		
		cfg_script.Add("Text", "玩家参与名将之路副本无次数限制\n 副本通关奖励每日可获取一次。\n消耗礼金/元宝可重置通关奖励领取次数");
		cfg_script.Pop();

		cfg_script.Pop();
		//cfg_script.Write("tipstest.cfg");
		if(tipsText.valid()) tipsText->setHypertext(cfg_script);

		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2) + 0.115;
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
// crJXJUIJunTuanFuBenKaiQiTipMethod 
//军团副本开启提示
//
/////////////////////////////////////////
crJXJUIJunTuanFuBenKaiQiTipMethod  :: crJXJUIJunTuanFuBenKaiQiTipMethod  ()
{
}
crJXJUIJunTuanFuBenKaiQiTipMethod  :: crJXJUIJunTuanFuBenKaiQiTipMethod  (const  crJXJUIJunTuanFuBenKaiQiTipMethod  & handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget),
	m_groupCanvas(handle.m_groupCanvas),
	m_idx(handle.m_idx)

{
}
void  crJXJUIJunTuanFuBenKaiQiTipMethod  ::inputParam(int i, void *param)
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
void  crJXJUIJunTuanFuBenKaiQiTipMethod  ::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	case 2:
		m_groupCanvas = str;
		break;
	case 3:
		m_idx = (unsigned char )atoi(str.c_str());
		break;
	}
}

void crJXJUIJunTuanFuBenKaiQiTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		int goldneed = 0;
		int fundneed = 0;
		char tmpText[32];
		std::string str;
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);

		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content", 1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);

		if (!m_groupCanvas.empty())
		{
			ref_ptr<crCanvasNode> groupCanvas = crFilterRenderManager::getInstance()->findCanvas(m_groupCanvas);
			if ( groupCanvas.valid()  && groupCanvas->getDataClass())
			{
				crData * data = myPlayer->getDataClass();
				CRCore::ScopedLock<crData> lock(*data);

				crData * groupData = groupCanvas->getDataClass();
				CRCore::ScopedLock<crData> lock2(*groupData);

				void *param;
				groupData->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo  *myGroup = (crJXJGroupInfo *)param;

				data->getParam(WCHDATA_JXJGiftGold,param);
				int giftgold = *(int *)param;

				data->getParam(WCHDATA_Money,param);
				int money = *(int *)param;

				ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				crTableIO::StrVec strVec;
				int fundsCostIdx = fubenTab->getTitleIndex("消耗资金");
				int yuanBaoCostIdx = fubenTab->getTitleIndex("征伐值");
				unsigned char maxGroupLevel = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaMaxLevel).c_str());
				if (myGroup->getLv() <= maxGroupLevel)
				{
					bool isFullLevel = false;
					if (myGroup->getLv() == maxGroupLevel)
					{
						isFullLevel = true;
					}
					crTableIO::DataVec  dataVec;
					int fubenType = 1;
					if (fubenTab.valid() )
					{
						int fubenIdIdx = fubenTab->getTitleIndex("id");
						int chapterIdx = fubenTab->getTitleIndex("chapterid");
						fubenTab->queryRecords(chapterIdx,crArgumentParser::appItoa(fubenType),dataVec);

						groupData->getParam(WCHDATA_JXJMyGroupFuBenCurPage,param);
						int fuBenCurPage = *(int *)param;

						
						if (fuBenCurPage + m_idx - 1 < dataVec.size())
						{
							int needMoney = atoi(dataVec[fuBenCurPage + m_idx - 1][yuanBaoCostIdx].c_str());
							int needFunds = atoi(dataVec[fuBenCurPage + m_idx - 1][fundsCostIdx].c_str());
							if ( isFullLevel && myGroup->getFunds() >= needFunds)
							{
								if (isFullLevel)
								{
									if (myGroup->getFunds()-needFunds>=6000)
									{
										fundneed = needFunds;//sprintf(tmpText,"%d 军团资金\0",needExp);
									}
								}
							}
							if (giftgold >= needMoney || money >= needMoney)
							{
								goldneed = needMoney;
							}
						}
					}
				}
			}
		}
		if (fundneed != 0)
		{
			sprintf(tmpText, "     消耗%d军团资金\0", fundneed);
			cfg_script.Add("Text", tmpText);
			cfg_script.Pop();
		}
		else if(goldneed != 0)
		{
			cfg_script.Add("Text", "             消耗");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			sprintf(tmpText, "%d礼金/元宝\0", goldneed);
			cfg_script.Add("Text", tmpText);
			cfg_script.Pop();
		}
		else
		{
			cfg_script.Add("Text", "军团资金和(礼金/元宝)都不足！");
			cfg_script.Pop();
		}

		cfg_script.Pop();
		cfg_script.Write("tipstest.cfg");
		if(tipsText.valid()) tipsText->setHypertext(cfg_script);

		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJUIGerenJuanXianTipMethod
//个人捐献提示
//
/////////////////////////////////////////
crJXJUIGerenJuanXianTipMethod :: crJXJUIGerenJuanXianTipMethod ():
	m_this(NULL)
{
}
crJXJUIGerenJuanXianTipMethod :: crJXJUIGerenJuanXianTipMethod (const  crJXJUIGerenJuanXianTipMethod & handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget),
	m_resource  (handle.m_resource),
	m_yuanBao1  (handle.m_yuanBao1),
	m_yuanBao2  (handle.m_yuanBao2),
	m_yuanBao3  (handle.m_yuanBao3),
	m_yuanBao4  (handle.m_yuanBao4),
	m_canvas	(handle.m_canvas)
{
}
void  crJXJUIGerenJuanXianTipMethod ::inputParam(int i, void *param)
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
void  crJXJUIGerenJuanXianTipMethod ::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	case 2:
		m_yuanBao1 = str;
		break;
	case 3:
		m_yuanBao2 = str;
		break;
	case 4:
		m_yuanBao3 = str;
		break;
	case 5:
		m_yuanBao4 = str;
		break;
	case 6:
		m_resource = str;
		break;
	case 7:
		m_canvas = str;
		break;
	}
}

void crJXJUIGerenJuanXianTipMethod ::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	std::string goldneed;
	std::string str;
	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);

	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() == false) return;
	
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio1= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao1));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio2= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao2));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio3= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao3));
	ref_ptr<crRadioWidgetNode >  yuanBaoRadio4= dynamic_cast<crRadioWidgetNode *>(canvas->getWidget(m_yuanBao4));

	ref_ptr<crTableIO>  contributeTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMyGroupContributeTab);
	ref_ptr<crTableIO>  CoefficientTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCoefficientTab);
	crTableIO::StrVec corecord;
	float factor = 1.0;
	if (CoefficientTab.valid())
	{
		void *param;
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		crRole *mainRole = playerGameData->getMainRole();
		crData *roleData = mainRole->getDataClass();
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;
		int LvIdx = CoefficientTab->getTitleIndex("Level");
		int juanxianIdx = CoefficientTab->getTitleIndex("军团捐献系数");
		if (CoefficientTab->queryOneRecord(LvIdx,crArgumentParser::appItoa(rolelv),corecord) >= 0)
		{
			factor = atof(corecord[juanxianIdx].c_str());
		}
	}
	//if (resourceRadio.valid() && resourceRadio->getSelect())
	//{
	//	std::string str1,str2,str3;
	//	str1 = contributeTab->getData(0,8);
	//	str2 = contributeTab->getData(0,9);
	//	str3 = "花费资源各10000获得，"+str1+"军团资金"+str2+"国家贡献";
	//	cfg_script.Add("Hypertext");
	//	cfg_script.Push("Hypertext");

	//	cfg_script.Add("Content");
	//	cfg_script.Push("Content", 1);
	//	v_i.clear();
	//	v_i.push_back(0.0f);
	//	v_i.push_back(0.0f);
	//	v_i.push_back(0.0f);
	//	v_i.push_back(255.0f);
	//	cfg_script.Add("Color", v_i);
	//	cfg_script.Add("Text", str3);
	//	cfg_script.Pop();
	//}
	//else
	//{
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content", 1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	cfg_script.Add("Text", "花费");
	cfg_script.Pop();

	if (yuanBaoRadio1.valid() && yuanBaoRadio1->getSelect())
	{
		std::string str1,str2,str3;
		str1 = contributeTab->getData(0,8);
		str2 = crArgumentParser::appItoa(crArgumentParser::appAtoI64(contributeTab->getData(0,9)) * factor);
		goldneed = contributeTab->getData(0,7);
		str = "获得"+str1+"军团资金"+str2+"军团贡献";
	}
	if (yuanBaoRadio2.valid() && yuanBaoRadio2->getSelect())
	{
		std::string str1,str2,str3;
		str1 = contributeTab->getData(1,8);
		str2 = crArgumentParser::appItoa(crArgumentParser::appAtoI64(contributeTab->getData(1,9)) * factor);
		goldneed = contributeTab->getData(1,7);
		str = "获得"+str1+"军团资金"+str2+"军团贡献";
	}
	if (yuanBaoRadio3.valid() && yuanBaoRadio3->getSelect())
	{
		std::string str1,str2,str3;
		str1 = contributeTab->getData(2,8);
		str2 = crArgumentParser::appItoa(crArgumentParser::appAtoI64(contributeTab->getData(2,9)) * factor);
		goldneed = contributeTab->getData(2,7);
		str = "获得"+str1+"军团资金"+str2+"军团贡献";
	}
	if (yuanBaoRadio4.valid() && yuanBaoRadio4->getSelect())
	{
		std::string str1,str2,str3;
		str1 = contributeTab->getData(3,8);
		str2 = crArgumentParser::appItoa(crArgumentParser::appAtoI64(contributeTab->getData(3,9)) * factor);
		goldneed = contributeTab->getData(3,7);
		str = "获得"+str1+"军团资金"+str2+"军团贡献";
	}

	cfg_script.Add("Content");
	cfg_script.Push("Content", 2);
	v_i.clear();
	v_i.push_back(140.0f);
	v_i.push_back(40.0f);
	v_i.push_back(40.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	//char tmpText[32];
	//sprintf(tmpText, "%d元宝\0", goldneed);
	cfg_script.Add("Text", goldneed+"礼金/元宝");
	cfg_script.Pop();

	cfg_script.Add("Content");
	cfg_script.Push("Content", 3);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	cfg_script.Add("Text", str);
	cfg_script.Pop();
	/*}*/

	
	cfg_script.Pop();
	//cfg_script.Write("tipstest.cfg");
	if(tipsText.valid()) tipsText->setHypertext(cfg_script);

	const crBoundingBox &btnbox = m_this->getBoundBox();
	const crBoundingBox &tipsbox = tips->getBoundBox();
	const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
	float posx = btnbox.m_min[0];
	float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
	////////////
	crVector2 mouse(crVector2(posx,posy));
	tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
	crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
}

/////////////////////////////////////////
//
//crJXJUIShangBiaoJunGongTipMethod
//上表军功提示
//
/////////////////////////////////////////
crJXJUIShangBiaoJunGongTipMethod::crJXJUIShangBiaoJunGongTipMethod():
	m_this(NULL)
{

}

crJXJUIShangBiaoJunGongTipMethod::crJXJUIShangBiaoJunGongTipMethod( const crJXJUIShangBiaoJunGongTipMethod & handle ):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget)
{
	
}

void crJXJUIShangBiaoJunGongTipMethod::inputParam( int i, void *param )
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void crJXJUIShangBiaoJunGongTipMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	default:
		break;
	}
}

void crJXJUIShangBiaoJunGongTipMethod::operator()( crHandle &handle )
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	int goldneed = 0;
	std::string str;
	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);

	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content", 1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	cfg_script.Add("Text", "         花费");
	cfg_script.Pop();

	crCanvasNode *canvas = m_this->getParentCanvas();
	
	if (canvas && canvas->getDataClass())
	{
		int Cost = 0;
		crData * canvasData = canvas->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJJunTuanFuBenTotalTimes,param);
		unsigned char times = *(unsigned char *)param;

		canvasData->getParam(WCHDATA_JXJJunTuanFuBenId,param);
		int fubenid = *(int *)param;
		crTableIO::StrVec record;
		ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJJunTuanFuBenTab);
		if (fubentab.valid())
		{
			int doubleCostIdx = fubentab->getTitleIndex("花费");
			if(times != 4)
			{
				if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),1,crArgumentParser::appItoa(times+1),record)>=0)
					Cost = atoi(record[doubleCostIdx].c_str());
			}
		}

		if(times == 0)
			goldneed = Cost;
		if(times == 1)
			goldneed = Cost;
		if(times == 2)
			goldneed = Cost;
		if(times == 3)
			goldneed = Cost;
		if(times == 4)
			return;

		str = "刷新奖励";
	}

	cfg_script.Add("Content");
	cfg_script.Push("Content", 2);
	v_i.clear();
	v_i.push_back(140.0f);
	v_i.push_back(40.0f);
	v_i.push_back(40.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	char tmpText[32];
	sprintf(tmpText, "%d礼金/元宝\0", goldneed);
	cfg_script.Add("Text", tmpText);
	cfg_script.Pop();

	cfg_script.Add("Content");
	cfg_script.Push("Content", 3);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	cfg_script.Add("Text", str);
	cfg_script.Pop();
	


	cfg_script.Pop();
	//cfg_script.Write("tipstest.cfg");
	if(tipsText.valid()) tipsText->setHypertext(cfg_script);

	const crBoundingBox &btnbox = m_this->getBoundBox();
	const crBoundingBox &tipsbox = tips->getBoundBox();
	const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
	float posx = btnbox.m_min[0];
	float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
	////////////
	crVector2 mouse(crVector2(posx,posy));
	tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
	crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	
}

/////////////////////////////////////////
//
//crJXJUIDuoBeiHuoQvQueRenMethod
//多倍获取确认
//
/////////////////////////////////////////
crJXJUIDuoBeiHuoQvQueRenMethod::crJXJUIDuoBeiHuoQvQueRenMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJUIDuoBeiHuoQvQueRenMethod::crJXJUIDuoBeiHuoQvQueRenMethod(const crJXJUIDuoBeiHuoQvQueRenMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_index(handle.m_index)
{
}
void crJXJUIDuoBeiHuoQvQueRenMethod::inputParam(int i, void *param)
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
void crJXJUIDuoBeiHuoQvQueRenMethod::addParam(int i, const std::string& str)
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
	case 3:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJUIDuoBeiHuoQvQueRenMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
	ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
	ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
	int needgold = 0;
	std::string str;
	switch (m_index)
	{
	case 2:
		needgold = 20;
		str = "获得二倍奖励吗？";
		break;
	case 3:
		needgold = 50;
		str = "获得三倍奖励吗？";
		break;
	case 4:
		needgold = 100;
		str = "获得四倍奖励吗？";
		break;
	}
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 
	void *param;
	crData *data = playerGameData->getDataClass();
	data->getParam(WCHDATA_JXJGiftGold,param);
	int giftgold = *(int *)param;

	data->getParam(WCHDATA_Money,param);
	int money = *(int *)param;
	if (giftgold>=needgold || money>=needgold)
	{
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content",1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("Text","你确认要花费");
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
		sprintf(tmpText,"%d礼金/元宝\0",needgold);
		cfg_script.Add("Text",tmpText);
		cfg_script.Pop();

		cfg_script.Add("Content");
		cfg_script.Push("Content",3);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("Text",str);
		cfg_script.Pop();

		cfg_script.Pop();
		if(information.valid())information->setHypertext(cfg_script);

		if(okbtn.valid())okbtn->setEnable(true);
		crData *dlgData = tipscanvas->getDataClass();
		int commandtype = CDP_Widget;
		dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
		dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
		crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
	}
	else
	{
		playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIBattleBuffMethod
//战场犒赏确认框
//
//////////////////////////////////////////////////////////////////////////
crJXJUIBattleBuffMethod::crJXJUIBattleBuffMethod()
{}
crJXJUIBattleBuffMethod::crJXJUIBattleBuffMethod(const crJXJUIBattleBuffMethod& handle)
{

}
void crJXJUIBattleBuffMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUIBattleBuffMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}

void crJXJUIBattleBuffMethod::operator()(crHandle &handle)
{
	int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCaoShangHuaFeiID).c_str());
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));

	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 
	void *param;
	crData *data = playerGameData->getDataClass();
	data->getParam(WCHDATA_JXJGiftGold,param);
	int giftgold = *(int *)param;

	data->getParam(WCHDATA_Money,param);
	int money = *(int *)param;
	data->getParam(WCHDATA_JXJKaoShangFuUsedTime,param);
	_crInt64 lastkaoshangtime = *(_crInt64 *)param;
	time_t curtime = time(0);
	if (curtime - lastkaoshangtime <= 7200)
	{
		needgold = 0;
	}
	if(needgold <= giftgold + money)
	{
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);
		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content",1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("Text","消耗");
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
		sprintf(tmpText,"%d礼金/元宝\0",needgold);
		cfg_script.Add("Text",tmpText);
		cfg_script.Pop();

		cfg_script.Add("Content");
		cfg_script.Push("Content",3);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("Text","犒赏三军，提高战场所有友军攻击力5%，部队血量5%");
		cfg_script.Pop();

		cfg_script.Pop();
		msg->setHypertext(cfg_script);
		ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
		crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		crData *dlgData = msgCanvas->getDataClass();
		int commandtype = CDP_Widget;
		dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
		dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
	}
	else
	{
		playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleBuffMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleBuffMethod::crJXJBattleBuffMethod():m_index(1)
{}
crJXJBattleBuffMethod::crJXJBattleBuffMethod(const crJXJBattleBuffMethod &handle):
	crMethod(handle),m_index(handle.m_index)
{

}
void crJXJBattleBuffMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crButtonWidgetNode *)param;
		break;
	default:
		break;
	}
}

void crJXJBattleBuffMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJBattleBuffMethod::operator ()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 
	int playerID = playerGameData->getPlayerID();

	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	crData *roomData = room->getDataClass();

	roomData->getParam(WCHDATA_JXJBattleID,param);
	int battleid = *(int *)param;
	unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
	unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));

	crData *playerData = playerGameData->getDataClass();
	playerData->getParam(WCHDATA_JXJShiliID,param);
	unsigned char myshiliid = *(unsigned char *)param;

	roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
	crVector2i battlebuff = *(crVector2i *)param;


	int id = myshiliid == attackShili? 0 : 1;

	if (battlebuff[id] == 0)
	{

		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		crPlayerDataSceneEventPacket packet;
		crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvBattleRBuff,NULL);
		if(netConductor)
			netConductor->getNetManager()->sendPacket("all",packet);
	}
	else
	{
		playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(9100,NULL));//犒赏失败
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleRBuffMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleRBuffMethod::crJXJRecvBattleRBuffMethod(){}
crJXJRecvBattleRBuffMethod::crJXJRecvBattleRBuffMethod(const crJXJRecvBattleRBuffMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvBattleRBuffMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvBattleRBuffMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	//case 1:
		//m_strInput = str;
		//break;
	default:
		break;
	}
}

void crJXJRecvBattleRBuffMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == SceneServer)
		{
			int playerID = m_this->getPlayerID();
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer); 
			crNetDataManager *netDataManager = sceneServer->getNetDataManager();
			ref_ptr<crSceneServerPlayerData> playerdata = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerID));
			if(playerdata.valid())
			{
				int roomid = playerdata->getRoomID();
				ref_ptr<crSceneServerCallback>callback = dynamic_cast<crSceneServerCallback*>(netDataManager->getNetCallback());
				ref_ptr<crRoom>room = callback->findRoom(roomid);
				if(room.valid())
				{
					void *param;
					crData *roomdata = room->getDataClass();
					roomdata->excHandle(MAKEINT64(WCH_LockData,1));
					roomdata->getParam(WCHDATA_JXJBattleRBuffId,param);
					crVector2i* battleRbuffids = (crVector2i *)param;
					roomdata->getParam(WCHDATA_JXJBattleID,param);
					_crInt32 battleid = *(_crInt32 *)param;
					unsigned char ashili = LOINT16(LOINT32(battleid));
					unsigned char dshili = HIINT16(LOINT32(battleid));//(*chengchimap)[chengchiid]->getChengzhuShili();
					crData *mydata = m_this->getDataClass();
					mydata->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myShili = *(unsigned char *)param;
					if(myShili == ashili)
					{
						if((*battleRbuffids)[0]>0)
						{//
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeUChar(0);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,stream.get());
							sceneServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
						}
						else
						{
							(*battleRbuffids)[0] = playerID;
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,NULL);
							crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
							gameServer->getNetManager()->sendPacket("all",packet);
						}
					}
					else
					{
						if((*battleRbuffids)[1]>0)
						{//
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeUChar(0);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,stream.get());
							sceneServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
						}
						else
						{
							(*battleRbuffids)[1] = playerID;
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,NULL);
							crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
							gameServer->getNetManager()->sendPacket("all",packet);
						}
					}
					roomdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

		}
		else if(m_netType == GameServer)
		{
			int playerID = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
			ref_ptr<crPlayerGameData> playerGameData;
			ref_ptr<crData> playerData;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			ref_ptr<crStreamBuf> streamScene = new crStreamBuf;
			if(netDataManager)
			{
				playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
				if(playerGameServerPlayerData.valid())
				{
					playerGameData = playerGameServerPlayerData->getPlayerGameData();
					if(playerGameData.valid() && playerGameData->getDataClass())
					{
						playerData = playerGameData->getDataClass();
						playerData->excHandle(MAKEINT64(WCH_LockData,1));
					
						char rbuff = 0;
						int kaoshangcost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCaoShangHuaFeiID).c_str());

						playerData->getParam(WCHDATA_JXJGoldingot,param);
						int *goldingot = (int*)param;
						playerData->getParam(WCHDATA_JXJGiftGold,param);
						int *giftgold = (int*)param;
						playerData->getParam(WCHDATA_JXJKaoShangFuUsedTime,param);
						_crInt64 lastkaoshangtime = *(_crInt64*)param;
						time_t curtime = time(0);
						
						if (curtime -lastkaoshangtime > 7200)
						{
							if(kaoshangcost <= *goldingot +*giftgold)
							{
								//扣除元宝
								bool needdeductgold = false;
								MoneyChangeData moneydata(kaoshangcost,"战场犒赏");
								m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));

								rbuff  = 1;

								stream->createBuf(1);
								stream->_writeUChar(1);
								//stream->_writeUChar(inspireLv);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,stream.get());
								gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

								streamScene->createBuf(5);
								streamScene->_writeInt(playerID);
								streamScene->_writeChar(rbuff);
								crJXJBattleRBuffPacket packet_scene;
								crJXJBattleRBuffPacket::buildRequestPacket(packet_scene,streamScene.get());
								gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getSceneServerAddress(),packet_scene);
							}
							else
							{
								stream->createBuf(1);
								stream->_writeUChar(2);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,stream.get());
								gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

								streamScene->createBuf(5);
								streamScene->_writeInt(playerID);
								streamScene->_writeChar(rbuff);
								crJXJBattleRBuffPacket packet_scene;
								crJXJBattleRBuffPacket::buildRequestPacket(packet_scene,streamScene.get());
								gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getSceneServerAddress(),packet_scene);
							}
						}
						else
						{
							stream->createBuf(1);
							stream->_writeUChar(1);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvBattleRBuff,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

							streamScene->createBuf(5);
							streamScene->_writeInt(playerID);
							streamScene->_writeChar(rbuff);
							crJXJBattleRBuffPacket packet_scene;
							crJXJBattleRBuffPacket::buildRequestPacket(packet_scene,streamScene.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getSceneServerAddress(),packet_scene);
						}
						playerData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 

			unsigned char _case = m_stream->_readUChar();
			if (_case == 1)
			{//犒赏成功 
				
				int playerID = playerGameData->getPlayerID();

				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				crData *roomData = room->getDataClass();

				roomData->getParam(WCHDATA_JXJBattleID,param);
				int battleid = *(int *)param;
				unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
				unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));

				crData *playerData = playerGameData->getDataClass();
				playerData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myshiliid = *(unsigned char *)param;

				roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
				crVector2i *battlebuff = (crVector2i *)param;
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
				ref_ptr<crMultiSwitch> sw_donghua;
				ref_ptr<crButtonWidgetNode>  kaoshangbtn;

				int id = myshiliid == attackShili? 0 : 1;
				if(id == 0)
				{
					sw_donghua = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("sw280"));
					kaoshangbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget("Kaoshang1"));
				}
				if(id == 1)
				{
					sw_donghua = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("sw279"));
					kaoshangbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget("Kaoshang2"));
				}

				//(*battlebuff)[id] = playerID;
				sw_donghua->setActiveSwitchSet(myshiliid-10);
				sw_donghua->setVisiable(true);
				//kaoshangbtn->setEnable(false);
				//ref_ptr<crMultiSwitch> sw_kaoshang = dynamic_cast<crMultiSwitch *>(kaoshangbtn->getChild(0));
				//sw_kaoshang->setActiveSwitchSet(3);
				
			}
			else if(_case == 0)
			{//犒赏失败
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(9100,NULL));//犒赏失败
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJKaoShangCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJKaoShangCanvasUpdateMethod::crJXJKaoShangCanvasUpdateMethod()
{}
crJXJKaoShangCanvasUpdateMethod::crJXJKaoShangCanvasUpdateMethod(const crJXJKaoShangCanvasUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_kaoshangA(handle.m_kaoshangA),
	m_kaoshangD(handle.m_kaoshangD),
	m_inspireButton_attack(handle.m_inspireButton_attack),
	m_inspireButton_defend(handle.m_inspireButton_defend)
{
}
void crJXJKaoShangCanvasUpdateMethod::inputParam(int i, void *param)
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
void crJXJKaoShangCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_kaoshangA = str;
		break;
	case 1:
		m_kaoshangD = str;
		break;
	case 2:
		m_inspireButton_attack = str;
		break;
	case 3:
		m_inspireButton_defend = str;
		break;
	}
}

void crJXJKaoShangCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crButtonWidgetNode> kaoshangA = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_kaoshangA));
	ref_ptr<crButtonWidgetNode> kaoshangD = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_kaoshangD));
	ref_ptr<crButtonWidgetNode> inpireButton_A = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_inspireButton_attack));
	ref_ptr<crButtonWidgetNode> inpireButton_D = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_inspireButton_defend));
	ref_ptr<crMultiSwitch> sw_donghuaa = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("sw280"));
	ref_ptr<crMultiSwitch> sw_donghuad = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("sw279"));
	if(kaoshangA.valid() && kaoshangD.valid() && sw_donghuaa.valid() && sw_donghuad.valid())
	{
		void *param;
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();

		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		crData *roomData = room->getDataClass();

		roomData->getParam(WCHDATA_JXJBattleID,param);
		int battleid = *(int *)param;
		unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
		unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));

		crData *playerData = playerGameData->getDataClass();
		playerData->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshiliid = *(unsigned char *)param;

		roomData->getParam(WCHDATA_JXJBattleRBuffId,param);
		crVector2i battlebuff = *(crVector2i *)param;

		if (myshiliid == attackShili)
		{
			kaoshangA->setVisiable(true);
			kaoshangD->setVisiable(false);
			if (inpireButton_A.valid() && inpireButton_D.valid())
			{
				inpireButton_A->setVisiable(true);
				inpireButton_D->setVisiable(false);
			}
		} 
		else
		{
			kaoshangD->setVisiable(true);
			kaoshangA->setVisiable(false);
			if (inpireButton_A.valid() && inpireButton_D.valid())
			{
				inpireButton_D->setVisiable(true);
				inpireButton_A->setVisiable(false);
			}
		}

		if (battlebuff[0] > 0)
		{
			kaoshangA->setEnable(false);
			sw_donghuaa->setVisiable(true);
			sw_donghuaa->setActiveSwitchSet(attackShili-c_startShiliID);	
		}
		else
		{
			kaoshangA->setEnable(true);
			sw_donghuaa->setVisiable(false);
			sw_donghuaa->setActiveSwitchSet(attackShili-c_startShiliID);	
		}

		if (battlebuff[1] > 0)
		{
			kaoshangD->setEnable(false);
			sw_donghuad->setVisiable(true);
			sw_donghuad->setActiveSwitchSet(defenceShili-c_startShiliID);
		} 
		else
		{
			kaoshangD->setEnable(true);
			sw_donghuad->setVisiable(false);
			sw_donghuad->setActiveSwitchSet(defenceShili-c_startShiliID);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUILunHuiStoreAffirmMethod
//轮回仓库存储确认框
//
//////////////////////////////////////////////////////////////////////////
crJXJUILunHuiStoreAffirmMethod::crJXJUILunHuiStoreAffirmMethod():m_index(0)
{}
crJXJUILunHuiStoreAffirmMethod::crJXJUILunHuiStoreAffirmMethod(const crJXJUILunHuiStoreAffirmMethod& handle)
{

}
void crJXJUILunHuiStoreAffirmMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUILunHuiStoreAffirmMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJUILunHuiStoreAffirmMethod::operator()(crHandle &handle)
{
	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	crData *canvasData = canvas->getDataClass();
	//crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();

	canvasData->excHandle(MAKEINT64(WCH_LockData,1));
	canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
	ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
	canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
	ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
	canvasData->excHandle(MAKEINT64(WCH_LockData,0));

	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));

	std::string str;
	if (m_index == 0)
	{
		if (cardMap->empty()) return;
		str = "       您确认存储此将卡吗？";
	} 
	else
	{
		if (itemMap->empty()) return;
		str = "       您确认存储此装备吗？";
	}
	std::vector<float>v_i;
	ItemMap itemMapp;
	rcfg::ConfigScript cfg_script(&itemMapp);
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content",1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color",v_i);
	cfg_script.Add("Text",str);
	cfg_script.Pop();

	cfg_script.Pop();
	msg->setHypertext(cfg_script);
	//ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
	crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
	crData *dlgData = msgCanvas->getDataClass();
	int commandtype = CDP_Widget;
	dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
	dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
}

/////////////////////////////////////////
//
//crJXJUIUndoStoreJiangKaDragMethod
//
/////////////////////////////////////////
crJXJUIUndoStoreJiangKaDragMethod::crJXJUIUndoStoreJiangKaDragMethod()
{
}
crJXJUIUndoStoreJiangKaDragMethod::crJXJUIUndoStoreJiangKaDragMethod(const crJXJUIUndoStoreJiangKaDragMethod& handle):
	crMethod(handle),
	m_cardcanvas(handle.m_cardcanvas)
{	
}
void crJXJUIUndoStoreJiangKaDragMethod::inputParam(int i, void *param)
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
void crJXJUIUndoStoreJiangKaDragMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_cardcanvas = str;
		break;
	}
}
void crJXJUIUndoStoreJiangKaDragMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas("UI_XLunhui");
	crData *canvasData = canvas->getDataClass();

	canvasData->excHandle(MAKEINT64(WCH_LockData,1));
	canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
	ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
	canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
	ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
	canvasData->excHandle(MAKEINT64(WCH_LockData,0));

	if (!m_cardcanvas.empty())
	{
		ref_ptr<crCanvasNode>cardcanvas = crFilterRenderManager::getInstance()->findCanvas(m_cardcanvas);
		if(!cardcanvas) return;
		crData *formcarddata = cardcanvas->getDataClass();
		formcarddata->excHandle(MAKEINT64(WCH_LockData,1));
		formcarddata->getParam(WCHDATA_JXJForJKVec, param);
		JXJItemBagVec *tempcardvec = (JXJItemBagVec *)param;

		for (ItemOrCardMap::iterator iter=cardMap->begin();iter!=cardMap->end();iter++)
		{
			int cardid = iter->second->getItemID();
			if (cardid > 0)
			{
				for (JXJItemBagVec::iterator itr = tempcardvec->begin(); itr!=tempcardvec->end(); ++itr)
				{
					if ((*itr) == NULL)
					{
						ref_ptr<crBagItemData> undocard = new crBagItemData;
						undocard->setItemID(cardid);
						undocard->setItemCount(1);
						(*itr) = undocard;
						break;
					}
				}
			}
		}
		formcarddata->excHandle(MAKEINT64(WCH_LockData,0));
	}
	else
	{
		crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJItemBagVec, param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;

		for (ItemOrCardMap::iterator iter=itemMap->begin();iter!=itemMap->end();iter++)
		{
			JXJItemBagVec::iterator itr_bag = itembagvec->begin();
			for (; itr_bag != itembagvec->end(); itr_bag++)
			{
				if ((*itr_bag) == NULL)
				{
					ref_ptr<crBagItemData> temp = new crBagItemData;
					temp->setItemID(iter->second->getItemID());
					temp->setEquipMagic(iter->second->getEquipMagic());
					temp->setItemCount(1);

					*itr_bag = temp;

					break;
				}
			}
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJUILunHuiStoreTiHuanAffirmMethod
//轮回仓库存储替换确认框
//
//////////////////////////////////////////////////////////////////////////
crJXJUILunHuiStoreTiHuanAffirmMethod::crJXJUILunHuiStoreTiHuanAffirmMethod():m_index(0)
{}
crJXJUILunHuiStoreTiHuanAffirmMethod::crJXJUILunHuiStoreTiHuanAffirmMethod(const crJXJUILunHuiStoreTiHuanAffirmMethod& handle)
{

}
void crJXJUILunHuiStoreTiHuanAffirmMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crListControlWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJUILunHuiStoreTiHuanAffirmMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_canvas = str;
		break;
	case 5:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJUILunHuiStoreTiHuanAffirmMethod::operator()(crHandle &handle)
{
	void *param;
	std::string str;
	crCanvasNode *lunhuiCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(!lunhuiCanvas) return;

	crData *canvasData = lunhuiCanvas->getDataClass();
	canvasData->excHandle(MAKEINT64(WCH_LockData,1));
	canvasData->getParam(WCHDATA_JXJTempItemSwitchMap,param);
	ItemSwitchMap *switchmap = (ItemSwitchMap *)param;
	canvasData->getParam(WCHDATA_JXJTempCardSwitchMap,param);
	ItemSwitchMap *switchmapjk = (ItemSwitchMap *)param;
	canvasData->getParam(WCHDATA_JXJPlayerLunHuiReplacecount,param);
	char replacecount = *(char *)param;
	canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	if (m_index == 0)
	{
		if(switchmapjk->size() <= replacecount) return;
		str = "原先已经存在将卡,放入新的将卡会使原先的将卡消失";
	} 
	else
	{
		if(switchmap->size() <= replacecount) return;
		str = "原先已经存在装备,放入新的装备会使原先的装备消失";
	}
	
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	std::vector<float>v_i;
	ItemMap itemMapp;
	rcfg::ConfigScript cfg_script(&itemMapp);
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content",1);
	v_i.clear();
	v_i.push_back(255.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color",v_i);
	cfg_script.Add("Text",str);
	cfg_script.Pop();

	cfg_script.Pop();
	msg->setHypertext(cfg_script);
	//ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
	crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
	crData *dlgData = msgCanvas->getDataClass();
	int commandtype = CDP_Widget;
	dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
	dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);

}

/////////////////////////////////////////
//
//crJXJUIUpgradeResAddBuildingTipMethod
//资源加成筑升级提示
//
/////////////////////////////////////////
crJXJUIUpgradeResAddBuildingTipMethod::crJXJUIUpgradeResAddBuildingTipMethod() 
{
}

crJXJUIUpgradeResAddBuildingTipMethod::crJXJUIUpgradeResAddBuildingTipMethod(const crJXJUIUpgradeResAddBuildingTipMethod& handle) :
	crMethod(handle)
{
}

void crJXJUIUpgradeResAddBuildingTipMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode*)param;
		break;
	}
}

void crJXJUIUpgradeResAddBuildingTipMethod::addParam(int i, const std::string& str)
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

void crJXJUIUpgradeResAddBuildingTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;

	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

	void *param;
	char str;
	int price = 0;
	char tmText[32];
	bool conditionfull = false;

	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);

	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content", 1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);

	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>resaddbuildingcanvas = m_this->getParentCanvas();
		crTableIO::StrVec ResAddBuildingrecord;
		ref_ptr<crTableIO> ResAddBuildingTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJResAddBuildingTab);
		int needviplvindex = ResAddBuildingTab->getTitleIndex("Viplv");
		int buildinglvindex = ResAddBuildingTab->getTitleIndex("lv");
		int priceindex = ResAddBuildingTab->getTitleIndex("元宝");
		if (resaddbuildingcanvas.valid() && needviplvindex >= 0 && buildinglvindex >= 0 && priceindex >= 0)
		{
			crData *data = playerGameData->getDataClass();
			crData *mcanvasdata = resaddbuildingcanvas->getDataClass();
			mcanvasdata->getParam(WCHDATA_JXJCheckBuildingID, param);
			int buildingid = *(int *)param;
			data->getParam(buildingid, param);
			unsigned char buildinglv = *(unsigned char *)param;
			if (ResAddBuildingTab->queryOneRecord(0, crArgumentParser::appItoa(buildingid), buildinglvindex, crArgumentParser::appItoa(buildinglv + 1), ResAddBuildingrecord) >= 0)
			{
				unsigned char needviplv = atoi(ResAddBuildingrecord[needviplvindex].c_str());
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char viplv = *(unsigned char *)param;
				if (viplv >= needviplv)
				{
					price = atoi(ResAddBuildingrecord[priceindex].c_str());
					int tempprice = price;
					playerGameData->doEvent(WCH_JXJCheckLijinGold, MAKEINT64(&tempprice, NULL));
					if (tempprice == 0)
					{
						conditionfull = true;
					}
					else
					{
						str = '1';
					}
				}
				else
				{
					str = '2';
					sprintf(tmText, "  等级不足，需要VIP%d\0", needviplv);
				}
			}
		}
		else
		{
			str = '3';	
		}

		if (conditionfull)
		{
			cfg_script.Add("Text", "    需要");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			char tmpText[32];
			sprintf(tmpText, "%d礼金/元宝\0", price);
			cfg_script.Add("Text", tmpText);
			cfg_script.Pop();
		} 
		else
		{
			if (str == '2')
			{
				cfg_script.Add("Text",tmText);
			} 
			else if(str == '1')
			{
				cfg_script.Add("Text","       礼金/元宝不足");
			}
			else
			{
				cfg_script.Add("Text","    建筑已升至最高级");
			}
			cfg_script.Pop();
		}

		cfg_script.Pop();
		if(tipsText.valid()) tipsText->setHypertext(cfg_script);
		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0];
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
	}
}


/////////////////////////////////////////
//
//crJXJUILunHuidaojishiTipMethod
//轮回倒计时提示
//
/////////////////////////////////////////
crJXJUILunHuidaojishiTipMethod::crJXJUILunHuidaojishiTipMethod() 
{
}

crJXJUILunHuidaojishiTipMethod::crJXJUILunHuidaojishiTipMethod(const crJXJUILunHuidaojishiTipMethod& handle) :
	crMethod(handle)
{
}

void crJXJUILunHuidaojishiTipMethod::inputParam(int i, void *param)
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

void crJXJUILunHuidaojishiTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsname = str;
		break;
	case 2:
		m_tipsTextWidget = str;
		break;
	}
}

void crJXJUILunHuidaojishiTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if(!tips.valid())
		return;
	ref_ptr<crStaticTextWidgetNode>  tipsTextn = dynamic_cast< crStaticTextWidgetNode  *>(tips->getWidget(m_tipsname));
	ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));
	std::string str = "轮回";
	tipsTextn->setColor(crVector4(231.0,27.0,100.0,255.0)/255.0f);
	tipsTextn->setString(str);
	
	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);

	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content", 1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color", v_i);
	cfg_script.Add("Text", "本服务器已经开启轮回，将在显示时间归零后轮回");
	cfg_script.Pop();

	cfg_script.Pop();
	if(tipsText.valid()) tipsText->setHypertext(cfg_script);


	const crBoundingBox &btnbox = m_this->getBoundBox();
	const crBoundingBox &tipsbox = tips->getBoundBox();
	const crMatrix &mat = m_this->getMatrix();
	float posx = btnbox.m_min[0] + 0.2;
	float posy = (btnbox.m_min[1] - tipsbox.yLength()/2) + 0.1;
	////////////
	crVector2 mouse(crVector2(posx,posy));
	tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
	crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
}

/////////////////////////////////////////
//
//crJXJLunHuidaojishiCanvasUpdateMethod
//轮回倒计时更新
//
/////////////////////////////////////////
crJXJLunHuidaojishiCanvasUpdateMethod::crJXJLunHuidaojishiCanvasUpdateMethod() 
{
}

crJXJLunHuidaojishiCanvasUpdateMethod::crJXJLunHuidaojishiCanvasUpdateMethod(const crJXJLunHuidaojishiCanvasUpdateMethod& handle) :
	crMethod(handle)
{
}

void crJXJLunHuidaojishiCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJLunHuidaojishiCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	}
}

void crJXJLunHuidaojishiCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crStaticTextWidgetNode>  InputText = dynamic_cast< crStaticTextWidgetNode  *>(m_this->getWidget(m_input));
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJClientCycleTimer,param);
		int cycleTimer = *(int *)param;
		playerData->excHandle(MAKEINT64(WCH_LockData,0));

		int days = cycleTimer/86400;
		int hours = (cycleTimer%86400)/3600;
		int minutes = ((cycleTimer%86400)%3600)/60;
		int seconds = ((cycleTimer%86400)%3600)%60;
		char tmpText[32];
		sprintf(tmpText, "%d天%d小时%d分%d秒\0", days,hours,minutes,seconds);
		std::string str = tmpText;
		InputText->setString(str);
	}
}

/////////////////////////////////////////
//
//crJXJLunHuiRemainTimeQueryMethod
//轮回剩余时间查询
//
/////////////////////////////////////////
crJXJLunHuiRemainTimeQueryMethod::crJXJLunHuiRemainTimeQueryMethod() 
{
}

crJXJLunHuiRemainTimeQueryMethod::crJXJLunHuiRemainTimeQueryMethod(const crJXJLunHuiRemainTimeQueryMethod& handle) :
	crMethod(handle)
{
}

void crJXJLunHuiRemainTimeQueryMethod::inputParam(int i, void *param)
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

void crJXJLunHuiRemainTimeQueryMethod::addParam(int i, const std::string& str)
{
}

void crJXJLunHuiRemainTimeQueryMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCycleTimer,NULL);
	if(netConductor)
		netConductor->getNetManager()->sendPacket("all",packet);
}

//////////////////////////////////////////
//
//crJXJRecvLunHuiRemainTimerMethod
//轮回剩余时间接收
//
//////////////////////////////////////////
crJXJRecvLunHuiRemainTimerMethod::crJXJRecvLunHuiRemainTimerMethod(){}
crJXJRecvLunHuiRemainTimerMethod::crJXJRecvLunHuiRemainTimerMethod(const crJXJRecvLunHuiRemainTimerMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvLunHuiRemainTimerMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvLunHuiRemainTimerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strInput = str;
		break;
	default:
		break;
	}
}

void crJXJRecvLunHuiRemainTimerMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			int playerID = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
			if(playerGameServerPlayerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				crData * serverBrainData = crServerBrainHandle::getInstance()->getDataClass();
				serverBrainData->excHandle(MAKEINT64(WCH_LockData,1));
				serverBrainData->getParam(WCHDATA_JXJServerCycleTimer, param);
				float cycleTimer = *(float *)param;
				serverBrainData->excHandle(MAKEINT64(WCH_LockData,0));

				stream->createBuf(4);
				stream->_writeFloat(cycleTimer);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvCycleTimer,stream.get());
				gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			float cycleTimer = m_stream->_readFloat();
			int cycleTime = (int)cycleTimer;

			crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->inputParam(WCHDATA_JXJClientCycleTimer,&cycleTime);
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseJianZhuLingMethod
//
/////////////////////////////////////////
crJXJServerUseJianZhuLingMethod::crJXJServerUseJianZhuLingMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseJianZhuLingMethod::crJXJServerUseJianZhuLingMethod(const crJXJServerUseJianZhuLingMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseJianZhuLingMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseJianZhuLingMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseJianZhuLingMethod::operator()(crHandle &handle)
{
	void *param;
	int timesid = WCHDATA_JXJBuildingTime0;
	crData *data = m_user->getDataClass();
	data->excHandle(MAKEINT64(WCH_LockData,1));

	for(;timesid<WCHDATA_JXJRecruitTime0;timesid++)
	{
		data->getParam(timesid,param);
		if(param)
		{
			int* time = (int *)param;
			if (*time > 0)
			{
				*time = 0.0f;
				*m_useResult = 1;
				break;
			}
		}
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));

	if (timesid == WCHDATA_JXJRecruitTime0)
	{
		*m_useResult = USE_JianZhuLingError;
	}
	else
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeChar(1);
			stream->_writeInt(timesid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyCD,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
	
}

/////////////////////////////////////////
//
//crJXJServerUseZhengMuLingMethod
//
/////////////////////////////////////////
crJXJServerUseZhengMuLingMethod::crJXJServerUseZhengMuLingMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseZhengMuLingMethod::crJXJServerUseZhengMuLingMethod(const crJXJServerUseZhengMuLingMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseZhengMuLingMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseZhengMuLingMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseZhengMuLingMethod::operator()(crHandle &handle)
{
	void *param;
	int timesid = WCHDATA_JXJRecruitTime0;
	crData *data = m_user->getDataClass();
	data->excHandle(MAKEINT64(WCH_LockData,1));

	for(;timesid<WCHDATA_JXJCurCDHintID;timesid++)
	{
		data->getParam(timesid,param);
		if(param)
		{
			int* time = (int *)param;
			if (*time > 0)
			{
				*time = 0.0f;
				*m_useResult = 1;
				break;
			}
		}
	}
	data->excHandle(MAKEINT64(WCH_LockData,0));

	if (timesid == WCHDATA_JXJCurCDHintID)
	{
		*m_useResult = USE_JianZhuLingError;
	}
	else
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeChar(1);
			stream->_writeInt(timesid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyCD,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckJianZhuLingCountMethod
//检查背包里是否有建筑令/征募令/召集令
//
//////////////////////////////////////////////////////////////////////////
crJXJUICheckJianZhuLingCountMethod::crJXJUICheckJianZhuLingCountMethod():m_index(0)
{}
crJXJUICheckJianZhuLingCountMethod::crJXJUICheckJianZhuLingCountMethod(const crJXJUICheckJianZhuLingCountMethod& handle)
{

}
void crJXJUICheckJianZhuLingCountMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUICheckJianZhuLingCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJUICheckJianZhuLingCountMethod::operator()(crHandle &handle)
{
	bool cancelTask = false;
	int count = 0;
	unsigned int ifhint;
	int hinttype;

	void *param;
	crCanvasNode* canvas = m_this->getParentCanvas();
	crData *canvasData = canvas->getDataClass();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	playerData->getParam(WCHDATA_JXJItemBagVec,param);
	JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;

	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	crButtonWidgetNode *okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	//std::vector<std::string> record;

	char tmpText[32];
	std::string str;
	if (m_index == 0)
	{
		//if(itemtab->queryOneRecord(2,"匠造牌",record) >= 0)
		//{
			//int id = atoi(record[0].c_str());
			for (int i=0;i<itemBagVec->size();i++)
			{
				if((*itemBagVec)[i].valid())
				{
					if ((*itemBagVec)[i]->getItemID() == JiangZaoPai/*7520*/)
					{
						count += (*itemBagVec)[i]->getItemCount();
					}
				}
			
			}
			sprintf(tmpText, "%d个匠造牌\0", count);
			str = "一个匠造牌";
			playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			ifhint = *(unsigned int *)param & JXJGoldCostType_BuildingCDHint;
			hinttype = JXJGoldCostType_BuildingCDHint;
		//}
	} 
	else if(m_index == 1)
	{
		//if(itemtab->queryOneRecord(2,"征募符",record) >= 0)
		//{
			//int id = atoi(record[0].c_str());
			for (int i=0;i<itemBagVec->size();i++)
			{
				if((*itemBagVec)[i].valid())
				{
					if ((*itemBagVec)[i]->getItemID() == ZhengMuFu/*7521*/)
					{
						count += (*itemBagVec)[i]->getItemCount();
					}
				}
			
			}
			sprintf(tmpText, "%d个征募符\0", count);
			str = "一个征募符";
			playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			ifhint = *(unsigned int *)param & JXJGoldCostType_RecruitCDHint;
			hinttype = JXJGoldCostType_RecruitCDHint;
		//}
	}
	else
	{
		//if(itemtab->queryOneRecord(2,"督战令",record) >= 0)
		//{
			//int id = atoi(record[0].c_str());
			for (int i=0;i<itemBagVec->size();i++)
			{
				if((*itemBagVec)[i].valid())
				{
					if ((*itemBagVec)[i]->getItemID() == DuZhanLing /*7522*/)
					{
						count += (*itemBagVec)[i]->getItemCount();
					}
				}
			}
			sprintf(tmpText, "%d个督战令\0", count);
			str = "一个督战令";
			playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			ifhint = *(unsigned int *)param & JXJGoldCostType_DeserterCDHint;
			hinttype = JXJGoldCostType_DeserterCDHint;
		//}
	}

	if(count > 0)
	{
		cancelTask = true;
		
		if (ifhint)
		{
			crData *braindata = crBrain::getInstance()->getDataClass();
			braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

			std::vector<float>v_i;
			ItemMap itemMapp;
			rcfg::ConfigScript cfg_script(&itemMapp);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content",1);
			v_i.clear();
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text","您当前拥有");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",tmpText);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",3);
			v_i.clear();
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text","本次cd消除免费，消耗");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",4);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("Text",str);
			cfg_script.Pop();

			cfg_script.Pop();
			msg->setHypertext(cfg_script);
			//ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
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

	handle.outputParam(0,&cancelTask);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJConfirmUseJianZhuLingMethod
//建筑令/征募令/召集令,使用确认
//
//////////////////////////////////////////////////////////////////////////
crJXJConfirmUseJianZhuLingMethod::crJXJConfirmUseJianZhuLingMethod():m_index(0)
{}
crJXJConfirmUseJianZhuLingMethod::crJXJConfirmUseJianZhuLingMethod(const crJXJConfirmUseJianZhuLingMethod& handle)
{

}
void crJXJConfirmUseJianZhuLingMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJConfirmUseJianZhuLingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJConfirmUseJianZhuLingMethod::operator()(crHandle &handle)
{
	int timesid = 0;
	switch (m_index)
	{
	case 0:
		timesid = WCHDATA_JXJBuildingTime0;
		break;
	case 1:
		timesid = WCHDATA_JXJBuildingTime1;
		break;
	case 2:
		timesid = WCHDATA_JXJBuildingTime2;
		break;
	case 3:
		timesid = WCHDATA_JXJBuildingTime3;
		break;
	case 4:
		timesid = WCHDATA_JXJRecruitTime0;
		break;
	case 5:
		timesid = WCHDATA_JXJRecruitTime1;
		break;
	case 6:
		timesid = WCHDATA_JXJRecruitTime2;
		break;
	case 7:
		timesid = WCHDATA_JXJRecruitTime3;
		break;
	case 8:
		timesid = WCHDATA_JXJBuildingTime0;
		break;
	}

	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(4);
	stream->_writeInt(timesid);

	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUseJianZhuLing,stream.get());
	if(netConductor)
		netConductor->getNetManager()->sendPacket("all",packet);
}

//////////////////////////////////////////
//
//crJXJRecvConfirmUseJianZhuLingMethod
//
//////////////////////////////////////////
crJXJRecvConfirmUseJianZhuLingMethod::crJXJRecvConfirmUseJianZhuLingMethod(){}
crJXJRecvConfirmUseJianZhuLingMethod::crJXJRecvConfirmUseJianZhuLingMethod(const crJXJRecvConfirmUseJianZhuLingMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvConfirmUseJianZhuLingMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvConfirmUseJianZhuLingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strInput = str;
		break;
	default:
		break;
	}
}

void crJXJRecvConfirmUseJianZhuLingMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int timesid = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid())
			{
				//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				//std::vector<std::string> record;
				char success = 0;
				void *param;
				bool hasitem = false;
				int itemid = 0;
				int selectid = 0;
				crDataObject *dataObject = NULL;
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				//检查我背包里是否有这个itemid
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
				if (timesid>=WCHDATA_JXJBuildingTime0 && timesid<=WCHDATA_JXJBuildingTime3)
				{
					//if(itemtab->queryOneRecord(2,"匠造牌",record) >= 0)
					//{
						//int id = atoi(record[0].c_str());
						for (int i=0;i<itemBagVec->size();i++)
						{
							if((*itemBagVec)[i].valid())
							{
								if ((*itemBagVec)[i]->getItemID() == JiangZaoPai/*7520*/)
								{
									hasitem = true;
									itemid = JiangZaoPai;
									selectid = i;
									break;
								}
							}
						}
					//}
				}
				else if(timesid>=WCHDATA_JXJRecruitTime0 && timesid<=WCHDATA_JXJRecruitTime3)
				{
					//if(itemtab->queryOneRecord(2,"征募符",record) >= 0)
					//{
						//int id = atoi(record[0].c_str());
						for (int i=0;i<itemBagVec->size();i++)
						{
							if((*itemBagVec)[i].valid())
							{
								if ((*itemBagVec)[i]->getItemID() == ZhengMuFu/*7521*/)
								{
									hasitem = true;
									itemid = ZhengMuFu;
									selectid = i;
									break;
								}
							}
						}
					//}
				}
				else
				{
					//if(itemtab->queryOneRecord(2,"督战令",record) >= 0)
					//{
						//int id = atoi(record[0].c_str());
						for (int i=0;i<itemBagVec->size();i++)
						{
							if((*itemBagVec)[i].valid())
							{
								if ((*itemBagVec)[i]->getItemID() == DuZhanLing/*7522*/)
								{
									hasitem = true;
									itemid = DuZhanLing;
									selectid = i;
									break;
								}
							}
						}
					//}
				}

				if (hasitem)
				{
					data->getParam(WCHDATA_JXJItemObjectMap,param);
					ItemObjectMap *itemObjectMap = (ItemObjectMap *)param;
					ItemObjectMap::iterator itr = itemObjectMap->find(itemid);
					if(itr == itemObjectMap->end())
					{
						ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
						crTableIO::StrVec record;
						int scriptid = itemTab->getTitleIndex("脚本");
						if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
						{
							std::string script = record[scriptid];
							if(!script.empty())
							{
								crData *itemdata = CREncapsulation::loadData(crGlobalHandle::getInstance()->getScriptDir()+script);
								if(itemdata)
								{
									dataObject = new crDataObject;
									dataObject->setDataClass(itemdata);
									(*itemObjectMap)[itemid] = dataObject;
								}
							}
						}
					}
					else
					{
						dataObject = itr->second.get();
					}
					if(dataObject)
					{
						//itemtab->queryOneRecord(2,"召集令",record);
						//int id = atoi(record[0].c_str());
						char useResult = 0;
						//crVector3i param(itemid,selectid,(int)(&useResult));
						//dataObject->doEvent(MAKEINT64(WCH_JXJUseBagItem,GameServer),MAKEINT64(m_this,&param));
						if (itemid == DuZhanLing/*7522*/)
						{
							crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
							gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
							gsBrainData->getParam(WCHDATA_JXJBattleDropPlayerMap,param);
							BattleDropPlayerMap *battleDropPlayerMap = (BattleDropPlayerMap *)param;
							BattleDropPlayerMap::iterator itr;
							itr = battleDropPlayerMap->find(playerid);
							if (itr!=battleDropPlayerMap->end())
							{
								float taobingtime = itr->second.second;
								if (taobingtime > 0)
								{
									battleDropPlayerMap->erase(itr);
									useResult = 1;//秒时间成功
								}
								else
								{
									useResult = USE_JianZhuLingError;
								}
							}
							else
							{
								useResult = USE_JianZhuLingError;
							}
							gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
						} 
						else
						{
							data->getParam(timesid,param);
							if(param)
							{
								int* time = (int *)param;
								if (*time > 0)
								{
									*time = 0.0f;
									useResult = 1;
								}
								else
								{
									useResult = USE_JianZhuLingError;
								}
							}
						}
						

						if(useResult == 1)
						{//使用成功，扣除背包里的道具
							unsigned char itemcount = 0;
							if (selectid < itemBagVec->size())
							{
								if((*itemBagVec)[selectid].valid())
								{
									if ((*itemBagVec)[selectid]->getItemID() == itemid)
									{
										itemcount = (*itemBagVec)[selectid]->getItemCount();
										if (itemcount > 1)
										{
											itemcount -= 1;
											(*itemBagVec)[selectid]->setItemCount(itemcount);
										}
										else
										{
											itemcount = 0;
											(*itemBagVec)[selectid] = NULL;
										}
									}
								}
							}
							success = 1;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(7);
							stream->_writeChar(useResult);
							stream->_writeInt(itemid);
							stream->_writeUChar(selectid);
							stream->_writeUChar(itemcount);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUseBagItem,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

							if (itemid == DuZhanLing)
							{
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(1);
								stream->_writeUChar(success);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvClearPunishTime,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							} 
							else
							{
								ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
								stream1->createBuf(5);
								stream1->_writeChar(1);
								stream1->_writeInt(timesid);
								crPlayerDataEventPacket packet1;
								crPlayerDataEventPacket::buildReplyPacket(packet1,playerid,WCH_JXJRecvBuyCD,stream1.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet1);
							}
							
						}
						else
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(5);
							stream->_writeChar(useResult);
							stream->_writeInt(itemid);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUseBagItem,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				///游戏日志
				std::string logdata = "使用物品（success，itemid）：" + crArgumentParser::appVectoa(crVector2i(success,itemid));
				GameLogData gamelog(Log_UseBagItem,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));	
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseZhaoJiLingMethod
//
/////////////////////////////////////////
crJXJServerUseZhaoJiLingMethod::crJXJServerUseZhaoJiLingMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseZhaoJiLingMethod::crJXJServerUseZhaoJiLingMethod(const crJXJServerUseZhaoJiLingMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseZhaoJiLingMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseZhaoJiLingMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseZhaoJiLingMethod::operator()(crHandle &handle)
{
	void *param;
	unsigned char success = 0;
	int playerid = m_user->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (playerData.valid())
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
		gsBrainData->getParam(WCHDATA_JXJBattleDropPlayerMap,param);
		BattleDropPlayerMap *battleDropPlayerMap = (BattleDropPlayerMap *)param;
		BattleDropPlayerMap::iterator itr;
		itr = battleDropPlayerMap->find(playerid);
		if (itr!=battleDropPlayerMap->end())
		{
			float taobingtime = itr->second.second;
			if (taobingtime > 0)
			{
					battleDropPlayerMap->erase(itr);
					*m_useResult = 1;//秒时间成功
					success = 1;
			}
			else
			{
				*m_useResult = USE_JianZhuLingError;
			}
		}
		else
		{
			*m_useResult = USE_JianZhuLingError;
		}
		gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		if (success = 1)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(success);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvClearPunishTime,stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJZhengZhanShopCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJZhengZhanShopCanvasUpdateMethod::crJXJZhengZhanShopCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJZhengZhanShopCanvasUpdateMethod::crJXJZhengZhanShopCanvasUpdateMethod( const crJXJZhengZhanShopCanvasUpdateMethod & handle ):
	crMethod(handle),
	m_cZhangQuan (handle.m_cZhangQuan),
	m_scroll(handle.m_scroll)
	
{
	for (unsigned char i = 0; i < 8; ++ i)
	{
		m_sw[i] = handle.m_sw[i];
		m_imageBox[i] = handle.m_imageBox[i];
		m_count[i] = handle.m_count[i];
		m_goumaiBtn[i] = handle.m_goumaiBtn[i];
		m_needLvInput[i] = handle.m_needLvInput[i];
		m_priceInput[i] = handle.m_priceInput[i];
	}
}

void crJXJZhengZhanShopCanvasUpdateMethod::inputParam( int i, void *param )
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

void crJXJZhengZhanShopCanvasUpdateMethod::addParam( int i, const std::string& str )
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
		m_sw[i] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		m_imageBox[i-8] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		m_count[i-16] = str;
		break;
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		m_goumaiBtn[i-24] = str;
		break;
	case 32:		
	case 33:	
	case 34:	
	case 35:
	case 36:	
	case 37:
	case 38:
	case 39:
		m_priceInput[i-32] = str;
		break;
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
		m_needLvInput[i-40] = str;
		break;
	case 48:
		m_cZhangQuan = str;
		break;
	case 49:
		m_scroll = str;
		break;
	default:
		break;
	}
}

void crJXJZhengZhanShopCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		void *param;
		std::string shuLiang = "数量"; 
		std::string danJia = "商店价格";
		std::string dengJi = "等级"; 
		std::string itemidin = "itemid";
		std::string goumaishangxian = "每日购买上限";
		int val = 0;
		ref_ptr<crStaticTextWidgetNode > cZhanQuan = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cZhangQuan));
		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));
		ref_ptr<crMultiSwitch> sw[8] = {NULL};
		ref_ptr<crImageBoxWidgetNode > imageBox[8] = {NULL};
		ref_ptr<crStaticTextWidgetNode > count[8] = {NULL};
		ref_ptr<crButtonWidgetNode> gouMaiBtn[8] = {NULL};
		ref_ptr<crHypertextWidgetNode> needLvInput[8] = {NULL};
		ref_ptr<crStaticTextWidgetNode > priceInput[8] = {NULL};
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJZhengZhanShopTab);
		int cRow = itemtab->getRowCount();

		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		crData *playerData = myPlayer->getDataClass();
		playerData->getParam(WCHDATA_JXJZhengZhanQuan,param);
		int cZhanZhengQuan = *(int*)param;
		playerData->getParam(WCHDATA_JXJBuyShangXianMap,param);
		std::map<int,unsigned char> buyshangxianmap = *(std::map<int,unsigned char>*)param;
		char tmText[32];
		sprintf(tmText, "%d\0", cZhanZhengQuan);
		std::string str = tmText;
		cZhanQuan->setString(str);

		crData *mainroledata = myPlayer->getMainRole()->getDataClass();
		mainroledata->getParam(WCHDATA_Level, param);
		unsigned char mylv = *(unsigned char *)param;

		if (scroll.valid())
		{
			if (cRow < 8)
			{
				scroll->setEnable(false);
			}
			else
			{
				scroll->setEnable(true);
				int crows = cRow%4==0? cRow/4 : cRow/4+1;
				scroll->setRange(0, crows-2);
				scroll->setLineValue(1.0f);
				scroll->setPageValue(1.0f);
			}
			val = scroll->getValue();
		} 

		

		for (unsigned char i = 0; i < 8; ++ i) 
		{
			sw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw[i]));
			imageBox[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox[i]));
			count[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_count[i]));
			gouMaiBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goumaiBtn[i]));
			needLvInput[i] = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_needLvInput[i]));
			priceInput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_priceInput[i]));
			
			if(i+4*val < cRow) 
			{
				std::string counts = itemtab->getData(i+4*val,shuLiang);
				std::string needLv = itemtab->getData(i+4*val,dengJi);
				std::string price = itemtab->getData(i+4*val,danJia);
				int itemid = atoi(itemtab->getData(i+4*val,itemidin).c_str());
				unsigned char buyshangxian = atoi(itemtab->getData(i+4*val,goumaishangxian).c_str());
				int totalPrice = atoi(counts.c_str()) * atoi(price.c_str());

				unsigned char buytop = 0;
				std::map<int,unsigned char>::iterator itr = buyshangxianmap.find(itemid);
				if (itr != buyshangxianmap.end())
				{
					buytop = itr->second;
				}
				buytop = buyshangxian - buytop;

				std::vector<float>v_i;
				ItemMap itemMapp;
				rcfg::ConfigScript cfg_script(&itemMapp);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content",1);
				v_i.clear();
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				cfg_script.Add("Text","需要等级");
				cfg_script.Pop();
				cfg_script.Add("Content");
				cfg_script.Push("Content",2);
				v_i.clear();
				v_i.push_back(140.0f);
				v_i.push_back(40.0f);
				v_i.push_back(40.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color",v_i);
				cfg_script.Add("Text",needLv);
				cfg_script.Pop();

				if (sw[i].valid())
				{
					if (atoi(needLv.c_str()) > mylv)
					{
						sw[i]->setActiveSwitchSet(1);
					} 
					else
					{
						sw[i]->setActiveSwitchSet(0);
					}
				}
			
				if (imageBox[i].valid())
				{
					if (buytop>0)
					{
						imageBox[i]->setVisiable(true);
						imageBox[i]->setImageName(itemtab->getData(i+4*val,4));
					}
					else
					{
						imageBox[i]->setVisiable(false);
						//imageBox[i]->setImageName("");
					}
				}

				if (count[i].valid())
				{
					if (buytop>0)
					{
						count[i]->setVisiable(true);
						count[i]->setString(crArgumentParser::appItoa(buytop));
					}
					else
					{
						count[i]->setVisiable(false);
					}
				}

				if (gouMaiBtn[i].valid())
				{
					gouMaiBtn[i]->setVisiable(true);
					if(mylv >= atoi(needLv.c_str()) && cZhanZhengQuan >= totalPrice && cZhanZhengQuan!=0)
						gouMaiBtn[i]->setEnable(true);
					else
						gouMaiBtn[i]->setEnable(false);

					if (buytop <= 0)
						gouMaiBtn[i]->setEnable(false);
				}

				if (needLvInput[i].valid())
				{
					cfg_script.Pop();
					needLvInput[i]->setHypertext(cfg_script);
				}

				if (priceInput[i].valid())
				{
					char tmText[32];
					sprintf(tmText, "%d\0", totalPrice);
					std::string str = tmText;
					priceInput[i]->setString(str);

					if (buytop <= 0)
					{
						char tmText[32];
						sprintf(tmText, "%d\0", 0);
						std::string str = tmText;
						priceInput[i]->setString(str);
					}
				}
			}
			else
			{
				if (sw[i].valid())
					sw[i]->setActiveSwitchSet(0);

				if (imageBox[i].valid())
				{
					imageBox[i]->setVisiable(false);
					imageBox[i]->setImageName("");
				}

				if (count[i].valid())
					count[i]->setVisiable(false);

				if (gouMaiBtn[i].valid())
				{
					gouMaiBtn[i]->setVisiable(true);
					gouMaiBtn[i]->setEnable(false);
				}

				if (priceInput[i].valid())
				{
					char tmText[32];
					sprintf(tmText, "%d\0", 0);
					std::string str = tmText;
					priceInput[i]->setString(str);
				}
			}		
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJZhengZhanShopBuyMethod
//征战商店购买物品
//
//////////////////////////////////////////////////////////////////////////
crJXJZhengZhanShopBuyMethod::crJXJZhengZhanShopBuyMethod()
{}
crJXJZhengZhanShopBuyMethod::crJXJZhengZhanShopBuyMethod(const crJXJZhengZhanShopBuyMethod& handle)
{

}
void crJXJZhengZhanShopBuyMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJZhengZhanShopBuyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	//case 0:
		//m_priceInput = str;
		//break;
	case 0:
		m_imageBox = str;
		break;
	}
}

void crJXJZhengZhanShopBuyMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = m_this->getParentCanvas();
	//ref_ptr<crStaticTextWidgetNode > priceInput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_priceInput));
	ref_ptr<crImageBoxWidgetNode > imageBox = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_imageBox));
	std::string imageName = imageBox->getImageName();
	//int price = atoi(priceInput->getUTF8String().c_str());

	ref_ptr<crTableIO> ZhengZhanShop = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJZhengZhanShopTab);
	crTableIO::StrVec record;
	int itemid = 0;
	if (ZhengZhanShop->queryOneRecord(4,imageName,record) >= 0)
		itemid = atoi(record[0].c_str());

	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(4);
	//stream->_writeInt(price);
	stream->_writeInt(itemid);

	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvZhengZhanShopBuy,stream.get());
	if(netConductor)
		netConductor->getNetManager()->sendPacket("all",packet);
}

//////////////////////////////////////////
//
//crJXJRecvZhengZhanShopBuyMethod
//
//////////////////////////////////////////
crJXJRecvZhengZhanShopBuyMethod::crJXJRecvZhengZhanShopBuyMethod(){}
crJXJRecvZhengZhanShopBuyMethod::crJXJRecvZhengZhanShopBuyMethod(const crJXJRecvZhengZhanShopBuyMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvZhengZhanShopBuyMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvZhengZhanShopBuyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_animation = str;
		break;
	case 2:
		m_sequence = str;
		break;
	default:
		break;
	}
}

void crJXJRecvZhengZhanShopBuyMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int price = 0;
			unsigned char topcount = 10;
			int itemid = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (playerData.valid())
			{
				ref_ptr<crTableIO> ZhengZhanShop = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJZhengZhanShopTab);
				int shangxianindex = ZhengZhanShop->getTitleIndex("每日购买上限");
				crTableIO::StrVec record;
				if (ZhengZhanShop->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
				{
					price = atoi(record[7].c_str())*atoi(record[10].c_str());
					topcount = atoi(record[shangxianindex].c_str());
				}

				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJZhengZhanQuan,param);
				int *cZhanQuan = (int*)param;

				unsigned char buyedcount =  0;
				data->getParam(WCHDATA_JXJBuyShangXianMap,param);
				std::map<int,unsigned char> *buyshangxianmap = (std::map<int,unsigned char>*)param;
				std::map<int, unsigned char>::iterator itr = buyshangxianmap->find(itemid);
				if (itr != buyshangxianmap->end())
				{
					buyedcount = itr->second;
				}
				if (*cZhanQuan >= price && *cZhanQuan != 0 && topcount-buyedcount > 0)
				{
					ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
					//RewardItemVec rewardItems;
					//rewardItems.reserve(1);
					CRCore::ref_ptr<crBagItemData> itemPtr = new crBagItemData;
					itemPtr->setItemID(itemid);
					itemPtr->setEquipMagic(0);
					itemPtr->setItemCount(atoi(record[7].c_str()));
					//rewardItems.push_back(itemPtr);
					reward_info->getRewardItemsVec().push_back(itemPtr);
					//m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
					reward_info->setType(GP_RewardItem);
					m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward_info.get(),NULL));

					unsigned char count = 0;
					*cZhanQuan-=price;
					if (itr == buyshangxianmap->end())
					{
						(*buyshangxianmap)[itemid] = 1;
						count = 1;
					}
					else
					{
						itr->second = itr->second + 1;
						count = itr->second;
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(10);
					stream->_writeUChar(1);
					stream->_writeInt(itemid);
					stream->_writeInt(*cZhanQuan);
					stream->_writeUChar(count);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvZhengZhanShopBuy,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

					std::string str = "征战商店兑换,花费征战券";
					str += crArgumentParser::appItoa(price);
					str += "获得兵符";
					str += crArgumentParser::appItoa(itemid);
					GameLogData gamelog(Log_RecvZhengZhanShopBuy, str);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				} 
				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(0);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvZhengZhanShopBuy,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char succes = m_stream->_readUChar();
			if (succes == 1)
			{
				int itemid = m_stream->_readInt();
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				crData *playerData = myPlayer->getDataClass();
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJZhengZhanQuan,param);
				int *cZhanQuan = (int*)param;
				*cZhanQuan = m_stream->_readInt();

				playerData->getParam(WCHDATA_JXJBuyShangXianMap,param);
				std::map<int,unsigned char> *buyshangxianmap = (std::map<int,unsigned char>*)param;
				(*buyshangxianmap)[itemid] = m_stream->_readUChar();
				// 播放购买成功的动画
			/*	ref_ptr<crCanvasNode> animatinoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_animation);
				ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(animatinoCanvas->getChildNode(m_sequence));
				if (sequence.valid())
				{
					animatinoCanvas->setCanFocus(false);
					animatinoCanvas->setCanCaptureMouse(false);

					crFilterRenderManager::getInstance()->showCanvas(animatinoCanvas.get(),true);
					sequence->startEffect();
					sequence->getVanishWhenStop();
				}
				// 播放声音
				m_this->doEvent(WCH_JXJGetAwardsPlaySound); */
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				canvas->doEvent(WCH_UI_UpdateData);
			} 
			else
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9134, NULL));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvZhengZanQuanMethod
//
/////////////////////////////////////////
crJXJRecvZhengZanQuanMethod::crJXJRecvZhengZanQuanMethod():
	m_this(NULL),
	m_zhengzhanquan(0)
{
}

crJXJRecvZhengZanQuanMethod::crJXJRecvZhengZanQuanMethod(const crJXJRecvZhengZanQuanMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_zhengzhanquan(0)
{
}

void crJXJRecvZhengZanQuanMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_zhengzhanquan = LOINT64(param64);
		}
		else
		{
			m_zhengzhanquan = 0;
		}
		break;
	}
}

void crJXJRecvZhengZanQuanMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvZhengZanQuanMethod::operator()(crHandle &handle)
{
	if(m_zhengzhanquan!=0)
	{	
		void *param;
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crData *gameData = m_this->getDataClass();
			gameData->excHandle(MAKEINT64(WCH_LockData,1));
			gameData->getParam(WCHDATA_JXJZhengZhanQuan,param);
			int *cZhengZhanQuan = (int *)param;
			INTLIMITADD(*cZhengZhanQuan,m_zhengzhanquan,INT_MAX)

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(*cZhengZhanQuan);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvZhengZanQuan,stream.get());
			if (playerData.valid())
			{
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			gameData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJClientRecvZhengZanQuanMethod
//
/////////////////////////////////////////
crJXJClientRecvZhengZanQuanMethod::crJXJClientRecvZhengZanQuanMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvZhengZanQuanMethod::crJXJClientRecvZhengZanQuanMethod(const crJXJClientRecvZhengZanQuanMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvZhengZanQuanMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientRecvZhengZanQuanMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}

void crJXJClientRecvZhengZanQuanMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int cZhengZhanQuan = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJZhengZhanQuan,param);
				int extra = cZhengZhanQuan-(*(int *)param);
				data->inputParam(WCHDATA_JXJZhengZhanQuan,&cZhengZhanQuan);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Zhanquan,extra));

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJChangeFrameAndDelayHindMethod
//
/////////////////////////////////////////
crJXJChangeFrameAndDelayHindMethod::crJXJChangeFrameAndDelayHindMethod():
	m_this(NULL)
{
}
crJXJChangeFrameAndDelayHindMethod::crJXJChangeFrameAndDelayHindMethod(const crJXJChangeFrameAndDelayHindMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJChangeFrameAndDelayHindMethod::inputParam(int i, void *param)
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
void crJXJChangeFrameAndDelayHindMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJChangeFrameAndDelayHindMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this && myPlayer)
	{
		//crData *playerData = myPlayer->getDataClass();
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode >  settingBox = dynamic_cast<crCheckBoxWidgetNode*>(canvas->getWidget("ShowSetting"));
			if (settingBox.valid())
			{
				char filePath[MAX_PATH];
				std::string strPath = crGlobalHandle::getInstance()->getGameDir();
				sprintf(filePath,"%ssystem.ini\0",strPath.c_str());

				bool ifhint = settingBox->getSelect();
				std::string isCheck = crArgumentParser::appItoa(ifhint);
				::WritePrivateProfileString("frame","frameable",isCheck.c_str(),filePath);
			}
		}
	}
	
}

/////////////////////////////////////////
//
//crJXJCheckGameDalyTaskMethod
//
/////////////////////////////////////////
crJXJCheckGameDalyTaskMethod::crJXJCheckGameDalyTaskMethod():
	m_trueExecCount(1)
{
}
crJXJCheckGameDalyTaskMethod::crJXJCheckGameDalyTaskMethod(const crJXJCheckGameDalyTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckGameDalyTaskMethod::inputParam(int i, void *param)
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
void crJXJCheckGameDalyTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckGameDalyTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		if (m_trueExecCount != 0)
		{
			crData *data = playerData->getDataClass();
			if (data)
			{
				std::vector<std::string> record;
				ref_ptr<crTableIO> taskTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTab);
				if (taskTab.valid())
				{
					int paramid = taskTab->getTitleIndex("参数1");
					if (taskTab->queryOneRecord(0, crArgumentParser::appItoa(m_this->getTaskID()), record) >= 0)
					{
						int totaljifen = atoi(record[paramid].c_str());
						data->getParam(WCHDATA_JXJGuoZhanJiFen, param);
						int jifen = *(int*)param;
						if (jifen >= totaljifen)
						{
							check = true;
							if (m_trueExecCount > 0)
								m_trueExecCount--;
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&check);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvGuoZhanJiFenMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvGuoZhanJiFenMethod::crJXJRecvGuoZhanJiFenMethod(){}
crJXJRecvGuoZhanJiFenMethod::crJXJRecvGuoZhanJiFenMethod(const crJXJRecvGuoZhanJiFenMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_guozhanjifen(NULL)
{

}
void crJXJRecvGuoZhanJiFenMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_guozhanjifen = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_guozhanjifen = int (LOINT64(param64));
		}
		else
		{
			m_guozhanjifen = NULL;
		}
		break;
	}
}

void crJXJRecvGuoZhanJiFenMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvGuoZhanJiFenMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		crData *data = m_this->getDataClass();
		if (data)
		{
			void *param;
			data->excHandle(MAKEINT64(WCH_LockData, 1));
			data->getParam(WCHDATA_JXJGuoZhanJiFen,param);
			int *jifen = (int*)param;
			*jifen += m_guozhanjifen;

			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*jifen);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJClientRecvGuoZhanJiFen, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
			}
			data->excHandle(MAKEINT64(WCH_LockData, 0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvGuoZhanJiFenMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvGuoZhanJiFenMethod::crJXJClientRecvGuoZhanJiFenMethod() :
	m_time(5.0f),
	m_netType(GameClient_Game){}
crJXJClientRecvGuoZhanJiFenMethod::crJXJClientRecvGuoZhanJiFenMethod(const crJXJClientRecvGuoZhanJiFenMethod& handle) :
	crMethod(handle),
	m_time(handle.m_time),
	m_canvas(handle.m_canvas)
{
}
void crJXJClientRecvGuoZhanJiFenMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientRecvGuoZhanJiFenMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvGuoZhanJiFenMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			crData *data = m_this->getDataClass();
			int extra = 0;
			if (data)
			{
				void *param;
				int newjifen = m_stream->_readInt();
				data->excHandle(MAKEINT64(WCH_LockData, 1));
				data->getParam(WCHDATA_JXJGuoZhanJiFen,param);
				extra = newjifen - *(int*)param;
				data->inputParam(WCHDATA_JXJGuoZhanJiFen,&newjifen);
				data->excHandle(MAKEINT64(WCH_LockData, 0));
			}
			if(extra!=0)
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_GuoZhanJiFen,extra));

			ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
			if (renwucanvas.valid())
				renwucanvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckGameZhouChangTaskMethod
//
/////////////////////////////////////////
crJXJCheckGameZhouChangTaskMethod::crJXJCheckGameZhouChangTaskMethod():
	m_trueExecCount()
{
}
crJXJCheckGameZhouChangTaskMethod::crJXJCheckGameZhouChangTaskMethod(const crJXJCheckGameZhouChangTaskMethod& handle):
	crMethod(handle),
	m_trueExecCount(handle.m_trueExecCount)
{
}
void crJXJCheckGameZhouChangTaskMethod::inputParam(int i, void *param)
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
void crJXJCheckGameZhouChangTaskMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_trueExecCount = (char)(atoi(str.c_str()));
		break;
	}
}
void crJXJCheckGameZhouChangTaskMethod::operator()(crHandle &handle)
{
	bool check = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData && playerData->getDataClass())
	{
		void *param;
		//if (m_trueExecCount != 0)
		//{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
			if (canvas.valid())
			{
				crData *canvasdata = canvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJZhanLingCityCountCanvas,param);
				unsigned char citycount = *(unsigned char *)param;
				if (m_this->getTaskProgress() < 5 && citycount > m_this->getTaskProgress())
				{
					check = true;
					//if(m_trueExecCount>0)
						//m_trueExecCount--;
				}	
			}
			/*crData *data = playerData->getDataClass();
			if (data)
			{
				data->excHandle(MAKEINT64(WCH_LockData, 1));
				data->getParam(WCHDATA_JXJGuoZhanJiFen,param);
				int jifen = *(int*)param;
				data->excHandle(MAKEINT64(WCH_LockData, 0));
				if (jifen >= 100)
				{
					check = true;
					if(m_trueExecCount>0)
						m_trueExecCount--;
				}	
			}*/
		//}
	}
	handle.outputParam(0,&check);
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvZhanLingCityCountMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvZhanLingCityCountMethod::crJXJClientRecvZhanLingCityCountMethod() :
	m_time(5.0f),
	m_netType(GameClient_Game){}
crJXJClientRecvZhanLingCityCountMethod::crJXJClientRecvZhanLingCityCountMethod(const crJXJClientRecvZhanLingCityCountMethod& handle) :
	crMethod(handle),
	m_time(handle.m_time),
	m_canvas(handle.m_canvas)
{
}
void crJXJClientRecvZhanLingCityCountMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientRecvZhanLingCityCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvZhanLingCityCountMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			//void *param;
			unsigned char count = m_stream->_readUChar();
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
			ref_ptr<crCanvasNode> renwucanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
			if (canvas.valid())
			{
				crData *canvasdata = canvas->getDataClass();
				canvasdata->inputParam(WCHDATA_JXJZhanLingCityCountCanvas,&count);
			}
			if (renwucanvas.valid())
				renwucanvas->doEvent(WCH_UI_UpdateData);

			/*crRole *mainRole = m_this->getMainRole();
			if (mainRole)
			{
			crData *myMetierData = mainRole->getMetierDataClass();
			if (myMetierData)
			{
			myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
			myMetierData->getParam(WCHDATA_GameTaskMap,param);
			GameTaskMap *gameTaskMap = (GameTaskMap *)param;
			if (gameTaskMap)
			{
			for( GameTaskMap::iterator itr = gameTaskMap->begin();itr != gameTaskMap->end()&&itr->second->getTaskID()==22001;++itr )
			{
			if (count > itr->second->getTaskProgress() && itr->second->getTaskProgress() < 5)
			{
			m_this->doEvent(WCH_JXJAdvanceTask,MAKEINT64(itr->second->getTaskID(),NULL));
			}
			}
			}
			myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			}*/
		}
	}
}

crJXJActiveZhouChangTaskMethod::crJXJActiveZhouChangTaskMethod():
	m_this(NULL)
{
}

crJXJActiveZhouChangTaskMethod::crJXJActiveZhouChangTaskMethod( const crJXJActiveZhouChangTaskMethod & handle ):
	crMethod(handle)
{
}

void crJXJActiveZhouChangTaskMethod::inputParam( int i, void *param )
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

void crJXJActiveZhouChangTaskMethod::addParam( int i, const std::string& str )
{
}

void crJXJActiveZhouChangTaskMethod::operator()( crHandle &handle )
{
	void *param;
	crJXJShili * shili;
	crData * shiliData;
	crData *gsBrainData = m_this->getDataClass();
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	for (int i = 0; i < 3; ++ i)
	{
		gsBrainData->getParam(WCHDATA_JXJShiliWei+i,param);
		shili = (crJXJShili *)param;
		shiliData = shili->getDataClass();
		shiliData->excHandle(MAKEINT64(WCH_LockData,1));
		shiliData->getParam(WCHDATA_JXJChengxiang,param);
		int chengxiangid = *(int*)param;
		shiliData->getParam(WCHDATA_JXJDasima,param);
		int dasimaid = *(int*)param;
		shiliData->getParam(WCHDATA_JXJDajiangjun,param);
		int dajiangjunid = *(int*)param;
		for (int j=0;j<3;j++)
		{
			int id = 0;
			if(j == 0)
				id = chengxiangid;
			if(j == 1)
				id = dasimaid;
			if(j == 2)
				id = dajiangjunid;

			if (id != 0)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
				if (netConductor)
				{
					ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(id));
					if (playerData.valid() && playerData->getPlayerGameData())
					{
						crRole *mainRole = playerData->getPlayerGameData()->getMainRole();
						if (mainRole)
						{
							crData *myMetierData = mainRole->getMetierDataClass();
							if(myMetierData)
							{
								myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
								myMetierData->getParam(WCHDATA_GameTaskMap,param);
								GameTaskMap *gameTaskMap = (GameTaskMap *)param;
								if(gameTaskMap && !gameTaskMap->empty())
								{
									ref_ptr<crTableIO>tasktitletab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
									int taskidindex = tasktitletab->getTitleIndex("taskid");
									crTableIO::StrVec record;

									crGameTask *task;
									std::vector<int> GetTaskVec;
									crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
									crNetDataManager *netDataManager = netConductor->getNetDataManager();
									crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
									int serverid = callback->getServerID();
									crDBSavePacket dbpacket;
									ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;
									for( GameTaskMap::iterator itr = gameTaskMap->begin();
										itr != gameTaskMap->end();
										++itr )
									{
										task = itr->second.get();
										int renwutype = 0;
										if(tasktitletab->queryOneRecord(taskidindex,crArgumentParser::appItoa(task->getTaskID()),record) >= 0)
											renwutype = atoi(record[0].c_str());
										if (renwutype == 5)
										{
											task->setActivation(true);
											GetTaskVec.push_back(task->getTaskID());

											if(dbConductor)
											{//任务日志
												insertTaskrecord->buildUpdateSql(id,serverid,mainRole->getIName(),task->getTaskID(),TS_Activation,task->getTaskProgress());
												crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
												dbConductor->getNetManager()->sendPacket("all",dbpacket);
											}
										}
									}
									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(1+GetTaskVec.size()*4);
									stream->_writeUChar(GetTaskVec.size());
									for( std::vector<int>::iterator itr = GetTaskVec.begin();
										itr != GetTaskVec.end();
										++itr )
									{
										stream->_writeInt(*itr);
									}
									crJXJPlayerMetierEventPacket packet;
									crJXJPlayerMetierEventPacket::buildReplyPacket(packet,id,WCH_JXJRecvActiveTaskVec,stream.get());
									netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
								}
								myMetierData->excHandle(MAKEINT64(WCH_LockData,0));

							}
						}
					}
				}
			}
		}
		shiliData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
}


/////////////////////////////////////////
//
//crJXJSearchSceneRoomMethod
//
/////////////////////////////////////////
crJXJSearchSceneRoomMethod::crJXJSearchSceneRoomMethod()
{}
crJXJSearchSceneRoomMethod::crJXJSearchSceneRoomMethod(const crJXJSearchSceneRoomMethod& handle):
	crMethod(handle)
{
}
void crJXJSearchSceneRoomMethod::inputParam(int i, void *param)
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

void crJXJSearchSceneRoomMethod::addParam(int i, const std::string& str)
{
}

void crJXJSearchSceneRoomMethod::operator()(crHandle &handle)
{
	//bool taskCanceled = true;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		unsigned short fubenid = *(unsigned short *)param;
		ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
		crTableIO::StrVec record;
		if(fubenid>0 && worldfubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
		{
			int sceneindex = worldfubenTab->getTitleIndex("sceneid");
			int sceneid = atoi(record[sceneindex].c_str());
			if(sceneid>0)
			{
				//crWaitNetReturnStreamLogic::getLock();
				crJXJSearchSceneRoomPacket packet;
				crJXJSearchSceneRoomPacket::buildRequestPacket(packet,sceneid);
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				netConductor->getNetManager()->sendPacket("all",packet);
				//taskCanceled = false;
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJLoginWorldFuBenMethod
//
/////////////////////////////////////////
crJXJLoginWorldFuBenMethod::crJXJLoginWorldFuBenMethod(){}
crJXJLoginWorldFuBenMethod::crJXJLoginWorldFuBenMethod(const crJXJLoginWorldFuBenMethod& handle):
	crMethod(handle)
	
{
}
void crJXJLoginWorldFuBenMethod::inputParam(int i, void *param)
{
}

void crJXJLoginWorldFuBenMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJLoginWorldFuBenMethod::operator()(crHandle &handle)
{
	//bool taskCanceled = true;
	void *param;
	crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
	unsigned char type = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
	unsigned short fubenid = *(unsigned short *)param;
	//ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
	ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);

	crTableIO::StrVec record;
	if(type == FT_WorldFuben && fubenid>0 && worldfubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
	{
		int sceneindex = worldfubenTab->getTitleIndex("sceneid");
		int sceneid = atoi(record[sceneindex].c_str());
		if(sceneid>0)
		{
			//crWaitNetReturnStreamLogic::getLock();
			//crMyPlayerData::getInstance()->setLoginGameSceneMode(1);
			crGlobalHandle::getInstance()->doEvent(WCH_UINotify,MAKEINT64(1060,NULL));
			crLoginScenePacket packet;
			crLoginScenePacket::buildRequestPacket(packet,sceneid,WCH_JXJLoginWorldFuBen);
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			netConductor->getNetManager()->sendPacket("all",packet);
			//taskCanceled = false;
		}
	}
}
//
///////////////////////////////////////////
////
////crJXJSetWorldFubenIDMethod
////
///////////////////////////////////////////
//crJXJSetWorldFubenIDMethod::crJXJSetWorldFubenIDMethod()
//{
//}
//crJXJSetWorldFubenIDMethod::crJXJSetWorldFubenIDMethod(const crJXJSetWorldFubenIDMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJSetWorldFubenIDMethod::inputParam(int i, void *param)
//{
//}
//void crJXJSetWorldFubenIDMethod::addParam(int i, const std::string& str)
//{
//}
//void crJXJSetWorldFubenIDMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(myPlayer)
//	{
//		void *param;
//		crData *data = myPlayer->getDataClass();
//		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
//		unsigned char type = *(unsigned char *)param;
//		data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
//		unsigned short fubenid = *(unsigned short *)param;
//		//ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
//		ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
//
//		crTableIO::StrVec record;
//		if(type == FT_WorldFuben && fubenid>0 && worldfubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
//		{
//			ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
//			streamBuf->createBuf(3);
//			streamBuf->_writeUShort(fubenid);
//			streamBuf->_writeUChar(type);
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenID,streamBuf.get());
//			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//			netConductor->getNetManager()->sendPacket("all",packet);
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJIsWorldFuBenOrChengZhanMethod
//
/////////////////////////////////////////
crJXJIsWorldFuBenOrChengZhanMethod::crJXJIsWorldFuBenOrChengZhanMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJIsWorldFuBenOrChengZhanMethod::crJXJIsWorldFuBenOrChengZhanMethod(const crJXJIsWorldFuBenOrChengZhanMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_battlelist(handle.m_battlelist),
	m_canvas(handle.m_canvas),
	m_battlename(handle.m_battlename),
	m_index(handle.m_index)
{
}
void crJXJIsWorldFuBenOrChengZhanMethod::inputParam(int i, void *param)
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
void crJXJIsWorldFuBenOrChengZhanMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJIsWorldFuBenOrChengZhanMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crTableIO::StrVec record;
		crData *data = myPlayer->getDataClass();

		unsigned char type = FT_WorldFuben;
		unsigned short id = 0;
		std::string name = "将星台";
		ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
		if(worldfubenTab->queryOneRecord(2,name,record)>=0)
			id = atoi(record[0].c_str());

		data->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
		data->inputParam(WCHDATA_JXJFubenOrChengchiID,&id);
				
			
			
			/*unsigned short selectID = 0;
			std::string name;
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata)
			{
				int startindex = 0;
				if (m_index < 7)
				{
					canvasdata->getParam(WCHDATA_JXJCityListstartindexA,param);
					startindex = *(int *)param; 
				}
				else
				{
					canvasdata->getParam(WCHDATA_JXJCityListstartindexD,param);
					startindex = *(int *)param; 
					m_index = m_index - 7;
				}
				if (battlelist)
				{
					selectID = battlelist->getData(startindex + m_index);
				}
			}
			unsigned char type = 0;
			unsigned short id = 0;
			if(selectID>0)
			{
				ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
				ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(selectID),record)>=0)
				{
					if(record[1] == "将星台")
						name = record[1];
					else
						isChengZhan = true;		
				}

				if(worldfubenTab->queryOneRecord(2,name,record)>=0)
				{
					id = atoi(record[0].c_str());
					type = FT_WorldFuben;
				}
				
				
			}
			data->inputParam(WCHDATA_JXJFubenOrChengchiType,&type);
			data->inputParam(WCHDATA_JXJFubenOrChengchiID,&id);*/
	}
}

/////////////////////////////////////////
//
//crJXJSetWorldFubenBirthPointMethod
//
/////////////////////////////////////////
crJXJSetWorldFubenBirthPointMethod::crJXJSetWorldFubenBirthPointMethod()
{
}
crJXJSetWorldFubenBirthPointMethod::crJXJSetWorldFubenBirthPointMethod(const crJXJSetWorldFubenBirthPointMethod& handle):
	crMethod(handle)
{
}
void crJXJSetWorldFubenBirthPointMethod::inputParam(int i, void *param)
{
}
void crJXJSetWorldFubenBirthPointMethod::addParam(int i, const std::string& str)
{
}
void crJXJSetWorldFubenBirthPointMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getDataClass())
		{
			char birthpoint = 0;
			void *param;
			crData *data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myshili = *(unsigned char *)param;
			birthpoint = myshili-c_startShiliID;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeChar(birthpoint);
			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvWorldFuBenBirthPoint,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		
	}
}

/////////////////////////////////////////
//
//crJXJRecvWorldFubenBirthPointMethod
//
/////////////////////////////////////////
crJXJRecvWorldFubenBirthPointMethod::crJXJRecvWorldFubenBirthPointMethod():
	m_netType(GameClient_Game){}
crJXJRecvWorldFubenBirthPointMethod::crJXJRecvWorldFubenBirthPointMethod(const crJXJRecvWorldFubenBirthPointMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvWorldFubenBirthPointMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvWorldFubenBirthPointMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvWorldFubenBirthPointMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == SceneServer)
		{
			char index = m_stream->_readChar();
			int playerid = m_this->getPlayerID();
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer); 
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getRoomID()!=0)
			{
				crNetDataManager *netDataManager = sceneServer->getNetDataManager();
				crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				CRNetApp::crRoom* room = callback->findRoom(playerData->getRoomID());
				if(room)
				{
					crRoomPlayer *roomPlayer = room->getMember(playerid);
					if(roomPlayer)
					{
						playerData->getPlayerGameData()->doEvent(WCH_PlayerJoinRoom);
						roomPlayer->setBirthPointIndex(index);
						room->setReady(roomPlayer);
						playerData->getPlayerGameData()->doEvent(WCH_JXJInitWorldFubenPlayerReliveCount);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJInitWorldFubenPlayerReliveCountMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJInitWorldFubenPlayerReliveCountMethod::crJXJInitWorldFubenPlayerReliveCountMethod(){}
crJXJInitWorldFubenPlayerReliveCountMethod::crJXJInitWorldFubenPlayerReliveCountMethod(const crJXJInitWorldFubenPlayerReliveCountMethod &handle)
{

}
void crJXJInitWorldFubenPlayerReliveCountMethod::inputParam(int i, void *param)
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
void crJXJInitWorldFubenPlayerReliveCountMethod::addParam(int i, const std::string& str)
{

}

void crJXJInitWorldFubenPlayerReliveCountMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
	crData *playerData = m_this->getDataClass();
	if(playerData)
	{
		playerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		unsigned char relivecount = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldFuBenReliveCount,vipLevel).c_str()));
		playerData->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		unsigned short fubenid = *(unsigned short *)param;
		if(type == FT_WorldFuben && fubenid>0)
		{
			playerData->inputParam(WCHDATA_JXJFubenReliveCount,&relivecount);
		}
	}
}

///////////////////////////////////////////
////
////crJXJUIZhengZhanShopCommodityTipsMethod
////
///////////////////////////////////////////
crJXJUIZhengZhanShopCommodityTipsMethod::crJXJUIZhengZhanShopCommodityTipsMethod():
	m_ea(NULL),
	m_this(NULL),
	//m_ifexpand(false),
	//m_expandnum(1.0f),
	m_imageindex(0)
{
}
crJXJUIZhengZhanShopCommodityTipsMethod::crJXJUIZhengZhanShopCommodityTipsMethod(const crJXJUIZhengZhanShopCommodityTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJUIZhengZhanShopCommodityTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJUIZhengZhanShopCommodityTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemicon = str;
		break;
	case 4:
		m_canvas = str;
		break;
	case 5:
		m_infocanvas = str;
		break;
	case 6:
		m_infobk = str;
		break;
	case 7:
		m_infodi = str;
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 9:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 10:
		m_imageindex = atoi(str.c_str());
		break;
	}
}
void crJXJUIZhengZhanShopCommodityTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> parentcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid() && parentcanas.valid())
	{
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		if (info2)
		{
			info2->clearText();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}

		crVector4f colormode[5];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i =0; i<5;i++)
		{
			if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
			{
				if (qualityindex>=0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int nameindex = itemtab->getTitleIndex("name");
		int colorindex = itemtab->getTitleIndex("品质");

		ref_ptr<crTableIO> shoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJZhengZhanShopTab);
		int cRow = shoptab->getRowCount();
		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(parentcanas->getWidget(m_canvas));
		int val = 0;
		int itemid;
		if (scroll.valid())
			val = scroll->getValue();
		if (m_imageindex+4*val < cRow)
			itemid = atoi(shoptab->getData(m_imageindex+4*val,"itemid").c_str());

		if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
		{
			int colormodeid = atoi(record[colorindex].c_str());
			if(name)
			{
				name->setColor(colormode[colormodeid]);
				name->setString(record[nameindex]);
			}
			if (itemicon)
			{
				itemicon->setImageName(record[iconindex]);
			}
			if(info2)
			{
				ItemMap itemMap;
				rcfg::ConfigScript outscript(&itemMap);
				crVector2i item(itemid,0);
				playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
				info2->setHypertext(outscript);
			}

			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//centerTansTar[1] += 0.5f * (tmpLength - texheight);
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(infobk)infobk->setMatrix(rotmat);

				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2,posy2;
			const crBoundingBox &infos = infobk->getBoundBox();
			float posinfox = (infos.m_max[0]-infos.m_min[0]);
			float posinfoy = (infos.m_max[1]-infos.m_min[1]);
			if (pos2[0] >= 0)
			{
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + 2.0f * posy - needmove;
			}
			if((abs(posy2)+posy)>1.0f)
			{
				if (posy2 >= 0)
				{
					posy2 -= posy;
				}
				else
				{
					posy2 += posy;
				}
			}
			crVector2 mouse(crVector2(posx2,posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

///////////////////////////////////////
//
//crJXJShowMonsterInforMethod
//
///////////////////////////////////////
crJXJShowMonsterInforMethod::crJXJShowMonsterInforMethod() :
	m_zhengtingid(0),
	m_lastperHP(0.0f),
	m_lastlivecount(0)
	//m_npctableid(0),
	//m_rewardtype(0)
	//m_rewarditem(crVector3i(0,0,0))
{

}
crJXJShowMonsterInforMethod::crJXJShowMonsterInforMethod(const crJXJShowMonsterInforMethod& handle) :
	crMethod(handle),
	m_zhengtingid(handle.m_zhengtingid),
	m_lastperHP(handle.m_lastperHP),
	m_lastlivecount(handle.m_lastlivecount)
	//m_npctableid(handle.m_npctableid),
	//m_valname(handle.m_valname)
	//m_rewardtype(handle.m_rewardtype),
	//m_rewarditem(handle.m_rewarditem)

{
}
void crJXJShowMonsterInforMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zhengtingid = atoi(str.c_str());
		break;
		/*case 1:
		m_npctableid = atoi(str.c_str());
		break;*/
	//case 1:
	//	m_valname = str;
	//	break;
	//case 2:
	//	m_rewardtype = (unsigned char)(atoi(str.c_str()));
	//	break;
	//case 3:
	//	m_rewarditem = str;
		//crArgumentParser::appAtoVec(str, m_rewarditem);
	//	break;
	}
}
void crJXJShowMonsterInforMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJShowMonsterInforMethod::operator()(crHandle &handle)
{
	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
	if(scene)
	{
		ref_ptr<crInstanceItem>item;
		crData *itemData;
		//unsigned char itemstate;
		void *param;
		crData *data = m_this->getDataClass();
		if (data)
		{
			int abstracticid = 0;
			int relivecount = 0;
			//float maxhp = 0.0f;
			float perHP = 0.0f;
			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));

			data->getParam(WCHDATA_JXJBattleID,param);
			int battleid = *(int *)param;
			unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));

			ref_ptr<crSightInfo> defenceSightInfo = m_this->getOrCreateSightInfo(defenceShili);
			int drelivecount = defenceSightInfo->getItemEyeMap().size();

			data->excHandle(MAKEINT64(WCH_LockData,0));

			ref_ptr<crInstanceItem> zhengtingitem = scene->findRoomItem(m_this->getRoomID(),m_zhengtingid);
			if(zhengtingitem.valid() && zhengtingitem->getDataClass())
			{
				itemData = zhengtingitem->getDataClass();
				//zhengtingitem->doEvent(MAKEINT64(WCH_GetHP,NULL),MAKEINT64(&maxhp,NULL));
				itemData->getParam(WCHDATA_RTHP,param);
				perHP = *(float*)param;
			}
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock

			if (m_lastperHP != perHP || m_lastlivecount != drelivecount)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(8);
				stream->_writeInt(drelivecount);
				stream->_writeFloat(perHP);
				ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
				crPlayerDataEventPacket::buildReplyPacket(*packet, 0, WCH_JXJRecvFubenShowMonsterInfo, stream.get());
				m_this->sendRoomMessage(packet.get());
				m_lastperHP = perHP;
				m_lastlivecount = drelivecount;
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJRecvShowMonsterInforMethod
//
/////////////////////////////////////////
crJXJRecvShowMonsterInforMethod::crJXJRecvShowMonsterInforMethod():
	m_isstart(false)
{
}

crJXJRecvShowMonsterInforMethod::crJXJRecvShowMonsterInforMethod(const crJXJRecvShowMonsterInforMethod& handle) :
	crMethod(handle),
	m_XinxiCanvas(handle.m_XinxiCanvas),
	m_boshu(handle.m_boshu),
	m_guaiwu(handle.m_guaiwu),
	m_zhengting(handle.m_zhengting),
	m_fancanvas(handle.m_fancanvas),
	m_valname(handle.m_valname),
	m_isstart(handle.m_isstart),
	m_KillTimeText(handle.m_KillTimeText)
{
}

void crJXJRecvShowMonsterInforMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_stream = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvShowMonsterInforMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_XinxiCanvas = str;
		break;
	case 1:
		m_boshu = str;
		break;
	case 2:
		m_guaiwu = str;
		break;
	case 3:
		m_zhengting = str;
		break;
	case 4:
		m_fancanvas = str;
		break;
	case 5:
		m_valname = str;
		break;
	case 6:
		m_KillTimeText = str;
		break;
	default:
		break;
	}
}

void crJXJRecvShowMonsterInforMethod::operator()(crHandle &handle)
{
	if (m_netType == GameClient_Game)
	{
		int monsterc = m_stream->_readInt();
		float perhp = m_stream->_readFloat();
		unsigned short boshu = 0;

		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_XinxiCanvas);
		ref_ptr<crCanvasNode> fancanvas = crFilterRenderManager::getInstance()->findCanvas(m_fancanvas);
		if (canvas.valid() && fancanvas.valid())
		{
			if (canvas->getVisiable() == false && fancanvas->getVisiable() == false)
			{
				m_isstart = false;
			}
			if (monsterc > 0)
			{
				m_isstart = true;
			}
			ref_ptr<crStaticTextWidgetNode> boshuText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_boshu));
			ref_ptr<crStaticTextWidgetNode> zhengting = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_zhengting));
			ref_ptr<crStaticTextWidgetNode> guaiwu = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_guaiwu));
			ref_ptr<crStaticTextWidgetNode> killTimeText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_KillTimeText));

			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
			{
				void *param;
				ref_ptr<crData> data = room->getDataClass();
				if (data.valid())
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJScenarioVariableMap,param);
					ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
					ScenarioVariableMap::iterator svmitr = variableMap->find(m_valname);
					if (svmitr != variableMap->end())
					{
						boshu = (unsigned short)svmitr->second;
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

			if (killTimeText.valid())
			{
				killTimeText->setString(L"消灭当前波数敌军");
			}
			if (boshu == 0 && m_isstart)
			{
				boshu = 1;
			}
			if (boshuText.valid())
				boshuText->setString(crArgumentParser::appItoa((int)boshu));

			if(zhengting.valid())
				zhengting->setString(crArgumentParser::appItoa((int)perhp) /*+ "/" + crArgumentParser::appItoa((int)maxhp)*/);

			if (guaiwu.valid())
				guaiwu->setString(crArgumentParser::appItoa(monsterc));

			if (fancanvas->getVisiable() == false)
			{
				crFilterRenderManager::getInstance()->showCanvas(m_XinxiCanvas,true);
			}
			else
			{
				crFilterRenderManager::getInstance()->showCanvas(m_XinxiCanvas,false);
			}
			
		}
	}
}

crJXJActiveTaskAfterCountryFoundedMethod::crJXJActiveTaskAfterCountryFoundedMethod():
	m_this(NULL),
	m_isFirstWeek(false),
	m_shiliID(0)
{
}

crJXJActiveTaskAfterCountryFoundedMethod::crJXJActiveTaskAfterCountryFoundedMethod( const crJXJActiveTaskAfterCountryFoundedMethod & handle ):
	crMethod(handle),
	m_isFirstWeek(handle.m_isFirstWeek),
	m_shiliID(handle.m_shiliID)
{
}

void crJXJActiveTaskAfterCountryFoundedMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_isFirstWeek = (bool )(LOINT64(param64));
			m_shiliID = (unsigned char )(HIINT64(param64));
		}
		else
		{
			m_isFirstWeek = false;
			m_shiliID = 0;
		}
		break;
	}
}

void crJXJActiveTaskAfterCountryFoundedMethod::addParam( int i, const std::string& str )
{
}

void crJXJActiveTaskAfterCountryFoundedMethod::operator()( crHandle &handle )
{
	void* param;
	crData *data = m_this->getDataClass();
	data->getParam(WCHDATA_JXJShiliWei+m_shiliID-c_startShiliID,param);
	crJXJShili *jxjshili = (crJXJShili *)param;
	if(jxjshili && jxjshili->getDataClass())
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if (gameServer)
		{
			crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
			int serverid = callback->getServerID();
			crDBSavePacket dbpacket;
			ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;

			ref_ptr<crGameServerPlayerData> playerData;
			crData *shiliData = jxjshili->getDataClass();
			shiliData->excHandle(MAKEINT64(WCH_LockData,1));
			shiliData->getParam(WCHDATA_JXJPeopleMap,param);
			PeopleMap peopleMap = *(PeopleMap *)param;
			shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			for (PeopleMap::iterator itr_people = peopleMap.begin();itr_people != peopleMap.end();itr_people++)
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(itr_people->first));
				if (playerData.valid())
				{
					ref_ptr<crPlayerGameData> playergamedata = playerData->getPlayerGameData();
					if (playergamedata.valid())
					{
						ref_ptr<crRole> mainrole = playergamedata->getMainRole();
						if (mainrole.valid())
						{
							crData *myMetierData = mainrole->getMetierDataClass();
							if(myMetierData)
							{
								myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
								myMetierData->getParam(WCHDATA_GameTaskMap,param);
								GameTaskMap *gameTaskMap = (GameTaskMap *)param;
								if(!gameTaskMap||gameTaskMap->empty())
								{
									myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
									return;
								}
								GameTaskMap::iterator itr = gameTaskMap->find(21004);
								if (itr !=gameTaskMap->end())
								{
									std::vector<int> GetTaskVec;
									itr->second->setActivation(true);
									GetTaskVec.push_back(itr->second->getTaskID());

									ref_ptr<crStreamBuf> stream = new crStreamBuf;
									stream->createBuf(1+GetTaskVec.size()*4);
									stream->_writeUChar(GetTaskVec.size());
									for( std::vector<int>::iterator gitr = GetTaskVec.begin();
										gitr != GetTaskVec.end();
										++gitr )
									{
										//if(*itr == 1)
										//	firsttask = true;
										stream->_writeInt(*gitr);
									}
									crJXJPlayerMetierEventPacket packet;
									crJXJPlayerMetierEventPacket::buildReplyPacket(packet,itr_people->first,WCH_JXJRecvActiveTaskVec,stream.get());
									gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

									if(dbConductor)
									{//任务日志
										insertTaskrecord->buildUpdateSql(mainrole->getPlayerID(),serverid,mainrole->getIName(),itr->second->getTaskID(),TS_Activation,itr->second->getTaskProgress());
										crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
										dbConductor->getNetManager()->sendPacket("all",dbpacket);
									}
								}
								myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
							}
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJTreasonCountryTiShiCanvasShowMethod
//
/////////////////////////////////////////
crJXJTreasonCountryTiShiCanvasShowMethod::crJXJTreasonCountryTiShiCanvasShowMethod(){}
crJXJTreasonCountryTiShiCanvasShowMethod::crJXJTreasonCountryTiShiCanvasShowMethod(const crJXJTreasonCountryTiShiCanvasShowMethod& handle):
	crMethod(handle)
{
}
void crJXJTreasonCountryTiShiCanvasShowMethod::inputParam(int i, void *param)
{
	//switch(i) 
	//{
	//case 0:
	//	if(param == 0)
	//	{//释放
	//		m_this = NULL;
	//	}
	//	break;
	//case 1:
	//	m_this = (crCanvasNode *)param;
	//	break;
	//}
}

void crJXJTreasonCountryTiShiCanvasShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strcaenvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_type = atoi(str.c_str());
		break;
	}
}

void crJXJTreasonCountryTiShiCanvasShowMethod::operator()(crHandle &handle)
{
	m_this = crFilterRenderManager::getInstance()->findCanvas(m_strcaenvas);
	if (m_this)
	{
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input));
		ref_ptr<crTableIO> codetextTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		if (input.valid())
		{
			crTableIO::StrVec record;
			std::string str1; //"叛国限制\n1.角色担任国家官职、处于军团中不可叛国\n2.游戏处于选举日等特殊时间段内，不可叛国\n3.玩家可叛国三次，叛国需要消耗150元宝，叛国冷却时间3天\n3.叛国需要等级达到55级以上\n\n";
			std::string str2;  //"叛国惩罚\n1.角色叛逃到其他国家，将进入12小时惩罚期\n2.惩罚期内角色无法参加国战\n3.惩罚期内角色领地资源收益减少50%\n4.角色非所属国将卡自动转为将魂\n5.角色所有兵力清零";
			if (m_type == 0)
			{
				if (codetextTab->queryOneRecord(0,crArgumentParser::appItoa(9193),record) >= 0)
					str1 = record[1];
				if (codetextTab->queryOneRecord(0,crArgumentParser::appItoa(9194),record) >= 0)
					str2 = record[1];
				std::string content = str1 + str2;
				input->setString(content);
			}
			else if(m_type == 1)
			{
				if (codetextTab->queryOneRecord(0,crArgumentParser::appItoa(9202),record) >= 0)
					input->setString( record[1]);
			}
		}
		crFilterRenderManager::getInstance()->showCanvas(m_this,true);
	}
}

/////////////////////////////////////////
//
//crJXJtreasonbuffTimeupdataMethod
//
/////////////////////////////////////////
crJXJtreasonbuffTimeupdataMethod::crJXJtreasonbuffTimeupdataMethod():
	m_dt(0),
	m_totaltime(0.0f)
{
}
crJXJtreasonbuffTimeupdataMethod::crJXJtreasonbuffTimeupdataMethod(const crJXJtreasonbuffTimeupdataMethod& handle):
	crMethod(handle),
	m_dt(handle.m_dt),
	m_totaltime(handle.m_totaltime)
{
}
void crJXJtreasonbuffTimeupdataMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = *((int*)(LOINT64(param64)));
		}
		else
		{
			m_dt = 0;
		}
		break;
	}
}
void crJXJtreasonbuffTimeupdataMethod::addParam(int i, const std::string& str)
{
}
void crJXJtreasonbuffTimeupdataMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			void *param;
			int netrbuftime = 0;
			int netrpunishitime = 0;
			int netrcooltime = 0;
			bool ifneedsendpacket = false;
			crData *thisData = m_this->getDataClass();
			thisData->excHandle(MAKEINT64(WCH_LockData,1));
			thisData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
			int *trbuftime = (int *)param;
			thisData->getParam(WCHDATA_JXJTreasonPunishTimer,param);
			int *trpunishtime = (int *)param;
			thisData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
			int *trcooltime = (int *)param;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			crPlayerDataEventPacket packet;
			stream->createBuf(12);
			
			netrbuftime = *trbuftime - m_dt;
			if (netrbuftime < 0)
			{
				netrbuftime = 0.0f;
				if (*trbuftime > 0)
				{
					ifneedsendpacket = true;
				}
			}
			*trbuftime = netrbuftime;
			stream->_writeInt(*trbuftime);		
			
			netrpunishitime = *trpunishtime - m_dt;
			if (netrpunishitime < 0)
			{
				netrpunishitime = 0.0f;
				if (*trpunishtime > 0)
				{
					ifneedsendpacket = true;
				}
			}
			*trpunishtime = netrpunishitime;
			stream->_writeInt(*trpunishtime);	

			netrcooltime = *trcooltime - m_dt;
			if (netrcooltime < 0)
			{
				netrcooltime = 0.0f;
				if (*trcooltime > 0)
				{
					ifneedsendpacket = true;
				}
			}
			*trcooltime = netrcooltime;
			stream->_writeInt(*trcooltime);		

			thisData->excHandle(MAKEINT64(WCH_LockData,0));

			m_totaltime += m_dt;
			if (m_totaltime >= 20.0f)
			{
				ifneedsendpacket = true;
				m_totaltime = 0.0f;//
			}
			if (ifneedsendpacket)
			{
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvTreasonbuffTime,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvtreasonbuffTimeMethod
//
/////////////////////////////////////////
crJXJRecvtreasonbuffTimeMethod::crJXJRecvtreasonbuffTimeMethod():
	m_this(NULL)
{
}
crJXJRecvtreasonbuffTimeMethod::crJXJRecvtreasonbuffTimeMethod(const crJXJRecvtreasonbuffTimeMethod& handle):
	crMethod(handle),
	m_this(handle.m_this)
{
}
void crJXJRecvtreasonbuffTimeMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvtreasonbuffTimeMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvtreasonbuffTimeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
		int *trbuftime = (int *)param;
		*trbuftime = m_stream->_readInt();

		thisData->getParam(WCHDATA_JXJTreasonPunishTimer,param);
		int *trpunishtime = (int *)param;
		*trpunishtime = m_stream->_readInt();

		thisData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
		int *trcooltime = (int *)param;
		*trcooltime = m_stream->_readInt();

		if (*trbuftime > 0)
		{
			ref_ptr<crCanvasNode> panguobuffcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_Panguo");
			if(panguobuffcanvas.valid())
				crFilterRenderManager::getInstance()->showCanvas(panguobuffcanvas.get(),true);
		}
		if (*trpunishtime > 0)
		{
			ref_ptr<crCanvasNode> punishcanvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_Chengfaqi");
			if (punishcanvas.valid())
				crFilterRenderManager::getInstance()->showCanvas(punishcanvas.get(),true);
		}
		thisData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJtreasonbuffCanvasUpdateMethod
//叛国缓冲期更新
//
/////////////////////////////////////////
crJXJtreasonbuffCanvasUpdateMethod::crJXJtreasonbuffCanvasUpdateMethod() 
{
}

crJXJtreasonbuffCanvasUpdateMethod::crJXJtreasonbuffCanvasUpdateMethod(const crJXJtreasonbuffCanvasUpdateMethod& handle) :
	crMethod(handle)
{
}

void crJXJtreasonbuffCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJtreasonbuffCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	}
}

void crJXJtreasonbuffCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crStaticTextWidgetNode>  InputText = dynamic_cast< crStaticTextWidgetNode  *>(m_this->getWidget(m_input));
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *playerData = myPlayer->getDataClass();
	if(playerData)
	{
		playerData->getParam(WCHDATA_JXJPlayerSelectedShiLi,param);
		unsigned char selectedshili = *(unsigned char *)param; 

		bool isselectedCountrylose = false;
		myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isselectedCountrylose,&selectedshili));
		if (isselectedCountrylose)
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor)
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCancleChangShiLi,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}	
		}
		else
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
			int *treasonbuffTimer = (int *)param;
		
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myShiliid = *(unsigned char *)param;
			bool isLoseCountry = false;
			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myShiliid));
			if (isLoseCountry)
				*treasonbuffTimer = 0;

			*treasonbuffTimer -= 1;
			if(*treasonbuffTimer <= 0)
				*treasonbuffTimer = 0;

			if (*treasonbuffTimer > 0)
			{
				int hours = *treasonbuffTimer/3600;
				int minutes = (*treasonbuffTimer%3600)/60;
				int seconds = (*treasonbuffTimer%3600)%60;
				char tmpText[32];
				sprintf(tmpText, "%d小时%d分%d秒\0",hours,minutes,seconds);
				std::string str = tmpText;
				InputText->setString(str);
			}
			else
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					crPlayerDataEventPacket packet;
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(2);
					stream->_writeUChar(selectedshili);//1
					stream->_writeUChar(2);//1
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerChangeShili,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
			    }
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJPlayerCancleChangeShiliMethod
//
/////////////////////////////////////////
crJXJPlayerCancleChangeShiliMethod::crJXJPlayerCancleChangeShiliMethod()

{

}
crJXJPlayerCancleChangeShiliMethod::crJXJPlayerCancleChangeShiliMethod(const crJXJPlayerCancleChangeShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJPlayerCancleChangeShiliMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void crJXJPlayerCancleChangeShiliMethod::addParam(int i, const std::string& str)
{
	///*switch (i)
	//{
	//case 0:
	//	m_strRadioGroup = str;
	//	break;
	//}*/
}

void crJXJPlayerCancleChangeShiliMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCancleChangShiLi,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}	
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerCancleChangeShiliMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvPlayerCancleChangeShiliMethod::crJXJRecvPlayerCancleChangeShiliMethod(){}
crJXJRecvPlayerCancleChangeShiliMethod::crJXJRecvPlayerCancleChangeShiliMethod(const crJXJRecvPlayerCancleChangeShiliMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPlayerCancleChangeShiliMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvPlayerCancleChangeShiliMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_huodongcanvas = str;
		break;
	case 1:
		m_chengzhanbtn = str;
		break;
	case 2:
		m_panguocanvas = str;
		break;
	}
}

void crJXJRecvPlayerCancleChangeShiliMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
			int *buffertime = (int *)param; 
			*buffertime = 0;
			playerData->excHandle(MAKEINT64(WCH_LockData,0));

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> gplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(gplayerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvCancleChangShiLi,NULL);
				gameServer->getNetManager()->sendPacket(gplayerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if (m_netType == GameClient_Game)
	{
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
			int *buffertime = (int *)param; 
			*buffertime = 0;
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		/*ref_ptr<crCanvasNode> huodongcanvas = crFilterRenderManager::getInstance()->findCanvas(m_huodongcanvas);
		if (huodongcanvas.valid())
		{
			ref_ptr<crButtonWidgetNode> chengzhan = dynamic_cast<crButtonWidgetNode *>(huodongcanvas->getWidget(m_chengzhanbtn));
			if(chengzhan.valid())
			{
				chengzhan->setEnable(true);
				chengzhan->setVisiable(true);
			}
		}*/
		ref_ptr<crCanvasNode> panguobuffcanvas = crFilterRenderManager::getInstance()->findCanvas(m_panguocanvas);
		if(panguobuffcanvas.valid())
			crFilterRenderManager::getInstance()->showCanvas(panguobuffcanvas.get(),false);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClienttreasoncooltimeUpdateMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJClienttreasoncooltimeUpdateMethod::crJXJClienttreasoncooltimeUpdateMethod(){}
crJXJClienttreasoncooltimeUpdateMethod::crJXJClienttreasoncooltimeUpdateMethod(const crJXJClienttreasoncooltimeUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJClienttreasoncooltimeUpdateMethod::inputParam(int i, void *param)
{
}

void crJXJClienttreasoncooltimeUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJClienttreasoncooltimeUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJTreasonCoolingTimer,param);
		int *trecooltime = (int *)param;
		if(*trecooltime>0)
			(*trecooltime)--;
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}	
}

/////////////////////////////////////////
//
//crJXJtreasonpunishCanvasUpdateMethod
//叛国惩罚期更新
//
/////////////////////////////////////////
crJXJtreasonpunishCanvasUpdateMethod::crJXJtreasonpunishCanvasUpdateMethod() 
{
}

crJXJtreasonpunishCanvasUpdateMethod::crJXJtreasonpunishCanvasUpdateMethod(const crJXJtreasonpunishCanvasUpdateMethod& handle) :
	crMethod(handle)
{
}

void crJXJtreasonpunishCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJtreasonpunishCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	}
}

void crJXJtreasonpunishCanvasUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crStaticTextWidgetNode>  InputText = dynamic_cast< crStaticTextWidgetNode  *>(m_this->getWidget(m_input));
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *playerData = myPlayer->getDataClass();
	if(playerData)
	{
		playerData->excHandle(MAKEINT64(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJTreasonPunishTimer,param);
		int *trepunishtime = (int *)param;

		*trepunishtime -= 1;
		if(*trepunishtime <= 0)
			*trepunishtime = 0;

		if (*trepunishtime > 0)
		{
			int hours = *trepunishtime/3600;
			int minutes = (*trepunishtime%3600)/60;
			int seconds = (*trepunishtime%3600)%60;
			char tmpText[32];
			sprintf(tmpText, "%d小时%d分%d秒\0",hours,minutes,seconds);
			std::string str = tmpText;
			InputText->setString(str);
		}
		else
		{
			crFilterRenderManager::getInstance()->showCanvas(m_this,false);
		}
		playerData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJClearTreasonBuffTimeMethod
//
/////////////////////////////////////////
crJXJClearTreasonBuffTimeMethod::crJXJClearTreasonBuffTimeMethod()

{

}
crJXJClearTreasonBuffTimeMethod::crJXJClearTreasonBuffTimeMethod(const crJXJClearTreasonBuffTimeMethod& handle):
	crMethod(handle)
{
}
void crJXJClearTreasonBuffTimeMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}

void crJXJClearTreasonBuffTimeMethod::addParam(int i, const std::string& str)
{
	///*switch (i)
	//{
	//case 0:
	//	m_strRadioGroup = str;
	//	break;
	//}*/
}

void crJXJClearTreasonBuffTimeMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *playerData = myPlayer->getDataClass();
	if(playerData)
	{
		void *param;
		playerData->getParam(WCHDATA_JXJPlayerSelectedShiLi,param);
		unsigned char shiliid = *(unsigned char *)param; 

		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(shiliid);//1
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvClearTreasonBuffTime,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}	
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvClearTreasonBuffTimeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvClearTreasonBuffTimeMethod::crJXJRecvClearTreasonBuffTimeMethod(){}
crJXJRecvClearTreasonBuffTimeMethod::crJXJRecvClearTreasonBuffTimeMethod(const crJXJRecvClearTreasonBuffTimeMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvClearTreasonBuffTimeMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvClearTreasonBuffTimeMethod::addParam(int i, const std::string& str)
{
	/*switch (i)
	{
	case 0:
	m_huodongcanvas = str;
	break;
	case 1:
	m_chengzhanbtn = str;
	break;
	case 2:
	m_panguocanvas = str;
	break;
	}*/
}

void crJXJRecvClearTreasonBuffTimeMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		unsigned char shiliid = m_stream->_readUChar();
		crData *playerData = m_this->getDataClass();
		if(playerData)
		{
			char _case = 0;
			unsigned short chengchiid = 0;
			//playerData->getParam(WCHDATA_JXJChengchiID,param);
			//unsigned short * mychengchiID = (unsigned short *)param;
			unsigned char needgold = 10; //atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChangeShiLiNeedGold,vipLevel).c_str());
			playerData->getParam(WCHDATA_JXJGoldingot,param);
			int goldingot = *(int*)param;
			playerData->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;

			if(needgold > (goldingot + giftgold))
			{
				_case = 2;//元宝不足
			}
			else
			{
				playerData->excHandle(MAKEINT64(WCH_LockData,1));

				playerData->getParam(WCHDATA_JXJChengchiID,param);
				unsigned short * mychengchiID = (unsigned short *)param;

				bool needdeductgold = false;
				MoneyChangeData moneydata(needgold,"主动叛国");
				m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
				//叛国
				//开启惩罚期
				playerData->getParam(WCHDATA_JXJTreasonPunishTimer,param);
				int *punishtime = (int *)param; 
				*punishtime = 43200;

				//结束缓冲期
				playerData->getParam(WCHDATA_JXJTreasonBufferTimer,param);
				int *buffertime = (int *)param; 
				*buffertime = 0;

				//清除兵力
				playerData->getParam(WCHDATA_JXJTroopsMap,param);
				TroopsMap *troopsMap = (TroopsMap *)param;
				troopsMap->clear();

				m_this->doEvent(WCH_JXJChangeShili,MAKEINT64(&shiliid,NULL));
				_case = 1;
				ref_ptr<crTableIO>shiliTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				crTableIO::StrVec record;
				if(shiliTab->queryOneRecord(0,crArgumentParser::appItoa(shiliid),record)>=0)
				{
					int chengchiindex = shiliTab->getTitleIndex("默认城池");
					*mychengchiID = (unsigned short)(atoi(record[chengchiindex].c_str()));
					chengchiid = *mychengchiID;
				}
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}

			//发包客户端
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(gsplayerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if(_case == 1)
				{
					stream->createBuf(4);
					stream->_writeChar(_case);//1
					stream->_writeUChar(shiliid);//1
					stream->_writeUShort(chengchiid);//2
					
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvPlayerChangeShili,stream.get());
					gameServer->getNetManager()->sendPacket(gsplayerData->getPlayerConnectServerAddress(),packet);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(_case);
					
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvClearTreasonBuffTime,stream.get());
					gameServer->getNetManager()->sendPacket(gsplayerData->getPlayerConnectServerAddress(),packet);
				}
			}
			
		}
	}
	else if (m_netType == GameClient_Game)
	{
		//提示元宝不足
		m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(2002, NULL));
	}
}

/////////////////////////////////////////
//
//crJXJIsMyCountryLoseMethod
//
/////////////////////////////////////////

unsigned char crJXJIsMyCountryLostMethod::m_isFirstEnter = 0;

crJXJIsMyCountryLostMethod::crJXJIsMyCountryLostMethod():
	m_this(NULL)
{
}
crJXJIsMyCountryLostMethod::crJXJIsMyCountryLostMethod(const crJXJIsMyCountryLostMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{

}
void crJXJIsMyCountryLostMethod::inputParam(int i, void *param)
{
}
void crJXJIsMyCountryLostMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}
void crJXJIsMyCountryLostMethod::operator()(crHandle &handle)
{
	if(crRunGameHandle::getInstance()->isInManor())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(myPlayer && myPlayer->getDataClass())
		{
			void *param;
			crData *playerData = myPlayer->getDataClass();
			playerData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myShiliid = *(unsigned char *)param;

			bool isLoseCountry = false;
			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myShiliid));
			if(isLoseCountry && m_isFirstEnter==0 && canvas.valid() )
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);

			if (m_isFirstEnter==0)
			{
				//是否有交战城池
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJFindCityInBattle,NULL);
					netConductor->getNetManager()->sendPacket("all",packet);
				}	

				//世界战场活动是否开启
				time_t t = time(0);
				struct  tm  sTm = *localtime(&t);
				int nowtime = sTm.tm_hour*3600+sTm.tm_min*60+sTm.tm_sec;
				if ((nowtime>37800 && nowtime<39300) || (nowtime>48600 && nowtime<50100) || (nowtime>68400 && nowtime<69900))
				{
					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("Shijiefuben");
					if (canvas.valid())
					{
						ref_ptr<crStaticTextWidgetNode > text = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget("Shijie_wenbe"));
						std::string neirong = "世界战场活动已经开启，是否前往世界战场？";
						if(text.valid())
							text->setString(neirong);

						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);

						ref_ptr<crCanvasNode> guozhancanvas = crFilterRenderManager::getInstance()->findCanvas("guozhantanchu");
						if (guozhancanvas.valid())
							crFilterRenderManager::getInstance()->showCanvas(guozhancanvas.get(),false);
					}
				}
			}
			m_isFirstEnter++;
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckInTreasonPunishTimeMethod
//
/////////////////////////////////////////
crJXJCheckInTreasonPunishTimeMethod::crJXJCheckInTreasonPunishTimeMethod(){}
crJXJCheckInTreasonPunishTimeMethod::crJXJCheckInTreasonPunishTimeMethod(const crJXJCheckInTreasonPunishTimeMethod& handle):
	crMethod(handle)
{
}
void crJXJCheckInTreasonPunishTimeMethod::inputParam(int i, void *param)
{
}

void crJXJCheckInTreasonPunishTimeMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckInTreasonPunishTimeMethod::operator()(crHandle &handle)
{
	void *param;
	bool cancelTask = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getDataClass())
	{
		ref_ptr<crData> data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJTreasonPunishTimer,param);
		int punishtime = *(int *)param; 
		if(punishtime != 0) 
		{
			cancelTask = true;
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9160, NULL));
		}
	}
	//bool cancelTask = !crRunGameHandle::getInstance()->isInManor();
	handle.outputParam(0,&cancelTask);
}

/////////////////////////////////////////
//
//crJXJViewPlayerInfoMethod
//
/////////////////////////////////////////
crJXJViewPlayerInfoMethod::crJXJViewPlayerInfoMethod():
	m_this(NULL)
{
}
crJXJViewPlayerInfoMethod::crJXJViewPlayerInfoMethod(const crJXJViewPlayerInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJViewPlayerInfoMethod::inputParam(int i, void *param)
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
void crJXJViewPlayerInfoMethod::addParam(int i, const std::string& str)
{
}
void crJXJViewPlayerInfoMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && myPlayer)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if (netConductor)
		{
			void *param;
			bool myself = false;
			crData *playerdata = myPlayer->getDataClass();
			playerdata->inputParam(WCHDATA_JXJCheckIsMyself,&myself);
			crData *brain = crBrain::getInstance()->getDataClass();
			brain->excHandle(MAKEINT64(WCH_LockData,1));
			brain->getParam(WCHDATA_JXJAddFriendName,param);
			std::string nickname = *(std::string *)param;
			brain->excHandle(MAKEINT64(WCH_LockData,0));
			std::string myname = crMyPlayerData::getInstance()->getCharacterName();
			if(nickname.length()>0&&nickname.length()<15&&myname!=nickname)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4+nickname.length());
				stream->_writeString(nickname);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvPlayerInfo,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvPlayerInfoMethod
//
/////////////////////////////////////////
crJXJRecvPlayerInfoMethod::crJXJRecvPlayerInfoMethod():
	m_netType(GameClient_Game)
{
}
crJXJRecvPlayerInfoMethod::crJXJRecvPlayerInfoMethod(const crJXJRecvPlayerInfoMethod& handle):
	crMethod(handle)
{
	m_name = handle.m_name;
	m_guoJia = handle.m_guoJia;
	m_level = handle.m_level;
	m_guanzhi = handle.m_guanzhi;
	m_gongxun = handle.m_gongxun;
	m_bingfa = handle.m_bingfa;
	m_zhenfa = handle.m_zhenfa;
	m_tongshuai = handle.m_tongshuai;
	m_dongcha = handle.m_dongcha;
	m_touxiang = handle.m_touxiang;
	m_exp = handle.m_exp;
	for (int i = 0; i < 5; ++i)
	{
		m_equip[i] = handle.m_equip[i];
		m_defaultname[i] = handle.m_defaultname[i];
	}
	for (int i = 0; i < 4; ++i)
	{
		m_btnAdd[i] = handle.m_btnAdd[i];
		m_radioBtn[i] = handle.m_radioBtn[i] ;
	}
	m_radioGroup = handle.m_radioGroup;
}
void crJXJRecvPlayerInfoMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}
void crJXJRecvPlayerInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_name = str;
		break;
	case 1:
		m_guoJia = str;
		break;
	case 2:
		m_level = str;
		break;
	case 3:
		m_guanzhi = str;
		break;
	case 4:
		m_gongxun = str;
		break;
	case 5:
		m_bingfa = str;
		break;
	case 6:
		m_zhenfa = str;
		break;
	case 7:
		m_tongshuai = str;
		break;
	case 8:
		m_dongcha = str;
		break;
	case 9:
		m_equip[0] = str;
		break;
	case 10:
		m_equip[1] = str;
		break;
	case 11:
		m_equip[2] = str;
		break;
	case 12:
		m_equip[3] = str;
		break;
	case 13:
		m_equip[4] = str;
		break;
	case 14:
		m_touxiang = str;
		break;
	case 15:
		m_exp = str;
		break;
	case 16:
		m_defaultname[0] = str;
		break;
	case 17:
		m_defaultname[1] = str;
		break;
	case 18:
		m_defaultname[2] = str;
		break;
	case 19:
		m_defaultname[3] = str;
		break;
	case 20:
		m_defaultname[4] = str;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
		m_btnAdd[i-21] = str;
		break;
	case 25:
	case 26:
	case 27:
	case 28:
		m_radioBtn[i-25] = str;
		break;
	case 29:
		m_radioGroup = str;
		break;
	}
}
void crJXJRecvPlayerInfoMethod::operator()(crHandle &handle)
{	
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			if(!m_this->getMainRole())
				return;
			std::string nickname = m_stream->_readString();
			int myplayerid = m_this->getPlayerID();
			//unsigned char lv2 = 0;
			//unsigned char iconid2 = 0;
			//crData *data = m_this->getDataClass();
			std::string myname = m_this->getMainRole()->getIName2();
			//unsigned char result = 0;
			int playerid2 = 0;
			void *param;
			//std::string playername;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			if (myname != nickname)
			{
				ref_ptr<crGameServerPlayerData> playerData2;
				bool found = false;
				netDataManager->lockPlayerDataMap();//WCH 根据CharacterName找玩家
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();++itr )
				{
					playerData2 = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if(!playerData2.valid())
					{
						continue;
					}
					if(playerData2->getCharacterName() == nickname)
					{
						found = true;
						break;
					}
				}
				netDataManager->unLockPlayerDataMap();
				if(found)
				{
					unsigned char viplv = 0;
					unsigned char iconid = 0;
					unsigned char shiliid = 0;
					std::string name;
					int  curexperience = 0;
					unsigned char nowlv = 0;
					int iequipvec[5] = {0};
					_crInt32 equipmagic[5] = {0};
					unsigned char index = 0;
					unsigned char openedEquips = 0;
					float totlebingfa = 0;
					float totlezhenfa = 0;
					float totledongcha = 0;
					float totletongshuai = 0;
					short bingfaextra = 0;
					short zhenfaextra = 0;
					short dongchaextra = 0;
					short tongshuaiextra = 0;
					std::string guanzhi;
					unsigned char lv = 0;
					int exploit = 0;

					playerid2 = playerData2->getPlayerID();
					crData *data2 = playerData2->getPlayerGameData()->getDataClass();
					crRole *mainrole = playerData2->getPlayerGameData()->getMainRole();
					if (data2 && mainrole && mainrole->getDataClass())
					{
						data2 -> getParam(WCHDATA_JXJVipLv,param);
						viplv = *(unsigned char *)param;
						data2->getParam(WCHDATA_PlayerIconID,param);
						iconid = *(unsigned char *)param;
						data2->getParam(WCHDATA_JXJShiliID,param);
						shiliid = *(unsigned char *)param;
						name =  playerData2->getPlayerGameData()->getCharacterName();
					

						crData *mainroledata = mainrole->getDataClass();
						mainroledata->getParam(WCHDATA_Experience,param);
						curexperience = *(int *)param;

						mainroledata->getParam(WCHDATA_Level,param);
						nowlv = *(unsigned char *)param;

						mainroledata->getParam(WCHDATA_JXJLordEquipVec,param);
						JXJLordEquipVec equipvec = *(JXJLordEquipVec *)param;
						for (int i = 0; i<equipvec.size();i++)
						{
							iequipvec[i] = equipvec[i].first;
							equipmagic[i] = equipvec[i].second;
						}

						mainroledata->getParam(WCHDATA_JXJCurrentEquipVecIndex,param);
						index = *(unsigned char *)param;
						mainroledata->getParam(WCHDATA_JXJSavedEquipVecs,param);
						openedEquips =  *(unsigned char*)param;

						mainroledata->getParam(WCHDATA_JXJBingFa,param);
						short bingfa = *(short *)param;
						mainrole->doEvent(WCH_JXJGetBingFa,MAKEINT64(&totlebingfa,NULL));
						bingfaextra = (short)totlebingfa -  bingfa;

						mainroledata->getParam(WCHDATA_JXJZhenFa,param);
						short zhenfa = *(short *)param;
						mainrole->doEvent(WCH_JXJGetZhenFa,MAKEINT64(&totlezhenfa,NULL));
						zhenfaextra = (short)totlezhenfa - zhenfa;

						mainroledata->getParam(WCHDATA_JXJDongCha,param);
						short dongcha = *(short *)param;
						mainrole->doEvent(WCH_JXJGetDongCha,MAKEINT64(&totledongcha,NULL));
						dongchaextra = (short)totledongcha - dongcha;

						crVector3f factor;
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),factor);
						mainroledata->getParam(WCHDATA_JXJTongShuai,param);
						short tongshuai = *(short *)param;
						mainrole->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
						tongshuaiextra = (short)totletongshuai - tongshuai;

						crTableIO::StrVec govrecord;
						ref_ptr<crTableIO>postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
						int postnameid = postTab->getTitleIndex("govpost");
						mainroledata->getParam(WCHDATA_JXJGovPost,param);
						unsigned char govpost = *(unsigned char *)param;
						if (postTab->queryOneRecord(0,crArgumentParser::appItoa(govpost),govrecord) >= 0)
							guanzhi = govrecord[postnameid];

						mainroledata->getParam(WCHDATA_Level,param);
						lv = *(unsigned char *)param;

						mainroledata->getParam(WCHDATA_JXJExploit,param);
						exploit = *(int *)param;
					
						crPlayerDataEventPacket replypacket;
						ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
						stream1->createBuf(79+name.length()+guanzhi.length());
						stream1->_writeUChar(viplv);
						stream1->_writeUChar(iconid);
						stream1->_writeUChar(shiliid);
						stream1->_writeString(name);
						stream1->_writeInt(curexperience);
						stream1->_writeUChar(nowlv);
						stream1->_writeInt(iequipvec[0]);
						stream1->_writeInt(iequipvec[1]);
						stream1->_writeInt(iequipvec[2]);
						stream1->_writeInt(iequipvec[3]);
						stream1->_writeInt(iequipvec[4]);
						stream1->_writeInt(equipmagic[0]);
						stream1->_writeInt(equipmagic[1]);
						stream1->_writeInt(equipmagic[2]);
						stream1->_writeInt(equipmagic[3]);
						stream1->_writeInt(equipmagic[4]);
						stream1->_writeUChar(index);
						stream1->_writeUChar(openedEquips);
						stream1->_writeShort((short)totlebingfa);
						stream1->_writeShort((short)totlezhenfa);
						stream1->_writeShort((short)totledongcha);
						stream1->_writeShort((short)totletongshuai);
						stream1->_writeShort(bingfaextra);
						stream1->_writeShort(zhenfaextra);
						stream1->_writeShort(dongchaextra);
						stream1->_writeShort(tongshuaiextra);
						stream1->_writeString(guanzhi);
						stream1->_writeUChar(lv);
						stream1->_writeInt(exploit);
						ref_ptr<crGameServerPlayerData> senderPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(myplayerid));
						if (senderPlayerData.valid())
						{
							crPlayerDataEventPacket::buildReplyPacket(replypacket,myplayerid,WCH_JXJRecvPlayerInfo,stream1.get());
							netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),replypacket);
						}
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char viplv = m_stream->_readUChar();
			unsigned char iconid = m_stream->_readUChar();
			unsigned char shiliid = m_stream->_readUChar();
			std::string name = m_stream->_readString();
			int  curexperience = m_stream->_readInt();
			unsigned char nowlv = m_stream->_readUChar();
			int iequipvec[5] = {0};
			_crInt32 equipmagic[5] = {0};
			iequipvec[0] = m_stream->_readInt();
			iequipvec[1] = m_stream->_readInt();
			iequipvec[2] = m_stream->_readInt();
			iequipvec[3] = m_stream->_readInt();
			iequipvec[4] = m_stream->_readInt();
			equipmagic[0] = m_stream->_readInt();
			equipmagic[1] = m_stream->_readInt();
			equipmagic[2] = m_stream->_readInt();
			equipmagic[3] = m_stream->_readInt();
			equipmagic[4] = m_stream->_readInt();
			unsigned char index = m_stream->_readUChar();
			unsigned char openedEquips = m_stream->_readUChar();
			short totlebingfa = m_stream->_readShort();
			short totlezhenfa = m_stream->_readShort();
			short totledongcha = m_stream->_readShort();
			short totletongshuai = m_stream->_readShort();
			short bingfaextra = m_stream->_readShort();
			short zhenfaextra = m_stream->_readShort();
			short dongchaextra = m_stream->_readShort();
			short tongshuaiextra = m_stream->_readShort();
			std::string guanzhi = m_stream->_readString();
			unsigned char lv = m_stream->_readUChar();
			int exploit = m_stream->_readInt();

			crPlayerGameData *PlayerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			crData *playerdata = PlayerGameData->getDataClass();
			if (playerdata)
			{
				playerdata->excHandle(MAKEINT64(WCH_LockData,1));
				playerdata->getParam(WCHDATA_JXJMyFriendLordEquipVec, param);
				JXJLordEquipVec *friendEquipvec = (JXJLordEquipVec *)param;
				unsigned char equiponmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLordEquipMaxNum,viplv).c_str()));
				(*friendEquipvec).resize(equiponmax);
				for (int i = 0; i < (*friendEquipvec).size();i++)
				{
					(*friendEquipvec)[i].first = iequipvec[i];
					(*friendEquipvec)[i].second = equipmagic[i];
				}
				playerdata->excHandle(MAKEINT64(WCH_LockData,0));
			}

			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Beta2_FeudalLord");
			if (canvas.valid())
			{
				crImageBoxWidgetNode *equip[5];
				for (int i = 0; i < 5; i++)
				{
					equip[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_equip[i]));
				}

				crTableIO::StrVec record,recordd,record2,itemrecord,govrecord;
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);

				ref_ptr<crTableIO>playerIconTable = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
				if(playerIconTable.valid() && playerIconTable->queryOneRecord(0,crArgumentParser::appItoa(iconid),record) >= 0 )
				{
					//头像
					int iconindex = playerIconTable->getTitleIndex("icon2");
					ref_ptr<crImageBoxWidgetNode> image = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_touxiang));
					if(image.valid() && iconindex>=0 )image->setImageName(record[iconindex]);
				}

				///国家
				ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				if(playershilitab.valid() && playershilitab->queryOneRecord(0,crArgumentParser::appItoa(shiliid),record2) >= 0 )
				{
					int shilinameindex = playershilitab->getTitleIndex("name");
					ref_ptr<crStaticTextWidgetNode> guojia = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_guoJia));
					if(guojia.valid() && shilinameindex>=0 ) 
						guojia->setString(record2[shilinameindex]);
				}

				///名字
				ref_ptr<crStaticTextWidgetNode> namestr = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_name));
				if(namestr.valid())
					namestr->setString(name);

				//经验
				ref_ptr<crProgressWidgetNode> expProgress = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_exp));
				ref_ptr<crTableIO>LordlvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordlvTab);
				int lvid = LordlvTab->getTitleIndex("lv");
				int experience = LordlvTab->getTitleIndex("经验");
				unsigned char nextlv = nowlv+1;
				LordlvTab->queryOneRecord(lvid,crArgumentParser::appItoa((int)(nowlv)),recordd);
				LordlvTab->queryOneRecord(lvid,crArgumentParser::appItoa((int)nextlv),record);
				int maxlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevel,viplv).c_str());
				if (int(nowlv) >= maxlv)
				{
					float nowprogess = 1;
					if (expProgress.valid()) expProgress->setProgress(nowprogess);
				}
				else
				{
					if (int(nowlv) == 1)
					{
						float nowprogess = float(curexperience)/float(atoi(record[experience].c_str()) - atoi(recordd[experience].c_str()));
						if(expProgress.valid())expProgress->setProgress(nowprogess);
					} 
					else
					{
						float nowprogess = float(curexperience - atoi(recordd[experience].c_str()))/float(atoi(record[experience].c_str()) - atoi(recordd[experience].c_str()));
						if(expProgress.valid())expProgress->setProgress(nowprogess);
					}
				}

				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				int imageid = itemtab->getTitleIndex("icon");
				for (int i = 0;i<5;i++)
				{
					if(equip[i])
					{
						equip[i]->setVisiable(true);
						equip[i]->setImageName(m_defaultname[i]);
					}
					if(iequipvec[i]>0)
					{
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(iequipvec[i]),itemrecord) >= 0)
						{
							if(equip[i] && itemrecord.size()>0)equip[i]->setImageName(itemrecord[imageid]);
						}
					}
				}
				ref_ptr<crButtonWidgetNode> btnAdd[4]={NULL};
				ref_ptr<crRadioWidgetNode> radio_equips[4] = {NULL};
				ref_ptr<crRadioGroupWidgetNode> radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioGroup));
				if(radioGroup.valid())
					radioGroup->select(index);

				for (int i = 0 ; i < 4; ++i)
				{
					btnAdd[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_btnAdd[i]));
					radio_equips[i] = dynamic_cast<crRadioWidgetNode*>(canvas->getWidget(m_radioBtn[i]));
					if(btnAdd[i].valid() && radio_equips[i].valid() )
					{
						if (i <= openedEquips-1)
						{
							radio_equips[i]->setVisiable(true);
							radio_equips[i]->setEnable(false);
						} 
						else
						{
							radio_equips[i]->setVisiable(false);
						}

						if(i == openedEquips)
						{
							btnAdd[i]->setVisiable(true);
							btnAdd[i]->setEnable(false);
						}else
						{
							btnAdd[i]->setVisiable(false);
						}
					}
				}

				//兵法
				ref_ptr<crStaticTextWidgetNode> inputBingfa = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_bingfa));
				if(inputBingfa.valid())
				{
					std::string totalStr =  crArgumentParser::appItoa(totlebingfa);
					if(bingfaextra > 0 )
					{
						inputBingfa->setColor(FCOLOR_ORANGE);
						totalStr +="(+"+crArgumentParser::appItoa(bingfaextra)+")";
					}
					else
					{
						inputBingfa->setColor(FCOLOR_WHITE);
					}
					inputBingfa->setString(totalStr);
				}

				//阵法
				ref_ptr<crStaticTextWidgetNode> inputZhenfa = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_zhenfa));
				if(inputZhenfa.valid())
				{
					std::string totalStr = crArgumentParser::appItoa(totlezhenfa);
					if(zhenfaextra > 0)
					{
						inputZhenfa->setColor(FCOLOR_ORANGE);
						totalStr += "(+"+crArgumentParser::appItoa(zhenfaextra)+")";
					}
					else
					{
						inputZhenfa->setColor(FCOLOR_WHITE);
					}
					inputZhenfa->setString(totalStr);
				}

				//洞察
				ref_ptr<crStaticTextWidgetNode> inputDongCha = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_dongcha));
				if(inputDongCha.valid())
				{
					std::string totalStr = crArgumentParser::appItoa(totledongcha);
					if(dongchaextra > 0)
					{
						inputDongCha->setColor(FCOLOR_ORANGE);
						totalStr += "(+"+crArgumentParser::appItoa(dongchaextra)+")";
					}
					else
					{
						inputDongCha->setColor(FCOLOR_WHITE);
					}
					inputDongCha->setString(totalStr);
				}

				//统率
				ref_ptr<crStaticTextWidgetNode> inputTongshuai = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_tongshuai));
				if(inputTongshuai.valid())
				{
					std::string totalStr = crArgumentParser::appItoa(totletongshuai);
					if(tongshuaiextra > 0)
					{
						inputTongshuai->setColor(FCOLOR_ORANGE);
						totalStr +="(+"+crArgumentParser::appItoa(tongshuaiextra)+")";
					}
					else
					{
						inputTongshuai->setColor(FCOLOR_WHITE);
					}
					inputTongshuai->setString(totalStr);
				}

				//官职
				ref_ptr<crStaticTextWidgetNode> inputGuanzhi = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_guanzhi));
				if(inputGuanzhi.valid())
					inputGuanzhi->setString(guanzhi);

				//等级
				ref_ptr<crStaticTextWidgetNode> inputLevel = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_level));
				if(inputLevel.valid())inputLevel->setString(crArgumentParser::appItoa(lv)+std::string("级"));

				//功勋
				ref_ptr<crStaticTextWidgetNode> inputGongxun = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_gongxun));
				if(inputGongxun.valid())inputGongxun->setString(crArgumentParser::appItoa(exploit));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJNewRecvBattleBrocastMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJNewRecvBattleBrocastMethod::crJXJNewRecvBattleBrocastMethod(){}
crJXJNewRecvBattleBrocastMethod::crJXJNewRecvBattleBrocastMethod(const crJXJNewRecvBattleBrocastMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJNewRecvBattleBrocastMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJNewRecvBattleBrocastMethod::addParam(int i, const std::string& str)
{

}

void crJXJNewRecvBattleBrocastMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			bool isAttack = true;
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			crPlayerGameData * player = crMyPlayerData::getInstance()->getPlayerGameData();
			unsigned char myShili = 0;
			if (room && player)
			{
				crData *roomData = room->getDataClass();
				crData *playerData = player->getDataClass();
				if (roomData && playerData)
				{
					roomData->getParam(WCHDATA_JXJBattleID, param);
					int battleid = *(int *)param;
					unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));

					playerData->getParam(WCHDATA_JXJShiliID, param);
					myShili = *(unsigned char *)param;
					if (myShili == defenceShili)
						isAttack = false;
				}
			}

			unsigned char _case = m_stream->_readUChar();
			if (_case == 1)
			{
				/*std::string text,playerName,itemName,guojia,zhiwei;
				unsigned char shili = m_stream->_readUChar();
				unsigned char guanzhi = m_stream->_readUChar();
				playerName = m_stream->_readString();
				itemName = m_stream->_readString();
				if(shili == 10)
				guojia = "魏国";
				else if(shili == 11)
				guojia = "蜀国";
				else if(shili == 12)
				guojia = "吴国";
				else if(shili == 13)
				guojia = "群";

				if(guanzhi == 1)
				zhiwei = "丞相";
				else if(guanzhi == 2)
				zhiwei = "大司马";
				else if(guanzhi == 3)
				zhiwei = "大将军";

				if (myShili != shili)
				{
				text = std::string("恭喜玩家[") + playerName + std::string("]成功击杀[") + guojia + zhiwei + itemName + std::string("]");
				}
				else
				{
				text = std::string("[我国")  + zhiwei + itemName + std::string("]已被玩家[") + playerName + std::string("]击杀！");
				}*/
			}
			else if(_case == 2)
			{
				std::string text,playerName,guojia,zhiwei,biaoxian;
				unsigned char shili = m_stream->_readUChar();
				unsigned char guanzhi = m_stream->_readUChar();
				char deaddengji = m_stream->_readChar();
				int deadcount  = m_stream->_readInt();
				playerName = m_stream->_readString();
				if(shili == 10)
					guojia = "魏国";
				else if(shili == 11)
					guojia = "蜀国";
				else if(shili == 12)
					guojia = "吴国";
				else if(shili == 13)
					guojia = "群";

				if(guanzhi == 1)
					zhiwei = "丞相";
				else if(guanzhi == 2)
					zhiwei = "大司马";
				else if(guanzhi == 3)
					zhiwei = "大将军";
				else
					zhiwei = "大将";

				if (deaddengji == 1)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 2)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 3)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 4)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 5)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 6)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";
				else if(deaddengji == 7)
					biaoxian = ",风萧萧兮易水寒，壮士一去兮不复还!";

				//text = "风萧萧兮易水寒," + guojia + zhiwei; //+ playerName + "\n在战场中" + biaoxian;
				text = guojia + zhiwei; 

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("Can_Zhanbiaoxian");
				if (canvas.valid())
				{
				   biaoxian = " 精忠报国之心,天地可鉴！";
					biaoxian = crArgumentParser::appItoa(deadcount) + biaoxian;
					if(playerName.size() == 2)
						text = "     "+text;
					else if(playerName.size() == 3)
						text = "    "+text;
					else if(playerName.size() == 4)
						text = "   "+text;
					else if(playerName.size() == 5)
						text = "  "+text;

					ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("UI_Zhanbiaoxian"));
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);

					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);

					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content", 1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", text);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 2);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Push("Character",18);
					cfg_script.Add("Text", " "+playerName);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 3);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					std::string tmpText = " 身先士卒损兵 ";
					cfg_script.Add("Text", tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 4);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", biaoxian);
					cfg_script.Pop();

					cfg_script.Pop();
					if(contentText.valid()) contentText->setHypertext(cfg_script);

					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
			else if(_case == 3)
			{
				std::string text,playerName,guojia,zhiwei,biaoxian,chenghao,text1;
				unsigned char shili = m_stream->_readUChar();
				unsigned char guanzhi = m_stream->_readUChar();
				char killdengji = m_stream->_readChar();
				int killount  = m_stream->_readInt();
				playerName = m_stream->_readString();
				if(shili == 10)
					guojia = "魏国";
				else if(shili == 11)
					guojia = "蜀国";
				else if(shili == 12)
					guojia = "吴国";
				else if(shili == 13)
					guojia = "群";

				if(guanzhi == 1)
					zhiwei = "丞相";
				else if(guanzhi == 2)
					zhiwei = "大司马";
				else if(guanzhi == 3)
					zhiwei = "大将军";
				else
					zhiwei = "大将";

				
				if(killdengji == 3)
					biaoxian = "，特赏人中豪杰封号！";
				else if(killdengji == 4)
				{
					chenghao = "荣膺万人斩！";
					biaoxian = "，特赏乱世豪杰封号！";
				}
				else if(killdengji == 5)
				{
					biaoxian = "，特赏只手遮天封号!";
				}
				else if(killdengji == 7)
					biaoxian = "，特赏威震八方封号!";
				else if(killdengji == 8)
					biaoxian = "，特赏横扫千军封号！";
				else if(killdengji == 9)
					biaoxian = "，特赏万夫莫敌封号！";
				else if(killdengji == 10)
					biaoxian = "，特赏杀戮之神封号！";
				else if(killdengji == 11)
					biaoxian = "，特赏杀戮之神封号！";
				else if(killdengji == 12)
					biaoxian = "，特赏杀戮之神封号！";

				//text = "风起云涌,战火四起," + guojia + zhiwei; //+ playerName + "\n在战场中" + biaoxian;
				text = guojia + zhiwei;
				text1 = "[战报]：恭喜" + guojia + "玩家" + playerName + chenghao; 

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("Can_Zhanbiaoxian");
				if (canvas.valid())
				{
					biaoxian = ",勇冠三军,如武神下凡！";

					if(playerName.size() == 2)
						text = "    "+text;
					else if(playerName.size() == 3)
						text = "   "+text;
					else if(playerName.size() == 4)
						text = "  "+text;
					else if(playerName.size() == 5)
						text = " "+text;

					ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("UI_Zhanbiaoxian"));
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);

					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);

					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content", 1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", text);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 2);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Push("Character",18);
					cfg_script.Add("Text", " "+playerName);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 3);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					std::string tmpText = " 冲锋陷阵杀敌 ";
					cfg_script.Add("Text", tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 4);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", crArgumentParser::appItoa(killount));
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content", 5);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", biaoxian);
					cfg_script.Pop();

					cfg_script.Pop();
					if(contentText.valid()) contentText->setHypertext(cfg_script);

					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
			else if(_case == 4)
			{
				/*std::string text,playerName,itemName;
				char type = m_stream->_readChar();
				playerName = m_stream->_readString();
				itemName = m_stream->_readString();
				if (isAttack)
				{
				if (type == 0)
				text = std::string("恭喜玩家[") + playerName + std::string("]成功击破[") + itemName + std::string("]");
				else if (type == 1)
				text = std::string("恭喜玩家[") + playerName + std::string("]成功杀死[") + itemName + std::string("]");
				}
				else
				{
				if (type == 0)
				text = std::string("[") + itemName + std::string("]已被玩家[") + playerName + std::string("]击破！");
				else if (type == 1)
				text = std::string("[") + itemName + std::string("]已被玩家[") + playerName + std::string("]杀死！");
				}*/
			}

			//ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
			//record->setChatText(text);
			////record->setTextColor(COLOR_GREEN);
			//record->setChatChannelName("[战场]");
			//crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
		}
	}
}

/////////////////////////////////////////
//
//crJXJIsLostCountryofFoundMethod
//
/////////////////////////////////////////
crJXJIsLostCountryofFoundMethod::crJXJIsLostCountryofFoundMethod():
	m_this(NULL)
{
}
crJXJIsLostCountryofFoundMethod::crJXJIsLostCountryofFoundMethod(const crJXJIsLostCountryofFoundMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{

}
void crJXJIsLostCountryofFoundMethod::inputParam(int i, void *param)
{
}
void crJXJIsLostCountryofFoundMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_citywei = str;
		break;
	case 2:
		m_cityshu = str;
		break;
	case 3:
		m_citywu = str;
		break;
	}
}
void crJXJIsLostCountryofFoundMethod::operator()(crHandle &handle)
{
	if(crRunGameHandle::getInstance()->isInManor())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(myPlayer && myPlayer->getDataClass())
		{
			if (crJXJIsMyCountryLostMethod::m_isFirstEnter == 1)
			{
				void *param;
				crData *playerData = myPlayer->getDataClass();
				playerData->getParam(WCHDATA_JXJOpenBattle,param);
				bool openBattle = *(bool *)param;
				if (openBattle)
				{
					playerData->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myShiliid = *(unsigned char *)param;

					std::string city;
					bool flag = false;
					playerData->getParam(WCHDATA_JXJChengChiMap,param);
					ChengchiMap *map_city = (ChengchiMap *)param;
					if (myShiliid == 10)
						city = m_citywei;
					else if(myShiliid == 11)
						city = m_cityshu;
					else
						city = m_citywu;

					std::vector<std::string> strCity;
					std::vector<int> index;
					for (int i=0;i!=city.size();i++)
					{
						if(city[i] == '|')
							index.push_back(i);
					}
					for (int i=0;i<=index.size();i++)
					{
						if(i == 0)
							strCity.push_back(city.substr(0,index[0]));
						else if (i == index.size())
							strCity.push_back(city.substr(index[i-1]+1,city.size()-index[i-1]-1));
						else
							strCity.push_back(city.substr(index[i-1]+1,index[i]-index[i-1]-1));
					}
					for (int i=0;i!=strCity.size();i++)
					{
						ChengchiMap::iterator itr = map_city->find(atoi(strCity[i].c_str()));
						if (itr != map_city->end())
							if (myShiliid != itr->second->getChengzhuShili() && itr->second->getChengzhuShili() != 2)
							{
								flag = true;
								break;
							}
					}
				
					if(canvas.valid() && flag == true)
					{
						crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
						if(!playerData) return;
						crData *roledata = playerData->getMainRole()->getDataClass();
						roledata->getParam(WCHDATA_Level, param);
						unsigned char characterlevel = *(unsigned char *)param;
						if (characterlevel >= 10)
						{
							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);
							ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("wenbe"));

							std::vector<float>v_i;
							ItemMap itemMap;
							rcfg::ConfigScript cfg_script(&itemMap);

							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");

							cfg_script.Add("Content");
							cfg_script.Push("Content", 1);
							v_i.clear();
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color", v_i);
							std::string tmpText = "\n国战告急，建国城池已被攻占，请主公今日加大兵力，共同守护本国荣耀。";
							cfg_script.Add("Text", tmpText);
							cfg_script.Pop();

							cfg_script.Pop();
							if(contentText.valid()) contentText->setHypertext(cfg_script);

							bool sign =  true;
							ref_ptr<crCanvasNode> shijiecanvas = crFilterRenderManager::getInstance()->findCanvas("Shijiefuben");
							if (shijiecanvas.valid() && shijiecanvas->getVisiable())
								sign = false;

							if(sign)
								crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJIsMyCountryInBattleMethod
//
/////////////////////////////////////////
//crJXJIsMyCountryInBattleMethod::crJXJIsMyCountryInBattleMethod():
//	m_netType(GameClient_Game){}
//crJXJIsMyCountryInBattleMethod::crJXJIsMyCountryInBattleMethod(const crJXJIsMyCountryInBattleMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJIsMyCountryInBattleMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_stream = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crPlayerGameData*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_stream = (crStreamBuf *)(LOINT64(param64));
//			m_netType = HIINT64(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void crJXJIsMyCountryInBattleMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_battlecanvas = str;
//		break;
//	}
//}
//
//void crJXJIsMyCountryInBattleMethod::operator()(crHandle &handle)
//{
//	if (m_this)
//	{
//		crCanvasNode* battlecanvas = crFilterRenderManager::getInstance()->findCanvas(m_battlecanvas);
//		if (battlecanvas)
//		{
//			void *param;
//			crData *data = battlecanvas->getDataClass();
//			if (data)
//			{
//				data->excHandle(MAKEINT64(WCH_LockData,1));
//				data->getParam(WCHDATA_JXJIsFirstIniUIBattleData,param);
//				unsigned char *isfirst = (unsigned char *)param;
//
//				if (*isfirst == 0)
//				{
//					int count = 0;
//
//					data->getParam(WCHDATA_JXJCityInBattleSet,param);
//					std::set<unsigned short> cityinbattleset = *(std::set<unsigned short> *)param;
//
//					data->getParam(WCHDATA_JXJBattleCitiesSet, param);
//					std::set<unsigned short> *battlecityset = ((std::set<unsigned short> *)param);
//					data->getParam(WCHDATA_JXJMyBattleCitiesSet, param);
//					std::set<unsigned short> *my_cities = ((std::set<unsigned short> *)param);
//					for (std::set<unsigned short>::iterator itr = cityinbattleset.begin();itr!=cityinbattleset.end();itr++)
//					{
//						if(battlecityset->find(*itr) != battlecityset->end())
//							count++;
//
//						if(my_cities->find(*itr) != my_cities->end())
//							count++;
//					}
//
//					if (count != 0)
//					{
//						ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("guozhantanchu");
//						if(canvas.valid())
//						{
//							ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("wenbe"));
//
//							//std::vector<float>v_i;
//							ItemMap itemMap;
//							rcfg::ConfigScript cfg_script(&itemMap);
//
//							cfg_script.Add("Hypertext");
//							cfg_script.Push("Hypertext");
//
//							/*cfg_script.Add("Content");
//							cfg_script.Push("Content", 1);
//							v_i.clear();
//							v_i.push_back(140.0f);
//							v_i.push_back(40.0f);
//							v_i.push_back(40.0f);
//							v_i.push_back(255.0f);
//							cfg_script.Add("Color", v_i);
//							cfg_script.Add("Text", "主公：");
//							cfg_script.Pop();*/
//
//							cfg_script.Add("Content");
//							cfg_script.Push("Content", 1);
//							/*v_i.clear();
//							v_i.push_back(255.0f);
//							v_i.push_back(255.0f);
//							v_i.push_back(225.0f);
//							v_i.push_back(255.0f);
//							cfg_script.Add("Color", v_i);*/
//							std::string tmpText = "\n你所处的国家存在交战城池，是否前往帮忙？";
//							cfg_script.Add("Text", tmpText);
//							cfg_script.Pop();
//
//							cfg_script.Pop();
//							if(contentText.valid()) contentText->setHypertext(cfg_script);
//
//							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
//						}
//					}
//
//					(*isfirst)++;
//				}
//				data->excHandle(MAKEINT64(WCH_LockData,0));
//			}
//		}
//	}
//}

///////////////////////////////////////////////////////////////////////////
//
//crBigMapMoveToSelectedCityPosMethod
//
//////////////////////////////////////////////////////////////////////////
crBigMapMoveToSelectedCityPosMethod::crBigMapMoveToSelectedCityPosMethod():
	m_this(NULL),
	m_cityindex(0)
{

}
crBigMapMoveToSelectedCityPosMethod::crBigMapMoveToSelectedCityPosMethod(const crBigMapMoveToSelectedCityPosMethod& handle):
	crMethod(handle),
	m_cityindex(handle.m_cityindex),
	m_this(NULL)
{

}
void crBigMapMoveToSelectedCityPosMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crBigMapMoveToSelectedCityPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strBigMapImage = str;
		break;
	case 1:
		m_strOtherMapCanvas = str;
		break;
	case 2:
		m_strOtherMapImage = str;
		break;
	case 3:
		m_strFrame = str;
		break;
	case 4:
		m_strBigMapCanvas = str;
		break;
	case 5:
		m_battleList = str;
		break;
	case 6:
		m_cityindex = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crBigMapMoveToSelectedCityPosMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>  parentcanvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  miniMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strOtherMapCanvas);
	ref_ptr<crCanvasNode>  bigMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strBigMapCanvas);
	//ref_ptr<crCanvasNode>  bigMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strBigMapCanvas);
	if(!miniMapCanvas.valid() || !bigMapCanvas.valid() || !parentcanvas.valid()) return;
	crData *canvasData = bigMapCanvas->getDataClass();
	if(!canvasData) return;
	ref_ptr<crImageBoxWidgetNode> miniimagebox = dynamic_cast<crImageBoxWidgetNode *>(miniMapCanvas->getWidget(m_strOtherMapImage));
	ref_ptr<crWidgetNode> bigimagebox = dynamic_cast<crWidgetNode *>(bigMapCanvas->getWidget(m_strBigMapImage));
	ref_ptr<crWidgetNode>  frameWeidget = miniMapCanvas->getWidget(m_strFrame);
	if(!frameWeidget.valid()) 
	{
		canvasData->getParam(WCHDATA_JXJMiniMapFrameWidget,param);
		frameWeidget = (crWidgetNode *)param;
	}
	if(!miniimagebox.valid() || !bigimagebox.valid() || !frameWeidget.valid()) return;

	const crBoundingBox& bigmapbox = bigimagebox->getBoundBox();
	const crBoundingBox& minimapImageboxBox = miniimagebox->getBoundBox();
	crVector3 minimapCenter = minimapImageboxBox.center();
	const crBoundingBox& frameBox = frameWeidget->getBoundBox();

	ref_ptr<crTableIO>chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	crTableIO::StrVec record;
	//获取城池id
	unsigned short selectedchengchiID = 0;
	crTableWidgetNode *battleList = dynamic_cast<crTableWidgetNode *>(parentcanvas->getWidget(m_battleList));
	crData *parentcanvasData = parentcanvas->getDataClass();
	if (parentcanvasData && battleList)
	{
		if (m_cityindex == 10)
		{
			parentcanvasData->getParam(WCHDATA_JXJCityNowInBattle,param);
			selectedchengchiID = *(unsigned short *)param; 
		}
		else
		{
			parentcanvasData->getParam(WCHDATA_JXJCityListstartindexA,param);
			int startindex = *(int *)param; 
			selectedchengchiID = battleList->getData(startindex + m_cityindex);
		}
	}

	crPlayerGameData *me = crMyPlayerData::getInstance()->getPlayerGameData();
	if(me && chengchiTab.valid())
	{
		int buttonid = chengchiTab->getTitleIndex("buttonname");
		crData * playerData = me->getDataClass();
		if(playerData)
		{
			//playerData->getParam(WCHDATA_JXJChengchiID,param);
			//unsigned short mychengchiID = *(unsigned short *)param;

			if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(selectedchengchiID),record)>=0 && buttonid>=0)
			{
				crRadioWidgetNode * bt = dynamic_cast<crRadioWidgetNode *>(bigMapCanvas->getWidget(record[buttonid]));
				if(bt)
				{
					const crBoundingBox& btBox = bt->getBoundBox();
					crVector2 curOffset(-btBox.center().x(),-btBox.center().y());

					canvasData->getParam(WCHDATA_JXJCityMapFrameScale,param);
					crVector3 & framescale = *(crVector3 *)param;

					canvasData->getParam(WCHDATA_JXJCityMapOffsetMax,param);
					crVector2 maxoffset = *(crVector2 *)param;

					crVector2 scaleVec(minimapImageboxBox.xLength()/bigmapbox.xLength(),minimapImageboxBox.yLength()/bigmapbox.yLength());
					crVector3 frameOffset(-curOffset.x()*scaleVec.x(),-curOffset.y()*scaleVec.y(),0.0f);

					if(curOffset[0]<-maxoffset[0])
						curOffset[0] = -maxoffset[0];
					else if(curOffset[0]>maxoffset[0])
						curOffset[0] = maxoffset[0];
					if(curOffset[1]<-maxoffset[1])
						curOffset[1] = -maxoffset[1];
					else if(curOffset[1]>maxoffset[1])
						curOffset[1] = maxoffset[1];

					bigMapCanvas->setMatrix(crMatrix::translate(curOffset[0],curOffset[1],0.0f));
					frameWeidget->setMatrix(crMatrix::scale(framescale) * crMatrix::translate(frameOffset[0],frameOffset[1],0.0f));

					canvasData->inputParam(WCHDATA_JXJCityCenterOffset,&curOffset);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJUISetGuoZhanGuiZheMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUISetGuoZhanGuiZheMethod::crJXJUISetGuoZhanGuiZheMethod():
	m_this(NULL)
{

}
crJXJUISetGuoZhanGuiZheMethod::crJXJUISetGuoZhanGuiZheMethod(const crJXJUISetGuoZhanGuiZheMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJUISetGuoZhanGuiZheMethod::inputParam(int i, void *param)
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
void crJXJUISetGuoZhanGuiZheMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strInput = str;
		break;
	default:
		break;
	}
}
void crJXJUISetGuoZhanGuiZheMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crStaticTextWidgetNode> inputguizhe = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strInput));
		if (inputguizhe.valid())
		{
			ref_ptr<crTableIO>codtexttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			std::string text;
			std::vector<std::string> textrecord;
			//std::string text = "国战开启：国战战场需满足5人以上报名开启。\n关隘战：全天开放，只可攻打类型为关隘的城池。\n攻城战：每日下午2点至3点半，晚上7点半至9点半，可攻打任意类型城池。\n胜负判定：进攻方击破城池内正厅可获胜，守方击破进攻方全部据点可获胜。\n战场提示：战场中兵力死亡可获得经验，击杀可获得功勋奖励。";
			if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9170),textrecord)>=0)
				text = textrecord[1];
			inputguizhe->setString(text);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJSelectAttactListQieYeMethod
//
//////////////////////////////////////////////////////////////////////////
//crJXJSelectAttactListQieYeMethod::crJXJSelectAttactListQieYeMethod():
//	m_this(NULL)
//{
//
//}
//crJXJSelectAttactListQieYeMethod::crJXJSelectAttactListQieYeMethod(const crJXJSelectAttactListQieYeMethod& handle):
//	crMethod(handle),
//	m_this(NULL)
//{
//
//}
//void crJXJSelectAttactListQieYeMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crWidgetNode *)param;
//		break;
//	}
//}
//void crJXJSelectAttactListQieYeMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_qieyesw = str;
//		break;
//	default:
//		break;
//	}
//}
//void crJXJSelectAttactListQieYeMethod::operator ()(crHandle &handle)
//{
//	if (m_this)
//	{
//		ref_ptr<crCanvasNode> parentcanvas = m_this->getParentCanvas();
//		if (parentcanvas.valid() && parentcanvas->getDataClass())
//		{
//			//void *param;
//			ref_ptr<crData> data  = parentcanvas->getDataClass();
//			ref_ptr<crMultiSwitch> qieyesw = dynamic_cast<crMultiSwitch *>(parentcanvas->getChildNode(m_qieyesw));
//			if (qieyesw.valid())
//			{
//				unsigned char shili = 13;
//				int index = qieyesw->getActiveSwitchSet();
//
//				if (index == 0)
//					shili = 10;
//				else if(index == 1)
//					shili = 11;
//				else if(index == 2)
//					shili = 12;
//				else
//					shili = 13;
//
//				data->inputParam(WCHDATA_JXJselectedqieye,&shili);
//			}
//			parentcanvas->doEvent(WCH_UI_UpdateData);
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJBattleOpenCenterNoticeCreateMethod
//
/////////////////////////////////////////
crJXJBattleOpenCenterNoticeCreateMethod::crJXJBattleOpenCenterNoticeCreateMethod():m_time(0){}
crJXJBattleOpenCenterNoticeCreateMethod::crJXJBattleOpenCenterNoticeCreateMethod(const crJXJBattleOpenCenterNoticeCreateMethod& handle):
	crMethod(handle),
	m_time(handle.m_time)
{
}
void crJXJBattleOpenCenterNoticeCreateMethod::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_time = LOINT64(param64);
			//m_iswin = *(bool *)(HIINT64(param64));
		}
		else
		{
			m_time = 0;
			//m_iswin = false;
		}
		break;
	}
}

void crJXJBattleOpenCenterNoticeCreateMethod::addParam(int i, const std::string& str)
{
}

void crJXJBattleOpenCenterNoticeCreateMethod::operator()(crHandle &handle)
{
	if(m_this!=NULL)
	{
		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);

		if (m_time != 3)
		{
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content", 1);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", "三国鼎立，谁主沉浮！国家战争将在");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(140.0f);
			v_i.push_back(40.0f);
			v_i.push_back(40.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			std::string tmpText = crArgumentParser::appItoa(m_time/60);
			cfg_script.Add("Text", tmpText);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 3);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", "分钟后打响，望诸位主公整顿兵力保卫疆土！");
			cfg_script.Pop();

			cfg_script.Pop();
		}
		else
		{
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			cfg_script.Add("Content");
			cfg_script.Push("Content", 1);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			cfg_script.Add("Text", "三国鼎立，谁主沉浮！国家战争正式打响，请诸位主公尽力参战！");
			cfg_script.Pop();

			cfg_script.Pop();
		}

		ref_ptr<crStreamBuf> streamBuffer = new crStreamBuf;
		crGlobalHandle::getInstance()->getScriptStream(cfg_script,streamBuffer);

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(streamBuffer->getRemainSize());
		stream->write(streamBuffer->getPtr(),streamBuffer->getRemainSize());

		crPlayerDataEventPacket packetpk;
		crPlayerDataEventPacket::buildReplyPacket(packetpk, 0, WCH_JXJRecvTempClientNotice, stream.get());
		stream->seekBegin();
		//gsbraindata->inputParam(WCHDATA_JXJCenterNotice,stream.get());
		crStreamBuf *pkgstream = packetpk.getStreamBuf();
		int size = pkgstream->getBufSize();
		ref_ptr<crGameServerPlayerData> recvPlayerData;
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crNetManager *netManager = gameServer->getNetManager();
		if(netDataManager && netManager)
		{
			netDataManager->lockPlayerDataMap();
			crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (recvPlayerData.valid())
				{
					if (m_time != 0)
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(recvPlayerData->getPlayerID());
						pkgstream->setBufSize(size);
						gameServer->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packetpk);
					}

					if (m_time == 0)
					{
						crPlayerDataEventPacket replypacket;
						ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
						stream1->createBuf(1);
						stream1->_writeChar(1);
						crPlayerDataEventPacket::buildReplyPacket(replypacket,recvPlayerData->getPlayerID(),WCH_JXJRecvBattleOpenTiShi,stream1.get());
						netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),replypacket);
					}
				}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleOpenTiShiMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleOpenTiShiMethod::crJXJRecvBattleOpenTiShiMethod(){}
crJXJRecvBattleOpenTiShiMethod::crJXJRecvBattleOpenTiShiMethod(const crJXJRecvBattleOpenTiShiMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvBattleOpenTiShiMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvBattleOpenTiShiMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvBattleOpenTiShiMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(!playerData) return;
			crData *roledata = playerData->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;
			if (characterlevel >= 10)
			{
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("guozhantanchu");
				if(canvas.valid())
				{
					ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("wenbe"));

					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);

					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content", 1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					std::string tmpText = "\n国家战争正式开启，是否前往一展宏图？";
					cfg_script.Add("Text", tmpText);
					cfg_script.Pop();

					cfg_script.Pop();
					if(contentText.valid()) contentText->setHypertext(cfg_script);

					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvCityInBAattleMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvCityInBAattleMethod::crJXJRecvCityInBAattleMethod(){}
crJXJRecvCityInBAattleMethod::crJXJRecvCityInBAattleMethod(const crJXJRecvCityInBAattleMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCityInBAattleMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvCityInBAattleMethod::addParam(int i, const std::string& str)
{
	/*switch (i)
	{
	case 0:
	m_huodongcanvas = str;
	break;
	case 1:
	m_chengzhanbtn = str;
	break;
	case 2:
	m_panguocanvas = str;
	break;
	}*/
}

void crJXJRecvCityInBAattleMethod::operator()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		crData * data = crServerBrainHandle::getInstance()->getDataClass();

		unsigned char myShili = 0;
		unsigned char myGuanzhi = 0;
		crData *dat = m_this->getDataClass();
		if (dat && data)
		{
			dat->getParam(WCHDATA_JXJShiliID,param);
			myShili = *(unsigned char *)param;
		
			int playerid = m_this->getPlayerID();
			crJXJShili *shili;
			crData *shiliData;
			data->getParam(WCHDATA_JXJShiliWei+myShili-c_startShiliID,param);
			shili = (crJXJShili *)param;

			int chengxiangid = 0;
			int dasimaid = 0;
			int dajiangjunid = 0;
			if(shili && shili->getDataClass())
			{
				shiliData = shili->getDataClass();
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));

				shiliData->getParam(WCHDATA_JXJChengxiang,param);
				chengxiangid = *(int*)param;
				shiliData->getParam(WCHDATA_JXJDasima,param);
				dasimaid = *(int*)param;
				shiliData->getParam(WCHDATA_JXJDajiangjun,param);
				dajiangjunid = *(int*)param;

				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}

			dat->excHandle(MAKEINT64(WCH_LockData,1));
			dat->getParam(WCHDATA_JXJPlayerGuanZhi,param);
			unsigned char * guanzhi = (unsigned char *)param;
			if (playerid == chengxiangid)
				*guanzhi = 1;
			else if(playerid == dasimaid)
				*guanzhi = 2;
			else if(playerid == dajiangjunid)
				*guanzhi = 3;
			else
				*guanzhi = 0;
			myGuanzhi = *guanzhi;
			dat->excHandle(MAKEINT64(WCH_LockData,0));
		}

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
		if(gsplayerData.valid() && gsplayerData->getPlayerGameData() && data)
		{
			char _case  = 0;
			std::set<unsigned char> OnBattleshiliSet;
			data->getParam(WCHDATA_JXJOpenBattleMap,param);
			OpenBattleMap *openBattleMap = (OpenBattleMap *)param;
			for( OpenBattleMap::iterator itr = openBattleMap->begin();
				itr != openBattleMap->end();
				++itr )
			{
				OnBattleshiliSet.insert(HIINT16(LOINT32(itr->first)));
				OnBattleshiliSet.insert(LOINT16(LOINT32(itr->first)));
			}

			unsigned char myshili = 0;
			ref_ptr<crData> playerdata = gsplayerData->getPlayerGameData()->getDataClass();
			if(playerdata.valid())
			{
				playerdata->getParam(WCHDATA_JXJShiliID,param);
				myshili = *(unsigned char *)param;
			}
			
			for (std::set<unsigned char>::iterator itr=OnBattleshiliSet.begin();itr!=OnBattleshiliSet.end();itr++)
			{
				if(myshili == *itr)
					_case++;
			}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			stream->_writeChar(_case);//1
			stream->_writeUChar(myGuanzhi);
			
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJFindCityInBattle,stream.get());
			gameServer->getNetManager()->sendPacket(gsplayerData->getPlayerConnectServerAddress(),packet);
		}
	}
	else if(m_netType == GameClient_Game)
	{
		char _case = m_stream->_readChar();
		unsigned char myGuanzhi = m_stream->_readUChar();
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!playerData) return;
		crData *data = playerData->getDataClass();
		data->inputParam(WCHDATA_JXJPlayerGuanZhi,&myGuanzhi);
		if (_case != 0)
		{
			crData *roledata = playerData->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;
			if (characterlevel >= 10)
			{
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("guozhantanchu");
				if(canvas.valid())
				{
					ref_ptr<CRUI::crHypertextWidgetNode>  contentText = dynamic_cast< CRUI::crHypertextWidgetNode  *>(canvas->getWidget("wenbe"));

					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);

					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					/*cfg_script.Add("Content");
					cfg_script.Push("Content", 1);
					v_i.clear();
					v_i.push_back(140.0f);
					v_i.push_back(40.0f);
					v_i.push_back(40.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					cfg_script.Add("Text", "主公：");
					cfg_script.Pop();*/

					cfg_script.Add("Content");
					cfg_script.Push("Content", 1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color", v_i);
					std::string tmpText = "\n你所处的国家存在交战城池，是否前往帮忙？";
					cfg_script.Add("Text", tmpText);
					cfg_script.Pop();

					cfg_script.Pop();
					if(contentText.valid()) contentText->setHypertext(cfg_script);

					bool sign =  true;
					ref_ptr<crCanvasNode> shijiecanvas = crFilterRenderManager::getInstance()->findCanvas("Shijiefuben");
					if (shijiecanvas.valid() && shijiecanvas->getVisiable())
						sign = false;
					
					if(sign)
						crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUISetDefaultAttactCountryMethod
//
/////////////////////////////////////////
crJXJUISetDefaultAttactCountryMethod::crJXJUISetDefaultAttactCountryMethod():
	m_this(NULL)
{
}
crJXJUISetDefaultAttactCountryMethod::crJXJUISetDefaultAttactCountryMethod(const crJXJUISetDefaultAttactCountryMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_radiowei(handle.m_radiowei),
	m_radioshu(handle.m_radioshu),
	m_radiowu(handle.m_radiowu)
{
}
void crJXJUISetDefaultAttactCountryMethod::inputParam(int i, void *param)
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
void crJXJUISetDefaultAttactCountryMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radiowei = str;
		break;
	case 1:
		m_radioshu = str;
		break;
	case 2:
		m_radiowu = str;
		break;
	default:
		break;
	}
}
void crJXJUISetDefaultAttactCountryMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crRadioGroupWidgetNode> radiowei = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radiowei));
		ref_ptr<crRadioGroupWidgetNode> radioshu = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radioshu));
		ref_ptr<crRadioGroupWidgetNode> radiowu = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radiowu));

		crData *canvas_data = m_this->getDataClass();
		if (!canvas_data)
			return;

		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!playerData) return;
		crData *data = playerData->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshiliid =  *(unsigned char *)param;
		CRCore::ScopedLock<crData> lock(*data);
		CRCore::ScopedLock<crData> lock1(*canvas_data);
		canvas_data->getParam(WCHDATA_JXJBattleCitiesSet, param);
		std::set<unsigned short>* battlecityset = ((std::set<unsigned short> *)param);

		if (myshiliid == 10)
		{
			if (radiowei.valid())
			{
				std::set<unsigned short> shu;
				std::set<unsigned short> wu;
				std::set<unsigned short> qun;
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *mapcity = (ChengchiMap *)param;
				for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
				{
					ChengchiMap::iterator resitr = mapcity->find(*itr);
					if (resitr != mapcity->end())
					{
						if (resitr->second->getChengzhuShili()==11)
						{
							shu.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==12)
						{
							wu.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==2)
						{
							qun.insert(*itr);
						}
					}
				}
				
				if (shu.size()>=wu.size())
				{
					if (shu.size()>=qun.size())
					{
						radiowei->select(0);
					}
					else
					{
						radiowei->select(2);
					}
				}
				else
				{
					if (wu.size()>=qun.size())
					{
						radiowei->select(1);
					}
					else
					{
						radiowei->select(2);
					}
				}
			}
		}
		else if(myshiliid == 11)
		{
			if (radioshu.valid())
			{
				std::set<unsigned short> wu;
				std::set<unsigned short> wei;
				std::set<unsigned short> qun;
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *mapcity = (ChengchiMap *)param;
				for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
				{
					ChengchiMap::iterator resitr = mapcity->find(*itr);
					if (resitr != mapcity->end())
					{
						if (resitr->second->getChengzhuShili()==10)
						{
							wei.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==12)
						{
							wu.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==2)
						{
							qun.insert(*itr);
						}
					}
				}

				if (wu.size()>=wei.size())
				{
					if (wu.size()>=qun.size())
					{
						radioshu->select(0);
					}
					else
					{
						radioshu->select(2);
					}
				}
				else
				{
					if (wei.size()>=qun.size())
					{
						radioshu->select(1);
					}
					else
					{
						radioshu->select(2);
					}
				}
			}
		}
		else if(myshiliid == 12)
		{
			if (radiowu.valid())
			{
				std::set<unsigned short> wei;
				std::set<unsigned short> shu;
				std::set<unsigned short> qun;
				data->getParam(WCHDATA_JXJChengChiMap,param);
				ChengchiMap *mapcity = (ChengchiMap *)param;
				for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
				{
					ChengchiMap::iterator resitr = mapcity->find(*itr);
					if (resitr != mapcity->end())
					{
						if (resitr->second->getChengzhuShili()==10)
						{
							wei.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==11)
						{
							shu.insert(*itr);
						}
						else if(resitr->second->getChengzhuShili()==2)
						{
							qun.insert(*itr);
						}
					}
				}

				if (wei.size()>=shu.size())
				{
					if (wei.size()>=qun.size())
					{
						radiowu->select(0);
					}
					else
					{
						radiowu->select(2);
					}
				}
				else
				{
					if (shu.size()>=qun.size())
					{
						radiowu->select(1);
					}
					else
					{
						radiowu->select(2);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerRessetDailyTaskMethod
//
/////////////////////////////////////////
crJXJServerRessetDailyTaskMethod::crJXJServerRessetDailyTaskMethod()
{
}
crJXJServerRessetDailyTaskMethod::crJXJServerRessetDailyTaskMethod(const crJXJServerRessetDailyTaskMethod& handle):
	crMethod(handle)
{
}
void crJXJServerRessetDailyTaskMethod::inputParam(int i, void *param)
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
		m_this = (crRole*)param;
		break;
	}
}
void crJXJServerRessetDailyTaskMethod::addParam(int i, const std::string& str)
{
}
void crJXJServerRessetDailyTaskMethod::operator()(crHandle &handle)
{
	void *param;
	crData *myMetierData = m_this->getMetierDataClass();
	if(myMetierData)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
		int serverid = callback->getServerID();
		crDBSavePacket dbpacket;
		ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;

		myMetierData->excHandle(MAKEINT64(WCH_LockData,1));
		myMetierData->getParam(WCHDATA_GameTaskMap,param);
		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
		if(!gameTaskMap||gameTaskMap->empty())
		{
			myMetierData->excHandle(MAKEINT64(WCH_LockData,0));
			return;
		}

		crData *data = m_this->getDataClass();
		data->getParam(WCHDATA_Level,param);
		unsigned char lv = *(unsigned char *)param;

		ref_ptr<crTableIO>tasktitletab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTaskTitleTab);
		int taskidindex = tasktitletab->getTitleIndex("taskid");
		crTableIO::StrVec record;

		crGameTask *task;
		std::vector<int> taskidvec;
		for( GameTaskMap::iterator itr = gameTaskMap->begin();
			itr != gameTaskMap->end();
			++itr )
		{
			task = itr->second.get();

			int renwutype = 0;
			if(tasktitletab->queryOneRecord(taskidindex,crArgumentParser::appItoa(task->getTaskID()),record) >= 0)
				renwutype = atoi(record[0].c_str());
			if (renwutype==4)
			{
				if(task->getActivation() && !task->isComplete())
				{
					const CRCore::crVector2ub& lvrange = task->getLvRange();
					if (lv>lvrange[1])
					{
						task->resetTask();
						taskidvec.push_back(task->getTaskID());

						if(dbConductor)
						{//任务日志
							insertTaskrecord->buildUpdateSql(m_this->getPlayerID(),serverid,m_this->getIName(),task->getTaskID(),TS_Reset,task->getTaskProgress());
							crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
							dbConductor->getNetManager()->sendPacket("all",dbpacket);
						}
					}
				}
			}
		}
		myMetierData->excHandle(MAKEINT64(WCH_LockData,0));

		if (taskidvec.size() > 0)
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
			if(netConductor)
			{//server to client
				ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
				stream1->createBuf(4*taskidvec.size() + 1);
				stream1->_writeUChar(taskidvec.size());
				for (int i=0;i<taskidvec.size();i++)
				{
					stream1->_writeInt(taskidvec[i]);
				}
			
				crPlayerDataEventPacket packet1;
				crPlayerDataEventPacket::buildReplyPacket(packet1,m_this->getPlayerID(),WCH_JXJRecvResetDailyTask,stream1.get());
				ref_ptr<crGameServerPlayerData> playerData1 = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
				if(playerData1.valid())
					netConductor->getNetManager()->sendPacket(playerData1->getPlayerConnectServerAddress(),packet1);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvResetDailyTaskMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvResetDailyTaskMethod::crJXJRecvResetDailyTaskMethod(){}
crJXJRecvResetDailyTaskMethod::crJXJRecvResetDailyTaskMethod(const crJXJRecvResetDailyTaskMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvResetDailyTaskMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvResetDailyTaskMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvResetDailyTaskMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			ref_ptr<crRole> role = m_this->getMainRole();
			if (role.valid())
			{
				crData *metierData = role->getMetierDataClass();
				if (metierData)
				{
					unsigned char size = m_stream->_readUChar();
					void *param;
					metierData->excHandle(MAKEINT64(WCH_LockData,1));
					metierData->getParam(WCHDATA_GameTaskMap,param);
					GameTaskMap* gameTaskMap = (GameTaskMap* )param;
					GameTaskMap::iterator taskitr;
					int taskid;
					//bool firsttask = false;
					for( int i = 0; i<size; i++)
					{
						taskid = m_stream->_readInt();
						//if(taskid == 1)
						//	firsttask = true;
						taskitr = gameTaskMap->find(taskid);
						if(taskitr!=gameTaskMap->end())
						{
							taskitr->second->resetTask();
						}
					}
					metierData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJReBuildAttactListMethod
//
/////////////////////////////////////////
crJXJReBuildAttactListMethod::crJXJReBuildAttactListMethod():
	m_this(NULL)
{
}
crJXJReBuildAttactListMethod::crJXJReBuildAttactListMethod(const crJXJReBuildAttactListMethod& handle):
	crMethod(handle),
	m_battleList(handle.m_battleList)
{

}
void crJXJReBuildAttactListMethod::inputParam(int i, void *param)
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
void crJXJReBuildAttactListMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_battleList = str;
		break;
	case 1:
		m_qiehuanshilisw = str;
		break;
	case 2:
		m_radiowei = str;
		break;
	case 3:
		m_radioshu = str;
		break;
	case 4:
		m_radiowu = str;
		break;
	case 5:
		m_enterbtnA[0] = str;
		break;
	case 6:
		m_enterbtnA[1] = str;
		break;
	case 7:
		m_enterbtnA[2] = str;
		break;
	case 8:
		m_enterbtnA[3] = str;
		break;
	case 9:
		m_enterbtnA[4] = str;
		break;
	case 10:
		m_enterbtnA[5] = str;
		break;
	case 11:
		m_enterbtnA[6] = str;
		break;
	}
}
void crJXJReBuildAttactListMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1");
	if (canvas.valid())
	{
		void *param;
		crData *canvas_data = canvas->getDataClass();
		if(!canvas_data) return;

		crButtonWidgetNode *enterbuttonA[7] = {NULL};
		for (int i = 0; i < 7; i++)
		{
			enterbuttonA[i] = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_enterbtnA[i]));
			if(enterbuttonA[i])
			{
				enterbuttonA[i]->setEnable(false);
			}
		}

		crTableWidgetNode *battleList = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_battleList));
		if(battleList)
		{
			canvas_data->getParam(WCHDATA_JXJBattleCitiesSet, param);
			std::set<unsigned short> *battlecityset = ((std::set<unsigned short> *)param);
			canvas_data->getParam(WCHDATA_JXJCityInBattleSet, param);
			std::set<unsigned short> *cityinbattleset = ((std::set<unsigned short> *)param);

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(!playerData) return;
			crData *data = playerData->getDataClass();

			unsigned char attackcountry = 13;
			ref_ptr<crMultiSwitch> shilisw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_qiehuanshilisw));
			ref_ptr<crRadioGroupWidgetNode> radiowei = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radiowei));
			ref_ptr<crRadioGroupWidgetNode> radioshu = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radioshu));
			ref_ptr<crRadioGroupWidgetNode> radiowu = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radiowu));
			if (shilisw.valid())
			{
				data->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myshiliid =  *(unsigned char *)param;
				if (myshiliid == 10)
				{
					shilisw->setActiveSwitchSet(0);
					if (radiowei.valid())
					{
						if(radiowei->getSelect() == 0)
							attackcountry = 11;
						else if(radiowei->getSelect() == 1)
							attackcountry = 12;
						else
							attackcountry = 2;
					}
				}
				else if(myshiliid == 11)
				{
					shilisw->setActiveSwitchSet(1);
					if (radioshu.valid())
					{
						if(radioshu->getSelect() == 0)
							attackcountry = 12;
						else if(radioshu->getSelect() == 1)
							attackcountry = 10;
						else
							attackcountry = 2;
					}

				}
				else
				{
					shilisw->setActiveSwitchSet(2);
					if (radiowu.valid())
					{
						if(radiowu->getSelect() == 0)
							attackcountry = 10;
						else if(radiowu->getSelect() == 1)
							attackcountry = 11;
						else
							attackcountry = 2;
					}
				}
			}

			//重构进攻城池列表
			std::set<unsigned short> attackset;
			ref_ptr<crTableIO>tab_city = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			battleList->clearData();
			int Row = 0;
			crTableIO::StrVec temp_rec,record;
			ref_ptr<crTableIO>tab_side = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			data->getParam(WCHDATA_JXJChengChiMap,param);
			ChengchiMap *mapcity = (ChengchiMap *)param;
			for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
			{
				ChengchiMap::iterator resitr = mapcity->find(*itr);
				if (resitr != mapcity->end())
				{
					if (resitr->second->getChengzhuShili()==attackcountry && cityinbattleset->find(*itr)!=cityinbattleset->end())
					{
						if(tab_side->queryOneRecord(0,crArgumentParser::appItoa(attackcountry),temp_rec)>=0)
						{
							tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
							attackset.insert(*itr);
							battleList->addData(0,Row,record[1]);
							battleList->addData(1,Row,temp_rec[1]);
							battleList->addData(2,Row,"攻");
							battleList->addData(Row,(int)(*itr));
							Row++;
						}
					}
				}
			}
			for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
			{
				if (attackset.find(*itr) == attackset.end())
				{
					ChengchiMap::iterator resitr = mapcity->find(*itr);
					if (resitr != mapcity->end())
					{
						if (resitr->second->getChengzhuShili()==attackcountry)
						{
							if (tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record)>=0 && atoi(record[6].c_str())==0)
							{
								if(tab_side->queryOneRecord(0,crArgumentParser::appItoa(attackcountry),temp_rec)>=0)
								{
									attackset.insert(*itr);
									battleList->addData(0,Row,record[1]);
									battleList->addData(1,Row,temp_rec[1]);
									battleList->addData(2,Row,"攻");
									battleList->addData(Row,(int)(*itr));
									Row++;
								}
							}
						}
					}
				}
			}
			for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
			{
				if (attackset.find(*itr) == attackset.end())
				{
					ChengchiMap::iterator resitr = mapcity->find(*itr);
					if (resitr != mapcity->end())
					{
						if (resitr->second->getChengzhuShili()==attackcountry)
						{
							if (tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record)>=0)
							{
								if(tab_side->queryOneRecord(0,crArgumentParser::appItoa(attackcountry),temp_rec)>=0)
								{
									attackset.insert(*itr);
									battleList->addData(0,Row,record[1]);
									battleList->addData(1,Row,temp_rec[1]);
									battleList->addData(2,Row,"攻");
									battleList->addData(Row,(int)(*itr));
									Row++;
								}
							}
						}
					}
				}
			}
			for (int ii = 0; ii<Row && ii<7; ii++)
			{
				enterbuttonA[ii]->setEnable(true);
			}
		}
	}
}

///////////////////////////////////////
//
//crJXJJunZiArrivedDestinationMethod
//
///////////////////////////////////////
crJXJJunZiArrivedDestinationMethod::crJXJJunZiArrivedDestinationMethod() 
{

}
crJXJJunZiArrivedDestinationMethod::crJXJJunZiArrivedDestinationMethod(const crJXJJunZiArrivedDestinationMethod& handle) :
	crMethod(handle),
	m_countid(handle.m_countid)
{
}
void crJXJJunZiArrivedDestinationMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_type = atoi(str.c_str());
		break;
	case 1:
		m_countid = str;
		break;
	}
}
void crJXJJunZiArrivedDestinationMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJJunZiArrivedDestinationMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO>codtexttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	std::string text;
	std::vector<std::string> textrecord;
	if (m_type == 1 || m_type == 5)//资源车抵达目的地
	{
		unsigned char countryid = atoi(m_countid.c_str());
		unsigned char diguo1 = 0;
		unsigned char diguo2 = 0;
		int texid = 9181;
		if(m_countid == "10")
		{
			diguo1 = 11;
		    diguo2 = 12;
			texid = m_type == 1?9223:9226;
		}
		else if(m_countid == "11")
		{
			diguo1 = 10;
			diguo2 = 12;
			texid = m_type == 1?9224:9227;
		}
		else if(m_countid == "12")
		{
			diguo1 = 10;
			diguo2 = 11;
			texid = m_type == 1?9225:9228;
		}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(0);
		ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
		crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvJunZiArriveDesti,stream.get());
		m_this->sendRoomMessage(packet.get(),countryid);

		ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
		stream1->createBuf(9);
		stream1->_writeUChar(countryid);
		ref_ptr<crPlayerDataEventPacket> packet1 = new crPlayerDataEventPacket;
		crPlayerDataEventPacket::buildReplyPacket(*packet1,0,WCH_JXJRecvJunZiArriveDesti,stream1.get());
		m_this->sendRoomMessage(packet1.get(),diguo1);
		m_this->sendRoomMessage(packet1.get(),diguo2);

		if(codtexttab->queryOneRecord(0,crArgumentParser::appItoa(texid),textrecord)>=0)
			text = textrecord[1];
	}
	else if(m_type == 2)//资源车刷新
	{
		if(codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9171),textrecord)>=0)
			text = textrecord[1];
	}
	else if(m_type == 3)//1点半boss刷新
	{
		if(codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9172),textrecord)>=0)
			text = textrecord[1];
	}
	else if(m_type == 4)//6点半boss刷新
	{
		if(codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9173),textrecord)>=0)
			text = textrecord[1];
	}

	std::vector<float>v_i;
	ItemMap itemMap;
	int index = 1;
	rcfg::ConfigScript cfg_script(&itemMap);
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content",index++);
	cfg_script.Push("Character",16);
	cfg_script.Add("Text",text);
	cfg_script.Pop();

	cfg_script.Pop();

	ref_ptr<crStreamBuf> streamBuffer = new crStreamBuf;
	crGlobalHandle::getInstance()->getScriptStream(cfg_script,streamBuffer);

	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(streamBuffer->getRemainSize());
	stream->write(streamBuffer->getPtr(),streamBuffer->getRemainSize());

	ref_ptr<crPlayerDataEventPacket> packetpk = new crPlayerDataEventPacket;
	crPlayerDataEventPacket::buildReplyPacket(*packetpk, 0, WCH_JXJRecvTempClientNotice, stream.get());
	
	m_this->sendRoomMessage(packetpk.get());
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvJunZiArriveDestiMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvJunZiArriveDestiMethod::crJXJRecvJunZiArriveDestiMethod(){}
crJXJRecvJunZiArriveDestiMethod::crJXJRecvJunZiArriveDestiMethod(const crJXJRecvJunZiArriveDestiMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvJunZiArriveDestiMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvJunZiArriveDestiMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvJunZiArriveDestiMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			ref_ptr<crTableIO>codtexttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			std::string text;
			std::vector<std::string> textrecord;
			unsigned char type = m_stream->_readUChar();
			if (type == 0)
			{
				if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9168),textrecord)>=0)
					text = textrecord[1];
				
			}
			else if(type == 10)
			{
				if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9169),textrecord)>=0)
					text = "魏国" + textrecord[1];
			}
			else if(type == 11)
			{
				if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9169),textrecord)>=0)
					text = "蜀国" + textrecord[1];
			}
			else if(type == 12)
			{
				if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9169),textrecord)>=0)
					text = "吴国" + textrecord[1];
			}
			else if(type == 13)//击杀boss
			{
				unsigned char shiliid = m_stream->_readUChar();
				std::string bossname = m_stream->_readString();
				if(shiliid == 10)
					text = "魏国成功击杀boss";
				else if(shiliid == 11)
					text = "蜀国成功击杀boss";
				else if(shiliid == 12)
					text = "吴国成功击杀boss";

				if (codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9175),textrecord)>=0)
					text = text + bossname + textrecord[1];
			}
			ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
			record->setChatText(text);
			//record->setTextColor(COLOR_GREEN);
			record->setChatChannelName("[战场]");
			crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
		}
	}
}

///////////////////////////////////////
//
//crJXJGuoJiaZiJinJiangLiMethod
//
///////////////////////////////////////
crJXJGuoJiaZiJinJiangLiMethod::crJXJGuoJiaZiJinJiangLiMethod() :
	m_count(0)
{
}
crJXJGuoJiaZiJinJiangLiMethod::crJXJGuoJiaZiJinJiangLiMethod(const crJXJGuoJiaZiJinJiangLiMethod& handle) :
	crMethod(handle),
	m_count(handle.m_count),
	m_item(handle.m_item)
{
}
void crJXJGuoJiaZiJinJiangLiMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_count = atoi(str.c_str());
		break;
	case 1:
		m_item = str.c_str();
		break;
	case 2:
		m_points = atoi(str.c_str());
		break;
	}
}
void crJXJGuoJiaZiJinJiangLiMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dead = (crInstanceItem*)(LOINT64(param64));
			m_fire = (crInstanceItem*)HIINT64(param64);
		}
		else
		{
			m_dead = NULL;
			m_fire = NULL;
		}
		break;
	}
}
void crJXJGuoJiaZiJinJiangLiMethod::operator()(crHandle &handle)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crJXJGuoJiaZiJinJiangLiMethod"<<std::endl;
	void *param;
	unsigned char shiliid = 0;
	int playerid = 0;
	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);

	
	crRole *fireRole = dynamic_cast<crRole *>(m_fire.get());
	if (fireRole)
	{
		ref_ptr<crPlayerGameData> playgadata = fireRole->getPlayerGameData();
		if(playgadata.valid() && playgadata->getDataClass())
		{
			ref_ptr<crData> playerdata = playgadata->getDataClass();
			playerdata->getParam(WCHDATA_JXJShiliID,param);
			shiliid = *(unsigned char *)param;
			playerid = playgadata->getPlayerID();
			
			if (m_dead.valid())
			{
				std::string itemName = m_dead->getIName();
				std::string text;
				if(shiliid == 10)
					text = "恭喜魏国";
				else if(shiliid == 11)
					text = "恭喜蜀国";
				else if(shiliid == 12)
					text = "恭喜吴国";

				text = text + "成功击杀boss" + itemName;
				//中央公告
				std::vector<float>v_i;
				ItemMap itemMap;
				int index = 1;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");

				cfg_script.Add("Content");
				cfg_script.Push("Content",index++);
				cfg_script.Push("Character",16);
				cfg_script.Add("Text",text);
				cfg_script.Pop();

				cfg_script.Pop();

				ref_ptr<crStreamBuf> streamBuffer = new crStreamBuf;
				crGlobalHandle::getInstance()->getScriptStream(cfg_script,streamBuffer);

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(streamBuffer->getRemainSize());
				stream->write(streamBuffer->getPtr(),streamBuffer->getRemainSize());

				ref_ptr<crPlayerDataEventPacket> packetpk = new crPlayerDataEventPacket;
				crPlayerDataEventPacket::buildReplyPacket(*packetpk, 0, WCH_JXJRecvTempClientNotice, stream.get());
				m_this->sendRoomMessage(packetpk.get());

				//聊天通告
				ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
				stream1->createBuf(6+itemName.size());
				stream1->_writeUChar(13);
				stream1->_writeUChar(shiliid);
				stream1->_writeString(itemName);
				ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
				crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvJunZiArriveDesti,stream1.get());
				m_this->sendRoomMessage(packet.get());
			}

			crData *data = m_this->getDataClass();
			if(data)
			{
				int playerid = fireRole->getPlayerID();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJBattleStatsMap,param);
				BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;

				BattleStatsMap::iterator itr = battleStatsMap->find(playerid);
				if(itr!=battleStatsMap->end())
				{
					std::string name  = itr->second->getPlayerName();
					itr->second->addPoints((unsigned short)m_points);

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(11 + name.size());
					stream->_writeUChar(5);
					stream->_writeShort(m_points);
					stream->_writeInt(playerid);
					stream->_writeString(name);

					ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
					crPlayerDataEventPacket::buildReplyPacket(*packet, 0, WCH_JXJRecvWorldFuBenData, stream.get());
					m_this->sendRoomMessage(packet.get(), itr->second->getShiliID());
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}

			if (m_count > 0)
			{
				crVector3i vec3;
				crArgumentParser::appAtoVec(m_item,vec3);

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(22);
				stream->_writeUChar(shiliid);
				stream->_writeInt(m_count);
				stream->_writeVec3i(vec3);
				stream->_writeUChar(1);
				stream->_writeInt(playerid);

				crJXJCountryCapitalAwardPacket packet;
				crJXJCountryCapitalAwardPacket::buildRequestPacket(packet,WCH_JXJGameServerRecvCountryCapital,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}


///////////////////////////////////////
//
//crJXJGuoJiaZiJinAwardSourceCarMethod
//
///////////////////////////////////////
crJXJGuoJiaZiJinAwardSourceCarMethod::crJXJGuoJiaZiJinAwardSourceCarMethod() :
	m_shiliid(0),
	m_count(0)
{
}
crJXJGuoJiaZiJinAwardSourceCarMethod::crJXJGuoJiaZiJinAwardSourceCarMethod(const crJXJGuoJiaZiJinAwardSourceCarMethod& handle) :
	crMethod(handle),
	m_shiliid(handle.m_shiliid),
	m_count(handle.m_count),
	m_item(handle.m_item)
{
}
void crJXJGuoJiaZiJinAwardSourceCarMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_shiliid = atoi(str.c_str());
		break;
	case 1:
		m_count = atoi(str.c_str());
		break;
	case 2:
		m_item = str.c_str();
		break;
	case 3:
		m_points = atoi(str.c_str());
		break;
	}
}
void crJXJGuoJiaZiJinAwardSourceCarMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJGuoJiaZiJinAwardSourceCarMethod::operator()(crHandle &handle)
{
	if (m_this && m_this->getDataClass())
	{
		void *param;
		unsigned char shiliid = m_shiliid;
		crData *data = m_this->getDataClass();
		
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJWeiGuoPoints+m_shiliid-c_startShiliID,param);
		unsigned short * points = (unsigned short *)param;
		*points = *points + (unsigned short)m_points;

		data->excHandle(MAKEINT64(WCH_LockData,0));

		crRoom::PlayerList &playerlist = m_this->getPlayerList();
		crRoomPlayer *roomPlayer;
		std::vector<int> playeridvec;
		for( crRoom::PlayerList::iterator itr = playerlist.begin();
			itr != playerlist.end();
			++itr )
		{
			roomPlayer = itr->get();
			if(roomPlayer->getGroupID() == shiliid)
			{
				playeridvec.push_back(roomPlayer->getPlayerID());
			}
		}

		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);

		crVector3i vec3;
		crArgumentParser::appAtoVec(m_item,vec3);

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(18+playeridvec.size()*4);
		stream->_writeUChar(shiliid);
		stream->_writeInt(m_count);
		stream->_writeVec3i(vec3);
		stream->_writeUChar(playeridvec.size());
		for (int i=0;i<playeridvec.size();i++)
			stream->_writeInt(playeridvec[i]);

		crJXJCountryCapitalAwardPacket packet;
		crJXJCountryCapitalAwardPacket::buildRequestPacket(packet,WCH_JXJGameServerRecvCountryCapital,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);

		ref_ptr<crStreamBuf> gstream = new crStreamBuf;
		gstream->createBuf(4);
		gstream->_writeInt(m_points);

		ref_ptr<crPlayerDataEventPacket> gpacket = new crPlayerDataEventPacket;
		crPlayerDataEventPacket::buildReplyPacket(*gpacket,0,WCH_JXJZiYuanCheArriveAddPoint,gstream.get());
		m_this->sendRoomMessage(gpacket.get(),shiliid);
	}
}

/////////////////////////////////////////
//
//crJXJGameServerRecvCountryCapitalMethod
//
/////////////////////////////////////////
crJXJGameServerRecvCountryCapitalMethod::crJXJGameServerRecvCountryCapitalMethod(){}
crJXJGameServerRecvCountryCapitalMethod::crJXJGameServerRecvCountryCapitalMethod(const crJXJGameServerRecvCountryCapitalMethod& handle):
	crMethod(handle)
{
}
void crJXJGameServerRecvCountryCapitalMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJGameServerRecvCountryCapitalMethod::addParam(int i, const std::string& str)
{
}

void crJXJGameServerRecvCountryCapitalMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		unsigned char shiliid = m_stream->_readUChar();
		int count = m_stream->_readUInt();
		crVector3i vec3 = m_stream->_readVec3i();
		unsigned char size = m_stream->_readUChar();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> gsplayerData ;
		
		crData *serdata = m_this->getDataClass();
		crJXJShili *shili;
		crData *shiliData;
		serdata->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
		shili = (crJXJShili *)param;
		if(shili && shili->getDataClass())
		{
			shiliData = shili->getDataClass();
			shiliData->excHandle(MAKEINT64(WCH_LockData,1));
			shiliData->getParam(WCHDATA_JXJCountryCapital,param);
			int *guojiazijin = (int *)param;
			*guojiazijin += count;

			shiliData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		
		if (vec3[2] > 0)
		{
			ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
			ref_ptr<crBagItemData> itemdata;
			itemdata = new crBagItemData;
			itemdata->setItemID(vec3[0]);
			itemdata->setEquipMagic(vec3[1]);
			itemdata->setItemCount(vec3[2]);
			reward_info->getRewardItemsVec().push_back(itemdata);
			reward_info->setType(GP_RewardFubenEnd);

			for (int i=0; i<size; i++)
			{
				int playerid = m_stream->_readInt();
				reward_info->setPlayerID(playerid);
				gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if (gsplayerData.valid())
				{
					ref_ptr<crData> playerdata = gsplayerData->getPlayerGameData()->getDataClass();
					if(playerdata.valid())
					{
						playerdata->excHandle(MAKEINT64(WCH_LockData,1));
						gsplayerData->getPlayerGameData()->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward_info.get(),NULL));
						playerdata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJZiYuanCheArriveAddPointMethod
//
/////////////////////////////////////////
crJXJZiYuanCheArriveAddPointMethod::crJXJZiYuanCheArriveAddPointMethod():
	m_netType(GameClient_Game){}
crJXJZiYuanCheArriveAddPointMethod::crJXJZiYuanCheArriveAddPointMethod(const crJXJZiYuanCheArriveAddPointMethod& handle):
	crMethod(handle)
{
}
void crJXJZiYuanCheArriveAddPointMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJZiYuanCheArriveAddPointMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJZiYuanCheArriveAddPointMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			int points = m_stream->_readInt();
			ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (rankcanvas.valid() && rankcanvas->getDataClass())
			{
				void *param;
				ref_ptr<crData> canvasdata = rankcanvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				canvasdata->getParam(WCHDATA_JXJTempCountryPoints,param);
				int *countrypoints = (int *)param;
				*countrypoints += points;
				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckGuoZhanOrJianGuoMethod
//
/////////////////////////////////////////
crJXJCheckGuoZhanOrJianGuoMethod::crJXJCheckGuoZhanOrJianGuoMethod():
	m_this(NULL)
{
}
crJXJCheckGuoZhanOrJianGuoMethod::crJXJCheckGuoZhanOrJianGuoMethod(const crJXJCheckGuoZhanOrJianGuoMethod& handle):
	crMethod(handle),
	m_guozhansw(handle.m_guozhansw),
	m_guozhan(handle.m_guozhan),
	m_guanai(handle.m_guanai),
	m_sweep(handle.m_sweep),
	m_battlejr(handle.m_battlejr)
{
}
void crJXJCheckGuoZhanOrJianGuoMethod::inputParam(int i, void *param)
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
void crJXJCheckGuoZhanOrJianGuoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_guozhansw = str;
		break;
	case 1:
		m_guozhan = str;
		break;
	case 2:
		m_guanai = str;
		break;
	case 3:
		m_sweep = str;
		break;
	case 4:
		m_battlejr = str;
		break;
	default:
		break;
	}
}
void crJXJCheckGuoZhanOrJianGuoMethod::operator()(crHandle &handle)
{	
	void *param;
	ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_guozhansw));
	ref_ptr<crButtonWidgetNode> guozhan = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_guozhan));
	ref_ptr<crButtonWidgetNode> guanai = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_guanai));
	ref_ptr<crButtonWidgetNode> jianguo = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget("UI_Jianguo"));
	ref_ptr<crSequence> guozhandonghua = dynamic_cast<crSequence *>(m_this->getChildNode("Guozhandh_Widget"));
	ref_ptr<crCanvasNode> sweepCanvas = crFilterRenderManager::getInstance()->findCanvas(m_sweep);
	ref_ptr<crCanvasNode> battlejrCanvas = crFilterRenderManager::getInstance()->findCanvas(m_battlejr);
	ref_ptr<crButtonWidgetNode> shijiefuben = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget("UI_Shijie"));
	//if(shijiefuben.valid()) shijiefuben->setEnable(false);
	if (sw.valid() && guozhan.valid() && guanai.valid() && jianguo.valid())
	{
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if (playerData && playerData->getDataClass())
		{
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
			unsigned char getAward = *(unsigned char *)param;

			crData *roledata = playerData->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJVipLv, param);
			unsigned char viplv = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJTreasonBufferTimer,param);
			int bufftime = *(int *)param;
			data->getParam(WCHDATA_JXJTreasonPunishTimer,param);
			int punishtime = *(int *)param;

			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFoundCountryOpenLv,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				if (getAward == (unsigned char)-1) 
				{
					//foundCountry->setVisiable(false);
					// 已经全部领取
					sw->setActiveSwitchSet(0);
					data->getParam(WCHDATA_JXJOpenBattle, param);
					bool isbattleopened = *((bool *)param);
					if (isbattleopened)
					{
						sw->setActiveSwitchSet(0);
						if(guozhandonghua.valid())
							guozhandonghua->setVisiable(true);
					}
					else
					{
						sw->setActiveSwitchSet(1);
						if(guozhandonghua.valid())
							guozhandonghua->setVisiable(false);
					}

					bool isLose = false;
					data->getParam(WCHDATA_JXJShiliID,param);
					unsigned char shiliid = *(unsigned char *)param;

					playerData->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLose,&shiliid));
					if(isLose || bufftime>0 || punishtime>0)
					{
						guozhan->setEnable(false);
						guanai->setEnable(false);
					}
					else
					{
						guozhan->setEnable(true);
						guanai->setEnable(true);
					}
				}
				else 
				{//foundCountry->setVisiable(true);
					sw->setActiveSwitchSet(2);
					jianguo->setEnable(true);
				}
			}
			else 
			{
				//foundCountry->setVisiable(false);
				sw->setActiveSwitchSet(2);
				jianguo->setEnable(false);
			}
		}
		if (sweepCanvas.valid() && battlejrCanvas.valid())
		{
			if (sweepCanvas->getVisiable() || battlejrCanvas->getVisiable())
			{
				jianguo->setEnable(false);
				guozhan->setEnable(false);
				guanai->setEnable(false);
			}
		}
		ref_ptr<crCanvasNode> dequeCanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1_Cz");
		if (dequeCanvas.valid() && dequeCanvas->getVisiable())
		{
			jianguo->setEnable(false);
			guozhan->setEnable(false);
			guanai->setEnable(false);
			guozhandonghua->setVisiable(false);
		}
	}
}

/////////////////////////////////////////
//
//crJXJMyCountryHaveCityInBattleMethod
//
/////////////////////////////////////////
crJXJMyCountryHaveCityInBattleMethod::crJXJMyCountryHaveCityInBattleMethod():
	m_this(NULL)
{
}
crJXJMyCountryHaveCityInBattleMethod::crJXJMyCountryHaveCityInBattleMethod(const crJXJMyCountryHaveCityInBattleMethod& handle):
	crMethod(handle)
{

}
void crJXJMyCountryHaveCityInBattleMethod::inputParam(int i, void *param)
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
void crJXJMyCountryHaveCityInBattleMethod::addParam(int i, const std::string& str)
{
}
void crJXJMyCountryHaveCityInBattleMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1");
	bool inbattle = false;
	if (canvas.valid())
	{
		void *param;
		crData *canvas_data = canvas->getDataClass();
		if(!canvas_data) return;

		std::set<unsigned short> jiaozhancity;
		canvas_data->excHandle(MAKEINT64(WCH_LockData,1));
		canvas_data->getParam(WCHDATA_JXJBattleCitiesSet, param);
		std::set<unsigned short> *battlecityset = ((std::set<unsigned short> *)param);
		canvas_data->getParam(WCHDATA_JXJMyBattleCitiesSet, param);
		std::set<unsigned short> *my_cities = ((std::set<unsigned short> *)param);
		canvas_data->getParam(WCHDATA_JXJCityInBattleSet, param);
		std::set<unsigned short> *cityinbattleset = ((std::set<unsigned short> *)param);

		for (std::set<unsigned short>::iterator battitr = cityinbattleset->begin();battitr!=cityinbattleset->end();++battitr)
		{
			if (my_cities->find(*battitr) != my_cities->end())
				jiaozhancity.insert(*battitr);

			if (battlecityset->find(*battitr) != battlecityset->end())
				jiaozhancity.insert(*battitr);
		}
		if(jiaozhancity.size() != 0)
		{
			inbattle = true;
			unsigned short id = *(jiaozhancity.begin());
			canvas_data->inputParam(WCHDATA_JXJCityNowInBattle,&id);
		}

		canvas_data->excHandle(MAKEINT64(WCH_LockData,0));
	}
	handle.outputParam(0,&inbattle);
}

/////////////////////////////////////////
//
//crJXJSortAttactListMethod
//
/////////////////////////////////////////
crJXJSortAttactListMethod::crJXJSortAttactListMethod():
	m_this(NULL)
{
}
crJXJSortAttactListMethod::crJXJSortAttactListMethod(const crJXJSortAttactListMethod& handle):
	crMethod(handle),
	m_battleList(handle.m_battleList)
{

}
void crJXJSortAttactListMethod::inputParam(int i, void *param)
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
void crJXJSortAttactListMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_battleList = str;
		break;
	case 1:
		m_qiehuanshilisw = str;
		break;
	}
}
void crJXJSortAttactListMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		void *param;
		crData *canvas_data = m_this->getDataClass();
		if(!canvas_data) return;

		ref_ptr<crMultiSwitch> shilisw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_qiehuanshilisw));
		if(shilisw.valid()) shilisw->setVisiable(false);

		crTableWidgetNode *battleList = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_battleList));
		if(battleList)
		{
			ref_ptr<crTableIO>tab_city = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			ref_ptr<crTableIO>tab_side = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			crTableIO::StrVec record,temp_rec;

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(!playerData) return;

			crData *data = playerData->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJChengChiMap,param);
			ChengchiMap *map_city = (ChengchiMap *)param;

			canvas_data->excHandle(MAKEINT64(WCH_LockData,1));
			canvas_data->getParam(WCHDATA_JXJBattleCitiesSet, param);
			std::set<unsigned short> *battlecityset = ((std::set<unsigned short> *)param);
			canvas_data->getParam(WCHDATA_JXJCityInBattleSet, param);
			std::set<unsigned short> *cityinbattleset = ((std::set<unsigned short> *)param);
			
			std::set<unsigned short> cityinbattle;
			std::set<unsigned short> citywei;
			std::set<unsigned short> cityshu;
			std::set<unsigned short> citywu;
			std::set<unsigned short> cityqun;

			if (battlecityset->size() != 0)
			{
				for( std::set<unsigned short>::iterator itr = battlecityset->begin(); itr != battlecityset->end(); ++itr)
				{
					unsigned char chengzhushili = (*map_city)[*itr]->getChengzhuShili();
					if (cityinbattleset->find(*itr) != cityinbattleset->end())
					{
						cityinbattle.insert(*itr);
					}
					else if(chengzhushili == 10)
					{
						citywei.insert(*itr);
					}
					else if(chengzhushili == 11)
					{
						cityshu.insert(*itr);
					}
					else if(chengzhushili == 12)
					{
						citywu.insert(*itr);
					}
					else if(chengzhushili == 2)
					{
						cityqun.insert(*itr);
					}
				}
			}
			canvas_data->excHandle(MAKEINT64(WCH_LockData,0));
			data->excHandle(MAKEINT64(WCH_LockData,0));
			int row = 0;
			battleList->clearData();
			for( std::set<unsigned short>::iterator itr = cityinbattle.begin(); itr != cityinbattle.end(); ++itr,row++)
			{
				if(tab_side->queryOneRecord(0,crArgumentParser::appItoa((*map_city)[*itr]->getChengzhuShili()),temp_rec)>=0)
				{
					tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
					battleList->addData(0,row,record[1]);
					battleList->addData(1,row,temp_rec[1]);
					battleList->addData(2,row,"攻");
					battleList->addData(row,(int)(*itr));
				}
			}
			for( std::set<unsigned short>::iterator itr = citywei.begin(); itr != citywei.end(); ++itr,row++)
			{
				if(tab_side->queryOneRecord(0,crArgumentParser::appItoa((*map_city)[*itr]->getChengzhuShili()),temp_rec)>=0)
				{
					tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
					battleList->addData(0,row,record[1]);
					battleList->addData(1,row,temp_rec[1]);
					battleList->addData(2,row,"攻");
					battleList->addData(row,(int)(*itr));
				}
			}
			for( std::set<unsigned short>::iterator itr = cityshu.begin(); itr != cityshu.end(); ++itr,row++)
			{
				if(tab_side->queryOneRecord(0,crArgumentParser::appItoa((*map_city)[*itr]->getChengzhuShili()),temp_rec)>=0)
				{
					tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
					battleList->addData(0,row,record[1]);
					battleList->addData(1,row,temp_rec[1]);
					battleList->addData(2,row,"攻");
					battleList->addData(row,(int)(*itr));
				}
			}
			for( std::set<unsigned short>::iterator itr = citywu.begin(); itr != citywu.end(); ++itr,row++)
			{
				if(tab_side->queryOneRecord(0,crArgumentParser::appItoa((*map_city)[*itr]->getChengzhuShili()),temp_rec)>=0)
				{
					tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
					battleList->addData(0,row,record[1]);
					battleList->addData(1,row,temp_rec[1]);
					battleList->addData(2,row,"攻");
					battleList->addData(row,(int)(*itr));
				}
			}
			for( std::set<unsigned short>::iterator itr = cityqun.begin(); itr != cityqun.end(); ++itr,row++)
			{
				if(tab_side->queryOneRecord(0,crArgumentParser::appItoa((*map_city)[*itr]->getChengzhuShili()),temp_rec)>=0)
				{
					tab_city->queryOneRecord(0, crArgumentParser::appItoa((int)(*itr)), record);
					battleList->addData(0,row,record[1]);
					battleList->addData(1,row,temp_rec[1]);
					battleList->addData(2,row,"攻");
					battleList->addData(row,(int)(*itr));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCityHaveDequeUpdateMethod
//
/////////////////////////////////////////
crJXJCityHaveDequeUpdateMethod::crJXJCityHaveDequeUpdateMethod(){}
crJXJCityHaveDequeUpdateMethod::crJXJCityHaveDequeUpdateMethod(const crJXJCityHaveDequeUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJCityHaveDequeUpdateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_chengchiid = LOINT32(LOINT64(param64));
			m_type = HIINT32(LOINT64(param64));
			m_attactshili = LOINT16(LOINT32(HIINT64(param64)));
			m_defenedshili = HIINT16(LOINT32(HIINT64(param64)));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJCityHaveDequeUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJCityHaveDequeUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);

		if (netConductor)
		{
			crData *serdata = m_this->getDataClass();
			crJXJShili *shili;
			crData *shiliData;
			serdata->getParam(WCHDATA_JXJShiliWei+m_attactshili-c_startShiliID,param);
			shili = (crJXJShili *)param;
		
			if(shili && shili->getDataClass())
			{
				shiliData = shili->getDataClass();
				shiliData->excHandle(MAKEINT64(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap *peoplemap = (PeopleMap*)param;
				for (PeopleMap::iterator itr = peoplemap->begin();itr != peoplemap->end();itr++)
				{
					ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(itr->first));
					if(playerData.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(3);
						stream->_writeUShort(m_chengchiid);
						stream->_writeUChar(m_type);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,itr->first,WCH_JXJRecvBattDequeUpdata,stream.get());
						netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

					}

				}
				shiliData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvUpdataBattleDequeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvUpdataBattleDequeMethod::crJXJRecvUpdataBattleDequeMethod(){}
crJXJRecvUpdataBattleDequeMethod::crJXJRecvUpdataBattleDequeMethod(const crJXJRecvUpdataBattleDequeMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvUpdataBattleDequeMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvUpdataBattleDequeMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvUpdataBattleDequeMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Jr1");
			if (canvas.valid() && canvas->getDataClass())
			{
				ref_ptr<crData> data = canvas->getDataClass();
				unsigned short chengchiid = m_stream->_readUShort();
				unsigned char type = m_stream->_readUChar();

				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJCityHaveDeque, param);
				std::set<unsigned short> *cityhavedequeset = ((std::set<unsigned short> *)param);
				if (type == 1)
				{
					cityhavedequeset->insert(chengchiid);
				} 
				else
				{
					cityhavedequeset->erase(chengchiid);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJIntoWorldBFConfirmMethod
//进入世界战场确认框
//
//////////////////////////////////////////////////////////////////////////
crJXJIntoWorldBFConfirmMethod::crJXJIntoWorldBFConfirmMethod()
{}
crJXJIntoWorldBFConfirmMethod::crJXJIntoWorldBFConfirmMethod(const crJXJIntoWorldBFConfirmMethod& handle)
{

}
void crJXJIntoWorldBFConfirmMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJIntoWorldBFConfirmMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_drag = str;
		break;
	case 2:
		m_input = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	}
}

void crJXJIntoWorldBFConfirmMethod::operator()(crHandle &handle)
{
	int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldFubenOpenLevel).c_str());
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = player->getDataClass();
	void *param;
	data ->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	crRole *mainrole = player->getMainRole();
	crData *mainroledata = mainrole->getDataClass();
	mainroledata->getParam(WCHDATA_Level,param);
	unsigned char mylv = *(unsigned char *)param;

	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	
	std::vector<float>v_i;
	ItemMap itemMap;
	rcfg::ConfigScript cfg_script(&itemMap);
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content",1);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color",v_i);
	if(mylv<openlv)
		cfg_script.Add("Text","等级不够！");
	else
		cfg_script.Add("Text","确定要进入世界战场吗？");
	cfg_script.Pop();

	cfg_script.Pop();
	msg->setHypertext(cfg_script);
	//ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
	crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
	crData *dlgData = msgCanvas->getDataClass();
	int commandtype = CDP_Widget;
	dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
	dlgData->inputParam(WCHDATA_CommandDlgParent,mylv<openlv?NULL:m_this);
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJBattleBaseNoticeUpdataMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleBaseNoticeUpdataMethod::crJXJBattleBaseNoticeUpdataMethod():
	m_this(NULL)
{

}
crJXJBattleBaseNoticeUpdataMethod::crJXJBattleBaseNoticeUpdataMethod(const crJXJBattleBaseNoticeUpdataMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJBattleBaseNoticeUpdataMethod::inputParam(int i, void *param)
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
void crJXJBattleBaseNoticeUpdataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strInput = str;
		break;
	default:
		break;
	}
}
void crJXJBattleBaseNoticeUpdataMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		void *param;
		ref_ptr<crData> canvasdata = m_this->getDataClass();
		if(!canvasdata.valid()) return;

		ref_ptr<crHypertextWidgetNode> inputnotice = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strInput));
		if (inputnotice.valid())
		{
			int index = 1;
			std::vector<float>v_i;
			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);

			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");

			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJBattleBrocastqeque,param);
			std::list<std::string> * tonggaoqueue = (std::list<std::string> *)param;
			for (std::list<std::string>::iterator itr = tonggaoqueue->begin(); itr != tonggaoqueue->end(); itr++)
			{
				std::string text;
				if(itr == --tonggaoqueue->end())
					text = *itr;
				else
					text = *itr + "\n";

				cfg_script.Add("Content");
				cfg_script.Push("Content", index++);
				v_i.clear();
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				cfg_script.Add("Text", text);
				cfg_script.Pop();
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));

			cfg_script.Pop();
			inputnotice->clearText();
			inputnotice->setHypertext(cfg_script);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJBattleBehaveFrameMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleBehaveFrameMethod::crJXJBattleBehaveFrameMethod():
	m_this(NULL)
{

}
crJXJBattleBehaveFrameMethod::crJXJBattleBehaveFrameMethod(const crJXJBattleBehaveFrameMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJBattleBehaveFrameMethod::inputParam(int i, void *param)
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
void crJXJBattleBehaveFrameMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}
void crJXJBattleBehaveFrameMethod::operator ()(crHandle &handle)
{
	if (m_this && m_this->getDataClass())
	{
		void *param;
		ref_ptr<crData> cavasdata = m_this->getDataClass();
		cavasdata->excHandle(MAKEINT64(WCH_LockData,1));
		cavasdata->getParam(WCHDATA_JXJBattleBehaveTime,param);
		char * timecontinue = (char *)param;
		if(*timecontinue >= 4)
			crFilterRenderManager::getInstance()->showCanvas(m_this,false);
		else
			(*timecontinue)++;

		cavasdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJBattleBehaveOnShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleBehaveOnShowMethod::crJXJBattleBehaveOnShowMethod():
	m_this(NULL)
{

}
crJXJBattleBehaveOnShowMethod::crJXJBattleBehaveOnShowMethod(const crJXJBattleBehaveOnShowMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJBattleBehaveOnShowMethod::inputParam(int i, void *param)
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
void crJXJBattleBehaveOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	default:
		break;
	}
}
void crJXJBattleBehaveOnShowMethod::operator ()(crHandle &handle)
{
	if (m_this && m_this->getDataClass())
	{
		void *param;
		ref_ptr<crData> cavasdata = m_this->getDataClass();
		cavasdata->excHandle(MAKEINT64(WCH_LockData,1));
		cavasdata->getParam(WCHDATA_JXJBattleBehaveTime,param);
		char * timecontinue = (char *)param;
		*timecontinue = 0;
		cavasdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleNoticeHideMethod
//隐藏战场基础信息通告
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleNoticeHideMethod::crJXJBattleNoticeHideMethod()
{}
crJXJBattleNoticeHideMethod::crJXJBattleNoticeHideMethod(const crJXJBattleNoticeHideMethod& handle)
{

}
void crJXJBattleNoticeHideMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJBattleNoticeHideMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_kaiguan = str;
		break;
	case 1:
		m_text = str;
		break;
	}
}

void crJXJBattleNoticeHideMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>  Canvas = m_this->getParentCanvas();
		if (Canvas.valid())
		{
			ref_ptr<crMultiSwitch> shilisw = dynamic_cast<crMultiSwitch *>(Canvas->getChildNode(m_kaiguan));
			ref_ptr<crHypertextWidgetNode> inputnotice = dynamic_cast<crHypertextWidgetNode *>(Canvas->getWidget(m_text));
			if (shilisw.valid() && inputnotice.valid())
			{
				if (shilisw->getActiveSwitchSet() == 0)
				{
					shilisw->setActiveSwitchSet(1);
					inputnotice->setVisiable(false);
				} 
				else
				{
					shilisw->setActiveSwitchSet(0);
					inputnotice->setVisiable(true);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleShowNoticeCanvasMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJBattleShowNoticeCanvasMethod::crJXJBattleShowNoticeCanvasMethod()
{

}
crJXJBattleShowNoticeCanvasMethod::crJXJBattleShowNoticeCanvasMethod(const crJXJBattleShowNoticeCanvasMethod &handle)
{

}
void crJXJBattleShowNoticeCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJBattleShowNoticeCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_showCanvas = str;
		break;
	}
}
void crJXJBattleShowNoticeCanvasMethod::operator ()(crHandle &handle)
{
	CRCore::ref_ptr<crCanvasNode>  noticeCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	CRCore::ref_ptr<crCanvasNode>  showCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_showCanvas);
	if(showCanvas.valid() && noticeCanvas.valid())
	{
		const crBoundingBox& canvas_box = noticeCanvas->getBoundBox();
		const crBoundingBox& bbox = showCanvas->getBoundBox();
		//noticeCanvas->setMatrix(crMatrix::translate(bbox.xMax()-canvas_box.xLength()*0.5f,bbox.center().y()+bbox.yLength()*0.5+canvas_box.yLength()*0.5f-bbox.yLength()*0.34,0.0f));
		noticeCanvas->setMatrix(crMatrix::translate(bbox.xMax()-canvas_box.xLength()*0.5f,bbox.yMax()+canvas_box.yLength()*0.5f-0.056656f*crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor(),0.0f));
		//noticeCanvas->setMatrix(crMatrix::translate(bbox.xMax()-canvas_box.xLength()*0.5f,bbox.yMin()-canvas_box.yLength()*0.5f,0.0f));
		//crFilterRenderManager::getInstance()->showCanvas(noticeCanvas.get(), true);
		//noticeCanvas->setVisiable(false);
		//CRCore::notify(CRCore::ALWAYS)<<"JXJBattleShowNoticeCanvas bbox.yLength()= "<<0.0727f*crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor()<<std::endl;
	}
	//crBrain::getInstance()->doEvent(WCH_JXJUIUpdateChatMessageList);
}

/////////////////////////////////////////
//
//crJXJCountryOfficialLeaveMethod
//
/////////////////////////////////////////
crJXJCountryOfficialLeaveMethod::crJXJCountryOfficialLeaveMethod():
	m_playerid(0){}
crJXJCountryOfficialLeaveMethod::crJXJCountryOfficialLeaveMethod(const crJXJCountryOfficialLeaveMethod& handle):
	crMethod(handle),
	m_playerid(handle.m_playerid)
{
}
void crJXJCountryOfficialLeaveMethod::addParam(int i, const std::string& str)
{
}
void crJXJCountryOfficialLeaveMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerid = LOINT64(param64);
			m_playerGameData = (crPlayerGameData *)HIINT64(param64);
		}
		break;
	}
}
void crJXJCountryOfficialLeaveMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		if(m_playerGameData.valid() && m_playerGameData->getDataClass())
		{
			unsigned char roomType = 0;
			crData *roomData = m_this->getDataClass();
			if(roomData)
			{
				roomData->getParam(WCHDATA_JXJRoomType,param);
				roomType = *(unsigned char *)param;
			}
			if (roomType == 1)
			{
				crData *gameData = m_playerGameData->getDataClass();
				gameData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				gameData->getParam(WCHDATA_JXJPlayerGuanZhi,param);
				unsigned char guanzhi = *(unsigned char *)param;
				std::string playerName = m_playerGameData->getCharacterName();
				if (guanzhi ==1 || guanzhi == 2 || guanzhi == 3)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(7+playerName.size());
					stream->_writeUChar(3);
					stream->_writeUChar(shiliid);
					stream->_writeUChar(guanzhi);
					stream->_writeString(playerName);

					ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
					crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvBattleBrocast,stream.get());
					m_this->sendRoomMessage(packet.get());
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCountryOfficialEnterMethod
//
/////////////////////////////////////////
crJXJCountryOfficialEnterMethod::crJXJCountryOfficialEnterMethod()
{}
crJXJCountryOfficialEnterMethod::crJXJCountryOfficialEnterMethod(const crJXJCountryOfficialEnterMethod& handle):
	crMethod(handle),
	m_playerData(handle.m_playerData)
{
}
void crJXJCountryOfficialEnterMethod::addParam(int i, const std::string& str)
{
}
void crJXJCountryOfficialEnterMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerData = (crSceneServerPlayerData *)LOINT64(param64);
		}
		break;
	}
}
void crJXJCountryOfficialEnterMethod::operator()(crHandle &handle)
{
	if(m_this && m_playerData.valid())
	{
		void *param;
		if(m_playerData->getPlayerGameData() && m_playerData->getPlayerGameData()->getDataClass())
		{
			unsigned char roomType = 0;
			crData *roomData = m_this->getDataClass();
			if(roomData)
			{
				roomData->getParam(WCHDATA_JXJRoomType,param);
				roomType = *(unsigned char *)param;
			}
			if (roomType == 1)
			{
				crPlayerGameData *playerGameData = m_playerData->getPlayerGameData();
				crData *gameData = playerGameData->getDataClass();
				gameData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char shiliid = *(unsigned char *)param;
				gameData->getParam(WCHDATA_JXJPlayerGuanZhi,param);
				unsigned char guanzhi = *(unsigned char *)param;
				std::string playerName = playerGameData->getCharacterName();
				if (guanzhi ==1 || guanzhi == 2 || guanzhi == 3)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(7+playerName.size());
					stream->_writeUChar(2);
					stream->_writeUChar(shiliid);
					stream->_writeUChar(guanzhi);
					stream->_writeString(playerName);

					ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
					crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvBattleBrocast,stream.get());
					m_this->sendRoomMessage(packet.get());
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJBattleNoticeMouseMoveMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleNoticeMouseMoveMethod::crJXJBattleNoticeMouseMoveMethod():
	m_this(NULL)
{

}
crJXJBattleNoticeMouseMoveMethod::crJXJBattleNoticeMouseMoveMethod(const crJXJBattleNoticeMouseMoveMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJBattleNoticeMouseMoveMethod::inputParam(int i, void *param)
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
void crJXJBattleNoticeMouseMoveMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_hide = str;
		break;
	case 2:
		m_show = str;
		break;
	case 3:
		m_type = str;
		break;
	default:
		break;
	}
}
void crJXJBattleNoticeMouseMoveMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		//ref_ptr<crHypertextWidgetNode> inputnotice = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_input));
		ref_ptr<crButtonWidgetNode> hidebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_hide));
		ref_ptr<crButtonWidgetNode> showbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_show));
		if (showbtn.valid() && hidebtn.valid())
		{
			if (m_type == "111")
			{
				showbtn->setVisiable(true);
				hidebtn->setVisiable(true);
			}
			else if(m_type == "000")
			{
				showbtn->setVisiable(false);
				hidebtn->setVisiable(false);
			}
		}
		
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJNewClientRecvDamageUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJNewClientRecvDamageUIUpdateMethod::crJXJNewClientRecvDamageUIUpdateMethod():m_this(NULL),m_offset(0.0f),m_duration(0.0f),
	m_speed(0.0f),m_maxoffset(0.0f),m_deltax(0.0f),m_deltay(0.0f),m_count(0)
{

}
crJXJNewClientRecvDamageUIUpdateMethod::crJXJNewClientRecvDamageUIUpdateMethod(const crJXJNewClientRecvDamageUIUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_offset(handle.m_offset),
	m_duration(handle.m_duration),
	m_speed(handle.m_speed),
	m_maxoffset(handle.m_maxoffset),
	m_deltax(handle.m_deltax),
	m_deltay(handle.m_deltay),
	m_count(handle.m_count)
{

}
void crJXJNewClientRecvDamageUIUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJNewClientRecvDamageUIUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_maxoffset = atof(str.c_str());
		break;
	case 1:
		m_speed = atof(str.c_str());
		break;
	}
}
void crJXJNewClientRecvDamageUIUpdateMethod::operator ()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		void *param;
		crVector3 window,pos;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			ref_ptr<CRUtil::crSceneView>sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			crData *data = m_this->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJRecvDamageItem,param);
			crInstanceItem *pItem = (crInstanceItem *)param;
			//data->getParam(WCHDATA_JXJDemageNodeIsUsing,param);
			//bool *isUsing = (bool *)param;
			if(m_offset<m_maxoffset && m_this->getVisiable())
			{
				if(pItem)
				{
					pos = pItem->getPosition();
					crData *itemdata = pItem->getDataClass();
					itemdata->getParam(WCHDATA_EyeHeight,param);
					short eyeheight = *(short *)param;
					pos[2]+=eyeheight * crGlobalHandle::gData()->gUnitScale();
					if(crBrain::getInstance()->getCameraFrustum().contains(pos))
					{
						crMatrix matrix = m_this->getMatrix();
						crVector3f vec3 = matrix.getScale();
						if (m_offset <= 0)
						{
							m_deltax = vec3[0]/2;
							m_deltay = vec3[1]/2;
							vec3[0] = vec3[0]  + vec3[0]/2;
							vec3[1] = vec3[1]  + vec3[1]/2;
							//vec3[2] = vec3[2]  + vec3[2]/3;
							m_this->setMatrix(crMatrix::scale(vec3));
						}
						else
						{
							if (m_count < 3)
							{
								vec3[0] = vec3[0]  - m_deltax/3;
								vec3[1] = vec3[1]  - m_deltay/3;
								m_this->setMatrix(crMatrix::scale(vec3));
								m_count++;
							}
						}
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						m_offset += m_speed*(*m_dt);
						pos[1]+= m_offset;
						m_this->setPosition(pos);
						
					}
					else
					{
						m_deltax = 0.0f;
						m_deltay = 0.0f;
						m_count = 0;
						m_offset = 0.0f;
						m_this->setVisiable(false);
						//*isUsing = false;
						//m_this->setEnable(false);
					}
				}
				else
				{
					m_deltax = 0.0f;
					m_deltay = 0.0f;
					m_count = 0;
					m_offset = 0.0f;
					m_this->setVisiable(false);
					//*isUsing = false;
					//m_this->setEnable(false);
				}
			}
			else
			{
				m_deltax = 0.0f;
				m_deltay = 0.0f;
				m_count = 0;
				m_offset = 0.0f;
				m_this->setVisiable(false);
				//*isUsing = false;
				//m_this->setEnable(false);
			}

			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJNewClientRecvDamageUIUpdate2Method
//
//////////////////////////////////////////////////////////////////////////
crJXJNewClientRecvDamageUIUpdate2Method::crJXJNewClientRecvDamageUIUpdate2Method():m_this(NULL),m_offset(0.0f),m_duration(0.0f),
	m_speed(0.0f),m_maxoffset(0.0f),m_deltax(0.0f),m_deltay(0.0f),m_count(0)
{

}
crJXJNewClientRecvDamageUIUpdate2Method::crJXJNewClientRecvDamageUIUpdate2Method(const crJXJNewClientRecvDamageUIUpdate2Method& handle):
	crMethod(handle),
	m_this(NULL),
	m_offset(handle.m_offset),
	m_duration(handle.m_duration),
	m_speed(handle.m_speed),
	m_maxoffset(handle.m_maxoffset),
	m_deltax(handle.m_deltax),
	m_deltay(handle.m_deltay),
	m_count(handle.m_count)
{

}
void crJXJNewClientRecvDamageUIUpdate2Method::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			int lparam = LOINT64(param64);
			m_dt = (float*)lparam;
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJNewClientRecvDamageUIUpdate2Method::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_maxoffset = atof(str.c_str());
		break;
	case 1:
		m_speed = atof(str.c_str());
		break;
	}
}
void crJXJNewClientRecvDamageUIUpdate2Method::operator ()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		void *param;
		crVector3 window,pos;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			ref_ptr<CRUtil::crSceneView>sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			crData *data = m_this->getDataClass();
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJRecvDamageItem,param);
			crInstanceItem *pItem = (crInstanceItem *)param;
			//data->getParam(WCHDATA_JXJDemageNodeIsUsing,param);
			//bool *isUsing = (bool *)param;
			if(m_offset<m_maxoffset && m_this->getVisiable())
			{
				if(pItem)
				{
					pos = pItem->getPosition();
					crData *itemdata = pItem->getDataClass();
					itemdata->getParam(WCHDATA_EyeHeight,param);
					short eyeheight = *(short *)param;
					pos[2]+=eyeheight * crGlobalHandle::gData()->gUnitScale();
					if(crBrain::getInstance()->getCameraFrustum().contains(pos))
					{
						crMatrix matrix = m_this->getMatrix();
						crVector3f vec3 = matrix.getScale();
						if (m_offset <= 0)
						{
							m_deltax = vec3[0]/3;
							m_deltay = vec3[1]/3;
							vec3[0] = vec3[0]  - vec3[0]/3;
							vec3[1] = vec3[1]  - vec3[1]/3;
							//vec3[2] = vec3[2]  + vec3[2]/3;
							m_this->setMatrix(crMatrix::scale(vec3));
						}
						else
						{
							if (m_count < 3)
							{
								vec3[0] = vec3[0] + m_deltax/3;
								vec3[1] = vec3[1] + m_deltay/3;
								m_this->setMatrix(crMatrix::scale(vec3));
								pos[1]+= m_offset;
								m_count++;
							}
						}
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						m_offset += m_speed*(*m_dt);
						//pos[1]+= m_offset;
						m_this->setPosition(pos);
						
					}
					else
					{
						m_deltax = 0.0f;
						m_deltay = 0.0f;
						m_count = 0;
						m_offset = 0.0f;
						m_this->setVisiable(false);
						//*isUsing = false;
						//m_this->setEnable(false);
					}
				}
				else
				{
					m_deltax = 0.0f;
					m_deltay = 0.0f;
					m_count = 0;
					m_offset = 0.0f;
					m_this->setVisiable(false);
					//*isUsing = false;
					//m_this->setEnable(false);
				}
			}
			else
			{
				m_deltax = 0.0f;
				m_deltay = 0.0f;
				m_count = 0;
				m_offset = 0.0f;
				m_this->setVisiable(false);
				//*isUsing = false;
				//m_this->setEnable(false);
			}

			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJShowZhenCanvasMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJShowZhenCanvasMethod::crJXJShowZhenCanvasMethod()
{

}
crJXJShowZhenCanvasMethod::crJXJShowZhenCanvasMethod(const crJXJShowZhenCanvasMethod &handle)
{

}
void crJXJShowZhenCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJShowZhenCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_noticeCanvas = str;
		break;
	case 1:
		m_showCanvas = str;
		break;
	}
}
void crJXJShowZhenCanvasMethod::operator ()(crHandle &handle)
{

	CRCore::ref_ptr<crCanvasNode>  noticeCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	CRCore::ref_ptr<crCanvasNode>  showCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_showCanvas);
	if(noticeCanvas.valid())
	{
		const crBoundingBox& canvas_box = noticeCanvas->getBoundBox();
		//const crBoundingBox& bbox = showCanvas->getBoundBox();
		noticeCanvas->setMatrix(crMatrix::translate(canvas_box.center().x(),canvas_box.center().y()+0.1,canvas_box.center().z()));

		//crFilterRenderManager::getInstance()->showCanvas(noticeCanvas.get(), true);
		//noticeCanvas->setVisiable(false);
	}
	//crBrain::getInstance()->doEvent(WCH_JXJUIUpdateChatMessageList);
}


/////////////////////////////////////////
//
//crJXJAddSightRangeMethod
//
/////////////////////////////////////////
crJXJAddSightRangeMethod::crJXJAddSightRangeMethod():m_extra(0.0)
{}
crJXJAddSightRangeMethod::crJXJAddSightRangeMethod(const crJXJAddSightRangeMethod& handle):
	crMethod(handle),
	m_playerData(handle.m_playerData),
	m_extra(handle.m_extra)
{
}
void crJXJAddSightRangeMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_extra = atof(str.c_str());
		break;
	}
}
void crJXJAddSightRangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerData = (crSceneServerPlayerData *)LOINT64(param64);
		}
		break;
	}
}
void crJXJAddSightRangeMethod::operator()(crHandle &handle)
{
	if(m_playerData.valid())
	{
		crRole *player;
		crData *itemData;
		void *param;
		short *sightRange;
		short addrange = m_extra / crGlobalHandle::gData()->gUnitScale();
		crSceneServerPlayerData::RoleMap &roleMap = m_playerData->getRoleMap();
		for( crSceneServerPlayerData::RoleMap::iterator ritr = roleMap.begin();
			ritr != roleMap.end();
			++ritr )
		{
			player = ritr->second.get();
			itemData = player->getDataClass();
			if(!itemData) continue;
			itemData->excHandle(MAKEINT64(WCH_LockData,1));
			itemData->getParam(WCHDATA_SightRange,param);
			sightRange = (short *)param;
			*sightRange += addrange;
			itemData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientAddSightRangeMethod
//
/////////////////////////////////////////
crJXJClientAddSightRangeMethod::crJXJClientAddSightRangeMethod():m_extra(0.0)
{}
crJXJClientAddSightRangeMethod::crJXJClientAddSightRangeMethod(const crJXJClientAddSightRangeMethod& handle):
	crMethod(handle),
	m_extra(handle.m_extra)
{
}
void crJXJClientAddSightRangeMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_extra = atof(str.c_str());
		break;
	}
}
void crJXJClientAddSightRangeMethod::inputParam(int i, void *param)
{
}
void crJXJClientAddSightRangeMethod::operator()(crHandle &handle)
{
	crRole *player;
	crData *itemData;
	void *param;
	short *sightRange;
	short addrange = m_extra / crGlobalHandle::gData()->gUnitScale();
	crMyPlayerData::getInstance()->lockMyRoleNpcMap();
	crMyPlayerData::MyRoleNpcMap &myRoles = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	for( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoles.begin();
		itr != myRoles.end();
		++itr )
	{
		player = itr->second.first.get();
		itemData = player->getDataClass();
		if(!itemData) continue;
		itemData->excHandle(MAKEINT64(WCH_LockData,1));
		itemData->getParam(WCHDATA_SightRange,param);
		sightRange = (short *)param;
		*sightRange += addrange;
		itemData->excHandle(MAKEINT64(WCH_LockData,0));
	}
	crMyPlayerData::getInstance()->unlockMyRoleNpcMap();
}

/////////////////////////////////////////
//
//crJXJRecvOpenTimersMethod
//
/////////////////////////////////////////
crJXJRecvOpenTimersMethod::crJXJRecvOpenTimersMethod():
	m_netType(GameClient_Game){}
crJXJRecvOpenTimersMethod::crJXJRecvOpenTimersMethod(const crJXJRecvOpenTimersMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvOpenTimersMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvOpenTimersMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvOpenTimersMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			bool issend = false;
			int nowbuildingbar = 0;
			int nowrecruitbar = 0;
			char type = m_stream->_readChar();
			void *param;
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int maxbuildingcount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuildingTimeBarMaxCount,viplv).c_str()));
			int maxrecruitcount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitTimeBarMaxCount,viplv).c_str()));
			
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJBuildingTimes,param);
			crVector4i *buildingTimes = (crVector4i *)param;
			data->getParam(WCHDATA_JXJRecruitTimes,param);
			crVector4i *recruitTimes = (crVector4i *)param;

			for(int i = 0; i < 4; i++)
			{
				if((*buildingTimes)[i] >= 0)
					nowbuildingbar++;
				if((*recruitTimes)[i] >= 0)
					nowrecruitbar++;
			}

			if (type == 0)
			{
				for (int i=nowbuildingbar;i<maxbuildingcount;i++)
				{
					if((*buildingTimes)[i] >= 0)
						continue;
					else
					{
						(*buildingTimes)[i] = 0;
						issend = true;
					}
				}
			} 
			else
			{
				for (int i=nowrecruitbar;i<maxrecruitcount;i++)
				{
					if((*recruitTimes)[i] >= 0)
						continue;
					else
					{
						(*recruitTimes)[i] = 0;
						issend = true;
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			//返回信息
			if (issend)
			{
				int playerid = m_this->getPlayerID();
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if(playerData.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(9);
					stream->_writeChar(type);
					if(type == 0)
					{
						stream->_writeInt(nowbuildingbar);
						stream->_writeInt(maxbuildingcount);
					}
					else
					{
						stream->_writeInt(nowrecruitbar);
						stream->_writeInt(maxrecruitcount);
					}
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvOpenTimer,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
			
		}
		else if(m_netType == GameClient_Game)
		{
			char type = m_stream->_readChar();
			void *param;
			crData *playerData = m_this->getDataClass();
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJBuildingTimes,param);
			crVector4i *buildingTimes = (crVector4i *)param;
			playerData->getParam(WCHDATA_JXJRecruitTimes,param);
			crVector4i *recruitTimes = (crVector4i *)param;
			if (type == 0)
			{
				int nowbuildingbar = m_stream->_readInt();
				int maxbuildingcount = m_stream->_readInt();

				for (int i=nowbuildingbar;i<maxbuildingcount;i++)
				{
					if((*buildingTimes)[i] >= 0)
						continue;
					else
						(*buildingTimes)[i] = 0;
				}
			} 
			else
			{
				int nowrecruitbar = m_stream->_readInt();
				int maxrecruitcount = m_stream->_readInt();

				for (int i=nowrecruitbar;i<maxrecruitcount;i++)
				{
					if((*recruitTimes)[i] >= 0)
						continue;
					else
						(*recruitTimes)[i] = 0;
				}
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
			
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crPunishCanvasTipsShowMethod
//
//////////////////////////////////////////////////////////////////////////
crPunishCanvasTipsShowMethod::crPunishCanvasTipsShowMethod():
	m_this(NULL)
	
{

}
crPunishCanvasTipsShowMethod::crPunishCanvasTipsShowMethod(const crPunishCanvasTipsShowMethod& handle):
	crMethod(handle)
	
{

}
void crPunishCanvasTipsShowMethod::inputParam(int i, void *param)
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
void crPunishCanvasTipsShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	}
}
void crPunishCanvasTipsShowMethod::operator ()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
		if(!tips.valid())
			return;

		ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast< crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));

		std::vector<float>v_i;
		ItemMap itemMap;
		rcfg::ConfigScript cfg_script(&itemMap);

		cfg_script.Add("Hypertext");
		cfg_script.Push("Hypertext");

		cfg_script.Add("Content");
		cfg_script.Push("Content", 1);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("Text", "     资源产量减半，无法进入战场");
		cfg_script.Pop();

		cfg_script.Pop();
		//cfg_script.Write("tipstest.cfg");
		if(tipsText.valid()) tipsText->setHypertext(cfg_script);

		const crBoundingBox &btnbox = m_this->getBoundBox();
		const crBoundingBox &tipsbox = tips->getBoundBox();
		//const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		float posx = btnbox.m_min[0] + btnbox.xLength()*0.5;
		float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
		////////////
		crVector2 mouse(crVector2(posx,posy));
		tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
		crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);

	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvWorldFuBenDataMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvWorldFuBenDataMethod::crJXJRecvWorldFuBenDataMethod(){}
crJXJRecvWorldFuBenDataMethod::crJXJRecvWorldFuBenDataMethod(const crJXJRecvWorldFuBenDataMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvWorldFuBenDataMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvWorldFuBenDataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvWorldFuBenDataMethod::operator ()(crHandle &handle){
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			int myid = crMyPlayerData::getInstance()->getPlayerGameData()->getPlayerID();
			ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (rankcanvas.valid() && rankcanvas->getDataClass())
			{
				void *param;
				ref_ptr<crData> canvasdata = rankcanvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				canvasdata->getParam(WCHDATA_JXJMyWorldInfo,param);
				int *myinfo = (int *)param;
				canvasdata->getParam(WCHDATA_JXJTempCountryPoints,param);
				int *countrypoints = (int *)param;
				canvasdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
				unsigned char tempaction = *(unsigned char *)param;

				unsigned char _case = m_stream->_readUChar();
				PlayerData data;
				if (_case == 1)  //玩家损失兵量更新伤亡排行榜
				{
					
					/*short count = m_stream->_readShort();
					int playerid = m_stream->_readInt();
					std::string name = m_stream->_readString();
					data.playerid = playerid;
					data.name = name;
					canvasdata->getParam(WCHDATA_JXJDeadRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *deadrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;

					std::multimap<int,PlayerData,std::greater<int>>::iterator itr =deadrankmap->begin();
					for (;itr != deadrankmap->end(); itr++)
					{
					if(itr->second.playerid == playerid)
					break;
					}
					if(itr != deadrankmap->end())
					{
					int first  = itr->first;
					first += (int)count;
					deadrankmap->erase(itr);
					deadrankmap->insert(std::make_pair(first,data));
					}
					else
					deadrankmap->insert(std::make_pair((int)count,data));

					if (myid == playerid)
					{
					myinfo[4] += (int)count;
					}*/
				}
				else if(_case == 2) //玩家杀死玩家部队更新积分排行榜和杀敌排行榜
				{
					unsigned char budui = m_stream->_readUChar();
					short count = m_stream->_readShort();
					int playerid = m_stream->_readInt();
					std::string name = m_stream->_readString();
					data.playerid = playerid;
					data.name = name;

					/*canvasdata->getParam(WCHDATA_JXJKillRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *killrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;*/
					canvasdata->getParam(WCHDATA_JXJPointsRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *pointsrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;

					/*std::multimap<int,PlayerData,std::greater<int>>::iterator itr = killrankmap->begin();
					for (;itr != killrankmap->end(); itr++)
					{
					if(itr->second.playerid == playerid)
					break;
					}
					if(itr != killrankmap->end())
					{
					int first  = itr->first;
					first += (int)count;
					killrankmap->erase(itr);
					killrankmap->insert(std::make_pair(first,data));
					}
					else
					killrankmap->insert(std::make_pair((int)count,data));*/

					//积分
					std::multimap<int,PlayerData,std::greater<int>>::iterator iter = pointsrankmap->begin();
					for (;iter != pointsrankmap->end(); iter++)
					{
						if(iter->second.playerid == playerid)
							break;
					}
					if(iter != pointsrankmap->end())
					{
						int first  = iter->first;
						first += (int)(budui*5);
						pointsrankmap->erase(iter);
						pointsrankmap->insert(std::make_pair(first,data));
					}
					else
						pointsrankmap->insert(std::make_pair((int)(budui*5),data));

					if (myid == playerid)
					{
						myinfo[3] += (int)count;
						myinfo[5] += (int)(budui*5);
					}
					(*countrypoints) += (int)(budui*5);
				}
				else if(_case == 3) //杀死玩家兵量更行杀敌排行榜
				{
					/*short count = m_stream->_readShort();
					int playerid = m_stream->_readInt();
					std::string name = m_stream->_readString();
					data.playerid = playerid;
					data.name = name;

					canvasdata->getParam(WCHDATA_JXJKillRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *killrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;

					std::multimap<int,PlayerData,std::greater<int>>::iterator itr = killrankmap->begin();
					for (;itr != killrankmap->end(); itr++)
					{
					if(itr->second.playerid == playerid)
					break;
					}
					if(itr != killrankmap->end())
					{
					int first  = itr->first;
					first += (int)count;
					killrankmap->erase(itr);
					killrankmap->insert(std::make_pair(first,data));
					}
					else
					killrankmap->insert(std::make_pair((int)count,data));

					if (myid == playerid)
					{
					myinfo[3] += (int)count;
					}*/
				}
				else if(_case == 4) //杀死小怪更新积分排行榜
				{
					unsigned char budui = m_stream->_readUChar();
					int playerid = m_stream->_readInt();
					std::string name = m_stream->_readString();
					data.playerid = playerid;
					data.name = name;

					canvasdata->getParam(WCHDATA_JXJPointsRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *pointsrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;

					std::multimap<int,PlayerData,std::greater<int>>::iterator iter = pointsrankmap->begin();
					for (;iter != pointsrankmap->end(); iter++)
					{
						if(iter->second.playerid == playerid)
							break;
					}
					if(iter != pointsrankmap->end())
					{
						int first  = iter->first;
						first += (int)(budui*10);
						pointsrankmap->erase(iter);
						pointsrankmap->insert(std::make_pair(first,data));
					}
					else
						pointsrankmap->insert(std::make_pair((int)(budui*10),data));

					if (myid == playerid)
					{
						myinfo[5] += (int)(budui*10);
					}
					(*countrypoints) += (int)(budui*10);
				}
				else if(_case == 5) //击杀boss更新积分排行榜
				{
					unsigned short points = m_stream->_readUShort();
					int playerid = m_stream->_readInt();
					std::string name = m_stream->_readString();
					data.playerid = playerid;
					data.name = name;

					canvasdata->getParam(WCHDATA_JXJPointsRank,param);
					std::multimap<int,PlayerData,std::greater<int>> *pointsrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;

					std::multimap<int,PlayerData,std::greater<int>>::iterator iter = pointsrankmap->begin();
					for (;iter != pointsrankmap->end(); iter++)
					{
						if(iter->second.playerid == playerid)
							break;
					}
					if(iter != pointsrankmap->end())
					{
						int first  = iter->first;
						first += (int)points;
						pointsrankmap->erase(iter);
						pointsrankmap->insert(std::make_pair(first,data));
					}
					else
						pointsrankmap->insert(std::make_pair((int)points,data));

					if (myid == playerid)
					{
						myinfo[5] += (int)points;
					}
					(*countrypoints) += (int)(points);
				}
				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));

				rankcanvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

///////////////////////////////////////
//
//crJXJWorldHuoDongStartOrEndMethod
//
///////////////////////////////////////
crJXJWorldHuoDongStartOrEndMethod::crJXJWorldHuoDongStartOrEndMethod() :
	m_startorend(2),
	m_exercisetype(3)
{
}
crJXJWorldHuoDongStartOrEndMethod::crJXJWorldHuoDongStartOrEndMethod(const crJXJWorldHuoDongStartOrEndMethod& handle) :
	crMethod(handle),
	m_startorend(handle.m_startorend),
	m_exercisetype(handle.m_exercisetype)
{
}
void crJXJWorldHuoDongStartOrEndMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_startorend = atoi(str.c_str());
		break;
	case 1:
		m_exercisetype = atoi(str.c_str());
		break;
	}
}
void crJXJWorldHuoDongStartOrEndMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJWorldHuoDongStartOrEndMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crData *data = m_this->getDataClass();
		if (data)
		{
			void * param;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJBattleStatsMap,param);
			BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;
			data->getParam(WCHDATA_JXJWorldPraisedIdMap,param);
			std::map<int,std::set<int>> *praisedidMap = (std::map<int,std::set<int>> *)param;
			data->getParam(WCHDATA_JXJWeiGuoPoints,param);
			unsigned short *pointsshiliwei = (unsigned short *)param;
			data->getParam(WCHDATA_JXJShuGuoPoints,param);
			unsigned short *pointsshilishu = (unsigned short *)param;
			data->getParam(WCHDATA_JXJWuGuoPoints,param);
			unsigned short *pointshiliwu = (unsigned short *)param;
			data->getParam(WCHDATA_JXJWorldTemHuoDong,param);
			unsigned char *temaction = (unsigned char *)param;
			
			if (m_startorend == 0)
			{
				*temaction = 3;

				int weitotalpoint = 0;
				int shutotalpoint = 0;
				int wutotalpoint = 0;

				std::multimap<int,int,std::greater<int>> killwei;
				std::multimap<int,int,std::greater<int>> deadwei;
				std::multimap<int,int,std::greater<int>> pointswei;

				std::multimap<int,int,std::greater<int>> killshu;
				std::multimap<int,int,std::greater<int>> deadshu;
				std::multimap<int,int,std::greater<int>> pointsshu;

				std::multimap<int,int,std::greater<int>> killwu;
				std::multimap<int,int,std::greater<int>> deadwu;
				std::multimap<int,int,std::greater<int>> pointswu;

				int killbudui  = 0;
				int killxiaoguai  = 0;
				unsigned short otherjifen = 0;
				int playertotalpoint = 0;
				int temp = 0;

				for (BattleStatsMap::iterator itr = battleStatsMap->begin(); itr != battleStatsMap->end(); itr++)
				{
					int playerid = itr->second->getPlayerID();
					if (itr->second->getShiliID() == 10)
					{
						KillArmyInfoMap & deadmap = itr->second->getDeadCount();
						KillArmyInfoMap & killmap = itr->second->getKillArmyInfoMap();
						killbudui  = itr->second->getKillCount();
						killxiaoguai  = itr->second->getNpcKillCount();
						otherjifen = itr->second->getPoint();

						playertotalpoint = killbudui*5 + killxiaoguai*10 +(int)otherjifen;
						if(playertotalpoint != 0)
							pointswei.insert(std::make_pair(playertotalpoint,playerid));

						weitotalpoint += playertotalpoint;

						/*temp = 0;
						for (KillArmyInfoMap::iterator iter = deadmap.begin(); iter != deadmap.end(); iter++)
						{
						temp += iter->second;
						}
						if(temp != 0)
						deadwei.insert(std::make_pair(temp,playerid));

						temp = 0;
						for (KillArmyInfoMap::iterator iter = killmap.begin(); iter != killmap.end(); iter++)
						{
						temp += iter->second;
						}
						if(temp != 0)
						killwei.insert(std::make_pair(temp,playerid));*/
					}
					else if (itr->second->getShiliID() == 11)
					{
						KillArmyInfoMap & deadmap = itr->second->getDeadCount();
						KillArmyInfoMap & killmap = itr->second->getKillArmyInfoMap();
						int killbudui  = itr->second->getKillCount();
						int killxiaoguai  = itr->second->getNpcKillCount();
						unsigned short otherjifen = itr->second->getPoint();

						int playertotalpoint = killbudui*5 + killxiaoguai*10 +(int)otherjifen;
						if(playertotalpoint != 0)
							pointsshu.insert(std::make_pair(playertotalpoint,playerid));

						shutotalpoint += playertotalpoint;

						/*int temp = 0;
						for (KillArmyInfoMap::iterator iter = deadmap.begin(); iter != deadmap.end(); iter++)
						{
							temp += iter->second;
						}
						if(temp != 0)
							deadshu.insert(std::make_pair(temp,playerid));

						temp = 0;
						for (KillArmyInfoMap::iterator iter = killmap.begin(); iter != killmap.end(); iter++)
						{
							temp += iter->second;
						}
						if(temp != 0)
							killshu.insert(std::make_pair(temp,playerid));*/
					}
					else if (itr->second->getShiliID() == 12)
					{
						KillArmyInfoMap & deadmap = itr->second->getDeadCount();
						KillArmyInfoMap & killmap = itr->second->getKillArmyInfoMap();
						int killbudui  = itr->second->getKillCount();
						int killxiaoguai  = itr->second->getNpcKillCount();
						unsigned short otherjifen = itr->second->getPoint();

						int playertotalpoint = killbudui*5 + killxiaoguai*10 +(int)otherjifen;
						if(playertotalpoint != 0)
							pointswu.insert(std::make_pair(playertotalpoint,playerid));

						wutotalpoint += playertotalpoint;

						/*int temp = 0;
						for (KillArmyInfoMap::iterator iter = deadmap.begin(); iter != deadmap.end(); iter++)
						{
						temp += iter->second;
						}
						if(temp != 0)
						deadwu.insert(std::make_pair(temp,playerid));

						temp = 0;
						for (KillArmyInfoMap::iterator iter = killmap.begin(); iter != killmap.end(); iter++)
						{
						temp += iter->second;
						}
						if(temp != 0)
						killwu.insert(std::make_pair(temp,playerid));*/
					}
				}
				weitotalpoint += *pointsshiliwei;
				shutotalpoint += *pointsshilishu;
				wutotalpoint += *pointshiliwu;

				//发包
				ref_ptr<crTableIO>worldawardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldRankAwardTab);
				int rowcount = worldawardTab->getRowCount();
				crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
				if (netConductor)
				{
					std::multimap<int,int,std::greater<int>> *rankmap;
					for (int j=0; j<9; j++)
					{
						rankmap = NULL;
						switch (j)
						{
						case 0:
							rankmap = &pointswei;
							break;
						case 1:
							rankmap = &pointsshu;
							break;
						case 2:
							rankmap = &pointswu;
							break;
						case 3:
							//rankmap = &killwei;
							break;
						case 4:
							//rankmap = &killshu;
							break;
						case 5:
							//rankmap = &killwu;
							break;
						case 6:
							//rankmap = &deadwei;
							break;
						case 7:
							//rankmap = &deadshu;
							break;
						case 8:
							//rankmap = &deadwu; 
							break;
						default:
							break;
						}

						if(!rankmap || rankmap->size() == 0)
							continue;

						int i = 0;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						if(rankmap->size() <= rowcount)
						{
							stream->createBuf(3+(rankmap->size())*4);
							stream->_writeUChar(0);

							if(j>=0 && j<3)
								stream->_writeUChar(2);
							else if(j>=3 && j<6)
								stream->_writeUChar(0);
							else
								stream->_writeUChar(1);

							stream->_writeUChar(rankmap->size());
						}
						else
						{
							stream->createBuf(3+4*rowcount);
							stream->_writeUChar(0);

							if(j>=0 && j<3)
								stream->_writeUChar(2);
							else if(j>=3 && j<6)
								stream->_writeUChar(0);
							else
								stream->_writeUChar(1);

							stream->_writeUChar((unsigned char)rowcount);
						}
						for (std::multimap<int,int,std::greater<int>>::iterator it = rankmap->begin(); it != rankmap->end() && i<rowcount; it++,i++)
						{
							stream->_writeInt(it->second);
						}
						crJXJCountryCapitalAwardPacket packet;
						crJXJCountryCapitalAwardPacket::buildRequestPacket(packet,WCH_JXJRecvWorldFuBenRankAward,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(17);
					stream->_writeUChar(1);
					stream->_writeInt(m_exercisetype);
					stream->_writeInt(weitotalpoint);
					stream->_writeInt(shutotalpoint);
					stream->_writeInt(wutotalpoint);

					crJXJCountryCapitalAwardPacket packet;
					crJXJCountryCapitalAwardPacket::buildRequestPacket(packet,WCH_JXJRecvWorldFuBenRankAward,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);

					ref_ptr<crStreamBuf> achievestream = new crStreamBuf;
					achievestream->createBuf(2+(pointswei.size()+pointsshu.size()+pointswu.size())*4);
					achievestream->_writeUChar(2);
					achievestream->_writeUChar(pointswei.size()+pointsshu.size()+pointswu.size());
					for (std::multimap<int,int,std::greater<int>>::iterator itr = pointswei.begin(); itr != pointswei.end(); itr++)
					{
						achievestream->_writeInt(itr->second);
					}
					for (std::multimap<int,int,std::greater<int>>::iterator itr = pointsshu.begin(); itr != pointsshu.end(); itr++)
					{
						achievestream->_writeInt(itr->second);
					}
					for (std::multimap<int,int,std::greater<int>>::iterator itr = pointswu.begin(); itr != pointswu.end(); itr++)
					{
						achievestream->_writeInt(itr->second);
					}
					crJXJCountryCapitalAwardPacket achievepacket;
					crJXJCountryCapitalAwardPacket::buildRequestPacket(achievepacket,WCH_JXJRecvWorldFuBenRankAward,achievestream.get());
					netConductor->getNetManager()->sendPacket("all",achievepacket);

					for (int i=0; i<3; i++)
					{
						int jifen = 0;
						if (i==0)
							jifen = weitotalpoint;
						else if (i==1)
							jifen = shutotalpoint;
						else
							jifen = wutotalpoint;

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(8);
						stream->_writeInt(m_exercisetype);
						stream->_writeInt(jifen);
						
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJDirectGetAwardWindowPop,stream.get());
						m_this->sendPacketToGroup(10+i,packet);
					}
				}
			}
			else if(m_startorend == 1)
			{
				*temaction = (unsigned char)m_exercisetype;

				battleStatsMap->clear();
				praisedidMap->clear();
				*pointsshiliwei = 0;
				*pointsshilishu = 0;
				*pointshiliwu = 0;

				crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
				if (netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(1);

					crJXJCountryCapitalAwardPacket packet;
					crJXJCountryCapitalAwardPacket::buildRequestPacket(packet,WCH_JXJWorldFuBenStart,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}

				ref_ptr<crStreamBuf> stream1 = new crStreamBuf;
				stream1->createBuf(1);
				stream1->_writeUChar((unsigned char)m_exercisetype);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJHuoDongStartClearData,stream1.get());
				m_this->sendPacketToAll(packet);
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJgServerRecvWorldFuBenAwardMethod
//
/////////////////////////////////////////
crJXJgServerRecvWorldFuBenAwardMethod::crJXJgServerRecvWorldFuBenAwardMethod(){}
crJXJgServerRecvWorldFuBenAwardMethod::crJXJgServerRecvWorldFuBenAwardMethod(const crJXJgServerRecvWorldFuBenAwardMethod& handle):
	crMethod(handle)
{
}
void crJXJgServerRecvWorldFuBenAwardMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJgServerRecvWorldFuBenAwardMethod::addParam(int i, const std::string& str)
{
}

void crJXJgServerRecvWorldFuBenAwardMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		unsigned char type = m_stream->_readUChar();

		if (type == 0)
		{
			unsigned char ranktype = m_stream->_readUChar();
			unsigned char size = m_stream->_readUChar();

			ref_ptr<crTableIO>worldawardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldRankAwardTab);
			int typeindex1 =  worldawardTab->getTitleIndex("类型1");
			int typeindex2 =  worldawardTab->getTitleIndex("类型2");
			int awardindex1 =  worldawardTab->getTitleIndex("奖励1");
			int awardindex2 =  worldawardTab->getTitleIndex("奖励2");
			crTableIO::StrVec record;
			
			ref_ptr<crPlayerRewardData> reward_info;
			ref_ptr<crBagItemData> itemdata;
			crVector3i vec3;
			ref_ptr<crGameServerPlayerData> gsplayerData ;
			ref_ptr<crData> playerdata;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			for (int i=0; i<size; i++)
			{
				int playerid = m_stream->_readInt();

				reward_info = new crPlayerRewardData;

				if(worldawardTab->queryOneRecord(0,crArgumentParser::appItoa(i+1),record)>=0)
				{
					if (atoi(record[typeindex1].c_str()) == 10)
					{
						if(crArgumentParser::appAtoVec(record[awardindex1],vec3) && vec3[0]>0)
						{
							itemdata = new crBagItemData;
							itemdata->setItemID(vec3[0]);
							itemdata->setEquipMagic(vec3[1]);
							itemdata->setItemCount(vec3[2]);
							reward_info->getRewardItemsVec().push_back(itemdata);
							reward_info->setPlayerID(playerid);
							reward_info->setType(GP_RewardFubenEnd);
						}
					}
					if (atoi(record[typeindex2].c_str()) == 10)
					{
						if(crArgumentParser::appAtoVec(record[awardindex2],vec3) && vec3[0]>0)
						{
							itemdata = new crBagItemData;
							itemdata->setItemID(vec3[0]);
							itemdata->setEquipMagic(vec3[1]);
							itemdata->setItemCount(vec3[2]);
							reward_info->getRewardItemsVec().push_back(itemdata);
						}
					}

					gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
					if (gsplayerData.valid() && gsplayerData->getPlayerGameData())
					{
						playerdata = gsplayerData->getPlayerGameData()->getDataClass();
						if(playerdata.valid())
						{
							playerdata->excHandle(MAKEINT64(WCH_LockData,1));
							gsplayerData->getPlayerGameData()->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward_info.get(),NULL));
							playerdata->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
					else
					{
						m_this->doEvent(WCH_JXJCreateRewardMail,MAKEINT64(reward_info.get(),NULL));
					}
				}
			}
		}
		else if(type == 1)
		{
			int time =  m_stream->_readInt();
			int point[3];
			point[0] = m_stream->_readInt();
			point[1] = m_stream->_readInt();
			point[2] = m_stream->_readInt();

			int rewardzijin = 0;
			int first = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,0).c_str());
			int second = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,1).c_str());
			int third = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,2).c_str());

			crData *serdata = m_this->getDataClass();
			crJXJShili *shili;
			crData *shiliData;
			if (serdata)
			{
				for (int i=0; i<3; i++)
				{
					rewardzijin = 0;
					if (point[i] >= third)
						rewardzijin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,5).c_str());
					else if(point[i] >= second)
						rewardzijin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,4).c_str());
					else if(point[i] >= first)
						rewardzijin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,3).c_str());

					serdata->getParam(WCHDATA_JXJShiliWei+i,param);
					shili = (crJXJShili *)param;
					if(shili && shili->getDataClass())
					{
						shiliData = shili->getDataClass();
						shiliData->excHandle(MAKEINT64(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJGuoJiaJiFen,param);
						crVector3i *shilipoint = (crVector3i *)param;
						(*shilipoint)[time] = point[i];

						shiliData->getParam(WCHDATA_JXJCountryCapital,param);
						int *countrycapital = (int *)param;
						*countrycapital += rewardzijin;

						shiliData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
		}
		else if(type == 2)
		{
			int size = m_stream->_readUChar();
			int playerid = 0;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> gsplayerData;
			ref_ptr<crData> playerdata;
			for (int i=0; i<size; i++)
			{
				playerid = m_stream->_readInt();
				gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				if (gsplayerData.valid() && gsplayerData->getPlayerGameData())
				{
					playerdata = gsplayerData->getPlayerGameData()->getDataClass();
					if(playerdata.valid())
					{
						playerdata->excHandle(MAKEINT64(WCH_LockData,1));
						playerdata->getParam(WCHDATA_JXJPassWorldFuBenCount,param);
						unsigned short *count = (unsigned short *)param;
						*count += 1;
						playerdata->excHandle(MAKEINT64(WCH_LockData,0));

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJPassWorldFuben,NULL);
						gameServer->getNetManager()->sendPacket(gsplayerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvPassWorldFuBenCountMethod
//
/////////////////////////////////////////
crJXJRecvPassWorldFuBenCountMethod::crJXJRecvPassWorldFuBenCountMethod():
	m_netType(GameClient_Game){}
crJXJRecvPassWorldFuBenCountMethod::crJXJRecvPassWorldFuBenCountMethod(const crJXJRecvPassWorldFuBenCountMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPassWorldFuBenCountMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvPassWorldFuBenCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvPassWorldFuBenCountMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if (m_netType == GameClient_Game)
		{
			ref_ptr<crData> playerdata = m_this->getDataClass();
			if(playerdata.valid())
			{
				playerdata->excHandle(MAKEINT64(WCH_LockData,1));
				playerdata->getParam(WCHDATA_JXJPassWorldFuBenCount,param);
				unsigned short *count = (unsigned short *)param;
				*count += 1;
				playerdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJQueryWorldFuBenDataMethod
//
/////////////////////////////////////////
crJXJQueryWorldFuBenDataMethod::crJXJQueryWorldFuBenDataMethod()
{
}
crJXJQueryWorldFuBenDataMethod::crJXJQueryWorldFuBenDataMethod(const crJXJQueryWorldFuBenDataMethod& handle):
	crMethod(handle)
{
}
void crJXJQueryWorldFuBenDataMethod::inputParam(int i, void *param)
{
}
void crJXJQueryWorldFuBenDataMethod::addParam(int i, const std::string& str)
{
}
void crJXJQueryWorldFuBenDataMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getDataClass())
		{
			void *param;
			crData *data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myshili = *(unsigned char *)param;

			/*ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(myshili);
			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJQueryWorldFuBenData,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);*/

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(myshili);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJQueryWorldFuBenData,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}



/////////////////////////////////////////
//
//crJXJInitWorldFuBenRankDataMethod
//
/////////////////////////////////////////
crJXJInitWorldFuBenRankDataMethod::crJXJInitWorldFuBenRankDataMethod()
{}
crJXJInitWorldFuBenRankDataMethod::crJXJInitWorldFuBenRankDataMethod(const crJXJInitWorldFuBenRankDataMethod& handle):
	crMethod(handle),
	m_playerData(handle.m_playerData)
{
}
void crJXJInitWorldFuBenRankDataMethod::addParam(int i, const std::string& str)
{
}
void crJXJInitWorldFuBenRankDataMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerData = (crSceneServerPlayerData *)LOINT64(param64);
		}
		break;
	}
}
void crJXJInitWorldFuBenRankDataMethod::operator()(crHandle &handle)
{
	if(m_playerData.valid() && m_this && m_this->getDataClass())
	{
		int myplayerid = m_playerData->getPlayerID();
		crData *data = m_this->getDataClass();
		void * param;
		if(m_playerData->getPlayerGameData() && m_playerData->getPlayerGameData()->getDataClass())
		{
			int time2=0;
			int time3=0;
			int eventtype=0;
			unsigned char temaction = 0;

			ref_ptr<crTableIO> codtexttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crPlayerGameData *playerGameData = m_playerData->getPlayerGameData();
			crData *gameData = playerGameData->getDataClass();
			gameData->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;

			bool ishaveshowed = true;
			std::set<int> praisedid;
			std::multimap<int,PlayerData,std::greater<int>> killrankmap;
			std::multimap<int,PlayerData,std::greater<int>> deadrankmap;
			std::multimap<int,PlayerData,std::greater<int>> pointsrankmap;

			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJBattleStatsMap,param);
			BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;

			data->getParam(WCHDATA_JXJWeiGuoPoints+shiliid-c_startShiliID,param);
			unsigned short guojiajifen = *(unsigned short *)param;
			int shilijifen = (int)guojiajifen;

			data->getParam(WCHDATA_JXJWorldPraisedIdMap,param);
			std::map<int,std::set<int>> *praisedidMap = (std::map<int,std::set<int>> *)param;
			std::map<int,std::set<int>>::iterator iter = praisedidMap->find(myplayerid);
			if(iter != praisedidMap->end())
				praisedid = iter->second;

			data->getParam(WCHDATA_JXJWorldTemHuoDong,param);
			temaction = *(unsigned char *)param;
			if (temaction == 4)
			{
				std::vector<std::string> timerecord;
				std::vector<int> actiontime;
				if(codtexttab.valid() && codtexttab->queryOneRecord(0,crArgumentParser::appItoa(9219),timerecord)>=0)
				{
					time_t t = time(0);
					struct  tm  sTm = *localtime(&t);
					int hour = sTm.tm_hour;
					int minu = sTm.tm_min;
					int sec = sTm.tm_sec;
					int nowtime = hour*3600 + minu*60 + sec;

					crArgumentParser::appAtoVec(timerecord[1],actiontime);
					
					if(nowtime >= actiontime[0] && nowtime < actiontime[1])
						temaction = 0;
					else if(nowtime >= actiontime[2] && nowtime < actiontime[3])
						temaction = 1;
					else if(nowtime >= actiontime[4] && nowtime < actiontime[5])
						temaction = 2;
					else
						temaction = 3;
				}
			}
			data->getParam(WCHDATA_JXJScenarioVariableMap,param);
			ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
			ScenarioVariableMap::iterator svmitr = variableMap->find("daojishi");
			if (svmitr != variableMap->end())
			{
				time3 = (int)svmitr->second;
			}
			svmitr = variableMap->find("timenote");
			if (svmitr != variableMap->end())
			{
				eventtype = (int)svmitr->second;
			}
			else
			{
				if (codtexttab.valid())
				{
					std::vector<std::string> record;
					std::vector<int> eventstime;
					std::vector<std::string> events;
					int times;
					for (int i=10000; i<10022; i++)
					{
						if(codtexttab->queryOneRecord(0,crArgumentParser::appItoa(i),record)>=0)
						{
							crArgumentParser::appAtoVec(record[1],events);
							times = (atoi(events[1].c_str()))*3600 + (atoi(events[2].c_str()))*60;
							eventstime.push_back(times);
						}
					}
					time_t t = time(0);
					struct  tm  sTm = *localtime(&t);
					int hour = sTm.tm_hour;
					int minu = sTm.tm_min;
					int sec = sTm.tm_sec;
					int nowtime = hour*3600 + minu*60 + sec;
					std::vector<int>::iterator iter = eventstime.begin();
					std::vector<int>::iterator iter1 = iter + 1;
					for (; iter1!=eventstime.end(); iter++,iter1++)
					{
						if(nowtime < *iter)
						{
							(*variableMap)[std::string("timenote")] = iter - eventstime.begin();
							(*variableMap)[std::string("time")] = *iter;
							break;
						}
						else if(nowtime >= *iter && nowtime < *iter1)
						{
							(*variableMap)[std::string("timenote")] = iter1 - eventstime.begin();
							(*variableMap)[std::string("time")] = *iter1;
							break;
						}
						else if(iter1 == eventstime.end()-1)
						{
							(*variableMap)[std::string("timenote")] = 0;
							(*variableMap)[std::string("time")] = eventstime[0];
							break;
						}
					}
				}
			}

			svmitr = variableMap->find("time");
			if (svmitr != variableMap->end())
			{
				time2 = (int)svmitr->second;
			}

			
			int killbudui =0 ;
			int killxiaoguai  = 0;
			unsigned short otherjifen = 0;
			int playertotalpoint = 0;
			int temp = 0;
			std::string playername;
			PlayerData idnamedata;

			for (BattleStatsMap::iterator itr = battleStatsMap->begin(); itr != battleStatsMap->end(); itr++)
			{
				
				if (itr->second->getShiliID() == shiliid)
				{
					int playerid = itr->second->getPlayerID();
					playername = itr->second->getPlayerName();
					idnamedata.playerid = playerid;
					idnamedata.name = playername;

					KillArmyInfoMap & deadmap = itr->second->getDeadCount();
					KillArmyInfoMap & killmap = itr->second->getKillArmyInfoMap();
					killbudui  = itr->second->getKillCount();
					killxiaoguai  = itr->second->getNpcKillCount();
					otherjifen = itr->second->getPoint();

					if(playerid == myplayerid)
						ishaveshowed = itr->second->getShow();

					playertotalpoint = killbudui*5 + killxiaoguai*10 +(int)otherjifen;
					shilijifen += playertotalpoint;
					if(playertotalpoint != 0)
						pointsrankmap.insert(std::make_pair(playertotalpoint,idnamedata));

					if (itr->second->getPlayerID() == myplayerid)
					{
						temp = 0;
						for (KillArmyInfoMap::iterator iter = deadmap.begin(); iter != deadmap.end(); iter++)
						{
							temp += iter->second;
						}
						if(temp != 0)
							deadrankmap.insert(std::make_pair(temp,idnamedata));

						temp = 0;
						for (KillArmyInfoMap::iterator iter = killmap.begin(); iter != killmap.end(); iter++)
						{
							temp += iter->second;
						}
						if(temp != 0)
							killrankmap.insert(std::make_pair(temp,idnamedata));
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));

			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetManager *netManager = sceneServer->getNetManager();
			std::multimap<int,PlayerData,std::greater<int>> *rankmap;
			for (int j=0; j<3; j++)
			{
				switch (j)
				{
				case 0:
					rankmap = &killrankmap;
					break;
				case 1:
					rankmap = &deadrankmap;
					break;
				case 2:
					rankmap = &pointsrankmap;
					break;
				default:
					break;
				}

				int i = 0;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if(rankmap->size() <= 10)
				{
					stream->createBuf(3+(rankmap->size())*8);
					stream->_writeUChar(1);
					stream->_writeUChar(j);
					stream->_writeUChar(rankmap->size());
				}
				else
				{
					stream->createBuf(83);
					stream->_writeUChar(1);
					stream->_writeUChar(j);
					stream->_writeUChar(10);
				}

				crVector2i rankpair;
				for (std::multimap<int,PlayerData,std::greater<int>>::iterator it = rankmap->begin(); it != rankmap->end() && i<10; it++,i++)
				{
					rankpair[0] = it->first;
					rankpair[1] = it->second.playerid;
					stream->_writeVec2i(rankpair);
					stream->_writeString(it->second.name);

				}

				i = 1;
				bool singal = true;
				for (std::multimap<int,PlayerData,std::greater<int>>::iterator it = rankmap->begin(); it != rankmap->end(); it++,i++)
				{
					if(it->second.playerid == myplayerid)
					{
						stream->_writeInt(i);
						stream->_writeInt(it->first);
						singal = false;
						break;
					}
				}
				if (singal)
				{
					stream->_writeInt(0);
					stream->_writeInt(0);
				}
				
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJQueryWorldFuBenData,stream.get());

				netManager->sendPacket(m_playerData->getPlayerConnectServerAddress(),packet);
			}
			
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4+(praisedid.size())*4);
			stream->_writeUChar(1);
			stream->_writeUChar(3);
			stream->_writeUChar(praisedid.size());
			stream->_writeBool(ishaveshowed);
			stream->_writeUChar(temaction);
			stream->_writeInt(shilijifen);
			for (std::set<int>::iterator ptr = praisedid.begin(); ptr != praisedid.end(); ptr++)
			{
				stream->_writeInt(*ptr);
			}
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJQueryWorldFuBenData,stream.get());
			netManager->sendPacket(m_playerData->getPlayerConnectServerAddress(),packet);

			time_t t = time(0);
			struct  tm  sTm = *localtime(&t);
			int hour = sTm.tm_hour;
			int minu = sTm.tm_min;
			int sec = sTm.tm_sec;

			ref_ptr<crStreamBuf> gstream = new crStreamBuf;
			gstream->createBuf(9);
			gstream->_writeUChar(1);
			//gstream->_writeInt(time);
			gstream->_writeInt(eventtype);
			gstream->_writeInt(time2);
			gstream->_writeInt(hour);
			gstream->_writeInt(minu);
			gstream->_writeInt(sec);
			
			crPlayerDataSceneEventPacket gpacket;
			crPlayerDataSceneEventPacket::buildReplyPacket(gpacket,myplayerid,WCH_JXJWorldEventRemainTime,gstream.get());
			netManager->sendPacket(m_playerData->getPlayerConnectServerAddress(),gpacket);
		}	
	}
}


/////////////////////////////////////////
//
//crJXJDisposeQueryWorldFuBenDataMethod
//
/////////////////////////////////////////
crJXJDisposeQueryWorldFuBenDataMethod::crJXJDisposeQueryWorldFuBenDataMethod():
	m_netType(GameClient_Game){}
crJXJDisposeQueryWorldFuBenDataMethod::crJXJDisposeQueryWorldFuBenDataMethod(const crJXJDisposeQueryWorldFuBenDataMethod& handle):
	crMethod(handle)
{
}
void crJXJDisposeQueryWorldFuBenDataMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJDisposeQueryWorldFuBenDataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJDisposeQueryWorldFuBenDataMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if (m_netType == GameServer)
		{
			unsigned char shiliid = m_stream->_readUChar();
			int tianshu = 0;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			crData *serdata = crServerBrainHandle::getInstance()->getDataClass();
			if (serdata)
			{
				crJXJShili *shili;
				crData *shiliData;
				serdata->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
				shili = (crJXJShili *)param;

				if(shili && shili->getDataClass())
				{
					shiliData = shili->getDataClass();

					shiliData->getParam(WCHDATA_JXJGuoJiaJiFen,param);
					crVector3i weishilipoint = *(crVector3i *)param;

					int playerid = m_this->getPlayerID();
					crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
					ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
					if(playerData.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(17);
						stream->_writeUChar(0);
						stream->_writeVec3i(weishilipoint);
						stream->_writeInt(tianshu);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJQueryWorldFuBenData,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char type = m_stream->_readUChar(); 

			ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (rankcanvas.valid() && rankcanvas->getDataClass())
			{
				ref_ptr<crData> canvasdata = rankcanvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				if (type == 0)
				{
					crVector3i shilijifen  = m_stream->_readVec3i();
					int tianshu = m_stream->_readInt();

					canvasdata->getParam(WCHDATA_JXJCountryPoints,param);
					crVector3i *shilipoint = (crVector3i *)param;
					*shilipoint = shilijifen;

					canvasdata->getParam(WCHDATA_JXJOpenDiJiTian,param);
					int *dijitian = (int *)param;
					*dijitian = tianshu;
				}
				else if(type == 1)
				{
					unsigned char ranktype = m_stream->_readUChar(); 
					unsigned char size = m_stream->_readUChar(); 

					canvasdata->getParam(WCHDATA_JXJMyWorldInfo,param);
					int * myinfo = (int *)param;

					PlayerData idnamedata;
					crVector2i rankpair;
					if (ranktype == 0)
					{
						canvasdata->getParam(WCHDATA_JXJKillRank,param);
						std::multimap<int,PlayerData,std::greater<int>> *killrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
						killrankmap->clear();
						for (int i=0; i<size; i++)
						{
							rankpair = m_stream->_readVec2i();
							idnamedata.playerid = rankpair[1];
							idnamedata.name = m_stream->_readString();
							killrankmap->insert(std::make_pair(rankpair[0],idnamedata));
						}
						myinfo[0] = m_stream->_readInt();
						myinfo[3] = m_stream->_readInt();
					}
					else if (ranktype == 1)
					{
						canvasdata->getParam(WCHDATA_JXJDeadRank,param);
						std::multimap<int,PlayerData,std::greater<int>> *deadrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
						deadrankmap->clear();
						for (int i=0; i<size; i++)
						{
							rankpair = m_stream->_readVec2i();
							idnamedata.playerid = rankpair[1];
							idnamedata.name = m_stream->_readString();
							deadrankmap->insert(std::make_pair(rankpair[0],idnamedata));
						}
						myinfo[1] = m_stream->_readInt();
						myinfo[4] = m_stream->_readInt();
					}
					else if(ranktype == 2)
					{
						canvasdata->getParam(WCHDATA_JXJPointsRank,param);
						std::multimap<int,PlayerData,std::greater<int>> *pointrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
						pointrankmap->clear();
						for (int i=0; i<size; i++)
						{
							rankpair = m_stream->_readVec2i();
							idnamedata.playerid = rankpair[1];
							idnamedata.name = m_stream->_readString();
							pointrankmap->insert(std::make_pair(rankpair[0],idnamedata));
						}
						myinfo[2] = m_stream->_readInt();
						myinfo[5] = m_stream->_readInt();
					}
					else if(ranktype == 3)
					{
						bool ishaveshowed = m_stream->_readBool();
						unsigned char temhuodong = m_stream->_readUChar();
						int shilijifen = m_stream->_readInt();
						canvasdata->getParam(WCHDATA_JXJWorldIsCanShow,param);
						bool *iscanshow = (bool *)param;
						*iscanshow = ishaveshowed;
						canvasdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
						unsigned char *tempaction = (unsigned char *)param;
						*tempaction = temhuodong;

						canvasdata->getParam(WCHDATA_JXJTempCountryPoints,param);
						int *temguojiajifen = (int *)param;
						*temguojiajifen = shilijifen;

						canvasdata->getParam(WCHDATA_JXJWorldPraisedI,param);
						std::set<int> *praisedidset = (std::set<int> *)param;
						praisedidset->clear();
						for (int i=0; i<size; i++)
						{
							praisedidset->insert(m_stream->_readInt());
						}
					}
				}
				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

///////////////////////////////////////
//
//crJXJWorldEventNoticeMethod
//
///////////////////////////////////////
crJXJWorldEventNoticeMethod::crJXJWorldEventNoticeMethod() 
{
}
crJXJWorldEventNoticeMethod::crJXJWorldEventNoticeMethod(const crJXJWorldEventNoticeMethod& handle) :
	crMethod(handle)
	
{
}
void crJXJWorldEventNoticeMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJWorldEventNoticeMethod::inputParam(int i, void *param)
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
		m_this = (crRoom *)param;
		break;
	}
}
void crJXJWorldEventNoticeMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		crData *data = m_this->getDataClass();
		if (data)
		{
			void *param;
			int eventtype = 0;
			int time2 = 0;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJScenarioVariableMap,param);
			ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
			ScenarioVariableMap::iterator svmitr = variableMap->find("timenote");
			if (svmitr != variableMap->end())
			{
				eventtype = (int)svmitr->second;
			}
			svmitr = variableMap->find("time");
			if (svmitr != variableMap->end())
			{
				time2 = (int)svmitr->second;
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));

			time_t t = time(0);
			struct  tm  sTm = *localtime(&t);
			int hour = sTm.tm_hour;
			int minu = sTm.tm_min;
			int sec = sTm.tm_sec;

			ref_ptr<crStreamBuf> gstream = new crStreamBuf;
			gstream->createBuf(9);
			gstream->_writeUChar(1);
			gstream->_writeInt(eventtype);
			gstream->_writeInt(time2);
			gstream->_writeInt(hour);
			gstream->_writeInt(minu);
			gstream->_writeInt(sec);

			crPlayerDataSceneEventPacket gpacket;
			crPlayerDataSceneEventPacket::buildReplyPacket(gpacket,0,WCH_JXJWorldEventRemainTime,gstream.get());
			m_this->sendPacketToAll(gpacket);
		}
	}
}

/////////////////////////////////////////
//
//crJXJWorldFuBenStartBrocastMethod
//
/////////////////////////////////////////
crJXJWorldFuBenStartBrocastMethod::crJXJWorldFuBenStartBrocastMethod(){}
crJXJWorldFuBenStartBrocastMethod::crJXJWorldFuBenStartBrocastMethod(const crJXJWorldFuBenStartBrocastMethod& handle):
	crMethod(handle)
{
}
void crJXJWorldFuBenStartBrocastMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJWorldFuBenStartBrocastMethod::addParam(int i, const std::string& str)
{
}

void crJXJWorldFuBenStartBrocastMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		//void *param;
		unsigned char type = m_stream->_readUChar();
	
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		
		ref_ptr<crGameServerPlayerData> recvPlayerData;
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crNetManager *netManager = gameServer->getNetManager();
		if(netDataManager && netManager)
		{
			void *param;
			crData *roledata;
			unsigned char lv;
			unsigned char openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldFubenOpenLevel).c_str());
			crPlayerDataEventPacket packet;
			netDataManager->lockPlayerDataMap();
			crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (recvPlayerData.valid() && recvPlayerData->getSceneID()==0 && recvPlayerData->getPlayerGameData() && recvPlayerData->getPlayerGameData()->getMainRole())
				{
					roledata = recvPlayerData->getPlayerGameData()->getMainRole()->getDataClass();
					if(roledata)
					{
						roledata->getParam(WCHDATA_Level,param);
						lv = *(unsigned char *)param;
						if(lv>=openlv)
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,itr->first,WCH_JXJWorldFuBenClientBrocast,NULL);
							netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
			}
			netDataManager->unLockPlayerDataMap();
		}
	}	
}

/////////////////////////////////////////
//
//crJXJClientWorldFuBenBrocastMethod
//
/////////////////////////////////////////
crJXJClientWorldFuBenBrocastMethod::crJXJClientWorldFuBenBrocastMethod():
	m_netType(GameClient_Game){}
crJXJClientWorldFuBenBrocastMethod::crJXJClientWorldFuBenBrocastMethod(const crJXJClientWorldFuBenBrocastMethod& handle):
	crMethod(handle)
{
}
void crJXJClientWorldFuBenBrocastMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientWorldFuBenBrocastMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_text = str;
		break;
	}
}

void crJXJClientWorldFuBenBrocastMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		//void *param;
		if (m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid())
			{
				ref_ptr<crStaticTextWidgetNode > text = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_text));
				std::string neirong = "世界战场活动已经开启，是否前往世界战场？";
				if(text.valid())
					text->setString(neirong);

				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJDirectlyGetAwardWindowpopMethod
//
/////////////////////////////////////////
crJXJDirectlyGetAwardWindowpopMethod::crJXJDirectlyGetAwardWindowpopMethod():
	m_netType(GameClient_Game){}
crJXJDirectlyGetAwardWindowpopMethod::crJXJDirectlyGetAwardWindowpopMethod(const crJXJDirectlyGetAwardWindowpopMethod& handle):
	crMethod(handle)
{
}
void crJXJDirectlyGetAwardWindowpopMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJDirectlyGetAwardWindowpopMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_rankcanvas = str;
		break;
	}
}

void crJXJDirectlyGetAwardWindowpopMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if (m_netType == GameClient_Game)
		{
			int huodongtype =  m_stream->_readInt();
			int jifen =  m_stream->_readInt();
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas(m_rankcanvas);
			if (rankcanvas.valid() && rankcanvas->getDataClass())
			{
				ref_ptr<crData> canvasdata = rankcanvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				canvasdata->getParam(WCHDATA_JXJCountryPoints,param);
				crVector3i *shilipoint = (crVector3i *)param;
				(*shilipoint)[huodongtype] = jifen;

				canvasdata->getParam(WCHDATA_JXJTempHuoDongId,param);
				unsigned char *huodongid = (unsigned char *)param;
				*huodongid = (unsigned char)huodongtype;

				canvasdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
				unsigned char *temaction = (unsigned char *)param;
				*temaction = 3;


				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}

			/*if (canvas.valid())
			{
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
			}*/
		}
	}
}

/////////////////////////////////////////
//
//crJXJButtonGetJiFenAwardMethod
//
/////////////////////////////////////////
crJXJButtonGetJiFenAwardMethod::crJXJButtonGetJiFenAwardMethod():
	m_id(4),
	m_this(NULL)
{
}
crJXJButtonGetJiFenAwardMethod::crJXJButtonGetJiFenAwardMethod(const crJXJButtonGetJiFenAwardMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_id(handle.m_id)
{
}
void crJXJButtonGetJiFenAwardMethod::inputParam(int i, void *param)
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
void crJXJButtonGetJiFenAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_id = atoi(str.c_str());
		break;
	case 1:
		m_rankcanvas = str;
		break;
	}
}
void crJXJButtonGetJiFenAwardMethod::operator()(crHandle &handle)
{
	void *param;
	int huodongtype = 0;
	int goldneed = 0;
	if (m_id==0 || m_id == 1 || m_id == 2)
	{
		huodongtype = m_id;
	}
	
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		int huodongtype = 0;
		if (m_id==0 || m_id==1 || m_id==2)
		{
			huodongtype = m_id;
		}
		else if(m_id == 3)
		{
			ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas(m_rankcanvas);
			if (rankcanvas.valid() && rankcanvas->getDataClass())
			{
				ref_ptr<crData> canvasdata = rankcanvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJTempHuoDongId,param);
				unsigned char type = *(unsigned char *)param;
				huodongtype = (int)type;
			}	
		}
		ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
		if (playergamedata.valid() && playergamedata->getDataClass())
		{
			ref_ptr<crData> data = playergamedata->getDataClass();
			data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
			unsigned char gettimes = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char myviplv = *(unsigned char *)param;
			
			int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());
			if (gettimes >= alltimes)
			{
				playergamedata->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9179, NULL)); //领取次数不够
			}
			else
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeInt(huodongtype);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJButtonGetJiFenAward,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}

	ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crData> data = playergamedata->getDataClass();
	data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
	unsigned char gettimes = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char myviplv = *(unsigned char *)param;
	int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());

	ref_ptr<crMultiSwitch> switchnode1,switchnode2,switchnode3;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	switch (m_id)
	{
	case 0:
		switchnode1 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw1"));
		if(switchnode1.valid())
		{
			if(gettimes>=1&&gettimes<alltimes)
			{
				switchnode1->setActiveSwitchSet(1);
			}
			else if (gettimes == alltimes)
			{
				switchnode1->setActiveSwitchSet(2);
			}
			else
			{
				switchnode1->setActiveSwitchSet(0);
			}
		}
		break;
	case 1:
		switchnode2 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw2"));
		if(switchnode2.valid())
		{
			if(gettimes>=1&&gettimes<alltimes)
			{
				switchnode2->setActiveSwitchSet(1);
			}
			else if (gettimes == alltimes)
			{
				switchnode2->setActiveSwitchSet(2);
			}
			else
			{
				switchnode2->setActiveSwitchSet(0);
			}
		}
		break;
	case 2:
		switchnode3 = dynamic_cast<crMultiSwitch *>(canvas->getChildNode("Linquiry_Sw3"));
		if(switchnode3.valid())
		{
			if(gettimes>=1&&gettimes<alltimes)
			{
				switchnode3->setActiveSwitchSet(1);
			}
			else if (gettimes == alltimes)
			{
				switchnode3->setActiveSwitchSet(2);
			}
			else
			{
				switchnode3->setActiveSwitchSet(0);
			}
		}
		break;
	default:
		break;
	}

}

/////////////////////////////////////////
//
//crJXJRecvButtonGetJiFenAwardMethod
//
/////////////////////////////////////////
crJXJRecvButtonGetJiFenAwardMethod::crJXJRecvButtonGetJiFenAwardMethod():
	m_netType(GameClient_Game){}
crJXJRecvButtonGetJiFenAwardMethod::crJXJRecvButtonGetJiFenAwardMethod(const crJXJRecvButtonGetJiFenAwardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvButtonGetJiFenAwardMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvButtonGetJiFenAwardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_huodongdengji);
		break;
	}
}

void crJXJRecvButtonGetJiFenAwardMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this)
	{
		void *param;
		int playerid = m_this->getPlayerID();
		if (m_netType == GameServer)
		{
			ref_ptr<crData> data = m_this->getDataClass();
			if (!data.valid()) return;

			int huodongtype = m_stream->_readInt();
			char _case  = 0;

			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
			unsigned char gettimes = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char myviplv = *(unsigned char *)param;

			crData *mainroledata = m_this->getMainRole()->getDataClass();
			mainroledata->getParam(WCHDATA_Level, param);
			unsigned char mylv = *(unsigned char *)param;

			crData *serdata = crServerBrainHandle::getInstance()->getDataClass();
			if (!serdata) return;
			
			crJXJShili *shili;
			crData *shiliData;
			serdata->getParam(WCHDATA_JXJShiliWei+shiliid-c_startShiliID,param);
			shili = (crJXJShili *)param;

			if(!shili || !shili->getDataClass()) return;
			
			shiliData = shili->getDataClass();
			shiliData->getParam(WCHDATA_JXJGuoJiaJiFen,param);
			crVector3i shilipoint = *(crVector3i *)param;

			std::string dengji;
			int jifen = shilipoint[huodongtype];
			if (jifen < m_huodongdengji[0])
				dengji = "notfinished";
			else if (jifen >= m_huodongdengji[0] && jifen < m_huodongdengji[1])
				dengji = "first";
			else if (jifen >= m_huodongdengji[1] && jifen < m_huodongdengji[2])
				dengji = "second";
			else
				dengji = "third";

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> gsplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(!gsplayerData.valid()) return;

			if (dengji == "notfinished")
			{
				_case = 0;//活动没有完成
			}
			else
			{
				int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());
				if (gettimes >= alltimes)
				{
					_case = -1;//领取次数已满
				}
				else
				{
					ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldJiFenAwardJiSuanTab);
					std::vector<std::string> record;
					int dengjiindex = itemtab->getTitleIndex("等级系数");
					int shijieindex = itemtab->getTitleIndex("世界时间系数");
					int yuanbaoindex = itemtab->getTitleIndex("元宝数");
					int lianshiindex = itemtab->getTitleIndex("粮食基数");
					int mucaiindex = itemtab->getTitleIndex("木材基数");
					int ironindex = itemtab->getTitleIndex("铁矿基数");
					int mapiindex = itemtab->getTitleIndex("马匹基数");
					int jingyanindex = itemtab->getTitleIndex("经验基数");

					int needgold = 0;
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(gettimes+1),record) >= 0)
						needgold = atoi(record[yuanbaoindex].c_str());

					bool needdeductgold = false;
					MoneyChangeData moneydata(needgold,"领积分奖励");
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
					if (moneydata.first == -1)
					{
						_case = -2;//元宝不够
					}
					else
					{
						_case = 1;

						int tianshu = 0;
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));

						float foodbasecount = 0.0;
						float woodbasecount = 0.0;
						float ironbasecount = 0.0;
						float horsebasecount = 0.0;
						float expbasecount = 0.0;

						int foodcount = 0;
						int woodcount = 0;
						int ironcount = 0;
						int horsecount = 0;
						int expcount = 0;

						if (itemtab->queryOneRecord(0,dengji,record) >= 0)
							foodbasecount = atof(record[lianshiindex].c_str());

						if (itemtab->queryOneRecord(0,dengji,record) >= 0)
							woodbasecount = atof(record[mucaiindex].c_str());

						if (itemtab->queryOneRecord(0,dengji,record) >= 0)
							ironbasecount = atof(record[ironindex].c_str());

						if (itemtab->queryOneRecord(0,dengji,record) >= 0)
							horsebasecount = atof(record[mapiindex].c_str());


						if (itemtab->queryOneRecord(0,dengji,record) >= 0)
							expbasecount = atof(record[jingyanindex].c_str());

						float timexishu = 0.0;
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(tianshu),record) >= 0)
							timexishu = atof(record[shijieindex].c_str());
						
						float dengjixishu = 0.0;
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(mylv),record) >= 0)
							dengjixishu = atof(record[dengjiindex].c_str());

						foodcount = (int)(foodbasecount * timexishu * dengjixishu);
						woodcount = (int)(woodbasecount * timexishu * dengjixishu);
						ironcount = (int)(ironbasecount * timexishu * dengjixishu);
						horsecount = (int)(horsebasecount * timexishu * dengjixishu);
						expcount = (int)(expbasecount * timexishu * dengjixishu);

						data->excHandle(MAKEINT64(WCH_LockData,1));
						data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
						unsigned char *times = (unsigned char *)param;
						*times = *times + 1;
						int dijici = *times;
						data->excHandle(MAKEINT64(WCH_LockData,0));

						ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
						reward_info->setFood(foodcount);
						reward_info->setWood(woodcount);
						reward_info->setIron(ironcount);
						reward_info->setHorse(horsecount);
						reward_info->setExperience(expcount);
						reward_info->setPlayerID(playerid);
						reward_info->setType(GP_RewardFubenEnd);
						
						if (gsplayerData->getPlayerGameData())
						{
							ref_ptr<crData> playerdata = gsplayerData->getPlayerGameData()->getDataClass();
							if(playerdata.valid())
							{
								playerdata->excHandle(MAKEINT64(WCH_LockData,1));
								gsplayerData->getPlayerGameData()->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(reward_info.get(),NULL));
								playerdata->excHandle(MAKEINT64(WCH_LockData,0));
							}
						}

						std::string logdata = "获取世界战场积分奖励（whichaction，dijici）：" + crArgumentParser::appVectoa(crVector2i(huodongtype,dijici));
						GameLogData gamelog(Log_GetWorldJifenAward,logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
					}
				}
			}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeChar(_case);
			stream->_writeInt(huodongtype);

			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJButtonGetJiFenAward,stream.get());
			gameServer->getNetManager()->sendPacket(gsplayerData->getPlayerConnectServerAddress(),packet);
		}
		else if (m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			int huodongtype = m_stream->_readInt();

			if (_case == -2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9178, NULL)); //元宝不够
			}
			else if (_case == -1)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9179, NULL)); //领取次数不够
			}
			else if (_case == -0)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9180, NULL)); //没有完成活动
			}
			else if (_case == 1)
			{
				ref_ptr<crData> data = m_this->getDataClass();
				if (data.valid()) 
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
					unsigned char *times = (unsigned char *)param;
					*times = *times + 1;
					data->excHandle(MAKEINT64(WCH_LockData,0));

					ref_ptr<crCanvasNode> rankcanvas = crFilterRenderManager::getInstance()->findCanvas("Renwuzhanbao");
					if (rankcanvas.valid())
						rankcanvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerDisposeShowPraiseMethod
//
/////////////////////////////////////////
crJXJServerDisposeShowPraiseMethod::crJXJServerDisposeShowPraiseMethod():
	m_netType(GameClient_Game){}
crJXJServerDisposeShowPraiseMethod::crJXJServerDisposeShowPraiseMethod(const crJXJServerDisposeShowPraiseMethod& handle):
	crMethod(handle)
{
}
void crJXJServerDisposeShowPraiseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJServerDisposeShowPraiseMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_dengji1);
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_dengji2);
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_dengji3);
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_dengji4);
		break;
	case 4:
		crArgumentParser::appAtoVec(str,m_level1);
		break;
	case 5:
		crArgumentParser::appAtoVec(str,m_level2);
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_level3);
		break;
	}
}

void crJXJServerDisposeShowPraiseMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this)
	{
		void *param;
		if(m_netType == SceneServer)
		{
			char index = m_stream->_readChar();
			
			int playerid = m_this->getPlayerID();
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer); 
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && playerData->getRoomID()!=0)
			{
				crNetDataManager *netDataManager = sceneServer->getNetDataManager();
				crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				CRNetApp::crRoom* room = callback->findRoom(playerData->getRoomID());
				if(room && m_this->getDataClass())
				{
					crData *roomData = room->getDataClass();
					roomData->excHandle(MAKEINT64(WCH_LockData,1));
					if (index == 0)//处理赞
					{
						int prisedplayerid = m_stream->_readInt();
						roomData->getParam(WCHDATA_JXJWorldPraisedIdMap,param);
						std::map<int,std::set<int>> *praisedplayeridMap = (std::map<int,std::set<int>> *)param;
						std::map<int,std::set<int>>::iterator itr = praisedplayeridMap->find(playerid);
						if (itr != praisedplayeridMap->end())
							itr->second.insert(prisedplayerid);
						else
						{
							std::set<int> tempset;
							tempset.insert(prisedplayerid);
							praisedplayeridMap->insert(std::make_pair(playerid,tempset));
						}
					}
					else if(index == 1)//处展示
					{
						int kill = m_stream->_readInt();
						roomData->getParam(WCHDATA_JXJBattleStatsMap,param);
						BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;
						BattleStatsMap::iterator itr = battleStatsMap->find(playerid);
						if(itr!=battleStatsMap->end())
						{
							if (itr->second->getShow())
							{
								itr->second->setShow(false);

								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(8);
								stream->_writeInt(kill);
								stream->_writeInt(itr->second->getKillCount());

								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJWorldExecuteShow,stream.get());
								crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
								gameServer->getNetManager()->sendPacket("all",packet);
							}
						}
					}
					roomData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
		else if(m_netType == GameServer)
		{
			int myplayerid = m_this->getPlayerID();
			char index = m_stream->_readChar();
			int prisedplayerid = m_stream->_readInt();

			crNetConductor *gServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gServer->getNetDataManager()->getPlayerData(prisedplayerid));

			ref_ptr<crGameServerPlayerData> myplayerData = dynamic_cast<crGameServerPlayerData *>(gServer->getNetDataManager()->getPlayerData(myplayerid));

			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				ref_ptr<crPlayerGameData> pgamedata = playerData->getPlayerGameData();
				ref_ptr<crData> playerGameData = playerData->getPlayerGameData()->getDataClass();
				playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
				playerGameData->getParam(WCHDATA_JXJHavePraiseCounts,param);
				int * praisecount = (int *)param;
				int oldcount =  * praisecount;
				playerGameData->getParam(WCHDATA_JXJPlayerPraiseLimit,param);
				unsigned char * limitcount = (unsigned char *)param;

				if (index == 0)//在世界战场里点赞
				{
					*praisecount = * praisecount + 1;

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(2);
					stream->_writeUShort(1);
					stream->_writeUShort(0);
				
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,prisedplayerid,WCH_JXJWorldExecutePraise,stream.get());
					gServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				else if(index == 1)//在聊天栏里点赞
				{
					if (*limitcount > 10)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1);
						stream->_writeUChar(2);


						if(myplayerData.valid())
						{
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJWorldExecutePraise,stream.get());
							gServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);
						}
					}
					else
					{
						*limitcount = *limitcount + 1;
						*praisecount = * praisecount + 1;

						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeUChar(1);
						stream->_writeUChar(1);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,prisedplayerid,WCH_JXJWorldExecutePraise,stream.get());
						gServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					
				}
				int newcount = *praisecount;
				playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

				if (newcount > oldcount)
				{
					ref_ptr<crData> mydata = m_this->getDataClass();
					mydata->excHandle(MAKEINT64(WCH_LockData,1));
					mydata->getParam(WCHDATA_JXJPraiseOtherCounts,param);
					int * praiseothercount = (int *)param;
					*praiseothercount = *praiseothercount + 1;
					
					mydata->excHandle(MAKEINT64(WCH_LockData,0));

					if (myplayerData.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(1);
						stream->_writeUChar(3);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJWorldExecutePraise,stream.get());
						gServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
			else
			{
				if (myplayerData.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(4);

					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,myplayerid,WCH_JXJWorldExecutePraise,stream.get());
					gServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char _case = m_stream->_readUChar();
			if (_case == 2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9184, NULL)); //玩家被赞次数已满
			}
			else if (_case == 4)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9216, NULL)); //玩家不在先线
			}
			else if (_case == 1)
			{
				unsigned char place = m_stream->_readUChar();
				ref_ptr<crData> playerGameData = m_this->getDataClass();
				playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
				playerGameData->getParam(WCHDATA_JXJPlayerPraiseLimit,param);
				unsigned char * limitcount = (unsigned char *)param;
				playerGameData->getParam(WCHDATA_JXJHavePraiseCounts,param);
				int * praisecount = (int *)param;
				
				*praisecount = * praisecount + 1;

				if(place == 1)//世界战场里点赞不需要此限制
					*limitcount = *limitcount + 1;

				playerGameData->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else if(_case == 3)
			{
				ref_ptr<crData> mydata = m_this->getDataClass();
				mydata->excHandle(MAKEINT64(WCH_LockData,1));
				mydata->getParam(WCHDATA_JXJPraiseOtherCounts,param);
				int * praiseothercount = (int *)param;
				
				*praiseothercount = *praiseothercount + 1;
			
				mydata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJWorldHuoDongStartClearDataMethod
//
/////////////////////////////////////////
crJXJWorldHuoDongStartClearDataMethod::crJXJWorldHuoDongStartClearDataMethod():
	m_netType(GameClient_Game){}
crJXJWorldHuoDongStartClearDataMethod::crJXJWorldHuoDongStartClearDataMethod(const crJXJWorldHuoDongStartClearDataMethod& handle):
	crMethod(handle)
{
}
void crJXJWorldHuoDongStartClearDataMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJWorldHuoDongStartClearDataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJWorldHuoDongStartClearDataMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if (m_netType == GameClient_Game)
		{
			unsigned char temphuodong = m_stream->_readUChar();
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (canvas.valid() && canvas->getDataClass())
			{
				ref_ptr<crData> canvasdata = canvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				canvasdata->getParam(WCHDATA_JXJDeadRank,param);
				std::multimap<int,PlayerData,std::greater<int>> *deadrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				canvasdata->getParam(WCHDATA_JXJKillRank,param);
				std::multimap<int,PlayerData,std::greater<int>> *killrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				canvasdata->getParam(WCHDATA_JXJPointsRank,param);
				std::multimap<int,PlayerData,std::greater<int>> *pointrankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				canvasdata->getParam(WCHDATA_JXJMyWorldInfo,param);
				int *myinfo = (int *)param;
				canvasdata->getParam(WCHDATA_JXJWorldPraisedI,param);
				std::set<int> *preaisedidset = (std::set<int> *)param;
				canvasdata->getParam(WCHDATA_JXJWorldIsCanShow,param);
				bool *iscanshow = (bool *)param;
				canvasdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
				unsigned char *tempaction = (unsigned char *)param;


				deadrankmap->clear();
				killrankmap->clear();
				pointrankmap->clear();
				preaisedidset->clear();
				*iscanshow = true;
				*tempaction = temphuodong;

				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));

				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJWorldRankButtonMethod
//单击战场排行榜按钮
//
//////////////////////////////////////////////////////////////////////////
crJXJWorldRankButtonMethod::crJXJWorldRankButtonMethod()
{}
crJXJWorldRankButtonMethod::crJXJWorldRankButtonMethod(const crJXJWorldRankButtonMethod& handle)
{

}
void crJXJWorldRankButtonMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJWorldRankButtonMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_kaiguan = str;
		break;
	}
}

void crJXJWorldRankButtonMethod::operator()(crHandle &handle)
{
	bool ret = true;
	if (m_this)
	{
		ref_ptr<crCanvasNode>  Canvas = crFilterRenderManager::getInstance()->findCanvas(m_kaiguan);
		if (Canvas.valid())
		{
			if (Canvas->getVisiable())
			{
				ret = false;
			}
			else
			{
				ret = true;
			}
		}
	}
	handle.outputParam(0,&ret);
}

/////////////////////////////////////////
//
//crJXJWorldRankCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJWorldRankCanvasUpdateMethod::crJXJWorldRankCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJWorldRankCanvasUpdateMethod::crJXJWorldRankCanvasUpdateMethod( const crJXJWorldRankCanvasUpdateMethod & handle ):
	crMethod(handle)
{
}

void crJXJWorldRankCanvasUpdateMethod::inputParam( int i, void *param )
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

void crJXJWorldRankCanvasUpdateMethod::addParam( int i, const std::string& str )
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
	case 8:
	case 9:
		m_ranktext[i] = str;
		break;
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
		m_zanbutton[i-10] = str;
		break;
	case 20:
	case 21:
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
		m_imageBox[i-20] = str;
		break;
	case 40:
		m_wopaiming = str;
		break;
	case 41:
		m_woshadi = str;
		break;
	case 42:
		m_wosishang = str;
		break;
	case 43:
		m_wojifen = str;
		break;
	case 44:
		m_showbtn = str;
		break;
	case 45:
		m_sw = str;
		break;
	case 46:
		m_radiogroup = str;
		break;
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
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
		m_itemcount[i-47] = str;
		break;
	case 67:
		m_paihanglist = str;
		break;
	default:
		break;
	}
}

void crJXJWorldRankCanvasUpdateMethod::operator()( crHandle &handle )
{
	if (m_this && m_this->getDataClass())
	{
		int myid = crMyPlayerData::getInstance()->getPlayerGameData()->getPlayerID();
		ref_ptr<crTableIO> rankawardtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldRankAwardTab);
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		std::vector<std::string> record;
		std::vector<std::string> recorditem;
		int type1index = rankawardtab->getTitleIndex("类型1");
		int award1index = rankawardtab->getTitleIndex("奖励1");
		int type2index = rankawardtab->getTitleIndex("类型2");
		int award2index = rankawardtab->getTitleIndex("奖励2");
		int iconindex = itemtab->getTitleIndex("icon");

		void * param;
		ref_ptr<crData> canvasdata = m_this->getDataClass();
		ref_ptr<crStaticTextWidgetNode > myrank = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_wopaiming));
		ref_ptr<crStaticTextWidgetNode > mykill = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_woshadi));
		ref_ptr<crStaticTextWidgetNode > mydead = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_wosishang));
		ref_ptr<crStaticTextWidgetNode > mypoints = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_wojifen));
		ref_ptr<crButtonWidgetNode> showbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_showbtn));
		ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		ref_ptr<crRadioGroupWidgetNode> radiogroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getChildNode(m_radiogroup));
		ref_ptr<crImageBoxWidgetNode > imageBox[20] = {NULL};
		ref_ptr<crButtonWidgetNode> praiseBtn[10] = {NULL};
		ref_ptr<crStaticTextWidgetNode > itemcounttext[20] = {NULL};
		
		if (radiogroup.valid())
		{
			int selec = radiogroup->getSelect();
			selec = 2;

			for (int i = 0; i < 20; i++)
			{
				imageBox[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox[i]));
				if(imageBox[i].valid())
					imageBox[i]->clearImage();

				itemcounttext[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_itemcount[i]));
				if(itemcounttext[i].valid())
					itemcounttext[i]->clearString();

				if(i < 10)
				{
					//ranktext[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_ranktext[i]));
					praiseBtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_zanbutton[i]));

// 					if(ranktext[i].valid())
// 						ranktext[i]->clearString();

					if(praiseBtn[i].valid())
						praiseBtn[i]->setEnable(false);
				}
			}
			
			int textsw = 0;
			std::multimap<int,PlayerData,std::greater<int>> *rankmap = NULL;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			if (selec == 0)
			{
				textsw = 0;
				canvasdata->getParam(WCHDATA_JXJKillRank,param);
				rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
			}
			else if (selec == 1)
			{
				textsw = 1;
				canvasdata->getParam(WCHDATA_JXJDeadRank,param);
				rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
			}
			else if (selec == 2)
			{
				textsw = 2;
				canvasdata->getParam(WCHDATA_JXJPointsRank,param);
				rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
			}
			canvasdata->getParam(WCHDATA_JXJMyWorldInfo,param);
			int *myinfo = (int *)param;
			canvasdata->getParam(WCHDATA_JXJWorldPraisedI,param);
			std::set<int> *preaisedidset = (std::set<int> *)param;
			canvasdata->getParam(WCHDATA_JXJWorldIsCanShow,param);
			bool *iscanshow = (bool *)param;

			if(showbtn.valid())
				showbtn->setEnable(iscanshow);

			if(sw.valid())
				sw->setActiveSwitchSet(textsw);

			if(myrank.valid())
				myrank->setString("我的排行:"+crArgumentParser::appItoa(myinfo[textsw]));

			if(mykill.valid())
				mykill->setString("杀敌数:"+crArgumentParser::appItoa(myinfo[3]));

			if(mydead.valid())
				mydead->setString("死伤数:"+crArgumentParser::appItoa(myinfo[4]));

			if(mypoints.valid())
				mypoints->setString("贡献积分:"+crArgumentParser::appItoa(myinfo[5]));

			int i = 0;
			int j = 0;
			bool isdo = true;
			ref_ptr<crTableWidgetNode > rankList = dynamic_cast<crTableWidgetNode *>(m_this->getChildNode(m_paihanglist));
			if (rankList.valid())rankList->clearData();
			for (std::multimap<int,PlayerData,std::greater<int>>::iterator itr=rankmap->begin(); itr !=rankmap->end() && i<10; itr++,i++)
			{
				if(itr->second.playerid == myid)
				{
				  myinfo[textsw] = i+1;
				  isdo = false;
				}

				//std::string text = crArgumentParser::appItoa(i+1) + "          " + itr->second.name + "     " +crArgumentParser::appItoa(itr->first);

				if(rankList.valid())
				{
					rankList->addData(0,i,crArgumentParser::appItoa(i+1));
					rankList->addData(1,i,itr->second.name);
					rankList->addData(2,i,crArgumentParser::appItoa(itr->first));
					//for(int f = 1;f<10;f++)
					//{
					//	rankList->addData(0,f,crArgumentParser::appItoa(f+1));
					//	rankList->addData(1,f,itr->second.name);
					//	rankList->addData(2,f,crArgumentParser::appItoa(2154655));
					//}
				}
				if(praiseBtn[i].valid())
				{
					std::set<int>::iterator iter = preaisedidset->find(itr->second.playerid);
					if(iter == preaisedidset->end())
						praiseBtn[i]->setEnable(true);
					else
						praiseBtn[i]->setEnable(false);
				}

				if (rankawardtab->queryOneRecord(0,crArgumentParser::appItoa(i+1),record) >= 0)
				{
					
					if (record[type1index] == "10")
					{
						crVector3i vec;
						crArgumentParser::appAtoVec(record[award1index],vec);
						if (vec[0] > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(vec[0]),recorditem) >= 0 && imageBox[j].valid())
						{
							if(imageBox[j].valid())
								imageBox[j]->setImageName(recorditem[iconindex]);
							if(itemcounttext[j].valid())
								itemcounttext[j]->setString(crArgumentParser::appItoa(vec[2]));
						}
					}
					if (record[type2index] == "10")
					{
						crVector3i vec;
						crArgumentParser::appAtoVec(record[award2index],vec);
						if (vec[0] > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(vec[0]),recorditem) >= 0 && imageBox[j+1].valid())
						{
							if(imageBox[j+1].valid())
								imageBox[j+1]->setImageName(recorditem[iconindex]);
							if(itemcounttext[j+1].valid())
								itemcounttext[j+1]->setString(crArgumentParser::appItoa(vec[2]));
						}
					}
				}

				j += 2;
			}

			if (isdo)
			{
				int k = 0;
				std::multimap<int,PlayerData,std::greater<int>>::iterator itr=rankmap->begin();
				for (;itr !=rankmap->end(); itr++,k++)
				{
					if(itr->second.playerid == myid)
					{
						myinfo[textsw] = k+1;
						break;
					}
				}
				if(itr ==rankmap->end())
					myinfo[textsw] = 0;
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJWorldRankDoPraiseMethod
//玩家点赞
//
//////////////////////////////////////////////////////////////////////////
crJXJWorldRankDoPraiseMethod::crJXJWorldRankDoPraiseMethod()
{}
crJXJWorldRankDoPraiseMethod::crJXJWorldRankDoPraiseMethod(const crJXJWorldRankDoPraiseMethod& handle)
{

}
void crJXJWorldRankDoPraiseMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJWorldRankDoPraiseMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radiogroup = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJWorldRankDoPraiseMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>  Canvas = m_this->getParentCanvas();
		if (Canvas.valid() && Canvas->getDataClass())
		{
			void *param;
			ref_ptr<crRadioGroupWidgetNode> radiogroup = dynamic_cast<crRadioGroupWidgetNode *>(Canvas->getChildNode(m_radiogroup));
			
			if (radiogroup.valid())
			{
				int selec = radiogroup->getSelect();
				selec = 2;

				std::multimap<int,PlayerData,std::greater<int>> *rankmap = NULL;
				ref_ptr<crData> canvasdata = Canvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				if (selec == 0)
				{
					canvasdata->getParam(WCHDATA_JXJKillRank,param);
					rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				}
				else if (selec == 1)
				{
					canvasdata->getParam(WCHDATA_JXJDeadRank,param);
					
					rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				}
				else if (selec == 2)
				{
					canvasdata->getParam(WCHDATA_JXJPointsRank,param);
					rankmap = (std::multimap<int,PlayerData,std::greater<int>> *)param;
				}

				int i=0;
				std::multimap<int,PlayerData,std::greater<int>>::iterator itr = rankmap->begin();
				for (; i < m_index && itr != rankmap->end(); i++,itr++)
				{
				}

				if(itr != rankmap->end())
				{
					int playerid =  itr->second.playerid;

					canvasdata->getParam(WCHDATA_JXJWorldPraisedI,param);
					std::set<int> * idset = (std::set<int> *)param;
					if(idset->find(playerid) == idset->end())
					{
						idset->insert(playerid);

						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game); 
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(5);
						stream->_writeUChar(0);
						stream->_writeInt(playerid);
						crPlayerDataSceneEventPacket packet;
						crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJWorldExecutePraise,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);

						ref_ptr<crStreamBuf> gstream = new crStreamBuf;
						gstream->createBuf(1);
						gstream->_writeUChar(0);
						gstream->_writeInt(playerid);
						crPlayerDataEventPacket gpacket;
						crPlayerDataEventPacket::buildRequestPacket(gpacket,WCH_JXJWorldExecutePraise,gstream.get());
						netConductor->getNetManager()->sendPacket("all",gpacket);
					}
				}
				else
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
					return;
				}
				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerShowWorldRankDataMethod
//玩家展示
//
//////////////////////////////////////////////////////////////////////////
crJXJPlayerShowWorldRankDataMethod::crJXJPlayerShowWorldRankDataMethod()
{}
crJXJPlayerShowWorldRankDataMethod::crJXJPlayerShowWorldRankDataMethod(const crJXJPlayerShowWorldRankDataMethod& handle)
{

}
void crJXJPlayerShowWorldRankDataMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJPlayerShowWorldRankDataMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radiogroup = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJPlayerShowWorldRankDataMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>  Canvas = m_this->getParentCanvas();
		if (Canvas.valid() && Canvas->getDataClass())
		{
			void *param;

			std::multimap<int,PlayerData,std::greater<int>> *rankmap = NULL;
			ref_ptr<crData> canvasdata = Canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

			canvasdata->getParam(WCHDATA_JXJMyWorldInfo,param);
			int *myinfo = (int *)param;

			canvasdata->getParam(WCHDATA_JXJWorldIsCanShow,param);
			bool *iscanshow = (bool *)param;
			*iscanshow = false;

			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game); 
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(5);
			stream->_writeUChar(1);
			stream->_writeInt(myinfo[3]);
			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJWorldExecutePraise,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);

			/*ref_ptr<crStreamBuf> gstream = new crStreamBuf;
			gstream->createBuf(1);
			gstream->_writeInt(myinfo[3]);
			gstream->_writeInt(myinfo[4]);
			gstream->_writeInt(myinfo[5]);
			crPlayerDataEventPacket gpacket;
			crPlayerDataEventPacket::buildRequestPacket(gpacket,WCH_JXJWorldExecuteShow,gstream.get());
			netConductor->getNetManager()->sendPacket("all",gpacket);*/
			
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJGameServerDisposeShowMethod
//
/////////////////////////////////////////
crJXJGameServerDisposeShowMethod::crJXJGameServerDisposeShowMethod():
	m_netType(GameClient_Game){}
crJXJGameServerDisposeShowMethod::crJXJGameServerDisposeShowMethod(const crJXJGameServerDisposeShowMethod& handle):
	crMethod(handle)
{
}
void crJXJGameServerDisposeShowMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJGameServerDisposeShowMethod::addParam(int i, const std::string& str)
{
}

void crJXJGameServerDisposeShowMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this)
	{
		void *param;
		if(m_netType == GameServer)
		{
			int kill = m_stream->_readInt();
			int killbudui = m_stream->_readInt();
			
			int playerid = m_this->getPlayerID();
			std::string playername =  m_this->getCharacterName();

			ref_ptr<crData> playerGameData = m_this->getDataClass();
			playerGameData->excHandle(MAKEINT64(WCH_LockData,1));
			playerGameData->getParam(WCHDATA_JXJPlayerPraiseLimit,param);
			unsigned char * limitcount = (unsigned char *)param;
			*limitcount = 0;
			playerGameData->excHandle(MAKEINT64(WCH_LockData,0));

			ref_ptr<crStreamBuf> gstream = new crStreamBuf;
			gstream->createBuf(16+playername.size());
			gstream->_writeInt(kill);
			gstream->_writeInt(killbudui);
			gstream->_writeInt(playerid);
			gstream->_writeString(playername);

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> recvPlayerData;
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crNetManager *netManager = gameServer->getNetManager();
			if(netDataManager && netManager)
			{
				crPlayerDataEventPacket packet;
				netDataManager->lockPlayerDataMap();
				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
					itr != playerDataMap.end();
					++itr )
				{
					recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
					if (recvPlayerData.valid())
					{
						crPlayerDataEventPacket::buildReplyPacket(packet,itr->first,WCH_JXJWorldExecuteShow,gstream.get());
						netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
					}
				}
				netDataManager->unLockPlayerDataMap();
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crData *playerData = m_this->getDataClass();
			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJPlayerPraiseLimit,param);
			unsigned char * limitcount = (unsigned char *)param;
			*limitcount = 0;

			int kill = m_stream->_readInt();
			int killbudui = m_stream->_readInt();
			int playerid = m_stream->_readInt();
			std::string playername =  m_stream->_readString();

			std::string text = "^7^玩家[" + playername + "]在世界战场中杀敌：" + crArgumentParser::appItoa(kill) + "杀死玩家部队：" + crArgumentParser::appItoa(killbudui) + "，请大家为他";
			text = text + "{" + crArgumentParser::appItoa(playerid) + "}";

			ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
			record->setChatChannelName("[国家]");
			record->setChatText(text);
			record->setTextColor(COLOR_GREEN);
		
			crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKEINT64(record.get(),NULL));
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJpraiseLinkClickMethod
//
/////////////////////////////////////////
crJXJpraiseLinkClickMethod::crJXJpraiseLinkClickMethod():
	m_ea(NULL),
	m_this(NULL),
	m_playerid(0),
	m_count(0)
	
{
}
crJXJpraiseLinkClickMethod::crJXJpraiseLinkClickMethod(const crJXJpraiseLinkClickMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_playerid(handle.m_playerid),
	m_count(handle.m_count)
{
}
void crJXJpraiseLinkClickMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
			m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_playerid = *(int*)param;
		break;
	}
}
void crJXJpraiseLinkClickMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJpraiseLinkClickMethod::operator()(crHandle &handle)
{
	if (m_count == 0)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game); 
	
		ref_ptr<crStreamBuf> gstream = new crStreamBuf;
		gstream->createBuf(5);
		gstream->_writeUChar(1);
		gstream->_writeInt(m_playerid);
		crPlayerDataEventPacket gpacket;
		crPlayerDataEventPacket::buildRequestPacket(gpacket,WCH_JXJWorldExecutePraise,gstream.get());
		netConductor->getNetManager()->sendPacket("all",gpacket);

		m_linkInfo->second->setColor(crVector4(0.44f,0.44f,0.44f,1.0f));
		m_count++;
	}
	else
	{
		crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(9215, NULL)); //只能赞一次
	}
}

/////////////////////////////////////////
//
//crJXJDirecGetAwardCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJDirecGetAwardCanvasUpdateMethod::crJXJDirecGetAwardCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJDirecGetAwardCanvasUpdateMethod::crJXJDirecGetAwardCanvasUpdateMethod( const crJXJDirecGetAwardCanvasUpdateMethod & handle ):
	crMethod(handle)
{
}

void crJXJDirecGetAwardCanvasUpdateMethod::inputParam( int i, void *param )
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

void crJXJDirecGetAwardCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_rankcanvas = str;
		break;
	case 1:
		m_jingyan = str;
		break;
	case 2:
		m_liangshi = str;
		break;
	case 3:
		m_mucai = str;
		break;
	case 4:
		m_tiekuang = str;
		break;
	case 5:
		m_mapi = str;
		break;
	case 6:
		m_getsw = str;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_huodongdengji);
		break;
	case 8:
		m_tishi = str;
		break;
	}
}

void crJXJDirecGetAwardCanvasUpdateMethod::operator()( crHandle &handle )
{
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_rankcanvas);

	if (m_this && canvas.valid() && canvas->getDataClass())
	{
		ref_ptr<crPlayerGameData> playgamedata = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!playgamedata.valid() || !playgamedata->getDataClass()) return;

		ref_ptr<crData> data  = playgamedata->getDataClass();
		ref_ptr<crData> canvasdata  = canvas->getDataClass();
		canvasdata->getParam(WCHDATA_JXJCountryPoints,param);
		crVector3i guojiajifen = *(crVector3i *)param;
		canvasdata->getParam(WCHDATA_JXJTempHuoDongId,param);
		unsigned char huodongtype = *(unsigned char *)param;
		canvasdata->getParam(WCHDATA_JXJOpenDiJiTian,param);
		int tianshu = *(int *)param;

		data->getParam(WCHDATA_JXJAmHuoDongGetTimes+huodongtype,param);
		unsigned char gettimes = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char myviplv = *(unsigned char *)param;

		crData *mainroledata = playgamedata->getMainRole()->getDataClass();
		mainroledata->getParam(WCHDATA_Level, param);
		unsigned char mylv = *(unsigned char *)param;

		std::string biaoxian;
		std::string dengji;
		int jifen = guojiajifen[huodongtype];
		if (jifen < m_huodongdengji[0])
		{
			dengji = "notfinished";
		}
		else if (jifen >= m_huodongdengji[0] && jifen < m_huodongdengji[1])
		{
			dengji = "first";
			biaoxian = "完成任务";
		}
		else if (jifen >= m_huodongdengji[1] && jifen < m_huodongdengji[2])
		{
			dengji = "second";
			biaoxian = "较好完成任务";
		}
		else
		{
			dengji = "third";
			biaoxian = "圆满完成任务";
		}

		if (dengji == "notfinished")
		{
			crFilterRenderManager::getInstance()->showCanvas(m_this,false);//活动没有完成
		}
		else
		{
			int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());
			if (gettimes >= alltimes)
			{
				crFilterRenderManager::getInstance()->showCanvas(m_this,false);//领取次数已满
			}
			else
			{
				ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldJiFenAwardJiSuanTab);
				std::vector<std::string> record;
				int dengjiindex = itemtab->getTitleIndex("等级系数");
				int shijieindex = itemtab->getTitleIndex("世界时间系数");
				int yuanbaoindex = itemtab->getTitleIndex("元宝数");
				int lianshiindex = itemtab->getTitleIndex("粮食基数");
				int mucaiindex = itemtab->getTitleIndex("木材基数");
				int ironindex = itemtab->getTitleIndex("铁矿基数");
				int mapiindex = itemtab->getTitleIndex("马匹基数");
				int jingyanindex = itemtab->getTitleIndex("经验基数");

				//int tianshu = 0;
				//crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));

				float foodbasecount = 0.0;
				float woodbasecount = 0.0;
				float ironbasecount = 0.0;
				float horsebasecount = 0.0;
				float expbasecount = 0.0;

				int foodcount = 0;
				int woodcount = 0;
				int ironcount = 0;
				int horsecount = 0;
				int expcount = 0;

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					foodbasecount = atof(record[lianshiindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					woodbasecount = atof(record[mucaiindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					ironbasecount = atof(record[ironindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					horsebasecount = atof(record[mapiindex].c_str());


				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					expbasecount = atof(record[jingyanindex].c_str());

				float timexishu = 0.0;
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(tianshu),record) >= 0)
					timexishu = atof(record[shijieindex].c_str());

				float dengjixishu = 0.0;
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(mylv),record) >= 0)
					dengjixishu = atof(record[dengjiindex].c_str());

				foodcount = (int)(foodbasecount * timexishu * dengjixishu);
				woodcount = (int)(woodbasecount * timexishu * dengjixishu);
				ironcount = (int)(ironbasecount * timexishu * dengjixishu);
				horsecount = (int)(horsebasecount * timexishu * dengjixishu);
				expcount = (int)(expbasecount * timexishu * dengjixishu);

				ref_ptr<crStaticTextWidgetNode > tishi = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_tishi));
				ref_ptr<crStaticTextWidgetNode > experence = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_jingyan));
				ref_ptr<crStaticTextWidgetNode > food = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_liangshi));
				ref_ptr<crStaticTextWidgetNode > wood = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mucai));
				ref_ptr<crStaticTextWidgetNode > iron = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_tiekuang));
				ref_ptr<crStaticTextWidgetNode > horse = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mapi));
				ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_getsw));

				std::string text  = "世界战场活动已经结束，本国活动总积分为" + crArgumentParser::appItoa(jifen) + "点\n" + biaoxian + "，请前往出生点领取活动奖励。";
				if(tishi.valid())
					tishi->setString(text);

				if(experence.valid())
				{
					if(expcount >= 100000)
						experence->setString(crArgumentParser::appItoa(expcount/10000) +"万");
					else
						experence->setString(crArgumentParser::appItoa(expcount));
				}
				if(food.valid())
				{
					if(foodcount >= 100000)
						food->setString(crArgumentParser::appItoa(foodcount/10000) +"万");
					else
						food->setString(crArgumentParser::appItoa(foodcount));
				}
				if(wood.valid())
				{
					if(woodcount >= 100000)
						wood->setString(crArgumentParser::appItoa(woodcount/10000) +"万");
					else
						wood->setString(crArgumentParser::appItoa(woodcount));
				}
				if(iron.valid())
				{
					if(ironcount >= 100000)
						iron->setString(crArgumentParser::appItoa(ironcount/10000) +"万");
					else
						iron->setString(crArgumentParser::appItoa(ironcount));
				}
				if(horse.valid())
				{
					if(horsecount >= 100000)
						horse->setString(crArgumentParser::appItoa(horsecount/10000) +"万");
					else
						horse->setString(crArgumentParser::appItoa(horsecount));
				}

				if(gettimes == 0)
					sw->setActiveSwitchSet(0);
				else
					sw->setActiveSwitchSet(1);
					
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGetAwardCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJGetAwardCanvasUpdateMethod::crJXJGetAwardCanvasUpdateMethod():
	m_this(NULL)
{

}

crJXJGetAwardCanvasUpdateMethod::crJXJGetAwardCanvasUpdateMethod( const crJXJGetAwardCanvasUpdateMethod & handle ):
	crMethod(handle)
{
}

void crJXJGetAwardCanvasUpdateMethod::inputParam( int i, void *param )
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

void crJXJGetAwardCanvasUpdateMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
		m_jingyan[i] = str;
		break;
	case 3:
	case 4:
	case 5:
		m_liangshi[i-3] = str;
		break;
	case 6:
	case 7:
	case 8:
		m_mucai[i-6] = str;
		break;
	case 9:
	case 10:
	case 11:
		m_tiekuang[i-9] = str;
		break;
	case 12:
	case 13:
	case 14:
		m_mapi[i-12] = str;
		break;
	case 15:
	case 16:
	case 17:
		m_getbtn[i-15] = str;
		break;
	case 18:
		crArgumentParser::appAtoVec(str,m_huodongdengji);
		break;
	case 19:
		m_rankcanvas = str;
		break;
	case 20:
	case 21:
	case 22:
		m_totaljifen[i-20] = str;
		break;
	case 23:
	case 24:
	case 25:
		m_dangci[i-23] = str;
		break;
	}
}

void crJXJGetAwardCanvasUpdateMethod::operator()( crHandle &handle )
{
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_rankcanvas);

	if (m_this && canvas.valid() && canvas->getDataClass())
	{
		ref_ptr<crPlayerGameData> playgamedata = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!playgamedata.valid() || !playgamedata->getDataClass()) return;

		ref_ptr<crData> data  = playgamedata->getDataClass();
		ref_ptr<crData> canvasdata  = canvas->getDataClass();
		canvasdata->getParam(WCHDATA_JXJCountryPoints,param);
		crVector3i guojiajifen = *(crVector3i *)param;
		/*canvasdata->getParam(WCHDATA_JXJTempHuoDongId,param);
		unsigned char huodongtype = *(unsigned char *)param;*/
		canvasdata->getParam(WCHDATA_JXJOpenDiJiTian,param);
		int tianshu = *(int *)param;

		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char myviplv = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJGoldingot,param);
		int  yuanbao = *(int *)param;
		data->getParam(WCHDATA_JXJGiftGold,param);
		int lijin = *(int *)param;

		crData *mainroledata = playgamedata->getMainRole()->getDataClass();
		mainroledata->getParam(WCHDATA_Level, param);
		unsigned char mylv = *(unsigned char *)param;

		ref_ptr<crStaticTextWidgetNode > totaljifen = NULL;
		ref_ptr<crMultiSwitch > dangci = NULL;

		m_huodongdengji[0] = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,0).c_str());
		m_huodongdengji[1] = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,1).c_str());
		m_huodongdengji[2] = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldHuoDongLv,2).c_str());

		for (int i = 0; i < 3; i++)
		{
			totaljifen = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_totaljifen[i]));
			dangci = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_dangci[i]));
			if(totaljifen.valid())
				totaljifen->setString(crArgumentParser::appItoa(guojiajifen[i]));

			ref_ptr<crButtonWidgetNode> getbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getChildNode(m_getbtn[i]));
			data->getParam(WCHDATA_JXJAmHuoDongGetTimes+i,param);
			unsigned char gettimes = *(unsigned char *)param;

			int dengjii = 0;
			std::string dengji;
			int jifen = guojiajifen[i];
			if (jifen < m_huodongdengji[0])
				dengji = "notfinished";
			else if (jifen >= m_huodongdengji[0] && jifen < m_huodongdengji[1])
			{
				dengji = "first";
				dengjii = 0;
			}
			else if (jifen >= m_huodongdengji[1] && jifen < m_huodongdengji[2])
			{
				dengji = "second";
				dengjii = 1;
			}
			else
			{
				dengji = "third";
				dengjii = 2;
			}

			if (dengji == "notfinished")
			{
				if(getbtn.valid())
					getbtn->setEnable(false);//活动没有完成

				if(dangci.valid())
					dangci->setVisiable(false);
			}
			else
			{
				if(dangci.valid())
				{
					dangci->setVisiable(true);
					dangci->setActiveSwitchSet(dengjii);
				}

				ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldJiFenAwardJiSuanTab);
				std::vector<std::string> record;
				int dengjiindex = itemtab->getTitleIndex("等级系数");
				int shijieindex = itemtab->getTitleIndex("世界时间系数");
				int yuanbaoindex = itemtab->getTitleIndex("元宝数");
				int lianshiindex = itemtab->getTitleIndex("粮食基数");
				int mucaiindex = itemtab->getTitleIndex("木材基数");
				int ironindex = itemtab->getTitleIndex("铁矿基数");
				int mapiindex = itemtab->getTitleIndex("马匹基数");
				int jingyanindex = itemtab->getTitleIndex("经验基数");

				
				
				//int tianshu = 0;
				//crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));

				float foodbasecount = 0.0;
				float woodbasecount = 0.0;
				float ironbasecount = 0.0;
				float horsebasecount = 0.0;
				float expbasecount = 0.0;

				int foodcount = 0;
				int woodcount = 0;
				int ironcount = 0;
				int horsecount = 0;
				int expcount = 0;

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					foodbasecount = atof(record[lianshiindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					woodbasecount = atof(record[mucaiindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					ironbasecount = atof(record[ironindex].c_str());

				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					horsebasecount = atof(record[mapiindex].c_str());


				if (itemtab->queryOneRecord(0,dengji,record) >= 0)
					expbasecount = atof(record[jingyanindex].c_str());

				float timexishu = 0.0;
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(tianshu),record) >= 0)
					timexishu = atof(record[shijieindex].c_str());

				float dengjixishu = 0.0;
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(mylv),record) >= 0)
					dengjixishu = atof(record[dengjiindex].c_str());


				foodcount = (int)(foodbasecount * timexishu * dengjixishu);
				woodcount = (int)(woodbasecount * timexishu * dengjixishu);
				ironcount = (int)(ironbasecount * timexishu * dengjixishu);
				horsecount = (int)(horsebasecount * timexishu * dengjixishu);
				expcount = (int)(expbasecount * timexishu * dengjixishu);

				ref_ptr<crStaticTextWidgetNode > experence = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_jingyan[i]));
				ref_ptr<crStaticTextWidgetNode > food = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_liangshi[i]));
				ref_ptr<crStaticTextWidgetNode > wood = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mucai[i]));
				ref_ptr<crStaticTextWidgetNode > iron = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_tiekuang[i]));
				ref_ptr<crStaticTextWidgetNode > horse = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_mapi[i]));

				if(experence.valid())
				{
					if(expcount >= 100000)
						experence->setString(crArgumentParser::appItoa(expcount/10000) +"万");
					else
						experence->setString(crArgumentParser::appItoa(expcount));
				}
				if(food.valid())
				{
					if(foodcount >= 100000)
						food->setString(crArgumentParser::appItoa(foodcount/10000) +"万");
					else
						food->setString(crArgumentParser::appItoa(foodcount));
				}
				if(wood.valid())
				{
					if(woodcount >= 100000)
						wood->setString(crArgumentParser::appItoa(woodcount/10000) +"万");
					else
						wood->setString(crArgumentParser::appItoa(woodcount));
				}
				if(iron.valid())
				{
					if(ironcount >= 100000)
						iron->setString(crArgumentParser::appItoa(ironcount/10000) +"万");
					else
						iron->setString(crArgumentParser::appItoa(ironcount));
				}
				if(horse.valid())
				{
					if(horsecount >= 100000)
						horse->setString(crArgumentParser::appItoa(horsecount/10000) +"万");
					else
						horse->setString(crArgumentParser::appItoa(horsecount));
				}

				if(getbtn.valid())
				{
					int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());
					if (gettimes >= alltimes)
						getbtn->setEnable(false);//领取次数已满
					else
					{
						int needgold = 0;
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(gettimes+1),record) >= 0)
							needgold = atoi(record[yuanbaoindex].c_str());

						if (needgold > yuanbao+lijin)
							getbtn->setEnable(false);//元宝不足
						else
							getbtn->setEnable(true);
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJWorldFuBenStatusUpdataMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJWorldFuBenStatusUpdataMethod::crJXJWorldFuBenStatusUpdataMethod():
	m_this(NULL)
{

}
crJXJWorldFuBenStatusUpdataMethod::crJXJWorldFuBenStatusUpdataMethod(const crJXJWorldFuBenStatusUpdataMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJWorldFuBenStatusUpdataMethod::inputParam(int i, void *param)
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
void crJXJWorldFuBenStatusUpdataMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rankcanvas = str;
		break;
	case 1:
		m_jifen = str;
		break;
	case 2:
		m_status = str;
		break;
	default:
		break;
	}
}
void crJXJWorldFuBenStatusUpdataMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_rankcanvas);
	if (m_this && m_this->getDataClass() && canvas.valid() && canvas->getDataClass())
	{
		ref_ptr<crData> rankdata = canvas->getDataClass();
		ref_ptr<crData> statusdata = m_this->getDataClass();
		ref_ptr<crStaticTextWidgetNode> jifen = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_jifen));
		ref_ptr<crStaticTextWidgetNode> eventtime = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_status));

		rankdata->getParam(WCHDATA_JXJTempCountryPoints,param);
		int guojiajifen = *(int *)param;
		rankdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
		unsigned char huodongtype = *(unsigned char *)param;
		if (huodongtype == 3)
		{
			guojiajifen = 0;
		}
		statusdata->excHandle(MAKEINT64(WCH_LockData,1));
		statusdata->getParam(WCHDATA_JXJWorldEventRemainTime,param);
		int *time = (int *)param;
		statusdata->getParam(WCHDATA_JXJWorldEvent,param);
	    std::string nextevent = *(std::string *)param;

		if (*time > 0)
		{
			--(*time);
		}
		else
		{
			*time = 0;
		}

		int hour = (*time)/3600;
		int minu = (*time)%3600/60;
		int sec  = (*time)%3600%60;
		std::string ramaintime = crArgumentParser::appItoa(hour)+':'+crArgumentParser::appItoa(minu)+':'+crArgumentParser::appItoa(sec);
		if(jifen.valid())
			jifen->setString("活动总积分   "+crArgumentParser::appItoa(guojiajifen));

		if(eventtime.valid())
			eventtime->setString(nextevent+"   "+ramaintime);

		statusdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}


///////////////////////////////////////////////////////////////////////////
//
//crJXJWorldEventTimeCorrectMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJWorldEventTimeCorrectMethod::crJXJWorldEventTimeCorrectMethod():
	m_this(NULL)
{

}
crJXJWorldEventTimeCorrectMethod::crJXJWorldEventTimeCorrectMethod(const crJXJWorldEventTimeCorrectMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJWorldEventTimeCorrectMethod::inputParam(int i, void *param)
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
void crJXJWorldEventTimeCorrectMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJWorldEventTimeCorrectMethod::operator ()(crHandle &handle)
{
	void *param;
	if (m_this && m_this->getDataClass())
	{
		time_t t = time(0);
		struct  tm  sTm = *localtime(&t);

		ref_ptr<crData> statusdata = m_this->getDataClass();
		statusdata->excHandle(MAKEINT64(WCH_LockData,1));
		statusdata->getParam(WCHDATA_JXJWorldEventTime,param);
		int occortime = *(int *)param;
		statusdata->getParam(WCHDATA_JXJWorldEventRemainTime,param);
		int *remaintime = (int *)param;
		
		//*remaintime = occortime - (sTm.tm_hour*3600+sTm.tm_min*60+sTm.tm_sec);
		if(occortime != 37800)
			*remaintime = occortime - (sTm.tm_hour*3600+sTm.tm_min*60+sTm.tm_sec);
		else
		{
			int nowtime = sTm.tm_hour*3600+sTm.tm_min*60+sTm.tm_sec;
			if(nowtime > 37800)
				*remaintime = 124200 - nowtime;
			else
				*remaintime = nowtime;
		}

		statusdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJWorldEventRemainTimeQueryMethod
//
/////////////////////////////////////////
crJXJWorldEventRemainTimeQueryMethod::crJXJWorldEventRemainTimeQueryMethod()
{
}
crJXJWorldEventRemainTimeQueryMethod::crJXJWorldEventRemainTimeQueryMethod(const crJXJWorldEventRemainTimeQueryMethod& handle):
	crMethod(handle)
{
}
void crJXJWorldEventRemainTimeQueryMethod::inputParam(int i, void *param)
{
}
void crJXJWorldEventRemainTimeQueryMethod::addParam(int i, const std::string& str)
{
}
void crJXJWorldEventRemainTimeQueryMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getDataClass())
		{
			/*void *param;
			crData *data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myshili = *(unsigned char *)param;

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(myshili);*/

			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJWorldEventRemainTime,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

/////////////////////////////////////////
//
//crJXJWorldEventRemainTimeSendMethod
//
/////////////////////////////////////////
crJXJWorldEventRemainTimeSendMethod::crJXJWorldEventRemainTimeSendMethod():
	m_netType(GameClient_Game){}
crJXJWorldEventRemainTimeSendMethod::crJXJWorldEventRemainTimeSendMethod(const crJXJWorldEventRemainTimeSendMethod& handle):
	crMethod(handle)
{
}
void crJXJWorldEventRemainTimeSendMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJWorldEventRemainTimeSendMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJWorldEventRemainTimeSendMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if (m_netType == SceneServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer); 
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				//crNetDataManager *netDataManager = sceneServer->getNetDataManager();
				//crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				//CRNetApp::crRoom* room = callback->findRoom(playerData->getRoomID());
				//if(room && m_this->getDataClass())
				//{
				//	int time;
				//	int eventtype;
				//	void *param;
				//	crData *data = room->getDataClass();
				//	data->excHandle(MAKEINT64(WCH_LockData,1));
				//	//data->getParam(WCHDATA_JXJWorldTemHuoDong,param);
				//	//unsigned char temaction = *(unsigned char *)param;
				//	data->getParam(WCHDATA_JXJScenarioVariableMap,param);
				//	ScenarioVariableMap *variableMap = (ScenarioVariableMap *)param;
				//	ScenarioVariableMap::iterator svmitr = variableMap->find("daojishi");
				//	if (svmitr != variableMap->end())
				//	{
				//		time = (int)svmitr->second;
				//	}
				//	svmitr = variableMap->find("timenote");
				//	if (svmitr != variableMap->end())
				//	{
				//		eventtype = (int)svmitr->second;
				//	}
				//	data->excHandle(MAKEINT64(WCH_LockData,0));

				time_t t = time(0);
				struct  tm  sTm = *localtime(&t);
				int hour = sTm.tm_hour;
				int minu = sTm.tm_min;
				int sec = sTm.tm_sec;

				crNetManager *netManager = sceneServer->getNetManager();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(13);
				stream->_writeUChar(0);
				stream->_writeInt(hour);
				stream->_writeInt(minu);
				stream->_writeInt(sec);

				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildReplyPacket(packet,playerid,WCH_JXJWorldEventRemainTime,stream.get());

				netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			//}
		}
		else if (m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode> statuscan = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if (statuscan.valid() && statuscan->getDataClass())
			{
				unsigned char type = m_stream->_readUChar();
				if (type == 1)
				{
				
					//unsigned char tempaction = m_stream->_readUChar();
					//int time = m_stream->_readInt();
					int eventtype = m_stream->_readInt();
					eventtype = eventtype>21?0:eventtype;
					int time2 = m_stream->_readInt();
					int tm_hour = m_stream->_readInt();
					int tm_min = m_stream->_readInt();
					int tm_sec = m_stream->_readInt();

					std::vector<std::string> record;
					std::string eventstr;
					ref_ptr<crTableIO> codtexttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
					if(codtexttab.valid() && codtexttab->queryOneRecord(0,crArgumentParser::appItoa(10000+eventtype),record)>=0)
					{
						std::vector<std::string> events;
						crArgumentParser::appAtoVec(record[1],events);
						eventstr = events[0];//record[1];
					}


					ref_ptr<crData> canvasdata = statuscan->getDataClass();
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				
					/*canvasdata->getParam(WCHDATA_JXJWorldTemHuoDong,param);
					unsigned char *huodong = (unsigned char *)param;
					*huodong = tempaction>2?0:tempaction;*/
					canvasdata->getParam(WCHDATA_JXJWorldEventRemainTime,param);
					int *remaintime = (int *)param;
					canvasdata->getParam(WCHDATA_JXJWorldEventTime,param);
					int *oldtime = (int *)param;
					*oldtime = time2;
					canvasdata->getParam(WCHDATA_JXJWorldEvent,param);
					std::string *nextevent = (std::string *)param;
					*nextevent = eventstr;

					//time_t t = time(0);
					//struct  tm  sTm = *localtime(&t);
					if(time2 != 37800)
						*remaintime = time2 - (tm_hour*3600+tm_min*60+tm_sec);
					else
					{
						int nowtime = tm_hour*3600+tm_min*60+tm_sec;
						if(nowtime > 37800)
							*remaintime = 124200 - nowtime;
						else
							*remaintime = time2 - nowtime;
					}

					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
				else if(type == 0)
				{
					int tm_hour = m_stream->_readInt();
					int tm_min = m_stream->_readInt();
					int tm_sec = m_stream->_readInt();

					ref_ptr<crData> canvasdata = statuscan->getDataClass();
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata->getParam(WCHDATA_JXJWorldEventTime,param);
					int eventtime = *(int *)param;
					canvasdata->getParam(WCHDATA_JXJWorldEventRemainTime,param);
					int *remaintime = (int *)param;

					if(eventtime != 37800)
						*remaintime = eventtime - (tm_hour*3600+tm_min*60+tm_sec);
					else
					{
						int nowtime = tm_hour*3600+tm_min*60+tm_sec;
						if(nowtime > 37800)
							*remaintime = 124200 - nowtime;
						else
							*remaintime = eventtime - nowtime;
					}

					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJResetWorldCanvasPositionMethod
//
/////////////////////////////////////////
crJXJResetWorldCanvasPositionMethod::crJXJResetWorldCanvasPositionMethod():
	m_this(NULL),
	m_index(0)
{

}

crJXJResetWorldCanvasPositionMethod::crJXJResetWorldCanvasPositionMethod( const crJXJResetWorldCanvasPositionMethod & handle ):
	crMethod(handle),
	m_index(handle.m_index)
{
}

void crJXJResetWorldCanvasPositionMethod::inputParam( int i, void *param )
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

void crJXJResetWorldCanvasPositionMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_baseCanvas = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJResetWorldCanvasPositionMethod::operator()( crHandle &handle )
{
	void *param;
	for(int i = 0;i < 3;i++)
	{
		ref_ptr<crPlayerGameData> playergamedata = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crData> data = playergamedata->getDataClass();
		data->getParam(WCHDATA_JXJAmHuoDongGetTimes+i,param);
		unsigned char gettimes = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char myviplv = *(unsigned char *)param;
		int alltimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGetTimesAccordToVip,myviplv).c_str());
		
		ref_ptr<crMultiSwitch> switchnode1,switchnode2,switchnode3;
		switch (i)
		{
		case 0:
			switchnode1 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("Linquiry_Sw1"));
			if(switchnode1.valid())
			{
				if(gettimes>=1&&gettimes < alltimes)
				{
					switchnode1->setActiveSwitchSet(1);
				}
				else if (gettimes == alltimes)
				{
					switchnode1->setActiveSwitchSet(2);
				}
				else
				{
					switchnode1->setActiveSwitchSet(0);
				}
			}
			break;
		case 1:
			switchnode2 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("Linquiry_Sw2"));
			if(switchnode2.valid())
			{
				if(gettimes>=1&&gettimes < alltimes)
				{
					switchnode2->setActiveSwitchSet(1);
				}
				else if (gettimes == alltimes)
				{
					switchnode2->setActiveSwitchSet(2);
				}
				else
				{
					switchnode2->setActiveSwitchSet(0);
				}
			}
			break;
		case 2:
			switchnode3 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("Linquiry_Sw3"));
			if(switchnode3.valid())
			{
				if(gettimes>=1&&gettimes < alltimes)
				{
					switchnode3->setActiveSwitchSet(1);
				}
				else if (gettimes == alltimes)
				{
					switchnode3->setActiveSwitchSet(2);
				}
				else
				{
					switchnode3->setActiveSwitchSet(0);
				}
			}
			break;
		default:
			break;
		}

	}
	

	ref_ptr<crCanvasNode> basecanvas = crFilterRenderManager::getInstance()->findCanvas(m_baseCanvas);

	if (m_this && basecanvas.valid())
	{
		const crBoundingBox& disposed_box = m_this->getBoundBox();
		const crBoundingBox& basebox = basecanvas->getBoundBox();
		if(m_index == 0)
			m_this->setMatrix(crMatrix::translate(basebox.center().x()+basebox.xLength()*0.2f,basebox.yMax()+disposed_box.yLength()*0.5f-basebox.yLength()*0.29,0.0f));
		else
			m_this->setMatrix(crMatrix::translate(basebox.center().x(),basebox.yMax()+disposed_box.yLength()*0.5f+basebox.yLength()*0.1f,0.0f));
	}
}


/////////////////////////////////////////
//
//crJXJRecvGuoZhanAchieveinfoMethod
//
/////////////////////////////////////////
crJXJRecvGuoZhanAchieveinfoMethod::crJXJRecvGuoZhanAchieveinfoMethod():
	m_netType(GameClient_Game){}
crJXJRecvGuoZhanAchieveinfoMethod::crJXJRecvGuoZhanAchieveinfoMethod(const crJXJRecvGuoZhanAchieveinfoMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGuoZhanAchieveinfoMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGuoZhanAchieveinfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_killdoor);
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_killtower);
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_killzhengti);
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_killboss);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_danchangjisha[i-4] = atoi(str.c_str());
		break;
	}
}

void crJXJRecvGuoZhanAchieveinfoMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this)
	{
		void *param;
		int playerid = m_this->getPlayerID();
		//ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		//int beizhuindex = accomplish_table->getTitleIndex("备注");
		//std::vector<std::string> achieverecord;
		if (m_netType == GameServer)
		{
			int troopsid = 0;
			CRCore::crVector3i oldtroopsinfo;
			CRCore::crVector3i newtroopsinfo;
			CRCore::crVector4i noteinfo;
			std::vector<std::string> achieveid;
			std::vector<int> needsetcomplete;
			std::map<int,CRCore::crVector3i> troopsinfomap;
			int playernpcdead[3] = {0};
			int doorboss[4] = {0};
			int cbingxi[8] = {0};

			playernpcdead[0] = m_stream->_readInt();
			playernpcdead[1] = m_stream->_readInt();
			playernpcdead[2] = m_stream->_readInt();
			doorboss[0] = m_stream->_readInt();
			doorboss[1] = m_stream->_readInt();
			doorboss[2] = m_stream->_readInt();
			doorboss[3] = m_stream->_readInt();
			cbingxi[0] = m_stream->_readInt();
			cbingxi[1] = m_stream->_readInt();
			cbingxi[2] = m_stream->_readInt();
			cbingxi[3] = m_stream->_readInt();
			unsigned short size = m_stream->_readUShort();
			for (unsigned short i=0; i<size; i++)
				troopsinfomap.insert(std::make_pair(m_stream->_readInt(),m_stream->_readVec3i()));

			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_this->getDataClass())
			{
				ref_ptr<crData> mainRoleData = m_this->getMainRole()->getDataClass();
				ref_ptr<crData> playerData = m_this->getDataClass();
				playerData->excHandle(MAKEINT64(WCH_LockData,1));

				//for (int j=0; j<3; j++)
				//{
				//	playerData->getParam(WCHDATA_JXJKillPlayerCount+j,param);
				//	int * count = (int *)param;
				//	*count = *count + playernpcdead[j];
				//}
				playerData->getParam(WCHDATA_JXJKillPlayerCount,param);
				int * count = (int *)param;
				*count = *count + playernpcdead[0];
				playerData->getParam(WCHDATA_JXJDayKillPlayerCount,param);
				count = (int *)param;
				*count = *count + playernpcdead[0];
				playerData->getParam(WCHDATA_JXJKillNpcCount,param);
				count = (int *)param;
				*count = *count + playernpcdead[1];
				playerData->getParam(WCHDATA_JXJDayKillNPCCount,param);
				count = (int *)param;
				*count = *count + playernpcdead[1];
				playerData->getParam(WCHDATA_JXJDayPlayerDeadCount,param);
				count = (int *)param;
				*count = *count + playernpcdead[2];
				playerData->getParam(WCHDATA_JXJPlayerDeadCount, param);
				count = (int *)param;
				*count = *count + playernpcdead[2];

                m_this->doEvent(WCH_JXJCheck7Day);
				playerData->getParam(WCHDATA_JXJTroopsAchieveInfo,param);
				std::map<int,CRCore::crVector3i> * bingzhonginfomap = (std::map<int,CRCore::crVector3i> *)param;
				std::map<int,CRCore::crVector3i>::iterator bingzhongitr;

				std::map<int,CRCore::crVector3i>::iterator troopitr = troopsinfomap.begin();
				for (; troopitr != troopsinfomap.end(); troopitr++)
				{
					bingzhongitr = bingzhonginfomap->find(troopitr->first);
					if (bingzhongitr != bingzhonginfomap->end())
					{
						bingzhongitr->second[0] += troopitr->second[0];
						bingzhongitr->second[1] += troopitr->second[1];
						bingzhongitr->second[2] += troopitr->second[2];
					}
					else
					{
						bingzhonginfomap->insert(std::make_pair(troopitr->first,troopitr->second));
					}

					if ((troopitr->first >= 20001 && troopitr->first <= 20006) || (troopitr->first >= 20505 && troopitr->first <= 20507) || troopitr->first == 20023)
						cbingxi[4] += troopitr->second[1];

					if ((troopitr->first >= 20007 && troopitr->first <= 20012) || (troopitr->first >= 20500 && troopitr->first <= 20504))
						cbingxi[5] += troopitr->second[1];

					if ((troopitr->first >= 20013 && troopitr->first <= 20018) || (troopitr->first >= 20508 && troopitr->first <= 20510))
						cbingxi[6] += troopitr->second[1];

					if ((troopitr->first >= 20019 && troopitr->first <= 20021) || (troopitr->first >= 20505 && troopitr->first <= 20507))
						cbingxi[7] += troopitr->second[1];
				}

				playerData->getParam(WCHDATA_JXJKillEachTroopsCount,param);
				crVector4i * ckilleachtroops = (crVector4i *)param;
				playerData->getParam(WCHDATA_JXJDayKillEachTroops,param);
				crVector4i * cdaykilleachtroops = (crVector4i *)param;
				playerData->getParam(WCHDATA_JXJDeadEachTroopsCount,param);
				crVector4i * cdeadeachtroops = (crVector4i *)param;
				
				for (int j=0; j<8; j++)
				{
					if(j>=0 && j<4)
					{
						(*ckilleachtroops)[j] = (*ckilleachtroops)[j] + cbingxi[j];
						(*cdaykilleachtroops)[j] = (*cdaykilleachtroops)[j] + cbingxi[j];
					}
					else if(j>=4 && j<8)
					{
						
						(*cdeadeachtroops)[j-4] = (*cdeadeachtroops)[j-4] + cbingxi[j];
					}
				}

				playerData->getParam(WCHDATA_JXJDanDhangKillCount,param);
				crVector4i * danchangkill = (crVector4i *)param;
				playerData->getParam(WCHDATA_JXJKillDoorBossCount,param);
				crVector4s * killdoorboss = (crVector4s *)param;
				for (int i=0; i<4; i++)
				{
					(*danchangkill)[i] = cbingxi[i];
					(*killdoorboss)[i] += (short)doorboss[i];
				}
				playerData->excHandle(MAKEINT64(WCH_LockData,0));

				playerData->getParam(WCHDATA_JXJKillPlayerCount,param);
				int KillPlayerCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJKillNpcCount,param);
				int KillnpcCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJDayPlayerDeadCount,param);
				int daydeadCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJPlayerDeadCount, param);
				int deadCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJTroopsAchieveInfo,param);
				std::map<int,CRCore::crVector3i> bingzhongmap = *(std::map<int,CRCore::crVector3i> *)param;
				playerData->getParam(WCHDATA_JXJKillEachTroopsCount,param);
				crVector4i  ckilltroops = *(crVector4i *)param;
				playerData->getParam(WCHDATA_JXJDeadEachTroopsCount,param);
				crVector4i  cdeadtroops = *(crVector4i *)param;
				playerData->getParam(WCHDATA_JXJDanDhangKillCount,param);
				crVector4i cdanchangkill = *(crVector4i *)param;
				playerData->getParam(WCHDATA_JXJKillDoorBossCount,param);
				crVector4s ckilldoorboss = *(crVector4s *)param;

				playerData->getParam(WCHDATA_JXJDayKillPlayerCount,param);
				int dayKillPlayerCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJDayKillNPCCount,param);
				int dayKillnpcCount = *(int *)param;
				playerData->getParam(WCHDATA_JXJDayKillEachTroops,param);
				crVector4i cdaykilltroops = *(crVector4i *)param;

				crNetConductor *gServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crGameServerPlayerData> gplayerData = dynamic_cast<crGameServerPlayerData *>(gServer->getNetDataManager()->getPlayerData(playerid));
				if(gplayerData.valid())
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(105+troopsinfomap.size()*16);
					stream->_writeInt(KillPlayerCount);
					stream->_writeInt(KillnpcCount);
					stream->_writeInt(daydeadCount);
					stream->_writeInt(deadCount);
					stream->_writeVec4i(ckilltroops);
					stream->_writeVec4i(cdeadtroops);
					stream->_writeVec4i(cdanchangkill);
					stream->_writeVec4s(ckilldoorboss);
					stream->_writeUChar(troopsinfomap.size());
					if (troopsinfomap.size() > 0)
					{
						for (std::map<int,CRCore::crVector3i>::iterator itr=troopsinfomap.begin(); itr!=troopsinfomap.end(); itr++)
						{
							stream->_writeInt(itr->first);
							stream->_writeVec3i(itr->second);
						}
					}
					stream->_writeInt(dayKillPlayerCount);
					stream->_writeInt(dayKillnpcCount);
					stream->_writeVec4i(cdaykilltroops);

					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJGuoZhanAchievement,stream.get());
					gServer->getNetManager()->sendPacket(gplayerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_this->getDataClass())
			{
				ref_ptr<crData> playerData = m_this->getDataClass();
				playerData->excHandle(MAKEINT64(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJKillPlayerCount,param);
				int * KillPlayerCount = (int *)param;
				*KillPlayerCount = m_stream->_readInt();

				playerData->getParam(WCHDATA_JXJKillNpcCount,param);
				int *KillnpcCount = (int *)param;
				*KillnpcCount = m_stream->_readInt();

				playerData->getParam(WCHDATA_JXJDayPlayerDeadCount,param);
				int *daydeadCount = (int *)param;
				*daydeadCount = m_stream->_readInt();
				playerData->getParam(WCHDATA_JXJPlayerDeadCount, param);
				int *deadCount = (int *)param;
				*deadCount = m_stream->_readInt();

				playerData->getParam(WCHDATA_JXJKillEachTroopsCount,param);
				crVector4i  *ckilltroops = (crVector4i *)param;
				*ckilltroops = m_stream->_readVec4i();

				playerData->getParam(WCHDATA_JXJDeadEachTroopsCount,param);
				crVector4i  *cdeadtroops = (crVector4i *)param;
				*cdeadtroops = m_stream->_readVec4i();

				playerData->getParam(WCHDATA_JXJDanDhangKillCount,param);
				crVector4i  *cdanchangkill = (crVector4i *)param;
				*cdanchangkill = m_stream->_readVec4i();

				playerData->getParam(WCHDATA_JXJKillDoorBossCount,param);
				crVector4s  *ckilldoorboss = (crVector4s *)param;
				*ckilldoorboss = m_stream->_readVec4s();

				playerData->getParam(WCHDATA_JXJTroopsAchieveInfo,param);
				std::map<int,CRCore::crVector3i> * bingzhonginfomap = (std::map<int,CRCore::crVector3i> *)param;
				std::map<int,CRCore::crVector3i>::iterator bingzhongitr;
				crVector3i troopinfo;
				int troopid = 0;
				int size = m_stream->_readUChar();
				for (int i=0; i<size; i++)
				{
					troopid = m_stream->_readInt();
					troopinfo = m_stream->_readVec3i();
					bingzhongitr = bingzhonginfomap->find(troopid);
					if (bingzhongitr != bingzhonginfomap->end())
					{
						bingzhongitr->second[0] += troopinfo[0];
						bingzhongitr->second[1] += troopinfo[1];
						bingzhongitr->second[2] += troopinfo[2];
					}
					else
					{
						bingzhonginfomap->insert(std::make_pair(troopid,troopinfo));
					}
				}
				int daykillplayer = m_stream->_readInt();
				int daykillnpc = m_stream->_readInt();
				crVector4i daykilltroops = m_stream->_readVec4i();
				playerData->inputParam(WCHDATA_JXJDayKillPlayerCount,&daykillplayer);
				playerData->inputParam(WCHDATA_JXJDayKillNPCCount,&daykillnpc);
				playerData->inputParam(WCHDATA_JXJDayKillEachTroops,&daykilltroops);
				playerData->excHandle(MAKEINT64(WCH_LockData,0));
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJDetroyOppositeCountryMethod
//
/////////////////////////////////////////
crJXJDetroyOppositeCountryMethod::crJXJDetroyOppositeCountryMethod():
	m_this(NULL),
	m_achieveid1(0),
	m_achieveid2(0),
	m_achieveid3(0),
	m_achieveid4(0),
	m_achieveid5(0)
{
}
crJXJDetroyOppositeCountryMethod::crJXJDetroyOppositeCountryMethod(const crJXJDetroyOppositeCountryMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_achieveid1(handle.m_achieveid1),
	m_achieveid2(handle.m_achieveid2),
	m_achieveid3(handle.m_achieveid3),
	m_achieveid4(handle.m_achieveid4),
	m_achieveid5(handle.m_achieveid5)
{
}
void crJXJDetroyOppositeCountryMethod::inputParam(int i, void *param)
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
void crJXJDetroyOppositeCountryMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_achieveid1 = atoi(str.c_str());
		break;
	case 1:
		m_achieveid2 = atoi(str.c_str());
		break;
	case 2:
		m_achieveid3 = atoi(str.c_str());
		break;
	case 3:
		m_achieveid4 = atoi(str.c_str());
		break;
	case 4:
		m_achieveid5 = atoi(str.c_str());
		break;
	}
}
void crJXJDetroyOppositeCountryMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crData *playerdata = myPlayer->getDataClass();
		playerdata->getParam(WCHDATA_JXJShiliID,param);
		unsigned char shiliid = *(unsigned char *)param;

		unsigned short achieveid = m_this->getID();
		if((achieveid-334) == (shiliid-c_startShiliID)) return;

		ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
		if (canvas.valid() && canvas->getDataClass())
		{
			ref_ptr<crData> canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJDestroyCountryid1,param);
			char destroycountryid1 = *(char *)param;
			canvasdata->getParam(WCHDATA_JXJDestroyCountryid2,param);
			char destroycountryid2 = *(char *)param;
			if (achieveid == m_achieveid1)
			{
				if(destroycountryid1==0 || destroycountryid2==0)
					m_this->setStep(1);
			}	
			else if(achieveid == m_achieveid2)
			{
				if(destroycountryid1==1 || destroycountryid2==1)
					m_this->setStep(1);
			}
			else if(achieveid == m_achieveid3)
			{
				if(destroycountryid1==2 || destroycountryid2==2)
					m_this->setStep(1);
			}
			else if(achieveid == m_achieveid4)
			{
				bool isLoseCountry = false;
				myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
				if(isLoseCountry)
					m_this->setStep(1);
			}
			else if(achieveid == m_achieveid5)
			{
				playerdata->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
				unsigned char changeTimes = *(unsigned char *)param;
				if(changeTimes==0)
				{
					bool isLoseCountry = false;
					myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
					if(!isLoseCountry)
					{
						unsigned char step = 0; 
						unsigned char othershili = c_startShiliID;
						if(shiliid != othershili)
						{
							isLoseCountry = false;
							myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
							if(isLoseCountry)
							{
								step++;
							}
						}
						othershili += 1;
						if(shiliid != othershili)
						{
							isLoseCountry = false;
							myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
							if(isLoseCountry)
							{
								step++;
							}
						}
						othershili += 1;
						if(shiliid != othershili)
						{
							isLoseCountry = false;
							myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
							if(isLoseCountry)
							{
								step++;
							}
						}
						m_this->setStep(step);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJDetroyOppositeCountrCheckMethod
//
/////////////////////////////////////////
crJXJDetroyOppositeCountrCheckMethod::crJXJDetroyOppositeCountrCheckMethod():
	m_this(NULL),
	m_player(NULL),
	m_achieveid1(0),
	m_achieveid2(0),
	m_achieveid3(0),
	m_achieveid4(0),
	m_achieveid5(0)
{
}
crJXJDetroyOppositeCountrCheckMethod::crJXJDetroyOppositeCountrCheckMethod(const crJXJDetroyOppositeCountrCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_achieveid1(handle.m_achieveid1),
	m_achieveid2(handle.m_achieveid2),
	m_achieveid3(handle.m_achieveid3),
	m_achieveid4(handle.m_achieveid4),
	m_achieveid5(handle.m_achieveid5)
{
}
void crJXJDetroyOppositeCountrCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJDetroyOppositeCountrCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_achieveid1 = atoi(str.c_str());
		break;
	case 1:
		m_achieveid2 = atoi(str.c_str());
		break;
	case 2:
		m_achieveid3 = atoi(str.c_str());
		break;
	case 3:
		m_achieveid4 = atoi(str.c_str());
		break;
	case 4:
		m_achieveid5 = atoi(str.c_str());
		break;
	}
}
void crJXJDetroyOppositeCountrCheckMethod::operator()(crHandle &handle)
{
	if(m_player && m_player->getDataClass())
	{
		void *param;
		unsigned short achieveid = m_this->getID();
		crData *playerData = m_player->getDataClass();
		playerData->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshiliid = *(unsigned char *)param;

		if((achieveid-334) == (myshiliid-c_startShiliID)) return;

		crData *serdata = crServerBrainHandle::getInstance()->getDataClass();
		crJXJShili *shili;
		crData *shiliData;
		serdata->getParam(WCHDATA_JXJShiliWei+myshiliid-c_startShiliID,param);
		shili = (crJXJShili *)param;
		char destroycountryid1 = -1;
		char destroycountryid2 = -1;
		if(shili && shili->getDataClass())
		{
			shiliData = shili->getDataClass();
			shiliData->excHandle(MAKEINT64(WCH_LockData,1));

			shiliData->getParam(WCHDATA_JXJDestroyCountryid1,param);
			destroycountryid1 = *(char*)param;
			shiliData->getParam(WCHDATA_JXJDestroyCountryid2,param);
			destroycountryid2 = *(char*)param;
			shiliData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		if (achieveid == m_achieveid1)
		{
			if(destroycountryid1==0 || destroycountryid2==0)
				m_this->setStep(1);
		}	
		else if(achieveid == m_achieveid2)
		{
			if(destroycountryid1==1 || destroycountryid2==1)
				m_this->setStep(1);
		}
		else if(achieveid == m_achieveid3)
		{
			if(destroycountryid1==2 || destroycountryid2==2)
				m_this->setStep(1);
		}
		else if(achieveid == m_achieveid4)
		{
			bool isLoseCountry = false;
			crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
			if(isLoseCountry)
				m_this->setStep(1);
		}
		else if(achieveid == m_achieveid5)
		{
			playerData->getParam(WCHDATA_JXJPlayerChangeShiliTimes,param);
			unsigned char changeTimes = *(unsigned char *)param;
			if(changeTimes==0)
			{
				bool isLoseCountry = false;
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
				if(!isLoseCountry)
				{
					unsigned char step = 0; 
					unsigned char othershili = c_startShiliID;
					if(myshiliid != othershili)
					{
						isLoseCountry = false;
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
						if(isLoseCountry)
						{
							step++;
						}
					}
					othershili += 1;
					if(myshiliid != othershili)
					{
						isLoseCountry = false;
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
						if(isLoseCountry)
						{
							step++;
						}
					}
					othershili += 1;
					if(myshiliid != othershili)
					{
						isLoseCountry = false;
						crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&othershili));
						if(isLoseCountry)
						{
							step++;
						}
					}
					m_this->setStep(step);
				}
			}
		}
	}
}


/////////////////////////////////////////
//
//crJXJTroopsTechExamineMethod
//
/////////////////////////////////////////
crJXJTroopsTechExamineMethod::crJXJTroopsTechExamineMethod():
	m_this(NULL)
{
}
crJXJTroopsTechExamineMethod::crJXJTroopsTechExamineMethod(const crJXJTroopsTechExamineMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJTroopsTechExamineMethod::inputParam(int i, void *param)
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
void crJXJTroopsTechExamineMethod::addParam(int i, const std::string& str)
{
}
void crJXJTroopsTechExamineMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && myPlayer->getDataClass())
	{
		void *param;
		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			ref_ptr<crData> data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJTroopsTechMap,param);
			TroopsTechMap troopstechmap = *(TroopsTechMap *)param;
	
			unsigned short troopstechid = (unsigned short)atoi(record[beizhuindex].c_str());
			TroopsTechMap::iterator troopsitr = troopstechmap.find(troopstechid);
			if(troopsitr != troopstechmap.end())
				m_this->setStep(troopsitr->second);
		}
	}
}

/////////////////////////////////////////
//
//crJXJTroopsTechExamineCheckMethod
//
/////////////////////////////////////////
crJXJTroopsTechExamineCheckMethod::crJXJTroopsTechExamineCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJTroopsTechExamineCheckMethod::crJXJTroopsTechExamineCheckMethod(const crJXJTroopsTechExamineCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJTroopsTechExamineCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJTroopsTechExamineCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJTroopsTechExamineCheckMethod::operator()(crHandle &handle)
{
	if(m_player && m_player->getDataClass())
	{
		void *param;
		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			ref_ptr<crData> data = m_player->getDataClass();
			data->getParam(WCHDATA_JXJTroopsTechMap,param);
			TroopsTechMap troopstechmap = *(TroopsTechMap *)param;

			unsigned short troopstechid = (unsigned short)atoi(record[beizhuindex].c_str());
			TroopsTechMap::iterator troopsitr = troopstechmap.find(troopstechid);
			if(troopsitr != troopstechmap.end())
				m_this->setStep(troopsitr->second);
		}
	}
}

/////////////////////////////////////////
//
//crJXJTroopsTechAllFinishMethod
//
/////////////////////////////////////////
crJXJTroopsTechAllFinishMethod::crJXJTroopsTechAllFinishMethod():
	m_this(NULL)
{
}
crJXJTroopsTechAllFinishMethod::crJXJTroopsTechAllFinishMethod(const crJXJTroopsTechAllFinishMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJTroopsTechAllFinishMethod::inputParam(int i, void *param)
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
void crJXJTroopsTechAllFinishMethod::addParam(int i, const std::string& str)
{
}
void crJXJTroopsTechAllFinishMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this->getComplete()) return;

	if(myPlayer && myPlayer->getDataClass())
	{
		void *param;
		bool setcomplete = false;
		ref_ptr<crData> data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap troopstechmap = *(TroopsTechMap *)param;
		TroopsTechMap::iterator troopsitr;

		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			crTableIO::DataVec records;
			int fulllv = 0;
			int nowlv = 0;
			crVector2i range;
			crArgumentParser::appAtoVec(record[beizhuindex],range);
			for (int i=range[0]; i<=range[1]; i++)
			{
				troopsitr = troopstechmap.find((unsigned short)i);
				if(troopsitr == troopstechmap.end()) 
					break;
				else
					nowlv = troopsitr->second;

				TroopsTechLvTab->queryRecords(0,crArgumentParser::appItoa(i),records);
				fulllv = atoi(records[records.size()-1][1].c_str());

				if(nowlv != fulllv) 
					break;
				else
				{
					if(i == range[1])
						setcomplete = true;
				}
			}

			if(setcomplete)
				m_this->setStep(1);
		}
	}
}

/////////////////////////////////////////
//
//crJXJTroopsTechAllFinishCheckMethod
//
/////////////////////////////////////////
crJXJTroopsTechAllFinishCheckMethod::crJXJTroopsTechAllFinishCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJTroopsTechAllFinishCheckMethod::crJXJTroopsTechAllFinishCheckMethod(const crJXJTroopsTechAllFinishCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJTroopsTechAllFinishCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJTroopsTechAllFinishCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJTroopsTechAllFinishCheckMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	if(m_player && m_player->getDataClass())
	{
		void *param;
		bool setcomplete = false;
		ref_ptr<crData> data = m_player->getDataClass();
		data->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap troopstechmap = *(TroopsTechMap *)param;
		TroopsTechMap::iterator troopsitr;

		unsigned short achieveid = m_this->getID();

		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			crTableIO::DataVec records;
			int fulllv = 0;
			int nowlv = 0;
			crVector2i range;
			crArgumentParser::appAtoVec(record[beizhuindex],range);
			for (int i=range[0]; i<=range[1]; i++)
			{
				troopsitr = troopstechmap.find((unsigned short)i);
				if(troopsitr == troopstechmap.end()) 
					break;
				else
					nowlv = troopsitr->second;

				TroopsTechLvTab->queryRecords(0,crArgumentParser::appItoa(i),records);
				fulllv = atoi(records[records.size()-1][1].c_str());

				if(nowlv != fulllv) 
					break;
				else
				{
					if(i == range[1])
						setcomplete = true;
				}
			}

			if(setcomplete)
				m_this->setStep(1);
		}
	}
}


/////////////////////////////////////////
//
//crJXJMingJiangWorldPassCountMethod
//
/////////////////////////////////////////
crJXJMingJiangWorldPassCountMethod::crJXJMingJiangWorldPassCountMethod():
	m_this(NULL),
	m_achieveid1(0),
	m_achieveid2(0),
	m_achieveid3(0)
{
}
crJXJMingJiangWorldPassCountMethod::crJXJMingJiangWorldPassCountMethod(const crJXJMingJiangWorldPassCountMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_achieveid1(handle.m_achieveid1),
	m_achieveid2(handle.m_achieveid2),
	m_achieveid3(handle.m_achieveid3),
	m_achieveid4(handle.m_achieveid4)
{
}
void crJXJMingJiangWorldPassCountMethod::inputParam(int i, void *param)
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
void crJXJMingJiangWorldPassCountMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_achieveid1 = atoi(str.c_str());
		break;
	case 1:
		m_achieveid2 = atoi(str.c_str());
		break;
	case 2:
		m_achieveid3 = atoi(str.c_str());
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_achieveid4);
		break;
	}
}
void crJXJMingJiangWorldPassCountMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && myPlayer->getDataClass())
	{
		int achieveid = m_this->getID();
		ref_ptr<crData> data = myPlayer->getDataClass();
		
		if (achieveid == m_achieveid1)
		{
			data->getParam(WCHDATA_JXJMingJiangFailCount,param);
			unsigned char count = *(unsigned char *)param;
			m_this->setStep(count);
		}
		else if (achieveid == m_achieveid2 || achieveid == m_achieveid3)
		{
			data->getParam(WCHDATA_JXJPassMingJiangCount,param);
			unsigned short count = *(unsigned short *)param;
			m_this->setStep(count);
		}
		else if (achieveid == m_achieveid4[0] || achieveid == m_achieveid4[1] || achieveid == m_achieveid4[2])
		{
			data->getParam(WCHDATA_JXJZhengZhanTianXiaFailCount,param);
			unsigned char count = *(unsigned char *)param;
			m_this->setStep(count);
		}
		else
		{
			data->getParam(WCHDATA_JXJPassWorldFuBenCount,param);
			unsigned short count = *(unsigned short *)param;
			m_this->setStep(count);
		}
	}
}

/////////////////////////////////////////
//
//crJXJMingJiangWorldPassCountCheckMethod
//
/////////////////////////////////////////
crJXJMingJiangWorldPassCountCheckMethod::crJXJMingJiangWorldPassCountCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJMingJiangWorldPassCountCheckMethod::crJXJMingJiangWorldPassCountCheckMethod(const crJXJMingJiangWorldPassCountCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJMingJiangWorldPassCountCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJMingJiangWorldPassCountCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_achieveid1 = atoi(str.c_str());
		break;
	case 1:
		m_achieveid2 = atoi(str.c_str());
		break;
	case 2:
		m_achieveid3 = atoi(str.c_str());
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_achieveid4);
		break;
	}
}
void crJXJMingJiangWorldPassCountCheckMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	void *param;
	if(m_player && m_player->getDataClass())
	{
		int achieveid = m_this->getID();
		ref_ptr<crData> data = m_player->getDataClass();

		if (achieveid == m_achieveid1)
		{
			data->getParam(WCHDATA_JXJMingJiangFailCount,param);
			unsigned char count = *(unsigned char *)param;
			m_this->setStep(count);
		}
		else if (achieveid == m_achieveid2 || achieveid == m_achieveid3)
		{
			data->getParam(WCHDATA_JXJPassMingJiangCount,param);
			unsigned short count = *(unsigned short *)param;
			m_this->setStep(count);
		}
		else if (achieveid == m_achieveid4[0] || achieveid == m_achieveid4[1] || achieveid == m_achieveid4[2])
		{
			data->getParam(WCHDATA_JXJZhengZhanTianXiaFailCount,param);
			unsigned char count = *(unsigned char *)param;
			m_this->setStep(count);
		}
		else
		{
			data->getParam(WCHDATA_JXJPassWorldFuBenCount,param);
			unsigned short count = *(unsigned short *)param;
			m_this->setStep(count);
		}
	}
}

/////////////////////////////////////////
//
//crJXJMingJiangFinishMethod
//
/////////////////////////////////////////
crJXJMingJiangFinishMethod::crJXJMingJiangFinishMethod():
	m_this(NULL)
{
}
crJXJMingJiangFinishMethod::crJXJMingJiangFinishMethod(const crJXJMingJiangFinishMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJMingJiangFinishMethod::inputParam(int i, void *param)
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
void crJXJMingJiangFinishMethod::addParam(int i, const std::string& str)
{
}
void crJXJMingJiangFinishMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && myPlayer->getDataClass())
	{
		void *param;
		ref_ptr<crData> data = myPlayer->getDataClass();

		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			int fubenid = atoi(record[beizhuindex].c_str());
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
			if(fubenid > 0 && fubenProgress)
			{
				FubenProgressMap::iterator itrcur;
				itrcur = fubenProgress->find((unsigned short)fubenid);
				if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 0)
					m_this->setStep(1);
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////
//
//crJXJMingJiangFinishCheckMethod
//
/////////////////////////////////////////
crJXJMingJiangFinishCheckMethod::crJXJMingJiangFinishCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJMingJiangFinishCheckMethod::crJXJMingJiangFinishCheckMethod(const crJXJMingJiangFinishCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJMingJiangFinishCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJMingJiangFinishCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJMingJiangFinishCheckMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	if(m_player && m_player->getDataClass())
	{
		void *param;
		ref_ptr<crData> data = m_player->getDataClass();

		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			int fubenid = atoi(record[beizhuindex].c_str());
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
			if(fubenid > 0 && fubenProgress)
			{
				FubenProgressMap::iterator itrcur;
				itrcur = fubenProgress->find((unsigned short)fubenid);
				if (itrcur != fubenProgress->end() && itrcur->second->getComplete() > 0)
					m_this->setStep(1);
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}


/////////////////////////////////////////
//
//crJXJContinueFinishHuYueDuMethod
//
/////////////////////////////////////////
crJXJContinueFinishHuYueDuMethod::crJXJContinueFinishHuYueDuMethod():
	m_netType(GameClient_Game){}
crJXJContinueFinishHuYueDuMethod::crJXJContinueFinishHuYueDuMethod(const crJXJContinueFinishHuYueDuMethod& handle):
	crMethod(handle)
{
}
void crJXJContinueFinishHuYueDuMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJContinueFinishHuYueDuMethod::addParam(int i, const std::string& str)
{
}

void crJXJContinueFinishHuYueDuMethod::operator()(crHandle &handle)
{
	if(m_stream.valid() && m_this && m_this->getDataClass())
	{
		void *param;
		if(m_netType == GameServer)
		{
			unsigned char newdays = m_stream->_readUChar();
			int achievecount = m_stream->_readInt();
			ref_ptr<crData> data = m_this->getDataClass();

			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJIsCheckHuoYueDuSign,param);
			bool *ischeck = (bool *)param;
			*ischeck = false;

			data->getParam(WCHDATA_JXJFinishHuoYueDuDayCount,param);
			unsigned char *continueday = (unsigned char *)param;
			*continueday = newdays;
			data->getParam(WCHDATA_JXJAchievePointsCount,param);
			int *praisecount = (int *)param;
			*praisecount = achievecount;

			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveShopBuyMethod
//成就商店购买物品
//
//////////////////////////////////////////////////////////////////////////
crJXJAchieveShopBuyMethod::crJXJAchieveShopBuyMethod()
{}
crJXJAchieveShopBuyMethod::crJXJAchieveShopBuyMethod(const crJXJAchieveShopBuyMethod& handle)
{

}
void crJXJAchieveShopBuyMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJAchieveShopBuyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_scrollbar = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJAchieveShopBuyMethod::operator()(crHandle &handle)
{
	crCanvasNode *canvas = m_this->getParentCanvas();
	if(!canvas) return;

	ref_ptr<crScrollBarWidgetNode> shopscroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scrollbar));
	if(!shopscroll.valid()) return;

	int val = 0;
	val = shopscroll->getValue();
	ref_ptr<crTableIO> AchieveShoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAchieveShopTab);
	int cRow = AchieveShoptab->getRowCount();
	int id = m_index+5*val;
	if(id < cRow) 
	{
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(1);
		stream->_writeUChar(id);

		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAchieveShopBuy,stream.get());
		if(netConductor)
			netConductor->getNetManager()->sendPacket("all",packet);
	}
}

//////////////////////////////////////////
//
//crJXJRecvAchieveShopBuyMethod
//
//////////////////////////////////////////
crJXJRecvAchieveShopBuyMethod::crJXJRecvAchieveShopBuyMethod(){}
crJXJRecvAchieveShopBuyMethod::crJXJRecvAchieveShopBuyMethod(const crJXJRecvAchieveShopBuyMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvAchieveShopBuyMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvAchieveShopBuyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvAchieveShopBuyMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			unsigned char topcount = 10;
			unsigned char index = m_stream->_readUChar();

			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (playerData.valid() &&  m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *mainRoleData = m_this->getMainRole()->getDataClass();
				mainRoleData->getParam(WCHDATA_Level, param);
				unsigned char mylv = *(unsigned char *)param;

				crTableIO::StrVec record;
				ref_ptr<crTableIO> achieveshoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAchieveShopTab);
				int countindex = achieveshoptab->getTitleIndex("数量");
				int dengjiindex = achieveshoptab->getTitleIndex("等级");
				int typeindex = achieveshoptab->getTitleIndex("类型");
				int priceindex = achieveshoptab->getTitleIndex("商店价格");
				int buycountindex = achieveshoptab->getTitleIndex("购买次数");
				int achieveindex = achieveshoptab->getTitleIndex("成就条件");
				
				char code = 0;//-1成就点数不足，-2等级不足，-3购买次数,-4成就条件未达到
				record = achieveshoptab->getDataVec()[index];
				unsigned char id = atoi(record[0].c_str());
				crData *playergameData = m_this->getDataClass();
				playergameData->excHandle(MAKEINT64(WCH_LockData,1));
				playergameData->getParam(WCHDATA_JXJAchievePointsCount,param);
				int *dailypoints = (int*)param;
				playergameData->getParam(WCHDATA_JXJAchieveShopMap,param);
				AchieveShopMap *achieveShopMap = (AchieveShopMap *)param;
				int buycount = 0;
				int price = 0;
				do 
				{
					price = atoi(record[priceindex].c_str());
					if(*dailypoints < price)
					{
						code = -1;
						break;
					}
					int needlv = atoi(record[dengjiindex].c_str());
					if(mylv < needlv)
					{
						code = -2;
						break;
					}
					buycount = atoi(record[buycountindex].c_str());
					if(buycount>0)
					{
						AchieveShopMap::iterator itr = achieveShopMap->find(id);
						if(itr != achieveShopMap->end())
						{
							if(buycount<=itr->second)
							{
								code = -3;
								break;
							}
						}
					}
					int achieveid = atoi(record[achieveindex].c_str());
					if(achieveid>0)
					{
						mainRoleData->getParam(WCHDATA_JXJAccomplishMap,param);
						mainRoleData->excHandle(MAKEINT64(WCH_LockData,1));
						AccomplishMap *accomplishMap = (AccomplishMap*)param;
						AccomplishMap::iterator itr = accomplishMap->find(achieveid);
						if(itr == accomplishMap->end() || !itr->second->isComplete())
						{
							code = -4;
							break;
						}
						mainRoleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				} while (0);

				if(code == 0)
				{//可以购买
					int thingtype = atoi(record[typeindex].c_str());
					int rescount = 0;
					crVector3i iteminfo;
					if (thingtype==T_Item || thingtype==T_TroopsEquip)
						crArgumentParser::appAtoVec(record[countindex],iteminfo);
					else
						rescount = atoi(record[countindex].c_str());

					CRCore::ref_ptr<crPlayerRewardData> rewardData = new crPlayerRewardData;
					switch (thingtype)
					{
					case T_Food:
						rewardData->setFood(rescount);
						break;
					case T_Wood:
						rewardData->setWood(rescount);
						break;
					case T_Iron:
						rewardData->setIron(rescount);
						break;
					case T_Horse:
						rewardData->setHorse(rescount);
						break;
					case T_Copper:
						rewardData->setCoopercash(rescount);
						break;
					case T_Exp:
						rewardData->setExperience(rescount);
						break;
					case T_Achievement:
						rewardData->setAchievement(rescount);
						break;
					case T_Giftgold:
						rewardData->setGiftgold(rescount);
						break;
					case T_TroopsEquip:
						{
							rewardData->getEquipsVec().push_back(std::make_pair(iteminfo[0],iteminfo[2]));
						}
						break;
					case T_Item:
						{
							ref_ptr<crBagItemData> itemdata = new crBagItemData;
							itemdata->setItemID(iteminfo[0]);
							itemdata->setEquipMagic(iteminfo[1]);
							itemdata->setItemCount(iteminfo[2]);
							rewardData->getRewardItemsVec().push_back(itemdata);
						}
						break;
					case T_Contribute:
						rewardData->setCountryContribution(rescount);
						break;
					case T_Exploit:
						rewardData->setExploit(rescount);
						break;
					case T_Zhanquan:
						m_this->doEvent(WCH_JXJRecvZhengZanQuan,MAKEINT64(rescount,NULL));
						break;
					case T_VIPExp:
						m_this->doEvent(WCH_JXJAddVipExp, MAKEINT64(rescount, NULL));
						break;
					}

					if (thingtype != T_VIPExp && thingtype != T_Zhanquan)
					{
						rewardData->setType(GP_OnlineReward);
						if(rewardData.valid())
						{
							m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKEINT64(rewardData.get(),NULL));
						}
					}

					*dailypoints -= price;
					unsigned char count = 0;
					if(buycount>0)
					{
						(*achieveShopMap)[id] += 1;
						count = (*achieveShopMap)[id];
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(5);
					stream->_writeChar(code);
					stream->_writeInt(*dailypoints);
					stream->_writeUChar(id);
					stream->_writeUChar(count);

					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAchieveShopBuy,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeChar(code);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAchieveShopBuy,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				playergameData->excHandle(MAKEINT64(WCH_LockData,0));

				if(code == 0)
				{
					std::string logdata = "成就商店兑换："+crArgumentParser::appItoa(id);
					GameLogData gamelog(Log_AchieveShopBuy, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(playerid, &gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char code = m_stream->_readUChar();
			switch (code)
			{
			case 0:
				{
					crData *playerData = m_this->getDataClass();

					int usedpoints = m_stream->_readInt();
					playerData->inputParam(WCHDATA_JXJAchievePointsCount,&usedpoints);
					unsigned char id = m_stream->_readUChar();
					unsigned char count = m_stream->_readUChar();
					void *param;
					playerData->excHandle(MAKEINT64(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJAchieveShopMap,param);
					AchieveShopMap *achieveShopMap = (AchieveShopMap *)param;
					(*achieveShopMap)[id] = count;
					playerData->excHandle(MAKEINT64(WCH_LockData,0));

					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					canvas->doEvent(WCH_UI_UpdateData);
				}
				break;
			case -1://成就点数不足
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(1117, NULL));
				break;
			case -2://等级不足
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(1118, NULL));
				break;
			case -3://需要完成相关的成就
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(1119, NULL));
				break;
			case -4://需要完成相关的成就
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(1120, NULL));
				break;
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJChengjiuShopScrollInitMethod
//
//
/////////////////////////////////////////
crJXJChengjiuShopScrollInitMethod::crJXJChengjiuShopScrollInitMethod()
{
}
crJXJChengjiuShopScrollInitMethod::crJXJChengjiuShopScrollInitMethod(const crJXJChengjiuShopScrollInitMethod& handle):
	crMethod(handle)
{
}
void crJXJChengjiuShopScrollInitMethod::inputParam(int i, void *param)
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
void crJXJChengjiuShopScrollInitMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_scrollbar = str;
		break;
	}
}

void crJXJChengjiuShopScrollInitMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crScrollBarWidgetNode>  shopscroll = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scrollbar));
		ref_ptr<crTableIO> shoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAchieveShopTab);
		int cRow = shoptab->getRowCount();
		if (shopscroll.valid())
		{
			if (cRow < 10)
			{
				shopscroll->setEnable(false);
			}
			else
			{
				shopscroll->setEnable(true);
				int crows = cRow%5==0? cRow/5 : cRow/5+1;
				shopscroll->setRange(0, crows-2);
				shopscroll->setLineValue(1.0f);
				shopscroll->setPageValue(1.0f);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGuoZhanAchieveMethod
//
/////////////////////////////////////////
crJXJGuoZhanAchieveMethod::crJXJGuoZhanAchieveMethod():
	m_this(NULL)
{
}
crJXJGuoZhanAchieveMethod::crJXJGuoZhanAchieveMethod(const crJXJGuoZhanAchieveMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJGuoZhanAchieveMethod::inputParam(int i, void *param)
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
void crJXJGuoZhanAchieveMethod::addParam(int i, const std::string& str)
{
}
void crJXJGuoZhanAchieveMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && myPlayer->getDataClass())
	{
		void *param;
		ref_ptr<crData> playergdata = myPlayer->getDataClass();

		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			CRCore::crVector4i notevec;
			crArgumentParser::appAtoVec(record[beizhuindex],notevec);
			int stepcount = notevec[0];
			int nowstep = 0;
			if (notevec[1] > 0)
			{
				if(notevec[2] == 0)
				{
					playergdata->getParam(notevec[1],param);
					nowstep = *(int *)param;
				}
				else if(notevec[2] <= 4)
				{
					if (stepcount < 255)
					{
						playergdata->getParam(notevec[1],param);
						crVector4s cbingxi = *(crVector4s *)param;
						nowstep = cbingxi[notevec[2] - 1];
					}
					else
					{
						playergdata->getParam(notevec[1],param);
						crVector4i cbingxi = *(crVector4i *)param;
						nowstep = cbingxi[notevec[2] - 1];
					}
				}
				else
				{
					playergdata->getParam(notevec[1],param);
					std::map<int,CRCore::crVector3i> troopsinfomap = *(std::map<int,CRCore::crVector3i> *)param;
					std::map<int,CRCore::crVector3i>::iterator troopinfoitr = troopsinfomap.find(notevec[2]);
					if(troopinfoitr != troopsinfomap.end())
						nowstep = troopinfoitr->second[notevec[3]];
				}
			}
			if (stepcount < 255)
			{
				m_this->setStep(nowstep);
			}
			else
			{
				if (nowstep >= stepcount)
					m_this->setStep(255);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJGuoZhanAchieveCheckMethod
//
/////////////////////////////////////////
crJXJGuoZhanAchieveCheckMethod::crJXJGuoZhanAchieveCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJGuoZhanAchieveCheckMethod::crJXJGuoZhanAchieveCheckMethod(const crJXJGuoZhanAchieveCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJGuoZhanAchieveCheckMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_player = (crPlayerGameData *)(LOINT64(param64));
		}
		else
		{
			m_player = NULL;
		}
		break;
	}
}
void crJXJGuoZhanAchieveCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJGuoZhanAchieveCheckMethod::operator()(crHandle &handle)
{
	if(m_this->getComplete()) return;

	if (m_player)
	{
		void *param;
		ref_ptr<crData> playergdata = m_player->getDataClass();

		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa((int)achieveid),record) >=0)
		{
			CRCore::crVector4i notevec;
			crArgumentParser::appAtoVec(record[beizhuindex],notevec);
			int stepcount = notevec[0];
			int nowstep = 0;
			if (notevec[1] > 0)
			{
				if(notevec[2] == 0)
				{
					playergdata->getParam(notevec[1],param);
					nowstep = *(int *)param;
				}
				else if(notevec[2] <= 4)
				{
					if (stepcount < 255)
					{
						playergdata->getParam(notevec[1],param);
						crVector4s cbingxi = *(crVector4s *)param;
						nowstep = cbingxi[notevec[2] - 1];
					}
					else
					{
						playergdata->getParam(notevec[1],param);
						crVector4i cbingxi = *(crVector4i *)param;
						nowstep = cbingxi[notevec[2] - 1];
					}
				}
				else
				{
					playergdata->getParam(notevec[1],param);
					std::map<int,CRCore::crVector3i> troopsinfomap = *(std::map<int,CRCore::crVector3i> *)param;
					std::map<int,CRCore::crVector3i>::iterator troopinfoitr = troopsinfomap.find(notevec[2]);
					if(troopinfoitr != troopsinfomap.end())
						nowstep = troopinfoitr->second[notevec[3]];
				}
			}
			if (stepcount < 255)
			{
				m_this->setStep(nowstep);
			}
			else
			{
				if(nowstep >= stepcount)
					m_this->setStep(255);
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJFoundCountryCanvasUpdateNewMethod
 
**************************************************************************/
JXJ::crJXJFoundCountryCanvasUpdateNewMethod::crJXJFoundCountryCanvasUpdateNewMethod():
	m_this(NULL)
{

}

JXJ::crJXJFoundCountryCanvasUpdateNewMethod::crJXJFoundCountryCanvasUpdateNewMethod( const crJXJFoundCountryCanvasUpdateNewMethod & handle ):
	crMethod(handle),
	m_getAllAwardBtn(handle.m_getAllAwardBtn),
	m_pageSw(handle.m_pageSw),
	m_goToGuozhanBtn(handle.m_goToGuozhanBtn),
	m_goalInput1(handle.m_goalInput1),
	m_goalInput2(handle.m_goalInput2),
	m_goalInput3(handle.m_goalInput3),
	m_ditu(handle.m_ditu)
{
	//for (int i = 0; i < 3; ++ i)
	//{
	//	for (int j = 0; j < 8; ++ j)
	//	{
	//		m_citySw[i][j] = handle.m_citySw[i][j];
	//		m_citybckSw[i][j] = handle.m_citybckSw[i][j];
	//	}
	//}
	//for (int j = 0; j < 8; ++ j)
	//	m_BtnSw[j] = handle.m_BtnSw[j];
}

void JXJ::crJXJFoundCountryCanvasUpdateNewMethod::inputParam( int i, void *param )
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

void JXJ::crJXJFoundCountryCanvasUpdateNewMethod::addParam( int i, const std::string& str )
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
	case 22:
	case 23:
		m_citybckSw[i / 8][i % 8] = str;
		break;
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
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
		m_citySw[(i - 24) / 8][(i - 24) % 8] = str;
		break;
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
		m_BtnSw[i - 48] = str;
		break;
	case 56:
		m_pageSw = str;
		break;
	case 57:
		m_goalInput1 = str;
		break;
	case 58:
		m_goalInput2 = str;
		break;
	case 59:
		m_goalInput3 = str;
		break;
	case 60:
		m_getAllAwardBtn = str;
		break;
	case 61:
		m_goToGuozhanBtn = str;
		break;
	case 62:
		m_ditu = str;
		break;
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
	case 76:
	case 77:
	case 78:
		m_icon[i-63] = str;
		break;
	case 79:
	case 80:
	case 81:
	case 82:
	case 83:
	case 84:
	case 85:
	case 86:
	case 87:
	case 88:
	case 89:
	case 90:
	case 91:
	case 92:
	case 93:
	case 94:
		m_num[i-79] = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJFoundCountryCanvasUpdateNewMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crMultiSwitch> pageSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_pageSw));
		ref_ptr<crMultiSwitch> ditu = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_ditu));
		ref_ptr<crStaticTextWidgetNode> goalInput1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goalInput1));
		ref_ptr<crStaticTextWidgetNode> goalInput2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goalInput2));
		ref_ptr<crStaticTextWidgetNode> goalInput3 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goalInput3));
		ref_ptr<crButtonWidgetNode> getAllAwardBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_getAllAwardBtn));
		ref_ptr<crButtonWidgetNode> goToBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goToGuozhanBtn));
		//ref_ptr<crMultiSwitch> getAllAwardBtnSw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_getAllAwardBtnSw));

		ref_ptr<crMultiSwitch> citySw[3][8] = {NULL};
		ref_ptr<crMultiSwitch> BtnSw[8] = {NULL};
		//ref_ptr<crStaticTextWidgetNode> cityInput[3][9] = {NULL};
		ref_ptr<crMultiSwitch> citybckSw[3][8] = {NULL};
		for (int i = 0; i < 3; ++ i)
		{
			for (int j  = 0; j < 8; ++ j)
			{
				citySw[i][j]  = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_citySw[i][j]));
				citybckSw[i][j] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_citybckSw[i][j]));
			}
		}
		for (int j  = 0; j < 8; ++ j)
		{
			BtnSw[j]  = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_BtnSw[j]));
		}
		unsigned char hadGotAward = 0;
		unsigned char shiliId = 0;
		unsigned char winCity = 0;

		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getDataClass())
		{
			crData * playerData = myPlayer->getDataClass();
			playerData->getParam(WCHDATA_JXJFoundCountryGetAward,param);
			hadGotAward = *(unsigned char *)param;

			playerData->getParam(WCHDATA_JXJShiliID,param);
			shiliId = *(unsigned char *)param;

			playerData->excHandle(MAKEINT64(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJMyCountryData,param);
			crJXJShili *shiliPtr = (crJXJShili *)param;
			if (shiliPtr && shiliPtr->getDataClass())
			{
				crData *shiliData = shiliPtr->getDataClass();
				shiliData->getParam(WCHDATA_JXJShiliFoundCountryWinCity,param);
				winCity = *(unsigned char *)param;
			}
			playerData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		if (shiliId >= c_startShiliID && shiliId < c_startShiliID + 3)
		{
			unsigned char shiliOffset = shiliId - c_startShiliID;
			if (pageSw.valid())
				pageSw->setActiveSwitchSet(shiliOffset);
			if(ditu.valid())
				ditu->setActiveSwitchSet(shiliOffset);

			ref_ptr<crTableIO> foundCountryAwardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);
			crTableIO::DataVec dataVec;
			int shiliindex = foundCountryAwardTab->getTitleIndex("shili");
			int awardindex1 = foundCountryAwardTab->getTitleIndex("奖励1");
			int awardindex2 = foundCountryAwardTab->getTitleIndex("奖励2");
			foundCountryAwardTab->queryRecords(shiliindex,crArgumentParser::appItoa(shiliId),dataVec);
			CRCore::crVector4i awardVec;
			ref_ptr<crImageBoxWidgetNode> icon[16];
			ref_ptr<crStaticTextWidgetNode> num[16];
			for (int i=0; i<16; i++)
			{
				icon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i]));
				num[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_num[i]));
			}
			for( int i = 0; i<8; i++)
			{
				crArgumentParser::appAtoVec(dataVec[i][awardindex1],awardVec);
				setRewardInfo( awardVec,icon[i*2].get(),num[i*2].get());
				crArgumentParser::appAtoVec(dataVec[i][awardindex2],awardVec);
				setRewardInfo( awardVec,icon[i*2+1].get(),num[i*2+1].get());
			}
			//int textID = 0;
			//if (winCity == (unsigned char)-1)
			//{
			//	// 全部占领
			//	switch (shiliId)
			//	{
			//	case 10:
			//		textID = 4100;
			//		break;
			//	case 11:
			//		textID = 4101;
			//		break;
			//	case 12:
			//		textID = 4102;
			//		break;
			//	default:
			//		break;
			//	}
			//}
			//else
			//{
			//	// 建国途中
			//	switch (shiliId)
			//	{
			//	case 10:
			//		textID = 4097;
			//		break;
			//	case 11:
			//		textID = 4098;
			//		break;
			//	case 12:
			//		textID = 4099;
			//		break;
			//	default:
			//		break;
			//	}
			//}
			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec textrecord;
			if(textTab.valid())
			{
				if(textTab->queryOneRecord(0,crArgumentParser::appItoa(9189),textrecord) >= 0)
					goalInput1->setString(textrecord[1]);
				if(textTab->queryOneRecord(0,crArgumentParser::appItoa(9190),textrecord) >= 0)
					goalInput2->setString(textrecord[1]);
				if(textTab->queryOneRecord(0,crArgumentParser::appItoa(9191),textrecord) >= 0)
					goalInput3->setString(textrecord[1]);
			}
			
			for (int i = 0; i < 8; ++ i)
			{
				if (winCity & (1 << i)) 
				{
					// 如果被占领
					if (!(hadGotAward & (1 << i))) // 没有领取过
					{
						if(BtnSw[i].valid())
							BtnSw[i]->setActiveSwitchSet(1);

						if(citySw[shiliOffset][i].valid())
							citySw[shiliOffset][i]->setActiveSwitchSet(0);
					}
					else 
					{
						if(BtnSw[i].valid())
							BtnSw[i]->setActiveSwitchSet(2);

						if(citySw[shiliOffset][i].valid())
							citySw[shiliOffset][i]->setActiveSwitchSet(0);
					}
					if(citybckSw[shiliOffset][i].valid())
						citybckSw[shiliOffset][i]->setActiveSwitchSet(1);
				}
				else
				{
					if(citybckSw[shiliOffset][i].valid())
						citybckSw[shiliOffset][i]->setActiveSwitchSet(1);

					if(BtnSw[i].valid())
						BtnSw[i]->setActiveSwitchSet(0);

					if(citySw[shiliOffset][i].valid())
						citySw[shiliOffset][i]->setActiveSwitchSet(1);
				}
			}
			
			if (getAllAwardBtn.valid())
			{
				if (winCity != (unsigned char)-1)
				{
					getAllAwardBtn->setVisiable(false);
					//getAllAwardBtn->setEnable(false);
					//if (getAllAwardBtnSw.valid())
					//	getAllAwardBtnSw->setActiveSwitchSet(3);
				}
				else if (winCity == (unsigned char) -1)
				{
					getAllAwardBtn->setVisiable(true);
					//getAllAwardBtn->setEnable(true);
					//if (getAllAwardBtnSw.valid())
					//	getAllAwardBtnSw->setActiveSwitchSet(0);
				}
			}
			if (goToBtn.valid())
			{
				if (winCity != (unsigned char ) -1)
				{
					goToBtn->setVisiable(true);
					//goToBtn->setEnable(true);
				}
				else goToBtn->setVisiable(false);//goToBtn->setEnable(false);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJSetIsClickHuoDongSignMethod
//
//
//////////////////////////////////////////////////////////////////////////

unsigned char crJXJSetIsClickHuoDongSignMethod::s_isclickhudongbtn[2] = {0,0};
crJXJSetIsClickHuoDongSignMethod::crJXJSetIsClickHuoDongSignMethod()
{}
crJXJSetIsClickHuoDongSignMethod::crJXJSetIsClickHuoDongSignMethod(const crJXJSetIsClickHuoDongSignMethod& handle)
{

}
void crJXJSetIsClickHuoDongSignMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJSetIsClickHuoDongSignMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJSetIsClickHuoDongSignMethod::operator()(crHandle &handle)
{
	/*ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid() && canvas->getDataClass())
	{*/
		/*void *param;
		ref_ptr<crData> canvasdata = canvas->getDataClass();
		canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
		canvasdata->getParam(WCHDATA_JXJIsClickHongDongBtn,param);
		crVector2s *sign = (crVector2s *)param;
		(*sign)[m_index] = 1;*/
		s_isclickhudongbtn[m_index] = 1;
		/*canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}*/
}

/////////////////////////////////////////
//
//crJXJChengjiuButtonReClickMethod
//成就按钮转换[UI]
//
/////////////////////////////////////////
//crJXJChengjiuButtonReClickMethod::crJXJChengjiuButtonReClickMethod():
//	m_index(0)
//{
//}
//crJXJChengjiuButtonReClickMethod::crJXJChengjiuButtonReClickMethod(const crJXJChengjiuButtonReClickMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJChengjiuButtonReClickMethod::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crWidgetNode*)param;
//		break;
//	}
//}
//void crJXJChengjiuButtonReClickMethod::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_index = atoi(str.c_str());
//		break;
//	case 1:
//	case 2:
//	case 3:
//	case 4:
//	case 5:
//	case 6:
//	case 7:
//	case 8:
//	case 9:
//	case 10:
//	case 11:
//		m_Button[i-1] = str;
//		break;
//	}
//}
//
//void crJXJChengjiuButtonReClickMethod::operator()(crHandle &handle)
//{
//	crCanvasNode *canvas = m_this->getParentCanvas();
//	if(canvas)
//	{
//		crData *canvasData = canvas->getDataClass();
//		if(canvasData)
//		{
//			void *param;
//			std::vector<crCheckBoxWidgetNode*> ButtonVec;
//			for(int i = 0; i < 11; ++i)
//			{
//				crCheckBoxWidgetNode *button = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_Button[i]));
//				if(button)ButtonVec.push_back(button);
//			}
//			
//			canvasData->getParam(WCHDATA_JXJAccomplishCanvasIcon,param);
//			CanvasIconVec accomplishBtnIcon = *(CanvasIconVec*) param;
//			if(accomplishBtnIcon[m_index].second != "1") return;
//
//			for (int k=0; k<accomplishBtnIcon.size(); k++)
//			{
//				if (accomplishBtnIcon[k].second == "1" && accomplishBtnIcon[k+1].second != "1")
//				{
//					ButtonVec.at(k+2)->select(true);
//					
//					break;
//				}
//			}
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJRecvAchievePointMethod
//
/////////////////////////////////////////
crJXJRecvAchievePointMethod::crJXJRecvAchievePointMethod():
	m_this(NULL),
	m_achievepoint(0)
{
}

crJXJRecvAchievePointMethod::crJXJRecvAchievePointMethod(const crJXJRecvAchievePointMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_achievepoint(0)
{
}

void crJXJRecvAchievePointMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_achievepoint = LOINT64(param64);
		}
		else
		{
			m_achievepoint = 0;
		}
		break;
	}
}

void crJXJRecvAchievePointMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvAchievePointMethod::operator()(crHandle &handle)
{
	if(m_achievepoint!=0)
	{	
		void *param;
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crData *gameData = m_this->getDataClass();
			gameData->excHandle(MAKEINT64(WCH_LockData,1));
			gameData->getParam(WCHDATA_JXJAchievePointsCount,param);
			int *AchievePoint = (int *)param;
			INTLIMITADD(*AchievePoint,m_achievepoint,INT_MAX)

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(*AchievePoint);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvAchievePoint,stream.get());
			if (playerData.valid())
			{
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
			gameData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvAchievePointMethod
//
/////////////////////////////////////////
crJXJClientRecvAchievePointMethod::crJXJClientRecvAchievePointMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvAchievePointMethod::crJXJClientRecvAchievePointMethod(const crJXJClientRecvAchievePointMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvAchievePointMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJClientRecvAchievePointMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvAchievePointMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int achievepoint = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJAchievePointsCount,param);
				int extra = achievepoint-(*(int *)param);
				data->inputParam(WCHDATA_JXJAchievePointsCount,&achievepoint);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_AchievePoint,extra));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJListCtrlScrollExMethod
//
/////////////////////////////////////////
crJXJListCtrlScrollExMethod::crJXJListCtrlScrollExMethod()
{
}

crJXJListCtrlScrollExMethod::crJXJListCtrlScrollExMethod(const crJXJListCtrlScrollExMethod& handle):
	crMethod(handle),
	m_sw(handle.m_sw),
	m_srollbar1(handle.m_srollbar2),
	m_srollbar2(handle.m_srollbar2),
	m_type(handle.m_type)
{
}

void crJXJListCtrlScrollExMethod::inputParam(int i, void *param)
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

void crJXJListCtrlScrollExMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_sw = str;
		break;
	case 1:
		m_srollbar1 = str;
		break;
	case 2:
		m_srollbar2 = str;
		break;
	case 3:
		m_type = atoi(str.c_str());
		break;
	}
}

void crJXJListCtrlScrollExMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		ref_ptr<crMultiSwitch> zonglansw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		if(zonglansw.valid())
		{
			ref_ptr<crScrollBarWidgetNode> vScollbar = NULL;
			if(zonglansw->getActiveSwitchSet() == 0)
				vScollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_srollbar1));
			else if(zonglansw->getActiveSwitchSet() == 1)
				vScollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_srollbar2));

			if (vScollbar.valid())
			{
				if (!m_type)
				{
					vScollbar->setValue(vScollbar->getValue() - vScollbar->getLineValue());
				}
				else
				{
					vScollbar->setValue(vScollbar->getValue() + vScollbar->getLineValue());
				}
			}
		}
	}
}


///////////////////////////////////////////
////
////crJXJUIAchieveShopCommodityTipsMethod
////
///////////////////////////////////////////
crJXJUIAchieveShopCommodityTipsMethod::crJXJUIAchieveShopCommodityTipsMethod():
	m_ea(NULL),
	m_this(NULL),
	//m_ifexpand(false),
	//m_expandnum(1.0f),
	m_imageindex(0)
{
}
crJXJUIAchieveShopCommodityTipsMethod::crJXJUIAchieveShopCommodityTipsMethod(const crJXJUIAchieveShopCommodityTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJUIAchieveShopCommodityTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJUIAchieveShopCommodityTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemicon = str;
		break;
	case 4:
		m_canvas = str;
		break;
	case 5:
		m_infocanvas = str;
		break;
	case 6:
		m_infobk = str;
		break;
	case 7:
		m_infodi = str;
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 9:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 10:
		m_imageindex = atoi(str.c_str());
		break;
	}
}
void crJXJUIAchieveShopCommodityTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> parentcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid() && parentcanas.valid())
	{
		ref_ptr<crTableIO> shoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAchieveShopTab);
		int cRow = shoptab->getRowCount();
		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(parentcanas->getWidget(m_canvas));
		int val = 0;
		int itemtype = 0;
		std::string shulingstr;
		if (scroll.valid())
			val = scroll->getValue();
		int id = m_imageindex+5*val;
		if (id < cRow)
		{
			itemtype = atoi(shoptab->getData(id,"类型").c_str());
			shulingstr= shoptab->getData(id,"数量");
		}

		if(itemtype == 0) return;
		if (itemtype != 10)
		{
			int count = atoi(shulingstr.c_str());
			std::string tipsstring;
			switch (itemtype)
			{
			case T_Food:
				tipsstring = crArgumentParser::appItoa(count) + "粮食";
				break;
			case T_Wood:
				tipsstring = crArgumentParser::appItoa(count) + "木材";
				break;
			case T_Iron:
				tipsstring = crArgumentParser::appItoa(count) + "铁矿";
				break;
			case T_Horse:
				tipsstring = crArgumentParser::appItoa(count) + "马匹";
				break;
			case T_Copper:
				tipsstring = crArgumentParser::appItoa(count) + "铜钱";
				break;
			case T_Exploit:
				tipsstring = crArgumentParser::appItoa(count) + "功勋";
				break;
			case T_Exp:
				tipsstring = crArgumentParser::appItoa(count) + "经验";
				break;
			case T_Giftgold:
				tipsstring = crArgumentParser::appItoa(count) + "礼金";
				break;
			case T_VIPExp:
				tipsstring = "VIP经验，可用于升级VIP等级。";
				break;
			default:
				break;
			}
			ref_ptr<crCanvasNode>tips = crFilterRenderManager::getInstance()->findCanvas("Tips1");
			if (tips.valid() && !tipsstring.empty())
			{
				ref_ptr<crStaticTextWidgetNode> tipsText = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget("TipsText1"));
				if (tipsText.valid())
				{
					tipsText->setString(tipsstring);
					tipsText->setColor(FCOLOR_BLACK);
					const crBoundingBox &btnbox = m_this->getBoundBox();
					//
					const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
					float posx = btnbox.m_min[0];
					float posy = (btnbox.m_min[1] + 1.5f*(btnbox.m_max[1] - btnbox.m_min[1]));
					//
					crVector2 mouse(crVector2(posx, posy));
					tips->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f) * mat);
					crFilterRenderManager::getInstance()->showCanvas(tips.get(), true);
				}
			}
		}
		else
		{
			crVector3i count;
			crArgumentParser::appAtoVec(shulingstr,count);
			int itemid = count[0];
			crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
			crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
			crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
			crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
			crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
			if (name)
			{
				name->clearString();
			}
			if (info2)
			{
				info2->clearText();
			}
			if (itemicon)
			{
				itemicon->clearImage();
			}

			crVector4f colormode[5];//white,green,blue,purple,orange
			ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
			crTableIO::StrVec colorrecord;
			int qualityindex = qualitytab->getTitleIndex("color");
			for (int i =0; i<5;i++)
			{
				if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
				{
					if (qualityindex>=0)
					{
						crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
						colormode[i] /= 255.0f;
					}
				}
			}

			crTableIO::StrVec record;
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int iconindex = itemtab->getTitleIndex("icon");
			int nameindex = itemtab->getTitleIndex("name");
			int colorindex = itemtab->getTitleIndex("品质");

			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
			{
				int colormodeid = atoi(record[colorindex].c_str());
				if(name)
				{
					name->setColor(colormode[colormodeid]);
					name->setString(record[nameindex]);
				}
				if (itemicon)
				{
					itemicon->setImageName(record[iconindex]);
				}
				if(info2)
				{
					ItemMap itemMap;
					rcfg::ConfigScript outscript(&itemMap);
					crVector2i item(itemid,0);
					playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
					info2->setHypertext(outscript);
				}

				///////////计算缩放
				float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
				info2->updateData();
				const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
				const crBoundingBox &bbox2 = info2->getBoundBox();
				float texheight = 0.0f;
				float needmove = 0.0f;
				if(bbox.valid())
				{
					texheight = bbox.yLength();
					texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
					if(texheight<boxYlength) texheight = boxYlength;
					float scale = texheight/boxYlength;

					float needmove ;
					const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
					crVector3 centerTansTar = info2->getBound().center();

					float tmpLength = bbox2.yLength();
					if(infodi)
					{
						float diYlength = infodi->getChild(0)->getBoundBox().yLength();
						//centerTansTar[1] += 0.5f * (tmpLength - texheight);
						centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
					}
					else
						centerTansTar[1] += 0.5f * (tmpLength - texheight);
					// 源中心点
					crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
					// 将源中心点平移到原点
					crMatrix put_mat = crMatrix::translate(centerTansSrc);
					crMatrix inv_put = crMatrix::inverse(put_mat);
					crMatrix rotmat;
					rotmat.postMult(inv_put);
					rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
					rotmat.postMult(crMatrix::translate(centerTansTar));
					if(infobk)infobk->setMatrix(rotmat);

					if(infodi)
					{
						float diYlength = infodi->getChild(0)->getBoundBox().yLength();
						//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
						//needmove = boxYlength - texheight;
						needmove = boxYlength - texheight + diYlength * 0.25f;
						infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
					}
				}
				else
				{
					infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
					if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
				}

				/////////////////////////////////////////
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				const crBoundingBox &iconbox = m_this->getBoundBox();
				const crBoundingBox &tips = tipsCanvas->getBoundBox();
				float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
				float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
				/////////////////////////////////////////
				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
				pos = pos * mat;
				pos2 = pos2 * mat;
				float posx2,posy2;
				const crBoundingBox &infos = infobk->getBoundBox();
				float posinfox = (infos.m_max[0]-infos.m_min[0]);
				float posinfoy = (infos.m_max[1]-infos.m_min[1]);
				if (pos2[0] >= 0)
				{
					posx2 = pos2[0] - posx;
				}
				else
				{
					posx2 = pos[0] + posx;
				}
				if (pos2[1] >= 0)
				{
					posy2 = pos[1] - posy;
				}
				else
				{
					posy2 = pos2[1] + 2.0f * posy - needmove;
				}
				if((abs(posy2)+posy)>1.0f)
				{
					if (posy2 >= 0)
					{
						posy2 -= posy;
					}
					else
					{
						posy2 += posy;
					}
				}
				crVector2 mouse(crVector2(posx2,posy2));
				/////////////////////////////////////////
				tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
				tipsCanvas->setCanFocus(false);
				infocanvas->setCanFocus(false);
				infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

				crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
			}
		}
	}
}

/************************************************************************/
/*    crJXJXinShouGuoZhanJieSuanMethod                                                                  */
/************************************************************************/
JXJ::crJXJXinShouGuoZhanJieSuanMethod::crJXJXinShouGuoZhanJieSuanMethod():
	m_this(NULL),
	m_playerid(0),
	m_fubenid(0)
	//m_stream(NULL),
	//m_fuBenId(0)
{

}

JXJ::crJXJXinShouGuoZhanJieSuanMethod::crJXJXinShouGuoZhanJieSuanMethod( const crJXJXinShouGuoZhanJieSuanMethod & handle ):
	crMethod(handle),
	m_playerid(handle.m_playerid),
	m_fubenid(handle.m_fubenid)
	//m_stream(handle.m_stream),
	//m_fuBenId(handle.m_fuBenId)
{
}

void JXJ::crJXJXinShouGuoZhanJieSuanMethod::inputParam( int i, void *param )
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_playerid = (int)LOINT64(param64);
			m_fubenid = (int)HIINT64(param64);
		}
		else
		{
			m_playerid = 0;
			m_fubenid = 0;
			//m_fuBenId = 0;
			//m_stream = NULL;
		}
		break;
	}
}

void JXJ::crJXJXinShouGuoZhanJieSuanMethod::addParam( int i, const std::string& str )
{

}

void JXJ::crJXJXinShouGuoZhanJieSuanMethod::operator()( crHandle &handle )
{
	if (m_playerid != 0)
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if (m_this && gameServer && m_this->getDataClass())
		{
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				crPlayerGameData *playerGameData = playerData->getPlayerGameData();
				crData * data = playerGameData->getDataClass();
				//void *param;

				ref_ptr<crTableIO> fubenrewardtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
				int expindex = fubenrewardtab->getTitleIndex("经验");
				int exploitindex = fubenrewardtab->getTitleIndex("功勋");
				std::vector<std::string> record;
				if (fubenrewardtab->queryOneRecord(0,crArgumentParser::appItoa(m_fubenid),record) >= 0)
				{
					int experencec = atoi(record[expindex].c_str());
					int exploitc = atoi(record[exploitindex].c_str());
					crRole *mainRole = playerGameData->getMainRole();
					if (mainRole)
					{
						mainRole->doEvent(WCH_JXJRecvExperience,MAKEINT64(experencec,NULL));
						mainRole->doEvent(WCH_JXJRecvExploit,MAKEINT64(exploitc,NULL));

						CRCore::ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(8);
						stream->_writeInt(experencec);
						stream->_writeInt(exploitc);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_playerid,WCH_JXJClientDisposeNewHandlGuoZhan,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvXinShouGuoZhanJieSuanMethod
//
/////////////////////////////////////////
crJXJRecvXinShouGuoZhanJieSuanMethod::crJXJRecvXinShouGuoZhanJieSuanMethod():
	m_netType(GameClient_Game){}
crJXJRecvXinShouGuoZhanJieSuanMethod::crJXJRecvXinShouGuoZhanJieSuanMethod(const crJXJRecvXinShouGuoZhanJieSuanMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_frameTitle(handle.m_frameTitle)
{
}
void crJXJRecvXinShouGuoZhanJieSuanMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvXinShouGuoZhanJieSuanMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_frameTitle = str;
		break;
	case 2:
		m_settleList = str;
		break;
	case 3:
		m_Title1 = str;
		break;
	case 4:
		m_Title2 = str;
		break;
	case 5:
		m_Title3 = str;
		break;
	case 6:
		m_Title4 = str;
		break;
	case 7:
		m_count1 = str;
		break;
	case 8:
		m_count2 = str;
		break;
	case 9:
		m_count3 = str;
		break;
	case 10:
		m_count4 = str;
		break;
	case 11:
		m_switch = str;
		break;
	case 12:
		m_charactersize = atof(str.c_str());
		break;
	case 13:
		m_sizex = atof(str.c_str());
		break;
	case 14:
		m_sizey = atof(str.c_str());
		break;
	case 15:
		m_linespacing = atof(str.c_str());
		break;
	case 16:
		m_underlinespacing = atof(str.c_str());
		break;
	case 17:
		m_uplinespacing = atof(str.c_str());
		break;
	case 18:
		m_list[0] = str;
		break;
	case 19:
		m_list[1] = str;
		break;
	case 20:
		m_list[2] = str;
		break;
	case 21:
		m_list[3] = str;
		break;
	case 22:
		m_list[4] = str;
		break;
	}
}

void crJXJRecvXinShouGuoZhanJieSuanMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int experencec = m_stream->_readInt(); 
			int exploitc = m_stream->_readInt(); 
			crData *playerData = m_this->getDataClass();
			ref_ptr<crCanvasNode >canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid() && playerData)
			{
				ref_ptr<crStaticTextWidgetNode> frameTitle = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_frameTitle));
				ref_ptr<crMultiSwitch> mulitSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
				if(mulitSwitch.valid())
					mulitSwitch->setActiveSwitchSet(1);

				frameTitle->setAlignment(CRText::crText::CENTER_CENTER);
				frameTitle->setColor(crVector4(255.0,255.0,255.0,255.0));

				ref_ptr<crTableWidgetNode> settleList = dynamic_cast<crTableWidgetNode *>(canvas->getChildNode(m_settleList));
				ref_ptr<crListBoxWidgetNode> listItems=NULL;
				for (int i =0;i<5;i++)
				{
					listItems = dynamic_cast<crListBoxWidgetNode *>(canvas->getWidget(m_list[i]));
					if (listItems.valid())
					{
						listItems->setCharacterSize(m_charactersize);
						listItems->setFontSize(crVector2f(m_sizex,m_sizey));
						listItems->setLineSpacing(m_linespacing);
						listItems->setUnderLineSpacing(m_underlinespacing);
						listItems->setUpLineSpacing(m_uplinespacing);//
					}
				}

				crTableIO::StrVec record;
				playerData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myshiliid = *(unsigned char *)param;
				ref_ptr<crTableIO> playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				std::string name = m_this->getCharacterName();
				unsigned short index = 1;
				settleList->addData(0,index-1,crArgumentParser::appItoa(index));
				settleList->addData(1,index-1,name);
				if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(myshiliid),record)>=0)
				{
					int shilinameindex = playershilitab->getTitleIndex("name");
					settleList->addData(2,index-1,record[shilinameindex]);
				}
				settleList->addData(3,index-1,crArgumentParser::appItoa(100));
				//settleList->addData(4,index-1,strInfo);

				ref_ptr<crStaticTextWidgetNode> count1 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getChildNode(m_count1));
				ref_ptr<crStaticTextWidgetNode> count2 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getChildNode(m_count2));
				ref_ptr<crStaticTextWidgetNode> count3 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getChildNode(m_count3));
				ref_ptr<crStaticTextWidgetNode> count4 = dynamic_cast<crStaticTextWidgetNode *>(canvas->getChildNode(m_count4));

				if(count1.valid())
					count1->setString(std::string("0"));
				if(count2.valid())
					count2->setString(std::string("0"));
				if(count3.valid())
					count3->setString(std::string("0"));
				if(count4.valid())
					count4->setString(std::string("0"));

				std::string strtex = "死兵经验："+ crArgumentParser::appItoa(experencec);
				if(count1.valid())
					count1->setString(strtex);
				//if(count2.valid())
					//count2->setString("0");
				if(count3.valid())
					count3->setString(crArgumentParser::appItoa(exploitc));
				//if(count4.valid())
					//count4->setString(crArgumentParser::appItoa(vec3[2]));

				ref_ptr<CRPhysics::crCamera>camera = CRPhysics::crCameraManager::getInstance()->getMainCamera();
				if(camera.valid())
				{
					crData *camadata = camera->getAttachedNode()->getDataClass();
					playerData->getParam(WCHDATA_JXJVipLv,param);
					unsigned char vipLevel = *(unsigned char *)param;
					short timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJExitTimerLimit,vipLevel).c_str());
					camadata->inputParam(WCHDATA_JXJTimer,&timer);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckCanvasStatusOfShowMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJCheckCanvasStatusOfShowMethod::crJXJCheckCanvasStatusOfShowMethod()
{}
crJXJCheckCanvasStatusOfShowMethod::crJXJCheckCanvasStatusOfShowMethod(const crJXJCheckCanvasStatusOfShowMethod& handle)
{

}
void crJXJCheckCanvasStatusOfShowMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJCheckCanvasStatusOfShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJCheckCanvasStatusOfShowMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	bool isvisiable = false;
	if (canvas.valid())
		isvisiable = canvas->getVisiable();
		
	handle.outputParam(0,&isvisiable);
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveButtonShowMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJAchieveButtonShowMethod::crJXJAchieveButtonShowMethod()
{}
crJXJAchieveButtonShowMethod::crJXJAchieveButtonShowMethod(const crJXJAchieveButtonShowMethod& handle)
{

}
void crJXJAchieveButtonShowMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	case 2:
		if (param)
		{
			m_param = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(m_param));
		}
		else
		{
			m_ea = NULL;
			m_param = NULL;
		}
		break;
	}
}
void crJXJAchieveButtonShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	case 2:
		m_scroll = str;
		break;
	case 3:
		m_chatCanvas = str;
		break;
	case 4:
		m_chatInput = str;
		break;
	}
}

void crJXJAchieveButtonShowMethod::operator()(crHandle &handle)
{
	if (!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_SHIFT)) return;

	ref_ptr<crCanvasNode> canvas =  crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode> chatcanvas =  crFilterRenderManager::getInstance()->findCanvas(m_chatCanvas);
	if(!chatcanvas) return;
	ref_ptr<crEditWidgetNode>  chatInput = dynamic_cast<crEditWidgetNode *>(chatcanvas->getWidget(m_chatInput));
	if(!chatInput) return;
	crData *chatCanvasData = chatcanvas->getDataClass();
	if(!chatCanvasData) return;

	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		ref_ptr<crData> canvasdata = canvas->getDataClass();
		
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(!myPlayer) return;

		int achieveid = 0;
		int step = 0;
		std::string achievenameStr;
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		ref_ptr<crTableIO> picture_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishPictureTab);
		int pictureTabIdIndex = picture_table->getTitleIndex("id");
		int pictureTabtitleIndex = picture_table->getTitleIndex("title");
		int noteIndex = accomplish_table->getTitleIndex("备注");
		int nameIndex = accomplish_table->getTitleIndex("Name");
		int stepIndex = accomplish_table->getTitleIndex("Steps");
		int accomplishidIndex = accomplish_table->getTitleIndex("id");
		int title1Index = accomplish_table->getTitleIndex("title1");
		int title2Index = accomplish_table->getTitleIndex("title2");
		crTableIO::StrVec recordPicture, accomplishTabRecord;
		crTableIO::DataVec iconRecords;
		canvasdata->getParam(WCHDATA_JXJAccomplishCanvasIcon,param);
		CanvasIconVec accomplishBtnIcon = *(CanvasIconVec*) param;
		canvasdata->getParam(WCHDATA_JXJAccomplishBtnIndex,param);
		int index = *(int *)param;
		crScrollBarWidgetNode *chapterscroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scroll));
		if (index != 0)
		{
			if (accomplishBtnIcon[index].second == "1" && accomplishBtnIcon[index+1].second != "1")
				index += 1;

			if (accomplishBtnIcon[index].second == "1" && accomplishBtnIcon[index+1].second == "1")
			{
				if(picture_table->queryOneRecord(pictureTabIdIndex,accomplishBtnIcon[index].first,accomplishTabRecord) >= 0)
					accomplish_table->queryRecords(title1Index,accomplishTabRecord[pictureTabtitleIndex],iconRecords);

			}
			else
			{
				if(picture_table->queryOneRecord(pictureTabIdIndex,accomplishBtnIcon[index].first,accomplishTabRecord) >= 0)
					accomplish_table->queryRecords(title2Index,accomplishTabRecord[pictureTabtitleIndex],iconRecords);	
			}

			if (chapterscroll && iconRecords.size() > 0)
			{
				int indexid = -1;
				if (!chapterscroll->getEnable())
					indexid = m_index;	
				else
					indexid = m_index + chapterscroll->getValue();

				if (indexid >=0)
				{
					achieveid = atoi(iconRecords[indexid][accomplishidIndex].c_str());
					achievenameStr = iconRecords[indexid][nameIndex];
					int stepcount = atoi(iconRecords[indexid][stepIndex].c_str());
					if (stepcount >= 255)
					{
						crVector4i notevec;
						crArgumentParser::appAtoVec(iconRecords[indexid][noteIndex],notevec);

						ref_ptr<crData> playergdata = myPlayer->getDataClass();
						if (!playergdata.valid()) return;

						if (notevec[1] > 0)
						{
							if(notevec[2] == 0)
							{
								playergdata->getParam(notevec[1],param);
								step = *(int *)param;
							}
							else if(notevec[2] <= 4)
							{
								playergdata->getParam(notevec[1],param);
								crVector4i cbingxi = *(crVector4i *)param;
								step = cbingxi[notevec[2] - 1];
							}
							else
							{
								playergdata->getParam(notevec[1],param);
								std::map<int,CRCore::crVector3i> troopsinfomap = *(std::map<int,CRCore::crVector3i> *)param;
								std::map<int,CRCore::crVector3i>::iterator troopinfoitr = troopsinfomap.find(notevec[2]);
								if(troopinfoitr != troopsinfomap.end())
									step = troopinfoitr->second[notevec[3]];
							}
						}
					}
					else
					{
						ref_ptr<crData> mainRoleData = myPlayer->getMainRole()->getDataClass();
						if (!mainRoleData.valid()) return;

						mainRoleData->excHandle(MAKEINT64(WCH_LockData,1));
						mainRoleData->getParam(WCHDATA_JXJAccomplishMap,param);
						AccomplishMap *accomplishMap = (AccomplishMap*)param;
						AccomplishMap::iterator itr = accomplishMap->find(achieveid);
						if(itr != accomplishMap->end())
						{
							if(itr->second->getComplete())
								step = itr->second->getStepCount();
							else
								step = itr->second->getStep();
						}

						mainRoleData->excHandle(MAKEINT64(WCH_LockData,0));

					}
				}
			}
		}

		chatCanvasData->excHandle(MAKEINT64(WCH_LockData,1));
		chatCanvasData->getParam(WCHDATA_JXJShowAchieveIDVec,param);
		ShowAchieveIDVec *achieveIDVec = (ShowAchieveIDVec *)param;
		chatCanvasData->getParam(WCHDATA_JXJIsToShowAchieve,param);
		bool *isitem = (bool *)param;
		if(achieveIDVec->size()<4)
		{
			if(achieveid > 0)
			{
				achieveIDVec->push_back(std::make_pair(achieveid,step));
				*isitem = true; 
			}

			if(!achievenameStr.empty())
			{
				std::string tempStr = chatInput->getUTF8String();
				std::string itemName = std::string("[")+achievenameStr+std::string("]");
				tempStr+=itemName;
				chatInput->setString(tempStr);
				chatInput->setFocusPos(chatInput->getUTF8String().size());
			}
		}
		chatCanvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJShowAchieveLinkClickMethod
//
/////////////////////////////////////////
crJXJShowAchieveLinkClickMethod::crJXJShowAchieveLinkClickMethod():
	m_ea(NULL),
	m_this(NULL),
	m_achieveid(0),
	m_completestep(0)
{
}
crJXJShowAchieveLinkClickMethod::crJXJShowAchieveLinkClickMethod(const crJXJShowAchieveLinkClickMethod& handle):
	crMethod(handle),
	m_achievetips(handle.m_achievetips),
	m_icon(handle.m_icon),
	m_achievename(handle.m_achievename),
	m_achievemiaoshu(handle.m_achievemiaoshu),
	m_achievejindu(handle.m_achievejindu),
	m_achievepoints(handle.m_achievepoints),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJShowAchieveLinkClickMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
			m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HIINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_achieveid = *(int*)param;
		break;
	case 4:
		m_completestep = *(int*)param;
		break;
	case 5:
		m_playername = *(std::string *)param;
		break;
	}
}
void crJXJShowAchieveLinkClickMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_achievetips = str;
		break;
	case 1:
		m_icon = str;
		break;
	case 2:
		m_achievename = str;
		break;
	case 3:
		m_achievemiaoshu = str;
		break;
	case 4:
		m_achievejindu = str;
		break;
	case 5:
		m_achievepoints = str;
		break;
	}
}
void crJXJShowAchieveLinkClickMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_achievetips);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
	if(m_achieveid>0 && tipsCanvas.valid() && playerData && accomplish_table.valid())
	{
		//ref_ptr<crHypertextWidgetNode>  jinengmiaoshu = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_jinengmiaoshu));
		//ref_ptr<crImageBoxWidgetNode>  card = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_card));
		ref_ptr<crImageBoxWidgetNode>  achieveimage = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_icon));
		ref_ptr<crStaticTextWidgetNode>  achievename = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_achievename));
		ref_ptr<crStaticTextWidgetNode>  achievemiashu = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_achievemiaoshu));
		ref_ptr<crStaticTextWidgetNode>  achievejindu = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_achievejindu));
		ref_ptr<crStaticTextWidgetNode>  achievepoints = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_achievepoints));
			
		int noteIndex = accomplish_table->getTitleIndex("备注");
		int nameIndex = accomplish_table->getTitleIndex("Name");
		int stepIndex = accomplish_table->getTitleIndex("Steps");
		int pointIndex = accomplish_table->getTitleIndex("成就");
		int shuomingIndex = accomplish_table->getTitleIndex("说明");
		int ivonIndex = accomplish_table->getTitleIndex("icon");
		crTableIO::StrVec accomplishRecord;
			
		if (accomplish_table->queryOneRecord(0,crArgumentParser::appItoa(m_achieveid),accomplishRecord)>=0)
		{
			if(achieveimage.valid())
				achieveimage->setImageName(accomplishRecord[ivonIndex]);
					
			if(achievename.valid())
				achievename->setString(accomplishRecord[nameIndex]);
				
			if(achievemiashu.valid())
				achievemiashu->setString(accomplishRecord[shuomingIndex]);
				
			if(achievepoints.valid())
				achievepoints->setString(accomplishRecord[pointIndex]);
			
			int stepcount = atoi(accomplishRecord[stepIndex].c_str());
			std::string jindu;
			crVector4i notevec;
			crArgumentParser::appAtoVec(accomplishRecord[noteIndex],notevec);
			if (stepcount < 255)
			{
				if(m_completestep >= stepcount)
					jindu = m_playername + "已完成该成就";
				else
					jindu = crArgumentParser::appItoa(m_completestep) + "/" + crArgumentParser::appItoa(stepcount);
			}
			else
			{
				if(m_completestep >= notevec[0])
					jindu = m_playername + "已完成该成就";
				else
					jindu = crArgumentParser::appItoa(m_completestep) + "/" + crArgumentParser::appItoa(notevec[0]);
			}

			if(achievejindu.valid())
				achievejindu->setString(jindu);

			const crBoundingBox &infobbox = tipsCanvas->getBoundBox();
			crVector3f mouse(m_ea->getXnormalized()+infobbox.xLength()*0.5 ,m_ea->getYnormalized()+infobbox.yLength()*0.5,0.0f);
			tipsCanvas->setPosition(mouse);
			tipsCanvas->setCanFocus(false);
			tipsCanvas->setCanCaptureMouse(true);
			crFilterRenderManager::getInstance()->doModal(tipsCanvas.get());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJAchieveShopBuyButtonTipsMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJAchieveShopBuyButtonTipsMethod::crJXJAchieveShopBuyButtonTipsMethod()
{}
crJXJAchieveShopBuyButtonTipsMethod::crJXJAchieveShopBuyButtonTipsMethod(const crJXJAchieveShopBuyButtonTipsMethod& handle)
{

}
void crJXJAchieveShopBuyButtonTipsMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJAchieveShopBuyButtonTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tipcanvas = str;
		break;
	case 1:
		m_tiptext = str;
		break;
	case 2:
		m_shopscroll = str;
		break;
	case 3:
		m_index = atoi(str.c_str());
		break;
	case 4:
		m_tipcanvas2 = str;
		break;
	case 5:
		m_tiptext2 = str;
		break;
	}
}

void crJXJAchieveShopBuyButtonTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas =  m_this->getParentCanvas();
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (canvas.valid() && myPlayer && myPlayer->getDataClass())
	{
		crRole *mainRole = myPlayer->getMainRole();
		if(mainRole && mainRole->getDataClass())
		{
			void *param;
			crData *mainRoleData = mainRole->getDataClass();
			/*mainRoleData->getParam(WCHDATA_JXJAccomplishPoint,param);
			int accpoint = *(int *)param;*/
			mainRoleData->getParam(WCHDATA_Level, param);
			unsigned char mylv = *(unsigned char *)param;

			crData *playerData = myPlayer->getDataClass();
			/*playerData->getParam(WCHDATA_JXJUsedAchievePoints,param);
			int usedpoints = *(int*)param;*/
			playerData->getParam(WCHDATA_JXJAchievePointsCount,param);
			int dailypoints = *(int*)param;

			//int havepoints = accpoint + dailypoints - usedpoints;

			int val = 0;
			ref_ptr<crScrollBarWidgetNode>  shopscroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_shopscroll));
			if (shopscroll.valid())		
				val = shopscroll->getValue();

			ref_ptr<crTableIO> achieveshoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAchieveShopTab);
			std::string danJia = "商店价格";
			int countindex = achieveshoptab->getTitleIndex("数量");
			int dengjiindex = achieveshoptab->getTitleIndex("等级");
			int typeindex = achieveshoptab->getTitleIndex("类型");
			int priceindex = achieveshoptab->getTitleIndex("商店价格");
			int buycountindex = achieveshoptab->getTitleIndex("购买次数");
			int achieveindex = achieveshoptab->getTitleIndex("成就条件");
			crTableIO::StrVec achieverecord;
			int cRow = achieveshoptab->getRowCount();
			int id = m_index+5*val;
			if(id < cRow) 
			{
				//for (int j=0;j<4;j++)
				//{
				//	achieverecord[j] = achieveshoptab->getData(id,j);
				//}
				achieverecord = achieveshoptab->getDataVec()[id];
				int shopid = atoi(achieverecord[0].c_str());
				int intprice =  atoi(achieverecord[priceindex].c_str());
				int needlv = atoi(achieverecord[dengjiindex].c_str());
				int buycount = atoi(achieverecord[buycountindex].c_str());
				int achieveid = atoi(achieverecord[achieveindex].c_str());

				std::string tiptext;
				if(dailypoints < intprice)
					tiptext = " 成就积分不足!";
				else if(mylv < needlv)
					tiptext = "需要领主等级达到" + crArgumentParser::appItoa(needlv);
				else if(buycount>0)
				{
					playerData->excHandle(MAKEINT64(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJAchieveShopMap,param);
					AchieveShopMap *achieveShopMap = (AchieveShopMap *)param;
					AchieveShopMap::iterator itr = achieveShopMap->find(shopid);
					int remaincount = itr==achieveShopMap->end()?buycount:buycount - itr->second;
					playerData->excHandle(MAKEINT64(WCH_LockData,0));

					tiptext = "剩余购买次数："+crArgumentParser::appItoa(remaincount)+"次";
				}
				bool twoline = false;
				if(achieveid>0)
				{
					mainRoleData->getParam(WCHDATA_JXJAccomplishMap,param);
					mainRoleData->excHandle(MAKEINT64(WCH_LockData,1));
					AccomplishMap *accomplishMap = (AccomplishMap*)param;
					AccomplishMap::iterator itr = accomplishMap->find(achieveid);
					if(itr == accomplishMap->end() || !itr->second->isComplete())
					{
						ref_ptr<crTableIO>accomplishTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
						crTableIO::StrVec record;
						std::string name;
						if(accomplishTab->queryOneRecord(0,crArgumentParser::appItoa(achieveid),record)>=0)
						{
							int nameindex = accomplishTab->getTitleIndex("Name");
							name = record[nameindex];
						}
						if(tiptext.empty())
						{
							tiptext = "需要完成成就["+name+"]";
						}
						else
						{
							tiptext += "\n需要完成成就["+name+"]";
							twoline = true;
						}
					}
					mainRoleData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				if (!tiptext.empty())
				{
					std::string tipcanvas = twoline?m_tipcanvas2:m_tipcanvas;
					std::string tipwidget = twoline?m_tiptext2:m_tiptext;
					ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(tipcanvas);
					if(!tips.valid()) return;
					
					ref_ptr<crStaticTextWidgetNode>  tipsText = dynamic_cast< crStaticTextWidgetNode  *>(tips->getWidget(tipwidget));

					tipsText->setString(tiptext);

					const crBoundingBox &btnbox = m_this->getBoundBox();
					const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
					float posx;
					float posy;
					if(twoline)
					{
						posx = btnbox.center()[0];
						posy = btnbox.center()[1];
					}
					else
					{
						const crBoundingBox &tipsbox = tips->getBoundBox();
						posx = btnbox.m_min[0];
						posy = (btnbox.m_min[1] - tipsbox.yLength()*0.5f);
					}
					////////////
					crVector2 mouse(crVector2(posx,posy));
					tips->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
					crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUISetFuBenScrollValueMethod
//
/////////////////////////////////////////
crJXJUISetFuBenScrollValueMethod::crJXJUISetFuBenScrollValueMethod()
{
}

crJXJUISetFuBenScrollValueMethod::crJXJUISetFuBenScrollValueMethod(const crJXJUISetFuBenScrollValueMethod& handle):
	m_canvas(handle.m_canvas),
	m_scroll(handle.m_scroll)
{
}

void crJXJUISetFuBenScrollValueMethod::inputParam(int i, void *param)
{

}

void crJXJUISetFuBenScrollValueMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_scroll = str;
		break;
	default:
		break;
	}
}

void crJXJUISetFuBenScrollValueMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		ref_ptr<crScrollBarWidgetNode> chapterscroll = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scroll));
		if(chapterscroll.valid())
			chapterscroll->setValue(0.0);
	}
}

/////////////////////////////////////////
//
//crJXJSetNewGuoZhanBirthPointMethod
//
/////////////////////////////////////////
crJXJSetNewGuoZhanBirthPointMethod::crJXJSetNewGuoZhanBirthPointMethod():
	m_this(NULL)
{
}
crJXJSetNewGuoZhanBirthPointMethod::crJXJSetNewGuoZhanBirthPointMethod(const crJXJSetNewGuoZhanBirthPointMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSetNewGuoZhanBirthPointMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJSetNewGuoZhanBirthPointMethod::addParam(int i, const std::string& str)
{
	
}
void crJXJSetNewGuoZhanBirthPointMethod::operator()(crHandle &handle)
{
	m_this = crFilterRenderManager::getInstance()->findCanvas("UI_Battleground_Xzcsd");
	if(m_this)
		crFilterRenderManager::getInstance()->doModal(m_this);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryCountryStrengthMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJQueryCountryStrengthMethod::crJXJQueryCountryStrengthMethod()
{}
crJXJQueryCountryStrengthMethod::crJXJQueryCountryStrengthMethod(const crJXJQueryCountryStrengthMethod& handle)
{

}
void crJXJQueryCountryStrengthMethod::inputParam(int i, void *param)
{
}
void crJXJQueryCountryStrengthMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryCountryStrengthMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJQueryCountryStrength,NULL);
	if(netConductor)
		netConductor->getNetManager()->sendPacket("all",packet);
	
}

//////////////////////////////////////////
//
//crJXJRecvCountryStrengthMethod
//
//////////////////////////////////////////
crJXJRecvCountryStrengthMethod::crJXJRecvCountryStrengthMethod(){}
crJXJRecvCountryStrengthMethod::crJXJRecvCountryStrengthMethod(const crJXJRecvCountryStrengthMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvCountryStrengthMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvCountryStrengthMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvCountryStrengthMethod::operator ()(crHandle &handle)
{
	void *param;
	if(m_netType == GameServer)
	{
		ref_ptr<crData> gameserdata = crServerBrainHandle::getInstance()->getDataClass();
		if (gameserdata.valid())
		{
			crJXJShili *shili;
			crData *shiliData;
			unsigned char strength[3] = {1,1,1};
			for( int i = 0; i < 3; ++ i)
			{
				gameserdata->getParam(WCHDATA_JXJShiliWei+i,param);
				shili = (crJXJShili *)param;
				shiliData = shili->getDataClass();

				if(shiliData)
				{
					shiliData->getParam(WCHDATA_JXJCountryStrength,param);
					strength[i] = *(unsigned char*)param;
				}
			}

			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(3);
				stream->_writeUChar(strength[0]);
				stream->_writeUChar(strength[1]);
				stream->_writeUChar(strength[2]);
				
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJQueryCountryStrength,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	else if(m_netType == GameClient_Game)
	{
		crVector3i strength;
		strength[0] = (int)m_stream->_readUChar();
		strength[1] = (int)m_stream->_readUChar();
		strength[2] = (int)m_stream->_readUChar();
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid() && canvas->getDataClass())
		{
			ref_ptr<crData> canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJCountryStrength,param);
			crVector3i *power = (crVector3i *)param;
			*power = strength;

			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJLunHuiCanvasUpdateNewMethod
//
/////////////////////////////////////////
crJXJLunHuiCanvasUpdateNewMethod::crJXJLunHuiCanvasUpdateNewMethod(){}
crJXJLunHuiCanvasUpdateNewMethod::crJXJLunHuiCanvasUpdateNewMethod(const crJXJLunHuiCanvasUpdateNewMethod& handle):
	crMethod(handle)
{
}
void crJXJLunHuiCanvasUpdateNewMethod::inputParam(int i, void *param)
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

void crJXJLunHuiCanvasUpdateNewMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strRadioGroup = str;
		break;
	case 1:
		m_strSwitch = str;
		break;
	case 2:
		m_strcardnote = str;
		break;
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
		m_strCardImage[i-3] = str;
		break;
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
		m_strCardQuality[i-13] = str;
		break;
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
		m_strCardStatus[i-23] = str;
		break;
	case 33:
		m_stritemnote = str;
		break;
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
		m_strItemImage[i-34] = str;
		break;
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:	
	case 52:	
	case 53:
		m_strItemStatus[i-44] = str;
		break;
	case 54:
		m_strgoldimage = str;
		break;
	case 55:
		m_strgoldnote = str;
		break;
	case 56:
		m_strgoldtext = str;
		break;
	case 57:
		m_strgoldcount = str;
		break;
	case 58:
		m_strlijinimage = str;
		break;
	case 59:
		m_strlijinnote = str;
		break;
	case 60:
		m_strlijintext = str;
		break;
	case 61:
		m_strlijincount = str;
		break;
	case 62:
		m_strDefaultCardImg = str;
		break;
	case 63:
		m_strDefaultItemImg = str;
		break;
	case 64:
		m_cunchucountcard = str;
		break;
	case 65:
		m_cunchucountitem = str;
		break;
	default:
		break;
	}
}

void crJXJLunHuiCanvasUpdateNewMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_strRadioGroup));
	crRole *mainRole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	playerData->getParam(WCHDATA_JXJVipLv,param);
	unsigned char vipLevel = *(unsigned char *)param;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	int iconIndex = itemtab->getTitleIndex("icon");
	int qualityIndex = itemtab->getTitleIndex("品质");
	crTableIO::StrVec itemrecord;
	//int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiBtVisiableLv,vipLevel).c_str());

	ref_ptr<crData> canvasData = m_this->getDataClass();
	crTableIO::StrVec record;
	ref_ptr<crTableIO>texttab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	if(radioGroup.valid() && mainRole && itemtab.valid())
	{
		crData *mainRoleData = mainRole->getDataClass();
		mainRoleData->getParam(WCHDATA_Level,param);
		unsigned char mylv = *(unsigned char *)param;
		if(radioGroup->getSelect() == 0)
		{
			crImageBoxWidgetNode * goldimgBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strgoldimage));
			ref_ptr<crStaticTextWidgetNode> goldnote = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strgoldnote));
			ref_ptr<crStaticTextWidgetNode> goldtext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strgoldtext));
			ref_ptr<crStaticTextWidgetNode> goldcount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strgoldcount));
			if(goldimgBox)
			{
				goldimgBox->setVisiable(true);
				goldimgBox->setImageName("UI_Lunhui_img.img");
			}

			int money = 0;
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			if(playerStore.valid())
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					//playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					playerStoreData->getParam(WCHDATA_Money,param);
					money = *(int *)param;
					//playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			if(goldcount.valid())
				goldcount->setString(crArgumentParser::appItoa(money));

			if(goldnote.valid() && texttab->queryOneRecord(0,crArgumentParser::appItoa(9206),record) >= 0)
				goldnote->setString(record[1]);
		}
		else if(radioGroup->getSelect() == 1)
		{
			crImageBoxWidgetNode * lijinimgBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strlijinimage));
			ref_ptr<crStaticTextWidgetNode> lijinnote = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strlijinnote));
			ref_ptr<crStaticTextWidgetNode> lijintext = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strlijintext));
			ref_ptr<crStaticTextWidgetNode> lijincount = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strlijincount));
			if(lijinimgBox)
			{
				lijinimgBox->setVisiable(true);
				lijinimgBox->setImageName("UI_Lunhui_img.img");
			}

			int giftgold = 0;
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			if(playerStore.valid())
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					playerStoreData->getParam(WCHDATA_JXJPlayerTotalAddMoney,param);
					int totalmoney = *(int *)param;
					//轮回获得礼金
					float factor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiGiftGoldFactor,0).c_str());
					giftgold = (float)(totalmoney) *factor;
				}
			}
			
			if(lijincount.valid())
				lijincount->setString(crArgumentParser::appItoa(giftgold));

			if(lijinnote.valid() && texttab->queryOneRecord(0,crArgumentParser::appItoa(9207),record) >= 0)
				lijinnote->setString(record[1]);
		}
		else if(radioGroup->getSelect() == 2)
		{
			//显示轮回将卡背包内容
			ref_ptr<crStaticTextWidgetNode> cardshuming = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strcardnote));
			ref_ptr<crStaticTextWidgetNode> cardnote = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cunchucountcard));
			if(cardnote.valid())
				cardnote->setVisiable(false);

			if(cardshuming.valid() && texttab->queryOneRecord(0,crArgumentParser::appItoa(9208),record) >= 0)
				cardshuming->setString(record[1]);

			crImageBoxWidgetNode *imageCard[10] = {NULL};
			crMultiSwitch * cardQulitySw[10] = {NULL};
			crMultiSwitch * cardstatusSw[10] = {NULL};
			for (int i= 0;i<10;i++)
			{
				imageCard[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strCardImage[i]));
				cardQulitySw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strCardQuality[i]));
				cardstatusSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strCardStatus[i]));

				if(imageCard[i])
				{
					imageCard[i]->setVisiable(true);
					imageCard[i]->setImageName(m_strDefaultCardImg);
				}
				if(cardstatusSw[i])
				{
					cardstatusSw[i]->setVisiable(false);
				}
			}
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			if(playerStore.valid())
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
					JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;

					for (int i= 0;i<10;i++)
					{
						if(imageCard[i])
						{
							if(i<cardVec->size())
							{
								imageCard[i]->setVisiable(true);
								
								if(itemtab.valid() && (*cardVec)[i].valid() && \
									itemtab->queryOneRecord(0,crArgumentParser::appItoa((*cardVec)[i]->getItemID()),itemrecord)>=0)
								{
									

									imageCard[i]->setImageName(itemrecord[iconIndex]);

									int q = atoi(itemrecord[qualityIndex].c_str());
									if(cardQulitySw[i])
									{
										cardQulitySw[i]->setActiveSwitchSet(0);
							
										cardQulitySw[i]->setActiveSwitchSet(q+1);
									}
									if(cardstatusSw[i])
									{
										cardstatusSw[i]->setVisiable(true);
										if(1 == (*cardVec)[i]->getItemHasCycle())
											cardstatusSw[i]->setActiveSwitchSet(2);
										else
											cardstatusSw[i]->setActiveSwitchSet(1);
									}
								}
							}
							else
							{
								imageCard[i]->setImageName(m_strDefaultCardImg);
								cardQulitySw[i]->setActiveSwitchSet(0);
							}
						}
					}
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}

			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
			ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
			
			if(!cardMap->empty())
			{
				for (ItemOrCardMap::iterator iter=cardMap->begin(); iter!=cardMap->end(); iter++)
				{
					int index = iter->first;
					int itemid = iter->second->getItemID();
					if(itemtab.valid() && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
					{
						if(imageCard[index])
							imageCard[index]->setImageName(itemrecord[iconIndex]);

						int q = atoi(itemrecord[qualityIndex].c_str());
						if(cardQulitySw[index])
						{
							cardQulitySw[index]->setActiveSwitchSet(0);

							cardQulitySw[index]->setActiveSwitchSet(q+1);
						}
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
		else if(radioGroup->getSelect() == 3)
		{
			ref_ptr<crStaticTextWidgetNode> itemshuming = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_stritemnote));
			ref_ptr<crStaticTextWidgetNode> itemnote = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cunchucountitem));
			if(itemnote.valid())
				itemnote->setVisiable(false);

			if(itemshuming.valid() && texttab->queryOneRecord(0,crArgumentParser::appItoa(9209),record) >= 0)
				itemshuming->setString(record[1]);

			crImageBoxWidgetNode *imageItem[10] = {NULL};
			crMultiSwitch * itemstatusSw[10] = {NULL};
			for (int i= 0;i<10;i++)
			{
				itemstatusSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strItemStatus[i]));
				imageItem[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strItemImage[i]));

				if(imageItem[i])
				{
					imageItem[i]->setVisiable(true);
					imageItem[i]->setImageName(m_strDefaultItemImg);
				}
				if(itemstatusSw[i])
				{
					itemstatusSw[i]->setVisiable(false);
				}
			}
			playerData->getParam(WCHDATA_JXJPlayerStore,param);
			ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			if(playerStore.valid())
			{
				crData *playerStoreData = playerStore->getDataClass();
				if(playerStoreData)
				{
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
					playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
					JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;

					for (int i= 0;i<10;i++)
					{
						if(imageItem[i])
						{
							if(i<itemVec->size())
							{
								imageItem[i]->setVisiable(true);
								
								if(itemtab.valid() && (*itemVec)[i].valid() && \
									itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itemVec)[i]->getItemID()),itemrecord)>=0)
								{
									
									//int qualityIndex = itemtab->getTitleIndex("品质");

									imageItem[i]->setImageName(itemrecord[iconIndex]);
									if(itemstatusSw[i])
									{
										itemstatusSw[i]->setVisiable(true);
										if(1 == (*itemVec)[i]->getItemHasCycle())
											itemstatusSw[i]->setActiveSwitchSet(2);
										else
											itemstatusSw[i]->setActiveSwitchSet(1);
									}
								}
							}
							else
							{
								imageItem[i]->setImageName(m_strDefaultItemImg);
							}
						}
					}
					playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
			
			canvasData->excHandle(MAKEINT64(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
			ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
			if(!itemMap->empty())
			{
				for (ItemOrCardMap::iterator iter=itemMap->begin(); iter!=itemMap->end(); iter++)
				{
					int index = iter->first;
					int itemid = iter->second->getItemID();
					if(itemtab.valid() && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
					{
						if(imageItem[index])
							imageItem[index]->setImageName(itemrecord[iconIndex]);
					}
				}
			}
			canvasData->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckLunHuiDaoJuCountMethod
//检查背包里是否有
//
//////////////////////////////////////////////////////////////////////////
crJXJUICheckLunHuiDaoJuCountMethod::crJXJUICheckLunHuiDaoJuCountMethod():m_itemid(0)
{}
crJXJUICheckLunHuiDaoJuCountMethod::crJXJUICheckLunHuiDaoJuCountMethod(const crJXJUICheckLunHuiDaoJuCountMethod& handle)
{

}
void crJXJUICheckLunHuiDaoJuCountMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUICheckLunHuiDaoJuCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_itemid = atoi(str.c_str());
		break;
	}
}

void crJXJUICheckLunHuiDaoJuCountMethod::operator()(crHandle &handle)
{
	bool isdaojuenough = false;
	int count = 0;
	int tosavecount = 0;

	void *param;
	crTableIO::StrVec record;
	ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	int typeindex = itemtab->getTitleIndex("类型");

	ref_ptr<crData> mainroleData = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole()->getDataClass();
	mainroleData->getParam(WCHDATA_Level,param);
	unsigned char mylv = *(unsigned char *)param;
	int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPlayerLunHuiCunChuDengJi).c_str());

	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	playerData->getParam(WCHDATA_JXJItemBagVec,param);
	JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;

	if (m_itemid > 0)
	{
		for (int i=0;i<itemBagVec->size();i++)
		{
			if((*itemBagVec)[i].valid())
			{
				if ((*itemBagVec)[i]->getItemID() == m_itemid)
				{
					count += (*itemBagVec)[i]->getItemCount();
				}
			}
		}
	} 

	bool isotherthing = false;
	ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
	if (canvas.valid()&&canvas->getDataClass())
	{
		crData *canvasData = canvas->getDataClass();
		
		if(m_itemid == 7525)
		{
			canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
			ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
			tosavecount = cardMap->size();
			if (tosavecount > 0)
			{
				for (ItemOrCardMap::iterator iter = cardMap->begin(); iter != cardMap->end();iter++)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*iter).second->getItemID()),record) >= 0)
						if(atoi(record[typeindex].c_str()) != 6)
						{
							isotherthing = true;
							break;
						}
				}
			}
		}
		else
		{
			canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
			ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
			tosavecount = itemMap->size();
			if (tosavecount > 0)
			{
				for (ItemOrCardMap::iterator iter = itemMap->begin(); iter != itemMap->end();iter++)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*iter).second->getItemID()),record) >= 0)
						if(atoi(record[typeindex].c_str()) != 1)
						{
							isotherthing = true;
							break;
						}
				}
			}
		}
		if(count >= tosavecount)
			isdaojuenough = true;
			
	}

	bool iscansave = true;
	if(!isdaojuenough || mylv < openlv || tosavecount == 0 || isotherthing)
	{
		int texid = 0;
		if(mylv < openlv)
			texid = 9204;
		else if(tosavecount == 0)
			texid = 9198;
		else if(isotherthing)
			texid = 9205;
		else
			texid = 9197;

		iscansave = false;

		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		int mode = 1;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);	
	}
	handle.outputParam(0,&iscansave);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJConfirmUseLunHuiDaoJuMethod
//轮回道具使用确认
//
//////////////////////////////////////////////////////////////////////////
crJXJConfirmUseLunHuiDaoJuMethod::crJXJConfirmUseLunHuiDaoJuMethod():m_index(0)
{}
crJXJConfirmUseLunHuiDaoJuMethod::crJXJConfirmUseLunHuiDaoJuMethod(const crJXJConfirmUseLunHuiDaoJuMethod& handle)
{

}
void crJXJConfirmUseLunHuiDaoJuMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJConfirmUseLunHuiDaoJuMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJConfirmUseLunHuiDaoJuMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(canvas.valid())
	{
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			void *param;
			unsigned char tosavecount = 0;
			if(m_index == 0)
			{
				canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
				ItemOrCardMap *cardMap = (ItemOrCardMap *)param;
				tosavecount = cardMap->size();
			}
			else
			{
				canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
				ItemOrCardMap *itemMap = (ItemOrCardMap *)param;
				tosavecount = itemMap->size();
			}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			stream->_writeUChar((unsigned char)m_index);
			stream->_writeUChar(tosavecount);

			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUseLunHuiDaoJU,stream.get());
			if(netConductor)
				netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

//////////////////////////////////////////
//
//crJXJRecvConfirmUseLunHuiDaoJuMethod
//
//////////////////////////////////////////
crJXJRecvConfirmUseLunHuiDaoJuMethod::crJXJRecvConfirmUseLunHuiDaoJuMethod(){}
crJXJRecvConfirmUseLunHuiDaoJuMethod::crJXJRecvConfirmUseLunHuiDaoJuMethod(const crJXJRecvConfirmUseLunHuiDaoJuMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvConfirmUseLunHuiDaoJuMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvConfirmUseLunHuiDaoJuMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvConfirmUseLunHuiDaoJuMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			unsigned char index = m_stream->_readUChar();
			int tosavecount = (int)m_stream->_readUChar();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid())
			{
				//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				//std::vector<std::string> record;
				char success = 0;
				void *param;
				bool hasitem = false;
				int itemid = 0;
				int selectid = 0;
				int haveitemcount = 0;
				std::vector<crVector2i> selectidcount;
				crDataObject *dataObject = NULL;
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				//检查我背包里是否有这个itemid
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
				if(index == 0)
					itemid = 7525;
				else
					itemid = 7524;

				for (int i=0;i<itemBagVec->size();i++)
				{
					if((*itemBagVec)[i].valid())
					{
						if ((*itemBagVec)[i]->getItemID() == itemid)
						{
							haveitemcount = (*itemBagVec)[i]->getItemCount();
							if (haveitemcount >= tosavecount)
							{
								hasitem = true;
								selectidcount.push_back(crVector2i(i,haveitemcount-tosavecount));
								break;
							}
							else
							{
								selectidcount.push_back(crVector2i(i,0));
								tosavecount -= haveitemcount;
							}
						}
					}
				}

				if (hasitem)
				{
					/*if(dataObject)
					{*/
						//itemtab->queryOneRecord(2,"召集令",record);
						//int id = atoi(record[0].c_str());
						char useResult = 1;
						//crVector3i param(itemid,selectid,(int)(&useResult));
						//dataObject->doEvent(MAKEINT64(WCH_JXJUseBagItem,GameServer),MAKEINT64(m_this,&param));
						if(useResult == 1)
						{//使用成功，扣除背包里的道具
							unsigned char itemcount = 0;
							for (int i=0;i<selectidcount.size();i++)
							{
								if (selectidcount[i][0] < itemBagVec->size())
								{
									if((*itemBagVec)[selectidcount[i][0]].valid())
									{
										if ((*itemBagVec)[selectidcount[i][0]]->getItemID() == itemid)
										{
											if (selectidcount[i][1] > 0)
											{
												(*itemBagVec)[selectidcount[i][0]]->setItemCount(selectidcount[i][1]);
											}
											else
											{
												(*itemBagVec)[selectidcount[i][0]] = NULL;
											}
										}
									}
								}
							
								success = 1;
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(7);
								stream->_writeChar(useResult);
								stream->_writeInt(itemid);
								stream->_writeUChar((unsigned char)selectidcount[i][0]);
								stream->_writeUChar((unsigned char)selectidcount[i][1]);
								crPlayerDataEventPacket packet;
								crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUseBagItem,stream.get());
								gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
						else
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(5);
							stream->_writeChar(useResult);
							stream->_writeInt(itemid);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUseBagItem,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					//}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				///游戏日志
				std::string logdata = "使用物品（success，itemid）：" + crArgumentParser::appVectoa(crVector2i(success,itemid));
				GameLogData gamelog(Log_UseBagItem,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));	
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIQuChuLunHuiThingMethod
//轮回仓库存储确认框
//
//////////////////////////////////////////////////////////////////////////
crJXJUIQuChuLunHuiThingMethod::crJXJUIQuChuLunHuiThingMethod()
{}
crJXJUIQuChuLunHuiThingMethod::crJXJUIQuChuLunHuiThingMethod(const crJXJUIQuChuLunHuiThingMethod& handle)
{

}
void crJXJUIQuChuLunHuiThingMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUIQuChuLunHuiThingMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJUIQuChuLunHuiThingMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *data = myPlayer->getDataClass();
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		roledata->getParam(WCHDATA_Level, param);
		unsigned char characterlevel = *(unsigned char *)param;

		int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPlayerLunHuiTiQuDengJi).c_str());

		//data->getParam(WCHDATA_JXJClientIsServerCycling,param);
		//bool bCycle = *(bool *)param;

		//bool hasething = false;
		//data->getParam(WCHDATA_JXJPlayerStore,param);
		//ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
		//if(playerStore.valid())
		//{
		//	crData *playerStoreData = playerStore->getDataClass();
		//	if(playerStoreData)
		//	{
		//		playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
		//		playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
		//		JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;
		//		playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
		//		JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;
		//		for (JXJStoreItemBagVec::iterator iter=cardVec->begin();iter!=cardVec->end();iter++)
		//		{
		//			if((*iter)->getItemHasCycle())
		//			{
		//				hasething = true;
		//				break;
		//			}
		//		}
		//		if (!hasething)
		//		{
		//			for (JXJStoreItemBagVec::iterator iter=itemVec->begin();iter!=itemVec->end();iter++)
		//			{
		//				if((*iter)->getItemHasCycle())
		//				{
		//					hasething = true;
		//					break;
		//				}
		//			}
		//		}
		//		playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
		//	}
		//}

		if(characterlevel >= openlv)
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvQuChuLunHuiThing,NULL);
			if(netConductor)
				netConductor->getNetManager()->sendPacket("all",packet);
				
		}
		else
		{
			int texid = 9200;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			int mode = 1;
			noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
			noticeHandle->inputParam(WCHDATA_NoticeTextID, &texid);
			std::string param1 = crArgumentParser::appItoa(openlv);
			noticeHandle->inputParam(WCHDATA_NoticeParam1, &param1);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
		}
	}
}

//////////////////////////////////////////
//
//crJXJRecvQuChuLunHuiTingMethod
//
//////////////////////////////////////////
crJXJRecvQuChuLunHuiTingMethod::crJXJRecvQuChuLunHuiTingMethod(){}
crJXJRecvQuChuLunHuiTingMethod::crJXJRecvQuChuLunHuiTingMethod(const crJXJRecvQuChuLunHuiTingMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvQuChuLunHuiTingMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvQuChuLunHuiTingMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvQuChuLunHuiTingMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			void *param;
			crData *data = m_this->getDataClass();
			crData *roledata = m_this->getMainRole()->getDataClass();
			roledata->getParam(WCHDATA_Level, param);
			unsigned char characterlevel = *(unsigned char *)param;

			int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPlayerLunHuiTiQuDengJi).c_str());

			//bool hasething = false;
			//data->getParam(WCHDATA_JXJPlayerStore,param);
			//ref_ptr<crJXJPlayerStore> playerStore = (crJXJPlayerStore *)param;
			//if(playerStore.valid())
			//{
			//	crData *playerStoreData = playerStore->getDataClass();
			//	if(playerStoreData)
			//	{
			//		playerStoreData->excHandle(MAKEINT64(WCH_LockData,1));
			//		playerStoreData->getParam(WCHDATA_JXJJiangkaBagVec,param);
			//		JXJStoreItemBagVec *cardVec = (JXJStoreItemBagVec *)param;
			//		playerStoreData->getParam(WCHDATA_JXJItemBagVec,param);
			//		JXJStoreItemBagVec *itemVec = (JXJStoreItemBagVec *)param;
			//		for (JXJStoreItemBagVec::iterator iter=cardVec->begin();iter!=cardVec->end();iter++)
			//		{
			//			if((*iter)->getItemHasCycle())
			//			{
			//				hasething = true;
			//				break;
			//			}
			//		}
			//		if (!hasething)
			//		{
			//			for (JXJStoreItemBagVec::iterator iter=itemVec->begin();iter!=itemVec->end();iter++)
			//			{
			//				if((*iter)->getItemHasCycle())
			//				{
			//					hasething = true;
			//					break;
			//				}
			//			}
			//		}
			//		playerStoreData->excHandle(MAKEINT64(WCH_LockData,0));
			//	}
			//}

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			if (openlv == -1)
			{
				stream->_writeChar(-1);
			}
			else if(/*hasething && */characterlevel>=openlv)
			{
				m_this->doEvent(WCH_JXJGetPlayerGameStore);
				stream->_writeChar(1);
			}
			else
			{
				stream->_writeChar(0);
				stream->_writeUChar(openlv);
			}

			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
			if(playerdata.valid())
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvQuChuLunHuiThing,stream.get());
				gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char _case = m_stream->_readChar();
			if(_case == 1)
			{
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_XLunhui");
				if(canvas.valid())
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);
			}
			else if (_case == -1)
			{
				int texid = 1125;
				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				int mode = 1;
				noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
				noticeHandle->inputParam(WCHDATA_NoticeTextID, &texid);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			}
			else if (_case == 0)
			{
				unsigned char openlv = m_stream->_readUChar();
				int texid = 9200;
				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				int mode = 1;
				noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
				noticeHandle->inputParam(WCHDATA_NoticeTextID, &texid);
				std::string param1 = crArgumentParser::appItoa(openlv);
				noticeHandle->inputParam(WCHDATA_NoticeParam1, &param1);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJUICheckCharacterLevelMethod
//检查领主等级
//
//////////////////////////////////////////////////////////////////////////
crJXJUICheckCharacterLevelMethod::crJXJUICheckCharacterLevelMethod()
{}
crJXJUICheckCharacterLevelMethod::crJXJUICheckCharacterLevelMethod(const crJXJUICheckCharacterLevelMethod& handle)
{

}
void crJXJUICheckCharacterLevelMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJUICheckCharacterLevelMethod::addParam(int i, const std::string& str)
{
}

void crJXJUICheckCharacterLevelMethod::operator()(crHandle &handle)
{
	void *param;
	bool isdaojuenough = false;
	
	crData *roledata = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole()->getDataClass();
	roledata->getParam(WCHDATA_Level, param);
	unsigned char characterlevel = *(unsigned char *)param;

	int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJPlayerLunHuiTiQuDengJi).c_str());

	if(characterlevel >= openlv)
	{
		isdaojuenough = true;
	}
	else
	{
		int texid = 9200;
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		int mode = 1;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);	
	}
	handle.outputParam(0,&isdaojuenough);
}

/////////////////////////////////////////
//
//crJXJAdvanceLunHuiRePlaceCountMethod
//
/////////////////////////////////////////
crJXJAdvanceLunHuiRePlaceCountMethod::crJXJAdvanceLunHuiRePlaceCountMethod()
{
}
crJXJAdvanceLunHuiRePlaceCountMethod::crJXJAdvanceLunHuiRePlaceCountMethod(const crJXJAdvanceLunHuiRePlaceCountMethod& handle):
	crMethod(handle)
{
}
void crJXJAdvanceLunHuiRePlaceCountMethod::inputParam(int i, void *param)
{
	// 	switch(i) 
	// 	{
	// 	case 0:
	// 		if(param == 0)
	// 		{//释放
	// 			m_this = NULL;
	// 		}
	// 		break;
	// 	case 1:
	// 		m_this = (crCanvasNode*)param;
	// 		break;
	// 	}
}
void crJXJAdvanceLunHuiRePlaceCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	default:
		break;
	}
}
void crJXJAdvanceLunHuiRePlaceCountMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);

	crData *canvasData = canvas->getDataClass();
	if(canvasData)
	{
		canvasData->excHandle(MAKEINT64(WCH_LockData,1));

		canvasData->getParam(WCHDATA_JXJPlayerLunHuiReplacecount,param);
		char *replacecount = (char *)param;
		*replacecount += 1;

		canvasData->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJGetViplvRewardMethod
///领取vip等级礼包
//
//////////////////////////////////////////////////////////////////////////
crJXJGetViplvRewardMethod::crJXJGetViplvRewardMethod()
{}
crJXJGetViplvRewardMethod::crJXJGetViplvRewardMethod(const crJXJGetViplvRewardMethod& handle)
{

}
void crJXJGetViplvRewardMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJGetViplvRewardMethod::addParam(int i, const std::string& str)
{

}

void crJXJGetViplvRewardMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data  = playerData->getDataClass();
		if(data)
		{
			void *param;
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJPlayerHaveGotViplvReward,param);
			unsigned char havegot = *(unsigned char *)param;
			if(havegot < vipLevel)
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJPlayerGetViplvReward,NULL);
				if(netConductor)
					netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

//////////////////////////////////////////
//
//crJXJRecvGetViplvRewardMethod
//
//////////////////////////////////////////
crJXJRecvGetViplvRewardMethod::crJXJRecvGetViplvRewardMethod(){}
crJXJRecvGetViplvRewardMethod::crJXJRecvGetViplvRewardMethod(const crJXJRecvGetViplvRewardMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvGetViplvRewardMethod::inputParam(int i, void *param)
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
			_crInt64 param64 = *(_crInt64*)param;
			m_stream = (crStreamBuf *)(LOINT64(param64));
			m_netType = HIINT64(param64);
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvGetViplvRewardMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvGetViplvRewardMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		if(m_netType == GameServer)
		{
			char code = 0;
			crData *data  = m_this->getDataClass();
			void *param;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJPlayerHaveGotViplvReward,param);
			unsigned char *havegot = (unsigned char *)param;
			if(*havegot < vipLevel)
			{
				crTableIO::StrVec record;
				ref_ptr<crTableIO> viplvawardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJViplvRewardTab);
				int awardindex[5];
				awardindex[0] =  viplvawardTab->getTitleIndex("奖励1");
				awardindex[1] =  viplvawardTab->getTitleIndex("奖励2");
				awardindex[2] =  viplvawardTab->getTitleIndex("奖励3");
				awardindex[3] =  viplvawardTab->getTitleIndex("奖励4");
				awardindex[4] =  viplvawardTab->getTitleIndex("奖励5");

				if (viplvawardTab->queryOneRecord(0,crArgumentParser::appItoa(*havegot+1),record)>=0)
				{
					std::vector<CRCore::crVector4i> awards;
					crVector4i awardVec;
					for (int i = 0; i < 5; ++ i)
					{
						if (record[awardindex[i]] == "") continue;
						crArgumentParser::appAtoVec(record[awardindex[i]],awardVec);
						awards.push_back(awardVec);
					}
					sendReward(awards,m_this,"VIP等级奖励");
					(*havegot)++;
					code = 1;
				}
				//std::vector<crVector3i> bagitem;
				//crVector3i vec3;

				////for (int i = *havegot+1; i<=vipLevel; i++)
				////{
				//	if (viplvawardTab->queryOneRecord(0,crArgumentParser::appItoa(*havegot+1),record)>=0)
				//	{
				//		for (int j = 0; j < 5; j++)
				//		{
				//			crArgumentParser::appAtoVec(record[awardindex[j]],vec3);
				//			if (vec3[0]>0)
				//				bagitem.push_back(vec3);
				//		}
				//	}
				////}

				//bool canget = false;
				//int emptybag = 0;
				//data->getParam(WCHDATA_JXJItemBagVec,param);
				//JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				//if (!bagitem.empty())
				//{
				//	for (JXJItemBagVec::iterator itr = itembagvec->begin(); itr!=itembagvec->end();++itr)
				//	{
				//		if ((*itr) == NULL)
				//		{
				//			emptybag ++;
				//			if (emptybag >= bagitem.size())
				//			{
				//				canget = true;
				//				break;
				//			}
				//		}
				//	}
				//}
				//else
				//{
				//	canget = true;
				//}

				//if (canget)
				//{
				//	RewardItemVec rewardItems;
				//	ref_ptr<crBagItemData> itemdata;
				//	unsigned char type = 0;
				//	for (std::vector<crVector3i>::iterator vitr = bagitem.begin(); vitr != bagitem.end(); ++vitr)
				//	{
				//		itemdata = new crBagItemData;
				//		itemdata->setItemID((*vitr)[0]);
				//		itemdata->setEquipMagic((*vitr)[1]);
				//		itemdata->setItemCount((*vitr)[2]);
				//		rewardItems.push_back(itemdata);
				//	}

				//	if(!rewardItems.empty())
				//		m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,type));

				//	(*havegot)++;

				//	code = 1;
				//}
				//else
				//{
				//	code = 0;
				//}

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeChar(code);
				if(code == 1)
					stream->_writeUChar(*havegot);

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
				crNetDataManager *netDataManager = gameServer->getNetDataManager();
				ref_ptr<crGameServerPlayerData> playerdata = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
				if(playerdata.valid())
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJPlayerGetViplvReward,stream.get());
					gameServer->getNetManager()->sendPacket(playerdata->getPlayerConnectServerAddress(),packet);
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));

			if(code == 1)
			{
				std::string logdata = "领取VIP等级奖励："+crArgumentParser::appItoa(*havegot);
				GameLogData gamelog(Log_GetViplvReward, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKEINT64(m_this->getPlayerID(), &gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crData *data  = m_this->getDataClass();
			if(data)
			{
				void *param;
				char code = m_stream->_readChar();
				if(code == 1)
				{
					unsigned char gotviplv = m_stream->_readUChar();
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char vipLevel = *(unsigned char *)param;
					data->getParam(WCHDATA_JXJPlayerHaveGotViplvReward,param);
					unsigned char *havegot = (unsigned char *)param;

					*havegot = gotviplv;
					data->excHandle(MAKEINT64(WCH_LockData,0));

					ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if(canvas.valid())
						canvas->doEvent(WCH_UI_UpdateData);
				}
				else
				{
					int texid = 9214;

					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);	
				}
			}
		}
	}
}


///////////////////////////////////////////
////
////crJXJUIViplvRewardTipsMethod
////
///////////////////////////////////////////
crJXJUIViplvRewardTipsMethod::crJXJUIViplvRewardTipsMethod():
	m_ea(NULL),
	m_this(NULL),
	//m_ifexpand(false),
	//m_expandnum(1.0f),
	m_imageindex(0)
{
}
crJXJUIViplvRewardTipsMethod::crJXJUIViplvRewardTipsMethod(const crJXJUIViplvRewardTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJUIViplvRewardTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJUIViplvRewardTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemicon = str;
		break;
	case 4:
		m_canvas = str;
		break;
	case 5:
		m_infocanvas = str;
		break;
	case 6:
		m_infobk = str;
		break;
	case 7:
		m_infodi = str;
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 9:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 10:
		m_imageindex = atoi(str.c_str());
		break;
	}
}
void crJXJUIViplvRewardTipsMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode> parentcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid() && parentcanas.valid())
	{
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		if (info2)
		{
			info2->clearText();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}

		crVector4f colormode[5];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i =0; i<5;i++)
		{
			if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
			{
				if (qualityindex>=0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int nameindex = itemtab->getTitleIndex("name");
		int colorindex = itemtab->getTitleIndex("品质");

		crData *canvasdata = parentcanas->getDataClass();
		canvasdata->getParam(WCHDATA_JXJVipCanvasCurPageNum,param);
		int curpagenum = *(int *)param;

		ref_ptr<crTableIO> viplvrewardtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJViplvRewardTab);
		crVector3i vec3i;
		int itemid;
		if (viplvrewardtab->queryOneRecord(0,crArgumentParser::appItoa(curpagenum),record) >= 0)
		{
			crArgumentParser::appAtoVec(record[1],vec3i);
			itemid = vec3i[0];
		}
			
		if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
		{
			int colormodeid = atoi(record[colorindex].c_str());
			if(name)
			{
				name->setColor(colormode[colormodeid]);
				name->setString(record[nameindex]);
			}
			if (itemicon)
			{
				itemicon->setImageName(record[iconindex]);
			}
			if(info2)
			{
				ItemMap itemMap;
				rcfg::ConfigScript outscript(&itemMap);
				crVector2i item(itemid,0);
				playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
				info2->setHypertext(outscript);
			}

			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//centerTansTar[1] += 0.5f * (tmpLength - texheight);
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(infobk)infobk->setMatrix(rotmat);

				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2,posy2;
			const crBoundingBox &infos = infobk->getBoundBox();
			float posinfox = (infos.m_max[0]-infos.m_min[0]);
			float posinfoy = (infos.m_max[1]-infos.m_min[1]);
			if (pos2[0] >= 0)
			{
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + 2.0f * posy - needmove;
			}
			if((abs(posy2)+posy)>1.0f)
			{
				if (posy2 >= 0)
				{
					posy2 -= posy;
				}
				else
				{
					posy2 += posy;
				}
			}
			crVector2 mouse(crVector2(posx2,posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIShuoMingTipInfoNewMethod
//
/////////////////////////////////////////
crJXJUIShuoMingTipInfoNewMethod::crJXJUIShuoMingTipInfoNewMethod():
	m_ea(NULL),
	m_this(NULL)
{
}
crJXJUIShuoMingTipInfoNewMethod::crJXJUIShuoMingTipInfoNewMethod(const crJXJUIShuoMingTipInfoNewMethod& handle):
	crMethod(handle)
{
}
void crJXJUIShuoMingTipInfoNewMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJUIShuoMingTipInfoNewMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_header = str;
		break;
	case 1:
		m_trunk = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_text = str;
		break;
	case 4:
		m_textbk = str;
		break;
	case 5:
		m_textdi = str;
		break;
	case 6:
		m_titleid = atoi(str.c_str());
		break;
	case 7:
		m_textid = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIShuoMingTipInfoNewMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> headerCanvas = crFilterRenderManager::getInstance()->findCanvas(m_header);
	ref_ptr<crCanvasNode> trunkCanvas = crFilterRenderManager::getInstance()->findCanvas(m_trunk);
	if (headerCanvas.valid() && trunkCanvas.valid())
	{
		ref_ptr<crStaticTextWidgetNode > title = dynamic_cast<crStaticTextWidgetNode *>(headerCanvas->getWidget(m_title));
		ref_ptr<crHypertextWidgetNode> text = dynamic_cast<crHypertextWidgetNode *>(trunkCanvas->getWidget(m_text));
		if(!text.valid()) return;
		crMatrixTransform *textbk = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textbk));
		if(!textbk) return;
		crMatrixTransform *textdi = dynamic_cast<crMatrixTransform *>(trunkCanvas->getChildNode(m_textdi));
		if(!textdi) return;

		std::string titlestr;
		std::string notice;
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		if (noticeHandle)
		{
			ref_ptr<crTableIO>textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_textid),record) >= 0)
			{
				notice = record[1];
			}
			if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_titleid),record) >= 0)
			{
				titlestr = record[1];
			}

			ItemMap itemMap;
			rcfg::ConfigScript cfg_script(&itemMap);
			cfg_script.Add("Hypertext");
			cfg_script.Push("Hypertext");
			cfg_script.Add("Content");
			cfg_script.Push("Content",1);
			cfg_script.Add("Text",notice + " /n");
			cfg_script.Pop();
			cfg_script.Pop();
			text->setHypertext(cfg_script);
		}
		if (title.valid())
		{
			title->setString(titlestr);
		}
		
		///////////计算缩放

		const crBoundingBox& basebox = m_this->getBoundBox();
		const crBoundingBox& headbox = headerCanvas->getBoundBox();
		const crBoundingBox& trunkbox = trunkCanvas->getBoundBox();
		float headhigh =  headbox.yLength();

		float boxYlength = textbk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
		text->updateData();
		const crBoundingBox &bbox = text->getTextObject()->getBoundBox();
		const crBoundingBox &bbox2 = text->getBoundBox();
		float texheight = 0.0f;
		float needmove = 0.0f;
		if(bbox.valid())
		{
			texheight = bbox.yLength();
			texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
			if(texheight<boxYlength) texheight = boxYlength;
			float scale = texheight/boxYlength;

			float needmove ;
			const crBoundingBox &tipsbb = trunkCanvas->getBoundBox();
			crVector3 centerTansTar = text->getBound().center();

			float tmpLength = bbox2.yLength();
			if(textdi)
			{
				float diYlength = textdi->getChild(0)->getBoundBox().yLength();
				centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
			}
			else
				centerTansTar[1] += 0.5f * (tmpLength - texheight);
			// 源中心点
			crVector3 centerTansSrc = textbk->getChild(0)->getBoundBox().center();
			// 将源中心点平移到原点
			crMatrix put_mat = crMatrix::translate(centerTansSrc);
			crMatrix inv_put = crMatrix::inverse(put_mat);
			crMatrix rotmat;
			rotmat.postMult(inv_put);
			rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
			rotmat.postMult(crMatrix::translate(centerTansTar));
			if(textbk)textbk->setMatrix(rotmat);

			if(textdi)
			{
				float diYlength = textdi->getChild(0)->getBoundBox().yLength();
				//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
				needmove = boxYlength - texheight + diYlength * 0.25f;
				//needmove = boxYlength - texheight;
				textdi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
			}
		}
		else
		{
			textbk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
			if(textdi) textdi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
		}
		///////////////////////////////////////
		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		const crBoundingBox &iconbox = m_this->getBoundBox();
		const crBoundingBox &tips = trunkCanvas->getBoundBox();
		float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
		float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
		crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
		/////////////////////////////////////////
		crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
		pos = pos * mat;
		pos2 = pos2 * mat;
		float posx2,posy2;
		/////////////////////////////////////////                             
		if (pos2[0] >= 0)
		{
			posx2 = pos2[0] - posx;
		}
		else
		{
			posx2 = pos[0] + posx;
		}
		if (pos2[1] >= 0)
		{
			posy2 = pos[1] - posy;
		}
		else
		{
			posy2 = pos2[1] + 2.0f * posy - needmove;
		}
		if((abs(posy2)+posy)>1.0f)
		{
			if (posy2 >= 0)
			{
				posy2 -= posy;
			}
			else
			{
				posy2 += posy;
			}
		}
		crVector2 mouse(crVector2(posx2,posy2));

		headerCanvas->setMatrix(crMatrix::translate(basebox.xMin()-headbox.xLength()*0.5f,basebox.yMin()-headbox.yLength()*0.5f,0.0f));

		trunkCanvas->setMatrix(crMatrix::translate(basebox.xMin()-trunkbox.xLength()*0.5f,basebox.yMin()-trunkbox.yLength()*0.05f,0.0f));
		trunkCanvas->setCanFocus(false);
		headerCanvas->setCanFocus(false);

		bool ifshowcanvas = true;

		crFilterRenderManager::getInstance()->showCanvas(headerCanvas.get(),true);
		crFilterRenderManager::getInstance()->showCanvas(trunkCanvas.get(),true);
	}
}

/////////////////////////////////////////
//
//crJXJServerUseRefineItemMethod
//
/////////////////////////////////////////
crJXJServerUseRefineItemMethod::crJXJServerUseRefineItemMethod():
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL){}
crJXJServerUseRefineItemMethod::crJXJServerUseRefineItemMethod(const crJXJServerUseRefineItemMethod& handle):
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseRefineItemMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_user = (crPlayerGameData*)LOINT64(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HIINT64(param64);
			m_itemid = vparam[0];
			m_selectid = vparam[1];
			m_useResult = (char *)(vparam[2]);
		}
		else
		{
			m_itemid = 0;
		}
		break;
	}
}

void crJXJServerUseRefineItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_stoneid);
		break;
	default:
		break;
	}
}

void crJXJServerUseRefineItemMethod::operator()(crHandle &handle)
{
	if (m_itemid == m_stoneid[0])//7533
		*m_useResult = USE_RefineStoneError;
	else if(m_itemid == m_stoneid[1])//7534
		*m_useResult = USE_HigherRefineStoneError;
	else if(m_itemid == m_stoneid[2])//7535
		*m_useResult = USE_RefineLockError;
}

///////////////////////////////////////////
////
////crJXJWorldRankRewardImageTipsMethod
////
///////////////////////////////////////////
crJXJWorldRankRewardImageTipsMethod::crJXJWorldRankRewardImageTipsMethod():
	m_ea(NULL),
	m_this(NULL)
	//m_ifexpand(false),
	//m_expandnum(1.0f),
{
}
crJXJWorldRankRewardImageTipsMethod::crJXJWorldRankRewardImageTipsMethod(const crJXJWorldRankRewardImageTipsMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJWorldRankRewardImageTipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_ea = NULL;
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJWorldRankRewardImageTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_info2 = str;
		break;
	case 3:
		m_itemicon = str;
		break;
	case 4:
		m_canvas = str;
		break;
	case 5:
		m_infocanvas = str;
		break;
	case 6:
		m_infobk = str;
		break;
	case 7:
		m_infodi = str;
		break;
	case 8:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 9:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 10:
		crArgumentParser::appAtoVec(str,m_imageindex);
		break;
	}
}
void crJXJWorldRankRewardImageTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> parentcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData && infocanvas.valid() && parentcanas.valid())
	{
		crStaticTextWidgetNode *name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		crImageBoxWidgetNode *itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		crHypertextWidgetNode *info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name)
		{
			name->clearString();
		}
		if (info2)
		{
			info2->clearText();
		}
		if (itemicon)
		{
			itemicon->clearImage();
		}

		crVector4f colormode[5];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i =0; i<5;i++)
		{
			if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord) >= 0)
			{
				if (qualityindex>=0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int nameindex = itemtab->getTitleIndex("name");
		int colorindex = itemtab->getTitleIndex("品质");

		ref_ptr<crTableIO> rankawardtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldRankAwardTab);
		std::vector<std::string> recorditem;
		int type1index = rankawardtab->getTitleIndex("类型1");
		int award1index = rankawardtab->getTitleIndex("奖励1");
		int type2index = rankawardtab->getTitleIndex("类型2");
		int award2index = rankawardtab->getTitleIndex("奖励2");

		ref_ptr<crTableIO> shoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJZhengZhanShopTab);
		int cRow = shoptab->getRowCount();
		ref_ptr<crScrollBarWidgetNode>  scroll = dynamic_cast<crScrollBarWidgetNode *>(parentcanas->getWidget(m_canvas));
		int val = 0;
		int itemid;
		if (rankawardtab->queryOneRecord(0,crArgumentParser::appItoa(m_imageindex[0]),record) >= 0)
		{
			if (m_imageindex[1] == 0)
			{
				if (record[type1index] == "10")
				{
					crVector3i vec;
					crArgumentParser::appAtoVec(record[award1index],vec);
					if (vec[0] > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(vec[0]),recorditem) >= 0)
						itemid = vec[0];
				}
			}
			else
			{
				if (record[type2index] == "10")
				{
					crVector3i vec;
					crArgumentParser::appAtoVec(record[award2index],vec);
					if (vec[0] > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(vec[0]),recorditem) >= 0)
						itemid = vec[0];
				}
			}
		}

		if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
		{
			int colormodeid = atoi(record[colorindex].c_str());
			if(name)
			{
				name->setColor(colormode[colormodeid]);
				name->setString(record[nameindex]);
			}
			if (itemicon)
			{
				itemicon->setImageName(record[iconindex]);
			}
			if(info2)
			{
				ItemMap itemMap;
				rcfg::ConfigScript outscript(&itemMap);
				crVector2i item(itemid,0);
				playerData->doEvent(WCH_JXJCreateItemTips,MAKEINT64(&item,&outscript));
				info2->setHypertext(outscript);
			}

			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//centerTansTar[1] += 0.5f * (tmpLength - texheight);
					centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				}
				else
					centerTansTar[1] += 0.5f * (tmpLength - texheight);
				// 源中心点
				crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				// 将源中心点平移到原点
				crMatrix put_mat = crMatrix::translate(centerTansSrc);
				crMatrix inv_put = crMatrix::inverse(put_mat);
				crMatrix rotmat;
				rotmat.postMult(inv_put);
				rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if(infobk)infobk->setMatrix(rotmat);

				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2,posy2;
			const crBoundingBox &infos = infobk->getBoundBox();
			float posinfox = (infos.m_max[0]-infos.m_min[0]);
			float posinfoy = (infos.m_max[1]-infos.m_min[1]);
			if (pos2[0] >= 0)
			{
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + 2.0f * posy - needmove;
			}
			if((abs(posy2)+posy)>1.0f)
			{
				if (posy2 >= 0)
				{
					posy2 -= posy;
				}
				else
				{
					posy2 += posy;
				}
			}
			crVector2 mouse(crVector2(posx2,posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));

			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJEquipeRefineAttributeTipMethod
//装备精炼属性提示
//
//////////////////////////////////////////////////////////////////////////
crJXJEquipeRefineAttributeTipMethod::crJXJEquipeRefineAttributeTipMethod()
{}
crJXJEquipeRefineAttributeTipMethod::crJXJEquipeRefineAttributeTipMethod(const crJXJEquipeRefineAttributeTipMethod& handle)
{

}
void crJXJEquipeRefineAttributeTipMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJEquipeRefineAttributeTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_name = str;
		break;
	case 2:
		m_tips2 = str;
		break;
	case 3:
		m_tipstext = str;
		break;
	}
}

void crJXJEquipeRefineAttributeTipMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>tipsCanvas = NULL;
		ref_ptr<crStaticTextWidgetNode> name = NULL;

		ref_ptr<crHypertextWidgetNode> attribution = dynamic_cast<crHypertextWidgetNode *>(m_this);
		if (attribution.valid())
		{
			std::wstring content;
			attribution->getString(0,content);
			std::string str2 = CRIOManager::convertUTF16toUTF8(content.c_str());
			std::string str3(str2.begin(),str2.begin()+str2.find('+'));
			std::string str1;
			ref_ptr<crTableIO>attributioninfoTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAttributionDescribeTab);
			crTableIO::StrVec record;
			if (attributioninfoTab.valid() && attributioninfoTab->queryOneRecord(0,str3,record) >= 0)
				str1 = record[1];

			if (str3=="反弹物伤" || str3=="反弹法伤" || str3=="反潜行")
			{
				tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips2);
				if(tipsCanvas.valid())
					name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_tipstext));
			}
			else
			{
				tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
				if(tipsCanvas.valid())
					name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
			}

			if(tipsCanvas.valid())
			{
				if (name.valid())
				{
					name->setString(str1);
				}

				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				const crBoundingBox &iconbox = m_this->getBoundBox();
				const crBoundingBox &tips = tipsCanvas->getBoundBox();
				float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
				float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
				/////////////////////////////////////////
				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
				float posx2,posy2;
				/////////////////////////////////////////
				if (pos2[0] >= 0)
				{
					posx2 = pos2[0] - posx;
				}
				else
				{
					posx2 = pos[0] + posx;
				}
				if (pos2[1] >= 0)
				{
					posy2 = pos[1] - posy;
				}
				else
				{
					posy2 = pos2[1] + posy;
				}
				crVector2 mouse(crVector2(posx2,posy2));
				/////////////////////////////////////////
				tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
				tipsCanvas->setCanFocus(false);
				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
			}
		}
		/*ref_ptr<crCanvasNode>  Canvas = m_this->getParentCanvas();
		if (Canvas.valid())
		{
		ref_ptr<crMultiSwitch> shilisw = dynamic_cast<crMultiSwitch *>(Canvas->getChildNode(m_kaiguan));
		ref_ptr<crHypertextWidgetNode> inputnotice = dynamic_cast<crHypertextWidgetNode *>(Canvas->getWidget(m_text));
		if (shilisw.valid() && inputnotice.valid())
		{
		if (shilisw->getActiveSwitchSet() == 0)
		{
		shilisw->setActiveSwitchSet(1);
		inputnotice->setVisiable(false);
		} 
		else
		{
		shilisw->setActiveSwitchSet(0);
		inputnotice->setVisiable(true);
		}
		}
		}*/
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJDetachCardOrItemForSavingMethod
//取下将要存储的将卡或装备
//
//////////////////////////////////////////////////////////////////////////
crJXJDetachCardOrItemForSavingMethod::crJXJDetachCardOrItemForSavingMethod()
{}
crJXJDetachCardOrItemForSavingMethod::crJXJDetachCardOrItemForSavingMethod(const crJXJDetachCardOrItemForSavingMethod& handle)
{

}
void crJXJDetachCardOrItemForSavingMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJDetachCardOrItemForSavingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_jiangkacanvas = str;
		break;
	case 1:
		m_text = str;
		break;
	}
}

void crJXJDetachCardOrItemForSavingMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode>  Canvas = m_this->getParentCanvas();
		if (Canvas.valid())
		{
			ref_ptr<crTableIO>itemTable = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(!itemTable.valid()) return;
			crTableIO::StrVec	record;
			int colorindex = itemTable->getTitleIndex("品质");
			int iconIndex = itemTable->getTitleIndex("icon");
			int maxCountIndex = itemTable->getTitleIndex("叠加数量");

			void *param;
			crData *canvasData = Canvas->getDataClass();
			crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			
			ItemOrCardMap *cardoritemMap = NULL;
			JXJItemBagVec *cardoritembagvec = NULL;
			
			int index = 11;
			ref_ptr<crImageBoxWidgetNode>  imagebox = dynamic_cast<crImageBoxWidgetNode *>(m_this);
			std::string boxname = imagebox->getName();
			if (boxname.find("XLunhui_Card") != std::string::npos)
			{
				std::string strindex =  boxname.substr(boxname.find('d')+1,boxname.size()-boxname.find('d')-1);
				index = atoi(strindex.c_str()) - 1;

				canvasData->getParam(WCHDATA_JXJLunHuiCardMap,param);
				cardoritemMap = (ItemOrCardMap *)param;

				ref_ptr<crCanvasNode>cardcanvas = crFilterRenderManager::getInstance()->findCanvas(m_jiangkacanvas);
				if(!cardcanvas) return;
				crData *formcarddata = cardcanvas->getDataClass();
				formcarddata->excHandle(MAKEINT64(WCH_LockData,1));
				formcarddata->getParam(WCHDATA_JXJForJKVec, param);
				JXJItemBagVec *tempcardvec = (JXJItemBagVec *)param;

				if (index<10 && cardoritemMap && tempcardvec)
				{
					ItemOrCardMap::iterator iter = cardoritemMap->find((char)index);
					if (iter != cardoritemMap->end())
					{
						int cardid = iter->second->getItemID();
						canvasData->excHandle(MAKEINT64(WCH_LockData,1));
						cardoritemMap->erase(iter);
						canvasData->excHandle(MAKEINT64(WCH_LockData,0));
						if (cardid > 0)
						{
							for (JXJItemBagVec::iterator itr = tempcardvec->begin(); itr!=tempcardvec->end(); ++itr)
							{
								if ((*itr) == NULL)
								{
									ref_ptr<crBagItemData> undocard = new crBagItemData;
									undocard->setItemID(cardid);
									undocard->setItemCount(1);
									(*itr) = undocard;
									break;
								}
							}
						}
					}
				}
				formcarddata->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else if (boxname.find("XLunhui_Icon") != std::string::npos)
			{
				std::string strindex =  boxname.substr(boxname.find('o')+2,boxname.size()-boxname.find('o')-2);
				index = atoi(strindex.c_str()) - 1;

				canvasData->getParam(WCHDATA_JXJLunHuiItemMap, param);
				cardoritemMap = (ItemOrCardMap *)param;

				playerData->getParam(WCHDATA_JXJItemBagVec, param);
				cardoritembagvec = (JXJItemBagVec *)param;

				if (index<10 && cardoritemMap && cardoritembagvec)
				{
					ItemOrCardMap::iterator iter = cardoritemMap->find((char)index);
					if (iter != cardoritemMap->end())
					{
						ref_ptr<crBagItemData> temp =  new crBagItemData;
						temp = iter->second;
						canvasData->excHandle(MAKEINT64(WCH_LockData,1));
						cardoritemMap->erase(iter);
						canvasData->excHandle(MAKEINT64(WCH_LockData,0));

						if(itemTable->queryOneRecord(0, crArgumentParser::appItoa(temp->getItemID()), record)>=0)
						{
							playerData->excHandle(MAKEINT64(WCH_LockData,1));
							int maxCount = atoi(record[maxCountIndex].c_str());
							bool needNew = true;
							JXJItemBagVec::iterator itr_bag;
							itr_bag = cardoritembagvec->begin();
							for (; itr_bag != cardoritembagvec->end(); itr_bag++)
							{
								if (!itr_bag->valid())continue;
								if((*itr_bag)->getItemID()==temp->getItemID() && \
									itr_bag->get()->getItemCount()< maxCount)
								{
									itr_bag->get()->setItemCount(itr_bag->get()->getItemCount()+1);
									needNew = false;
									break;
								}
							}
							if(needNew)
							{
								itr_bag = cardoritembagvec->begin();
								for (; itr_bag != cardoritembagvec->end(); itr_bag++)
								{
									if((*itr_bag)==NULL)
									{
										*itr_bag = temp;
										break;
									}
								}
							}
							playerData->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeWindowPopMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJCustomContributeWindowPopMethod::crJXJCustomContributeWindowPopMethod()
{}
crJXJCustomContributeWindowPopMethod::crJXJCustomContributeWindowPopMethod(const crJXJCustomContributeWindowPopMethod& handle)
{

}
void crJXJCustomContributeWindowPopMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJCustomContributeWindowPopMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	}
}

void crJXJCustomContributeWindowPopMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		void *param;
		ref_ptr<crData> data =  crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		if (!data.valid()) return;

		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char myviplv = *(unsigned char *)param;

		int contributelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCustomContributeLv).c_str());

		if (myviplv >= contributelv)
		{
			ref_ptr<crCanvasNode> windows = crFilterRenderManager::getInstance()->findCanvas(m_tips);
			if(windows.valid())
				 crFilterRenderManager::getInstance()->showCanvas(windows.get(),true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeBtnTipMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJCustomContributeBtnTipMethod::crJXJCustomContributeBtnTipMethod()
{}
crJXJCustomContributeBtnTipMethod::crJXJCustomContributeBtnTipMethod(const crJXJCustomContributeBtnTipMethod& handle)
{

}
void crJXJCustomContributeBtnTipMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJCustomContributeBtnTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips2 = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	}
}

void crJXJCustomContributeBtnTipMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips2);
		
		std::string str1;

		if(tipsCanvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_tipstext));
			if (name.valid())
			{
				int contributelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCustomContributeLv).c_str());
				str1 = "VIP等级达到"+crArgumentParser::appItoa(contributelv)+"级，由此进行自定义建设。";
				name->setString(str1);
			}

			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			float posx2,posy2;
			/////////////////////////////////////////
			if (pos2[0] >= 0)
			{
				posx2 = pos2[0] - posx;
			}
			else
			{
				posx2 = pos[0] + posx;
			}
			if (pos2[1] >= 0)
			{
				posy2 = pos[1] - posy;
			}
			else
			{
				posy2 = pos2[1] + posy;
			}
			crVector2 mouse(crVector2(posx2,posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			tipsCanvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJExcuteCustomContributeMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJExcuteCustomContributeMethod::crJXJExcuteCustomContributeMethod()
{}
crJXJExcuteCustomContributeMethod::crJXJExcuteCustomContributeMethod(const crJXJExcuteCustomContributeMethod& handle)
{

}
void crJXJExcuteCustomContributeMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJExcuteCustomContributeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_radio = str;
		break;
	default:
		break;
	}
}

void crJXJExcuteCustomContributeMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> parentcanvas = m_this->getParentCanvas();
		if (parentcanvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(parentcanvas->getWidget(m_input));
			if(input.valid())
			{
				int count = atoi((input->getUTF8String()).c_str());

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
				CRNet::crNetConductor *netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
				if (canvas.valid()&&playerGameData&&netConductor)
				{
					ref_ptr<crRadioGroupWidgetNode> radio =  dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_radio));
					unsigned char select;
					if (radio.valid())
					{
						select = radio->getSelect();
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(6);
					stream->_writeUChar(select);
					stream->_writeUChar(1);
					stream->_writeInt(count);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuildMyCountry,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUICheckContributeInputNumMethod
//
/////////////////////////////////////////
crJXJUICheckContributeInputNumMethod::crJXJUICheckContributeInputNumMethod():
	m_this(NULL)
{
}
crJXJUICheckContributeInputNumMethod::crJXJUICheckContributeInputNumMethod(const crJXJUICheckContributeInputNumMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_input(handle.m_input)
{
}
void crJXJUICheckContributeInputNumMethod::inputParam(int i, void *param)
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
void crJXJUICheckContributeInputNumMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	}
}
void crJXJUICheckContributeInputNumMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		int count = 0;
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input));
		if(input.valid())
		{
			ref_ptr<crData> data = myPlayer->getDataClass();
			data->getParam(WCHDATA_JXJCoppercash,param);
			int coppercash = *(int *)param;
			int havecopper = coppercash/10000;
			int count = atoi((input->getUTF8String()).c_str());
			
			if(count > havecopper) 
				input->setString(crArgumentParser::appItoa(havecopper));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRefineButtonTipMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJRefineButtonTipMethod::crJXJRefineButtonTipMethod()
{}
crJXJRefineButtonTipMethod::crJXJRefineButtonTipMethod(const crJXJRefineButtonTipMethod& handle)
{

}
void crJXJRefineButtonTipMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJRefineButtonTipMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipstext = str;
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJRefineButtonTipMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);

		if(tipsCanvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_tipstext));
			if (name.valid())
			{
				void *param;
				std::string str1;
				crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
				crData *data = playerData->getDataClass();
				int crefinestone = 0;
				int chigherrefinestone = 0;
				int crefinelock = 0;
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				int itemid;
				for (JXJItemBagVec::iterator itr=itembagvec->begin(); itr!=itembagvec->end(); itr++)
				{
					if(itr->valid())
					{
						itemid = (*itr)->getItemID();
						if(itemid == 7533)
							crefinestone += (*itr)->getItemCount();
						else if(itemid == 7534)
							chigherrefinestone += (*itr)->getItemCount();
						else if(itemid == 7535)
							crefinelock += (*itr)->getItemCount();
					}
				}

				if (m_index == 0)
				{
					str1 = "当前拥有精炼石"+crArgumentParser::appItoa(crefinestone)+"个。";
				}
				else
				{
					str1 = "当前拥有至尊精炼石"+crArgumentParser::appItoa(chigherrefinestone)+"个。";
				}
			
				name->setString(str1);
			}

			//const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			//const crBoundingBox &iconbox = m_this->getBoundBox();
			//const crBoundingBox &tips = tipsCanvas->getBoundBox();
			//float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
			//float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
			//crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
			///////////////////////////////////////////
			//crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
			//float posx2,posy2;
			///////////////////////////////////////////
			//if (pos2[0] >= 0)
			//{
			//	posx2 = pos2[0] - posx;
			//}
			//else
			//{
			//	posx2 = pos[0] + posx;
			//}
			//if (pos2[1] >= 0)
			//{
			//	posy2 = pos[1] - posy;
			//}
			//else
			//{
			//	posy2 = pos2[1] + posy;
			//}
			
			/////////////////////////////////////////

			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tipsCanvas->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength()/2);
			crVector2 mouse(crVector2(posx,posy));

			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			tipsCanvas->setCanFocus(false);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCustomContributeAddOrReduceMethod
//
//
//////////////////////////////////////////////////////////////////////////
crJXJCustomContributeAddOrReduceMethod::crJXJCustomContributeAddOrReduceMethod()
{}
crJXJCustomContributeAddOrReduceMethod::crJXJCustomContributeAddOrReduceMethod(const crJXJCustomContributeAddOrReduceMethod& handle)
{

}
void crJXJCustomContributeAddOrReduceMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJCustomContributeAddOrReduceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJCustomContributeAddOrReduceMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> Canvas = m_this->getParentCanvas();
		if(Canvas.valid())
		{
			ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(Canvas->getWidget(m_input));
			if(input.valid())
			{
				int count = atoi((input->getUTF8String()).c_str());
				if(m_index == 1)
					count = count + 1;
				else
				{
					count = count - 1;
					if(count <= 0)
						count = 0;
				}
				input->setString(crArgumentParser::appItoa(count));
			}
		}
	}
}

