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
#include <CRCore/crTriangleIndexFunctor.h>

#include <CRUtil/crSimplifier.h>

#include <CRUtil/crSmoothingVisitor.h>
#include <CRUtil/crTriStripVisitor.h>

#include <set>
#include <list>
#include <iterator>
using namespace CRUtil;


struct dereference_less
{
    template<class T, class U>
    inline bool operator() (const T& lhs,const U& rhs) const
    {
        return *lhs < *rhs;
    }
};

struct dereference_clear
{
    template<class T>
    inline void operator() (const T& t)
    {
        T& non_const_t = const_cast<T&>(t);
        non_const_t->clear();
    }
};

class EdgeCollapse
{
public:

    struct Triangle;
    struct Edge;
    struct Point;


    EdgeCollapse() {}
        
    ~EdgeCollapse();

    void setGeometry(CRCore::crGeometry* geometry, const crSimplifier::IndexList& protectedPoints);
    CRCore::crGeometry* getGeometry() { return m_geometry; }

    unsigned int getNumOfTriangles() { return m_triangleSet.size(); }

    Point* computeInterpolatedPoint(Edge* edge,float r) const
    {
        Point* point = new Point;
        float r1 = 1.0f-r;
        float r2 = r;
        Point* p1 = edge->m_p1.get();
        Point* p2 = edge->m_p2.get();
        
        if (p1==0 || p2==0)
        {
            CRCore::notify(CRCore::NOTICE)<<"Error computeInterpolatedPoint("<<edge<<",r) p1 and/or p2==0"<<std::endl;
            return 0;
        } 
        
        point->m_vertex = p1->m_vertex * r1 + p2->m_vertex * r2;
        unsigned int s = CRCore::minimum(p1->m_attributes.size(),p2->m_attributes.size());
        for(unsigned int i=0;i<s;++i)
        {
            point->m_attributes.push_back(p1->m_attributes[i]*r1 + p2->m_attributes[i]*r2); 
        }
        return point;
    }

    Point* computeOptimalPoint(Edge* edge) const
    {
        return computeInterpolatedPoint(edge,0.5f);
    }
    
    float computeErrorMetric(Edge* edge,Point* point) const
    {
        typedef std::set< CRCore::ref_ptr<Triangle> > LocalTriangleSet ;
        LocalTriangleSet triangles;
        std::copy( edge->m_p1->m_triangles.begin(), edge->m_p1->m_triangles.end(), std::inserter(triangles,triangles.begin()));
        std::copy( edge->m_p2->m_triangles.begin(), edge->m_p2->m_triangles.end(), std::inserter(triangles,triangles.begin()));
    
        const CRCore::crVector3& vertex = point->m_vertex;
        float error = 0.0f;
        
        if (triangles.empty()) return 0.0f;
        
        for(LocalTriangleSet::iterator itr=triangles.begin();
            itr!=triangles.end();
            ++itr)
        {
            error += fabs( (*itr)->distance(vertex) );
        }
        
        // use average of error
        error /= (float)triangles.size();
        
        return error;
    }
    
    void updateErrorMetricForEdge(Edge* edge)
    {
        if (!edge->m_p1 || !edge->m_p2)
        {
            CRCore::notify(CRCore::NOTICE)<<"Error updateErrorMetricForEdge("<<edge<<") p1 and/or p2==0"<<std::endl;
            return;
        }


        CRCore::ref_ptr<Edge> keep_local_reference_to_edge(edge);
    
        if (_edgeSet.count(keep_local_reference_to_edge)!=0)
        {
            _edgeSet.erase(keep_local_reference_to_edge);
        }
        
        
        edge->_proposedPoint = computeOptimalPoint(edge);
        edge->updateMaxNormalDeviationOnEdgeCollapse();
        
        if (edge->getMaxNormalDeviationOnEdgeCollapse()<=1.0f && !edge->isAdjacentToBoundary())
            edge->setErrorMetric( computeErrorMetric( edge, edge->_proposedPoint.get()));
        else
            edge->setErrorMetric( FLT_MAX );
        
        _edgeSet.insert(keep_local_reference_to_edge);
    }
    
    void updateErrorMetricForAllEdges()
    {
        typedef std::vector< CRCore::ref_ptr<Edge> > LocalEdgeList ;
        LocalEdgeList edges;
        std::copy( _edgeSet.begin(), _edgeSet.end(), std::back_inserter(edges));
        
        _edgeSet.clear();
        
        for(LocalEdgeList::iterator itr=edges.begin();
            itr!=edges.end();
            ++itr)
        {
            Edge* edge = itr->get();

        edge->_proposedPoint = computeOptimalPoint(edge);
        edge->updateMaxNormalDeviationOnEdgeCollapse();
        
        if (edge->getMaxNormalDeviationOnEdgeCollapse()<=1.0f && !edge->isAdjacentToBoundary())
            edge->setErrorMetric( computeErrorMetric( edge, edge->_proposedPoint.get()));
        else
            edge->setErrorMetric( FLT_MAX );
            
            _edgeSet.insert(edge);
        }
    }

    bool collapseMinimumErrorEdge()
    {
        if (!_edgeSet.empty())
        {
            Edge* edge = const_cast<Edge*>(_edgeSet.begin()->get());

            if (edge->getErrorMetric()==FLT_MAX)
            {
                //CRCore::notify(CRCore::INFO)<<"collapseMinimumErrorEdge() return false due to edge->getErrorMetric()==FLT_MAX"<<std::endl;
                return false;
            }

            CRCore::ref_ptr<Point> pNew = edge->_proposedPoint.valid()? edge->_proposedPoint : computeInterpolatedPoint(edge,0.5f);
            return (collapseEdge(edge,pNew.get()));
        }
        //CRCore::notify(CRCore::INFO)<<"collapseMinimumErrorEdge() return false due to _edgeSet.empty()"<<std::endl;
        return false;
    }

    void copyBackToGeometry();

    typedef std::vector<float>                                                         FloatList;
    typedef std::set<CRCore::ref_ptr<Edge>,dereference_less >                          EdgeSet;
    typedef std::set< CRCore::ref_ptr<Point>,dereference_less >                        PointSet;
    typedef std::vector< CRCore::ref_ptr<Point> >                                      PointList;
    typedef std::list< CRCore::ref_ptr<Triangle> >                                     TriangleList;
    typedef std::set< CRCore::ref_ptr<Triangle> >                                      TriangleSet;
    typedef std::map< CRCore::ref_ptr<Triangle>, unsigned int, dereference_less >      TriangleMap;

