#pragma once
#include "afxwin.h"


#include <vector>
#include <string>

typedef std::vector<std::string> DirVec;

// CUninstallDlg 对话框

class CUninstallDlg : public CDialog
{
	DECLARE_DYNAMIC(CUninstallDlg)

public:
	CUninstallDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUninstallDlg();

// 对话框数据
	enum { IDD = IDD_UNINSTALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	CString GetCurrentUninstallDir();
	//bool GetJXJProgressExit();
	BOOL CheckReg4Game();

	HANDLE m_thread;

	bool m_bDelete;

	DirVec m_dirVec;

	void GetUnInstallList(std::wstring & dir);
	BOOL DeleteLink( LPCWSTR lpszShortcut);
	//bool onStartUninstall();
public:
	afx_msg void OnBnClickedCancel();
	CEdit m_edit;
	wchar_t m_strModule[MAX_PATH];	//路径
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool UninstallGame();	//卸载游戏
	void addTextToEditBox(const wchar_t * str);
	void dlgClose();
	void setBtText(const wchar_t * str);
	bool DeleteDirectory(const wchar_t * sDirName);
};
