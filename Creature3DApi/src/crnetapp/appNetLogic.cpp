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
#include <CREncapsulation/crNodeCallbacks.h>
#include <CRNetApp/appNetLogic.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/crArgumentParser.h>
#include <CRNet/crNetWorkUpdater.h>
#include <CREncapsulation/crLoader.h>
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetPackets.h>
#include <CRCore/thread/crThread.h>
#include "rcfg/ConfigScriptOO.h"
#include <sys/stat.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRPhysics/crBodyCaseMatterObject.h>
#include <CRCore/crEffectSequenceManager.h>
//#include <CRParticle/crEffectParticleManager.h>
#include <CRCore/crSequence.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crSoundManager.h>
#include <CRCore/crLightSourceManager.h>
#include <CRIOManager/crLoadManager.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRIOManager/crCookFile.h>
#include <CRNetApp/appNetMethod.h>
#include <CRSound/crSoundSystem.h>
#include <CRDownload/crDownload.h>
#include <CRNetApp/appNodeCallbacks.h>
using namespace CRCore;
using namespace CRNet;
using namespace CRGUI;
using namespace CRNetApp;
using namespace CRPhysics;
using namespace CRProducer;
using namespace CRParticle;
//using namespace CRAL;
using namespace CREncapsulation;
using namespace CRIOManager;
using namespace CRUI;
using namespace CRSound;
using namespace CRDownload;
///////////////////////////////////////////
////
////crDownloadedUpdatecfgLogic
////
///////////////////////////////////////////
//crDownloadedUpdatecfgLogic::crDownloadedUpdatecfgLogic():
//m_fileName(NULL),
//m_netType(NULL),
//m_downloadFlg(0),
//m_needDownload(false)
//{
//}
//crDownloadedUpdatecfgLogic::crDownloadedUpdatecfgLogic(const crDownloadedUpdatecfgLogic &handle):
//crLogic(handle),
//m_fileName(NULL),
//m_netType(0),
//m_downloadFlg(0),
//m_needDownload(false)
//{
//	if(handle.m_checkAndDownloadFileMathod.valid())
//		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
//}
//void crDownloadedUpdatecfgLogic::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_fileName = NULL;
//			m_netType = 0;
//			m_downloadFlg = 0;
//		}
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_fileName = (std::string *)(LOINT64(param64));
//			int i = HIINT64(param64);
//			m_netType = LOINT32(i);
//			m_downloadFlg = (unsigned short)(HIINT32(i));
//		}
//		else
//		{
//			m_fileName = NULL;
//			m_netType = 0;
//			m_downloadFlg = 0;
//		}
//		break;
//	}
//}
//
//void crDownloadedUpdatecfgLogic::addParam(int i, const std::string& str)
//{
//}
//void crDownloadedUpdatecfgLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
//		break;
//	}
//}
//void crDownloadedUpdatecfgLogic::outputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_needDownload = *(bool *)param;
//		break;
//	}
//}
//void crDownloadedUpdatecfgLogic::operator()(crHandle &handle)
//{
//	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(m_netType);
//	if(m_fileName && !m_fileName->empty() && m_checkAndDownloadFileMathod.valid())
//	{
//		crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
//		if(netConductor && data)
//		{
//			do 
//			{
//				std::string ext = crArgumentParser::getFileExtension(*m_fileName);
//				std::string file;
//				CRCore::Timer_t mtime;
//				m_needDownload = false;
//				bool needDownload = false;
//				if(ext.compare("cfg") == 0)
//				{
//					rcfg::ConfigScript cfg_script;
//					struct stat buf;
//					CRIOManager::crLoadCookFile scopedLoad(*m_fileName);
//					if (stat( m_fileName->c_str(), &buf )) 
//					{
//						CRCore::notify(CRCore::FATAL)<<*m_fileName<<" open error"<<std::endl;
//						break;
//					}
//					if (!cfg_script.Open(m_fileName->c_str()))
//					{
//						CRCore::notify(CRCore::FATAL)<<*m_fileName<<" read error "<<cfg_script.GetLastError()<<std::endl;
//						break;
//					}
//
//					int nFile = 1;
//					while(cfg_script.Push("File",nFile++))
//					{
//						cfg_script.Get("FileName", file);
//						if(!file.empty()&&file.compare("NULL")!=0)
//						{
//							std::string updatedate;
//							cfg_script.Get("UpdateDate", updatedate);
//							mtime = crArgumentParser::appAtoI64(updatedate);
//							m_checkAndDownloadFileMathod->inputParam(1,&file);
//							m_checkAndDownloadFileMathod->inputParam(2,&mtime);
//							m_checkAndDownloadFileMathod->inputParam(3,&m_downloadFlg);
//							(*m_checkAndDownloadFileMathod)(*this);
//							m_checkAndDownloadFileMathod->inputParam(1,NULL);
//							m_checkAndDownloadFileMathod->inputParam(2,NULL);
//							needDownload |= m_needDownload;
//						}
//						if (!cfg_script.Pop())
//							CRCore::notify(CRCore::FATAL)<<"crDownloadedUpdatecfgLogic::operator() "<<cfg_script.GetLastError()<<std::endl;
//					}
//				}
//				else
//				{//table
//					ref_ptr<crTableIO> updtab = new crTableIO;
//					if(!updtab->openFile(*m_fileName))
//					{
//						break;
//					}
//					int rowCount = updtab->getRowCount();
//					for(int i = 0; i<rowCount; i++)
//					{
//						file = (*updtab)(i,0);
//						mtime = crArgumentParser::appAtoI64((*updtab)(i,1));
//						if(!file.empty()&&file.compare("NULL")!=0)
//						{
//							m_checkAndDownloadFileMathod->inputParam(1,&file);
//							m_checkAndDownloadFileMathod->inputParam(2,&mtime);
//							m_checkAndDownloadFileMathod->inputParam(3,&m_downloadFlg);
//							(*m_checkAndDownloadFileMathod)(*this);
//							m_checkAndDownloadFileMathod->inputParam(1,NULL);
//							m_checkAndDownloadFileMathod->inputParam(2,NULL);
//							needDownload |= m_needDownload;
//						}
//					}
//				}
//				if(needDownload)
//				{
//					if(m_downloadFlg == DF_InitCheckEnd)
//					{
//						crInitCheckEndMethod::needUpdate(1);
//					}
//					crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
//					data->startDownloadFile(m_downloadFlg,m_netType);
//				}
//				else
//				{
//					crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,m_downloadFlg),NULL);
//				}
//				return;
//			} while (0);
//		}
//	}
//	crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,m_downloadFlg),NULL);
//}
/////////////////////////////////////////
//
//crRecvSceneInfoLogic
//
/////////////////////////////////////////
crRecvSceneInfoLogic::crRecvSceneInfoLogic():
m_needDownload(false){}
crRecvSceneInfoLogic::crRecvSceneInfoLogic(const crRecvSceneInfoLogic& handle):
crLogic(handle),
m_needDownload(false)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crRecvSceneInfoLogic::inputParam(int i, void *param)
{
}

void crRecvSceneInfoLogic::addParam(int i, const std::string& str)
{
}
void crRecvSceneInfoLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crRecvSceneInfoLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crRecvSceneInfoLogic::operator()(crHandle &handle)
{
	crQuerySceneInfoData *sceneInfoData = crMyPlayerData::getInstance()->getSceneInfoData();
	if(m_checkAndDownloadFileMathod.valid() && sceneInfoData)
	{//检查并下载scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		//if(gamedata) gamedata->lockHttpDownload();

		bool needDownload = false; 
		m_needDownload = false;
		int taskcount = 0;
		crDownloadManager::getInstance()->setCompleteCount(0);
		int downloadFlg = HttpD_EnterScene;
		if(crMyPlayerData::getInstance()->getSelectedRoom())
			downloadFlg = HttpD_EnterSceneRoom;

		std::string filename;
		std::string sceneFile = sceneInfoData->getFileName();
		std::string str = crArgumentParser::getFileNameEliminateExt(sceneFile) + ".crb";
		ref_ptr<crTableIO> table = crTableIO::openFile(str);
		int rowcount;
		if(table.valid())
		{
			rowcount = table->getRowCount();
			for (int i = 0; i < rowcount; i++)
			{
				filename = (*table)(i,0);
				m_checkAndDownloadFileMathod->inputParam(1,&filename);
				m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
				(*m_checkAndDownloadFileMathod)(*this);
				needDownload |= m_needDownload;
				if(m_needDownload)
					taskcount++;
			}
		}
		///下载CollideMap
		std::string collidefile = sceneInfoData->getCollideFile();
		rcfg::ConfigScript cfg_script;
		if(!crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,collidefile))
		{
			CRCore::notify(CRCore::FATAL)<<"crRecvSceneInfoLogic(): file open error,"<<collidefile<<std::endl;
			return;
		}
		int nLayer = 1;
		std::string collidemap;
		int nMap = 1;
		while (cfg_script.Push("Layer",nLayer++))
		{
			nMap = 1;
			while (cfg_script.Get("CollideMap", collidemap,nMap++))
			{
				collidemap = "scene/"+collidemap;
				m_checkAndDownloadFileMathod->inputParam(1,&collidemap);
				m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
				(*m_checkAndDownloadFileMathod)(*this);
				needDownload |= m_needDownload;
				if(m_needDownload)
					taskcount++;
			}
		}
		//下载场景.crs
		filename = sceneFile;
		m_checkAndDownloadFileMathod->inputParam(1,&filename);
		m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
		(*m_checkAndDownloadFileMathod)(*this);
		needDownload |= m_needDownload;
		if(m_needDownload)
			taskcount++;
		//下载UI.cru
		std::string uifile = crGlobalHandle::getInstance()->getUISceneDir()+sceneInfoData->getUIScene();//crMyPlayerData::getInstance()->getSelectedGame()->getUIFile();
		str = crArgumentParser::getFileNameEliminateExt(uifile) + ".crb";
		table = crTableIO::openFile(str);
		if(table.valid())
		{
			rowcount = table->getRowCount();
			for (int i = 0; i < rowcount; i++)
			{
				filename = (*table)(i,0);
				m_checkAndDownloadFileMathod->inputParam(1,&filename);
				m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
				(*m_checkAndDownloadFileMathod)(*this);
				needDownload |= m_needDownload;
				if(m_needDownload)
					taskcount++;
			}
		}
		filename = uifile;
		m_checkAndDownloadFileMathod->inputParam(1,&filename);
		m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
		(*m_checkAndDownloadFileMathod)(*this);
		needDownload |= m_needDownload;
		if(m_needDownload)
			taskcount++;
		//CRCore::notify(CRCore::ALWAYS)<<"DownloadSceneInfo "<<std::endl;
		if(!needDownload)
		{
			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,downloadFlg));
		}
		else if(gamedata)
		{
			char gbuf[256];
			sprintf(gbuf,"StartHttpDownloadTask HttpDownload:%d,GainID:%d\n\0",LOINT32(downloadFlg),HIINT32(downloadFlg));
			gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
			gamedata->startHttpDownloadTask(downloadFlg);
			crDownloadManager::getInstance()->setTaskCount(taskcount);
		}
		//if(gamedata) gamedata->unlockHttpDownload();
	}

	//if(sceneInfoData && m_checkAndDownloadFileMathod.valid())
	//{//检查并下载scene数据
	//	std::string filename;
 //       //CRCore::Timer_t fileModifyTime;
 //       
	//	unsigned short downloadFlg = DF_EnterScene;
	//	if(crMyPlayerData::getInstance()->getSelectedRoom())
	//		downloadFlg = DF_EnterSceneRoom;
	//	m_needDownload = false;
	//	bool needDownload = false;

	//	//crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_notifyStage);
	//	//if(stage && stage->getShow())
	//	//{
	//	//	crStaticTextBoxElement *downloadInfo = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_notifyTextElement));
	//	//	if(downloadInfo)
	//	//	{
	//	//		downloadInfo->setStringArrayByString("Check scene Files...");
	//	//		stage->setNeedReDraw();
	//	//	}
	//	//}

	//	//filename = sceneInfoData->getFileName();
	//	//fileModifyTime = sceneInfoData->getFileModifyTime();
	//	//m_checkAndDownloadFileMathod->inputParam(1,&filename);
	//	//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
	//	//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
	//	//(*m_checkAndDownloadFileMathod)(*this);
	//	//m_checkAndDownloadFileMathod->inputParam(1,NULL);
	//	//m_checkAndDownloadFileMathod->inputParam(2,NULL);
	//	//needDownload |= m_needDownload;

	//	//filename = crScene::getCollideFile(sceneInfoData->getFileName());
	//	//fileModifyTime = sceneInfoData->getCollideFileModifyTime();
	//	//m_checkAndDownloadFileMathod->inputParam(1,&filename);
	//	//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
	//	//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
	//	//(*m_checkAndDownloadFileMathod)(*this);
	//	//m_checkAndDownloadFileMathod->inputParam(1,NULL);
	//	//m_checkAndDownloadFileMathod->inputParam(2,NULL);
	//	//needDownload |= m_needDownload;

	//	//filename = crScene::getUpdatecfg(sceneInfoData->getFileName());
	//	//fileModifyTime = sceneInfoData->getUpdcfgModifyTime();
	//	//m_checkAndDownloadFileMathod->inputParam(1,&filename);
	//	//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
	//	//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
	//	//(*m_checkAndDownloadFileMathod)(*this);
	//	//m_checkAndDownloadFileMathod->inputParam(1,NULL);
	//	//m_checkAndDownloadFileMathod->inputParam(2,NULL);
	//	//needDownload |= m_needDownload;

	//	if(needDownload)
	//	{
	//		crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
	//		data->startDownloadFile(downloadFlg,GameClient_Game);
	//	}
	//	else
	//	{
	//		crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,downloadFlg),NULL);
	//	}
	//}
}
///////////////////////////////////////////
////
////crDownloadRoleFilesLogic
////
///////////////////////////////////////////
//crDownloadRoleFilesLogic::crDownloadRoleFilesLogic():
//m_needDownload(false){}
//crDownloadRoleFilesLogic::crDownloadRoleFilesLogic(const crDownloadRoleFilesLogic& handle):
//crLogic(handle),
//m_needDownload(false),
//m_notifyStage(handle.m_notifyStage),
//m_notifyTextElement(handle.m_notifyTextElement)
//{
//	if(handle.m_checkAndDownloadFileMathod.valid())
//		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
//}
//void crDownloadRoleFilesLogic::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_role = NULL;
//		}
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_role = (crRole *)(LOINT64(param64));
//		}
//		else
//		{
//			m_role = NULL;
//		}
//		break;
//	}
//}
//
//void crDownloadRoleFilesLogic::addParam(int i, const std::string& str)
//{
//	switch(i) 
//	{
//	case 0:
//		m_notifyStage = str;
//		break;
//	case 1:
//		m_notifyTextElement = str;
//		break;
//	}
//}
//void crDownloadRoleFilesLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
//		break;
//	}
//}
//void crDownloadRoleFilesLogic::outputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_needDownload = *(bool *)param;
//		break;
//	}
//}
//void crDownloadRoleFilesLogic::operator()(crHandle &handle)
//{
//	//crRole *role = crMyPlayerData::getInstance()->getRole();
//	if(m_role.valid() && m_checkAndDownloadFileMathod.valid())
//	{//检查并下载scene数据
//		std::string filename;
//		//std::string fileModifyTime;
//		
//		unsigned short downloadFlg = DF_CheckRoleUpdatecfg;
//
//		m_needDownload = false;
//		bool needDownload = false;
//
//		//crImageStage *stage = cr2DStageManager::getInstance()->findStage(m_notifyStage);
//		//if(stage && stage->getShow())
//		//{
//		//	crStaticTextBoxElement *downloadInfo = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_notifyTextElement));
//		//	if(downloadInfo)
//		//	{
//		//		downloadInfo->setStringArrayByString("Check role Files...");
//		//		stage->setNeedReDraw();
//		//	}
//		//}
//		////char mtime[20];
//		//CRCore::Timer_t fileModifyTime;
//		//filename = m_role->getAbstractItem()->getScript();
//		//fileModifyTime = m_role->getAbstractItem()->getScriptModifyTime();
//		//m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//(*m_checkAndDownloadFileMathod)(*this);
//		//m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//needDownload |= m_needDownload;
//
//		//crAbstractMetier *ametier = m_role->getAbstractMetier();
//		//if(ametier)
//		//{
//		//	filename = ametier->getMetierScript();
//		//	if(!filename.empty())
//		//	{
//		//		fileModifyTime = ametier->getMetierScriptModifyTime();
//		//		m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//		m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//		m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//		(*m_checkAndDownloadFileMathod)(*this);
//		//		m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//		m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//		needDownload |= m_needDownload;
//		//	}
//
//		//	filename = ametier->getTaskTable();
//		//	fileModifyTime = ametier->getTaskTableModifyTime();
//		//	m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//	m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//	m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//	(*m_checkAndDownloadFileMathod)(*this);
//		//	m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//	m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//	needDownload |= m_needDownload;
//		//}
//
//		//filename = m_role->getAbstractItem()->getResScript();
//		//fileModifyTime = m_role->getAbstractItem()->getResScriptModifyTime();
//		//m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//(*m_checkAndDownloadFileMathod)(*this);
//		//m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//needDownload |= m_needDownload;
//
//		//filename = m_role->getAbstractItem()->getFileName();
//		//fileModifyTime = m_role->getAbstractItem()->getFileModifyTime();
//		//m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//(*m_checkAndDownloadFileMathod)(*this);
//		//m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//needDownload |= m_needDownload;
//
//		//filename = m_role->getAbstractItem()->getUpdatecfg();
//		//fileModifyTime = m_role->getAbstractItem()->getUpdcfgModifyTime();
//		//m_checkAndDownloadFileMathod->inputParam(1,&filename);
//		//m_checkAndDownloadFileMathod->inputParam(2,&fileModifyTime);
//		//m_checkAndDownloadFileMathod->inputParam(3,&downloadFlg);
//		//(*m_checkAndDownloadFileMathod)(*this);
//		//m_checkAndDownloadFileMathod->inputParam(1,NULL);
//		//m_checkAndDownloadFileMathod->inputParam(2,NULL);
//		//needDownload |= m_needDownload;
//
//		if(needDownload)
//		{
//			crGameClientData *data = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
//			data->startDownloadFile(downloadFlg,GameClient_Game);
//		}
//		else
//		{
//			crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_RecvedFileStream,downloadFlg),NULL);
//		}
//	}
//}
/////////////////////////////////////////
//
//crDownloadItemLogic
//
/////////////////////////////////////////
crDownloadItemLogic::crDownloadItemLogic():
m_needDownload(false),
m_downloadFlg(HttpD_DownLoadItem){}
crDownloadItemLogic::crDownloadItemLogic(const crDownloadItemLogic& handle):
crLogic(handle),
m_needDownload(false),
m_downloadFlg(handle.m_downloadFlg)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crDownloadItemLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_item = NULL;
		}
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_item = (crInstanceItem *)(LOCREPARAM(param64));
			m_downloadFlg = (unsigned short)(HICREPARAM(param64));
		}
		else
		{
			m_item = NULL;
		}
		break;
	}
}

