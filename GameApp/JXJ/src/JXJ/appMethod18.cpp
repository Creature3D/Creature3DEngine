/**********************************************************************
*
*	文件:	   appMethod.cpp
*
*	描述:
*
*	作者:	   吴财华
*
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMethod16.h>
#include <JXJ/appMethod18.h>
#include <JXJ/appDataParam.h>
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
#include <CREncapsulation/crGameDices.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRIOManager/crConvertUTF.h>
#include <CRIOManager/crLoadManager.h>
#include <CRCore/crBlockDetectThread.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRDownload/crDownload.h>
#include <CRNetApp/appDataLockManager.h>
#include <sstream>
#include <shellapi.h>
using namespace CRCore;
using namespace CRNetApp;
using namespace CRDataBase;
using namespace CRNet;
using namespace CRProducer;
using namespace CRPhysics;
using namespace CREncapsulation;
using namespace CRUtil;
using namespace CRIOManager;
using namespace rbody;
using namespace JXJ;
using namespace CRUI;
using namespace CRDownload;
/////////////////////////////////////////
//
//crJXJLoginCrossGameMethod
//
/////////////////////////////////////////
crJXJLoginCrossGameMethod::crJXJLoginCrossGameMethod() {}
crJXJLoginCrossGameMethod::crJXJLoginCrossGameMethod(const crJXJLoginCrossGameMethod& handle) :
	crMethod(handle),
	m_callback(handle.m_callback),
	m_servername(handle.m_servername),
	m_password(handle.m_password)
{
}
void crJXJLoginCrossGameMethod::inputParam(int i, void *param)
{
}

void crJXJLoginCrossGameMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_callback = str;
		break;
	case 1:
		m_servername = str;
		break;
	case 2:
		m_password = str;
		break;
	}
}
void crJXJLoginCrossGameMethod::operator()(crHandle &handle)
{
	bool sucess = false;
	crQueryConnectServerData *connectServerData = crMyPlayerData::getInstance()->getConnectServerData();
	if (connectServerData)
	{
		CRCore::ref_ptr<CRNet::crNetConductor> netConductor = new CRNet::crNetConductor;
		netConductor->init(connectServerData->getPort());
		netConductor->setNetType(CRNetApp::GameClient_CrossGame);
		netConductor->setName("GameClient_CrossGame");
		netConductor->setEnableCook(true);
		int playerid = crMyPlayerData::getInstance()->getPlayerID();
		CRCore::notify(CRCore::ALWAYS) << "连接到游戏服务器:" << connectServerData->getIP() << ":" << connectServerData->getPort() << std::endl;
		crBlockDetectThread::getInstance()->pause();
		int code = 0;
		if (crDisplaySettings::instance()->getRunMode() >= crDisplaySettings::Robot)
			code = netConductor->getNetManager()->clientReconnectToServerCircle(connectServerData->getIP(), m_callback, playerid, m_servername, m_password, connectServerData->getPort(), 0);
		else
			code = netConductor->getNetManager()->clientReconnectToServerCircle(connectServerData->getIP(), m_callback, playerid, m_servername, m_password, connectServerData->getPort(), 0, 10);
		//int code = netConductor->getNetManager()->clientReconnectToServerCircle(connectServerData->getIP(),m_callback,playerid,m_servername,m_password,connectServerData->getPort(),0,5);
		crBlockDetectThread::getInstance()->resume();
		if (code <= 0)
		{
			switch (code)
			{
			case 0:
				//无法创建客户端
				CRCore::notify(CRCore::FATAL) << "无法连接到游戏服务器，请检查服务器IP地址，端口或者WaiterConnectServer是否启动" << std::endl;
				break;
			case -1:
				//超出服务器连接人数限制
				CRCore::notify(CRCore::FATAL) << "超出服务器连接人数限制" << std::endl;
				break;
			case -2:
				//同名客户端已经存在
				CRCore::notify(CRCore::FATAL) << "同名客户端已经存在" << std::endl;
				break;
			case -3:
				//要求连接的服务端名字与本服务器名不同
				CRCore::notify(CRCore::FATAL) << "要求连接的服务端名字与本服务器名不同" << std::endl;
				break;
			case -4:
				//服务器密码不正确
				CRCore::notify(CRCore::FATAL) << "服务器密码不正确" << std::endl;
				break;
			default:
				CRCore::notify(CRCore::FATAL) << "无法连接到游戏服务器" << std::endl;
			}
		}
		else
		{
			//CRNet::crNetContainer::getInstance()->pushDynamicNetConductor(netConductor.get());
			////CRCore::notify(CRCore::ALWAYS)<<"已经与游戏服务器建立连接"<<std::endl;
			//if (crMyPlayerData::getInstance()->getSelectedGame())
			//{
			//	//crJXJLoginCrossGamePacket packet;
			//	crJXJLoginGamePacket::buildRequestPacket(packet);
			//	netConductor->getNetManager()->sendPacket("all", packet);

			//	sucess = true;
			//}
		}
	}
	handle.outputParam(0, &sucess);
}
/////////////////////////////////////////
//
//crJXJUISuperEquipCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJUISuperEquipCanvasUpdateMethod::crJXJUISuperEquipCanvasUpdateMethod()
{
}
crJXJUISuperEquipCanvasUpdateMethod::crJXJUISuperEquipCanvasUpdateMethod(const crJXJUISuperEquipCanvasUpdateMethod& handle) :
	crMethod(handle)
{
}
void crJXJUISuperEquipCanvasUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJUISuperEquipCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_equipRadioGroup = str;
		break;
	case 1:
		m_equipPageRadioGroup = str;
		break;
	case 2:
		m_equipPageSW = str;
		break;
	case 3:
		m_equipImage = str;
		break;
	case 4:
		m_equipLevelText = str;
		break;
	case 5:
		m_equipQualityText = str;
		break;
	case 6:
		m_equipLvupInfoText = str;
		break;
	case 7:
		m_equipAttrInfoText = str;
		break;
	case 8:
		m_expProgress = str;
		break;
	case 9:
		m_expText = str;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		m_jiangkaImage[i - 10] = str;
		break;
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_jiangkaText[i - 15] = str;
		break;
	case 20:
		m_activeButton = str;
		break;
	case 21:
		m_scroll = str;
		break;
	case 22:
		m_lunhuiDistillButton = str;
		break;
	case 23:
		m_lunhuiEquipParamText = str;
		break;
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
		m_lunhuiEquipExpText[i-24] = str;
		break;
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
		m_equipZhulinglvText[i - 32] = str;
		break;
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		m_equipZhulinglvAddButton[i - 42] = str;
		break;
	case 52:
		m_equipZhulingPointText = str;
		break;
	case 53:
		m_equipZhulingResetButton = str;
		break;
	case 54:
		m_expBuyButton = str;
		break;
	case 55:
		m_activeSW = str;
		break;
	}
}
void crJXJUISuperEquipCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crMultiSwitch> activeSW = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_activeSW));
	ref_ptr<crRadioGroupWidgetNode> equipRadioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_equipRadioGroup));
	ref_ptr<crRadioGroupWidgetNode> equipPageRadioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_equipPageRadioGroup));
	ref_ptr<crMultiSwitch> equipPageSW = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_equipPageSW));
	ref_ptr<crImageBoxWidgetNode> equipImage = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_equipImage));
	ref_ptr<crStaticTextWidgetNode> equipLevelText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipLevelText));
	ref_ptr<crStaticTextWidgetNode> equipQualityText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipQualityText));
	ref_ptr<crStaticTextWidgetNode> equipLvupInfoText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipLvupInfoText));
	ref_ptr<crStaticTextWidgetNode> equipAttrInfoText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipAttrInfoText));
	ref_ptr<crProgressWidgetNode> expProgress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_expProgress));
	ref_ptr<crStaticTextWidgetNode> expText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_expText));
	ref_ptr<crButtonWidgetNode> expBuyButton = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_expBuyButton));
	//将卡
	ref_ptr<crImageBoxWidgetNode> jiangkaImage[5];
	ref_ptr<crStaticTextWidgetNode> jiangkaText[5];
	for (int i = 0; i<5; i++)
	{
		jiangkaImage[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_jiangkaImage[i]));
		jiangkaText[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_jiangkaText[i]));
	}
	ref_ptr<crButtonWidgetNode> activeButton = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_activeButton));
	ref_ptr<crScrollBarWidgetNode> scrollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));
	//轮回
	ref_ptr<crButtonWidgetNode> lunhuiDistillButton = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lunhuiDistillButton));
	ref_ptr<crStaticTextWidgetNode> lunhuiEquipParamText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_lunhuiEquipParamText));
	ref_ptr<crStaticTextWidgetNode> lunhuiEquipExpText[SuperEquipCount];
	for (int i=0; i<SuperEquipCount; i++)
	{
		lunhuiEquipExpText[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_lunhuiEquipExpText[i]));
	}

	int selectEquip = 0;
	if (equipRadioGroup.valid())
		selectEquip = equipRadioGroup->getSelect();
	int selectPage = 0;
	if (equipPageRadioGroup.valid())
		selectPage = equipPageRadioGroup->getSelect();
	if(equipPageSW.valid())
		equipPageSW->setActiveSwitchSet(selectPage == 3 ? 1 : 0);
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getMainRole())
	{
		char buf[128];
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		void *param;
		roledata->lock();
		roledata->getParam(WCHDATA_JXJSuperEquipDataVec, param);
		SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
		roledata->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
		unsigned char lvuppoint = *(unsigned char *)param;
		bool actived = selectEquip < superEquipDataVec->size();
		if (activeSW.valid())
			activeSW->setActiveSwitchSet(actived ? 1 : 0);
		if (actived)
		{
			ref_ptr<SuperEquipData> superEquipData = (*superEquipDataVec)[selectEquip];
			int superEquipID = superEquipData->getSuperEquipID();
			unsigned char lv = superEquipData->getSuperEquipLevel();
			//if (actived)
			{
				int quality = (lv - 1) / 5;
				if (selectPage == 1)
				{
					quality++;
					lv = quality * 5 + 1;
				}
				else if (selectPage == 2)
				{
					lv = SuperEquipMaxLevel;
					quality = (SuperEquipMaxLevel - 1) / 5;
				}
				if (selectPage != 3)
				{//切页0
					ref_ptr<crTableIO> superEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipTab);
					ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
					int lvindex = superEquipTab->getTitleIndex("等级");
					int expindex = superEquipTab->getTitleIndex("经验");
					int imageindex = superEquipTab->getTitleIndex("图片");
					int bingfaindex = superEquipTab->getTitleIndex("兵法");
					int zhengfaindex = superEquipTab->getTitleIndex("阵法");
					int dongchaindex = superEquipTab->getTitleIndex("洞察");
					int abstractindex = superEquipTab->getTitleIndex("abstractid");
					int qualityindex = qualitytab->getTitleIndex("color");
					int qualitynameindex = qualitytab->getTitleIndex("name");
					crTableIO::StrVec record, nextrecord;
					crTableIO::StrVec colorrecord;
					crVector4f colormode;
					if (superEquipTab->queryOneRecord(0, crArgumentParser::appItoa(superEquipID), lvindex, crArgumentParser::appItoa(lv), record)>=0)
					{
						//CRCore::notify(CRCore::ALWAYS) << "equipImage: " << equipImage.get()<<"控件名:"<< m_equipImage<<"图片名:"<< record[imageindex] << std::endl;
						if (equipImage.valid())
						{
							equipImage->setImageName(record[imageindex]);
						}
						if (equipLevelText.valid())
						{
							equipLevelText->setString(crArgumentParser::appItoa(lv));
						}
						if (equipQualityText.valid())
						{
							if (qualitytab->queryOneRecord(0, crArgumentParser::appItoa(quality), colorrecord) >= 0)
							{
								crArgumentParser::appAtoVec(colorrecord[qualityindex], colormode);
								colormode /= 255.0f;
								equipQualityText->setColor(colormode);
								equipQualityText->setString(colorrecord[qualitynameindex]);
							}
						}
						if (equipLvupInfoText.valid())
						{
							sprintf_s(buf, "额外获得%d点注灵点\0", lvuppoint);
							equipLvupInfoText->setString(std::string(buf));
						}
						if (equipAttrInfoText.valid())
						{
							sprintf_s(buf, "兵法:%s    阵法:%s    洞察:%s\0", record[bingfaindex].c_str(), record[zhengfaindex].c_str(), record[dongchaindex].c_str());
							equipAttrInfoText->setString(std::string(buf));
						}
						if (expProgress.valid() && expText.valid() && expBuyButton.valid())
						{
							if (selectPage == 0)
							{
								if (lv == SuperEquipMaxLevel)
								{
									expProgress->setProgress(1.0f);
									expText->setVisiable(false);
									expBuyButton->setEnable(false);
								}
								else if (superEquipTab->queryOneRecord(0, crArgumentParser::appItoa(superEquipID), lvindex, crArgumentParser::appItoa(lv + 1), nextrecord) >= 0)
								{
									int curexp = superEquipData->getSuperEquipExp();
									int curlvexp = atoi(record[expindex].c_str());
									int nextexp = atoi(nextrecord[expindex].c_str());
									expProgress->setProgress((float)(curexp - curlvexp) / (float)(nextexp - curlvexp));
									sprintf_s(buf, "%d/%d\0", curexp, nextexp);
									expText->setString(std::string(buf));
									expText->setVisiable(true);
									expBuyButton->setEnable(true);
								}
							}
							else
							{
								expProgress->setProgress(1.0f);
								expText->setVisiable(false);
								expBuyButton->setEnable(false);
							}
						}
						
						////将卡组合信息
						ref_ptr<crTableIO> cardSuperExtraTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCardSuperExtraTab);
						int desindex = cardSuperExtraTab->getTitleIndex("描述");
						ref_ptr<crTableIO> itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
						int jkiconindex = itemTab->getTitleIndex("icon");
						int nameindex = itemTab->getTitleIndex("name");
						crTableIO::StrVec itemrecord;
						std::string abstractstr = "|"+record[abstractindex]+"|";
						crTableIO::DataVec &dataVec = cardSuperExtraTab->getDataVec();
						std::map<std::string,crVector2i>CardExtraIDMap;//cardname,cardid,rowindex
						int rowcount = cardSuperExtraTab->getRowCount();
						std::vector<int> cardidvec;
						for (int i = 0; i<rowcount;i++)
						{
							if ((*cardSuperExtraTab)(i,1).find(abstractstr) != std::string::npos)
							{
								crArgumentParser::appAtoVec((*cardSuperExtraTab)(i, 0), cardidvec, '|');
								for (std::vector<int>::iterator itr = cardidvec.begin();
									itr != cardidvec.end();
									++itr)
								{
									//CRCore::notify(CRCore::ALWAYS) << "i=" << i << " cardid=" << *itr << std::endl;
									if ((*itr)>0 && itemTab->queryOneRecord(0, crArgumentParser::appItoa(*itr), itemrecord) >= 0)
										CardExtraIDMap.insert(std::make_pair(itemrecord[nameindex],crVector2i(*itr, i)));
								}
							}
						}
						int count = CardExtraIDMap.size();
						int scroll = 0;
						if (count > 5)
						{
							scrollbar->setRange(0, count - 5);
							scrollbar->setLineValue(1);
							scrollbar->setPageValue(5);
							scrollbar->setEnable(true);
							scrollbar->setVisiable(true);
							scroll = scrollbar->getValue();
						}
						else
						{
							scrollbar->setVisiable(false);
							scrollbar->setEnable(false);
						}
						//CRCore::notify(CRCore::ALWAYS) << "scroll=" << scroll << " count=" << count << std::endl;
						std::map<std::string, crVector2i>::iterator citr = CardExtraIDMap.begin();
						while (scroll>0)
						{
							if(citr == CardExtraIDMap.end())
								break;
							scroll--;
							citr++;
						}
						for (int i = 0; i < 5; i++)
						{
							if (citr != CardExtraIDMap.end())
							{
								jiangkaText[i]->setString((*cardSuperExtraTab)(citr->second[1], desindex));
								if (itemTab->queryOneRecord(0, crArgumentParser::appItoa(citr->second[0]), itemrecord) >= 0)
								{
									jiangkaImage[i]->setImageName(itemrecord[jkiconindex]);
								}
								jiangkaImage[i]->setVisiable(true);
								jiangkaText[i]->setVisiable(true);
								citr++;
							}
							else
							{
								jiangkaImage[i]->setVisiable(false);
								jiangkaText[i]->setVisiable(false);
							}
						}
					}
				}
				//神器注灵页面
				ref_ptr<crStaticTextWidgetNode> equipZhulinglvText[SuperEquipZhulingCount];
				ref_ptr<crButtonWidgetNode> equipZhulinglvAddButton[SuperEquipZhulingCount];
				for (int i = 0; i < SuperEquipZhulingCount; i++)
				{
					equipZhulinglvText[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipZhulinglvText[i]));
					equipZhulinglvAddButton[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_equipZhulinglvAddButton[i]));
				}
				ref_ptr<crStaticTextWidgetNode> equipZhulingPointText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_equipZhulingPointText));
				ref_ptr<crButtonWidgetNode> equipZhulingResetButton = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_equipZhulingResetButton));

				roledata->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				if (superEquipZhulingData)
				{
					ref_ptr<crTableIO> superEquipAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipAttrTab);
					int needPointIndex = superEquipAttrTab->getTitleIndex("消耗点数");
					int startindex = superEquipAttrTab->getTitleIndex("生命值");
					crTableIO::StrVec zhulingRecord;
					SuperEquipZhulinglvVec &superEquipZhulinglvVec = superEquipZhulingData->getZhulinglvVec();
					int zhulinglv = 0;
					for (int i = 0; i < SuperEquipZhulingCount; i++)
					{
						zhulinglv += superEquipZhulinglvVec[i];
						if (superEquipAttrTab->queryOneRecord(0, crArgumentParser::appItoa(superEquipZhulinglvVec[i]), zhulingRecord) >= 0)
						{
							if (equipZhulinglvText[i].valid())
							{
								const std::string &title = superEquipAttrTab->getTitle(i + startindex);
								if (title == "攻击距离")
								{
									sprintf_s(buf, "%s\0", zhulingRecord[startindex + i].c_str());
								}
								else if(title == "溅射")
								{
									crVector2f vecbase;
									crArgumentParser::appAtoVec(zhulingRecord[startindex + i], vecbase);
									sprintf_s(buf, "(%d,%.1f%%)\0", (short)vecbase[0], vecbase[1] * 0.1f);
								}
								else
								{
									sprintf_s(buf, "+%.1f%%\0", atof(zhulingRecord[startindex + i].c_str()) * 0.1f);
								}
								equipZhulinglvText[i]->setString(std::string(buf));
							}
							if (equipZhulinglvAddButton[i].valid())
							{
								if (superEquipAttrTab->queryOneRecord(0, crArgumentParser::appItoa(superEquipZhulinglvVec[i] + 1), zhulingRecord) >= 0
									&& atoi(zhulingRecord[needPointIndex].c_str()) <= superEquipZhulingData->getZhulingPoint())
								{
									equipZhulinglvAddButton[i]->setEnable(true);
								}
								else
								{
									equipZhulinglvAddButton[i]->setEnable(false);
								}
							}
						}
					}
					equipZhulingPointText->setString(crArgumentParser::appItoa(superEquipZhulingData->getZhulingPoint()));
					if (equipZhulingResetButton.valid())
					{
						equipZhulingResetButton->setEnable(true);
					}
				}
			}
		}
		roledata->unlock();
		if (selectPage == 3 && actived)
		{
			//切页1 轮回信息
			crData *playerdata = myPlayer->getDataClass();
			void *param;
			playerdata->lock();
			playerdata->getParam(WCHDATA_JXJPlayerStore, param);
			ref_ptr<crJXJPlayerStore>playerStore = (crJXJPlayerStore *)param;
			if (playerStore.valid() && playerStore->getDataClass())
			{
				crData *playerStoreData = playerStore->getDataClass();
				playerStoreData->getParam(WCHDATA_JXJSuperEquipCycleLvupPoint, param);
				unsigned char cyclelvupoint = *(unsigned char *)param;
				playerStoreData->getParam(WCHDATA_JXJSuperEquipCycleExpVec, param);
				SuperEquipCycleExpVec *superEquipCycleExpVec = (SuperEquipCycleExpVec *)param;
				bool candistill = false;
				for (int i = 0; i < SuperEquipCount && i < superEquipCycleExpVec->size(); i++)
				{
					//CRCore::notify(CRCore::ALWAYS) <<"text:"<<lunhuiEquipExpText[i].get()<<" superEquipCycleExpVec i=" << i << " exp=" << (*superEquipCycleExpVec)[i]<< std::endl;
					if (lunhuiEquipExpText[i].valid())
						lunhuiEquipExpText[i]->setString(crArgumentParser::appItoa((*superEquipCycleExpVec)[i]));
					if ((*superEquipCycleExpVec)[i] > 0)
					{
						candistill = true;
					}
				}
				if (lunhuiEquipParamText.valid())
				{
					sprintf_s(buf, "升级效果:额外获得%d点注灵点数\0", cyclelvupoint);
					lunhuiEquipParamText->setString(std::string(buf));
				}
				//CRCore::notify(CRCore::ALWAYS) <<"text:"<<lunhuiEquipParamText.get()<<" cyclelvupoint =" << cyclelvupoint<< std::endl;
				if (cyclelvupoint > lvuppoint)
					candistill = true;
				if (lunhuiDistillButton.valid())
					lunhuiDistillButton->setEnable(candistill);
			}
			playerdata->unlock();
			//else
			//{
			//	CRCore::notify(CRCore::ALWAYS) << "playerStore 为空"<< std::endl;
			//}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSuperEquipZhulingMethod
//
/////////////////////////////////////////
crJXJSuperEquipZhulingMethod::crJXJSuperEquipZhulingMethod() :
	m_this(NULL)
{
}
crJXJSuperEquipZhulingMethod::crJXJSuperEquipZhulingMethod(const crJXJSuperEquipZhulingMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSuperEquipZhulingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJSuperEquipZhulingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pername = str;
		break;
	}
}
void crJXJSuperEquipZhulingMethod::operator()(crHandle &handle)
{
	std::string name = m_this->getName();
	name.erase(0, m_pername.size());
	int zhulingid = atoi(name.c_str())-1;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getMainRole())
		{
			crData *roledata = myPlayer->getMainRole()->getDataClass();
			void *param;
			roledata->lock();
			roledata->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
			SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
			if (superEquipZhulingData)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(zhulingid);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvSuperEquipZhuling, stream.get());
				netConductor->getNetManager()->sendPacket("all", packet);
			}
			roledata->unlock();
		}
	}
}
/////////////////////////////////////////
//
//crJXJSuperEquipZhulingTipMethod
//
/////////////////////////////////////////
crJXJSuperEquipZhulingTipMethod::crJXJSuperEquipZhulingTipMethod()
{
}
crJXJSuperEquipZhulingTipMethod::crJXJSuperEquipZhulingTipMethod(const crJXJSuperEquipZhulingTipMethod& handle) :
	crMethod(handle),
	m_tipsCanvas(handle.m_tipsCanvas),
	m_tipsTextWidget(handle.m_tipsTextWidget),
	m_pername(handle.m_pername)
{
}
void crJXJSuperEquipZhulingTipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJSuperEquipZhulingTipMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_tipsCanvas = str;
		break;
	case 1:
		m_tipsTextWidget = str;
		break;
	case 2:
		m_pername = str;
		break;
	}
}

void crJXJSuperEquipZhulingTipMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> tips = crFilterRenderManager::getInstance()->findCanvas(m_tipsCanvas);
	if (!tips.valid())
		return;

	void *param;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData && playerData->getMainRole() && playerData->getMainRole()->getDataClass())
	{
		int needpoint = 0;
		ref_ptr<crHypertextWidgetNode>  tipsText = dynamic_cast<crHypertextWidgetNode  *>(tips->getWidget(m_tipsTextWidget));
		std::string name = m_this->getName();
		name.erase(0, m_pername.size());
		int zhulingid = atoi(name.c_str()) - 1;
		crData *roledata = playerData->getMainRole()->getDataClass();
		roledata->lock();
		roledata->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
		SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
		if (superEquipZhulingData)
		{
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJVipLv, param);
			unsigned char vipLevel = *(unsigned char *)param;
			float factor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJVipZhulingFactor, vipLevel).c_str());
			ref_ptr<crTableIO> superEquipAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipAttrTab);
			int needPointIndex = superEquipAttrTab->getTitleIndex("消耗点数");
			crTableIO::StrVec zhulingRecord;
			int zhulinglv = superEquipZhulingData->getZhulinglv(zhulingid) + 1;
			if (superEquipAttrTab->queryOneRecord(0, crArgumentParser::appItoa(zhulinglv), zhulingRecord) >= 0)
			{
				needpoint = ceil(atof(zhulingRecord[needPointIndex].c_str())*factor);
			}
		}
		roledata->unlock();

		if (needpoint > 0)
		{
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
			cfg_script.Add("Text", "    消耗");
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
			sprintf(tmpText, "%d注灵点\0", needpoint);
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
			cfg_script.Add("Text", "注灵");
			cfg_script.Pop();

			cfg_script.Pop();
			//cfg_script.Write("tipstest.cfg");
			if (tipsText.valid()) tipsText->setHypertext(cfg_script);

			const crBoundingBox &btnbox = m_this->getBoundBox();
			const crBoundingBox &tipsbox = tips->getBoundBox();
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			float posx = btnbox.m_min[0];
			float posy = (btnbox.m_min[1] - tipsbox.yLength() / 2);
			////////////
			crVector2 mouse(crVector2(posx, posy));
			tips->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f) * mat);
			crFilterRenderManager::getInstance()->showCanvas(tips.get(), true);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvSuperEquipZhulingMethod
//
/////////////////////////////////////////
crJXJRecvSuperEquipZhulingMethod::crJXJRecvSuperEquipZhulingMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvSuperEquipZhulingMethod::crJXJRecvSuperEquipZhulingMethod(const crJXJRecvSuperEquipZhulingMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvSuperEquipZhulingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvSuperEquipZhulingMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvSuperEquipZhulingMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				crData *data = m_this->getDataClass();
				data->getParam(WCHDATA_JXJVipLv, param);
				unsigned char vipLevel = *(unsigned char *)param;
				float factor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJVipZhulingFactor, vipLevel).c_str());

				unsigned char zhulingid = m_stream->_readUChar();
				unsigned char zhulinglv;
				crData *roleData = m_this->getMainRole()->getDataClass();
				roleData->lock();
				roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				if (superEquipZhulingData && zhulingid<SuperEquipZhulingCount)
				{
					ref_ptr<crTableIO> superEquipAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipAttrTab);
					int needPointIndex = superEquipAttrTab->getTitleIndex("消耗点数");
					crTableIO::StrVec zhulingRecord;
					zhulinglv = superEquipZhulingData->getZhulinglv(zhulingid)+1;
					if (superEquipAttrTab->queryOneRecord(0, crArgumentParser::appItoa(zhulinglv), zhulingRecord) >= 0)
					{
						short needpoint = ceil(atof(zhulingRecord[needPointIndex].c_str())*factor);
						if (needpoint <= superEquipZhulingData->getZhulingPoint())
						{
							superEquipZhulingData->setZhulinglv(zhulingid,zhulinglv);
							superEquipZhulingData->reduceZhulingPoint(needpoint);
							success = 1;//注灵成功
						}
						else
						{//
							success = -1;//失败，注灵点数不足
						}
					}
					else
					{//
						success = -2;//已经到达注灵最高等级
					}
				}
				roleData->unlock();

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(6);
				stream->_writeChar(success);
				if (success == 1)
				{
					stream->_writeUChar(zhulingid);
					stream->_writeUChar(zhulinglv);
					stream->_writeUShort(superEquipZhulingData->getZhulingPoint());
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvSuperEquipZhuling, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				///游戏日志
				std::string logdata = crArgumentParser::appVectoa(crVector3i(success,zhulingid, zhulinglv), '|');
				GameLogData gamelog(Log_RecvSuperEquipZhuling, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
			}
		}
		else if (m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			switch (success)
			{
			case 0:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1141, NULL));//不能注灵!
				break;
			case -1:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1142, NULL));//失败，注灵点数不足!
				break;
			case -2:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1143, NULL));//已经到达注灵最高等级!
				break;
			case 1:
				{
					unsigned char zhulingid = m_stream->_readUChar();
					unsigned char zhulinglv = m_stream->_readUChar();
					unsigned short zhulingpoint = m_stream->_readUShort();
					if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
					{
						crData *roleData = m_this->getMainRole()->getDataClass();
						void *param;
						roleData->lock();
						roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
						SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
						if (superEquipZhulingData)
						{
							superEquipZhulingData->setZhulingPoint(zhulingpoint);
							superEquipZhulingData->setZhulinglv(zhulingid,zhulinglv);
						}
						roleData->unlock();
					}
				}
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJResetZhulingMsgBoxMethod
//
/////////////////////////////////////////
crJXJResetZhulingMsgBoxMethod::crJXJResetZhulingMsgBoxMethod()
{
}
crJXJResetZhulingMsgBoxMethod::crJXJResetZhulingMsgBoxMethod(const crJXJResetZhulingMsgBoxMethod& handle) :
	crMethod(handle),
	m_msgbox(handle.m_msgbox),
	m_msgtext(handle.m_msgtext)
{
}
void crJXJResetZhulingMsgBoxMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJResetZhulingMsgBoxMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_msgbox = str;
		break;
	case 1:
		m_msgtext = str;
		break;
	}
}
void crJXJResetZhulingMsgBoxMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
	if (msgCanvas.valid() && myPlayer && netConductor)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char vipLevel = *(unsigned char *)param;
		int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResetZhulingGold, vipLevel).c_str());
		int checkmoney = needgold;
		myPlayer->doEvent(WCH_JXJCheckLijinGold, MAKECREPARAM(&checkmoney, NULL));
		if (checkmoney == 0)
		{
			crHypertextWidgetNode *msgtext = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msgtext));
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
			cfg_script.Add("Text", "您确认要花费");
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			char tmpText[64];
			sprintf(tmpText, "%d礼金/元宝\0", needgold);
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
			cfg_script.Add("Text", "重置注灵吗？");
			cfg_script.Pop();

			cfg_script.Pop();

			if (msgtext)msgtext->setHypertext(cfg_script);
			crData *dlgData = msgCanvas->getDataClass();
			int commandtype = CDP_Widget;
			dlgData->inputParam(WCHDATA_CommandDlgParentType, &commandtype);
			dlgData->inputParam(WCHDATA_CommandDlgParent, m_this);
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{
			myPlayer->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2036, NULL));
		}
	}
}
/////////////////////////////////////////
//
//crJXJResetZhulingMethod
//
/////////////////////////////////////////
crJXJResetZhulingMethod::crJXJResetZhulingMethod():
	m_this(NULL)
{
}
crJXJResetZhulingMethod::crJXJResetZhulingMethod(const crJXJResetZhulingMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJResetZhulingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJResetZhulingMethod::addParam(int i, const std::string& str)
{
}
void crJXJResetZhulingMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvResetZhuling, NULL);
		netConductor->getNetManager()->sendPacket("all", packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvResetZhulingMethod
//
/////////////////////////////////////////
crJXJRecvResetZhulingMethod::crJXJRecvResetZhulingMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvResetZhulingMethod::crJXJRecvResetZhulingMethod(const crJXJRecvResetZhulingMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvResetZhulingMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvResetZhulingMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvResetZhulingMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				unsigned short gainpoint = 0;
				crData *roleData = m_this->getMainRole()->getDataClass();
				roleData->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
				unsigned char lvuppoint = *(unsigned char *)param;
				roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				if (superEquipZhulingData)
				{
					crData *data = m_this->getDataClass();
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char vipLevel = *(unsigned char *)param;
					int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJResetZhulingGold, vipLevel).c_str());
					MoneyChangeData moneydata(needgold, "重置注灵");
					bool needdeductgold = false;
					m_this->doEvent(WCH_JXJDeductLijinGold, MAKECREPARAM(&moneydata, &needdeductgold));
					if (moneydata.first == 0)
					{
						roleData->lock();
						crTableIO::StrVec record;
						ref_ptr<crTableIO> superEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipTab);
						int lvindex = superEquipTab->getTitleIndex("等级");
						int pointindex = superEquipTab->getTitleIndex("注灵点");
						unsigned short totalpoint = 0;
						unsigned short curzhulingpoint = superEquipZhulingData->getZhulingPoint();
						roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
						SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
						for (SuperEquipDataVec::iterator itr = superEquipDataVec->begin();
							itr != superEquipDataVec->end();
							++itr)
						{
							for (int i = 1; i <= (*itr)->getSuperEquipLevel(); i++)
							{
								if (superEquipTab->queryOneRecord(0, crArgumentParser::appItoa((*itr)->getSuperEquipID()), lvindex, crArgumentParser::appItoa(i), record) >= 0)
								{
									totalpoint += atoi(record[pointindex].c_str()) + lvuppoint;
								}
							}
						}
						gainpoint = totalpoint - curzhulingpoint;
						superEquipZhulingData->setZhulingPoint(totalpoint);
						superEquipZhulingData->resetZhuling();
						success = 1;
						roleData->unlock();

						if (needdeductgold)
						{
							crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
							callback->savePlayerData(playerData.get());
						}
					}
					else
					{
						success = -1;//元宝不够
					}
				}
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(3);
				stream->_writeChar(success);
				if (success == 1)
				{
					stream->_writeUShort(superEquipZhulingData->getZhulingPoint());
					///游戏日志
					std::string logdata = crArgumentParser::appVectoa(crVector2i(success, gainpoint), '|');
					GameLogData gamelog(Log_RecvResetZhuling, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvResetZhuling, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
			}
		}
		else if (m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			switch (success)
			{
			case 0:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1149, NULL));//请先激活神器
				break;
			case -1:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2036, NULL));//元宝不够
				break;
			case 1:
				{
					unsigned short zhulingpoint = m_stream->_readUShort();
					if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
					{
						crData *roleData = m_this->getMainRole()->getDataClass();
						void *param;
						roleData->lock();
						roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
						SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
						if (superEquipZhulingData)
						{
							superEquipZhulingData->setZhulingPoint(zhulingpoint);
							superEquipZhulingData->resetZhuling();
						}
						roleData->unlock();
					}
				}
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJServerUseExpPelletMethod
//
/////////////////////////////////////////
crJXJServerUseExpPelletMethod::crJXJServerUseExpPelletMethod() :
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL) {}
crJXJServerUseExpPelletMethod::crJXJServerUseExpPelletMethod(const crJXJServerUseExpPelletMethod& handle) :
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseExpPelletMethod::inputParam(int i, void *param)
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
		m_this = (crDataObject*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_user = (crPlayerGameData*)LOCREPARAM(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HICREPARAM(param64);
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

void crJXJServerUseExpPelletMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseExpPelletMethod::operator()(crHandle &handle)
{
	if (m_user && m_itemid > 0)
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getMainRole() && playerData->getPlayerGameData()->getMainRole()->getDataClass())
		{
			ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
			crTableIO::StrVec record;
			if (expPelletTab->queryOneRecord(0, crArgumentParser::appItoa(m_itemid), record) >= 0)
			{
				int equipindex = expPelletTab->getTitleIndex("equipid");
				int expindex = expPelletTab->getTitleIndex("exp");
				int equipid = atoi(record[equipindex].c_str());
				int exp = atoi(record[expindex].c_str());

				crData *roleData = playerData->getPlayerGameData()->getMainRole()->getDataClass();
				void *param;
				roleData->lock();
				roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
				SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
				ref_ptr<SuperEquipData> superEquipData;
				for (SuperEquipDataVec::iterator itr = superEquipDataVec->begin();
					itr != superEquipDataVec->end();
					++itr)
				{
					if ((*itr)->getSuperEquipID() == equipid)
					{
						superEquipData = *itr;
						break;
					}
				}
				if (superEquipData.valid())
				{
					if (superEquipData->getSuperEquipLevel() == 0)
					{
						*m_useResult = USE_SuperEquipNotActived;
					}
					else if (superEquipData->getSuperEquipLevel() == SuperEquipMaxLevel)
					{
						*m_useResult = USE_SuperEquipMaxLevel;
					}
					else
					{
						roleData->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
						unsigned char lvuppoint = *(unsigned char *)param;
						roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
						SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
						unsigned short gainpoint = superEquipData->addSuperEquipExp(exp, lvuppoint);
						superEquipZhulingData->addZhulingPoint(gainpoint);
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(8);
						stream->_writeUChar(equipid);
						stream->_writeInt(superEquipData->getSuperEquipExp());
						stream->_writeUChar(superEquipData->getSuperEquipLevel());
						stream->_writeUShort(superEquipZhulingData->getZhulingPoint());

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvUseExpPellet, stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
						////游戏日志
						*m_useResult = 1;
						std::string logdata = "使用经验丹：" + crArgumentParser::appItoa(m_itemid) + "获得：" + crArgumentParser::appItoa(exp) + "神器经验，" + crArgumentParser::appItoa(gainpoint) + "注灵点";
						GameLogData gamelog(Log_UseExpPellet, logdata);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
					}
				}
				roleData->unlock();				
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUseExpPelletMethod
//
/////////////////////////////////////////
crJXJRecvUseExpPelletMethod::crJXJRecvUseExpPelletMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvUseExpPelletMethod::crJXJRecvUseExpPelletMethod(const crJXJRecvUseExpPelletMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvUseExpPelletMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvUseExpPelletMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUseExpPelletMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			unsigned char superEquipID = m_stream->_readUChar();
			int exp = m_stream->_readInt();
			unsigned char lv = m_stream->_readUChar();
			unsigned short point = m_stream->_readUShort();
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *roleData = m_this->getMainRole()->getDataClass();
				void *param;
				roleData->lock();
				roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
				SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
				roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				superEquipZhulingData->setZhulingPoint(point);
				ref_ptr<SuperEquipData> superEquipData;
				for (SuperEquipDataVec::iterator itr = superEquipDataVec->begin();
					itr != superEquipDataVec->end();
					++itr)
				{
					if ((*itr)->getSuperEquipID() == superEquipID)
					{
						superEquipData = *itr;
						int extraexp = exp - superEquipData->getSuperEquipExp();
						superEquipData->setSuperEquipExp(exp);
						superEquipData->setSuperEquipLevel(lv);
						if (extraexp != 0)
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_SuperEquipExp, extraexp));
						break;
					}
				}
				roleData->unlock();
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUseExpPelletMethod
//
/////////////////////////////////////////
crJXJUseExpPelletMethod::crJXJUseExpPelletMethod() :
	m_this(NULL)
{
}
crJXJUseExpPelletMethod::crJXJUseExpPelletMethod(const crJXJUseExpPelletMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJUseExpPelletMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJUseExpPelletMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_equipRadioGroup = str;
		break;
	case 1:
		m_expPelletCanvas = str;
		break;
	}
}
void crJXJUseExpPelletMethod::operator()(crHandle &handle)
{
	bool canceltask = false;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		ref_ptr<crRadioGroupWidgetNode> equipRadioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getParentCanvas()->getWidget(m_equipRadioGroup));
		int selectEquip = 0;
		if (equipRadioGroup.valid())
			selectEquip = equipRadioGroup->getSelect();
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if (myPlayer && myPlayer->getMainRole())
		{
			crData *roledata = myPlayer->getMainRole()->getDataClass();
			void *param;
			roledata->lock();
			roledata->getParam(WCHDATA_JXJSuperEquipDataVec, param);
			SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
			ref_ptr<SuperEquipData> superEquipData;
			if (selectEquip < superEquipDataVec->size())
			{
				superEquipData = (*superEquipDataVec)[selectEquip];
			}
			roledata->unlock();
			if(superEquipData.valid())
			{
				int superEquipID = superEquipData->getSuperEquipID();
				crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas(m_expPelletCanvas);
				if (canvas)
				{
					crData *canvasData = canvas->getDataClass();
					canvasData->inputParam(WCHDATA_JXJUIID, &superEquipID);
				}
				ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
				crTableIO::DataVec recordVec;
				expPelletTab->queryRecords(1, crArgumentParser::appItoa(superEquipID), recordVec);
				if (!recordVec.empty())
				{//检查背包里有没
					crData *data = myPlayer->getDataClass();
					//检查我背包里是否有这个itemid
					data->lock();
					data->getParam(WCHDATA_JXJItemBagVec, param);
					JXJItemBagVec *itemBagVec = (JXJItemBagVec *)param;
					int count = itemBagVec->size();
					for (crTableIO::DataVec::iterator itr = recordVec.begin();
						 itr != recordVec.end();
						++itr)
					{
						int itemid = atoi((*itr)[0].c_str());
						for (int i = 0;i<count;i++)
						{
							if ((*itemBagVec)[i].valid() && (*itemBagVec)[i]->getItemID() == itemid && (*itemBagVec)[i]->getItemCount()>0)
							{
								canceltask = true;
								myPlayer->doEvent(MAKEINT64(WCH_JXJUseBagItem, NULL), MAKECREPARAM(itemid, i));
								break;
							}
						}
					}
					data->unlock();
				}
			}
		}
	}
	handle.outputParam(0, &canceltask);
}
/////////////////////////////////////////
//
//crJXJUIExpPelletCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJUIExpPelletCanvasUpdateMethod::crJXJUIExpPelletCanvasUpdateMethod()
{
}
crJXJUIExpPelletCanvasUpdateMethod::crJXJUIExpPelletCanvasUpdateMethod(const crJXJUIExpPelletCanvasUpdateMethod& handle) :
	crMethod(handle)
{
}
void crJXJUIExpPelletCanvasUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJUIExpPelletCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
		m_buyButton[i] = str;
		break;
	case 3:
	case 4:
	case 5:
		m_priceText[i-3] = str;
		break;
	case 6:
	case 7:
	case 8:
		m_imageBox[i-6] = str;
		break;
	case 9:
	case 10:
	case 11:
		m_countText[i-9] = str;
		break;
	}
}
void crJXJUIExpPelletCanvasUpdateMethod::operator()(crHandle &handle)
{
	crPlayerGameData *player = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = player->getDataClass();
	void *param;
	data->getParam(WCHDATA_JXJVipLv, param);
	unsigned char vipLevel = *(unsigned char *)param;
	int maxtimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpPelletBuyTimes, vipLevel).c_str());
	unsigned char buytimes = 0;
	if (player->getMainRole() && player->getMainRole()->getDataClass())
	{
		crData *roleData = player->getMainRole()->getDataClass();
		roleData->getParam(WCHDATA_JXJExpPelletBuyTimes, param);
		buytimes = *(unsigned char *)param;
	}

	ref_ptr<crButtonWidgetNode> buyButton[3];
	ref_ptr<crImageBoxWidgetNode> imageBox[3];
	ref_ptr<crStaticTextWidgetNode> priceText[3];
	ref_ptr<crStaticTextWidgetNode> countText[3];
	for (int i = 0; i < 3; i++)
	{
		buyButton[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_buyButton[i]));
		imageBox[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox[i]));
		priceText[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_priceText[i]));
		countText[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_countText[i]));
	}
	crData *canvasData = m_this->getDataClass();
	canvasData->getParam(WCHDATA_JXJUIID, param);
	int superEquipID = *(int *)param;

	ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
	int countindex = expPelletTab->getTitleIndex("购买数量");
	int priceindex = expPelletTab->getTitleIndex("价格");
	ref_ptr<crTableIO>itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	int iconindex = itemTab->getTitleIndex("icon");
	crTableIO::DataVec records;
	crTableIO::StrVec itemrecord;
	expPelletTab->queryRecords(1, crArgumentParser::appItoa(superEquipID), records);
	data->getParam(WCHDATA_Money, param);
	int money = *(int *)param;
	//CRCore::notify(CRCore::ALWAYS) << "superEquipID=" << superEquipID<<" recsize="<< records.size() << std::endl;
	for (int i = 0; i<3&&i<records.size();i++)
	{
		priceText[i]->setString(records[i][priceindex]);
		countText[i]->setString(records[i][countindex]);
		if (itemTab->queryOneRecord(0, records[i][0], itemrecord) >= 0)
		{
			imageBox[i]->setImageName(itemrecord[iconindex]);
		}
		int price = atoi(records[i][priceindex].c_str());
		buyButton[i]->setEnable(buytimes<maxtimes && price <= money);
	}
}
/////////////////////////////////////////
//
//crJXJBuyExpPelletMsgBoxMethod
//
/////////////////////////////////////////
crJXJBuyExpPelletMsgBoxMethod::crJXJBuyExpPelletMsgBoxMethod()
{
}
crJXJBuyExpPelletMsgBoxMethod::crJXJBuyExpPelletMsgBoxMethod(const crJXJBuyExpPelletMsgBoxMethod& handle) :
	crMethod(handle),
	m_msgbox(handle.m_msgbox),
	m_msgtext(handle.m_msgtext)
{
}
void crJXJBuyExpPelletMsgBoxMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyExpPelletMsgBoxMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pername = str;
		break;
	case 1:
		m_msgbox = str;
		break;
	case 2:
		m_msgtext = str;
		break;
	}
}
void crJXJBuyExpPelletMsgBoxMethod::operator()(crHandle &handle)
{
	std::string name = m_this->getName();
	name.erase(0, m_pername.size());
	int index = atoi(name.c_str()) - 1;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
	if (msgCanvas.valid() && myPlayer && netConductor)
	{
		crData *canvasData = m_this->getParentCanvas()->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJUIID, param);
		int superEquipID = *(int *)param;
		ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
		int priceindex = expPelletTab->getTitleIndex("价格");
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int nameindex = itemtab->getTitleIndex("name");
		crTableIO::StrVec itemrecord;
		crTableIO::DataVec records;
		expPelletTab->queryRecords(1, crArgumentParser::appItoa(superEquipID), records);
		crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
		data->getParam(WCHDATA_Money, param);
		int money = *(int *)param;
		if (index < records.size() && itemtab->queryOneRecord(0, records[index][0], itemrecord) >= 0)
		{
			int cost = atoi(records[index][priceindex].c_str());
			if (cost <= money)
			{
				crHypertextWidgetNode *msgtext = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msgtext));
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
				cfg_script.Add("Text", "您确认要花费");
				cfg_script.Pop();

				cfg_script.Add("Content");
				cfg_script.Push("Content", 2);
				v_i.clear();
				v_i.push_back(255.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				char tmpText[64];
				sprintf(tmpText, "%d元宝\0", cost);
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
				sprintf(tmpText, "购买商品[%s]吗？\0", itemrecord[nameindex].c_str());
				cfg_script.Add("Text", tmpText);
				cfg_script.Pop();

				cfg_script.Pop();

				if (msgtext)msgtext->setHypertext(cfg_script);
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType, &commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent, m_this);
				crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
			}
			else
			{
				myPlayer->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(3025, NULL));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuyExpPelletMethod
//
/////////////////////////////////////////
crJXJBuyExpPelletMethod::crJXJBuyExpPelletMethod() :
	m_this(NULL)
{
}
crJXJBuyExpPelletMethod::crJXJBuyExpPelletMethod(const crJXJBuyExpPelletMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJBuyExpPelletMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyExpPelletMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_pername = str;
		break;
	}
}
void crJXJBuyExpPelletMethod::operator()(crHandle &handle)
{
	std::string name = m_this->getName();
	name.erase(0, m_pername.size());
	int index = atoi(name.c_str()) - 1;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crData *canvasData = m_this->getParentCanvas()->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJUIID, param);
		int superEquipID = *(int *)param;
		ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
		crTableIO::DataVec records;
		expPelletTab->queryRecords(1, crArgumentParser::appItoa(superEquipID), records);
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeInt(atoi(records[index][0].c_str()));
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvBuyExpPellet, stream.get());
		netConductor->getNetManager()->sendPacket("all", packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuyExpPelletMethod
//
/////////////////////////////////////////
crJXJRecvBuyExpPelletMethod::crJXJRecvBuyExpPelletMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvBuyExpPelletMethod::crJXJRecvBuyExpPelletMethod(const crJXJRecvBuyExpPelletMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvBuyExpPelletMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvBuyExpPelletMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuyExpPelletMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				void *param;
				crData *roledata = m_this->getMainRole()->getDataClass();
				roledata->getParam(WCHDATA_JXJExpPelletBuyTimes, param);
				unsigned char buytimes = *(unsigned char *)param;
				bool needdeductgold = false;
				int itemid = m_stream->_readInt();
				ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
				int countindex = expPelletTab->getTitleIndex("购买数量");
				int priceindex = expPelletTab->getTitleIndex("价格");
				crTableIO::StrVec record;
				if (expPelletTab->queryOneRecord(0, crArgumentParser::appItoa(itemid), record) >= 0)
				{
					crData *data = m_this->getDataClass();
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char vipLevel = *(unsigned char *)param;
					int maxtimes = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpPelletBuyTimes, vipLevel).c_str());
					if (buytimes < maxtimes)
					{
						data->getParam(WCHDATA_Money, param);
						int money = *(int *)param;
						int price = atoi(record[priceindex].c_str());
						if (money >= price)
						{
							MoneyChangeData moneydata(-price, "元宝购买经验丹:" + record[0]);
							m_this->doEvent(WCH_JXJDeductLijinGold, MAKECREPARAM(&moneydata, &needdeductgold));
							if (moneydata.first == -1)
							{
								success = -1;//元宝不够
							}
							else
							{
								ref_ptr<crBagItemData> itemdata = new crBagItemData;
								itemdata->setItemID(itemid);
								itemdata->setItemCount(atoi(record[countindex].c_str()));
								RewardItemVec rewardItems;
								rewardItems.push_back(itemdata);
								data->lock();
								m_this->doEvent(WCH_JXJRecvRewardItems, MAKECREPARAM(&rewardItems, 0));
								data->unlock();
								buytimes++;
								roledata->inputParam(WCHDATA_JXJExpPelletBuyTimes, &buytimes);
							}
						}
						else
						{
							success = -1;
						}
					}
					else
					{
						success = -2;//已达到购买次数上限
					}
				}

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(success);
				stream->_writeUChar(buytimes);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvBuyExpPellet, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);

				if (success == 0)
				{
					std::string logdata = "元宝购买经验丹：" + crArgumentParser::appItoa(itemid);
					GameLogData gamelog(Log_BuyExpPellet, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
					if (needdeductgold)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
						callback->savePlayerData(playerData.get());
					}
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			char code = m_stream->_readChar();
			unsigned char buytimes = m_stream->_readUChar();
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *roleData = m_this->getMainRole()->getDataClass();
				roleData->inputParam(WCHDATA_JXJExpPelletBuyTimes, &buytimes);
			}
			if (code == 0)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2118, NULL));
			}
			else if (code == -1)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2119, NULL));
			}
			else if (code == -2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2120, NULL));
			}
		}
	}
}
///////////////////////////////////////////
////
////crJXJExpPelletTipsMethod
////
///////////////////////////////////////////
crJXJExpPelletTipsMethod::crJXJExpPelletTipsMethod() :
	m_ea(NULL),
	m_this(NULL)
{
}
crJXJExpPelletTipsMethod::crJXJExpPelletTipsMethod(const crJXJExpPelletTipsMethod& handle) :
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info2(handle.m_info2),
	m_itemicon(handle.m_itemicon),
	m_colorred(handle.m_colorred),
	//m_ifexpand(handle.m_ifexpand),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	//m_expandnum(handle.m_expandnum),
	//m_imageindex(handle.m_imageindex),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJExpPelletTipsMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOCREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJExpPelletTipsMethod::addParam(int i, const std::string& str)
{
	switch (i)
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
		m_infocanvas = str;
		break;
	case 5:
		m_infobk = str;
		break;
	case 6:
		m_infodi = str;
		break;
	case 7:
		crArgumentParser::appAtoVec(str, m_offsets);
		break;
	case 8:
		crArgumentParser::appAtoVec(str, m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 9:
		m_pername = str;
		break;
	}
}
void crJXJExpPelletTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> parentcanas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode> tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode> infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (tipsCanvas.valid() && playerData && infocanvas.valid() && parentcanas.valid())
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

		crVector4f colormode[8];//white,green,blue,purple,orange
		ref_ptr<crTableIO>qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		for (int i = 0; i < 8; i++)
		{
			if (qualitytab->queryOneRecord(0, crArgumentParser::appItoa(i), colorrecord) >= 0)
			{
				if (qualityindex >= 0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex], colormode[i]);
					colormode[i] /= 255.0f;
				}
			}
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO>itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconindex = itemtab->getTitleIndex("icon");
		int nameindex = itemtab->getTitleIndex("name");
		int colorindex = itemtab->getTitleIndex("品质");

		std::string widgetname = m_this->getName();
		widgetname.erase(0, m_pername.size());
		int index = atoi(widgetname.c_str()) - 1;
		crData *canvasData = m_this->getParentCanvas()->getDataClass();
		void *param;
		canvasData->getParam(WCHDATA_JXJUIID, param);
		int superEquipID = *(int *)param;
		ref_ptr<crTableIO>expPelletTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJExpPelletTab);
		crTableIO::DataVec records;
		expPelletTab->queryRecords(1, crArgumentParser::appItoa(superEquipID), records);
		int itemid = atoi(records[index][0].c_str());

		if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), record) >= 0)
		{
			int colormodeid = atoi(record[colorindex].c_str());
			if (name)
			{
				name->setColor(colormode[colormodeid]);
				name->setString(record[nameindex]);
				//CRCore::notify(CRCore::ALWAYS) << "Color="<< colormodeid<<" name="<< record[nameindex] << std::endl;
			}
			if (itemicon)
			{
				itemicon->setImageName(record[iconindex]);
			}
			if (info2)
			{
				ItemMap itemMap;
				rcfg::ConfigScript outscript(&itemMap);
				crVector2i item(itemid, 0);
				playerData->doEvent(WCH_JXJCreateItemTips, MAKECREPARAM(&item, &outscript));
				info2->setHypertext(outscript);
			}

			///////////计算缩放
			float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
			info2->updateData();
			const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
			const crBoundingBox &bbox2 = info2->getBoundBox();
			float texheight = 0.0f;
			float needmove = 0.0f;
			if (bbox.valid())
			{
				texheight = bbox.yLength();
				texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale() / crDisplaySettings::instance()->getUIScaleFactor();
				if (texheight < boxYlength) texheight = boxYlength;
				float scale = texheight / boxYlength;

				float needmove;
				const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
				crVector3 centerTansTar = info2->getBound().center();

				float tmpLength = bbox2.yLength();
				if (infodi)
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
				rotmat.postMult(crMatrix::scale(1.0f, scale, 1.0f));
				rotmat.postMult(crMatrix::translate(centerTansTar));
				if (infobk)infobk->setMatrix(rotmat);

				if (infodi)
				{
					float diYlength = infodi->getChild(0)->getBoundBox().yLength();
					//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
					//needmove = boxYlength - texheight;
					needmove = boxYlength - texheight + diYlength * 0.25f;
					infodi->setMatrix(crMatrix::translate(0.0f, needmove/*+c_tipsoffsets[2]*scale*/, 0.0f));
				}
			}
			else
			{
				infobk->setMatrix(crMatrix::scale(1.0f, 1.0f/*m_expandnum*/, 1.0f) * crMatrix::translate(0.0f, 0.0f, 0.0f));
				if (infodi) infodi->setMatrix(crMatrix::translate(0.0f, 0.0f, 0.0f));
			}

			/////////////////////////////////////////
			const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
			const crBoundingBox &iconbox = m_this->getBoundBox();
			const crBoundingBox &tips = tipsCanvas->getBoundBox();
			float posx = (tips.m_max[0] - tips.m_min[0])*0.5;
			float posy = (tips.m_max[1] - tips.m_min[1])*0.5;
			crVector3 pos(iconbox.m_max[0], iconbox.m_max[1], 0.0f);
			/////////////////////////////////////////
			crVector3 pos2(iconbox.m_min[0], iconbox.m_min[1], 0.0f);
			pos = pos * mat;
			pos2 = pos2 * mat;
			float posx2, posy2;
			const crBoundingBox &infos = infobk->getBoundBox();
			float posinfox = (infos.m_max[0] - infos.m_min[0]);
			float posinfoy = (infos.m_max[1] - infos.m_min[1]);
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
			if ((abs(posy2) + posy) > 1.0f)
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
			crVector2 mouse(crVector2(posx2, posy2));
			/////////////////////////////////////////
			tipsCanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f));
			tipsCanvas->setCanFocus(false);
			infocanvas->setCanFocus(false);
			infocanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f));

			crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(), true);
			crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(), true);
		}
	}
}
/////////////////////////////////////////
//
//crJXJActiveSuperEquipMethod
//
/////////////////////////////////////////
crJXJActiveSuperEquipMethod::crJXJActiveSuperEquipMethod()
{
}
crJXJActiveSuperEquipMethod::crJXJActiveSuperEquipMethod(const crJXJActiveSuperEquipMethod& handle) :
	crMethod(handle)
{
}
void crJXJActiveSuperEquipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crRole*)param;
		break;
	}
}
void crJXJActiveSuperEquipMethod::addParam(int i, const std::string& str)
{
}
void crJXJActiveSuperEquipMethod::operator()(crHandle &handle)
{
	crData *roledata = m_this->getDataClass();
	void *param;
	ref_ptr<crTableIO> superEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipTab);
	int lvindex = superEquipTab->getTitleIndex("等级");
	crTableIO::DataVec dataVec;
	superEquipTab->queryRecords(lvindex, "1", dataVec);
	bool success = false;
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(SuperEquipCount*2+1);
	roledata->lock();
	roledata->getParam(WCHDATA_JXJSuperEquipDataVec, param);
	SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
	if (dataVec.size() >= SuperEquipCount)
	{
		superEquipDataVec->resize(SuperEquipCount);
		stream->_writeUChar(SuperEquipCount);
		ref_ptr<SuperEquipData> superEquipData;
		int superid;
		crTableIO::StrVec record;
		ref_ptr<crTableIO> superEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipTab);
		int lvindex = superEquipTab->getTitleIndex("等级");
		int pointindex = superEquipTab->getTitleIndex("注灵点");
		roledata->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
		unsigned char lvuppoint = *(unsigned char*)param;
		unsigned short gainpoint = 0;
		for (int i = 0; i < SuperEquipCount; i++)
		{
			superid = atoi(dataVec[i][0].c_str());
			superEquipData = (*superEquipDataVec)[i].get();
			if (!superEquipData.valid() || superid != superEquipData->getSuperEquipID())
			{
				superEquipData = new SuperEquipData(superid);
				(*superEquipDataVec)[i] = superEquipData;
				if (superEquipTab->queryOneRecord(0, crArgumentParser::appItoa(superid), lvindex, crArgumentParser::appItoa(1), record) >= 0)
				{
					gainpoint += atoi(record[pointindex].c_str()) + lvuppoint;
				}
			}
			if (superEquipData->getSuperEquipLevel() == 0)
				superEquipData->setSuperEquipLevel(1);
			stream->_writeUChar(superEquipData->getSuperEquipID());
			stream->_writeUChar(superEquipData->getSuperEquipLevel());
		}
		roledata->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
		SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
		if (!superEquipZhulingData)
		{
			superEquipZhulingData = new SuperEquipZhulingData;
			roledata->inputParam(WCHDATA_JXJSuperEquipZhulingData, superEquipZhulingData);
		}
		superEquipZhulingData->addZhulingPoint(gainpoint);
		stream->_writeUShort(superEquipZhulingData->getZhulingPoint());
		success = true;
	}
	else
	{
		stream->_writeUChar(0);
	}
	roledata->unlock();
	int playerid = m_this->getPlayerID();
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
	if (playerData.valid())
	{
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvActiveSuperEquip, stream.get());
		gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
	}
	GameLogData gamelog(Log_ActiveSuperEquip, success?"神器激活成功": "神器激活失败");
	crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
}
/////////////////////////////////////////
//
//crJXJRecvActiveSuperEquipMethod
//
/////////////////////////////////////////
crJXJRecvActiveSuperEquipMethod::crJXJRecvActiveSuperEquipMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvActiveSuperEquipMethod::crJXJRecvActiveSuperEquipMethod(const crJXJRecvActiveSuperEquipMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvActiveSuperEquipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvActiveSuperEquipMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvActiveSuperEquipMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *roleData = m_this->getMainRole()->getDataClass();
				void *param;
				roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
				SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
				unsigned char count = m_stream->_readUChar();
				if (count > 0)
				{
					roleData->lock();
					superEquipDataVec->resize(count);
					ref_ptr<SuperEquipData> superEquipData;
					unsigned char superid;
					unsigned char lv;
					for (int i = 0; i < count; i++)
					{
						superid = m_stream->_readUChar();
						lv = m_stream->_readUChar();
						superEquipData = (*superEquipDataVec)[i].get();
						if (!superEquipData.valid() || superid != superEquipData->getSuperEquipID())
						{
							superEquipData = new SuperEquipData(superid);
							(*superEquipDataVec)[i] = superEquipData;
						}
						superEquipData->setSuperEquipLevel(lv);
					}
					roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
					SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
					if (!superEquipZhulingData)
					{
						superEquipZhulingData = new SuperEquipZhulingData;
						roleData->inputParam(WCHDATA_JXJSuperEquipZhulingData, superEquipZhulingData);
					}
					superEquipZhulingData->setZhulingPoint(m_stream->_readUShort());
					roleData->unlock();

					int texid = 1148;
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
					noticeHandle->inputParam(WCHDATA_NoticeTextID, &texid);
					std::string param1 = crArgumentParser::appItoa(count);
					noticeHandle->inputParam(WCHDATA_NoticeParam1, &param1);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
				}
				else
				{
					m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1146, NULL));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvDistillSuperEquipMethod
//
/////////////////////////////////////////
crJXJRecvDistillSuperEquipMethod::crJXJRecvDistillSuperEquipMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvDistillSuperEquipMethod::crJXJRecvDistillSuperEquipMethod(const crJXJRecvDistillSuperEquipMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvDistillSuperEquipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvDistillSuperEquipMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvDistillSuperEquipMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *data = m_this->getDataClass();
				void *param;
				data->getParam(WCHDATA_JXJPlayerStore, param);
				ref_ptr<crJXJPlayerStore>playerStore = (crJXJPlayerStore *)param;
				crData *playerStoreData = playerStore->getDataClass();
				playerStoreData->getParam(WCHDATA_JXJSuperEquipCycleLvupPoint, param);
				unsigned char *lvupoint = (unsigned char *)param;
				playerStoreData->getParam(WCHDATA_JXJSuperEquipCycleExpVec, param);
				SuperEquipCycleExpVec *superEquipCycleExpVec = (SuperEquipCycleExpVec *)param;
				int count = superEquipCycleExpVec->size();
				crData *roleData = m_this->getMainRole()->getDataClass();
				roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
				SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
				roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				//roleData->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
				//unsigned char *superEquipLvupPoint = (unsigned char *)param;
				//*superEquipLvupPoint = m_stream->_readUChar();
				*lvupoint = m_stream->_readUChar();
				roleData->lock();
				for (int i = 0; i < SuperEquipCount; i++)
				{
					if(i<count)
						(*superEquipCycleExpVec)[i] = 0;
					(*superEquipDataVec)[i]->setSuperEquipLevel(m_stream->_readUChar());
					(*superEquipDataVec)[i]->setSuperEquipExp(m_stream->_readInt());
				}
				superEquipZhulingData->setZhulingPoint(m_stream->_readUShort());
				roleData->unlock();
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJDistillSuperEquipMethod
//
/////////////////////////////////////////
crJXJDistillSuperEquipMethod::crJXJDistillSuperEquipMethod() :
	m_this(NULL)
{
}
crJXJDistillSuperEquipMethod::crJXJDistillSuperEquipMethod(const crJXJDistillSuperEquipMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJDistillSuperEquipMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJDistillSuperEquipMethod::addParam(int i, const std::string& str)
{
}
void crJXJDistillSuperEquipMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer && myPlayer->getMainRole())
	{
		crData *roledata = myPlayer->getMainRole()->getDataClass();
		void *param;
		roledata->lock();
		roledata->getParam(WCHDATA_JXJSuperEquipDataVec, param);
		SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
		if (superEquipDataVec->size() == SuperEquipCount)
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if (netConductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(4);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvQuChuLunHuiThing, stream.get());
				netConductor->getNetManager()->sendPacket("all", packet);
			}
		}
		roledata->unlock();
	}
}
/////////////////////////////////////////
//
//crJXJGainSuperEquipLvupPointMethod
//
/////////////////////////////////////////
crJXJGainSuperEquipLvupPointMethod::crJXJGainSuperEquipLvupPointMethod():
	m_lvuppoint(0)
{
}
crJXJGainSuperEquipLvupPointMethod::crJXJGainSuperEquipLvupPointMethod(const crJXJGainSuperEquipLvupPointMethod& handle) :
	crMethod(handle)
{
}
void crJXJGainSuperEquipLvupPointMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_lvuppoint = LOCREPARAM(param64);
		}
		break;
	}
}
void crJXJGainSuperEquipLvupPointMethod::addParam(int i, const std::string& str)
{
}
void crJXJGainSuperEquipLvupPointMethod::operator()(crHandle &handle)
{
	crData *roleData = m_this->getDataClass();
	void *param;
	roleData->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
	unsigned char* lvuppoint = (unsigned char *)param;
	if (*lvuppoint < m_lvuppoint)
	{
		unsigned char delta = m_lvuppoint - *lvuppoint;
		*lvuppoint = m_lvuppoint;
		unsigned short gainpoint = 0;
		roleData->lock();
		roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
		SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
		if (superEquipZhulingData)
		{//神器激活
			crTableIO::StrVec record;
			ref_ptr<crTableIO> superEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipTab);
			int lvindex = superEquipTab->getTitleIndex("等级");
			int pointindex = superEquipTab->getTitleIndex("注灵点");
			roleData->getParam(WCHDATA_JXJSuperEquipDataVec, param);
			SuperEquipDataVec *superEquipDataVec = (SuperEquipDataVec *)param;
			for (SuperEquipDataVec::iterator itr = superEquipDataVec->begin();
				itr != superEquipDataVec->end();
				++itr)
			{
				gainpoint += (*itr)->getSuperEquipLevel() * delta;
			}
			superEquipZhulingData->addZhulingPoint(gainpoint);
		}
		roleData->unlock();

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(3);
		stream->_writeUChar(m_lvuppoint);
		stream->_writeUShort(gainpoint);
		////游戏日志
		std::string logdata = crArgumentParser::appVectoa(crVector2i(m_lvuppoint, gainpoint), '|');
		GameLogData gamelog(Log_GainSuperEquipLvupPoint, logdata);
		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(m_this->getPlayerID(), &gamelog));

		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
		if (playerData.valid())
		{
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJRecvGainSuperEquipLvupPoint, stream.get());
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvGainSuperEquipLvupPointMethod
//
/////////////////////////////////////////
crJXJRecvGainSuperEquipLvupPointMethod::crJXJRecvGainSuperEquipLvupPointMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvGainSuperEquipLvupPointMethod::crJXJRecvGainSuperEquipLvupPointMethod(const crJXJRecvGainSuperEquipLvupPointMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvGainSuperEquipLvupPointMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvGainSuperEquipLvupPointMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvGainSuperEquipLvupPointMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			if (m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				unsigned char lvuppoint = m_stream->_readUChar();
				unsigned short gainpoint = m_stream->_readUShort();
				crData *roleData = m_this->getMainRole()->getDataClass();
				void *param;
				roleData->getParam(WCHDATA_JXJSuperEquipZhulingData, param);
				SuperEquipZhulingData *superEquipZhulingData = (SuperEquipZhulingData *)param;
				roleData->getParam(WCHDATA_JXJSuperEquipLvupPoint, param);
				unsigned char *superEquipLvupPoint = (unsigned char *)param;
				*superEquipLvupPoint = lvuppoint;
				if (superEquipZhulingData)
					superEquipZhulingData->addZhulingPoint(gainpoint);

				int texid = 1150;
				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
				int mode = 1;
				noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
				noticeHandle->inputParam(WCHDATA_NoticeTextID, &texid);
				std::string param1 = crArgumentParser::appItoa(lvuppoint);
				std::string param2 = crArgumentParser::appItoa(gainpoint);
				noticeHandle->inputParam(WCHDATA_NoticeParam1, &param1);
				noticeHandle->inputParam(WCHDATA_NoticeParam2, &param2);
				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJUIClearTextMethod
//
/////////////////////////////////////////
crJXJUIClearTextMethod::crJXJUIClearTextMethod(){}
crJXJUIClearTextMethod::crJXJUIClearTextMethod(const crJXJUIClearTextMethod& handle) :
	crMethod(handle)
{
}
void crJXJUIClearTextMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJUIClearTextMethod::addParam(int i, const std::string& str)
{
	m_textVec.push_back(str);
}

void crJXJUIClearTextMethod::operator()(crHandle &handle)
{
	for (std::vector<std::string>::iterator itr = m_textVec.begin();
		 itr != m_textVec.end();
		++itr)
	{
		crStaticTextWidgetNode *text = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(*itr));
		if (text)
			text->clearString();
	}
}
/////////////////////////////////////////
//
//crJXJRecvSuperEquipCoinMethod
//
/////////////////////////////////////////
crJXJRecvSuperEquipCoinMethod::crJXJRecvSuperEquipCoinMethod() :
	m_this(NULL),
	m_coin(0)
{
}

crJXJRecvSuperEquipCoinMethod::crJXJRecvSuperEquipCoinMethod(const crJXJRecvSuperEquipCoinMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_coin(0)
{
}

void crJXJRecvSuperEquipCoinMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_coin = LOCREPARAM(param64);
		}
		else
		{
			m_coin = 0;
		}
		break;
	}
}

void crJXJRecvSuperEquipCoinMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvSuperEquipCoinMethod::operator()(crHandle &handle)
{
	if (m_coin != 0)
	{
		void *param;
		int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid())
		{
			crData *gameData = m_this->getDataClass();
			gameData->excHandle(MAKECREPARAM(WCH_LockData, 1));
			gameData->getParam(WCHDATA_JXJSuperEquipCoin, param);
			int *coin = (int *)param;
			INTLIMITADD(*coin, m_coin, INT_MAX)
			gameData->excHandle(MAKECREPARAM(WCH_LockData, 0));
			
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(4);
			stream->_writeInt(*coin);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJClientRecvSuperEquipCoin, stream.get());
			if (playerData.valid())
			{
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJClientRecvSuperEquipCoinMethod
//
/////////////////////////////////////////
crJXJClientRecvSuperEquipCoinMethod::crJXJClientRecvSuperEquipCoinMethod() :
	m_netType(GameClient_Game) {}
crJXJClientRecvSuperEquipCoinMethod::crJXJClientRecvSuperEquipCoinMethod(const crJXJClientRecvSuperEquipCoinMethod& handle) :
	crMethod(handle)
{
}
void crJXJClientRecvSuperEquipCoinMethod::inputParam(int i, void *param)
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

void crJXJClientRecvSuperEquipCoinMethod::addParam(int i, const std::string& str)
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

void crJXJClientRecvSuperEquipCoinMethod::operator()(crHandle &handle)
{
	if (m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			void *param;
			int coin = m_stream->_readInt();
			crData *data = m_this->getDataClass();
			if (data)
			{
				data->getParam(WCHDATA_JXJSuperEquipCoin, param);
				int extra = coin - (*(int *)param);
				data->inputParam(WCHDATA_JXJSuperEquipCoin, &coin);
				if (extra != 0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_SuperEquipCoin, extra));

				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				if(canvas.valid())
					canvas->doEvent(WCH_UI_UpdateData);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSuperEquipShopCanvasUpdateMethod
//
/////////////////////////////////////////
crJXJSuperEquipShopCanvasUpdateMethod::crJXJSuperEquipShopCanvasUpdateMethod() :
	m_this(NULL),
	m_init(false)
{

}

crJXJSuperEquipShopCanvasUpdateMethod::crJXJSuperEquipShopCanvasUpdateMethod(const crJXJSuperEquipShopCanvasUpdateMethod & handle) :
	crMethod(handle),
	m_init(false)
{
}

void crJXJSuperEquipShopCanvasUpdateMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJSuperEquipShopCanvasUpdateMethod::addParam(int i, const std::string& str)
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
		m_priceInput[i] = str;
		break;
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
		m_imageBox[i - 12] = str;
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
		m_countInput[i - 24] = str;
		break;
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
		m_goumaiBtn[i - 36] = str;
		break;
	case 48:
		m_scroll = str;
		break;
	case 49:
		m_coin = str;
		break;
	default:
		break;
	}
}
void crJXJSuperEquipShopCanvasUpdateMethod::init()
{
	for (int i =0; i<12; i++)
	{
		m_imageBoxWidget[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox[i]));
		m_priceInputWidget[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_priceInput[i]));
		m_countInputWidget[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_countInput[i]));
		m_gouMaiBtnWidget[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_goumaiBtn[i]));
	}
	m_coinTextWidget = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_coin));
	m_scrollWidget = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_scroll));
	m_init = true;
}
void crJXJSuperEquipShopCanvasUpdateMethod::operator()(crHandle &handle)
{
	if (!m_init)
	{
		init();
	}
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if (playerData)
	{
		crData *data = playerData->getDataClass();
		void *param;
		//商店
		data->getParam(WCHDATA_JXJSuperEquipCoin, param);
		int coin = *(int*)param;
		m_coinTextWidget->setString(crArgumentParser::appItoa(coin));

		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		int iconIdx = itemtab->getTitleIndex("icon");
		ref_ptr<crTableIO> shoptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipShopTab);
		int countindex = shoptab->getTitleIndex("数量");
		int priceindex = shoptab->getTitleIndex("价格");

		int rowcount = shoptab->getRowCount();
		int scroll = 0;
		if (rowcount > 12)
		{
			m_scrollWidget->setRange(0, rowcount - 12);
			m_scrollWidget->setLineValue(6);
			m_scrollWidget->setPageValue(12);
			m_scrollWidget->setEnable(true);
			scroll = m_scrollWidget->getValue();
		}
		else
		{
			m_scrollWidget->setEnable(false);
		}
		crTableIO::StrVec itemrecord;
		int itemid = 0;
		int rowid;
		for (int i = 0; i<12; i++)
		{
			itemid = 0;
			rowid = i + scroll;
			if (rowid < rowcount && itemtab->queryOneRecord(0, (*shoptab)(rowid, 0), itemrecord) >= 0)
			{
				m_imageBoxWidget[i]->setImageName(itemrecord[iconIdx]);
				itemid = atoi((*shoptab)(rowid, 0).c_str());
				m_gouMaiBtnWidget[i]->getDataClass()->inputParam(WCHDATA_JXJUIID, &itemid);
				m_imageBoxWidget[i]->getDataClass()->inputParam(WCHDATA_JXJUIID, &itemid);
				m_gouMaiBtnWidget[i]->setEnable(atoi((*shoptab)(rowid, priceindex).c_str()) <= coin);
				m_countInputWidget[i]->setString((*shoptab)(rowid, countindex));
				m_priceInputWidget[i]->setString((*shoptab)(rowid, priceindex));
				m_gouMaiBtnWidget[i]->setVisiable(true);
				m_imageBoxWidget[i]->setVisiable(true);
				m_countInputWidget[i]->setVisiable(true);
				m_priceInputWidget[i]->setVisiable(true);
			}
			else
			{
				m_gouMaiBtnWidget[i]->setVisiable(false);
				m_imageBoxWidget[i]->setVisiable(false);
				m_countInputWidget[i]->setVisiable(false);
				m_priceInputWidget[i]->setVisiable(false);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJSetSuperEquipShopRadioEnableMethod
//
/////////////////////////////////////////
crJXJSetSuperEquipShopRadioEnableMethod::crJXJSetSuperEquipShopRadioEnableMethod() :
	m_this(NULL),
	m_init(false)
{

}

crJXJSetSuperEquipShopRadioEnableMethod::crJXJSetSuperEquipShopRadioEnableMethod(const crJXJSetSuperEquipShopRadioEnableMethod & handle) :
	crMethod(handle),
	m_init(false)
{
}

void crJXJSetSuperEquipShopRadioEnableMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJSetSuperEquipShopRadioEnableMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_radio = str;
		break;
	default:
		break;
	}
}
void crJXJSetSuperEquipShopRadioEnableMethod::init()
{
	m_radioWidget = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_radio));
	m_init = true;
}
void crJXJSetSuperEquipShopRadioEnableMethod::operator()(crHandle &handle)
{
	if (!m_init)
	{
		init();
	}
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		crData *roleData = myPlayer->getMainRole()->getDataClass();
		if (roleData)
		{
			void *param;
			roleData->getParam(WCHDATA_Level, param);
			unsigned char characterlv = *(unsigned char *)param;
			int openlv = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJSuperEquipShopOpenLevel).c_str()));
			m_radioWidget->setEnable(characterlv >= openlv);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSuperEquipShopBuyMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSuperEquipShopBuyMethod::crJXJSuperEquipShopBuyMethod()
{}
crJXJSuperEquipShopBuyMethod::crJXJSuperEquipShopBuyMethod(const crJXJSuperEquipShopBuyMethod& handle)
{
}
void crJXJSuperEquipShopBuyMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJSuperEquipShopBuyMethod::addParam(int i, const std::string& str)
{
}

