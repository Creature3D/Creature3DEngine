/**********************************************************************
*
*	文件:	   appNodeCallbacks.cpp
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
**********************************************************************/
#include <rbody/Creature3D/CreBody.h>
#include <JXJ/appNodeCallbacks.h>
#include <CRCore/crArgumentParser.h>
#include <CRUtil/crSetObjectColorVisitor.h>
#include <CRPhysics/crViewMatterObject.h>
using namespace CRCore;
using namespace CRUtil;
using namespace CRPhysics;
using namespace JXJ;
using namespace rbody;
////////////////////////////////
//crJXJSetDrawableColorCallback
//UpdateCallback
///////////////////////////////////
crJXJSetDrawableColorCallback::crJXJSetDrawableColorCallback():
	m_done(2)
{
	m_color.set(1.0f,1.0f,1.0f,1.0f);
}
crJXJSetDrawableColorCallback::crJXJSetDrawableColorCallback(const crJXJSetDrawableColorCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_color(callback.m_color),
	m_done(callback.m_done)
{
}
void crJXJSetDrawableColorCallback::addEventParam(int i, const std::string &str)
{
	switch (i)
	{
	case 0:
		{
			crVector4 color;
			crArgumentParser::appAtoVec(str,color);
			m_color = color/255.0f;
			m_done = 0;
		}
		break;
	}
}
void crJXJSetDrawableColorCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	if(m_done<2)
	{
		crSetObjectColorVisitor setcolor;
		setcolor.setColor(m_color);
		node->accept(setcolor);
		m_done = 2;
	}
	crNodeCallback::traverse(node,nv);
}
void crJXJSetDrawableColorCallback::releaseObjects(CRCore::crState* state)
{
	m_color.set(1.0f,1.0f,1.0f,1.0f);
	m_done = 0;
}
////////////////////////////////
//crJXJScaleBodyCallback
//UpdateCallback
///////////////////////////////////
crJXJScaleBodyCallback::crJXJScaleBodyCallback():
	m_origscale(1.0f),
	m_destscale(1.0f),
	m_speed(1.0f),
	m_scale(1.0f),
	m_done(2)
{
}
crJXJScaleBodyCallback::crJXJScaleBodyCallback(const crJXJScaleBodyCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_origscale(callback.m_origscale),
	m_destscale(callback.m_destscale),
	m_speed(callback.m_speed),
	m_scale(callback.m_scale),
	m_done(callback.m_done)
{
}
void crJXJScaleBodyCallback::addEventParam(int i, const std::string &str)
{
	switch (i)
	{
	case 0:
		{
			crVector3 vec2;
			crArgumentParser::appAtoVec(str,vec2);
			m_destscale = vec2[0];
			m_speed = vec2[1];
			m_done = 0;
		}
		break;
	}
}
void crJXJScaleBodyCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	if(m_done<2)
	{
		crViewMatterObject *bot = dynamic_cast<crViewMatterObject *>(node);
		if(bot)
		{
			CreBodyNode *bodyNode = dynamic_cast<CreBodyNode *>(bot->getCurrentActiveBody());
			if(m_done==0)
			{
				m_origscale = bodyNode->getBody()->getScale();
				m_scale = m_origscale;
				m_done = 1;
			}
			
			float dt = crFrameStamp::getInstance()->getFrameInterval();
			float ds = m_speed * dt;
			if(m_scale>m_destscale)
			{
				m_scale -= ds;
				if(m_scale<m_destscale)
				{
					m_scale = m_destscale;
					m_done = 2;
				}
			}
			else if(m_scale<m_destscale)
			{
				m_scale += ds;
				if(m_scale>m_destscale)
				{
					m_scale = m_destscale;
					m_done = 2;
				}
			}
			else
			{
				m_done = 2;
			}
			bodyNode->setScale(m_scale);
		}
		else
		{
			m_done = 2;
		}
	}
	crNodeCallback::traverse(node,nv);
}
void crJXJScaleBodyCallback::releaseObjects(CRCore::crState* state)
{
	m_destscale = m_origscale;
	m_done = 1;
}
////////////////////////////////
//crJXJCloseToTargetNodeCallback
//UpdateCallback
///////////////////////////////////
crJXJCloseToTargetNodeCallback::crJXJCloseToTargetNodeCallback():
	m_speed(10.0f),
	m_done(2)
{
}
crJXJCloseToTargetNodeCallback::crJXJCloseToTargetNodeCallback(const crJXJCloseToTargetNodeCallback& callback,const CRCore::crCopyOp&copyop):
	crNodeCallback(callback,copyop),
	m_speed(callback.m_speed),
	m_done(callback.m_done)
{
}
void crJXJCloseToTargetNodeCallback::addEventParam(int i, const std::string &str)
{
	switch (i)
	{
	case 0:
		m_speed = atof(str.c_str());
		m_done = 0;
		break;
	}
}
void crJXJCloseToTargetNodeCallback::setTargetNode(CRCore::crMatrixTransform *targetNode)
{
	m_targetNode = targetNode;
}
void crJXJCloseToTargetNodeCallback::operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv)
{
	if(m_done<2)
	{
		crMatrixTransform *thisNode = dynamic_cast<crMatrixTransform *>(node);
		if(thisNode && m_targetNode.valid())
		{
			crVector3 thispos = thisNode->getTrans();
			crVector3 targetpos = m_targetNode->getBound().center();
			crVector3 dir = targetpos - thispos;
			float dist = dir.length();
			if(dist<0.1f)
			{
				m_done = 2;
			}
			else
			{
				dir.normalize();
				float dt = crFrameStamp::getInstance()->getFrameInterval();
				thispos += dir * m_speed * dt;
				thisNode->setPosition(thispos);
				m_done = 1;
			}
		}
		else
		{
			m_done = 2;
		}
	}
	crNodeCallback::traverse(node,nv);
}
void crJXJCloseToTargetNodeCallback::releaseObjects(CRCore::crState* state)
{
	m_targetNode = NULL;
}