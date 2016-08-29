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
#ifndef CRENCAPSULATION_CRLOGICCLASS_H
#define CRENCAPSULATION_CRLOGICCLASS_H 1

#include <CREncapsulation/crExport.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crNode.h>
#include <CRCore/crMatrixTransform.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRGUI/crElement.h>
#include <vector>

namespace CREncapsulation{

class crTransformNodeLogic : public CRCore::crLogic
{//
public:
	crTransformNodeLogic();
	crTransformNodeLogic(const crTransformNodeLogic& handle);
	LogicClass(CREncapsulation, TransformNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;
    CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_panNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_rotNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_scaleNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_adjustEditorNodeMathod;
private:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
class crPlaceLoadedNodeLogic : public CRCore::crLogic
{//
public:
	crPlaceLoadedNodeLogic();
	crPlaceLoadedNodeLogic(const crPlaceLoadedNodeLogic& handle);
	LogicClass(CREncapsulation, PlaceLoadedNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
private:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
class crSpawnSelectNodesLogic : public CRCore::crLogic
{//
public:
	crSpawnSelectNodesLogic();
	crSpawnSelectNodesLogic(const crSpawnSelectNodesLogic& handle);
	LogicClass(CREncapsulation, SpawnSelectNodes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_newLight;
	CRCore::ref_ptr<CRCore::crHandle> m_newParticle;
	CRCore::ref_ptr<CRCore::crHandle> m_adjustEditorNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_setEditorCoordinateMathod;
	std::string m_newEditorNodeDes;
	bool m_mirror;
private:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
class crCenterModelLogic : public CRCore::crLogic
{////将当前所选中的模型，坐标系移动到模型中心
public:
	crCenterModelLogic();
	crCenterModelLogic(const crCenterModelLogic& handle);
	LogicClass(CREncapsulation, CenterModel)
		virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_setEditorCoordinateMathod;
private:
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
class crDeleteSelectNodesLogic : public CRCore::crLogic
{//
public:
	crDeleteSelectNodesLogic();
	crDeleteSelectNodesLogic(const crDeleteSelectNodesLogic& handle);
	LogicClass(CREncapsulation, DeleteSelectNodes)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_deleteNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_deleteLightSource;
	CRCore::ref_ptr<CRCore::crHandle> m_deleteParticle;
};

class crLoadModelLogic : public CRCore::crLogic
{//
public:
	crLoadModelLogic();
	crLoadModelLogic(const crLoadModelLogic& handle);
	LogicClass(CREncapsulation, LoadModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_addModelMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_placeLoadedNodeMathod;
	std::string m_rootName;
	std::string m_filePath;
	std::string m_fileExt;
	std::string m_fileName;
	std::string m_addDescription;
	CRCore::crGroup *m_root;

	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
};

class crGetListBoxStrLogic : public CRCore::crLogic
{//
public:
	crGetListBoxStrLogic();
	crGetListBoxStrLogic(const crGetListBoxStrLogic& handle);
	LogicClass(CREncapsulation, GetListBoxStr)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
	CRCore::ref_ptr<CRGUI::crElement> m_element;
    std::string m_listBoxNameID;
	std::string m_headStr;
};

class crNewEditorModelLogic : public CRCore::crLogic
{//
public:
	crNewEditorModelLogic();
	crNewEditorModelLogic(const crNewEditorModelLogic& handle);
	LogicClass(CREncapsulation, NewEditorModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_newEditorNode;
	CRCore::ref_ptr<CRCore::crHandle> m_loadModel;
    CRCore::ref_ptr<CRCore::crHandle> m_adjustEditorNodeMathod;
	CRCore::ref_ptr<CRCore::crHandle> m_setEditorCoordinateMathod;
	std::string m_newEditorNodeDes;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
	CRCore::ref_ptr<CRCore::crNode> m_effectNode;
};

class crFocusSelectNodeLogic : public CRCore::crLogic
{//
public:
	crFocusSelectNodeLogic();
	crFocusSelectNodeLogic(const crFocusSelectNodeLogic& handle);
	LogicClass(CREncapsulation, FocusSelectNode)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_focusNode;
};

class crInitLightEditorModelLogic : public CRCore::crLogic
{//
public:
	crInitLightEditorModelLogic();
	crInitLightEditorModelLogic(const crInitLightEditorModelLogic& handle);
	LogicClass(CREncapsulation, InitLightEditorModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crGroup *m_root;
	std::string m_rootName;
	CRCore::ref_ptr<CRCore::crHandle> m_loadModel;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
};

class crInitParticleEditorModelLogic : public CRCore::crLogic
{//
public:
	crInitParticleEditorModelLogic();
	crInitParticleEditorModelLogic(const crInitParticleEditorModelLogic& handle);
	LogicClass(CREncapsulation, InitParticleEditorModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::crGroup *m_root;
	std::string m_rootName;
	CRCore::ref_ptr<CRCore::crHandle> m_loadModel;
	CRCore::ref_ptr<CRCore::crNode> m_loadedNode;
};

class crGetModelFileNameLogic : public CRCore::crLogic
{//
public:
	crGetModelFileNameLogic();
	crGetModelFileNameLogic(const crGetModelFileNameLogic& handle);
	LogicClass(CREncapsulation, GetModelFileName)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
	std::string m_filePath;
	std::string m_ext;
};

class crModifyDrawableMaterialLogic : public CRCore::crLogic
{//
public:
	crModifyDrawableMaterialLogic();
	crModifyDrawableMaterialLogic(const crModifyDrawableMaterialLogic& handle);
	LogicClass(CREncapsulation, ModifyDrawableMaterial)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_modifyMaterial;
	CRCore::ref_ptr<CRCore::crHandle> m_modifyObjectDrawable;
	CRCore::ref_ptr<CRCore::crObject> m_currentSelectObject;
	CRCore::ref_ptr<CRCore::crDrawable> m_currentSelectDrawable;
};

class crProxyElementLogic : public CRCore::crLogic
{//
public:
	crProxyElementLogic();
	crProxyElementLogic(const crProxyElementLogic& handle);
	LogicClass(CREncapsulation, ProxyElement)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crHandle> m_handle;
	CRCore::ref_ptr<CRGUI::crElement> m_proxyElement;
	std::string m_consignerElement;
};

class crMousePickMoveLogic : public CRCore::crLogic
{//
public:
	crMousePickMoveLogic();
	crMousePickMoveLogic(const crMousePickMoveLogic& handle);
	LogicClass(CREncapsulation, MousePickMove)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRProducer::crGUIEventAdapter> m_ea;
	CRCore::ref_ptr<CRCore::crHandle> m_pickTarget;
	CRCore::ref_ptr<CRCore::crHandle> m_moveToTarget;
	CRCore::ref_ptr<CRCore::crHandle> m_talkWithBot;
	int m_targetType;
	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_targetNode;
	CRCore::crVector3 m_targerPosition;
};

class crRpgFireAndFollowLogic : public CRCore::crLogic
{//WCH_UPDATEVISITOR 
public:
	crRpgFireAndFollowLogic();
	crRpgFireAndFollowLogic(const crRpgFireAndFollowLogic& handle);
	LogicClass(CREncapsulation, RpgFireAndFollow)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	_crInt64 m_param;

	CRCore::ref_ptr<CRCore::crHandle> m_turnToTarget;
	CRCore::ref_ptr<CRCore::crHandle> m_moveToTarget;

	CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_targetNode;
	CRCore::crVector3 m_targerPosition;
};

class crBotAILogic : public CRCore::crLogic
{//WCH_UPDATEVISITOR 
public:
	crBotAILogic();
	crBotAILogic(const crBotAILogic& handle);
	LogicClass(CREncapsulation, BotAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;

	CRCore::ref_ptr<CRCore::crHandle> m_botAISelectTarget;
	CRCore::ref_ptr<CRCore::crHandle> m_botAIPatrol;
	CRCore::ref_ptr<CRCore::crHandle> m_rpgFireAndFollow;
    CRCore::ref_ptr<CRCore::crHandle> m_botAIAvoidCollide;
	bool m_avoidCollide;
};
class crPlayerAILogic : public CRCore::crLogic
{//WCH_UPDATEVISITOR 
public:
	crPlayerAILogic();
	crPlayerAILogic(const crPlayerAILogic& handle);
	LogicClass(CREncapsulation, PlayerAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRCore::crHandle> m_rpgFireAndFollow;
};
class crNetPlayerAILogic : public CRCore::crLogic
{//WCH_UPDATEVISITOR 
public:
	crNetPlayerAILogic();
	crNetPlayerAILogic(const crNetPlayerAILogic& handle);
	LogicClass(CREncapsulation, NetPlayerAI)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRCore::ref_ptr<CRCore::crNode> m_node;
	CRCore::ref_ptr<CRCore::crHandle> m_moveToTarget;
	CRCore::ref_ptr<CRCore::crHandle> m_rpgFireAndFollow;
};
class crShowTalkStageLogic : public CRCore::crLogic
{// 
public:
	crShowTalkStageLogic();
	crShowTalkStageLogic(const crShowTalkStageLogic& handle);
	LogicClass(CREncapsulation, ShowTalkStage)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
	struct ButtonHandle : public CRCore::Referenced
	{
		std::string m_buttonName;
		std::string m_buttonText;
	    CRCore::ref_ptr<CRCore::crHandle> m_buttonHandle;
	};
	typedef std::vector< CRCore::ref_ptr<ButtonHandle> > ButtonHandleArray;
protected:
	std::string m_stageName;
	int m_showType;
	std::string m_textElementName;
	std::string m_text;
	ButtonHandleArray m_buttonHandleArray;
};
class crUpdateModelLogic : public CRCore::crMethod
{//
public:
	crUpdateModelLogic();
	crUpdateModelLogic(const crUpdateModelLogic& handle);
	LogicClass(CREncapsulation, UpdateModel)
	virtual void operator()(crHandle &handle);
	virtual void inputParam(int i, void *param);
	virtual void addParam(int i, const std::string& str);
	virtual void inputHandle(int i, void *param);
	virtual void outputParam(int i, void *param);
protected:
	CRGUI::crElement* m_this;
	std::string m_modelListElement;
	CRCore::ref_ptr<CRCore::crHandle> m_convertToTransformMathod;
	CRCore::ref_ptr<CRCore::crMatrixTransform> m_convertedTransform;
};
}
#endif