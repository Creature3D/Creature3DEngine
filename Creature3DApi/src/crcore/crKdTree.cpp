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
#include <CRCore/crKdTree.h>
#include <CRCore/crObject.h>
#include <CRCore/crTriangleIndexFunctor.h>
#include <CRCore/crTimer.h>

//#include <CRCore/io_utils.h>

using namespace CRCore;

//#define VERBOSE_OUTPUT

////////////////////////////////////////////////////////////////////////////////
//
// BuildKdTree Declarartion - class used for building an single crKdTree

struct BuildKdTree
{
    BuildKdTree(crKdTree& kdTree):
        m_kdTree(kdTree) {}

    typedef std::vector< CRCore::crVector3 >            CenterList;
    typedef std::vector< unsigned int >           Indices;
    typedef std::vector< unsigned int >         AxisStack;

    bool build(crKdTree::BuildOptions& options, CRCore::crGeometry* geometry);

    void computeDivisions(crKdTree::BuildOptions& options);

    int divide(crKdTree::BuildOptions& options, CRCore::crBoundingBox& bb, int nodeIndex, unsigned int level);

    crKdTree&             m_kdTree;

    CRCore::crBoundingBox    m_bb;
    AxisStack           m_axisStack;
    Indices             m_primitiveIndices;
    CenterList          m_centers;

protected:

    BuildKdTree& operator = (const BuildKdTree&) { return *this; }
};

////////////////////////////////////////////////////////////////////////////////
//
// Functor for collecting triangle indices from crGeometry

struct TriangleIndicesCollector
{
    TriangleIndicesCollector():
        m_buildKdTree(0)
    {
    }

    inline void operator () (unsigned int p0, unsigned int p1, unsigned int p2)
    {
        const CRCore::crVector3& v0 = (*(m_buildKdTree->m_kdTree.getVertices()))[p0];
        const CRCore::crVector3& v1 = (*(m_buildKdTree->m_kdTree.getVertices()))[p1];
        const CRCore::crVector3& v2 = (*(m_buildKdTree->m_kdTree.getVertices()))[p2];
    
        // discard degenerate points
        if (v0==v1 || v1==v2 || v1==v2)
        {
            //CRCore::notify(CRCore::NOTICE)<<"Disgarding degenerate triangle"<<std::endl;
            return;
        }

        unsigned int i = m_buildKdTree->m_kdTree.addTriangle(crKdTree::Triangle(p0,p1,p2));
        
        CRCore::crBoundingBox bb;
        bb.expandBy(v0);
        bb.expandBy(v1);
        bb.expandBy(v2);

        m_buildKdTree->m_centers.push_back(bb.center());
        m_buildKdTree->m_primitiveIndices.push_back(i);
        
    }
    
    BuildKdTree* m_buildKdTree;

};


////////////////////////////////////////////////////////////////////////////////
//
// BuildKdTree Implementation

bool BuildKdTree::build(crKdTree::BuildOptions& options, CRCore::crGeometry* geometry)
{
    
#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"CRCore::KDTreeBuilder::createKDTree()"<<std::endl;146
#endif

    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geometry->getVertexArray());
    if (!vertices) return false;
    
    if (vertices->size() <= options.m_targetNumTrianglesPerLeaf) return false;

    m_bb = geometry->getBound();
    m_kdTree.setVertices(vertices);
    
    unsigned int estimatedSize = (unsigned int)(2.0*float(vertices->size())/float(options.m_targetNumTrianglesPerLeaf));

