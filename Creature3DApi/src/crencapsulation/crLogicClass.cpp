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
#include <CREncapsulation/crLogicClass.h>
#include <CREncapsulation/crDataClass.h>
#include <CRCore/crDOFTransform.h>
#include <CREncapsulation/crMethodClass2.h>

//#include <CRAI/crAIGroup.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRProducer/crViewer.h>

#include <CRCore/crBrain.h>
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CREncapsulation/crSceneManager.h>
#include <CRCore/crLightSourceManager.h>

#include <CRIOManager/crLoadManager.h>
#include <CRIOManager/crReadFile.h>
#include <CRIOManager/crDatabasePager.h>
#include <CRPhysics/crViewMatterObject.h>

#include <CRGUI/crImageStage.h>
#include <CRGUI/cr2DStageManager.h>
#include <CRProducer/crGUIEventAdapter.h>
#include <CRParticle/crParticleSystemUpdater.h>
#include "rcfg/ConfigScriptOO.h"
#include <rcfg/ItemMap.h>
#include <CRIOManager/crFileNameUtils.h>
#include <CRTerrain/crTerrainTile.h>
#include <CRCore/crShapeDrawable.h>
#include <CREncapsulation/crStartHandler.h>
#include <CRUtil/crOptimizer.h>
#include <CRUtil/crPrepareVisitor.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRIOManager/crLoadManager.h>
#include <sys/stat.h>
#include <stdio.h>
#include <commdlg.h>
using namespace CREncapsulation;
using namespace CRCore;
using namespace CRGUI;
using namespace CRPhysics;
using namespace CRProducer;
using namespace CRParticle;
using namespace CRIOManager;
//using namespace CRAI;
/////////////////////////////////////////
//
//crTransformNodeLogic
//
/////////////////////////////////////////
crTransformNodeLogic::crTransformNodeLogic()
{
}

crTransformNodeLogic::crTransformNodeLogic(const crTransformNodeLogic& handle):
crLogic(handle)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
	if(handle.m_panNodeMathod.valid())
		m_panNodeMathod = handle.m_panNodeMathod->clone();
	if(handle.m_rotNodeMathod.valid())
		m_rotNodeMathod = handle.m_rotNodeMathod->clone();
	if(handle.m_scaleNodeMathod.valid())
		m_scaleNodeMathod = handle.m_scaleNodeMathod->clone();
	if(handle.m_adjustEditorNodeMathod.valid())
		m_adjustEditorNodeMathod = handle.m_adjustEditorNodeMathod->clone();
}

void crTransformNodeLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param) m_param = *(_crInt64*)param;
		else m_param = 0;
		break;
	}
}

void crTransformNodeLogic::addParam(int i, const std::string& str)
{
}

void crTransformNodeLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	case 1:
        m_panNodeMathod = (CRCore::crHandle *)param;
		break;
	case 2:
		m_rotNodeMathod = (CRCore::crHandle *)param;
		break;
	case 3:
		m_scaleNodeMathod = (CRCore::crHandle *)param;
		break;
	case 4:
		m_adjustEditorNodeMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crTransformNodeLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}

void crTransformNodeLogic::operator()(crHandle &handle)
{
	if(!m_node.valid() || !m_convertToTransformMathod.valid()) return;
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;
	if(bindview->isCursorOnShow())
	{
		bool cancelTask = true;
		handle.outputParam(0,&cancelTask);
		return;
	}

	crViewMatterObject *body = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(body)
	{
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);
		crData *infoData = bodyInfo->getDataClass();
		assert(!infoData);

		void *param;
		infoData->getParam(EDP_CanMove,param);
		int canmove = *((int*)param);
		if(!canmove) return;
		infoData->getParam(EDP_FreezeProxySet,param);
		crEditorInfoData::FreezeProxySet* freezeProxySet = (crEditorInfoData::FreezeProxySet*)param;
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			void *_transformNodeMode;
			infoData->getParam(1,_transformNodeMode);
			int transformNodeMode = *((int*)_transformNodeMode);
			if(transformNodeMode>0)
			{
				for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
					 itr != selectNodeVec.end();
					 ++itr )
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					(*itr)->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						if(freezeProxySet->find(proxyNode)!=freezeProxySet->end())
							return;
					}
					if(!dynamic_cast<crMatrixTransform *>(itr->get()))
					{
						m_convertToTransformMathod->inputParam(1,itr->get());
					    (*m_convertToTransformMathod)(*this);
					    m_convertToTransformMathod->inputParam(1,NULL);
						if(m_convertedTransform.valid())
						{
							(*itr)->doEvent(WCH_UNSELECTNODE);
							*itr = dynamic_cast<crNode *>(m_convertedTransform.get());
							(*itr)->doEvent(WCH_SELECTNODE);
							m_convertedTransform = NULL;
						}
					}
				}
			}
			switch (transformNodeMode)
			{
			case 0:
				return;
			case 1:
				if(m_panNodeMathod.valid())
				{
					m_panNodeMathod->inputParam(1,m_node.get());
                    m_panNodeMathod->inputParam(2,&m_param);
					(*m_panNodeMathod)(*this);
					m_panNodeMathod->inputParam(1,NULL);
				}
				break;
			case 2:
				if(m_rotNodeMathod.valid())
				{
					m_rotNodeMathod->inputParam(1,m_node.get());
					m_rotNodeMathod->inputParam(2,&m_param);
					(*m_rotNodeMathod)(*this);
					m_rotNodeMathod->inputParam(1,NULL);
				}
				break;
			case 3:
				if(m_scaleNodeMathod.valid())
				{
					m_scaleNodeMathod->inputParam(1,m_node.get());
					m_scaleNodeMathod->inputParam(2,&m_param);
					(*m_scaleNodeMathod)(*this);
					m_scaleNodeMathod->inputParam(1,NULL);
				}
				break;
			}

			if(transformNodeMode>0 && m_adjustEditorNodeMathod.valid())
			{
				for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
					itr != selectNodeVec.end();
					++itr )
				{
					m_adjustEditorNodeMathod->inputParam(1,itr->get());
					(*m_adjustEditorNodeMathod)(*this);
					m_adjustEditorNodeMathod->inputParam(1,NULL);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crPlaceLoadedNodeLogic
//
/////////////////////////////////////////
crPlaceLoadedNodeLogic::crPlaceLoadedNodeLogic()
{
}

crPlaceLoadedNodeLogic::crPlaceLoadedNodeLogic(const crPlaceLoadedNodeLogic& handle):
crLogic(handle)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
}

void crPlaceLoadedNodeLogic::inputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param == 0)
		{//释放
			m_loadedNode = NULL;
		}
		break;
	case 1:
		m_loadedNode = param?(crNode*)param:NULL;
		break;
	}
}

void crPlaceLoadedNodeLogic::addParam(int i, const std::string& str)
{
}
void crPlaceLoadedNodeLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crPlaceLoadedNodeLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}

void crPlaceLoadedNodeLogic::operator()(crHandle &handle)
{
	if(m_loadedNode.valid())
	{
		CRProducer::crViewer *viewer = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!viewer || !viewer->isInited())
			return;

		crMatrix viewerMat = viewer->getViewMatrix();       
		ref_ptr<crMatrixTransform> matrixTransform = dynamic_cast<crMatrixTransform *>(m_loadedNode.get());
		if(!matrixTransform.valid())
		{
			m_convertToTransformMathod->inputParam(1,m_loadedNode.get());
			(*m_convertToTransformMathod)(*this);
			m_convertToTransformMathod->inputParam(1,NULL);
			if(m_convertedTransform.valid())
			{
				matrixTransform = m_convertedTransform.get();
				m_convertedTransform = NULL;
			}
			//matrixTransform = new crMatrixTransform;
			//matrixTransform->setName("PlaceLoadedNode");
			//m_loadedNode->getParent(0)->replaceChild(m_loadedNode.get(),matrixTransform);
			//matrixTransform->addChild(m_loadedNode.get());
		}
		if(matrixTransform.valid())
		{
			crVector3d eye,center,up;
			viewerMat.getLookAt(eye,center,up);
			crVector3d lv = center - eye;
			lv.normalize();
			matrixTransform->setPosition(center + lv * m_loadedNode->getBound().radius());

			m_loadedNode = matrixTransform.get();
			handle.outputParam(1,m_loadedNode.get());
		}
	}
}
/////////////////////////////////////////
//
//crSpawnSelectNodesLogic
//
/////////////////////////////////////////
crSpawnSelectNodesLogic::crSpawnSelectNodesLogic():
m_mirror(false)
{
}

crSpawnSelectNodesLogic::crSpawnSelectNodesLogic(const crSpawnSelectNodesLogic& handle):
crLogic(handle),
m_mirror(handle.m_mirror)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
	if(handle.m_newLight.valid())
		m_newLight = handle.m_newLight->clone();
	if(handle.m_newParticle.valid())
		m_newParticle = handle.m_newParticle->clone();
	if(handle.m_adjustEditorNodeMathod.valid())
		m_adjustEditorNodeMathod = handle.m_adjustEditorNodeMathod->clone();
	if(handle.m_setEditorCoordinateMathod.valid())
		m_setEditorCoordinateMathod = handle.m_setEditorCoordinateMathod->clone();
}

