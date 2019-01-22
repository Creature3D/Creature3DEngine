// creBroswer.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "creBroswer.h"
#include <map>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CcreBroswerApp

BEGIN_MESSAGE_MAP(CcreBroswerApp, CWinApp)
END_MESSAGE_MAP()

// CcreBroswerApp 构造

CcreBroswerApp::CcreBroswerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

}


// 唯一的一个 CcreBroswerApp 对象

CcreBroswerApp theApp;
typedef std::map<HWND,creWebDialog *> DialogMap;
DialogMap g_dialogMap;
extern  "C" __declspec(dllexport) HWND create(int x,int y,int width,int height)
{
	creWebDialog *dlg = new creWebDialog;
	dlg->Create(IDD_DIALOG1,NULL);
	dlg->ShowWindow(SW_HIDE);
	//g_dialog.MoveWindow(400,300,640,512);
	dlg->MoveWindow(x,y,width,height);
	HWND hwnd = dlg->GetSafeHwnd();
	g_dialogMap[hwnd] = dlg;
	return hwnd;
}
//extern  "C" __declspec(dllexport) HWND getHWND()
//{
//	return g_dialog.GetSafeHwnd();
//}
extern  "C" __declspec(dllexport) void Navigate(HWND hwnd,const char* url)
{
	DialogMap::iterator itr = g_dialogMap.find(hwnd);
	if(itr != g_dialogMap.end())
	{
		creWebDialog *dlg = g_dialogMap[hwnd];
		if(dlg)
		{
			dlg->Navigate(url);
			dlg->ShowWindow(SW_SHOW);
		}
	}
}

extern  "C" __declspec(dllexport) void release()
{
	for (DialogMap::iterator itr = g_dialogMap.begin();
		itr != g_dialogMap.end();
		++itr)
	{
		delete itr->second;
		itr->second = nullptr;
	}
	g_dialogMap.clear();
}

// CcreBroswerApp 初始化
void CcreBroswerApp::WriteWebBrowserRegKey(LPCTSTR lpKey, DWORD dwValue)
{
	HKEY hk;
	CString str = "Software\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\";
	str += lpKey;
	if (RegCreateKey(HKEY_CURRENT_USER, str, &hk) == 0)
	{
		RegSetValueEx(hk, m_exeName, NULL, REG_DWORD, (const byte*)&dwValue, 4);
		RegCloseKey(hk);
	}
}
BOOL CcreBroswerApp::InitInstance()
{
	CWinApp::InitInstance();

	//先获取运行程序的完整路径
	char szFileName[256];
	memset(szFileName, '\0', sizeof(szFileName));

	GetModuleFileName(NULL, szFileName, sizeof(szFileName));

	//再分割完整路径的字符串，最后一个就是程序的名字 
	char seps[] = "\\";
	char *token = NULL;

	memset(m_exeName, '\0', sizeof(m_exeName));

	token = strtok(szFileName, seps);
	while (token != NULL)
	{
		sprintf(m_exeName, "%s", token);
		token = strtok(NULL, seps);
	}
	//MessageBox(NULL, m_exeName, m_exeName, MB_OK);

	WriteWebBrowserRegKey("FEATURE_BROWSER_EMULATION", 10000);
	WriteWebBrowserRegKey("FEATURE_ACTIVEX_REPURPOSEDETECTION", 1);
	WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_IMG", 1);
	WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_OBJECT", 1);
	WriteWebBrowserRegKey("FEATURE_BLOCK_LMZ_SCRIPT", 1);
	WriteWebBrowserRegKey("FEATURE_Cross_Domain_Redirect_Mitigation", 1);
	WriteWebBrowserRegKey("FEATURE_ENABLE_SCRIPT_PASTE_URLACTION_IF_PROMPT", 1);
	WriteWebBrowserRegKey("FEATURE_LOCALMACHINE_LOCKDOWN", 1);
	WriteWebBrowserRegKey("FEATURE_GPU_RENDERING", 1);

	return TRUE;
}
