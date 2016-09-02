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
#ifndef CRCORE_CRPOLYTOPE
#define CRCORE_CRPOLYTOPE 1

#include <CRCore/crPlane.h>
#include <CRCore/fast_back_stack.h>

namespace CRCore {


/** A crPolytope class for representing convex clipping volumes made up.
  * When adding planes, their normals should point inwards (into the volume) */
class CR_EXPORT crPolytope
{

    public:

        typedef unsigned int                    ClippingMask;
        typedef std::vector<crPlane>            PlaneList;
        typedef std::vector<crVector3>          VertexList;
        typedef fast_back_stack<ClippingMask>   MaskStack;

        inline crPolytope() {setupMask();}

        inline crPolytope(const crPolytope& cv) : 
            m_maskStack(cv.m_maskStack),
            m_resultMask(cv.m_resultMask),
            m_planeList(cv.m_planeList),
            m_referenceVertexList(cv.m_referenceVertexList) {}
        
        inline crPolytope(const PlaneList& pl) : m_planeList(pl) {setupMask();}

        inline ~crPolytope() {}

        inline void clear() { m_planeList.clear(); setupMask(); }

        inline crPolytope& operator = (const crPolytope& cv)
        {
            if (&cv==this) return *this;
            m_maskStack = cv.m_maskStack;
            m_resultMask = cv.m_resultMask;
            m_planeList = cv.m_planeList;
            m_referenceVertexList = cv.m_referenceVertexList;
            return *this;
        }
        
        /** Create a crPolytope with is cube, centered at 0,0,0, with sides of 2 units.*/
        void setToUnitFrustum(bool withNear=true, bool withFar=true)
        {
            m_planeList.erase(m_planeList.begin(),m_planeList.end());
            m_planeList.push_back(crPlane(1.0f,0.0f,0.0f,1.0f)); // left plane.
            m_planeList.push_back(crPlane(-1.0f,0.0f,0.0f,1.0f)); // right plane.
            m_planeList.push_back(crPlane(0.0f,1.0f,0.0f,1.0f)); // bottom plane.
            m_planeList.push_back(crPlane(0.0f,-1.0f,0.0f,1.0f)); // top plane.
            if (withNear) m_planeList.push_back(crPlane(0.0f,0.0f,-1.0f,1.0f)); // near plane
            if (withFar) m_planeList.push_back(crPlane(0.0f,0.0f,1.0f,1.0f)); // far plane
            setupMask();
        }

        inline void setAndTransformProvidingInverse(const crPolytope& pt, const CRCore::crMatrix& matrix)
        {
            m_referenceVertexList = pt.m_referenceVertexList;

            unsigned int resultMask = pt.m_maskStack.back();
            if (resultMask==0)
            {
                m_maskStack.back() = 0;
                m_resultMask = 0;
                m_planeList.clear();
                return;
            }
            
            ClippingMask selector_mask = 0x1;

            unsigned int numActivePlanes = 0;

            // count number active planes.
            PlaneList::const_iterator itr;
            for(itr=pt.m_planeList.begin();
                itr!=pt.m_planeList.end();
                ++itr)
            {
                if (resultMask&selector_mask) ++numActivePlanes;
                selector_mask <<= 1;
            }
            
            m_planeList.resize(numActivePlanes);
            m_resultMask = 0;
            selector_mask = 0x1;
            unsigned int index = 0;
            for(itr=pt.m_planeList.begin();
                itr!=pt.m_planeList.end();
                ++itr)
            {
                if (resultMask&selector_mask)
                {
                     m_planeList[index] = *itr;
                     m_planeList[index++].transformProvidingInverse(matrix);
                    m_resultMask = (m_resultMask<<1) | 1;
                }
                selector_mask <<= 1;
            }

            m_maskStack.back() = m_resultMask;
        }

        inline void set(const PlaneList& pl) { m_planeList = pl; setupMask(); }
        
        
        inline void add(const CRCore::crPlane& pl) { m_planeList.push_back(pl); setupMask(); }

        /** flip/reverse the orientation of all the planes.*/
        inline void flip()
        {
            for(PlaneList::iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                itr->flip();
            }
        }


        inline PlaneList& getPlaneList() { return m_planeList; }

        inline const PlaneList& getPlaneList() const { return m_planeList; }


        inline void setReferenceVertexList(VertexList& vertices) { m_referenceVertexList=vertices; }

        inline VertexList& getReferenceVertexList() { return m_referenceVertexList; }
        
        inline const VertexList& getReferenceVertexList() const { return m_referenceVertexList; }
		
		inline void addVertex( const crVector3 &v ) { m_referenceVertexList.push_back(v); }


        inline void setupMask()
        {
            m_resultMask = 0;
            for(unsigned int i=0;i<m_planeList.size();++i)
            {
                m_resultMask = (m_resultMask<<1) | 1;
            }
            m_maskStack.back() = m_resultMask;
        }