void crSpawnSelectNodesLogic::inputParam(int i, void *param)
{

}

void crSpawnSelectNodesLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_mirror = (bool)(atoi(str.c_str()));
		break;
	}
}

void crSpawnSelectNodesLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	case 1:
		m_newLight = (CRCore::crHandle *)param;
		break;
	case 2:
		m_newParticle = (CRCore::crHandle *)param;
		break;
	case 3:
		m_adjustEditorNodeMathod = (CRCore::crHandle *)param;
		break;
	case 4:
		m_setEditorCoordinateMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crSpawnSelectNodesLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param) m_newEditorNodeDes = *((std::string *)param);
		else m_newEditorNodeDes.clear();
		break;
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}

void crSpawnSelectNodesLogic::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
	assert(bodyInfo);
	crData *infoData = bodyInfo->getDataClass();
	assert(infoData);
	CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;

	//void *_selectType;
	//infoData->getParam(0,_selectType);
	//int selectType = *((int*)_selectType);
	//if(selectType>0) return;

	crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crGroup *proxyNode;
		crGroup *root;
		ref_ptr<crGroup> spawnNode;
		bool maybeEitorNode = true;
		ref_ptr<crNode> tmpNode;
		ref_ptr<crMatrixTransform> matrixTransform;
		std::string editorNodeType,editorNodeName;

		//crMatrix viewerMat = viewer->getViewMatrix();
		//viewerMat.setTrans(crVector3f(0.0f,0.0f,0.0f));
		crVector3 trans,offset;
		//CRCore::rangef offsetRange(0.5f,2.0f);
		float offsetLength = 1.0f;//offsetRange.get_random();
		
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setSearchNodeType(GROUP);

		bodyInfo->unSelectAll();
		for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
			itr != selectNodeVec.end();
			++itr )
		{
			maybeEitorNode = true;
			searchByNameVisitor.reset();
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setSearchNodeType(GROUP);
			(*itr)->accept(searchByNameVisitor);
			proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(proxyNode)
			{
				root = proxyNode->getParent(0);
				if(root)
				{
					spawnNode = dynamic_cast<crGroup *>(proxyNode->clone(crCopyOp::DEEP_COPY_NODES));
					matrixTransform = dynamic_cast<crMatrixTransform *>(spawnNode->getChild(0));
					if(!matrixTransform.valid())
					{
						tmpNode = spawnNode->getChild(0);

						m_convertToTransformMathod->inputParam(1,tmpNode.get());
						(*m_convertToTransformMathod)(*this);
						m_convertToTransformMathod->inputParam(1,NULL);
						if(m_convertedTransform.valid())
						{
							matrixTransform = m_convertedTransform.get();
							m_convertedTransform = NULL;
						}
						//matrixTransform = new crMatrixTransform;
						//matrixTransform->setName("SpawnMatrix");
						//spawnNode->replaceChild(tmpNode.get(),matrixTransform);
						//matrixTransform->addChild(tmpNode.get());
						maybeEitorNode = false;
					}

					//*itr = matrixTransform;
					ref_ptr<crData> data = matrixTransform->getDataClass();
					if(!data.valid())
					{
						data = crDataManager::getInstance()->getData("Event");
						matrixTransform->setDataClass(data.get());
					}
					if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
					{
						crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
						driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
						data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
						data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
					}
					bodyInfo->selectNode(matrixTransform.get());

					crMatrix mat = matrixTransform->getMatrix();
					if(!m_mirror)
					{
						//offset
						offset.set(0.0f,offsetLength,0.0f);
						//offset = viewerMat * offset;

						crVector3 trans = matrixTransform->getTrans();//matrixTransform->getBound().center();
						crMatrix put_mat = crMatrix::translate(trans);
						crMatrix inv_put = crMatrix::inverse(put_mat);
						mat.postMult(inv_put);
						mat.postMult(crMatrix::translate(offset));
						mat.postMult(put_mat);
					}
					else
					{
						void *_rotNodeMode;
						infoData->getParam(3,_rotNodeMode);
						int rotNodeMode = *((int*)_rotNodeMode);
						crMatrix reflectMatrix;
						switch (rotNodeMode)
						{
						case 0://x
							reflectMatrix = crMatrix::scale(-1.0f,1.0f,1.0f);
							break;
						case 1://y
							reflectMatrix = crMatrix::scale(1.0f,-1.0f,1.0f);
							break;
						case 2://x
							reflectMatrix = crMatrix::scale(1.0f,1.0f,-1.0f);
							break;
						}
						mat.preMult(reflectMatrix);
					}
					matrixTransform->setMatrix(mat);

					if(maybeEitorNode && crArgumentParser::readKeyValue(matrixTransform->getDescriptions(),"EditorNodeType",editorNodeType))
					{
						crArgumentParser::readKeyValue(matrixTransform->getDescriptions(),"EditorNodeName",editorNodeName);
						if(editorNodeType.compare("LightModel") == 0)
						{
							if(m_newLight.valid())
							{
								crLightSource *ls = crLightSourceManager::getInstance()->findLightSource(editorNodeName);
								m_newLight->inputParam(0,ls);
								(*m_newLight)(*this);
								m_newLight->inputParam(0,NULL);
							}
						}
						else if(editorNodeType.compare("ParticleModel") == 0)
						{
							if(m_newParticle.valid())
							{
								crNode *effectNode = crParticleSystemUpdater::getInstance()->findEffectGroup(editorNodeName);
								m_newParticle->inputParam(0,effectNode);
								(*m_newParticle)(*this);
								m_newParticle->inputParam(0,NULL);
							}
						}

						if(!m_newEditorNodeDes.empty())
						{
							crArgumentParser::readKeyValue(m_newEditorNodeDes,"EditorNodeName",editorNodeName);
							std::string des = matrixTransform->getDescriptionsInString();
							crArgumentParser::updateOrAddKeyValue(des,"EditorNodeName",editorNodeName);
							matrixTransform->setDescriptionsInString(des);
							matrixTransform->setName(editorNodeName);
							m_newEditorNodeDes.clear();
						}

						if(m_adjustEditorNodeMathod.valid())
						{
							m_adjustEditorNodeMathod->inputParam(1,itr->get());
							(*m_adjustEditorNodeMathod)(*this);
							m_adjustEditorNodeMathod->inputParam(1,NULL);
						}
					}
					if(m_setEditorCoordinateMathod.valid())
						(*m_setEditorCoordinateMathod)(*this);
			
					CRIOManager::crLoadManager::getInstance()->requestAddNode(root,spawnNode.get(),false);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crCenterModelLogic
//
/////////////////////////////////////////
crCenterModelLogic::crCenterModelLogic()
{
}

crCenterModelLogic::crCenterModelLogic(const crCenterModelLogic& handle):
	crLogic(handle)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
	if(handle.m_setEditorCoordinateMathod.valid())
		m_setEditorCoordinateMathod = handle.m_setEditorCoordinateMathod->clone();
}

void crCenterModelLogic::inputParam(int i, void *param)
{
}

void crCenterModelLogic::addParam(int i, const std::string& str)
{
}
void crCenterModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	case 1:
		m_setEditorCoordinateMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crCenterModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}

void crCenterModelLogic::operator()(crHandle &handle)
{
	crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
	assert(cameraBot);
	crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
	assert(bodyInfo);
	crData *infoData = bodyInfo->getDataClass();
	assert(infoData);
	CRProducer::crViewer *bindview = CRProducer::crKeyboardMouseHandle::getInstance()->getBindViewer();
	if(!bindview || !bindview->isInited())
		return;

	crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
	if(!selectNodeVec.empty())
	{
		crGroup *proxyNode;
		crGroup *root;
		ref_ptr<crNode> tmpNode;
		ref_ptr<crMatrixTransform> matrixTransform;

		crVector3 trans;
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId("ProxyNode");
		searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
		searchByNameVisitor.setSearchNodeType(GROUP);

		bodyInfo->unSelectAll();
		for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
			itr != selectNodeVec.end();
			++itr )
		{
			searchByNameVisitor.reset();
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setSearchNodeType(GROUP);
			(*itr)->accept(searchByNameVisitor);
			proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
			if(proxyNode)
			{
				root = proxyNode->getParent(0);
				if(root)
				{
					matrixTransform = dynamic_cast<crMatrixTransform *>(proxyNode->getChild(0));
					if(!matrixTransform.valid())
					{
						tmpNode = proxyNode->getChild(0);

						m_convertToTransformMathod->inputParam(1,tmpNode.get());
						(*m_convertToTransformMathod)(*this);
						m_convertToTransformMathod->inputParam(1,NULL);
						if(m_convertedTransform.valid())
						{
							matrixTransform = m_convertedTransform.get();
							m_convertedTransform = NULL;
						}
					}
					crMatrix mat = matrixTransform->getMatrix();
					crVector3 center = matrixTransform->getBound().center();
					crVector3 trans = matrixTransform->getTrans();
					crMatrix put_mat = crMatrix::translate(trans);
					crMatrix inv_put = crMatrix::inverse(put_mat);
					mat.postMult(inv_put);
					mat.postMult(crMatrix::translate(-center));
					mat.postMult(put_mat);
					matrixTransform->setMatrix(mat);

					crDisconnectDrawableVisitor ddv;
					proxyNode->accept(ddv);
					CRUtil::crOptimizer::crFreezeTransformsVisitor fztv;
					fztv.setIgnorePhysics(true);
					proxyNode->accept(fztv);
					fztv.freeze();
					CRUtil::crPrepareVisitor prepareVisitor;
					proxyNode->accept(prepareVisitor);
					proxyNode->releaseObjects();

					tmpNode = proxyNode->getChild(0);
					m_convertToTransformMathod->inputParam(1,tmpNode.get());
					(*m_convertToTransformMathod)(*this);
					m_convertToTransformMathod->inputParam(1,NULL);
					if(m_convertedTransform.valid())
					{
						matrixTransform = m_convertedTransform.get();
						m_convertedTransform = NULL;
					}
					matrixTransform->setPosition(center);

					crLoadManager::getInstance()->requestCompile(proxyNode);

					ref_ptr<crData> data = proxyNode->getDataClass();
					if(!data.valid())
					{
						data = crDataManager::getInstance()->getData("Event");
						proxyNode->setDataClass(data.get());
					}
					if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
					{
						crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
						driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
						data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
						data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
					}
					bodyInfo->selectNode(proxyNode);

					if(m_setEditorCoordinateMathod.valid())
						(*m_setEditorCoordinateMathod)(*this);
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crDeleteSelectNodesLogic
//
/////////////////////////////////////////
crDeleteSelectNodesLogic::crDeleteSelectNodesLogic()
{
}

crDeleteSelectNodesLogic::crDeleteSelectNodesLogic(const crDeleteSelectNodesLogic& handle):
crLogic(handle)
{
	if(handle.m_deleteNodeMathod.valid())
		m_deleteNodeMathod = handle.m_deleteNodeMathod->clone();
	if(handle.m_deleteLightSource.valid())
		m_deleteLightSource = handle.m_deleteLightSource->clone();
	if(handle.m_deleteParticle.valid())
		m_deleteParticle = handle.m_deleteParticle->clone();
}

void crDeleteSelectNodesLogic::inputParam(int i, void *param)
{
}

void crDeleteSelectNodesLogic::addParam(int i, const std::string& str)
{
}

void crDeleteSelectNodesLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_deleteNodeMathod = (CRCore::crHandle *)param;
		break;
	case 1:
		m_deleteLightSource = (CRCore::crHandle *)param;
		break;
	case 2:
		m_deleteParticle = (CRCore::crHandle *)param;
		break;
	}
}

void crDeleteSelectNodesLogic::outputParam(int i, void *param)
{
}

void crDeleteSelectNodesLogic::operator()(crHandle &handle)
{
	if(m_deleteNodeMathod.valid())
	{
		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *bodyInfo = cameraBot->getGameBodyInfo();
		assert(bodyInfo);
		crData *infoData = bodyInfo->getDataClass();
		assert(infoData);

		//void *_selectType;
		//infoData->getParam(0,_selectType);
		//int selectType = *((int*)_selectType);
		//if(selectType>0) return;

		crGameBodyInfo::SelectNodeVec selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			crGroup *proxyNode;
			crGroup *root;
			crMatrixTransform *matrixTransform;
			std::string editorNodeType,editorNodeName;
			
			CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
			searchByNameVisitor.setNameId("ProxyNode");
			searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
			searchByNameVisitor.setSearchNodeType(GROUP);

			bodyInfo->unSelectAll();
			for( crGameBodyInfo::SelectNodeVec::iterator itr = selectNodeVec.begin();
				itr != selectNodeVec.end();
				++itr )
			{
				//proxyNode = (*itr)->getParent(0);
				//assert(!proxyNode);
				//root = proxyNode->getParent(0);
				//assert(!root);
				searchByNameVisitor.reset();
				searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
				searchByNameVisitor.setSearchNodeType(GROUP);
				(*itr)->accept(searchByNameVisitor);
				proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
				if(proxyNode)
				{
					root = proxyNode->getParent(0);
					if(root)
					{
						matrixTransform = dynamic_cast<crMatrixTransform *>(proxyNode->getChild(0));
						if(matrixTransform && crArgumentParser::readKeyValue(matrixTransform->getDescriptions(),"EditorNodeType",editorNodeType))
						{
							crArgumentParser::readKeyValue(matrixTransform->getDescriptions(),"EditorNodeName",editorNodeName);
							if(editorNodeType.compare("LightModel") == 0)
							{
								if(m_deleteLightSource.valid())
								{
									m_deleteLightSource->inputParam(0,&editorNodeName);
									(*m_deleteLightSource)(*this);
									m_deleteLightSource->inputParam(0,NULL);
								}
							}
							else if(editorNodeType.compare("ParticleModel") == 0)
							{
								if(m_deleteParticle.valid())
								{
									m_deleteParticle->inputParam(0,&editorNodeName);
									(*m_deleteParticle)(*this);
									m_deleteParticle->inputParam(0,NULL);
								}
							}
						}
						m_deleteNodeMathod->inputParam(1,proxyNode);
						m_deleteNodeMathod->inputParam(3,root);
						(*m_deleteNodeMathod)(*this);
						m_deleteNodeMathod->inputParam(1,NULL);
						m_deleteNodeMathod->inputParam(3,NULL);
					}
				}
			}
		}
	}
}
/////////////////////////////////////////
//
//crLoadModelLogic
//
/////////////////////////////////////////
crLoadModelLogic::crLoadModelLogic():
m_root(NULL)
{
}

crLoadModelLogic::crLoadModelLogic(const crLoadModelLogic& handle):
crLogic(handle),
m_rootName(handle.m_fileExt),
m_filePath(handle.m_filePath),
m_fileExt(handle.m_fileExt),
m_fileName(handle.m_fileName),
m_root(NULL)/*,
m_addDescription(handle.m_addDescription)*/
{
	if(handle.m_addModelMathod.valid())
		m_addModelMathod = handle.m_addModelMathod->clone();
	if(handle.m_placeLoadedNodeMathod.valid())
		m_placeLoadedNodeMathod = handle.m_placeLoadedNodeMathod->clone();
}

void crLoadModelLogic::inputParam(int i, void *param)
{
	switch(i)
	{
	case 3:
		if(param) m_fileName = *((std::string *)param);
		else m_fileName.clear();
		break;
	case 4:
		if(param) m_addDescription = *(std::string *)param;
		else m_addDescription.clear();
		break;
	}
}

void crLoadModelLogic::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_rootName = str;
        break;
	case 1:
		m_filePath = str;
		break;
	case 2:
		m_fileExt = str;
		break;
	case 3:
		m_fileName = str;
		break;
	case 4:
		m_addDescription = str;
		break;
	}
}

void crLoadModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_addModelMathod = (CRCore::crHandle *)param;
		break;
	case 1:
		m_placeLoadedNodeMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crLoadModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadedNode = param?(CRCore::crNode *)param:NULL;
		break;
	}
}

void crLoadModelLogic::operator()(crHandle &handle)
{
	if(/*!m_fileName.empty() &&*/ m_addModelMathod.valid())
	{
		CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
		searchByNameVisitor.setNameId(m_rootName);
		searchByNameVisitor.setSearchNodeType(GROUP);
		CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
		m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

		if(m_root)
		{
			std::string datafile;
			if(!m_fileName.empty()) 
			{
				if(m_fileExt.empty())
				{
                    datafile = m_filePath + m_fileName;
				}
				else
				{
					datafile = m_filePath + m_fileName + "." + m_fileExt;
				}
			    m_addModelMathod->inputParam(3,&datafile);
			}
			m_loadedNode = NULL;
			m_addModelMathod->inputParam(1,m_root);
			m_addModelMathod->inputParam(4,&m_addDescription);
			(*m_addModelMathod)(*this);
			m_addModelMathod->inputParam(1,NULL);
			m_addModelMathod->inputParam(3,NULL);
		    m_addModelMathod->inputParam(4,NULL);

			if(m_loadedNode.valid() && m_placeLoadedNodeMathod.valid())
			{
				m_placeLoadedNodeMathod->inputParam(1,m_loadedNode.get());
				(*m_placeLoadedNodeMathod)(*this);
				m_placeLoadedNodeMathod->inputParam(1,NULL);
			}

			handle.outputParam(1,m_loadedNode.get());
			m_loadedNode = NULL;
		}
	}
}

/////////////////////////////////////////
//
//crGetListBoxStrLogic
//
/////////////////////////////////////////
crGetListBoxStrLogic::crGetListBoxStrLogic()
{
}

crGetListBoxStrLogic::crGetListBoxStrLogic(const crGetListBoxStrLogic& handle):
crLogic(handle),
m_listBoxNameID(handle.m_listBoxNameID),
m_headStr(handle.m_headStr)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crGetListBoxStrLogic::inputParam(int i, void *param)
{
	if(i==0)
		m_element = (CRGUI::crElement*)param;
}

void crGetListBoxStrLogic::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_listBoxNameID = str;
		break;
	case 1:
		m_headStr = str;
		break;
	}
}

void crGetListBoxStrLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crGetListBoxStrLogic::outputParam(int i, void *param)
{
}

