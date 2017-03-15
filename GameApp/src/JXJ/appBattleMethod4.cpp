
/**********************************************************************
*
*	文件:	   appBattleMethod4.cpp
*
*	描述:	   
*
*	作者:	   刘超
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appBattleMethod4.h>
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
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNodeCallbacks.h>
#include <CREncapsulation/crGameDices.h>
#include <CRProducer/crViewer.h>
#include <CRIOManager/crCookFile.h>
#include <JXJ/appData3.h>
#include <JXJ/appMethod.h>
#include <sys/stat.h>
#include <imm.h>
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

JXJ::crJXJRoomElapseTimeIsGreaterMethod::crJXJRoomElapseTimeIsGreaterMethod():
	m_this(NULL),
	m_scriptTime(0)
{

}

JXJ::crJXJRoomElapseTimeIsGreaterMethod::crJXJRoomElapseTimeIsGreaterMethod( const crJXJRoomElapseTimeIsGreaterMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_scriptTime(handle.m_scriptTime)
{

}

void JXJ::crJXJRoomElapseTimeIsGreaterMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}

void JXJ::crJXJRoomElapseTimeIsGreaterMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_scriptTime = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJRoomElapseTimeIsGreaterMethod::operator()( crHandle &handle )
{
	bool isGreater = false;
	if (m_this)
	{
		isGreater = m_this->getGameTime() - m_this->timeRemain() > m_scriptTime;
	}
	handle.outputParam(0,&isGreater);
}

/**************************************************************************

METHOD: crJXJAlterNpcAttrFromTableMethod
 
**************************************************************************/

JXJ::crJXJAlterRelivedNpcAttrFromTableMethod::crJXJAlterRelivedNpcAttrFromTableMethod():
	m_this(NULL),
	m_tableid(0)
{

}

JXJ::crJXJAlterRelivedNpcAttrFromTableMethod::crJXJAlterRelivedNpcAttrFromTableMethod( const crJXJAlterRelivedNpcAttrFromTableMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_tableid(handle.m_tableid)
{

}

void JXJ::crJXJAlterRelivedNpcAttrFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}

void JXJ::crJXJAlterRelivedNpcAttrFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableid = atoi(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_attrFactorVec);
		m_attrFactorVec.resize(4,1.0f);
		break;
	default:
		break;
	}

}

void JXJ::crJXJAlterRelivedNpcAttrFromTableMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			crInstanceItem *item;
			void *param;
			crData *data = m_this->getDataClass();
			//std::list< ref_ptr<crInstanceItem> >ItemList;
			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));

			PatrolPointVec* patrolPointVec;
			int count;
			char idx;
			crVector2 coord;
			//float coordz;
			crVector3 pos;
			float scale = crGlobalHandle::gData()->gUnitScale();

			data->getParam(WCHDATA_JXJBattleID,param);
			int battleid = *(int *)param;
			unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
			unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));
			unsigned short chengchiid = (unsigned short)(HIINT32(battleid));

			float countFactor = 1.0f;
			float hpFactor = 1.0f;
			float attackFactor = 1.0f;
			crTableIO::StrVec record;
			crVector4i lv;
			//获取系数
			ref_ptr<crTableIO> chengchiTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiTab);
			ref_ptr<crTableIO> chengchilvTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJChengChiLvTab);

			data->getParam(WCHDATA_JXJRoomType,param);
			unsigned char roomType = *(unsigned char *)param;

			if(roomType == 1 && chengchiTab.valid() && chengchilvTab.valid())
			{
				int lvIndex = chengchiTab->getTitleIndex("建国后等级");
				if (2 == defenceShili)
				{//降低建国城池等级，占领建国城池后等级恢复正常
					lvIndex = chengchiTab->getTitleIndex("城池等级");
				}
				int countIndex = chengchilvTab->getTitleIndex("数量系数");
				int hpIndex = chengchilvTab->getTitleIndex("HP系数");
				int attackIndex = chengchilvTab->getTitleIndex("攻击系数");
				if(chengchiTab->queryOneRecord(0,crArgumentParser::appItoa(chengchiid),record)>=0 && lvIndex>=0)
				{
					crArgumentParser::appAtoVec(record[lvIndex],lv);
					int index = attackShili - c_startShiliID;
					if( index<4 && chengchilvTab->queryOneRecord(0,crArgumentParser::appItoa(lv[index]),record)>=0 )
					{
						if(countIndex>=0)
							countFactor = atof(record[countIndex].c_str());
						if(hpIndex>=0) 
							hpFactor = atof(record[hpIndex].c_str());
						if(attackIndex>=0)
							attackFactor = atof(record[attackIndex].c_str());
					}
				}
			}

			//unsigned short armcount;
			/*unsigned short*/int armcount; //14、15级城池的政厅和箭塔血量超出65535，显示结果错误，所以更改数据类型
			int* hp;
			int* attack;
			float iscale;
			unsigned char itemstate;
			ref_ptr<crTableIO> dataAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_NpcDataAttrTab);
			int hpid = dataAttrTab->getTitleIndex("HP");
			int attackid = dataAttrTab->getTitleIndex("攻击力");
			int orighp,origattack;
			unsigned char npcType;
			crTableIO::StrVec attrrecord;
		
			int roomid = m_this->getRoomID();

			crSightInfo *defenceSightInfo = m_this->getOrCreateSightInfo(defenceShili);
			crSightInfo *attackSightInfo = m_this->getOrCreateSightInfo(attackShili);
			crData *itemData;
			data->getParam(WCHDATA_JXJReliveItemMap,param);
			ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
			ReliveItemMap::iterator itr = reliveItemMap->find(m_tableid);
			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableid;
				++itr )
			{
				item = scene->findRoomItem(roomid,itr->second->getInstanceID());
				if(item && item->getDataClass())
				{
					itemData = item->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate == IS_Dead)
					{
						npcType = itr->second->getNpcItemType();
						itemData->inputParam(WCHDATA_JXJNpcItemType,&npcType);

						if(!itr->second->getName().empty())
							item->setIName(itr->second->getName());
						if(dataAttrTab->queryOneRecord(0,crArgumentParser::appItoa(item->getAbstractItemID()),attrrecord)>=0)
						{
							if(hpid>0)
							{
								orighp = atoi(attrrecord[hpid].c_str());
								itemData->inputParam(WCHDATA_HP,&orighp);
							}
							if(attackid>0)
							{
								origattack = atoi(attrrecord[attackid].c_str());
								itemData->inputParam(WCHDATA_Attack,&origattack);
							}
						}
						//itemData->inputParam(WCHDATA_ItemState, &itemstate);
						itemData->excHandle(MAKEINT64(WCH_LockData,1));
						armcount = itr->second->getCount();
						if(armcount > 1) 
							armcount  = (int)((float)armcount * countFactor * m_attrFactorVec[3]);
						itemData->inputParam(WCHDATA_JXJArmyCount,&armcount);
						iscale = itr->second->getHPScale();
						if(iscale!=1.0f)
						{
							itemData->getParam(WCHDATA_HP,param);
							if(param)
							{
								hp = (int *)param;
								if(armcount>1)
									*hp = (int)((float)(*hp) * iscale * hpFactor * m_attrFactorVec[0] + 0.5f);
								else
									*hp = (int)((float)(*hp) * iscale * hpFactor * countFactor * m_attrFactorVec[0] + 0.5f);

								float rthp = armcount * (*hp);
								itemData->inputParam(WCHDATA_RTHP,&rthp);
							}
						}
						iscale = itr->second->getAttackScale();
						if(iscale!=1.0f)
						{
							itemData->getParam(WCHDATA_Attack,param);
							if(param)
							{
								attack = (int *)param;
								if(armcount>1)
									*attack = (int)((float)(*attack) * iscale * attackFactor * m_attrFactorVec[1] + 0.5f);
								else
									*attack = (int)((float)(*attack) * iscale * attackFactor * countFactor * m_attrFactorVec[1] + 0.5f);
							}
						}
						itemData->getParam(WCHDATA_PatrolPointVec,param);
						patrolPointVec = (PatrolPointVec*)param;
						if(patrolPointVec && !patrolPointVec->empty())
						{
							itemData->getParam(WCHDATA_PatrolLoopMode,param);
							unsigned char loopmode = *(unsigned char *)param;
							if(loopmode == PL_NoLoop)
							{
								idx = 0;
							}
							else
							{
								count = patrolPointVec->size();
								idx = rangei(0,count).get_random();
								itemData->inputParam(WCHDATA_PatrolIndex,&idx);
							}
							coord = (*patrolPointVec)[idx];
							//coordz = scene->getPosZ(item->getLayerID(),coord[0],coord[1], item->getZoffset() * scale);
							if(!scene->findWalkablePos(item,coord,c_walkableSearchRange,pos))
							{
								CRCore::notify(CRCore::ALWAYS)<<"crJXJReliveItemsFromTabMethod findWalkablePos失败 "<<coord<<std::endl;
							}
							else
							{
								pos/=scale;
								item->setPosxy(pos[0],pos[1]);
								item->setPosz(pos[2]);
							}
						}
						itemData->excHandle(MAKEINT64(WCH_InitData,item));
						itemData->excHandle(MAKEINT64(WCH_LockData,0));
						//if(item/* && item->getItemtype() == crInstanceItem::Npc*/)
						//{
						switch (itr->second->getCamp())
						{
						case 1:
							if(itemData)
								itemData->inputParam(WCHDATA_Camp,&defenceShili);
							item->setSightInfo(defenceSightInfo);//->addEyeItem(item);
							break;
						case 2:
							if(itemData)
								itemData->inputParam(WCHDATA_Camp,&attackShili);
							item->setSightInfo(attackSightInfo);//attackSightInfo->addEyeItem(item);
							break;
						default:
							break;
						}
						//}
						scene->itemRelive(item);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
		}
	}
}

