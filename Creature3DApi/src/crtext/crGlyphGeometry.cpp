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
#include <CRText/crGlyphGeometry.h>
//#include <CRCore/io_utils.h>
#include <CRCore/crTriangleIndexFunctor.h>
#include <CRCore/crLineWidth.h>
#include <CRUtil/crTesselator.h>
#include <CRCore/crCullFace.h>

#include <limits.h>

namespace CRText
{

/////////////////////////////////////////////////////////////////////////////////////////
//
// Boundary
//
class Boundary
{
public:

    typedef std::pair<unsigned int, unsigned int> Segment;
    typedef std::vector<Segment>  Segments;
    CRCore::ref_ptr<const CRCore::Vec3Array> m_vertices;
    CRCore::ref_ptr<const CRCore::DrawElementsUShort> m_elements;
    Segments m_segments;

    Boundary(const CRCore::Vec3Array* vertices, const CRCore::crPrimitive* primitiveSet)
    {
        const CRCore::DrawArrays* drawArrays = dynamic_cast<const CRCore::DrawArrays*>(primitiveSet);
        if (drawArrays)
        {
            set(vertices, drawArrays->getFirst(), drawArrays->getCount());
        }
        else
        {
            const CRCore::DrawElementsUShort* elements = dynamic_cast<const CRCore::DrawElementsUShort*>(primitiveSet);
            if (elements) set(vertices, elements);
        }
    }

    void set(const CRCore::Vec3Array* vertices, unsigned int start, unsigned int count)
    {
        CRCore::DrawElementsUShort* elements = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_POLYGON);
        for(unsigned int i=start; i<start+count; ++i)
        {
            elements->push_back(i);
        }

        set(vertices, elements);
    }

    void set(const CRCore::Vec3Array* vertices, const CRCore::DrawElementsUShort* elements)
    {
        m_vertices = vertices;
        m_elements = elements;

        m_segments.clear();

        if (elements->empty()) return;

        m_segments.reserve(elements->getNumIndices()-1);
        for(unsigned int i=0; i<elements->getNumIndices()-1; ++i)
        {
            m_segments.push_back(Segment((*elements)[i],(*elements)[i+1]));
        }
    }

    CRCore::crVector3 computeRayIntersectionPoint(const CRCore::crVector3& a, const CRCore::crVector3& an, const CRCore::crVector3& c, const CRCore::crVector3& cn)
    {
        float denominator = ( cn.x() * an.y() - cn.y() * an.x());
        if (denominator==0.0f)
        {
            //CRCore::notify(CRCore::NOTICE)<<"computeRayIntersectionPoint()<<denominator==0.0"<<std::endl;
            // line segments must be parallel.
            return (a+c)*0.5f;
        }

        float t = ((a.x()-c.x())*an.y() - (a.y()-c.y())*an.x()) / denominator;
        return c + cn*t;
    }

    CRCore::crVector3 computeIntersectionPoint(const CRCore::crVector3& a, const CRCore::crVector3& b, const CRCore::crVector3& c, const CRCore::crVector3& d)
    {
        return computeRayIntersectionPoint(a, b-a, c, d-c);
    }

    CRCore::crVector3 computeBisectorNormal(const CRCore::crVector3& a, const CRCore::crVector3& b, const CRCore::crVector3& c, const CRCore::crVector3& d)
    {
        CRCore::crVector2 ab(a.x()-b.x(), a.y()-b.y());
        CRCore::crVector2 dc(d.x()-c.x(), d.y()-c.y());
        /*float length_ab =*/ ab.normalize();
        /*float length_dc =*/ dc.normalize();

        float e = dc.y() - ab.y();
        float f = ab.x() - dc.x();
        float denominator = sqrtf(e*e + f*f);
        float nx = e / denominator;
        float ny = f / denominator;
        if (( ab.x()*ny - ab.y()*nx) > 0.0f)
        {
            // CRCore::notify(CRCore::NOTICE)<<"   computeBisectorNormal(a=["<<a<<"], b=["<<b<<"], c=["<<c<<"], d=["<<d<<"]), nx="<<nx<<", ny="<<ny<<", denominator="<<denominator<<" no need to swap"<<std::endl;
            return CRCore::crVector3(nx,ny,0.0f);
        }
        else
        {
           // CRCore::notify(CRCore::INFO)<<"   computeBisectorNormal(a=["<<a<<"], b=["<<b<<"], c=["<<c<<"], d=["<<d<<"]), nx="<<nx<<", ny="<<ny<<", denominator="<<denominator<<" need to swap!!!"<<std::endl;
            return CRCore::crVector3(-nx,-ny,0.0f);
        }
    }