#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"kdTree->m_kdNodes.reserve()="<<estimatedSize<<std::endl<<std::endl;
#endif

    m_kdTree.getNodes().reserve(estimatedSize*5);
    
    computeDivisions(options);

    options.m_numVerticesProcessed += vertices->size();

    unsigned int estimatedNumTriangles = vertices->size()*2;
    m_primitiveIndices.reserve(estimatedNumTriangles);
    m_centers.reserve(estimatedNumTriangles);

    m_kdTree.getTriangles().reserve(estimatedNumTriangles);

    CRCore::crTriangleIndexFunctor<TriangleIndicesCollector> collectTriangleIndices;
    collectTriangleIndices.m_buildKdTree = this;
    geometry->accept(collectTriangleIndices);

    m_primitiveIndices.reserve(vertices->size());

    crKdTree::KdNode node(-1, m_primitiveIndices.size());
    node.bb = m_bb;

    int nodeNum = m_kdTree.addNode(node);

    CRCore::crBoundingBox bb = m_bb;
    nodeNum = divide(options, bb, nodeNum, 0);
    
    // now reorder the triangle list so that it's in order as per the primitiveIndex list.
    crKdTree::TriangleList triangleList(m_kdTree.getTriangles().size());
    for(unsigned int i=0; i<m_primitiveIndices.size(); ++i)
    {
        triangleList[i] = m_kdTree.getTriangle(m_primitiveIndices[i]);
    }
    
    m_kdTree.getTriangles().swap(triangleList);
    
    
#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"Root nodeNum="<<nodeNum<<std::endl;
#endif
    
    
//    CRCore::notify(CRCore::NOTICE)<<"m_kdNodes.size()="<<k_kdNodes.size()<<"  estimated size = "<<estimatedSize<<std::endl;
//    CRCore::notify(CRCore::NOTICE)<<"_kdLeaves.size()="<<_kdLeaves.size()<<"  estimated size = "<<estimatedSize<<std::endl<<std::endl;


    return !m_kdTree.getNodes().empty();
}

void BuildKdTree::computeDivisions(crKdTree::BuildOptions& options)
{
    CRCore::crVector3 dimensions(m_bb.xMax()-m_bb.xMin(),
                         m_bb.yMax()-m_bb.yMin(),
                         m_bb.zMax()-m_bb.zMin());

#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"computeDivisions("<<options.m_maxNumLevels<<") "<<dimensions<< " { "<<std::endl;
#endif

    m_axisStack.reserve(options.m_maxNumLevels);
 
    for(unsigned int level=0; level<options.m_maxNumLevels; ++level)
    {
        int axis = 0;
        if (dimensions[0]>=dimensions[1])
        {
            if (dimensions[0]>=dimensions[2]) axis = 0;
            else axis = 2;
        }
        else if (dimensions[1]>=dimensions[2]) axis = 1;
        else axis = 2;

        m_axisStack.push_back(axis);
        dimensions[axis] /= 2.0f;

#ifdef VERBOSE_OUTPUT    
        CRCore::notify(CRCore::NOTICE)<<"  "<<level<<", "<<dimensions<<", "<<axis<<std::endl;
#endif
    }

#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"}"<<std::endl;
#endif
}