JXJ::crJXJCheckItemsLivedFromTabMethod::crJXJCheckItemsLivedFromTabMethod():
m_id(0),
m_orOrAnd(0)
{}

JXJ::crJXJCheckItemsLivedFromTabMethod::crJXJCheckItemsLivedFromTabMethod( const crJXJCheckItemsLivedFromTabMethod & handle ):
crMethod(handle),
m_id(handle.m_id),
m_orOrAnd(handle.m_orOrAnd)
{

}

void JXJ::crJXJCheckItemsLivedFromTabMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJCheckItemsLivedFromTabMethod::addParam( int i, const std::string& str )
{
	switch(i) 
	{
	case 0:
		m_id = atoi(str.c_str());
		break;
	case 1:
		m_orOrAnd = atoi(str.c_str());
		break;
	}
}

void JXJ::crJXJCheckItemsLivedFromTabMethod::operator()( crHandle &handle )
{
	bool condition = false;
	if (m_orOrAnd)
	{
		condition = true;
	}
	else condition = false;

	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
	if(scene)
	{
		crInstanceItem *item;
		crData *itemData;
		unsigned char itemstate;
		void *param;
		crData *data = m_this->getDataClass();
		scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
		data->excHandle(MAKEINT64(WCH_LockData,1));
		//data->getParam(WCHDATA_RoomProgress,param);
		//short progress = *(short *)param;
		data->getParam(WCHDATA_JXJReliveItemMap,param);
		ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
		ReliveItemMap::iterator itr = reliveItemMap->find(m_id);
		for( ;
			itr != reliveItemMap->end() && itr->first == m_id;
			++itr )
		{
			item = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
			if(item)
			{	
				itemData = item->getDataClass();
				itemData->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				if(itemstate != IS_Dead && !m_orOrAnd)
				{
					condition = true;
					break;
				}
				if (itemstate == IS_Dead && m_orOrAnd)
				{
					condition = false;
					break;
				}
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
		scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
	}
	handle.outputParam(0,&condition);
}

/**************************************************************************

METHOD: crJXJNPCCloseToPlayerFromTableMethod
 
**************************************************************************/
JXJ::crJXJNPCCloseToPlayerFromTableMethod::crJXJNPCCloseToPlayerFromTableMethod():
	m_this(NULL),
	m_tableID(0),
	m_distance(0.0f)
{

}

JXJ::crJXJNPCCloseToPlayerFromTableMethod::crJXJNPCCloseToPlayerFromTableMethod( const crJXJNPCCloseToPlayerFromTableMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_tableID(handle.m_tableID),
	m_distance(handle.m_distance)
{

}

void JXJ::crJXJNPCCloseToPlayerFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}

void JXJ::crJXJNPCCloseToPlayerFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID = atoi(str.c_str());
		break;
	case 1:
		m_distance = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJNPCCloseToPlayerFromTableMethod::operator()( crHandle &handle )
{
	bool isClosing = false;
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			ref_ptr<crInstanceItem > npcItem = NULL;
			void *param;
			crData *data = m_this->getDataClass();
			//typedef std::list< ref_ptr<crInstanceItem> >ItemList;
			//ItemList itemList;
			typedef std::vector<CRCore::crVector3f> PositionVec;
			PositionVec rolesPositionVec;
			PositionVec npcPositionVec;

			ref_ptr<crRole> role = NULL;
			unsigned char itemstate = IS_Dead;
			crData *roledata;
			scene->lockRoomRoleMap();
			CRNetApp::crScene::RoomRoleMap &roomRoleMap = scene->getRoomRoleMap();
			CRNetApp::crScene::SceneRoleMap &sceneRoleMap = roomRoleMap[m_this->getRoomID()];
			for( CRNetApp::crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
				sritr != sceneRoleMap.end();
				++sritr )
			{
				role = sritr->second.get();
				roledata = role->getDataClass();
				roledata->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				if(itemstate != IS_Dead)
				{
					rolesPositionVec.push_back(role->getPosition());
				}
			}
			scene->unlockRoomRoleMap();

			if(!rolesPositionVec.empty())
			{
				scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
				data->excHandle(MAKEINT64(WCH_LockData,1));
				data->getParam(WCHDATA_JXJReliveItemMap,param);
				ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
				crData *itemData;
				ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID);
				for( ;
					itr != reliveItemMap->end() && itr->first == m_tableID;
					++itr )
				{
					npcItem = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
					if(npcItem.valid() && npcItem->getDataClass())
					{
						itemData = npcItem->getDataClass();
						itemData->getParam(WCHDATA_ItemState,param);
						itemstate = *(unsigned char *)param;
						if(itemstate != IS_Dead)
							npcPositionVec.push_back(npcItem->getPosition());
					}
				}
				data->excHandle(MAKEINT64(WCH_LockData,0));
				scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock

				for (PositionVec::iterator rItr = rolesPositionVec.begin();
					rItr != rolesPositionVec.end(); ++ rItr)
				{
					for (PositionVec::iterator nItr = npcPositionVec.begin();
						nItr != npcPositionVec.end(); ++ nItr)
					{
						if ((*rItr - *nItr).length() <= m_distance)
						{
							isClosing = true;
							break;
						}
					}
					if (isClosing)
					{
						break;
					}
				}
			}
		}
	}
	handle.outputParam(0,&isClosing);
}

