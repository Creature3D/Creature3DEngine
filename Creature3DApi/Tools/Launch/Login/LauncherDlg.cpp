// LauncherDlg.cpp : implementation file
//
//#include <Windows.h>
#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "resource.h"
#include <sys/stat.h>
#include <fstream>
#include "SystemInfo.h"
#include <gl/GL.h>
using namespace std;
#pragma comment(lib, "Opengl32.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CWebBrowser, CHtmlView)

CWebBrowser::CWebBrowser()
: m_bContextMenuEnabled(TRUE)
{
}

CWebBrowser::~CWebBrowser()
{
}

BEGIN_MESSAGE_MAP(CWebBrowser, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CWebBrowser 消息处理程序
void CWebBrowser::OnTitleChange(LPCTSTR lpszText)
{
	// TODO: 在此添加专用代码和/或调用基类

	CHtmlView::OnTitleChange(lpszText);
}

void CWebBrowser::OnDocumentComplete(LPCTSTR lpszURL)
{
	// TODO: 在此添加专用代码和/或调用基类

	CHtmlView::OnDocumentComplete(lpszURL);
}


void CWebBrowser::OnDestroy()
{
	m_pBrowserApp.Release();

	m_wndBrowser.DestroyWindow();

	CWnd::OnDestroy();
}

int CWebBrowser::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CWebBrowser::PostNcDestroy()
{
	//防止自删除
	//CHtmlView::PostNcDestroy();
}

BOOL CWebBrowser::PreTranslateMessage(MSG* pMsg)
{
	if (WM_RBUTTONUP == pMsg->message || WM_CONTEXTMENU == pMsg->message)
	{
		if (!IsContextMenuEnabled()) //屏蔽右键菜单和键盘菜单键
		{
			return TRUE;
		}
	}

	return CHtmlView::PreTranslateMessage(pMsg);
}

HRESULT CWebBrowser::OnGetHostInfo(DOCHOSTUIINFO *pInfo)
{
	pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO | DOCHOSTUIFLAG_NO3DBORDER;
	return S_OK;
}

#define _USER_NAME_	L"?user="
void CWebBrowser::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	if (wcsstr(lpszURL, _USER_NAME_))
	{
		wchar_t szBuf[256] = {0};
		wcscpy(szBuf, lpszURL);
		wchar_t * pTmp = wcsstr(szBuf, _USER_NAME_);
		if (pTmp)
		{
			pTmp += wcslen(_USER_NAME_);

			//转换字符串
			char szTmp[256] = {0};
			WideCharToMultiByte( CP_ACP, 0, pTmp, -1, szTmp, 255, NULL, NULL );
			CString strPath = m_strGameDir;
			//AfxMessageBox(m_strGameDir);
			strPath += L"/jxjlogin.dat";
			FILE * pFile = _wfopen(strPath, L"wb");
			if (NULL == pFile)
			{
				AfxMessageBox(L"jxjlogin.dat 写入失败");
				return ;
			}

			fwrite(szTmp, strlen(szTmp), 1, pFile);
			fclose(pFile);

			::Sleep(1);

			//运行游戏
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory( &si, sizeof(si) );	// 将启动信息结构清零
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );	// 将进程信息结构清零

			//调试版本的路径
			strPath = m_strGameDir;
			strPath += L"/Launcher.exe";
			//AfxMessageBox(strPath);
			CreateProcess(strPath, NULL, NULL, NULL, FALSE, 0, NULL, m_strGameDir, &si, &pi);

			//退出程序
			ExitProcess(0);
		}
	}
	else
		CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

HRESULT CWebBrowser::OnTranslateUrl(DWORD dwTranslate,
									OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	//AfxMessageBox(pchURLIn);
	if( wcslen(pchURLIn) == 0 || wcsstr(pchURLIn, _USER_NAME_) ||
		wcsstr(pchURLIn, L"micro") ||
		wcsstr(pchURLIn, L"4game") )
		return S_OK;
	if(	wcsstr(pchURLIn, L"http") )
	{
		ShellExecute(NULL,L"open",pchURLIn,NULL,NULL,SW_SHOW);
	}
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////

// CLauncherDlg dialog
CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialogSK(CLauncherDlg::IDD, pParent)
	, m_Download(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bQuitProcess = FALSE;
	m_uCurDownloadSize = 0;
	m_uCurDownloadFileSize = 1;
	m_bDownloadFinish = FALSE;

	m_strModule[0] = 0;

	m_btDownloadStage = 0;
}
CLauncherDlg::~CLauncherDlg()
{
	m_Download.Uninit();
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_START_GAME, m_btnStartGame);
	DDX_Control(pDX, IDC_BUTTON_MINI, m_btnMiniDlg);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_btnQuit);
	DDX_Control(pDX, IDC_PROGRESS1, m_Process1);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnVideoSetting);
	DDX_Control(pDX, IDC_CHECK1, m_btnVideoLevel[0]);
	DDX_Control(pDX, IDC_CHECK2, m_btnVideoLevel[1]);
	DDX_Control(pDX, IDC_CHECK3, m_btnVideoLevel[2]);
	DDX_Control(pDX, IDC_CHECK4, m_btnVideoLevel[3]);
	DDX_Control(pDX, IDC_CHECK5, m_btnVideoLevel[4]);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_SETTING_CANCLE, m_btnSelectCancel);
	//DDX_Control(pDX, IDC_INFO, m_staticText);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialogSK)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_START_GAME, &CLauncherDlg::OnBnClickedStartGame)
	ON_BN_CLICKED(IDC_BUTTON_MINI, &CLauncherDlg::OnBnClickedButtonMini)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CLauncherDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CLauncherDlg::OnBnClickedButtonVideoSetting)
	ON_BN_CLICKED(IDC_CHECK1,  &CLauncherDlg::OnBnClickedButtonLevelVeryLow)
	ON_BN_CLICKED(IDC_CHECK2, &CLauncherDlg::OnBnClickedButtonLevelLow)
	ON_BN_CLICKED(IDC_CHECK3, &CLauncherDlg::OnBnClickedButtonLevelNormal)
	ON_BN_CLICKED(IDC_CHECK4,  &CLauncherDlg::OnBnClickedButtonLevelHigh)
	ON_BN_CLICKED(IDC_CHECK5,  &CLauncherDlg::OnBnClickedButtonLevelVeryHigh)
	ON_BN_CLICKED(IDC_BUTTON_SELECT,  &CLauncherDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_SETTING_CANCLE,  &CLauncherDlg::OnBnClickedButtonCancel)
	ON_WM_TIMER()
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CLauncherDlg::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()