    struct Point : public CRCore::Referenced
    {
        Point(): _protected(false), m_index(0) {}
        
        bool _protected;

        unsigned int m_index;

        CRCore::crVector3   m_vertex;
        FloatList           m_attributes;
        TriangleSet         m_triangles;

        void clear()
        {
            m_attributes.clear();
            m_triangles.clear();
        }

        bool operator < ( const Point& rhs) const
        {
            if (m_vertex < rhs.m_vertex) return true;
            if (rhs.m_vertex < m_vertex) return false;
            
            return m_attributes < rhs.m_attributes;
        }
        
        bool isBoundaryPoint() const
        {
            if (_protected) return true;
        
            for(TriangleSet::const_iterator itr=m_triangles.begin();
                itr!=m_triangles.end();
                ++itr)
            {
                const Triangle* triangle = itr->get();
                if ((triangle->_e1->m_p1==this || triangle->_e1->m_p2==this) && triangle->_e1->isBoundaryEdge()) return true;
                if ((triangle->_e2->m_p1==this || triangle->_e2->m_p2==this) && triangle->_e2->isBoundaryEdge()) return true;
                if ((triangle->_e3->m_p1==this || triangle->_e3->m_p2==this) && triangle->_e3->isBoundaryEdge()) return true;
            
                //if ((*itr)->isBoundaryTriangle()) return true;
            }
            return false;
        }

    };

    struct Edge : public CRCore::Referenced
    {
        Edge(): _errorMetric(0.0f), _maximumDeviation(1.0f) {}
        
        void clear()
        {
            m_p1 = 0;
            m_p2 = 0;
            m_triangles.clear();
        }

        CRCore::ref_ptr<Point> m_p1;
        CRCore::ref_ptr<Point> m_p2;
        
        TriangleSet m_triangles;

        float _errorMetric;
        float _maximumDeviation;

        CRCore::ref_ptr<Point> _proposedPoint;

        void setErrorMetric(float errorMetric) { _errorMetric = errorMetric; }
        float getErrorMetric() const { return _errorMetric; }
        
        bool operator < ( const Edge& rhs) const
        {
            // both error metrics are computed
            if (getErrorMetric()<rhs.getErrorMetric()) return true;
            else if (rhs.getErrorMetric()<getErrorMetric()) return false;
            
            if (m_p1 < rhs.m_p1) return true;
            else if (rhs.m_p1 < m_p1) return false;

            return (m_p2 < rhs.m_p2);
        }
        
        bool operator == ( const Edge& rhs) const
        {
            if (&rhs==this) return true;
            if (*this<rhs) return false;
            if (rhs<*this) return false;
            return true;
        }

        bool operator != ( const Edge& rhs) const
        {
            if (&rhs==this) return false;
            if (*this<rhs) return true;
            if (rhs<*this) return true;
            return false;
        }
        
        void addTriangle(Triangle* triangle)
        {
            m_triangles.insert(triangle);
            // if (m_triangles.size()>2) CRCore::notify(CRCore::NOTICE)<<"Warning too many traingles ("<<m_triangles.size()<<") sharing edge "<<std::endl;
        }
        
        bool isBoundaryEdge() const
        {
            return m_triangles.size()<=1;
        }
        
        bool isAdjacentToBoundary() const
        {
            return isBoundaryEdge() || m_p1->isBoundaryPoint() || m_p2->isBoundaryPoint(); 
        }
        

        void updateMaxNormalDeviationOnEdgeCollapse()
        {
            //CRCore::notify(CRCore::NOTICE)<<"updateMaxNormalDeviationOnEdgeCollapse()"<<std::endl;
            _maximumDeviation = 0.0f;
            for(TriangleSet::iterator itr1=m_p1->m_triangles.begin();
                itr1!=m_p1->m_triangles.end();
                ++itr1)
            {
                if (m_triangles.count(*itr1)==0)
                {
                    _maximumDeviation = CRCore::maximum(_maximumDeviation, (*itr1)->computeNormalDeviationOnEdgeCollapse(this,_proposedPoint.get()));
                }
            }
            for(TriangleSet::iterator itr2=m_p2->m_triangles.begin();
                itr2!=m_p2->m_triangles.end();
                ++itr2)
            {
                if (m_triangles.count(*itr2)==0)
                {
                    _maximumDeviation = CRCore::maximum(_maximumDeviation, (*itr2)->computeNormalDeviationOnEdgeCollapse(this,_proposedPoint.get()));
                }
            }
        }
        
        float getMaxNormalDeviationOnEdgeCollapse() const { return _maximumDeviation; } 

    };

    struct Triangle : public CRCore::Referenced
    {
        Triangle() {}
        
        void clear()
        {
            m_p1 = 0;
            m_p2 = 0;
            m_p3 = 0;
        
            _e1 = 0;
            _e2 = 0;
            _e3 = 0;
        }

        inline bool operator < (const Triangle& rhs) const
        {
            if (m_p1 < rhs.m_p1) return true;
            if (rhs.m_p1 < m_p1) return false;


            const Point* lhs_lower = m_p2<m_p3 ? m_p2.get() : m_p3.get(); 
            const Point* rhs_lower = rhs.m_p2<rhs.m_p3 ? rhs.m_p2.get() : rhs.m_p3.get(); 

            if (lhs_lower < rhs_lower) return true;
            if (rhs_lower < lhs_lower) return false;

            const Point* lhs_upper = m_p2<m_p3 ? m_p3.get() : m_p2.get(); 
            const Point* rhs_upper = rhs.m_p2<rhs.m_p3 ? rhs.m_p3.get() : rhs.m_p2.get(); 

            return (lhs_upper < rhs_upper);
        }


        void setOrderedPoints(Point* p1, Point* p2, Point* p3)
        {
            Point* points[3];
            points[0] = p1;
            points[1] = p2;
            points[2] = p3;

            // find the lowest value point in the list.
            unsigned int lowest = 0;        
            if (points[1]<points[lowest]) lowest = 1;
            if (points[2]<points[lowest]) lowest = 2;


            m_p1 = points[lowest];
            m_p2 = points[(lowest+1)%3];
            m_p3 = points[(lowest+2)%3];
        }
        
        void update()
        {
            m_plane.set(m_p1->m_vertex,m_p2->m_vertex,m_p3->m_vertex);
            
        }
        
