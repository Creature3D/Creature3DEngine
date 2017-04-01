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
#include "creature3d.h"
#include <gnelib.h>
//#include <assert.h>
//#include "resource.h"
#include <CRNetApp/appNetLoader.h>
#include <CRNetApp/appNetStartLoader.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appGlobalHandle.h>
#include <CRNetApp/appDataClass.h>
#include <crgl/GL2Extensions.h>
#include <CREncapsulation/crTableIO.h>
#include <CRIOManager/crLoadManager.h>
#include <CRSound/crSoundSystem.h>
#include <shellapi.h>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC 
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include<stdlib.h> 
#include<crtdbg.h>
#endif

//#  include <windows.h>
//#  include <wininet.h>
//#include <WS2tcpip.h>
////#  include <errno.h>
//#ifdef _MSC_VER 
//#  pragma comment(lib, "Wininet")
//#  pragma comment(lib, "Wsock32")
//#pragma comment(lib,"Ws2_32.lib")
//#endif

using namespace CRCore;
using namespace CRPhysics;
using namespace CRParticle;
using namespace CRGUI;
using namespace CRNet;
using namespace CREncapsulation;

#if !defined(_DEBUG) && !defined(_ACTIVEX) && !defined(_Console)
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 
#endif

//#ifndef _DEBUG
#define CookFile 1
//#define _RetainSourceFile 1 
//#define CookWarn 1
//#endif
#define NetCookFile 1
#define Editor 0
#define NetGame 1
#define SingleGame 2
#define WebGame 3
#define Robot 4
#define RobotNoTexture 5 
//#define PublishRun 1
//HHOOK GKeyBoarHook = NULL;
//void BossKeyPressed()
//{
// 	static bool s_hide = false;
//	Producer::Window hwnd = CREncapsulation::crStartHandler::getInstance()->getWindowHandle();
//	if (hwnd)
//	{
//		//Producer::Window hParent = GetParent(hwnd);
//		//if (hParent)
//		//{
//		//	::ShowWindow(hParent, s_hide ? SW_SHOW : SW_HIDE);
//		//}
//		::ShowWindow(hwnd, s_hide?SW_SHOW:SW_HIDE);
//		s_hide = !s_hide;
//	}
//}
//LRESULT CALLBACK MyKeyboard(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if( nCode == HC_ACTION )
//	{
//		PKBDLLHOOKSTRUCT pKeyboardHookStruct = (PKBDLLHOOKSTRUCT)lParam;
//		if(wParam == WM_KEYDOWN)
//		{
//			if (pKeyboardHookStruct->vkCode == VK_OEM_3 && ((GetKeyState(VK_CONTROL) & 0x8000) != 0))
//			{
//				BossKeyPressed();
//			}
//		}
//	}
//	return CallNextHookEx(GKeyBoarHook, nCode, wParam, lParam);
//}
#ifdef _ACTIVEX
extern  "C" __declspec(dllexport) void ExitCreature3D()
{
	CRNetApp::crRunGameHandle::getInstance()->setRunning(0);
	CREncapsulation::crStartHandler::getInstance()->setEndCode(1);
	ref_ptr<CRProducer::crViewer>bindview = CREncapsulation::crStartHandler::getInstance()->getViewer();
	if(bindview.valid()) bindview->setDone(true);
}
extern  "C" __declspec(dllexport) int main(char *argv)
#else
int main( int argc, char **argv )
#endif
{
	HINSTANCE hUser32 = LoadLibrary( "user32.dll" );
	if( hUser32 )
	{//防止UI自动放大（系统屏幕放大功能）
		typedef BOOL ( WINAPI* LPSetProcessDPIAware )( void );
		LPSetProcessDPIAware pSetProcessDPIAware = ( LPSetProcessDPIAware )GetProcAddress( hUser32,
			"SetProcessDPIAware" );
		if( pSetProcessDPIAware )
		{
			pSetProcessDPIAware();
		}
		FreeLibrary( hUser32 );
	}
	//GKeyBoarHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyboard, NULL, 0);
	//{//tcp
	//	int    sockfd, n;
	//	char    recvline[4096], sendline[4096];
	//	struct sockaddr_in    servaddr;

	//	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	//		printf("create socket error: %s(errno: %d)/n", strerror(errno),errno);
	//		exit(0);
	//	}

	//	memset(&servaddr, 0, sizeof(servaddr));
	//	servaddr.sin_family = AF_INET;
	//	servaddr.sin_port = htons(9100);
	//	if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
	//		exit(0);
	//	}
	//	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
	//		printf("connect error: %s(errno: %d)/n",strerror(errno),errno);
	//		exit(0);
	//	}

	//	printf("send msg to server: /n");
	//	fgets(sendline, 4096, stdin);
	//	if( send(sockfd, sendline, strlen(sendline), 0) < 0)
	//	{
	//		printf("send msg error: %s(errno: %d)/n", strerror(errno), errno);
	//		exit(0);
	//	}

	//	//close(sockfd);
	//}
	//_crInt64 msg = 77311508691;
	//_crInt32 lmsg = LOINT64(msg);
	//_crInt32 hmsg = HIINT64(msg);
	//_crInt64 param = 353988215239081984;
	//_crInt32 lparam = LOINT64(param);
	//_crInt32 hparam = HIINT64(param);

	crDebugInfo::getInstance()->init("creature3d.txt");
	//{
	//	rcfg::ConfigScript cfg_script;
	//	CRIOManager::crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,"JXJ/script/giftspackmail.cfg",false,"");
	//	cfg_script.Write("t1.cfg");
	//}
	//{
	//	rcfg::ConfigScript cfg_script;
	//	CRIOManager::crScriptLoadManager::getInstance()->loadConfigScript(cfg_script,"JXJ/script/giftspackmail.cfg",true,"");
	//	cfg_script.Write("t2.cfg");
	//}
	CRNetApp::crGlobalHandle::setClient();
//#ifdef PublishRun
//	CRIOManager::SetPublishRun(true);
//#endif
	//ref_ptr<CREncapsulation::crTableIO> tabletest = new CREncapsulation::crTableIO;
	//tabletest->openFile("test.tab");
	//tabletest->saveToFile("test.tab");
//#ifdef _DEBUG
	//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//_CRTDBG_REPORT_FLAG
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(406442);
//#endif
	//crDisplaySettings::instance()->setUseZperPass(true);
	_putenv("CRE_RESTART=0");
	//std::string program = "CRE_PROGRAM=";
	//program += argv[0];
	crNetContainer::getInstance()->setMainNetType(CRNetApp::Client);
#ifndef _ACTIVEX
	int runMode = NetGame;//0 Editor, 1 NetGame, 2 SingleGame
	//if(1<argc && argv[1])
	//{
	//	//program += " ";
	//	//program += argv[1];
	//	if(strcmp(argv[1],"Editor") == 0)
	//	{
	//		runMode = 0;
	//		if(argc>2 && strcmp(argv[2],"Publish") == 0)
	//			CRIOManager::SetPublishRun(true);
	//	}
	//	else if(strcmp(argv[1],"Publish") == 0)
	//		CRIOManager::SetPublishRun(true);
	//	else if(strcmp(argv[1],"Robot") == 0)
	//		runMode = Robot;
	//}
	//if(argc>2 && argv[2])
	//{
	//	fpsControl = atof(argv[2]);
	//}
#else
	int runMode = WebGame;//0 Editor, 1 NetGame, 2 SingleGame
#endif
	std::string webstr;
	std::string argvstr;
#ifdef _ACTIVEX
	argvstr = argv;
#else
	for( int i = 1; i<argc; i++)
	{
		argvstr += argv[i];
		argvstr += " ";
	}
	std::string firststr = argv[0];
	argvstr = firststr + " " + argvstr;
#endif

	CRNetApp::crGlobalHandle::argvstr() = argvstr;

	char gbuf[256];
	sprintf(gbuf,"%s\n\0",argvstr.c_str());
	gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	gDebugInfo->writeToFile();
	//MessageBox(NULL,argvstr.c_str(),"Creature3D",MB_OKCANCEL);
	if (argvstr.find("Editor") != std::string::npos)
		runMode = 0;
	else if (argvstr.find("RobotNoTexture") != std::string::npos)
	{
		runMode = RobotNoTexture;
	}
	else if(argvstr.find("Robot")!= std::string::npos)
	{
		runMode = Robot;
	}
	else if(argvstr.find("SingleGame")!= std::string::npos)
	{
		runMode = SingleGame;
	}
	if(argvstr.find("Publish")!=std::string::npos)
		CRIOManager::SetPublishRun(true);
	//-WebLogin key|playerid|serverid|isp
	//-WebLogin aknpqwBDEOPSZ126|8|1|1 
	if(crArgumentParser::readKeyValue(argvstr,"-WebLogin",webstr) && !webstr.empty())
	{
		char buf[64];
		sprintf(buf,"WebLogin=%s\0",webstr.c_str());//key|playerid|serverid|isp
		_putenv(buf);
	}
	else
	{
		_putenv("WebLogin=\0");
	}
	////_putenv(program.c_str());
	HANDLE hMutex = NULL;
	if(runMode > 0/* && runMode < WebGame*/)
	{//双开
		hMutex = CreateMutex(NULL, false, "Creature3D");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hMutex);
			MessageBox(::GetActiveWindow(), "程序已经在运行中，不能重复启动！", "Creature3D", MB_OK);
			return 0;
		}
		CRNetApp::crGlobalHandle::setRunProtectHandle(hMutex);
	}
