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
#ifndef CRUTIL_CRSMOOTHINGVISITOR_H
#define CRUTIL_CRSMOOTHINGVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crMath.h>

#include <CRUtil/crExport.h>

namespace CRUtil {

/** A smoothing visitor for calculating smoothed normals for
  * CRCore::GeoSet's which contains surface primitives.
  */
class CRUTIL_EXPORT crSmoothingVisitor : public CRCore::crNodeVisitor
{
public:
    /// default to traversing all children.
	crSmoothingVisitor();
	virtual ~crSmoothingVisitor();

    /// smooth geoset by creating per vertex normals.
    static void smooth(CRCore::crGeometry& geoset, double creaseAngle=PI);

    /// apply smoothing method to all geode geosets.
    virtual void apply(CRCore::crObject& object);

	/// set the maximum angle, in radians, at which angle between adjacent triangles that normals are smoothed
	/// for edges that greater the shared vertices are duplicated
	void setCreaseAngle(double angle) { m_creaseAngle = angle; }
	double getCreaseAngle() const { return m_creaseAngle; }
protected:

	double m_creaseAngle;

};

}

#endif
