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
#include <CRCore/crTriangleFunctor.h>
#include <CRCore/crTriangleIndexFunctor.h>
#include <CRUtil/crSmoothingVisitor.h>

#include <stdio.h>
#include <list>
#include <set>


using namespace CRCore;
using namespace CRUtil;

namespace Smoother
{

struct LessPtr
{
    inline bool operator() (const CRCore::crVector3* lhs,const CRCore::crVector3* rhs) const
    {
        return *lhs<*rhs;
    }
};

// triangle functor.
struct SmoothTriangleFunctor
{

    CRCore::crVector3* m_coordBase;
    CRCore::crVector3* m_normalBase;

    typedef std::multiset<const CRCore::crVector3*,LessPtr> CoordinateSet;
    CoordinateSet m_coordSet;

    SmoothTriangleFunctor():
         m_coordBase(0),
         m_normalBase(0) {}
    
    void set(CRCore::crVector3 *cb,int noVertices, CRCore::crVector3 *nb)
    {
        m_coordBase=cb;
        m_normalBase=nb;

        CRCore::crVector3* vptr = cb;
        for(int i=0;i<noVertices;++i)
        {
            m_coordSet.insert(vptr++);
        }
    }

    inline void updateNormal(const CRCore::crVector3& normal,const CRCore::crVector3* vptr)
    {
        std::pair<CoordinateSet::iterator,CoordinateSet::iterator> p =
            m_coordSet.equal_range(vptr);

        for(CoordinateSet::iterator itr=p.first;
            itr!=p.second;
            ++itr)
        {
            CRCore::crVector3* nptr = m_normalBase + (*itr-m_coordBase);
            (*nptr) += normal;
        }
    }

    inline void operator() ( const CRCore::crVector3 &v1, const CRCore::crVector3 &v2, const CRCore::crVector3 &v3, bool treatVertexDataAsTemporary )
    {
        if (!treatVertexDataAsTemporary)
        {
            // calc orientation of triangle.
            CRCore::crVector3 normal = (v2-v1)^(v3-v1);
            // normal.normalize();

            updateNormal(normal,&v1);
            updateNormal(normal,&v2);
            updateNormal(normal,&v3);
        }

    }
};

static void smooth_old(CRCore::crGeometry& geom)
{
    crGeometry::PrimitiveList& primitives = geom.getPrimitiveList();
    crGeometry::PrimitiveList::iterator itr;
    unsigned int numSurfacePrimitives=0;
    for(itr=primitives.begin();
        itr!=primitives.end();
        ++itr)
    {
        switch((*itr)->getMode())
        {
            case(crPrimitive::PT_TRIANGLES):
            case(crPrimitive::PT_TRIANGLE_STRIP):
            case(crPrimitive::PT_TRIANGLE_FAN):
            case(crPrimitive::PT_QUADS):
            case(crPrimitive::PT_QUAD_STRIP):
            case(crPrimitive::PT_POLYGON):
                ++numSurfacePrimitives;
                break;
            default:
                break;
        }
    }

    if (!numSurfacePrimitives) return;

    CRCore::Vec3Array *coords = dynamic_cast<CRCore::Vec3Array*>(geom.getVertexArray());
    if (!coords || !coords->size()) return;

    CRCore::Vec3Array *normals = new CRCore::Vec3Array(coords->size());

    CRCore::Vec3Array::iterator nitr;
    for(nitr = normals->begin();
        nitr!=normals->end();
        ++nitr)
    {
        nitr->set(0.0f,0.0f,0.0f);
    }

    crTriangleFunctor<SmoothTriangleFunctor> stf;
    stf.set(&(coords->front()),coords->size(),&(normals->front()));

    geom.accept(stf);

    for(nitr= normals->begin();
        nitr!=normals->end();
        ++nitr)
    {
        nitr->normalize();
    }
    geom.setNormalArray( normals );
    geom.setNormalIndices( geom.getVertexIndices() );
    geom.setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);


    geom.dirtyDisplayList();
}


struct SmoothTriangleIndexFunctor
{
    SmoothTriangleIndexFunctor():
        m_vertices(0),
        m_normals(0)
    {
    }

    bool set(CRCore::Vec3Array* vertices, CRCore::Vec3Array* normals)
    {
        m_vertices = vertices;
        m_normals = normals;

        if (!m_vertices)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: SmoothTriangleIndexFunctor::set(..) requires a valid vertex arrays."<<std::endl;
            return false;
        }