void crGetListBoxStrLogic::operator()(crHandle &handle)
{
	if(m_element.valid() && m_handle.valid())
	{
		crImageStage *stage = m_element->getParentStage();
		if(!stage) return;
		crListBoxElement *modelList = dynamic_cast<crListBoxElement *>(stage->getElement(m_listBoxNameID));
		if(modelList && modelList->getDataSize()>0)
		{
			std::string listBoxStr;
			modelList->getData(modelList->getSelect(),listBoxStr);
			//std::string datafile = m_filePath + file + "." + m_fileExt;
			std::string fileName = m_headStr + listBoxStr;
			m_handle->inputParam(3,&fileName);
			(*m_handle)(*this);
			m_handle->inputParam(3,NULL);
		}
	}
}
/////////////////////////////////////////
//
//crNewEditorModelLogic
//
/////////////////////////////////////////
crNewEditorModelLogic::crNewEditorModelLogic()
{
}

crNewEditorModelLogic::crNewEditorModelLogic(const crNewEditorModelLogic& handle):
crLogic(handle)
{
	if(handle.m_newEditorNode.valid())
		m_newEditorNode = handle.m_newEditorNode->clone();
	if(handle.m_loadModel.valid())
		m_loadModel = handle.m_loadModel->clone();
	if(handle.m_adjustEditorNodeMathod.valid())
		m_adjustEditorNodeMathod = handle.m_adjustEditorNodeMathod->clone();
	if(handle.m_setEditorCoordinateMathod.valid())
		m_setEditorCoordinateMathod = handle.m_setEditorCoordinateMathod->clone();
}

void crNewEditorModelLogic::inputParam(int i, void *param)
{
}

void crNewEditorModelLogic::addParam(int i, const std::string& str)
{
}

void crNewEditorModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_newEditorNode = (CRCore::crHandle *)param;
		break;
	case 1://logic
		m_loadModel = (CRCore::crHandle *)param;
		break;
	case 2:
		m_adjustEditorNodeMathod = (CRCore::crHandle *)param;
		break;
	case 3:
		m_setEditorCoordinateMathod = (CRCore::crHandle *)param;
		break;
	}
}

void crNewEditorModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		if(param) m_newEditorNodeDes = *((std::string *)param);
		else m_newEditorNodeDes.clear();
		break;
	case 1:
		m_loadedNode = param?(crNode*)param:NULL;
		break;
	case 2:
		m_effectNode = param?(crNode*)param:NULL;
		break;
	}
}

void crNewEditorModelLogic::operator()(crHandle &handle)
{
	if(m_newEditorNode.valid() && m_loadModel.valid())
	{
		CRProducer::crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
		if(!bindview || !bindview->isInited())
			return;

		(*m_newEditorNode)(*this);
        
		m_loadModel->inputParam(4,NULL);
		(*m_loadModel)(*this);

		if(!m_loadedNode.valid()) return;

		crViewMatterObject *cameraBot = crSceneManager::getInstance()->getPlayer();
		assert(cameraBot);
		crGameBodyInfo *gamebodyInfo = cameraBot->getGameBodyInfo();
		assert(gamebodyInfo);
		crData *infoData = gamebodyInfo->getDataClass();
		assert(infoData);

		int select = 0;
		infoData->inputParam(0,&select);
		gamebodyInfo->unSelectAll();
		ref_ptr<crData> data = m_loadedNode->getDataClass();
		if(!data.valid())
		{
			data = crDataManager::getInstance()->getData("Event");
			m_loadedNode->setDataClass(data.get());
		}
		if(!data->getHandle(MAKEINT64(WCH_SELECTNODE,NULL)))
		{
			crHandle *driver = crHandleManager::getInstance()->getHandle("OneSlot");
			driver->inputHandle(0,crHandleManager::getInstance()->getHandle("SelectNodeMaterial"));
			data->insertHandle(MAKEINT64(WCH_SELECTNODE,NULL),driver);
			data->insertHandle(MAKEINT64(WCH_UNSELECTNODE,NULL),driver);
		}
		gamebodyInfo->selectNode(m_loadedNode.get());
		
		std::string editorNodeName;
		crArgumentParser::readKeyValue(m_newEditorNodeDes,"EditorNodeName",editorNodeName);
        m_loadedNode->setName(editorNodeName);
		m_loadedNode->setDescriptionsInString(m_newEditorNodeDes);
		m_newEditorNodeDes.clear();

		if(m_adjustEditorNodeMathod.valid())
		{
			m_adjustEditorNodeMathod->inputParam(1,m_loadedNode.get());
			m_adjustEditorNodeMathod->inputParam(2,m_effectNode.get());
			(*m_adjustEditorNodeMathod)(*this);
			m_adjustEditorNodeMathod->inputParam(1,NULL);
			m_adjustEditorNodeMathod->inputParam(2,NULL);
		}
		if(m_setEditorCoordinateMathod.valid())
			(*m_setEditorCoordinateMathod)(*this);
		m_loadedNode = NULL;
	}
}
/////////////////////////////////////////
//
//crFocusSelectNodeLogic
//
/////////////////////////////////////////
crFocusSelectNodeLogic::crFocusSelectNodeLogic()
{
}

crFocusSelectNodeLogic::crFocusSelectNodeLogic(const crFocusSelectNodeLogic& handle):
crLogic(handle)
{
	if(handle.m_focusNode.valid())
		m_focusNode = handle.m_focusNode->clone();
}

void crFocusSelectNodeLogic::inputParam(int i, void *param)
{
}

void crFocusSelectNodeLogic::addParam(int i, const std::string& str)
{
}

void crFocusSelectNodeLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_focusNode = (CRCore::crHandle *)param;
		break;
	}
}

void crFocusSelectNodeLogic::outputParam(int i, void *param)
{
}

void crFocusSelectNodeLogic::operator()(crHandle &handle)
{
	if(m_focusNode.valid())
	{
		crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
		assert(!body);
		crGameBodyInfo *bodyInfo = body->getGameBodyInfo();
		assert(!bodyInfo);
		crGameBodyInfo::SelectNodeVec& selectNodeVec = bodyInfo->getSelectNodeVec();
		if(!selectNodeVec.empty())
		{
			m_focusNode->inputParam(1,selectNodeVec[0].get());
			(*m_focusNode)(*this);
			m_focusNode->inputParam(1,NULL);
		}
	}
}

/////////////////////////////////////////
//
//crInitLightEditorModelLogic
//
/////////////////////////////////////////
crInitLightEditorModelLogic::crInitLightEditorModelLogic():
m_root(NULL)
{
}

crInitLightEditorModelLogic::crInitLightEditorModelLogic(const crInitLightEditorModelLogic& handle):
crLogic(handle),
m_rootName(handle.m_rootName),
m_root(NULL)
{
	if(handle.m_loadModel.valid())
		m_loadModel = handle.m_loadModel->clone();
}

void crInitLightEditorModelLogic::inputParam(int i, void *param)
{
}

void crInitLightEditorModelLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rootName = str;
		break;
	}
}

void crInitLightEditorModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0://logic
		m_loadModel = (CRCore::crHandle *)param;
		break;
	}
}

void crInitLightEditorModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadedNode = (crNode*)param;
		break;
	}
}

void crInitLightEditorModelLogic::operator()(crHandle &handle)
{
    CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_root)
	{
		crLightSourceManager *manager = crLightSourceManager::getInstance();
		crLightSourceManager::LSVec &lsVec = manager->getLSVec();
		crLightSource *ls;
		crMatrixTransform* matrixTransform;
		std::string des;
		crVector3 dir;
		crVector4 position;
		crLight *light;
		for( crLightSourceManager::LSVec::iterator itr = lsVec.begin();
			itr != lsVec.end();
			++itr )
		{
			ls = itr->get();
			if(!ls->getLightIsSunLight())
			{
				searchByNameVisitor.reset();
				searchByNameVisitor.setNameId(ls->getName());
				searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
				m_root->accept(searchByNameVisitor);
				matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
				if(!matrixTransform)
				{
					m_loadModel->inputParam(4,NULL);
					(*m_loadModel)(*this);

					matrixTransform = dynamic_cast<crMatrixTransform *>(m_loadedNode.get());
					if(!matrixTransform)
					{
						matrixTransform = new crMatrixTransform;
						m_loadedNode->getParent(0)->replaceChild(m_loadedNode.get(),matrixTransform);
						matrixTransform->addChild(m_loadedNode.get());
					}

					des = "EditorNodeType:LightModel\nEditorNodeName:"+ls->getName();
					matrixTransform->setName(ls->getName());
					matrixTransform->setDescriptionsInString(des);

					light = dynamic_cast<crLight *>(ls->getLight());
					dir = light->getDirection();
					position = light->getPosition();
					crMatrix mat = CRCore::crMatrix::rotate(CRCore::Y_AXIS,dir);
					mat.setTrans(crVector3(position[0],position[1],position[2]));
					matrixTransform->setMatrix(mat);

		            m_loadedNode = NULL;
				}
			}
		}
	}
}

/////////////////////////////////////////
//
//crInitParticleEditorModelLogic
//
/////////////////////////////////////////
crInitParticleEditorModelLogic::crInitParticleEditorModelLogic():
m_root(NULL)
{
}

