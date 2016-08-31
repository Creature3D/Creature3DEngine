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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_CRRENDERGRAPH_H
#define CRUTIL_CRRENDERGRAPH_H

#include <CRUtil/crExport.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crState.h>
#include <CRCore/crLight.h>
#include <CRCore/crStateAttribute.h>
#include <CRUtil/crRenderLeaf.h>

#include <map>
#include <vector>
#include <algorithm>

namespace CRUtil{

struct LeafDepthSortFunctor
{
  bool operator() (const CRCore::ref_ptr<crRenderLeaf>& lhs,const CRCore::ref_ptr<crRenderLeaf>& rhs)
  {
    return (lhs->m_depth>rhs->m_depth);
  }
};

class CRUTIL_EXPORT crRenderGraph : public CRCore::Referenced
{
public:


  typedef std::map< const CRCore::crStateSet*, CRCore::ref_ptr<crRenderGraph> >   ChildList;
  typedef std::vector< CRCore::ref_ptr<crRenderLeaf> >                 LeafList;
  typedef std::vector< CRCore::ref_ptr<crRenderGraph> >                GraphList;

  crRenderGraph*                   m_parent;
  const CRCore::crStateSet*        m_stateset;

  int                              m_depth;
  ChildList                        m_children;
  LeafList                         m_leaves;

  mutable float                    m_averageDistance;
  mutable float                    m_minimumDistance;

  CRCore::ref_ptr<CRCore::Referenced>  m_userData;


  crRenderGraph():
    m_parent(NULL), 
    m_stateset(NULL), 
    m_depth(0),
    m_averageDistance(0),
    m_minimumDistance(0),
    m_userData(NULL)
  {
  }

  crRenderGraph(crRenderGraph* parent,const CRCore::crStateSet* stateset):
    m_parent(parent), 
    m_stateset(stateset),
    m_depth(0),
    m_averageDistance(0),
    m_minimumDistance(0),
    m_userData(NULL)
  {
    if (m_parent) m_depth = m_parent->m_depth + 1;
  }

  ~crRenderGraph() {}

  crRenderGraph* cloneType() const { return new crRenderGraph; }

  void setUserData(Referenced* obj) { m_userData = obj; }
  Referenced* getUserData() { return m_userData.get(); }
  const Referenced* getUserData() const { return m_userData.get(); }

  //const crStateSet* getStateSet() { return m_stateset; }
  //const crStateSet* getStateSet()const { return m_stateset; }

  /** return true if all of drawables, lights and children are empty.*/
  inline bool empty() const
  {
    return m_leaves.empty() && m_children.empty();
  }

  inline bool leaves_empty() const
  {
    return m_leaves.empty();
  }

  inline bool leaves_empty()
  {
	  return m_leaves.empty();
  }

  inline float getAverageDistance() const
  {
    if (m_averageDistance==FLT_MAX && !m_leaves.empty())
    {
      m_averageDistance = 0.0f;
      for(LeafList::const_iterator itr=m_leaves.begin();
        itr!=m_leaves.end();
        ++itr)
      {
        m_averageDistance += (*itr)->m_depth;
      }
      m_averageDistance /= (float)m_leaves.size();

    }
    return m_averageDistance;
  }

  inline float getMinimumDistance() const
  {
    if (m_minimumDistance==FLT_MAX && !m_leaves.empty())
    {
      LeafList::const_iterator itr=m_leaves.begin();
      m_minimumDistance = (*itr)->m_depth;
      ++itr;
      for(;
        itr!=m_leaves.end();
        ++itr)
      {
        if ((*itr)->m_depth<m_minimumDistance) m_minimumDistance=(*itr)->m_depth;
      }

    }
    return m_minimumDistance;
  }

  inline void sortFrontToBack()
  {
    std::sort(m_leaves.begin(),m_leaves.end(),LeafDepthSortFunctor());
  }

  /** reset the internal contents of a crRenderGraph, including deleting all children.*/
  void reset();

