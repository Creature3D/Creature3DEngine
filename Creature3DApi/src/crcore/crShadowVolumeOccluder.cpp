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
#include <CRCore/crShadowVolumeOccluder.h>
#include <CRCore/crCullStack.h>

#include <CRCore/crGroup.h>
#include <CRCore/crObject.h>

using namespace CRCore;


typedef std::pair<unsigned int,crVector3>    Point; // bool=true signifies a newly created point, false indicates original point.
typedef std::vector<Point>      PointList;
typedef std::vector<crVector3>       VertexList;


// copyVertexListToPointList a vector for crVector3 into a vector of Point's.
void copyVertexListToPointList(const VertexList& in,PointList& out)
{
    out.reserve(in.size());
    for(VertexList::const_iterator itr=in.begin();
        itr!=in.end();
        ++itr)
    {
        out.push_back(Point(0,*itr));
    }
}

void copyPointListToVertexList(const PointList& in,VertexList& out)
{
    out.reserve(in.size());
    for(PointList::const_iterator itr=in.begin();
        itr!=in.end();
        ++itr)
    {
        out.push_back(itr->second);
    }
}

// clip the convex hull 'in' to plane to generate a clipped convex hull 'out'
// return true if points remain after clipping.
unsigned int clip(const crPlane& plane,const PointList& in, PointList& out,unsigned int planeMask)
{
    std::vector<float> distance;
    distance.reserve(in.size());
    for(PointList::const_iterator itr=in.begin();
        itr!=in.end();
        ++itr)
    {
        distance.push_back(plane.distance(itr->second));
    }

    out.clear();

    for(unsigned int i=0;i<in.size();++i)
    {
        unsigned int i_1 = (i+1)%in.size(); // do the mod to wrap the index round back to the start.
        
        if (distance[i]>=0.0f)
        {
            out.push_back(in[i]);
            
            
            if (distance[i_1]<0.0f)
            {
                unsigned int mask = (in[i].first & in[i_1].first) | planeMask;
                float r = distance[i_1]/(distance[i_1]-distance[i]);
                out.push_back(Point(mask,in[i].second*r+in[i_1].second*(1.0f-r)));
            }
        
        }
        else if (distance[i_1]>0.0f)
        {
            unsigned int mask = (in[i].first & in[i_1].first) | planeMask;
            float r = distance[i_1]/(distance[i_1]-distance[i]);
            out.push_back(Point(mask,in[i].second*r+in[i_1].second*(1.0f-r)));
        }
    }
    
    return out.size();
}

// clip the convex hull 'in' to planeList to generate a clipped convex hull 'out'
// return true if points remain after clipping.
unsigned int clip(const crPolytope::PlaneList& planeList,const VertexList& vin,PointList& out)
{
    PointList in;
    copyVertexListToPointList(vin,in);
    
    unsigned int planeMask = 0x1;
    for(crPolytope::PlaneList::const_iterator itr=planeList.begin();
        itr!=planeList.end();
        ++itr)
    {
        if (!clip(*itr,in,out,planeMask)) return false;
        in.swap(out);
        planeMask <<= 1;
    }

    in.swap(out);

    return out.size();
}

void transform(PointList& points,const CRCore::crMatrix& matrix)
{
    for(PointList::iterator itr=points.begin();
        itr!=points.end();
        ++itr)
    {
        itr->second = itr->second*matrix;
    }
}

void transform(const PointList& in,PointList& out,const CRCore::crMatrix& matrix)
{
    for(PointList::const_iterator itr=in.begin();
        itr!=in.end();
        ++itr)
    {
        out.push_back(Point(itr->first,itr->second * matrix));
    }
}

void pushToFarPlane(PointList& points)
{
    for(PointList::iterator itr=points.begin();
        itr!=points.end();
        ++itr)
    {
        itr->second.z() = 1.0f;
    }
}

void computePlanes(const PointList& front, const PointList& back, crPolytope::PlaneList& planeList)
{
    for(unsigned int i=0;i<front.size();++i)
    {
        unsigned int i_1 = (i+1)%front.size(); // do the mod to wrap the index round back to the start.
        if (!(front[i].first & front[i_1].first))
        {
            planeList.push_back(crPlane(front[i].second,front[i_1].second,back[i].second));
        }
    }
}

crPlane computeFrontPlane(const PointList& front)
{
    return crPlane(front[2].second,front[1].second,front[0].second);
}

// compute the volume between the front and back polygons of the occluder/hole.
float computePolytopeVolume(const PointList& front, const PointList& back)
{
    float volume = 0.0f;
    crVector3 frontStart = front[0].second;
    crVector3 backStart = back[0].second;
    for(unsigned int i=1;i<front.size()-1;++i)
    {
        volume += computeVolume(frontStart, front[i].second, front[i+1].second,
                                backStart, back[i].second, back[i+1].second);
    }
    return volume;
}