int BuildKdTree::divide(crKdTree::BuildOptions& options, CRCore::crBoundingBox& bb, int nodeIndex, unsigned int level)
{
    crKdTree::KdNode& node = m_kdTree.getNode(nodeIndex);

    bool needToDivide = level < m_axisStack.size() &&
                        (node.first<0 && static_cast<unsigned int>(node.second)>options.m_targetNumTrianglesPerLeaf);
                        
    if (!needToDivide)
    {
        if (node.first<0)
        {
            int istart = -node.first-1;
            int iend = istart+node.second-1;
    
            // leaf is done, now compute bound on it.
            node.bb.init();
            for(int i=istart; i<=iend; ++i)
            {
                const crKdTree::Triangle& tri = m_kdTree.getTriangle(m_primitiveIndices[i]);
                const CRCore::crVector3& v0 = (*m_kdTree.getVertices())[tri.p0];
                const CRCore::crVector3& v1 = (*m_kdTree.getVertices())[tri.p1];
                const CRCore::crVector3& v2 = (*m_kdTree.getVertices())[tri.p2];
                node.bb.expandBy(v0);
                node.bb.expandBy(v1);
                node.bb.expandBy(v2);
                
            }

            if (node.bb.valid())
            {
                float epsilon = 1e-6f;
                node.bb.m_min.x() -= epsilon;
                node.bb.m_min.y() -= epsilon;
                node.bb.m_min.z() -= epsilon;
                node.bb.m_max.x() += epsilon;
                node.bb.m_max.y() += epsilon;
                node.bb.m_max.z() += epsilon;
            }
            
#ifdef VERBOSE_OUTPUT    
            if (!node.bb.valid())
            {
                CRCore::notify(CRCore::NOTICE)<<"After reset "<<node.first<<","<<node.second<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"  bb.m_min ("<<node.bb.m_min<<")"<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"  bb.m_max ("<<node.bb.m_max<<")"<<std::endl;
            }
            else
            {
                CRCore::notify(CRCore::NOTICE)<<"Set bb for nodeIndex = "<<nodeIndex<<std::endl;
            }
#endif
        }

        return nodeIndex;

    }

    int axis = m_axisStack[level];

#ifdef VERBOSE_OUTPUT    
    CRCore::notify(CRCore::NOTICE)<<"divide("<<nodeIndex<<", "<<level<< "), axis="<<axis<<std::endl;
#endif

    if (node.first<0)
    {    
        // leaf node as first <= 0, so look at dividing it.
        
        int istart = -node.first-1;
        int iend = istart+node.second-1;

        //CRCore::notify(CRCore::NOTICE)<<"  divide leaf"<<std::endl;
        
        float original_min = bb.m_min[axis];
        float original_max = bb.m_max[axis];

        float mid = (original_min+original_max)*0.5f;

        int originalLeftChildIndex = 0;
        int originalRightChildIndex = 0;
        bool insitueDivision = false;

        {
            //CRCore::Vec3Array* vertices = kdTree.m_vertices.get();
            int left = istart;
            int right = iend;
            
            while(left<right)
            {
                while(left<right && (m_centers[m_primitiveIndices[left]][axis]<=mid)) { ++left; }

                while(left<right && (m_centers[m_primitiveIndices[right]][axis]>mid)) { --right; }
                
                while(left<right && (m_centers[m_primitiveIndices[right]][axis]>mid)) { --right; }

                if (left<right)
                {
                    std::swap(m_primitiveIndices[left], m_primitiveIndices[right]);
                    ++left;
                    --right;
                }
            }
            
            if (left==right)
            {
                if (m_centers[m_primitiveIndices[left]][axis]<=mid) ++left;
                else --right;
            }
            
            crKdTree::KdNode leftLeaf(-istart-1, (right-istart)+1);
            crKdTree::KdNode rightLeaf(-left-1, (iend-left)+1);

#if 0
            CRCore::notify(CRCore::NOTICE)<<"In  node.first     ="<<node.first     <<" node.second     ="<<node.second<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    leftLeaf.first ="<<leftLeaf.first <<" leftLeaf.second ="<<leftLeaf.second<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    rightLeaf.first="<<rightLeaf.first<<" rightLeaf.second="<<rightLeaf.second<<std::endl;
            CRCore::notify(CRCore::NOTICE)<<"    left="<<left<<" right="<<right<<std::endl;

            if (node.second != (leftLeaf.second +rightLeaf.second))
            {
                CRCore::notify(CRCore::NOTICE)<<"*** Error in size, leaf.second="<<node.second
                                        <<", leftLeaf.second="<<leftLeaf.second
                                        <<", rightLeaf.second="<<rightLeaf.second<<std::endl;
            }
            else
            {
                CRCore::notify(CRCore::NOTICE)<<"Size OK, leaf.second="<<node.second
                                        <<", leftLeaf.second="<<leftLeaf.second
                                        <<", rightLeaf.second="<<rightLeaf.second<<std::endl;
            }
#endif

            if (leftLeaf.second<=0)
            {
                //CRCore::notify(CRCore::NOTICE)<<"LeftLeaf empty"<<std::endl;
                originalLeftChildIndex = 0;
                //originalRightChildIndex = addNode(rightLeaf);
                originalRightChildIndex = nodeIndex;
                insitueDivision = true;
            }
            else if (rightLeaf.second<=0)
            {
                //CRCore::notify(CRCore::NOTICE)<<"RightLeaf empty"<<std::endl;
                // originalLeftChildIndex = addNode(leftLeaf);
                originalLeftChildIndex = nodeIndex;
                originalRightChildIndex = 0;
                insitueDivision = true;
            }
            else
            {
                originalLeftChildIndex = m_kdTree.addNode(leftLeaf);
                originalRightChildIndex = m_kdTree.addNode(rightLeaf);
            }
        }

        
        float restore = bb.m_max[axis];
        bb.m_max[axis] = mid;

        //CRCore::notify(CRCore::NOTICE)<<"  divide leftLeaf "<<kdTree.getNode(nodeNum).first<<std::endl;
        int leftChildIndex = originalLeftChildIndex!=0 ? divide(options, bb, originalLeftChildIndex, level+1) : 0;

        bb.m_max[axis] = restore;
        
        restore = bb.m_min[axis];
        bb.m_min[axis] = mid;

        //CRCore::notify(CRCore::NOTICE)<<"  divide rightLeaf "<<kdTree.getNode(nodeNum).second<<std::endl;
        int rightChildIndex = originalRightChildIndex!=0 ? divide(options, bb, originalRightChildIndex, level+1) : 0;
        
        bb.m_min[axis] = restore;
        

        if (!insitueDivision)
        {
            // take a second reference to node we are working on as the std::vector<> resize could
            // have invalidate the previous node ref.
            crKdTree::KdNode& newNodeRef = m_kdTree.getNode(nodeIndex);
        
            newNodeRef.first = leftChildIndex;
            newNodeRef.second = rightChildIndex; 

            insitueDivision = true;

            newNodeRef.bb.init();
            if (leftChildIndex!=0) newNodeRef.bb.expandBy(m_kdTree.getNode(leftChildIndex).bb);
            if (rightChildIndex!=0) newNodeRef.bb.expandBy(m_kdTree.getNode(rightChildIndex).bb);

            if (!newNodeRef.bb.valid())
            {
                CRCore::notify(CRCore::NOTICE)<<"leftChildIndex="<<leftChildIndex<<" && originalLeftChildIndex="<<originalLeftChildIndex<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"rightChildIndex="<<rightChildIndex<<" && originalRightChildIndex="<<originalRightChildIndex<<std::endl;

                CRCore::notify(CRCore::NOTICE)<<"Invalid BB leftChildIndex="<<leftChildIndex<<", "<<rightChildIndex<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"  bb.m_min ("<<newNodeRef.bb.m_min<<")"<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"  bb.m_max ("<<newNodeRef.bb.m_max<<")"<<std::endl;

                if (leftChildIndex!=0)
                {
                    CRCore::notify(CRCore::NOTICE)<<"  getNode(leftChildIndex).bb min = "<<m_kdTree.getNode(leftChildIndex).bb.m_min<<std::endl;
                    CRCore::notify(CRCore::NOTICE)<<"                                 max = "<<m_kdTree.getNode(leftChildIndex).bb.m_max<<std::endl;
                }
                if (rightChildIndex!=0)
                {
                    CRCore::notify(CRCore::NOTICE)<<"  getNode(rightChildIndex).bb min = "<<m_kdTree.getNode(rightChildIndex).bb.m_min<<std::endl;
                    CRCore::notify(CRCore::NOTICE)<<"                              max = "<<m_kdTree.getNode(rightChildIndex).bb.m_max<<std::endl;
                }
            }
        }
    }
    else
    {
        CRCore::notify(CRCore::NOTICE)<<"NOT expecting to get here"<<std::endl;
    }
    
    return nodeIndex;
    
}