        inline ClippingMask& getCurrentMask() { return m_maskStack.back(); }

        inline ClippingMask getCurrentMask() const { return m_maskStack.back(); }

        inline void setResultMask(ClippingMask mask) { m_resultMask=mask; }

        inline ClippingMask getResultMask() const { return m_resultMask; }
        
        MaskStack& getMaskStack() { return m_maskStack; }
        
        const MaskStack& getMaskStack() const { return m_maskStack; }
        
        
        inline void pushCurrentMask()
        {
            m_maskStack.push_back(m_resultMask);
        }

        inline void popCurrentMask()
        {
            m_maskStack.pop_back();
        }

        /** Check whether a vertex is contained with clipping set.*/
        inline bool contains(const CRCore::crVector3& v) const
        {
            if (!m_maskStack.back()) return true;
            
            unsigned int selector_mask = 0x1;
            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if ((m_maskStack.back()&selector_mask) && (itr->distance(v)<0.0f)) return false;
                selector_mask <<= 1;
            }
            return true;
        }

        /** Check whether any part of vertex list is contained with clipping set.*/
        inline bool contains(const std::vector<crVector3>& vertices)
        {
            if (!m_maskStack.back()) return true;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;

            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(vertices);
                    if (res<0) return false; // outside clipping set.
                    else if (res>0) m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }

        /** Check whether any part of a bounding sphere is contained within clipping set.
            Using a mask to determine which planes should be used for the check, and
            modifying the mask to turn off planes which wouldn't contribute to clipping
            of any internal objects.  This feature is used in CRCoreUtil::CullVisitor
            to prevent redundant plane checking.*/
        inline bool contains(const CRCore::crBoundingSphere& bs)
        {
            if (!m_maskStack.back()) return true;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;

            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(bs);
                    if (res<0) return false; // outside clipping set.
                    else if (res>0) m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }
        
        /** Check whether any part of a bounding box is contained within clipping set.
            Using a mask to determine which planes should be used for the check, and
            modifying the mask to turn off planes which wouldn't contribute to clipping
            of any internal objects.  This feature is used in CRCoreUtil::CullVisitor
            to prevent redundant plane checking.*/
        inline bool contains(const CRCore::crBoundingBox& bb)
        {
            if (!m_maskStack.back()) return true;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;

            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(bb);
                    if (res<0) return false; // outside clipping set.
                    else if (res>0) m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }

        /** Check whether all of vertex list is contained with clipping set.*/
        inline bool containsAllOf(const std::vector<crVector3>& vertices)
        {
            if (!m_maskStack.back()) return false;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;

            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(vertices);
                    if (res<1) return false;  // intersects, or is below plane.
                    m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }

        /** Check whether the entire bounding sphere is contained within clipping set.*/
        inline bool containsAllOf(const CRCore::crBoundingSphere& bs)
        {
            if (!m_maskStack.back()) return false;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;
            
            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(bs);
                    if (res<1) return false;  // intersects, or is below plane.
                    m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }
        
        /** Check whether the entire bounding box is contained within clipping set.*/
        inline bool containsAllOf(const CRCore::crBoundingBox& bb)
        {
            if (!m_maskStack.back()) return false;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;

            for(PlaneList::const_iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    int res=itr->intersect(bb);
                    if (res<1) return false;  // intersects, or is below plane.
                    m_resultMask ^= selector_mask; // subsequent checks against this plane not required.
                }
                selector_mask <<= 1; 
            }
            return true;
        }

        
        /** Transform the clipping set by matrix.  Note, this operations carries out
          * the calculation of the inverse of the matrix since to transforms
          * planes must be multiplied my the inverse transposed. This
          * make this operation expensive.  If the inverse has been already
          * calculated elsewhere then use transformProvidingInverse() instead.
          * See http://www.worldserver.com/turk/computergraphics/NormalTransformations.pdf*/
        inline void transform(const CRCore::crMatrix& matrix)
        {
            CRCore::crMatrix inverse;
            inverse.invert(matrix);
            transformProvidingInverse(inverse);
        }
        
        /** Transform the clipping set by provide a pre inverted matrix.
          * see transform for details. */
        inline void transformProvidingInverse(const CRCore::crMatrix& matrix)
        {
            if (!m_maskStack.back()) return;

            m_resultMask = m_maskStack.back();
            ClippingMask selector_mask = 0x1;
            for(PlaneList::iterator itr=m_planeList.begin();
                itr!=m_planeList.end();
                ++itr)
            {
                if (m_resultMask&selector_mask)
                {
                    itr->transformProvidingInverse(matrix);
                    selector_mask <<= 1; 
                }
            }
        }
        
    protected:


        MaskStack                           m_maskStack;
        ClippingMask                        m_resultMask;
        PlaneList                           m_planeList;
        VertexList                          m_referenceVertexList;

};

}	// end of namespace

#endif 