void crDownloadItemLogic::addParam(int i, const std::string& str)
{
}
void crDownloadItemLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crDownloadItemLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crDownloadItemLogic::operator()(crHandle &handle)
{
	if(m_item.valid() && m_checkAndDownloadFileMathod.valid())
	{//检查并下载scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		//if(gamedata) gamedata->lockHttpDownload();
		std::string filename;
		m_needDownload = false;
		bool needDownload = false;
		int downloadFlg = m_downloadFlg;
		unsigned short id = 0;
		if(m_downloadFlg == HttpD_DownLoadItem)
		{
			id = crGlobalHandle::gainHttpDownloadID();
			downloadFlg = MAKEINT32(m_downloadFlg,id);
			//crMyPlayerData::getInstance()->pushDownloadItem(m_item.get());
		}
		filename = m_item->getAbstractItem()->getFileName();
		if(!filename.empty())
		{
			std::string imgfile;
			std::string str = crArgumentParser::getFileNameEliminateExt(filename) + ".crb";
			ref_ptr<crTableIO> table = crTableIO::openFile(str);
			int rowcount;
			if(table.valid())
			{
				rowcount = table->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					imgfile = (*table)(i,0);
					m_checkAndDownloadFileMathod->inputParam(1,&imgfile);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
				}
			}
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
		}
		if(m_downloadFlg == HttpD_DownLoadItem)
		{
			if(!needDownload)
			{//不需要下载
				//crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,m_downloadFlg));
				crGlobalHandle::getInstance()->doEvent(WCH_LoadItem,MAKECREPARAM(m_item.get(),NULL));
				crGlobalHandle::recycleHttpDownloadID(id);
				char gbuf[256];
				sprintf(gbuf,"DownloadItem LoadItem %s\n\0",m_item->getIName().c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
			}
			else
			{
				char gbuf[256];
				sprintf(gbuf,"DownloadItem %s\n\0",m_item->getIName().c_str());
				gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
				crMyPlayerData::getInstance()->insertDownloadItem(id,m_item.get());
				if(gamedata)
				{
					//char gbuf[256];
					sprintf(gbuf,"StartHttpDownloadTask HttpDownload:%d,GainID:%d\n\0",LOINT32(downloadFlg),HIINT32(downloadFlg));
					gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
					gamedata->startHttpDownloadTask(downloadFlg);
				}
			}
		}
		else
		{
			if(!needDownload)
			{
				crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,downloadFlg));
			}
			else if(gamedata)
			{
				char gbuf[256];
				sprintf(gbuf,"StartHttpDownloadTask HttpDownload:%d,GainID:%d\n\0",LOINT32(downloadFlg),HIINT32(downloadFlg));
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
				gamedata->startHttpDownloadTask(downloadFlg);
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crDownloadItemLogic "<<std::endl;
		//if(gamedata) gamedata->unlockHttpDownload();
	}
	m_item = NULL;
}

/////////////////////////////////////////
//
//crNetMousePickLogic
//鼠标走路，拾取目标点，键盘走路通过预测目标点来做，当移动到离目标点距离<0.1的时候，重新预测目标点
/////////////////////////////////////////
crNetMousePickLogic::crNetMousePickLogic():
m_targetType(0)
{
}

crNetMousePickLogic::crNetMousePickLogic(const crNetMousePickLogic& handle):
crLogic(handle),
m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crNetMousePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		m_this = NULL;
		m_ea = NULL;
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crNetMousePickLogic::addParam(int i, const std::string& str)
{
}

void crNetMousePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crNetMousePickLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_TargetType:
		m_targetType = param?*((unsigned char*)param):0;
		break;
	case WCHDATA_TargetNode:
		m_targetNode = param?((crViewMatterObject *)(param)):NULL;
		break;
	case WCHDATA_TargetPos:
		m_targetPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	}
}

void crNetMousePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	crData *thisData = m_this->getDataClass();
	unsigned char flg = CF_Stop;
	thisData->inputParam(WCHDATA_ControllerFlg,&flg);

	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_this->getRelNode());
	if(!bot) return;
	void *param;
	thisData->getParam(WCHDATA_RTHP,param);
	float rthp  = *((float*)param);
	if(rthp <= 0) return;
	m_this->doEvent(WCH_NodeCoordToItem,MAKECREPARAM(NULL,bot));

	ref_ptr<crInstanceItem> lastTargetItem;
	ref_ptr<crMatrixTransform> lastTargetNode;
	m_this->clientGetTarget(lastTargetItem,lastTargetNode);

	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	(*m_pickTarget)(*this);
	thisData->inputParam(WCHDATA_TargetType,&m_targetType);
	//thisData->inputParam(WCHDATA_TargetNode,m_targetNode.get());
	
	//crVector3 targetPosition = m_targetPosition;
	unsigned char itemstate = IS_None;
	crInstanceItem *targetItem = NULL;
	int targetid = 0;
	int targetroleid = 0;
	if(m_targetType & Target_Instance || m_targetType & Target_Self || m_targetType & Target_StaticNpc/* || m_targetType & Target_StaticItem*/)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		targetItem = ((crInstanceItem*)param);
		if(targetItem)
		{
			targetid = targetItem->getID();
			targetroleid = targetItem->getRoleID();
			m_targetPosition.set(targetItem->getPosx(),targetItem->getPosy(),targetItem->getPosz());
			m_targetPosition *= crGlobalHandle::gData()->gUnitScale();
		}
		//if(dynamic_cast<crMatrixTransform *>(m_targetNode.get()))
		//{
		//	targetPosition = (dynamic_cast<crMatrixTransform *>(m_targetNode.get()))->getTrans();
		//}
	}
	thisData->inputParam(WCHDATA_TargetID,&targetid);
	thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
	thisData->inputParam(WCHDATA_TargetPos,&m_targetPosition);

	if(lastTargetItem.valid() && lastTargetItem != targetItem)
	{
		lastTargetItem->doEvent(WCH_UnTouch,MAKECREPARAM(NULL,NULL));
	}
	m_this->doEvent(WCH_ShowTargetInfo,MAKECREPARAM(targetItem,NULL));

	crVector3 myPos = bot->getTrans();
	crVector3 targetDir = m_targetPosition - myPos;
	targetDir[2] = 0.0f;
	ref_ptr<crStreamBuf> stream = new crStreamBuf;
    stream->createBuf(64);
	stream->_writeUChar(m_targetType);
	if(m_targetType & Target_Instance || m_targetType & Target_Self || m_targetType & Target_StaticNpc /*|| m_targetType & Target_StaticItem*/)
	{
		stream->_writeInt(targetid);//4
		if(m_targetType & Target_Role)
			stream->_writeInt(targetroleid);
	}
	else
		stream->_writeVec3(m_targetPosition);
	do 
	{
		char isEnemy = 0;
        if(targetItem)
		    m_this->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem,&isEnemy));
		//检查是否施法
	    thisData->getParam(WCHDATA_AboutToUseItemID,param);
		int itemid = *((int*)param);
		if(itemid>0)
		{
			crItemChild *itemChild = m_this->findChildItem(itemid);
			if(!itemChild || !itemChild->isLoaded())
			{
				itemstate = IS_Stop;
				stream->_writeUChar(itemstate);
				break;
			}
			crInstanceItem *item = itemChild->getInstanceItem();
			crData *itemData = item->getDataClass();
			itemData->getParam(WCHDATA_TargetType,param);
			unsigned char targetType = *(unsigned char *)param;
			if(targetType & m_targetType)
			{//对象符合
				if(isEnemy == -1)
				{
					if(targetType & Target_Friend)
					{//UI提示使用对象不符
						itemstate = IS_Stop;
						stream->_writeUChar(itemstate);
						break;
					}
				}
				else if(isEnemy == 1)
				{
					if(targetType & Target_Enemy)
					{//UI提示使用对象不符
						itemstate = IS_Stop;
						stream->_writeUChar(itemstate);
						break;
					}
				}
				else
				{
					break;
				}
				float dist = targetDir.length();
				itemData->getParam(WCHDATA_ItemUseRange,param);
				unsigned short itemUseRange = *((unsigned short*)param);
				float relRange = (float)itemUseRange * crGlobalHandle::gData()->gUnitScale();
				float followDistance = relRange-crGlobalHandle::gData()->gFollowDelta();
                thisData->inputParam(WCHDATA_FollowDistance,&followDistance);
				if(dist<=relRange)
				{//在使用范围内
					UseItemParam useItemPair;
					useItemPair.m_user = m_this;
					if(targetItem && (targetItem->getItemtype()==crInstanceItem::Npc || targetItem->getItemtype()==crInstanceItem::Role))
						useItemPair.m_target = targetItem;
					unsigned short useResult = UR_None;
					item->doEvent(MAKEINT64(WCH_UseItem,UT_Client),MAKECREPARAM(&useItemPair,&useResult));
					if(useResult == UR_Succeed)
					{
						thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);//
						//crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
						//bindview->showCursor("Default");
						itemstate = IS_UseItem;
						stream->_writeUChar(itemstate);
						stream->_writeInt(itemid);
						//useitem 可能会引起hp和mp的变化
						thisData->getParam(WCHDATA_RTHP,param);
						float rthp = *((float*)param);
						stream->_writeFloat(rthp);
						thisData->getParam(WCHDATA_RTMP,param);
						float rtmp = *((float*)param);
						stream->_writeFloat(rtmp);

						stream->_writeFloat(followDistance);
					}
					else
					{
                        thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);//
						//crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
						//bindview->showCursor("Default");
					}
				}
				else if(m_targetType == Target_Coord)
				{//移动到使用范围内,然后使用，如果是Target_Instance由PlayerAI可处理
					itemstate = IS_MoveToUseSkill;
					//crVector3 dir = targetDir.normalize();
					//crVector3 moveToPos = m_targetPosition - (dir * followDistance);
					crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
					//float speed = 0;
					//m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&speed,NULL));
					//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
					//bot->doEvent(MAKEINT64(WCH_NetMoveToTarget,NULL),MAKEINT64(&moveToPos,&relspeed));//移动到使用范围
					stream->_writeUChar(itemstate);
					stream->_writeFloat(followDistance);
					//stream->_writeVec3(moveToPos);
				}
				else
				{
					itemstate = IS_CloseTargetUseSkill;
					stream->_writeUChar(itemstate);
					stream->_writeFloat(followDistance);
					crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
				}
				break;
			}
			else
			{//UI提示使用对象不符
                itemstate = IS_Stop;
				stream->_writeUChar(itemstate);
				break;
			}
		}
		if(m_targetType & Target_Self)
		{//pick myself,停止当前动作，不改变转向
			itemstate = IS_Stop;
			stream->_writeUChar(itemstate);
			break;
		}
		//
		if(m_targetType == Target_Coord)
		{
			if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
			{
				itemstate = IS_Move;
				stream->_writeUChar(itemstate);
				crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
				//float speed = 0;
				//m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&speed,NULL));
				//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
				//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&m_targetPosition,&relspeed));
				break;
			}
		}
		if(!targetItem) break;
		if(isEnemy == -1)
		{
			//检查普通攻击
			thisData->getParam(WCHDATA_CurrentAttackID,param);
			itemid = *((int*)param);
			if(itemid>0)
			{
				crItemChild *itemChild = m_this->findChildItem(itemid);
				if(!itemChild || !itemChild->isLoaded())
					break;
				crInstanceItem *item = itemChild->getInstanceItem();
				crData *itemData = item->getDataClass();
				itemData->getParam(WCHDATA_TargetType,param);
				unsigned char targetType = *(unsigned char *)param;
				if(targetType & m_targetType)
				{//对象符合
					float dist = targetDir.length();
					itemData->getParam(WCHDATA_ItemUseRange,param);
					unsigned short itemUseRange = *((unsigned short*)param);
					float relRange = (float)itemUseRange * crGlobalHandle::gData()->gUnitScale();
					float followDistance = relRange - crGlobalHandle::gData()->gFollowDelta();
					thisData->inputParam(WCHDATA_FollowDistance,&followDistance);
					if(dist<=relRange)
					{//在使用范围内
						UseItemParam useItemParam;
						useItemParam.m_user = m_this;
						if(targetItem && (targetItem->getItemtype()==crInstanceItem::Npc || targetItem->getItemtype()==crInstanceItem::Role))
							useItemParam.m_target = targetItem;
						unsigned short useResult = UR_None;
						item->doEvent(MAKEINT64(WCH_UseItem,UT_Client),MAKECREPARAM(&useItemParam,&useResult));
						if(useResult == UR_Succeed)
						{
							itemstate = IS_UseItem;
							stream->_writeUChar(itemstate);
							stream->_writeInt(itemid);
							stream->_writeInt(m_this->getPosx());
							stream->_writeInt(m_this->getPosy());
							//useitem 可能会引起hp和mp的变化
							thisData->getParam(WCHDATA_RTHP,param);
							float rthp = *((float*)param);
							stream->_writeFloat(rthp);
							thisData->getParam(WCHDATA_RTMP,param);
							float rtmp = *((float*)param);
							stream->_writeFloat(rtmp);

							stream->_writeFloat(followDistance);
						}
					}
					else if(m_targetType == Target_Coord)
					{//移动到使用范围内,然后使用，如果是Target_Instance由PlayerAI可处理
						itemstate = IS_MoveToPos;
						crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
						//crVector3 dir = targetDir.normalize();
						//crVector3 moveToPos = m_targetPosition - (dir * followDistance);
						//float speed = 0;
						//m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&speed,NULL));
						//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
						//bot->doEvent(MAKEINT64(WCH_NetMoveToTarget,NULL),MAKEINT64(&moveToPos,&relspeed));//移动到使用范围
						stream->_writeUChar(itemstate);
						stream->_writeFloat(followDistance);
						//stream->_writeVec3(moveToPos);
					}
					else
					{
						itemstate = IS_CloseTarget;
						stream->_writeUChar(itemstate);
						stream->_writeFloat(followDistance);
						crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
					}
					break;
				}
			}
		}
		else
		{//检查是否有事件
			if(targetItem->hasHandle(WCH_Touch))
			{
				float dist = targetDir.length();
                crData *targetItemData = targetItem->getDataClass();
				targetItemData->getParam(WCHDATA_TouchDistance,param);
				unsigned short touchDistance = *(unsigned short *)param;
				float relDist = (float)touchDistance * crGlobalHandle::gData()->gUnitScale();
				float followDistance = relDist - crGlobalHandle::gData()->gFollowDelta();
				if(dist<=relDist)
				{//在使用范围内
                    targetItem->doEvent(WCH_Touch );
					itemstate = IS_Stop;
					stream->_writeUChar(itemstate);
				}
				else
				{
					thisData->inputParam(WCHDATA_FollowDistance,&followDistance);
					itemstate = IS_CloseTargetTouch;
					crMyPlayerData::getInstance()->getScene()->getPathFindingManager()->addPathFindRequest(m_this);
					//crVector3 dir = targetDir.normalize();
					//crVector3 moveToPos = m_targetPosition - (dir * followDistance);
					//float speed = 0;
					//m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&speed,NULL));
					//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
					//bot->doEvent(MAKEINT64(WCH_NetMoveToTarget,NULL),MAKEINT64(&moveToPos,&relspeed));//移动到使用范围
					stream->_writeUChar(itemstate);
					stream->_writeFloat(followDistance);
					//stream->_writeVec3(moveToPos);
				}
				break;
			}
		}
		itemstate = IS_Stop;
		stream->_writeUChar(itemstate);
		//float speed = 0;
		//m_this->doEvent(WCH_GetSpeed,MAKECREPARAM(&speed,NULL));
		//float relspeed = speed * crGlobalHandle::gData()->gUnitScale();
		//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&m_targetPosition,&relspeed));
	} while (0);
	if(itemstate == IS_None) return;
	//if(itemstate == IS_Stop)
	//{
	//	//float speed = 0.0f;
	//	//crVector3 nullpos;
	//	//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
	//}
	//else 
	if(itemstate < IS_Move)
	{
		//turn to target
		crVector3 dir = targetDir.normalize();
		m_this->setDir(dir);

		//float speed = 0.0f;
		//crVector3 nullpos;
		//bot->doEvent(WCH_NetMoveToTarget,MAKEINT64(&nullpos,&speed));//停止移动
		m_this->doEvent(WCH_ItemCoordToNode,MAKECREPARAM(bot,crMatterObject::MD_RotMatrix));//转向拾取点
	}
	//if(itemstate!=IS_UseItem)
	//{
	//	stream->_writeInt(m_this->getPosx());
	//	stream->_writeInt(m_this->getPosy());
	//	stream->_writeInt(m_this->getPosz());
	//	if(itemstate < IS_Move)
	//	{
	//		stream->_writeFloat(m_this->getDirx());
	//		stream->_writeFloat(m_this->getDiry());//4
	//		stream->_writeFloat(m_this->getDirz());
	//	}
	//}
	//else
	//{
	//	itemstate = IS_Stop;
	//}
	if(itemstate!=IS_UseItem)
		itemstate = IS_Stop;
	thisData->inputParam(WCHDATA_ItemState,&itemstate);
	crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
	if(netConductor)
	{
		crPlayerEventPacket packet;
		crPlayerEventPacket::buildRequestPacket(packet,m_this,WCH_NetControl,stream.get());
		netConductor->getNetManager()->sendPacket("all",packet);
		//CRCore::notify(CRCore::ALWAYS)<<"crNetMousePickLogic Send WCH_NetMousePick"<<std::endl;
	}
}
/////////////////////////////////////////
//
//crDownloadItemChildLogic
//
/////////////////////////////////////////
crDownloadItemChildLogic::crDownloadItemChildLogic():
m_needDownload(false){}
crDownloadItemChildLogic::crDownloadItemChildLogic(const crDownloadItemChildLogic& handle):
crLogic(handle),
m_needDownload(false)
{
	if(handle.m_checkAndDownloadFileMathod.valid())
		m_checkAndDownloadFileMathod = handle.m_checkAndDownloadFileMathod->clone();
}
void crDownloadItemChildLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_itemchild = NULL;
			m_rootitem = NULL;
		}
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_itemchild = (crItemChild *)(LOCREPARAM(param64));
			m_rootitem = (crInstanceItem *)(HICREPARAM(param64));
		}
		else
		{
			m_itemchild = NULL;
			m_rootitem = NULL;
		}
		break;
	}
}

