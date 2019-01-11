/**********************************************************\

  Auto-generated Creature3DAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "Creature3DAPI.h"
#include <shlobj.h>
#pragma comment(lib,"wininet.lib")

#if defined(WIN32) && !defined(__CYGWIN__)
#include <Io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // for _mkdir

#define mkdir(x,y) _mkdir((x))
#define stat64 _stati64

// set up for windows so acts just like unix access().
#define F_OK 4


#else // unix

#if defined( __APPLE__ ) || defined(__CYGWIN__) || defined(__FreeBSD__)
#define stat64 stat
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

// set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif
#include <string>
#include <fstream>
#include <time.h>
#include <vector>
#include <fstream>
#include <stack>

void findFileInDir(const char* rootDir, std::vector<std::string>& fileNameVec);
bool fileExists(const std::string& filename);
std::string convertFileNameToUnixStyle(const std::string& fileName);
bool makeDirectory( const std::string &path );
bool makeDirectoryForFile( const std::string &path );
std::string getFilePath(const std::string& fileName);
void appendLibraryFilePaths(const std::string &path);
DWORD WINAPI  StartDownload(void* ctrl) ;

static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);  
static WNDPROC lpOldProc = NULL;  
///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant Creature3DAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant Creature3DAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn Creature3DPtr Creature3DAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
Creature3DPtr Creature3DAPI::getPlugin()
{
    Creature3DPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read-only property version
std::string Creature3DAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void Creature3DAPI::setParam(const std::string& key, const std::string& value)
{
	if (!key.empty() && !value.empty())
	{
		m_params[key] = value;
	}
}

std::string Creature3DAPI::getParam(const std::string& key)
{
	if (!key.empty())
	{
		return m_params[key];
	}
	return "";
}
//void Creature3DAPI::download()
//{
//	HINTERNET internetopen;
//	internetopen=InternetOpenA("Testing",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
//	if (internetopen==NULL){
//		MessageBoxA(NULL,"Internet open failed!","error",MB_OK);
//		return;
//	}
//	std::string vcredisturl = getParam("vcredist");
//	m_internetopenurl=InternetOpenUrlA(internetopen,vcredisturl.c_str()/*"http://jxj.quegame.com/vcredist_x86.exe"*/,NULL,0,INTERNET_FLAG_RELOAD,0);
//	if (m_internetopenurl==NULL){
//		MessageBoxA(NULL,"Internet open url failed!","error",MB_OK);
//		InternetCloseHandle(internetopen);
//		return;
//	}
//	m_downloadState = 1;
//	RECT rc;  
//	GetClientRect(m_hWnd, &rc); 
//	InvalidateRect(m_hWnd,&rc,true);
//	m_downloadThread = CreateThread(NULL, 0, StartDownload, (void*)this, 0, 0);
//}
//DWORD WINAPI  StartDownload(void* ctrl) 
//{
//	Creature3DAPI *theCtrl = (Creature3DAPI *)ctrl;
//	if (!theCtrl)
//	{
//		theCtrl->m_downloadState = 2;
//		MessageBoxA(NULL,"","error",MB_OK);
//		return 0;
//	}
//
//	char buf[1024];
//	wsprintfA(buf,"%s%s\0",theCtrl->m_clientPath.c_str(),"\\vcredist_x86.exe");
//	HANDLE vcredistFile=CreateFileA(buf,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
//	if (vcredistFile==INVALID_HANDLE_VALUE){
//		char szError[256] = {0};
//		wsprintfA(szError,"Create File failed! %u",GetLastError());
//		MessageBoxA(NULL,szError,"error",MB_OK);
//		InternetCloseHandle(theCtrl->m_internetopenurl);
//		theCtrl->m_downloadState = 2;
//		return 0;
//	}
//
//	BOOL hwrite;
//	DWORD written;
//	DWORD byteread=0;
//	char buffer[1024];
//	memset(buffer,0,1024);
//	while (true)
//	{
//		BOOL internetreadfile=InternetReadFile(theCtrl->m_internetopenurl,buffer,sizeof(buffer),&byteread);
//		if(byteread==0)
//		{
//			CloseHandle(vcredistFile);
//			break;
//		}
//		hwrite=WriteFile(vcredistFile,buffer,sizeof(buffer),&written,NULL);
//		if (hwrite==0){
//			//"Write to file failed!"
//			CloseHandle(vcredistFile);
//			break;
//		}
//	}
//	InternetCloseHandle(theCtrl->m_internetopenurl);
//	MessageBoxA(NULL,"您的电脑缺少微软运行时库,点击开始安装.","提示",MB_OK);
//	ShellExecuteA(theCtrl->m_hWnd,"open","vcredist_x86.exe",theCtrl->m_clientPath.c_str(),NULL,SW_SHOWNORMAL);
//	theCtrl->m_downloadState = 2;
//	RECT rc;  
//	GetClientRect(theCtrl->m_hWnd, &rc); 
//	InvalidateRect(theCtrl->m_hWnd,&rc,true);
//	return 0;
//}
DWORD WINAPI  StartCreature3DThread(void* ctrl) 
{
	Creature3DAPI *theCtrl = (Creature3DAPI *)ctrl;
	//MessageBoxA(NULL,"调用StartCreature3D","Creature3DX",MB_OKCANCEL);
	theCtrl->StartCreature3D();
	return 0;
}