void CLauncherDlg::CreateDesktopShortCut(const wchar_t * pDir)
{
	wchar_t szBuf[255] = {0};
	wsprintf(szBuf, L"%s/Login.exe", pDir);

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink *pisl;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;

			//这里是我们要创建快捷方式的原始文件地址
			pisl->SetPath(szBuf);
			pisl->SetWorkingDirectory(pDir);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
			if (SUCCEEDED(hr))
			{

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//这里是我们要创建快捷方式的目标地址
				TCHAR szDesktopPath[MAX_PATH] = {0}, szPath[MAX_PATH];
				SHGetSpecialFolderPath(NULL, szDesktopPath, CSIDL_DESKTOP, FALSE);
				wsprintf(szPath, L"%s/将星诀.lnk", szDesktopPath);
				DeleteFile(szPath);

				pIPF->Save(szPath, FALSE);
				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}
}

// CLauncherDlg message handlers
BOOL CLauncherDlg::OnInitDialog()
{
	CDialogSK::OnInitDialog();

	//CString curDir = GetCurrentUninstallDir();
	//如果是卸载则立刻卸载
// 	if (2 == __argc)
// 	{
// 		if(GetJXJProgressExit())
// 		{
// 			if (IDYES == MessageBox(L"是否狠心卸载将星诀？", L"提示", MB_YESNO))
// 			{
// 				if(UninstallGame())
// 					AfxMessageBox(L"卸载完成");
// 				else
// 					AfxMessageBox(L"卸载失败,请手动卸载!");
// 				ExitProcess(0);
// 				return FALSE;
// 			}
// 		}
// 		else
// 		{
// 			MessageBox(L"将星诀程序正在运行，请先关闭程序后再试！",L"提示",MB_OK|MB_ICONERROR);
// 			ExitProcess(0);
// 			return FALSE;
// 		}
// 	}

	char *web = NULL;
	struct stat buf;
	if(!stat("login.ini", &buf ))
	{
		std::ifstream fin("login.ini",std::ios::in|std::ios::binary);
		if(fin)
		{
			fin.seekg(0, std::ios::end);
			int length = fin.tellg();
			web = new char[length+1];
			memset(web,0,length+1);
			fin.seekg(0, std::ios::beg);
			fin.read(web, length);
			fin.close();
		}
	}
	if(web)
	{
		m_loginurl = web;
		delete [] web;
		web = NULL;
	}
	if(m_loginurl.IsEmpty())
		m_loginurl = "http://www.quegame.com/micro/";

	CRect stRect;
	GetWindowRect(stRect);
	MoveWindow(stRect.top, stRect.left, 1035, 590);
	//MoveWindow(stRect.top, stRect.left, 1032, 536);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// 设置对话框背景信息
	EnableEasyMove();						// 允许移动
	SetBitmap(L"LauncherDlg.bmp");
	SetStyle (LO_RESIZE);					// 使对话框大小适合位图大小
	SetTransparentColor(RGB(64, 64, 64));	// 设置透明色
	//SetTransparentColor(RGB(0, 255, 0));	// 设置透明色
	//
	SetWindowText(L"将星诀安装程序");
	//设置退出按钮图片
	m_btnQuit.SetBitmaps(IDB_BITMAP_QUIT_1, RGB(0, 255, 0), IDB_BITMAP_QUIT_3, RGB(0, 255, 0));
	m_btnQuit.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnQuit.SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnQuit.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnQuit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnQuit.SizeToContent();
	m_btnQuit.DrawBorder(FALSE, FALSE);
	m_btnQuit.MoveWindow(981, 80, 25, 26);
	//m_btnQuit.MoveWindow(894, 110, 23, 23);

	//设置最小化按钮图片
	m_btnMiniDlg.SetBitmaps(IDB_BITMAP_MINI_1, RGB(0, 255, 0), IDB_BITMAP_MINI_3, RGB(0, 255, 0));
	m_btnMiniDlg.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnMiniDlg.SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnMiniDlg.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnMiniDlg.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnMiniDlg.SizeToContent();
	m_btnMiniDlg.DrawBorder(FALSE, FALSE);
	m_btnMiniDlg.MoveWindow(951, 80, 25, 26);
	//m_btnMiniDlg.MoveWindow(842, 116, 31, 9);

	//设置显示设置按钮
	m_btnVideoSetting.SetBitmaps(IDB_BITMAP_SETTING_3, RGB(0, 255, 0), IDB_BITMAP_SETTING_1, RGB(0, 255, 0));
	m_btnVideoSetting.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoSetting.SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoSetting.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoSetting.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoSetting.SizeToContent();
	m_btnVideoSetting.DrawBorder(FALSE, FALSE);
	m_btnVideoSetting.MoveWindow(829, 81, 103, 23);
	//m_btnVideoSetting.MoveWindow(22, 298, 81, 81);

	//设置显示设置模式
	//99,313    267,313    435,313    603,313   771.313
	m_btnVideoLevel[0].SetBitmaps(IDB_BITMAP_QUAD_1, RGB(0, 255, 0), IDB_BITMAP_QUAD_3, RGB(0, 255, 0));
	m_btnVideoLevel[0].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoLevel[0].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoLevel[0].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoLevel[0].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoLevel[0].SizeToContent();
	m_btnVideoLevel[0].DrawBorder(FALSE, FALSE);
	m_btnVideoLevel[0].MoveWindow(101, 313, 172, 172);
	//m_btnVideoLevel[0].MoveWindow(127, 221, 138, 138);
	m_btnVideoLevel[0].ShowWindow(SW_HIDE);


	m_btnVideoLevel[1].SetBitmaps(IDB_BITMAP_QUAD_1, RGB(0, 255, 0), IDB_BITMAP_QUAD_3, RGB(0, 255, 0));
	m_btnVideoLevel[1].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoLevel[1].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoLevel[1].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoLevel[1].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoLevel[1].SizeToContent();
	m_btnVideoLevel[1].DrawBorder(FALSE, FALSE);
	m_btnVideoLevel[1].MoveWindow(269, 313, 172, 172);
	//m_btnVideoLevel[1].MoveWindow(261, 221, 138, 138);
	m_btnVideoLevel[1].ShowWindow(SW_HIDE);

	m_btnVideoLevel[2].SetBitmaps(IDB_BITMAP_QUAD_1, RGB(0, 255, 0), IDB_BITMAP_QUAD_3, RGB(0, 255, 0));
	m_btnVideoLevel[2].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoLevel[2].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoLevel[2].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoLevel[2].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoLevel[2].SizeToContent();
	m_btnVideoLevel[2].DrawBorder(FALSE, FALSE);
	m_btnVideoLevel[2].MoveWindow(437, 313, 172, 172);
	//m_btnVideoLevel[2].MoveWindow(396, 221, 138, 138);
	m_btnVideoLevel[2].ShowWindow(SW_HIDE);

	m_btnVideoLevel[3].SetBitmaps(IDB_BITMAP_QUAD_1, RGB(0, 255, 0), IDB_BITMAP_QUAD_3, RGB(0, 255, 0));
	m_btnVideoLevel[3].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoLevel[3].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoLevel[3].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoLevel[3].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoLevel[3].SizeToContent();
	m_btnVideoLevel[3].DrawBorder(FALSE, FALSE);
	m_btnVideoLevel[3].MoveWindow(605, 313, 172, 172);
	//m_btnVideoLevel[3].MoveWindow(531, 221, 138, 138);
	m_btnVideoLevel[3].ShowWindow(SW_HIDE);

	m_btnVideoLevel[4].SetBitmaps(IDB_BITMAP_QUAD_1, RGB(0, 255, 0), IDB_BITMAP_QUAD_3, RGB(0, 255, 0));
	m_btnVideoLevel[4].SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnVideoLevel[4].SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnVideoLevel[4].SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnVideoLevel[4].SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnVideoLevel[4].SizeToContent();
	m_btnVideoLevel[4].DrawBorder(FALSE, FALSE);
	m_btnVideoLevel[4].MoveWindow(773, 313, 172, 172);
	//m_btnVideoLevel[4].MoveWindow(664, 221, 138, 138);
	m_btnVideoLevel[4].ShowWindow(SW_HIDE);

	//设置模式选择按钮
	m_btnSelect.SetBitmaps(IDB_BITMAP_SELECT_3, RGB(0, 255, 0), IDB_BITMAP_SELECT_1, RGB(0, 255, 0));
	m_btnSelect.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnSelect.SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnSelect.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnSelect.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnSelect.SizeToContent();
	m_btnSelect.DrawBorder(FALSE, FALSE);
	m_btnSelect.MoveWindow(370, 500, 106, 27);
	//m_btnSelect.MoveWindow(300, 381, 121, 42);
	m_btnSelect.ShowWindow(SW_HIDE);
	//设置模式取消按钮
	m_btnSelectCancel.SetBitmaps(IDB_BITMAP_CANCEL_3, RGB(0, 255, 0), IDB_BITMAP_CANCEL_1, RGB(0, 255, 0));
	m_btnSelectCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP, FALSE);
	m_btnSelectCancel.SetPressedStyle(CButtonST::BTNST_PRESSED_LEFTRIGHT, FALSE);
	m_btnSelectCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255, 255, 255));
	m_btnSelectCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
	m_btnSelectCancel.SizeToContent();
	m_btnSelectCancel.DrawBorder(FALSE, FALSE);
	m_btnSelectCancel.MoveWindow(568, 500, 106, 27);
	//m_btnSelectCancel.MoveWindow(514, 381, 121, 42);
	m_btnSelectCancel.ShowWindow(SW_HIDE);
	//浏览网页
	const int nXStart = 38, nYStart = 120;
	//const int nXStart = 93, nYStart = 165;
	ModifyStyleEx( WS_EX_CLIENTEDGE, 0, 0 );	//防止CHtmlView有立体的边框
	m_html.Create(NULL, NULL, (WS_CHILD | WS_VISIBLE), CRect(nXStart, nYStart, nXStart + 930, nYStart + 395), this, 1201);
	//m_html.Create(NULL, NULL, (WS_CHILD | WS_VISIBLE), CRect(nXStart, nYStart, nXStart + 753, nYStart + 294), this, 1201);
	//m_html.Navigate(L"http://www.quegame.com/login/");
	//m_html.Navigate(L"http://jxj.quegame.com/download/trunk/login/tpl/index.html");
	m_htmlWait.Create(NULL, NULL, (WS_CHILD | WS_VISIBLE), CRect(nXStart, nYStart, nXStart + 930, nYStart + 395), this, 1201);
	//m_htmlWait.Create(NULL, NULL, (WS_CHILD | WS_VISIBLE), CRect(nXStart, nYStart, nXStart + 753, nYStart + 294), this, 1201);
	m_htmlWait.ShowWindow(SW_HIDE);
	m_isWait = FALSE;
	
	m_Process1.MoveWindow(72, 552, 965 - 72, 565 - 552);
	//m_Process1.MoveWindow(113, 465, 814 - 113, 478 - 465);
	m_Process1.SetRange(0, 100);
	m_Process1.ShowWindow(SW_HIDE);

	//m_staticText.MoveWindow(72+100,552,965-72,565-552);
	//m_staticText.ShowWindow(SW_HIDE);
	
	SetTimer(1, 100, NULL);

	m_Download.Init();
	DoInitCheckGame();

	return TRUE;  // return TRUE  unless you set the focus to a control
}