void crDownloadItemChildLogic::addParam(int i, const std::string& str)
{
}
void crDownloadItemChildLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_checkAndDownloadFileMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crDownloadItemChildLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_needDownload = *(bool *)param;
		break;
	}
}
void crDownloadItemChildLogic::operator()(crHandle &handle)
{
	if(m_itemchild.valid() && m_checkAndDownloadFileMathod.valid())
	{//检查并下载scene数据
		crGameClientData *gamedata = dynamic_cast<crGameClientData *>(crGlobalHandle::getInstance()->getDataClass());
		//if(gamedata) gamedata->lockHttpDownload();

		std::string filename;
		m_needDownload = false;
		bool needDownload = false;
		unsigned short id = crGlobalHandle::gainHttpDownloadID();
		int downloadFlg = MAKEINT32(HttpD_DownLoadItemChild,id);
		//crMyPlayerData::getInstance()->pushDownloadItemChild(m_itemchild.get(),m_rootitem.get());
		crInstanceItem *item = m_itemchild->getInstanceItem();
		filename = item->getAbstractItem()->getFileName();
		if(!filename.empty())
		{
			std::string str = crArgumentParser::getFileNameEliminateExt(filename) + ".crb";
			ref_ptr<crTableIO> table = crTableIO::openFile(str);
			int rowcount;
			if(table.valid())
			{
				rowcount = table->getRowCount();
				for (int i = 0; i < rowcount; i++)
				{
					filename = (*table)(i,0);
					m_checkAndDownloadFileMathod->inputParam(1,&filename);
					m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
					(*m_checkAndDownloadFileMathod)(*this);
					needDownload |= m_needDownload;
				}
			}
			filename = item->getAbstractItem()->getFileName();
			m_checkAndDownloadFileMathod->inputParam(1,&filename);
			m_checkAndDownloadFileMathod->inputParam(2,&downloadFlg);
			(*m_checkAndDownloadFileMathod)(*this);
			needDownload |= m_needDownload;
		}
		if(!needDownload)
		{//不需要下载
			//crGlobalHandle::getInstance()->doEvent(MAKEINT64(WCH_DownloadTaskDone,downloadFlg));
			crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild,MAKECREPARAM(m_itemchild.get(),m_rootitem.get()));
			//crGlobalHandle::getInstance()->doEvent(WCH_LoadItemChild );//清理
			crGlobalHandle::recycleHttpDownloadID(id);
		}
		else
		{
			crMyPlayerData::getInstance()->insertDownloadItemChild(id,m_itemchild.get(),m_rootitem.get());
			if(gamedata)
			{
				char gbuf[256];
				sprintf(gbuf,"StartHttpDownloadTask HttpDownload:%d,GainID:%d\n\0",LOINT32(downloadFlg),HIINT32(downloadFlg));
				gDebugInfo->debugInfo(CRCore::ALWAYS,gbuf);
				gamedata->startHttpDownloadTask(downloadFlg);
			}
		}
		//CRCore::notify(CRCore::ALWAYS)<<"crDownloadItemChildLogic "<<std::endl;
		//if(gamedata) gamedata->unlockHttpDownload();
	}
	m_itemchild = NULL;
	m_rootitem = NULL;
}
/////////////////////////////////////////
//
//crBulletCollideLogic
//
/////////////////////////////////////////
crBulletCollideLogic::crBulletCollideLogic():
m_contactInfo(NULL){}
crBulletCollideLogic::crBulletCollideLogic(const crBulletCollideLogic& handle):
crLogic(handle),
m_contactInfo(NULL)
{
	if(handle.m_collideFxHandle.valid())
		m_collideFxHandle = handle.m_collideFxHandle->clone();
}
void crBulletCollideLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crBulletMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_collideNode = (crNode *)(LOCREPARAM(param64));
			m_contactInfo = (dContact *)(HICREPARAM(param64));
		}
		else
		{
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	}
}
void crBulletCollideLogic::addParam(int i, const std::string& str)
{
}
void crBulletCollideLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_collideFxHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crBulletCollideLogic::outputParam(int i, void *param)
{
}
void crBulletCollideLogic::operator()(crHandle &handle)
{
	//CRCore::notify(CRCore::ALWAYS)<<"crBulletCollideLogic"<<std::endl;
	if(m_this->getCollided())
	{
		bool taskCanceled = true;
		handle.outputParam(0,&taskCanceled);
		return;
	}
	if(m_collideNode.valid() && m_contactInfo && m_this->getBulletObject())
	{
		const crBulletObject *bulletObject = m_this->getBulletObject();

		crBulletObject::ExecutionPhysicsType pt = bulletObject->getExecutionPhysicsType();

		crBodyCaseMatterObject *bodyCase = dynamic_cast<crBodyCaseMatterObject *>(m_collideNode.get());
		if(!bodyCase && (bulletObject->getAttrMask() & crBulletObject::VanishWhenCollide))
		{//
			//if(m_this->getGeomID()==m_contactInfo->geom.g1)
			//{
			//	m_this->setFlying(false);
			//	m_this->setCollided(true);
			//}
			//else
			//{
			//	m_this->bulletGeomCollide();
			//}
			if(m_this->getTrackTarget() && m_collideNode != m_this->getTrackTarget())
			{
				crViewMatterObject *firer = const_cast<crViewMatterObject *>(m_this->getFireWeapon()->getFireBody());
				if(firer)
				{
					void *param;
					crData *data = firer->getDataClass();
					data->getParam(WCHDATA_Item,param);
					if(param)
					{
						ref_ptr<crInstanceItem> item = ((crInstanceItem*)param);
						if(item.valid())
						{
							if(item->getItemtype()==crInstanceItem::Role)
							{//Player
								firer->doEvent(WCH_TrackFireMiss,MAKECREPARAM(m_this,m_collideNode.get()));
							}
						}
					}
				}
			}
			m_this->setFlying(false);
			m_this->setCollided(true);
		}

		//////////effect
		crVector3 contactPos(m_contactInfo->geom.pos[0],m_contactInfo->geom.pos[1],m_contactInfo->geom.pos[2]);
		crVector3 contactNormal(m_contactInfo->geom.normal[0], m_contactInfo->geom.normal[1], m_contactInfo->geom.normal[2]);
		//contactPos = contactPos - contactNormal * m_contactInfo->geom.depth;
		if(m_collideFxHandle.valid())
		{
			crData *collideNodeData = m_collideNode->getDataClass();
			ref_ptr<crInstanceItem> hitItem;
			if(collideNodeData)
			{
				void *param;
				collideNodeData->getParam(WCHDATA_Item,param);
				hitItem = (crInstanceItem *)param;
			}
			m_collideFxHandle->inputParam(3,&contactPos);
			m_collideFxHandle->inputParam(WCHDATA_FXItem,hitItem.get());
			//m_collideFxHandle->inputParam(4,&contactNormal);
			(*m_collideFxHandle)(*this);
		}
		//crBulletObject::EffectAttrMask effectAttrMask = bulletObject->getEffectAttrMask();
		//std::string attrStr;
		//CRCore::crArgumentParser::readKeyValue(m_collideNode->getDescriptions(),"AttributeString",attrStr);
		//if(attrStr.empty()) attrStr = "stone";
		//std::string effectString = m_this->getName()+"_"+attrStr;
		//crParticleEffect *particleEffectNode;
		//if(effectAttrMask & crBulletObject::Collide_Particle)
		//{
		//	particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
		//	if(particleEffectNode)
		//	{
		//		particleEffectNode->setEffectPosition(contactPos);
		//		particleEffectNode->setEffectDirection(contactNormal);
		//		particleEffectNode->startEffect();
		//	}
		//}
		//if(effectAttrMask & crBulletObject::Collide_Particle2)
		//{
		//	particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString + "2");
		//	if(particleEffectNode)
		//	{
		//		particleEffectNode->setEffectPosition(contactPos);
		//		particleEffectNode->setEffectDirection(contactNormal);
		//		particleEffectNode->startEffect();
		//	}
		//}
		//if(effectAttrMask & crBulletObject::Collide_Particle3)
		//{
		//	particleEffectNode = crEffectParticleManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString + "3");
		//	if(particleEffectNode)
		//	{
		//		particleEffectNode->setEffectPosition(contactPos);
		//		particleEffectNode->setEffectDirection(contactNormal);
		//		particleEffectNode->startEffect();
		//	}
		//}
		//if(effectAttrMask & crBulletObject::Collide_Sequence)
		//{
		//	//CRCore::notify(CRCore::ALWAYS)<<"Collide_Sequence "<<effectString<< std::endl;
		//	crSequence *sequenceNode = crEffectSequenceManager::getInstance()->reuseOrgetOrCreateAnIdleEffectNodes(effectString);
		//	if(sequenceNode)
		//	{
		//		//CRCore::notify(CRCore::ALWAYS)<<"m_sequenceNode "<<effectString<< std::endl;
		//		sequenceNode->setEffectPosition(contactPos);
		//		sequenceNode->startEffect();
		//	}
		//}
		//if(effectAttrMask & crBulletObject::Collide_Sound)
		//{
		//	//m_soundState = CRAL::crSoundManager::instance()->pushSoundEvent(effectString);
		//	crSoundState *soundState = CRAL::crSoundManager::instance()->getOrCreateBotSoundState(m_this->getFireWeapon()->getFireBody()->getName(),effectString);
		//	if(soundState)
		//	{
		//		soundState->setPosition(contactPos);
		//		//CRCore::crVector3f listener;
		//		//CRAL::crSoundManager::instance()->getListener()->getPosition(listener[0],listener[1],listener[2]);
		//		//CRCore::crVector3f dir = collidePos - listener;
		//		//dir.normalize();
		//		//m_soundState->setDirection(dir);
		//		soundState->setPlay(true);
		//	}
		//}

		///////
		if(pt == crBulletObject::Type_Explode)
		{
			m_this->explode(contactPos);
		}
		else if(pt == crBulletObject::Type_Volume)
		{
			m_this->explode(contactPos);
		}
	}
}
/////////////////////////////////////////
//
//crBulletCollideSequenceLogic
//
/////////////////////////////////////////
crBulletCollideSequenceLogic::crBulletCollideSequenceLogic():
m_contactInfo(NULL),
m_loadNode(NULL){}
crBulletCollideSequenceLogic::crBulletCollideSequenceLogic(const crBulletCollideSequenceLogic& handle):
crLogic(handle),
m_contactInfo(NULL),
m_offsetTrans(handle.m_offsetTrans),
m_rotAngles(handle.m_rotAngles),
m_attrSeqMap(handle.m_attrSeqMap),
m_loadNode(NULL)
{
	for( AttrSeqMap::iterator itr = m_attrSeqMap.begin();
		 itr != m_attrSeqMap.end();
		 ++itr )
	{
		itr->second.second = NULL;
	}
	if(handle.m_loadAndCloneNodeHandle.valid())
		m_loadAndCloneNodeHandle = handle.m_loadAndCloneNodeHandle->clone();
}
void crBulletCollideSequenceLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crBulletMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_collideNode = (crNode *)(LOCREPARAM(param64));
			m_contactInfo = (dContact *)(HICREPARAM(param64));
		}
		else
		{
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	}
}
void crBulletCollideSequenceLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		CRCore::crArgumentParser::appAtoVec(str,m_offsetTrans);
		break;
	case 1:
		CRCore::crArgumentParser::appAtoVec(str,m_rotAngles);
		break;
	default:
		{
			std::string type,relStr;
			std::string::size_type comma = str.find_first_of(',');
			type = std::string(str.begin(),str.begin()+comma);
			relStr = std::string(str.begin()+comma + 1,str.end());
			if(!relStr.empty())
			    m_attrSeqMap[type] = FileSeqPair(relStr,(crSequence*)NULL);
		}
	}
}
void crBulletCollideSequenceLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadAndCloneNodeHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crBulletCollideSequenceLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadNode = (CRCore::crNode *)param;
		break;
	}
}
void crBulletCollideSequenceLogic::operator()(crHandle &handle)
{
	if(m_collideNode.valid() && m_contactInfo)
	{
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(m_collideNode->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "default";

		AttrSeqMap::iterator itr = m_attrSeqMap.find(attrStr);
		if(itr != m_attrSeqMap.end())
		{
            if(!itr->second.second.valid() && m_loadAndCloneNodeHandle.valid())
			{
				m_loadNode = NULL;
				m_loadAndCloneNodeHandle->inputParam(1,m_this->getParent(0));//parentgroup
				m_loadAndCloneNodeHandle->inputParam(2,&(itr->second.first));//filename
				m_loadAndCloneNodeHandle->inputParam(3,NULL);//Description
				(*m_loadAndCloneNodeHandle)(*this);
				m_loadAndCloneNodeHandle->inputParam(1,NULL);//parentgroup
				m_loadAndCloneNodeHandle->inputParam(2,NULL);//filename
                if(m_loadNode)
				{
					CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
					searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
					searchByClassNameVisitor.insertClassNameId("crSequence");
					searchByClassNameVisitor.insertClassNameId("crMatrixSequence");
					m_loadNode->accept(searchByClassNameVisitor);
					crSequence *sequence = dynamic_cast<crSequence *>(searchByClassNameVisitor.getResult());
					if(sequence)
					{
                        itr->second.second = sequence;
					}
				}
			}
			if(itr->second.second.valid())
			{//player sequence
				crVector3 contactPos(m_contactInfo->geom.pos[0],m_contactInfo->geom.pos[1],m_contactInfo->geom.pos[2]);
				//crVector3 contactNormal(m_contactInfo->geom.normal[0], m_contactInfo->geom.normal[1], m_contactInfo->geom.normal[2]);
				itr->second.second->lockEffect();
				itr->second.second->setEffectPosition(contactPos);
				itr->second.second->startEffect();
				itr->second.second->unlockEffect();
				//CRCore::notify(CRCore::NOTICE)<<"crBulletCollideSequenceLogic startEffect "<<itr->second.second.get()<< std::endl;
			}
		}
		else
		{
			CRCore::notify(CRCore::NOTICE)<<"crBulletCollideSequenceLogic 没有找到与属性 "<<attrStr<<" 的碰撞特效"<< std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crBulletCollideSequence2Logic
//
/////////////////////////////////////////
crBulletCollideSequence2Logic::crBulletCollideSequence2Logic():
m_contactInfo(NULL),
m_loadNode(NULL)
{
	m_scale.set(1.0f,1.0f,1.0f);
}
crBulletCollideSequence2Logic::crBulletCollideSequence2Logic(const crBulletCollideSequence2Logic& handle):
crLogic(handle),
m_contactInfo(NULL),
m_offsetTrans(handle.m_offsetTrans),
m_scale(handle.m_scale),
m_attrSeqMap(handle.m_attrSeqMap),
m_loadNode(NULL)
{
	for( AttrSeqMap::iterator itr = m_attrSeqMap.begin();
		itr != m_attrSeqMap.end();
		++itr )
	{
		itr->second.second.first = NULL;
		itr->second.second.second = NULL;
	}
	if(handle.m_loadAndCloneNodeHandle.valid())
		m_loadAndCloneNodeHandle = handle.m_loadAndCloneNodeHandle->clone();
}
void crBulletCollideSequence2Logic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crBulletMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_collideNode = (crNode *)(LOCREPARAM(param64));
			m_contactInfo = (dContact *)(HICREPARAM(param64));
		}
		else
		{
			m_collideNode = NULL;
			m_contactInfo = NULL;
		}
		break;
	}
}
void crBulletCollideSequence2Logic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		CRCore::crArgumentParser::appAtoVec(str,m_offsetTrans);
		break;
	case 1:
		CRCore::crArgumentParser::appAtoVec(str,m_scale);
		break;
	default:
		{
			std::string type,relStr;
			std::string::size_type comma = str.find_first_of(',');
			type = std::string(str.begin(),str.begin()+comma);
			relStr = std::string(str.begin()+comma + 1,str.end());
			if(!relStr.empty())
				m_attrSeqMap[type] = FileMatSeqPair(relStr,std::make_pair((crMatrixTransform*)NULL,(crSequence*)NULL));
		}
	}
}
void crBulletCollideSequence2Logic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadAndCloneNodeHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crBulletCollideSequence2Logic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadNode = (CRCore::crNode *)param;
		break;
	}
}
void crBulletCollideSequence2Logic::operator()(crHandle &handle)
{
	if(m_collideNode.valid() && m_contactInfo)
	{
		std::string attrStr;
		CRCore::crArgumentParser::readKeyValue(m_collideNode->getDescriptions(),"AttributeString",attrStr);
		if(attrStr.empty()) attrStr = "default";

		AttrSeqMap::iterator itr = m_attrSeqMap.find(attrStr);
		if(itr != m_attrSeqMap.end())
		{
			if(!itr->second.second.first.valid() && m_loadAndCloneNodeHandle.valid())
			{
				m_loadNode = NULL;
				unsigned short copyop = crCopyOp::DEEP_COPY_NODES|crCopyOp::DEEP_COPY_Callbacks;
				m_loadAndCloneNodeHandle->inputParam(1,m_this->getParent(0));//parentgroup
				m_loadAndCloneNodeHandle->inputParam(2,&(itr->second.first));//filename
				m_loadAndCloneNodeHandle->inputParam(3,NULL);//Description
				m_loadAndCloneNodeHandle->inputParam(4,&copyop);//Description
				(*m_loadAndCloneNodeHandle)(*this);
				m_loadAndCloneNodeHandle->inputParam(1,NULL);//parentgroup
				m_loadAndCloneNodeHandle->inputParam(2,NULL);//filename
				if(m_loadNode)
				{
					itr->second.second.first = dynamic_cast<crMatrixTransform *>(m_loadNode);
					CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
					searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
					searchByClassNameVisitor.insertClassNameId("crObject");
					searchByClassNameVisitor.insertClassNameId("crBillboard");
					m_loadNode->accept(searchByClassNameVisitor);
					crObject *obj = dynamic_cast<crObject *>(searchByClassNameVisitor.getResult());
					if(obj)
					{
						crSeqTextureTileCallback *seqcallback = dynamic_cast<crSeqTextureTileCallback *>(obj->getUpdateCallback("SeqTextureTile"));
						if(seqcallback)
						{
							crSequence *seq = seqcallback->getSequence();
							itr->second.second.second = seq;
						}
					}
				}
			}
			if(itr->second.second.first.valid())
			{//player sequence
				crVector3 contactPos(m_contactInfo->geom.pos[0],m_contactInfo->geom.pos[1],m_contactInfo->geom.pos[2]);
				crMatrix mat;
				mat.makeScale(m_scale);
				mat.setTrans(contactPos + m_offsetTrans);
				itr->second.second.first->setMatrix(mat);
				itr->second.second.second->startEffect();
				//CRCore::notify(CRCore::NOTICE)<<"crBulletCollideSequenceLogic startEffect "<<itr->second.second.get()<< std::endl;
			}
		}
		else
		{
			CRCore::notify(CRCore::NOTICE)<<"crBulletCollideSequence2Logic 没有找到与属性 "<<attrStr<<" 的碰撞特效"<< std::endl;
		}
	}
}
/////////////////////////////////////////
//
//crWeaponFireLightLogic
//
/////////////////////////////////////////
crWeaponFireLightLogic::crWeaponFireLightLogic():
m_duration(0.1f){}
crWeaponFireLightLogic::crWeaponFireLightLogic(const crWeaponFireLightLogic& handle):
crNodeLogic(handle),
m_duration(handle.m_duration)
{
	if(handle.m_createLightHandle.valid())
		m_createLightHandle = handle.m_createLightHandle->clone();
}
void crWeaponFireLightLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crWeaponMatterObject*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_gunpoint = *((crVector3 *)(LOCREPARAM(param64)));
			m_dir = *((crVector3 *)(HICREPARAM(param64)));
		}
		break;
	}
}
void crWeaponFireLightLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_duration = atof(str.c_str());
		break;
	}
}
void crWeaponFireLightLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_createLightHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crWeaponFireLightLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_ls = (CRCore::crLightSource *)param;
		break;
	}
}
void crWeaponFireLightLogic::releaseObjects(CRCore::crState* state)
{
	crLightSourceManager::getInstance()->removeLightSource(m_ls.get());
    m_ls = NULL;
}
void crWeaponFireLightLogic::operator()(crHandle &handle)
{
	if(!m_ls.valid())
	{
		m_createLightHandle->inputParam(1,m_this->getParent(0));//parentgroup
		(*m_createLightHandle)(*this);
		m_createLightHandle->inputParam(1,NULL);

		if(m_duration>0.0f)
		{
			crLightEnableTimerCallback *callback = new crLightEnableTimerCallback;
			callback->setInterval(m_duration);
            m_ls->addUpdateCallback(callback);
		}
	}
	m_ls->setEffectPosition(m_gunpoint);
	m_ls->startEffect();
}
/////////////////////////////////////////
//
//crBulletFlyParticleLogic
//
/////////////////////////////////////////
crBulletFlyParticleLogic::crBulletFlyParticleLogic():
m_particleFlyOffset(0.0f){}
crBulletFlyParticleLogic::crBulletFlyParticleLogic(const crBulletFlyParticleLogic& handle):
crNodeLogic(handle),
m_particleFlyOffset(handle.m_particleFlyOffset),
m_filename(handle.m_filename)
{
	if(handle.m_loadParticleHandle.valid())
		m_loadParticleHandle = handle.m_loadParticleHandle->clone();
}
void crBulletFlyParticleLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crBulletMatterObject*)param;
		break;
	}
}
void crBulletFlyParticleLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_particleFlyOffset = atof(str.c_str());
		break;
	case 1:
		m_filename = str;
		break;
	}
}
void crBulletFlyParticleLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadParticleHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crBulletFlyParticleLogic::outputParam(int i, void *param)
{
}
void crBulletFlyParticleLogic::releaseObjects(CRCore::crState* state)
{
	if(m_particleEffectGroup.valid())
	{
		crLoadManager::getInstance()->requestRemoveNode(crSceneManager::getInstance()->getDynamicRoot(),m_particleEffectGroup.get());
		//m_particleEffectGroup->releaseObjects(state);
		//crEffectParticleManager::getInstance()->getEffectRoot()->removeChild(m_particleEffectGroup.get());
	    m_particleEffectGroup = NULL;
	}
}
void crBulletFlyParticleLogic::operator()(crHandle &handle)
{
	if(!m_particleEffectGroup.valid())
	{
		m_particleEffectGroup = new crMatrixTransform;
		m_loadParticleHandle->inputParam(1,m_particleEffectGroup.get());//parentgroup
		m_loadParticleHandle->inputParam(2,&m_filename);
		(*m_loadParticleHandle)(*this);
		m_loadParticleHandle->inputParam(1,NULL);
		m_loadParticleHandle->inputParam(2,NULL);
	}
	crVector3 position = m_this->getTrans();
	m_particleEffectGroup->setPosition(position);
	crParticleEffect *effect;
	CRCore::NodeArray& nodeArray = m_particleEffectGroup->getChildArray();
	for( CRCore::NodeArray::iterator itr = nodeArray.begin();
			itr != nodeArray.end();
			++itr )
	{
        effect = dynamic_cast<crParticleEffect *>(itr->get());
		//effect->setEffectPosition(position);
		effect->startEffect();
	}
}
///////////////////////////////////////////
////
////crNpcEnmityTestLogic
////
///////////////////////////////////////////
//crNpcEnmityTestLogic::crNpcEnmityTestLogic():
//m_dist(0.0f),
//m_isInPatrolHandle(false),
//m_enmity(0.0f){}
//crNpcEnmityTestLogic::crNpcEnmityTestLogic(const crNpcEnmityTestLogic& handle):
//crLogic(handle),
//m_dist(handle.m_dist),
//m_isInPatrolHandle(false),
//m_enmity(0.0f)
//{
//	if(handle.m_isInPatrolHandle.valid())
//		m_isInPatrolHandle = handle.m_isInPatrolHandle->clone();
//	if(handle.m_calcEnmityHandle.valid())
//		m_calcEnmityHandle = handle.m_calcEnmityHandle->clone();
//}
//void crNpcEnmityTestLogic::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_item = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_item = (crInstanceItem *)(LOCREPARAM(param64));
//			m_dist = *(float *)(HIINT64(param64));
//		}
//		else
//		{
//			m_item = NULL;
//			m_dist = 0.0f;
//		}
//		break;
//	}
//}
//void crNpcEnmityTestLogic::addParam(int i, const std::string& str)
//{
//}
//void crNpcEnmityTestLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_isInPatrolHandle = (CRCore::crHandle *)param;
//		break;
//	case 1:
//		m_calcEnmityHandle = (CRCore::crHandle *)param;
//		break;
//	}
//}
//void crNpcEnmityTestLogic::outputParam(int i, void *param)
//{
//	switch (i)
//	{
//	case 1:
//		m_isInPatrol = param?*(bool *)param:false;
//		break;
//	case 2:
//		m_enmity = param?*(float *)param:0.0f;
//		break;
//	}
//}
//void crNpcEnmityTestLogic::operator()(crHandle &handle)
//{
//	if(m_this && m_item)
//	{
//		void *param;
//		crData *data = m_this->getDataClass();
//
//		bool inRange = false;
//		bool lastInRange = false;
//		float guardRange = 1.0f;
//		
//		crData *itemData = m_item->getDataClass();
//		itemData->getParam(WCHDATA_RTHP,param);
//		if(param)
//		{
//			float rthp = *(float *)param;
//			if(rthp>0)
//			{
//				if(m_dist >= 0.0f)
//				{
//					data->getParam(WCHDATA_GuardRange,param);
//					guardRange = *((short*)param);
//					if(m_dist<(float)guardRange)
//					{
//						m_isInPatrol = false;
//						m_isInPatrolHandle->inputParam(1,m_this);
//						m_isInPatrolHandle->inputParam(2,m_item);
//						(*m_isInPatrolHandle)(*this);
//						m_isInPatrolHandle->inputParam(1,NULL);
//						m_isInPatrolHandle->inputParam(2,NULL);
//
//						inRange = m_isInPatrol;
//					}
//				}
//			}
//		}
//		data->excHandle(MAKEINT64(WCH_LockData,1));
//		if(m_item->getItemtype() == crInstanceItem::Role)
//		{
//			_crInt64 enmityid = MAKEINT64(m_item->getID(),m_item->getRoleID());
//			data->getParam(WCHDATA_PlayerEnmityMap,param);
//			PlayerEnmityMap* playerEnmityMap = (PlayerEnmityMap*)param;
//			PlayerEnmityMap::iterator itr = playerEnmityMap->find(enmityid);
//			lastInRange = itr != playerEnmityMap->end();
//
//			if(inRange)
//			{
//				//计算Enmity
//				m_enmity = 0.0f;
//				float distRate = 1.0f - m_dist / guardRange;
//				m_calcEnmityHandle->inputParam(1,&distRate);
//				(*m_calcEnmityHandle)(*this);
//				m_calcEnmityHandle->inputParam(1,NULL);
//				//if(lastInRange)
//				//	m_enmity += itr->second;
//				(*playerEnmityMap)[enmityid] = m_enmity; 
//			}
//			else if(lastInRange)
//			{
//				playerEnmityMap->erase(enmityid);
//			}
//		}
//		else if(m_item->getItemtype() == crInstanceItem::Npc)
//		{
//			int enmityid = m_item->getID();
//			data->getParam(WCHDATA_NpcEnmityMap,param);
//			NpcEnmityMap* npcEnmityMap = (NpcEnmityMap*)param;
//			NpcEnmityMap::iterator itr = npcEnmityMap->find(enmityid);
//			lastInRange = itr != npcEnmityMap->end();
//
//			if(inRange)
//			{
//				//计算Enmity
//				m_enmity = 0.0f;
//				float distRate = 1.0f - m_dist / guardRange;
//				m_calcEnmityHandle->inputParam(1,&distRate);
//				(*m_calcEnmityHandle)(*this);
//				m_calcEnmityHandle->inputParam(1,NULL);
//				//if(lastInRange)
//				//	m_enmity += itr->second;
//				(*npcEnmityMap)[enmityid] = m_enmity; 
//			}
//			else if(lastInRange)
//			{
//				npcEnmityMap->erase(enmityid);
//			}
//		}
//		data->excHandle(MAKEINT64(WCH_LockData,0));
//	}
//}
/////////////////////////////////////////
//
//crNpcAI1Logic
//
/////////////////////////////////////////
crNpcAI1Logic::crNpcAI1Logic():
m_dt(0.0f),m_enmityid(0),m_isAboutToUseItem(false),
m_aiInterval(0.5f),
m_aitime(0.0f){}
crNpcAI1Logic::crNpcAI1Logic(const crNpcAI1Logic& handle):
crLogic(handle),
m_dt(0.0f),
m_enmityid(0),
m_isAboutToUseItem(false),
m_aiInterval(handle.m_aiInterval),
m_aitime(0.0f)
{
	if(handle.m_getEnemy1Handle.valid())
		m_getEnemy1Handle = handle.m_getEnemy1Handle->clone();
	if(handle.m_selectSkillHandle.valid())
		m_selectSkillHandle = handle.m_selectSkillHandle->clone();
	if(handle.m_useItemAndSendHandle.valid())
		m_useItemAndSendHandle = handle.m_useItemAndSendHandle->clone();
	if(handle.m_patrolHandle.valid())
		m_patrolHandle = handle.m_patrolHandle->clone();
}
void crNpcAI1Logic::inputParam(int i, void *param)
{
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
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float *)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}
void crNpcAI1Logic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_aiInterval = atof(str.c_str());
		break;
	}
}
void crNpcAI1Logic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_getEnemy1Handle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_selectSkillHandle = (CRCore::crHandle *)param;
		break;
	case 2:
		m_useItemAndSendHandle = (CRCore::crHandle *)param;
		break;
	case 3:
		m_patrolHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crNpcAI1Logic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_enmityid = *(_crInt64 *)param;
		break;
	case 2:
		m_isAboutToUseItem = *(bool *)param;
		break;
	case 3:
		m_useResult = *(unsigned short *)param;
		break;
	}
}
void crNpcAI1Logic::operator()(crHandle &handle) 
{
	m_aitime += m_dt;
	if(m_aitime > m_aiInterval)
	{
		m_aitime = 0.0f;

		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *((unsigned char*)param);
		if(itemstate == IS_Dead)
			return;
		unsigned int guisestate = GS_Normal;
		m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
		{
			return;
		}
		//thisData->getParam(WCHDATA_GuiseState,param);
		//unsigned char guiseState = *(unsigned char *)param;
		//if(guiseState == GS_UnVisiable || guiseState == GS_StaticUnVisiable)
		//	return;

		m_enmityid = 0;
		m_getEnemy1Handle->inputParam(1,m_this);
		(*m_getEnemy1Handle)(*this);
		m_getEnemy1Handle->inputParam(1,NULL);
		if(m_enmityid!=0)
		{
			int playerid = LOINT64(m_enmityid);
			int roleid = HIINT64(m_enmityid);
			crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
			crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
			ref_ptr<crSceneServerPlayerData> playerData = dynamic_cast<crSceneServerPlayerData *>(netDataManager->getPlayerData(playerid));
			if(playerData.valid() && playerData->isClientRunning())
			{
				crRole *role = playerData->getRole(roleid);
				m_isAboutToUseItem = false;
				m_selectSkillHandle->inputParam(1,m_this);
				m_selectSkillHandle->inputParam(2,role);
				(*m_selectSkillHandle)(*this);
				m_selectSkillHandle->inputParam(1,NULL);
				m_selectSkillHandle->inputParam(2,NULL);

				if(m_isAboutToUseItem)
					thisData->getParam(WCHDATA_AboutToUseItemID,param);
				else
					thisData->getParam(WCHDATA_CurrentAttackID,param);
				int itemid = *((int*)param);
				unsigned char targetType = Target_Role;
				thisData->inputParam(WCHDATA_TargetType,&targetType);
				thisData->inputParam(WCHDATA_TargetID,&playerid);
				thisData->inputParam(WCHDATA_TargetRoleID,&roleid);
				m_useItemAndSendHandle->inputParam(1,m_this);
				m_useItemAndSendHandle->inputParam(2,&itemid);
				m_useItemAndSendHandle->inputParam(3,&m_isAboutToUseItem);
				(*m_useItemAndSendHandle)(*this);
				m_useItemAndSendHandle->inputParam(1,NULL);

				//crItemChild *itemChild = m_this->findChildItem(itemid);
				//crInstanceItem *item = itemChild->getInstanceItem();
				//crData *itemData = item->getDataClass();
				//itemData->getParam(WCHDATA_ItemUseRange,param);
				//unsigned short itemUseRange = *((unsigned short*)param);
				//float fitemUseRange = (float)itemUseRange * crGlobalHandle::gData()->gUnitScale();
				//float followDistance = fitemUseRange - crGlobalHandle::gData()->gFollowDelta();
				//thisData->inputParam(WCHDATA_FollowDistance,&followDistance);

				//unsigned char targetType = Target_Role;
				//thisData->inputParam(WCHDATA_TargetType,&targetType);
				//thisData->inputParam(WCHDATA_TargetID,&playerid);
				//thisData->inputParam(WCHDATA_TargetRoleID,&roleid);
				//crVector3 myPos = m_this->getPosition();
				//crVector3 targetPos = role->getPosition();
				//crVector3 targetDir = targetPos - myPos;
				//targetDir[2] = 0.0f;
				//float dist = targetDir.length();
				//unsigned char itemstate;
				//if(dist<=fitemUseRange)
				//{//在攻击范围内
				//	UseItemParam useItemParam;
				//	useItemParam.m_user = m_this;
				//	useItemParam.m_target = role;
				//	unsigned short useResult = UR_None;
				//	item->doEvent(MAKEINT64(WCH_UseItem,UT_Server),MAKEINT64(&useItemParam,&useResult));
				//	if(useResult == UR_Succeed)
				//	{
				//		if(m_isAboutToUseItem) 
				//		{
				//			thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);//
				//		}
				//		//sendpacket
				//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//		stream->createBuf(26);
				//		stream->_writeUChar(targetType);
				//		stream->_writeInt(playerid);//targetID
				//		stream->_writeInt(roleid);//targetRoleID

				//		stream->_writeUChar(IS_UseItem);
				//		stream->_writeInt(itemid);
				//		thisData->getParam(WCHDATA_RTHP,param);
				//		float rthp = *((float*)param);
				//		stream->_writeFloat(rthp);
				//		thisData->getParam(WCHDATA_RTMP,param);
				//		float rtmp = *((float*)param);
				//		stream->_writeFloat(rtmp);
				//		stream->_writeFloat(followDistance);
				//		//turn to target
				//		targetDir.normalize();
				//		m_this->setTargetDir(targetDir);
				//		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				//		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
				//		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				//		crItemEventPacket packet;
				//		crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_NetControl,stream.get());
				//		crScene *scene = callback->findScene(m_this->getSceneID());
				//		scene->sendPacketToItemNeighbor(m_this,packet);
				//		itemstate = IS_Stop;
				//		thisData->inputParam(WCHDATA_ItemState,&itemstate);
				//		//CRCore::notify(CRCore::FATAL)<<"crNpcAI1Logic:IS_UseItem "<<std::endl;
				//	}
				//	else if(guiseState != GS_Static)
				//	{
				//		itemstate = IS_Stop;
				//		thisData->inputParam(WCHDATA_ItemState,&itemstate);
				//		if(m_isAboutToUseItem)
				//		{
				//			thisData->inputParam(WCHDATA_AboutToUseItemID,NULL);//
				//		}
				//		ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//		stream->createBuf(1);
				//		stream->_writeUChar(itemstate);
				//		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				//		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
				//		crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				//		crItemEventPacket packet;
				//		crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_RecvItemState,stream.get());
				//		crScene *scene = callback->findScene(m_this->getSceneID());
				//		scene->sendPacketToItemNeighbor(m_this,packet);
				//		//CRCore::notify(CRCore::FATAL)<<"crNpcAI1Logic:IS_Stop dist"<<dist<<std::endl;
				//	}
				//}
				//else if(guiseState != GS_Static)
				//{//close to target
				//	itemstate = IS_CloseTarget;
				//	thisData->inputParam(WCHDATA_ItemState,&itemstate);
				//	thisData->getParam(WCHDATA_TargetPos,param);
				//	crVector3 curTargetPos = *(crVector3 *)param;
				//	curTargetPos[2] = targetPos[2];
				//	if( m_this->getOrCreatePathFinder()->getPathStatus()!=crInstanceItem::crPathFinder::Found||
				//		(curTargetPos - targetPos).length2() >= 1.0f)
				//	{
				//		thisData->inputParam(WCHDATA_TargetPos,&targetPos);
				//		crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				//		crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
				//		crSceneServerCallback *netCallback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				//		crScene *scene = netCallback->findScene(m_this->getSceneID());
				//		scene->getPathFindingManager()->addPathFindRequest(m_this);
				//		//CRCore::notify(CRCore::FATAL)<<"crNpcAI1Logic:IS_CloseTarget dist="<<dist<<" curTargetPos="<<curTargetPos<<" targetPos="<<targetPos<<std::endl;
				//	}
				//	//send to client
				//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
				//	stream->createBuf(14);
				//	stream->_writeUChar(Target_Role);
				//	stream->_writeInt(playerid);//targetID
				//	stream->_writeInt(roleid);//targetRoleID
				//	stream->_writeUChar(itemstate);
				//	stream->_writeFloat(followDistance);
				//	crNetConductor *sceneServerConductor = crNetContainer::getInstance()->getNetConductor(SceneServer);
				//	crNetDataManager *netDataManager = sceneServerConductor->getNetDataManager();
				//	crSceneServerCallback *callback = dynamic_cast<crSceneServerCallback *>(netDataManager->getNetCallback());
				//	crItemEventPacket packet;
				//	crItemEventPacket::buildRequestPacket(packet,0,m_this,WCH_NetControl2,stream.get());
				//	crScene *scene = callback->findScene(m_this->getSceneID());
				//	scene->sendPacketToItemNeighbor(m_this,packet);
				//	//CRCore::notify(CRCore::FATAL)<<"crNpcAI1Logic:IS_CloseTarget findPath "<<(int)(findPath)<<std::endl;
				//}
				return;
			}
		}
		if(!(guisestate & GS_Static) && !(guisestate & GS_StaticNoneBlock))
		{
			ref_ptr<crInstanceItem> targetItem = m_this->serverGetTarget();
			if(targetItem.valid())
			{
				crData *targetData = targetItem->getDataClass();
				targetData->getParam(WCHDATA_ItemState,param);
				unsigned char targetState = *(unsigned char *)param;
				if(targetState == IS_Dead)
				{
					unsigned char targetType = Target_Coord;
					thisData->inputParam(WCHDATA_TargetType,&targetType);
				}
				else
				{
					//thisData->getParam(WCHDATA_SightRange,param);
					//short sightRange = *(unsigned short *)param;
					float sightRange = 0;
					m_this->doEvent(WCH_GetSightRange, MAKECREPARAM(&sightRange,NULL));
					float fsightRange = sightRange * crGlobalHandle::gData()->gUnitScale();
					crVector3 targetPos = targetItem->getPosition();
					crVector3 mypos = m_this->getPosition();
					if((targetPos - mypos).length()>fsightRange)
					{
						unsigned char targetType = Target_Coord;
						thisData->inputParam(WCHDATA_TargetType,&targetType);
					}
				}
			}
			else
			{
				if(m_patrolHandle.valid())
				{
					//CRCore::notify(CRCore::FATAL)<<"crNpcAI1Logic:Patrol"<<std::endl;
					m_patrolHandle->inputParam(1,m_this);
					(*m_patrolHandle)(*this);
					m_patrolHandle->inputParam(1,NULL);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crPlayerAI1Logic
//
/////////////////////////////////////////
crPlayerAI1Logic::crPlayerAI1Logic():
m_dt(0.0f),
m_interval(0.1f),
m_time(0.0f),
m_autoFightTime(0.0f),
m_autoFightInterval(0.0f),
m_selectTargetRange(10.0f),
m_attackFailed(0),
m_attackFailedRange(5)
{
	m_autoFightIntervalVec.set(0.5f,2.0f);
}
crPlayerAI1Logic::crPlayerAI1Logic(const crPlayerAI1Logic& handle):
crLogic(handle),
m_dt(0.0f),
m_interval(handle.m_interval),
m_time(0.0f),
m_autoFightTime(0.0f),
m_autoFightInterval(0.0f),
m_autoFightIntervalVec(handle.m_autoFightIntervalVec),
m_selectTargetRange(handle.m_selectTargetRange),
m_attackFailed(0),
m_attackFailedRange(handle.m_attackFailedRange)
{
	if(handle.m_useItemAndSendHandle.valid())
		m_useItemAndSendHandle = handle.m_useItemAndSendHandle->clone();
}
void crPlayerAI1Logic::inputParam(int i, void *param)
{
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
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_dt = *(float *)(LOCREPARAM(param64));
		}
		else
		{
			m_dt = 0.0f;
		}
		break;
	}
}
void crPlayerAI1Logic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_interval = atof(str.c_str());
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_autoFightIntervalVec);
		break;
	case 2:
		m_selectTargetRange = atof(str.c_str());
		break;
	case 3:
		m_attackFailedRange = atoi(str.c_str());
		break;
	}
}
void crPlayerAI1Logic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_useItemAndSendHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crPlayerAI1Logic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 3:
		m_attackResult = *(unsigned short *)param;
		break;
	}
}
void crPlayerAI1Logic::operator()(crHandle &handle) 
{
	if(!crMyPlayerData::getInstance()->ifItemIsMe(m_this))
	{
		return;
	}
	m_time += m_dt;
	if(m_time>m_interval)
	{
		m_autoFightTime += m_time;
		m_time = 0.0f;
		void *param;
		crData *thisData = m_this->getDataClass();
		thisData->getParam(WCHDATA_ItemState,param);
		unsigned char itemstate = *((unsigned char*)param);
		if(itemstate == IS_Dead)
			return;
		unsigned int guisestate = GS_Normal;
		m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
		if(guisestate & GS_UnVisiable || guisestate & GS_StaticUnVisiable)
		{
			return;
		}
		do 
		{
			crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_this->getRelNode());
			if(!bot) break;
			//检查是否施法
			thisData->getParam(WCHDATA_AboutToUseItemID,param);
			int itemid = *((int*)param);
			if(itemid!=0 && (itemstate == IS_MoveToUseSkill || itemstate == IS_CloseTargetUseSkill))
			{
				bool isAboutToUseItem = true;
				bool isClient = true;
				m_useItemAndSendHandle->inputParam(1,m_this);
				m_useItemAndSendHandle->inputParam(2,&itemid);
				m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
				m_useItemAndSendHandle->inputParam(4,&isClient);
				(*m_useItemAndSendHandle)(*this);
				m_useItemAndSendHandle->inputParam(1,NULL);

				m_autoFightTime = 0.0f;
				m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
				return;
			}
			else
			{
				bool autoFight = false;
				thisData->getParam(WCHDATA_TargetType,param);
				unsigned char targettype = *(unsigned char *)param;
				if(targettype&Target_Instance || targettype & Target_StaticNpc/* || targettype & Target_StaticItem*/)
				{//追踪并攻击目标
					ref_ptr<crInstanceItem> targetItem;
					ref_ptr<crMatrixTransform> targetNode;
					m_this->clientGetTarget(targetItem,targetNode);
					//thisData->getParam(WCHDATA_TargetNode,param);
					//crNode *targetNode = (crNode *)param;
					//if(targetNode)
					//{
					//	crData *targetData = targetNode->getDataClass();
					//	targetData->getParam(WCHDATA_Item,param);
					//	targetItem = (crInstanceItem*)param;
					//}
					if(targetItem.valid() && m_this != targetItem.get())
					{
						crData *targetItemData = targetItem->getDataClass();
						crVector3 targetPos = targetItem->getPosition();
						if(itemstate == IS_CloseTargetTouch)
						{
							targetItemData->getParam(WCHDATA_TouchDistance,param);
							unsigned short touchDistance = *(unsigned short *)param;
							float relDist = (float)touchDistance * crGlobalHandle::gData()->gUnitScale();
							crVector3 myPos(m_this->getPosx(),m_this->getPosy(),m_this->getPosz());
							myPos *= crGlobalHandle::gData()->gUnitScale();
							crVector3 targetDir = targetPos - myPos;
							targetDir[2] = 0;
							float dist = targetDir.length();
							if(dist<=relDist)
							{
								//itemstate = IS_Stop;
								//thisData->inputParam(WCHDATA_ItemState,&itemstate);
								////send itemstate to server
								//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
								//if(netConductor)
								//{
								//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
								//	stream->createBuf(1);
								//	stream->_writeUChar(itemstate);//1
								//	crPlayerEventPacket packet;
								//	crPlayerEventPacket::buildRequestPacket(packet,m_this->getRoleID(),WCH_RecvItemState,stream.get());
								//	netConductor->getNetManager()->sendPacket("all",packet);
								//}
								targetItem->doEvent(WCH_Touch );
							}
							break;
						}
						autoFight = false;
						char isEnemy = 0;
						m_this->doEvent(WCH_EnemyCheck,MAKECREPARAM(targetItem.get(),&isEnemy));
						if(isEnemy == -1)
						{
							unsigned int guisestate = GS_Normal;
							targetItem->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate,NULL));
							if(guisestate & GS_StaticUnVisiable || guisestate & GS_UnVisiable || !targetNode->getVisiable())
							{
								autoFight = true;
							}
							//targetItemData->getParam(WCHDATA_GuiseState,param);
							//if(param)
							//{
							//	unsigned char guiseState = *(unsigned char *)param;
							//	if(guiseState == GS_UnVisiable || guiseState == GS_StaticUnVisiable || guiseState == GS_Hide)
							//	{//目标不可见
							//		//targettype = Target_Coord;
							//		//thisData->inputParam(WCHDATA_TargetType,&targettype);
							//		//itemstate = IS_Stop;
							//		//thisData->inputParam(WCHDATA_ItemState,&itemstate);
							//		////send to net
							//		//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
							//		//if(netConductor)
							//		//{
							//		//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
							//		//	stream->createBuf(2);
							//		//	stream->_writeUChar(itemstate);//1
							//		//	stream->_writeUChar(targettype);//1
							//		//	crPlayerEventPacket packet;
							//		//	crPlayerEventPacket::buildRequestPacket(packet,m_this->getRoleID(),WCH_RecvItemState,stream.get());
							//		//	netConductor->getNetManager()->sendPacket("all",packet);
							//		//}
							//		//return;
							//		autoFight = true;
							//	}
							//}
							if(!autoFight)
							{
								thisData->getParam(WCHDATA_CurrentAttackID,param);
								itemid = *((int*)param);
								bool isAboutToUseItem = false;
								m_useItemAndSendHandle->inputParam(1,m_this);
								m_useItemAndSendHandle->inputParam(2,&itemid);
								m_useItemAndSendHandle->inputParam(3,&isAboutToUseItem);
								(*m_useItemAndSendHandle)(*this);
								m_useItemAndSendHandle->inputParam(1,NULL);
								//CRCore::notify(CRCore::ALWAYS)<<"crPlayerAI1Logic::operator() m_closeAndFightHandle"<<std::endl;
								if(m_attackResult == UR_OverRange)
								{
									m_attackFailed++;
									if(m_attackFailed>m_attackFailedRange)
									{
										autoFight = true;
										m_attackFailed = 0;
									}
								}
								else
								{
									m_attackFailed = 0;
									m_autoFightTime = 0.0f;
									m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
									return;
								}
							}
						}
						else
						{//跟随状态
							autoFight = true;
						}
					}
					else
					{
						autoFight = true;
					}
				}
				if(itemstate == IS_Stop || itemstate == IS_AttackToPos || autoFight)
				{//自动选择附近的可攻击目标
					if(m_autoFightTime > m_autoFightInterval)
					{
						CRNetApp::crScene *scene = crMyPlayerData::getInstance()->getScene();
						crSceneLayer *scenelayer = scene->getSceneLayer(m_this->getLayerID());
						crVector2s coord = scenelayer->getCoord(m_this->getPosx()*crGlobalHandle::gData()->gUnitScale(),m_this->getPosy()*crGlobalHandle::gData()->gUnitScale());
						unsigned char mapProtected = scenelayer->getMapProtected(coord[0],coord[1]);
						if(mapProtected != MP_Normal)
						{
							break;
						}
						//判断是否需要回血回蓝
						//随机技能
						//选择敌人
						//crMyPlayerData::ItemNpcPair itemnpc = crMyPlayerData::getInstance()->getOneInRangeNpc(m_this,m_selectTargetRange);
						crInstanceItem *enemy = crMyPlayerData::getInstance()->getOneInRangeEnemy(m_this);
						if(enemy)
						{
							//char isEnemy = 0;
							//m_this->doEvent(WCH_EnemyCheck,MAKECREPARAM(itemnpc.first.get(),&isEnemy));
							//if(isEnemy == -1)
							//{
								bool isAboutToUseItem = false;
								//m_selectSkillHandle->inputParam(1,m_this);
								//m_selectSkillHandle->inputParam(2,playerData->getRole());
								//(*m_selectSkillHandle)(*this);
								//m_selectSkillHandle->inputParam(1,NULL);
								//m_selectSkillHandle->inputParam(2,NULL);
								itemstate = isAboutToUseItem?IS_CloseTargetUseSkill:IS_CloseTarget;
								thisData->inputParam(WCHDATA_ItemState,&itemstate);
								if(isAboutToUseItem)
									thisData->getParam(WCHDATA_AboutToUseItemID,param);
								else
									thisData->getParam(WCHDATA_CurrentAttackID,param);
								int itemid = *((int*)param);
								crItemChild *itemChild = m_this->findChildItem(itemid);
								if(!itemChild || !itemChild->isLoaded())
									break;
								crInstanceItem *item = itemChild->getInstanceItem();
								crData *itemData = item->getDataClass();
								itemData->getParam(WCHDATA_ItemUseRange,param);
								unsigned short itemUseRange = *((unsigned short*)param);
								float fitemUseRange = (float)itemUseRange * crGlobalHandle::gData()->gUnitScale();
								float followDistance = fitemUseRange - crGlobalHandle::gData()->gFollowDelta();
								thisData->inputParam(WCHDATA_FollowDistance,&followDistance);

								targettype = enemy->getItemtype()==crInstanceItem::Role?Target_Role:Target_Npc;
								thisData->inputParam(WCHDATA_TargetType,&targettype);
								//thisData->inputParam(WCHDATA_TargetNode,enemy->getRelNode());
								int targetid = enemy->getID();
								int targetroleid = enemy->getRoleID();
								thisData->inputParam(WCHDATA_TargetID,&targetid);
								thisData->inputParam(WCHDATA_TargetRoleID,&targetroleid);
								crVector3 targetPos = enemy->getPosition();
								thisData->inputParam(WCHDATA_TargetPos,&targetPos);
								//send to net
								//crNetConductor *netConductor = crNetContainer::getInstance()->getDynamicNetConductor(GameClient_Game);
								//if(netConductor)
								//{
								//	ref_ptr<crStreamBuf> stream = new crStreamBuf;
								//	stream->createBuf(14);
								//	stream->_writeUChar(targettype);
								//	if(targettype & Target_Instance)
								//		stream->_writeInt(targetid);
								//	if(targettype & Target_Role)
								//		stream->_writeInt(targetroleid);
								//	stream->_writeUChar(itemstate);
								//	stream->_writeFloat(followDistance);
								//	crPlayerEventPacket packet;
								//	crPlayerEventPacket::buildRequestPacket(packet,m_this->getRoleID(),WCH_NetControl2,stream.get());
								//	netConductor->getNetManager()->sendPacket("all",packet);
								//}
								m_autoFightTime = 0.0f;
								m_autoFightInterval = m_autoFightIntervalVec[0];//修改下一个进入自动战斗的时间
							//}
						}
					}
					return;
				}
			}
		}while(0);
		m_autoFightTime = 0.0f;
		m_autoFightInterval = m_autoFightIntervalVec[1];
	}
}
/////////////////////////////////////////
//
//crExtraEffectLogic
//
/////////////////////////////////////////
crExtraEffectLogic::crExtraEffectLogic():
m_init(false),
m_gs(0){}
crExtraEffectLogic::crExtraEffectLogic(const crExtraEffectLogic& handle):
crNodeLogic(handle),
m_init(false),
m_gs(handle.m_gs)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crHandle *>(handle.m_playSceneFx->clone());
}
void crExtraEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_bot = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_bot = (CRPhysics::crViewMatterObject*)(HICREPARAM(param64));
		}
		else
		{
			m_bot = NULL;
		}
		break;
	case WCHDATA_AttachDir:
		m_playSceneFx->inputParam(WCHDATA_AttachDir,param);
		break;
	}
}
void crExtraEffectLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_gs = 1 << (unsigned int)(atoi(str.c_str()));
		break;
	}
}
void crExtraEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crHandle *)param;
		break;
	}
}
void crExtraEffectLogic::outputParam(int i, void *param)
{
}
void crExtraEffectLogic::operator()(crHandle &handle)
{
	bool gscheck = true;
	if (m_gs)
	{
		unsigned int guisestate = GS_Normal;
		m_this->doEvent(WCH_GetGuiseState, MAKECREPARAM(&guisestate, NULL));
		gscheck = guisestate & m_gs;
	}
	if(!m_init && gscheck)
	{
 		m_playSceneFx->inputParam(WCHDATA_AttachToNode,m_bot.get());
		(*m_playSceneFx)(*this);
		m_init = true;
	}
	if (m_init && !gscheck)
	{
		releaseObjects(NULL);
		m_init = false;
	}
}
void crExtraEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
/////////////////////////////////////////
//
//crVolumeEffectLogic
//
/////////////////////////////////////////
crVolumeEffectLogic::crVolumeEffectLogic(){}
crVolumeEffectLogic::crVolumeEffectLogic(const crVolumeEffectLogic& handle):
crNodeLogic(handle),
m_positionOffset(handle.m_positionOffset)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crVolumeEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crVolumeNode*)param;
		break;
	}
}
void crVolumeEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	}
}
void crVolumeEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crVolumeEffectLogic::outputParam(int i, void *param)
{
}
void crVolumeEffectLogic::operator()(crHandle &handle)
{
	if(m_this->getEnable()&&m_playSceneFx.valid())
	{
		crVector3 position;
		const crBoundingBox &bbox = m_this->getBoundBox();
		if(dynamic_cast<crMatrixVolumeNode *>(m_this))
		{
			crBulletMatterObject *bullet = dynamic_cast<crBulletMatterObject *>(m_this->getMainBody());
			if(bullet)
			{
				//const crViewMatterObject *fireBot = bullet->getFireWeapon()->getFireBody();
				position = bullet->getGunpoint();
				crVector3 dir = bullet->getFireDirect();
				dir.normalize();
				m_playSceneFx->inputParam(4,&dir);
			}
			else
			{
				position = bbox.center();
				position[1] += bbox.yLength()*0.5f; 
				crMatrix rotation = dynamic_cast<crMatrixVolumeNode *>(m_this)->getEffectMatrix();
				rotation.setTrans(0.0f,0.0f,0.0f);
				crVector3 dir = (-Y_AXIS * rotation).normalize();
				m_playSceneFx->inputParam(4,&dir);
			}
		}
		else
		{
			position = bbox.center();
		}
		position += m_positionOffset;
		m_playSceneFx->inputParam(3,&position);
		(*m_playSceneFx)(*this);
	}
}
void crVolumeEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
///////////////////////////////////////////
////
////crHitTargetEffectLogic
////
///////////////////////////////////////////
//crHitTargetEffectLogic::crHitTargetEffectLogic(){}
//crHitTargetEffectLogic::crHitTargetEffectLogic(const crHitTargetEffectLogic& handle):
//	crNodeLogic(handle)
//{
//	if(handle.m_playSceneFx.valid())
//		m_playSceneFx = handle.m_playSceneFx->clone();
//}
//void crHitTargetEffectLogic::inputParam(int i, void *param)
//{
//	switch(i) 
//	{
//	case 0:
//		if(param == 0)
//		{//释放
//			m_this = NULL;
//			m_hitParam = NULL;
//		}
//		break;
//	case 1:
//		m_this = (crInstanceItem*)param;
//		break;
//	case 2:
//		if(param)
//		{
//			CREPARAM& param64 = *(CREPARAM*)param;
//			m_hitParam = (HitParam *)(LOINT64(param64));
//		}
//		else
//		{
//			m_hitParam = NULL;
//		}
//		break;
//	}
//}
//void crHitTargetEffectLogic::addParam(int i, const std::string& str)
//{
//}
//void crHitTargetEffectLogic::inputHandle(int i, void *param)
//{
//	switch (i)
//	{
//	case 0:
//		m_playSceneFx = (CRCore::crHandle *)param;
//		break;
//	}
//}
//void crHitTargetEffectLogic::outputParam(int i, void *param)
//{
//}
//void crHitTargetEffectLogic::operator()(crHandle &handle)
//{
//	if(m_hitParam->m_hitItem.valid() && m_hitParam->m_hitItem->getRelNode())
//	{
//		m_playSceneFx->inputParam(WCHDATA_AttachToNode,m_hitParam->m_hitItem->getRelNode());
//		(*m_playSceneFx)(*this);
//	}
//}
/////////////////////////////////////////
//
//crGameTaskCheckLogic
//
/////////////////////////////////////////
crGameTaskCheckLogic::crGameTaskCheckLogic():
m_taskid(0)
{
}