crInitParticleEditorModelLogic::crInitParticleEditorModelLogic(const crInitParticleEditorModelLogic& handle):
crLogic(handle),
m_rootName(handle.m_rootName),
m_root(NULL)
{
	if(handle.m_loadModel.valid())
		m_loadModel = handle.m_loadModel->clone();
}

void crInitParticleEditorModelLogic::inputParam(int i, void *param)
{
}

void crInitParticleEditorModelLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_rootName = str;
		break;
	}
}

void crInitParticleEditorModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0://logic
		m_loadModel = (CRCore::crHandle *)param;
		break;
	}
}

void crInitParticleEditorModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_loadedNode = (crNode*)param;
		break;
	}
}

void crInitParticleEditorModelLogic::operator()(crHandle &handle)
{
	CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
	searchByNameVisitor.setNameId(m_rootName);
	searchByNameVisitor.setSearchNodeType(GROUP);
	CRCore::crBrain::getInstance()->accept(searchByNameVisitor);
	m_root = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());

	if(m_root)
	{
		crParticleSystemUpdater::getInstance()->lock();
		std::set<crGroup *> effectGroupSet;
		crParticleSystemUpdater::ParticleEffectSet &effectSet = crParticleSystemUpdater::getInstance()->getParticleEffectSet();
		CRCore::ref_ptr<crParticleEffect> setItr;
		for( crParticleSystemUpdater::ParticleEffectSet::iterator itr = effectSet.begin();
			itr != effectSet.end();
			++itr )
		{
			setItr = *itr;
			effectGroupSet.insert(setItr->getParent(0));
		}
		crParticleSystemUpdater::getInstance()->unlock();

		crMatrixTransform *effectGroup;
		crMatrixTransform* matrixTransform;
		std::string des;
		for( std::set<crGroup *>::iterator itr = effectGroupSet.begin();
			itr != effectGroupSet.end();
			++itr )
		{
			effectGroup = dynamic_cast<crMatrixTransform *>(*itr);
			if(!effectGroup) continue;

			searchByNameVisitor.reset();
			searchByNameVisitor.setNameId(effectGroup->getName());
			searchByNameVisitor.setSearchNodeType(MATRIXTRANSFORM);
			m_root->accept(searchByNameVisitor);
			matrixTransform = dynamic_cast<crMatrixTransform *>(searchByNameVisitor.getResult());
			if(!matrixTransform)
			{
				m_loadModel->inputParam(4,NULL);
				(*m_loadModel)(*this);

				matrixTransform = dynamic_cast<crMatrixTransform *>(m_loadedNode.get());
				if(!matrixTransform)
				{
					matrixTransform = new crMatrixTransform;
					m_loadedNode->getParent(0)->replaceChild(m_loadedNode.get(),matrixTransform);
					matrixTransform->addChild(m_loadedNode.get());
				}

				des = "EditorNodeType:ParticleModel\nEditorNodeName:"+effectGroup->getName();
				matrixTransform->setName(effectGroup->getName());
				matrixTransform->setDescriptionsInString(des);
				matrixTransform->setMatrix(effectGroup->getMatrix());
				m_loadedNode = NULL;
			}
		}
	}
}

/////////////////////////////////////////
//
//crGetModelFileNameLogic
//
/////////////////////////////////////////
crGetModelFileNameLogic::crGetModelFileNameLogic()
{
	m_ext = "cre";
}

crGetModelFileNameLogic::crGetModelFileNameLogic(const crGetModelFileNameLogic& handle):
crLogic(handle),
m_filePath(handle.m_filePath),
m_ext(handle.m_ext)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crGetModelFileNameLogic::inputParam(int i, void *param)
{
}

void crGetModelFileNameLogic::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_filePath = str;
		break;
	case 1:
		m_ext = str;
		break;
	}
}

void crGetModelFileNameLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crGetModelFileNameLogic::outputParam(int i, void *param)
{
}

void crGetModelFileNameLogic::operator()(crHandle &handle)
{
	if(m_handle.valid())
	{
		char programDir[256];
		GetCurrentDirectory(256,programDir);
		//std::string dir = programDir;
		//dir = dir + "/" + m_filePath;
		//SetCurrentDirectory(dir.c_str());

		OPENFILENAME ofn;       // common dialog box structure
		char szFile[256];       // buffer for file name

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ZeroMemory(szFile, sizeof(szFile));
		//sprintf(szFile,"%s\0",m_filePath.c_str());
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = crStartHandler::getInstance()->getWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if(m_ext.compare("rbody") == 0)
			ofn.lpstrFilter = "rbody(.rbody)\0*.rbody\0All(*.*)\0*.*\0";
		else if(m_ext.compare("cru") == 0)
			ofn.lpstrFilter = "cru(.cru)\0*.cru\0All(*.*)\0*.*\0";
		else
            ofn.lpstrFilter = "cre(.cre)\0*.cre\0All(*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = m_filePath.c_str();
		ofn.Flags = OFN_EXPLORER;

		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn)==TRUE) 
		{
			SetCurrentDirectory(programDir);
			std::string filename = ofn.lpstrFile;
			filename.erase(0,strlen(programDir)+1);
			filename = CRIOManager::convertFileNameToUnixStyle(filename);
			std::string ext = CRIOManager::getFileExtension(filename);
			if(ext.empty())
				filename = filename + "." + m_ext;
			//filename = crArgumentParser::getFileNameEliminateExt(filename);
			m_handle->inputParam(3,&filename);
			(*m_handle)(*this);
			m_handle->inputParam(3,NULL);
		}
		SetCurrentDirectory(programDir);
	}
}

/////////////////////////////////////////
//
//crModifyDrawableMaterialLogic
//
/////////////////////////////////////////
crModifyDrawableMaterialLogic::crModifyDrawableMaterialLogic()
{
}

crModifyDrawableMaterialLogic::crModifyDrawableMaterialLogic(const crModifyDrawableMaterialLogic& handle):
crLogic(handle)
{
	if(handle.m_modifyMaterial.valid())
		m_modifyMaterial = handle.m_modifyMaterial->clone();
	if(handle.m_modifyObjectDrawable.valid())
		m_modifyObjectDrawable = handle.m_modifyObjectDrawable->clone();
}

void crModifyDrawableMaterialLogic::inputParam(int i, void *param)
{
	if(i==0)
	{
		if(m_modifyMaterial.valid())
			m_modifyMaterial->inputParam(0,param);
	}
}

void crModifyDrawableMaterialLogic::addParam(int i, const std::string& str)
{
}

void crModifyDrawableMaterialLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_modifyMaterial = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_modifyObjectDrawable = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crModifyDrawableMaterialLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_currentSelectObject = param?(crObject*)param:NULL;
		break;
	case 1:
		m_currentSelectDrawable = param?(crDrawable*)param:NULL;
		break;
	}
}

void crModifyDrawableMaterialLogic::operator()(crHandle &handle)
{
	//try
	//{
	if(m_modifyMaterial.valid() && m_modifyObjectDrawable.valid())
	{
        (*m_modifyMaterial)(*this);
        m_modifyObjectDrawable->inputParam(0,m_currentSelectObject.get());
		m_modifyObjectDrawable->inputParam(1,m_currentSelectDrawable.get());
		(*m_modifyObjectDrawable)(*this);
		m_modifyObjectDrawable->inputParam(0,NULL);
		m_modifyObjectDrawable->inputParam(1,NULL);

		m_currentSelectObject = NULL;
		m_currentSelectDrawable = NULL;
	}
	//}
	//catch (...)
	//{
 //       CRCore::notify(CRCore::ALWAYS)<<"crModifyDrawableMaterialLogic error"<<std::endl;
	//}
}

/////////////////////////////////////////
//
//crProxyElementLogic
//
/////////////////////////////////////////
crProxyElementLogic::crProxyElementLogic()
{
}

crProxyElementLogic::crProxyElementLogic(const crProxyElementLogic& handle):
crLogic(handle),
m_consignerElement(handle.m_consignerElement)
{
	if(handle.m_handle.valid())
		m_handle = handle.m_handle->clone();
}

void crProxyElementLogic::inputParam(int i, void *param)
{
	if(m_handle.valid())
	{
		if(i==0)
		{
			m_proxyElement = param?(CRGUI::crElement*)param:NULL;
			if(m_proxyElement.valid())
			{
				crImageStage *stage = m_proxyElement->getParentStage();
				assert(stage);
				crElement *consignerElement = dynamic_cast<crElement *>(stage->getElement(m_consignerElement));
				if(consignerElement)
					m_handle->inputParam(i,consignerElement);
			}
		}
		else
		{
			m_handle->inputParam(i,param);
		}
	}
}

void crProxyElementLogic::addParam(int i, const std::string& str)
{
	switch(i)
	{
	case 0:
		m_consignerElement = str;
		break;
	}
}

void crProxyElementLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_handle = (CRCore::crHandle *)param;
		break;
	}
}

void crProxyElementLogic::outputParam(int i, void *param)
{
	if(m_handle.valid())
	{
		m_handle->outputParam(i,param);
	}
}

void crProxyElementLogic::operator()(crHandle &handle)
{
	if(m_handle.valid())
	{
		(*m_handle)(*this);
	}
}
/////////////////////////////////////////
//
//crMousePickMoveLogic
//
/////////////////////////////////////////
crMousePickMoveLogic::crMousePickMoveLogic()
{
}

