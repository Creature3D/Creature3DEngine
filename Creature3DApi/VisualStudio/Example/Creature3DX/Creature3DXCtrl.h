#pragma once
// Creature3DXCtrl.h : CCreature3DXCtrl ActiveX 控件类的声明。
#include <string>

// CCreature3DXCtrl : 有关实现的信息，请参阅 Creature3DXCtrl.cpp。

class CCreature3DXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCreature3DXCtrl)

// 构造函数
public:
	CCreature3DXCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	void StartCreature3D();
	void needRestart();
	void waitClose();
	void exitGame();
// 实现
protected:
	~CCreature3DXCtrl();
	afx_msg int OnCreate(LPCREATESTRUCT   lpCreateStruct); 
	afx_msg void OnNcDestroy();
	//bool m_exit;
	HMODULE m_creature3d;
	HANDLE m_thread;
	std::string m_clientPath;
	bool m_needRestart;
	//crGameThread *m_gameThread;
	DECLARE_OLECREATE_EX(CCreature3DXCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CCreature3DXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCreature3DXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CCreature3DXCtrl)		// 类型名称和杂项状态
	CString m_strLoginInfo;

	void WriteLoginInfo2File();	//将登录信息写文件
// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
	};
};