/**************************************************************************

METHOD: crJXJAlterNPCRTHPFromTableMethod
 
**************************************************************************/

JXJ::crJXJAlterNPCRTHPFromTableMethod::crJXJAlterNPCRTHPFromTableMethod():
	m_this(NULL),
	m_tableID(0),
	m_factor(0.0f)
{

}

JXJ::crJXJAlterNPCRTHPFromTableMethod::crJXJAlterNPCRTHPFromTableMethod( const crJXJAlterNPCRTHPFromTableMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_tableID(handle.m_tableID),
	m_factor(handle.m_factor)
{

}

void JXJ::crJXJAlterNPCRTHPFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}

void JXJ::crJXJAlterNPCRTHPFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID = atoi(str.c_str());
		break;
	case 1:
		m_factor = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJAlterNPCRTHPFromTableMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			typedef std::list< ref_ptr<crInstanceItem> >ItemList;
			ItemList itemList;
			crInstanceItem *item;
			crData *itemData;
			unsigned char itemstate;
			void *param;
			crData *data = m_this->getDataClass();
			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJReliveItemMap,param);
			ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
			ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID);

			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableID;
				++itr )
			{
				item = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
				if(item)
				{
					itemData = item->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						itemList.push_back(item);
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
			float rthp;
			for( ItemList::iterator itr = itemList.begin();
				itr != itemList.end();
				++itr )
			{
				if ((*itr).valid() && (*itr)->getDataClass())
				{
					itemData = (*itr)->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						itemData->getParam(WCHDATA_RTHP,param);
						rthp = *(float *)param;
						rthp *= m_factor;
						DamagePair dp(rthp,std::make_pair(SkillDM,0));
						(*itr)->doEvent(WCH_AddDamage,MAKEINT64(&dp,NULL));
					}
				}
			}
		}
	}
}


/**************************************************************************

METHOD: crJXJRoomProgressIsBelowValMethod
 
**************************************************************************/

JXJ::crJXJRoomProgressIsBelowValMethod::crJXJRoomProgressIsBelowValMethod():
	m_this(NULL)
{

}

JXJ::crJXJRoomProgressIsBelowValMethod::crJXJRoomProgressIsBelowValMethod( const crJXJRoomProgressIsBelowValMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_progressVec(handle.m_progressVec)
{

}

void JXJ::crJXJRoomProgressIsBelowValMethod::operator()( crHandle &handle )
{
	bool condition = false;
	if (m_this)
	{
		void *param;
		crData *data = m_this->getDataClass();
		data->getParam(WCHDATA_RoomProgress,param);
		short progress = *(short *)param;
		ProgressVec::iterator pvItr = std::find(m_progressVec.begin(),m_progressVec.end(),progress);
		condition = pvItr != m_progressVec.end();
	}
	handle.outputParam(0,&condition);
}

void JXJ::crJXJRoomProgressIsBelowValMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJRoomProgressIsBelowValMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_progressVec);
		break;
	default:
		break;
	}

}

/**************************************************************************

METHOD: crJXJNPCCloseToNPCFromTableMethod
 
**************************************************************************/
JXJ::crJXJNPCCloseToNPCFromTableMethod::crJXJNPCCloseToNPCFromTableMethod():
	m_this(NULL),
	m_tableID1(0),
	m_tableID2(0),
	m_distance(0.0f)
{

}

JXJ::crJXJNPCCloseToNPCFromTableMethod::crJXJNPCCloseToNPCFromTableMethod( const crJXJNPCCloseToNPCFromTableMethod & handle ):
	crMethod(handle),
	m_tableID1(handle.m_tableID1),
	m_tableID2(handle.m_tableID2),
	m_distance(handle.m_distance)
{

}

void JXJ::crJXJNPCCloseToNPCFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJNPCCloseToNPCFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID1 = atoi(str.c_str());
		break;
	case 1:
		m_tableID2 = atoi(str.c_str());
		break;
	case 2:
		m_distance = atof(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJNPCCloseToNPCFromTableMethod::operator()( crHandle &handle )
{
	bool isClosing = false;
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			ref_ptr<crInstanceItem > npcItem = NULL;
			void *param;
			crData *data = m_this->getDataClass();
			typedef std::vector<CRCore::crVector3f> PositionVec;
			PositionVec npcPositionVec[2];
			unsigned char itemstate = IS_Dead;

			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJReliveItemMap,param);
			ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
			crData *itemData;
			ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID1);
			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableID1;
				++itr )
			{
				npcItem = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
				if(npcItem.valid() && npcItem->getDataClass())
				{
					itemData = npcItem->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
						npcPositionVec[0].push_back(npcItem->getPosition());
				}
			}

			itr = reliveItemMap->find(m_tableID2);
			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableID2;
				++itr )
			{
				npcItem = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
				if(npcItem.valid() && npcItem->getDataClass())
				{
					itemData = npcItem->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
						npcPositionVec[1].push_back(npcItem->getPosition());
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock

			for (PositionVec::iterator rItr = npcPositionVec[0].begin();
				rItr != npcPositionVec[0].end(); ++ rItr)
			{
				for (PositionVec::iterator nItr = npcPositionVec[1].begin();
					nItr != npcPositionVec[1].end(); ++ nItr)
				{
					if ((*rItr - *nItr).length() <= m_distance)
					{
						isClosing = true;
						break;
					}
				}
				if (isClosing)
				{
					break;
				}
			}
		}
	}
	handle.outputParam(0,&isClosing);
}