  /** recursively clean the crRenderGraph of all its drawables, lights and depths.
  * Leaves children intact, and ready to be populated again.*/
  void clean();

  /** recursively prune the crRenderGraph of empty children.*/
  void prune();
  
 // void prune_optimizer();

  inline crRenderGraph* find_or_insert(const CRCore::crStateSet* stateset, bool detailCompare = false)
  {
 //   // search for the appropriate state group, return it if found.
	//ChildList::iterator itr = m_children.find(stateset);
 //   if (itr!=m_children.end()) return itr->second.get();

 //    //create a state group and insert it into the children list
 //    //then return the state group.
 //   crRenderGraph* sg = new crRenderGraph(this,stateset);
 //   m_children[stateset] = sg;
	//return sg;

	if(!detailCompare)
	{
		CRCore::ref_ptr<crRenderGraph> &sg = m_children[stateset];
		if(!sg.valid()) sg = new crRenderGraph(this,stateset);//createOrReuseRenderGraph(this,stateset);

		return sg.get();
	}
	else
	{
		//try
		//{
		for( ChildList::iterator itr = m_children.begin();
				itr != m_children.end();
				++itr )
		{
			if(itr->first == stateset || itr->first->compare(*stateset,true) == 0)
			{
				return itr->second.get();
			}
		}
		//}
		//catch (...)
		//{
		//    CRCore::notify(CRCore::ALWAYS)<<"crRenderGraph::find_or_insert() error "<<std::endl;
		//}
		crRenderGraph* sg = new crRenderGraph(this,stateset);//createOrReuseRenderGraph(this,stateset);
		m_children[stateset] = sg;

		return sg;
    }
  }

  inline void find_and_remove( const CRCore::crDrawable *drawable )
  {
	  const CRCore::crStateSet* stateset = drawable->getStateSet();
	  LeafList::iterator itr;
	  if(stateset)
	  {
	      CRCore::ref_ptr<crRenderGraph> &sg = m_children[stateset];
          if(sg.valid())
		  {
			  for( itr = sg->m_leaves.begin();
			       itr!=sg->m_leaves.end();
			       ++itr)
			  {
			      if( (*itr)->m_drawable == drawable )
			      {
				      m_leaves.erase(itr);
				      return;
				  }
			  }
		  }
	  }	
	  else
	  {
		  for( itr = m_leaves.begin();
			  itr!=m_leaves.end();
			  ++itr)
		  {
			  if( (*itr)->m_drawable == drawable )
			  {
				  m_leaves.erase(itr);
				  return;
			  }
		  }
	  }
  }

  inline void remove_child( const CRCore::crStateSet* stateset )
  {
      m_children.erase(stateset);  
  }

  /** add a render leaf.*/
  inline void addLeaf(CRCore::ref_ptr<crRenderLeaf>& leaf)
  {
    if (leaf.valid())
    {
      m_averageDistance = FLT_MAX; // signify dirty.
      m_minimumDistance = FLT_MAX; // signify dirty.
      m_leaves.push_back(leaf);
      leaf->m_parent = this;
    }
  }

