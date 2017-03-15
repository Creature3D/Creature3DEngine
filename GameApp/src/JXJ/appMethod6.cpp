/**********************************************************************
*
*	文件:	   appMethod6.cpp
*
*	描述:	   
*
*	作者:	   李婧
*					  
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod6.h>
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
#include <stdlib.h>
#include <JXJ/appDBQuery.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crCookFile.h>
#include <CRIOManager/crReadFile.h>
#include <JXJ/appMethod16.h>

using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace rbody;
using namespace JXJ;
using namespace CRIOManager;
using namespace CRUI;

/////////////////////////////////////////
//
//crJXJSmelterOnShowMethod
//
/////////////////////////////////////////
crJXJSmelterOnShowMethod::crJXJSmelterOnShowMethod()
{
}

crJXJSmelterOnShowMethod::crJXJSmelterOnShowMethod(const crJXJSmelterOnShowMethod& handle):
	crMethod(handle)
{
}

void crJXJSmelterOnShowMethod::inputParam(int i, void *param)
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

void crJXJSmelterOnShowMethod::addParam(int i, const std::string& str)
{
}

void crJXJSmelterOnShowMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		//void *param;
		//crData *data = playerGameData->getDataClass();
		//crData *canvasdata  = m_this->getDataClass();
		//if (data && canvasdata)
		//{
		//	data->getParam(WCHDATA_JXJItemBagVec,param);
		//	JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		//	canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
		//	JXJItemBagVec *tempitembagvec = (JXJItemBagVec *)param;
		//	tempitembagvec->clear();
		//	for (JXJItemBagVec::iterator itr = itembagvec->begin();
		//		itr != itembagvec->end();
		//		++itr)
		//	{
		//		tempitembagvec->push_back((*itr));
		//	}
		//}
	}
}
/////////////////////////////////////////
//
//crJXJSmelterUpdateMethod
//
/////////////////////////////////////////
crJXJSmelterUpdateMethod::crJXJSmelterUpdateMethod(){}
crJXJSmelterUpdateMethod::crJXJSmelterUpdateMethod(const crJXJSmelterUpdateMethod& handle):
	crMethod(handle),
	m_myBagList(handle.m_myBagList),
	m_pagenum(handle.m_pagenum),
	m_itemtype(handle.m_itemtype),
	m_defaulticon(handle.m_defaulticon),
	m_mateicon(handle.m_mateicon),
	m_matecountinput(handle.m_matecountinput),
	m_smeltSW(handle.m_smeltSW),
	m_addbtn(handle.m_addbtn),
	m_deductbtn(handle.m_deductbtn)
{
}
void crJXJSmelterUpdateMethod::inputParam(int i, void *param)
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
void crJXJSmelterUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_myBagList = str;
		break;
	case 1:
		m_pagenum = str;
		break;
	case 2:
		m_itemtype = str;
		break;
	case 3:
		m_defaulticon = str;
		break;
	case 4:
		m_mateicon = str;
		break;
	case 5:
		m_matecountinput = str;
		break;
	case 6:
		m_smeltSW = str;
		break;
	case 7:
		m_addbtn = str;
		break;
	case 8:
		m_deductbtn = str;
		break;
	}
}
void crJXJSmelterUpdateMethod::operator()(crHandle &handle)
{
	//ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this)
	{
		bool ifshow = false;
		void *param;
		int typeitem = 0,curpage = 1;
		crData *canvasdata = m_this->getDataClass();
		if (canvasdata)
		{
			crData *data = playerData->getDataClass();
			canvasdata -> getParam(WCHDATA_JXJSmelterCurPage,param);
			curpage = *(int *)param;
			//canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			ref_ptr<crListControlWidgetNode> backPackList = dynamic_cast<crListControlWidgetNode *>(m_this->getWidget(m_myBagList));
			ref_ptr<crStaticTextWidgetNode> pagenum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pagenum));
			ref_ptr<crRadioGroupWidgetNode> itemtype = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_itemtype));
			ref_ptr<crEditWidgetNode> matecountinput = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_matecountinput));
			ref_ptr<crImageBoxWidgetNode> mateicon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mateicon));
			//ref_ptr<crButtonWidgetNode> copperbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_copperbtn));
			//ref_ptr<crButtonWidgetNode> goldbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goldbtn));
			ref_ptr<crMultiSwitch> smeltSW = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_smeltSW));
			ref_ptr<crButtonWidgetNode> addbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_addbtn));
			ref_ptr<crButtonWidgetNode> deductbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_deductbtn));
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec itemrecord,smeltrecord;
			int nameid = itemtab->getTitleIndex("name");
			int iconid = itemtab->getTitleIndex("icon");
			int attrid = itemtab->getTitleIndex("类型");
			int colorid = itemtab->getTitleIndex("品质");
			canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
			crBagItemData *bagitemdata = (crBagItemData *)param;
			canvasdata->getParam(WCHDATA_JXJSmeltItemSelectIndex,param);
			int selectindex = *(int*)param;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			if(backPackList.valid())
			{
				///////////左边的列表
				int totalcount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax,viplv).c_str()));
				int onepagecount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterPerPage,viplv).c_str()));
				if(itemtype.valid())
				{
					typeitem = itemtype->getSelect();
				}
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJOpenBagNum,param);
				unsigned char openbagnum = *(unsigned char *)param;
				std::string iconfile;
				std::string title,title2;
				backPackList->setListNodeCount(totalcount);
				backPackList->setStartYPos(curpage - 1);
				crListControlWidgetNode::ListNodeVec& listNodeVec = backPackList->getListNodeVec();
				int i = 0;
				int count = 0;
				//itembagvec->resize(openbagnum);
				int newdata = 0;
				for( JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr,++i,++newdata )
				{
					ifshow = true;
					if(i>=0 && i<openbagnum)
					{
						if((*itr)!=NULL && (*itr)->getItemID()!=0)
						{
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itr)->getItemID()),itemrecord) >= 0)
							{
								int nowcount = (*itr)->getItemCount();
								if (bagitemdata && newdata == selectindex)
								{
									nowcount = nowcount - bagitemdata->getItemCount();
								}
								switch (typeitem)
								{
								case 0:
									if (atoi(itemrecord[attrid].c_str())!=IT_Equip || atoi(itemrecord[colorid].c_str()) < CardColor_Purple)
									{
										ifshow = false;
									}
									break;
								case 1:
									if (atoi(itemrecord[attrid].c_str())!=IT_SmeltMaterial)
									{
										ifshow = false;
									}
									break;
								case 2:
									if (atoi(itemrecord[attrid].c_str())!=IT_Gemstone)
									{
										ifshow = false;
									}
									break;
								default:
									break;
								}
								if (nowcount == 0)
								{
									ifshow = false;
								}
								if (!ifshow)
								{
									i--;
									continue;
								}
								title = itemrecord[nameid];
								iconfile = itemrecord[iconid];
								//	listNodeVec[i]->setTitle(title);
								listNodeVec[i]->setFont("simhei.ttf");
								if (nowcount > 1)
								{
									title2 = crArgumentParser::appItoa(nowcount) + std::string(" ");;
								}
								else
								{
									title2.clear();
								}
								listNodeVec[i]->setTitle2(title2);
								listNodeVec[i]->setData(/*curpage*onepagecount+*/newdata);
								listNodeVec[i]->setImageName(iconfile);
								listNodeVec[i]->setVisiable(true);
								listNodeVec[i]->setEnable(true);
								count++;
							}
						}
						else
						{
							i--;
						}
					}
				}
				for(i;i<openbagnum;i++)
				{
					listNodeVec[i]->setTitle("");
					listNodeVec[i]->setTitle2("");
					listNodeVec[i]->setData(-1);
					listNodeVec[i]->setImageName(m_defaulticon);
					listNodeVec[i]->setVisiable(true);
					listNodeVec[i]->setEnable(false);
				}
				for(int j = openbagnum; j < totalcount; j++)
				{
					listNodeVec[j]->setData(j);
					listNodeVec[j]->setImageName("");
					//title = "暂未购买";
					listNodeVec[j]->setTitle("");
					//title = "点击购买";
					listNodeVec[j]->setTitle2("");
					listNodeVec[j]->setEnable(true);
					listNodeVec[j]->setVisiable(false);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				//if(i<0)i=0;
				int totalpage = ceil((float)count/(float)onepagecount);
				if (totalpage < 1)
				{
					totalpage = 1;
				}
				canvasdata -> inputParam(WCHDATA_JXJSmelterTotalPage,&totalpage);
				if(pagenum.valid())pagenum->setString(crArgumentParser::appItoa(curpage)+"/"+crArgumentParser::appItoa(totalpage));
			}
			///////////////////////右边的炉子
			crVector4 color = COLOR_WHITE;
			int mincount = 1;
			//if (copperbtn)
			//{
			//	copperbtn->setVisiable(false);
			//	copperbtn->setEnable(false);
			//}
			//if (goldbtn)
			//{
			//	goldbtn->setVisiable(false);
			//	goldbtn->setEnable(false);
			//}
			if (addbtn.valid())
			{
				addbtn->setVisiable(false);
			}
			if (deductbtn.valid())
			{
				deductbtn->setVisiable(false);
			}
			if (matecountinput.valid())
			{
				matecountinput->clearString();
				matecountinput->setEnable(false);
			}
			if (mateicon.valid())mateicon->setVisiable(false);
			if (smeltSW.valid())
			{
				switch (typeitem)
				{
				case 0:
					smeltSW->setActiveSwitchSet(1);
					break;
				case 1:
					smeltSW->setActiveSwitchSet(2);
					break;
				case 2:
					smeltSW->setActiveSwitchSet(3);
					break;
				}
			}
			if (bagitemdata)
			{
				int smeltitemid = bagitemdata->getItemID();
				int smeltitemcount = bagitemdata->getItemCount();
				if (smeltitemid > 0 && smeltitemcount >= 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(smeltitemid),smeltrecord) >= 0)
				{
					//if (copperbtn.valid())copperbtn->setVisiable(true);
					//if (goldbtn.valid())goldbtn->setVisiable(true);
					if (mateicon.valid())
					{
						mateicon->setVisiable(true);
						mateicon->setImageName(smeltrecord[iconid]);
					}
					int itemtype = atoi(smeltrecord[attrid].c_str());
					if (itemtype == IT_Equip)
					{
						if (bagitemdata->getInlayID() > 0)
						{
							if (smeltSW.valid())
							{
								smeltSW->setActiveSwitchSet(1);
							}
						}
						else
						{
							if (smeltSW.valid())
							{
								smeltSW->setActiveSwitchSet(0);
							}
						}
					}
					else
					{
						if (matecountinput.valid())matecountinput->setEnable(true);
						if (addbtn.valid())
						{
							addbtn->setVisiable(true);
						}
						if (deductbtn.valid())
						{
							deductbtn->setVisiable(true);
						}
						if (itemtype == IT_Gemstone)
						{
							mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount, viplv).c_str()));
							if (smeltSW.valid())
							{
								smeltSW->setActiveSwitchSet(3);
							}
						}
						else if (itemtype == IT_SmeltMaterial)
						{
							mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount, viplv).c_str()));
							if (smeltSW.valid())
							{
								smeltSW->setActiveSwitchSet(2);
							}
						}
					}
					if (smeltitemcount >= mincount)
					{
						//if (copperbtn)copperbtn->setEnable(true);
						//if (goldbtn)goldbtn->setEnable(true);
						color = COLOR_WHITE;
					}
					else
					{
						color = COLOR_RED;
					}
					if (matecountinput.valid() && smeltitemcount >= 0)
					{
						matecountinput->setColor(color);
						if (smeltitemcount > 0)
						{
							matecountinput->setString(crArgumentParser::appItoa(smeltitemcount));
						}
						else
						{
							matecountinput->clearString();
						}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSmelterEditCheckMethod
//
/////////////////////////////////////////
crJXJSmelterEditCheckMethod::crJXJSmelterEditCheckMethod():
	m_this(NULL)
{
}
crJXJSmelterEditCheckMethod::crJXJSmelterEditCheckMethod(const crJXJSmelterEditCheckMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_leftbtn(handle.m_leftbtn),
	m_rightbtn(handle.m_rightbtn),
	m_edit(handle.m_edit)
{
}
void crJXJSmelterEditCheckMethod::inputParam(int i, void *param)
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
	//	m_this = (crEditWidgetNode*)param;
	//	break;
	//}
}
void crJXJSmelterEditCheckMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_leftbtn = str;
		break;
	case 2:
		m_rightbtn = str;
		break;
	case 3:
		m_edit = str;
		break;
	default:
		break;
	}
}
void crJXJSmelterEditCheckMethod::operator()(crHandle &handle)
{	
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>gongfangCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	int mincount = 0;
	if(playerGameData && gongfangCanvas.valid() && gongfangCanvas->getDataClass())
	{
		ref_ptr<crButtonWidgetNode> leftbtn = dynamic_cast<crButtonWidgetNode *>(gongfangCanvas->getWidget(m_leftbtn));
		ref_ptr<crButtonWidgetNode> rightbtn = dynamic_cast<crButtonWidgetNode *>(gongfangCanvas->getWidget(m_rightbtn));
		ref_ptr<crEditWidgetNode> edit = dynamic_cast<crEditWidgetNode *>(gongfangCanvas->getWidget(m_edit));
		if (edit.valid())
		{
			crData *gongfangdata = gongfangCanvas->getDataClass();
			gongfangdata->getParam(WCHDATA_JXJSmelterMateMaxCount,param);
			int maxcount = *(int *)param;
			gongfangdata->getParam(WCHDATA_JXJSmelterItemData,param);
			crBagItemData *bagitemdata = (crBagItemData *)param;
			int editcount = atoi(edit->getUTF8String().c_str());
			if (leftbtn.valid())
			{
				leftbtn->setEnable(true);
			}
			if (rightbtn.valid())
			{
				rightbtn->setEnable(true);
			}
			if (editcount >= maxcount)
			{
				editcount = maxcount;
				if (rightbtn.valid())
				{
					rightbtn->setEnable(false);
				}
			}
			if(editcount <= 1)
			{
				if (leftbtn.valid())
				{
					leftbtn->setEnable(false);
				}
				if (editcount <= 0)
				{
					editcount = 0;
				}
			}
			bagitemdata->setItemCount(editcount);
			if (!edit->getUTF8String().empty())
			{
				edit->setString(crArgumentParser::appItoa(editcount));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIPutMateOnSmelterMethod
//
/////////////////////////////////////////
crJXJUIPutMateOnSmelterMethod::crJXJUIPutMateOnSmelterMethod()
{
}
crJXJUIPutMateOnSmelterMethod::crJXJUIPutMateOnSmelterMethod(const crJXJUIPutMateOnSmelterMethod& handle):
	crMethod(handle),
	m_itemlist(handle.m_itemlist),
	m_inputcountcanvas(handle.m_inputcountcanvas),
	m_inputedit(handle.m_inputedit)
{
}
void crJXJUIPutMateOnSmelterMethod::inputParam(int i, void *param)
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
void crJXJUIPutMateOnSmelterMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_itemlist = str;
		break;
	case 1:
		m_inputcountcanvas = str;
		break;
	case 2:
		m_inputedit = str;
		break;
	}
}
void crJXJUIPutMateOnSmelterMethod::operator()(crHandle &handle)
{
	bool canceltask = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		crData *data = playerData->getDataClass();
		void *param;
		char success = 0;
		int itemtype = 0;
		int tipindex = 0;
		int itemcount = 0;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		data -> getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		//ref_ptr<crCanvasNode>inputcanvas = crFilterRenderManager::getInstance()->findCanvas(m_inputcountcanvas);
		int mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount,viplv).c_str()));
		int storemincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount,viplv).c_str()));
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int attrid = itemtab->getTitleIndex("类型");
		int colorid = itemtab->getTitleIndex("品质");
		if (canvas.valid() && itemtab.valid() && attrid > 0 && colorid > 0/* && inputcanvas.valid()*/)
		{
			int nowitemcount = 0;
			ref_ptr<crListControlWidgetNode> cardsaleList = dynamic_cast<crListControlWidgetNode *>(canvas->getWidget(m_itemlist));
			ref_ptr<crEditWidgetNode> inputedit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_inputedit));
			crData *canvasdata = canvas->getDataClass();
			if (cardsaleList.valid())
			{
				ref_ptr<crListControlNode> selectNode = cardsaleList->getSelectNode();
				if(selectNode.valid())
				{
					int seletindex = selectNode->getData();
					if (seletindex>= 0)
					{
						if (canvasdata)
						{
							unsigned char techlv = 0;
							data->excHandle(MAKEINT64(WCH_LockData,1));
							data->getParam(WCHDATA_JXJItemBagVec,param);
							//canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
							JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
							//
							canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
							crBagItemData *itemdata = (crBagItemData *)param;
							if ((*itembagvec)[seletindex] != NULL && itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itembagvec)[seletindex]->getItemID()),record) >= 0)
							{
								itemtype = atoi(record[attrid].c_str());
								itemdata->setItemID((*itembagvec)[seletindex]->getItemID());
								if (itemtype == IT_Equip)
								{
									data->getParam(WCHDATA_JXJSmeltEquipTechLv,param);
									techlv = *(unsigned char *)param;
									if (techlv > 0)
									{
										nowitemcount = itemcount = 1;
										canvasdata->inputParam(WCHDATA_JXJSmelterMateMaxCount,&itemcount);
										if(atoi(record[colorid].c_str()) >= CardColor_Purple)
										{
											success = 1;
										}
										else
										{
											success = -1;//装备品质不足
											tipindex = 2049;
										}
										if (inputedit.valid())
										{
											inputedit->setString(std::string("1"));
											//inputedit->doEvent(WCH_UI_ValueChange);
										}
									}
									else
									{
										success = -4;//前置科技不足
										tipindex = 2095;
									}
								}
								if (itemtype == IT_SmeltMaterial)
								{
									data->getParam(WCHDATA_JXJSmeltMaterialTechLv,param);
									techlv = *(unsigned char *)param;
									if(techlv > 0)
									{
										itemcount = (*itembagvec)[seletindex]->getItemCount();
										if(itemcount >= mincount)
										{
											success = 1;
											canvasdata->inputParam(WCHDATA_JXJSmelterMateMaxCount,&itemcount);
											if (inputedit.valid())
											{
												nowitemcount = mincount;
												inputedit->setString(std::string(crArgumentParser::appItoa(mincount)));
												//inputedit->doEvent(WCH_UI_ValueChange);
											}
											//crFilterRenderManager::getInstance()->doModal(m_inputcountcanvas);
										}
										else
										{
											success = -2;//材料数量不足
											tipindex = 2050;
										}
									}
									else
									{
										success = -4;//前置科技不足
										tipindex = 2095;
									}
								}
								if (itemtype == IT_Gemstone)
								{
									data->getParam(WCHDATA_JXJSmeltStoneTechLv,param);
									techlv = *(unsigned char *)param;
									if (techlv > 0)
									{
										itemcount = (*itembagvec)[seletindex]->getItemCount();
										if(itemcount >= storemincount)
										{
											if(atoi(record[colorid].c_str()) < CardColor_Eight)
											{
												success = 1;
												canvasdata->inputParam(WCHDATA_JXJSmelterMateMaxCount,&itemcount);
												if (inputedit.valid())
												{
													nowitemcount = storemincount;
													inputedit->setString(std::string(crArgumentParser::appItoa(storemincount)));
													//inputedit->doEvent(WCH_UI_ValueChange);
												}
												//crFilterRenderManager::getInstance()->doModal(m_inputcountcanvas);
											}
											else
											{
												success = -3;//品质已达到最高
												tipindex = 2051;
											}
										}
										else
										{
											success = -2;//材料数量不足
											tipindex = 2050;
										}
									}
									else
									{
										success = -4;//前置科技不足
										tipindex = 2095;
									}
								}
								if (success > 0)
								{
									canceltask = true;
									itemdata->setItemCount(nowitemcount);
									itemdata->setEquipMagic((*itembagvec)[seletindex]->getEquipMagic());
									canvasdata->inputParam(WCHDATA_JXJSmeltItemSelectIndex,&seletindex);
									//(*itembagvec)[seletindex] = NULL;
								}
								else
								{
									itemdata->setItemID(0);
									itemdata->setItemCount(0);
									itemdata->setEquipMagic(0);
									playerData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipindex,NULL));//失败
								}
							}
							else
							{
							}
							data->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
			}
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
	handle.outputParam(0,&canceltask);
}
/////////////////////////////////////////
//
//crJXJUIPutMateOnSmelterMethod
//
/////////////////////////////////////////
crJXJPutMateOnSmelterMethod::crJXJPutMateOnSmelterMethod()
{
}
crJXJPutMateOnSmelterMethod::crJXJPutMateOnSmelterMethod(const crJXJPutMateOnSmelterMethod& handle):
	crMethod(handle),
	m_countinput(handle.m_countinput),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJPutMateOnSmelterMethod::inputParam(int i, void *param)
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
void crJXJPutMateOnSmelterMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_countinput = str;
		break;
	case 1:
		m_gongfangcanvas = str;
		break;
	case 2:
		m_itemtyperadio = str;
		break;
	}
}
void crJXJPutMateOnSmelterMethod::operator()(crHandle &handle)
{
	bool cancelTask = false;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		int mincount = 0;
		ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid() && gongfangcanvas.valid())
		{
			crData *data = playerData -> getDataClass();
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			ref_ptr<crEditWidgetNode> editbox = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_countinput));
			if (editbox.valid())
			{
				ref_ptr<crRadioGroupWidgetNode> itemtyperadio = dynamic_cast<crRadioGroupWidgetNode *>(gongfangcanvas->getWidget(m_itemtyperadio));
				if (itemtyperadio.valid())
				{
					if (itemtyperadio->getSelect() == 1)
					{
						mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount,viplv).c_str()));
					}
					else
					{
						mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount,viplv).c_str()));
					}
				}
				int count = atoi(editbox->getUTF8String().c_str());
				if (count >= mincount)
				{
					crData *canvasdata = gongfangcanvas->getDataClass();
					crData *data = playerData->getDataClass();
					if (canvasdata && data)
					{
						//canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
						data->getParam(WCHDATA_JXJItemBagVec,param);
						JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
						canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
						crBagItemData *bag = (crBagItemData *)param;
						canvasdata->getParam(WCHDATA_JXJSmeltItemSelectIndex,param);
						int selectindex = *(int*)param;
						if (bag)
						{
							if (selectindex < itembagvec->size() && (*itembagvec)[selectindex]!=NULL)
							{
								if((*itembagvec)[selectindex]->getItemCount() >= count)
								{
									bag->setItemCount(count);
								}
								else
								{
									bag->setItemID(0);
									bag->setItemCount(0);
								}
							}
						}
					}
				}
				else//数量不足最低数量
				{
					cancelTask = true;
					playerData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2055,NULL));//失败
				}
			}
		}
	}
	handle.outputParam(0,&cancelTask);
}

/////////////////////////////////////////
//
//crJXJCancelPutMateOnSmelterMethod
//
/////////////////////////////////////////
crJXJCancelPutMateOnSmelterMethod::crJXJCancelPutMateOnSmelterMethod()
{
}
crJXJCancelPutMateOnSmelterMethod::crJXJCancelPutMateOnSmelterMethod(const crJXJCancelPutMateOnSmelterMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJCancelPutMateOnSmelterMethod::inputParam(int i, void *param)
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
void crJXJCancelPutMateOnSmelterMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	}
}
void crJXJCancelPutMateOnSmelterMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		if (gongfangcanvas.valid())
		{
			crData *canvasdata = gongfangcanvas->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
				crBagItemData *bag = (crBagItemData *)param;
				if (bag)
				{
					bag->setItemID(0);
					bag->setItemCount(0);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJUISmeltingMethod
//
/////////////////////////////////////////
crJXJUISmeltingMethod::crJXJUISmeltingMethod():
	m_index(0),
	m_font("simhei.ttf"),
	m_characterSize(16),
	m_goldColor(0.0f,1.0f,1.0f,1.0f),
	m_textColor(1.0f,1.0f,0.0f,1.0f)
{
}
crJXJUISmeltingMethod::crJXJUISmeltingMethod(const crJXJUISmeltingMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_textColor(handle.m_textColor),
	m_goldColor(handle.m_goldColor),
	m_font(handle.m_font),
	m_characterSize(handle.m_characterSize),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJUISmeltingMethod::inputParam(int i, void *param)
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
void crJXJUISmeltingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_promptCanvas = str;
		break;
	case 2:
		m_information = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_font = str;
		break;
	case 5:
		m_characterSize = atoi(str.c_str());
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}
void crJXJUISmeltingMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			crData *data = playerGameData->getDataClass();
			crVector2i price;
			bool getstone = false;
			int getstonegold = 0;
			int mymoney = 0;
			int mincount = 0;
			int tipindex = -1;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmeltPriceVec2i,viplv).c_str(),price);
			std::string buildingname;
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec smeltrecord;
			int attrid = itemtab->getTitleIndex("类型");
			int colorid = itemtab->getTitleIndex("品质");
			crData *canvasdata = m_this->getParentCanvas()->getDataClass();
			if (canvasdata)
			{
				int index = m_index;
				canvasdata->inputParam(WCHDATA_JXJSmelterCostType,&index);
				canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
				crBagItemData *bagitemdata = (crBagItemData *)param;
				if (bagitemdata)
				{
					int smeltitemid = bagitemdata->getItemID();
					int smeltitemcount = bagitemdata->getItemCount();
					if (smeltitemid > 0)
					{
						if (smeltitemcount >= 0)
						{
							if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(smeltitemid),smeltrecord) >= 0)
							{
								if(atoi(smeltrecord[attrid].c_str()) == IT_Gemstone)
								{
									mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount,viplv).c_str()));
									if (atoi(smeltrecord[colorid].c_str()) >= CardColor_Eight)
									{
										tipindex = 2051;//五彩石品质已最高
									}
								}
								if(atoi(smeltrecord[attrid].c_str()) == IT_Equip)
								{
									mincount = 1;
									if (bagitemdata->getInlayID() > 0)
									{
										getstone = true;
										getstonegold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmeltInlayStone, viplv).c_str());
									}
									else if (atoi(smeltrecord[colorid].c_str()) < CardColor_Purple)
									{
										tipindex = 2049;//装备品质不足
									}
								}
								if (atoi(smeltrecord[attrid].c_str()) == IT_SmeltMaterial)
								{
									mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount,viplv).c_str()));
								}
								if (tipindex < 0)
								{
									if (smeltitemcount >= mincount)
									{
										tipindex = 0;
									}
									else
									{//数量不足
										tipindex = 2050;//材料不足
									}
								}
							}
							else
							{
								tipindex = 2046;//物品信息出错
							}
						}
						else
						{
							tipindex = 2050;//材料不足
						}
					}
					else
					{
						tipindex = 2075;//未放入熔炼材料
					}
				}
			}
			if (tipindex > 0)
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipindex,NULL));
			}
			else if (tipindex == 0)
			{
				int needbagcount = 1;
				playerGameData->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagcount,NULL));
				if (needbagcount == 0)
				{
					if (getstone)
					{
						m_index = SmeltType_Gold;
						price[m_index] = getstonegold;
					}
					if (m_index == SmeltType_Copper)
					{
						data->getParam(WCHDATA_JXJCoppercash,param);
						mymoney = *(int *)param;
					}
					else
					{
						data->getParam(WCHDATA_JXJGoldingot,param);
						mymoney = *(int *)param;
						data->getParam(WCHDATA_JXJGiftGold,param);
						mymoney += *(int *)param;
					}
					if (mymoney >= price[m_index])
					{
						unsigned int ifhint = 1;
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));
						//if (m_index == SmeltType_Copper)
						//{
						//	ifhint = 0;
						//}
						//else
						//{
						//	data->getParam(WCHDATA_JXJGoldCostHintInt,param);
						//	ifhint = *(unsigned int *)param & JXJGoldCostType_GoldSmeltHint;
						//}
						if (ifhint)
						{
							//int hinttype = JXJGoldCostType_GoldSmeltHint;
							//crData *braindata = crBrain::getInstance()->getDataClass();
							//braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
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
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
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
							if (m_index == SmeltType_Copper)
							{
								sprintf(tmpText,"%d铜钱\0",price[m_index]);
							}
							else
							{
								sprintf(tmpText,"%d礼金/元宝\0",price[m_index]);
							}
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
							std::string tmpText2;
							if (getstone)
								tmpText2 = "从装备上取出";
							else
								tmpText2 =  m_index == SmeltType_Gold ? "高级熔炼" : "熔炼";
							cfg_script.Add("Text",tmpText2 + "八卦石吗？");
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
						int tipindex = 0;
						if (m_index == SmeltType_Copper)
						{
							tipindex = 2032;//铜钱不足
						}
						else
						{
							tipindex = 2036;//元宝不足
						}
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipindex,NULL));
					}
				}
				else
				{//背包不足
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2008,NULL));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSmeltingMethod
//
/////////////////////////////////////////
crJXJSmeltingMethod::crJXJSmeltingMethod():
	m_this(NULL)
{
}
crJXJSmeltingMethod::crJXJSmeltingMethod(const crJXJSmeltingMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSmeltingMethod::inputParam(int i, void *param)
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
void crJXJSmeltingMethod::addParam(int i, const std::string& str)
{
}
void crJXJSmeltingMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
			crBagItemData *bagitemdata = (crBagItemData*)param;
			canvasdata->getParam(WCHDATA_JXJSmelterCostType,param);
			int costtype = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJSmeltItemSelectIndex,param);
			int selectindex = *(int*)param;
			if(bagitemdata)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(11);
				stream->_writeInt(bagitemdata->getItemID());
				stream->_writeUChar(bagitemdata->getItemCount());
				stream->_writeInt(bagitemdata->getEquipMagic());
				stream->_writeUChar((unsigned char)costtype);
				stream->_writeUChar((unsigned char)selectindex);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSmelting,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvSmeltingMethod
//
/////////////////////////////////////////
crJXJRecvSmeltingMethod::crJXJRecvSmeltingMethod():
	m_netType(GameClient_Game){}
crJXJRecvSmeltingMethod::crJXJRecvSmeltingMethod(const crJXJRecvSmeltingMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJRecvSmeltingMethod::inputParam(int i, void *param)
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

void crJXJRecvSmeltingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvSmeltingMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{//
				int itemid = m_stream->_readInt();
				unsigned char count = m_stream->_readUChar();
				int equipmagic = m_stream->_readInt();
				unsigned char costtype = m_stream->_readUChar();
				unsigned char selectindex = m_stream->_readUChar();
				//int *mycash;
				int curcount = 0;

				unsigned char techlv = 0;
				RewardItemVec rewardItems;
				void *param;
				char success = 0;
				int itemtype = 0;
				bool needdeductgold = false;
				crTableIO::StrVec record;
				crVector2i price;
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmeltPriceVec2i,viplv).c_str(),price);
				int mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount,viplv).c_str()));
				int storemincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount,viplv).c_str()));
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				int attrid = itemtab->getTitleIndex("类型");
				int colorid = itemtab->getTitleIndex("品质");
				int levelid = itemtab->getTitleIndex("等级");
				int updateindex = itemtab->getTitleIndex("升级所得物品");
				if (itemtab.valid() && attrid > 0 && colorid > 0 && updateindex > 0)
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					data->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					ref_ptr<crBagItemData> bagItemData;
					if (selectindex < itembagvec->size() && (*itembagvec)[selectindex]!=NULL)
					{
						if ((*itembagvec)[selectindex]->getItemID() == itemid && (*itembagvec)[selectindex]->getEquipMagic() == equipmagic)
						{
							curcount = (*itembagvec)[selectindex]->getItemCount();
							bagItemData = (*itembagvec)[selectindex];
						}
					}
					if (bagItemData.valid() && curcount >= count)
					{
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0 && storemincount > 0)
						{
							itemtype = atoi(record[attrid].c_str());
							if (itemtype == IT_Equip)
							{
								unsigned char inlayid = bagItemData->getInlayID();
								if (inlayid > 0)
								{//八卦石剥离
									ref_ptr<crTableIO>inlaytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInlayTab);
									crTableIO::StrVec inlayRec;
									if (inlaytab->queryOneRecord(1, crArgumentParser::appItoa(inlayid), inlayRec) >= 0)
									{
										int stoneid = atoi(inlayRec[0].c_str());
										crTableIO::StrVec stoneItemRec;
										if (stoneid > 0 && itemtab->queryOneRecord(0, crArgumentParser::appItoa(stoneid), stoneItemRec) >= 0)
										{
											int smeltInlayStoneCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmeltInlayStone, viplv).c_str());
											MoneyChangeData moneydata(smeltInlayStoneCost, "八卦石剥离");
											m_this->doEvent(WCH_JXJDeductLijinGold, MAKEINT64(&moneydata, &needdeductgold));
											if (moneydata.first == 0)
											{
												ref_ptr<crBagItemData> bagitemdata = new crBagItemData;
												bagitemdata->setItemID(stoneid);
												bagitemdata->setItemCount(1);
												rewardItems.push_back(bagitemdata);
												bagItemData->setInlayID(0);
												success = 2;
											}
											else
											{
												success = -6;//元宝不足
											}
										}
										else
										{
											success = 3;
											bagItemData->setInlayID(0);
										}
									}
								}
								else
								{
									data->getParam(WCHDATA_JXJSmeltEquipTechLv, param);
									techlv = *(unsigned char *)param;
									if (techlv > 0)
									{
										if (atoi(record[colorid].c_str()) >= CardColor_Purple)
										{
											success = 1;
										}
										else
										{
											success = -1;//装备品质不足
										}
									}
									else
									{
										success = -7;//科技未研发
									}
								}
							}
							if (itemtype == IT_SmeltMaterial)
							{
								data->getParam(WCHDATA_JXJSmeltMaterialTechLv,param);
								techlv = *(unsigned char *)param;
								if (techlv > 0)
								{
									if(count >= mincount)
									{
										success = 1;
									}
									else
									{
										success = -2;//材料数量不足
									}
								}
								else
								{
									success = -7;//科技未研发
								}
							}
							if (itemtype == IT_Gemstone)
							{
								data->getParam(WCHDATA_JXJSmeltStoneTechLv,param);
								techlv = *(unsigned char *)param;
								if (techlv > 0)
								{
									if(count >= storemincount)
									{
										if(atoi(record[colorid].c_str()) < CardColor_Eight)
										{
											success = 1;
										}
										else
										{
											success = -3;//品质已达到最高
										}
									}
									else
									{
										success = -2;//材料数量不足
									}
								}
								else
								{
									success = -7;//科技未研发
								}
							}
						}
						else
						{
							success = -4;//物品信息出错
						}
					}
					else
					{
						success = -2;//材料数量不足
					}
					if (success == 1)
					{
						if (costtype == SmeltType_Copper)
						{
							m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&price[costtype],NULL));
							if (price[costtype] == 0)
							{
								success = 1;
							}
							else
							{
								success = -5;//铜钱不足
							}
						}
						else
						{
							MoneyChangeData moneydata(price[costtype],"高级熔炼");
							m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
							if (moneydata.first == 0)
							{
								success = 1;
							}
							else
							{
								success = -6;//元宝不足
							}
						}
					}
					if (success == 1)
					{//熔炼条件满足，开始熔炼
						int r = 0;
						short stoneSyntheticPer = 0;
						short materialSyntheticPer = 0;
						short equipSyntheticPer = 0;
						std::vector<unsigned char> newitemcount;// [3] = { 0 };
						crTableIO::StrVec recordStone;
						crTableIO::DataVec recordVec;
						std::vector<float> synthetic,synthetic1,synthetic2;
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJStoneToStoneSynthetic,viplv),synthetic);
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaterialToStoneSynthetic,viplv),synthetic1);
						crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJEquipToStoneSynthetic,viplv),synthetic2);
						itemtab->queryRecords(attrid,crArgumentParser::appItoa(IT_Gemstone),levelid,crArgumentParser::appItoa(1),recordVec);
						ref_ptr<crBagItemData> bagitemdata;
						int stonesize = recordVec.size();
						newitemcount.resize(stonesize, 0);
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
						{
							int newitemid = 0;
							if (itemtype == IT_Gemstone) //
							{
								if (costtype == SmeltType_Copper)
								{
									stoneSyntheticPer = count / storemincount + (atoi(record[levelid].c_str()) * synthetic[0] + synthetic[1]) * count; //消耗铜钱
								}
								else if(costtype == SmeltType_Gold)
								{
									stoneSyntheticPer = count / storemincount + (atoi(record[levelid].c_str()) * synthetic[0] + synthetic[2]) * count; //消耗元宝
								}
								bagitemdata = new crBagItemData;
								newitemid = atoi(record[updateindex].c_str());
								bagitemdata->setItemID(newitemid);
								bagitemdata->setItemCount(stoneSyntheticPer);
								rewardItems.push_back(bagitemdata);
							}
							if (itemtype == IT_SmeltMaterial)
							{
								if (costtype == SmeltType_Copper)
								{
									materialSyntheticPer = (synthetic1[0] + atoi(record[levelid].c_str()) * synthetic1[2] + (atoi(record[colorid].c_str()) + 1) * synthetic1[3]) * count; //消耗铜钱
								}
								else if(costtype == SmeltType_Gold)
								{
									materialSyntheticPer = (synthetic1[1] + atoi(record[levelid].c_str()) * synthetic1[2] + (atoi(record[colorid].c_str()) + 1) * synthetic1[3]) * count; //消耗元宝
								}

								for (int i = 0; i < materialSyntheticPer; i++)
								{
									r = rangei(0,recordVec.size()).get_random();
									//newitemid = atoi(recordVec[r][0].c_str());
									newitemcount[r]++;
								}
								for (int i = 0; i < stonesize; i++)
								{
									if (newitemcount[i] > 0)
									{
										bagitemdata = new crBagItemData;
										bagitemdata->setItemID(atoi(recordVec[i][0].c_str()));
										bagitemdata->setItemCount(newitemcount[i]);
										rewardItems.push_back(bagitemdata);
									}
								}
							}
							if (itemtype == IT_Equip)
							{
								if (costtype == SmeltType_Copper)
								{
									equipSyntheticPer = (synthetic2[2] + synthetic2[4] + (atoi(record[colorid].c_str()) + 1) * synthetic2[3] + synthetic2[0]) * atoi(record[levelid].c_str()); //消耗铜钱
								}
								else if(costtype == SmeltType_Gold)
								{
									equipSyntheticPer = (synthetic2[2] + synthetic2[4] + (atoi(record[colorid].c_str()) + 1) * synthetic2[3] + synthetic2[1]) * atoi(record[levelid].c_str());  //消耗元宝
								}
								for (int i = 0; i < equipSyntheticPer; i++)
								{
									r = rangei(0,recordVec.size()).get_random();
									//newitemid = atoi(recordVec[r][0].c_str());
									//bagitemdata = new crBagItemData;
									newitemcount[r]++;
								}
								for (int i = 0; i < stonesize; i++)
								{
									if (newitemcount[i] > 0)
									{
										bagitemdata = new crBagItemData;
										bagitemdata->setItemID(atoi(recordVec[i][0].c_str()));
										bagitemdata->setItemCount(newitemcount[i]);
										rewardItems.push_back(bagitemdata);
									}
								}
							}
							curcount -= count;
							if (curcount > 0)
							{
								(*itembagvec)[selectindex]->setItemCount(curcount);
							}
							else
							{
								(*itembagvec)[selectindex] = NULL;
							}
						}
					}
					data->excHandle(MAKEINT64(WCH_LockData,0));
				}

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(3);
					stream->_writeChar(success);
					stream->_writeUChar(selectindex);
					stream->_writeUChar(curcount);
				}
				else if (success >= 2)
				{
					stream->createBuf(2);
					stream->_writeChar(success);
					stream->_writeUChar(selectindex);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSmelting,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				//
				if (!rewardItems.empty())
				{
					data->excHandle(MAKEINT64(WCH_LockData, 1));
					m_this->doEvent(WCH_JXJRecvRewardItems, MAKEINT64(&rewardItems, 0));
					data->excHandle(MAKEINT64(WCH_LockData, 0));
				}
				if (needdeductgold)
				{//扣除了元宝，立即存储
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
				if (success > 0)
				{///游戏日志
					std::string logdata = crArgumentParser::appVectoa(crVector4i(itemid,count,equipmagic,costtype));
					GameLogData gamelog(Log_Smelting,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			if (success > 0)
			{
				unsigned char selectindex = m_stream->_readUChar();

				ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
				if (gongfangcanvas.valid())
				{
					crData *gongfangdata = gongfangcanvas->getDataClass();
					if (gongfangdata)
					{
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,1));
						gongfangdata->getParam(WCHDATA_JXJSmelterItemData,param);
						crBagItemData *bagitemdata = (crBagItemData *)param;
						bagitemdata->setItemID(0);
						bagitemdata->setItemCount(0);
						bagitemdata->setEquipMagic(0);
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
				crData *data = m_this -> getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				if (selectindex < itembagvec->size() && (*itembagvec)[selectindex]!=NULL)
				{
					if (success == 1)
					{
						unsigned char itemcount = m_stream->_readUChar();
						if (itemcount > 0)
						{
							(*itembagvec)[selectindex]->setItemCount(itemcount);
						}
						else
						{
							(*itembagvec)[selectindex] = NULL;
						}
					}
					else if (success >= 2)
					{
						(*itembagvec)[selectindex]->setInlayID(0);
						if (success == 3)
						{
							int index = 1126;//该八卦石已经不存在
							m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKEINT64(index, NULL));
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				int index = 0;
				switch (success)
				{
				case 0:
					index = 2029;//操作失败
					break;
				case -1:
					index = 2049;//装备品质不足
					break;
				case -2:
					index = 2050;//材料不足
					break;
				case -3:
					index = 2051;//五彩石品质已最高
					break;
				case -4:
					index = 2046;//物品信息出错
					break;
				case -5:
					index = 2032;//铜钱不足
					break;
				case -6:
					index = 2036;//元宝不足
					break;
				case -7:
					index = 2095;//科技未研发
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAttachStoveUpdateMethod
//
/////////////////////////////////////////
crJXJAttachStoveUpdateMethod::crJXJAttachStoveUpdateMethod(){}
crJXJAttachStoveUpdateMethod::crJXJAttachStoveUpdateMethod(const crJXJAttachStoveUpdateMethod& handle):
	crMethod(handle),
	m_myStoreList(handle.m_myStoreList),
	m_pagenum(handle.m_pagenum),
	m_defaulticon(handle.m_defaulticon),
	m_equipicon(handle.m_equipicon),
	m_mateicon(handle.m_mateicon),
	m_attachbtn(handle.m_attachbtn)
{
}
void crJXJAttachStoveUpdateMethod::inputParam(int i, void *param)
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
void crJXJAttachStoveUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_myStoreList = str;
		break;
	case 1:
		m_pagenum = str;
		break;
	case 2:
		m_equipicon = str;
		break;
	case 3:
		m_mateicon = str;
		break;
	case 4:
		m_attachbtn = str;
		break;
	case 5:
		m_defaulticon = str;
		break;
	}
}
void crJXJAttachStoveUpdateMethod::operator()(crHandle &handle)
{
	//ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this)
	{
		crData *data = playerData->getDataClass();
		bool ifshow = false;
		void *param;
		int typeitem = 0,curpage = 1;
		crData *canvasdata = m_this->getDataClass();
		if (canvasdata)
		{
			canvasdata -> getParam(WCHDATA_JXJAttachMateCurPage,param);
			curpage = *(int *)param;
		//	canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			ref_ptr<crListControlWidgetNode> myStoreList = dynamic_cast<crListControlWidgetNode *>(m_this->getWidget(m_myStoreList));
			ref_ptr<crStaticTextWidgetNode> pagenum = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_pagenum));
			ref_ptr<crImageBoxWidgetNode> equipicon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_equipicon));
			ref_ptr<crImageBoxWidgetNode> mateicon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mateicon));
			ref_ptr<crButtonWidgetNode> attachbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_attachbtn));
			crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			ref_ptr<crTableIO>itemtab2 = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
			crTableIO::StrVec itemrecord,itemrecord2,smeltrecord;
			int nameid = itemtab->getTitleIndex("name");
			int iconid = itemtab->getTitleIndex("icon");
			int attrid = itemtab->getTitleIndex("类型");
			int colorid = itemtab->getTitleIndex("品质");
			canvasdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
			crBagItemData *matedata = (crBagItemData *)param;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			if(myStoreList.valid())
			{
				//下边的列表
				canvasdata->getParam(WCHDATA_JXJAttachMateSelectIndex,param);
				int selectindex = *(int *)param;
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int totalcount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBagNumMax,viplv).c_str()));
				int onepagecount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAttachMateOnePageCount,viplv).c_str()));
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJOpenBagNum,param);
				unsigned char openbagnum = *(unsigned char *)param;
				std::string iconfile;
				std::string title,title2;
				myStoreList->setListNodeCount(totalcount);
				myStoreList->setStartYPos(curpage - 1);
				crListControlWidgetNode::ListNodeVec& listNodeVec = myStoreList->getListNodeVec();
				int i = 0;
				int count = 0;
				//itembagvec->resize(openbagnum);
				int newdata = 0;
				for( JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr,++i,++newdata )
				{
					ifshow = true;
					if(i>=0 && i<openbagnum)
					{
						if((*itr)!=NULL && (*itr)->getItemID()!=0)
						{
							if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itr)->getItemID()),itemrecord) >= 0)
							{
								int nowcount = (*itr)->getItemCount();
								if (atoi(itemrecord[attrid].c_str())!=IT_Gemstone)
								{
									ifshow = false;
								}
								if (newdata == selectindex && matedata && matedata->getItemCount() > 0 && matedata->getItemID() > 0)
								{
									nowcount -= matedata->getItemCount();
								}
								if (nowcount == 0)
								{
									ifshow = false;
								}
								if (!ifshow)
								{
									i--;
									continue;
								}
								title = itemrecord[nameid];
								iconfile = itemrecord[iconid];
								//	listNodeVec[i]->setTitle(title);
								listNodeVec[i]->setFont("simhei.ttf");
								if (nowcount > 1)
								{
									title2 = crArgumentParser::appItoa(nowcount) + std::string(" ");;
								}
								else
								{
									title2.clear();
								}
								listNodeVec[i]->setTitle2(title2);
								listNodeVec[i]->setData(/*curpage*onepagecount+*/newdata);
								listNodeVec[i]->setImageName(iconfile);
								listNodeVec[i]->setVisiable(true);
								listNodeVec[i]->setEnable(true);
								count++;
							}
						}
						else
						{
							i--;
						}
					}
				}
				for(i;i<openbagnum;i++)
				{
					listNodeVec[i]->setTitle("");
					listNodeVec[i]->setTitle2("");
					listNodeVec[i]->setData(-1);
					listNodeVec[i]->setImageName(m_defaulticon);
					listNodeVec[i]->setVisiable(true);
					listNodeVec[i]->setEnable(false);
				}
				for(int j = openbagnum; j < totalcount; j++)
				{
					listNodeVec[j]->setData(j);
					listNodeVec[j]->setImageName("");
					//title = "暂未购买";
					listNodeVec[j]->setTitle("");
					//title = "点击购买";
					listNodeVec[j]->setTitle2("");
					listNodeVec[j]->setEnable(true);
					listNodeVec[j]->setVisiable(false);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				//if(i<0)i=0;
				int totalpage = ceil((float)count/(float)onepagecount);
				if (totalpage < 1)
				{
					totalpage = 1;
				}
				canvasdata -> inputParam(WCHDATA_JXJAttachMateTotalPage,&totalpage);
				if(pagenum.valid())pagenum->setString(crArgumentParser::appItoa(curpage)+"/"+crArgumentParser::appItoa(totalpage));
			}
			//右边的炉子
			if(equipicon.valid())equipicon->setVisiable(false);
			if(mateicon.valid())mateicon->setVisiable(false);
			//if(attachbtn)attachbtn->setEnable(false);
			canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData *)param;
			bool ifequip = false;
			bool ifmate = false;
			if (equipdata)
			{
				int equipid = equipdata->getItemID();
				int equipcount = equipdata->getItemCount();
				if (equipid > 0 && equipcount > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(equipid),smeltrecord) >= 0)
				{
					if (equipicon.valid())
					{
						equipicon->setVisiable(true);
						equipicon->setImageName(smeltrecord[iconid]);
					}
					ifequip = true;
				}
			}
			if (matedata)
			{
				int mateid = matedata->getItemID();
				int matecount = matedata->getItemCount();
				if (mateid > 0 && matecount > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mateid),smeltrecord) >= 0)
				{
					if (mateicon.valid())
					{
						mateicon->setVisiable(true);
						mateicon->setImageName(smeltrecord[iconid]);
					}
					ifmate = true;
				}
			}
			//if (ifequip && ifmate)
			//{
			//	if (attachbtn)attachbtn->setEnable(true);
			//}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISetInputNumMethod
//
/////////////////////////////////////////
crJXJUISetInputNumMethod::crJXJUISetInputNumMethod():
	m_this(NULL)
{
}
crJXJUISetInputNumMethod::crJXJUISetInputNumMethod(const crJXJUISetInputNumMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_input(handle.m_input),
	m_step(handle.m_step)
{
}
void crJXJUISetInputNumMethod::inputParam(int i, void *param)
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
void crJXJUISetInputNumMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_step = str;
		break;
	}
}
void crJXJUISetInputNumMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
		if(input.valid())
		{
			int nowcount = atoi((input->getUTF8String()).c_str());
			int step = atoi(m_step.c_str());
			nowcount+=step;
			input->setString(crArgumentParser::appItoa(nowcount));
			input->doEvent(WCH_UI_ValueChange);
		}

	}
}

/////////////////////////////////////////
//
//crJXJAttachStoreMethod
//
/////////////////////////////////////////
crJXJAttachStoreMethod::crJXJAttachStoreMethod():
	m_this(NULL)
{
}
crJXJAttachStoreMethod::crJXJAttachStoreMethod(const crJXJAttachStoreMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJAttachStoreMethod::inputParam(int i, void *param)
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
void crJXJAttachStoreMethod::addParam(int i, const std::string& str)
{
}
void crJXJAttachStoreMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData*)param;
			canvasdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
			crBagItemData *matedata = (crBagItemData*)param;
			canvasdata->getParam(WCHDATA_JXJAttachEquipSelectIndex,param);
			int equipselectindex = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJAttachMateSelectIndex,param);
			int mateselectindex = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJRefineEquipSuitIndex,param);
			int suitIndex = *(int*)param;
			if(equipdata && matedata && mateselectindex >= 0)
			{
				if (equipdata->getItemID() <= 0)
				{
					myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2052,NULL));//装备出错
				}
				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(16);
					stream->_writeInt(equipdata->getItemID());
					stream->_writeInt(equipdata->getEquipMagic());
					stream->_writeShort((short)equipselectindex);
					stream->_writeInt(matedata->getItemID());
					stream->_writeUChar((unsigned char)mateselectindex);
					stream->_writeChar((char)suitIndex);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAttachStore,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvAttachStoreMethod
//
/////////////////////////////////////////
crJXJRecvAttachStoreMethod::crJXJRecvAttachStoreMethod():
	m_netType(GameClient_Game){}
crJXJRecvAttachStoreMethod::crJXJRecvAttachStoreMethod(const crJXJRecvAttachStoreMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas),
	m_successdonghua(handle.m_successdonghua)
{
}
void crJXJRecvAttachStoreMethod::inputParam(int i, void *param)
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

void crJXJRecvAttachStoreMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_successdonghua = str;
		break;
	default:
		break;
	}
}

void crJXJRecvAttachStoreMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{//
				char success = 1;//-1:该装备不能附炼 -2:装备信息有误 -3:材料不足 -4:该材料不能附炼
				int equipid = m_stream->_readInt();
				//ref_ptr<crTableIO>equipInlayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipinlayTab);
				crTableIO::StrVec record;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				crData *data = m_this->getDataClass();
				crData *roleData = m_this->getMainRole()->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				//roleData->excHandle(MAKEINT64(WCH_LockData,1));
				int equipmagic = m_stream->_readInt();
				short equipindex = m_stream->_readShort();
				int mateid = m_stream->_readInt();
				unsigned char mateindex = m_stream->_readUChar();
				char suitIndex = m_stream->_readChar();
				do 
				{
					if (equipid <= 0)
					{
						success = -2;//该装备不能附炼
						break;
					}
					//if(!equipInlayTab->queryOneRecord(0,crArgumentParser::appItoa(equipid),record) >= 0)
					//{
					//	success = -1;//该装备不能附炼
					//	break;
					//}
					//int *mycash;
					void *param;
					unsigned char newinlayid = 0;
					unsigned char partid = 0;
					unsigned char stonecount = 0;
					crVector2i price;
					data -> getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmeltPriceVec2i,viplv).c_str(),price);
					int mincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateMinCount,viplv).c_str()));
					int storemincount = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSmelterMateStoreMinCount,viplv).c_str()));
					ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
					int attrid = itemtab->getTitleIndex("类型");
					int colorid = itemtab->getTitleIndex("品质");
					data->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					bool hasequip = false;
					bool hasmate = false;
					JXJLordEquipVec::iterator litr;
					if (equipindex < 0)
					{
						//roleData->getParam(WCHDATA_JXJLordEquipVec,param);
						//JXJLordEquipVec *lordequipvec = (JXJLordEquipVec *)param;
						//for (litr = lordequipvec->begin();
						//	litr != lordequipvec->end(); 
						//	++litr,++partid)
						//{
						//	if (litr->first == equipid && litr->second == equipmagic)
						//	{
						//		hasequip = true;
						//		break;
						//	}
						//}
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJLordEquipVecs,param);
						std::vector<JXJLordEquipVec>* openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
						if (suitIndex >=0 && suitIndex < openedEquipVec->size())
						{
							JXJLordEquipVec &lordequipvec = (*openedEquipVec)[suitIndex];
							for (litr = lordequipvec.begin();
								litr != lordequipvec.end(); 
								++litr,++partid)
							{
								if (litr->first == equipid && litr->second == equipmagic)
								{
									hasequip = true;
									break;
								}
							}
						}						
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					else
					{
						if (equipindex < itembagvec->size() && (*itembagvec)[equipindex] != NULL)
						{
							if ((*itembagvec)[equipindex]->getItemID() == equipid && (*itembagvec)[equipindex]->getEquipMagic() == equipmagic &&
								(*itembagvec)[equipindex]->getItemCount() > 0)
							{
								hasequip = true;
							}
						}
					}
					if (!hasequip)
					{
						success = -2;//装备信息有误
						break;
					}
					if (mateindex < itembagvec->size() && (*itembagvec)[mateindex] != NULL)
					{
						if ((*itembagvec)[mateindex]->getItemID() == mateid && (*itembagvec)[mateindex]->getItemCount() > 0
							&& itemtab->queryOneRecord(0,crArgumentParser::appItoa(mateid),record) >= 0)
						{
							if (atoi(record[attrid].c_str()) == IT_Gemstone)
							{
								hasmate = true;
							}
						}
					}
					if (!hasmate)
					{
						success = -3;//材料不足
						break;
					}
					ref_ptr<crTableIO>inlayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInlayTab);
					crTableIO::DataVec inlayrecores;
					inlayTab->queryRecords(0,crArgumentParser::appItoa(mateid),inlayrecores);
					if(inlayrecores.empty())
					{
						success = -4;//该材料不能附炼
						break;
					}
					//附炼条件满足，开始附炼
					newinlayid = 1;
					//随机inlayid
					int weight = 0;
					std::vector<int>WeightVec;
					int inlayindex = inlayTab->getTitleIndex("inlayid");
					int weightindex =  inlayTab->getTitleIndex("掉落几率");
					for( crTableIO::DataVec::iterator itr = inlayrecores.begin();
						 itr != inlayrecores.end();
						 ++itr )
					{
						weight += atoi((*itr)[weightindex].c_str());
						WeightVec.push_back(weight);
					}
					int r = rangei(0,weight).get_random();
					int weightcount = WeightVec.size();
					for( int i = 0; i<weightcount; i++ )
					{
						if(r<=WeightVec[i])
						{
							newinlayid = atoi(inlayrecores[i][inlayindex].c_str());
							break;
						}
					}
					_crInt16 lo16 = LOINT32(equipmagic);
					_crInt16 hi16 = HIINT32(equipmagic);
					equipmagic = MAKEINT32(lo16,MAKEINT16(LOINT16(hi16),newinlayid));
					if (equipindex < 0)
					{
						litr->second = equipmagic;
					}
					else
					{
						(*itembagvec)[equipindex]->setEquipMagic(equipmagic);
					}
					stonecount = (*itembagvec)[mateindex]->getItemCount() - 1;
					if (stonecount > 0)
					{
						(*itembagvec)[mateindex]->setItemCount(stonecount);
					}
					else
					{
						(*itembagvec)[mateindex] = NULL;
					}

					stream->createBuf(11);
					stream->_writeChar(success);
					stream->_writeShort(equipindex);
					stream->_writeUChar(partid);
					stream->_writeInt(equipmagic);
					stream->_writeUChar(mateindex);
					stream->_writeChar(suitIndex);
					stream->_writeUChar(stonecount);
				} while (0);
				//roleData->excHandle(MAKEINT64(WCH_LockData,0));
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if(success<0)
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAttachStore,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = "附炼"+crArgumentParser::appVectoa(crVector3i(success,equipid,mateid),'|');
				GameLogData gamelog(Log_AttachStone,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			if (success == 1)
			{
				int itemid = 0;
				short equipindex = m_stream->_readShort();
				unsigned char partid = m_stream->_readUChar();
				int equipmagic = m_stream->_readInt();
				unsigned char mateindex = m_stream->_readUChar();
				char suitIndex = m_stream->_readChar();
				unsigned char stonecount = m_stream->_readUChar();

				crData *data = m_this -> getDataClass();
				crRole *mainRole = m_this->getMainRole();
				crData *roleData = mainRole->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				//roleData->getParam(WCHDATA_JXJLordEquipVec,param);
				//JXJLordEquipVec *lordequipvec = (JXJLordEquipVec *)param;
				if (mateindex < itembagvec->size() && (*itembagvec)[mateindex]!=NULL)
				{
					if (stonecount > 0)
					{
						(*itembagvec)[mateindex]->setItemCount(stonecount);
					}
					else
					{
						(*itembagvec)[mateindex] = NULL;
					}
				}
				if (equipindex >= 0)
				{
					if (equipindex < itembagvec->size() && (*itembagvec)[equipindex]!=NULL)
					{
						(*itembagvec)[equipindex]->setEquipMagic(equipmagic);
						itemid = (*itembagvec)[equipindex]->getItemID();
					}
				}
				else
				{
					roleData->getParam(WCHDATA_JXJLordEquipVecs,param);
					std::vector<JXJLordEquipVec>* openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
					if (suitIndex>=0 && suitIndex<openedEquipVec->size())
					{
						JXJLordEquipVec& lordequipvec = (*openedEquipVec)[suitIndex];
						if (partid <lordequipvec.size())
						{
							lordequipvec[partid].second = equipmagic;
							itemid = lordequipvec[partid].first;
						}
					}
				}
				ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
				ref_ptr<crCanvasNode>successdonghua = crFilterRenderManager::getInstance()->findCanvas(m_successdonghua);
				if (successdonghua.valid())
				{
					ref_ptr<crHypertextWidgetNode> successinput = dynamic_cast<crHypertextWidgetNode *>(successdonghua->getWidget("UI_GongFang_Fl_Input"));
					if (successinput.valid())
					{
						_crInt16 hi16 = HIINT32(equipmagic);
						unsigned char inlayid = HIINT16(hi16);
						int newequipmagic = MAKEINT32(0,MAKEINT16(0,inlayid));
						crVector2i newitem = crVector2i(itemid,newequipmagic);
						ItemMap itemMap;
						rcfg::ConfigScript outscript(&itemMap);
						m_this->doEvent(WCH_JXJCreateItemRefineTips,MAKEINT64(&newitem,&outscript));
						successinput->setHypertext(outscript);
					}
					crFilterRenderManager::getInstance()->showCanvas(successdonghua.get(),true);

				}
				if (gongfangcanvas.valid())
				{
					crData *gongfangdata = gongfangcanvas->getDataClass();
					if (gongfangdata)
					{
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,1));
						gongfangdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
						crBagItemData *equipdata = (crBagItemData *)param;
						gongfangdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
						crBagItemData *matedata = (crBagItemData *)param;
						//equipdata->setItemID(0);
						//equipdata->setItemCount(0);
						equipdata->setEquipMagic(equipmagic);
						matedata->setItemID(0);
						matedata->setItemCount(0);
						matedata->setEquipMagic(0);
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				int index = 0;
				switch (success)
				{//-1:该装备不能附炼 -2:装备信息有误 -3:材料不足 -4:该材料不能附炼
				case 0:
					index = 2029;//操作失败
					break;
				case -1:
					index = 2053;
					break;
				case -2:
					index = 2052;
					break;
				case -3:
					index = 2050;
					break;
				case -4:
					index = 2054;
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJPutMateOnStoveMethod
//
/////////////////////////////////////////
crJXJPutMateOnStoveMethod::crJXJPutMateOnStoveMethod()
{
}
crJXJPutMateOnStoveMethod::crJXJPutMateOnStoveMethod(const crJXJPutMateOnStoveMethod& handle):
	crMethod(handle),
	m_gamelist(handle.m_gamelist),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJPutMateOnStoveMethod::inputParam(int i, void *param)
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
void crJXJPutMateOnStoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_gamelist = str;
		break;
	}
}
void crJXJPutMateOnStoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		crData *data = playerData->getDataClass();
		void *param;
		int itemtype = 0;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int attrid = itemtab->getTitleIndex("类型");
		if (canvas.valid() && itemtab.valid() && attrid > 0)
		{
			ref_ptr<crListControlWidgetNode> cardsaleList = dynamic_cast<crListControlWidgetNode *>(canvas->getWidget(m_gamelist));
			crData *canvasdata = canvas->getDataClass();
			if (cardsaleList.valid())
			{
				ref_ptr<crListControlNode> selectNode = cardsaleList->getSelectNode();
				if(selectNode.valid())
				{
					int seletindex = selectNode->getData();
					if (seletindex>= 0)
					{
						if (canvasdata)
						{
							data->excHandle(MAKEINT64(WCH_LockData,1));
							data->getParam(WCHDATA_JXJItemBagVec,param);
							JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
							//
							if ((*itembagvec)[seletindex] != NULL && itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itembagvec)[seletindex]->getItemID()),record) >= 0)
							{
								itemtype = atoi(record[attrid].c_str());
								if (itemtype == IT_Gemstone)
								{
									canvasdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
									crBagItemData *itemdata = (crBagItemData *)param;
									itemdata->setItemID((*itembagvec)[seletindex]->getItemID());
									itemdata->setItemCount(1);
									canvasdata->inputParam(WCHDATA_JXJAttachMateSelectIndex,&seletindex);
								}
							}
							else
							{
							}
							data->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCancelPutMateOnStoveMethod
//
/////////////////////////////////////////
crJXJCancelPutMateOnStoveMethod::crJXJCancelPutMateOnStoveMethod()
{
}
crJXJCancelPutMateOnStoveMethod::crJXJCancelPutMateOnStoveMethod(const crJXJCancelPutMateOnStoveMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJCancelPutMateOnStoveMethod::inputParam(int i, void *param)
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
void crJXJCancelPutMateOnStoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	}
}
void crJXJCancelPutMateOnStoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		if (gongfangcanvas.valid())
		{
			crData *canvasdata = gongfangcanvas->getDataClass();
			if (canvasdata)
			{
				int selectindex = -1;
				canvasdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
				canvasdata->inputParam(WCHDATA_JXJAttachMateSelectIndex,&selectindex);
				crBagItemData *bag = (crBagItemData *)param;
				if (bag)
				{
					bag->setItemID(0);
					bag->setItemCount(0);
					bag->setEquipMagic(0);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJPutEquipOnStoveMethod
//
/////////////////////////////////////////
crJXJPutEquipOnStoveMethod::crJXJPutEquipOnStoveMethod():
	m_index(-1)
{
}
crJXJPutEquipOnStoveMethod::crJXJPutEquipOnStoveMethod(const crJXJPutEquipOnStoveMethod& handle):
	crMethod(handle),
	m_gamelist(handle.m_gamelist),
	m_gongfangcanvas(handle.m_gongfangcanvas),
	m_gongfangradio(handle.m_gongfangradio),
	m_index(handle.m_index),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_promptCanvas(handle.m_promptCanvas),
	m_gamelist2(handle.m_gamelist2)
{
}
void crJXJPutEquipOnStoveMethod::inputParam(int i, void *param)
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
void crJXJPutEquipOnStoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_gamelist = str;
		break;
	case 2:
		m_gongfangradio = str;
		break;
	case 3:
		m_index = atoi(str.c_str());
		break;
	case 4:
		m_promptCanvas = str;
		break;
	case 5:
		m_information = str;
		break;
	case 6:
		m_okbtn = str;
		break;
	case 7:
		m_gamelist2 = str;
		break;
	case 8:
	case 9:
		m_workshopRadio[i-8] = str;
		break;
	}
}
void crJXJPutEquipOnStoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		crData *data = playerData->getDataClass();
		crData *mainroledata = playerData->getMainRole()->getDataClass();
		void *param;
		int itemtype = 0;
		int itemid = 0;
		int seletindex = -1;
		_crInt32 magic = 0;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int attrid = itemtab->getTitleIndex("类型");
		if (data && mainroledata && canvas.valid() && itemtab.valid() && attrid > 0 && canvas->getDataClass())
		{
			ref_ptr<crRadioGroupWidgetNode> gongfangradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_gongfangradio));
			crData *canvasdata = canvas->getDataClass();
			ref_ptr<crRadioGroupWidgetNode> workshopRadio;// = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_workshopRadio));
			int equipIndex = -1;//workshopRadio.valid()?workshopRadio->getSelect():-1;
			if (m_index < 0)
			{
				ref_ptr<crListControlWidgetNode> cardsaleList = NULL;
				if (gongfangradio->getSelect() == GongfangType_JingLian)
				{
					cardsaleList = dynamic_cast<crListControlWidgetNode *>(canvas->getWidget(m_gamelist));
				}
				else if (gongfangradio->getSelect() == GongfangType_FuLian)
				{
					cardsaleList = dynamic_cast<crListControlWidgetNode *>(canvas->getWidget(m_gamelist2));
				}
				if (cardsaleList.valid())
				{
					ref_ptr<crListControlNode> selectNode = cardsaleList->getSelectNode();
					if(selectNode.valid())
					{
						seletindex = selectNode->getData();
						if (seletindex>= 0)
						{
							data->excHandle(MAKEINT64(WCH_LockData,1));
							data->getParam(WCHDATA_JXJItemBagVec,param);
							JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
							//
							if ((*itembagvec)[seletindex] != NULL && (*itembagvec)[seletindex]->getItemCount() > 0)
							{
								itemid = (*itembagvec)[seletindex]->getItemID();
								magic = (*itembagvec)[seletindex]->getEquipMagic();
							}
							data->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
				}
			}
			else 
			{
				if (gongfangradio->getSelect() == GongfangType_JingLian)
				{
					workshopRadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_workshopRadio[0]));
					if(workshopRadio.valid())
						equipIndex = workshopRadio->getSelect();
				}
				else if (gongfangradio->getSelect() == GongfangType_FuLian)
				{
					workshopRadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_workshopRadio[1]));
					if(workshopRadio.valid())
						equipIndex = workshopRadio->getSelect();
				}
				JXJLordEquipVec lordequipvec;
				if (workshopRadio.valid())
				{
					mainroledata->getParam(WCHDATA_JXJLordEquipVecs, param);
					std::vector<JXJLordEquipVec> openedEquipVec = *(std::vector<JXJLordEquipVec> *)param;
					if (equipIndex >=0 && equipIndex < openedEquipVec.size())
					{
						lordequipvec = openedEquipVec[equipIndex];
					}
				}else
				{
					mainroledata->getParam(WCHDATA_JXJLordEquipVec,param);
					lordequipvec = *(JXJLordEquipVec *)param;
				}
				
				
				if (m_index < lordequipvec.size())
				{
					itemid = lordequipvec[m_index].first;
					magic = lordequipvec[m_index].second;
				}
			}
			if (itemid > 0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0)
			{
				itemtype = atoi(record[attrid].c_str());
				if (itemtype == IT_Equip)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					if (gongfangradio->getSelect() == GongfangType_JingLian)
					{
						canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
						int afterrefinemagic = *(int *)param;
						if (afterrefinemagic == 0)
						{
							canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
							crBagItemData *itemdata = (crBagItemData *)param;
							itemdata->setItemID(itemid);
							itemdata->setItemCount(1);
							itemdata->setEquipMagic(magic);
							canvasdata->inputParam(WCHDATA_JXJRefineEquipSelectIndex,&seletindex);
							canvasdata->inputParam(WCHDATA_JXJRefineEquipSuitIndex,&equipIndex);
						}
						else
						{
							ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
							if (tipscanvas.valid())
							{
								ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
								ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
								std::vector<float>v_i;
								ItemMap itemMap;
								rcfg::ConfigScript cfg_script(&itemMap);
								cfg_script.Add("Hypertext");
								cfg_script.Push("Hypertext");

								cfg_script.Add("Content");
								cfg_script.Push("Content",1);
								v_i.clear();
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","您确认要重新精炼吗？\n");
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",2);
								v_i.clear();
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(0.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","之前的精炼将不会被保存！");
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
						}
					}
					if (gongfangradio->getSelect() == GongfangType_FuLian)
					{
						canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
						crBagItemData *itemdata = (crBagItemData *)param;
						itemdata->setItemID(itemid);
						itemdata->setItemCount(1);
						itemdata->setEquipMagic(magic);
						canvasdata->inputParam(WCHDATA_JXJAttachEquipSelectIndex,&seletindex);
						canvasdata->inputParam(WCHDATA_JXJRefineEquipSuitIndex,&equipIndex);
					}
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJCancelPutEquipOnStoveMethod
//
/////////////////////////////////////////
crJXJCancelPutEquipOnStoveMethod::crJXJCancelPutEquipOnStoveMethod()
{
}
crJXJCancelPutEquipOnStoveMethod::crJXJCancelPutEquipOnStoveMethod(const crJXJCancelPutEquipOnStoveMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas),
	m_gongfangradio(handle.m_gongfangradio)
{
}
void crJXJCancelPutEquipOnStoveMethod::inputParam(int i, void *param)
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
void crJXJCancelPutEquipOnStoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_gongfangradio = str;
		break;
	}
}
void crJXJCancelPutEquipOnStoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		if (gongfangcanvas.valid())
		{
			ref_ptr<crRadioGroupWidgetNode> gongfangradio = dynamic_cast<crRadioGroupWidgetNode *>(gongfangcanvas->getWidget(m_gongfangradio));
			crData *canvasdata = gongfangcanvas->getDataClass();
			if (canvasdata)
			{
				int selectindex = -1;
				crBagItemData *bag = NULL;
				if (gongfangradio.valid())
				{
					if (gongfangradio->getSelect() == GongfangType_JingLian)
					{
						canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
						canvasdata->inputParam(WCHDATA_JXJRefineEquipSelectIndex,&selectindex);
						canvasdata->inputParam(WCHDATA_JXJRefineEquipSuitIndex,0);
						
						bag = (crBagItemData *)param;
					}
					if (gongfangradio->getSelect() == GongfangType_FuLian)
					{
						canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
						canvasdata->inputParam(WCHDATA_JXJAttachEquipSelectIndex,&selectindex);
						canvasdata->inputParam(WCHDATA_JXJRefineEquipSuitIndex,0);
						bag = (crBagItemData *)param;
					}
					if (bag)
					{
						bag->setItemID(0);
						bag->setItemCount(0);
						bag->setEquipMagic(0);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClearRefineStoveMethod
//
/////////////////////////////////////////
crJXJClearRefineStoveMethod::crJXJClearRefineStoveMethod()
{
}
crJXJClearRefineStoveMethod::crJXJClearRefineStoveMethod(const crJXJClearRefineStoveMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas),
	m_gongfangradio(handle.m_gongfangradio)
{
}
void crJXJClearRefineStoveMethod::inputParam(int i, void *param)
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
void crJXJClearRefineStoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	case 1:
		m_gongfangradio = str;
		break;
	}
}
void crJXJClearRefineStoveMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
		if (gongfangcanvas.valid())
		{
			//ref_ptr<crRadioGroupWidgetNode> gongfangradio = dynamic_cast<crRadioGroupWidgetNode *>(gongfangcanvas->getWidget(m_gongfangradio));
			crData *canvasdata = gongfangcanvas->getDataClass();
			if (canvasdata)
			{
				int selectindex = -1;
				crBagItemData *bag = NULL;
				//if (gongfangradio)
				//{
				//	if (gongfangradio->getSelect() == GongfangType_JingLian)
				//	{
				canvasdata->inputParam(WCHDATA_JXJRefineEquipSelectIndex,&selectindex);
				canvasdata->inputParam(WCHDATA_JXJRefineEquipSuitIndex,0);
				
				canvasdata->inputParam(WCHDATA_JXJAfterRefineEquipMagic,0);
				canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
				bag = (crBagItemData *)param;
				//	}
				if (bag)
				{
					bag->setItemID(0);
					bag->setItemCount(0);
					bag->setEquipMagic(0);
				}
			//	}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIEquipRefineMethod
//
/////////////////////////////////////////
crJXJUIEquipRefineMethod::crJXJUIEquipRefineMethod():
	m_index(0),
	m_font("simhei.ttf"),
	m_characterSize(16),
	m_goldColor(0.0f,1.0f,1.0f,1.0f),
	m_textColor(1.0f,1.0f,0.0f,1.0f)
{
}
crJXJUIEquipRefineMethod::crJXJUIEquipRefineMethod(const crJXJUIEquipRefineMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_textColor(handle.m_textColor),
	m_goldColor(handle.m_goldColor),
	m_font(handle.m_font),
	m_characterSize(handle.m_characterSize),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJUIEquipRefineMethod::inputParam(int i, void *param)
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
void crJXJUIEquipRefineMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_promptCanvas = str;
		break;
	case 2:
		m_information = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_font = str;
		break;
	case 5:
		m_characterSize = atoi(str.c_str());
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}
void crJXJUIEquipRefineMethod::operator()(crHandle &handle)
{
	void *param;
	bool ifCancelTask = false;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			std::string jinglian;
			int needgold = 0;
			int tempneedgold = 0;
			int ifhint = 1;
			std::string buildingname;
			crData *data = playerGameData->getDataClass();
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crData *canvasdata = m_this->getParentCanvas()->getDataClass();
			if (canvasdata)
			{			
				data->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
				unsigned short freerefinecount = *(unsigned short *)param;
				if (freerefinecount < 100)
				{
					if(m_index == 0 || m_index == 1)
					{
						m_index = RefineType_Gold;
						jinglian = "进行精炼吗？";
					}
					else
					{
						m_index = RefineType_SuperGold;
						jinglian = "进行至尊精炼吗？";
					}
				}
				else
				{
					m_index = RefineType_Free;
				}
				int index = m_index;
				canvasdata->inputParam(WCHDATA_JXJRefineEquipType,&index);
				canvasdata->getParam(WCHDATA_JXJRefineNeedGold,param);
				tempneedgold = needgold = *(int *)param;
			}
			int needbagcount = 0;
			//playerGameData->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagcount,NULL));
			if (needbagcount == 0)
			{
				if (m_index != RefineType_Free)
				{
					ifCancelTask = true;
					playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgold,NULL));
					if (needgold == 0)
					{
						data->getParam(WCHDATA_JXJGoldCostHintInt,param);
						unsigned int goldcosthint = *(unsigned int *)param;
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));

						int hinttype = 0;
						if (m_index == RefineType_Gold)
						{
							hinttype = JXJGoldCostType_GoldRefineHint;
							ifhint = goldcosthint & JXJGoldCostType_GoldRefineHint;
						}
						else if (m_index == RefineType_SuperGold)
						{
							hinttype = JXJGoldCostType_SuperGoldRefineHint;
							ifhint = goldcosthint & JXJGoldCostType_SuperGoldRefineHint;
						}

						if (ifhint)
						{
							crData *braindata = crBrain::getInstance()->getDataClass();
							braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
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
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
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
							sprintf(tmpText,"%d礼金/元宝\0",tempneedgold);
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
							cfg_script.Add("Text",jinglian);
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
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));
					}
				}
			}
			else
			{//背包不足
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2008,NULL));
			}
		}
	}
	handle.outputParam(0,&ifCancelTask);
}
/////////////////////////////////////////
//
//crJXJEquipRefineMethod
//
/////////////////////////////////////////
crJXJEquipRefineMethod::crJXJEquipRefineMethod():
	m_this(NULL)
{
}
crJXJEquipRefineMethod::crJXJEquipRefineMethod(const crJXJEquipRefineMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJEquipRefineMethod::inputParam(int i, void *param)
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
void crJXJEquipRefineMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
		m_lock[i] = str;
		break;
	default:
		break;
	}
}
void crJXJEquipRefineMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		bool canrefine = true;
		unsigned char iflock[3] = {0};
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode> refinelock[3] = {NULL};
			for (int i = 0; i < 3; ++i)
			{
				refinelock[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_lock[i]));
				if (refinelock[i].valid())
				{
					if(refinelock[i]->getSelect())
					{
						iflock[i] = 1;
					}
				}
			}
			crData *canvasdata = canvas->getDataClass();
			crData *data = myPlayer->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData*)param;
			canvasdata->getParam(WCHDATA_JXJRefineEquipSelectIndex,param);
			int equipselectindex = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJRefineEquipType,param);
			int refinetype = *(int*)param;
			
			canvasdata->getParam(WCHDATA_JXJRefineEquipSuitIndex,param);
			int suitIndex = *(int*)param;
			//if(refinetype == RefineType_Free)
			//{
			//	data->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
			//	unsigned char freerefinecount = *(unsigned char *)param;
			//	if (freerefinecount <= 0)
			//	{
			//		canrefine = false;
			//		myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2058,NULL));
			//	}
			//}
			if(equipdata && canrefine)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(14);
				stream->_writeInt(equipdata->getItemID());
				stream->_writeInt(equipdata->getEquipMagic());
				stream->_writeShort((short)equipselectindex);
				stream->_writeUChar((unsigned char)refinetype);
				for (int i = 0; i < 3; ++i)
				{
					stream->_writeUChar(iflock[i]);
				}
				stream->_writeChar((char)suitIndex);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvEquipRefine,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvEquipRefineMethod
//
/////////////////////////////////////////
crJXJRecvEquipRefineMethod::crJXJRecvEquipRefineMethod():
	m_netType(GameClient_Game){}
crJXJRecvEquipRefineMethod::crJXJRecvEquipRefineMethod(const crJXJRecvEquipRefineMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJRecvEquipRefineMethod::inputParam(int i, void *param)
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

void crJXJRecvEquipRefineMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvEquipRefineMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{//
				void *param;
				char success = 1;//-1:该装备不能精炼 -2:装备信息有误 -3:精炼信息有误 -4:免费次数不足 -5:元宝不足
				int equipid = m_stream->_readInt();
				int equipmagic = m_stream->_readInt();
				int oequipmagic = equipmagic;
				crTableIO::StrVec refrecord,itemrec;
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				ref_ptr<crTableIO>equipref0tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine0Tab);
				ref_ptr<crTableIO>equipref1tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine1Tab);
				ref_ptr<crTableIO>equipref2tab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine2Tab);
				ref_ptr<crTableIO>equipRefineQualityTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefineQualityTab);
				//ref_ptr<crTableIO>equiplvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquiplvTab);
				//ref_ptr<crTableIO>magicequipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMagicEquipTab);
				//ref_ptr<crTableIO>equiplvidTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquiplvIDTab);
				//ref_ptr<crTableIO>magicidTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJMagicIDTab);
				//crTableIO::StrVec record,recordm;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				crData *data = m_this->getDataClass();
				crData *roleData = m_this->getMainRole()->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
				unsigned short *freerefinecount = (unsigned short *)param;
				std::string infostr;
				unsigned char iflock[3] = {0};

				int crefinestone = 0;
				int chigherrefinestone = 0;
				int crefinelock = 0;
				DeductItemMap deductitemmap;
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

				do 
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equipid),itemrec) < 0)
					{
						success = -1;//装备出错
						break;
					}
					int qualityid = atoi(itemrec[itemtab->getTitleIndex("品质")].c_str());
					crTableIO::DataVec refQualityData;
					equipRefineQualityTab->queryRecords(0,crArgumentParser::appItoa(qualityid),refQualityData);
					if(refQualityData.empty())
					{
						success = -6;//装备品质出错
						break;
					}
					int lockcount = 0;
					short equipindex = m_stream->_readShort();
					unsigned char refinetype = m_stream->_readUChar();
					for (int i = 0; i < 3; ++i)
					{
						iflock[i] = m_stream->_readUChar();
						if (iflock[i] > 0)
						{
							lockcount++;
						}
					}
					if (lockcount > 2)
					{
						lockcount = 2;
					}
					int suitIndex = m_stream->_readChar();
					//int *mycash;
					//unsigned char equiplvid = 0;
					//unsigned char newmagicid = 0;
					//int refqualityid = 0;
					//unsigned char newequiplvid = 0;
					data -> getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					crVector2i price2i;
					int price = 0;
					data->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipPriceVec2i,viplv).c_str(),price2i);
					int magicproindex = -1;
					bool hasequip = false;
					crVector2i lockgold;
					int lockneedgold = 0;
					crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineLockPrice,viplv).c_str(),lockgold);
					if (lockcount > 0)
					{
						if(crefinelock > 0)
						{
							ref_ptr<crBagItemData> deductitem;
							deductitem =new crBagItemData;

							if (crefinelock >= lockcount)
							{
								lockneedgold = 0;
								
								deductitem->setItemID(7535);
								deductitem->setItemDeductCount(lockcount);
								deductitem->setEquipMagic(0);
								deductitemmap.insert(std::make_pair(7535,deductitem));
							}
							else
							{
								lockneedgold = lockgold[lockcount - crefinelock - 1];
								deductitem->setItemID(7535);
								deductitem->setItemDeductCount(crefinelock);
								deductitem->setEquipMagic(0);
								deductitemmap.insert(std::make_pair(7535,deductitem));
							}
						}
						else
							lockneedgold = lockgold[lockcount - 1];
					}
					JXJLordEquipVec::iterator litr;
					if (equipindex < 0)
					{
						roleData->getParam(WCHDATA_JXJLordEquipVecs,param);
						std::vector<JXJLordEquipVec> openedEquipVec = *(std::vector<JXJLordEquipVec> *)param;
						if (suitIndex>=0 && suitIndex<openedEquipVec.size())
						{
							JXJLordEquipVec& lordequipvec = openedEquipVec[suitIndex];
							for (int i= 0; i < lordequipvec.size(); ++i)
							{
								if (lordequipvec[i].first == equipid && lordequipvec[i].second == equipmagic)
								{
									hasequip = true;
									break;
								}
							}
						}
					}
					else
					{
						if (equipindex < itembagvec->size() && (*itembagvec)[equipindex] != NULL)
						{
							if ((*itembagvec)[equipindex]->getItemID() == equipid && 
								(*itembagvec)[equipindex]->getEquipMagic() == equipmagic &&
								(*itembagvec)[equipindex]->getItemCount() > 0)
							{
								hasequip = true;
							}
						}
					}
					if (!hasequip)
					{
						success = -2;//装备信息有误
						break;
					}
					switch (refinetype)
					{
					case RefineType_Free:
						price = 0;
						magicproindex = equipRefineQualityTab->getTitleIndex("免费");
						//equiplvindex = equiplvidTab->getTitleIndex("免费");
						break;
					case RefineType_Gold:
						if(crefinestone > 0)
						{
							price = lockneedgold;

							ref_ptr<crBagItemData> deductitem;
							deductitem =new crBagItemData;
							deductitem->setItemID(7533);
							deductitem->setItemDeductCount(1);
							deductitem->setEquipMagic(0);
							deductitemmap.insert(std::make_pair(7533,deductitem));
						}
						else
							price = price2i[0] + lockneedgold;
						magicproindex = equipRefineQualityTab->getTitleIndex("元宝");
						//equiplvindex = equiplvidTab->getTitleIndex("元宝");
						infostr = "元宝精炼";
						break;
					case RefineType_SuperGold:
						if(chigherrefinestone > 0)
						{
							price = lockneedgold;

							ref_ptr<crBagItemData> deductitem;
							deductitem =new crBagItemData;
							deductitem->setItemID(7534);
							deductitem->setItemDeductCount(1);
							deductitem->setEquipMagic(0);
							deductitemmap.insert(std::make_pair(7534,deductitem));
						}
						else
							price = price2i[1] + lockneedgold;
						magicproindex = equipRefineQualityTab->getTitleIndex("至尊");
						//equiplvindex = equiplvidTab->getTitleIndex("至尊");
						infostr = "至尊精炼";
						break;
					default:
						break;
					}
					if (magicproindex < 0)
					{
						success = -3;//精炼信息有误
						break;
					}
					if (refinetype == RefineType_Free)
					{
						if(*freerefinecount < 100)
						{
							success = -4;//免费次数不足 
							break;
						}
						else
						{
							//扣除免费次数
							*freerefinecount -= 100;
						}
					}
					else
					{
						bool needdeductgold = false;
						MoneyChangeData moneydata(price,infostr);
						m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
						if (moneydata.first != 0)
						{
							success = -5;//元宝不足 
							break;
						}
					}
					//精炼条件满足，开始精炼
					ref_ptr<crTableIO>refcreatetab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefineCreateTab);
					crTableIO::StrVec refcreaterecord;
					crVector3ub refid;
					crVector3ub oldrefid;
					crVector2i vec2;
					int refqualityid = 0;
					int weight = 0;
					std::vector<int>WeightVec;
					weight = 0;
					refid[0] = oldrefid[0] = LOINT16(LOINT32(equipmagic));
					refid[1] = oldrefid[1] = HIINT16(LOINT32(equipmagic));
					refid[2] = oldrefid[2] = LOINT16(HIINT32(equipmagic));
					for( crTableIO::DataVec::iterator itr = refQualityData.begin();
						itr != refQualityData.end();
						++itr )
					{
						weight += atoi((*itr)[magicproindex].c_str());
						WeightVec.push_back(weight);
					} 
					rangei weightrnd(1,weight);
					int r,i;
					int weightcount = WeightVec.size();
					for( int ref = 0; ref<3; ++ref )
					{
						if (iflock[ref] != 1)
						{
							//随机精炼品质
							refqualityid = 0;
							r = weightrnd.get_random();
							for( i = 0; i<weightcount; i++ )
							{
								if(r<=WeightVec[i])
								{
									refqualityid = atoi(refQualityData[i][1].c_str());
									break;
								}
							}
							//随机ID
							vec2.set(0,256);
							if(refcreatetab->queryOneRecord(0,crArgumentParser::appItoa(refqualityid),refcreaterecord) >= 0)
							{
								crArgumentParser::appAtoVec(refcreaterecord[1],vec2);

							}
							if(refqualityid == 0)
							{
								vec2[0] = 0;
							}
							refid[ref] = getRefineID(ref,vec2,refid);//rangei(vec2[0],vec2[1]).get_random();
						}
					}
					//equiplvid = HIINT16(LOINT32(equipmagic));
					//newequiplvid = equiplvid + 1;
					//if(equiplvidTab->queryOneRecord(0,crArgumentParser::appItoa(newequiplvid),recordlv))
					//{
					//	int rr = rangei(0,10000).get_random();
					//	if (rr > atoi(recordlv[equiplvindex].c_str()))
					//	{
					//		newequiplvid -= 1;
					//	}
					//}
					//精炼成功
					_crInt16 hi16 = HIINT32(equipmagic);
					equipmagic = MAKEINT32(MAKEINT16(refid[0],refid[1]),MAKEINT16(refid[2],HIINT16(hi16)));

					data->inputParam(WCHDATA_JXJRefineMagicID,&equipmagic);
					stream->createBuf(7);
					stream->_writeChar(success);
					stream->_writeUShort(*freerefinecount);
					stream->_writeInt(equipmagic);
				} while (0);
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if(success<0)
				{
					stream->createBuf(3);
					stream->_writeChar(success);
					stream->_writeUShort(*freerefinecount);
				}
				
				if(success>0)
				{
					//扣除物品
					m_this->doEvent(WCH_JXJRecvDeductItems,MAKEINT64(&deductitemmap,0));
					//增加活跃度/////////////////////////////////
					m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_RefineEquip, 1));
					///////////////////////////////////////////////
					///游戏日志
					std::string logdata = "精炼"+crArgumentParser::appVectoa(crVector3i(equipid,oequipmagic,equipmagic),'|');
					GameLogData gamelog(Log_EquipRefine,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvEquipRefine,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crData *data = m_this->getDataClass();
			char success = m_stream->_readChar();
			unsigned short freerefinecount = m_stream->_readUShort();
			data->inputParam(WCHDATA_JXJRefineEquipFreeCount,&freerefinecount);
			if (success == 1)
			{
				int newequipmagic = m_stream->_readInt();

				ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
				if (gongfangcanvas.valid())
				{
					crData *canvasdata = gongfangcanvas->getDataClass();
					if (canvasdata)
					{
						canvasdata->inputParam(WCHDATA_JXJAfterRefineEquipMagic,&newequipmagic);
					}
				}
			}
			else
			{
				int index = 0;
				switch (success)
				{//-1:该装备不能精炼 -2:装备信息有误 -3:精炼信息有误 -4:免费次数不足 -5:元宝不足 -6//装备品质出错
				case 0:
					index = 2029;//操作失败
					break;
				case -1:
					index = 2056;
					break;
				case -2:
					index = 2052;
					break;
				case -3:
					index = 2057;
					break;
				case -4:
					index = 2058;
					break;
				case -5:
					index = 2059;
					break;
				case -6:
					index = 2066;
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}
unsigned char crJXJRecvEquipRefineMethod::getRefineID(int refineindex,CRCore::crVector2i vec2,CRCore::crVector3ub refine3ub)
{
	crTableIO::StrVec record[3];
	ref_ptr<crTableIO>equipreftab[3];
	equipreftab[0] = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine0Tab);
	equipreftab[1] = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine1Tab);
	equipreftab[2] = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipRefine2Tab);
	int shuxingid = 0;
	unsigned char refineid = 0;
	bool idvalid = true;
	std::vector<int> shuxingidvec;
	int i = 0, j = 0;
	std::set<unsigned char> IDSet;
	int trycount = 10;
	do 
	{
		trycount--;
		shuxingidvec.clear();
		for (i = 0; i < 3; ++i)
		{
			record[i].clear();
		}
		idvalid = true;
		refineid = rangei(vec2[0],vec2[1]).get_random();			
		if(IDSet.find(refineid)!=IDSet.end())
		{
			continue;
		}
		IDSet.insert(refineid);
		
		if (equipreftab[refineindex]->queryOneRecord(0,crArgumentParser::appItoa(refineid),record[refineindex]) >= 0)
		{
			int columncount = equipreftab[refineindex]->getColumnCount();
			for (i = 1;i < columncount; ++i)
			{
				if (!record[refineindex][i].empty())
				{
					shuxingidvec.push_back(i);
				}
			}
		}
		for (i = 0; i < 3; ++i)
		{
			if(refine3ub[i] > 0 && i != refineindex)
			{
				if (equipreftab[i]->queryOneRecord(0,crArgumentParser::appItoa(refine3ub[i]),record[i]) >= 0)
				{
					for (j = 0; j < shuxingidvec.size();++j)
					{
						if (!record[i][shuxingidvec[j]].empty())
						{
							idvalid = false;
						}
					}
				}
			}
		}
		if (idvalid)
		{
			break;
		}
		else
		{
			refineid = 0;
		}
	} while (trycount);
	return refineid;
}

/////////////////////////////////////////
//
//crJXJSaveE quipRefineMethod
//
/////////////////////////////////////////
crJXJSaveEquipRefineMethod::crJXJSaveEquipRefineMethod():
	m_this(NULL)
{
}
crJXJSaveEquipRefineMethod::crJXJSaveEquipRefineMethod(const crJXJSaveEquipRefineMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSaveEquipRefineMethod::inputParam(int i, void *param)
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
void crJXJSaveEquipRefineMethod::addParam(int i, const std::string& str)
{
}
void crJXJSaveEquipRefineMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData*)param;
			canvasdata->getParam(WCHDATA_JXJRefineEquipSelectIndex,param);
			int equipselectindex = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJRefineEquipSuitIndex,param);
			int suitIndex = *(int*)param;
			canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
			int newmagic = *(int*)param;
			if(equipdata)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(14);
				stream->_writeInt(equipdata->getItemID());
				stream->_writeInt(equipdata->getEquipMagic());
				stream->_writeShort((short)equipselectindex);
				stream->_writeChar(suitIndex);
				stream->_writeInt(newmagic);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSaveEquipRefine,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvSaveEquipRefineMethod
//
/////////////////////////////////////////
crJXJRecvSaveEquipRefineMethod::crJXJRecvSaveEquipRefineMethod():
	m_netType(GameClient_Game){}
crJXJRecvSaveEquipRefineMethod::crJXJRecvSaveEquipRefineMethod(const crJXJRecvSaveEquipRefineMethod& handle):
	crMethod(handle),
	m_gongfangcanvas(handle.m_gongfangcanvas)
{
}
void crJXJRecvSaveEquipRefineMethod::inputParam(int i, void *param)
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

void crJXJRecvSaveEquipRefineMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gongfangcanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvSaveEquipRefineMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{//
				char success = 1;//-1:装备信息有误 
				int equipid = m_stream->_readInt();
				int equipmagic = m_stream->_readInt();
				short equipindex = m_stream->_readShort();
				char suitIndex = m_stream->_readChar();
				int clientmagic = m_stream -> _readInt();

				ref_ptr<crTableIO>equipInlayTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJEquipinlayTab);
				crTableIO::StrVec record;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				crData *data = m_this->getDataClass();
				crData *roleData = m_this->getMainRole()->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				void *param;
				data->getParam(WCHDATA_JXJRefineMagicID,param);
				_crInt32 *newmagic = ( _crInt32 *)param;
				do 
				{
					if (*newmagic != clientmagic)
					{//操作失败
						success = 0;
						break;
					}
					//int *mycash;
					unsigned char newmagicid = 0;
					unsigned char partid = 0;
					data->getParam(WCHDATA_JXJItemBagVec,param);
					JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
					bool hasequip = false;
					bool hasmate = false;
					JXJLordEquipVec::iterator litr;
					if (equipindex < 0)
					{
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJLordEquipVecs,param);
						std::vector<JXJLordEquipVec>* openedEquipVec = (std::vector<JXJLordEquipVec> *)param;
						if (suitIndex >=0 && suitIndex < openedEquipVec->size())
						{
							JXJLordEquipVec &lordequipvec = (*openedEquipVec)[suitIndex];
							for (litr = lordequipvec.begin();
								litr != lordequipvec.end(); 
								++litr,++partid)
							{
								if (litr->first == equipid && litr->second == equipmagic)
								{
									hasequip = true;
									break;
								}
							}
						}						
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					else
					{
						if (equipindex < itembagvec->size() && (*itembagvec)[equipindex] != NULL)
						{
							if ((*itembagvec)[equipindex]->getItemID() == equipid && (*itembagvec)[equipindex]->getEquipMagic() == equipmagic &&
								(*itembagvec)[equipindex]->getItemCount() > 0)
							{
								hasequip = true;
							}
						}
					}
					if (!hasequip)
					{
						success = -1;//装备信息有误
						break;
					}
					if (equipindex < 0)
					{
						litr->second = *newmagic;
					}
					else
					{
						(*itembagvec)[equipindex]->setEquipMagic(*newmagic);
					}

					stream->createBuf(8);
					stream->_writeChar(success);
					stream->_writeShort(equipindex);
					stream->_writeChar(suitIndex);
					stream->_writeUChar(partid);
					stream->_writeInt(*newmagic);
					//清空临时magicid
					*newmagic = 0;
				} while (0);
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if(success<=0)
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSaveEquipRefine,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				if(success>0)
				{
					///游戏日志
					std::string logdata = crArgumentParser::appVectoa(crVector3i(equipid,equipmagic,clientmagic));
					GameLogData gamelog(Log_SaveEquipRefine,logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			if (success == 1)
			{
				short equipindex = m_stream->_readShort();
				char suitIndex = m_stream->_readChar();
				unsigned char partid = m_stream->_readUChar();
				int equipmagic = m_stream->_readInt();

				crData *data = m_this -> getDataClass();
				crRole *mainRole = m_this->getMainRole();
				crData *roleData = mainRole->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				roleData->getParam(WCHDATA_JXJLordEquipVecs,param);
				std::vector<JXJLordEquipVec>* openedEquipVec = (std::vector<JXJLordEquipVec>*) param;
				if (equipindex >= 0)
				{
					if (equipindex < itembagvec->size() && (*itembagvec)[equipindex]!=NULL)
					{
						(*itembagvec)[equipindex]->setEquipMagic(equipmagic);
					}
				}
				else if (suitIndex >= 0 && suitIndex < openedEquipVec->size())
				{
					JXJLordEquipVec& lordequipvec = (*openedEquipVec)[suitIndex];
					lordequipvec[partid].second = equipmagic;
				}
				ref_ptr<crCanvasNode>gongfangcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gongfangcanvas);
				if (gongfangcanvas.valid())
				{
					crData *gongfangdata = gongfangcanvas->getDataClass();
					if (gongfangdata)
					{
						int seletid = -1;
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,1));
						gongfangdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
						crBagItemData *equipdata = (crBagItemData *)param;
						//equipdata->setItemID(0);
						//equipdata->setItemCount(0);
						equipdata->setEquipMagic(equipmagic);
						gongfangdata->excHandle(MAKEINT64(WCH_LockData,0));
						gongfangdata->inputParam(WCHDATA_JXJAfterRefineEquipMagic,0);
						//gongfangdata->inputParam(WCHDATA_JXJRefineEquipSelectIndex,&seletid);
					}
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				int index = 0;
				switch (success)
				{//-1:装备信息有误
				case 0:
					index = 2029;//操作失败
					break;
				case -1:
					index = 2052;
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRefineFreeCountProduceMethod
//
/////////////////////////////////////////
crJXJRefineFreeCountProduceMethod::crJXJRefineFreeCountProduceMethod():
	m_dt(0),
	m_delta(0.0f)
{
}
crJXJRefineFreeCountProduceMethod::crJXJRefineFreeCountProduceMethod(const crJXJRefineFreeCountProduceMethod& handle):
	crMethod(handle),
	m_dt(0),
	m_delta(0.0f)
{
}
void crJXJRefineFreeCountProduceMethod::inputParam(int i, void *param)
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
void crJXJRefineFreeCountProduceMethod::addParam(int i, const std::string& str)
{
}
void crJXJRefineFreeCountProduceMethod::operator()(crHandle &handle)
{
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(playerData.valid())
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			//m_dt += m_dt_record;
			//m_dt_record = 0.0f;

			void *param;
			unsigned char techlv;
			crVector4i maxcountvec;
			crVector4i producevec;
			int producefactor = 100;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			unsigned short limit = (unsigned short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipFreeCountMax,viplv).c_str()));
			float produce = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipFreeCountProduce,viplv).c_str());
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAddRefineFreeCountTechMax,viplv).c_str(),maxcountvec);
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineFreeCountProduceTech,viplv).c_str(),producevec);
			for (int i = 0; i < 4; ++i)
			{
				data -> getParam(WCHDATA_JXJAddRefineFreeCount1TechLv + i,param);
				techlv = *(unsigned char *)param;
				if (techlv > 0)
				{
					limit = maxcountvec[i];
				}
				data -> getParam(WCHDATA_JXJAddRefineFreeProduce1TechLv + i,param);
				techlv = *(unsigned char *)param;
				if (techlv > 0)
				{
					if (producevec[i] > producefactor)
					{
						producefactor = producevec[i];
					}
				}
			}
			produce = produce * (100 + producefactor) / 100;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
			unsigned short *refinefreecount = (unsigned short *)param;
			limit *= 100;
			if(*refinefreecount < limit)
			{
				float dt = (float)m_dt / (float)c_24hours;
				m_delta += produce * dt * 100.0f;
				int output = (int)m_delta;
				if(output>0)
				{
					int last = *refinefreecount/100;
					m_delta -= output;
					*refinefreecount += output;
					if(*refinefreecount > limit) *refinefreecount = limit;
					int newcount = *refinefreecount/100;
					if(newcount>last)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeUShort(*refinefreecount);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvRefineFreeCountProduce,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
		//else
		//{
		//	m_dt_record += m_dt;
		//}
	}
}
/////////////////////////////////////////
//
//crJXJRecvRefineFreeCountProduceMethod
//
/////////////////////////////////////////
crJXJRecvRefineFreeCountProduceMethod::crJXJRecvRefineFreeCountProduceMethod():
	m_netType(GameClient_Game){}
crJXJRecvRefineFreeCountProduceMethod::crJXJRecvRefineFreeCountProduceMethod(const crJXJRecvRefineFreeCountProduceMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvRefineFreeCountProduceMethod::inputParam(int i, void *param)
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

void crJXJRecvRefineFreeCountProduceMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvRefineFreeCountProduceMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			unsigned short refinefreecount = m_stream->_readUShort();
			if (data)
			{
				data->getParam(WCHDATA_JXJRefineEquipFreeCount, param);
				int extra = (int)(refinefreecount/100) - (int)((*(unsigned short *)param)/100);
				if (extra != 0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_FreeRefine, extra));
				data->inputParam(WCHDATA_JXJRefineEquipFreeCount, &refinefreecount);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRefineStoveUpdateMethod
//
/////////////////////////////////////////
crJXJRefineStoveUpdateMethod::crJXJRefineStoveUpdateMethod() :
	m_zbjlIcon(-1)
{}
crJXJRefineStoveUpdateMethod::crJXJRefineStoveUpdateMethod(const crJXJRefineStoveUpdateMethod& handle):
	crMethod(handle),
	m_zbjlIcon(handle.m_zbjlIcon),
	m_equipicon(handle.m_equipicon),
	m_freecountinput(handle.m_freecountinput),
	m_upswitch(handle.m_upswitch)
 {
	for (int i = 0; i < 4; i++)
	{
		m_btn[i] = handle.m_btn[i];
		if (i < 2)
		{
			m_dataniput[i] = handle.m_dataniput[i];
			m_equiplv[i] = handle.m_equiplv[i];
		}
	}
}
void crJXJRefineStoveUpdateMethod::inputParam(int i, void *param)
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
void crJXJRefineStoveUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_equipicon = str;
		break;
	case 1:
		m_equiplv[0] = str;
		break;
	case 2:
		m_equiplv[1] = str;
		break;
	case 3:
		m_freecountinput = str;
		break;
	case 4:
	case 5:
		m_dataniput[i - 4] = str;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		m_btn[i - 6] = str;
		break;
	case 10:
		m_upswitch = str;
		break;
	case 11:
		m_tiaomuStr[0] = str;
		break;
	case 12:
		m_tiaomuStr[1] = str;
		break;
	case 13:
		m_tiaomuStr[2] = str;
		break;
	case 14:
		m_lockSwitchStr[0] = str;
		break;
	case 15:
		m_lockSwitchStr[1] = str;
		break;
	case 16:
		m_lockSwitchStr[2] = str;
		break;
	case 17:
		m_tiaomuNewStr[0] = str;
		break;
	case 18:
		m_tiaomuNewStr[1] = str;
		break;
	case 19:
		m_tiaomuNewStr[2] = str;
		break;
	case 20:
		m_changegoldsw = str;
		break;
	case 21:
		m_goldinput = str;
		break;
	case 22:
		m_refinebtnsw = str;
		break;
	case 23:
		m_refinebtn = str;
		break;
	case 24:
		m_higherrefinebtn = str;
		break;
	case 25:
		m_refinelockcount[0] = str;
		break;
	case 26:
		m_refinelockcount[1] = str;
		break;
	case 27:
		m_refinelockcount[2] = str;
		break;
	case 28:
		m_goldrefinebtn = str;
		break;
	}
}
void crJXJRefineStoveUpdateMethod::operator()(crHandle &handle)
{
	//ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this)
	{
		ref_ptr<crHypertextWidgetNode> tiaomu[3];
		ref_ptr<crHypertextWidgetNode> tiaomuNew[3];
		ref_ptr<crCheckBoxWidgetNode> lockSwitch[3];
		ref_ptr<crStaticTextWidgetNode> lockstonecount[3];
		for (int i = 0; i < 3; i++)
		{
			tiaomu[i] = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_tiaomuStr[i]));
			tiaomuNew[i] = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_tiaomuNewStr[i]));
			lockSwitch[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_lockSwitchStr[i]));
			lockstonecount[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_refinelockcount[i]));
		}
		int rowIndex = 0;
		for (int i = 0; i < 3; i++)
		{
			if (tiaomu[i].valid())
				tiaomu[i]->setVisiable(false);
			if (lockSwitch[i].valid())
				lockSwitch[i]->setVisiable(false);
			if (tiaomuNew[i].valid())
				tiaomuNew[i]->setVisiable(false);
			if (lockstonecount[i].valid())
				lockstonecount[i]->setVisiable(false);
		}

		bool showTiaomu = false;

		crData *data = playerData->getDataClass();
		bool ifshow = false;
		void *param;
		int typeitem = 0;
//		int curpage = 1;
		unsigned char techlv = 0;
		crData *canvasdata = m_this->getDataClass();
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJTheWayToEnterWorkshorp,param);
			char enterway = *(char *)param;

			int crefinestone = 0;
			int chigherrefinestone = 0;
			int crefinelock = 0;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int maxfree = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipFreeCountMax,viplv).c_str()));
			crVector4i maxcountvec;
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAddRefineFreeCountTechMax,viplv).c_str(),maxcountvec);
			for (int i = 0; i < 4; ++i)
			{
				data -> getParam(WCHDATA_JXJAddRefineFreeCount1TechLv + i,param);
				techlv = *(unsigned char *)param;
				if (techlv > 0)
				{
					maxfree = maxcountvec[i];
				}
			}
//			canvasdata -> getParam(WCHDATA_JXJAttachMateCurPage,param);
//			curpage = *(int *)param;
			//	canvasdata->getParam(WCHDATA_JXJTempItemBagVec,param);
			data -> getParam(WCHDATA_JXJRefineEquipFreeCount,param);
			int freecount = *(unsigned short *)param;
			freecount = (int)(freecount/100);
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
			ref_ptr<crImageBoxWidgetNode> equipicon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_equipicon));
			ref_ptr<crStaticTextWidgetNode> freecountinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_freecountinput));
			ref_ptr<crMultiSwitch> upswitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_upswitch));
			ref_ptr<crMultiSwitch> changegoldsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_changegoldsw));
			ref_ptr<crStaticTextWidgetNode> goldinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_goldinput));
			ref_ptr<crHypertextWidgetNode> dataniput[2] = {NULL};
			ref_ptr<crButtonWidgetNode> btn[4] = {NULL};
			ref_ptr<crMultiSwitch> refinebtnsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_refinebtnsw));
			ref_ptr<crButtonWidgetNode> higherrefinebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_higherrefinebtn));
			ref_ptr<crButtonWidgetNode> refinebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_refinebtn));
			ref_ptr<crButtonWidgetNode> goldrefinebtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goldrefinebtn));
			if (higherrefinebtn.valid())
				higherrefinebtn->setVisiable(false);
			if (goldrefinebtn.valid())
				goldrefinebtn->setVisiable(false);
			if (refinebtnsw.valid())
			{
				if(enterway == 2 && chigherrefinestone > 0 && freecount <= 0)
				{
					refinebtnsw->setActiveSwitchSet(1);
					if (higherrefinebtn.valid())
						higherrefinebtn->setEnable(true);
					if (refinebtn.valid())
						refinebtn->setEnable(false);
					if (goldrefinebtn.valid())
						goldrefinebtn->setEnable(false);
				}
				else
				{
					if (freecount > 0)
					{
						refinebtnsw->setActiveSwitchSet(0);
						if (higherrefinebtn.valid())
							higherrefinebtn->setEnable(false);
						if (goldrefinebtn.valid())
							goldrefinebtn->setEnable(false);
						if (refinebtn.valid())
							refinebtn->setEnable(true);
					}
					else
					{
						refinebtnsw->setActiveSwitchSet(2);
						if (higherrefinebtn.valid())
							higherrefinebtn->setEnable(false);
						if (goldrefinebtn.valid())
							goldrefinebtn->setEnable(true);
						if (refinebtn.valid())
							refinebtn->setEnable(false);
					}
				}
			}
			//ref_ptr<crStaticTextWidgetNode> equiplv[2] = {NULL};
			for (int i = 0; i < 4;i ++)
			{
				btn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btn[i]));
				if (btn[i].valid())btn[i]->setVisiable(false);
				if (i < 2)
				{
					//equiplv[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equiplv[i]));
					//if(equiplv[i])equiplv[i]->clearString();
					dataniput[i] = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_dataniput[i]));
					if(dataniput[i].valid())
					{
						dataniput[i]->clearText();
					}
				}
			}
			if(upswitch.valid())upswitch->setActiveSwitchSet(0);
			if(equipicon.valid())equipicon->setVisiable(false);
			if(freecountinput.valid())freecountinput->clearString();
			
			if(freecountinput.valid())freecountinput->setString(crArgumentParser::appItoa(freecount) + "/" +crArgumentParser::appItoa(maxfree));
			if(changegoldsw.valid())
			{				
				//data->getParam(WCHDATA_JXJRefineEquipFreeCount,param);
				//unsigned short freerefinecount = *(unsigned short *)param;
				if (freecount > 0)
				{
					changegoldsw->setActiveSwitchSet(0);
				}
				else
				{
					changegoldsw->setActiveSwitchSet(1);
				}
			}

			if (freecount == 0)
			{
				showTiaomu = true;
			}

			crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
			crTableIO::StrVec itemrecord,magicidrecord,magicrecord,magicidrecord2,magicrecord2,colorRec;
			int iconid = itemtab->getTitleIndex("icon");
			//右边的炉子
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
			crBagItemData *equipdata = (crBagItemData *)param;
			canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
			int aftermagic = *(int *)param;
			bool ifequip = false;
			bool ifmate = false;
			int addprice =  0;
			int rowLockCount = 0;
			crVector4 color = COLOR_WHITE;
			if (equipdata)
			{
				crVector2i item,item2;
				ItemMap itemMap,itemMap2;
				rcfg::ConfigScript outscript(&itemMap);
				rcfg::ConfigScript outscript2(&itemMap2);
				int equipid = equipdata->getItemID();
				if (m_zbjlIcon != equipid)
				{
					for (int i = 0; i < 3; i++)
					{
						if (lockSwitch[i].valid())
						{
							lockSwitch[i]->select(false);
						}
					}
					m_zbjlIcon = equipid;
				}

				int equipcount = equipdata->getItemCount();
				int equipmagicid = equipdata->getEquipMagic();
				if (equipid > 0 && equipcount > 0 &&
					itemtab->queryOneRecord(0,crArgumentParser::appItoa(equipid),itemrecord) >= 0)
				{//炉子中的装备
					if (equipicon.valid())
					{
						equipicon->setVisiable(true);
						equipicon->setImageName(itemrecord[iconid]);
					}
					for (int i = 0; i < 3; i++)
					{
						if (btn[i].valid())btn[i]->setVisiable(true);
					}
					item = crVector2i(equipid,equipmagicid);
					playerData->doEvent(WCH_JXJCreateItemRefineTips,MAKEINT64(&item,&outscript));
					if (dataniput[0].valid())
					{
						dataniput[0]->setHypertext(outscript);
					}
					if (aftermagic != 0 )
					{
						//int aftermagicid = LOINT16(LOINT32(aftermagic));
						//int afterequiplv = HIINT16(LOINT32(aftermagic));
						//equiplv[1]->setString(crArgumentParser::appItoa(afterequiplv));
						item2 = crVector2i(equipid,aftermagic);
						playerData->doEvent(WCH_JXJCreateItemRefineTips,MAKEINT64(&item2,&outscript2));
						if (dataniput[1].valid())
						{
							dataniput[1]->setHypertext(outscript2);
						}
						if (btn[3].valid())btn[3]->setVisiable(true);
					}
					if (higherrefinebtn.valid())
						higherrefinebtn->setVisiable(true);
					if (goldrefinebtn.valid())
						goldrefinebtn->setVisiable(true);
				}
				// 精炼属性
				{
					crVector3ub oldid,newid;
					_crInt16 lo16 = LOINT32(equipmagicid);
					_crInt16 hi16 = HIINT32(equipmagicid);
					oldid[0] = LOINT16(lo16);
					oldid[1] = HIINT16(lo16);
					oldid[2] = LOINT16(hi16);


					_crInt16 after_lo16 = LOINT32(aftermagic);
					_crInt16 after_hi16 = HIINT32(aftermagic);
					newid[0] = LOINT16(after_lo16);
					newid[1] = HIINT16(after_lo16);
					newid[2] = LOINT16(after_hi16);

					crVector2i lockneedgold2i;
					crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineLockPrice,viplv).c_str(),lockneedgold2i);
					crVector2i lockCountneedvip2i;
					crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineLockNeedViplv,viplv).c_str(),lockCountneedvip2i);
					int lockCount = 0;
					if (showTiaomu)
					{
						if (viplv >= lockCountneedvip2i[0])
							lockCount = 1;
						if (viplv >= lockCountneedvip2i[1])
							lockCount = 2;	
					}
					bool rowLocks[3] = {false, false, false};
					
					int rowIndex = 0;

					for (;rowIndex < 3; ++rowIndex)
					{
						if (tiaomuNew[rowIndex].valid() && newid[rowIndex] != 0)
						{
							ItemMap itemMap;
							rcfg::ConfigScript outscript(&itemMap);
							_crInt32 equipmagic = 0;
							switch (rowIndex)
							{
							case 0:
								equipmagic = MAKEINT32(MAKEINT16(newid[rowIndex], 0), 0);
								break;
							case 1:
								equipmagic = MAKEINT32(MAKEINT16(0, newid[rowIndex]), 0);
								break;
							case 2:
								equipmagic = MAKEINT32(MAKEINT16(0, 0), newid[rowIndex]);
								break;
							}
							item = crVector2i(equipid, equipmagic);
							playerData->doEvent(WCH_JXJCreateItemRefineTips, MAKEINT64(&item, &outscript));

							tiaomuNew[rowIndex]->setHypertext(outscript);
							tiaomuNew[rowIndex]->setVisiable(true);
						}
						if (oldid[rowIndex] != 0)
						{	
							if (tiaomu[rowIndex].valid())
							{
								ItemMap itemMap;
								rcfg::ConfigScript outscript(&itemMap);
								_crInt32 equipmagic = 0;
								switch (rowIndex)
								{
								case 0:
									equipmagic = MAKEINT32(MAKEINT16(oldid[rowIndex], 0), 0);
									break;
								case 1:
									equipmagic = MAKEINT32(MAKEINT16(0, oldid[rowIndex]), 0);
									break;
								case 2:
									equipmagic = MAKEINT32(MAKEINT16(0, 0), oldid[rowIndex]);
									break;
								}
								item = crVector2i(equipid, equipmagic);
								playerData->doEvent(WCH_JXJCreateItemRefineTips, MAKEINT64(&item, &outscript));

								tiaomu[rowIndex]->setHypertext(outscript);
								tiaomu[rowIndex]->setVisiable(true);
							}
							if (lockSwitch[rowIndex].valid() && lockCount > 0)
							{
								if(lockstonecount[rowIndex].valid())
								{
									lockstonecount[rowIndex]->setVisiable(true);
									lockstonecount[rowIndex]->setColor(crVector4(0.0,255.0,0.0,255.0));
									lockstonecount[rowIndex]->setString(crArgumentParser::appItoa(crefinelock));
								}

								lockSwitch[rowIndex]->setVisiable(true);
								if (lockSwitch[rowIndex]->getSelect() == true)
								{
									rowLocks[rowIndex] = true;
									++rowLockCount;
								}
							}
						}
					}
					if (rowLockCount >= lockCount)
					{
						for (int i = 0; i < 3; i++)
						{
							if (rowLocks[i] == false)
							{
								if(lockstonecount[i].valid())
									lockstonecount[i]->setVisiable(false);

								lockSwitch[i]->setVisiable(false);
								lockSwitch[i]->select(false);
							}
						}
					}
					//计算锁需要的元宝数
					switch (rowLockCount)
					{
					case 1:
						if(crefinelock >= rowLockCount)
							addprice = 0;
						else
							addprice = lockneedgold2i[0];
						break;
					case 2:
						if (crefinelock > 0)
						{
							if(crefinelock >= rowLockCount)
								addprice = 0;
							else
								addprice = lockneedgold2i[0];
						}
						else
							addprice = lockneedgold2i[1];
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (m_zbjlIcon != -1)
				{
					for (int i = 0; i < 3; i++)
					{
						if (lockSwitch[i].valid())
						{
							lockSwitch[i]->select(false);
						}
					}
					m_zbjlIcon = -1;
				}
			}
			if(goldinput.valid())
			{
				crVector2i price2i;
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRefineEquipPriceVec2i,viplv).c_str(),price2i);
				int totalneedgold = 0;
				if (enterway==2 && chigherrefinestone>0)
				{
					totalneedgold = addprice;
				}
				else if(crefinestone > 0)
				{
					totalneedgold = addprice;
				}
				else
				{
					totalneedgold = price2i[0] + addprice;
				}
				goldinput->setString(crArgumentParser::appItoa(totalneedgold));

				canvasdata->inputParam(WCHDATA_JXJRefineNeedGold,&totalneedgold);
			}
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJGongFangCloseCheckMethod
//
/////////////////////////////////////////
crJXJGongFangCloseCheckMethod::crJXJGongFangCloseCheckMethod()
{
}
crJXJGongFangCloseCheckMethod::crJXJGongFangCloseCheckMethod(const crJXJGongFangCloseCheckMethod& handle):
	crMethod(handle),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_promptCanvas(handle.m_promptCanvas)
{
}
void crJXJGongFangCloseCheckMethod::inputParam(int i, void *param)
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
void crJXJGongFangCloseCheckMethod::addParam(int i, const std::string& str)
{
	switch(i) 
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
void crJXJGongFangCloseCheckMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		crData *mainroledata = playerData->getMainRole()->getDataClass();
		if (m_this && m_this->getDataClass())
		{
			crData *canvasdata = m_this->getDataClass();
			canvasdata->getParam(WCHDATA_JXJAfterRefineEquipMagic,param);
			int afterrefinemagic = *(int *)param;
			if(afterrefinemagic != 0)
			{
				crFilterRenderManager::getInstance()->showCanvas(m_this,true);
				ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
				if (tipscanvas.valid())
				{
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text","您确认关闭工坊吗？\n");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text","之前的精炼将不会被保存！");
					cfg_script.Pop();


					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = tipscanvas->getDataClass();
					int commandtype = CDP_Widget;
					if (dlgData)
					{
						dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					}
					crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
				}
			}
			else
			{
				canvasdata->getParam(WCHDATA_JXJRefineEquipBagitemdata,param);
				crBagItemData *refinebagitem = (crBagItemData *)param;
				if (refinebagitem)
				{
					refinebagitem->setItemID(0);
					refinebagitem->setItemCount(0);
					refinebagitem->setEquipMagic(0);
				}
				canvasdata->getParam(WCHDATA_JXJAttachEquipBagitemdata,param);
				crBagItemData *attachequip = (crBagItemData *)param;
				if (attachequip)
				{
					attachequip->setItemID(0);
					attachequip->setItemCount(0);
					attachequip->setEquipMagic(0);
				}
				canvasdata->getParam(WCHDATA_JXJAttachMateBagitemdata,param);
				crBagItemData *attachmate = (crBagItemData *)param;
				if (attachmate)
				{
					attachmate->setItemID(0);
					attachmate->setItemCount(0);
					attachmate->setEquipMagic(0);
				}
				canvasdata->getParam(WCHDATA_JXJSmelterItemData,param);
				crBagItemData *smeltdata = (crBagItemData *)param;
				if(smeltdata)
				{
					smeltdata->setItemID(0);
					smeltdata->setItemCount(0);
					smeltdata->setEquipMagic(0);
				}
				canvasdata->getParam(WCHDATA_JXJTheWayToEnterWorkshorp,param);
				char *openway = (char *)param;
				*openway = 0;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJOneKeyUpgradeBuildinglvMethod
//
/////////////////////////////////////////
//crJXJOneKeyUpgradeBuildinglvMethod::crJXJOneKeyUpgradeBuildinglvMethod()
//{
//}
//
//crJXJOneKeyUpgradeBuildinglvMethod::crJXJOneKeyUpgradeBuildinglvMethod(const crJXJOneKeyUpgradeBuildinglvMethod& handle):
//	crMethod(handle),
//	m_upgradebtn(handle.m_upgradebtn)
//{
//}
//
//void crJXJOneKeyUpgradeBuildinglvMethod::inputParam(int i, void *param)
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
//
//void crJXJOneKeyUpgradeBuildinglvMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_upgradebtn = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJOneKeyUpgradeBuildinglvMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
//	if(m_this && playerGameData)
//	{
//		crData *braindata = crBrain::getInstance()->getDataClass();
//		unsigned char ifonekey = 1;
//		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
//		if (braindata && canvas)
//		{
//			ref_ptr<crWidgetNode> upgradebtn = dynamic_cast<crWidgetNode *>(canvas->getWidget(m_upgradebtn));
//			if(upgradebtn)
//			{
//				upgradebtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
//				braindata->inputParam(WCHDATA_JXJIfOneKeyUpgrade,&ifonekey);
//				braindata->inputParam(WCHDATA_JXJOneKeyUpgradeButton,upgradebtn);
//			}
//		}
//	}
//}

/////////////////////////////////////////
//
//crJXJOneKeyBuildingLevelUpMethod
//
/////////////////////////////////////////
crJXJOneKeyBuildingLevelUpMethod::crJXJOneKeyBuildingLevelUpMethod()
{
}
crJXJOneKeyBuildingLevelUpMethod::crJXJOneKeyBuildingLevelUpMethod(const crJXJOneKeyBuildingLevelUpMethod& handle):
	crMethod(handle)
{
}
void crJXJOneKeyBuildingLevelUpMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}
void crJXJOneKeyBuildingLevelUpMethod::addParam(int i, const std::string& str)
{
}
void crJXJOneKeyBuildingLevelUpMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	//bool cancelTask = true;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(!myPlayer) return;

	// check level up
	void *param;
	crData *canvasData = canvas->getDataClass();
	canvasData->getParam(WCHDATA_JXJCheckBuildingID,param);
	int buildingid = *(int *)param;
	//
	BuildingLevelUpCheckResult result;
	result.m_code = 0;
	myPlayer->doEvent(WCH_JXJBuildingLevelUpCheck,MAKEINT64(buildingid,&result));
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(result.m_code <= 0 || !netConductor) return;

	// parse and send package
	crData *playerData = myPlayer->getDataClass();
	playerData->getParam(buildingid,param);
	unsigned char buildinglv = *(unsigned char *)(param);
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(5/* + 4*/);
	stream->_writeInt(buildingid);
	stream->_writeUChar(buildinglv);
	//stream->_writeInt(int(m_this));
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvOneKeyBuildingLevelUp,stream.get());
	netConductor->getNetManager()->sendPacket("all",packet);

	//播放特效
	CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
	if(scene && buildingid>0)
	{
		crData *itemdata;
		void *param;
		crInstanceItem *item;
		int itembuildingid;
		CRNetApp::crScene::SceneItemMap &sceneItemMap = scene->getSceneItemMap();
		for( CRNetApp::crScene::SceneItemMap::iterator itr = sceneItemMap.begin();
			itr != sceneItemMap.end();
			++itr )
		{
			item = itr->second.get();
			itemdata = item->getDataClass();
			itemdata->getParam(WCHDATA_JXJBuildingID,param);
			if(param)
			{
				itembuildingid = *(int *)param;
				if(itembuildingid == buildingid)
				{
					item->doEvent(WCH_JXJBuildingLevelChange);
					break;
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvOneKeyBuildingLevelUpMethod
//
/////////////////////////////////////////
crJXJRecvOneKeyBuildingLevelUpMethod::crJXJRecvOneKeyBuildingLevelUpMethod():
	m_netType(GameClient_Game){}
crJXJRecvOneKeyBuildingLevelUpMethod::crJXJRecvOneKeyBuildingLevelUpMethod(const crJXJRecvOneKeyBuildingLevelUpMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvOneKeyBuildingLevelUpMethod::inputParam(int i, void *param)
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
void crJXJRecvOneKeyBuildingLevelUpMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvOneKeyBuildingLevelUpMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int buildingid = m_stream->_readInt();
			unsigned char buildinglv = m_stream->_readUChar();
			void *param;
			crData *data = m_this->getDataClass();
			data->getParam(buildingid,param);
			unsigned char sbuildinglv = *(unsigned char *)param;
			if(sbuildinglv==0)//该建筑未开启
				return;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			BuildingLevelUpCheckResult result;
			result.m_code = 0;
			int *coppercash = NULL;
			int *wood = NULL;
			int *iron = NULL;
			int *food = NULL;
			unsigned char success = 0;
			crVector4i* buildingTimes = NULL;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int bulidingToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBulidingLvUpToVipExpFactor,viplv).c_str());
			if(sbuildinglv == buildinglv)
			{
				while(1)
				{
					m_this->doEvent(WCH_JXJBuildingLevelUpCheck,MAKEINT64(buildingid,&result));
					if (result.m_code <= 0)
					{
						break;
					}
					else
					{
						///////时间
						data->getParam(WCHDATA_JXJBuildingTimes,param);
						buildingTimes = (crVector4i *)param;
						data->getParam(WCHDATA_JXJVipLv,param);
						unsigned char viplv = *(unsigned char *)param;
						short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime,viplv).c_str()));
						int credittime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCreditTime,viplv).c_str()));
						int i = 0;
						int t;
						for(int i = 0; result.m_timeNeed>0 && i<4; i++)
						{
							if((*buildingTimes)[i]>=0 && (*buildingTimes)[i]<overTimes)
							{
								t = overTimes + credittime - (*buildingTimes)[i];
								if(result.m_timeNeed>t)
								{
									(*buildingTimes)[i] += t;
									result.m_timeNeed -= t;
								}
								else
								{
									(*buildingTimes)[i] += result.m_timeNeed;
									result.m_timeNeed = 0;
								}
							}
						}
						if(result.m_timeNeed>0)
						{
							for(int i = 0; i<4; i++)
							{
								if((*buildingTimes)[i]>=0 && (*buildingTimes)[i]<=overTimes)
								{

									(*buildingTimes)[i] += result.m_timeNeed ;						
									break;
								}
							}
						}
						//铜钱
						data->getParam(WCHDATA_JXJCoppercash,param);
						coppercash = (int *)param;
						*coppercash -= result.m_coppercashNeed;
						//木材
						data->getParam(WCHDATA_JXJWood,param);
						wood = (int *)param;
						*wood -= result.m_woodNeed;
						//铁矿
						data->getParam(WCHDATA_JXJIron,param);
						iron = (int *)param;
						*iron -= result.m_ironNeed;
						//粮食
						data->getParam(WCHDATA_JXJFood,param);
						food = (int *)param;
						*food -= result.m_foodNeed;

						sbuildinglv += 1;
						data->inputParam(buildingid,&sbuildinglv);
						//建筑升级增加成长值
						//m_this->doEvent(WCH_JXJAddVipExp,MAKEINT64(bulidingToVipExpFactor,NULL));
						if (!success)
						{
							success = 1;
						}
					}
				}
			}
			if(success)
				m_this->doEvent(WCH_JXJCheck7Day);
			if (buildingid == WCHDATA_JXJFuyalv)
			{//酒肆，市场，兵营，工坊等级等于府衙等级
				data->inputParam(WCHDATA_JXJJiusilv,&sbuildinglv);
				data->inputParam(WCHDATA_JXJShijilv,&sbuildinglv);
				data->inputParam(WCHDATA_JXJBingyinglv,&sbuildinglv);
				data->inputParam(WCHDATA_JXJGongfanglv,&sbuildinglv);
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));


			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(38/* + 4*/);
				if (!success)
				{
					stream->_writeChar(result.m_code);
				}
				else
				{
					stream->_writeChar(success);
				}
				stream->_writeInt(buildingid);
				stream->_writeUChar(sbuildinglv);
				if(success == 1)
				{
					stream->_writeVec4i(*buildingTimes);//16
					stream->_writeInt(*coppercash);//4
					stream->_writeInt(*wood);//4
					stream->_writeInt(*iron);//4
					stream->_writeInt(*food);//4
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvOneKeyBuildingLevelUp,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = "一键升级（id，lv）：" + crArgumentParser::appVectoa(crVector2i(buildingid,sbuildinglv));
				GameLogData gamelog(Log_OneKeyBuildingLevelUp,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
//			void *param;
			//char *ptr = m_stream->getPtr();
			crData *braindata = crBrain::getInstance()->getDataClass();
			char returnCode = m_stream->_readChar();
			int buildingid = m_stream->_readInt();
			unsigned char sbuildinglv = m_stream->_readUChar();
			crData *data = m_this->getDataClass();
			data->inputParam(buildingid,&sbuildinglv);
			if(returnCode>0)
			{
				crVector4i buildingTimes = m_stream->_readVec4i();
				data->inputParam(WCHDATA_JXJBuildingTimes,&buildingTimes);
				int coppercash = m_stream->_readInt();
				data->inputParam(WCHDATA_JXJCoppercash,&coppercash);
				int wood = m_stream->_readInt();
				data->inputParam(WCHDATA_JXJWood,&wood);
				int iron = m_stream->_readInt();
				data->inputParam(WCHDATA_JXJIron,&iron);
				int food = m_stream->_readInt();
				data->inputParam(WCHDATA_JXJFood,&food);
				m_this->doEvent(WCH_JXJBuildingLevelChange,MAKEINT64(buildingid,sbuildinglv));
				if (buildingid == WCHDATA_JXJFuyalv)
				{//酒肆，市场，兵营，工坊等级等于府衙等级
					data->inputParam(WCHDATA_JXJJiusilv,&sbuildinglv);
					data->inputParam(WCHDATA_JXJShijilv,&sbuildinglv);
					data->inputParam(WCHDATA_JXJBingyinglv,&sbuildinglv);
					data->inputParam(WCHDATA_JXJGongfanglv,&sbuildinglv);
					m_this->doEvent(WCH_JXJBuildingLevelChange,MAKEINT64(WCHDATA_JXJJiusilv,sbuildinglv));
					m_this->doEvent(WCH_JXJBuildingLevelChange,MAKEINT64(WCHDATA_JXJShijilv,sbuildinglv));
					m_this->doEvent(WCH_JXJBuildingLevelChange,MAKEINT64(WCHDATA_JXJBingyinglv,sbuildinglv));
					m_this->doEvent(WCH_JXJBuildingLevelChange,MAKEINT64(WCHDATA_JXJGongfanglv,sbuildinglv));
				}
			}
			if (buildingid == WCHDATA_JXJBingyinglv || buildingid == WCHDATA_JXJFuyalv)
			{
				bool ifset = true;
				if (braindata)
				{
					braindata->inputParam(WCHDATA_JXJIfSetArmyNum,&ifset);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJGoldCostHindCanvasOnshowMethod
//
/////////////////////////////////////////
crJXJGoldCostHindCanvasOnshowMethod::crJXJGoldCostHindCanvasOnshowMethod():
	m_this(NULL)
{
}
crJXJGoldCostHindCanvasOnshowMethod::crJXJGoldCostHindCanvasOnshowMethod(const crJXJGoldCostHindCanvasOnshowMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_checkboxArray(handle.m_checkboxArray)
{
}
void crJXJGoldCostHindCanvasOnshowMethod::inputParam(int i, void *param)
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
void crJXJGoldCostHindCanvasOnshowMethod::addParam(int i, const std::string& str)
{
	m_checkboxArray.push_back(str);
}
void crJXJGoldCostHindCanvasOnshowMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		void *param;
		crData *playerData = myPlayer->getDataClass();
		playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int goldcosthint = *(unsigned int *)param;
		ref_ptr<crCheckBoxWidgetNode> checkbox;
		int i = 0;
		for (std::vector<std::string>::iterator itr = m_checkboxArray.begin();
			itr != m_checkboxArray.end();
			++itr)
		{
			checkbox = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(*itr));
			if (checkbox.valid())
			{
				if (goldcosthint & (1 << i))
				{
					checkbox->select(false);
				}
				else
				{

					checkbox->select(true);
				}
			}
			i++;
		}
	}
}
/////////////////////////////////////////
//
//crJXJGoldCostHindSelectAllMethod
//
/////////////////////////////////////////
crJXJGoldCostHindSelectAllMethod::crJXJGoldCostHindSelectAllMethod():
	m_this(NULL)
{
}
crJXJGoldCostHindSelectAllMethod::crJXJGoldCostHindSelectAllMethod(const crJXJGoldCostHindSelectAllMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_checkboxArray(handle.m_checkboxArray)
{
}
void crJXJGoldCostHindSelectAllMethod::inputParam(int i, void *param)
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
		m_this = (crCheckBoxWidgetNode*)param;
		break;
	}
}
void crJXJGoldCostHindSelectAllMethod::addParam(int i, const std::string& str)
{
	m_checkboxArray.push_back(str);
}
void crJXJGoldCostHindSelectAllMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		bool checkboxselect = m_this->getSelect();
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.get())
		{
			ref_ptr<crCheckBoxWidgetNode>checkbox;
			for(std::vector<std::string>::iterator itr = m_checkboxArray.begin();
				 itr != m_checkboxArray.end();
				 ++itr)
			{
				checkbox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(*itr));
				if (checkbox.valid())
				{
					checkbox->select(checkboxselect);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJChangeGoldCostHindMethod
//
/////////////////////////////////////////
crJXJChangeGoldCostHindMethod::crJXJChangeGoldCostHindMethod():
	m_this(NULL)
{
}
crJXJChangeGoldCostHindMethod::crJXJChangeGoldCostHindMethod(const crJXJChangeGoldCostHindMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_checkboxArray(handle.m_checkboxArray)
{
}
void crJXJChangeGoldCostHindMethod::inputParam(int i, void *param)
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
void crJXJChangeGoldCostHindMethod::addParam(int i, const std::string& str)
{
	m_checkboxArray.push_back(str);
}
void crJXJChangeGoldCostHindMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		void *param;
		crData *playerData = myPlayer->getDataClass();
		playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int *goldcosthint = (unsigned int *)param;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			bool ifhint = true;
			ref_ptr<crCheckBoxWidgetNode> checkbox;
			int i = 0;
			for (std::vector<std::string>::iterator itr = m_checkboxArray.begin();
				itr != m_checkboxArray.end();
				++itr)
			{
				ifhint = true;
				checkbox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(*itr));
				if (checkbox.valid())
				{
					ifhint = !checkbox->getSelect();
				}
				if (ifhint)
				{
					*goldcosthint |= (1 << i);
				}
				else
				{
					*goldcosthint &=~ (1 << i);
				}
				i++;
			}
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeUInt(*goldcosthint);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeGoldCostHind,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvChangeGoldCostHindMethod
//
/////////////////////////////////////////
crJXJRecvChangeGoldCostHindMethod::crJXJRecvChangeGoldCostHindMethod():
	m_netType(GameClient_Game){}
crJXJRecvChangeGoldCostHindMethod::crJXJRecvChangeGoldCostHindMethod(const crJXJRecvChangeGoldCostHindMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvChangeGoldCostHindMethod::inputParam(int i, void *param)
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

void crJXJRecvChangeGoldCostHindMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvChangeGoldCostHindMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			unsigned int cdhintid = m_stream->_readUInt();
			crData *data = m_this->getDataClass();
			if(data)data->inputParam(WCHDATA_JXJGoldCostHintInt,&cdhintid);
			///游戏日志
			GameLogData gamelog(Log_ChangeGoldCostHint,"改变消费提示信息");
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(m_this->getPlayerID(),&gamelog));
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBuyNewBuildingMethod
//
/////////////////////////////////////////
crJXJUIBuyNewBuildingMethod::crJXJUIBuyNewBuildingMethod():
	m_this(NULL)
{
}
crJXJUIBuyNewBuildingMethod::crJXJUIBuyNewBuildingMethod(const crJXJUIBuyNewBuildingMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_tips(handle.m_tips),
	m_tipcontent(handle.m_tipcontent)
{
}
void crJXJUIBuyNewBuildingMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	}
}
void crJXJUIBuyNewBuildingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_tipcontent = str;
		break;
	case 2:
		m_promptCanvas = str;
		break;
	case 3:
		m_information = str;
		break;
	case 4:
		m_okbtn = str;
		break;
	default:
		break;
	}
}
void crJXJUIBuyNewBuildingMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this &&playerGameData)
	{
		void *param;
		unsigned char tipsindex = 0;
		crData *thisData = m_this->getDataClass();
		thisData->getParam(WCHDATA_JXJBuildingID,param);
		int buildingid = *(int *)param;
		crData *data = playerGameData->getDataClass();
		crData *mainroledata = playerGameData->getMainRole()->getDataClass();
		mainroledata->getParam(WCHDATA_Level,param);
		unsigned char lordlv = *(unsigned char *)param;
		data->getParam(buildingid,param);
		unsigned char buildinglv = *(unsigned char *)param;
		data->getParam(buildingid - 1,param);
		unsigned char buildinglvpro = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		int needlordlv = 0;
		std::string needitemname;
		int needitemcount = 0;
		int haveitemcount = 0;
		if (buildinglv == 0 && buildinglvpro > 0)
		{
			ref_ptr<crTableIO>abstractitemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_AbstractItemTab);
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			crTableIO::StrVec record,itemrecord;
			if (abstractitemtab.valid() && itemtab.valid())
			{
				crVector2i needitem;
				int lordlvindex = abstractitemtab->getTitleIndex("lordlv");
				int needitemindex = abstractitemtab->getTitleIndex("needitem");
				int buildingidindex = abstractitemtab->getTitleIndex("buildingid");

				int nameindex = itemtab->getTitleIndex("name");
				if (lordlvindex > 0 && needitemindex > 0 && buildingidindex > 0 && abstractitemtab->queryOneRecord(buildingidindex,crArgumentParser::appItoa(buildingid),record) >= 0)
				{
					needlordlv = atoi(record[lordlvindex].c_str());
					crArgumentParser::appAtoVec(record[needitemindex].c_str(),needitem);
					needitemcount =  needitem[1];
					if (needitem[0] > 0)
					{
						if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(needitem[0]),itemrecord) >= 0)
						{
							needitemname = itemrecord[nameindex];
							if (lordlv >= needlordlv)
							{
									for (JXJItemBagVec::iterator itr = itembagvec->begin();
										itr != itembagvec->end();
										++itr)
									{
										if (itr->get())
										{
											if (itr->get()->getItemID() == needitem[0])
											{
												haveitemcount += itr->get()->getItemCount();
											}
										}

									}
									if (haveitemcount < needitemcount)
									{
										tipsindex= 2;//材料不足
									}
							}
							else
							{
								tipsindex = 1;//领主等级不足
							}
						}
						else
						{
							tipsindex = 4;
						}
					}
					else
					{
						tipsindex= 3;//该建筑将稍后开启
					}
				}
				else
				{
					tipsindex = 4;
				}
			}

			if (tipsindex > 0)
			{
				std::string str1 = "开启需求：\n领主等级" + crArgumentParser::appItoa(needlordlv) + "，\n"  + crArgumentParser::appItoa(needitemcount) + "张"+ needitemname + "，\n";
				std::string str2;
				switch (tipsindex)
				{
				case 1:
					str2 = "领主等级不足，无法开启！\n";
					break;
				case 2:
					str2 = needitemname + "不足（" + crArgumentParser::appItoa(haveitemcount) + "/" + crArgumentParser::appItoa(needitemcount) +")，无法开启！\n" + needitemname + "可在国家商店购买";
					break;
				case 3:
					str1.clear();
					str2 = "该建筑将稍后开启";
					break;
				case 4:
					str1.clear();
					str2 = "操作失败";
					break;
				default:
					break;
				}
				ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tips);
				if (tips.valid())
				{
					ref_ptr<crStaticTextWidgetNode> tipinput = dynamic_cast<crStaticTextWidgetNode *>(tips->getWidget(m_tipcontent));
					if (tipinput.valid())
					{
						tipinput->setString(str1 + str2);
						crFilterRenderManager::getInstance()->showCanvas(tips.get(),true);
					}
				}
			}
			else
			{
				ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
				if (tipscanvas.valid())
				{
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					cfg_script.Add("Content");
					cfg_script.Push("Content",1);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text","你确认要使用");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					char tmpText[32];
					sprintf(tmpText,"%d张%s\0",needitemcount,needitemname.c_str());
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",3);
					v_i.clear();
					v_i.push_back(255.0f);
					v_i.push_back(255.0f);
					v_i.push_back(0.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text","开启建筑吗？");
					cfg_script.Pop();


					cfg_script.Pop();

					if(information.valid())information->setHypertext(cfg_script);
					if(okbtn.valid())okbtn->setEnable(true);
					crData *dlgData = tipscanvas->getDataClass();
					int commandtype = CDP_InstanceItem;
					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
					crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuyNewBuildingMethod
//
/////////////////////////////////////////
crJXJBuyNewBuildingMethod::crJXJBuyNewBuildingMethod():
	m_this(NULL)
{
}
crJXJBuyNewBuildingMethod::crJXJBuyNewBuildingMethod(const crJXJBuyNewBuildingMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
	for (int i = 0; i < 13; i++)
	{
		m_check[i] = handle.m_check[i];
	}
}
void crJXJBuyNewBuildingMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	}
}
void crJXJBuyNewBuildingMethod::addParam(int i, const std::string& str)
{
}
void crJXJBuyNewBuildingMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->getParam(WCHDATA_JXJBuildingID,param);
		int buildingid = *(int *)param;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeInt(buildingid);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuyNewBuilding,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuyNewBuildingMethod
//
/////////////////////////////////////////
crJXJRecvBuyNewBuildingMethod::crJXJRecvBuyNewBuildingMethod():
	m_netType(GameClient_Game){}
crJXJRecvBuyNewBuildingMethod::crJXJRecvBuyNewBuildingMethod(const crJXJRecvBuyNewBuildingMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvBuyNewBuildingMethod::inputParam(int i, void *param)
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

void crJXJRecvBuyNewBuildingMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuyNewBuildingMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				int buildingid = m_stream->_readInt();
				crData *data = m_this->getDataClass();


				crData *mainroledata = m_this->getMainRole()->getDataClass();
				mainroledata->getParam(WCHDATA_Level,param);
				unsigned char lordlv = *(unsigned char *)param;
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(buildingid,param);
				unsigned char buildinglv = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				int needlordlv = 0;
				std::string needitemname;
				int needitemcount = 0;
				int haveitemcount = 0;
				ref_ptr<crTableIO>abstractitemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_AbstractItemTab);
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				crTableIO::StrVec record,itemrecord;
				crVector2i needitem;
				int lordlvindex = abstractitemtab->getTitleIndex("lordlv");
				int needitemindex = abstractitemtab->getTitleIndex("needitem");
				int buildingidindex = abstractitemtab->getTitleIndex("buildingid");
				int nameindex = itemtab->getTitleIndex("name");
				do 
				{
					if (buildinglv != 0)
					{
						success = -1;//建筑已开启
						break;
					}

					if (lordlvindex < 0 || needitemindex < 0 || buildingidindex < 0 || !abstractitemtab->queryOneRecord(buildingidindex,crArgumentParser::appItoa(buildingid),record))
					{
						success = -2;//建筑ID出错
						break;
					}
					needlordlv = atoi(record[lordlvindex].c_str());
					if (lordlv < needlordlv)
					{
						success = -3;//领主等级不足
						break;
					}
					crArgumentParser::appAtoVec(record[needitemindex].c_str(),needitem);
					needitemcount =  needitem[1];
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(needitem[0]),itemrecord) < 0)
					{
						success = -4;//物品出错
						break;
					}
					for (JXJItemBagVec::iterator itr = itembagvec->begin();
						itr != itembagvec->end();
						++itr)
					{
						if (itr->get())
						{
							if (itr->get()->getItemID() == needitem[0])
							{
								haveitemcount += itr->get()->getItemCount();
								if (haveitemcount >= needitemcount)
								{
									break;
								}
							}
						}

					}
					if (haveitemcount < needitemcount)
					{
						success = -5;//材料不足
						break;
					}
					if(success == 0)
					{
						success = 1;
						break;
					}
				} while (0);
				if (success > 0)
				{
					unsigned char itemcount = 0;
					for (JXJItemBagVec::iterator itr = itembagvec->begin();
						itr != itembagvec->end();
						++itr)
					{
						if (itr->get())
						{
							if (itr->get()->getItemID() == needitem[0])
							{
								itemcount = itr->get()->getItemCount();
								if(itemcount >= needitemcount)
								{
									itr->get()->setItemCount(itemcount - needitemcount);
									if (itemcount - needitemcount == 0)
									{
										(*itr) = NULL;
									}
									break;
								}
								else
								{
									(*itr) = NULL;
									needitemcount -= itemcount;
								}
							}
						}
					}
					buildinglv = 1;
					data->inputParam(buildingid,&buildinglv);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success > 0)
				{
					stream->createBuf(10);
					stream->_writeChar(success);
					stream->_writeInt(buildingid);
					stream->_writeInt(needitem[0]);
					stream->_writeUChar((unsigned char)needitem[1]);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyNewBuilding,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,buildingid));
				GameLogData gamelog(Log_BuyNewBuilding,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			if (success > 0)
			{
				int buildinglv = 1;
				crData *data = m_this->getDataClass();
				int buildingid = m_stream->_readInt();
				int needitemid = m_stream->_readInt();
				unsigned char needitemcount = m_stream->_readUChar();
				data -> inputParam(buildingid, &buildinglv);
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJItemBagVec,param);
				JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
				unsigned char itemcount = 0;
				for (JXJItemBagVec::iterator itr = itembagvec->begin();
					itr != itembagvec->end();
					++itr)
				{
					if (itr->get())
					{
						if (itr->get()->getItemID() == needitemid)
						{
							itemcount = itr->get()->getItemCount();
							if(itemcount >= needitemcount)
							{
								itr->get()->setItemCount(itemcount - needitemcount);
								if (itemcount - needitemcount == 0)
								{
									(*itr) = NULL;
								}
								break;
							}
							else
							{
								(*itr) = NULL;
								needitemcount -= itemcount;
							}
						}
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				//播放特效
				CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
				if(scene && buildingid>0)
				{
					crData *itemdata;
					void *param;
					crInstanceItem *item;
					int itembuildingid;
					CRNetApp::crScene::SceneItemMap &sceneItemMap = scene->getSceneItemMap();
					for( CRNetApp::crScene::SceneItemMap::iterator itr = sceneItemMap.begin();
						itr != sceneItemMap.end();
						++itr )
					{
						item = itr->second.get();
						itemdata = item->getDataClass();
						itemdata->getParam(WCHDATA_JXJBuildingID,param);
						if(param)
						{
							itembuildingid = *(int *)param;
							if(itembuildingid == buildingid)
							{
								item->doEvent(WCH_JXJBuildingLevelChange);
								break;
							}
						}
					}
				}
			}
			else
			{
				int index = 0;
				switch(success)
				{
				case -1:
					index = 2067;//建筑已开启
					break;
				case -2:
					index = 2068;//建筑ID出错
					break;
				case -3:
					index = 2024;//领主等级不足
					break;
				case -4:
					index = 2047;//物品ID出错
					break;
				case -5:
					index = 2045;//材料不足
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBuyLordEquipMaterialsMethod
//
/////////////////////////////////////////
crJXJUIBuyLordEquipMaterialsMethod::crJXJUIBuyLordEquipMaterialsMethod():
	m_this(NULL)
{
}
crJXJUIBuyLordEquipMaterialsMethod::crJXJUIBuyLordEquipMaterialsMethod(const crJXJUIBuyLordEquipMaterialsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn),
	m_index(handle.m_index)
{
}
void crJXJUIBuyLordEquipMaterialsMethod::inputParam(int i, void *param)
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
void crJXJUIBuyLordEquipMaterialsMethod::addParam(int i, const std::string& str)
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
void crJXJUIBuyLordEquipMaterialsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this &&playerGameData)
	{
		void *param;
		int itemid = 0;
		int itemcount = 0;
		int needgold = 0;
		int needgoldtemp = 0;
		crData *data = playerGameData->getDataClass();
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJGongFangMaterialVec,param);
				GongFangMaterialVec *matevec = (GongFangMaterialVec *)param;
				canvasdata->inputParam(WCHDATA_JXJGongFangMaterialIndex,&m_index);
				if (m_index < matevec->size())
				{
					itemid = (*matevec)[m_index]->getItemID();
					itemcount = (*matevec)[m_index]->getItemDeductCount();
				}
			}
		}
		ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record;
		int shoppriceindex = itemTab->getTitleIndex("商店价格");
		if (itemid > 0 && itemcount > 0)
		{
			int needbagcount = 1;
			//检测背包
			playerGameData->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagcount,NULL));
			if (needbagcount == 0)
			{
				//检测元宝
				if (itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0 && shoppriceindex > 0)
				{
					needgold = atof(record[shoppriceindex].c_str()) * (float)itemcount;
					if (needgold < 1)
					{
						needgold = 1;
					}
					needgoldtemp = needgold;
					playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgoldtemp,NULL));
					if (needgoldtemp == 0)
					{
					//	data->getParam(WCHDATA_JXJGoldCostHintInt,param);
					//	unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_GongFangBuyMaterialHint;
						ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
						if (tipscanvas.valid())
						{
							ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
							ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
					//		if (ifhint)
							{
								std::vector<float>v_i;
								ItemMap itemMap;
								rcfg::ConfigScript cfg_script(&itemMap);
								cfg_script.Add("Hypertext");
								cfg_script.Push("Hypertext");

								cfg_script.Add("Content");
								cfg_script.Push("Content",1);
								v_i.clear();
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
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
								v_i.push_back(255.0f);
								v_i.push_back(255.0f);
								v_i.push_back(0.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","购买材料吗？");
								cfg_script.Pop();


								cfg_script.Pop();

								if(information.valid())information->setHypertext(cfg_script);
							}
							if(okbtn.valid())okbtn->setEnable(true);
							crData *dlgData = tipscanvas->getDataClass();
							int commandtype = CDP_Widget;
							dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
							dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
						//	if (ifhint)
						//	{
								crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
						//	}
						//	else
						//	{
						//		if(okbtn)okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
						//	}
						}
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2047,NULL));
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2008,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuyLordEquipMaterialsMethod
//
/////////////////////////////////////////
crJXJBuyLordEquipMaterialsMethod::crJXJBuyLordEquipMaterialsMethod():
	m_this(NULL)
{
}
crJXJBuyLordEquipMaterialsMethod::crJXJBuyLordEquipMaterialsMethod(const crJXJBuyLordEquipMaterialsMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
	for (int i = 0; i < 13; i++)
	{
		m_check[i] = handle.m_check[i];
	}
}
void crJXJBuyLordEquipMaterialsMethod::inputParam(int i, void *param)
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
void crJXJBuyLordEquipMaterialsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		break;
	default:
		break;
	}
}
void crJXJBuyLordEquipMaterialsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		int itemid = 0;
		int itemcount = 0;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJGongFangMaterialVec,param);
				GongFangMaterialVec *matevec = (GongFangMaterialVec *)param;
				canvasdata->getParam(WCHDATA_JXJGongFangMaterialIndex,param);
				int index = *(int *)param;
				if (index < matevec->size())
				{
					itemid = (*matevec)[index]->getItemID();
					itemcount = (*matevec)[index]->getItemDeductCount();
				}
			}
		}
		if(itemid > 0 && itemcount > 0)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(8);
			stream->_writeInt(itemid);
			stream->_writeInt(itemcount);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuyLordEquipMaterials,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuyLordEquipMaterialsMethod
//
/////////////////////////////////////////
crJXJRecvBuyLordEquipMaterialsMethod::crJXJRecvBuyLordEquipMaterialsMethod():
	m_netType(GameClient_Game){}
crJXJRecvBuyLordEquipMaterialsMethod::crJXJRecvBuyLordEquipMaterialsMethod(const crJXJRecvBuyLordEquipMaterialsMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvBuyLordEquipMaterialsMethod::inputParam(int i, void *param)
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

void crJXJRecvBuyLordEquipMaterialsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuyLordEquipMaterialsMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				crTableIO::StrVec record;
				int shoppriceindex = itemTab->getTitleIndex("商店价格");
				int maxcountindex = itemTab->getTitleIndex("叠加数量");
				int needgold = 0;
				bool needdeductgold = false;
				int itemid = m_stream->_readInt();
				int itemcount = m_stream->_readInt();
				crData *data = m_this->getDataClass();
				int maxcount = 0;
				do 
				{
					if (itemid <= 0 || itemcount <= 0)
					{
						success = -1;//物品出错
						break;
					}
					if (itemTab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record) >= 0 && shoppriceindex > 0 && maxcountindex > 0)
					{
						maxcount =  atoi(record[maxcountindex].c_str());
					}
					else
					{
						success = -1;//物品出错
						break;
					}
					if (maxcount <= 0)
					{
						maxcount = 1;
					}
					int needbagcount = ceil((float)itemcount / (float)maxcount);
					int needsendcount = needbagcount;
					//检测背包
					m_this->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagcount,NULL));
					if (needbagcount > 0)
					{
						success = -2;//背包不足
						break;
					}
					//检测元宝
					needgold = atof(record[shoppriceindex].c_str()) * (float)itemcount;
					if (needgold < 1)
					{
						needgold = 1;
					}
					MoneyChangeData moneydata(needgold,"工坊买材料," + crArgumentParser::appItoa(itemid) + "，" + crArgumentParser::appItoa(itemcount));
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
					if (moneydata.first != 0)
					{
						success = -3;//元宝不足
					}
					if(success == 0)
					{
						success = 1;
						ref_ptr<crBagItemData> newcard;
						RewardItemVec rewardItems;
						data->excHandle(MAKEINT64(WCH_LockData,1));
						for (int i = 0; i < needsendcount; ++i)
						{
							//rewardItems.clear();
							newcard = new crBagItemData;
							if (itemcount > maxcount)
							{
								newcard->setItemCount(maxcount);
								itemcount -= maxcount;
							}
							else
							{
								newcard->setItemCount(itemcount);
								itemcount = 0;
							}
							newcard->setItemID(itemid);
							rewardItems.push_back(newcard);
							if (itemcount == 0)
							{
								break;
							}
						}
						m_this->doEvent(WCH_JXJRecvRewardItems,MAKEINT64(&rewardItems,0));
						data->excHandle(MAKEINT64(WCH_LockData,0));
						if (needdeductgold)
						{
							crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
							ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
							if(playerData.valid())
							{
								crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
								callback->savePlayerData(playerData.get());
							}
						}
						break;
					}
				} while (0);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(success);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuyLordEquipMaterials,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,itemid));
				GameLogData gamelog(Log_BuyLordEquipMaterials,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			if (success < 0)
			{
				int index = 0;
				switch(success)
				{
				case -1:
					index = 2047;//物品出错
					break;
				case -2:
					index = 2008;//背包不足
					break;
				case -3:
					index = 2036;//元宝不足
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUpgradeVipAffairsMethod
//
/////////////////////////////////////////
crJXJUpgradeVipAffairsMethod::crJXJUpgradeVipAffairsMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJUpgradeVipAffairsMethod::crJXJUpgradeVipAffairsMethod(const crJXJUpgradeVipAffairsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{
}
void crJXJUpgradeVipAffairsMethod::inputParam(int i, void *param)
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
void crJXJUpgradeVipAffairsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUpgradeVipAffairsMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		int tipsindex = 0;
		bool success = true;
		int affairid = 0;
		unsigned char affairlv = 0;
		ref_ptr<crTableIO>vipAffairslvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);
		crTableIO::StrVec record;
		int expIndex = vipAffairslvTab->getTitleIndex("消耗成长值");
		int lvIndex = vipAffairslvTab->getTitleIndex("lv");
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char*)param;
		data->getParam(WCHDATA_JXJVipExp,param);
		int vipExp = *(int *)param;
		if (vipLevel < 1)
		{
			tipsindex = 2069;//需要VIP1
			success = false;
		}
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crData *canvasdata = canvas->getDataClass();
			if (canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJVipAffairsIDVec,param);
				VIPAffairsVec *affairsvec = (VIPAffairsVec *)param;
				if (m_index < affairsvec->size())
				{
					affairid = (*affairsvec)[m_index];
				}
			}
		}
		if (affairid > 0)
		{
			data->getParam(affairid,param);
			affairlv = *(unsigned char *)param;
			if (vipAffairslvTab->queryOneRecord(0,crArgumentParser::appItoa(affairid),lvIndex,crArgumentParser::appItoa(affairlv),record) >= 0)
			{
				if (atoi(record[expIndex].c_str()) > vipExp)
				{
					tipsindex = 2071;//成长值不足
					success = false;
				}
			}
			else
			{
				tipsindex = 2070;//无效的ID
				success = false;
			}
		}
		else
		{
			tipsindex = 2070;//无效的ID
			success = false;
		}
		if (success)
		{
			if (vipAffairslvTab->queryOneRecord(0,crArgumentParser::appItoa(affairid),lvIndex,crArgumentParser::appItoa(affairlv + 1),record) < 0)
			{
				tipsindex = 2072;//等级最高
				success = false;
			}
		}
		if (success)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(affairid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUpgradeVipAffairs,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,tipsindex);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUpgradeVipAffairsMethod
//
/////////////////////////////////////////
crJXJRecvUpgradeVipAffairsMethod::crJXJRecvUpgradeVipAffairsMethod():
	m_netType(GameClient_Game){}
crJXJRecvUpgradeVipAffairsMethod::crJXJRecvUpgradeVipAffairsMethod(const crJXJRecvUpgradeVipAffairsMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvUpgradeVipAffairsMethod::inputParam(int i, void *param)
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

void crJXJRecvUpgradeVipAffairsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUpgradeVipAffairsMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			char success = 0;
			int needvipExp = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				ref_ptr<crTableIO>vipAffairslvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);
				crTableIO::StrVec record;
				int expIndex = vipAffairslvTab->getTitleIndex("消耗成长值");
				int lvIndex = vipAffairslvTab->getTitleIndex("lv");
				int affairid = m_stream->_readInt();
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT16(WCH_LockData,1));
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char*)param;
				data->getParam(WCHDATA_JXJVipExp,param);
				int vipExp = *(int *)param;
				data->getParam(affairid,param);
				unsigned char *affairlv = (unsigned char*)param;
				do 
				{
					if (vipAffairslvTab -> queryOneRecord(0,crArgumentParser::appItoa(affairid),lvIndex,crArgumentParser::appItoa(*affairlv),record) < 0 || affairid < 0 || expIndex < 0)
					{
						success = -1;//无效的ID
						break;
					}
					if (vipLevel < 1)
					{
						success = -2;//VIP等级不足
						break;
					}
					needvipExp = atoi(record[expIndex].c_str());
					if (needvipExp > vipExp)
					{
						success = -3;//成长值不足
						break;
					}
					if (vipAffairslvTab -> queryOneRecord(0,crArgumentParser::appItoa(affairid),lvIndex,crArgumentParser::appItoa(*affairlv + 1),record) < 0)
					{
						success = -4;//已到最高级
						break;
					}
					if(success == 0)
					{
						success = 1;
						m_this->doEvent(WCH_JXJAddVipExp,MAKEINT64(-needvipExp,NULL));
						*affairlv += 1;
						break;
					}
				} while (0);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(6);
					stream->_writeChar(success);
					stream->_writeInt(affairid);
					stream->_writeUChar(*affairlv);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUpgradeVipAffairs,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				data->excHandle(MAKEINT16(WCH_LockData,0));
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,affairid));
				GameLogData gamelog(Log_UpgradeVipAffairs,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			if (success < 0)
			{
				int index = 0;
				switch(success)
				{
				case -1:
					index = 2070;//无效的ID
					break;
				case -2:
					index = 2069;//VIP等级不足
					break;
				case -3:
					index = 2071;//成长值不足
					break;
				case -4:
					index = 2072;//已到最高级
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
			else if (success == 1)
			{
				int affairid = m_stream->_readInt();
				unsigned char affairlv = m_stream->_readUChar();
				crData *data = m_this->getDataClass();
				data->inputParam(affairid,&affairlv);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetProgressBarTypeMethod
//
/////////////////////////////////////////
crJXJSetProgressBarTypeMethod::crJXJSetProgressBarTypeMethod():
	m_this(NULL),
	m_bartype(0)
{
}
crJXJSetProgressBarTypeMethod::crJXJSetProgressBarTypeMethod(const crJXJSetProgressBarTypeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_bartype(handle.m_bartype),
	m_progressbar(handle.m_progressbar)
{
}
void crJXJSetProgressBarTypeMethod::inputParam(int i, void *param)
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
void crJXJSetProgressBarTypeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_progressbar = str;
		break;
	case 1:
		m_bartype = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJSetProgressBarTypeMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		ref_ptr<crProgressWidgetNode> progressbar = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_progressbar));
		if (progressbar.valid())
		{
			if (m_bartype == 0)
			{
				progressbar->setProgressBarType(crProgressWidgetNode::HORIZONTALBAR);
			}
			else
			{
				progressbar->setProgressBarType(crProgressWidgetNode::VERTICALBAR);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJStopBatchUseItemMethod
//
/////////////////////////////////////////
crJXJStopBatchUseItemMethod::crJXJStopBatchUseItemMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJStopBatchUseItemMethod::crJXJStopBatchUseItemMethod(const crJXJStopBatchUseItemMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_index(handle.m_index)
{
}
void crJXJStopBatchUseItemMethod::inputParam(int i, void *param)
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
void crJXJStopBatchUseItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJStopBatchUseItemMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
		}
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (m_index == 0)
		{
			braindata->inputParam(WCHDATA_JXJUseItemRemainCount,0);
		}
		else
		{
			braindata->inputParam(WCHDATA_JXJRollCardRemainCount,0);
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetCheckBoxSelectMethod
//
/////////////////////////////////////////
crJXJSetCheckBoxSelectMethod::crJXJSetCheckBoxSelectMethod():
	m_index(0),
	m_this(NULL)
{
}
crJXJSetCheckBoxSelectMethod::crJXJSetCheckBoxSelectMethod(const crJXJSetCheckBoxSelectMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_canvas(handle.m_canvas),
	m_checkbox(handle.m_checkbox),
	m_this(handle.m_this)
{
}
void crJXJSetCheckBoxSelectMethod::inputParam(int i, void *param)
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
		m_this = (crCheckBoxWidgetNode*)param;
		break;
	}
}
void crJXJSetCheckBoxSelectMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_checkbox = str;
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJSetCheckBoxSelectMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode> checkbox = dynamic_cast<crCheckBoxWidgetNode *>(canvas -> getWidget(m_checkbox));
			if (checkbox.valid())
			{
				checkbox->select(m_index);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckHuoDongBtnMethod
//
/////////////////////////////////////////
crJXJCheckHuoDongBtnMethod::crJXJCheckHuoDongBtnMethod():
	m_this(NULL)
{
}
crJXJCheckHuoDongBtnMethod::crJXJCheckHuoDongBtnMethod(const crJXJCheckHuoDongBtnMethod& handle):
	crMethod(handle),
	m_jinglianbtn(handle.m_jinglianbtn),
	m_shuishoubtn(handle.m_shuishoubtn),
	m_strOnlineBtn(handle.m_strOnlineBtn),
	m_guanzhibtn(handle.m_guanzhibtn),
	m_LoginEveryDay(handle.m_LoginEveryDay),
	m_guoZhan(handle.m_guoZhan),
	m_strLunHui(handle.m_strLunHui),
	m_onlineInput(handle.m_onlineInput),
	m_govsequence(handle.m_govsequence),
	m_onlineDongHua (handle.m_onlineDongHua),
	m_huodongBtn(handle.m_huodongBtn),
	m_smallManager(handle.m_smallManager),
	m_foundCountry(handle.m_foundCountry),
	m_7DaysLogin (handle.m_7DaysLogin),
	m_firstRecharge(handle.m_firstRecharge),
	m_7DaysLoginAnimation(handle.m_7DaysLoginAnimation),
	m_yuekabtn(handle.m_yuekabtn),
	m_zixuanlibaobtn(handle.m_zixuanlibaobtn),
	m_chongzhihuikuibtn(handle.m_chongzhihuikuibtn),
	m_zhengzhanshopbtn(handle.m_zhengzhanshopbtn),
	m_danbichongzhibtn(handle.m_danbichongzhibtn),
	m_xiaofeihuikuibtn(handle.m_xiaofeihuikuibtn),
	m_zhoukabtn(handle.m_zhoukabtn),
	m_danbichongzhitimer(handle.m_danbichongzhitimer),
	m_xiaofeihuikuitimer(handle.m_xiaofeihuikuitimer),
	m_chongzhihuikuitimer(handle.m_chongzhihuikuitimer),
	m_zixuanlibaotimer(handle.m_zixuanlibaotimer),
	m_pvpbtn(handle.m_pvpbtn),
	m_houxuanren(handle.m_houxuanren),
	m_guanyuanxuanju(handle.m_guanyuanxuanju),
	m_toupiaojieguo(handle.m_toupiaojieguo),
	m_panguobtn(handle.m_panguobtn),
	m_YellowNewbieBtn(handle.m_YellowNewbieBtn),
	m_YellowBtn(handle.m_YellowBtn),
	m_meirihaili(handle.m_meirihaili),
	m_meirihailisq(handle.m_meirihailisq),
	m_xinshouBaoDian(handle.m_xinshouBaoDian),
	m_junTuanHuoDong(handle.m_junTuanHuoDong),
	m_junTuanHuoDongSequece(handle.m_junTuanHuoDongSequece)
{
	for (unsigned char i = 0; i < 2; ++ i)
	{
		m_sequence[i] = handle.m_sequence[i];
	}
}
void crJXJCheckHuoDongBtnMethod::inputParam(int i, void *param)
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
void crJXJCheckHuoDongBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_LoginEveryDay = str;
		break;
	case 1:
		m_strOnlineBtn = str;
		break;
	case 2:
		m_jinglianbtn = str;
		break;
	case 3:
		m_guanzhibtn = str;
		break;
	case 4:
		m_shuishoubtn = str;
		break;
	case 5:
		m_guoZhan = str;
		break;
	case 6:
		m_strLunHui = str;
		break;
	case 7:
		m_onlineInput = str;
		break;
	case 8:
	case 9:
		m_sequence[i - 8] = str;
		break;
	case 10:
		m_govsequence = str;
		break;
	case 11:
		m_onlineDongHua = str;
		break;
	case 12:
		m_huodongBtn = str;
		break;
	case 13:
		m_smallManager = str;
		break;
	case 14:
		m_foundCountry = str;
		break;
	case 15:
		m_7DaysLogin = str;
		break;
	case 16:
		m_7DaysLoginAnimation = str;
		break;
	case 17:
		m_firstRecharge = str;
		break;
	case 18:
		m_yuekabtn = str;
	case 19:
		m_zixuanlibaobtn = str;
	case 20:
		m_chongzhihuikuibtn = str;
		break;
	case 21:
		m_zhengzhanshopbtn = str;
		break;
	case 22:
		m_danbichongzhibtn = str;
		break;
	case 23:
		m_xiaofeihuikuibtn = str;
		break;
	case 24:
		m_zhoukabtn = str;
		break;
	case 25:
		m_danbichongzhitimer = str;
		break;
	case 26:
		m_xiaofeihuikuitimer = str;
		break;
	case 27:
		m_chongzhihuikuitimer = str;
		break;
	case 28:
		m_zixuanlibaotimer = str;
		break;
	case 29:
		m_pvpbtn = str;
		break;
	case 30:
		m_houxuanren = str;
		break;
	case 31:
		m_guanyuanxuanju = str;
		break;
	case 32:
		m_toupiaojieguo = str;
		break;
	case 33:
		m_panguobtn = str;
		break;
	case 34:
		m_YellowNewbieBtn = str;
		break;
	case 35:
		m_YellowBtn = str;
		break;
	case 36:
		m_meirihaili = str;
		break;
	case 37:
		m_meirihailisq = str;
		break;
	case 38:
		m_xinshouBaoDian = str;
		break;
	case 39:
		m_junTuanHuoDong = str;
		break;
	case 40:
		m_junTuanHuoDongSequece = str;
		break;
	default:
		break;
	}
}
void crJXJCheckHuoDongBtnMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		time_t curtime = time(0);
		void *param;
		crData *data = myPlayer->getDataClass();
		ref_ptr<crButtonWidgetNode> loginEveryDay = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_LoginEveryDay));
		ref_ptr<crButtonWidgetNode> btOnline = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strOnlineBtn));
		ref_ptr<crButtonWidgetNode> jinglianbtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_jinglianbtn));
		ref_ptr<crButtonWidgetNode> guanzhibtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_guanzhibtn));
		ref_ptr<crButtonWidgetNode> shuishoubtn = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_shuishoubtn));
		ref_ptr<crButtonWidgetNode> guoZhan = dynamic_cast<crButtonWidgetNode *>(m_this -> getWidget(m_guoZhan));
		ref_ptr<crButtonWidgetNode> lunhuibtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strLunHui));
		ref_ptr<crButtonWidgetNode> huodongBtn  = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_huodongBtn));
		ref_ptr<crButtonWidgetNode> smallManagerBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_smallManager));
		ref_ptr<crButtonWidgetNode> foundCountry = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_foundCountry));
		ref_ptr<crButtonWidgetNode> _7DaysIcon = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_7DaysLogin));
		ref_ptr<crButtonWidgetNode> firstRecharge = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_firstRecharge));
		ref_ptr<crButtonWidgetNode> yuekaBtn  = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_yuekabtn));
		ref_ptr<crButtonWidgetNode> zhengzhanshopBtn  = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_zhengzhanshopbtn));
		ref_ptr<crButtonWidgetNode> zixuanlibao = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_zixuanlibaobtn));
		ref_ptr<crButtonWidgetNode> chongzhihuikuiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_chongzhihuikuibtn));
		ref_ptr<crButtonWidgetNode> danbichongzhiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_danbichongzhibtn));
		ref_ptr<crButtonWidgetNode> zhoukaBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_zhoukabtn));
		ref_ptr<crButtonWidgetNode> xiaofeihuikuiBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_xiaofeihuikuibtn));
		ref_ptr<crButtonWidgetNode> pvp = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_pvpbtn));
		ref_ptr<crButtonWidgetNode> houxuanren = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_houxuanren));
		ref_ptr<crButtonWidgetNode> guanyuanxuanju = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_guanyuanxuanju));
		ref_ptr<crButtonWidgetNode> toupiaojieguo = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_toupiaojieguo));
		ref_ptr<crButtonWidgetNode> panguobtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_panguobtn));
		ref_ptr<crStaticTextWidgetNode> onlineInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_onlineInput));
		ref_ptr<crStaticTextWidgetNode> danbichongzhitimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_danbichongzhitimer));
		ref_ptr<crStaticTextWidgetNode> xiaofeihuikuitimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_xiaofeihuikuitimer));
		ref_ptr<crStaticTextWidgetNode> chongzhihuikuitimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_chongzhihuikuitimer));
		ref_ptr<crStaticTextWidgetNode> zixuanlibaotimer = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_zixuanlibaotimer));
		ref_ptr<crButtonWidgetNode> yellowNewbie = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_YellowNewbieBtn));
		ref_ptr<crButtonWidgetNode> yellow = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_YellowBtn));
		ref_ptr<crButtonWidgetNode> meirihaili = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_meirihaili));
		ref_ptr<crButtonWidgetNode> newbieIntroduction = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_xinshouBaoDian));
		ref_ptr<crButtonWidgetNode> junTuanHuodongBtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_junTuanHuoDong));

		m_this->setEnable(false);
		data->getParam(WCHDATA_JXJYELLOWAWARDINFO,param);
		int yellowInfo = *(int*)param;
		data->getParam(WCHDATA_JXJISYELLOW,param);
		unsigned char isYellow = *(unsigned char*)param;
		crRole *pMainRole = myPlayer->getMainRole();
		crData* pRoleData = pMainRole?pMainRole->getDataClass():NULL;
		if(yellowNewbie.valid() && yellow.valid() && pRoleData)
		{
			pRoleData->getParam(WCHDATA_JXJNewHuodongMap,param);
			NewHuodongMap huodongMap = *(NewHuodongMap *)param;
			NewHuodongMap ::iterator itr = huodongMap.find(ACTIVITY_YELLOW_ID) ;
			if (itr == huodongMap.end() || !itr->second->isActivation())
			{
				yellowNewbie->setVisiable(false);
				//yellow->setVisiable(false);
			}
			else
			{
				if ((yellowInfo & YellowAward_Newbie) !=0)
				{
					yellowNewbie->setVisiable(false);
				}
				else
				{
					yellowNewbie->setVisiable(true);
				}
			}
			itr = huodongMap.find(ACTIVITY_YEAR_YELLOW) ;
			if (itr == huodongMap.end() || !itr->second->isActivation())
			{
				yellow->setVisiable(false);
			}
			else
			{
				if(((yellowInfo & 0xff000000) == 0 || (yellowInfo & 0xff0000) == 0) && isYellow == 1)
					yellow->setVisiable(true);
				else
					yellow->setVisiable(false);
			}
		}
		
		if(onlineInput.valid() && btOnline.valid())
		{
			onlineInput->setVisiable(btOnline->getVisiable());
		}
		ref_ptr<crSequence> govsequence = dynamic_cast<crSequence *>(m_this->getChildNode(m_govsequence));
		ref_ptr<crSequence> onlineDongHua = dynamic_cast<crSequence *>(m_this->getChildNode(m_onlineDongHua));
		ref_ptr<crSequence> _7DaysLoginDongHua = dynamic_cast<crSequence *>(m_this->getChildNode(m_7DaysLoginAnimation));
		ref_ptr<crSequence> meirihailisq = dynamic_cast<crSequence *>(m_this->getChildNode(m_meirihailisq));
		ref_ptr<crSequence> junTuanHuodongSequence = dynamic_cast<crSequence *>(m_this->getChildNode(m_junTuanHuoDongSequece));
		ref_ptr<crSequence> sequence[2] = {NULL};
		for (unsigned char i = 0; i < 2; ++ i)
		{
			sequence[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_sequence[i]));
			if (sequence[i].valid())
			{
				sequence[i]->setVisiable(false);
			}
		}
		//计算是否可以升官
		ref_ptr<crTableIO>postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
		crData *mainroledata = myPlayer->getMainRole()->getDataClass();
		mainroledata->getParam(WCHDATA_JXJGovPost,param);
		unsigned char post = *(unsigned char*)param;
		mainroledata->getParam(WCHDATA_JXJExploit,param);
		int govpostgot = *(int *)param;
		int npost = post+1;
		crTableIO::StrVec record1;
		if(postTab->queryOneRecord(0,crArgumentParser::appItoa(npost),record1)>=0)
		{
			int rowcount = postTab->getRowCount();
			if(npost>rowcount) 
			{
				if(govsequence.valid())govsequence->setVisiable(false);
			}
			else
			{
				int govpostneed = atoi(record1[4].c_str());
				if(govpostneed<=govpostgot)
				{
					if(govsequence.valid())govsequence->setVisiable(true);
				}
				else
				{//提示功勋不够
					if(govsequence.valid())govsequence->setVisiable(false);
				}
			}
		}
		else
		{
			if(govsequence.valid())govsequence->setVisiable(false);
		}
		/////
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		roledata->getParam(WCHDATA_Level, param);
		unsigned char characterlevel = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char viplv = *(unsigned char *)param;
		if (loginEveryDay.valid())
		{
			int loginEveryDayopenlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJloginEveryDayopenlv, viplv).c_str());
			data->getParam(WCHDATA_JXJLandRewardGetCount,param);
			unsigned char  lanGetCount = *(unsigned char *)param;
			if (lanGetCount>0 && characterlevel>=loginEveryDayopenlv)
				loginEveryDay->setVisiable(true);
			else
				loginEveryDay->setVisiable(false);
		}
		
		if(btOnline.valid())
		{
			int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOnlineRewardOpenLv,viplv).c_str());
			data->getParam(WCHDATA_JXJOnlineRewardCount,param);
			unsigned char olRewardCount = *(unsigned char*)param;

			bool showDongHua = false;
			crData *braindata = crBrain::getInstance()->getDataClass();
			if (braindata)
			{
				braindata->getParam(WCHDATA_JXJOnlineGiftDonghua	,param);
				showDongHua = *(bool *)param;
			}

			if(olRewardCount>0 && characterlevel >= openlv)
			{
				btOnline->setVisiable(true);
				if (onlineDongHua.valid())
				{
					if (showDongHua)
						onlineDongHua->setVisiable(true);
					else 
						onlineDongHua->setVisiable(false);
				}
			}
			else
				btOnline->setVisiable(false);
		}
		if (jinglianbtn.valid())
		{
			crVector3i openlv;
			unsigned short freehintcount = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFreeEquipRefineHintCount,viplv).c_str()));
			crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGongfangOpenlvVec3i,viplv).c_str(),openlv);
			data -> getParam(WCHDATA_JXJRefineEquipFreeCount,param);
			unsigned short freecount = *(unsigned short *)param;
			data->getParam(WCHDATA_JXJGongfanglv,param);
			unsigned char gongfanglv = *(unsigned char *)param;
			freehintcount *= 100;
			data->getParam(WCHDATA_JXJEquipRefineTechLv,param);
			unsigned char equipRefineTechLv = *(unsigned char *)param;
			if (freecount >= freehintcount && /*gongfanglv >= openlv[0]*/equipRefineTechLv>0)
			{
				jinglianbtn->setVisiable(true);
			}
			else
			{
				jinglianbtn->setVisiable(false);
			}
		}
		if (guanzhibtn.valid())
		{
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGovBtnOpenLv,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				guanzhibtn->setVisiable(true);
			}
			else
			{
				guanzhibtn->setVisiable(false);
			}
		}
		if (shuishoubtn.valid())
		{
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLevyBtnOpenLv,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				shuishoubtn->setVisiable(true);
			}
			else
			{
				shuishoubtn->setVisiable(false);
			}
			//data->getParam(WCHDATA_JXJLevyCount,param);
			//char levyCount = *(char *)param;
			//crVector2i maxlevycount;
			//crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,viplv).c_str(),maxlevycount);
			//if(levyCount > maxlevycount[LevyType_Gold])
			//{
			//	shuishoubtn->setVisiable(true);
			//}
			//else
			//{
			//	shuishoubtn->setVisiable(false);
			//}
		}
		//if (guanzhibtn.valid())
		//{
		//	guanzhibtn->setVisiable(true);
		//}
		if (lunhuibtn.valid() && lunhuibtn->getVisiable()==false)
		{
			//unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLunHuiBtVisiableLv,viplv).c_str()));
			data->getParam(WCHDATA_JXJClientIsServerCycling,param);
			bool bCycle = *(bool *)param;

			if (bCycle /*|| characterlevel >= openlv*/)
			{
				lunhuibtn->setVisiable(true);
				crFilterRenderManager::getInstance()->showCanvas(crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_Lunhui"),true);
			}
			else
			{
				crFilterRenderManager::getInstance()->showCanvas(crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_Lunhui"),false);
			}
		}

		data->getParam(WCHDATA_JXJFoundCountryGetAward,param);
		unsigned char getAward = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJTreasonBufferTimer,param);
		int bufftime = *(int *)param;
		data->getParam(WCHDATA_JXJTreasonPunishTimer,param);
		int punishtime = *(int *)param;
		//// to do test 要删掉
		//getAward = (unsigned char)-1;
		////
		if (foundCountry.valid())
		{
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFoundCountryOpenLv,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				if (getAward == (unsigned char)-1) 
					foundCountry->setVisiable(false);
				else 
					foundCountry->setVisiable(true);
			}
			else 
			{
				foundCountry->setVisiable(false);
			}

			//foundCountry->setVisiable(false);
		}

		if (guoZhan.valid())
		{
			if (getAward == (unsigned char)-1)
			{
				// 已经全部领取
				guoZhan->setVisiable(true);
				int battlelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevel,viplv).c_str());
				int maxBattlelv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenMaxLimitLevel,viplv).c_str());
				int IncreaseDate = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleOpenLevelIncreaseDate,viplv).c_str());
				int tianshu = 0;
				if (crGlobalHandle::isClient())
				{
					crGlobalHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
				}
				else
				{
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
				}
				if (tianshu >= 0 && tianshu < IncreaseDate)
				{
					if (characterlevel < battlelv)
					{
						if (sequence[0].valid())
							sequence[0]->setVisiable(false);
						if (sequence[1].valid())
							sequence[1]->setVisiable(false);
					}
					else
					{
						data->getParam(WCHDATA_JXJOpenBattle, param);
						bool isbattleopened = *((bool *)param);
						if (isbattleopened)
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(true);
						}
						else
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(false);
						}

						bool isLose = false;
						data->getParam(WCHDATA_JXJShiliID,param);
						unsigned char shiliid = *(unsigned char *)param;

						myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLose,&shiliid));
						if(isLose)
						{
							guoZhan->setVisiable(false);
						}
					}
				}
				else if (tianshu >= IncreaseDate && tianshu < maxBattlelv-IncreaseDate)
				{
					int battlelv_1 = battlelv + tianshu - IncreaseDate + 1;
					if (characterlevel < battlelv_1)
					{
						if (sequence[0].valid())
							sequence[0]->setVisiable(false);
						if (sequence[1].valid())
							sequence[1]->setVisiable(false);
					}
					else
					{
						data->getParam(WCHDATA_JXJOpenBattle, param);
						bool isbattleopened = *((bool *)param);
						if (isbattleopened)
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(true);
						}
						else
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(false);
						}

						bool isLose = false;
						data->getParam(WCHDATA_JXJShiliID,param);
						unsigned char shiliid = *(unsigned char *)param;

						myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLose,&shiliid));
						if(isLose)
						{
							guoZhan->setVisiable(false);
						}
					}
				}
				else
				{
					if (characterlevel < maxBattlelv)
					{
						if (sequence[0].valid())
							sequence[0]->setVisiable(false);
						if (sequence[1].valid())
							sequence[1]->setVisiable(false);
					}
					else
					{
						data->getParam(WCHDATA_JXJOpenBattle, param);
						bool isbattleopened = *((bool *)param);
						if (isbattleopened)
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(true);
						}
						else
						{
							if (sequence[0].valid())
								sequence[0]->setVisiable(false);
						}

						bool isLose = false;
						data->getParam(WCHDATA_JXJShiliID,param);
						unsigned char shiliid = *(unsigned char *)param;

						myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLose,&shiliid));
						if(isLose)
						{
							guoZhan->setVisiable(false);
						}
					}
				}

				//if (characterlevel < battlelv)
				//{
				//	if (sequence[0].valid())
				//		sequence[0]->setVisiable(false);
				//	if (sequence[1].valid())
				//		sequence[1]->setVisiable(false);
				//}
				//else
				//{
				//	data->getParam(WCHDATA_JXJOpenBattle, param);
				//	bool isbattleopened = *((bool *)param);
				//	if (isbattleopened)
				//	{
				//		if (sequence[0].valid())
				//			sequence[0]->setVisiable(true);
				//	}
				//	else
				//	{
				//		if (sequence[0].valid())
				//			sequence[0]->setVisiable(false);
				//	}

				//	bool isLose = false;
				//	data->getParam(WCHDATA_JXJShiliID,param);
				//	unsigned char shiliid = *(unsigned char *)param;

				//	myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLose,&shiliid));
				//	if(isLose)
				//	{
				//		guoZhan->setVisiable(false);
				//	}
				//}
			}
			else
			{
				guoZhan->setVisiable(false);
			}

			if (bufftime>0 || punishtime>0)
				guoZhan->setEnable(false);

			//guoZhan->setVisiable(false);
		}
		int huodongopenLv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJYuekaOpenLv, viplv).c_str());

		if (huodongBtn.valid())
		{
			if (characterlevel < huodongopenLv)
			{
				huodongBtn->setVisiable(false);
			}
			else
			{
				huodongBtn->setVisiable(true);
			}
		}

		if (smallManagerBtn.valid())
		{
			int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJHuoyueduOpenlv,viplv).c_str());
			if (characterlevel >= openlv)
			{
				smallManagerBtn->setVisiable(true);
			}
			else 
				smallManagerBtn->setVisiable(false);
		}

		//data->getParam(WCHDATA_JXJ7DaysGetAward,param);
		//unsigned char  _7DaysAward = *(unsigned char *)param;

		data->getParam(WCHDATA_JXJPlayerLoginDays,param);
		unsigned short loginDays = *(unsigned short *)param;
		
		if (_7DaysIcon.valid())
		{
			unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJ7DaysGetAwardOpenLv	,viplv).c_str()));
			if (characterlevel >= openlv)
			{
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJ7DayData,param);
				char *_7DayData = (char *)param;
				bool bFound = false;
				for (int i = 0; i < 7; ++i)
				{
					for (int j = 0; j < 5; ++j)
					{
						if (*(_7DayData + i*5+j) == 1)
						{
							bFound = true;
							break;
						}
					}
					if (bFound)
					{
						break;
					}
				}
				if (_7DaysLoginDongHua.valid())
				{
					if (bFound)
					{
						_7DaysLoginDongHua->setVisiable(true);
					}
					else
					{
						_7DaysLoginDongHua->setVisiable(false);
					}
				}
				if (loginDays > 7 && !bFound)
				{
					_7DaysIcon->setVisiable(false);
				}
				else
				{
					_7DaysIcon->setVisiable(true);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
			}
			else
			{
				_7DaysIcon->setVisiable(false);
			}
			//unsigned char openlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJ7DaysGetAwardOpenLv	,viplv).c_str()));
			//if (characterlevel >= openlv)
			//
			//if (_7DAYSALLGOT == _7DaysAward || characterlevel < openlv)
			//{
			//	_7DaysIcon->setVisiable(false);

			//}
			//else
			//{
			//	_7DaysIcon->setVisiable(true);
			//	if (_7DaysLoginDongHua.valid())
			//	{
			//		if ( ((_7DaysAward >> 1) & (unsigned char )((1 << loginDays) - 1)) == (unsigned char )((1 << loginDays) - 1))
			//		{
			//			// 全部领取
			//			_7DaysLoginDongHua->setVisiable(false);
			//		}
			//		else
			//		{
			//			_7DaysLoginDongHua->setVisiable(true);
			//		}
			//	}
			//}
		}
		if (firstRecharge.valid())
		{
			int openlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJShouChongOpenlv,viplv).c_str());
			data->getParam(WCHDATA_JXJShouchong,param);
			unsigned char shouchong = *(unsigned char *)param;
			if(shouchong<=1&&characterlevel>=openlv)
			{
				firstRecharge->setVisiable(true);
			}
			else
			{
				firstRecharge->setVisiable(false);
			}
		}

		if (yuekaBtn.valid())
		{
			int yuekaopenlv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJYuekaOpenLv, viplv).c_str());
			data->getParam(WCHDATA_JXJYueKaEndTime,param);
			time_t endtime = *(_crInt64 *)param;
			data->getParam(WCHDATA_JXJYueKaLastGetTime,param);
			time_t lastgettime = *(_crInt64 *)param;
			
			int endday = (int)(endtime/86400);
			int lastgetday = (int)(lastgettime/86400);
			int curday = (int)(curtime/86400);
			if (characterlevel < yuekaopenlv || (endday -curday > 5 && lastgetday >= curday))
			{
				yuekaBtn->setVisiable(false);
			}
			else
			{
				yuekaBtn->setVisiable(true);
			}
		}
		if (zixuanlibao.valid())
		{
			if (characterlevel < huodongopenLv)
			{
				zixuanlibao->setVisiable(false);
				if (zixuanlibaotimer.valid())
				{
					zixuanlibaotimer->setVisiable(false);
				}
			}
			else
			{									
				do 
				{
					/*int zixuanhuodongID = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJZiXuanLibaoHuodongID, viplv).c_str());*/
					int zixuanhuodongID = 0;
					ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
					if (false == huodongTab.valid()) break;
					crTableIO::StrVec huoRecord;
					/*int startTimeIdx = huodongTab->getTitleIndex("开始时间");
					int endTimeIdx = huodongTab->getTitleIndex("结束时间");*/
					int idIdx = huodongTab->getTitleIndex("id");
					int titleIdx = huodongTab->getTitleIndex("标题");

					crTableIO::DataVec huodongIDVec;
					std::string titletext("自选礼包");
					huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
					if (!huodongIDVec.empty()) 
					{
						crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
						crRole *mainRole = playerGameData->getMainRole();
						if (mainRole && mainRole->getDataClass())
						{
							crData *data = mainRole->getDataClass();	
							data->excHandle(MAKEINT64(WCH_LockData,1));
							data->getParam(WCHDATA_JXJNewHuodongMap	,param);
							NewHuodongMap *huodongMap= (NewHuodongMap *)param;
							NewHuodongMap ::iterator itr;
							int huodongid = 0;
							/*for(int si = 0; si<huodongIDVec.size();++si)*/
							for(crTableIO::DataVec::iterator sitr = huodongIDVec.begin();sitr!=huodongIDVec.end();++sitr)
							{
								huodongid = atoi((*sitr)[idIdx].c_str());
								itr = huodongMap->find(huodongid);	
								if(itr != huodongMap->end())
								{
									if(itr->second->isActivation())
									{
										zixuanhuodongID = huodongid;
										break;
									}
								}
							}
							data->excHandle(MAKEINT64(WCH_LockData,0));
						}
					}
					if (zixuanhuodongID > 0)
					{
						if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(zixuanhuodongID),huoRecord) < 0) 
						{
							zixuanlibao->setVisiable(false);
						}
						else
						{
							zixuanlibao->setVisiable(true);
							int endTimeIdx = huodongTab->getTitleIndex("结束时间");
							int opendayIdx = huodongTab->getTitleIndex("开服天数");
							CRCore::crVector2i opendayVec2i;
							crArgumentParser::appAtoVec(huoRecord[opendayIdx],opendayVec2i);
							time_t endtime = 0;
							if (opendayVec2i[0] != 0)
							{
								typedef std::vector<int> Vec5i;
								Vec5i startTimeVec5i;
								std::string dateStr = crGlobalHandle::gData()->gStartDate();
								crArgumentParser::appAtoVec(dateStr,startTimeVec5i);
								startTimeVec5i.resize(5,0);
								time_t nowTime_t = time(0);
								struct  tm  nowTm = *localtime(&nowTime_t);
								struct tm  startTm;
								memset(&startTm,0,sizeof (startTm));
								startTm.tm_year = startTimeVec5i[0] - 1900;
								startTm.tm_mon = startTimeVec5i[1] - 1;
								startTm.tm_mday = startTimeVec5i[2];
								startTm.tm_hour = startTimeVec5i[3];
								startTm.tm_min = startTimeVec5i[4];
								startTm.tm_isdst = nowTm.tm_isdst;
								time_t startTime_t = mktime(&startTm);
								if (opendayVec2i[1] != 0)
								{
									endtime = startTime_t + opendayVec2i[1] * c_24hours;
								}
								else
								{
									endtime = startTime_t + opendayVec2i[0] * c_24hours;
								}
							}
							else
							{
								CRCore::crVector3i endtimeVec;
								crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
								struct tm tm_endtime;
								memset(&tm_endtime,0,sizeof(tm_endtime));
								tm_endtime.tm_year = endtimeVec[0] - 1900;
								tm_endtime.tm_mon = endtimeVec[1] - 1;
								tm_endtime.tm_mday = endtimeVec[2];
								tm_endtime.tm_hour = 23;
								tm_endtime.tm_min = 59;
								tm_endtime.tm_sec = 59;
								endtime = mktime(&tm_endtime);
							}
							time_t remainingtime = endtime - curtime;
							int _hour = remainingtime /3600;
							int _min = (remainingtime  - 3600*_hour)/60;
							int _sec = (remainingtime  - 3600*_hour -60*_min);
							std::string remaining;
							if (_hour >= 24)
							{
								remaining = "还有" + crArgumentParser::appItoa(_hour/24) + "天";
							}
							else if (_sec >= 0)
							{
								remaining =  crArgumentParser::appItoa(_hour) 
									+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
							}
							else
							{
								remaining = "0:00:00";
							}
							if (zixuanlibaotimer.valid())
							{
								zixuanlibaotimer->setVisiable(zixuanlibao->getVisiable());
								zixuanlibaotimer->clearString();
								zixuanlibaotimer->setString(remaining);
							}

						}
					}
					else
					{
						zixuanlibao->setVisiable(false);
					}

				} while (0);

			}
			
		}

		if (chongzhihuikuiBtn.valid())
		{
			if (characterlevel < huodongopenLv)
			{
				chongzhihuikuiBtn->setVisiable(false);
				//if (chongzhihuikuitimer.valid())
				//{
				//	chongzhihuikuitimer->setVisiable(false);
				//}
			}
			else
			{
				do 
				{
					bool visible = false;
					crRole *mainRole = myPlayer->getMainRole();
					if(mainRole && mainRole->getDataClass())
					{
						crTableIO::StrVec record;
						ref_ptr<crTableIO>huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
						if(!huodongTab.valid()) break;
						int displayid = huodongTab->getTitleIndex("活动显示");
						int titleid = huodongTab->getTitleIndex("标题");
						crData *roleData = mainRole->getDataClass();
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *huodongMap= (NewHuodongMap *)param;
						NewHuodongMap ::iterator itr = huodongMap->begin();
						int id;
						for (; itr != huodongMap->end(); ++ itr)
						{
							//if(itr->second->isIntimeCheck())
							id = itr->first;
							if(itr->second->getActivation()==1 || (itr->second->getComplete() && itr->second->isIntimeCheck()))
							{
								if(huodongTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
								{
									if(atoi(record[displayid].c_str()) == 2)
									{
										visible = true;
										break;
									}
								}
							}
						}
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
					}
					chongzhihuikuiBtn->setVisiable(visible);
				} while (0);
				//do 
				//{
				//	int chongzhihuikuiID = 0;
				//	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
				//	ref_ptr<crRole> mainRole = playerData->getMainRole();
				//	ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				//	if(mainRole.valid() && mainRole->getDataClass() )
				//	{
				//		int titleIdx = huodongTab->getTitleIndex("标题");
				//		int idIdx = huodongTab->getTitleIndex("id");
				//		crTableIO::DataVec huodongIDVec;
				//		std::string titletext("累计充值回馈");

				//		crData *roleData = mainRole->getDataClass();	
				//		roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
				//		NewHuodongMap *huodongMap= (NewHuodongMap *)param;
				//		NewHuodongMap ::iterator itr = huodongMap->end();

				//		huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
				//		if (!huodongIDVec.empty()) 
				//		{	
				//			int huodongid = 0;
				//			for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
				//			{
				//				huodongid = atoi((*siter)[idIdx].c_str());
				//				itr = huodongMap->find(huodongid);	
				//				if(itr != huodongMap->end())
				//				{
				//					if(itr->second->isActivation())
				//					{
				//						chongzhihuikuiID = huodongid;
				//						break;
				//					}
				//				}
				//			}

				//		}
				//		crTableIO::StrVec huoRecord;
				//		if (chongzhihuikuiID > 0)
				//		{
				//			if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(chongzhihuikuiID),huoRecord) < 0) 
				//			{
				//				chongzhihuikuiBtn->setVisiable(true);
				//			}
				//			else
				//			{
				//				chongzhihuikuiBtn->setVisiable(true);

				//				//int startTimeIdx = huodongTab->getTitleIndex("开始时间");
				//				//int endTimeIdx = huodongTab->getTitleIndex("结束时间");
				//				//CRCore::crVector3i starttimeVec;
				//				//CRCore::crVector3i endtimeVec;
				//				//crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
				//				//crArgumentParser::appAtoVec(huoRecord[startTimeIdx],starttimeVec);
				//				//struct tm tm_endtime;
				//				//memset(&tm_endtime,0,sizeof(tm_endtime));
				//				//struct tm tm_starttime;
				//				//memset(&tm_starttime,0,sizeof(tm_starttime));
				//				//tm_endtime.tm_year = endtimeVec[0] - 1900;
				//				//tm_endtime.tm_mon = endtimeVec[1] - 1;
				//				//tm_endtime.tm_mday = endtimeVec[2];
				//				//tm_endtime.tm_hour = 23;
				//				//tm_endtime.tm_min = 59;
				//				//tm_endtime.tm_sec = 59;
				//				//time_t endtime = mktime(&tm_endtime);
				//				//tm_starttime.tm_year = starttimeVec[0] - 1900;
				//				//tm_starttime.tm_mon = starttimeVec[1] - 1;
				//				//tm_starttime.tm_mday = starttimeVec[2];
				//				//tm_starttime.tm_hour = 0;
				//				//tm_starttime.tm_min = 0;
				//				//tm_starttime.tm_sec = 0;
				//				//time_t starttime = mktime(&tm_starttime);
				//				//time_t remainingtime = endtime - curtime;

				//				//int _hour = remainingtime /3600;
				//				//int _min = (remainingtime  - 3600*_hour)/60;
				//				//int _sec = (remainingtime  - 3600*_hour -60*_min);
				//				//std::string remaining;
				//				//if (_hour >= 24)
				//				//{
				//				//	remaining = "还有" + crArgumentParser::appItoa(_hour/24) + "天";
				//				//}
				//				//else if (_sec >= 0)
				//				//{
				//				//	remaining =  crArgumentParser::appItoa(_hour) 
				//				//		+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
				//				//}
				//				//else
				//				//{
				//				//	remaining = "0:00:00";
				//				//}
				//				//if (chongzhihuikuitimer.valid())
				//				//{
				//				//	chongzhihuikuitimer->setVisiable(chongzhihuikuiBtn->getVisiable());
				//				//	chongzhihuikuitimer->clearString();
				//				//	chongzhihuikuitimer->setString(remaining);
				//				//}

				//			}
				//		}
				//		else
				//		{
				//			chongzhihuikuiBtn->setVisiable(true);
				//		}
				//	}
				//} while (0);
			}
			
		}
		if (zhengzhanshopBtn.valid())
		{
			unsigned char needlv = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJZhengZhanShopKaiQiTiaoJian).c_str()));
			if(characterlevel >= needlv)
				zhengzhanshopBtn->setVisiable(true);
			else
				zhengzhanshopBtn->setVisiable(false);
		}
		if (danbichongzhiBtn.valid())
		{
			if (characterlevel < huodongopenLv)
			{
				danbichongzhiBtn->setVisiable(false);
				if (danbichongzhitimer.valid())
				{
					danbichongzhitimer->setVisiable(false);
				}
			}
			else
			{
				do 
				{
					int danbichongzhiID = 0;
					crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
					ref_ptr<crRole> mainRole = playerData->getMainRole();
					ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
					if(mainRole.valid() && mainRole->getDataClass() )
					{
						int titleIdx = huodongTab->getTitleIndex("标题");
						int idIdx = huodongTab->getTitleIndex("id");
						crTableIO::DataVec huodongIDVec;
						std::string titletext("单笔充值回馈");

						crData *roleData = mainRole->getDataClass();
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *huodongMap= (NewHuodongMap *)param;
						NewHuodongMap ::iterator itr = huodongMap->end();

						huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
						if (!huodongIDVec.empty()) 
						{	
							int huodongid = 0;
							for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
							{
								huodongid = atoi((*siter)[idIdx].c_str());
								itr = huodongMap->find(huodongid);	
								if(itr != huodongMap->end())
								{
									if(itr->second->isActivation())
									{
										danbichongzhiID = huodongid;
										break;
									}
								}
							}
						}
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
						crTableIO::StrVec huoRecord;
						if (danbichongzhiID > 0)
						{
							if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(danbichongzhiID),huoRecord) < 0) 
							{
								danbichongzhiBtn->setVisiable(false);
							}
							else
							{
								danbichongzhiBtn->setVisiable(true);

								int startTimeIdx = huodongTab->getTitleIndex("开始时间");
								int endTimeIdx = huodongTab->getTitleIndex("结束时间");
								CRCore::crVector3i starttimeVec;
								CRCore::crVector3i endtimeVec;
								crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
								crArgumentParser::appAtoVec(huoRecord[startTimeIdx],starttimeVec);
								struct tm tm_endtime;
								memset(&tm_endtime,0,sizeof(tm_endtime));
								struct tm tm_starttime;
								memset(&tm_starttime,0,sizeof(tm_starttime));
								tm_endtime.tm_year = endtimeVec[0] - 1900;
								tm_endtime.tm_mon = endtimeVec[1] - 1;
								tm_endtime.tm_mday = endtimeVec[2];
								tm_endtime.tm_hour = 23;
								tm_endtime.tm_min = 59;
								tm_endtime.tm_sec = 59;
								time_t endtime = mktime(&tm_endtime);
								tm_starttime.tm_year = starttimeVec[0] - 1900;
								tm_starttime.tm_mon = starttimeVec[1] - 1;
								tm_starttime.tm_mday = starttimeVec[2];
								tm_starttime.tm_hour = 0;
								tm_starttime.tm_min = 0;
								tm_starttime.tm_sec = 0;
								time_t starttime = mktime(&tm_starttime);
								time_t remainingtime = endtime - curtime;

								int _hour = remainingtime /3600;
								int _min = (remainingtime  - 3600*_hour)/60;
								int _sec = (remainingtime  - 3600*_hour -60*_min);
								std::string remaining;
								if (_hour >= 24)
								{
									remaining = "还有" + crArgumentParser::appItoa(_hour/24) + "天";
								}
								else if (_sec >= 0)
								{
									remaining =  crArgumentParser::appItoa(_hour) 
										+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
								}
								else
								{
									remaining = "0:00:00";
								}
								if (danbichongzhitimer.valid())
								{
									danbichongzhitimer->setVisiable(danbichongzhiBtn->getVisiable());
									danbichongzhitimer->clearString();
									danbichongzhitimer->setString(remaining);
								}
							}

						}
						else
						{
							danbichongzhiBtn->setVisiable(false);
						}
					}
				} while (0);
			}	
		}

		if (xiaofeihuikuiBtn.valid())
		{
			if (characterlevel < huodongopenLv)
			{
				xiaofeihuikuiBtn->setVisiable(false);
				if (xiaofeihuikuitimer.valid())
				{
					xiaofeihuikuitimer->setVisiable(false);
				}
			}
			else
			{
				do 
				{
					int xiaofeihuikuiID = 0;
					crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
					ref_ptr<crRole> mainRole = playerData->getMainRole();
					ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
					if(mainRole.valid() && mainRole->getDataClass() )
					{
						int titleIdx = huodongTab->getTitleIndex("标题");
						int idIdx = huodongTab->getTitleIndex("id");
						crTableIO::DataVec huodongIDVec;
						std::string titletext("累计消费回馈");

						crData *roleData = mainRole->getDataClass();
						roleData->excHandle(MAKEINT64(WCH_LockData,1));
						roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
						NewHuodongMap *huodongMap= (NewHuodongMap *)param;
						NewHuodongMap ::iterator itr = huodongMap->end();

						huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
						if (!huodongIDVec.empty()) 
						{	
							int huodongid = 0;
							for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
							{
								huodongid = atoi((*siter)[idIdx].c_str());
								itr = huodongMap->find(huodongid);	
								if(itr != huodongMap->end())
								{
									if(itr->second->isActivation())
									{
										xiaofeihuikuiID = huodongid;
										break;
									}
								}
							}
						}
						roleData->excHandle(MAKEINT64(WCH_LockData,0));
						crTableIO::StrVec huoRecord;
						if (xiaofeihuikuiID > 0)
						{
							if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(xiaofeihuikuiID),huoRecord) < 0) 
							{
								xiaofeihuikuiBtn->setVisiable(false);
							}
							else
							{
								xiaofeihuikuiBtn->setVisiable(true);

								int startTimeIdx = huodongTab->getTitleIndex("开始时间");
								int endTimeIdx = huodongTab->getTitleIndex("结束时间");
								CRCore::crVector3i starttimeVec;
								CRCore::crVector3i endtimeVec;
								crArgumentParser::appAtoVec(huoRecord[endTimeIdx],endtimeVec);
								crArgumentParser::appAtoVec(huoRecord[startTimeIdx],starttimeVec);
								struct tm tm_endtime;
								memset(&tm_endtime,0,sizeof(tm_endtime));
								struct tm tm_starttime;
								memset(&tm_starttime,0,sizeof(tm_starttime));
								tm_endtime.tm_year = endtimeVec[0] - 1900;
								tm_endtime.tm_mon = endtimeVec[1] - 1;
								tm_endtime.tm_mday = endtimeVec[2];
								tm_endtime.tm_hour = 23;
								tm_endtime.tm_min = 59;
								tm_endtime.tm_sec = 59;
								time_t endtime = mktime(&tm_endtime);
								tm_starttime.tm_year = starttimeVec[0] - 1900;
								tm_starttime.tm_mon = starttimeVec[1] - 1;
								tm_starttime.tm_mday = starttimeVec[2];
								tm_starttime.tm_hour = 0;
								tm_starttime.tm_min = 0;
								tm_starttime.tm_sec = 0;
								time_t starttime = mktime(&tm_starttime);
								time_t remainingtime = endtime - curtime;

								int _hour = remainingtime /3600;
								int _min = (remainingtime  - 3600*_hour)/60;
								int _sec = (remainingtime  - 3600*_hour -60*_min);
								std::string remaining;
								if (_hour >= 24)
								{
									remaining = "还有" + crArgumentParser::appItoa(_hour/24) + "天";
								}
								else if (_sec >= 0)
								{
									remaining =  crArgumentParser::appItoa(_hour) 
										+ ":" + crArgumentParser::appItoa(_min) +":" + crArgumentParser::appItoa(_sec);
								}
								else
								{
									remaining = "0:00:00";
								}
								if (xiaofeihuikuitimer.valid())
								{
									xiaofeihuikuitimer->setVisiable(xiaofeihuikuiBtn->getVisiable());
									xiaofeihuikuitimer->clearString();
									xiaofeihuikuitimer->setString(remaining);
								}

							}
						}
						else
						{
							xiaofeihuikuiBtn->setVisiable(false);
						}
					}
				} while (0);
			}	
		}

		if (zhoukaBtn.valid())
		{
			if (characterlevel< huodongopenLv)
			{
				zhoukaBtn->setVisiable(false);
			}
			else
			{
				do 
				{
					data->getParam(WCHDATA_JXJZhouKaEndTime,param);
					time_t endtime = *(_crInt64 *)param;
					data->getParam(WCHDATA_JXJGainedZhouKaID,param);
					int zhoukaid = *(int *)param;
					data->getParam(WCHDATA_JXJZhouKaGetDayIdx,param);
					int zhoukadayIdx = *(int *)param;
					data->getParam(WCHDATA_JXJZhouKaGetStatus,param);
					unsigned char zhoukastatus = *(unsigned char *)param;
					if (endtime > 0 && zhoukaid > 0 && zhoukadayIdx >=0)
					{
						if (((zhoukastatus >> zhoukadayIdx) & 1) == 0)
						{
							zhoukaBtn->setVisiable(true);
						}
						else
						{
							zhoukaBtn->setVisiable(false);
						}
					}
					else
					{
						int zhoukaID = 0;
						crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
						ref_ptr<crRole> mainRole = playerData->getMainRole();
						ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
						if(mainRole.valid() && mainRole->getDataClass() )
						{
							int titleIdx = huodongTab->getTitleIndex("标题");
							int idIdx = huodongTab->getTitleIndex("id");
							crTableIO::DataVec huodongIDVec;
							std::string titletext("小周卡");
							crData *roleData = mainRole->getDataClass();
							roleData->excHandle(MAKEINT64(WCH_LockData,1));
							roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
							NewHuodongMap *huodongMap= (NewHuodongMap *)param;
							NewHuodongMap ::iterator itr = huodongMap->end();
							huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
							if (!huodongIDVec.empty()) 
							{	
								int huodongid = 0;
								for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
								{
									huodongid = atoi((*siter)[idIdx].c_str());
									itr = huodongMap->find(huodongid);	
									if(itr != huodongMap->end())
									{
										if(itr->second->isActivation())
										{
											zhoukaID = huodongid;
											break;
										}
									}
								}
							}
							roleData->excHandle(MAKEINT64(WCH_LockData,0));
							crTableIO::StrVec huoRecord;
							if (zhoukaID > 0)
							{
								if (huodongTab->queryOneRecord(idIdx,crArgumentParser::appItoa(zhoukaID),huoRecord) < 0) 
								{
									zhoukaBtn->setVisiable(false);
								}
								else
								{
									zhoukaBtn->setVisiable(true);
								}
							}
							else
							{
								zhoukaBtn->setVisiable(false);
							}
						}

					}	
				} while (0);
			}
			
		}

		if (meirihaili.valid())
		{
			if (characterlevel >= huodongopenLv)
			{
				int meirihaoliID = 0;
				crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
				ref_ptr<crRole> mainRole = playerData->getMainRole();
				ref_ptr<crTableIO> huodongTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJHuodongTab);
				if(mainRole.valid() && mainRole->getDataClass() )
				{
					int titleIdx = huodongTab->getTitleIndex("标题");
					int idIdx = huodongTab->getTitleIndex("id");
					crTableIO::DataVec huodongIDVec;
					std::string titletext("每日十元");
					crData *roleData = mainRole->getDataClass();
					roleData->excHandle(MAKEINT64(WCH_LockData,1));
					roleData->getParam(WCHDATA_JXJNewHuodongMap	,param);
					NewHuodongMap *huodongMap= (NewHuodongMap *)param;
					NewHuodongMap ::iterator itr = huodongMap->end();
					huodongTab->queryRecords(titleIdx,titletext,huodongIDVec);
					if (!huodongIDVec.empty()) 
					{	
						int huodongid = 0;
						for(crTableIO::DataVec::iterator siter = huodongIDVec.begin(); siter!=huodongIDVec.end();++siter)
						{
							huodongid = atoi((*siter)[idIdx].c_str());
							itr = huodongMap->find(huodongid);	
							if(itr != huodongMap->end() && itr->second.valid())
							{
								if(itr->second->isActivation())
								{
									meirihaoliID = huodongid;
									break;
								}
							}
						}

					}
					if (meirihaoliID > 0)
					{
						meirihaili->setVisiable(true);
						if (meirihailisq.valid())
						{
							if (itr != huodongMap->end() && itr->second.valid())
							{
								if (itr->second->isCanAdvance() && !itr->second->getComplete())
								{
									meirihailisq->setVisiable(true);
								}
								else
								{
									meirihailisq->setVisiable(false);
								}
							}
						}
					}
					else
					{
						meirihaili->setVisiable(false);
					}
					roleData->excHandle(MAKEINT64(WCH_LockData,0));
				}

			}
			else
			{
				meirihaili->setVisiable(false);
			}
		}

		if (pvp.valid())
		{
			pvp->setVisiable(false);
		}
		////周六显示国家官员候选人自荐图标 仅候选人显示
		if (houxuanren.valid())
		{
			unsigned char electIcon = 0;
			data->getParam(WCHDATA_JXJElectIconTimeControl, param);
			electIcon = *(unsigned char *)param;
			bool recommendMyself = false;
			data->getParam(WCHDATA_JXJCountryRecommendSet,param);
			if (param)
			{
				recommendMyself = *(bool *)param;
			}
			if (recommendMyself || electIcon != 6)
			{ 
				houxuanren->setVisiable(false);
			}
			else
			{
				bool candidate = false;
				data->getParam(WCHDATA_JXJCandidate, param);
				if (param)
				{
					candidate = *(bool *)param;
				}
				if (candidate)
				{
					houxuanren->setVisiable(true);
				}
				else
				{
					houxuanren->setVisiable(false);
				}
			}
		}
		////周日显示官员选举图标 有选举权资格的人才会显示
		if (guanyuanxuanju.valid()) 
		{
			unsigned char electIcon = 0;
			data->getParam(WCHDATA_JXJElectIconTimeControl, param);
			electIcon = *(unsigned char *)param;
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Country1");
			SelectLeaderIdSet *selectLeaderIdSet;
			if(canvas.valid() && canvas->getDataClass())
			{
				crData *canvasdata = canvas->getDataClass();
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));

				canvasdata->getParam(WCHDATA_JXJSelectLeaderSet,param);
				selectLeaderIdSet = (SelectLeaderIdSet *)param;

				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
			}
			if (!selectLeaderIdSet->empty() || electIcon != 7)
			{
				guanyuanxuanju->setVisiable(false);
			}
			else
			{
				bool elector = false;
				data->getParam(WCHDATA_JXJElector,param);
				if (param)
				{
					elector = *(bool *)param;
				}
				if (elector)
				{
					guanyuanxuanju->setVisiable(true);
				}
				else
				{
					guanyuanxuanju->setVisiable(false);
				}
			}
		}
		////周一显示投票结果图标，对所有玩家可见
		if (toupiaojieguo.valid())
		{
			unsigned char electIcon = 0;
			data->getParam(WCHDATA_JXJElectIconTimeControl, param);
			electIcon = *(unsigned char *)param;
			if (electIcon == 1)
			{
				toupiaojieguo->setVisiable(true);
			}
			else
			{
				toupiaojieguo->setVisiable(false);
			}
		}
		//叛国图标显示
		if (panguobtn.valid())
		{
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char myshiliid = *(unsigned char *)param;
			bool isLoseCountry = false;
			myPlayer->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&myshiliid));
			ref_ptr<crCanvasNode> panguocanvas = crFilterRenderManager::getInstance()->findCanvas("UI_Treason_Country");
			if (panguocanvas.valid())
			{
				if(isLoseCountry && panguocanvas->getVisiable()==false)
					panguobtn->setVisiable(true);
				else
					panguobtn->setVisiable(false);
			}
		}
		//新手指南图标显示
		if (newbieIntroduction.valid())
		{
			data->getParam(WCHDATA_JXJGameIntroductionNoTipsCheck, param);
			bool noTips = *(bool *)param;
			newbieIntroduction->setVisiable(!noTips);
		}

		//军团开服活动图标显示
		if (junTuanHuodongBtn.valid())
		{
			int Deadline = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJConsortiaPromotionDeadline, 0).c_str())); //活动截止到开服第几天
			data->getParam(WCHDATA_JXJGameStartedDays, param);
			unsigned short startDays = *(unsigned short *)param;
			data->getParam(WCHDATA_JXJHadGottenConsortiaPromotionReward, param);
			bool hadGotten = *(bool *)param;
			data->getParam(WCHDATA_JXJConsortiaPromotionMyRankVal, param);
			unsigned char myRankVal = *(unsigned char *)param;
			if (hadGotten)
			{
				junTuanHuodongBtn->setVisiable(false);
			}
			else 
			{
				if (myRankVal > 0)
				{
					junTuanHuodongBtn->setVisiable(true);
				}
				else 
				{//未入选的玩家在结果公布后的第二天不再显示该图标
					if (startDays <= Deadline)
					{
						junTuanHuodongBtn->setVisiable(true);
					}
					else
					{
						junTuanHuodongBtn->setVisiable(false);
					}
				}
			}
			if (junTuanHuodongSequence.valid())
			{
				ref_ptr<crCanvasNode> junTuanHuodongCanvas = crFilterRenderManager::getInstance()->findCanvas("Juntuan_Paidui");
				if (junTuanHuodongCanvas.valid() && junTuanHuodongCanvas->getDataClass())
				{
					ref_ptr<crData> canvasData = junTuanHuodongCanvas->getDataClass();
					canvasData->getParam(WCHDATA_JXJConsortiaPromotionTimer, param);
					int restTimer = *(int *)param;
					if (!hadGotten && myRankVal > 0 && restTimer <= 0)
					{
						junTuanHuodongSequence->setVisiable(true);
					}
					else 
					{
						junTuanHuodongSequence->setVisiable(false);
					}
				}
			}
		}

		// 按图标排列顺序 从右到 左 依次push_back
		typedef std::vector<ref_ptr<crButtonWidgetNode> > BtnVec;
		BtnVec btnVec;
		btnVec.push_back(smallManagerBtn);
		btnVec.push_back(foundCountry);
		btnVec.push_back(guoZhan);
		btnVec.push_back(lunhuibtn);
		btnVec.push_back(shuishoubtn);
		btnVec.push_back(guanzhibtn);
		btnVec.push_back(jinglianbtn);
		btnVec.push_back(btOnline);
		btnVec.push_back(loginEveryDay);
		btnVec.push_back(huodongBtn);
		btnVec.push_back(_7DaysIcon);
		btnVec.push_back(firstRecharge);
		btnVec.push_back(yuekaBtn);
		btnVec.push_back(zixuanlibao);
		btnVec.push_back(chongzhihuikuiBtn);
		btnVec.push_back(zhengzhanshopBtn);
		btnVec.push_back(danbichongzhiBtn);
		btnVec.push_back(xiaofeihuikuiBtn);
		btnVec.push_back(zhoukaBtn);
		btnVec.push_back(pvp);
		btnVec.push_back(houxuanren);
		btnVec.push_back(guanyuanxuanju);
		btnVec.push_back(toupiaojieguo);
		btnVec.push_back(panguobtn);
		btnVec.push_back(yellowNewbie);
		btnVec.push_back(yellow);
		btnVec.push_back(meirihaili);
		btnVec.push_back(newbieIntroduction);
		btnVec.push_back(junTuanHuodongBtn);
		ref_ptr<crTableIO> iconOrderTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJIconCoordinateTab);
		int coordIdx = -1;
		if (iconOrderTab.valid())
		{
			coordIdx = iconOrderTab->getTitleIndex("坐标");
		}
		crTableIO::StrVec coordRec;
		crVector2f vec2f;
		unsigned char iconOrder = 1;
		unsigned char iconRevOrder = 1;
		//if (iconOrderTab.valid())
		//{
		//	iconRevOrder = iconOrderTab->getRowCount();
		//}
		int rowcount = iconOrderTab->getRowCount();
		float height = crDisplaySettings::instance()->getViewHeight();// / 2;
		float width = crDisplaySettings::instance()->getViewWidth();// / 2;

		//const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		//const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
		crVector3 iconPos;
		for (BtnVec::iterator bvItr = btnVec.begin(); bvItr != btnVec.end(); ++ bvItr)
		{
			if ((*bvItr).valid())
			{
				iconPos.set(0.0f,0.0f,0.0f);
				if ((*bvItr)->getVisiable())
				{
					(*bvItr)->setMsgTraverse(KM_NotTrans);
					if (iconOrderTab.valid() && iconOrderTab->queryOneRecord(0,crArgumentParser::appItoa(iconOrder),coordRec) >= 0)
					{
						crArgumentParser::appAtoVec(coordRec[coordIdx],vec2f);
						iconPos = crFilterRenderManager::getInstance()->getIdenticPosition(width - vec2f[0],height - vec2f[1]);
						//x = 1.0f - vec2f[0] / width;
						//y = 1.0f - vec2f[1] / height;
						if(iconOrder<rowcount)
							++iconOrder;
					}
				}
				else
				{
					if (iconOrderTab.valid() && iconOrderTab->queryOneRecord(0,crArgumentParser::appItoa(1/*iconOrder*/),coordRec) >= 0)
					{
						crArgumentParser::appAtoVec(coordRec[coordIdx],vec2f);
						iconPos = crFilterRenderManager::getInstance()->getIdenticPosition(width - vec2f[0],height - vec2f[1]);
						//x = 1.0f - vec2f[0] / width;
						//y = 1.0f - vec2f[1] / height;
						//if(iconRevOrder<rowcount)
						//	++iconRevOrder;
					}
				}
				(*bvItr)->setEnableMatrix(true);
				(*bvItr)->setMatrix(crMatrix ::translate(iconPos));
				//if ( (*bvItr) == btOnline )
				//{
				//	if (onlineInput.valid() /*&& onlineInput->getVisiable()*/)
				//	{
				//		onlineInput->setVisiable((*bvItr)->getVisiable());
				//		onlineInput->setEnableMatrix(true);
				//		onlineInput->setMatrix(crMatrix ::translate(x,y,0.0f));
				//	}
				//}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISetNoTipsSwitchMethod
//
/////////////////////////////////////////
crJXJUISetNoTipsSwitchMethod::crJXJUISetNoTipsSwitchMethod():
	m_this(NULL)
{
}
crJXJUISetNoTipsSwitchMethod::crJXJUISetNoTipsSwitchMethod(const crJXJUISetNoTipsSwitchMethod& handle):
	crMethod(handle),
	m_notipsw(handle.m_notipsw),
	m_checkbox(handle.m_checkbox),
	m_this(handle.m_this)
{
}
void crJXJUISetNoTipsSwitchMethod::inputParam(int i, void *param)
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
void crJXJUISetNoTipsSwitchMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_notipsw = str;
		break;
	case 1:
		m_checkbox = str;
		break;
	default:
		break;
	}
}
void crJXJUISetNoTipsSwitchMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		ref_ptr<crCheckBoxWidgetNode> checkbox = dynamic_cast<crCheckBoxWidgetNode *>(m_this -> getWidget(m_checkbox));
		crMultiSwitch *vipinfosw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_notipsw));
		crData *braindata = crBrain::getInstance()->getDataClass();
		braindata->getParam(WCHDATA_JXJCurGoldHintType,param);
		int goldhinttypd = *(int *)param;
		crData * data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJGoldCostHintInt,param);
		unsigned int ifhint = *(unsigned int *)param & goldhinttypd;
		if (ifhint == 0)
		{
			ifhint = 1;
		}
		else
		{
			ifhint = 0;
		}
		if (checkbox.valid() && vipinfosw)
		{
			if (goldhinttypd >= 0 )
			{
				vipinfosw->setActiveSwitchSet(1);
				checkbox->select(ifhint);
			}
			else
			{
				vipinfosw->setActiveSwitchSet(0);
				checkbox->select(ifhint);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIClearNoTipsParamMethod
//
/////////////////////////////////////////
crJXJUIClearNoTipsParamMethod::crJXJUIClearNoTipsParamMethod()
{
}
crJXJUIClearNoTipsParamMethod::crJXJUIClearNoTipsParamMethod(const crJXJUIClearNoTipsParamMethod& handle):
	crMethod(handle)
{
}
void crJXJUIClearNoTipsParamMethod::inputParam(int i, void *param)
{
}
void crJXJUIClearNoTipsParamMethod::addParam(int i, const std::string& str)
{
}
void crJXJUIClearNoTipsParamMethod::operator()(crHandle &handle)
{
	int hinttype = -1;
	crData *braindata = crBrain::getInstance()->getDataClass();
	braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
}
/////////////////////////////////////////
//
//crJXJUIListControlLostFocusMethod
//
/////////////////////////////////////////
crJXJUIListControlLostFocusMethod::crJXJUIListControlLostFocusMethod()
{
}
crJXJUIListControlLostFocusMethod::crJXJUIListControlLostFocusMethod(const crJXJUIListControlLostFocusMethod& handle):
	crMethod(handle)
{
}
void crJXJUIListControlLostFocusMethod::inputParam(int i, void *param)
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
		m_this = (crListControlWidgetNode*)param;
		break;
	}
}
void crJXJUIListControlLostFocusMethod::addParam(int i, const std::string& str)
{
}
void crJXJUIListControlLostFocusMethod::operator()(crHandle &handle)
{
	if(m_this)m_this->select(-1);
}
/////////////////////////////////////////
//
//crJXJCheckMarketLimitBuyMethod
//
/////////////////////////////////////////
crJXJCheckMarketLimitBuyMethod::crJXJCheckMarketLimitBuyMethod():
	m_this(NULL),
	m_output(false)
{
}

crJXJCheckMarketLimitBuyMethod::crJXJCheckMarketLimitBuyMethod(const crJXJCheckMarketLimitBuyMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_output(false)
{
}

void crJXJCheckMarketLimitBuyMethod::inputParam(int i, void *param)
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
			m_output = (int *)(LOINT64(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}

void crJXJCheckMarketLimitBuyMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckMarketLimitBuyMethod::operator()(crHandle &handle)
{
	void *param;
	if (m_this && m_this->getDataClass())
	{
		crData *data = m_this->getDataClass();
		crTableIO::StrVec shijirecord,viprecord;
		data->getParam(WCHDATA_MarketBuyResourceCount,param);
		int buyresourcecount = *(int *)param;
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJShijilv,param);
		int shijilv = (int)(*(unsigned char *)param);
		ref_ptr<crTableIO>shijiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShijiTab);
		int buymaxindex = shijiTab->getTitleIndex("每天购买量");
		ref_ptr<crTableIO>viplvtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
		int vipaddindex = viplvtab->getTitleIndex("市集购买加成");
		int limitbuy = 0;
		int mycount = *(int *)param;
		if(	shijiTab->queryOneRecord(0,crArgumentParser::appItoa((int)shijilv),shijirecord) >= 0 && buymaxindex > 0 && vipaddindex > 0)
		{
			if(viplv > 0 && viplvtab->queryOneRecord(0,crArgumentParser::appItoa((int)viplv),viprecord) >= 0)
			{
				limitbuy = atof(shijirecord[buymaxindex].c_str()) * (1.0f + atof(viprecord[vipaddindex].c_str()) * 0.01f) - buyresourcecount;
			}
			else
			{
				limitbuy = atof(shijirecord[buymaxindex].c_str()) - buyresourcecount;
			}
		}
		if (limitbuy < 0)
		{
			limitbuy = 0;
		}
		*m_output = limitbuy;
	}
}
/////////////////////////////////////////
//
//crJXJUIOperateInitMethod
//
/////////////////////////////////////////
crJXJUIOperateInitMethod::crJXJUIOperateInitMethod()
{
}
crJXJUIOperateInitMethod::crJXJUIOperateInitMethod(const crJXJUIOperateInitMethod& handle):
	crMethod(handle),
	m_list1(handle.m_list1),
	m_list2(handle.m_list2)
{
}
void crJXJUIOperateInitMethod::inputParam(int i, void *param)
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
void crJXJUIOperateInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_list1 = str;
		break;
	case 1:
		m_list2 = str;
		break;
	case 2:
		m_onlinelist = str;
		break;
	default:
		break;
	}
}
void crJXJUIOperateInitMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crTableWidgetNode> list1 = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list1));
		ref_ptr<crTableWidgetNode> list2 = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list2));
		ref_ptr<crTableWidgetNode> onlinelist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_onlinelist));
		if (list1.valid() && list2.valid() && onlinelist.valid())
		{
			list1->addData(0,0,"注册&在线与流失");
			list1->addData(0,1,"角色管理");
			list1->addData(0,2,"货币管理与充值管理");
			list1->addData(0,3,"运营管理");
			//////////////////////////////
			list2->addData(0,0,"总创建统计");
			list2->addData(0,1,"在线实时统计");
			list2->addData(0,2,"时间段流失率统计");
			list2->addData(0,3,"加载完成数据");
			list2->addData(0,4,"玩家回访");
			list2->addData(0,5,"");
			list2->addData(0,6,"铜币资源产出消耗记录");
			///////////////////////////////
			onlinelist->addData(0,0,"服务器名");
			onlinelist->addData(1,0,"人数");
			onlinelist->addData(2,0,"查询时间");
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIOperateUpdateMethod
//
/////////////////////////////////////////
crJXJUIOperateUpdateMethod::crJXJUIOperateUpdateMethod()
{
}
crJXJUIOperateUpdateMethod::crJXJUIOperateUpdateMethod(const crJXJUIOperateUpdateMethod& handle):
	crMethod(handle),
	m_list1(handle.m_list1),
	m_list2(handle.m_list2),
	m_pagesw(handle.m_pagesw)
{
	for (int i = 0; i < 4; i++)
	{
		m_sepagesw[i] = handle.m_sepagesw[i];
	}
}
void crJXJUIOperateUpdateMethod::inputParam(int i, void *param)
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
void crJXJUIOperateUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_list1 = str;
		break;
	case 1:
		m_list2 = str;
		break;
	case 2:
		m_pagesw = str;
		break;
	case 3:
		m_sepagesw[0] = str;
		break;
	case 4:
		m_sepagesw[1] = str;
		break;
	case 5:
		m_sepagesw[2] = str;
		break;
	case 6:
		m_sepagesw[3] = str;
		break;
	default:
		break;
	}
}
void crJXJUIOperateUpdateMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		ref_ptr<crTableWidgetNode> list1 = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list1));
		ref_ptr<crTableWidgetNode> list2 = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list2));
		crMultiSwitch *pagesw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_pagesw));
		crMultiSwitch *sepagesw[4] = {NULL};
		for (int i = 0; i < 4;i++)
		{
			sepagesw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sepagesw[i]));
		}
		if (list1.valid() && list2.valid() && pagesw)
		{
			int sel1 = list1->getSelect();
			int sel2 = list2->getSelect();
			//pagesw->setActiveSwitchSet(sel1);
			if (sel1 > 0 && sel1 < 2)
			{
				sepagesw[0]->setActiveSwitchSet(sel1);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIOnlinePageUpdateMethod
//
/////////////////////////////////////////
crJXJUIOnlinePageUpdateMethod::crJXJUIOnlinePageUpdateMethod():
	m_index(0)
{
}
crJXJUIOnlinePageUpdateMethod::crJXJUIOnlinePageUpdateMethod(const crJXJUIOnlinePageUpdateMethod& handle):
	crMethod(handle),
	m_list(handle.m_list),
	m_index(handle.m_index)
{
	for (int i = 0; i < 4; ++ i)
	{
		m_title[i] = handle.m_title[i];
	}
}
void crJXJUIOnlinePageUpdateMethod::inputParam(int i, void *param)
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
void crJXJUIOnlinePageUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_list = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		m_title[i - 1] = str;
		break;
	default:
		break;
	}
}
void crJXJUIOnlinePageUpdateMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		void *param;
		double curtime;
		ref_ptr<crStaticTextWidgetNode> titleInput[4] = {NULL};
		for (char i = 0; i < 4; ++ i)
		{
			titleInput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_title[i]));
		}

		if (titleInput[0].valid())
			titleInput[0]->setString(std::string("服务器名"));
		if (titleInput[1].valid())
			titleInput[1]->setString(std::string("人数"));
		if (titleInput[2].valid())
			titleInput[2]->setString(std::string("查询时间"));
		
		ref_ptr<crTableWidgetNode> playerlist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_list));
		if (playerlist.valid())
		{
			crData *canvasdata = m_this->getDataClass();
			char tmp[64];
			time_t t = time(0);
			int i = 0;
			playerlist->clearData();
			//playerlist->addData(0,i,"服务器名");
			//playerlist->addData(1,i,"人数");
			//playerlist->addData(2,i,"查询时间");
			if (canvasdata)
			{
				canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
				++i;
				canvasdata->getParam(WCHDATA_JXJSceneSeverOnlineMap,param);
				SceneServerPlayerCountMap *playercountmap = (SceneServerPlayerCountMap *)param;
				canvasdata->getParam(WCHDATA_JXJOnlineCount,param);
				int onlinecount = *(int *)param;
				strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
				playerlist->addData(0,i,"总服务器");
				playerlist->addData(1,i,crArgumentParser::appItoa(onlinecount));
				playerlist->addData(2,i,tmp);
				for (SceneServerPlayerCountMap::iterator itr = playercountmap->begin();
					itr != playercountmap->end();
					++itr)
				{
					++i;
					//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
					playerlist->addData(0,i,itr->first);
					playerlist->addData(1,i,crArgumentParser::appItoa(itr->second));
					playerlist->addData(2,i,tmp);
				}
				canvasdata->getParam(WCHDATA_JXJServerHeartMap,param);
				crJXJOperationCanvasData::ServerHeartMap *servermap = (crJXJOperationCanvasData::ServerHeartMap *)param;
				for (crJXJOperationCanvasData::ServerHeartMap::iterator itr = servermap->begin();
					itr != servermap->end();
					++itr)
				{
					++i;
					//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&t) ); 
					curtime = crFrameStamp::getInstance()->getReferenceTime();
					playerlist->addData(0,i,itr->first);
					playerlist->addData(1,i,crArgumentParser::appItoa(curtime-itr->second));
					playerlist->addData(2,i,tmp);
				}
				canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				//////////
				//////////
				//ref_ptr<crTableIO> table = new crTableIO;
				//table->openFileNoCook("实时在线统计.tab");
				//crTableIO::StrVec strVec;
				//strVec.resize(3);
				//strVec[0] = "服务器名";
				//strVec[1] = "人数";
				//strVec[2] = "时间";
				//table->setTitleVec(strVec);
				//strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0",localtime(&t) ); 
				//strVec[0] = "总服务器";
				//strVec[1] = crArgumentParser::appItoa(onlinecount);
				//strVec[2] = tmp;
				//table->addData(strVec);
				//for (SceneServerPlayerCountMap::iterator itr = playercountmap->begin();
				//	itr != playercountmap->end();
				//	++itr)
				//{
				//	strftime( tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S\0",localtime(&t) ); 
				//	strVec[0] = itr->first;
				//	strVec[1] = crArgumentParser::appItoa(itr->second);
				//	strVec[2] = tmp;
				//	table->addData(strVec);
				//}
				//table->saveToFileNoCook("实时在线统计.tab");
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckOnlineCountMethod
//
/////////////////////////////////////////
crJXJCheckOnlineCountMethod::crJXJCheckOnlineCountMethod():
	m_this(NULL)
{
}
crJXJCheckOnlineCountMethod::crJXJCheckOnlineCountMethod(const crJXJCheckOnlineCountMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckOnlineCountMethod::inputParam(int i, void *param)
{
}
void crJXJCheckOnlineCountMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckOnlineCountMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckOnlineCount,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvCheckOnlineCountMethod
//
/////////////////////////////////////////
crJXJRecvCheckOnlineCountMethod::crJXJRecvCheckOnlineCountMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckOnlineCountMethod::crJXJRecvCheckOnlineCountMethod(const crJXJRecvCheckOnlineCountMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckOnlineCountMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckOnlineCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckOnlineCountMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)

		{
			void *param;
			int playerid = m_this->getPlayerID();
			int mapsize = 0;
			int sceneid = 0;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();

			ref_ptr<crGameServerPlayerData>playerdata;
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
			SceneServerPlayerCountMap countmap;
			SceneServerPlayerCountMap::iterator countmapitr;
			std::string scenename;
			int weicount = 0;
			int shucount = 0;
			int wucount = 0;
			unsigned char shiliid = 0;
			crData *data = NULL;
			netDataManager->lockPlayerDataMap();
			crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			if (!playerDataMap.empty())
			{
				mapsize = playerDataMap.size();
			}
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				playerdata = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (playerdata.valid())
				{
					sceneid = playerdata->getSceneID();
					if (sceneid > 0)
					{
						scenename = callback->findSceneServerName(sceneid);
						countmapitr = countmap.find(scenename);
						if (countmapitr != countmap.end())
						{
							countmapitr->second += 1;
						}
						else
						{
							countmap.insert(std::make_pair(scenename,1));
						}
					}
					data = playerdata->getPlayerGameData()->getDataClass();
					data->getParam(WCHDATA_JXJShiliID,param);
					shiliid = *(unsigned char *)param;
					switch (shiliid - c_startShiliID)
					{
					case 0:
						++weicount;
						break;
					case 1:
						++shucount;
						break;
					case 2:
						++wucount;
						break;
					default:
						break;
					}
				}
			}

			countmap.insert(std::make_pair("1魏",weicount));
			countmap.insert(std::make_pair("2蜀",shucount));
			countmap.insert(std::make_pair("3吴",wucount));
			netDataManager->unLockPlayerDataMap();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				int scenemapsize = countmap.size();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(8 + 24 * scenemapsize);
				stream->_writeInt(mapsize);
				stream->_writeInt(scenemapsize);
				for (SceneServerPlayerCountMap::iterator itr = countmap.begin();
					itr != countmap.end();
					++itr)
				{
					stream->_writeString(itr->first);
					stream->_writeInt(itr->second);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCheckOnlineCount,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			int onlinecount = m_stream->_readInt();
			int mapsize = m_stream->_readInt();
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_Operation");
			if (canvasNode.valid())
			{
				std::string servername;
				int playercount = 0;
				crData *canvasdata = canvasNode->getDataClass();
				if (canvasdata)
				{
					canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
					canvasdata->getParam(WCHDATA_JXJSceneSeverOnlineMap,param);
					SceneServerPlayerCountMap *countmap = (SceneServerPlayerCountMap *)param;
					countmap->clear();
					for (int i = 0; i < mapsize; ++i)
					{
						servername = m_stream->_readString();
						playercount = m_stream->_readInt();
						countmap->insert(std::make_pair(servername,playercount));
					}
					canvasdata->inputParam(WCHDATA_JXJOnlineCount,&onlinecount);
					canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
				}
				//canvasNode->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
///////////////////////////////////////////
////
////crJXJCheckOnlineCountMethod压测数据测后删除
////
///////////////////////////////////////////
//crJXJTestInitMethod::crJXJTestInitMethod():
//	m_this(NULL)
//{
//}
//crJXJTestInitMethod::crJXJTestInitMethod(const crJXJTestInitMethod& handle):
//	crMethod(handle),
//	m_this(NULL)
//{
//}
//void crJXJTestInitMethod::inputParam(int i, void *param)
//{
//}
//void crJXJTestInitMethod::addParam(int i, const std::string& str)
//{
//}
//void crJXJTestInitMethod::operator()(crHandle &handle)
//{
//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//	if(myPlayer && netConductor)
//	{
//		crPlayerDataEventPacket packet;
//		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvTestInit,NULL);
//		netConductor->getNetManager()->sendPacket("all",packet);
//	}
//}
///////////////////////////////////////////
////
////crJXJRecvTestInitMethod压测数据测后删除
////
///////////////////////////////////////////
//crJXJRecvTestInitMethod::crJXJRecvTestInitMethod():
//	m_netType(GameClient_Game){}
//crJXJRecvTestInitMethod::crJXJRecvTestInitMethod(const crJXJRecvTestInitMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJRecvTestInitMethod::inputParam(int i, void *param)
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
//void crJXJRecvTestInitMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crJXJRecvTestInitMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			void *param;
//			int playerid = m_this->getPlayerID();
//			crData *data = m_this->getDataClass();
//			int mapsize = 0;
//			int sceneid = 0;
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			crNetDataManager *netDataManager = gameServer->getNetDataManager();
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			data->getParam(WCHDATA_JXJTroopsMap,param);
//			TroopsMap *troops = (TroopsMap *)param;
//			troops->insert(std::make_pair(20015,19999));
//			troops->insert(std::make_pair(20021,19999));
//			troops->insert(std::make_pair(20004,19999));
//			if (playerData.valid())
//			{
//				crPlayerDataEventPacket packet;
//				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvTestInit,NULL);
//				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//			}
//
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			void *param;
//			crData *data = m_this->getDataClass();
//			data->getParam(WCHDATA_JXJTroopsMap,param);
//			TroopsMap *troops = (TroopsMap *)param;
//			troops->insert(std::make_pair(20015,19999));
//			troops->insert(std::make_pair(20021,19999));
//			troops->insert(std::make_pair(20004,19999));
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJCheckEditBoxInputCountMethod
//
/////////////////////////////////////////
crJXJCheckEditBoxInputCountMethod::crJXJCheckEditBoxInputCountMethod():
	m_this(NULL),
	m_inputmaxcount(0)
{
}
crJXJCheckEditBoxInputCountMethod::crJXJCheckEditBoxInputCountMethod(const crJXJCheckEditBoxInputCountMethod& handle):
	crMethod(handle),
	m_inputmaxcount(handle.m_inputmaxcount)
{
}
void crJXJCheckEditBoxInputCountMethod::inputParam(int i, void *param)
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
		m_this = (crEditWidgetNode*)param;
		break;
	}
}
void crJXJCheckEditBoxInputCountMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_inputmaxcount = atoi(str.c_str());
		break;
	}
}
void crJXJCheckEditBoxInputCountMethod::operator()(crHandle &handle)
{	
	std::string str = m_this->getUTF8String();
	if (str.size() <= m_inputmaxcount)
	{
		m_lastinput = str;
	}
	m_this->setString(m_lastinput);
}
/////////////////////////////////////////
//
//crJXJRecvCheckPlayerDataMethod
//
/////////////////////////////////////////
crJXJRecvCheckPlayerDataMethod::crJXJRecvCheckPlayerDataMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckPlayerDataMethod::crJXJRecvCheckPlayerDataMethod(const crJXJRecvCheckPlayerDataMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckPlayerDataMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckPlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckPlayerDataMethod::operator()(crHandle &handle)
{
	if(m_this/* && m_stream.valid()*/)
	{
		//if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			int mapsize = 0;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			//crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			//if (!playerDataMap.empty())
			//{
			//	mapsize = playerDataMap.size();
			//}
			ref_ptr<crGameServerPlayerData>playerdata;
			//crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
			int _playerid = 0;
			std::string _account;//帐号
			std::string _nickname;//昵称
			std::string _createaccount;//注册时间
			std::string _createdate;//创建角色时间
			std::string _logindate;//最后登陆时间
			std::string _onlinetime;//在线时间
			std::string _email;//Email地址
			//unsigned char _shiliid = 0;//势力ID
			//int _gold = 0;//元宝
			//int _lijin = 0;//礼金
			//unsigned char _buildingtimebarcount = 0;//建筑时间条数
			//unsigned char _recuittimebarcount = 0;//招募时间条
			//unsigned char _lordlv = 0;//领主等级
			//unsigned char _govpost = 0;//官职
			//int _groupid = 0;//军团ID
			//int _exploit = 0;//功勋
			//unsigned char _junling = 0;//军令
			//int _achieve = 0;//战功
			//int _countrycontri = 0;//国家贡献
			//int _copper = 0;//铜钱
			//int _food = 0;//粮食
			//int _wood = 0;//木材
			//int _iron = 0;//铁矿
			//int _horse = 0;//马匹
			//TechLvVec _techlvvec;//科技
			//BuildingLvVec _buildinglvvec;//建筑等级
			//FubenProgressMap _fubenprogressmap;//副本map
			//JXJItemBagVec _itembagvec;//背包
			//JXJItemBagVec2 _lordequipvec;//领主装备
			//JXJItemBagVec _jiangkabagvec;//将卡背包
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
			gsBrainData->getParam(WCHDATA_JXJCheckPlayerDataMap,param);
			PlayerMap *playermap = (PlayerMap *)param;
			gsBrainData->inputParam(WCHDATA_JXJCheckPlayerDataPlayerID,&playerid);
			//PlayerMap playermap;
			//crVector4i buildingtimes;
			//crVector4i recruittimes;
			//从数据库获取该数据
			//
			//////
			if (playermap->empty())
			{
				ref_ptr<crPlayerDataQuery> newplayerdata = NULL;
				crDataBase *global_db = crDataBaseManager::getInstance()->getAccountDB();//->getGlobalDB();
				//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
				ref_ptr<crDataBase> db_session = global_db->beginSession();
				ref_ptr<crQueryPlayerData> playerdataquery = new crQueryPlayerData;
				playerdataquery->buildQuerySql();
				db_session->executeQuery(playerdataquery.get());
				crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
				for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
					itr != playerdataQueryResultVec.end();
					++itr)
				{
					playerdataquery = dynamic_cast<crQueryPlayerData *>((*itr).get());
					if (playerdataquery.valid())
					{
						newplayerdata = new crPlayerDataQuery;
						_playerid = playerdataquery->getID();
						_account = playerdataquery->getAccountName();
						_createaccount = playerdataquery->getCreateDate();
						_logindate = playerdataquery->getLoginDate();
						_onlinetime = playerdataquery->getOnlinetime();
						_email = playerdataquery->getEmail();
						newplayerdata -> setPlayerID(_playerid);
						newplayerdata -> setAccountName(_account);
						newplayerdata -> setCreateDate(_createaccount);
						newplayerdata -> setLoginDate(_logindate);
						newplayerdata -> setOnlinetime(_onlinetime);
						newplayerdata -> setEmail(_email);
						playermap->insert(std::make_pair(_playerid,newplayerdata));
					}
				}
				db_session->releaseQuery();
				global_db->endSession(db_session.get());
			}
			/////
			gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));

		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckPlayerDataDisposeMethod
//
/////////////////////////////////////////
crJXJCheckPlayerDataDisposeMethod::crJXJCheckPlayerDataDisposeMethod():
	m_this(NULL),
	m_taskcount(5){}
crJXJCheckPlayerDataDisposeMethod::crJXJCheckPlayerDataDisposeMethod(const crJXJCheckPlayerDataDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_taskcount(handle.m_taskcount)
{
}
void crJXJCheckPlayerDataDisposeMethod::inputParam(int i, void *param)
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

void crJXJCheckPlayerDataDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_taskcount = atoi(str.c_str());
		break;
	}
}

void crJXJCheckPlayerDataDisposeMethod::operator()(crHandle &handle)
{
	//////////////////////////
	//返回					//
	//		5条玩家数据		//
	//////////////////////////
	m_taskVec.resize(0);
	m_taskVec.reserve(m_taskcount);
	bool ended = false;
	std::pair<int,CRCore::ref_ptr<crPlayerDataQuery> > taskpair;
	void *param;
	crData *gsBrainData = m_this->getDataClass();
	gsBrainData->getParam(WCHDATA_JXJCheckPlayerDataPlayerID,param);
	int playerid = *( int *)param;
	if(playerid<=0)
		return;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> myplayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(!myplayerData.valid())
	{
		return;
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
	gsBrainData->getParam(WCHDATA_JXJCheckPlayerDataMap,param);
	PlayerMap *playermap = (PlayerMap *)param;
	int i = 0;
	for( PlayerMap::iterator itr = playermap->begin();
		 itr != playermap->end() && i < m_taskcount;
		 i++ )
	{
		taskpair = *itr;
		itr = playermap->erase(itr);
		m_taskVec.push_back(taskpair);
		if(itr == playermap->end())
			ended = true;
	}
	gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
	if(!m_taskVec.empty())
	{
		ref_ptr<crGameServerPlayerData>playerdata;
		//crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
		int _playerid = 0;
		std::string _account;//帐号
		std::string _nickname;//昵称
		std::string _createaccount;//注册时间
		std::string _createdate;//创建角色时间
		std::string _logindate;//最后登陆时间
		std::string _onlinetime;//在线时长
		std::string _email;//Email地址
		unsigned short _serieslanddays;//连续登录天数
		unsigned char _shiliid = 0;//势力ID
		int _gold = 0;//元宝
		int _lijin = 0;//礼金
		unsigned char _buildingtimebarcount = 0;//建筑时间条数
		unsigned char _recuittimebarcount = 0;//招募时间条
		unsigned char _lordlv = 0;//领主等级
		unsigned char _govpost = 0;//官职
		int _groupid = 0;//军团ID
		int _exploit = 0;//功勋
		unsigned short _junling = 0;//军令
		int _achieve = 0;//战功
		int _countrycontri = 0;//国家贡献
		int _copper = 0;//铜钱
		int _food = 0;//粮食
		int _wood = 0;//木材
		int _iron = 0;//铁矿
		int _horse = 0;//马匹
		TechLvVec _techlvvec;//科技
		BuildingLvVec _buildinglvvec;//建筑等级
		FubenProgressMap _fubenprogressmap;//副本map
		JXJItemBagVec _itembagvec;//背包
		JXJItemBagVec2 _lordequipvec;//领主装备
		JXJItemBagVec _jiangkabagvec;//将卡背包
		crVector4i buildingtimes;
		crVector4i recruittimes;
		crData *_data = NULL;
		crData *_mainroledata = NULL;

		crJXJSendPlayerDataPacket packet;
		ref_ptr<crGameServerPlayerData> playerData = NULL;
		ref_ptr<crStreamBuf> stream = NULL;
		for( std::vector< std::pair<int,CRCore::ref_ptr<crPlayerDataQuery> > >::iterator titr = m_taskVec.begin();
			titr != m_taskVec.end();
			++titr )
		{
			playerData = getPlayerData(titr->first);//dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(_playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getMainRole())
			{
				stream = new crStreamBuf;
				stream->createBuf(2048);
				//if (playermap->size()<=1)
				//{
				//	stream->_writeUChar(0);
				//}
				//else
				//{
				//	stream->_writeUChar(1);
				//}
				//stream->_writeInt(mapsize);
				_buildingtimebarcount = 0;
				_recuittimebarcount = 0;
				_data = playerData->getPlayerGameData()->getDataClass();
				_mainroledata = playerData->getPlayerGameData()->getMainRole()->getDataClass();
				_playerid = titr->first;
				_account = titr->second->getAccountName();
				_nickname = playerData->getCharacterName();
				_createaccount = titr->second->getCreateDate();
				_createdate = "";
				_logindate = titr->second->getLoginDate();
				_onlinetime = titr->second->getOnlinetime();
				_email = titr->second->getEmail();
				_data->getParam(WCHDATA_JXJSeriesLandDay,param);
				_serieslanddays = *( unsigned short *)param;
				_data->getParam(WCHDATA_JXJShiliID,param);
				_shiliid = *( unsigned char *)param;
				_data->getParam(WCHDATA_Money,param);
				_gold = *( int *)param;
				_data->getParam(WCHDATA_JXJGiftGold,param);
				_lijin = *( int *)param;
				_data->getParam(WCHDATA_JXJBuildingTimes,param);
				buildingtimes = *( crVector4i *)param;
				_data->getParam(WCHDATA_JXJRecruitTimes,param);
				recruittimes = *( crVector4i *)param;
				for (int i = 0; i < 4; i++)
				{
					if (buildingtimes[i] >= 0)
					{
						_buildingtimebarcount++;
					}
					if (recruittimes[i] >= 0)
					{
						_recuittimebarcount++;
					}
				}
				_mainroledata->getParam(WCHDATA_Level,param);
				_lordlv = *( unsigned char *)param;
				_mainroledata->getParam(WCHDATA_JXJGovPost,param);
				_govpost = *( unsigned char *)param;
				_data->getParam(WCHDATA_JXJConsortiaID,param);
				_groupid = *( int *)param;
				_mainroledata->getParam(WCHDATA_JXJExploit,param);
				_exploit = *( int *)param;
				_data->getParam(WCHDATA_JXJJunling,param);
				_junling = *( unsigned short *)param;
				_mainroledata->getParam(WCHDATA_JXJAchievement,param);
				_achieve = *( int *)param;
				_data->getParam(WCHDATA_JXJCountryContribute,param);
				_countrycontri = *( int *)param;
				_data->getParam(WCHDATA_JXJCoppercash,param);
				_copper = *( int *)param;
				_data->getParam(WCHDATA_JXJFood,param);
				_food = *( int *)param;
				_data->getParam(WCHDATA_JXJWood,param);
				_wood = *( int *)param;
				_data->getParam(WCHDATA_JXJIron,param);
				_iron = *( int *)param;
				_data->getParam(WCHDATA_JXJHorse,param);
				_horse = *( int *)param;
				stream->_writeInt(_playerid);
				stream->_writeString(_account);
				stream->_writeString(_nickname);
				stream->_writeString(_createaccount);
				stream->_writeString(_createdate);
				stream->_writeString(_logindate);
				stream->_writeString(_onlinetime);
				stream->_writeString(_email);
				stream->_writeUShort(_serieslanddays);
				stream->_writeUChar(_shiliid);
				stream->_writeInt(_gold);
				stream->_writeInt(_lijin);
				stream->_writeUChar(_buildingtimebarcount);
				stream->_writeUChar(_recuittimebarcount);
				stream->_writeUChar(_lordlv);
				stream->_writeUChar(_govpost);
				stream->_writeInt(_groupid);
				stream->_writeInt(_exploit);
				stream->_writeUShort(_junling);
				stream->_writeInt(_achieve);
				stream->_writeInt(_countrycontri);
				stream->_writeInt(_copper);
				stream->_writeInt(_food);
				stream->_writeInt(_wood);
				stream->_writeInt(_iron);
				stream->_writeInt(_horse);
				crJXJSendPlayerDataPacket::buildReplyPacket(packet,playerid,stream.get());
				gameServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet);
			}
		}
		if(ended)
		{
			crJXJEndSendPlayerDataPacket epacket;
			crJXJEndSendPlayerDataPacket::buildReplyPacket(epacket,playerid,NULL);
			gameServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),epacket);
		}
	}
}
CRCore::ref_ptr<crGameServerPlayerData> crJXJCheckPlayerDataDisposeMethod::getPlayerData(int playerid)
{
	ref_ptr<crGameServerPlayerData> playerData = NULL;
	playerData = new crGameServerPlayerData;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	playerData->setPlayerID(playerid);
	unsigned short gamedbid = 1;
	bool found = false;
	int money = 0;
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crPlayerGameDBQueryData> playergamedbQuery = new crPlayerGameDBQueryData;
	playergamedbQuery->buildQuerySql(playerid,netCallback->getServerID());

	globalSession->executeQuery(playergamedbQuery.get());
	crDataBase::QueryResultVec &playergamedbQueryResultVec = globalSession->getQueryResult();
	if(!playergamedbQueryResultVec.empty())
	{
		playergamedbQuery = dynamic_cast<crPlayerGameDBQueryData *>(playergamedbQueryResultVec[0].get());
		gamedbid = playergamedbQuery->getGameDBID();
		found = true;
	}
	globalSession->releaseQuery();

	if(found)
	{
		ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
		playerMoneyQuery->buildQuerySql(playerid,netCallback->getServerID());

		globalSession->executeQuery(playerMoneyQuery.get());
		crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
		if(!playerMoneyQueryResultVec.empty())
		{
			playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
			money = playerMoneyQuery->getMoney();
		}
		globalSession->releaseQuery();
	}
	db->endSession(globalSession.get());
	//加载playergamedata
	if(found)
	{
		playerData->setGameDBID(gamedbid);
		////////playerGameData查询
		ref_ptr<crPlayerGameData> playerGameData;
		std::string script = netCallback->getGameName() + "/playergamedata.cfg";
		ref_ptr<crData> data = CREncapsulation::loadData(script);
		if(data.valid())
		{
			playerGameData = new crPlayerGameData;
			playerGameData->setPlayerID(playerid);
			playerGameData->setDataClass(data.get());
			data->inputParam(WCHDATA_Money,&money);
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,playerData->getGameDBID());
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crPlayerGameDataQueryData> playerGameDataQuery = new crPlayerGameDataQueryData;
				playerGameDataQuery->buildQuerySql(playerid);

				gameSession->executeQuery(playerGameDataQuery.get());
				crDataBase::QueryResultVec &playerGameDataQueryResultVec = gameSession->getQueryResult();
				if(!playerGameDataQueryResultVec.empty())
				{
					playerGameDataQuery = dynamic_cast<crPlayerGameDataQueryData *>(playerGameDataQueryResultVec[0].get());
					playerGameData->setID(playerGameDataQuery->getID());
					playerGameData->setStream(playerGameDataQuery->getStream());
					int roleid = playerGameDataQuery->getMainRoleID();
					if(roleid > 0)
					{//查询并设置mainrole
						playerGameData->setMainRoleID(roleid);
					}
					playerGameData->setDBValid(true);
				}
				gameSession->releaseQuery();
				gamedb->endSession(gameSession.get());
				if(playerGameData->getStream())
				{
					data->inputParam(WCHDATA_DataStream,playerGameData->getStream());
					playerGameData->setStream(NULL);
				}
				playerData->setPlayerGameData(playerGameData.get());
			}
			else
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = "<<playerData->getGameDBID()<<std::endl;
				//sprintf(gDebugInfo->buf(),"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = %d\n\0",playerData->getGameDBID());
				//gDebugInfo->debugInfo(CRCore::ALWAYS);
			}
		}
	}
	////加载mainrole
	if(playerData.valid())
	{
		unsigned short gamedbid = playerData->getGameDBID();
		bool isMainRole = true;
		crPlayerGameData *playerGameData = playerData->getPlayerGameData();
		if(playerGameData)
		{
			int roleid = playerGameData->getMainRoleID();
			ref_ptr<crRole> role;
			CRCore::ref_ptr<crItemChild> itemChild;
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				bool game_ended = false;
				ref_ptr<crRoleQueryData> roleQuery = new crRoleQueryData;
				roleQuery->buildQuerySql(roleid);
				gameSession->executeQuery(roleQuery.get());
				crDataBase::QueryResultVec &roleQueryResultVec = gameSession->getQueryResult();
				int ametierid = 0;
				if(!roleQueryResultVec.empty())
				{
					role = new crRole;
					roleQuery = dynamic_cast<crRoleQueryData*>(roleQueryResultVec[0].get());
					role->setRoleID(roleid);
					role->setPlayerID(playerid);
					role->setInstanceItemID(roleQuery->getInstanceItemID());
					ametierid = roleQuery->getAbstractMetierID();
					role->setMetierStream(roleQuery->getMetierStream());
				}
				gameSession->releaseQuery();
				//gamedb->endSession(gameSession.get());

				if(role.valid())
				{
					crAbstractMetier *aMetier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(ametierid);
					role->setAbstractMetier(aMetier);

					if(role->getInstanceItemID()>0)
					{
						ref_ptr<crInstanceItemQueryData> instanceItemQuery = new crInstanceItemQueryData;
						instanceItemQuery->buildQuerySql(role->getInstanceItemID());
						gameSession->executeQuery(instanceItemQuery.get());
						crDataBase::QueryResultVec &instanceItemQueryResultVec = gameSession->getQueryResult();
						int abstractid = 0;
						if(!instanceItemQueryResultVec.empty())
						{
							instanceItemQuery = dynamic_cast<crInstanceItemQueryData*>(instanceItemQueryResultVec[0].get());
							abstractid = instanceItemQuery->getAbstractItemID();
							//role->setAbstractItemID();
							role->setItemtype(instanceItemQuery->getItemtype());
							role->setIName(instanceItemQuery->getIName());
							role->setSceneID(instanceItemQuery->getSceneID());
							role->setLayerID(instanceItemQuery->getLayerid());
							role->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
							role->setZoffset(instanceItemQuery->getZoffset());
							role->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
							role->setStream(instanceItemQuery->getStream());
						}
						gameSession->releaseQuery();

						//if(cAbstractID!=0)
						//	abstractid = cAbstractID;
						crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
						role->setAbstractItem(aitem);

						////ItemChild查询
						//ref_ptr<crItemChildQueryData> itemChildQuery = new crItemChildQueryData;
						//itemChildQuery->buildQuerySql(role->getInstanceItemID());
						//gameSession->executeQuery(itemChildQuery.get());
						//crDataBase::QueryResultVec &itemChildQueryResultVec = gameSession->getQueryResult();
						//if(!itemChildQueryResultVec.empty())
						//{
						//	for( crDataBase::QueryResultVec::iterator citr = itemChildQueryResultVec.begin();
						//		citr != itemChildQueryResultVec.end();
						//		++citr )
						//	{
						//		itemChildQuery = dynamic_cast<crItemChildQueryData *>(citr->get());
						//		itemChild = new crItemChild;
						//		itemChild->setItemChildID(itemChildQuery->getID());
						//		itemChild->setInstanceItemID(itemChildQuery->getInstanceitemid());
						//		itemChild->setParentItem(role.get());
						//		role->insertChildItem(itemChild.get());
						//	}
						//}
						//gameSession->releaseQuery();

						////ChildItem查询
						//role->childItemQuery(gameSession.get());
					}
					gamedb->endSession(gameSession.get());
					game_ended = true;
					if(isMainRole)
					{
						playerGameData->setMainRole(role.get());
						role->setSceneID(0);
						role->setRoomID(0);
						role->loadMetierData(SaveStream);
						role->loadItemData(SaveStream,true);
						//role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
						//role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
					}
				}
				if(!game_ended) gamedb->endSession(gameSession.get());
			}
		}
	}
	if (!found)
	{
		return NULL;
	}
	return playerData.get();
}
/////////////////////////////////////////
//
//crJXJCheckOnlineCountMethod
//
/////////////////////////////////////////
crJXJShowGroupMenuMethod::crJXJShowGroupMenuMethod():
	m_this(NULL),
	m_ea(NULL)
{
}
crJXJShowGroupMenuMethod::crJXJShowGroupMenuMethod(const crJXJShowGroupMenuMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_ea(NULL),
	m_canvas(handle.m_canvas),
	m_memberlist(handle.m_memberlist)
{
}
void crJXJShowGroupMenuMethod::inputParam(int i, void *param)
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
void crJXJShowGroupMenuMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_memberlist = str;
		break;
	case 2:
		m_addfriend = str;
		break;
	case 3:
		m_deletefriend = str;
		break;
	case 4:
		m_kickmember = str;
		break;
	}
}
void crJXJShowGroupMenuMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>menucanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if (myPlayer && menucanvas.valid() && canvas.valid())
	{
		crData *data = myPlayer->getDataClass();
		bool ifshow = false;
		crData *brain = crBrain::getInstance()->getDataClass();
		ref_ptr<crTableWidgetNode> memberlist = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_memberlist));
		int selplayerid = memberlist->getSelectData();
		if (myPlayer->getPlayerID() != selplayerid)
		{
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJMyGroupMemberVec,param);
			ConsortiaMemberVec *memberVec = (ConsortiaMemberVec*)param;
			for (ConsortiaMemberVec::iterator itr = memberVec->begin();
				itr != memberVec->end();
				++itr)
			{
				if (selplayerid == (*itr)->getPlayerID())
				{
					brain->inputParam(WCHDATA_JXJAddFriendName,& (*itr)->getName());
					canvasdata -> inputParam(WCHDATA_JXJMyGroupSelPlayData,(*itr).get());
					ifshow = true;
					break;
				}
			}
			if (ifshow)
			{
				ref_ptr<crButtonWidgetNode> addfriend = dynamic_cast<crButtonWidgetNode *>(menucanvas->getWidget(m_addfriend));
				ref_ptr<crButtonWidgetNode> deletefriend = dynamic_cast<crButtonWidgetNode *>(menucanvas->getWidget(m_deletefriend));
				ref_ptr<crButtonWidgetNode> kickmember = dynamic_cast<crButtonWidgetNode *>(menucanvas->getWidget(m_kickmember));
				data->getParam(WCHDATA_JXJFriendList,param);
				FriendVec *friendVec = (FriendVec *)param;
				FriendVec::iterator fitr;
				for (fitr = friendVec->begin();
					fitr != friendVec->end();
					++fitr)
				{
					if ((*fitr)->getPlayerID() == selplayerid)
					{
						if (addfriend.valid())
						{
							addfriend->setEnable(false);
						}
						if (deletefriend.valid())
						{
							deletefriend->setEnable(true);
						}
						break;
					}
				}
				if (fitr == friendVec->end())
				{
					if (addfriend.valid())
					{
						addfriend->setEnable(true);
					}
					if (deletefriend.valid())
					{
						deletefriend->setEnable(false);
					}
				}
				std::string myname = myPlayer->getCharacterName();
				canvasdata->getParam(WCHDATA_JXJMyGroupInfo,param);
				crJXJGroupInfo *groupinfo = (crJXJGroupInfo *)param;
				if (kickmember.valid())
				{
					if(myname.compare(groupinfo->getCreatorName()) == 0)
					{
						kickmember->setEnable(true);
					}
					else
					{
						kickmember->setEnable(false);
					}
				}
				////////////////////////////////////////////////////////
				const crBoundingBox &tips = menucanvas->getBoundBox();
				float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
				float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
				crVector2 mouse(m_ea->getXnormalized()+posx,m_ea->getYnormalized()-posy);
				/////////////////////////////////////////
				if ((mouse[0] + (posx * 2))>=1.5)
				{
					mouse[0]-=posx * 2;
				}
				if ((mouse[1] - (posy * 2))<=-1.5)
				{
					mouse[1]+=posy * 2;
				}
				/////////////////////////////////////////
				menucanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
				//canvas->setCanFocus(true);
				//menucanvas->setCanCaptureMouse(true);
				crFilterRenderManager::getInstance()->setMouseCapturer(menucanvas.get());
				//if (menucanvas->getVisiable())
				//{
				//	crFilterRenderManager::getInstance()->showCanvas(menucanvas,false);
				//}
				//else
				//{
				crFilterRenderManager::getInstance()->showCanvas(menucanvas.get(),true);
				//}
				//crFilterRenderManager::getInstance()->setMouseCapturer(NULL);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJFormationGetLeaderPowerMethod
//
/////////////////////////////////////////
crJXJFormationGetLeaderPowerMethod::crJXJFormationGetLeaderPowerMethod():
	m_this(NULL),
	m_output(0)
{
}

crJXJFormationGetLeaderPowerMethod::crJXJFormationGetLeaderPowerMethod(const crJXJFormationGetLeaderPowerMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_output(0)
{
}

void crJXJFormationGetLeaderPowerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_output = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_output = (int *)(LOINT64(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}

void crJXJFormationGetLeaderPowerMethod::addParam(int i, const std::string& str)
{
}

void crJXJFormationGetLeaderPowerMethod::operator()(crHandle &handle)
{
	void *param;
	int poweradd = 0;
	*m_output = 0;
	crJXJShili *shili = NULL;
	crData *data = m_this->getDataClass();
	crRole* mainrole = m_this->getMainRole();
	if (NULL == mainrole) return;
	crData *mainroledata = mainrole->getDataClass();
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char viplv = *(unsigned char *)param;
	data->getParam(WCHDATA_JXJShiliID,param);
	unsigned char shiliid = *(unsigned char *)param;
	crTableIO::StrVec	record,recordpost;
	ref_ptr<crTableIO>postTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJGovPostTab);
	int leadmaxindex = postTab->getTitleIndex("领导力");
	mainroledata->getParam(WCHDATA_JXJGovPost, param);
	if(leadmaxindex > 0 && postTab->queryOneRecord(0, crArgumentParser::appItoa(*((unsigned char*)param)), recordpost) >= 0)
	{
		*m_output += atoi(recordpost[leadmaxindex].c_str());
	}
	if (crGlobalHandle::isClient())
	{
		//crData *braindata = crBrain::getInstance()->getDataClass();
		//braindata->getParam(WCHDATA_JXJMyCountryData,param);
		data->getParam(WCHDATA_JXJMyCountryData,param);
		shili = (crJXJShili *)param;
	}
	else
	{
		crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
		gsBrainData->getParam(WCHDATA_JXJShiliWei + shiliid - c_startShiliID, param);
		shili = (crJXJShili *)param;
	}
	if (shili)
	{
		crData *shilidata = shili->getDataClass();
		shilidata->getParam(WCHDATA_JXJChengxiang, param);
		int kingplayerid = *(int *)param;
		shilidata->getParam(WCHDATA_JXJDasima, param);
		int dasimaId = *(int *)param;
		shilidata->getParam(WCHDATA_JXJDajiangjun, param);
		int dajiangjunId = *(int *)param;
		int playerid = m_this->getPlayerID();
		if (kingplayerid == playerid)
		{
			poweradd = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJKingLeaderPowerAdd,viplv).c_str());
		}
		else if (dasimaId == playerid)
		{
			poweradd = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSecondLeaderPowerAdd,viplv).c_str());
		}
		else if (dajiangjunId == playerid)
		{
			poweradd = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJThirdLeaderPowerAdd	,viplv).c_str());
		}
	}
	int permaxlead = 0;
	float totletongshuai = 0.0f;
	crRole *role = m_this->getMainRole();
	if (role)
	{
		crVector3f factor;
		crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTongShuaiFactor,viplv),factor);
		role->doEvent(WCH_JXJGetTongShuai,MAKEINT64(&totletongshuai,NULL));
		permaxlead = totletongshuai * factor[0] + factor[1];
		poweradd += permaxlead;
	}
	*m_output += poweradd;
}
/////////////////////////////////////////
//
//crJXJChangePlayerDataMethod
//
/////////////////////////////////////////
crJXJChangePlayerDataMethod::crJXJChangePlayerDataMethod():
	m_this(NULL),
	m_needChange(0),
	m_type(0)
{
}

crJXJChangePlayerDataMethod::crJXJChangePlayerDataMethod(const crJXJChangePlayerDataMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_needChange(0),
	m_type(0)
{
}

void crJXJChangePlayerDataMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_needChange = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_needChange = (int *)(LOINT64(param64));
			m_type = (unsigned char )(HIINT64(param64));
		}
		else
		{
			m_needChange = NULL;
			m_type = NULL;
		}
		break;
	}
}

void crJXJChangePlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJChangePlayerDataMethod::operator()(crHandle &handle)
{
	bool success = false;
	if (*m_needChange == 0)
	{
		success = true;
	}
	if(m_this && !success)
	{	
		// 0：扣除成功|增加成功
		//-1：扣除失败
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData.valid())
		{
			crData *data = m_this->getDataClass();
			void *param;
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int *curdata = 0;
			unsigned short *curdata2 = 0;
			int maxnum = 0;
			switch(m_type)
			{
			case T_CardImpression:
				data->getParam(WCHDATA_JXJCardImpression,param);
				curdata2 = (unsigned short *)param;
				maxnum = INT_MAX;
				break;
			case T_JiangHun:
				data->getParam(WCHDATA_JXJCardPieces,param);
				curdata = (int *)param;
				maxnum = INT_MAX;
				break;
			}
			//扣除好感度|增加好感度
			if (*m_needChange >= 0)
			{
				switch (m_type)
				{
				case T_CardImpression:
					INTLIMITADD(*curdata2,*m_needChange,maxnum);
					break;
				default:
					INTLIMITADD(*curdata,*m_needChange,maxnum);
					success = true;
					break;
				}
			}
			else
			{
				switch (m_type)
				{
				case T_CardImpression:
					if (*curdata2 >= -*m_needChange)
					{
						INTLIMITADD(*curdata2,*m_needChange,maxnum);
						success = true;
					}
					break;
				default:
					if (*curdata >= -*m_needChange)
					{
						INTLIMITADD(*curdata,*m_needChange,maxnum);
						success = true;
					}
					break;
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));

			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			switch (m_type)
			{
			case T_CardImpression:
				stream->createBuf(3);
				stream->_writeUChar(m_type);
				stream->_writeUShort(*curdata2);
				break;
			default:
				stream->createBuf(5);
				stream->_writeUChar(m_type);
				stream->_writeInt(*curdata);
				break;
			}
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvChangePlayerData,stream.get());
			if (playerData.valid())
			{
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}
		}
	}
	if (success)
	{
		*m_needChange = 0;
	}
	else
	{
		*m_needChange = -1;
	}
}
/////////////////////////////////////////
//
//crJXJCheckPlayerDataMethod
//
/////////////////////////////////////////
crJXJCheckPlayerDataMethod::crJXJCheckPlayerDataMethod():
	m_this(NULL),
	m_needdeduct(0),
	m_type(0)
{
}

crJXJCheckPlayerDataMethod::crJXJCheckPlayerDataMethod(const crJXJCheckPlayerDataMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_needdeduct(0),
	m_type(0)
{
}

void crJXJCheckPlayerDataMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_needdeduct = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_needdeduct = (int *)(LOINT64(param64));
			m_type = (unsigned char)(HIINT64(param64));
		}
		else
		{
			m_needdeduct = NULL;
			m_type = NULL;
		}
		break;
	}
}

void crJXJCheckPlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJCheckPlayerDataMethod::operator()(crHandle &handle)
{
	bool success = false;
	if (m_needdeduct <= 0)
	{
		success = true;
	}
	if(m_this && m_needdeduct>0)
	{	
		// 0：检测成功
		//-1：检测失败
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			int curdata = 0;
			switch(m_type)
			{
			case T_CardImpression:
				data->getParam(WCHDATA_JXJCardImpression,param);
				curdata = *(unsigned short *)param;
				break;
			case T_JiangHun:
				data->getParam(WCHDATA_JXJCardPieces,param);
				curdata = *(int *)param;
				break;
			}
			//检测好感度
			if (curdata >= *m_needdeduct)
			{
				success = true;
			}
		}
	}
	if (success)
	{
		*m_needdeduct = 0;
	}
	else
	{
		*m_needdeduct = -1;
	}
}
/////////////////////////////////////////
//
//crJXJRecvChangePlayerDataMethod
//
/////////////////////////////////////////
crJXJRecvChangePlayerDataMethod::crJXJRecvChangePlayerDataMethod():
	m_netType(GameClient_Game){}
crJXJRecvChangePlayerDataMethod::crJXJRecvChangePlayerDataMethod(const crJXJRecvChangePlayerDataMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvChangePlayerDataMethod::inputParam(int i, void *param)
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

void crJXJRecvChangePlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvChangePlayerDataMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char thingtype = m_stream->_readUChar();
			int curdata;
			unsigned short curdata2;
			int msgid = 0;
			int extra = 0;
			switch (thingtype)
			{
			case T_CardImpression:
				curdata2 = m_stream->_readUShort();
				msgid = WCHDATA_JXJCardImpression;
				break;
			case T_JiangHun:
				curdata = m_stream->_readInt();
				msgid = WCHDATA_JXJCardPieces;
				break;
			default:
				break;
			}
			crData *data = m_this->getDataClass();
			if (data)
			{
				switch (thingtype)
				{
				case T_CardImpression:
					data->getParam(msgid,param);
					extra = curdata2 - *(unsigned short *)param;
					data->inputParam(msgid,&curdata2);
					break;
				case T_JiangHun:
					data->getParam(msgid,param);
					extra = curdata - *(int *)param;
					data->inputParam(msgid,&curdata);
					break;
				default:
					break;
				}
				if(extra!=0 && (crRunGameHandle::getInstance()->isInManor() || crRunGameHandle::getInstance()->isInBattle()))
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(thingtype,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvDeductItemsMethod
//
/////////////////////////////////////////
crJXJRecvDeductItemsMethod::crJXJRecvDeductItemsMethod():
	m_this(NULL),
	m_deductItems(NULL)
{
}

crJXJRecvDeductItemsMethod::crJXJRecvDeductItemsMethod(const crJXJRecvDeductItemsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_deductItems(NULL),
	m_type(NULL)
{
}

void crJXJRecvDeductItemsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_deductItems = NULL;
			m_type = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_deductItems = (DeductItemMap *)(LOINT64(param64));
			m_type = (unsigned char *)(HIINT64(param64));
		}
		else
		{
			m_deductItems = NULL;
			m_type = NULL;
		}
		break;
	}
}

void crJXJRecvDeductItemsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvDeductItemsMethod::operator()(crHandle &handle)
{
	if(m_this && m_deductItems)
	{
		DeductItemMap deductItems = *m_deductItems;
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));

			JXJItemBagVec itembagtemp;
			JXJItemBagVec cardbagtemp;
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			data->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
			for (JXJItemBagVec::iterator itr = itembagvec->begin();
				itr != itembagvec->end();
				++itr)
			{
				itembagtemp.push_back(itr->get());
			}
			for (JXJItemBagVec::iterator itr = cardbagvec->begin();
				itr != cardbagvec->end();
				++itr)
			{
				cardbagtemp.push_back(itr->get());
			}
			ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			int itemtypeindex= itemtab->getTitleIndex("类型");
			crTableIO::StrVec itemrecord;
			JXJItemBagVec deductitembagvec;
			JXJItemBagVec tempdeductitembagvec;
			JXJItemBagVec deductcardbagvec;
			JXJItemBagVec tempdeductcardbagvec;
			int maxnum = 0;
			CRCore::ref_ptr<crBagItemData> deductitem;
			CRCore::ref_ptr<crBagItemData> tempdeductitem;
			int itemid;
			_crInt32 equipmagic;
			unsigned char itemcount;
			for( DeductItemMap::iterator itr_deductitem = deductItems.begin();
				itr_deductitem!=deductItems.end();
				++itr_deductitem )
			{
				itemid = itr_deductitem->second->getItemID();
				equipmagic = itr_deductitem->second->getEquipMagic();
				itemcount = itr_deductitem->second->getItemDeductCount();
				if (itemcount > 0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord) >= 0)
					{
						deductitem = new crBagItemData;
						deductitem->setItemID(itemid);
						deductitem->setEquipMagic(equipmagic);
						deductitem->setItemDeductCount(itemcount);
						tempdeductitem = new crBagItemData;
						tempdeductitem->setItemID(itemid);
						tempdeductitem->setEquipMagic(equipmagic);
						tempdeductitem->setItemDeductCount(itemcount);
						if (atoi(itemrecord[itemtypeindex].c_str()) == IT_Jiangka)
						{
							deductcardbagvec.push_back(deductitem);
							tempdeductcardbagvec.push_back(tempdeductitem);
						}
						else
						{
							deductitembagvec.push_back(deductitem);
							tempdeductitembagvec.push_back(tempdeductitem);
						}
					}
				}
			}
			bool candeduct = true;
			int needdeductcount = 0;
			unsigned char curcount = 0;
			unsigned char bagindex = 0;
			typedef std::map<unsigned char,unsigned char > TempDeductItemMap;//bagindex,count
			TempDeductItemMap deductitemmap;
			TempDeductItemMap deductcardmap;
			JXJItemBagVec::iterator itr_newitem;
			//背包是否有该物品
			if(!deductitembagvec.empty())
			{
				itr_newitem = deductitembagvec.begin();
				while (itr_newitem!=deductitembagvec.end())
				{
					bagindex = 0;
					for (JXJItemBagVec::iterator bagitr = itembagtemp.begin();
						bagitr != itembagtemp.end();
						++bagitr,++bagindex)
					{
						if ((*itr_newitem) != NULL && (*bagitr) != NULL)
						{
							if ((*itr_newitem)->getItemID() == (*bagitr)->getItemID() 
								&& (*itr_newitem)->getEquipMagic() == (*bagitr)->getEquipMagic())
							{
								needdeductcount = (*itr_newitem)->getItemDeductCount();
								curcount = (*bagitr)->getItemCount();
								if (curcount > needdeductcount)
								{
									(*bagitr)->setItemCount(curcount - needdeductcount);
									deductitemmap.insert(std::make_pair(bagindex,curcount - needdeductcount));
									needdeductcount = 0;
								}
								else
								{
									(*bagitr) = NULL;
									deductitemmap.insert(std::make_pair(bagindex,0));
									needdeductcount -= curcount;
								}
								(*itr_newitem)->setItemDeductCount(needdeductcount);
								if (needdeductcount == 0)
								{
									break;
								}
							}
						}
					}
					if ((*itr_newitem) != NULL && (*itr_newitem) -> getItemDeductCount() > 0)
					{
						candeduct = false;
						break;
					}
					itr_newitem++;
				}
			}
			if (candeduct)
			{
				//&& 将卡背包是否有该物品
				itr_newitem = deductcardbagvec.begin();
				while (itr_newitem!=deductcardbagvec.end())
				{
					bagindex = 0;
					for (JXJItemBagVec::iterator bagitr = cardbagtemp.begin();
						bagitr != cardbagtemp.end();
						++bagitr,++bagindex)
					{
						if ((*itr_newitem) != NULL && (*bagitr) != NULL)
						{
							if ((*itr_newitem)->getItemID() == (*bagitr)->getItemID() 
								&& (*itr_newitem)->getEquipMagic() == (*bagitr)->getEquipMagic())
							{
								needdeductcount = (*itr_newitem)->getItemDeductCount();
								curcount = (*bagitr)->getItemCount();
								if (curcount > needdeductcount)
								{
									(*bagitr)->setItemCount(curcount - needdeductcount);
									deductcardmap.insert(std::make_pair(bagindex,curcount - needdeductcount));
									needdeductcount = 0;
								}
								else
								{
									(*bagitr) = NULL;
									deductcardmap.insert(std::make_pair(bagindex,0));
									needdeductcount -= curcount;
								}
								(*itr_newitem)->setItemDeductCount(needdeductcount);
								if (needdeductcount == 0)
								{
									break;
								}
							}
						}
					}
					if ((*itr_newitem) != NULL && (*itr_newitem) -> getItemDeductCount() > 0)
					{
						candeduct = false;
						break;
					}
					itr_newitem++;
				}
			}
			if(candeduct)
			{
				unsigned char mapsize = 0;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//扣除背包物品
				if(!tempdeductitembagvec.empty())
				{
					itembagvec->clear();
					for (JXJItemBagVec::iterator itr = itembagtemp.begin();
						itr != itembagtemp.end();
						++itr)
					{
						itembagvec->push_back(itr->get());
					}
					//发送到客户端
					if(playerData.valid())
					{
						mapsize = deductitemmap.size();
						//char size = newitembagvec.size() + newcardbagvec.size();
						stream->createBuf(3 + tempdeductitembagvec.size()*12 + mapsize * 2);
						stream->_writeUChar(0);						//发送物品
						stream->_writeUChar(tempdeductitembagvec.size());
						itr_newitem = tempdeductitembagvec.begin();
						while (itr_newitem!=tempdeductitembagvec.end())
						{
							stream->_writeInt((*itr_newitem)->getItemID());
							stream->_writeInt((*itr_newitem)->getEquipMagic());
							stream->_writeInt((*itr_newitem)->getItemDeductCount());
							itr_newitem++;
						}
						stream->_writeUChar(mapsize);
						for (TempDeductItemMap::iterator itr = deductitemmap.begin();
							itr != deductitemmap.end();
							itr++)
						{
							stream->_writeUChar(itr->first);
							stream->_writeUChar(itr->second);
						}
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJClientRecvDeductItems,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
				//扣除将卡背包物品
				if(!tempdeductcardbagvec.empty())
				{
					cardbagvec->clear();
					for (JXJItemBagVec::iterator itr = cardbagtemp.begin();
						itr != cardbagtemp.end();
						++itr)
					{
						cardbagvec->push_back(itr->get());
					}
					//发送到客户端
					if(playerData.valid())
					{
						mapsize = deductcardmap.size();
						stream->createBuf(3 + tempdeductcardbagvec.size() * 8 + mapsize * 2);
						stream->_writeUChar(1);
						stream->_writeUChar(tempdeductcardbagvec.size());

						itr_newitem = tempdeductcardbagvec.begin();
						while (itr_newitem!=tempdeductcardbagvec.end())
						{
							stream->_writeInt((*itr_newitem)->getItemID());
							stream->_writeInt((*itr_newitem)->getItemDeductCount());
							itr_newitem++;
						}
						stream->_writeUChar(mapsize);
						for (TempDeductItemMap::iterator itr = deductcardmap.begin();
							itr != deductcardmap.end();
							itr++)
						{
							stream->_writeUChar(itr->first);
							stream->_writeUChar(itr->second);
						}

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJClientRecvDeductItems,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvDeductItemsMethod
//
/////////////////////////////////////////
crJXJClientRecvDeductItemsMethod::crJXJClientRecvDeductItemsMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvDeductItemsMethod::crJXJClientRecvDeductItemsMethod(const crJXJClientRecvDeductItemsMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvDeductItemsMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData *)param;
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

void crJXJClientRecvDeductItemsMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvDeductItemsMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			JXJItemBagVec deductitembagvec;
			JXJItemBagVec deductcardbagvec;
			deductitembagvec.push_back(NULL);
			deductcardbagvec.push_back(NULL);
			unsigned char _case = m_stream->_readUChar();
			if(_case == 0)
			{
				//物品
				unsigned char itemsize = m_stream->_readUChar();
				ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				//int maxnumid= itemtab->getTitleIndex("叠加数量");
				crTableIO::StrVec itemrecord;
				CRCore::ref_ptr<crBagItemData> newitem;
				int item_id;
				int equipmagic;
				int item_count;
				for(int i = 0;i<itemsize;i++)
				{
					newitem = new crBagItemData;
					item_id = m_stream->_readInt();
					equipmagic = m_stream->_readInt();
					item_count = m_stream->_readInt();
					newitem->setItemID(item_id);
					newitem->setEquipMagic(equipmagic);
					newitem->setItemDeductCount(item_count);
					deductitembagvec.push_back(newitem);
				}
			}
			else if(_case == 1)
			{
				//将卡
				unsigned char cardsize = m_stream->_readUChar();
				CRCore::ref_ptr<crBagItemData> newcard;
				int card_id;
				//int magicid;
				int card_count;
				for(int i = 0;i<cardsize;i++)
				{
					newcard = new crBagItemData;
					card_id = m_stream->_readInt();
					//magicid = m_stream->_readUChar();
					card_count = m_stream->_readInt();
					newcard->setItemID(card_id);
					//newcard->setEquipMagic(magicid);
					newcard->setItemDeductCount(card_count);
					deductcardbagvec.push_back(newcard);
				}

			}
			unsigned char mapsize = m_stream->_readUChar();
			unsigned char itemcount = 0;
			unsigned char bagindex = 0;
			ref_ptr<crBagItemData> bagitemdata;
			ref_ptr<crBagItemData> bagitemdataget;
			JXJItemBagVec *bagvec;
			if(_case == 0)
			{
				data->getParam(WCHDATA_JXJItemBagVec,param);
				bagvec = (JXJItemBagVec *)param;
			}
			else if(_case == 1)
			{
				data->getParam(WCHDATA_JXJJiangkaBagVec,param);
				bagvec = (JXJItemBagVec *)param;
			}
			for (int i = 0; i < mapsize; i++)
			{
				bagindex = m_stream->_readUChar();
				itemcount = m_stream->_readUChar();
				if (itemcount > 0)
				{
					(*bagvec)[bagindex] -> setItemCount(itemcount);
				}
				else
				{
					(*bagvec)[bagindex] = NULL;
				}
			}

			//扣除物品提示
			if(data && (crRunGameHandle::getInstance()->isInManor() || crRunGameHandle::getInstance()->isInBattle()))
			{
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeItems,MAKEINT64(&deductitembagvec,&deductcardbagvec));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRollCardCountProduceMethod
//
/////////////////////////////////////////
crJXJRollCardCountProduceMethod::crJXJRollCardCountProduceMethod():
	m_dt(0),
	m_delta(0.0f)
{
}
crJXJRollCardCountProduceMethod::crJXJRollCardCountProduceMethod(const crJXJRollCardCountProduceMethod& handle):
	crMethod(handle),
	m_dt(0),
	m_delta(0.0f)
{
}
void crJXJRollCardCountProduceMethod::inputParam(int i, void *param)
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
void crJXJRollCardCountProduceMethod::addParam(int i, const std::string& str)
{
}
void crJXJRollCardCountProduceMethod::operator()(crHandle &handle)
{
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(playerData.valid())
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			//m_dt += m_dt_record;
			//m_dt_record = 0.0f;

			void *param;
			unsigned char FreeRollCardTechLv[3];
			unsigned char FreeRollCardProduceTechLv[4];
			data -> getParam(WCHDATA_JXJFreeRollCardTechLv,param);
			FreeRollCardTechLv[0] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCard2TechLv,param);
			FreeRollCardTechLv[1] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCard3TechLv,param);
			FreeRollCardTechLv[2] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCardProduce1TechLv,param);
			FreeRollCardProduceTechLv[0] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCardProduce2TechLv,param);
			FreeRollCardProduceTechLv[1] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCardProduce3TechLv,param);
			FreeRollCardProduceTechLv[2] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJFreeRollCardProduce4TechLv,param);
			FreeRollCardProduceTechLv[3] = *(unsigned char *)param;
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int limit = 0;
			float produce = 0.0f;
			int curtech = -1;
			int curtechpro = -1;
			for (int i = 0; i < 4; i++)
			{
				if (FreeRollCardTechLv[i] > 0 && i < 3)
				{
					curtech = i;
				}
				if (FreeRollCardProduceTechLv[i] > 0)
				{
					curtechpro = i;
				}
			}
			produce = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountProduce,viplv).c_str());
			crVector3i techmaxvec;
			crVector4i techpromaxvec;
			if (curtech >= 0)
			{
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFreeRollCardTechMaxcount,viplv).c_str(),techmaxvec);
				limit = techmaxvec[curtech];
			}
			else
			{
				limit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountMax,viplv).c_str());
			}
			if (curtechpro >= 0)
			{
				crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFreeRollCardProduceTech,viplv).c_str(),techpromaxvec);
				produce = produce * (100 + techpromaxvec[curtechpro]) / 100;
			}
			//else
			//{
			//	produce = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountMax,viplv).c_str()));
			//}
			//if ( FreeRollCardTechLv > 0)
			//{
			//	limit = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFreeRollCardTechMaxcount,viplv).c_str()));
			//	produce = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFreeRollCardTechProduce,viplv).c_str());
			//}
			//else
			//{
			//	limit = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountMax,viplv).c_str()));
			//	produce = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRollCardFreeCountProduce,viplv).c_str());
			//}

			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJRollCardFreeCount,param);
			unsigned short *rollcardfreecount = (unsigned short *)param;
			limit *= 100;
			if(*rollcardfreecount < limit)
			{
				float dt = (float)m_dt / (float)c_24hours;
				m_delta += produce * dt * 100.0f;
				int output = (int)m_delta;
				if(output>0)
				{
					m_delta -= output;
					int last = *rollcardfreecount/100;
					*rollcardfreecount += output;
					if(*rollcardfreecount > limit) *rollcardfreecount = limit;
					int newcount = *rollcardfreecount/100;
					if(newcount>last)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2);
						stream->_writeUShort(*rollcardfreecount);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvRollCardFreeCountProduce,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
		}
		//else
		//{
		//	m_dt_record += m_dt;
		//}
	}
}
/////////////////////////////////////////
//
//crJXJRecvRollCardCountProduceMethod
//
/////////////////////////////////////////
crJXJRecvRollCardCountProduceMethod::crJXJRecvRollCardCountProduceMethod():
	m_netType(GameClient_Game){}
crJXJRecvRollCardCountProduceMethod::crJXJRecvRollCardCountProduceMethod(const crJXJRecvRollCardCountProduceMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvRollCardCountProduceMethod::inputParam(int i, void *param)
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

void crJXJRecvRollCardCountProduceMethod::addParam(int i, const std::string& str)
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

void crJXJRecvRollCardCountProduceMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			crData *data = m_this->getDataClass();
			unsigned short rollcardfreecount = m_stream->_readUShort();
			if (data)
			{
				data->getParam(WCHDATA_JXJRollCardFreeCount, param);
				int extra = rollcardfreecount/100 - (*(unsigned short *)param/100);
				if (extra != 0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue, MAKEINT64(T_FreeRollCard, extra));
				data->inputParam(WCHDATA_JXJRollCardFreeCount, &rollcardfreecount);
			}
			ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if(canvas.valid() && canvas->getVisiable())
			{
				canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIJiusiSetOnShowMethod
//
/////////////////////////////////////////
crJXJUIJiusiSetOnShowMethod::crJXJUIJiusiSetOnShowMethod()
{
}
crJXJUIJiusiSetOnShowMethod::crJXJUIJiusiSetOnShowMethod(const crJXJUIJiusiSetOnShowMethod& handle):
	crMethod(handle)
{
	for (int i = 0;i<3;i++)
	{
		m_checkbox[i] = handle.m_checkbox[i];
	}
}
void crJXJUIJiusiSetOnShowMethod::inputParam(int i, void *param)
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
void crJXJUIJiusiSetOnShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_checkbox[0] = str;
		break;
	case 1:
		m_checkbox[1]  = str;
		break;
	case 2:
		m_checkbox[2]  = str;
		break;
	}
}
void crJXJUIJiusiSetOnShowMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		data -> getParam(WCHDATA_JXJAutoSellCard,param);
		unsigned short autosellcard  = *(unsigned short *)param;
		ref_ptr<crCheckBoxWidgetNode> checkbox[3] = {NULL};
		for(int i = 0; i<3 ;i++)
		{
			checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_checkbox[i]));
			if (checkbox[i].valid())
			{
				checkbox[i]->select(autosellcard & (1 << i));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJChangeGoldCostHindMethod
//
/////////////////////////////////////////
crJXJUIChangeJiusiSetMethod::crJXJUIChangeJiusiSetMethod():
	m_this(NULL)
{
}
crJXJUIChangeJiusiSetMethod::crJXJUIChangeJiusiSetMethod(const crJXJUIChangeJiusiSetMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
	for (int i = 0; i < 3; i++)
	{
		m_check[i] = handle.m_check[i];
	}
}
void crJXJUIChangeJiusiSetMethod::inputParam(int i, void *param)
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
void crJXJUIChangeJiusiSetMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_check[i] = str;
		break;
	case 1:
		m_check[i] = str;
		break;
	case 2:
		m_check[i] = str;
		break;
	default:
		break;
	}
}
void crJXJUIChangeJiusiSetMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		bool ifhint;
		void *param;
		crData *playerData = myPlayer->getDataClass();
		playerData->getParam(WCHDATA_JXJAutoSellCard,param);
		unsigned short *autosellcard = (unsigned short *)param;
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode> checkbox[3] = {NULL};
			for (int i = 0; i < 3; i++)
			{
				checkbox[i] = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_check[i]));
				if (checkbox[i].valid())
				{
					ifhint = checkbox[i]->getSelect();
				}
				if (ifhint)
				{
					*autosellcard |= (1 << i);
				}
				else
				{
					*autosellcard &=~ (1 << i);
				}
			}
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUShort(*autosellcard);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvChangeJiusiSet,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvChangeGoldCostHindMethod
//
/////////////////////////////////////////
crJXJRecvChangeJiusiSetMethod::crJXJRecvChangeJiusiSetMethod():
	m_netType(GameClient_Game){}
crJXJRecvChangeJiusiSetMethod::crJXJRecvChangeJiusiSetMethod(const crJXJRecvChangeJiusiSetMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvChangeJiusiSetMethod::inputParam(int i, void *param)
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

void crJXJRecvChangeJiusiSetMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvChangeJiusiSetMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			crData *data = m_this->getDataClass();
			//int playerid = m_this->getPlayerID();
			//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			unsigned short autosellcardp;
			autosellcardp = m_stream->_readUShort();
			data->inputParam(WCHDATA_JXJAutoSellCard,&autosellcardp);
			//ref_ptr<crStreamBuf> stream = new crStreamBuf;
			//stream->createBuf(2);
			//stream->_writeUShort(autosellcardp);
			//CRNetApp::crPlayerDataEventPacket packet;
			//CRNetApp::crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvSellJiangkaOK, stream.get());
			//gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			///游戏日志
			GameLogData gamelog(Log_ChangeAutoSellCard,crArgumentParser::appItoa(autosellcardp));
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(m_this->getPlayerID(),&gamelog));
		}
	}
}
/////////////////////////////////////////
//
//crJXJTroopsTechUpdateMethod
//
/////////////////////////////////////////
crJXJTroopsTechUpdateMethod::crJXJTroopsTechUpdateMethod():
	m_this(NULL)
{
}
crJXJTroopsTechUpdateMethod::crJXJTroopsTechUpdateMethod(const crJXJTroopsTechUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_armytyperadio(handle.m_armytyperadio)
{
}
void crJXJTroopsTechUpdateMethod::inputParam(int i, void *param)
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
void crJXJTroopsTechUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_armytyperadio = str;
		break;
	default:
		break;
	}
}
void crJXJTroopsTechUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		int tipsindex = 0;
		bool success = true;
		unsigned short troopstechid = 0;
		unsigned short preid = 0;
		unsigned char prelv = 0;
		unsigned char troopstechlv = 0;
		int armytype = 0;
		ref_ptr<crTableIO>TroopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		ref_ptr<crTableIO>TroopsTechTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechTab);
		ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);
		crTableIO::StrVec record,recordlv;
		crTableIO::DataVec records,armyrecords;
		std::string btnname;
		std::string iconname;
		std::string swname;
		std::string lvname;
		int tarmytypeIndex = TroopsTab->getTitleIndex("兵种");
		int ticonnameIndex = TroopsTab->getTitleIndex("iconname");
		int tswnameIndex = TroopsTab->getTitleIndex("swname");
		int ticonIndex = TroopsTab->getTitleIndex("icon");
		int ticondisableIndex = TroopsTab->getTitleIndex("icon灰");
		int tneedIndex = TroopsTab->getTitleIndex("需求军备ID");
		int armytypeIndex = TroopsTechTab->getTitleIndex("兵种");
		int preidIndex = TroopsTechTab->getTitleIndex("前置ID");
		int btnnameIndex = TroopsTechTab->getTitleIndex("buttonname");
		int iconnameIndex = TroopsTechTab->getTitleIndex("iconname");
		int lvnameIndex = TroopsTechTab->getTitleIndex("lvname");
		int swnameIndex = TroopsTechTab->getTitleIndex("swname");
		int iconIndex = TroopsTechTab->getTitleIndex("icon");
		int icondisableIndex = TroopsTechTab->getTitleIndex("icon灰");
		int copperIndex = TroopsTechLvTab->getTitleIndex("消耗铜钱");
		int lvIndex = TroopsTechLvTab->getTitleIndex("lv");
		int gongfanglvIndex = TroopsTechLvTab->getTitleIndex("工坊lv");
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char*)param;
		ref_ptr<crButtonWidgetNode> btn = NULL;
		ref_ptr<crButtonWidgetNode> imagebox = NULL;
		//ref_ptr<crImageBoxWidgetNode> imagebox = NULL;
		crMultiSwitch *arrowsw = NULL;
		ref_ptr<crStaticTextWidgetNode> lvinput = NULL;
		ref_ptr<crRadioGroupWidgetNode> armytyperadio = dynamic_cast<crRadioGroupWidgetNode*>(m_this->getWidget(m_armytyperadio));
		armytype = armytyperadio->getSelect();
		//data->getParam(WCHDATA_JXJVipExp,param);
		//int vipExp = *(int *)param;
		//if (vipLevel < 1)
		//{
		//	tipsindex = 2069;//需要VIP1
		//	success = false;
		//}
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		TroopsTechMap::iterator troopsitr;
		std::string imagename;
		//////////////////////////////////////////////
		//											//
		//					科技树					//
		//											//
		//////////////////////////////////////////////
		//ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		crVector4f colorgray = crVector4f(170.0f,170.0f,170.0f,255.0f)/255.0f;
		crVector4f colorgreen = crVector4f(0.0f,255.0f,85.0f,255.0f)/255.0f;
		crVector4f coloryellow = crVector4f(255.0f,255.0f,0.0f,255.0f)/255.0f;
		crVector4f colorred = crVector4f(255.0f,80.0f,50.0f,255.0f)/255.0f;
		crVector4f lvcolor = colorgray;
		TroopsTechTab->queryRecords(armytypeIndex,crArgumentParser::appItoa(armytype),records);
		for (crTableIO::DataVec::iterator itr = records.begin();
			itr != records.end();
			++itr)
		{
			success = true;
			swname = (*itr)[swnameIndex];
			lvname = (*itr)[lvnameIndex];
			iconname = (*itr)[iconnameIndex];
			//imagebox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(iconname));
			imagebox = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(iconname));
			arrowsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(swname));
			lvinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(lvname));
			do 
			{
				troopstechid = atoi((*itr)[0].c_str());
				preid = atoi((*itr)[preidIndex].c_str());
				if (troopstechid <= 0)
				{
					//无效的ID
					success = false;
					break;
				}
				troopsitr = troopstechmap -> find(troopstechid);
				if (troopsitr != troopstechmap->end())
				{
					troopstechlv = troopsitr->second;
				}
				else
				{
					troopstechlv = 0;
				}
				if(lvinput.valid())lvinput->setString(crArgumentParser::appItoa(troopstechlv));
				if (preid > 0)
				{
					troopsitr = troopstechmap -> find(preid);
					if (troopsitr != troopstechmap->end())
					{
						prelv = troopsitr->second;
						if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(preid),lvIndex,crArgumentParser::appItoa(prelv + 1),recordlv) >= 0)
						{
							//前置军备未升满
							success = false;
						}
					}
					else
					{
						//前置军备未升满
						success = false;
					}
				}
				int needcopper = 0;
				if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv),recordlv) >= 0)
				{
					needcopper = atoi(recordlv[copperIndex].c_str());

					data->getParam(WCHDATA_JXJGongfanglv,param);
					unsigned char gongfanglv = *(unsigned char *)param;
					if (gongfanglv < atoi(recordlv[gongfanglvIndex].c_str()))
					{
						//工坊等级不足
						lvcolor = colorgray;
						success = false;
					}
				}
				else
				{
					//无效的ID
					lvcolor = colorgray;
					success = false;
				}
				if (success)
				{
					imagename = (*itr)[iconIndex];//imagebox->setImageName((*itr)[iconIndex]);
					if(arrowsw)arrowsw->setActiveSwitchSet(1);
				}
				else
				{
					imagename = (*itr)[icondisableIndex];//imagebox->setImageName((*itr)[icondisableIndex]);
					if(arrowsw)arrowsw->setActiveSwitchSet(0);
				}
				if (imagebox.valid())
				{
					ref_ptr<crStateSet> state_set = dynamic_cast<crObject *>(( dynamic_cast<CRCore::crGroup *>(imagebox->getChild(0)))->getChild(3))->getDrawable(0)->getStateSet();
					if (state_set.valid())
					{
						crTexture2D *texture2d = dynamic_cast<crTexture2D *>(state_set->getTextureAttribute(0,CRCore::crStateAttribute::TEXTURE));
						if (texture2d)
						{
							texture2d->setImage(0, texture2d->getImageDataRequestHandler()->requestImageFile(imagename,texture2d));
						}
					}
				}
				//if (imagebox.valid())imagebox->setImageName(imagename);
				if (!success)
				{
					lvcolor = colorgray;
					break;
				}
				myPlayer->doEvent(WCH_JXJCheckCopperCash,MAKEINT64(&needcopper,NULL));
				if (needcopper != 0)
				{
					//铜钱不足
					lvcolor = colorred;
					success = false;
					break;
				}
				if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv + 1),recordlv) < 0)
				{
					//等级最高
					success = false;
					lvcolor = coloryellow;
					break;
				}
			} while (0);
			btnname = (*itr)[btnnameIndex];
			btn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(btnname));
			if (success)
			{
				lvcolor = colorgreen;
				if (btn.valid())btn->setEnable(true);
				if (btn.valid())btn->setVisiable(true);
			}
			else
			{
				if (btn.valid())btn->setEnable(false);
				if (btn.valid())btn->setVisiable(false);
			}
			if(lvinput.valid())lvinput->setColor(lvcolor);
		}
		//////////////////////////////////////////////
		//											//
		//					兵种树					//
		//											//
		//////////////////////////////////////////////
		std::vector < int > needvec;
		bool ifcanrecuit = true;
		ref_ptr<crImageBoxWidgetNode> bingzhongimagebox = NULL;
		TroopsTab->queryRecords(tarmytypeIndex,crArgumentParser::appItoa(armytype),armyrecords);
		for (crTableIO::DataVec::iterator itr = armyrecords.begin();
			itr != armyrecords.end();
			++itr)
		{
			needvec.clear();
			ifcanrecuit = true;
			iconname = (*itr)[ticonnameIndex];
			swname = (*itr)[tswnameIndex];
			bingzhongimagebox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(iconname));
			arrowsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(swname));
			crArgumentParser::appAtoVec((*itr)[tneedIndex],needvec);
			for (int i = 0; i < needvec.size(); ++i)
			{
				if (needvec[i] > 0)
				{
					troopsitr = troopstechmap -> find(needvec[i]);
					if (troopsitr == troopstechmap->end() || troopsitr->second <= 0)
					{
						ifcanrecuit = false;
						break;
					}
				}
			}
			if (ifcanrecuit)
			{
				if(bingzhongimagebox.valid())bingzhongimagebox->setImageName((*itr)[ticonIndex]);
				if(arrowsw)arrowsw->setActiveSwitchSet(1);
			}
			else
			{
				if(bingzhongimagebox.valid())bingzhongimagebox->setImageName((*itr)[ticondisableIndex]);
				if(arrowsw)arrowsw->setActiveSwitchSet(0);
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJUpgradeTroopsTechMethod
//
/////////////////////////////////////////
crJXJUpgradeTroopsTechMethod::crJXJUpgradeTroopsTechMethod():
	m_this(NULL)/*,
				m_index(0)*/
{
}
crJXJUpgradeTroopsTechMethod::crJXJUpgradeTroopsTechMethod(const crJXJUpgradeTroopsTechMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_armytyperadio(handle.m_armytyperadio)
{
}
void crJXJUpgradeTroopsTechMethod::inputParam(int i, void *param)
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
void crJXJUpgradeTroopsTechMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_armytyperadio = str;
		break;
	default:
		break;
	}
}
void crJXJUpgradeTroopsTechMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		int tipsindex = 0;
		bool success = true;
		unsigned short troopstechid = 0;
		unsigned short preid = 0;
		unsigned char prelv = 0;
		unsigned char troopstechlv = 0;
		int armytype = 0;
		ref_ptr<crTableIO>TroopsTechTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechTab);
		ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);
		crTableIO::StrVec record,recordlv;
		std::string btnname = m_this->getName();
		int armytypeIndex = TroopsTechTab->getTitleIndex("兵种");
		int preidIndex = TroopsTechTab->getTitleIndex("前置ID");
		int btnnameIndex = TroopsTechTab->getTitleIndex("buttonname");
		int iconnameIndex = TroopsTechTab->getTitleIndex("iconname");
		int copperIndex = TroopsTechLvTab->getTitleIndex("消耗铜钱");
		int lvIndex = TroopsTechLvTab->getTitleIndex("lv");
		int gongfanglvIndex = TroopsTechLvTab->getTitleIndex("工坊lv");
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char*)param;
		//data->getParam(WCHDATA_JXJVipExp,param);
		//int vipExp = *(int *)param;
		//if (vipLevel < 1)
		//{
		//	tipsindex = 2069;//需要VIP1
		//	success = false;
		//}
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			ref_ptr<crRadioGroupWidgetNode> armytyperadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_armytyperadio));
			if (armytyperadio.valid())
			{
				armytype = armytyperadio->getSelect();
			}
		}

		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJTroopsTechMap,param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		TroopsTechMap::iterator troopsitr;
		if (TroopsTechTab->queryOneRecord(btnnameIndex,btnname,armytypeIndex,crArgumentParser::appItoa(armytype),record) >= 0
			||TroopsTechTab->queryOneRecord(iconnameIndex,btnname,armytypeIndex,crArgumentParser::appItoa(armytype),record) >= 0)
		{
			troopstechid = atoi(record[0].c_str());
			preid = atoi(record[preidIndex].c_str());
		}
		do 
		{
			if (troopstechid <= 0)
			{
				tipsindex = 2070;//无效的ID
				success = false;
				break;
			}
			if (preid > 0)
			{
				troopsitr = troopstechmap -> find(preid);
				if (troopsitr != troopstechmap->end())
				{
					prelv = troopsitr->second;
					if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(preid),lvIndex,crArgumentParser::appItoa(prelv + 1),recordlv) >= 0)
					{
						tipsindex = 2080;//前置军备未升满
						success = false;
						break;
					}
				}
				else
				{
					tipsindex = 2080;//前置军备未升满
					success = false;
					break;
				}
			}
			troopsitr = troopstechmap -> find(troopstechid);
			if (troopsitr != troopstechmap->end())
			{
				troopstechlv = troopsitr->second;
			}
			else
			{
				troopstechlv = 0;
			}
			if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv),recordlv) >= 0)
			{
				int needcopper = atoi(recordlv[copperIndex].c_str());

				myPlayer->doEvent(WCH_JXJCheckCopperCash,MAKEINT64(&needcopper,NULL));
				if (needcopper != 0)
				{
					tipsindex = 2032;//铜钱不足
					success = false;
					break;
				}
				data->getParam(WCHDATA_JXJGongfanglv,param);
				unsigned char gongfanglv = *(unsigned char *)param;
				if (gongfanglv < atoi(recordlv[gongfanglvIndex].c_str()))
				{
					tipsindex = 2081;//工坊等级不足
					success = false;
					break;
				}
			}
			else
			{
				tipsindex = 2070;//无效的ID
				success = false;
				break;
			}
			if (success)
			{
				if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv + 1),recordlv) < 0)
				{
					tipsindex = 2072;//等级最高
					success = false;
					break;
				}
			}
		} while (0);
		data->excHandle(MAKEINT64(WCH_LockData,0));
		if (success)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(2);
			stream->_writeUShort(troopstechid);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUpgradeTroopsTech,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,tipsindex);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUpgradeTroopsTechMethod
//
/////////////////////////////////////////
crJXJRecvUpgradeTroopsTechMethod::crJXJRecvUpgradeTroopsTechMethod():
	m_netType(GameClient_Game){}
crJXJRecvUpgradeTroopsTechMethod::crJXJRecvUpgradeTroopsTechMethod(const crJXJRecvUpgradeTroopsTechMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvUpgradeTroopsTechMethod::inputParam(int i, void *param)
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

void crJXJRecvUpgradeTroopsTechMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUpgradeTroopsTechMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			unsigned short troopstechid = m_stream->_readUShort();
			char success = 0;
			unsigned short preid = 0;
			unsigned char prelv = 0;
			unsigned char troopstechlv = 0;
			int needcopper  = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				ref_ptr<crTableIO>TroopsTechTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechTab);
				ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);
				crTableIO::StrVec record,recordlv;
				int preidIndex = TroopsTechTab->getTitleIndex("前置ID");
				int copperIndex = TroopsTechLvTab->getTitleIndex("消耗铜钱");
				int lvIndex = TroopsTechLvTab->getTitleIndex("lv");
				int gongfanglvIndex = TroopsTechLvTab->getTitleIndex("工坊lv");
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT16(WCH_LockData,1));
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char*)param;
				data->getParam(WCHDATA_JXJTroopsTechMap,param);
				TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
				TroopsTechMap::iterator troopsitr;
				do 
				{
					if (troopstechid <= 0 || TroopsTechTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),record) < 0 || 
						preidIndex < 0 || copperIndex < 0 || lvIndex < 0 || gongfanglvIndex < 0)
					{
						success = -1;//无效的ID
						break;
					}
					preid = atoi(record[preidIndex].c_str());
					if (preid > 0)
					{
						troopsitr = troopstechmap -> find(preid);
						if (troopsitr != troopstechmap->end())
						{
							prelv = troopsitr->second;
							if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(preid),lvIndex,crArgumentParser::appItoa(prelv + 1),recordlv) >= 0)
							{
								success = -2;//前置军备未升满
								break;
							}
						}
						else
						{
							success = -2;//前置军备未升满
							break;
						}
					}
					troopsitr = troopstechmap -> find(troopstechid);
					if (troopsitr != troopstechmap->end())
					{
						troopstechlv = troopsitr->second;
					}
					else
					{
						troopstechlv = 0;
					}
					if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv),recordlv) >= 0)
					{
						needcopper = atoi(recordlv[copperIndex].c_str());
						data->getParam(WCHDATA_JXJGongfanglv,param);
						unsigned char gongfanglv = *(unsigned char *)param;
						if (gongfanglv < atoi(recordlv[gongfanglvIndex].c_str()))
						{
							success = -4;//工坊等级不足
							break;
						} 
					}
					else
					{
						success = -1;//无效的ID
						break;
					}
					troopstechlv += 1;
					if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(troopstechlv),recordlv) < 0)
					{
						success = -5;//已到最高级
						break;
					}
					m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&needcopper,NULL));
					if (needcopper != 0)
					{
						success = -3;//铜钱不足
						break;
					}
					success = 1;
					if (troopsitr != troopstechmap->end())
					{
						troopsitr->second += 1;
					}
					else
					{
						troopstechmap->insert(std::make_pair(troopstechid,troopstechlv));
					}
                    m_this->doEvent(WCH_JXJCheck7Day);
				} while (0);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(4);
					stream->_writeChar(success);
					stream->_writeUShort(troopstechid);
					stream->_writeUChar(troopstechlv);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUpgradeTroopsTech,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				data->excHandle(MAKEINT16(WCH_LockData,0));
				///游戏日志
				std::string logdata = "升级兵种科技(success,success,troopstechlv)，" + crArgumentParser::appItoa(success)+","+ crArgumentParser::appItoa(success)+","+ crArgumentParser::appItoa(troopstechlv);
				GameLogData gamelog(Log_UpgradeTroopsTech,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream->_readChar();
			if (success < 0)
			{
				int index = 0;
				switch(success)
				{
				case -1:
					index = 2070;//无效的ID
					break;
				case -2:
					index = 2080;//前置军备未升满
					break;
				case -3:
					index = 2032;//铜钱不足
					break;
				case -4:
					index = 2081;//工坊等级不足
					break;
				case -5:
					index = 2072;//已到最高级
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
			else if (success == 1)
			{
				unsigned short troopstechid = m_stream->_readUShort();
				unsigned char troopstechlv = m_stream->_readUChar();
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT16(WCH_LockData,1));
				data->getParam(WCHDATA_JXJTroopsTechMap,param);
				TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
				TroopsTechMap::iterator troopsitr;
				troopsitr = troopstechmap->find(troopstechid);
				if (troopsitr != troopstechmap->end())
				{
					troopsitr->second = troopstechlv;
				}
				else
				{
					troopstechmap->insert(std::make_pair(troopstechid,troopstechlv));
				}
				data->excHandle(MAKEINT16(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTroopsTechTipsMethod
//
/////////////////////////////////////////
crJXJTroopsTechTipsMethod::crJXJTroopsTechTipsMethod():
	m_ea(NULL),
	m_colorred(COLOR_RED),
	m_colorgreen(COLOR_GREEN)
{
}
crJXJTroopsTechTipsMethod::crJXJTroopsTechTipsMethod(const crJXJTroopsTechTipsMethod& handle):
	crMethod(handle),
	m_tipscanvas(handle.m_tipscanvas),
	m_typeradio(handle.m_typeradio),
	m_icon(handle.m_icon),
	m_infocanvas(handle.m_infocanvas),
	m_colorred(handle.m_colorred),
	m_colorgreen(handle.m_colorgreen)
{
	for (int i = 0; i < 2; i++)
	{
		m_input[i] = handle.m_input[i];
	}
}
void crJXJTroopsTechTipsMethod::inputParam(int i, void *param)
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
void crJXJTroopsTechTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipscanvas = str;
		break;
	case 1:
		m_typeradio = str;
		break;
	case 2:
		m_icon = str;
		break;
	case 3:
		m_input[0] = str;
		break;
	case 4:
		m_input[1] = str;
		break;
	case 5:
		m_infocanvas = str;
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_colorred);
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_colorgreen);
		break;
	case 8:
		m_infobk = str;
		break;
	case 9:
		m_infodi = str;
		break;
	}
}
void crJXJTroopsTechTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipscanvas);
	ref_ptr<crCanvasNode>infoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(tipsCanvas.valid() && canvas.valid() && playerData && playerData->getDataClass())
	{
		//void *param;
		crData *data = playerData->getDataClass();
		std::string btnname;
		int troopstype = 0;
		ref_ptr<crRadioGroupWidgetNode> typeradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_typeradio));
		if(typeradio.valid())troopstype = typeradio->getSelect();
		btnname = m_this->getName(); 
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_icon));
		ref_ptr<crStaticTextWidgetNode> nameinput = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_input[0]));
		ref_ptr<crHypertextWidgetNode> infoinput = dynamic_cast<crHypertextWidgetNode *>(infoCanvas->getWidget(m_input[1]));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infoCanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infoCanvas->getChildNode(m_infodi));
		//ref_ptr<crHypertextWidgetNode> info2 = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_input[1]));

		//ref_ptr<crHypertextWidgetNode> jinengmiaoshu = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_jinengmiaoshu));
		//ref_ptr<crImageBoxWidgetNode> card = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_card));
		//ref_ptr<crImageBoxWidgetNode> nameimage = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_nameimage));
		//crMultiSwitch *backboard = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_backboard));
		//crMultiSwitch *cardlvsw = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_cardlvsw));
		if(setInfo() && infoinput.valid())
		{
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
			//crVector2 mouse(crVector2(posx2,posy2));
			///////////////////////////////////////////
			//tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f) * mat);
			///////////计算缩放
			float charactersize = infoinput->getCharacterSize();
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			infoinput->updateData();
			//const crBoundingBox &tips = tipsCanvas->getBoundBox();
			const crBoundingBox &bbox = infoinput->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = infoinput->getBoundBox();

			float texheight = 0.0f;
			float needmove = 0.0f;
			if(bbox.valid())
			{
				texheight = bbox.yLength();
				float factor = crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
				texheight *= factor;
				charactersize *= factor*c_tipsoffsets;
				if(texheight<boxYlength) texheight = boxYlength;
				float scale = texheight/boxYlength;

				float needmove ;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = infoinput->getBound().center();

				float tmpLength = bbox2.yLength();
				if(infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
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
					//needmove = boxYlength - texheight + diYlength / 2 ;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.125f;
					infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				}
				//const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				//crVector3 centerTansTar = infoinput->getBound().center();

				//float tmpLength = bbox2.yLength();
				//centerTansTar[1] += 0.5 * (tmpLength - texheight) +  charactersize ;
				//// 源中心点
				//crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
				//// 将源中心点平移到原点
				//crMatrix put_mat = crMatrix::translate(centerTansSrc);
				//crMatrix inv_put = crMatrix::inverse(put_mat);
				//crMatrix rotmat;
				//rotmat.postMult(inv_put);
				//rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
				//rotmat.postMult(crMatrix::translate(centerTansTar));
				//if(infobk)infobk->setMatrix(rotmat);

				//if(infodi)
				//{
				//	float diYlength = infodi->getChild(0)->getBoundBox().yLength();
				//	//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
				//	needmove = boxYlength - texheight + charactersize ;
				//	infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
				//}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
				if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
			}
			///////////////////////////////////////
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
			//infobk->setMatrix(crMatrix::scale(1.0f,m_expandnum,1.0f) * crMatrix::translate(0,(infobk->getChild(0)->getBoundBox().yLength()*0.5f),0.0f));
			infoCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));//(crMatrix::translate(mouse[0],mouse[1]-posy*2.0f-tipsCanvas->getBoundBox().yLength()*0.5f,0.0f));

			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
			tipsCanvas->setCanFocus(false);
			infoCanvas->setCanFocus(false);

			crFilterRenderManager::getInstance()->showCanvas(infoCanvas.get(),true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		}
		else
		{
			crFilterRenderManager::getInstance()->closeCanvas(infoCanvas.get());
			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
		}
	}
}
//std::string crJXJTroopsTechTipsMethod::getMagicAttrTipsStr(int startid,const CREncapsulation::crTableIO::StrVec &record)
//{
//	std::string str;
//	char buf[64];
//	//int basepercent = getBaseAttrPercent(startid,record);
//	//if(basepercent>0)
//	//{
//	//	float f = (float)basepercent * 0.1f;
//
//	//	sprintf(buf,"基础属性+%.1f%%\0",f);
//	//	str = buf;
//	//	//str = "基础属性"+crArgumentParser::appFtoa(f)+"%";
//	//	return str;
//	//}
//	short base;
//	std::string perstr[27] = { "攻速", "CD缩减", "移动速度", "护甲值", "法术抗性", "格挡几率", "攻击力", "生命", "暴击",
//		"闪避", "攻城攻击", "射程", "命中", "穿透", "法术穿透", "潜行", "反潜行", "视野", "无视闪避\n", "反弹物伤",
//		"反弹法伤", "物理吸血", "法术吸血", "暴击伤害", "溅射伤害\n", "物伤加深", "法伤加深" };
//	std::vector<float>v_i;
//	std::string _str;
//	for (int i = 0; i<28;i++)
//	{
//		if(i==18)
//		{//无视闪避
//			base = (short)(atoi(record[startid+i].c_str()));
//			if(base!=0)
//			{
//				str += "无视闪避\n";
//			}
//		}
//		else if (i == 23)
//		{//暴击剑,不可叠加
//			base = (short)(atoi(record[startid + i].c_str()));
//			if (base != 0)
//			{
//				float f = (float)base * 0.1f;
//				//sprintf(buf,"暴击伤害+%.1f%%\0",f);
//				if (base - ((int)f * 10) > 0)
//				{
//					sprintf(buf, "暴击伤害+%.1f%%\n\0", f);
//				}
//				else
//				{
//					sprintf(buf, "暴击伤害+%.0f%%\n\0", f);
//				}
//				str += buf;
//				//str += "暴击伤害提升到250%,";
//			}
//		}
//		else if(i==24)
//		{//溅射,不可叠加
//			crVector2f vecbase;
//			if(crArgumentParser::appAtoVec(record[startid+i],vecbase))
//			{
//				char buf[64];
//				sprintf(buf,"近战溅射伤害(%d,%.1f%%)\n\0",(short)vecbase[0],vecbase[1]*0.1f);
//				//sprintf(buf,"溅射伤害(%d,%d%%),\0",(short)vecbase[0],(short)vecbase[1]);
//				str += buf;
//			}
//		}
//		else if(i==25)
//		{
//			_str = perstr[i];
//			base = (short)(atoi(record[startid+i].c_str()));
//			if(base<0)
//			{
//				base = -base;
//				_str = "物伤减免";
//			}
//			if(base!=0)
//			{
//				float f = (float)base * 0.1f;
//				sprintf(buf,"+%.1f%%\n\0",f);
//				str += _str+buf;
//				//str += _str+crArgumentParser::appFtoa(f)+"%,";
//			}
//		}
//		else if(i==26)
//		{
//			_str = perstr[i];
//			base = (short)(atoi(record[startid+i].c_str()));
//			if(base<0)
//			{
//				base = -base;
//				_str = "法伤减免";
//			}
//			if(base!=0)
//			{
//				float f = (float)base * 0.1f;
//				sprintf(buf,"+%.1f%%\n\0",f);
//				str += _str+buf;
//				//str += _str+crArgumentParser::appFtoa(f)+"%,";
//			}
//		}
//		else if (i == 27)
//		{
//			if ((startid + i) < record.size())
//			{
//				crTableIO::StrVec bufRec;
//				ref_ptr<crTableIO>skillAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
//				int bufindex = skillAttrTab->getTitleIndex("描述");
//				int bufaid = atoi(record[i].c_str());
//				if (bufaid > 0)
//				{
//					if (skillAttrTab->queryOneRecord(0, crArgumentParser::appItoa(bufaid), bufRec) >= 0)
//					{//buf描述
//						if (!bufRec[bufindex].empty())
//						{
//							str += bufRec[bufindex]+"\n";
//						}
//					}
//				}
//			}
//		}
//		else
//		{
//			base = (short)(atoi(record[startid+i].c_str()));
//			if(base!=0)
//			{
//				if(i<=1||(i>=3&&i<=10)||(i>=12&&i<=14)||(i>=19&&i<=22))
//				{
//					float f = (float)base * 0.1f;
//					if (base - ((int)f * 10) > 0)
//					{
//						sprintf(buf, "%.1f%%\n\0", f);
//					}
//					else
//					{
//						sprintf(buf, "%.0f%%\n\0", f);
//					}
//
//					if (base < 0)
//						str += perstr[i] + "-" + buf;
//					else
//						str += perstr[i] + "+" + buf;
//					//str += perstr[i]+crArgumentParser::appFtoa(f)+"%,";
//				}
//				else
//				{
//					if(base<0)
//						str += perstr[i]+"-"+crArgumentParser::appItoa(base)+"\n";
//					else
//						str += perstr[i]+"+"+crArgumentParser::appItoa(base)+"\n";
//				}
//			}
//		}
//	}
//	//if(!str.empty())
//	//	str[str.length()-1] = '\n';
//	return str;
//}
bool crJXJTroopsTechTipsMethod::setInfo()
{
	ref_ptr<crCanvasNode>tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipscanvas);
	ref_ptr<crCanvasNode>infoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if(tipsCanvas.valid() && canvas.valid() && playerData && playerData->getDataClass())
	{
		void *param;
		crData *data = playerData->getDataClass();
		std::string btnname;
		int troopstype = 0;
		ref_ptr<crRadioGroupWidgetNode> typeradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_typeradio));
		if(typeradio.valid())troopstype = typeradio->getSelect();
		btnname = m_this->getName(); 
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_icon));
		ref_ptr<crStaticTextWidgetNode> nameinput = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_input[0]));
		ref_ptr<crHypertextWidgetNode> infoinput = dynamic_cast<crHypertextWidgetNode *>(infoCanvas->getWidget(m_input[1]));
		//ref_ptr<crHypertextWidgetNode> info2 = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_input[1]));

		//ref_ptr<crHypertextWidgetNode> jinengmiaoshu = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_jinengmiaoshu));
		//ref_ptr<crImageBoxWidgetNode> card = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_card));
		//ref_ptr<crImageBoxWidgetNode> nameimage = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_nameimage));
		//crMultiSwitch *backboard = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_backboard));
		//crMultiSwitch *cardlvsw = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_cardlvsw));
		int troopstechid = 0;
		crTableIO::StrVec record,needtechrecord,needtechlvrecord,techlvrecord,troopsrecord,nexttechlvrecord;
		ref_ptr<crTableIO>TroopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		ref_ptr<crTableIO>TroopsTechTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechTab);
		ref_ptr<crTableIO>TroopsTechLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTechLvTab);

		int TnameIndex = TroopsTab->getTitleIndex("name");
		int iconIndex = TroopsTechTab->getTitleIndex("icon");
		int nameIndex = TroopsTechTab->getTitleIndex("name");
		int infoIndex = TroopsTechTab->getTitleIndex("介绍");
		int iconnameIndex = TroopsTechTab->getTitleIndex("iconname");
		int buttonnameIndex = TroopsTechTab->getTitleIndex("buttonname");
		int armytypeIndex = TroopsTechTab->getTitleIndex("兵种");
		int needtechidindex = TroopsTechTab->getTitleIndex("前置ID");
		int lvIndex = TroopsTechLvTab->getTitleIndex("lv");
		int startIndex = TroopsTechLvTab->getTitleIndex("攻速");
		int TarmytypeIndex = TroopsTechLvTab->getTitleIndex("兵种");
		int TabstractidIndex = TroopsTechLvTab->getTitleIndex("abstractid");
		int TgongfanglvIndex = TroopsTechLvTab->getTitleIndex("工坊lv");
		int TneedcopperIndex = TroopsTechLvTab->getTitleIndex("消耗铜钱");
		data->getParam(WCHDATA_JXJTroopsTechMap, param);
		TroopsTechMap *troopstechmap = (TroopsTechMap *)param;
		std::string effectarmytypestr;
		std::string addstr = "0级无效果";
		std::string addstrnext = "已到最高级";
		std::string needgongfanglvstr = "需求工坊等级：";
		std::string needcopperstr = "需求铜钱：";
		std::string needqianzhitechstr;//前置科技
		unsigned char needgongfanglv = 0;
		unsigned char techlv = 0;
		unsigned char needtechlv = 0;
		int Tbingzhong = 0;
		int needcopper = 0;
		int needtechid = 0;
		bool ifneedtechmax = false;
		std::vector<int> Tabstractid;
		if (TroopsTechTab->queryOneRecord(iconnameIndex,btnname,armytypeIndex,crArgumentParser::appItoa(troopstype),record) >= 0 ||
			TroopsTechTab->queryOneRecord(buttonnameIndex,btnname,armytypeIndex,crArgumentParser::appItoa(troopstype),record) >= 0)
		{
			troopstechid = atoi(record[0].c_str());
			if (troopstechid > 0)
			{
				TroopsTechMap::iterator fitr = troopstechmap->find(troopstechid);
				if (fitr != troopstechmap->end())
				{
					techlv = fitr->second;
				}
				if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(techlv),techlvrecord) >= 0)
				{
					needcopper = atoi(techlvrecord[TneedcopperIndex].c_str());
					needgongfanglv = atoi(techlvrecord[TgongfanglvIndex].c_str());
					if (needcopper > 0)
					{
						needcopperstr +=techlvrecord[TneedcopperIndex];
					}
					else
					{
						needcopperstr = "不消耗铜钱";
					}
					needtechid = atoi(record[needtechidindex].c_str());
					if (needtechid > 0)
					{
						if (TroopsTechTab->queryOneRecord(0,crArgumentParser::appItoa(needtechid),needtechrecord) >= 0)
						{
							fitr = troopstechmap->find(needtechid);
							if (fitr != troopstechmap->end())
							{
								needtechlv = fitr->second;
							}
							if(TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(needtechid),lvIndex,crArgumentParser::appItoa(needtechlv + 1),needtechlvrecord) < 0)
							{
								ifneedtechmax = true;
							}
							needqianzhitechstr = "需要[" + needtechrecord[nameIndex] + "]达到满级";
						}
					}
					else
					{
						ifneedtechmax = true;
						needqianzhitechstr = "无前置条件";
					}
					needgongfanglvstr += techlvrecord[TgongfanglvIndex];
					Tbingzhong = atoi(techlvrecord[TarmytypeIndex].c_str());
					crArgumentParser::appAtoVec(techlvrecord[TabstractidIndex],Tabstractid);
					if (Tbingzhong == -1 && Tabstractid[0] == -1)
					{
						effectarmytypestr = "对所有兵种有效";
					}
					else
					{
						if (Tbingzhong != -1)
						{
							switch (Tbingzhong)
							{
							case 0:
								effectarmytypestr = "对步兵有效";
								break;
							case 1:
								effectarmytypestr = "对弓兵有效";
								break;
							case 2:
								effectarmytypestr = "对骑兵有效";
								break;
							case 3:
								effectarmytypestr = "对器械有效";
								break;
							}
						}
						else
						{
							effectarmytypestr = "对";
							for (int i = 0; i < Tabstractid.size(); ++i)
							{
								if (TroopsTab->queryOneRecord(0,crArgumentParser::appItoa(Tabstractid[i]),troopsrecord) >= 0)
								{
									effectarmytypestr += "[" + troopsrecord[TnameIndex] + "]";
								}
							}
							effectarmytypestr += "有效";
						}
					}


					addstr = crJXJCreateItemTipsMethod::getMagicAttrTipsStr(startIndex, techlvrecord,'\n');

					if (TroopsTechLvTab->queryOneRecord(0,crArgumentParser::appItoa(troopstechid),lvIndex,crArgumentParser::appItoa(techlv + 1),nexttechlvrecord) >= 0)
					{
						//addstrnext += "下一级： \n";
						//for (int i = startIndex; i < startIndex + 27; ++i)
						//{
						//	if(atoi(nexttechlvrecord[i].c_str()) > 0)
						//	{
						addstrnext = crJXJCreateItemTipsMethod::getMagicAttrTipsStr(startIndex, nexttechlvrecord, '\n');
						//	}
						//}
					}
					if(icon.valid())icon->setImageName(record[iconIndex]);
					if(nameinput.valid())nameinput->setString(record[nameIndex]);
					//插入TIPS
					data->getParam(WCHDATA_JXJGongfanglv,param);
					unsigned char gongfanglv = *(unsigned char *)param;
					data->getParam(WCHDATA_JXJCoppercash,param);
					int mycopper = *(int *)param;
					int content = 0;
					std::vector<float>v_i;
					ItemMap itemMap,itemMap2;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");

					v_i.clear();
					v_i.push_back(32.0f);
					v_i.push_back(32.0f);
					v_i.push_back(32.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Color",v_i);

					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",effectarmytypestr + "\n");
					cfg_script.Pop();

					v_i.clear();
					for (int i = 0; i < 4; ++i)
					{
						v_i.push_back(m_colorgreen[i]);
					}
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",addstr + "\n");
					cfg_script.Pop();

					v_i.clear();
					for (int i = 0; i < 4; ++i)
					{
						v_i.push_back(m_colorred[i]);
					}
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text","下一级：\n" + addstrnext + "\n");
					cfg_script.Pop();
					//需求工坊等级
					v_i.clear();
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					if (gongfanglv < needgongfanglv)
					{
						for (int i = 0; i < 4; ++i)
						{
							v_i.push_back(m_colorred[i]);
						}
						cfg_script.Add("Color",v_i);
					}
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",needgongfanglvstr + "\n");
					cfg_script.Pop();

					//需求铜钱
					v_i.clear();
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					if (mycopper < needcopper || gongfanglv < needgongfanglv)
					{
						for (int i = 0; i < 4; ++i)
						{
							v_i.push_back(m_colorred[i]);
						}
						cfg_script.Add("Color",v_i);
					}
					else
					{
						for (int i = 0; i < 4; ++i)
						{
							v_i.push_back(m_colorgreen[i]);
						}
						cfg_script.Add("Color",v_i);
					}
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",needcopperstr + "\n");
					cfg_script.Pop();

					//需求前置科技
					v_i.clear();
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					if (!ifneedtechmax)
					{
						for (int i = 0; i < 4; ++i)
						{
							v_i.push_back(m_colorred[i]);
						}
						cfg_script.Add("Color",v_i);
					}
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",needqianzhitechstr + "\n");
					cfg_script.Pop();

					v_i.clear();
					v_i.push_back(32.0f);
					v_i.push_back(32.0f);
					v_i.push_back(32.0f);
					v_i.push_back(255.0f);
					cfg_script.Add("Content");
					cfg_script.Push("Content",++content);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("DrawMode","TEXT");
					cfg_script.Add("CharacterSize",16);
					cfg_script.Add("Text",record[infoIndex]);
					cfg_script.Pop();

					cfg_script.Pop();
					//cfg_script.Write("1314");
					if(infoinput.valid())infoinput->setHypertext(cfg_script);
					return true;
				}
			}
		}
	}
	return false;
}
/////////////////////////////////////////
//
//crJXJJiusiCritTimerMethod
//
/////////////////////////////////////////
crJXJJiusiCritTimerMethod::crJXJJiusiCritTimerMethod():
	m_lastpunishtime(0),
	m_lasteffect(0)
{
}

crJXJJiusiCritTimerMethod::crJXJJiusiCritTimerMethod(const crJXJJiusiCritTimerMethod& handle):
	crMethod(handle),
	m_critsw(handle.m_critsw),
	m_lastpunishtime(handle.m_lastpunishtime),
	m_lasteffect(handle.m_lasteffect)
{
	for (int i = 0; i < 4; i++)
	{
		m_sequence[i] = handle.m_sequence[i];
	}
}

void crJXJJiusiCritTimerMethod::inputParam(int i, void *param)
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

void crJXJJiusiCritTimerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_critsw = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		m_sequence[i - 1] = str;
		break;
	default:
		break;
	}
}

void crJXJJiusiCritTimerMethod::operator()(crHandle &handle)
{
	if (!m_this)
	{
		return;
	}
	crMultiSwitch *critsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_critsw));
	int seqindex = 0;
	if (critsw)
	{
		seqindex = critsw->getActiveSwitchSet();
	}
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_lastpunishtime <= 0 || m_lasteffect!= seqindex)
	{
		crSequence *sequence = dynamic_cast<crSequence *>(m_this->getChildNode(m_sequence[seqindex]));
		if(sequence)
		{
			sequence->setVanishWhenStop(true);
			//sequence->setDuration(0.1f, 1);
			sequence->startEffect();
			m_lastpunishtime = 2;
			m_lasteffect = seqindex;
		}
	}
	else
	{
		m_lastpunishtime -= 1;
	}
	int punish = m_lastpunishtime;
	if(punish <= 0)
	{
		if(m_this->getVisiable())
		{
			crFilterRenderManager::getInstance()->closeCanvas(m_this);
		}
	}
}
/////////////////////////////////////////
//
//crJXJAddBlackNameMethod
//
/////////////////////////////////////////
crJXJAddBlackNameMethod::crJXJAddBlackNameMethod():
	m_this(NULL)
{
}
crJXJAddBlackNameMethod::crJXJAddBlackNameMethod(const crJXJAddBlackNameMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_armytyperadio(handle.m_armytyperadio)
{
}
void crJXJAddBlackNameMethod::inputParam(int i, void *param)
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
void crJXJAddBlackNameMethod::addParam(int i, const std::string& str)
{
}
void crJXJAddBlackNameMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		ref_ptr<crCanvasNode>canvas = m_this -> getParentCanvas();
		crData *canvasdata = canvas->getDataClass();
		std::string blackname;
		void *param;
		crData *data = myPlayer->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data -> getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		int blacknamemax = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBlackNameSetMax,viplv).c_str());
		data->getParam(WCHDATA_JXJBlackNameSet,param);
		BlackNameSet *blacknameset = (BlackNameSet *)param;
		BlackNameSet::iterator finditr = blacknameset->find(blackname);
		if (blacknameset->size() < blacknamemax)
		{
			if (finditr == blacknameset->end())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4 + blackname.size());
				stream->_writeString(blackname);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddBlackName,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);
				blacknameset->insert(blackname);
			}
			else
			{
				myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,2084);//已在黑名单中
			}
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,2086);//黑名单已满
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJRecvAddBlackNameMethod
//
/////////////////////////////////////////
crJXJRecvAddBlackNameMethod::crJXJRecvAddBlackNameMethod():
	m_netType(GameClient_Game){}
crJXJRecvAddBlackNameMethod::crJXJRecvAddBlackNameMethod(const crJXJRecvAddBlackNameMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvAddBlackNameMethod::inputParam(int i, void *param)
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

void crJXJRecvAddBlackNameMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvAddBlackNameMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this -> getPlayerID();
			crData *data = m_this -> getDataClass();
			std::string blackname = m_stream->_readString();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				char success = 0;
				crData *data = m_this->getDataClass();
				data -> getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int blacknamemax = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBlackNameSetMax,viplv).c_str());
				data->excHandle(MAKEINT16(WCH_LockData,1));
				data->getParam(WCHDATA_JXJBlackNameSet,param);
				BlackNameSet *blacknameset = (BlackNameSet*)param;
				do
				{
					if (blackname == m_this->getCharacterName())
					{
						success = -1;
						break;
					}
					if (blacknameset->size() >= blacknamemax)
					{
						success = -2;
						break;
					}
					if (success == 0)
					{
						blacknameset->insert(blackname);
						data->getParam(WCHDATA_JXJFriendList,param);
						FriendVec *friendVec = (FriendVec *)param;
						for (FriendVec::iterator itr = friendVec->begin();
							itr != friendVec->end();
							++itr)
						{
							if ((*itr)->getName() == blackname)
							{
								friendVec->erase(itr);
								break;
							}
						}
						success = 1;
					}
				}while (0);
				data->excHandle(MAKEINT16(WCH_LockData,0));
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(success);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddBlackName,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = "添加黑名单";
				GameLogData gamelog(Log_AddBlackName,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int tipsindex = 0;
			char success = m_stream -> _readChar();
			if (success == 1)
			{
				tipsindex = 2094;
			}
			else
			{
				switch (success)
				{
				case  -2:
					tipsindex = 2086;
					break;
				default:
					break;
				}
			}
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,tipsindex);//黑名单已满
		}
	}
}
/////////////////////////////////////////
//
//crJXJDeleteBlackNameMethod
//
/////////////////////////////////////////
crJXJDeleteBlackNameMethod::crJXJDeleteBlackNameMethod():
	m_this(NULL)
{
}
crJXJDeleteBlackNameMethod::crJXJDeleteBlackNameMethod(const crJXJDeleteBlackNameMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_armytyperadio(handle.m_armytyperadio)
{
}
void crJXJDeleteBlackNameMethod::inputParam(int i, void *param)
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
void crJXJDeleteBlackNameMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_blacklist = str;
		break;
	default:
		break;
	}
}
void crJXJDeleteBlackNameMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		std::string blackname;
		std::wstring outdata;
		ref_ptr<crCanvasNode>canvas = m_this -> getParentCanvas();
		ref_ptr<crTableWidgetNode> friendList = dynamic_cast<crTableWidgetNode *>(canvas->getWidget(m_blacklist));
		if (friendList.valid())
		{
			friendList->getData(0,friendList->getSelect(),outdata);
			blackname = CRIOManager::convertUTF16toUTF8(outdata);
		}
		crData *canvasdata = canvas->getDataClass();
		void *param;
		crData *data = myPlayer->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJBlackNameSet,param);
		BlackNameSet *blacknameset = (BlackNameSet *)param;
		BlackNameSet::iterator finditr = blacknameset->find(blackname);
		if (finditr != blacknameset->end())
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4 + blackname.size());
			stream->_writeString(blackname);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvDeleteBlackName,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
			blacknameset->erase(finditr);
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,2085);//黑名单中不存在该帐号
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJRecvDeleteBlackNameMethod
//
/////////////////////////////////////////
crJXJRecvDeleteBlackNameMethod::crJXJRecvDeleteBlackNameMethod():
	m_netType(GameClient_Game){}
crJXJRecvDeleteBlackNameMethod::crJXJRecvDeleteBlackNameMethod(const crJXJRecvDeleteBlackNameMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvDeleteBlackNameMethod::inputParam(int i, void *param)
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

void crJXJRecvDeleteBlackNameMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvDeleteBlackNameMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			int playerid = m_this -> getPlayerID();
			crData *data = m_this -> getDataClass();
			std::string blackname = m_stream->_readString();
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT16(WCH_LockData,1));
				data->getParam(WCHDATA_JXJBlackNameSet,param);
				BlackNameSet *blacknameset = (BlackNameSet*)param;
				BlackNameSet::iterator finditr = blacknameset->find(blackname);
				if (finditr != blacknameset->end())
				{
					blacknameset->erase(finditr);
				}
				data->excHandle(MAKEINT16(WCH_LockData,0));
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddBlackName,NULL);
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = "删除黑名单";
				GameLogData gamelog(Log_DeleteBlackName,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIUpdateTechLvMethod
//
/////////////////////////////////////////
crJXJUIUpdateTechLvMethod::crJXJUIUpdateTechLvMethod():
	m_this(NULL)
{
}
crJXJUIUpdateTechLvMethod::crJXJUIUpdateTechLvMethod(const crJXJUIUpdateTechLvMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{

}
void crJXJUIUpdateTechLvMethod::inputParam(int i, void *param)
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
void crJXJUIUpdateTechLvMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJUIUpdateTechLvMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this)
	{
		int techid = 0;
		//crData *roledata = playerGameData->getMainRole()->getDataClass();
		crData *data = playerGameData->getDataClass();
		data->excHandle(MAKEINT64(WCH_LockData,1));
		//roledata->excHandle(MAKEINT64(WCH_LockData,1));
		if (data)
		{
			void *param;
			ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
			if (canvas.valid())
			{
				crData *canvasdata = canvas->getDataClass();
				canvasdata->getParam(WCHDATA_JXJShowTechVec,param);
				ShowTechVec *techvec = (ShowTechVec *)param;
				if (m_index < techvec->size())
				{
					techid = (*techvec)[m_index];
				}
			}
			if (techid>0)
			{
				crTableIO::StrVec record;
				ref_ptr<crTableIO>techtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
				int copperindex = techtab->getTitleIndex("铜钱");
				int foodindex = techtab->getTitleIndex("粮食");
				int woodindex = techtab->getTitleIndex("木材");
				int ironindex = techtab->getTitleIndex("铁矿");
				int fubenidindex = techtab->getTitleIndex("前置副本");
				data->getParam(WCHDATA_JXJFubenProgressMap, param);
				FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
				data->getParam(WCHDATA_JXJTechDevelopMap, param);
				TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
				data->getParam(WCHDATA_JXJCoppercash, param);
				int mycopper = *(int *)param;
				data->getParam(WCHDATA_JXJFood, param);
				int myfood = *(int *)param;
				data->getParam(WCHDATA_JXJWood, param);
				int mywood = *(int *)param;
				data->getParam(WCHDATA_JXJIron, param);
				int myiron = *(int *)param;
				crVector4i myresource = crVector4i(mycopper,myfood,mywood,myiron);
				if(techtab->queryOneRecord(0,crArgumentParser::appItoa(techid),record) >= 0)
				{
					bool success = false;
					int tipsindex = 0;
					int fubenid = atoi( record[fubenidindex].c_str());
					int needcopper = atoi( record[copperindex].c_str());
					int needfood = atoi( record[foodindex].c_str());
					int needwood = atoi( record[woodindex].c_str());
					int neediron = atoi( record[ironindex].c_str());
					crVector4i needresource = (crVector4i(needcopper,needfood,needwood,neediron));
					do 
					{
						TechDevelopMap::iterator titr = techdevelopmap->find(techid);
						if (titr != techdevelopmap->end())
						{
							tipsindex = 2093;//在研发中
							break;
						}
						data->getParam(techid, param);
						unsigned char techlv = *(unsigned char *)param;
						if (techlv > 0)
						{
							tipsindex = 2092;//已经研发过了
							break;
						}
						FubenProgressMap::iterator fitr = fubenprogressmap->find(fubenid);
						if (fitr == fubenprogressmap->end() || fitr->second->getComplete() <= 0)
						{
							tipsindex = 2087;//前置副本未通关
							break;
						}
						for (int i = 0; i < 4; i++)
						{
							if (myresource[i] < needresource[i])
							{
								tipsindex = 2088 + i;//资源不足
								break;
							}
						}
						if (tipsindex > 0)
						{
							break;
						}
						//std::string str,str1,str2;
						//for (int i = 0; i < 4; ++i)
						//{
						//	str1 = crArgumentParser::appItoa(needresource[i]) + ",";
						//	str2 = crArgumentParser::appItoa(myresource[i]) + ",";
						//}
						//str = str1 + str2 + crArgumentParser::appItoa(tipsindex);
						//char gbuf[256];
						//sprintf(gbuf,"%s\n\0",str.c_str());
						//gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
						success = true;
					} while (0);
					if (success)
					{
						CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
						stream->createBuf(4);
						stream->_writeInt(techid);
						CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
						CRNetApp::crPlayerDataEventPacket packet;
						CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvUpdateTechLv,stream.get());
						net_conductor->getNetManager()->sendPacket("all",packet);
					}
					else
					{
						playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipsindex,NULL));//
					}
				}
			}
		}
		//roledata->excHandle(MAKEINT64(WCH_LockData,0));
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJRecvUpdateTechLvMethod
//
/////////////////////////////////////////
crJXJRecvUpdateTechLvMethod::crJXJRecvUpdateTechLvMethod():
	m_netType(GameClient_Game){}
crJXJRecvUpdateTechLvMethod::crJXJRecvUpdateTechLvMethod(const crJXJRecvUpdateTechLvMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvUpdateTechLvMethod::inputParam(int i, void *param)
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

void crJXJRecvUpdateTechLvMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUpdateTechLvMethod::operator()(crHandle &handle)
{
	if(m_this && m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			char success = 0;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				int techid = m_stream -> _readInt();
				crData *data = m_this->getDataClass();

				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJFood, param);
				int *myfood = (int *)param;
				data->getParam(WCHDATA_JXJWood, param);
				int *mywood = (int *)param;
				data->getParam(WCHDATA_JXJIron, param);
				int *myiron = (int *)param;
				float needCD = 0.0f;
				if (techid > 0)
				{
					crTableIO::StrVec record;
					ref_ptr<crTableIO>techtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
					int copperindex = techtab->getTitleIndex("铜钱");
					int foodindex = techtab->getTitleIndex("粮食");
					int woodindex = techtab->getTitleIndex("木材");
					int ironindex = techtab->getTitleIndex("铁矿");
					int fubenidindex = techtab->getTitleIndex("前置副本");
					int cdindex = techtab->getTitleIndex("CD");
					data->getParam(WCHDATA_JXJFubenProgressMap, param);
					FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
					data->getParam(WCHDATA_JXJTechDevelopMap, param);
					TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
					data->getParam(WCHDATA_JXJCoppercash, param);
					int mycopper = *(int *)param;
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char viplv = *(unsigned char *)param;
					crVector4i myresource = crVector4i(mycopper,*myfood,*mywood,*myiron);
					if(techtab->queryOneRecord(0,crArgumentParser::appItoa(techid),record) >= 0)
					{
						float reducedcd = (float)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJReduceTechDevelopCD,viplv).c_str()));
						int tipsindex = 0;
						int fubenid = atoi( record[fubenidindex].c_str());
						int needcopper = atoi( record[copperindex].c_str());
						int needfood = atoi( record[foodindex].c_str());
						int needwood = atoi( record[woodindex].c_str());
						int neediron = atoi( record[ironindex].c_str());
						needCD = atof( record[cdindex].c_str()) * (reducedcd * 0.01f);
						crVector4i needresource = (crVector4i(needcopper,needfood,needwood,neediron));
						do 
						{
							TechDevelopMap::iterator titr = techdevelopmap->find(techid);
							if (titr != techdevelopmap->end())
							{
								success = -1;//已在研发中
								break;
							}
							data->getParam(techid, param);
							unsigned char techlv = *(unsigned char *)param;
							if (techlv > 0)
							{
								success = -2;//已经研发过了
								break;
							}
							FubenProgressMap::iterator fitr = fubenprogressmap->find(fubenid);
							if (fitr == fubenprogressmap->end() || fitr->second->getComplete() <= 0)
							{
								success = -3;//前置副本未通关
								break; 
							}
							for (int i = 0; i < 4; i++)
							{
								if (myresource[i] < needresource[i])
								{
									success = -4 - i;//资源不足
									break;
								}
							}
							if (success != 0)
							{
								break;
							}
							success = 1;
							//扣除资源
							m_this->doEvent(WCH_JXJDeductCopperCash,MAKEINT64(&needcopper,NULL));
							*myfood -= needfood;
							*mywood -= needwood;
							*myiron -= neediron;
							techdevelopmap->insert(std::make_pair(techid,needCD));
						} while (0);
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(21);
					stream->_writeChar(success);
					stream->_writeInt(techid);
					stream->_writeFloat(needCD);
					stream->_writeInt(*myfood);
					stream->_writeInt(*mywood);
					stream->_writeInt(*myiron);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUpdateTechLv,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector2i(success,techid));
				GameLogData gamelog(Log_UpgradeTechLv,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char success = m_stream->_readUChar();
			if (success == 1)
			{
				crData *data = m_this->getDataClass();
				int techid = m_stream->_readInt();
				float needCD = m_stream->_readFloat();
				int myfood = m_stream->_readInt();
				int mywood = m_stream->_readInt();
				int myiron = m_stream->_readInt();
				void *param;
				data->getParam(WCHDATA_JXJFood, param);
				int curfood = *(int *)param;
				data->getParam(WCHDATA_JXJWood, param);
				int curwood = *(int *)param;
				data->getParam(WCHDATA_JXJIron, param);
				int curiron = *(int *)param;
				int extra = myfood - curfood;
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Food,extra));
				extra = mywood - curwood;
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Wood,extra));
				extra = myiron - curiron;
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKEINT64(T_Iron,extra));
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJTechDevelopMap, param);
				TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
				techdevelopmap->insert(std::make_pair(techid,needCD));
				data->excHandle(MAKEINT64(WCH_LockData,0));
				data->inputParam(WCHDATA_JXJFood, &curfood);
				data->inputParam(WCHDATA_JXJWood, &curwood);
				data->inputParam(WCHDATA_JXJIron, &curiron);
			}
			else
			{
				int tipsindex = 0;
				switch (tipsindex)
				{
				case -1:
					tipsindex = 2093;
					break;
				case -2:
					tipsindex = 2092;
					break;
				case -3:
					tipsindex = 2087;
					break;
				case -4:
					tipsindex = 2088;
					break;
				case -5:
					tipsindex = 2089;
					break;
				case -6:
					tipsindex = 2090;
					break;
				case -7:
					tipsindex = 2091;
					break;
				default:
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipsindex,NULL));//
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUpdateTechCanvasMethod
//
/////////////////////////////////////////
crJXJUpdateTechCanvasMethod::crJXJUpdateTechCanvasMethod():
	m_this(NULL)
{
	for (int i = 0; i < 7; i++)
	{
		m_lastclienttime[i] = 0;
		m_lastservertime[i] = 0;
	}
}
crJXJUpdateTechCanvasMethod::crJXJUpdateTechCanvasMethod(const crJXJUpdateTechCanvasMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_techpageinput(handle.m_techpageinput)
{
	for (int i = 0; i < 7; i++)
	{
		m_techtitleinput[i] = handle.m_techtitleinput[i];
		m_techdetailinput[i] = handle.m_techdetailinput[i];
		m_techupgradebtn[i] = handle.m_techupgradebtn[i];
		m_techCDinput[i] = handle.m_techCDinput[i];
		m_techCDclearbtn[i] = handle.m_techCDclearbtn[i];
		m_techstatesw[i] = handle.m_techstatesw[i];
		m_lastclienttime[i] = handle.m_lastclienttime[i];
		m_lastservertime[i] = handle.m_lastservertime[i];
		m_techicon[i] = handle.m_techicon[i];
	}
}
void crJXJUpdateTechCanvasMethod::inputParam(int i, void *param)
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
void crJXJUpdateTechCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		m_techtitleinput[i] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		m_techdetailinput[i - 7] = str;
		break;
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
		m_techupgradebtn[i - 14] = str;
		break;
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
		m_techCDinput[i - 21] = str;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		m_techCDclearbtn[i - 28] = str;
		break;
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
	case 40:
	case 41:
		m_techstatesw[i - 35] = str;
		break;
	case 42:
		m_techpageinput = str;
		break;
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
		m_techCDsw[i - 43] = str;
		break;
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
		m_techicon[i - 50] = str;
		break;
	}
}
void crJXJUpdateTechCanvasMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && m_this)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		if (data)
		{
			int disshowcount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTechDisUpgradeShowcount,0).c_str());
			int showcountcurpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJTechShowCountCurPage,0).c_str());
			crData *canvasdata = m_this->getDataClass();
			canvasdata->getParam(WCHDATA_JXJTechCanvasCurPage,param);
			int curpage = *(int *)param;
			canvasdata->excHandle(MAKEINT64(WCH_LockData,1));
			data->excHandle(MAKEINT64(WCH_LockData,1));
			canvasdata->getParam(WCHDATA_JXJShowTechVec,param);
			ShowTechVec *showtechvec = (ShowTechVec *)param;
			showtechvec->clear();
			data->getParam(WCHDATA_JXJFubenProgressMap,param);
			FubenProgressMap *fubenmap = (FubenProgressMap *)param;
			data->getParam(WCHDATA_JXJTechDevelopMap,param);
			TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
			int techid = 0;
			unsigned short profubenid = 0;
			ref_ptr<crButtonWidgetNode> techupgradebtn[7] = {NULL};
			ref_ptr<crButtonWidgetNode> techCDclearbtn[7] = {NULL};
			ref_ptr<crStaticTextWidgetNode> techtitleinput[7] = {NULL};
			ref_ptr<crStaticTextWidgetNode> techdetailinput[7] = {NULL};
			ref_ptr<crStaticTextWidgetNode> techCDinput[7] = {NULL};
			ref_ptr<crImageBoxWidgetNode> techicon[7] = {NULL};
			crMultiSwitch *techstatesw[7] = {NULL};
			crMultiSwitch *techCDsw[7] = {NULL};
			for (int i = 0; i < 7; i++)
			{
				techupgradebtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_techupgradebtn[i]));
				techCDclearbtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_techCDclearbtn[i]));
				techtitleinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_techtitleinput[i]));
				techdetailinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_techdetailinput[i]));
				techCDinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_techCDinput[i]));
				techicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_techicon[i]));
				techstatesw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_techstatesw[i]));
				techCDsw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_techCDsw[i]));
				if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(false);
				if (techCDclearbtn[i].valid())techCDclearbtn[i]->setVisiable(false);
				if (techtitleinput[i].valid())techtitleinput[i]->clearString();
				if (techdetailinput[i].valid())techdetailinput[i]->clearString();
				if (techCDinput[i].valid())techCDinput[i]->clearString();
				if (techicon[i].valid())techicon[i]->setVisiable(false);
				if (techstatesw[i])techstatesw[i]->setActiveSwitchSet(0);
				if (techCDsw[i])techCDsw[i]->setActiveSwitchSet(1);
			}
			FubenProgressMap::iterator fitr;
			TechDevelopMap::iterator titr;
			std::vector <int> upgradevec;
			std::vector <int> disupgradevec;
			crTableIO::StrVec record;
			crTableIO::StrVec frecord;
			ref_ptr<crTableIO>techtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
			ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			int nameindex = techtab->getTitleIndex("name");
			int infoindex = techtab->getTitleIndex("描述");
			int proindex = techtab->getTitleIndex("前置副本");
			int fnameindex = fubentab->getTitleIndex("name");
			int iconindex = techtab->getTitleIndex("icon");
			for (int i = 0; i < techtab->getRowCount();i++)
			{
				techid = atoi((*techtab)(i,0).c_str());
				if (techid <= 0)
				{
					continue;
				}
				profubenid = atoi((*techtab)(i,proindex).c_str());
				if(profubenid > 0)
				{
					fitr = fubenmap->find(profubenid);
					if (fitr != fubenmap->end() && fitr->second->getComplete() > 0)
					{
						upgradevec.push_back(techid);
					}
					else
					{
						if (disupgradevec.size() < disshowcount)
						{
							disupgradevec.push_back(techid);
						}
					}
				}
			}
			int upgradeindex = 0;
			unsigned char techlv = 0;
			int times = 0;
			int hour = 0;
			int t = 0;
			int minute = 0;
			int s = 0;
			bool showtitle = true;
			std::string title;
			for (int i = 0; i < 7; i++)
			{
				showtitle = true;
				techid = 0;
				if (techCDclearbtn[i].valid()){techCDclearbtn[i]->setVisiable(false);}
				if(techtitleinput[i].valid())techtitleinput[i]->clearString();
				if(techdetailinput[i].valid())techdetailinput[i]->clearString();
				if (techCDinput[i].valid())techCDinput[i]->clearString();
				if (curpage == 1)
				{
					if ( i < disupgradevec.size() )
					{
						techid = disupgradevec[disupgradevec.size() - i - 1];
						if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(false);
						showtechvec->push_back(techid);
						showtitle = false;
					}
					else
					{
						upgradeindex = i - disupgradevec.size();
						if (upgradeindex < upgradevec.size())
						{
							if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(true);
							if (techupgradebtn[i].valid())techupgradebtn[i]->setEnable(true);
							techid = upgradevec[upgradevec.size() - upgradeindex - 1];
							showtechvec->push_back(techid);
						}
					}
				}
				else
				{
					upgradeindex = showcountcurpage * (curpage - 1) - disupgradevec.size() + i;
					if (upgradeindex < upgradevec.size())
					{
						if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(true);
						if (techupgradebtn[i].valid())techupgradebtn[i]->setEnable(true);
						techid = upgradevec[upgradevec.size() - upgradeindex - 1];
						showtechvec->push_back(techid);
					}
				}
				if (techid > 0 && techtab->queryOneRecord(0,crArgumentParser::appItoa(techid),record) >= 0)
				{
					//if (techupgradebtn[i])techupgradebtn[i]->setVisiable(true);
					if (showtitle)
					{
						title = record[infoindex];
					}
					else
					{
						if(fubentab->queryOneRecord(0,record[proindex],frecord) >= 0)
						{
							title = "通关副本【"+ frecord[fnameindex] +"】后开启";
						}
					}
					if (techicon[i].valid() && iconindex > 0)
					{
						techicon[i]->setVisiable(true);
						techicon[i]->setImageName(record[iconindex]);
					}
					if(techtitleinput[i].valid())techtitleinput[i]->setString(record[nameindex]);
					if(techdetailinput[i].valid())techdetailinput[i]->setString(title);
					data->getParam(techid,param);//WCHDATA_JXJCardImpression2CritTechLv
					techlv = *(unsigned char *)param;
					if (techlv > 0)
					{
						if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(false);
						//if (techCDinput[i])techCDinput[i]->setString(std::string("已研发"));
						if (techstatesw[i])techstatesw[i] -> setActiveSwitchSet(1);
					}
					else
					{
						if (techstatesw[i])techstatesw[i] -> setActiveSwitchSet(0);
					}
					titr = techdevelopmap->find(techid);
					if (titr != techdevelopmap->end())
					{
						times = (int)titr->second;
						if (m_lastservertime[i] != times)
						{
							m_lastservertime[i] = times;
							m_lastclienttime[i] = times;
						}
						else
						{
							times = m_lastclienttime[i] - 1;
							m_lastclienttime[i] = times;
						}
						if (times < 0)
						{
							times = 0;
						}
						hour = times/3600;
						t = times%3600;
						minute = t/60;
						s = t%60;
						if (techCDsw[i])techCDsw[i]->setActiveSwitchSet(0);
						if (techupgradebtn[i].valid())techupgradebtn[i]->setVisiable(true);
						if (techupgradebtn[i].valid())techupgradebtn[i]->setEnable(false);
						if (techCDclearbtn[i].valid())techCDclearbtn[i]->setVisiable(true);
						if (techCDinput[i].valid())
						{
							techCDinput[i]->setString(crArgumentParser::appItoa(hour) + ":" + crArgumentParser::appItoa(minute) + ":" + crArgumentParser::appItoa(s));
						}
					}
				}
			}
			ref_ptr<crStaticTextWidgetNode> techpageinput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_techpageinput));
			int totalpage = ceil((float)(disupgradevec.size() + upgradevec.size())/(float)showcountcurpage);
			if (totalpage < 1)
			{
				totalpage = 1;
			}
			if (techpageinput.valid())
			{
				techpageinput->setString(crArgumentParser::appItoa(curpage) + "/" + crArgumentParser::appItoa(totalpage));
			}
			canvasdata->inputParam(WCHDATA_JXJTechCanvasTotalPage, &totalpage);
			data->excHandle(MAKEINT64(WCH_LockData,0));
			canvasdata->excHandle(MAKEINT64(WCH_LockData,0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJTechDevelopUpdateMethod
//
/////////////////////////////////////////
crJXJTechDevelopUpdateMethod::crJXJTechDevelopUpdateMethod():
	m_dt(0),
	m_delta(0.0f)
{
}
crJXJTechDevelopUpdateMethod::crJXJTechDevelopUpdateMethod(const crJXJTechDevelopUpdateMethod& handle):
	crMethod(handle),
	m_dt(0),
	m_delta(0.0f)
{
}
void crJXJTechDevelopUpdateMethod::inputParam(int i, void *param)
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
void crJXJTechDevelopUpdateMethod::addParam(int i, const std::string& str)
{
}
void crJXJTechDevelopUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if(playerData.valid())
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			unsigned char techlv = 1;
			int dt = m_dt;
			data->getParam(WCHDATA_JXJTechDevelopMap,param);
			TechDevelopMap *techdevelopMap = (TechDevelopMap *)param;
			for (TechDevelopMap::iterator itr = techdevelopMap->begin();
				itr != techdevelopMap->end();)
			{
				if(itr->second > dt)
				{
					itr->second -= dt;
				}
				else
				{
					data->inputParam(itr->first,&techlv);

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(itr->first);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvTechDevelopUpdate,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);

					itr = techdevelopMap->erase(itr);
					continue;
				}
				++itr;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvTechDevelopUpdateMethod
//
/////////////////////////////////////////
crJXJRecvTechDevelopUpdateMethod::crJXJRecvTechDevelopUpdateMethod():
	m_netType(GameClient_Game){}
crJXJRecvTechDevelopUpdateMethod::crJXJRecvTechDevelopUpdateMethod(const crJXJRecvTechDevelopUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvTechDevelopUpdateMethod::inputParam(int i, void *param)
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

void crJXJRecvTechDevelopUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvTechDevelopUpdateMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char techlv = 1;
			crData *data = m_this->getDataClass();
			int techid = m_stream->_readInt();
			data->inputParam(techid,&techlv);
			data->getParam(WCHDATA_JXJTechDevelopMap,param);
			TechDevelopMap *techdevelopMap = (TechDevelopMap *)param;
			TechDevelopMap::iterator itr = techdevelopMap->find(techid);
			if(itr != techdevelopMap->end())
			{
				techdevelopMap->erase(itr);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIClearTechDevelopTimeMethod
//
/////////////////////////////////////////
crJXJUIClearTechDevelopTimeMethod::crJXJUIClearTechDevelopTimeMethod():
	m_index(0)
{
}
crJXJUIClearTechDevelopTimeMethod::crJXJUIClearTechDevelopTimeMethod(const crJXJUIClearTechDevelopTimeMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_msgbox(handle.m_msgbox),
	m_title(handle.m_title),
	m_msg(handle.m_msg),
	m_ok(handle.m_ok),
	m_index(handle.m_index)
{
}
void crJXJUIClearTechDevelopTimeMethod::inputParam(int i, void *param)
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
void crJXJUIClearTechDevelopTimeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_msgbox = str;
		break;
	case 2:
		m_title = str;
		break;
	case 3:
		m_msg = str;
		break;
	case 4:
		m_ok = str;
		break;
	case 5:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIClearTechDevelopTimeMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && myPlayer->getDataClass())
	{
		void *param;
		bool canbuy = false;
		int needgold= 0;
		//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crData *playerData = myPlayer->getDataClass();
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if(canvas.valid() && canvas->getDataClass())
		{
			int techid = 0;
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int curgoldmin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCurTechCDMinCostGold,viplv).c_str());
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJShowTechVec,param);
			ShowTechVec *showtechvec = (ShowTechVec *)param;
			if (m_index < showtechvec->size())
			{
				techid = (*showtechvec)[m_index];
			}
			if (techid > 0)
			{
				playerData->getParam(WCHDATA_JXJTechDevelopMap,param);
				TechDevelopMap *developmap = (TechDevelopMap *)param;
				TechDevelopMap::iterator fitr = developmap->find(techid);
				if (fitr != developmap->end())
				{
					int cleartime = fitr->second;
					needgold = (1 + cleartime/60) * curgoldmin;
					canvasdata->inputParam(WCHDATA_JXJClearTechDevelopTimeNeedGold,&needgold);
					canvasdata->inputParam(WCHDATA_JXJClearTechDevelopID,&techid);

					playerData->getParam(WCHDATA_JXJGoldingot,param);
					int goldingot = *(int*)param;
					playerData->getParam(WCHDATA_JXJGiftGold,param);
					int giftgold = *(int*)param;
					goldingot += giftgold;
					if (goldingot >= needgold)
					{
						canbuy = true;
					}
				}
			}
		}
		if (canbuy)
		{
			playerData->getParam(WCHDATA_JXJGoldCostHintInt,param);
			unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_TechDevelopCDHint;
			ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
			ref_ptr<crStaticTextWidgetNode> title = dynamic_cast<crStaticTextWidgetNode *>(msgCanvas->getWidget(m_title));
			ref_ptr<crHypertextWidgetNode> msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msg));
			ref_ptr<crButtonWidgetNode> okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_ok));
			if (ifhint)
			{
				int hinttype = JXJGoldCostType_TechDevelopCDHint;
				crData *braindata = crBrain::getInstance()->getDataClass();
				braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text","您确认要花费");
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content",2);
				v_i.clear();
				v_i.push_back(140.0);
				v_i.push_back(40.0);
				v_i.push_back(40.0);
				v_i.push_back(255.0);
				cfg_script.Add("Color",v_i);
				char tmpText[20];
				sprintf(tmpText,"【%d礼金/元宝】\0",needgold);
				cfg_script.Add("Text",tmpText);
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content",3);
				cfg_script.Add("Text","立刻完成该项科技吗？");
				cfg_script.Pop();

				cfg_script.Pop();
				if(msg.valid())msg->setHypertext(cfg_script);
			}
			if(okBtn.valid())okBtn->setEnable(true);
			if (msgCanvas.valid())
			{
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
					if(okBtn.valid())okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				}
			}
		}
		else
		{
			int tipsindex = 2002;
			if (needgold == 0)
			{
				tipsindex = 2097;
			}
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipsindex,NULL));//
		}
	}
}
/////////////////////////////////////////
//
//crJXJClearTechDevelopTimeMethod
//
/////////////////////////////////////////
crJXJClearTechDevelopTimeMethod::crJXJClearTechDevelopTimeMethod():
	m_this(NULL)
{
}
crJXJClearTechDevelopTimeMethod::crJXJClearTechDevelopTimeMethod(const crJXJClearTechDevelopTimeMethod& handle):
	crMethod(handle)
{
}
void crJXJClearTechDevelopTimeMethod::inputParam(int i, void *param)
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
void crJXJClearTechDevelopTimeMethod::addParam(int i, const std::string& str)
{
}
void crJXJClearTechDevelopTimeMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		crData *canvasdata = canvas->getDataClass();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(netConductor)
		{
			canvasdata->getParam(WCHDATA_JXJClearTechDevelopTimeNeedGold,param);
			int goldneed = *(int *)param;
			canvasdata->getParam(WCHDATA_JXJClearTechDevelopID,param);
			int techid = *(int *)param;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(8);
			stream->_writeInt(techid);
			stream->_writeInt(goldneed);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvClearTechDevelopTime,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvClearTechDevelopTimeMethod
//
/////////////////////////////////////////
crJXJRecvClearTechDevelopTimeMethod::crJXJRecvClearTechDevelopTimeMethod():
	m_netType(GameClient_Game){}
crJXJRecvClearTechDevelopTimeMethod::crJXJRecvClearTechDevelopTimeMethod(const crJXJRecvClearTechDevelopTimeMethod& handle)
{
}
void crJXJRecvClearTechDevelopTimeMethod::inputParam(int i, void *param)
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

void crJXJRecvClearTechDevelopTimeMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvClearTechDevelopTimeMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				unsigned char success = 0;
				void *param;
				int techid = m_stream->_readInt();
				int goldneed = m_stream->_readInt();
				int goldneedsever = 0;
				bool needdeductgold = false;
				unsigned char techlv = 1;
				m_this->getDataClass()->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int curgoldmin = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCurTechCDMinCostGold,viplv).c_str());
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJTechDevelopMap,param);
				TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
				TechDevelopMap::iterator itr;
				itr = techdevelopmap->find(techid);
				if (itr!=techdevelopmap->end())
				{
					goldneedsever = (itr->second/60 + 1) * curgoldmin;
					if (goldneed >= goldneedsever - curgoldmin)
					{
						MoneyChangeData moneydata(goldneed,"秒科技研发cd");
						m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
						if (moneydata.first == 0)
						{
							techdevelopmap->erase(itr);
							data->inputParam(techid,&techlv);
							success = 1;//秒时间成功
						}
					}
				}
				else
				{
					//	MoneyChangeData moneydata(goldneed,"秒逃兵时间");
					//	m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
					//	if (moneydata.first == 0)
					//	{
					success = 1;
					//	}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if (success == 1)
				{
					if (needdeductgold)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
						callback->savePlayerData(playerData.get());
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(5);
					stream->_writeUChar(success);
					stream->_writeInt(techid);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeUChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvClearTechDevelopTime,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_ClearPunishTime,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char success = m_stream -> _readUChar();
			int tipsindex = 2002;
			if (success == 1)
			{
				tipsindex = 2096;
				unsigned char techlv = 1;
				int techid = m_stream->_readInt();
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJTechDevelopMap,param);
				TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
				TechDevelopMap::iterator fitr = techdevelopmap->find(techid);
				if (fitr != techdevelopmap->end())
				{
					techdevelopmap->erase(fitr);
				}
				data->inputParam(techid,&techlv);
			}
			//策划要求删除清除成功提示信息
			//m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipsindex,NULL));//
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetMaxArmyCountMethod
//
/////////////////////////////////////////
crJXJSetMaxArmyCountMethod::crJXJSetMaxArmyCountMethod()
{
}
crJXJSetMaxArmyCountMethod::crJXJSetMaxArmyCountMethod(const crJXJSetMaxArmyCountMethod& handle):
	crMethod(handle),
	m_formationcanvas(handle.m_formationcanvas)
{
	for (int i = 0; i < 3;i++)
	{
		m_countinput[i] = handle.m_countinput[i];
	}
}
void crJXJSetMaxArmyCountMethod::inputParam(int i, void *param)
{
}
void crJXJSetMaxArmyCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formationcanvas = str;
		break;
	case 1:
	case 2:
	case 3:
		m_countinput[i - 1] = str;
		break;
	default:
		break;
	}
}
void crJXJSetMaxArmyCountMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_formationcanvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		bool canset = true;
		int temppagenum = 0;
		ref_ptr<crTableIO>troopstable = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);	// troops table 
		crTableIO::StrVec record;
		int curlead = troopstable->getTitleIndex("领导力");
		ref_ptr<crStaticTextWidgetNode> countinput[3] = {NULL};
		ref_ptr<crRadioGroupWidgetNode> pageradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget("FormationRaido2"));
		if (pageradio.valid())
		{
			temppagenum = pageradio->getSelect();
		}
		for (int i = 0; i < 3; i++)
		{
			countinput[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_countinput[i]));
			if (countinput[i].valid())
			{
				countinput[i]->clearString();
			}
			else
			{
				canset = false;
			}
		}
		if (canset)
		{
			// PlayerGameData
			crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
			crData *data = player->getDataClass();										// player data
			crData *roledata = player->getMainRole()->getDataClass();						// player role data

			int curtroopstotal = 0;
			int curtroopsother = 0; 

			int totallead = 0;
			player->doEvent(WCH_JXJFormationGetLeaderPower,MAKEINT64(&totallead,NULL));
			data->getParam(WCHDATA_JXJFormationInfoVec, param);
			FormationInfoVec *formations = (FormationInfoVec *)param;
			data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
			SavedFormationInfoVec *savedtroop_infors = (SavedFormationInfoVec *)param;
			data->getParam(WCHDATA_JXJTroopsMap, param);
			TroopsMap *troops =	(TroopsMap *)param;			

			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			crData *canvasdata = canvas->getDataClass();
			canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
			FormationInfoVec *tempformation = (FormationInfoVec *)param;
			int curtroopsselect = 0;//int(m_this->getValue());
			int curabstractid = 0;
			int perlead = 0;
			int maxtroopsselect = 0;
			int power = 0;
			int totalpower = 0;
			crVector4i formation4i;
			crVector2i equip;
			for (int index = 0; index < tempformation->size(); index++)
			{
				formation4i = crVector4i(0,0,0,0);
				equip = crVector2i(0,0);
				curtroopsother = 0;
				perlead = 1;
				if ((*tempformation)[index] == NULL)
				{
					continue;
				}
				curabstractid = (*tempformation)[index]->getAbstractID();
				equip = (*tempformation)[index]->getEquips();
				if(curabstractid <= 0)
				{
					continue;
				}
				curtroopstotal = 0;
				TroopsMap::iterator troopitr = troops->find(curabstractid);
				if (troopitr != troops->end())
				{
					curtroopstotal = troopitr->second;
				}
				int fsize = tempformation->size();
				for(int i = 0; i< 3; i++)
				{
					if ((*tempformation)[i]!=NULL)
					{
						if(i != index && (*tempformation)[i]->getAbstractID() == curabstractid)
						{
							if(countinput[i].valid())curtroopsother += (*tempformation)[i]->getCount();
						}
					}
					if (i<fsize && temppagenum < savedtroop_infors->size() && i < (*savedtroop_infors)[temppagenum].second.size() && (*savedtroop_infors)[temppagenum].second[i]!=NULL)
					{
						if((*savedtroop_infors)[temppagenum].second[i]->getAbstractID() == curabstractid)
						{
							curtroopstotal += (*savedtroop_infors)[temppagenum].second[i]->getCount();
						}
					}
				}

				curtroopsselect = curtroopstotal - curtroopsother;
				if (curtroopsselect < 0)
				{
					curtroopsselect = 0;
				}
				if (troopstable->queryOneRecord(0,crArgumentParser::appItoa(curabstractid),record) >= 0)
				{
					perlead = atoi(record[curlead].c_str());
				}
				if (perlead <= 0)
				{
					perlead = 1;
				}
				maxtroopsselect = (totallead / 3) / perlead;
				if (curtroopsselect >maxtroopsselect )
				{
					curtroopsselect = maxtroopsselect;
				}
				if (curtroopsselect != maxtroopsselect)
				{
					countinput[index]->setColor(FCOLOR_RED);
				}
				else
				{
					countinput[index]->setColor(FCOLOR_WHITE);
				}
				countinput[index]->setString(crArgumentParser::appItoa(curtroopsselect) + "/" +crArgumentParser::appItoa(maxtroopsselect));
				(*tempformation)[index]->setCount(curtroopsselect);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetArmyPowerMethod
//
/////////////////////////////////////////
crJXJSetArmyPowerMethod::crJXJSetArmyPowerMethod()
{
}
crJXJSetArmyPowerMethod::crJXJSetArmyPowerMethod(const crJXJSetArmyPowerMethod& handle):
	crMethod(handle),
	m_formationcanvas(handle.m_formationcanvas)
{
	for (int i = 0; i < 3;i++)
	{
		m_powerinput[i] = handle.m_powerinput[i];
	}
}
void crJXJSetArmyPowerMethod::inputParam(int i, void *param)
{
}
void crJXJSetArmyPowerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formationcanvas = str;
		break;
	case 1:
	case 2:
	case 3:
		m_powerinput[i - 1] = str;
		break;
	case 4:
		m_totalpowerinput = str;
		break;
	default:
		break;
	}
}
void crJXJSetArmyPowerMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_formationcanvas);
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		ref_ptr<crStaticTextWidgetNode> powerinput[3] = {NULL};
		for (int i = 0; i < 3; i++)
		{
			powerinput[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_powerinput[i]));
			if (powerinput[i].valid())
			{
				powerinput[i]->clearString();
			}
		}
		ref_ptr<crStaticTextWidgetNode> totalpowerinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_totalpowerinput));
		if (totalpowerinput.valid())
		{
			totalpowerinput->clearString();
		}
			// PlayerGameData
		crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData(); // player entity
	
		crData *canvasdata = canvas->getDataClass();
		canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
		FormationInfoVec *tempformation = (FormationInfoVec *)param;
		int curtroopsselect = 0;//int(m_this->getValue());
		int curabstractid = 0;
		crVector2i power = crVector2i(0,0);
		int totalpower = 0;
		crVector4i formation4i;
		crVector2i equip;
		canvasdata->getParam(WCHDATA_JXJFormationTempRoleMap,param);
		TempRoleMap *roleMap = (TempRoleMap *)param;
		roleMap->clear();
		for (int index = 0; index < tempformation->size(); index++)
		{
			curtroopsselect = 0;
			formation4i = crVector4i(0,0,0,0);
			equip = crVector2i(0,0);
			if ((*tempformation)[index] == NULL)
			{
				continue;
			}
			curabstractid = (*tempformation)[index]->getAbstractID();
			equip = (*tempformation)[index]->getEquips();
			if(curabstractid <= 0)
			{
				continue;
			}
			if ((*tempformation)[index]->getCount() != 0)
			{
				curtroopsselect = (*tempformation)[index]->getCount();
				formation4i = crVector4i(curabstractid,curtroopsselect,equip[0],equip[1]);
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				power[1] = index;
				
				myPlayer->doEvent(WCH_JXJCalculatePower,MAKEINT64(&formation4i,&power));
			}
			if (powerinput[index].valid())
			{
				powerinput[index]->setString(crArgumentParser::appItoa(power[0]));
			}
			totalpower += power[0];
		}
		if (totalpowerinput.valid())
		{
			totalpowerinput->setString(crArgumentParser::appItoa(totalpower));
		}
	}
}
/////////////////////////////////////////
//
//crJXJChangeSavedFormationVecMethod
//
/////////////////////////////////////////
crJXJChangeSavedFormationVecMethod::crJXJChangeSavedFormationVecMethod():
	m_index(0)
{
}
crJXJChangeSavedFormationVecMethod::crJXJChangeSavedFormationVecMethod(const crJXJChangeSavedFormationVecMethod& handle):
	crMethod(handle),
	m_formationcanvas(handle.m_formationcanvas),
	m_index(handle.m_index)
{
}
void crJXJChangeSavedFormationVecMethod::inputParam(int i, void *param)
{
}
void crJXJChangeSavedFormationVecMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formationcanvas = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJChangeSavedFormationVecMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_formationcanvas);
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		crData *data = myplayer->getDataClass();
		crData *canvasdata = canvas->getDataClass();

		data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
		SavedFormationInfoVec *savedtempformationvec = (SavedFormationInfoVec *)param;
		canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
		FormationInfoVec *tempformationvec	= (FormationInfoVec *)param;
		tempformationvec->clear();
		for (FormationInfoVec::iterator itr = (*savedtempformationvec)[m_index].second.begin();
			itr !=(*savedtempformationvec)[m_index].second.end();
			++itr)
		{
			tempformationvec->push_back(*itr);
		}
	}
}
/////////////////////////////////////////
//
//crJXJResetTempFormationVecMethod
//
/////////////////////////////////////////
crJXJResetTempFormationVecMethod::crJXJResetTempFormationVecMethod()
{
}
crJXJResetTempFormationVecMethod::crJXJResetTempFormationVecMethod(const crJXJResetTempFormationVecMethod& handle):
	crMethod(handle),
	m_formationcanvas(handle.m_formationcanvas),
	m_savedradio(handle.m_savedradio)
{
	for (int i = 0; i < 4; ++i)
	{
		m_inputsw[i] = handle.m_inputsw[i];
	}
}
void crJXJResetTempFormationVecMethod::inputParam(int i, void *param)
{
}
void crJXJResetTempFormationVecMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formationcanvas = str;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		m_inputsw[i - 1] = str;
		break;
	case 5:
		m_savedradio = str;
		break;
	default:
		break;
	}
}
void crJXJResetTempFormationVecMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_formationcanvas);
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		crData *data = myplayer->getDataClass();
		crData *canvasdata = canvas->getDataClass();

		canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
		FormationInfoVec *tempformationvec	= (FormationInfoVec *)param;
		data->getParam(WCHDATA_JXJFormationInfoVec, param);
		FormationInfoVec *formationvec	= (FormationInfoVec *)param;
		data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
		SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
		int j = 0;
		for (FormationInfoVec::iterator itr = tempformationvec->begin(); itr != tempformationvec->end() && j< 3; itr++,j++)
		{
			if (itr->valid())
			{
				(*itr)->setAbstractID(-1);
				(*itr)->setCount(0);
				(*itr)->setEquips(crVector2i(-1,-1));
			}
			else
			{
				ref_ptr<crFormationInfo> newformation = new crFormationInfo;
				newformation->setAbstractID(-1);
				newformation->setCount(0);
				newformation->setEquips(crVector2i(-1,-1));
				(*itr) = newformation;
			}
		}		
		crMultiSwitch *inputsw[4] = {NULL};
		for (int i = 0; i < 4; i++)
		{
			inputsw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_inputsw[i]));
			if(inputsw[i])inputsw[i]->setActiveSwitchSet(1);
		}
		unsigned char pageindex = 0;
		data->getParam(WCHDATA_JXJSavedFormationIndex, param);
		pageindex = *(unsigned char *)param;
		
		//int pageindexadd = 0;
		//for (SavedFormationInfoVec::iterator itr = savedformationvec->begin();
		//	itr != savedformationvec->end();
		//	++itr,++pageindexadd)
		//{
		//	j = 0;
		//	for(FormationInfoVec::iterator itr2 = formationvec->begin(); itr2 != formationvec->end() && j< itr->second.size(); itr2++,j++)
		//	{
		//		JXJ::crFormationInfo *formation = itr2->get();
		//		if (!formation || itr->second.empty())
		//		{
		//			continue;
		//		}
		//		if (formation->getAbstractID() != itr->second[j]->getAbstractID() || formation->getEquips() != itr->second[j]->getEquips())
		//		{
		//			continue;
		//		}
		//		pageindex = pageindexadd;
		//	}
		//	if (pageindex >= 0)
		//	{
		//		break;
		//	}
		//}
		j = 0;
		//if (pageindex >= 0)
		//{
		//	for(FormationInfoVec::iterator itr = formationvec->begin(); itr != formationvec->end() && j< 3; itr++,j++)
		//	{
		//		JXJ::crFormationInfo *formation = itr->get();
		//		ref_ptr<crFormationInfo> newformation = new crFormationInfo;
		//		newformation->setAbstractID(formation->getAbstractID());
		//		newformation->setCount(formation->getCount());
		//		newformation->setEquips(formation->getEquips());
		//		(*tempformationvec)[j] = newformation;
		//	}
		//}
		//else
		{
			if (savedformationvec->size() > pageindex && (*savedformationvec)[pageindex].second.size() > 0)
			{
				for(FormationInfoVec::iterator itr = (*savedformationvec)[pageindex].second.begin(); itr != (*savedformationvec)[pageindex].second.end() && j< 3; itr++,j++)
				{
					JXJ::crFormationInfo *formation = itr->get();
					ref_ptr<crFormationInfo> newformation = new crFormationInfo;
					newformation->setAbstractID(formation->getAbstractID());
					newformation->setCount(formation->getCount());
					newformation->setEquips(formation->getEquips());
					(*tempformationvec)[j] = newformation;
				}
			}
		}
		ref_ptr<crRadioGroupWidgetNode> savedradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getChildNode(m_savedradio));
		if (savedradio.valid())
		{
			savedradio->select(pageindex);
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetTempFormationVecMethod
//
/////////////////////////////////////////
crJXJSetTempFormationVecMethod::crJXJSetTempFormationVecMethod()
{
}
crJXJSetTempFormationVecMethod::crJXJSetTempFormationVecMethod(const crJXJSetTempFormationVecMethod& handle):
	crMethod(handle),
	m_formationcanvas(handle.m_formationcanvas),
	m_inputradio(handle.m_inputradio)
{
}
void crJXJSetTempFormationVecMethod::inputParam(int i, void *param)
{
}
void crJXJSetTempFormationVecMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_formationcanvas = str;
		break;
	case 1:
		m_inputradio = str;
		break;
	default:
		break;
	}
}
void crJXJSetTempFormationVecMethod::operator()(crHandle &handle)
{	
	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_formationcanvas);
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (canvas.valid() && canvas->getDataClass())
	{
		void *param;
		crData *data = myplayer->getDataClass();
		crData *canvasdata = canvas->getDataClass();

		canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
		FormationInfoVec *tempformationvec	= (FormationInfoVec *)param;
		data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
		SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
		ref_ptr<crRadioGroupWidgetNode> inputradio = dynamic_cast<crRadioGroupWidgetNode *>(canvas -> getWidget(m_inputradio));
		int index = 0;
		if (inputradio.valid())
		{
			index = inputradio->getSelect();
			crRole* pMainRole = myplayer->getMainRole();
			crData* pRoleData = pMainRole?pMainRole->getDataClass():NULL;
			//切换绑定的装备
			if(pRoleData)
			{
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int maxPage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxEquipVec,viplv).c_str());
				if (index>=0 && index<maxPage)
				{
					data->getParam(WCHDATA_JXJEquipBindInfo,param);
					int equipBindInfo = *(int*)param;
					
					unsigned char index_[4];//绑定的装备索引
					index_[0] = LOINT16(LOINT32(equipBindInfo));
					index_[1] = HIINT16(LOINT32(equipBindInfo));
					index_[2] = LOINT16(HIINT32(equipBindInfo));
					index_[3] = HIINT16(HIINT32(equipBindInfo));
					unsigned char select = index_[index];
					pRoleData->getParam(WCHDATA_JXJCurrentEquipVecIndex,param);
					unsigned char currentIndex = *(unsigned char*)param;
					pRoleData->inputParam(WCHDATA_JXJCurrentEquipVecIndex,&select);
					CRNet::crNetConductor	*netConductor	= CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
					CRCore::ref_ptr<CRCore::crStreamBuf> stream = new CRCore::crStreamBuf;
					if(netConductor && stream.valid())
					{
						stream->createBuf(1);
						stream->_writeUChar(select);
						CRNetApp::crPlayerDataEventPacket packet;
						CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSwitchEquipPage,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}		
				}
			}
		}
		for(FormationInfoVec::iterator itr = tempformationvec->begin(); itr != tempformationvec->end(); ++itr)
		{
			itr->get()->setAbstractID(-1);
			itr->get()->setEquips(crVector2i(-1,-1));
		}
		if (!savedformationvec->empty())
		{
			if (index < savedformationvec->size())
			{
				FormationInfoVec formationvec = (*savedformationvec)[index].second;
				int j = 0;
				for(FormationInfoVec::iterator itr = formationvec.begin(); itr != formationvec.end() && j< 3; itr++,j++)
				{
					JXJ::crFormationInfo *formation = itr->get();
					ref_ptr<crFormationInfo> newformation = new crFormationInfo;
					newformation->setAbstractID(formation->getAbstractID());
					newformation->setCount(formation->getCount());
					newformation->setEquips(formation->getEquips());
					(*tempformationvec)[j] = newformation;
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISetTempFormationMethod
//
/////////////////////////////////////////
crJXJUISetTempFormationMethod::crJXJUISetTempFormationMethod():
	m_this(NULL)
{
}
crJXJUISetTempFormationMethod::crJXJUISetTempFormationMethod(const crJXJUISetTempFormationMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
	for (int i = 0; i < 4; ++i)
	{
		m_nameedit[i] = handle.m_nameedit[i];
		m_editbtn[i] = handle.m_editbtn[i];
		m_addbtn[i] = handle.m_addbtn[i];
		m_backboard[i] = handle.m_backboard[i];
		m_nameinput[i] = handle.m_nameinput[i];
		m_nameradio[i] = handle.m_nameradio[i];
	}
}
void crJXJUISetTempFormationMethod::inputParam(int i, void *param)
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
void crJXJUISetTempFormationMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_nameedit[i] = str;
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		m_editbtn[i - 4] = str;
		break;
	case 8:
	case 9:
	case 10:
	case 11:
		m_addbtn[i - 8] = str;
		break;
	case 12:
	case 13:
	case 14:
	case 15:
		m_backboard[i - 12] = str;
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		m_nameinput[i - 16] = str;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
		m_nameradio[i - 20] = str;
		break;
	default:
		break;
	}
}
void crJXJUISetTempFormationMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this/* && m_this->getDataClass()*/)
	{
		void *param;
		crData *data = myplayer->getDataClass();
		//crData *canvasdata = canvas->getDataClass();

		//canvasdata->getParam(WCHDATA_JXJTempFormationVec, param);
		//FormationInfoVec *tempformationvec	= (FormationInfoVec *)param;
		data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
		SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
		data->getParam(WCHDATA_JXJOpenSaveFormationInfo, param);
		unsigned char opensavedformation = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char viplv = *(unsigned char *)param;

		unsigned char savedformationmax = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,viplv).c_str()));
		savedformationmax = CRCore::maximum(savedformationmax,(unsigned char)4);
		ref_ptr<crStaticTextWidgetNode> nameinput[4] = {NULL};
		ref_ptr<crEditWidgetNode> nameedit[4] = {NULL};
		ref_ptr<crButtonWidgetNode> editbtn[4] = {NULL};
		ref_ptr<crButtonWidgetNode> addbtn[4] = {NULL};
		ref_ptr<crRadioWidgetNode> nameradio[4] = {NULL};
		ref_ptr<crNode> backboard[4] = {NULL};
		for (int i = 0; i < 4; ++i)
		{
			nameinput[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_nameinput[i]));
			nameedit[i] = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_nameedit[i]));
			editbtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_editbtn[i]));
			addbtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_addbtn[i]));
			nameradio[i] = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_nameradio[i]));
			backboard[i] = m_this->getChildNode(m_backboard[i]);
			if(nameinput[i].valid())
			{
				nameinput[i]->clearString();
				nameinput[i]->setVisiable(false);
			}
			if(nameedit[i].valid())
			{
				nameedit[i]->clearString();
				nameedit[i]->setVisiable(false);
			}
			if(editbtn[i].valid())editbtn[i]->setVisiable(false);
			if(addbtn[i].valid())addbtn[i]->setVisiable(false);
			if(backboard[i].valid())backboard[i]->setVisiable(false);
			if (nameradio[i].valid())nameradio[i]->setVisiable(false);
		}
		int index = 0;
		std::string name;
		int ifopen = 0;
		for (;index < savedformationmax; ++index)
		{
			if (opensavedformation > index)
			{
				ifopen = 1;
			}
			else
			{
				ifopen = 0;
			}
			//ifopen = (opensavedformation & (1 << index)) > 0;
			if(ifopen > 0 && index < savedformationvec->size())
			{
				name = (*savedformationvec)[index].first;
				if (name.empty())
				{
					name = "编队" + crArgumentParser::appItoa(index + 1);
				}
				if (nameinput[index].valid())
				{
					nameinput[index]->setString(name);
					nameinput[index]->setVisiable(true);
				}
				if (nameedit[index].valid())
				{
					nameedit[index]->setString(name);
					nameedit[index]->setVisiable(true);
				}
				if(editbtn[index].valid())editbtn[index]->setVisiable(true);
				if(backboard[index].valid())backboard[index]->setVisiable(true);
				if (nameradio[index].valid())nameradio[index]->setVisiable(true);
			}
			else
			{
				if(addbtn[index].valid())addbtn[index]->setVisiable(true);
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIBuySavedFormationPageMethod
//
/////////////////////////////////////////
crJXJUIBuySavedFormationPageMethod::crJXJUIBuySavedFormationPageMethod():
	m_index(0),
	m_font("simhei.ttf"),
	m_characterSize(16),
	m_goldColor(0.0f,1.0f,1.0f,1.0f),
	m_textColor(1.0f,1.0f,0.0f,1.0f)
{
}
crJXJUIBuySavedFormationPageMethod::crJXJUIBuySavedFormationPageMethod(const crJXJUIBuySavedFormationPageMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_textColor(handle.m_textColor),
	m_goldColor(handle.m_goldColor),
	m_font(handle.m_font),
	m_characterSize(handle.m_characterSize),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJUIBuySavedFormationPageMethod::inputParam(int i, void *param)
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
void crJXJUIBuySavedFormationPageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_promptCanvas = str;
		break;
	case 2:
		m_information = str;
		break;
	case 3:
		m_okbtn = str;
		break;
	case 4:
		m_font = str;
		break;
	case 5:
		m_characterSize = atoi(str.c_str());
		break;
	case 6:
		crArgumentParser::appAtoVec(str,m_textColor);
		m_textColor = m_textColor / 255.0f;
		break;
	case 7:
		crArgumentParser::appAtoVec(str,m_goldColor);
		m_goldColor = m_goldColor / 255.0f;
		break;
	}
}
void crJXJUIBuySavedFormationPageMethod::operator()(crHandle &handle)
{
	void *param;
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerGameData)
	{
		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
		if (canvas.valid())
		{
			//crVector2i price;
			int ifhint = 1;
			std::string buildingname;
			crData *data = playerGameData->getDataClass();
			data -> getParam(WCHDATA_JXJVipLv,param);
			unsigned char viplv = *(unsigned char *)param;
			int price = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuySavedFormationPagePrice,viplv).c_str());
			int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,viplv).c_str());
			data->getParam(WCHDATA_JXJOpenSaveFormationInfo,param);
			unsigned char opensaveformationifo = *(unsigned char *)param;
			if(opensaveformationifo < maxpage)
			{
				int needgold = price;
				playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgold,NULL));
				if (needgold == 0)
				{
					ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_information));
					ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_okbtn));

					//int hinttype = 0;
					//if (m_index == RefineType_Gold)
					//{
					//	hinttype = JXJGoldCostType_GoldRefineHint;
					//	ifhint = goldcosthint & JXJGoldCostType_GoldRefineHint;
					//}
					//else if (m_index == RefineType_SuperGold)
					//{
					//	hinttype = JXJGoldCostType_SuperGoldRefineHint;
					//	ifhint = goldcosthint & JXJGoldCostType_SuperGoldRefineHint;
					//}

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
						cfg_script.Add("Text","购买编队页吗？");
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
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2098,NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuySavedFormationPageMethod
//
/////////////////////////////////////////
crJXJBuySavedFormationPageMethod::crJXJBuySavedFormationPageMethod():
	m_this(NULL)
{
}
crJXJBuySavedFormationPageMethod::crJXJBuySavedFormationPageMethod(const crJXJBuySavedFormationPageMethod& handle):
	crMethod(handle)
{
}
void crJXJBuySavedFormationPageMethod::inputParam(int i, void *param)
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
void crJXJBuySavedFormationPageMethod::addParam(int i, const std::string& str)
{
}
void crJXJBuySavedFormationPageMethod::operator()(crHandle &handle)
{	
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvBuySavedFormationPage,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuySavedFormationPageMethod
//
/////////////////////////////////////////
crJXJRecvBuySavedFormationPageMethod::crJXJRecvBuySavedFormationPageMethod():
	m_netType(GameClient_Game){}
crJXJRecvBuySavedFormationPageMethod::crJXJRecvBuySavedFormationPageMethod(const crJXJRecvBuySavedFormationPageMethod& handle)
{
}
void crJXJRecvBuySavedFormationPageMethod::inputParam(int i, void *param)
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

void crJXJRecvBuySavedFormationPageMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuySavedFormationPageMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				char success = 0;
				void *param;
				bool needdeductgold = false;
				m_this->getDataClass()->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuySavedFormationPagePrice,viplv).c_str());
				int maxpage = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSavedFormationMax,viplv).c_str());
				crData *data = m_this->getDataClass();
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJOpenSaveFormationInfo,param);
				unsigned char *opensaveformationifo = (unsigned char *)param;
				do 
				{
					if (*opensaveformationifo >= maxpage)
					{
						//编队切页已达最大
						success = -1;
						break;
					}
					MoneyChangeData moneydata(needgold,"购买编队切页");
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
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
						(*opensaveformationifo) += 1;
					}
				} while (0);
				data->excHandle(MAKEINT64(WCH_LockData,0));
				if (success == 1)
				{
					data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
					SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
					savedformationvec->resize(*opensaveformationifo);
					if (needdeductgold)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
						callback->savePlayerData(playerData.get());
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				if (success == 1)
				{
					stream->createBuf(2);
					stream->_writeChar(success);
					stream->_writeUChar(*opensaveformationifo);
				}
				else
				{
					stream->createBuf(1);
					stream->_writeChar(success);
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvBuySavedFormationPage,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				///游戏日志
				std::string logdata = crArgumentParser::appItoa(success);
				GameLogData gamelog(Log_BuySavedFormationPage,logdata);//Log_ClearPunishTime
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKEINT64(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			char success = m_stream -> _readChar();
			int tipsindex = 0;
			if (success == 1)
			{
				tipsindex = 2099;
				unsigned char saveformationinfo = m_stream->_readUChar();
				crData *data = m_this->getDataClass();
				data->inputParam(WCHDATA_JXJOpenSaveFormationInfo,&saveformationinfo);
				data->getParam(WCHDATA_JXJSavedFormationInfoVec, param);
				SavedFormationInfoVec *savedformationvec = (SavedFormationInfoVec *)param;
				savedformationvec->resize(saveformationinfo);
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
			m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(tipsindex,NULL));//
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISetTempFormationMethod
//
/////////////////////////////////////////
crJXJUIChangeFormationEditMethod::crJXJUIChangeFormationEditMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJUIChangeFormationEditMethod::crJXJUIChangeFormationEditMethod(const crJXJUIChangeFormationEditMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_editinput(handle.m_editinput),
	m_staticinput(handle.m_staticinput),
	m_index(handle.m_index)
{
	for (int i = 0; i < 4; ++i)
	{
		m_inputsw[i] = handle.m_inputsw[i];
	}
}
void crJXJUIChangeFormationEditMethod::inputParam(int i, void *param)
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
void crJXJUIChangeFormationEditMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_editinput = str;
		break;
	case 1:
		m_staticinput = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		m_inputsw[i - 2] = str;
		break;
	case 6:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIChangeFormationEditMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this/* && m_this->getDataClass()*/)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			ref_ptr<crEditWidgetNode> nameedit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_editinput));
			//ref_ptr<crStaticTextWidgetNode> nameinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_staticinput));
			crMultiSwitch *inputsw[4] = {NULL};
			for (int i = 0; i < 4; ++i)
			{
				inputsw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_inputsw[i]));
				if (i != m_index)
				{
					if (inputsw[m_index])
					{
						inputsw[m_index]->setActiveSwitchSet(1);
					}
				}
			}
			
			if (nameedit.valid())
			{
				nameedit->kernelGetFocus();
				CRCore::crFilterRenderManager::getInstance()->setInputCapturer(nameedit.get());
				CRCore::crFilterRenderManager::getInstance()->setFocusNode(nameedit.get());
				//if (nameinput)
				//{
				//	nameinput->setString(nameedit->getUTF8String());
				//}
			}
			if (inputsw[m_index])
			{
				inputsw[m_index]->setActiveSwitchSet(0);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIFormationEditLostFocusMethod
//
/////////////////////////////////////////
crJXJUIFormationEditLostFocusMethod::crJXJUIFormationEditLostFocusMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJUIFormationEditLostFocusMethod::crJXJUIFormationEditLostFocusMethod(const crJXJUIFormationEditLostFocusMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_editinput(handle.m_editinput),
	m_staticinput(handle.m_staticinput),
	m_index(handle.m_index)
{
}
void crJXJUIFormationEditLostFocusMethod::inputParam(int i, void *param)
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
void crJXJUIFormationEditLostFocusMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_editinput = str;
		break;
	case 1:
		m_staticinput = str;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		m_inputsw[i - 2] = str;
		break;
	case 6:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUIFormationEditLostFocusMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *myplayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (m_this/* && m_this->getDataClass()*/)
	{
		ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
		if (canvas.valid())
		{
			void *param;
			std::string pagename;
			ref_ptr<crEditWidgetNode> nameedit = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_editinput));
			ref_ptr<crStaticTextWidgetNode> nameinput = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_staticinput));
			crMultiSwitch *inputsw[4] = {NULL};
			for (int i = 0; i < 4; ++i)
			{
				inputsw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_inputsw[i]));
				if (inputsw[m_index])
				{
					inputsw[m_index]->setActiveSwitchSet(1);
				}
			}

			if (nameedit.valid())
			{
				pagename = nameedit->getUTF8String();
				if (nameinput.valid())
				{
					nameinput -> setString(pagename);
				}
			}
			crData *data = myplayer->getDataClass();
			data->getParam(WCHDATA_JXJSavedFormationInfoVec,param);
			SavedFormationInfoVec *savedtempformationvec = (SavedFormationInfoVec *)param;
			if (m_index < savedtempformationvec->size())
			{
				(*savedtempformationvec)[m_index].first = pagename;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUISetAddFriendTypeMethod
//
/////////////////////////////////////////
crJXJUISetAddFriendTypeMethod::crJXJUISetAddFriendTypeMethod():
	m_this(NULL)
{
}
crJXJUISetAddFriendTypeMethod::crJXJUISetAddFriendTypeMethod(const crJXJUISetAddFriendTypeMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJUISetAddFriendTypeMethod::inputParam(int i, void *param)
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
void crJXJUISetAddFriendTypeMethod::addParam(int i, const std::string& str)
{
}
void crJXJUISetAddFriendTypeMethod::operator()(crHandle &handle)
{	
	unsigned char addfriend = JXJAddFriendType_Friend;
	crData *braindata = crBrain::getInstance()->getDataClass();
	braindata->inputParam(WCHDATA_JXJAddFriendOrBlack,&addfriend);
}
/////////////////////////////////////////
//
//crJXJCalculatePowerMethod
//
/////////////////////////////////////////
crJXJCalculatePowerMethod::crJXJCalculatePowerMethod(){}
crJXJCalculatePowerMethod::crJXJCalculatePowerMethod(const crJXJCalculatePowerMethod& handle):
	crMethod(handle)
{
}
void crJXJCalculatePowerMethod::inputParam(int i, void *param)
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
			m_troopinfo = *(crVector4i*)(LOINT64(param64));//id,count,jiangka1,jiangka2
			m_output = (crVector2i*)(HIINT64(param64));
		}
		break;
	}
}

void crJXJCalculatePowerMethod::addParam(int i, const std::string& str)
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

void crJXJCalculatePowerMethod::operator()(crHandle &handle)
{
	if(m_troopinfo[0]>0)
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO>troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		crTableIO::StrVec itemrecord,record;
		int abid = itemtab->getTitleIndex("abstractid");
		int scriptindex = troopsTab->getTitleIndex("脚本");
		if (troopsTab->queryOneRecord(0,crArgumentParser::appItoa(m_troopinfo[0]),record) >= 0 && scriptindex > 0)
		{
			ref_ptr<crRole> role = new crRole;
			role->setAbstractItemID(m_troopinfo[0]);
			role->getAbstractItem()->setScript(record[scriptindex]);
			role->setItemtype(crInstanceItem::Npc);
			role->loadItemData(PlayerStream);
			/*unsigned short*/int armyCount = m_troopinfo[1];
			EquipOnMap *equipOnMap;
			crData *roleData;
			bool found = false;
			float output = 0.0f;
			roleData = role->getDataClass();
			if(roleData)
			{
				roleData->inputParam(WCHDATA_JXJArmyCount,&armyCount);
				roleData->inputParam(WCHDATA_JXJRTArmyCount,&armyCount);
				crVector2i equips(m_troopinfo[2],m_troopinfo[3]);
				roleData->excHandle(MAKEINT64(WCH_LockData,1));
				void *param;
				roleData->getParam(WCHDATA_EquipOnMap,param);
				equipOnMap = (EquipOnMap *)param;
				if(equips[0]>0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord) >= 0)
						equipOnMap->insert(std::make_pair(atoi(itemrecord[abid].c_str()),0));
				}
				if(equips[1]>0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[1]),itemrecord) >= 0)
						equipOnMap->insert(std::make_pair(atoi(itemrecord[abid].c_str()),0));
				}
				roleData->excHandle(MAKEINT64(WCH_LockData,0));

				//设置天赋
				m_this->doEvent(WCH_JXJSetInborn,MAKEINT64(role.get(),&equips));
				//计算战斗力
				ref_ptr<crFormationInfo> formationinfo = new crFormationInfo;
				formationinfo->setAbstractID(m_troopinfo[0]);
				formationinfo->setCount(m_troopinfo[1]);
				formationinfo->setEquips(equips);
				role->doEvent(WCH_JXJGetRoleFightingPower,MAKEINT64(&output,formationinfo.get()));
				(*m_output)[0] = output;

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
				if(canvas.valid())
				{
					crData *canvasData = canvas->getDataClass();
					if(canvasData)
					{
						canvasData->excHandle(MAKEINT64(WCH_LockData,1));
						canvasData->getParam(WCHDATA_JXJFormationTempRoleMap,param);
						TempRoleMap *roleMap = (TempRoleMap *)param;

						roleMap->insert(std::make_pair((*m_output)[1],role));
						canvasData->excHandle(MAKEINT64(WCH_LockData,0));
					}
				}
			}
			else
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJRecvRoleInfo2Method Error aitemid= "<<role->getAbstractItemID()<<std::endl;
				//sprintf(gDebugInfo->buf(),"crJXJCalculatePowerMethod Error aitemid=%d\n\0",role->getAbstractItemID());
				//gDebugInfo->debugInfo(CRCore::ALWAYS);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckEditMethod
//
/////////////////////////////////////////
crJXJCheckEditMethod::crJXJCheckEditMethod():
	m_this(NULL),
	m_index(0)
{
}
crJXJCheckEditMethod::crJXJCheckEditMethod(const crJXJCheckEditMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJCheckEditMethod::inputParam(int i, void *param)
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
		m_this = (crEditWidgetNode*)param;
		break;
	}
}
void crJXJCheckEditMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJCheckEditMethod::operator()(crHandle &handle)
{	
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this)
	{
		void *param;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		std::string thisstring = m_this->getUTF8String();
		int canedit = -1;
		switch (m_index)
		{
		case 1:
			canedit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFormationnameMaxSize,viplv).c_str());
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
		if (canedit >= 0)
		{
			if (thisstring.size() > canedit)
			{
				thisstring = m_laststring;
			}
			else
			{
				m_laststring = thisstring;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckRemainPlayerCountMethod
//获取玩家留存率
/////////////////////////////////////////
crJXJCheckRemainPlayerCountMethod::crJXJCheckRemainPlayerCountMethod():
	m_this(NULL)
{
}
crJXJCheckRemainPlayerCountMethod::crJXJCheckRemainPlayerCountMethod(const crJXJCheckRemainPlayerCountMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckRemainPlayerCountMethod::inputParam(int i, void *param)
{
}
void crJXJCheckRemainPlayerCountMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckRemainPlayerCountMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckRemainPlayerCount,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvCheckRemainPlayerCountMethod
//获取玩家留存率
/////////////////////////////////////////
crJXJRecvCheckRemainPlayerCountMethod::crJXJRecvCheckRemainPlayerCountMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckRemainPlayerCountMethod::crJXJRecvCheckRemainPlayerCountMethod(const crJXJRecvCheckRemainPlayerCountMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckRemainPlayerCountMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckRemainPlayerCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckRemainPlayerCountMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)

		{
			//void *param;
			int playerid = m_this->getPlayerID();

			int id = 0;
			RemainPlayerVec remainplayervec;
			RemainPlayerIDVec remainplayerIDvec[4];
			std::string selcolname = "id";
			std::string tabname = "account";
			std::string colname = "logindate";
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crDataBase *global_db = crDataBaseManager::getInstance()->getAccountDB();//getGlobalDB();
			//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
			ref_ptr<crDataBase> db_session = global_db->beginSession();
			ref_ptr<crCheckRemainPlayerData> playerdataquery = NULL;
			int overdue = 0;
			for (int i = 0; i < 4; ++i)
			{
				switch (i)
				{//n天留存率
				case 0:
					overdue = 1;
					break;
				case 1:
					overdue = 3;
					break;
				case 2:
					overdue = 7;
					break;
				case 3:
					overdue = 0;
					break;
				default:
					break;
				}
				playerdataquery = new crCheckRemainPlayerData;
				playerdataquery->buildQuerySql(selcolname,tabname,colname,overdue);
				db_session->executeQuery(playerdataquery.get());
				crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
				for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
					itr != playerdataQueryResultVec.end();
					++itr )
				{
					playerdataquery = dynamic_cast<crCheckRemainPlayerData *>(itr->get());
					id = playerdataquery->getID();
					remainplayerIDvec[i].push_back(id);
				}
				remainplayervec.push_back(playerdataQueryResultVec.size());
				db_session->releaseQuery();
			}
			global_db->endSession(db_session.get());
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				int idvecsize = 0;
				for (int i = 0; i < 4; i++)
				{
					idvecsize += remainplayerIDvec[i].size();
				}
				unsigned char remainsize = remainplayervec.size();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(8 + 4 * remainsize + 4 * idvecsize);
				stream->_writeUChar(remainsize);
				for (int i = 0; i < remainsize; ++i)
				{
					stream->_writeInt(remainplayervec[i]);
				}
				for (int ii = 0; ii < 4; ++ii)
				{
					idvecsize = remainplayerIDvec[ii].size();
					stream->_writeInt(idvecsize);
					for (int i = 0; i < idvecsize; ++i)
					{
						stream->_writeInt(remainplayerIDvec[ii][i]);
					}
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCheckRemainPlayerCount,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}

		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			int playercount = 0;
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_Operation");
			if (canvasNode.valid())
			{
				std::string servername;
				int playercount = 0;
				int playerid = 0;
				crData *canvasdata = canvasNode->getDataClass();
				RemainPlayerIDVec playeridvec[4];
				if (canvasdata)
				{
					//留存人数
					canvasdata->getParam(WCHDATA_JXJRemainPlayerVec,param);
					RemainPlayerVec *remainvec = (RemainPlayerVec *)param;
					//留存玩家playerid
					canvasdata->getParam(WCHDATA_JXJRemainPlayerMap,param);
					RemainPlayerMap *remainmap = (RemainPlayerMap *)param;
					remainvec->clear();
					unsigned char vecsize = m_stream->_readUChar(); 
					for (int i = 0; i < vecsize; ++i)
					{
						playercount = m_stream->_readInt();
						remainvec->push_back(playercount);
					}
					for (int ii = 0; ii < 4; ++ii)
					{
						vecsize = m_stream->_readInt(); 
						for (int i = 0; i < vecsize; ++i)
						{
							playerid = m_stream->_readInt();
							playeridvec[ii].push_back(playerid);
						}
						remainmap->insert(std::make_pair(ii,playeridvec[ii]));
					}
					ref_ptr<crTableIO> table = new crTableIO;
					crTableIO::StrVec strVec;
					crTableIO::StrVec strVec2;
					strVec.resize(5);
					strVec2.resize(5);
					int index = 0;
					strVec.resize(5);
					strVec[index++] = "title";
					strVec[index++] = "1天内";
					strVec[index++] = "3天内";
					strVec[index++] = "7天内";
					strVec[index++] = "0天内";
					table->setTitleVec(strVec);
					index = 0;
					strVec2[index++] = "留存玩家";
					strVec2[index++] = crArgumentParser::appItoa((*remainvec)[0]);
					strVec2[index++] = crArgumentParser::appItoa((*remainvec)[1]);
					strVec2[index++] = crArgumentParser::appItoa((*remainvec)[2]);
					strVec2[index++] = crArgumentParser::appItoa((*remainvec)[3]);
					table->addData(strVec2);
					table->saveToFileNoCook("玩家留存统计.tab");
					//unsigned char shiliid = 0;
					//int shiliplayercount[3] = {0};
					//void *param;
					//crData *braindata = crBrain::getInstance()->getDataClass();
					//braindata->getParam(WCHDATA_JXJCheckPlayDataTable,param);
					//ref_ptr<crTableIO>table = (ref_ptr<crTableIO>)param;
					//int shiliindex = table->getTitleIndex("势力ID");
					//crTableIO::StrVec record;
					//for (int i = 0; i < 4; ++i)
					//{
					//	for (RemainPlayerIDVec::iterator itr = playeridvec[i].begin();
					//		itr != playeridvec[i].end();++itr)
					//	{
					//		if (table->queryOneRecord(0,crArgumentParser::appItoa(*itr),record) >= 0)
					//		{
					//			shiliid = atoi(record[shiliindex].c_str()) - c_startShiliID;
					//			switch(shiliid)
					//			{
					//			case 0:
					//			case 1:
					//			case 2:
					//				shiliplayercount[shiliid] += 1;
					//				break;
					//			}
					//		}
					//	}
					//}
				}
				canvasNode->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckLostPlayerCountMethod
//获取玩家流失率
/////////////////////////////////////////
crJXJCheckLostPlayerCountMethod::crJXJCheckLostPlayerCountMethod():
	m_this(NULL)
{
}
crJXJCheckLostPlayerCountMethod::crJXJCheckLostPlayerCountMethod(const crJXJCheckLostPlayerCountMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckLostPlayerCountMethod::inputParam(int i, void *param)
{
}
void crJXJCheckLostPlayerCountMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckLostPlayerCountMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckLostPlayerCount,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvCheckLostPlayerCountMethod
//获取玩家流失率
/////////////////////////////////////////
crJXJRecvCheckLostPlayerCountMethod::crJXJRecvCheckLostPlayerCountMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckLostPlayerCountMethod::crJXJRecvCheckLostPlayerCountMethod(const crJXJRecvCheckLostPlayerCountMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckLostPlayerCountMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckLostPlayerCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckLostPlayerCountMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)

		{
			//void *param;
			int playerid = m_this->getPlayerID();

			int id = 0;
			RemainPlayerVec remainplayervec;
			RemainPlayerIDVec remainplayerIDvec[4];
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crDataBase *global_db = crDataBaseManager::getInstance()->getAccountDB();//getGlobalDB();
			//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
			ref_ptr<crDataBase> db_session = global_db->beginSession();
			ref_ptr<crCheckTableCountData> tablecountdata = NULL;
			std::string tabname = "account";
			int overdue = 0;
			std::string colname = "playerid";
			for (int i = 0; i < 4; ++i)
			{
				switch (i)
				{//n天
				case 0:
					overdue = 0;
					break;
				case 1:
					overdue = 1;
					break;
				case 2:
					overdue = 3;
					break;
				case 3:
					overdue = 7;
					break;
				default:
					break;
				}
				tablecountdata = new crCheckTableCountData;
				tablecountdata->buildQuerySql(tabname,colname,overdue);
				db_session->executeQuery(tablecountdata.get());
				crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
				for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
					itr != playerdataQueryResultVec.end();
					++itr )
				{
					tablecountdata = dynamic_cast<crCheckTableCountData *>(itr->get());
					id = tablecountdata->getTabCount();
					remainplayerIDvec[i].push_back(id);
				}
				remainplayervec.push_back(playerdataQueryResultVec.size());
				db_session->releaseQuery();
			}
			global_db->endSession(db_session.get());
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				int idvecsize = 0;
				for (int i = 0; i < 4; i++)
				{
					idvecsize += remainplayerIDvec[i].size();
				}
				unsigned char remainsize = remainplayervec.size();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5 + 4 * remainsize + 4 * idvecsize);
				stream->_writeUChar(remainsize);
				for (int i = 0; i < remainsize; ++i)
				{
					stream->_writeInt(remainplayervec[i]);
				}
				for (int ii = 0; ii < 4; ++ii)
				{
					idvecsize = remainplayerIDvec[ii].size();
					stream->_writeUChar(remainsize);
					for (int i = 0; i < remainsize; ++i)
					{
						stream->_writeInt(remainplayerIDvec[ii][i]);
					}
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCheckLostPlayerCount,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}

		}
		else if(m_netType == GameClient_Game)
		{
			//void *param;
			int playercount = 0;
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_Operation");
			if (canvasNode.valid())
			{
				canvasNode->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckTatolPlayerCountMethod
//获取玩家总注册 总创建
/////////////////////////////////////////
crJXJCheckTatolPlayerCountMethod::crJXJCheckTatolPlayerCountMethod():
	m_this(NULL)
{
}
crJXJCheckTatolPlayerCountMethod::crJXJCheckTatolPlayerCountMethod(const crJXJCheckTatolPlayerCountMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckTatolPlayerCountMethod::inputParam(int i, void *param)
{
}
void crJXJCheckTatolPlayerCountMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckTatolPlayerCountMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckTotalPlayerCount,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvCheckTatolPlayerCountMethod
//获取玩家注册创建数
/////////////////////////////////////////
crJXJRecvCheckTatolPlayerCountMethod::crJXJRecvCheckTatolPlayerCountMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckTatolPlayerCountMethod::crJXJRecvCheckTatolPlayerCountMethod(const crJXJRecvCheckTatolPlayerCountMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckTatolPlayerCountMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckTatolPlayerCountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckTatolPlayerCountMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)

		{
			//void *param;
			int playerid = m_this->getPlayerID();

			int id = 0;
			RemainPlayerVec totalaccountvec;
			RemainPlayerIDVec totalaccountIDvec[4];
			RemainPlayerVec createplayervec;
			RemainPlayerIDVec createplayerIDvec[4];
			std::string selcolname = "id";
			std::string tabname = "account";
			std::string colname = "createdate";
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			//查询account表（0，1，3，7）
			crDataBase *account_db = crDataBaseManager::getInstance()->getAccountDB();//getGlobalDB();
			//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
			ref_ptr<crDataBase> db_session = account_db->beginSession();
			ref_ptr<crCheckRemainPlayerData> playerdataquery = NULL;
			crVector4i overdue = crVector4i(0,1,3,7);
			for (int i = 0; i < 4; ++i)
			{
				playerdataquery = new crCheckRemainPlayerData;
				playerdataquery->buildQuerySql(selcolname,tabname,colname,overdue[i]);
				db_session->executeQuery(playerdataquery.get());
				crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
				for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
					itr != playerdataQueryResultVec.end();
					++itr )
				{
					playerdataquery = dynamic_cast<crCheckRemainPlayerData *>(itr->get());
					id = playerdataquery->getID();
					totalaccountIDvec[i].push_back(id);
				}
				totalaccountvec.push_back(playerdataQueryResultVec.size());
				db_session->releaseQuery();
			}
			//查询总注册数
			int totalcount = 0; 
			ref_ptr<crCheckTableCountData> tablecountquery = new crCheckTableCountData;
			tablecountquery->buildQuerySql(tabname);
			db_session->executeQuery(tablecountquery.get());
			crDataBase::QueryResultVec &accountQueryResultVec = db_session->getQueryResult();
			if (!accountQueryResultVec.empty())
			{
				tablecountquery = dynamic_cast<crCheckTableCountData *>(accountQueryResultVec[0].get());
				totalcount = tablecountquery->getTabCount();
			}
			db_session->releaseQuery();
			
			account_db->endSession(db_session.get());

			selcolname = "playerid";
			tabname = "charactername";
			colname = "createdate";
			//查询gameglobal表（总创建）
			//crDataBase *gameglobaldb = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
			crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();
			ref_ptr<crDataBase> dbSession = global_db->beginSession();
			for (int i = 0; i < 4; ++i)
			{
				playerdataquery = new crCheckRemainPlayerData;
				playerdataquery->buildQuerySql(selcolname,tabname,colname,overdue[i]);
				dbSession->executeQuery(playerdataquery.get());
				crDataBase::QueryResultVec &playerdataQueryResultVec = dbSession->getQueryResult();
				for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
					itr != playerdataQueryResultVec.end();
					++itr )
				{
					playerdataquery = dynamic_cast<crCheckRemainPlayerData *>(itr->get());
					id = playerdataquery->getID();
					createplayerIDvec[i].push_back(id);
				}
				createplayervec.push_back(playerdataQueryResultVec.size());
				dbSession->releaseQuery();
			}
			global_db->endSession(dbSession.get());
			//向客户端发包
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				int idvecsize = 0;
				for (int i = 0; i < 4; i++)
				{
					idvecsize += totalaccountIDvec[i].size();
					idvecsize += createplayerIDvec[i].size();
				}
				unsigned char remainsize = totalaccountvec.size();
				unsigned char remainsize2 = createplayervec.size();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(20 + 4 * (remainsize + remainsize2 + idvecsize));
				//总注册
				stream->_writeInt(totalcount);
				//注册0，1，3，7
				stream->_writeUChar(remainsize);
				for (int i = 0; i < remainsize; ++i)
				{
					stream->_writeInt(totalaccountvec[i]);
				}
				for (int ii = 0; ii < 4; ++ii)
				{
					idvecsize = totalaccountIDvec[ii].size();
					stream->_writeInt(idvecsize);
					for (int i = 0; i < idvecsize; ++i)
					{
						stream->_writeInt(totalaccountIDvec[ii][i]);
					}
				}
				//总创建
				stream->_writeUChar(remainsize2);
				for (int i = 0; i < remainsize2; ++i)
				{
					stream->_writeInt(createplayervec[i]);
				}
				for (int ii = 0; ii < 4; ++ii)
				{
					idvecsize = createplayerIDvec[ii].size();
					stream->_writeInt(idvecsize);
					for (int i = 0; i < idvecsize; ++i)
					{
						stream->_writeInt(createplayerIDvec[ii][i]);
					}
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCheckTotalPlayerCount,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}

		}
		else if(m_netType == GameClient_Game)
		{
			void *param;
			int playercount = 0;
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_Operation");
			if (canvasNode.valid())
			{
				std::string servername;
				int playercount = 0;
				int playerid = 0;
				crData *canvasdata = canvasNode->getDataClass();
				RemainPlayerIDVec playeridvec[4];
				RemainPlayerIDVec createplayeridvec[4];
				if (canvasdata)
				{
					CRCore::ScopedLock<crData> lock(*canvasdata);
					//总注册
					canvasdata->getParam(WCHDATA_JXJTotalPlayerVec,param);
					RemainPlayerVec *totalvec = (RemainPlayerVec *)param;
					canvasdata->getParam(WCHDATA_JXJTotalPlayerMap,param);
					RemainPlayerMap *totalmap = (RemainPlayerMap *)param;
					totalvec->clear();
					totalmap->clear();
					int totalcount = m_stream->_readInt();
					unsigned char vecsize = m_stream->_readUChar(); 
					for (int i = 0; i < vecsize; ++i)
					{
						playercount = m_stream->_readInt();
						totalvec->push_back(playercount);
					}
					for (int ii = 0; ii < 4; ++ii)
					{
						vecsize = m_stream->_readInt(); 
						for (int i = 0; i < vecsize; ++i)
						{
							playerid = m_stream->_readInt();
							playeridvec[ii].push_back(playerid);
						}
						totalmap->insert(std::make_pair(ii,playeridvec[ii]));
					}
					//总创建
					canvasdata->getParam(WCHDATA_JXJCreatePlayerVec,param);
					RemainPlayerVec *createvec = (RemainPlayerVec *)param;
					canvasdata->getParam(WCHDATA_JXJCreatePlayerMap,param);
					RemainPlayerMap *createmap = (RemainPlayerMap *)param;
					createvec->clear();
					createmap->clear();
					vecsize = m_stream->_readUChar(); 
					for (int i = 0; i < vecsize; ++i)
					{
						playercount = m_stream->_readInt();
						createvec->push_back(playercount);
					}
					for (int ii = 0; ii < 4; ++ii)
					{
						vecsize = m_stream->_readInt(); 
						for (int i = 0; i < vecsize; ++i)
						{
							playerid = m_stream->_readInt();
							createplayeridvec[ii].push_back(playerid);
						}
						createmap->insert(std::make_pair(ii,createplayeridvec[ii]));
					}
					//////////////////////////////////////
					PlayerLvMap *playerlvmap[3] = {NULL};
					for (int i = 0; i < 3; ++i)
					{
						canvasdata->getParam(WCHDATA_JXJPlayerLvMapWei + i,param);
						playerlvmap[i] = (PlayerLvMap *)param;
					}
					RemainPlayerVec createveclv[5];
					for (int i = 0; i < 5; ++i)
					{
						createveclv[i].resize(81);
					}
					crVector4i cshilipeople[3];
					int mindex = 0;
					for (int i = 0; i < 3; ++i)
					{
						for (PlayerLvMap::iterator litr = playerlvmap[i]->begin();
							litr != playerlvmap[i]->end();++litr)
						{
							createveclv[4][litr->second] += 1;
							mindex = 0;
							for (RemainPlayerMap::iterator itr = createmap->begin(); itr != createmap->end(); ++itr,++mindex)
							{
								for (RemainPlayerIDVec::iterator pitr = itr->second.begin();
									pitr != itr->second.end();++pitr)
								{
									if ((*pitr) == litr->first)
									{
										(cshilipeople[i][mindex]) += 1;
										createveclv[mindex][litr->second] += 1;
									}
								}
							}
						}
					}
					int totalplayerlvmapsize = 0;
					int playerlvmapsize[3] ={0};
					for (int i = 0; i < 3; ++i)
					{
						playerlvmapsize[i] = playerlvmap[i]->size();
						totalplayerlvmapsize+=playerlvmapsize[i];
					}
					totalvec->push_back(totalcount);
					createvec->push_back(totalplayerlvmapsize);
					ref_ptr<crTableIO> table = new crTableIO;
					crTableIO::StrVec strVec;
					crTableIO::StrVec strVec2;
					crTableIO::StrVec strVec3;
					strVec.resize(6);
					strVec2.resize(6);
					strVec3.resize(6);
					int index = 0;
					int percent = 0;
					strVec.resize(6);
					strVec[index++] = "title";
					strVec[index++] = "0天内";
					strVec[index++] = "1天内";
					strVec[index++] = "3天内";
					strVec[index++] = "7天内";
					strVec[index++] = "全部";
					table->setTitleVec(strVec);
					index = 0;
					strVec2[index++] = "总注册";
					strVec2[index++] = crArgumentParser::appItoa((*totalvec)[0]);
					strVec2[index++] = crArgumentParser::appItoa((*totalvec)[1]);
					strVec2[index++] = crArgumentParser::appItoa((*totalvec)[2]);
					strVec2[index++] = crArgumentParser::appItoa((*totalvec)[3]);
					strVec2[index++] = crArgumentParser::appItoa((*totalvec)[4]);
					table->addData(strVec2);
					index = 0;
					strVec3[index++] = "总创建";
					strVec3[index++] = crArgumentParser::appItoa((*createvec)[0]);
					strVec3[index++] = crArgumentParser::appItoa((*createvec)[1]);
					strVec3[index++] = crArgumentParser::appItoa((*createvec)[2]);
					strVec3[index++] = crArgumentParser::appItoa((*createvec)[3]);
					strVec3[index++] = crArgumentParser::appItoa((*createvec)[4]);
					table->addData(strVec3);
					for (int i = 0; i < 3; ++i)
					{
						index = 0;
						strVec.clear();
						strVec.resize(6);
						switch(i)
						{
						case 0:
							strVec[index++] = "魏创建";
							break;
						case 1:
							strVec[index++] = "蜀创建";
							break;
						case 2:
							strVec[index++] = "吴创建";
							break;
						}
						for(int ii = 0; ii < 4; ++ii)
						{
							percent = 0;
							if((*createvec)[ii]!=0)percent = cshilipeople[i][ii]* 100/(*createvec)[ii] ;
							strVec[index++] = crArgumentParser::appItoa(cshilipeople[i][ii]) + "("+ crArgumentParser::appItoa(percent) +"%)";
						}
						if(totalplayerlvmapsize!=0)percent = playerlvmapsize[i]* 100/totalplayerlvmapsize ;
						strVec[index++] = crArgumentParser::appItoa(playerlvmapsize[i]) + "("+ crArgumentParser::appItoa(percent) +"%)";
						table->addData(strVec);
					}
					for (int i = 0;i < 80;++i)
					{
						strVec.clear();
						strVec.resize(6);
						strVec[0] = crArgumentParser::appItoa(i + 1) + "级";
						for (index = 0; index < 5; index++)
						{
							percent = 0;
							if((*createvec)[index]!=0)percent = createveclv[index][i]* 100/(*createvec)[index] ;
							strVec[index + 1] = crArgumentParser::appItoa(createveclv[index][i]) + "("+ crArgumentParser::appItoa(percent) +"%)";
						}
						table->addData(strVec);
					}
					table->saveToFileNoCook("玩家注册创建统计.tab");
				}
				canvasNode->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSkipNewHandIntroMethod
//跳过新手引导
/////////////////////////////////////////
crJXJSkipNewHandIntroMethod::crJXJSkipNewHandIntroMethod():
	m_this(NULL)
{
}
crJXJSkipNewHandIntroMethod::crJXJSkipNewHandIntroMethod(const crJXJSkipNewHandIntroMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSkipNewHandIntroMethod::inputParam(int i, void *param)
{
}
void crJXJSkipNewHandIntroMethod::addParam(int i, const std::string& str)
{
}
void crJXJSkipNewHandIntroMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);
		data -> getParam(WCHDATA_JXJSkipNewHand,param);
		bool *skipnewhand = (bool *)param;
		if (!(*skipnewhand))
		{
			*skipnewhand = true;
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSkipNewHandIntro,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvSkipNewHandIntroMethod
//跳过新手引导
/////////////////////////////////////////
crJXJRecvSkipNewHandIntroMethod::crJXJRecvSkipNewHandIntroMethod():
	m_netType(GameClient_Game){}
crJXJRecvSkipNewHandIntroMethod::crJXJRecvSkipNewHandIntroMethod(const crJXJRecvSkipNewHandIntroMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvSkipNewHandIntroMethod::inputParam(int i, void *param)
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

void crJXJRecvSkipNewHandIntroMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvSkipNewHandIntroMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			crData *data = m_this->getDataClass();
			CRCore::ScopedLock<crData> lock(*data);
			bool skipnewhand = true;
			data -> inputParam(WCHDATA_JXJSkipNewHand,&skipnewhand);
		}
	}
}
///////////////////////////////////////////
////
////crJXJGetShiliPeoPleMapMethod
////
///////////////////////////////////////////
//crJXJGetShiliPeoPleMapMethod::crJXJGetShiliPeoPleMapMethod():
//	m_this(NULL)
//{
//}
//crJXJGetShiliPeoPleMapMethod::crJXJGetShiliPeoPleMapMethod(const crJXJGetShiliPeoPleMapMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJGetShiliPeoPleMapMethod::inputParam(int i, void *param)
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
//void crJXJGetShiliPeoPleMapMethod::addParam(int i, const std::string& str)
//{
//}
//void crJXJGetShiliPeoPleMapMethod::operator()(crHandle &handle)
//{	
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
//	if(netConductor)
//	{
//		crPlayerDataEventPacket packet;
//		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGetShiliPeoPleMap,NULL);
//		netConductor->getNetManager()->sendPacket("all",packet);
//	}
//}
///////////////////////////////////////////
////
////crJXJRecvGetShiliPeoPleMapMethod
////
///////////////////////////////////////////
//crJXJRecvGetShiliPeoPleMapMethod::crJXJRecvGetShiliPeoPleMapMethod():
//	m_netType(GameClient_Game){}
//crJXJRecvGetShiliPeoPleMapMethod::crJXJRecvGetShiliPeoPleMapMethod(const crJXJRecvGetShiliPeoPleMapMethod& handle):
//	m_gmcanvas(handle.m_gmcanvas)
//{
//}
//void crJXJRecvGetShiliPeoPleMapMethod::inputParam(int i, void *param)
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
//void crJXJRecvGetShiliPeoPleMapMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_gmcanvas = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJRecvGetShiliPeoPleMapMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			void *param;
//			int playerid = m_this->getPlayerID();
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			if(playerData.valid())
//			{
//				crData * shiliData = NULL;
//				crJXJShili * shili = NULL;
//				PeopleMap * peopleMap = {NULL};
//				PlayerLvMap playerlvmap[3];//playerid,lv
//				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
//				serverData->excHandle(MAKEINT64(WCH_LockData,1));
//				for (int i = 0; i < 3; ++i)
//				{
//					serverData->getParam(WCHDATA_JXJShiliWei+i,param);
//					shili = (crJXJShili *)param;
//
//					shiliData = shili->getDataClass();
//					if (!shiliData) break;
//					shiliData->excHandle(MAKEINT64(WCH_LockData,1));
//
//					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
//					peopleMap = (PeopleMap *)param;
//					if (peopleMap -> empty()) 
//					{
//						shiliData->excHandle(MAKEINT64(WCH_LockData,0));
//						continue;
//					}
//					for (PeopleMap::iterator itr = peopleMap->begin();
//						itr != peopleMap->end();
//						++itr)
//					{
//						playerlvmap[i].insert(std::make_pair(itr->first,itr->second->getLv()));
//					}
//					shiliData->excHandle(MAKEINT64(WCH_LockData,0));
//				}
//				serverData->excHandle(MAKEINT64(WCH_LockData,0));
//				int bufsize = 0;
//				int lvmapsize[3] = {0};
//				for (int i = 0;i < 3; ++i)
//				{
//					lvmapsize[i] = playerlvmap[i].size();
//					bufsize += lvmapsize[i] * 5;
//				}
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				stream->createBuf(12 + bufsize);
//				for (int i = 0; i < 3; ++i)
//				{
//					stream->_writeInt(lvmapsize[i]);
//					for (PlayerLvMap::iterator itr = playerlvmap[i].begin();
//						itr != playerlvmap[i].end();
//						++itr)
//					{
//						stream->_writeInt(itr->first);
//						stream->_writeUChar(itr->second);
//					}
//				}
//				crPlayerDataEventPacket packet;
//				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvGetShiliPeoPleMap,stream.get());
//				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//			}
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			void *param;
//			int playerlvmapsize = 0;
//			int playerid = 0;
//			unsigned char playerlv = 0;
//			crData *data = m_this->getDataClass();
//			ref_ptr<crCanvasNode> gmcanvas = crFilterRenderManager::getInstance()->findCanvas(m_gmcanvas);
//			crData *canvasdata = gmcanvas->getDataClass();
//			PlayerLvMap *playerlvmap = NULL;
//			for (int i = 0; i < 3; ++i)
//			{
//				canvasdata->getParam(WCHDATA_JXJPlayerLvMapWei+i,param);
//				playerlvmap = (PlayerLvMap *)param;
//				playerlvmap->clear();
//				playerlvmapsize = m_stream -> _readInt();
//				for (int i = 0; i < playerlvmapsize; ++i)
//				{
//					playerid = m_stream -> _readInt();
//					playerlv = m_stream -> _readUChar();
//					playerlvmap->insert(std::make_pair(playerid,playerlv));
//				}
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJCheckMoneyRecordMethod
//获取玩家充值记录
/////////////////////////////////////////
crJXJCheckMoneyRecordMethod::crJXJCheckMoneyRecordMethod():
	m_this(NULL)
{
}
crJXJCheckMoneyRecordMethod::crJXJCheckMoneyRecordMethod(const crJXJCheckMoneyRecordMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCheckMoneyRecordMethod::inputParam(int i, void *param)
{
}
void crJXJCheckMoneyRecordMethod::addParam(int i, const std::string& str)
{
}
void crJXJCheckMoneyRecordMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCheckMoneyRecord,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvCheckMoneyRecordMethod
//获取玩家充值记录
/////////////////////////////////////////
crJXJRecvCheckMoneyRecordMethod::crJXJRecvCheckMoneyRecordMethod():
	m_netType(GameClient_Game){}
crJXJRecvCheckMoneyRecordMethod::crJXJRecvCheckMoneyRecordMethod(const crJXJRecvCheckMoneyRecordMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCheckMoneyRecordMethod::inputParam(int i, void *param)
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

void crJXJRecvCheckMoneyRecordMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvCheckMoneyRecordMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)

		{
			//void *param;
			int playerid = m_this->getPlayerID();

			int id = 0;
			int moneycount = 0;
			std::string infostr;
			std::string createdate;
			PlayerPayMap playpaymap;
			int bufsize = 0;
			ref_ptr<crPlayerPayQuery> playerpayquery = NULL;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			crDataBase *global_db = crDataBaseManager::getInstance()->getGlobalDB();
			//crDataBase *global_db = crDataBaseManager::getInstance()->getDataBase(GameGlobalDB);
			ref_ptr<crDataBase> db_session = global_db->beginSession();
			ref_ptr<crCheckMoneyRecordData> moneyrecorddata = new crCheckMoneyRecordData;
			moneyrecorddata->buildQuerySql();
			db_session->executeQuery(moneyrecorddata.get());
			crDataBase::QueryResultVec &playerdataQueryResultVec = db_session->getQueryResult();
			for( crDataBase::QueryResultVec::iterator itr = playerdataQueryResultVec.begin();
				itr != playerdataQueryResultVec.end();
				++itr )
			{
				playerpayquery = new crPlayerPayQuery;
				moneyrecorddata = dynamic_cast<crCheckMoneyRecordData *>(itr->get());
				id = moneyrecorddata->getPlayerID();
				moneycount = moneyrecorddata->getMoneyCount();
				infostr = moneyrecorddata->getStr();
				createdate = moneyrecorddata->getCreatedate();
				playerpayquery->setPlayerID(id);
				playerpayquery->setMoneyCount(moneycount);
				playerpayquery->setStr(infostr);
				playerpayquery->setCreateDate(createdate);
				playpaymap.insert(std::make_pair(id,playerpayquery));
				bufsize += 16 + createdate.size() + infostr.size();
				//remainplayerIDvec[i].push_back(id);
			}
			db_session->releaseQuery();
			global_db->endSession(db_session.get());
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4 + bufsize);
				stream->_writeInt(playpaymap.size());
				for(PlayerPayMap::iterator itr = playpaymap.begin();
					itr != playpaymap.end();
					++itr)
				{
					stream->_writeInt(itr->first);
					stream->_writeInt(itr->second->getMoneyCount());
					stream->_writeString(itr->second->getStr());
					stream->_writeString(itr->second->getCreateDate());
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvCheckMoneyRecord,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}

		}
		else if(m_netType == GameClient_Game)
		{
			//void *param;
			int index = 0;
			crTableIO::StrVec strVec;
			strVec.resize(4);
			ref_ptr<crTableIO>table = new crTableIO;
			strVec[index++] = "playerid";
			strVec[index++] = "充值金额";
			strVec[index++] = "充值描述";
			strVec[index++] = "充值时间";
			table->setTitleVec(strVec);
			int playerid = 0;
			int money = 0;
			std::string str;
			std::string date;
			int paymapsize = m_stream->_readInt();
			for (int i = 0; i < paymapsize; ++i)
			{
				index = 0;
				playerid = m_stream->_readInt();
				money = m_stream->_readInt();
				str = m_stream->_readString();
				date = m_stream->_readString();
				strVec[index++] = crArgumentParser::appItoa(playerid);
				strVec[index++] = crArgumentParser::appItoa(money);
				strVec[index++] = str;
				strVec[index++] = date;
				table->addData(strVec);
			}
			table->saveToFileNoCook("充值统计.tab");
			ref_ptr<crCanvasNode>canvasNode = crFilterRenderManager::getInstance()->findCanvas("UI_Operation");
			if (canvasNode.valid())
			{
				canvasNode->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJTechTipsMethod
//
/////////////////////////////////////////
crJXJTechTipsMethod::crJXJTechTipsMethod():
	m_ea(NULL)
{
}
crJXJTechTipsMethod::crJXJTechTipsMethod(const crJXJTechTipsMethod& handle):
	crMethod(handle),
	m_tipscanvas(handle.m_tipscanvas),
	m_icon(handle.m_icon)
{
	for (int i = 0; i < 2; i++)
	{
		m_input[i] = handle.m_input[i];
	}
}
void crJXJTechTipsMethod::inputParam(int i, void *param)
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
void crJXJTechTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipscanvas = str;
		break;
	case 1:
		m_icon = str;
		break;
	case 2:
		m_input[0] = str;
		break;
	case 3:
		m_input[1] = str;
		break;
	}
}
void crJXJTechTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipscanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	ref_ptr<crTableIO>techlvtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
	if(tipsCanvas.valid() && canvas.valid() && playerData && playerData->getDataClass() && techlvtab.valid())
	{
		void *param;
		crTableIO::StrVec record;
		crData *data = playerData->getDataClass();
		crData *canvasdata = canvas->getDataClass();
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJSelFubenID,param);
			unsigned short fubenid = *(unsigned short *)param;
			int tecnIconIndex = techlvtab->getTitleIndex("icon");
			int nameIndex = techlvtab->getTitleIndex("name");
			int fubenIndex = techlvtab->getTitleIndex("前置副本");
			int infoIndex = techlvtab->getTitleIndex("描述");
			if(fubenIndex >= 0 && tecnIconIndex >= 0 && techlvtab->queryOneRecord(fubenIndex,crArgumentParser::appItoa(fubenid),record)>=0)
			{

				ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_icon));
				ref_ptr<crStaticTextWidgetNode> nameinput = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_input[0]));
				ref_ptr<crStaticTextWidgetNode> infoinput = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_input[1]));
				if (icon.valid())
				{
					icon->setImageName(record[tecnIconIndex]);
				}
				if (nameinput.valid())
				{
					nameinput->setString(record[nameIndex]);
				}
				if (infoinput.valid())
				{
					infoinput->setString(record[infoIndex]);
				}
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				const crBoundingBox &iconbox = m_this->getBoundBox();
				const crBoundingBox &tip = tipsCanvas->getBoundBox();
				float posx = (tip.m_max[0]-tip.m_min[0])*0.5;
				float posy = (tip.m_max[1]-tip.m_min[1])*0.5;
				crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
				pos = pos * mat;
				/////////////////////////////////////////
				crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
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
					posy2 = pos2[1] + posy;
				}
				crVector2 mouse = crVector2(posx2,posy2);
				/////////////////////////////////////////
				tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f)/* * mat*/);
				tipsCanvas->setCanFocus(false);
				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
				tipsCanvas->setCanFocus(false);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckJunTuanBtnMethod
//军团招募提示
//
/////////////////////////////////////////

JXJ::crJXJCheckJunTuanBtnMethod::crJXJCheckJunTuanBtnMethod():
	m_this(NULL)
{
}

JXJ::crJXJCheckJunTuanBtnMethod::crJXJCheckJunTuanBtnMethod( const crJXJCheckJunTuanBtnMethod & handle ):
	crMethod(handle),
	m_sw(handle.m_sw),
	m_dongHua1(handle.m_dongHua1),
	m_juntunfbtip(handle.m_juntunfbtip)
{
}

void JXJ::crJXJCheckJunTuanBtnMethod::inputParam( int i, void *param )
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

void JXJ::crJXJCheckJunTuanBtnMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_sw = str;
		break;
	case 1:
		m_dongHua1 = str;
		break;
	case 2:
		m_juntunfbtip = str;
		break;
	default:
		break;
	}
}

void JXJ::crJXJCheckJunTuanBtnMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas("UI_Crops1");
		canvas->updateData();
		crData * canvasData = canvas->getDataClass();
		crMultiSwitch * sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		if (canvasData)
		{
			void * param;
			canvasData->getParam(WCHDATA_JXJMyGroupInfo,param);
			crJXJGroupInfo * groupInfo = (crJXJGroupInfo *) param;
			if (sw)
			{
				sw->setActiveSwitchSet(0);
			}
			canvasData->getParam(WCHDATA_JXJConsortiaHasNewApplyer,param);
			if(!param) return;
			bool hasNewApplyer = *(bool *)param;
			canvasData->getParam(WCHDATA_JXJConsortiaHadOpenShenHe,param);
			bool hadOpenShenHe = *(bool *)param;
			canvasData->getParam(WCHDATA_JXJMySelfGroupMemberINfo,param);
			crJXJConsortiaMember * mySelfInfo = (crJXJConsortiaMember *) param;
			int myGroupPos = 0;
			if (mySelfInfo)
				myGroupPos = mySelfInfo->getGroupPos();
			PermissionCheck checkPermission;
			if (hasNewApplyer && checkPermission(myGroupPos,JXJGroup_AgreeGroupMember))
			{
				crSequence *dongHua1 = dynamic_cast<crSequence*>(m_this->getChildNode(m_dongHua1));
				if (dongHua1) 
				{
					if (sw)
					{
						sw->setActiveSwitchSet(1);
					}
					dongHua1->setVisiable(true);
					dongHua1->setMode(crSequence::START);
					dongHua1->setDuration(0.1,5);
				}
			}
			bool isopen = false;
			canvasData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
			StartedFubenMap * startedFubenMap = (StartedFubenMap *)param;
			StartedFubenMap::iterator sfmitr = startedFubenMap->begin();
			for (;sfmitr != startedFubenMap->end(); ++sfmitr)
			{
				if(sfmitr->second.first < 0)
				{
					isopen = true;
				}
			}
			crMultiSwitch * tipsw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_juntunfbtip));
			if (tipsw)
			{
				if (isopen)
				{
					tipsw->setActiveSwitchSet(0);
				}
				else
				{
					tipsw->setActiveSwitchSet(1);
				}
			}
			
		}
	}
}

/////////////////////////////////////////
//
//crJXJCheckKejiBtnMethod
//
/////////////////////////////////////////
crJXJCheckKejiBtnMethod::crJXJCheckKejiBtnMethod():
	m_this(NULL)
{
}
crJXJCheckKejiBtnMethod::crJXJCheckKejiBtnMethod(const crJXJCheckKejiBtnMethod& handle):
	crMethod(handle),
	m_kejisw(handle.m_kejisw),
	m_sequence(handle.m_sequence)
{
}
void crJXJCheckKejiBtnMethod::inputParam(int i, void *param)
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
void crJXJCheckKejiBtnMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_kejisw = str;
		break;
	case 1:
		m_sequence = str;
		break;
	default:
		break;
	}
}
void crJXJCheckKejiBtnMethod::operator()(crHandle &handle)
{	
	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getDataClass())
	{
		crData *data = playerData->getDataClass();

		data->getParam(WCHDATA_JXJIfTechBtnNew, param);
		unsigned char iftechbtnnew = *(unsigned char *)param;
	
		ref_ptr<crMultiSwitch> kejisw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_kejisw));
		ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(m_this->getChildNode(m_sequence));

		if (kejisw.valid())
		{
			kejisw->setActiveSwitchSet(0);
		}
		if (iftechbtnnew <= 0)
		{
			return;
		}
		bool success = false;
		crTableIO::StrVec record;
		ref_ptr<crTableIO>techtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
		int copperindex = techtab->getTitleIndex("铜钱");
		int foodindex = techtab->getTitleIndex("粮食");
		int woodindex = techtab->getTitleIndex("木材");
		int ironindex = techtab->getTitleIndex("铁矿");
		int fubenidindex = techtab->getTitleIndex("前置副本");
		data->getParam(WCHDATA_JXJFubenProgressMap, param);
		FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
		data->getParam(WCHDATA_JXJTechDevelopMap, param);
		TechDevelopMap *techdevelopmap = (TechDevelopMap *)param;
		int mycopper = *(int *)param;
		data->getParam(WCHDATA_JXJFood, param);
		int myfood = *(int *)param;
		data->getParam(WCHDATA_JXJWood, param);
		int mywood = *(int *)param;
		data->getParam(WCHDATA_JXJIron, param);
		int myiron = *(int *)param;
		crVector4i myresource = crVector4i(mycopper,myfood,mywood,myiron);
		int techid = 0;
		int fubenid = 0;
		int needcopper = 0;
		int needfood = 0;
		int needwood = 0;
		int neediron = 0;
		for (int i = 0; i < techtab->getRowCount(); ++i)
		{
			techid = atoi( (*techtab)(i,0).c_str());
			fubenid = atoi( (*techtab)(i,fubenidindex).c_str());
			needcopper = atoi( (*techtab)(i,copperindex).c_str());
			needfood = atoi( (*techtab)(i,foodindex).c_str());
			needwood = atoi( (*techtab)(i,woodindex).c_str());
			neediron = atoi( (*techtab)(i,ironindex).c_str());
			crVector4i needresource = (crVector4i(needcopper,needfood,needwood,neediron));
			do 
			{
				if (techid <= 0)
				{//id无效
					break;
				}
				TechDevelopMap::iterator titr = techdevelopmap->find(techid);
				if (titr != techdevelopmap->end())
				{//在研发中
					break;
				}
				if(techid>0)
				{
					data->getParam(techid, param);
					unsigned char techlv = *(unsigned char *)param;
					if (techlv > 0)
					{//已经研发过了
						break;
					}
				}
				FubenProgressMap::iterator fitr = fubenprogressmap->find(fubenid);
				if (fitr == fubenprogressmap->end() || fitr->second->getComplete() <= 0)
				{//前置副本未通关
					break;
				}
				for (int i = 0; i < 4; i++)
				{
					if (myresource[i] < needresource[i])
					{//资源不足
						break;
					}
				}
				success = true;
			}while(0);
			if (success)
			{
				if (kejisw.valid())
				{
					kejisw->setActiveSwitchSet(1);
				}
				if (sequence.valid())
				{
					//sequence->setVanishWhenStop(true);
					//sequence->setDuration(0.1,1);
					sequence->startEffect();
				}
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCheckChengkaDonghuaMethod
//
/////////////////////////////////////////
crJXJCheckChengkaDonghuaMethod::crJXJCheckChengkaDonghuaMethod():
	m_this(NULL)
{
}
crJXJCheckChengkaDonghuaMethod::crJXJCheckChengkaDonghuaMethod(const crJXJCheckChengkaDonghuaMethod& handle):
	crMethod(handle),
	m_successcanvas(handle.m_successcanvas)
{
	for (int i = 0 ; i < 5; ++i)
	{
		m_donghua[i] = handle.m_donghua[i];
	}
}
void crJXJCheckChengkaDonghuaMethod::inputParam(int i, void *param)
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
void crJXJCheckChengkaDonghuaMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		m_donghua[i] = str;
		break;
	case 5:
		m_successcanvas = str;
		break;
	default:
		break;
	}
}
void crJXJCheckChengkaDonghuaMethod::operator()(crHandle &handle)
{	
	void *param;
	bool ifstop = true;
	int *flag = 0;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getDataClass())
	{
		crData *canvasdata = m_this->getDataClass();
		if (canvasdata)
		{
			canvasdata->getParam(WCHDATA_JXJRecvChengKaUpgradeFlag,param);
			flag = (int *)param;
		}
		if (*flag > 0)
		{
			//ref_ptr<crMultiSwitch> kejisw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_kejisw));
			ref_ptr<crCanvasNode>tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_successcanvas);
			ref_ptr<crSequence> sequence[5] = {NULL};
			for (int i = 0 ; i < 5; ++i)
			{
				sequence[i] = dynamic_cast<crSequence *>(m_this->getChildNode(m_donghua[i]));
				if(sequence[i].valid() && sequence[i]->getMode() != crSequence::STOP/*STOP*/)
				{
					ifstop = false;
				}
			}
			if (ifstop)
			{
				*flag = 0;
				if (tipsCanvas.valid())
				{
					//crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
					crFilterRenderManager::getInstance()->doModal(tipsCanvas.get());
				}
				m_this->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJResetMapPositionMethod
//
/////////////////////////////////////////
crJXJResetMapPositionMethod::crJXJResetMapPositionMethod():
	m_this(NULL)
{
}
crJXJResetMapPositionMethod::crJXJResetMapPositionMethod(const crJXJResetMapPositionMethod& handle):
	crMethod(handle)
{
}
void crJXJResetMapPositionMethod::inputParam(int i, void *param)
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
void crJXJResetMapPositionMethod::addParam(int i, const std::string& str)
{
}
void crJXJResetMapPositionMethod::operator()(crHandle &handle)
{	
	void *param;
	bool ifstop = true;
	int *flag = 0;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getDataClass())
	{
		crData *canvasdata = m_this->getDataClass();
		crData *data = playerData->getDataClass();
		crTableIO::StrVec shilirecord,chengchirecord;
		ref_ptr<crTableIO>shilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		int guoduindex = shilitab->getTitleIndex("国都");
		ref_ptr<crTableIO>chengchitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		int nameindex = chengchitab->getTitleIndex("buttonname");
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshili = *(unsigned char *)param;
		if (shilitab->queryOneRecord(0,crArgumentParser::appItoa(myshili),shilirecord) >= 0)
		{
			unsigned short chengchiid = atoi(shilirecord[guoduindex].c_str());
			if (chengchitab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),chengchirecord) >= 0)
			{
				ref_ptr<crRadioWidgetNode> chengchiradio = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(chengchirecord[nameindex]));
				if (chengchiradio.valid())
				{
					//offsetX = m_this->getBoundBox();
					m_this->setMatrix(crMatrix::translate(offsetX,offsetY,0.0f));
					crVector2 curOffset(offsetX,offsetY);
					canvasdata->inputParam(WCHDATA_JXJCityCenterOffset,&curOffset);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJFirstGameVideoSettingMethod
//
/////////////////////////////////////////
crJXJFirstGameVideoSettingMethod::crJXJFirstGameVideoSettingMethod():
	m_this(NULL)
{
}
crJXJFirstGameVideoSettingMethod::crJXJFirstGameVideoSettingMethod(const crJXJFirstGameVideoSettingMethod& handle):
	crMethod(handle)
{
}
void crJXJFirstGameVideoSettingMethod::inputParam(int i, void *param)
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
void crJXJFirstGameVideoSettingMethod::addParam(int i, const std::string& str)
{
}
void crJXJFirstGameVideoSettingMethod::operator()(crHandle &handle)
{	
	if( !CRIOManager::fileExists("GameVideoSetting.ini")) return; 

	LPTSTR lpPath = new char[MAX_PATH];
	strcpy(lpPath, ".\\GameVideoSetting.ini");
	//::GetPrivateProfileString("Test","id","Error",str,20，"d://vc//Ex1//ex1.ini");
	int settingindex = GetPrivateProfileInt("GameVideoSetting", "SettingValue", 0, lpPath);
	bool needUpdate = GetPrivateProfileInt("GameVideoSetting", "SettingUpdate", 0, lpPath);

	if (!needUpdate) return;

	WritePrivateProfileString("GameVideoSetting", "SettingUpdate", "0", lpPath);
	delete [] lpPath;
	////////////////////
	//低配 1 1 0 0 0 0 
	//中配 1 2 2 2 0 1
	//高配 1 3 3 1 1 1
	/////////////////////
	//贴图精度|贴图过滤|阴影|阴影半透|抗锯齿|HDR|FPS设置
	int setting[7] = {0};
	//bool alphaShadow = false;
	//if (settingindex < 0 || settingindex > 2)
	//{
	//	settingindex = 0;
	//}
	switch (settingindex)
	{
	case 0:
		setting[0] = 1;//贴图精度
		setting[1] = 2;//贴图过滤
		setting[2] = 0;//阴影
		setting[3] = 0;//阴影半透
		setting[4] = 0;//抗锯齿
		setting[5] = 0;//HDR
		setting[6] = 2;//FPS设置
		break;
	case 1:
		setting[0] = 1;//贴图精度
		setting[1] = 4;//贴图过滤
		setting[2] = 0;//阴影
		setting[3] = 0;//阴影半透
		setting[4] = 2;//抗锯齿
		setting[5] = 0;//HDR
		setting[6] = 2;//FPS设置
		break;
	case 2:
		setting[0] = 1;//贴图精度
		setting[1] = 8;//贴图过滤
		setting[2] = 2;//阴影
		setting[3] = 1;//阴影半透
		setting[4] = 4;//抗锯齿
		setting[5] = 0;//HDR
		setting[6] = 2;//FPS设置
		break;
	case 3:
		setting[0] = 1;//贴图精度
		setting[1] = 16;//贴图过滤
		setting[2] = 3;//阴影
		setting[3] = 1;//阴影半透
		setting[4] = 4;//抗锯齿
		setting[5] = 0;//HDR
		setting[6] = 2;//FPS设置
		break;
	case 4:
		setting[0] = 1;//贴图精度
		setting[1] = 16;//贴图过滤
		setting[2] = 3;//阴影
		setting[3] = 1;//阴影半透
		setting[4] = 2;//抗锯齿
		setting[5] = 1;//HDR
		setting[6] = 2;//FPS设置
		break;
	default:
		setting[0] = 1;//贴图精度
		setting[1] = 2;//贴图过滤
		setting[2] = 0;//阴影
		setting[3] = 0;//阴影半透
		setting[4] = 0;//抗锯齿
		setting[5] = 0;//HDR
		setting[6] = 2;//FPS设置
		break;
	}

	crDisplaySettings::instance()->setTextureDetail(setting[0]);
	crDisplaySettings::instance()->setMaxAnisotropy(setting[1]);
	crDisplaySettings::instance()->setShadowDetail(setting[2]);
	crDisplaySettings::instance()->setEnableAlphaShadow(setting[3]);
	crDisplaySettings::instance()->setNumMultiSamples(setting[4]);
	crDisplaySettings::instance()->setHDR(setting[5]);
	//crDisplaySettings::instance()->setFpsControl(setting[6]);

	//DeleteFile(".\\GameVideoSetting.ini");
	rcfg::ConfigScript cfg_script;
	std::string filename = "script/init.cfg";
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,filename,true))
	{
		CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJFirstGameVideoSettingMethod(): file open error "<<filename<<std::endl;
		//sprintf(gDebugInfo->buf(),"crJXJUpdateInitCfgMethod: file open error%s\n\0",filename.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}
	if(cfg_script.Push("DisplaySetting"))
	{
		if(!cfg_script.Modify("TextureDetail", setting[0]))
		{
			cfg_script.Add("TextureDetail",setting[0]);
		}
		if(!cfg_script.Modify("MaxAnisotropy", setting[1]))
		{
			cfg_script.Add("MaxAnisotropy",setting[1]);
		}
		if(!cfg_script.Modify("ShadowDetail", setting[2]))
		{
			cfg_script.Add("ShadowDetail",setting[2]);
		}
		if(!cfg_script.Modify("EnableAlphaShadow", setting[3]))
		{
			cfg_script.Add("EnableAlphaShadow",setting[3]);
		}
		if(!cfg_script.Modify("numMultiSamples", setting[4]))
		{
			cfg_script.Add("numMultiSamples",setting[4]);
		}
		if(!cfg_script.Modify("HDR", setting[5]))
		{
			cfg_script.Add("HDR",setting[5]);
		}
		if (!cfg_script.Pop())
		{
			//CRCore::notify(CRCore::FATAL)<<"crUpdateInitCfgMethod::operator(): "<<cfg_script.GetLastError()<<std::endl;
			char gbuf[256];
			sprintf(gbuf,"crUpdateInitCfgMethod::operator():%s\n\0",cfg_script.GetLastError().c_str());
			gDebugInfo->debugInfo(CRCore::FATAL,gbuf);
		}

	}
	cfg_script.Write("script/init.cfg");
	cfg_script.Close();

	CRIOManager::crWriteCookFile scopedWrite("script/init.cfg");
}
/////////////////////////////////////////
//
//crJXJGMGetPlayerDataMethod
//
/////////////////////////////////////////
crJXJGMGetPlayerDataMethod::crJXJGMGetPlayerDataMethod():
	m_this(NULL),
	m_playerid(0)
{}
crJXJGMGetPlayerDataMethod::crJXJGMGetPlayerDataMethod(const crJXJGMGetPlayerDataMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_playerid(handle.m_playerid),
	m_playerdata(handle.m_playerdata)
{
}
void crJXJGMGetPlayerDataMethod::inputParam(int i, void *param)
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
			m_playerdata = (crGameServerPlayerData *)(LOINT64(param64));
			m_playerid = *(int *)(HIINT64(param64));
		}
		else
		{
			m_playerdata = NULL;
		}
		break;
	}
}

void crJXJGMGetPlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJGMGetPlayerDataMethod::operator()(crHandle &handle)
{
	//ref_ptr<crGameServerPlayerData> playerData = NULL;
	//playerData = new crGameServerPlayerData;
	if (!m_playerdata)
	{
		return;
	}
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	crGameServerCallback *netCallback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
	m_playerdata->setPlayerID(m_playerid);
	unsigned short gamedbid = 1;
	bool found = false;
	int money = 0;
	crDataBase *db = crDataBaseManager::getInstance()->getGlobalDB();
	ref_ptr<crDataBase> globalSession = db->beginSession();
	ref_ptr<crPlayerGameDBQueryData> playergamedbQuery = new crPlayerGameDBQueryData;
	playergamedbQuery->buildQuerySql(m_playerid,netCallback->getServerID());

	globalSession->executeQuery(playergamedbQuery.get());
	crDataBase::QueryResultVec &playergamedbQueryResultVec = globalSession->getQueryResult();
	if(!playergamedbQueryResultVec.empty())
	{
		playergamedbQuery = dynamic_cast<crPlayerGameDBQueryData *>(playergamedbQueryResultVec[0].get());
		gamedbid = playergamedbQuery->getGameDBID();
		found = true;
	}
	globalSession->releaseQuery();

	if(found)
	{
		ref_ptr<crPlayerMoneyQueryData> playerMoneyQuery = new crPlayerMoneyQueryData;
		playerMoneyQuery->buildQuerySql(m_playerid,netCallback->getServerID());

		globalSession->executeQuery(playerMoneyQuery.get());
		crDataBase::QueryResultVec &playerMoneyQueryResultVec = globalSession->getQueryResult();
		if(!playerMoneyQueryResultVec.empty())
		{
			playerMoneyQuery = dynamic_cast<crPlayerMoneyQueryData *>(playerMoneyQueryResultVec[0].get());
			money = playerMoneyQuery->getMoney();
		}
		globalSession->releaseQuery();
	}
	db->endSession(globalSession.get());
	//加载playergamedata
	if(found)
	{
		m_playerdata->setGameDBID(gamedbid);
		////////playerGameData查询
		ref_ptr<crPlayerGameData> playerGameData;
		std::string script = netCallback->getGameName() + "/playergamedata.cfg";
		ref_ptr<crData> data = CREncapsulation::loadData(script);
		if(data.valid())
		{
			playerGameData = new crPlayerGameData;
			playerGameData->setPlayerID(m_playerid);
			playerGameData->setDataClass(data.get());
			data->inputParam(WCHDATA_Money,&money);
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,m_playerdata->getGameDBID());
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				ref_ptr<crPlayerGameDataQueryData> playerGameDataQuery = new crPlayerGameDataQueryData;
				playerGameDataQuery->buildQuerySql(m_playerid);

				gameSession->executeQuery(playerGameDataQuery.get());
				crDataBase::QueryResultVec &playerGameDataQueryResultVec = gameSession->getQueryResult();
				if(!playerGameDataQueryResultVec.empty())
				{
					playerGameDataQuery = dynamic_cast<crPlayerGameDataQueryData *>(playerGameDataQueryResultVec[0].get());
					playerGameData->setID(playerGameDataQuery->getID());
					playerGameData->setStream(playerGameDataQuery->getStream());
					int roleid = playerGameDataQuery->getMainRoleID();
					if(roleid > 0)
					{//查询并设置mainrole
						playerGameData->setMainRoleID(roleid);
					}
					playerGameData->setDBValid(true);
				}
				gameSession->releaseQuery();
				gamedb->endSession(gameSession.get());
				if(playerGameData->getStream())
				{
					data->inputParam(WCHDATA_DataStream,playerGameData->getStream());
					playerGameData->setStream(NULL);
				}
				m_playerdata->setPlayerGameData(playerGameData.get());
			}
			else
			{
				CRCore::notify(CRCore::ALWAYS)<<"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = "<<m_playerdata->getGameDBID()<<std::endl;
				//sprintf(gDebugInfo->buf(),"crJXJCheckPlayerDataDisposeMethod Error 没有找到GameDB dbid = %d\n\0",playerData->getGameDBID());
				//gDebugInfo->debugInfo(CRCore::ALWAYS);
			}
		}
	}
	////加载mainrole
	if(m_playerdata)
	{
		unsigned short gamedbid = m_playerdata->getGameDBID();
		bool isMainRole = true;
		crPlayerGameData *playerGameData = m_playerdata->getPlayerGameData();
		if(playerGameData)
		{
			int roleid = playerGameData->getMainRoleID();
			ref_ptr<crRole> role;
			CRCore::ref_ptr<crItemChild> itemChild;
			crDataBase *gamedb = crDataBaseManager::getInstance()->getDataBase(GameDB,gamedbid);
			if(gamedb)
			{
				ref_ptr<crDataBase> gameSession = gamedb->beginSession();
				bool game_ended = false;
				ref_ptr<crRoleQueryData> roleQuery = new crRoleQueryData;
				roleQuery->buildQuerySql(roleid);
				gameSession->executeQuery(roleQuery.get());
				crDataBase::QueryResultVec &roleQueryResultVec = gameSession->getQueryResult();
				int ametierid = 0;
				if(!roleQueryResultVec.empty())
				{
					role = new crRole;
					roleQuery = dynamic_cast<crRoleQueryData*>(roleQueryResultVec[0].get());
					role->setRoleID(roleid);
					role->setPlayerID(m_playerid);
					role->setInstanceItemID(roleQuery->getInstanceItemID());
					ametierid = roleQuery->getAbstractMetierID();
					role->setMetierStream(roleQuery->getMetierStream());
				}
				gameSession->releaseQuery();
				//gamedb->endSession(gameSession.get());

				if(role.valid())
				{
					crAbstractMetier *aMetier = crGlobalHandle::getInstance()->gameServerQueryAbstractMetier(ametierid);
					role->setAbstractMetier(aMetier);

					if(role->getInstanceItemID()>0)
					{
						ref_ptr<crInstanceItemQueryData> instanceItemQuery = new crInstanceItemQueryData;
						instanceItemQuery->buildQuerySql(role->getInstanceItemID());
						gameSession->executeQuery(instanceItemQuery.get());
						crDataBase::QueryResultVec &instanceItemQueryResultVec = gameSession->getQueryResult();
						int abstractid = 0;
						if(!instanceItemQueryResultVec.empty())
						{
							instanceItemQuery = dynamic_cast<crInstanceItemQueryData*>(instanceItemQueryResultVec[0].get());
							abstractid = instanceItemQuery->getAbstractItemID();
							//role->setAbstractItemID();
							role->setItemtype(instanceItemQuery->getItemtype());
							role->setIName(instanceItemQuery->getIName());
							role->setSceneID(instanceItemQuery->getSceneID());
							role->setLayerID(instanceItemQuery->getLayerid());
							role->setPosxy(instanceItemQuery->getPosx(),instanceItemQuery->getPosy());
							role->setZoffset(instanceItemQuery->getZoffset());
							role->setDir(crVector3(instanceItemQuery->getDirx(),instanceItemQuery->getDiry(),instanceItemQuery->getDirz()));
							role->setStream(instanceItemQuery->getStream());
						}
						gameSession->releaseQuery();

						//if(cAbstractID!=0)
						//	abstractid = cAbstractID;
						crAbstractItem *aitem = crGlobalHandle::getInstance()->gameServerQueryAbstractItem(abstractid);
						role->setAbstractItem(aitem);

						////ItemChild查询
					}
					gamedb->endSession(gameSession.get());
					game_ended = true;
					if(isMainRole)
					{
						playerGameData->setMainRole(role.get());
						role->setSceneID(0);
						role->setRoomID(0);
						role->loadMetierData(SaveStream);
						role->loadItemData(SaveStream,true);
						//role->getDataClass()->excHandle(MAKEINT64(WCH_GameServerCheckData,role.get()));
						//role->doEvent(WCH_MainRoleInit,MAKEINT64(playerGameData,NULL));
					}
				}
				if(!game_ended) gamedb->endSession(gameSession.get());
			}
		}
	}
	//if (!found)
	//{
	//	return NULL;
	//}
	//return playerData.get();
}
/////////////////////////////////////////
//
//crJXJGMChangePlayerDataMethod
//
/////////////////////////////////////////
crJXJGMChangePlayerDataMethod::crJXJGMChangePlayerDataMethod():
	m_this(NULL)
{}
crJXJGMChangePlayerDataMethod::crJXJGMChangePlayerDataMethod(const crJXJGMChangePlayerDataMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJGMChangePlayerDataMethod::inputParam(int i, void *param)
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

void crJXJGMChangePlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJGMChangePlayerDataMethod::operator()(crHandle &handle)
{
	//T_Food = 1,
	//	T_Wood,
	//	T_Iron,
	//	T_Horse,
	//	T_Copper,
	//	T_Exp,
	//	T_Achievement,
	//	T_Giftgold,
	//	T_TroopsEquip,
	//	T_Item,
	//	T_Troops,
	//	T_Exploit,
	//	T_Contribute,//国家贡献，新增加
	//	T_Gold,//14
	//	T_JiangHun,//将魂
	//	T_CardImpression,//将卡好感度
	//	T_JunLing,//军令
	crData *playerData = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
	CRNetApp::crPlayerDataEventPacket packet;
	CRCore::ref_ptr<CRCore::crStreamBuf> stream;
	stream = new CRCore::crStreamBuf;
	std::string playername;
	unsigned char changetype = 0;
	int changeid = 0;
	int changecount = 0;
	unsigned char goldtype = 0;
	switch (changetype)
	{
	case T_Copper:
	case T_Wood:
	case T_Food:
	case T_Iron:
	case T_Horse:
	case T_Exp:
	case T_Achievement:
	case T_Giftgold:
	case T_Exploit:
	case T_Contribute:
	case T_JiangHun:
	case T_CardImpression:
	case T_JunLing:
	case T_Level:
		stream->createBuf(4);
		stream->_writeInt(changecount);
		break;
	case T_Item:
	case T_Troops:
		stream->createBuf(8);
		stream->_writeInt(changeid);
		stream->_writeInt(changecount);
	case T_Gold:
		stream->createBuf(5);
		stream->_writeUChar(goldtype);
		stream->_writeInt(changecount);
	default:
		break;
	}
	int bufsize = stream->getBufSize();
	bufsize += 5+playername.size();
	stream->setBufSize(bufsize);
	stream->seekBegin();
	stream->_writeUChar(changetype);
	stream->_writeString(playername);
	CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGMChangePlayerData,stream.get());
	net_conductor->getNetManager()->sendPacket("all",packet);
}
/////////////////////////////////////////
//
//crJXJRecvGMChangePlayerDataMethod
//
/////////////////////////////////////////
crJXJRecvGMChangePlayerDataMethod::crJXJRecvGMChangePlayerDataMethod():
	m_this(NULL),
	m_netType(GameClient_Game)
{}
crJXJRecvGMChangePlayerDataMethod::crJXJRecvGMChangePlayerDataMethod(const crJXJRecvGMChangePlayerDataMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJRecvGMChangePlayerDataMethod::inputParam(int i, void *param)
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

void crJXJRecvGMChangePlayerDataMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvGMChangePlayerDataMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			void *param;
			//int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			unsigned char changetype = m_stream->_readUChar();
			std::string playername = m_stream->_readString();
			//get playerid
			//CRNet::crNetConductor *gameserver = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer); // packet replay and tip 
			crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
			int playerid = CRNetApp::crGlobalHandle::getInstance()->gameServerQueryPlayerID(playername,callback->getServerID());
			ref_ptr<crGameServerPlayerData> gameserverplayerdata = new crGameServerPlayerData;
			m_this->doEvent(WCH_JXJGMGetPlayerData,MAKEINT64(gameserverplayerdata.get(),playerid));
			int changeid = 0;
			int changecount = 0;
			unsigned char goldtype = 0;
			int *curcountint = 0;
			unsigned short *curcountunshort = 0;
			unsigned char *curcountunchar = 0;
			if (gameserverplayerdata.valid() && gameserverplayerdata->getPlayerGameData() && gameserverplayerdata->getPlayerGameData()->getMainRole())
			{
				crData *playerdata = gameserverplayerdata->getPlayerGameData()->getDataClass();
				crData *playermainroledata = gameserverplayerdata->getPlayerGameData()->getMainRole()->getDataClass();
				switch (changetype)
				{
				case T_Copper:
				case T_Wood:
				case T_Food:
				case T_Iron:
				case T_Horse:
				case T_Exp:
				case T_Achievement:
				case T_Giftgold:
				case T_Exploit:
				case T_Contribute:
				case T_JiangHun:
				case T_CardImpression:
				case T_JunLing:
				case T_Level:
					changecount = m_stream->_readInt();
					break;
				case T_Item:
				case T_Troops:
					changeid = m_stream->_readInt();
					changecount = m_stream->_readInt();
				case T_Gold:
					goldtype = m_stream->_readUChar();
					changecount = m_stream->_readInt();
				default:
					break;
				}
				switch (changetype)
				{
				case T_Copper:
					playerdata->getParam(WCHDATA_JXJCoppercash,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Wood:
					playerdata->getParam(WCHDATA_JXJWood,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Food:
					playerdata->getParam(WCHDATA_JXJFood,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Iron:
					playerdata->getParam(WCHDATA_JXJIron,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Horse:
					playerdata->getParam(WCHDATA_JXJHorse,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Exp:
					playermainroledata->getParam(WCHDATA_Experience,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Achievement:
					playermainroledata->getParam(WCHDATA_JXJAchievement,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Giftgold:
					playerdata->getParam(WCHDATA_JXJGiftGold,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Exploit:
					playermainroledata->getParam(WCHDATA_JXJExploit,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_Contribute:
					playerdata->getParam(WCHDATA_JXJCountryContribute,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_JiangHun:
					playerdata->getParam(WCHDATA_JXJCardPieces,param);
					curcountint = (int *)param;
					(*curcountint) += changecount;
					break;
				case T_CardImpression:
					playerdata->getParam(WCHDATA_JXJCardImpression,param);
					curcountunshort = (unsigned short *)param;
					if (changecount < 0 && (-changecount) > (*curcountunshort))
					{
						(*curcountunshort) = 0;
					}
					else
					{
						(*curcountunshort) += changecount;
					}
					break;
				case T_JunLing:
					playerdata->getParam(WCHDATA_JXJJunling,param);
					curcountunshort = (unsigned short *)param;
					changecount *= 100;
					if (changecount < 0 && (-changecount) > (*curcountunshort))
					{
						(*curcountunshort) = 0;
					}
					else
					{
						(*curcountunshort) += changecount;
					}
					break;
				case T_Level:
					playermainroledata->getParam(WCHDATA_Level,param);
					curcountunchar = (unsigned char *)param;
					if (changecount < 0 && (-changecount) > (*curcountunchar))
					{
						(*curcountunchar) = 0;
					}
					else
					{
						(*curcountunchar) += changecount;
					}
					break;
				case T_Item:
				case T_Troops:
					changeid = m_stream->_readInt();
					changecount = m_stream->_readInt();
				case T_Gold:
					goldtype = m_stream->_readUChar();
					changecount = m_stream->_readInt();
				default:
					break;
				}
				if ((*curcountint) < 0)
				{
					(*curcountint) = 0;
				}
			}

		}
		else if(m_netType == GameClient_Game)
		{
		}
	}
}
void crJXJRecvGMChangePlayerDataMethod::ChangeResource(int changetype,int changecount)
{
}
void crJXJRecvGMChangePlayerDataMethod::ChangeItems(int changeid,int changecount)
{
}
void crJXJRecvGMChangePlayerDataMethod::ChangeTroops(int changeid,int changecount)
{
}
void crJXJRecvGMChangePlayerDataMethod::ChangeGold(unsigned char goldtype,int changecount)
{
}
/////////////////////////////////////////
//
//crJXJResetFormationPageMethod
//
/////////////////////////////////////////
crJXJResetFormationPageMethod::crJXJResetFormationPageMethod()
{
}

crJXJResetFormationPageMethod::crJXJResetFormationPageMethod(const crJXJResetFormationPageMethod& handle)
{
}

void crJXJResetFormationPageMethod::inputParam(int i, void *param)
{
}

void crJXJResetFormationPageMethod::addParam(int i, const std::string& str)
{
}

void crJXJResetFormationPageMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *braindata = crBrain::getInstance()->getDataClass();
		if (braindata)
		{
			int curpage = 1;
			braindata->inputParam(WCHDATA_JXJFormationCurPage, &curpage);
		}
	}
}
/////////////////////////////////////////
//
//crJXJResetTechNewBtnFlagMethod
//跳过新手引导
/////////////////////////////////////////
crJXJResetTechNewBtnFlagMethod::crJXJResetTechNewBtnFlagMethod():
	m_this(NULL)
{
}
crJXJResetTechNewBtnFlagMethod::crJXJResetTechNewBtnFlagMethod(const crJXJResetTechNewBtnFlagMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJResetTechNewBtnFlagMethod::inputParam(int i, void *param)
{
}
void crJXJResetTechNewBtnFlagMethod::addParam(int i, const std::string& str)
{
}
void crJXJResetTechNewBtnFlagMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		CRCore::ScopedLock<crData> lock(*data);
		data -> getParam(WCHDATA_JXJIfTechBtnNew,param);
		unsigned char *skipnewhand = (unsigned char *)param;
		if (*skipnewhand)
		{
			*skipnewhand = 0;
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvResetTechNewBtnFlag,NULL);
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvResetTechNewBtnFlagMethod
//跳过新手引导
/////////////////////////////////////////
crJXJRecvResetTechNewBtnFlagMethod::crJXJRecvResetTechNewBtnFlagMethod():
	m_netType(GameClient_Game){}
crJXJRecvResetTechNewBtnFlagMethod::crJXJRecvResetTechNewBtnFlagMethod(const crJXJRecvResetTechNewBtnFlagMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvResetTechNewBtnFlagMethod::inputParam(int i, void *param)
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

void crJXJRecvResetTechNewBtnFlagMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvResetTechNewBtnFlagMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			crData *data = m_this->getDataClass();
			CRCore::ScopedLock<crData> lock(*data);
			unsigned char skipnewhand = 0;
			data -> inputParam(WCHDATA_JXJIfTechBtnNew,&skipnewhand);
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetVipCanvasPageNumMethod
//
/////////////////////////////////////////
crJXJSetVipCanvasPageNumMethod::crJXJSetVipCanvasPageNumMethod():
	m_this(NULL)
{
}
crJXJSetVipCanvasPageNumMethod::crJXJSetVipCanvasPageNumMethod(const crJXJSetVipCanvasPageNumMethod& handle):
	crMethod(handle)
{
}
void crJXJSetVipCanvasPageNumMethod::inputParam(int i, void *param)
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
void crJXJSetVipCanvasPageNumMethod::addParam(int i, const std::string& str)
{
}
void crJXJSetVipCanvasPageNumMethod::operator()(crHandle &handle)
{	
	void *param;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_VipLv,param);
		unsigned char viplv = *(unsigned char *)param;
		ref_ptr<crTableIO>viptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
		int totalpagenum = viptab->getRowCount();
		crData *canvasdata = m_this->getDataClass();
		int curpagenum = viplv;
		canvasdata->inputParam(WCHDATA_JXJVipCanvasCurPageNum,&curpagenum);
		canvasdata->inputParam(WCHDATA_JXJVipCanvasTotalPageNum,&totalpagenum);
	}
}
/////////////////////////////////////////
//
//crJXJSearchBuildingLvCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchBuildingLvCompleteCheckMethod::crJXJSearchBuildingLvCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL),
	m_buildingid(0)
{
}
crJXJSearchBuildingLvCompleteCheckMethod::crJXJSearchBuildingLvCompleteCheckMethod(const crJXJSearchBuildingLvCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_buildingid(handle.m_buildingid)
{
}
void crJXJSearchBuildingLvCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchBuildingLvCompleteCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	}
}
void crJXJSearchBuildingLvCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player && m_buildingid > 0)
	{
		void *param;
		int maxcount = 0;
		unsigned char buildinglv = 0;
		unsigned char maxlv = 0;
		crData *data = m_player->getDataClass();
		switch (m_buildingid)
		{
		case WCHDATA_JXJFuyalv:
		case WCHDATA_JXJBingyinglv:
		case WCHDATA_JXJJiusilv:
		case WCHDATA_JXJGongfanglv:
		case WCHDATA_JXJShijilv:
		case WCHDATA_JXJFukulv:
			maxcount = 1;
			break;
		case WCHDATA_JXJMingjulv1:
		case WCHDATA_JXJNongtianlv1:
			maxcount = 8;
			break;
		case WCHDATA_JXJMuchanglv1:
		case WCHDATA_JXJLingdilv1:
		case WCHDATA_JXJKuangshanlv1:
			maxcount = 4;
			break;
		default:
			maxcount = 0;
			break;
		}
		if (maxcount > 0)
		{
			for (int i = 0;i < maxcount; ++i)
			{
				data->getParam(m_buildingid + i,param);
				buildinglv = *(unsigned char*)param;
				if (buildinglv > maxlv)
				{
					maxlv = buildinglv;
					m_this->setStep(maxlv);
					if(m_this->isComplete())
					{
						break;
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchBuildingLvMethod
//
/////////////////////////////////////////
crJXJSearchBuildingLvMethod::crJXJSearchBuildingLvMethod():
	m_this(NULL),
	m_buildingid(0)
{
}
crJXJSearchBuildingLvMethod::crJXJSearchBuildingLvMethod(const crJXJSearchBuildingLvMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_buildingid(handle.m_buildingid)
{
}
void crJXJSearchBuildingLvMethod::inputParam(int i, void *param)
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
void crJXJSearchBuildingLvMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_buildingid = atoi(str.c_str());
		break;
	}
}
void crJXJSearchBuildingLvMethod::operator()(crHandle &handle)
{
	if (!m_this->getComplete())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(myPlayer && m_buildingid > 0)
		{
			void *param;
			int maxcount = 0;
			unsigned char buildinglv = 0;
			unsigned char maxlv = 0;
			crData *data = myPlayer->getDataClass();
			switch (m_buildingid)
			{
			case WCHDATA_JXJFuyalv:
			case WCHDATA_JXJBingyinglv:
			case WCHDATA_JXJJiusilv:
			case WCHDATA_JXJGongfanglv:
			case WCHDATA_JXJShijilv:
			case WCHDATA_JXJFukulv:
				maxcount = 1;
				break;
			case WCHDATA_JXJMingjulv1:
			case WCHDATA_JXJNongtianlv1:
				maxcount = 8;
				break;
			case WCHDATA_JXJMuchanglv1:
			case WCHDATA_JXJLingdilv1:
			case WCHDATA_JXJKuangshanlv1:
				maxcount = 4;
				break;
			default:
				maxcount = 0;
				break;
			}
			if (maxcount > 0)
			{
				for (int i = 0;i < maxcount; ++i)
				{
					data->getParam(m_buildingid + i,param);
					buildinglv = *(unsigned char*)param;
					if (buildinglv > maxlv)
					{
						maxlv = buildinglv;
						m_this->setStep(maxlv);
						//if(m_this->isComplete())
						//{
						//	//ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
						//	//if(canvas.valid() && canvas->getDataClass())
						//	//{
						//	//	crData *data = canvas->getDataClass();
						//	//	data->inputParam(WCHDATA_JXJVisiableTime,&m_time);

						//	//	unsigned short accomplishid = *(unsigned short *)param;
						//	//	data->inputParam(WCHDATA_JXJCurAccomplishID,&accomplishid);
						//	//	crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
						//	//}
						//	break;
						//}
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchResourceCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchResourceCompleteCheckMethod::crJXJSearchResourceCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL),
	m_resourceid(0)
{
}
crJXJSearchResourceCompleteCheckMethod::crJXJSearchResourceCompleteCheckMethod(const crJXJSearchResourceCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_resourceid(handle.m_resourceid)
{
}
void crJXJSearchResourceCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchResourceCompleteCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_resourceid = atoi(str.c_str());
		break;
	}
}
void crJXJSearchResourceCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player && m_resourceid > 0)
	{
		void *param;
		int curstep = 0;
		crData *data = m_player->getDataClass();
		data->getParam(m_resourceid,param);
		int resourcecount = *(int *)param;
		int jishu = 1;
		switch (m_resourceid)
		{
		case WCHDATA_JXJGoldingot:
			jishu = 1;
			break;
		default:
			jishu = c_onehundredthousand;
			break;
		}
		curstep = resourcecount/jishu;
		m_this->setStep(curstep);
	}
}
/////////////////////////////////////////
//
//crJXJSearchResourceMethod
//
/////////////////////////////////////////
crJXJSearchResourceMethod::crJXJSearchResourceMethod():
	m_this(NULL),
	m_resourceid(0)
{
}
crJXJSearchResourceMethod::crJXJSearchResourceMethod(const crJXJSearchResourceMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_resourceid(handle.m_resourceid)
{
}
void crJXJSearchResourceMethod::inputParam(int i, void *param)
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
void crJXJSearchResourceMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_resourceid = atoi(str.c_str());
		break;
	}
}
void crJXJSearchResourceMethod::operator()(crHandle &handle)
{
	if (!m_this->getComplete())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(myPlayer && m_resourceid > 0)
		{
			void *param;
			int curstep = 0;
			crData *data = myPlayer->getDataClass();
			data->getParam(m_resourceid,param);
			int resourcecount = *(int *)param;
			int jishu = 1;
			switch (m_resourceid)
			{
			case WCHDATA_JXJGoldingot:
				jishu = 1;
				break;
			default:
				jishu = c_onehundredthousand;
				break;
			}
			curstep = resourcecount/jishu;
			m_this->setStep(curstep);
			//if(m_this->isComplete())
			//{
			//	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			//	if(canvas.valid() && canvas->getDataClass())
			//	{
			//		crData *data = canvas->getDataClass();
			//		unsigned short accomplishid = m_this->getID();
			//		data->inputParam(WCHDATA_JXJVisiableTime,&m_time);
			//		data->inputParam(WCHDATA_JXJCurAccomplishID,&accomplishid);

			//		crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
			//	}
			//}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchGovPostCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchGovPostCompleteCheckMethod::crJXJSearchGovPostCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchGovPostCompleteCheckMethod::crJXJSearchGovPostCompleteCheckMethod(const crJXJSearchGovPostCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJSearchGovPostCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchGovPostCompleteCheckMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchGovPostCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player && m_player->getMainRole() && m_player->getMainRole()->getDataClass())
	{
		void *param;
		int curstep = 0;

		crData *data = m_player->getMainRole()->getDataClass();
		data->getParam(WCHDATA_JXJGovPost,param);
		unsigned char govpost = *(unsigned char *)param;
		m_this->setStep(govpost - 1);
	}
}
/////////////////////////////////////////
//
//crJXJSearchGovPostMethod
//
/////////////////////////////////////////
crJXJSearchGovPostMethod::crJXJSearchGovPostMethod():
	m_this(NULL)
{
}
crJXJSearchGovPostMethod::crJXJSearchGovPostMethod(const crJXJSearchGovPostMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchGovPostMethod::inputParam(int i, void *param)
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
void crJXJSearchGovPostMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchGovPostMethod::operator()(crHandle &handle)
{
	if (!m_this->getComplete())
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(myPlayer && myPlayer->getMainRole() && myPlayer->getMainRole()->getDataClass())
		{
			void *param;
			int curstep = 0;
			crData *data = myPlayer->getMainRole()->getDataClass();
			data->getParam(WCHDATA_JXJGovPost,param);
			unsigned char govpost = *(unsigned char *)param;
			m_this->setStep(govpost - 1);
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchChapterPassCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchChapterPassCompleteCheckMethod::crJXJSearchChapterPassCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchChapterPassCompleteCheckMethod::crJXJSearchChapterPassCompleteCheckMethod(const crJXJSearchChapterPassCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJSearchChapterPassCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchChapterPassCompleteCheckMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchChapterPassCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0, crArgumentParser::appItoa((int)achieveid), record) >= 0)
		{
			CRCore::crVector2i notevec;
			crArgumentParser::appAtoVec(record[beizhuindex], notevec);
			int needchapterid = notevec[1];
			ref_ptr<crTableIO>fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec record1;
			int chapterIndex = fubenTab->getTitleIndex("chapterid");
			int chapterid;
			crData *data = m_player->getDataClass();
			void *param;
			int passcount = 0;
			unsigned short fubenid;
			data->excHandle(MAKEINT64(WCH_LockData, 1));
			data->getParam(WCHDATA_JXJFubenProgressMap, param);
			FubenProgressMap *fubenProgressMap = (FubenProgressMap *)param;
			for (FubenProgressMap::iterator itr = fubenProgressMap->begin();
				itr != fubenProgressMap->end();
				++itr)
			{
				if (itr->second->getComplete())
				{
					fubenid = itr->first;
					if (fubenTab->queryOneRecord(0, crArgumentParser::appItoa(fubenid), record1) >= 0)
					{
						chapterid = atoi(record1[chapterIndex].c_str());
						if (chapterid == needchapterid)
						{
							passcount++;
							m_this->setStep(passcount);
							if (m_this->isComplete())
							{
								break;
							}
						}
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData, 0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchChapterPassMethod
//
/////////////////////////////////////////
crJXJSearchChapterPassMethod::crJXJSearchChapterPassMethod():
	m_this(NULL)
{
}
crJXJSearchChapterPassMethod::crJXJSearchChapterPassMethod(const crJXJSearchChapterPassMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchChapterPassMethod::inputParam(int i, void *param)
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
void crJXJSearchChapterPassMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchChapterPassMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		unsigned short achieveid = m_this->getID();
		ref_ptr<crTableIO> accomplish_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJAccomplishTab);
		int beizhuindex = accomplish_table->getTitleIndex("备注");
		std::vector<std::string> record;
		if (accomplish_table->queryOneRecord(0, crArgumentParser::appItoa((int)achieveid), record) >= 0)
		{
			CRCore::crVector2i notevec;
			crArgumentParser::appAtoVec(record[beizhuindex], notevec);
			int needchapterid = notevec[1];
			ref_ptr<crTableIO>fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec record1;
			int chapterIndex = fubenTab->getTitleIndex("chapterid");
			int chapterid;
			crData *data = myPlayer->getDataClass();
			void *param;
			int passcount = 0;
			unsigned short fubenid;
			data->excHandle(MAKEINT64(WCH_LockData, 1));
			data->getParam(WCHDATA_JXJFubenProgressMap, param);
			FubenProgressMap *fubenProgressMap = (FubenProgressMap *)param;
			for (FubenProgressMap::iterator itr = fubenProgressMap->begin();
				itr != fubenProgressMap->end();
				++itr)
			{
				if (itr->second->getComplete())
				{
					fubenid = itr->first;
					if (fubenTab->queryOneRecord(0, crArgumentParser::appItoa(fubenid), record1) >= 0)
					{
						chapterid = atoi(record1[chapterIndex].c_str());
						if (chapterid == needchapterid)
						{
							passcount++;
							m_this->setStep(passcount);
							if (m_this->isComplete())
							{
								break;
							}
						}
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData, 0));
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchFubenStarCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchFubenStarCompleteCheckMethod::crJXJSearchFubenStarCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchFubenStarCompleteCheckMethod::crJXJSearchFubenStarCompleteCheckMethod(const crJXJSearchFubenStarCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJSearchFubenStarCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchFubenStarCompleteCheckMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchFubenStarCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		crData *data = m_player->getDataClass();
		void *param;
		int passcount = 0;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgressMap = (FubenProgressMap *)param;
		for( FubenProgressMap::iterator itr = fubenProgressMap->begin();
			itr != fubenProgressMap->end();
			++itr )
		{
			if(itr->second->getComplete())
			{
				if(itr->second->getStarCount() == 3)
				{
					passcount++;
					m_this->setStep(passcount);
					if(m_this->isComplete())
					{
						break;
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchFubenStarMethod
//
/////////////////////////////////////////
crJXJSearchFubenStarMethod::crJXJSearchFubenStarMethod():
	m_this(NULL)
{
}
crJXJSearchFubenStarMethod::crJXJSearchFubenStarMethod(const crJXJSearchFubenStarMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchFubenStarMethod::inputParam(int i, void *param)
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
void crJXJSearchFubenStarMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchFubenStarMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		crData *data = myPlayer->getDataClass();
		void *param;
		int passcount = 0;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJFubenProgressMap,param);
		FubenProgressMap *fubenProgressMap = (FubenProgressMap *)param;
		for( FubenProgressMap::iterator itr = fubenProgressMap->begin();
			itr != fubenProgressMap->end();
			++itr )
		{
			if(itr->second->getComplete())
			{
				if(itr->second->getStarCount() == 3)
				{
					passcount++;
					m_this->setStep(passcount);
					if(m_this->isComplete())
					{
						break;
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchEquipColorCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchEquipColorCompleteCheckMethod::crJXJSearchEquipColorCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL),
	m_equipcolor(0)
{
}
crJXJSearchEquipColorCompleteCheckMethod::crJXJSearchEquipColorCompleteCheckMethod(const crJXJSearchEquipColorCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_equipcolor(handle.m_equipcolor)
{
}
void crJXJSearchEquipColorCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchEquipColorCompleteCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_itemtype = atoi(str.c_str());
		break;
	case 1:
		m_equipcolor = atoi(str.c_str());
		break;
	}
}
void crJXJSearchEquipColorCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crData *data = m_player->getDataClass();
		void *param;
		int itemid = 0;
		int passcount = 0;
		JXJItemBagVec *itembagvec = NULL;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		if (m_itemtype != IT_Jiangka)
		{
			data->getParam(WCHDATA_JXJItemBagVec,param);
		}
		else
		{
			data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		}
		itembagvec = (JXJItemBagVec *)param;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr )
		{
			if ((*itr) != NULL)
			{
				itemid = (*itr)->getItemID();
				itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record1);
				if (atoi(record1[typeIndex].c_str()) == m_itemtype)
				{
					if (atoi(record1[colorIndex].c_str()) == m_equipcolor)
					{
						passcount++;
						m_this->setStep(passcount);
						if(m_this->isComplete())
						{
							break;
						}
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchEquipColorMethod
//
/////////////////////////////////////////
crJXJSearchEquipColorMethod::crJXJSearchEquipColorMethod():
	m_this(NULL),
	m_equipcolor(0)
{
}
crJXJSearchEquipColorMethod::crJXJSearchEquipColorMethod(const crJXJSearchEquipColorMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_equipcolor(handle.m_equipcolor)
{
}
void crJXJSearchEquipColorMethod::inputParam(int i, void *param)
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
void crJXJSearchEquipColorMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_itemtype = atoi(str.c_str());
		break;
	case 1:
		m_equipcolor = atoi(str.c_str());
		break;
	}
}
void crJXJSearchEquipColorMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crData *data = myPlayer->getDataClass();
		void *param;
		int itemid = 0;
		int passcount = 0;
		JXJItemBagVec *itembagvec = NULL;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		if (m_itemtype != IT_Jiangka)
		{
			data->getParam(WCHDATA_JXJItemBagVec,param);
		}
		else
		{
			data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		}
		itembagvec = (JXJItemBagVec *)param;
		for( JXJItemBagVec::iterator itr = itembagvec->begin();
			itr != itembagvec->end();
			++itr )
		{
			if ((*itr) != NULL)
			{
				itemid = (*itr)->getItemID();
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record1)>=0 && atoi(record1[typeIndex].c_str()) == m_itemtype)
				{
					if (atoi(record1[colorIndex].c_str()) == m_equipcolor)
					{
						passcount++;
						m_this->setStep(passcount);
						if(m_this->isComplete())
						{
							break;
						}
					}
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchItemCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchItemCompleteCheckMethod::crJXJSearchItemCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchItemCompleteCheckMethod::crJXJSearchItemCompleteCheckMethod(const crJXJSearchItemCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL),
	m_itemidvec(handle.m_itemidvec),
	m_itemcountvec(handle.m_itemcountvec)
{
}
void crJXJSearchItemCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchItemCompleteCheckMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_itemidvec);
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_itemcountvec);
		break;
	}
}
void crJXJSearchItemCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crData *data = m_player->getDataClass();
		void *param;
		int itemid = 0;
		int passcount = 0;
		int itemcount = 0;
		int curitemcount = 0;
		bool ischeckformation = true;
		JXJItemBagVec *itemvec = NULL;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
		data->getParam(WCHDATA_JXJFormationInfoVec, param);
		FormationInfoVec *formations = (FormationInfoVec *)param;
		for (int i = 0;i < m_itemidvec.size() && i < m_itemcountvec.size();++i)
		{
			ischeckformation = true;
			curitemcount = 0;
			itemid = m_itemidvec[i];
			itemcount = m_itemcountvec[i];
			itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record1);
			if (atoi(record1[typeIndex].c_str()) != IT_Jiangka)
			{
				itemvec = itembagvec;
			}
			else
			{
				itemvec = cardbagvec;
			}
			for( JXJItemBagVec::iterator itr = itemvec->begin();
				itr != itemvec->end();
				++itr )
			{
				if ((*itr) != NULL)
				{
					if(/*(*itr)->getItemID() == itemid*/(*itr)->getItemID() >= itemid && (*itr)->getItemID() <= itemid+5)
					{
						curitemcount += (*itr)->getItemCount();
						if (curitemcount >= itemcount)
						{
							curitemcount = itemcount;
							ischeckformation = false;
							break;
						}
					}
				}
			}
			if (ischeckformation && atoi(record1[typeIndex].c_str()) == IT_Jiangka)
			{
				for( FormationInfoVec::iterator itr = formations->begin();
					itr != formations->end();
					++itr )
				{
					crVector2i cardid = (*itr)->getEquips();

					if(cardid[0] >= itemid && cardid[0] <= itemid+5)
					{
						curitemcount += 1;
						if (curitemcount >= itemcount)
							curitemcount = itemcount;

						break;

					}
					if(cardid[1] >= itemid && cardid[1] <= itemid+5)
					{
						curitemcount += 1;
						if (curitemcount >= itemcount)
							curitemcount = itemcount;

						break;
					}
				}
			}
			passcount += curitemcount;
		}
		m_this->setStep(passcount);
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchItemColorMethod
//
/////////////////////////////////////////
crJXJSearchItemColorMethod::crJXJSearchItemColorMethod():
	m_this(NULL)
{
}
crJXJSearchItemColorMethod::crJXJSearchItemColorMethod(const crJXJSearchItemColorMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_itemidvec(handle.m_itemidvec),
	m_itemcountvec(handle.m_itemcountvec)
{
}
void crJXJSearchItemColorMethod::inputParam(int i, void *param)
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
void crJXJSearchItemColorMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_itemidvec);
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_itemcountvec);
		break;
	}
}
void crJXJSearchItemColorMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record1;
		int typeIndex = itemtab->getTitleIndex("类型");
		int colorIndex = itemtab->getTitleIndex("品质");
		crData *data = myPlayer->getDataClass();
		void *param;
		int itemid = 0;
		int itemcount = 0;
		int curitemcount = 0;
		int passcount = 0;
		bool ischeckformation = true;
		JXJItemBagVec *itemvec = NULL;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		data->getParam(WCHDATA_JXJJiangkaBagVec,param);
		JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
		data->getParam(WCHDATA_JXJFormationInfoVec, param);
		FormationInfoVec *formations = (FormationInfoVec *)param;
		for (int i = 0;i < m_itemidvec.size() && i < m_itemcountvec.size();++i)
		{
			ischeckformation = true;
			curitemcount = 0;
			itemid = m_itemidvec[i];
			itemcount = m_itemcountvec[i];
			itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record1);
			if (atoi(record1[typeIndex].c_str()) != IT_Jiangka)
			{
				itemvec = itembagvec;
			}
			else
			{
				itemvec = cardbagvec;
			}
			for( JXJItemBagVec::iterator itr = itemvec->begin();
				itr != itemvec->end();
				++itr )
			{
				if ((*itr) != NULL)
				{
					if(/*(*itr)->getItemID() == itemid*/(*itr)->getItemID() >= itemid && (*itr)->getItemID() <= itemid+5)
					{
						curitemcount += (*itr)->getItemCount();
						if (curitemcount >= itemcount)
						{
							curitemcount = itemcount;
							ischeckformation = false;
							break;
						}
					}
				}
			}
			if (ischeckformation && atoi(record1[typeIndex].c_str()) == IT_Jiangka)
			{
				for( FormationInfoVec::iterator itr = formations->begin();
					itr != formations->end();
					++itr )
				{
					crVector2i cardid = (*itr)->getEquips();
					
					if(cardid[0] >= itemid && cardid[0] <= itemid+5)
					{
						curitemcount += 1;
						if (curitemcount >= itemcount)
							curitemcount = itemcount;

						break;
							
					}
					if(cardid[1] >= itemid && cardid[1] <= itemid+5)
					{
						curitemcount += 1;
						if (curitemcount >= itemcount)
							curitemcount = itemcount;

						break;
					}
				}
			}
			passcount += curitemcount;
		}
		m_this->setStep(passcount);
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJUIFubenRushQuickEndMethod
//
/////////////////////////////////////////
crJXJUIFubenRushQuickEndMethod::crJXJUIFubenRushQuickEndMethod():
	m_this(NULL)
{
}
crJXJUIFubenRushQuickEndMethod::crJXJUIFubenRushQuickEndMethod(const crJXJUIFubenRushQuickEndMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_promptCanvas(handle.m_promptCanvas),
	m_information(handle.m_information),
	m_okbtn(handle.m_okbtn)
{
}
void crJXJUIFubenRushQuickEndMethod::inputParam(int i, void *param)
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
void crJXJUIFubenRushQuickEndMethod::addParam(int i, const std::string& str)
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
	default:
		break;
	}
}
void crJXJUIFubenRushQuickEndMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this &&playerGameData)
	{
		//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		void *param;
		int remaincount = 0;
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char viplv = *(unsigned char *)param;

		//检测VIP等级
		int needviplv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenRushQuickEndOpenViplv,viplv).c_str());
		if (viplv >= needviplv)
		{
			//检测元宝
			int needgoldpersecond = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushPerGold,viplv).c_str());
			int needgold,needgoldtemp;
			crData *brainData = crBrain::getInstance()->getDataClass();
			brainData->getParam(WCHDATA_JXJRemainFubenRushCount,param);
			remaincount = *(unsigned char *)param;
			needgold = needgoldtemp = needgoldpersecond * remaincount;
			playerGameData->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgoldtemp,NULL));
			if (needgoldtemp == 0)
			{
				//检测背包
				int needbagcount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushNeedBag,viplv).c_str());
				playerGameData->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagcount,NULL));
				if (needbagcount == 0)
				{
					data->getParam(WCHDATA_JXJGoldCostHintInt,param);
					unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_QuickEndFubenRushHint;
					ref_ptr<crCanvasNode>tipscanvas = crFilterRenderManager::getInstance()->findCanvas(m_promptCanvas);
					if (tipscanvas.valid())
					{
						ref_ptr<crHypertextWidgetNode> information = dynamic_cast<crHypertextWidgetNode *>(tipscanvas->getWidget(m_information));
						ref_ptr<crButtonWidgetNode> okbtn = dynamic_cast<crButtonWidgetNode *>(tipscanvas->getWidget(m_okbtn));
						if (ifhint)
						{
							int hinttype = JXJGoldCostType_QuickEndFubenRushHint;
							crData *braindata = crBrain::getInstance()->getDataClass();
							braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);
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
							cfg_script.Add("Text","立即完成扫荡吗？");
							cfg_script.Pop();


							cfg_script.Pop();

							if(information.valid())information->setHypertext(cfg_script);
						}
						if(okbtn.valid())okbtn->setEnable(true);
						crData *dlgData = tipscanvas->getDataClass();
						int commandtype = CDP_Widget;
						dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
						dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
						if (ifhint)
						{ 
							crFilterRenderManager::getInstance()->doModal(tipscanvas.get());
						}
						else
						{
							if(okbtn.valid())okbtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
						}
					}
				}
				else
				{
					playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2008,NULL));//背包不足
				}
			}
			else
			{
				playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2036,NULL));//元宝不足
			}
		}
		else
		{
			playerGameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(2101,NULL));//VIP等级不足
		}
	}
}
/////////////////////////////////////////
//
//crJXJFubenRushQuickEndMethod
//
/////////////////////////////////////////
crJXJFubenRushQuickEndMethod::crJXJFubenRushQuickEndMethod():
	m_this(NULL)
{
}
crJXJFubenRushQuickEndMethod::crJXJFubenRushQuickEndMethod(const crJXJFubenRushQuickEndMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJFubenRushQuickEndMethod::inputParam(int i, void *param)
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
void crJXJFubenRushQuickEndMethod::addParam(int i, const std::string& str)
{
}
void crJXJFubenRushQuickEndMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		//void *param;
		//crData *data = myPlayer->getDataClass();
		//unsigned char viplv = *(unsigned char *)param;
		//int needgoldpersecond = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushPerGold,viplv).c_str());
		//crData *brainData = crBrain::getInstance()->getDataClass();
		//brainData->getParam(WCHDATA_JXJRemainFubenRushCount,param);
		//unsigned char remaincount = *(unsigned char *)param;
		//int needgold = needgoldpersecond * remaincount;
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//stream->createBuf(4);
		//stream->_writeInt(needgold);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenRushQuickEnd,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvFubenRushQuickEndMethod
//
/////////////////////////////////////////
crJXJRecvFubenRushQuickEndMethod::crJXJRecvFubenRushQuickEndMethod():
	m_netType(GameClient_Game){}
crJXJRecvFubenRushQuickEndMethod::crJXJRecvFubenRushQuickEndMethod(const crJXJRecvFubenRushQuickEndMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvFubenRushQuickEndMethod::inputParam(int i, void *param)
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

void crJXJRecvFubenRushQuickEndMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvFubenRushQuickEndMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			char success = 0;
			void *param;
			bool iffindme = false;
			int nowjunlingcount = 0;
			bool needdeductgold = false;
			//int curgold = 0;
			int playerid = m_this->getPlayerID();
			crData *data = m_this->getDataClass();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid())
			{
				ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
				crTableIO::StrVec record;
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char viplv = *(unsigned char *)param;
				int needgoldpertemp = 0;
				int needgoldpersecond = needgoldpertemp = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushPerGold,viplv).c_str());
				int needgold,needgoldtemp;
				unsigned short fubenid = 0;
				int remainrushcount = 0;
				int needjunling;
				int junlingindex = fubentab->getTitleIndex("军令");
				int needbagnum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJQuickEndFubenRushNeedBag,viplv).c_str());
				m_this->doEvent(WCH_JXJCheckBagEmptyBag,MAKEINT64(&needbagnum,NULL));
				if (needbagnum != 0)
				{//背包不足
					success = -1;
				}
				else
				{
					crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJFubenRushMap,param);
					JXJFubenRushMap *rushplayermap = (JXJFubenRushMap *)param;
					for(JXJFubenRushMap::iterator itr = rushplayermap->begin();
						itr != rushplayermap->end();
						++itr)
					{
						for (JXJFubenRushVec::iterator vitr = itr->second.begin();
							vitr != itr->second.end();
							++vitr)
						{
							if (playerid == (*vitr)->getPlayerID())
							{
								iffindme = true;
								//检测元宝
								fubenid = (*vitr)->getFubenID();
								remainrushcount = (*vitr)->getRushCount();
								needgold = needgoldtemp = needgoldpersecond * remainrushcount;
								m_this->doEvent(WCH_JXJCheckLijinGold,MAKEINT64(&needgoldtemp,NULL));
								if (needgoldtemp != 0)
								{//元宝不足
									success = -2;
									break;
								}
								itr->second.erase(vitr);
								//if(itr->first != 0)
								//{
								//	ref_ptr<crRushPlayerInfo> playerinfo = new crRushPlayerInfo;
								//	playerinfo->setPlayerID((*vitr)->getPlayerID());
								//	playerinfo->setAllRushCount((*vitr)->getAllRushCount());
								//	playerinfo->setFubenID(fubenid);
								//	playerinfo->setRushCount(0);
								//	playerinfo->setState(FubenRushType_Rushing);
								//	vitr = itr->second.erase(vitr);
								//	(*rushplayermap)[0].push_back(playerinfo);
								//}
								//else
								//{
								//	(*vitr)->setState(FubenRushType_Rushing);
								//	(*vitr)->setRushCount(0);
								//}
								break;
							}
						}
						if (iffindme)
						{
							break;
						}
					}
					gsBrainData->excHandle(MAKEINT64(WCH_LockData,0));
				}
				if (!iffindme)
				{
					success = -3;
				}
				int needviplv = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenRushQuickEndOpenViplv,viplv).c_str());
				if (needviplv > viplv)
				{
					success = -4;//VIP等级不足
				}
				if (success == 0)
				{
					data->excHandle(MAKEINT64(WCH_LockData,1));
					if (fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record) >= 0)
					{
						needjunling = atoi(record[junlingindex].c_str());
					}
					data->getParam(WCHDATA_JXJJunling,param);
					unsigned short *junling = (unsigned short *)param;
					//扣除军令
					*junling -= needjunling * remainrushcount * 100;
					nowjunlingcount = *junling;
					ref_ptr<crStreamBuf> stream2 = new crStreamBuf;
					stream2->createBuf(3);
					stream2->_writeUChar(0);
					stream2->_writeUShort(nowjunlingcount);
					crPlayerDataEventPacket packet2;
					crPlayerDataEventPacket::buildReplyPacket(packet2, playerid, WCH_JXJRecvFubenRushInfo, stream2.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet2);
					//扣除元宝
					MoneyChangeData moneydata(needgold,"立即完成扫荡");
					m_this->doEvent(WCH_JXJDeductLijinGold,MAKEINT64(&moneydata,&needdeductgold));
					//发放奖励
					for (int i = 0; i < remainrushcount; ++i)
					{
						needgoldpertemp = needgoldpersecond;
						m_this->doEvent(WCH_JXJFubenRushReward,MAKEINT64(&fubenid,NULL));	
					}
					data->inputParam(WCHDATA_JXJFubenOrChengchiType,NULL);
					data->inputParam(WCHDATA_JXJFubenOrChengchiType,NULL);
					data->excHandle(MAKEINT64(WCH_LockData, 0));
					//增加活跃度/////////////////////////////////
					m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKEINT64(HYD_FubenRush, remainrushcount));
					///////////////////////////////////////////////
				}
				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeChar(success);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvFubenRushQuickEnd, stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				}
				if (needdeductgold)
				{
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerData.get());
				}
			} 
		}
		else if(m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			if (success < 0)
			{
				int index = 0;
				switch(success)
				{
				case -1:
					index = 2008;//背包不足
					break;
				case -2:
					index = 2036;//元宝不足
					break;
				case -4:
					index = 2101;//VIP等级不足
					break;
				}
				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
			}
			//else
			//{
			//	int junlingcount = m_stream->_readUChar();
			//	if(m_this && m_this->getDataClass())
			//	{
			//		m_this->getDataClass()->inputParam(WCHDATA_JXJJunling,&junlingcount);
			//	}
			//}
		}
	}
}
/////////////////////////////////////////
//
//crJXJQueryLoseCountryMethod
//
/////////////////////////////////////////
crJXJQueryLoseCountryMethod::crJXJQueryLoseCountryMethod():
	m_this(NULL)
{
}
crJXJQueryLoseCountryMethod::crJXJQueryLoseCountryMethod(const crJXJQueryLoseCountryMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJQueryLoseCountryMethod::inputParam(int i, void *param)
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
void crJXJQueryLoseCountryMethod::addParam(int i, const std::string& str)
{
}
void crJXJQueryLoseCountryMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(myPlayer && netConductor)
	{
		crJXJQueryLoseCountryPacket packet;
		crJXJQueryLoseCountryPacket::buildRequestPacket(packet);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
///////////////////////////////////////////
////
////crJXJRecvQueryLoseCountryMethod
////
///////////////////////////////////////////
//crJXJRecvQueryLoseCountryMethod::crJXJRecvQueryLoseCountryMethod():
//	m_netType(GameClient_Game){}
//crJXJRecvQueryLoseCountryMethod::crJXJRecvQueryLoseCountryMethod(const crJXJRecvQueryLoseCountryMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJRecvQueryLoseCountryMethod::inputParam(int i, void *param)
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
//void crJXJRecvQueryLoseCountryMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crJXJRecvQueryLoseCountryMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_stream.valid())
//	{
//		if(m_netType == GameServer)
//		{
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
//			unsigned char shiliid = 0;
//			unsigned char loseShiliid = 0;
//			bool isLoseCountry = false;
//			for (int i = 0; i < 3; ++i)
//			{
//				shiliid = c_startShiliID + i;
//				crServerBrainHandle::getInstance()->doEvent(WCH_JXJCheckCountryLose,MAKEINT64(&isLoseCountry,&shiliid));
//				if (isLoseCountry)
//				{
//					loseShiliid = shiliid;
//				}
//			}
//			ref_ptr<crStreamBuf> stream = new crStreamBuf;
//			stream->createBuf(1);
//			stream->_writeUChar(loseShiliid);
//			crPlayerDataEventPacket packet;
//			crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvQueryLoseCountry,stream.get());
//			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			char success = m_stream->_readChar();
//			if (success < 0)
//			{
//				int index = 0;
//				switch(success)
//				{
//				case -1:
//					index = 2008;//背包不足
//					break;
//				case -2:
//					index = 2036;//元宝不足
//					break;
//				}
//				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKEINT64(index,NULL));
//			}
//		}
//	}
//}
/////////////////////////////////////////
//
//crJXJSearchMyCountryCityCompleteCheckMethod
//成就关于国战城池归属
/////////////////////////////////////////
crJXJSearchMyCountryCityCompleteCheckMethod::crJXJSearchMyCountryCityCompleteCheckMethod():
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchMyCountryCityCompleteCheckMethod::crJXJSearchMyCountryCityCompleteCheckMethod(const crJXJSearchMyCountryCityCompleteCheckMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJSearchMyCountryCityCompleteCheckMethod::inputParam(int i, void *param)
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
void crJXJSearchMyCountryCityCompleteCheckMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchMyCountryCityCompleteCheckMethod::operator()(crHandle &handle)
{
	if(m_player)
	{
		crData *data = m_player->getDataClass();
		ref_ptr<crData> serverdata = crServerBrainHandle::getInstance()->getDataClass();
		void *param;
		int passcount = 0;
		
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshiliid = *(unsigned char *)param;
		serverdata->excHandle(MAKEINT64(WCH_LockData,1));
		serverdata->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;
		for( ChengchiMap::iterator itr = chengchimap->begin();
			itr != chengchimap->end();
			++itr )
		{
			if(itr->second->getChengzhuShili() == myshiliid)
			{
				passcount++;
				m_this->setStep(passcount);
				if(m_this->isComplete())
				{
					break;
				}
			}
		}
		serverdata->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJSearchMyCountryCityMethod
//成就关于国战城池归属
/////////////////////////////////////////
crJXJSearchMyCountryCityMethod::crJXJSearchMyCountryCityMethod():
	m_this(NULL)
{
}
crJXJSearchMyCountryCityMethod::crJXJSearchMyCountryCityMethod(const crJXJSearchMyCountryCityMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchMyCountryCityMethod::inputParam(int i, void *param)
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
void crJXJSearchMyCountryCityMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchMyCountryCityMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && !m_this->getComplete())
	{
		crData *data = myPlayer->getDataClass();
		void *param;
		int passcount = 0;
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJShiliID,param);
		unsigned char myshiliid = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;
		for( ChengchiMap::iterator itr = chengchimap->begin();
			itr != chengchimap->end();
			++itr )
		{
			if(itr->second->getChengzhuShili() == myshiliid)
			{
				passcount++;
				m_this->setStep(passcount);
				if(m_this->isComplete())
				{
					break;
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJLoadingImageMethod
//
/////////////////////////////////////////
crJXJLoadingImageMethod::crJXJLoadingImageMethod():
	m_updateInterval(2.0f),
	m_dt(0.0f),
	m_timer(0.0f),
	m_currentid(-1){}
crJXJLoadingImageMethod::crJXJLoadingImageMethod(const crJXJLoadingImageMethod& handle):
	crMethod(handle),
	m_ImgTab(handle.m_ImgTab),
	m_updateInterval(handle.m_updateInterval),
	m_dt(0.0f),
	m_timer(0.0f),
	m_currentid(-1)
{
}
void crJXJLoadingImageMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crImageBoxElement*)param;
	else if(i == -3)
	{
		m_dt = *(float *)param;
	}
}
void crJXJLoadingImageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_updateInterval = atof(str.c_str());
		break;
	case 1:
		m_ImgTab = crTableIO::openFile(str);
		break;
	}
}
void crJXJLoadingImageMethod::operator()(crHandle &handle)
{
	if(m_element && m_ImgTab.valid())
	{
		if(m_currentid == -1)
		{
			int maxid = m_ImgTab->getRowCount();
			m_currentid = rangei(0,maxid).get_random();
			m_timer = m_updateInterval;
		}
		m_timer+=m_dt;
		if(m_timer>m_updateInterval)
		{
			std::string file = "media/loading/";
			file += m_ImgTab->getData(m_currentid,1);
			m_element->setImageFile(file);
			m_timer = 0.0f;
			m_currentid++;
			if(m_currentid>=m_ImgTab->getRowCount())
				m_currentid = 0;
		}
	}
}
/////////////////////////////////////////
//
//crJXJLoadingImage1Method
//
/////////////////////////////////////////
crJXJLoadingImage1Method::crJXJLoadingImage1Method():
	m_updateInterval(2.0f),
	m_dt(NULL),
	m_timer(0.0f){}
crJXJLoadingImage1Method::crJXJLoadingImage1Method(const crJXJLoadingImage1Method& handle):
	crMethod(handle),
	m_imgWidget(handle.m_imgWidget),
	m_ImgTab(handle.m_ImgTab),
	m_updateInterval(handle.m_updateInterval),
	m_dt(NULL),
	m_timer(0.0f)
{
}
void crJXJLoadingImage1Method::inputParam(int i, void *param)
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
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJLoadingImage1Method::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_imgWidget = str;
		break;
	case 1:
		m_updateInterval = atof(str.c_str());
		break;
	case 2:
		m_ImgTab = crTableIO::openFile(str);
		break;
	}
}
void crJXJLoadingImage1Method::operator()(crHandle &handle)
{
	static int s_currentid = -1;
	crImageBoxWidgetNode *img = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imgWidget)); 
	if(img && m_ImgTab.valid())
	{
		if(s_currentid == -1)
		{
			int maxid = m_ImgTab->getRowCount();
			s_currentid = rangei(0,maxid).get_random();
			//m_timer = m_updateInterval;
		}
		m_timer-=*m_dt;
		if(m_timer<=0.0f)
		{
			std::string file = m_ImgTab->getData(s_currentid,1);
			img->setImageName(file);
			m_timer = m_updateInterval;
			s_currentid++;
			if(s_currentid>=m_ImgTab->getRowCount())
				s_currentid = 0;
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetLoadingTextMethod
//
/////////////////////////////////////////
crJXJSetLoadingTextMethod::crJXJSetLoadingTextMethod(){}
crJXJSetLoadingTextMethod::crJXJSetLoadingTextMethod(const crJXJSetLoadingTextMethod& handle):
	crMethod(handle)
{
}
void crJXJSetLoadingTextMethod::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crStaticTextBoxElement*)param;
}
void crJXJSetLoadingTextMethod::addParam(int i, const std::string& str)
{
}
void crJXJSetLoadingTextMethod::operator()(crHandle &handle)
{
	if(m_element && crDisplaySettings::instance()->getRunMode()>crDisplaySettings::Editor)
	{
		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		if(noticeHandle)
		{
			int texid = 1081;
			noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
			int mode = 2;
			noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
			std::string output;
			noticeHandle->inputParam(WCHDATA_NoticeOutPut,&output);
			crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			m_element->setStringArrayByString(output);
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIJiusiAutoRollCardMethod
//
/////////////////////////////////////////
crJXJUIJiusiAutoRollCardMethod::crJXJUIJiusiAutoRollCardMethod()
{
}
crJXJUIJiusiAutoRollCardMethod::crJXJUIJiusiAutoRollCardMethod(const crJXJUIJiusiAutoRollCardMethod& handle):
	crMethod(handle)
{
}
void crJXJUIJiusiAutoRollCardMethod::inputParam(int i, void *param)
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
void crJXJUIJiusiAutoRollCardMethod::addParam(int i, const std::string& str)
{
}
void crJXJUIJiusiAutoRollCardMethod::operator()(crHandle &handle)
{
	void*param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *braindata = crBrain::getInstance()->getDataClass();
	if(braindata && playerData)
	{
		braindata->getParam(WCHDATA_JXJRollCardRemainCount,param);
		unsigned char *remaincount = (unsigned char *)param;
		if (*remaincount > 0)
		{
			*remaincount -= 1;
		}
		if (*remaincount > 0)
		{
			CRNet::crNetConductor *net_conductor = CRNet::crNetContainer::getInstance()->getDynamicNetConductor(CRNetApp::GameClient_Game);
			CRNetApp::crPlayerDataEventPacket packet;
			ref_ptr<crStreamBuf> stream = new CRCore::crStreamBuf;
			stream->createBuf(1);
			stream->_writeUChar(RefreshCardType_GoldBatch);
			CRNetApp::crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvRollCards,stream.get());
			net_conductor->getNetManager()->sendPacket("all",packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIAutoUseBagItemMethod
//
/////////////////////////////////////////
crJXJUIAutoUseBagItemMethod::crJXJUIAutoUseBagItemMethod()
{
}
crJXJUIAutoUseBagItemMethod::crJXJUIAutoUseBagItemMethod(const crJXJUIAutoUseBagItemMethod& handle):
	crMethod(handle)
{
}
void crJXJUIAutoUseBagItemMethod::inputParam(int i, void *param)
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
void crJXJUIAutoUseBagItemMethod::addParam(int i, const std::string& str)
{
}
void crJXJUIAutoUseBagItemMethod::operator()(crHandle &handle)
{
	void*param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *braindata = crBrain::getInstance()->getDataClass();
	if(braindata && playerData)
	{
		int itemid = 0;
		crData *data = playerData->getDataClass();
		braindata->getParam(WCHDATA_JXJUseItemRemainCount,param);
		unsigned char remaincount = *(unsigned char *)param;
		if(remaincount > 0)
		{
			braindata->getParam(WCHDATA_JXJSaleItemID,param);
			unsigned char selectid = *(unsigned char *)param;

			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
			if (selectid < itemBagVec->size())
			{ 
				if((*itemBagVec)[selectid].valid())
				{
					itemid = (*itemBagVec)[selectid]->getItemID();
					remaincount = (*itemBagVec)[selectid]->getItemCount();
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			if (itemid > 0 && remaincount > 0)
			{
				playerData->doEvent(MAKEINT64(WCH_JXJUseBagItem,NULL),MAKEINT64(itemid,selectid));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIAchievementPromptUIShowMethod
//成就弹出小框[UI]
//
/////////////////////////////////////////
crJXJUIAchievementPromptUIShowMethod::crJXJUIAchievementPromptUIShowMethod()
{
}
crJXJUIAchievementPromptUIShowMethod::crJXJUIAchievementPromptUIShowMethod(const crJXJUIAchievementPromptUIShowMethod& handle):
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_icon(handle.m_icon),
	m_input1(handle.m_input1),
	m_input2(handle.m_input2),
	m_sequence(handle.m_sequence),
	m_backboard(handle.m_backboard),
	m_sw(handle.m_sw)
{
}
void crJXJUIAchievementPromptUIShowMethod::inputParam(int i, void *param)
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
void crJXJUIAchievementPromptUIShowMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_icon = str;
		break;
	case 2:
		m_input1 = str;
		break;
	case 3:
		m_input2 = str;
		break;
	case 4:
		m_sequence = str;
		break;
	case 5:
		m_backboard = str;
		break;
	case 6:
		m_sw = str;
		break;
	}
}

void crJXJUIAchievementPromptUIShowMethod::operator()(crHandle &handle)
{
	if (m_this)
	{
		unsigned short accid = 0;
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crImageBoxWidgetNode> icon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getChildNode(m_icon));
		ref_ptr<crStaticTextWidgetNode> input1 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getChildNode(m_input1));
		ref_ptr<crStaticTextWidgetNode> input2 = dynamic_cast<crStaticTextWidgetNode *>(m_this->getChildNode(m_input2));
		ref_ptr<crSequence> sequence = dynamic_cast<crSequence *>(m_this->getChildNode(m_sequence));
		ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_sw));
		ref_ptr<crNode> backboard = m_this->getChildNode(m_backboard);
		if(playerData)
		{
			if(sequence.valid() && sequence->getMode() == crSequence::STOP)
			{
				//if(icon.valid())icon->setVisiable(true);
				//if(input1.valid())input1->setVisiable(true);
				//if(input2.valid())input2->setVisiable(true);
				//if(backboard.valid())backboard->setVisiable(true);
				if(sw.valid())sw->setActiveSwitchSet(1);
			}
			else
			{
				//if(icon.valid())icon->setVisiable(false);
				//if(input1.valid())input1->setVisiable(false);
				//if(input2.valid())input2->setVisiable(false);
				//if(backboard.valid())backboard->setVisiable(false);
				if(sw.valid())sw->setActiveSwitchSet(0);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIRotateClockMethod
//
/////////////////////////////////////////
crJXJUIRotateClockMethod::crJXJUIRotateClockMethod():
	m_unrealsec(0),
	m_lastangle(0.0f)
{
}

crJXJUIRotateClockMethod::crJXJUIRotateClockMethod(const crJXJUIRotateClockMethod& handle):
	crMethod(handle),
	m_rotatestr(handle.m_rotatestr),
	m_unrealsec(handle.m_unrealsec),
	m_lastangle(handle.m_lastangle),
	m_canvas(handle.m_canvas),
	m_timeinput(handle.m_timeinput)
{
}
void crJXJUIRotateClockMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	//case 0:
	//	if(param == 0)
	//	{//释放
	//		m_this = NULL;
	//	}
	//	break;
	//case 1:
	//	m_this = (crCanvasNode*)param;
		//	break;
	case 2:
		if(param)
		{
			m_unrealsec = *(int*)param;
		}
		break;
	}
}
void crJXJUIRotateClockMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rotatestr = str;
		break;
	case 1:
		m_canvas = str;
		break;
	case 2:
		m_timeinput = str;
		break;
	default:
		break;
	}
}

void crJXJUIRotateClockMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = dynamic_cast<crCanvasNode* >(crFilterRenderManager::getInstance()->findCanvas(m_canvas));
	if (canvas.valid())
	{
		ref_ptr<crMatrixTransform> matrixTransform = dynamic_cast<crMatrixTransform *>(canvas->getChildNode(m_rotatestr));
		//ref_ptr<crMultiSwitch> sw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_sw));
		ref_ptr<crStaticTextWidgetNode> timeText = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_timeinput));
		if (matrixTransform.valid())
		{
			const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
			const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
			float angle = ((float)m_unrealsec) / 86400.0f * 360.0f * M_PI / 180;
			//m_lastangle
			crMatrix rollMatrix = CRCore::crMatrix::rotate(-angle + m_lastangle, Z_AXIS);			
			crMatrix mat = matrixTransform->getMatrix();
			crVector3 trans = matrixTransform->getBound().center();//matrixTransform->getBound().center();
			crMatrix put_mat = crMatrix::translate(trans);
			crMatrix inv_put = crMatrix::inverse(put_mat);
			mat.postMult(inv_put);
			mat.postMult(invScaleMat * rollMatrix * scaleMat);
			mat.postMult(put_mat);
			matrixTransform->setMatrix(mat);
			m_lastangle = angle;
		}
		if(timeText.valid())
		{
			//显示时间
			int tianshu = 0;
			if (crGlobalHandle::isClient())
			{
				crGlobalHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			}
			else
			{
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJGetStartedDays, MAKEINT64(&tianshu, NULL));
			}
			int worldTime = 184 + tianshu;
			std::string str;
			std::string season;
			time_t nowTime_t = time(0);
			struct  tm  *nowTm = localtime(&nowTime_t); //服务器当前时间
			int nowsec = nowTm->tm_hour * 3600 + nowTm->tm_min * 60 + nowTm->tm_sec;
			if (0 <= nowsec && nowsec < 21600)
			{
				season = "春";
			}
			else if (21600 <= nowsec && nowsec < 43200)
			{
				season = "夏";
			}
			else if (43200 <= nowsec && nowsec < 64800)
			{
				season = "秋";
			}
			else
			{
				season = "冬";
			}
			str = "公元" + crArgumentParser::appItoa(worldTime) + "年" + season;
			timeText->setString(str);
			timeText->setColor(FCOLOR_YELLOW);

			//int timeiconid = m_unrealsec / 86400.0f * 12;
			//if (timeiconid > 11)
			//{
			//	timeiconid = 11;
			//}
			//if (sw.valid())
			//{
			//	sw->setActiveSwitchSet(timeiconid);
			//}
		}
	}
}
/////////////////////////////////////////
//
//crJXJAutoBattleSettingMethod
//
/////////////////////////////////////////
crJXJAutoBattleSettingMethod::crJXJAutoBattleSettingMethod()
{
}
crJXJAutoBattleSettingMethod::crJXJAutoBattleSettingMethod(const crJXJAutoBattleSettingMethod& handle):
	crMethod(handle)
{
}
void crJXJAutoBattleSettingMethod::inputParam(int i, void *param)
{
}

void crJXJAutoBattleSettingMethod::addParam(int i, const std::string& str)
{
}
void crJXJAutoBattleSettingMethod::operator()(crHandle &handle)
{
	void *param;
	crData *braindata = crBrain::getInstance()->getDataClass();
	braindata->getParam(WCHDATA_JXJAutoBattleSetting,param);
	int setting = *(int *)param;
	if (setting!=0)
	{
		crDisplaySettings *ds = crDisplaySettings::instance();
		ds->setShadowDetail(0);
		ds->setEnableAlphaShadow(false);
		ds->setHDR(0);
		ds->setNumMultiSamples(0);
		ds->setMaxAnisotropy(8);
	}
}