/**************************************************************************

METHOD: crJXJAlterPlayerRTHPMethod
**************************************************************************/
JXJ::crJXJAlterPlayerRTHPMethod::crJXJAlterPlayerRTHPMethod():
	m_this(NULL),
	m_factor(1.0f)
{

}

JXJ::crJXJAlterPlayerRTHPMethod::crJXJAlterPlayerRTHPMethod( const crJXJAlterPlayerRTHPMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_factor(handle.m_factor)
{

}

void JXJ::crJXJAlterPlayerRTHPMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJAlterPlayerRTHPMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_factor = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJAlterPlayerRTHPMethod::operator()( crHandle &handle )
{
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			typedef std::vector< ref_ptr<crRole> > RoleVec;
			RoleVec roleVec;
			void *param;
			ref_ptr<crRole> role = NULL;
			unsigned char itemstate = IS_Dead;
			crData *roledata;
			scene->lockRoomRoleMap();
			CRNetApp::crScene::RoomRoleMap &roomRoleMap = scene->getRoomRoleMap();
			CRNetApp::crScene::SceneRoleMap &sceneRoleMap = roomRoleMap[m_this->getRoomID()];
			for( CRNetApp::crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
				sritr != sceneRoleMap.end();
				++sritr )
			{
				role = sritr->second.get();
				if (role.valid() && role->getDataClass())
				{
					roledata = role->getDataClass();
					roledata->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						roleVec.push_back(role);
					}
				}
			}
			scene->unlockRoomRoleMap();

			for( RoleVec::iterator itr = roleVec.begin();
				itr != roleVec.end();
				++itr )
			{
				if ((*itr).valid() && (*itr)->getDataClass())
				{
					roledata = (*itr)->getDataClass();
					roledata->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						roledata->getParam(WCHDATA_RTHP,param);
						float rthp = *(float *)param;
						rthp *= m_factor;
						DamagePair dp(rthp,std::make_pair(SkillDM,0));
						(*itr)->doEvent(WCH_AddDamage,MAKEINT64(&dp,NULL));
					}
				}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJNPCCloseToPointFromTableMethod
 
**************************************************************************/
JXJ::crJXJNPCCloseToPointFromTableMethod::crJXJNPCCloseToPointFromTableMethod():
	m_this(NULL),
	m_tableID(0),
	m_distance(0.0f)
{

}

JXJ::crJXJNPCCloseToPointFromTableMethod::crJXJNPCCloseToPointFromTableMethod( const crJXJNPCCloseToPointFromTableMethod & handle ):
	crMethod(handle),
	m_this(NULL),
	m_tableID(handle.m_tableID),
	m_distance(handle.m_distance),
	m_point(handle.m_point)
{

}

void JXJ::crJXJNPCCloseToPointFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJNPCCloseToPointFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID = atoi(str.c_str());
		break;
	case 1:
		{
			crArgumentParser::appAtoVec(str,m_point);
			float scale = crGlobalHandle::gData()->gUnitScale();
			m_point[0] *= scale;
			m_point[1] *= scale;
		}
		break;
	case 2:
		m_distance = atof(str.c_str());
		break;
	default:
		break;
	}
}

void JXJ::crJXJNPCCloseToPointFromTableMethod::operator()( crHandle &handle )
{
	bool isClosing = false;
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			ref_ptr<crInstanceItem > npcItem = NULL;
			void *param;
			crData *data = m_this->getDataClass();
			typedef std::vector<CRCore::crVector2f> PositionVec;
			PositionVec npcPositionVec;
			CRCore::crVector3f point;
			unsigned char itemstate = IS_Dead;

			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJReliveItemMap,param);
			ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
			crData *itemData;
			ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID);
			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableID;
				++itr )
			{
				npcItem = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
				if(npcItem.valid() && npcItem->getDataClass())
				{
					itemData = npcItem->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						point = npcItem->getPosition();
						npcPositionVec.push_back(CRCore::crVector2f(point[0],point[1]));
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock

			for (PositionVec::iterator pvItr = npcPositionVec.begin();
				pvItr != npcPositionVec.end(); ++ pvItr)
			{
				if ( (*pvItr - m_point).length() <= m_distance)
				{
					isClosing = true;
					break;
				}
			}
		}
	}
	handle.outputParam(0,&isClosing);
}

/**************************************************************************

METHOD: crJXJPlayerCloseToPointMethod
 
**************************************************************************/

JXJ::crJXJPlayerCloseToPointMethod::crJXJPlayerCloseToPointMethod():
	m_this(NULL),
	m_distance(0.0f)
{

}

JXJ::crJXJPlayerCloseToPointMethod::crJXJPlayerCloseToPointMethod( const crJXJPlayerCloseToPointMethod & handle ):
	crMethod(handle),
	m_distance(handle.m_distance),
	m_point(handle.m_point)
{

}

void JXJ::crJXJPlayerCloseToPointMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJPlayerCloseToPointMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		{
			crArgumentParser::appAtoVec(str,m_point);
			float scale = crGlobalHandle::gData()->gUnitScale();
			m_point[0] *= scale;
			m_point[1] *= scale;
		}
		break;
	case 1:
		m_distance = atof(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJPlayerCloseToPointMethod::operator()( crHandle &handle )
{
	bool isClosing = false;
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			void *param;
			//crData *data = m_this->getDataClass();
			typedef std::vector<CRCore::crVector2f> PositionVec;
			PositionVec rolesPositionVec;
			CRCore::crVector3f point;

			ref_ptr<crRole> role = NULL;
			unsigned char itemstate = IS_Dead;
			crData *roledata;
			scene->lockRoomRoleMap();
			CRNetApp::crScene::RoomRoleMap &roomRoleMap = scene->getRoomRoleMap();
			CRNetApp::crScene::SceneRoleMap &sceneRoleMap = roomRoleMap[m_this->getRoomID()];
			for( CRNetApp::crScene::SceneRoleMap::iterator sritr = sceneRoleMap.begin();
				sritr != sceneRoleMap.end();
				++sritr )
			{
				role = sritr->second.get();
				roledata = role->getDataClass();
				roledata->getParam(WCHDATA_ItemState,param);
				itemstate = *(unsigned char *)param;
				if(itemstate != IS_Dead)
				{
					point = role->getPosition();
					rolesPositionVec.push_back(CRCore::crVector2f(point[0],point[1]));
				}
			}
			scene->unlockRoomRoleMap();

			for (PositionVec::iterator rItr = rolesPositionVec.begin();
				rItr != rolesPositionVec.end(); ++ rItr)
			{
				if ((*rItr - m_point).length() <= m_distance)
				{
					isClosing = true;
					break;
				}
			}
		}
	}
	handle.outputParam(0,&isClosing);
}

