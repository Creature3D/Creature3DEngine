/**********************************************************************
*
*	文件:	   appMethod17.h
*
*	描述:	   
*
*	作者:	   熊辉
*					
*
**********************************************************************/
#ifndef JXJ_APPMETHOD17_H
#define JXJ_APPMETHOD17_H
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

namespace JXJ
{
	class crJXJUIManorStairJueSeJunlingTipMethod : public CRCore::crMethod
	{
	public:
		crJXJUIManorStairJueSeJunlingTipMethod();
		crJXJUIManorStairJueSeJunlingTipMethod(const crJXJUIManorStairJueSeJunlingTipMethod& handle);
		MethodClass(JXJ,JXJUIManorStairJueSeJunlingTip)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_tips;
		std::string m_tipstext;
		std::string m_jueSeJumLingCanvas;
		int m_str;
	};
	class crJXJUIActiveCardExtraTipMethod : public CRCore::crMethod
	{
	public:
		crJXJUIActiveCardExtraTipMethod();
		crJXJUIActiveCardExtraTipMethod(const crJXJUIActiveCardExtraTipMethod& handle);
		MethodClass(JXJ,JXJUIActiveCardExtraTip)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_tips;
		std::string m_tipstext;
		std::string m_jiangkajihuoCanvas;
		int m_str;
	};
	class crJXJAttachStoreFulianMethod : public CRCore::crMethod
	{
	public:
		crJXJAttachStoreFulianMethod();
		crJXJAttachStoreFulianMethod(const crJXJAttachStoreFulianMethod& handle);
		MethodClass(JXJ,JXJAttachStoreFulian)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode* m_this;
		std::string m_promptCanvas;
		std::string m_information;
		std::string m_okbtn;
	};
	class crJXJButtonExpendHintMethod : public CRCore::crMethod
	{
	public:
		crJXJButtonExpendHintMethod();
		crJXJButtonExpendHintMethod(const crJXJButtonExpendHintMethod& handle);
		MethodClass(JXJ,JXJButtonExpendHint)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode* m_this;
		int m_id;
		std::string m_promptCanvas;
		std::string m_information;
		std::string m_okbtn;
	};
	//class crJXJCheckGamePlanTaskMethod : public CRCore::crMethod
	//{//GameClient Method 
	//public:
	//	crJXJCheckGamePlanTaskMethod();
	//	crJXJCheckGamePlanTaskMethod(const crJXJCheckGamePlanTaskMethod& handle);
	//	MethodClass(JXJ, JXJCheckGamePlanTask)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRCore::crCanvasNode *m_this;
	//	char m_trueExecCount;
	//	int m_case;
	//};
	class crJXJQiyuTaskUpdateMethod : public CRCore::crMethod
	{//GameServer Method 
	public:
		crJXJQiyuTaskUpdateMethod();
		crJXJQiyuTaskUpdateMethod(const crJXJQiyuTaskUpdateMethod& handle);
		MethodClass(JXJ, JXJQiyuTaskUpdate)
		virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRNetApp::crPlayerGameData* m_this;
		float* m_dt;
		int m_time;
	};
	//class crJXJCheckQiYuTaskUpdateMethod : public CRCore::crMethod
	//{//GameClient Method 
	//public:
	//	crJXJCheckQiYuTaskUpdateMethod();
	//	crJXJCheckQiYuTaskUpdateMethod(const crJXJCheckQiYuTaskUpdateMethod& handle);
	//	MethodClass(JXJ, JXJCheckQiYuTaskUpdate)
	//	virtual void operator()(crHandle &handle);
	//	virtual void inputParam(int i, void *param);
	//	virtual void addParam(int i, const std::string& str);
	//protected:
	//	CRNetApp::crPlayerGameData* m_this;
	//	float* m_dt;
	//};
	class crJXJUIDixingAdditionTipMethod : public CRCore::crMethod
	{
	public:
		crJXJUIDixingAdditionTipMethod();
		crJXJUIDixingAdditionTipMethod(const crJXJUIDixingAdditionTipMethod& handle);
		MethodClass(JXJ,JXJUIDixingAdditionTip)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_tips;
		std::string m_tipstext;
		int m_str;
	};
	class crJXJGetViplvRewardTipMethod : public CRCore::crMethod
	{
	public:
		crJXJGetViplvRewardTipMethod();
		crJXJGetViplvRewardTipMethod(const crJXJGetViplvRewardTipMethod& handle);
		MethodClass(JXJ,JXJGetViplvRewardTip)
			virtual void operator()(crHandle &handle);
		virtual void inputParam(int i, void *param);
		virtual void addParam(int i, const std::string& str);
	protected:
		CRCore::crWidgetNode *m_this;
		std::string m_tips;
		std::string m_tipstext;
		std::string m_jueSeJumLingCanvas;
		int m_str;
	};
}
#endif 