////////////////////////////////////////////////////////////////////////////////
//
// IntersectKdTree
//
struct IntersectKdTree
{
    IntersectKdTree(const CRCore::Vec3Array& vertices,
                    const crKdTree::KdNodeList& nodes,
                    const crKdTree::TriangleList& triangles,
                    crKdTree::LineSegmentIntersections& intersections,
                    const CRCore::crVector3& s, const CRCore::crVector3& e):
                        m_vertices(vertices),
                        m_kdNodes(nodes),
                        m_triangles(triangles),
                        m_intersections(intersections),
                        m_s(s),
                        m_e(e)
    {
        m_d = e - s;
        m_length = m_d.length();
        m_inverse_length = m_length!=0.0f ? 1.0f/m_length : 0.0;
        m_d *= m_inverse_length;
        
        m_d_invX = m_d.x()!=0.0f ? m_d/m_d.x() : CRCore::crVector3(0.0f,0.0f,0.0f);
        m_d_invY = m_d.y()!=0.0f ? m_d/m_d.y() : CRCore::crVector3(0.0f,0.0f,0.0f);
        m_d_invZ = m_d.z()!=0.0f ? m_d/m_d.z() : CRCore::crVector3(0.0f,0.0f,0.0f);
    }

    void intersect(const crKdTree::KdNode& node, const CRCore::crVector3& s, const CRCore::crVector3& e) const;
    bool intersectAndClip(CRCore::crVector3& s, CRCore::crVector3& e, const CRCore::crBoundingBox& bb) const;

