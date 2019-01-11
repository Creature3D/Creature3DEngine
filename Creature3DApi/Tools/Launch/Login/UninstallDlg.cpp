// UninstallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Launcher.h"
#include "UninstallDlg.h"
#include<TlHelp32.h>
#include <fstream>

using namespace std;
#define EXE_NAME	L"Jiangxingjue.exe"

// CUninstallDlg 对话框

IMPLEMENT_DYNAMIC(CUninstallDlg, CDialog)

CUninstallDlg::CUninstallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUninstallDlg::IDD, pParent)
{

}

CUninstallDlg::~CUninstallDlg()
{
}

void CUninstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
}


BEGIN_MESSAGE_MAP(CUninstallDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CUninstallDlg::OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CUninstallDlg 消息处理程序

BOOL CUninstallDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);
	if(AfxMessageBox(L"是否要卸载将星诀?",MB_OKCANCEL)==IDOK)
	{
		m_edit.SetLimitText(UINT_MAX);
		m_edit.SetWindowText(L"正在卸载以下文件：\n");
		m_bDelete = true;
		SetTimer(9, 1000, NULL);
	}
	else
	{
		CDialog::OnCancel();
	}
	//GetUnInstallList();
	return TRUE;
}
void CUninstallDlg::addTextToEditBox(const wchar_t * str)
{
	wstring strTemp(str);
	strTemp+=wstring(L"\n");
	int lenrht = m_edit.GetWindowTextLength();
	m_edit.SetSel(lenrht,lenrht);
	m_edit.ReplaceSel(strTemp.c_str());
	UpdateData();
}
void CUninstallDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bDelete = false;
	CloseHandle(m_thread);
	if(m_hWnd)
		OnCancel();
}

CString CUninstallDlg::GetCurrentUninstallDir()
{
	wchar_t strFullPath[MAX_PATH];
	GetModuleFileName(NULL,strFullPath,MAX_PATH);
	CString str(strFullPath);
	int index = str.ReverseFind('\\');
	str = str.Left(index);

	//MessageBox(strFullPath);
	//MessageBox(str);
	return str;
}

bool GetJXJProgressExit()
{
	HANDLE hProcesses;
	PROCESSENTRY32 lpe;
	lpe.dwSize = sizeof(PROCESSENTRY32);

	CString strFileName(EXE_NAME);
	strFileName.MakeLower();
	hProcesses = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcesses == INVALID_HANDLE_VALUE)
		return false;

	BOOL isExist = ::Process32First(hProcesses, &lpe);
	CString strName;
	while (isExist)
	{
		strName = lpe.szExeFile;
		strName.MakeLower();
		if (strFileName.Compare(strName)==0)
		{
			return false;
			break;
		}
		// 遍历下一个进程实例
		isExist = ::Process32Next(hProcesses, &lpe);
	}

	return true;
}

