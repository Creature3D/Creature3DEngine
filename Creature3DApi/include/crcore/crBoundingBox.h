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

#ifndef CRCORE_BOUNDINGBOX_H
#define CRCORE_BOUNDINGBOX_H 1

#include <CRCore/crExport.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMath.h>
#include <float.h>

namespace CRCore {

class crBoundingSphere;

/** General purpose axis-aligned bounding box class for enclosing objects/vertices.
    Used to bounding the leaf objects in the scene,
    i.e. CRCore::crDrawable's to assist in view frustum culling etc.
*/

class CR_EXPORT crBoundingBox
{
    public:
    
	/** The corner with the smallest values for each coordinate of the
	    bounding box.*/
	crVector3 m_min;
	/** The corner with the largest values for each coordinate of the
	    bounding box.*/
	crVector3 m_max;

    /** construct to invalid values to represent an unset bounding box.*/ 
    inline crBoundingBox() : m_min(FLT_MAX,FLT_MAX,FLT_MAX),
                    m_max(-FLT_MAX,-FLT_MAX,-FLT_MAX) {}

    /** construct to with specified min and max values.*/ 
    inline crBoundingBox(float xmin,float ymin,float zmin,
                float xmax,float ymax,float zmax) :
                m_min(xmin,ymin,zmin),
                m_max(xmax,ymax,zmax) {}

    /** construct to with specified min and max values.*/ 
    inline crBoundingBox(const crVector3& min,const crVector3& max) : 
                m_min(min),
                m_max(max) {}