        CRCore::crPlane computeNewPlaneOnEdgeCollapse(Edge* edge,Point* pNew) const
        {
            const Point* p1 = (m_p1==edge->m_p1 || m_p1==edge->m_p2) ? pNew : m_p1.get();  
            const Point* p2 = (m_p2==edge->m_p1 || m_p2==edge->m_p2) ? pNew : m_p2.get();  
            const Point* p3 = (m_p3==edge->m_p1 || m_p3==edge->m_p2) ? pNew : m_p3.get();
            
            return CRCore::crPlane(p1->m_vertex,p2->m_vertex,p3->m_vertex);
        }
        
        // note return 1 - dotproduct, so that deviation is in the range of 0.0 to 2.0, where 0 is coincendent, 1.0 is 90 degrees, and 2.0 is 180 degrees.
        float computeNormalDeviationOnEdgeCollapse(Edge* edge,Point* pNew) const
        {
            const Point* p1 = (m_p1==edge->m_p1 || m_p1==edge->m_p2) ? pNew : m_p1.get();  
            const Point* p2 = (m_p2==edge->m_p1 || m_p2==edge->m_p2) ? pNew : m_p2.get();  
            const Point* p3 = (m_p3==edge->m_p1 || m_p3==edge->m_p2) ? pNew : m_p3.get();
            
            CRCore::crVector3 new_normal = (p2->m_vertex - p1->m_vertex) ^ (p3->m_vertex - p2->m_vertex);
            new_normal.normalize();
            
            float result = 1.0f - (new_normal.x() * m_plane[0] + new_normal.y() * m_plane[1] + new_normal.z() * m_plane[2]);
            return result;
        }

        float distance(const CRCore::crVector3& vertex) const
        {
            return m_plane.distance(vertex);
        }
        
        bool isBoundaryTriangle() const
        {
            return (_e1->isBoundaryEdge() || _e2->isBoundaryEdge() ||  _e3->isBoundaryEdge());
        }

       
        CRCore::ref_ptr<Point> m_p1;
        CRCore::ref_ptr<Point> m_p2;
        CRCore::ref_ptr<Point> m_p3;
        
        CRCore::ref_ptr<Edge> _e1;
        CRCore::ref_ptr<Edge> _e2;
        CRCore::ref_ptr<Edge> _e3;
        
        CRCore::crPlane m_plane;

    };


    Triangle* addTriangle(unsigned int p1, unsigned int p2, unsigned int p3)
    {
        //CRCore::notify(CRCore::NOTICE)<<"addTriangle("<<p1<<","<<p2<<","<<p3<<")"<<std::endl;

        // detect if triangle is degenerate.
        if (p1==p2 || p2==p3 || p1==p3) return 0;
        
        Triangle* triangle = new Triangle;

        Point* points[3];
        points[0] = addPoint(triangle, p1);
        points[1] = addPoint(triangle, p2);
        points[2] = addPoint(triangle, p3);
        
        // find the lowest value point in the list.
        unsigned int lowest = 0;        
        if (points[1]<points[lowest]) lowest = 1;
        if (points[2]<points[lowest]) lowest = 2;
        

        triangle->m_p1 = points[lowest];
        triangle->m_p2 = points[(lowest+1)%3];
        triangle->m_p3 = points[(lowest+2)%3];

        triangle->_e1 = addEdge(triangle, triangle->m_p1.get(), triangle->m_p2.get());
        triangle->_e2 = addEdge(triangle, triangle->m_p2.get(), triangle->m_p3.get());
        triangle->_e3 = addEdge(triangle, triangle->m_p3.get(), triangle->m_p1.get());
        
        triangle->update();

        m_triangleSet.insert(triangle);
        
        return triangle;
    }
    
    Triangle* addTriangle(Point* p1, Point* p2, Point* p3)
    {
        // CRCore::notify(CRCore::NOTICE)<<"addTriangle("<<p1<<","<<p2<<","<<p3<<")"<<std::endl;

        // detect if triangle is degenerate.
        if (p1==p2 || p2==p3 || p1==p3) 
        {
            CRCore::notify(CRCore::NOTICE)<<"    **** addTriangle failed - p1==p2 || p2==p3 || p1==p3"<<std::endl;
            return 0;
        }
        
        Triangle* triangle = new Triangle;

        Point* points[3];
        points[0] = addPoint(triangle, p1);
        points[1] = addPoint(triangle, p2);
        points[2] = addPoint(triangle, p3);
        
        // find the lowest value point in the list.
        unsigned int lowest = 0;        
        if (points[1]<points[lowest]) lowest = 1;
        if (points[2]<points[lowest]) lowest = 2;
        

        triangle->m_p1 = points[lowest];
        triangle->m_p2 = points[(lowest+1)%3];
        triangle->m_p3 = points[(lowest+2)%3];

        triangle->_e1 = addEdge(triangle, triangle->m_p1.get(), triangle->m_p2.get());
        triangle->_e2 = addEdge(triangle, triangle->m_p2.get(), triangle->m_p3.get());
        triangle->_e3 = addEdge(triangle, triangle->m_p3.get(), triangle->m_p1.get());
        
        triangle->update();

        m_triangleSet.insert(triangle);

        return triangle;
    }

    void removeTriangle(Triangle* triangle)
    {
        if (triangle->m_p1.valid()) removePoint(triangle,triangle->m_p1.get());
        if (triangle->m_p2.valid()) removePoint(triangle,triangle->m_p2.get());
        if (triangle->m_p3.valid()) removePoint(triangle,triangle->m_p3.get());
        
        if (triangle->_e1.valid()) removeEdge(triangle,triangle->_e1.get());
        if (triangle->_e2.valid()) removeEdge(triangle,triangle->_e2.get());
        if (triangle->_e3.valid()) removeEdge(triangle,triangle->_e3.get());

        m_triangleSet.erase(triangle);
    }

    void replaceTrianglePoint(Triangle* triangle, Point* pOriginal, Point* pNew)
    {
        if (triangle->m_p1==pOriginal || triangle->m_p2==pOriginal || triangle->m_p3==pOriginal)
        {
            // fix the corner points to use the new point
            if (triangle->m_p1==pOriginal) triangle->m_p1=pNew;
            if (triangle->m_p2==pOriginal) triangle->m_p2=pNew;
            if (triangle->m_p3==pOriginal) triangle->m_p3=pNew;
            
            // fixes the edges so they point to use the new point
            triangle->_e1 = replaceEdgePoint(triangle->_e1.get(),pOriginal,pNew);
            triangle->_e2 = replaceEdgePoint(triangle->_e2.get(),pOriginal,pNew);
            triangle->_e3 = replaceEdgePoint(triangle->_e3.get(),pOriginal,pNew);
            
            // remove the triangle form the orignal point, and possibly the point if its the last triangle to use it
            removePoint(triangle, pOriginal);
            
            // add the triangle to that point
            addPoint(triangle,pNew);
        }
        
    }
    
