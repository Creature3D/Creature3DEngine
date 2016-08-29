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
#ifndef CRCORE_CRPLANE_H
#define CRCORE_CRPLANE_H 1

#include <CRCore/crExport.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBoundingBox.h>

#include <vector>

namespace CRCore {

/** A plane class. It can be used to represent an infinite plane.*/
class CR_EXPORT crPlane
{

    public:

	inline crPlane():m_fv(0.0f,0.0f,0.0f,0.0f) { m_lowerBBCorner = 0; m_upperBBCorner = 0; }
	inline crPlane(const crPlane& pl):m_fv(pl.m_fv) { calculateUpperLowerBBCorners(); }
	inline crPlane(float a,float b,float c,float d):m_fv(a,b,c,d) { calculateUpperLowerBBCorners(); }
	inline crPlane(const crVector4& vec):m_fv(vec) { calculateUpperLowerBBCorners(); }
	inline crPlane(const crVector3& norm,float d):m_fv(norm[0],norm[1],norm[2],d) { calculateUpperLowerBBCorners(); }
    inline crPlane(const crVector3& v1, const crVector3& v2, const crVector3& v3) { set(v1,v2,v3); }
	inline crPlane(const crVector3& norm, const crVector3& point) { set(norm,point); }

	virtual ~crPlane(){}

	inline crPlane& operator = (const crPlane& pl)
    {
        if (&pl==this) return *this;
        m_fv = pl.m_fv;
        m_lowerBBCorner = pl.m_lowerBBCorner;
        m_upperBBCorner = pl.m_upperBBCorner;
        return *this;
     }

	inline void set(const crPlane& pl) { m_fv = pl.m_fv; calculateUpperLowerBBCorners(); }
	inline void set(float a,float b,float c,float d) { m_fv.set(a,b,c,d); calculateUpperLowerBBCorners(); }
	inline void set(const crVector4& vec) { m_fv = vec; calculateUpperLowerBBCorners(); }
	inline void set(const crVector3& norm,float d) { m_fv.set(norm[0],norm[1],norm[2],d); calculateUpperLowerBBCorners(); }
    inline void set(const crVector3& v1, const crVector3& v2, const crVector3& v3)
    {
        CRCore::crVector3 norm = (v2-v1)^(v3-v2);
        float length = norm.length();
        if (length>1e-6) norm/= length;
        else norm.set(0.0f,0.0f,0.0f);
        m_fv.set(norm[0],norm[1],norm[2],-(v1*norm));
        calculateUpperLowerBBCorners();
    }
        
    inline void set(const crVector3& norm, const crVector3& point)
    {
        float d = -norm[0]*point[0] - norm[1]*point[1] - norm[2]*point[2];
        m_fv.set(norm[0],norm[1],norm[2],d);
        calculateUpperLowerBBCorners();
    }

       /** flip/reverse the orientation of the plane.*/
    inline void flip()
    {
        m_fv = -m_fv;
        calculateUpperLowerBBCorners();
    }


    inline void makeUnitLength()
    {
        float length = sqrtf(m_fv[0]*m_fv[0] + m_fv[1]*m_fv[1]+ m_fv[2]*m_fv[2]);
        m_fv /= length;
    }

    /** calculate the upper and lower bounding box corners to be used
      * in the intersect(crBoundingBox&) method for speeding calculations.*/
    inline void calculateUpperLowerBBCorners()
    {
        m_upperBBCorner = (m_fv.x()>=0.0f?1:0) |
                         (m_fv.y()>=0.0f?2:0) |
                         (m_fv.z()>=0.0f?4:0);

        m_lowerBBCorner = (~m_upperBBCorner)&7;
    }

    inline bool valid() const { return m_fv.valid(); }

    inline bool operator == (const crPlane& plane) const { return m_fv==plane.m_fv;  }
    inline bool operator != (const crPlane& plane) const { return m_fv!=plane.m_fv; }
    inline bool operator <  (const crPlane& plane) const { return m_fv<plane.m_fv; }

	inline float* ptr() { return m_fv.ptr(); }
	inline const float* ptr() const { return m_fv.ptr(); }

	inline crVector4& asVec4() { return m_fv; }

	inline const crVector4& asVec4() const { return m_fv; }
       
    inline float& operator [] (unsigned int i) { return m_fv[i]; }
    inline float  operator [] (unsigned int i) const { return m_fv[i]; }
        
       
    inline CRCore::crVector3 getNormal() const { return CRCore::crVector3(m_fv[0],m_fv[1],m_fv[2]); }
     
             /** calculate the distance between a point and the plane.*/
    inline float distance(const CRCore::crVector3& v) const
    {
        return m_fv[0]*v.x()+
               m_fv[1]*v.y()+
               m_fv[2]*v.z()+
               m_fv[3];
    }