crMousePickMoveLogic::crMousePickMoveLogic(const crMousePickMoveLogic& handle):
crLogic(handle)
{
	if(handle.m_pickTarget.valid())
		m_pickTarget = handle.m_pickTarget->clone();
	if(handle.m_moveToTarget.valid())
		m_moveToTarget = handle.m_moveToTarget->clone();
	if(handle.m_talkWithBot.valid())
		m_talkWithBot = handle.m_talkWithBot->clone();
}

void crMousePickMoveLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
			m_ea = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		if(param)
		{
			_crInt64 param64 = *(_crInt64*)param;
			m_ea = (crGUIEventAdapter *)(LOINT64(param64));
		}
		else
		{
			m_ea = NULL;
		}
		break;
	}
	if(m_pickTarget.valid())
		m_pickTarget->inputParam(i,param);
	if(m_moveToTarget.valid())
		m_moveToTarget->inputParam(i,param);
	if(m_talkWithBot.valid())
		m_talkWithBot->inputParam(i,param);
}

void crMousePickMoveLogic::addParam(int i, const std::string& str)
{
}

void crMousePickMoveLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_pickTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_moveToTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	case 2:
		m_talkWithBot = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crMousePickMoveLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 3:
		m_targetType = param?*((int*)param):0;
		break;
	case 4:
		m_targetNode = param?(crViewMatterObject *)(param):NULL;
		break;
	case 5:
		m_targerPosition = param?*((crVector3*)param):crVector3(0.0f,0.0f,0.0f);
		break;
	}
}

void crMousePickMoveLogic::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	if(!bot || !m_ea.valid()) return;
	crData *data = bot->getDataClass();
	if(!bot->isDead())
	{
        m_targetType = 0;
        m_targetNode = NULL;
		if(m_pickTarget.valid()) (*m_pickTarget)(*this);
		if(data) 
		{
			data->inputParam(4,&m_targetType);
			data->inputParam(5,m_targetNode.get());
			data->inputParam(6,&m_targerPosition);
		}
		if(m_targetType == 1)
		{
			if(bot == crSceneManager::getInstance()->getPlayer() && m_talkWithBot.valid())
			{
				if(bot->checkTeam(m_targetNode->getTeam())>=0)
				{
					data = m_targetNode->getDataClass();
					void *_hasTalkEvent;
					data->getParam(2,_hasTalkEvent);
					bool hasTalkEvent = *((bool*)_hasTalkEvent);
					if(hasTalkEvent)
					{
						void *_talkDistance;
						data->getParam(3,_talkDistance);
						float talkDistance = *((float*)_talkDistance);
						float distance = (m_targetNode->getTrans() - bot->getTrans()).length();
						if(distance<talkDistance)
						{
							m_talkWithBot->inputParam(3,m_targetNode.get());
							(*m_talkWithBot)(*this);
							m_talkWithBot->inputParam(3,NULL);
							return;
						}
					}
				}
			}
		}
		else if(m_targetType == 2)
		{
			if(!(m_ea->getModKeyMask() & crGUIEventAdapter::MODKEY_CTRL))
			{
				if(m_moveToTarget.valid()) 
				{
					m_moveToTarget->inputParam(3,&m_targerPosition);
					(*m_moveToTarget)(*this);
				}
			}
			else
			{
				m_targetType = 0;
			    data->inputParam(4,&m_targetType);
			}
		}
	}
	else
	{
		m_targetType = 0;
        m_targetNode = NULL;
		data->inputParam(4,&m_targetType);
		data->inputParam(5,m_targetNode.get());
	}
}

/////////////////////////////////////////
//
//crRpgFireAndFollowLogic
//
/////////////////////////////////////////
crRpgFireAndFollowLogic::crRpgFireAndFollowLogic()
{
}

crRpgFireAndFollowLogic::crRpgFireAndFollowLogic(const crRpgFireAndFollowLogic& handle):
crLogic(handle)
{
	if(handle.m_turnToTarget.valid())
	    m_turnToTarget = handle.m_turnToTarget->clone();
	if(handle.m_moveToTarget.valid())
		m_moveToTarget = handle.m_moveToTarget->clone();
}

void crRpgFireAndFollowLogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	case 2:
		m_param = param?*(_crInt64*)param:NULL;
		break;
	}
	if(m_turnToTarget.valid())
		m_turnToTarget->inputParam(i,param);
	if(m_moveToTarget.valid())
		m_moveToTarget->inputParam(i,param);
}

void crRpgFireAndFollowLogic::addParam(int i, const std::string& str)
{
}

void crRpgFireAndFollowLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_turnToTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_moveToTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crRpgFireAndFollowLogic::outputParam(int i, void *param)
{
}

void crRpgFireAndFollowLogic::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
    crData *data = bot->getDataClass();
	if(bot->isDead())
	{
		int targetType = 0;
		m_targetNode = NULL;
		data->inputParam(4,&targetType);
		data->inputParam(5,m_targetNode.get());
		return;
	}
	void *_targetNode;
	data->getParam(5,_targetNode);
	if(_targetNode!=NULL)
	{
        m_targetNode = (crViewMatterObject*)_targetNode;
		if(!m_targetNode->isDead())
		{
			m_targerPosition = m_targetNode->getTrans();
			int teamflg = bot->checkTeam(m_targetNode->getTeam());
            if(teamflg<0)
			{//敌人
				//如果是左键点击敌人，则移动到攻击范围内攻击敌人
				if(bot->getCurrentWeaponMatterObject())
				{
					float attackDistance = bot->getCurrentWeaponMatterObject()->getWeaponObject()->getAttackDistance();
					//距离判断
					crVector3f myPos = bot->getTrans();
					float dis = (m_targerPosition - myPos).length();
					if(dis<attackDistance)
					{
						unsigned int currentWeaponMask = bot->getCurrentWeaponMask();
						unsigned int targetMask = m_targetNode->getAttributeMask();
						if(currentWeaponMask & targetMask)
						{//可以攻击
							//turnToTarget
							if(m_turnToTarget.valid())
							{
								m_turnToTarget->inputParam(3,&m_targerPosition);
								(*m_turnToTarget)(*this);
							}

							//if(m_fireAtTarget.valid()) (*m_fireAtTarget)(*this);
							CRPhysics::crMatterGroup *bodyMG = dynamic_cast<CRPhysics::crMatterGroup *>(bot->getParent(0));
							bodyMG->acceptEventMessage(WCH_WEAPONEVENT,NULL,MAKEINT64(WCH_WEAPON_FIRE_T,m_targetNode.get()));

							crRpgCharacterUpdateMethod *characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
							characterUpdateMethod->wantChangeUpState(crRpgCharacterUpdateMethod::US_WeaponFire,0.1);
							//unsigned int currentDS = characterUpdateMethod->getCurrentDownState();
							//if( currentDS == crRpgCharacterUpdateMethod::DS_Run ||
							//	currentDS == crRpgCharacterUpdateMethod::DS_Walk ||
							//	currentDS == crRpgCharacterUpdateMethod::DS_Sprint )
							characterUpdateMethod->wantChangeDownState(crRpgCharacterUpdateMethod::DS_Stand,0.1);
						}
						else
						{//invalidTarget，提示不能攻击
						}
					}
					else
					{//移动到攻击距离范围内
						//crVector3f dir = m_targerPosition - myPos;
						//dir.normalize();
						//m_targerPosition = myPos + dir * attackDistance;
						if(m_turnToTarget.valid())
						{
							m_turnToTarget->inputParam(3,&m_targerPosition);
							(*m_turnToTarget)(*this);
						}
						if(m_moveToTarget.valid())
						{
							m_moveToTarget->inputParam(3,&m_targerPosition);
							(*m_moveToTarget)(*this);
						}
					}
				}
			}
			else
			{//中立或者队友，跟随队友
				if(m_moveToTarget.valid()) 
				{
					crVector3f myPos = bot->getTrans();
					float dis = (m_targerPosition - myPos).length();
					crVector3f dir = m_targerPosition - myPos;
					dir.normalize();
					m_targerPosition = myPos + (dir * (dis-2.0f));

					if(m_turnToTarget.valid())
					{
						m_turnToTarget->inputParam(3,&m_targerPosition);
						(*m_turnToTarget)(*this);
					}

					m_moveToTarget->inputParam(3,&m_targerPosition);
					(*m_moveToTarget)(*this);
				}
			}
		}
		else
		{
			int targetType = 0;
			m_targetNode = NULL;
			data->inputParam(4,&targetType);
			data->inputParam(5,m_targetNode.get());
		}
	}
}
/////////////////////////////////////////
//
//crBotAILogic
//
/////////////////////////////////////////
crBotAILogic::crBotAILogic():
m_avoidCollide(false)
{
}