/**************************************************************************

METHOD: crJXJNPCSurvivalNumIsEqualMethod
 
**************************************************************************/
JXJ::crJXJNPCSurvivalNumIsNotLessMethod::crJXJNPCSurvivalNumIsNotLessMethod():
	m_this(NULL),
	m_num(0),
	m_tableID(0)
{

}

JXJ::crJXJNPCSurvivalNumIsNotLessMethod::crJXJNPCSurvivalNumIsNotLessMethod( const crJXJNPCSurvivalNumIsNotLessMethod & handle ):
	crMethod(handle),
	m_num(handle.m_num),
	m_tableID(handle.m_tableID)
{

}

void JXJ::crJXJNPCSurvivalNumIsNotLessMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}
}

void JXJ::crJXJNPCSurvivalNumIsNotLessMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID = atoi(str.c_str());
		break;
	case 1:
		m_num = atoi(str.c_str());
		break;
	default:
		break;
	}

}

void JXJ::crJXJNPCSurvivalNumIsNotLessMethod::operator()( crHandle &handle )
{
	bool condition = false;
	unsigned char count = 0;
	if (m_this)
	{
		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
		CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
		if(scene)
		{
			ref_ptr<crInstanceItem > npcItem = NULL;
			void *param;
			crData *data = m_this->getDataClass();
			unsigned char itemstate = IS_Dead;

			scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
			data->excHandle(MAKEINT64(WCH_LockData,1));
			data->getParam(WCHDATA_JXJReliveItemMap,param);
			ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
			crData *itemData;
			ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID);
			for( ;
				itr != reliveItemMap->end() && itr->first == m_tableID;
				++itr )
			{
				npcItem = scene->findRoomItem(m_this->getRoomID(),itr->second->getInstanceID());
				if(npcItem.valid() && npcItem->getDataClass())
				{
					itemData = npcItem->getDataClass();
					itemData->getParam(WCHDATA_ItemState,param);
					itemstate = *(unsigned char *)param;
					if(itemstate != IS_Dead)
					{
						++ count;
					}
				}
			}
			data->excHandle(MAKEINT64(WCH_LockData,0));
			scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
		}
	}
	condition = count >= m_num;
	handle.outputParam(0,&condition);
}



/**************************************************************************

METHOD: crJXJExtraGainMpPerAttackMethod
 
**************************************************************************/
crJXJExtraGainMpPerAttackMethod::crJXJExtraGainMpPerAttackMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraGainMpPerAttackMethod::crJXJExtraGainMpPerAttackMethod(const crJXJExtraGainMpPerAttackMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraGainMpPerAttackMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraGainMpPerAttackMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraGainMpPerAttackMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_JXJAttackExtraGainMp,param);
		short* extra = (short *)param;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}


/************************************************************************/
/*  crJXJExtraGainMpPerTimerMethod                                                                    */
/************************************************************************/

crJXJExtraGainMpPerTimerMethod::crJXJExtraGainMpPerTimerMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_interval(0.0f)
	{}
crJXJExtraGainMpPerTimerMethod::crJXJExtraGainMpPerTimerMethod(const crJXJExtraGainMpPerTimerMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f)
{
}
void crJXJExtraGainMpPerTimerMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraGainMpPerTimerMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = atof(str.c_str());
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraGainMpPerTimerMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = 0.0f;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_dtrec >= m_interval)
		{
			// 每 m_interval 作用一次
			m_dtrec -= m_interval;
			if(m_extraValue>0.0f)
			{
				float addMp = m_extraValue;
				m_this->doEvent(MAKEINT64(WCH_JXJCureRTMp,0),MAKEINT64(&addMp,NULL));
			}
			else
			{
				float addMp = -m_extraValue;
				m_this->doEvent(MAKEINT64(WCH_JXJReduceRTMp,0),MAKEINT64(&addMp,NULL));
			}
		}
	}
}

/************************************************************************/
/*  crJXJExtraReduceMpPerTimerAndRemoveExtraMethod                                                                    */
/************************************************************************/

crJXJExtraReduceMpPerTimerAndRemoveExtraMethod::crJXJExtraReduceMpPerTimerAndRemoveExtraMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0),
	m_start(false),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_interval(0.0f),
	m_output(NULL)
	{}
crJXJExtraReduceMpPerTimerAndRemoveExtraMethod::crJXJExtraReduceMpPerTimerAndRemoveExtraMethod(const crJXJExtraReduceMpPerTimerAndRemoveExtraMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_interval(handle.m_interval),
	m_start(false),
	m_timer(0.0f),
	m_output(NULL),
	m_dtrec(0.0f)
{
}
void crJXJExtraReduceMpPerTimerAndRemoveExtraMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	case 3:
		m_output = (bool*)param;
		break;
	}
}

void crJXJExtraReduceMpPerTimerAndRemoveExtraMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = atof(str.c_str());
		break;
	case 2:
		m_interval = atof(str.c_str());
		break;
	}
}