    unsigned int testTriangle(Triangle* triangle)
    {
        unsigned int result = 0;
        if (!(triangle->m_p1))
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p1==NULL"<<std::endl;
            ++result;
        }
        else if (triangle->m_p1->m_triangles.count(triangle)==0) 
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p1->m_triangles does not contain triangle"<<std::endl;
            ++result;
        }

        if (!(triangle->m_p2))
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p2==NULL"<<std::endl;
            ++result;
        }
        else if (triangle->m_p2->m_triangles.count(triangle)==0) 
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p2->m_triangles does not contain triangle"<<std::endl;
            ++result;
        }

        if (!(triangle->m_p3))
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p3==NULL"<<std::endl;
            ++result;
        }
        else if (triangle->m_p3->m_triangles.count(triangle)==0) 
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") m_p3->m_triangles does not contain triangle"<<std::endl;
            ++result;
        }
        
        if (testEdge(triangle->_e1.get()))
        {
            ++result;
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") _e1 test failed"<<std::endl;
        }
        
        if (testEdge(triangle->_e2.get()))
        {
            ++result;
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") _e2 test failed"<<std::endl;
        }

        if (testEdge(triangle->_e3.get()))
        {
            CRCore::notify(CRCore::NOTICE)<<"testTriangle("<<triangle<<") _e3 test failed"<<std::endl;
            ++result;
        }

        return result;
    }

    unsigned int testAllTriangles()
    {
        unsigned int numErrors = 0;
        for(TriangleSet::iterator itr=m_triangleSet.begin();
            itr!=m_triangleSet.end();
            ++itr)
        {
            numErrors += testTriangle(const_cast<Triangle*>(itr->get()));
        }
        return numErrors;
    }

    Edge* addEdge(Triangle* triangle, Point* p1, Point* p2)
    {
        //CRCore::notify(CRCore::NOTICE)<<"addEdge("<<p1<<","<<p2<<")"<<std::endl;
        CRCore::ref_ptr<Edge> edge = new Edge;
        if (p1<p2)
        {
            edge->m_p1 = p1;
            edge->m_p2 = p2;
        }
        else
        {
            edge->m_p1 = p2;
            edge->m_p2 = p1;
        }
        
        EdgeSet::iterator itr = _edgeSet.find(edge);
        if (itr==_edgeSet.end())
        {
            // CRCore::notify(CRCore::NOTICE)<<"  addEdge("<<edge.get()<<") edge->m_p1="<<edge->m_p1.get()<<" m_p2="<<edge->m_p2.get()<<std::endl;
            _edgeSet.insert(edge);
        }
        else
        {
            // CRCore::notify(CRCore::NOTICE)<<"  reuseEdge("<<edge.get()<<") edge->m_p1="<<edge->m_p1.get()<<" m_p2="<<edge->m_p2.get()<<std::endl;
            edge = *itr;
        }
        
        edge->addTriangle(triangle);
        
        return edge.get();
    }

    void removeEdge(Triangle* triangle, Edge* edge)
    {
        EdgeSet::iterator itr = _edgeSet.find(edge);
        if (itr!=_edgeSet.end())
        {
            edge->m_triangles.erase(triangle);
            if (edge->m_triangles.empty())
            {
                // edge no longer in use, so need to delete.
                _edgeSet.erase(itr);

                edge->m_p1 = 0;
                edge->m_p2 = 0;
            }
        }
    }

    Edge* replaceEdgePoint(Edge* edge, Point* pOriginal, Point* pNew)
    {
        if (edge->m_p1==pOriginal || edge->m_p2==pOriginal)
        {
            EdgeSet::iterator itr = _edgeSet.find(edge);
            if (itr!=_edgeSet.end())
            {
                // remove the edge from the list, as its positoin in the list
                // may need to change once its values have been ammended 
                _edgeSet.erase(itr);
            }
            
            // modify its values
            if (edge->m_p1==pOriginal) edge->m_p1=pNew;
            if (edge->m_p2==pOriginal) edge->m_p2=pNew;

            if (edge->m_p2 > edge->m_p1)
            {
                // swap to ensure that lowest ptr value of p1, p2 pair is first.
                CRCore::ref_ptr<Point> tmp = edge->m_p1;
                edge->m_p1 = edge->m_p2;
                edge->m_p2 = tmp;
            }

            itr = _edgeSet.find(edge);
            if (itr!=_edgeSet.end())
            {
                // reuse existing edge.
                edge = const_cast<Edge*>(itr->get());
            }
            else
            {
                // put it back in.
                _edgeSet.insert(edge);
            }
            return edge;
        }
        else
        {
            return edge;
        }
        
    }


    bool collapseEdge(Edge* edge, Point* pNew)
    {
        //if (edge->m_triangles.size()<2) return false;
        //if (edge->m_triangles.size()>2) return false;

        if (edge->getMaxNormalDeviationOnEdgeCollapse()>1.0)
        {
//            CRCore::notify(CRCore::NOTICE)<<"collapseEdge("<<edge<<") refused due to edge->getMaxNormalDeviationOnEdgeCollapse() = "<<edge->getMaxNormalDeviationOnEdgeCollapse()<<std::endl;
            return false;
        }
        else
        {
            //CRCore::notify(CRCore::NOTICE)<<"collapseEdge("<<edge<<") edge->getMaxNormalDeviationOnEdgeCollapse() = "<<edge->getMaxNormalDeviationOnEdgeCollapse()<<std::endl;
        }


        typedef std::set< CRCore::ref_ptr<Edge> > LocalEdgeList;

        CRCore::ref_ptr<Edge> keep_edge_locally_referenced_to_prevent_premature_deletion = edge;
        CRCore::ref_ptr<Point> keep_point_locally_referenced_to_prevent_premature_deletion = pNew;
        CRCore::ref_ptr<Point> edge_p1 = edge->m_p1;
        CRCore::ref_ptr<Point> edge_p2 = edge->m_p2;
        
        TriangleMap  triangleMap;
        TriangleList triangles_p1;
        TriangleList triangles_p2;
        LocalEdgeList oldEdges;
        
        
        if (edge_p1 != pNew)
        {
            for(TriangleSet::iterator itr=edge_p1->m_triangles.begin();
                itr!=edge_p1->m_triangles.end();
                ++itr)
            {
                if (edge->m_triangles.count(*itr)==0)
                {
                    Triangle* triangle = const_cast<Triangle*>(itr->get());
                    triangles_p1.push_back(triangle);
                    oldEdges.insert(triangle->_e1);
                    oldEdges.insert(triangle->_e2);
                    oldEdges.insert(triangle->_e3);
                }
            }
            
            //triangles_p1 = edge_p1->m_triangles;
        }
                
        if (edge_p2 != pNew)
        {
            for(TriangleSet::iterator itr=edge_p2->m_triangles.begin();
                itr!=edge_p2->m_triangles.end();
                ++itr)
            {
                if (edge->m_triangles.count(*itr)==0)
                {
                    Triangle* triangle = const_cast<Triangle*>(itr->get());
                    triangles_p2.push_back(triangle);
                    oldEdges.insert(triangle->_e1);
                    oldEdges.insert(triangle->_e2);
                    oldEdges.insert(triangle->_e3);
                }
            }
            //triangles_p2 = edge_p2->m_triangles;
        }

        for(LocalEdgeList::iterator oeitr=oldEdges.begin();
            oeitr!=oldEdges.end();
            ++oeitr)
        {
            _edgeSet.erase(*oeitr);
            
            const_cast<Edge*>(oeitr->get())->setErrorMetric(0.0f);
            
            _edgeSet.insert(*oeitr);
        }

        TriangleList::iterator titr_p1, titr_p2;
        
        for(titr_p1 = triangles_p1.begin();
            titr_p1 != triangles_p1.end();
            ++titr_p1)
        {
            removeTriangle(const_cast<Triangle*>(titr_p1->get()));
        }

        for(titr_p2 = triangles_p2.begin();
            titr_p2 != triangles_p2.end();
            ++titr_p2)
        {
            removeTriangle(const_cast<Triangle*>(titr_p2->get()));
        }

        //CRCore::notify(CRCore::NOTICE)<<"  pNew="<<pNew<<"\tedge_p1"<<edge_p1.get()<<"\tedge_p2"<<edge_p2.get()<<std::endl;
        
        // we copy the edge's m_triangles and interate the copy of the triangle set to avoid invalidating iterators.
        TriangleSet trianglesToRemove = edge->m_triangles;
        for(TriangleSet::iterator teitr=trianglesToRemove.begin();
            teitr!=trianglesToRemove.end();
            ++teitr)
        {
            Triangle* triangle = const_cast<Triangle*>(teitr->get());
            removeTriangle(triangle);
        }

        LocalEdgeList newEdges;

 
        for(titr_p1 = triangles_p1.begin();
            titr_p1 != triangles_p1.end();
            ++titr_p1)
        {
            Triangle* triangle = const_cast<Triangle*>(titr_p1->get());

            Point* p1 = (triangle->m_p1==edge_p1 || triangle->m_p1==edge_p2)? pNew : triangle->m_p1.get();
            Point* p2 = (triangle->m_p2==edge_p1 || triangle->m_p2==edge_p2)? pNew : triangle->m_p2.get();
            Point* p3 = (triangle->m_p3==edge_p1 || triangle->m_p3==edge_p2)? pNew : triangle->m_p3.get();

            Triangle* newTri = addTriangle(p1,p2,p3);

            if (newTri)
            {
                newEdges.insert(newTri->_e1);
                newEdges.insert(newTri->_e2);
                newEdges.insert(newTri->_e3);
            }
        }


        for(titr_p2 = triangles_p2.begin();
            titr_p2 != triangles_p2.end();
            ++titr_p2)
        {
            Triangle* triangle = const_cast<Triangle*>(titr_p2->get());

            Point* p1 = (triangle->m_p1==edge_p1 || triangle->m_p1==edge_p2)? pNew : triangle->m_p1.get();
            Point* p2 = (triangle->m_p2==edge_p1 || triangle->m_p2==edge_p2)? pNew : triangle->m_p2.get();
            Point* p3 = (triangle->m_p3==edge_p1 || triangle->m_p3==edge_p2)? pNew : triangle->m_p3.get();

            Triangle* newTri = addTriangle(p1,p2,p3);

            if (newTri)
            {
                newEdges.insert(newTri->_e1);
                newEdges.insert(newTri->_e2);
                newEdges.insert(newTri->_e3);
            }
        }



        // CRCore::notify(CRCore::NOTICE)<<"Edges to recalibarate "<<newEdges.size()<<std::endl;

        for(LocalEdgeList::iterator itr=newEdges.begin();
            itr!=newEdges.end();
            ++itr)
        {
            //CRCore::notify(CRCore::NOTICE)<<"updateErrorMetricForEdge("<<itr->get()<<")"<<std::endl;
            updateErrorMetricForEdge(const_cast<Edge*>(itr->get()));
        }

         return true;
    }

    unsigned int testEdge(Edge* edge)
    {
        unsigned int numErrors = 0;
        for(TriangleSet::iterator teitr=edge->m_triangles.begin();
            teitr!=edge->m_triangles.end();
            ++teitr)
        {
            Triangle* triangle = const_cast<Triangle*>(teitr->get());
            if (!(triangle->_e1 == edge || triangle->_e2 == edge || triangle->_e3 == edge))
            {
                CRCore::notify(CRCore::NOTICE)<<"testEdge("<<edge<<"). triangle != point back to this edge"<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"                     triangle->_e1=="<<triangle->_e1.get()<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"                     triangle->_e2=="<<triangle->_e2.get()<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"                     triangle->_e3=="<<triangle->_e3.get()<<std::endl;
                ++numErrors;
            }
        }
        
        if (edge->m_triangles.empty())
        {
            CRCore::notify(CRCore::NOTICE)<<"testEdge("<<edge<<").m_triangles is empty"<<std::endl;
            ++numErrors;
        }
        return numErrors;
    }

    unsigned int testAllEdges()
    {
        unsigned int numErrors = 0;
        for(EdgeSet::iterator itr = _edgeSet.begin();
            itr!=_edgeSet.end();
            ++itr)
        {
            numErrors += testEdge(const_cast<Edge*>(itr->get()));
        }
        return numErrors;
    }

    unsigned int computeNumBoundaryEdges()
    {
        unsigned int numBoundaryEdges = 0;
        for(EdgeSet::iterator itr = _edgeSet.begin();
            itr!=_edgeSet.end();
            ++itr)
        {
            if ((*itr)->isBoundaryEdge()) ++numBoundaryEdges;
        }
        return numBoundaryEdges;
    }


    Point* addPoint(Triangle* triangle, unsigned int p1)
    {
        return addPoint(triangle,_originalPointList[p1].get());
    }

    Point* addPoint(Triangle* triangle, Point* point)
    {
        
        PointSet::iterator itr = _pointSet.find(point);
        if (itr==_pointSet.end())
        {
            //CRCore::notify(CRCore::NOTICE)<<"  addPoint("<<point.get()<<")"<<std::endl;
            _pointSet.insert(point);
        }
        else
        {
            point = const_cast<Point*>(itr->get());
            //CRCore::notify(CRCore::NOTICE)<<"  reusePoint("<<point.get()<<")"<<std::endl;
        }

        point->m_triangles.insert(triangle);
        
        return point;
    }

    void removePoint(Triangle* triangle, Point* point)
    {
        PointSet::iterator itr = _pointSet.find(point);
        if (itr!=_pointSet.end())
        {
            point->m_triangles.erase(triangle);
            
            if (point->m_triangles.empty())
            {
                // point no longer in use, so need to delete.
                _pointSet.erase(itr);
            }
        }
        
    }
    
    unsigned int testPoint(Point* point)
    {
        unsigned int numErrors = 0;
        
        for(TriangleSet::iterator itr=point->m_triangles.begin();
            itr!=point->m_triangles.end();
            ++itr)
        {
            Triangle* triangle = const_cast<Triangle*>(itr->get());
            if (!(triangle->m_p1 == point || triangle->m_p2 == point || triangle->m_p3 == point))
            {
                CRCore::notify(CRCore::NOTICE)<<"testPoint("<<point<<") error, triangle "<<triangle<<" does not point back to this point"<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"             triangle->m_p1 "<<triangle->m_p1.get()<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"             triangle->m_p2 "<<triangle->m_p2.get()<<std::endl;
                CRCore::notify(CRCore::NOTICE)<<"             triangle->m_p3 "<<triangle->m_p3.get()<<std::endl;
                ++numErrors;
            }
        }
        
        return numErrors;
    }
    
    unsigned int testAllPoints()
    {
        unsigned int numErrors = 0;
        for(PointSet::iterator itr = _pointSet.begin();
            itr!=_pointSet.end();
            ++itr)
        {
            numErrors += testPoint(const_cast<Point*>(itr->get()));
        }
        return numErrors;
    }
    