crGameTaskCheckLogic::crGameTaskCheckLogic(const crGameTaskCheckLogic& handle):
crLogic(handle),
m_taskid(handle.m_taskid)
{
	if(handle.m_referTaskHandle.valid())
		m_referTaskHandle = handle.m_referTaskHandle->clone();
	if(handle.m_showTalkStage0.valid())
		m_showTalkStage0 = handle.m_showTalkStage0->clone();
	if(handle.m_showTalkStage1.valid())
		m_showTalkStage1 = handle.m_showTalkStage1->clone();
	if(handle.m_showTalkStage2.valid())
		m_showTalkStage2 = handle.m_showTalkStage2->clone();
}

void crGameTaskCheckLogic::inputParam(int i, void *param)
{
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
	if(m_referTaskHandle.valid())
		m_referTaskHandle->inputParam(i,param);
	if(m_showTalkStage0.valid())
		m_showTalkStage0->inputParam(i,param);
	if(m_showTalkStage1.valid())
		m_showTalkStage1->inputParam(i,param);
	if(m_showTalkStage2.valid())
		m_showTalkStage2->inputParam(i,param);
}

void crGameTaskCheckLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_taskid = atoi(str.c_str());
		break;
	}
}

void crGameTaskCheckLogic::inputHandle(int i, void *param)
{
	switch(i) 
	{
	case 0:
		m_referTaskHandle = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_showTalkStage0 = param?(CRCore::crHandle *)param:NULL;
		break;
	case 2:
		m_showTalkStage1 = param?(CRCore::crHandle *)param:NULL;
		break;
	case 3:
		m_showTalkStage2 = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crGameTaskCheckLogic::outputParam(int i, void *param)
{
}

void crGameTaskCheckLogic::operator()(crHandle &handle)
{
	void *param;
	crRole *me = crMyPlayerData::getInstance()->getCurrentRole();
	crData *metierData = me->getMetierDataClass();
	ref_ptr<crGameTask> gameTask;
	metierData->getParam(WCHDATA_GameTaskMap,param);
	metierData->excHandle(MAKEINT64(WCH_LockData,1));
	GameTaskMap* gameTaskMap = (GameTaskMap* )param;
	GameTaskMap::iterator itr = gameTaskMap->find(m_taskid);
	gameTask = itr!=gameTaskMap->end()?itr->second.get():NULL;
	metierData->excHandle(MAKEINT64(WCH_LockData,0));

	if(gameTask.valid())
	{
		//me->doEvent(WCH_SetKeyboardMouseMode,MAKEINT64(0,NULL));
		//crNode *node = (dynamic_cast<crGroup *>(me->getNode()))->getChild(0);
//         node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_KEYBOARDMOUSE),MAKECREPARAM(WCH_SetKeyboardMouseMode,0));
		if(gameTask->getActivation())
		{
			if(gameTask->isTaskCanRefer())
			{
				(*m_referTaskHandle)(*this);
			}
			if(gameTask->isComplete())
			{
				if(m_showTalkStage2.valid())
				{
					(*m_showTalkStage2)(*this);
				}
			}
			else
			{
				if(m_showTalkStage1.valid())
				{
					(*m_showTalkStage1)(*this);
				}
			}
		}
		else
		{
			if(m_showTalkStage0.valid())
			{
				(*m_showTalkStage0)(*this);
			}
		}
	}
}
/////////////////////////////////////////
//
//crWaitNetReturnStreamLogic
//
/////////////////////////////////////////
CRCore::ref_ptr<CRCore::crStreamBuf> crWaitNetReturnStreamLogic::s_netReturnStream;
CRCore::crCondition crWaitNetReturnStreamLogic::s_condition;
CRCore::crMutex crWaitNetReturnStreamLogic::s_mutex;
CRCore::crMutex crWaitNetReturnStreamLogic::s_protectMutex;
int crWaitNetReturnStreamLogic::s_lostcount = 0;
crWaitNetReturnStreamLogic::crWaitNetReturnStreamLogic():m_waitTime(10000){}
crWaitNetReturnStreamLogic::crWaitNetReturnStreamLogic(const crWaitNetReturnStreamLogic& handle):
crMethod(handle),
m_waitTime(handle.m_waitTime)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}
void crWaitNetReturnStreamLogic::inputParam(int i, void *param)
{
	if(m_handle.valid())
		m_handle->inputParam(i,param);
}
void crWaitNetReturnStreamLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_waitTime = atoi(str.c_str());
		if(m_waitTime <= 0) m_waitTime = INFINITE;
		else if(m_waitTime<10000)
		{
			m_waitTime = 10000;
		}
		break;
	}
}
void crWaitNetReturnStreamLogic::inputHandle(int i, void *param)
{
	switch(i) 
	{
	case 0:
		m_handle = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}
void crWaitNetReturnStreamLogic::outputParam(int i, void *param)
{
}
void crWaitNetReturnStreamLogic::getLock()
{
    s_protectMutex.lock();
	s_netReturnStream = NULL;
	//CRCore::notify(CRCore::ALWAYS)<<"crWaitNetReturnStreamLogic getLock"<<std::endl;
}
void crWaitNetReturnStreamLogic::netReturn(CRCore::crStreamBuf *stream)
{
	//while(!s_condition.haswaiters())
	//{
	//	//CRCore::crThread::yieldCurrentThread();
	//	//if(!s_protectMutex.trylock())
	//	//{
	//	//	s_protectMutex.unlock();
	//	//	return;
	//	//}
	//	return;
	//}
	if(s_lostcount>0)
	{
		s_lostcount--;
		return;
	}
	s_netReturnStream = stream;
	s_condition.signal();
}
void crWaitNetReturnStreamLogic::simpleWait(int ms)
{
	s_condition.wait(&s_mutex,ms);
	s_protectMutex.unlock();
}
void crWaitNetReturnStreamLogic::operator()(crHandle &handle)
{
	CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	bool sucess = !(s_condition.wait(&s_mutex,m_waitTime));
	CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"crWaitNetReturnStreamLogic wait = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<" success = "<<(int)sucess<<std::endl;
	if(sucess)
	{
		s_protectMutex.unlock();
		if(m_handle.valid())
		{
			m_handle->inputParam(3,s_netReturnStream.get());
			(*m_handle)(*this);
			m_handle->inputParam(3,NULL);
		}
	}
	else
	{
		s_lostcount++;
		s_protectMutex.unlock();
		//std::string str = "网络未响应";
		crGlobalHandle::getInstance()->doEvent(WCH_UINotify, MAKECREPARAM(27,NULL));
	}
	s_netReturnStream = NULL;
}
/////////////////////////////////////////
//
//crUseThingParticleLogic
//
/////////////////////////////////////////
crUseThingParticleLogic::crUseThingParticleLogic(){}
crUseThingParticleLogic::crUseThingParticleLogic(const crUseThingParticleLogic& handle):
crNodeLogic(handle),
m_filename(handle.m_filename)
{
	if(handle.m_loadParticleHandle.valid())
		m_loadParticleHandle = handle.m_loadParticleHandle->clone();
}
void crUseThingParticleLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_user = NULL;
		}
		break;
	case 1:
		m_this = (crInstanceItem*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_user = (crInstanceItem *)(LOCREPARAM(param64));
		}
		else
		{
			m_user = NULL;
		}
		break;
	}
}
void crUseThingParticleLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_filename = str;
		break;
	}
}
void crUseThingParticleLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadParticleHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crUseThingParticleLogic::outputParam(int i, void *param)
{
}
void crUseThingParticleLogic::releaseObjects(CRCore::crState* state)
{
	if(m_particleEffectGroup.valid())
	{
		crLoadManager::getInstance()->requestRemoveNode(crSceneManager::getInstance()->getDynamicRoot(),m_particleEffectGroup.get());
		//m_particleEffectGroup->releaseObjects(state);
		//crEffectParticleManager::getInstance()->getEffectRoot()->removeChild(const_cast<crMatrixTransform *>(m_particleEffectGroup.get()));
		m_particleEffectGroup = NULL;
	}
}
void crUseThingParticleLogic::operator()(crHandle &handle)
{
	if(m_user.valid())
	{
        crMatrixTransform *node = dynamic_cast<crMatrixTransform *>(m_user->getRelNode());
		if(node)
		{
			if(!m_particleEffectGroup.valid())
			{
				m_particleEffectGroup = new crMatrixTransform;
				ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
				ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
				ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");
				method->inputParam(3,node);
				//method->inputParam(4,&offset);
				driver->inputHandle(0,method.get());
				data->insertHandle(WCH_UPDATEVISITOR,driver.get());
				m_particleEffectGroup->setDataClass(data.get());

				m_loadParticleHandle->inputParam(1,m_particleEffectGroup.get());//parentgroup
				m_loadParticleHandle->inputParam(2,&m_filename);
				(*m_loadParticleHandle)(*this);
				m_loadParticleHandle->inputParam(1,NULL);
				m_loadParticleHandle->inputParam(2,NULL);
			}
			else
			{
                crHandle* method = m_particleEffectGroup->getHandle(WCH_UPDATEVISITOR,"AttachNodeUpdate");
				if(method)
				{
					method->inputParam(3,node);
				}
			}
			crParticleEffect *effect;
			CRCore::NodeArray& nodeArray = m_particleEffectGroup->getChildArray();
			for( CRCore::NodeArray::iterator itr = nodeArray.begin();
				itr != nodeArray.end();
				++itr )
			{
				effect = dynamic_cast<crParticleEffect *>(itr->get());
				effect->startEffect();
			}
			crVector3 position = node->getTrans();
			m_particleEffectGroup->setPosition(position);
		}
	}
}
/////////////////////////////////////////
//
//crSlotTimerLogic
//
/////////////////////////////////////////
crSlotTimerLogic::crSlotTimerLogic():
m_id(0),
m_interval(0),
m_inputid(0),
m_inputInterval(0)
{
}