bool crShadowVolumeOccluder::computeOccluder(const NodePath& nodePath,const crConvexPlanarOccluder& occluder,crCullStack& cullStack,bool /*createDrawables*/)
{
    crCullingSet& cullingset = cullStack.getCurrentCullingSet();

    const RefMatrix& MV = cullStack.getModelViewMatrix();
    const RefMatrix& P = cullStack.getProjectionMatrix();

    // take a reference to the NodePath to this occluder.
    m_nodePath = nodePath;

    // take a reference to the projection matrix.
    m_projectionMatrix = &P;
    
    // initialize the volume
    m_volume = 0.0f;
    

    // compute the inverse of the projection matrix.
    crMatrix invP;
    invP.invert(P);
    
    float volumeview = cullStack.getFrustumVolume();

    
    // compute the transformation matrix which takes form local coords into clip space.
    crMatrix MVP(MV*P);
    
    // for the occluder polygon and each of the holes do
    //     first transform occluder polygon into clipspace by multiple it by c[i] = v[i]*(MV*P)
    //     then push to coords to far plane by setting its coord to c[i].z = -1.
    //     then transform far plane polygon back into projection space, by p[i]*inv(P)
    //     compute orientation of front plane, if normal.z()<0 then facing away from eye pont, so reverse the polygons, or simply invert planes.
    //     compute volume (quality) betwen front polygon in projection space and back polygon in projection space.
    
    
    const VertexList& vertices_in = occluder.getOccluder().getVertexList();
    
    PointList points;
    
    if (clip(cullingset.getFrustum().getPlaneList(),vertices_in,points)>=3)
    {
        // compute the points on the far plane.
        PointList farPoints;
        farPoints.reserve(points.size());
        transform(points,farPoints,MVP);
        pushToFarPlane(farPoints);
        transform(farPoints,invP);
        
        // move the occlude points into projection space.
        transform(points,MV);

        // use the points on the front plane as reference vertices on the m_occluderVolume
        // so that the vertices can later by used to test for occlusion of the occluder itself.
        copyPointListToVertexList(points,m_occluderVolume.getReferenceVertexList());

        // create the front face of the occluder
        crPlane occludePlane = computeFrontPlane(points);
        m_occluderVolume.add(occludePlane);

        // create the sides of the occluder
        computePlanes(points,farPoints,m_occluderVolume.getPlaneList());

        m_occluderVolume.setupMask();
        
        // if the front face is pointing away from the eye point flip the whole polytope.
        if (occludePlane[3]>0.0f)
        {
            m_occluderVolume.flip();
        }
        
        m_volume = computePolytopeVolume(points,farPoints)/volumeview;

        
        for(crConvexPlanarOccluder::HoleList::const_iterator hitr=occluder.getHoleList().begin();
            hitr!=occluder.getHoleList().end();
            ++hitr)
        {
            PointList points;
            if (clip(cullingset.getFrustum().getPlaneList(),hitr->getVertexList(),points)>=3)
            {
                m_holeList.push_back(crPolytope());
                crPolytope& polytope = m_holeList.back();

                // compute the points on the far plane.
                PointList farPoints;
                farPoints.reserve(points.size());
                transform(points,farPoints,MVP);
                pushToFarPlane(farPoints);
                transform(farPoints,invP);

                // move the occlude points into projection space.
                transform(points,MV);

                // use the points on the front plane as reference vertices on the m_occluderVolume
                // so that the vertices can later by used to test for occlusion of the occluder itself.
                copyPointListToVertexList(points,polytope.getReferenceVertexList());

                // create the front face of the occluder
                crPlane occludePlane = computeFrontPlane(points);

                // create the sides of the occluder
                computePlanes(points,farPoints,polytope.getPlaneList());

                polytope.setupMask();

                // if the front face is pointing away from the eye point flip the whole polytope.
                if (occludePlane[3]>0.0f)
                {
                    polytope.flip();
                }

                // remove the hole's volume from the occluder volume.
                m_volume -= computePolytopeVolume(points,farPoints)/volumeview;
            }
            
        }
        //std::cout << "final volume = "<<m_volume<<std::endl;

        return true;
    }
    return false;
}

bool crShadowVolumeOccluder::contains(const std::vector<crVector3>& vertices)
{
    if (m_occluderVolume.containsAllOf(vertices))
    {
        for(HoleList::iterator itr=m_holeList.begin();
            itr!=m_holeList.end();
            ++itr)
        {
            PointList points;
            if (clip(itr->getPlaneList(),vertices,points)>=3) return false;
        }
        return true;
    }
    return false;
}

bool crShadowVolumeOccluder::contains(const crBoundingSphere& bound)
{
    //std::cout << "Sphere testing occluder "<<this<<" mask="<<m_occluderVolume.getCurrentMask();
    if (m_occluderVolume.containsAllOf(bound))
    {
        for(HoleList::iterator itr=m_holeList.begin();
            itr!=m_holeList.end();
            ++itr)
        {
            if (itr->contains(bound))
            {
                //std::cout << " - not in occluder"<<std::endl;
                return false;
            }
        }
        //std::cout << " - in occluder ******"<<std::endl;
        return true;
    }
    //std::cout << " - not in occluder"<<std::endl;
    return false;
}

bool crShadowVolumeOccluder::contains(const crBoundingBox& bound)
{
    //std::cout << "Box testing occluder "<<this<<" mask="<<m_occluderVolume.getCurrentMask();
    if (m_occluderVolume.containsAllOf(bound))
    {
        for(HoleList::iterator itr=m_holeList.begin();
            itr!=m_holeList.end();
            ++itr)
        {
            if (itr->contains(bound))
            {
                //std::cout << " + not in occluder"<<std::endl;
                return false;
            }
        }
        //std::cout << "+ in occluder ********"<<std::endl;
        return true;
    }
    //std::cout << "+ not in occluder"<<std::endl;
    return false;
}
