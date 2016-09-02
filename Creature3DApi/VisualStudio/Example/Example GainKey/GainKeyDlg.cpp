/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
// GainKeyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GainKey.h"
#include "GainKeyDlg.h"
#include ".\gainkeydlg.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#include <string>

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGainKeyDlg 对话框



CGainKeyDlg::CGainKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGainKeyDlg::IDD, pParent)
	, m_keyStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGainKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_keyStr);
}

BEGIN_MESSAGE_MAP(CGainKeyDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CGainKeyDlg 消息处理程序

BOOL CGainKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CGainKeyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGainKeyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CGainKeyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define Rnd(i) i * rand() / RAND_MAX

#define ShaderDebug

#ifdef ShaderDebug
std::string basecode = "CSDA0B1JF2L4RNET3XW8VZ6Y7QGKU9MP5";
#else
std::string basecode = "A0BD1JCF2L4RNETS3XW8VZ6Y7QGKU9MP5";
#endif

std::string getRegistrationCode()
{
	srand( (unsigned)time( NULL ) );

	std::string code;
	char abc[4];
	int i[4];
	int iabc[3];
	int j,k;
	for(j = 0; j<3; j++)
	{
		for(k = 0; k<4; k++)
		{
			i[k] = Rnd(33);
			abc[k] = basecode[i[k]];

			code.push_back(abc[k]);
		}
		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
		code.push_back(basecode[iabc[j]]);
		code.push_back('-');
	}

	int iend[5];
	iend[0] = iabc[0] + iabc[1] + iabc[2];
	iend[1] = iabc[1] + iabc[2] + iend[0];
	iend[2] = iabc[2] + iend[0] + iend[1];
	iend[3] = iend[0] + iend[1] + iend[2];
	iend[4] = iend[1] + iend[2] + iend[3];

	for(j = 0; j<5; j++)
	{
		code.push_back(basecode[iend[j] % 21]);
	}

	return code;
}
//////AWCQF_WSRUL_GYJKW_E8281
////bool incorrectRegistrationCode(std::string &code)
////{
////	//encode(code);
////
////	char abc[4];
////	char end;
////	int i[4];
////	int iabc[3];
////	int j,k;
////	for(j = 0; j<3; j++)
////	{
////		for(k = 0; k<4; k++)
////		{
////			abc[k] = code[(j*6+k)];
////			i[k] = basecode.find(abc[k]);
////			if(i[k] == -1) return false;
////		}
////
////		iabc[j] = (i[0]+i[1]+i[2]+i[3])%21;
////		if(j == 0)
////		{    
////			end = code[4];
////			if(code[5] != '-') 
////				return false;
////		}
////		else if(j == 1)
////		{
////			end = code[10];
////			if(code[11] != '-') 
////				return false;
////		}
////		else if(j == 2)
////		{
////			end = code[16];
////			if(code[17] != '-') 
////				return false;
////		}
////		if(iabc[j]!=basecode.find(end))
////			return false;
////	}
////
////
////	int iend[5];
////	iend[0] = iabc[0] + iabc[1] + iabc[2];
////	iend[1] = iabc[1] + iabc[2] + iend[0];
////	iend[2] = iabc[2] + iend[0] + iend[1];
////	iend[3] = iend[0] + iend[1] + iend[2];
////	iend[4] = iend[1] + iend[2] + iend[3];
////
////	for(j = 0; j<5; j++)
////	{
////		if(iend[j] % 21 != basecode.find(code[18+j]))
////			return false;
////	}
////
////	return true;
////}

void CGainKeyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_keyStr = getRegistrationCode().c_str();
    UpdateData(false);

	//OnOK();
}

void CGainKeyDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	////std::string ss = m_keyStr.GetBuffer();
	////bool bln = incorrectRegistrationCode(ss);
	////m_keyStr = bln?"1":"0";
	////UpdateData(false);

	OnCancel();
}