//protected:

    typedef std::vector< CRCore::ref_ptr<CRCore::Array> > ArrayList;

    CRCore::crGeometry*                  m_geometry;
    
    EdgeSet                         _edgeSet;
    TriangleSet                     m_triangleSet;
    PointSet                        _pointSet;
    PointList                       _originalPointList;
    
};

struct CollectTriangleOperator
{

    CollectTriangleOperator():_ec(0) {}

    void setEdgeCollapse(EdgeCollapse* ec) { _ec = ec; }
    
    EdgeCollapse* _ec;    

    // for use  in the triangle functor.
    inline void operator()(unsigned int p1, unsigned int p2, unsigned int p3)
    {
        _ec->addTriangle(p1,p2,p3);
    }

};

EdgeCollapse::~EdgeCollapse()
{
	std::for_each(_edgeSet.begin(),_edgeSet.end(),dereference_clear());

	std::for_each(m_triangleSet.begin(),m_triangleSet.end(),dereference_clear());
	std::for_each(_pointSet.begin(),_pointSet.end(),dereference_clear());
	std::for_each(_originalPointList.begin(),_originalPointList.end(),dereference_clear());
}


typedef CRCore::crTriangleIndexFunctor<CollectTriangleOperator> CollectTriangleIndexFunctor;

