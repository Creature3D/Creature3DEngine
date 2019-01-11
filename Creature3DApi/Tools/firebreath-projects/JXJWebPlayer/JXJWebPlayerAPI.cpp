/**********************************************************\

  Auto-generated JXJWebPlayerAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "JXJWebPlayerAPI.h"
#include "DOM.h"


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



extern std::string g_dllPath;

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
/// @fn JXJWebPlayerPtr JXJWebPlayerAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
JXJWebPlayerPtr JXJWebPlayerAPI::getPlugin()
{
    JXJWebPlayerPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string JXJWebPlayerAPI::get_testString()
{
    return m_testString;
}

void JXJWebPlayerAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string JXJWebPlayerAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

JXJWebPlayerAPI::~JXJWebPlayerAPI()
{
	
}

void JXJWebPlayerAPI::setParam(std::string key,std::string value)
{
	if (!key.empty() && !value.empty())
	{
		m_params[key] = value;
	}
}

std::string JXJWebPlayerAPI::getParam(std::string key)
{
	if (!key.empty())
	{
		return m_params[key];
	}
	return "";
}
void JXJWebPlayerAPI::download()
{
	HINTERNET internetopen;
	internetopen=InternetOpenA("Testing",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if (internetopen==NULL){
		MessageBoxA(NULL,"Internet open failed!","error",MB_OK);
		return;
	}
	std::string vcredisturl = getParam("vcredist");
	m_internetopenurl=InternetOpenUrlA(internetopen,vcredisturl.c_str()/*"http://jxj.quegame.com/vcredist_x86.exe"*/,NULL,0,INTERNET_FLAG_RELOAD,0);
	if (m_internetopenurl==NULL){
		MessageBoxA(NULL,"Internet open url failed!","error",MB_OK);
		InternetCloseHandle(internetopen);
		return;
	}
	m_downloadState = 1;
	RECT rc;  
	GetClientRect(m_hWnd, &rc); 
	InvalidateRect(m_hWnd,&rc,true);
	m_downloadThread = CreateThread(NULL, 0, StartDownload, (void*)this, 0, 0);
}
DWORD WINAPI  StartDownload(void* ctrl) 
{
	JXJWebPlayerAPI *theCtrl = (JXJWebPlayerAPI *)ctrl;
	if (!theCtrl)
	{
		theCtrl->m_downloadState = 2;
		MessageBoxA(NULL,"","error",MB_OK);
		return 0;
	}

	char buf[1024];
	wsprintfA(buf,"%s%s\0",theCtrl->m_clientPath.c_str(),"\\vcredist_x86.exe");
	HANDLE vcredistFile=CreateFileA(buf,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
	if (vcredistFile==INVALID_HANDLE_VALUE){
		char szError[256] = {0};
		wsprintfA(szError,"Create File failed! %u",GetLastError());
		MessageBoxA(NULL,szError,"error",MB_OK);
		InternetCloseHandle(theCtrl->m_internetopenurl);
		theCtrl->m_downloadState = 2;
		return 0;
	}

	BOOL hwrite;
	DWORD written;
	DWORD byteread=0;
	char buffer[1024];
	memset(buffer,0,1024);
	while (true)
	{
		BOOL internetreadfile=InternetReadFile(theCtrl->m_internetopenurl,buffer,sizeof(buffer),&byteread);
		if(byteread==0)
		{
			CloseHandle(vcredistFile);
			break;
		}
		hwrite=WriteFile(vcredistFile,buffer,sizeof(buffer),&written,NULL);
		if (hwrite==0){
			//"Write to file failed!"
			CloseHandle(vcredistFile);
			break;
		}
	}
	InternetCloseHandle(theCtrl->m_internetopenurl);
	MessageBoxA(NULL,"您的电脑缺少微软运行时库,点击开始安装.","提示",MB_OK);
	ShellExecuteA(theCtrl->m_hWnd,"open","vcredist_x86.exe",theCtrl->m_clientPath.c_str(),NULL,SW_SHOWNORMAL);
	theCtrl->m_downloadState = 2;
	RECT rc;  
	GetClientRect(theCtrl->m_hWnd, &rc); 
	InvalidateRect(theCtrl->m_hWnd,&rc,true);
	return 0;
}
DWORD WINAPI  StartCreature3DThread(void* ctrl) 
{
	JXJWebPlayerAPI *theCtrl = (JXJWebPlayerAPI *)ctrl;
	//MessageBoxA(NULL,"调用StartCreature3D","Creature3DX",MB_OKCANCEL);
	theCtrl->StartCreature3D();
	return 0;
}

void JXJWebPlayerAPI::Initialize()
{
	if(m_thread == NULL)
	{
		std::string _Path = getParam("gamedir");
		//MessageBoxA(NULL,_Path.c_str(),"_Path",MB_OKCANCEL);
		if(_Path.empty())
			_Path = WcharToChar(FB::utf8_to_wstring(g_dllPath).c_str());
		else
		{
			std::string tmppath = "C:/"+_Path;
			std::string dllpath = tmppath+"/Creature3D.dll";
			do 
			{
				if(fileExists(dllpath))
					break;
				tmppath = "D:/"+_Path;
				dllpath = tmppath+"/Creature3D.dll";
				if(fileExists(dllpath))
					break;
				tmppath = "E:/"+_Path;
				dllpath = tmppath+"/Creature3D.dll";
				if(fileExists(dllpath))
					break;
				tmppath = "F:/"+_Path;
				dllpath = tmppath+"/Creature3D.dll";
				if(fileExists(dllpath))
					break;
				tmppath = "G:/"+_Path;
				dllpath = tmppath+"/Creature3D.dll";
				if(fileExists(dllpath))
					break;
				tmppath = "H:/"+_Path;
				dllpath = tmppath+"/Creature3D.dll";
				if(fileExists(dllpath))
					break;
				//MessageBoxA(NULL,dllpath.c_str(),"文件未找到",MB_OKCANCEL);
				//需要重新安装游戏
				std::string pluginurl = getParam("pluginurl");
				getHost()->Navigate(pluginurl,"_self");
				return;
			} while (0);
			_Path = dllpath;
		}
		//std::string _Path = WcharToChar(FB::utf8_to_wstring(g_dllPath).c_str());
		m_clientPath = getFilePath(_Path);
		appendLibraryFilePaths(m_clientPath);
		m_clientPath.erase(m_clientPath.size()-4,4);

		m_hWnd = getPlugin().get()->getHwnd();
		if(!lpOldProc) lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);  

		// 将窗口与 Plugin 对象相关联，这样就可以在窗口处理中访问Plugin 对象   
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);  
		initDialog();
		//SetWindowLong(m_hWnd,GWL_USERDATA,(long)this);
		//SetWindowLongPtr(m_hWnd, GWLP_WNDPROC,(LPARAM)(WNDPROC)PluginWinProc);  

		//strcpy(m_szLoginInfo,getParam("strLoginInfo").c_str());
		//MessageBoxA(m_hWnd,"创建线程","Creature3DX",MB_OKCANCEL);
		m_thread = CreateThread(NULL, 0, StartCreature3DThread, (void*)this, 0, 0);
	}
}
void JXJWebPlayerAPI::shut()
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
	if(m_downloadThread)
	{
		CloseHandle(m_downloadThread);
		m_downloadThread = NULL;
	}
}
void ConvertGBKToUtf8(std::string& amp, std::string strGBK)
{ 
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	//strGBK = szUtf8; 
	amp=szUtf8;
	delete[] szUtf8; 
	delete[] wszUtf8; 
} 
//bool JXJWebPlayerAPI::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
//{	
//	switch (uMsg)
//	{  
//	case WM_USER+602:
//		{
//			std::string str = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
//			//MessageBox(NULL, str.c_str(), "xxx", MB_OK);
//			str += "&openid="+getParam("openid");
//			str += "&openkey="+getParam("openkey");
//			str += "&pf="+getParam("pf");
//			str += "&pfkey="+getParam("pfkey");
//			//std::string url ;
//			//ConvertGBKToUtf8(url,str);
//			getHost()->Navigate(str,"_blank");
//			//FB::DOM::WindowPtr window = getHost()->getDOMWindow();
//			//if (window && window->getJSObject()->HasProperty("window")) {
//			//	FB::JSObjectPtr obj = window->getJSObject();//window->getProperty<FB::JSObjectPtr>("Navigate");
//			//	// Invoke alert with some text
//			//	obj->Invoke("recharge", FB::variant_list_of(str));
//			//}
//		}
//		break;
//	case WM_USER+603:
//		{
//			std::string url = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
//			getHost()->Navigate(url,"_blank");
//			//FB::DOM::WindowPtr window = getHost()->getDOMWindow();
//			//if (window && window->getJSObject()->HasProperty("window")) {
//			//	FB::JSObjectPtr obj = window->getJSObject();//window->getProperty<FB::JSObjectPtr>("Navigate");
//			//	// Invoke alert with some text
//			//	obj->Invoke("Navigate", FB::variant_list_of(url));
//			//}
//		}
//		break;
//	case WM_PAINT:  
//		{  
//			// draw a frame and display some string   
//			PAINTSTRUCT ps;  
//			HDC hdc = BeginPaint(hWnd, &ps);  
//			RECT rc;  
//			GetClientRect(hWnd, &rc);  
//			FillRect( hdc, &rc,  GetStockBrush(BLACK_BRUSH));  
//
//			Sleep(100);
//			EndPaint(hWnd, &ps);  
//		}  
//		break;
//	case WM_USER+10:
//		{
//			needRestart();
//		}
//		break;
//	default:  
//		break;  
//	}  
//
//	return true;
//}
typedef __declspec(dllimport) HWND INITDIALOG(int x,int y,int width,int height);
void JXJWebPlayerAPI::initDialog()
{
	if(!m_dialog)
	{
		char buf[256];
		wsprintfA(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\creBroswer.dll");
		m_dialog = LoadLibraryA(buf);
		if(m_dialog)
		{
			INITDIALOG* dialogNavigate;
			dialogNavigate = (INITDIALOG*)GetProcAddress (m_dialog, "create");
			if (dialogNavigate)
			{
				m_dlgHwnd = (*dialogNavigate)(400,300,640,512);
			}
		}
		//else
		//{
		//	char szError[256] = {0};
		//	sprintf(szError,"Load creBroswer.dll Faild %u",GetLastError());
		//	MessageBoxA(NULL,szError,"error",MB_OK);
		//}
	}
}
typedef __declspec(dllimport) void RUNDIALOG(HWND hwnd,const char *);
void JXJWebPlayerAPI::showDialog(const char* url)
{
	if(m_dialog)
	{
		RUNDIALOG* dialogNavigate;
		dialogNavigate = (RUNDIALOG*)GetProcAddress (m_dialog, "Navigate");
		if (dialogNavigate)
		{
			(*dialogNavigate)(m_dlgHwnd,url);
		}
	}
}
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{  
	switch (msg) {  
	case WM_USER+602:
		{
			std::string str = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
			JXJWebPlayerAPI * p = (JXJWebPlayerAPI*) GetWindowLongPtr(hWnd, GWLP_USERDATA);  
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
				p->showDialog(str.c_str());
				//ShellExecute(::GetActiveWindow(),L"open",L"crExplorer.exe",url.c_str(),NULL,SW_HIDE);
			}
		}
		break;
	case WM_USER+603:
		{
			std::string url = (char*)((COPYDATASTRUCT*)lParam)->lpData;	
			//MessageBox(NULL,url.c_str(),"",MB_OK);
			JXJWebPlayerAPI * p = (JXJWebPlayerAPI*) GetWindowLong(hWnd, GWLP_USERDATA);  
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
	case WM_PAINT:  
		{  
			PAINTSTRUCT ps;  
			HDC hdc = BeginPaint(hWnd, &ps);  
			RECT rc;  
			JXJWebPlayerAPI * p = (JXJWebPlayerAPI*) GetWindowLong(hWnd, GWLP_USERDATA);  
			GetClientRect(hWnd, &rc);  
			std::string str;

			SetTextColor(hdc, RGB(230, 0, 0));
			int x = (rc.right-rc.left)/2 -100;
			int y = (rc.bottom-rc.top)/2 - 40;
			if (p)
			{
				if(p->m_downloadState == 1)
				{
					str = "发现关键文件缺失,请等待...";
				}
				else if(p->m_downloadState == 2)
				{
					str = "安装完成后请刷新进入游戏";
				}
				TextOutA(hdc, x, y, str.c_str(), str.length());
				EndPaint(hWnd, &ps);
			}else
			{
				// draw a frame and display some string   
				FillRect( hdc, &rc,  GetStockBrush(BLACK_BRUSH));  
				Sleep(100);
				EndPaint(hWnd, &ps);  
			}
			
			
			//FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));  
			//Plugin * p = (Plugin*) GetWindowLongPtr(hWnd, GWLP_USERDATA);  
			//if(p) {  
			//	char *s = "PLUGIN , PLUGIN!";//p->GetGuiText();   
			//	DrawText(hdc, s, strlen(s), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);  
			//}  			
		}  
		break;
	case WM_USER+10:
		{
			JXJWebPlayerAPI *p=(JXJWebPlayerAPI *)GetWindowLong(hWnd,GWLP_USERDATA);
			p->needRestart();
		}
		break;
	default:  
		break;  
	}  

	return DefWindowProc(hWnd, msg, wParam, lParam);  
}  
typedef __declspec(dllimport) void EXITCREATURE3D();
void JXJWebPlayerAPI::exitGame()
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
void JXJWebPlayerAPI::needRestart()
{
	m_needRestart = true;
}
void JXJWebPlayerAPI::waitClose()
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
std::string getFilePath(const std::string& fileName)
{
	std::string::size_type slash1 = fileName.find_last_of('/');
	std::string::size_type slash2 = fileName.find_last_of('\\');
	if (slash1==std::string::npos) 
	{
		if (slash2==std::string::npos) return std::string();
		return std::string(fileName,0,slash2);
	}
	if (slash2==std::string::npos) return std::string(fileName,0,slash1);
	return std::string(fileName, 0, slash1>slash2 ?  slash1 : slash2);
}

void appendLibraryFilePaths(const std::string &path)
{
	static bool inited = false;
	if(!inited)
	{
		static char newPath[1024];
		char* ptr;
		if ((ptr = getenv( "PATH" )))
		{
			sprintf(newPath,"PATH=%s;%s\0",path.c_str(),ptr);
			putenv(newPath);
			
		}
		else
		{
			sprintf(newPath,"PATH=%s\0",path.c_str());
			putenv(newPath);
		}
		inited = true;
	}
}
void JXJWebPlayerAPI::Release()  
{  
	if(m_char)  
	{  
		delete m_char;  
		m_char=NULL;  
	}  
	if(m_wchar)  
	{  
		delete m_wchar;  
		m_wchar=NULL;  
	}  
} 

wchar_t* JXJWebPlayerAPI::CharToWchar(const char* c)  
{  
	Release();  
	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);  
	m_wchar=new wchar_t[len+1];  
	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);  
	m_wchar[len]='\0';  
	return m_wchar;  
} 