    /** intersection test between plane and vertex list
        return 1 if the bs is completely above plane,
        return 0 if the bs intersects the plane,
        return -1 if the bs is completely below the plane.*/
    inline int intersect(const std::vector<crVector3>& vertices) const
    {
        if (vertices.empty()) return -1;
       
        int noAbove = 0;
        int noBelow = 0;
        int noOn = 0;
        for(std::vector<crVector3>::const_iterator itr=vertices.begin();
            itr != vertices.end();
            ++itr)
        {
            float d = distance(*itr);
            if (d>0.0f) ++noAbove;
            else if (d<0.0f) ++noBelow;
            else ++noOn;
        }
           
        if (noAbove>0)
        {
            if (noBelow>0) return 0;
            else return 1;
        }
        return -1; // treat points on line as outside...
    }

    /** intersection test between plane and bounding sphere.
        return 1 if the bs is completely above plane,
        return 0 if the bs intersects the plane,
        return -1 if the bs is completely below the plane.*/
    inline int intersect(const crBoundingSphere& bs) const
    {
        float d = distance(bs.center());
                      
        if (d>bs.radius()) return 1;
        else if (d<-bs.radius()) return -1;
        else return 0;
    }
        
        
     /** intersection test between plane and bounding sphere.
         return 1 if the bs is completely above plane,
         return 0 if the bs intersects the plane,
         return -1 if the bs is completely below the plane.*/
    inline int intersect(const crBoundingBox& bb) const
    {
         // if lowest point above plane than all above.
        if (distance(bb.corner(m_lowerBBCorner))>0.0f) return 1;
            
        // if highest point is below plane then all below.
        if (distance(bb.corner(m_upperBBCorner))<0.0f) return -1;
           
        // d_lower<=0.0f && d_upper>=0.0f
        // therefore must be crossing plane.
        return 0;   
    }
       /** Transform the plane by matrix.  Note, this operations carries out
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
       
    /** Transform the plane by provide a pre inverted matrix.
      * see transform for details. */
    inline void transformProvidingInverse(const CRCore::crMatrix& matrix)
    {
        // note pre multiplications, which effectively transposes matrix.
        m_fv = matrix * m_fv;
        makeUnitLength();
        calculateUpperLowerBBCorners();
    }

    friend inline std::ostream& operator << (std::ostream& output, const crPlane& pl);

  //      inline void normalize()
		//{
		//	float l = getNormal().length();
  //          
		//	float invLen = 1.0f / length(normal);
		//	normal *= invLen;
		//	offset = w * invLen;
		//}
protected:

    crVector4           m_fv;
        
    // variables cached to optimize calcs against bounding boxes.
    unsigned int        m_upperBBCorner;
    unsigned int        m_lowerBBCorner;


};

inline std::ostream& operator << (std::ostream& output, const crPlane& pl)
{
    output << pl.m_fv[0] << " "
	   << pl.m_fv[1] << " "
	   << pl.m_fv[2] << " "
	   << pl.m_fv[3];
    return output; 	// to enable cascading
}

#define FRUSTUM_LEFT   0
#define FRUSTUM_RIGHT  1
#define FRUSTUM_TOP    2
#define FRUSTUM_BOTTOM 3
#define FRUSTUM_FAR    4
#define FRUSTUM_NEAR   5

/** 6-planes view frustum class */
class CR_EXPORT crFrustum 
{
public:
	inline void loadFrustum(const crMatrixd &mvp)
	{
		planes[FRUSTUM_LEFT  ] = crPlane(mvp[12] - mvp[0], mvp[13] - mvp[1], mvp[14] - mvp[2],  mvp[15] - mvp[3]);
		planes[FRUSTUM_RIGHT ] = crPlane(mvp[12] + mvp[0], mvp[13] + mvp[1], mvp[14] + mvp[2],  mvp[15] + mvp[3]);

		planes[FRUSTUM_TOP   ] = crPlane(mvp[12] - mvp[4], mvp[13] - mvp[5], mvp[14] - mvp[6],  mvp[15] - mvp[7]);
		planes[FRUSTUM_BOTTOM] = crPlane(mvp[12] + mvp[4], mvp[13] + mvp[5], mvp[14] + mvp[6],  mvp[15] + mvp[7]);

		planes[FRUSTUM_FAR   ] = crPlane(mvp[12] - mvp[8], mvp[13] - mvp[9], mvp[14] - mvp[10], mvp[15] - mvp[11]);
		planes[FRUSTUM_NEAR  ] = crPlane(mvp[12] + mvp[8], mvp[13] + mvp[9], mvp[14] + mvp[10], mvp[15] + mvp[11]);

		//planes[FRUSTUM_LEFT  ] = crPlane(mvp(3,0) - mvp(0,0), mvp(3,1) - mvp(0,1), mvp(3,2) - mvp(0,2), mvp(3,3) - mvp(0,3));
		//planes[FRUSTUM_RIGHT ] = crPlane(mvp(3,0) + mvp(0,0), mvp(3,1) + mvp(0,1), mvp(3,2) + mvp(0,2), mvp(3,3) + mvp(0,3));

		//planes[FRUSTUM_TOP   ] = crPlane(mvp(3,0) - mvp(1,0), mvp(3,1) - mvp(1,1), mvp(3,2) - mvp(1,2), mvp(3,3) - mvp(1,3));
		//planes[FRUSTUM_BOTTOM] = crPlane(mvp(3,0) + mvp(1,0), mvp(3,1) + mvp(1,1), mvp(3,2) + mvp(1,2), mvp(3,3) + mvp(1,3));

		//planes[FRUSTUM_FAR   ] = crPlane(mvp(3,0) - mvp(2,0), mvp(3,1) - mvp(2,1), mvp(3,2) - mvp(2,2), mvp(3,3) - mvp(2,3));
		//planes[FRUSTUM_NEAR  ] = crPlane(mvp(3,0) + mvp(2,0), mvp(3,1) + mvp(2,1), mvp(3,2) + mvp(2,2), mvp(3,3) + mvp(2,3));

		for(int i = 0; i<6; i++)
		{
            planes[i].makeUnitLength();
		}
	}