crBotAILogic::crBotAILogic(const crBotAILogic& handle):
crLogic(handle),
m_avoidCollide(false)
{
	if(handle.m_botAISelectTarget.valid())
		m_botAISelectTarget = handle.m_botAISelectTarget->clone();
	if(handle.m_botAIPatrol.valid())
		m_botAIPatrol = handle.m_botAIPatrol->clone();
	if(handle.m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow = handle.m_rpgFireAndFollow->clone();
	if(handle.m_botAIAvoidCollide.valid())
		m_botAIAvoidCollide = handle.m_botAIAvoidCollide->clone();
}

void crBotAILogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
	if(m_botAISelectTarget.valid())
		m_botAISelectTarget->inputParam(i,param);
	if(m_botAIPatrol.valid())
		m_botAIPatrol->inputParam(i,param);
	if(m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow->inputParam(i,param);
	if(m_botAIAvoidCollide.valid())
		m_botAIAvoidCollide->inputParam(i,param);
}

void crBotAILogic::addParam(int i, const std::string& str)
{
}

void crBotAILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_botAISelectTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_botAIPatrol = param?(CRCore::crHandle *)param:NULL;
		break;
	case 2:
		m_botAIAvoidCollide = param?(CRCore::crHandle *)param:NULL;
		break;
	case 3:
		m_rpgFireAndFollow = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crBotAILogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_avoidCollide = param?*((bool*)param):0;
		break;
	}
}

void crBotAILogic::operator()(crHandle &handle)
{
	if(m_botAISelectTarget.valid())
	{
		crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
		crData *data = bot->getDataClass();
		void *_gameState;
        data->getParam(1,_gameState);
		int gameState = *((int*)_gameState);
		if(gameState==1)
		{//in talk
			crRpgCharacterUpdateMethod *characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
			characterUpdateMethod->wantChangeUpState(crRpgCharacterUpdateMethod::US_None,0.1);
			characterUpdateMethod->wantChangeDownState(crRpgCharacterUpdateMethod::DS_Stand,0.1);//DS_Talk
		}
		else
		{
			void *_targetNode;
			(*m_botAISelectTarget)(*this);
			data->getParam(5,_targetNode);
			if(_targetNode!=NULL)
			{//加入NPC是否出了巡逻区域的判断
				(*m_rpgFireAndFollow)(*this);
			}
			else
			{//巡逻
				(*m_botAIAvoidCollide)(*this);
				if(!m_avoidCollide) (*m_botAIPatrol)(*this);
			}
		}
	}
}
/////////////////////////////////////////
//
//crPlayerAILogic
//
/////////////////////////////////////////
crPlayerAILogic::crPlayerAILogic()
{
}

crPlayerAILogic::crPlayerAILogic(const crPlayerAILogic& handle):
crLogic(handle)
{
	if(handle.m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow = handle.m_rpgFireAndFollow->clone();
}

void crPlayerAILogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
	if(m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow->inputParam(i,param);
}

void crPlayerAILogic::addParam(int i, const std::string& str)
{
}

void crPlayerAILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_rpgFireAndFollow = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crPlayerAILogic::outputParam(int i, void *param)
{
}

void crPlayerAILogic::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	crData *data = bot->getDataClass();
	void *_gameState;
	data->getParam(1,_gameState);
	int gameState = *((int*)_gameState);
	if(gameState==1)
	{//in talk
		crRpgCharacterUpdateMethod *characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
		characterUpdateMethod->wantChangeUpState(crRpgCharacterUpdateMethod::US_None,0.1);
		characterUpdateMethod->wantChangeDownState(crRpgCharacterUpdateMethod::DS_Stand,0.1);//DS_Talk
	}
	else if(m_rpgFireAndFollow.valid())
	{
		(*m_rpgFireAndFollow)(*this);
	}
}
/////////////////////////////////////////
//
//crNetPlayerAILogic
//
/////////////////////////////////////////
crNetPlayerAILogic::crNetPlayerAILogic()
{
}

crNetPlayerAILogic::crNetPlayerAILogic(const crNetPlayerAILogic& handle):
crLogic(handle)
{
	if(handle.m_moveToTarget.valid())
		m_moveToTarget = handle.m_moveToTarget->clone();
	if(handle.m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow = handle.m_rpgFireAndFollow->clone();
}

void crNetPlayerAILogic::inputParam(int i, void *param)
{
	switch(i) 
	{
	case 0:
		if(param == 0)
		{//释放
			m_node = NULL;
		}
		break;
	case 1:
		m_node = (crNode*)param;
		break;
	}
	if(m_moveToTarget.valid())
		m_moveToTarget->inputParam(i,param);
	if(m_rpgFireAndFollow.valid())
		m_rpgFireAndFollow->inputParam(i,param);
}

void crNetPlayerAILogic::addParam(int i, const std::string& str)
{
}

void crNetPlayerAILogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_moveToTarget = param?(CRCore::crHandle *)param:NULL;
		break;
	case 1:
		m_rpgFireAndFollow = param?(CRCore::crHandle *)param:NULL;
		break;
	}
}

void crNetPlayerAILogic::outputParam(int i, void *param)
{
}

void crNetPlayerAILogic::operator()(crHandle &handle)
{
	crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(m_node.get());
	crData *data = bot->getDataClass();
	void *_gameState;
	data->getParam(1,_gameState);
	int gameState = *((int*)_gameState);
	if(gameState==1)
	{//in talk
		crRpgCharacterUpdateMethod *characterUpdateMethod = dynamic_cast<crRpgCharacterUpdateMethod *>(m_node->getHandle(WCH_UPDATEVISITOR,"RpgCharacterUpdate"));
		characterUpdateMethod->wantChangeUpState(crRpgCharacterUpdateMethod::US_None,0.1);
		characterUpdateMethod->wantChangeDownState(crRpgCharacterUpdateMethod::DS_Stand,0.1);//DS_Talk
	}
	else
	{
		void *_targetType;
		data->getParam(4,_targetType);
		int targetType = *((int*)_targetType);
		if(targetType == 1)
		{
			if(m_rpgFireAndFollow.valid())
			{
				(*m_rpgFireAndFollow)(*this);
			}
		}
		else if(targetType == 2)
		{
			if(m_moveToTarget.valid())
			{
				void *_targerPosition;
				data->getParam(6,_targerPosition);
				crVector3 targerPosition = *((crVector3*)_targerPosition);
				m_moveToTarget->inputParam(6,&targerPosition);
				(*m_moveToTarget)(*this);
			}
		}
	}
}
/////////////////////////////////////////
//
//crShowTalkStageLogic
//
/////////////////////////////////////////
crShowTalkStageLogic::crShowTalkStageLogic():
m_showType(1)
{
}

crShowTalkStageLogic::crShowTalkStageLogic(const crShowTalkStageLogic& handle):
crLogic(handle),
m_stageName(handle.m_stageName),
m_showType(handle.m_showType),
m_textElementName(handle.m_textElementName),
m_text(handle.m_text)
{
	ButtonHandle *buttonHandle;
	for( ButtonHandleArray::const_iterator itr = handle.m_buttonHandleArray.begin();
		 itr != handle.m_buttonHandleArray.end();
		 ++itr )
	{
        buttonHandle = new ButtonHandle;
		buttonHandle->m_buttonName = (*itr)->m_buttonName;
		buttonHandle->m_buttonText = (*itr)->m_buttonText;
		if((*itr)->m_buttonHandle.valid())
		    buttonHandle->m_buttonHandle = (*itr)->m_buttonHandle->clone();
		m_buttonHandleArray.push_back(buttonHandle);
	}
}

void crShowTalkStageLogic::inputParam(int i, void *param)
{
	for( ButtonHandleArray::iterator itr = m_buttonHandleArray.begin();
		itr != m_buttonHandleArray.end();
		++itr )
	{
		(*itr)->m_buttonHandle->inputParam(i,param);
	}
}

void crShowTalkStageLogic::addParam(int i, const std::string& str)
{
	switch(i) 
	{
	case 0:
		m_stageName = str;
		break;
	case 1:
		m_showType = atoi(str.c_str());
		break;
	case 2:
		m_textElementName = str;
		break;
	case 3:
		m_text = str;
		break;
	default:
		{
			if(i%2==0)
			{
				ButtonHandle *buttonHandle = new ButtonHandle;
		        m_buttonHandleArray.push_back(buttonHandle);
				buttonHandle->m_buttonName = str;
			}
			else
			{
				m_buttonHandleArray[m_buttonHandleArray.size()-1]->m_buttonText = str;
			}
		}
	}
}

void crShowTalkStageLogic::inputHandle(int i, void *param)
{
    if(i>=0 && i<m_buttonHandleArray.size())
	{
		m_buttonHandleArray[i]->m_buttonHandle = param?(CRCore::crHandle *)param:NULL;
	}
}

void crShowTalkStageLogic::outputParam(int i, void *param)
{
}