class CopyArrayToPointsVisitor : public CRCore::ArrayVisitor
{
    public:
        CopyArrayToPointsVisitor(EdgeCollapse::PointList& pointList):
            m_pointList(pointList) {}
        
        template<class T>
        void copy(T& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
                m_pointList[i]->m_attributes.push_back((float)array[i]);  
        }
        
        virtual void apply(CRCore::Array&) {}
        virtual void apply(CRCore::ByteArray& array) { copy(array); }
        virtual void apply(CRCore::ShortArray& array) { copy(array); }
        virtual void apply(CRCore::IntArray& array) { copy(array); }
        virtual void apply(CRCore::UByteArray& array) { copy(array); }
        virtual void apply(CRCore::UShortArray& array) { copy(array); }
        virtual void apply(CRCore::UIntArray& array) { copy(array); }
        virtual void apply(CRCore::FloatArray& array) { copy(array); }

        virtual void apply(CRCore::Vec4ubArray& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                CRCore::crVector4ub& value = array[i];
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                attributes.push_back((float)value.r());  
                attributes.push_back((float)value.g());  
                attributes.push_back((float)value.b());  
                attributes.push_back((float)value.a());  
            }
        }

        virtual void apply(CRCore::Vec2Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                CRCore::crVector2& value = array[i];
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                attributes.push_back(value.x());  
                attributes.push_back(value.y());  
            }
        }

        virtual void apply(CRCore::Vec3Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                CRCore::crVector3& value = array[i];
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                attributes.push_back(value.x());  
                attributes.push_back(value.y());  
                attributes.push_back(value.z());  
            }
        }
        
        virtual void apply(CRCore::Vec4Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                CRCore::crVector4& value = array[i];
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                attributes.push_back(value.x());  
                attributes.push_back(value.y());  
                attributes.push_back(value.z());  
                attributes.push_back(value.w());  
            }
        }
        
        EdgeCollapse::PointList& m_pointList;
};

class CopyVertexArrayToPointsVisitor : public CRCore::ArrayVisitor
{
    public:
        CopyVertexArrayToPointsVisitor(EdgeCollapse::PointList& pointList):
            m_pointList(pointList) {}
        
        virtual void apply(CRCore::Vec2Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i] = new EdgeCollapse::Point;
                m_pointList[i]->m_index = i;
                
                CRCore::crVector2& value = array[i];
                CRCore::crVector3& vertex = m_pointList[i]->m_vertex;
                vertex.set(value.x(),value.y(),0.0f);  
            }
        }

        virtual void apply(CRCore::Vec3Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i] = new EdgeCollapse::Point;
                m_pointList[i]->m_index = i;
                
                m_pointList[i]->m_vertex = array[i];
            }
        }
        
        virtual void apply(CRCore::Vec4Array& array)
        {
            if (m_pointList.size()!=array.size()) return;
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i] = new EdgeCollapse::Point;
                m_pointList[i]->m_index = i;
                
                CRCore::crVector4& value = array[i];
                CRCore::crVector3& vertex = m_pointList[i]->m_vertex;
                vertex.set(value.x()/value.w(),value.y()/value.w(),value.z()/value.w());  
            }
        }
        
        EdgeCollapse::PointList& m_pointList;
};