std::string getFilePath(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1 == std::string::npos)
	{
		if (slash2 == std::string::npos) return std::string();
		return std::string(fileName, 0, slash2);
	}
	if (slash2 == std::string::npos) return std::string(fileName, 0, slash1);
	return std::string(fileName, 0, slash1 > slash2 ? slash1 : slash2);
}
void appendLibraryFilePaths(const std::string &path)
{
	static bool inited = false;
	if (!inited)
	{
		static char newPath[1024];
		char* ptr;
		if ((ptr = getenv("PATH")))
		{
			sprintf(newPath, "PATH=%s;%s\0", path.c_str(), ptr);
			putenv(newPath);

		}
		else
		{
			sprintf(newPath, "PATH=%s\0", path.c_str());
			putenv(newPath);
		}
		inited = true;
	}
}
bool fileExists(const std::string& filename)
{
	return access(filename.c_str(), F_OK) == 0;
}
std::string getSimpleFileName(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1 == std::string::npos)
	{
		if (slash2 == std::string::npos) return fileName;
		return std::string(fileName.begin() + slash2 + 1, fileName.end());
	}
	if (slash2 == std::string::npos) return std::string(fileName.begin() + slash1 + 1, fileName.end());
	return std::string(fileName.begin() + (slash1 > slash2 ? slash1 : slash2) + 1, fileName.end());
}
std::string Creature3DAPI::findPath(const std::string &path)
{
	char buf[_MAX_PATH];
	ZeroMemory(buf,_MAX_PATH);
	SHGetSpecialFolderPathA(NULL,buf,CSIDL_SYSTEM,0);
	sprintf(buf,"%c:/%s\0",buf[0],path.c_str());
	std::string _path = buf;
	//MessageBoxA(NULL,_path.c_str(),"findPath",MB_OK);
	do
	{
		if (fileExists(_path))
			break;
		_path = "C:/" + path;
		if (fileExists(_path))
			break;
		_path = "D:/" + path;
		if (fileExists(_path))
			break;
		_path = "E:/" + path;
		if (fileExists(_path))
			break;
		_path = "F:/" + path;
		if (fileExists(_path))
			break;
		_path = "G:/" + path;
		if (fileExists(_path))
			break;
		_path = "H:/" + path;
		if (fileExists(_path))
			break;
	} while (0);
	return _path;
}
void Creature3DAPI::initialize()
{
	std::string exepath = findPath(getParam("gamedir"));
	if(exepath.empty())
	{
		//MessageBoxA(NULL,dllpath.c_str(),"文件未找到",MB_OKCANCEL);
		//需要重新安装插件
		std::string pluginurl = getParam("pluginurl");
		getHost()->Navigate(pluginurl, "_self");
		return;
	}
	m_exename = getSimpleFileName(exepath);
	m_clientPath = getFilePath(exepath);
	appendLibraryFilePaths(m_clientPath);
	m_clientPath.erase(m_clientPath.size() - 4, 4);//"/bin"

	m_hWnd = getPlugin().get()->getHwnd();
	lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);  

	//SetCurrentDirectoryA(m_clientPath.c_str());
	// 将窗口与 Plugin 对象相关联，这样就可以在窗口处理中访问Plugin 对象   
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);  
	initDialog();
	m_thread = CreateThread(NULL, 0, StartCreature3DThread, (void*)this, 0, 0);
	//RECT rect;
	//GetWindowRect(m_hWnd, &rect);
	//int x = rect.left;
	//int y = rect.top;
	//int width = rect.right - rect.left;
	//int height = rect.bottom - rect.top;
	//char buf[2048];
	//std::string param = getParam("param");
	//wsprintfA(buf, "%s -hWndParent %d -xpos %d -ypos %d -width %d -height %d -fullscreen 0 -board 0 %s\0", dllpath.c_str(), m_hWnd, x<0?0:x, y<0?0:y, width, height,param.c_str());
	////MessageBoxA(NULL, buf, "Creature3DAPI", MB_OK);
	////WinExec(buf, SW_SHOW);
	//STARTUPINFOA si; //This is an [in] parameter
	//ZeroMemory(&si, sizeof(si));
	//si.cb = sizeof si; //Only compulsory field
	//si.dwFlags = STARTF_USESHOWWINDOW;
	//si.wShowWindow = SW_SHOW;
	//m_inited = CreateProcessA(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&m_exeProcess);
}
void Creature3DAPI::shut()
{
	exitGame();
	waitClose();
	if (lpOldProc)
	{
		SubclassWindow(m_hWnd, lpOldProc);
		lpOldProc = NULL;
	}

	if(m_dialog)
	{
		FreeLibrary(m_dialog);
		m_dialog = NULL;
	}
	if(m_thread)
	{
		CloseHandle( m_thread );
		m_thread = NULL;
	}
	//if(m_downloadThread)
	//{
	//	CloseHandle(m_downloadThread);
	//	m_downloadThread = NULL;
	//}
	m_shutdown = true;
	while(!m_downloadmap.empty())
	{
		Sleep(10);
	}
	if(m_internetopen)
	{
		InternetCloseHandle(m_internetopen);
		m_internetopen = NULL;
	}
	HANDLE closethread;
	while(!m_closeThreadHandleDeque.empty())
	{
		closethread = m_closeThreadHandleDeque.front();
		m_closeThreadHandleDeque.pop_front();
		CloseHandle( closethread );
	}
}
//void ConvertGBKToUtf8(std::string& amp, std::string strGBK)
//{ 
//	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, NULL,0); 
//	unsigned short * wszUtf8 = new unsigned short[len+1]; 
//	memset(wszUtf8, 0, len * 2 + 2); 
//	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len); 
//	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
//	char *szUtf8=new char[len + 1]; 
//	memset(szUtf8, 0, len + 1); 
//	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
//	//strGBK = szUtf8; 
//	amp=szUtf8;
//	delete[] szUtf8; 
//	delete[] wszUtf8; 
//}
typedef __declspec(dllimport) void INITDIALOG();
void Creature3DAPI::initDialog()
{
	if(!m_dialog)
	{
		char buf[256];
		wsprintfA(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\creBroswer.dll");
		m_dialog = LoadLibraryA(buf);
		if(m_dialog)
		{
			INITDIALOG* dialogNavigate;
			dialogNavigate = (INITDIALOG*)GetProcAddress (m_dialog, "init");
			if (dialogNavigate)
			{
				(*dialogNavigate)();
			}
		}
		else
		{
			char szError[256] = {0};
			sprintf(szError,"Load creBroswer.dll Faild %u",GetLastError());
			MessageBoxA(NULL,szError,"error",MB_OK);
		}
	}
}
typedef __declspec(dllimport) void RUNDIALOG(char *);
void Creature3DAPI::showDialog(char* url)
{
	if(m_dialog)
	{
		RUNDIALOG* dialogNavigate;
		dialogNavigate = (RUNDIALOG*)GetProcAddress (m_dialog, "Navigate");
		if (dialogNavigate)
		{
			(*dialogNavigate)(url);
		}
	}
}
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{
	switch (msg) 
	{  
	case WM_USER+602:
		{
			std::string str = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
			Creature3DAPI * p = (Creature3DAPI*) GetWindowLongPtr(hWnd, GWLP_USERDATA);  
			if (p)
			{
				str += "&openid="+p->getParam("openid");
				str += "&openkey="+p->getParam("openkey");
				str += "&pf="+p->getParam("pf");
				str += "&pfkey="+p->getParam("pfkey");
				//std::string url ;
				//ConvertGBKToUtf8(url,str);
				//MessageBox(NULL, url.c_str(), "xxx", MB_OK);
				//p->getHost()->Navigate(url,"_blank");
				p->showDialog((char*)str.c_str());
				//ShellExecute(::GetActiveWindow(),L"open",L"crExplorer.exe",url.c_str(),NULL,SW_HIDE);
			}
		}
		break;
	case WM_USER+603:
		{
			std::string url = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
			//MessageBox(NULL,url.c_str(),"",MB_OK);
			Creature3DAPI * p = (Creature3DAPI*) GetWindowLong(hWnd, GWLP_USERDATA);  
			if (p)
			{
				p->getHost()->Navigate(url,"_blank");
				//FB::DOM::WindowPtr window = p->getHost()->getDOMWindow();
				//if (window && window->getJSObject()) {
				//	FB::JSObjectPtr obj = window->getJSObject();
				//	// Invoke alert with some text
				//	obj->Invoke("Navigate", FB::variant_list_of(url.c_str()));
				//}
			}
		}
		break;
	//case WM_PAINT:  
	//	{  
	//		PAINTSTRUCT ps;  
	//		HDC hdc = BeginPaint(hWnd, &ps);  
	//		RECT rc;  
	//		Creature3DAPI * p = (Creature3DAPI*) GetWindowLong(hWnd, GWLP_USERDATA);  
	//		GetClientRect(hWnd, &rc);  
	//		std::string str;

	//		SetTextColor(hdc, RGB(230, 0, 0));
	//		int x = (rc.right-rc.left)/2 -100;
	//		int y = (rc.bottom-rc.top)/2 - 40;
	//		if (p)
	//		{
	//			if(p->m_downloadState == 1)
	//			{
	//				str = "发现关键文件缺失,请等待...";
	//			}
	//			else if(p->m_downloadState == 2)
	//			{
	//				str = "安装完成后请刷新进入游戏";
	//			}
	//			TextOutA(hdc, x, y, str.c_str(), str.length());
	//			EndPaint(hWnd, &ps);
	//		}else
	//		{
	//			// draw a frame and display some string   
	//			FillRect( hdc, &rc,  GetStockBrush(BLACK_BRUSH));  
	//			Sleep(100);
	//			EndPaint(hWnd, &ps);  
	//		}
	//		//FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));  
	//		//Plugin * p = (Plugin*) GetWindowLongPtr(hWnd, GWLP_USERDATA);  
	//		//if(p) {  
	//		//	char *s = "PLUGIN , PLUGIN!";//p->GetGuiText();   
	//		//	DrawText(hdc, s, strlen(s), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);  
	//		//}  			
	//	}  
	//	break;
	case WM_USER+10:
		{
			Creature3DAPI *p=(Creature3DAPI *)GetWindowLong(hWnd,GWLP_USERDATA);
			p->needRestart();
		}
		break;
	default:  
		break;  
	}  

	return DefWindowProc(hWnd, msg, wParam, lParam);  
}  
typedef __declspec(dllimport) void EXITCREATURE3D();
void Creature3DAPI::exitGame()
{
	if(m_creature3d)
	{
		EXITCREATURE3D* exitgame;
		exitgame = (EXITCREATURE3D*)GetProcAddress (m_creature3d, "ExitCreature3D");
		if (exitgame)
		{
			(*exitgame)();
		}
	}
}
void Creature3DAPI::needRestart()
{
	m_needRestart = true;
}
void Creature3DAPI::waitClose()
{
	MSG msg;
	while(m_creature3d)
	{
		//::Sleep(10);
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}
}
//void Creature3DAPI::gameLog(int v)
//{
//	FB::DOM::WindowPtr window = getHost()->getDOMWindow();
//	if (window && window->getJSObject()) {
//		FB::JSObjectPtr obj = window->getJSObject();
//		// Invoke alert with some text
//		obj->Invoke(/*"recordqq"*/"record", FB::variant_list_of(v));
//	}
//}
typedef __declspec(dllimport) int RUNCREATURE3D(char *);
void Creature3DAPI::StartCreature3D(/*HWND hWndParent, HWND hWnd, int x, int y, int width, int height*/)
{
	SetCurrentDirectoryA(m_clientPath.c_str());
	//////////版本文件检测与更新
	std::vector<std::string> fileNameVec;
	std::string pakpath = getParam("pakpath");
	if(pakpath.empty())
		pakpath = "jxjpkg\\bin";
	else
		pakpath += "\\bin";
	findFileInDir(pakpath.c_str(),fileNameVec);//"jxjpkg\\bin"
	if(!fileNameVec.empty())
	{
		std::string filename;
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
			filename = *itr;
			filename.erase(0,7);
			if(copyFile(*itr,filename))
				DeleteFileA((*itr).c_str());
		}
	}

	//先写文件
	std::string param = getParam("param");
	std::string loginInfo = getParam("loginInfo");
	WriteLoginInfo2File(loginInfo);

	if(!m_creature3d)
	{
		char buf[1024];
#ifdef _DEBUG
		wsprintf(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\Creature3Dd.dll");
#else
		wsprintfA(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\Creature3D.dll");
#endif
		//MessageBoxA(NULL,buf,"buf",MB_OK);
		m_creature3d = LoadLibraryA(buf);
	}
	if(m_creature3d)
	{
		//gameLog(1);
		RUNCREATURE3D* creature3DMain;
		creature3DMain = (RUNCREATURE3D*)GetProcAddress (m_creature3d, "main");
		if (creature3DMain)
		{
			RECT rect;
			GetWindowRect(m_hWnd,&rect);
			int x = rect.left;
			int y = rect.top;
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			char argv[1024];
			if(loginInfo.empty())
				sprintf(argv,"-hWndParent %d -Wnd %d -xpos %d -ypos %d -width %d -height %d %s\0",(int)m_hWnd,0,(int)x,(int)y,(int)width,(int)height,param.c_str());
			else
				sprintf(argv,"-hWndParent %d -Wnd %d -xpos %d -ypos %d -width %d -height %d -WebLogin %s %s\0",(int)m_hWnd,0,(int)x,(int)y,(int)width,(int)height,loginInfo.c_str(),param.c_str());
			//sprintf_s(argv,"-hWndParent %d -Wnd %d -xpos %d -ypos %d -width %d -height %d\0",(int)m_hWnd,0,(int)x,(int)y,(int)width,(int)height);
			//MessageBoxA(m_hWnd,argv,"Creature3DX",MB_OKCANCEL);
			(*creature3DMain)(argv);
		}
		FreeLibrary(m_creature3d);
		m_creature3d = NULL;
	}
	else
	{
		//gameLog(0);
		///*char szError[256] = {0};
		//sprintf(szError,"Load Creature3D.dll Faild %u",GetLastError());
		//MessageBox(NULL,szError,"error",MB_OK);*/
		////if (GetLastError() == 126)
		//{
		//	download();
		//}
		char buf[2048];
		if(loginInfo.empty())
			wsprintfA(buf, "%s/bin/%s %s\0", m_clientPath.c_str(),m_exename.c_str(),param.c_str());
		else
			wsprintfA(buf, "%s/bin/%s -WebLogin %s %s\0", m_clientPath.c_str(),m_exename.c_str(), loginInfo.c_str(),param.c_str());
		//MessageBoxA(NULL, buf, "Creature3DAPI", MB_OK);
		//WinExec(buf, SW_SHOW);
		PROCESS_INFORMATION pi;
		STARTUPINFOA si; //This is an [in] parameter
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof si; //Only compulsory field
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		CreateProcessA(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi);
	}
	if(m_needRestart)
	{
		m_needRestart = false;
		StartCreature3D();
	}
}

void Creature3DAPI::WriteLoginInfo2File(const std::string& logininfo)
{
	FILE * pFile = fopen("jxjlogin.dat", "wb");
	fwrite(logininfo.c_str(), 1, logininfo.length(), pFile);
	fclose(pFile);
}

//////////////////////////////////////////////////////////////////////////
void findFileInDir(const char* rootDir, std::vector<std::string>& fileNameVec)
{
	std::string rdir = rootDir;
	char fname[1024];
	ZeroMemory(fname, 1024);

	WIN32_FIND_DATAA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATAA));

	HANDLE hSearch;

	char filePathName[1024];
	char tmpPath[1024];
	ZeroMemory(filePathName, 1024);
	ZeroMemory(tmpPath, 1024);

	strcpy(filePathName, rootDir);

	bool bSearchFinished = FALSE;

	if( filePathName[strlen(filePathName) -1] != '\\' )
	{
		strcat(filePathName, "\\");
	}
	strcat(filePathName, "*");
	hSearch = FindFirstFileA(filePathName, &fd);

	if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		strcpy(tmpPath, rootDir);
		strcat(tmpPath, fd.cFileName);
		strcat(tmpPath, "\\");
		findFileInDir(tmpPath, fileNameVec);
	}
	else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
	{
		if( rootDir[strlen(rootDir) -1] != '\\' )
			sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
		else
			sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
		fileNameVec.push_back(fname);
	}

	while( !bSearchFinished )
	{
		if( FindNextFileA(hSearch, &fd) )
		{
			if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )       
			{
				strcpy(tmpPath, rootDir);
				strcat(tmpPath, fd.cFileName);
				strcat(tmpPath, "\\");
				findFileInDir(tmpPath, fileNameVec);
			}
			else if( strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..") )
			{
				if( rootDir[strlen(rootDir) -1] != '\\' )
					sprintf(fname, "%s\\%s\0",rootDir,fd.cFileName);
				else
					sprintf(fname, "%s%s\0",rootDir,fd.cFileName);
				fileNameVec.push_back(fname);
			}
		}
		else
		{
			if( GetLastError() == ERROR_NO_MORE_FILES )
			{
				bSearchFinished = TRUE;
			}
			else
				bSearchFinished = TRUE;
		}
	}
	FindClose(hSearch);
}
std::string convertFileNameToUnixStyle(const std::string& fileName)
{
	std::string new_fileName(fileName);

	std::string::size_type slash = 0;
	while( (slash=new_fileName.find_first_of('\\',slash)) != std::string::npos)
	{
		new_fileName[slash]='/';
	}

	return new_fileName;
}
bool makeDirectory( const std::string &path )
{
	if (path.empty())
	{
		return false;
	}

	struct stat64 stbuf;
	if( stat64( path.c_str(), &stbuf ) == 0 )
	{
		if( S_ISDIR(stbuf.st_mode))
			return true;
		else
		{
			return false;
		}
	}

	std::string dir = path;
	std::stack<std::string> paths;
	while( true )
	{
		if( dir.empty() )
			break;

		if( stat64( dir.c_str(), &stbuf ) < 0 )
		{
			switch( errno )
			{
			case ENOENT:
			case ENOTDIR:
				paths.push( dir );
				break;

			default:
				return false;
			}
		}
		dir = getFilePath(std::string(dir));
	}

	while( !paths.empty() )
	{
		std::string dir = paths.top();
#if defined(WIN32)
		if (dir.size() == 2 && dir.c_str()[1] == ':') {
			paths.pop();
			continue;
		}
#endif
		if( mkdir( dir.c_str(), 0755 )< 0 )
		{
			return false;
		} 
		paths.pop();
	}
	return true;
}
bool makeDirectoryForFile( const std::string &path )
{
	return makeDirectory( getFilePath( path ));
}
std::string getFileExtension(const std::string& fileName)
{
	std::string::size_type dot = fileName.find_last_of('.');
	if (dot==std::string::npos) return std::string("");
	return std::string(fileName.begin()+dot+1,fileName.end());
}
bool Creature3DAPI::copyFile(const std::string &srcFile, const std::string &destFile)
{
	if(fileExists(destFile))
	{
		std::string _destFile = "_"+destFile;
		makeDirectoryForFile(_destFile);
		if(fileExists(_destFile))
			DeleteFileA(_destFile.c_str());
		MoveFileA(destFile.c_str(),_destFile.c_str());
	}
	if(fileExists(srcFile))
	{
		std::string src = convertFileNameToUnixStyle(srcFile);
		if(src.find("./") == 0)
		{
			src.erase(0,2);
		}
		std::string dest = convertFileNameToUnixStyle(destFile);
		if(dest.find("./") == 0)
		{
			dest.erase(0,2);
		}
		if(src.compare(dest) != 0)
		{
			makeDirectoryForFile(destFile);
			std::ofstream out(destFile.c_str(),std::ios::binary|std::ios::out);
			if(!out)
			{
				char buf[128];
				sprintf(buf,"错误:无法更新文件[%s],请确认文件是否被锁定(比如被杀毒软件错误隔离)。\0",destFile.c_str());
				MessageBoxA(::GetDesktopWindow(),buf,"启动更新",MB_OK);
				return false;
			}
			else
			{
				std::ifstream in(srcFile.c_str(),std::ios::binary|std::ios::in);
				out << in.rdbuf();
				out.close();
				in.close();
				return true;
			}
		}
	}
	return false;
}
//////////////VersionUpdate
DWORD WINAPI  StartDownload(void* ctrl) 
{
	Sleep(10);
	Creature3DAPI *theCtrl = (Creature3DAPI *)ctrl;

	std::string file = theCtrl->m_currentfile;
	Creature3DAPI::DownloadTaskMap::iterator itr = theCtrl->m_downloadmap.find(file);
	if(itr != theCtrl->m_downloadmap.end())
	{
		HINTERNET internetopenurl = itr->second.second.second;

		std::string filename = theCtrl->m_savepath+"/"+file;
		makeDirectoryForFile(filename);
		HANDLE distFile=CreateFileA(filename.c_str(),GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		if (distFile==INVALID_HANDLE_VALUE){
			char szError[1024] = {0};
			wsprintfA(szError,"创建文件（%s）失败! 通常是您没有写文件权限引起，请尝试重置Internet Explorer设置!\0",filename.c_str());
			MessageBoxA(::GetActiveWindow(),szError,"创建文件失败",MB_OK);
			InternetCloseHandle(internetopenurl);
			HANDLE thread = itr->second.second.first;
			theCtrl->m_closeThreadHandleDeque.push_back(thread);
			theCtrl->m_downloadmap.erase(file);
			return 0;
		}

		BOOL hwrite;
		DWORD written;
		DWORD byteread=0;
		char buffer[1024];
		memset(buffer,0,1024);
		DWORD recvedsize = 0;

		DWORD dwFileSize = 0;
		DWORD dwSize = sizeof(dwFileSize);
		::HttpQueryInfo(internetopenurl,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwFileSize, &dwSize, NULL);
		//char buf[256] = {0};
		//sprintf(buf,"dwFileSize=%d\0",dwFileSize);
		//MessageBoxA(NULL,buf,"dwFileSize",MB_OK);
		float progress = 0.0f;
		while (!(theCtrl->m_shutdown))
		{
			BOOL internetreadfile=InternetReadFile(internetopenurl,buffer,sizeof(buffer),&byteread);
			if(byteread==0)
			{
				break;
			}
			hwrite=WriteFile(distFile,buffer,byteread,&written,NULL);
			if (hwrite==0){
				char szError[1024] = {0};
				sprintf(szError,"写文件出错，请检查%s盘剩余空间是否充足！",theCtrl->m_disk.c_str());
				MessageBoxA(::GetActiveWindow(),szError,"文件下载出错",MB_OK);
				break;
			}
			recvedsize += byteread;
			progress = float(recvedsize)/float(dwFileSize);
			if(progress>0.99f) progress = 0.99f;
			theCtrl->m_downloadmap[file].first = progress;

			//HWND hWnd = theCtrl->getPlugin().get()->getHwnd();
			//RECT rc;  
			//GetClientRect(hWnd, &rc); 
			//InvalidateRect(hWnd,&rc,true);
		}
		InternetCloseHandle(internetopenurl);
		CloseHandle(distFile);
		if(recvedsize != dwFileSize)//文件大小不正确，下载失败
			DeleteFileA(filename.c_str());
		else
		{//解压放到WeiruiVR做
			std::string ext = getFileExtension(filename.c_str());
			bool is7zfile = false;
			if(ext=="7z")
			{
				is7zfile = true;
			}
			if(is7zfile)
			{
				char buf[1024];
				memset(buf,0,1024);
				sprintf_s(buf,"%s/7z.exe x -r %s -y -o%s\0",theCtrl->m_dllpath.c_str(),filename.c_str(),theCtrl->m_uncompresspath.c_str());
				//MessageBoxA(NULL,buf,"UnCompress",MB_OK);
				PROCESS_INFORMATION pi;
				STARTUPINFOA si; //This is an [in] parameter
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof si; //Only compulsory field
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_HIDE;
				if(CreateProcessA(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
				{
					WaitForSingleObject(pi.hProcess,INFINITE);
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
					//MessageBoxA(NULL,filename.c_str(),"success",MB_OK);
				}
				else
				{//解压文件出错
					char szError[1024] = {0};
					sprintf(szError,"文件无法正常解压，请检查%s盘剩余空间是否充足！",theCtrl->m_disk.c_str());
					MessageBoxA(::GetActiveWindow(),szError,"文件解压出错",MB_OK);
					//DeleteFileA(filename.c_str());
				}
				if(filename.find("Creature3D.7z") != std::string::npos)
				{
					theCtrl->versionUpdate();
				}
			}
		}
		HANDLE thread = itr->second.second.first;
		theCtrl->m_closeThreadHandleDeque.push_back(thread);
		theCtrl->m_downloadmap.erase(file);
	}
	return 0;
}
void GetDiskInfo(DWORD &dwNum, std::string chDriveInfo[], unsigned __int64 iDiskFreeSize[],char disk[])
{
	DWORD DiskCount = 0;

	//利用GetLogicalDrives()函数可以获取系统中逻辑驱动器的数量，函数返回的是一个32位无符号整型数据。
	DWORD DiskInfo = GetLogicalDrives();

	////通过循环操作查看每一位数据是否为1，如果为1则磁盘为真,如果为0则磁盘不存在。
	//while(DiskInfo)
	//{
	//	//通过位运算的逻辑与操作，判断是否为1
	//	Sleep(10);
	//	if(DiskInfo&1)
	//	{
	//		DiskCount++;
	//	}
	//	DiskInfo = DiskInfo >> 1;//通过位运算的右移操作保证每循环一次所检查的位置向右移动一位。*/
	//}

	//if (dwNum < DiskCount)
	//{
	//	return;//实际的磁盘数目大于dwNum
	//}
	//dwNum = DiskCount;//将磁盘分区数量保存

	//通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度
	int DSLength = GetLogicalDriveStringsA(0, NULL);

	char* DStr = new char[DSLength];
	memset(DStr, 0, DSLength);

	//通过GetLogicalDriveStrings将字符串信息复制到堆区数组中,其中保存了所有驱动器的信息。
	GetLogicalDriveStringsA(DSLength,DStr);

	int DType;
	//int si=0;
	BOOL fResult;
	unsigned __int64 i64FreeBytesToCaller;
	unsigned __int64 i64TotalBytes;
	unsigned __int64 i64FreeBytes;

	//读取各驱动器信息，由于DStr内部数据格式是A:\NULLB:\NULLC:\NULL，所以DSLength/4可以获得具体大循环范围
	char buf[128];
	for(int i=0; i<DSLength/4; ++i)
	{
		Sleep(10);
		std::string strdriver = DStr + i*4;
		std::string strTmp,strTotalBytes, strFreeBytes;
		DType = GetDriveTypeA(strdriver.c_str());//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
		if(DType == DRIVE_FIXED)
		{
			disk[DiskCount] = strdriver[0];
			//GetDiskFreeSpaceEx函数，可以获取驱动器磁盘的空间状态,函数返回的是个BOOL类型数据
			fResult = GetDiskFreeSpaceExA (strdriver.c_str(), (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);
			if(fResult)
			{
				iDiskFreeSize[DiskCount] = i64FreeBytesToCaller;
				sprintf(buf,"磁盘总容量%fMB\0",(float)i64TotalBytes/1024/1024);
				strTotalBytes=buf;
				sprintf(buf,"磁盘剩余空间%fMB\0",(float)i64FreeBytesToCaller/1024/1024);
				strFreeBytes = buf;
			}
			else
			{
				strTotalBytes="";
				strFreeBytes="";
			}
			chDriveInfo[DiskCount] = /*strTmp + */"(" + strdriver + "):" + strTotalBytes + strFreeBytes;
			DiskCount++;
		}
		//si+=4;
		//switch (DType)
		//{
		//case DRIVE_FIXED:
		//	{
		//		strTmp="本地磁盘";
		//	}
		//	break;
		//case DRIVE_CDROM:
		//	{
		//		strTmp="DVD驱动器";
		//	}
		//	break;
		//case DRIVE_REMOVABLE:
		//	{
		//		strTmp="可移动磁盘";
		//	}
		//	break;
		//case DRIVE_REMOTE:
		//	{
		//		strTmp="网络磁盘";
		//	}
		//	break;
		//case DRIVE_RAMDISK:
		//	{
		//		strTmp="虚拟RAM磁盘";
		//	}
		//	break;
		//case DRIVE_UNKNOWN:
		//	{
		//		strTmp="虚拟RAM未知设备";
		//	}
		//	break;
		//default:
		//	strTmp="未知设备";
		//	break;
		//}
	}
	dwNum = DiskCount;
}
void Creature3DAPI::setHost(const std::string& str)
{
	m_downloadHost = str;
	//HWND hWnd = getPlugin().get()->getHwnd();
	//SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this); 
	//lpOldProc = SubclassWindow(hWnd, (WNDPROC)PluginWinProc);
	char buf[_MAX_PATH];
	ZeroMemory(buf,_MAX_PATH);
	SHGetSpecialFolderPathA(NULL,buf,CSIDL_SYSTEM,0);
	sprintf(buf,"%c:/Creature3DPlayer\0",buf[0]);
	m_dllpath = buf;
	//MessageBoxA(NULL,m_dllpath.c_str(),"m_dllpath",MB_OK);
	std::string inifile = m_dllpath+"/disk.dat";
	//MessageBoxA(NULL,inifile.c_str(),"inifile",MB_OK);
	if(fileExists(inifile))
	{
		std::ifstream fin;
		fin.open(inifile.c_str());
		fin >> m_disk;
		fin.close();
		//MessageBoxA(NULL,m_disk.c_str(),"fin",MB_OK);
	}
	if(m_disk.empty())
	{
		DWORD dwNum = 50;
		enum { eBufferSize = 50, };
		std::string strDiskInfo[eBufferSize];
		unsigned __int64 iDiskFreeSize[eBufferSize];
		char disk[eBufferSize];
		ZeroMemory(iDiskFreeSize, eBufferSize * sizeof(unsigned _int64));
		GetDiskInfo(dwNum, strDiskInfo, iDiskFreeSize,disk);
		if (dwNum > 5)//cdefgh
			dwNum = 5;
		//C盘和D盘比较大小
		const char * pTmp = NULL;
		char cDisk = disk[0];//L'C';
		unsigned __int64 maxDiskFreeSize = 0;
		//unsigned __int64 uTmpValue = 20 * 1024 * 1024;
		//uTmpValue *= 1024;
		for(int i = 0; i<dwNum; i++)
		{
			//MessageBoxA(NULL,strDiskInfo[i].c_str(),"fout",MB_OK);
			//pTmp = strstr(strDiskInfo[i].c_str(), "本地磁盘(");
			if (/*pTmp && */iDiskFreeSize[i] >= maxDiskFreeSize)	//如果C盘剩余小于20G则安装在D盘，以后不比较大小了
			{
				maxDiskFreeSize = iDiskFreeSize[i];
				cDisk = disk[i];//L'D';
				//MessageBoxA(NULL,strDiskInfo[i].c_str(),"setHost",MB_OK);
			}
		}
		m_disk = cDisk;
		std::ofstream fout;
		fout.open(inifile.c_str());
		fout << m_disk;
		fout.close();
		//MessageBoxA(NULL,m_disk.c_str(),"fout",MB_OK);
	}
	m_apppath = m_disk+":/Creature3D";
}
void Creature3DAPI::setSavePath(const std::string& str)
{
	m_savepath = m_apppath+"/"+str;
	makeDirectory(m_savepath);
	//MessageBoxA(NULL,m_savepath.c_str(),"SavePath",MB_OK);
}
void Creature3DAPI::setUnCompressPath(const std::string& str)
{
	m_uncompresspath = m_apppath+"/"+str;
	makeDirectory(m_uncompresspath);
	//MessageBoxA(NULL,m_uncompresspath.c_str(),"UnCompressPath",MB_OK);
}
char Creature3DAPI::download(const std::string& url)
{
	return downloadAndRename(url);
}
char Creature3DAPI::downloadAndRename(const std::string& url,std::string filerename)
{
	if (!url.empty())
	{
		MessageBoxA(::GetActiveWindow(),url.c_str(),"downloadAndRename",MB_OK);
		std::string filename = filerename.empty()?getSimpleFileName(url):filerename;
		if(isFileExist(filename))
		{//-2文件已经存在
			return -2;
		}
		if(m_downloadmap.size()>5)
		{//-3下载任务数量超限（最多5个）
			return -3; 
		}
		DownloadTaskMap::iterator itr = m_downloadmap.find(filename);
		if(itr != m_downloadmap.end())
		{//-4已经在下载
			return -4;
		}

		if(m_internetopen==NULL)
		{
			m_internetopen=InternetOpenA(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		}
		if (m_internetopen==NULL){
			MessageBoxA(::GetActiveWindow(),"Internet open failed!","error",MB_OK);
			return -5;
		}
		//std::string fullurl = m_downloadHost.empty()?url:m_downloadHost+"/"+url;
		std::string fullurl;
		if (m_downloadHost.empty())
			fullurl = url;
		else
		{
			if ((*m_downloadHost.rbegin()) == '/')
				fullurl = m_downloadHost + url;
			else
				fullurl = m_downloadHost + "/" + url;
		}
		HINTERNET internetopenurl=InternetOpenUrlA(m_internetopen,fullurl.c_str(),NULL,0,INTERNET_FLAG_RELOAD,0);
		if (internetopenurl==NULL){
			char szError[1024] = {0};
			wsprintfA(szError,"url:%s\0",fullurl.c_str());
			MessageBoxA(::GetActiveWindow(),szError,"Internet open url failed!",MB_OK);
			//InternetCloseHandle(m_internetopen);
			return -5;
		}
		m_currentfile = filename;
		m_downloadmap[filename] = std::make_pair(0.0f,std::make_pair((HANDLE)0,internetopenurl));
		HANDLE thread = CreateThread(NULL, 0, StartDownload, (void*)this, 0, 0);
		itr = m_downloadmap.find(filename);
		if(itr != m_downloadmap.end())
			itr->second.second.first = thread;
		return 0;
	}
	HANDLE closethread;
	while(!m_closeThreadHandleDeque.empty())
	{
		closethread = m_closeThreadHandleDeque.front();
		m_closeThreadHandleDeque.pop_front();
		CloseHandle( closethread );
	}
	return -1;//-1传入的文件名是空的
}
char Creature3DAPI::versionCheckAndUpdate(const std::string &url)
{
	setSavePath("UpdateDown");
	setUnCompressPath("UpdateFiles");
	deleteFile("Creature3D.ver");
	download(url+"Creature3D.ver");
	std::string serverVerFile = m_savepath+"/"+"Creature3D.ver";
	while(!fileExists(serverVerFile))
	{
		Sleep(10);
	}
	std::string localVerFile = m_apppath+"/Creature3D.ver";
	std::string localExeFile = m_apppath+"/bin/Creature3D.dll";
	//std::string localver,serverver;
	std::ifstream fin;
	fin.open(serverVerFile.c_str());
	fin >> m_serverVer;
	fin.close();
	if(fileExists(localVerFile) && fileExists(localExeFile))
	{
		fin.open(localVerFile.c_str());
		fin >> m_localVer;
		fin.close();
	}
	if(m_localVer.empty() || (!m_serverVer.empty() && m_localVer != m_serverVer))
	{//需要做版本更新
		//MessageBoxA(NULL,m_localVer.c_str(),"客户端版本号",MB_OK);
		//MessageBoxA(NULL,m_serverVer.c_str(),"服务器版本号",MB_OK);
		deleteFile("Creature3D.7z");
		download(url+"Creature3D.7z");
		//versionUpdate();//test
		return 1;
	}
	return 0;
}
float Creature3DAPI::getVersionUpdateProgress()
{
	return getDownloadProgress("Creature3D.7z");
}
std::string Creature3DAPI::getLocalVersion()
{
	return m_localVer;
}
std::string Creature3DAPI::getServerVersion()
{
	return m_serverVer;
}
void Creature3DAPI::versionUpdate()
{
	std::vector<std::string> fileNameVec;
	std::string rootdir = m_uncompresspath+"/";
	findFileInDir(rootdir.c_str(),fileNameVec);
	//char buf[256];
	//sprintf(buf,"findFileInDir文件数量：%d,%s\0",fileNameVec.size(),fileNameVec[0].c_str());
	//MessageBoxA(NULL,buf,"findFileInDir",MB_OK);
	if(!fileNameVec.empty())
	{
		//system("move /y jxjpkg\\bin\\*.* bin");
		std::string filename;
		int dirlen = m_uncompresspath.length();
		//SetCurrentDirectoryA(m_dllpath.c_str());
		for( std::vector<std::string>::iterator itr = fileNameVec.begin();
			itr != fileNameVec.end();
			++itr )
		{
			filename = *itr;
			if(filename.find("vcredist_x86.exe")!=std::string::npos)
			{
				if(m_localVer.empty())
				{
					char buf[1024];
					memset(buf,0,1024);
					sprintf_s(buf,"%s\0",filename.c_str());
					PROCESS_INFORMATION pi;
					STARTUPINFOA si; //This is an [in] parameter
					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof si; //Only compulsory field
					si.dwFlags = STARTF_USESHOWWINDOW;
					si.wShowWindow = SW_SHOW;
					if(CreateProcessA(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi))
					{
						WaitForSingleObject(pi.hProcess,INFINITE);
						CloseHandle(pi.hThread);
						CloseHandle(pi.hProcess);
					}
				}
			}
			else
			{
				filename.erase(0,dirlen);
				if(versionUpdateCopyFile(*itr,filename))
					DeleteFileA((*itr).c_str());
			}
		}
	}
	std::string verfile = "/Creature3D.ver";
	std::string serverVerFile = m_savepath+"/Creature3D.ver";
	versionUpdateCopyFile(serverVerFile,verfile);
	//MessageBoxA(NULL,"版本更新完成","版本更新",MB_OK);
}
float Creature3DAPI::getDownloadProgress(const std::string& str)
{
	float progress = 0.0f;
	if (!str.empty())
	{
		Creature3DAPI::DownloadTaskMap::iterator itr = m_downloadmap.find(str);
		if(itr!=m_downloadmap.end())
			progress = itr->second.first;
		else if(isFileExist(str))
			progress = 1.0f;
	}
	return progress;
}
bool Creature3DAPI::isFileExist(const std::string& str)
{//返回true:表示本地存在该文件，false表示不存在
	std::string file = m_savepath+"/"+str;
	return fileExists(file);
}
void Creature3DAPI::deleteFile(const std::string& str)
{//删除本地文件，用于可能用到的重新下载功能
	std::string file = m_savepath+"/"+str;
	if (fileExists(file))
		DeleteFileA(file.c_str());
}
bool Creature3DAPI::versionUpdateCopyFile(const std::string &srcFile, const std::string &destFile)
{
	std::string destpath;
	if(srcFile.find("npCreature3D.dll")!=std::string::npos)
	{
		destpath = m_dllpath;
	}
	else
	{
		destpath = m_apppath;
	}
	std::string relDestFile = destpath+destFile;
	if(fileExists(relDestFile))
	{
		std::string _destFile = destpath+"/bak"+destFile;
		makeDirectoryForFile(_destFile);
		if(fileExists(_destFile))
			DeleteFileA(_destFile.c_str());
		MoveFileA(relDestFile.c_str(),_destFile.c_str());
	}
	if(fileExists(srcFile))
	{
		std::string src = convertFileNameToUnixStyle(srcFile);
		if(src.find("./") == 0)
		{
			src.erase(0,2);
		}
		std::string dest = convertFileNameToUnixStyle(relDestFile);
		if(dest.find("./") == 0)
		{
			dest.erase(0,2);
		}
		if(src.compare(dest) != 0)
		{
			makeDirectoryForFile(relDestFile);
			std::ofstream out(relDestFile.c_str(),std::ios::binary|std::ios::out);
			if(!out)
			{
				char buf[1024];
				sprintf(buf,"错误:无法更新文件[%s],请确认文件是否被锁定(请关闭程序后手动拷贝过去),%s\0",relDestFile.c_str(),srcFile.c_str());
				MessageBoxA(::GetActiveWindow(),buf,"版本更新",MB_OK);
				return false;
			}
			else
			{
				std::ifstream in(srcFile.c_str(),std::ios::binary|std::ios::in);
				out << in.rdbuf();
				out.close();
				in.close();
				return true;
			}
		}
	}
	return false;
}