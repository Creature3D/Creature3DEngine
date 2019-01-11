// creWebDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "creBroswer.h"
#include "creWebDialog.h"


// creWebDialog 对话框

IMPLEMENT_DYNCREATE(creWebDialog, CDHtmlDialog)

creWebDialog::creWebDialog(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(creWebDialog::IDD, creWebDialog::IDH, pParent)
{

}

creWebDialog::~creWebDialog()
{
}

void creWebDialog::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL creWebDialog::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(creWebDialog, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(creWebDialog)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// creWebDialog 消息处理程序

HRESULT creWebDialog::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT creWebDialog::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


LRESULT creWebDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if(message == WM_ERASEBKGND)
	//	return TRUE;
	return CDHtmlDialog::WindowProc(message, wParam, lParam);
}