void crJXJExtraReduceMpPerTimerAndRemoveExtraMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		*m_output = false;
		m_dtrec = 0.0f;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_dtrec >= m_interval)
		{
			// 每m_interval 作用一次
			m_dtrec -= m_interval;
			crData *thisData = m_this->getDataClass();
			void *param;
			thisData->getParam(WCHDATA_RTMP,param);
			float rtmp = *(float *)param;
			if (rtmp < m_extraValue)
			{
				*m_output = true;
			}
			else
			{
				float reduceMp = m_extraValue;
				m_this->doEvent(MAKEINT64(WCH_JXJReduceRTMp,0),MAKEINT64(&reduceMp,NULL));
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJExtraReduceMpPerSkillDamageMethod
 
**************************************************************************/
crJXJExtraReduceMpPerSkillDamageMethod::crJXJExtraReduceMpPerSkillDamageMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraReduceMpPerSkillDamageMethod::crJXJExtraReduceMpPerSkillDamageMethod(const crJXJExtraReduceMpPerSkillDamageMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraReduceMpPerSkillDamageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraReduceMpPerSkillDamageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraReduceMpPerSkillDamageMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_JXJExtraUseItemReduceMp,param);
		short* extra = (short *)param;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}

/**************************************************************************

METHOD: crJXJGetGainRTMPMethod
 
**************************************************************************/
crJXJGetGainRTMPMethod::crJXJGetGainRTMPMethod():
	m_output(NULL),
m_lasttime(0L){}
crJXJGetGainRTMPMethod::crJXJGetGainRTMPMethod(const crJXJGetGainRTMPMethod& handle):
	crMethod(handle),
	m_output(NULL),
	m_lasttime(0L)
{
}
void crJXJGetGainRTMPMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_output = (float *)(LOINT64(param64));
			m_type = HIINT64(param64); 
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}
void crJXJGetGainRTMPMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetGainRTMPMethod::operator()(crHandle &handle)
{
	if(m_this && m_output)
	{
		crData *data = m_this->getDataClass();
		void *param;
		short base = 0;
		short extraGainMp = 0;
		switch (m_type)
		{
		case 0://
			data->getParam(WCHDATA_JXJDamageGainMP,param);
			base = *((short *)param);
			// 受到伤害
			data->getParam(WCHDATA_JXJDamageExtraGainMp_RD,param);
			extraGainMp = *((short*)param);
			break;
		case 1:
			if(!crGlobalHandle::isClient())
			{
				CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
				if(m_lasttime == 0)
				{
					m_lasttime = t1;
				}
				else
				{
					float dt = CRCore::Timer::instance()->delta_s( m_lasttime, t1 );
					if(dt<0.1f)
					{
						break;
					}
					m_lasttime = t1;
				}
			}
			data->getParam(WCHDATA_JXJAttackGainMP,param);
			base = *((short *)param);
			// 攻击类型
			data->getParam(WCHDATA_JXJAttackExtraGainMp_RD,param);
			extraGainMp = *((short*)param);
			break;
		}
		//data->getParam(WCHDATA_ExtraAttackPercent_RD,param);
		//short extraPercent = *((short*)param);

		//data->getParam(WCHDATA_InbornAttackPercent,param);
		//short inborn = *((short*)param);

		//crItemChild *itemChild;
		//float equip = 0.0f;
		//float equipSum = 0.0f;
		//crInstanceItem *item;
		//data->excHandle(MAKEINT64(WCH_LockData,1));
		//data->getParam(WCHDATA_EquipOnMap,param);
		//EquipOnMap equipOnMap = *(EquipOnMap*)param;
		//data->excHandle(MAKEINT64(WCH_LockData,0));
		//for( EquipOnMap::iterator itr = equipOnMap.begin();
		//	 itr != equipOnMap.end();
		//	 ++itr )
		//{
  //          itemChild = m_this->findChildItem(itr->second);
		//	if(itemChild && itemChild->isLoaded())
		//	{
		//		item = itemChild->getInstanceItem();
		//		equip = 0.0f;
		//		item->doEvent(WCH_GetEquipAttack,MAKEINT64(m_this,&equip));
		//		equipSum+=equip;
		//	}
		//}

		//*m_output = (float)extraAttack+(float)base*(1.0f+(float)(inborn+extraPercent/*+equipSum*/)*0.001f);
		*m_output = (float)base + (float)extraGainMp;
		if(*m_output<0.0f) *m_output = 0.0f;
		//*m_output += (float)extraAttack;
		//if(*m_output<0.0f) *m_output = 0.0f;
		//*m_output *= crGameDices::getInstance()->dicef(m_dice[0],m_dice[1]);
	}
}

/**************************************************************************

METHOD: crJXJGetSkillNeedRTMPMethod
 
**************************************************************************/
crJXJGetSkillNeedRTMPMethod::crJXJGetSkillNeedRTMPMethod():
	m_output(NULL),
	m_skillThis(NULL)
{}
crJXJGetSkillNeedRTMPMethod::crJXJGetSkillNeedRTMPMethod(const crJXJGetSkillNeedRTMPMethod& handle):
	crMethod(handle),
	m_skillThis(NULL),
	m_output(NULL)
{
}
void crJXJGetSkillNeedRTMPMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_output = (float *)(LOINT64(param64));
			m_skillThis = (crInstanceItem*)(HIINT64(param64));
		}
		else
		{
			m_output = NULL;
		}
		break;
	}
}
void crJXJGetSkillNeedRTMPMethod::addParam(int i, const std::string& str)
{
}
void crJXJGetSkillNeedRTMPMethod::operator()(crHandle &handle)
{
	if(m_this && m_output)
	{
		crData *data = m_this->getDataClass();
		void *param;

		crData * skillData = m_skillThis->getDataClass();
		skillData->getParam(WCHDATA_NeedMP,param);
		unsigned short base = *((unsigned short *)param);

		data->getParam(WCHDATA_JXJExtraUseItemReduceMp_RD,param);
		short extraSkillNeedMp = *((short*)param);

		//*m_output = (float)extraAttack+(float)base*(1.0f+(float)(inborn+extraPercent/*+equipSum*/)*0.001f);
		*m_output = (float)base + (float)extraSkillNeedMp;
		if(*m_output<0.0f) *m_output = 0.0f;
		//*m_output += (float)extraAttack;
		//if(*m_output<0.0f) *m_output = 0.0f;
		//*m_output *= crGameDices::getInstance()->dicef(m_dice[0],m_dice[1]);
	}
}


/**************************************************************************

METHOD: crJXJExtraDonotGetMPMethod 

**************************************************************************/
crJXJExtraDonotGetMPMethod::crJXJExtraDonotGetMPMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraDonotGetMPMethod::crJXJExtraDonotGetMPMethod(const crJXJExtraDonotGetMPMethod & handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraDonotGetMPMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraDonotGetMPMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	//case 1:
	//	m_extraValue = (short)(atoi(str.c_str()));
	//	break;
	}
}

void crJXJExtraDonotGetMPMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		void *param;
		thisData->getParam(WCHDATA_JXJExtraRevertType,param);
		unsigned char *revertType = (unsigned char *)param;
		*revertType |= ART_NotRevertMP;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}

