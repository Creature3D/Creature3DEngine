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
#include <CRNetApp/appNodeCallbacks.h>
#include <CRNetApp/appNetGameData.h>
#include <CRNetApp/appClientPlayerData.h>
#include <CRUtil/crUpdateVisitor.h>
#include <CRCore/crShapeDrawable.h>
#include <CRIOManager/crLoadManager.h>

using namespace CRNetApp;
using namespace CRCore;
using namespace CRIOManager;
////////////////////////////////
//crDecalUpdateCallback
//UpdateCallback
///////////////////////////////////
crDecalUpdateCallback::crDecalUpdateCallback():
	m_dirty(false),
	m_visiable(false)
{
}
crDecalUpdateCallback::crDecalUpdateCallback(const crDecalUpdateCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_position(callback.m_position),
	m_size(callback.m_size),
	m_dirty(false),
	m_color(callback.m_color)
{
}
void crDecalUpdateCallback::addEventParam(int i, const std::string &str)
{
}
void crDecalUpdateCallback::setPosition(const CRCore::crVector2 &pos)
{
	if(m_position != pos)
	{
		m_position = pos;
		dirty();
	}
}
void crDecalUpdateCallback::setSize(const CRCore::crVector2 &size)
{
	if(m_size != size)
	{
		m_size = size;
		dirty();
	}
}
void crDecalUpdateCallback::dirty()
{
	m_dirty = true;
}
void crDecalUpdateCallback::setColor(const CRCore::crVector4 &color)
{
	m_color = color;
}
void crDecalUpdateCallback::setVisiable(bool visiable)
{
	m_visiable = visiable;
}
void crDecalUpdateCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv)
	{
		node->setVisiable(m_visiable);
		if(m_visiable && dynamic_cast<crObject *>(node))
		{
			crObject *obj = dynamic_cast<crObject *>(node);
			crShapeDrawable *shape = dynamic_cast<crShapeDrawable *>(obj->getDrawable(0));
			if(shape)
			{
				shape->setColor(m_color);
				if(m_dirty)
				{
					crHeightField *heightField = dynamic_cast<crHeightField *>(shape->getShape());
					if(heightField)
					{//更新
						crSceneLayer *layer = crMyPlayerData::getInstance()->getSceneLayer();
						float dx = layer->getXInterval();
						float dy = layer->getYInterval();
						int numColumns = m_size[0] / dx;
						int numRows = m_size[1] / dy;
						if(numColumns<3)
							numColumns = 3;
						else if(numColumns%2==0)
						{
							numColumns+=1;
						}
						if(numRows<3) 
							numRows = 3;
						else if(numRows%2==0)
						{
							numRows+=1;
						}
						
						heightField->allocate(numColumns,numRows);
						heightField->setXInterval(dx);
						heightField->setYInterval(dy);
						int c_2 = numColumns>>1;
						int r_2 = numRows>>1;
						crVector2 origin = layer->getOrigin();
						heightField->setOrigin(crVector3(m_position[0] - c_2 * dx,m_position[1] - r_2 * dy,0.0f));
						crVector2 pos = m_position;
						pos -= origin;
						pos[0] /= dx;
						pos[1] /= dy;
						int s1,t1,s,t;
						for(s = 0; s<numColumns; ++s)
						{
							for(t=0;t<numRows;++t)
							{
								s1 = s + pos[0] - c_2;
								t1 = t + pos[1] - r_2;
								heightField->setHeight(s,t,layer->getHeight(s1,t1)+0.02f);
							}
						}
					}
					m_dirty = false;
				}
			}
		}
		crNodeCallback::traverse(node,nv);
	}
}
////////////////////////////////
//crAutoTransCallback
//CullCallback
///////////////////////////////////
crAutoTransCallback::crAutoTransCallback():
	m_minalpha(0.5f),
	m_alpha(1.0f),
	m_interval(0.0f),
	m_speed(2.0f),
	m_previousTraversalNumber(0)
{
}
crAutoTransCallback::crAutoTransCallback(const crAutoTransCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_minalpha(callback.m_minalpha),
	m_alpha(1.0f),
	m_interval(0.0f),
	m_speed(callback.m_speed),
	m_previousTraversalNumber(0)
{
	crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ONE_MINUS_SRC_ALPHA);
	m_stateset = new crStateSet;
	m_stateset->setAttributeAndModes(blendFunc,crStateAttribute::ON|crStateAttribute::OVERRIDE);
	m_stateset->setRenderingHint(crStateSet::TRANSPARENT_BIN);
	m_stateset->setName("#AutoTrans");
}
void crAutoTransCallback::addEventParam(int i, const std::string &str)
{
}
void crAutoTransCallback::setAutoTrans(float interval,float minalpha)
{
	m_interval = interval;
	m_minalpha = minalpha;
}
void crAutoTransCallback::setSpeed(float speed)
{
	m_speed = speed;
}
void crAutoTransCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(nv);
	crObject *obj = dynamic_cast<crObject *>(node);
	if (obj&&cv&&cv->getRenderMode() == CRUtil::crCullVisitor::NormalRender)
	{
		if(nv->getTraversalNumber() != m_previousTraversalNumber)
		{
			m_previousTraversalNumber = nv->getTraversalNumber();
			float dt = crFrameStamp::getInstance()->getFrameInterval();
			if(m_interval>0.0f)
			{
				if(m_alpha > m_minalpha)
				{
					m_alpha -= m_speed*dt;
					if(m_alpha < m_minalpha)
					{
						m_alpha = m_minalpha;
					}
				}
				m_interval -= dt;
			}
			else if(m_alpha<1.0f)
			{//恢复到1.0
				m_alpha += m_speed*dt;
				if(m_alpha>1.0f)
					m_alpha = 1.0f;
			}
		}
		crStateSet *ss = obj->getStateSet();
		if(m_alpha<1.0f)
		{
			if(!ss || ss->getName().compare("#AutoTrans")!=0)
			{
				m_objstateset = ss;
				obj->setStateSet(m_stateset.get());
				ss = m_objstateset.get();
			}
			if(ss && ss->getName().compare("#AutoTrans")==0)
			{
				crObject::DrawableList &drawableList = obj->getDrawableList();
				for( crObject::DrawableList::iterator itr = drawableList.begin();
					itr != drawableList.end();
					++itr )
				{
					(*itr)->getColor().a() = m_alpha;
				}
				obj->setNodeMask(obj->getNodeMask() | AutoTransMask);
			}
		}
		else
		{
			if(ss && ss->getName().compare("#AutoTrans")==0)
			{
				obj->setStateSet(m_objstateset.get());
				crObject::DrawableList &drawableList = obj->getDrawableList();
				for( crObject::DrawableList::iterator itr = drawableList.begin();
					itr != drawableList.end();
					++itr )
				{
					(*itr)->getColor().a() = m_alpha;
				}
				obj->setNodeMask(obj->getNodeMask() & ~AutoTransMask);
			}
		}
		crNodeCallback::traverse(node,nv);
	}
}
////////////////////////////////
//crActFxCallback
//UpdateCallback
///////////////////////////////////
crActFxCallback::crActFxCallback():
	m_actitem(NULL),
	m_act(ACT_None)
{
}
crActFxCallback::crActFxCallback(const crActFxCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_act(callback.m_act),
	m_actitem(callback.m_actitem)
{
}

void crActFxCallback::setActInstanceItem(crInstanceItem *item)
{
	m_actitem = item;
}
void crActFxCallback::setAct(unsigned short act)
{
	m_act = act;
}
void crActFxCallback::setVisiableOrUnload(bool flg)
{
	m_visiableOrUnload = flg;
}
void crActFxCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(nv);
	if (uv && m_actitem)
	{
		crData *data = m_actitem->getDataClass();
		if(data)
		{
			void *param;
			data->getParam(WCHDATA_CurActState,param);
			unsigned short act = *(unsigned short *)param;
			if(act != m_act)
			{
				if(m_visiableOrUnload)
				{
					node->setVisiable(false);
				}
				else
				{
					crGroup *proxyNode = node->getParent(0);
					crGroup *root = proxyNode->getParent(0);
					CRIOManager::crLoadManager::getInstance()->requestRemoveNode(root,proxyNode);
				}
			}
		}
		crNodeCallback::traverse(node,nv);
	}
}