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
#ifndef CRNETAPP_SINGLEGAMEMETHODCLASS_H
#define CRNETAPP_SINGLEGAMEMETHODCLASS_H 1

#include <CRNetApp/appExport.h>
#include <CRNetApp/appDataClass.h>
#include <CRCore/crHandleManager.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRNetApp/appDataStreamTransfer.h>
#include <CRNetApp/appServerPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRText/crText.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRCore/crUniversalHandles.h>
#include <ode/ode.h>
#include <string>
#include <vector>
#include <list>

namespace CRNetApp{
class CRNETAPP_EXPORT crCreateSingleGameDataMethod : public CRCore::crMethod
{//单机游戏 Method
public:
	crCreateSingleGameDataMethod();
	crCreateSingleGameDataMethod(const crCreateSingleGameDataMethod& handle);
	MethodClass(CRNetApp, CreateSingleGameData)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_gameName;
	std::string m_gameCName;
	unsigned char m_rolecount;
	std::string m_roleName;
	std::string m_sceneName;
	std::string m_sceneFileName;
	std::string m_collidefile;
	std::string m_metierName;
	std::string m_metierScript;
	std::string m_tasktable;
	std::string m_resFileName;
	std::string m_resScript;
	std::string m_abstractName;
	std::string m_abstractScript;
	short m_zoffset;
};
class CRNETAPP_EXPORT crSingleGameItemInRangeTestMethod : public CRCore::crMethod
{//单机游戏 Method
public:
	crSingleGameItemInRangeTestMethod();
	crSingleGameItemInRangeTestMethod(const crSingleGameItemInRangeTestMethod& handle);
	MethodClass(CRNetApp, SingleGameItemInRangeTest)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<crRole> m_player;
	CRCore::ref_ptr<crInstanceItem> m_item;
};
class CRNETAPP_EXPORT crSingleGameRecvItemRTDataMethod : public CRCore::crMethod
{//单机游戏 Method
public:
	crSingleGameRecvItemRTDataMethod();
	crSingleGameRecvItemRTDataMethod(const crSingleGameRecvItemRTDataMethod& handle);
	MethodClass(CRNetApp, SingleGameRecvItemRTData)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	crInstanceItem* m_this;
};
class CRNETAPP_EXPORT crTransportCommandMethod : public CRCore::crMethod
{//单机游戏 Method
public:
	crTransportCommandMethod();
	crTransportCommandMethod(const crTransportCommandMethod& handle);
	MethodClass(CRNetApp, TransportCommand)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_command;
	int m_id;
};
class CRNETAPP_EXPORT crDoTransportCommandMethod : public CRCore::crMethod
{//单机游戏 Method
public:
	crDoTransportCommandMethod();
	crDoTransportCommandMethod(const crDoTransportCommandMethod& handle);
	MethodClass(CRNetApp, DoTransportCommand)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
	std::string m_command;
};
class CRNETAPP_EXPORT crShowTalkChooseMethod : public CRCore::crMethod
{
public:
	crShowTalkChooseMethod();
	crShowTalkChooseMethod(const crShowTalkChooseMethod& handle);
	MethodClass(CRNetApp, ShowTalkChoose)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	bool m_visiable;
	unsigned short m_buttonid;
	unsigned short m_textid;
	CRCore::ref_ptr<CRCore::crNode> m_filterNode;
};
class CRNETAPP_EXPORT crShowTalkPreviewMethod : public CRCore::crMethod
{
public:
	crShowTalkPreviewMethod();
	crShowTalkPreviewMethod(const crShowTalkPreviewMethod& handle);
	MethodClass(CRNetApp, ShowTalkPreview)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	bool m_visiable;
	CRCore::crNode* m_this;

