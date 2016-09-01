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
#include <CRCore/crRenderToTextureManager.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;
ref_ptr<crRenderToTextureManager> crRenderToTextureManager::m_instance = NULL;

crRenderToTextureManager::crRenderToTextureManager():
	m_rttShadowMode(0)
{
	//setNumChildrenRequiringUpdateUniformTraversal(1);
}

crRenderToTextureManager::~crRenderToTextureManager()
{
    //CRCore::notify(CRCore::ALWAYS)<<"~crRenderToTextureManager()"<< std::endl;
}

crRenderToTextureManager::crRenderToTextureManager(const crRenderToTextureManager& rttNode,const crCopyOp& copyop):
crNode(rttNode,copyop),
m_rttShadowMode(rttNode.m_rttShadowMode)
{
}

crRenderToTextureManager* crRenderToTextureManager::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crRenderToTextureManager;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
	//static ref_ptr<crRenderToTextureManager> s_manager = new crRenderToTextureManager;
	//return s_manager.get();
}

void crRenderToTextureManager::clear()
{
	m_instance = NULL;
}

void crRenderToTextureManager::addRenderToTexture(crRenderToTexture *rtt)
{
	//if(sync)
	//{
	//	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_syncAddRTTVecMutex);
	//	m_syncAddRTTVec.push_back(rtt);
	//}
	//else
	//{
		m_renderToTextureMap[rtt->getName()] = rtt;
		char gbuf[256];
		sprintf(gbuf,"addRenderToTexture %s\n\0",rtt->getName().c_str());
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
		if(rtt->getName() == "ls1shadow")
		{
			if(m_rttShadowMode == 0) m_rttShadowMode = 1;
		}
		else if(rtt->getName() == "ls2shadow")
		{
			m_rttShadowMode = 2;
		}
	//}
}
crRenderToTexture *crRenderToTextureManager::findRenderToTexture(const std::string &name)
{
	RenderToTextureMap::iterator itr = m_renderToTextureMap.find(name);
	return itr != m_renderToTextureMap.end()?itr->second.get():NULL;
}
//void crRenderToTextureManager::removeAllRenderTexture()
//{
//	m_renderToTextureMap.clear();
//}
void crRenderToTextureManager::traverse(crNodeVisitor& nv)
{
	//std::string str = nv.getTraverseString();
	//nv.setTraverseString("RenderToTextureManager");
	crNodeVisitor::VisitorType vt = nv.getVisitorType();
	crRenderToTexture *rt;
	if (vt==CRCore::crNodeVisitor::CULL_VISITOR)
	{
		crPolytope view_frustum;
		view_frustum.setToUnitFrustum(true,true);
		view_frustum.transformProvidingInverse( m_currentViewMatrix * m_currentProjectMatrix );
		crRenderToTexture::setCurrentViewFrustum(view_frustum);
		//crVector3d eye,center,up;//,lv;
		//m_currentViewMatrix.getLookAt(eye,center,up);
		//eye.z() = 0.0f;
		//center.z() = 0.0f;
		//lv = center - eye;
		//lv.normalize();

		//eye += lv * c_sunViewRange * 0.5f;

		//int frameNumber = nv.getFrameStamp()->getFrameNumber();
		//m_syncAddRTTVecMutex.acquire();
		//if(!m_syncAddRTTVec.empty())
		//{
		//	for( SyncAddRTTVec::iterator itr = m_syncAddRTTVec.begin();
		//		itr != m_syncAddRTTVec.end();
		//		++itr )
		//	{
		//		addRenderToTexture(itr->get());
		//	}
		//	m_syncAddRTTVec.clear();
		//}
		//m_syncAddRTTVecMutex.release();
		for( RenderToTextureMap::iterator itr = m_renderToTextureMap.begin();
			itr != m_renderToTextureMap.end();
			++itr )
		{
			rt = itr->second.get();

			if(!rt->getLightSource()) 
			{
				if(rt->getNeedUpd(/*nv.getFrameStamp()*/))
				{
					rt->update();

					if(!rt->culled())
					{
						rt->traverse(nv);
					}
				}
			}
		}
	}
	else if(vt == CRCore::crNodeVisitor::SWAPBUFFER_VISITOR || vt==CRCore::crNodeVisitor::INIT_VISITOR)
	{
		for( RenderToTextureMap::iterator itr = m_renderToTextureMap.begin();
			itr != m_renderToTextureMap.end();
			++itr )
		{
			itr->second->traverse(nv);
		}
	}
	//nv.setTraverseString(str);
}

//void crRenderToTextureManager::updateUniform(/*crViewPort *sceneViewPort*/)
//{
//	for( RenderToTextureMap::iterator itr = m_renderToTextureMap.begin();
//		itr != m_renderToTextureMap.end();
//		++itr )
//	{
//		itr->second->updateUniform();
//	}
//}

void crRenderToTextureManager::releaseObjects(crState* state)
{
	for( RenderToTextureMap::iterator itr = m_renderToTextureMap.begin();
		itr != m_renderToTextureMap.end();
		++itr )
	{
		itr->second->releaseObjects(state);
	}
}

//void crRenderToTextureManager::swapBuffers()
//{
//	for( RenderToTextureMap::iterator itr = m_renderToTextureMap.begin();
//		itr != m_renderToTextureMap.end();
//		++itr )
//	{
//		itr->second->swapBuffers();
//	}
//}