	void loadFrustum(const crMatrixf &mvp)
	{
		planes[FRUSTUM_LEFT  ] = crPlane(mvp[12] - mvp[0], mvp[13] - mvp[1], mvp[14] - mvp[2],  mvp[15] - mvp[3]);
		planes[FRUSTUM_RIGHT ] = crPlane(mvp[12] + mvp[0], mvp[13] + mvp[1], mvp[14] + mvp[2],  mvp[15] + mvp[3]);

		planes[FRUSTUM_TOP   ] = crPlane(mvp[12] - mvp[4], mvp[13] - mvp[5], mvp[14] - mvp[6],  mvp[15] - mvp[7]);
		planes[FRUSTUM_BOTTOM] = crPlane(mvp[12] + mvp[4], mvp[13] + mvp[5], mvp[14] + mvp[6],  mvp[15] + mvp[7]);

		planes[FRUSTUM_FAR   ] = crPlane(mvp[12] - mvp[8], mvp[13] - mvp[9], mvp[14] - mvp[10], mvp[15] - mvp[11]);
		planes[FRUSTUM_NEAR  ] = crPlane(mvp[12] + mvp[8], mvp[13] + mvp[9], mvp[14] + mvp[10], mvp[15] + mvp[11]);

		//planes[FRUSTUM_LEFT  ] = crPlane(mvp(3,0) - mvp(0,0), mvp(3,1) - mvp(0,1), mvp(3,2) - mvp(0,2), mvp(3,3) - mvp(0,3));
		//planes[FRUSTUM_RIGHT ] = crPlane(mvp(3,0) + mvp(0,0), mvp(3,1) + mvp(0,1), mvp(3,2) + mvp(0,2), mvp(3,3) + mvp(0,3));

		//planes[FRUSTUM_TOP   ] = crPlane(mvp(3,0) - mvp(1,0), mvp(3,1) - mvp(1,1), mvp(3,2) - mvp(1,2), mvp(3,3) - mvp(1,3));
		//planes[FRUSTUM_BOTTOM] = crPlane(mvp(3,0) + mvp(1,0), mvp(3,1) + mvp(1,1), mvp(3,2) + mvp(1,2), mvp(3,3) + mvp(1,3));

		//planes[FRUSTUM_FAR   ] = crPlane(mvp(3,0) - mvp(2,0), mvp(3,1) - mvp(2,1), mvp(3,2) - mvp(2,2), mvp(3,3) - mvp(2,3));
		//planes[FRUSTUM_NEAR  ] = crPlane(mvp(3,0) + mvp(2,0), mvp(3,1) + mvp(2,1), mvp(3,2) + mvp(2,2), mvp(3,3) + mvp(2,3));
		
		for(int i = 0; i<6; i++)
		{
			planes[i].makeUnitLength();
		}
	}

	const crPlane &getPlane(const int index) const { return planes[index]; }

	bool isPointInFrustum(const crVector3 &pos) const
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			if ((pos * planes[i].getNormal()) + planes[i][3] <= 0) return false;
		}
		return true;
	}
	bool isSphereInFrustum(const crVector3 &pos, const float radius) const
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			if ((pos * planes[i].getNormal()) + planes[i][3] <= -radius) return false;
		}
		return true;
	}

protected:
	crPlane planes[6];
};

}	// end of namespace

#endif 
