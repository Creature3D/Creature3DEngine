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
#ifndef CRCORE_CRSHADOWVOLUMEOCCLUDER_H
#define CRCORE_CRSHADOWVOLUMEOCCLUDER_H 1

#include <CRCore/crExport.h>
#include <CRCore/crPolytope.h>
#include <CRCore/crConvexPlanarOccluder.h>
#include <CRCore/crNode.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crVector3.h>

namespace CRCore {

class crCullStack;

/** crShadowVolumeOccluder is a helper class for implementating shadow occlusion culling. */
class CR_EXPORT crShadowVolumeOccluder
{

    public:
    
    
        typedef std::vector<CRCore::crPolytope> HoleList;

        crShadowVolumeOccluder(const crShadowVolumeOccluder& svo):
            m_volume(svo.m_volume),
            m_nodePath(svo.m_nodePath),
            m_projectionMatrix(svo.m_projectionMatrix),
            m_occluderVolume(svo.m_occluderVolume),
            m_holeList(svo.m_holeList) {}

        crShadowVolumeOccluder():
            m_volume(0.0f) {}

		virtual	~crShadowVolumeOccluder(){};

        bool operator < (const crShadowVolumeOccluder& svo) const { return getVolume()>svo.getVolume(); } // not greater volume first. 

        /** compute the shadow volume occluder. */
        bool computeOccluder(const CRCore::NodePath& nodePath,const CRCore::crConvexPlanarOccluder& occluder,crCullStack& cullStack,bool createDrawables=false);
        

        inline void disableResultMasks();
        
        inline void pushCurrentMask();
        inline void popCurrentMask();
        
        
        /** return true if the matrix passed in matches the projection matrix that this ShaowVolumeOccluder is
          * associated with.*/
        bool matchProjectionMatrix(const CRCore::crMatrix& matrix) const
        {
            if (m_projectionMatrix.valid()) return matrix==*m_projectionMatrix;
            else return false;
        }
        

        /** Set the CRCore::NodePath which describes the which node in the scene graph
          * that this occluder was attached to.*/
        inline void setNodePath(CRCore::NodePath& nodePath) { m_nodePath = nodePath; }
        inline CRCore::NodePath& getNodePath() { return m_nodePath; }
        inline const CRCore::NodePath& getNodePath() const { return m_nodePath; }


        /** get the volume of the occluder minus its holes, in eye coords, the volume is normalized by dividing by 
          * the volume of the view frustum in eye coords.*/
        float getVolume() const { return m_volume; }
        
        /** return the occluder polytope.*/
        CRCore::crPolytope& getOccluder() { return m_occluderVolume; }
        
        /** return the const occluder polytope.*/
        const CRCore::crPolytope& getOccluder() const { return m_occluderVolume; }
        
        /** return the list of holes.*/
        HoleList& getHoleList() { return m_holeList; }
        
        /** return the const list of holes.*/
        const HoleList& getHoleList() const { return m_holeList; }
        

        /** return true if the specified vertex list is contaned entirely
          * within this shadow occluder volume.*/
        bool contains(const std::vector<CRCore::crVector3>& vertices);

        /** return true if the specified bounding sphere is contaned entirely
          * within this shadow occluder volume.*/
        bool contains(const CRCore::crBoundingSphere& bound);
        
        /** return true if the specified bounding box is contained entirely
          * within this shadow occluder volume.*/
        bool contains(const CRCore::crBoundingBox& bound);

        inline void transformProvidingInverse(const CRCore::crMatrix& matrix)
        {
            m_occluderVolume.transformProvidingInverse(matrix);
            for(HoleList::iterator itr=m_holeList.begin();
                itr!=m_holeList.end();
                ++itr)
            {
                itr->transformProvidingInverse(matrix);
            }
        }
        

    protected:

        float                       m_volume;
        CRCore::NodePath            m_nodePath;
		CRCore::ref_ptr<const CRCore::RefMatrix>    m_projectionMatrix;
        CRCore::crPolytope          m_occluderVolume;
        HoleList                    m_holeList;
};


/** A list of crShadowVolumeOccluder, used by CollectOccluderVisitor and CullVistor's.*/
typedef std::vector<crShadowVolumeOccluder> ShadowVolumeOccluderList;


inline void crShadowVolumeOccluder::disableResultMasks()
{
    //std::cout<<"crShadowVolumeOccluder::disableResultMasks() - m_occluderVolume.getMaskStack().size()="<<m_occluderVolume.getMaskStack().size()<<"  "<<m_occluderVolume.getCurrentMask()<<std::endl;
    m_occluderVolume.setResultMask(0);
    for(HoleList::iterator itr=m_holeList.begin();
        itr!=m_holeList.end();
        ++itr)
    {
        itr->setResultMask(0);
    }
}

inline void crShadowVolumeOccluder::pushCurrentMask()
{
    //std::cout<<"crShadowVolumeOccluder::pushCurrentMasks() - m_occluderVolume.getMaskStack().size()="<<m_occluderVolume.getMaskStack().size()<<"  "<<m_occluderVolume.getCurrentMask()<<std::endl;
    m_occluderVolume.pushCurrentMask();
    if (!m_holeList.empty())
    {
        for(HoleList::iterator itr=m_holeList.begin();
            itr!=m_holeList.end();
            ++itr)
        {
            itr->pushCurrentMask();
        }
    }
}

inline void crShadowVolumeOccluder::popCurrentMask()
{
    m_occluderVolume.popCurrentMask();
    if (!m_holeList.empty())
    {
        for(HoleList::iterator itr=m_holeList.begin();
            itr!=m_holeList.end();
            ++itr)
        {
            itr->popCurrentMask();
        }
    }
    //std::cout<<"crShadowVolumeOccluder::popCurrentMasks() - m_occluderVolume.getMaskStack().size()="<<m_occluderVolume.getMaskStack().size()<<"  "<<m_occluderVolume.getCurrentMask()<<std::endl;
}

}	// end of namespace

#endif 
