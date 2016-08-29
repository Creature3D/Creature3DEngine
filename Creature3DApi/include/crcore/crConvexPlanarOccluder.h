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
#ifndef CRCORE_CRCONVEXPLANAROCCLUDER_H
#define CRCORE_CRCONVEXPLANAROCCLUDER_H 1

#include <CRCore/crConvexPlanarPolygon.h>
#include <CRCore/crBase.h>

namespace CRCore {

//class OccluderVolume;

/** A class for representing convex clipping volumes made up.
  * When adding planes, their normals should point inwards (into the volume) */
class CR_EXPORT crConvexPlanarOccluder : public crBase
{

    public:

        crConvexPlanarOccluder():crBase() {}
		virtual ~crConvexPlanarOccluder() {}

        crConvexPlanarOccluder(const crConvexPlanarOccluder& cpo,const crCopyOp& copyop=crCopyOp::SHALLOW_COPY):
            crBase(cpo,copyop),
            m_occluder(cpo.m_occluder),
            m_holeList(cpo.m_holeList) {}
        
        META_Base(CRCore,crConvexPlanarOccluder);

        void setOccluder(const crConvexPlanarPolygon& cpp) { m_occluder = cpp; }
        
        crConvexPlanarPolygon& getOccluder() { return m_occluder; }

        const crConvexPlanarPolygon& getOccluder() const { return m_occluder; }
        
        

        typedef std::vector<crConvexPlanarPolygon> HoleList;

        void addHole(const crConvexPlanarPolygon& cpp) { m_holeList.push_back(cpp); }

		void setHoleList(const HoleList& holeList) { m_holeList=holeList; }

        HoleList& getHoleList() { return m_holeList; }
        
        const HoleList& getHoleList() const { return m_holeList; }

    protected:
        crConvexPlanarPolygon m_occluder;
        HoleList m_holeList;

};

}	// end of namespace

#endif 