    const CRCore::Vec3Array&               m_vertices;
    const crKdTree::KdNodeList&           m_kdNodes;
    const crKdTree::TriangleList&         m_triangles;
    crKdTree::LineSegmentIntersections&   m_intersections;

    CRCore::crVector3 m_s;
    CRCore::crVector3 m_e;

    CRCore::crVector3 m_d;
    float     m_length;
    float     m_inverse_length;
    
    CRCore::crVector3 m_d_invX;
    CRCore::crVector3 m_d_invY;
    CRCore::crVector3 m_d_invZ;


protected:

    IntersectKdTree& operator = (const IntersectKdTree&) { return *this; }
};


void IntersectKdTree::intersect(const crKdTree::KdNode& node, const CRCore::crVector3& ls, const CRCore::crVector3& le) const
{
    if (node.first<0)
    {
        // treat as a leaf
        
        //CRCore::notify(CRCore::NOTICE)<<"crKdTree::intersect("<<&leaf<<")"<<std::endl;
        int istart = -node.first-1;
        int iend = istart + node.second;
        
        for(int i=istart; i<iend; ++i)
        {
            //const Triangle& tri = m_triangles[m_primitiveIndices[i]];
            const crKdTree::Triangle& tri = m_triangles[i];
            // CRCore::notify(CRCore::NOTICE)<<"   tri("<<tri.p1<<","<<tri.p2<<","<<tri.p3<<")"<<std::endl;

            const CRCore::crVector3& v0 = m_vertices[tri.p0];
            const CRCore::crVector3& v1 = m_vertices[tri.p1];
            const CRCore::crVector3& v2 = m_vertices[tri.p2];

            CRCore::crVector3 T = m_s - v0;
            CRCore::crVector3 E2 = v2 - v0;
            CRCore::crVector3 E1 = v1 - v0;
            
            CRCore::crVector3 P =  m_d ^ E2;
            
            float det = P * E1;
            
            float r,r0,r1,r2;
            
            const float esplison = 1e-10f;
            if (det>esplison)
            {
                float u = (P*T);
                if (u<0.0 || u>det) continue;
                
                CRCore::crVector3 Q = T ^ E1;
                float v = (Q*m_d);
                if (v<0.0 || v>det) continue;
                
                if ((u+v)> det) continue;

                float inv_det = 1.0f/det;                
                float t = (Q*E2)*inv_det;            
                if (t<0.0 || t>m_length) continue;

                u *= inv_det;
                v *= inv_det;

                r0 = 1.0f-u-v;
                r1 = u;
                r2 = v; 
                r = t * m_inverse_length;
            }
            else if (det<-esplison)
            {

                float u = (P*T);
                if (u>0.0 || u<det) continue;
                
                CRCore::crVector3 Q = T ^ E1;
                float v = (Q*m_d);
                if (v>0.0 || v<det) continue;
                
                if ((u+v) < det) continue;

                float inv_det = 1.0f/det;                
                float t = (Q*E2)*inv_det;            
                if (t<0.0 || t>m_length) continue;

                u *= inv_det;
                v *= inv_det;

                r0 = 1.0f-u-v;
                r1 = u;
                r2 = v;
                r = t * m_inverse_length;
            }
            else
            {
                continue;
            }

            CRCore::crVector3 in = v0*r0 + v1*r1 + v2*r2;
            CRCore::crVector3 normal = E1^E2;
            normal.normalize();
            
#if 1
            m_intersections.push_back(crKdTree::LineSegmentIntersection());
            crKdTree::LineSegmentIntersection& intersection = m_intersections.back();

            intersection.ratio = r;
            intersection.primitiveIndex = i;
            intersection.intersectionPoint = in;
            intersection.intersectionNormal = normal;

            intersection.p0 = tri.p0;
            intersection.p1 = tri.p1;
            intersection.p2 = tri.p2;
            intersection.r0 = r0;
            intersection.r1 = r1;
            intersection.r2 = r2;

#endif
            // CRCore::notify(CRCore::NOTICE)<<"  got intersection ("<<in<<") ratio="<<r<<std::endl;
        }
    }
    else
    {
        if (node.first>0)
        {
            CRCore::crVector3 l(ls), e(le);
            if (intersectAndClip(l,e, m_kdNodes[node.first].bb))
            {
                intersect(m_kdNodes[node.first], l, e);
            }
        }
        if (node.second>0)
        {
            CRCore::crVector3 l(ls), e(le);
            if (intersectAndClip(l,e, m_kdNodes[node.second].bb))
            {
                intersect(m_kdNodes[node.second], l, e);
            }
        }
    }
}

