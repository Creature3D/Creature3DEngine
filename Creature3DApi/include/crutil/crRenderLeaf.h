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
#ifndef CRUTIL_CRRENDERLEAF_H
#define CRUTIL_CRRENDERLEAF_H 1

#include <CRUtil/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crState.h>

#include <CRUtil/crExport.h>

namespace CRUtil {

// forward declare crRenderGraph
class crRenderGraph;

/** container class for all data required for rendering of drawables.
  */
class CRUTIL_EXPORT crRenderLeaf : public CRCore::Referenced
{
    public:
    

		inline crRenderLeaf(CRCore::crDrawable* drawable,CRCore::RefMatrix* projection,CRCore::RefMatrix* modelview, float depth=0.0f,CRCore::crNode *needUnLockNode = 0):
            m_parent(0),
            m_drawable(drawable),
            m_projection(projection),
            m_modelview(modelview),
            m_depth(depth),
			m_needUnlockNode(needUnLockNode){}

        
	inline void set(CRCore::crDrawable* drawable,CRCore::RefMatrix* projection,CRCore::RefMatrix* modelview, float depth=0.0f,CRCore::crNode *needUnLockNode = 0)
	{
	    m_parent = 0;
	    m_drawable = drawable;
        m_projection = projection,
        m_modelview = modelview,
	    m_depth = depth;
		m_needUnlockNode = needUnLockNode;
	}
	
	//inline void reset()
	//{
	//    m_parent = 0;
	//    m_drawable = 0;
	//    m_projection = 0;
	//    m_modelview = 0;
	//    m_depth = 0.0f;
 //       m_needUnlockNode = 0;
	//}

	
     virtual void render(CRCore::crState& state,crRenderLeaf* previous);
        
     /// allow crRenderGraph to change the crRenderLeaf's m_parent.
     friend class CRUtil::crRenderGraph;

	 //crRenderGraph* getParent() { return m_parent; }
	 //crRenderGraph* getParent() const { return m_parent; }
	 //CRCore::crDrawable* getDrawable() { return m_drawable; }
	 //CRCore::crDrawable* getDrawable() const { return m_drawable; }

	 virtual void renderZprePass(CRCore::crState& state,crRenderLeaf* previous);
    public:
    
        crRenderGraph*                    m_parent;
        mutable CRCore::ref_ptr<CRCore::crDrawable>       m_drawable;
        CRCore::ref_ptr<CRCore::RefMatrix>    m_projection;
        CRCore::ref_ptr<CRCore::RefMatrix>    m_modelview;
        float                           m_depth;

    private:

        /// disallow creation of blank crRenderLeaf as this isn't useful.
        crRenderLeaf():
            m_parent(0),
            m_drawable(0),
            m_projection(0),
            m_modelview(0),
            m_depth(0.0f),
			m_needUnlockNode(0){}

        /// disallow copy construction.
        crRenderLeaf(const crRenderLeaf&):CRCore::Referenced() {}
        /// disallow copy operator.
        crRenderLeaf& operator = (const crRenderLeaf&) { return *this; }
		CRCore::ref_ptr<CRCore::crNode> m_needUnlockNode;

};

}

#endif