#ifdef CookFile
	CRIOManager::SetCooked(true);
#endif
#ifdef _RetainSourceFile
	CRIOManager::SetRetainSourceFile(true);
#endif
#if defined(CookWarn) && defined(CookFile)
	if(MessageBox(::GetActiveWindow(),"警告：程序将以COOK方式运行，请确认!","Creature3D Cook" ,MB_OKCANCEL)==IDCANCEL) return 0;
#endif

#ifdef NetCookFile
	CRIOManager::SetNetCooked(true);
#endif

	srand(time(0));
	CRSound::crSoundSystem::getInstance()->init();
	//srand(GetCurrentThreadId()*10); 
	//_putenv( "CRE_RunMode=Editor" );
	//CRCore::crDisplaySettings::instance()->setRunMode(0);
	//CRGUI::crWindow::s_defaultCursor = (HCURSOR)LoadImage(GetModuleHandle(NULL),"media/ui/default.ani",IMAGE_CURSOR,0,0,LR_LOADFROMFILE);
	CREncapsulation::registerEventCallback();
    CREncapsulation::registerHandleClass();

	CRNetApp::registerDataClass();
	CRNetApp::registerHandleClass();
	CRNetApp::registerNetPacket();
	CRNetApp::registerNetCallbacks();
	CRNetApp::registerVRHandleClass();
	CRNetApp::registerEventCallback();
	//CRPhysics::crOdeWorld::setCharacterCollide(true);

	//CRUtil::crSceneView::registerSceneManagerUpdater(new CREncapsulation::crSceneManagerUpdater);
	crDisplaySettings *ds = CRCore::crDisplaySettings::instance();
	ds->setRunMode(runMode);
	CREncapsulation::loadInitCfg(true);
	int hWndParent = NULL;
	int hWnd = NULL;
	int xpos = 0;
	int ypos = 0;
	int width = 1366;
	int height = 768;
	int fullscreen = 0;
	int board = 1;