        if (!m_normals)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: SmoothTriangleIndexFunctor::set(..) requires a valid normal arrays."<<std::endl;
            return false;
        }

        for(CRCore::Vec3Array::iterator itr = m_normals->begin();
            itr != m_normals->end();
            ++itr)
        {
            (*itr).set(0.0f,0.0f,0.0f);
        }

        return true;
    }

    void normalize()
    {
        if (!m_normals) return;

        for(CRCore::Vec3Array::iterator itr = m_normals->begin();
            itr != m_normals->end();
            ++itr)
        {
            (*itr).normalize();
        }
    }

    void operator() (unsigned int p1, unsigned int p2, unsigned int p3)
    {
        if (p1==p2 || p2==p3 || p1==p3)
        {
            return;
        }

        const CRCore::crVector3& v1 = (*m_vertices)[p1];
        const CRCore::crVector3& v2 = (*m_vertices)[p2];
        const CRCore::crVector3& v3 = (*m_vertices)[p3];
        CRCore::crVector3 normal( (v2-v1)^(v3-v1) );

        normal.normalize();

        (*m_normals)[p1] += normal;
        (*m_normals)[p2] += normal;
        (*m_normals)[p3] += normal;
    }

    CRCore::Vec3Array*     m_vertices;
    CRCore::Vec3Array*     m_normals;
};



struct FindSharpEdgesFunctor
{
    FindSharpEdgesFunctor():
        m_vertices(0),
        m_normals(0),
        m_maxDeviationDotProduct(0.0f),
        m_currentPrimitiveSetIndex(0)
    {
    }

    struct Triangle : public CRCore::Referenced
    {
        Triangle(unsigned int primitiveSetIndex, unsigned int p1, unsigned int p2, unsigned int p3):
            m_primitiveSetIndex(primitiveSetIndex), m_p1(p1), m_p2(p2), m_p3(p3) {}

        Triangle(const Triangle& tri):
            m_primitiveSetIndex(tri.m_primitiveSetIndex), m_p1(tri.m_p1), m_p2(tri.m_p2), m_p3(tri.m_p3) {}

        Triangle& operator = (const Triangle& tri)
        {
            m_primitiveSetIndex = tri.m_primitiveSetIndex;
            m_p1 = tri.m_p1;
            m_p2 = tri.m_p2;
            m_p3 = tri.m_p3;
            return *this;
        }

        unsigned int m_primitiveSetIndex;
        unsigned int m_p1;
        unsigned int m_p2;
        unsigned int m_p3;
    };

    typedef std::list< CRCore::ref_ptr<Triangle> > Triangles;

    struct ProblemVertex : public CRCore::Referenced
    {
        ProblemVertex(unsigned int p):
            m_point(p) {}

        unsigned int m_point;
        Triangles m_triangles;
    };

    typedef std::vector< CRCore::ref_ptr<ProblemVertex> > ProblemVertexVector;
    typedef std::list< CRCore::ref_ptr<ProblemVertex> > ProblemVertexList;
    typedef std::list< CRCore::ref_ptr<CRCore::Array> > ArrayList;

    bool set(CRCore::crGeometry* geom, float creaseAngle)
    {
        m_geometry = geom;
        m_creaseAngle = creaseAngle;

        if (!m_geometry)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: SmoothTriangleIndexFunctor::set(..) requires a geometry."<<std::endl;
            return false;
        }

        m_vertices = dynamic_cast<CRCore::Vec3Array*>(m_geometry->getVertexArray());
        m_normals = dynamic_cast<CRCore::Vec3Array*>(m_geometry->getNormalArray());
        m_maxDeviationDotProduct = cos(m_creaseAngle*0.5);