bool IntersectKdTree::intersectAndClip(CRCore::crVector3& s, CRCore::crVector3& e, const CRCore::crBoundingBox& bb) const
{
    //return true;

    //if (!bb.valid()) return true;

    // compate s and e against the xMin to xMax range of bb.
    if (s.x()<=e.x())
    {

        // trivial reject of segment wholely outside.
        if (e.x()<bb.xMin()) return false;
        if (s.x()>bb.xMax()) return false;

        if (s.x()<bb.xMin())
        {
            // clip s to xMin.
            s = s+m_d_invX*(bb.xMin()-s.x());
        }

        if (e.x()>bb.xMax())
        {
            // clip e to xMax.
            e = s+m_d_invX*(bb.xMax()-s.x());
        }
    }
    else
    {
        if (s.x()<bb.xMin()) return false;
        if (e.x()>bb.xMax()) return false;

        if (e.x()<bb.xMin())
        {
            // clip s to xMin.
            e = s+m_d_invX*(bb.xMin()-s.x());
        }

        if (s.x()>bb.xMax())
        {
            // clip e to xMax.
            s = s+m_d_invX*(bb.xMax()-s.x());
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
            s = s+m_d_invY*(bb.yMin()-s.y());
        }

        if (e.y()>bb.yMax())
        {
            // clip e to yMax.
            e = s+m_d_invY*(bb.yMax()-s.y());
        }
    }
    else
    {
        if (s.y()<bb.yMin()) return false;
        if (e.y()>bb.yMax()) return false;

        if (e.y()<bb.yMin())
        {
            // clip s to yMin.
            e = s+m_d_invY*(bb.yMin()-s.y());
        }

        if (s.y()>bb.yMax())
        {
            // clip e to yMax.
            s = s+m_d_invY*(bb.yMax()-s.y());
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
            s = s+m_d_invZ*(bb.zMin()-s.z());
        }

        if (e.z()>bb.zMax())
        {
            // clip e to zMax.
            e = s+m_d_invZ*(bb.zMax()-s.z());
        }
    } 
    else
    {
        if (s.z()<bb.zMin()) return false;
        if (e.z()>bb.zMax()) return false;

        if (e.z()<bb.zMin())
        {
            // clip s to zMin.
            e = s+m_d_invZ*(bb.zMin()-s.z());
        }

        if (s.z()>bb.zMax())
        {
            // clip e to zMax.
            s = s+m_d_invZ*(bb.zMax()-s.z());
        }
    }
    
    // CRCore::notify(CRCore::NOTICE)<<"clampped segment "<<s<<" "<<e<<std::endl;
    
    // if (s==e) return false;

    return true;    
}