void EdgeCollapse::setGeometry(CRCore::crGeometry* geometry, const crSimplifier::IndexList& protectedPoints)
{
    m_geometry = geometry;
    
    // check to see if vertex attributes indices exists, if so expand them to remove them
    if (m_geometry->suitableForOptimization())
    {
        // removing coord indices
        //CRCore::notify(CRCore::INFO)<<"EdgeCollapse::setGeometry(..): Removing attribute indices"<<std::endl;
        m_geometry->copyToAndOptimize(*m_geometry);
    }

    unsigned int numVertices = geometry->getVertexArray()->getNumElements();
        
    _originalPointList.resize(numVertices);
    
    // copy vertices across to local point list
    CopyVertexArrayToPointsVisitor copyVertexArrayToPoints(_originalPointList);
    m_geometry->getVertexArray()->accept(copyVertexArrayToPoints);
    
    // copy other per vertex attributes across to local point list.
    CopyArrayToPointsVisitor        copyArrayToPoints(_originalPointList);

    for(unsigned int ti=0;ti<m_geometry->getNumTexCoordArrays();++ti)
    {
        if (m_geometry->getTexCoordArray(ti))
            geometry->getTexCoordArray(ti)->accept(copyArrayToPoints);
    }

    if (m_geometry->getNormalArray() && m_geometry->getNormalBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        geometry->getNormalArray()->accept(copyArrayToPoints);
        
    if (m_geometry->getColorArray() && m_geometry->getColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        geometry->getColorArray()->accept(copyArrayToPoints);
        
    if (m_geometry->getSecondaryColorArray() && m_geometry->getSecondaryColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        geometry->getSecondaryColorArray()->accept(copyArrayToPoints);

    if (m_geometry->getFogCoordArray() && m_geometry->getFogCoordBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        geometry->getFogCoordArray()->accept(copyArrayToPoints);

    for(unsigned int vi=0;vi<m_geometry->getNumVertexAttribArrays();++vi)
    {
        if (m_geometry->getVertexAttribArray(vi) &&  m_geometry->getVertexAttribBinding(vi)==CRCore::crGeometry::BIND_PER_VERTEX)
            geometry->getVertexAttribArray(vi)->accept(copyArrayToPoints);
    }

    // now set the protected points up.
    for(crSimplifier::IndexList::const_iterator pitr=protectedPoints.begin();
        pitr!=protectedPoints.end();
        ++pitr)
    {
        _originalPointList[*pitr]->_protected = true;
    }


    CollectTriangleIndexFunctor collectTriangles;
    collectTriangles.setEdgeCollapse(this);
    
    m_geometry->accept(collectTriangles);
    
}
 


class CopyPointsToArrayVisitor : public CRCore::ArrayVisitor
{
    public:
        CopyPointsToArrayVisitor(EdgeCollapse::PointList& pointList):
            m_pointList(pointList),
            m_index(0) {}


        template<typename T,typename R>
        void copy(T& array, R /*dummy*/)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                if (m_index<m_pointList[i]->m_attributes.size()) 
                {
                    float val = (m_pointList[i]->m_attributes[m_index]);
                    array[i] = R (val);
                }
            }
                
            ++m_index;
        }
        
        // use local typedefs if usinged char,short and int to get round gcc 3.3.1 problem with defining unsigned short()
        typedef unsigned char dummy_uchar;
        typedef unsigned short dummy_ushort;
        typedef unsigned int dummy_uint;
        
        virtual void apply(CRCore::Array&) {}
        virtual void apply(CRCore::ByteArray& array) { copy(array, char());}
        virtual void apply(CRCore::ShortArray& array) { copy(array, short()); }
        virtual void apply(CRCore::IntArray& array) { copy(array, int()); }
        virtual void apply(CRCore::UByteArray& array) { copy(array, dummy_uchar()); }
        virtual void apply(CRCore::UShortArray& array) { copy(array,dummy_ushort()); }
        virtual void apply(CRCore::UIntArray& array) { copy(array, dummy_uint()); }
        virtual void apply(CRCore::FloatArray& array) { copy(array, float()); }

        virtual void apply(CRCore::Vec4ubArray& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                array[i].set((unsigned char)attributes[m_index],
                             (unsigned char)attributes[m_index+1],
                             (unsigned char)attributes[m_index+2],
                             (unsigned char)attributes[m_index+3]);
            }
            m_index += 4;
        }

        virtual void apply(CRCore::Vec2Array& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                if (m_index+1<attributes.size()) array[i].set(attributes[m_index],attributes[m_index+1]);
            }
            m_index += 2;
        }

        virtual void apply(CRCore::Vec3Array& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                if (m_index+2<attributes.size()) array[i].set(attributes[m_index],attributes[m_index+1],attributes[m_index+2]);
            }
            m_index += 3;
        }
        
        virtual void apply(CRCore::Vec4Array& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                EdgeCollapse::FloatList& attributes = m_pointList[i]->m_attributes;
                if (m_index+3<attributes.size()) array[i].set(attributes[m_index],attributes[m_index+1],attributes[m_index+2],attributes[m_index+3]);
            }
            m_index += 4;
        }
        
        EdgeCollapse::PointList& m_pointList;
        unsigned int m_index;
};

class NormalizeArrayVisitor : public CRCore::ArrayVisitor
{
    public:
        NormalizeArrayVisitor() {}
        
        template<typename Itr>
        void normalize(Itr begin, Itr end)
        {
            for(Itr itr = begin;
                itr != end;
                ++itr)
            {
                itr->normalize();
            }
        }
        
        virtual void apply(CRCore::Vec2Array& array) { normalize(array.begin(),array.end()); }
        virtual void apply(CRCore::Vec3Array& array) { normalize(array.begin(),array.end()); }
        virtual void apply(CRCore::Vec4Array& array) { normalize(array.begin(),array.end()); }
        
};

class CopyPointsToVertexArrayVisitor : public CRCore::ArrayVisitor
{
    public:
        CopyPointsToVertexArrayVisitor(EdgeCollapse::PointList& pointList):
            m_pointList(pointList) {}
        