crSlotTimerLogic::crSlotTimerLogic(const crSlotTimerLogic& handle):
crLogic(handle),
m_id(handle.m_id),
m_interval(handle.m_interval),
m_inputid(0),
m_inputInterval(0)
{
	if(handle.m_beginEvent.valid())
		m_beginEvent = handle.m_beginEvent->clone();
	if(handle.m_persistEvent.valid())
		m_persistEvent = handle.m_persistEvent->clone();
	if(handle.m_endEvent.valid())
		m_endEvent = handle.m_endEvent->clone();
}

void crSlotTimerLogic::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_updateVisitor = NULL;
		}
		break;
	case 1:
		m_node = (CRCore::crNode*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			_crInt32 param32 = LOCREPARAM(param64);
			m_inputid = (unsigned short)(LOINT32(param32));
			m_inputInterval = (unsigned short)(HIINT32(param32));
			m_updateVisitor = (CRUtil::crUpdateVisitor *)(HICREPARAM(param64));
		}
		else
		{
			m_inputid = 0;
			m_inputInterval = 0;
			m_updateVisitor = NULL;
		}
		break;
	}
	if(m_beginEvent.valid())
		m_beginEvent->inputParam(i,param);
	if(m_persistEvent.valid())
		m_persistEvent->inputParam(i,param);
	if(m_endEvent.valid())
		m_endEvent->inputParam(i,param);
}

void crSlotTimerLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msg = parseEventMsg16(str);
		break;
	case 1:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 2:
		m_id = (unsigned short)(atoi(str.c_str()));
		break;
	case 3:
		m_interval = atof(str.c_str());
		break;
	}
}
void crSlotTimerLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_beginEvent = (CRCore::crHandle *)param;
		break;
	case 1:
		m_persistEvent = (CRCore::crHandle *)param;
		break;
	case 2:
		m_endEvent = (CRCore::crHandle *)param;
		break;
	}
}
void crSlotTimerLogic::outputParam(int i, void *param)
{
}
void crSlotTimerLogic::operator()(crHandle &handle)
{
	if(m_node.valid() && m_updateVisitor.get() && m_inputid == m_id)
	{
		if(m_inputInterval == 0)
		{
			if(m_beginEvent.valid()) (*m_beginEvent)(*this);
		}
		
		float inputInterval = (float)m_inputInterval * 0.001f;
		if(inputInterval>m_interval)
		{
			m_inputid++;
			m_inputInterval = 0;
			if(m_endEvent.valid()) (*m_endEvent)(*this);
		}
		else
		{
			float dt = crFrameStamp::getInstance()->getFrameInterval();
			float interval = inputInterval + dt;
			m_inputInterval = (unsigned short)(interval * 1000.0f);
			if(m_persistEvent.valid()) (*m_persistEvent)(*this);
		}
		m_node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKECREPARAM(m_msg,MAKEINT32(m_inputid,m_inputInterval)));

		bool canceleTask = true;
		handle.outputParam(0,&canceleTask);
	}
	else if(m_inputid<m_id)
	{
		bool canceleTask = true;
		handle.outputParam(0,&canceleTask);
	}
}
/////////////////////////////////////////
//
//crSlotProgressLogic
//
/////////////////////////////////////////
char crSlotProgressLogic::s_advance = 0;
crSlotProgressLogic::crSlotProgressLogic():
m_id(0),
m_inputid(0)
{
	s_advance = 0;
}

crSlotProgressLogic::crSlotProgressLogic(const crSlotProgressLogic& handle):
crLogic(handle),
m_id(handle.m_id),
m_inputid(0)
{
	if(handle.m_beginEvent.valid())
		m_beginEvent = handle.m_beginEvent->clone();
	if(handle.m_persistEvent.valid())
		m_persistEvent = handle.m_persistEvent->clone();
	if(handle.m_endEvent.valid())
		m_endEvent = handle.m_endEvent->clone();
	s_advance = 0;
}

void crSlotProgressLogic::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_updateVisitor = NULL;
		}
		break;
	case 1:
		m_node = (CRCore::crNode*)param;
		break;
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			_crInt32 param32 = LOCREPARAM(param64);
			m_inputid = (unsigned short)(LOINT32(param32));
			m_updateVisitor = (CRUtil::crUpdateVisitor *)(HICREPARAM(param64));
		}
		else
		{
			m_inputid = 0;
			m_updateVisitor = NULL;
		}
		break;
	}
	if(m_beginEvent.valid())
		m_beginEvent->inputParam(i,param);
	if(m_persistEvent.valid())
		m_persistEvent->inputParam(i,param);
	if(m_endEvent.valid())
		m_endEvent->inputParam(i,param);
}

void crSlotProgressLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_msg = parseEventMsg16(str);
		break;
	case 1:
		m_msg = MAKEINT32(LOINT32(m_msg),parseEventMsg16(str));
		break;
	case 2:
		m_id = (unsigned short)(atoi(str.c_str()));
		break;
	}
}
void crSlotProgressLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_beginEvent = (CRCore::crHandle *)param;
		break;
	case 1:
		m_persistEvent = (CRCore::crHandle *)param;
		break;
	case 2:
		m_endEvent = (CRCore::crHandle *)param;
		break;
	}
}
void crSlotProgressLogic::outputParam(int i, void *param)
{
}
void crSlotProgressLogic::advance()
{
	if(s_advance == 1)
		s_advance = 2;
}
void crSlotProgressLogic::operator()(crHandle &handle)
{
	if(m_node.valid() && m_updateVisitor.get() && m_inputid == m_id)
	{
		if(s_advance == 0)
		{
			s_advance = 1;
			if(m_beginEvent.valid()) (*m_beginEvent)(*this);
		}
		else if(s_advance == 1)
		{
			if(m_persistEvent.valid()) (*m_persistEvent)(*this);
		}
		else if(s_advance == 2)
		{
			m_inputid++;
			s_advance = 0;
			if(m_endEvent.valid()) (*m_endEvent)(*this);
		}
		else
		{
            return;
		}
		m_node->doEvent(MAKEINT64(WCH_MSGCONTAINER,WCH_UPDATEVISITOR),MAKECREPARAM(m_msg,MAKEINT32(m_inputid,0)));

		bool canceleTask = true;
		handle.outputParam(0,&canceleTask);
	}
	else if(m_inputid<m_id)
	{
		bool canceleTask = true;
		handle.outputParam(0,&canceleTask);
	}
}
/////////////////////////////////////////
//
//crCreateSoundLogic
//
/////////////////////////////////////////
crCreateSoundLogic::crCreateSoundLogic():
m_ambient(true),
m_gain(1.0f),
m_play(true),
m_looping(false),
m_minDistance(10.0f),
m_maxDistance(50.0f),
m_pitch(1.0f),
m_inputPos(true),
m_dir(0.0f,0.0f,0.0f),
m_sound(NULL)
{}
crCreateSoundLogic::crCreateSoundLogic(const crCreateSoundLogic& handle):
crLogic(handle),
m_filename(handle.m_filename),
m_ambient(handle.m_ambient),
m_gain(handle.m_gain),
m_play(handle.m_play),
m_looping(handle.m_looping),
m_minDistance(handle.m_minDistance),
m_maxDistance(handle.m_maxDistance),
m_pitch(handle.m_pitch),
m_inputPos(handle.m_inputPos),
m_position(handle.m_position),
m_dir(handle.m_dir),
m_sound(NULL)
{
	if(handle.m_loadSoundHandle.valid())
		m_loadSoundHandle = handle.m_loadSoundHandle->clone();
}
void crCreateSoundLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		if(!m_ambient && m_inputPos && param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			if(LOCREPARAM(param64))
			{
				m_position = *((crVector3 *)(LOCREPARAM(param64)));
				//m_dir = *((crVector3 *)(LOINT64(param64)));
			}
		}
		break;
	}
}
void crCreateSoundLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		//m_createSoundNode = (bool)(atoi(str.c_str()));
		break;
	case 1:
		m_filename = str;
		break;
	case 2:
		m_ambient = (bool)(atoi(str.c_str()));
		break;
	case 3:
		m_gain = atof(str.c_str());
		break;
	case 4:
		m_play = (bool)(atoi(str.c_str()));
		break;
	case 5:
		m_looping = (bool)(atoi(str.c_str()));
		break;
	case 6:
		m_minDistance = atof(str.c_str());
		break;
	case 7:
		m_maxDistance = atof(str.c_str());
		break;
	case 8:
		m_pitch = atof(str.c_str());
		break;
	case 9:
		m_inputPos = (bool)(atoi(str.c_str()));
		break;
	case 10:
		CRCore::crArgumentParser::appAtoVec(str,m_position);
		break;
	case 11:
		CRCore::crArgumentParser::appAtoVec(str,m_dir);
		break;
	}
}
void crCreateSoundLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadSoundHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crCreateSoundLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_sound = (CRSound::crSound *)param;
		break;
	}
}
void crCreateSoundLogic::operator()(crHandle &handle)
{
	if(crDisplaySettings::instance()->getRunMode()>=crDisplaySettings::Robot)
		return;
	if(m_filename.find("#BKSOUND")==0)
	{
		crScene *scene = crMyPlayerData::getInstance()->getScene();
		ref_ptr<crTableIO> table = crGlobalHandle::gData()->gGlobalTable(WCHDATA_SceneTab);
		if(scene && table.valid())
		{
			int sceneid = scene->getSceneID();
			crTableIO::StrVec record;
			if(table->queryOneRecord(0,crArgumentParser::appItoa(sceneid),record)>=0)
			{
				int bksoundid = table->getTitleIndex("背景音乐");
				if(bksoundid>0)
				{
					std::string filename = record[bksoundid];
					m_loadSoundHandle->inputParam(1,&filename);//
					(*m_loadSoundHandle)(*this);
				}
			}
		}
	}
	else
	{
		m_loadSoundHandle->inputParam(1,&m_filename);//
		(*m_loadSoundHandle)(*this);
	}

	if(m_sound.valid())
	{
		if(m_ambient)
		{
			m_sound->createSound(FMOD_2D);
		}
		else
		{
			m_sound->createSound(FMOD_3D|FMOD_3D_LINEARROLLOFF);
			m_sound->getSound()->set3DMinMaxDistance(m_minDistance,m_maxDistance);//声音开始变小的距离,声音停止变小的距离
		}
		if(m_looping)
			m_sound->getSound()->setMode(FMOD_LOOP_NORMAL);
		else
			m_sound->getSound()->setMode(FMOD_LOOP_OFF);
		
		m_sound->playSound(/*FMOD_CHANNEL_FREE, */true);
		bool disableSound = crDisplaySettings::instance()->getDisableSound();
		if(!m_ambient)
		{
			m_sound->getChannel()->setVolume(disableSound?0.0f:m_gain * crDisplaySettings::instance()->getFxSoundVolume());
		}
		else if(!m_looping)
		{
			m_sound->getChannel()->setVolume(disableSound?0.0f:m_gain * crDisplaySettings::instance()->getAmbSoundVolume());
		}
		else
		{
			m_sound->getChannel()->setVolume(disableSound?0.0f:crDisplaySettings::instance()->getAmbSoundVolume());
			crSoundSystem::getInstance()->insertAmbSound(m_sound.get());
		}
		if(!m_ambient)
		{
			FMOD_VECTOR pos = { m_position[0], m_position[1], m_position[2] };
			FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };
			m_sound->getChannel()->set3DAttributes(&pos,&vel);
		}
		m_sound->getChannel()->setPaused(false);
		////m_sound->setGain(m_gain);
		//m_sound->setLooping(m_looping);
		//m_sound->setReferenceDistance(m_referenceDistance);
		//m_sound->setMaxDistance(m_maxDistance);
		//m_sound->setAmbient(m_ambient);
		//m_sound->setPitch(m_pitch);

		//if(!m_ambient)
		//{
		//	m_sound->setPosition(m_position);
		//	m_sound->setDirection(m_dir);
		//	m_sound->setGain(m_gain * crDisplaySettings::instance()->getFxSoundVolume());
		//}
		//else if(!m_looping)
		//{
		//	m_sound->setGain(m_gain * crDisplaySettings::instance()->getAmbSoundVolume());
		//}
		//else
		//{
		//	m_sound->setGain(crDisplaySettings::instance()->getAmbSoundVolume());
		//}
		//m_sound->setPlay(true);
		m_sound = NULL;
	}
}
/////////////////////////////////////////
//
//crPlaySceneFxLogic
//
/////////////////////////////////////////
crPlaySceneFxLogic::crPlaySceneFxLogic():
m_validTime(0.0f),
m_visiableTime(0.0f),
m_attachRotAngle(0.0f),
m_attachToThis(false),
m_ithisValid(false),
m_resetFX(false),
m_actFx(ACT_None){ m_attachScale.set(1.0f,1.0f,1.0f); }
crPlaySceneFxLogic::crPlaySceneFxLogic(const crPlaySceneFxLogic& handle):
crNodeLogic(handle),
m_fileName(handle.m_fileName),
m_validTime(handle.m_validTime),
m_visiableTime(handle.m_visiableTime),
m_loadNode(NULL),
m_position(handle.m_position),
m_dir(handle.m_dir),
m_attachToNode(NULL),
m_attachBoneName(handle.m_attachBoneName),
m_attachOffset(handle.m_attachOffset),
m_attachScale(handle.m_attachScale),
m_attachDir(handle.m_attachDir),
m_attachRotAngle(handle.m_attachRotAngle),
m_attachToThis(handle.m_attachToThis),
m_ithisValid(handle.m_ithisValid),
m_createNodeName(handle.m_createNodeName),
m_resetFX(handle.m_resetFX),
m_actFx(handle.m_actFx)
{
	if(handle.m_loadAndCloneNodeHandle.valid())
		m_loadAndCloneNodeHandle = handle.m_loadAndCloneNodeHandle->clone();
	if(handle.m_placeLoadedNodeHandle.valid())
		m_placeLoadedNodeHandle = handle.m_placeLoadedNodeHandle->clone();
}
void crPlaySceneFxLogic::inputParam(int i, void *param)
{
	switch (i)
	{
	//case 0:
	//	if(param == 0)
	//	{//释放
	//		m_this = NULL;
	//		m_ithis = NULL;
	//		if(m_loadAndCloneNodeHandle.valid())
	//			m_loadAndCloneNodeHandle->inputParam(0,NULL);
	//		if(m_placeLoadedNodeHandle.valid())
	//			m_placeLoadedNodeHandle->inputParam(0,NULL);
	//	}
	//	break;
	case 1:
		if(m_ithisValid)
			m_ithis = (crInstanceItem *)param;
		else
			m_this = (CRCore::crNode*)param;
		break;
	case 2:
		if(m_placeLoadedNodeHandle.valid())
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			if(LOCREPARAM(param64) && HICREPARAM(param64))
			{
				m_position = *((crVector3 *)(LOCREPARAM(param64)));
				m_dir = *((crVector3 *)(HICREPARAM(param64)));
			}
		}
		break;
	case 3:
		m_position = param?*(crVector3*)param:crVector3();
		break;
	case 4:
		m_dir = param?*(crVector3*)param:crVector3();
		break;
	case 5:
	case WCHDATA_FileName:
		if(param)
			m_fileName = *(std::string*)param;
		else
			m_fileName.clear();
		break;
	case WCHDATA_AttachToNode:
		m_attachToNode = param?(crNode*)param:NULL;
		break;
	case WCHDATA_AttachBoneName:
		if(param)
			m_attachBoneName = *(std::string*)param;
		else
			m_attachBoneName.clear();
		break;
	case WCHDATA_AttachOffset:
		m_attachOffset = param?*(crVector3*)param:crVector3();
		break;
	case WCHDATA_AttachScale:
		m_attachScale = param?*(crVector3*)param:crVector3(1.0f,1.0f,1.0f);
		break;
	case WCHDATA_AttachRotAngle:
		m_attachRotAngle = param?*(float*)param:0.0f;
		break;
	case WCHDATA_AttachDir:
		m_attachDir = param?*(crVector3*)param:crVector3(0.0f,0.0f,0.0f);
		break;
	case WCHDATA_ActFX:
		m_actFx = param?*(unsigned short*)param:ACT_None;
		break;
	}
}
void crPlaySceneFxLogic::addParam(int i, const std::string& str)
{
	std::string type,relStr;
	std::string::size_type comma = str.find_first_of(',');
	type = std::string(str.begin(),str.begin()+comma);
	relStr = std::string(str.begin()+comma + 1,str.end());
	int _type = atoi(type.c_str());
	switch(_type) 
	{
	case WCHDATA_FileName:
		m_fileName = relStr;
		break;
	case WCHDATA_ValidTime:
		m_validTime = atof(relStr.c_str());
		break;
	case WCHDATA_VisiableTime:
		m_visiableTime = atof(relStr.c_str());
		break;
	case WCHDATA_IthisValid:
		m_ithisValid = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_CreateNodeName:
		m_createNodeName = relStr;
		break;
	case WCHDATA_AttachToThis:
		m_attachToThis = (bool)(atoi(relStr.c_str()));
		break;
	case WCHDATA_AttachBoneName:
		m_attachBoneName = relStr;
		break;
	case WCHDATA_AttachOffset:
		crArgumentParser::appAtoVec(relStr,m_attachOffset);
		break;
	case WCHDATA_AttachScale:
		crArgumentParser::appAtoVec(relStr,m_attachScale);
		break;
	case WCHDATA_AttachRotAngle:
		m_attachRotAngle = atof(relStr.c_str());
		break;
	case WCHDATA_AttachDir:
		crArgumentParser::appAtoVec(relStr,m_attachDir);
		break;
	case WCHDATA_ResetFX:
		m_resetFX = atoi(relStr.c_str());
		break;
	case WCHDATA_ActFX:
		m_actFx = atoi(relStr.c_str());
		break;
	}
}
void crPlaySceneFxLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_loadAndCloneNodeHandle = (CRCore::crHandle *)param;
		break;
	case 1:
		m_placeLoadedNodeHandle = (CRCore::crHandle *)param;
		break;
	}
} 
void crPlaySceneFxLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadNode = (CRCore::crNode *)param;
		break;
	}
}
class ResetFXVisitor : public CRCore::crNodeVisitor
{
public:
	ResetFXVisitor():crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN){}
	virtual ~ResetFXVisitor(){}

	virtual void apply(CRParticle::crParticleEffect& node){ node.getEmitter()->setCurrentTime(0); node.getEmitter()->reset(); }
};
void crPlaySceneFxLogic::operator()(crHandle &handle)
{
	if(!m_fileName.empty())
	{
		if(!m_loadNode.valid() || m_validTime>0.0f || (m_actFx!=ACT_None&&!m_resetFX))
		{
			m_loadAndCloneNodeHandle->inputParam(1,crSceneManager::getInstance()->getDynamicRoot());//parentgroup
			m_loadAndCloneNodeHandle->inputParam(2,&m_fileName);//filename
			m_loadAndCloneNodeHandle->inputParam(3,NULL);//Description
			if(m_ithisValid)
				m_loadAndCloneNodeHandle->inputParam(5,m_ithis.get());//Description
			else
				m_loadAndCloneNodeHandle->inputParam(5,NULL);//Description
			m_loadAndCloneNodeHandle->inputParam(6,&m_createNodeName);//Description
			(*m_loadAndCloneNodeHandle)(*this);
			m_loadAndCloneNodeHandle->inputParam(1,NULL);//parentgroup
			m_loadAndCloneNodeHandle->inputParam(2,NULL);//filename
		}
		if(m_loadNode.valid())
		{
			if(m_attachToThis && (m_this||m_ithis.valid()))
			{
				if(m_ithisValid)
				{
					m_attachToNode = m_ithis->getRelNode();
				}
				else
				{
					m_attachToNode = m_this;
				}
			}
			if(m_attachToNode.valid())
			{
				if( !dynamic_cast<crMatrixTransform *>(m_loadNode.get()) && 
					!dynamic_cast<crLightSource *>(m_loadNode.get()) )
				{
					CRCore::ref_ptr<crGroup> parent = m_loadNode->getParent(0);
					ref_ptr<crMatrixTransform> nodeTransform = new crMatrixTransform;
					nodeTransform->setName(m_loadNode->getName());
					if(!m_loadNode->getBoundSetted() && m_loadNode->getName().compare("CreBodyRoot") != 0 && typeid(*m_loadNode)==typeid(CRCore::crGroup))
					{
						parent->replaceChildExt(m_loadNode.get(),nodeTransform.get());
					}
					else
					{
						parent->removeChild(m_loadNode.get());
						nodeTransform->addChild(m_loadNode.get());
						parent->addChild(nodeTransform.get());
					}
					m_loadNode = nodeTransform.get();
				}
				ref_ptr<crData> data = crDataManager::getInstance()->getData("Event");
				ref_ptr<crHandle> driver = crHandleManager::getInstance()->getHandle("OneSlot");
				ref_ptr<crHandle> method = crHandleManager::getInstance()->getHandle("AttachNodeUpdate");

				if(!m_attachBoneName.empty())
				{
					method->inputParam(7,&m_attachBoneName);
				}
				if( !m_attachBoneName.empty() &&
					m_attachBoneName.compare("BBOX_CENTER")!=0 &&
					m_attachBoneName.compare("BBOX_TOP")!=0 &&
					m_attachBoneName.compare("BBOX_BOTTOM")!=0 )
				{
					CRCore::crSearchNodeBYClassNameVisitor searchByClassNameVisitor;
					searchByClassNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
					searchByClassNameVisitor.insertClassNameId("CreBodyNode");
					m_attachToNode->accept(searchByClassNameVisitor);
					rbody::CreBodyNode *bodyNode = dynamic_cast<rbody::CreBodyNode *>(searchByClassNameVisitor.getResult());
					if(bodyNode)
					{
						m_attachToNode = bodyNode;
					}
				}
				crVector3 offset;
				method->inputParam(3,m_attachToNode.get());
				method->inputParam(4,&m_attachOffset);
				method->inputParam(5,&m_attachScale);
				float rotAngle = CRCore::DegreesToRadians(m_attachRotAngle);
				method->inputParam(6,&rotAngle);
				method->inputParam(8,&m_attachDir);
				driver->inputHandle(0,method.get());
				data->insertHandle(WCH_UPDATEVISITOR,driver.get());
				m_loadNode->setDataClass(data.get());

				if(m_actFx!=ACT_None)
				{
					void *param;
					crData *itemdata = m_attachToNode->getDataClass();
					itemdata->getParam(WCHDATA_Item,param);
					if(param)
					{
						ref_ptr<crInstanceItem> item = ((crInstanceItem*)param);
						if(item.valid())
						{
							crActFxCallback *callback = new crActFxCallback;
							callback->setActInstanceItem(item.get());
							callback->setAct(m_actFx);
							callback->setVisiableOrUnload(m_resetFX);
							m_loadNode->setUpdateCallback(callback);
						}
					}
				}
				m_attachToNode = NULL;
			}
			else if(m_placeLoadedNodeHandle.valid())
			{
				m_placeLoadedNodeHandle->inputParam(1,m_loadNode.get());
				m_placeLoadedNodeHandle->inputParam(2,&m_position);
				m_placeLoadedNodeHandle->inputParam(3,&m_dir);
				(*m_placeLoadedNodeHandle)(*this);
				m_placeLoadedNodeHandle->inputParam(0,NULL);
			}

			if(m_validTime>0.0f)
			{
				crUnloadTimerCallback *callback = new crUnloadTimerCallback;
				callback->setInterval(m_validTime);
				m_loadNode->setUpdateCallback(callback);
			}
			else if(m_visiableTime>0.0f)
			{
				crVisableTimerCallback *callback = dynamic_cast<crVisableTimerCallback *>(m_loadNode->getUpdateCallback("VisableTimer"));
				if(!callback)
				{
					callback = new crVisableTimerCallback;
					m_loadNode->setUpdateCallback(callback);
				}
				callback->setInterval(m_visiableTime);
			}

			if(m_resetFX)
			{
				ResetFXVisitor visitor;
				m_loadNode->accept(visitor);
			}
			m_loadNode->setVisiable(true);
			handle.outputParam(WCHDATA_LoadedNode,m_loadNode.get());
			//if(m_validTime>0.0f)
			//	m_loadNode = NULL;
		}
	}
}
void crPlaySceneFxLogic::releaseObjects(CRCore::crState* state)
{
	if(m_loadNode.valid())
	{
		ref_ptr<crUnloadTimerCallback> callback = dynamic_cast<crUnloadTimerCallback *>(m_loadNode->getUpdateCallback("UnloadTimer"));
		if(!callback.valid())
		{
			callback = new crUnloadTimerCallback;
			m_loadNode->setUpdateCallback(callback.get());
		}
		callback->setInterval(0.1f);
		m_loadNode = NULL;
	}
}
/////////////////////////////////////////
//
//crAttachCallbackMethod
//
/////////////////////////////////////////
crAttachCallbackMethod::crAttachCallbackMethod():
	m_ithisValid(false),
	m_callbacktype(0)
{}
crAttachCallbackMethod::crAttachCallbackMethod(const crAttachCallbackMethod& handle):
	crMethod(handle),
	m_ithisValid(handle.m_ithisValid),
	m_callbacktype(handle.m_callbacktype),
	m_callbackName(handle.m_callbackName),
	m_paramVec(handle.m_paramVec)
{
}
void crAttachCallbackMethod::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ithis = NULL;
		}
		break;
	case 1:
		if(m_ithisValid)
			m_ithis = (crInstanceItem *)param;
		else
			m_this = (CRCore::crNode*)param;
		break;
	case WCHDATA_AttachToNode:
		m_attachToNode = param?(crNode*)param:NULL;
		break;
	//case 2:
	//	if(param)
	//	{
	//		CREPARAM& param64 = *(CREPARAM*)param;
	//		if(LOINT64(param64))
	//		{
	//			m_position = *((crVector3 *)(LOINT64(param64)));
	//			m_dir = *((crVector3 *)(LOINT64(param64)));
	//		}
	//	}
	//	break;
	//case 3:
	//	m_position = param?*(crVector3*)param:crVector3();
	//	break;
	//case 4:
	//	m_dir = param?*(crVector3*)param:crVector3();
	//	break;
	}
}
void crAttachCallbackMethod::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_ithisValid = (bool)(atoi(str.c_str()));
		break;
	case 1:
		m_callbacktype = atoi(str.c_str());
		break;
	case 2:
		m_callbackName = str;
		break;
	default:
		m_paramVec.push_back(str);
		break;
	}
}
void crAttachCallbackMethod::operator()(crHandle &handle)
{
	if(!m_attachToNode.valid())
	{
		if(m_ithisValid)
			m_attachToNode = m_ithis->getRelNode();
		else
			m_attachToNode = m_this;
	}
	if(m_attachToNode.valid())
	{
		if(!m_callback.valid())
		{
			if(m_callbacktype==0)
			{
				m_callback = m_attachToNode->getUpdateCallback(m_callbackName);
				if(!m_callback.valid())
				{
					m_callback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(m_callbackName));
					m_attachToNode->setUpdateCallback(m_callback.get());
				}
			}
			else
			{
				m_callback = m_attachToNode->getCullCallback(m_callbackName);
				if(!m_callback.valid())
				{
					m_callback = dynamic_cast<crNodeCallback *>(crEventCallbackManager::getInstance()->getEventCallback(m_callbackName));
					m_attachToNode->setCullCallback(m_callback.get());
				}
			}
		}
		if(m_callback.valid())
		{
			int i = 0;
			for( ParamVec::iterator itr = m_paramVec.begin();
				itr != m_paramVec.end();
				++itr,i++ )
			{
				m_callback->addEventParam(i,*itr);
			}
		}
		handle.outputParam(WCHDATA_AttachToNode,m_attachToNode.get());
	}
}
void crAttachCallbackMethod::releaseObjects(CRCore::crState* state)
{
	if(m_callback.valid())
	{
		m_callback->releaseObjects(state);
	}
}
/////////////////////////////////////////
//
//crPlaceLoadedNode2Logic
//
/////////////////////////////////////////
crPlaceLoadedNode2Logic::crPlaceLoadedNode2Logic()
{
}