////////////////////////////////////////////////////////////////////////////////
//
// crKdTree::BuildOptions

crKdTree::BuildOptions::BuildOptions():
        m_numVerticesProcessed(0),
        m_targetNumTrianglesPerLeaf(4),
        m_maxNumLevels(32)
{
}

////////////////////////////////////////////////////////////////////////////////
//
// crKdTree

crKdTree::crKdTree()
{
}

crKdTree::crKdTree(const crKdTree& rhs, const CRCore::crCopyOp& copyop):
    crShape(rhs, copyop),
    m_vertices(rhs.m_vertices),
    m_kdNodes(rhs.m_kdNodes),
    m_triangles(rhs.m_triangles)
{
}

bool crKdTree::build(BuildOptions& options, CRCore::crGeometry* geometry)
{
    BuildKdTree build(*this);
    return build.build(options, geometry);
}

bool crKdTree::intersect(const CRCore::crVector3& start, const CRCore::crVector3& end, LineSegmentIntersections& intersections) const
{
    if (m_kdNodes.empty()) 
    {
        CRCore::notify(CRCore::NOTICE)<<"Warning: m_kdTree is empty"<<std::endl;
        return false;
    }

    unsigned int numIntersectionsBefore = intersections.size();

    IntersectKdTree intersector(*m_vertices,
                                m_kdNodes,
                                m_triangles,
                                intersections,
                                start, end);
                    
    intersector.intersect(getNode(0), start, end);
    
    return numIntersectionsBefore != intersections.size();
}

////////////////////////////////////////////////////////////////////////////////
//
// crKdTreeBuilder
crKdTreeBuilder::crKdTreeBuilder():
    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN)
{            
    m_kdTreePrototype = new CRCore::crKdTree;
}

crKdTreeBuilder::crKdTreeBuilder(const crKdTreeBuilder& rhs):
    CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
    m_buildOptions(rhs.m_buildOptions),
    m_kdTreePrototype(rhs.m_kdTreePrototype)
{
}

void crKdTreeBuilder::apply(CRCore::crObject& geode)
{
    for(unsigned int i=0; i<geode.getNumDrawables(); ++i)
    {            

        CRCore::crGeometry* geom = geode.getDrawable(i)->asGeometry();
        if (geom)
        {
            CRCore::crKdTree* previous = dynamic_cast<CRCore::crKdTree*>(geom->getShape());
            if (previous) continue;

            CRCore::ref_ptr<CRCore::crKdTree> kdTree = dynamic_cast<CRCore::crKdTree*>(m_kdTreePrototype->cloneType());

            if (kdTree->build(m_buildOptions, geom))
            {
                geom->setShape(kdTree.get());
            }
        }   
    }
}