        if (!m_vertices)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: SmoothTriangleIndexFunctor::set(..) requires a valid vertex arrays."<<std::endl;
            return false;
        }

        if (!m_normals)
        {
            CRCore::notify(CRCore::NOTICE)<<"Warning: SmoothTriangleIndexFunctor::set(..) requires a valid normal arrays."<<std::endl;
            return false;
        }

        m_problemVertexVector.resize(m_vertices->size());

        addArray(geom->getVertexArray(), CRCore::crGeometry::BIND_PER_VERTEX);
        addArray(geom->getNormalArray(), geom->getNormalBinding());
        addArray(geom->getColorArray(), geom->getColorBinding());
        addArray(geom->getSecondaryColorArray(), geom->getSecondaryColorBinding());
        addArray(geom->getFogCoordArray(), geom->getFogCoordBinding());

        for(unsigned int i=0; i<geom->getNumTexCoordArrays(); ++i)
        {
            addArray(geom->getTexCoordArray(i), CRCore::crGeometry::BIND_PER_VERTEX);
        }

        return true;
    }

    void addArray(CRCore::Array* array, CRCore::crGeometry::AttributeBinding binding)
    {
        if (array && binding==CRCore::crGeometry::BIND_PER_VERTEX)
        {
            m_arrays.push_back(array);
        }
    }

    void operator() (unsigned int p1, unsigned int p2, unsigned int p3)
    {
        CRCore::crVector3 normal( computeNormal(p1, p2, p3) );

        if (p1==p2 || p2==p3 || p1==p3)
        {
            // CRCore::notify(CRCore::NOTICE)<<"NULL triangle ("<<p1<<", "<<p2<<", "<<p3<<")"<<std::endl;
            return;
        }

        Triangle* tri = new Triangle(m_currentPrimitiveSetIndex, p1, p2, p3);
        m_triangles.push_back(tri);

        if (checkDeviation(p1, normal)) markProblemVertex(p1);
        if (checkDeviation(p2, normal)) markProblemVertex(p2);
        if (checkDeviation(p3, normal)) markProblemVertex(p3);
    }

    void markProblemVertex(unsigned int p)
    {
        if (!m_problemVertexVector[p])
        {
            m_problemVertexVector[p] = new ProblemVertex(p);
            m_problemVertexList.push_back(m_problemVertexVector[p]);
        }
    }

    void checkTrianglesForProblemVertices()
    {
        for(Triangles::iterator itr = m_triangles.begin();
            itr != m_triangles.end();
            ++itr)
        {
            Triangle* tri = itr->get();
            insertTriangleIfProblemVertex(tri->m_p1, tri);
            insertTriangleIfProblemVertex(tri->m_p2, tri);
            insertTriangleIfProblemVertex(tri->m_p3, tri);
        }
    }

    void insertTriangleIfProblemVertex(unsigned int p, Triangle* tri)
    {
        if (m_problemVertexVector[p].valid())
		{
			m_problemVertexVector[p]->m_triangles.push_back(tri);
		}
    }

    bool checkDeviation(unsigned int p, CRCore::crVector3& normal)
    {
        float deviation = normal * (*m_normals)[p];
        return (deviation < m_maxDeviationDotProduct);
    }

    CRCore::crVector3 computeNormal(unsigned int p1, unsigned int p2, unsigned int p3)
    {
        const CRCore::crVector3& v1 = (*m_vertices)[p1];
        const CRCore::crVector3& v2 = (*m_vertices)[p2];
        const CRCore::crVector3& v3 = (*m_vertices)[p3];
        CRCore::crVector3 normal( (v2-v1)^(v3-v1) );
        normal.normalize();
        return normal;
    }

    void listProblemVertices()
    {
        CRCore::notify(CRCore::NOTICE)<<"listProblemVertices() "<<m_problemVertexList.size()<<std::endl;
        for(ProblemVertexList::iterator itr = m_problemVertexList.begin();
            itr != m_problemVertexList.end();
            ++itr)
        {
            ProblemVertex* pv = itr->get();
            CRCore::notify(CRCore::NOTICE)<<"  pv.m_point = "<<pv->m_point<<" triangles "<<pv->m_triangles.size()<<std::endl;
            for(Triangles::iterator titr = pv->m_triangles.begin();
                titr != pv->m_triangles.end();
                ++titr)
            {
                Triangle* tri = titr->get();
                CRCore::notify(CRCore::NOTICE)<<"    triangle("<<tri->m_p1<<", "<<tri->m_p2<<", "<<tri->m_p3<<")"<<std::endl;
                CRCore::crVector3 normal( computeNormal(tri->m_p1, tri->m_p2, tri->m_p3) );
                float deviation = normal * (*m_normals)[pv->m_point];
                CRCore::notify(CRCore::NOTICE)<<"    deviation "<<deviation<<std::endl;
            }

        }
    }

    class DuplicateVertex : public CRCore::ArrayVisitor
    {
        public:

            unsigned int _i;
            unsigned int _end;

            DuplicateVertex(unsigned int i):
                                _i(i),
                                _end(i) {}

            template <class ARRAY>
            void apply_imp(ARRAY& array)
            {
                _end = array.size();
                array.push_back(array[_i]);
            }

            virtual void apply(CRCore::ByteArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::ShortArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::IntArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::UByteArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::UShortArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::UIntArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::Vec4ubArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::FloatArray& ba) { apply_imp(ba); }
            virtual void apply(CRCore::Vec2Array& ba) { apply_imp(ba); }
            virtual void apply(CRCore::Vec3Array& ba) { apply_imp(ba); }
            virtual void apply(CRCore::Vec4Array& ba) { apply_imp(ba); }

    };

    unsigned int duplicateVertex(unsigned int i)
    {
        DuplicateVertex duplicate(i);
        for(ArrayList::iterator aItr = m_arrays.begin();
            aItr != m_arrays.end();
            ++aItr)
        {
            (*aItr)->accept(duplicate);
        }
        return duplicate._end;
    }

    void duplicateProblemVertexAll(ProblemVertex* pv)
    {
        unsigned int p = pv->m_point;
        Triangles::iterator titr = pv->m_triangles.begin();
        ++titr;
        for(;
            titr != pv->m_triangles.end();
            ++titr)
        {
            Triangle* tri = titr->get();
            unsigned int duplicated_p = duplicateVertex(p);
            if (tri->m_p1==p) tri->m_p1 = duplicated_p;
            if (tri->m_p2==p) tri->m_p2 = duplicated_p;
            if (tri->m_p3==p) tri->m_p3 = duplicated_p;
        }
    }

    void duplicateProblemVertex(ProblemVertex* pv)
    {
        if (pv->m_triangles.size()<=2)
        {
            duplicateProblemVertexAll(pv);
        }
        else
        {
            // implement a form of greedy association based on similar orientation
            // rather than iterating through all the various permutation of triangles that might
            // provide the best fit.
            unsigned int p = pv->m_point;
            Triangles::iterator titr = pv->m_triangles.begin();
            while(titr != pv->m_triangles.end())
            {
                Triangle* tri = titr->get();
                CRCore::crVector3 normal = computeNormal(tri->m_p1, tri->m_p2, tri->m_p3);

                Triangles associatedTriangles;
                associatedTriangles.push_back(tri);

                // remove triangle for list
                pv->m_triangles.erase(titr);

                // reset iterator
                titr = pv->m_triangles.begin();
                while(titr != pv->m_triangles.end())
                {
                    Triangle* tri2 = titr->get();
                    CRCore::crVector3 normal2 = computeNormal(tri2->m_p1, tri2->m_p2, tri2->m_p3);
                    float deviation = normal * normal2;
                    if (deviation >= m_maxDeviationDotProduct)
                    {
                        // Tri and tri2 are close enough together to associate.
                        associatedTriangles.push_back(tri2);

                        Triangles::iterator titr_to_erase = titr;

                        ++titr;
                        pv->m_triangles.erase(titr_to_erase);
                    }
                    else
                    {
                        ++titr;
                    }
                }

                // create duplicate vertex to set of associated triangles
                unsigned int duplicated_p = duplicateVertex(p);

                // now rest the index on th triangles of the point that was duplicated
                for(Triangles::iterator aitr = associatedTriangles.begin();
                    aitr != associatedTriangles.end();
                    ++aitr)
                {
                    Triangle* tri = aitr->get();
                    if (tri->m_p1==p) tri->m_p1 = duplicated_p;
                    if (tri->m_p2==p) tri->m_p2 = duplicated_p;
                    if (tri->m_p3==p) tri->m_p3 = duplicated_p;
                }

                // reset iterator to beginning
                titr = pv->m_triangles.begin();
            }

        }
    }

    void duplicateProblemVertices()
    {
        checkTrianglesForProblemVertices();

        for(ProblemVertexList::iterator itr = m_problemVertexList.begin();
            itr != m_problemVertexList.end();
            ++itr)
        {
            ProblemVertex* pv = itr->get();
            if (pv->m_triangles.size()>1)
            {
                duplicateProblemVertex(itr->get());
            }
        }
    }

    CRCore::crPrimitive* createPrimitiveSet(Triangles& triangles)
    {
		if(m_vertices->size()<16384)
		{
			DrawElementsUShort *elements = new CRCore::DrawElementsUShort(GL_TRIANGLES);
			elements->reserve(triangles.size()*3);
			for(Triangles::iterator itr = triangles.begin();
				itr != triangles.end();
				++itr)
			{
				Triangle* tri = itr->get();
				elements->push_back(tri->m_p1);
				elements->push_back(tri->m_p2);
				elements->push_back(tri->m_p3);
			}
			return elements;
		}
		else
		{
			DrawElementsUInt *elements = new CRCore::DrawElementsUInt(GL_TRIANGLES);
			elements->reserve(triangles.size()*3);
			for(Triangles::iterator itr = triangles.begin();
				itr != triangles.end();
				++itr)
			{
				Triangle* tri = itr->get();
				elements->push_back(tri->m_p1);
				elements->push_back(tri->m_p2);
				elements->push_back(tri->m_p3);
			}
			return elements;
		}
    }

    void updateGeometry()
    {
        duplicateProblemVertices();


        typedef std::map<unsigned int, Triangles> TriangleMap;
        TriangleMap triangleMap;
        for(Triangles::iterator itr = m_triangles.begin();
            itr != m_triangles.end();
            ++itr)
        {
            Triangle* tri = itr->get();
            triangleMap[tri->m_primitiveSetIndex].push_back(tri);
        }

        for(TriangleMap::iterator itr = triangleMap.begin();
            itr != triangleMap.end();
            ++itr)
        {
            CRCore::crPrimitive* originalPrimitiveSet = m_geometry->getPrimitive(itr->first);
            CRCore::crPrimitive* newPrimitiveSet = createPrimitiveSet(itr->second);
            newPrimitiveSet->setName(originalPrimitiveSet->getName());
            m_geometry->setPrimitive(itr->first, newPrimitiveSet);
        }
    }


    CRCore::crGeometry*      m_geometry;
    CRCore::Vec3Array*     m_vertices;
    CRCore::Vec3Array*     m_normals;
    ArrayList           m_arrays;
    float               m_creaseAngle;
    float               m_maxDeviationDotProduct;
    ProblemVertexVector m_problemVertexVector;
    ProblemVertexList   m_problemVertexList;
    Triangles           m_triangles;
    unsigned int        m_currentPrimitiveSetIndex;
};


