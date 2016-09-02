#pragma once

// Creature3DXPropPage.h : CCreature3DXPropPage 属性页类的声明。


// CCreature3DXPropPage : 有关实现的信息，请参阅 Creature3DXPropPage.cpp。

class CCreature3DXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCreature3DXPropPage)
	DECLARE_OLECREATE_EX(CCreature3DXPropPage)

// 构造函数
public:
	CCreature3DXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CREATURE3DX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