bool CLauncherDlg::DoInitCheckUpdate()
{
	bool bUpdate = false;
	CString strLocalPath(m_strModule);
	strLocalPath += L"/LoginExeVersionInfo.ini";
	//获取本地版本信息
	int nlocalbgValue = GetPrivateProfileInt(L"VersionInfo", L"DlgBackVersionNumber", 0, strLocalPath);
	int nlocalexeValue = GetPrivateProfileInt(L"VersionInfo", L"LoginExeVersionNumber", 0, strLocalPath);
	//下载Ini文件
	bool isSucess = true;
	int tryCount = 0;
	do 
	{
		if(tryCount>=TRY_DOWNLOAD_MAXCOUNT)
		{
			MessageBox(L"文件LoginExeVersionInfo.ini下载失败，请稍后重试！");
			break;
		}
		isSucess = m_Download.DownloadFile(L"http://jxj.quegame.com/download/LoginExeVersionInfo.ini", strLocalPath, false);
		tryCount++;
		
		char *path ="d:/";
		debuglog(path,"%s:%d\n","LoginExeVersionInfo.ini",tryCount);
	} while (!isSucess);
	

	//首先考虑是否背景贴图有更新？
	int nValue = GetPrivateProfileInt(L"VersionInfo", L"DlgBackVersionNumber", 0, strLocalPath);
	
	wstring strTmp = m_strModule;
	strTmp += L"/LauncherDlg.bmp";
	WIN32_FIND_DATA stFindFileData;
	ZeroMemory(&stFindFileData, sizeof(WIN32_FIND_DATA));
	HANDLE hFile = FindFirstFile(strTmp.c_str(), &stFindFileData);
	if (INVALID_HANDLE_VALUE == hFile || nlocalbgValue < nValue)
	{
		//下载登录器
		strLocalPath = m_strModule;
		strLocalPath += L"/LauncherDlg.bmp";

		isSucess = true;
		tryCount = 0;
		do 
		{
			if(tryCount>=TRY_DOWNLOAD_MAXCOUNT)
			{
				MessageBox(L"文件LauncherDlg.bmp下载失败，请稍后重试！");
				break;
			}
			isSucess = m_Download.DownloadFile(L"http://jxj.quegame.com/download/LauncherDlg.bmp", strLocalPath, false);
			tryCount++;

			char *path ="c:/";
			debuglog(path,"%s:%d\n","LauncherDlg.bmp",tryCount);
		} while (!isSucess);
	}
	::FindClose ( hFile );
	//比较版本
	strLocalPath = m_strModule;
	strLocalPath += L"/LoginExeVersionInfo.ini";
	nValue = GetPrivateProfileInt(L"VersionInfo", L"LoginExeVersionNumber", 0, strLocalPath);
	if (nlocalexeValue < nValue)
	{
		//if (IDYES == MessageBox(L"登录器有更新，是否更新？", L"提示", MB_YESNO))
		{
			//下载登录器
			strLocalPath = m_strModule;
			strLocalPath += L"/TmpLogin.exe";


			isSucess = true;
			tryCount = 0;
			do 
			{
				if(tryCount>=TRY_DOWNLOAD_MAXCOUNT)
				{
					MessageBox(L"文件“Login.exe”下载失败，请稍后重试！");
					break;
				}
				isSucess = m_Download.DownloadFile(L"http://jxj.quegame.com/download/Login.exe", strLocalPath, false);
				tryCount++;

				char *path ="c:/";
				debuglog(path,"%s:%d\n","WebLauncher.exe",tryCount);
			} while (!isSucess);

			//使用程序更新登录器
			PROCESS_INFORMATION pi;
			STARTUPINFO si; //This is an [in] parameter
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof si; //Only compulsory field
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			wchar_t buf[256] = {0};
			swprintf(buf, L"%s/update.exe", m_strModule);
			
			if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
			{
				//MessageBox(buf);
				//WaitForSingleObject(pi.hProcess,INFINITE);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}

			ExitProcess(0);
		}
	}

	//比较游戏的版本，如果过期就卸载
	nValue = GetPrivateProfileInt(L"VersionInfo", L"VersionNumber", 0, strLocalPath);

	//读取客户端版本信息
	strLocalPath = m_strModule;
	strLocalPath += L"/jxjversion.ini";
	FILE * pFile = _wfopen(strLocalPath, L"rb");
	if (NULL == pFile)
		return bUpdate;
	fseek(pFile,0,SEEK_END);
	int nFileLen = ftell(pFile);
	fseek(pFile,0,SEEK_SET);
	char szReadBuf[256] = {0};
	if (nFileLen > 255)
		nFileLen = 255;
	fread(szReadBuf, nFileLen, 1, pFile);
	fclose(pFile);

	char * pTmp = strstr(szReadBuf, "version:");
	if (pTmp)
	{
		int nCurGameVersion = atoi(szReadBuf + strlen("version:"));
		if (nCurGameVersion < nValue)
		{
			m_htmlWait.ShowWindow(SW_NORMAL);
			m_htmlWait.Navigate(L"http://www.quegame.com/micro/waiting.php");
			m_html.ShowWindow(SW_HIDE);
			m_isWait = TRUE; 

			bUpdate = true;
			//卸载游戏
			if(IDOK == MessageBox(L"检测到你的游戏版本长时间未更新，为确保游戏能够正常运行，请先手动卸载程序"));
			{
				m_uidlg.DoModal();
			}
		}
	}

	return bUpdate;
}

#define FILE_SEPARATOR	L"/"
#define FILE_APP_NAME	L"Login.exe"
void CLauncherDlg::CreateStartMenu()
{
    TCHAR chStartupFolder[MAX_PATH];
    SHGetSpecialFolderPath(this->GetSafeHwnd(), chStartupFolder,CSIDL_PROGRAMS,FALSE);
    CString csStartupFolder = chStartupFolder;
    csStartupFolder.Append(FILE_SEPARATOR);
    csStartupFolder.Append(L"将星诀");
    CreateDirectory(csStartupFolder, NULL);

    CString csInstallPath = m_strModule;
    CString csEXEFilePath;
    csEXEFilePath = csInstallPath;
    csEXEFilePath.Append(FILE_SEPARATOR);
    csEXEFilePath.Append(FILE_APP_NAME);
    CString csUnExeFilePath;
    csUnExeFilePath = csInstallPath;
    csUnExeFilePath.Append(FILE_SEPARATOR);
    csUnExeFilePath.Append(FILE_APP_NAME);
	//csUnExeFilePath.Append(L" -Uninstall");
    CString csLinkFileName = csStartupFolder;
    csLinkFileName.Append(FILE_SEPARATOR);
    csLinkFileName.Append(L"将星诀.lnk");
    CString csUnlinkFileName = csStartupFolder;
    csUnlinkFileName.Append(FILE_SEPARATOR);
	csUnlinkFileName.Append(L"卸载游戏.lnk");

    //get icon path
    CString csExeIconPath;
    csExeIconPath = csInstallPath;
    csExeIconPath.Append(FILE_SEPARATOR);
    csExeIconPath.Append(FILE_APP_NAME);
    CString csUnExeIconPath;
    csUnExeIconPath = csInstallPath;
    csUnExeIconPath.Append(FILE_SEPARATOR);
    csUnExeIconPath.Append(FILE_APP_NAME);

    CreateShortCut(csLinkFileName, csEXEFilePath, csExeIconPath, L"");
    CreateShortCut(csUnlinkFileName, csUnExeFilePath, csUnExeIconPath, L"-Uninstall");
}

void CLauncherDlg::CreateShortCut(CString csLinkPath, CString csExePath, CString csIconPath, CString csArg)
{
	HRESULT hres;
	hres = ::CoInitialize(NULL);
	//delete old link
	CFileFind cfind;
	if(cfind.FindFile(csLinkPath))
		CFile::Remove(csLinkPath);

	IShellLink * pShellLink ;
	hres = ::CoCreateInstance( CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER, IID_IShellLink,(void **)&pShellLink);
	if( SUCCEEDED( hres))
	{
		pShellLink -> SetPath(csExePath);
		pShellLink -> SetIconLocation(csIconPath, 0); 
//		pShellLink -> SetHotkey( MAKEWORD( 'R', HOTKEYF_SHIFT | HOTKEYF_CONTROL));
		CString csWorkingDir;
		csWorkingDir = csExePath.Left(2);
		csWorkingDir.Append(FILE_SEPARATOR);

		pShellLink -> SetWorkingDirectory(csWorkingDir);
		pShellLink->SetArguments(csArg);

		IPersistFile *pPersistFile;
		hres = pShellLink -> QueryInterface( IID_IPersistFile, (void **)&pPersistFile) ;
		if( SUCCEEDED(hres))
		{
			hres = pPersistFile -> Save(csLinkPath, TRUE);
			pPersistFile -> Release();
		}
		pShellLink -> Release();
	}
	::CoUninitialize();
}

string CLauncherDlg::getCardInfo()
{
	CDC* pDC = GetDC();
	HDC hdc = pDC->m_hDC;
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),
		1,                     
		PFD_DRAW_TO_WINDOW |   
		PFD_SUPPORT_OPENGL | 
		PFD_DOUBLEBUFFER,      
		PFD_TYPE_RGBA,         
		32,                    
		0, 0, 0, 0, 0, 0,      
		0,                     
		0,                     
		0,                     
		0, 0, 0, 0,            
		24,
		0,                     
		0,                     
		PFD_MAIN_PLANE,        
		0,                     
		0, 0, 0                
	};
	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	string renderer = (char*) glGetString(GL_RENDERER);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(pDC);
	return renderer;
}
void CLauncherDlg::vedioSetting()
{
	wstring strTmp(m_strModule);
	strTmp += L"\\GameVideoSetting.ini";
	WIN32_FIND_DATA stFindFileData;
	HANDLE hFile = FindFirstFile(strTmp.c_str(), &stFindFileData);
	const char* index ;
	string cardInfo  = getCardInfo();
	if (INVALID_HANDLE_VALUE == hFile)
	{
		int nCardLevel = 0;
		//stSysInfo.GetDisplayCardInfo(dwNum, strDiskInfo);
		if (index = strstr(cardInfo.c_str(), "GTX "))	//nvidia
		{
			//确定显卡号，分成大于2000和小于2000的
			int nCardNumber = atoi(index + strlen("GTX "));
			if (nCardNumber > 2000)
				nCardLevel = 0;	//低
			else
			{
				int nSubNumber = nCardNumber % 100;
				if (nSubNumber > 50)
					nCardLevel = 3;	//高
				else if (nSubNumber <= 30)
					nCardLevel = 0;	//低
				else
					nCardLevel = 2;	//中
			}
		}
		else if (index = strstr(cardInfo.c_str(), "Titan"))	//nvidia
		{
			nCardLevel = 3;	//高
		}
		else if (index = strstr(cardInfo.c_str(), "Radeon HD"))	//Ati
		{
			int nCardNumber = atoi(index + strlen("Radeon HD "));
			if (nCardNumber < 5000)
				nCardLevel = 1;	//低
			else
			{
				int nSubNumber = nCardNumber % 1000;
				if (nSubNumber <= 50)
					nCardLevel = 1;	//低
				else if (nSubNumber <= 70)
					nCardLevel = 2;	//中
				else
					nCardLevel = 3;	//高
			}

		}
		else if (index = strstr(cardInfo.c_str(), "Radeon R9"))	//Ati
		{
			nCardLevel = 3;	//高
		}
		else if (index = strstr(cardInfo.c_str(), "Radeon R7"))	//Ati
		{
			int nCardNumber = atoi(index + strlen("Radeon R7 "));
			if (nCardNumber < 260)
				nCardLevel = 2;	//中
			else
			{
				nCardLevel = 3;	//高
			}
		}
		else
			nCardLevel = 0;	//低

		//写配置文件
		CString strInfo;
		strInfo.Format(L"%d", nCardLevel);
		WritePrivateProfileString(L"GameVideoSetting", L"SettingValue", strInfo, strTmp.c_str());
		WritePrivateProfileString(L"GameVideoSetting", L"SettingUpdate", L"1", strTmp.c_str());
	}
	::FindClose ( hFile );
}
void CLauncherDlg::DoInitCheckGame()
{
	//查找运行程序Launcher.exe是否存在，如果不存在，则强制下载
	BOOL bCheckReg4Game = CheckReg4Game();
	BOOL bCanRunGame = CanRunGame();

	//检查注册表中是否有那项
	BOOL bRunUpdate = FALSE;
	if (bCanRunGame)
	{
		m_html.m_strGameDir = m_strModule;

		bool bUpdate = DoInitCheckUpdate();
		if ( ! bUpdate)
		{
			//m_html.Navigate(L"http://www.quegame.com/micro/");
			m_html.Navigate(m_loginurl.GetString());
			//m_html.Navigate(L"http://www.quegame.com/micronew/");
			m_htmlWait.ShowWindow(SW_HIDE);
			m_html.ShowWindow(SW_NORMAL);
			m_isWait = FALSE;
		}
		else
			bRunUpdate = TRUE;
	}
	else
		bRunUpdate = TRUE;

	if (bRunUpdate)
	{
		m_htmlWait.ShowWindow(SW_NORMAL);
		m_htmlWait.Navigate(L"http://www.quegame.com/micro/waiting.php");
		m_html.ShowWindow(SW_HIDE);
		m_isWait = TRUE;

		CSystemInfo stSysInfo;
		CString chProcessorName, chProcessorType;DWORD dwNum = 50;
		//stSysInfo.GetCpuInfo(chProcessorName, chProcessorType, dwNum, dwMaxClockSpeed);
		enum { eBufferSize = 50, };
		CString strDiskInfo[eBufferSize];
		unsigned __int64 iDiskFreeSize[eBufferSize];
		wchar_t disk[eBufferSize];
		ZeroMemory(iDiskFreeSize, eBufferSize * sizeof(unsigned _int64));
		stSysInfo.GetDiskInfo(dwNum, strDiskInfo, iDiskFreeSize,disk);

		//C盘和D盘比较大小
		wchar_t szBuf[255] = {0};
		const wchar_t * pTmp = NULL;
		wchar_t cDisk = disk[0];//L'C';
		wcscpy_s(szBuf, 255, strDiskInfo[1]); 
		pTmp = wcsstr(szBuf, L"本地磁盘(");
		unsigned __int64 uTmpValue = 5 * 1024 * 1024;
		uTmpValue *= 1024;
		if (pTmp && iDiskFreeSize[0] < uTmpValue)	//如果C盘剩余小于5G则安装在D盘，以后不比较大小了
		{
			cDisk = disk[1];//L'D';
		}
		swprintf_s(m_strModule, L"%c:\\Jiangxingjue", cDisk);
		swprintf_s(m_Download.m_strModule, L"%c:\\Jiangxingjue", cDisk);

		m_html.m_strGameDir = m_strModule;

		CreateDirectory(m_strModule, NULL);
		//如果本地没有配置文件，则确定显卡，之后生成配置文件
		vedioSetting();
		
		

		//将解压到临时目录的东西移动到游戏目录
		//判断当前文件运行目录是否是临时文件件
		//如果是临时文件夹则复制文件，否则不复制

		//strTmp = m_strModule;
		//strTmp += L"\\Login.exe";
		//hFile = FindFirstFile(strTmp.c_str(), &stFindFileData);

		GetModuleFileName(NULL, szBuf, 255); //得到当前模块路径
		wchar_t * pcurLast = NULL, * pcurTmp = wcsstr(szBuf, L"\\");
		while (pcurTmp)
		{
			pcurLast = pcurTmp;
			pcurTmp = wcsstr(pcurLast + 1, L"\\");
		}
		if (pcurLast)
			pcurLast[0] = 0;
		CString strTmpPath;
		strTmpPath.Format(L"%s",szBuf);

		if (strTmpPath.Compare(m_strModule)!=0)
		{
			strTmpPath.Format(L"%s\\Login.exe", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\login.ini", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			//strTmpPath.Format(L"%s\\mfc90u.dll", szBuf);
			//CopyBatchFile2DirPath(strTmpPath, m_strModule);
			//strTmpPath.Format(L"%s\\mfcm90u.dll", szBuf);
			//CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\LauncherDlg.bmp", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\Uninstall.list", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\LoginExeVersionInfo.ini", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\JXJClient.7z", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			strTmpPath.Format(L"%s\\7zr.exe", szBuf);
			CopyBatchFile2DirPath(strTmpPath, m_strModule);
			
		}

		//strTmp = m_strModule;
		//strTmp += L"/JXJClient.7z";
		//hFile = FindFirstFile(strTmp.c_str(), &stFindFileData);
		//if (INVALID_HANDLE_VALUE == hFile)
		//{
			//如果注册表中的键值无效则下载文件
			/*CString strHttpFile("http://jxj.quegame.com/download/JXJClient.7z");
			wstring strTmp = m_strModule;
			strTmp += L"/JXJClient.7z";
			m_Download.StartDownload(strHttpFile, strTmp.c_str());

			m_Process1.ShowWindow(SW_SHOW);
			m_Process1.setText(CString(L"文件下载中...."));*/
		//}
		//else
		//{
		//	::FindClose ( hFile );
		//}

		//解压
		//Decompress();
		m_Download.m_bStartCompress = TRUE;

		CreateStartMenu();
		CreateDesktopShortCut(m_strModule);
	}else
	{
		vedioSetting();
	}
}

void CLauncherDlg::CopyBatchFile2DirPath(CString strFilePath, CString strDirPath)
{
	//创建目录
	CreateDirectory(strDirPath, NULL);

	//移动文件
	CString strFileName, strMove2FilePath;
	if (GetFileName(strFilePath, strFileName))
	{
		strMove2FilePath.Format(L"%s/%s", strDirPath, strFileName);
		CopyFile(strFilePath, strMove2FilePath, FALSE);
	}
}

BOOL CLauncherDlg::GetFileName(CString strPath, CString & strFileName)
{
	const wchar_t * pLast = NULL, * pTmp = wcsstr(strPath, L"\\");
	while (pTmp)
	{
		pLast = pTmp;
		pTmp = wcsstr(pLast + 1, L"\\");
		if (NULL == pTmp)
			pTmp = wcsstr(pLast + 1, L"/");
	}
	if (pLast)
		strFileName = pLast + 1;
	return TRUE;
}

BOOL CLauncherDlg::CanRunGame()
{
	wstring strTmp(m_strModule);
	strTmp += L"/Launcher.exe";
	//AfxMessageBox(strTmp.c_str());
	WIN32_FIND_DATA stFindFileData;
	HANDLE hFile = FindFirstFile(strTmp.c_str(), &stFindFileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		//AfxMessageBox(L"false1");
		::FindClose ( hFile );
		return FALSE;
	}
	::FindClose ( hFile );
	return TRUE;
}

void CLauncherDlg::RunGame()
{
	// 启动进程信息
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );	// 将启动信息结构清零
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );	// 将进程信息结构清零

	wstring strTmp(m_strModule);
	strTmp += L"/Launcher.exe";

	//调试版本的路径
	CreateProcess(strTmp.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi);
}

