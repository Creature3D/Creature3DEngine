// LauncherDlg.h : header file
//

#pragma once

#include "CDialogSK.h"
#include "BtnST.h"
#include <afxhtml.h>
#include "UpdateNetEventSink.h"
#include "afxcmn.h"
#include "UninstallDlg.h"

#include "TextProgress.h"
#include <string>
using namespace std;
//#include "UninstallDlg.h"
//网页浏览器封装
class CWebBrowser : public CHtmlView
{
	DECLARE_DYNCREATE(CWebBrowser)
	DECLARE_MESSAGE_MAP()

public:
	CWebBrowser();          
	virtual ~CWebBrowser();

	BOOL IsContextMenuEnabled() const { return m_bContextMenuEnabled; }
	void EnableContextMenu(BOOL newVal = TRUE) { m_bContextMenuEnabled = newVal; }

	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);	//去掉框的边缘的滚动条

	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags,
		LPCTSTR lpszTargetFrameName, CByteArray& baPostedData,
		LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual HRESULT OnTranslateUrl(DWORD dwTranslate,
		OLECHAR* pchURLIn, OLECHAR** ppchURLOut);

	CString m_strGameDir;

protected:
	
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();

protected:
	virtual void OnTitleChange(LPCTSTR lpszText);
	virtual void OnDocumentComplete(LPCTSTR lpszURL);

protected:
	BOOL m_bContextMenuEnabled;
};



// CLauncherDlg dialog
class CLauncherDlg : public CDialogSK, public CUpdateEventCallback
{
// Construction
public:
	CLauncherDlg(CWnd* pParent = NULL);	// standard constructor
	~CLauncherDlg();

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual void OnCancel();
	virtual void OnOK(){};
	void vedioSetting();
// Implementation
protected:
	HICON m_hIcon;
	CButtonST m_btnStartGame;	//开始游戏
	CButtonST m_btnQuit;		//关闭程序
	CButtonST m_btnMiniDlg;		//最小化程序
	CButtonST m_btnVideoSetting;//显示设置按钮
	CButtonST m_btnVideoLevel[5];			//显示模式
	CButtonST m_btnSelect;		//选择模式
	CButtonST m_btnSelectCancel;//取消选择
	CString m_loginurl;

	CWebBrowser m_html;	//浏览器控件
	CWebBrowser m_htmlWait;	//等待的时候显示的页面
	BOOL m_isWait;

	CUpdateNetEventSink m_Download;	//下载

	BOOL m_bDownloadFinish;	//下载结束
	BOOL m_bQuitProcess;	//退出程序
	UINT m_uCurDownloadSize;		//当前下载的大小
	UINT m_uCurDownloadFileSize;	//当前下载的文件大小
	CString m_strCurDownFileName;
	CUninstallDlg m_uidlg;
	//CStatic m_staticText;//静态文本框
	virtual void CurDownloadProc(CString & fileName,UINT uCurDownload, UINT uTotalSize);	//当前下载的字节数和总的文件大小
	virtual void OnDownloadFinish(const CString & strHttpFile, const CString & strLocalFile);	//现在文件完成的通知
	virtual void OnDecompressFile();

	BOOL CheckReg4Game();	//在注册表中检测是否有游戏的启动项，说明游戏正确安装了

	void RunGame();	//运行游戏
	void RunSetup();//运行安装游戏

	wchar_t m_strModule[256];	//路径

	void CreateDesktopShortCut(const wchar_t * pDir);	//创建桌面快捷方式
	BOOL CanRunGame();	//查找Launcher.exe是否存在，如果不存在，说明是重新安装，则强制下载

	BOOL GetFileName(CString strPath, CString & strFileName);	//得到文件名

	void CopyBatchFile2DirPath(CString strFilePath, CString strDirPath);	//将txt名的几个文件移动到指定目录中

	void DoInitCheckGame();
	bool DoInitCheckUpdate();
	void CreateStartMenu();
	void CreateShortCut(CString csLinkPath, CString csExePath, CString csIconPath, CString csArg);
	string getCardInfo();
	byte m_btDownloadStage;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartGame();
	afx_msg void OnBnClickedButtonMini();
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonVideoSetting();
	afx_msg void OnBnClickedButtonLevelVeryLow();
	afx_msg void OnBnClickedButtonLevelLow();
	afx_msg void OnBnClickedButtonLevelNormal();
	afx_msg void OnBnClickedButtonLevelHigh();
	afx_msg void OnBnClickedButtonLevelVeryHigh();
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CTextProgress m_Process1;
/*	afx_msg void OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult);*/
};
