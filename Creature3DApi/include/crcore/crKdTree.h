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
#ifndef CRCORE_KDTREE_H
#define CRCORE_KDTREE_H 1

#include <CRCore/crShape.h>
#include <CRCore/crGeometry.h>

#include <map>

namespace CRCore
{

/** Implementation of a kdtree for crGeometry leaves, to enable fast intersection tests.*/
class CR_EXPORT crKdTree : public CRCore::crShape 
{
    public:
    
        
        crKdTree();
        
        crKdTree(const crKdTree& rhs, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Shape(CRCore, crKdTree);
        
        struct CR_EXPORT BuildOptions
        {
            BuildOptions();
                        
            unsigned int m_numVerticesProcessed;
            unsigned int m_targetNumTrianglesPerLeaf;
            unsigned int m_maxNumLevels;
        };
        
        
        /** Build the kdtree from the specified source geometry object.
          * retun true on success. */ 
        virtual bool build(BuildOptions& buildOptions, CRCore::crGeometry* geometry);
        
        struct LineSegmentIntersection
        {
            LineSegmentIntersection():
                ratio(-1.0),
                p0(0),
                p1(0),
                p2(0),
                r0(0.0f),
                r1(0.0f),
                r2(0.0f),
                primitiveIndex(0) {}

            bool operator < (const LineSegmentIntersection& rhs) const { return ratio < rhs.ratio; }

            typedef std::vector<unsigned int>   IndexList;
            typedef std::vector<double>         RatioList;

            double                          ratio;
            CRCore::crVector3d                      intersectionPoint;
            CRCore::crVector3                       intersectionNormal;

            unsigned int                    p0;
            unsigned int                    p1;
            unsigned int                    p2;
            float                           r0;
            float                           r1;
            float                           r2;

            unsigned int                    primitiveIndex;
        };        
        

        typedef std::vector<LineSegmentIntersection> LineSegmentIntersections;
        
        /** compute the intersection of a line segment and the kdtree, return true if an intersection has been found.*/
        virtual bool intersect(const CRCore::crVector3& start, const CRCore::crVector3& end, LineSegmentIntersections& intersections) const;


        typedef int value_type;

        struct KdNode
        {
            KdNode():
                first(0),
                second(0) {}

            KdNode(value_type f, value_type s):
                first(f),
                second(s) {}

            CRCore::crBoundingBox bb;

            value_type first;    
            value_type second;
        };
    
        struct Triangle
        {
            Triangle():
                p0(0),p1(0),p2(0) {}
        
            Triangle(unsigned int ip0, unsigned int ip1, unsigned int ip2):
                p0(ip0), p1(ip1), p2(ip2) {}

            bool operator < (const Triangle& rhs) const
            {
                if (p0<rhs.p0) return true;
                if (p0>rhs.p0) return false;
                if (p1<rhs.p1) return true;
                if (p1>rhs.p1) return false;
                return p2<rhs.p2;
            }

            unsigned int p0;
            unsigned int p1;
            unsigned int p2;
        };

        typedef std::vector< KdNode >       KdNodeList;
        typedef std::vector< Triangle >     TriangleList;

        int addNode(const KdNode& node)
        {
            int num = static_cast<int>(m_kdNodes.size()); 
            m_kdNodes.push_back(node); 
            return num;
        }

        KdNode& getNode(int nodeNum) { return m_kdNodes[nodeNum]; }
        const KdNode& getNode(int nodeNum) const { return m_kdNodes[nodeNum]; }
        
        KdNodeList& getNodes() { return m_kdNodes; }
        const KdNodeList& getNodes() const { return m_kdNodes; }

        void setVertices(CRCore::Vec3Array* vertices) { m_vertices = vertices; }
        const CRCore::Vec3Array* getVertices() const { return m_vertices.get(); }

        unsigned int addTriangle(const Triangle& tri)
        {
            unsigned int num = static_cast<unsigned int>(m_triangles.size());
            m_triangles.push_back(tri);
            return num;
        }

        Triangle& getTriangle(unsigned int i) { return m_triangles[i]; }
        const Triangle& getTriangle(unsigned int i) const { return m_triangles[i]; }

        TriangleList& getTriangles() { return m_triangles; }
        const TriangleList& getTriangles() const { return m_triangles; }


    protected:

        CRCore::ref_ptr<CRCore::Vec3Array>        m_vertices;
        KdNodeList                          m_kdNodes;
        TriangleList                        m_triangles;

};

class CR_EXPORT crKdTreeBuilder : public CRCore::crNodeVisitor
{
    public:
        crKdTreeBuilder();
        crKdTreeBuilder(const crKdTreeBuilder& rhs);

        virtual crKdTreeBuilder* clone() { return new crKdTreeBuilder(*this); }

        void apply(CRCore::crObject& geode);
    
        crKdTree::BuildOptions m_buildOptions;

        CRCore::ref_ptr<CRCore::crKdTree> m_kdTreePrototype;


                
    protected:
    
        virtual ~crKdTreeBuilder() {}
        
};

}

#endif
