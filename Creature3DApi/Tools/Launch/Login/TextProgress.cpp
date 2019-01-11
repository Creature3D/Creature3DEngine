#include "stdafx.h"
#include "TextProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CTextProgress::CTextProgress()
{
	m_crText  = RGB(0, 0, 0);
	m_crBlank  = RGB(255, 255, 255);
	m_crProgress = RGB(69, 181, 170);
	m_strText = TEXT("");
}
CTextProgress::~CTextProgress()
{

}

BEGIN_MESSAGE_MAP(CTextProgress, CProgressCtrl)
	//{{AFX_MSG_MAP(CTextProgress)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTextProgress::OnPaint()
{
	PAINTSTRUCT ps;
	CDC* pDC = BeginPaint(&ps);      //开始绘制
	int nPos = GetPos();       //获取当前进度条的位置
	CRect clientRC;
	GetClientRect(clientRC);
	CSize sztext = pDC->GetTextExtent(m_strText);
	int nX = (clientRC.Width() - sztext.cx) / 2;
	int nY = (clientRC.Height() - sztext.cy) / 2;
	pDC->SetBkMode(TRANSPARENT); 
	int nMin, nMax;
	GetRange(nMin, nMax); 
	double dFraction = (double)clientRC.Width() / (nMax-nMin);
	int nLeft = nPos * dFraction;
	CRect leftRC  = clientRC;
	leftRC.right  = nLeft;
	CRect rightRC = clientRC;
	rightRC.left  = nLeft;
	pDC->FillRect(leftRC, &CBrush(m_crProgress));
	pDC->FillRect(rightRC, &CBrush(m_crBlank));
	pDC->SetTextColor(m_crText);
	pDC->TextOut(nX, nY, m_strText);
	ReleaseDC(pDC);
	EndPaint(&ps);
}

void CTextProgress::setText(CString & str)
{
	m_strText = str;
}