//#ifdef _ACTIVEX
	crArgumentParser::readKeyValue(argvstr,"-hWndParent",hWndParent);
	crArgumentParser::readKeyValue(argvstr,"-Wnd",hWnd);
	crArgumentParser::readKeyValue(argvstr,"-xpos",xpos);
	crArgumentParser::readKeyValue(argvstr,"-ypos",ypos);
	crArgumentParser::readKeyValue(argvstr,"-width",width);
	crArgumentParser::readKeyValue(argvstr,"-height",height);
	if (hWndParent != NULL)
		ds->setOwnWindow(false);
	if (crArgumentParser::readKeyValue(argvstr, "-fullscreen", fullscreen))
	{
		ds->setFullScreen(fullscreen);
	}
	if (crArgumentParser::readKeyValue(argvstr, "-board", board))
	{
		ds->setUseBoard(board);
	}
	ds->setScreenWidth(width);
	ds->setScreenHeight(height);
	ds->setViewSize(width,height);
	//char programDir[256];
	//GetCurrentDirectory(256,programDir);
	//MessageBox((HWND)hWnd,programDir,"Creature3DX",MB_OKCANCEL);
//#endif
//#ifdef _WIN32
//	if(ds->getFullScreen())
//	    CREncapsulation::crStartHandler::changeDisplaySettings(ds->getScreenWidth(),ds->getScreenHeight(),ds->getColorBits());
//#endif
    
	CRIOManager::FilePathList& filePathList = CRIOManager::crRegistry::instance()->getDataFilePathList();
	filePathList.push_back("media/textures");
	filePathList.push_back("media/icon");
	filePathList.push_back("media/fonts");
	filePathList.push_back("scene");
	filePathList.push_back("data");
	filePathList.push_back("media/textures/engine");
	filePathList.push_back("media/textures/develop");
	filePathList.push_back("media/heightmap");
	filePathList.push_back("uiscene");
	//filePathList.push_back("characters/");
	//filePathList.push_back("script/");
	//filePathList.push_back("media/");
	CRIOManager::FilePathList& libPathList = CRIOManager::crRegistry::instance()->getLibraryFilePathList();
	libPathList.push_back("bin");

	ref_ptr<crTableIO> table = crTableIO::openFile("gamedll.crb");
	if(table.valid())
	{
		int rowcount = table->getRowCount();
		int col = 1;
		for(int i = 0; i<rowcount; ++i)
		{
			CRIOManager::loadScript((*table)(i,col));
		}
	}

	//if(runMode == Editor)
	//	CRGUI::crTextAttribute::loadTextAttrMap("script/textattr.cfg");
	//else
	//	CRGUI::crTextAttribute::loadTextAttrMap("script/gametextattr.cfg");

	////CRCore::Timer_t start_tick = CRCore::Timer::instance()->tick();
	//CREncapsulation::loadStageCfg();
	if(runMode == Editor)
	{
		CRGUI::crTextAttribute::loadTextAttrMap("script/textattr.cfg");
		CREncapsulation::loadStageCfg();
	}
	crData *data = CREncapsulation::loadData("script/clientevent.cfg");
	CREncapsulation::loadIncludeData("script/GlobalData.cfg",data);
	CRNetApp::crGlobalHandle::getInstance()->setDataClass(data);
	if(runMode != Editor)
	{
		int sceneid = CRNetApp::crGlobalHandle::getInstance()->gData()->gFirstGameSceneID();
		data = CREncapsulation::loadData("script/RunGame.cfg");
		data->inputParam(WCHDATA_SceneID,&sceneid);
		CRNetApp::crRunGameHandle::getInstance()->setDataClass(data);
	}
	//CRCore::Timer_t end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to loadStageCfg = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;
	
	/*if(!CREncapsulation::checkSN())
	{
		//crFilterRenderManager::getInstance()->releaseObjects();
		CRAL::crSoundManager::instance()->shutdown();
#ifdef _WIN32
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
#endif
		return 0;
	}*/

	//CRGUI::cr2DStageManager::getInstance()->showStage("Esplash",true);

	CRCore::crTexture::setImageDataRequestHandler(new CREncapsulation::crImageDataRequestClass);
	CRParticle::crParticleEffect::setLoadNodeCfgHandler(new CREncapsulation::crLoadNodeCfgClass);
	CalLoader::setLoadingMode(LOADER_INVERT_V_COORD);

	CRPhysics::crCharacterObjectNearCallback::instance()->setFriendDamage(false);
	CRPhysics::crMatterGroup::addNearCallback("RadarCollideDetect",CRPhysics::crRadarObjectNearCallback::instance());
	CRPhysics::crMatterGroup::addNearCallback("CharacterCollideDetect",CRPhysics::crCharacterObjectNearCallback::instance());
	CRPhysics::crMatterGroup::addNearCallback("ViewCollideDetect",CRPhysics::crViewObjectNearCallback::instance());

	//start_tick = CRCore::Timer::instance()->tick();
    CRIOManager::readObjectFile("all.shd");
	CRCore::gCredefimage() = CRIOManager::readImageFile("credef.img");
	//end_tick = CRCore::Timer::instance()->tick();
	//CRCore::notify(CRCore::ALWAYS)<<"Time to loadShaderManagerCfg = "<<CRCore::Timer::instance()->delta_s(start_tick,end_tick)<<std::endl;

	//CREncapsulation::loadLoadManagerCfg();
	//CREncapsulation::loadSaveCfg();
	//CREncapsulation::wantToLoadSave(0);
	//CREncapsulation::loadSceneManagerCfg();

	//CRGUI::cr2DStageManager::getInstance()->showStage("Esplash",false);
    //CRGUI::cr2DStageManager::getInstance()->close("Esplash");
	///////////////////////////////////////////////
	//可以利用Main窗口测试UI，
	//最终系统将在进入3D渲染器后显示main窗口UI
	////////////////////////////////////////////////
	if(runMode != Editor)
	{
//#ifdef _ACTIVEX
//		CRGUI::cr2DStageManager::getInstance()->setParentWnd((HWND)hWndParent);
//		if(hWnd)
//		{
//			CRGUI::crImageStage *loadingStage = CRGUI::cr2DStageManager::getInstance()->findStage("loading");
//			char buf[32];
//			sprintf(buf,"CRE_ViewX=%d\0",xpos);
//			_putenv(buf);
//			sprintf(buf,"CRE_ViewY=%d\0",ypos);
//			_putenv(buf);
//			loadingStage->setRect(xpos,ypos,width,height);
//			loadingStage->setScaleWithScreen(CRGUI::crImageStage::Scale_None);
//		}
//#endif
		CRNetApp::crRunGameHandle::getInstance()->setRunning(true);
		CRNetApp::crRunGameHandle::getInstance()->setRunCode(CRNetApp::crRunGameHandle::Login);
		CRNetApp::crRunGameHandle::getInstance()->setParentWnd((Producer::Window)hWndParent);
		CRNetApp::crRunGameHandle::getInstance()->setRenderWnd((Producer::Window)hWnd);
		CRNetApp::crRunGameHandle::getInstance()->doEvent(WCH_Startup);
	}
	else
	{
		//CRAL::crSoundManager::instance()->loadSound("script/sound0.cfg");
		//crBrain::getInstance()->pushInstance(CRAL::crSoundManager::instance());

		CRGUI::crImageStage *stage;

		if(runMode == 1)
			stage = CRGUI::cr2DStageManager::getInstance()->findStage("Login");
		else if(runMode == 0)
			stage = CRGUI::cr2DStageManager::getInstance()->findStage("Emain");
		else if(runMode == 2)
			stage = CRGUI::cr2DStageManager::getInstance()->findStage("SingleGame");
		CRGUI::cr2DStageManager::getInstance()->setParentWnd((Producer::Window)hWndParent);
		CRGUI::cr2DStageManager::getInstance()->doModal(stage);
	}
	CRIOManager::crLoadManager::getInstance()->clear();
	CRGUI::cr2DStageManager::getInstance()->clear();
	CRCore::crEventCallbackManager::getInstance()->clear();
	CRCore::crDataManager::getInstance()->clear();
	CRCore::crHandleManager::getInstance()->clear();
	//CRAL::crSoundManager::instance()->shutdown();
	CRCore::crTexture::setImageDataRequestHandler(NULL);
	CRParticle::crParticleEffect::setLoadNodeCfgHandler(NULL);
	GL2Extensions::clearExtensions();
	CRIOManager::crScriptLoadManager::getInstance()->clear();
	CRSound::crSoundSystem::getInstance()->shutdown();