        virtual void apply(CRCore::Vec2Array& array)
        {
            array.resize(m_pointList.size());
            
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i]->m_index = i;
                CRCore::crVector3& vertex = m_pointList[i]->m_vertex;
                array[i].set(vertex.x(),vertex.y());
            }
        }

        virtual void apply(CRCore::Vec3Array& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i]->m_index = i;
                array[i] = m_pointList[i]->m_vertex;
            }
        }
        
        virtual void apply(CRCore::Vec4Array& array)
        {
            array.resize(m_pointList.size());
        
            for(unsigned int i=0;i<m_pointList.size();++i) 
            {
                m_pointList[i]->m_index = i;
                CRCore::crVector3& vertex = m_pointList[i]->m_vertex;
                array[i].set(vertex.x(),vertex.y(),vertex.z(),1.0f);
            }
        }
        
        EdgeCollapse::PointList& m_pointList;
};


void EdgeCollapse::copyBackToGeometry()
{

    // rebuild the m_pointList from the _pointSet
    _originalPointList.clear();
    std::copy(_pointSet.begin(),_pointSet.end(),std::back_inserter(_originalPointList));

    // copy vertices across to local point list
    CopyPointsToVertexArrayVisitor copyVertexArrayToPoints(_originalPointList);
    m_geometry->getVertexArray()->accept(copyVertexArrayToPoints);
    
    // copy other per vertex attributes across to local point list.
    CopyPointsToArrayVisitor  copyArrayToPoints(_originalPointList);

    for(unsigned int ti=0;ti<m_geometry->getNumTexCoordArrays();++ti)
    {
        if (m_geometry->getTexCoordArray(ti))
            m_geometry->getTexCoordArray(ti)->accept(copyArrayToPoints);
    }

    if (m_geometry->getNormalArray() && m_geometry->getNormalBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
    {
        m_geometry->getNormalArray()->accept(copyArrayToPoints);

        // now normalize the normals.
        NormalizeArrayVisitor nav;
        m_geometry->getNormalArray()->accept(nav);
    }
        
    if (m_geometry->getColorArray() && m_geometry->getColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        m_geometry->getColorArray()->accept(copyArrayToPoints);
        
    if (m_geometry->getSecondaryColorArray() && m_geometry->getSecondaryColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        m_geometry->getSecondaryColorArray()->accept(copyArrayToPoints);

    if (m_geometry->getFogCoordArray() && m_geometry->getFogCoordBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        m_geometry->getFogCoordArray()->accept(copyArrayToPoints);

    for(unsigned int vi=0;vi<m_geometry->getNumVertexAttribArrays();++vi)
    {
        if (m_geometry->getVertexAttribArray(vi) &&  m_geometry->getVertexAttribBinding(vi)==CRCore::crGeometry::BIND_PER_VERTEX)
            m_geometry->getVertexAttribArray(vi)->accept(copyArrayToPoints);
    }

    CRCore::DrawElementsUInt* primitives = new CRCore::DrawElementsUInt(GL_TRIANGLES,m_triangleSet.size()*3);
    unsigned int pos = 0;
    for(TriangleSet::iterator titr=m_triangleSet.begin();
        titr!=m_triangleSet.end();
        ++titr)
    {
        const Triangle* triangle = (*titr).get();
		CRCore::DrawElementsUInt::PT_IndexArray &indexArray = primitives->getIndexArray();
        indexArray[pos++] = triangle->m_p1->m_index;
        indexArray[pos++] = triangle->m_p2->m_index;
        indexArray[pos++] = triangle->m_p3->m_index;
    }
    
    m_geometry->getPrimitiveList().clear();
    m_geometry->addPrimitive(primitives);

#if 0    
    CRUtil::crSmoothingVisitor::smooth(*m_geometry);
    
    CRUtil::crTriStripVisitor stripper;
    stripper.stripify(*m_geometry);
#endif  
}


crSimplifier::crSimplifier(float sampleRatio, float maximumError):
            CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN),
            m_sampleRatio(sampleRatio),
            m_maximumError(maximumError)
{
}

void crSimplifier::simplify(CRCore::crGeometry& geometry)
{
    // pass an empty list of indices to simply(crGeometry,IndexList)
    // so that this one method handle both cases of non protected indices
    // and specified indices.
    IndexList emptyList;
    simplify(geometry,emptyList);
}

void crSimplifier::simplify(CRCore::crGeometry& geometry, const IndexList& protectedPoints)
{
    //CRCore::notify(CRCore::INFO)<<"++++++++++++++simplifier************"<<std::endl;

    EdgeCollapse ec;
    ec.setGeometry(&geometry, protectedPoints);

    ec.updateErrorMetricForAllEdges();

    unsigned int numOriginalPrimitives = ec.m_triangleSet.size();

    while (!ec._edgeSet.empty() &&
           continueSimplification((*ec._edgeSet.begin())->getErrorMetric() , numOriginalPrimitives, ec.m_triangleSet.size()) && 
           ec.collapseMinimumErrorEdge())
    {
       //CRCore::notify(CRCore::INFO)<<"   Collapsed edge ec.m_triangleSet.size()="<<ec.m_triangleSet.size()<<" error="<<(*ec._edgeSet.begin())->getErrorMetric()<<" vs "<<getMaximumError()<<std::endl;
    }

//    CRCore::notify(CRCore::INFO)<<"******* AFTER EDGE COLLAPSE *********"<<ec.m_triangleSet.size()<<std::endl;
//
//    CRCore::notify(CRCore::INFO)<<"Number of triangle errors after edge collapse= "<<ec.testAllTriangles()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of edge errors before edge collapse= "<<ec.testAllEdges()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of point errors after edge collapse= "<<ec.testAllPoints()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of triangles= "<<ec.m_triangleSet.size()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of points= "<<ec._pointSet.size()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of edges= "<<ec._edgeSet.size()<<std::endl;
//    CRCore::notify(CRCore::INFO)<<"Number of boundary edges= "<<ec.computeNumBoundaryEdges()<<std::endl;
//
//    //CRCore::notify(CRCore::INFO)<<std::endl<<"crSimplifier, in = "<<numOriginalPrimitives<<"\tout = "<<ec.m_triangleSet.size()<<"\terror="<<(*ec._edgeSet.begin())->getErrorMetric()<<"\tvs "<<getMaximumError()<<std::endl<<std::endl;
//    CRCore::notify(CRCore::INFO)<<           "        !ec._edgeSet.empty()  = "<<!ec._edgeSet.empty()<<std::endl;
////    CRCore::notify(CRCore::INFO)<<           "        continueSimplification(,,)  = "<<continueSimplification((*ec._edgeSet.begin())->getErrorMetric() , numOriginalPrimitives, ec.m_triangleSet.size())<<std::endl;
    
    ec.copyBackToGeometry();
}