bool CUninstallDlg::DeleteDirectory(const wchar_t * sDirName)
{

	//检测文件夹是否合法
	CString currentDir(sDirName);
	int index  = currentDir.Find(L"Jiangxingjue");
	if(index==-1)
	{
		index  = currentDir.Find(L"将星诀");
	}
	if(index==-1)
	{
		index  = currentDir.Find(L"将星决");
	}
	if(index==-1)
	{
		return false;
	}

	wchar_t sTempFileFind[256];
	wcscpy(sTempFileFind, sDirName);
	wcscat(sTempFileFind, L"\\*.*");
	//MessageBox(NULL,sDirName,L"",MB_OK);

	wstring strFilePath;
	WIN32_FIND_DATA stFindFileData;
	HANDLE hSearch = ::FindFirstFile ( sTempFileFind, &stFindFileData );
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"无法找到目录，请确认输入路径正确", 0, 0);
		::FindClose ( hSearch );
		return false;
	}
	do
	{
		if ( wcscmp ( stFindFileData.cFileName, L"." ) == 0 || wcscmp ( stFindFileData.cFileName, L".." ) == 0 )
			continue;

		//不卸载的文件
		if (wcsstr(stFindFileData.cFileName, L"LauncherDlg.bmp") || wcsstr(stFindFileData.cFileName, L"Launcherdlg.bmp") || wcsstr(stFindFileData.cFileName, L"launcherdlg.bmp"))
			continue;
		//if (wcsstr(stFindFileData.cFileName, L"mfc90u.dll"))
		//	continue;
		//if (wcsstr(stFindFileData.cFileName, L"mfcm90u.dll"))
		//	continue;

		if ( stFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			wstring strDirPath(sDirName);
			strDirPath += L"\\";
			strDirPath += stFindFileData.cFileName;
			DeleteDirectory(strDirPath.c_str());
			continue;
		}

		strFilePath = sDirName;
		strFilePath += L"\\";
		strFilePath += stFindFileData.cFileName;

		//MessageBox(NULL,strFilePath,L"",MB_OK);
		
		if(m_bDelete)
		{
			addTextToEditBox(stFindFileData.cFileName);
			DeleteFile(strFilePath.c_str());
		}

	} while ( ::FindNextFile ( hSearch, &stFindFileData ) );
	::FindClose ( hSearch );

	RemoveDirectory(sDirName);	//删除文件夹

	return true;
}

BOOL CUninstallDlg::CheckReg4Game()
{
	HKEY hKey;
	//LONG lRet = RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\MozillaPlugins\\@jxj.com", 0, KEY_QUERY_VALUE, &hKey );
	LONG lRet = RegOpenKeyEx( HKEY_CURRENT_USER, L"SOFTWARE\\MozillaPlugins\\quegame.com/JXJWebPlayer", 0, KEY_QUERY_VALUE, &hKey );
	if( lRet != ERROR_SUCCESS )
	{
		RegCloseKey( hKey );
		return FALSE;
	}
	else
	{
		//char szBuf[128] = {0};
		wchar_t szBuf[255] = {0};
		DWORD dwBufLen = 127;
		lRet = RegQueryValueEx( hKey, L"Path", NULL, NULL, (LPBYTE) szBuf, &dwBufLen);
		if( lRet != ERROR_SUCCESS )
		{
			RegCloseKey( hKey );
			return FALSE;
		}
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
	}
	RegCloseKey( hKey );

	return TRUE;
}

bool CUninstallDlg::UninstallGame()
{
	CheckReg4Game();

	PROCESS_INFORMATION pi;
	STARTUPINFO si; //This is an [in] parameter
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof si; //Only compulsory field
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	wchar_t buf[256] = {0};
	swprintf(buf, L"regsvr32 /s /u %s/bin/npJXJWebPlayer.dll", m_strModule);
	if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess,INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	//swprintf(buf, L"%s/RegisterKeyNP.exe -del", m_strModule);
	//if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, m_strModule, &si, &pi))
	//{
	//	WaitForSingleObject(pi.hProcess,INFINITE);
	//	CloseHandle(pi.hThread);
	//	CloseHandle(pi.hProcess);
	//}

	//获得当前目录-》检测目录路径中-》获得目录下卸载文件列表-》回调删除文件
	//检测JXJ进程是否运行
	
	//获得当前目录
	CString currentDir = GetCurrentUninstallDir();
	//获取文件列表
	GetUnInstallList(wstring(currentDir.GetBuffer()));
	
	//检测文件夹是否合法
	int index  = currentDir.Find(L"Jiangxingjue");
	if(index==-1)
	{
		index  = currentDir.Find(L"将星诀");
	}
	if(index==-1)
	{
		index  = currentDir.Find(L"将星决");
	}
	if(index==-1 || m_dirVec.empty())
	{
		MessageBox(L"卸载路径错误，请手动删除！",L"提示",MB_OK|MB_ICONERROR);
		return false;
	}
	
	CString strFileOrDirName;
	WIN32_FIND_DATA stFindFileData;
	DirVec::iterator itr_dir = m_dirVec.begin();
	HANDLE hSearch = INVALID_HANDLE_VALUE;
	while(itr_dir!=m_dirVec.end())
	{
		strFileOrDirName = itr_dir->c_str();
		strFileOrDirName = currentDir+strFileOrDirName;

		hSearch = ::FindFirstFile ( strFileOrDirName, &stFindFileData );
		if (hSearch != INVALID_HANDLE_VALUE)
		{
			if ( stFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				DeleteDirectory(strFileOrDirName.GetBuffer());
			}
			else
			{
				addTextToEditBox(strFileOrDirName.GetBuffer());
				DeleteFile(strFileOrDirName);
			}
		}
		FindClose(hSearch);
		itr_dir++;
	}
	//查找将星诀线程是否运行
	//addTextToEditBox(L"请稍后....");
	//DeleteDirectory(currentDir.GetBuffer());
	return true;
}

