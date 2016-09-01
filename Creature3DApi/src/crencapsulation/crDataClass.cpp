/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include <CREncapsulation/crDataClass.h>

//#include <CRAI/crAIGroup.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crViewer.h>
#include <CRCore/crBrain.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRIOManager/crLoadManager.h>

#include <CRPhysics/crViewMatterObject.h>

#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRIOManager/crReadFile.h>
#include "rcfg/ConfigScriptOO.h"
#include <rcfg/ItemMap.h>
#include <CRParticle/crParticleEffect.h>
#include <sys/stat.h>
#include <stdio.h>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRGUI;
using namespace CRPhysics;
using namespace CRParticle;
/////////////////////////////////////////
//
//crEditorInfoData
//
/////////////////////////////////////////
crEditorInfoData::crEditorInfoData():
m_selectType(0),
m_transformNodeMode(0),
m_panMode(0),
m_rotMode(2),
m_scaleMode(0),
m_transformNodeSpeed(0.3f),
m_terEditMode(TER_Pick),
m_currentTerTool(0),
m_terToolRadius(32),
m_terToolHeight(2.0f),
m_sceneModified(false)
{
	init();
}
crEditorInfoData::crEditorInfoData(const crEditorInfoData& data):
crData(data),
m_selectType(data.m_selectType),
m_transformNodeMode(data.m_transformNodeMode),
m_panMode(data.m_panMode),
m_rotMode(data.m_rotMode),
m_scaleMode(data.m_scaleMode),
m_selectClassName(data.m_selectClassName),
m_transformNodeSpeed(data.m_transformNodeSpeed),
m_terEditMode(data.m_terEditMode),
m_terTools(data.m_terTools),
m_currentTerTool(data.m_currentTerTool),
m_terToolRadius(data.m_terToolRadius),
m_terToolHeight(data.m_terToolHeight),
m_currentFocusTexture(data.m_currentFocusTexture),
m_sceneModified(data.m_sceneModified)
{
}

void crEditorInfoData::init()
{
	m_selectClassName.push_back("ProxyNode");
	m_selectClassName.push_back("PickObject");
	m_selectClassName.push_back("PickMesh");
	m_selectClassName.push_back("CreBodyNode");
	m_selectClassName.push_back("crGroup");
	m_selectClassName.push_back("crTraverseStringNode");
	m_selectClassName.push_back("crDOFTransform");
	m_selectClassName.push_back("crSequence");
	m_selectClassName.push_back("crMatrixSequence");
	m_selectClassName.push_back("crMultiSwitch");
	m_selectClassName.push_back("crLod");
	m_selectClassName.push_back("crPagedLOD");
	m_selectClassName.push_back("crVolumeNode");
	m_selectClassName.push_back("crMatterGroup");
	m_selectClassName.push_back("crViewMatterObject");
	m_selectClassName.push_back("crTerrainTile");
	m_selectClassName.push_back("crMatrixTransform");
	m_selectClassName.push_back("crBoxMatterObject");
	m_selectClassName.push_back("crSphereMatterObject");
	m_selectClassName.push_back("crCCylinderMatterObject");
	m_selectClassName.push_back("crMeshMatterObject");
	m_selectClassName.push_back("PickEditorNode");
	m_selectClassName.push_back("PickWaypoint");
}

void crEditorInfoData::addParam(int i, const std::string& str)
{
	ref_ptr<crImage> heightimage = CRIOManager::readImageFile(str);
	if(heightimage.valid())
	{
		m_terTools.push_back(heightimage.get());
	}
}

void crEditorInfoData::inputParam(int i, void *param)
{
	switch(i) 
	{
	case EDP_:
		if(param==NULL)
		{
			m_currentSelectDrawable = NULL;
		}
		break;
	case EDP_SelectType:
		m_selectType = *((int*)param);
		break;
	case EDP_TransformNodeMode:
        m_transformNodeMode = *((int*)param);
		break;
	case EDP_PanMode:
		m_panMode = *((int*)param);
		break;
	case EDP_RotMode:
		m_rotMode = *((int*)param);
		break;
	case EDP_ScaleMode:
		m_scaleMode = *((int*)param);
		break;
	case EDP_TransformNodeSpeed:
		m_transformNodeSpeed = *((float*)param);
		break;
	case EDP_SelectClassName:
		break;
	case EDP_CurrentSelectDrawable:
		m_currentSelectDrawable = param==NULL?NULL:(crDrawable*)param;
		break;
	case EDP_CanMove:
		break;
	case EDP_TerEditMode:
		m_terEditMode = param == NULL?TER_Pick:*((unsigned char*)param);
		break;
	case EDP_CurrentTerTool:
		m_currentTerTool = param == NULL?0:*((unsigned char*)param);
		break;
	case EDP_TerToolRadius:
		m_terToolRadius = param == NULL?0:*((int*)param);
		break;
	case EDP_TerToolHeight:
		m_terToolHeight = param == NULL?0:*((float*)param);
		break;
	case EDP_CurrentFocusTexture:
		if(param == NULL)
			m_currentFocusTexture.clear();
		else
		    m_currentFocusTexture = *((std::string*)param);
		break;
	case EDP_CurrentEditorMenu:
		if(param == NULL)
			m_currentEditorMenu.clear();
		else
			m_currentEditorMenu = *((std::string*)param);
		break;
	case EDP_SceneModified:
		m_sceneModified = param == NULL?false:*((bool*)param);
		break;
	case Edit_CopiedParticle:
		m_copiedParticle = (crParticleEffect*)param;
		break;
	}
}

void crEditorInfoData::getParam(int i, void*& param)
{
	switch(i) 
	{
	case EDP_:
		break;
	case EDP_SelectType:
		param = &m_selectType;
		break;
	case EDP_TransformNodeMode:
		param = &m_transformNodeMode;
		break;
	case EDP_PanMode:
		param = &m_panMode;
		break;
	case EDP_RotMode:
		param = &m_rotMode;
		break;
	case EDP_ScaleMode:
		param = &m_scaleMode;
		break;
	case EDP_TransformNodeSpeed:
		param = &m_transformNodeSpeed;
		break;
	case EDP_SelectClassName:
		param = &(m_selectClassName[m_selectType]);
		break;
	case EDP_CurrentSelectDrawable:
		param = m_currentSelectDrawable.get();
		break;
	case EDP_CanMove://canmove
		m_canmove = m_selectType>0 && m_selectType<16?0:1;
		param = &m_canmove;
		break;
	case EDP_TerEditMode:
		param = &m_terEditMode;
		break;
	case EDP_TerTools:
		param = &m_terTools;
		break;
	case EDP_CurrentTerTool:
		param = &m_currentTerTool;
		break;
	case EDP_TerToolRadius:
		param = &m_terToolRadius;
		break;
	case EDP_TerToolHeight:
		param = &m_terToolHeight;
		break;
	case EDP_CurrentFocusTexture:
		param = &m_currentFocusTexture;
		break;
	case EDP_CurrentEditorMenu:
		param = &m_currentEditorMenu;
		break;
	case EDP_FreezeProxySet:
		param = &m_freezeProxySet;
		break;
	case EDP_SceneModified:
		param = &m_sceneModified;
		break;
	case Edit_CopiedParticle:
		param = m_copiedParticle.get();
		break;
	}
}