    float computeBisectorIntersectorThickness(const CRCore::crVector3& a, const CRCore::crVector3& b, const CRCore::crVector3& c, const CRCore::crVector3& d, const CRCore::crVector3& e, const CRCore::crVector3& f)
    {
        CRCore::crVector3 intersection_abcd = computeIntersectionPoint(a,b,c,d);
        CRCore::crVector3 bisector_abcd = computeBisectorNormal(a,b,c,d);
        CRCore::crVector3 intersection_cdef = computeIntersectionPoint(c,d,e,f);
        CRCore::crVector3 bisector_cdef = computeBisectorNormal(c,d,e,f);
        if (bisector_abcd==bisector_cdef)
        {
            //CRCore::notify(CRCore::NOTICE)<<"computeBisectorIntersector(["<<a<<"], ["<<b<<"], ["<<c<<"], ["<<d<<"], ["<<e<<"], ["<<f<<"[)"<<std::endl;
            //CRCore::notify(CRCore::NOTICE)<<"   bisectors parallel, thickness = "<<FLT_MAX<<std::endl;
            return FLT_MAX;
        }

        CRCore::crVector3 bisector_intersection = computeRayIntersectionPoint(intersection_abcd,bisector_abcd, intersection_cdef, bisector_cdef);
        CRCore::crVector3 normal(d.y()-c.y(), c.x()-d.x(), 0.0);
        float cd_length = normal.length();
		normal.normalize();
        if (cd_length==0)
        {
            //CRCore::notify(CRCore::NOTICE)<<"computeBisectorIntersector(["<<a<<"], ["<<b<<"], ["<<c<<"], ["<<d<<"], ["<<e<<"], ["<<f<<"[)"<<std::endl;
            //CRCore::notify(CRCore::NOTICE)<<"   segment length==0, thickness = "<<FLT_MAX<<std::endl;
            return FLT_MAX;
        }

        float thickness = (bisector_intersection - c) * normal;
    #if 0
        CRCore::notify(CRCore::NOTICE)<<"computeBisectorIntersector(["<<a<<"], ["<<b<<"], ["<<c<<"], ["<<d<<"], ["<<e<<"], ["<<f<<"[)"<<std::endl;
        CRCore::notify(CRCore::NOTICE)<<"   bisector_abcd = "<<bisector_abcd<<", bisector_cdef="<<bisector_cdef<<std::endl;
        CRCore::notify(CRCore::NOTICE)<<"   bisector_intersection = "<<bisector_intersection<<", thickness = "<<thickness<<std::endl;
    #endif
        return thickness;
    }


    float computeThickness(unsigned int i)
    {
        Segment& seg_before = m_segments[ (i+m_segments.size()-1) % m_segments.size() ];
        Segment& seg_target = m_segments[ (i) % m_segments.size() ];
        Segment& seg_after =  m_segments[ (i+1) % m_segments.size() ];
        return computeBisectorIntersectorThickness(
            (*m_vertices)[seg_before.first], (*m_vertices)[seg_before.second],
            (*m_vertices)[seg_target.first], (*m_vertices)[seg_target.second],
            (*m_vertices)[seg_after.first], (*m_vertices)[seg_after.second]);
    }

    void computeAllThickness()
    {
        for(unsigned int i=0; i<m_segments.size(); ++i)
        {
            computeThickness(i);
        }
    }


    bool findMinThickness(unsigned int& minThickness_i, float& minThickness)
    {
        minThickness_i = m_segments.size();
        for(unsigned int i=0; i<m_segments.size(); ++i)
        {
            float thickness = computeThickness(i);
            if (thickness>0.0 && thickness <  minThickness)
            {
                minThickness = thickness;
                minThickness_i = i;
            }
        }

        return minThickness_i != m_segments.size();
    }

    void removeAllSegmentsBelowThickness(float targetThickness)
    {
        // CRCore::notify(CRCore::NOTICE)<<"removeAllSegmentsBelowThickness("<<targetThickness<<")"<<std::endl;
        for(;;)
        {
            unsigned int minThickness_i = m_segments.size();
            float minThickness = targetThickness;
            if (!findMinThickness(minThickness_i,minThickness)) break;

            // CRCore::notify(CRCore::NOTICE)<<"  removing segment m_segments["<<minThickness_i<<"] ("<<m_segments[minThickness_i].first<<", "<<m_segments[minThickness_i].second<<" with thickness="<<minThickness<<" "<<std::endl;
            m_segments.erase(m_segments.begin()+minThickness_i);
        }
    }

    bool findMaxThickness(unsigned int& maxThickness_i, float& maxThickness)
    {
        maxThickness_i = m_segments.size();
        for(unsigned int i=0; i<m_segments.size(); ++i)
        {
            float thickness = computeThickness(i);
            if (thickness<0.0 && thickness >  maxThickness)
            {
                maxThickness = thickness;
                maxThickness_i = i;
            }
        }

        return maxThickness_i != m_segments.size();
    }


    void removeAllSegmentsAboveThickness(float targetThickness)
    {
        // CRCore::notify(CRCore::NOTICE)<<"removeAllSegmentsBelowThickness("<<targetThickness<<")"<<std::endl;
        for(;;)
        {
            unsigned int maxThickness_i = m_segments.size();
            float maxThickness = targetThickness;
            if (!findMaxThickness(maxThickness_i,maxThickness)) break;

            // CRCore::notify(CRCore::NOTICE)<<"  removing segment m_segments["<<minThickness_i<<"] ("<<m_segments[minThickness_i].first<<", "<<m_segments[minThickness_i].second<<" with thickness="<<minThickness<<" "<<std::endl;
            m_segments.erase(m_segments.begin()+maxThickness_i);
        }
    }

