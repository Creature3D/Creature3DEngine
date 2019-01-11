#pragma once

class CUpdateEventCallback
{
public:
	CUpdateEventCallback(){}

	virtual void CurDownloadProc(CString & fileName,UINT uCurDownload, UINT uTotalSize){};	//当前下载的字节数和总的文件大小

	virtual void OnDownloadFinish(const CString & strHttpFile, const CString & strLocalFile){};	//现在文件完成的通知

	virtual void OnDecompressFile(){};	//开始解压文件

	virtual void OnGameLog(){};
};

//下载文件
class CUpdateNetEventSink
{
public:
	CUpdateNetEventSink(CUpdateEventCallback * pCallback);
	~CUpdateNetEventSink(void);

	bool Init();
	void Uninit();

	void StartDownload(CString strFileURLInServer, CString strFileLocalFullPath);

	void ProcDownload();	//执行下载任务

	bool m_bIsDownloading;	//是否正在下载

	bool m_bStartCompress;	//是否开始解压
	BOOL m_bQuitProcess;	//退出程序

	wchar_t m_strModule[256];	//路径

	bool DownloadFile( CString strFileURLInServer, CString strFileLocalFullPath, bool bCallBack = true);	//下载文件
	bool GameLog(CString strURL);
protected:
	CString m_strFileURLInServer;
	CString m_strFileLocalFullPath;

	//wchar_t m_szFileURLInServer[500];
	//wchar_t m_szFileLocalFullPath[500];

	CWinThread * m_pThread;		//下载线程

	bool m_bInit;	//是否初始化过
public:
	CUpdateEventCallback * m_pCallBack;	//事件通知
};

