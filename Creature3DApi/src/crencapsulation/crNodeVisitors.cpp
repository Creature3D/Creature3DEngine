/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#include <CRUtil/crSceneView.h>
#include <CREncapsulation/crNodeVisitors.h>
#include <CRProducer/crViewer.h>
#include <CRProducer/crKeyboardMouseHandle.h>
#include <CRTerrain/crTerrainTile.h>
#include <CRProducer/crSceneHandler.h>
#include <CRIOManager/crLoadManager.h>
#include <CREncapsulation/crLoader.h>
using namespace CRCore;
using namespace CREncapsulation;
using namespace CRProducer;
using namespace CRUtil;
using namespace CRTerrain;
using namespace CRIOManager;
/////////////////////////////////////////
//
//crDisconnectDrawableVisitor
//
/////////////////////////////////////////
void crDisconnectDrawableVisitor::apply(CRCore::crObject& node)
{
	crObject::DrawableList drawableList = node.getDrawableList();
	//node.releaseObjects(NULL);
	node.removeDrawable(0,node.getNumDrawables());
	for( crObject::DrawableList::iterator itr = drawableList.begin();
		itr != drawableList.end();
		++itr )
	{
		node.addDrawable(dynamic_cast<crDrawable *>((*itr)->clone(crCopyOp::DEEP_COPY_DRAWABLES | crCopyOp::DEEP_COPY_ARRAYS | crCopyOp::DEEP_COPY_PRIMITIVES)));
	}
}
/////////////////////////////////////////
//
//crRectSelectNodeVisitor
//
/////////////////////////////////////////
void crRectSelectNodeVisitor::setSelectType(int _type)
{ m_selectType = _type; }
void crRectSelectNodeVisitor::setSelectClassName(const std::string &classname)
{ m_classname = classname; }
void crRectSelectNodeVisitor::setRect(CRCore::crVector4i &rect)
{ 
	m_rectBB.set(crVector3(rect[0],rect[1],-1),crVector3(rect[2],rect[3],1));
	m_rectBB.correct(); 
}
void crRectSelectNodeVisitor::apply(crNode& node)
{
	if(node.getVisiable())
	{
		if(m_selectType>=3 && m_selectType<21)
		{
			if(m_classname.compare(node.className()) == 0)
			{
				doSelect(node);
				return;
			}
		}
		traverse(node);
	}
}
void crRectSelectNodeVisitor::apply(crGroup& node)
{
	if(node.getVisiable())
	{
		if(m_selectType == 0)
		{
			if(node.getName() == "ProxyNode")
			{
				crNode *selectNode = node.getChild(0);
				if(selectNode && selectNode->getVisiable()) doSelect(*selectNode);
				return;
			}
		}
		traverse(node);
	}
}
void crRectSelectNodeVisitor::apply(crObject& node)
{
	if(node.getVisiable())
	{
		if(m_selectType == 1)
		{
			doSelect(node);
			return;
		}
		traverse(node);
	}
}
void crRectSelectNodeVisitor::doSelect(crNode &node)
{
	crViewer *bindview = crKeyboardMouseHandle::getInstance()->getBindViewer();
	if( bindview && bindview->isInited() && !dynamic_cast<crTerrainTile *>(&node) && node.getName().compare("sky") != 0 &&
		node.getName().compare("EditorAxis") != 0 && 
		node.getName().compare("ViewGrid") != 0 && 
		node.getName().compare("WaypointLink") != 0 )
	{
		crMatrix mat;
		if( !m_matrixList.empty() )
			mat = *(m_matrixList.back().get());
		crSceneView *sceneView = bindview->getDefaultSceneHandler()->getSceneView();
		crVector3 window;
		crVector3 pos;
		pos = node.getBound().center() * mat;
		if(crBrain::getInstance()->getCameraFrustum().contains(pos))
		{
			sceneView->projectObjectIntoWindow(pos,window);
			window[2] = 0;
			if(m_rectBB.contains(window))
			{
				m_selectNodeVec.push_back(&node);
			}
		}
	}
}
void crRectSelectNodeVisitor::apply(CRCore::crTransform& node)
{
	if(node.getVisiable())
	{
		if(m_selectType == 22)
		{
			if(node.getName() == "#Waypoint")
			{
				doSelect(node);
				return;
			}
		}
		CRCore::ref_ptr<RefMatrix> matrix = new RefMatrix;
		node.computeLocalToWorldMatrix(*matrix,this);

		if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
		{
			if(!m_matrixList.empty())
				matrix->preMult(*(m_matrixList.back().get()));
		}

		pushMatrix(matrix.get());

		traverse(node);

		popMatrix();
	}
}
/////////////////////////////////////////
//
//crLoadUISceneVisitor
//
/////////////////////////////////////////
void crLoadUISceneVisitor::apply(crGroup& node)
{ 
	if(node.getName().compare("ProxyNode") == 0)
	{
		CRIOManager::crLoadManager::getInstance()->requestAddNode(m_root,&node,true);
		addUINodeToScene(dynamic_cast<crGroup *>(&node));
	}
	traverse(node);
}