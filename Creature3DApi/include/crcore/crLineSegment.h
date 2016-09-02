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
#ifndef CRCORE_CRINESEGMENT_H
#define CRCORE_CRINESEGMENT_H 1

#include <CRCore/crMatrix.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crBoundingSphere.h>

namespace CRCore {

/** crLineSegment class for representing a line segment.*/
class CR_EXPORT crLineSegment : public Referenced
{
    public:

        crLineSegment() {};
        crLineSegment(const crLineSegment& seg) : Referenced(),m_s(seg.m_s),m_e(seg.m_e) {}
        crLineSegment(const crVector3& s,const crVector3& e) : m_s(s),m_e(e) {}

        crLineSegment& operator = (const crLineSegment& seg) { m_s = seg.m_s;  m_e = seg.m_e; return *this; }

        inline void set(const crVector3& s,const crVector3& e) { m_s=s; m_e=e; }
        
        inline crVector3& start() { return m_s; }
        inline const crVector3& start() const { return m_s; }

        inline crVector3& end() { return m_e; }
        inline const crVector3& end() const { return m_e; }

        inline bool valid() const { return m_s.valid() && m_e.valid() && m_s!=m_e; }

        /** return true if segment intersects crBoundingBox.*/
        bool intersect(const crBoundingBox& bb) const;

        /** return true if segment intersects crBoundingBox and return the intersection ratio's.*/
        bool intersect(const crBoundingBox& bb,float& r1,float& r2) const;

        /** return true if segment intersects crBoundingSphere.*/
        bool intersect(const crBoundingSphere& bs) const;

        /** return true if segment intersects crBoundingSphere and return the intersection ratio's.*/
        bool intersect(const crBoundingSphere& bs,float& r1,float& r2) const;
        
        /** return true if segment intersects triangle and set ratio long segment. */
        bool intersect(const crVector3& v1,const crVector3& v2,const crVector3& v3,float& r);

        /** post multiply a segment by matrix.*/
        inline void mult(const crLineSegment& seg,const crMatrix& m) { m_s = seg.m_s*m; m_e = seg.m_e*m; }
        /** pre multiply a segment by matrix.*/
        inline void mult(const crMatrix& m,const crLineSegment& seg) { m_s = m*seg.m_s; m_e = m*seg.m_e; }

    protected:

        virtual ~crLineSegment() {}
        
        static bool intersectAndClip(crVector3& s,crVector3& e,const crBoundingBox& bb);

        crVector3 m_s;
        crVector3 m_e;
};

}

#endif