    CRCore::crVector3 computeBisectorPoint(unsigned int i, float targetThickness)
    {
        Segment& seg_before = m_segments[ (i+m_segments.size()-1) % m_segments.size() ];
        Segment& seg_target = m_segments[ (i) % m_segments.size() ];
        const CRCore::crVector3& a = (*m_vertices)[seg_before.first];
        const CRCore::crVector3& b = (*m_vertices)[seg_before.second];
        const CRCore::crVector3& c = (*m_vertices)[seg_target.first];
        const CRCore::crVector3& d = (*m_vertices)[seg_target.second];
        CRCore::crVector3 intersection_abcd = computeIntersectionPoint(a,b,c,d);
        CRCore::crVector3 bisector_abcd = computeBisectorNormal(a,b,c,d);
        CRCore::crVector3 ab_sidevector(b.y()-a.y(), a.x()-b.x(), 0.0);
        ab_sidevector.normalize();
        float scale_factor = 1.0/ (bisector_abcd*ab_sidevector);
        CRCore::crVector3 new_vertex = intersection_abcd + bisector_abcd*(scale_factor*targetThickness);

        // CRCore::notify(CRCore::NOTICE)<<"bisector_abcd = "<<bisector_abcd<<", ab_sidevector="<<ab_sidevector<<", b-a="<<b-a<<", scale_factor="<<scale_factor<<std::endl;
        return new_vertex;
    }

    void addBoundaryToGeometry(CRCore::crGeometry* geometry, float targetThickness, bool requireFace)
    {
        if (m_segments.empty()) return;

        if (geometry->getVertexArray()==0) geometry->setVertexArray(new CRCore::Vec3Array);
        CRCore::Vec3Array* new_vertices = dynamic_cast<CRCore::Vec3Array*>(geometry->getVertexArray());

        // allocate the primitive set to store the face geometry
        CRCore::ref_ptr<CRCore::DrawElementsUShort> face = new CRCore::DrawElementsUShort(GL_POLYGON);
        face->setName("face");

        // reserve enough space in the vertex array to accomodate the vertices associated with the segments
        new_vertices->reserve(new_vertices->size() + m_segments.size()+1 + m_elements->getNumIndices());

        // create vertices
        unsigned int previous_second = m_segments[0].second;
        CRCore::crVector3 newPoint = computeBisectorPoint(0, targetThickness);
        unsigned int first = new_vertices->size();
        new_vertices->push_back(newPoint);

        unsigned int start = (*m_elements)[0];
        unsigned int count = m_elements->getNumIndices();

        if (m_segments[0].first != start)
        {
            //CRCore::notify(CRCore::NOTICE)<<"We have pruned from the start"<<std::endl;
            for(unsigned int j=start; j<=m_segments[0].first;++j)
            {
                face->push_back(first);
            }
        }
        else
        {
            face->push_back(first);
        }


        for(unsigned int i=1; i<m_segments.size(); ++i)
        {
            newPoint = computeBisectorPoint(i, targetThickness);
            unsigned int vi = new_vertices->size();
            new_vertices->push_back(newPoint);

            if (previous_second != m_segments[i].first)
            {
                //CRCore::notify(CRCore::NOTICE)<<"Gap in boundary"<<previous_second<<" to "<<m_segments[i].first<<std::endl;
                for(unsigned int j=previous_second; j<=m_segments[i].first;++j)
                {
                    face->push_back(vi);
                }
            }
            else
            {
                face->push_back(vi);
            }

            previous_second = m_segments[i].second;
        }

        // fill the end of the polygon with repititions of the first index in the polygon to ensure
        // that the orignal and new boundary polygons have the same number and pairing of indices.
        // This ensures that the bevel can be created coherently.
        while(face->getNumIndices() < count)
        {
            face->push_back(first);
        }


        if (requireFace)
        {
            // add face primitive set for polygon
            geometry->addPrimitive(face.get());
        }

        
        CRCore::DrawElementsUShort* bevel = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        bevel->setName("bevel");
        bevel->reserve(count*2);
        for(unsigned int i=0; i<count; ++i)
        {
            unsigned int vi = new_vertices->size();
            new_vertices->push_back((*m_vertices)[(*m_elements)[i]]);
            bevel->push_back(vi);
            bevel->push_back((*face)[i]);
        }
        geometry->addPrimitive(bevel);
    }

    void newAddBoundaryToGeometry(CRCore::crGeometry* geometry, float targetThickness, const std::string& faceName, const std::string& bevelName)
    {
        if (m_segments.empty()) return;

        unsigned int start = (*m_elements)[0];
        unsigned int count = m_elements->getNumIndices();

        if (geometry->getVertexArray()==0) geometry->setVertexArray(new CRCore::Vec3Array(*m_vertices));
        CRCore::Vec3Array* new_vertices = dynamic_cast<CRCore::Vec3Array*>(geometry->getVertexArray());

        // allocate the primitive set to store the face geometry
        CRCore::ref_ptr<CRCore::DrawElementsUShort> face = new CRCore::DrawElementsUShort(GL_POLYGON);
        face->setName(faceName);

        // reserve enough space in the vertex array to accomodate the vertices associated with the segments
        new_vertices->reserve(new_vertices->size() + m_segments.size()+1 + count);

        // create vertices
        unsigned int previous_second = m_segments[0].second;
        CRCore::crVector3 newPoint = computeBisectorPoint(0, targetThickness);
        unsigned int first = new_vertices->size();
        new_vertices->push_back(newPoint);

        if (m_segments[0].first != start)
        {
            //CRCore::notify(CRCore::NOTICE)<<"We have pruned from the start"<<std::endl;
            for(unsigned int j=start; j<=m_segments[0].first;++j)
            {
                face->push_back(first);
            }
        }
        else
        {
            face->push_back(first);
        }


        for(unsigned int i=1; i<m_segments.size(); ++i)
        {
            newPoint = computeBisectorPoint(i, targetThickness);
            unsigned int vi = new_vertices->size();
            new_vertices->push_back(newPoint);

            if (previous_second != m_segments[i].first)
            {
                //CRCore::notify(CRCore::NOTICE)<<"Gap in boundary"<<previous_second<<" to "<<m_segments[i].first<<std::endl;
                for(unsigned int j=previous_second; j<=m_segments[i].first;++j)
                {
                    face->push_back(vi);
                }
            }
            else
            {
                face->push_back(vi);
            }

            previous_second = m_segments[i].second;
        }

        // fill the end of the polygon with repititions of the first index in the polygon to ensure
        // that the orignal and new boundary polygons have the same number and pairing of indices.
        // This ensures that the bevel can be created coherently.
        while(face->getNumIndices() < count)
        {
            face->push_back(first);
        }

        if (!faceName.empty())
        {
            // add face primitive set for polygon
            geometry->addPrimitive(face.get());
        }

        CRCore::DrawElementsUShort* bevel = new CRCore::DrawElementsUShort(GL_QUAD_STRIP);
        bevel->setName(bevelName);
        bevel->reserve(count*2);
        for(unsigned int i=0; i<count; ++i)
        {
            bevel->push_back((*m_elements)[i]);
            bevel->push_back((*face)[i]);
        }
        geometry->addPrimitive(bevel);
    }
};