crPlaceLoadedNode2Logic::crPlaceLoadedNode2Logic(const crPlaceLoadedNode2Logic& handle):
	crLogic(handle),
	m_position(handle.m_position),
	m_dir(handle.m_dir)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
}

void crPlaceLoadedNode2Logic::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_loadedNode = NULL;
		}
		break;
	case 1:
		m_loadedNode = param?(crNode*)param:NULL;
		break;
	case 2:
		m_position = param?*(crVector3*)param:crVector3();
		break;
	case 3:
		m_dir = param?*(crVector3*)param:crVector3();
		break;
	}
}

void crPlaceLoadedNode2Logic::addParam(int i, const std::string& str)
{
}
void crPlaceLoadedNode2Logic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crPlaceLoadedNode2Logic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}

void crPlaceLoadedNode2Logic::operator()(crHandle &handle)
{
	if(m_loadedNode.valid())
	{
		ref_ptr<crMatrixTransform> matrixTransform = dynamic_cast<crMatrixTransform *>(m_loadedNode.get());
		if(!matrixTransform.valid())
		{
			m_convertToTransformMathod->inputParam(1,m_loadedNode.get());
			(*m_convertToTransformMathod)(*this);
			m_convertToTransformMathod->inputParam(1,NULL);
			if(m_convertedTransform.valid())
			{
				matrixTransform = m_convertedTransform.get();
				m_convertedTransform = NULL;
			}
			//matrixTransform = new crMatrixTransform;
			//matrixTransform->setName("PlaceLoadedNode");
			//m_loadedNode->getParent(0)->replaceChild(m_loadedNode.get(),matrixTransform);
			//matrixTransform->addChild(m_loadedNode.get());
		}
		if(matrixTransform.valid())
		{
			//const crMatrix &mat = matrixTransform->getMatrix();
			//crVector3f scale = mat.getScale();
			CRCore::crMatrix newMat;
			if(m_dir == crVector3(0.0f,0.0f,0.0f))
			{
				m_dir[1] = -1.0f;
			}
			if(m_dir[1] > 0.999)
			{
				newMat.makeRotate(-CRCore::Y_AXIS,m_dir);
				CRCore::crVector3d ep,center,up;
				newMat.getLookAt(ep,center,up);
				if(center[2]>0.0f)
				{
					center = -center;
				}
				newMat.makeLookAt(ep,center,up);
			}
			else
				newMat.makeRotate(-CRCore::Y_AXIS,m_dir);
			newMat.setTrans(m_position);

			matrixTransform->setMatrix(/*crMatrix::scale(scale) **/newMat);
			m_loadedNode = matrixTransform.get();
			handle.outputParam(1,m_loadedNode.get());
		}
	}
}
/////////////////////////////////////////
//
//crSelectRolePickLogic
//
/////////////////////////////////////////
crSelectRolePickLogic::crSelectRolePickLogic():
m_targetType(0)
{
}

crSelectRolePickLogic::crSelectRolePickLogic(const crSelectRolePickLogic& handle):
crLogic(handle),
m_targetType(0)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
}

void crSelectRolePickLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
			m_ea = NULL;
			m_targetNode = NULL;
		}
		break;
	case 1:
		m_this = (crRole*)param;
		break;
	case 2:
		m_param = *(CREPARAM*)param;
		m_ea = (crGUIEventAdapter *)(LOCREPARAM(m_param));
		break;
	}
}

void crSelectRolePickLogic::addParam(int i, const std::string& str)
{
}

void crSelectRolePickLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crSelectRolePickLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_TargetType:
		m_targetType = param?*((unsigned char*)param):0;
		break;
	case WCHDATA_TargetNode:
		m_targetNode = param?((crViewMatterObject *)(param)):NULL;
		break;
	case WCHDATA_TargetPos:
		m_targetPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	}
}

void crSelectRolePickLogic::operator()(crHandle &handle)
{
	if(!m_pickTarget.valid()) return;
	void *param;
	m_targetType = 0;
	m_targetNode = NULL;
	m_pickTarget->inputParam(2,&m_param);
	(*m_pickTarget)(*this);
	int targetid = 0;
	if(m_targetType & Target_Self)
	{
		crData *targetData = m_targetNode->getDataClass();
		targetData->getParam(WCHDATA_Item,param);
		crRole* targetRole = (crRole*)param;
		if(targetRole)
			crMyPlayerData::getInstance()->setSelectedGameRole(targetRole);
	}
}
/////////////////////////////////////////
//
//crCreateRoleInitLogic
//
/////////////////////////////////////////
crCreateRoleInitLogic::crCreateRoleInitLogic(){ m_attachScale.set(1.0f,1.0f,1.0f); }
crCreateRoleInitLogic::crCreateRoleInitLogic(const crCreateRoleInitLogic& handle):
	crLogic(handle),
	m_roletabfile(handle.m_roletabfile),
	m_attachToNodeName(handle.m_attachToNodeName),
	m_attachBoneName(handle.m_attachBoneName),
	m_attachOffset(handle.m_attachOffset),
	m_attachScale(handle.m_attachScale),
	m_attachRotAngle(handle.m_attachRotAngle)
{
	if(handle.m_addModelHandle.valid())
		m_addModelHandle = handle.m_addModelHandle->clone();
}
void crCreateRoleInitLogic::inputParam(int i, void *param)
{
}
void crCreateRoleInitLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_roletabfile = str;
		break;
	case 1:
		m_attachToNodeName = str;
		break;
	case 2:
		m_attachBoneName = str;
		break;
	case 3:
		crArgumentParser::appAtoVec(str,m_attachOffset);
		break;
	case 4:
		crArgumentParser::appAtoVec(str,m_attachScale);
		break;
	case 5:
		m_attachRotAngle = atof(str.c_str());
		break;
	}
}
void crCreateRoleInitLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_addModelHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crCreateRoleInitLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_loadedNode = (CRCore::crNode *)param;
		break;
	}
}
void crCreateRoleInitLogic::operator()(crHandle &handle)
{
	CRCore::ref_ptr<crTableIO> table = crTableIO::openFile(m_roletabfile);
	crData *data = crBrain::getInstance()->getDataClass();
	data->inputParam(WCHDATA_CreateRoleTab,table.get());
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_attachToNodeName);
	searchByNameVisitor.setSearchNodeType(TRANSFORM);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	crNode *node = dynamic_cast<crNode *>(searchByNameVisitor.getResult());
	data->inputParam(WCHDATA_AttachToNode,node);
	data->inputParam(WCHDATA_AttachBoneName,&m_attachBoneName);
	data->inputParam(WCHDATA_AttachOffset,&m_attachOffset);
	data->inputParam(WCHDATA_AttachScale,&m_attachScale);
	data->inputParam(WCHDATA_AttachRotAngle,&m_attachRotAngle);

	void *param;
	data->getParam(WCHDATA_CurrentRoleTabIndex,param);
	int index = *(int *)param;
	if(table.valid())
	{
		std::string filename = (*table)(index,2);
		if(m_addModelHandle.valid())
		{
			m_addModelHandle->inputParam(5,&filename);

			m_addModelHandle->inputParam(WCHDATA_AttachToNode,node);
			m_addModelHandle->inputParam(WCHDATA_AttachBoneName,&m_attachBoneName);
			m_addModelHandle->inputParam(WCHDATA_AttachOffset,&m_attachOffset);
			m_addModelHandle->inputParam(WCHDATA_AttachScale,&m_attachScale);
			m_addModelHandle->inputParam(WCHDATA_AttachRotAngle,&m_attachRotAngle);
			(*m_addModelHandle)(*this);

			data->inputParam(WCHDATA_LoadedNode,m_loadedNode.get());
			m_loadedNode = NULL;
		}
	}
}
/////////////////////////////////////////
//
//crCreateRoleSelectLogic
//
/////////////////////////////////////////
crCreateRoleSelectLogic::crCreateRoleSelectLogic():
m_left(false){}
crCreateRoleSelectLogic::crCreateRoleSelectLogic(const crCreateRoleSelectLogic& handle):
crLogic(handle),
m_left(handle.m_left),
m_headImageWidget(handle.m_headImageWidget)
{
	if(handle.m_addModelHandle.valid())
		m_addModelHandle = handle.m_addModelHandle->clone();
}
void crCreateRoleSelectLogic::inputParam(int i, void *param)
{
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
void crCreateRoleSelectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_left = (bool)(atoi(str.c_str()));
		break;
	case 1:
		m_headImageWidget = str;
		break;
	}
}
void crCreateRoleSelectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_addModelHandle = (CRCore::crHandle *)param;
		break;
	}
}
void crCreateRoleSelectLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case WCHDATA_LoadedNode:
		m_loadedNode = (CRCore::crNode *)param;
		break;
	}
}
void crCreateRoleSelectLogic::operator()(crHandle &handle)
{
	{
		ref_ptr<crCanvasNode> canvas = m_this->getParentCanvas();
		ref_ptr<crImageBoxWidgetNode> headImageWidget = dynamic_cast<crImageBoxWidgetNode *>(canvas->getWidget(m_headImageWidget));
		
		crData *data = crBrain::getInstance()->getDataClass();
		void *param;
		data->getParam(WCHDATA_LoadedNode,param);
		crNode *node = (crNode *)param;
		if(node)
		{
			crGroup *proxyNode = node->getParent(0);
			if(proxyNode->getNumParents()!=0)
			{
				crGroup *root = proxyNode->getParent(0);
				CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
			}
			else
			{
				return;
			}
		}
		data->getParam(WCHDATA_CreateRoleTab,param);
		crTableIO *table = (crTableIO *)param;
		if(table)
		{
			int rowCount = table->getRowCount();
			data->getParam(WCHDATA_CurrentRoleTabIndex,param);
			int* index = (int *)param;
			if(m_left)
				(*index)--;
			else
				(*index)++;
			if((*index)<0)
			{
				(*index) = rowCount-1;
			}
			else if((*index) >= rowCount)
			{
				(*index) = 0;
			}
			std::string filename = (*table)((*index),2);
			std::string iconfile = (*table)((*index),4);
			if(headImageWidget.valid())
			{
				headImageWidget->setImageName(iconfile);
			}
			if(m_addModelHandle.valid())
			{
				m_addModelHandle->inputParam(5,&filename);

				data->getParam(WCHDATA_AttachToNode,param);
				m_addModelHandle->inputParam(WCHDATA_AttachToNode,param);
				data->getParam(WCHDATA_AttachBoneName,param);
				m_addModelHandle->inputParam(WCHDATA_AttachBoneName,param);
				data->getParam(WCHDATA_AttachOffset,param);
				m_addModelHandle->inputParam(WCHDATA_AttachOffset,param);
				data->getParam(WCHDATA_AttachScale,param);
				m_addModelHandle->inputParam(WCHDATA_AttachScale,param);
				data->getParam(WCHDATA_AttachRotAngle,param);
				m_addModelHandle->inputParam(WCHDATA_AttachRotAngle,param);
				(*m_addModelHandle)(*this);

				data->inputParam(WCHDATA_LoadedNode,m_loadedNode.get());
				m_loadedNode = NULL;
			}
		}
	}
}
/////////////////////////////////////////
//
//crItemEffectLogic
//
/////////////////////////////////////////
crItemEffectLogic::crItemEffectLogic():
m_attachToNode(false){}
crItemEffectLogic::crItemEffectLogic(const crItemEffectLogic& handle):
	crNodeLogic(handle),
	m_positionOffset(handle.m_positionOffset),
	m_attachToNode(false)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crItemEffectLogic::inputParam(int i, void *param)
{
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
	}
}
void crItemEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	case 1:
		m_attachToNode = (bool)(atoi(str.c_str()));
		break;
	}
}
void crItemEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crItemEffectLogic::outputParam(int i, void *param)
{
}
void crItemEffectLogic::operator()(crHandle &handle)
{
	if(m_this->getRelNode())
	{
		if(m_attachToNode)
		{
			m_playSceneFx->inputParam(WCHDATA_AttachToNode,m_this->getRelNode());
			(*m_playSceneFx)(*this);
		}
		else
		{
			crVector3 position = m_this->getPosition();	
			position += m_positionOffset;
			m_playSceneFx->inputParam(3,&position);
			crVector3 dir = m_this->getDir();
			m_playSceneFx->inputParam(4,&dir);
			(*m_playSceneFx)(*this);
		}
	}
}
void crItemEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
/////////////////////////////////////////
//
//crParamItemEffectLogic
//
/////////////////////////////////////////
crParamItemEffectLogic::crParamItemEffectLogic():
	m_attachToNode(false){}
crParamItemEffectLogic::crParamItemEffectLogic(const crParamItemEffectLogic& handle):
	crNodeLogic(handle),
	m_positionOffset(handle.m_positionOffset),
	m_attachToNode(false)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crParamItemEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 2:
		if(param)
		{
			CREPARAM& param64 = *(CREPARAM*)param;
			m_item = (crInstanceItem*)(LOCREPARAM(param64));
		}
		else
		{
			m_item = NULL;
		}
		break;
	}
}
void crParamItemEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	case 1:
		m_attachToNode = (bool)(atoi(str.c_str()));
		break;
	}
}
void crParamItemEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crParamItemEffectLogic::outputParam(int i, void *param)
{
}
void crParamItemEffectLogic::operator()(crHandle &handle)
{
	if(m_item.valid() && m_item->getRelNode())
	{
		if(m_attachToNode)
		{
			m_playSceneFx->inputParam(WCHDATA_AttachToNode,m_item->getRelNode());
			(*m_playSceneFx)(*this);
		}
		else
		{
			crVector3 position = m_item->getPosition();	
			position += m_positionOffset;
			m_playSceneFx->inputParam(3,&position);
			crVector3 dir = m_item->getDir();
			m_playSceneFx->inputParam(4,&dir);
			(*m_playSceneFx)(*this);
		}
	}
}
void crParamItemEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
/////////////////////////////////////////
//
//crWeaponFireEffectLogic
//
/////////////////////////////////////////
crWeaponFireEffectLogic::crWeaponFireEffectLogic():
m_rotAngle(0.0f){ m_scale.set(1.0f,1.0f,1.0f); }
crWeaponFireEffectLogic::crWeaponFireEffectLogic(const crWeaponFireEffectLogic& handle):
	crNodeLogic(handle),
	m_offset(handle.m_offset),
	m_scale(handle.m_scale),
	m_rotAngle(handle.m_rotAngle),
	m_bonename(handle.m_bonename)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crWeaponFireEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (CRPhysics::crWeaponMatterObject*)param;
		break;
	}
}
void crWeaponFireEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_offset);
		break;
	case 1:
		crArgumentParser::appAtoVec(str,m_scale);
		break;
	case 2:
		m_rotAngle = atof(str.c_str());
		break;
	case 3:
		m_bonename = str;
		break;
	}
}
void crWeaponFireEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crWeaponFireEffectLogic::outputParam(int i, void *param)
{
}
void crWeaponFireEffectLogic::operator()(crHandle &handle)
{
	m_playSceneFx->inputParam(WCHDATA_AttachToNode,const_cast<crViewMatterObject *>(m_this->getFireBody()));
	m_playSceneFx->inputParam(WCHDATA_AttachOffset,&m_offset);
	m_playSceneFx->inputParam(WCHDATA_AttachScale,&m_scale);
	m_playSceneFx->inputParam(WCHDATA_AttachRotAngle,&m_rotAngle);
	m_playSceneFx->inputParam(WCHDATA_AttachBoneName,&m_bonename);
	(*m_playSceneFx)(*this);
}
void crWeaponFireEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}
/////////////////////////////////////////
//
//crBotEffectLogic
//
/////////////////////////////////////////
crBotEffectLogic::crBotEffectLogic():
	m_attachToNode(false){}
crBotEffectLogic::crBotEffectLogic(const crBotEffectLogic& handle):
	crNodeLogic(handle),
	m_positionOffset(handle.m_positionOffset),
	m_attachToNode(false)
{
	if(handle.m_playSceneFx.valid())
		m_playSceneFx = dynamic_cast<crNodeLogic *>(handle.m_playSceneFx->clone());
}
void crBotEffectLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_this = NULL;
		}
		break;
	case 1:
		m_this = (crMatrixTransform *)param;
		break;
	}
}
void crBotEffectLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		crArgumentParser::appAtoVec(str,m_positionOffset);
		break;
	case 1:
		m_attachToNode = (bool)(atoi(str.c_str()));
		break;
	}
}
void crBotEffectLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_playSceneFx = (CRCore::crNodeLogic *)param;
		break;
	}
}
void crBotEffectLogic::outputParam(int i, void *param)
{
}
void crBotEffectLogic::operator()(crHandle &handle)
{
	if(m_attachToNode)
	{
		m_playSceneFx->inputParam(WCHDATA_AttachToNode,m_this);
		(*m_playSceneFx)(*this);
	}
	else
	{
		crVector3 position = m_this->getTrans();	
		position += m_positionOffset;
		m_playSceneFx->inputParam(3,&position);
		crMatrixf rotation = m_this->getMatrix();
		rotation.setTrans(0.0f,0.0f,0.0f);
		crVector3 dir = (-Y_AXIS * rotation).normalize();
		m_playSceneFx->inputParam(4,&dir);
		(*m_playSceneFx)(*this);
	}
}
void crBotEffectLogic::releaseObjects(CRCore::crState* state)
{
	if(m_playSceneFx.valid())
		m_playSceneFx->releaseObjects(state);
}