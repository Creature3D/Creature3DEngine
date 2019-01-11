#include "StdAfx.h"
#include "UpdateNetEventSink.h"
#include <afxinet.h>

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

				//运行install.cmd
				//wstring strTmp(m_strModule);
				//strTmp += L"/Install.cmd";
				//_wsystem(strTmp.c_str());

				//每个命令执行
				//swprintf(buf, L"regsvr32 /s /u %s/Bin/Creature3DX.ocx", pNet->m_strModule);
				//if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				//{
				//	WaitForSingleObject(pi.hProcess,INFINITE);
				//	CloseHandle(pi.hThread);
				//	CloseHandle(pi.hProcess);
				//}
				//else
				//	AfxMessageBox(L"regsvr32");
				//swprintf(buf, L"%s/RegisterKeyNP.exe -del", pNet->m_strModule);
				//if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				//{
				//	WaitForSingleObject(pi.hProcess,INFINITE);
				//	CloseHandle(pi.hThread);
				//	CloseHandle(pi.hProcess);
				//}
				//else
				//	AfxMessageBox(L"RegisterKeyNP");
				//每个命令执行
				swprintf(buf, L"regsvr32 /s %s/bin/npJXJWebPlayer.dll\0", pNet->m_strModule);
				if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				{
					WaitForSingleObject(pi.hProcess,INFINITE);
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
				}
				else
					AfxMessageBox(L"regsvr32");
				//swprintf(buf, L"regsvr32 /s %s/Bin/Creature3DX.ocx", pNet->m_strModule);
				//if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				//{
				//	WaitForSingleObject(pi.hProcess,INFINITE);
				//	CloseHandle(pi.hThread);
				//	CloseHandle(pi.hProcess);
				//}
				//else
				//	AfxMessageBox(L"regsvr32");
				//swprintf(buf, L"%s/RegisterKeyNP.exe -reg", pNet->m_strModule);
				//if(CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, pNet->m_strModule, &si, &pi))
				//{
				//	WaitForSingleObject(pi.hProcess,INFINITE);
				//	CloseHandle(pi.hThread);
				//	CloseHandle(pi.hProcess);
				//}
				//else
				//	AfxMessageBox(L"RegisterKeyNP");
			}
			else
				AfxMessageBox(L"解压失败");

			pNet->m_bQuitProcess = TRUE;
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
		pHttpFile->AddRequestHeaders(L"User-Agent:JXJ/r/n",HTTP_ADDREQ_FLAG_ADD_IF_NEW);
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

