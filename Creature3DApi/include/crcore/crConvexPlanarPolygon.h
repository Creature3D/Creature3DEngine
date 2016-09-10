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
//Modified by Wucaihua
#ifndef CRCORE_CONVEXPLANARPOLYGON
#define CRCORE_CONVEXPLANARPOLYGON 1

#include <CRCore/crPlane.h>

#include <vector>

namespace CRCore {

class crBoundingBox;
class crBoundingSphere;

/** A class for representing convex clipping volumes made up.
  * When adding planes, their normals should point inwards (into the volume) */
class CR_EXPORT crConvexPlanarPolygon
{

    public:

        crConvexPlanarPolygon();
		virtual ~crConvexPlanarPolygon(){};
            
        typedef std::vector<CRCore::crVector3> VertexList;

        void add(const crVector3& v) { m_vertexList.push_back(v); }
        
		void setVertexList(const VertexList& vertexList) { m_vertexList=vertexList; }

        VertexList& getVertexList() { return m_vertexList; }
        
        const VertexList& getVertexList() const { return m_vertexList; }
      
    protected:

        VertexList m_vertexList;

};

}	// end of namespace

#endif 