	//text属性
	float m_characterSize;
	unsigned char m_alignmentType;
	CRCore::crVector4 m_color;
	CRCore::crVector4 m_textRect;
	float m_offset;
	//
	static std::string m_front;
	static CRCore::ref_ptr<CRText::crText> m_text;
	float m_characterSize2;
};

class CRNETAPP_EXPORT crShowTalkTextMethod : public CRCore::crMethod
{
public:
	crShowTalkTextMethod();
	crShowTalkTextMethod(const crShowTalkTextMethod& handle);
	MethodClass(CRNetApp, ShowTalkText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	std::string m_nodeName;
	bool m_visiable;
	CRCore::crNode* m_this;

	//text属性
	float m_characterSize;
	unsigned char m_alignmentType;
	CRCore::crVector4 m_color;
	CRCore::crVector4 m_textRect;
	//
	unsigned short m_textid;
	CRCore::crVector4 m_titleRect;
	std::string m_titleStr;
	static std::string m_front;
	static CRCore::ref_ptr<CRText::crText> m_text;
	static CRCore::ref_ptr<CRText::crText> m_titleText;
	float m_characterSize2;
};
class CRNETAPP_EXPORT crPlayerChooseTalkMethod : public CRCore::crMethod
{
public:
	crPlayerChooseTalkMethod();
	crPlayerChooseTalkMethod(const crPlayerChooseTalkMethod& handle);
	MethodClass(CRNetApp, PlayerChooseTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::crNode* m_this;
};
class CRNETAPP_EXPORT crCutTalkMethod : public CRCore::crMethod
{
public:
	crCutTalkMethod();
	crCutTalkMethod(const crCutTalkMethod& handle);
	MethodClass(CRNetApp, CutTalk)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned short m_talkid;
};
class CRNETAPP_EXPORT crCutSceneMethod : public CRCore::crMethod
{
public:
	crCutSceneMethod();
	crCutSceneMethod(const crCutSceneMethod& handle);
	MethodClass(CRNetApp, CutScene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned short m_id;
};
class CRNETAPP_EXPORT crSetSpecialActStateMethod : public CRCore::crMethod
{//Client Method
public:
	crSetSpecialActStateMethod();
	crSetSpecialActStateMethod(const crSetSpecialActStateMethod& handle);
	MethodClass(CRNetApp, SetSpecialActState)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned short m_specActState;
	unsigned char m_itemtype;
	int m_id;
	int m_roleid;
};
class CRNETAPP_EXPORT crShowTextMethod : public CRCore::crMethod
{
public:
	crShowTextMethod();
	crShowTextMethod(const crShowTextMethod& handle);
	MethodClass(CRNetApp, ShowText)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	unsigned short m_id;
};
class CRNETAPP_EXPORT crGlobalValue_ICBCMethod : public CRCore::crMethod
{
public:
	crGlobalValue_ICBCMethod();
	crGlobalValue_ICBCMethod(const crGlobalValue_ICBCMethod& handle);
	MethodClass(CRNetApp, GlobalValue_ICBC)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	enum GlobalType
	{
		Intervene,
	};
	static bool GetGlobalValue(unsigned short type, bool &value);
protected:
	static bool s_intervene;
	unsigned short m_type;
	union Value
	{
		bool m_bln;
		int m_int;
		float m_float;
	};
	Value m_value;
};
class CRNETAPP_EXPORT crIsInterveneMethod : public CRCore::crMethod
{
public:
	crIsInterveneMethod();
	crIsInterveneMethod(const crIsInterveneMethod& handle);
	MethodClass(CRNetApp, IsIntervene)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
};
class CRNETAPP_EXPORT crVR_ICBCChangeRoleMethod : public CRCore::crMethod
{//GameClient Method
public:
	crVR_ICBCChangeRoleMethod();
	crVR_ICBCChangeRoleMethod(const crVR_ICBCChangeRoleMethod& handle);
	MethodClass(CRNetApp, VR_ICBCChangeRole)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
protected:
	CRCore::ref_ptr<CRGUI::crElement> m_element;
	int m_addid;
	std::string m_roleTypeElement;
	std::string m_roleImageElement;
	std::string m_createElement;
	std::string m_defaultImage;
	typedef std::vector<std::string> FileVec;
	FileVec m_roleTypeVec;
	FileVec m_roleImageVec;
	static int s_currentid;
};
}
#endif