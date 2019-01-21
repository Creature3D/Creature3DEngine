/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRUtil/crRenderLeaf.h>
#include <CRUtil/crRenderGraph.h>
#include <CRCore/crStatistics.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crNotify.h>

using namespace CRCore;
using namespace CRUtil;

void crRenderLeaf::renderZprePass(CRCore::crState& state,crRenderLeaf* previous)
{
	// don't draw this leaf if the abort rendering flag has been set.
	if (state.getAbortRendering() || m_drawable->isAbortCurrentRendering())
	{
		//cout << "early abort"<<endl;
		goto RenderLeafReturn;
	}

	crStatistics *statistics = crStatistics::getInstance();

	if (previous)
	{
		// apply matrices if required.
		state.applyProjectionMatrix(m_projection.get());
		state.applyModelViewMatrix(m_modelview.get());

		//crStatistics::getInstance()->addMatrix();

		// apply state if required.
		crRenderGraph* prev_rg = previous->m_parent;
		crRenderGraph* prev_rg_parent = prev_rg->m_parent;
		crRenderGraph* rg = m_parent;
#ifdef _DEBUG
		try
		{
#endif
			if (prev_rg_parent!=rg->m_parent)
			{
				crRenderGraph::moveRenderGraph(state,prev_rg_parent,rg->m_parent);
				// send state changes and matrix changes to OpenGL.
				//state.apply(rg->m_stateset);
				//if(statistics->getStat())
				//	statistics->addState();
			}
			else if (rg!=prev_rg)
			{
				// send state changes and matrix changes to OpenGL.
				//state.apply(rg->m_stateset);
				//if(statistics->getStat())
				//	statistics->addState();
			}
#ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() stateset error stateset = "<<rg->m_stateset->getName()<< std::endl;
		}
#endif
		// draw the drawable
#ifdef _DEBUG
		try
		{
#endif
			m_drawable->draw(state);
#ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() m_drawable->draw(state) = "<<m_drawable->getName()<< std::endl;
		}
#endif

		if(statistics->getStat())
		{
			statistics->addDrawable();
			crGeometry *geom = m_drawable->asGeometry();
			if(geom)
			{
				statistics->addVertexCount(geom->getVertexArray()->getNumElements());

				for( crGeometry::PrimitiveList::iterator itr = geom->getPrimitiveList().begin();
					itr != geom->getPrimitiveList().end();
					++itr )
				{
					statistics->addPrimCount((*itr)->getNumPrimitives());
				}
			}
		}
	}
	else
	{
		// apply matrices if required.
		state.applyProjectionMatrix(m_projection.get());
		state.applyModelViewMatrix(m_modelview.get());

		//crStatistics::getInstance()->addMatrix();

		// apply state if required.
		crRenderGraph::moveRenderGraph(state,NULL,m_parent->m_parent);

#ifdef _DEBUG
		try
		{
#endif
			//state.apply(m_parent->m_stateset);
			//if(statistics->getStat())
			//	statistics->addState();

			// draw the drawable
			m_drawable->draw(state);
#ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() m_parent error stateset = "<<m_parent->m_stateset->getName()<< std::endl;
		}
#endif
		if(statistics->getStat())
		{
			statistics->addDrawable();
			crGeometry *geom = m_drawable->asGeometry();
			if(geom)
			{
				statistics->addVertexCount(geom->getVertexArray()->getNumElements());

				for( crGeometry::PrimitiveList::iterator itr = geom->getPrimitiveList().begin();
					itr != geom->getPrimitiveList().end();
					++itr )
				{
					statistics->addPrimCount((*itr)->getNumPrimitives());
				}
			}
		}
	}
RenderLeafReturn:
	if(m_needUnlockNode.valid())
		m_needUnlockNode->unlock();
	//crDrawable::ParentList parentList = m_drawable->getParents();
	//   for(crDrawable::ParentList::iterator itr = parentList.begin();
	//	itr != parentList.end();
	//	++itr)
	//{
	//	try
	//	{
	//	    if(typeid(**itr) == typeid(CRCore::crBillboard))
	//               (*itr)->unlock();
	//	}
	//	catch (...) 
	//	{
	//		int i = 0;
	//	}
	//}
}
void crRenderLeaf::render(crState& state,crRenderLeaf* previous)
{
    // don't draw this leaf if the abort rendering flag has been set.
    if (!m_drawable.valid() || state.getAbortRendering() || m_drawable->isAbortCurrentRendering())
    {
        //cout << "early abort"<<endl;
        goto RenderLeafReturn;
    }

	crStatistics *statistics = crStatistics::getInstance();

    if (previous)
    {
        // apply matrices if required.
		state.applyProjectionMatrix(m_projection.get());
		state.applyModelViewMatrix(m_modelview.get());
        
		//crStatistics::getInstance()->addMatrix();

        // apply state if required.
        crRenderGraph* prev_rg = previous->m_parent;
        crRenderGraph* prev_rg_parent = prev_rg->m_parent;
        crRenderGraph* rg = m_parent;
		#ifdef _DEBUG
		try
		{
        #endif
        if (prev_rg_parent!=rg->m_parent)
        {
            crRenderGraph::moveRenderGraph(state,prev_rg_parent,rg->m_parent);
            // send state changes and matrix changes to OpenGL.

			state.apply(rg->m_stateset);
		    if(statistics->getStat())
		        statistics->addState();
        }
        else if (rg!=prev_rg)
        {
            // send state changes and matrix changes to OpenGL.
            state.apply(rg->m_stateset);
			if(statistics->getStat())
				statistics->addState();
        }
		#ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() stateset error stateset = "<<rg->m_stateset->getName()<< std::endl;
		}
        #endif
        // draw the drawable
		#ifdef _DEBUG
		try
		{
        #endif
	        m_drawable->draw(state);
        #ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() m_drawable->draw(state) = "<<m_drawable->getName()<< std::endl;
		}
        #endif

	   if(statistics->getStat())
	   {
		   statistics->addDrawable();
		   crGeometry *geom = m_drawable->asGeometry();
		   if(geom)
		   {
			   statistics->addVertexCount(geom->getVertexArray()->getNumElements());
		   
				for( crGeometry::PrimitiveList::iterator itr = geom->getPrimitiveList().begin();
						itr != geom->getPrimitiveList().end();
						++itr )
				{
					statistics->addPrimCount((*itr)->getNumPrimitives());
				}
		   }
	   }
	}
    else
    {
        // apply matrices if required.
        state.applyProjectionMatrix(m_projection.get());
        state.applyModelViewMatrix(m_modelview.get());

		//crStatistics::getInstance()->addMatrix();

        // apply state if required.
        crRenderGraph::moveRenderGraph(state,NULL,m_parent->m_parent);

		#ifdef _DEBUG
		try
		{
        #endif
        state.apply(m_parent->m_stateset);
		if(statistics->getStat())
			statistics->addState();

        // draw the drawable
        m_drawable->draw(state);
		#ifdef _DEBUG
		}
		catch (...)
		{
			CRCore::notify(CRCore::ALWAYS)<<"crRenderLeaf::render() m_parent error stateset = "<<m_parent->m_stateset->getName()<< std::endl;
		}
        #endif
		if(statistics->getStat())
		{
			statistics->addDrawable();
			crGeometry *geom = m_drawable->asGeometry();
			if(geom)
			{
				statistics->addVertexCount(geom->getVertexArray()->getNumElements());
			
				for( crGeometry::PrimitiveList::iterator itr = geom->getPrimitiveList().begin();
					itr != geom->getPrimitiveList().end();
					++itr )
				{
					statistics->addPrimCount((*itr)->getNumPrimitives());
				}
			}
		}
    }
RenderLeafReturn:
	if(m_needUnlockNode.valid())
		m_needUnlockNode->unlock();
	//crDrawable::ParentList parentList = m_drawable->getParents();
 //   for(crDrawable::ParentList::iterator itr = parentList.begin();
	//	itr != parentList.end();
	//	++itr)
	//{
	//	try
	//	{
	//	    if(typeid(**itr) == typeid(CRCore::crBillboard))
 //               (*itr)->unlock();
	//	}
	//	catch (...) 
	//	{
	//		int i = 0;
	//	}
	//}
}
