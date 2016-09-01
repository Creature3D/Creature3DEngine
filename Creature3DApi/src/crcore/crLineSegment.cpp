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
#include <CRCore/crLineSegment.h>

using namespace CRCore;

bool crLineSegment::intersectAndClip(crVector3& s,crVector3& e,const crBoundingBox& bb)
{
    // compate s and e against the xMin to xMax range of bb.
    if (s.x()<=e.x())
    {

        // trivial reject of segment wholely outside.
        if (e.x()<bb.xMin()) return false;
        if (s.x()>bb.xMax()) return false;

        if (s.x()<bb.xMin())
        {
            // clip s to xMin.
            s = s+(e-s)*(bb.xMin()-s.x())/(e.x()-s.x());
        }

        if (e.x()>bb.xMax())
        {
            // clip e to xMax.
            e = s+(e-s)*(bb.xMax()-s.x())/(e.x()-s.x());
        }
    }
    else
    {
        if (s.x()<bb.xMin()) return false;
        if (e.x()>bb.xMax()) return false;

        if (e.x()<bb.xMin())
        {
            // clip s to xMin.
            e = s+(e-s)*(bb.xMin()-s.x())/(e.x()-s.x());
        }

        if (s.x()>bb.xMax())
        {
            // clip e to xMax.
            s = s+(e-s)*(bb.xMax()-s.x())/(e.x()-s.x());
        }
    }

    // compate s and e against the yMin to yMax range of bb.
    if (s.y()<=e.y())
    {

        // trivial reject of segment wholely outside.
        if (e.y()<bb.yMin()) return false;
        if (s.y()>bb.yMax()) return false;

        if (s.y()<bb.yMin())
        {
            // clip s to yMin.
            s = s+(e-s)*(bb.yMin()-s.y())/(e.y()-s.y());
        }

        if (e.y()>bb.yMax())
        {
            // clip e to yMax.
            e = s+(e-s)*(bb.yMax()-s.y())/(e.y()-s.y());
        }
    }
    else
    {
        if (s.y()<bb.yMin()) return false;
        if (e.y()>bb.yMax()) return false;

        if (e.y()<bb.yMin())
        {
            // clip s to yMin.
            e = s+(e-s)*(bb.yMin()-s.y())/(e.y()-s.y());
        }

        if (s.y()>bb.yMax())
        {
            // clip e to yMax.
            s = s+(e-s)*(bb.yMax()-s.y())/(e.y()-s.y());
        }
    }

    // compate s and e against the zMin to zMax range of bb.
    if (s.z()<=e.z())
    {

        // trivial reject of segment wholely outside.
        if (e.z()<bb.zMin()) return false;
        if (s.z()>bb.zMax()) return false;

        if (s.z()<bb.zMin())
        {
            // clip s to zMin.
            s = s+(e-s)*(bb.zMin()-s.z())/(e.z()-s.z());
        }

        if (e.z()>bb.zMax())
        {
            // clip e to zMax.
            e = s+(e-s)*(bb.zMax()-s.z())/(e.z()-s.z());
        }
    }
    else
    {
        if (s.z()<bb.zMin()) return false;
        if (e.z()>bb.zMax()) return false;

        if (e.z()<bb.zMin())
        {
            // clip s to zMin.
            e = s+(e-s)*(bb.zMin()-s.z())/(e.z()-s.z());
        }

        if (s.z()>bb.zMax())
        {
            // clip e to zMax.
            s = s+(e-s)*(bb.zMax()-s.z())/(e.z()-s.z());
        }
    }

    return true;
}


bool crLineSegment::intersect(const crBoundingBox& bb) const
{
    if (!bb.valid()) return false;

    crVector3 s=m_s,e=m_e;
    return intersectAndClip(s,e,bb);
}


bool crLineSegment::intersect(const crBoundingBox& bb,float& r1,float& r2) const
{
    if (!bb.valid()) return false;

    crVector3 s=m_s,e=m_e;
    bool result = intersectAndClip(s,e,bb);
    if (result)
    {
        float len = (m_e-m_s).length();
        if (len>0.0f)
        {
            float inv_len = 1.0f/len;
            r1 = (s-m_s).length()*inv_len;
            r2 = (e-m_e).length()*inv_len;
        }
        else
        {
            r1 = 0.0f;
            r2 = 0.0f;
        }
    }
    return result;
}


bool crLineSegment::intersect(const crBoundingSphere& bs,float& r1,float& r2) const
{
    crVector3 sm = m_s-bs.m_center;
    float c = sm.length2()-bs.m_fRadius*bs.m_fRadius;

    crVector3 se = m_e-m_s;
    float a = se.length2();

    float b = sm*se*2.0f;

    float d = b*b-4.0f*a*c;

    if (d<0.0f) return false;

    d = sqrtf(d);

    float div = 1.0f/(2.0f*a);

    r1 = (-b-d)*div;
    r2 = (-b+d)*div;

    if (r1<=0.0f && r2<=0.0f) return false;

    if (r1>=1.0f && r2>=1.0f) return false;

    return true;
}


bool crLineSegment::intersect(const crBoundingSphere& bs) const
{
    crVector3 sm = m_s-bs.m_center;
    float c = sm.length2()-bs.m_fRadius*bs.m_fRadius;
    if (c<0.0f) return true;

    crVector3 se = m_e-m_s;
    float a = se.length2();

    float b = (sm*se)*2.0f;

    float d = b*b-4.0f*a*c;

    if (d<0.0f) return false;

    d = sqrtf(d);

    float div = 1.0f/(2.0f*a);

    float r1 = (-b-d)*div;
    float r2 = (-b+d)*div;

    if (r1<=0.0f && r2<=0.0f) return false;

    if (r1>=1.0f && r2>=1.0f) return false;

    return true;
}


bool crLineSegment::intersect(const crVector3& v1,const crVector3& v2,const crVector3& v3,float& r)
{
    if (v1==v2 || v2==v3 || v1==v3) return false;

    crVector3 vse = m_e-m_s;

    crVector3 v12 = v2-v1;
    crVector3 n12 = v12^vse;
    float ds12 = (m_s-v1)*n12;
    float d312 = (v3-v1)*n12;
    if (d312>=0.0f)
    {
        if (ds12<0.0f) return false;
        if (ds12>d312) return false;
    }
    else                         // d312 < 0
    {
        if (ds12>0.0f) return false;
        if (ds12<d312) return false;
    }

    crVector3 v23 = v3-v2;
    crVector3 n23 = v23^vse;
    float ds23 = (m_s-v2)*n23;
    float d123 = (v1-v2)*n23;
    if (d123>=0.0f)
    {
        if (ds23<0.0f) return false;
        if (ds23>d123) return false;
    }
    else                         // d123 < 0
    {
        if (ds23>0.0f) return false;
        if (ds23<d123) return false;
    }

    crVector3 v31 = v1-v3;
    crVector3 n31 = v31^vse;
    float ds31 = (m_s-v3)*n31;
    float d231 = (v2-v3)*n31;
    if (d231>=0.0f)
    {
        if (ds31<0.0f) return false;
        if (ds31>d231) return false;
    }
    else                         // d231 < 0
    {
        if (ds31>0.0f) return false;
        if (ds31<d231) return false;
    }

    float r3 = ds12/d312;
    float r1 = ds23/d123;
    float r2 = ds31/d231;

    //    float rt = r1+r2+r3;

    crVector3 in = v1*r1+v2*r2+v3*r3;

    float length = vse.length();
    vse /= length;
    float d = (in-m_s)*vse;

    if (d<0.0f) return false;
    if (d>length) return false;

    r = d/length;

    return true;
}
