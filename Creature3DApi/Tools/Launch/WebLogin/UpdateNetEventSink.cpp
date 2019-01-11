#include "StdAfx.h"
#include "UpdateNetEventSink.h"
#include <afxinet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // for _mkdir
// set up for windows so acts just like unix access().
#define F_OK 4
void CreateDesktopShortCut(const wchar_t * pDir)
{
	wchar_t szBuf[255] = {0};
	wsprintf(szBuf, L"%s/Launcher.exe\0", pDir);

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
				wsprintf(szPath, L"%s/将星诀Web.lnk", szDesktopPath);
				DeleteFile(szPath);

				pIPF->Save(szPath, FALSE);
				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}

	//wchar_t szBuf[255] = {0};
	wsprintf(szBuf, L"%s/Login.exe\0", pDir);
	hr = CoInitialize(NULL);
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

UINT DownloadThreadProc(LPVOID pParam)
{
	CUpdateNetEventSink * pNet = (CUpdateNetEventSink *)pParam;
	while (pNet)
	{
		if (pNet->m_bIsDownloading)
			pNet->ProcDownload();

		::Sleep(64);

		if (pNet->m_bStartCompress)
		{
			pNet->m_bStartCompress = FALSE;

			//解压
			wchar_t buf[256];
			memset(buf,0,256);
			swprintf(buf, L"%s/7zr.exe x -r %s/JXJClient.7z -y\0", pNet->m_strModule, pNet->m_strModule);
			PROCESS_INFORMATION pi;
			STARTUPINFO si; //This is an [in] parameter
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof si; //Only compulsory field
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			pNet->m_pCallBack->OnDecompressFile();
			if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
			{
				WaitForSingleObject(pi.hProcess,INFINITE);
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);

				//SetCurrentDirectory(L"C:\\国战三国");
				//CreateDesktopShortCut(L"C:\\国战三国");
				//WinExec("C:\\国战三国\\Launcher.exe",SW_SHOWNORMAL);
				//exit(0);
				////运行install.cmd
				////wstring strTmp(m_strModule);
				////strTmp += L"/Install.cmd";
				////_wsystem(strTmp.c_str());
				//每个命令执行
				swprintf(buf, L"regsvr32 /s %s/Bin/npJXJWebPlayer.dll\0", pNet->m_strModule);
				if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				{
					WaitForSingleObject(pi.hProcess,INFINITE);
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
				}
				else
					AfxMessageBox(L"regsvr32");
				CreateDesktopShortCut(pNet->m_strModule);
				////启动登陆器
				Sleep(5000);
				wsprintf(buf, L"%s/creature3d.txt\0", pNet->m_strModule);
				if(::GetFileAttributes(buf)==-1)
				{
					wsprintf(buf, L"%s/Login.exe\0", pNet->m_strModule);
					ShellExecute(NULL,L"open",buf,NULL,NULL,SW_SHOW);
				}
				//触发退出
				pNet->m_pCallBack->OnGameLog();
				/*swprintf(buf, L"%s/RegisterNP.exe -reg", pNet->m_strModule);
				if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				{
					WaitForSingleObject(pi.hProcess,INFINITE);
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
				}
				else
					AfxMessageBox(L"RegisterNP");*/
			}
			else
				AfxMessageBox(L"解压失败");

			
		}
	}

	return 0;
}

CUpdateNetEventSink::CUpdateNetEventSink(CUpdateEventCallback * pCallback)
	: m_bIsDownloading(false)
	, m_bInit(false)
	, m_pThread(NULL)
	, m_pCallBack(pCallback)
	, m_bStartCompress(false)
	, m_bQuitProcess(false)
{
	m_strModule[0] = 0;
}

CUpdateNetEventSink::~CUpdateNetEventSink(void)
{
}

bool CUpdateNetEventSink::Init()
{
	if (m_bInit)
	{
		return true;
	}

	m_bIsDownloading = false;

	//创建线程
	m_pThread = AfxBeginThread(DownloadThreadProc, this);
	if (NULL == m_pThread)
	{
		return false;
	}

	return true;
}

void CUpdateNetEventSink::Uninit()
{
	if (m_bInit)
	{
		//查看当前是否在下载中？
		if (m_bIsDownloading)
		{
			m_bIsDownloading = false;
		}

		//回收线程
		//m_pThread->Delete();
		m_pThread->ExitInstance();
		m_pThread = NULL;

		m_bInit = false;
	}
}

void CUpdateNetEventSink::StartDownload(CString strFileURLInServer, CString strFileLocalFullPath)
{
	m_strFileURLInServer = strFileURLInServer;
	m_strFileLocalFullPath = strFileLocalFullPath;

	m_bIsDownloading = true;
}