    /** initialize to invalid values to represent an unset bounding box.*/ 
    inline void init()
    {
        m_min.set(FLT_MAX,FLT_MAX,FLT_MAX);
        m_max.set(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    }
                    
    inline bool valid() const
    {
        return m_max.x()>=m_min.x() &&  m_max.y()>=m_min.y() &&  m_max.z()>=m_min.z();
    }

    inline void correct()
	{
		if(!valid())
		{
			float temp;
			if( m_min.x() > m_max.x() )
			{
				temp = m_min.x();
				m_min.x() = m_max.x();
                m_max.x() = temp;
			}
			if( m_min.y() > m_max.y() )
			{
				temp = m_min.y();
				m_min.y() = m_max.y();
				m_max.y() = temp;
			}
			if( m_min.z() > m_max.z() )
			{
				temp = m_min.z();
				m_min.z() = m_max.z();
				m_max.z() = temp;
			}
		}
	}

    inline void set (float xmin,float ymin,float zmin,
                        float xmax,float ymax,float zmax)
    {
        m_min.set(xmin,ymin,zmin);
        m_max.set(xmax,ymax,zmax);
    }

    /** construct to with specified min and max values.*/ 
    inline void set(const crVector3& min,const crVector3& max)
    {
        m_min = min;
        m_max = max;
    }


    inline float& xMin() { return m_min.x(); }
    inline float xMin() const { return m_min.x(); }

    inline float& yMin() { return m_min.y(); }
    inline float yMin() const { return m_min.y(); }

    inline float& zMin() { return m_min.z(); }
    inline float zMin() const { return m_min.z(); }

    inline float& xMax() { return m_max.x(); }
    inline float xMax() const { return m_max.x(); }

    inline float& yMax() { return m_max.y(); }
    inline float yMax() const { return m_max.y(); }

    inline float& zMax() { return m_max.z(); }
    inline float zMax() const { return m_max.z(); }

	inline float xLength()const{ return m_max.x() - m_min.x(); }
	inline float yLength()const{ return m_max.y() - m_min.y(); }
	inline float zLength()const{ return m_max.z() - m_min.z(); }

    /** Calculate and return the center of the bounding box.*/
    inline const crVector3 center() const
    {
        return (m_min+m_max)*0.5f;
    }

    /** Calculate and return the radius of the bounding box.*/
    inline float radius() const
    {
        return sqrtf(radius2());
    }

    /** Calculate and return the radius squared of the bounding box.
        Note, radius2() is faster to calculate than radius().*/            
    inline float radius2() const
    {
        return 0.25f*((m_max-m_min).length2());
    }

    /** return the corner of the bounding box.
        Position (pos) is specified by a number between 0 and 7,
        the first bit toggles between x min and x max, second
        bit toggles between y min and y max, third bit toggles
        between z min and z max.*/
    inline const crVector3 corner(unsigned int pos) const
    {
        return crVector3(pos&1?m_max.x():m_min.x(),pos&2?m_max.y():m_min.y(),pos&4?m_max.z():m_min.z());
    }

    /** If the vertex is out-with the box expand to encompass vertex.
        If this box is empty then move set this box's min max to vertex. */
    inline void expandBy(const crVector3& v)
    {
        if(v.x()<m_min.x()) m_min.x() = v.x();
        if(v.x()>m_max.x()) m_max.x() = v.x();

        if(v.y()<m_min.y()) m_min.y() = v.y();
        if(v.y()>m_max.y()) m_max.y() = v.y();

        if(v.z()<m_min.z()) m_min.z() = v.z();
        if(v.z()>m_max.z()) m_max.z() = v.z();
    }

    /** If the vertex is out-with the box expand to encompass vertex.
        If this box is empty then move set this box's min max to vertex. */
    inline void expandBy(float x,float y,float z)
    {
        if(x<m_min.x()) m_min.x() = x;
        if(x>m_max.x()) m_max.x() = x;

        if(y<m_min.y()) m_min.y() = y;
        if(y>m_max.y()) m_max.y() = y;

        if(z<m_min.z()) m_min.z() = z;
        if(z>m_max.z()) m_max.z() = z;
    }

    /** If incoming box is out-with the box expand to encompass incoming box.
        If this box is empty then move set this box to incoming box. */
    void expandBy(const crBoundingBox& bb);

    /** If incoming sphere is out-with the box expand to encompass incoming sphere.
        If this box is empty then move set this box to encompass the sphere. */
    void expandBy(const crBoundingSphere& sh);
    

    /** Compute the intesection of this bounding box with the specified bounding box.*/
    crBoundingBox intersect(const crBoundingBox& bb) const
    {    return CRCore::crBoundingBox(CRCore::maximum(xMin(),bb.xMin()),CRCore::maximum(yMin(),bb.yMin()),CRCore::maximum(zMin(),bb.zMin()),
                                    CRCore::minimum(xMax(),bb.xMax()),CRCore::minimum(yMax(),bb.yMax()),CRCore::minimum(zMax(),bb.zMax()));

    }

    /** return true if this bounding box with the specified bounding box.*/
    bool intersects(const crBoundingBox& bb) const
    {    return CRCore::maximum(xMin(),bb.xMin()) <= CRCore::minimum(xMax(),bb.xMax()) &&
                CRCore::maximum(yMin(),bb.yMin()) <= CRCore::minimum(yMax(),bb.yMax()) &&
                CRCore::maximum(zMin(),bb.zMin()) <= CRCore::minimum(zMax(),bb.zMax());

    }

	bool intersect2D(const crBoundingBox& bb) const
	{    
		return CRCore::maximum(xMin(),bb.xMin()) <= CRCore::minimum(xMax(),bb.xMax()) &&
			   CRCore::maximum(yMin(),bb.yMin()) <= CRCore::minimum(yMax(),bb.yMax());
	}

	inline bool contains(const crBoundingBox& bb) const
	{
        return contains(bb.m_max)&&contains(bb.m_min);
	}

	inline bool contains(const crBoundingBox& bb) 
	{
		return contains(bb.m_max)&&contains(bb.m_min);
	}

    /** return true is vertex v is within the box.*/
    inline bool contains(const crVector3& v) const
    {
        return valid() && 
                (v.x()>=m_min.x() && v.x()<=m_max.x()) &&
                (v.y()>=m_min.y() && v.y()<=m_max.y()) &&
                (v.z()>=m_min.z() && v.z()<=m_max.z());
    }

	inline bool contains(const crVector3& v)
	{
		return valid() && 
			(v.x()>=m_min.x() && v.x()<=m_max.x()) &&
			(v.y()>=m_min.y() && v.y()<=m_max.y()) &&
			(v.z()>=m_min.z() && v.z()<=m_max.z());
	}

	inline crBoundingBox& operator = (const crBoundingBox& bb) 
	{ m_min = bb.m_min; m_max = bb.m_max; return (*this); }

};

}

#endif