#ifdef _ACTIVEX
	CRIOManager::crRegistry::instance()->closeAllLibraries();
#endif

#ifdef _WIN32
	ChangeDisplaySettings(NULL,0);
	ShowCursor(TRUE);
#endif
	if(CRNetApp::crGlobalHandle::getRunProtectHandle())
	{//LoginJXJ.cfg
		CloseHandle(CRNetApp::crGlobalHandle::getRunProtectHandle());
		CRNetApp::crGlobalHandle::setRunProtectHandle(NULL);
	}
	char *restart = getenv("CRE_RESTART");
	if(restart)
	{
		int code = atoi(restart);
//#ifndef _ACTIVEX
//		if(code == 1)
//		{
//			ShellExecute(::GetDesktopWindow(),"open",argv[0],argv[1],NULL,SW_SHOWNORMAL);
//		}
//		//else if( code == 2)
//		//{
//		//	MessageBox(::GetDesktopWindow(),"游戏将自动关闭并运行更新程序，请完成更新后重新运行游戏","Creature3D",MB_OK);
//		//	//运行cmd 解压Creature3DUpdate后自动重启游戏程序
//		//	SetCurrentDirectory("bin");
//		//	ShellExecute(::GetDesktopWindow(),"open","Creature3DUpdate.exe",NULL,NULL,SW_SHOWNORMAL);
//		//}
//#else
//		//网页情况，需要测试
//#endif
		if(code == 3)
		{//本程序退出后将更新目录下的所有文件拷贝到游戏目录下，然后重启游戏
#ifdef _ACTIVEX
			::SendMessage((HWND)hWndParent,WM_USER+10,NULL,NULL);
#else
			ShellExecute(::GetDesktopWindow(),"open","Launcher.exe",NULL,NULL,SW_HIDE);
#endif
			//system("Launcher.exe");
			//WinExec("Launcher.exe",SW_HIDE);
		}
	}
	if(CRCore::crDisplaySettings::instance()->getRunMode() >= Robot)
	{
		Sleep(60000);
		//UpLoadBugFile(CRNetApp::crGlobalHandle::argvstr());
		WinExec(CRNetApp::crGlobalHandle::argvstr().c_str(),SW_NORMAL);
	}
	//UnhookWindowsHookEx(GKeyBoarHook);
	return 0;
}