void CUpdateNetEventSink::ProcDownload()
{
	if (m_bIsDownloading && m_strFileURLInServer != "" && m_strFileLocalFullPath != "")
	{
		int tryCount = 0;
		bool isSucess = true;
		do 
		{
			if(tryCount>=TRY_DOWNLOAD_MAXCOUNT)
			{
				AfxMessageBox(m_strFileLocalFullPath);
				break;
			}
			DownloadFile(m_strFileURLInServer, m_strFileLocalFullPath);
			tryCount++;

			int index = m_strFileLocalFullPath.Find(L"/");
			CString strLocalPath = m_strFileLocalFullPath.Right(index-1);
			char * path = "c:/";
			debuglog(path,"%s:%d\n",strLocalPath,tryCount);
		} while (!isSucess);
		
	}
}
bool CUpdateNetEventSink::DownloadFile( CString strFileURLInServer, CString strFileLocalFullPath, bool bCallBack)
{
	ASSERT(strFileURLInServer != "" );
	ASSERT(strFileLocalFullPath != "" );
	CInternetSession session;
	CHttpConnection *  pHttpConnection = NULL;
	CHttpFile *  pHttpFile  =  NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;

	DWORD dwType;
	const int nTimeOut = 2000 ;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);  // 重试之间的等待延时 
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5);    // 重试次数 
	session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,7000);//接受数据的超时时间
	session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);//两次重试之间的间隔时间

	char *  pszBuffer = NULL;
	try
	{
		DWORD dwHttpRequestFlags= INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection  =  session.GetHttpConnection(strServer, wPort);
		pHttpFile  =  pHttpConnection -> OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject,NULL,1,NULL,NULL,dwHttpRequestFlags);
		if (pHttpFile -> SendRequest()  ==  FALSE)
			return false ;

		DWORD dwStateCode;
		pHttpFile -> QueryInfoStatusCode(dwStateCode);
		if (dwStateCode == HTTP_STATUS_OK)
		{
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);   // 创建本地文件 
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return   false ;
			}

			BOOL bResult = FALSE;
			CString strLength;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, strLength);
			DWORD dwFileSize = _wtoi(strLength);

			const int BUFFER_LENGTH = 1024;//buffer大小16K
			pszBuffer = new char [BUFFER_LENGTH];   // 读取文件的缓冲 
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);  // 读取服务器上数据 
			while (nRead > 0 )
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);   // 写到本地文件 
				dwTotalWrite += dwWrite;

				nRead = pHttpFile -> Read(pszBuffer, BUFFER_LENGTH);

				if (bCallBack)
					m_pCallBack->CurDownloadProc(CString(L""),dwTotalWrite, dwFileSize);
			}


			delete[]pszBuffer;
			pszBuffer  =  NULL;

			DWORD fileSize = GetFileSize(hFile,NULL);
			if(fileSize!=dwFileSize)
			{
				CloseHandle(hFile);
				throw(0);
			}
			CloseHandle(hFile);

			m_bIsDownloading = false;

			

			if (bCallBack)
				m_pCallBack->OnDownloadFinish(strFileURLInServer, strFileLocalFullPath);
		}
		else 
		{
			delete[]pszBuffer;
			pszBuffer  =  NULL;
			if (pHttpFile  !=  NULL)
			{
				pHttpFile -> Close();
				delete pHttpFile;
				pHttpFile  =  NULL;
			}
			if (pHttpConnection  !=  NULL)
			{
				pHttpConnection -> Close();
				delete pHttpConnection;
				pHttpConnection  =  NULL;
			}
			session.Close();
			return   false ;
		}
	}
	catch (...)
	{
		delete[]pszBuffer;
		pszBuffer  =  NULL;
		if (pHttpFile  !=  NULL)
		{
			pHttpFile -> Close();
			delete pHttpFile;
			pHttpFile  =  NULL;
		}
		if (pHttpConnection  !=  NULL)
		{
			pHttpConnection -> Close();
			delete pHttpConnection;
			pHttpConnection  =  NULL;
		}
		session.Close();

		return false ;
	}

	if (pHttpFile != NULL)
		pHttpFile->Close();
	if (pHttpConnection != NULL)
		pHttpConnection->Close();
	session.Close();

	return true ;
}

bool CUpdateNetEventSink::GameLog(CString strURL)
{
	//ASSERT(strFileURLInServer != "" );
	CInternetSession session;
	CHttpConnection *  pHttpConnection = NULL;
	CHttpFile *  pHttpFile  =  NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;

	DWORD dwType;
	const int nTimeOut = 2000 ;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);  // 重试之间的等待延时 
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5);    // 重试次数 
	session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT,7000);//接受数据的超时时间
	session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);       // 7秒的接收超时
	session.SetOption(INTERNET_OPTION_CONNECT_BACKOFF,500);//两次重试之间的间隔时间

	char *  pszBuffer = NULL;
	try
	{
		DWORD dwHttpRequestFlags= INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;
		AfxParseURL(strURL, dwType, strServer, strObject, wPort);
		pHttpConnection  =  session.GetHttpConnection(strServer, wPort);
		pHttpFile  =  pHttpConnection -> OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject,NULL,1,NULL,NULL,dwHttpRequestFlags);
		if (pHttpFile -> SendRequest()  ==  FALSE)
			return false ;

	}
	catch (...)
	{
		delete[]pszBuffer;
		pszBuffer  =  NULL;
		if (pHttpFile  !=  NULL)
		{
			pHttpFile -> Close();
			delete pHttpFile;
			pHttpFile  =  NULL;
		}
		if (pHttpConnection  !=  NULL)
		{
			pHttpConnection -> Close();
			delete pHttpConnection;
			pHttpConnection  =  NULL;
		}
		session.Close();

		return false ;
	}

	return true;
}