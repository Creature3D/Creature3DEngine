
// qLoginDlg.h : 头文件
//
#pragma once


#include "UpdateNetEventSink.h"
#include "CDialogSK.h"
#include <string>
// CqLoginDlg 对话框
class CqLoginDlg : public CDialogSK, public CUpdateEventCallback
{
// 构造
public:
	CqLoginDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CqLoginDlg();
// 对话框数据
	enum { IDD = IDD_QLOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	virtual void OnCancel(){};
	virtual void OnOK(){};

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Install();
// 实现
protected:
	std::string getCardInfo();
	void vedioSetting();
	HICON m_hIcon;
	CUpdateNetEventSink m_Download;
	wchar_t m_strModule[256];	//路径
	CProgressCtrl m_Process;
	CStatic m_Text;
	CButton m_button1;
	CButton m_button2;
	byte m_btDownloadStage;
	BOOL m_bQuitProcess;	//退出程序
	UINT m_uCurDownloadSize;		//当前下载的大小
	UINT m_uCurDownloadFileSize;	//当前下载的文件大小
	bool flag;

	virtual void CurDownloadProc(CString & fileName,UINT uCurDownload, UINT uTotalSize);	//当前下载的字节数和总的文件大小
	virtual void OnDownloadFinish(const CString & strHttpFile, const CString & strLocalFile);	//现在文件完成的通知
	virtual void OnDecompressFile();
	virtual void OnGameLog();
	BOOL GetFileName(CString strPath, CString & strFileName);	//得到文件名
	void DoInitCheckGame();
	bool DoInitCheckUpdate();
	void CopyBatchFile2DirPath(CString strFilePath, CString strDirPath);	//将txt名的几个文件移动到指定目录中
	BOOL CanRunGame();	//查找Launcher.exe是否存在，如果不存在，说明是重新安装，则强制下载
	BOOL CheckReg4Game();	//在注册表中检测是否有游戏的启动项，说明游戏正确安装了;
	void CreateStartMenu();
	void CreateShortCut(CString csLinkPath, CString csExePath, CString csIconPath, CString csArg);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