/////////////////////////////////////////////////////////////////////////////////////////
//
// computeGlyphGeometry
//
struct CollectTriangleIndicesFunctor
{
    CollectTriangleIndicesFunctor() {}

    typedef std::vector<unsigned int> Indices;
    Indices _indices;

    void operator() (unsigned int p1, unsigned int p2, unsigned int p3)
    {
        if (p1==p2 || p2==p3 || p1==p3)
        {
            return;
        }

        _indices.push_back(p1);
        _indices.push_back(p3);
        _indices.push_back(p2);

    }
};


CRTEXT_EXPORT CRCore::crGeometry* computeGlyphGeometry(const CRText::crGlyph3D* glyph, float bevelThickness, float shellThickness)
{
    const CRCore::Vec3Array* orig_vertices = glyph->getRawVertexArray();
    const CRCore::crGeometry::PrimitiveList& orig_primitives = glyph->getRawFacePrimitiveSetList();

    CRCore::ref_ptr<CRCore::crGeometry> new_geometry = new CRCore::crGeometry;

    for(CRCore::crGeometry::PrimitiveList::const_iterator itr = orig_primitives.begin();
        itr != orig_primitives.end();
        ++itr)
    {
        if ((*itr)->getMode()==GL_POLYGON)
        {
            Boundary boundaryInner(orig_vertices, itr->get());
            boundaryInner.removeAllSegmentsBelowThickness(bevelThickness);
            boundaryInner.newAddBoundaryToGeometry(new_geometry.get(), bevelThickness, "face", "bevel");

            Boundary boundaryOuter(orig_vertices, itr->get());
            boundaryOuter.removeAllSegmentsAboveThickness(-shellThickness);
            boundaryOuter.newAddBoundaryToGeometry(new_geometry.get(), -shellThickness, "", "shell");
        }
        
    }

    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(new_geometry->getVertexArray());

    // need to tessellate the inner boundary
    {
        CRCore::ref_ptr<CRCore::crGeometry> face_geometry = new CRCore::crGeometry;
        face_geometry->setVertexArray(vertices);

        CRCore::crCopyOp copyop(CRCore::crCopyOp::DEEP_COPY_ALL);

        CRCore::crGeometry::PrimitiveList primitiveSets;

        for(CRCore::crGeometry::PrimitiveList::iterator itr = new_geometry->getPrimitiveList().begin();
            itr != new_geometry->getPrimitiveList().end();
            ++itr)
        {
            CRCore::crPrimitive* prim = itr->get();
            if (prim->getName()=="face")  face_geometry->addPrimitive(copyop(itr->get()));
            else primitiveSets.push_back(prim);
        }

        CRUtil::crTesselator ts;
        ts.setWindingType(CRUtil::crTesselator::TESS_WINDING_POSITIVE);
        ts.setTesselationType(CRUtil::crTesselator::TESS_TYPE_GEOMETRY);
        ts.retesselatePolygons(*face_geometry);

        CRCore::crTriangleIndexFunctor<CollectTriangleIndicesFunctor> ctif;
        face_geometry->accept(ctif);
        CollectTriangleIndicesFunctor::Indices& indices = ctif._indices;

        // remove the previous primitive sets
        new_geometry->getPrimitiveList().clear();

        // create a front face using triangle indices
        CRCore::DrawElementsUShort* front_face = new CRCore::DrawElementsUShort(GL_TRIANGLES);
        front_face->setName("face");
        new_geometry->addPrimitive(front_face);
        for(unsigned int i=0; i<indices.size();++i)
        {
            front_face->push_back(indices[i]);
        }

        for(CRCore::crGeometry::PrimitiveList::iterator itr = primitiveSets.begin();
            itr != primitiveSets.end();
            ++itr)
        {
            CRCore::crPrimitive* prim = itr->get();
            if (prim->getName()!="face")  new_geometry->addPrimitive(prim);
        }
    }

    return new_geometry.release();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// computeTextGeometry
//
CRTEXT_EXPORT CRCore::crGeometry* computeTextGeometry(const CRText::crGlyph3D* glyph, float width)
{
    const CRCore::Vec3Array* orig_vertices = glyph->getRawVertexArray();
    const CRCore::crGeometry::PrimitiveList& orig_primitives = glyph->getRawFacePrimitiveSetList();

    CRCore::ref_ptr<CRCore::crGeometry> text_geometry = new CRCore::crGeometry;
    CRCore::ref_ptr<CRCore::Vec3Array> vertices = new CRCore::Vec3Array((*orig_vertices));

    text_geometry->setVertexArray(vertices.get());
    text_geometry->setPrimitiveList(orig_primitives);

    CRUtil::crTesselator ts;
    ts.setWindingType(CRUtil::crTesselator::TESS_WINDING_POSITIVE);
    ts.setTesselationType(CRUtil::crTesselator::TESS_TYPE_GEOMETRY);
    ts.retesselatePolygons(*text_geometry);

    CRCore::crTriangleIndexFunctor<CollectTriangleIndicesFunctor> ctif;
    text_geometry->accept(ctif);
    CollectTriangleIndicesFunctor::Indices& indices = ctif._indices;

    // remove the previous primitive sets
    text_geometry->getPrimitiveList().clear();

    if (indices.empty()) return 0;


    // create a front face using triangle indices
    CRCore::DrawElementsUShort* frontFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    frontFace->setName("front");
    text_geometry->addPrimitive(frontFace);
    for(unsigned int i=0; i<indices.size();++i)
    {
        frontFace->push_back(indices[i]);
    }

    typedef std::vector<unsigned int> Indices;
    const unsigned int NULL_VALUE = UINT_MAX;
    Indices back_indices;
    back_indices.resize(vertices->size(), NULL_VALUE);
    CRCore::crVector3 forward(0,0,-width);

    // build up the vertices primitives for the back face, and record the indices
    // for later use, and to ensure sharing of vertices in the face primitive set
    // the order of the triangle indices are flipped to make sure that the triangles are back face
    CRCore::DrawElementsUShort* backFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    backFace->setName("back");
    text_geometry->addPrimitive(backFace);
    for(unsigned int i=0; i<indices.size()-2;)
    {
        unsigned int p1 = indices[i++];
        unsigned int p2 = indices[i++];
        unsigned int p3 = indices[i++];
        if (back_indices[p1]==NULL_VALUE)
        {
            back_indices[p1] = vertices->size();
            vertices->push_back((*vertices)[p1]+forward);
        }

        if (back_indices[p2]==NULL_VALUE)
        {
            back_indices[p2] = vertices->size();
            vertices->push_back((*vertices)[p2]+forward);
        }

        if (back_indices[p3]==NULL_VALUE)
        {
            back_indices[p3] = vertices->size();
            vertices->push_back((*vertices)[p3]+forward);
        }

        backFace->push_back(back_indices[p1]);
        backFace->push_back(back_indices[p3]);
        backFace->push_back(back_indices[p2]);
    }

    unsigned int orig_size = orig_vertices->size();
    Indices frontedge_indices, backedge_indices;
    frontedge_indices.resize(orig_size, NULL_VALUE);
    backedge_indices.resize(orig_size, NULL_VALUE);


    for(CRCore::crGeometry::PrimitiveList::const_iterator itr = orig_primitives.begin();
        itr != orig_primitives.end();
        ++itr)
    {
        CRCore::DrawElementsUShort* edging = new CRCore::DrawElementsUShort(CRCore::crPrimitive::PT_QUAD_STRIP);
        edging->setName("wall");
        text_geometry->addPrimitive(edging);

        const CRCore::DrawElementsUShort* elements = dynamic_cast<const CRCore::DrawElementsUShort*>(itr->get());
        if (elements)
        {
            for(unsigned int i=0; i<elements->getNumIndices(); ++i)
            {
                unsigned int ei = (*elements)[i];
                if (frontedge_indices[ei]==NULL_VALUE)
                {
                    frontedge_indices[ei] = vertices->size();
                    vertices->push_back((*orig_vertices)[ei]);
                }
                if (backedge_indices[ei]==NULL_VALUE)
                {
                    backedge_indices[ei] = vertices->size();
                    vertices->push_back((*orig_vertices)[ei]+forward);
                }

                edging->push_back(backedge_indices[ei]);
                edging->push_back(frontedge_indices[ei]);
            }
        }
    }

    return text_geometry.release();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// computeTextGeometry
//
CRTEXT_EXPORT CRCore::crGeometry* computeTextGeometry(CRCore::crGeometry* glyphGeometry, const CRText::crBevel& profile, float width)
{
    CRCore::Vec3Array* orig_vertices = dynamic_cast<CRCore::Vec3Array*>(glyphGeometry->getVertexArray());
    if (!orig_vertices)
    {
       // CRCore::notify(CRCore::INFO)<<"computeTextGeometry(..): No vertices on glyphGeometry."<<std::endl;
        return 0;
    }

    CRCore::ref_ptr<CRCore::crGeometry> text_geometry = new CRCore::crGeometry;
    CRCore::ref_ptr<CRCore::Vec3Array> vertices = new CRCore::Vec3Array;
    text_geometry->setVertexArray(vertices.get());

    typedef std::vector<unsigned int> Indices;
    const unsigned int NULL_VALUE = UINT_MAX;
    Indices front_indices, back_indices;
    front_indices.resize(orig_vertices->size(), NULL_VALUE);
    back_indices.resize(orig_vertices->size(), NULL_VALUE);

    CRCore::DrawElementsUShort* face = 0;
    CRCore::crGeometry::PrimitiveList bevelPrimitiveSets;
    CRCore::crVector3 forward(0,0,-width);

    // collect bevels and face primitive sets
    for(CRCore::crGeometry::PrimitiveList::iterator itr = glyphGeometry->getPrimitiveList().begin();
        itr != glyphGeometry->getPrimitiveList().end();
        ++itr)
    {
        CRCore::crPrimitive* prim = itr->get();
        if (prim->getName()=="face") face = dynamic_cast<CRCore::DrawElementsUShort*>(prim);
        else if (prim->getName()=="bevel") bevelPrimitiveSets.push_back(prim);
    }

    // if we don't have a face we can't create any 3d text
    if (!face) return 0;

    // face doesn't have enough vertices on it to represent a polygon.
    if (face->getNumIndices()<3)
    {
        CRCore::notify(CRCore::NOTICE)<<"Face does not have enough elements to be able to represent a polygon, face->size() = "<<face->getNumIndices()<<std::endl;
        return 0;
    }

    // build up the vertices primitives for the front face, and record the indices
    // for later use, and to ensure sharing of vertices in the face primitive set
    CRCore::DrawElementsUShort* frontFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    frontFace->setName("front");
    text_geometry->addPrimitive(frontFace);
    for(unsigned int i=0; i<face->getNumIndices();)
    {
        unsigned int pi = (*face)[i++];
        if (front_indices[pi]==NULL_VALUE)
        {
            front_indices[pi] = vertices->size();
            vertices->push_back((*orig_vertices)[pi]);
        }
        frontFace->push_back(front_indices[pi]);
    }


    // build up the vertices primitives for the back face, and record the indices
    // for later use, and to ensure sharing of vertices in the face primitive set
    // the order of the triangle indices are flipped to make sure that the triangles are back face
    CRCore::DrawElementsUShort* backFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    backFace->setName("back");
    text_geometry->addPrimitive(backFace);
    for(unsigned int i=0; i<face->getNumIndices()-2;)
    {
        unsigned int p1 = (*face)[i++];
        unsigned int p2 = (*face)[i++];
        unsigned int p3 = (*face)[i++];
        if (back_indices[p1]==NULL_VALUE)
        {
            back_indices[p1] = vertices->size();
            vertices->push_back((*orig_vertices)[p1]+forward);
        }

        if (back_indices[p2]==NULL_VALUE)
        {
            back_indices[p2] = vertices->size();
            vertices->push_back((*orig_vertices)[p2]+forward);
        }

        if (back_indices[p3]==NULL_VALUE)
        {
            back_indices[p3] = vertices->size();
            vertices->push_back((*orig_vertices)[p3]+forward);
        }

        backFace->push_back(back_indices[p1]);
        backFace->push_back(back_indices[p3]);
        backFace->push_back(back_indices[p2]);
    }

    bool shareVerticesWithFaces = true;

    // now build up the bevel
    for(CRCore::crGeometry::PrimitiveList::iterator itr = bevelPrimitiveSets.begin();
        itr != bevelPrimitiveSets.end();
        ++itr)
    {
        CRCore::DrawElementsUShort* bevel = dynamic_cast<CRCore::DrawElementsUShort*>(itr->get());
        if (!bevel) continue;

        unsigned int no_vertices_on_boundary = bevel->getNumIndices()/2;

        const CRText::crBevel::Vertices& profileVertices = profile.getVertices();
        unsigned int no_vertices_on_bevel = profileVertices.size();

        Indices bevelIndices;
        bevelIndices.resize(no_vertices_on_boundary*no_vertices_on_bevel, NULL_VALUE);

        // populate vertices
        for(unsigned int i=0; i<no_vertices_on_boundary; ++i)
        {
            unsigned int topi = (*bevel)[i*2];
            unsigned int basei = (*bevel)[i*2+1];

            CRCore::crVector3& top_vertex = (*orig_vertices)[ topi ];
            CRCore::crVector3& base_vertex = (*orig_vertices)[ basei ];
            CRCore::crVector3 up = top_vertex-base_vertex;

            if (shareVerticesWithFaces)
            {
                if (front_indices[basei]==NULL_VALUE)
                {
                    front_indices[basei] = vertices->size();
                    vertices->push_back(base_vertex);
                }

                bevelIndices[i*no_vertices_on_bevel + 0] = front_indices[basei];

                for(unsigned int j=1; j<no_vertices_on_bevel-1; ++j)
                {
                    const CRCore::crVector2& pv = profileVertices[j];
                    CRCore::crVector3 pos( base_vertex + (forward * pv.x()) + (up * pv.y()) );
                    bevelIndices[i*no_vertices_on_bevel + j] = vertices->size();
                    vertices->push_back(pos);
                }

                if (back_indices[basei]==NULL_VALUE)
                {
                    back_indices[basei] = vertices->size();
                    vertices->push_back(base_vertex + forward);
                }

                bevelIndices[i*no_vertices_on_bevel + no_vertices_on_bevel-1] = back_indices[basei];
            }
            else
            {
                for(unsigned int j=0; j<no_vertices_on_bevel; ++j)
                {
                    const CRCore::crVector2& pv = profileVertices[j];
                    CRCore::crVector3 pos( base_vertex + (forward * pv.x()) + (up * pv.y()) );
                    bevelIndices[i*no_vertices_on_bevel + j] = vertices->size();
                    vertices->push_back(pos);
                }
            }
        }

        CRCore::DrawElementsUShort* elements = new CRCore::DrawElementsUShort(GL_TRIANGLES);
        elements->setName("wall");
        unsigned int base, next;
        for(unsigned int i = 0; i< no_vertices_on_boundary-1; ++i)
        {
            for(unsigned int j=0; j<no_vertices_on_bevel-1; ++j)
            {
                base = i*no_vertices_on_bevel + j;
                next = base + no_vertices_on_bevel;

                elements->push_back(bevelIndices[base]);
                elements->push_back(bevelIndices[next]);
                elements->push_back(bevelIndices[base+1]);

                elements->push_back(bevelIndices[base+1]);
                elements->push_back(bevelIndices[next]);
                elements->push_back(bevelIndices[next+1]);
            }
        }

        text_geometry->addPrimitive(elements);
    }

    return text_geometry.release();
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// computeShellGeometry
//
CRTEXT_EXPORT CRCore::crGeometry* computeShellGeometry(CRCore::crGeometry* glyphGeometry, const CRText::crBevel& profile, float width)
{
    CRCore::Vec3Array* orig_vertices = dynamic_cast<CRCore::Vec3Array*>(glyphGeometry->getVertexArray());
    if (!orig_vertices)
    {
        CRCore::notify(CRCore::NOTICE)<<"computeTextGeometry(..): No vertices on glyphGeometry."<<std::endl;
        return 0;
    }

    CRCore::ref_ptr<CRCore::crGeometry> text_geometry = new CRCore::crGeometry;
    CRCore::ref_ptr<CRCore::Vec3Array> vertices = new CRCore::Vec3Array;
    text_geometry->setVertexArray(vertices.get());

    typedef std::vector<unsigned int> Indices;
    const unsigned int NULL_VALUE = UINT_MAX;
    Indices front_indices, back_indices;
    front_indices.resize(orig_vertices->size(), NULL_VALUE);
    back_indices.resize(orig_vertices->size(), NULL_VALUE);

    CRCore::DrawElementsUShort* face = 0;
    CRCore::crGeometry::PrimitiveList bevelPrimitiveSets;
    CRCore::crGeometry::PrimitiveList shellPrimitiveSets;
    CRCore::crVector3 frontOffset(0,0,width);
    CRCore::crVector3 backOffset(0,0,-2.0*width);
    CRCore::crVector3 forward(backOffset-frontOffset);

    // collect bevels and face primitive sets
    for(CRCore::crGeometry::PrimitiveList::iterator itr = glyphGeometry->getPrimitiveList().begin();
        itr != glyphGeometry->getPrimitiveList().end();
        ++itr)
    {
        CRCore::crPrimitive* prim = itr->get();
        if (prim->getName()=="face") face = dynamic_cast<CRCore::DrawElementsUShort*>(prim);
        else if (prim->getName()=="bevel") bevelPrimitiveSets.push_back(prim);
        else if (prim->getName()=="shell") shellPrimitiveSets.push_back(prim);
    }

    // if we don't have a face we can't create any 3d text
    if (!face) return 0;

    // build up the vertices primitives for the front face, and record the indices
    // for later use, and to ensure sharing of vertices in the face primitive set
    // the order of the triangle indices are flipped to make sure that the triangles are back face
    CRCore::DrawElementsUShort* frontFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    text_geometry->addPrimitive(frontFace);
    for(unsigned int i=0; i<face->getNumIndices()-2;)
    {
        unsigned int p1 = (*face)[i++];
        unsigned int p2 = (*face)[i++];
        unsigned int p3 = (*face)[i++];
        if (front_indices[p1]==NULL_VALUE)
        {
            front_indices[p1] = vertices->size();
            vertices->push_back((*orig_vertices)[p1]+frontOffset);
        }

        if (front_indices[p2]==NULL_VALUE)
        {
            front_indices[p2] = vertices->size();
            vertices->push_back((*orig_vertices)[p2]+frontOffset);
        }

        if (front_indices[p3]==NULL_VALUE)
        {
            front_indices[p3] = vertices->size();
            vertices->push_back((*orig_vertices)[p3]+frontOffset);
        }

        frontFace->push_back(front_indices[p1]);
        frontFace->push_back(front_indices[p3]);
        frontFace->push_back(front_indices[p2]);
    }


    // build up the vertices primitives for the back face, and record the indices
    // for later use, and to ensure sharing of vertices in the face primitive set
    CRCore::DrawElementsUShort* backFace = new CRCore::DrawElementsUShort(GL_TRIANGLES);
    text_geometry->addPrimitive(backFace);
    for(unsigned int i=0; i<face->getNumIndices();)
    {
        unsigned int pi = (*face)[i++];
        if (back_indices[pi]==NULL_VALUE)
        {
            back_indices[pi] = vertices->size();
            vertices->push_back((*orig_vertices)[pi]+backOffset);
        }
        backFace->push_back(back_indices[pi]);
    }

    for(CRCore::crGeometry::PrimitiveList::iterator itr = bevelPrimitiveSets.begin();
        itr != bevelPrimitiveSets.end();
        ++itr)
    {
        CRCore::DrawElementsUShort* strip = dynamic_cast<CRCore::DrawElementsUShort*>(itr->get());
        if (!strip) continue;

        CRCore::crCopyOp copyop(CRCore::crCopyOp::DEEP_COPY_ALL);

        CRCore::DrawElementsUShort* front_strip = dynamic_cast<CRCore::DrawElementsUShort*>(copyop(strip));
        text_geometry->addPrimitive(front_strip);
        for(unsigned int i=0; i<front_strip->getNumIndices(); ++i)
        {
            unsigned short& pi  = (*front_strip)[i];
            if (front_indices[pi]==NULL_VALUE)
            {
                front_indices[pi] = vertices->size();
                vertices->push_back((*orig_vertices)[pi]+frontOffset);
            }
            pi = front_indices[pi]; 
        }

        for(unsigned int i=0; i<front_strip->getNumIndices()-1;)
        {
            unsigned short& p1  = (*front_strip)[i++];
            unsigned short& p2  = (*front_strip)[i++];
            std::swap(p1,p2);
        }

        CRCore::DrawElementsUShort* back_strip = dynamic_cast<CRCore::DrawElementsUShort*>(copyop(strip));
        text_geometry->addPrimitive(back_strip);
        for(unsigned int i=0; i<back_strip->getNumIndices(); ++i)
        {
            unsigned short& pi  = (*back_strip)[i];
            if (back_indices[pi]==NULL_VALUE)
            {
                back_indices[pi] = vertices->size();
                vertices->push_back((*orig_vertices)[pi]+backOffset);
            }
            pi = back_indices[pi];
        }
    }


    // now build up the shell
    for(CRCore::crGeometry::PrimitiveList::iterator itr = shellPrimitiveSets.begin();
        itr != shellPrimitiveSets.end();
        ++itr)
    {
        CRCore::DrawElementsUShort* bevel = dynamic_cast<CRCore::DrawElementsUShort*>(itr->get());
        if (!bevel) continue;

        unsigned int no_vertices_on_boundary = bevel->getNumIndices()/2;

        const CRText::crBevel::Vertices& profileVertices = profile.getVertices();
        unsigned int no_vertices_on_bevel = profileVertices.size();

        Indices bevelIndices;
        bevelIndices.resize(no_vertices_on_boundary*no_vertices_on_bevel, NULL_VALUE);

        // populate vertices
        for(unsigned int i=0; i<no_vertices_on_boundary; ++i)
        {
            unsigned int topi = (*bevel)[i*2+1];
            unsigned int basei = (*bevel)[i*2];

            CRCore::crVector3 top_vertex = (*orig_vertices)[ topi ] + frontOffset;
            CRCore::crVector3 base_vertex = (*orig_vertices)[ basei ] + frontOffset;
            CRCore::crVector3 up = top_vertex-base_vertex;

            if (front_indices[basei]==NULL_VALUE)
            {
                front_indices[basei] = vertices->size();
                vertices->push_back(base_vertex);
            }

            bevelIndices[i*no_vertices_on_bevel + 0] = front_indices[basei];

            for(unsigned int j=1; j<no_vertices_on_bevel-1; ++j)
            {
                const CRCore::crVector2& pv = profileVertices[j];
                CRCore::crVector3 pos( base_vertex + (forward * pv.x()) + (up * pv.y()) );
                bevelIndices[i*no_vertices_on_bevel + j] = vertices->size();
                vertices->push_back(pos);
            }

            if (back_indices[basei]==NULL_VALUE)
            {
                back_indices[basei] = vertices->size();
                vertices->push_back(base_vertex + forward);
            }

            bevelIndices[i*no_vertices_on_bevel + no_vertices_on_bevel-1] = back_indices[basei];
        }

        CRCore::DrawElementsUShort* elements = new CRCore::DrawElementsUShort(GL_TRIANGLES);
        unsigned int base, next;
        for(unsigned int i = 0; i< no_vertices_on_boundary-1; ++i)
        {
            for(unsigned int j=0; j<no_vertices_on_bevel-1; ++j)
            {
                base = i*no_vertices_on_bevel + j;
                next = base + no_vertices_on_bevel;

                elements->push_back(bevelIndices[base]);
                elements->push_back(bevelIndices[base+1]);
                elements->push_back(bevelIndices[next]);

                elements->push_back(bevelIndices[base+1]);
                elements->push_back(bevelIndices[next+1]);
                elements->push_back(bevelIndices[next]);
            }
        }

        text_geometry->addPrimitive(elements);
    }

#if 1
    CRCore::Vec4Array* new_colours = new CRCore::Vec4Array;
    new_colours->push_back(CRCore::crVector4(1.0,1.0,1.0,0.2));
    text_geometry->setColorArray(new_colours);
    text_geometry->setColorBinding(CRCore::crGeometry::BIND_OVERALL);


    CRCore::crStateSet* stateset = text_geometry->getOrCreateStateSet();
    stateset->setMode(GL_BLEND, CRCore::crStateAttribute::ON);
    stateset->setMode(GL_LIGHTING, CRCore::crStateAttribute::OFF);
    stateset->setAttributeAndModes(new CRCore::crCullFace, CRCore::crStateAttribute::ON);
    //stateset->setRenderingHint(CRCore::crStateSet::TRANSPARENT_BIN);
    stateset->setRenderBinDetails(11, "SORT_FRONT_TO_BACK");
#endif
    return text_geometry.release();
}

}