char* JXJWebPlayerAPI::WcharToChar(const wchar_t* wc)   
{   
	Release();   
	int len= WideCharToMultiByte(CP_ACP,0,wc,wcslen(wc),NULL,0,NULL,NULL);   
	m_char=new char[len+1];   
	WideCharToMultiByte(CP_ACP,0,wc,wcslen(wc),m_char,len,NULL,NULL);   
	m_char[len]='\0';   
	return m_char;   
}   

void JXJWebPlayerAPI::gameLog(int v)
{
	FB::DOM::WindowPtr window = getHost()->getDOMWindow();
	if (window && window->getJSObject()) {
		FB::JSObjectPtr obj = window->getJSObject();
		// Invoke alert with some text
		obj->Invoke(/*"recordqq"*/"record", FB::variant_list_of(v));
	}
}
typedef __declspec(dllimport) int RUNCREATURE3D(char *);
void JXJWebPlayerAPI::StartCreature3D(/*HWND hWndParent, HWND hWnd, int x, int y, int width, int height*/)
{
	SetCurrentDirectoryA(m_clientPath.c_str());
	//////////版本文件检测与更新
	std::vector<std::string> fileNameVec;
	findFileInDir("jxjpkg\\bin",fileNameVec);
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
	WriteLoginInfo2File();

	if(!m_creature3d)
	{
		char buf[1024];
#ifdef _DEBUG
		wsprintf(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\Creature3Dd.dll");
#else
		wsprintfA(buf,"%s%s\0",m_clientPath.c_str(),"\\bin\\Creature3D.dll");
#endif
		//MessageBox(NULL,buf,"buf",MB_OK);
		m_creature3d = LoadLibraryA(buf);
	}
	if(m_creature3d)
	{
		gameLog(1);
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
			char argv[2048];
			sprintf(argv,"-hWndParent %d -Wnd %d -xpos %d -ypos %d -width %d -height %d -WebLogin %s\0",(int)m_hWnd,0,(int)x,(int)y,(int)width,(int)height,getParam("strLoginInfo").c_str());
			//sprintf_s(argv,"-hWndParent %d -Wnd %d -xpos %d -ypos %d -width %d -height %d\0",(int)m_hWnd,0,(int)x,(int)y,(int)width,(int)height);
			//MessageBoxA(m_hWnd,argv,"Creature3DX",MB_OKCANCEL);
			(*creature3DMain)(argv);
		}
		FreeLibrary(m_creature3d);
		m_creature3d = NULL;
	}
	else
	{
		char buf[2048];
		wsprintfA(buf, "%s/bin/Jiangxingjue.exe -WebLogin %s\0", m_clientPath.c_str(), getParam("strLoginInfo").c_str());
		//MessageBoxA(NULL, buf, "Creature3DAPI", MB_OK);
		//WinExec(buf, SW_SHOW);
		PROCESS_INFORMATION pi;
		STARTUPINFOA si; //This is an [in] parameter
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof si; //Only compulsory field
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		CreateProcessA(NULL, buf, NULL,NULL,FALSE,0,NULL, NULL,&si,&pi);
		//gameLog(0);
		///*char szError[256] = {0};
		//sprintf(szError,"Load Creature3D.dll Faild %u",GetLastError());
		//MessageBox(NULL,szError,"error",MB_OK);*/
		////if (GetLastError() == 126)
		//{
		//	download();
		//}
	}
	if(m_needRestart)
	{
		m_needRestart = false;
		StartCreature3D();
	}
}

void JXJWebPlayerAPI::WriteLoginInfo2File()
{
	FILE * pFile = fopen("jxjlogin.dat", "wb");
	std::string info = getParam("strLoginInfo");
	fwrite(info.c_str(), 1, info.length(), pFile);
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
bool fileExists(const std::string& filename)
{
	return access( filename.c_str(), F_OK ) == 0;
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
bool JXJWebPlayerAPI::copyFile(const std::string &srcFile, const std::string &destFile)
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