DWORD _stdcall onStartUninstall(LPVOID param)
{
	CUninstallDlg * dlg = (CUninstallDlg *)param;
	
	dlg->UninstallGame();

	TCHAR szDesktopPath[MAX_PATH] = {0}, szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szDesktopPath, CSIDL_DESKTOP, FALSE);
	wsprintf(szPath, L"%s/将星诀Web.lnk\0", szDesktopPath);
	DeleteFile(szPath);
	wsprintf(szPath, L"%s/将星诀.lnk\0", szDesktopPath);
	DeleteFile(szPath);

	TCHAR chStartupFolder[MAX_PATH];
	SHGetSpecialFolderPath(dlg->GetSafeHwnd(), chStartupFolder,CSIDL_PROGRAMS,FALSE);
	wsprintf(szPath, L"%s/将星诀\0", chStartupFolder);
	dlg->DeleteDirectory(szPath);

	dlg->addTextToEditBox(L"卸载成功！");
	dlg->setBtText(L"确定");
	//if(MessageBox(NULL,L"成功卸载将星诀",L"提示",MB_OK)==IDOK)
		//dlg->ShowWindow(SW_HIDE);
	return 0;
}
void CUninstallDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent==9)
	{
		KillTimer(nIDEvent);
		if(GetJXJProgressExit())
		{
			m_thread = CreateThread(NULL,0,onStartUninstall,this,0,NULL);
		}
		else
		{
			MessageBox(L"将星诀程序正在运行，请先关闭程序后再试！",L"提示",MB_OK|MB_ICONERROR);
			OnCancel();
		}
		//onStartUninstall();
	}
	CDialog::OnTimer(nIDEvent);
}
void CUninstallDlg::dlgClose()
{
	OnBnClickedCancel();
}

void CUninstallDlg::GetUnInstallList(wstring & dir)
{
	string strDir;
	wstring listFile;
	listFile = dir+wstring(L"\\Uninstall.list");
	fstream filePtr;
	filePtr.open(listFile.c_str(),ios::in);
	if(filePtr.is_open())
	{
		while(getline(filePtr,strDir))
		{
			m_dirVec.push_back(strDir);
		}
	}

	filePtr.close();
}

void CUninstallDlg::setBtText(const wchar_t * str)
{
	GetDlgItem(IDCANCEL)->SetWindowText(str);
}
BOOL CUninstallDlg::DeleteLink(LPCWSTR lpszShortcut) 
{ 
	SHFILEOPSTRUCT   fos   ; 

	ZeroMemory(   &fos,   sizeof(fos))   ; 
	fos.hwnd   =   HWND_DESKTOP   ; 
	fos.wFunc   =   FO_DELETE   ; 
	fos.pFrom   =   lpszShortcut; 
	fos.pTo   =   NULL   ; 
	fos.fFlags   =   FOF_SILENT   |   FOF_ALLOWUNDO   ; 
	//删除快捷方式（*.lnk) 
	if(   0   !=   SHFileOperation(   &fos)) 
		return   FALSE   ; 
	return   TRUE   ; 
} 