void crJXJSuperEquipShopBuyMethod::operator()(crHandle &handle)
{
	void *param;
	crData *data = m_this->getDataClass();
	data->getParam(WCHDATA_JXJUIID, param);
	int itemid = *(int *)param;
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(4);
	stream->_writeInt(itemid);

	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	crPlayerDataEventPacket packet;
	crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvSuperEquipShopBuy, stream.get());
	if (netConductor)
		netConductor->getNetManager()->sendPacket("all", packet);
}

//////////////////////////////////////////
//
//crJXJRecvSuperEquipShopBuyMethod
//
//////////////////////////////////////////
crJXJRecvSuperEquipShopBuyMethod::crJXJRecvSuperEquipShopBuyMethod() {}
crJXJRecvSuperEquipShopBuyMethod::crJXJRecvSuperEquipShopBuyMethod(const crJXJRecvSuperEquipShopBuyMethod &handle) :
	crMethod(handle)
{

}
void crJXJRecvSuperEquipShopBuyMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvSuperEquipShopBuyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvSuperEquipShopBuyMethod::operator ()(crHandle &handle)
{
	if (m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			void *param;
			int price = 0;
			unsigned char count;
			int itemid = m_stream->_readInt();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if (playerData.valid())
			{
				bool success = 0;
				ref_ptr<crTableIO> superEquipShop = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJSuperEquipShopTab);
				int countindex = superEquipShop->getTitleIndex("数量");
				int priceindex = superEquipShop->getTitleIndex("价格");
				crTableIO::StrVec record;
				if (superEquipShop->queryOneRecord(0, crArgumentParser::appItoa(itemid), record) >= 0)
				{
					price = atoi(record[priceindex].c_str());
					count = atoi(record[countindex].c_str());

					crData *data = m_this->getDataClass();
					data->excHandle(MAKECREPARAM(WCH_LockData, 1));
					data->getParam(WCHDATA_JXJSuperEquipCoin, param);
					int *coin = (int*)param;

					if (*coin >= price)
					{//成功
						success = 1;
						ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
						CRCore::ref_ptr<crBagItemData> itemPtr = new crBagItemData;
						itemPtr->setItemID(itemid);
						itemPtr->setEquipMagic(0);
						itemPtr->setItemCount(count);
						reward_info->getRewardItemsVec().push_back(itemPtr);
						reward_info->setType(GP_RewardItem);
						m_this->doEvent(WCH_JXJRecvPlayerRewardInfo, MAKECREPARAM(reward_info.get(), NULL));

						*coin -= price;
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(5);
						stream->_writeChar(success);
						stream->_writeInt(*coin);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvSuperEquipShopBuy, stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);

						std::string str = "神器商店购买,花费神器币";
						str += crArgumentParser::appItoa(price);
						str += "获得";
						str += crArgumentParser::appVectoa(crVector2i(itemid, count), '|');
						GameLogData gamelog(Log_SuperEquipShopBuy, str);
						crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
					}
					else
						success = -1;//神器币不足
					data->excHandle(MAKECREPARAM(WCH_LockData, 0));
				}
				else
				{//商品已经下架
					success = -2;
				}
				if(success!=1)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeUChar(success);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvSuperEquipShopBuy, stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				}
			}
		}
		else if (m_netType == GameClient_Game)
		{
			void *param;
			unsigned char succes = m_stream->_readChar();
			if (succes == 1)
			{
				crData *playerData = m_this->getDataClass();
				playerData->excHandle(MAKECREPARAM(WCH_LockData, 1));
				playerData->getParam(WCHDATA_JXJSuperEquipCoin, param);
				int *coin = (int*)param;
				*coin = m_stream->_readInt();
				playerData->excHandle(MAKECREPARAM(WCH_LockData, 0));
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
				canvas->doEvent(WCH_UI_UpdateData);
			}
			else if(succes == -1)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1151, NULL));
			}
			else if (succes == -2)
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1152, NULL));
			}
			else
			{
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(9134, NULL));
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJAddTaskInfoMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJAddTaskInfoMethod::crJXJAddTaskInfoMethod()
{}
crJXJAddTaskInfoMethod::crJXJAddTaskInfoMethod(const crJXJAddTaskInfoMethod& handle):
	m_canvasName(handle.m_canvasName),
	m_taskVec(handle.m_taskVec),
	m_texid(handle.m_texid)
{
}
void crJXJAddTaskInfoMethod::inputParam(int i, void *param)
{
}
void crJXJAddTaskInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		crArgumentParser::appAtoVec(str, m_taskVec);
		break;
	case 2:
		m_texid = atoi(str.c_str());
		break;
	}
}

void crJXJAddTaskInfoMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
	if (canvas.valid())
	{
		void *param;
		crRole *mainRole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		crData *myMetierData = mainRole->getMetierDataClass();
		myMetierData->excHandle(MAKECREPARAM(WCH_LockData, 1));
		myMetierData->getParam(WCHDATA_GameTaskMap, param);
		GameTaskMap *gameTaskMap = (GameTaskMap *)param;
		if (!gameTaskMap || gameTaskMap->empty())
		{
			myMetierData->excHandle(MAKECREPARAM(WCH_LockData, 0));
			return;
		}
		ref_ptr<crData> canvasdata = canvas->getDataClass();
		bool changed = false;
		if (canvasdata.valid())
		{
			crGameTask* gameTask;
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 1));
			canvasdata->getParam(WCHDATA_JXJBattleBrocastqeque, param);
			std::deque<std::string> * tonggaoqueue = (std::deque<std::string> *)param;
			int prog = 0;
			int full = 0;
			crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
			noticeHandle->inputParam(WCHDATA_NoticeTextID, &m_texid);
			int mode = 2;
			noticeHandle->inputParam(WCHDATA_NoticeMode, &mode);
			for (std::vector<int>::iterator itr = m_taskVec.begin();
				itr != m_taskVec.end();
				++itr)
			{
				if (gameTaskMap->find(*itr) != gameTaskMap->end())
				{
					gameTask = (*gameTaskMap)[*itr].get();
					if (gameTask && gameTask->getActivation() && !gameTask->isComplete())
					{
						prog = gameTask->getTaskProgress();
						full = gameTask->getTaskSteps() - 1;
						noticeHandle->inputParam(WCHDATA_NoticeParam1, &(crArgumentParser::appItoa(prog)));
						noticeHandle->inputParam(WCHDATA_NoticeParam2, &(crArgumentParser::appItoa(full)));
						std::string output;
						noticeHandle->inputParam(WCHDATA_NoticeOutPut, &output);
						crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);

						if (tonggaoqueue->size() >= 5)
						{
							tonggaoqueue->pop_front();
						}
						tonggaoqueue->push_back(output);
						changed = true;
					}
				}
			}
			canvasdata->excHandle(MAKECREPARAM(WCH_LockData, 0));
		}
		myMetierData->excHandle(MAKECREPARAM(WCH_LockData, 0));
		if (changed)
		{
			crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCheckTaskValueChangeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCheckTaskValueChangeMethod::crJXJCheckTaskValueChangeMethod() :
	m_this(NULL),
	m_WCHDATA_ID(0),
	m_dataType(0),
	m_lastdata(-1)
{

}

crJXJCheckTaskValueChangeMethod::crJXJCheckTaskValueChangeMethod(const crJXJCheckTaskValueChangeMethod & handle) :
	crMethod(handle),
	m_WCHDATA_ID(handle.m_WCHDATA_ID),
	m_dataType(handle.m_dataType),
	m_lastdata(-1)
{

}

void crJXJCheckTaskValueChangeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crGameTask *)param;
		break;
	}
}

void crJXJCheckTaskValueChangeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_WCHDATA_ID = atoi(str.c_str());
		break;
	case 1:
		m_dataType = atoi(str.c_str());
		break;
	case 2:
		m_dataStruct = (JXJDataStruct)atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJCheckTaskValueChangeMethod::operator()(crHandle &handle)
{//WCHDATA_JXJPlayerDeadCount
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	do
	{
		if (!myPlayer || myPlayer->getDataClass() == NULL
			|| myPlayer->getMainRole() == NULL) break;

		if(!m_this->getActivation() || m_this->isComplete() || m_this->isTaskCanRefer())
			break;
		crData * playerGameData = myPlayer->getDataClass();

		crData * data = playerGameData;
		if (JXJDataStruct_mainRoleData == m_dataStruct)
		{
			data = myPlayer->getMainRole()->getDataClass();
		}
		if (!data)
		{
			break;
		}

		void *param = NULL;
		int valueData = -1;
		playerGameData->getParam(m_WCHDATA_ID, param);
		switch (m_dataType)
		{
		case JXJDataType_Int:
			valueData = *(int *)param;
			break;
		case JXJDataType_UInt:
			valueData = *(unsigned int *)param;
			break;
		case JXJDataType_Short:
			valueData = *(short *)param;
			break;
		case JXJDataType_Char:
			valueData = *(char *)param;
			break;
		case JXJDataType_UChar:
			valueData = *(unsigned char *)param;
			break;
		default:
			break;
		}
		if (-1 == valueData) break;

		if (m_lastdata == -1)
		{
			m_lastdata = valueData;
		}
		else/* if(!m_this->isCanAdvance())*/
		{
			int delta = valueData - m_lastdata;
			if (delta>0)//不计消耗
			{
				int progress = delta + m_this->getTaskProgress();
				if (progress >= m_this->getTaskSteps())
					progress = m_this->getTaskSteps() - 1;

				if (progress != m_this->getTaskProgress())
				{
					m_this->setTaskProgress(progress);
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					if (netConductor)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(8);
						stream->_writeInt(m_this->getTaskID());
						stream->_writeInt(progress);
						crJXJServerMetierEventPacket packet;
						crJXJServerMetierEventPacket::buildRequestPacket(packet, WCH_RecvTaskProgress, stream.get());
						netConductor->getNetManager()->sendPacket("all", packet);
					}
				}
			}
			m_lastdata = valueData;
			//if(m_this->isCanAdvance())
			//{
			//	m_lastdata += m_this->getStepCount();
			//}
		}
	} while (0);
}
/////////////////////////////////////////
//
//crJXJRecvTaskProgressMethod
//
/////////////////////////////////////////
crJXJRecvTaskProgressMethod::crJXJRecvTaskProgressMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvTaskProgressMethod::crJXJRecvTaskProgressMethod(const crJXJRecvTaskProgressMethod& handle) :
	crMethod(handle),
	m_netType(GameClient_Game)
{
}
void crJXJRecvTaskProgressMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if (param)
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
void crJXJRecvTaskProgressMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvTaskProgressMethod::operator()(crHandle &handle)
{
	if (m_stream.valid() && m_this->getMetierDataClass())
	{
		int taskid = m_stream->_readInt();
		int progress = m_stream->_readInt();
		void *param;
		crData *metierData = m_this->getMetierDataClass();
		ref_ptr<crGameTask> gameTask;
		metierData->getParam(WCHDATA_GameTaskMap, param);
		metierData->excHandle(MAKECREPARAM(WCH_LockData, 1));
		GameTaskMap* gameTaskMap = (GameTaskMap*)param;
		GameTaskMap::iterator itr = gameTaskMap->find(taskid);
		gameTask = itr != gameTaskMap->end() ? itr->second.get() : NULL;
		if (gameTask.valid())
		{
			if (m_netType == GameServer)
			{
				if (gameTask->getActivation() && !gameTask->isComplete() && !gameTask->isTaskCanRefer())
				{
					if (progress >= gameTask->getTaskSteps())
						progress = gameTask->getTaskSteps() - 1;
					gameTask->setTaskProgress(progress);

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(8);
					stream->_writeInt(taskid);
					stream->_writeInt(gameTask->getTaskProgress());
					int playerid = m_this->getPlayerID();
					crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
					if (netConductor)
					{//server to client
						crJXJPlayerMetierEventPacket packet;
						crJXJPlayerMetierEventPacket::buildReplyPacket(packet, playerid, WCH_RecvTaskProgress, stream.get());
						ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(playerid));
						if (playerData.valid())
							netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);

						//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
						//if(dbConductor)
						//{//任务日志
						//	crNetDataManager *netDataManager = netConductor->getNetDataManager();
						//	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
						//	int serverid = callback->getServerID();
						//	crDBSavePacket dbpacket;
						//	ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;
						//	insertTaskrecord->buildUpdateSql(playerid,serverid,m_this->getIName(),taskid,TS_Progress,gameTask->getTaskProgress());
						//	crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
						//	dbConductor->getNetManager()->sendPacket("all",dbpacket);
						//}
					}
				}
			}
			else if (m_netType == GameClient_Game)
			{
				gameTask->setTaskProgress(progress);
				crCanvasNode *canvas = crFilterRenderManager::getInstance()->findCanvas("UI_ManorStair_RenWu");
				if (canvas)
				{
					canvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
		metierData->excHandle(MAKECREPARAM(WCH_LockData, 0));
	}
}
/////////////////////////////////////////
//
//crJXJAddTaskProgressMethod
//
/////////////////////////////////////////
crJXJAddTaskProgressMethod::crJXJAddTaskProgressMethod() :
	m_taskid(0),
	m_addProgress(0)
{
}
crJXJAddTaskProgressMethod::crJXJAddTaskProgressMethod(const crJXJAddTaskProgressMethod& handle) :
	crMethod(handle),
	m_taskid(0),
	m_addProgress(0)
{
}
void crJXJAddTaskProgressMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_taskid = LOCREPARAM(param64);
			m_addProgress = HICREPARAM(param64);
		}
		break;
	}
}
void crJXJAddTaskProgressMethod::addParam(int i, const std::string& str)
{
}
void crJXJAddTaskProgressMethod::operator()(crHandle &handle)
{
	void *param;
	crData *metierData;
	crRole *mainRole = m_this->getMainRole();
	if (mainRole && mainRole->getMetierDataClass())
	{
		metierData = mainRole->getMetierDataClass();
		//if(crGlobalHandle::isClient())
		//{
		//	crRole *mainrole = crMyPlayerData::getInstance()->getPlayerGameData()->getMainRole();
		//	metierData = mainrole->getMetierDataClass();
		//}
		//else
		//{
		//	metierData = m_this->getMetierDataClass();
		//}
		int taskid = m_taskid;
		if (taskid == 0)
		{
			metierData->getParam(WCHDATA_CurrentTaskID, param);
			taskid = *(int *)param;
		}
		ref_ptr<crGameTask> gameTask;
		metierData->getParam(WCHDATA_GameTaskMap, param);
		metierData->excHandle(MAKECREPARAM(WCH_LockData, 1));
		GameTaskMap* gameTaskMap = (GameTaskMap*)param;
		GameTaskMap::iterator itr = gameTaskMap->find(taskid);
		gameTask = itr != gameTaskMap->end() ? itr->second.get() : NULL;
		//CRCore::notify(CRCore::ALWAYS) << "crJXJAdvanceTaskMethod: taskid=" << taskid<< std::endl;
		metierData->excHandle(MAKECREPARAM(WCH_LockData, 0));
		if (gameTask.valid() && gameTask->getActivation() && !gameTask->isComplete() && !gameTask->isTaskCanRefer())
		{
			int progress = gameTask->getTaskProgress() + m_addProgress;
			if (progress >= gameTask->getTaskSteps())
				progress = gameTask->getTaskSteps() - 1;
			gameTask->setTaskProgress(progress);

			//CRCore::notify(CRCore::ALWAYS) << "crJXJAdvanceTaskMethod: TaskProgress=" << gameTask->getTaskProgress() << std::endl;
			crNetConductor *netConductor;
			if (crGlobalHandle::isClient())
			{
				netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if (netConductor)
				{//client to server
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(8);
					stream->_writeInt(taskid);
					stream->_writeInt(gameTask->getTaskProgress());
					crJXJServerMetierEventPacket packet;
					crJXJServerMetierEventPacket::buildRequestPacket(packet, WCH_RecvTaskProgress, stream.get());
					netConductor->getNetManager()->sendPacket("all", packet);
				}
			}
			else
			{
				netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
				if (netConductor)
				{//server to client
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(8);
					stream->_writeInt(taskid);
					stream->_writeInt(gameTask->getTaskProgress());
					int playerid = m_this->getPlayerID();
					crJXJPlayerMetierEventPacket packet;
					crJXJPlayerMetierEventPacket::buildReplyPacket(packet, playerid, WCH_RecvTaskProgress, stream.get());
					ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(playerid));
					if (playerData.valid())
						netConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);

						//crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
						//if(dbConductor)
						//{//任务日志
						//	crNetDataManager *netDataManager = netConductor->getNetDataManager();
						//	crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
						//	int serverid = callback->getServerID();
						//	crDBSavePacket dbpacket;
						//	ref_ptr<crJXJInsertTaskrecord> insertTaskrecord = new crJXJInsertTaskrecord;
						//	insertTaskrecord->buildUpdateSql(playerid,serverid,mainRole->getIName(),m_taskid,TS_Progress,gameTask->getTaskProgress());
						//	crDBSavePacket::buildRequestPacket(dbpacket,GameGlobalDB,insertTaskrecord.get());
						//	dbConductor->getNetManager()->sendPacket("all",dbpacket);
						//}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJIsCanBuyFubenMethod
//
/////////////////////////////////////////
crJXJIsCanBuyFubenMethod::crJXJIsCanBuyFubenMethod()
{}
crJXJIsCanBuyFubenMethod::crJXJIsCanBuyFubenMethod(const crJXJIsCanBuyFubenMethod& handle) :
	crMethod(handle)
{
}
void crJXJIsCanBuyFubenMethod::inputParam(int i, void *param)
{
}

void crJXJIsCanBuyFubenMethod::addParam(int i, const std::string& str)
{
}

void crJXJIsCanBuyFubenMethod::operator()(crHandle &handle)
{
	bool canBuy = false;
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *data = myPlayer->getDataClass();
	void *param;
	data->getParam(WCHDATA_JXJPlayerCycleCount, param);
	short cyclecount = *(short *)param;
	if (cyclecount > 0)
	{
		data->getParam(WCHDATA_JXJFubenOrChengchiType, param);
		unsigned char type = *(unsigned char *)param;
		data->getParam(WCHDATA_JXJFubenOrChengchiID, param);
		unsigned short fubenid = *(unsigned short *)param;
		data->lock();
		data->getParam(WCHDATA_JXJFubenProgressMap, param);
		FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
		FubenProgressMap::iterator itr = fubenprogressmap->find(fubenid);
		if (itr == fubenprogressmap->end() || itr->second->getComplete() == 0)
		{
			ref_ptr<crTableIO>fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			crTableIO::StrVec record;
			if (type == FT_Fuben && fubenid >=1000 && fubenid<2000)
			{
				canBuy = true;
			}
		}
		data->unlock();
	}
	handle.outputParam(0, &canBuy);
}
/////////////////////////////////////////
//
//crJXJBuyFubenMsgBoxMethod
//
/////////////////////////////////////////
crJXJBuyFubenMsgBoxMethod::crJXJBuyFubenMsgBoxMethod()
{
}
crJXJBuyFubenMsgBoxMethod::crJXJBuyFubenMsgBoxMethod(const crJXJBuyFubenMsgBoxMethod& handle) :
	crMethod(handle),
	m_msgbox(handle.m_msgbox),
	m_msgtext(handle.m_msgtext)
{
}
void crJXJBuyFubenMsgBoxMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyFubenMsgBoxMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_msgbox = str;
		break;
	case 1:
		m_msgtext = str;
		break;
	}
}
void crJXJBuyFubenMsgBoxMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgbox);
	if (msgCanvas.valid() && myPlayer && netConductor)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJVipLv, param);
		unsigned char vipLevel = *(unsigned char *)param;
		int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuyFubenGold, vipLevel).c_str());
		int checkmoney = needgold;
		myPlayer->doEvent(WCH_JXJCheckLijinGold, MAKECREPARAM(&checkmoney, NULL));
		if (checkmoney == 0)
		{
			ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
			crTableIO::StrVec record;
			std::string str1,str2;
			if (textTab->queryOneRecord(0, crArgumentParser::appItoa(1155), record) >= 0)
			{
				str1 = record[1];
			}
			if (textTab->queryOneRecord(0, crArgumentParser::appItoa(1156), record) >= 0)
			{
				str2 = record[1];
			}
			crHypertextWidgetNode *msgtext = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_msgtext));
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
			cfg_script.Add("Text", str1);
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content", 2);
			v_i.clear();
			v_i.push_back(255.0f);
			v_i.push_back(255.0f);
			v_i.push_back(0.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color", v_i);
			char tmpText[64];
			sprintf(tmpText, "%d礼金/元宝\0", needgold);
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
			cfg_script.Add("Text", str2);
			cfg_script.Pop();

			cfg_script.Pop();

			if (msgtext)msgtext->setHypertext(cfg_script);
			crData *dlgData = msgCanvas->getDataClass();
			int commandtype = CDP_Widget;
			dlgData->inputParam(WCHDATA_CommandDlgParentType, &commandtype);
			dlgData->inputParam(WCHDATA_CommandDlgParent, m_this);
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{//元宝不足
			m_this->doEvent(WCH_CANCEL);
		}
	}
}
/////////////////////////////////////////
//
//crJXJBuyFubenMethod
//
/////////////////////////////////////////
crJXJBuyFubenMethod::crJXJBuyFubenMethod() :
	m_this(NULL)
{
}
crJXJBuyFubenMethod::crJXJBuyFubenMethod(const crJXJBuyFubenMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJBuyFubenMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crWidgetNode*)param;
		break;
	}
}
void crJXJBuyFubenMethod::addParam(int i, const std::string& str)
{
}
void crJXJBuyFubenMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (netConductor)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		crData *data = myPlayer->getDataClass();
		void *param;
		data->getParam(WCHDATA_JXJFubenOrChengchiID, param);
		unsigned short fubenid = *(unsigned short *)param;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeUShort(fubenid);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet, WCH_JXJRecvBuyFuben, stream.get());
		netConductor->getNetManager()->sendPacket("all", packet);
	}
}
/////////////////////////////////////////
//
//crJXJRecvBuyFubenMethod
//
/////////////////////////////////////////
crJXJRecvBuyFubenMethod::crJXJRecvBuyFubenMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvBuyFubenMethod::crJXJRecvBuyFubenMethod(const crJXJRecvBuyFubenMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvBuyFubenMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvBuyFubenMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvBuyFubenMethod::operator()(crHandle &handle)
{
	if (m_this && m_stream.valid())
	{
		if (m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			char success = 0;
			//返回信息
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if (playerData.valid() && m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				unsigned short fubenid = m_stream->_readUShort();
				void *param;
				crData *data = m_this->getDataClass();

				data->getParam(WCHDATA_JXJPassZhengZhanTianXiaCount, param);
				unsigned short passcount = *(unsigned short *)param;

				data->lock();
				data->getParam(WCHDATA_JXJFubenProgressMap, param);
				FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
				FubenProgressMap::iterator itr = fubenprogressmap->find(fubenid);
				if (itr != fubenprogressmap->end() && itr->second->getComplete()>0)
				{
					success = -1;//该副本已经通过，不能购买
				}
				else
				{
					data->getParam(WCHDATA_JXJVipLv, param);
					unsigned char vipLevel = *(unsigned char *)param;
					int needgold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBuyFubenGold, vipLevel).c_str());
					MoneyChangeData moneydata(needgold, "购买副本");
					bool needdeductgold = false;
					m_this->doEvent(WCH_JXJDeductLijinGold, MAKECREPARAM(&moneydata, &needdeductgold));
					if (moneydata.first == 0)
					{
						if (fubenid >= 1000 && fubenid<2000)
						{
							//征战天下活动
							m_this->doEvent(WCH_JXJUpdate7DayHuoDong);

							//通过征战天下次数记录
							passcount += 1;
							data->inputParam(WCHDATA_JXJPassZhengZhanTianXiaCount, &passcount);
						}

						ref_ptr<crFubenInfo> fubeninfo = new crFubenInfo;
						fubeninfo->setComplete(1);
						fubeninfo->setStarCount(3);
						(*fubenprogressmap)[fubenid] = fubeninfo;

						ref_ptr<crTableIO> fubenRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
						crTableIO::DataVec recordVec;
						fubenRewardTab->queryRecords(0, crArgumentParser::appItoa(fubenid), recordVec);
						if (!recordVec.empty())
						{
							int copperIndex = fubenRewardTab->getTitleIndex("铜钱");
							int expIndex = fubenRewardTab->getTitleIndex("经验");
							int gongxunIndex = fubenRewardTab->getTitleIndex("功勋");
							int giftgoldIndex = fubenRewardTab->getTitleIndex("礼券");
							ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
							if (copperIndex >= 0)
								reward_info->setCoopercash(atoi(recordVec[1][copperIndex].c_str()) +
									atoi(recordVec[2][copperIndex].c_str()) + atoi(recordVec[3][copperIndex].c_str()));
							if (expIndex >= 0)
								reward_info->setExperience(atoi(recordVec[1][expIndex].c_str()) +
									atoi(recordVec[2][expIndex].c_str()) + atoi(recordVec[3][expIndex].c_str()));
							if (gongxunIndex >= 0)
								reward_info->setExploit(atoi(recordVec[1][gongxunIndex].c_str()) +
									atoi(recordVec[2][gongxunIndex].c_str()) + atoi(recordVec[3][gongxunIndex].c_str()));
							if (giftgoldIndex >= 0)
								reward_info->setGiftgold(atoi(recordVec[1][giftgoldIndex].c_str()) +
									atoi(recordVec[2][giftgoldIndex].c_str()) + atoi(recordVec[3][giftgoldIndex].c_str()));
							reward_info->setPlayerID(playerid);
							reward_info->setType(GP_BuyFuben);
							m_this->doEvent(WCH_JXJRecvPlayerRewardInfo, MAKECREPARAM(reward_info.get(), NULL));
						}
						if (needdeductgold)
						{
							crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
							callback->savePlayerData(playerData.get());
						}
						success = 1;
					}
					else
					{
						success = -2;//元宝不够
					}
				}
				data->unlock();

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5);
				stream->_writeChar(success);
				if (success == 1)
				{
					stream->_writeUShort(fubenid);
					stream->_writeUShort(passcount);
					//游戏日志
					std::string logdata = crArgumentParser::appItoa(fubenid);
					GameLogData gamelog(Log_BuyFuben, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvBuyFuben, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
			}
		}
		else if (m_netType == GameClient_Game)
		{
			char success = m_stream->_readChar();
			switch (success)
			{
			case 0:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2029, NULL));//操作失败
				break;
			case -1:
				//该副本已经通过，不能购买
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(1154, NULL));
				break;
			case -2:
				m_this->doEvent(WCH_JXJUIShowTipsCanvas, MAKECREPARAM(2036, NULL));//元宝不够
				break;
			case 1:
				{
					unsigned short fubenid = m_stream->_readUShort();
					unsigned short passcount = m_stream->_readUShort();
					crData *data = m_this->getDataClass();
					void *param;
					data->lock();
					data->inputParam(WCHDATA_JXJPassZhengZhanTianXiaCount, &passcount);
					data->getParam(WCHDATA_JXJFubenProgressMap, param);
					FubenProgressMap *fubenprogressmap = (FubenProgressMap *)param;
					ref_ptr<crFubenInfo> fubeninfo = new crFubenInfo;
					fubeninfo->setComplete(1);
					fubeninfo->setStarCount(3);
					(*fubenprogressmap)[fubenid] = fubeninfo;
					data->unlock();
					ref_ptr<crTableIO> techtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTechlvTab);
					int qianzhifubenindex = techtab->getTitleIndex("前置副本");
					crTableIO::StrVec techrecord;
					if (techtab->queryOneRecord(qianzhifubenindex, crArgumentParser::appItoa(fubenid), techrecord) >= 0)
					{//是否开启新科技
						unsigned char btnnew = 1;
						data->inputParam(WCHDATA_JXJIfTechBtnNew, &btnnew);
					}
				}
				break;
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRefineDiscountTimerMethod
//
/////////////////////////////////////////
crJXJRefineDiscountTimerMethod::crJXJRefineDiscountTimerMethod() :
	m_dt(0)
{
}
crJXJRefineDiscountTimerMethod::crJXJRefineDiscountTimerMethod(const crJXJRefineDiscountTimerMethod& handle) :
	crMethod(handle),
	m_dt(handle.m_dt)
{
}
void crJXJRefineDiscountTimerMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *((int*)(LOCREPARAM(param64)));
		}
		else
		{
			m_dt = 0;
		}
		break;
	}
}
void crJXJRefineDiscountTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJRefineDiscountTimerMethod::operator()(crHandle &handle)
{
	void *param;
	crData *thisData = m_this->getDataClass();
	thisData->excHandle(MAKECREPARAM(WCH_LockData, 1));
	thisData->getParam(WCHDATA_JXJRefineDiscountTimer, param);
	int *timer = (int *)param;
	if (*timer > 0)
	{
		*timer -= m_dt;
		if (*timer < 0)
		{
			*timer = 0;
		}
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(4);
		stream->_writeInt(*timer);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJRecvRefineDiscountTimer, stream.get());
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
		if (playerData.valid()) gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
	}
	thisData->excHandle(MAKECREPARAM(WCH_LockData, 0));
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvRefineDiscountTimerMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJRecvRefineDiscountTimerMethod::crJXJRecvRefineDiscountTimerMethod() {}
crJXJRecvRefineDiscountTimerMethod::crJXJRecvRefineDiscountTimerMethod(const crJXJRecvRefineDiscountTimerMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvRefineDiscountTimerMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvRefineDiscountTimerMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvRefineDiscountTimerMethod::operator()(crHandle &handle)
{
	if (m_stream.valid())
	{
		void *param;
		crTableIO::StrVec record;
		if (m_netType == GameClient_Game)
		{
			crMyPlayerData *me = crMyPlayerData::getInstance();
			if (me && me->getPlayerGameData())
			{
				crData *playerData = me->getPlayerGameData()->getDataClass();
				playerData->excHandle(MAKECREPARAM(WCH_LockData, 1));
				playerData->getParam(WCHDATA_JXJRefineDiscountTimer, param);
				int *timer = (int *)param;
				*timer = m_stream->_readInt();
				playerData->excHandle(MAKECREPARAM(WCH_LockData, 0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJServerUseRefineDiscountMethod
//
/////////////////////////////////////////
crJXJServerUseRefineDiscountMethod::crJXJServerUseRefineDiscountMethod() :
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL) {}
crJXJServerUseRefineDiscountMethod::crJXJServerUseRefineDiscountMethod(const crJXJServerUseRefineDiscountMethod& handle) :
	crMethod(handle),
	m_itemid(0),
	m_selectid(0),
	m_useResult(NULL),
	m_this(NULL),
	m_user(NULL)
{
}
void crJXJServerUseRefineDiscountMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crDataObject*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_user = (crPlayerGameData*)LOCREPARAM(param64);
			CRCore::crVector3i vparam = *(crVector3i *)HICREPARAM(param64);
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

void crJXJServerUseRefineDiscountMethod::addParam(int i, const std::string& str)
{
}

void crJXJServerUseRefineDiscountMethod::operator()(crHandle &handle)
{
	if (m_user && m_itemid > 0)
	{
		int playerid = m_user->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getMainRole() && playerData->getPlayerGameData()->getMainRole()->getDataClass())
		{
			ref_ptr<crTableIO>refineDiscountTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJRefineDiscountTab);
			crTableIO::StrVec record;
			if (refineDiscountTab->queryOneRecord(0, crArgumentParser::appItoa(m_itemid), record) >= 0)
			{
				int timeindex = refineDiscountTab->getTitleIndex("time");
				int discountindex = refineDiscountTab->getTitleIndex("discount");
				int time = atoi(record[timeindex].c_str());
				unsigned char discount = (unsigned char)atoi(record[discountindex].c_str());

				crData *data = playerData->getPlayerGameData()->getDataClass();
				void *param;
				data->getParam(WCHDATA_JXJRefineDiscount, param);
				unsigned char curDiscount = *(unsigned char*)param;
				if (curDiscount == discount)
				{
					data->getParam(WCHDATA_JXJRefineDiscountTimer, param);
					time += *(int*)param;
				}
				else
				{
					data->inputParam(WCHDATA_JXJRefineDiscount, &discount);
				}
				data->inputParam(WCHDATA_JXJRefineDiscountTimer, &time);

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(5);
				stream->_writeInt(time);
				stream->_writeUChar(discount);

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvRefineDiscount, stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				////游戏日志
				*m_useResult = 1;
				std::string logdata = "使用精炼打折道具：" + crArgumentParser::appItoa(m_itemid) + "获得:" + crArgumentParser::appVectoa(crVector2i(time, discount));
				GameLogData gamelog(Log_UseRefineDiscount, logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvUseRefineDiscountMethod
//
/////////////////////////////////////////
crJXJRecvUseRefineDiscountMethod::crJXJRecvUseRefineDiscountMethod() :
	m_netType(GameClient_Game) {}
crJXJRecvUseRefineDiscountMethod::crJXJRecvUseRefineDiscountMethod(const crJXJRecvUseRefineDiscountMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvUseRefineDiscountMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvUseRefineDiscountMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvUseRefineDiscountMethod::operator()(crHandle &handle)
{
	if (m_stream.valid())
	{
		if (m_netType == GameClient_Game)
		{
			int time = m_stream->_readInt();
			unsigned char discount = m_stream->_readUChar();
			crData *data = m_this->getDataClass();
			data->inputParam(WCHDATA_JXJRefineDiscount, &discount);
			data->inputParam(WCHDATA_JXJRefineDiscountTimer, &time);
		}
	}
}
/////////////////////////////////////////
//
//crJXJSearchVipLevelMethod
//
/////////////////////////////////////////
crJXJSearchVipLevelMethod::crJXJSearchVipLevelMethod() :
	m_this(NULL)
{
}
crJXJSearchVipLevelMethod::crJXJSearchVipLevelMethod(const crJXJSearchVipLevelMethod& handle) :
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSearchVipLevelMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	}
}
void crJXJSearchVipLevelMethod::addParam(int i, const std::string& str)
{
}
void crJXJSearchVipLevelMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if (myPlayer)
	{
		void *param;
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		crData *data = playerGameData->getDataClass();
		data->getParam(WCHDATA_VipLv, param);
		unsigned char nowlv = *(unsigned char *)param;
		m_this->setStep(nowlv);
	}
}

/////////////////////////////////////////
//
//crJXJSearchVipLevelCompleteCheckMethod
//
/////////////////////////////////////////
crJXJSearchVipLevelCompleteCheckMethod::crJXJSearchVipLevelCompleteCheckMethod() :
	m_this(NULL),
	m_player(NULL)
{
}
crJXJSearchVipLevelCompleteCheckMethod::crJXJSearchVipLevelCompleteCheckMethod(const crJXJSearchVipLevelCompleteCheckMethod& handle) :
	crMethod(handle),
	m_this(NULL),
	m_player(NULL)
{
}
void crJXJSearchVipLevelCompleteCheckMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crJXJAccomplish*)param;
		break;
	case 2:
		if (param)
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
void crJXJSearchVipLevelCompleteCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJSearchVipLevelCompleteCheckMethod::operator()(crHandle &handle)
{
	if (m_player)
	{
		void *param;
		crData *data = m_player->getDataClass();
		data->getParam(WCHDATA_VipLv, param);
		unsigned char nowlv = *(unsigned char *)param;
		m_this->setStep(nowlv);
	}
}
/////////////////////////////////////////
//
//crJXJGetCureEffectPercentMethod
//
/////////////////////////////////////////
crJXJGetCureEffectPercentMethod::crJXJGetCureEffectPercentMethod() :
	m_output(NULL) {}
crJXJGetCureEffectPercentMethod::crJXJGetCureEffectPercentMethod(const crJXJGetCureEffectPercentMethod& handle) :
	crMethod(handle),
	m_output(NULL)
{
}
void crJXJGetCureEffectPercentMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_output = (float *)(LOCREPARAM(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}
void crJXJGetCureEffectPercentMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetCureEffectPercentMethod::operator()(crHandle &handle)
{
	if (m_output)
	{
		crData *data = m_this->getDataClass();
		void *param;
		data->getParam(WCHDATA_ExtraCureEffectPercent_RD, param);
		short extra = *((short*)param);

		data->getParam(WCHDATA_InbornCureEffectPercent, param);
		short inborn = *((short*)param);

		*m_output = (float)(extra + inborn/*+equipSum*/) * 0.001f;//千分比
	}
}
/////////////////////////////////////////
//
//crJXJMoreOpenDetectMethod
//
/////////////////////////////////////////
crJXJMoreOpenDetectMethod::crJXJMoreOpenDetectMethod()
{
}
crJXJMoreOpenDetectMethod::crJXJMoreOpenDetectMethod(const crJXJMoreOpenDetectMethod& handle) :
	crMethod(handle)
{
}
void crJXJMoreOpenDetectMethod::inputParam(int i, void *param)
{
}
void crJXJMoreOpenDetectMethod::addParam(int i, const std::string& str)
{
}
void crJXJMoreOpenDetectMethod::operator()(crHandle &handle)
{
	char *moreOpenStr = getenv("MoreOpen");
	if (moreOpenStr && atoi(moreOpenStr) > 0)
	{
		HWND hWnd = crStartHandler::getInstance()->getWindowHandle();
		::SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}
/////////////////////////////////////////
//
//crJXJAutoTouchItemMethod
//
/////////////////////////////////////////
crJXJAutoTouchItemMethod::crJXJAutoTouchItemMethod():
	m_dist(10)
{
}
crJXJAutoTouchItemMethod::crJXJAutoTouchItemMethod(const crJXJAutoTouchItemMethod& handle) :
	crMethod(handle),
	m_dist(handle.m_dist)
{
}
void crJXJAutoTouchItemMethod::inputParam(int i, void *param)
{
}
void crJXJAutoTouchItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_dist = atof(str.c_str());
		break;
	}
}
void crJXJAutoTouchItemMethod::operator()(crHandle &handle)
{
	crRole *myRole;
	crData *myRoleData;
	void *param;
	unsigned char itemstate;
	std::vector< ref_ptr<crRole> > RoleVec;
	crMyPlayerData::getInstance()->lockMyRoleNpcMap();
	crMyPlayerData::MyRoleNpcMap &myRoleNpcMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
	for ( crMyPlayerData::MyRoleNpcMap::iterator itr = myRoleNpcMap.begin();
		 itr != myRoleNpcMap.end();
		 ++itr )
	{
		myRole = itr->second.first.get();
		myRoleData = myRole->getDataClass();
		myRoleData->getParam(WCHDATA_ItemState, param);
		itemstate = *(unsigned char *)param;
		if (itemstate == IS_Dead)
			continue;
		RoleVec.push_back(myRole);
	}
	crMyPlayerData::getInstance()->unlockMyRoleNpcMap();

	for (std::vector< ref_ptr<crRole> >::iterator itr = RoleVec.begin();
		 itr != RoleVec.end();
		++itr)
	{
		myRole = itr->get();
		crInstanceItem *item = crMyPlayerData::getInstance()->getOneInRangeItem(myRole,m_dist);
		if (item)
		{
			item->doEvent(WCH_Touch);
			itemstate = IS_Stop;
			myRole->getDataClass()->inputParam(WCHDATA_ItemState, &itemstate);
			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJQueryItemRTHPMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJQueryItemRTHPMethod::crJXJQueryItemRTHPMethod() {}
crJXJQueryItemRTHPMethod::crJXJQueryItemRTHPMethod(const crJXJQueryItemRTHPMethod& handle) :
	crMethod(handle)
{
}
void crJXJQueryItemRTHPMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJQueryItemRTHPMethod::addParam(int i, const std::string& str)
{
}

void crJXJQueryItemRTHPMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if (myPlayer && netConductor)
	{
		void *param;
		crData *cameraData = m_this->getDataClass();
		cameraData->getParam(WCHDATA_JXJQueryItemRTHPMap, param);
		QueryItemRTHPMap *queryItemRTHPMap = (QueryItemRTHPMap *)param;
		queryItemRTHPMap->clear();
		crInstanceItem *item;
		crViewMatterObject *bot;
		crData *itemData;
		float clienthp;
		crMyPlayerData::getInstance()->lockInRangePlayerMap();
		crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
		for (crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
			itr != playerMap.end();
			++itr)
		{
			item = itr->second.first.get();
			bot = dynamic_cast<crViewMatterObject *>(item->getRelNode());
			if (!bot/* || !bot->getVisiable()*/)
				continue;
			if (crBrain::getInstance()->getCameraFrustum().contains(bot->getBoundBox()))
			{
				itemData = item->getDataClass();
				clienthp = 0.0f;
				if (itemData)
				{
					itemData->getParam(WCHDATA_RTHP, param);
					clienthp = *(float*)param;
					itemData->getParam(WCHDATA_ExtraHP, param);
					clienthp += *(float*)param;
				}
				queryItemRTHPMap->insert(std::make_pair(item, clienthp));
			}
		}
		crMyPlayerData::getInstance()->unlockInRangePlayerMap();

		crMyPlayerData::getInstance()->lockInRangeNpcMap();
		crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
		for (crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
			itr != npcMap.end();
			++itr)
		{
			item = itr->second.first.get();
			bot = dynamic_cast<crViewMatterObject *>(item->getRelNode());
			if (!bot/* || !bot->getVisiable()*/)
				continue;
			if (crBrain::getInstance()->getCameraFrustum().contains(bot->getBoundBox()))
			{
				itemData = item->getDataClass();
				clienthp = 0.0f;
				if (itemData)
				{
					itemData->getParam(WCHDATA_RTHP, param);
					clienthp = *(float*)param;
					itemData->getParam(WCHDATA_ExtraHP, param);
					clienthp += *(float*)param;
				}
				queryItemRTHPMap->insert(std::make_pair(item, clienthp));
			}

		}
		crMyPlayerData::getInstance()->unlockInRangeNpcMap();

		crMatrixTransform *itembot = NULL;
		crMyPlayerData::getInstance()->lockInRangeItemMap();
		crMyPlayerData::InRangeItemMap &itemMap = crMyPlayerData::getInstance()->getInRangeItemMap();
		for (crMyPlayerData::InRangeItemMap::iterator itr = itemMap.begin();
			itr != itemMap.end();
			++itr)
		{
			item = itr->second.first.get();
			itembot = dynamic_cast<crMatrixTransform *>(item->getRelNode());
			if (!itembot/* || !itembot->getVisiable()*/)
				continue;
			if (crBrain::getInstance()->getCameraFrustum().contains(itembot->getBoundBox()))
			{
				queryItemRTHPMap->insert(std::make_pair(item, 0.0f));
			}
		}
		crMyPlayerData::getInstance()->unlockInRangeItemMap();

		if (!queryItemRTHPMap->empty())
		{
			unsigned char size = queryItemRTHPMap->size();
			ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
			streamBuf->createBuf(13 * size+1);
			streamBuf->_writeUChar(size);
			ref_ptr<crInstanceItem>item;
			for (QueryItemRTHPMap::iterator itr = queryItemRTHPMap->begin();
				itr != queryItemRTHPMap->end();
				++itr)
			{
				item = itr->first;
				streamBuf->_writeUChar(item->getItemtype());
				streamBuf->_writeInt(item->getID());
				if (item->getItemtype() == crInstanceItem::Role)
					streamBuf->_writeInt(item->getRoleID());
				streamBuf->_writeFloat(itr->second);
			}

			crPlayerDataSceneEventPacket packet;
			crPlayerDataSceneEventPacket::buildRequestPacket(packet, WCH_JXJRecvQueryItemRTHP, streamBuf.get());
			netConductor->getNetManager()->sendPacket("all", packet);
		}
		//CRCore::notify(CRCore::ALWAYS) << "JXJQueryItemRTHP = " << queryItemRTHPMap->size() << std::endl;
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvQueryItemRTHPMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvQueryItemRTHPMethod::crJXJRecvQueryItemRTHPMethod() {}
crJXJRecvQueryItemRTHPMethod::crJXJRecvQueryItemRTHPMethod(const crJXJRecvQueryItemRTHPMethod& handle) :
	crMethod(handle)
{
}
void crJXJRecvQueryItemRTHPMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if (param)
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

void crJXJRecvQueryItemRTHPMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvQueryItemRTHPMethod::operator()(crHandle &handle)
{
	if (m_stream.valid())
	{
		unsigned char itemType;
		int id;
		int roleid;
		float rthp;
		float extrahp;
		int armyCount;
		ref_ptr<crInstanceItem>item;
		ref_ptr<crData>itemData;
		if (m_netType == SceneServer)
		{
			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			crNetManager *netManager = sceneServerConductor->getNetManager();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
			if (playerData.valid() && m_this->getMainRole())
			{
				void *param;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				unsigned char size = m_stream->_readUChar();
				CRNetApp::crScene *scene = m_this->getMainRole()->getScene();
				int roomid = m_this->getMainRole()->getRoomID();
				unsigned char count = 0;
				float hp;
				stream->createBuf(128);
				stream->_writeUChar(count);
				m_this->clearInCameraItemSet();
				for (int i = 0; i<size; i++)
				{
					itemType = m_stream->_readUChar();
					id = m_stream->_readInt();
					if (itemType == crInstanceItem::Role)
					{
						roleid = m_stream->_readInt();
					}
					hp = m_stream->_readFloat();
					if (itemType == crInstanceItem::Role)
						item = scene->findRoomRole(roomid, id, roleid);
					else
						item = scene->findRoomItem(roomid, id);
					if (item.valid())
					{
						m_this->insertInCameraItem(item.get());
						itemData = item->getDataClass();
						if (itemData.valid())
						{
							itemData->getParam(WCHDATA_RTHP, param);
							if (param)
							{
								rthp = *(float*)param;
								itemData->getParam(WCHDATA_ExtraRTHP, param);
								extrahp = *(float*)param;
								if (fabs(rthp + extrahp - hp) > 1.0f)
								{
									itemData->getParam(WCHDATA_JXJRTArmyCount, param);
									armyCount = *(int *)param;
									stream->_writeUChar(itemType);
									stream->_writeInt(id);
									if (itemType == crInstanceItem::Role)
										stream->_writeInt(roleid);
									stream->_writeFloat(rthp);
									stream->_writeFloat(extrahp);
									stream->_writeInt(armyCount);
									count++;
								}
							}
						}
					}
				}
				if (count > 0)
				{
					int bufsize = stream->getBufSize();
					stream->seekBegin();
					stream->_writeUChar(count);
					stream->setBufSize(bufsize);
				}

				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildReplyPacket(packet, m_this->getPlayerID(), WCH_JXJRecvQueryItemRTHP, stream.get());
				netManager->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
			}
			//CRCore::notify(CRCore::ALWAYS) << "JXJRecvQueryItemRTHP = " << (int)count << std::endl;
		}
		else if (m_netType == GameClient_Game)
		{
			unsigned char size = m_stream->_readUChar();
			//CRCore::notify(CRCore::ALWAYS) << "JXJRecvQueryItemRTHP = " << (int)size << std::endl;
			for (int i = 0; i < size; i++)
			{
				itemType = m_stream->_readUChar();
				id = m_stream->_readInt();
				if (itemType == crInstanceItem::Role)
				{
					roleid = m_stream->_readInt();
				}
				rthp = m_stream->_readFloat();
				extrahp = m_stream->_readFloat();
				armyCount = m_stream->_readInt();
				if (itemType == crInstanceItem::Role)
				{
					crMyPlayerData::RoleNpcPair roleNpcPair;
					if (crMyPlayerData::getInstance()->findInRangePlayer(id, roleid, roleNpcPair))
					{
						item = roleNpcPair.first.get();
					}
				}
				else
				{
					crMyPlayerData::ItemNpcPair itemNpcPair;
					if (crMyPlayerData::getInstance()->findInRangeNpc(id, itemNpcPair))
					{
						item = itemNpcPair.first.get();
					}
				}
				if (item.valid())
				{
					itemData = item->getDataClass();
					if (itemData.valid())
					{
						itemData->inputParam(WCHDATA_RTHP, &rthp);
						itemData->inputParam(WCHDATA_ExtraRTHP, &extrahp);
						itemData->inputParam(WCHDATA_JXJRTArmyCount, &armyCount);
					}
				}
			}
		}
	}
}