void CLauncherDlg::RunSetup()
{
	//// 启动进程信息
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//ZeroMemory( &si, sizeof(si) );	// 将启动信息结构清零
	//si.cb = sizeof(si);
	//ZeroMemory( &pi, sizeof(pi) );	// 将进程信息结构清零

	//wstring strTmp(m_strModule);
	//strTmp += L"/JXJGame.exe";

	////调试版本的路径
	//CreateProcess(strTmp.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogSK::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags the minimized window.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLauncherDlg::OnBnClickedStartGame()
{
	//// 启动进程信息
	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;
	//ZeroMemory( &si, sizeof(si) );	// 将启动信息结构清零
	//si.cb = sizeof(si);
	//ZeroMemory( &pi, sizeof(pi) );	// 将进程信息结构清零

	////调试版本的路径
	//CreateProcess(L"../Game.exe", NULL, NULL, NULL, FALSE, 0, NULL, L"../", &si, &pi);

	//退出程序
	//OnOK();
}

void CLauncherDlg::OnBnClickedButtonMini()
{
	ShowWindow(SW_MINIMIZE);
}

void CLauncherDlg::OnBnClickedButtonQuit()
{
	CDialogSK::OnCancel();
}

void CLauncherDlg::OnBnClickedButtonVideoSetting()
{
	m_html.ShowWindow(SW_HIDE);
	m_htmlWait.ShowWindow(SW_HIDE);

	m_btnSelect.ShowWindow(SW_SHOW);
	m_btnSelectCancel.ShowWindow(SW_SHOW);

	wstring strTmp(m_strModule);
	strTmp += L"/GameVideoSetting.ini";
	int selectId = GetPrivateProfileInt(L"GameVideoSetting", L"SettingValue",2,strTmp.c_str());
	if(selectId>4) selectId = 4;
	
	for (int i = 0;i<5;i++)
	{
		m_btnVideoLevel[i].ShowWindow(SW_SHOW);
		if(i == selectId)
			m_btnVideoLevel[i].SetCheck(1);
		else
			m_btnVideoLevel[i].SetCheck(0);
	}
}
void CLauncherDlg::OnBnClickedButtonLevelVeryLow()
{
	for (int i = 1;i<5;i++)
	{
		m_btnVideoLevel[i].SetCheck(0);
	}
	m_btnVideoLevel[0].SetCheck(1);
}
void CLauncherDlg::OnBnClickedButtonLevelLow()
{
	for (int i = 0;i<5;i++)
	{
		if(i == 1) continue; 
		m_btnVideoLevel[i].SetCheck(0);
	}
	m_btnVideoLevel[1].SetCheck(1);
}
void CLauncherDlg::OnBnClickedButtonLevelNormal()
{
	for (int i = 0;i<5;i++)
	{
		if(i == 2) continue; 
		m_btnVideoLevel[i].SetCheck(0);
	}
	m_btnVideoLevel[2].SetCheck(1);
}
void CLauncherDlg::OnBnClickedButtonLevelHigh()
{
	for (int i = 0;i<5;i++)
	{
		if(i == 3) continue; 
		m_btnVideoLevel[i].SetCheck(0);
	}
	m_btnVideoLevel[3].SetCheck(1);
}
void CLauncherDlg::OnBnClickedButtonLevelVeryHigh()
{
	for (int i = 0;i<5;i++)
	{
		if(i == 4) continue; 
		m_btnVideoLevel[i].SetCheck(0);
	}
	m_btnVideoLevel[4].SetCheck(1);
}
void CLauncherDlg::OnBnClickedButtonSelect()
{
	wstring strTmp(m_strModule);
	strTmp += L"/GameVideoSetting.ini";
	int selectId = GetPrivateProfileInt(L"GameVideoSetting", L"SettingValue",2,strTmp.c_str());

	for (int i = 0;i<5;i++)
	{
		if(m_btnVideoLevel[i].GetCheck()==1)
		{
			selectId = i;
			break;
		}
	}

	CString strInfo;
	strInfo.Format(L"%d", selectId);
	WritePrivateProfileString(L"GameVideoSetting", L"SettingValue", strInfo, strTmp.c_str());
	WritePrivateProfileString(L"GameVideoSetting", L"SettingUpdate", L"1", strTmp.c_str());
	OnBnClickedButtonCancel();
}
void CLauncherDlg::OnBnClickedButtonCancel()
{
	if(m_isWait == TRUE)
	{
		m_htmlWait.ShowWindow(SW_SHOW);
	}
	else
	{
		m_html.ShowWindow(SW_SHOW);
	}
	
	

	for (int i = 0;i<5;i++)
	{
		m_btnVideoLevel[i].ShowWindow(SW_HIDE);
	}

	m_btnSelect.ShowWindow(SW_HIDE);
	m_btnSelectCancel.ShowWindow(SW_HIDE);
}
void CLauncherDlg::OnCancel()
{

}

wstring GetProgramDir()    
{     
	TCHAR exeFullPath[MAX_PATH]; 
	GetCurrentDirectory(MAX_PATH,exeFullPath);
	return exeFullPath;
	//TCHAR exeFullPath[MAX_PATH];  
	//GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	//wstring strPath = __TEXT("");         
	//strPath = (wstring)exeFullPath;    
	//int pos = strPath.find_last_of(L'\\', strPath.length());  
	//return strPath.substr(0, pos);  
}   

BOOL CLauncherDlg::CheckReg4Game()
{
	HKEY hKey;
	bool bFind = FALSE;
	//LONG lRet = RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\MozillaPlugins\\@jxj.com", 0, KEY_QUERY_VALUE, &hKey );
	LONG lRet = RegOpenKeyEx( HKEY_CURRENT_USER, L"SOFTWARE\\MozillaPlugins\\quegame.com/JXJWebPlayer", 0, KEY_QUERY_VALUE, &hKey );
	if( lRet != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
	}
	else
	{
		//char szBuf[128] = {0};
		wchar_t szBuf[255] = {0};
		DWORD dwBufLen = 127;
		lRet = RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) szBuf, &dwBufLen);
		if( lRet == ERROR_SUCCESS )
		{
			wcscpy(m_strModule, szBuf);
			wchar_t * pIndex = wcsstr(m_strModule, L"Bin");
			if (pIndex)
				pIndex[0] = 0;
			else
			{
				pIndex = wcsstr(m_strModule, L"bin");
				if (pIndex)
					pIndex[0] = 0;
			}
			bFind = TRUE;
		}
		
		RegCloseKey( hKey );
	}

	if (bFind == FALSE)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		wstring path = GetProgramDir();
		wstring findPath = path + L"/Launcher.exe";
		//AfxMessageBox(findPath.c_str());
		const TCHAR *FilePathBuff = findPath.c_str();
		hFind = FindFirstFile(FilePathBuff, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			bFind = TRUE;
			wcscpy(m_strModule, path.c_str());
		}
		::FindClose(hFind);
	}
	return bFind;
}

