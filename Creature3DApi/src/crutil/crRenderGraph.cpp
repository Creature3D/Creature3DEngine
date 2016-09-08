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
//Modified by 吴财华
#include <CRUtil/crRenderGraph.h>
#include <CRCore/crMath.h>

using namespace std;
using namespace CRCore;
using namespace CRUtil;

//crRenderGraph::ToEraseList crRenderGraph::s_toEraseList;
//crRenderGraph::GraphList crRenderGraph::s_reuseGraphList;
//int crRenderGraph::s_currentReuseRenderGraphIndex = 0;

void crRenderGraph::reset()
{
  m_parent = NULL;
  m_stateset = NULL;

  m_depth = 0;

  m_children.clear();
  m_leaves.resize(0);

  for(ChildList::iterator itr=m_children.begin();
	  itr!=m_children.end();
	  ++itr)
  {
	  itr->second->reset();
  }
  m_children.clear();
}

/** recursively clean the crRenderGraph of all its drawables, lights and depths.
* Leaves children intact, and ready to be populated again.*/
void crRenderGraph::clean()
{
  m_leaves.resize(0);
	//m_leaves.clear();
  // call clean on all children.
  for(ChildList::iterator itr=m_children.begin();
    itr!=m_children.end();
    ++itr)
  {
    itr->second->clean();
  }
}

/** recursively prune the crRenderGraph of empty children.*/
void crRenderGraph::prune()
{
  /*collect();

  for(ToEraseList::iterator eitr=s_toEraseList.begin();
    eitr!=s_toEraseList.end();
    ++eitr)
  {
    m_children.erase(*eitr);
  }

  s_toEraseList.resize(0);*/

	//std::list<const CRCore::crStateSet*> toEraseList;

	// call prune on all children.
	for(ChildList::iterator citr=m_children.begin();
		citr!=m_children.end();
		/*++citr*/)
	{
		citr->second->prune();

		if (citr->second->empty())
		{
			//toEraseList.push_back(citr->first);
			citr = m_children.erase(citr);
		}
		else
		{
			++citr;
		}
	}

	//for(std::list<const CRCore::crStateSet*>::iterator eitr=toEraseList.begin();
	//	eitr!=toEraseList.end();
	//	++eitr)
	//{
	//	m_children.erase(*eitr);
	//}
}

//void crRenderGraph::collect()
//{
//	// call prune on all children.
//	for(ChildList::iterator citr=m_children.begin();
//		citr!=m_children.end();
//		++citr)
//	{
//		citr->second->collect();
//
//		if (citr->second->empty())
//		{
//			s_toEraseList.push_back(citr->first);
//		}
//	}
//}


//void crRenderGraph::prune_optimizer()
//{
//	std::vector<const crStateSet*> toEraseList;
//
//	// call prune on all children.
//	for(ChildList::iterator citr=m_children.begin();
//		citr!=m_children.end();
//		++citr)
//	{
//		citr->second->prune_optimizer();
//
//		if (citr->second->empty())
//		{
//			toEraseList.push_back(citr->first);
//		}
//	}
//
//	for(std::vector<const crStateSet*>::iterator eitr=toEraseList.begin();
//		eitr!=toEraseList.end();
//		++eitr)
//	{
//		m_children.erase(*eitr);
//	}
//
//	//合并叶子节点
//	for(LeafList::iterator litr = m_leaves.begin();
//		litr != m_leaves.end();
//		litr++)
//	{
//
//	}
//}