void crShowTalkStageLogic::operator()(crHandle &handle)
{
	crImageStage * stage = cr2DStageManager::getInstance()->findStage(m_stageName);
	if(stage)
	{
		crStaticTextBoxElement *textElement = dynamic_cast<crStaticTextBoxElement *>(stage->getElement(m_textElementName));
		if(textElement)
		{
            textElement->setStringArrayByString(m_text);
		}
		crStaticTextBoxElement *buttonText;
		crButtonElement *button;
		for( ButtonHandleArray::iterator itr = m_buttonHandleArray.begin();
			itr != m_buttonHandleArray.end();
			++itr )
		{
			buttonText = dynamic_cast<crStaticTextBoxElement *>(stage->getElement((*itr)->m_buttonName + "_N"));
			if(buttonText)
			{
				buttonText->setStringArrayByString((*itr)->m_buttonText);
			}
			button = dynamic_cast<crButtonElement *>(stage->getElement((*itr)->m_buttonName));
			if(button)
			{
                crHandle *handle = button->getHandle(MAKEINT64(WCH_UI_LBtnUp,NULL));
				if(handle)//OneSlot drive
				    handle->inputHandle(0,(*itr)->m_buttonHandle.get());
			}
		}
		switch(m_showType)
		{
		case 0:
			cr2DStageManager::getInstance()->showStage(stage,false);
			break;
		case 1:
			cr2DStageManager::getInstance()->showStage(stage,true);
			break;
		case 2:
			cr2DStageManager::getInstance()->doModal(stage);
			break;
		case 3:
            cr2DStageManager::getInstance()->showStage(stage,stage->isHide());
			break;
		}
	}
}
/////////////////////////////////////////
//
//crUpdateModelLogic
//
/////////////////////////////////////////
class UpdateModelVisitor : public CRCore::crNodeVisitor
{
public:
	UpdateModelVisitor():
	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	  virtual ~UpdateModelVisitor(){}

	  void init(const std::string &datafile, const std::string &newfile, crNode *loadedNode) { m_dataFile = datafile; m_newFile = newfile; m_loadedNode = loadedNode; }
	  virtual void apply(crGroup& node)
	  { 
		  if(node.getName().compare("ProxyNode") == 0)
		  {
			  std::string str;
			  if(crArgumentParser::readKeyValue(node.getDescriptions(),"DataFile",str) && str.compare(m_dataFile) == 0)
			  {
				  if(m_dataFile.compare(m_newFile)!=0)
				  {
					  std::string des = node.getDescriptionsInString();
					  crArgumentParser::updateOrAddKeyValue(des,"DataFile",m_newFile);
					  node.setDescriptionsInString(des);
				  }
				  crGroup *cloneGroup = dynamic_cast<crGroup *>(m_loadedNode->clone(crCopyOp::DEEP_COPY_NODES));
				  crGroup *child = dynamic_cast<crGroup *>(node.getChild(0));
				  if(dynamic_cast<crMatrixTransform *>(child))
				  {//
					  crMatrixTransform *matrixChild = dynamic_cast<crMatrixTransform *>(child);
					  if(dynamic_cast<crMatrixTransform *>(cloneGroup))
					  {
						  dynamic_cast<crMatrixTransform *>(cloneGroup)->setMatrix(matrixChild->getMatrix());
						  CRIOManager::crLoadManager::getInstance()->requestRemoveNode(&node,child);
						  CRIOManager::crLoadManager::getInstance()->requestAddNode(&node,cloneGroup,true);
					  }
					  else if(child->getNumChildren()==1)
					  {
						  CRIOManager::crLoadManager::getInstance()->requestRemoveNode(matrixChild,matrixChild->getChild(0));
						  CRIOManager::crLoadManager::getInstance()->requestAddNode(matrixChild,cloneGroup,true);
					  }
					  else
					  {
						  CRCore::notify(CRCore::ALWAYS)<<"该节点不能替换，节点名： "<<child->getName()<<std::endl;
						  crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
						  crGameBodyInfo *gamebodyInfo = body->getGameBodyInfo();
						  gamebodyInfo->selectNode(&node);
					  }
				  }
				  else if(child->getNumChildren() == 1 && cloneGroup->getNumChildren() == 1)
				  {
					  crMatrixTransform *childMatrix = dynamic_cast<crMatrixTransform *>(child->getChild(0));
					  crMatrixTransform *loadChildMatrix = dynamic_cast<crMatrixTransform *>(child->getChild(0));
					  if(childMatrix && loadChildMatrix)
					  {
						  loadChildMatrix->setMatrix(childMatrix->getMatrix());
						  dynamic_cast<crMatrixTransform *>(cloneGroup)->setMatrix(childMatrix->getMatrix());
						  CRIOManager::crLoadManager::getInstance()->requestRemoveNode(&node,child);
						  CRIOManager::crLoadManager::getInstance()->requestAddNode(&node,cloneGroup,true);
					  }
				  }
				  else
				  {
					  CRCore::notify(CRCore::ALWAYS)<<"该节点不能替换，节点名： "<<child->getName()<<std::endl;
					  crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
					  crGameBodyInfo *gamebodyInfo = body->getGameBodyInfo();
					  gamebodyInfo->selectNode(&node);
				  }
			  }
		  }
		  traverse(node);
	  }
protected:
	std::string m_dataFile;
	std::string m_newFile;
	crNode *m_loadedNode;
};

static UpdateModelVisitor s_updateModelVisitor;

crUpdateModelLogic::crUpdateModelLogic(){}
crUpdateModelLogic::crUpdateModelLogic(const crUpdateModelLogic& handle):
crMethod(handle),
m_modelListElement(handle.m_modelListElement)
{
	if(handle.m_convertToTransformMathod.valid())
		m_convertToTransformMathod = handle.m_convertToTransformMathod->clone();
}
void crUpdateModelLogic::inputParam(int i, void *param)
{
	if(i==0)
		m_this = (CRGUI::crElement*)param;
}

void crUpdateModelLogic::addParam(int i, const std::string& str)
{
	switch (i)
	{
	case 0:
		m_modelListElement = str;
		break;
	}
}
void crUpdateModelLogic::inputHandle(int i, void *param)
{
	switch (i)
	{
	case 0:
		m_convertToTransformMathod = (CRCore::crHandle *)param;
		break;
	}
}
void crUpdateModelLogic::outputParam(int i, void *param)
{
	switch (i)
	{
	case 1:
		m_convertedTransform = (CRCore::crMatrixTransform *)param;
		break;
	}
}
void crUpdateModelLogic::operator()(crHandle &handle)
{
	if(m_this)
	{
		crImageStage *stage = m_this->getParentStage();
		crListBoxElement *modeList = dynamic_cast<crListBoxElement *>(stage->getElement(m_modelListElement));
		if(modeList)
		{
			std::string fileName;
			modeList->getData(modeList->getSelect(),fileName);
			fileName = "data/"+fileName+".cre";

			crViewMatterObject *body = crSceneManager::getInstance()->getPlayer();
			crGameBodyInfo *gamebodyInfo = body->getGameBodyInfo();
			crData *infoData = gamebodyInfo->getDataClass();
			crGameBodyInfo::SelectNodeVec &selectNodeVec = gamebodyInfo->getSelectNodeVec();
			if(!selectNodeVec.empty())
			{
				crNode *currentSelectNode = selectNodeVec[0].get();
				if(currentSelectNode)
				{
					CRCore::crSearchNodeBYNameVisitor searchByNameVisitor;
					searchByNameVisitor.setTraversalMode(crNodeVisitor::TRAVERSE_PARENTS);
					searchByNameVisitor.setNameId("ProxyNode");
					searchByNameVisitor.setSearchNodeType(ALLNODE);
					currentSelectNode->accept(searchByNameVisitor);
					crGroup *proxyNode = dynamic_cast<crGroup *>(searchByNameVisitor.getResult());
					if(proxyNode)
					{
						std::string dataFile;
						if(crArgumentParser::readKeyValue(proxyNode->getDescriptions(),"DataFile",dataFile))
						{
							//ref_ptr<crNode> node = CRIOManager::readNodeFile(fileName/*dataFile*/);
							//if(node.valid())
							//{//遍历场景，找到所有该dataFile的模型进行更新
							//	//保留矩阵信息
							//	ref_ptr<crGroup> dummyGroup = dynamic_cast<crGroup *>(node.get());
							//	if(dummyGroup.valid()&&dummyGroup->getName().compare("LoadNodeDummy")==0)
							//	{
							//		node = dummyGroup->getChild(0);
							//		dummyGroup->removeChild(0,dummyGroup->getNumChildren());
							//	}
							//	CRIOManager::crDatabasePager *dp = CRIOManager::crRegistry::instance()->getOrCreateDatabasePager();
							//	CRIOManager::crDatabasePager::LoadedMap &loadedMap = dp->getLoadedMap();
							//	loadedMap[dataFile] = node.get();
							
							CRCore::ref_ptr<crGroup> proxyNode = new crGroup;
							proxyNode->setName("ProxyNode");
							proxyNode->setDescriptionsInString("DataFile "+fileName);
							CRIOManager::crLoadManager::getInstance()->requestNode(fileName,proxyNode.get(),false,true);
							if(proxyNode->getNumChildren()==1)
							{
								crNode *loadedNode = proxyNode->getChild(0);
								ref_ptr<crMatrixTransform> matrixTransform = dynamic_cast<crMatrixTransform *>(loadedNode);
								if(!matrixTransform.valid())
								{
									m_convertToTransformMathod->inputParam(1,loadedNode);
									(*m_convertToTransformMathod)(*this);
									m_convertToTransformMathod->inputParam(1,NULL);
									if(m_convertedTransform.valid())
									{
										matrixTransform = m_convertedTransform.get();
										m_convertedTransform = NULL;
									}
								}
								gamebodyInfo->unSelectAll();
								s_updateModelVisitor.init(dataFile,fileName,matrixTransform.valid()?matrixTransform.get():loadedNode);
								crBrain::getInstance()->accept(s_updateModelVisitor);
							}
						}
					}
				}
			}
		}
	}
}