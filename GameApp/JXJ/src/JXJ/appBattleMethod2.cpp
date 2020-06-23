/**********************************************************************
*
*	文件:	   appBattleMethod2.cpp
*
*	描述:	   
*
*	作者:	   曹霖麟
*					
*
**********************************************************************/
#include <sys/stat.h>
#include <rbody/Creature3D/CreBody.h>
#include <crcore/crUniversalHandles.h>
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
#include <CREncapsulation/crStartHandler.h>
#include <CRProducer/crViewer.h>
#include <CRIOManager/crCookFile.h>
#include <JXJ/appBattleMethod2.h>
#include <JXJ/appDataParam.h>
#include <JXJ/appMsg.h>
#include <JXJ/appPackets.h>
#include <JXJ/appData3.h>
#include <JXJ/appMethod.h>
#include <JXJ/appMethod16.h>
#include <CRNetApp/appNetMethod.h>
//

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
using namespace std;
/////////////////////////////////////////
//
//crJXJShowFaceIconMethod
//
/////////////////////////////////////////
// crJXJShowFaceIconMethod::crJXJShowFaceIconMethod(){}
// crJXJShowFaceIconMethod::crJXJShowFaceIconMethod(const crJXJShowFaceIconMethod& handle):
// 	crMethod(handle),
// 	m_armyRadio(handle.m_armyRadio)
// {
// }
// void crJXJShowFaceIconMethod::inputParam(int i, void *param)
// {
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
// }
// 
// void crJXJShowFaceIconMethod::addParam(int i, const std::string& str)
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		m_armyRadio = str;
// 		break;
// 	case 1:
// 		m_faceicon2 = str;
// 		break;
// 	case 2:
// 		m_faceicon3 = str;
// 		break;
// 	}
// }
// 
// void crJXJShowFaceIconMethod::operator()(crHandle &handle)
// {
// 	crRadioGroupWidgetNode *armyRadioGroup = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_armyRadio));
// 	ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
// 	crRadioGroupWidgetNode::RadioGroup &radioGroup = armyRadioGroup->getRadioGroup();
// 	crMyPlayerData *me = crMyPlayerData::getInstance();
// 	crRole *role = me->getLoadedRoleByIndex(0);//0,1,2
// 
// 	int itemid1 = role->getAbstractItemID();
// 	crTableIO::StrVec record1;
// 	troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid1),record1);
// 	role = me->getLoadedRoleByIndex(1);//0,1,2
// 	int itemid2 = role->getAbstractItemID();
// 	crTableIO::StrVec record2;
// 	troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid2),record2);
// 	role = me->getLoadedRoleByIndex(2);//0,1,2
// 	int itemid3 = role->getAbstractItemID();
// 	crTableIO::StrVec record3;
// 	troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid3),record3);
// 	int iconid = troopsTab->getTitleIndex("icon");
// 	int iconid1 = troopsTab->getTitleIndex("icon灰");
// 	std::string iconfile1;
// 	crMultiSwitch *radioButton;
// 	crStateSet *ss1;
// 	crTexture2D *tex2d;
// 	crRadioGroupWidgetNode::RadioGroup::iterator itr = radioGroup.begin();
// 
// 	int i = 0;
// 	for(;i < 3 && itr != radioGroup.end();)
// 	{
// 		switch (i)
// 		{
// 		case 0:
// 			iconfile1 = record1[iconid];
// 			break;
// 		case 1:
// 			iconfile1 = record2[iconid];
// 			break;
// 		case 2:
// 			iconfile1 = record3[iconid];
// 			break;
// 		}
// 		radioButton = itr->second->getButton();
// 		radioButton->setVisiable(true);
// 		ss1 = dynamic_cast<crObject *>(radioButton->getChild(0))->getDrawable(0)->getStateSet();
// 
// 		tex2d = dynamic_cast<crTexture2D *>(ss1->getTextureAttribute(0,crStateAttribute::TEXTURE));
// 		if(tex2d)
// 		{
// 			if(tex2d->getImageNameID().compare(iconfile1)!=0)
// 				tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
// 		}
// 		else
// 		{
// 			tex2d = new crTexture2D;
// 			tex2d->setFilter(crTexture2D::MIN_FILTER, crTexture::LINEAR);
// 			tex2d->setFilter(crTexture2D::MAG_FILTER, crTexture::LINEAR);
// 			tex2d->setWrap(crTexture::WRAP_S,crTexture::CLAMP_TO_BORDER);
// 			tex2d->setWrap(crTexture::WRAP_T,crTexture::CLAMP_TO_BORDER);
// 			tex2d->setWrap(crTexture::WRAP_R,crTexture::CLAMP_TO_BORDER);
// 			tex2d->setImage(0,tex2d->getImageDataRequestHandler()->requestImageFile(iconfile1,tex2d));
// 			ss1->setTextureAttributeAndModes(0,tex2d,crStateAttribute::ON);
// 		}
// 		i++;
// 	}
// }
/////////////////////////////////////////
//
//crJXJShowJKIconMethod
//
/////////////////////////////////////////
crJXJShowJKIconMethod::crJXJShowJKIconMethod(){}
crJXJShowJKIconMethod::crJXJShowJKIconMethod(const crJXJShowJKIconMethod& handle):
	crMethod(handle),
	m_mainIconQ(handle.m_mainIconQ),
	m_mainIconW(handle.m_mainIconW),
	m_mainIconE(handle.m_mainIconE),
	m_iconQ(handle.m_iconQ),
	m_iconW(handle.m_iconW),
	m_iconE(handle.m_iconE)
{
}
void crJXJShowJKIconMethod::inputParam(int i, void *param)
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

void crJXJShowJKIconMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_mainIconQ = str;
		break;
	case 1:
		m_mainIconW = str;
		break;
	case 2:
		m_mainIconE = str;
		break;
	case 3:
		m_iconQ = str;
		break;
	case 4:
		m_iconW = str;
		break;
	case 5:
		m_iconE = str;
		break;
	case 6:
		m_mainIconQSwitch = str;
		break;
	case 7:
		m_mainIconWSwitch = str;
		break;
	case 8:
		m_mainIconESwitch = str;
		break;
	case 9:
		m_iconQSwitch = str;
		break;
	case 10:
		m_iconWSwitch = str;
		break;
	case 11:
		m_iconESwitch = str;
		break;
	case 12:
		m_strArmyName[0] = str;
		break;
	case 13:
		m_strArmyName[1] = str;
		break;
	case 14:
		m_strArmyName[2] = str;
		break;
	}
}

void crJXJShowJKIconMethod::operator()(crHandle &handle)
{
	void *param;
	std::string iconfile1,iconfile2,iconfile3;
	ref_ptr<crImageBoxWidgetNode>  mainIconQ = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mainIconQ));
	ref_ptr<crImageBoxWidgetNode>  mainIconW = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mainIconW));
	ref_ptr<crImageBoxWidgetNode>  mainIconE = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_mainIconE));
	ref_ptr<crImageBoxWidgetNode>  iconQ = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_iconQ));
	ref_ptr<crImageBoxWidgetNode>  iconW = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_iconW));
	ref_ptr<crImageBoxWidgetNode>  iconE = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_iconE));

	ref_ptr<crMultiSwitch>  mainIconQSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_mainIconQSwitch));
	ref_ptr<crMultiSwitch>  mainIconWSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_mainIconWSwitch));
	ref_ptr<crMultiSwitch>  mainIconESwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_mainIconESwitch));
	ref_ptr<crMultiSwitch>  iconQSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_iconQSwitch));
	ref_ptr<crMultiSwitch>  iconWSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_iconWSwitch));
	ref_ptr<crMultiSwitch>  iconESwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_iconESwitch));

	ref_ptr<crStaticTextWidgetNode> roleName[3];
	for (int i = 0; i < 3;i++)
	{
		roleName[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strArmyName[i]));
	}
	
	ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	
	mainIconQ->setVisiable(false);
	mainIconW->setVisiable(false);
	mainIconE->setVisiable(false);
	iconQ->setVisiable(false);
	iconW->setVisiable(false);
	iconE->setVisiable(false);

	int iconid = itemtab->getTitleIndex("icon");
	int nameid = itemtab->getTitleIndex("name");
	int abstractid = itemtab->getTitleIndex("abstractid");
	int levelid = itemtab->getTitleIndex("品质");
	crVector2i equip = crVector2i(-1,-1);
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *playerData = playerGameData->getDataClass();
	crTableIO::StrVec record1,itemrecord1;
	int index = 0;
	int cardcolor = 0;
	if(playerData)
	{
		playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *formationVec = (FormationInfoVec *)param;
		crMyPlayerData::MyRoleNpcMap roleMap =  crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for(crMyPlayerData::MyRoleNpcMap::iterator itr_role = roleMap.begin();itr_role!=roleMap.end();itr_role++,index++)
		{
			for(FormationInfoVec::iterator itr = formationVec->begin();itr!=formationVec->end();itr++)
			{
				if(itr->get()->getRoleID()==itr_role->first)
				{
					equip = itr->get()->getEquips();
					break;
				}
			}
			if(index == 0)
			{
				if(equip[0]>0 || equip[1]>0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						mainIconQ->setVisiable(true);
						mainIconQ->setImageName(itemrecord1[iconid]);
						mainIconQSwitch->setActiveSwitchSet(cardcolor+1);
					}
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						iconQ->setVisiable(true);
						iconQ->setImageName(itemrecord1[iconid]);
						iconQSwitch->setActiveSwitchSet(cardcolor+1);
					}
				}
				if(roleName[index].valid())
					roleName[index]->setString(std::string(itr_role->second.first->getIName().c_str()));
			}
			else if(index == 1)
			{
				if(equip[0]>0 || equip[1]>0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						mainIconW->setVisiable(true);
						mainIconW->setImageName(itemrecord1[iconid]);
						mainIconWSwitch->setActiveSwitchSet(cardcolor+1);
					}
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						iconW->setVisiable(true);
						iconW->setImageName(itemrecord1[iconid]);
						iconWSwitch->setActiveSwitchSet(cardcolor+1);
					}
				}

				if(roleName[index].valid())
					roleName[index]->setString(std::string(itr_role->second.first->getIName().c_str()));
			}
			else if(index == 2)
			{
				if(equip[0]>0 || equip[1]>0)
				{
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[0]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						mainIconE->setVisiable(true);
						mainIconE->setImageName(itemrecord1[iconid]);
						mainIconESwitch->setActiveSwitchSet(cardcolor+1);
					}
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equip[1]),itemrecord1)>=0)
					{
						cardcolor = atoi(itemrecord1[levelid].c_str());
						//if (cardcolor > CardColor_Orange)
						//{
						//	cardcolor = CardColor_Orange;
						//}
						iconE->setVisiable(true);
						iconE->setImageName(itemrecord1[iconid]);
						iconESwitch->setActiveSwitchSet(cardcolor+1);
					}
				}

				if(roleName[index].valid())
					roleName[index]->setString(std::string(itr_role->second.first->getIName().c_str()));
			}
		}
		playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
	}	
}

/////////////////////////////////////////
//
//crJXJUpdateJKIconMethod
//
/////////////////////////////////////////
crJXJUpdateJKIconMethod::crJXJUpdateJKIconMethod(){}
crJXJUpdateJKIconMethod::crJXJUpdateJKIconMethod(const crJXJUpdateJKIconMethod& handle):
	crMethod(handle),
	m_isDeadSwitchQ(handle.m_isDeadSwitchQ),
	m_isDeadSwitchW(handle.m_isDeadSwitchW),
	m_isDeadSwitchE(handle.m_isDeadSwitchE)
{
	for (unsigned char i = 0; i < 3; ++ i)
	{
		m_shiqi[i] = handle.m_shiqi[i];
		m_shiqi_input[i] = handle.m_shiqi_input[i];
	}
}
void crJXJUpdateJKIconMethod::inputParam(int i, void *param)
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

void crJXJUpdateJKIconMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_isDeadSwitchQ = str;
		break;
	case 1:
		m_isDeadSwitchW = str;
		break;
	case 2:
		m_isDeadSwitchE = str;
		break;
	case 9:
	case 10:
	case 11:
		m_shiqi[i - 9] = str;
		break;
	case 12:
	case 13:
	case 14:
		m_shiqi_input[i - 12] = str;
		break;
	}
}

void crJXJUpdateJKIconMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crMultiSwitch>  isDeadQ = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_isDeadSwitchQ));
	ref_ptr<crMultiSwitch>  isDeadW = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_isDeadSwitchW));
	ref_ptr<crMultiSwitch>  isDeadE = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_isDeadSwitchE));

	ref_ptr<crProgressWidgetNode> shiqi[3] = {NULL};
	ref_ptr<crStaticTextWidgetNode > shiqi_input[3] = {NULL};
	for (unsigned char i = 0; i < 3; ++ i)
	{
		shiqi[i] = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_shiqi[i]));
		shiqi_input[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_shiqi_input[i]));
		if (shiqi[i].valid())
		{
			shiqi[i]->setProgress(0);
		}
		if (shiqi_input[i].valid())
		{
			shiqi_input[i]->clearString();
		}
	}
	if(!isDeadQ || !isDeadW || !isDeadE) return;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	crRole *role = me->getLoadedRoleByIndex(0);//0,1,2
	crRole *role1 = me->getLoadedRoleByIndex(1);
	crRole *role2 = me->getLoadedRoleByIndex(2);
	crData *roledata  = NULL;

	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	int iconid = itemtab->getTitleIndex("icon");
	int nameid = itemtab->getTitleIndex("name");
	int abstractid = itemtab->getTitleIndex("abstractid");
	int itemid;
	crTableIO::StrVec record1,itemrecord1;

	crData *playerdata = me->getPlayerGameData()->getDataClass();
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
	playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec &myteam = *(FormationInfoVec *)param;
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));

	crVector2i equips = crVector2i(-1,-1);

	bool mpUnlack = false;
	bool inCD = false;
	bool hasJK = false;
	float maxMp = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMAXRTMPVal).c_str());
	if(role)
	{
		roledata= role->getDataClass();
		unsigned int guisestate = GS_Normal;
		role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		roledata->getParam(WCHDATA_RTHP,param);
		float rthp = *(float*)param;
		roledata->getParam(WCHDATA_RTMP,param);
		float rtmp = *(float *)param;

		mpUnlack = true;
		inCD = false;
		hasJK = false;
		equips[0] = equips[1] = -1;

		for (FormationInfoVec::iterator cItr = myteam.begin(); cItr != myteam.end(); ++ cItr)
		{
			if (!cItr->valid())
				continue;
			if ((*cItr)->getRoleID() == role->getRoleID())
			{
				equips = (*cItr)->getEquips();
				break;
			}
		}
		if(equips[0]>0)
		{
			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord1)>=0)
			{
				itemid = atoi(itemrecord1[abstractid].c_str());
				roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
				roledata->getParam(WCHDATA_EquipOnMap,param);
				EquipOnMap *equipOnMap = (EquipOnMap *)param;
				EquipOnMap::iterator itr_equipmap = equipOnMap->find(itemid);
				crItemChild *itemChild  = NULL;
				if(itr_equipmap!=equipOnMap->end())
					itemChild= role->findChildItem(itr_equipmap->second);	
				if(itemChild && itemChild->isLoaded())
				{
					hasJK = true;
					crInstanceItem *item = itemChild->getInstanceItem();
					crData *skillData = item->getDataClass();
					if(skillData)
					{
						skillData->getParam(WCHDATA_JXJUseMpValLimit,param);
						unsigned short useMPLimit = *(unsigned short*)param;
						mpUnlack = rtmp < useMPLimit || rtmp < maxMp;

						CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
						skillData->getParam(WCHDATA_CDTime,param);
						float cdtime = *(float *)param;

						float cdreduce = 0.0f;
						skillData->getParam(WCHDATA_DataType,param);
						unsigned char datatype = *(unsigned char*)param;
						if(datatype == DT_Attack)
							role->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
						else
							role->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&cdreduce,NULL));
						if(cdreduce<=-1.0f)
							cdreduce = -0.99f; 
						cdtime = cdtime/(1.0f+cdreduce);
						//cdtime += protectTime;
						//该技能上一次的施法时间

						skillData->getParam(WCHDATA_LastUseTime,param);
						_crInt64 lastUseTime = *(_crInt64 *)param;
						if(lastUseTime!=0)
						{
							float dt = CRCore::Timer::instance()->delta_s( lastUseTime, t1 );
							int timer =(int)(cdtime-dt);
							inCD = timer > 0;
						}
					}
				}
				roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		if (shiqi[0].valid() && hasJK && shiqi_input[0].valid())
		{
			shiqi[0]->setProgress(rtmp / maxMp);
			shiqi_input[0]->setString(crArgumentParser::appItoa((int)rtmp) + "/" +  crArgumentParser::appItoa((int)maxMp));
		}
		if (rthp <= 0.0f)
			// 死亡状态
			isDeadQ->setActiveSwitchSet(1);
		else if(/*rthp<=0.0f ||*/
			guisestate & GS_Silent/* || guisestate & GS_Garrison*/ || guisestate & GS_Swoon || guisestate & GS_Stagnate ||
			guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare || guisestate & GS_Sheep || guisestate & GS_OnHorse || 
			guisestate & GS_ExtraShifting/* || guisestate & GS_NoAttack*/ || guisestate & GS_Nihility )
			// 不能施法状态
			isDeadQ->setActiveSwitchSet(3);
		else if (inCD || mpUnlack)
			// 明亮
			isDeadQ->setActiveSwitchSet(0);
		else 
			isDeadQ->setActiveSwitchSet(2);
	}
	if(role1)
	{
		roledata= role1->getDataClass();
		unsigned int guisestate = GS_Normal;
		role1->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		roledata->getParam(WCHDATA_RTHP,param);
		float rthp = *(float*)param;
		//if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)

		roledata->getParam(WCHDATA_RTMP,param);
		float rtmp = *(float *)param;

		mpUnlack = true;
		inCD = false;
		hasJK = false;
		equips[0] = equips[1] = -1;

		for (FormationInfoVec::iterator cItr = myteam.begin(); cItr != myteam.end(); ++ cItr)
		{
			if (!cItr->valid())
				continue;
			if ((*cItr)->getRoleID() == role1->getRoleID())
			{
				equips = (*cItr)->getEquips();
				break;
			}
		}
		if(equips[0]>0)
		{
			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord1)>=0)
			{
				itemid = atoi(itemrecord1[abstractid].c_str());
				roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
				roledata->getParam(WCHDATA_EquipOnMap,param);
				EquipOnMap *equipOnMap = (EquipOnMap *)param;
				EquipOnMap::iterator itr_equipmap = equipOnMap->find(itemid);
				crItemChild *itemChild  = NULL;
				if(itr_equipmap!=equipOnMap->end())
					itemChild= role1->findChildItem(itr_equipmap->second);	
				if(itemChild && itemChild->isLoaded())
				{
					hasJK = true;
					crInstanceItem *item = itemChild->getInstanceItem();
					crData *skillData = item->getDataClass();
					if(skillData)
					{
						skillData->getParam(WCHDATA_JXJUseMpValLimit,param);
						unsigned short useMPLimit = *(unsigned short*)param;
						mpUnlack = rtmp < useMPLimit || rtmp < maxMp;
						CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
						skillData->getParam(WCHDATA_CDTime,param);
						float cdtime = *(float *)param;

						float cdreduce = 0.0f;
						skillData->getParam(WCHDATA_DataType,param);
						unsigned char datatype = *(unsigned char*)param;
						if(datatype == DT_Attack)
							role1->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
						else
							role1->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&cdreduce,NULL));
						if(cdreduce<=-1.0f)
							cdreduce = -0.99f; 
						cdtime = cdtime/(1.0f+cdreduce);
						//cdtime += protectTime;
						//该技能上一次的施法时间

						skillData->getParam(WCHDATA_LastUseTime,param);
						_crInt64 lastUseTime = *(_crInt64 *)param;
						if(lastUseTime!=0)
						{
							float dt = CRCore::Timer::instance()->delta_s( lastUseTime, t1 );
							int timer =(int)(cdtime-dt);
							inCD = timer > 0;
						}
					}
				}
				roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		if (shiqi[1].valid() && hasJK && shiqi_input[1].valid())
		{
			shiqi[1]->setProgress(rtmp / maxMp);
			shiqi_input[1]->setString(crArgumentParser::appItoa((int)rtmp) + "/" +  crArgumentParser::appItoa((int)maxMp));
		}
		if (rthp <= 0.0f)
			// 死亡状态
			isDeadW->setActiveSwitchSet(1);
		else if(/*rthp<=0.0f ||*/
			guisestate & GS_Silent/* || guisestate & GS_Garrison*/ || guisestate & GS_Swoon || guisestate & GS_Stagnate ||
			guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare || guisestate & GS_Sheep || guisestate & GS_OnHorse || 
			guisestate & GS_ExtraShifting /*|| guisestate & GS_NoAttack*/ || guisestate & GS_Nihility)
			// 不能施法状态
			isDeadW->setActiveSwitchSet(3);
		else if (inCD || mpUnlack)
			// 明亮
			isDeadW->setActiveSwitchSet(0);
		else 
			isDeadW->setActiveSwitchSet(2);
	}
	if(role2)
	{
		roledata= role2->getDataClass();
		unsigned int guisestate = GS_Normal;
		role2->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		roledata->getParam(WCHDATA_RTHP,param);
		float rthp = *(float*)param;

		roledata->getParam(WCHDATA_RTMP,param);
		float rtmp = *(float *)param;

		mpUnlack = true;
		inCD = false;
		hasJK = false;
		equips[0] = equips[1] = -1;

		for (FormationInfoVec::iterator cItr = myteam.begin(); cItr != myteam.end(); ++ cItr)
		{
			if (!cItr->valid())
				continue;
			if ((*cItr)->getRoleID() == role2->getRoleID())
			{
				equips = (*cItr)->getEquips();
				break;
			}
		}
		if(equips[0]>0)
		{
			if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord1)>=0)
			{
				itemid = atoi(itemrecord1[abstractid].c_str());
				roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
				roledata->getParam(WCHDATA_EquipOnMap,param);
				EquipOnMap *equipOnMap = (EquipOnMap *)param;
				EquipOnMap::iterator itr_equipmap = equipOnMap->find(itemid);
				crItemChild *itemChild  = NULL;
				if(itr_equipmap!=equipOnMap->end())
					itemChild= role2->findChildItem(itr_equipmap->second);	
				if(itemChild && itemChild->isLoaded())
				{
					hasJK = true;
					crInstanceItem *item = itemChild->getInstanceItem();
					crData *skillData = item->getDataClass();
					if(skillData)
					{
						skillData->getParam(WCHDATA_JXJUseMpValLimit,param);
						unsigned short useMPLimit = *(unsigned short*)param;
						mpUnlack = rtmp < useMPLimit|| rtmp < maxMp;

						CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
						skillData->getParam(WCHDATA_CDTime,param);
						float cdtime = *(float *)param;

						float cdreduce = 0.0f;
						skillData->getParam(WCHDATA_DataType,param);
						unsigned char datatype = *(unsigned char*)param;
						if(datatype == DT_Attack)
							role2->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
						else
							role2->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&cdreduce,NULL));
						if(cdreduce<=-1.0f)
							cdreduce = -0.99f; 
						cdtime = cdtime/(1.0f+cdreduce);
						//cdtime += protectTime;
						//该技能上一次的施法时间

						skillData->getParam(WCHDATA_LastUseTime,param);
						_crInt64 lastUseTime = *(_crInt64 *)param;
						if(lastUseTime!=0)
						{
							float dt = CRCore::Timer::instance()->delta_s( lastUseTime, t1 );
							int timer =(int)(cdtime-dt);
							inCD = timer > 0;
						}
					}
				}
				roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		if (shiqi[2].valid() && hasJK && shiqi_input[2].valid())
		{
			shiqi[2]->setProgress(rtmp / maxMp);
			shiqi_input[2]->setString(crArgumentParser::appItoa((int)rtmp) + "/" +  crArgumentParser::appItoa((int)maxMp));
		}
		if (rthp <= 0.0f) 
			// 死亡状态
			isDeadE->setActiveSwitchSet(1);
		else if(/*rthp<=0.0f ||*/
			guisestate & GS_Silent/* || guisestate & GS_Garrison*/ || guisestate & GS_Swoon || guisestate & GS_Stagnate ||
			guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare || guisestate & GS_Sheep || guisestate & GS_OnHorse || 
			guisestate & GS_ExtraShifting/* || guisestate & GS_NoAttack*/ || guisestate & GS_Nihility)
			// 不能施法状态
			isDeadE->setActiveSwitchSet(3);
		else if (inCD || mpUnlack)
			// 明亮
			isDeadE->setActiveSwitchSet(0);
		else 
			isDeadE->setActiveSwitchSet(2);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJShowTipsMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJShowTipsMethod::crJXJShowTipsMethod():m_this(NULL){}
crJXJShowTipsMethod::crJXJShowTipsMethod(const crJXJShowTipsMethod &handle):
	crMethod(handle),m_this(NULL),
	m_tips(handle.m_tips),m_show(0),m_offsetx(0),m_offsety(0),m_str(0)
{}
void crJXJShowTipsMethod::inputParam(int i, void *param)
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
void crJXJShowTipsMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_tips = str;
		break;
	case 1:
		m_show = atoi(str.c_str());
		break;
	case 2:
		m_offsetx = atof(str.c_str());
		break;
	case 3:
		m_offsety = atof(str.c_str());
		break;
	case 4:
		m_name = str;
		break;
	case 5:
		m_str = atoi(str.c_str());
		break;
	}
}

void crJXJShowTipsMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	if(canvas.valid())
	{
		const crBoundingBox &bbox = m_this->getBoundBox();
		crVector3 pos = bbox.center();

		canvas->setPosition(crVector3(pos[0],pos[1]+bbox.yLength()*0.5+0.03,0.0f));
		canvas->setCanFocus(false);
		crStaticTextWidgetNode* text = dynamic_cast<crStaticTextWidgetNode *>(canvas->getChildNode(m_name));
		text->setColor(crVector4(255.0,255.0,255.0,255.0));
		text->setCharacterSize(11);
		switch (m_str)
		{
		case 1:
			text->setString(L"攻击(A)");
			break;
		case 2:
			text->setString(L"停止(S)");
			break;
		case 3:
			text->setString(L"移动(M)");
			break;
		case 4:
			text->setString(L"巡逻(P)");
			break;
		case 5:
			text->setString(L"警戒(H)");
			break;
		case 6:
			text->setString(L"信号(I)");
			break;
		case 7:
			text->setString(L"背景(K)");
			break;
		case 8:
			text->setString(L"队形(L)");
			break;
		case 9:
			text->setString(L"视角跟随(C)");
			break;
		case 10:
			text->setString(L"系统");
			break;
		case 11:
			text->setString(L"退出");
			break;
		case 12:
			text->setString(L"智能AI");
			break;
		case 13:
			text->setString(L"帮助");
			break;
		}
		crFilterRenderManager::getInstance()->showCanvas(canvas.get(),m_show);

	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJShowTargetInfoMethod
//
//////////////////////////////////////////////////////////////////////////

crJXJShowTargetInfoMethod::crJXJShowTargetInfoMethod(){}
crJXJShowTargetInfoMethod::crJXJShowTargetInfoMethod(const crJXJShowTargetInfoMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_strName(handle.m_strName),
	m_strHp(handle.m_strHp),
	m_strAttack(handle.m_strAttack),
	m_strDefence(handle.m_strDefence),
	m_strATipCanvas(handle.m_strATipCanvas),
	m_strTipAttack(handle.m_strTipAttack),
	m_strAttackSpeed(handle.m_strAttackSpeed),
	m_strAttackScope(handle.m_strAttackScope),
	m_strCrit(handle.m_strCrit),
	m_strDTipCanvas(handle.m_strDTipCanvas),
	m_strTipDefence(m_strTipDefence),
	m_strMoJia(m_strMoJia),
	m_strDodge(m_strDodge),
	m_strParry(m_strParry)
{

}

void crJXJShowTargetInfoMethod::inputParam(int i, void *param)
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
void crJXJShowTargetInfoMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strName = str;
		break; 
	case 1:
		m_strHp = str;
		break;
	case 2:
		m_strAttack = str;
		break;
	case 3:
		m_strDefence = str;
		break;
	case 4:
		m_strATipCanvas = str;
		break;
	case 5:
		m_strTipAttack = str;
		break;
	case 6:
		m_strAttackSpeed = str;
		break;
	case 7:
		m_strAttackScope = str;
		break;
	case 8:
		m_strCrit = str;
		break;
	case 9:
		m_strDTipCanvas = str;
	case 10:
		m_strTipDefence = str;
		break;
	case 11:
		m_strMoJia = str;
		break;
	case 12:
		m_strDodge = str;
		break;
	case 13:
		m_strParry = str;
		break;
	case 14:
		m_strHPTipCanvas = str;
		break;
	case 15:
		m_strArmyCount = str;
		break;
	case 16:
		m_strPerHp = str;
		break;
	default:
		break;
	}
}
void crJXJShowTargetInfoMethod::operator ()(crHandle &handle)
{///////////需要加入WCHDATA_EquipDefend等装备的附加信息。吴财华
	//ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crStaticTextWidgetNode>  curName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strName));
	ref_ptr<crHypertextWidgetNode>  hp = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strHp));
	ref_ptr<crHypertextWidgetNode>  attack = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strAttack));
	ref_ptr<crHypertextWidgetNode>  defense = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_strDefence));
	ref_ptr<crCanvasNode> aTipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strATipCanvas);
	ref_ptr<crCanvasNode> dTipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strDTipCanvas);
	ref_ptr<crCanvasNode> hpTipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strHPTipCanvas);

	void*param;
	char str[64];
	float outParam;
	crData *canvasData = m_this->getDataClass();
	if(canvasData)
	{
		CRCore::ScopedLock<crData> canvaslock(*canvasData);
		canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
		crInstanceItem* selectItem  = (crInstanceItem *)param;

		if(selectItem)
		{
			crData *roleData = selectItem->getDataClass();
			if(roleData)
			{
				if(curName.valid()) curName->setString(std::string(selectItem->getIName().c_str()));

				ItemMap itemMap_hp;										
				rcfg::ConfigScript cfg_script_hp(&itemMap_hp);
				roleData->getParam(WCHDATA_JXJArmyCount,param);
				/*unsigned short*/int armyCount = *(/*unsigned short*/int *)param;		
				roleData->getParam(WCHDATA_JXJRTArmyCount,param);
				/*unsigned short*/int rtarmy = *(/*unsigned short*/int *)param;
				float maxhp = 0.0f;
				int extrahp = 0;
				float rthp = 0.0f;
				int perHp = 0;
				roleData->getParam(WCHDATA_HP,param);
				perHp = *(int *)param;
				selectItem->doEvent(WCH_GetHP,MAKECREPARAM(&maxhp,NULL));	
				roleData->getParam(WCHDATA_RTHP,param);
				rthp = *(float*)param;
				if(armyCount>1)
				{
					sprintf(str,"%d/%d\0",rtarmy,armyCount);
				}
				else
					sprintf(str,"%d/%d\0",(int)rthp,(int)maxhp);

				extrahp = (int)maxhp-perHp;
				CreateInfoHyperText(cfg_script_hp,(int)maxhp,extrahp);
				if(hp.valid()) hp->setHypertext(cfg_script_hp);	
				//血量属性tips
				if (hpTipCanvas.valid())
				{
					ref_ptr<crHypertextWidgetNode>  tipArmyCount = dynamic_cast<crHypertextWidgetNode *>(hpTipCanvas->getWidget(m_strArmyCount));
					ref_ptr<crHypertextWidgetNode>  tipPerHp = dynamic_cast<crHypertextWidgetNode *>(hpTipCanvas->getWidget(m_strPerHp));

					ItemMap itemMap_armyCount;
					rcfg::ConfigScript cfg_script_armyCount(&itemMap_armyCount);
					CreateInfoHyperText(cfg_script_armyCount, rtarmy, 0);
					if (tipArmyCount.valid()) tipArmyCount->setHypertext(cfg_script_armyCount);

					ItemMap itemMap_perHp;
					outParam = maxhp * rtarmy;
					rcfg::ConfigScript cfg_script_perHp(&itemMap_perHp);
					CreateInfoHyperText(cfg_script_perHp, (int)(outParam), 0);
					if (tipPerHp.valid()) tipPerHp->setHypertext(cfg_script_perHp);
				}
				//攻击力
				ItemMap itemMap_attack;
				outParam = 0.0f;
				rcfg::ConfigScript cfg_script_attack(&itemMap_attack);
				roleData->getParam(WCHDATA_Attack,param);
				int attackvalue = *(int *)param;
				selectItem->doEvent(WCH_GetDamage,MAKECREPARAM(&outParam,NULL));
				int extraAttackvalue = (int)(outParam)-attackvalue;
				CreateInfoHyperText(cfg_script_attack,(int)(outParam),extraAttackvalue);
				if(attack.valid()) attack->setHypertext(cfg_script_attack);

				//防御
				ItemMap itemMap_defend;
				outParam = 0.0f;
				rcfg::ConfigScript cfg_script_defend(&itemMap_defend);
				roleData->getParam(WCHDATA_Defend,param);
				int defendvalue = *(int *)param;
				selectItem->doEvent(WCH_GetDefend,MAKECREPARAM(&outParam,NULL));
				int extraDefendvalue = (int)(outParam) - defendvalue;
				CreateInfoHyperText(cfg_script_defend,(int)(outParam),extraDefendvalue);
				if(defense.valid()) defense->setHypertext(cfg_script_defend);

				//攻击属性TIPS
				if(aTipCanvas.valid())
				{
					ref_ptr<crHypertextWidgetNode>  tipAttack = dynamic_cast<crHypertextWidgetNode *>(aTipCanvas->getWidget(m_strTipAttack));
					ref_ptr<crHypertextWidgetNode>  tipAttackSpeed = dynamic_cast<crHypertextWidgetNode *>(aTipCanvas->getWidget(m_strAttackSpeed));
					ref_ptr<crHypertextWidgetNode>  tipAttackScope = dynamic_cast<crHypertextWidgetNode *>(aTipCanvas->getWidget(m_strAttackScope));
					ref_ptr<crHypertextWidgetNode>  tipCrit = dynamic_cast<crHypertextWidgetNode *>(aTipCanvas->getWidget(m_strCrit));

					if(tipAttack.valid()) tipAttack->setHypertext(cfg_script_attack);

					//攻击速度
					ItemMap itemMap_attackSpeed;
					roleData->getParam(WCHDATA_AttackMap,param);
					AttackMap *attackMap = (AttackMap *)param;
					if(attackMap && !attackMap->empty())
					{
						crItemChild *attackitem = selectItem->findChildItem(attackMap->begin()->second);
						if(attackitem && attackitem->isLoaded())
						{
							crData* childItemData = attackitem->getInstanceItem()->getDataClass();
							childItemData->getParam(WCHDATA_CDTime,param);
							float cdtime = *(float *)param;
							float cdreduce = 0.0f;
							childItemData->getParam(WCHDATA_DataType,param);
							unsigned char datatype = *(unsigned char*)param;
							if(datatype == DT_Attack)
								selectItem->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
							else
								selectItem->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&cdreduce,NULL));
							if(cdreduce<=-1.0f)
								cdreduce = -0.99f; 
							cdtime = cdtime/(1.0f+cdreduce);
							float cdtimeextra = cdreduce/cdtime;

							rcfg::ConfigScript cfg_script_attackspeed(&itemMap_attackSpeed);
							roleData->getParam(WCHDATA_AttackSpeed,param);
							short attackspeedvalue = *(short *)param;
							//int extraAttackspeedvalue = (int)(outParam+0.5)-attackspeedvalue;
							CreateInfoHyperText(cfg_script_attackspeed,1.0f/cdtime,cdtimeextra);
							if(tipAttackSpeed.valid()) tipAttackSpeed->setHypertext(cfg_script_attackspeed);
						}
					}

					//攻击范围
					ItemMap itemMap_scope;
					outParam = 0.0f;
					rcfg::ConfigScript cfg_script_scope(&itemMap_scope);

					roleData->getParam(WCHDATA_CurrentAttackID, param);
					int itemid = *((int*)param);
					if (itemid == 0)
					{
						roleData->excHandle(MAKECREPARAM(WCH_LockData, 1));
						roleData->getParam(WCHDATA_AttackMap, param);
						AttackMap *attackMap = (AttackMap *)param;
						if (attackMap && !attackMap->empty())
						{
							itemid = attackMap->begin()->second;
							roleData->inputParam(WCHDATA_CurrentAttackID, &itemid);
						}
						roleData->excHandle(MAKECREPARAM(WCH_LockData, 0));
					}
					if (itemid != 0)
					{
						crItemChild *itemChild = selectItem->findChildItem(itemid);
						if(itemChild && itemChild->isLoaded())
						{
							crInstanceItem *item = itemChild->getInstanceItem();
							crData *itemData = item->getDataClass();
							itemData->getParam(WCHDATA_ItemUseRange, param);
							unsigned short itemUseRange = *((unsigned short*)param);
							float relRange = itemUseRange * crGlobalHandle::gData()->gUnitScale();
							selectItem->doEvent(MAKEINT64(WCH_GetAttackDist, NULL), MAKECREPARAM(&outParam, NULL));
							int scope = (int)(outParam)+relRange;
							CreateInfoHyperText(cfg_script_scope, scope, (int)(outParam));
							if (tipAttackScope.valid()) tipAttackScope->setHypertext(cfg_script_scope);
						}
					}

					//攻击暴击
					ItemMap itemMap_crit;
					outParam = 0.0f;
					rcfg::ConfigScript cfg_script_crit(&itemMap_crit);
					roleData->getParam(WCHDATA_Crit,param);
					short critvalue = *(short *)param;
					selectItem->doEvent(MAKEINT64(WCH_GetCrit,NULL),MAKECREPARAM(&outParam,NULL));
					int extraCritValue = (int)(outParam) - critvalue;
					CreateInfoHyperText(cfg_script_crit,(int)(outParam),extraCritValue);
					if(tipCrit.valid()) tipCrit->setHypertext(cfg_script_crit);
				}
				//防御相关属性
				if(dTipCanvas.valid())
				{
					ref_ptr<crHypertextWidgetNode>  tipDefence = dynamic_cast<crHypertextWidgetNode *>(dTipCanvas->getWidget(m_strTipDefence));
					ref_ptr<crHypertextWidgetNode>  tipMojia = dynamic_cast<crHypertextWidgetNode *>(dTipCanvas->getWidget(m_strMoJia));
					ref_ptr<crHypertextWidgetNode>  tipDodge = dynamic_cast<crHypertextWidgetNode *>(dTipCanvas->getWidget(m_strDodge));
					ref_ptr<crHypertextWidgetNode>  tipParry = dynamic_cast<crHypertextWidgetNode *>(dTipCanvas->getWidget(m_strParry));

					if(tipDefence.valid()) tipDefence->setHypertext(cfg_script_defend);

					//闪避
					ItemMap itemMap_dodge;
					outParam = 0.0f;
					rcfg::ConfigScript cfg_script_dodge(&itemMap_dodge);
					roleData->getParam(WCHDATA_Dodge,param);
					short dodgevalue = *(short*)param;
					selectItem->doEvent(MAKEINT64(WCH_GetDodge,NULL),MAKECREPARAM(&outParam,NULL));
					int extraDodgevalue = (int)(outParam) - dodgevalue;
					CreateInfoHyperText(cfg_script_dodge,(int)(outParam),extraDodgevalue);
					if(tipDodge.valid()) tipDodge->setHypertext(cfg_script_dodge);

					//格挡
					ItemMap itemMap_parry;
					outParam = 0.0f;
					rcfg::ConfigScript cfg_script_parry(&itemMap_parry);
					roleData->getParam(WCHDATA_Parry,param);
					short blockvalue = *(short *)param;
					selectItem->doEvent(MAKEINT64(WCH_GetParry,NULL),MAKECREPARAM(&outParam,NULL));
					int extraBlockvalue =(int)(outParam) -  blockvalue;
					CreateInfoHyperText(cfg_script_parry,(int)(outParam),extraBlockvalue);
					if(tipParry.valid()) tipParry->setHypertext(cfg_script_parry);

					//魔甲
					ItemMap itemMap_magicD;
					outParam = 0.0f;
					rcfg::ConfigScript cfg_script_magicD(&itemMap_magicD);
					roleData->getParam(WCHDATA_MagicDefend,param);
					short magicdvalue = *(short *)param;
					selectItem->doEvent(MAKEINT64(WCH_GetDefend,1),MAKECREPARAM(&outParam,NULL));
					int extramagicDvalue =(int)(outParam) -  magicdvalue;
					CreateInfoHyperText(cfg_script_magicD,(int)(outParam),extramagicDvalue);
					if(tipMojia.valid()) tipMojia->setHypertext(cfg_script_magicD);
				}
			}
		}
	}
	//速度
// 	ItemMap itemMap_speed;
// 	outParam = 0.0f;
// 	rcfg::ConfigScript cfg_script_speed(&itemMap_speed);
// 	data->getParam(WCHDATA_Speed,param);
// 	short speedvalue = *(short *)param;
// 	selectItem->doEvent(WCH_GetSpeed,MAKECREPARAM(&outParam,NULL));
// 	int extraSpeedvalue = (int)(outParam) - speedvalue;
// 	CreateInfoHyperText(cfg_script_speed,(int)(outParam),extraSpeedvalue);
// 	speed->setHypertext(cfg_script_speed);
	
}
void crJXJShowTargetInfoMethod::CreateInfoHyperText(rcfg::ConfigScript &cfg_script,float baseValue,float extraValue)
{
	std::vector<float>v_i;	
	int index = 1;
	char tmpText[32];
	crVector4f color;
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");
	if(extraValue>0.0f)
		color = COLOR_GREEN_INFO;
	else if(extraValue <0.0f)
		color = COLOR_RED;
	else
		color = COLOR_GRAY_INFO;
	cfg_script.Add("Content");								
	cfg_script.Push("Content",index);							
	v_i.clear();											
	v_i.push_back(color[0]);									
	v_i.push_back(color[1]);									
	v_i.push_back(color[2]);									
	v_i.push_back(color[3]);									
	cfg_script.Add("Color",v_i);														
	sprintf(tmpText,"%.2f\0",baseValue);
	cfg_script.Add("Text",tmpText);							
	cfg_script.Pop();
	index++;

	if(extraValue!=0.0f)
	{
		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);														
		cfg_script.Add("Color",v_i);
		if(extraValue>0.0f)
		{
			cfg_script.Add("Text","(+");	
			/**/
		}
		else
		{
			cfg_script.Add("Text","(-");
			/*color = COLOR_RED;*/
		}
		cfg_script.Pop();
		index++;

		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);							
		v_i.clear();											
		v_i.push_back(color[0]);									
		v_i.push_back(color[1]);									
		v_i.push_back(color[2]);									
		v_i.push_back(color[3]);									
		cfg_script.Add("Color",v_i);														
		sprintf(tmpText,"%.2f\0",abs(extraValue));
		cfg_script.Add("Text",tmpText);
		cfg_script.Pop();
		index++;

		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);							
		v_i.clear();											
		v_i.push_back(color[0]);									
		v_i.push_back(color[1]);									
		v_i.push_back(color[2]);									
		v_i.push_back(color[3]);									
		cfg_script.Add("Color",v_i);														
		cfg_script.Add("Text",string(")"));
		cfg_script.Pop();
	}
	cfg_script.Pop();
}
void crJXJShowTargetInfoMethod::CreateInfoHyperText(rcfg::ConfigScript &cfg_script,int baseValue,int extraValue)
{
	std::vector<float>v_i;	
	int index = 1;
	char tmpText[32];
	crVector4f color;
	cfg_script.Add("Hypertext");
	cfg_script.Push("Hypertext");

	if(extraValue>0)
		color = COLOR_GREEN_INFO;
	else if(extraValue<0)
		color = COLOR_RED;
	else
		color = COLOR_GRAY_INFO;
	cfg_script.Add("Content");								
	cfg_script.Push("Content",index);							
	v_i.clear();											
	v_i.push_back(color[0]);									
	v_i.push_back(color[1]);									
	v_i.push_back(color[2]);									
	v_i.push_back(color[3]);									
	cfg_script.Add("Color",v_i);														
	sprintf(tmpText,"%d\0",baseValue);
	cfg_script.Add("Text",tmpText);							
	cfg_script.Pop();
	index++;

	if(extraValue!=0)
	{
		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);														
		cfg_script.Add("Color",v_i);
		if(extraValue>0)
		{
			cfg_script.Add("Text","(+");	
		}
		else
		{
			cfg_script.Add("Text","(-");
		}
		cfg_script.Pop();
		index++;

		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);							
		v_i.clear();											
		v_i.push_back(color[0]);									
		v_i.push_back(color[1]);									
		v_i.push_back(color[2]);									
		v_i.push_back(color[3]);									
		cfg_script.Add("Color",v_i);														
		sprintf(tmpText,"%d\0",abs(extraValue));
		cfg_script.Add("Text",tmpText);	
		cfg_script.Pop();
		index++;

		cfg_script.Add("Content");								
		cfg_script.Push("Content",index);							
		v_i.clear();											
		v_i.push_back(color[0]);									
		v_i.push_back(color[1]);									
		v_i.push_back(color[2]);									
		v_i.push_back(color[3]);									
		cfg_script.Add("Color",v_i);														
		cfg_script.Add("Text",string(")"));
		cfg_script.Pop();
	}
	cfg_script.Pop();
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJButtonAttackToPosMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJButtonAttackToPosMethod::crJXJButtonAttackToPosMethod(){}
crJXJButtonAttackToPosMethod::crJXJButtonAttackToPosMethod(const crJXJButtonAttackToPosMethod& handle):
	crMethod(handle)
{
}
void crJXJButtonAttackToPosMethod::inputParam(int i, void *param)
{
}

void crJXJButtonAttackToPosMethod::addParam(int i, const std::string& str)
{
}

void crJXJButtonAttackToPosMethod::operator()(crHandle &handle)
{
	//void *param;
	bool isatttack = true;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		unsigned short attackToPos;
		attackToPos = Cam_AttackToPos;	
		cameraData->inputParam(WCHDATA_JXJCameraOperator,&attackToPos);
	}
	handle.outputParam(0,&isatttack);
}


//////////////////////////////////////////////////////////////////////////
//
//crJXJSetRoleStopMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSetRoleStopMethod::crJXJSetRoleStopMethod():
	m_garrison(false){}
crJXJSetRoleStopMethod::crJXJSetRoleStopMethod(const crJXJSetRoleStopMethod &handle):
	m_garrison(handle.m_garrison)
{

}
void crJXJSetRoleStopMethod::inputParam(int i, void *param)
{

}
void crJXJSetRoleStopMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_garrison = (bool)(atoi(str.c_str()));
		break; 
	}
}
void crJXJSetRoleStopMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		crRole *myRole;
		crData *roleData;
		unsigned char targetType = Target_None;
		unsigned char itemstate = IS_Stop;
		unsigned char citemstate;
		PatrolPointVec *patrolPosVec;
		unsigned int *gs;
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//stream->createBuf(64);
		for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
			itr != selectMyRoles->end();
			++itr )
		{
			myRole = itr->get();
			roleData = myRole->getDataClass();
			roleData->getParam(WCHDATA_ItemState,param);
			citemstate = *(unsigned char *)param;
			if(citemstate != IS_Dead)
			{
				roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
				roleData->inputParam(WCHDATA_ItemState_rec, NULL);
				roleData->inputParam(WCHDATA_ItemState,&itemstate);
				roleData->inputParam(WCHDATA_TargetType,&targetType);
				roleData->getParam(WCHDATA_PatrolPointVec,param);
				patrolPosVec = (PatrolPointVec*)param;
				patrolPosVec->clear();
				roleData->getParam(WCHDATA_GuiseState,param);
				gs = (unsigned int *)param;
				if(m_garrison)
				{
					*gs |= GS_Garrison;
					*gs &= ~GS_Stop;
				}
				else
				{
					*gs |= GS_Stop;
					*gs &= ~GS_Garrison;
				}
				roleData->getParam(WCHDATA_CurActState,param);
				unsigned short* act = (unsigned short *)param;
				if (*act == ACT_Skill || *act == ACT_Attack || *act == ACT_CritAttack || *act == ACT_NearAttack)
				{//中断施法
					*act = ACT_Idle;
				}
				roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
				if(myRole->getPathFinder())
					myRole->getPathFinder()->dirty();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJTouXiangShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJTouXiangShowMethod::crJXJTouXiangShowMethod():m_this(NULL){}
crJXJTouXiangShowMethod::crJXJTouXiangShowMethod(const crJXJTouXiangShowMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas)
{

}

void crJXJTouXiangShowMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJTouXiangShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_imageBox_F1 = str;
		break;
	case 2:
		m_imageBox_F2 = str;
		break;
	case 3:
		m_imageBox_F3 = str;
		break;
	case 4:
		m_progress_F1 = str;
		break;
	case 5:
		m_progress_F2 = str;
		break;
	case 6:
		m_progress_F3 = str;
		break;
	}
}

void crJXJTouXiangShowMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if (canvas.valid())
	{
		crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
		ref_ptr<crImageBoxWidgetNode>  image_box_f1 = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_imageBox_F1));
		ref_ptr<crImageBoxWidgetNode>  image_box_f2 = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_imageBox_F2));
		ref_ptr<crImageBoxWidgetNode>  image_box_f3 = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_imageBox_F3));
		ref_ptr<crProgressWidgetNode>  progress_f1 = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progress_F1));
		ref_ptr<crProgressWidgetNode>  progress_f2 = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progress_F2));
		ref_ptr<crProgressWidgetNode>  progress_f3 = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progress_F3));

		image_box_f1->setVisiable(false);
		image_box_f2->setVisiable(false);
		image_box_f3->setVisiable(false);
		void *param = NULL;
		std::string iconfile1,iconfile2,iconfile3;
		/*unsigned short*/int armyCount,rtArmyCount;


		ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		crMyPlayerData *me = crMyPlayerData::getInstance();
		crRole *role = me->getLoadedRoleByIndex(0);//0,1,2
		crRole *role1 = me->getLoadedRoleByIndex(1);
		crRole *role2 = me->getLoadedRoleByIndex(2);
		int itemid1 = role->getAbstractItemID();

		crData *data = role->getDataClass();

		int iconid = troopsTab->getTitleIndex("icon");
		int iconid1 = troopsTab->getTitleIndex("icon灰");

		crTableIO::StrVec record1;
		troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid1),record1);
		if (role)
		{
			crData *roledata = role->getDataClass();
			void *param;
			unsigned int guisestate = GS_Normal;
			//role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			//if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
			if(rthp<=0.0f)
				iconfile1 = record1[iconid1];
			else
				iconfile1 = record1[iconid];
			image_box_f1->setVisiable(true);
			if(image_box_f1.valid())image_box_f1->setImageName(iconfile1);
			if(progress_f1.valid()) 
				progress_f1->setProgress(rtArmyCount/(float)armyCount);
		}
		if (role1)
		{
			int itemid2 = role1->getAbstractItemID();
			crTableIO::StrVec record2;
			troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid2),record2);		

			crData *roledata = role1->getDataClass();
			void *param;
			unsigned int guisestate = GS_Normal;
			//role1->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			//if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
			if(rthp <= 0.0f)
				iconfile2 = record2[iconid1];
			else
				iconfile2 = record2[iconid];
			image_box_f2->setVisiable(true);
			if(image_box_f2.valid())image_box_f2->setImageName(iconfile2);
			if(progress_f2.valid()) 
				progress_f2->setProgress(rtArmyCount/(float)armyCount);
		}
		if (role2)
		{
			int itemid3 = role2->getAbstractItemID();
			crTableIO::StrVec record3;
			troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid3),record3);

			crData *roledata = role2->getDataClass();
			void *param;
			unsigned int guisestate = GS_Normal;
			//role2->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));

			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;
			//if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
			if(rthp <= 0.0f)
				iconfile3 = record3[iconid1];	
			else
				iconfile3 = record3[iconid];	
			image_box_f3->setVisiable(true);
			if(image_box_f3.valid())image_box_f3->setImageName(iconfile3);
			if(progress_f3.valid()) 
				progress_f3->setProgress(rtArmyCount/(float)armyCount);
		}		
	}	
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJTouXiangUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJTouXiangUIUpdateMethod::crJXJTouXiangUIUpdateMethod():m_this(NULL){}
crJXJTouXiangUIUpdateMethod::crJXJTouXiangUIUpdateMethod(const crJXJTouXiangUIUpdateMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}

void crJXJTouXiangUIUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJTouXiangUIUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_switch_F1 = str;
		break;
	case 1:
		m_switch_F2 = str;
		break;
	case 2:
		m_switch_F3 = str;
		break;
	}
}

void crJXJTouXiangUIUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void  *param;
		ref_ptr<crMultiSwitch>  switchF1 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch_F1));
		ref_ptr<crMultiSwitch>  switchF2 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch_F2));
		ref_ptr<crMultiSwitch>  switchF3 = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch_F3));
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		crMyPlayerData *me = crMyPlayerData::getInstance();
		if(!cameraData || !me || !switchF1 || !switchF2 || !switchF3) return;
		switchF1->setActiveSwitchSet(0);
		switchF2->setActiveSwitchSet(0);
		switchF3->setActiveSwitchSet(0);

		crMyPlayerData::MyRoleNpcMap myRoleMap = me->getMyRoleNpcMap();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		int index = 0;
		for (SelectMyRoleVec::iterator itr_select = selectMyRoles->begin();
			itr_select!=selectMyRoles->end();itr_select++)
		{
			crMyPlayerData::MyRoleNpcMap::iterator itr_role = myRoleMap.begin();
			index = 0;
			while (itr_role!=myRoleMap.end())
			{
				if(itr_role->second.first->getRoleID() == itr_select->get()->getRoleID())
				{
					if(index==0)
						switchF1->setActiveSwitchSet(1);
					else if(index ==1)
						switchF2->setActiveSwitchSet(1);
					else if(index == 2)
						switchF3->setActiveSwitchSet(1);
					break;
				}
				itr_role++;
				index++;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJFaceIconShowRadioMethod
//
/////////////////////////////////////////////////////////////////////////////
crJXJFaceIconShowRadioMethod::crJXJFaceIconShowRadioMethod(){}
crJXJFaceIconShowRadioMethod::crJXJFaceIconShowRadioMethod(const crJXJFaceIconShowRadioMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_radio(handle.m_radio)
{

}
void crJXJFaceIconShowRadioMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}
void crJXJFaceIconShowRadioMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_radio = str;
		break;
	}
}

void crJXJFaceIconShowRadioMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		ref_ptr<crMultiSwitch>  page = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_radio));
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
			SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
			if (!selectMyRoles->empty())
			{
				if (selectMyRoles->size() == 1)
				{
					page->setActiveSwitchSet(0);
				}
				else
				{
					page->setActiveSwitchSet(1);
				}
			}
			else
			{
				crInstanceItem *selectItem = NULL;
				cameraData->getParam(WCHDATA_SelectItem,param);
				selectItem = (crInstanceItem *)param;
				if (selectItem)
				{
					char isEnemy = 0;
					crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(selectItem,&isEnemy));
					if(isEnemy == -1)
						page->setActiveSwitchSet(0);
					else if(isEnemy == 1)
						page->setActiveSwitchSet(0);
				}	
			}
		}	
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJMyArmyInfoShowMethod
//////////////////////////////////////////////////////////////////////////
crJXJMyArmyInfoShowMethod::crJXJMyArmyInfoShowMethod():m_this(NULL){}
crJXJMyArmyInfoShowMethod::crJXJMyArmyInfoShowMethod(const crJXJMyArmyInfoShowMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_imageFirst(handle.m_imageFirst),m_imageSecond(handle.m_imageSecond),m_imageThrid(handle.m_imageThrid),
	m_barFirst(handle.m_barFirst),m_barSecond(handle.m_barSecond),m_barThird(handle.m_barThird)
{

}
void crJXJMyArmyInfoShowMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode*)param;
		break;
	}
}

void crJXJMyArmyInfoShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_imageFirst = str;
		break;
	case 1:
		m_imageSecond = str;
		break;
	case 2:
		m_imageThrid = str;
		break;
	case 3:
		m_barFirst = str;
		break;
	case 4:
		m_barSecond = str;
		break;
	case 5:
		m_barThird = str;
		break;
	case 6:
		m_hpbar = str;
		break;
	}
}
void crJXJMyArmyInfoShowMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param = NULL;
		std::string iconfile1,iconfile2,iconfile3;
		float maxhp,rthp;
		/*unsigned short*/int armyCount,rtArmyCount;

		ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		crMyPlayerData *me = crMyPlayerData::getInstance();
		crRole *role = me->getLoadedRoleByIndex(0);//0,1,2
		crRole *role1 = me->getLoadedRoleByIndex(1);
		crRole *role2 = me->getLoadedRoleByIndex(2);
		int itemid1 = role->getAbstractItemID();

		crData *data = role->getDataClass();

		int iconid = troopsTab->getTitleIndex("icon");
		int iconid1 = troopsTab->getTitleIndex("icon灰");

		crData *cameraData = camera->getAttachedNode()->getDataClass();
		if (!cameraData)
			return;

		bool isRolePicked = false;
		bool isRole1Picked = false;
		bool isRole2Picked = false;
		int rolesCount = 0;
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		rolesCount = selectMyRoles->size();
		if (role) 
		{
			for (int i = 0; i < rolesCount; ++i)
			{
				if (role->getID() == selectMyRoles->at(i)->getID() && role->getRoleID() == selectMyRoles->at(i)->getRoleID())
				{
					isRolePicked = true;
					break;
				}
			}
		}
		if (role1) 
		{
			for (int i = 0; i < rolesCount; ++i)
			{
				if (role1->getID() == selectMyRoles->at(i)->getID() && role1->getRoleID() == selectMyRoles->at(i)->getRoleID())
				{
					isRole1Picked = true;
					break;
				}
			}
		}
		if (role2) 
		{
			for (int i = 0; i < rolesCount; ++i)
			{
				if (role2->getID() == selectMyRoles->at(i)->getID() && role2->getRoleID() == selectMyRoles->at(i)->getRoleID())
				{
					isRole2Picked = true;
					break;
				}
			}
		}

		ref_ptr<crImageBoxWidgetNode>  image[3];
		image[0] = dynamic_cast<crImageBoxWidgetNode*>(m_this->getWidget(m_imageFirst));
		image[1]  = dynamic_cast<crImageBoxWidgetNode*>(m_this->getWidget(m_imageSecond));
		image[2]  = dynamic_cast<crImageBoxWidgetNode*>(m_this->getWidget(m_imageThrid));
		ref_ptr<crStaticTextWidgetNode> bar[3];
		bar[0] = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_barFirst));
		bar[1] = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_barSecond));
		bar[2] = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_barThird));
		ref_ptr<crStaticTextWidgetNode>  hpbar = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_hpbar));
		for (int i = 0; i < 3; ++i)
		{
			if (bar[i].valid()) 
			{
				bar[i]->setColor(COLOR_GRAY);
				bar[i]->setCharacterSize(13);
				bar[i]->setString(std::string(""));
			}
			if (image[i].valid())
				image[i]->setVisiable(false);
		}

		int imageIndex = 0;
		crTableIO::StrVec record1;
		char temp[64];
		troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid1),record1);
		if (isRolePicked)
		{
			crData *roledata = role->getDataClass();
			void *param;
			roledata->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				iconfile1 = record1[iconid1];
			else
				iconfile1 = record1[iconid];

			maxhp = 0.0f;
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
			role->doEvent(WCH_GetHP,MAKECREPARAM(&maxhp,NULL));
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;

			roledata->getParam(WCHDATA_RTHP,param);
			rthp = *(float*)param;
			if (bar[imageIndex].valid()) 
			{
				sprintf(temp,"%d/%d\0",rtArmyCount,armyCount);
				bar[imageIndex]->setString(std::string(temp));
			}
			if (hpbar.valid())
			{
				hpbar->setString(std::string(temp));
				hpbar->setColor(COLOR_WHITE);
			}
			if(image[imageIndex].valid())
			{
				image[imageIndex]->setImageName(iconfile1);
				image[imageIndex]->setVisiable(true);
			}
			imageIndex++;
		}
		if (isRole1Picked)
		{
			int itemid2 = role1->getAbstractItemID();
			crTableIO::StrVec record2;
			troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid2),record2);		

			crData *roledata = role1->getDataClass();
			void *param;
			roledata->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				iconfile2 = record2[iconid1];
			else
				iconfile2 = record2[iconid];

			maxhp = 0.0f;
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
			role1->doEvent(WCH_GetHP,MAKECREPARAM(&maxhp,NULL));
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;

			roledata->getParam(WCHDATA_RTHP,param);
			rthp = *(float*)param;
			if (bar[imageIndex].valid())
			{
				sprintf(temp,"%d/%d\0",rtArmyCount,armyCount);
				bar[imageIndex]->setString(std::string(temp));
			}
			if(image[imageIndex].valid())
			{
				image[imageIndex]->setImageName(iconfile2);
				image[imageIndex]->setVisiable(true);
			}
			imageIndex++;
		}
		if (isRole2Picked)
		{
			int itemid3 = role2->getAbstractItemID();
			crTableIO::StrVec record3;
			troopsTab->queryOneRecord(0,crArgumentParser::appItoa(itemid3),record3);

			crData *roledata = role2->getDataClass();
			void *param;
			roledata->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *(unsigned char *)param;
			if(itemstate == IS_Dead)
				iconfile3 = record3[iconid1];	
			else
				iconfile3 = record3[iconid];	

			maxhp = 0.0f;
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,1));
			role2->doEvent(WCH_GetHP,MAKECREPARAM(&maxhp,NULL));
			//roledata->excHandle(MAKECREPARAM(WCH_LockData,0));
			roledata->getParam(WCHDATA_JXJArmyCount,param);
			armyCount = *(/*unsigned short*/int *)param;
			roledata->getParam(WCHDATA_JXJRTArmyCount,param);
			rtArmyCount = *(/*unsigned short*/int *)param;

			roledata->getParam(WCHDATA_RTHP,param);
			rthp = *(float*)param;
			if (bar[imageIndex].valid())
			{
				sprintf(temp,"%d/%d\0",rtArmyCount,armyCount);
				bar[imageIndex]->setString(std::string(temp));
			}
			if(image[imageIndex].valid())
			{
				image[imageIndex]->setImageName(iconfile3);
				image[imageIndex]->setVisiable(true);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
crJXJGetBattleRadioGroupValueMethod::crJXJGetBattleRadioGroupValueMethod():
	m_this(NULL)
{}

crJXJGetBattleRadioGroupValueMethod::crJXJGetBattleRadioGroupValueMethod(const crJXJGetBattleRadioGroupValueMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_radioGroup(handle.m_radioGroup)
{
}

void crJXJGetBattleRadioGroupValueMethod::inputParam(int i, void *param)
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

void crJXJGetBattleRadioGroupValueMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_radioGroup = str;
		break;
	}
}

void crJXJGetBattleRadioGroupValueMethod::operator()(crHandle &handle)
{
	ref_ptr<crMultiSwitch>  radioGroup = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_radioGroup));
	int value = radioGroup.valid() ? radioGroup->getActiveSwitchSet():0;
	handle.outputParam(0,&value);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleTotalArmyCountMethod
//
//////////////////////////////////////////////////////////////////////////
struct BattleGroupInfo
{
	unsigned char totalPlayers;
	unsigned short totalRoles;
	unsigned int totalRtArmyCount;
	unsigned int totalArmyCount;
};
crJXJBattleTotalArmyCountMethod::crJXJBattleTotalArmyCountMethod():m_this(NULL),m_totalPlayers(0),m_totalRoles(0)
{

}
crJXJBattleTotalArmyCountMethod::crJXJBattleTotalArmyCountMethod(const crJXJBattleTotalArmyCountMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_totalPlayers(0),
	m_totalRoles(0)
{

}

void crJXJBattleTotalArmyCountMethod::inputParam(int i, void *param)
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
		m_this = (CRNetApp::crRoom *)param;
		break;
	}
}
void crJXJBattleTotalArmyCountMethod::addParam(int i,const std::string& str)
{

}
void crJXJBattleTotalArmyCountMethod::operator ()(CRCore::crHandle &handle)
{
	void *param;
	crData *data;
	crRoomPlayer *member;
	int playerid;
	ref_ptr<crSceneServerPlayerData> playerData;
	crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetDataManager *netDataManager = sceneServer->getNetDataManager();
	ref_ptr<crRole> role;
	std::map<unsigned char,BattleGroupInfo> battleGroupMap;

	unsigned char totalPlayers = 0;
	unsigned short totalRoles = 0;
	m_this->lockPlayerList();
	crRoom::PlayerList &playerList = m_this->getPlayerList();
	for ( crRoom::PlayerList::iterator itr = playerList.begin();
		itr != playerList.end();
		++itr )
	{
		member = itr->get();
		playerid = member->getPlayerID();
		unsigned char key = member->getGroupID();
		playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
		if (playerData.valid())
		{
			battleGroupMap[key].totalPlayers+=1;
			totalPlayers++;
			crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
			for(crSceneServerPlayerData::RoleMap::iterator it = roleMap.begin();
				it!=roleMap.end();
				it++)
			{
				data = it->second->getDataClass();
				if(data)
				{
					data->getParam(WCHDATA_JXJRTArmyCount,param);
					/*unsigned short*/int armyRTCount = *(/*unsigned short*/int *)param;
					data->getParam(WCHDATA_JXJArmyCount, param);
					/*unsigned short*/int armyCount = *(/*unsigned short*/int *)param;
					if(armyRTCount>0)
					{
						battleGroupMap[key].totalRoles+=1;
						battleGroupMap[key].totalRtArmyCount += armyRTCount;
						battleGroupMap[key].totalArmyCount += armyCount;
						totalRoles++;
					}
				}
			}
		}
	}
	m_this->unlockPlayerList();

	//if((totalPlayers == m_totalPlayers) && (totalRoles == m_totalRoles)) return;
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	stream->createBuf(1+battleGroupMap.size()*12);
	stream->_writeUChar(battleGroupMap.size());
	for( std::map<unsigned char,BattleGroupInfo>::iterator it = battleGroupMap.begin();
		it!=battleGroupMap.end();it++)
	{
		stream->_writeUChar(it->first);
		stream->_writeUChar(it->second.totalPlayers);
		stream->_writeUShort(it->second.totalRoles);
		stream->_writeUInt(it->second.totalRtArmyCount);
		stream->_writeUInt(it->second.totalArmyCount);
	}
	ref_ptr<crPlayerDataEventPacket> packet = new crPlayerDataEventPacket;
	crPlayerDataEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvBattleArmyCount,stream.get());
	m_this->sendRoomMessage(packet.get());

	m_totalPlayers = totalPlayers;
	m_totalRoles = totalRoles;
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvBattleTotalArmyCountMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleTotalArmyCountMethod::crJXJRecvBattleTotalArmyCountMethod(){}
crJXJRecvBattleTotalArmyCountMethod::crJXJRecvBattleTotalArmyCountMethod(const crJXJRecvBattleTotalArmyCountMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{
	for (int i = 0; i < 7; ++i)
	{
		if (i < 2)
		{
			m_progressbarsw[i] = handle.m_progressbarsw[i];
		}
		m_progressbar[i] = handle.m_progressbar[i];
	}
}
void crJXJRecvBattleTotalArmyCountMethod::inputParam(int i, void *param)
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

void crJXJRecvBattleTotalArmyCountMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_strMyToTalPlayers = str;
		break;
	case 2:
		m_strMyTotalRoles = str;
		break;
	case 3:
		m_strEnemyTotalPlayers = str;
		break;
	case 4:
		m_strEnemyTotalRoles = str;
		break;
	case 5:
	case 6:
		m_progressbarsw[i - 5] = str;
		break;
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		m_progressbar[i - 7] = str;
		break;
	}
}

void crJXJRecvBattleTotalArmyCountMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			if(!crRunGameHandle::getInstance()->isInBattle()) return;
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			crData *gameData = m_this->getDataClass();
			if(canvas.valid() && gameData)
			{
				//////////////////////////////////////////////////////////////////////////
				ref_ptr<crStaticTextWidgetNode>  inputMyTotalPlayers = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strMyToTalPlayers));
				ref_ptr<crStaticTextWidgetNode>  inputMyTotalRoles = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strMyTotalRoles));
				ref_ptr<crStaticTextWidgetNode>  inputEnemyTotalPlayers = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strEnemyTotalPlayers));
				ref_ptr<crStaticTextWidgetNode>  inputEnemyTotalRoles = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strEnemyTotalRoles));
				ref_ptr<crMultiSwitch> progressbarsw[2] = { NULL };
				ref_ptr<crProgressWidgetNode> progressbar[7] = { NULL };

				for (int i = 0; i < 7; ++i)
				{
					if (i < 2)
					{
						progressbarsw[i] = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_progressbarsw[i]));
					}
					progressbar[i] = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progressbar[i]));
				}

				unsigned char id = 0;
				gameData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myshiliid = *(unsigned char *)param;

				crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
				if(room)
				{
					crData *roomData = room->getDataClass();
					if(roomData)
					{
						roomData->getParam(WCHDATA_JXJRoomType,param);
						unsigned char roomType = *(unsigned char *)param;
						if(roomType == 1)
						{
							roomData->getParam(WCHDATA_JXJBattleID,param);
							int battleid = *(int *)param;

							unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));//attack进攻方势力
							unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));//defend防守方势力
							unsigned char enemyID = (myshiliid == ashili)?dshili:ashili;//敌方势力

							std::map<unsigned char,BattleGroupInfo> battleGroupMap;
							BattleGroupInfo battleGroupInfo;
							unsigned char mapsize = m_stream->_readUChar();
							for (int i=0;i<mapsize;i++)
							{
								id = m_stream->_readUChar();
								battleGroupInfo.totalPlayers = m_stream->_readUChar();
								battleGroupInfo.totalRoles = m_stream->_readUShort();
								battleGroupInfo.totalRtArmyCount = m_stream->_readUInt();
								battleGroupInfo.totalArmyCount = m_stream->_readUInt();
								battleGroupMap.insert(make_pair(id,battleGroupInfo));
							}
							if(myshiliid == ashili)
							{
								if(inputMyTotalPlayers.valid())
								{
									//inputMyTotalPlayers->setColor(COLOR_GREEN);
									inputMyTotalPlayers->setString(crArgumentParser::appItoa(battleGroupMap[myshiliid].totalPlayers));
								}
								if(inputMyTotalRoles.valid())
								{
									//inputMyTotalRoles->setColor(COLOR_GREEN);
									std::string strRoleNum = crArgumentParser::appItoa(battleGroupMap[myshiliid].totalRoles);
									std::string strRTNum = crArgumentParser::appItoa(battleGroupMap[myshiliid].totalRtArmyCount);
									std::string strInfo = std::string("部队：") + strRoleNum + std::string("总兵力：") + strRTNum;

									inputMyTotalRoles->setString(strInfo);
								}

								if(inputEnemyTotalPlayers.valid())
								{
									//inputEnemyTotalPlayers->setColor(COLOR_RED);
									//inputEnemyTotalPlayers->setString(crArgumentParser::appItoa(battleGroupMap[enemyID].totalPlayers));
									inputEnemyTotalPlayers->setVisiable(false);
								}
								if(inputEnemyTotalRoles.valid())
								{
									//inputEnemyTotalRoles->setColor(COLOR_RED);
									//inputEnemyTotalRoles->setString(crArgumentParser::appItoa(battleGroupMap[enemyID].totalRoles));

									//std::string strRoleNum = crArgumentParser::appItoa(battleGroupMap[enemyID].totalRoles);
									//std::string strRTNum = crArgumentParser::appItoa(battleGroupMap[enemyID].totalRtArmyCount);
									//std::string strInfo = std::string("部队：") + strRoleNum + std::string("总兵力：") + strRTNum;
									int code = 1137;
									if (battleGroupMap[enemyID].totalPlayers == 0)
									{
										code = 1134;
									}
									else if (battleGroupMap[enemyID].totalPlayers <= 5)
									{
										code = 1135;
									}
									else if (battleGroupMap[enemyID].totalPlayers <= 10)
									{
										code = 1136;
									}
									ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									crTableIO::StrVec record;
									if (textTab->queryOneRecord(0, crArgumentParser::appItoa(code), record) >= 0)
									{
										inputEnemyTotalRoles->setString(record[1]);
									}
								}
							}
							else
							{
								if(inputMyTotalPlayers.valid())
								{
									//inputMyTotalPlayers->setColor(COLOR_GREEN);
									//inputMyTotalPlayers->setString(crArgumentParser::appItoa(battleGroupMap[enemyID].totalPlayers));
									inputMyTotalPlayers->setVisiable(false);
								}
								if(inputMyTotalRoles.valid())
								{
									//inputMyTotalRoles->setColor(COLOR_GREEN);
									//std::string strRoleNum = crArgumentParser::appItoa(battleGroupMap[enemyID].totalRoles);
									//std::string strRTNum = crArgumentParser::appItoa(battleGroupMap[enemyID].totalRtArmyCount);
									//std::string strInfo = std::string("部队：") + strRoleNum + std::string("总兵力：") + strRTNum;

									//inputMyTotalRoles->setString(strInfo);
									int code = 1137;
									if (battleGroupMap[enemyID].totalPlayers == 0)
									{
										code = 1134;
									}
									else if (battleGroupMap[enemyID].totalPlayers <= 5)
									{
										code = 1135;
									}
									else if (battleGroupMap[enemyID].totalPlayers <= 10)
									{
										code = 1136;
									}
									ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
									crTableIO::StrVec record;
									if (textTab->queryOneRecord(0, crArgumentParser::appItoa(code), record) >= 0)
									{
										inputMyTotalRoles->setString(record[1]);
									}
								}

								if(inputEnemyTotalPlayers.valid())
								{
									//inputEnemyTotalPlayers->setColor(COLOR_RED);
									inputEnemyTotalPlayers->setString(crArgumentParser::appItoa(battleGroupMap[myshiliid].totalPlayers));
								}
								if(inputEnemyTotalRoles.valid())
								{
									//inputEnemyTotalRoles->setColor(COLOR_RED);
									//inputEnemyTotalRoles->setString(crArgumentParser::appItoa(battleGroupMap[enemyID].totalRoles));

									std::string strRoleNum = crArgumentParser::appItoa(battleGroupMap[myshiliid].totalRoles);
									std::string strRTNum = crArgumentParser::appItoa(battleGroupMap[myshiliid].totalRtArmyCount);
									std::string strInfo = std::string("部队：") + strRoleNum + std::string("总兵力：") + strRTNum;

									inputEnemyTotalRoles->setString(strInfo);
								}

							}
							for (int i = 0; i < 7; ++i)
							{
								if (progressbar[i].valid())
								{
									progressbar[i]->setProgress(1.0f);
								}
							}
							if(myshiliid == ashili)
							{
								if (progressbarsw[0].valid())
								{
									progressbarsw[0]->setActiveSwitchSet(myshiliid - c_startShiliID);
								}
								if (progressbarsw[1].valid())
								{
									if (enemyID >= c_startShiliID)
									{
										progressbarsw[1]->setActiveSwitchSet(enemyID - c_startShiliID);
									}
									else
									{
										progressbarsw[1]->setActiveSwitchSet(3);
									}
								}
							}
							else
							{
								if (progressbarsw[1].valid())
								{
									progressbarsw[1]->setActiveSwitchSet(myshiliid - c_startShiliID);
								}
								if (progressbarsw[0].valid())
								{
									if (enemyID >= c_startShiliID)
									{
										progressbarsw[0]->setActiveSwitchSet(enemyID - c_startShiliID);
									}
									else
									{
										progressbarsw[0]->setActiveSwitchSet(3);
									}
								}
							}
						}
					}
				}
			}	
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJButtonMoveToPosMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJButtonMoveToPosMethod::crJXJButtonMoveToPosMethod(){}
crJXJButtonMoveToPosMethod::crJXJButtonMoveToPosMethod(const crJXJButtonMoveToPosMethod& handle):
	crMethod(handle)
{
}
void crJXJButtonMoveToPosMethod::inputParam(int i, void *param)
{
}

void crJXJButtonMoveToPosMethod::addParam(int i, const std::string& str)
{
}

void crJXJButtonMoveToPosMethod::operator()(crHandle &handle)
{
	//	void *param;
	bool isMove = false;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		//cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		unsigned short moveToPos;// = *(unsigned short *)param;

		moveToPos = Cam_MoveToPos;
		isMove = true;	

		cameraData->inputParam(WCHDATA_JXJCameraOperator,&moveToPos);
	}
	handle.outputParam(0,&isMove);
}

/////////////////////////////////////////
//
//crJXJCancelMoveToPosMethod
//
/////////////////////////////////////////
crJXJCancelMoveToPosMethod::crJXJCancelMoveToPosMethod():
	m_this(NULL){}
crJXJCancelMoveToPosMethod::crJXJCancelMoveToPosMethod(const crJXJCancelMoveToPosMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCancelMoveToPosMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJCancelMoveToPosMethod::addParam(int i, const std::string& str)
{
}

void crJXJCancelMoveToPosMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		unsigned short moveToPos;

		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		moveToPos = *(unsigned short*)param;
		if(moveToPos == Cam_MoveToPos)
		{
			moveToPos = Cam_Default;	
			cameraData->inputParam(WCHDATA_JXJCameraOperator,&moveToPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJMinMapSignalPosMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJMinMapSignalPosMethod::crJXJMinMapSignalPosMethod():m_this(NULL)
{

}
crJXJMinMapSignalPosMethod::crJXJMinMapSignalPosMethod(const crJXJMinMapSignalPosMethod &handle):
	crMethod(handle)
{

}

void crJXJMinMapSignalPosMethod::inputParam(int i, void *param)
{
}
void crJXJMinMapSignalPosMethod::addParam(int i,const std::string& str)
{

}
void crJXJMinMapSignalPosMethod::operator ()(CRCore::crHandle &handle)
{

	ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
	bool issign = false;
	if(crRunGameHandle::getInstance()->isInGame())
	{
		crCamera *camera = crCameraManager::getInstance()->getCurrentCamera();
		crData *cameraData = camera->getAttachedNode()->getDataClass();

		unsigned short signal = Cam_Signal;
		issign = true;

		cameraData->inputParam(WCHDATA_JXJCameraOperator,&signal);
		handle.outputParam(0,&issign);
	}
}

///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
crJXJIsSignalMethod::crJXJIsSignalMethod(){}
crJXJIsSignalMethod::crJXJIsSignalMethod(const crJXJIsSignalMethod& handle):
	crMethod(handle)
{
}
void crJXJIsSignalMethod::inputParam(int i, void *param)
{
}

void crJXJIsSignalMethod::addParam(int i, const std::string& str)
{
}

void crJXJIsSignalMethod::operator()(crHandle &handle)
{
	unsigned short signal;
	bool issign = false;
	void *param;
	crCamera *camera = crCameraManager::getInstance()->getCurrentCamera();
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	if(cameraData)
	{
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);

		if(param)
			signal = *(unsigned short*)param;
		issign = (signal == Cam_Signal)?true:false;
		//cameraData->inputParam(WCHDATA_JXJCameraOperator,NULL);
	}
	handle.outputParam(0,&issign);
}

///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
crJXJIsGameServerMethod::crJXJIsGameServerMethod(){}
crJXJIsGameServerMethod::crJXJIsGameServerMethod(const crJXJIsGameServerMethod& handle):
	crMethod(handle)
{
}
void crJXJIsGameServerMethod::inputParam(int i, void *param)
{
}

void crJXJIsGameServerMethod::addParam(int i, const std::string& str)
{
}

void crJXJIsGameServerMethod::operator()(crHandle &handle)
{
	bool isServer = !crGlobalHandle::isClient();
	handle.outputParam(0,&isServer);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCancelSignalMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCancelSignalMethod::crJXJCancelSignalMethod():
	m_this(NULL){}
crJXJCancelSignalMethod::crJXJCancelSignalMethod(const crJXJCancelSignalMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJCancelSignalMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJCancelSignalMethod::addParam(int i, const std::string& str)
{
}

void crJXJCancelSignalMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void* param;
		unsigned short attackToPos;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		attackToPos = *(unsigned short*)param;
		if(attackToPos == Cam_Signal)
		{
			attackToPos = Cam_Default;
			cameraData->inputParam(WCHDATA_JXJCameraOperator,&attackToPos);
		}	
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCancelPatrolMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCancelPatrolMethod::crJXJCancelPatrolMethod() :
m_this(NULL){}
crJXJCancelPatrolMethod::crJXJCancelPatrolMethod(const crJXJCancelPatrolMethod& handle) :
crMethod(handle),
m_this(NULL)
{
}
void crJXJCancelPatrolMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJCancelPatrolMethod::addParam(int i, const std::string& str)
{
}

void crJXJCancelPatrolMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void* param;
		unsigned short attackToPos;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJCameraOperator, param);
		attackToPos = *(unsigned short*)param;
		if (attackToPos == Cam_Patrol)
		{
			attackToPos = Cam_Default;
			cameraData->inputParam(WCHDATA_JXJCameraOperator, &attackToPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJSignalShowControlMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSignalShowControlMethod::crJXJSignalShowControlMethod(){}
crJXJSignalShowControlMethod::crJXJSignalShowControlMethod(const crJXJSignalShowControlMethod & handle){}
void crJXJSignalShowControlMethod::inputParam(int i, void *param)
{
}
void crJXJSignalShowControlMethod::addParam(int i, const std::string& str){}
void crJXJSignalShowControlMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ref_ptr<CRCore::crWidgetNode> signalInfo;

		if (cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJSignalsMap, param);
			int i = 0;
			if (param)
			{
				SignalNodeMap *signalMap = (SignalNodeMap *)param;

				SignalNodeMap::iterator itr_signal = signalMap->begin();

				for (; itr_signal != signalMap->end(); itr_signal++)
				{

					if (itr_signal->second == 0.0f)
					{
						continue;
					}

					itr_signal->second -= 1.0f;

					signalInfo = itr_signal->first;
					if (itr_signal->second <= 0.0f)
					{
						itr_signal->second = 0.0f;
						signalInfo->setVisiable(false);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvSignalsPosMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvSignalsPosMethod::crJXJRecvSignalsPosMethod(){}
crJXJRecvSignalsPosMethod::crJXJRecvSignalsPosMethod(const crJXJRecvSignalsPosMethod &handle)
{

}
void crJXJRecvSignalsPosMethod::inputParam(int i, void *param)
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
void crJXJRecvSignalsPosMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strMiniMap = str;
		break;
	case 2:
		m_strSignal = str;
		break;
	case 3:
		m_strRotMap = str;
		break;
	default:
		break;
	}
}

void crJXJRecvSignalsPosMethod::operator ()(crHandle &handle)
{
	//void *param;
	if(m_this && m_stream.valid())
	{
		if(m_netType == SceneServer)
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				crRoom *room = callback->findRoom(playerData->getRoomID());
				if(room)
				{
					crRoomPlayer *member = room->getMember(m_this->getPlayerID());
					if(member)
					{
						////bool isminimap = m_stream->_readBool();
						crVector3 pos = m_stream->_readVec3();
						ref_ptr<crStreamBuf> posStream = new crStreamBuf;
						posStream->createBuf(12);
						////posStream->_writeBool(isminimap);
						posStream->_writeVec3(pos);//4*3

						ref_ptr<crPlayerDataSceneEventPacket> packet = new crPlayerDataSceneEventPacket;
						crPlayerDataSceneEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvSignalsPos,posStream.get());
						room->sendRoomMessage(packet.get(),member->getGroupID(),m_this->getPlayerID());
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			if(crRunGameHandle::getInstance()->isInBattle() && crMyPlayerData::getInstance()->getPlayerGameData() && crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass())
			{
				//bool isminimap = m_stream->_readBool();
				crVector3 hitpos = m_stream->_readVec3();
				unsigned char signaltype = Signal_Def;
				if(!m_stream->eof())
					signaltype = m_stream->_readUChar();
				std::string signalname;
				float interval = 10.0f;
				switch (signaltype)
				{
				case Signal_Def:
					signalname = m_strSignal;
					interval = 10.0f;
					break;
				case Signal_ItemPos:
					signalname = m_strSignal;
					interval = 3.0f;
					break;
				}
				void *param;
				ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
				if(camera.valid())
				{
					crData *cameraData = camera->getAttachedNode()->getDataClass();
					CRCore::ref_ptr<CRCore::crWidgetNode> icon;
					CRCore::ScopedLock<crData> lock(*cameraData);
					cameraData->getParam(WCHDATA_JXJSignalsMap,param);
					if(param)
					{
						SignalNodeMap *signaldeque = (SignalNodeMap *)param;
						ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();	
						ref_ptr<crWidgetNode>  iconWidget;	
						ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
						ref_ptr<crWidgetNode>  miniMap = canvas->getWidget(m_strMiniMap);
						ref_ptr<crWidgetNode>  signalWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(signalname));
						ref_ptr<crWidgetNode>  rotMapWidget = canvas->getWidget(m_strRotMap);
						SignalNodeMap::iterator itr_w = signaldeque->begin();
						while (itr_w != signaldeque->end())
						{
// 							if(signaldeque->size()>=20 && itr_w->first->getName().compare(m_strSignal)==0)
// 							{
// 								iconWidget = itr_w->first;
// 								itr_w->second = 10.0f;
// 								break;
// 							}
							if (itr_w->first->getVisiable() == false && \
								itr_w->first->getName().compare(signalname) == 0)
							{
								iconWidget = itr_w->first;
								itr_w->second = interval;
								break;
							}
							itr_w++;
						}

						if (itr_w == signaldeque->end() && signaldeque->size() <= 20)
						{
							iconWidget = dynamic_cast<crWidgetNode *>(signalWidget->clone(crCopyOp::DEEP_COPY_NODES));
							signaldeque->insert(std::make_pair(iconWidget, 10.0f));
							crLoadManager::getInstance()->requestAddNode(canvas.get(),iconWidget.get(),false);
						}
							

						if (iconWidget.valid())
						{
							//if (isminimap)
							//{
							//	iconWidget->setMatrix(crMatrix::translate(pos));	
							//	crVector3 centerCoord = miniMap->getBound().center();
							//	const crBoundingBox &bbox = miniMap->getBoundBox();
							//	crVector2 mapWidgetSize(bbox.xLength(),bbox.yLength());
							//	crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);
							//	crVector3 hitPos;
							//	hitPos[0] = (pos[0] - centerCoord[0])/mapFactor[0];
							//	hitPos[1] = (pos[1] - centerCoord[1])/mapFactor[1];

							//	camera->getAttachedNode()->doEvent(WCH_JXJPlaySignalFx,MAKECREPARAM(&hitPos,NULL));
							//}
							//else
							//{
								//pos = pos*rotMapWidget->getMatrix();
							crVector2 mapWidgetSize;
							const crBoundingBox &bbox = miniMap->getBoundBox();
							mapWidgetSize.set(bbox.xLength(),bbox.yLength());
							crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);
							crVector3 pos;
							float px = hitpos.x() * mapFactor[0];
							float py = hitpos.y() * mapFactor[1];
							crVector3 dir = miniMap->getBound().center();
							pos = dir+crVector3(px,py,0.0f);
							pos = pos*rotMapWidget->getMatrix();
							const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
							const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
							iconWidget->setMatrix(scaleMat*invScaleMat*crMatrix::translate(pos));
							iconWidget->setVisiable(true);
								//const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
								//const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
								//iconWidget->setMatrix(scaleMat*invScaleMat*crMatrix::translate(pos));
							camera->getAttachedNode()->doEvent(WCH_JXJPlaySignalFx,MAKECREPARAM(&hitpos,NULL));
							//}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
////
////crJXJWindowMouseHook
////
////////////////////////////////////////////////////////////////////////////
////
//
//crJXJWindowMouseHookMethod::crJXJWindowMouseHookMethod(){}
//crJXJWindowMouseHookMethod::crJXJWindowMouseHookMethod(const crJXJWindowMouseHookMethod &handle):m_bHook(handle.m_bHook){}
//void crJXJWindowMouseHookMethod::inputParam(int i, void *param){}
//void crJXJWindowMouseHookMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_bHook = atoi(str.c_str());
//		break;
//	}
//}
//void crJXJWindowMouseHookMethod::operator ()(crHandle &handle)
//{
//#ifndef _DEBUG
//	HWND hWnd = crKeyboardMouseHandle::getInstance()->getBindViewer()->getCameraWindow();
//	HINSTANCE m_hook = LoadLibrary("mousehook.dll");
//	if(m_hook)
//	{
//		if(m_bHook)
//		{
//			INITPROC hook = (INITPROC)GetProcAddress(m_hook,"Inithook");
//			hook(hWnd);
//		}
//		else
//		{
//			ENDHOOKPROC endhook = (ENDHOOKPROC)GetProcAddress(m_hook,"Endhook");
//			endhook();
//		}
//	}
//#endif	
//}
//////////////////////////////////////////////////////////////////////////
//crJXJButtonRolePatrolMethod
//////////////////////////////////////////////////////////////////////////
crJXJButtonRolePatrolMethod::crJXJButtonRolePatrolMethod(){}
crJXJButtonRolePatrolMethod::crJXJButtonRolePatrolMethod(const crJXJButtonRolePatrolMethod &handle){}
void crJXJButtonRolePatrolMethod::inputParam(int i, void *param)
{

}
void crJXJButtonRolePatrolMethod::addParam(int i, const std::string& str)
{

}
void crJXJButtonRolePatrolMethod::operator ()(crHandle &handle)
{
	bool isPatrol = false;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		unsigned short patrol;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		patrol = Cam_Patrol;
		isPatrol = true;	

		cameraData->inputParam(WCHDATA_JXJCameraOperator,&patrol);
	}
	handle.outputParam(0,&isPatrol);
}
//////////////////////////////////////////////////////////////////////////
//crJXJChooseBirthPointTimerMethod
//////////////////////////////////////////////////////////////////////////
crJXJChooseBirthPointTimerMethod::crJXJChooseBirthPointTimerMethod():
	m_this(NULL)
{

}
crJXJChooseBirthPointTimerMethod::crJXJChooseBirthPointTimerMethod(const crJXJChooseBirthPointTimerMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJChooseBirthPointTimerMethod::inputParam(int i, void *param)
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
		m_this = (crStaticTextWidgetNode*)param;
		break;
	}
}
void crJXJChooseBirthPointTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJChooseBirthPointTimerMethod::operator ()(crHandle &handle)
{
	bool bExit = true;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJBattleChoosePointTimer,param);
		short timer = *(short*)param;
		char str[4];
		sprintf(str,"%02d\0",timer-1);
		m_this->setString(string(str));
		if(timer < 0)
			bExit = false;
		if(timer>=0)
			timer--;
		cameraData->inputParam(WCHDATA_JXJBattleChoosePointTimer,&timer);
	}
	handle.outputParam(0,&bExit);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCardInfoShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCardInfoShowMethod::crJXJCardInfoShowMethod():m_this(NULL)
{

}
crJXJCardInfoShowMethod::crJXJCardInfoShowMethod(const crJXJCardInfoShowMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_equipIndex(handle.m_equipIndex),
	m_isShow(handle.m_isShow),
	m_index(handle.m_index),
	m_tipCanvas(handle.m_tipCanvas),
	m_name(handle.m_name),
	m_SkillName(handle.m_SkillName),
	m_infoCanvas(handle.m_infoCanvas),
	m_infoBack(handle.m_infoBack),
	m_infoDi(handle.m_infoDi),
	m_info(handle.m_info),
	m_offsetx(handle.m_offsetx),
	m_offsety(handle.m_offsety)
{

}
void  crJXJCardInfoShowMethod::inputParam(int i, void *param)
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
		m_this = (crImageBoxWidgetNode*)param;
		break;
	}
}
void crJXJCardInfoShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_equipIndex = atoi(str.c_str());
		break;
	case 1:
		m_isShow = atoi(str.c_str());
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	case 3:
		m_tipCanvas = str;
		break;
	case 4:
		m_name = str;
		break;
	case 5:
		m_SkillName = str;
		break;
	case 6:
		m_infoCanvas = str;
		break;
	case 7:
		m_infoBack = str;
		break;
	case 8:
		m_infoDi = str;
		break;
	case 9:
		m_info = str;
		break;
	case 10:
		m_offsetx = atof(str.c_str());
		break;
	case 11:
		m_offsety = atof(str.c_str());
		break;
	//case 9:
	//	m_inputCd = str;
	//	break;
	//case 10:
	//	m_inputHotKey = str;
	//	break;
	//case 11:
	//	m_skillCdTime = str;
	//	break;
	//case 12:
	//	m_skilHotkey = str;
	//	break;

	}
}
void crJXJCardInfoShowMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  tipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tipCanvas);
	ref_ptr<crCanvasNode>  infoCanvas = crFilterRenderManager::getInstance()->findCanvas(m_infoCanvas);
	if(!tipCanvas || !infoCanvas) return;
	//crBoundingBox box = m_this->getBoundBox();
	//crVector3 pos = box.center();
	bool isvisiable = m_this->getVisiable();
	void *param;
	if(m_isShow && isvisiable)
	{
		//canvas->setParentWidget(m_this);
		ref_ptr<crStaticTextWidgetNode>  cardName = dynamic_cast<crStaticTextWidgetNode *>(tipCanvas->getWidget(m_name));
		ref_ptr<crStaticTextWidgetNode>  skillName = dynamic_cast<crStaticTextWidgetNode *>(tipCanvas->getWidget(m_SkillName));
		ref_ptr<crMatrixTransform>infobk = dynamic_cast<crMatrixTransform *>(infoCanvas->getChildNode(m_infoBack));
		ref_ptr<crMatrixTransform>infodi = dynamic_cast<crMatrixTransform *>(infoCanvas->getChildNode(m_infoDi));
		ref_ptr<crHypertextWidgetNode>  info2 = dynamic_cast<crHypertextWidgetNode *>(infoCanvas->getWidget(m_info));
		//ref_ptr<crStaticTextWidgetNode>  cardCD = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_inputCd));
		//ref_ptr<crStaticTextWidgetNode>  hotkey = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_inputHotKey));
		//ref_ptr<crStaticTextWidgetNode>  cdTime = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_skillCdTime));
		//ref_ptr<crStaticTextWidgetNode>  skillHotKey = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_skilHotkey));

		if(cardName.valid())
			cardName->setAlignment(CRText::crText::CENTER_BASE_LINE);
		//if(cardCD.valid())
		//	cardCD->setString(std::string("冷却时间："));
		//if(hotkey.valid())
		//	hotkey->setString(std::string("快捷键："));

		//if(skillHotKey.valid())
		//{
		//	std::string hot_key;
		//	if(m_index == 0) hot_key = string("Q");
		//	else if(m_index == 1) hot_key = string("W");
		//	else if(m_index == 2) hot_key = string("E");
		//	skillHotKey->setString(hot_key);
		//}

		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(!itemtab.valid()) return;
		ref_ptr<crTableIO> skilltab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
		int nameid = itemtab->getTitleIndex("name");
		int skillid = itemtab->getTitleIndex("abstractid");
		//int jinengmiaoshuid = itemtab->getTitleIndex("技能描述");
		int skillNameid = skilltab->getTitleIndex("名字");
		//int skillCdid = skilltab->getTitleIndex("CD");

		crMyPlayerData *me = crMyPlayerData::getInstance();
		crData *playerdata = me->getPlayerGameData()->getDataClass();
		if(!playerdata) return;
		playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
		playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *myteam = (FormationInfoVec *)param;
		crVector2i equips = crVector2i(-1,-1);
		crTableIO::StrVec itemrecord;
		crTableIO::StrVec skillrecord;
		int index = 0;
		crMyPlayerData::MyRoleNpcMap roleMap =  crMyPlayerData::getInstance()->getMyRoleNpcMap();
		for(crMyPlayerData::MyRoleNpcMap::iterator itr_role = roleMap.begin();itr_role!=roleMap.end();itr_role++,index++)
		{
			if(index == m_index)
			{
				for(FormationInfoVec::iterator itr = myteam->begin();itr!=myteam->end();itr++)
				{
					if(itr->get()->getRoleID()==itr_role->first)
					{
						equips = itr->get()->getEquips();
						break;
					}
				}
				break;
			}
		}
		if(equips[m_equipIndex]>0)
		{
			if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[m_equipIndex]),itemrecord)>=0)
			{
				//std::string file = itemrecord[jinengmiaoshuid];
				//if(!file.empty())
				//{
				//	skillInfo->clearText();
				//	file = crMyPlayerData::getInstance()->getSelectedGame()->getTextDir() + file;
				//	skillInfo->setHypertext(file);
				//}
				//else
				//{
				//	skillInfo->clearText();
				//}
				if(info2.valid())
				{
					ItemMap itemMap;
					rcfg::ConfigScript outscript(&itemMap);
					crVector2i item(equips[m_equipIndex],0);
					me->getPlayerGameData()->doEvent(WCH_JXJCreateItemTips,MAKECREPARAM(&item,&outscript));
					info2->setHypertext(outscript);
				}

				cardName->setString(itemrecord[nameid]);

				if(skilltab->queryOneRecord(0,itemrecord[skillid],skillrecord)>=0)
				{
					//int cd_time = atoi(skillrecord[skillCdid].c_str());
					//if(cdTime.valid())
					//	cdTime->setString(crArgumentParser::appItoa(cd_time)+std::string("秒"));

					if(skillName.valid())
						skillName->setString(skillrecord[skillNameid]);
				}
			}
		}
		playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
		///////////计算缩放
		float diYlength = 0.0f;
		float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
		info2->updateData();
		const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
		const crBoundingBox &bbox2 = info2->getBoundBox();
		float texheight = 0.0f;
		float needmove = 0.0f;
		float uiscale = crDisplaySettings::instance()->getUIMeterScale();
		if (bbox.valid())
		{
			texheight = bbox.yLength();
			texheight *= uiscale;
			if (texheight < boxYlength) texheight = boxYlength;
			float scale = texheight / boxYlength;

			const crBoundingBox &tipsbb = tipCanvas->getBoundBox();
			crVector3 centerTansTar = info2->getBound().center();

			float tmpLength = bbox2.yLength();
			if (infodi.valid())
			{
				float diYlength = infodi->getChild(0)->getBoundBox().yLength();
				centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
				//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
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
			if (infobk.valid())infobk->setMatrix(rotmat);

			if (infodi.valid())
			{
				diYlength = infodi->getChild(0)->getBoundBox().yLength();
				//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
				needmove = boxYlength - texheight + diYlength * 0.25f;
				//needmove = boxYlength - texheight;
				infodi->setMatrix(crMatrix::translate(0.0f, needmove/*+c_tipsoffsets[2]*scale*/, 0.0f));
				diYlength = infodi->getChild(0)->getBoundBox().yLength();
			}
		}
		else
		{
			infobk->setMatrix(crMatrix::scale(1.0f, 1.0f/*m_expandnum*/, 1.0f) * crMatrix::translate(0.0f, 0.0f, 0.0f));
			if (infodi.valid()) infodi->setMatrix(crMatrix::translate(0.0f, 0.0f, 0.0f));
		}

		float xlength = tipCanvas->getBoundBox().xLength();
		//const crBoundingBox &canvasbox = tipCanvas->getBoundBox();
		const crBoundingBox &thisbbox = m_this->getBoundBox();
		crVector3 pos(thisbbox.center().x() + xlength / 2 + m_offsetx * uiscale, thisbbox.yMax() + diYlength + texheight + m_offsety * uiscale, 0.0f);
		tipCanvas->setPosition(pos);
		infoCanvas->setPosition(pos);

		tipCanvas->setCanFocus(false);
		infoCanvas->setCanFocus(false);
	}
	crFilterRenderManager::getInstance()->showCanvas(tipCanvas.get(), m_isShow);
	crFilterRenderManager::getInstance()->showCanvas(infoCanvas.get(), m_isShow);
}

//////////////////////////////////////////////////////////////////////////
//crJXJCardPushMethod
//////////////////////////////////////////////////////////////////////////
crJXJCardPushMethod::crJXJCardPushMethod():
	m_index(0){}
crJXJCardPushMethod::crJXJCardPushMethod(const crJXJCardPushMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJCardPushMethod::inputParam(int i, void *param)
{
}

void crJXJCardPushMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJCardPushMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(camera.valid() && itemtab.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		//cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		//SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		crVector2i equips = crVector2i(-1,-1);
		crTableIO::StrVec itemrecord;
		crMyPlayerData *me = crMyPlayerData::getInstance();
		crData *playerdata = me->getPlayerGameData()->getDataClass();
		playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec *myteam = (FormationInfoVec *)param;

		crRole *role = crMyPlayerData::getInstance()->getLoadedRoleByIndex(m_index);
		if (role)
		{
			//crRole *role = (*selectMyRoles)[0].get();
			crData *roleData = role->getDataClass();
			int roleid = role->getRoleID();
			roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
			roleData->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *((unsigned char*)param);
			unsigned int guisestate = GS_Normal;
			role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			roleData->getParam(WCHDATA_RTHP,param);
			float rthp = *(float*)param;

			if( itemstate != IS_Dead && rthp>0.0f && 
				!(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
				!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
				/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
				/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
				!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
				!(guisestate & GS_ExtraShifting)/* && !(guisestate & GS_NoAttack)*/ )
			{ //不需要判断 itemid 是否为0，在此之前并未设置itemid
				int abstractID = 0;
				int aboutUseID = 0;
				for( FormationInfoVec::iterator itr = myteam->begin();
					itr != myteam->end();
					++itr )
				{
					if(roleid == (*itr)->getRoleID())
					{
						equips = (*itr)->getEquips();
						break;
					}
				}
				if(equips[0]>0)
				{
					itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord) ;
					if(itemrecord.size()>0)
					{
						abstractID =atoi(itemrecord[itemtab->getTitleIndex("abstractid")].c_str());

					}
				}
				if(abstractID!=0)
				{
					roleData->getParam(WCHDATA_EquipOnMap,param);
					EquipOnMap *equipOnMap = (EquipOnMap *)param;
					EquipOnMap::iterator itr_map = equipOnMap->find(abstractID);
					int itemid;
					if(itr_map!=equipOnMap->end())
					{
						itemid = itr_map->second;
					}
					crItemChild *itemChild = role->findChildItem(itemid);
					if(itemChild && itemChild->isLoaded())
					{
						crInstanceItem *item  = itemChild->getInstanceItem();
						aboutUseID = item->getInstanceItemID();
						UseItemParam useItemParam;
						useItemParam.m_user = role;
						useItemParam.m_target = NULL;
						unsigned short useResult = UR_None;
						item->doEvent(WCH_UseItemCheck,MAKECREPARAM(&useItemParam,&useResult));
						if(useResult == UR_Succeed)
						{
							//roleData->inputParam(WCHDATA_AboutToUseItemID,&aboutUseID);
							AboutUseItemPair useItempair(role,aboutUseID);
							cameraData->inputParam(WCHDATA_JXJRoleAboutToUseItemID,&useItempair);
						}
					}
				}
			}
			roleData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//
//crJXJCardPushUseSkillMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCardPushUseSkillMethod::crJXJCardPushUseSkillMethod(){}
crJXJCardPushUseSkillMethod::crJXJCardPushUseSkillMethod(const crJXJCardPushUseSkillMethod& handle):
	crMethod(handle)
{
}
void crJXJCardPushUseSkillMethod::inputParam(int i, void *param)
{
}

void crJXJCardPushUseSkillMethod::addParam(int i, const std::string& str)
{
}

void crJXJCardPushUseSkillMethod::operator()(crHandle &handle)
{
	int _case = 0;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJCameraOperator,param);
		unsigned short attackToPos = *(unsigned short *)param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		if(attackToPos == Cam_Default)
		{
			// 		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
			// 		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
			cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
			AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
			crRole *role = useItemPair->first.get();
			if(role)
			{
				//crRole *role = (*selectMyRoles)[0].get();
				crData *roleData = role->getDataClass();
				//roleData->getParam(WCHDATA_AboutToUseItemID,param);
				int itemid = useItemPair->second;//*((int*)param);
				unsigned int guisestate = GS_Normal;
				role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));

				if(itemid != 0 && !(guisestate & GS_Silent) && !(guisestate & GS_Confusion) && 
					!(guisestate & GS_Sheep) && !(guisestate & GS_OnHorse) && 
					/*!(guisestate & GS_Disarm) &&*/ !(guisestate & GS_Scare) && 
					/*!(guisestate & GS_Nihility) &&*/ !(guisestate & GS_Swoon) &&
					!(guisestate & GS_Stagnate) && !(guisestate & GS_Taunt) && 
					!(guisestate & GS_ExtraShifting)/* && !(guisestate & GS_NoAttack)*/ )
				{
					crItemChild *itemChild = role->findChildItem(itemid);
					if(itemChild && itemChild->isLoaded())
					{

						unsigned short range = 0;
						crInstanceItem *item = itemChild->getInstanceItem();
						crData *itemData = item->getDataClass();
						crVector3 pos;
						crMatrixTransform *bot;
						itemData->getParam(WCHDATA_ItemUseRange,param);
						if(param)
						{
							range = *(unsigned short*)param;
							if(range == 0)
							{
								//直接释放技能
								bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
								if(bot && bot->getVisiable())
								{
									pos = role->getPosition()+role->getDir() * 0.5f;
									unsigned char itemstate = IS_UseItem;
									roleData->inputParam(WCHDATA_ItemState,&itemstate);
									unsigned char targettype = Target_Coord;
									roleData->inputParam(WCHDATA_TargetType,&targettype);
									//int playerid = myPlayer->getPlayerID();
									//int roleid = role->getRoleID();
									//roleData->inputParam(WCHDATA_TargetID,NULL);
									//roleData->inputParam(WCHDATA_TargetRoleID,NULL);							
									roleData->inputParam(WCHDATA_TargetPos,&pos);
									roleData->inputParam(WCHDATA_AboutToUseItemID,&itemid);
									_case = 3;
								}
							}
							else
							{
								itemData->getParam(WCHDATA_ItemUseScope,param);
								if(param)
								{
									short scope = *(short *)param;
									if(scope>=0)//>=0
									{
										_case = 2;
									}
									else if(scope < 0)
									{
										_case = 1;
									}
								}
								else
								{
									_case = 1;
								}
							}
						}
					}
				}
			}
		}
	}
	handle.outputParam(0,&_case);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCardUseSkillDecalUpdateLogic
//
//////////////////////////////////////////////////////////////////////////
crJXJCardUseSkillDecalUpdateLogic::crJXJCardUseSkillDecalUpdateLogic():
	m_this(NULL),
	m_decalNode(NULL)
{
}

crJXJCardUseSkillDecalUpdateLogic::crJXJCardUseSkillDecalUpdateLogic(const crJXJCardUseSkillDecalUpdateLogic& handle):
	crNodeLogic(handle),
	m_this(NULL),
	m_decalNode(NULL)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
	if(handle.m_playSceneFx_range.valid())
		m_playSceneFx_range = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx_range->clone());
}

void crJXJCardUseSkillDecalUpdateLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crImageBoxWidgetNode*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (crGUIEventAdapter *)(LOCREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}

void crJXJCardUseSkillDecalUpdateLogic::addParam(int i, const std::string& str)
{
}

void crJXJCardUseSkillDecalUpdateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	case 1:
		m_playSceneFx_range = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJCardUseSkillDecalUpdateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}

void crJXJCardUseSkillDecalUpdateLogic::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!m_playSceneFx.valid() || !m_playSceneFx_range.valid() || !camera.valid()) return;
	void *param;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	cameraData->getParam(WCHDATA_JXJUseSkillDecal,param);
	m_decalNode = (crNode *)param;

	if(!m_decalNode)
	{
		(*m_playSceneFx)(*this);
		cameraData->inputParam(WCHDATA_JXJUseSkillDecal,m_decalNode);
	}
	if(m_decalNode)
	{
		CRCore::ScopedLock<crData> lock(*cameraData);
		do 
		{
			//cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
			//SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
			cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
			AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
			crRole *role = useItemPair->first.get();
			if(role)
			{
				//crRole *role = (*selectMyRoles)[0].get();
				crData *roleData = role->getDataClass();
				if(roleData)
				{
					roleData->getParam(WCHDATA_RTHP,param);
					float rthp = *(float*)param;
					if(rthp>0.0f)
					{
						int itemid = useItemPair->second;//*((int*)param);
						if(itemid != 0)
						{
							crItemChild *itemChild = role->findChildItem(itemid);
							if(itemChild && itemChild->isLoaded())
							{
								crInstanceItem *item = itemChild->getInstanceItem();
								crData *itemData = item->getDataClass();
								itemData->getParam(WCHDATA_ItemUseScope,param);
								if(param)
								{
									short scope = *(short *)param;
									if(scope>0)
									{
										crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(m_decalNode->getUpdateCallback("DecalUpdate"));
										if(callback)
										{
											callback->setVisiable(true);
											callback->setColor(crVector4(0,1,0,1));
											crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
											if(bindview && bindview->isInited())
											{
												CRCore::crIntersectVisitor::HitList hits;
												bindview->computeIntersections(m_ea->getXnormalized(),m_ea->getYnormalized(),crSceneManager::getInstance()->getStaticRoot(),hits,~AutoTransMask);
												if(!hits.empty())
												{
													CRCore::crVector3 pickPoint = hits[0].getWorldIntersectPoint();
													callback->setPosition(crVector2(pickPoint[0],pickPoint[1]));
												}
												float fscope = (float)scope * crGlobalHandle::gData()->gUnitScale();
												callback->setSize(crVector2(fscope,fscope));
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			m_decalNode->setVisiable(false);
		}
		while(0);
	}
	//////////////////////////////////////////////////////////////////////////
	crMatrixTransform *bot;
	crVector3 pos;
	cameraData->getParam(WCHDATA_JXJUseSkillRangeNodeMap,param);
	m_decalNode = (crNode *)param;
	if(!m_decalNode)
	{
		(*m_playSceneFx_range)(*this);
		cameraData->inputParam(WCHDATA_JXJUseSkillRangeNodeMap,m_decalNode);
	}
	if(m_decalNode)
	{
		CRCore::ScopedLock<crData> lock(*cameraData);
		do 
		{
			cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
			AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
			if(useItemPair && useItemPair->first.valid())
			{
				crRole *role = useItemPair->first.get();
				//crInstanceItem *role = (*selectMyRoles)[0].get();
				crData *roleData = role->getDataClass();
				if(roleData)
				{
					roleData->getParam(WCHDATA_RTHP,param);
					float rthp = *(float*)param;
					if(rthp>0.0f)
					{
						int itemid = useItemPair->second;//*((int*)param);
						if(itemid != 0)
						{
							crItemChild *itemChild = role->findChildItem(itemid);
							if(itemChild && itemChild->isLoaded())
							{
								crInstanceItem *item = itemChild->getInstanceItem();
								crData *itemData = item->getDataClass();
								itemData->getParam(WCHDATA_ItemUseRange,param);
								if(param)
								{
									unsigned short range = *(unsigned short *)param;
									if(range>0)
									{
										crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode);
										if(skillRangeMatrix)
										{
											bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
											if(!bot || !bot->getVisiable())
											{
												skillRangeMatrix->setVisiable(false);
											}
											else
											{
												skillRangeMatrix->setVisiable(true);
												pos = bot->getTrans();
												float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
												//skillRangeMatrix->setMatrix(crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f) * crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+1.5)));
												if(fscope<300.0f)
													skillRangeMatrix->setMatrix(crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+1.5f)));
												else
													skillRangeMatrix->setMatrix(crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+1.5f)));
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
			m_decalNode->setVisiable(false);
		}
		while(0);
	}
}
void crJXJCardUseSkillDecalUpdateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
	if(m_playSceneFx_range.valid())
		m_playSceneFx_range->releaseObjects(state);
}
//////////////////////////////////////////////////////////////////////////
//crJXJChooseBirthPointButtonMethod
//////////////////////////////////////////////////////////////////////////
crJXJChooseBirthPointButtonMethod::crJXJChooseBirthPointButtonMethod():m_this(NULL)
{

}
crJXJChooseBirthPointButtonMethod::crJXJChooseBirthPointButtonMethod(const crJXJChooseBirthPointButtonMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJChooseBirthPointButtonMethod::inputParam(int i, void *param)
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
		m_this = (crButtonWidgetNode*)param;
		break;
	}
}
void crJXJChooseBirthPointButtonMethod::addParam(int i, const std::string& str)
{

}
void crJXJChooseBirthPointButtonMethod::operator ()(crHandle &handle)
{
	void *param;
	if(m_this)
	{
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		if (FT_Chengchi)
		{
			ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
			//crRadioGroupWidgetNode *birthPointRadio = dynamic_cast<crRadioGroupWidgetNode *>(canvas->getWidget(m_birthPointRadioGroup));
			m_this->getDataClass()->getParam(WCHDATA_JXJBirthPointButtonData,param);
			crVector3i selectPoint = *(crVector3i *)param;
			if(selectPoint != crVector3i(0,0,0))
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(12);
					stream->_writeVec3i(selectPoint);
					crPlayerDataSceneEventPacket packet;
					crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvBirthPoint,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
		}
		else
		{
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(netConductor)
			{
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeChar(0);
				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenBirthPoint,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);

			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//crJXJConjureCDTimerMethod
//////////////////////////////////////////////////////////////////////////
crJXJConjureCDTimerMethod::crJXJConjureCDTimerMethod():m_this(NULL)
{

}
crJXJConjureCDTimerMethod::crJXJConjureCDTimerMethod(const crJXJConjureCDTimerMethod& handle):
	crMethod(handle),
	m_this(NULL),m_cdinputQ(handle.m_cdinputQ),m_sequenceQ(handle.m_sequenceQ),
	m_cdinputW(handle.m_cdinputW),m_sequenceW(handle.m_sequenceW),
	m_cdinputE(handle.m_cdinputE),m_sequenceE(handle.m_sequenceE)
{

}
void  crJXJConjureCDTimerMethod::inputParam(int i, void *param)
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
void crJXJConjureCDTimerMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_cdinputQ = str;
		break;
	case 1:
		m_sequenceQ = str;
		break;
	case 2:
		m_cdinputW = str;
		break;
	case 3:
		m_sequenceW = str;
		break;
	case 4:
		m_cdinputE = str;
		break;
	case 5:
		m_sequenceE = str;
		break;
	}
}
void crJXJConjureCDTimerMethod::operator ()(crHandle &handle)
{
	if(!crMyPlayerData::getInstance()->getPlayerGameData())
		return;
	ref_ptr<crStaticTextWidgetNode>  cdinputQ = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cdinputQ));
	ref_ptr<crSequence>  seqQ = dynamic_cast<crSequence*>(m_this->getChildNode(m_sequenceQ));
	ref_ptr<crStaticTextWidgetNode>  cdinputW = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cdinputW));
	ref_ptr<crSequence>  seqW = dynamic_cast<crSequence*>(m_this->getChildNode(m_sequenceW));
	ref_ptr<crStaticTextWidgetNode>  cdinputE = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_cdinputE));
	ref_ptr<crSequence>  seqE = dynamic_cast<crSequence*>(m_this->getChildNode(m_sequenceE));

	seqQ->setVanishWhenStop(true);
	seqW->setVanishWhenStop(true);
	seqE->setVanishWhenStop(true);
	vector<ref_ptr<crStaticTextWidgetNode >> cdinputVec;
	vector<ref_ptr<crSequence>> seqVec;
	cdinputVec.push_back(cdinputQ);
	cdinputVec.push_back(cdinputW);
	cdinputVec.push_back(cdinputE);
	seqVec.push_back(seqQ);
	seqVec.push_back(seqW);
	seqVec.push_back(seqE);

	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	int iconid = itemtab->getTitleIndex("icon");
	int nameid = itemtab->getTitleIndex("name");
	int abstractid = itemtab->getTitleIndex("abstractid");
	void *param;
	int itemid;

	crTableIO::StrVec record1,itemrecord1;
	int roleid = -1;
	crData *playerdata = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
	playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec *myteam = (FormationInfoVec *)param;
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
	crVector2i equips = crVector2i(-1,-1);

	crInstanceItem *selectItem = NULL;
	crData *selectdata = NULL;
	crRole *role = NULL;
	int index = 0;
	crMyPlayerData::MyRoleNpcMap roleMap =  crMyPlayerData::getInstance()->getMyRoleNpcMap();
	for(crMyPlayerData::MyRoleNpcMap::iterator itr_role = roleMap.begin();itr_role!=roleMap.end();itr_role++,index++)
	{
		role = itr_role->second.first.get();
		for( FormationInfoVec::iterator itr = myteam->begin();
			itr != myteam->end();
			++itr)
		{
			if(itr->get()->getRoleID() == itr_role->first)
			{
				equips = itr->get()->getEquips();
				break;
			}
		}

		if(role)
		{
			crData *roleData = role->getDataClass();
			if(roleData)
			{
				roleData->getParam(WCHDATA_RTHP,param);
				float rthp = *(float*)param;
				if(rthp<=0.0f)
				{
					seqVec[index]->setVisiable(false); 
					cdinputVec[index]->setVisiable(false);
					continue;
				}
			}
			if(equips[0]>0)
			{
				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord1)>=0)
				{
					itemid = atoi(itemrecord1[abstractid].c_str());
					crData *data = role->getDataClass();
					data->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->getParam(WCHDATA_EquipOnMap,param);
					EquipOnMap *equipOnMap = (EquipOnMap *)param;
					EquipOnMap::iterator itr_equipmap = equipOnMap->find(itemid);
					crItemChild *itemChild  = NULL;
					if(itr_equipmap!=equipOnMap->end())
						itemChild= role->findChildItem(itr_equipmap->second);	
					if(itemChild && itemChild->isLoaded())
					{
						crInstanceItem *item = itemChild->getInstanceItem();
						crData *skillData = item->getDataClass();
						if(skillData)
						{
							CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
							skillData->getParam(WCHDATA_CDTime,param);
							float cdtime = *(float *)param;
							//skillData->getParam(WCHDATA_UseItemProtectTime,param);
							//float protectTime = *(float *)param;

							float cdreduce = 0.0f;
							skillData->getParam(WCHDATA_DataType,param);
							unsigned char datatype = *(unsigned char*)param;
							if(datatype == DT_Attack)
								role->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
							else
								role->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&cdreduce,NULL));
							if(cdreduce<=-1.0f)
								cdreduce = -0.99f; 
							cdtime = cdtime/(1.0f+cdreduce);
							//cdtime += protectTime;
							//该技能上一次的施法时间

							skillData->getParam(WCHDATA_LastUseTime,param);
							_crInt64 lastUseTime = *(_crInt64 *)param;
							if(lastUseTime!=0)
							{
								crSequence::LoopMode loopmode;
								int begin;
								int end;
								int frameCount;
								float dt = CRCore::Timer::instance()->delta_s( lastUseTime, t1 );
								if(dt<cdtime-1.0f && seqVec[index]->getMode() == crSequence::STOP)//progressVec[index].getvi
								{
									// 									if(seqVec[index]->getMode() == crSequence::STOP)
									// 									{
									seqVec[index]->getInterval(loopmode,begin,end,frameCount);
									seqVec[index]->setMode(crSequence::START);
									seqVec[index]->setDuration((cdtime-dt)/(float)frameCount,1);
									//}
								}
								char str[8]={};
								int timer =(int)(cdtime-dt);
								if(timer>=0 && seqVec[index]->getMode() == crSequence::START)
								{
									seqVec[index]->setVisiable(true);
									sprintf(str,"%d\0",timer+1);
									cdinputVec[index]->setCharacterSize(18);
									cdinputVec[index]->setAlignment(CRText::crText::CENTER_CENTER);
									cdinputVec[index]->setVisiable(true);
									cdinputVec[index]->setString(string(str));	
									cdinputVec[index]->setColor(COLOR_YELLOW);
								}
								else
								{
									cdinputVec[index]->setVisiable(false);
								}
							}
							else
							{
								if(seqVec[index]->getMode() == crSequence::START)
								{
									seqVec[index]->stopEffect();
									cdinputVec[index]->clearString();
								}
							}
						}
					}
					data->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJRandomBirthPointMethod
//////////////////////////////////////////////////////////////////////////
crJXJRandomBirthPointMethod::crJXJRandomBirthPointMethod()
{

}
crJXJRandomBirthPointMethod::crJXJRandomBirthPointMethod(const crJXJRandomBirthPointMethod& handle):
	crMethod(handle)
{

}
void crJXJRandomBirthPointMethod::inputParam(int i, void *param)
{

}
void crJXJRandomBirthPointMethod::addParam(int i, const std::string& str)
{
}
void crJXJRandomBirthPointMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	if(myPlayer && room)
	{
		void *param;
		crData *data = myPlayer->getDataClass();
		crData *roomData = room->getDataClass();
		if(data && roomData)
		{
			roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
			roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
			JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;

			data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			unsigned char type = *(unsigned char *)param;
			if(type == FT_Chengchi)
			{
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short chengchiid = *(unsigned short *)param;
				if(chengchiid>0)
				{
					const crSceneLayer::BirthPointArray& pointVec = crMyPlayerData::getInstance()->getSceneLayer()->getBirthPointArray();
					crSceneLayer::BirthPointArray tempvec;

					ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
					crTableIO::StrVec record;
					chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record);
					std::string str = record[chengchiTab->getTitleIndex("出生点数")];
					crVector2i vec;
					crArgumentParser::appAtoVec(str,vec);

					data->getParam(WCHDATA_JXJChengChiMap,param);
					ChengchiMap *chengchimap = (ChengchiMap *)param;
					roomData->getParam(WCHDATA_JXJBattleID,param);
					_crInt32 battleid = *(_crInt32 *)param;
					//unsigned char ashili = LOINT16(LOINT32(battleid));
					unsigned char dshili = HIINT16(LOINT32(battleid));//(*chengchimap)[chengchiid]->getChengzhuShili();
					data->getParam(WCHDATA_JXJShiliID,param);
					unsigned char myShili = *(unsigned char *)param;
					int selectCount  = dshili == myShili?vec[1]:0;//vec[0];功方从阵营出生点出
					int beginIndex = dshili == myShili?vec[0]:0;
					//int totalCount = 0;
					if(!pointVec.empty())
					{
						for (int index = beginIndex; index<beginIndex + selectCount; index++)
						{
							tempvec.push_back(pointVec[index]);
							//totalCount++;
						}
					}
					JXJCampBirthpointMap::iterator itr = campMap->find(myShili);
					if(itr != campMap->end())
					{
						for( JXJCampBirthpoint::iterator itr_p = itr->second.begin();
							 itr_p!=itr->second.end();
							 itr_p++ )
						{
							tempvec.push_back(itr_p->first);
							//totalCount++;
						}
					}
					//if(totalCount == 0)
					//{//未找到出生点，从0号出生点出
					//	tempvec.push_back(pointVec[0]);
					//	totalCount++;
					//}
					int totalCount = tempvec.size();
					if(totalCount>0)
					{
						CRCore::rangei r(0,totalCount);
						char selectid = r.get_random();

						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
						if(netConductor && selectid<tempvec.size())
						{
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(12);
							stream->_writeVec3i(tempvec[selectid]);
							crPlayerDataSceneEventPacket packet;
							crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvBirthPoint,stream.get());
							netConductor->getNetManager()->sendPacket("all",packet);
						}
					}
				}
			}
			else
			{
				crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
				if(netConductor)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1);
					stream->_writeChar(0);
					crPlayerDataSceneEventPacket packet;
					crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenBirthPoint,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
				}
			}
			roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJIsAllDeadShowMethod
//////////////////////////////////////////////////////////////////////////
crJXJIsAllDeadShowMethod::crJXJIsAllDeadShowMethod():
	m_this(NULL){}
crJXJIsAllDeadShowMethod::crJXJIsAllDeadShowMethod(const crJXJIsAllDeadShowMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_canvas(handle.m_canvas),
	m_canvasAdd(handle.m_canvasAdd)
{
}
void crJXJIsAllDeadShowMethod::inputParam(int i, void *param)
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
void crJXJIsAllDeadShowMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_canvasAdd = str;
		break;
	case 2:
		m_strInfo = str;
		break;
	}
}
void crJXJIsAllDeadShowMethod::operator ()(crHandle &handle)
{
	void *param;
	AddArmyInfoVec armyinfo;
	unsigned char vipLevel = 0;
	char _case = -1;//-1,补兵失败，0-》VIP0粮草充足并且兵力充足补兵成功，1-》VIP1兵力充足，2-》兵力不足，资源充足，直接扣除资源补兵；3-》资源不足，扣除元宝补兵
	crMyPlayerData *me = crMyPlayerData::getInstance();
	ref_ptr<crTableIO> equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
	ref_ptr<crTableIO> troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer && crRunGameHandle::getInstance()->isInGame())
	{
		crData *myPlayerData = myPlayer->getDataClass();
		if(myPlayerData)
		{
			myPlayerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int addArmyNeedExtraFood = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleAddArmyExtraFoodNeed, vipLevel).c_str()));
			short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime, vipLevel).c_str()));
			int credittime = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime, vipLevel).c_str()));
			crRole *role = NULL;

			bool isshow = true;
			unsigned int guisestate;
			for(int i = 0;i<3;i++)
			{
				role = me->getLoadedRoleByIndex(i);
				if(!role)
					continue;
				crData *data = role->getDataClass();
				guisestate = GS_Normal;
				role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
				//guisestate = *(unsigned int *)param;
				isshow = isshow && (guisestate & GS_UnVisiable);
			}
			if(!isshow) return;
			role = me->getLoadedRoleByIndex(0);
			crVector3 deadPos;
			if(role) deadPos = role->getPosition();
			//////////////////////////////////////////////////////////////////////////
			ref_ptr<crCanvasNode>   addArmyCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasAdd);
			ref_ptr<crCanvasNode>   canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(me && me->ifItemIsMe(m_this) && addArmyCanvas.valid() && canvas.valid())
			{
				ref_ptr<crStaticTextWidgetNode>  info = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strInfo));
				crData *canvasData = addArmyCanvas->getDataClass();
				//crPlayerGameData *myPlayer = me->getPlayerGameData();
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
				int perleaderneed = 0;
				int totalLeaderCount = 0;
				int goldneed = 0;
				if(canvasData)
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJBattleAddArmyInfoMap,param);
					BattleAddArmyInfoVec *addArmyInfoVec = (BattleAddArmyInfoVec *)param;
					//crData *myPlayerData = myPlayer->getDataClass();
					//if(myPlayerData)
					
						//////////////////////////////////////////////////////////////////////////
					myPlayerData->getParam(WCHDATA_JXJVipLv,param);
					vipLevel = *(unsigned char *)param;
					myPlayerData->getParam(WCHDATA_JXJCoppercash,param);
					int mycopper = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJWood,param);
					int mywood = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJIron,param);
					int myiron = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJHorse,param);
					int myhorse = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJFood,param);
					int myfood = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJGoldingot,param);
					int myGold = *(int *)param;
					myPlayerData->getParam(WCHDATA_JXJGiftGold,param);
					myGold+=(*(int *)param);
					myPlayerData->getParam(WCHDATA_JXJTroopsMap,param);
					TroopsMap *troops = (TroopsMap *)param;
					crRole *role = NULL;
					TroopsMap tempTroopsMap;
					for(int i = 0;i<3;i++)
					{
						role = me->getLoadedRoleByIndex(i);
						if(!role)
							continue;
						int abstractid = role->getAbstractItemID();
						TroopsMap::iterator itr_troops = troops->find(abstractid);
						if(itr_troops!=troops->end())
							tempTroopsMap.insert(make_pair(itr_troops->first,itr_troops->second));
					}
					for(int i = 0;i<3;i++)
					{
						role = me->getLoadedRoleByIndex(i);
						if(!role)
							continue;
						crData *data = role->getDataClass();
						int abstractid = role->getAbstractItemID();
						int roleid = role->getRoleID();
						if(data)
						{
							data->getParam(WCHDATA_JXJArmyCount,param);
							/*unsigned short*/int count = *(/*unsigned short*/int *)param;
							int troopRTcount = 0;
							TroopsMap::iterator itr_troops = tempTroopsMap.find(abstractid);
							if(itr_troops!=tempTroopsMap.end())
							{
								troopRTcount = itr_troops->second;
							}
							if(troopRTcount>count)
							{
								itr_troops->second-=count;
							}
							if (count > troopRTcount)
							{ 
								armyinfo.push_back(make_pair(abstractid,count-troopRTcount));
							}
							addArmyInfoVec->push_back( make_pair(abstractid,make_pair(roleid,count)));
						}			
					}
					
					if(armyinfo.empty())
					{
						if(vipLevel == 0)
						{
							if(myfood>=addArmyNeedExtraFood)
							{
								_case = 0;
								canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&addArmyNeedExtraFood);
							}
							else
								_case = -2;
						}
						else
						{
							_case = 1;
						}
					}
					else
					{
						if(vipLevel > 1)
						{
							
							crTableIO::StrVec record,record2;
							AddArmyInfoVec::iterator itr = armyinfo.begin();
							int equipcopperindex = equipTab->getTitleIndex("铜钱");
							int equipfoodindex = equipTab->getTitleIndex("粮食");
							int equiphorseindex = equipTab->getTitleIndex("马匹");
							int equipironindex = equipTab->getTitleIndex("铁矿");
							int equipwoodindex = equipTab->getTitleIndex("木材");
							int leaderneedindex = troopTab->getTitleIndex("领导力");
							while (itr!=armyinfo.end())
							{
								RecruitCheckResult recruitCheck;
								myPlayer->doEvent(WCH_JXJRecruitCheck,MAKECREPARAM(itr->first,&recruitCheck));
								if(recruitCheck.m_code == -12)
								{//不可招募
									_case = -12;
								}
								if(troopTab->queryOneRecord(0,crArgumentParser::appItoa(itr->first),record)>=0)
								{
									if(leaderneedindex>0)
										perleaderneed = atoi(record[leaderneedindex].c_str());
								}

								copperneed += ceil(itr->second*recruitCheck.m_needresourcevec[T_Copper - 1]);
								foodneed += ceil(itr->second*recruitCheck.m_needresourcevec[T_Food - 1]);
								woodneed += ceil(itr->second*recruitCheck.m_needresourcevec[T_Wood - 1]);
								ironneed += ceil(itr->second*recruitCheck.m_needresourcevec[T_Iron - 1]);
								horseneed += ceil(itr->second*recruitCheck.m_needresourcevec[T_Horse - 1]);
								timeneed += ceil(itr->second*recruitCheck.m_timeNeed);
								totalLeaderCount += itr->second*perleaderneed;

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
							if(_case != -12)
							{
								myPlayerData->getParam(WCHDATA_JXJRecruitTimes,param);
								crVector4i recruitTimes = *(crVector4i *)param;

								int i = 0;
								int timeRemain = 0;
								for(; i<4; i++)
								{
									if(recruitTimes[i]>=0 && recruitTimes[i] < overTimes)
										timeRemain += overTimes - recruitTimes[i] + credittime;
								}
								if(copperneed > mycopper || woodneed > mywood || ironneed > myiron || horseneed > myhorse || foodneed > myfood || timeneed > timeRemain)
								{
									goldneed = totalLeaderCount/100+10;
									if(vipLevel>=3)
									{
										if(goldneed<=myGold)
										{
											_case = 3;
											canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedGold,&goldneed);
										}
										else
											_case = -3;
									}
									else
										_case = -2;
								}
								else
								{
									_case = 2;
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedCopper,&copperneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedFood,&foodneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedWood,&woodneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedIron,&ironneed);
									canvasData->inputParam(WCHDATA_JXJBattleAddArmyNeedHorse,&horseneed);
								}
							}
						}
					}
					
					canvasData->inputParam(WCHDATA_JXJBattleAddArmyCase,&_case);
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				if(_case<0)
				{
					int textid = 1087;
					switch (_case)
					{
					case -1:
						textid = 1088;//兵力不足，补兵失败！点击确定后退出
						break;
					case -2:
						textid = 1089;//粮草不足，补兵失败！点击确定后退出
						break;
					case -3:
						textid = 1090;//元宝不足
						break;
					case -4:
						textid = 1091;//资源不足
						break;
					case -6:
						textid = 1092;//铜钱不足
						break;
					case -7:
						textid = 1093;//今日可购资源量不足
						break;
					case -8:
						textid = 1094;//资源够买失败
						break;
					case -12:
						textid = 1114;//兵力不足,且不可招募！
						break;
					}
					if(info.valid())
					{
						ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
						crTableIO::StrVec record;
						std::string str;
						if(textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),record)>=0)
						{
							str = record[1];
						}
						info->setString(str);
					}
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
				else
				{
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					myPlayerData->getParam(WCHDATA_JXJChooseAutoBattleMode,param);
					bool autoBattle = *(bool*)param;
					
					bool canAutoAdd  = false;
					if(autoBattle && netConductor)
					{
						int needVIP = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAutoAddArmyVIPLevel,vipLevel).c_str());
						int needResourceVIP = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJAutoAddArmyByResource,vipLevel).c_str());
						
						if ((foodneed > 0 || woodneed > 0 || ironneed > 0 || horseneed > 0) && vipLevel >= needResourceVIP && goldneed == 0)
						{
							canAutoAdd = true;
						}
						else if (foodneed == 0 && woodneed == 0 && ironneed == 0 && horseneed == 0 && vipLevel >= needVIP && goldneed == 0)
						{
							canAutoAdd = true;
						}
					}

					if (canAutoAdd)
					{
						crJXJCampAddArmyInfoPacket packet;
						netConductor->getNetManager()->sendPacket("all",packet);
						return;
					}else
					{
						switch (_case)
						{
						case 0:
						case 1:
						case 2:
						case 3:
							crFilterRenderManager::getInstance()->showCanvas(addArmyCanvas.get(),true);
							break;
						}
					}
					
				}
				ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
				if(camera.valid())
				{
					crData *camadata = camera->getAttachedNode()->getDataClass();

					//crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();

					//myPlayerData->getParam(WCHDATA_JXJVipLv,param);
					//unsigned char vipLevel = *(unsigned char *)param;
					short timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJExitTimerLimit, vipLevel).c_str());
					camadata->inputParam(WCHDATA_JXJTimer,&timer);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJShowTimerMethod
//////////////////////////////////////////////////////////////////////////
crJXJShowTimerMethod::crJXJShowTimerMethod():
	m_this(NULL)
{

}
crJXJShowTimerMethod::crJXJShowTimerMethod(const crJXJShowTimerMethod &handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJShowTimerMethod::inputParam(int i, void *param)
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
		m_this = (crStaticTextWidgetNode*)param;
		break;
	}
}
void crJXJShowTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJShowTimerMethod::operator ()(crHandle &handle)
{
	bool bExit = true;
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		m_this->setAlignment(CRText::crText::CENTER_BASE_LINE);
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		cameraData->getParam(WCHDATA_JXJTimer,param);
		short timer = *(short*)param;
		char str[4];
		sprintf(str,"%02d\0",timer-1);
		m_this->setString(string(str));
		if(timer<=0)
			bExit = false;
		if(timer>0)
			timer--;
		cameraData->inputParam(WCHDATA_JXJTimer,&timer);
	}
	handle.outputParam(0,&bExit);
}
///////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
crJXJMainHallDamageSignalMethod::crJXJMainHallDamageSignalMethod(){}
crJXJMainHallDamageSignalMethod::crJXJMainHallDamageSignalMethod(const crJXJMainHallDamageSignalMethod& handle):
	crMethod(handle)
{
}
void crJXJMainHallDamageSignalMethod::inputParam(int i, void *param)
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

void crJXJMainHallDamageSignalMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_seq = str;
		break;
	case 2:
		m_image = str;
		break;
	case 3:
		m_rotate = str;
		break;
	}
}

void crJXJMainHallDamageSignalMethod::operator()(crHandle &handle)
{
	if (crRunGameHandle::getInstance()->isInBattle())
	{
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if (!m_signal.valid())
		{
			ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
			if (!mainCanvas)
				return;
			ref_ptr<crWidgetNode>  signalWidget = dynamic_cast<crWidgetNode *>(mainCanvas->getWidget(m_seq));
			if (!signalWidget)
				return;
			m_signal = dynamic_cast<crWidgetNode *>(signalWidget->clone(crCopyOp::DEEP_COPY_NODES));
			ref_ptr<crWidgetNode>  rotateWeidget = canvas->getWidget(m_rotate);
			//rotateWeidget->addChild(m_signal.get());
			crLoadManager::getInstance()->requestAddNode(rotateWeidget.get(), m_signal.get(), false);
			m_signal->setParentCanvas(canvas.get());

		}
		m_signal->setVisiable(true);
		ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(m_signal->getChild(0));
		seq->setDuration(-1, 3);
		seq->startEffect();
		const crMatrix &scaleMat = crDisplaySettings::instance()->getUIScaleMatrix();
		const crMatrix &invScaleMat = crDisplaySettings::instance()->getInvUIScaleMatrix();
		ref_ptr<crImageBoxWidgetNode>  mapWidget = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_image));
		crVector2 mapWidgetSize;
		const crBoundingBox &bbox = mapWidget->getBoundBox();
		mapWidgetSize.set(bbox.xLength(), bbox.yLength());
		crVector2 mapFactor = crMyPlayerData::getInstance()->getMapFactor(mapWidgetSize);
		float px = m_this->getPosx()*crGlobalHandle::gData()->gUnitScale();
		float py = m_this->getPosy()*crGlobalHandle::gData()->gUnitScale();
		px *= mapFactor[0];
		py *= mapFactor[1];
		crVector3 pos = mapWidget->getBound().center() + crVector3(px, py, 0.0f);
		m_signal->setMatrix(invScaleMat  * scaleMat * crMatrix::translate(pos));
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJUIBattleAddArmyInitMethod
//////////////////////////////////////////////////////////////////////////
crJXJUIBattleAddArmyInitMethod::crJXJUIBattleAddArmyInitMethod():m_this(NULL){}
crJXJUIBattleAddArmyInitMethod::crJXJUIBattleAddArmyInitMethod(const crJXJUIBattleAddArmyInitMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_drag(handle.m_drag)
{
}

void crJXJUIBattleAddArmyInitMethod::inputParam(int i, void *param)
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
void crJXJUIBattleAddArmyInitMethod::addParam(int i,const std::string& str)
{
	switch(i)
	{
	case 0:
		m_drag = str;
		break;
	case 1:
		m_icon[0] = str;
		break;
	case 2:
		m_icon[1] = str;
		break;
	case 3:
		m_icon[2] = str;
		break;
	case 4:
		m_input[0] = str;
		break;
	case 5:
		m_input[1] = str;
		break;
	case 6:
		m_input[2] = str;
		break;
	case 7:
		m_strBtMakeStrong = str;
		break;
	}
}
void crJXJUIBattleAddArmyInitMethod::operator()(crHandle &handle)
{
	void *param;
	string iconfile;
	crTableIO::StrVec record;
	ref_ptr<crImageBoxWidgetNode>  icon[3];
	ref_ptr<crStaticTextWidgetNode>  input[3];
	crMyPlayerData *me = crMyPlayerData::getInstance();
	me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJTroopsMap,param);
	TroopsMap *troops = (TroopsMap*)param;
	ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
	int totalCount[3];//总兵力
	int armyCount[3];//编队兵力
	crRole *role = NULL;

	//crStaticTextWidgetNode *drag = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_drag));
	icon[0] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[0]));
	icon[1] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[1]));
	icon[2] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[2]));
	input[0] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[0]));
	input[1] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[1]));
	input[2] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input[2]));

	ref_ptr<crButtonWidgetNode>  btMakeStrong = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strBtMakeStrong));
	if(btMakeStrong.valid()) btMakeStrong->setEnable(false);
	//if(drag)
	//{
	//	drag->setString(string(TEXT("战场提示")));
	//	drag->setColor(COLOR_YELLOW);
	//	drag->setAlignment(CRText::crText::CENTER_BASE_LINE);
	//}
	for(int i = 0;i<3;i++)
	{
		icon[i]->setVisiable(false);
		role = me->getLoadedRoleByIndex(i);
		if(!role)
			continue;
		crData *data = role->getDataClass();
		int abstractid = role->getAbstractItemID();
		int iconid = troopsTab->getTitleIndex("icon");
		troopsTab->queryOneRecord(0,crArgumentParser::appItoa(abstractid),record);
		iconfile = record[iconid];
		if(icon[i]!= NULL) 
		{
			icon[i]->setImageName(iconfile);
			icon[i]->setVisiable(true);
		}
		if(data)
		{
			data->getParam(WCHDATA_JXJArmyCount,param);
			/*unsigned short*/int count = *(/*unsigned short*/int *)param;
			TroopsMap::iterator itr_troops = troops->find(abstractid);
			if(itr_troops!=troops->end())
				totalCount[i] = itr_troops->second;
			armyCount[i] = count;
			if(input[i].valid()) 
			{
				input[i]->setAlignment(CRText::crText::CENTER_BASE_LINE);
				input[i]->setCharacterSize(12.0f);
				input[i]->setString(crArgumentParser::appItoa(armyCount[i])+std::string(TEXT("/总:"))+crArgumentParser::appItoa(totalCount[i]));
			}
		}			
	}

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		crData *myPlayerData = myPlayer->getDataClass();
		if(myPlayerData)
		{
			void *param;
			myPlayerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			myPlayerData->getParam(WCHDATA_JXJInspireLevel,param);
			unsigned char inspireLv = *(unsigned char *)param;
			char count = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireMaxCount,vipLevel).c_str());
			if((count>0 && inspireLv<count) && btMakeStrong.valid())
				btMakeStrong->setEnable(true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJUIBattleAddArmyMethod
//////////////////////////////////////////////////////////////////////////
crJXJUIBattleAddArmyMethod::crJXJUIBattleAddArmyMethod():m_addArmyCount(0)
{}
crJXJUIBattleAddArmyMethod::crJXJUIBattleAddArmyMethod(const crJXJUIBattleAddArmyMethod& handle)
{

}
void crJXJUIBattleAddArmyMethod::inputParam(int i, void *param)
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
void crJXJUIBattleAddArmyMethod::addParam(int i, const std::string& str)
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
void crJXJUIBattleAddArmyMethod::operator()(crHandle &handle)
{
	void *param;
	m_addArmyCount++;
	crCanvasNode* canvas = m_this->getParentCanvas();
	ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_noticeCanvas);
	crMyPlayerData *me = crMyPlayerData::getInstance();
// 	int inspireCost = 0;
// 	if(me && me->getPlayerGameData())
// 	{
// 		crData *playerData = me->getPlayerGameData()->getDataClass();
// 		if(playerData)
// 		{
// 			playerData->getParam(WCHDATA_JXJVipLv,param);
// 			unsigned char vipLevel = *(unsigned char *)param;
// 
// 			inspireCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireCost, vipLevel).c_str());
// 		}
// 	}
	if(!msgCanvas)return;
	ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_input));
	ref_ptr<crButtonWidgetNode>  okBtn = dynamic_cast<crButtonWidgetNode *>(msgCanvas->getWidget(m_okbtn));
	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	if(canvas && room)
	{
		crData *canvasData = canvas->getDataClass();
		crData *roomData = room->getDataClass();
		if(canvasData && roomData)
		{
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJBattleAddArmyInfoMap,param);
			BattleAddArmyInfoVec *armyInfoMap = (BattleAddArmyInfoVec *)param;
			canvasData->getParam(WCHDATA_JXJBattleAddArmyCase,param);
			char _case = *(char *)param;
			string strInfo;
			if( _case == 1)
			{
// 				if(m_index == 1 && inspireCost>0)
// 				{
// 					std::vector<float>v_i;
// 					ItemMap itemMap;
// 					rcfg::ConfigScript cfg_script(&itemMap);
// 					cfg_script.Add("Hypertext");
// 					cfg_script.Push("Hypertext");
// 					cfg_script.Add("Content");
// 					cfg_script.Push("Content");
// 					cfg_script.Add("Text","您确认要花费");
// 					cfg_script.Pop();
// 
// 					cfg_script.Add("Content");
// 					cfg_script.Push("Content",2);
// 					v_i.clear();
// 					v_i.push_back(255.0);
// 					v_i.push_back(255.0);
// 					v_i.push_back(0.0);
// 					v_i.push_back(255.0);
// 					cfg_script.Add("Color",v_i);
// 					strInfo =string("元宝：") + crArgumentParser::appItoa(inspireCost);
// 					cfg_script.Add("Text",strInfo);
// 					cfg_script.Pop();
// 
// 					cfg_script.Add("Content");
// 					cfg_script.Push("Content",3);
// 					strInfo = crArgumentParser::appItoa(inspireCost);
// 					cfg_script.Add("Text","进行战场鼓舞！");
// 					cfg_script.Pop();
// 
// 					cfg_script.Pop();
// 					msg->setHypertext(cfg_script);
// 					ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
// 					//crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);
// 					crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
// 					crData *dlgData = msgCanvas->getDataClass();
// 					int commandtype = CDP_Widget;
// 					dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
// 					dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
// 				}
// 				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(1+armyInfoMap->size()*10);
					//stream->_writeChar(0);//0,死亡补兵；1，据的补兵
					stream->_writeChar(armyInfoMap->size());
					BattleAddArmyInfoVec::iterator itr_map = armyInfoMap->begin();
					while (itr_map!=armyInfoMap->end())
					{
						stream->_writeInt(itr_map->first);
						stream->_writeInt(itr_map->second.first);
						stream->_writeInt(itr_map->second.second);
						itr_map++;
					}
					crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddArmy,stream.get());
					netConductor->getNetManager()->sendPacket("all",packet);
					armyInfoMap->clear();
					crFilterRenderManager::getInstance()->closeCanvas(canvas);
				}
			}
			else if(_case == 0 ||_case == 2 || _case ==3)
			{
				//int hinttype = JXJGoldCostType_BattleInspire;
				//crData *braindata = crBrain::getInstance()->getDataClass();
				//braindata->inputParam(WCHDATA_JXJCurGoldHintType,&hinttype);

				//crData * data = me->getPlayerGameData()->getDataClass();
				//data->getParam(WCHDATA_JXJGoldCostHintInt,param);
				//unsigned int ifhint = *(unsigned int *)param & JXJGoldCostType_BattleInspire;
				//if (ifhint)
				//{
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					//cfg_script.Add("Hypertext");
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
					char tmpText2[80];
					string tmpText;
					if(_case==0)
					{
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedFood,param);
						int foodneed = *(int *)param;
						if(foodneed>0) tmpText+=string("粮草：")+crArgumentParser::appItoa(foodneed)+string(",");
					}
					else if(_case==2)
					{
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedCopper,param);
						int copperneed = *(int *)param;
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedWood,param);
						int woodneed = *(int *)param;
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedIron,param);
						int ironneed = *(int *)param;
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedHorse,param);
						int horseneed = *(int *)param;
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedFood,param);
						int foodneed = *(int *)param;
						if(copperneed>0) tmpText=string("铜钱：")+crArgumentParser::appItoa(copperneed)+string(",");
						if(woodneed>0) tmpText+=string("木材：")+crArgumentParser::appItoa(woodneed)+string(",");
						if(ironneed>0) tmpText+=string("铁矿：")+crArgumentParser::appItoa(ironneed)+string(",");
						if(horseneed>0) tmpText+=string("马匹：")+crArgumentParser::appItoa(horseneed)+string(",");
						if(foodneed>0) tmpText+=string("粮草：")+crArgumentParser::appItoa(foodneed)+string(",");
					}
					else if(_case == 3)
					{
						canvasData->getParam(WCHDATA_JXJBattleAddArmyNeedGold,param);
						int goldneed = *(int *)param;

						tmpText+=string("礼金/元宝：")+crArgumentParser::appItoa(goldneed);
						tmpText+=string("(由于资源不足，扣除礼金/元宝)");
					}
					cfg_script.Add("Text",tmpText);
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",3);
					sprintf(tmpText2,"来补充兵力!\0");
					cfg_script.Add("Text",tmpText2);
					cfg_script.Pop();
					// 				if(m_index==1 && inspireCost>0)
					// 				{
					// 					cfg_script.Add("Content");
					// 					cfg_script.Push("Content",3);
					// 					strInfo = string("另花费元宝：")+crArgumentParser::appItoa(inspireCost)+std::string("进行战场鼓舞！\0");
					// 					cfg_script.Add("Text",strInfo);
					// 					cfg_script.Pop();
					// 				}
					cfg_script.Pop();
					msg->setHypertext(cfg_script);
				//}
				ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
				//crFilterRenderManager::getInstance()->showCanvas(canvas.get(),false);
				//crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				crData *dlgData = msgCanvas->getDataClass();
				int commandtype = CDP_Widget;
				dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
				dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
				//if (ifhint)
				//{ //始终弹出战场补兵消费提示， 解除与 战场鼓舞消费提示 的绑定
					crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
				//}
				//else
				//{
				//	if(okBtn.valid())okBtn->doEvent(MAKEINT32(crGUIEventAdapter::LEFT_MOUSE_BUTTON,crGUIEventAdapter::RELEASE));
				//}
			}
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleAddArmyMethod::crJXJBattleAddArmyMethod():
	m_this(NULL)
{
}
crJXJBattleAddArmyMethod::crJXJBattleAddArmyMethod(const crJXJBattleAddArmyMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJBattleAddArmyMethod::inputParam(int i, void *param)
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
void crJXJBattleAddArmyMethod::addParam(int i, const std::string& str)
{
}
void crJXJBattleAddArmyMethod::operator()(crHandle &handle)
{
	void *param;
	crRole *role = NULL;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	BattleAddArmyInfoVec addarmymap;
	for(int i = 0;i<3;i++)
	{
		role = me->getLoadedRoleByIndex(i);
		if(!role)
			continue;
		crData *data = role->getDataClass();
		int roleid = role->getRoleID();
		int abstractid = role->getAbstractItemID();
		if(data)
		{
			data->getParam(WCHDATA_JXJArmyCount,param);
			/*unsigned short*/int count = *(/*unsigned short*/int *)param;
			addarmymap.push_back(make_pair(abstractid,make_pair(roleid,count)));
		}			
	}
	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
	if(room)
	{
		crData *roomData = room->getDataClass();
		if(roomData)
		{
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			stream->createBuf(1+addarmymap.size()*10);
			//stream->_writeChar(0);//0,死亡补兵；1，据的补兵
			stream->_writeChar(addarmymap.size());
			BattleAddArmyInfoVec::iterator itr_map = addarmymap.begin();
			while (itr_map!=addarmymap.end())
			{
				stream->_writeInt(itr_map->first);
				stream->_writeInt(itr_map->second.first);
				stream->_writeInt(itr_map->second.second);
				itr_map++;
			}
			crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvAddArmy,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJRecvBattleAddArmyMethod
//////////////////////////////////////////////////////////////////////////
crJXJRecvBattleAddArmyMethod::crJXJRecvBattleAddArmyMethod():
	m_netType(GameClient_Game){}
crJXJRecvBattleAddArmyMethod::crJXJRecvBattleAddArmyMethod(const crJXJRecvBattleAddArmyMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvBattleAddArmyMethod::inputParam(int i, void *param)
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

void crJXJRecvBattleAddArmyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strText = str;
		break;
	default:
		break;
	}
}

void crJXJRecvBattleAddArmyMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			//如果预备兵够就直接补充
			//如果不够执行军师扩展
			int code = 0;
			m_this->doEvent(MAKEINT64(WCH_JXJVIPExtend,WCH_JXJRecvAddArmy),MAKECREPARAM(&code,NULL));
			//如果返回false表示没有军师等级，执行默认逻辑
			AddArmyInfoVec armyinfo;
			unsigned char vipLevel = 0;
			char _case = -1;
			int troopRTcount =0;
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
			int perleaderneed = 0;
			int totalLeaderCount = 0;
			bool isTroopMapArmyEnough = true;
			//vector<pair<pair<int,int>,pair<unsigned short,unsigned short> > > addArmyInfo;//roleid,roleabstractid,补兵数量,需要招募的数量
			vector<crVector4i> addArmyInfo;//roleid,roleabstractid,补兵数量,需要招募的数量
			TroopsMap temptroops;
			vector<NeedArmyInfo> needArmyVec;
			NeedArmyInfo tempInfo;
			ref_ptr<crTableIO> equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
			ref_ptr<crTableIO> troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			crData *myPlayerData = m_this->getDataClass();
			//bool isHaveBirthpoint = true;
			int playerid = m_this->getPlayerID();
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && myPlayerData)
			{
				std::string addarmstr="sceneid"+crArgumentParser::appItoa(playerData->getSceneID())+"|";
				myPlayerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				myPlayerData->getParam(WCHDATA_JXJShiliID, param);
				unsigned char shiliid = *(unsigned char *)param;
				myPlayerData->getParam(WCHDATA_JXJBattleRoomID, param);
				_crInt64 battleroomid = *(_crInt64 *)param;
				int battleid = LOINT64(battleroomid);
				unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));

				//crData *gsbrainData = crServerBrainHandle::getInstance()->getDataClass();
				//if (ashili == shiliid && gsbrainData)
				//{
				//	gsbrainData->excHandle(MAKECREPARAM(WCH_LockData, 1));
				//	gsbrainData->getParam(WCHDATA_JXJCloseBattleSet, param);
				//	ClosedBattleSet * closeBtlSet = (ClosedBattleSet *)param;
				//	if (closeBtlSet && closeBtlSet->find(battleroomid) != closeBtlSet->end())
				//	{
				//		isHaveBirthpoint = false;
				//	}
				//	gsbrainData->excHandle(MAKECREPARAM(WCH_LockData, 0));
				//}

				//if (isHaveBirthpoint)
				{
					short overTimes = (short)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJOverloadTime, vipLevel).c_str()));
					int credittime = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRecruitCreditTime, vipLevel).c_str());
					int addArmyExtraFood = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleAddArmyExtraFoodNeed, vipLevel).c_str());
					bool needdeductgold = false;
					myPlayerData->excHandle(MAKECREPARAM(WCH_LockData, 1));
					//此处需要获取VIP等级
					myPlayerData->getParam(WCHDATA_JXJVipLv, param);
					vipLevel = *(unsigned char *)param;
					myPlayerData->getParam(WCHDATA_JXJCoppercash, param);
					int *mycopper = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJWood, param);
					int *mywood = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJIron, param);
					int *myiron = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJHorse, param);
					int *myhorse = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJFood, param);
					int *myfood = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJGoldingot, param);
					int *myGold = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJGiftGold, param);
					int *myGiftGold = (int *)param;
					myPlayerData->getParam(WCHDATA_JXJTroopsMap, param);
					TroopsMap *troops = (TroopsMap*)param;
					myPlayerData->getParam(WCHDATA_JXJFormationInfoVec, param);
					FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
					myPlayerData->getParam(WCHDATA_JXJRecruitTimes, param);
					crVector4i *recruitTimes = (crVector4i *)param;

					int rolesize = m_stream->_readChar();
					for (char i = 0; i < rolesize; i++)
					{
						int roleabstractid = m_stream->_readInt();
						int roleid = m_stream->_readInt();
						int rolecount = m_stream->_readInt();
						tempInfo.roleid = roleid;
						tempInfo.abstarctid = roleabstractid;
						tempInfo.needcount = rolecount;
						TroopsMap::iterator itr_troopsmap = troops->find(roleabstractid);
						if (itr_troopsmap != troops->end())
							temptroops[roleabstractid] = itr_troopsmap->second;
						needArmyVec.push_back(tempInfo);
						addarmstr += crArgumentParser::appVectoa(crVector2i(roleabstractid,rolecount));
					}

					vector<NeedArmyInfo>::iterator itr_need = needArmyVec.begin();
					for (; itr_need != needArmyVec.end(); itr_need++)
					{
						int needcount = itr_need->needcount;
						TroopsMap::iterator itr_tempmap = temptroops.find(itr_need->abstarctid);
						if (itr_tempmap != temptroops.end())
						{
							troopRTcount = itr_tempmap->second;
							if (troopRTcount < needcount)
							{
								//addArmyInfo.push_back(make_pair(make_pair(itr_need->roleid,itr_need->abstarctid),make_pair(needcount,needcount-troopRTcount)));
								addArmyInfo.push_back(crVector4i(itr_need->roleid, itr_need->abstarctid, needcount, needcount - troopRTcount));
								isTroopMapArmyEnough = false;
								itr_tempmap->second = 0;
							}
							else
							{
								//addArmyInfo.push_back(make_pair(make_pair(itr_need->roleid,itr_need->abstarctid),make_pair(needcount,0)));
								addArmyInfo.push_back(crVector4i(itr_need->roleid, itr_need->abstarctid, needcount, 0));
								itr_tempmap->second -= needcount;
							}
						}
					}
					if (isTroopMapArmyEnough)
					{
						if (vipLevel == 0)
						{
							if (*myfood >= addArmyExtraFood)
								_case = 0;
						}
						else
						{
							_case = 1;
						}
					}
					else
					{
						if (vipLevel > 1)
						{
							crTableIO::StrVec record, record2;

							// 						int troopcopperindex = troopTab->getTitleIndex("铜钱");
							// 						int troopfoodindex = troopTab->getTitleIndex("粮食");
							// 						int trooptimeIndex = troopTab->getTitleIndex("时间");
							// 						int troopequipidindex = troopTab->getTitleIndex("装备ID");
							// 						int troopequipcountindex = troopTab->getTitleIndex("装备数量");
							int leaderneedindex = troopTab->getTitleIndex("领导力");

							int equipcopperindex = equipTab->getTitleIndex("铜钱");
							int equipfoodindex = equipTab->getTitleIndex("粮食");
							int equiphorseindex = equipTab->getTitleIndex("马匹");
							int equipironindex = equipTab->getTitleIndex("铁矿");
							int equipwoodindex = equipTab->getTitleIndex("木材");
							vector<crVector4i>::iterator itr = addArmyInfo.begin();
							while (itr != addArmyInfo.end())
							{
								if (troopTab->queryOneRecord(0, crArgumentParser::appItoa((*itr)[1]), record) >= 0)
								{
									// 								if(troopcopperindex>0)
									// 									percopperneed = atoi(record[troopcopperindex].c_str());
									// 								if(troopfoodindex>0)
									// 									perfoodneed = atoi(record[troopfoodindex].c_str());
									// 								if(trooptimeIndex>0)
									// 									pertimeneed = atoi(record[trooptimeIndex].c_str());
									// 								if(troopequipidindex>0)
									// 									equipid = atoi(record[troopequipidindex].c_str());
									// 								if(troopequipcountindex)
									// 									equipneed = atoi(record[troopequipcountindex].c_str());
									if (leaderneedindex > 0)
										perleaderneed = atoi(record[leaderneedindex].c_str());
								}
								RecruitCheckResult recruitCheck;
								m_this->doEvent(WCH_JXJRecruitCheck, MAKECREPARAM((*itr)[1], &recruitCheck));
								if(recruitCheck.m_code!=1)
								{
									_case = -12;//不能招募，各种原因
									break;
								}
								copperneed += ceil((*itr)[3] * recruitCheck.m_needresourcevec[T_Copper - 1]);
								foodneed += ceil((*itr)[3] * recruitCheck.m_needresourcevec[T_Food - 1]);
								woodneed += ceil((*itr)[3] * recruitCheck.m_needresourcevec[T_Wood - 1]);
								ironneed += ceil((*itr)[3] * recruitCheck.m_needresourcevec[T_Iron - 1]);
								horseneed += ceil((*itr)[3] * recruitCheck.m_needresourcevec[T_Horse - 1]);
								timeneed += ceil((*itr)[3] * recruitCheck.m_timeNeed);

								//copperneed += (*itr)[3]*percopperneed;
								//foodneed += (*itr)[3]*perfoodneed;
								//timeneed += (*itr)[3]*pertimeneed;
								totalLeaderCount += (*itr)[3] * perleaderneed;

								if (equipTab->queryOneRecord(0, crArgumentParser::appItoa(recruitCheck.m_equipid), record2) >= 0)
								{
									if (equipcopperindex > 0)
										percopperneed = atoi(record2[equipcopperindex].c_str());
									if (equipfoodindex > 0)
										perfoodneed = atoi(record2[equipfoodindex].c_str());
									if (equiphorseindex > 0)
										perhorseneed = atoi(record2[equiphorseindex].c_str());
									if (equipironindex > 0)
										perironneed = atoi(record2[equipironindex].c_str());
									if (equipwoodindex)
										perwoodneed = atoi(record2[equipwoodindex].c_str());
								}
								copperneed += percopperneed * recruitCheck.m_equipCount *(*itr)[3];
								woodneed += perwoodneed * recruitCheck.m_equipCount *(*itr)[3];
								ironneed += perironneed * recruitCheck.m_equipCount * (*itr)[3];
								horseneed += perhorseneed * recruitCheck.m_equipCount * (*itr)[3];
								foodneed += perfoodneed * recruitCheck.m_equipCount *(*itr)[3];

								itr++;
							}
							if(_case != -12)
							{
								int timeRemain = 0;
								for (int i = 0; i<4; i++)
								{
									if ((*recruitTimes)[i] >= 0 && (*recruitTimes)[i] < overTimes)
										timeRemain += overTimes - (*recruitTimes)[i] + credittime;
								}
								if (copperneed > *mycopper || woodneed > *mywood || ironneed > *myiron || horseneed > *myhorse || foodneed > *myfood || timeneed > timeRemain)
								{
									if (vipLevel >= 3)
									{
										int goldneed = totalLeaderCount / 100 + 10;
										if (goldneed <= *myGold + *myGiftGold)
										{
											_case = 3;
										}
									}
								}
								else
								{
									_case = 2;
								}
							}
						}
					}
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					if (_case >= 0)
					{
						ref_ptr<crStreamBuf> stream_sceneserver = new crStreamBuf;
						stream_sceneserver->createBuf(6 + addArmyInfo.size() * 6);
						stream_sceneserver->_writeInt(playerid);
						stream_sceneserver->_writeChar(addArmyInfo.size());
						//兵力处理
						vector<crVector4i>::iterator itr = addArmyInfo.begin();
						//unsigned short remainArmyCount = 0;
						while (itr != addArmyInfo.end())
						{
							int count = (*itr)[2] - (*itr)[3];//补兵数量－招募数量
							TroopsMap::iterator itr_troopsmap = troops->find((*itr)[1]);
							if (itr_troopsmap != troops->end())
								itr_troopsmap->second -= count;
							FormationInfoVec::iterator itr_for = formationInfoVec->begin();
							while (itr_for != formationInfoVec->end())
							{
								if ((*itr_for)->getRoleID() == (*itr)[0])
								{
									(*itr_for)->setCount((*itr)[2]);
									break;
								}
								itr_for++;
							}
							stream_sceneserver->_writeInt((*itr)[0]);
							stream_sceneserver->_writeUShort((*itr)[2]);
							//stream_sceneserver->_writeUShort(itr->second.second);
							itr++;
						}
						myPlayerData->getParam(WCHDATA_JXJInspireLevel, param);
						unsigned char inspireLv = *(unsigned char *)param;

						stream_sceneserver->_writeUChar(inspireLv);
						crJXJBattleAddArmyInfoPacket packet_scene;
						crJXJBattleAddArmyInfoPacket::buildRequestPacket(packet_scene, stream_sceneserver.get());
						gameServer->getNetManager()->sendPacket(playerData->getSceneServerAddress(), packet_scene);

						if (_case == 0)
						{
							//粮食扣除
							(*myfood) -= addArmyExtraFood;
							//发包
							stream->createBuf(6 + addArmyInfo.size() * 8);
							stream->_writeChar(_case);
							stream->_writeChar(addArmyInfo.size());
							stream->_writeInt(*myfood);
							itr = addArmyInfo.begin();
							while (itr != addArmyInfo.end())
							{
								stream->_writeInt((*itr)[0]);
								stream->_writeUShort((*itr)[2]);
								stream->_writeUShort((*itr)[3]);
								itr++;
							}
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvAddArmy, stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
						}
						else if (_case == 1)
						{
							//只扣兵力
							stream->createBuf(2 + addArmyInfo.size() * 8);
							stream->_writeChar(_case);
							stream->_writeChar(addArmyInfo.size());
							itr = addArmyInfo.begin();
							while (itr != addArmyInfo.end())
							{
								stream->_writeInt((*itr)[0]);
								stream->_writeUShort((*itr)[2]);
								stream->_writeUShort((*itr)[3]);
								itr++;
							}
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvAddArmy, stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
						}
						else if (_case == 2)
						{
							//扣除资源
							*mycopper -= copperneed;
							*myfood -= foodneed;
							*myiron -= ironneed;
							*mywood -= woodneed;
							*myhorse -= horseneed;

							for (int i = 0; timeneed > 0 && i < 4; i++)
							{
								if ((*recruitTimes)[i] >= 0 && (*recruitTimes)[i]<overTimes)
								{
									int t = overTimes + credittime - (*recruitTimes)[i];
									if (timeneed>t)
									{
										(*recruitTimes)[i] += t;
										timeneed -= t;
									}
									else
									{
										(*recruitTimes)[i] += timeneed;
										timeneed = 0;
									}
								}
							}

							stream->createBuf(38 + addArmyInfo.size() * 8);
							stream->_writeChar(_case);
							stream->_writeChar(addArmyInfo.size());
							stream->_writeInt(*mycopper);
							stream->_writeInt(*myfood);
							stream->_writeInt(*myiron);
							stream->_writeInt(*mywood);
							stream->_writeInt(*myhorse);
							stream->_writeVec4i(*recruitTimes);
							itr = addArmyInfo.begin();
							while (itr != addArmyInfo.end())
							{
								stream->_writeInt((*itr)[0]);
								stream->_writeUShort((*itr)[2]);
								stream->_writeUShort((*itr)[3]);
								itr++;
							}
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvAddArmy, stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
						}
						else if (_case == 3)
						{
							//元宝扣除
							int goldneed = totalLeaderCount / 100 + 10;

							// 						*myGiftGold-=goldneed;
							// 						if(*myGiftGold<0)
							// 						{
							// 							(*myGold)+=*myGiftGold;
							// 							*myGiftGold = 0;
							// 						}

							MoneyChangeData moneydata(goldneed, "战场补兵");
							m_this->doEvent(WCH_JXJDeductLijinGold, MAKECREPARAM(&moneydata, &needdeductgold));

							////元宝扣除增加成长值
							//myPlayerData->getParam(WCHDATA_JXJVipLv,param);
							//unsigned char vipLevel = *(unsigned char *)param;
							//int goldToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGoldToVipExpFactor, vipLevel).c_str());
							//m_this->doEvent(WCH_JXJAddVipExp,MAKECREPARAM(goldneed * goldToVipExpFactor,NULL));
							//发包
							stream->createBuf(2+addArmyInfo.size()*8);
							stream->_writeChar(_case);
							stream->_writeChar(addArmyInfo.size());
							itr = addArmyInfo.begin();
							while (itr!=addArmyInfo.end())
							{
								stream->_writeInt((*itr)[0]);
								stream->_writeUShort((*itr)[2]);
								stream->_writeUShort((*itr)[3]);
								itr++;
							}
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvAddArmy,stream.get());
							gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					else if (_case < 0)
					{
						stream->createBuf(1);
						stream->_writeChar(_case);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvAddArmy, stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
					}
					myPlayerData->excHandle(MAKECREPARAM(WCH_LockData, 0));
					if (needdeductgold)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
						callback->savePlayerData(playerData.get());
					}
					///游戏日志
					std::string logdata = "战场补兵0扣粮食1扣兵力2扣资源3扣元宝:" + crArgumentParser::appItoa(_case)+addarmstr;
					GameLogData gamelog(Log_SupplementArmy, logdata);
					crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(playerid, &gamelog));
				}
				//else
				//{
				//	int playerid = m_this->getPlayerID();
				//	ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
				//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//	_case = -9;//没有出生点
				//	stream->createBuf(1);
				//	stream->_writeChar(_case);
				//	crPlayerDataEventPacket packet;
				//	crPlayerDataEventPacket::buildReplyPacket(packet, playerid, WCH_JXJRecvAddArmy, stream.get());
				//	gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(), packet);
				//}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crData *myPlayerData = m_this->getDataClass();
			if(myPlayerData)
			{
				char _case = m_stream->_readChar();
				if(_case>=0)
				{
					myPlayerData->excHandle(MAKECREPARAM(WCH_LockData, 1));
					char size = m_stream->_readChar();
					if(_case==0)
					{
						int food = m_stream->_readInt();
						myPlayerData->inputParam(WCHDATA_JXJFood,&food);
					}
					else if(_case==2)
					{
						int coppercash = m_stream->_readInt();
						int food = m_stream->_readInt();
						int wood = m_stream->_readInt();
						int iron = m_stream->_readInt();
						int horse = m_stream->_readInt();
						crVector4i recruitTimes = m_stream->_readVec4i();

						myPlayerData->inputParam(WCHDATA_JXJCoppercash,&coppercash);
						myPlayerData->inputParam(WCHDATA_JXJFood,&food);
						myPlayerData->inputParam(WCHDATA_JXJWood,&wood);
						myPlayerData->inputParam(WCHDATA_JXJIron,&iron);
						myPlayerData->inputParam(WCHDATA_JXJHorse,&horse);
						myPlayerData->inputParam(WCHDATA_JXJRecruitTimes,&recruitTimes);
					}
					//else if(_case==3)
					//{
					// 						int gold = m_stream->_readInt();
					// 						int giftGold = m_stream->_readInt();
					// 						myPlayerData->inputParam(WCHDATA_JXJGoldingot,&gold);
					// 						myPlayerData->inputParam(WCHDATA_JXJGiftGold,&giftGold);
					//}
					std::vector<std::pair<int,std::pair<unsigned short,unsigned short>>> count_vec;
					vector<pair<int,pair<unsigned short,unsigned short>>> addArmyInfo;
					for(char i =0;i<size;i++)
					{
						int roleid = m_stream->_readInt();
						int count = m_stream->_readUShort();
						int needcount = m_stream->_readUShort();
						count_vec.push_back(std::make_pair(roleid,std::make_pair(count,needcount)));
					}
					myPlayerData->getParam(WCHDATA_JXJFormationInfoVec,param);
					FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
					myPlayerData->getParam(WCHDATA_JXJTroopsMap,param);
					TroopsMap *troopsMap = (TroopsMap *)param;

					std::vector<std::pair<int,std::pair<unsigned short,unsigned short>>>::iterator itr = count_vec.begin();
					crRole *role = NULL;
					crMyPlayerData *me = crMyPlayerData::getInstance();
					while (itr!=count_vec.end())
					{
						int count = itr->second.first-itr->second.second;

						for(int i = 0;i<3;i++)
						{
							role = me->getLoadedRoleByIndex(i);
							if(!role)
								continue;
							int roleid = role->getRoleID();
							if(roleid == itr->first)
							{
								TroopsMap::iterator itr_troopsmap = troopsMap->find(role->getAbstractItemID());
								if(itr_troopsmap!=troopsMap->end())
								{
									itr_troopsmap->second-=count;
									// 							if(itr_troopsmap->second<0)
									// 								itr_troopsmap->second = 0;
								}
								addArmyInfo.push_back(make_pair(role->getAbstractItemID(),make_pair(itr->second.first,itr->second.second)));
							}
						}
						FormationInfoVec::iterator itr_for = formationInfoVec->begin();
						while (itr_for!=formationInfoVec->end())
						{
							if((*itr_for)->getRoleID() == itr->first)
							{
								(*itr_for)->setCount(itr_for->get()->getCount()+itr->second.first);
								break;
							}
							itr_for++;
						}
						itr++;
					}
					ref_ptr<crTableIO> equipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsEquipTab);
					ref_ptr<crTableIO> troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
					myPlayerData->getParam(WCHDATA_JXJTroopsEquipMap,param);
					TroopsEquipMap *equipmap = (TroopsEquipMap *)param;

					int	equipneed = 0,
						equipid = 0;
					crTableIO::StrVec record,record2;
					vector<pair<int,pair<unsigned short,unsigned short> > >::iterator itr_info = addArmyInfo.begin();
					map<int,int> equip_needmap;//id,count
					while (itr_info!=addArmyInfo.end())
					{
						troopTab->queryOneRecord(0,crArgumentParser::appItoa(itr_info->first),record);
						equipid = atoi(record[troopTab->getTitleIndex("装备ID")].c_str());
						equipneed = atoi(record[troopTab->getTitleIndex("装备数量")].c_str())*itr_info->second.second;
						equip_needmap[equipid]+=equipneed;
						itr_info++;
					}
					map<int,int>::iterator itr_equip = equip_needmap.begin();
					int count = 0;
					while (itr_equip!=equip_needmap.end())
					{
						TroopsEquipMap::iterator itr_troopequip = equipmap->find(itr_equip->first);
						if(itr_troopequip!=equipmap->end())
						{
							count = itr_troopequip->second;
							equipmap->find(itr_equip->first)->second -= (count>itr_equip->second)?itr_equip->second:count;
						}
						itr_equip++;
					}
					myPlayerData->excHandle(MAKECREPARAM(WCH_LockData, 0));
				}
				else
				{
					//补兵失败
					ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
					ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
					if (canvas.valid() && camera.valid())
					{
						ref_ptr<crStaticTextWidgetNode> text = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strText));
						if (text.valid())
						{
							//if (_case = -9)
							//{
							//	text->setString(std::string("出生点全部失守，点击确定后退出"));
							//	crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
							//}
							//else
							//{
								text->setString(std::string("补兵失败！点击确定后退出"));
								crFilterRenderManager::getInstance()->showCanvas(canvas.get(), true);
							//}
							crData *camadata = camera->getAttachedNode()->getDataClass();
							short timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJExitTimerLimit, 0).c_str());
							camadata->inputParam(WCHDATA_JXJTimer, &timer);
						}
					}

				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//crJXJVIPAddArmyMethod
//////////////////////////////////////////////////////////////////////////
crJXJVIPAddArmyMethod::crJXJVIPAddArmyMethod():
	m_code(0){}
crJXJVIPAddArmyMethod::crJXJVIPAddArmyMethod(const crJXJVIPAddArmyMethod& handle):
	crMethod(handle),
	m_code(0)
{
}
void crJXJVIPAddArmyMethod::inputParam(int i, void *param)
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
			m_code = (int *)(LOCREPARAM(param64));
		}
		break;
	}
}

void crJXJVIPAddArmyMethod::addParam(int i, const std::string& str)
{
}

void crJXJVIPAddArmyMethod::operator()(crHandle &handle)
{
	void *param;
	crData *data = m_this->getDataClass();
	//此处需要获取VIP等级
	data->getParam(WCHDATA_JXJVipLv,param);
	unsigned char vipLevel = *(unsigned char *)param;
	if(vipLevel>0)
	{
		*m_code = 1;
	}
}
/////////////////////////////////////////
//
//crUIJXJRecvDamageMethod//第三版UI功能
//
/////////////////////////////////////////
crUIJXJRecvDamageMethod::crUIJXJRecvDamageMethod(){}
crUIJXJRecvDamageMethod::crUIJXJRecvDamageMethod(const crUIJXJRecvDamageMethod& handle):
	crMethod(handle)
{
}
void crUIJXJRecvDamageMethod::inputParam(int i, void *param)
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

void crUIJXJRecvDamageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_f1sequence = str;
		break;
	case 1:
		m_f2sequence = str;
		break;
	case 2:
		m_f3sequence = str;
		break;
	case 3:
		m_canvas = str;
		break;
	}
}

void crUIJXJRecvDamageMethod::operator()(crHandle &handle)
{
	//客户端显示
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crSequence>  seqf1 = dynamic_cast<crSequence*>(canvas->getChildNode(m_f1sequence));
		ref_ptr<crSequence>  seqf2 = dynamic_cast<crSequence*>(canvas->getChildNode(m_f2sequence));
		ref_ptr<crSequence>  seqf3 = dynamic_cast<crSequence*>(canvas->getChildNode(m_f3sequence));
		seqf1->setVanishWhenStop(true);
		seqf2->setVanishWhenStop(true);
		seqf3->setVanishWhenStop(true);
		ref_ptr<crSequence>  seq = NULL;
		for (int i = 0;i<3;i++)
		{
			switch (i)
			{
			case 0:
				seq = seqf1;
				break;
			case 1:
				seq = seqf2;
				break;
			case 2:
				seq = seqf3;
				break;
			}
			crMyPlayerData *me = crMyPlayerData::getInstance();
			crRole *role = me->getLoadedRoleByIndex(i);
			if(!role)
				continue;
			if(crMyPlayerData::getInstance()->ifItemIsMe(m_this))
			{
				if(role->getRoleID()==m_this->getRoleID() && seq->getMode() == crSequence::STOP)
				{
					seq->setVisiable(true);
					seq->setMode(crSequence::START);
					seq->setDuration(0.1,5);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJCameraKeyMoveMethod
//
/////////////////////////////////////////
crJXJCameraKeyMoveMethod::crJXJCameraKeyMoveMethod():
	m_director(-1)
{
}
crJXJCameraKeyMoveMethod::crJXJCameraKeyMoveMethod(const crJXJCameraKeyMoveMethod& handle):
	crMethod(handle),
	m_director(handle.m_director)
{
}
void crJXJCameraKeyMoveMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}
void crJXJCameraKeyMoveMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_director = atoi(str.c_str());
		break;
	}
}
void crJXJCameraKeyMoveMethod::operator()(crHandle &handle)
{
	unsigned char cameraMove = crCameraData::None;
	switch (m_director)
	{
	case 0:
		cameraMove |= crCameraData::MoveLeft;
		break;
	case 1:
		cameraMove |= crCameraData::MoveRight;
		break;
	case 2:
		cameraMove |= crCameraData::MoveDown;
		break;
	case 3:
		cameraMove |= crCameraData::MoveUp;
		break;
	}
	crData *data = m_this->getDataClass();
	data->inputParam(WCHDATA_CameraMove,&cameraMove);
}

/////////////////////////////////////////
//
//crJXJMouseOnRoleMethod
//
/////////////////////////////////////////
crJXJMouseOnRoleMethod::crJXJMouseOnRoleMethod()
{
}

crJXJMouseOnRoleMethod::crJXJMouseOnRoleMethod(const crJXJMouseOnRoleMethod& handle):
	crMethod(handle)
{
}

void crJXJMouseOnRoleMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		break;
	}
}

void crJXJMouseOnRoleMethod::addParam(int i, const std::string& str)
{
}

void crJXJMouseOnRoleMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crInstanceItem *targetItem = NULL;
		void *param;
		crNode *fxNode;
		crDecalUpdateCallback *callback;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		targetItem = ((crInstanceItem*)m_this);
		if(targetItem)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJBattleMouseOnRoleVec,param);
			ItemVec *mouseOnItemVec = (ItemVec *)param;
			ItemVec::iterator itr_item = mouseOnItemVec->begin();
			while (itr_item!=mouseOnItemVec->end())
			{
				if(itr_item->get() == targetItem)
					break;
				itr_item++;
			}
			if(itr_item == mouseOnItemVec->end())
			{
				if(!mouseOnItemVec->empty())
				{
					itr_item = mouseOnItemVec->begin();
					while (itr_item!=mouseOnItemVec->end())
					{
						fxNode = itr_item->get()->findSceneFxNode("$MouseOnDecal");
						if(fxNode)
						{
							callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
							if(callback)
							{
								callback->setVisiable(false);
								crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
								itr_item->get()->getRelNode()->accept(selectNodeVisitor);
							}
						}
						itr_item++;
					}
					mouseOnItemVec->resize(0);
				}
				mouseOnItemVec->push_back(targetItem);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJMouseOnRoleDecalUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJMouseOnRoleDecalUpdateMethod::crJXJMouseOnRoleDecalUpdateMethod():
	m_addRadius(0.0f)
{
}

crJXJMouseOnRoleDecalUpdateMethod::crJXJMouseOnRoleDecalUpdateMethod(const crJXJMouseOnRoleDecalUpdateMethod& handle):
	crMethod(handle),
	m_addRadius(handle.m_addRadius),
	m_myColor(handle.m_myColor),
	m_friendColor(handle.m_friendColor),
	m_enemyColor(handle.m_enemyColor),
	m_otherColor(handle.m_otherColor)
{
}

void crJXJMouseOnRoleDecalUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJMouseOnRoleDecalUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_addRadius = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_myColor);
		m_myColor /= 255.0f;
		break;
	case 2:
		crArgumentParser::appAtoVec(str,m_friendColor);
		m_friendColor /= 255.0f;
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_enemyColor);
		m_enemyColor /= 255.0f;
		break;
	case 4:
		crArgumentParser::appAtoVec(str,m_otherColor);
		m_otherColor /= 255.0f;
		break;
	}
}

void crJXJMouseOnRoleDecalUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crData *cameraData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_JXJBattleMouseOnRoleVec,param);
	ItemVec mouseOnRoleVec = *(ItemVec *)param;
	cameraData->getParam(WCHDATA_SelectItem, param);
	crInstanceItem *selectItem = (crInstanceItem*)param;

	crNode *fxNode;
	crDecalUpdateCallback *callback;
	crMatrixTransform *bot;
	crVector3 pos;
	crData *data;
	crVector2 lengths;
	crVector3 halfPhysicsBox;
	char isEnemy = 0;
	ItemVec::iterator itr_item = mouseOnRoleVec.begin();
	for (;itr_item!=mouseOnRoleVec.end();itr_item++)
	{
		crInstanceItem *targetItem = itr_item->get();
		if(targetItem)
		{
			data = targetItem->getDataClass();
			crRole *me = crMyPlayerData::getInstance()->getCurrentRole();		
			me->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem,&isEnemy));
			if(targetItem)
			{
				fxNode = targetItem->findSceneFxNode("$MouseOnDecal");
				if(fxNode)
				{
					callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						bot = dynamic_cast<crMatrixTransform *>(targetItem->getRelNode());
						if (!bot || !bot->getVisiable())
						{
							callback->setVisiable(false);
							continue;
						}
						else
						{
							if (targetItem != selectItem)
								callback->setVisiable(true);
							if(isEnemy == -1)
							{
								crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Red);
								bot->accept(selectNodeVisitor);
								callback->setColor(m_enemyColor);
							}
							else if(isEnemy == 1)
							{
								crRole *me = crMyPlayerData::getInstance()->getCurrentRole();	
								int myRoleID = me->getRoleID();
								if(crMyPlayerData::getInstance()->ifItemIsMe(targetItem))
								{
									crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Green);
									bot->accept(selectNodeVisitor);
									callback->setColor(m_myColor);
								}
								else	
								{
									callback->setColor(m_friendColor);
									crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Blue);
									bot->accept(selectNodeVisitor);
								}
							}
							else
							{
								crSetObjectOutlineColorModeVisitor selectNodeVisitor(crObject::OL_Black);
								bot->accept(selectNodeVisitor);
								callback->setColor(m_otherColor);
							}
						}
						pos = bot->getTrans();
						callback->setPosition(crVector2(pos[0],pos[1]));
						data->getParam(WCHDATA_HalfPhysicsBox,param);
						halfPhysicsBox = *((crVector3*)(param));
						lengths.set(halfPhysicsBox[0],halfPhysicsBox[1]);
						lengths *= 2.0f;
						lengths[0] = lengths.length()+m_addRadius;
						lengths[1] = lengths[0];
						callback->setSize(lengths);
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
crJXJUseSkillRangeDecalUpdateMethod::crJXJUseSkillRangeDecalUpdateMethod():
	m_zoffset(0.0f)
{
}

crJXJUseSkillRangeDecalUpdateMethod::crJXJUseSkillRangeDecalUpdateMethod(const crJXJUseSkillRangeDecalUpdateMethod& handle):
	crMethod(handle),
	m_zoffset(handle.m_zoffset)
{
}

void crJXJUseSkillRangeDecalUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJUseSkillRangeDecalUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	}
}

void crJXJUseSkillRangeDecalUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crVector3 pos;
	crMatrixTransform *bot;
	crData *cameraData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	cameraData->getParam(WCHDATA_JXJRoleAboutToUseItemID,param);
	AboutUseItemPair *useItemPair = (AboutUseItemPair *)param;
	cameraData->getParam(WCHDATA_JXJUseSkillRangeNodeMap,param);
	SkillRangeNodeMap *skillNodeMap = (SkillRangeNodeMap *)param;
	CRCore::crNode *decalNode = NULL;
	SkillRangeNodeMap::iterator itr_node = skillNodeMap->begin();
	for (;itr_node!=skillNodeMap->end();itr_node++)
	{
		if(itr_node->second.valid() && itr_node->second->getVisiable())
		{
			decalNode = itr_node->second.get();
		}
	}
	if(decalNode && decalNode->getVisiable())
	{
		if(useItemPair && useItemPair->first.valid())
		{
			crRole *role = useItemPair->first.get();				
			crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(decalNode);
			if(skillRangeMatrix)
			{
				bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
				if(!bot || !bot->getVisiable())
				{
					skillRangeMatrix->setVisiable(false);
				}
				else
				{
					pos = bot->getTrans();
					skillRangeMatrix->setPosition(crVector3f(pos[0],pos[1],pos[2]+1.5));
					
					crItemChild *itemChild = role->findChildItem(useItemPair->second);
					if (itemChild && itemChild->isLoaded())
					{
						crInstanceItem *item = itemChild->getInstanceItem();
						crData *itemData = item->getDataClass();
						itemData->getParam(WCHDATA_ItemUseRange, param);
						unsigned short range = *(unsigned short *)param;
// 						if (range > 0)
// 						{
// 							//float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
// // 							crVector3f centerPos(pos[0], pos[1], pos[2] + m_zoffset);
// // 							m_this->doEvent(WCH_JXJSkillEffectRange, MAKECREPARAM(&centerPos, &fscope));
// 						}
					}
				}
			}
		}
	}

	cameraData->getParam(WCHDATA_JXJPreUseSkillRangeDecal,param);
	crNode *preDecalNode = (crNode *)param;
	cameraData->getParam(WCHDATA_JXJPreUseSkillRange,param);
	unsigned short range = *(unsigned short*)param;
	cameraData->getParam(WCHDATA_JXJPreUseSkillRole,param);
	crRole *rolePreUseSkill = (crRole *)param;
	cameraData->getParam(WCHDATA_JXJPreUseSkillDecal,param);
	crNode *preScopeDecalNode = (crNode *)param;
	cameraData->getParam(WCHDATA_JXJPreUseSkillScope,param);
	short scope = *(short *)param;
	cameraData->getParam(WCHDATA_JXJPreUseSkillEffectTargetType, param);
	unsigned char effectTargetType = *(unsigned char *)param;

	if(preDecalNode && range>0 && rolePreUseSkill && preDecalNode->getVisiable())
	{
		crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(preDecalNode);
		if(skillRangeMatrix)
		{
			crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(rolePreUseSkill->getRelNode());
			if(!bot || !bot->getVisiable())
			{
				skillRangeMatrix->setVisiable(false);
			}
			else
			{
				//skillRangeMatrix->setVisiable(true);
				pos = bot->getTrans();
				crVector3f dir = rolePreUseSkill->getDir();
				CRCore::crMatrix newMat;
				if(dir == crVector3(0.0f,0.0f,0.0f))
				{
					dir[1] = -1.0f;
				}
				if(dir[1] > 0.999)
				{
					newMat.makeRotate(-CRCore::Y_AXIS,dir);
					CRCore::crVector3d ep,center,up;
					newMat.getLookAt(ep,center,up);
					if(center[2]>0.0f)
					{
						center = -center;
					}
					newMat.makeLookAt(ep,center,up);
				}
				else
					newMat.makeRotate(-CRCore::Y_AXIS,dir);
				float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
				//skillRangeMatrix->setMatrix(newMat*crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
				if(fscope<300.0f)
					skillRangeMatrix->setMatrix(newMat*crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
				else
					skillRangeMatrix->setMatrix(newMat*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));

				//////////////////////////////////////////////////////////////////////////
// 				crVector3f centerPos(pos[0], pos[1], pos[2] + m_zoffset);
// 				m_this->doEvent(WCH_JXJSkillEffectRange, MAKECREPARAM(&centerPos,&fscope));
			}
		}
	}
	if(preScopeDecalNode && scope>0 && rolePreUseSkill && preScopeDecalNode->getVisiable())
	{
		ref_ptr<crTableIO> skillDataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
		int effectIndex = skillDataAttrTab->getTitleIndex("作用类型");
		crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(preScopeDecalNode->getUpdateCallback("DecalUpdate"));
		if(callback)
		{
			crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(rolePreUseSkill->getRelNode());
			if(!bot || !bot->getVisiable())
			{
				callback->setVisiable(false);
			}
			else
			{
				//callback->setVisiable(true);
				callback->setColor(crVector4(0,1,0,1));
				pos = bot->getTrans();
				float fscope = (float)scope * crGlobalHandle::gData()->gUnitScale();
				float radius = fscope;
				fscope *= 2.0f;
				callback->setPosition(crVector2(pos[0],pos[1]));
				callback->setSize(crVector2(fscope,fscope));

				crVector2f vec;
				vec[0] = radius;
				vec[1] = effectTargetType;
				m_this->doEvent(WCH_JXJSkillEffectRange, MAKECREPARAM(&pos, &vec));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvPlayerRewardInfoMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvPlayerRewardInfoMethod::crJXJRecvPlayerRewardInfoMethod(){}
crJXJRecvPlayerRewardInfoMethod::crJXJRecvPlayerRewardInfoMethod(const crJXJRecvPlayerRewardInfoMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_playerRewardData(NULL)
{

}
void crJXJRecvPlayerRewardInfoMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_playerRewardData = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_playerRewardData = (crPlayerRewardData *)(LOCREPARAM(param64));
		}
		else
		{
			m_playerRewardData = NULL;
		}
		break;
	}
}

void crJXJRecvPlayerRewardInfoMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvPlayerRewardInfoMethod::operator ()(crHandle &handle)
{
	if(m_this && m_this->getMainRole() && m_this->getMainRole()->getDataClass() && m_playerRewardData.valid())
	{
		int rewardtype = m_playerRewardData->getType();
		std::string str,rewardstr;
		switch (rewardtype)
		{
		case GP_RewardGameEnd:
			str = "国战奖励";
			break;
		case GP_RewardDropItems:
			str = "战场掉落";
			break;
		case GP_OnlineReward:
			str = "在线礼包";
			break;
		case GP_DailyLandReward:
			str = "每日登录抽奖";
			break;
		case GP_RewardFubenEnd:
			str = "副本奖励";
			break;
		case GP_MailReward:
			str = "邮件奖励";
			break;
		case GP_RewardFubenSuccess:
			str = "副本抽奖";
			break;
		case GP_RewardFubenDropItems:
			str = "副本掉落";
			break;
		case GP_RewardFubenRush:
			str = "副本扫荡";
			break;
		case GP_RewardPingpan:
			str = "平叛奖励";
			break;
		case GP_RewardShuofu:
			str = "说服奖励";
			break;
		case GP_RewardXiaozhouka:
			str = "小周卡奖励";
			break;
		default:
			break;
		}
		MoneyChangeData  moneydata;//(itr->second,str);
		crData *data =  m_this->getDataClass();
		crRole *mainRole = m_this->getMainRole();
		//crData *maindata = mainRole->getDataClass();
		//int playerid = m_this->getPlayerID();
		//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));

		if(m_playerRewardData->getCooperCashReward()!=0)
		{
			//data->excHandle(MAKECREPARAM(WCH_LockData,1));
			//data->getParam(WCHDATA_JXJCoppercash,param);
			//int *coppercash = (int *)param;
			//*coppercash += m_playerRewardData->getCooperCashReward();
			//data->excHandle(MAKECREPARAM(WCH_LockData,0));
			rewardstr += ",铜钱" + crArgumentParser::appItoa(m_playerRewardData->getCooperCashReward());
			m_this->doEvent(WCH_JXJRecvCoppercash,MAKECREPARAM(m_playerRewardData->getCooperCashReward(),NULL));
		}
		if(m_playerRewardData->getExperience()!=0)
		{
			rewardstr += ",经验" + crArgumentParser::appItoa(m_playerRewardData->getExperience());
			mainRole->doEvent(WCH_JXJRecvExperience,MAKECREPARAM(m_playerRewardData->getExperience(),NULL));
		}
		if(m_playerRewardData->getExploit()!=0)
		{
			rewardstr += ",功勋" + crArgumentParser::appItoa(m_playerRewardData->getExploit());
			mainRole->doEvent(WCH_JXJRecvExploit,MAKECREPARAM(m_playerRewardData->getExploit(),NULL));
		}
		if (m_playerRewardData->getScore()!=0)
		{
			rewardstr += ",国战积分" + crArgumentParser::appItoa(m_playerRewardData->getScore());
			m_this->doEvent(WCH_JXJRecvGuoZhanJiFen, MAKECREPARAM(m_playerRewardData->getScore(), NULL));
		}
		if(m_playerRewardData->getAchievement()!=0)
		{
			rewardstr += ",战功" + crArgumentParser::appItoa(m_playerRewardData->getAchievement());
			mainRole->doEvent(WCH_JXJRecvAchievement,MAKECREPARAM(m_playerRewardData->getAchievement(),NULL));
		}
		if(m_playerRewardData->getFood()!=0)
		{
			rewardstr += ",粮食" + crArgumentParser::appItoa(m_playerRewardData->getFood());
			m_this->doEvent(WCH_JXJRecvFood,MAKECREPARAM(m_playerRewardData->getFood(),NULL));
		}
		if(m_playerRewardData->getWood()!=0)
		{
			rewardstr += ",木头" + crArgumentParser::appItoa(m_playerRewardData->getWood());
			m_this->doEvent(WCH_JXJRecvWood,MAKECREPARAM(m_playerRewardData->getWood(),NULL));
		}
		if(m_playerRewardData->getIron()!=0)
		{
			rewardstr += ",铁矿" + crArgumentParser::appItoa(m_playerRewardData->getIron());
			m_this->doEvent(WCH_JXJRecvIron,MAKECREPARAM(m_playerRewardData->getIron(),NULL));
		}
		if(m_playerRewardData->getHorse()!=0)
		{
			rewardstr += ",马匹" + crArgumentParser::appItoa(m_playerRewardData->getHorse());
			m_this->doEvent(WCH_JXJRecvHorse,MAKECREPARAM(m_playerRewardData->getHorse(),NULL));
		}
		if(m_playerRewardData->getGiftgold()!=0)
		{
			moneydata.first = m_playerRewardData->getGiftgold();
			moneydata.second = str;
			m_this->doEvent(WCH_JXJRecvGiftgold,MAKECREPARAM(&moneydata,NULL));
		}
		if(m_playerRewardData->getCountryContribution()!=0)
		{
			rewardstr += ",国家贡献" + crArgumentParser::appItoa(m_playerRewardData->getCountryContribution());
			m_this->doEvent(WCH_JXJRecvCountryContribute,MAKECREPARAM(m_playerRewardData->getCountryContribution(),NULL));
		}
		if (m_playerRewardData->getGroupContribution() != 0)
		{
			rewardstr += ",军团贡献" + crArgumentParser::appItoa(m_playerRewardData->getGroupContribution());
			m_this->doEvent(WCH_JXJRecvGroupContribution, MAKECREPARAM(m_playerRewardData->getGroupContribution(), NULL));
		}
		if(!m_playerRewardData->getEquipsVec().empty())
		{
			RewardEquipsVec& rewardEquipsVec = m_playerRewardData->getEquipsVec();
			m_this->doEvent(WCH_JXJRecvRewardEquips,MAKECREPARAM(&rewardEquipsVec,NULL));
		}
		if(!m_playerRewardData->getRewardItemsVec().empty())
		{
			RewardItemVec& rewardItems = m_playerRewardData->getRewardItemsVec();
			unsigned char type = m_playerRewardData->getType();
			for (RewardItemVec::iterator itr = rewardItems.begin(); itr != rewardItems.end(); ++itr)
			{
				rewardstr += ",道具" + crArgumentParser::appVectoa(crVector3i((*itr)->getItemID(), (*itr)->getItemCount(), (*itr)->getEquipMagic()));
			}
			m_this->doEvent(WCH_JXJRecvRewardItems,MAKECREPARAM(&rewardItems,type));
		}
		if (m_playerRewardData->getHuoyuedu() == 1)
		{
			rewardstr += ",活跃度" + crArgumentParser::appItoa(m_playerRewardData->getHuoyuedu());
			m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKECREPARAM(HYD_BattleGround, 1));
		}
		if (m_playerRewardData->getwarcounts())
		{
			int _type = 2;	//攻城掠地活动
			m_this->doEvent(WCH_JXJUpdate7DayHuoDong,MAKECREPARAM(_type,NULL));
			_type = 3;	//血战沙场活动
			m_this->doEvent(WCH_JXJUpdate7DayHuoDong,MAKECREPARAM(_type,NULL));
			_type = 4;	//视死如归活动
			m_this->doEvent(WCH_JXJUpdate7DayHuoDong,MAKECREPARAM(_type,NULL));
		}
		if (!rewardstr.empty())
		{
			GameLogData gamelog(Log_RecvPlayerRewardInfo, str + rewardstr);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog, MAKECREPARAM(m_this->getPlayerID(), &gamelog));
		}
	}
}

/////////////////////////////////////////
//
//crJXJCreateRewardMailMethod
//
/////////////////////////////////////////
crJXJCreateRewardMailMethod::crJXJCreateRewardMailMethod(){}
crJXJCreateRewardMailMethod::crJXJCreateRewardMailMethod(const crJXJCreateRewardMailMethod& handle):
	crMethod(handle)
{
}
void crJXJCreateRewardMailMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_playerRewardData = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_playerRewardData = (crPlayerRewardData *)(LOCREPARAM(param64));
		}
		else
		{
			m_playerRewardData = NULL;
		}
		break;
	}
}

void crJXJCreateRewardMailMethod::addParam(int i, const std::string& str)
{
}

void crJXJCreateRewardMailMethod::operator()(crHandle &handle)
{
	if(m_this!=NULL && m_playerRewardData!=NULL)
	{
		int playerid = m_playerRewardData->getPlayerID();
		//RewardItemVec& rewardItems = m_playerRewardData->getRewardItemsVec();
		//RewardItemVec::iterator itr_rewarditem = rewardItems.begin();
		//ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
		//unsigned char packet_type = m_playerRewardData->getType();
		//if(packet_type == GP_RewardGameEnd)
		//{
		//	streamBuf->createBuf(18+rewardItems.size()*9);
		//	streamBuf->_writeUChar(m_playerRewardData->getType());//1
		//	streamBuf->_writeInt(m_playerRewardData->getCooperCashReward());
		//	streamBuf->_writeInt(m_playerRewardData->getExperience());
		//	streamBuf->_writeInt(m_playerRewardData->getExploit());
		//	streamBuf->_writeInt(m_playerRewardData->getAchievement());
		//}
		//else/* if(packet_type == GP_RewardItem || packet_type == GP_RewardDropItems)*////字节超出问题20140128
		//{
		//	streamBuf->createBuf(2+rewardItems.size()*9);
		//	streamBuf->_writeUChar(m_playerRewardData->getType());//1
		//}
		//streamBuf->_writeUChar(rewardItems.size());
		//while (itr_rewarditem!=rewardItems.end())
		//{
		//	streamBuf->_writeInt((*itr_rewarditem)->getItemID());
		//	streamBuf->_writeInt((*itr_rewarditem)->getEquipMagic());
		//	streamBuf->_writeUChar((*itr_rewarditem)->getItemCount());
		//	itr_rewarditem++;
		//}

		std::string key;
		m_this->doEvent(WCH_JXJGainGiftsPackKey,MAKECREPARAM(&key,NULL));
		ref_ptr<crStreamBuf> streamBuf;
		m_playerRewardData->buildGiftStream(streamBuf);
		ref_ptr<crCreateGiftsPack>giftsPack = new crCreateGiftsPack(playerid,key,streamBuf.get());
		m_playerRewardData->setKeyWord(key);
		void *param;
		crData *serverData = m_this->getDataClass();
		serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJCreateGiftsPackDeque,param);
		CreateGiftsPackDeque *giftsPacketDeque = (CreateGiftsPackDeque *)param;
		giftsPacketDeque->push_back(giftsPack.get());
		serverData->excHandle(MAKECREPARAM(WCH_LockData,0));

		//////////////////////////////////////////////////////////////////////////写邮件
		m_this->doEvent(WCH_JXJWriteRewardMail,MAKECREPARAM(m_playerRewardData.get(),NULL));
	}
}

/////////////////////////////////////////
//
//crJXJWriteRewardMailMethod
//
/////////////////////////////////////////
crJXJWriteRewardMailMethod::crJXJWriteRewardMailMethod(){}
crJXJWriteRewardMailMethod::crJXJWriteRewardMailMethod(const crJXJWriteRewardMailMethod& handle):
	crMethod(handle)
{
}
void crJXJWriteRewardMailMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_giftsPack = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_giftsPack = (crPlayerRewardData *)(LOCREPARAM(param64));
		}
		else
		{
			m_giftsPack = NULL;
		}
		break;
	}
}

void crJXJWriteRewardMailMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailCfg = str;
		break;
	}
}

void crJXJWriteRewardMailMethod::operator()(crHandle &handle)
{
	if(m_this!=NULL && m_giftsPack!=NULL)
	{
		void *param;
		std::string key  = m_giftsPack->getKeyWord();
		int experience = m_giftsPack->getExperience();
		int coppercash = m_giftsPack->getCooperCashReward();
		int exploit = m_giftsPack->getExploit();
		int achievement = m_giftsPack->getAchievement();
		int score = m_giftsPack->getScore();
		RewardItemVec& rewardItemsVec = m_giftsPack->getRewardItemsVec();

		if (experience == 0 && coppercash == 0 && exploit == 0 && \
			achievement == 0 && rewardItemsVec.empty() && score == 0)
			return;

		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(!itemtab.valid()) return;
		crTableIO::StrVec itemrecord;
		int nameid = itemtab->getTitleIndex("name");

		char buf[64];
		char textBuf[128];

		int paramindex = 1;
		rcfg::ConfigScript cfg_script;
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_mailCfg,true,""))
		{
			CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJWriteRewardMailMethod(): file open error "<<m_mailCfg<<std::endl;
			//sprintf(gDebugInfo->buf(),"crJXJWriteRewardMailMethod(): file open error %s\n\0",m_mailCfg.c_str());
			//gDebugInfo->debugInfo(CRCore::FATAL);
			return;
		}

		cfg_script.Push("Hypertext");
		cfg_script.Add("Content");
		cfg_script.Push("Content",paramindex++);
		std::vector<float>v_i;
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT");
		cfg_script.Add("Text","恭喜您");
		cfg_script.Pop();

		if(!m_giftsPack->getDescribtion().empty())
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			v_i.clear();
			v_i.push_back(32.0f);
			v_i.push_back(32.0f);
			v_i.push_back(32.0f);
			v_i.push_back(255.0f);
			cfg_script.Add("Color",v_i);
			cfg_script.Add("DrawMode","TEXT");
			cfg_script.Add("Text",m_giftsPack->getDescribtion().empty());
			cfg_script.Pop();
		}
		
		cfg_script.Push("Hypertext");
		cfg_script.Add("Content");
		cfg_script.Push("Content",paramindex++);
		v_i.clear();
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT");
		cfg_script.Add("Text","获得如下：");
		cfg_script.Pop();


		if(coppercash>0)
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			cfg_script.Add("DrawMode","TEXT");
			sprintf(buf,"%d铜钱，\0",coppercash);
			cfg_script.Add("Text",buf);
			cfg_script.Pop();
		}
		if(experience>0)
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			cfg_script.Add("DrawMode","TEXT");
			sprintf(buf,"%d经验，\0",experience);
			cfg_script.Add("Text",buf);
			cfg_script.Pop();
		}
		if(exploit>0)
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			cfg_script.Add("DrawMode","TEXT");
			sprintf(buf,"%d功勋，\0",exploit);
			cfg_script.Add("Text",buf);
			cfg_script.Pop();
		}
		if(achievement>0)
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			cfg_script.Add("DrawMode","TEXT");
			sprintf(buf,"%d战功，\0",achievement);
			cfg_script.Add("Text",buf);
			cfg_script.Pop();
		}
		if(score>0)
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",paramindex++);
			cfg_script.Add("DrawMode","TEXT");
			sprintf(buf,"%d国战积分，\0",score);
			cfg_script.Add("Text",buf);
			cfg_script.Pop();
		}
		if(!rewardItemsVec.empty())
		{
			int itemid;
			_crInt32 equipmagic;
			unsigned char itemcount;
			RewardItemVec::iterator itr_item = rewardItemsVec.begin();
			while (itr_item!=rewardItemsVec.end())
			{
				itemid = (*itr_item)->getItemID();
				equipmagic = (*itr_item)->getEquipMagic(); 
				itemcount = (*itr_item)->getItemCount();
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
				{
					cfg_script.Add("Content");
					cfg_script.Push("Content",paramindex++);
					cfg_script.Add("DrawMode","TEXT");
					sprintf(textBuf,"%d%s，\0",itemcount,itemrecord[nameid].c_str());
					cfg_script.Add("Text",textBuf);
					cfg_script.Pop();
				}
				itr_item++;
			}
		}
		cfg_script.Add("Content");
		cfg_script.Push("Content",paramindex++);
		v_i.clear();
		v_i.push_back(140.0f);
		v_i.push_back(40.0f);
		v_i.push_back(40.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT|UNDERLINE");
		cfg_script.Add("Text",std::string("点击此处即可领取"));
		cfg_script.Add("Link",1);
		cfg_script.Pop();
		cfg_script.Pop();

		//cfg_script.Add("Data");
		cfg_script.Push("Data");
		sprintf(buf,"%d,%s\0",WCHDATA_JXJGiftsPackKey,key.c_str());
		cfg_script.Add("Param",buf);
		for( RewardItemVec::iterator itr = rewardItemsVec.begin();
			itr != rewardItemsVec.end();
			++itr)
		{
			sprintf(buf,"%d,%d,%d,%d\0",WCHDATA_JXJGiftsPackItemInfo,(*itr)->getItemID(),(*itr)->getEquipMagic(),(*itr)->getItemCount());
			cfg_script.Add("Param",buf);
		}
		cfg_script.Pop();

		//cfg_script.Write("JXJ/script/giftstest.cfg");

		ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
		ref_ptr<crMailInfo> mailInfo = new crMailInfo;
		crData *server_data = m_this->getDataClass();
		server_data->excHandle(MAKECREPARAM(WCH_LockData,1));
		server_data->getParam(WCHDATA_JXJMailTaskList, param);
		MailTaskDeque *mail_deque = (MailTaskDeque *)param;
		crGlobalHandle::getInstance()->getScriptStream(cfg_script,mailBuffer);
		mailInfo->setRecvID(m_giftsPack->getPlayerID());
		mailInfo->setSenderName("系统");
		mailInfo->setSenderID(0);
		if(m_giftsPack->getType() == GP_RewardItem || m_giftsPack->getType() == GP_RewardGameEnd)
			mailInfo->setTitle("系统奖励邮件");
		else if(m_giftsPack->getType() == GP_RewardDropItems)
			mailInfo->setTitle("拾取物品相关邮件");
		mailInfo->setBody(mailBuffer.get());
		mailInfo->setState(Mail_HasEx);
		mail_deque->push_back(mailInfo.get());
		server_data->excHandle(MAKECREPARAM(WCH_LockData,0));

		cfg_script.Close();

		//ref_ptr<crStreamBuf> query_result = new crStreamBuf;
		//query_result->createBuf(1);
		//query_result->_writeBool(true);
		//crPlayerDataEventPacket packet;
		//packet.buildReplyPacket(packet,m_giftsPack->getPlayerID(),WCH_JXJNewMailRequest,query_result.get());
		//CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
		//ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(m_giftsPack->getPlayerID()));
		//if(player_net_data.valid())
		//{
		//	server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
		//}

	}
}
/////////////////////////////////////////
//
//crJXJRecvExperienceMethod
//
/////////////////////////////////////////
crJXJRecvExperienceMethod::crJXJRecvExperienceMethod():
	m_this(NULL),
	m_experience(0),
	m_distillLv(0)
{
}

crJXJRecvExperienceMethod::crJXJRecvExperienceMethod(const crJXJRecvExperienceMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_experience(0),
	m_distillLv(handle.m_distillLv)
{
}

void crJXJRecvExperienceMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_experience = 0;
		}
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_experience = LOCREPARAM(param64);
		}
		else
		{
			m_experience = 0;
		}
		break;
	}
}

void crJXJRecvExperienceMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_distillLv = atoi(str.c_str());
		break;
	}
}

void crJXJRecvExperienceMethod::operator()(crHandle &handle)
{
	if(m_this && m_experience!=0)
	{
		crData *maindata = m_this->getDataClass();
		if(maindata)
		{
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				void *param;
				crTableIO::StrVec record;
				ref_ptr<crTableIO> lordlvtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordlvTab);
				unsigned char lv = 0;
				int expneed = 0;
				int getvipexp = 0;
				int lvindex = lordlvtab->getTitleIndex("lv");
				int expindex = lordlvtab->getTitleIndex("经验");
				int bingfaindex = lordlvtab->getTitleIndex("兵法");
				int zhenfaindex = lordlvtab->getTitleIndex("阵法");
				int dongchaindex = lordlvtab->getTitleIndex("洞察");
				int tongshuaiindex = lordlvtab->getTitleIndex("统率");
				bool lvchange = false;
				maindata->excHandle(MAKECREPARAM(WCH_LockData,1));
				maindata->getParam(WCHDATA_Experience,param);
				int *experience = (int *)param;
				INTLIMITADD(*experience,m_experience,INT_MAX);
				maindata->getParam(WCHDATA_Level,param);
				unsigned char *mylv = (unsigned char *)param;
				unsigned char lastlv = *mylv;
				//兵法
				maindata->getParam(WCHDATA_JXJBingFa,param);
				short *bingfa = (short *)param;
				//阵法
				maindata->getParam(WCHDATA_JXJZhenFa,param);
				short *zhenfa = (short *)param;
				//洞察
				maindata->getParam(WCHDATA_JXJDongCha,param);
				short *dongcha = (short *)param;
				//统率
				maindata->getParam(WCHDATA_JXJTongShuai,param);
				short *tongshuai = (short *)param;
				/////
				int maxlv = lordlvtab->getRowCount()+1;
				if(*mylv<maxlv)
				{
					lv = *mylv+1;
					for (;lv<maxlv;lv++)
					{
						if(lordlvtab->queryOneRecord(lvindex,crArgumentParser::appItoa(lv),record)>=0)
						{
							expneed = atoi(record[expindex].c_str());
							if (expneed > *experience)
							{
								break;
							}
						}
					}
					lv-=1;
					if (lv > *mylv)
					{
						crData *data = playerData->getPlayerGameData()->getDataClass();
						data -> getParam(WCHDATA_JXJVipLv,param);
						unsigned char viplv = *(unsigned char *)param;
						//int lvToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLevelToVipExpFactor,viplv).c_str());
						//getvipexp = (lv - *mylv) * lvToVipExpFactor;
						//if (getvipexp < 0)
						//{
						//	getvipexp = 0;
						//}
						*mylv = lv;
						if(lordlvtab->queryOneRecord(lvindex,crArgumentParser::appItoa(lv),record)>=0)
						{
							if(bingfaindex>=0)
							{
								*bingfa = atoi(record[bingfaindex].c_str());
							}
							if(zhenfaindex>=0)
							{
								*zhenfa = atoi(record[zhenfaindex].c_str());
							}
							if(dongchaindex>=0)
							{
								*dongcha = atoi(record[dongchaindex].c_str());
							}
							if(tongshuaiindex>=0)
							{
								*tongshuai = atoi(record[tongshuaiindex].c_str());
							}
						}
						lvchange = true;
					}
				}
				maindata->excHandle(MAKECREPARAM(WCH_LockData,0));
				if(lvchange)
				{
					//if(lastlv<m_distillLv && m_distillLv<=*mylv)
					//{//等级升级到10级的时候获取轮回道具
					//	playerData->getPlayerGameData()->doEvent(WCH_JXJGetPlayerGameStore);
					//}
					//m_this->doEvent(WCH_JXJServerResetDailyTask);
					m_this->doEvent(WCH_JXJServerActiveTask);
					playerData->getPlayerGameData()->doEvent(WCH_JXJCheckResBuildingLevel);
					playerData->getPlayerGameData()->doEvent(WCH_JXJCheckPresentSoldier,MAKECREPARAM((int)lastlv,(int)(*mylv)));
					//if(getvipexp>0)
					//	playerData->getPlayerGameData()->doEvent(WCH_JXJAddVipExp,MAKECREPARAM(getvipexp,NULL));
                    playerData->getPlayerGameData()->doEvent(WCH_JXJCheck7Day);
					

					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(13);
					stream->_writeInt(*experience);  //4
					stream->_writeUChar(lv);         //1
					stream->_writeShort(*bingfa);		
					stream->_writeShort(*zhenfa);
					stream->_writeShort(*dongcha);
					stream->_writeShort(*tongshuai);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvUpdateLordLvInfo,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				else
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					stream->createBuf(4);
					stream->_writeInt(*experience);  //4
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvExperience,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				crNetConductor *dbConductor = crNetContainer::getInstance()->getNetConductor(GameServerClient_DB);
				if(dbConductor)
				{
					crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
					crNetManager *netManager = gameServer->getNetManager();
					crNetDataManager *netDataManager = gameServer->getNetDataManager();
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netDataManager->getNetCallback());
					int serverid = callback->getServerID();

					ref_ptr<crJXJUpdateCharacterLevel> updatecharacterlv = new crJXJUpdateCharacterLevel;
					updatecharacterlv->buildUpdateSql(playerid, serverid, m_this->getRoleID(),*mylv);
					crDBSavePacket dbpacket;
					crDBSavePacket::buildRequestPacket(dbpacket,updatecharacterlv.get());
					dbConductor->getNetManager()->sendPacket("all",dbpacket);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvExperienceMethod
//
/////////////////////////////////////////
crJXJClientRecvExperienceMethod::crJXJClientRecvExperienceMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvExperienceMethod::crJXJClientRecvExperienceMethod(const crJXJClientRecvExperienceMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvExperienceMethod::inputParam(int i, void *param)
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

void crJXJClientRecvExperienceMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvExperienceMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameClient_Game)
		{
			int exp =m_stream->_readInt(); 
			int extraexp = 0;
			//m_this->doEvent(WCH_JXJCheckResBuildingLevel);
			if(m_this->getMainRole() && m_this->getMainRole()->getDataClass())
			{
				crData *data = m_this->getMainRole()->getDataClass();
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_Experience,param);
				int *experience = (int *)param;
				extraexp = exp-(*experience);
				*experience = exp;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				if(extraexp!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Exp,extraexp));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvExploitMethod
//
/////////////////////////////////////////
crJXJRecvExploitMethod::crJXJRecvExploitMethod():
	m_this(NULL),
	m_exploit(0)
{
}

crJXJRecvExploitMethod::crJXJRecvExploitMethod(const crJXJRecvExploitMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_exploit(0)
{
}

void crJXJRecvExploitMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_exploit = NULL;
		}
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_exploit = LOCREPARAM(param64);
		}
		else
		{
			m_exploit = NULL;
		}
		break;
	}
}

void crJXJRecvExploitMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvExploitMethod::operator()(crHandle &handle)
{
	if(m_this && m_exploit!=0)
	{	
		crData *maindata = m_this->getDataClass();
		if(maindata)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				//crPlayerGameData *playerGameData = playerData->getPlayerGameData();
				//crData *gameData = playerGameData->getDataClass();
				//gameData->getParam(WCHDATA_JXJShiliID,param);
				//unsigned char shiliid = *(unsigned char *)param;
				//int startShili = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJStartShiliID).c_str());
				//int rankingid = shiliid - c_startShiliID;

				maindata->excHandle(MAKECREPARAM(WCH_LockData,1));
				maindata->getParam(WCHDATA_JXJExploit,param);
				int *exploit = (int *)param;
				INTLIMITADD(*exploit,m_exploit,INT_MAX);
				//maindata->getParam(WCHDATA_JXJGovPost,param);
				//unsigned char govpost = *(unsigned char*) param;
				//maindata->getParam(WCHDATA_Level,param);
				//unsigned char level = *(unsigned char*) param;
				//maindata->getParam(WCHDATA_JXJNobility,param);
				//unsigned char nobility = *(unsigned char*) param;

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*exploit);

				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvExploit,stream.get());
				if (playerData.valid())
				{
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				maindata->excHandle(MAKECREPARAM(WCH_LockData,0));

				//int minexploit = 0;
				//int maxRankingNum = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxExpRanking).c_str());
				//crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				//serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
				////serverData->getParam(WCHDATA_JXJExpRankingMap1+rankingid,param);
				//serverData->getParam(WCHDATA_JXJExpRankingMap,param);
				//ExpRankingMap *expRankingMap = (ExpRankingMap *)param;
				//if(expRankingMap->size()>=maxRankingNum)
				//{
				//	minexploit = expRankingMap->rbegin()->first;
				//}
				//if(*exploit>minexploit)
				//{
				//	ref_ptr<crPlayerRanking> ranking;
				//	serverData->getParam(WCHDATA_JXJExpRankingWaitMap,param);
				//	ExpRankingMap *expRankingWaitMap = (ExpRankingMap *)param;
				//	//寻找playerid是否存在
				//	bool found = false;
				//	for( ExpRankingMap::iterator fitr = expRankingWaitMap->begin();
				//		 fitr != expRankingWaitMap->end();
				//		 ++fitr )
				//	{
				//		ranking = fitr->second.get();
				//		if(playerid == ranking->getPlayerID())
				//		{
				//			ranking->setExploit(*exploit);
				//			ranking->setGovpost(govpost);
				//			ranking->setLevel(level);
				//			ranking->setNobility(nobility);
				//			found = true;
				//			break;
				//		}
				//	}
				//	if(!found)
				//	{
				//		if(expRankingWaitMap->size()>=maxRankingNum)
				//		{
				//			minexploit = expRankingMap->rbegin()->first;
				//			ranking = expRankingMap->rbegin()->second.get();
				//			if(*exploit>minexploit)
				//			{
				//				ranking->setExploit(*exploit);
				//				ranking->setGovpost(govpost);
				//				ranking->setPlayerID(playerid);
				//				ranking->setName(playerData->getCharacterName());
				//				ranking->setShiliID(shiliid);
				//				ranking->setLevel(level);
				//				ranking->setNobility(nobility);
				//			}
				//		}
				//		else
				//		{
				//			ranking = new crPlayerRanking;
				//			ranking->setExploit(*exploit);
				//			ranking->setGovpost(govpost);
				//			ranking->setPlayerID(playerid);
				//			ranking->setName(playerData->getCharacterName());
				//			ranking->setShiliID(shiliid);
				//			ranking->setLevel(level);
				//			ranking->setNobility(nobility);
				//			expRankingWaitMap->insert(std::make_pair(*exploit,ranking.get()));
				//		}
				//	}
				//}
				//serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvExploitMethod
//
/////////////////////////////////////////
crJXJClientRecvExploitMethod::crJXJClientRecvExploitMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvExploitMethod::crJXJClientRecvExploitMethod(const crJXJClientRecvExploitMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvExploitMethod::inputParam(int i, void *param)
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

void crJXJClientRecvExploitMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvExploitMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if(m_netType == GameClient_Game && m_this->getMainRole())
		{
			int temp_exploit =m_stream->_readInt();
			int extra = 0;
			crData *data = m_this->getMainRole()->getDataClass();
			if(data)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJExploit,param);
				int *exploit = (int *)param;
				extra = temp_exploit-(*exploit);
				*exploit= temp_exploit;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			if(extra!=0)
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Exploit,extra));
		}
	}
}

/////////////////////////////////////////
//
//crJXJUIUpdateChatMessageListMethod
//
/////////////////////////////////////////
crJXJRecvAchievementMethod::crJXJRecvAchievementMethod():
	m_this(NULL),
	m_achievement(0)
{
}

crJXJRecvAchievementMethod::crJXJRecvAchievementMethod(const crJXJRecvAchievementMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_achievement(0)
{
}

void crJXJRecvAchievementMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_achievement = NULL;
		}
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_achievement = LOCREPARAM(param64);
		}
		else
		{
			m_achievement = NULL;
		}
		break;
	}
}

void crJXJRecvAchievementMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvAchievementMethod::operator()(crHandle &handle)
{
	if(m_this && m_achievement!=0)
	{	
		crData *maindata = m_this->getDataClass();
		if(maindata)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				maindata->excHandle(MAKECREPARAM(WCH_LockData,1));
				maindata->getParam(WCHDATA_JXJAchievement,param);
				int *achievement = (int *)param;
				INTLIMITADD(*achievement,m_achievement,INT_MAX);
				maindata->excHandle(MAKECREPARAM(WCH_LockData,0));

				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*achievement);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvAchievement,stream.get());
				if (playerData.valid())
				{
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvFoodMethod
//
/////////////////////////////////////////
crJXJRecvFoodMethod::crJXJRecvFoodMethod():
	m_this(NULL),
	m_food(0)
{
}

crJXJRecvFoodMethod::crJXJRecvFoodMethod(const crJXJRecvFoodMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_food(0)
{
}

void crJXJRecvFoodMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_food = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_food = LOCREPARAM(param64);
		}
		else
		{
			m_food = NULL;
		}
		break;
	}
}

void crJXJRecvFoodMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvFoodMethod::operator()(crHandle &handle)
{
	if(m_this && m_food!=0)
	{	
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJFood,param);
				int *food = (int *)param;
				INTLIMITADD(*food,m_food,INT_MAX);
				data->excHandle(MAKECREPARAM(WCH_LockData,0));

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*food);  //4
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvFood,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}	
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvWoodMethod
//
/////////////////////////////////////////
crJXJRecvWoodMethod::crJXJRecvWoodMethod():
	m_this(NULL),
	m_wood(0)
{
}

crJXJRecvWoodMethod::crJXJRecvWoodMethod(const crJXJRecvWoodMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_wood(0)
{
}

void crJXJRecvWoodMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_wood = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_wood = LOCREPARAM(param64);
		}
		else
		{
			m_wood = NULL;
		}
		break;
	}
}

void crJXJRecvWoodMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvWoodMethod::operator()(crHandle &handle)
{
	if(m_this && m_wood!=0)
	{	
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJWood,param);
				int *wood = (int *)param;
				INTLIMITADD(*wood,m_wood,INT_MAX);
				data->excHandle(MAKECREPARAM(WCH_LockData,0));

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*wood);  //4
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvWood,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvIronMethod
//
/////////////////////////////////////////
crJXJRecvIronMethod::crJXJRecvIronMethod():
	m_this(NULL),
	m_iron(0)
{
}

crJXJRecvIronMethod::crJXJRecvIronMethod(const crJXJRecvIronMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_iron(0)
{
}

void crJXJRecvIronMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_iron = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_iron = LOCREPARAM(param64);
		}
		else
		{
			m_iron = NULL;
		}
		break;
	}
}

void crJXJRecvIronMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvIronMethod::operator()(crHandle &handle)
{
	if(m_this && m_iron!=0)
	{	
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJIron,param);
				int *iron = (int *)param;
				INTLIMITADD(*iron,m_iron,INT_MAX);
				data->excHandle(MAKECREPARAM(WCH_LockData,0));

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*iron);  //4
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvIron,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvHorseMethod
//
/////////////////////////////////////////
crJXJRecvHorseMethod::crJXJRecvHorseMethod():
	m_this(NULL),
	m_horse(0)
{
}

crJXJRecvHorseMethod::crJXJRecvHorseMethod(const crJXJRecvHorseMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_horse(0)
{
}

void crJXJRecvHorseMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_horse = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_horse = LOCREPARAM(param64);
		}
		else
		{
			m_horse = NULL;
		}
		break;
	}
}

void crJXJRecvHorseMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvHorseMethod::operator()(crHandle &handle)
{
	if(m_this && m_horse!=0)
	{	
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJHorse,param);
				int *horse = (int *)param;
				INTLIMITADD(*horse,m_horse,INT_MAX);
				data->excHandle(MAKECREPARAM(WCH_LockData,0));

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(4);
				stream->_writeInt(*horse);  //4
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJClientRecvHorse,stream.get());
				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
			}	
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvAchievementMethod
//
/////////////////////////////////////////
crJXJClientRecvAchievementMethod::crJXJClientRecvAchievementMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvAchievementMethod::crJXJClientRecvAchievementMethod(const crJXJClientRecvAchievementMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvAchievementMethod::inputParam(int i, void *param)
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

void crJXJClientRecvAchievementMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvAchievementMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if(m_netType == GameClient_Game && m_this->getMainRole())
		{
			int temp_achievement =m_stream->_readInt(); 
			int extra = 0;
			crData *data = m_this->getMainRole()->getDataClass();
			if(data)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJAchievement,param);
				int *achievement = (int *)param;
				extra = temp_achievement-(*achievement);
				*achievement= temp_achievement;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Achievement,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvRewardItemsMethod
//
/////////////////////////////////////////
crJXJRecvRewardItemsMethod::crJXJRecvRewardItemsMethod():
	m_this(NULL),
	m_rewardItems(NULL),
	m_type(0),
	m_cycle(0)
{
}

crJXJRecvRewardItemsMethod::crJXJRecvRewardItemsMethod(const crJXJRecvRewardItemsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_rewardItems(NULL),
	m_type(0),
	m_cycle(0)
{
}

void crJXJRecvRewardItemsMethod::inputParam(int i, void *param)
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
			m_rewardItems = (RewardItemVec *)(LOCREPARAM(param64));
			_crInt32 hi = HICREPARAM(param64);
			m_type = LOINT32(hi);
			m_cycle = HIINT32(hi);
		}
		else
		{
			m_rewardItems = NULL;
			m_type = 0;
			m_cycle = 0;
		}
		break;
	}
}

void crJXJRecvRewardItemsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvRewardItemsMethod::operator()(crHandle &handle)
{
	if(m_rewardItems && !m_rewardItems->empty())
	{
		RewardItemVec rewardItems = *m_rewardItems;
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));

			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJItemBagVec,param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			data->getParam(WCHDATA_JXJJiangkaBagVec,param);
			JXJItemBagVec *cardbagvec = (JXJItemBagVec *)param;
			JXJItemBagVec::iterator itr_item = itembagvec->begin();
			JXJItemBagVec::iterator itr_card = cardbagvec->begin();
			int packetRemainSize = 0;//背包剩余空间
			int cardpacketRemainSize = 0;//将卡背包剩余空间
			while (itr_item!=itembagvec->end())
			{
				if(!itr_item->get())
				{
					packetRemainSize++;
				}
				itr_item++;
			}
			while (itr_card!=cardbagvec->end())
			{
				if(!itr_card->get())
				{
					cardpacketRemainSize++;
				}
				itr_card++;
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(!itemtab.valid()) return;
			int maxnumid= itemtab->getTitleIndex("叠加数量");
			int itemtypeindex= itemtab->getTitleIndex("类型");
			crTableIO::StrVec itemrecord;
			JXJItemBagVec newitembagvec;
			JXJItemBagVec newcardbagvec;
			int maxnum = 0;
			CRCore::ref_ptr<crBagItemData> newitem;
			int itemid;
			_crInt32 equipmagic;
			unsigned char itemcount;
			for( RewardItemVec::iterator itr_rewarditem = rewardItems.begin();
				itr_rewarditem!=rewardItems.end();
				++itr_rewarditem )
			{
				itemid = (*itr_rewarditem)->getItemID();
				equipmagic = (*itr_rewarditem)->getEquipMagic();
				itemcount = (*itr_rewarditem)->getItemCount();
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),itemrecord)>=0)
				{
					maxnum = atoi(itemrecord[maxnumid].c_str());
					if(maxnum == 0) maxnum = 1;
					int index = itemcount/maxnum; 
					unsigned char remaindercount = itemcount%maxnum;
					while (index>0)
					{
						newitem = new crBagItemData;
						newitem->setItemID(itemid);
						newitem->setEquipMagic(equipmagic);
						newitem->setItemCount(maxnum);
						if (atoi(itemrecord[itemtypeindex].c_str()) == IT_Jiangka)
						{
							newcardbagvec.push_back(newitem);
						}
						else
						{
							newitembagvec.push_back(newitem);
						}
						index--;
					}
					if(remaindercount>0)
					{
						newitem = new crBagItemData;
						newitem->setItemID(itemid);
						newitem->setEquipMagic(equipmagic);
						newitem->setItemCount(remaindercount);
						if (atoi(itemrecord[itemtypeindex].c_str()) == IT_Jiangka)
						{
							newcardbagvec.push_back(newitem);
						}
						else
						{
							newitembagvec.push_back(newitem);
						}
					}
				}
			}
			//背包充足  直接放入
			if(!newitembagvec.empty())
			{
				if(packetRemainSize >= newitembagvec.size())
				{
					//放入背包
					JXJItemBagVec::iterator itr_newitem = newitembagvec.begin();

					int itemremaincount = 0;
					int emptybagindex = 0;
					int curindex = 0;
					while (itr_newitem!=newitembagvec.end())
					{
						m_this->doEvent(WCH_JXJPutItemInBag,MAKECREPARAM(itr_newitem->get(),MAKEINT32(0,m_cycle)));
						itr_newitem++;
					}

					//发送到客户端
					if(playerData.valid())
					{
						//char size = newitembagvec.size() + newcardbagvec.size();
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2 + newitembagvec.size()*9);
						stream->_writeUChar(0);						//发送物品
						stream->_writeUChar(newitembagvec.size());
						itr_newitem = newitembagvec.begin();
						while (itr_newitem!=newitembagvec.end())
						{
							stream->_writeInt((*itr_newitem)->getItemID());
							stream->_writeInt((*itr_newitem)->getEquipMagic());
							stream->_writeUChar((*itr_newitem)->getItemCount());
							itr_newitem++;
						}
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJClientRecvRewardItem,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
				else
				{
					//发送邮件
					ref_ptr<crPlayerRewardData> temp_RewardInfo = new crPlayerRewardData;
					temp_RewardInfo->getRewardItemsVec() = newitembagvec;
					if(m_type == GP_RewardGameEnd)
						temp_RewardInfo->setType(GP_RewardItem);
					else if(m_type == GP_RewardDropItems)
						temp_RewardInfo->setType(GP_RewardDropItems);
					else
						temp_RewardInfo->setType(GP_RewardItem);
					temp_RewardInfo->setPlayerID(m_this->getPlayerID());
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJCreateRewardMail,MAKECREPARAM(temp_RewardInfo.get(),NULL));

					std::string strMessage = std::string("物品背包空间不足,已经发放到邮件中,请及时领取.");
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[系统]",strMessage,0,0);
					chatInfo->setRecvName(m_this->getCharacterName());
					crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
					serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
					serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}

			//&& 将卡背包充足
			if(!newcardbagvec.empty())
			{
				if(cardpacketRemainSize >= newcardbagvec.size())
				{
					//放入将卡背包
					JXJItemBagVec::iterator itr_newcard = newcardbagvec.begin();

					while (itr_newcard!=newcardbagvec.end())
					{
						m_this->doEvent(WCH_JXJPutItemInBag,MAKECREPARAM(itr_newcard->get(),MAKEINT32(0,m_cycle)));
						itr_newcard++;
					}

					//发送到客户端
					if(playerData.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(2 + newcardbagvec.size()*5);
						stream->_writeUChar(1);
						stream->_writeUChar(newcardbagvec.size());

						itr_newcard = newcardbagvec.begin();
						while (itr_newcard!=newcardbagvec.end())
						{
							stream->_writeInt((*itr_newcard)->getItemID());
							stream->_writeUChar((*itr_newcard)->getItemCount());
							itr_newcard++;
						}

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJClientRecvRewardItem,stream.get());
						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
				else
				{
					//发送邮件
					ref_ptr<crPlayerRewardData> temp_RewardInfo = new crPlayerRewardData;
					temp_RewardInfo->getRewardItemsVec() = newcardbagvec;
					if(m_type == GP_RewardGameEnd)
						temp_RewardInfo->setType(GP_RewardItem);
					else if(m_type == GP_RewardDropItems)
						temp_RewardInfo->setType(GP_RewardDropItems);
					else
						temp_RewardInfo->setType(GP_RewardItem);
					temp_RewardInfo->setPlayerID(m_this->getPlayerID());
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJCreateRewardMail,MAKECREPARAM(temp_RewardInfo.get(),NULL));

					std::string strMessage = std::string("将卡背包空间不足,已经发放到邮件中,请及时领取.");
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[系统]",strMessage,0,0);
					chatInfo->setRecvName(m_this->getCharacterName());
					crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
					serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
					serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvRewardItemsMethod
//
/////////////////////////////////////////
crJXJClientRecvRewardItemsMethod::crJXJClientRecvRewardItemsMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvRewardItemsMethod::crJXJClientRecvRewardItemsMethod(const crJXJClientRecvRewardItemsMethod& handle):
	crMethod(handle),
	m_jiangkabag(handle.m_jiangkabag)
{
}
void crJXJClientRecvRewardItemsMethod::inputParam(int i, void *param)
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

void crJXJClientRecvRewardItemsMethod::addParam(int i, const std::string& str)
{
	switch	(i)
	{
	case 0:
		m_jiangkabag = str;
		break;
	}
}

void crJXJClientRecvRewardItemsMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			JXJItemBagVec newitembagvec;
			JXJItemBagVec newcardbagvec;
			unsigned char _case = m_stream->_readUChar();
			if(_case == 0)
			{
				//物品
				unsigned char itemsize = m_stream->_readUChar();
				//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				//int maxnumid= itemtab->getTitleIndex("叠加数量");
				crTableIO::StrVec itemrecord;
				CRCore::ref_ptr<crBagItemData> newitem;
				int item_id;
				int equipmagic;
				unsigned char item_count;
				for(int i = 0;i<itemsize;i++)
				{
					newitem = new crBagItemData;
					item_id = m_stream->_readInt();
					equipmagic = m_stream->_readInt();
					item_count = m_stream->_readUChar();
					newitem->setItemID(item_id);
					newitem->setEquipMagic(equipmagic);
					newitem->setItemCount(item_count);
					newitembagvec.push_back(newitem);
				}
			}
			else if(_case == 1)
			{
				//将卡
				ref_ptr<crCanvasNode>jiangkabag = crFilterRenderManager::getInstance()->findCanvas(m_jiangkabag);
				unsigned char cardsize = m_stream->_readUChar();
				CRCore::ref_ptr<crBagItemData> newcard;
				int card_id;
				unsigned char card_count;
				for(int i = 0;i<cardsize;i++)
				{
					newcard = new crBagItemData;
					card_id = m_stream->_readInt();
					//magicid = m_stream->_readUChar();
					card_count = m_stream->_readUChar();
					newcard->setItemID(card_id);
					//newcard->setEquipMagic(magicid);
					newcard->setItemCount(card_count);
					newcardbagvec.push_back(newcard);
				}

				if (jiangkabag.valid() && jiangkabag->getVisiable())
				{
					crFilterRenderManager::getInstance()->showCanvas(jiangkabag.get(),true);
				}
			}


			//int temp_achievement =m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				//JXJItemBagVec::iterator itr_newitem = newitembagvec.begin();
				//while (itr_newitem!=newitembagvec.end())
				//{
				//	m_this->doEvent(WCH_JXJPutItemInBag,MAKECREPARAM(itr_newitem->get(),NULL));
				//	itr_newitem++;
				//}
				//JXJItemBagVec::iterator itr_newcard = newcardbagvec.begin();
				//while (itr_newcard!=newcardbagvec.end())
				//{
				//	m_this->doEvent(WCH_JXJPutItemInBag,MAKECREPARAM(itr_newcard->get(),NULL));
				//	itr_newcard++;
				//}
				crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeItems,MAKECREPARAM(&newitembagvec,&newcardbagvec));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJRecvRewardEquipsMethod
//
/////////////////////////////////////////
crJXJRecvRewardEquipsMethod::crJXJRecvRewardEquipsMethod():
	m_this(NULL),
	m_rewardEquips(NULL)
{
}

crJXJRecvRewardEquipsMethod::crJXJRecvRewardEquipsMethod(const crJXJRecvRewardEquipsMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_rewardEquips(NULL)
{
}

void crJXJRecvRewardEquipsMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_rewardEquips = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_rewardEquips = (RewardEquipsVec *)(LOCREPARAM(param64));
		}
		else
		{
			m_rewardEquips = NULL;
		}
		break;
	}
}

void crJXJRecvRewardEquipsMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvRewardEquipsMethod::operator()(crHandle &handle)
{
	if(m_this && m_rewardEquips)
	{	
		crData *data = m_this->getDataClass();
		if(data)
		{
			void *param;
			//int playerid = m_this->getPlayerID();
			//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJTroopsEquipMap,param);
			TroopsEquipMap *equipmap = (TroopsEquipMap *)param;
			TroopsEquipMap::iterator itr_equips;
			for(RewardEquipsVec::iterator itr = m_rewardEquips->begin();itr!=m_rewardEquips->end();itr++)
			{
				if((itr_equips = equipmap->find(itr->first)) != equipmap->end())
					itr_equips->second+=itr->second;
				else
				{
					(*equipmap)[itr->first] = itr->second;
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJCityBelongCheckMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCityBelongCheckMethod::crJXJCityBelongCheckMethod():
	m_battleid(0){}
crJXJCityBelongCheckMethod::crJXJCityBelongCheckMethod(const crJXJCityBelongCheckMethod& handle):
	crMethod(handle),
	m_battleid(0)
{
}
void crJXJCityBelongCheckMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_battleid = LOCREPARAM(param64);
		}
		else
		{
			m_battleid = 0;
		}
		break;
	}
}

void crJXJCityBelongCheckMethod::addParam(int i, const std::string& str)
{
}

void crJXJCityBelongCheckMethod::operator()(crHandle &handle)
{///如果归属发生变化，重置城防值
	unsigned char ashili = (unsigned char)(LOINT16(LOINT32(m_battleid)));
	unsigned char dshili = (unsigned char)(HIINT16(LOINT32(m_battleid)));
	unsigned short chengchiid = HIINT32(m_battleid);

	void *param;
	crData *data = m_this->getDataClass();
	ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	unsigned char maxcount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCityChangeDeqMaxCount, 0).c_str());
	//int chengfangindex = chengchiTab->getTitleIndex("城防值");
	crTableIO::StrVec record;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJChengChiMap,param);
	ChengchiMap *chengchiMap = (ChengchiMap *)param;
	data->getParam(WCHDATA_JXJBattleScoreMap,param);
	BattleScoreMap *battleScoreMap = (BattleScoreMap *)param;
	//std::multimap< crVector2i,unsigned char, std::greater<crVector2i> >ScoreMuMap;
	//std::multimap< crVector2i,unsigned char, std::greater<crVector2i> >::iterator smitr;
	//ShiliScoreMap::iterator sitr;
	//crVector2i greaterScore;
	unsigned char winshili;
	char cityLose = 0;
	//rangei rnd;
	//int r;
	ChengchiMap::iterator itr = chengchiMap->find(chengchiid);
	//for( ChengchiMap::iterator itr = chengchiMap->begin();
	//	 itr != chengchiMap->end();
	//	 ++itr )
	if(itr != chengchiMap->end()/* && itr->second.second == 0*/)
	{//城池被攻下
		//winshili = itr->second.first;
		//ShiliScoreMap &shiliScoreMap = (*battleScoreMap)[m_chengchiid];
		//if(!shiliScoreMap.empty())
		//{
		//	if(shiliScoreMap.size() == 1)
		//	{
		//		winshili = shiliScoreMap.begin()->first;
		//	}
		//	else
		//	{
		//		for( sitr = shiliScoreMap.begin();
		//			sitr != shiliScoreMap.end();
		//			++sitr )
		//		{
		//			ScoreMuMap.insert(std::make_pair(crVector2i(sitr->second.first,sitr->second.second),sitr->first));
		//		}
		//		smitr = ScoreMuMap.begin();
		//		greaterScore = smitr->first;
		//		for( ++smitr;smitr != ScoreMuMap.end(); )
		//		{
		//			if(smitr->first < greaterScore)
		//				smitr = ScoreMuMap.erase(smitr);
		//			else
		//				++smitr;
		//		}
		//		if(ScoreMuMap.size() == 1)
		//		{
		//			winshili = ScoreMuMap.begin()->second;
		//		}
		//		else
		//		{
		//			rnd.set(0,ScoreMuMap.size());
		//			r = rnd.get_random();
		//			smitr = ScoreMuMap.begin();
		//			while (r>0)
		//			{
		//				r--;
		//				smitr++;
		//			}
		//			winshili = smitr->second;
		//		}
		//	}
		//}
		winshili = itr->second->getChengzhuShili();
		//国家信息
		bool isModify = true;
		unsigned char lostshili = dshili;
		crJXJShili *shili;
		crData *shiliData;
		ShiliCityChangeDeq *cityDeq;
		if (lostshili>=c_startShiliID)
		{
			data->getParam(WCHDATA_JXJShiliWei+lostshili-c_startShiliID,param);
			shili = (crJXJShili *)param;
			if(shili && shili->getDataClass())
			{
				shiliData = shili->getDataClass();
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJShiliLostCityDeq,param);
				cityDeq = (ShiliCityChangeDeq *)param;
				cityDeq->push_back(chengchiid);
				if (cityDeq->size()>maxcount)
				{
					cityDeq->pop_front();
				}
				shiliData->inputParam(WCHDATA_JXJShiliModify,&isModify);
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		if (winshili>=c_startShiliID)
		{
			data->getParam(WCHDATA_JXJShiliWei+winshili-c_startShiliID,param);
			shili = (crJXJShili *)param;
			if(shili && shili->getDataClass())
			{
				shiliData = shili->getDataClass();
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJShiliWinCityDeq,param);
				cityDeq = (ShiliCityChangeDeq *)param;
				cityDeq->push_back(chengchiid);
				if (cityDeq->size()>maxcount)
				{
					cityDeq->pop_front();
				}
				shiliData->inputParam(WCHDATA_JXJShiliModify,&isModify);

				shiliData->getParam(WCHDATA_JXJZhanLingCityCount,param);
				unsigned char *count = (unsigned char*)param;
				*count += 1;

				shiliData->getParam(WCHDATA_JXJTodayConqueredTity,param);
				unsigned char *conqueredcity = (unsigned char*)param;
				*conqueredcity += 1;

				shiliData->getParam(WCHDATA_JXJChengxiang,param);
				int chengxiangid = *(int*)param;
				shiliData->getParam(WCHDATA_JXJDasima,param);
				int dasimaid = *(int*)param;
				shiliData->getParam(WCHDATA_JXJDajiangjun,param);
				int dajiangjunid = *(int*)param;

				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(1);
				stream->_writeUChar(*count);
				
				ref_ptr<crGameServerPlayerData> chengxiangData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(chengxiangid));
				ref_ptr<crGameServerPlayerData> dasimaData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(dasimaid));
				ref_ptr<crGameServerPlayerData> dajiangjunData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(dajiangjunid));
				if (chengxiangData.valid())
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,chengxiangid,WCH_JXJClientRecvZhanLingCityCount,stream.get());
					gameServer->getNetManager()->sendPacket(chengxiangData->getPlayerConnectServerAddress(),packet);
				}
				if (dasimaData.valid())
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,dasimaid,WCH_JXJClientRecvZhanLingCityCount,stream.get());
					gameServer->getNetManager()->sendPacket(dasimaData->getPlayerConnectServerAddress(),packet);
				}
				if (dajiangjunData.valid())
				{
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildReplyPacket(packet,dajiangjunid,WCH_JXJClientRecvZhanLingCityCount,stream.get());
					gameServer->getNetManager()->sendPacket(dajiangjunData->getPlayerConnectServerAddress(),packet);
				}
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		////重置城防值
		//unsigned char oldShili = itr->second.first;//原属势力
		//itr->second.first = winshili;
		//if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(m_chengchiid),record)>=0)
		//{
		//	itr->second.second = (unsigned short)(atoi(record[chengfangindex].c_str()));
		//}
		//
		//battleScoreMap->erase(chengchiid);
		////通知所有玩家
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//stream->createBuf(5);
		//stream->_writeUShort(itr->first);
		//stream->_writeUChar(itr->second.first);
		//stream->_writeUShort(itr->second.second);
		//crPlayerDataEventPacket packet;
		//crPlayerDataEventPacket::buildReplyPacket(packet,0,WCH_JXJRecvChengchiChange,stream.get());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		//crNetDataManager *netDataManager = netConductor->getNetDataManager();
		//netDataManager->sendPacketToAllPlayer(packet);
		//刷新国家所属州
		m_this->doEvent(WCH_JXJCountryCantonMapRefresh,MAKECREPARAM(itr->first,itr->second->getChengzhuShili()));

		//通告
		data->getParam(WCHDATA_JXJShiliWei,param);
		crJXJShili *shiliWei = (crJXJShili *)param;
		data->getParam(WCHDATA_JXJShiliShu,param);
		crJXJShili *shiliShu = (crJXJShili *)param;
		data->getParam(WCHDATA_JXJShiliWu,param);
		crJXJShili *shiliWu = (crJXJShili *)param;
		int weiID = shiliWei->getID();
		int shuID = shiliShu->getID();
		int wuID = shiliWu->getID();

		crJXJShili * winShiliPtr = NULL;
		crJXJShili * loseShiliPtr = NULL;

		std::string strAttack,strDefend;
		if(winshili == weiID)
		{
			strAttack = "魏";
			winShiliPtr = shiliWei;
		}
		else if(winshili == shuID)
		{
			strAttack = "蜀";
			winShiliPtr = shiliShu;
		}
		else if(winshili == wuID)
		{
			strAttack = "吴";
			winShiliPtr = shiliWu;
		}
		if(lostshili == weiID)
		{
			strDefend = "魏";
			loseShiliPtr = shiliWei;
		}
		else if(lostshili == shuID)
		{
			strDefend = "蜀";
			loseShiliPtr = shiliShu;
		}
		else if(lostshili == wuID)
		{
			strDefend = "吴";
			loseShiliPtr = shiliWu;
		}
		// 添加到国家历史
		if (winShiliPtr && winShiliPtr->getDataClass())
		{
			crData * shiliData = winShiliPtr->getDataClass();
			shiliData ->excHandle(MAKECREPARAM(WCH_LockData,1));
			shiliData ->getParam(WCHDATA_JXJCountryHistory,param);
			CountryHistory *history = (CountryHistory *)param;
			CRCore::ref_ptr<CountryHistoryChengChiChange > winMsg = new CountryHistoryChengChiChange;
			winMsg->m_infoId = 3072;
			winMsg->m_winOrlose = true;
			winMsg->m_chengchiId = itr->first;
			winMsg->m_shiliid =winshili;
			history->push_back(winMsg.get());

			// 设置建国系统的相应位
			ref_ptr<crTableIO> foundCountryCityTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliFoundCountryCityTab);

			if (foundCountryCityTab.valid())
			{
				crTableIO::StrVec record;
				int idIdx = foundCountryCityTab->getTitleIndex("id");
				int shiliIdx = foundCountryCityTab->getTitleIndex("shili");
				int orderIdx = foundCountryCityTab->getTitleIndex("顺序");
				if (foundCountryCityTab->queryOneRecord(idIdx,crArgumentParser::appItoa(chengchiid)
					,shiliIdx,crArgumentParser::appItoa(winshili),record)>=0)
				{
					shiliData->getParam(WCHDATA_JXJShiliFoundCountryWinCity,param);
					unsigned char winCity = *(unsigned char *)param;
					unsigned char order = atoi(record[orderIdx].c_str()) - 1;

					if (order < 8 && !(winCity & (1 << order)))
					{
						winCity |= 1 << order; 
						shiliData->inputParam(WCHDATA_JXJShiliFoundCountryWinCity,&winCity);
						if (winCity == (unsigned char)-1)
						{
							// 建国完成
							m_this->doEvent(WCH_JXJGenerateCountryOfficial,MAKECREPARAM(true,winshili));
						}
					}
				}
			}

			//ref_ptr<crTableIO>playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
			//if (playershilitab.valid())
			//{
			//	crTableIO::StrVec record;
			//	int guoduIndex = playershilitab->getTitleIndex("国都");

			//	if (playershilitab->queryOneRecord(0, crArgumentParser::appItoa(winshili), record) >= 0 && guoduIndex >= 0)
			//	{
			//		unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
			//		if (winshili == ashili && guoduid == itr->first)
			//		{
			//			//国民列表驻地变换
			//			shiliData->getParam(WCHDATA_JXJPeopleMap, param);
			//			PeopleMap* peopleMap = (PeopleMap *)param;

			//			ref_ptr<crGameServerPlayerData> memberPlayerData;
			//			//crData *playerData = NULL;
			//			//crPlayerDataEventPacket packet;
			//			for (PeopleMap::iterator itr_people = peopleMap->begin(); itr_people != peopleMap->end(); itr_people++)
			//			{
			//				memberPlayerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(itr->first));
			//				if (memberPlayerData.valid() && memberPlayerData->getPlayerGameData())
			//				{
			//					memberPlayerData->getPlayerGameData()->doEvent(WCH_JXJPlayerChengchiIDCheck);//判断我当前驻地是否丢失，如果丢失判断国都是否丢失，再迁移驻地
			//				}
			//			}
			//		}
			//	}
			//}
			shiliData ->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
		if (loseShiliPtr && loseShiliPtr->getDataClass())
		{
			crData * shiliData = loseShiliPtr->getDataClass();
			shiliData ->excHandle(MAKECREPARAM(WCH_LockData,1));
			shiliData ->getParam(WCHDATA_JXJCountryHistory,param);
			CountryHistory *history = (CountryHistory *)param;
			CRCore::ref_ptr<CountryHistoryChengChiChange > winMsg = new CountryHistoryChengChiChange;
			winMsg->m_infoId = 3071;
			winMsg->m_winOrlose = false;
			winMsg->m_chengchiId = itr->first;
			winMsg->m_shiliid = winshili;
			history->push_back(winMsg.get());

			////国民列表驻地变换
			//shiliData->getParam(WCHDATA_JXJPeopleMap, param);
			//PeopleMap* peopleMap = (PeopleMap *)param;

			//ref_ptr<crGameServerPlayerData> memberPlayerData;
			////crData *playerData = NULL;
			////crPlayerDataEventPacket packet;
			//for (PeopleMap::iterator itr_people = peopleMap->begin(); itr_people != peopleMap->end(); itr_people++)
			//{
			//	memberPlayerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(itr->first));
			//	if (memberPlayerData.valid() && memberPlayerData->getPlayerGameData())
			//	{
			//		memberPlayerData->getPlayerGameData()->doEvent(WCH_JXJPlayerChengchiIDCheck);//判断我当前驻地是否丢失，如果丢失判断国都是否丢失，再迁移驻地
			//	}
			//}

			shiliData ->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	
		//添加系统通告和灭国提示
		//////////////////////////////////////////////////////////////////////////
		//检测国都和陪都是否失守
		cityLose = 0;//1国都失守，-1陪都失守
		ref_ptr<crTableIO> playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		if(playershilitab.valid())
		{
			crTableIO::StrVec record;
			int guoduIndex = playershilitab->getTitleIndex("国都");
			int peiduIndex = playershilitab->getTitleIndex("陪都");

			if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(lostshili),record)>=0 && guoduIndex>=0 && peiduIndex>=0)
			{
				unsigned short guoduid = (unsigned short)(atoi(record[guoduIndex].c_str()));
				unsigned short peiduid = (unsigned short)(atoi(record[peiduIndex].c_str()));

				if(chengchiid == guoduid)
					cityLose = 1;
				else if(chengchiid == peiduid)
					cityLose = -1;			
			}
		}
		//处理提醒消息
		ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
		crTableIO::StrVec textrecord;
		if(1 == cityLose)
		{
			char szNotice[256];
			int aid = 4023;
			int did = 4022;
			if(chengchiTab.valid() && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0 && chengchiTab->getTitleIndex("名字")>0 )
			{
				if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(did),textrecord))
				{
					sprintf_s(szNotice,textrecord[1].c_str(),record[chengchiTab->getTitleIndex("名字")].c_str());
					//
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[通告]",szNotice,0,lostshili);
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
				}
			}
				
			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(aid),textrecord))
			{
				sprintf_s(szNotice,textrecord[1].c_str(),strDefend.c_str());
				//
				ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[通告]",szNotice,0,winshili);
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
			}
		}
		else if(-1 == cityLose)
		{//灭国
			int dismissTime = 0;
			crData * consortiaData;
			data->getParam(WCHDATA_JXJConsortiaMap,param);
			ConsortiaMap *consortiaMap = (ConsortiaMap *)param;
			for( ConsortiaMap::iterator mitr = consortiaMap->begin();
				 mitr != consortiaMap->end();
				 ++mitr )
			{
				if(mitr->second->getShiLi() == lostshili)
				{
					consortiaData = mitr->second->getDataClass();
					if(consortiaData)
						consortiaData->inputParam(WCHDATA_JXJConsortiaDismissTime,&dismissTime);			
				}
			}
			//清除国家官员
			if (loseShiliPtr && loseShiliPtr->getDataClass())
			{
				crData * shiliData = loseShiliPtr->getDataClass();
				shiliData->inputParam(WCHDATA_JXJChengxiang, NULL);
				shiliData->inputParam(WCHDATA_JXJDasima, NULL);
				shiliData->inputParam(WCHDATA_JXJDajiangjun, NULL);
			}
			char szNotice[256];
			int aid = 4025;
			int did = 4024;
			int textid = 4028;
			if(chengchiTab.valid() && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0 && chengchiTab->getTitleIndex("名字")>0 )
			{
				if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(did),textrecord))
				{
					sprintf_s(szNotice,textrecord[1].c_str(),record[chengchiTab->getTitleIndex("名字")].c_str());
					//
					ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[通告]",szNotice,0,lostshili);
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
				}
			}

			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(aid),textrecord))
			{
				sprintf_s(szNotice,textrecord[1].c_str(),strDefend.c_str());
				//
				ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[通告]",szNotice,0,winshili);
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
			}
			//全服通告某国灭亡
			if(textTab.valid() && textTab->queryOneRecord(0,crArgumentParser::appItoa(textid),textrecord))
			{
				sprintf_s(szNotice,textrecord[1].c_str(),strDefend.c_str(),strAttack.c_str(),strDefend.c_str());
				//
				ref_ptr<crJXJChatMessageInfo> chatInfo = new crJXJChatMessageInfo("[通告]",szNotice,0,0);
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermNoticeCreate,MAKECREPARAM(chatInfo.get(),NULL));
			}
			////////
			////////在胜利国数据中记录被灭亡的国家
			if (lostshili>=c_startShiliID && winshili>=c_startShiliID)
			{
				data->getParam(WCHDATA_JXJShiliWei+winshili-c_startShiliID,param);
				shili = (crJXJShili *)param;
				if(shili && shili->getDataClass())
				{
					shiliData = shili->getDataClass();
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJDestroyCountryid1,param);
					char *decountryid1 = (char *)param;
					shiliData->getParam(WCHDATA_JXJDestroyCountryid2,param);
					char *decountryid2 = (char *)param;

					if (*decountryid1>=0 && *decountryid1<=2)
						*decountryid2 = lostshili - c_startShiliID;
					else
						*decountryid1 = lostshili - c_startShiliID;
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));

	//判断轮回是否开始
	m_this->doEvent(WCH_JXJCheckGameServerCycleStart,MAKECREPARAM(cityLose,NULL));
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSelectRoleInfoTipsShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSelectRoleInfoTipsShowMethod::crJXJSelectRoleInfoTipsShowMethod():m_this(NULL)
{

}
crJXJSelectRoleInfoTipsShowMethod::crJXJSelectRoleInfoTipsShowMethod(const crJXJSelectRoleInfoTipsShowMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJSelectRoleInfoTipsShowMethod::inputParam(int i, void *param)
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
		m_this = (crImageBoxWidgetNode *)param;
		break;
	}
}
void crJXJSelectRoleInfoTipsShowMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_friendCanvas = str;
		break;
	case 1:
		m_enemyCanvas = str;
		break;
	}
}
void crJXJSelectRoleInfoTipsShowMethod::operator ()(CRCore::crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		ref_ptr<crCanvasNode>  friendTips = crFilterRenderManager::getInstance()->findCanvas(m_friendCanvas);
		ref_ptr<crCanvasNode>  enemyTips = crFilterRenderManager::getInstance()->findCanvas(m_enemyCanvas);
		if(!friendTips || !enemyTips) return;
		ref_ptr<crCanvasNode> canvas;
		crVector3 pos = m_this->getBoundBox().center();
		friendTips->setMatrix(crMatrix::translate(0,0.2f,0.0f));
		enemyTips->setMatrix(crMatrix::translate(0.0,0.2f,.0f));
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		if(cameraData)
		{
			cameraData->getParam(WCHDATA_JXJIsSelectRoleInfoTipsShow,param);
			m_isShow = *(bool *)param;
			m_isShow = !m_isShow;
			cameraData->inputParam(WCHDATA_JXJIsSelectRoleInfoTipsShow,&m_isShow);
			if(m_isShow)
			{
				CRCore::ScopedLock<crData> lock(*cameraData);
				cameraData->getParam(WCHDATA_SelectItem,param);
				crInstanceItem *selectItem = (crInstanceItem *)param;
				if (selectItem)
				{
					char isEnemy = 0;
					crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(selectItem,&isEnemy));
					if(isEnemy == -1)
						canvas = enemyTips;
					else if(isEnemy == 1)
						canvas = friendTips;

				}	
				if(canvas.get())
				{
					crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
				}
			}
			else
			{
				crFilterRenderManager::getInstance()->closeCanvas(friendTips.get());
				crFilterRenderManager::getInstance()->closeCanvas(enemyTips.get());
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUISelectRoleInfoTipsInitMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUISelectRoleInfoTipsInitMethod::crJXJUISelectRoleInfoTipsInitMethod():m_this(NULL)
{

}
crJXJUISelectRoleInfoTipsInitMethod::crJXJUISelectRoleInfoTipsInitMethod(const crJXJUISelectRoleInfoTipsInitMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJUISelectRoleInfoTipsInitMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJUISelectRoleInfoTipsInitMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_shili = str;
		break;
	case 1:
		m_grade= str;
		break;
	}
}
void crJXJUISelectRoleInfoTipsInitMethod::operator ()(CRCore::crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crTableIO::StrVec record;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		ref_ptr<crStaticTextWidgetNode>  shili = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_shili));
		ref_ptr<crStaticTextWidgetNode>  grade = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_grade));
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_SelectItem,param);
			crInstanceItem *selectItem = (crInstanceItem *)param;
			if (selectItem)
			{
				selectItem->getDataClass()->getParam(WCHDATA_Camp,param);
				unsigned char shiliid = *(unsigned char *)param;
				selectItem->getDataClass()->getParam(WCHDATA_Level,param);
				unsigned short level = *(unsigned char *)param;
				ref_ptr<crTableIO> playershilitab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
				if(playershilitab->queryOneRecord(0,crArgumentParser::appItoa(shiliid),record)>=0)
				{
					int shilinameindex = playershilitab->getTitleIndex("name");
					shili->setString(record[shilinameindex]);
					shili->setColor(COLOR_WHITE);
				}
				grade->setString(crArgumentParser::appItoa(level));
				grade->setColor(COLOR_WHITE);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleAddFriendMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleAddFriendMethod::crJXJBattleAddFriendMethod(){}
crJXJBattleAddFriendMethod::crJXJBattleAddFriendMethod(const crJXJBattleAddFriendMethod &handle){}
void crJXJBattleAddFriendMethod::inputParam(int i, void *param)
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
		m_this = (crWidgetNode *)param;
		break;
	}
}
void crJXJBattleAddFriendMethod::addParam(int i, const std::string& str)
{
}

void crJXJBattleAddFriendMethod::operator ()(CRCore::crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>canvas = m_this->getParentCanvas();
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(canvas.valid())
	{
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			CRCore::ScopedLock<crData> lock(*canvasData);

			canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
			RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
			canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
			crInstanceItem* curSelect = (crInstanceItem *)param;
			
			char isEnemy = 0;
			crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(curSelect,&isEnemy));
			if(isEnemy == 1)
			{
				RoomMemberInfoMap::iterator itr_info = infoMap->find(curSelect->getID());
				if(itr_info!=infoMap->end())
				{
					std::string nickname = itr_info->second->getCharacterName();
					std::string myname = crMyPlayerData::getInstance()->getCharacterName();

					if(!nickname.empty() && nickname.length()<13 && myname!=nickname)
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(4+nickname.length());
						stream->_writeString(nickname);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvGameAddFriend,stream.get());
						netConductor->getNetManager()->sendPacket("all",packet);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleSendMsgToFriendMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleSendMsgToFriendMethod::crJXJBattleSendMsgToFriendMethod(){}
crJXJBattleSendMsgToFriendMethod::crJXJBattleSendMsgToFriendMethod(const crJXJBattleSendMsgToFriendMethod &handle){}
void crJXJBattleSendMsgToFriendMethod::inputParam(int i, void *param)
{

}
void crJXJBattleSendMsgToFriendMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_canvasName = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_chatmode = str;
		break;
	case 3:
		m_radio = str;
		break;
	}
}

void crJXJBattleSendMsgToFriendMethod::operator ()(CRCore::crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		ref_ptr<crCanvasNode>  canvasNode = crFilterRenderManager::getInstance()->findCanvas(m_canvasName);
		if(!canvasNode)return;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		ref_ptr<crMultiSwitch>  curmodeSW = dynamic_cast<crMultiSwitch *>(canvasNode->getChildNode(m_chatmode));
		ref_ptr<crRadioGroupWidgetNode>  radioGroup = dynamic_cast<crRadioGroupWidgetNode *>(canvasNode->getWidget(m_radio));
		ref_ptr<crEditWidgetNode>  input = dynamic_cast<crEditWidgetNode *>(canvasNode->getWidget(m_input));
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_SelectItem,param);
			crInstanceItem *selectItem = (crInstanceItem *)param;
			if (selectItem)
			{
				char isEnemy = 0;
				crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(selectItem,&isEnemy));
				if(isEnemy == 1)
				{
					crRole *role = dynamic_cast<crRole *>(selectItem);
					std::string nickname;
					std::string myname = crMyPlayerData::getInstance()->getCharacterName();
					crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
					ref_ptr<crRoomPlayer> roomMember = room->getMember(role->getPlayerID());
					nickname = roomMember->getCharacterName();
					crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(selectItem,&isEnemy));
					if(!nickname.empty() && nickname.length()<13 && myname!=nickname)
					{
						input -> setString("\\" + nickname  + " ");
						//crBrain::getInstance()->getDataClass()->inputParam(WCHDATA_JXJChatOneToOneReciverName,&nickname);
						if (curmodeSW.valid())
						{
							curmodeSW->setActiveSwitchSet(ChatOneToOne-1);
						}
						if(radioGroup.valid())
						{
							radioGroup->select(ChatOneToOne);
						}
					}
				}
			}	
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIInitBattleInfoCanvasMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUIInitBattleInfoCanvasMethod::crJXJUIInitBattleInfoCanvasMethod():
	m_this(NULL)
{

}
crJXJUIInitBattleInfoCanvasMethod::crJXJUIInitBattleInfoCanvasMethod(const crJXJUIInitBattleInfoCanvasMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_strName(handle.m_strName),
	m_strAShili(handle.m_strAShili),
	m_strDShili(handle.m_strDShili),
	m_kaoshang1(handle.m_kaoshang1),
	m_kaoshang2(handle.m_kaoshang2),
	m_donghuaA(handle.m_donghuaA),
	m_donghuaD(handle.m_donghuaD),
	m_terrainInfo(handle.m_terrainInfo)
{

}
void crJXJUIInitBattleInfoCanvasMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJUIInitBattleInfoCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_strName = str;
		break;
	case 1:
		m_terrainInfo = str;
		break;
	case 2:
		m_strAShili = str;
		break;
	case 3:
		m_strDShili = str;
		break;
	case 4:
		m_kaoshang1 = str;
		break;
	case 5:
		m_kaoshang2 = str;
		break;
	case 6:
		m_donghuaA = str;
		break;
	case 7:
		m_donghuaD = str;
		break;
	}
}
void crJXJUIInitBattleInfoCanvasMethod::operator()(CRCore::crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		ref_ptr<crStaticTextWidgetNode>  name = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strName));
		ref_ptr<crStaticTextWidgetNode>  terrainInfo = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_terrainInfo));
		ref_ptr<crMultiSwitch> sw_a = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strAShili));
		ref_ptr<crMultiSwitch> sw_d = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strDShili));
		//ref_ptr<crButtonWidgetNode> kaoshang1 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_kaoshang1));
		//ref_ptr<crButtonWidgetNode> kaoshang2 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_kaoshang2));
		//ref_ptr<crMultiSwitch> sw_donghuaa = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_donghuaA));
		//ref_ptr<crMultiSwitch> sw_donghuad = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_donghuaD));
		//if(kaoshang1.valid())
			//kaoshang1->setVisiable(false);
		//if(kaoshang2.valid())
			//kaoshang2->setVisiable(false);

		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room && name.valid())
		{
			crTableIO::StrVec record;
			void *param;
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			unsigned char type = *(unsigned char *)param;
			if(type == FT_Chengchi)
			{
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short battleChengchiID = *(unsigned short*)param;
				if(battleChengchiID != 0)
				{
					ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
					if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(battleChengchiID),record)>=0)
					{
						name->setString(record[chengchiTab->getTitleIndex("名字")]);
						if(terrainInfo.valid())
						{
							terrainInfo->setString(record[chengchiTab->getTitleIndex("地形")]);
						}
					}
				}

				crData *roomData = room->getDataClass();
				if(roomData)
				{
					roomData->getParam(WCHDATA_JXJBattleID,param);
					int battleid = *(int *)param;
					unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
					unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));

					//crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 
					//if (playerGameData && playerGameData->getDataClass())
					//{
						//crData *playerData = playerGameData->getDataClass();
						//playerData->getParam(WCHDATA_JXJShiliID,param);
						//unsigned char myshiliid = *(unsigned char *)param;
						//if (myshiliid == attackShili)
						//{
							//kaoshang1->setVisiable(true);
						//}
						//if (myshiliid == defenceShili)
						//{
							//kaoshang2->setVisiable(true);
						//}
					//}

					if(sw_a.valid())
						sw_a->setActiveSwitchSet(attackShili-c_startShiliID);

					if(sw_d.valid())
					{
						if(defenceShili == 2)
						{
							sw_d->setActiveSwitchSet(3);
						}
						else
						{
							sw_d->setActiveSwitchSet(defenceShili-c_startShiliID);
						}
					}
				}
			}
			else if(type == FT_Fuben)
			{
				data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
				unsigned short fubenid = *(unsigned short*)param;
				if(fubenid != 0)
				{
					ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
					if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
					{
						//name->setFontSize(crVector2s(13,13));
						name->setString(record[fubenTab->getTitleIndex("name")]);
						if(terrainInfo.valid())
						{
							terrainInfo->setString(record[fubenTab->getTitleIndex("地形")]);
						}
					}
				}
			}
			else if(type == FT_WorldFuben)
			{
				name->setString(std::string("世界战场"));
				ref_ptr<crMultiSwitch> timespliter = dynamic_cast<crMultiSwitch *>(m_this->getChildNode("Juqingmoshi_time"));
				if(timespliter.valid())
					timespliter->setVisiable(false);
			}
			//{
			//	int roomod = room->getRoomID();
			//	roomData->getParam(WCHDATA_JXJBattleID,param);
			//	int battleid = *(int *)param;
			//	unsigned short chengchiid = HIINT32(battleid);
			//	if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record))
			//	{
			//		if(name)
			//			name->setString(record[chengchiTab->getTitleIndex("名字")]);
			//	}
			//	else
			//	{
			//		crData *mydata = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
			//		mydata->getParam(WCHDATA_JXJFubenID,param);
			//		unsigned short fubenid = *(unsigned short *)param;
			//		if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record))
			//		{
			//			if(name)
			//			{
			//				name->setFontSize(crVector2s(13,13));
			//				name->setString(record[fubenTab->getTitleIndex("name")]);
			//			}
			//		}
			//	}
			//}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJUIInitBattleTimerUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUIInitBattleTimerUpdateMethod::crJXJUIInitBattleTimerUpdateMethod():
	m_this(NULL)
{

}
crJXJUIInitBattleTimerUpdateMethod::crJXJUIInitBattleTimerUpdateMethod(const crJXJUIInitBattleTimerUpdateMethod &handle):
	crMethod(handle),
	m_this(NULL)
{

}

void crJXJUIInitBattleTimerUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJUIInitBattleTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_strTimer[i] = str;
		break;
	default:
		break;
	}
}
void crJXJUIInitBattleTimerUpdateMethod::operator()(CRCore::crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	ref_ptr<crMultiSwitch> mulitSw[4] = { NULL };
	for (int i = 0; i < 4; i++)
	{
		mulitSw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strTimer[i]));
		if (mulitSw[i].valid())
		{
			mulitSw[i]->setActiveSwitchSet(0);
		}
	}
	if(playerData)
	{
		void *param;
		crData *data = playerData->getDataClass();
		data->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		if(type == FT_WorldFuben)
		{
			for (int i = 0; i < 4; i++)
			{
				if (mulitSw[i].valid())
					mulitSw[i]->setVisiable(false);
			}
		}

		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			int remain_time = room->timeRemain();
			if(remain_time>0)
			{
				int minute = remain_time/60;
				int mx = minute / 10;
				int my = minute % 10;

				if (mx > 9) mx = 9;
				if (mulitSw[0].valid()) mulitSw[0]->setActiveSwitchSet(mx);
				if (mulitSw[1].valid()) mulitSw[1]->setActiveSwitchSet(my);


				int second = remain_time % 60;
				int sx = second / 10;
				int sy = second % 10;
				if (mulitSw[2].valid()) mulitSw[2]->setActiveSwitchSet(sx);
				if (mulitSw[3].valid()) mulitSw[3]->setActiveSwitchSet(sy);
			}
			
			room->timeRemain() -= 1.0f;
		}
	}
}

/////////////////////////////////////////
//
//crJXJCardPushSelectRoleMethod
//
/////////////////////////////////////////
crJXJCardPushSelectRoleMethod::crJXJCardPushSelectRoleMethod():
	m_index(0){}
crJXJCardPushSelectRoleMethod::crJXJCardPushSelectRoleMethod(const crJXJCardPushSelectRoleMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJCardPushSelectRoleMethod::inputParam(int i, void *param)
{
}

void crJXJCardPushSelectRoleMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJCardPushSelectRoleMethod::operator()(crHandle &handle)
{
	crMyPlayerData *me = crMyPlayerData::getInstance();
	crRole *role = me->getLoadedRoleByIndex(m_index);
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(role && camera.valid())
	{
		unsigned int guisestate = GS_Normal;
		role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate /*|| guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
		{//不可被自己选中
			return;
		}
		//相机选择
		bool found = false;
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
		SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
		crNode *fxNode;
		for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
			itr != selectMyRoles->end();
			++itr )
		{

			fxNode = (*itr)->findSceneFxNode("$SelectDecal");
			if(fxNode)
			{
				crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
				if(callback)
				{
					callback->setVisiable(false);
				}
			}
		}
		selectMyRoles->clear();
		selectMyRoles->push_back(role);
		fxNode = role->findSceneFxNode("$SelectDecal");
		if(fxNode)
		{
			crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
			if(callback)
			{
				callback->setVisiable(true);
			}
		}
		crRole *role = (*selectMyRoles)[0].get();
		crVector3 pos = role->getPosition();
		camera->doEvent(WCH_CameraFocus,MAKECREPARAM(&pos,NULL));
	}
}

/////////////////////////////////////////
//
//crJXJUIInitExitBattleShowMethod
//
/////////////////////////////////////////
crJXJUIInitExitBattleShowMethod::crJXJUIInitExitBattleShowMethod():
m_codetext(1096),
m_timer(60){}
crJXJUIInitExitBattleShowMethod::crJXJUIInitExitBattleShowMethod(const crJXJUIInitExitBattleShowMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas),
	m_input(handle.m_input),
	m_codetext(handle.m_codetext),
	m_timer(handle.m_timer)
{
}
void crJXJUIInitExitBattleShowMethod::inputParam(int i, void *param)
{
}

void crJXJUIInitExitBattleShowMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_codetext = atoi(str.c_str());
		break;
	case 3:
		m_timer = atoi(str.c_str());
		break;
	}
}

void crJXJUIInitExitBattleShowMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		crData *data = canvas->getDataClass();
		data->inputParam(WCHDATA_JXJExitSceneCodeText,&m_codetext);
		data->inputParam(WCHDATA_JXJExitSceneTimer,&m_timer);

		crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
		noticeHandle->inputParam(WCHDATA_NoticeTextID,&m_codetext);
		int mode = 2;
		noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
		std::string param1 = crArgumentParser::appItoa(m_timer);
		noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
		std::string output;
		noticeHandle->inputParam(WCHDATA_NoticeOutPut,&output);
		crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);

		ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_input));
		if(input.valid())
		{
			input->setString(output);
		}
	}
	//ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	//if(camera.valid())
	//{
	//	ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_input));
	//	//crStaticTextWidgetNode *drag = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_drag));
	//	if(input.valid())
	//	{
	//		input->setString(std::string("战场尚未结束，是否确定退出？          （60秒后自动退出）"));
	//	}
	//	//if(drag)
	//	//{
	//	//drag->setString(std::string("战场提示"));
	//	//}
	//	crData *camadata = camera->getAttachedNode()->getDataClass();

	//	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	//	if(myPlayer)
	//	{
	//		crData *myPlayerData = myPlayer->getDataClass();
	//		if(myPlayerData)
	//		{
	//			void *param;
	//			myPlayerData->getParam(WCHDATA_JXJVipLv,param);
	//			unsigned char vipLevel = *(unsigned char *)param;
	//			int selectTimer = (int)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJChooseTimeLimit,vipLevel).c_str()));
	//			camadata->inputParam(WCHDATA_JXJBattleChoosePointTimer,&selectTimer);
	//		}
	//	}
	//}
}

/////////////////////////////////////////
//
//crJXJTimeUpAutoExitMethod
//
/////////////////////////////////////////
crJXJTimeUpAutoExitMethod::crJXJTimeUpAutoExitMethod():m_this(NULL){}
crJXJTimeUpAutoExitMethod::crJXJTimeUpAutoExitMethod(const crJXJTimeUpAutoExitMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_input(handle.m_input)
{
}
void crJXJTimeUpAutoExitMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}

void crJXJTimeUpAutoExitMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_input = str;
		break;
	}
}

void crJXJTimeUpAutoExitMethod::operator()(crHandle &handle)
{
	void *param;
	bool bExit = true;
	crData *data = m_this->getDataClass();
	data->getParam(WCHDATA_JXJExitSceneCodeText,param);
	int texid = *(int *)param;
	data->getParam(WCHDATA_JXJExitSceneTimer,param);
	short* timer = (short *)param;
	(*timer)--;
	if((*timer) == 0)
	{
		bExit = false;
	}
	crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
	noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
	int mode = 2;
	noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
	std::string param1 = crArgumentParser::appItoa(*timer);
	noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
	std::string output;
	noticeHandle->inputParam(WCHDATA_NoticeOutPut,&output);
	crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	
	ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_input));
	if(input.valid())
	{
		input->setString(output);
	}

	//ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	//if(camera.valid())
	//{
	//	ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode*>(m_this->getWidget(m_input));
	//	crData *camadata = camera->getAttachedNode()->getDataClass();
	//	if(!camadata)return;
	//	camadata->excHandle(MAKECREPARAM(WCH_LockData,1));
	//	camadata->getParam(WCHDATA_JXJBattleChoosePointTimer,param);
	//	short time = *(short*)param;
	//	time--;
	//	char temp[128];
	//	if(input.valid())
	//	{
	//		sprintf(temp,"战场尚未结束，是否确定退出？          （%02d秒后自动退出）",time);
	//		input->setString(std::string(temp));
	//	}
	//	if(time == 0)
	//	{
	//		bExit = false;
	//	}
	//	camadata->inputParam(WCHDATA_JXJBattleChoosePointTimer,&time);
	//	camadata->excHandle(MAKECREPARAM(WCH_LockData,0));
	//}
	handle.outputParam(0,&bExit);
}

///////////////////////////////////////////
////
////crJXJInitPassUICanvasMethod
////
///////////////////////////////////////////
//crJXJInitPassUICanvasMethod::crJXJInitPassUICanvasMethod():m_this(NULL){}
//crJXJInitPassUICanvasMethod::crJXJInitPassUICanvasMethod(const crJXJInitPassUICanvasMethod& handle):
//	crMethod(handle),
//	m_this(NULL),
//	m_input_a(handle.m_input_a),
//	m_input_d(handle.m_input_d)
//{
//}
//void crJXJInitPassUICanvasMethod::inputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		if(param == 0)
//		{
//			m_this = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crCanvasNode *)param;
//		break;
//	}
//}
//
//void crJXJInitPassUICanvasMethod::addParam(int i, const std::string& str)
//{
//	switch(i)
//	{
//	case 0:
//		m_input_a = str;
//		break;
//	case 1:
//		m_input_d = str;
//		break;
//	case 2:
//		m_progress = str;
//		break;
//	case 3:
//		m_seqRed = str;
//		break;
//	case 4:
//		m_seqBlue = str;
//		break;
//	case 5:
//		m_smallProgressF = str;
//		break;
//	case 6:
//		m_smallProgressS = str;
//		break;
//	case 7:
//		m_smallProgressT = str;
//		break;
//	case 8:
//		m_canvas = str;
//		break;
//	}
//}
//
//void crJXJInitPassUICanvasMethod::operator()(crHandle &handle)
//{
//	void *param;
//	if(m_this)
//	{
//		ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//		if(!canvas) return;
//		crFilterRenderManager::getInstance()->showCanvas(m_canvas,true);
//		crStaticTextWidgetNode *inputA = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input_a));
//		crStaticTextWidgetNode *inpudD = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input_d));
//		crProgressWidgetNode * progress = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progress));
//		crSequence *seqRed = dynamic_cast<crSequence *>(canvas->getChildNode(m_seqRed));
//		crSequence *seqBlue = dynamic_cast<crSequence *>(canvas->getChildNode(m_seqBlue));
//		crProgressWidgetNode *progressF = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgressF));
//		crProgressWidgetNode *progressS = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgressS));
//		crProgressWidgetNode *progressT = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgressT));
//
//		if(!inputA || !inpudD || !progress || !seqRed || !seqBlue) return;
//		seqRed->setMode(crSequence::STOP);
//		seqBlue->setMode(crSequence::STOP);
//		seqRed->setVanishWhenStop(true);
//		seqBlue->setVanishWhenStop(true);
//		progress->setProgress(0.5f);
//		progressF->setProgress(0.5f);
//		progressS->setProgress(0.5f);
//		progressT->setProgress(0.5f);
//		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
//		crData *room_data = room->getDataClass();
//		if(!room_data) return;
//		room_data->getParam(WCHDATA_JXJPassRedRemain, param);
//		short red_remain = *(short *)param;
//		room_data->getParam(WCHDATA_JXJPassBlueRemain, param);
//		short blue_remain = *(short *)param;
//		inputA->setString(crArgumentParser::appItoa(red_remain));
//		inpudD->setString(crArgumentParser::appItoa(blue_remain));
//	}
//}

/////////////////////////////////////////
//
//crJXJUIServerDisconnectMessageBoxMethod
//
/////////////////////////////////////////
crJXJUIServerDisconnectMessageBoxMethod::crJXJUIServerDisconnectMessageBoxMethod(){}
crJXJUIServerDisconnectMessageBoxMethod::crJXJUIServerDisconnectMessageBoxMethod(const crJXJUIServerDisconnectMessageBoxMethod& handle):
	crMethod(handle),
	m_msgCanvas(handle.m_msgCanvas),
	m_message(handle.m_message),
	m_messageIndex(handle.m_messageIndex)
{
}
void crJXJUIServerDisconnectMessageBoxMethod::inputParam(int i, void *param)
{
}

void crJXJUIServerDisconnectMessageBoxMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_msgCanvas = str;
		break;
	case 1:
		m_message = str;
		break;
	case 2:
		m_messageIndex = atoi(str.c_str());
		break;
	}
}

void crJXJUIServerDisconnectMessageBoxMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO> textTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJCodeTextTab);
	crTableIO::StrVec record;
	std::string str;
	if(textTab->queryOneRecord(0,crArgumentParser::appItoa(m_messageIndex),record)>=0)
	{
		std::string str = record[1];
		ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_msgCanvas);
		if(msgCanvas.valid()  /*&& !msgCanvas->getVisiable()*/)
		{
			ref_ptr<crStaticTextWidgetNode>  messageInfo = dynamic_cast<crStaticTextWidgetNode *>(msgCanvas->getWidget(m_message));
			if(!messageInfo) return;
			messageInfo->setAlignment(CRText::crText::LEFT_BASE_LINE);
			messageInfo->setString(str);
			crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
		}
		else
		{
			if(IDOK == MessageBox(crStartHandler::getInstance()->getWindowHandle(),str.c_str(),"提示",MB_OK))
				exit(0);
		}
	}
}

/////////////////////////////////////////
//
//crJXJCameraMouseOnEdgeMethod
//
/////////////////////////////////////////
crJXJCameraMouseOnEdgeMethod::crJXJCameraMouseOnEdgeMethod()
{
}
crJXJCameraMouseOnEdgeMethod::crJXJCameraMouseOnEdgeMethod(const crJXJCameraMouseOnEdgeMethod& handle) :
crMethod(handle),
m_delta(handle.m_delta),
m_delta_extra(handle.m_delta_extra),
m_up(handle.m_up),
m_upleft(handle.m_upleft),
m_upright(handle.m_upright),
m_down(handle.m_down),
m_downleft(handle.m_downleft),
m_downright(handle.m_downright),
m_left(handle.m_left),
m_right(handle.m_right)
{
}
void crJXJCameraMouseOnEdgeMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if (param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		if (param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (crGUIEventAdapter *)(LOCREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
}
void crJXJCameraMouseOnEdgeMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_delta = atof(str.c_str());
		break;
	case 1:
		m_delta_extra = atof(str.c_str());
		break;
	case 2:
		m_down = atoi(str.c_str());
		break;
	case 3:
		m_up = atoi(str.c_str());
		break;
	case 4:
		m_left = atoi(str.c_str());
		break;
	case 5:
		m_right = atoi(str.c_str());
		break;
	case 6:
		m_upleft = atoi(str.c_str());
		break;
	case 7:
		m_upright = atoi(str.c_str());
		break;
	case 8:
		m_downleft = atoi(str.c_str());
		break;
	case 9:
		m_downright = atoi(str.c_str());
		break;
	}
}
void crJXJCameraMouseOnEdgeMethod::operator()(crHandle &handle)
{
	bool canceltask = false;
	if (m_ea)
	{
		float x = m_ea->getXnormalized();
		float y = m_ea->getYnormalized();
		unsigned char cameraMove = crCameraData::None;
		if (x <= -1.0f + m_delta)
		{
			cameraMove = crCameraData::MoveLeft;
		}
		else if (x >= 1.0f - m_delta)
		{
			cameraMove = crCameraData::MoveRight;
		}
		else if (y <= -1.0f + m_delta)
		{
			cameraMove = crCameraData::MoveUp;
		}
		else if (y >= 1.0f - m_delta)
		{
			cameraMove = crCameraData::MoveDown;
		}
		//四个角的判断区域比边的判断区域稍大，优先级高
		if (x<-1.0f + m_delta_extra && y>1.0f - m_delta_extra)
		{
			cameraMove = crCameraData::MoveLeft | crCameraData::MoveDown;
		}
		else if (x > 1.0f - m_delta_extra && y > 1.0f - m_delta_extra)
		{
			cameraMove = crCameraData::MoveRight | crCameraData::MoveDown;
		}
		else if (x > 1.0f - m_delta_extra && y < -1.0f + m_delta_extra)
		{
			cameraMove = crCameraData::MoveRight | crCameraData::MoveUp;
		}
// 		else if (x < -1.0f + m_delta_extra && y < -1.0f + m_delta_extra)
// 		{
// 			cameraMove = crCameraData::MoveLeft | crCameraData::MoveUp;
// 		}

		crData *data = m_this->getDataClass();
		data->inputParam(WCHDATA_CameraMove, &cameraMove);

		if (cameraMove != crCameraData::None)
		{
			ref_ptr<crCanvasNode>  mouseCanvas = crFilterRenderManager::getInstance()->getMouseCanvas();
			if (mouseCanvas.valid())
			{
				ref_ptr<crMultiSwitch>  mouseSW = dynamic_cast<crMultiSwitch *>(mouseCanvas->getChild(0));
				if (mouseSW.valid())
				{
					if (cameraMove == crCameraData::MoveUp)
						mouseSW->setActiveSwitchSet(m_up);
					else if (cameraMove == crCameraData::MoveDown)
						mouseSW->setActiveSwitchSet(m_down);
					else if (cameraMove == crCameraData::MoveLeft)
						mouseSW->setActiveSwitchSet(m_left);
					else if (cameraMove == crCameraData::MoveRight)
						mouseSW->setActiveSwitchSet(m_right);
					else if (cameraMove == (crCameraData::MoveUp | crCameraData::MoveLeft))
						mouseSW->setActiveSwitchSet(m_upleft);
					else if (cameraMove == (crCameraData::MoveUp | crCameraData::MoveRight))
						mouseSW->setActiveSwitchSet(m_upright);
					else if (cameraMove == (crCameraData::MoveDown | crCameraData::MoveLeft))
						mouseSW->setActiveSwitchSet(m_downleft);
					else if (cameraMove == (crCameraData::MoveDown | crCameraData::MoveRight))
						mouseSW->setActiveSwitchSet(m_downright);
					// 			else 
					// 				mouseSW->setActiveSwitchSet(0);

					canceltask = true;
				}
			}
		}
	}
	
	handle.outputParam(0, &canceltask);
}

///////////////////////////////////////////
////
////crJXJPassCanavsUIUpdateMethod
////
///////////////////////////////////////////
//crJXJPassCanavsUIUpdateMethod::crJXJPassCanavsUIUpdateMethod()
//{
//}
//crJXJPassCanavsUIUpdateMethod::crJXJPassCanavsUIUpdateMethod(const crJXJPassCanavsUIUpdateMethod& handle):
//	crMethod(handle)
//{
//	m_scoreVec.resize(0);
//	m_offsetVec.resize(0);
//}
//void crJXJPassCanavsUIUpdateMethod::inputParam(int i, void *param)
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
//		m_this = (crCanvasNode*)param;
//		break;
//	}
//}
//void crJXJPassCanavsUIUpdateMethod::addParam(int i, const std::string& str)
//{
//	switch(i)
//	{
//	case 0:
//		m_inputRed = str;
//		break;
//	case 1:
//		m_inputBlue = str;
//		break;
//	case 2:
//		m_progressBar = str;
//		break;
//	case 3:
//		m_seqRed = str;
//		break;
//	case 4:
//		m_seqBlue = str;
//		break;
//	case 5:
//		m_imageArrow = str;
//		break;
//	case 6:
//		m_smallProgress1 = str;
//		break;
//	case 7:
//		m_smallProgress2 = str;
//		break;
//	case 8:
//		m_smallProgress3 = str;
//		break;
//	case 9:
//		m_input1 = str;
//		break;
//	case 10:
//		m_input2 = str;
//		break;
//	case 11:
//		m_input3 = str;
//		break;
//	case 12:
//		m_arrow1 = str;
//		break;
//	case 13:
//		m_arrow2 = str;
//		break;
//	case 14:
//		m_arrow3 = str;
//		break;
//	case 15:
//		m_canvas = str;
//		break;
//	}
//}
//void crJXJPassCanavsUIUpdateMethod::operator()(crHandle &handle)
//{
//	if(!m_this) return;
//	ref_ptr<crCanvasNode>canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
//	if(!canvas) return;
//	crStaticTextWidgetNode *inputRed = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputRed));
//	crStaticTextWidgetNode *inputBlue = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_inputBlue));
//	crStaticTextWidgetNode *inputF = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1));
//	crStaticTextWidgetNode *inputS = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2));
//	crStaticTextWidgetNode *inputT = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input3));
//	crProgressWidgetNode *progress = dynamic_cast<crProgressWidgetNode *>(canvas->getWidget(m_progressBar));
//	crProgressWidgetNode *smallProgressF = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgress1));
//	crProgressWidgetNode *smallProgressS = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgress2));
//	crProgressWidgetNode *smallProgressT = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_smallProgress3));
//	crSequence *seqRed = dynamic_cast<crSequence *>(canvas->getChildNode(m_seqRed));
//	crSequence *seqBlue = dynamic_cast<crSequence *>(canvas->getChildNode(m_seqBlue));
//	crMatrixTransform *imageArrow = dynamic_cast<crMatrixTransform *>(canvas->getChildNode(m_imageArrow));
//	crMatrixTransform *smallImageArrowF = dynamic_cast<crMatrixTransform *>(m_this->getChildNode(m_arrow1));
//	crMatrixTransform *smallImageArrowS = dynamic_cast<crMatrixTransform *>(m_this->getChildNode(m_arrow2));
//	crMatrixTransform *smallImageArrowT = dynamic_cast<crMatrixTransform *>(m_this->getChildNode(m_arrow3));
//	if(!inputRed || !inputBlue || !inputF || !inputS || !inputT || !progress || !seqRed || !imageArrow) return;
//	if(!smallProgressF || !smallProgressS || !smallProgressT || !smallImageArrowF || !smallImageArrowS || !smallImageArrowT) return;
//	vector<ref_ptr<crStaticTextWidgetNode>> inputVec;
//	vector<ref_ptr<crProgressWidgetNode>> smallProgressVec;
//	vector<ref_ptr<crMatrixTransform>> smallArrowVec;
//	inputVec.push_back(inputF);
//	inputVec.push_back(inputS);
//	inputVec.push_back(inputT);
//	smallProgressVec.push_back(smallProgressF);
//	smallProgressVec.push_back(smallProgressS);
//	smallProgressVec.push_back(smallProgressT);
//	smallArrowVec.push_back(smallImageArrowF);
//	smallArrowVec.push_back(smallImageArrowS);
//	smallArrowVec.push_back(smallImageArrowT);
//	crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
//	if(!room) return;
//
//	crData *roomData = room->getDataClass();
//	if(!roomData) return;
//	void *param;
//	roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
//	roomData->getParam(WCHDATA_JXJPassRedRemain,param);
//	short *red_remain = (short *)param;
//	roomData->getParam(WCHDATA_JXJPassBlueRemain,param);
//	short *blue_remain = (short *)param;
//	roomData->getParam(WCHDATA_JXJCheckPointVec,param);
//	PassCheckVec *checksVec = (PassCheckVec *)param;
//	roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
//
//	inputRed->setString(crArgumentParser::appItoa(*red_remain));
//	inputBlue->setString(crArgumentParser::appItoa(*blue_remain));
//
//	if(m_scoreVec.empty())
//		m_scoreVec.resize(checksVec->size()+1,0);
//	if(m_offsetVec.empty())
//		m_offsetVec.resize(checksVec->size(),0);
//
//	PassCheckVec::iterator itr_passPoint = checksVec->begin();
//	int index = 0;
//	char type = 0;
//	short score = 0.0f;
//	short checkLimit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckPointLimit).c_str());
//	for(;itr_passPoint!=checksVec->end();itr_passPoint++,index++)
//	{
//		type = itr_passPoint->get()->getType();
//		score = itr_passPoint->get()->getScore();
//		crMultiSwitch *imageSwitch = dynamic_cast<crMultiSwitch *>(smallArrowVec[index]->getChild(0));
//		float progressValue = (checkLimit-score)/(float)(2*checkLimit);
//		smallProgressVec[index]->setProgress(progressValue);
//		const crBoundingBox & progressbox = smallProgressVec[index]->getBoundBox();
//
//		if(score <=-checkLimit)
//			inputVec[index]->setString(string("攻方已占领"));
//		else if(score >= checkLimit)
//			inputVec[index]->setString(string("守方已占领"));
//		else
//		{
//			if(score > m_scoreVec[index])
//			{
//				imageSwitch->setActiveSwitchSet(1);
//				m_offsetVec[index] = progressValue;
//				m_scoreVec[index] = score;
//			}
//			else if(score < m_scoreVec[index])
//			{
//				imageSwitch->setActiveSwitchSet(2);
//				m_offsetVec[index] = progressValue-1.0f;
//				m_scoreVec[index] = score;
//			}
//			else if(score == 0)
//			{
//				imageSwitch->setActiveSwitchSet(0);
//				m_offsetVec[index] = 0.0f;
//			}
//			if(type == 0)
//				inputVec[index]->setString(string("粮仓"));
//			else if(type == 1)
//				inputVec[index]->setString(string("关隘"));
//		}
//
//		smallArrowVec[index]->setMatrix(crMatrix::translate(crVector3f(m_offsetVec[index] * progressbox.xLength(),0.0f,0.0f)));
//	}
//	float progressV = (checkLimit-score)/(float)(2*checkLimit);
//	const crBoundingBox & pbbox = progress->getBoundBox();
//	progress->setProgress(progressV);
//
//	crMultiSwitch *arrowSwitch =  dynamic_cast<crMultiSwitch *>(imageArrow->getChild(0));
//	if(score >m_scoreVec[index])
//	{
//		arrowSwitch->setActiveSwitchSet(1);
//		m_scoreVec[index] = score;
//	}
//	if(score < m_scoreVec[index])
//	{
//		arrowSwitch->setActiveSwitchSet(2);
//		m_scoreVec[index] = score;
//	}
//	else if(score == 0)
//		arrowSwitch->setActiveSwitchSet(0);
//	imageArrow->setMatrix(crMatrix::translate(crVector3f(m_offsetVec[index-1] * pbbox.xLength(),0.0f,0.0f)));
//
//	if(score <= -checkLimit)
//		seqRed->setMode(crSequence::START);
//	else if(score >= checkLimit)
//		seqBlue->setMode(crSequence::START);
//	else
//	{
//		seqRed->setMode(crSequence::STOP);
//		seqBlue->setMode(crSequence::STOP);
//	}
//}

/////////////////////////////////////////
//
//crJXJFuBenReplayMethod
//
/////////////////////////////////////////
crJXJFuBenReplayMethod::crJXJFuBenReplayMethod()
{
}
crJXJFuBenReplayMethod::crJXJFuBenReplayMethod(const crJXJFuBenReplayMethod& handle):
	crMethod(handle)
{
}
void crJXJFuBenReplayMethod::inputParam(int i, void *param)
{
}
void crJXJFuBenReplayMethod::addParam(int i, const std::string& str)
{
}
void crJXJFuBenReplayMethod::operator()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		//ref_ptr<crStreamBuf> stream = new crStreamBuf;
		//stream->createBuf(0);
		crPlayerDataSceneEventPacket packet;
		crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvFuBenRestart,NULL);
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFuBenRestartMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvFuBenRestartMethod::crJXJRecvFuBenRestartMethod(){}
crJXJRecvFuBenRestartMethod::crJXJRecvFuBenRestartMethod(const crJXJRecvFuBenRestartMethod &handle)
{

}
void crJXJRecvFuBenRestartMethod::inputParam(int i, void *param)
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
void crJXJRecvFuBenRestartMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvFuBenRestartMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == SceneServer)
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				crRoom *room = callback->findRoom(playerData->getRoomID());
				if(room)
				{
					room->doEvent(WCH_JXJFuBenRoomRestart);
					room->timeRemain() = 0;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvRoomChatMessageMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvRoomChatMessageMethod::crJXJRecvRoomChatMessageMethod(){}
crJXJRecvRoomChatMessageMethod::crJXJRecvRoomChatMessageMethod(const crJXJRecvRoomChatMessageMethod &handle)
{

}
void crJXJRecvRoomChatMessageMethod::inputParam(int i, void *param)
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
void crJXJRecvRoomChatMessageMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvRoomChatMessageMethod::operator ()(crHandle &handle)
{
	//void *param;
	if(m_this && m_stream.valid())
	{
		if(m_netType == SceneServer)
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			int playerid = m_this->getPlayerID();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData.valid())
			{
				crData *serverData = crServerBrainHandle::getInstance()->getDataClass();
				void *param;
				serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
				serverData->getParam(WCHDATA_JXJNoChatSet,param);
				NoChatSet *nochatset = (NoChatSet *)param;
				if(nochatset->find(playerid)!=nochatset->end())
				{//向玩家回复被禁言
					if(playerData.valid())
					{
						ref_ptr<crStreamBuf> stream = new crStreamBuf;
						stream->createBuf(5);
						stream->_writeInt(playerid);
						stream->_writeChar(-1);
						crJXJGameChatPacket packet;
						packet.setStreamBuf(stream.get());
						sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
					serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
					return;
				}
				serverData->excHandle(MAKECREPARAM(WCH_LockData,0));

				crRoom *room = callback->findRoom(playerData->getRoomID());
				if(room)
				{
					crRoomPlayer *member = room->getMember(playerid);
					if(member)
					{
						//unsigned char success = 0;
						char chatmode = m_stream->_readChar();
						std::string chat_text = m_stream->_readString();
						ref_ptr<crTableIO> invalidTextTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJInvalidTextTab);
						if (invalidTextTab.valid())
						{//广告词屏蔽
							int count = invalidTextTab->getRowCount();
							for (int i = 0; i < count; ++i)
							{
								if (chat_text.find((*invalidTextTab)(i,0)) == 0)
								{
									return;
								}
							}
						}
						std::string sendername = playerData->getCharacterName();

						ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
						streamBuf->createBuf(chat_text.length()+sendername.length()+9);
						streamBuf->_writeChar(chatmode);
						streamBuf->_writeString(sendername);
						streamBuf->_writeString(chat_text);

						crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
						if(chatmode == ChatInTeam)
						{//gmOrder
							bool igm = false;
							//if(!crGlobalHandle::gData()->gEnableRobotTest())
							//{
								//crTableIO::StrVec record;
								//CRCore::ref_ptr<CREncapsulation::crTableIO> gmTab = crGlobalHandle::gData()->gGMTable();
								//if(gmTab.valid() && gmTab->queryOneRecord(0,crArgumentParser::appItoa(playerid),record)>=0)
								//	igm = true;
							short quanxian = crGlobalHandle::gData()->getGMAuthority(playerid);
							if(quanxian>0 && quanxian<JXJGMPermission_newer)
								igm = true;
							//}
							if(igm)
							{
								if(chat_text.find("/jxjgod")==0)
								{//战场无敌
									crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
									crData *roledata;
									for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
										itr != roleMap.end();
										++itr )
									{
										roledata = itr->second->getDataClass();
										int attack = 30000;
										roledata->inputParam(WCHDATA_Attack,&attack);
										short dodge = 1000;
										roledata->inputParam(WCHDATA_Dodge,&dodge);
									}
									std::string logdata = "(" + m_this->getCharacterName() + ")" +  "输入了GM指令：" + chat_text;
									GameLogData gamelog(Log_GMOrder,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
								}
								else if(chat_text.find("/jxjpassbattle")==0)
								{
									crData *roomdata = room->getDataClass();
									short progress = 2;
									roomdata->inputParam(WCHDATA_RoomProgress,&progress);

									std::string logdata = "(" + m_this->getCharacterName() + ")" +  "输入了GM指令：" + chat_text;
									GameLogData gamelog(Log_GMOrder,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
								}
								else if(chat_text.find("/jxjsetprogress")==0)
								{
									std::string logdata = "(" + m_this->getCharacterName() + ")" +  "输入了GM指令：" + chat_text;

									std::string keystr = "/jxjsetprogress";
									int keysize = keystr.size();
									chat_text.erase(0,keysize);
									short progress = atoi(chat_text.c_str());
									crData *roomdata = room->getDataClass();
									roomdata->inputParam(WCHDATA_RoomProgress,&progress);

									GameLogData gamelog(Log_GMOrder,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
								}
								else if(chat_text.find("/jxjsceneserverthrow")==0)
								{
									std::string logdata = "(" + m_this->getCharacterName() + ")" +  "输入了GM指令：" + chat_text;
									GameLogData gamelog(Log_GMOrder,logdata);
									crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));

									//_asm   int   3   //只是为了让程序崩溃
									__debugbreak();
								}
								else if(chat_text.find("/jxjqueryprogress")==0)
								{
									void *param;
									crData *roomdata = room->getDataClass();
									roomdata->getParam(WCHDATA_RoomProgress,param);
									short progress = *(short*)param;
									crPlayerDataEventPacket packet2;
									ref_ptr<crStreamBuf> streamBuf2 = new crStreamBuf;
									streamBuf2->createBuf(3);
									packet2.setStreamBuf(streamBuf2.get());
									streamBuf2->_writeUChar(42);
									streamBuf2->_writeShort(progress);
									crPlayerDataEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvGMOrder,streamBuf2.get());
									sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet2);
								}
								else
								{
									//success = 1;
									ref_ptr<crPlayerDataSceneEventPacket> packet = new crPlayerDataSceneEventPacket;
									crPlayerDataSceneEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvRoomChatMessage,streamBuf.get());
									room->sendRoomMessage(packet.get(),member->getGroupID(),playerid);
								}
							}
							else
							{
								if(chat_text.find("/jxj")==0)
									return;
								//success = 1;
								ref_ptr<crPlayerDataSceneEventPacket> packet = new crPlayerDataSceneEventPacket;
								crPlayerDataSceneEventPacket::buildReplyPacket(*packet,0,WCH_JXJRecvRoomChatMessage,streamBuf.get());
								room->sendRoomMessage(packet.get(),member->getGroupID(),playerid);
							}
						}
						else if (chatmode == ChatOneToOne )
						{
							if (chat_text.find("/jxjping") == 0)
							{
								CRCore::ref_ptr<crStreamBuf > streamBuf = new crStreamBuf;
								chat_text = chat_text + " sceneserver ";

								streamBuf->createBuf(4 + chat_text.size());
								streamBuf->_writeString(chat_text);

								crPlayerDataSceneEventPacket packet2;
								crPlayerDataSceneEventPacket::buildReplyPacket(packet2,playerid,WCH_JXJRecvPingMsg,streamBuf.get());
								sceneServerConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet2);
							}

							// 回给客户端
							//crPlayerDataEventPacket packet2;
							//crPlayerDataEventPacket::buildReplyPacket(packet2,playerData->getPlayerID(),WCH_JXJRecvPingMsg,NULL);
						}
// 						else if(chatmode == ChatInRoomOneToOne)
// 						{
// 							crPlayerDataSceneEventPacket packet;
// 							crPlayerDataSceneEventPacket::buildReplyPacket(packet,0,WCH_JXJRecvRoomChatMessage,streamBuf.get());
// 							std::string reciverName = m_stream->_readString();
// 							crRoom::PlayerList &playerList = room->getPlayerList();
// 							crRoomPlayer *roomPlayer;
// 							int recvPlayerid = 0;
// 							for( crRoom::PlayerList::iterator itr = playerList.begin();
// 								itr != playerList.end();
// 								++itr )
// 							{
// 								roomPlayer = itr->get();
// 								if(reciverName.compare(roomPlayer->getCharacterName())==0)
// 								{
// 									recvPlayerid = roomPlayer->getPlayerID();
// 									break;
// 								}
// 							}
// 							if(recvPlayerid!=0 && recvPlayerid!=m_this->getPlayerID())
// 							{
// 								//success = 1;
// 								ref_ptr<crSceneServerPlayerData> recvPlayerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
// 								if(recvPlayerData.valid())
// 									sceneServerConductor->getNetManager()->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
// 							}
// 							else
// 							{//发送失败
// 								//ref_ptr<crStreamBuf> stream = new crStreamBuf;
// 								//stream->createBuf(1);
// 								//stream->_writeUChar(success);
// 								crPlayerDataSceneEventPacket packet2;
// 								crPlayerDataSceneEventPacket::buildReplyPacket(packet2,m_this->getPlayerID(),WCH_JXJRecvGameChat,NULL);
// 								sceneServerConductor->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet2);
// 							}
// 						}
						//if (firstshow < 0)
						//{
						//	//给自己发包
						//	//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
						//}
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crMyPlayerData::getInstance()->getPlayerGameData()->doEvent(WCH_JXJRecvGameChat,MAKECREPARAM(m_stream.get(),NULL));
		}
	}
}
/////////////////////////////////////////
//
//crJXJGetMailItemMethod
//
/////////////////////////////////////////
crJXJGetMailItemMethod::crJXJGetMailItemMethod()
{
}

crJXJGetMailItemMethod::crJXJGetMailItemMethod(const crJXJGetMailItemMethod& handle):
	crMethod(handle)
{
}

void crJXJGetMailItemMethod::inputParam(int i, void *param)
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
		m_this = (crHypertextWidgetNode*)param;
		break;
	case 2:
		if(param)
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

void crJXJGetMailItemMethod::addParam(int i, const std::string& str)
{
}

void crJXJGetMailItemMethod::operator()(crHandle &handle)
{
	if(m_ea && m_this)
	{
		void *param;
		//获取data
		crData *data = m_this->getDataClass();
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		if(data)
		{
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJGiftsPackKey,param);
			std::string giftskey = *(string *)param;
			//data->getParam(WCHDATA_JXJGiftsPackItemInfo,param);
			//RewardItemVec *rewardItemVec = (RewardItemVec *)param;

			crData *brainData = crBrain::getInstance()->getDataClass();
			brainData->getParam(WCHDATA_JXJRecvMailVec,param);
			JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
			brainData->getParam(WCHDATA_JXJCheckMailIndex,param);
			unsigned char mailindex = *(unsigned char *)param;
			if(mailindex<mailvec->size())
			{
				int mailID  = (*mailvec)[mailindex].get()->getID();
				ref_ptr<crStreamBuf> streamBuf = new crStreamBuf;
				streamBuf->createBuf(giftskey.length()+8);
				streamBuf->_writeString(giftskey);
				streamBuf->_writeInt(mailID);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvMailReward,streamBuf.get());
				netConductor->getNetManager()->sendPacket("all",packet);
			}

			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}

	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJChatInputRecordReshowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatInputRecordReshowMethod::crJXJChatInputRecordReshowMethod():
	m_this(NULL),
	m_input(""),
	m_step(0),
	m_index(0)
{

}
crJXJChatInputRecordReshowMethod::crJXJChatInputRecordReshowMethod(const crJXJChatInputRecordReshowMethod& handle):
	crMethod(handle),
	m_this(handle.m_this),
	m_input(handle.m_input),
	m_step(handle.m_step)
{

}
void crJXJChatInputRecordReshowMethod::inputParam(int i, void *param)
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
void crJXJChatInputRecordReshowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	case 1:
		m_step = atoi(str.c_str());
		break;
	}
}
void crJXJChatInputRecordReshowMethod::operator ()(crHandle &handle)
{
	if(!m_this) return;
	crData * canvasData = m_this->getDataClass();
	if(!canvasData) return;
	void *param;
	canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
	canvasData->getParam(WCHDATA_JXJChatInputRecordDeque,param);
	ChatInputRecordDeque inputRecordDeq = *(ChatInputRecordDeque *)param;
	if(inputRecordDeq.size() == 0)
	{
		canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
		return;
	}
	m_index=m_index+m_step;
	if(m_index>=inputRecordDeq.size())
		m_index = inputRecordDeq.size()-1;
	else if(m_index<0)
		m_index = 0;
	std::string text = inputRecordDeq[m_index];
	canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));

	ref_ptr<crEditWidgetNode>  input = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_input));
	if(!input || text.empty()) return;
	input->setString(text);
	input->setFocusPos(input->getUTF8String().size());
}

/////////////////////////////////////////
//
//crJXJRecvMailRewardMethod
//
/////////////////////////////////////////
crJXJRecvMailRewardMethod::crJXJRecvMailRewardMethod():
	m_netType(GameClient_Game)
{
}
crJXJRecvMailRewardMethod::crJXJRecvMailRewardMethod(const crJXJRecvMailRewardMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvMailRewardMethod::inputParam(int i, void *param)
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
void crJXJRecvMailRewardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_mailCanvas = str;
		break;
	}
}
void crJXJRecvMailRewardMethod::operator()(crHandle &handle)
{	
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameServer)
		{
			int playerid = m_this->getPlayerID();
			std::string keyWord = m_stream->_readString();
			int mailID = m_stream->_readInt();
			// make query 
			void *param;
			crData *server_data = crServerBrainHandle::getInstance()->getDataClass();
			server_data->excHandle(MAKECREPARAM(WCH_LockData,1));
			server_data->getParam(WCHDATA_JXJQueryGiftsPackDeque, param);
			QueryGiftsPackDeque *query_deque = (QueryGiftsPackDeque *)param;
			// 查询
			bool found = false;
			for(QueryGiftsPackDeque::iterator itr = query_deque->begin(); itr != query_deque->end(); ++itr)
			{
				if(itr->get()->getPlayerID() == playerid && itr->get()->getKey() == keyWord)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				//根据key查询数据库
				CRCore::ref_ptr<crQueryGiftsPack> querypacket = new crQueryGiftsPack(playerid,keyWord);
				// input to queue
				querypacket->setMailID(mailID);
				query_deque->push_back(querypacket);
			}
			server_data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}	
		else if(m_netType = GameClient_Game)
		{
			void *param;
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			std::string resultmessage;
			int _case = m_stream->_readChar();
			if(_case == -1)
			{
				resultmessage = std::string(TEXT("物品背包已满，\n请先整理背包后再重试！"));
			}
			else if(_case == -2)
			{
				resultmessage = std::string(TEXT("将卡背包已满，\n请先整理背包后再重试！"));
			}
			else if(_case == -3)
			{
				resultmessage = std::string(TEXT("领取失败，\n请稍后再重试！"));
			}
			else if(_case == 1)
			{
				resultmessage = std::string(TEXT("恭喜您奖励领取成功！"));
				crData *brainData = crBrain::getInstance()->getDataClass();
				brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
				brainData->getParam(WCHDATA_JXJRecvMailVec,param);
				JXJRecvMailVec *mailvec = (JXJRecvMailVec *)param;
				brainData->getParam(WCHDATA_JXJCheckMailIndex,param);
				unsigned char mailindex = *(unsigned char *)param;
				if(mailindex<mailvec->size())
					(*mailvec)[mailindex].get()->setState(Mail_BeenGetted);
				brainData->excHandle(MAKECREPARAM(WCH_LockData,0));

				ref_ptr<crCanvasNode>  mailCanvas = crFilterRenderManager::getInstance()->findCanvas(m_mailCanvas);
				if(mailCanvas.valid())
					mailCanvas->doEvent(WCH_UI_UpdateData);
			}
			else if(_case == 2)
			{
				resultmessage = std::string(TEXT("奖励已被领取，不能重复领取！"));
			}
			else
			{
				resultmessage = std::string(TEXT("奖励领取失败，请稍后再试！"));
			}
			if(canvas.valid())
			{
				ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input));
				if(input.valid())
					input->setString(resultmessage);
				crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJMouseDragCanvasMoveMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJMouseDragCanvasMoveMethod::crJXJMouseDragCanvasMoveMethod():
	m_this(NULL)
{

}
crJXJMouseDragCanvasMoveMethod::crJXJMouseDragCanvasMoveMethod(const crJXJMouseDragCanvasMoveMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_minimapcanvas(handle.m_minimapcanvas),
	m_frame(handle.m_frame),
	m_imagebox(handle.m_imagebox)
{

}
void crJXJMouseDragCanvasMoveMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	case 2:
		if(param)
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
void crJXJMouseDragCanvasMoveMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_minimapcanvas = str;
		break;
	case 1:
		m_frame = str;
		break;
	case 2:
		m_imagebox = str;
		break;
	case 3:
		m_strBigImg = str;
		break;
	case 4:
		m_strminimapcanvas2 = str;
		break;
	case 5:
		m_strimagebox2 = str;
		break;
	}
}
void crJXJMouseDragCanvasMoveMethod::operator ()(crHandle &handle)
{
	if(m_ea && m_this)
	{
		void *param;
		crData *data = m_this->getDataClass();
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		if(data)
		{
			data->getParam(WCHDATA_JXJCityMapCanDrag,param);
			bool isCanDrag = *(bool *)param;
			if(isCanDrag)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJCityMapMouseCenterOffsetPos,param);
				crVector2 mousePos = *(crVector2 *)param;
				data->getParam(WCHDATA_JXJCityMapOffsetMax,param);
				crVector2 maxoffset = *(crVector2 *)param;
				crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
				offsetX = mouse[0]-mousePos[0];
				offsetY = mouse[1]-mousePos[1];

				if(offsetX<-maxoffset[0])
					offsetX = -maxoffset[0];
				else if(offsetX>maxoffset[0])
					offsetX = maxoffset[0];
				if(offsetY<-maxoffset[1])
					offsetY = -maxoffset[1];
				else if(offsetY>maxoffset[1])
					offsetY = maxoffset[1];

				data->getParam(WCHDATA_JXJCityMapFrameScale,param);
				crVector3 scale = *(crVector3 *)param;
				data->getParam(WCHDATA_JXJCityMapFrameScale2,param);
				crVector3 scale2 = *(crVector3 *)param;
				data->excHandle(MAKECREPARAM(WCH_LockData,0));

				ref_ptr<crCanvasNode>  miniMap = crFilterRenderManager::getInstance()->findCanvas(m_minimapcanvas);
				ref_ptr<crCanvasNode>  miniMap2 = crFilterRenderManager::getInstance()->findCanvas(m_strminimapcanvas2);
				if(!miniMap.valid() || !miniMap2.valid())return;
				ref_ptr<crWidgetNode>  viewPort = miniMap->getWidget(m_frame);
				ref_ptr<crWidgetNode>  viewPort2 = miniMap2->getWidget(m_frame);
				ref_ptr<crImageBoxWidgetNode>  imageBox = dynamic_cast<crImageBoxWidgetNode *>(miniMap->getWidget(m_imagebox));
				ref_ptr<crImageBoxWidgetNode>  imageBox2 = dynamic_cast<crImageBoxWidgetNode *>(miniMap2->getWidget(m_strimagebox2));
				ref_ptr<crWidgetNode>  bigImageBox = dynamic_cast<crWidgetNode *>(m_this->getWidget(m_strBigImg));
				if(!viewPort.valid() || !imageBox.valid() || !bigImageBox.valid() || !viewPort2.valid() || !imageBox2.valid()) return;
				const crBoundingBox& imageBBox = imageBox->getBoundBox();
				const crBoundingBox& imageBBox2 = imageBox2->getBoundBox();
				const crBoundingBox& bbox = bigImageBox->getBoundBox();
				crVector2 scaleVec(imageBBox.xLength()/bbox.xLength(),imageBBox.yLength()/bbox.yLength());
				viewPort->setMatrix(crMatrix::scale(scale)*crMatrix::translate(-offsetX*scaleVec[0],-offsetY*scaleVec[1],0.0f));

				crVector2 scaleVec2(imageBBox2.xLength()/bbox.xLength(),imageBBox2.yLength()/bbox.yLength());
				viewPort2->setMatrix(crMatrix::scale(scale2)*crMatrix::translate(-offsetX*scaleVec2[0],-offsetY*scaleVec2[1],0.0f));

				m_this->setMatrix(crMatrix::translate(offsetX,offsetY,0.0f));
				crVector2 curOffset(offsetX,offsetY);
				data->inputParam(WCHDATA_JXJCityCenterOffset,&curOffset);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJCityMapMousePushPosMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCityMapMousePushPosMethod::crJXJCityMapMousePushPosMethod():
	m_this(NULL)
{

}
crJXJCityMapMousePushPosMethod::crJXJCityMapMousePushPosMethod(const crJXJCityMapMousePushPosMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJCityMapMousePushPosMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	case 2:
		if(param)
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
void crJXJCityMapMousePushPosMethod::addParam(int i, const std::string& str)
{

}
void crJXJCityMapMousePushPosMethod::operator ()(crHandle &handle)
{
	if(m_ea && m_this)
	{
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		crData *data = m_this->getDataClass();
		if(data)
		{
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			crVector3 center = m_this->getBoundBox().center();
			crVector2 offset(mouse[0]-center[0],mouse[1]-center[1]);
			data->inputParam(WCHDATA_JXJCityMapMouseCenterOffsetPos,&offset);
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//crJXJCityMapCanDragMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCityMapCanDragMethod::crJXJCityMapCanDragMethod():
	m_this(NULL),
	m_canBe(false)
{

}
crJXJCityMapCanDragMethod::crJXJCityMapCanDragMethod(const crJXJCityMapCanDragMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_canBe(handle.m_canBe)
{

}
void crJXJCityMapCanDragMethod::inputParam(int i, void *param)
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
void crJXJCityMapCanDragMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canBe = atoi(str.c_str());
	default:
		break;
	}
}
void crJXJCityMapCanDragMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		crData *data = m_this->getDataClass();
		if(data)
		{
			data->inputParam(WCHDATA_JXJCityMapCanDrag,&m_canBe);
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJUICityMiniMapInitMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUICityMiniMapInitMethod::crJXJUICityMiniMapInitMethod():
	m_this(NULL),
	m_init(false)
{

}
crJXJUICityMiniMapInitMethod::crJXJUICityMiniMapInitMethod(const crJXJUICityMiniMapInitMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_miniMap(handle.m_miniMap),
	m_init(false)
{

}
void crJXJUICityMiniMapInitMethod::inputParam(int i, void *param)
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
void crJXJUICityMiniMapInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_miniMap = str;
		break;
	case 1:
		m_wei = str;
		break;
	case 2:
		m_shu = str;
		break;
	case 3:
		m_wu = str;
		break;
	case 4:
		m_npc = str;
		break;
	case 5:
		m_frame = str;
		break;
	case 6:
		m_imagebox = str;
		break;
	case 7:
		m_strBigImg = str;
		break;
	case 8:
		m_type = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJUICityMiniMapInitMethod::operator ()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>  miniMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_miniMap);
		ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
		//ref_ptr<crWidgetNode>  bigImgWeidget = m_this->getWidget(m_strBigImg);
		ref_ptr<crWidgetNode>  weiWeidget = mainCanvas->getWidget(m_wei);
		ref_ptr<crWidgetNode>  shuWeidget = mainCanvas->getWidget(m_shu);
		ref_ptr<crWidgetNode>  wuWeidget = mainCanvas->getWidget(m_wu);
		ref_ptr<crWidgetNode>  npcWeidget = mainCanvas->getWidget(m_npc);
		ref_ptr<crWidgetNode>  frameWeidget = mainCanvas->getWidget(m_frame);
		if(!miniMapCanvas.valid() || !weiWeidget.valid() || !shuWeidget.valid() || !wuWeidget.valid()	\
			|| !npcWeidget.valid() ) return;
		ref_ptr<crImageBoxWidgetNode>  imagebox = dynamic_cast<crImageBoxWidgetNode *>(miniMapCanvas->getWidget(m_imagebox));
		if(!imagebox.valid()) return;
		imagebox->setVisiable(false);
		crData *canvasData = m_this->getDataClass();
		if(!canvasData) return;
		crData *data = myPlayer->getDataClass();
		data->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		//ref_ptr<crTableIO> shiliTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJShiliTab);
		int buttonid = chengchiTab->getTitleIndex("buttonname");

		ChengchiMap::iterator itr_city = chengchimap->begin();
		string buttonName;

		ref_ptr<crRadioWidgetNode> cityRadio;
		ref_ptr<crWidgetNode> cityWidget;

		const crBoundingBox& bigmapbox = m_this->getBoundBox();
		const crBoundingBox& minimapbox = miniMapCanvas->getBoundBox();
		const crBoundingBox& minimapImageboxBox = imagebox->getBoundBox();
		crVector3 scale(minimapImageboxBox.xLength()/bigmapbox.xLength(),minimapImageboxBox.yLength()/bigmapbox.yLength(),1.0f);
		float xlength = bigmapbox.xLength()*0.5-1.0f;
		float ylength = bigmapbox.yLength()*0.5-1.0f;
		crVector3 minimapCenter = minimapImageboxBox.center();
		crVector2 maxoffset(xlength,ylength);
		const crBoundingBox& frameBox = frameWeidget->getBoundBox();
		crVector3 framescale(2.0f*minimapImageboxBox.xLength()/(bigmapbox.xLength()*frameBox.xLength()),2.0f*minimapImageboxBox.yLength()/(bigmapbox.yLength()*frameBox.yLength()),1.0f);
		frameWeidget->setVisiable(false);
		if(!m_init)
		{
			unsigned char chengzhu;
			for(;itr_city!=chengchimap->end();itr_city++)
			{
				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(itr_city->first),record)>=0)
				{
					chengzhu = itr_city->second->getChengzhuShili();
					buttonName = record[buttonid];
					if(buttonName.empty()) continue;
					cityRadio = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(buttonName));
					if(!cityRadio.valid()) continue;
					const crBoundingBox& bbox = cityRadio->getBoundBox();
					crVector3 radioPos = bbox.center();
					if(chengzhu == 10)
					{
						cityWidget = dynamic_cast<crWidgetNode*>(weiWeidget->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
					}
					else if(chengzhu == 11)
					{
						cityWidget = dynamic_cast<crWidgetNode*>(shuWeidget->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
					}
					else if(chengzhu == 12)
					{
						cityWidget = dynamic_cast<crWidgetNode*>(wuWeidget->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
					}
					else if(chengzhu == 2)
					{
						cityWidget = dynamic_cast<crWidgetNode*>(npcWeidget->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
					}

					//miniMapCanvas->addChild(cityWidget.get());
					crLoadManager::getInstance()->requestAddNode(miniMapCanvas.get(),cityWidget.get(),false);
					cityWidget->setVisiable(true);
					cityWidget->setMatrix(crMatrix::translate(radioPos[0]*scale[0]+minimapCenter[0],radioPos[1]*scale[1]+minimapCenter[1],0.0f));
				}
			}
			ref_ptr<crWidgetNode>  copyframeWeidget = dynamic_cast<crWidgetNode*>(frameWeidget->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
			copyframeWeidget->setMatrix(crMatrix::scale(framescale));
			//miniMapCanvas->addChild(copyframeWeidget.get());
			crLoadManager::getInstance()->requestAddNode(miniMapCanvas.get(),copyframeWeidget.get(),false);
			copyframeWeidget->setVisiable(true);
			if(m_type == 1)
			{
				canvasData->inputParam(WCHDATA_JXJMiniMapFrameWidget,copyframeWeidget.get());
			}
			m_init = true;
		}

		miniMapCanvas->setMatrix(crMatrix::translate(-1.0+minimapbox.xLength()*0.5,-1.0+minimapbox.yLength()*0.5,0.0f));
		if(m_type==1)
		{
			canvasData->inputParam(WCHDATA_JXJCityMapFrameScale,&framescale);
		}
		else if(m_type == 0)
		{
			canvasData->inputParam(WCHDATA_JXJCityMapFrameScale2,&framescale);
		}
		canvasData->inputParam(WCHDATA_JXJCityMapOffsetMax,&maxoffset);
	}
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJUICityMiniMapZoomMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUICityMiniMapZoomMethod::crJXJUICityMiniMapZoomMethod():
	m_this(NULL)
{

}
crJXJUICityMiniMapZoomMethod::crJXJUICityMiniMapZoomMethod(const crJXJUICityMiniMapZoomMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJUICityMiniMapZoomMethod::inputParam(int i, void *param)
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
void crJXJUICityMiniMapZoomMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strBigMapCanvas = str;
		break;
	case 1:
		m_strBigMapImage = str;
		break;
	case 2:
		m_strOtherMapCanvas = str;
		break;
	case 3:
		m_strOtherMapImage = str;
		break;
	case 4:
		m_strFrame = str;
		break;
	default:
		break;
	}
}
void crJXJUICityMiniMapZoomMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>  miniMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strOtherMapCanvas);
	ref_ptr<crCanvasNode>  bigMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strBigMapCanvas);
	if(!miniMapCanvas.valid() || !bigMapCanvas.valid()) return;
	crData *canvasData = bigMapCanvas->getDataClass();
	if(!canvasData) return;
	ref_ptr<crImageBoxWidgetNode> miniimagebox = dynamic_cast<crImageBoxWidgetNode *>(miniMapCanvas->getWidget(m_strOtherMapImage));
	//ref_ptr<crImageBoxWidgetNode> bigimagebox = dynamic_cast<crImageBoxWidgetNode *>(bigMapCanvas->getWidget(m_strBigMapImage));
	ref_ptr<crWidgetNode>  frameWeidget = miniMapCanvas->getWidget(m_strFrame);
	if(!miniimagebox.valid() || !frameWeidget.valid()) return;

	const crBoundingBox& bigmapbox = bigMapCanvas->getBoundBox();
	const crBoundingBox& minimapImageboxBox = miniimagebox->getBoundBox();

	//清除原有缩放矩阵
	frameWeidget->setMatrix(crMatrix::scale(crVector3(1.0f,1.0f,1.0f)));
	//////////////////////////////////////////////////////////////////////////
	const crBoundingBox& frameBox = frameWeidget->getBoundBox();
	crVector3 framescale(2.0f*minimapImageboxBox.xLength()/(bigmapbox.xLength()*frameBox.xLength()),2.0f*minimapImageboxBox.yLength()/(bigmapbox.yLength()*frameBox.yLength()),1.0f);

	//定位frame的位置
	canvasData->getParam(WCHDATA_JXJCityCenterOffset,param);
	crVector2 & curOffset = *(crVector2 *)param;
	crVector2 scaleVec(minimapImageboxBox.xLength()/bigmapbox.xLength(),minimapImageboxBox.yLength()/bigmapbox.yLength());
	crVector3 frameOffset(-curOffset.x()*scaleVec.x(),-curOffset.y()*scaleVec.y(),0.0f);

	frameWeidget->setMatrix(crMatrix::scale(framescale) * crMatrix::translate(frameOffset));

	canvasData->inputParam(WCHDATA_JXJCityMapFrameScale,&framescale);
}
///////////////////////////////////////////////////////////////////////////
//
//crJXJMouseDragMinimapFrameMoveMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJMouseDragMinimapFrameMoveMethod::crJXJMouseDragMinimapFrameMoveMethod():
	m_this(NULL)
{

}
crJXJMouseDragMinimapFrameMoveMethod::crJXJMouseDragMinimapFrameMoveMethod(const crJXJMouseDragMinimapFrameMoveMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_frame(handle.m_frame),
	m_bigmap(handle.m_bigmap)
{

}
void crJXJMouseDragMinimapFrameMoveMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_this = (crCanvasNode*)param;
		break;
	case 2:
		if(param)
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
void crJXJMouseDragMinimapFrameMoveMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_frame = str;
		break;
	case 1:
		m_imageBox = str;
		break;
	case 2:
		m_bigmap = str;
		break;
	case 3:
		m_strBigMapImg = str;
		break;
	case 4:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJMouseDragMinimapFrameMoveMethod::operator ()(crHandle &handle)
{
	if(m_ea && m_this)
	{
		void *param;
		ref_ptr<crCanvasNode>  bigMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_bigmap);
		if(!bigMapCanvas)return;
		crData *canvasData = bigMapCanvas->getDataClass();
		if(!canvasData) return;
		ref_ptr<crWidgetNode>  frameWidget = m_this->getWidget(m_frame);
		ref_ptr<crImageBoxWidgetNode>  imageBox = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_imageBox));
		//ref_ptr<crImageBoxWidgetNode>  bigMapImageBox = dynamic_cast<crImageBoxWidgetNode *>(bigMapCanvas->getWidget(m_strBigMapImg));
		if(!frameWidget.valid() || !imageBox.valid()) return;
		crVector2 mouse(m_ea->getXnormalized(),m_ea->getYnormalized());
		m_this->mouseTrans(mouse[0],mouse[1]);
		
		canvasData->getParam(WCHDATA_JXJCityMapFrameScale,param);
		crVector3 scale = *(crVector3 *)param;
		canvasData->getParam(WCHDATA_JXJCityMapOffsetMax,param);
		crVector2 maxoffset = *(crVector2 *)param;		

		const crBoundingBox& framebox = frameWidget->getBoundBox();
		const crBoundingBox& bigmapbox = bigMapCanvas->getBoundBox();
		const crBoundingBox& imagebbox = imageBox->getBoundBox();
		crVector2 scaleVec(bigmapbox.xLength()/imagebbox.xLength(),bigmapbox.yLength()/imagebbox.yLength());
		float xlength = framebox.xLength()*0.5f*scale[0];
		float ylength = framebox.yLength()*0.5f*scale[1];
		float offsetx = 0.0f;
		float offsety = 0.0f;

		crVector2 framemaxoffset(maxoffset[0]/scaleVec[0],maxoffset[1]/scaleVec[1]);
		if(mouse[0] < -framemaxoffset[0])
			offsetx = -framemaxoffset[0];
		else if(mouse[0]>framemaxoffset[0])
			offsetx = framemaxoffset[0];
		else 
			offsetx = mouse[0];

		if(mouse[1]<-framemaxoffset[1])
			offsety = -framemaxoffset[1];
		else if(mouse[1]>framemaxoffset[1])
			offsety = framemaxoffset[1];
		else offsety = mouse[1];
		crVector3 minimapCenter = imagebbox.center();
		frameWidget->setMatrix(crMatrix::scale(scale)*crMatrix::translate(offsetx+minimapCenter[0],offsety+minimapCenter[1],0.0f));	//
		bigMapCanvas->setMatrix(crMatrix::translate(-offsetx*scaleVec[0],-offsety*scaleVec[1],0.0f));

		crVector2 curOffset(-offsetx*scaleVec[0],-offsety*scaleVec[1]);
		canvasData->inputParam(WCHDATA_JXJCityCenterOffset,&curOffset);
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJPassMainUpdateMethod
//////////////////////////////////////////////////////////////////////////
crJXJPassMainUpdateMethod::crJXJPassMainUpdateMethod()
{
}
crJXJPassMainUpdateMethod::crJXJPassMainUpdateMethod(const crJXJPassMainUpdateMethod& handle)
{

}
void crJXJPassMainUpdateMethod::inputParam(int i, void *param)
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
		m_this = (CRNetApp::crRoom *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJPassMainUpdateMethod::addParam(int i, const std::string& str)
{

}
void crJXJPassMainUpdateMethod::operator()(crHandle &handle)
{
	/*crData *roomData =m_this->getDataClass();
	roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
	void *param;
	roomData->getParam(WCHDATA_JXJCheckPointVec, param);
	PassCheckVec *check_points = (PassCheckVec *)param;
	roomData->getParam(WCHDATA_JXJBattleID,param);
	int battle_id = *(int *)param;
	roomData->getParam(WCHDATA_JXJRoomDataFlag, param);
	bool *is_modifed = (bool *) param;
	unsigned char deffence = (unsigned char)(LOINT16(LOINT32(battle_id)));
	CREncapsulation::crTableIO::StrVec	record;
	if(check_points->empty()) return;
	int check_span = m_checks->getTitleIndex("兵力差异");
	int check_speed = m_checks->getTitleIndex("占领速度");

	m_this->lockPlayerList();
	crRoom::PlayerList &players = m_this->getPlayerList();

	// check every check point
	for(PassCheckVec::iterator sitr = check_points->begin(); sitr != check_points->end(); sitr++)
	{
	int red_arms = 0;
	int blue_arms = 0;
	//检测是否为关隘
	//if(sitr->type == 1) continue;

	// check evry player
	for(crRoom::PlayerList::iterator pitr = players.begin(); pitr != players.end(); pitr++)
	{
	// check every arms
	ref_ptr<crSceneServerPlayerData> player_data = dynamic_cast<crSceneServerPlayerData *>
	(crNetContainer::getInstance()->getNetConductor(SceneServer)->getNetDataManager()->getPlayerData(pitr->get()->getPlayerID()));
	if (player_data.valid() && player_data->getPlayerGameData() && player_data->getPlayerGameData()->getDataClass())
	{ 
	player_data->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJShiliID, param);
	unsigned char side_id = *(int *)param;
	crSceneServerPlayerData::RoleMap &arms = player_data->getRoleMap();

	for(crSceneServerPlayerData::RoleMap::iterator ritr = arms.begin(); ritr != arms.end(); ritr++)
	{
	// calculations
	crData*	data = ritr->second->getDataClass();
	data->getParam(WCHDATA_JXJRTArmyCount,param);
	int armyRTCount = *(unsigned short *)param;
	crVector3f postion = ritr->second->getPosition();
	float dis_sq = (postion[0] - sitr->get()->getPosX()) * (postion[0] - sitr->get()->getPosX()) + (postion[1] - sitr->get()->getPosY()) * (postion[1] - sitr->get()->getPosY()); // 距离的平方
	if(dis_sq <= sitr->get()->getArea() * sitr->get()->getArea())
	{
	red_arms += (side_id == deffence ? armyRTCount : 0);
	blue_arms += (side_id == deffence ? 0 : armyRTCount);
	}
	}
	}
	}

	// take position
	short check_limit = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCheckPointLimit).c_str());

	int dif = red_arms - blue_arms;
	dif = (abs(dif)<10)?10:(abs(dif)/10)*10;
	if(dif>m_checks->getRowCount()*10)
	dif = m_checks->getRowCount()*10;

	if(red_arms == blue_arms)
	{
	continue;
	}

	if(red_arms > blue_arms && sitr->get()->getScore() <= -check_limit)
	{
	sitr->get()->setScore(-check_limit);
	*is_modifed = true;
	continue;
	}

	if(red_arms < blue_arms && sitr->get()->getScore() >= check_limit)
	{
	*is_modifed = true;
	sitr->get()->setScore(check_limit);
	continue;
	}

	CREncapsulation::crTableIO::StrVec	record;
	if(m_checks->queryOneRecord(check_span, crArgumentParser::appItoa(dif), record))
	{
	*is_modifed = true;
	int dir = (red_arms>=blue_arms)?1:-1;
	short score = sitr->get()->getScore();
	score -= dir*atoi(record[check_speed].c_str());	
	sitr->get()->setScore(score);
	}
	}
	m_this->unlockPlayerList();
	roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
	*/
}
////////////////////////////////////////////////////////////////////////////
////crJXJISFuBenInviteMethod
////////////////////////////////////////////////////////////////////////////
//crJXJISFuBenInviteMethod::crJXJISFuBenInviteMethod()
//{
//}
//crJXJISFuBenInviteMethod::crJXJISFuBenInviteMethod(const crJXJISFuBenInviteMethod& handle)
//{
//
//}
//void crJXJISFuBenInviteMethod::inputParam(int i, void *param)
//{
//
//}
//void crJXJISFuBenInviteMethod::addParam(int i, const std::string& str)
//{
//
//}
//void crJXJISFuBenInviteMethod::operator()(crHandle &handle)
//{
//	void *param;
//	int _case = 0;
//	crData *data = crBrain::getInstance()->getDataClass();
//	if(data)
//	{
//		data->getParam(WCHDATA_JXJCurInviteType,param);
//		unsigned char inviteType = *(unsigned char *)param;
//		_case = inviteType;
//	}
//	handle.outputParam(0,&_case);
//}

//////////////////////////////////////////////////////////////////////////
//crJXJRecvInviteTypeMethod
//////////////////////////////////////////////////////////////////////////
crJXJRecvInviteTypeMethod::crJXJRecvInviteTypeMethod()
{
}
crJXJRecvInviteTypeMethod::crJXJRecvInviteTypeMethod(const crJXJRecvInviteTypeMethod& handle)
{

}
void crJXJRecvInviteTypeMethod::inputParam(int i, void *param)
{

}
void crJXJRecvInviteTypeMethod::addParam(int i, const std::string& str)
{

}
void crJXJRecvInviteTypeMethod::operator()(crHandle &handle)
{
	void *param;
	int _case = 0;
	crData *data = crMyPlayerData::getInstance()->getPlayerGameData()->getDataClass();
	if(data && crRunGameHandle::getInstance()->isInManor())
	{
		data->getParam(WCHDATA_JXJInviteType,param);
		unsigned char inviteType = *(unsigned char *)param;
		_case = inviteType;
	}
	handle.outputParam(0,&_case);
}

/////////////////////////////////////////
//
//crJXJHidePreUseSkillRangeDecalMethod
//
/////////////////////////////////////////
crJXJHidePreUseSkillRangeDecalMethod::crJXJHidePreUseSkillRangeDecalMethod(){}
crJXJHidePreUseSkillRangeDecalMethod::crJXJHidePreUseSkillRangeDecalMethod(const crJXJHidePreUseSkillRangeDecalMethod& handle)
{
}
void crJXJHidePreUseSkillRangeDecalMethod::inputParam(int i, void *param)
{
}

void crJXJHidePreUseSkillRangeDecalMethod::addParam(int i, const std::string& str)
{
}

void crJXJHidePreUseSkillRangeDecalMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		void *param;
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		if(!cameraData) return;
		CRCore::ScopedLock<crData> lock(*cameraData);
		cameraData->getParam(WCHDATA_JXJPreUseSkillRangeDecal,param);
		crNode *decalRangeNode = (crNode *)param;
		cameraData->getParam(WCHDATA_JXJPreUseSkillDecal,param);
		crNode *decalScopeNode = (crNode *)param;
		if(decalRangeNode)
		{
			crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(decalRangeNode);
			if(skillRangeMatrix)
			{
				skillRangeMatrix->setVisiable(false);
			}
		}
		if(decalScopeNode)
		{
			crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(decalScopeNode->getUpdateCallback("DecalUpdate"));
			//crMatrixTransform * skillScopeMatrix = dynamic_cast<crMatrixTransform *>(decalScopeNode);
			if(callback)
			{
				callback->setVisiable(false);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJPreUseSkillRangeDecalCreateLogic
//
/////////////////////////////////////////
crJXJPreUseSkillRangeDecalCreateLogic::crJXJPreUseSkillRangeDecalCreateLogic():
	m_decalNode(NULL),
	m_zoffset(1.0f)
{
}

crJXJPreUseSkillRangeDecalCreateLogic::crJXJPreUseSkillRangeDecalCreateLogic(const crJXJPreUseSkillRangeDecalCreateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL),
	m_zoffset(handle.m_zoffset),
	m_filename(handle.m_filename)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJPreUseSkillRangeDecalCreateLogic::inputParam(int i, void *param)
{
}

void crJXJPreUseSkillRangeDecalCreateLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	case 2:
		m_filename = str;
		break;
	}
}

void crJXJPreUseSkillRangeDecalCreateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJPreUseSkillRangeDecalCreateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}

void crJXJPreUseSkillRangeDecalCreateLogic::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!m_playSceneFx.valid() || !camera.valid()) return;
	void *param;
	ref_ptr<crData> cameraData = camera->getAttachedNode()->getDataClass();
	if(!cameraData.valid()) return;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;

	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(!me) return;
	crData *playerdata = me->getPlayerGameData()->getDataClass();
	if(!playerdata) return;
	crVector3 pos;
	crVector2i equips = crVector2i(-1,-1);
	crTableIO::StrVec record;
	int abstractID = 0;
	ref_ptr<crTableIO> dataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
	int useRangeMatrix = dataAttrTab->getTitleIndex("范围模型");
	int radiusid = dataAttrTab->getTitleIndex("使用半径");

	playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
	playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec *myteam = (FormationInfoVec *)param;
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(!myteam) return;
	ref_ptr<crRole> role = me->getLoadedRoleByIndex(m_index);
	unsigned short range = 0;
	cameraData->getParam(WCHDATA_JXJPreUseSkillRangeDecal,param);
	m_decalNode = (CRCore::crNode *)param;
	if(role.valid())
	{
		crData *roleData = role->getDataClass();
		int roleid = role->getRoleID();
		roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
		roleData->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *((unsigned char*)param);
		if(itemstate != IS_Dead)
		{
			for( FormationInfoVec::iterator itr = myteam->begin();
				itr != myteam->end();
				++itr )
			{
				if(roleid == (*itr)->getRoleID())
				{
					equips = (*itr)->getEquips();
					break;
				}
			}
			if(equips[0]>0)
			{
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),record)>=0)
				{
					abstractID =atoi(record[itemtab->getTitleIndex("abstractid")].c_str());

				}
			}
			if(abstractID!=0 && dataAttrTab.valid())
			{

				if(param )
				{
					if(dataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(abstractID),record)>=0)
					{
						range = atof(record[radiusid].c_str());
						range /= crGlobalHandle::gData()->gUnitScale();
						m_filename = record[useRangeMatrix];
						if(!m_decalNode || m_decalNode->getName().compare(m_filename)!=0)
						{
							m_playSceneFx->inputParam(5,&m_filename);
							(*m_playSceneFx)(*this);
							if(m_decalNode.valid())
								m_decalNode->setName(m_filename);
							cameraData->inputParam(WCHDATA_JXJPreUseSkillRangeDecal,m_decalNode.get());
						}
						cameraData->inputParam(WCHDATA_JXJPreUseSkillRange,&range);
						cameraData->inputParam(WCHDATA_JXJPreUseSkillRole,role.get());
					}
				}
			}
		}
		roleData->excHandle(MAKECREPARAM(WCH_LockData,0));

		if(m_decalNode.valid())
		{
			do 
			{
				if(range>0 && role.valid())
				{
					crMatrixTransform * skillRangeMatrix = dynamic_cast<crMatrixTransform *>(m_decalNode.get());
					if(skillRangeMatrix)
					{
						crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
						if(!bot || !bot->getVisiable())
						{
							skillRangeMatrix->setVisiable(false);
						}
						else
						{
							skillRangeMatrix->setVisiable(true);
							pos = bot->getTrans();
							crVector3f dir = role->getDir();
							CRCore::crMatrix newMat;
							if(dir == crVector3(0.0f,0.0f,0.0f))
							{
								dir[1] = -1.0f;
							}
							if(dir[1] > 0.999)
							{
								newMat.makeRotate(-CRCore::Y_AXIS,dir);
								CRCore::crVector3d ep,center,up;
								newMat.getLookAt(ep,center,up);
								if(center[2]>0.0f)
								{
									center = -center;
								}
								newMat.makeLookAt(ep,center,up);
							}
							else
								newMat.makeRotate(-CRCore::Y_AXIS,dir);
							float fscope = (float)range * crGlobalHandle::gData()->gUnitScale();
							skillRangeMatrix->setMatrix(newMat*crMatrix::scale(fscope/10.0f,fscope/10.0f,1.0f)*crMatrix::translate(crVector3f(pos[0],pos[1],pos[2]+m_zoffset)));
							break;
						}
					}
				}
				m_decalNode->setVisiable(false);
			}
			while(0);
		}
	}
}
void crJXJPreUseSkillRangeDecalCreateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
/////////////////////////////////////////
//
//crJXJPreUseSkillDecalCreateLogic
//
/////////////////////////////////////////
crJXJPreUseSkillDecalCreateLogic::crJXJPreUseSkillDecalCreateLogic():
	m_decalNode(NULL),
	m_zoffset(1.0f)
{
}

crJXJPreUseSkillDecalCreateLogic::crJXJPreUseSkillDecalCreateLogic(const crJXJPreUseSkillDecalCreateLogic& handle):
	crNodeLogic(handle),
	m_decalNode(NULL),
	m_zoffset(handle.m_zoffset),
	m_filename(handle.m_filename)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crJXJPreUseSkillDecalCreateLogic::inputParam(int i, void *param)
{
}

void crJXJPreUseSkillDecalCreateLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_zoffset = atof(str.c_str());
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	case 2:
		m_filename = str;
		break;
	}
}

void crJXJPreUseSkillDecalCreateLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJPreUseSkillDecalCreateLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_decalNode = (crNode *)param;
		break;
	}
}
void crJXJPreUseSkillDecalCreateLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJPreUseSkillDecalCreateLogic::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!m_playSceneFx.valid() || !camera.valid()) return;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	void *param;
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	if(!cameraData)
		return;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(!me) return;
	crData *playerdata = me->getPlayerGameData()->getDataClass();
	if(!playerdata) return;
	crVector3 pos;
	crVector2i equips = crVector2i(-1,-1);
	crTableIO::StrVec record;
	int abstractID = 0;
	ref_ptr<crTableIO> dataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);
	//int useRangeMatrix = dataAttrTab->getTitleIndex("范围模型");
	int scopeid = dataAttrTab->getTitleIndex("作用范围");
	int effectTargetIndex = dataAttrTab->getTitleIndex("作用类型");

	playerdata->excHandle(MAKECREPARAM(WCH_LockData,1));
	playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec *myteam = (FormationInfoVec *)param;
	playerdata->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(!myteam) return;
	crRole *role = me->getLoadedRoleByIndex(m_index);
	short scope = 0;
	unsigned char effectTarget = 0;
	cameraData->getParam(WCHDATA_JXJPreUseSkillDecal,param);
	m_decalNode = (CRCore::crNode *)param;
	if(role)
	{
		crData *roleData = role->getDataClass();
		int roleid = role->getRoleID();
		roleData->excHandle(MAKECREPARAM(WCH_LockData,1));
		roleData->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *((unsigned char*)param);
		if(itemstate != IS_Dead)
		{
			for( FormationInfoVec::iterator itr = myteam->begin();
				itr != myteam->end();
				++itr )
			{
				if(roleid == (*itr)->getRoleID())
				{
					equips = (*itr)->getEquips();
					break;
				}
			}
			if(equips[0]>0)
			{
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),record)>=0)
				{
					abstractID =atoi(record[itemtab->getTitleIndex("abstractid")].c_str());

				}
			}
			if(abstractID!=0 && dataAttrTab.valid())
			{
				if(param )
				{
					if(dataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(abstractID),record)>=0)
					{
						scope = atoi(record[scopeid].c_str());
						effectTarget = atoi(record[effectTargetIndex].c_str());
						scope = (float)scope/crGlobalHandle::gData()->gUnitScale();
						if(!m_decalNode)
						{
							(*m_playSceneFx)(*this);
							cameraData->inputParam(WCHDATA_JXJPreUseSkillDecal,m_decalNode);
						}

						cameraData->inputParam(WCHDATA_JXJPreUseSkillScope,&scope);
						cameraData->inputParam(WCHDATA_JXJPreUseSkillEffectTargetType, &effectTarget);
					}
				}
			}
		}
		roleData->excHandle(MAKECREPARAM(WCH_LockData,0));

		if(m_decalNode)
		{
			do 
			{
				if(scope>0 && role)
				{
					crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(m_decalNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						crMatrixTransform *bot = dynamic_cast<crMatrixTransform *>(role->getRelNode());
						if(!bot || !bot->getVisiable())
						{
							callback->setVisiable(false);
						}
						else
						{
							callback->setVisiable(true);
							callback->setColor(crVector4(0,1,0,1));
							pos = bot->getTrans();
							float fscope = (float)scope * crGlobalHandle::gData()->gUnitScale();
							fscope *= 2.0f;
							callback->setPosition(crVector2(pos[0],pos[1]));
							callback->setSize(crVector2(fscope,fscope));
							break;
						}
					}
				}
				m_decalNode->setVisiable(false);
			}
			while(0);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvDamageMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvDamageMethod::crJXJClientRecvDamageMethod():
	m_damage(0),
	m_type(0),
	m_ifire(0)
{

}
crJXJClientRecvDamageMethod::crJXJClientRecvDamageMethod(const crJXJClientRecvDamageMethod& handle):
	m_damage(0),
	m_type(0),
	m_ifire(0),
	m_normalInjure(handle.m_normalInjure),
	m_normalInput(handle.m_normalInput),
	m_critInjure(handle.m_critInjure),
	m_critInput(handle.m_critInput),
	m_skillInjure(handle.m_skillInjure),
	m_skillInput(handle.m_skillInput),
	m_cureHPInjure(handle.m_cureHPInjure),
	m_cureHPInput(handle.m_cureHPInput),
	m_jfscInjure(handle.m_jfscInjure),
	m_jfscInput(handle.m_jfscInput),
	m_jfbjInjure(handle.m_jfbjInjure),
	m_jfbjInput(handle.m_jfbjInput),
	m_jffsInjure(handle.m_jffsInjure),
	m_jffsInput(handle.m_jffsInput),
	m_yfscInjure(handle.m_yfscInjure),
	m_yfscInput(handle.m_yfscInput),
	m_yfbjInjure(handle.m_yfbjInjure),
	m_yfbjInput(handle.m_yfbjInput),
	m_yffsInjure(handle.m_yffsInjure),
	m_yffsInput(handle.m_yffsInput),
	m_extraCanvas(handle.m_extraCanvas),
	m_extraInput(handle.m_extraInput)
{
}
void crJXJClientRecvDamageMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_damage = LOCREPARAM(param64);
			m_type = (unsigned char)(LOINT32(HICREPARAM(param64)));
			m_ifire = (char)(HIINT32(HICREPARAM(param64)));
		}
		else
		{
			m_damage = 0;
			m_type = 0;
			m_ifire = 0;
		}
		break;
	}
}
void crJXJClientRecvDamageMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_normalInjure = str;
		break;
	case 1:
		m_normalInput = str;
		break;
	case 2:
		m_critInjure = str;
		break;
	case 3:
		m_critInput = str;
		break;
	case 4:
		m_skillInjure = str;
		break;
	case 5:
		m_skillInput = str;
		break;
	case 6:
		m_prray = str;
		break;
	case 7:
		m_dodge = str;
		break;
	case 8:
		m_prrayOther = str;
		break;
	case 9:
		m_dodgeOther = str;
		break;
	case 10:
		m_deviate = str;
		break;
	case 11:
		m_cureHPInjure = str;
		break;
	case 12:
		m_cureHPInput = str;
		break;
	case 13:
		m_jfscInjure = str;
		break;
	case 14:
        m_jfscInput = str;
		break;
	case 15:
		m_jfbjInjure = str;
		break;
	case 16:
		m_jfbjInput = str;
		break;
	case 17:
		m_jffsInjure = str;
		break;
	case 18:
		m_jffsInput = str;
		break;
	case 19:
		m_yfscInjure = str;
		break;
	case 20:
		m_yfscInput = str;
		break;
	case 21:
		m_yfbjInjure = str;
		break;
	case 22:
		m_yfbjInput = str;
		break;
	case 23:
		m_yffsInjure = str;
		break;
	case 24:
		m_yffsInput = str;
		break;
	case 25:
		m_extraCanvas = str;
		break;
	case 26:
		m_extraInput = str;
		break;
	default:
		break;
	}
}
void crJXJClientRecvDamageMethod::operator ()(crHandle &handle)
{
	if(crDisplaySettings::instance()->getRunMode()>=crDisplaySettings::Robot)
		return;
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(bindview && bindview->isInited() && crRunGameHandle::getInstance()->isInBattle())
	{
		void *param;
		crVector3 window,pos;
		string injure;
		string injureInput;
		char isEnemy = 0;
		crMyPlayerData::getInstance()->getCurrentRole()->doEvent(WCH_EnemyCheck,MAKECREPARAM(m_this,&isEnemy));
		unsigned char type = m_type;
		if(type == CritDM)
		{
			if(m_ifire==1)
			{//自己
				injure = m_jfbjInjure;
				injureInput = m_jfbjInput;
				type = CritDM_me;
			}
			else if(isEnemy == -1)
			{//友军造成伤害
				injure = m_yfbjInjure;
				injureInput = m_yfbjInput;
				type = CritDM_You;
			}
			else
			{
				injure = m_critInjure;
				injureInput = m_critInput;
			}
		}
		else if(type == SkillDM)
		{
			if(m_ifire==1)
			{//自己
				injure = m_jffsInjure;
				injureInput = m_jffsInput;
				type = SkillDM_me;
			}
			else if(isEnemy == -1)
			{//友军造成伤害
				injure = m_yffsInjure;
				injureInput = m_yffsInput;
				type = SkillDM_You;
			}
			else
			{
				injure = m_skillInjure;
				injureInput = m_skillInput;
			}
		}
		else if(type == ParryDM)
		{
			if(isEnemy == -1)
			{
				type = -ParryDM;
				injure = m_prrayOther;
			}
			else
				injure = m_prray;
		}
		else if(type == DodgeDM)
		{
			if(isEnemy == -1)
			{
				type = -DodgeDM;
				injure = m_dodgeOther;
			}
			else
				injure = m_dodge;
		}
		else if(type == MissDM)
		{
			injure = m_deviate;
		}
		else if(type == CureHP)
		{
			injure = m_cureHPInjure;
			injureInput = m_cureHPInput;
		}
		else if (type == ExtraDM)
		{//暂时用SkillDM的
			//if (m_ifire == 1)
			//{//自己
			//	injure = m_jffsInjure;
			//	injureInput = m_jffsInput;
			//	type = ExtraDM_me;
			//}
			//else if (isEnemy == -1)
			//{//友军造成伤害
			//	injure = m_yffsInjure;
			//	injureInput = m_yffsInput;
			//	type = ExtraDM_You;
			//}
			//else
			//{
				injure = m_extraCanvas;
				injureInput = m_extraInput;
			//}
		}
		else 
		{
			if(m_ifire==1)
			{//自己
				injure = m_jfscInjure;
				injureInput = m_jfscInput;
				type = PhysicsDM_Me;
			}
			else if(isEnemy == -1)
			{//友军造成伤害
				injure = m_yfscInjure;
				injureInput = m_yfscInput;
				type = PhysicsDM_You;
			}
			else
			{
				injure = m_normalInjure;
				injureInput = m_normalInput;
			}
			
		}
		ref_ptr<crCanvasNode>  mainCanvas = crFilterRenderManager::getInstance()->getMainCanvas();
		ref_ptr<crCanvasNode>  demageNode = crFilterRenderManager::getInstance()->findCanvas(injure);
		ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
		if( mainCanvas.valid() && demageNode.valid() &&
			bindview && bindview->isInited() && camera.valid())
		{
			ref_ptr<crSceneView> sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			if(cameraData)
			{
				CRCore::ref_ptr<CRCore::crCanvasNode> cpDamageNode;
				cameraData->excHandle(MAKECREPARAM(WCH_LockData,1));
				cameraData->getParam(WCHDATA_JXJRecvDamageMap,param);
				WidgetNodeMap *DamageNodeMap = (WidgetNodeMap *)param;
				//cameraData->getParam(WCHDATA_JXJRecvDamageShowDeque,param);
				//WidgetNodeDeque *damageNodeDeque = (WidgetNodeDeque *)param;

				WidgetNodeMap::iterator itr = DamageNodeMap->find(type);
				bool found = false;
				for ( ; itr != DamageNodeMap->end() && itr->first == type; ++itr )
				{
					if(!itr->second->getVisiable())
					{
						cpDamageNode = itr->second.get();
						found = true;
						break;
					}
				}
				if(!found)
				{
					cpDamageNode = dynamic_cast<crCanvasNode *>(demageNode->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
					cpDamageNode->setCanFocus(false);
					//cpDamageNode->setEnable(true);
					cpDamageNode->setMsgTraverse(KM_All);
					DamageNodeMap->insert(std::make_pair(type,cpDamageNode.get()));
					cameraData->excHandle(MAKECREPARAM(WCH_LockData,0));
					crLoadManager::getInstance()->requestAddNode(mainCanvas.get(),cpDamageNode.get(),false);
				}
				else
				{
					cameraData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				if(cpDamageNode.valid())
				{
					//mainCanvas->addChild(m_cpDamageNode.get());
					if(!injureInput.empty())
					{
						/*if (m_normalInjure == "5WEI")
						{

						ref_ptr<crMultiSwitch> weishusw[5] = {NULL};
						weishusw[0] = dynamic_cast<crMultiSwitch *>(cpDamageNode->getChildNode("sw21"));
						weishusw[1] = dynamic_cast<crMultiSwitch *>(cpDamageNode->getChildNode("sw25"));
						weishusw[2] = dynamic_cast<crMultiSwitch *>(cpDamageNode->getChildNode("sw22"));
						weishusw[3] = dynamic_cast<crMultiSwitch *>(cpDamageNode->getChildNode("sw23"));
						weishusw[4] = dynamic_cast<crMultiSwitch *>(cpDamageNode->getChildNode("sw24"));
						if(m_damage<0)
						{
						int damage = -1 * m_damage;
						std::string str = crArgumentParser::appItoa(damage);
						if (str.size() == 1)
						{
						weishusw[0]->setVisiable(false);
						weishusw[1]->setVisiable(false);
						weishusw[2]->setActiveSwitchSet(damage);
						weishusw[3]->setVisiable(false);
						weishusw[4]->setVisiable(false);
						}
						else if (str.size() == 2)
						{
						weishusw[0]->setVisiable(false);
						weishusw[1]->setActiveSwitchSet(damage/10);
						weishusw[2]->setActiveSwitchSet(damage%10);
						weishusw[3]->setVisiable(false);
						weishusw[4]->setVisiable(false);
						}
						}
						}
						else
						{*/
							ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode *>(cpDamageNode->getWidget(injureInput));
							//if(input!=NULL)
							//{
								if(m_damage>0)
								{
									input->setString(std::string("+")+crArgumentParser::appItoa(m_damage));
								}
								else
								{
									input->setString(crArgumentParser::appItoa(m_damage));
								}
							//}
						//}
					}
					if(injureInput.empty())
					{
						ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(cpDamageNode->getChild(0));
						if(seq.valid())
						{
							seq->setVanishWhenStop(true);
							seq->setDuration(0.1,1);
							seq->setMode(crSequence::START);
						}
					}
					crData *data = cpDamageNode->getDataClass();
					if(data)
					{
						data->inputParam(WCHDATA_JXJRecvDamageItem,m_this);

						pos = m_this->getPosition();
						crData *itemdata = m_this->getDataClass();
						itemdata->getParam(WCHDATA_EyeHeight,param);
						short eyeheight = *(short *)param;
						pos[2]+=eyeheight * crGlobalHandle::gData()->gUnitScale();
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						cpDamageNode->setPosition(pos);
						cpDamageNode->setVisiable(true);
						//damageNodeDeque->push_back(cpDamageNode);
					}
					else
					{
						cpDamageNode->setVisiable(false);
						char gbuf[256];
						sprintf(gbuf,"JXJClientRecvDamage:%s没有data\n\0",cpDamageNode->getName().c_str());
						gDebugInfo->debugInfo(CRCore::WARN,gbuf);
					}
				}
			}
		}
	}
}
///////////////////////////////////////////
////
////crJXJRecvDemageNodeShowMethod
////
///////////////////////////////////////////
//crJXJRecvDemageNodeShowMethod::crJXJRecvDemageNodeShowMethod()
//{
//}
//crJXJRecvDemageNodeShowMethod::crJXJRecvDemageNodeShowMethod(const crJXJRecvDemageNodeShowMethod& handle):
//	crMethod(handle)
//{}
//void crJXJRecvDemageNodeShowMethod::inputParam(int i, void *param)
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
//		m_this = (crMatrixTransform*)param;
//		break;
//	}
//}
//void crJXJRecvDemageNodeShowMethod::addParam(int i, const std::string& str)
//{
//
//}
//void crJXJRecvDemageNodeShowMethod::operator()(crHandle &handle)
//{
//	void *param;
//	crData *cameraData = m_this->getDataClass();
//	if(cameraData)
//	{
//		cameraData->excHandle(MAKECREPARAM(WCH_LockData,1));
//		cameraData->getParam(WCHDATA_JXJRecvDamageShowDeque,param);
//		WidgetNodeDeque *damageNodeDeque = (WidgetNodeDeque *)param;
//		if(!damageNodeDeque->empty())
//		{
//			ref_ptr<crCanvasNode>   demageNode = damageNodeDeque->begin()->get();
//			if(demageNode.valid())
//			{
//				demageNode->setVisiable(true);
//			}
//			damageNodeDeque->pop_front();
//		}
//
//		cameraData->excHandle(MAKECREPARAM(WCH_LockData,0));
//	}
//}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvDamageUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvDamageUIUpdateMethod::crJXJClientRecvDamageUIUpdateMethod():m_this(NULL),m_offset(0.0f),
	m_speed(0.0f),m_maxoffset(0.0f)
{

}
crJXJClientRecvDamageUIUpdateMethod::crJXJClientRecvDamageUIUpdateMethod(const crJXJClientRecvDamageUIUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_offset(0.0f),
	m_speed(handle.m_speed),
	m_maxoffset(handle.m_maxoffset)
{

}
void crJXJClientRecvDamageUIUpdateMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)LOCREPARAM(param64);
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJClientRecvDamageUIUpdateMethod::addParam(int i, const std::string& str)
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
void crJXJClientRecvDamageUIUpdateMethod::operator ()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		void *param;
		crVector3 window,pos;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			ref_ptr<crSceneView>sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			crData *data = m_this->getDataClass();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
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
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						m_offset += m_speed*(*m_dt);
						pos[1]+= m_offset;
						m_this->setPosition(pos);
					}
					else
					{
						m_offset = 0.0f;
						m_this->setVisiable(false);
						//*isUsing = false;
						//m_this->setEnable(false);
					}
				}
				else
				{
					m_offset = 0.0f;
					m_this->setVisiable(false);
					//*isUsing = false;
					//m_this->setEnable(false);
				}
			}
			else
			{
				m_offset = 0.0f;
				m_this->setVisiable(false);
				//*isUsing = false;
				//m_this->setEnable(false);
			}

			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvZeroDamageUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvZeroDamageUIUpdateMethod::crJXJClientRecvZeroDamageUIUpdateMethod():m_this(NULL),m_offset(0.0f),
	m_speed(0.0f),m_maxoffset(0.0f)
{

}
crJXJClientRecvZeroDamageUIUpdateMethod::crJXJClientRecvZeroDamageUIUpdateMethod(const crJXJClientRecvZeroDamageUIUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_offset(0.0f),
	m_speed(handle.m_speed),
	m_maxoffset(handle.m_maxoffset)
{

}
void crJXJClientRecvZeroDamageUIUpdateMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)LOCREPARAM(param64);
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJClientRecvZeroDamageUIUpdateMethod::addParam(int i, const std::string& str)
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
void crJXJClientRecvZeroDamageUIUpdateMethod::operator ()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		void *param;
		crVector3 window,pos;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			ref_ptr<crSceneView> sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			crData *data = m_this->getDataClass();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
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
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						m_offset += m_speed*(*m_dt);
						pos[1]+= m_offset;
						m_this->setPosition(pos);
					}
					else
					{
						m_offset = 0.0f;
						m_this->setVisiable(false);
						ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(m_this->getChild(0));
						if(seq.valid())
						{
							seq->setMode(crSequence::STOP);
						}
						//*isUsing = false;
						//m_this->setEnable(false);
					}
				}
				else
				{
					m_offset = 0.0f;
					m_this->setVisiable(false);
					ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(m_this->getChild(0));
					if(seq.valid())
					{
						seq->setMode(crSequence::STOP);
					}
					//*isUsing = false;
					//m_this->setEnable(false);
				}
			}
			else
			{
				m_offset = 0.0f;
				m_this->setVisiable(false);
				ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(m_this->getChild(0));
				if(seq.valid())
				{
					seq->setMode(crSequence::STOP);
				}
				//*isUsing = false;
				//m_this->setEnable(false);
			}

			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvTrackFireMissUIUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvTrackFireMissUIUpdateMethod::crJXJClientRecvTrackFireMissUIUpdateMethod():m_this(NULL),m_offset(0.0f),
	m_speed(0.0f),m_maxoffset(0.0f)
{

}
crJXJClientRecvTrackFireMissUIUpdateMethod::crJXJClientRecvTrackFireMissUIUpdateMethod(const crJXJClientRecvTrackFireMissUIUpdateMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_offset(0.0f),
	m_speed(handle.m_speed),
	m_maxoffset(handle.m_maxoffset)
{

}
void crJXJClientRecvTrackFireMissUIUpdateMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)LOCREPARAM(param64);
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJClientRecvTrackFireMissUIUpdateMethod::addParam(int i, const std::string& str)
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
void crJXJClientRecvTrackFireMissUIUpdateMethod::operator ()(crHandle &handle)
{
	if(m_this && m_this->getVisiable())
	{
		void *param;
		crVector3 window,pos;
		crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(bindview && bindview->isInited())
		{
			ref_ptr<crSceneView> sceneView = bindview->getDefaultSceneHandler()->getSceneView();
			ref_ptr<crSequence>  seq = dynamic_cast<crSequence *>(m_this->getChild(0));
			crData *data = m_this->getDataClass();
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJRecvTrackFireMissObject,param);
			CRPhysics::crBulletMatterObject *pItem = (CRPhysics::crBulletMatterObject *)param;
			data->getParam(WCHDATA_JXJTrackFireMissPos,param);
			crVector3 pos = *(crVector3 *)param;
			//data->getParam(WCHDATA_JXJDemageNodeIsUsing,param);
			//bool *isUsing = (bool *)param;
			if(m_offset<m_maxoffset && m_this->getVisiable())
			{
				if(pItem)
				{
					if(crBrain::getInstance()->getCameraFrustum().contains(pos))
					{
						pos[2]+=2.0;
						sceneView->projectObjectIntoWindow(pos,window);
						pos = crFilterRenderManager::getInstance()->getIdenticPosition(window);
						pos[2] = 0.0f;
						m_offset += m_speed*(*m_dt);
						pos[1]+= m_offset;
						m_this->setPosition(pos);
					}
					else
					{
						m_offset = 0.0f;
						m_this->setVisiable(false);
						if(seq.valid())
							seq->setMode(crSequence::STOP);
						//*isUsing = false;
						//m_this->setEnable(false);
					}
				}
				else
				{
					m_offset = 0.0f;
					m_this->setVisiable(false);
					if(seq.valid())
						seq->setMode(crSequence::STOP);
					//*isUsing = false;
					//m_this->setEnable(false);
				}
			}
			else
			{
				m_offset = 0.0f;
				m_this->setVisiable(false);
				if(seq.valid())
					seq->setMode(crSequence::STOP);
				//*isUsing = false;
				//m_this->setEnable(false);
			}

			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJUIShiliShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJUIShiliShowMethod::crJXJUIShiliShowMethod()
{

}
crJXJUIShiliShowMethod::crJXJUIShiliShowMethod(const crJXJUIShiliShowMethod& handle)
{

}
void crJXJUIShiliShowMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
			m_this = NULL;
		break;
	case 1:
		m_this = (crCanvasNode *)param;
	default:
		break;
	}
}
void crJXJUIShiliShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_defence = str;
		break;
	case 1:
		m_attack = str;
		break;
	case 2:
		m_strAddFriendSeq = str;
		break;
	}
}
void crJXJUIShiliShowMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crMultiSwitch>  defenceSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_defence));
		ref_ptr<crMultiSwitch>  attackSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_attack));
		ref_ptr<crSequence> addFriendSeq = dynamic_cast<crSequence *>(m_this->getChildNode(m_strAddFriendSeq));
		if(addFriendSeq.valid())
		{
			addFriendSeq->setVanishWhenStop(true);
			addFriendSeq->setMode(crSequence::STOP);
		}
		if(!defenceSwitch || !attackSwitch) return;
		crData *gameData = myPlayer->getDataClass();
		gameData->getParam(WCHDATA_JXJBattleRoomID,param);
		_crInt64 battleroomid = *(_crInt64 *)param;
		int battleid = LOINT64(battleroomid);
		if(battleid != 0)
		{
			unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));
			unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
			if(dshili == 10)
			{
				defenceSwitch->setActiveSwitchSet(0);
			}
			else if(dshili == 11)
			{
				defenceSwitch->setActiveSwitchSet(1);
			}
			else if(dshili == 12)
			{
				defenceSwitch->setActiveSwitchSet(2);
			}
			else if(dshili == 2)
			{
				defenceSwitch->setActiveSwitchSet(3);
			}
			if(ashili == 10)
			{
				attackSwitch->setActiveSwitchSet(0);
			}
			else if(ashili == 11)
			{
				attackSwitch->setActiveSwitchSet(1);
			}
			else if(ashili == 12)
			{
				attackSwitch->setActiveSwitchSet(2);
			}
			else if(ashili == 2)
			{
				attackSwitch->setActiveSwitchSet(3);
			}
		}
		else
		{
			//crData *myData = myPlayer->getDataClass();
			gameData->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			unsigned char type = *(unsigned char *)param;
			gameData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			unsigned short fubenid = *(unsigned short *)param;
			if(type == FT_Fuben && fubenid!=0)
			{
				gameData->getParam(WCHDATA_JXJShiliID,param);
				unsigned char myShili = *(unsigned char *)param;
				if(myShili == 10)
				{
					attackSwitch->setActiveSwitchSet(0);
				}
				else if(myShili == 11)
				{
					attackSwitch->setActiveSwitchSet(1);
				}
				else if(myShili == 12)
				{
					attackSwitch->setActiveSwitchSet(2);
				}
				else if(myShili == 2)
				{
					attackSwitch->setActiveSwitchSet(3);
				}
				defenceSwitch->setActiveSwitchSet(3);
			}
		}
	}
}

crJXJKeyTabMethod::crJXJKeyTabMethod():m_this(NULL)
{

}
crJXJKeyTabMethod::crJXJKeyTabMethod(const crJXJKeyTabMethod& handle):
	crMethod(handle),
	m_this(NULL)
{

}
void crJXJKeyTabMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}
void crJXJKeyTabMethod::addParam(int i, const std::string& str)
{

}
void crJXJKeyTabMethod::operator ()(crHandle &handle)
{
	void *param;
	int _case = -1;
	if(m_this)
	{
		crData *cameraData = m_this->getDataClass();
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
			SelectMyRoleVec* selectMyRoles = (SelectMyRoleVec *)param;
			if(selectMyRoles->empty())
				_case = 0;
		}
	}

	handle.outputParam(0,&_case);
}

/////////////////////////////////////////
//
//crJXJSelectAllRolesMethod
//
/////////////////////////////////////////
crJXJSelectAllRolesMethod::crJXJSelectAllRolesMethod():
	m_this(NULL)
{}
crJXJSelectAllRolesMethod::crJXJSelectAllRolesMethod(const crJXJSelectAllRolesMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJSelectAllRolesMethod::inputParam(int i, void *param)
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
		m_this = (crMatrixTransform*)param;
		break;
	}
}

void crJXJSelectAllRolesMethod::addParam(int i, const std::string& str)
{

}

void crJXJSelectAllRolesMethod::operator()(crHandle &handle)
{
	crNode *fxNode,*fxTargetNode = NULL;
	void *param;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	crData *cameraData = m_this->getDataClass();
	if(!cameraData) return;
	CRCore::ScopedLock<crData> lock(*cameraData);

	cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
	SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
	cameraData->getParam(WCHDATA_JXJBattleTargetRoleVec,param);
	TargetItemVec *targetVec = (TargetItemVec *)param;

	for( SelectMyRoleVec::iterator itr = selectMyRoles->begin();
		itr != selectMyRoles->end();
		++itr )
	{

		fxNode = (*itr)->findSceneFxNode("$SelectDecal");
		if(fxNode)
		{
			crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
			if(callback)
			{
				callback->setVisiable(false);
			}
		}
	}
	for( TargetItemVec::iterator itr = targetVec->begin();
		itr != targetVec->end();
		++itr )
	{

		fxNode = (*itr)->findSceneFxNode("$SelectDecal");
		if(fxNode)
		{
			crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
			if(callback)
			{
				callback->setVisiable(false);
			}
		}
	}
	selectMyRoles->clear();
	targetVec->clear();
	for (int i = 0;i<3;i++)
	{
		fxTargetNode = NULL;
		fxNode = NULL;
		crRole *role = me->getLoadedRoleByIndex(i);
		if(role)
		{
			unsigned int guisestate = GS_Normal;
			role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
			if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || guisestate & GS_Stagnate/* || guisestate & GS_NoAttack*/ || guisestate & GS_Taunt || guisestate & GS_Confusion || guisestate & GS_Scare)
			{//不可被自己选中
				continue;
			}

			crVector3 pos = role->getPosition();
			if(crBrain::getInstance()->getCameraFrustum().contains(pos))
			{
				selectMyRoles->push_back(role);
				fxNode = role->findSceneFxNode("$SelectDecal");

				ref_ptr<crInstanceItem> targetItem;
				ref_ptr<crMatrixTransform> targetNode;
				role->clientGetTarget(targetItem,targetNode);
				if(targetItem.valid() && !crMyPlayerData::getInstance()->ifItemIsMe(targetItem.get()))
				{
					targetVec->push_back(targetItem);
					fxTargetNode = targetItem->findSceneFxNode("$SelectDecal");
				}

				if(fxNode)
				{
					crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						callback->setVisiable(true);
					}
				}
				if(fxTargetNode)
				{
					crDecalUpdateCallback *callback = dynamic_cast<crDecalUpdateCallback *>(fxNode->getUpdateCallback("DecalUpdate"));
					if(callback)
					{
						callback->setVisiable(true);

						callback->setColor(COLOR_RED);
					}
				}
			}
		}
	}

	// 	crMyPlayerData::getInstance()->lockInRangePlayerMap();
	// 	crMyPlayerData::InRangePlayerMap &playerMap = crMyPlayerData::getInstance()->getInRangePlayerMap();
	// 	for( crMyPlayerData::InRangePlayerMap::iterator itr = playerMap.begin();
	// 		itr != playerMap.end();
	// 		++itr )
	// 	{
	// 	}
	// 	crMyPlayerData::getInstance()->unlockInRangePlayerMap();
	// 
	// 	crMyPlayerData::getInstance()->lockInRangeNpcMap();
	// 	crMyPlayerData::InRangeNpcMap &npcMap = crMyPlayerData::getInstance()->getInRangeNpcMap();
	// 	for( crMyPlayerData::InRangeNpcMap::iterator itr = npcMap.begin();
	// 		itr != npcMap.end();
	// 		++itr )
	// 	{
	// 
	// 	}
	// 	crMyPlayerData::getInstance()->unlockInRangeNpcMap();

	//if(!selectMyRoles->empty())
	//{
	//	crRole *role = (*selectMyRoles)[0].get();
	//	crVector3 pos = role->getPosition();
	//	m_this->doEvent(WCH_CameraFocus,MAKECREPARAM(&pos,NULL));
	//}
}

/////////////////////////////////////////
//
//crJXJHideNPCHeadCanvasMethod
//
/////////////////////////////////////////
// crJXJHideNPCHeadCanvasMethod::crJXJHideNPCHeadCanvasMethod()
// {}
// crJXJHideNPCHeadCanvasMethod::crJXJHideNPCHeadCanvasMethod(const crJXJHideNPCHeadCanvasMethod& handle)
// {
// }
// void crJXJHideNPCHeadCanvasMethod::inputParam(int i, void *param)
// {
// }
// 
// void crJXJHideNPCHeadCanvasMethod::addParam(int i, const std::string& str)
// {
// 
// }
// 
// void crJXJHideNPCHeadCanvasMethod::operator()(crHandle &handle)
// {
// 	void *param;
// 	bool isShow;
// 	crCamera *camera = crCameraManager::getInstance()->getMainCamera();
// 	if(camera)
// 	{
// 		crData *cameraData = camera->getAttachedNode()->getDataClass();
// 		if(cameraData)
// 		{
// 			cameraData->getParam(WCHDATA_JXJISShowNPCHeadCanvas,param);
// 			isShow = *(bool *)param;
// 			if(isShow)
// 				isShow = false;
// 			else if(!isShow)
// 				isShow = true;
// 			cameraData->inputParam(WCHDATA_JXJISShowNPCHeadCanvas,&isShow);
// 		}
// 	}
// 
// }

/////////////////////////////////////////
//
//crJXJChangeMoveSpeedStateMethod
//
/////////////////////////////////////////
// crJXJChangeMoveSpeedStateMethod::crJXJChangeMoveSpeedStateMethod()
// {}
// crJXJChangeMoveSpeedStateMethod::crJXJChangeMoveSpeedStateMethod(const crJXJChangeMoveSpeedStateMethod& handle)
// {
// }
// void crJXJChangeMoveSpeedStateMethod::inputParam(int i, void *param)
// {
// }
// 
// void crJXJChangeMoveSpeedStateMethod::addParam(int i, const std::string& str)
// {
// 
// }
// 
// void crJXJChangeMoveSpeedStateMethod::operator()(crHandle &handle)
// {
// 	void *param;
// 	bool isShow;
// 	crCamera *camera = crCameraManager::getInstance()->getMainCamera();
// 	if(camera)
// 	{
// 		crData *cameraData = camera->getAttachedNode()->getDataClass();
// 		if(cameraData)
// 		{
// 			cameraData->getParam(WCHDATA_JXJIsChangeMoveSpeed,param);
// 			isShow = *(bool *)param;
// 			if(isShow)
// 				isShow = false;
// 			else if(!isShow)
// 				isShow = true;
// 			cameraData->inputParam(WCHDATA_JXJIsChangeMoveSpeed,&isShow);
// 		}
// 	}
// 
// }
//////////////////////////////////////////////////////////////////////////
//
//crJXJShowItemInPacketMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJShowItemInPacketMethod::crJXJShowItemInPacketMethod()
{

}
crJXJShowItemInPacketMethod::crJXJShowItemInPacketMethod(const crJXJShowItemInPacketMethod& handle)
{

}
void crJXJShowItemInPacketMethod::inputParam(int i, void *param)
{

}
void crJXJShowItemInPacketMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_chatCanvas = str;
		break;
	case 1:
		m_chatInput = str;
		break;
	default:
		break;
	}
}
void crJXJShowItemInPacketMethod::operator ()(crHandle &handle)
{
	void *param;
	string itemnameStr;
	ref_ptr<crCanvasNode>  chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_chatCanvas);
	if(!chatCanvas) return;
	ref_ptr<crEditWidgetNode>  chatInput = dynamic_cast<crEditWidgetNode *>(chatCanvas->getWidget(m_chatInput));
	if(!chatInput) return;
	crData *chatCanvasData = chatCanvas->getDataClass();
	if(!chatCanvasData) return;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;

	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	crData *braindata = crBrain::getInstance()->getDataClass();
	braindata->getParam(WCHDATA_JXJSaleItemID,param);
	unsigned char selectid = *(unsigned char*)param;
	crTableIO::StrVec itemrecord;
	int nameindex = itemtab->getTitleIndex("name");

	if(selectid>=0)
	{
		crData *data = myPlayer->getDataClass();
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJItemBagVec,param);
		JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
		if(selectid < itembagvec->size())
		{
			int itemId = (*itembagvec)[selectid]->getItemID();
			int itemEquipMagic = (*itembagvec)[selectid]->getEquipMagic();
			chatCanvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
			chatCanvasData->getParam(WCHDATA_JXJAboutToShowItemIDVec,param);
			ShowItemIDVec *itemIDVec = (ShowItemIDVec *)param;
			chatCanvasData->getParam(WCHDATA_JXJIsToShowItems,param);
			bool *isitem = (bool *)param;
			if(itemIDVec->size()<4)
			{
				if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemId),itemrecord)>=0)
				{
					if(nameindex >= 0)
					{
						itemIDVec->push_back(make_pair(itemId,itemEquipMagic));
						*isitem = true; 
						itemnameStr = itemrecord[nameindex];
					}
				}

				if(!itemnameStr.empty())
				{
					string tempStr = chatInput->getUTF8String();
					string itemName = string("[")+itemnameStr+string("]");
					tempStr+=itemName;
					chatInput->setString(tempStr);
					chatInput->setFocusPos(chatInput->getUTF8String().size());
				}
			}
			chatCanvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJShowItemLinkClickMethod
//
/////////////////////////////////////////
crJXJShowItemLinkClickMethod::crJXJShowItemLinkClickMethod():
	m_ea(NULL),
	m_this(NULL),
	m_itemid(0),
	m_equipMagic(0),
	m_cardlvsw("XingJi")
{
}
crJXJShowItemLinkClickMethod::crJXJShowItemLinkClickMethod(const crJXJShowItemLinkClickMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info1(handle.m_info1),
	m_info2(handle.m_info2),
	m_itemlv(handle.m_itemlv),
	m_itemicon(handle.m_itemicon),
	m_itemequipon(handle.m_itemequipon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	m_JKtips(handle.m_JKtips),
	m_card(handle.m_card),
	m_backboard(handle.m_backboard),
	m_guojia(handle.m_guojia),
	m_shuxing(handle.m_shuxing),
	m_bingzhong(handle.m_bingzhong),
	m_leixing(handle.m_leixing),
	m_jinengmiaoshu(handle.m_jinengmiaoshu),
	m_nameimage(handle.m_nameimage),
	m_jineng(handle.m_jineng),
	m_cardlvsw(handle.m_cardlvsw),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJShowItemLinkClickMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_ea = (CRProducer::crGUIEventAdapter*)(LOCREPARAM(param64));
			m_linkInfo = (CRUI::crHypertextWidgetNode::LinkInfoPair *)(HICREPARAM(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	case 3:
		m_itemid = *(int*)param;
		break;
	case 4:
		m_equipMagic = *(int*)param;
		break;
	}
}
void crJXJShowItemLinkClickMethod::addParam(int i, const std::string& str)
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
		m_info1 = str;
		break;
	case 3:
		m_info2 = str;
		break;
	case 4:
		m_itemlv = str;
		break;
	case 5:
		m_itemicon = str;
		break;
	case 6:
		m_itemequipon = str;
		break;
	case 7:
		m_canvas = str;
		break;
	case 8:
		m_infocanvas = str;
		break;
	case 9:
		m_infobk = str;
		break;
	case 10:
		m_infodi = str;
		break;
	case 11:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 12:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 13:
		m_JKtips = str;
		break;
	case 14:
		m_card = str;
		break;
	case 15:
		m_backboard = str;
		break;
	case 16:
		m_guojia = str;
		break;
	case 17:
		m_shuxing = str;
		break;
	case 18:
		m_bingzhong = str;
		break;
	case 19:
		m_leixing = str;
		break;
	case 20:
		m_jinengmiaoshu = str;
		break;
	case 21:
		m_nameimage = str;
		break;
	case 22:
		m_jineng = str;
		break;
	}
}
void crJXJShowItemLinkClickMethod::operator()(crHandle &handle)
{
	int type = 0;
	crBrain::getInstance()->doEvent(WCH_JXJCheckItemType,MAKECREPARAM(m_itemid,&type));
	if(type == IT_Jiangka)
	{
		ref_ptr<crCanvasNode>  tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_JKtips);
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		ref_ptr<crTableIO> troopsEquipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(m_itemid>0 && tipsCanvas.valid() && playerData && troopsEquipTab.valid())
		{
			ref_ptr<crHypertextWidgetNode>  jinengmiaoshu = dynamic_cast<crHypertextWidgetNode *>(tipsCanvas->getWidget(m_jinengmiaoshu));
			ref_ptr<crImageBoxWidgetNode>  card = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_card));
			ref_ptr<crImageBoxWidgetNode>  nameimage = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_nameimage));
			ref_ptr<crMultiSwitch>  backboard = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_backboard));
			ref_ptr<crMultiSwitch>  cardlvsw = dynamic_cast<crMultiSwitch *>(tipsCanvas->getChildNode(m_cardlvsw));

			int cardid = troopsEquipTab->getTitleIndex("picture");
			int nameimageid = troopsEquipTab->getTitleIndex("姓名");
			int backboardid = troopsEquipTab->getTitleIndex("品质");
			int guojiaid = troopsEquipTab->getTitleIndex("国家");
			crTableIO::StrVec record;
			if (troopsEquipTab->queryOneRecord(0,crArgumentParser::appItoa(m_itemid),record)>=0)
			{
				if(jinengmiaoshu.valid())
				{
					ItemMap itemMap;
					rcfg::ConfigScript outscript(&itemMap);
					crVector2i item(m_itemid,0);
					playerData->doEvent(WCH_JXJCreateItemTips,MAKECREPARAM(&item,&outscript));
					jinengmiaoshu->setHypertext(outscript);
				}
				if(card.valid())
				{
					card->setImageName(record[cardid].c_str());
				}
				if(nameimage.valid())
				{
					nameimage->setImageName(record[nameimageid].c_str());
				}
				if(backboard.valid())
				{
					int cardcolor = atoi(record[backboardid].c_str());
					backboard->setActiveSwitchSet(cardcolor);
					//if (cardcolor < CardColor_Orange)
					//{
					//	backboard->setActiveSwitchSet(cardcolor);
					//	if (cardlvsw.valid())
					//	{
					//		cardlvsw->setActiveSwitchSet(0);
					//	}
					//}
					//else
					//{
					//	backboard->setActiveSwitchSet(cardcolor);//CardColor_Orange
					//	if (cardlvsw.valid())
					//	{
					//		cardlvsw->setActiveSwitchSet(cardcolor - CardColor_Orange + 1);
					//	}
					//}
				}

				const crBoundingBox &infobbox = tipsCanvas->getBoundBox();
				crVector3f mouse(m_ea->getXnormalized()+infobbox.xLength()*0.5 ,m_ea->getYnormalized()+infobbox.yLength()*0.5,0.0f);
				tipsCanvas->setPosition(mouse);
				tipsCanvas->setCanFocus(false);
				tipsCanvas->setCanCaptureMouse(true);
				crFilterRenderManager::getInstance()->doModal(tipsCanvas.get());
			}
		}
	}
	else
	{
		ref_ptr<crCanvasNode>  tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		ref_ptr<crCanvasNode>  infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(tipsCanvas.valid() && playerData && infocanvas.valid() && playerData->getMainRole() && playerData->getMainRole()->getDataClass())
		{
			void *param;
			ref_ptr<crStaticTextWidgetNode>  name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
			//crStaticTextWidgetNode *info1 = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_info1));
			ref_ptr<crStaticTextWidgetNode>  itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
			ref_ptr<crImageBoxWidgetNode>  itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
			ref_ptr<crStaticTextWidgetNode>  itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
			//
			ref_ptr<crHypertextWidgetNode>  info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
			crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
			crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
			if (name.valid())
			{
				name->clearString();
			}
			//if (info1)
			//{
			//	info1->clearString();
			//}
			if (info2.valid())
			{
				info2->clearText();
			}
			if (itemlv.valid())
			{
				itemlv->clearString();
			}
			if (itemicon.valid())
			{
				itemicon->clearImage();
			}
			if (itemequipon.valid())
			{
				itemequipon->clearString();
			}
			crRole *mainRole = playerData->getMainRole();
			crData *roleData = mainRole->getDataClass();
			roleData->getParam(WCHDATA_Level,param);
			unsigned char rolelv = *(unsigned char *)param;
			//crListControlNode *selectNode = m_this->getSelectNode();
			crData *data = playerData->getDataClass();
			data->getParam(WCHDATA_JXJOpenBagNum,param);
			unsigned char openbagnum = *(unsigned char *)param;

			crVector4f colormode;//white,green,blue,purple,orange
			ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
			crTableIO::StrVec colorrecord;
			int qualityindex = qualitytab->getTitleIndex("color");
			//for (int i =0; i<5;i++)
			//{
			//	if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(i),colorrecord)>=0)
			//	{
			//		if (qualityindex>=0)
			//		{
			//			crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode);
			//			colormode /= 255.0f;
			//		}
			//	}
			//}
			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(m_itemid>0 && itemtab.valid())
			{
				ref_ptr<crTableIO> lordequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
				//int tnameindex = lordequiptab->getTitleIndex("称号");

				int iconindex = itemtab->getTitleIndex("icon");
				//	int info2index = itemtab->getTitleIndex("功能");
				int nameindex = itemtab->getTitleIndex("name");
				int typeindex = itemtab->getTitleIndex("类型");
				int colorindex = itemtab->getTitleIndex("品质");
				int needlvindex = itemtab->getTitleIndex("人物等级");
				//int shuxingindex = itemtab->getTitleIndex("物品描述");
				crTableIO::StrVec record,lordrecord;

				if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(m_itemid),record)>=0)
				{
					int colormodeid = atoi(record[colorindex].c_str());
					if(qualitytab->queryOneRecord(0,record[colorindex],colorrecord) >= 0 )
					{
						crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode);
						colormode /= 255.0f;
					}
					if(name.valid())
					{
						name->setColor(colormode);
						name->setBackdropType(CRText::crText::OUTLINE);
						//name->setBackdropOffset(crVector2f(0.008f,0.008f));
						//name->setBackdropColor(colormode[colormodeid]);
						name->setString(record[nameindex]);
					}
					std::string file;
					if (itemlv.valid())
					{
						if (rolelv < atoi(record[needlvindex].c_str()))//人物等级不足时显示红字
						{
							itemlv->setColor(m_colorred);
							if (itemequipon.valid())
							{
								itemequipon->setColor(m_colorred);
							}
						}
						else
						{
							itemlv->setColor(COLOR_WHITE);
							if (itemequipon.valid())
							{
								itemequipon->setColor(COLOR_GREEN);
							}
						}
						itemlv->setString(record[needlvindex]);
					}
					if (itemicon.valid())
					{
						itemicon->setImageName(record[iconindex]);
					}
					if (itemequipon.valid())
					{
						std::string str;// = "未使用";
						if (atoi(record[typeindex].c_str()) == IT_Equip)
						{
							str = "未装备";
						}
						itemequipon->setString(str);
					}
					if(info2.valid())
					{
						ItemMap itemMap;
						rcfg::ConfigScript outscript(&itemMap);
						crVector2i item(m_itemid,m_equipMagic);
						playerData->doEvent(WCH_JXJCreateItemTips,MAKECREPARAM(&item,&outscript));
						info2->setHypertext(outscript);
					}
					const crBoundingBox &tips = tipsCanvas->getBoundBox();
					float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
					float posy = (tips.m_max[1]-tips.m_min[1])*0.5;

					/////////////////////////////////////////
					//crVector2 mouse(m_ea->getXnormalized()-posx,m_ea->getYnormalized()-posy);
					tipsCanvas->setCanFocus(false);
					infocanvas->setCanFocus(false);
					infocanvas->setEnable(false);
					///////////计算缩放
					float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
					info2->updateData();
					const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
					const crBoundingBox &bbox2 = info2->getBoundBox();
					float texheight = 0.0f;
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
							centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.125f * diYlength;
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
							needmove = boxYlength - texheight + diYlength * 0.125f;
															  
							infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
						}
					}
					else
					{
						infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
						if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
					}

					const crBoundingBox &tipsbbox = tipsCanvas->getBoundBox();
					const crBoundingBox &infobbox = infobk->getBoundBox();
					crVector3f mouse(m_ea->getXnormalized()+infobbox.xLength()*0.5 ,m_ea->getYnormalized()+infobbox.yLength(),0.0f);
					//canvas->setMatrix(crMatrix::translate(mouse[0],mouse[1],0.0f));
					tipsCanvas->setPosition(mouse);
					infocanvas->setPosition(mouse);
					infocanvas->setCanCaptureMouse(true);
					tipsCanvas->setCanCaptureMouse(true);
					crFilterRenderManager::getInstance()->doModal(infocanvas.get());
					crFilterRenderManager::getInstance()->doModal(tipsCanvas.get());
				}
			}

		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJSelectBirthPointMapTypeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSelectBirthPointMapTypeMethod::crJXJSelectBirthPointMapTypeMethod(){}
crJXJSelectBirthPointMapTypeMethod::crJXJSelectBirthPointMapTypeMethod(const crJXJSelectBirthPointMapTypeMethod& handle){}
void crJXJSelectBirthPointMapTypeMethod::inputParam(int i, void *param){}
void crJXJSelectBirthPointMapTypeMethod::addParam(int i, const std::string& str){}
void crJXJSelectBirthPointMapTypeMethod::operator ()(crHandle &handle)
{
	int _case = 0;
	const crVector2i &mapSize = crBrain::getInstance()->getMapSize();
	if(mapSize.x()>mapSize.y())
		_case = 1;
	else if(mapSize.x()<mapSize.y())
		_case = 2;
	handle.outputParam(0,&_case);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJChatInputGetFocusMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChatInputGetFocusMethod::crJXJChatInputGetFocusMethod(){}
crJXJChatInputGetFocusMethod::crJXJChatInputGetFocusMethod(const crJXJChatInputGetFocusMethod& handle){}
void crJXJChatInputGetFocusMethod::inputParam(int i, void *param){}
void crJXJChatInputGetFocusMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_edit = str;
		break;
	}
}
void crJXJChatInputGetFocusMethod::operator ()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		canvas->setDefFocusIndex(0);
		ref_ptr<crEditWidgetNode>  editBox = dynamic_cast<crEditWidgetNode *>(canvas->getWidget(m_edit));
		if(editBox.valid())
		{
			string str = editBox->getUTF8String();
			editBox->setFocusPos(str.length());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJIsPlayerInFuBenMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJIsPlayerInFuBenMethod::crJXJIsPlayerInFuBenMethod(){}
crJXJIsPlayerInFuBenMethod::crJXJIsPlayerInFuBenMethod(const crJXJIsPlayerInFuBenMethod& handle){}
void crJXJIsPlayerInFuBenMethod::inputParam(int i, void *param)
{

}
void crJXJIsPlayerInFuBenMethod::addParam(int i, const std::string& str)
{

}
void crJXJIsPlayerInFuBenMethod::operator ()(crHandle &handle)
{
	bool isInFuben = false;
	void *param;
	unsigned char roomType = 0;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me)
	{
		crRoom *room = me->getSelectedRoom();
		if(room)
		{
			crData *roomData = room->getDataClass();
			if(roomData)
			{
				roomData->getParam(WCHDATA_JXJRoomType,param);
				roomType = *(unsigned char *)param;
			}
		}
	}
	if(roomType==1)
		isInFuben = false;
	else if(roomType == 2)
		isInFuben = true;
	else if(roomType == 3)
		isInFuben = true;
	handle.outputParam(0,&isInFuben);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJIsAllDeadInFubenMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJIsAllDeadInFubenMethod::crJXJIsAllDeadInFubenMethod()
{}
crJXJIsAllDeadInFubenMethod::crJXJIsAllDeadInFubenMethod(const crJXJIsAllDeadInFubenMethod& handle):
	crMethod(handle)
{}
void crJXJIsAllDeadInFubenMethod::inputParam(int i, void *param)
{

}
void crJXJIsAllDeadInFubenMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_victoryCanvas = str;
		break;
	default:
		break;
	}
}
void crJXJIsAllDeadInFubenMethod::operator ()(crHandle &handle)
{
	bool iswfbrcisz = false;
	void *param;
	bool isshow = true;
	unsigned int guisestate;
	crRole *role = NULL;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	for(int i = 0;i<3;i++)
	{
		role = me->getLoadedRoleByIndex(i);
		if(!role)
			continue;
		//crData *data = role->getDataClass();
		guisestate = GS_Normal;
		role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		isshow = isshow && (guisestate & GS_UnVisiable);
	}
	crVector3 deadPos;
	role = me->getLoadedRoleByIndex(0);
	if(role) deadPos = role->getPosition();

	if(!isshow)
	{
		iswfbrcisz = false;
		handle.outputParam(0,&iswfbrcisz);
		return;
	}
	int reliveCount = 0;
	unsigned char count = 0;
	ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
	ref_ptr<crTableIO>worldfubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJWorldFubenab);
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode>  fubenCanvas = crFilterRenderManager::getInstance()->findCanvas(m_victoryCanvas);
	crPlayerGameData *myPlayer = me->getPlayerGameData();
	if(canvas.valid() && fubenCanvas.valid() && fubenCanvas->getDataClass() && myPlayer)
	{
		unsigned char vipLevel = 0;
		//unsigned char roomType = 0;
		unsigned char type = 1;
		ref_ptr<crHypertextWidgetNode>  input = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_input));
		//crButtonWidgetNode *btRelive = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_btRelive));
		crData *myPlayerData = myPlayer->getDataClass();
		if(input.valid() && myPlayerData)
		{
			//btRelive->setEnable(false);
			unsigned short fubenid = 0;
			crTableIO::StrVec record;


			myPlayerData->getParam(WCHDATA_JXJFubenOrChengchiType,param);
			type = *(unsigned char *)param;
			//if(type == FT_Fuben)
			//{
			myPlayerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
			fubenid = *(unsigned short *)param;

			myPlayerData->getParam(WCHDATA_JXJVipLv,param);
			vipLevel = *(unsigned char *)param;

			//crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			//if(room)
			//{
			//	crData *roomData = room->getDataClass();
			//	if(roomData)
			//	{
			//		roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
			//		myPlayerData->getParam(WCHDATA_JXJShiliID,param);
			//		unsigned char myShili = *(unsigned char *)param;

			//		crSceneLayer::BirthPointArray pointVec = crMyPlayerData::getInstance()->getSceneLayer()->getBirthPointArray();
			//		int index = 0;
			//		crVector2 birthPoint,pointOffset,pointMin;
			//		crVector3 minPoint;
			//		char minPointIndex = index;

			//		birthPoint = crVector2(pointVec[index].x()*crGlobalHandle::gData()->gUnitScale(),pointVec[index].y()*crGlobalHandle::gData()->gUnitScale());
			//		pointOffset = birthPoint-crVector2(deadPos.x(),deadPos.y());
			//		minPoint = pointVec[index];
			//		minPointIndex = index;
			//		pointMin = pointOffset;
			//		index++;
			//		roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
			//		JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
			//		JXJCampBirthpoint::iterator itr_point = (*campMap)[myShili].begin();

			//		for (;itr_point!=(*campMap)[myShili].end();itr_point++,index++)
			//		{
			//			birthPoint = crVector2(itr_point->x()*crGlobalHandle::gData()->gUnitScale(),itr_point->y()*crGlobalHandle::gData()->gUnitScale());
			//			pointOffset = birthPoint-crVector2(deadPos.x(),deadPos.y());
			//			if(pointMin.length() > pointOffset.length())
			//			{
			//				minPoint = pointVec[index];
			//				minPointIndex = index;
			//				pointMin = pointOffset;
			//			}
			//		}
			//		roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
			//	}
			//}
			//}


			/*crRoom *room = me->getSelectedRoom();
			if(room)
			{
			crData *roomData = room->getDataClass();
			if(roomData)
			{
			roomData->getParam(WCHDATA_JXJRoomType,param);
			roomType = *(unsigned char *)param;
			}
			}*/

			if(fubenid>0 /*&& fubentab.valid()*/)
			{
				//int reliveIndex = fubentab->getTitleIndex("允许补兵次数");
				if (type == 2)
				{
					if(fubentab.valid() && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
					{
						int reliveIndex = fubentab->getTitleIndex("允许补兵次数");
						if(reliveIndex>0)
							reliveCount = atoi(record[reliveIndex].c_str());
					}
				}
				else if(type == 3)
				{
					reliveCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJWorldFuBenReliveCount,vipLevel).c_str());
				}
			}
			crData *canvasData = fubenCanvas->getDataClass();
			canvasData->getParam(WCHDATA_JXJFubenCanReliveCount,param);
			count = *(unsigned char *)param;
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(reliveCount>count)
			{
				myPlayerData->getParam(WCHDATA_JXJChooseAutoBattleMode,param);
				bool autoBattle = *(bool*)param;
				if(autoBattle && netConductor)
				{//自动战斗&&自动复活
					crPlayerDataSceneEventPacket packet;
					crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenRelive,NULL);
					netConductor->getNetManager()->sendPacket("all",packet);
				}
				else
				{
					//btRelive->setEnable(true);
					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					cfg_script.Add("Hypertext");
					cfg_script.Push("Hypertext");
					cfg_script.Add("Content");
					cfg_script.Push("Content");
					cfg_script.Add("Text","您还有");
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",2);
					v_i.clear();
					v_i.push_back(0.0);
					v_i.push_back(255.0);
					v_i.push_back(0.0);
					v_i.push_back(255.0);
					cfg_script.Add("Color",v_i);
					cfg_script.Add("Text",crArgumentParser::appItoa(reliveCount-count));
					cfg_script.Pop();

					cfg_script.Add("Content");
					cfg_script.Push("Content",3);
					cfg_script.Add("Text",string("次补兵机会，是否补满兵力？"));
					cfg_script.Pop();
					cfg_script.Pop();

					input->setHypertext(cfg_script);
					crFilterRenderManager::getInstance()->doModal(m_canvas);
				}
			}
			else
			{
				//发包服务器是否结算
				if (type == 2)
				{
					
					if(netConductor)
					{
						crPlayerDataSceneEventPacket packet;
						crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJFubenReliveCountHasUsed,NULL);
						netConductor->getNetManager()->sendPacket("all",packet);
					}
				}
				else if(type == 3)
				{
					iswfbrcisz = true;
				}
			}
		}
	}
	handle.outputParam(0,&iswfbrcisz);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJChangeBirthPointMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJChangeBirthPointMethod::crJXJChangeBirthPointMethod()
{

}
crJXJChangeBirthPointMethod::crJXJChangeBirthPointMethod(const crJXJChangeBirthPointMethod& handle)
{

}
void crJXJChangeBirthPointMethod::inputParam(int i, void *param)
{

}
void crJXJChangeBirthPointMethod::addParam(int i, const std::string& str)
{

}
void crJXJChangeBirthPointMethod::operator ()(crHandle &handle)
{

}
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenReliveArmyMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenReliveArmyMethod::crJXJFubenReliveArmyMethod()
{

}
crJXJFubenReliveArmyMethod::crJXJFubenReliveArmyMethod(const crJXJFubenReliveArmyMethod& handle)
{

}
void crJXJFubenReliveArmyMethod::inputParam(int i, void *param)
{

}
void crJXJFubenReliveArmyMethod::addParam(int i, const std::string& str)
{

}
void crJXJFubenReliveArmyMethod::operator ()(crHandle &handle)
{
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
		if(room)
		{
			crData *roomData = room->getDataClass();
			if(roomData)
			{
				// 				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				// 				stream->createBuf(1);
				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvFubenRelive,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJFubenSettle2Method
//
/////////////////////////////////////////
// crJXJFubenSettle2Method::crJXJFubenSettle2Method():
// 	m_progress(0){}
// crJXJFubenSettle2Method::crJXJFubenSettle2Method(const crJXJFubenSettle2Method& handle):
// 	crMethod(handle),
// 	m_progress(handle.m_progress)
// {
// }
// void crJXJFubenSettle2Method::addParam(int i, const std::string& str)
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		m_progress = (short)(atoi(str.c_str()));
// 		break;
// 	}
// }
// void crJXJFubenSettle2Method::inputParam(int i, void *param)
// {
// 	switch(i) 
// 	{
// 	case 0:
// 		if(param == 0)
// 		{//释放
// 			m_this = NULL;
// 		}
// 		break;
// 	case 1:
// 		m_this = (crRoom *)param;
// 		break;
// 	}
// }
// void crJXJFubenSettle2Method::operator()(crHandle &handle)
// {
// 	void *param;
// 	crData *data = m_this->getDataClass();
// 	data->excHandle(MAKECREPARAM(WCH_LockData,1));
// 	data->getParam(WCHDATA_JXJBattleID,param);
// 	int battleid = *(int *)param;
// 	unsigned short fubenid = HIINT32(battleid);
// 	data->getParam(WCHDATA_RoomProgress,param);
// 	short progress = *(short *)param;
// 	data->getParam(WCHDATA_JXJBattleStatsMap,param);
// 	BattleStatsMap *battleStatsMap = (BattleStatsMap *)param;
// 	ref_ptr<crStreamBuf> stream = new crStreamBuf;
// 	stream->createBuf(5+battleStatsMap->size()*12);
// 	stream->_writeUShort(fubenid);//or playerid
// 	stream->_writeBool(progress == m_progress);//true表示攻方胜利
// 	stream->_writeUShort(battleStatsMap->size());
// 	//crJXJBattleStats *battleStats;
// 	//百分比兵力返还//计算经验奖励//发包
// 	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
// 	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
// 	//crSceneServerPlayerData *playerData = NULL;
// 	//crTableIO::StrVec record;
// // 	ref_ptr<crTableIO> troopsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
// // 	int experienceIndex = troopsTab->getTitleIndex("死亡经验系数");
// // 	float experienceFactor = atof(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleExperienceFactor,0).c_str());
// 	unsigned char relivecount = 0;
// 	for( BattleStatsMap::iterator itr = battleStatsMap->begin();
// 		itr != battleStatsMap->end();
// 		++itr )
// 	{
// 		stream->_writeInt(itr->first);//4
// 		stream->_writeInt(itr->second->getNpcKillCount());//4
// 		stream->_writeInt(itr->second->getBosskillCount());//4
// 	}
// 	data->excHandle(MAKECREPARAM(WCH_LockData,0));
// 
// 	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);
// 	crJXJFubenSettlePacket packet;
// 	crJXJFubenSettlePacket::buildRequestPacket(packet,stream.get());
// 	netConductor->getNetManager()->sendPacket("all",packet);
// }

/////////////////////////////////////////
//
//crJXJRecvFubenSettle2Method
//
/////////////////////////////////////////
crJXJRecvFubenSettle2Method::crJXJRecvFubenSettle2Method(){}
crJXJRecvFubenSettle2Method::crJXJRecvFubenSettle2Method(const crJXJRecvFubenSettle2Method& handle):
	crMethod(handle)
{
}
void crJXJRecvFubenSettle2Method::inputParam(int i, void *param)
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
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
		}
		else
		{
			m_stream = NULL;
		}
		break;
	}
}

void crJXJRecvFubenSettle2Method::addParam(int i, const std::string& str)
{
}

void crJXJRecvFubenSettle2Method::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		unsigned short fubenid = m_stream->_readUShort();//2

		bool win = m_stream->_readBool();//1
		unsigned short mapsize = m_stream->_readUShort();//2
		int playerid = 0;
		float experience = 0.0f;
// 		int npcCount = 0;
// 		int bossCount = 0;

		std::map< int, FubenKillAndStarInfo  > playerMap;
		FubenKillAndStarInfo tempPlayerFuben;
		for ( int i = 0; i<mapsize; i++ )
		{
			
			playerid = m_stream->_readInt();//4
			tempPlayerFuben.npcCount = m_stream->_readInt();//4
			tempPlayerFuben.bossCount = m_stream->_readInt();//4
			tempPlayerFuben.starCount = m_stream->_readUChar();//1
			playerMap.insert(make_pair(playerid, tempPlayerFuben));
		}
		int roomid = m_stream->_readInt();
		float circle = m_stream->_readFloat();

		if (fubenid == 3010)
		{
			m_this->doEvent(WCH_JXJDisposeNewHandlGuoZhan,MAKECREPARAM(playerid,(int)fubenid));
			return;
		}

		crTableIO::StrVec record;
		ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		int chaptureIdx = fubentab->getTitleIndex("chapterid");
		if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
		{
			if (atoi(record[chaptureIdx].c_str()) == 1) // 军团副本结算
			{
				//军团副本 全部算胜利
				win = true;
				crJXJJunTuanFuBenSettleMethod::FuBenPlayerInfo fuBenPlayerInfo;
				fuBenPlayerInfo.fubenID = fubenid;
				fuBenPlayerInfo.playerMap = & playerMap;
				fuBenPlayerInfo.win = win;

				/////军团副本结束时清此副本；
				crData * groupData = NULL;
				crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
				gsBrainData->getParam(WCHDATA_JXJConsortiaMap,param);
				ConsortiaMap *consortiamap = (ConsortiaMap *)param;
				ConsortiaMap::iterator cmitr = consortiamap->begin();
				for (;cmitr != consortiamap->end();++cmitr)
				{
					if (cmitr->second.valid())
					{
						groupData = cmitr->second->getDataClass();
						if (groupData)
						{
							groupData->getParam(WCHDATA_JXJMyGroupStartedFuBenMap,param);
							StartedFubenMap * startedFubenMap  = (StartedFubenMap *)param;
							StartedFubenMap::iterator sfmitr = startedFubenMap->find(fubenid);
							if (sfmitr != startedFubenMap->end())
							{
								if (sfmitr->second.first == roomid)
								{
									startedFubenMap->erase(sfmitr);
									break;
								}
							}

						}
					}
				}
				gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
				if (win)
				{
					m_this->doEvent(WCH_JXJJunTuanFuBenSettle,MAKECREPARAM(&fuBenPlayerInfo,&circle));
					return;
				}
			}
		}

// 		int canreliveCount = 0;
// 		if(fubentab)
// 		{
// 			int reliveIndex = fubentab->getTitleIndex("允许补兵次数");
// 			if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
// 			{
// 				if(reliveIndex>0)
// 					canreliveCount = atoi(record[reliveIndex].c_str());
// 			}
// 		}
		

		ref_ptr<crTableIO> fubenRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
		crTableIO::DataVec recordVec;
		fubenRewardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),recordVec);
		if(!recordVec.empty())
		{
			ref_ptr<crGameServerPlayerData> playerData;
			ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
			ref_ptr<crPlayerRewardData> reward_info_copy;
			//FubenDropRewardVec fubenDropVec;
			//RewardItemVec reward_items;
			//RewardEquipsVec reward_equip;
			crPlayerGameData *playerGameData = NULL;
			int copperIndex = fubenRewardTab->getTitleIndex("铜钱");
			int expIndex = fubenRewardTab->getTitleIndex("经验");
			int gongxunIndex = fubenRewardTab->getTitleIndex("功勋");
			int giftgoldIndex = fubenRewardTab->getTitleIndex("礼券");
			//int index = (win)?1:0;
			crPlayerDataEventPacket packet;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			map< int, FubenKillAndStarInfo >::iterator itr_player = playerMap.begin();

			if(win)
				m_this->doEvent(WCH_JXJFubenDropItems,MAKECREPARAM(&fubenid,&(reward_info->getFubenDropVec())));
			int _type = 0;
			for (;itr_player!=playerMap.end();itr_player++)
			{
				reward_info_copy = new crPlayerRewardData(reward_info.get());
				reward_info_copy->setPlayerID(itr_player->first);
				reward_info_copy->setType(GP_RewardGameEnd);
				playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(itr_player->first));
				if(playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
				{
					playerGameData = playerData->getPlayerGameData();
					//playerGameData->doEvent(WCH_JXJFubenDropItems,MAKECREPARAM(&fubenid,&fubenDropVec));
					//reward_info->setFubenDropItems(fubenDropVec);
					crData *data = playerGameData->getDataClass();
					unsigned char starCount = itr_player->second.starCount;
					unsigned char rewardindex = 0;
					unsigned char oldStarCount = 0;

					int defineid = 0;
					unsigned char delcount = 0;

					if(data)
					{
						unsigned char completecount = 0;
						int chapterid=0;
						int chaptureIdx = fubentab->getTitleIndex("chapterid");
						if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && chaptureIdx>=0)
						{
							chapterid = atoi(record[chaptureIdx].c_str());
						}

						if(win)
						{
							starCount+=1;
							int todoIndex = fubentab->getTitleIndex("完成目标");
							crTableIO::StrVec record;
							crVector3i vecTodo;
							if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && todoIndex>=0)
							{
								crArgumentParser::appAtoVec(record[todoIndex],vecTodo);
							}

							if (vecTodo[0]>0 && itr_player->second.npcCount >= vecTodo[0]) starCount++;
							if (vecTodo[1]>0 && itr_player->second.bossCount >= vecTodo[1]) starCount++;


							if (starCount > 3) starCount = 3;
							//int chapterid=0;

							data->excHandle(MAKECREPARAM(WCH_LockData,1));
							data->getParam(WCHDATA_JXJFubenProgressMap,param);
							FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
							FubenProgressMap::iterator itr_fuben = fubenProgress->find(fubenid);
							bool hasreward = true;
							if(itr_fuben!=fubenProgress->end())
							{
								completecount = itr_fuben->second.get()->getComplete();
								if (completecount > 2 && (chapterid >= 2000 && chapterid < 3000))
								{
									reward_info_copy->getFubenDropVec().clear();
									hasreward = false;
								}
								oldStarCount = itr_fuben->second.get()->getStarCount();
								if(oldStarCount >= starCount)
								{
									rewardindex = 1;
								}
								else
								{
									itr_fuben->second.get()->setStarCount(starCount);
									rewardindex = starCount;
                                    playerGameData->doEvent(WCH_JXJCheck7Day);
								}
								itr_fuben->second.get()->setChapterId(chapterid);
							}
							if (chapterid >= 2000 && chapterid < 3000)
							{
								data->getParam(WCHDATA_JXJPassMingJiangCount,param);
								unsigned short *passcount = (unsigned short *)param;
								*passcount += 1;

								defineid  = WCHDATA_JXJPassMingJiangCount;
								delcount = 1;
							}
							data->excHandle(MAKECREPARAM(WCH_LockData,0));

							if (chapterid >= 1000 && chapterid < 2000)
							{
								_type = 0;	//征战天下活动
								playerGameData->doEvent(WCH_JXJUpdate7DayHuoDong,MAKECREPARAM(_type,NULL));

								//通过征战天下次数记录
								data->excHandle(MAKECREPARAM(WCH_LockData,1));
								data->getParam(WCHDATA_JXJPassZhengZhanTianXiaCount,param);
								unsigned short *passcount = (unsigned short *)param;
								*passcount += 1;
								data->excHandle(MAKECREPARAM(WCH_LockData,0));

								defineid  = WCHDATA_JXJPassZhengZhanTianXiaCount;
								delcount = 1;
							}
							else if ((chapterid >= 2000 && chapterid < 3000) && hasreward)
							{
								_type = 1;	//名将之路活动
								playerGameData->doEvent(WCH_JXJUpdate7DayHuoDong,MAKECREPARAM(_type,NULL));
							}
						}
						else
						{
							if (chapterid >= 1000 && chapterid < 2000)
							{
								data->excHandle(MAKECREPARAM(WCH_LockData,1));
								data->getParam(WCHDATA_JXJZhengZhanTianXiaFailCount,param);
								unsigned char *failcount = (unsigned char *)param;
								*failcount += 1;
								data->excHandle(MAKECREPARAM(WCH_LockData,0));

								defineid  = WCHDATA_JXJZhengZhanTianXiaFailCount;
								delcount = 1;
							}
							else if (chapterid >= 2000 && chapterid < 3000)
							{
								data->excHandle(MAKECREPARAM(WCH_LockData,1));
								data->getParam(WCHDATA_JXJMingJiangFailCount,param);
								unsigned char *failcount = (unsigned char *)param;
								*failcount += 1;
								data->excHandle(MAKECREPARAM(WCH_LockData,0));

								defineid  = WCHDATA_JXJMingJiangFailCount;
								delcount = 1;
							}
						}

						if(rewardindex<=1)
						{
							if(copperIndex>=0)
								reward_info_copy->setCoopercash(atoi(recordVec[rewardindex][copperIndex].c_str()));
							if(expIndex>=0)
								reward_info_copy->setExperience(atoi(recordVec[rewardindex][expIndex].c_str()));
							if(gongxunIndex>=0)
								reward_info_copy->setExploit(atoi(recordVec[rewardindex][gongxunIndex].c_str()));
							if(giftgoldIndex>=0)
								reward_info_copy->setGiftgold(atoi(recordVec[rewardindex][giftgoldIndex].c_str()));
						}
						else
						{
							if(starCount == 2 || (starCount == 3 && oldStarCount == 2))
							{
								if(copperIndex>=0)
									reward_info_copy->setCoopercash(atoi(recordVec[1][copperIndex].c_str())+
									atoi(recordVec[rewardindex][copperIndex].c_str()));
								if(expIndex>=0)
									reward_info_copy->setExperience(atoi(recordVec[1][expIndex].c_str())+
									atoi(recordVec[rewardindex][expIndex].c_str()));
								if(gongxunIndex>=0)
									reward_info_copy->setExploit(atoi(recordVec[1][gongxunIndex].c_str())+
									atoi(recordVec[rewardindex][gongxunIndex].c_str()));
								if(giftgoldIndex>=0)
									reward_info_copy->setGiftgold(atoi(recordVec[1][giftgoldIndex].c_str())+
									atoi(recordVec[rewardindex][giftgoldIndex].c_str()));
							}
							else if(starCount == 3 && oldStarCount<2)
							{
								if(copperIndex>=0)
									reward_info_copy->setCoopercash(atoi(recordVec[1][copperIndex].c_str())+
									atoi(recordVec[2][copperIndex].c_str())+atoi(recordVec[3][copperIndex].c_str()));
								if(expIndex>=0)
									reward_info_copy->setExperience(atoi(recordVec[1][expIndex].c_str())+
									atoi(recordVec[2][expIndex].c_str()) + atoi(recordVec[3][expIndex].c_str()));
								if(gongxunIndex>=0)
									reward_info_copy->setExploit(atoi(recordVec[1][gongxunIndex].c_str())+
									atoi(recordVec[2][gongxunIndex].c_str()) + atoi(recordVec[3][gongxunIndex].c_str()));
								if(giftgoldIndex>=0)
									reward_info_copy->setGiftgold(atoi(recordVec[1][giftgoldIndex].c_str())+
									atoi(recordVec[2][giftgoldIndex].c_str()) + atoi(recordVec[3][giftgoldIndex].c_str()));
							}
						}
						
						if (completecount > 2 && (chapterid >= 2000 && chapterid < 3000))
						{
							reward_info_copy->setCoopercash(0);
							reward_info_copy->setExperience(0);
						}

						time_t t = time(0);
						reward_info_copy->setTimer(t);
						data->inputParam(WCHDATA_JXJFubenRewardInfo,reward_info_copy.get());
					}
					//发包
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					if(win)
					{
						random_shuffle(reward_info_copy->getFubenDropVec().begin(),reward_info_copy->getFubenDropVec().end());

						stream->createBuf(10+reward_info_copy->getFubenDropVec().size()*13);
						stream->_writeBool(win);
						stream->_writeUShort(fubenid);
						stream->_writeInt(defineid);
						stream->_writeUChar(delcount);
						stream->_writeUChar(starCount);
						stream->_writeUChar(reward_info_copy->getFubenDropVec().size());
						for(FubenDropRewardVec::iterator itr = reward_info_copy->getFubenDropVec().begin();itr!=reward_info_copy->getFubenDropVec().end();itr++)
						{
							stream->_writeUChar(itr->first);
							stream->_writeVec3i(itr->second);
						}
					}
					else
					{
						stream->createBuf(3);
						stream->_writeBool(win);
						stream->_writeUShort(fubenid);
						stream->_writeInt(defineid);
						stream->_writeUChar(delcount);
					}
					crPlayerDataEventPacket::buildReplyPacket(packet,itr_player->first,WCH_JXJClientRecvFubenDrop,stream.get());
					gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
				}
				else
				{///玩家不在线，随机一个发邮件
					if(!reward_info_copy->getFubenDropVec().empty())
					{
						CRCore::rangei counrnd;
						counrnd.set(0,reward_info_copy->getFubenDropVec().size());
						int rewardindex = counrnd.get_random();
						int index = 0;
						unsigned char type = reward_info_copy->getFubenDropVec()[rewardindex].first;
						int resCount = 0;
						switch (type)
						{
						case T_Food:
							resCount = reward_info_copy->getFood();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setFood(resCount);
							break;
						case T_Wood:
							resCount = reward_info_copy->getWood();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setWood(resCount);
							break;
						case T_Iron:
							resCount = reward_info_copy->getIron();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setIron(resCount);
							break;
						case T_Horse:
							resCount = reward_info_copy->getHorse();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setHorse(resCount);
							break;
						case T_Copper:
							resCount = reward_info_copy->getCooperCashReward();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setCoopercash(resCount);
							break;
						case T_Exp:
							resCount = reward_info_copy->getExperience();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setExperience(resCount);
							break;
						case T_Exploit:
							resCount = reward_info_copy->getExploit();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setExploit(resCount);
							break;
						case T_GuoZhanJiFen:
							resCount = reward_info_copy->getScore();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setScore(resCount);
							break;
						case T_Achievement:
							resCount = reward_info_copy->getAchievement();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setAchievement(resCount);
							break;
						case T_Giftgold:
							resCount = reward_info_copy->getGiftgold();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setGiftgold(resCount);
							break;
						case T_Contribute:
							resCount = reward_info_copy->getCountryContribution();
							resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
							reward_info_copy->setCountryContribution(resCount);
							break;
						case T_TroopsEquip:
							reward_info_copy->getEquipsVec().push_back(std::make_pair((reward_info_copy->getFubenDropVec()[rewardindex].second)[0],(reward_info_copy->getFubenDropVec()[rewardindex].second)[2]));
							break;
						case T_Item:
							{
								ref_ptr<crBagItemData> itemdata = new crBagItemData;
								itemdata->setItemID((reward_info_copy->getFubenDropVec()[rewardindex].second)[0]);
								itemdata->setEquipMagic((reward_info_copy->getFubenDropVec()[rewardindex].second)[1]);
								itemdata->setItemCount((reward_info_copy->getFubenDropVec()[rewardindex].second)[2]);
								reward_info_copy->getRewardItemsVec().push_back(itemdata);
							}
							break;
						}
					}
					m_this->doEvent(WCH_JXJCreateRewardMail,MAKECREPARAM(reward_info_copy.get(),NULL));
				}
				///游戏日志
				//std::string logdata = crArgumentParser::appItoa(win);
				//GameLogData gamelog(Log_FubenEnd,logdata);
				//crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(*itr_player,&gamelog));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenDropItemsMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenDropItemsMethod::crJXJFubenDropItemsMethod():m_this(NULL),m_fubenid(0)
{

}
crJXJFubenDropItemsMethod::crJXJFubenDropItemsMethod(const crJXJFubenDropItemsMethod& handle):crMethod(handle),m_this(NULL),m_fubenid(handle.m_fubenid)
{

}
void crJXJFubenDropItemsMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
			m_fubenid = 0;
			m_dropVec=NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_fubenid = *(unsigned short *)LOCREPARAM(param64);
			m_dropVec = (FubenDropRewardVec *)HICREPARAM(param64);
		}
		else
		{
			m_fubenid = 0;
			m_dropVec = NULL;
		}
		break;
	}
}
void crJXJFubenDropItemsMethod::addParam(int i, const std::string& str)
{

}
void crJXJFubenDropItemsMethod::operator ()(crHandle &handle)
{
	if(m_this && m_fubenid>0 && m_dropVec)
	{
		//m_dropVec->resize(0);
		ref_ptr<crTableIO> dropTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenDropTab);
		if(dropTab.valid())
		{
			crTableIO::DataVec recordVec;
			dropTab->queryRecords(0,crArgumentParser::appItoa(m_fubenid),recordVec);
			if(!recordVec.empty())
			{
				int dropcountid = dropTab->getTitleIndex("掉落种数");
				int droptypeid = dropTab->getTitleIndex("掉落类型");
				int dropidindex = dropTab->getTitleIndex("dropid");
				int oddsid = dropTab->getTitleIndex("掉落几率");
				int countid = dropTab->getTitleIndex("数量");
				int dropcount = atoi(recordVec[0][dropcountid].c_str());
				int packetRemainSize = 0;
				int odds = 0;
				unsigned char droptype;
				int count;
				crVector2i countrange;
				CRCore::rangei counrnd;

				//RewardEquipsVec DropedEquipVec;//equipid,count
				//RewardItemVec DropedItemVec;//itemid,count
				int recordCount = recordVec.size();
				std::vector< std::pair<int,int> >DroprndVec;
				int i,j,k;
				int dropodds;
				for (i = 0; i < recordCount; i++)
				{
					dropodds = atoi(recordVec[i][oddsid].c_str());
					odds += dropodds;
					DroprndVec.push_back(std::make_pair(odds,dropodds));
				}
				crVector2i vec2;
				rangei rndi(0,odds);
				int r;
				for(i = 0; i<dropcount;i++)
				{
					r = rndi.get_random();
					for(j = 0; j<recordCount; j++)
					{
						if(r<=DroprndVec[j].first)
						{//该物品掉出
							DroprndVec[j].first = -1;
							odds -= DroprndVec[j].second;
							rndi.set(0,odds);
							for (k = j+1; k < recordCount; k++)
							{
								DroprndVec[k].first -= DroprndVec[j].second;
							}
							crArgumentParser::appAtoVec(recordVec[j][countid],countrange);
							counrnd.set(countrange[0],countrange[1]);
							count = counrnd.get_random();
							if(count>0)
							{
								droptype = atoi(recordVec[j][droptypeid].c_str());
								switch (droptype)
								{
								case T_Food:
									m_dropVec->push_back(std::make_pair(T_Food,crVector3i(WCHDATA_JXJFood,0,count)));
									break;
								case T_Wood:
									m_dropVec->push_back(std::make_pair(T_Wood,crVector3i(WCHDATA_JXJWood,0,count)));
									break;
								case T_Iron:
									m_dropVec->push_back(std::make_pair(T_Iron,crVector3i(WCHDATA_JXJIron,0,count)));
									break;
								case T_Horse:
									m_dropVec->push_back(std::make_pair(T_Horse,crVector3i(WCHDATA_JXJHorse,0,count)));
									break;
								case T_Copper:
									m_dropVec->push_back(make_pair(T_Copper,crVector3i(WCHDATA_JXJCoppercash,0,count)));
									break;
								case T_Exp:
									m_dropVec->push_back(make_pair(T_Exp,crVector3i(WCHDATA_Experience,0,count)));
									break;
								case T_Achievement:
									m_dropVec->push_back(make_pair(T_Achievement,crVector3i(WCHDATA_JXJAchievement,0,count)));
									break;
								case T_Exploit:
									m_dropVec->push_back(make_pair(T_Exploit,crVector3i(WCHDATA_JXJExploit,0,count)));
									break;
								case T_GuoZhanJiFen:
									m_dropVec->push_back(make_pair(T_GuoZhanJiFen,crVector3i(WCHDATA_JXJGuoZhanJiFen,0,count)));
									break;
								case T_Giftgold:
									m_dropVec->push_back(make_pair(T_Giftgold,crVector3i(WCHDATA_JXJGiftGold,0,count)));
									break;
								case T_Contribute:
									m_dropVec->push_back(make_pair(T_Contribute,crVector3i(WCHDATA_JXJCountryContribute,0,count)));
									break;
								case T_TroopsEquip:
									m_dropVec->push_back(make_pair(T_TroopsEquip,crVector3i(atoi(recordVec[j][dropidindex].c_str()),0,count)));
									break;
								case T_Item:
									{
										crArgumentParser::appAtoVec(recordVec[j][dropidindex],vec2);
										m_dropVec->push_back(make_pair(T_Item,crVector3i(vec2[0],vec2[1],count)));//itemid,equipmagic,count
									}
									break;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvFubenDropMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvFubenDropMethod::crJXJClientRecvFubenDropMethod()
{

}
crJXJClientRecvFubenDropMethod::crJXJClientRecvFubenDropMethod(const crJXJClientRecvFubenDropMethod& handle)
{

}
void crJXJClientRecvFubenDropMethod::inputParam(int i, void *param)
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
void crJXJClientRecvFubenDropMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}
void crJXJClientRecvFubenDropMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void * param;
			ref_ptr<crTableIO> fubenRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
			int exp = 0;
			int copper = 0;
			int giftGold = 0;
			int exploit = 0;
			crTableIO::DataVec recordVec;
			FubenDropRewardVec dropVec;
			int copperIndex = fubenRewardTab->getTitleIndex("铜钱");
			int expIndex = fubenRewardTab->getTitleIndex("经验");
			int gongxunIndex = fubenRewardTab->getTitleIndex("功勋");
			int giftgoldIndex = fubenRewardTab->getTitleIndex("礼券");

			bool win = m_stream->_readBool();
			unsigned short fubenid = m_stream->_readUShort();
			int defineid = m_stream->_readInt();
			unsigned char delcount = m_stream->_readUChar();
			unsigned char rewardIndex = 0;
			unsigned char starCount = 0;
			unsigned char oldStarCount = 0;
			if(win)
			{
				unsigned char completecount;
				crData *playerData = m_this->getDataClass();
				playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
				FubenProgressMap::iterator itr_fuben = fubenProgress->find(fubenid);
				if(itr_fuben!=fubenProgress->end())
					completecount = itr_fuben->second.get()->getComplete();

				starCount = m_stream->_readUChar();
				int size = m_stream->_readUChar();
				crVector3i vec3i;
				unsigned char type;
				for (int i = 0;i<size;i++)
				{
					type = m_stream->_readUChar();
					vec3i = m_stream->_readVec3i();
					//if(!(completecount > 2 && fubenid >= 2000))
					  dropVec.push_back(make_pair(type,vec3i));
				}
				
				if(itr_fuben!=fubenProgress->end())
				{
					oldStarCount = itr_fuben->second.get()->getStarCount();
					if(oldStarCount >= starCount)
					{
						rewardIndex = 1;
					}
					else
					{
						itr_fuben->second.get()->setStarCount(starCount);
						rewardIndex = starCount;
					}
				}

				/*if (fubenid >= 2000 && fubenid < 3000)
				{
				playerData->getParam(WCHDATA_JXJPassMingJiangCount,param);
				unsigned short *passcount = (unsigned short *)param;
				*passcount += 1;
				}*/

				if (defineid > 0)
				{
					playerData->getParam(defineid,param);
					unsigned short *passcount = (unsigned short *)param;
					*passcount += (unsigned short)delcount;
				}

				playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
			else
			{
				if (defineid > 0)
				{
					crData *playerData = m_this->getDataClass();
					playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
					playerData->getParam(defineid,param);
					unsigned char *failcount = (unsigned char *)param;
					*failcount += (unsigned char)delcount;;
					playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}

			if(fubenRewardTab.valid())
			{
				fubenRewardTab->queryRecords(0,crArgumentParser::appItoa(fubenid),recordVec);
				if(!recordVec.empty())
				{
					if(rewardIndex<=1)
					{
						if(copperIndex>=0)
							copper = atoi(recordVec[rewardIndex][copperIndex].c_str());
						if(expIndex>=0)
							exp = atoi(recordVec[rewardIndex][expIndex].c_str());
						if(gongxunIndex>=0)
							exploit = atoi(recordVec[rewardIndex][gongxunIndex].c_str());
						if(giftgoldIndex>=0)
							giftGold = atoi(recordVec[rewardIndex][giftgoldIndex].c_str());
					}
					else
					{
						if(starCount == 2 || (starCount == 3 && oldStarCount == 2))
						{
							if(copperIndex>=0)
								copper = atoi(recordVec[1][copperIndex].c_str())+
								atoi(recordVec[rewardIndex][copperIndex].c_str());
							if(expIndex>=0)
								exp = atoi(recordVec[1][expIndex].c_str())+
								atoi(recordVec[rewardIndex][expIndex].c_str());
							if(gongxunIndex>=0)
								exploit = atoi(recordVec[1][gongxunIndex].c_str())+
								atoi(recordVec[rewardIndex][gongxunIndex].c_str());
							if(giftgoldIndex>=0)
								giftGold = atoi(recordVec[1][giftgoldIndex].c_str())+
								atoi(recordVec[rewardIndex][giftgoldIndex].c_str());
						}
						else if(starCount == 3 && oldStarCount<2)
						{
							if(copperIndex>=0)
								copper = atoi(recordVec[1][copperIndex].c_str())+
								atoi(recordVec[2][copperIndex].c_str())+\
								atoi(recordVec[3][copperIndex].c_str());
							if(expIndex>=0)
								exp = atoi(recordVec[1][expIndex].c_str())+
								atoi(recordVec[2][expIndex].c_str())+\
								atoi(recordVec[3][expIndex].c_str());
							if(gongxunIndex>=0)
								exploit = atoi(recordVec[1][gongxunIndex].c_str())+
								atoi(recordVec[2][gongxunIndex].c_str())+\
								atoi(recordVec[3][gongxunIndex].c_str());
							if(giftgoldIndex>=0)
								giftGold = atoi(recordVec[1][giftgoldIndex].c_str())+
								atoi(recordVec[2][giftgoldIndex].c_str())+\
								atoi(recordVec[3][giftgoldIndex].c_str());
						}
					}
				}
			}
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crData *data = canvas->getDataClass();
				if(data)
				{
					data->inputParam(WCHDATA_JXJFubenRewardExp,&exp);
					data->inputParam(WCHDATA_JXJFubenRewardGold,&giftGold);
					data->inputParam(WCHDATA_JXJFubenRewardCopper,&copper);
					data->inputParam(WCHDATA_JXJFubenRewardExploit,&exploit);
					data->inputParam(WCHDATA_JXJFubenRewardDropVec,&dropVec);
					data->inputParam(WCHDATA_JXJFubenStarCount,&starCount);
					data->inputParam(WCHDATA_JXJFubenIsWin,&win);
				}
				crFilterRenderManager::getInstance()->doModal(m_canvas);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryFailOnShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryFailOnShowMethod::crJXJFubenVictoryFailOnShowMethod()
{

}
crJXJFubenVictoryFailOnShowMethod::crJXJFubenVictoryFailOnShowMethod(const crJXJFubenVictoryFailOnShowMethod& handle)
{

}
void crJXJFubenVictoryFailOnShowMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}
void crJXJFubenVictoryFailOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_switch = str;
		break;
	case 1:
		m_exp = str;
		break;
	case 2:
		m_copper = str;
		break;
	case 3:
		m_stars = str;
		break;
	case 4:
		m_canvasSwitch = str;
		break;
	case 5:
		m_button = str;
		break;
	case 6:
		m_strExploit = str;
		break;
	case 7:
		m_strGiftgold = str;
		break;
	case 8:
		m_strCanvas = str;
		break;
	case 9:
		m_strStarSwitch = str;
		break;
	case 10:
		m_strStarInput = str;
		break;
	case 11:
		m_strStarSeq = str;
		break;
	case 12:
		m_strSeq[0] =str;
		break;
	case 13:
		m_strSeq[1] = str;
		break;
	case 14:
		m_strSeq[2] = str;
		break;
	case 15:
		m_strBtSw = str;
		break;
	case 16:
		m_strbtDouble2 = str;
		break;
	case 17:
		m_strbtTriple = str;
		break;
	case 18:
		m_strbtDouble3 = str;
		break;
	case 19:
		m_strbtTriple2 = str;
		break;
	case 20:
		m_strFourfold = str;
		break;
	default:
		break;
	}
}
void crJXJFubenVictoryFailOnShowMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		//unsigned short fubenid = 0;
		void *param;
		ref_ptr<crMultiSwitch>  canvasSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_canvasSwitch));
		ref_ptr<crMultiSwitch>  winSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch));
		ref_ptr<crMultiSwitch>  starSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_stars));
		ref_ptr<crStaticTextWidgetNode>  expInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_exp));
		ref_ptr<crStaticTextWidgetNode>  copperInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_copper));
		ref_ptr<crStaticTextWidgetNode>  giftgoldInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strGiftgold));
		ref_ptr<crStaticTextWidgetNode>  exploitInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strExploit));
		ref_ptr<crButtonWidgetNode>  button = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_button));
		ref_ptr<crMultiSwitch> btSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strBtSw));
		ref_ptr<crButtonWidgetNode> btDouble2 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strbtDouble2));
		ref_ptr<crButtonWidgetNode> btTriple = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strbtTriple));
		ref_ptr<crButtonWidgetNode> btDouble3 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strbtDouble3));
		ref_ptr<crButtonWidgetNode> btTriple2 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strbtTriple2));
		ref_ptr<crButtonWidgetNode> btFourfold = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strFourfold));

		if(!winSwitch || !expInput || !copperInput || !canvasSwitch || !button) return;

		//双倍按钮禁用
		//button->setEnable(false);
		crPlayerGameData *playGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 
		int timer;
		unsigned char completecount = 0;
		unsigned short fubenid;
		int chapterid = 0;
		if(playGameData)
		{
			
			crData *data = playGameData->getDataClass();
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			int doubleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenDoubleNeedGold, vipLevel).c_str());
			int tripleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenTripleNeedGold, vipLevel).c_str());
			int fourfoldNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenFourfoldNeedGold, vipLevel).c_str());
			timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenCanvasAutoCloseTime, vipLevel).c_str());
			data->getParam(WCHDATA_JXJGoldingot,param);
			int mygold = *(int *)param;
			data->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;
			mygold += giftgold;

			data->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		    fubenid = *(unsigned short*)param;

			crTableIO::StrVec record;
			ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
			if (fubentab.valid())
			{
				int chaptureIdx = fubentab->getTitleIndex("chapterid");
				if(fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0 && chaptureIdx>=0)
				{
					chapterid = atoi(record[chaptureIdx].c_str());
				}

			}
			if (chapterid >= 2000 && chapterid < 3000)
			{
				data->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJFubenProgressMap,param);
				FubenProgressMap *fubenProgress = (FubenProgressMap *)param;
				FubenProgressMap::iterator itr_fuben = fubenProgress->find(fubenid);
				if(itr_fuben!=fubenProgress->end())
				    completecount = itr_fuben->second.get()->getComplete();
				
				data->excHandle(MAKECREPARAM(WCH_LockData,0));
			}

			////关闭双倍获取
			int index = -1;
			if(doubleNeedGold>0) index++;
			if(tripleNeedGold>0) index++;
			if(fourfoldNeedGold>0) index++;

			if(btSwitch.valid()) 
			{
				if (chapterid >= 2000 && chapterid < 3000 && index != -1 )
					btSwitch->setActiveSwitchSet(index);
				else
					btSwitch->setActiveSwitchSet(3);
			}

			if(mygold>doubleNeedGold)
			{
				button->setEnable(true);
				if(btDouble2.valid())
					btDouble2->setEnable(true);
				if(btDouble3.valid())
					btDouble3->setEnable(true);
			}
			else
			{
				button->setEnable(false);
				if(btDouble2.valid())
					btDouble2->setEnable(false);
				if(btDouble3.valid())
					btDouble3->setEnable(false);
			}

			if(mygold>tripleNeedGold)
			{
				if(btTriple.valid())
					btTriple->setEnable(true);
				if(btTriple2.valid())
					btTriple2->setEnable(true);
			}
			else
			{
				if(btTriple.valid())
					btTriple->setEnable(false);
				if(btTriple2.valid())
					btTriple2->setEnable(false);
			}

			if(mygold>fourfoldNeedGold)
			{
				if(btFourfold.valid())
					btFourfold->setEnable(true);
			}
			else
			{
				if(btFourfold.valid())
					btFourfold->setEnable(false);
			}
			//data->getParam(WCHDATA_JXJFubenID,param);
			//fubenid = *(unsigned short *)param;
		}
		// 		crTableIO::StrVec record;
		// 		unsigned char canReliveCount = 0;
		// 		ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
		// 		int index = fubentab->getTitleIndex("允许补兵次数");
		// 		if(fubenid>0 && fubentab && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record))
		// 		{
		// 			if(index>0)
		// 				canReliveCount = atoi(record[index].c_str());
		// 		}			
		crData *data = m_this->getDataClass();
		if(data)
		{
			data->getParam(WCHDATA_JXJFubenIsWin,param);
			bool isWin = *(bool *)param;
			data->getParam(WCHDATA_JXJFubenRewardExp,param);
			int exp = *(int *)param;
			data->getParam(WCHDATA_JXJFubenRewardGold,param);
			int gold = *(int *)param;
			data->getParam(WCHDATA_JXJFubenRewardCopper,param);
			int copper = *(int *)param;
			data->getParam(WCHDATA_JXJFubenRewardExploit,param);
			int exploit = *(int *)param;

			data->getParam(WCHDATA_JXJFubenStarCount,param);
			unsigned char starCount = *(unsigned char *)param;

			if(isWin)
			{
				winSwitch->setActiveSwitchSet(0);
				canvasSwitch->setActiveSwitchSet(0);
				if(starCount > 0)
				{
					starSwitch->setActiveSwitchSet(starCount);
				}

				if(!(completecount > 2 && (chapterid >= 2000 && chapterid < 3000)))
				{
					//canvasSwitch->setActiveSwitchSet(1);
					crSequence *seq = NULL;
					for (int i = 0;i<3;i++)
					{
						seq = dynamic_cast<crSequence *>(m_this->getWidget(m_strSeq[i]));
						if(seq)
							seq->startEffect();
					}
				}
			}
			else
			{
				winSwitch->setActiveSwitchSet(1);
				canvasSwitch->setActiveSwitchSet(2);
				starSwitch->setActiveSwitchSet(0);
			}

			if(completecount > 2 && (chapterid >= 2000 && chapterid < 3000))
			{
				exp = 0;
				copper = 0;
			}
			expInput->setString(crArgumentParser::appItoa(exp));
			copperInput->setString(crArgumentParser::appItoa(copper));
			if(giftgoldInput.valid())
				giftgoldInput->setString(crArgumentParser::appItoa(gold));
			if(exploitInput.valid())
				exploitInput->setString(crArgumentParser::appItoa(exploit));
			data->inputParam(WCHDATA_JXJFubenAutoCloseTimer,&timer);

			if(completecount > 2 && fubenid >= 2000)
				canvasSwitch->setActiveSwitchSet(2);

			//亮星星
			ref_ptr<crCanvasNode> starCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			if(starCanvas.valid()) crFilterRenderManager::getInstance()->closeCanvas(starCanvas.get());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryChooseRewardItemsMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryChooseRewardItemsMethod::crJXJFubenVictoryChooseRewardItemsMethod():m_index(0)
{

}
crJXJFubenVictoryChooseRewardItemsMethod::crJXJFubenVictoryChooseRewardItemsMethod(const crJXJFubenVictoryChooseRewardItemsMethod& handle):
	crMethod(handle),m_index(0)
{

}
void crJXJFubenVictoryChooseRewardItemsMethod::inputParam(int i, void *param)
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
void crJXJFubenVictoryChooseRewardItemsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	case 1:
		m_strswitch = str;
		break;
	case 2:
		m_stricon[0] = str;
		break;
	case 3:
		m_stricon[1] = str;
		break;
	case 4:
		m_stricon[2] = str;
		break;
	case 5:
		m_strseq[0] = str;
		break;
	case 6:
		m_strseq[1] = str;
		break;
	case 7:
		m_strseq[2] = str;
		break;	
	case 8:
		m_strinput[0] = str;
		break;
	case 9:
		m_strinput[1] = str;
		break;
	case 10:
		m_strinput[2] = str;
		break;
	case 11:
		m_strfoodIcon = str;
		break;
	case 12:
		m_strwoodIcon = str;
		break;
	case 13:
		m_strironIcon = str;
		break;
	case 14:
		m_strhorseIcon = str;
		break;
	case 15:
		m_strcopperIcon = str;
		break;
	case 16:
		m_strexpIcon = str;
		break;
	case 17:
		m_strgongxunIcon = str;
		break;
	case 18:
		m_strgiftgoldIcon = str;
		break;
	case 19:
		m_strgongxianIcon = str;
		break;
	case 20:
		m_strzhangongIcon = str;
		break;
	}
}
void crJXJFubenVictoryChooseRewardItemsMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();

		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec itemrecord,colorrecord;
		crVector4 color = COLOR_GRAY;
		if(canvas.valid())
		{

			ref_ptr<crMultiSwitch>  canvasSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strswitch));
			if(canvasSwitch.valid())
				canvasSwitch->setActiveSwitchSet(1);

			ref_ptr<crImageBoxWidgetNode> icon[3];
			ref_ptr<crStaticTextWidgetNode> input[3];
			ref_ptr<crSequence> seq[3];

			for ( int i = 0;i<3;i++)
			{
				icon[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_stricon[i]));
				input[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strinput[i]));
				seq[i] = dynamic_cast<crSequence *>(canvas->getChildNode(m_strseq[i]));
			}

			std::string iconName;
			std::string itemName;
			crData *canvasData = canvas->getDataClass();
			if(canvasData)
			{
				canvasData->getParam(WCHDATA_JXJFubenRewardDropVec,param);
				FubenDropRewardVec fubenDropItems = *(FubenDropRewardVec *)param;
				FubenDropRewardVec::iterator itr = fubenDropItems.begin();
				int index = 0;
				for (;itr!=fubenDropItems.end() && index<3;itr++,index++)
				{
					if(itr->first == T_Item || itr->first == T_TroopsEquip)
					{
						if(itemtab.valid())
						{
							int iconIndex = itemtab->getTitleIndex("icon");
							int nameIndex = itemtab->getTitleIndex("name");
							int qualityindex = itemtab->getTitleIndex("品质");
							int colorIndex = qualitytab->getTitleIndex("goodscolor");
							if (itemtab->queryOneRecord(0,crArgumentParser::appItoa((itr->second)[0]),itemrecord)>=0)
							{
								if(iconIndex>=0)
									iconName = itemrecord[iconIndex];
								if(nameIndex>=0)
									itemName = itemrecord[nameIndex];

								if(qualityindex>=0)
								{
									if (qualitytab->queryOneRecord(0,itemrecord[qualityindex],colorrecord)>=0)
									{
										if (colorIndex>=0)
										{
											crArgumentParser::appAtoVec(colorrecord[colorIndex],color);
										}
									}
								}

							}
						}
						//canvasData->inputParam(WCHDATA_JXJFubenDropItemChooseId,&((itr->second)[0]));
					}
					if(icon[index].valid())
					{
						icon[index]->setEnable(true);
						if(!iconName.empty())
						{
							icon[index]->setImageName(iconName);
						}
						else
						{
							switch (itr->first)
							{
							case T_Copper:
								icon[index]->setImageName(m_strcopperIcon);
								itemName = std::string("铜钱");
								break;
							case T_Wood:
								icon[index]->setImageName(m_strwoodIcon);
								itemName = std::string("木材");
								break;
							case T_Iron:
								icon[index]->setImageName(m_strironIcon);
								itemName = std::string("铁矿");
								break;
							case T_Horse:
								icon[index]->setImageName(m_strhorseIcon);
								itemName = std::string("马匹");
								break;
							case T_Food:
								icon[index]->setImageName(m_strfoodIcon);
								itemName = std::string("粮草");
								break;
							case T_Exp:
								icon[index]->setImageName(m_strexpIcon);
								itemName = std::string("经验");
								break;
							case T_Exploit:
								icon[index]->setImageName(m_strgongxunIcon);
								itemName = std::string("功勋");
								break;
							case T_Achievement:
								icon[index]->setImageName(m_strzhangongIcon);
								itemName = std::string("战功");
								break;
							case T_Contribute:
								icon[index]->setImageName(m_strgongxianIcon);
								itemName = std::string("国家贡献");
								break;
							}
						}
					}

					if(index == m_index && seq[index].valid())
					{
						seq[index]->setVisiable(true);
						seq[index]->startEffect();
					}
					else
					{
						seq[index]->setVanishWhenStop(true);
						seq[index]->stopEffect();
					}

					if(input[index].valid())
					{
						input[index]->setColor(color/255.0f);
						input[index]->setString(itemName + std::string("*")+crArgumentParser::appItoa((itr->second)[2]));
					}
					canvasData->inputParam(WCHDATA_JXJFubenDropChooseIndex,&m_index);
				}
				
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryDoubleTipsShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryDoubleTipsShowMethod::crJXJFubenVictoryDoubleTipsShowMethod()
{

}
crJXJFubenVictoryDoubleTipsShowMethod::crJXJFubenVictoryDoubleTipsShowMethod(const crJXJFubenVictoryDoubleTipsShowMethod& handle)
{

}
void crJXJFubenVictoryDoubleTipsShowMethod::inputParam(int i, void *param)
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
void crJXJFubenVictoryDoubleTipsShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_strInput = str;
		break;
	case 2:
		m_index = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJFubenVictoryDoubleTipsShowMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		const crBoundingBox &bbox = m_this->getBoundBox();
		ref_ptr<crCanvasNode>  tipCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(tipCanvas.valid())
		{
			crHypertextWidgetNode * hyperText = dynamic_cast<crHypertextWidgetNode *>(tipCanvas->getWidget(m_strInput));
			if(hyperText)
			{
				crPlayerGameData *playGameData = crMyPlayerData::getInstance()->getPlayerGameData(); 

				if(playGameData)
				{
					void *param;
					crData *data = playGameData->getDataClass();
					data->getParam(WCHDATA_JXJVipLv,param);
					unsigned char vipLevel = *(unsigned char *)param;

					int doubleNeedLv = -1;
					int tripleNeedLv = -1;
					int fourfoldNeedLv = -1;

					for(int i = 0;i<16;i++)
					{
						if(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenDoubleNeedGold, vipLevel).c_str())>0 && doubleNeedLv<0)
						{
							doubleNeedLv = i;
						}
						if(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenTripleNeedGold, vipLevel).c_str())>0 && tripleNeedLv<0)
						{
							tripleNeedLv = i;
						}
						if(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenFourfoldNeedGold, vipLevel).c_str())>0 && fourfoldNeedLv<0)
						{
							fourfoldNeedLv = i;
							break;
						}
					}
					

					int doubleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenDoubleNeedGold, vipLevel).c_str());
					int tripleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenTripleNeedGold, vipLevel).c_str());
					int fourfoldNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenFourfoldNeedGold, vipLevel).c_str());

					string strGold;
					string strNeedLv;
					switch (m_index)
					{
					case 2:
						if(doubleNeedGold>0)
							strGold = crArgumentParser::appItoa(doubleNeedGold)+string("礼金/元宝");
						else
							strNeedLv = string("VIP")+crArgumentParser::appItoa(doubleNeedLv);
						break;
					case 3:
						if(tripleNeedGold>0)
							strGold = crArgumentParser::appItoa(tripleNeedGold)+string("礼金/元宝");
						else
							strNeedLv = string("VIP")+crArgumentParser::appItoa(tripleNeedGold);
						break;
					case 4:
						if(fourfoldNeedGold>0)
							strGold = crArgumentParser::appItoa(fourfoldNeedGold)+string("礼金/元宝");
						else
							strNeedLv = string("VIP")+crArgumentParser::appItoa(fourfoldNeedGold);
						break;
					}

					std::vector<float>v_i;
					ItemMap itemMap;
					rcfg::ConfigScript cfg_script(&itemMap);
					int index = 1;
					if(!strGold.empty())
					{
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");
						cfg_script.Add("Content");
						cfg_script.Push("Content",index++);
						v_i.clear();
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						cfg_script.Add("Text","花费");
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",index++);
						v_i.clear();
						v_i.push_back(140.0);
						v_i.push_back(40.0);
						v_i.push_back(40.0);
						v_i.push_back(255.0);
						cfg_script.Add("Color",v_i);
						cfg_script.Add("Text",strGold);
						cfg_script.Pop();

						cfg_script.Add("Content");
						cfg_script.Push("Content",index++);
						v_i.clear();
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						v_i.push_back(255.0);
						cfg_script.Add("Text",string("即可多倍获得奖励物品"));
						cfg_script.Pop();
						cfg_script.Pop();

						hyperText->setHypertext(cfg_script);
					}
					else
					{
						if(!strNeedLv.empty())
						{
							cfg_script.Add("Hypertext");
							cfg_script.Push("Hypertext");
							cfg_script.Add("Content");
							cfg_script.Push("Content",1);
							v_i.clear();
							v_i.push_back(255.0);
							v_i.push_back(255.0);
							v_i.push_back(255.0);
							v_i.push_back(255.0);
							cfg_script.Add("Text","多倍获得奖励物品需");
							cfg_script.Pop();

							cfg_script.Add("Content");
							cfg_script.Push("Content",2);
							v_i.clear();
							v_i.push_back(255.0);
							v_i.push_back(255.0);
							v_i.push_back(0.0);
							v_i.push_back(255.0);
							cfg_script.Add("Color",v_i);
							cfg_script.Add("Text",strNeedLv);
							cfg_script.Pop();

							hyperText->setHypertext(cfg_script);
						}
					}

					tipCanvas->setCanvasPos(CANVAS_TOPMOST);
					crFilterRenderManager::getInstance()->showCanvas(tipCanvas.get(),true);
					tipCanvas->setMatrix(crMatrix::translate(bbox.xMax(),bbox.yMax(),0.0f));
				}
			}
		}	
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryFailCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryFailCanvasUpdateMethod::crJXJFubenVictoryFailCanvasUpdateMethod()
{
}
crJXJFubenVictoryFailCanvasUpdateMethod::crJXJFubenVictoryFailCanvasUpdateMethod(const crJXJFubenVictoryFailCanvasUpdateMethod& handle)
{

}
void crJXJFubenVictoryFailCanvasUpdateMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}
void crJXJFubenVictoryFailCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_input = str;
		break;
	default:
		break;
	}
}
void crJXJFubenVictoryFailCanvasUpdateMethod::operator ()(crHandle &handle)
{
	void *param;
	bool isClose = false;
	if(m_this)
	{
		crData *data = m_this->getDataClass();
		ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input));
		if(data)
		{
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJFubenAutoCloseTimer,param);
			int *time = (int *)param;
			if(input.valid())
				input->setString(crArgumentParser::appItoa(*time));
			(*time)--;
			if(*time < 0)
				isClose = true;
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
	handle.outputParam(0,&isClose);
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryFailCanvasCloseMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryFailCanvasCloseMethod::crJXJFubenVictoryFailCanvasCloseMethod()
{

}
crJXJFubenVictoryFailCanvasCloseMethod::crJXJFubenVictoryFailCanvasCloseMethod(const crJXJFubenVictoryFailCanvasCloseMethod& handle)
{

}
void crJXJFubenVictoryFailCanvasCloseMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}
void crJXJFubenVictoryFailCanvasCloseMethod::addParam(int i, const std::string& str)
{

}
void crJXJFubenVictoryFailCanvasCloseMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		crData *canvasData = m_this->getDataClass();
		void *param;
		if(canvasData)
		{
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
			if(playerGameData)
			{
				canvasData->getParam(WCHDATA_JXJFubenDropChooseIndex,param);
				unsigned char index = *(unsigned char *)param;
				canvasData->getParam(WCHDATA_JXJFubenDropOverTimes,param);
				unsigned char overTimes = *(unsigned char *)param;
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(2);
				stream->_writeUChar(index);
				stream->_writeUChar(overTimes);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvClientChooseFubenDrop,stream.get());
				if(netConductor)
					netConductor->getNetManager()->sendPacket("all",packet);
			}
			canvasData->inputParam(WCHDATA_JXJFubenDropChooseIndex,NULL);
			canvasData->inputParam(WCHDATA_JXJFubenDropOverTimes,NULL);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenVictoryDropItemDoubleMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenVictoryDropItemDoubleMethod::crJXJFubenVictoryDropItemDoubleMethod():m_Index(1)
{

}
crJXJFubenVictoryDropItemDoubleMethod::crJXJFubenVictoryDropItemDoubleMethod(const crJXJFubenVictoryDropItemDoubleMethod& handle):crMethod(handle),m_Index(handle.m_Index)
{

}
void crJXJFubenVictoryDropItemDoubleMethod::inputParam(int i, void *param)
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
void crJXJFubenVictoryDropItemDoubleMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_Index = (unsigned char)atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJFubenVictoryDropItemDoubleMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  canvas = m_this->getParentCanvas();
		if(canvas.valid())
		{
			crData *data = canvas->getDataClass();
			if(data)
			{
				//unsigned char overTimes = true;
				if(m_Index<=0) m_Index = 1;
				data->inputParam(WCHDATA_JXJFubenDropOverTimes,&m_Index);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvClientChooseFubenDropMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvClientChooseFubenDropMethod::crJXJRecvClientChooseFubenDropMethod()
{

}
crJXJRecvClientChooseFubenDropMethod::crJXJRecvClientChooseFubenDropMethod(const crJXJRecvClientChooseFubenDropMethod& handle)
{

}
void crJXJRecvClientChooseFubenDropMethod::inputParam(int i, void *param)
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
void crJXJRecvClientChooseFubenDropMethod::addParam(int i, const std::string& str)
{
}
void crJXJRecvClientChooseFubenDropMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			crData *playerData = m_this->getDataClass();
			int playerid = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			ref_ptr<crGameServerPlayerData> playerGameData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
			if(playerData && playerGameData.valid())
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				int doubleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenDoubleNeedGold, vipLevel).c_str());
				int tripleNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenTripleNeedGold, vipLevel).c_str());
				int fourfoldNeedGold = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJFubenFourfoldNeedGold, vipLevel).c_str());
				unsigned char chooseindex = m_stream->_readUChar();

				unsigned char overTimes = m_stream->_readUChar();
				//isDouble = false;//关闭双倍获取
				bool needdeductgold = false;
				playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJFubenRewardInfo,param);
				ref_ptr<crPlayerRewardData> rewardData = (crPlayerRewardData*)param;
				playerData->getParam(WCHDATA_JXJGoldingot,param);
				int *mygold = (int *)param;
				playerData->getParam(WCHDATA_JXJGiftGold,param);
				int *giftgold = (int*)param;

				unsigned char type = 0;
				int getcount = 0;
				crVector3i fubenDrop;
				int needGold = 0;
				if(rewardData.valid())
				{
					FubenDropRewardVec& dropItemVec = rewardData->getFubenDropVec();
					FubenDropRewardVec::iterator itr = dropItemVec.begin();
					RewardItemVec rewardItems = rewardData->getRewardItemsVec();
					RewardEquipsVec rewardEquips = rewardData->getEquipsVec();
					int index = 0;
					for (;itr!=dropItemVec.end();itr++,index++)
					{
						if(index!=chooseindex) continue;
						int doubleindex = overTimes;
						if(overTimes == 2) needGold = doubleNeedGold;
						else if(overTimes == 3) needGold = tripleNeedGold;
						else if(overTimes == 4) needGold = fourfoldNeedGold;
						if(needGold<=0 || *mygold+*giftgold <needGold )
							doubleindex = 1;
						if(doubleindex >= 2 && needGold>0)
						{
							MoneyChangeData moneydata(needGold,"副本双倍");
							m_this->doEvent(WCH_JXJDeductLijinGold,MAKECREPARAM(&moneydata,&needdeductgold));

							////消耗元宝增加成长值
							//int goldToVipExpFactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJGoldToVipExpFactor, vipLevel).c_str());
							//m_this->doEvent(WCH_JXJAddVipExp,MAKECREPARAM(needGold * goldToVipExpFactor,NULL));
						}
						type = itr->first;
						fubenDrop = itr->second;
						getcount = (itr->second)[2]*doubleindex;
						int resCount = 0;
						switch (type)
						{
						case T_Food:
							resCount = rewardData->getFood();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setFood(resCount);
							break;
						case T_Wood:
							resCount = rewardData->getWood();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setWood(resCount);
							break;
						case T_Iron:
							resCount = rewardData->getIron();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setIron(resCount);
							break;
						case T_Horse:
							resCount = rewardData->getHorse();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setHorse(resCount);
							break;
						case T_Copper:
							resCount = rewardData->getCooperCashReward();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setCoopercash(resCount);
							break;
						case T_Exp:
							resCount = rewardData->getExperience();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setExperience(resCount);
							break;
						case T_Achievement:
							resCount = rewardData->getAchievement();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setAchievement(resCount);
							break;
						case T_Exploit:
							resCount = rewardData->getExploit();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setExploit(resCount);
							break;
						case T_GuoZhanJiFen:
							resCount = rewardData->getScore();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setScore(resCount);
							break;
						case T_Giftgold:
							resCount = rewardData->getGiftgold();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setGiftgold(resCount);
							break;
						case T_Contribute:
							resCount = rewardData->getCountryContribution();
							resCount += getcount;//(itr->second)[2]*doubleindex;
							rewardData->setCountryContribution(resCount);
							break;
						case T_TroopsEquip:
							rewardData->getEquipsVec().push_back(std::make_pair((itr->second)[0],getcount/*(itr->second)[2]*doubleindex*/));
							break;
						case T_Item:
							{
								ref_ptr<crBagItemData> itemdata = new crBagItemData;
								itemdata->setItemID((itr->second)[0]);
								itemdata->setEquipMagic((itr->second)[1]);
								itemdata->setItemCount((itr->second)[2] * doubleindex);
								rewardData->getRewardItemsVec().push_back(itemdata);
// 								if(doubleindex == 2)
// 								{
// 									itemdata = new crBagItemData;
// 									itemdata->setItemID((itr->second)[0]);
// 									itemdata->setEquipMagic((itr->second)[1]);
// 									itemdata->setItemCount((itr->second)[2]);
// 									rewardData->getRewardItemsVec().push_back(itemdata);
// 								}
							}
							break;
						}
					}
					rewardData->setType(GP_RewardFubenSuccess);
					m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(rewardData.get(),NULL));
					playerData->inputParam(WCHDATA_JXJFubenRewardInfo,NULL);
				}
				playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
				if (needdeductgold)
				{
					crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(gameServer->getNetDataManager()->getNetCallback());
					callback->savePlayerData(playerGameData.get());
				}
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildReplyPacket(packet,playerid,WCH_JXJRecvClientChooseFubenDrop,NULL);
				gameServer->getNetManager()->sendPacket(playerGameData->getPlayerConnectServerAddress(),packet);

				///游戏日志需要记录玩家选择的奖励ID
				std::vector<int>logvec;
				logvec.resize(5);
				logvec[0] = overTimes;
				logvec[1] = type;
				logvec[2] = fubenDrop[0];
				logvec[3] = fubenDrop[1];
				logvec[4] = fubenDrop[2];
				std::string logdata = "选择副本奖励"+crArgumentParser::appVectoa(logvec);
				GameLogData gamelog(Log_FubenChooseReward,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			crBrain::getInstance()->doEvent(WCH_JXJExitBattleScene);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenDropClearMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenDropClearMethod::crJXJFubenDropClearMethod()
{

}
crJXJFubenDropClearMethod::crJXJFubenDropClearMethod(const crJXJFubenDropClearMethod& handle)
{

}
void crJXJFubenDropClearMethod::inputParam(int i, void *param)
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
void crJXJFubenDropClearMethod::addParam(int i, const std::string& str)
{
}
void crJXJFubenDropClearMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *playerData = m_this->getDataClass();
		//int playerid = m_this->getPlayerID();
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		//ref_ptr<crGameServerPlayerData> playerGameData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerid));
		if(playerData)
		{
			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJFubenRewardInfo,param);
			ref_ptr<crPlayerRewardData> reward_info_copy = (crPlayerRewardData*)param;
			if(reward_info_copy.valid())
			{//玩家掉线，随机一个奖励到邮箱
				if(!reward_info_copy->getFubenDropVec().empty())
				{
					CRCore::rangei counrnd;
					counrnd.set(0,reward_info_copy->getFubenDropVec().size());
					int rewardindex = counrnd.get_random();
					int index = 0;
					unsigned char type = reward_info_copy->getFubenDropVec()[rewardindex].first;
					int resCount = 0;
					switch (type)
					{
					case T_Food:
						resCount = reward_info_copy->getFood();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setFood(resCount);
						break;
					case T_Wood:
						resCount = reward_info_copy->getWood();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setWood(resCount);
						break;
					case T_Iron:
						resCount = reward_info_copy->getIron();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setIron(resCount);
						break;
					case T_Horse:
						resCount = reward_info_copy->getHorse();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setHorse(resCount);
						break;
					case T_Copper:
						resCount = reward_info_copy->getCooperCashReward();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setCoopercash(resCount);
						break;
					case T_Exp:
						resCount = reward_info_copy->getExperience();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setExperience(resCount);
						break;
					case T_Exploit:
						resCount = reward_info_copy->getExploit();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setExploit(resCount);
						break;
					case T_GuoZhanJiFen:
						resCount = reward_info_copy->getScore();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setScore(resCount);
						break;
					case T_Achievement:
						resCount = reward_info_copy->getAchievement();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setAchievement(resCount);
						break;
					case T_Giftgold:
						resCount = reward_info_copy->getGiftgold();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setGiftgold(resCount);
						break;
					case T_Contribute:
						resCount = reward_info_copy->getCountryContribution();
						resCount += (reward_info_copy->getFubenDropVec()[rewardindex].second)[2];
						reward_info_copy->setCountryContribution(resCount);
						break;
					case T_TroopsEquip:
						reward_info_copy->getEquipsVec().push_back(std::make_pair((reward_info_copy->getFubenDropVec()[rewardindex].second)[0],(reward_info_copy->getFubenDropVec()[rewardindex].second)[2]));
						break;
					case T_Item:
						{
							ref_ptr<crBagItemData> itemdata = new crBagItemData;
							itemdata->setItemID((reward_info_copy->getFubenDropVec()[rewardindex].second)[0]);
							itemdata->setEquipMagic((reward_info_copy->getFubenDropVec()[rewardindex].second)[1]);
							itemdata->setItemCount((reward_info_copy->getFubenDropVec()[rewardindex].second)[2]);
							reward_info_copy->getRewardItemsVec().push_back(itemdata);
						}
						break;
					}
				}
				crServerBrainHandle::getInstance()->doEvent(WCH_JXJCreateRewardMail,MAKECREPARAM(reward_info_copy.get(),NULL));
				playerData->inputParam(WCHDATA_JXJFubenRewardInfo,NULL);
				////////////////
				//if(!rewardData->getFubenDropVec().empty())
				//{
				//	CRCore::rangei counrnd;
				//	counrnd.set(0,rewardData->getFubenDropVec().size());
				//	int rewardindex = counrnd.get_random();
				//	int index = 0;
				//	unsigned char type = rewardData->getFubenDropVec()[rewardindex].first;
				//	int resCount = 0;
				//	switch (type)
				//	{
				//	case T_Food:
				//		resCount = rewardData->getFood();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setFood(resCount);
				//		break;
				//	case T_Wood:
				//		resCount = rewardData->getWood();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setWood(resCount);
				//		break;
				//	case T_Iron:
				//		resCount = rewardData->getIron();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setIron(resCount);
				//		break;
				//	case T_Horse:
				//		resCount = rewardData->getHorse();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setHorse(resCount);
				//		break;
				//	case T_Copper:
				//		resCount = rewardData->getCooperCashReward();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setCoopercash(resCount);
				//		break;
				//	case T_Exp:
				//		resCount = rewardData->getExperience();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setExperience(resCount);
				//		break;
				//	case T_Exploit:
				//		resCount = rewardData->getExploit();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setExploit(resCount);
				//		break;
				//	case T_GuoZhanJiFen:
				//		resCount = rewardData->getScore();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setScore(resCount);
				//		break;
				//	case T_Achievement:
				//		resCount = rewardData->getAchievement();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setAchievement(resCount);
				//		break;
				//	case T_Giftgold:
				//		resCount = rewardData->getGiftgold();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setGiftgold(resCount);
				//		break;
				//	case T_Contribute:
				//		resCount = rewardData->getCountryContribution();
				//		resCount += (rewardData->getFubenDropVec()[rewardindex].second)[2];
				//		rewardData->setCountryContribution(resCount);
				//		break;
				//	case T_TroopsEquip:
				//		rewardData->getEquipsVec().push_back(std::make_pair((rewardData->getFubenDropVec()[rewardindex].second)[0],(rewardData->getFubenDropVec()[rewardindex].second)[2]));
				//		break;
				//	case T_Item:
				//		{
				//			ref_ptr<crBagItemData> itemdata = new crBagItemData;
				//			itemdata->setItemID((rewardData->getFubenDropVec()[rewardindex].second)[0]);
				//			itemdata->setEquipMagic((rewardData->getFubenDropVec()[rewardindex].second)[1]);
				//			itemdata->setItemCount((rewardData->getFubenDropVec()[rewardindex].second)[2]);
				//			rewardData->getRewardItemsVec().push_back(itemdata);
				//		}
				//		break;
				//	}
				//	rewardData->setType(GP_RewardFubenDropItems);
				//	m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(rewardData.get(),NULL));
				//	playerData->inputParam(WCHDATA_JXJFubenRewardInfo,NULL);
				//}
			}
			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJFubenRushRewardMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFubenRushRewardMethod::crJXJFubenRushRewardMethod(){}
crJXJFubenRushRewardMethod::crJXJFubenRushRewardMethod(const crJXJFubenRushRewardMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_fubenID(0)
{

}
void crJXJFubenRushRewardMethod::inputParam(int i, void *param)
{
	switch(i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_fubenID = 0;
		}
		break;
	case 1:
		m_this = (crPlayerGameData *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_fubenID = *(unsigned short *)(LOCREPARAM(param64));
		}
		else
		{
			m_fubenID = 0;
		}
		break;
	}
}
void crJXJFubenRushRewardMethod::addParam(int i, const std::string& str)
{

}
void crJXJFubenRushRewardMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		//RewardItemVec reward_items;
		//RewardEquipsVec reward_equip;
		//FubenDropRewardVec fubenDropVec;
		crTableIO::DataVec recordVec;
		ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;

		ref_ptr<crTableIO> fubenRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenRewardTab);
		int copperIndex = fubenRewardTab->getTitleIndex("铜钱");
		int expIndex = fubenRewardTab->getTitleIndex("经验");
		reward_info->setPlayerID(m_this->getPlayerID());
		reward_info->setType(GP_RewardGameEnd);
		if(fubenRewardTab.valid())
		{
			fubenRewardTab->queryRecords(0,crArgumentParser::appItoa(m_fubenID),recordVec);
			if(!recordVec.empty())
			{
				if(copperIndex>0)
					reward_info->setCoopercash(atoi(recordVec[1][copperIndex].c_str()));
				if(expIndex>0)
					reward_info->setExperience(atoi(recordVec[1][expIndex].c_str()));
			}
		}
		crServerBrainHandle::getInstance()->doEvent(WCH_JXJFubenDropItems,MAKECREPARAM(&m_fubenID,&(reward_info->getFubenDropVec())));

		CRCore::rangei counrnd;
		counrnd.set(0,reward_info->getFubenDropVec().size());
		int rewardindex = counrnd.get_random();
		FubenDropRewardVec& fubenDropVec = reward_info->getFubenDropVec();
		unsigned char type = fubenDropVec[rewardindex].first;
		int resCount = 0;
		switch (type)
		{
		case T_Food:
			resCount = reward_info->getFood();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setFood(resCount);
			break;
		case T_Wood:
			resCount = reward_info->getWood();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setWood(resCount);
			break;
		case T_Iron:
			resCount = reward_info->getIron();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setIron(resCount);
			break;
		case T_Horse:
			resCount = reward_info->getHorse();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setHorse(resCount);
			break;
		case T_Copper:
			resCount = reward_info->getCooperCashReward();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setCoopercash(resCount);
			break;
		case T_Exp:
			resCount = reward_info->getExperience();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setExperience(resCount);
			break;
		case T_Achievement:
			resCount = reward_info->getAchievement();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setAchievement(resCount);
			break;
		case T_Exploit:
			resCount = reward_info->getExploit();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setExploit(resCount);
			break;
		case T_GuoZhanJiFen:
			resCount = reward_info->getScore();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setScore(resCount);
			break;
		case T_Giftgold:
			resCount = reward_info->getGiftgold();
			resCount += (fubenDropVec[rewardindex].second)[2];
			reward_info->setGiftgold(resCount);
			break;
		case T_Contribute:
			resCount = reward_info->getCountryContribution();
			resCount += (reward_info->getFubenDropVec()[rewardindex].second)[2];
			reward_info->setCountryContribution(resCount);
			break;
		case T_TroopsEquip:
			reward_info->getEquipsVec().push_back(std::make_pair((fubenDropVec[rewardindex].second)[0],(fubenDropVec[rewardindex].second)[2]));
			break;
		case T_Item:
			{
				ref_ptr<crBagItemData> itemdata = new crBagItemData;
				itemdata->setItemID((fubenDropVec[rewardindex].second)[0]);
				itemdata->setEquipMagic((fubenDropVec[rewardindex].second)[1]);
				itemdata->setItemCount((fubenDropVec[rewardindex].second)[2]);
				reward_info->getRewardItemsVec().push_back(itemdata);
			}
			break;
		}
		crData *data = m_this->getDataClass();
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		reward_info->setType(GP_RewardFubenRush);
		m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
		data->excHandle(MAKECREPARAM(WCH_LockData,0));

		int copper = reward_info->getCooperCashReward();
		int experice = reward_info->getExperience();
		int achi = reward_info->getAchievement();

		unsigned char equipSize = reward_info->getEquipsVec().size();
		unsigned char itemSize = reward_info->getRewardItemsVec().size();

		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(16+itemSize*9+equipSize*6);
		stream->_writeUShort(m_fubenID);
		stream->_writeInt(copper);
		stream->_writeInt(experice);
		stream->_writeInt(achi);
		stream->_writeUChar(itemSize);
		for(RewardItemVec::iterator itr_item = reward_info->getRewardItemsVec().begin();
			itr_item!=reward_info->getRewardItemsVec().end();itr_item++)
		{
			stream->_writeInt((*itr_item)->getItemID());
			stream->_writeInt((*itr_item)->getEquipMagic());
			stream->_writeUChar((*itr_item)->getItemCount());
		}
		stream->_writeUChar(equipSize);
		for(RewardEquipsVec::iterator itr_item = reward_info->getEquipsVec().begin();
			itr_item!=reward_info->getEquipsVec().end();itr_item++)
		{
			stream->_writeUShort(itr_item->first);
			stream->_writeInt(itr_item->second);
		}
		crPlayerDataEventPacket packet;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJClientRecvFubenRushReward,stream.get());
		ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
		if(playerData.valid())
			gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvFubenRushRewardMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientRecvFubenRushRewardMethod::crJXJClientRecvFubenRushRewardMethod()
{

}
crJXJClientRecvFubenRushRewardMethod::crJXJClientRecvFubenRushRewardMethod(const crJXJClientRecvFubenRushRewardMethod& handle)
{

}
void crJXJClientRecvFubenRushRewardMethod::inputParam(int i, void *param)
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
void crJXJClientRecvFubenRushRewardMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_checkBox = str;
		break;
	case 2:
		m_text = str;
		break;
	case 3:
		m_chatcfg = str;
		break;
	case 4:
		m_list = str;
		break;
	case 5:
		m_strItemCountInput[0] = str;
		break;
	case 6:
		m_strItemCountInput[1] = str;
		break;
	case 7:
		m_strItemCountInput[2] = str;
		break;
	case 8:
		m_strItemCountInput[3] = str;
		break;
	case 9:
		m_strItemCountInput[4] = str;
		break;
	case 10:
		m_strItemCountInput[5] = str;
		break;
	case 11:
		m_strItemCountInput[6] = str;
		break;
	case 12:
		m_inputItemsStr[0] = str;
		break;
	case 13:
		m_inputItemsStr[1] = str;
		break;
	case 14:
		m_inputItemsStr[2] = str;
		break;
	case 15:
		m_inputItemsStr[3] = str;
		break;
	case 16:
		m_inputItemsStr[4] = str;
		break;
	case 17:
		m_inputItemsStr[5] = str;
		break;
	case 18:
		m_inputItemsStr[6] = str;
		break;
	case 19:
		m_scrollbar = str;
		break;
	default:
		break;
	}
}
void crJXJClientRecvFubenRushRewardMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			RewardItemVec itemVec;
			RewardEquipsVec equipVec;
			unsigned short fubenID = m_stream->_readUShort();
			int copper = m_stream->_readInt();
			int experience = m_stream->_readInt();
			int archi = m_stream->_readInt();
			int itemSize = m_stream->_readUChar();
			ref_ptr<crBagItemData> itemdata;
			for (int i = 0;i<itemSize;i++)
			{
				itemdata = new crBagItemData;
				itemdata->setItemID(m_stream->_readInt());
				itemdata->setEquipMagic(m_stream->_readInt());
				itemdata->setItemCount(m_stream->_readUChar());
				itemVec.push_back(itemdata);
			}
			int equipSize = m_stream->_readUChar();
			unsigned short equipid;
			int count;
			for (int i = 0;i<equipSize;i++)
			{
				equipid = m_stream->_readShort();
				count = m_stream->_readInt();
				equipVec.push_back(std::make_pair(equipid,count));
			}
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				int index = 0;
				crData *canvasData = canvas->getDataClass();
				//ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_checkBox));
				ref_ptr<crHypertextWidgetNode>  hperInput = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_text));
				ref_ptr<crScrollBarWidgetNode > scrollBar = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_list));
				ref_ptr<crScrollBarWidgetNode > barInput = dynamic_cast<crScrollBarWidgetNode *>(canvas->getWidget(m_scrollbar));
				if(scrollBar.valid()) scrollBar->setValue(1.0f);
				//bool isAuto = false;
				//if(checkBox.valid())
					//isAuto = checkBox->getSelect();
				//自动出售
// 				if(checkBox->getSelect())
// 				{
// 					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
// 					crData *data = m_this->getDataClass();
// 					if(data && netConductor)
// 					{
// 						data->getParam(WCHDATA_JXJItemBagVec,param);
// 						JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
// 
// 						ref_ptr<crStreamBuf> stream = new crStreamBuf;
// 						stream->createBuf(10);
// 						for (RewardItemVec::iterator itr_item = itemVec.begin();itr_item!=itemVec.end();itr_item++)
// 						{
// 							int index = 0;
// 							for (JXJItemBagVec::iterator itr_bagItem = itembagvec->begin();
// 								itr_bagItem!=itembagvec->end();itr_bagItem++,index++)
// 							{
// 								if(itr_bagItem->valid())
// 								{
// 									if( (*itr_bagItem)->getItemID()==(*itr_item)->getItemID() && 
// 										(*itr_bagItem)->getEquipMagic()==(*itr_item)->getEquipMagic() && 
// 										(*itr_bagItem)->getItemCount()>= (*itr_item)->getItemCount() )
// 									{
// 										stream->seekBegin();
// 										stream->_writeUChar(index);
// 										stream->_writeInt((*itr_item)->getItemID());
// 										stream->_writeInt((*itr_item)->getEquipMagic());
// 										stream->_writeUChar((*itr_item)->getItemCount());
// 										crPlayerDataEventPacket packet;
// 										crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSaleItemEquip,stream.get());
// 										netConductor->getNetManager()->sendPacket("all",packet);
// 										break;
// 									}
// 								}
// 							}
// 						}
// 					}
// 				} // if(isAuto)
// 				else
				{
					ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
					if(!itemVec.empty() && itemtab.valid())
					{
						//显示
						ref_ptr<crStaticTextWidgetNode>  inputItemUI[7]; 
						ref_ptr<crStaticTextWidgetNode>  inputItemCount[7];
						for (int i = 0; i < 7; ++i)
						{
							inputItemUI[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_inputItemsStr[i]));
							inputItemCount[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strItemCountInput[i]));
						}

						if (inputItemUI[0].valid() && inputItemUI[1].valid() && inputItemUI[2].valid() && inputItemUI[3].valid() 
							&& inputItemUI[4].valid() && inputItemUI[5].valid() && inputItemUI[6].valid())
						{
							ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
							int colorIndex = qualitytab->getTitleIndex("color");

							crTableIO::StrVec itemrecord;
							int nameIndex = itemtab->getTitleIndex("name");
							int qualityIndex = itemtab->getTitleIndex("品质");
							//ref_ptr<crTableWidgetNode>  settleList= dynamic_cast<crTableWidgetNode *>(canvas->getChildNode(m_list));
							if(canvasData)
							{
								canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
								canvasData->getParam(WCHDATA_JXJFubenRushRewardItemMap,param);
								//需要修改equipmagic
								FubenRushItemMap *itemmap = (FubenRushItemMap *)param;
								//canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
								int itemid = 0;
								_crInt32 equipmagic = 0;
								int itemcount = 0;
								_crInt64 itemID;
								FubenRushItemMap::iterator itr_find;
								for (RewardItemVec::iterator itr_item = itemVec.begin();itr_item!=itemVec.end();itr_item++)
								{
									itemid = (*itr_item)->getItemID();
									equipmagic = (*itr_item)->getEquipMagic();
									itemcount = (*itr_item)->getItemCount();
									itemID = MAKEINT64(itemid,equipmagic);
									itr_find = itemmap->find(itemID);
									if(itr_find!=itemmap->end())
									{
										itr_find->second+=itemcount;
									}
									else
									{
										itemmap->insert(make_pair(itemID,itemcount));
									}
								}

								
								if(itemmap->size()>7) index = itemmap->size()-7;
								int itemindex = 0;
								std::string itemName;
								std::string itemCount;
								//settleList->clearData();
								for (FubenRushItemMap::iterator itr_items = itemmap->begin();itr_items!=itemmap->end();itr_items++,itemindex++)
								{
									if(itemindex<index) continue;
									if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itr_items->first),itemrecord)>=0)
									{
										if(nameIndex >= 0 && itemindex-index < 7)
										{
											itemName = itemrecord[nameIndex];
											itemCount = string("*")+crArgumentParser::appItoa(itr_items->second);
											if (qualityIndex > 0 && colorIndex > 0)
											{
												crTableIO::StrVec quality;
												CRCore::crVector4 colorVec;
												if (qualitytab->queryOneRecord(0, itemrecord[qualityIndex].c_str(), quality)>=0)
												{
													crArgumentParser::appAtoVec(quality[colorIndex], colorVec);
													colorVec.x() = colorVec.x()/255.0f;
													colorVec.y() = colorVec.y()/255.0f;
													colorVec.z() = colorVec.z()/255.0f;
													colorVec.w() = colorVec.w()/255.0f;
													inputItemUI[itemindex-index]->setColor(colorVec);
													inputItemUI[itemindex-index]->setString(itemName);
													inputItemUI[itemindex-index]->setVisiable(true);
													if(inputItemCount[itemindex-index].valid())
													{
														inputItemCount[itemindex-index]->setColor(colorVec);
														inputItemCount[itemindex-index]->setString(itemCount);
														inputItemCount[itemindex-index]->setVisiable(true);
													}
												}
											}
										}
									}
								} 						
								canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
							}
						} 
					} 
				}
				if(hperInput.valid())
				{
					CreateHyperText(hperInput.get(),fubenID,copper,experience,archi,itemVec);
					hperInput->setDefaultScrollPos(1);
				}
				if(barInput.valid())
				{
					barInput->setRange(0,index);
				}
			}
		}
	}
}

void crJXJClientRecvFubenRushRewardMethod::CreateHyperText(CRUI::crHypertextWidgetNode *hyperNode,unsigned short fubenid,int copper,int exp,int archi,RewardItemVec& itemVec )
{
	if(!hyperNode) return;
	void *param;
	crData *canvasData = hyperNode->getParentCanvas()->getDataClass();
	if(!canvasData) return;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	canvasData->getParam(WCHDATA_JXJFubenRushCount,param);
	unsigned int rushCount = *(unsigned int *)param;
	crHandle *linkhandle = NULL;
	crHandle *clonelink = NULL;
	vector<int> hypeyIDVec;
	crTableIO::StrVec record;
	crVector4f color = crVector4f(255.0f,255.0f,255.0f,255.0f);
	crVector4f colorGreen = crVector4f(0.0f,255.0f,0.0f,255.0f);
	ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
	ref_ptr<crTableIO> fubenTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
	crTableIO::StrVec colorrecord;
	crTableIO::StrVec itemrecord;
	int colorindex = qualitytab->getTitleIndex("color");
	int qualityindex = itemtab->getTitleIndex("品质");
	int nameIndex = itemtab->getTitleIndex("name");
	std::vector<float>v_i;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_chatcfg,true,""))
	{
		CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJChatReshowMethod(): file open error "<<m_chatcfg<<std::endl;
		//sprintf(gDebugInfo->buf(),"crJXJChatReshowMethod(): file open error %s\n\0",m_chatcfg.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}
	int index = 1;
	string fubenName;
	string itemName;
	if(fubenTab.valid())
	{
		int index = fubenTab->getTitleIndex("name");
		if(fubenTab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
		{
			if(index>=0)
				fubenName = record[index];
		}
	}
	cfg_script.Push("Hypertext");

	cfg_script.Add("Content");
	cfg_script.Push("Content",index++);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",std::string("-----------------------/n"));
	cfg_script.Pop();

	cfg_script.Add("Content");
	cfg_script.Push("Content",index++);
	v_i.resize(0);
	for (int i = 0; i<4;i++)
	{
		v_i.push_back(colorGreen[i]);
	}
	cfg_script.Add("Color",v_i);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",std::string("恭喜您在[")+fubenName+std::string("]第")+ crArgumentParser::appItoa(rushCount)+std::string("轮扫荡中获得了："));
	cfg_script.Pop();

	if(exp>0)
	{
		cfg_script.Add("Content");
		cfg_script.Push("Content",index++);
		v_i.resize(0);
		for (int i = 0; i<4;i++)
		{
			v_i.push_back(colorGreen[i]);
		}
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT");
		cfg_script.Add("Text",crArgumentParser::appItoa(exp)+"经验;");
		cfg_script.Pop();
	}
	if(copper>0)
	{
		cfg_script.Add("Content");
		cfg_script.Push("Content",index++);
		v_i.resize(0);
		for (int i = 0; i<4;i++)
		{
			v_i.push_back(colorGreen[i]);
		}
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT");
		cfg_script.Add("Text",crArgumentParser::appItoa(copper)+"铜钱;");
		cfg_script.Pop();
	}
	if(archi>0)
	{
		cfg_script.Add("Content");
		cfg_script.Push("Content",index++);
		v_i.resize(0);
		for (int i = 0; i<4;i++)
		{
			v_i.push_back(colorGreen[i]);
		}
		cfg_script.Add("Color",v_i);
		cfg_script.Add("DrawMode","TEXT");
		cfg_script.Add("Text",crArgumentParser::appItoa(archi)+"战功");
		cfg_script.Pop();
	}
	for (RewardItemVec::iterator itr_item = itemVec.begin();itr_item!=itemVec.end();itr_item++)
	{
		itemName.clear();
		if(itemtab->queryOneRecord(0,crArgumentParser::appItoa((*itr_item)->getItemID()),itemrecord)>=0)
		{
			if(nameIndex>=0)
			{
				itemName = itemrecord[nameIndex];
			}
			if(qualityindex>=0)
			{
				if (qualitytab->queryOneRecord(0,itemrecord[qualityindex],colorrecord)>=0)
				{
					if (colorindex>=0)
					{
						crArgumentParser::appAtoVec(colorrecord[colorindex],color);
					}
				}
			}
		}
		if(!itemName.empty())
		{
			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			v_i.resize(0);
			for (int i = 0; i<4;i++)
			{
				v_i.push_back(color[i]);
			}
			cfg_script.Add("Color",v_i);
			cfg_script.Add("DrawMode","TEXT");
			cfg_script.Add("Text",std::string("["));
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			v_i.resize(0);
			for (int i = 0; i<4;i++)
			{
				v_i.push_back(color[i]);
			}
			cfg_script.Add("Color",v_i);
			cfg_script.Add("DrawMode","TEXT|UNDERLINE");
			cfg_script.Add("Text",itemName);
			cfg_script.Add("Link",(*itr_item)->getItemID());
			hypeyIDVec.push_back((*itr_item)->getItemID());
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			v_i.resize(0);
			for (int i = 0; i<4;i++)
			{
				v_i.push_back(color[i]);
			}
			cfg_script.Add("Color",v_i);
			cfg_script.Add("DrawMode","TEXT");
			cfg_script.Add("Text",std::string("]"));
			cfg_script.Pop();

			cfg_script.Add("Content");
			cfg_script.Push("Content",index++);
			v_i.resize(0);
			for (int i = 0; i<4;i++)
			{
				v_i.push_back(colorGreen[i]);
			}
			cfg_script.Add("Color",v_i);
			cfg_script.Add("DrawMode","TEXT");
			cfg_script.Add("Text",crArgumentParser::appItoa((*itr_item)->getItemCount())+"个");
			cfg_script.Pop();
		}	
	}

	cfg_script.Add("Content");
	cfg_script.Push("Content",index++);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",std::string(" /n"));
	cfg_script.Pop();

	cfg_script.Add("Content");
	cfg_script.Push("Content",index++);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",std::string("-----------------------/n"));
	cfg_script.Pop();

	cfg_script.Pop();
	hyperNode->setHypertext(cfg_script,true);
	crData *chatData = hyperNode->getDataClass();
	std::vector<int>::iterator itr_link = hypeyIDVec.begin(); 
	for(;itr_link!=hypeyIDVec.end();itr_link++)
	{
		if(chatData)
		{
			linkhandle = chatData->getHandle(MAKEINT64(WCH_UI_LBtnDown,1));
			clonelink = linkhandle->clone();
			chatData->insertHandle(MAKEINT64(WCH_UI_LBtnDown,*itr_link),clonelink);
		}
	}
	cfg_script.Close();
	rushCount++;

	///////////////////////////////////////////////
	crData *brainData = crBrain::getInstance()->getDataClass();
	//int rushTaskCount = 0;
	//rushTaskCount++;
	brainData->getParam(WCHDATA_JXJBrainRushTaskCount,param);
	int rushTaskCount = *(int *)param;
	rushTaskCount++;
	brainData->inputParam(WCHDATA_JXJBrainRushTaskCount,&rushTaskCount);

	canvasData->inputParam(WCHDATA_JXJFubenRushCount,&rushCount);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvFubenReliveMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvFubenReliveMethod::crJXJRecvFubenReliveMethod(){}
crJXJRecvFubenReliveMethod::crJXJRecvFubenReliveMethod(const crJXJRecvFubenReliveMethod &handle)
{

}
void crJXJRecvFubenReliveMethod::inputParam(int i, void *param)
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
void crJXJRecvFubenReliveMethod::addParam(int i, const std::string& str)
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

void crJXJRecvFubenReliveMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == SceneServer)
		{
			//char birthIndex = m_stream->_readChar();
			crSceneLayer *sceneLayer = NULL;
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
				CRNetApp::crScene *scene = callback->findScene(playerData->getSceneID());
				crRoom *room = callback->findRoom(playerData->getRoomID());
				if(scene && room && !roleMap.empty())
				{
					crRoomPlayer *roomplayer = room->getMember(m_this->getPlayerID());
					if(roomplayer)
					{
						int index = room->getBirthPointIndex(roomplayer);
						crData *roleData;
						crRole *role = roleMap.begin()->second.get();
						crVector2i deadPos(role->getPosx(),role->getPosy());
						crSceneLayer *sceneLayer = scene->getSceneLayer(roleMap.begin()->second.get()->getLayerID());
						const crSceneLayer::BirthPointArray& pointVec = sceneLayer->getBirthPointArray();
						std::set<crVector3i> BirthPointSet;
						BirthPointSet.insert(pointVec[index]);
						void *param;
						crData *data = m_this->getDataClass();
						
						data->excHandle(MAKECREPARAM(WCH_LockData,1));
						data->getParam(WCHDATA_JXJFubenReliveCount,param);
						unsigned char *count = (unsigned char *)param;

						if(*count>0)
						{
							(*count)--;
							//补充兵力
							data->getParam(WCHDATA_JXJFormationInfoVec,param);
							FormationInfoVec *formationVec = (FormationInfoVec *)param;
							/*unsigned short*/int totalcount = 0;
							crSceneServerPlayerData::RoleMap::iterator itr_role;
							FormationInfoVec::iterator itr = formationVec->begin();
							for (;itr!=formationVec->end();itr++)
							{
								itr_role = roleMap.find(itr->get()->getRoleID());
								if(itr_role!=roleMap.end())
								{
									roleData = itr_role->second->getDataClass();
									if(!roleData) continue;
									roleData->getParam(WCHDATA_JXJArmyCount,param);
									totalcount = *(/*unsigned short*/int *)param;
									//roleData->getParam(WCHDATA_JXJRTArmyCount,param);
									//rtcount = *(unsigned short *)param;
									/*if(rtCount<=0 || count == rtCount) continue;*/

									roleData->inputParam(WCHDATA_JXJRTArmyCount,&totalcount);
									itr->get()->setCount(totalcount);
								}
							}
							data->excHandle(MAKECREPARAM(WCH_LockData,0));
							//////////////////////////////////////////////////////////////////////////
							data->getParam(WCHDATA_JXJShiliID,param);
							unsigned char myShili = *(unsigned char *)param;
							crData *roomData = room->getDataClass();
							roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
							roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
							JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
							JXJCampBirthpoint& birthpointSet = (*campMap)[myShili];
							for( JXJCampBirthpoint::iterator bitr = birthpointSet.begin();
								bitr != birthpointSet.end();
								++bitr )
							{
								BirthPointSet.insert(bitr->first);
							}
							roomData->excHandle(MAKECREPARAM(WCH_LockData,0)); 
							std::set<crVector3i>::iterator bpitr = BirthPointSet.begin();
							float mindist = (crVector2i((*bpitr)[0],(*bpitr)[1]) - deadPos).length();
							float dist;
							crVector3i nearBirthPoint = *bpitr;
							for( ++bpitr;
								bpitr != BirthPointSet.end();
								++bpitr )
							{
								dist = (crVector2i((*bpitr)[0],(*bpitr)[1]) - deadPos).length();
								if(dist<mindist)
								{
									mindist = dist;
									nearBirthPoint = *bpitr;
								}
							}
							//unsigned char itemstate = IS_Relive;
							float scale = crGlobalHandle::gData()->gUnitScale();
							crVector3f birthPoint;
							birthPoint[0] = nearBirthPoint[0]*scale;
							birthPoint[1] = nearBirthPoint[1]*scale;
							//rangei range(-nearBirthPoint[2],nearBirthPoint[2]);
							crVector3 pos;
							for( crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
								itr != roleMap.end();
								++itr )
							{
								role = itr->second.get();
								//birthPoint[0] = nearBirthPoint[0] + range.get_random();
								//birthPoint[1] = nearBirthPoint[1] + range.get_random();
								//z = scene->getPosZ(role->getLayerID(),birthPoint[0]*scale,birthPoint[1]*scale, role->getZoffset() * scale);
								if(!scene->findWalkablePos(role,crVector2(birthPoint[0],birthPoint[1]),c_walkableSearchRange,pos))
								{
									CRCore::notify(CRCore::ALWAYS)<<"crJXJRecvFubenReliveMethod findWalkablePos失败 "<<birthPoint<<std::endl;
								}
								else
								{
									pos/=scale;
									role->setPosxy(pos[0],pos[1]);
									role->setPosz(pos[2]);
								}
								roleData = role->getDataClass();
								roleData->excHandle(MAKECREPARAM(WCH_InitData,role));
								//roleData->inputParam(WCHDATA_ItemState, &itemstate);
								scene->itemRelive(itr->second.get());
							}

							crPlayerDataSceneEventPacket packet2;
							crPlayerDataSceneEventPacket::buildReplyPacket(packet2,m_this->getPlayerID(),WCH_JXJRecvFubenRelive,NULL);
							sceneServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet2);
						}
						else
						{
							data->excHandle(MAKECREPARAM(WCH_LockData,0));
						}
						room->doEvent(MAKEINT64(WCH_JXJPlayerRelive,WCH_Server),MAKECREPARAM(m_this,NULL));
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			//bu
			void *param;
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJFormationInfoVec,param);
				FormationInfoVec *formationInfoVec = (FormationInfoVec *)param;
				crBrain::getInstance()->doEvent(WCH_JXJRecvAddArmySucess);
				crMyPlayerData::MyRoleNpcMap &roleNpcMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
				crRole *role;
				float dt = 0.1f;
				for( crMyPlayerData::MyRoleNpcMap::iterator itr = roleNpcMap.begin();
					itr != roleNpcMap.end();
					++itr )
				{
					role = itr->second.first.get();
					if(role)
					{
						role->doEvent(WCH_ChangeActState,MAKECREPARAM(ACT_Idle,&dt));
						crData *data = role->getDataClass();
						data->getParam(WCHDATA_JXJArmyCount,param);
						/*unsigned short*/int count = *(/*unsigned short*/int *)param;
						FormationInfoVec::iterator itr_for = formationInfoVec->begin();
						while (itr_for!=formationInfoVec->end())
						{
							if((*itr_for)->getRoleID() == role->getRoleID())
							{
								(*itr_for)->setCount(count);
								break;
							}
							itr_for++;
						}
					}
				}
			}
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crData *canvasData = canvas->getDataClass();
				if(canvasData)
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJFubenCanReliveCount,param);
					unsigned char *count = (unsigned char *)param;
					(*count)++;
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
				room->doEvent(MAKEINT64(WCH_JXJPlayerRelive,WCH_Client),MAKECREPARAM(m_this,NULL));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJInitFubenPlayerReliveCountMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJInitFubenPlayerReliveCountMethod::crJXJInitFubenPlayerReliveCountMethod(){}
crJXJInitFubenPlayerReliveCountMethod::crJXJInitFubenPlayerReliveCountMethod(const crJXJInitFubenPlayerReliveCountMethod &handle)
{

}
void crJXJInitFubenPlayerReliveCountMethod::inputParam(int i, void *param)
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
void crJXJInitFubenPlayerReliveCountMethod::addParam(int i, const std::string& str)
{

}

void crJXJInitFubenPlayerReliveCountMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crTableIO> fubentab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFubenTab);
	crData *playerData = m_this->getDataClass();
	if(playerData)
	{
		playerData->getParam(WCHDATA_JXJFubenOrChengchiType,param);
		unsigned char type = *(unsigned char *)param;
		playerData->getParam(WCHDATA_JXJFubenOrChengchiID,param);
		unsigned short fubenid = *(unsigned short *)param;
		crTableIO::StrVec record;
		if(type == FT_Fuben && fubenid>0 && fubentab->queryOneRecord(0,crArgumentParser::appItoa(fubenid),record)>=0)
		{
			int index = fubentab->getTitleIndex("允许补兵次数");
			unsigned char count = atoi(record[index].c_str());
			playerData->inputParam(WCHDATA_JXJFubenReliveCount,&count);
		}
	}
}
/////////////////////////////////////////////////////////////////////////
//SceneServer判断补兵
//crJXJFubenReliveCheckMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJFubenReliveCheckMethod::crJXJFubenReliveCheckMethod()
{

}
crJXJFubenReliveCheckMethod::crJXJFubenReliveCheckMethod(const crJXJFubenReliveCheckMethod &handle)
{

}
void crJXJFubenReliveCheckMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
			m_playerData = NULL;
		}
		break;
	case 1:
		m_this = (crRoom *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_playerData = (crSceneServerPlayerData *)(LOCREPARAM(param64));
		}
		else
		{
			m_playerData = NULL;
		}
		break;
	default:
		break;
	}
}
void crJXJFubenReliveCheckMethod::addParam(int i, const std::string& str)
{

}
void crJXJFubenReliveCheckMethod::operator ()(crHandle &handle)
{//会导致动态下载玩家无法进入副本
	if(m_this && m_playerData)
	{
		void *param;
		crRole *role = NULL;
		crData *roleData = NULL;
		crData *data = NULL;
		unsigned int guisestate = GS_Normal;

		crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
		if(!sceneServer) return;
		m_this->lockPlayerList();
		crRoom::PlayerList &playerList = m_this->getPlayerList();
		crRoom::PlayerList tempRemoveList;
		int size = playerList.size();
		for(crRoom::PlayerList::iterator pitr = playerList.begin(); pitr != playerList.end(); pitr++)
		{
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>
				(sceneServer->getNetDataManager()->getPlayerData(pitr->get()->getPlayerID()));
			if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass())
			{
				bool isAllDead = true;
				crSceneServerPlayerData::RoleMap &roleMap = playerData->getRoleMap();
				for (crSceneServerPlayerData::RoleMap::iterator itr = roleMap.begin();
					itr!=roleMap.end();itr++)
				{
					if(itr->second.valid())
					{
						role = itr->second.get();
						if(role)
						{
							role->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
							isAllDead = isAllDead && (guisestate & GS_UnVisiable);
						}
					}
				}
				if(isAllDead)
				{
					data =  playerData->getPlayerGameData()->getDataClass();
					data->getParam(WCHDATA_JXJFubenReliveCount,param);
					unsigned char count = *(unsigned char *)param;

					if(count<=0)
					{
						size--;
						//tempRemoveList.push_back(*pitr);
					}
				}
			}
		}
		m_this->unlockPlayerList();
		if(size <= 0)
		{
			m_this->timeRemain() = 0;
		}
		else
		{
			// 			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>
			//  				(sceneServer->getNetDataManager()->getPlayerData(m_playerID));
			crPlayerDataSceneEventPacket packet2;
			crPlayerDataSceneEventPacket::buildReplyPacket(packet2,m_playerData->getPlayerID(),WCH_JXJFubenReliveCountHasUsed,NULL);
			sceneServer->getNetManager()->sendPacket(m_playerData->getPlayerConnectServerAddress(),packet2);
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleShowChatCanvasMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJBattleShowChatCanvasMethod::crJXJBattleShowChatCanvasMethod()
{

}
crJXJBattleShowChatCanvasMethod::crJXJBattleShowChatCanvasMethod(const crJXJBattleShowChatCanvasMethod &handle)
{

}
void crJXJBattleShowChatCanvasMethod::inputParam(int i, void *param)
{
}
void crJXJBattleShowChatCanvasMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_chatCanvas = str;
		break;
	case 1:
		m_minimapCanvas = str;
		break;
	}
}
void crJXJBattleShowChatCanvasMethod::operator ()(crHandle &handle)
{
	CRCore::ref_ptr<crCanvasNode>  chatCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_chatCanvas);
	CRCore::ref_ptr<crCanvasNode>  miniMapCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_minimapCanvas);
	if(miniMapCanvas.valid() && chatCanvas.valid() && (chatCanvas->getVisiable()==false))
	{
		const crBoundingBox& canvas_box = chatCanvas->getBoundBox();
		const crBoundingBox& bbox = miniMapCanvas->getBoundBox();
		chatCanvas->setMatrix(crMatrix::translate(bbox.xMin()+canvas_box.xLength()*0.5f,bbox.center().y()+bbox.yLength()*0.5+canvas_box.yLength()*0.5f+0.02,0.0f));
		crFilterRenderManager::getInstance()->showCanvas(chatCanvas.get(), true);
	}
	crBrain::getInstance()->doEvent(WCH_JXJUIUpdateChatMessageList);
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleChatCanvasOnEnterShowMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJBattleChatCanvasOnEnterShowMethod::crJXJBattleChatCanvasOnEnterShowMethod():m_value(1)
{

}
crJXJBattleChatCanvasOnEnterShowMethod::crJXJBattleChatCanvasOnEnterShowMethod(const crJXJBattleChatCanvasOnEnterShowMethod &handle)
{

}
void crJXJBattleChatCanvasOnEnterShowMethod::inputParam(int i, void *param)
{
}
void crJXJBattleChatCanvasOnEnterShowMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_chatCanvas = str;
		break;
	case 1:
		m_switch = str;
		break;
	case 2:
		m_value = atoi(str.c_str());
		break;
	}
}
void crJXJBattleChatCanvasOnEnterShowMethod::operator ()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>  chatCanvas = CRCore::crFilterRenderManager::getInstance()->findCanvas(m_chatCanvas);
	if(chatCanvas.valid())
	{
		ref_ptr<crMultiSwitch>  canvasSwitch = dynamic_cast<crMultiSwitch *>(chatCanvas->getChildNode(m_switch));
		if(canvasSwitch.valid())
			canvasSwitch->setActiveSwitchSet(m_value);
		crData *canvasData = chatCanvas->getDataClass();
		if(canvasData && m_value)
		{
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJChatCanvasAutoHideTimer,param);
			float *timer = (float *)param;
			*timer = 1.0f;
 			canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
			////切换成中文
			//if (NULL == ::LoadKeyboardLayout( "E0220804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
			//	if (NULL == ::LoadKeyboardLayout( "E0210804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
			//		if (NULL == ::LoadKeyboardLayout( "E0200804", KLF_ACTIVATE|KLF_SETFORPROCESS ))
			//			::LoadKeyboardLayout( "E0230804", KLF_ACTIVATE|KLF_SETFORPROCESS );
		}
		if(!m_value)
		{
			CRCore::crFilterRenderManager::getInstance()->setInputCapturer(NULL);
			CRCore::crFilterRenderManager::getInstance()->setFocusNode(NULL);
			//切换回英文
			//::LoadKeyboardLayout( "0x0409", KLF_ACTIVATE|KLF_SETFORPROCESS );
		}
	}
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJLoadKeyboardLayoutMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJLoadKeyboardLayoutMethod::crJXJLoadKeyboardLayoutMethod():
	m_key(0)
{

}
crJXJLoadKeyboardLayoutMethod::crJXJLoadKeyboardLayoutMethod(const crJXJLoadKeyboardLayoutMethod &handle):
	m_key(0)
{

}
void crJXJLoadKeyboardLayoutMethod::inputParam(int i, void *param)
{
}
void crJXJLoadKeyboardLayoutMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_key = atoi(str.c_str());
		break;
	}
}
void crJXJLoadKeyboardLayoutMethod::operator ()(crHandle &handle)
{
	if(crDisplaySettings::instance()->getRunMode()<crDisplaySettings::Robot)
	{
		if(m_key == 0)
		{
			HWND hWnd = crStartHandler::getInstance()->getWindowHandle();
			if(!hWnd) return;
			PostMessage(hWnd,WM_USER+1,0,0);
		}
		else
		{
			HWND hWnd = crStartHandler::getInstance()->getWindowHandle();
			if(!hWnd) return;
			PostMessage(hWnd,WM_USER+1,1,0);
		}
	}
}
/////////////////////////////////////////////////////////////////////////
//
//crJXJBattleChatCanvasOnEnterShowMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJBattleChatCanvasAutoHideMethod::crJXJBattleChatCanvasAutoHideMethod()
{

}
crJXJBattleChatCanvasAutoHideMethod::crJXJBattleChatCanvasAutoHideMethod(const crJXJBattleChatCanvasAutoHideMethod &handle)
{

}
void crJXJBattleChatCanvasAutoHideMethod::inputParam(int i, void *param)
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
	default:
		break;
	}
}
void crJXJBattleChatCanvasAutoHideMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_switch = str;
		break;
	case 1:
		m_input = str;
	}
}
void crJXJBattleChatCanvasAutoHideMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		ref_ptr<crEditWidgetNode>  input = dynamic_cast<crEditWidgetNode *>(m_this->getWidget(m_input));
		ref_ptr<crMultiSwitch>  canvasSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch));
		crData *data = m_this->getDataClass();
		bool isShow = canvasSwitch->getActiveSwitchSet();
		if(data && canvasSwitch.valid() && isShow)
		{
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJChatCanvasAutoHideTimer,param);
			float *timer = (float *)param;
			if(crFilterRenderManager::getInstance()->getInputCapturer() == input.get())
			{
				*timer = 1.0f;
			}
			else
			{
				if(*timer>0)
					*timer-=1.0f;
				else
				{
					canvasSwitch->setActiveSwitchSet(0);
					CRCore::crFilterRenderManager::getInstance()->setInputCapturer(NULL);
					CRCore::crFilterRenderManager::getInstance()->setFocusNode(NULL);
					//输入法切换英文
 					//g_hImc = ImmGetContext(crKeyboardMouseHandle::getInstance()->getBindViewer()->getCameraWindow());
 					//ImmSetOpenStatus(g_hImc,false);
				}
			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
//crJXJFubenReliveCountHasUsedMethod
//
/////////////////////////////////////////////////////////////////////////
crJXJFubenReliveCountHasUsedMethod::crJXJFubenReliveCountHasUsedMethod()
{

}
crJXJFubenReliveCountHasUsedMethod::crJXJFubenReliveCountHasUsedMethod(const crJXJFubenReliveCountHasUsedMethod &handle)
{

}
void crJXJFubenReliveCountHasUsedMethod::inputParam(int i, void *param)
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
void crJXJFubenReliveCountHasUsedMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_button = str;
		break;
	default:
		break;
	}
}
void crJXJFubenReliveCountHasUsedMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		//void *param;
		if(m_netType == SceneServer)
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(sceneServer->getNetDataManager()->getNetCallback());
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid())
			{
				CRNetApp::crScene *scene = callback->findScene(playerData->getSceneID());
				crRoom *room = callback->findRoom(playerData->getRoomID());
				if(scene && room)
				{
					room->doEvent(WCH_JXJFubenReliveCheck,MAKECREPARAM(playerData.get(),NULL));
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(canvas->getWidget(m_input));
				ref_ptr<crButtonWidgetNode>   button = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_button));
				//if(button)
				//button->setEnable(false);
				std::vector<float>v_i;
				ItemMap itemMap;
				rcfg::ConfigScript cfg_script(&itemMap);
				cfg_script.Add("Hypertext");
				cfg_script.Push("Hypertext");
				cfg_script.Add("Content");
				cfg_script.Push("Content");
				cfg_script.Add("Text","您的补兵次数已用完!");
				cfg_script.Pop();
				cfg_script.Pop();

				if(hyperText.valid())
					hyperText->setHypertext(cfg_script);
				crFilterRenderManager::getInstance()->doModal(m_canvas);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSkillProtectTimerProgressShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSkillProtectTimerProgressShowMethod::crJXJSkillProtectTimerProgressShowMethod()
{

}
crJXJSkillProtectTimerProgressShowMethod::crJXJSkillProtectTimerProgressShowMethod(const crJXJSkillProtectTimerProgressShowMethod &handle)
{

}
void crJXJSkillProtectTimerProgressShowMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJSkillProtectTimerProgressShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_imageBox = str;
		break;
	}
}
void crJXJSkillProtectTimerProgressShowMethod::operator ()(crHandle &handle)
{//
	if(m_this)
	{
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(canvas.valid())
		{
			ref_ptr<crImageBoxWidgetNode>  image = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_imageBox));
			if(image.valid())
			{
				const crBoundingBox &bbox = canvas->getBoundBox();
				const crBoundingBox &imaBbox = image->getBoundBox();
				m_this->setMatrix(crMatrix::translate(imaBbox.center().x(),bbox.yMax()+0.1f,0.0f));
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSkillProtectTimerProgressUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSkillProtectTimerProgressUpdateMethod::crJXJSkillProtectTimerProgressUpdateMethod()
{

}
crJXJSkillProtectTimerProgressUpdateMethod::crJXJSkillProtectTimerProgressUpdateMethod(const crJXJSkillProtectTimerProgressUpdateMethod &handle)
{

}
void crJXJSkillProtectTimerProgressUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJSkillProtectTimerProgressUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_progress = str;
		break;
	case 1:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJSkillProtectTimerProgressUpdateMethod::operator ()(crHandle &handle)
{//
	ref_ptr<crProgressWidgetNode>  progress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_progress));
	crRole *role = crMyPlayerData::getInstance()->getLoadedRoleByIndex(m_index);
	bool needclose = true;
	if(role && progress.valid())
	{
		void *param;
		crData *roleData = role->getDataClass();
		//roleData->getParam(WCHDATA_ItemState,param);
		//unsigned char itemstate = *(unsigned char *)param;
		//if(itemstate == IS_UseItem)
		//{
			crData *canvasData = m_this->getDataClass();
			if (canvasData)
			{
				canvasData->getParam(WCHDATA_JXJProgressCanvasAboutUseItemId, param);
				int itemId = *(int *)param;
				if(itemId!=0)
				{
					crItemChild *itemChild = role->findChildItem(itemId);
					if(itemChild && itemChild->isLoaded())
					{
						crInstanceItem *item = itemChild->getInstanceItem();
						crData *skillData = item->getDataClass();
						skillData->getParam(WCHDATA_UseItemProtectTime,param);
						float skillProtectTime = *(float *)param;
						roleData->getParam(WCHDATA_CurActState,param);
						unsigned short act = *(unsigned short *)param;
						if(act == ACT_Skill)
						{
							roleData->getParam(WCHDATA_CurActDuration,param);
							float duration = *(float *)param;
							if(duration<=skillProtectTime)
							{
								if(!m_this->getVisiable())
									crFilterRenderManager::getInstance()->showCanvas(m_this,true);
								if(progress.valid())
								{
									progress->setProgress((skillProtectTime-duration)/skillProtectTime);
								}
							}
							else
							{
								canvasData->inputParam(WCHDATA_JXJProgressCanvasAboutUseItemId, 0);
							}
							needclose = false;
						}
					}
				}
			}
		//}
	}
	if(needclose)
	{
		crFilterRenderManager::getInstance()->closeCanvas(m_this);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSkillBufferShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSkillBufferShowMethod::crJXJSkillBufferShowMethod()
{

}
crJXJSkillBufferShowMethod::crJXJSkillBufferShowMethod(const crJXJSkillBufferShowMethod &handle)
{

}
void crJXJSkillBufferShowMethod::inputParam(int i, void *param)
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
		m_this = (crCanvasNode *)param;
		break;
	}
}
void crJXJSkillBufferShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_icon[0][0] = str;
		break;
	case 1:
		m_icon[0][1] = str;
		break;
	case 2:
		m_icon[0][2] = str;
		break;
	case 3:
		m_icon[0][3] = str;
		break;
	case 4:
		m_icon[0][4] = str;
		break;
	case 5:
		m_icon[0][5] = str;
		break;
	case 6:
		m_icon[0][6] = str;
		break;
	case 7:
		m_icon[0][7] = str;
		break;
	case 8:
		m_icon[1][0] = str;
		break;
	case 9:
		m_icon[1][1] = str;
		break;
	case 10:
		m_icon[1][2] = str;
		break;
	case 11:
		m_icon[1][3] = str;
		break;
	case 12:
		m_icon[1][4] = str;
		break;
	case 13:
		m_icon[1][5] = str;
		break;
	case 14:
		m_icon[1][6] = str;
		break;
	case 15:
		m_icon[1][7] = str;
		break;
	case 16:
		m_icon[2][0] = str;
		break;
	case 17:
		m_icon[2][1] = str;
		break;
	case 18:
		m_icon[2][2] = str;
		break;
	case 19:
		m_icon[2][3] = str;
		break;
	case 20:
		m_icon[2][4] = str;
		break;
	case 21:
		m_icon[2][5] = str;
		break;
	case 22:
		m_icon[2][6] = str;
		break;
	case 23:
		m_icon[2][7] = str;
		break;
	case 24:
		m_strInput[0][0] = str;
		break;
	case 25:
		m_strInput[0][1] = str;
		break;
	case 26:
		m_strInput[0][2] = str;
		break;
	case 27:
		m_strInput[0][3] = str;
		break;
	case 28:
		m_strInput[0][4] = str;
		break;
	case 29:
		m_strInput[0][5] = str;
		break;
	case 30:
		m_strInput[0][6] = str;
		break;
	case 31:
		m_strInput[0][7] = str;
		break;
	case 32:
		m_strInput[1][0] = str;
		break;
	case 33:
		m_strInput[1][1] = str;
		break;
	case 34:
		m_strInput[1][2] = str;
		break;
	case 35:
		m_strInput[1][3] = str;
		break;
	case 36:
		m_strInput[1][4] = str;
		break;
	case 37:
		m_strInput[1][5] = str;
		break;
	case 38:
		m_strInput[1][6] = str;
		break;
	case 39:
		m_strInput[1][7] = str;
		break;
	case 40:
		m_strInput[2][0] = str;
		break;
	case 41:
		m_strInput[2][1] = str;
		break;
	case 42:
		m_strInput[2][2] = str;
		break;
	case 43:
		m_strInput[2][3] = str;
		break;
	case 44:
		m_strInput[2][4] = str;
		break;
	case 45:
		m_strInput[2][5] = str;
		break;
	case 46:
		m_strInput[2][6] = str;
		break;
	case 47:
		m_strInput[2][7] = str;
		break;
	default:
		break;
	}
}
void crJXJSkillBufferShowMethod::operator ()(crHandle &handle)
{
	ref_ptr<crImageBoxWidgetNode>  bufferImage[3][8];
	ref_ptr<crStaticTextWidgetNode> bufferText[3][8];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bufferImage[i][j] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon[i][j]));
			if (bufferImage[i][j].valid()) bufferImage[i][j]->setVisiable(false);
			bufferText[i][j] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strInput[i][j]));
			if (bufferText[i][j].valid())
			{
				bufferText[i][j]->clearString();
				bufferText[i][j]->setCanCaptureMouse(false);
			}
		}
	}
	void *param;
	int j = 0;
	float buffertime = 0.0f;
	crTableIO::StrVec record;
	std::string bufferIcon;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	ref_ptr<crTableIO> bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);
	int iconIndex = bufferAttrTab->getTitleIndex("BufferIcon");
	//int timeIndex = bufferAttrTab->getTitleIndex("BufferTime");
	crData *canvasData = m_this->getDataClass();
	CRCore::ScopedLock<crData> lock(*canvasData);
	canvasData->getParam(WCHDATA_JXJBufferTimerMap, param);
	BufferInfoMap * bufferInfoMap = (BufferInfoMap *)param;
	for (int i = 0; i < 3; i++)
	{
		j = 0;
		BufferInfoMap tempBufferInfoMap;
		crRole *role = me->getLoadedRoleByIndex(i);
		if (role)
		{
			crData * roleData = role->getDataClass();
			if (roleData)
			{
				roleData->getParam(WCHDATA_ExtraData, param);
				crData *extraData = (crData *)param;
				CRCore::ScopedLock<crData> lock(*extraData);
				extraData->getParam(WCHDATA_ExtraIDMap, param);
				ExtraIDMap* extraIDMap = (ExtraIDMap *)param;

				crHandle *handle;
				crExtraLifeTimeMethod* method;
				crData *widgetData = NULL;
				for (ExtraIDMap::iterator itr_extra = extraIDMap->begin();
					itr_extra != extraIDMap->end() && j < 8;
					++itr_extra)
				{
					handle = extraData->getHandle(MAKEINT64(WCH_DoExtra, itr_extra->first));
					method = dynamic_cast<crExtraLifeTimeMethod*>(handle->getHandle("ExtraLifeTime"));
					buffertime = 0.0f;
					if (method)
					{
						buffertime = method->getRemainTime();
					}
					if (buffertime == 0.0f)
						buffertime = FLT_MAX;
					short extraSecond = itr_extra->second;
					short secondValue = MAKEINT16(HIINT16(extraSecond), 0);
					std::string extraValue = crArgumentParser::appItoa(itr_extra->first) + \
						std::string("|") + crArgumentParser::appItoa(secondValue);
					int overlapCount = LOINT16(itr_extra->second);

					if (/*overlapCount >= 0 && */bufferAttrTab->queryOneRecord(0, extraValue, record) >= 0)
					{
						int iconIndex = bufferAttrTab->getTitleIndex("BufferIcon");
						bufferImage[i][j]->setVisiable(true);
						bufferImage[i][j]->setImageName(record[iconIndex]);
						widgetData = bufferImage[i][j]->getDataClass();
						if (widgetData)
						{
							if (buffertime <= 5.0f)
							{
								bool isFlash = true;
								widgetData->inputParam(WCHDATA_JXJBufferWidgetIsFlash, &isFlash);//开始
							}
							else
							{
								bool isFlash = false;
								widgetData->inputParam(WCHDATA_JXJBufferWidgetIsFlash, &isFlash);
							}
						}

						tempBufferInfoMap.insert(std::make_pair(itr_extra->first, std::make_pair(secondValue, buffertime)));
						if (overlapCount > 1/*0*/ && bufferText[i][j].valid())
						{
							bufferText[i][j]->setString(crArgumentParser::appItoa(overlapCount/* + 1*/));
						}
						j++;
					}
				}
			}
		}
		bufferInfoMap[i].swap(tempBufferInfoMap);
	}
}
//bool crJXJSkillBufferShowMethod::isInspire(int id)
//{
//	if (id == JXJExt_Inspire || id == JXJExt_Inspire_2\
//		|| id == JXJExt_Inspire_3 || id == JXJExt_Inspire_4\
//		|| id == JXJExt_Inspire_5 || id == JXJExt_Inspire_6\
//		|| id == JXJExt_Inspire_7 || id == JXJExt_Inspire_8\
//		|| id == JXJExt_Inspire_9 || id == JXJExt_Inspire_10\
//		|| id == JXJExt_Inspire_11 || id == JXJExt_Inspire_12\
//		|| id == JXJExt_Inspire_13 || id == JXJExt_Inspire_14\
//		|| id == JXJExt_Inspire_15)
//		return true;
//	return false;
//}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientUserRecvSomeValueMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientUserRecvSomeValueMethod::crJXJClientUserRecvSomeValueMethod()
{

}
crJXJClientUserRecvSomeValueMethod::crJXJClientUserRecvSomeValueMethod(const crJXJClientUserRecvSomeValueMethod &handle)
{

}
void crJXJClientUserRecvSomeValueMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
			m_id = 0;
			m_value = NULL;
		}
		break;
	case 1:
		m_this = (crBrain *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_id = LOCREPARAM(param64);
			m_value = HICREPARAM(param64);
		}
		else
		{
			m_id = 0;
			m_value = 0;
		}
		break;
	}
}
void crJXJClientUserRecvSomeValueMethod::addParam(int i, const std::string& str)
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
		crArgumentParser::appAtoVec(str, m_troopsCountTaskVec);
		break;
	default:
		break;
	}
}
void crJXJClientUserRecvSomeValueMethod::operator ()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		string text;
		if(m_value == 0) return;
		if(m_value>0)
		{
			text = string("恭喜您获得：");
			ref_ptr<crBagItemData>bagitemdataget = new crBagItemData;
			bagitemdataget->setItemID(m_value);
			crData *braindata = m_this->getDataClass();
			if(braindata)
			{
				braindata->excHandle(MAKECREPARAM(WCH_LockData,1));
				braindata->getParam(WCHDATA_JXJRecvBagItemDeq,param);
				if(param)
				{
					RecvBagItemDeq *itemDeq = (RecvBagItemDeq *)param;
					itemDeq->push_back(std::make_pair(m_id,bagitemdataget));
				}
				braindata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		else
			text = string("消耗：");
		ref_ptr<crTableIO> troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
		int nameIndex = troopTab->getTitleIndex("name");
		crTableIO::StrVec record;
		switch (m_id)
		{
		case T_Food:
			text+=crArgumentParser::appItoa(abs(m_value))+string("粮草");
			break;
		case T_Wood:
			text+=crArgumentParser::appItoa(abs(m_value))+string("木材");
			break;
		case T_Iron:
			text+=crArgumentParser::appItoa(abs(m_value))+string("铁矿");
			break;
		case T_Horse:
			text+=crArgumentParser::appItoa(abs(m_value))+string("马匹");
			break;
		case T_Copper:
			text+=crArgumentParser::appItoa(abs(m_value))+string("铜钱");
			break;
		case T_Exp:
			text+=crArgumentParser::appItoa(abs(m_value))+string("经验");
			break;
		case T_Achievement:
			text+=crArgumentParser::appItoa(abs(m_value))+string("战功");
			break;
		case T_Giftgold:
			text+=crArgumentParser::appItoa(abs(m_value))+string("礼金");
			break;
		case T_TroopsEquip:
			text+=crArgumentParser::appItoa(abs(m_value))+string("军械");
			break;
		//case T_Item:
		//	//text+=crArgumentParser::appItoa(abs(m_value))+string("军械");
		//	break;
		//case T_Troops:
		//	//text+=crArgumentParser::appItoa(abs(m_value))+string("军械");
		//	break;
		case T_Exploit:
			text+=crArgumentParser::appItoa(abs(m_value))+string("功勋");
			break;
		case T_Contribute:
			text+=crArgumentParser::appItoa(abs(m_value))+string("国家贡献");
			break;
		case T_Gold:
			text+=crArgumentParser::appItoa(abs(m_value))+string("元宝");
			break;
		case T_JiangHun:
			text+=crArgumentParser::appItoa(abs(m_value))+string("将魂");
			break;
		case T_CardImpression:
			text+=crArgumentParser::appItoa(abs(m_value))+string("好感度");
			break;
		case T_JunLing:
			text+=crArgumentParser::appItoa(abs(m_value))+string("军令");
			break;
		//case T_Level:
		//	text+=crArgumentParser::appItoa(abs(m_value))+string("等级");
		//	break;
		case T_FreeRollCard:
			text += crArgumentParser::appItoa(abs(m_value)) + string("免费抽将次数");
			break;
		case T_FreeRefine:
			text += crArgumentParser::appItoa(abs(m_value)) + string("免费精炼次数");
			break;
		case T_VIPExp:
			text += crArgumentParser::appItoa(abs(m_value)) + string("VIP经验");
			break;
		case T_Zhanquan:
			text += crArgumentParser::appItoa(abs(m_value)) + string("征战券");
			break;
		case T_GuoZhanJiFen:
			text += crArgumentParser::appItoa(abs(m_value)) + string("国战积分");
				break;
		case T_GroupContribute:
			text += crArgumentParser::appItoa(abs(m_value)) + string("军团贡献");
			break;
		case T_GroupFunds:
			text += crArgumentParser::appItoa(abs(m_value)) + string("军团资金");
			break;
		case T_AchievePoint:
			text += crArgumentParser::appItoa(abs(m_value)) + string("成就积分");
			break;
		case T_SuperEquipLvupPoint:
			text += crArgumentParser::appItoa(abs(m_value)) + string("注灵点数");
			break;
		case T_SuperEquipExp:
			text += crArgumentParser::appItoa(abs(m_value)) + string("神器经验");
			break;
		case T_SuperEquipCoin:
			text += crArgumentParser::appItoa(abs(m_value)) + string("神器币");
			break;
		default:
			if(m_id>T_END)
			{
				if(troopTab->queryOneRecord(0,crArgumentParser::appItoa(m_id),record)>=0 && nameIndex>=0)
				{
					text+=crArgumentParser::appItoa(abs(m_value))+ "[" + record[nameIndex] + "]";
				}
				if (m_value > 0 && !m_troopsCountTaskVec.empty())
				{
					crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
					if (playerData && playerData->getDataClass())
					{
						for (std::vector<int>::iterator itr = m_troopsCountTaskVec.begin();
							itr != m_troopsCountTaskVec.end();
							++itr)
						{
							playerData->doEvent(WCH_JXJAddTaskProgress, MAKECREPARAM(*itr, m_value));
						}
					}
				}
			////获得士兵任务进度更新600-605
			}
			break;
		}

		//ref_ptr<crCanvasNode>  chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		//if(chatCanvas.valid())
		//{
			//ref_ptr<crMultiSwitch>  chatSelect = dynamic_cast<crMultiSwitch *>(chatCanvas->getChildNode(m_switch));
			crMyPlayerData *me = crMyPlayerData::getInstance();
			if(/*chatSelect.valid() && */me->getPlayerGameData())
			{
				crData *playerData = me->getPlayerGameData()->getDataClass();
				if(playerData)
				{
					playerData->getParam(WCHDATA_JXJVipLv,param);
					unsigned char vipLevel = *(unsigned char *)param;
					int max_deque_size = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine, vipLevel).c_str());
					playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
					//int chat_mode = chatSelect->getActiveSwitchSet();
					//if (chat_mode != ChatInCountry && chat_mode != ChatInTeam 
					//	&& chat_mode != ChatInGroup && chat_mode != ChatOneToOne  && chat_mode != ChatInWorld)
					//{
					//	chat_mode = ChatAll;
					//}
					ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
					record->setChatChannelName("[系统]");
					record->setChatText(text);
					if(m_value>0)
						record->setTextColor(COLOR_GREEN);
					else
						record->setTextColor(COLOR_RED);
					crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKECREPARAM(record.get(),NULL));
					playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		//}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientUserRecvSomeItemsMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJClientUserRecvSomeItemsMethod::crJXJClientUserRecvSomeItemsMethod()
{

}
crJXJClientUserRecvSomeItemsMethod::crJXJClientUserRecvSomeItemsMethod(const crJXJClientUserRecvSomeItemsMethod &handle)
{

}
void crJXJClientUserRecvSomeItemsMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{
			m_this = NULL;
			m_itemVec = NULL;
			m_cardVec = NULL;
		}
		break;
	case 1:
		m_this = (crBrain *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_itemVec = (JXJItemBagVec *)LOCREPARAM(param64);
			m_cardVec = (JXJItemBagVec *)HICREPARAM(param64);
		}
		else
		{
			m_itemVec = NULL;
			m_cardVec = NULL;
		}
		break;
	}
}
void crJXJClientUserRecvSomeItemsMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_switch = str;
		break;
	default:
		break;
	}
}
void crJXJClientUserRecvSomeItemsMethod::operator ()(crHandle &handle)
{
	//if(m_this)
	//{
		//if(!crRunGameHandle::getInstance()->isInManor() && !crRunGameHandle::getInstance()->isInBattle()) return;
		void *param;
		int itemID = 0;
		int equipMagic = 0;
		int itemCount = 0;
		int cardID = 0;
		int cardCount = 0;
		int maxIndex = 0;
		int index = 0; 
		string text;
		vector<pair<int,int>> itemIDVec;
		if((m_itemVec && m_itemVec->size()>0) || (m_cardVec && m_cardVec->size()>0))
			text = "恭喜您获得：";
		if (m_itemVec->size()>0 &&  m_cardVec->size()>0 && (*m_itemVec)[0] == NULL && (*m_cardVec)[0] == NULL)
		{
			text = "您已失去：";
			m_itemVec->erase(m_itemVec->begin());
			m_cardVec->erase(m_cardVec->begin());
		}
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec itemrecord;
		if(itemtab.valid())
		{
			int name_index = itemtab->getTitleIndex("name");
			if(m_itemVec)
			{
				maxIndex = m_itemVec->size()-1;
				for (JXJItemBagVec::iterator itr = m_itemVec->begin();itr!=m_itemVec->end();itr++,index++)
				{
					itemID = itr->get()->getItemID();
					equipMagic = itr->get()->getEquipMagic();
					itemCount = itr->get()->getItemCount();
					if (itemCount <= 0)
					{
						itemCount = itr->get()->getItemDeductCount();
					}
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0 && name_index>=0)
					{
						std::string xingji;
						if (atoi(itemrecord[0].c_str()) >= 8010)
						{
							if (atoi(itemrecord[3].c_str()) > 4)
							{
								xingji = "(" +crArgumentParser::appItoa(atoi(itemrecord[3].c_str())-4) + "星";
								xingji = xingji + ")";
							}
						}
						
						itemIDVec.push_back(make_pair(itemID,equipMagic));
						text+=string("[")+itemrecord[name_index]+string("]")+xingji+string("*")+crArgumentParser::appItoa(itemCount);
						if(index<maxIndex)
							text+=string(";");
					}
				}
			}
			if(m_cardVec)
			{
				maxIndex = m_cardVec->size()-1;
				index = 0;
				for (JXJItemBagVec::iterator itr = m_cardVec->begin();itr!=m_cardVec->end();itr++)
				{
					itemID = itr->get()->getItemID();
					itemCount = itr->get()->getItemCount();
					if (itemCount <= 0)
					{
						itemCount = itr->get()->getItemDeductCount();
					}
					equipMagic = 0;
					if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0 && name_index>=0)
					{
						std::string xingji;
						if (atoi(itemrecord[0].c_str()) >= 8010)
						{
							if (atoi(itemrecord[3].c_str()) > 4)
							{
								xingji = "(" +crArgumentParser::appItoa(atoi(itemrecord[3].c_str())-4) + "星";
								xingji = xingji + ")";
							}
						}
						itemIDVec.push_back(make_pair(itemID,equipMagic));
						text+=string("[")+itemrecord[name_index]+string("]")+xingji+string("*")+crArgumentParser::appItoa(itemCount);;
						if(index<maxIndex)
							text+=string(";");
					}
				}
			}

			//ref_ptr<crCanvasNode>  chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			//if(chatCanvas.valid())
			//{
				//crData *canvasData = chatCanvas->getDataClass();
				//ref_ptr<crMultiSwitch>  chatSelect = dynamic_cast<crMultiSwitch *>(chatCanvas->getChildNode(m_switch));
				crMyPlayerData *me = crMyPlayerData::getInstance();
				if(me && me->getPlayerGameData())
				{
					crData *playerData = me->getPlayerGameData()->getDataClass();
					if(playerData)
					{
						playerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						int max_deque_size = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJLimitChatLine,vipLevel).c_str());
						if(itemIDVec.size()>0)
						{
							text += std::string("{");
							vector<pair<int,int>>::iterator itr_item = itemIDVec.begin();
							for (;itr_item!=itemIDVec.end();itr_item++)
							{
								if(itr_item->first >0)
								{
									text+=std::string("(")+crArgumentParser::appItoa(itr_item->first)+string("|")+crArgumentParser::appItoa(itr_item->second)+std::string(")");
								}
							}
							text += std::string("}");
							itemIDVec.clear();
						}

						playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
						//int chat_mode = chatSelect->getActiveSwitchSet()+1;
						//if (chat_mode != ChatInCountry && chat_mode != ChatInTeam 
						//	&& chat_mode != ChatInGroup && chat_mode != ChatOneToOne  && chat_mode != ChatInWorld)
						//{
						//	chat_mode = ChatAll;
						//}
						ref_ptr<crJXJChatRecord> record = new crJXJChatRecord();
						//record->setSenderName("[系统]");
						record->setChatText(text);
						record->setTextColor(COLOR_GREEN);
						record->setChatChannelName("[系统]");
// 						playerData->getParam(WCHDATA_JXJChatRecordMap,param);
// 						ChatRecordMap *chat_records = (ChatRecordMap *)param;
// 						ChatRecordMap::iterator itr_map = chat_records->begin();
// 
// 						for (;itr_map!=chat_records->end();itr_map++)
// 						{
// 							if(itr_map->second.size() >= max_deque_size)
// 								itr_map->second.pop_front();
// 							itr_map->second.push_back(record.get());
// 						}			
						
						crBrain::getInstance()->doEvent(WCH_JXJPutInChatRecordDeque,MAKECREPARAM(record.get(),NULL));

						playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			//}
		}
	//}
}
/////////////////////////////////////////
//
//crJXJClientRecvFoodMethod
//
/////////////////////////////////////////
crJXJClientRecvFoodMethod::crJXJClientRecvFoodMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvFoodMethod::crJXJClientRecvFoodMethod(const crJXJClientRecvFoodMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvFoodMethod::inputParam(int i, void *param)
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

void crJXJClientRecvFoodMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvFoodMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int food = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJFood,param);
				int extra = food - *(int *)param;
				data->inputParam(WCHDATA_JXJFood,&food);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Food,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvWoodMethod
//
/////////////////////////////////////////
crJXJClientRecvWoodMethod::crJXJClientRecvWoodMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvWoodMethod::crJXJClientRecvWoodMethod(const crJXJClientRecvWoodMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvWoodMethod::inputParam(int i, void *param)
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

void crJXJClientRecvWoodMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvWoodMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int wood = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJWood,param);
				int extra = wood - *(int *)param;
				data->inputParam(WCHDATA_JXJWood,&wood);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Wood,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvIronMethod
//
/////////////////////////////////////////
crJXJClientRecvIronMethod::crJXJClientRecvIronMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvIronMethod::crJXJClientRecvIronMethod(const crJXJClientRecvIronMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvIronMethod::inputParam(int i, void *param)
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

void crJXJClientRecvIronMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvIronMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int iron = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJIron,param);
				int extra = iron - *(int *)param;
				data->inputParam(WCHDATA_JXJIron,&iron);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Iron,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJClientRecvHorseMethod
//
/////////////////////////////////////////
crJXJClientRecvHorseMethod::crJXJClientRecvHorseMethod():
	m_netType(GameClient_Game){}
crJXJClientRecvHorseMethod::crJXJClientRecvHorseMethod(const crJXJClientRecvHorseMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvHorseMethod::inputParam(int i, void *param)
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

void crJXJClientRecvHorseMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvHorseMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			int horse = m_stream->_readInt(); 
			crData *data = m_this->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJHorse,param);
				int extra = horse - *(int *)param;
				data->inputParam(WCHDATA_JXJHorse,&horse);
				if(extra!=0)
					crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM(T_Horse,extra));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJIsPlayerHasCardGuideShowMethod
//
/////////////////////////////////////////
crJXJIsPlayerHasCardGuideShowMethod::crJXJIsPlayerHasCardGuideShowMethod(){}
crJXJIsPlayerHasCardGuideShowMethod::crJXJIsPlayerHasCardGuideShowMethod(const crJXJIsPlayerHasCardGuideShowMethod& handle):
	crMethod(handle)
{
}
void crJXJIsPlayerHasCardGuideShowMethod::inputParam(int i, void *param)
{
}

void crJXJIsPlayerHasCardGuideShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasFirst = str;
		break;
	case 1:
		m_canvasSecod = str;
		break;
	case 2:
		m_canvasCard = str;
		break;
	case 3:
		m_icon[0] = str;
		break;
	case 4:
		m_icon[1] = str;
		break;
	case 5:
		m_icon[2] = str;
		break;
	default:
		break;
	}
}

void crJXJIsPlayerHasCardGuideShowMethod::operator()(crHandle &handle)
{
	ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
	if(!camera.valid())
		return;
	void  *param;
	int index = -1;
	ref_ptr<crCanvasNode>  firstCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasFirst);
	ref_ptr<crCanvasNode>  secondCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasSecod);
	ref_ptr<crCanvasNode>  cardCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasCard);
	crData *cameraData = camera->getAttachedNode()->getDataClass();
	CRCore::ScopedLock<crData> lock(*cameraData);
	crVector2i equips = crVector2i(-1,-1);
	crTableIO::StrVec itemrecord;
	//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	crMyPlayerData *me = crMyPlayerData::getInstance();
	crData *playerdata = me->getPlayerGameData()->getDataClass();
	playerdata->getParam(WCHDATA_JXJFormationInfoVec,param);
	FormationInfoVec *myteam = (FormationInfoVec *)param;
	crRole *role = NULL;
	for (int i = 0;i<3;i++)
	{
		role = me->getLoadedRoleByIndex(i);
		if(role)
		{
			crData *roleData = role->getDataClass();
			int roleid = role->getRoleID();
			roleData->getParam(WCHDATA_ItemState,param);
			unsigned char itemstate = *((unsigned char*)param);
			if(itemstate != IS_Dead)
			{
				int j = 0;
				for( FormationInfoVec::iterator itr = myteam->begin();
					itr != myteam->end();
					++itr,j++)
				{
					if(roleid == (*itr)->getRoleID())
					{
						equips = (*itr)->getEquips();
						if(equips[0]>0)
						{
							index = j;
							break;
						}

					}
				}
				if(index>=0)
				{
					break;
				}
			}
		}
	}
	if(index>=0 && index<3)
	{
		ref_ptr<crImageBoxWidgetNode>  	imageBox;
		if(cardCanvas.valid())
		{
			imageBox = dynamic_cast<crImageBoxWidgetNode *>(cardCanvas->getWidget(m_icon[index]));
			if(imageBox.valid() && secondCanvas.valid())
			{
				const crBoundingBox &imagebbox = imageBox->getBoundBox();
				const crBoundingBox &bbox = secondCanvas->getBoundBox();
				crFilterRenderManager::getInstance()->showCanvas(secondCanvas.get(),m_canvasSecod,SH_Show);
				secondCanvas->setMatrix(crMatrix::translate(imagebbox.center().x(),imagebbox.center().y()+imagebbox.yLength()*0.5 - 0.1,0.0f));
			}
		}
	}
	else
	{
		if(firstCanvas.valid())
			crFilterRenderManager::getInstance()->doModal(firstCanvas.get(),m_canvasFirst);
	}
}

/////////////////////////////////////////
//
//crJXJShowUserHelpCanvasMethod
//
/////////////////////////////////////////
crJXJShowUserHelpCanvasMethod::crJXJShowUserHelpCanvasMethod(){}
crJXJShowUserHelpCanvasMethod::crJXJShowUserHelpCanvasMethod(const crJXJShowUserHelpCanvasMethod& handle):
	crMethod(handle)
{
}
void crJXJShowUserHelpCanvasMethod::inputParam(int i, void *param)
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

void crJXJShowUserHelpCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_hypertext = str;
		break;
	case 1:
		m_cfg = str;
		break;
	}
}

void crJXJShowUserHelpCanvasMethod::operator()(crHandle &handle)
{
	ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_hypertext));
	if(hyperText.valid())
		hyperText->setHypertext(m_cfg);
}

//////////////////////////////////////////////////////////////////////////
//在线奖励玩
//crJXJRecvOnlineRewardQueryMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvOnlineRewardQueryMethod::crJXJRecvOnlineRewardQueryMethod(){}
crJXJRecvOnlineRewardQueryMethod::crJXJRecvOnlineRewardQueryMethod(const crJXJRecvOnlineRewardQueryMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvOnlineRewardQueryMethod::inputParam(int i, void *param)
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

void crJXJRecvOnlineRewardQueryMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_icon[0] = str;
		break;
	case 2:
		m_icon[1] = str;
		break;
	case 3:
		m_icon[2] = str;
		break;
	case 4:
		m_icon[3] = str;
		break;
	case 5:
		m_input[0] = str;
		break;
	case 6:
		m_input[1] = str;
		break;
	case 7:
		m_input[2] = str;
		break;
	case 8:
		m_input[3] = str;
		break;
	case 9:
		m_imageName = str;
		break;
	case 10:
		m_mainCanvas = str;
		break;
	case 11:
		m_btOnlineReward = str;
		break;
	case 12:
		m_timerInput = str;
		break;
	default:
		break;
	}
}

void crJXJRecvOnlineRewardQueryMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		crTableIO::StrVec record;
		if(m_netType == GameServer)
		{
			bool issucess = false;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			//RewardItemVec reward_items;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			int playerID = m_this->getPlayerID();			
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
			ref_ptr<crPlayerGameData> playerGameData;
			ref_ptr<crData> playerData;
			ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
			unsigned char olcount = 0;
			if(netDataManager && olRewardTab.valid())
			{
				playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
				if(playerGameServerPlayerData.valid())
				{
					playerGameData = playerGameServerPlayerData->getPlayerGameData();
					if(playerGameData.valid() && playerGameData->getDataClass())
					{
						playerData = playerGameData->getDataClass();
						playerData->excHandle(MAKECREPARAM(WCH_LockData,1));

						playerData->getParam(WCHDATA_JXJOnlineRewardCount,param);
						unsigned char* olRewardCount = (unsigned char*)param;
						olcount = *olRewardCount;
						if(*olRewardCount>=1)
						{
							playerData->getParam(WCHDATA_JXJOnlineRewardTimer,param);
							_crInt64 *timer = (_crInt64 *)param;
							time_t t = time(0);
							int olineRewardTimer = 0;
							int timerIndex = olRewardTab->getTitleIndex("timer");
							int giftGoldIndex = olRewardTab->getTitleIndex("礼金");
							int item1index = olRewardTab->getTitleIndex("道具1");
							int item2index = olRewardTab->getTitleIndex("道具2");
							int item3index = olRewardTab->getTitleIndex("道具3");
							crVector3i vec3;
							ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
							ref_ptr<crBagItemData> itemdata;
							if(olRewardTab->queryOneRecord(0,crArgumentParser::appItoa(*olRewardCount),record)>=0)
							{
								if(timerIndex>=0)
									olineRewardTimer = atoi(record[timerIndex].c_str());
								if(olineRewardTimer <= t - *timer)
								{
									if(item1index>=0)
									{
										crArgumentParser::appAtoVec(record[item1index],vec3);
										if(vec3[0]>0)
										{
											itemdata = new crBagItemData;
											itemdata->setItemID(vec3[0]);
											itemdata->setEquipMagic(vec3[1]);
											itemdata->setItemCount(vec3[2]);
											reward_info->getRewardItemsVec().push_back(itemdata);
										}
									}
									if(item2index>=0)
									{
										crArgumentParser::appAtoVec(record[item2index],vec3);
										if(vec3[0]>0)
										{
											itemdata = new crBagItemData;
											itemdata->setItemID(vec3[0]);
											itemdata->setEquipMagic(vec3[1]);
											itemdata->setItemCount(vec3[2]);
											reward_info->getRewardItemsVec().push_back(itemdata);
										}
									}
									if(item3index>=0)
									{
										crArgumentParser::appAtoVec(record[item3index],vec3);
										if(vec3[0]>0)
										{
											itemdata = new crBagItemData;
											itemdata->setItemID(vec3[0]);
											itemdata->setEquipMagic(vec3[1]);
											itemdata->setItemCount(vec3[2]);
											reward_info->getRewardItemsVec().push_back(itemdata);
										}
									}
									if(giftGoldIndex>=0)
									{
										reward_info->setGiftgold(atoi(record[giftGoldIndex].c_str()));
									}
									reward_info->setType(GP_OnlineReward);
									m_this->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
									(*olRewardCount)--;
									*timer = t;
									issucess = true;

									stream->createBuf(2);
									stream->_writeBool(issucess);
									stream->_writeUChar(*olRewardCount);
									crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
									//ref_ptr<crGameServerPlayerData> playerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvOnlineRewardQuery,stream.get());
									gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
								}
								else
								{
									stream->createBuf(5);
									stream->_writeBool(false);
									stream->_writeInt(olineRewardTimer+*timer-t);
									crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
									//
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvOnlineRewardQuery,stream.get());
									gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
								}
							}
						}
						playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
			if(issucess)
			{
				///游戏日志
				std::string logdata = "在线奖励onlinecount:" +  crArgumentParser::appItoa(olcount);
				GameLogData gamelog(Log_GetOnlineReward,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerID,&gamelog));
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int inputN = 4;
			int iconN = 4;
			ref_ptr<crStaticTextWidgetNode>  input[4] = {NULL};
			ref_ptr<crImageBoxWidgetNode>  icon[4] = {NULL};
			ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
			bool issucess = m_stream->_readBool();
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid() && olRewardTab.valid())
			{
				for(int i = 0; i < inputN; i++)
				{
					input[i] = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_input[i]));

				}
				for(int i = 0; i < iconN; i++)
				{
					icon[i] = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_icon[i]));
				}
				crTableIO::StrVec record,itemrecord;
				//ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
				ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
				int widgetIndex = 0;

				int timerindex = olRewardTab->getTitleIndex("timer");
				crMyPlayerData *me = crMyPlayerData::getInstance();
				if( me && me->getPlayerGameData() && itemtab.valid())
				{
					crData *playerData = me->getPlayerGameData()->getDataClass();
					playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJOnlineRewardCount,param);
					unsigned char *count = (unsigned char *)param; 
					playerData->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
					int *timer = (int *)param;
					if(issucess)
					{
						*count = m_stream->_readUChar();
						if(*count>0)
						{
							int giftGoldIndex = olRewardTab->getTitleIndex("礼金");
							int item1index = olRewardTab->getTitleIndex("道具1");
							int item2index = olRewardTab->getTitleIndex("道具2");
							int item3index = olRewardTab->getTitleIndex("道具3");
							crVector3i vec3;
							ref_ptr<crBagItemData> itemdata1,itemdata2,iitemdata3;
							if(olRewardTab->queryOneRecord(0,crArgumentParser::appItoa(*count),record)>=0)
							{
								if(timerindex>=0)
									*timer = atoi(record[timerindex].c_str());

								if(giftGoldIndex>=0)
								{
									//
									if(icon[widgetIndex].valid())
										icon[widgetIndex]->setImageName(m_imageName);
									if(input[widgetIndex].valid())
									{
										input[widgetIndex]->setString(record[giftGoldIndex]);
										widgetIndex++;
									}
								}
								if(item1index>=0)
								{
									crArgumentParser::appAtoVec(record[item1index],vec3);
									if(vec3[0]>0)
									{
										itemdata1 = new crBagItemData;
										itemdata1->setItemID(vec3[0]);
										itemdata1->setEquipMagic(vec3[1]);
										itemdata1->setItemCount(vec3[2]);
									}
								}
								if(item2index>=0)
								{
									crArgumentParser::appAtoVec(record[item2index],vec3);
									if(vec3[0]>0)
									{
										itemdata2 = new crBagItemData;
										itemdata2->setItemID(vec3[0]);
										itemdata2->setEquipMagic(vec3[1]);
										itemdata2->setItemCount(vec3[2]);
									}
								}
								if(item3index>=0)
								{
									crArgumentParser::appAtoVec(record[item3index],vec3);
									if(vec3[0]>0)
									{
										iitemdata3 = new crBagItemData;
										iitemdata3->setItemID(vec3[0]);
										iitemdata3->setEquipMagic(vec3[1]);
										iitemdata3->setItemCount(vec3[2]);
									}
								}

								if(itemdata1.valid())
								{
									int iconIndex = itemtab->getTitleIndex("icon");
									if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemdata1->getItemID()),itemrecord)>=0)
									{
										if(iconIndex>=0 && icon[widgetIndex].valid() && input[widgetIndex].valid())
										{
											icon[widgetIndex]->setVisiable(true);
											icon[widgetIndex]->setImageName(itemrecord[iconIndex]);
											input[widgetIndex]->setString(crArgumentParser::appItoa(itemdata1->getItemCount()));
											widgetIndex++;
										}
									}							
								}
								if(itemdata2.valid())
								{
									int iconIndex = itemtab->getTitleIndex("icon");
									if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemdata2->getItemID()),itemrecord)>=0)
									{
										if(iconIndex>=0 && icon[widgetIndex].valid() && input[widgetIndex].valid())
										{
											icon[widgetIndex]->setVisiable(true);
											icon[widgetIndex]->setImageName(itemrecord[iconIndex]);
											input[widgetIndex]->setString(crArgumentParser::appItoa(itemdata2->getItemCount()));
											widgetIndex++;
										}
									}
								}
								if(iitemdata3.valid())
								{
									int iconIndex = itemtab->getTitleIndex("icon");
									if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(iitemdata3->getItemID()),itemrecord)>=0)
									{
										if(iconIndex>=0 && icon[widgetIndex].valid() && input[widgetIndex].valid())
										{
											icon[widgetIndex]->setVisiable(true);
											icon[widgetIndex]->setImageName(itemrecord[iconIndex]);
											input[widgetIndex]->setString(crArgumentParser::appItoa(iitemdata3->getItemCount()));
											widgetIndex++;
										}
									}
								}
							}
						}
					}
					else
					{
						*timer = m_stream->_readInt();
					}
					playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}		
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJOnlineRewardCanvasOnShowMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJOnlineRewardCanvasOnShowMethod::crJXJOnlineRewardCanvasOnShowMethod(){}
crJXJOnlineRewardCanvasOnShowMethod::crJXJOnlineRewardCanvasOnShowMethod(const crJXJOnlineRewardCanvasOnShowMethod& handle):
	crMethod(handle)
{
}
void crJXJOnlineRewardCanvasOnShowMethod::inputParam(int i, void *param)
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

void crJXJOnlineRewardCanvasOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_icon1 = str;
		break;
	case 1:
		m_icon2 = str;
		break;
	case 2:
		m_icon3 = str;
		break;
	case 3:
		m_icon4 = str;
		break;
	case 4:
		m_input1 = str;
		break;
	case 5:
		m_input2 = str;
		break;
	case 6:
		m_input3 = str;
		break;
	case 7:
		m_input4 = str;
		break;
	case 8:
		m_switch1 = str;
		break;
	case 9:
		m_switch2 = str;
		break;
	case 10:
		m_switch3 = str;
		break;
	case 11:
		m_switch4 = str;
		break;
	case 12:
		m_switch5 = str;
		break;
	case 13:
		m_switch6 = str;
		break;
	case 14:
		m_btGet = str;
		break;
	case 15:
		m_iconName = str;
		break;
	default:
		break;
	}
}

void crJXJOnlineRewardCanvasOnShowMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		ref_ptr<crImageBoxWidgetNode>  image[4] = {NULL}; 
		ref_ptr<crStaticTextWidgetNode>  input[4] = {NULL}; 
		ref_ptr<crMultiSwitch> mulitSwitch[6] = {NULL};
		image[0] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon1));
		image[1] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon2));
		image[2] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon3));
		image[3] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_icon4));
		input[0] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input1));
		input[1] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input2));
		input[2] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input3));
		input[3] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_input4));

		mulitSwitch[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch1));
		mulitSwitch[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch2));
		mulitSwitch[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch3));
		mulitSwitch[3] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch4));
		mulitSwitch[4] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch5));
		mulitSwitch[5] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch6));
		ref_ptr<crButtonWidgetNode>  btGet = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btGet));
		if(!image[0].valid() || !image[1].valid() || !image[2].valid() || !image[3].valid() || !btGet.valid()) return;
		if(!input[0].valid() || !input[1].valid() || !input[2].valid() || !input[3].valid()) return;
		if(!mulitSwitch[0].valid() || !mulitSwitch[1].valid() || !mulitSwitch[2].valid()\
			|| !mulitSwitch[3].valid() || !mulitSwitch[4].valid() || !mulitSwitch[5].valid()) return;
		crMyPlayerData *me = crMyPlayerData::getInstance();
		if(!me) return;
		image[0]->setVisiable(false);
		image[1]->setVisiable(false);
		image[2]->setVisiable(false);
		image[3]->setVisiable(false);
		input[0]->clearString();
		input[1]->clearString();
		input[2]->clearString();
		input[3]->clearString();

		int widgetIndex = 0;
		crData *canvasData = m_this->getDataClass();
		ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		crTableIO::StrVec record,itemrecord;
		if(olRewardTab.valid() && itemtab.valid() &&me->getPlayerGameData() && me->getPlayerGameData()->getDataClass())
		{
			me->getPlayerGameData()->getDataClass()->excHandle(MAKECREPARAM(WCH_LockData,1));
			me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJOnlineRewardCount,param);
			unsigned char count = *(unsigned char *)param;
			me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
			int *timer = (int *)param;

			int giftGoldIndex = olRewardTab->getTitleIndex("礼金");
			int item1index = olRewardTab->getTitleIndex("道具1");
			int item2index = olRewardTab->getTitleIndex("道具2");
			int item3index = olRewardTab->getTitleIndex("道具3");
			crVector3i vec3;
			ref_ptr<crBagItemData> itemdata1,itemdata2,itemdata3;
			if(count>0 && olRewardTab->queryOneRecord(0,crArgumentParser::appItoa(count),record)>=0)
			{
				// 				if(timerIndex >= 0)
				// 				{
				// 					int timeroffset = atoi(record[timerIndex].c_str());
				// 					if( *timer == 0 && timeroffset < t-systermTimer)
				// 						*timer = timeroffset+systermTimer-t;
				// 				}
				if(giftGoldIndex>=0)
				{
					//
					int giftGold = atoi(record[giftGoldIndex].c_str());
					if(image[widgetIndex].valid() && input[widgetIndex].valid() && giftGold>0)
					{
						image[widgetIndex]->setVisiable(true);
						image[widgetIndex]->setImageName(m_iconName);
						input[widgetIndex]->setString(record[giftGoldIndex]);
						widgetIndex++;
					}
				}
				if(item1index>=0)
				{
					crArgumentParser::appAtoVec(record[item1index],vec3);
					if(vec3[0]>0)
					{
						itemdata1 = new crBagItemData;
						itemdata1->setItemID(vec3[0]);
						itemdata1->setEquipMagic(vec3[1]);
						itemdata1->setItemCount(vec3[2]);
					}
				}
				if(item2index>=0)
				{
					crArgumentParser::appAtoVec(record[item2index],vec3);
					if(vec3[0]>0)
					{
						itemdata2 = new crBagItemData;
						itemdata2->setItemID(vec3[0]);
						itemdata2->setEquipMagic(vec3[1]);
						itemdata2->setItemCount(vec3[2]);
					}
				}
				if(item3index>=0)
				{
					crArgumentParser::appAtoVec(record[item3index],vec3);
					if(vec3[0]>0)
					{
						itemdata3 = new crBagItemData;
						itemdata3->setItemID(vec3[0]);
						itemdata3->setEquipMagic(vec3[1]);
						itemdata3->setItemCount(vec3[2]);
					}
				}

				if(itemdata1.valid())
				{
					int iconIndex = itemtab->getTitleIndex("icon");
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemdata1->getItemID()),itemrecord)>=0)
					{
						if(iconIndex>=0 && image[widgetIndex].valid() && input[widgetIndex].valid())
						{
							image[widgetIndex]->setVisiable(true);
							image[widgetIndex]->setImageName(itemrecord[iconIndex]);
							input[widgetIndex]->setString(crArgumentParser::appItoa(itemdata1->getItemCount()));
							widgetIndex++;
						}
					}							
				}
				if(itemdata2.valid())
				{
					int iconIndex = itemtab->getTitleIndex("icon");
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemdata2->getItemID()),itemrecord)>=0)
					{
						if(iconIndex>=0 && image[widgetIndex].valid() && input[widgetIndex].valid())
						{
							image[widgetIndex]->setVisiable(true);
							image[widgetIndex]->setImageName(itemrecord[iconIndex]);
							input[widgetIndex]->setString(crArgumentParser::appItoa(itemdata2->getItemCount()));
							widgetIndex++;
						}
					}
				}
				if(itemdata3.valid())
				{
					int iconIndex = itemtab->getTitleIndex("icon");
					if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemdata3->getItemID()),itemrecord)>=0)
					{
						if(iconIndex>=0 && image[widgetIndex].valid() && input[widgetIndex].valid())
						{
							image[widgetIndex]->setVisiable(true);
							image[widgetIndex]->setImageName(itemrecord[iconIndex]);
							input[widgetIndex]->setString(crArgumentParser::appItoa(itemdata3->getItemCount()));
							widgetIndex++;
						}
					}
				}
			}
			if(*timer>0)
			{
				int hour = *timer/3600;
				int minute = (*timer%3600)/60;
				int second = (*timer%3600)%60;
				mulitSwitch[0]->setActiveSwitchSet(hour/10);
				mulitSwitch[1]->setActiveSwitchSet(hour%10);
				mulitSwitch[2]->setActiveSwitchSet(minute/10);
				mulitSwitch[3]->setActiveSwitchSet(minute%10);
				mulitSwitch[4]->setActiveSwitchSet(second/10);
				mulitSwitch[5]->setActiveSwitchSet(second%10);
				btGet->setEnable(false);
			}
			else
			{
				btGet->setEnable(true);
			}
			if(count<=0)
				btGet->setEnable(false);
			me->getPlayerGameData()->getDataClass()->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJOnlineRewardCanvasUpdateMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJOnlineRewardCanvasUpdateMethod::crJXJOnlineRewardCanvasUpdateMethod(){}
crJXJOnlineRewardCanvasUpdateMethod::crJXJOnlineRewardCanvasUpdateMethod(const crJXJOnlineRewardCanvasUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJOnlineRewardCanvasUpdateMethod::inputParam(int i, void *param)
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

void crJXJOnlineRewardCanvasUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_switch1 = str;
		break;
	case 2:
		m_switch2 = str;
		break;
	case 3:
		m_switch3 = str;
		break;
	case 4:
		m_switch4 = str;
		break;
	case 5:
		m_switch5 = str;
		break;
	case 6:
		m_switch6 = str;
		break;
	case 7:
		m_btGet = str;
		break;
	default:
		break;
	}
}

void crJXJOnlineRewardCanvasUpdateMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  olineRewardCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(olineRewardCanvas.valid() && me)
	{
		void *param;
		crMultiSwitch* mulitSwitch[6] = {NULL};

		mulitSwitch[0] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch1));
		mulitSwitch[1] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch2));
		mulitSwitch[2] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch3));
		mulitSwitch[3] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch4));
		mulitSwitch[4] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch5));
		mulitSwitch[5] = dynamic_cast<crMultiSwitch *>(olineRewardCanvas->getChildNode(m_switch6));
		ref_ptr<crButtonWidgetNode>  btGet = dynamic_cast<crButtonWidgetNode *>(olineRewardCanvas->getWidget(m_btGet));
		if(!mulitSwitch[0] || !mulitSwitch[1] || !mulitSwitch[2] || !mulitSwitch[3] || !mulitSwitch[4] || !mulitSwitch[5]  || !btGet) return;


		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
			int *timer = (int *)param;
			if(*timer>0)
			{
				int hour = *timer/3600;
				int minute = (*timer%3600)/60;
				int second = (*timer%3600)%60;
				mulitSwitch[0]->setActiveSwitchSet(hour/10);
				mulitSwitch[1]->setActiveSwitchSet(hour%10);
				mulitSwitch[2]->setActiveSwitchSet(minute/10);
				mulitSwitch[3]->setActiveSwitchSet(minute%10);
				mulitSwitch[4]->setActiveSwitchSet(second/10);
				mulitSwitch[5]->setActiveSwitchSet(second%10);
				btGet->setEnable(false);
			}
			else
			{
				mulitSwitch[0]->setActiveSwitchSet(0);
				mulitSwitch[1]->setActiveSwitchSet(0);
				mulitSwitch[2]->setActiveSwitchSet(0);
				mulitSwitch[3]->setActiveSwitchSet(0);
				mulitSwitch[4]->setActiveSwitchSet(0);
				mulitSwitch[5]->setActiveSwitchSet(0);
				btGet->setEnable(true);
			}
			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJOnlineRewardTimerUpdateMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJOnlineRewardTimerUpdateMethod::crJXJOnlineRewardTimerUpdateMethod(){}
crJXJOnlineRewardTimerUpdateMethod::crJXJOnlineRewardTimerUpdateMethod(const crJXJOnlineRewardTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJOnlineRewardTimerUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crStaticTextWidgetNode*)param;
		break;
	}
}

void crJXJOnlineRewardTimerUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJOnlineRewardTimerUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		//m_this->setVisiable(true);
		crTableIO::StrVec record,itemrecord;
		m_this->setColor(COLOR_YELLOW);
		ref_ptr<crCanvasNode>  onlineRewardCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		crMyPlayerData *me = crMyPlayerData::getInstance();
		if(!me) return;
		bool canGet = true;
		if(me->getPlayerGameData() && me->getPlayerGameData()->getDataClass())
		{
			me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJOnlineRewardCount,param);
			unsigned char count = *(unsigned char *)param;
			// 			me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJOnlineRewardTimer,param);
			// 			_crInt64 systermTimer = *(_crInt64 *)param; 
			// 			time_t t = time(0);
			// 			ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
			// 			int timerIndex = olRewardTab->getTitleIndex("timer");
			// 			if(olRewardTab->queryOneRecord(0,crArgumentParser::appItoa(count),record))
			// 			{
			// 				if(time>=0)
			// 				{
			// 					int timerMax = atoi(record[timerIndex].c_str());
			// 					if(timerMax>t-systermTimer)
			// 					{
			// 						canGet = false;
			// 					}
			// 				}
			// 			}

			bool dongHuaVisiable = false;

			me->getPlayerGameData()->getDataClass()->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
			int timer = *(int *)param;
			if(timer>0)
			{
				int hour = timer/3600;
				int minute = (timer%3600)/60;
				int second = (timer%3600)%60;
				char tempText[32];
				sprintf(tempText,"%02d:%02d:%02d\0",hour,minute,second);
				m_this->setString(string(tempText));
			}
			else if(timer == 0 && count>0 &&canGet)
			{
				m_this->setString(string("可领取"));
				dongHuaVisiable = true;
			}
			else
			{
				m_this->clearString();
				dongHuaVisiable = false;
			}

			crData *braindata = crBrain::getInstance()->getDataClass();
			if (braindata)
				braindata->inputParam(WCHDATA_JXJOnlineGiftDonghua	,&dongHuaVisiable);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientOnlineRewardQueryMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJClientOnlineRewardQueryMethod::crJXJClientOnlineRewardQueryMethod(){}
crJXJClientOnlineRewardQueryMethod::crJXJClientOnlineRewardQueryMethod(const crJXJClientOnlineRewardQueryMethod& handle):
	crMethod(handle)
{
}
void crJXJClientOnlineRewardQueryMethod::inputParam(int i, void *param)
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
		m_this = (crButtonWidgetNode *)param;
		break;
	}
}

void crJXJClientOnlineRewardQueryMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientOnlineRewardQueryMethod::operator()(crHandle &handle)
{
	void *param;
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	ref_ptr<crCanvasNode>  onlineRewardCanvas = m_this->getParentCanvas();
	if(onlineRewardCanvas.valid())
	{
		crData *canvasData = onlineRewardCanvas->getDataClass();
		if(canvasData && netConductor)
		{
			canvasData->getParam(WCHDATA_JXJOnlineRewardCanvasTimer,param);
			int timer = *(int *)param;
			if(timer == 0)
			{
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvOnlineRewardQuery,NULL);
				netConductor->getNetManager()->sendPacket("all",packet);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJPlayerOnlineRewardTimerUpdateMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJPlayerOnlineRewardTimerUpdateMethod::crJXJPlayerOnlineRewardTimerUpdateMethod(){}
crJXJPlayerOnlineRewardTimerUpdateMethod::crJXJPlayerOnlineRewardTimerUpdateMethod(const crJXJPlayerOnlineRewardTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJPlayerOnlineRewardTimerUpdateMethod::inputParam(int i, void *param)
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
		m_this = (crPlayerGameData *)param;
		break;
	}
}

void crJXJPlayerOnlineRewardTimerUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJPlayerOnlineRewardTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crTableIO::StrVec record;
	crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	int playerID = m_this->getPlayerID();			
	crNetDataManager *netDataManager = gameServer->getNetDataManager();
	ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
	ref_ptr<crPlayerGameData> playerGameData;
	ref_ptr<crData> playerData;
	ref_ptr<crTableIO> olRewardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJOnlineRewardTab);
	if(netDataManager && olRewardTab.valid())
	{
		playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
		if(playerGameServerPlayerData.valid())
		{
			playerGameData = playerGameServerPlayerData->getPlayerGameData();
			if(playerGameData.valid() && playerGameData->getDataClass())
			{
				playerData = playerGameData->getDataClass();

				playerData->getParam(WCHDATA_JXJOnlineRewardCount,param);
				unsigned char olRewardCount = *(unsigned char*)param;
				if(olRewardCount>=1)
				{
					playerData->getParam(WCHDATA_JXJOnlineRewardTimer,param);
					_crInt64 timer = *(_crInt64 *)param;
					time_t t = time(0);
					int olineRewardTimer = 0;
					int timerIndex = olRewardTab->getTitleIndex("timer");
					if(olRewardTab->queryOneRecord(0,crArgumentParser::appItoa(olRewardCount),record)>=0)
					{
						if(timerIndex>=0)
						{
							olineRewardTimer = atoi(record[timerIndex].c_str());
							if(olineRewardTimer>t-timer)
							{
								int onlineRewardTime = olineRewardTimer+timer-t;
								playerData->inputParam(WCHDATA_JXJPlayerOnlineRewardTimer,&onlineRewardTime);
								//stream->createBuf(4);
								//stream->_writeInt(olineRewardTimer+timer-t);
								//crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
								////
								//crPlayerDataEventPacket packet;
								//crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJClientRecvOnlineRewardTimer,stream.get());
								//gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJClientRecvOnlineRewardTimerMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJClientRecvOnlineRewardTimerMethod::crJXJClientRecvOnlineRewardTimerMethod(){}
crJXJClientRecvOnlineRewardTimerMethod::crJXJClientRecvOnlineRewardTimerMethod(const crJXJClientRecvOnlineRewardTimerMethod& handle):
	crMethod(handle)
{
}
void crJXJClientRecvOnlineRewardTimerMethod::inputParam(int i, void *param)
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

void crJXJClientRecvOnlineRewardTimerMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientRecvOnlineRewardTimerMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		crTableIO::StrVec record;
		if(m_netType == GameClient_Game)
		{
			crMyPlayerData *me = crMyPlayerData::getInstance();
			if(me && me->getPlayerGameData())
			{
				crData *playerData = me->getPlayerGameData()->getDataClass();
				playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
				int *onlineRewardTime = (int *)param;
				*onlineRewardTime = m_stream->_readInt();
				playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}

		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJClientOnlineRewardTimerUpdateMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJClientOnlineRewardTimerUpdateMethod::crJXJClientOnlineRewardTimerUpdateMethod(){}
crJXJClientOnlineRewardTimerUpdateMethod::crJXJClientOnlineRewardTimerUpdateMethod(const crJXJClientOnlineRewardTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJClientOnlineRewardTimerUpdateMethod::inputParam(int i, void *param)
{
}

void crJXJClientOnlineRewardTimerUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJClientOnlineRewardTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me && me->getPlayerGameData())
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJPlayerOnlineRewardTimer,param);
		int *onlineRewardTime = (int *)param;
		if(*onlineRewardTime>0)
			(*onlineRewardTime)--;
		playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
	}	
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJSystermMailSendMethod
//
///////////////////////////////////////////////////////////////////////////
crJXJSystermMailSendMethod::crJXJSystermMailSendMethod(){}
crJXJSystermMailSendMethod::crJXJSystermMailSendMethod(const crJXJSystermMailSendMethod& handle):
	crMethod(handle)
{
}
void crJXJSystermMailSendMethod::inputParam(int i, void *param)
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
		m_this = (crButtonWidgetNode *)param;
		break;
	}
}

void crJXJSystermMailSendMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_allCountry = str;
		break;
	case 1:
		m_wei = str;
		break;
	case 2:
		m_shu = str;
		break;
	case 3:
		m_wu = str;
		break;
	case 4:
		m_title = str;
		break;
	case 5:
		m_content = str;
		break;
	case 6:
		m_item1 = str;
		break;
	case 7:
		m_item2 = str;
		break;
	case 8:
		m_item3 = str;
		break;
	case 9:
		m_item4 = str;
		break;
	case 10:
		m_toPlayer = str;
		break;
	case 11:
		m_playerID = str;
		break;
	default:
		break;
	}
}

void crJXJSystermMailSendMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
	if(parentCanvas.valid())
	{
		ref_ptr<crCheckBoxWidgetNode>  allUserCheckBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_allCountry));
		ref_ptr<crCheckBoxWidgetNode>  weiUserCheckBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_wei));
		ref_ptr<crCheckBoxWidgetNode>  shuUserCheckBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_shu));
		ref_ptr<crCheckBoxWidgetNode>  wuUserCheckBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_wu));
		ref_ptr<crCheckBoxWidgetNode>  singlePlayerCheckBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_toPlayer));

		ref_ptr<crEditWidgetNode>  titleEdit = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_title));
		ref_ptr<crEditWidgetNode>  contentEdit = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_content));
		ref_ptr<crEditWidgetNode>  itemEdit1 = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_item1));
		ref_ptr<crEditWidgetNode>  itemEdit2 = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_item2));
		ref_ptr<crEditWidgetNode>  itemEdit3 = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_item3));
		ref_ptr<crEditWidgetNode>  itemEdit4 = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_item4));
		ref_ptr<crEditWidgetNode>  playerIDEdit = dynamic_cast<crEditWidgetNode *>(parentCanvas->getWidget(m_playerID));
		if(!allUserCheckBox || !weiUserCheckBox || !shuUserCheckBox || !wuUserCheckBox || !singlePlayerCheckBox ||	\
			!titleEdit || !contentEdit ||													\
			!itemEdit1 || !itemEdit2 || !itemEdit3 || !itemEdit4 || !playerIDEdit)
			return;
		unsigned char type = 0;
		//crVector4i items = crVector4i(-1,-1,-1,-1);
		crVector3i item1,item2,item3,item4;
		item1[2]=1;
		item2[2]=1;
		item3[2]=1;
		item4[2]=1;
		string title = titleEdit->getUTF8String();
		string content = contentEdit->getUTF8String();
		if(title.empty() || content.empty())
		{
			return;
		}
		//items[0] = atoi(itemEdit1->getUTF8String().c_str());
		//items[1] = atoi(itemEdit2->getUTF8String().c_str());
		//items[2] = atoi(itemEdit3->getUTF8String().c_str());
		//items[3] = atoi(itemEdit4->getUTF8String().c_str());
		crArgumentParser::appAtoVec(itemEdit1->getUTF8String(),item1);
		crArgumentParser::appAtoVec(itemEdit2->getUTF8String(),item2);
		crArgumentParser::appAtoVec(itemEdit3->getUTF8String(),item3);
		crArgumentParser::appAtoVec(itemEdit4->getUTF8String(),item4);
		string playername = playerIDEdit->getUTF8String();
		//if(items[0]<0 && items[1]<0 && items[2]<0 && items[3]<0)
		//{
		//	return;
		//}

		if(allUserCheckBox->getSelect())
		{
			type = SYS_MAIL_ALL;
		}
		else
		{
			if(weiUserCheckBox->getSelect())
				type |= SYS_MAIL_WEI;
			if(shuUserCheckBox->getSelect())
				type |= SYS_MAIL_SHU;
			if(wuUserCheckBox->getSelect())
				type |= SYS_MAIL_WU;
			if(singlePlayerCheckBox->getSelect())
				type = SYS_MAIL_TO_PLAYER;
		}
		//获取内容并发至服务器
		if(type == 0) 
		{
			return;
		}
		if(type == SYS_MAIL_TO_PLAYER && playername.empty())
		{
			return;
		}

		//titleEdit->clearString();
		//contentEdit->clearString();
		//itemEdit1->clearString();
		//itemEdit2->clearString();
		//itemEdit3->clearString();
		//itemEdit4->clearString();
		playerIDEdit->clearString();
		parseString(content);
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		if(type ==SYS_MAIL_TO_PLAYER)
		{
			stream->createBuf(title.size()+content.size()+playername.size()+61);
			stream->_writeUChar(type);
			stream->_writeString(title);
			stream->_writeString(content); 
			//stream->_writeVec4i(items);
			stream->_writeVec3i(item1);//12
			stream->_writeVec3i(item2);
			stream->_writeVec3i(item3);
			stream->_writeVec3i(item4);
			stream->_writeString(playername);
		}
		else
		{
			stream->createBuf(title.size()+content.size()+57);
			stream->_writeUChar(type);
			stream->_writeString(title);
			stream->_writeString(content); 
			//stream->_writeVec4i(items);
			stream->_writeVec3i(item1);
			stream->_writeVec3i(item2);
			stream->_writeVec3i(item3);
			stream->_writeVec3i(item4);
		}

		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvSystermMail,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
	}
}
void crJXJSystermMailSendMethod::parseString(std::string &str)
{
	for (string::iterator itr_str = str.begin();itr_str!=str.end();itr_str++)
	{
		if(*itr_str == '\n')
		{
			*itr_str = '/';
			itr_str++;
			itr_str = str.insert(itr_str,'n');
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvSystermMailMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvSystermMailMethod::crJXJRecvSystermMailMethod(){}
crJXJRecvSystermMailMethod::crJXJRecvSystermMailMethod(const crJXJRecvSystermMailMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvSystermMailMethod::inputParam(int i, void *param)
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

void crJXJRecvSystermMailMethod::addParam(int i, const std::string& str)
{
}

void crJXJRecvSystermMailMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		//void *param;
		if(m_netType == GameServer)
		{
			void *param;
			string playername;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			if(gsBrainData)
			{
				unsigned char type = m_stream->_readUChar();
				std::string title = m_stream->_readString();
				std::string content = m_stream->_readString();
				//crVector4i items = m_stream->_readVec4i();
				crVector3i item1 = m_stream->_readVec3i();
				crVector3i item2 = m_stream->_readVec3i();
				crVector3i item3 = m_stream->_readVec3i();
				crVector3i item4 = m_stream->_readVec3i();
				if(type == SYS_MAIL_TO_PLAYER)
				{
					playername = m_stream->_readString();
				}
				ref_ptr<crJXJSystermMailInfo> systermMailInfo = new crJXJSystermMailInfo;
				systermMailInfo->setSysMailTitle(title);
				systermMailInfo->setSysMailContent(content);
				systermMailInfo->setSysMailItems(item1,item2,item3,item4);
				systermMailInfo->setSysMailType(type);
				systermMailInfo->setPlayerCharacterName(playername);
				if(type ==SYS_MAIL_TO_PLAYER)
				{
					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermSendMailToPlayer,MAKECREPARAM(systermMailInfo.get(),NULL));
				}
				else
				{
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJSystermMailInfoDeq,param);
					SystermMailInfoDeq *sysMailInfoDeq = (SystermMailInfoDeq *)param;
					sysMailInfoDeq->push_back(systermMailInfo.get());
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));

					crServerBrainHandle::getInstance()->doEvent(WCH_JXJSystermSendMailToPlayer);
				}			

			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvSystermMailMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSystermSendMailToPlayerMethod::crJXJSystermSendMailToPlayerMethod(){}
crJXJSystermSendMailToPlayerMethod::crJXJSystermSendMailToPlayerMethod(const crJXJSystermSendMailToPlayerMethod& handle):
	crMethod(handle)
{
}
void crJXJSystermSendMailToPlayerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_systermMailInfo = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_systermMailInfo = (crJXJSystermMailInfo *)(LOCREPARAM(param64));
		}
		else
		{
			m_systermMailInfo = NULL;
		}
		break;
	}
}

void crJXJSystermSendMailToPlayerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailCfg = str;
		break;
	default:
		break;
	}
}

void crJXJSystermSendMailToPlayerMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;

		unsigned char mailtype;
		crData* serverData = m_this->getDataClass();
		serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJCreateGiftsPackDeque,param);
		CreateGiftsPackDeque *giftsPacketDeque = (CreateGiftsPackDeque *)param;
		serverData->getParam(WCHDATA_JXJMailTaskList, param);
		MailTaskDeque *mail_deque = (MailTaskDeque *)param;
		if(m_systermMailInfo.valid())
		{
			mailtype = m_systermMailInfo->getSysMailType();
			int receiver_id = 0;
			if(mailtype == SYS_MAIL_TO_PLAYER)
			{
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				if(gameServer)
				{
					crNetDataManager *netManager = gameServer->getNetDataManager();
					if(netManager)
					{
						crGameServerCallback *callback = dynamic_cast<crGameServerCallback *>(netManager->getNetCallback());
						if(callback)
						{
							receiver_id = CRNetApp::crGlobalHandle::getInstance()->gameServerQueryPlayerID(m_systermMailInfo->getPlayerCharacterName(),callback->getServerID());
							if (receiver_id>0)
								CreataSysMail(receiver_id,m_systermMailInfo.get(),giftsPacketDeque,mail_deque);
						}
					}
				}
			}
		}
		else
		{
			crData *shiliData = NULL;
			serverData->getParam(WCHDATA_JXJSystermMailInfoDeq,param);
			SystermMailInfoDeq *sysMailInfoDeq = (SystermMailInfoDeq *)param;
			serverData->getParam(WCHDATA_JXJSystemMailDeq,param);
			SystemMailDeque *sysMailPairDeq = (SystemMailDeque *)param;
			serverData->getParam(WCHDATA_JXJShiliWei,param);
			crJXJShili *shiliWei = (crJXJShili *)param;
			serverData->getParam(WCHDATA_JXJShiliShu,param);
			crJXJShili *shiliShu = (crJXJShili *)param;
			serverData->getParam(WCHDATA_JXJShiliWu,param);
			crJXJShili *shiliWu = (crJXJShili *)param;
			for (SystermMailInfoDeq::iterator itr_mail = sysMailInfoDeq->begin();itr_mail!=sysMailInfoDeq->end();itr_mail++)
			{
				mailtype = itr_mail->get()->getSysMailType();
				if(mailtype & SYS_MAIL_WEI)
				{
					shiliData = shiliWei->getDataClass();
					if(shiliData)
					{
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJPeopleMap,param);
						PeopleMap* peopleMap = (PeopleMap *)param;
						for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
						{
							//CreataSysMail(itr_people->first,itr_mail->get(),giftsPacketDeque,mail_deque);
							sysMailPairDeq->push_back(std::make_pair(itr_people->first,itr_mail->get()));
						}
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
				if(mailtype & SYS_MAIL_SHU)
				{
					shiliData = shiliShu->getDataClass();
					if(shiliData)
					{
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJPeopleMap,param);
						PeopleMap* peopleMap = (PeopleMap *)param;
						for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
						{
							//CreataSysMail(itr_people->first,itr_mail->get(),giftsPacketDeque,mail_deque);
							sysMailPairDeq->push_back(std::make_pair(itr_people->first,itr_mail->get()));
						}
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
				if(mailtype & SYS_MAIL_WU)
				{
					shiliData = shiliWu->getDataClass();
					if(shiliData)
					{
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
						shiliData->getParam(WCHDATA_JXJPeopleMap,param);
						PeopleMap* peopleMap = (PeopleMap *)param;
						for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
						{
							//CreataSysMail(itr_people->first,itr_mail->get(),giftsPacketDeque,mail_deque);
							sysMailPairDeq->push_back(std::make_pair(itr_people->first,itr_mail->get()));
						}
						shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
			sysMailInfoDeq->clear();
		}

		serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
void crJXJSystermSendMailToPlayerMethod::CreataSysMail(int playerID,crJXJSystermMailInfo *sysMailInfo,CreateGiftsPackDeque *giftsPacketDeque,MailTaskDeque *mail_deque)
{
	std::string key;
	char textBuf[64];
	std::vector<float>v_i;
	const std::string& mailTitle = sysMailInfo->getSysMailTitle();
	const std::string& mailContent = sysMailInfo->getSysMailContent();
	const crVector3i& mailitem1 = sysMailInfo->getSysRewardItems1();
	const crVector3i& mailitem2 = sysMailInfo->getSysRewardItems2();
	const crVector3i& mailitem3 = sysMailInfo->getSysRewardItems3();
	const crVector3i& mailitem4 = sysMailInfo->getSysRewardItems4();

	//unsigned char count = 1;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	crTableIO::StrVec itemrecord;
	int nameid = itemtab->getTitleIndex("name");

	//int itemSize = 0;
	ref_ptr<crPlayerRewardData> playerReward = new crPlayerRewardData;
	ref_ptr<crBagItemData> bagItemData;
	RewardItemVec &rewardItemVec = playerReward->getRewardItemsVec();
	if(mailitem1[0]>0 && mailitem1[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem1[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem1[0]);
		bagItemData->setEquipMagic(mailitem1[1]);
		bagItemData->setItemCount(mailitem1[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem2[0]>0 && mailitem2[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem2[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem2[0]);
		bagItemData->setEquipMagic(mailitem2[1]);
		bagItemData->setItemCount(mailitem2[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem3[0]>0 && mailitem3[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem3[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem3[0]);
		bagItemData->setEquipMagic(mailitem3[1]);
		bagItemData->setItemCount(mailitem3[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem4[0]>0 && mailitem4[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem4[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem4[0]);
		bagItemData->setEquipMagic(mailitem4[1]);
		bagItemData->setItemCount(mailitem4[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if (!rewardItemVec.empty())
	{
		//生成礼包
		m_this->doEvent(WCH_JXJGainGiftsPackKey, MAKECREPARAM(&key, NULL));
	}
	ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
	playerReward->buildGiftStream(streamBuf);
	ref_ptr<crCreateGiftsPack>giftsPack = new crCreateGiftsPack(playerID,key,streamBuf.get());
	giftsPacketDeque->push_back(giftsPack.get());
	//生成超文本
	int paramindex = 1;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_mailCfg,true,""))
	{
		CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJWriteRewardMailMethod(): file open error "<<m_mailCfg<<std::endl;
		//sprintf(gDebugInfo->buf(),"crJXJChatReshowMethod(): file open error %s\n\0",m_mailCfg.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	cfg_script.Push("Hypertext");
	cfg_script.Add("Content");
	cfg_script.Push("Content",paramindex++);
	v_i.clear();
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	v_i.push_back(0.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color",v_i);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",mailContent);
	cfg_script.Pop();

	if (!rewardItemVec.empty())
	{
		cfg_script.Add("Content");
		cfg_script.Push("Content", paramindex++);
		v_i.clear();
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("DrawMode", "TEXT");
		cfg_script.Add("Text", "获得如下物品：");
		cfg_script.Pop();

		for (RewardItemVec::iterator itr = rewardItemVec.begin();
			itr != rewardItemVec.end();
			++itr)
		{
			int itemid = (*itr)->getItemID();
			int itemcount = (*itr)->getItemCount();
			if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), itemrecord) >= 0)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content", paramindex++);
				v_i.clear();
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				v_i.push_back(0.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				cfg_script.Add("DrawMode", "TEXT");
				sprintf(textBuf, "%s*%d，\0", itemrecord[nameid].c_str(), itemcount);
				cfg_script.Add("Text", textBuf);
				cfg_script.Pop();
			}
		}

		cfg_script.Add("Content");
		cfg_script.Push("Content", paramindex++);
		v_i.clear();
		v_i.push_back(255.0f);
		v_i.push_back(255.0f);
		v_i.push_back(0.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("DrawMode", "TEXT|UNDERLINE");
		cfg_script.Add("Text", std::string("点击此处即可领取"));
		cfg_script.Add("Link", 1);
		cfg_script.Pop();
	}
	cfg_script.Pop();

	//cfg_script.Add("Data");
	if (!rewardItemVec.empty())
	{
		cfg_script.Push("Data");
		sprintf(textBuf, "%d,%s\0", WCHDATA_JXJGiftsPackKey, key.c_str());
		cfg_script.Add("Param", textBuf);
		for (RewardItemVec::iterator itr = rewardItemVec.begin();
			itr != rewardItemVec.end();
			++itr)
		{
			sprintf(textBuf, "%d,%d,%d,%d\0", WCHDATA_JXJGiftsPackItemInfo, (*itr)->getItemID(), (*itr)->getEquipMagic(), (*itr)->getItemCount());//0:equipmagic
			cfg_script.Add("Param", textBuf);
		}
		cfg_script.Pop();
	}

	//cfg_script.Write("JXJ/script/giftstest.cfg");

	ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
	ref_ptr<crMailInfo> mailInfo = new crMailInfo;

	crGlobalHandle::getInstance()->getScriptStream(cfg_script,mailBuffer);
	mailInfo->setRecvID(playerID);
	mailInfo->setSenderName("系统");
	mailInfo->setSenderID(0);
	mailInfo->setTitle(mailTitle);
	mailInfo->setBody(mailBuffer.get());
	mailInfo->setState(Mail_HasEx);
	mail_deque->push_back(mailInfo.get());

	cfg_script.Close();


	//ref_ptr<crStreamBuf> query_result = new crStreamBuf;
	//query_result->createBuf(1);
	//query_result->_writeBool(true);
	crPlayerDataEventPacket packet;
	packet.buildReplyPacket(packet,playerID,WCH_JXJNewMailRequest,NULL);
	CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
	ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(playerID));
	if(player_net_data.valid())
	{
		server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJSystermMailDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJSystermMailDisposeMethod::crJXJSystermMailDisposeMethod():m_availableTime(0.02f){}
crJXJSystermMailDisposeMethod::crJXJSystermMailDisposeMethod(const crJXJSystermMailDisposeMethod& handle):
	crMethod(handle),
	m_availableTime(handle.m_availableTime)
{
}
void crJXJSystermMailDisposeMethod::inputParam(int i, void *param)
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

void crJXJSystermMailDisposeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mailCfg = str;
		break;
	case 1:
		m_availableTime = atof(str.c_str());
		break;
	}
}

void crJXJSystermMailDisposeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData* serverData = m_this->getDataClass();
		serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJCreateGiftsPackDeque,param);
		CreateGiftsPackDeque *giftsPacketDeque = (CreateGiftsPackDeque *)param;
		serverData->getParam(WCHDATA_JXJMailTaskList, param);
		MailTaskDeque *mail_deque = (MailTaskDeque *)param;
		serverData->getParam(WCHDATA_JXJSystemMailDeq,param);
		SystemMailDeque *sysMailPairDeq = (SystemMailDeque *)param;
		
		if(!sysMailPairDeq->empty())
		{
			const CRCore::Timer& timer = *CRCore::Timer::instance();
			CRCore::Timer_t start_tick = timer.tick();
			float elapsedTime = 0.0f;

			SystemMailPair  mailPair;
			while(elapsedTime<m_availableTime && !sysMailPairDeq->empty())
			{
				mailPair = sysMailPairDeq->front();
				sysMailPairDeq->pop_front();

				CreataSysMail(mailPair.first,mailPair.second.get(),giftsPacketDeque,mail_deque);
				elapsedTime += timer.delta_s(start_tick,timer.tick());
			}
		}

		
		serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
void crJXJSystermMailDisposeMethod::CreataSysMail(int playerID,crJXJSystermMailInfo *sysMailInfo,CreateGiftsPackDeque *giftsPacketDeque,MailTaskDeque *mail_deque)
{
	std::string key;
	char textBuf[64];
	std::vector<float>v_i;
	const std::string& mailTitle = sysMailInfo->getSysMailTitle();
	const std::string& mailContent = sysMailInfo->getSysMailContent();
	//crVector4i mailItems = sysMailInfo->getSysRewardItems();
	const crVector3i& mailitem1 = sysMailInfo->getSysRewardItems1();
	const crVector3i& mailitem2 = sysMailInfo->getSysRewardItems2();
	const crVector3i& mailitem3 = sysMailInfo->getSysRewardItems3();
	const crVector3i& mailitem4 = sysMailInfo->getSysRewardItems4();

	//unsigned char count = 1;
	ref_ptr<crStreamBuf>streamBuf = new crStreamBuf;
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(!itemtab.valid()) return;
	crTableIO::StrVec itemrecord;
	int nameid = itemtab->getTitleIndex("name");

	//int itemSize = 0;
	ref_ptr<crPlayerRewardData> playerReward = new crPlayerRewardData;
	ref_ptr<crBagItemData> bagItemData;
	RewardItemVec &rewardItemVec = playerReward->getRewardItemsVec();
	//for (int i = 0;i<4;i++)
	//{
	//	if(mailItems[i]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailItems[i]),itemrecord)>=0)
	//	{
	//		bagItemData = new crBagItemData;
	//		bagItemData->setItemID(mailItems[i]);
	//		bagItemData->setItemCount(1);
	//		rewardItemVec.push_back(bagItemData.get());
	//	}
	//}
	if(mailitem1[0]>0 && mailitem1[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem1[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem1[0]);
		bagItemData->setEquipMagic(mailitem1[1]);
		bagItemData->setItemCount(mailitem1[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem2[0]>0 && mailitem2[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem2[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem2[0]);
		bagItemData->setEquipMagic(mailitem2[1]);
		bagItemData->setItemCount(mailitem2[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem3[0]>0 && mailitem3[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem3[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem3[0]);
		bagItemData->setEquipMagic(mailitem3[1]);
		bagItemData->setItemCount(mailitem3[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if(mailitem4[0]>0 && mailitem4[2]>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(mailitem4[0]),itemrecord)>=0)
	{
		bagItemData = new crBagItemData;
		bagItemData->setItemID(mailitem4[0]);
		bagItemData->setEquipMagic(mailitem4[1]);
		bagItemData->setItemCount(mailitem4[2]);
		rewardItemVec.push_back(bagItemData.get());
	}
	if (!rewardItemVec.empty())
	{
		//生成礼包
		m_this->doEvent(WCH_JXJGainGiftsPackKey, MAKECREPARAM(&key, NULL));
		ref_ptr<crStreamBuf> streamBuf;
		playerReward->buildGiftStream(streamBuf);
		ref_ptr<crCreateGiftsPack>giftsPack = new crCreateGiftsPack(playerID, key, streamBuf.get());
		giftsPacketDeque->push_back(giftsPack.get());
	}
	//生成超文本
	int paramindex = 1;
	rcfg::ConfigScript cfg_script;
	if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,m_mailCfg,true,""))
	{
		CRCore::notify(CRCore::FATAL)<<"JXJ::crJXJWriteRewardMailMethod(): file open error "<<m_mailCfg<<std::endl;
		//sprintf(gDebugInfo->buf(),"crJXJChatReshowMethod(): file open error %s\n\0",m_mailCfg.c_str());
		//gDebugInfo->debugInfo(CRCore::FATAL);
		return;
	}

	cfg_script.Push("Hypertext");
	cfg_script.Add("Content");
	cfg_script.Push("Content",paramindex++);
	v_i.clear();
	v_i.push_back(32.0f);
	v_i.push_back(32.0f);
	v_i.push_back(32.0f);
	v_i.push_back(255.0f);
	cfg_script.Add("Color",v_i);
	cfg_script.Add("DrawMode","TEXT");
	cfg_script.Add("Text",mailContent);
	cfg_script.Pop();

	if (!rewardItemVec.empty())
	{
		cfg_script.Add("Content");
		cfg_script.Push("Content", paramindex++);
		v_i.clear();
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(32.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("DrawMode", "TEXT");
		cfg_script.Add("Text", "获得如下物品：");
		cfg_script.Pop();

		for (RewardItemVec::iterator itr = rewardItemVec.begin();
			itr != rewardItemVec.end();
			++itr)
		{
			int itemid = (*itr)->getItemID();
			int itemcount = (*itr)->getItemCount();
			if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), itemrecord) >= 0)
			{
				cfg_script.Add("Content");
				cfg_script.Push("Content", paramindex++);
				v_i.clear();
				v_i.push_back(32.0f);
				v_i.push_back(32.0f);
				v_i.push_back(32.0f);
				v_i.push_back(255.0f);
				cfg_script.Add("Color", v_i);
				cfg_script.Add("DrawMode", "TEXT");
				sprintf(textBuf, "%d%s，\0", itemcount, itemrecord[nameid].c_str());
				cfg_script.Add("Text", textBuf);
				cfg_script.Pop();
			}
		}

		cfg_script.Add("Content");
		cfg_script.Push("Content", paramindex++);
		v_i.clear();
		v_i.push_back(140.0f);
		v_i.push_back(40.0f);
		v_i.push_back(40.0f);
		v_i.push_back(255.0f);
		cfg_script.Add("Color", v_i);
		cfg_script.Add("DrawMode", "TEXT|UNDERLINE");
		cfg_script.Add("Text", std::string("点击此处即可领取"));
		cfg_script.Add("Link", 1);
		cfg_script.Pop();
	}
	cfg_script.Pop();

	//cfg_script.Add("Data");
	if (!rewardItemVec.empty())
	{
		cfg_script.Push("Data");
		sprintf(textBuf, "%d,%s\0", WCHDATA_JXJGiftsPackKey, key.c_str());
		cfg_script.Add("Param", textBuf);
		for (RewardItemVec::iterator itr = rewardItemVec.begin();
			itr != rewardItemVec.end();
			++itr)
		{
			sprintf(textBuf, "%d,%d,%d,%d\0", WCHDATA_JXJGiftsPackItemInfo, (*itr)->getItemID(), (*itr)->getEquipMagic(), (*itr)->getItemCount());//0:equipmagic
			cfg_script.Add("Param", textBuf);
		}
		cfg_script.Pop();
	}

	//cfg_script.Write("JXJ/script/giftstest.cfg");

	ref_ptr<crStreamBuf> mailBuffer = new crStreamBuf;
	ref_ptr<crMailInfo> mailInfo = new crMailInfo;

	crGlobalHandle::getInstance()->getScriptStream(cfg_script, mailBuffer);
	mailInfo->setRecvID(playerID);
	mailInfo->setSenderName("系统");
	mailInfo->setSenderID(0);
	mailInfo->setTitle(mailTitle);
	mailInfo->setBody(mailBuffer.get());
	if (!rewardItemVec.empty())
	{
		mailInfo->setState(Mail_HasEx);
	}
	else
	{
		mailInfo->setState(Mail_Recvd);
	}
	mail_deque->push_back(mailInfo.get());

	cfg_script.Close();
	

	//ref_ptr<crStreamBuf> query_result = new crStreamBuf;
	//query_result->createBuf(1);
	//query_result->_writeBool(true);
	crPlayerDataEventPacket packet;
	packet.buildReplyPacket(packet,playerID,WCH_JXJNewMailRequest,NULL);
	CRNet::crNetConductor *server_net = CRNet::crNetContainer::getInstance()->getNetConductor(GameServer);
	ref_ptr<crGameServerPlayerData> player_net_data = dynamic_cast<CRNetApp::crGameServerPlayerData *>(server_net->getNetDataManager()->getPlayerData(playerID));
	if(player_net_data.valid())
	{
		server_net->getNetManager()->sendPacket(player_net_data->getPlayerConnectServerAddress(),packet);
	}

	//if(itemSize>0)
	//{
	//	streamBuf->createBuf(itemSize *9 + 2);
	//	streamBuf->_writeUChar(GP_RewardItem);
	//	streamBuf->_writeUChar(itemSize);
	//	for (int i = 0;i<itemSize;i++)
	//	{
	//		streamBuf->_writeInt(mailItems[i]);
	//		streamBuf->_writeInt(0);//EquipMagicSize
	//		streamBuf->_writeUChar(count);
	//	}
	//	m_this->doEvent(WCH_JXJGainGiftsPackKey,MAKECREPARAM(&key,NULL));
	//	ref_ptr<crCreateGiftsPack>giftsPack = new crCreateGiftsPack(playerID,key,streamBuf.get());
	//	giftsPacketDeque->push_back(giftsPack.get());
	//}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJShowRecvBagItemMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJShowRecvBagItemMethod::crJXJShowRecvBagItemMethod(){}
crJXJShowRecvBagItemMethod::crJXJShowRecvBagItemMethod(const crJXJShowRecvBagItemMethod& handle):
	crMethod(handle)
{
}
void crJXJShowRecvBagItemMethod::inputParam(int i, void *param)
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
		m_this = (crBrain*)param;
		break;
	}
}

void crJXJShowRecvBagItemMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvasInput = str;
		break;
	case 1:
		m_input = str;
		break;
	case 2:
		m_canvasImage = str;
		break;
	case 3:
		m_image = str;
		break;
	case 4:
		m_beibaoParentCanvas = str;
		break;
	case 5:
		m_beibao = str;
		break;
	case 6:
		crArgumentParser::appAtoVec(str.c_str(),m_pos);
		break;
	case 7:
		m_speed = atof(str.c_str());
		break;
	case 8:
		crArgumentParser::appAtoVec(str.c_str(),m_inputPos);
		break;
	case 9:
		crArgumentParser::appAtoVec(str.c_str(),m_inputTargetPos);
		break;
	case 10:
		m_inputSpeed = atof(str.c_str());
		break;
	case 11:
		m_canvasJK = str;
		break;
	case 12:
		m_imageJK = str;
		break;
	case 13:
		m_switchJK = str;
		break;
	case 14:
		m_jkBeiBao = str;
		break;
	case 15:
		crArgumentParser::appAtoVec(str.c_str(),m_JKpos);
		break;
	default:
		break;
	}
}

void crJXJShowRecvBagItemMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  inPutCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasInput);
		ref_ptr<crCanvasNode>  imageCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasImage);
		ref_ptr<crCanvasNode>  JKCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvasJK);
		ref_ptr<crCanvasNode>  packetParentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_beibaoParentCanvas);
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord,record;
		CRCore::ref_ptr<CRCore::crCanvasNode> cpInputNode;
		CRCore::ref_ptr<CRCore::crCanvasNode> cpImageNode;
		ref_ptr<crImageBoxWidgetNode>  imageBox = NULL;
		ref_ptr<crStaticTextWidgetNode>  inputBox = NULL;
		crData *imageCanvasData = NULL;
		ref_ptr<crButtonWidgetNode>  btPacket = NULL;
		ref_ptr<crButtonWidgetNode>  btJKPacket = NULL;
		ref_ptr<crMultiSwitch>  JKSwitch = NULL;

		if(packetParentCanvas.valid())
		{
			btPacket = dynamic_cast<CRUI::crButtonWidgetNode *>(packetParentCanvas->getWidget(m_beibao));
			btJKPacket = dynamic_cast<CRUI::crButtonWidgetNode *>(packetParentCanvas->getWidget(m_jkBeiBao));
		}
		void *param;
		crTableIO::StrVec itemrecord;
		crData *brainData = m_this->getDataClass();
		if(brainData && imageCanvas.valid() && itemtab.valid() && btPacket.valid() && btJKPacket.valid() && inPutCanvas.valid() && JKCanvas.valid())
		{
			int imageIndex = itemtab->getTitleIndex("icon");
			int nameIndex = itemtab->getTitleIndex("name");
			int qualityIndex = itemtab->getTitleIndex("品质");
			brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
			brainData->getParam(WCHDATA_JXJRecvBagItemDeq,param);
			if(param)
			{
				RecvBagItemDeq *itemDeq = (RecvBagItemDeq *)param;
				ref_ptr<crBagItemData> bagItem;
				int itemID = 0;
				if (!itemDeq->empty())
				{
					bagItem = itemDeq->front().second;
					int itemType = itemDeq->front().first;
					if(bagItem.valid())
					{
						brainData->getParam(WCHDATA_JXJBagItemIconNodeVec,param);
						ItemCanvasPairVec *imageNodeVec = (ItemCanvasPairVec *)param;

						if(itemType==T_Item)
						{
							itemID = bagItem->getItemID();
							crBrain::getInstance()->doEvent(WCH_JXJCheckItemType,MAKECREPARAM(itemID,&itemType));

							if(itemType == IT_Jiangka)
							{
								for ( ItemCanvasPairVec::iterator itr = imageNodeVec->begin(); itr != imageNodeVec->end(); ++itr )
								{
									if(!itr->first.valid() || !itr->second.valid()) continue;
									if(itr->first->getVisiable()==false && itr->second->getVisiable()==false && itr->second->getName() == m_canvasJK)
									{
										cpInputNode = itr->first;
										cpImageNode = itr->second;
										break;
									}
								}
								if(!cpImageNode.valid() || !cpInputNode.valid())
								{
									cpImageNode = dynamic_cast<crCanvasNode *>(JKCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
									cpImageNode->setCanFocus(false);
									//cpImageNode->setMsgTraverse(KM_All);
									cpImageNode->setCanCaptureMouse(false);
									cpImageNode->setEnable(false);
									imageBox = dynamic_cast<crImageBoxWidgetNode *>(cpImageNode->getWidget(m_imageJK));

									cpInputNode = dynamic_cast<crCanvasNode *>(inPutCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
									cpInputNode->setEnable(false);
									cpInputNode->setCanFocus(false);
									//cpInputNode->setMsgTraverse(KM_All);
									cpInputNode->setCanCaptureMouse(false);
									crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpInputNode.get(),false);

									if(imageBox.valid())
										imageBox->initWindow();
									crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpImageNode.get(),false);
									imageNodeVec->push_back(std::make_pair(cpInputNode,cpImageNode));
								}
								if(cpImageNode.valid())
								{
									JKSwitch = dynamic_cast<crMultiSwitch *>(cpImageNode->getChildNode(m_switchJK));
									ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
									if (itemtab.valid())
									{
										int itemtypeindex= itemtab->getTitleIndex("品质");
										crTableIO::StrVec itemrecord;
										if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0)
										{
											if(itemtypeindex>=0 && JKSwitch.valid())
											{
												JKSwitch->setActiveSwitchSet(atoi(itemrecord[itemtypeindex].c_str()));
											}	
										}

										imageCanvasData = cpImageNode->getDataClass();
										imageCanvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
										imageBox = dynamic_cast<crImageBoxWidgetNode *>(cpImageNode->getWidget(m_imageJK));
										if(imageBox.valid() && imageCanvasData)
										{
											if(itemID>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0)
											{
												if(imageIndex>=0)
												{
													imageBox->setImageName(itemrecord[imageIndex]);
													//控件移动
													////cpImageNode->doEvent(WCH_JXJCanvasMove,MAKEINT64());
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
													crVector3 * beginPos = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
													crVector3 * targetPos = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
													crVector3 * moveSpeed = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveDistance,param);
													float *moveDistance = (float *)param;
													*beginPos = m_JKpos;
													(*beginPos)[2] =  0;
													const crBoundingBox & bbox = btJKPacket->getBoundBox();
													*targetPos = bbox.center();
													(*targetPos)[2] = 0.0f;

													crVector3 dirVector = *targetPos - *beginPos;
													float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
													*moveDistance = length;
													(*moveSpeed)[0] = m_speed* dirVector[0]/length;
													(*moveSpeed)[1] = m_speed* dirVector[1]/length;
													(*moveSpeed)[2] = 0.0f;
													cpImageNode->setMatrix(crMatrix::translate(m_JKpos));

													crFilterRenderManager::getInstance()->showCanvas(cpImageNode.get(),true);
												}
											}
										}
										imageCanvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
									}
								}
							}
							else
							{
								itemID = bagItem->getItemID();
								for ( ItemCanvasPairVec::iterator itr = imageNodeVec->begin(); itr != imageNodeVec->end(); ++itr )
								{
									if(!itr->first.valid() || !itr->second.valid()) continue;
									if(itr->first->getVisiable()==false && itr->second->getVisiable()==false && itr->second->getName() == m_canvasImage)
									{
										cpInputNode = itr->first;
										cpImageNode = itr->second;
										break;
									}
								}
								if(!cpImageNode.valid() || !cpInputNode.valid())
								{
									cpImageNode = dynamic_cast<crCanvasNode *>(imageCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
									cpImageNode->setCanFocus(false);
									//cpImageNode->setMsgTraverse(KM_All);
									cpImageNode->setCanCaptureMouse(false);
									imageBox = dynamic_cast<crImageBoxWidgetNode *>(cpImageNode->getWidget(m_image));
									if(imageBox.valid())
										imageBox->initWindow();

									cpInputNode = dynamic_cast<crCanvasNode *>(inPutCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
									cpInputNode->setEnable(false);
									cpInputNode->setCanFocus(false);
									//cpInputNode->setMsgTraverse(KM_All);
									cpInputNode->setCanCaptureMouse(false);
									crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpInputNode.get(),false);

									//crFilterRenderManager::getInstance()->getCanvasRoot()->addChild(cpImageNode.get());
									crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpImageNode.get(),false);
									imageNodeVec->push_back(std::make_pair(cpInputNode,cpImageNode));
								}
								if(cpImageNode.valid())
								{
									imageCanvasData = cpImageNode->getDataClass();
									if (imageCanvasData)
									{
										imageCanvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
										imageBox = dynamic_cast<crImageBoxWidgetNode *>(cpImageNode->getWidget(m_image));
										if(imageBox.valid() && imageCanvasData)
										{
											//imageBox->initWindow();
											if(itemID>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0)
											{
												if(imageIndex>=0)
												{
													imageBox->setImageName(itemrecord[imageIndex]);
													//控件移动
													//cpImageNode->doEvent(WCH_JXJCanvasMove,MAKEINT64());
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
													crVector3 * beginPos = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
													crVector3 * targetPos = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
													crVector3 * moveSpeed = (crVector3 *)param;
													imageCanvasData->getParam(WCHDATA_JXJMoveDistance,param);
													float *moveDistance = (float *)param;
													*beginPos = m_pos;
													(*beginPos)[2] =  0;
													const crBoundingBox & bbox = btPacket->getBoundBox();
													*targetPos = bbox.center();
													(*targetPos)[2] = 0.0f;

													crVector3 dirVector = *targetPos - *beginPos;
													float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
													*moveDistance = length;
													(*moveSpeed)[0] = m_speed* dirVector[0]/length;
													(*moveSpeed)[1] = m_speed* dirVector[1]/length;
													(*moveSpeed)[2] = 0.0f;
													cpImageNode->setMatrix(crMatrix::translate(m_pos));

													crFilterRenderManager::getInstance()->showCanvas(cpImageNode.get(),true);
												}
											}
										}
										imageCanvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
									}
								}
							}

							if(cpInputNode.valid() && cpInputNode->getDataClass())
							{
								crData *inputCanvasData = cpInputNode->getDataClass();
								inputCanvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
								inputBox = dynamic_cast<crStaticTextWidgetNode *>(cpInputNode->getWidget(m_input));
								if(inputBox.valid())
								{
									if(itemID>0 && itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemID),itemrecord)>=0)
									{
										if(nameIndex>=0)
										{
											int colorindex = qualitytab->getTitleIndex("color");
											crVector4f colormode = COLOR_WHITE;
											int quality = atoi(itemrecord[qualityIndex].c_str());
											if(itemType == IT_Jiangka && quality>CardColor_Orange) quality = CardColor_Orange;

											if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(quality),colorrecord)>=0)
											{
												if (colorindex>=0)
												{
													crArgumentParser::appAtoVec(colorrecord[colorindex],colormode);
												}
											}
											colormode /=255.0;

											std::string strItemName = itemrecord[nameIndex];
											if(itemType == IT_Jiangka) 
											{
												switch(atoi(itemrecord[qualityIndex].c_str()) - CardColor_Orange)
												{
												case 1:
													strItemName+=std::string("一星");
													break;
												case 2:
													strItemName+=std::string("二星");
													break;
												case 3:
													strItemName+=std::string("三星");
													break;
												case 4:
													strItemName+=std::string("四星");
													break;
												case 5:
													strItemName+=std::string("五星");
													break;
												}
											}
											inputBox->setColor(colormode);
											inputBox->setString(strItemName+string("*")+crArgumentParser::appItoa(bagItem->getItemCount()));
											inputBox->setAlignment(CRText::crText::CENTER_BASE_LINE);
											//控件移动
											//cpImageNode->doEvent(WCH_JXJCanvasMove,MAKEINT64());
											inputCanvasData->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
											crVector3 * beginPos = (crVector3 *)param;
											inputCanvasData->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
											crVector3 * targetPos = (crVector3 *)param;
											inputCanvasData->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
											crVector3 * moveSpeed = (crVector3 *)param;
											inputCanvasData->getParam(WCHDATA_JXJMoveDistance,param);
											float *moveDistance = (float *)param;

											*beginPos = m_inputPos;
											(*beginPos)[2] =  0;
											//*targetPos = m_inputTargetPos;
											//(*targetPos)[2] = 0.0f;

											(*targetPos)[0] = CRCore::rangei(-25,25).get_random() * 0.01f;
											(*targetPos)[1] = m_inputTargetPos.y();
											(*targetPos)[2] = 0.0f;

											crVector3 dirVector = *targetPos - *beginPos;
											float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
											*moveDistance = length;

											(*moveSpeed)[0] = m_inputSpeed* dirVector[0]/length;
											(*moveSpeed)[1] = m_inputSpeed* dirVector[1]/length+0.1f;
											(*moveSpeed)[2] = 0.0f;
											cpInputNode->setMatrix(crMatrix::translate(m_inputPos));
											cpInputNode->setCanvasPos(CANVAS_TOPMOST);
											//cpInputNode->setVisiable(true);
											crFilterRenderManager::getInstance()->showCanvas(cpInputNode.get(),true);
										}
									}
								}
								inputCanvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
							}
						}
						else
						{
							for ( ItemCanvasPairVec::iterator itr = imageNodeVec->begin(); itr != imageNodeVec->end(); ++itr )
							{
								if(!itr->first.valid()) continue;
								if(itr->first->getVisiable()==false && !itr->second.valid())
								{
									cpInputNode = itr->first;
									break;
								}
							}
							
							if(!cpInputNode.valid())
							{
								cpInputNode = dynamic_cast<crCanvasNode *>(inPutCanvas->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
								cpInputNode->setEnable(false);
								cpInputNode->setCanFocus(false);
								cpInputNode->setCanvasPos(CANVAS_TOPMOST);
								//cpInputNode->setMsgTraverse(KM_All);
								cpInputNode->setCanCaptureMouse(false);
								crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpInputNode.get(),false);
								cpImageNode = NULL;
								imageNodeVec->push_back(std::make_pair(cpInputNode,cpImageNode));
							}

							if(cpInputNode.valid() && cpInputNode->getDataClass())
							{
								crData *inputCanvasData = cpInputNode->getDataClass();
								inputCanvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
								inputBox = dynamic_cast<crStaticTextWidgetNode *>(cpInputNode->getWidget(m_input));
								ref_ptr<crTableIO> troopTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJTroopsTab);
								int trnameIndex = troopTab->getTitleIndex("name");
								if(inputBox.valid())
								{
									crVector4f colormode = COLOR_YELLOW;
									colormode /=255.0;
									std::string text = string("+");
									switch (itemType)
									{
									case T_Food:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("粮草");
										break;
									case T_Wood:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("木材");
										break;
									case T_Iron:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("铁矿");
										break;
									case T_Horse:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("马匹");
										break;
									case T_Copper:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("铜钱");
										break;
									case T_Exp:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("经验");
										break;
									case T_Achievement:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("战功");
										break;
									case T_Giftgold:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("礼金");
										break;
									case T_TroopsEquip:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("军械");
										break;
									case T_Exploit:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("功勋");
										break;
									case T_Contribute:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("国家贡献");
										break;
									case T_Gold:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("元宝");
										break;
									case T_JiangHun:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("将魂");
										break;
									case T_CardImpression:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("好感度");
										break;
									case T_JunLing:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("军令");
										break;
									case T_FreeRollCard:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("免费抽将次数");
										break;
									case T_FreeRefine:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("免费精炼次数");
										break;
									case T_VIPExp:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("VIP经验");
										break;
									case T_Zhanquan:
										text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+string("征战券");
										break;
									case T_GuoZhanJiFen:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("国战积分");
										break;
									case T_GroupContribute:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("军团贡献");
										break;
									case T_AchievePoint:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("成就积分");
										break;
									case T_SuperEquipLvupPoint:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("注灵点数");
										break;
									case T_SuperEquipExp:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("神器经验");
										break;
									case T_SuperEquipCoin:
										text += crArgumentParser::appItoa(abs(bagItem->getItemID())) + string("神器币");
										break;
									default:
										if(itemType>T_END)
										{
											if(troopTab->queryOneRecord(0,crArgumentParser::appItoa(itemType),record)>=0 && trnameIndex>=0)
											{
												text+=crArgumentParser::appItoa(abs(bagItem->getItemID()))+record[trnameIndex];
											}
										}
										break;
									}
									inputBox->setColor(colormode);
									inputBox->setString(text);
									inputBox->setAlignment(CRText::crText::CENTER_BASE_LINE);
									//控件移动
									//cpImageNode->doEvent(WCH_JXJCanvasMove,MAKEINT64());
									inputCanvasData->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
									crVector3 * beginPos = (crVector3 *)param;
									inputCanvasData->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
									crVector3 * targetPos = (crVector3 *)param;
									inputCanvasData->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
									crVector3 * moveSpeed = (crVector3 *)param;
									inputCanvasData->getParam(WCHDATA_JXJMoveDistance,param);
									float *moveDistance = (float *)param;

									*beginPos = m_inputPos;
									(*beginPos)[2] =  0;

									(*targetPos)[0] = CRCore::rangei(-25,25).get_random() * 0.01f;
									(*targetPos)[1] = m_inputTargetPos.y();
									(*targetPos)[2] = 0.0f;

									crVector3 dirVector = *targetPos - *beginPos;
									float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);
									*moveDistance = length;

									(*moveSpeed)[0] = m_inputSpeed* dirVector[0]/length;
									(*moveSpeed)[1] = m_inputSpeed* dirVector[1]/length+0.1f;
									(*moveSpeed)[2] = 0.0f;
									cpInputNode->setMatrix(crMatrix::translate(m_inputPos));
									
									//cpInputNode->setCanFocus(false);
									//cpInputNode->setVisiable(true);
									crFilterRenderManager::getInstance()->showCanvas(cpInputNode.get(),true);
									
								}
								inputCanvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
							}

						}
					}
					itemDeq->pop_front();
				}
			}
			brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJShowRecvBagItemCanvasUpdateMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJShowRecvBagItemCanvasUpdateMethod::crJXJShowRecvBagItemCanvasUpdateMethod():m_index(0),m_rotate(0.0f){}
crJXJShowRecvBagItemCanvasUpdateMethod::crJXJShowRecvBagItemCanvasUpdateMethod(const crJXJShowRecvBagItemCanvasUpdateMethod& handle):
	crMethod(handle),
	m_index(handle.m_index),
	m_rotate(handle.m_rotate)
{
}
void crJXJShowRecvBagItemCanvasUpdateMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float*)LOCREPARAM(param64);
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJShowRecvBagItemCanvasUpdateMethod::addParam(int i, const std::string& str)
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

void crJXJShowRecvBagItemCanvasUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *data = m_this->getDataClass();
		if(data)
		{
			data->excHandle(MAKECREPARAM(WCH_LockData,1));
			data->getParam(WCHDATA_JXJMoveCanvasBeginPos,param);
			crVector3 *beginPos = (crVector3 *)param;
			data->getParam(WCHDATA_JXJMoveCanvasTargetPos,param);
			crVector3 targetPos = *(crVector3 *)param; 
			data->getParam(WCHDATA_JXJMoveCanvasSpeed,param);
			crVector3  * speedDir = (crVector3 *)param;
			data->getParam(WCHDATA_JXJMoveDistance,param);
			float moveDistance = *(float *)param;

			crVector3 dirVector = targetPos - *beginPos;
			float length = sqrtf(dirVector[0]*dirVector[0]+dirVector[1]*dirVector[1]+dirVector[2]*dirVector[2]);

			float offset = 0.0f;
			if((*speedDir)[1]>0)
			{
				offset = (*beginPos)[1] - targetPos[1];
			}
			else
			{
				offset = targetPos[1]-(*beginPos)[1];
			}
			if(offset<0)
			{
				if(m_index==1)
				{
					(*speedDir)[1]-=0.005;
					*beginPos += *speedDir * m_dt;
					m_this->setMatrix(/*crMatrix::scale(length/moveDistance,length/moveDistance,1.0f)**/crMatrix::translate(*beginPos));
				}
				else if(m_index ==2)
				{
					*beginPos += *speedDir * m_dt;
					m_this->setMatrix(crMatrix::scale(length/moveDistance,length/moveDistance,1.0f)*crMatrix::translate(*beginPos));
					//m_rotate += PI * 0.5f;
				}
				else
				{
					*beginPos += *speedDir * m_dt;
					m_this->setMatrix(crMatrix::translate(*beginPos));
				}
			}
			else
			{
				//m_this->setVisiable(false);
				crFilterRenderManager::getInstance()->closeCanvas(m_this);
				m_rotate = 0.0f;
				// 				int autotrans = 1;
				// 				crAutoTransCallback *callback;
				// 				callback = dynamic_cast<crAutoTransCallback *>(m_this->getCullCallback("AutoTrans"));
				// 				if(!callback)
				// 				{
				// 					callback = dynamic_cast<crAutoTransCallback *>(crEventCallbackManager::getInstance()->getEventCallback("AutoTrans"));
				// 					m_this->addCullCallback(callback);
				// 				}
				// 				if(callback)
				// 				{
				// 					callback->setAutoTrans(5.0f,0.0f);
				// 					callback->setSpeed(0.1f);
				// 				}
				//(dynamic_cast<crObject *>(itr->get()))->setDrawOutLine(false);

			}
			data->excHandle(MAKECREPARAM(WCH_LockData,0));
			//crVector3 
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJIsGetSelectRolesSpeedMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJIsGetSelectRolesSpeedMethod::crJXJIsGetSelectRolesSpeedMethod():
	m_isCheckSelectRoles(true){}
crJXJIsGetSelectRolesSpeedMethod::crJXJIsGetSelectRolesSpeedMethod(const crJXJIsGetSelectRolesSpeedMethod& handle):
	crMethod(handle),
	m_isCheckSelectRoles(true)
{
}
void crJXJIsGetSelectRolesSpeedMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_isCheckSelectRoles = HICREPARAM(param64) == 0;
		}
		break;
	}
}

void crJXJIsGetSelectRolesSpeedMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strCheckBox = str;
		break;
	}
}
void crJXJIsGetSelectRolesSpeedMethod::operator()(crHandle &handle)
{
	//void *param;
	bool isCheck = false;
	if(crGlobalHandle::isClient() && m_isCheckSelectRoles)
	{
// 		crCamera *camera = crCameraManager::getInstance()->getCurrentCamera();
// 		if(camera)
// 		{
// 			crData *cameraData = camera->getAttachedNode()->getDataClass();
// 			if(cameraData)
// 			{
// 				cameraData->getParam(WCHDATA_JXJIsChangeMoveSpeed,param);
// 				isCheck = *(bool *)param;
// 			}
// 		}
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		if(canvas.valid())
		{
			ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(canvas->getWidget(m_strCheckBox));
			if(checkBox.valid())
			{
				isCheck = checkBox->getSelect();
			}
		}
		
	}
	handle.outputParam(0,&isCheck);
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJGetSelectRolesSpeedMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJGetSelectRolesSpeedMethod::crJXJGetSelectRolesSpeedMethod():
	m_this(NULL),
	m_output(NULL){}
crJXJGetSelectRolesSpeedMethod::crJXJGetSelectRolesSpeedMethod(const crJXJGetSelectRolesSpeedMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_output(NULL)
{
}
void crJXJGetSelectRolesSpeedMethod::inputParam(int i, void *param)
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
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			if(HICREPARAM(param64) == 0)
			{
				m_output = (float *)(LOCREPARAM(param64));
			}
		}
		break;
	}
}
void crJXJGetSelectRolesSpeedMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetSelectRolesSpeedMethod::operator()(crHandle &handle)
{
	if(crRunGameHandle::getInstance()->isInGame() && m_output)
	{
		float *output = m_output;
		crInstanceItem *_this = m_this;
		float thisSpeed = 0.0f;
		m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&thisSpeed,1));

		float itemSpeed = 0.0f;
		bool isRoleInVec = false;
		ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
		if(camera.valid())
		{
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			if(cameraData)
			{
				void *param;
				CRCore::ScopedLock<crData> lock(*cameraData);
				cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
				SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
				for (SelectMyRoleVec::iterator itr_item = selectMyRoles->begin();itr_item!=selectMyRoles->end();itr_item++)
				{
					if(itr_item->get()==_this)
					{
						isRoleInVec = true;
						break;
					}
				}
				if(isRoleInVec)
				{
					for (SelectMyRoleVec::iterator itr_item = selectMyRoles->begin();itr_item!=selectMyRoles->end();itr_item++)
					{
						if(itr_item->get()==_this) continue;
						(*itr_item)->doEvent(WCH_GetSpeed,MAKECREPARAM(&itemSpeed,1));
						if(itemSpeed<thisSpeed)
						{
							thisSpeed = itemSpeed;
						}
					}
				}
			}
		}
		*output = thisSpeed;
		if(*output<0.0f) *output = 0.0f;
	}
}

/////////////////////////////////////////
//
//crJXJSystermNoticeCreateMethod
//
/////////////////////////////////////////
crJXJSystermNoticeCreateMethod::crJXJSystermNoticeCreateMethod():m_chatInfo(NULL){}
crJXJSystermNoticeCreateMethod::crJXJSystermNoticeCreateMethod(const crJXJSystermNoticeCreateMethod& handle):
	crMethod(handle),
	m_chatInfo(NULL)
{
}
void crJXJSystermNoticeCreateMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_chatInfo = NULL;
		}
		break;
	case 1:
		m_this = (crServerBrainHandle*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_chatInfo = (crJXJChatMessageInfo *)(LOCREPARAM(param64));
		}
		else
		{
			m_chatInfo = NULL;
		}
		break;
	}
}

void crJXJSystermNoticeCreateMethod::addParam(int i, const std::string& str)
{
}

void crJXJSystermNoticeCreateMethod::operator()(crHandle &handle)
{
	if(m_chatInfo)
	{
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		if(m_chatInfo->getLeaderID()>0)
		{
			data->getParam(WCHDATA_JXJTeamMessageDeq,param);
			TeamMessageDeq *messageDeq = (TeamMessageDeq *)param;
			messageDeq->push_back(m_chatInfo);
		}
		else if(m_chatInfo->getGroupID() > 0)
		{
			data->getParam(WCHDATA_JXJGroupMessageDeq,param);
			GroupMessageDeq *messageDeq = (GroupMessageDeq *)param;
			messageDeq->push_back(m_chatInfo);
		}
		else if(!m_chatInfo->getRecvName().empty())
		{
			data->getParam(WCHDATA_JXJPlayerMessageDeq,param);
			PlayerMessageDeq *messageDeq = (PlayerMessageDeq *)param;
			messageDeq->push_back(m_chatInfo);
		}
		else if(m_chatInfo->getSenderID()>0)
		{
			data->getParam(WCHDATA_JXJBroadcastMessageDeq,param);
			BroadcastMessageDeq *messageDeq = (BroadcastMessageDeq *)param;
			messageDeq->push_back(m_chatInfo);
		}
		else
		{
			data->getParam(WCHDATA_JXJSysBroadcastMessageDeq,param);
			BroadcastMessageDeq *sysmessageDeq = (BroadcastMessageDeq *)param;
			sysmessageDeq->push_back(m_chatInfo);
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJBroadcastMessageDisposeMethod
//
/////////////////////////////////////////
crJXJBroadcastMessageDisposeMethod::crJXJBroadcastMessageDisposeMethod():
	m_this(NULL),
	m_id(WCHDATA_JXJBroadcastMessageDeq){}
crJXJBroadcastMessageDisposeMethod::crJXJBroadcastMessageDisposeMethod(const crJXJBroadcastMessageDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_id(handle.m_id)
{
}
void crJXJBroadcastMessageDisposeMethod::inputParam(int i, void *param)
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
	//case 2:
	//	if(param)
	//	{
	//		CREPARAM& param64 = *(CREPARAM*)param;
	//		int lparam = LOINT64(param64);
	//		if(lparam)
	//			m_dt = (float*)lparam;
	//	}
	//	else
	//	{
	//		m_dt = NULL;
	//	}
	//	break;
	}
}

void crJXJBroadcastMessageDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_id = atoi(str.c_str());
		break;
	}
}

void crJXJBroadcastMessageDisposeMethod::operator()(crHandle &handle)
{
	ref_ptr<crJXJChatMessageInfo> chatInfo;
	void *param;
	crData *data = m_this->getDataClass();
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(m_id,param);
	BroadcastMessageDeq *broadcastMessageDeq = (BroadcastMessageDeq *)param;
	if(!broadcastMessageDeq->empty())
	{
		chatInfo = broadcastMessageDeq->front().get();
		broadcastMessageDeq->pop_front();
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(chatInfo.valid())
	{
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crNetManager *netManager = gameServer->getNetManager();
		ref_ptr<crGameServerPlayerData> playerData;
		crJXJGameChatPacket packet;
		crStreamBuf *pkgstream;
		crData *pdata = NULL;
		int pkgsize;
		if(chatInfo->getBroadcastShiliID() == 0)
		{//全服广播
			if(m_id == WCHDATA_JXJSysBroadcastMessageDeq)
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatNull,chatInfo->getSenderShiliID(),chatInfo->getSender()/*string("【系统消息】")*/,chatInfo->getText());
			else
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatInWorld,chatInfo->getSenderShiliID(),chatInfo->getSender(),chatInfo->getText());

			pkgstream = packet.getStreamBuf();
			pkgsize = pkgstream->getBufSize();
			netDataManager->lockPlayerDataMap();
			crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if (playerData.valid() && playerData->getCharacterName() != chatInfo->getSender())
				{
					pdata = playerData->getPlayerGameData()->getDataClass();
					pdata -> getParam(WCHDATA_JXJBlackNameSet,param);
					BlackNameSet *blacknameset = (BlackNameSet *)param;
					BlackNameSet::iterator finditr = blacknameset->find(chatInfo->getSender());
					if (finditr == blacknameset->end())
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(playerData->getPlayerID());
						pkgstream->setBufSize(pkgsize);
						netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
			netDataManager->unLockPlayerDataMap();
		}
		else if(chatInfo->getBroadcastShiliID()>=c_startShiliID)
		{
			crJXJGameChatPacket packet;
			crJXJGameChatPacket::buildReplyPacket(packet,0,ChatInCountry,chatInfo->getSender(),chatInfo->getText());
			pkgstream = packet.getStreamBuf();
			pkgsize = pkgstream->getBufSize();

			data->getParam(WCHDATA_JXJShiliWei+chatInfo->getBroadcastShiliID()-c_startShiliID,param);
			crJXJShili *jxjshili = (crJXJShili *)param;
			if(jxjshili && jxjshili->getDataClass())
			{
				crData *shiliData = jxjshili->getDataClass();
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
				shiliData->getParam(WCHDATA_JXJPeopleMap,param);
				PeopleMap* peopleMap = (PeopleMap *)param;
				for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
				{
					playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(itr_people->first));
					if (playerData.valid() && playerData->getPlayerGameData() && playerData->getPlayerGameData()->getDataClass() && playerData->getCharacterName() != chatInfo->getSender())
					{
						pdata = playerData->getPlayerGameData()->getDataClass();
						pdata -> getParam(WCHDATA_JXJBlackNameSet,param);
						BlackNameSet *blacknameset = (BlackNameSet *)param;
						BlackNameSet::iterator finditr = blacknameset->find(chatInfo->getSender());
						if (finditr == blacknameset->end())
						{
							pkgstream->seekBegin();
							pkgstream->_writeInt(playerData->getPlayerID());
							pkgstream->setBufSize(pkgsize);
							netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
				shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJGroupMessageDisposeMethod
//
/////////////////////////////////////////
crJXJGroupMessageDisposeMethod::crJXJGroupMessageDisposeMethod():
	m_this(NULL){}
crJXJGroupMessageDisposeMethod::crJXJGroupMessageDisposeMethod(const crJXJGroupMessageDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJGroupMessageDisposeMethod::inputParam(int i, void *param)
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
		//case 2:
		//	if(param)
		//	{
		//		CREPARAM& param64 = *(CREPARAM*)param;
		//		int lparam = LOINT64(param64);
		//		if(lparam)
		//			m_dt = (float*)lparam;
		//	}
		//	else
		//	{
		//		m_dt = NULL;
		//	}
		//	break;
	}
}

void crJXJGroupMessageDisposeMethod::addParam(int i, const std::string& str)
{
}

void crJXJGroupMessageDisposeMethod::operator()(crHandle &handle)
{
	ref_ptr<crJXJChatMessageInfo> chatInfo;
	void *param;
	crData *data = m_this->getDataClass();
	crData *pdata = NULL;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJGroupMessageDeq,param);
	GroupMessageDeq *groupMessageDeq = (GroupMessageDeq *)param;
	if(!groupMessageDeq->empty())
	{
		chatInfo = groupMessageDeq->front().get();
		groupMessageDeq->pop_front();
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(chatInfo.valid())
	{
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJConsortiaMap,param);
		ConsortiaMap *consortiamap = ( ConsortiaMap *)param;
		ConsortiaMap::iterator mapitr;
		mapitr = consortiamap->find(chatInfo->getGroupID());
		if (mapitr != consortiamap->end())
		{
			crData *condata = mapitr->second->getDataClass();
			if (condata)
			{
				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
				crNetDataManager *netDataManager = gameServer->getNetDataManager();
				crNetManager *netManager = gameServer->getNetManager();
				ref_ptr<crGameServerPlayerData> playerData;
				crJXJGameChatPacket packet;
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatInGroup,chatInfo->getSender(),chatInfo->getText());
				crStreamBuf *pkgstream = packet.getStreamBuf();
				int pkgsize = pkgstream->getBufSize();

				condata->excHandle(MAKECREPARAM(WCH_LockData,1));
				condata->getParam(WCHDATA_JXJConsortiaMemberMap,param);
				ConsortiaMemberMap *membermap = (ConsortiaMemberMap *)param;
				for (ConsortiaMemberMap::iterator mitr = membermap->begin();
					mitr!=membermap->end();
					++mitr)
				{
					if (mitr->second->getName() != chatInfo->getSender())
					{
						playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(mitr->second->getPlayerID()));
						if(playerData.valid())
						{
							pdata = playerData->getPlayerGameData()->getDataClass();
							pdata -> getParam(WCHDATA_JXJBlackNameSet,param);
							BlackNameSet *blacknameset = (BlackNameSet *)param;
							BlackNameSet::iterator finditr = blacknameset->find(chatInfo->getSender());
							if (finditr == blacknameset->end())
							{
								pkgstream->seekBegin();
								pkgstream->_writeInt(playerData->getPlayerID());
								pkgstream->setBufSize(pkgsize);
								netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
							}
						}
					}
				}
				condata->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}
/////////////////////////////////////////
//
//crJXJTeamMessageDisposeMethod
//
/////////////////////////////////////////
crJXJTeamMessageDisposeMethod::crJXJTeamMessageDisposeMethod():
	m_this(NULL),
	m_countPerFrame(50){}
crJXJTeamMessageDisposeMethod::crJXJTeamMessageDisposeMethod(const crJXJTeamMessageDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_countPerFrame(handle.m_countPerFrame)
{
}
void crJXJTeamMessageDisposeMethod::inputParam(int i, void *param)
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

void crJXJTeamMessageDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_countPerFrame = atoi(str.c_str());
		break;
	}
}

void crJXJTeamMessageDisposeMethod::operator()(crHandle &handle)
{
	m_taskVec.resize(0);
	m_taskVec.reserve(m_countPerFrame);
	ref_ptr<crJXJChatMessageInfo> chatInfo;
	void *param;
	crData *data = m_this->getDataClass();
	//std::vector< ref_ptr<crJXJChatMessageInfo> > ChatMessageVec;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJTeamMessageDeq,param);
	TeamMessageDeq *messageDeq = (TeamMessageDeq *)param;
	int i = 0;
	while(!messageDeq->empty() && i<m_countPerFrame)
	{
		chatInfo = messageDeq->front().get();
		m_taskVec.push_back(chatInfo);
		messageDeq->pop_front();
		i++;
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(!m_taskVec.empty())
	{
		ref_ptr<crGameServerPlayerData> leaderData;
		ref_ptr<crGameServerPlayerData> recvPlayerData;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crNetManager *netManager = gameServer->getNetManager();
		crJXJGameChatPacket packet;
		crStreamBuf *pkgstream;
		TeamMemberVec *myTeam;
		int pkgsize;
		crData *ldata;
		crData *pdata;
		for( std::vector< ref_ptr<crJXJChatMessageInfo> >::iterator itr = m_taskVec.begin();
			itr != m_taskVec.end();
			++itr )
		{
			chatInfo = itr->get();
			leaderData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(chatInfo->getLeaderID()));
			if (leaderData.valid() && leaderData->getPlayerGameData() && leaderData->getPlayerGameData()->getDataClass())
			{
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatInTeam,chatInfo->getSender(),chatInfo->getText());
				pkgstream = packet.getStreamBuf();
				pkgsize = pkgstream->getBufSize();
				ldata = leaderData->getPlayerGameData()->getDataClass();
				ldata->getParam(WCHDATA_JXJMyTeam,param);
				myTeam = (TeamMemberVec *)param;
				for (TeamMemberVec::iterator teamitr = myTeam->begin(); teamitr != myTeam->end(); ++teamitr)
				{
					if ((*teamitr)->getName() != chatInfo->getSender())
					{
						recvPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData((*teamitr)->getPlayerID()));
						if(recvPlayerData.valid() && recvPlayerData->getPlayerGameData() && recvPlayerData->getPlayerGameData()->getDataClass())
						{
							pdata = recvPlayerData->getPlayerGameData()->getDataClass();
							pdata -> getParam(WCHDATA_JXJBlackNameSet,param);
							BlackNameSet *blacknameset = (BlackNameSet *)param;
							BlackNameSet::iterator finditr = blacknameset->find(chatInfo->getSender());
							if (finditr == blacknameset->end())
							{
								pkgstream->seekBegin();
								pkgstream->_writeInt(recvPlayerData->getPlayerID());
								pkgstream->setBufSize(pkgsize);
								netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
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
//crJXJPlayerMessageDisposeMethod
//
/////////////////////////////////////////
crJXJPlayerMessageDisposeMethod::crJXJPlayerMessageDisposeMethod():
	m_this(NULL),
	m_countPerFrame(50){}
crJXJPlayerMessageDisposeMethod::crJXJPlayerMessageDisposeMethod(const crJXJPlayerMessageDisposeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_countPerFrame(handle.m_countPerFrame)
{
}
void crJXJPlayerMessageDisposeMethod::inputParam(int i, void *param)
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

void crJXJPlayerMessageDisposeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_countPerFrame = atoi(str.c_str());
		break;
	}
}

void crJXJPlayerMessageDisposeMethod::operator()(crHandle &handle)
{
	m_taskVec.resize(0);
	m_taskVec.reserve(m_countPerFrame);
	ref_ptr<crJXJChatMessageInfo> chatInfo;
	void *param;
	crData *pdata = NULL;
	crData *data = m_this->getDataClass();
	std::vector< ref_ptr<crJXJChatMessageInfo> > ChatMessageVec;
	data->excHandle(MAKECREPARAM(WCH_LockData,1));
	data->getParam(WCHDATA_JXJPlayerMessageDeq,param);
	PlayerMessageDeq *messageDeq = (PlayerMessageDeq *)param;
	int i = 0;
	while(!messageDeq->empty() && i<m_countPerFrame)
	{
		chatInfo = messageDeq->front().get();
		m_taskVec.push_back(chatInfo);
		messageDeq->pop_front();
		i++;
	}
	data->excHandle(MAKECREPARAM(WCH_LockData,0));
	if(!m_taskVec.empty())
	{
		ref_ptr<crGameServerPlayerData> senderPlayerData;
		ref_ptr<crGameServerPlayerData> recvPlayerData;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		crNetDataManager *netDataManager = gameServer->getNetDataManager();
		crNetManager *netManager = gameServer->getNetManager();
		crJXJGameChatPacket packet;
		crStreamBuf *pkgstream;
		int pkgsize;
		unsigned char done = 0;
		netDataManager->lockPlayerDataMap();
		crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
		for( std::vector< ref_ptr<crJXJChatMessageInfo> >::iterator itr = m_taskVec.begin();
			itr != m_taskVec.end();
			++itr )
		{
			chatInfo = itr->get();
			if(chatInfo->getSenderID()==0)
			{
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatAll,chatInfo->getSender(),chatInfo->getText());
			}
			else
			{
				crJXJGameChatPacket::buildReplyPacket(packet,0,ChatOneToOne,chatInfo->getSender(),chatInfo->getText());
			}
			pkgstream = packet.getStreamBuf();
			pkgsize = pkgstream->getBufSize();
			done = 0;
			for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
				itr != playerDataMap.end();
				++itr )
			{
				recvPlayerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
				if(recvPlayerData.valid() && recvPlayerData->getCharacterName() == chatInfo->getRecvName())
				{
					pdata = recvPlayerData->getPlayerGameData()->getDataClass();
					pdata -> getParam(WCHDATA_JXJBlackNameSet,param);
					BlackNameSet *blacknameset = (BlackNameSet *)param;
					BlackNameSet::iterator finditr = blacknameset->find(chatInfo->getSender());
					if (finditr == blacknameset->end())
					{
						pkgstream->seekBegin();
						pkgstream->_writeInt(recvPlayerData->getPlayerID());
						pkgstream->setBufSize(pkgsize);
						netManager->sendPacket(recvPlayerData->getPlayerConnectServerAddress(),packet);
					}

					done = 1;
					break;
				}
			}
			if(done==0)
			{
				senderPlayerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(chatInfo->getSenderID()));
				if(senderPlayerData.valid())
				{
					crPlayerDataEventPacket packet2;
					crPlayerDataEventPacket::buildReplyPacket(packet2,chatInfo->getSenderID(),WCH_JXJRecvGameChat,NULL);
					netManager->sendPacket(senderPlayerData->getPlayerConnectServerAddress(),packet2);
				}
			}
		}
		netDataManager->unLockPlayerDataMap();
	}
}
/////////////////////////////////////////
//
//crJXJVIPCanvasOnShowMethod
//
/////////////////////////////////////////
crJXJVIPCanvasOnShowMethod::crJXJVIPCanvasOnShowMethod(){}
crJXJVIPCanvasOnShowMethod::crJXJVIPCanvasOnShowMethod(const crJXJVIPCanvasOnShowMethod& handle):
	crMethod(handle),
	m_levelText(handle.m_levelText),
	m_levelSwitch(handle.m_levelSwitch),
	m_radiowidget(handle.m_radiowidget),
	m_switch(handle.m_switch),
	m_vipProgressBar(handle.m_vipProgressBar),
	m_vipProgressValue(handle.m_vipProgressValue),
	m_vipList(handle.m_vipList),
	m_btUpLevel(handle.m_btUpLevel),
	m_radioSvWidget(handle.m_radioSvWidget),
	m_NeizhengRadio(handle.m_NeizhengRadio),
	m_NeizhengScrollBar(handle.m_NeizhengScrollBar),
	m_namesw(handle.m_namesw),
	m_vipinfolist(handle.m_vipinfolist),
	m_needCharge(handle.m_needCharge),
	m_upgradeVipLvText(handle.m_upgradeVipLvText),
	m_leftbtn(handle.m_leftbtn),
	m_rightbtn(handle.m_rightbtn)
{
	for (int i = 0; i < 4; i++)
	{
		m_NeizhengIcon[i] = handle.m_NeizhengIcon[i];
		m_NeizhengProgress[i] = handle.m_NeizhengProgress[i];
		m_NeizhengAdd[i] = handle.m_NeizhengAdd[i];
		m_NeizhengCost[i] = handle.m_NeizhengCost[i];
		m_NeizhengUpgradebtn[i] = handle.m_NeizhengUpgradebtn[i];
		m_levelsw[i] = handle.m_levelsw[i];
	}
	for (int i = 0; i < 5; i++)
	{
		m_rewardicon[i] = handle.m_rewardicon[i];
		m_rewardnum[i] = handle.m_rewardnum[i];
	}
}
void crJXJVIPCanvasOnShowMethod::inputParam(int i, void *param)
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

void crJXJVIPCanvasOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_levelText = str;
		break;
	case 1:
		m_levelSwitch = str;
		break;
	case 2:
		m_radiowidget = str;
		break;
	case 3:
		m_switch = str;
		break;
	case 4:
		m_vipProgressBar = str;
		break;
	case 5:
		m_vipProgressValue = str;
		break;
	case 6:
		m_vipList = str;
		break;
	case 7:
		m_btUpLevel = str;
		break;
	case 8:
		m_radioSvWidget = str;
		break;
	case 9:
		m_NeizhengRadio = str;
		break;
	case 10:
		m_NeizhengIcon[i - 10] = str;
		break;
	case 11:
		m_NeizhengIcon[i - 10] = str;
		break;
	case 12:
		m_NeizhengIcon[i - 10] = str;
		break;
	case 13:
		m_NeizhengIcon[i - 10] = str;
		break;
	case 14:
		m_NeizhengProgress[i - 14] = str;
		break;
	case 15:
		m_NeizhengProgress[i - 14] = str;
		break;
	case 16:
		m_NeizhengProgress[i - 14] = str;
		break;
	case 17:
		m_NeizhengProgress[i - 14] = str;
		break;
	case 18:
		m_NeizhengAdd[i - 18] = str;
		break;
	case 19:
		m_NeizhengAdd[i - 18] = str;
		break;
	case 20:
		m_NeizhengAdd[i - 18] = str;
		break;
	case 21:
		m_NeizhengAdd[i - 18] = str;
		break;
	case 22:
		m_NeizhengCost[i - 22] = str;
		break;
	case 23:
		m_NeizhengCost[i - 22] = str;
		break;
	case 24:
		m_NeizhengCost[i - 22] = str;
		break;
	case 25:
		m_NeizhengCost[i - 22] = str;
		break;
	case 26:
		m_NeizhengUpgradebtn[i - 26] = str;
		break;
	case 27:
		m_NeizhengUpgradebtn[i - 26] = str;
		break;
	case 28:
		m_NeizhengUpgradebtn[i - 26] = str;
		break;
	case 29:
		m_NeizhengUpgradebtn[i - 26] = str;
		break;
	case 30:
		m_NeizhengScrollBar = str;
		break;
	case 31:
	case 32:
	case 33:
	case 34:
		m_levelsw[i - 31] = str;
		break;
	case 35:
		m_namesw = str;
		break;
	case 36:
		m_vipinfolist = str;
		break;
	case 37:
		m_needCharge = str;
		break;
	case 38:
		m_upgradeVipLvText = str;
		break;
	case 39:
		m_leftbtn = str;
		break;
	case 40:
		m_rightbtn = str;
		break;
	case 41:
		m_lingqubtn = str;
		break;
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
		m_rewardicon[i-42] = str;
		break;
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
		m_rewardnum[i-47] = str;
		break;
	default:
		break;
	}
}

void crJXJVIPCanvasOnShowMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		//ref_ptr<crStaticTextWidgetNode>  levelText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_levelText));
		//ref_ptr<crStaticTextWidgetNode>  levelStatictext= dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_levelSwitch));
		//ref_ptr<crRadioGroupWidgetNode>  radioWidget = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_radiowidget));
		//ref_ptr<crMultiSwitch>  moduleSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_switch));
		ref_ptr<crButtonWidgetNode> leftbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_leftbtn));
		ref_ptr<crButtonWidgetNode> rightbtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_rightbtn));
		ref_ptr<crHypertextWidgetNode>  upgradeVipLvText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_upgradeVipLvText));
		ref_ptr<crHypertextWidgetNode>  needChargeUpgradeText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_needCharge));
		ref_ptr<crProgressWidgetNode>  progressVipExp = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_vipProgressBar));
		ref_ptr<crStaticTextWidgetNode>  vipExpText = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_vipProgressValue));
		ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_vipList));
		//ref_ptr<crButtonWidgetNode>  btLevelUp = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_btUpLevel));
		//crRadioWidgetNode *radioSvWidget = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(m_radioSvWidget));
		ref_ptr<crTableWidgetNode> vipinfolist = dynamic_cast<crTableWidgetNode *>(m_this->getWidget(m_vipinfolist));
		ref_ptr<crMultiSwitch>  levelsw[4] = {NULL};
		for(int i = 0; i < 4; ++i)
		{
			levelsw[i] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_levelsw[i]));
		}
		ref_ptr<crImageBoxWidgetNode>  rewardicon[5];
		ref_ptr<crStaticTextWidgetNode>  rewardnum[5];
		for(int i = 0; i < 5; ++i)
		{
			rewardicon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_rewardicon[i]));
			rewardnum[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_rewardnum[i]));
			rewardicon[i]->setVisiable(false);
			rewardnum[i]->setVisiable(false);
		}
		if (vipinfolist.valid())
		{
			vipinfolist->clearData();
		}

		//ref_ptr<crImageBoxWidgetNode>  rewardimg = dynamic_cast<crImageBoxWidgetNode *>(m_this->getChildNode(m_rewardimg));
		ref_ptr<crButtonWidgetNode> lingqubtn = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_lingqubtn));

		ref_ptr<crMultiSwitch>  namesw = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_namesw));
		//if(radioSvWidget) radioSvWidget->setVisiable(false);
		std::string listnamestr = "UI_ArmyAdviser_Listctrl_";
		ref_ptr<crListBoxWidgetNode> listvec[15];
		for (int i = 0; i < 15; ++i)
		{
			listvec[i] = dynamic_cast<crListBoxWidgetNode *>(m_this->getWidget(listnamestr + crArgumentParser::appItoa(i + 2)));
			if (listvec[i].valid())
			{
				listvec[i]->setColor(FCOLOR_BLACK);
			}
		}
		ref_ptr<crTableIO> vipinfotab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipInfoTab);
		if (vipinfolist.valid())
		{
			for (int i = 1; i < vipinfotab->getColumnCount(); ++i)
			{
				vipinfolist->addData(0, i-1, vipinfotab->getTitle(i));
			}
			for (int i = 1; i < vipinfotab->getColumnCount(); ++i)
			{
				for (int j = 0; j < vipinfotab->getRowCount(); ++j)
				{
					vipinfolist->addData(j+1, i-1, (*vipinfotab)(j,i));
				}
			}
			//for (int i = 0; i < vipinfotab->getColumnCount() - 1; ++i)
			//{
			//	for (int j = 0; j <= vipinfotab->getRowCount(); ++j)
			//	{
			//		if (0 == j)
			//		{
			//			vipinfolist->addData(j, i, vipinfotab->getTitle(i + 1));
			//		}
			//		else
			//		{
			//			vipinfolist->addData(j, i, (*vipinfotab)(j - 1, i + 1));
			//		}
			//	}
			//}
		}
		
		ref_ptr<crTableIO> viptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
		int vipshowfactor = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJVipExpShowFactor,0).c_str());
		if (vipshowfactor <= 0)
		{
			vipshowfactor = 1;
		}
		if(false == viptab.valid()) return;
		int expIndex = viptab->getTitleIndex("所需成长值");
		int cfgIndex = viptab->getTitleIndex("script");
		int nameIndex = viptab->getTitleIndex("称号");
		crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerData)
		{
			crData *data  = playerData->getDataClass();
			if(data)
			{
				data->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				data->getParam(WCHDATA_JXJVipExp,param);
				int vipExp = *(int *)param;
				//levelText->setString(crArgumentParser::appItoa(vipLevel)); 
				for (int i = 0; i < vipLevel; ++i)
				{
					if (listvec[i].valid())
					{
						listvec[i]->setColor(FCOLOR_ORANGE);
					}
				}
				//moduleSwitch->setActiveSwitchSet(radioWidget->getSelect());
				crData *canvasdata = m_this->getDataClass();
				canvasdata->getParam(WCHDATA_JXJVipCanvasCurPageNum,param);
				int curpagenum = *(int *)param;
				if (levelsw[0].valid() && levelsw[1].valid() && levelsw[2].valid() && levelsw[3].valid())
				{
					if (curpagenum <10)
					{
						levelsw[0]->setActiveSwitchSet(0);
						levelsw[1]->setActiveSwitchSet(curpagenum);
					}
					else
					{
						levelsw[0]->setActiveSwitchSet(1);

						levelsw[2]->setActiveSwitchSet(curpagenum/10);
						levelsw[3]->setActiveSwitchSet(curpagenum%10);
					}
				}
				//if (rewardimg.valid())
				//{
				//	rewardimg->setImageName("UI_Icon_Baoxiang.img");
				//}
				if (lingqubtn.valid())
				{
					//if (vipLevel == 0 || curpagenum > vipLevel)
					//	lingqubtn->setEnable(false);
					//else
					//{
						ref_ptr<crTableIO> viplvawardTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJViplvRewardTab);
						int awardindex[5];
						awardindex[0] =  viplvawardTab->getTitleIndex("奖励1");
						awardindex[1] =  viplvawardTab->getTitleIndex("奖励2");
						awardindex[2] =  viplvawardTab->getTitleIndex("奖励3");
						awardindex[3] =  viplvawardTab->getTitleIndex("奖励4");
						awardindex[4] =  viplvawardTab->getTitleIndex("奖励5");
						crTableIO::StrVec awardrecord;
						crVector4i awardvec;
						data->getParam(WCHDATA_JXJPlayerHaveGotViplvReward,param);
						unsigned char havegot = *(unsigned char *)param;
						if(havegot < vipLevel)
						{
							lingqubtn->setVisiable(true);
							lingqubtn->setEnable(true);
							if(viplvawardTab->queryOneRecord(0,crArgumentParser::appItoa(havegot+1),awardrecord)>=0)
							{
								for( int i = 0; i<5; i++ )
								{
									crArgumentParser::appAtoVec(awardrecord[awardindex[i]],awardvec);
									setRewardInfo(awardvec,rewardicon[i].get(),rewardnum[i].get());
								}
							}
						}
						else
						{
							if(havegot == 15)
							{//满级
								lingqubtn->setVisiable(false);
							}
							else
							{
								if(viplvawardTab->queryOneRecord(0,crArgumentParser::appItoa(curpagenum==0?1:curpagenum),awardrecord)>=0)
								{
									for( int i = 0; i<5; i++ )
									{
										crArgumentParser::appAtoVec(awardrecord[awardindex[i]],awardvec);
										setRewardInfo(awardvec,rewardicon[i].get(),rewardnum[i].get());
									}
								}
								lingqubtn->setVisiable(true);
								lingqubtn->setEnable(false);
							}
						}
					//}

				}
				if(namesw.valid())
				{
					if (curpagenum < 3)
					{
						namesw->setActiveSwitchSet(curpagenum);
					}
					else
					{
						namesw->setActiveSwitchSet(curpagenum-1);
					}
				}
				if(vipExpText.valid()) 
				{
					if (curpagenum < 15)
					{
						vipExpText->setString(crArgumentParser::appItoa(vipExp / vipshowfactor));
					}
					else 
					{
						vipExpText->setString(L"满级");
						if(progressVipExp.valid())progressVipExp->setProgress(1.0f);
					}
				}
				if (rightbtn.valid())
				{
					if (curpagenum < 15)
					{
						rightbtn->setEnable(true);
					}
					else
					{
						rightbtn->setEnable(false);
					}
				}
				if(viptab->queryOneRecord(0,crArgumentParser::appItoa(curpagenum),record)>=0)
				{
					if(cfgIndex>=0)
					{
						if(hyperText.valid())hyperText->setHypertext(crGlobalHandle::getInstance()->getTextDir()+record[cfgIndex]);
					}
					//if(nameIndex>=0)
						//levelStatictext->setString(record[nameIndex]);
				}
				else
				{
					if(hyperText.valid())hyperText->setHypertext(crGlobalHandle::getInstance()->getTextDir()+"Vip0.cfg");
				}
				if(viptab->queryOneRecord(0,crArgumentParser::appItoa(curpagenum+1),record)>=0)
				{
					if(expIndex>=0)
					{
						if(progressVipExp.valid())progressVipExp->setProgress((float)vipExp/atof(record[expIndex].c_str()));
						if(vipExpText.valid())vipExpText->setString(crArgumentParser::appItoa(vipExp/ vipshowfactor) + string("/")+ crArgumentParser::appItoa(atoi(record[expIndex].c_str())/ vipshowfactor));
						if (curpagenum <= vipLevel)
						{
							if (leftbtn.valid())
							{
								leftbtn->setEnable(false);
							}
						}
// 						else if (curpagenum < vipLevel)
// 						{
// 							if(progressVipExp.valid())progressVipExp->setProgress(1.0f);
// 							if(vipExpText.valid())vipExpText->setString( crArgumentParser::appItoa(atoi(record[expIndex].c_str())/ vipshowfactor) + string("/")+ crArgumentParser::appItoa(atoi(record[expIndex].c_str())/ vipshowfactor));
// 						}
						else
						{
							if (leftbtn.valid())
							{
								leftbtn->setEnable(true);
							}
						}
						//if(vipExp<atoi(record[expIndex].c_str()))
							//btLevelUp->setEnable(false);
						//else
						//{
							//btLevelUp->setEnable(true);
						//}
						
						if (needChargeUpgradeText.valid())
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
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
							v_i.push_back(0.0f);
							v_i.push_back(255.0f);
							cfg_script.Add("Color",v_i);
							cfg_script.Add("Text","每充值1元宝增加1点VIP经验");
							cfg_script.Pop();

							cfg_script.Pop();
							needChargeUpgradeText->setHypertext(cfg_script);
						}
						if (upgradeVipLvText.valid())
						{
							int needChargeGold = ((atoi(record[expIndex].c_str())/ vipshowfactor) - (vipExp/ vipshowfactor));
							if (needChargeGold <= 0)
							{
								needChargeGold = 0;
							}
							if (vipLevel < 15)
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
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","还需充值");
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",2);
								v_i.clear();
								v_i.push_back(140.0f);
								v_i.push_back(40.0f);
								v_i.push_back(40.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text",crArgumentParser::appItoa(needChargeGold));
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",3);
								v_i.clear();
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","元宝达到");
								cfg_script.Pop();

								cfg_script.Add("Content");
								cfg_script.Push("Content",4);
								v_i.clear();
								v_i.push_back(140.0f);
								v_i.push_back(40.0f);
								v_i.push_back(40.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text", "VIP" + crArgumentParser::appItoa(curpagenum+1));
								cfg_script.Pop();

								cfg_script.Pop();
								upgradeVipLvText->setHypertext(cfg_script);
							}
							else
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
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(0.0f);
								v_i.push_back(255.0f);
								cfg_script.Add("Color",v_i);
								cfg_script.Add("Text","您的VIP等级已经达到最高级");
								cfg_script.Pop();

								cfg_script.Pop();
								upgradeVipLvText->setHypertext(cfg_script);
							}
						}
					}
				}
			}
			//内政
			ref_ptr<crTableIO> VipAffairsTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsTab);
			ref_ptr<crTableIO> VipAffairsLvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipAffairsLvTab);
			crTableIO::DataVec affairsrecords,affairslvrecords;
			crTableIO::StrVec  afflvrecord;
			ref_ptr<crRadioGroupWidgetNode>  NeizhengRadio = dynamic_cast<crRadioGroupWidgetNode *>(m_this->getWidget(m_NeizhengRadio));
			ref_ptr<crScrollBarWidgetNode>  NeizhengScrollBar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_NeizhengScrollBar));
			ref_ptr<crImageBoxWidgetNode>  NeizhengIcon[4] = {NULL};
			ref_ptr<crProgressWidgetNode>  NeizhengProgress[4] = {NULL};
			ref_ptr<crStaticTextWidgetNode>  NeizhengAdd[4] = {NULL};
			ref_ptr<crStaticTextWidgetNode>  NeizhengCost[4] = {NULL};
			ref_ptr<crButtonWidgetNode>  NeizhengUpgradebtn[4] = {NULL};
			crData *canvasdata = m_this -> getDataClass();
			if (NeizhengRadio.valid() && canvasdata)
			{
				canvasdata->getParam(WCHDATA_JXJVipAffairsIDVec,param);
				VIPAffairsVec *affairvec = (VIPAffairsVec *)param;
				affairvec->clear();
				if (NeizhengScrollBar.valid())
				{
					NeizhengScrollBar->setVisiable(false);
				}
				for (int i = 0; i < 4; i++)
				{
					NeizhengIcon[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_NeizhengIcon[i]));
					NeizhengProgress[i] = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_NeizhengProgress[i]));
					NeizhengAdd[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_NeizhengAdd[i]));
					NeizhengCost[i] = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_NeizhengCost[i]));
					NeizhengUpgradebtn[i] = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_NeizhengUpgradebtn[i]));
					if (NeizhengIcon[i].valid())NeizhengIcon[i]->setVisiable(false);
					if (NeizhengProgress[i].valid())NeizhengProgress[i]->setVisiable(false);
					if (NeizhengAdd[i].valid())NeizhengAdd[i]->clearString();
					if (NeizhengCost[i].valid())NeizhengCost[i]->clearString();
					if (NeizhengUpgradebtn[i].valid())NeizhengUpgradebtn[i]->setVisiable(false);
				}
				int affairid = 0;
				unsigned char affairlv = 0; 
				int affairtype = NeizhengRadio -> getSelect();
				int typeindex = VipAffairsTab -> getTitleIndex("type");
				int nameindex = VipAffairsTab -> getTitleIndex("name");
				int iconindex = VipAffairsTab -> getTitleIndex("icon");
				int lvindex = VipAffairsLvTab -> getTitleIndex("lv");
				int addindex = VipAffairsLvTab -> getTitleIndex("add");
				int costindex = VipAffairsLvTab -> getTitleIndex("消耗成长值");
				std::string str;
				VipAffairsTab->queryRecords(typeindex,crArgumentParser::appItoa(affairtype),affairsrecords);
				for (int i = 0; i < 4 && i < affairsrecords.size(); i++)
				{
					affairid = atoi(affairsrecords[i][0].c_str());
					switch (affairid)
					{
					case WCHDATA_JXJVIPNongTianShuiLi:
						str = "增加粮食产量";
						break;
					case WCHDATA_JXJVIPZhiShuZaoLin:
						str = "增加木材产量";
						break;
					case WCHDATA_JXJVIPYouZhiCaoChang:
						str = "增加马匹产量";
						break;
					case WCHDATA_JXJVIPJingKuangShenWa:
						str = "增加铁矿产量";
						break;
					case WCHDATA_JXJVIPGuLiMaoYi:
						str = "增加征收暴击率";
						break;
					case WCHDATA_JXJVIPZhengShouXinDe:
						str = "增加征收暴击效果";
						break;
					case WCHDATA_JXJVIPYanBingYueWu:
						str = "增加招募暴击率";
						break;
					case WCHDATA_JXJVIPXunBingXinDe:
						str = "增加招募暴击效果";
						break;
					case WCHDATA_JXJVIPZongMiaoJiSi:
						str = "增加祭祀暴击率";
						break;
					case WCHDATA_JXJVIPJiSiXinDe:
						str = "增加祭祀暴击效果";
						break;
					}
					affairvec->push_back(affairid);
					data->getParam(affairid,param);
					affairlv = *(unsigned char *)param;
					if (NeizhengIcon[i].valid())
					{
						NeizhengIcon[i]->setVisiable(true);
						NeizhengIcon[i]->setImageName(affairsrecords[i][iconindex]);
					}
					if (NeizhengProgress[i].valid())
					{
						affairslvrecords.clear();
						VipAffairsLvTab->queryRecords(0,crArgumentParser::appItoa(affairid),affairslvrecords);
						NeizhengProgress[i]->setVisiable(true);
						NeizhengProgress[i]->setProgress((float)affairlv/(float)(affairslvrecords.size() - 1));
					}
					if(VipAffairsLvTab->queryOneRecord(0,affairsrecords[i][0],lvindex,crArgumentParser::appItoa(affairlv),afflvrecord)>=0)
					{
						if (NeizhengAdd[i].valid())
						{
							NeizhengAdd[i]->setString(str + crArgumentParser::appItoa((float)((int)(atof(afflvrecord[addindex].c_str()) * 100.0f)) / 1000.0f) + "%");
						}
						if (NeizhengCost[i].valid())
						{
							NeizhengCost[i]->setString(afflvrecord[costindex] + "成长值");
						}
						if (NeizhengUpgradebtn[i].valid())
						{
							NeizhengUpgradebtn[i]->setVisiable(true);
						}
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJVIPCanvasSwitchChangeMethod
//
/////////////////////////////////////////
crJXJVIPCanvasSwitchChangeMethod::crJXJVIPCanvasSwitchChangeMethod(){}
crJXJVIPCanvasSwitchChangeMethod::crJXJVIPCanvasSwitchChangeMethod(const crJXJVIPCanvasSwitchChangeMethod& handle):
	crMethod(handle)
{
}
void crJXJVIPCanvasSwitchChangeMethod::inputParam(int i, void *param)
{

}

void crJXJVIPCanvasSwitchChangeMethod::addParam(int i, const std::string& str)
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
		m_value = atoi(str.c_str());
		break;
	}
}

void crJXJVIPCanvasSwitchChangeMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	if(canvas.valid())
	{
		ref_ptr<crMultiSwitch>  caseSwitch = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_switch));
		if(caseSwitch.valid())
		{
			caseSwitch->setActiveSwitchSet(m_value);
		}
	}
}

/////////////////////////////////////////
//
//crJXJVIPLevelUpdateMethod
//
/////////////////////////////////////////
crJXJVIPLevelUpdateMethod::crJXJVIPLevelUpdateMethod(){}
crJXJVIPLevelUpdateMethod::crJXJVIPLevelUpdateMethod(const crJXJVIPLevelUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJVIPLevelUpdateMethod::inputParam(int i, void *param)
{

}

void crJXJVIPLevelUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJVIPLevelUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crTableIO::StrVec record;
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerData)
	{
		crData *data  = playerData->getDataClass();
		if(data)
		{
			data->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJVipExp,param);
			int vipExp = *(int *)param;

			ref_ptr<crTableIO> viptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
			if(false == viptab.valid()) return;
			int expIndex = viptab->getTitleIndex("所需成长值");

			if(viptab->queryOneRecord(0,crArgumentParser::appItoa(vipLevel+1),record)>=0)
			{
				if(expIndex>=0)
				{
					int needvipExp = atoi(record[expIndex].c_str());
					if(vipExp >= needvipExp)
					{
						crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);

						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvVipUpdate,NULL);
						netConductor->getNetManager()->sendPacket("all",packet);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvVIPUpdateMethod
//
/////////////////////////////////////////
crJXJRecvVIPUpdateMethod::crJXJRecvVIPUpdateMethod(){}
crJXJRecvVIPUpdateMethod::crJXJRecvVIPUpdateMethod(const crJXJRecvVIPUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvVIPUpdateMethod::inputParam(int i, void *param)
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

void crJXJRecvVIPUpdateMethod::addParam(int i, const std::string& str)
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

void crJXJRecvVIPUpdateMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			crTableIO::StrVec record;
			bool success = false;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			int playerID = m_this->getPlayerID();
			if(gameServer)
			{
				unsigned char *vipLevel = 0;
				crNetDataManager *netDataManager = gameServer->getNetDataManager();
				ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
				ref_ptr<crPlayerGameData> playerGameData;
				ref_ptr<crData> playerData;
				ref_ptr<crTableIO> vipTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
				if(netDataManager && vipTab.valid())
				{
					int expIndex = vipTab->getTitleIndex("所需成长值");
					playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
					if(playerGameServerPlayerData.valid())
					{
						playerGameData = playerGameServerPlayerData->getPlayerGameData();
						if(playerGameData.valid() && playerGameData->getDataClass())
						{
							playerData = playerGameData->getDataClass();

							//playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
							playerData->getParam(WCHDATA_JXJVipLv,param);
							vipLevel = (unsigned char*)param;
							playerData->getParam(WCHDATA_JXJVipExp,param);
							int *vipExp = (int *)param;

							if(vipTab->queryOneRecord(0,crArgumentParser::appItoa(*vipLevel+1),record)>=0)
							{
								if(expIndex>=0)
								{
									int needvipExp = atoi(record[expIndex].c_str());
									if(*vipExp >= needvipExp)
									{
										//m_this->doEvent(WCH_JXJAddVipExp,MAKECREPARAM(-needvipExp,NULL));
										//*vipLevel+=1;

										VipChangeData vipchange(1,0);
										m_this->doEvent(WCH_VIPChange,MAKECREPARAM(&vipchange,&success));
										//受VIP等级影响的参数、、、
										crVector2i maxlevycount[2];
										unsigned char junlingcount[2];
										crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,*vipLevel - 1).c_str(),maxlevycount[0]);
										crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,*vipLevel).c_str(),maxlevycount[1]);
										junlingcount[0] = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxJunlingBuyCount,*vipLevel - 1).c_str()));
										junlingcount[1] = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxJunlingBuyCount,*vipLevel).c_str()));
										playerData->getParam(WCHDATA_JXJLevyCount,param);
										unsigned char *levycount = (unsigned char*)param;
										*levycount += maxlevycount[1][LevyType_Free] + maxlevycount[1][LevyType_Gold] - maxlevycount[0][LevyType_Free] - maxlevycount[0][LevyType_Gold];
										//if (*levycount < 0)
										//{
										//	*levycount = 0;
										//}
										playerData->getParam(WCHDATA_JXJBuyJunlingCount,param);
										unsigned char *buyjulingcount = (unsigned char*)param;
										*buyjulingcount += junlingcount[1] - junlingcount[0];
										/////////////////////////////////////
										stream->createBuf(3);
										stream->_writeUChar(*vipLevel);
										stream->_writeUChar(*levycount);
										stream->_writeUChar(*buyjulingcount);
										crPlayerDataEventPacket packet;
										crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvVipUpdate,stream.get());
										gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
									}
								}
							}
							//playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
						}
					}
				}
				///游戏日志
				std::string logdata = "升级军师等级，当前等级：" + crArgumentParser::appItoa(*vipLevel);
				GameLogData gamelog(Log_UpgradeVIPLevel,logdata);
				crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerID,&gamelog));
			}
		}	
		else if(m_netType = GameClient_Game)
		{
			unsigned char vipLevel = m_stream->_readUChar();
			unsigned char levycount = m_stream->_readUChar();
			unsigned char junlingcount = m_stream->_readUChar();

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(playerData)
			{
				crData *data  = playerData->getDataClass();
				if(data)
				{
					data->inputParam(WCHDATA_JXJVipLv,&vipLevel);
					data->inputParam(WCHDATA_JXJLevyCount,&levycount);
					data->inputParam(WCHDATA_JXJBuyJunlingCount,&junlingcount);
					//////////////////////////////////////////////////////////////////////////军师界面刷新
					ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if(canvas.valid())
						canvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJAddVipExpMethod
//
/////////////////////////////////////////
crJXJAddVipExpMethod::crJXJAddVipExpMethod(){}
crJXJAddVipExpMethod::crJXJAddVipExpMethod(const crJXJAddVipExpMethod& handle):
	crMethod(handle)
{
}
void crJXJAddVipExpMethod::inputParam(int i, void *param)
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
			m_expValue = LOCREPARAM(param64);
		}
		else
		{
			m_expValue = NULL;
		}
		break;
	}
}

void crJXJAddVipExpMethod::addParam(int i, const std::string& str)
{
}

void crJXJAddVipExpMethod::operator()(crHandle &handle)
{
	if(m_this )
	{
		void *param;
		char addviplv = 0;
		bool success = false;
		crTableIO::StrVec record;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer);
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		int playerID = m_this->getPlayerID();
		if(gameServer)
		{
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
			ref_ptr<crPlayerGameData> playerGameData;
			ref_ptr<crData> playerData;
			if(netDataManager)
			{
				if(m_this && m_this->getDataClass())
				{
					playerData = m_this->getDataClass();
					//playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
					VipChangeData vipchange(0,m_expValue);
					m_this->doEvent(WCH_VIPChange,MAKECREPARAM(&vipchange,&success));
					//playerData->getParam(WCHDATA_JXJVipLv,param);
					//unsigned char *vipLevel = (unsigned char*)param;
					playerData->getParam(WCHDATA_JXJVipExp,param);
					int vipExp = *(int *)param;
					//*vipExp+=m_expValue;
					//if(*vipExp < 0)
					//	*vipExp = 0;
					///////////////
					//自动升级VIP//
					///////////////
					playerData->getParam(WCHDATA_JXJVipLv,param);
					unsigned char viplv = *(unsigned char *)param;
					playerData->getParam(WCHDATA_JXJLevyCount,param);
					unsigned char levycount = *(unsigned char*)param;
					playerData->getParam(WCHDATA_JXJBuyJunlingCount,param);
					unsigned char buyjulingcount = *(unsigned char*)param;
					if (m_expValue > 0)
					{
						//int needdeductexp = 0;
						int lastviplv = viplv;
						int needexp = 0;
						crTableIO::StrVec record;
						ref_ptr<crTableIO> viplvtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJVipLevelTab);
						int needexpindex = viplvtab->getTitleIndex("所需成长值");
						while (vipExp >= 0)
						{
							viplv += 1;
							if (needexpindex > 0 && viplvtab->queryOneRecord(0,crArgumentParser::appItoa(viplv),record) >= 0)
							{
								needexp = atoi(record[needexpindex].c_str());
								if (vipExp >= needexp)
								{
									addviplv += 1;
									//needdeductexp += needexp;
									//vipExp -= needexp;
								}
								else
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
						if (addviplv > 0)
						{
							success = false;
							VipChangeData vipchange(addviplv,0/*-needdeductexp*/);
							m_this->doEvent(WCH_VIPChange,MAKECREPARAM(&vipchange,&success));
							if (success)
							{
								// 受VIP影响的参数
								crVector2i maxlevycount[2];
								unsigned char junlingcount[2];
								viplv = lastviplv+addviplv;
								crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,lastviplv).c_str(),maxlevycount[0]);
								crArgumentParser::appAtoVec(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxLevyCount,viplv).c_str(),maxlevycount[1]);
								junlingcount[0] = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxJunlingBuyCount,lastviplv).c_str()));
								junlingcount[1] = (unsigned char)(atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJMaxJunlingBuyCount,viplv).c_str()));
								levycount += maxlevycount[1][LevyType_Free] + maxlevycount[1][LevyType_Gold] - maxlevycount[0][LevyType_Free] - maxlevycount[0][LevyType_Gold];
								//if (levycount < 0)
								//{
								//	levycount = 0;
								//}
								buyjulingcount += junlingcount[1] - junlingcount[0];
								if (buyjulingcount < 0)
								{
									buyjulingcount = 0;
								}
								////////////////////
							}
						}
					}

					playerData->inputParam(WCHDATA_JXJLevyCount,&levycount);
					playerData->inputParam(WCHDATA_JXJBuyJunlingCount,&buyjulingcount);
					stream->createBuf(7);
					stream->_writeInt(vipExp);

					playerData->getParam(WCHDATA_JXJVipLv,param);
					viplv = *(unsigned char *)param;
					stream->_writeUChar(viplv);
					// 受VIP影响的参数
					stream->_writeUChar(levycount);
					stream->_writeUChar(buyjulingcount);

					playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
					if(playerGameServerPlayerData.valid())
					{
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvVipExpUpdate,stream.get());
						gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
					}
					//playerData->excHandle(MAKECREPARAM(WCH_LockData, 0));
					if (addviplv > 0 && success)
					{
						m_this->doEvent(WCH_JXJRecvHuoyuedu, MAKECREPARAM(HYD_Vip, addviplv));
					}
				}
			}
		}
	}	
}

/////////////////////////////////////////
//
//crJXJRecvVIPExpUpdateMethod
//
/////////////////////////////////////////
crJXJRecvVIPExpUpdateMethod::crJXJRecvVIPExpUpdateMethod(){}
crJXJRecvVIPExpUpdateMethod::crJXJRecvVIPExpUpdateMethod(const crJXJRecvVIPExpUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvVIPExpUpdateMethod::inputParam(int i, void *param)
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

void crJXJRecvVIPExpUpdateMethod::addParam(int i, const std::string& str)
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

void crJXJRecvVIPExpUpdateMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		//void *param;
		if(m_netType = GameClient_Game)
		{
			int vipExp = m_stream->_readInt();
			unsigned char viplv = m_stream->_readUChar();
			unsigned char levycount = m_stream->_readUChar();
			unsigned char buyjulingcount = m_stream->_readUChar();

			crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
			if(playerData)
			{
				crData *data  = playerData->getDataClass();
				if(data)
				{
					//data->excHandle(MAKECREPARAM(WCH_LockData,1));
					data->inputParam(WCHDATA_JXJVipExp,&vipExp);
					//int *Exp = (int *)param;
					//*Exp = vipExp;
					data->inputParam(WCHDATA_JXJVipLv,&viplv);
					//////////////////////////////////////////////////////////////////////////军师界面刷新
					ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
					if(canvas.valid())
						canvas->doEvent(WCH_UI_UpdateData);

					//data->excHandle(MAKECREPARAM(WCH_LockData,0));
					// 受VIP影响的参数
					data->inputParam(WCHDATA_JXJLevyCount,&levycount);
					data->inputParam(WCHDATA_JXJBuyJunlingCount,&buyjulingcount);
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJItemTypeCheckMethod
//
/////////////////////////////////////////
crJXJItemTypeCheckMethod::crJXJItemTypeCheckMethod(){}
crJXJItemTypeCheckMethod::crJXJItemTypeCheckMethod(const crJXJItemTypeCheckMethod& handle):
	crMethod(handle)
{
}
void crJXJItemTypeCheckMethod::inputParam(int i, void *param)
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
		m_this = (crBrain*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_itemID = LOCREPARAM(param64);
			m_type = (int *)HICREPARAM(param64);
		}
		else
		{
			m_type = NULL;
		}
		break;
	}
}

void crJXJItemTypeCheckMethod::addParam(int i, const std::string& str)
{
}

void crJXJItemTypeCheckMethod::operator()(crHandle &handle)
{
	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(m_this && itemtab.valid())
	{
		int itemtypeindex= itemtab->getTitleIndex("类型");
		crTableIO::StrVec itemrecord;
		if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(m_itemID),itemrecord)>=0)
		{
			if(itemtypeindex>=0)
				*m_type = atoi(itemrecord[itemtypeindex].c_str());
		}
	}
}

/////////////////////////////////////////
//
//crJXJFaceIconCanvasInitMethod
//
/////////////////////////////////////////
crJXJFaceIconCanvasInitMethod::crJXJFaceIconCanvasInitMethod(){}
crJXJFaceIconCanvasInitMethod::crJXJFaceIconCanvasInitMethod(const crJXJFaceIconCanvasInitMethod& handle):
	crMethod(handle),
	m_chatCanvas(handle.m_chatCanvas),
	m_faceIconBtn(handle.m_faceIconBtn),
	m_hyperText(handle.m_hyperText)
{
}
void crJXJFaceIconCanvasInitMethod::inputParam(int i, void *param)
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

void crJXJFaceIconCanvasInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_chatCanvas = str;
		break;
	case 1:
		m_faceIconBtn = str;
		break;
	case 2:
		m_hyperText = str;
		break;
	default:
		break;
	}
}

void crJXJFaceIconCanvasInitMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crHypertextWidgetNode>  hyperText = dynamic_cast<crHypertextWidgetNode *>(m_this->getWidget(m_hyperText));
		m_this->setCanCaptureMouse(true);
		ref_ptr<crCanvasNode>  chatCanvas = crFilterRenderManager::getInstance()->findCanvas(m_chatCanvas);
		if(chatCanvas.valid() && hyperText.valid())
		{
			ref_ptr<crButtonWidgetNode>  faceIconBt = dynamic_cast<crButtonWidgetNode *>(chatCanvas->getWidget(m_faceIconBtn));
			if(faceIconBt.valid())
			{
				const crBoundingBox & bbox = faceIconBt->getBoundBox();
				const crBoundingBox & thisBox = m_this->getBoundBox();
				crVector3 center = bbox.center();

				m_this->setMatrix(crMatrix::translate(center.x()+thisBox.xLength()*0.5f,center.y()+thisBox.yLength()*0.5f,0.0f));
			}

			//m_this
			//////////////////////////////////////////////////////////////////////////显示表情
// 			ItemMap itemMap;
// 			rcfg::ConfigScript cfg_script(&itemMap);
// 			cfg_script.Add("Hypertext");
// 			cfg_script.Push("Hypertext");
// 
// 			cfg_script.Add("Content");
// 			cfg_script.Push("Content",1);
// 			cfg_script.Add("LineSpacing",5.0f);
// 			cfg_script.Add("UnderLineSpacing",5.0f);
// 			cfg_script.Add("DrawMode", "IMAGE");
// 			cfg_script.Add("ImageWidth", 16.0f);
// 			cfg_script.Add("ImageHeight", 16.0f);
// 			cfg_script.Add("ImageFile","T_BiaoQing_1.img");
// 			cfg_script.Pop();
// 
// 			cfg_script.Add("Content");								
// 			cfg_script.Push("Content",2);																				
// 			cfg_script.Add("Text",string("test"));
// 			cfg_script.Pop();
// 
// 
// 			cfg_script.Add("Content");
// 			cfg_script.Push("Content",3);
// 			cfg_script.Add("LineSpacing",16.0f);
// 			cfg_script.Add("UnderLineSpacing",16.0f);
// 			cfg_script.Add("DrawMode", "IMAGE");
// 			cfg_script.Add("ImageWidth", 16.0f);
// 			cfg_script.Add("ImageHeight", 16.0f);
// 			cfg_script.Add("ImageFile","T_Baierbing.img");
// 			cfg_script.Pop();
// 
// 			cfg_script.Pop();
// 
// 			hyperText->setHypertext(cfg_script);

		}
	}
}

/////////////////////////////////////////
//
//crJXJChangeBirthPointButtonStateMethod
//
/////////////////////////////////////////
crJXJChangeBirthPointButtonStateMethod::crJXJChangeBirthPointButtonStateMethod():m_index(-1){}
crJXJChangeBirthPointButtonStateMethod::crJXJChangeBirthPointButtonStateMethod(const crJXJChangeBirthPointButtonStateMethod& handle):
	crMethod(handle),
	m_index(handle.m_index)
{
}
void crJXJChangeBirthPointButtonStateMethod::inputParam(int i, void *param)
{

}

void crJXJChangeBirthPointButtonStateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_index = atoi(str.c_str());
		break;
	}
}

void crJXJChangeBirthPointButtonStateMethod::operator()(crHandle &handle)
{
	/*crRoom::getMember();crRoomPlayer::getPlayerGameData()->getMainRole*/
	void *param;
	ref_ptr<crCamera> camera = CRPhysics::crCameraManager::getInstance()->getMainCamera();
	if(camera.valid())
	{
		crData *cameraData = camera->getAttachedNode()->getDataClass();
		if(cameraData)
		{
			CRCore::ScopedLock<crData> lock(*cameraData);
			cameraData->getParam(WCHDATA_JXJBirthPointButtonVec,param);
			ButtonVector *btVector = (ButtonVector *)param;
			if(m_index<btVector->size())
			{
				ref_ptr<crMultiSwitch>  btSwitch = NULL;
				for (int i = 0;i<btVector->size();i++)
				{
					btSwitch = dynamic_cast<crMultiSwitch *>((*btVector)[i]->getChild(0));
					if(btSwitch.valid())
					{
						if(m_index == i)
							btSwitch->setActiveSwitchSet(1);
						else
							btSwitch->setActiveSwitchSet(0);
					}

				}

			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJTouchRoleShowMethod
//
/////////////////////////////////////////
crJXJTouchRoleShowMethod::crJXJTouchRoleShowMethod(){}
crJXJTouchRoleShowMethod::crJXJTouchRoleShowMethod(const crJXJTouchRoleShowMethod& handle):
	crMethod(handle),
	m_canvas(handle.m_canvas)
{
}
void crJXJTouchRoleShowMethod::inputParam(int i, void *param)
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
	//case 2:
	//	if(param)
	//	{
	//		CREPARAM& param64 = *(CREPARAM*)param;
	//		m_time = (double *)LOINT64(param64);
	//		//m_type = (int *)HIINT64(param64);
	//	}
	//	else
	//	{
	//		m_time = NULL;
	//	}
	//	break;
	}
}

void crJXJTouchRoleShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_strbt_a = str;
		break;
	case 2:
		m_strbt_m = str;
		break;
	case 3:
		m_strSwitch = str;
		break;
	case 4:
		m_strDown = str;
		break;
	case 5:
		m_strUp = str;
		break;
	case 6:
		m_strEquips = str;
		break;
	case 7:
		m_strName = str;
		break;
	case 8:
		m_strGrade = str;
		break;
	case 9:
		m_strPower = str;
		break;
	case 10:
		m_strTouxiang = str;
		break;
	case 11:
		m_strSwPlane = str;
		break;
	}
}

void crJXJTouchRoleShowMethod::operator()(crHandle &handle)
{
	if(crRunGameHandle::getInstance()->isInGame())
	{
		//bool showCanvas = false;
		int playerid = m_this->getID();
		void *param;
		ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
		if(canvas.valid())
		{
			ref_ptr<crButtonWidgetNode> bt_a = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strbt_a));
			ref_ptr<crButtonWidgetNode> bt_m = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strbt_m));
			ref_ptr<crMultiSwitch> switchA = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strSwitch));
			ref_ptr<crButtonWidgetNode> btUp = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strUp));
			ref_ptr<crButtonWidgetNode> btDown = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strDown));
			ref_ptr<crMultiSwitch> btsw = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strEquips));
			ref_ptr<crStaticTextWidgetNode> name = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_strName));
			ref_ptr<crStaticTextWidgetNode> grade = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_strGrade));
			ref_ptr<crStaticTextWidgetNode> power = dynamic_cast<crStaticTextWidgetNode*>(canvas->getWidget(m_strPower));
			ref_ptr<crImageBoxWidgetNode> img = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_strTouxiang));
			ref_ptr<crMultiSwitch> switchplane = dynamic_cast<crMultiSwitch *>(canvas->getChildNode(m_strSwPlane));

			crData *canvasData = canvas->getDataClass();
			if(canvasData)
			{
				if(m_this->getItemtype() == crInstanceItem::Role)
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
					RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
					canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
					crInstanceItem* curSelect = (crInstanceItem *)param;
					RoomMemberInfoMap::iterator itr_info = infoMap->find(playerid);

					if(itr_info != infoMap->end() && itr_info->second->getValid())
					{
						if(!curSelect || (curSelect && curSelect->getID() != playerid))
						{
							canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect,m_this);
						}

						//showCanvas = true;
					}
					else
					{
						//判断是不是自己人
						ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
						if(camera.valid())
						{
							//crData *cameraData = camera->getAttachedNode()->getDataClass();
							//CRCore::ScopedLock<crData> lock(*cameraData);
							//cameraData->getParam(WCHDATA_SelectMyRoleVec,param);
							//SelectMyRoleVec *selectMyRoles = (SelectMyRoleVec *)param;
							//if (!selectMyRoles->empty())
							if(crMyPlayerData::getInstance()->ifItemIsMe(m_this))
							{
								crPlayerGameData *playerGameData =  crMyPlayerData::getInstance()->getPlayerGameData();
								if(playerGameData && playerGameData->getMainRole())
								{
									ref_ptr<crJXJRoomMemberInfo> roomMemberInfo = new crJXJRoomMemberInfo;
									roomMemberInfo->setValid(true);
									crData *playerData = playerGameData->getDataClass();
									crData *mainRoleData = playerGameData->getMainRole()->getDataClass();
									if(playerData && mainRoleData)
									{
										playerData->getParam(WCHDATA_PlayerIconID,param);
										unsigned char playerIconID = *(unsigned char*)param;
										roomMemberInfo->setPlayerIconID(playerIconID);

										mainRoleData->getParam(WCHDATA_Level,param);
										unsigned char playerLv = *(unsigned char *)param;
										roomMemberInfo->setPlayerLv(playerLv);

										float playerBF =0;
										playerGameData->getMainRole()->doEvent(WCH_JXJGetBingFa,MAKECREPARAM(&playerBF,NULL));
										roomMemberInfo->setBingFa((unsigned short)playerBF);

										float playerZF = 0;
										playerGameData->getMainRole()->doEvent(WCH_JXJGetZhenFa,MAKECREPARAM(&playerZF,NULL));
										roomMemberInfo->setZhenFa((unsigned short)playerZF);

										float playerDC = 0;
										playerGameData->getMainRole()->doEvent(WCH_JXJGetDongCha,MAKECREPARAM(&playerDC,NULL));
										roomMemberInfo->setDongCha((unsigned short)playerDC);

										mainRoleData->getParam(WCHDATA_JXJLordEquipVec,param);
										JXJLordEquipVec *equipvec = (JXJLordEquipVec *)param;
										roomMemberInfo->getEquipVec() = *equipvec;

										mainRoleData->getParam(WCHDATA_JXJGovPost,param);//官职
										unsigned char govpost = *(unsigned char *)param;
										roomMemberInfo->setPlayerGovPos(govpost);

										mainRoleData->getParam(WCHDATA_JXJExploit,param);//功勋
										int exploit = *(int *)param;
										roomMemberInfo->setPlayerExploit(exploit);

										float tongshuai = 0;
										m_this->doEvent(WCH_JXJGetTongShuai,MAKECREPARAM(&tongshuai,NULL));
										roomMemberInfo->setPlayerLeaderPower((short)tongshuai);

										roomMemberInfo->setCharacterName(playerGameData->getCharacterName());

										//int output = 0;
										//playerGameData->doEvent(WCH_JXJGetPlayerFightingPower,MAKECREPARAM(&output,NULL));
										//roomMemberInfo->setFightPower(output);

										infoMap->insert(make_pair(playerid,roomMemberInfo.get()));
										canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect,m_this);
										//showCanvas = true;
									}
								}
							}
							else
							{
								canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect,m_this);

								//发送查询包
								crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
								ref_ptr<crStreamBuf> stream = new crStreamBuf;
								stream->createBuf(4);
								stream->_writeInt(playerid);
								crPlayerDataSceneEventPacket packet;
								crPlayerDataSceneEventPacket::buildRequestPacket(packet,WCH_JXJRecvGetRoomMemberInfo,stream.get());
								netConductor->getNetManager()->sendPacket("all",packet);

								// 							ref_ptr<crStreamBuf> scene_stream = new crStreamBuf;
								// 							scene_stream->createBuf(4);
								// 							scene_stream->_writeInt(playerid);
								// 							crPlayerDataSceneEventPacket scene_packet;
								// 							crPlayerDataSceneEventPacket::buildRequestPacket(scene_packet,WCH_JXJRecvPlayerFightPower,scene_stream.get());
								// 							netConductor->getNetManager()->sendPacket("all",scene_packet);

							}
						}
					}
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));

					if (bt_a.valid())
						bt_a->setVisiable(true);
					if (bt_m.valid())
						bt_m->setVisiable(false);
					if (switchA.valid())
						switchA->setActiveSwitchSet(0);

					if(btUp.valid())
						btUp->setVisiable(false);
					if(btDown.valid())
						btDown->setVisiable(true);
					if(btsw.valid())
						btsw->setActiveSwitchSet(0);

					if(img.valid())
						img->setVisiable(true);

					if (switchplane.valid())
						switchplane->setActiveSwitchSet(0);
				}
				else
				{
					if (bt_a.valid())
						bt_a->setVisiable(false);
					if (bt_m.valid())
						bt_m->setVisiable(false);
					if (switchA.valid())
						switchA->setActiveSwitchSet(0);

					if(btUp.valid())
						btUp->setVisiable(false);
					if(btDown.valid())
						btDown->setVisiable(false);
					if(btsw.valid())
						btsw->setActiveSwitchSet(0);

					if(name.valid())
						name->clearString();
					if(grade.valid())
						grade->clearString();
					if(power.valid())
						power->clearString();
					if(img.valid())
						img->setVisiable(false);

					if (switchplane.valid())
						switchplane->setActiveSwitchSet(1);
				}
			}

			canvasData->inputParam(WCHDATA_JXJRoomMemberCurSelect,m_this);
			
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvGetRoomMemberInfoMethod
//
/////////////////////////////////////////
crJXJRecvGetRoomMemberInfoMethod::crJXJRecvGetRoomMemberInfoMethod(){}
crJXJRecvGetRoomMemberInfoMethod::crJXJRecvGetRoomMemberInfoMethod(const crJXJRecvGetRoomMemberInfoMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvGetRoomMemberInfoMethod::inputParam(int i, void *param)
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

void crJXJRecvGetRoomMemberInfoMethod::addParam(int i, const std::string& str)
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

void crJXJRecvGetRoomMemberInfoMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == SceneServer)
		{
			crTableIO::StrVec record;
			//float output = 0.0f,fightPower = 0.0f;
			map< unsigned char,float > factorMap;
			crVector2i equips = crVector2i(-1,-1);
			crTableIO::StrVec itemrecord;
			crTableIO::StrVec factorrecord;
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			int playerID = m_this->getPlayerID();
			int roomPlayerID = m_stream->_readInt();
			if(gameServer)
			{
				crNetDataManager *netDataManager = gameServer->getNetDataManager();
				ref_ptr<crSceneServerPlayerData> playerGameServerPlayerData;
				ref_ptr<crSceneServerPlayerData> roomPlayerGameServerPlayerData;
				ref_ptr<crPlayerGameData> playerGameData;
				ref_ptr<crData> playerData;
				ref_ptr<crData> playerMainRoleData;
				if(netDataManager)
				{
					playerGameServerPlayerData = dynamic_cast<crSceneServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
					roomPlayerGameServerPlayerData = dynamic_cast<crSceneServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(roomPlayerID));
					if(playerGameServerPlayerData.valid() && roomPlayerGameServerPlayerData.valid() && roomPlayerGameServerPlayerData->getRoomID()!=0)
					{
						playerGameData = roomPlayerGameServerPlayerData->getPlayerGameData();
						if(playerGameData.valid() && playerGameData->getDataClass() && playerGameData->getMainRole() && playerGameData->getMainRole()->getDataClass())
						{
							playerData = playerGameData->getDataClass();
							playerMainRoleData = playerGameData->getMainRole()->getDataClass();

							//playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
							//playerMainRoleData->excHandle(MAKECREPARAM(WCH_LockData,1));

							playerData->getParam(WCHDATA_PlayerIconID,param);
							unsigned char playerIconID = *(unsigned char*)param;

							playerData->getParam(WCHDATA_JXJShiliID,param);
							unsigned char shiliid = *(unsigned char *)param;

							playerMainRoleData->getParam(WCHDATA_Level,param);
							unsigned char playerLv = *(unsigned char *)param;

// 							playerMainRoleData->getParam(WCHDATA_JXJBingFa,param);
// 							unsigned short playerBF = *(unsigned short*)param;
							float playerBF =0.0f;
							playerGameData->getMainRole()->doEvent(WCH_JXJGetBingFa,MAKECREPARAM(&playerBF,NULL));

							float playerZF =0.0f;
							playerGameData->getMainRole()->doEvent(WCH_JXJGetZhenFa,MAKECREPARAM(&playerZF,NULL));

							float playerDC =0.0f;
							playerGameData->getMainRole()->doEvent(WCH_JXJGetDongCha,MAKECREPARAM(&playerDC,NULL));

							playerMainRoleData->getParam(WCHDATA_JXJLordEquipVec,param);
							JXJLordEquipVec equipvec = *(JXJLordEquipVec *)param;

							playerMainRoleData->getParam(WCHDATA_JXJGovPost,param);//官职
							unsigned char govpost = *(unsigned char *)param;

							playerMainRoleData->getParam(WCHDATA_JXJExploit,param);//功勋
							int exploit = *(int *)param;

							float tongshuai = 0.0f;
							playerGameData->getMainRole()->doEvent(WCH_JXJGetTongShuai,MAKECREPARAM(&tongshuai,NULL));

							playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
							FormationInfoVec myteam = *(FormationInfoVec *)param;

							//ref_ptr<crTableIO> factorTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFightingPowerFactorTab);
							//ref_ptr<crTableIO> itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
							//if(factorTab)
							//{
							//	int index = factorTab->getTitleIndex("属性");
							//	int factorIndex = factorTab->getTitleIndex("系数");
							//	for (unsigned char i = 0;i<FP_SecondaryCard+1;i++)
							//	{
							//		if(factorTab->queryOneRecord(0,crArgumentParser::appItoa(i),factorrecord) && factorIndex>=0)
							//		{
							//			factorMap[i] = atof(factorrecord[factorIndex].c_str());
							//		}
							//	}
							//}

							//if(itemTab)
							//{
								//int itemQulityIndex = itemTab->getTitleIndex("品质");
								//crSceneServerPlayerData::RoleMap &playerRoleMap = roomPlayerGameServerPlayerData->getRoleMap();
								//for(crSceneServerPlayerData::RoleMap::iterator itr_role = playerRoleMap.begin();itr_role!=playerRoleMap.end();itr_role++)
								//{
								//	for(FormationInfoVec::iterator itr = myteam.begin();itr!=myteam.end();itr++)
								//	{
								//		if(itr->get()->getRoleID()==itr_role->first)
								//		{
								//			itr_role->second->doEvent(WCH_JXJGetRoleFightingPower,MAKECREPARAM(&output,itr->get()));
								//			fightPower+=output;
								//			//fightPower+=output * itr->get()->getCount();
								//			//equips = itr->get()->getEquips();
								//			//if(equips[0]>0)
								//			//{
								//			//	if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord))
								//			//	{
								//			//		fightPower += atoi(itemrecord[itemQulityIndex].c_str()) * factorMap[FP_MainCard];
								//			//	}
								//			//}
								//			//if(equips[1]>0)
								//			//{
								//			//	if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(equips[1]),itemrecord))
								//			//	{
								//			//		fightPower += atoi(itemrecord[itemQulityIndex].c_str()) * factorMap[FP_SecondaryCard];
								//			//	}
								//			//}
								//			break;
								//		}
								//	}
								//}
							//}

							//int totalFightPower = (int)fightPower;

							//playerMainRoleData->excHandle(MAKECREPARAM(WCH_LockData,0));
							//playerData->excHandle(MAKECREPARAM(WCH_LockData,0));							

							string characterName = playerGameData->getCharacterName();
							stream->createBuf(25+characterName.size()+equipvec.size()*8);
							stream->_writeUChar(playerIconID);		//1
							stream->_writeUChar(playerLv);			//1
							stream->_writeUShort((unsigned short)playerBF);			//2
							stream->_writeUShort((unsigned short)playerZF);			//2
							stream->_writeUShort((unsigned short)playerDC);			//2
							stream->_writeString(characterName);	//4+
							stream->_writeUChar(shiliid);			//1

							stream->_writeUChar(govpost);			//1
							stream->_writeInt(exploit);				//4
							stream->_writeShort((short)tongshuai);			//2

							stream->_writeUChar(equipvec.size());	//1
							for(JXJLordEquipVec::iterator itr = equipvec.begin();itr!=equipvec.end();itr++)
							{
								stream->_writeInt(itr->first);		//4
								stream->_writeInt(itr->second);		//4
							}
							//stream->_writeInt(totalFightPower);//4
							

							crPlayerDataSceneEventPacket packet;
							crPlayerDataSceneEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvGetRoomMemberInfo,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
						}
					}
				}
			}
		}	
		else if(m_netType = GameClient_Game)
		{
			bool isShowCanvas = false;
			time_t t = time(0);
			ref_ptr<crJXJRoomMemberInfo> roomMemberInfo = new crJXJRoomMemberInfo;
			roomMemberInfo->setPlayerIconID(m_stream->_readUChar());
			roomMemberInfo->setPlayerLv(m_stream->_readUChar());
			roomMemberInfo->setBingFa(m_stream->_readUShort());
			roomMemberInfo->setZhenFa(m_stream->_readUShort());
			roomMemberInfo->setDongCha(m_stream->_readUShort());
			roomMemberInfo->setCharacterName(m_stream->_readString());
			roomMemberInfo->setPlayerShiliID(m_stream->_readUChar());
			roomMemberInfo->setPlayerGovPos(m_stream->_readUChar());
			roomMemberInfo->setPlayerExploit(m_stream->_readInt());
			roomMemberInfo->setPlayerLeaderPower(m_stream->_readShort());

			unsigned char size = m_stream->_readUChar();
			int equipid = 0;
			int equipMagic = 0;
			JXJLordEquipVec &equipVec = roomMemberInfo->getEquipVec();
			equipVec.resize(size);
			for (int i = 0;i<size;i++)
			{
				equipid = m_stream->_readInt();
				equipMagic = m_stream->_readInt();
				equipVec[i] = make_pair(equipid,equipMagic);
			}

			//roomMemberInfo->setFightPower(m_stream->_readInt());
			roomMemberInfo->setValid(true);

			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crData *canvasData = canvas->getDataClass();
				if(canvasData)
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
					RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
					canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
					crInstanceItem* curSelect = (crInstanceItem *)param;

					if(curSelect)
					{
						infoMap->insert(make_pair(curSelect->getID(),roomMemberInfo.get()));
						//(*infoMap)[curSelect->getID()].get()->setFightPower(roomMemberInfo->getFightPower());

						//(*infoMap)[curSelect->getID()].get()setFightPower(roomMemberInfo->getFightPower());

						isShowCanvas = true;
					}
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
					if(isShowCanvas)
					{
						canvas->doEvent(WCH_UI_UpdateData);
						if(canvas->getVisiable()==false)
							crFilterRenderManager::getInstance()->showCanvas(canvas.get(),true);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRecvPlayerFightPowerMethod
//
/////////////////////////////////////////
crJXJRecvPlayerFightPowerMethod::crJXJRecvPlayerFightPowerMethod():
	m_netType(GameClient_Game){}
crJXJRecvPlayerFightPowerMethod::crJXJRecvPlayerFightPowerMethod(const crJXJRecvPlayerFightPowerMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvPlayerFightPowerMethod::inputParam(int i, void *param)
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

void crJXJRecvPlayerFightPowerMethod::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_canvas = str;
		break;
	}
}

void crJXJRecvPlayerFightPowerMethod::operator()(crHandle &handle)
{
	if(m_stream.valid())
	{
		void *param;
		if(m_netType == SceneServer)
		{
			int playerid = m_stream->_readInt();
			float output = 0.0f,fightPower = 0.0f;
			map< unsigned char,float > factorMap;
			crVector2i equips = crVector2i(-1,-1);
			crTableIO::StrVec itemrecord;
			crTableIO::StrVec factorrecord;
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer); 
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(playerid));
			ref_ptr<crSceneServerPlayerData> myplayerData = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
			if(playerData.valid() && playerData->getPlayerGameData() && playerData->getRoomID()!=0)
			{
				crData *playerGameData =  playerData->getPlayerGameData()->getDataClass();
				if(playerGameData)
				{
					playerGameData->excHandle(MAKECREPARAM(WCH_LockData,1));
					playerGameData->getParam(WCHDATA_JXJFormationInfoVec,param);
					FormationInfoVec myteam = *(FormationInfoVec *)param;
					playerGameData->excHandle(MAKECREPARAM(WCH_LockData,0));

					crSceneServerPlayerData::RoleMap &playerRoleMap = playerData->getRoleMap();
					for (crSceneServerPlayerData::RoleMap::iterator itr_role = playerRoleMap.begin(); itr_role != playerRoleMap.end(); itr_role++)
					{
						for (FormationInfoVec::iterator itr = myteam.begin(); itr != myteam.end(); itr++)
						{
							if (itr->get()->getRoleID() == itr_role->first)
							{
								itr_role->second->doEvent(WCH_JXJGetRoleFightingPower, MAKECREPARAM(&output, itr->get()));
								fightPower += output;
								break;
							}
						}
					}

					int totalFightPower = floor(fightPower);
					ref_ptr<crStreamBuf> scene_stream = new crStreamBuf;
					scene_stream->createBuf(4);
					scene_stream->_writeInt(totalFightPower);
					crPlayerDataSceneEventPacket packet2;
					crPlayerDataSceneEventPacket::buildReplyPacket(packet2,m_this->getPlayerID(),WCH_JXJRecvPlayerFightPower,scene_stream.get());
					if(myplayerData.valid())sceneServer->getNetManager()->sendPacket(myplayerData->getPlayerConnectServerAddress(),packet2);
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			int fightPower = m_stream->_readInt();
			ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			if(canvas.valid())
			{
				crData *canvasData = canvas->getDataClass();
				if(canvasData)
				{
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
					canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
					RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
					canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
					crInstanceItem* curSelect = (crInstanceItem *)param;

					if(curSelect)
					{
						RoomMemberInfoMap::iterator itr_info = infoMap->find(curSelect->getID());
						if(itr_info!=infoMap->end() && itr_info->second.get()->getValid())
						{
							//(*infoMap)[curSelect->getID()].get()->setFightPower(fightPower);
						}
						else
						{
							ref_ptr<crJXJRoomMemberInfo> jxjRoomMemberInfo = new crJXJRoomMemberInfo;
							//jxjRoomMemberInfo->setFightPower(fightPower);
							jxjRoomMemberInfo->setValid(true);
							infoMap->insert(make_pair(curSelect->getID(),jxjRoomMemberInfo));
						}
					}
										
					canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
				canvas->doEvent(WCH_UI_UpdateData);
			}
			canvas->doEvent(WCH_UI_UpdateData);
		}
	}
}
/////////////////////////////////////////
//
//crJXJBattleRoomMemberInfoCanvasMethod
//
/////////////////////////////////////////
crJXJBattleRoomMemberInfoCanvasMethod::crJXJBattleRoomMemberInfoCanvasMethod(){}
crJXJBattleRoomMemberInfoCanvasMethod::crJXJBattleRoomMemberInfoCanvasMethod(const crJXJBattleRoomMemberInfoCanvasMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_playerIcon(handle.m_playerIcon),
	m_playerNameInput(handle.m_playerNameInput),
	m_playerLvInput(handle.m_playerLvInput),
	m_playerBingFa(handle.m_playerBingFa),
	m_playerZhenFa(handle.m_playerZhenFa),
	m_playerDongCha(handle.m_playerDongCha),
	m_strRoleName(handle.m_strRoleName),
	m_strZhanDouLi(handle.m_strZhanDouLi),
	m_strUp(handle.m_strUp)
{
}
void crJXJBattleRoomMemberInfoCanvasMethod::inputParam(int i, void *param)
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

void crJXJBattleRoomMemberInfoCanvasMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_playerIcon=str;
		break;
	case 1:
		m_playerNameInput=str;
		break;
	case 2:
		m_playerLvInput=str;
		break;
	case 3:
		m_playerBingFa=str;
		break;
	case 4:
		m_playerZhenFa=str;
		break;
	case 5:
		m_playerDongCha=str;
		break;
	case 6:
		m_strZhanDouLi = str;
		break;
	case 7:
		m_strRoleName = str;
		break;
	case 8:
		m_equip[0] = str;
		break;
	case 9:
		m_equip[1] = str;
		break;
	case 10:
		m_equip[2] = str;
		break;
	case 11:
		m_equip[3] = str;
		break;
	case 12:
		m_equip[4] = str;
		break;
	case 13:
		m_defaultname[0] = str;
		break;
	case 14:
		m_defaultname[1] = str;
		break;
	case 15:
		m_defaultname[2] = str;
		break;
	case 16:
		m_defaultname[3] = str;
		break;
	case 17:
		m_defaultname[4] = str;
		break;
	case 18:
		m_strUp = str;
		break;
	case 19:
		m_strSwPlane = str;
		break;
	case 20:
		m_strNpcTouXiang = str;
		break;
	default:
		break;
	}
}

void crJXJBattleRoomMemberInfoCanvasMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		crData *canvasData = m_this->getDataClass();
		if(canvasData)
		{
			ref_ptr<crImageBoxWidgetNode>  imageFaceIcon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_playerIcon));
			ref_ptr<crImageBoxWidgetNode>  imageNpcFaceIcon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strNpcTouXiang));
			ref_ptr<crStaticTextWidgetNode>  playerLvInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_playerLvInput));
			ref_ptr<crStaticTextWidgetNode>  playerNameInput = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_playerNameInput));
			//ref_ptr<crStaticTextWidgetNode>  playerBF = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_playerBingFa));
			//ref_ptr<crStaticTextWidgetNode>  playerZF = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_playerZhenFa));
			//ref_ptr<crStaticTextWidgetNode>  playerDC = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_playerDongCha));
			ref_ptr<crStaticTextWidgetNode>  playerFightPower = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strZhanDouLi));
			ref_ptr<crStaticTextWidgetNode>  roleName = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strRoleName));
			ref_ptr<crMultiSwitch>  planeSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwPlane));

			if(!imageFaceIcon|| !playerLvInput || !playerNameInput \
				|| !playerFightPower || !roleName || !planeSwitch || !imageNpcFaceIcon)
				return;
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
			RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
			canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
			crInstanceItem* curSelect = (crInstanceItem *)param;
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
			
			
			int playerid = crMyPlayerData::getInstance()->getPlayerID();
			if (curSelect)
			{
				playerid = curSelect->getID();
			}

			
				float output = 0.0f;
				//t头像
			if (curSelect->getItemtype() == crInstanceItem::Role)
			{
				planeSwitch->setActiveSwitchSet(0);
				RoomMemberInfoMap::iterator itr_info = infoMap->find(playerid);
				if (itr_info != infoMap->end())
				{
					unsigned char iconID = itr_info->second->getPlayerIconID();
					unsigned char playerLv = itr_info->second->getPlayerLv();
					unsigned short bf = itr_info->second->getBingFa();
					unsigned short zf = itr_info->second->getZhenFa();
					unsigned short dc = itr_info->second->getDongCha();
					curSelect->doEvent(WCH_JXJGetRoleFightingPower,MAKECREPARAM(&output,NULL));
					int fightPower = output;

					ref_ptr<crTableIO> playerIconTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJPlayerIconTab);
					if (playerIconTab.valid())
					{
						int iconindex2 = playerIconTab->getTitleIndex("icon大");
						if (playerIconTab->queryOneRecord(0, crArgumentParser::appItoa(iconID), record) >= 0)
						{
							imageFaceIcon->setImageName(record[iconindex2]);
							// 						if(iconindex2>=0)
							// 						{
							// 							crStateSet *state_set = dynamic_cast<crObject *>(( dynamic_cast<crGroup *>(imageFaceIcon->getChild(0)))->getChild(0))->getDrawable(0)->getStateSet();
							// 							if (state_set)
							// 							{
							// 								crTexture2D *texture_2d = dynamic_cast<crTexture2D *>(state_set->getTextureAttribute(0,crStateAttribute::TEXTURE));
							// 								if (texture_2d)
							// 								{
							// 									texture_2d->setImage(0, texture_2d->getImageDataRequestHandler()->requestImageFile(record[1],texture_2d));
							// 									imageFaceIcon->setVisiable(true);
							// 								}
							// 							}
							// 						}
						}
					}

					playerLvInput->setString(crArgumentParser::appItoa(playerLv));
					playerNameInput->setString(itr_info->second->getCharacterName());

					string strFightingPower = crArgumentParser::appItoa(fightPower);
					playerFightPower->setString(strFightingPower);
				}
			}
			else
			{
				planeSwitch->setActiveSwitchSet(1);
				int abstractid = curSelect->getAbstractItemID();
				ref_ptr<crTableIO> dataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_NpcDataAttrTab);
				if (dataAttrTab.valid())
				{
					int iconindex = dataAttrTab->getTitleIndex("Icon");
					if (dataAttrTab->queryOneRecord(0, crArgumentParser::appItoa(abstractid), record) >= 0)
					{
						imageNpcFaceIcon->setVisiable(true);
						imageNpcFaceIcon->setImageName(record[iconindex]);
					}
				}
			}
			//装备显示
// 			canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
// 			crInstanceItem* curSelect = (crInstanceItem *)param;
// 			canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
// 			RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
			RoomMemberInfoMap::iterator itr_cur = infoMap->find(playerid);

			ref_ptr<crImageBoxWidgetNode>  equip[5] = {NULL};
			for (int i = 0;i<5;i++)
			{
				equip[i] = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_equip[i]));
				if(equip[i].valid())
					equip[i]->setVisiable(false);
			}

			ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
			if(itr_cur!=infoMap->end() && itemtab.valid())
			{
				JXJLordEquipVec & equipVec = itr_cur->second->getEquipVec();

				//装备
				int imageid = itemtab->getTitleIndex("icon");
				for (int i = 0; i<equipVec.size();i++)
				{
					if(equip[i].valid())
					{
						equip[i]->setVisiable(true);
						equip[i]->setImageName(m_defaultname[i]);
					}
					if(equipVec[i].first>0)
					{
						if(itemtab->queryOneRecord(0,crArgumentParser::appItoa(equipVec[i].first),record)>=0)
							if(equip[i].valid() && record.size()>0)
								equip[i]->setImageName(record[imageid]);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJBattleLordShowEquipMethod
//
/////////////////////////////////////////
crJXJBattleLordShowEquipMethod::crJXJBattleLordShowEquipMethod(){}
crJXJBattleLordShowEquipMethod::crJXJBattleLordShowEquipMethod(const crJXJBattleLordShowEquipMethod& handle):
	crMethod(handle)
{
}
void crJXJBattleLordShowEquipMethod::inputParam(int i, void *param)
{

	switch(i) 
	{
	case 1:
		m_this = (crButtonWidgetNode *)param;
		break;
	}
}

void crJXJBattleLordShowEquipMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strSwitch = str;
		break;
	case 1:
		m_strBt = str;
		break;
	case 2:
		m_nIndex = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJBattleLordShowEquipMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		if(parentCanvas.valid())
		{
			ref_ptr<crButtonWidgetNode>  btn = dynamic_cast<crButtonWidgetNode *>(parentCanvas->getWidget(m_strBt));
			if(btn.valid())
			{
				btn->setVisiable(true);
				m_this->setVisiable(false);
			}
			ref_ptr<crMultiSwitch>  switchI = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_strSwitch));
			if(switchI.valid())
			{
				switchI->setActiveSwitchSet(m_nIndex);
			}
		}
	}
}
/////////////////////////////////////////
//
//crJXJBattleLordEquipTipInfoMethod
//
/////////////////////////////////////////
crJXJBattleLordEquipTipInfoMethod::crJXJBattleLordEquipTipInfoMethod():
	m_ea(NULL),
	m_this(NULL),
	m_index(0)
{
}
crJXJBattleLordEquipTipInfoMethod::crJXJBattleLordEquipTipInfoMethod(const crJXJBattleLordEquipTipInfoMethod& handle):
	crMethod(handle),
	m_tips(handle.m_tips),
	m_name(handle.m_name),
	m_info1(handle.m_info1),
	m_info2(handle.m_info2),
	m_itemlv(handle.m_itemlv),
	m_itemicon(handle.m_itemicon),
	m_itemequipon(handle.m_itemequipon),
	m_canvas(handle.m_canvas),
	m_colorred(handle.m_colorred),
	m_infocanvas(handle.m_infocanvas),
	m_infobk(handle.m_infobk),
	m_infodi(handle.m_infodi),
	m_offsets(handle.m_offsets),
	m_index(handle.m_index),
	m_ea(NULL),
	m_this(NULL)
{
}
void crJXJBattleLordEquipTipInfoMethod::inputParam(int i, void *param)
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
void crJXJBattleLordEquipTipInfoMethod::addParam(int i, const std::string& str)
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
		m_info1 = str;
		break;
	case 3:
		m_info2 = str;
		break;
	case 4:
		m_itemlv = str;
		break;
	case 5:
		m_itemicon = str;
		break;
	case 6:
		m_itemequipon = str;
		break;
	case 7:
		m_canvas = str;
		break;
	case 8:
		m_infocanvas = str;
		break;
	case 9:
		m_infobk = str;
		break;
	case 10:
		m_infodi = str;
		break;
	case 11:
		crArgumentParser::appAtoVec(str,m_offsets);
		break;
	case 12:
		crArgumentParser::appAtoVec(str,m_colorred);//red
		m_colorred /= 255.0f;
		break;
	case 13:
		m_index = atoi(str.c_str());
		break;
	}
}
void crJXJBattleLordEquipTipInfoMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  tipsCanvas = crFilterRenderManager::getInstance()->findCanvas(m_tips);
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	ref_ptr<crCanvasNode>  infocanvas = crFilterRenderManager::getInstance()->findCanvas(m_infocanvas);
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(tipsCanvas.valid() && playerData &&playerData->getMainRole()&&playerData->getMainRole()->getDataClass()&& infocanvas.valid() && canvas.valid())
	{
		bool ifshowcanvas = false;
		void *param;
		ref_ptr<crStaticTextWidgetNode>  name = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_name));
		//crStaticTextWidgetNode *info1 = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_info1));
		ref_ptr<crStaticTextWidgetNode>  itemlv = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemlv));
		ref_ptr<crImageBoxWidgetNode>  itemicon = dynamic_cast<crImageBoxWidgetNode *>(tipsCanvas->getWidget(m_itemicon));
		ref_ptr<crStaticTextWidgetNode>  itemequipon = dynamic_cast<crStaticTextWidgetNode *>(tipsCanvas->getWidget(m_itemequipon));
		//
		ref_ptr<crHypertextWidgetNode>  info2 = dynamic_cast<crHypertextWidgetNode *>(infocanvas->getWidget(m_info2));
		crMatrixTransform *infobk = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infobk));
		crMatrixTransform *infodi = dynamic_cast<crMatrixTransform *>(infocanvas->getChildNode(m_infodi));
		if (name.valid())
		{
			name->clearString();
		}
		//if (info1)
		//{
		//	info1->clearString();
		//}
		if (info2.valid())
		{
			info2->clearText();
		}
		if (itemlv.valid())
		{
			itemlv->clearString();
		}
		if (itemicon.valid())
		{
			itemicon->clearImage();
		}
		if (itemequipon.valid())
		{
			itemequipon->clearString();
		}
		crRole *mainRole = playerData->getMainRole();
		crData *roleData = mainRole->getDataClass();
		roleData->getParam(WCHDATA_Level,param);
		unsigned char rolelv = *(unsigned char *)param;
		crData *data = playerData->getDataClass();
		int itemid = 0;
		_crInt32 equipmagic = 0;
		JXJLordEquipVec lordEquipVec;
		crVector4f colormode;//white,green,blue,purple,orange
		ref_ptr<crTableIO> qualitytab = crGlobalHandle::gData()->gGlobalTable( WCHDATA_JXJQualityTab);
		crTableIO::StrVec colorrecord;
		int qualityindex = qualitytab->getTitleIndex("color");
		
		crData *canvasData = canvas->getDataClass();
		if(canvasData)
		{
			canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
			canvasData->getParam(WCHDATA_JXJRoomMemberInfoMap,param);
			RoomMemberInfoMap *infoMap = (RoomMemberInfoMap *)param;
			canvasData->getParam(WCHDATA_JXJRoomMemberCurSelect,param);
			crInstanceItem* curSelect = (crInstanceItem *)param;
			RoomMemberInfoMap::iterator itr_member = infoMap->find(curSelect->getID());
			if(itr_member!=infoMap->end())
			{
				lordEquipVec = itr_member->second->getEquipVec();
				itemid = lordEquipVec[m_index].first;
				equipmagic = lordEquipVec[m_index].second;
			}

			canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
		
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(itemid>0 && itemtab.valid())
		{
			ref_ptr<crTableIO>lordequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);

			int iconindex = itemtab->getTitleIndex("icon");
			int nameindex = itemtab->getTitleIndex("name");
			int typeindex = itemtab->getTitleIndex("类型");
			int colorindex = itemtab->getTitleIndex("品质");
			int needlvindex = itemtab->getTitleIndex("人物等级");
			crTableIO::StrVec record, lordrecord;

			if (itemtab->queryOneRecord(0, crArgumentParser::appItoa(itemid), record) >= 0)
			{
				int colormodeid = atoi(record[colorindex].c_str());
				if (qualitytab->queryOneRecord(0, crArgumentParser::appItoa(colormodeid), colorrecord) >= 0)
				{
					crArgumentParser::appAtoVec(colorrecord[qualityindex], colormode);
				}
				if (name.valid())
				{
					name->setColor(colormode / 255.0f);
					name->setString(record[nameindex]);
				}
				std::string file;
				if (itemlv.valid())
				{
					if (rolelv < atoi(record[needlvindex].c_str()))//人物等级不足时显示红字
					{
						itemlv->setColor(m_colorred);
						if (itemequipon.valid())
						{
							itemequipon->setColor(m_colorred);
						}
					}
					else
					{
						itemlv->setColor(COLOR_WHITE);
						if (itemequipon.valid())
						{
							itemequipon->setColor(COLOR_GREEN);
						}
					}
					itemlv->setString(record[needlvindex]);
				}
				if (itemicon.valid())
				{
					itemicon->setImageName(record[iconindex]);
				}
				if (itemequipon.valid())
				{
					itemequipon->setString(std::string("已装备"));
				}
				if (info2.valid())
				{
					ItemMap itemMap;
					rcfg::ConfigScript outscript(&itemMap);
					crVector2i item(itemid, equipmagic);
					playerData->doEvent(WCH_JXJCreateItemTips, MAKECREPARAM(&item, &outscript));
					info2->setHypertext(outscript);
				}

				///////////计算缩放
				float diYlength = 0.0f;
				float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
				info2->updateData();
				const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
				const crBoundingBox &bbox2 = info2->getBoundBox();
				float texheight = 0.0f;
				float needmove = 0.0f;
				if (bbox.valid())
				{
					texheight = bbox.yLength();
					texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()/crDisplaySettings::instance()->getUIScaleFactor();
					if (texheight<boxYlength) texheight = boxYlength;
					float scale = texheight / boxYlength;

					const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
					crVector3 centerTansTar = info2->getBound().center();

					float tmpLength = bbox2.yLength();
					if (infodi)
					{
						float diYlength = infodi->getChild(0)->getBoundBox().yLength();
						centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.25f * diYlength;
						//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
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
						diYlength = infodi->getChild(0)->getBoundBox().yLength();
						//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
						needmove = boxYlength - texheight + diYlength * 0.25f;
						//needmove = boxYlength - texheight;
						infodi->setMatrix(crMatrix::translate(0.0f, needmove/*+c_tipsoffsets[2]*scale*/, 0.0f));
						diYlength = infodi->getChild(0)->getBoundBox().yLength();
					}
				}
				else
				{
					infobk->setMatrix(crMatrix::scale(1.0f, 1.0f/*m_expandnum*/, 1.0f) * crMatrix::translate(0.0f, 0.0f, 0.0f));
					if (infodi) infodi->setMatrix(crMatrix::translate(0.0f, 0.0f, 0.0f));
				}
				///////////////////////////////////////
				const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
				const crBoundingBox &iconbox = m_this->getBoundBox();
				const crBoundingBox &tips = tipsCanvas->getBoundBox();
				float posx = (tips.m_max[0] - tips.m_min[0])*0.5;
				float posy = (tips.m_max[1] - tips.m_min[1])*0.5 + diYlength;
				crVector3 pos(iconbox.m_max[0], iconbox.m_max[1], 0.0f);
				/////////////////////////////////////////
				crVector3 pos2(iconbox.m_min[0], iconbox.m_min[1], 0.0f);
				pos = pos * mat;
				pos2 = pos2 * mat;
				float posx2, posy2;
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
				if ((abs(posy2) + posy)>1.0f)
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
				//infobk->setMatrix(crMatrix::scale(1.0f,m_expandnum,1.0f) * crMatrix::translate(0,(infobk->getChild(0)->getBoundBox().yLength()*0.5f),0.0f));
				infocanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f));//(crMatrix::translate(mouse[0],mouse[1]-posy*2.0f-tipsCanvas->getBoundBox().yLength()*0.5f,0.0f));

				/////////////////////////////////////////
				tipsCanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1], 0.0f));
				tipsCanvas->setCanFocus(false);
				infocanvas->setCanFocus(false);

				crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(), true);
				crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(), true);
				ifshowcanvas = true;
			}
		}
		if (!ifshowcanvas)
		{
			crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
			crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
		}
		//	ref_ptr<crTableIO> lordequiptab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJLordEquipTab);
		//	//int tnameindex = lordequiptab->getTitleIndex("称号");
		//	//int shuxingindex = lordequiptab->getTitleIndex("属性");

		//	ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		//	int iconindex = itemtab->getTitleIndex("icon");
		//	//int info2index = itemtab->getTitleIndex("物品描述");
		//	int nameindex = itemtab->getTitleIndex("name");
		//	int typeindex = itemtab->getTitleIndex("类型");
		//	int colorindex = itemtab->getTitleIndex("品质");
		//	int needlvindex = itemtab->getTitleIndex("人物等级");
		//	crTableIO::StrVec record,lordrecord;

		//	if (itemtab->queryOneRecord(0,crArgumentParser::appItoa(itemid),record)>=0)
		//	{
		//		int colormodeid = atoi(record[colorindex].c_str());
		//		if (qualitytab->queryOneRecord(0,crArgumentParser::appItoa(colormodeid),colorrecord)>=0)
		//		{
		//			crArgumentParser::appAtoVec(colorrecord[qualityindex],colormode);
		//		}
		//		if(name.valid())
		//		{
		//			name->setColor(colormode/255.0);
		//			//name->setColor(crVector4f(88.0f,200.0f,200.0f,255.0f));
		//			name->setString(record[nameindex]);
		//		}
		//		std::string file;
		//		if (itemlv.valid())
		//		{
		//			if (rolelv < atoi(record[needlvindex].c_str()))//人物等级不足时显示红字
		//			{
		//				itemlv->setColor(m_colorred);
		//				if (itemequipon.valid())
		//				{
		//					itemequipon->setColor(m_colorred);
		//				}
		//			}
		//			else
		//			{
		//				itemlv->setColor(COLOR_WHITE);
		//				if (itemequipon.valid())
		//				{
		//					itemequipon->setColor(COLOR_GREEN);
		//				}
		//			}
		//			itemlv->setString(record[needlvindex]);
		//		}
		//		if (itemicon.valid())
		//		{
		//			itemicon->setImageName(record[iconindex]);
		//		}
		//		if (itemequipon.valid())
		//		{
		//			itemequipon->setString(std::string("已装备"));
		//		}
		//		if(info2.valid())
		//		{
		//			ItemMap itemMap;
		//			rcfg::ConfigScript outscript(&itemMap);
		//			crVector2i item(itemid,equipmagic);
		//			crHandle *handle = playerData->getHandle(MAKEINT64(WCH_JXJCreateItemTips,NULL),"JXJCreateItemTips");
		//			handle->inputParam(3,&lordEquipVec);
		//			playerData->doEvent(WCH_JXJCreateItemTips,MAKECREPARAM(&item,&outscript));
		//			info2->setHypertext(outscript);
		//		}

		//		const crMatrix &mat = m_this->getParentCanvas()->getMatrix();
		//		const crBoundingBox &iconbox = m_this->getBoundBox();
		//		const crBoundingBox &tips = tipsCanvas->getBoundBox();
		//		float posx = (tips.m_max[0]-tips.m_min[0])*0.5;
		//		float posy = (tips.m_max[1]-tips.m_min[1])*0.5;
		//		crVector3 pos(iconbox.m_max[0],iconbox.m_max[1],0.0f);
		//		/////////////////////////////////////////
		//		crVector3 pos2(iconbox.m_min[0] ,iconbox.m_min[1],0.0f);
		//		pos = pos * mat;
		//		pos2 = pos2 * mat;
		//		float posx2,posy2;
		//		const crBoundingBox &infos = infobk->getBoundBox();
		//		float posinfox = (infos.m_max[0]-infos.m_min[0]);
		//		float posinfoy = (infos.m_max[1]-infos.m_min[1]);
		//		/////////////////////////////////////////

		//		///////////计算缩放
		//		float boxYlength = infobk->getChild(0)->getBoundBox().yLength()/**crDisplaySettings::instance()->getUIViewScale()[1]*/;
		//		info2->updateData();
		//		const crBoundingBox &bbox = info2->getTextObject()->getBoundBox();
		//		const crBoundingBox &bbox2 = info2->getBoundBox();
		//		float texheight = 0.0f;
		//		float needmove = 0.0f;
		//		if(bbox.valid())
		//		{
		//			texheight = bbox.yLength();
		//			//texheight += 32.0f*crDisplaySettings::instance()->getUIViewScale()[1];//*crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale();
		//			//texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale()*c_tipsoffsets;
		//			texheight *= crDisplaySettings::instance()->getUIPixelMeter() * crDisplaySettings::instance()->getUIHeightScale();
		//			if(texheight<boxYlength) texheight = boxYlength;
		//			float scale = texheight/boxYlength;

		//			float needmove ;
		//			const crBoundingBox &tipsbb = tipsCanvas->getBoundBox();
		//			crVector3 centerTansTar = info2->getBound().center();

		//			float tmpLength = bbox2.yLength();
		//			if(infodi)
		//			{
		//				float diYlength = infodi->getChild(0)->getBoundBox().yLength();
		//				centerTansTar[1] += 0.5f * (tmpLength - texheight) + 0.125f * diYlength;
		//				//centerTansTar[1] += 0.5f * (tmpLength - texheight) ;
		//			}
		//			else
		//				centerTansTar[1] += 0.5f * (tmpLength - texheight);
		//			// 源中心点
		//			crVector3 centerTansSrc = infobk->getChild(0)->getBoundBox().center();
		//			// 将源中心点平移到原点
		//			crMatrix put_mat = crMatrix::translate(centerTansSrc);
		//			crMatrix inv_put = crMatrix::inverse(put_mat);
		//			crMatrix rotmat;
		//			rotmat.postMult(inv_put);
		//			rotmat.postMult(crMatrix::scale(1.0f,scale,1.0f));
		//			rotmat.postMult(crMatrix::translate(centerTansTar));
		//			if(infobk)infobk->setMatrix(rotmat);

		//			if(infodi)
		//			{
		//				float diYlength = infodi->getChild(0)->getBoundBox().yLength();
		//				//needmove = diYlength*0.5f*(scale-1.0f) - needmove;
		//				needmove = boxYlength - texheight + diYlength * 0.125f;
		//				infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
		//			}
		//			//float onpixel = crDisplaySettings::instance()->getUIPixelMeter()*crDisplaySettings::instance()->getUIHeightScale();
		//			//needmove = texheight-boxYlength;/*/crDisplaySettings::instance()->getUIViewScale()[1]*/
		//			//infobk->setMatrix(crMatrix::scale(1.0f,scale/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[1]*scale*/,0.0f));
		//			////needmove = texheight/crDisplaySettings::instance()->getUIViewScale()[1];
		//			//if(infodi)
		//			//{
		//			//	float diYlength = infodi->getChild(0)->getBoundBox().yLength();
		//			//	needmove = diYlength*0.5f*(scale-1.0f) - needmove;
		//			//	infodi->setMatrix(crMatrix::translate(0.0f,needmove/*+c_tipsoffsets[2]*scale*/,0.0f));
		//			//}
		//		}
		//		else
		//		{
		//			infobk->setMatrix(crMatrix::scale(1.0f,1.0f/*m_expandnum*/,1.0f) * crMatrix::translate(0.0f,0.0f,0.0f));
		//			if(infodi) infodi->setMatrix(crMatrix::translate(0.0f,0.0f,0.0f));
		//		}
		//		/////////////////////////////////////////
		//		if (pos2[0] >= 0)
		//		{
		//			posx2 = pos2[0] - posx;
		//		}
		//		else
		//		{
		//			posx2 = pos[0] + posx;
		//		}
		//		if (pos2[1] >= 0)
		//		{
		//			posy2 = pos[1] - posy;
		//		}
		//		else
		//		{
		//			posy2 = pos2[1] + posy;
		//		}
		//		crVector2 mouse(crVector2(posx2,posy2));

		//		float tipLength = tipsCanvas->getBoundBox().yLength()*0.5f;
		//		float infoLength = infocanvas->getBoundBox().yLength();

		//		tipsCanvas->setMatrix(crMatrix::translate(mouse[0],mouse[1] + tipLength + infoLength,0.0f));
		//		tipsCanvas->setCanFocus(false);
		//		infocanvas->setCanFocus(false);

		//		//infobk->setMatrix(crMatrix::scale(1.0f,m_expandnum,1.0f) * crMatrix::translate(0,(infobk->getChild(0)->getBoundBox().yLength()*0.5f),0.0f));
		//		infocanvas->setMatrix(crMatrix::translate(mouse[0], mouse[1] + tipLength + infoLength, 0.0f));//(crMatrix::translate(mouse[0],mouse[1]-posy*2.0f-tipsCanvas->getBoundBox().yLength()*0.5f,0.0f));

		//		crFilterRenderManager::getInstance()->showCanvas(infocanvas.get(),true);
		//		crFilterRenderManager::getInstance()->showCanvas(tipsCanvas.get(),true);
		//		ifshowcanvas = true;
		//	}
		//}
		//if (!ifshowcanvas)
		//{
		//	crFilterRenderManager::getInstance()->closeCanvas(tipsCanvas.get());
		//	crFilterRenderManager::getInstance()->closeCanvas(infocanvas.get());
		//}
	}
}	
/////////////////////////////////////////
//
//crJXJSetLordEquipCanvasCloseMethod
//
/////////////////////////////////////////
crJXJSetLordEquipCanvasCloseMethod::crJXJSetLordEquipCanvasCloseMethod(){}
crJXJSetLordEquipCanvasCloseMethod::crJXJSetLordEquipCanvasCloseMethod(const crJXJSetLordEquipCanvasCloseMethod& handle):
	crMethod(handle)
{
}
void crJXJSetLordEquipCanvasCloseMethod::inputParam(int i, void *param)
{
}

void crJXJSetLordEquipCanvasCloseMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	}
}

void crJXJSetLordEquipCanvasCloseMethod::operator()(crHandle &handle)
{
	void *param;
	ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	if(crRunGameHandle::getInstance()->isInGame() && canvas.valid() && canvas->getVisiable())
	{
		ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
		if(camera.valid())
		{
			crData *cameraData = camera->getAttachedNode()->getDataClass();
			cameraData->getParam(WCHDATA_TargetType,param);
			unsigned char targettype = *(unsigned char *)param;
			cameraData->getParam(WCHDATA_TargetItem,param);
			crInstanceItem *lastTargetItem = (crInstanceItem*)param;
			if(targettype == Target_Coord && !lastTargetItem)
			{
				crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
			}
			// = Target_Coord;
			
		}
	}
	
}
/////////////////////////////////////////
//
//crJXJOpenRebelTimerMethod
//
/////////////////////////////////////////
crJXJOpenRebelTimerMethod::crJXJOpenRebelTimerMethod():
	m_timer(0L)
{
}
crJXJOpenRebelTimerMethod::crJXJOpenRebelTimerMethod(const crJXJOpenRebelTimerMethod& handle):
	crMethod(handle),
	m_timer(0L)
{
}
void crJXJOpenRebelTimerMethod::inputParam(int i, void *param)
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
void crJXJOpenRebelTimerMethod::addParam(int i, const std::string& str)
{
}
void crJXJOpenRebelTimerMethod::operator()(crHandle &handle)
{
	time_t t = time(0);
	if(m_timer == 0)
	{
		m_timer = t;
		return;
	}
	int dt = t - m_timer;
	if(dt>0)
	{
		m_timer = t;
		void *param;
		crData *data = m_this->getDataClass();
		data->excHandle(MAKECREPARAM(WCH_LockData,1));

		char code = 0;
		data->getParam(WCHDATA_JXJOpenRebel,param);
		bool *openRebel = (bool *)param;
		if(!(*openRebel))
		{
			std::string opentime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRebelBeginTime, 0);
			std::string endtime = crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRebelEndTime, 0);
			char tmp[20];
			strftime( tmp, sizeof(tmp), "%H:%M:%S\0", localtime(&t) );
			crVector3i opentm,endtm,curtm;
			crArgumentParser::appTimetoVec(opentime,opentm);
			crArgumentParser::appTimetoVec(endtime,endtm);
			crArgumentParser::appTimetoVec(tmp,curtm);
			if(opentm<curtm && curtm<endtm)
			{
				int term = (endtm[0]-curtm[0])*3600+(endtm[1]-curtm[1])*60+endtm[2]-curtm[2];//atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleTerm).c_str());
				*openRebel = true;
				data->inputParam(WCHDATA_JXJContinuedRebelTimer,&term);

				m_nRandomTimer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRebelInterval, 0).c_str());
				data->inputParam(WCHDATA_JXJPerRebelTimer,&m_nRandomTimer);
				m_this->doEvent(WCH_JXJRandomRebelCity);
			}
		}
		else
		{
			data->getParam(WCHDATA_JXJContinuedRebelTimer,param);
			int *battleTimer = (int *)param;
			*battleTimer -= dt;

			data->getParam(WCHDATA_JXJPerRebelTimer,param);
			unsigned short *perTimer = (unsigned short *)param;
			if(*perTimer >0)
			{
				*perTimer -= 1;
			}
			if(*battleTimer<=0)
			{
				*battleTimer = 0;
				*openRebel = false;
				*perTimer = 0;

			}
			else
			{
				if(m_nRandomTimer>0)
				{
					m_nRandomTimer -= 1;
				}
				else
				{
					m_this->doEvent(WCH_JXJRandomRebelCity);
					m_nRandomTimer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRebelInterval, 0).c_str());
					data->inputParam(WCHDATA_JXJPerRebelTimer,&m_nRandomTimer);
				}
			}
		}

		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

/////////////////////////////////////////
//
//crJXJRandomRebelCityMethod
//
/////////////////////////////////////////
crJXJRandomRebelCityMethod::crJXJRandomRebelCityMethod():m_this(NULL){}
crJXJRandomRebelCityMethod::crJXJRandomRebelCityMethod(const crJXJRandomRebelCityMethod& handle):
	crMethod(handle),
	m_this(NULL)
{
}
void crJXJRandomRebelCityMethod::inputParam(int i, void *param)
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

void crJXJRandomRebelCityMethod::addParam(int i, const std::string& str)
{
}

void crJXJRandomRebelCityMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crData *shiliData = NULL;
		vector<unsigned short> weiChengchi;
		vector<unsigned short> shuChengchi;
		vector<unsigned short> wuChengchi;

		crData* serverData = m_this->getDataClass();
		serverData->excHandle(MAKECREPARAM(WCH_LockData,1));
		serverData->getParam(WCHDATA_JXJShiliWei,param);
		crJXJShili *shiliWei = (crJXJShili *)param;
		serverData->getParam(WCHDATA_JXJShiliShu,param);
		crJXJShili *shiliShu = (crJXJShili *)param;
		serverData->getParam(WCHDATA_JXJShiliWu,param);
		crJXJShili *shiliWu = (crJXJShili *)param;

		serverData->getParam(WCHDATA_JXJChengChiMap,param);
		ChengchiMap *chengchimap = (ChengchiMap *)param;

		serverData->getParam(WCHDATA_JXJRebelCityMap,param);
		RebelCityMap *rebelCityMap = (RebelCityMap *)param;
		rebelCityMap->clear();

		int weiID = shiliWei->getID();
		int shuID = shiliShu->getID();
		int wuID = shiliWu->getID();
		unsigned char chengzhu;
		for (ChengchiMap::iterator itr_chengchi = chengchimap->begin();	\
			itr_chengchi!=chengchimap->end();							\
			itr_chengchi++)
		{
			chengzhu = itr_chengchi->second->getChengzhuShili();
			if(chengzhu == weiID)
			{
				weiChengchi.push_back(itr_chengchi->first);
			}
			else if(chengzhu == shuID)
			{
				shuChengchi.push_back(itr_chengchi->first);
			}
			else if(chengzhu == wuID)
			{
				wuChengchi.push_back(itr_chengchi->first);
			}
		}

		//随机城池
		random_shuffle(weiChengchi.begin(),weiChengchi.end());
		random_shuffle(shuChengchi.begin(),shuChengchi.end());
		random_shuffle(wuChengchi.begin(),wuChengchi.end());

		float randomPercent = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRandomRebelPercent, 0).c_str())*0.01;

		unsigned char weiRandomCount = (unsigned char)weiChengchi.size()*randomPercent+1;
		unsigned char shuRandomCount = (unsigned char)shuChengchi.size()*randomPercent+1;
		unsigned char wuRandomCount = (unsigned char)wuChengchi.size()*randomPercent+1;
		//随机城池id发给玩家
		ref_ptr<crStreamBuf> weistream = new crStreamBuf;
		ref_ptr<crStreamBuf> shustream = new crStreamBuf;
		ref_ptr<crStreamBuf> wustream = new crStreamBuf;
		crPlayerDataEventPacket packet;

		weistream->createBuf(1+weiRandomCount*2);
		weistream->_writeUChar(weiRandomCount);
		for (int i = 0;i<weiRandomCount;i++)
		{
			weistream->_writeUShort(weiChengchi[i]);
			(*rebelCityMap)[weiID].insert(weiChengchi[i]);
		}

		shustream->createBuf(1+shuRandomCount*2);
		shustream->_writeUChar(shuRandomCount);
		for (int i = 0;i<shuRandomCount;i++)
		{
			shustream->_writeUShort(shuChengchi[i]);
			(*rebelCityMap)[shuID].insert(shuChengchi[i]);
		}

		wustream->createBuf(1+wuRandomCount*2);
		wustream->_writeUChar(wuRandomCount);
		for (int i = 0;i<wuRandomCount;i++)
		{
			wustream->_writeUShort(wuChengchi[i]);
			(*rebelCityMap)[wuID].insert(wuChengchi[i]);
		}

		ref_ptr<crGameServerPlayerData> playerData;
		crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
		if(gameServer)
		{
			crNetManager *netManager = gameServer->getNetManager();
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			if(netManager && netDataManager)
			{
				crData *shiliData = shiliWei->getDataClass();
				if(shiliData)
				{
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap* peopleMap = (PeopleMap *)param;
					for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
					{
						playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(itr_people->first));
						if (playerData.valid())
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,itr_people->first,WCH_JXJRecvRandomRebelCity,weistream.get());
							netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}

				shiliData = shiliShu->getDataClass();
				if(shiliData)
				{
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap* peopleMap = (PeopleMap *)param;
					for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
					{
						playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(itr_people->first));
						if (playerData.valid())
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,itr_people->first,WCH_JXJRecvRandomRebelCity,shustream.get());
							netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}

				shiliData = shiliWu->getDataClass();
				if(shiliData)
				{
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,1));
					shiliData->getParam(WCHDATA_JXJPeopleMap,param);
					PeopleMap* peopleMap = (PeopleMap *)param;
					for (PeopleMap::iterator itr_people = peopleMap->begin();itr_people != peopleMap->end();itr_people++)
					{
						playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(itr_people->first));
						if (playerData.valid())
						{
							crPlayerDataEventPacket::buildReplyPacket(packet,itr_people->first,WCH_JXJRecvRandomRebelCity,wustream.get());
							netManager->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
						}
					}
					shiliData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
		serverData->excHandle(MAKECREPARAM(WCH_LockData,0));
		//通知玩家平叛
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJRecvRandomRebelCityMethod
//////////////////////////////////////////////////////////////////////////
crJXJRecvRandomRebelCityMethod::crJXJRecvRandomRebelCityMethod(){}
crJXJRecvRandomRebelCityMethod::crJXJRecvRandomRebelCityMethod(const crJXJRecvRandomRebelCityMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvRandomRebelCityMethod::inputParam(int i, void *param)
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

void crJXJRecvRandomRebelCityMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_cityMapCanvas = str;
		break;
	case 1:
		m_rebelIcon = str;
		break;
	case 2:
		m_rebelCanvas = str;
		break;
	case 3:
		m_strEnterCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvRandomRebelCityMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType = GameClient_Game)
		{
			vector<int> cityIdVec;
			//接收服务器消息
			unsigned char size = m_stream->_readUChar();
			for (int i = 0;i<size;i++)
			{
				cityIdVec.push_back(m_stream->_readUShort());
			}
			ref_ptr<crCanvasNode>  cityCanvas = crFilterRenderManager::getInstance()->findCanvas(m_cityMapCanvas);
			crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
			ref_ptr<crCanvasNode>  rebelCanvas = crFilterRenderManager::getInstance()->findCanvas(m_rebelCanvas);
			ref_ptr<crCanvasNode>  enterCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strEnterCanvas);
			if(enterCanvas.valid())
				crFilterRenderManager::getInstance()->closeCanvas(enterCanvas.get());
			if(rebelCanvas.valid() && rebelCanvas->getVisiable()==true)
			{
				crFilterRenderManager::getInstance()->closeCanvas(rebelCanvas.get());
			}
			if(playerGameData && cityCanvas.valid())
			{
				crData *playerData = playerGameData->getDataClass();
				if(playerData)
				{
					playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
					playerData->getParam(WCHDATA_JXJRebelCityVec,param);
					RebelCityVec *rebelCityIdVec = (RebelCityVec *)param;
					rebelCityIdVec->clear();
					int index = 0;
					for (vector<int>::iterator itr_id = cityIdVec.begin();itr_id!=cityIdVec.end();itr_id++,index++)
					{
						rebelCityIdVec->push_back(*itr_id);
					}
					unsigned short timer = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJRebelInterval, 0).c_str());
					playerData->inputParam(WCHDATA_JXJRebelTimer,&timer);

					playerData->excHandle(MAKECREPARAM(WCH_LockData,0));

					cityCanvas->doEvent(WCH_UI_UpdateData);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityCanvasInitMethod
//////////////////////////////////////////////////////////////////////////
crJXJRebelCityCanvasInitMethod::crJXJRebelCityCanvasInitMethod(){}
crJXJRebelCityCanvasInitMethod::crJXJRebelCityCanvasInitMethod(const crJXJRebelCityCanvasInitMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelCityCanvasInitMethod::inputParam(int i, void *param)
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

void crJXJRebelCityCanvasInitMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strTitle = str;
		break;
	case 2:
		m_strIntro = str;
		break;
	case 3:
		m_strIcon = str;
		break;
	case 4:
		m_strCopper = str;
		break;
	case 5:
		m_strSwitch = str;
		break;
	default:
		break;
	}
}

void crJXJRebelCityCanvasInitMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record,itemRecord;
		ref_ptr<crStaticTextWidgetNode>  title = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strTitle));
		ref_ptr<crStaticTextWidgetNode>  intro = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strIntro));
		ref_ptr<crStaticTextWidgetNode>  copper = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strCopper));
		ref_ptr<crImageBoxWidgetNode>  imageIcon = dynamic_cast<crImageBoxWidgetNode *>(m_this->getWidget(m_strIcon));
		crMultiSwitch	*ctrlSwitch = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_strSwitch));

		if(!title || !intro || !copper || !imageIcon || !ctrlSwitch) return;
		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);

		if(cityMapCanvas.valid() && chengchiTab.valid())
		{
			int junlingIndex = chengchiTab->getTitleIndex("平叛消耗军令");
			int copperIndex = chengchiTab->getTitleIndex("平叛铜钱奖励");
			int itemIndex = chengchiTab->getTitleIndex("平叛掉落id");
			int nameIndex = chengchiTab->getTitleIndex("名字");

			crData *canvasData = cityMapCanvas->getDataClass();
			crMyPlayerData *mydata = crMyPlayerData::getInstance();
			crPlayerGameData *gameData = mydata->getPlayerGameData();
			crData* data = gameData->getDataClass();
			if(canvasData && data)
			{
				data->getParam(WCHDATA_JXJRebelTimer,param);
				unsigned short timer = *(unsigned short *)param;
				if(timer == 0)
				{
					//查询时间
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);

					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJQueryRebelTimer,NULL);
					if(netConductor) 
						netConductor->getNetManager()->sendPacket("all",packet);
				}
				canvasData->getParam(WCHDATA_JXJChooseRebelCityID,param);
				unsigned short chengchiID = *(unsigned short *)param;
				canvasData->getParam(WCHDATA_JXJBatchRebelCount,param);
				unsigned char rebelCount = *(unsigned char *)param;
				if(rebelCount<=0)
					ctrlSwitch->setActiveSwitchSet(1);

				data->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
				unsigned char freeRebelCount = *(unsigned char *)param;

				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiID),record)>=0)
				{
					char szText[128];
					sprintf_s(szText,sizeof(szText),"【%s】 %02d:%02d\0",record[nameIndex].c_str(),timer/60,timer%60);
					title->setString(string(szText));

					if(freeRebelCount>0)
					{
						sprintf_s(szText,sizeof(szText),"平叛%s需要%d个军令\n今日还可免费平叛%d次\0",record[nameIndex].c_str(),atoi(record[junlingIndex].c_str()),freeRebelCount);
					}
					else
					{
						sprintf_s(szText,sizeof(szText),"平叛%s需要%d个军令\n今日还可免费平叛已用完\0",record[nameIndex].c_str(),atoi(record[junlingIndex].c_str()),freeRebelCount);
					}
					intro->setString(string(szText));

					crVector3i idVec;
					crArgumentParser::appAtoVec(record[itemIndex],idVec);
					if(itemtab.valid() && itemtab->queryOneRecord(0,crArgumentParser::appItoa(idVec[0]),itemRecord)>=0)
					{
						imageIcon->setImageName(itemRecord[itemtab->getTitleIndex("icon")]);
					}

					sprintf_s(szText,sizeof(szText),"铜钱奖励  %d\0",atoi(record[copperIndex].c_str()));
					copper->setString(string(szText));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityStopMethod
//////////////////////////////////////////////////////////////////////////
crJXJRebelCityStopMethod::crJXJRebelCityStopMethod(){}
crJXJRebelCityStopMethod::crJXJRebelCityStopMethod(const crJXJRebelCityStopMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelCityStopMethod::inputParam(int i, void *param)
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

void crJXJRebelCityStopMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
// 	case 1:
// 		m_strTitle = str;
// 		break;
// 	case 2:
// 		m_strIntro = str;
// 		break;
// 	case 3:
// 		m_strIcon = str;
// 		break;
// 	case 4:
// 		m_strCopper = str;
// 		break;
	default:
		break;
	}
}

void crJXJRebelCityStopMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;

		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		//ref_ptr<crTableIO> itemtab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
		if(cityMapCanvas.valid())
		{
			crData *canvasData = cityMapCanvas->getDataClass();
			if(canvasData)
			{
				canvasData->getParam(WCHDATA_JXJChooseRebelCityID,param);
				unsigned short chengchiID = *(unsigned short *)param;
				canvasData->getParam(WCHDATA_JXJBatchRebelCount,param);
				unsigned char rebelCount = *(unsigned char *)param;

				if(rebelCount>0)
				{
					ref_ptr<crStreamBuf> stream = new crStreamBuf;
					crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
					
					stream->createBuf(2);
					stream->_writeUShort(chengchiID);
					crPlayerDataEventPacket packet;
					crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvStopCounterinsurgency,stream.get());
					if(netConductor) 
						netConductor->getNetManager()->sendPacket("all",packet);

					rebelCount = 0;
					canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&rebelCount);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvStopCounterinsurgencyMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvStopCounterinsurgencyMethod::crJXJRecvStopCounterinsurgencyMethod(){}
crJXJRecvStopCounterinsurgencyMethod::crJXJRecvStopCounterinsurgencyMethod(const crJXJRecvStopCounterinsurgencyMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvStopCounterinsurgencyMethod::inputParam(int i, void *param)
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

void crJXJRecvStopCounterinsurgencyMethod::addParam(int i, const std::string& str)
{

}

void crJXJRecvStopCounterinsurgencyMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			crServerBrainHandle * gameServerBrain = crServerBrainHandle::getInstance();
			if(gameServerBrain)
			{
				crData *gsBrainData = gameServerBrain->getDataClass();
				if(gsBrainData)
				{
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
					int playerid = m_this->getPlayerID();

					gsBrainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
					JXJCounterMap *counterplayermap = (JXJCounterMap *)param;
					for(JXJCounterMap::iterator itr = counterplayermap->begin();
						itr != counterplayermap->end();
						++itr)
					{
						for (JXJCounterVec::iterator vitr = itr->second.begin();
							vitr != itr->second.end();
							++vitr)
						{
							if (playerid == (*vitr)->getPlayerID())
							{
								if(itr->first != 0)
								{
									ref_ptr<crCounterPlayerInfo> playerinfo = new crCounterPlayerInfo;
									playerinfo->setPlayerID((*vitr)->getPlayerID());
									playerinfo->setTotalCount((*vitr)->getTotalCount());
									playerinfo->setChengChiID((*vitr)->getChengChiID());
									playerinfo->setRemainCount(0);
									//playerinfo->setState(FubenRushType_Rushing);
									vitr = itr->second.erase(vitr);
									(*counterplayermap)[0].push_back(playerinfo);
									break;
								}
								else
								{
									//(*vitr)->setState(FubenRushType_Rushing);
									(*vitr)->setRemainCount(0);
								}
							}
						}
					}
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
	}
	//if(m_this && m_stream.valid())
	//{
	//	void *param;
	//	if(m_netType == GameServer)
	//	{
	//		crServerBrainHandle * gameServerBrain = crServerBrainHandle::getInstance();
	//		if(gameServerBrain)
	//		{
	//			crData *brainData = gameServerBrain->getDataClass();
	//			if(brainData)
	//			{
	//				brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
	//				brainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
	//				CounterInfoMap *counterinsurgencyMap = (CounterInfoMap *)param;
	//				int playerID = m_this->getPlayerID();

	//				counterinsurgencyMap->erase(playerID);
	//				brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
	//			}
	//		}
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
//crJXJRebelCityCanvasFrameMethod
//////////////////////////////////////////////////////////////////////////
crJXJRebelCityCanvasFrameMethod::crJXJRebelCityCanvasFrameMethod()
{}
crJXJRebelCityCanvasFrameMethod::crJXJRebelCityCanvasFrameMethod(const crJXJRebelCityCanvasFrameMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelCityCanvasFrameMethod::inputParam(int i, void *param)
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
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = (float*)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJRebelCityCanvasFrameMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strTitle = str;
		break;
	case 2:
		m_strBtn1 = str;
		break;
	case 3:
		m_strBtn2 = str;
		break;
	case 4:
		m_strProgress = str;
		break;
	case 5:
		m_strInput = str;
		break;
	default:
		break;
	}
}

void crJXJRebelCityCanvasFrameMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		ref_ptr<crStaticTextWidgetNode>  title = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strTitle));
		ref_ptr<crButtonWidgetNode>  btn1 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strBtn1));
		ref_ptr<crButtonWidgetNode>  btn2 = dynamic_cast<crButtonWidgetNode *>(m_this->getWidget(m_strBtn2));
		ref_ptr<crProgressWidgetNode>  progress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_strProgress));
		ref_ptr<crStaticTextWidgetNode>  input = dynamic_cast<crStaticTextWidgetNode *>(m_this->getWidget(m_strInput));
		if(input.valid()) input->setString(string("平叛中..."));

		if(!title || !btn1 || !btn2 || !progress) return;
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();

		if(cityMapCanvas.valid() &&chengchiTab.valid() && myPlayer)
		{
			int nameIndex = chengchiTab->getTitleIndex("名字");
			crData *canvasData = cityMapCanvas->getDataClass();
			crData* data = myPlayer->getDataClass();
			unsigned char vipLevel;
			if(canvasData && data)
			{
				canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
				data->getParam(WCHDATA_JXJVipLv,param);
				vipLevel = *(unsigned char *)param;
				unsigned short mintimer = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());

				canvasData->getParam(WCHDATA_JXJChooseRebelCityID,param);
				unsigned short chengchiID = *(unsigned short *)param;

				data->getParam(WCHDATA_JXJRebelTimer,param);
				unsigned short timer = *(unsigned short *)param;
				if(timer>mintimer)
				{
					if(btn1->getEnable()==false)
					{
						btn1->setEnable(true);
					}
					if(btn2->getEnable()== false)
					{
						btn2->setEnable(true);
					}

					if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiID),record)>=0)
					{
						char szText[128];
						sprintf_s(szText,sizeof(szText),"【%s】 %02d:%02d\0",record[nameIndex].c_str(),timer/60,timer%60);
						title->setString(string(szText));
					}
				}
				else
				{
					btn1->setEnable(false);
					btn2->setEnable(false);
					//crFilterRenderManager::getInstance()->showCanvas(m_this,false);
				}
				//(*timer)--;
			//	float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());
			//	canvasData->getParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,param);
			//	float *turntimer = (float *)param;
			//	if(*turntimer >= 0 && turnTimer >= 0)
			//	{
			//		progress->setProgress( *turntimer / turnTimer);
			//		if (*turntimer>0)
			//		{
			//			if(m_dt)
			//			{
			//				(*turntimer) -= *m_dt;
			//			}
			//			else
			//			{
			//				-- (*turntimer) ;
			//			}
			//		}
			//	}
				//倒计时
				canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJRebelProgressFrameUpdateMethod
//
/////////////////////////////////////////
crJXJRebelProgressFrameUpdateMethod::crJXJRebelProgressFrameUpdateMethod()
{
}
crJXJRebelProgressFrameUpdateMethod::crJXJRebelProgressFrameUpdateMethod(const crJXJRebelProgressFrameUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelProgressFrameUpdateMethod::inputParam(int i, void *param)
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
void crJXJRebelProgressFrameUpdateMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_strCityMap = str;
		break;
	case 1:
		m_strProgress = str;
		break;
	}
}
void crJXJRebelProgressFrameUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCityMap);
		ref_ptr<crProgressWidgetNode>  progress = dynamic_cast<crProgressWidgetNode *>(m_this->getWidget(m_strProgress));
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();

		if(cityMapCanvas.valid() &&chengchiTab.valid() && myPlayer)
		{
			int nameIndex = chengchiTab->getTitleIndex("名字");
			crData *canvasData = cityMapCanvas->getDataClass();
			crData* data = myPlayer->getDataClass();
			unsigned char vipLevel;
			if(canvasData && data)
			{
				canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));

				data->getParam(WCHDATA_JXJVipLv,param);
				vipLevel = *(unsigned char *)param;

				float perTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());
				canvasData->getParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,param);
				float *turntimer = (float *)param;
				if(*turntimer>=0.0f)
				{
					progress->setProgress((perTimer-*turntimer)/perTimer);
					if(*turntimer>0)
						(*turntimer)-=perTimer/50.0f;
				}

				//倒计时
				canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
crJXJCounterinsurgencyMethod::crJXJCounterinsurgencyMethod():m_bIsPiliang(false){}
crJXJCounterinsurgencyMethod::crJXJCounterinsurgencyMethod(const crJXJCounterinsurgencyMethod& handle):
	crMethod(handle),
	m_strCanvas(handle.m_strCanvas),
	m_strMsgCanvas(handle.m_strMsgCanvas),
	m_strMsg(handle.m_strMsg),
	m_strSwitch(handle.m_strSwitch)
{
}
void crJXJCounterinsurgencyMethod::inputParam(int i, void *param)
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
		m_this = (crButtonWidgetNode*)param;
		break;
	}
}

void crJXJCounterinsurgencyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strMsgCanvas = str;
		break;
	case 2:
		m_strMsg = str;
		break;
	case 3:
		m_strSwitch = str;
		break;
	case 4:
		m_bIsPiliang = atoi(str.c_str());
		break;
	case 5:
		m_strProgress = str;
		break;
	default:
		break;
	}
}

void crJXJCounterinsurgencyMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		bool isCanCounterinsurgency = false;
		crTableIO::StrVec record;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crMyPlayerData *mydata = crMyPlayerData::getInstance();
		crPlayerGameData *gameData = mydata->getPlayerGameData();
		crData* data = gameData->getDataClass();
		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		crData *canvasData = NULL;
		if(cityMapCanvas.valid())
			canvasData = cityMapCanvas->getDataClass();
		ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strMsgCanvas);

		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		ref_ptr<crMultiSwitch>  mulitSwitch = NULL;
		ref_ptr<crProgressWidgetNode>  progress = NULL;
		if(parentCanvas.valid())
		{
			mulitSwitch = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_strSwitch));
			progress = dynamic_cast<crProgressWidgetNode *>(parentCanvas->getWidget(m_strProgress));
			if(progress.valid()) progress->setProgress(0.0f);
			//mulitSwitch->setActiveSwitchSet();
		}

		data->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		unsigned char count = 1;
		if(m_bIsPiliang)
		{
			count = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyCount, vipLevel).c_str());
		}


		if(data && canvasData && chengchiTab.valid())
		{
			int junlingIndex = chengchiTab->getTitleIndex("平叛消耗军令");
			int nameIndex = chengchiTab->getTitleIndex("名字");
			//判断免费次数是否充足
			data->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
			unsigned char freeCount = *(unsigned char *)param;
			data->getParam(WCHDATA_JXJJunling,param);
			unsigned short junling = *(unsigned short *)param;
			canvasData->getParam(WCHDATA_JXJChooseRebelCityID,param);
			unsigned short chengchiId = *(unsigned short *)param;

			if(freeCount>=count)
			{
				if(mulitSwitch.valid()) mulitSwitch->setActiveSwitchSet(0);
				float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());
				canvasData->inputParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,&turnTimer);
				stream->createBuf(3);
				stream->_writeUChar(count);
				stream->_writeUShort(chengchiId);
				crPlayerDataEventPacket packet;
				crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCounterinsurgency,stream.get());
				netConductor->getNetManager()->sendPacket("all",packet);

				canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&count);
				//设置倒计时进度条
				crData *braindata = crBrain::getInstance()->getDataClass();
				if (braindata)
				{
					unsigned char totalTime = count * (int)turnTimer;
					braindata->inputParam(WCHDATA_JXJBatchRebelCount,&totalTime);
				}
			}
			else
			{
				//判断军令
				//军令是否充足
				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiId),record)>=0)
				{
					int needJunling = atoi(record[junlingIndex].c_str()) * (count-freeCount);
					if(needJunling<=(int)(junling/100))
					{
						char szText[128];
						sprintf_s(szText,sizeof(szText),"确定要花费%d军令平定【%s】叛乱？(批量平叛军令分次扣除)\0",needJunling,record[nameIndex].c_str());
						ItemMap itemMap;
						rcfg::ConfigScript cfg_script(&itemMap);
						cfg_script.Add("Hypertext");
						cfg_script.Push("Hypertext");
						cfg_script.Add("Content");
						cfg_script.Push("Content");
						cfg_script.Add("Text",string(szText));
						cfg_script.Pop();
						cfg_script.Pop();

						if(msgCanvas.valid())
						{
							ref_ptr<crHypertextWidgetNode>  msg = dynamic_cast<crHypertextWidgetNode *>(msgCanvas->getWidget(m_strMsg));
							if(msg.valid())
								msg->setHypertext(cfg_script);
							crFilterRenderManager::getInstance()->doModal(msgCanvas.get());
							crData *dlgData = msgCanvas->getDataClass();
							int commandtype = CDP_Widget;
							dlgData->inputParam(WCHDATA_CommandDlgParentType,&commandtype);
							dlgData->inputParam(WCHDATA_CommandDlgParent,m_this);
						}	
					}
					else
					{
						//提示军令不足
						gameData->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(2009,NULL));
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJDoCounterinsurgencyMethod
//////////////////////////////////////////////////////////////////////////
crJXJDoCounterinsurgencyMethod::crJXJDoCounterinsurgencyMethod():m_bIsPiLiang(false){}
crJXJDoCounterinsurgencyMethod::crJXJDoCounterinsurgencyMethod(const crJXJDoCounterinsurgencyMethod& handle):
	crMethod(handle),
	m_strCanvas(handle.m_strCanvas)
{
}
void crJXJDoCounterinsurgencyMethod::inputParam(int i, void *param)
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
		m_this = (crButtonWidgetNode*)param;
		break;
	}
}

void crJXJDoCounterinsurgencyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strSwitch = str;
		break;
	case 2:
		m_bIsPiLiang = atoi(str.c_str());
		break;
	case 3:
		m_strProgress = str;
		break;
	default:
		break;
	}
}

void crJXJDoCounterinsurgencyMethod::operator()(crHandle &handle)
{
	crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
	if(myPlayer)
	{
		void *param;
		bool isCanCounterinsurgency = false;
		crTableIO::StrVec record;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crCanvasNode>  cityMapCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
		crData *canvasData = NULL;
		if(cityMapCanvas.valid())
			canvasData = cityMapCanvas->getDataClass();

		crData *myPlayerData = myPlayer->getDataClass();
		myPlayerData->getParam(WCHDATA_JXJVipLv,param);
		unsigned char vipLevel = *(unsigned char *)param;
		unsigned char count = 1;
		if(m_bIsPiLiang)
		{
			count = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyCount, vipLevel).c_str());
		}

		ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
		ref_ptr<crMultiSwitch>  mulitSwitch = NULL;
		ref_ptr<crProgressWidgetNode>  progress = NULL;
		if(parentCanvas.valid())
		{
			mulitSwitch = dynamic_cast<crMultiSwitch *>(parentCanvas->getChildNode(m_strSwitch));
			progress = dynamic_cast<crProgressWidgetNode *>(parentCanvas->getWidget(m_strProgress));
			if(progress.valid()) progress->setProgress(0.0f);
			//设置倒计时进度条
		}
		if(canvasData)
		{
			//判断免费次数是否充足
			canvasData->getParam(WCHDATA_JXJChooseRebelCityID,param);
			unsigned short chengchiId = *(unsigned short *)param;
			if(mulitSwitch.valid()) mulitSwitch->setActiveSwitchSet(0);

			float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval,vipLevel).c_str());
			canvasData->inputParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,&turnTimer);

			crData *braindata = crBrain::getInstance()->getDataClass();
			if (braindata)
			{
				unsigned char totalTime = count * (int)turnTimer;
				braindata->inputParam(WCHDATA_JXJBatchRebelCount,&totalTime);
			}

			stream->createBuf(3);
			stream->_writeUChar(count);
			stream->_writeUShort(chengchiId);
			crPlayerDataEventPacket packet;
			crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvCounterinsurgency,stream.get());
			netConductor->getNetManager()->sendPacket("all",packet);

			canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&count);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvCounterinsurgencyMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvCounterinsurgencyMethod::crJXJRecvCounterinsurgencyMethod(){}
crJXJRecvCounterinsurgencyMethod::crJXJRecvCounterinsurgencyMethod(const crJXJRecvCounterinsurgencyMethod& handle):
	crMethod(handle)
{
}
void crJXJRecvCounterinsurgencyMethod::inputParam(int i, void *param)
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

void crJXJRecvCounterinsurgencyMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_canvas = str;
		break;
	case 1:
		m_strCanvas = str;
		break;
	case 2:
		m_strSwitch = str;
		break;
	case 3:
		m_strProgress = str;
		break;
	case 4:
		m_strEnterCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJRecvCounterinsurgencyMethod::operator()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		char success = 0;
		crTableIO::StrVec record;
		ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
		ref_ptr<crPlayerGameData> playerGameData;
		ref_ptr<crData> playerData;
		if(m_netType == GameServer)
		{
			unsigned char count = m_stream->_readUChar();
			unsigned short chengchiID = m_stream->_readUShort();
			int playerid = m_this->getPlayerID();
			crServerBrainHandle * gameServerBrain = crServerBrainHandle::getInstance();
			if(gameServerBrain)
			{
				crData *gsBrainData = gameServerBrain->getDataClass();
				if(gsBrainData)
				{
					unsigned short countertime = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, 0).c_str());
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,1));
					gsBrainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
					JXJCounterMap *Counterplayermap = (JXJCounterMap *)param;
					for(JXJCounterMap::iterator itr = Counterplayermap->begin();
						itr != Counterplayermap->end();
						++itr)
					{
						for (JXJCounterVec::iterator vitr = itr->second.begin();
							vitr != itr->second.end();
							++vitr)
						{
							if (playerid == (*vitr)->getPlayerID())
							{
								//(*vitr)->setState(FubenRushType_Rushing);
								success = -1;
							}
						}
					}
					if (success >= 0)
					{
						gsBrainData->getParam(WCHDATA_JXJOpenRebel,param);
						bool isOpen = *(bool *)param;
						if (!isOpen)
						{
							success = -2;
						}
					}
					if (success >= 0)
					{
						ref_ptr<crCounterPlayerInfo> playerinfo = new crCounterPlayerInfo;
						playerinfo->setChengChiID(chengchiID);
						playerinfo->setRemainCount(count);
						playerinfo->setTotalCount(count);
						//playerinfo->setState(FubenRushType_Rushing);
						playerinfo->setPlayerID(playerid);
						(*Counterplayermap)[countertime].push_back(playerinfo);
					}
					gsBrainData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
			///游戏日志
			std::string logdata = "平叛，（城池ID，次数）：" + crArgumentParser::appVectoa(crVector2i(chengchiID,count));
			GameLogData gamelog(Log_KillRebel,logdata);
			crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerid,&gamelog));
		}
		else if(m_netType == GameClient_Game)
		{
			ref_ptr<crCanvasNode>  cityCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
			ref_ptr<crCanvasNode>  rebelCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
			ref_ptr<crCanvasNode>  enterCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strEnterCanvas);
			crData * playerData = m_this->getDataClass();
			if(playerData && rebelCanvas.valid()&& cityCanvas.valid())
			{
				playerData->getParam(WCHDATA_JXJVipLv,param);
				unsigned char vipLevel = *(unsigned char *)param;
				unsigned char count = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyCount, vipLevel).c_str());
				ref_ptr<crMultiSwitch>  crswitch = dynamic_cast<crMultiSwitch *>(rebelCanvas->getChildNode(m_strSwitch));
				ref_ptr<crProgressWidgetNode>  progress = dynamic_cast<crProgressWidgetNode *>(rebelCanvas->getWidget(m_strProgress));
				crData *canvasData = cityCanvas->getDataClass();
				playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
				unsigned char *freeCountCanUse = (unsigned char *)param;
				playerData->getParam(WCHDATA_JXJJunling,param);
				unsigned short * junling = (unsigned short *)param;

				unsigned char success = m_stream->_readUChar();
				if(success == 1)
				{
					unsigned char freeCount = m_stream->_readUChar();
					unsigned char lastTotalCount = m_stream->_readUChar();

					if(canvasData)
					{
						canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);
					}
					if(*freeCountCanUse>0)
					{
						(*freeCountCanUse)--;
						rebelCanvas->doEvent(WCH_UI_UpdateData);
					}

					crData *braindata = crBrain::getInstance()->getDataClass();
					if (braindata)
					{
						float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval,vipLevel).c_str());
						unsigned char totalTime = lastTotalCount * (int)turnTimer;
						braindata->inputParam(WCHDATA_JXJBatchRebelCount,&totalTime);
					}

					if(lastTotalCount == 0)
					{
						//结束
						if(crswitch.valid())
							crswitch->setActiveSwitchSet(1);
						//crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);
					}
				//	else
				//	{
				//		if(progress)
				//			progress->setProgress((count-lastTotalCount)/count);
				//	}
				}
				else if(success == 2)
				{
					*junling = m_stream->_readUShort();

					unsigned char lastTotalCount = m_stream->_readUChar();

					if(canvasData)
					{
						canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);
					}

					crData *braindata = crBrain::getInstance()->getDataClass();
					if (braindata)
					{
						float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval,vipLevel).c_str());
						unsigned char totalTime = lastTotalCount * (int)turnTimer;
						braindata->inputParam(WCHDATA_JXJBatchRebelCount,&totalTime);
					}
					if(lastTotalCount == 0)
					{
						//结束
						if(crswitch.valid())
							crswitch->setActiveSwitchSet(1);
						///crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);
					}
					else
					{
						float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());
						canvasData->inputParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,&turnTimer);
						//unsigned short *turntimer = (unsigned short *)param;
					}
				}
				else if(success == 0)
				{
					//失败
					unsigned char lastTotalCount = m_stream->_readUChar();
					crFilterRenderManager::getInstance()->closeCanvas(rebelCanvas.get());
					//提示第几轮扫荡失败。。。。。
					crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
					int texid = 3004;
					noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
					int mode = 1;
					noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
					std::string param1 = crArgumentParser::appItoa(lastTotalCount);
					noticeHandle->inputParam(WCHDATA_NoticeParam1,&param1);
					crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
					if(enterCanvas.valid())
						crFilterRenderManager::getInstance()->closeCanvas(enterCanvas.get());
					crFilterRenderManager::getInstance()->closeCanvas(rebelCanvas.get());

					lastTotalCount = 0;
					if(canvasData)
						canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);

					crData *braindata = crBrain::getInstance()->getDataClass();
					if (braindata)
					{
						braindata->inputParam(WCHDATA_JXJBatchRebelCount,NULL);
					}
				}
				else if(success == 3)
				{
					//平叛时间结束
					int index = 3003;
					m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(index,NULL));

					if(enterCanvas.valid())
						crFilterRenderManager::getInstance()->closeCanvas(enterCanvas.get());
					crFilterRenderManager::getInstance()->closeCanvas(rebelCanvas.get());

					unsigned char lastTotalCount = 0;
					if(canvasData)
						canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);

					crData *braindata = crBrain::getInstance()->getDataClass();
					if (braindata)
					{
						braindata->inputParam(WCHDATA_JXJBatchRebelCount,NULL);
					}
				}
				playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}
	//if(m_this && m_stream.valid())
	//{
	//	void *param;
	//	crTableIO::StrVec record;
	//	ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
	//	ref_ptr<crPlayerGameData> playerGameData;
	//	ref_ptr<crData> playerData;
	//	if(m_netType == GameServer)
	//	{
	//		unsigned char count = m_stream->_readUChar();
	//		unsigned short chengchiID = m_stream->_readUShort();
	//		int playerID = m_this->getPlayerID();
	//		crServerBrainHandle * gameServerBrain = crServerBrainHandle::getInstance();
	//		if(gameServerBrain)
	//		{
	//			crData *brainData = gameServerBrain->getDataClass();
	//			if(brainData)
	//			{
	//				brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
	//				brainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
	//				CounterInfoMap *counterinsurgencyMap = (CounterInfoMap *)param;
	//				ref_ptr<crJXJCounterinsurgencyInfo> counterInfo = new crJXJCounterinsurgencyInfo;
	//				
	//				counterInfo->getChengChiID()=chengchiID;
	//				counterInfo->getTotalCount() = count;
	//				counterInfo->getTimes() = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, 0).c_str());

	//				counterinsurgencyMap->insert(make_pair(playerID,counterInfo.get()));

	//				brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
	//			}
	//		}
	//		///游戏日志
	//		std::string logdata = "平叛，（城池ID，次数）：" + crArgumentParser::appVectoa(crVector2i(chengchiID,count));
	//		GameLogData gamelog(Log_KillRebel,logdata);
	//		crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerID,&gamelog));
	//	}
	//	else if(m_netType == GameClient_Game)
	//	{
	//		ref_ptr<crCanvasNode>cityCanvas = crFilterRenderManager::getInstance()->findCanvas(m_canvas);
	//		crCanvasNode *rebelCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	//		crCanvasNode *enterCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strEnterCanvas);
	//		crData * playerData = m_this->getDataClass();
	//		if(playerData && rebelCanvas && cityCanvas)
	//		{
	//			playerData->getParam(WCHDATA_JXJVipLv,param);
	//			unsigned char vipLevel = *(unsigned char *)param;
	//			unsigned char count = (unsigned char)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyCount, vipLevel).c_str());
	//			crMultiSwitch *crswitch = dynamic_cast<crMultiSwitch *>(rebelCanvas->getChildNode(m_strSwitch));
	//			CRUI::crProgressWidgetNode *progress = dynamic_cast<crProgressWidgetNode *>(rebelCanvas->getWidget(m_strProgress));
	//			crData *canvasData = cityCanvas->getDataClass();
	//			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
	//			playerData->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
	//			unsigned char *freeCountCanUse = (unsigned char *)param;
	//			playerData->getParam(WCHDATA_JXJJunling,param);
	//			unsigned char * junling = (unsigned char *)param;

	//			unsigned char success = m_stream->_readUChar();
	//			if(success == 1)
	//			{
	//				unsigned char freeCount = m_stream->_readUChar();
	//				unsigned char lastTotalCount = m_stream->_readUChar();

	//				if(canvasData)
	//					canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);

	//				if(*freeCountCanUse>0)
	//				{
	//					(*freeCountCanUse)--;
	//					rebelCanvas->doEvent(WCH_UI_UpdateData);
	//				}

	//				
	//				if(lastTotalCount == 0)
	//				{
	//					//结束
	//					if(crswitch)
	//						crswitch->setActiveSwitchSet(1);
	//					//crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);
	//				}
	//				else
	//				{
	//					if(progress)
	//						progress->setProgress((count-lastTotalCount)/count);
	//				}
	//			}
	//			else if(success == 2)
	//			{
	//				*junling = m_stream->_readUChar();
	//				
	//				unsigned char lastTotalCount = m_stream->_readUChar();

	//				if(canvasData)
	//					canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);

	//				if(lastTotalCount == 0)
	//				{
	//					//结束
	//					if(crswitch)
	//						crswitch->setActiveSwitchSet(1);
	//					//crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);
	//				}
	//				else
	//				{
	//					float turnTimer = (float)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, vipLevel).c_str());
	//					canvasData->inputParam(WCHDATA_JXJPerCounterinsurgencyTurnTimer,&turnTimer);
	//					//unsigned short *turntimer = (unsigned short *)param;
	//				}
	//			}
	//			else if(success == 0)
	//			{
	//				//失败
	//				unsigned char lastTotalCount = m_stream->_readUChar();
	//				crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);
	//				//提示第几轮扫荡失败。。。。。
	//				crHandle *noticeHandle = crGlobalHandle::getInstance()->getDataClass()->getHandle(WCH_GlobalNotice);
	//				int texid = 3004;
	//				noticeHandle->inputParam(WCHDATA_NoticeTextID,&texid);
	//				int mode = 1;
	//				noticeHandle->inputParam(WCHDATA_NoticeMode,&mode);
	//				noticeHandle->inputParam(WCHDATA_NoticeParam1,&lastTotalCount);
	//				crGlobalHandle::getInstance()->doEvent(WCH_GlobalNotice);
	//				if(enterCanvas)
	//					crFilterRenderManager::getInstance()->showCanvas(enterCanvas,false);
	//				crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);

	//				lastTotalCount = 0;
	//				if(canvasData)
	//					canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);

	//			}
	//			else if(success == 3)
	//			{
	//				//平叛时间结束
	//				int index = 3003;
	//				m_this->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(index,NULL));

	//				if(enterCanvas)
	//					crFilterRenderManager::getInstance()->showCanvas(enterCanvas,false);
	//				crFilterRenderManager::getInstance()->showCanvas(rebelCanvas,false);

	//				unsigned char lastTotalCount = 0;
	//				if(canvasData)
	//					canvasData->inputParam(WCHDATA_JXJBatchRebelCount,&lastTotalCount);
	//			}
	//			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
	//		}
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCounterinsurgencyDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJCounterinsurgencyDisposeMethod::crJXJCounterinsurgencyDisposeMethod(){}
crJXJCounterinsurgencyDisposeMethod::crJXJCounterinsurgencyDisposeMethod(const crJXJCounterinsurgencyDisposeMethod& handle):
	crMethod(handle)
{
}
void crJXJCounterinsurgencyDisposeMethod::inputParam(int i, void *param)
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

void crJXJCounterinsurgencyDisposeMethod::addParam(int i, const std::string& str)
{
}

void crJXJCounterinsurgencyDisposeMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		crData *brainData = m_this->getDataClass();
		ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
		ref_ptr<crPlayerGameData> playerGameData;
		ref_ptr<crData> playerData;
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		unsigned short turnTimer = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, 0).c_str());
		crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
		if(brainData && chengchiTab.valid())
		{
			brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
			//brainData->getParam(WCHDATA_JXJOpenRebel,param);
			//bool isOpen = *(bool *)param;
			brainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
			JXJCounterMap *counterinsurgencyMap = (JXJCounterMap *)param;
			brainData->getParam(WCHDATA_JXJRebelCityMap,param);
			RebelCityMap *rebelCityMap = (RebelCityMap *)param;

			JXJCounterVec *rushVec = NULL;
			if(!counterinsurgencyMap->empty())
			{
				rushVec = &((*counterinsurgencyMap)[0]);
			}
			brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
			if(/*isOpen &&*/rushVec && !rushVec->empty())
			{
				int junlingIndex = chengchiTab->getTitleIndex("平叛消耗军令");
				int copperIndex = chengchiTab->getTitleIndex("平叛铜钱奖励");
				int itemIndex = chengchiTab->getTitleIndex("平叛掉落id");
				ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
				ref_ptr<crBagItemData> itemdata = new crBagItemData;
				int copperCash = 0;
				int itemId = 0;
				crVector3i itemVec;
				unsigned char totalCount = 0;
				unsigned char count = 0;
				unsigned char remaincount = 0;
				int _playerid = 0;
				for(JXJCounterVec::iterator itr_info = rushVec->begin()
					;itr_info!=rushVec->end();)
				{
					remaincount = (*itr_info)->getRemainCount();
					_playerid = (*itr_info)->getPlayerID();
					if(_playerid<=0 || (*itr_info)->getTotalCount()==0 || remaincount==0)
					{
						itr_info = rushVec->erase(itr_info);
						continue;
					}

					playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(_playerid));
					if(playerGameServerPlayerData.valid())
					{
						playerGameData = playerGameServerPlayerData->getPlayerGameData();
						if(playerGameData.valid() && playerGameData->getDataClass())
						{
							playerData = playerGameData->getDataClass();
							playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
							playerData->getParam(WCHDATA_JXJJunling,param);
							unsigned short * junling = (unsigned short *)param;
							playerData->getParam(WCHDATA_JXJShiliID,param);
							unsigned char shiliID = *(unsigned char *)param;
							playerData->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
							unsigned char * freeCount = (unsigned char *)param;

							set<unsigned short>::iterator itr_chengchi = (*rebelCityMap)[shiliID].find((*itr_info)->getChengChiID());
							if(itr_chengchi!=(*rebelCityMap)[shiliID].end())
							{
								if(*freeCount>0)
								{
									remaincount -= 1;
									(*itr_info)->setRemainCount(remaincount);
									(*freeCount)--;
									count = remaincount;
									stream->createBuf(3);
									stream->_writeUChar(1);
									stream->_writeUChar(*freeCount);
									stream->_writeUChar(count);
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,_playerid,WCH_JXJRecvCounterinsurgency,stream.get());
									netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

									//奖励
									if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa((*itr_info)->getChengChiID()),record)>=0)
									{
										if(copperIndex>=0) 
										{
											copperCash = atoi(record[copperIndex].c_str());
											reward_info->setCoopercash(copperCash);
										}
										if(itemIndex>=0)
										{
											crArgumentParser::appAtoVec(record[itemIndex].c_str(),itemVec);
											itemdata->setItemID(itemVec[0]);
											itemdata->setEquipMagic(itemVec[1]);
											itemdata->setItemCount(itemVec[2]);
											reward_info->getRewardItemsVec().push_back(itemdata.get());
										}
										reward_info->setPlayerID(_playerid);
										reward_info->setType(GP_RewardPingpan);
										playerGameData->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
									}
								}
								else if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa((*itr_info)->getChengChiID()),record)>=0)
								{
									int needJunling = atoi(record[junlingIndex].c_str());
									needJunling *= 100;
									if(needJunling<= *junling)
									{
										remaincount -= 1;
										//(*itr_info)->setTotalCount(remaincount);
										(*itr_info)->setRemainCount(remaincount);
										*junling-=needJunling;
										count = remaincount;
										stream->createBuf(4);
										stream->_writeUChar(2);
										stream->_writeUShort(*junling);
										stream->_writeUChar(count);

										crPlayerDataEventPacket packet;
										crPlayerDataEventPacket::buildReplyPacket(packet,_playerid,WCH_JXJRecvCounterinsurgency,stream.get());
										netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

										//奖励
										if(copperIndex>=0) 
										{
											copperCash = atoi(record[copperIndex].c_str());
											reward_info->setCoopercash(copperCash);
										}
										if(itemIndex>=0)
										{
											crArgumentParser::appAtoVec(record[itemIndex].c_str(),itemVec);
											itemdata->setItemID(itemVec[0]);
											itemdata->setEquipMagic(itemVec[1]);
											itemdata->setItemCount(itemVec[2]);
											reward_info->getRewardItemsVec().push_back(itemdata.get());
										}
										reward_info->setPlayerID(_playerid);
										reward_info->setType(GP_RewardPingpan);
										playerGameData->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
									}
									else
									{
										stream->createBuf(2);
										stream->_writeUChar(0);
										stream->_writeUChar(totalCount);
										crPlayerDataEventPacket packet;
										crPlayerDataEventPacket::buildReplyPacket(packet,_playerid,WCH_JXJRecvCounterinsurgency,stream.get());
										netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
									}
								}
							}
							playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
						}
					}
					++itr_info;
				}
			}
			//else
			//{
			//	//平叛结束
			//}
			//交换map
			brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
			if(!counterinsurgencyMap->empty())
			{
				for (int i = 0; i < counterinsurgencyMap->size() - 1; i++)
				{
					(*counterinsurgencyMap)[i].swap((*counterinsurgencyMap)[i + 1]);
				}
			}
			brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
		}
	}
	//if(m_this)
	//{
	//	void *param;
	//	crTableIO::StrVec record;
	//	crData *brainData = m_this->getDataClass();
	//	ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
	//	ref_ptr<crPlayerGameData> playerGameData;
	//	ref_ptr<crData> playerData;
	//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
	//	ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
	//	unsigned short turnTimer = (unsigned short)atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCounterinsurgencyInterval, 0).c_str());
	//	crNetConductor *netConductor = crNetContainer::getInstance()->getNetConductor(GameServer);
	//	if(brainData && chengchiTab)
	//	{
	//		brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
	//		brainData->getParam(WCHDATA_JXJOpenRebel,param);
	//		bool isOpen = *(bool *)param;
	//		brainData->getParam(WCHDATA_JXJCounterinsurgencyInfoMap,param);
	//		CounterInfoMap *counterinsurgencyMap = (CounterInfoMap *)param;
	//		brainData->getParam(WCHDATA_JXJRebelCityMap,param);
	//		RebelCityMap *rebelCityMap = (RebelCityMap *)param;

	//		if(isOpen)
	//		{
	//			int junlingIndex = chengchiTab->getTitleIndex("平叛消耗军令");
	//			int copperIndex = chengchiTab->getTitleIndex("平叛铜钱奖励");
	//			int itemIndex = chengchiTab->getTitleIndex("平叛掉落id");
	//			ref_ptr<crPlayerRewardData> reward_info = new crPlayerRewardData;
	//			ref_ptr<crBagItemData> itemdata = new crBagItemData;
	//			int copperCash = 0;
	//			int itemId = 0;
	//			crVector3i itemVec;
	//			unsigned char totalCount = 0;
	//			unsigned char count = 0;
	//			for(CounterInfoMap::iterator itr_info = counterinsurgencyMap->begin()	\
	//				;itr_info!=counterinsurgencyMap->end();)
	//			{
	//				if(itr_info->first<=0 || itr_info->second->getTotalCount()==0)
	//				{
	//					itr_info = counterinsurgencyMap->erase(itr_info);
	//					continue;
	//				}

	//				playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(netConductor->getNetDataManager()->getPlayerData(itr_info->first));
	//				if(playerGameServerPlayerData.valid())
	//				{
	//					playerGameData = playerGameServerPlayerData->getPlayerGameData();
	//					if(playerGameData.valid() && playerGameData->getDataClass())
	//					{
	//						playerData = playerGameData->getDataClass();
	//						playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
	//						playerData->getParam(WCHDATA_JXJJunling,param);
	//						unsigned char * junling = (unsigned char *)param;
	//						playerData->getParam(WCHDATA_JXJShiliID,param);
	//						unsigned char shiliID = *(unsigned char *)param;
	//						playerData->getParam(WCHDATA_JXJFreeCounterinsurgencyCount,param);
	//						unsigned char * freeCount = (unsigned char *)param;

	//						set<unsigned short>::iterator itr_chengchi = (*rebelCityMap)[shiliID].find(itr_info->second->getChengChiID());
	//						if(itr_chengchi!=(*rebelCityMap)[shiliID].end())
	//						{
	//							if(itr_info->second->getTimes()>0)
	//							{
	//								itr_info->second->getTimes()-=1;
	//								++itr_info;
	//								continue;
	//							}

	//							if(*freeCount>0)
	//							{
	//								itr_info->second->getTotalCount()-=1;
	//								itr_info->second->getTimes() = turnTimer;
	//								(*freeCount)--;
	//								count = itr_info->second->getTotalCount();
	//								stream->createBuf(3);
	//								stream->_writeUChar(1);
	//								stream->_writeUChar(*freeCount);
	//								stream->_writeUChar(count);
	//								crPlayerDataEventPacket packet;
	//								crPlayerDataEventPacket::buildReplyPacket(packet,itr_info->first,WCH_JXJRecvCounterinsurgency,stream.get());
	//								netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

	//								//奖励
	//								if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(itr_info->second->getChengChiID()),record))
	//								{
	//									if(copperIndex>=0) 
	//									{
	//										copperCash = atoi(record[copperIndex].c_str());
	//										reward_info->setCoopercash(copperCash);
	//									}
	//									if(itemIndex>=0)
	//									{
	//										crArgumentParser::appAtoVec(record[itemIndex].c_str(),itemVec);
	//										itemdata->setItemID(itemVec[0]);
	//										itemdata->setEquipMagic(itemVec[1]);
	//										itemdata->setItemCount(itemVec[2]);
	//										reward_info->getRewardItemsVec().push_back(itemdata.get());
	//									}
	//									reward_info->setPlayerID(itr_info->first);
	//									reward_info->setType(GP_RewardGameEnd);
	//									playerGameData->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
	//								}

	//								++itr_info;
	//								continue;
	//							}

	//							if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(itr_info->second->getChengChiID()),record))
	//							{
	//								unsigned char needJunling = (unsigned char )atoi(record[junlingIndex].c_str());
	//								if(needJunling<= *junling)
	//								{
	//									itr_info->second->getTotalCount()-=1;
	//									*junling-=needJunling;
	//									itr_info->second->getTimes() = turnTimer;
	//									count = itr_info->second->getTotalCount();
	//									stream->createBuf(3);
	//									stream->_writeUChar(2);
	//									stream->_writeUChar(*junling);
	//									stream->_writeUChar(count);

	//									crPlayerDataEventPacket packet;
	//									crPlayerDataEventPacket::buildReplyPacket(packet,itr_info->first,WCH_JXJRecvCounterinsurgency,stream.get());
	//									netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

	//									//奖励
	//									if(copperIndex>=0) 
	//									{
	//										copperCash = atoi(record[copperIndex].c_str());
	//										reward_info->setCoopercash(copperCash);
	//									}
	//									if(itemIndex>=0)
	//									{
	//										crArgumentParser::appAtoVec(record[itemIndex].c_str(),itemVec);
	//										itemdata->setItemID(itemVec[0]);
	//										itemdata->setEquipMagic(itemVec[1]);
	//										itemdata->setItemCount(itemVec[2]);
	//										reward_info->getRewardItemsVec().push_back(itemdata.get());
	//									}
	//									reward_info->setPlayerID(itr_info->first);
	//									reward_info->setType(GP_RewardGameEnd);
	//									playerGameData->doEvent(WCH_JXJRecvPlayerRewardInfo,MAKECREPARAM(reward_info.get(),NULL));
	//								}
	//								else
	//								{
	//									stream->createBuf(2);
	//									stream->_writeUChar(0);
	//									stream->_writeUChar(totalCount);
	//									crPlayerDataEventPacket packet;
	//									crPlayerDataEventPacket::buildReplyPacket(packet,itr_info->first,WCH_JXJRecvCounterinsurgency,stream.get());
	//									netConductor->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
	//								}
	//								++itr_info;
	//								continue;
	//							}
	//						}
	//						playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
	//					}
	//				}

	//				++itr_info;
	//			}
	//		}
	//		else
	//		{
	//			//平叛结束
	//		}
	//		brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCounterinsurgencyDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRebelCityTimerUpdateMethod::crJXJRebelCityTimerUpdateMethod(){}
crJXJRebelCityTimerUpdateMethod::crJXJRebelCityTimerUpdateMethod(const crJXJRebelCityTimerUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelCityTimerUpdateMethod::inputParam(int i, void *param)
{
}

void crJXJRebelCityTimerUpdateMethod::addParam(int i, const std::string& str)
{
}

void crJXJRebelCityTimerUpdateMethod::operator()(crHandle &handle)
{
	void *param;
	crMyPlayerData *mydata = crMyPlayerData::getInstance();
	crPlayerGameData *gameData = mydata->getPlayerGameData();
	crData* data = gameData->getDataClass();
	if(data)
	{
		data->excHandle(MAKECREPARAM(WCH_LockData,1));
		data->getParam(WCHDATA_JXJRebelTimer,param);
		unsigned short *timer = (unsigned short *)param;
		if(*timer>0)
			(*timer)--;
		data->excHandle(MAKECREPARAM(WCH_LockData,0));
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJCounterinsurgencyDisposeMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRebelCityIconUpdateMethod::crJXJRebelCityIconUpdateMethod(){}
crJXJRebelCityIconUpdateMethod::crJXJRebelCityIconUpdateMethod(const crJXJRebelCityIconUpdateMethod& handle):
	crMethod(handle)
{
}
void crJXJRebelCityIconUpdateMethod::inputParam(int i, void *param)
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

void crJXJRebelCityIconUpdateMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strIcon = str;
		break;
	default:
		break;
	}
}

void crJXJRebelCityIconUpdateMethod::operator()(crHandle &handle)
{
	if(m_this)
	{
		void *param;
		crTableIO::StrVec record;
		ref_ptr<crWidgetNode> cpImageNode;
		ref_ptr<crImageBoxWidgetNode>  imageBox = NULL;
		ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
		crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
		if(playerGameData)
		{
			crData *playerData = playerGameData->getDataClass();
			crData *canvasData = m_this->getDataClass();
			if(playerData && canvasData)
			{
				ref_ptr<crWidgetNode>  rebelNode = m_this->getWidget(m_strIcon);
				rebelNode->setVisiable(false);
				int iconIndex = chengchiTab->getTitleIndex("叛乱兵种icon");
				int posid = chengchiTab->getTitleIndex("坐标");
				int btNameIndex = chengchiTab->getTitleIndex("buttonname");

				canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
				canvasData->getParam(WCHDATA_JXJRandomRebelCityNodeVec,param);
				RebelWidgetVec *rebelCityVec = (RebelWidgetVec *)param;

				playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
				playerData->getParam(WCHDATA_JXJRebelCityVec,param);
				RebelCityVec *rebelCityIdVec = (RebelCityVec *)param;

				int index = 0;
				for (RebelCityVec::iterator itr_id = rebelCityIdVec->begin();itr_id!=rebelCityIdVec->end();itr_id++,index++)
				{
					if(index<rebelCityVec->size())
					{
						cpImageNode = (*rebelCityVec)[index].get();
						cpImageNode->setVisiable(false);
					}
					else
					{
						cpImageNode = dynamic_cast<crWidgetNode *>(rebelNode->clone(crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_DATAS));
						cpImageNode->setEnableMatrix(true);
						cpImageNode->setCanFocus(false);
						cpImageNode->setVisiable(false);
						crLoadManager::getInstance()->requestAddNode(m_this,cpImageNode.get(),false);
						//crLoadManager::getInstance()->requestAddNode(crFilterRenderManager::getInstance()->getCanvasRoot(),cpImageNode.get(),false);
						rebelCityVec->push_back(cpImageNode);
					}
					if(cpImageNode.valid())
					{
						imageBox = dynamic_cast<crImageBoxWidgetNode *>(cpImageNode.get());
						if(imageBox.valid() && chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(*itr_id),record)>=0)
						{
							if(iconIndex>=0)
								imageBox->setImageName(record[iconIndex]);
							crRadioWidgetNode *buttonname = dynamic_cast<crRadioWidgetNode *>(m_this->getWidget(record[btNameIndex]));
							if(buttonname)
							{
								cpImageNode->setVisiable(true);
								crVector3 centerCoord = buttonname->getBound().center();
								imageBox->setPosition(centerCoord);
							}
						}
					}
				}

				playerData->excHandle(MAKECREPARAM(WCH_LockData,0));

				canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
			}
		}
	}	
}

/////////////////////////////////////////
//
//crJXJBingzhengBirthPointChangeMethod
//
/////////////////////////////////////////
crJXJBingzhengBirthPointChangeMethod::crJXJBingzhengBirthPointChangeMethod():
	m_this(NULL),
	m_fireItem(NULL),
	m_fubenprogress(0)
	//m_battleClosed(false)
	{}
crJXJBingzhengBirthPointChangeMethod::crJXJBingzhengBirthPointChangeMethod(const crJXJBingzhengBirthPointChangeMethod& handle):
	crMethod(handle),
	m_this(NULL),
	m_fireItem(NULL),
	m_fubenprogress(handle.m_fubenprogress)
	//m_battleClosed(false)
{
}
void crJXJBingzhengBirthPointChangeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_fireItem = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_fireItem = (crInstanceItem *)(LOCREPARAM(param64));
		}
		else
		{
			m_fireItem = NULL;
		}
		break;
	}
}

void crJXJBingzhengBirthPointChangeMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_fubenprogress = atoi(str.c_str());
		break;
	}
}

void crJXJBingzhengBirthPointChangeMethod::operator()(crHandle &handle)
{
	if(m_this->getID()>0)
	{
		void *param;
		//crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		//crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		//crNetManager *netManager = sceneServerConductor->getNetManager();
		//crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		crRoom *room = m_this->getRoom();// netCallback->findRoom(m_this->getRoomID());
		if(room)
		{
			crData *roomData = room->getDataClass();
			if(roomData && m_fireItem.valid() && m_fireItem->getDataClass())
			{
				ref_ptr<crData> data = m_fireItem->getDataClass();
				crData *thisData = m_this->getDataClass();

				data->getParam(WCHDATA_Camp,param);
				unsigned char camp = *(unsigned char *)param;
				thisData->getParam(WCHDATA_Camp,param);
				unsigned char thiscamp = *(unsigned char *)param;
				crVector3i pos;
				pos[0] = m_this->getPosx();
				pos[1] = m_this->getPosy();
				pos[2] = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJCampBirthRange, 0).c_str());

				roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
				roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
				JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;
				(*campMap)[thiscamp].erase(pos);
				(*campMap)[camp].insert(make_pair(pos,m_this->getIName2()));
				//增加攻防据点数是否为0判断如果为0则改变战场状态为无法进入
				//如果玩家打回则改变战场状态
				roomData->getParam(WCHDATA_JXJRoomType, param);
				unsigned char roomType = *(unsigned char *)param;

				if (roomType == 1)//1.城战2.副本
				{
					roomData->getParam(WCHDATA_JXJBattleID, param);
					int battleid = *(int *)param;
					unsigned char ashili = (unsigned char)(LOINT16(LOINT32(battleid)));
					//int roomid = room->getRoomID();
					//unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
					JXJCampBirthpointMap::iterator itrA = campMap->find(ashili);

					//crJXJChangeBattleStatePacket packet;
					//crNetConductor *gameServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServerClient_Game);

					if (/*m_battleClosed == false && \*/(itrA == campMap->end() || (itrA != campMap->end() && itrA->second.empty())))
					{
						//攻方丢失所有出生点 通知服务器
						//m_battleClosed = true;
						room->timeRemain() = 0.0f;//战斗结束
						//crJXJChangeBattleStatePacket::buildRequestPacket(packet, battleid, roomid, m_battleClosed);
						//if (gameServerConductor)
						//	gameServerConductor->getNetManager()->sendPacket("all", packet);

					}
					//else if (m_battleClosed == true && \
					//	itrA != campMap->end() && !itrA->second.empty())
					//{
					//	//攻防重新打回出生点
					//	m_battleClosed = false;

					//	//crJXJChangeBattleStatePacket::buildRequestPacket(packet, battleid, roomid, m_battleClosed);
					//	//if (gameServerConductor)
					//	//	gameServerConductor->getNetManager()->sendPacket("all", packet);
					//}
				}
				roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
				std::string str = m_this->getIName2();
				ref_ptr<crStreamBuf> stream = new crStreamBuf;
				stream->createBuf(14+str.length());
				stream->_writeUChar(thiscamp);			//1
				stream->_writeUChar(camp);				//1
				stream->_writeVec3(pos);
				stream->_writeString(str);
				crPlayerDataSceneEventPacket packet;
				crPlayerDataSceneEventPacket::buildReplyPacket(packet,0,WCH_JXJRecvCampBirthPointChange,stream.get());
				room->sendPacketToAll(packet);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////
////
////crJXJRecvCampBirthPointChangeMethod
////
////////////////////////////////////////////////////////////////////////////
crJXJRecvCampBirthPointChangeMethod::crJXJRecvCampBirthPointChangeMethod(){}
crJXJRecvCampBirthPointChangeMethod::crJXJRecvCampBirthPointChangeMethod(const crJXJRecvCampBirthPointChangeMethod &handle):
	crMethod(handle),
	m_this(NULL),
	m_stream(NULL)
{

}
void crJXJRecvCampBirthPointChangeMethod::inputParam(int i, void *param)
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

void crJXJRecvCampBirthPointChangeMethod::addParam(int i, const std::string& str)
{
	
}

void crJXJRecvCampBirthPointChangeMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		if(m_netType == GameClient_Game)
		{
			void *param;
			unsigned char camp = m_stream->_readUChar();
			unsigned char firecamp = m_stream->_readUChar();
			crVector3f pos = m_stream->_readVec3();
			std::string iName = m_stream->_readString();
			crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
			if(room)
			{
				crData *roomData = room->getDataClass();
				if(roomData)
				{
					roomData->excHandle(MAKECREPARAM(WCH_LockData,1));
					roomData->getParam(WCHDATA_JXJCampBirthPointMap,param);
					JXJCampBirthpointMap *campMap = (JXJCampBirthpointMap *)param;

					(*campMap)[camp].erase(pos);
					(*campMap)[firecamp].insert(make_pair(pos,iName));
					roomData->excHandle(MAKECREPARAM(WCH_LockData,0));
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJBattleInspireMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJBattleInspireMethod::crJXJBattleInspireMethod():
	m_itemId(0)
{}
crJXJBattleInspireMethod::crJXJBattleInspireMethod(const crJXJBattleInspireMethod &handle):
	crMethod(handle),
	m_itemId(handle.m_itemId)
{

}
void crJXJBattleInspireMethod::inputParam(int i, void *param)
{

}

void crJXJBattleInspireMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_itemId = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJBattleInspireMethod::operator ()(crHandle &handle)
{
	//vip
	void *param;
	char inspireCount = 0;
	bool bCanInspire = false;
	unsigned short itemId = 0;
	crMyPlayerData *me = crMyPlayerData::getInstance();
	if(me)
	{
		crData *playerData = me->getPlayerGameData()->getDataClass();
		if(playerData)
		{
			playerData->getParam(WCHDATA_JXJVipLv,param);
			unsigned char vipLevel = *(unsigned char *)param;
			playerData->getParam(WCHDATA_JXJInspireLevel,param);
			unsigned char inspireLv = *(unsigned char *)param;
			playerData->getParam(WCHDATA_JXJGoldingot,param);
			int goldingot = *(int*)param;
			playerData->getParam(WCHDATA_JXJGiftGold,param);
			int giftgold = *(int*)param;
			goldingot += giftgold;

			//检测背包里现有的鼓舞令数量
			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
			playerData->getParam(WCHDATA_JXJItemBagVec, param);
			JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
			int itemcount = 0;
			for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
				itemitr != itembagvec->end();
				++itemitr)
			{
				if (itemitr->get())
				{
					if (m_itemId == itemitr->get()->getItemID())
					{
						itemcount = (int)itemitr->get()->getItemCount();
						break;
					}
				}
			}
			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));

		
			int canInspireCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireMaxCount, vipLevel).c_str());
			
			if(canInspireCount>0 && inspireLv<canInspireCount )
			{
				int inspireCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireCost, inspireLv+1).c_str());
				if(itemcount >= 1)
				{
					bCanInspire = true;
					itemId = m_itemId; //itemId非0时 表示使用道具鼓舞，否则 表示通过元宝鼓舞
				}
				else if (goldingot >= inspireCost)
				{
					bCanInspire = true;
				}
				else
					me->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(7033,NULL));//鼓舞令和元宝都不足，鼓舞失败
					//me->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(3005,NULL));//鼓舞失败
			}
			else
			{
				me->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(3006,NULL));//鼓舞失败
			}
		}
	}
	if(bCanInspire)
	{
		crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
		ref_ptr<crStreamBuf> stream = new crStreamBuf;
		stream->createBuf(2);
		stream->_writeUShort(itemId);
		crPlayerDataEventPacket packet;
		crPlayerDataEventPacket::buildRequestPacket(packet,WCH_JXJRecvInspire,stream.get());
		if(netConductor)
			netConductor->getNetManager()->sendPacket("all",packet);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJRecvInspireMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJRecvInspireMethod::crJXJRecvInspireMethod(){}
crJXJRecvInspireMethod::crJXJRecvInspireMethod(const crJXJRecvInspireMethod &handle):
	crMethod(handle)
{

}
void crJXJRecvInspireMethod::inputParam(int i, void *param)
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

void crJXJRecvInspireMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strInput = str;
		break;
	case 2:
		m_strBt = str;
		break;
	case 3:
		m_strInput_defend = str;
		break;
	case 4:
		m_strBt_defend = str;
		break;
	default:
		break;
	}
}

void crJXJRecvInspireMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			unsigned short itemId = m_stream->_readUShort();
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
						playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
						playerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						playerData->getParam(WCHDATA_JXJInspireLevel,param);
						unsigned char *inspireLv = (unsigned char *)param;
						int canInspireCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireMaxCount, vipLevel).c_str());
						int inspireCost = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireCost, (*inspireLv+1)).c_str());												
						rangei rndi(0,100);
						int r = rndi.get_random();

						if((*inspireLv)<canInspireCount)
						{
							int inspireSucessRate = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJInspireSucessRate, (*inspireLv+1)).c_str());
							if(r<inspireSucessRate)
							{
								bool itemEnough = false;
								if (itemId != 0)
								{
									//检测背包里现有的鼓舞令数量
									playerData->getParam(WCHDATA_JXJItemBagVec, param);
									JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
									int itemCount = 0;
									for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
										itemitr != itembagvec->end();
										++itemitr)
									{
										if (itemitr->get())
										{
											if (itemId == itemitr->get()->getItemID())
											{
												itemCount = (int)itemitr->get()->getItemCount();
												if (itemCount >= 1)
												{
													itemEnough = true;
													itemitr->get()->setItemCount(--itemCount);
													break;
												}
											}
										}
									}
								}
								else
								{
									playerData->getParam(WCHDATA_JXJGoldingot,param);
									int *goldingot = (int*)param;
									playerData->getParam(WCHDATA_JXJGiftGold,param);
									int *giftgold = (int*)param;
									if (inspireCost <= *goldingot +*giftgold)
									{
										itemEnough = true;
										//扣除元宝
										bool needdeductgold = false;
										MoneyChangeData moneydata(inspireCost,"战场鼓舞");
										m_this->doEvent(WCH_JXJDeductLijinGold,MAKECREPARAM(&moneydata,&needdeductgold));
									}
								}

								if(itemEnough)
								{
									*inspireLv+=1;
									stream->createBuf(8);
									stream->_writeUChar(1);
									stream->_writeUChar(*inspireLv);
									stream->_writeUShort(itemId);
									stream->_writeInt(inspireCost);
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvInspire,stream.get());
									gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);

									streamScene->createBuf(5);
									streamScene->_writeInt(playerID);
									streamScene->_writeChar(*inspireLv);
									crJXJBattleInspirePacket packet_scene;
									crJXJBattleInspirePacket::buildRequestPacket(packet_scene,streamScene.get());
									gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getSceneServerAddress(),packet_scene);
									///游戏日志
									if (itemId != 0)
									{//使用道具鼓舞
										std::string logdata = "使用物品（success，itemid）：" + crArgumentParser::appVectoa(crVector2i(itemEnough,itemId));
										GameLogData gamelog(Log_UseBagItem,logdata);
										crServerBrainHandle::getInstance()->doEvent(WCH_GameLog,MAKECREPARAM(playerID,&gamelog));
									}
								}
								else
								{
									stream->createBuf(1);
									stream->_writeUChar(0);
									crPlayerDataEventPacket packet;
									crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvInspire,stream.get());
									gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
								}
							}
						}
						else
						{
							stream->createBuf(1);
							stream->_writeUChar(0);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJRecvInspire,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
						}
						playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char _case = m_stream->_readUChar();
			unsigned char inspireLv = m_stream->_readUChar();
			unsigned short itemId = m_stream->_readUShort();
			int inspireCost = m_stream->_readInt();
			crMyPlayerData *me = crMyPlayerData::getInstance();
			if(_case == 1)
			{
				crData *playerData = me->getPlayerGameData()->getDataClass();

				playerData->inputParam(WCHDATA_JXJInspireLevel,&inspireLv);
				ref_ptr<crCanvasNode> canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
				if(canvas.valid())
				{
					crRoom *room = crMyPlayerData::getInstance()->getSelectedRoom();
					if (room && room->getDataClass())
					{
						crData *roomData = room->getDataClass();
						roomData->getParam(WCHDATA_JXJBattleID, param);
						int battleid = *(int *)param;
						playerData->getParam(WCHDATA_JXJShiliID, param);
						unsigned char myShili = *(unsigned char *)param;
						unsigned char dshili = (unsigned char)(HIINT16(LOINT32(battleid)));
						ref_ptr<crStaticTextWidgetNode> input = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strInput));
						ref_ptr<crButtonWidgetNode> inspireBt = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strBt));
						if (myShili == dshili)
						{//防守方
							if (input.valid() && inspireBt.valid())
							{//隐藏攻防小鼓舞按钮
								input->setVisiable(false);
								inspireBt->setVisiable(false);
							}
							input = dynamic_cast<crStaticTextWidgetNode *>(canvas->getWidget(m_strInput_defend));
							inspireBt = dynamic_cast<crButtonWidgetNode *>(canvas->getWidget(m_strBt_defend));
						}
						playerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						int canInspireCount = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJBattleInspireMaxCount, vipLevel).c_str());
						if (inspireBt.valid())
						{
							if(canInspireCount<=0 || inspireLv >= canInspireCount)
								inspireBt->setEnable(false);
							else
								inspireBt->setEnable(true);
						}
						if(input.valid())
						{
							input->setString(crArgumentParser::appItoa(inspireLv));
						}
						
						if (itemId != 0)
						{//itemId不为0时才表示 使用道具进行鼓舞，否则是元宝鼓舞
							//扣除背包里现有的鼓舞令数量
							playerData->getParam(WCHDATA_JXJItemBagVec, param);
							JXJItemBagVec *itembagvec = (JXJItemBagVec *)param;
							int itemCount = 0;
							for (JXJItemBagVec::iterator itemitr = itembagvec->begin();
								itemitr != itembagvec->end();
								++itemitr)
							{
								if (itemitr->get())
								{
									if (itemId == itemitr->get()->getItemID())
									{
										itemCount = (int)itemitr->get()->getItemCount();
										if (itemCount > 0)
										{
											itemitr->get()->setItemCount(itemCount-1);
										}
										else
											itemitr->get()->setItemCount(0);
										break;
									}
								}
							}

							JXJItemBagVec deductitembagvec;
							JXJItemBagVec deductcardbagvec;
							deductitembagvec.push_back(NULL);
							deductcardbagvec.push_back(NULL);
							CRCore::ref_ptr<crBagItemData> newitem = new crBagItemData;
							newitem->setItemID((int)itemId);
							newitem->setEquipMagic(0);
							newitem->setItemDeductCount(1);
							deductitembagvec.push_back(newitem);
							//crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeValue,MAKECREPARAM((int)itemId,-1));
							crBrain::getInstance()->doEvent(WCH_JXJClientUserRecvSomeItems,MAKECREPARAM(&deductitembagvec,&deductcardbagvec));
						}
					}
				}
			}
			else
			{
				me->getPlayerGameData()->doEvent(WCH_JXJUIShowTipsCanvas,MAKECREPARAM(3005,NULL));//鼓舞失败
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//crJXJDoAcceptInviteMethod
//////////////////////////////////////////////////////////////////////////
crJXJDoAcceptInviteMethod::crJXJDoAcceptInviteMethod()
{
}
crJXJDoAcceptInviteMethod::crJXJDoAcceptInviteMethod(const crJXJDoAcceptInviteMethod& handle):
	crMethod(handle)
{

}
void crJXJDoAcceptInviteMethod::inputParam(int i, void *param)
{

}
void crJXJDoAcceptInviteMethod::addParam(int i, const std::string& str)
{
}
void crJXJDoAcceptInviteMethod::operator()(crHandle &handle)
{
	crPlayerGameData *player =  crMyPlayerData::getInstance()->getPlayerGameData();
	if(player && crRunGameHandle::getInstance()->isInManor())
	{
		player->doEvent(WCH_JXJAcceptInvite);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJQureyRebelTimerMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJQureyRebelTimerMethod::crJXJQureyRebelTimerMethod(){}
crJXJQureyRebelTimerMethod::crJXJQureyRebelTimerMethod(const crJXJQureyRebelTimerMethod &handle):
	crMethod(handle)
{

}
void crJXJQureyRebelTimerMethod::inputParam(int i, void *param)
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

void crJXJQureyRebelTimerMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strCityMapCanvas = str;
		break;
	default:
		break;
	}
}

void crJXJQureyRebelTimerMethod::operator ()(crHandle &handle)
{
	if(m_this && m_stream.valid())
	{
		void *param;
		if(m_netType == GameServer)
		{
			//crData *data = m_this->getDataClass();
			int playerID = m_this->getPlayerID();
			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
			crNetDataManager *netDataManager = gameServer->getNetDataManager();
			ref_ptr<crGameServerPlayerData> playerGameServerPlayerData;
			ref_ptr<crStreamBuf> stream = new crStreamBuf;
			crData *gsBrainData = crServerBrainHandle::getInstance()->getDataClass();
			if(netDataManager && gsBrainData)
			{
				playerGameServerPlayerData = dynamic_cast<crGameServerPlayerData *>(gameServer->getNetDataManager()->getPlayerData(playerID));
				if(/*data && */playerGameServerPlayerData.valid())
				{
					gsBrainData->getParam(WCHDATA_JXJOpenRebel,param);
					bool openRebel = *(bool *)param;
					if(openRebel)
					{
						gsBrainData->getParam(WCHDATA_JXJPerRebelTimer,param);
						unsigned short perTimer = *(unsigned short *)param;
						if(perTimer>0)
						{
							stream->createBuf(3);
							stream->_writeUChar(1);
							stream->_writeUShort(perTimer);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJQueryRebelTimer,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
						}
						else
						{
							stream->createBuf(1);
							stream->_writeUChar(0);
							crPlayerDataEventPacket packet;
							crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJQueryRebelTimer,stream.get());
							gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
						}
					}
					else
					{
						stream->createBuf(1);
						stream->_writeUChar(0);
						crPlayerDataEventPacket packet;
						crPlayerDataEventPacket::buildReplyPacket(packet,playerID,WCH_JXJQueryRebelTimer,stream.get());
						gameServer->getNetManager()->sendPacket(playerGameServerPlayerData->getPlayerConnectServerAddress(),packet);
					}
				}
			}
		}
		else if(m_netType == GameClient_Game)
		{
			unsigned char _case = m_stream->_readUChar();
			if(_case)
			{
				unsigned short perTimer = m_stream->_readUShort();
				crData *data = m_this->getDataClass();
				if(data)
				{
					data->inputParam(WCHDATA_JXJRebelTimer,&perTimer);
				}
			}
			else
			{
				ref_ptr<crCanvasNode>  canvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
				ref_ptr<crCanvasNode>  cityCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCityMapCanvas);
				if(canvas.valid())
					crFilterRenderManager::getInstance()->closeCanvas(canvas.get());
				if(cityCanvas.valid())
				{
					crData *canvasData = cityCanvas->getDataClass();
					if(canvasData)
					{
						canvasData->excHandle(MAKECREPARAM(WCH_LockData,1));
						canvasData->getParam(WCHDATA_JXJRandomRebelCityNodeVec,param);
						RebelWidgetVec *rebelCityVec = (RebelWidgetVec *)param;
						for (RebelWidgetVec::iterator itr = rebelCityVec->begin();itr!=rebelCityVec->end();itr++)
						{
							itr->get()->setVisiable(false);
						}

						rebelCityVec->resize(0);
						canvasData->excHandle(MAKECREPARAM(WCH_LockData,0));
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJGetRoleFightingPowerMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJGetRoleFightingPowerMethod::crJXJGetRoleFightingPowerMethod(){}
crJXJGetRoleFightingPowerMethod::crJXJGetRoleFightingPowerMethod(const crJXJGetRoleFightingPowerMethod &handle):
	crMethod(handle)
{

}
void crJXJGetRoleFightingPowerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_fightingPower = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_fightingPower = (float *)LOCREPARAM(param64);
			//formationinfo = (crFormationInfo *)HIINT64(param64);
		}
		else
		{
			m_fightingPower = NULL;
			//formationinfo = NULL;
		}
		break;
	}
}

void crJXJGetRoleFightingPowerMethod::addParam(int i, const std::string& str)
{
}

void crJXJGetRoleFightingPowerMethod::operator ()(crHandle &handle)
{
	ref_ptr<crTableIO> itemTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJItemTab);
	if(m_this && itemTab.valid())
	{
		//void *param;
		crTableIO::StrVec record;
		map< unsigned char,float > factorMap;
		crData *itemData = m_this->getDataClass();
		ref_ptr<crTableIO> factorTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJFightingPowerFactorTab);
		int absindex = itemTab->getTitleIndex("abstractid");
		int itemQulityIndex = itemTab->getTitleIndex("品质");
		int cardindex = itemTab->getTitleIndex("将卡类型");
		if(itemData && factorTab.valid())
		{
			int index = factorTab->getTitleIndex("属性");
			int factorIndex = factorTab->getTitleIndex("系数");
			for (unsigned char i = 0;i<FP_SecondaryCard+1;i++)
			{
				if(factorTab->queryOneRecord(0,crArgumentParser::appItoa(i),record)>=0 && factorIndex>=0)
				{
					factorMap[i] = atof(record[factorIndex].c_str());
				}
			}

			float fightingPower = 0.0f;
			float outParam = 0.0f;
			//itemData->excHandle(MAKECREPARAM(WCH_LockData,1));//防止阻塞
			m_this->doEvent(MAKEINT64(WCH_GetCrit,NULL),MAKECREPARAM(&outParam,NULL));
			fightingPower+= outParam*factorMap[FP_Crit];
			m_this->doEvent(WCH_GetDamage,MAKECREPARAM(&outParam,NULL));
			fightingPower+= outParam*factorMap[FP_Attack];
			m_this->doEvent(WCH_GetDefend,MAKECREPARAM(&outParam,NULL));
			fightingPower+= outParam*factorMap[FP_Defend];
			m_this->doEvent(WCH_GetHP,MAKECREPARAM(&outParam,NULL));
			fightingPower += outParam *factorMap[FP_HP];
			m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&outParam,NULL));
			fightingPower += outParam *factorMap[FP_MoveSpeed];
			m_this->doEvent(MAKEINT64(WCH_GetDodge,NULL),MAKECREPARAM(&outParam,NULL));
			fightingPower += outParam * factorMap[FP_Dodge];
			m_this->doEvent(MAKEINT64(WCH_GetParry,NULL),MAKECREPARAM(&outParam,NULL));
			fightingPower += outParam * factorMap[FP_Parry];
			m_this->doEvent(MAKEINT64(WCH_GetSkillCD,1),MAKECREPARAM(&outParam,NULL));
			fightingPower += outParam * factorMap[FP_SkillCD];

			int abstractid = -1;
			abstractid = m_this->getAbstractItemID();
			CREncapsulation::crTableIO::StrVec	recordData,itemrecord;	
			ref_ptr<crTableIO> role_data_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_RoleDataAttrTab);
			std::string attackRange,attackRange2;
			std::string range,fireRateCD,fireRateYC,fireRate;
			if(role_data_table->queryOneRecord(0, crArgumentParser::appItoa(abstractid), recordData) >= 0)
			{
				int attackRangeIndex = role_data_table->getTitleIndex("attackid");
				attackRange = "射程 " + recordData[attackRangeIndex];
				attackRange2 = recordData[attackRangeIndex];
				//SkillDataAttrTab
				float baserange = 0.0f;
				float extra = 0.0f;
				CREncapsulation::crTableIO::StrVec	recordSkill;	
				ref_ptr<crTableIO> role_skill_table = CRNetApp::crGlobalHandle::gData()->gGlobalTable(WCHDATA_SkillDataAttrTab);	// troops data look-up table 
				if(role_skill_table->queryOneRecord(0, attackRange2, recordSkill) >= 0)
				{
					int rangeindex = role_skill_table->getTitleIndex("使用半径");
					int cdindex = role_skill_table->getTitleIndex("CD");
					//if (m_index_1 == 1)
					//{
						baserange = atof(recordSkill[rangeindex].c_str());
						outParam = 0.0f;
						m_this->doEvent(MAKEINT64(WCH_GetAttackDist,NULL),MAKECREPARAM(&outParam,NULL));
						extra = outParam;// -atof(recordSkill[rangeindex].c_str());
						baserange += extra;
						fightingPower += baserange * factorMap[FP_AttackDistance];
						//range = "攻击距离 " + crArgumentParser::appItoa(int(baserange) + int(extra));
						//if(extra>=1.0f)
						//{
						//	range+=std::string("(+")+crArgumentParser::appItoa((int)extra)+std::string(")");
						//}
					//}
					//else
					//{
						//range ="攻击距离 " + recordSkill[rangeindex];
					//}
					float cdtime = atof(recordSkill[cdindex].c_str());
					float cdreduce = 0.0f;
					float attackspeed = 0.0f;
					m_this->doEvent(WCH_GetSkillCD,MAKECREPARAM(&cdreduce,NULL));
					if(cdreduce<=-1.0f)
						cdreduce = -0.99f; 
					cdtime = cdtime/(1.0f+cdreduce);
					attackspeed = 1.0f/cdtime;
					fightingPower += attackspeed * factorMap[FP_AttackSpeed];
				}
			}

			//m_this->doEvent(WCH_GetSkillCD,MAKECREPARAM(&outParam,NULL));
			//fightingPower += outParam * factorMap[FP_AttackSpeed];

			//m_this->doEvent(MAKEINT64(WCH_GetAttackDist, NULL), MAKECREPARAM(&outParam, NULL));
			//fightingPower += outParam * factorMap[FP_AttackDistance];

			m_this->doEvent(MAKEINT64(WCH_GetDefend,1),MAKECREPARAM(&outParam,NULL));
			fightingPower+=outParam*factorMap[FP_DefendSkill];

			//itemData->excHandle(MAKECREPARAM(WCH_LockData,0));
			//将卡战斗力加成
			void *param;
			///2张将卡
			//
			itemData->getParam(WCHDATA_EquipOnMap,param);
			EquipOnMap equipOnMap = *(EquipOnMap *)param;
			EquipOnMap::iterator itr_card = equipOnMap.begin();

			for (;itr_card!=equipOnMap.end();itr_card++)
			{
				if (itemTab->queryOneRecord(absindex,crArgumentParser::appItoa(itr_card->first),record)>=0 && cardindex>=0 && itemQulityIndex>=0)
				{
					if(JiangkaType_ji == atoi(record[cardindex].c_str()))
					{
						fightingPower += atoi(record[itemQulityIndex].c_str()) * factorMap[FP_MainCard];
					}

					if(JiangkaType_mou == atoi(record[cardindex].c_str()))
					{
						fightingPower += atoi(record[itemQulityIndex].c_str()) * factorMap[FP_SecondaryCard];
					}
				}
			}

			itemData->getParam(WCHDATA_JXJRTArmyCount,param);
			/*unsigned short*/int rtarmyCount = *(/*unsigned short*/int *)param;
			if(rtarmyCount > 1000)
				rtarmyCount = 1000;
			fightingPower = fightingPower * rtarmyCount;

			//int itemQulityIndex = itemTab->getTitleIndex("品质");
			//fightingPower+=fightingPower * formationinfo->getCount();
			/*crVector2i equips = formationinfo->getEquips();
			crTableIO::StrVec itemrecord[2];
			if(equips[0]>0)
			{
				if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(equips[0]),itemrecord[0])>=0)
				{
					fightingPower += atoi(itemrecord[0][itemQulityIndex].c_str()) * factorMap[FP_MainCard];
				}
			}
			if(equips[1]>0)
			{
				if(itemTab->queryOneRecord(0,crArgumentParser::appItoa(equips[1]),itemrecord[1])>=0)
				{
					fightingPower += atoi(itemrecord[1][itemQulityIndex].c_str()) * factorMap[FP_SecondaryCard];
				}
			}*/
			*m_fightingPower = ceil(fightingPower * 0.001f);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//
//crJXJGetFightingPowerMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJGetPlayerFightingPowerMethod::crJXJGetPlayerFightingPowerMethod(){}
crJXJGetPlayerFightingPowerMethod::crJXJGetPlayerFightingPowerMethod(const crJXJGetPlayerFightingPowerMethod &handle):
	crMethod(handle)
{

}
void crJXJGetPlayerFightingPowerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_fightingPower = NULL;
		}
		break;
	case 1:
		m_this = (crPlayerGameData *)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_fightingPower = (int *)(LOCREPARAM(param64));
		}
		else
		{
			m_fightingPower = NULL;
		}
		break;
	}
}

void crJXJGetPlayerFightingPowerMethod::addParam(int i, const std::string& str)
{
}

void crJXJGetPlayerFightingPowerMethod::operator ()(crHandle &handle)
{
	void *param;
	crData * playerData = m_this->getDataClass();
	float output = 0.0f,fightPower = 0.0f;
	map< unsigned char,float > factorMap;
	if(playerData)
	{
		playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
		playerData->getParam(WCHDATA_JXJFormationInfoVec,param);
		FormationInfoVec myteam = *(FormationInfoVec *)param;
		playerData->excHandle(MAKECREPARAM(WCH_LockData, 0));

		if (crGlobalHandle::isClient())
		{
			crMyPlayerData::MyRoleNpcMap roleMap = crMyPlayerData::getInstance()->getMyRoleNpcMap();
			int index = 0;
			for (crMyPlayerData::MyRoleNpcMap::iterator itr_role = roleMap.begin(); itr_role != roleMap.end(); itr_role++, index++)
			{
				for (FormationInfoVec::iterator itr = myteam.begin(); itr != myteam.end(); itr++)
				{
					if (itr->get()->getRoleID() == itr_role->first)
					{
						itr_role->second.first->doEvent(WCH_JXJGetRoleFightingPower, MAKECREPARAM(&output, itr->get()));
						fightPower += output;
						break;
					}
				}
			}
		}
		else
		{
			crNetConductor *sceneServer = crNetContainer::getInstance()->getNetConductor(SceneServer);
			if (sceneServer)
			{
				ref_ptr<crSceneServerPlayerData> player = dynamic_cast<crSceneServerPlayerData *>(sceneServer->getNetDataManager()->getPlayerData(m_this->getPlayerID()));
				crSceneServerPlayerData::RoleMap &playerRoleMap = player->getRoleMap();
				for (crSceneServerPlayerData::RoleMap::iterator itr_role = playerRoleMap.begin(); itr_role != playerRoleMap.end(); itr_role++)
				{
					for (FormationInfoVec::iterator itr = myteam.begin(); itr != myteam.end(); itr++)
					{
						if (itr->get()->getRoleID() == itr_role->first)
						{
							itr_role->second->doEvent(WCH_JXJGetRoleFightingPower, MAKECREPARAM(&output, itr->get()));
							fightPower += output;
							break;
						}
					}
				}
			}
		}

		*m_fightingPower = floor(fightPower);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//crJXJFightingPowerShowMethod
//
//////////////////////////////////////////////////////////////////////////
crJXJFightingPowerShowMethod::crJXJFightingPowerShowMethod(){}
crJXJFightingPowerShowMethod::crJXJFightingPowerShowMethod(const crJXJFightingPowerShowMethod &handle):
	crMethod(handle)
{

}
void crJXJFightingPowerShowMethod::inputParam(int i, void *param)
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

void crJXJFightingPowerShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_str_1 = str;
		break;
	case 1:
		m_str_2 = str;
		break;
	case 2:
		m_str_3 = str;
		break;
	case 3:
		m_str_4 = str;
		break;
	case 4:
		m_str_5 = str;
		break;
	case 5:
		m_str_6 = str;
		break;
	case 6:
		m_str_7 = str;
		break;
	}
}

void crJXJFightingPowerShowMethod::operator ()(crHandle &handle)
{
	ref_ptr<crMultiSwitch>  switch_Power[7] = {NULL};
	int output = 0;
	std::string strFightingPower;
	switch_Power[0] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_1));
	switch_Power[1] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_2));
	switch_Power[2] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_3));
	switch_Power[3] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_4));
	switch_Power[4] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_5));
	switch_Power[5] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_6));
	switch_Power[6] = dynamic_cast<crMultiSwitch *>(m_this->getChildNode(m_str_7));

	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(playerGameData)
	{
		playerGameData->doEvent(WCH_JXJGetPlayerFightingPower,MAKECREPARAM(&output,NULL));
		//if(output<=0)
		//	output = 1;
		strFightingPower = crArgumentParser::appItoa(output);
		//if(switch_Power[0])
			//switch_Power[0]->setActiveSwitchSet(strFightingPower[0]-'0');
		int index = 0;
		for( ;index<strFightingPower.length() && index<7;index++)
		{
			if(switch_Power[index].valid())
				switch_Power[index]->setActiveSwitchSet(strFightingPower[index]-'0');
		}
		while (index<7)
		{
			if(switch_Power[index].valid())
				switch_Power[index]->setActiveSwitchSet(10);
			index++;
		}
	}
}

/////////////////////////////////////////
//
//crJXJListCtrlScrollNewMethod
//
/////////////////////////////////////////
crJXJListCtrlScrollNewMethod::crJXJListCtrlScrollNewMethod()
{
}

crJXJListCtrlScrollNewMethod::crJXJListCtrlScrollNewMethod(const crJXJListCtrlScrollNewMethod& handle):
	crMethod(handle),
	m_srollbar(handle.m_srollbar),
	m_type(handle.m_type)
{
}

void crJXJListCtrlScrollNewMethod::inputParam(int i, void *param)
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

void crJXJListCtrlScrollNewMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_srollbar = str;
		break;
	case 1:
		m_type = atoi(str.c_str());
		break;
	}
}

void crJXJListCtrlScrollNewMethod::operator()(crHandle &handle)
{
	crPlayerGameData *playerData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_this && playerData && m_this->getParentCanvas())
	{
		ref_ptr<crScrollBarWidgetNode>  vScollbar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getParentCanvas()->getWidget(m_srollbar));
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
/////////////////////////////////////////
//
//crJXJMouseSettingOnShowMethod
//
/////////////////////////////////////////
crJXJMouseSettingOnShowMethod::crJXJMouseSettingOnShowMethod()
{
}

crJXJMouseSettingOnShowMethod::crJXJMouseSettingOnShowMethod(const crJXJMouseSettingOnShowMethod& handle):
	crMethod(handle)
{
}

void crJXJMouseSettingOnShowMethod::inputParam(int i, void *param)
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

void crJXJMouseSettingOnShowMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCheckBox = str;
		break;
	case 1:
		m_progressBar = str;
		break;
	default:
		break;
	}
}

void crJXJMouseSettingOnShowMethod::operator()(crHandle &handle)
{
	ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(m_this->getWidget(m_strCheckBox));
	ref_ptr<crScrollBarWidgetNode>  progressBar = dynamic_cast<crScrollBarWidgetNode *>(m_this->getWidget(m_progressBar));

	char speedValue[16];
	char filePath[MAX_PATH];
	std::string strPath = crGlobalHandle::getInstance()->getGameDir();
	sprintf(filePath,"%ssystem.ini\0",strPath.c_str());


	::GetPrivateProfileString("mouse","MoveSpeed","40",speedValue,16,filePath);
	float speed = atof(speedValue);
	speed-=10.0f;
	if(progressBar.valid())
		progressBar->setValue(speed/200.0f);

	bool isLock = ::GetPrivateProfileInt("mouse","Lockable",1,filePath);
	if(checkBox.valid())
		checkBox->select(isLock);
}
/////////////////////////////////////////
//
//crJXJMouseSettingOnShowMethod
//
/////////////////////////////////////////
crJXJSaveMouseSettingMethod::crJXJSaveMouseSettingMethod()
{
}

crJXJSaveMouseSettingMethod::crJXJSaveMouseSettingMethod(const crJXJSaveMouseSettingMethod& handle):
	crMethod(handle)
{
}

void crJXJSaveMouseSettingMethod::inputParam(int i, void *param)
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

void crJXJSaveMouseSettingMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCheckBox = str;
		break;
	case 1:
		m_progressBar = str;
		break;
	case 2:
		m_strTableName = str;
		break;
	default:
		break;
	}
}

void crJXJSaveMouseSettingMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  parentCanvas = m_this->getParentCanvas();
	if(parentCanvas.valid() && crRunGameHandle::getInstance()->isInGame())
	{
		ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_strCheckBox));
		ref_ptr<crScrollBarWidgetNode>  progressBar = dynamic_cast<crScrollBarWidgetNode *>(parentCanvas->getWidget(m_progressBar));

		char filePath[MAX_PATH];
		std::string strPath = crGlobalHandle::getInstance()->getGameDir();
		sprintf(filePath,"%ssystem.ini\0",strPath.c_str());

		if(progressBar.valid())
		{	
			std::string speedValue =  crArgumentParser::appFtoa(progressBar->getValue()*200.0f+10.0f);
			::WritePrivateProfileString("mouse","MoveSpeed",speedValue.c_str(),filePath);
			ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
			if(camera.valid())
			{
				crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(camera->getAttachedNode());
				crData *cameraData = cameraNode->getDataClass();
				float cameraSpeed = progressBar->getValue()*200.0f+10.0f;
				cameraData->inputParam(WCHDATA_JXJCameraMoveSpeed,&cameraSpeed);
			}
		}

		if(checkBox.valid())
		{
			bool cameraFollow = checkBox->getSelect();
			std::string isCheck = crArgumentParser::appItoa(cameraFollow);
			::WritePrivateProfileString("mouse","Lockable",isCheck.c_str(),filePath);

			ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
			if(camera.valid())
			{
				crMatrixTransform *cameraNode = dynamic_cast<crMatrixTransform *>(camera->getAttachedNode());
				crData *cameraData = cameraNode->getDataClass();
				cameraData->inputParam(WCHDATA_JXJCameraFollow,&cameraFollow);
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJMouseSettingOnShowMethod
//
/////////////////////////////////////////
crJXJCheckBoxSelectMethod::crJXJCheckBoxSelectMethod()
{
}

crJXJCheckBoxSelectMethod::crJXJCheckBoxSelectMethod(const crJXJCheckBoxSelectMethod& handle):
	crMethod(handle)
{
}

void crJXJCheckBoxSelectMethod::inputParam(int i, void *param)
{
}

void crJXJCheckBoxSelectMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_strCanvas = str;
		break;
	case 1:
		m_strCheckBox = str;
		break;
	case 2:
		m_newCanvas = str;
		break;
	case 3:
		m_newCanvas_sw = str;
		break;
	case 4:
		m_change = atoi(str.c_str());
		break;
	default:
		break;
	}
}

void crJXJCheckBoxSelectMethod::operator()(crHandle &handle)
{
	ref_ptr<crCanvasNode>  parentCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
	if(parentCanvas.valid())
	{
		ref_ptr<crCheckBoxWidgetNode>  checkBox = dynamic_cast<crCheckBoxWidgetNode *>(parentCanvas->getWidget(m_strCheckBox));
		bool ischeck = false;
		if(checkBox.valid())
		{
			ischeck = checkBox->getSelect();
			ischeck = !ischeck;
			if (m_change != 0)
			{
				checkBox->select(ischeck);
			}
		}
		ref_ptr<crCanvasNode> newCanvas = crFilterRenderManager::getInstance()->findCanvas(m_newCanvas);
		if (newCanvas.valid() && ischeck)
		{
			ref_ptr<crMultiSwitch> newCanvas_sw = dynamic_cast<crMultiSwitch *>(newCanvas->getChildNode(m_newCanvas_sw));
			if (newCanvas_sw.valid())
			{
				newCanvas_sw->setActiveSwitchSet(1);
			}
			crFilterRenderManager::getInstance()->showCanvas(newCanvas.get(), true);
			ref_ptr<crCamera> camera = crCameraManager::getInstance()->getMainCamera();
			if(camera.valid())
			{
				void *param;
				crPlayerGameData *myPlayer = crMyPlayerData::getInstance()->getPlayerGameData();
				if(myPlayer)
				{
					crData *myPlayerData = myPlayer->getDataClass();
					if(myPlayerData)
					{
						myPlayerData->getParam(WCHDATA_JXJVipLv,param);
						unsigned char vipLevel = *(unsigned char *)param;
						crData *camadata = camera->getAttachedNode()->getDataClass();
						short timer = atoi(crGlobalHandle::gData()->gGameGlobalValue(WCHDATA_JXJDelayTimer, vipLevel).c_str()); // 按键提示窗口显示3秒之后自动关闭
						camadata->inputParam(WCHDATA_JXJDelayCanvasCloseTime,&timer);
					}
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crJXJPlaySignalFxLogic
//
/////////////////////////////////////////
crJXJPlaySignalFxLogic::crJXJPlaySignalFxLogic():
	m_targetPosition(crVector3(0.0f,0.0f,0.0f))
{
}

crJXJPlaySignalFxLogic::crJXJPlaySignalFxLogic(const crJXJPlaySignalFxLogic& handle):
	crNodeLogic(handle),
	m_targetPosition(handle.m_targetPosition)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}

void crJXJPlaySignalFxLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_fxNode = NULL;
			if(m_playSceneFx.valid())
				m_playSceneFx->inputParam(0,NULL);
		}
		break;
	case 1:
		m_this = (crMatrixTransform*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_targetPosition = *(crVector3 *)(LOCREPARAM(m_param));
		break;
	}
}

void crJXJPlaySignalFxLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_fileName = str;
		break;
	case 1:
		m_flagName = str;
		break;
	default:
		break;
	}
}

void crJXJPlaySignalFxLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}

void crJXJPlaySignalFxLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_fxNode = param?((crNode *)(param)):NULL;
		break;
	}
}
void crJXJPlaySignalFxLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
void crJXJPlaySignalFxLogic::operator()(crHandle &handle)
{
	if(!m_playSceneFx.valid() && m_targetPosition == crVector3(0.0f,0.0f,0.0f)) return;
	if(!m_fxNode.valid())
	{
		crVector3 camPos = m_this->getTrans();
		crVector3 dir = camPos - m_targetPosition;
		dir[2] = 0.0f;
		dir.normalize();

		m_playSceneFx->inputParam(3,&m_targetPosition);		
		m_playSceneFx->inputParam(4,&dir);
		m_playSceneFx->inputParam(5,&m_fileName);
		(*m_playSceneFx)(*this);
	}
	crPlayerGameData *playerGameData = crMyPlayerData::getInstance()->getPlayerGameData();
	if(m_fxNode.valid() && playerGameData)
	{
		void *param;
		crGroup *root = crSceneManager::getInstance()->getStaticRoot();
		crData *data = playerGameData->getDataClass();
		if(root && data)
		{
			data->getParam(WCHDATA_JXJShiliID,param);
			unsigned char shiliid = *(unsigned char *)param;
			unsigned char activechildindex = shiliid - c_startShiliID;

			crSearchNodeBYNameVisitor collector(m_flagName,CRCore::MULTISWITCH);
			//collector.setSearchNodeType(CRCore::MULTISWITCH);
			//collector.setSearchNodeType(m_flagName);
			m_fxNode->accept(collector);
			crMultiSwitch *swNode = dynamic_cast<crMultiSwitch *>(collector.getResult());
			if(swNode)
			{
				switch (activechildindex)
				{
				case 0:
					swNode->setActiveSwitchSet(1);
					break;
				case 1:
					swNode->setActiveSwitchSet(2);
					break;
				case 2:
					swNode->setActiveSwitchSet(3);
					break;
				default:
					swNode->setActiveSwitchSet(0);
					break;
				}
			}
		}
	}
}

///////////////////////////////////////////
////
////crJXJGameServerTimerMethod
////
///////////////////////////////////////////
//crJXJGameServerTimerMethod::crJXJGameServerTimerMethod():
//	m_timer(0L)
//{
//}
//crJXJGameServerTimerMethod::crJXJGameServerTimerMethod(const crJXJGameServerTimerMethod& handle):
//	crMethod(handle),
//	m_timer(0L)
//{
//}
//void crJXJGameServerTimerMethod::inputParam(int i, void *param)
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
//		m_this = (crServerBrainHandle*)param;
//		break;
//	}
//}
//void crJXJGameServerTimerMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_totalTimer = atoi(str.c_str());
//		break;
//	default:
//		break;
//	}
//}
//void crJXJGameServerTimerMethod::operator()(crHandle &handle)
//{
//	if(m_this)
//	{
//		crData *brainData = m_this->getDataClass();
//		if(brainData)
//		{
//			void *param;
//			brainData->excHandle(MAKECREPARAM(WCH_LockData,1));
//			brainData->getParam(WCHDATA_JXJGameServerTimer,param);
//			int * timer  = ( int *)param;
//			(*timer)++;
//			m_timer++;
//			if(m_timer>m_totalTimer)
//			{
//				//客户端发消息同步时间
//				crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//				crNetDataManager *netDataManager = gameServer->getNetDataManager();
//				crNetManager *netManager = gameServer->getNetManager();
//				ref_ptr<crGameServerPlayerData> playerData;
//				ref_ptr<crStreamBuf> stream = new crStreamBuf;
//				crPlayerDataEventPacket packet;
//
//				netDataManager->lockPlayerDataMap();
//				crNetDataManager::PlayerDataMap &playerDataMap = netDataManager->getPlayerDataMap();
//				for( crNetDataManager::PlayerDataMap::iterator itr = playerDataMap.begin();
//					itr != playerDataMap.end();
//					++itr )
//				{
//					playerData = dynamic_cast<crGameServerPlayerData *>(itr->second.get());
//					if (playerData.valid())
//					{
//						stream->createBuf(5);
//						stream->_writeInt(*timer);
//						stream->_writeBool(true);
//						crPlayerDataEventPacket::buildReplyPacket(packet,itr->first,WCH_JXJRecvServerTimer,stream.get());
//						gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//					}
//				}
//				netDataManager->unLockPlayerDataMap();
//			}
//			brainData->excHandle(MAKECREPARAM(WCH_LockData,0));
//		}
//	}
//}
////////////////////////////////////////////////////////////////////////////
////
////crJXJRecvServerTimerMethod
////
/////////////////////////////////////////////////////////////////////////////
//crJXJRecvServerTimerMethod::crJXJRecvServerTimerMethod(){}
//crJXJRecvServerTimerMethod::crJXJRecvServerTimerMethod(const crJXJRecvServerTimerMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJRecvServerTimerMethod::inputParam(int i, void *param)
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
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_stream = (crStreamBuf *)(LOCREPARAM(param64));
//			m_netType = HICREPARAM(param64);
//		}
//		else
//		{
//			m_stream = NULL;
//		}
//		break;
//	}
//}
//
//void crJXJRecvServerTimerMethod::addParam(int i, const std::string& str)
//{
//	switch (i)
//	{
//	case 0:
//		m_validTime = atoi(str.c_str());
//		break;
//	case 1:
//		m_strCanvas = str;
//		break;
//	case 2:
//		m_strInput = str;
//		break;
//	default:
//		break;
//	}
//}
//
//void crJXJRecvServerTimerMethod::operator()(crHandle &handle)
//{
//	if(m_this && m_stream.valid())
//	{
//		void *param;
//		crTableIO::StrVec record;
//		if(m_netType == GameServer)
//		{
//			crNetConductor *gameServer = crNetContainer::getInstance()->getNetConductor(GameServer); 
//			crNetDataManager *netDataManager = gameServer->getNetDataManager();
//			crNetManager *netManager = gameServer->getNetManager();
//			ref_ptr<crGameServerPlayerData> playerData;
//			ref_ptr<crStreamBuf> stream = new crStreamBuf;
//			crPlayerDataEventPacket packet;
//			crData *brainData = crServerBrainHandle::getInstance()->getDataClass();
//			brainData->getParam(WCHDATA_JXJGameServerTimer,param);
//			int  timer  = *( int *)param;
//			playerData = dynamic_cast<crGameServerPlayerData *>(netDataManager->getPlayerData(m_this->getPlayerID()));
//			if (playerData.valid())
//			{
//				stream->createBuf(5);
//				stream->_writeInt(timer);
//				stream->_writeBool(false);
//				crPlayerDataEventPacket::buildReplyPacket(packet,m_this->getPlayerID(),WCH_JXJRecvServerTimer,stream.get());
//				gameServer->getNetManager()->sendPacket(playerData->getPlayerConnectServerAddress(),packet);
//			}
//		}
//		else if(m_netType == GameClient_Game)
//		{
//			int timer = m_stream->_readInt();
//			bool isCheck = m_stream->_readBool();
//			crMyPlayerData *me = crMyPlayerData::getInstance();
//			if(me && me->getPlayerGameData())
//			{
//				crData *playerData = me->getPlayerGameData()->getDataClass();
//				
//				if(playerData)
//				{
//					playerData->getParam(WCHDATA_JXJClientTimer,param);
//					int clientTimer = *( int *)param;
//
//					if(isCheck)
//					{
//						ref_ptr<crCanvasNode>  msgCanvas = crFilterRenderManager::getInstance()->findCanvas(m_strCanvas);
//						if(abs(clientTimer - timer) > m_validTime && msgCanvas)
//						{
//							crStaticTextWidgetNode *messageInfo = dynamic_cast<crStaticTextWidgetNode *>(msgCanvas->getWidget(m_strInput));
//							if(!messageInfo) return;
//							messageInfo->setAlignment(CRText::crText::LEFT_BASE_LINE);
//							messageInfo->setString(std::string("检测到客户端异常，请退出后重新登录！"));
//							crFilterRenderManager::getInstance()->doModal(msgCanvas);
//						}
//					}
//					else
//					{
//						playerData->inputParam(WCHDATA_JXJClientTimer,&timer);
//					}
//				}
//			}
//
//		}
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
////
////crJXJClientTimerUpdateMethod
////
/////////////////////////////////////////////////////////////////////////////
//crJXJClientTimerUpdateMethod::crJXJClientTimerUpdateMethod(){}
//crJXJClientTimerUpdateMethod::crJXJClientTimerUpdateMethod(const crJXJClientTimerUpdateMethod& handle):
//	crMethod(handle)
//{
//}
//void crJXJClientTimerUpdateMethod::inputParam(int i, void *param)
//{
//}
//
//void crJXJClientTimerUpdateMethod::addParam(int i, const std::string& str)
//{
//}
//
//void crJXJClientTimerUpdateMethod::operator()(crHandle &handle)
//{
//	void *param;
//	crMyPlayerData *me = crMyPlayerData::getInstance();
//	if(me && me->getPlayerGameData())
//	{
//		crData *playerData = me->getPlayerGameData()->getDataClass();
//		if(playerData)
//		{
//			playerData->excHandle(MAKECREPARAM(WCH_LockData,1));
//			playerData->getParam(WCHDATA_JXJClientTimer,param);
//			int * clientTimer = (int *)param;
//			(*clientTimer)++;
//			playerData->excHandle(MAKECREPARAM(WCH_LockData,0));
//		}
//	}
//}
//