void CLauncherDlg::CurDownloadProc(CString &fileName,UINT uCurDownload, UINT uTotalSize)
{
	m_strCurDownFileName = fileName;
	m_uCurDownloadSize = uCurDownload;
	m_uCurDownloadFileSize = uTotalSize;
}

void CLauncherDlg::OnDownloadFinish(const CString & strHttpFile, const CString & strLocalFile)
{
	//RunSetup();

	//m_bDownloadFinish = TRUE;

	//if (0 == m_btDownloadStage)
	//{
	//	m_btDownloadStage = 1;
	//}
	//else if (2 == m_btDownloadStage)
	//{
	//	//m_bDownloadFinish = TRUE;
	//	m_Download.m_bStartCompress = TRUE;
	//}
}
void CLauncherDlg::OnDecompressFile()
{
	CString str(L"正在安装....");
	m_Process1.SetPos(0);
	m_Process1.setText(str);
}
void CLauncherDlg::OnTimer(UINT_PTR nIDEvent)
{
	/*if (1 == m_btDownloadStage)
	{
		m_btDownloadStage = 2;

		CString strHttpFile("http://jxj.quegame.com/download/7zr.exe");
		wstring strTmp(m_strModule);
		strTmp += L"/7zr.exe";
		m_Download.StartDownload(strHttpFile, strTmp.c_str());
	}*/

	//if (m_bDownloadFinish)
	//{
	//	m_bDownloadFinish = FALSE;

	//	//解压
	//	wchar_t buf[256];
	//	memset(buf,0,256);
	//	swprintf(buf, L"%s/7zr.exe x -r %s/JXJClient.7z -y", m_strModule, m_strModule);
	//	PROCESS_INFORMATION pi;
	//	STARTUPINFO si; //This is an [in] parameter
	//	ZeroMemory(&si, sizeof(si));
	//	si.cb = sizeof si; //Only compulsory field
	//	si.dwFlags = STARTF_USESHOWWINDOW;
	//	si.wShowWindow = SW_HIDE;
	//	if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//	{
	//		WaitForSingleObject(pi.hProcess,INFINITE);
	//		CloseHandle(pi.hThread);
	//		CloseHandle(pi.hProcess);

	//		//运行install.cmd
	//		//wstring strTmp(m_strModule);
	//		//strTmp += L"/Install.cmd";
	//		//_wsystem(strTmp.c_str());

	//		//每个命令执行
	//		swprintf(buf, L"regsvr32 /s /u %s/Bin/Creature3DX.ocx", m_strModule);
	//		if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//		{
	//			WaitForSingleObject(pi.hProcess,INFINITE);
	//			CloseHandle(pi.hThread);
	//			CloseHandle(pi.hProcess);
	//		}
	//		else
	//			AfxMessageBox(L"regsvr32");
	//		swprintf(buf, L"%s/RegisterKeyNP.exe -del", m_strModule);
	//		if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//		{
	//			WaitForSingleObject(pi.hProcess,INFINITE);
	//			CloseHandle(pi.hThread);
	//			CloseHandle(pi.hProcess);
	//		}
	//		else
	//			AfxMessageBox(L"RegisterKeyNP");
	//		swprintf(buf, L"regsvr32 /s %s/Bin/Creature3DX.ocx", m_strModule);
	//		if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//		{
	//			WaitForSingleObject(pi.hProcess,INFINITE);
	//			CloseHandle(pi.hThread);
	//			CloseHandle(pi.hProcess);
	//		}
	//		else
	//			AfxMessageBox(L"regsvr32");
	//		swprintf(buf, L"%s/RegisterKeyNP.exe -reg", m_strModule);
	//		if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//		{
	//			WaitForSingleObject(pi.hProcess,INFINITE);
	//			CloseHandle(pi.hThread);
	//			CloseHandle(pi.hProcess);
	//		}
	//		else
	//			AfxMessageBox(L"RegisterKeyNP");
	//		swprintf(buf, L"%s/Launcher.exe", m_strModule);
	//		if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//		{
	//			WaitForSingleObject(pi.hProcess,INFINITE);
	//			CloseHandle(pi.hThread);
	//			CloseHandle(pi.hProcess);
	//		}
	//		else
	//			AfxMessageBox(L"Launcher");
	//	}
	//	else
	//		AfxMessageBox(L"解压失败");

	//	m_Download.Uninit();
	//	::Sleep(16);

	//	CDialogSK::OnCancel();

	//	//m_bQuitProcess = TRUE;


	//	////检查注册表中是否有那项
	//	//if (CheckReg4Game())
	//	//{
	//	//	//// 启动进程信息
	//	//	//RunGame();

	//	//	m_bQuitProcess = TRUE;
	//	//}
	//}
	if (m_Download.m_bQuitProcess || m_bQuitProcess)
	{
		//m_Download.Uninit();
		//::Sleep(16);

		//CDialogSK::OnCancel();

		m_Process1.SetPos(100);
		UpdateData();

		m_html.ShowWindow(SW_NORMAL);
		//m_html.Navigate(L"http://www.quegame.com/micro/");
		m_html.Navigate(m_loginurl.GetString());
		//m_html.Navigate(L"http://www.quegame.com/micronew/");
		m_htmlWait.ShowWindow(SW_HIDE);
		m_Process1.ShowWindow(SW_HIDE);
		m_isWait = FALSE;

		KillTimer(1);
	}
	else
	{
		if (m_uCurDownloadFileSize)
		{
			m_Process1.SetPos(m_uCurDownloadSize * 100.0f / m_uCurDownloadFileSize);
			UpdateData();
		}
	}

	__super::OnTimer(nIDEvent);
}
