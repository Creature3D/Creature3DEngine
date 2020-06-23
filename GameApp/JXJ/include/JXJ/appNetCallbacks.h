/**********************************************************************
*
*	文件:	   appBattleMethod.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#ifndef JXJ_APPNETCALLBACKS_H
#define JXJ_APPNETCALLBACKS_H

#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crRange.h>
#include <CREncapsulation/crLoader.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRDownload/crDownload.h>
#include <JXJ/appData.h>
#include <JXJ/appData3.h>
namespace JXJ{

/**************************************************************************

METHOD: crMonitorConnectServerCallback 
 
**************************************************************************/
class crMonitorConnectServerCallback :  public CRNet::crNetCallback
{
public:
	crMonitorConnectServerCallback ();
	crMonitorConnectServerCallback (const crMonitorConnectServerCallback & callback);
	META_NetCallback(CRNetApp, MonitorConnectServer)

	virtual void serverInitImplementation();
	virtual void clientInitImplementation();
	virtual void serverUpdateImplementation(float dt);
	virtual void clientUpdateImplementation(float dt);
	virtual void serverShutdown();
	virtual void clientShutdown();
};

};

#endif