  static inline void moveRenderGraph(CRCore::crState& state,crRenderGraph* sg_curr,crRenderGraph* sg_new)
  {
	/*static */std::vector< CRCore::ref_ptr<crRenderGraph> > s_return_path;
    if (sg_new==sg_curr || sg_new==NULL) return;
	CRCore::ref_ptr<crRenderGraph> rg;
    if (sg_curr==NULL)
    {
      // use return path to trace back steps to sg_new.
      s_return_path.resize(0);
      // need to pop back root render graph.
      do 
      {
        s_return_path.push_back(sg_new);
        sg_new = sg_new->m_parent;
      } while (sg_new);

      for(std::vector< CRCore::ref_ptr<crRenderGraph> >::reverse_iterator itr=s_return_path.rbegin();
        itr!=s_return_path.rend();
        ++itr)
      {
        rg = (*itr);
        if (rg->m_stateset) state.pushStateSet(rg->m_stateset);
      }
      return;
    }

    // first handle the typical case which is two state groups
    // are neighbours.
    if (sg_curr->m_parent==sg_new->m_parent)
    {
      // state has changed so need to pop old state.
      if (sg_curr->m_stateset) state.popStateSet();
      // and push new state.
      if (sg_new->m_stateset) state.pushStateSet(sg_new->m_stateset);
      return;
    }


    // need to pop back up to the same depth as the new state group.
    while (sg_curr->m_depth>sg_new->m_depth)
    {
      if (sg_curr->m_stateset) state.popStateSet();
      sg_curr = sg_curr->m_parent;
    }

    // use return path to trace back steps to sg_new.
    //std::vector<crRenderGraph*> return_path;
    s_return_path.resize(0);
    // need to pop back up to the same depth as the curr state group.
    while (sg_new->m_depth>sg_curr->m_depth)
    {
      s_return_path.push_back(sg_new);
      sg_new = sg_new->m_parent;
    }

    // now pop back up both parent paths until they agree.

    // DRT - 10/22/02
    // should be this to conform with above case where two crRenderGraph
    // nodes have the same parent
    while (sg_curr && sg_new && sg_curr != sg_new)
    {
      if (sg_curr->m_stateset) state.popStateSet();
      sg_curr = sg_curr->m_parent;

      s_return_path.push_back(sg_new);
      sg_new = sg_new->m_parent;
    }

    for(std::vector< CRCore::ref_ptr<crRenderGraph> >::reverse_iterator itr=s_return_path.rbegin();
      itr!=s_return_path.rend();
      ++itr)
    {
      rg = (*itr);
      if (rg->m_stateset) state.pushStateSet(rg->m_stateset);
    }

  }

  inline static void moveToRootRenderGraph(CRCore::crState& state,crRenderGraph* sg_curr)
  {
    // need to pop back all statesets and matrices.
    while (sg_curr)
    {
      if (sg_curr->m_stateset) state.popStateSet();
      sg_curr = sg_curr->m_parent;
    }

  }

  inline void set(crRenderGraph* parent,const CRCore::crStateSet* stateset)
  {
	  m_parent = parent;
	  m_stateset = stateset;
	  m_depth = m_parent?m_parent->m_depth + 1:0;
	  m_averageDistance = 0.0f;
	  m_minimumDistance = 0.0f;
	  m_userData = NULL;
  }
  //static inline void resetReuseRenderGraph() { s_currentReuseRenderGraphIndex = 0; }
  //static void clearReuseRenderGraph() { s_reuseGraphList.clear(); }
private:

  /// disallow copy construction.
  crRenderGraph(const crRenderGraph&):Referenced() {}
  /// disallow copy operator.
  crRenderGraph& operator = (const crRenderGraph&) { return *this; }

  //void collect();

  //static inline crRenderGraph *createOrReuseRenderGraph(crRenderGraph* parent,const CRCore::crStateSet* stateset)
  //{
	 // while (s_currentReuseRenderGraphIndex<s_reuseGraphList.size() && 
		//  s_reuseGraphList[s_currentReuseRenderGraphIndex]->referenceCount()>1)
	 // {
		//  ++s_currentReuseRenderGraphIndex;
	 // }
	 // if (s_currentReuseRenderGraphIndex<s_reuseGraphList.size())
	 // {
		//  crRenderGraph* renderGraph = s_reuseGraphList[s_currentReuseRenderGraphIndex++].get();
		//  renderGraph->set(parent,stateset);
		//  return renderGraph;
	 // }

	 // crRenderGraph* renderGraph = new crRenderGraph(parent,stateset);
	 // s_reuseGraphList.push_back(renderGraph);
	 // ++s_currentReuseRenderGraphIndex;
	 // return renderGraph;
  //}

  ////typedef std::vector<const CRCore::crStateSet*> ToEraseList;
  ////static ToEraseList s_toEraseList;
  //static GraphList s_reuseGraphList;
  //static int s_currentReuseRenderGraphIndex;
};
}

#endif