///**************************************************************************
//
//METHOD: crJXJReduceRTMPDirectlyMethod
// 
//**************************************************************************/
//
//JXJ::crJXJReduceRTMPDirectlyMethod::crJXJReduceRTMPDirectlyMethod():
//	m_this(NULL),
//	m_hitParam(NULL),
//	m_extraValue(0.0f)
//{
//
//}
//
//JXJ::crJXJReduceRTMPDirectlyMethod::crJXJReduceRTMPDirectlyMethod( const crJXJReduceRTMPDirectlyMethod& handle ):
//	crMethod(handle),
//	m_this(NULL),
//	m_hitParam(NULL),
//	m_extraValue(handle.m_extraValue)
//{
//
//}
//
//void JXJ::crJXJReduceRTMPDirectlyMethod::operator()( crHandle &handle )
//{
//	if(m_hitParam.valid() && m_hitParam->m_hitItem.valid())
//	{
//		m_hitParam->m_hitItem->doEvent(WCH_JXJReduceRTMp,MAKEINT64(&m_extraValue,0));
//	}
//}
//
//void JXJ::crJXJReduceRTMPDirectlyMethod::inputParam( int i, void *param )
//{
//	switch(i) 
//	{
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			_crInt64 param64 = *(_crInt64*)param;
//			m_hitParam = (HitParam *)(LOINT64(param64));
//		}
//		else
//		{
//			m_hitParam = NULL;
//		}
//		break;
//	}
//}
//
//void JXJ::crJXJReduceRTMPDirectlyMethod::addParam( int i, const std::string& str )
//{
//	switch (i)
//	{
//	case 0:
//		m_extraValue = atof(str.c_str());
//		break;
//	}
//}
/**************************************************************************

METHOD: crJXJHitParamRTMPMethod
 
**************************************************************************/
JXJ::crJXJHitParamRTMPMethod::crJXJHitParamRTMPMethod():
	m_this(NULL),
	m_hitParam(NULL),
	m_flg(0),
	m_extraValue(0.0f)
{
}

JXJ::crJXJHitParamRTMPMethod::crJXJHitParamRTMPMethod( const crJXJHitParamRTMPMethod& handle ):
	crMethod(handle),
	m_this(NULL),
	m_hitParam(NULL),
	m_flg(handle.m_flg),
	m_extraValue(handle.m_extraValue)
{
}
void JXJ::crJXJHitParamRTMPMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_hitParam = (HitParam *)(LOINT64(param64));
		}
		else
		{
			m_hitParam = NULL;
		}
		break;
	}

}

void JXJ::crJXJHitParamRTMPMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_flg = atoi(str.c_str());
		break;
	case 1:
		m_extraValue = atof(str.c_str());
		break;
	}
}
void JXJ::crJXJHitParamRTMPMethod::operator()( crHandle &handle )
{
	if(m_hitParam.valid())
	{
		switch (m_flg)
		{
		case 0:
			if(m_hitParam->m_fireItem.valid())
			{
				if(m_extraValue>0)
					m_hitParam->m_fireItem->doEvent(WCH_JXJCureRTMp,MAKEINT64(&m_extraValue,0));
				else
				{
					float value = -m_extraValue;
					m_hitParam->m_fireItem->doEvent(WCH_JXJReduceRTMp,MAKEINT64(&value,0));
				}
			}
			break;
		case 1:
			if(m_hitParam->m_hitItem.valid())
			{
				if(m_extraValue>0)
					m_hitParam->m_hitItem->doEvent(WCH_JXJCureRTMp,MAKEINT64(&m_extraValue,0));
				else
				{
					float value = -m_extraValue;
					m_hitParam->m_hitItem->doEvent(WCH_JXJReduceRTMp,MAKEINT64(&value,0));
				}
			}
			break;
		}
	}
}

/**************************************************************************

METHOD: crJXJClearTargetExtraPerTimeMethod
 
**************************************************************************/
crJXJClearTargetExtraPerTimeMethod::crJXJClearTargetExtraPerTimeMethod():
	m_exceptBufType(-1),
	m_timer(0.0f),
	m_interval(0.0f),
	m_dtrec(0.0f),
	m_duration(0.0f),
	m_start(false)
{}
crJXJClearTargetExtraPerTimeMethod::crJXJClearTargetExtraPerTimeMethod(const crJXJClearTargetExtraPerTimeMethod& handle):
	crMethod(handle),
	m_exceptBufType(handle.m_exceptBufType),
	m_interval(handle.m_interval),
	m_duration(handle.m_duration),
	m_timer(0.0f),
	m_dtrec(0.0f),
	m_start(false)
{
}
void crJXJClearTargetExtraPerTimeMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float *)(LOINT64(param64));
			//m_hitParam = (HitParam *)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}
void crJXJClearTargetExtraPerTimeMethod::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_exceptBufType = atoi(str.c_str());
		break;
	case 2:
		m_interval = atoi(str.c_str());
		break;
	default:
		break;
	}
}
void crJXJClearTargetExtraPerTimeMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
		m_dtrec = m_interval;
	}
	if(m_timer>=0.0f)
	{
		if(m_duration>0.0f) m_timer -= *m_dt;
		m_dtrec += *m_dt;
		if(m_dtrec>= m_interval)
		{//每隔interval对外作用一次
			m_dtrec -= m_interval;
			///////清楚负面BUF
			void *param;
			crData *data = m_this->getDataClass();
			data->getParam(WCHDATA_ExtraData,param);
			if(param)
			{////这个做法不合理，会引起宕机，暂时注掉
				crData *extraData = (crData *)param;
				//ref_ptr<crHandle> handle;
				extraData->excHandle(MAKEINT64(WCH_LockData,1));

				ref_ptr<crTableIO> bufferAttrTab = crGlobalHandle::gData()->gGlobalTable(WCHDATA_JXJBuffAttrTab);
				crTableIO::StrVec record;
				int isNegativeIdx = -1;
				if (bufferAttrTab.valid()) isNegativeIdx = bufferAttrTab->getTitleIndex("Buf类型");

				extraData->getParam(WCHDATA_ExtraIDMap,param);
				ExtraIDMap *extraIDMap = (ExtraIDMap *)param;
				for( ExtraIDMap::iterator itr = extraIDMap->begin();
					itr != extraIDMap->end(); )
				{
					std::string extraValue = crArgumentParser::appItoa(itr->first)+std::string("|")+crArgumentParser::appItoa(HIINT16(itr->second));
					if(bufferAttrTab.valid() && bufferAttrTab->queryOneRecord(0,extraValue,record)>=0 && isNegativeIdx >= 0)
					{
						if (atoi(record[isNegativeIdx].c_str()) == m_exceptBufType)
						{
							//if(crGlobalHandle::isClient())
							//{
							//	handle = extraData->getHandle(MAKEINT64(WCH_DoExtra,itr->first));
							//	if(handle.valid())
							//	{
							//		handle->releaseObjects(NULL);
							//	}
							//}
							extraData->removeHandle(MAKEINT64(WCH_DoExtra,itr->first));
							itr = extraIDMap->erase(itr);
						}
						else
						{
							++itr;
						}
					}
					else ++ itr;
				}
				extraData->excHandle(MAKEINT64(WCH_LockData,0));

				crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				if (sceneServerConductor)
				{
					crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
					crNetManager *netManager = sceneServerConductor->getNetManager();
					crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
					CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
					if(scene)
					{
						if(m_this->getItemtype() == crInstanceItem::Role)
						{
							int hitid = m_this->getID();
							crPlayerEventPacket clearExtraPacket;
							ref_ptr<crStreamBuf> stream = new crStreamBuf;
							stream->createBuf(1);
							stream->_writeChar(m_exceptBufType);
							crPlayerEventPacket::buildRequestPacket(clearExtraPacket,hitid,m_this,WCH_ClearTargetExtra	,stream.get());
							ref_ptr<crSceneServerPlayerData> hitPlayerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(hitid));
							if (hitPlayerData.valid())
							{
								netManager->sendPacket(hitPlayerData->getPlayerConnectServerAddress(),clearExtraPacket);
							}
							scene->sendPacketToItemNeighbor(m_this,clearExtraPacket);
						}
					}
				}
			}
		}
	}
}