static void smooth_new(CRCore::crGeometry& geom, double creaseAngle)
{
    //OSG_INFO<<"smooth_new("<<&geom<<", "<<CRCore::RadiansToDegrees(creaseAngle)<<")"<<std::endl;

    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geom.getVertexArray());
    if (!vertices) return;

    CRCore::Vec3Array* normals = dynamic_cast<CRCore::Vec3Array*>(geom.getNormalArray());
    if (!normals || (normals && normals->size() != vertices->size()))
    {
        normals = new CRCore::Vec3Array(vertices->size());
        geom.setNormalArray(normals);
        geom.setNormalBinding(CRCore::crGeometry::BIND_PER_VERTEX);
    }

    CRCore::crTriangleIndexFunctor<SmoothTriangleIndexFunctor> stif;
    if (stif.set(vertices, normals))
    {
        // accumulate all the normals
        geom.accept(stif);

        // normalize the normals
        stif.normalize();
    }

    CRCore::crTriangleIndexFunctor<FindSharpEdgesFunctor> fsef;
    if (fsef.set(&geom, creaseAngle))
    {
        // look for normals that deviate too far

        fsef.setVertexArray(vertices->getNumElements(), static_cast<const crVector3*>(vertices->getDataPointer()));
        for(unsigned int i = 0; i < geom.getNumPrimitives(); ++i)
        {
            fsef.m_currentPrimitiveSetIndex = i;
            geom.getPrimitive(i)->accept(fsef);
        }

        // fsef.listProblemVertices();
        fsef.updateGeometry();

        CRCore::crTriangleIndexFunctor<SmoothTriangleIndexFunctor> stif2;
        if (stif2.set(vertices, normals))
        {
            // accumulate all the normals
            geom.accept(stif);

            // normalize the normals
            stif.normalize();
        }

    }
}

}


crSmoothingVisitor::crSmoothingVisitor():
    m_creaseAngle(CRCore::PI)
{
    setTraversalMode(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN);
}

crSmoothingVisitor::~crSmoothingVisitor()
{
}

void crSmoothingVisitor::smooth(CRCore::crGeometry& geom, double creaseAngle)
{
    if (creaseAngle==CRCore::PI)
    {
        Smoother::smooth_old(geom);
    }
    else
    {
        Smoother::smooth_new(geom, creaseAngle);
    }
}


void crSmoothingVisitor::apply(CRCore::crObject& geode)
{
    for(unsigned int i = 0; i < geode.getNumDrawables(); i++ )
    {
        CRCore::crGeometry* geom = dynamic_cast<CRCore::crGeometry*>(geode.getDrawable(i));
        if (geom) smooth(*geom, m_creaseAngle);
    }
}
