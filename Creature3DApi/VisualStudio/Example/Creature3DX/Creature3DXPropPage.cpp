// Creature3DXPropPage.cpp : CCreature3DXPropPage 属性页类的实现。

#include "stdafx.h"
#include "Creature3DX.h"
#include "Creature3DXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCreature3DXPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CCreature3DXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CCreature3DXPropPage, "CREATURE3DX.Creature3DXPropPage.1",
	0x5ddfb85, 0x9dfd, 0x4d06, 0x91, 0x3f, 0x4c, 0x19, 0x46, 0xfc, 0x6a, 0x1a)



// CCreature3DXPropPage::CCreature3DXPropPageFactory::UpdateRegistry -
// 添加或移除 CCreature3DXPropPage 的系统注册表项

BOOL CCreature3DXPropPage::CCreature3DXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CREATURE3DX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CCreature3DXPropPage::CCreature3DXPropPage - 构造函数

CCreature3DXPropPage::CCreature3DXPropPage() :
	COlePropertyPage(IDD, IDS_CREATURE3DX_PPG_CAPTION)
{
}



// CCreature3DXPropPage::DoDataExchange - 在页和属性间移动数据

void CCreature3DXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CCreature3DXPropPage 消息处理程序