/**************************************************************************

METHOD: crJXJChangeNPCShiliFromTableMethod
 
**************************************************************************/

JXJ::crJXJChangeNPCShiliFromTableMethod::crJXJChangeNPCShiliFromTableMethod():
	m_this(NULL),
	m_tableID(0),
	m_scriptVal(-1),
	m_initdata(false)
{

}

JXJ::crJXJChangeNPCShiliFromTableMethod::crJXJChangeNPCShiliFromTableMethod( const crJXJChangeNPCShiliFromTableMethod & handle ):
	crMethod(handle),
	m_tableID(handle.m_tableID),
	m_scriptVal(handle.m_scriptVal),
	m_initdata(handle.m_initdata)
{

}

void JXJ::crJXJChangeNPCShiliFromTableMethod::inputParam( int i, void *param )
{
	switch(i) 
	{
	case 1:
		m_this = (crRoom *)param;
		break;
	}

}

void JXJ::crJXJChangeNPCShiliFromTableMethod::addParam( int i, const std::string& str )
{
	switch (i)
	{
	case 0:
		m_tableID = atoi(str.c_str());
		break;
	case 1:
		m_scriptVal = atoi(str.c_str());
		break;
	case 2:
		m_initdata = (bool)(atoi(str.c_str()));
		break;
	}
}

void JXJ::crJXJChangeNPCShiliFromTableMethod::operator()( crHandle &handle )
{
	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
	crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
	CRNetApp::crScene *scene = netCallback->findScene(m_this->getSceneID());
	if(scene)
	{
		ref_ptr<crInstanceItem> item;
		int roomid = m_this->getRoomID();

		void *param;
		crData *data = m_this->getDataClass();
		data->getParam(WCHDATA_JXJBattleID,param);
		int battleid = *(int *)param;
		unsigned char defenceShili = (unsigned char)(HIINT16(LOINT32(battleid)));
		unsigned char attackShili = (unsigned char)(LOINT16(LOINT32(battleid)));
		unsigned short chengchiid = (unsigned short)(HIINT32(battleid));

		crSightInfo *defenceSightInfo = m_this->getOrCreateSightInfo(defenceShili);
		crSightInfo *attackSightInfo = m_this->getOrCreateSightInfo(attackShili);
		crData *itemData;
		scene->lockRoomItemMap();//m_roomItemMutex->roomdatalock
		data->excHandle(MAKEINT64(WCH_LockData,1));
		data->getParam(WCHDATA_JXJReliveItemMap,param);
		ReliveItemMap *reliveItemMap = (ReliveItemMap *)param;
		ref_ptr<crSightInfo> itemsight;
		ReliveItemMap::iterator itr = reliveItemMap->find(m_tableID);
		for( ;
			 itr != reliveItemMap->end() && itr->first == m_tableID;
			 ++itr )
		{
			item = scene->findRoomItem(roomid,itr->second->getInstanceID());
			if(item.valid() && item->getDataClass())
			{
				itemData = item->getDataClass();
				//scene->itemDead(item.get());
				itemsight = item->getSightInfo();
				if(itemsight.valid())
					itemsight->removeEyeItem(item->getID());
				switch (m_scriptVal)
				{
				case 1:
					if (itemData)
					{
						itemData->inputParam(WCHDATA_Camp, &defenceShili);
						item->setSightInfo(defenceSightInfo);
					}
					break;
				case 2:
					if (itemData)
					{
						itemData->inputParam(WCHDATA_Camp, &attackShili);
						item->setSightInfo(attackSightInfo);
					}
					break;
				default:
					break;
				}
				if(m_initdata)
					itemData->excHandle(MAKEINT64(WCH_InitData,item.get()));
				scene->itemRelive(item.get());
			}
		}
		data->excHandle(MAKEINT64(WCH_LockData,0));
		scene->unlockRoomItemMap();//m_roomItemMutex->roomdatalock
	}
}


/**************************************************************************

METHOD: crJXJExtraGainMpPerDamageMethod
 
**************************************************************************/
crJXJExtraGainMpPerDamageMethod::crJXJExtraGainMpPerDamageMethod():
	m_dt(NULL),
	m_duration(0.0f),
	m_extraValue(0.0f),
	m_start(false),
	m_timer(0.0f){}
crJXJExtraGainMpPerDamageMethod::crJXJExtraGainMpPerDamageMethod(const crJXJExtraGainMpPerDamageMethod& handle):
	crMethod(handle),
	m_dt(NULL),
	m_duration(handle.m_duration),
	m_extraValue(handle.m_extraValue),
	m_start(false),
	m_timer(0.0f)
{
}
void crJXJExtraGainMpPerDamageMethod::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_dt = (float*)(LOINT64(param64));
		}
		else
		{
			m_dt = NULL;
		}
		break;
	}
}

void crJXJExtraGainMpPerDamageMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	case 1:
		m_extraValue = (short)(atoi(str.c_str()));
		break;
	}
}

void crJXJExtraGainMpPerDamageMethod::operator()(crHandle &handle)
{
	if(!m_start)
	{
		m_start = true;
		m_timer = m_duration;
	}
	if(m_timer>=0.0f)
	{
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->excHandle(MAKEINT64(WCH_LockData,1));
		thisData->getParam(WCHDATA_JXJDamageExtraGainMp,param);
		short* extra = (short *)param;
		*extra += m_extraValue;
		thisData->excHandle(MAKEINT64(WCH_LockData,0));

		if(m_duration>0.0f) m_timer -= *m_dt;
	}
}
