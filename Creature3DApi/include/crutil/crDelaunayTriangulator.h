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
#ifndef CRUTIL_DELAUNAYTRIANGULATOR_H
#define CRUTIL_DELAUNAYTRIANGULATOR_H

#include <CRCore/ref_ptr.h>
#include <CRCore/crArray.h>
#include <CRCore/Referenced.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crPrimitive.h>

#include <CRUtil/crExport.h>

namespace CRUtil 
{

/** Utility class that triangulates an irregular network of sample points.
    Just create a crDelaunayTriangulator, assign it the sample point array and call
    its triangulate() method to start the triangulation. Then you can obtain the
    generated primitive by calling the getTriangles() method.
*/
class CRUTIL_EXPORT crDelaunayTriangulator: public CRCore::Referenced {
public:

    crDelaunayTriangulator();
    explicit crDelaunayTriangulator(CRCore::Vec3Array *points, CRCore::Vec3Array *normals = 0);
    crDelaunayTriangulator(const crDelaunayTriangulator &copy, const CRCore::crCopyOp &copyop = CRCore::crCopyOp::SHALLOW_COPY);

    /** Get the const input point array. */
    inline const CRCore::Vec3Array *getInputPointArray() const;

    /** Get the input point array. */
    inline CRCore::Vec3Array *getInputPointArray();

    /** Set the input point array. */
    inline void setInputPointArray(CRCore::Vec3Array *points);

    /** Get the const output normal array (optional). */
    inline const CRCore::Vec3Array *getOutputNormalArray() const;

    /** Get the output normal array (optional). */
    inline CRCore::Vec3Array *getOutputNormalArray();

    /** Set the output normal array (optional). */
    inline void setOutputNormalArray(CRCore::Vec3Array *normals);

    /** Start triangulation. */
    bool triangulate();

    /** Get the generated primitive (call triangulate() first). */
    inline const CRCore::DrawElementsUInt *getTriangles() const;

    /** Get the generated primitive (call triangulate() first). */
    inline CRCore::DrawElementsUInt *getTriangles();

protected:
    virtual ~crDelaunayTriangulator();
    crDelaunayTriangulator &operator=(const crDelaunayTriangulator &) { return *this; }

private:
    CRCore::ref_ptr<CRCore::Vec3Array> m_points;
    CRCore::ref_ptr<CRCore::Vec3Array> m_normals;
    CRCore::ref_ptr<CRCore::DrawElementsUInt> m_prim_tris;
};

// INLINE METHODS

inline const CRCore::Vec3Array *crDelaunayTriangulator::getInputPointArray() const
{
    return m_points.get();
}

inline CRCore::Vec3Array *crDelaunayTriangulator::getInputPointArray()
{
    return m_points.get();
}

inline void crDelaunayTriangulator::setInputPointArray(CRCore::Vec3Array *points)
{
    m_points = points;
}

inline const CRCore::Vec3Array *crDelaunayTriangulator::getOutputNormalArray() const
{
    return m_normals.get();
}

inline CRCore::Vec3Array *crDelaunayTriangulator::getOutputNormalArray()
{
    return m_normals.get();
}

inline void crDelaunayTriangulator::setOutputNormalArray(CRCore::Vec3Array *normals)
{
    m_normals = normals;
}

inline const CRCore::DrawElementsUInt *crDelaunayTriangulator::getTriangles() const
{
    return m_prim_tris.get();
}

inline CRCore::DrawElementsUInt *crDelaunayTriangulator::getTriangles()
{
    return m_prim_tris.get();
}

}

#endif
