/**********************************************************************
*
*	文件:	   appMethod11.h
*
*	描述:	   
*
*	作者:	   于毅欣
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD_11_H
#define JXJ_APPMETHOD_11_H
#include <CRCore/crHandleManager.h>
#include <CRCore/crWidgetNode.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crRange.h>
#include <CREncapsulation/crLoader.h>
#include <CRNet/crNetContainer.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appDataClass.h>
#include <CRNetApp/appMsg.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appNetCallbacks.h>
#include <CRNetApp/appNetDefine.h>
#include <CRNetApp/appNetPackets.h>
#include <CRNetApp/appNetPackets2.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRText/crText.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>
#include <CRUI/crWidgetExtend.h>
namespace JXJ {

////负责显示对话框的时候初始化
//class crJXJUIImageDecorateUpdateMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJUIImageDecorateUpdateMethod(){};
//	crJXJUIImageDecorateUpdateMethod(const crJXJUIImageDecorateUpdateMethod& handle);
//	MethodClass(JXJ, JXJUIImageDecorateUpdate)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//
//	std::string m_Dialog;
//	std::string m_Radio1;
//	std::string m_Radio2;
//};
//
////发送购买头像消息
//class crJXJSendPlayerBuyIconMethod : public CRCore::crMethod
//{//Client Method
//public:
//	crJXJSendPlayerBuyIconMethod();
//	crJXJSendPlayerBuyIconMethod(const crJXJSendPlayerBuyIconMethod& handle);
//	MethodClass(JXJ, JXJSendPlayerBuyIcon)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	unsigned char m_buyIconIndex;	//从0开始
//};
//
////处理购买头像消息
//class crJXJRecvPlayerBuyIconMethod :public CRCore::crMethod
//{//GameServer & Client Method
//public:
//	crJXJRecvPlayerBuyIconMethod();
//	crJXJRecvPlayerBuyIconMethod(const crJXJRecvPlayerBuyIconMethod& handle);
//	MethodClass(JXJ, JXJRecvPlayerBuyIcon)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	int m_buyIconIndex;
//};
//
////使用头像或者背景
//class crJXJPlayerUseIconMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPlayerUseIconMethod();
//	crJXJPlayerUseIconMethod(const crJXJPlayerUseIconMethod& handle);
//	MethodClass(JXJ, JXJPlayerUseIcon)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	unsigned char m_useIconIndex;
//};
//
////服务器和客户端处理使用头像或者背景
//class crJXJProcPlayerUseIconMethod :public CRCore::crMethod
//{//Client GameServer Method
//public:
//	crJXJProcPlayerUseIconMethod();
//	crJXJProcPlayerUseIconMethod(const crJXJProcPlayerUseIconMethod& handle);
//	MethodClass(JXJ, JXJProcPlayerUseIcon)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRNetApp::crPlayerGameData *m_this;
//	CRCore::ref_ptr<CRCore::crStreamBuf> m_stream;
//	int m_netType;
//	unsigned char m_useIconIndex;
//};
//
//class crJXJPlayerIconShopInitMethod :public CRCore::crMethod
//{//Client Method
//public:
//	crJXJPlayerIconShopInitMethod();
//	crJXJPlayerIconShopInitMethod(const crJXJPlayerIconShopInitMethod& handle);
//	MethodClass(JXJ, JXJPlayerIconShopInit)
//		virtual void operator()(crHandle &handle);
//	virtual void inputParam(int i, void *param);
//	virtual void addParam(int i, const std::string& str);
//protected:
//	CRCore::crCanvasNode *m_this;
//	std::vector<std::string> m_ButtonName;
//	std::vector<std::string> m_ImageName;
//};

class crJXJRecvPlayerGameDataCreateStreamMethod :public CRCore::crMethod
{//GameServer Method
public:
	crJXJRecvPlayerGameDataCreateStreamMethod();
	crJXJRecvPlayerGameDataCreateStreamMethod(const crJXJRecvPlayerGameDataCreateStreamMethod& handle);
	MethodClass(JXJ, JXJRecvPlayerGameDataCreateStream)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRNetApp::crPlayerGameData *m_this;
	bool m_flag;

	unsigned int getCountryPlayerCount(int nOffset);	//得到某国的人数
};



}
#endif
