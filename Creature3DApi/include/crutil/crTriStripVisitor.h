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
#ifndef CRUTIL_CRTRISTRIPVISITOR_H
#define CRUTIL_CRTRISTRIPVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>

#include <CRUtil/crOptimizer.h>

#include <set>

namespace CRUtil {

/** A tri stripping visitor for converting crGeometry surface primitives into tri strips.
  * The current implemention is based up Tanguy Fautre's triangulation code.
  */
class CRUTIL_EXPORT crTriStripVisitor : public CRCore::crNodeVisitor
{
    public:

        /// default to traversing all children.
        crTriStripVisitor(crOptimizer* optimizer=0) : 
                CRCore::crNodeVisitor( CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN ), 
                m_cacheSize( 16 ),
                m_minStripSize( 2 ),
				m_generateFourPointPrimitivesQuads ( false)
        {}

        /** Convert mesh primitives in crGeometry into Tri Strips.
          * Converts all primitive types except points
          * and lines, linestrips which it leaves unchanged.
          */
        void stripify(CRCore::crGeometry& drawable);

        /** Stripfy the accumulated list of crGeometry drawables.*/
        void stripify();

        /// accumulate the crGeometry drawables to stripify
        virtual void apply(CRCore::crObject& geode);

        inline void setCacheSize( unsigned int size )
        {
            m_cacheSize = size;
        }
        
        inline unsigned int getCacheSize()
        {
            return m_cacheSize;
        }

        inline const unsigned int getCacheSize() const
        {
            return m_cacheSize;
        }
        
        inline void setMinStripSize( unsigned int size )
        {
            m_minStripSize = size;
        }
        
        inline unsigned int getMinStripSize()
        {
            return m_minStripSize;
        }

        inline const unsigned int getMinStripSize() const
        {
            return m_minStripSize;
        }
        
        inline bool isOperationPermissableForObject(const CRCore::crBase* base)
        {
            return m_optimizer ? m_optimizer->isOperationPermissableForObject(base,CRUtil::crOptimizer::TRISTRIP_GEOMETRY) : true; 
        }

		void setGenerateFourPointPrimitivesQuads(bool flag) { m_generateFourPointPrimitivesQuads = flag; }
		bool getGenerateFourPointPrimitivesQuads() const { return m_generateFourPointPrimitivesQuads; }

    private:
    
        typedef std::set<CRCore::crGeometry*> GeometryList;

        crOptimizer*   m_optimizer;
        unsigned int m_cacheSize;
        unsigned int m_minStripSize;
        GeometryList m_geometryList;
		bool         m_generateFourPointPrimitivesQuads;
};

}

#endif
