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
#include <crgl/gl.h>
#include <crgl/glu.h>

#include <CRCore/crNotify.h>
#include <CRUtil/crTesselator.h>
#include <CRCore/crStreamBuf.h>
using namespace CRCore;
using namespace CRUtil;


crTesselator::crTesselator() :
    m_wtype(TESS_WINDING_ODD),
    m_ttype(TESS_TYPE_GEOMETRY),
    m_boundaryOnly(false), m_numberVerts(0) 
{
    m_tobj = 0;
    m_errorCode = 0;
    m_index=0;
}

crTesselator::~crTesselator()
{
    reset();
}

void crTesselator::beginTesselation()
{
    reset();

    if (!m_tobj) m_tobj = gluNewTess();

	gluTessCallback(m_tobj, GLU_TESS_EDGE_FLAG,   (GLvoid (CALLBACK*)()) edgeFlagCallback);
    gluTessCallback(m_tobj, GLU_TESS_VERTEX_DATA, (GLvoid (CALLBACK*)()) vertexCallback);
    gluTessCallback(m_tobj, GLU_TESS_BEGIN_DATA,  (GLvoid (CALLBACK*)()) beginCallback);
    gluTessCallback(m_tobj, GLU_TESS_END_DATA,    (GLvoid (CALLBACK*)()) endCallback);
    gluTessCallback(m_tobj, GLU_TESS_COMBINE_DATA,(GLvoid (CALLBACK*)()) combineCallback);
    gluTessCallback(m_tobj, GLU_TESS_ERROR_DATA,  (GLvoid (CALLBACK*)()) errorCallback);

    gluTessBeginPolygon(m_tobj,this);
}    
    
void crTesselator::beginContour()
{
    if (m_tobj)
    {
        gluTessBeginContour(m_tobj);
    }
}
      
void crTesselator::addVertex(CRCore::crVector3* vertex)
{
    if (m_tobj)
    {
        Vec3d* data = new Vec3d;
        m_coordData.push_back(data);
        (*data)._v[0]=(*vertex)[0];
        (*data)._v[1]=(*vertex)[1];
        (*data)._v[2]=(*vertex)[2];
        gluTessVertex(m_tobj,data->_v,vertex);
    }
}

void crTesselator::endContour()
{
    if (m_tobj)
    {
        gluTessEndContour(m_tobj);
    }
}

void crTesselator::endTesselation()
{
    if (m_tobj)
    {
        gluTessEndPolygon(m_tobj);
        gluDeleteTess(m_tobj);
        m_tobj = 0;
        
        if (m_errorCode!=0)
        {
			char gbuf[256];
			sprintf(gbuf,"Tessellation Error: OpenGL error %s\n\0",gluErrorString(m_errorCode));
			gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
        }
    }
}

void crTesselator::reset()
{
    if (m_tobj)
    {
        gluDeleteTess(m_tobj);
        m_tobj = 0;
    }
    for (Vec3dList::iterator i = m_coordData.begin(); i != m_coordData.end(); ++i) {
      delete (*i);
    }
    m_coordData.clear();
    m_newVertexList.clear();
    m_primList.clear();
    m_errorCode = 0;
}

class InsertNewVertices : public CRCore::ArrayVisitor
{
    public:
    
        float _f1,_f2,_f3,_f4;
        unsigned int _i1,_i2,_i3,_i4;

        InsertNewVertices(float f1,unsigned int i1,
                          float f2,unsigned int i2,
                          float f3,unsigned int i3,
                          float f4,unsigned int i4):
                            _f1(f1),_f2(f2),_f3(f3),_f4(f4),
                            _i1(i1),_i2(i2),_i3(i3),_i4(i4){}

        template <class ARRAY,class TYPE>
        void apply_imp(ARRAY& array,TYPE initialValue)
        {
            TYPE val = initialValue;
            if (_f1) val += static_cast<TYPE>(array[_i1] * _f1);
            if (_f2) val += static_cast<TYPE>(array[_i2] * _f2);
            if (_f3) val += static_cast<TYPE>(array[_i3] * _f3);
            if (_f4) val += static_cast<TYPE>(array[_i4] * _f4);
            
            array.push_back(val);
        }

        virtual void apply(CRCore::ByteArray& ba) { apply_imp(ba,GLbyte(0)); }
        virtual void apply(CRCore::ShortArray& ba) { apply_imp(ba,GLshort(0)); }
        virtual void apply(CRCore::IntArray& ba) { apply_imp(ba,GLint(0)); }
        virtual void apply(CRCore::UByteArray& ba) { apply_imp(ba,GLubyte(0)); }
        virtual void apply(CRCore::UShortArray& ba) { apply_imp(ba,GLushort(0)); }
        virtual void apply(CRCore::UIntArray& ba) { apply_imp(ba,GLuint(0)); }
        virtual void apply(CRCore::Vec4ubArray& ba) { apply_imp(ba,crVector4ub()); }
        virtual void apply(CRCore::FloatArray& ba) { apply_imp(ba,float(0)); }
        virtual void apply(CRCore::Vec2Array& ba) { apply_imp(ba,crVector2()); }
        virtual void apply(CRCore::Vec3Array& ba) { apply_imp(ba,crVector3()); }
        virtual void apply(CRCore::Vec4Array& ba) { apply_imp(ba,crVector4()); }

};

void crTesselator::retesselatePolygons(CRCore::crGeometry &geom)
{
    // turn the contour list into primitives, a little like tesselator does but more generally
    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geom.getVertexArray());
    
    if (!vertices || vertices->empty() || geom.getPrimitiveList().empty()) return;

    // we currently don't handle geometry which use indices...
    if (geom.getVertexIndices() ||
        geom.getNormalIndices() ||
        geom.getColorIndices() ||
        geom.getSecondaryColorIndices() ||
        geom.getFogCoordIndices()) return;
        
    // not even text coord indices don't handle geometry which use indices...
    for(unsigned int unit=0;unit<geom.getNumTexCoordArrays();++unit)
    {
        if (geom.getTexCoordIndices(unit)) return;
    }

    if (m_ttype==TESS_TYPE_POLYGONS || m_ttype==TESS_TYPE_DRAWABLE) m_numberVerts=0; // 09.04.04 GWM reset tesselator
    // the reset is needed by the flt loader which reuses a tesselator for triangulating polygons.
    // as such it might be reset by other loaders/developers in future.
    m_index=0; // reset the counter for indexed vertices
    if (!m_numberVerts) 
	{
        m_numberVerts=geom.getVertexArray()->getNumElements();
        // save the contours for complex (winding rule) tesselations
        m_Contours=geom.getPrimitiveList();
    }

    // now cut out vertex attributes added on any previous tesselation
    reduceArray(geom.getVertexArray(), m_numberVerts);
    reduceArray(geom.getColorArray(), m_numberVerts);
    reduceArray(geom.getNormalArray(), m_numberVerts);
    reduceArray(geom.getFogCoordArray(), m_numberVerts);
    for(unsigned int unit1=0;unit1<geom.getNumTexCoordArrays();++unit1)
    {
        reduceArray(geom.getTexCoordArray(unit1), m_numberVerts);
    }

    // remove the existing primitives.
    unsigned int nprimsetoriginal= geom.getNumPrimitives();
    if (nprimsetoriginal) geom.removePrimitive(0, nprimsetoriginal);

    // the main difference from CRUtil::tesselator for crGeometry sets of multiple contours is that the begin/end tesselation
    // occurs around the whole set of contours.
    if (m_ttype==TESS_TYPE_GEOMETRY) 
	{
        beginTesselation();
        gluTessProperty(m_tobj, GLU_TESS_WINDING_RULE, m_wtype);
        gluTessProperty(m_tobj, GLU_TESS_BOUNDARY_ONLY , m_boundaryOnly);
    }
    // process all the contours into the tesselator
    int noContours = m_Contours.size();
    for(int primNo=0;primNo<noContours;++primNo)
    {
        CRCore::ref_ptr<CRCore::crPrimitive> primitive = m_Contours[primNo].get();
        if (m_ttype==TESS_TYPE_POLYGONS || m_ttype==TESS_TYPE_DRAWABLE) 
		{
			if (primitive->getMode()>CRCore::crPrimitive::PT_TRIANGLE_STRIP ||m_ttype==TESS_TYPE_DRAWABLE)  
			{
                beginTesselation();
                addContour(primitive.get(), vertices);
                endTesselation();

                collectTesselation(geom);
            } 
			else 
			{ // copy the contour primitive as it is not being tesselated
                geom.addPrimitive(primitive.get());
            }
        } 
		else 
		{
            if (primitive->getMode()==CRCore::crPrimitive::PT_POLYGON ||
                primitive->getMode()==CRCore::crPrimitive::PT_QUADS ||
                primitive->getMode()==CRCore::crPrimitive::PT_TRIANGLES ||
                primitive->getMode()==CRCore::crPrimitive::PT_LINE_LOOP ||
                primitive->getMode()==CRCore::crPrimitive::PT_QUAD_STRIP ||
                primitive->getMode()==CRCore::crPrimitive::PT_TRIANGLE_FAN ||
                primitive->getMode()==CRCore::crPrimitive::PT_TRIANGLE_STRIP)
            {
                addContour(primitive.get(), vertices);
            } 
			else 
			{ // copy the contour primitive as it is not being tesselated
                // in this case points, lines or line_strip
                geom.addPrimitive(primitive.get());
            }
        }
    }
    if (m_ttype==TESS_TYPE_GEOMETRY) 
	{
        endTesselation();
    
        collectTesselation(geom);    
    }
}

void crTesselator::addContour(CRCore::crPrimitive* primitive, CRCore::Vec3Array* vertices)
{
    // adds a single primitive as a contour.
    beginContour();
    unsigned int nperprim=0; // number of vertices per primitive
    if (primitive->getMode()==CRCore::crPrimitive::PT_QUADS) nperprim=4;
    if (primitive->getMode()==CRCore::crPrimitive::PT_TRIANGLES) nperprim=3;

    unsigned int idx=0;
    
    switch(primitive->getType())
    {
    case(CRCore::crPrimitive::DrawArraysPrimitiveType):
        {
            unsigned int i;
            CRCore::DrawArrays* drawArray = static_cast<CRCore::DrawArrays*>(primitive);
            unsigned int first = drawArray->getFirst(); 
            unsigned int last = first+drawArray->getCount();

            switch (primitive->getMode()) 
			{
            case CRCore::crPrimitive::PT_QUADS:
            case CRCore::crPrimitive::PT_TRIANGLES:
            case CRCore::crPrimitive::PT_POLYGON:
            case CRCore::crPrimitive::PT_LINE_LOOP:
            case CRCore::crPrimitive::PT_TRIANGLE_FAN:
                {
                    for(i=first;i<last;++i, idx++)
                    {
                        addVertex(&((*vertices)[i]));
                        if (nperprim>0 && i<last-1 && idx%nperprim==nperprim-1) 
						{
                            endContour();
			                gluTessEndPolygon(m_tobj);
					        gluTessBeginPolygon(m_tobj,this);
					        beginContour();
                        }
                    }
                }
                break;
            case CRCore::crPrimitive::PT_QUAD_STRIP:
                { // always has an even number of vertices
                    for( i=first;i<last;i+=2)
                    { // 0,2,4...
                        addVertex(&((*vertices)[i]));
                    }
                    for(i=last-1;i>=first;i-=2)
                    { // ...5,3,1
                        addVertex(&((*vertices)[i]));
                    }
                }
                break;
            case CRCore::crPrimitive::PT_TRIANGLE_STRIP:
                {
                    for( i=first;i<last;i+=2)
                    {// 0,2,4,...
                        addVertex(&((*vertices)[i]));
                    }
                    for(i=((last-first)%2)?(last-2):(last-1) ;i>first&& i<last;i-=2)
                    {
                        addVertex(&((*vertices)[i]));
                    }
                }
                break;
            default: // lines, points, line_strip
                {
                    for(i=first;i<last;++i, idx++)
                    {
                        addVertex(&((*vertices)[i]));
                        if (nperprim>0 && i<last-1 && idx%nperprim==nperprim-1) 
						{
                            endContour();
					        gluTessEndPolygon(m_tobj);
					        gluTessBeginPolygon(m_tobj,this);
					        beginContour();
                        }
                    }
                }
                break;
            }
            break;
        }
    case(CRCore::crPrimitive::DrawElementsUBytePrimitiveType):
        {
            CRCore::DrawElementsUByte* drawElements = static_cast<CRCore::DrawElementsUByte*>(primitive);

            for(CRCore::DrawElementsUByte::PT_IndexArray::iterator indexItr=drawElements->getIndexArray().begin();
            indexItr!=drawElements->getIndexArray().end();
            ++indexItr, idx++)
            {
                addVertex(&((*vertices)[*indexItr]));
                if (nperprim>0 && indexItr!=drawElements->getIndexArray().end() && idx%nperprim==nperprim-1) 
				{
                    endContour();
					gluTessEndPolygon(m_tobj);
					gluTessBeginPolygon(m_tobj,this);
					beginContour();
                }
            }
            break;
        }
    case(CRCore::crPrimitive::DrawElementsUShortPrimitiveType):
        {
            CRCore::DrawElementsUShort* drawElements = static_cast<CRCore::DrawElementsUShort*>(primitive);
            for(CRCore::DrawElementsUShort::PT_IndexArray::iterator indexItr=drawElements->getIndexArray().begin();
            indexItr!=drawElements->getIndexArray().end();
            ++indexItr, idx++)
            {
                addVertex(&((*vertices)[*indexItr]));
                if (nperprim>0 && indexItr!=drawElements->getIndexArray().end() && idx%nperprim==nperprim-1) 
				{
                    endContour();
					gluTessEndPolygon(m_tobj);
					gluTessBeginPolygon(m_tobj,this);
                    beginContour();
                }
            }
            break;
        }
    case(CRCore::crPrimitive::DrawElementsUIntPrimitiveType):
        {
            CRCore::DrawElementsUInt* drawElements = static_cast<CRCore::DrawElementsUInt*>(primitive);
			for(CRCore::DrawElementsUInt::PT_IndexArray::iterator indexItr=drawElements->getIndexArray().begin();
            indexItr!=drawElements->getIndexArray().end();
            ++indexItr, idx++)
            {
                addVertex(&((*vertices)[*indexItr]));
                if (nperprim>0 && indexItr!=drawElements->getIndexArray().end() && idx%nperprim==nperprim-1) 
				{
                    endContour();
					gluTessEndPolygon(m_tobj);
					gluTessBeginPolygon(m_tobj,this);
					beginContour();
                }
            }
            break;
        }
    default:
        break;
    }
    
    endContour();
}

void crTesselator::handleNewVertices(CRCore::crGeometry& geom,VertexPtrToIndexMap &vertexPtrToIndexMap)
{
    if (!m_newVertexList.empty())
    {
        CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geom.getVertexArray());
        CRCore::Vec3Array* normals = NULL;
        if (geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        {
            normals = geom.getNormalArray();
        }
        
        typedef std::vector<CRCore::Array*> ArrayList;
        ArrayList arrays;
        
        if (geom.getColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        {
            arrays.push_back(geom.getColorArray());
        }
        
        if (geom.getSecondaryColorBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        {
            arrays.push_back(geom.getSecondaryColorArray());
        }
        
        if (geom.getFogCoordBinding()==CRCore::crGeometry::BIND_PER_VERTEX)
        {
            arrays.push_back(geom.getFogCoordArray());
        }
        
        CRCore::crGeometry::ArrayList& tcal = geom.getTexCoordArrayList();
        for(CRCore::crGeometry::ArrayList::iterator tcalItr=tcal.begin();
        tcalItr!=tcal.end();
        ++tcalItr)
        {
            if (tcalItr->array.valid()) 
            {
                arrays.push_back(tcalItr->array.get());
            }
        }
        
        // now add any new vertices that are required.
        for(NewVertexList::iterator itr=m_newVertexList.begin();
        itr!=m_newVertexList.end();
        ++itr)
        {
            NewVertex& newVertex = (*itr);
            CRCore::crVector3* vertex = newVertex._vpos;
            
            // assign vertex.
            vertexPtrToIndexMap[vertex]=vertices->size();
            vertices->push_back(*vertex);
            
            // assign normals
            if (normals)
            {
                CRCore::crVector3 norm(0.0f,0.0f,0.0f);
                if (newVertex._v1) norm += (*normals)[vertexPtrToIndexMap[newVertex._v1]] * newVertex._f1;
                if (newVertex._v2) norm += (*normals)[vertexPtrToIndexMap[newVertex._v2]] * newVertex._f2;
                if (newVertex._v3) norm += (*normals)[vertexPtrToIndexMap[newVertex._v3]] * newVertex._f3;
                if (newVertex._v4) norm += (*normals)[vertexPtrToIndexMap[newVertex._v4]] * newVertex._f4;
                norm.normalize();
                normals->push_back(norm);
            }
            
            if (!arrays.empty())
            {
                InsertNewVertices inv(newVertex._f1,vertexPtrToIndexMap[newVertex._v1],
                    newVertex._f2,vertexPtrToIndexMap[newVertex._v2],
                    newVertex._f3,vertexPtrToIndexMap[newVertex._v3],
                    newVertex._f4,vertexPtrToIndexMap[newVertex._v4]);
                
                // assign the rest of the attributes.
                for(ArrayList::iterator aItr=arrays.begin();
                aItr!=arrays.end();
                ++aItr)
                {
                    (*aItr)->accept(inv);
                }
            }
        }  
    }   
}

void crTesselator::begin(GLenum mode)
{
	if(mode!=4)
	    CRCore::notify(CRCore::WARN)<<"crTesselator::begin error"<<std::endl;
    if(m_primList.size()<=0||m_primList.back()->m_mode!=mode)
       m_primList.push_back(new Prim(mode));
}

void crTesselator::vertex(CRCore::crVector3* vertex)
{
    if (!m_primList.empty())
    {
        Prim* prim = m_primList.back().get();
        prim->m_vertices.push_back(vertex);

    }
}

void crTesselator::combine(CRCore::crVector3* vertex,void* vertex_data[4],GLfloat weight[4])
{
    m_newVertexList.push_back(NewVertex(vertex,
                                    weight[0],(crVector3*)vertex_data[0],
                                     weight[1],(crVector3*)vertex_data[1],
                                     weight[2],(crVector3*)vertex_data[2],
                                     weight[3],(crVector3*)vertex_data[3]));
}

void crTesselator::end()
{
    // no need to do anything right now...
}

void crTesselator::error(GLenum errorCode)
{
    m_errorCode = errorCode;
}

void CALLBACK crTesselator::edgeFlagCallback(bool flag)
{
}

void CALLBACK crTesselator::beginCallback(GLenum which, void* userData)
{
    ((crTesselator*)userData)->begin(which);
}

void CALLBACK crTesselator::endCallback(void* userData)
{
    ((crTesselator*)userData)->end();
}

void CALLBACK crTesselator::vertexCallback(GLvoid *data, void* userData)
{
    ((crTesselator*)userData)->vertex((crVector3*)data);
}

void CALLBACK crTesselator::combineCallback(GLdouble coords[3], void* vertex_data[4],
                              GLfloat weight[4], void** outData,
                              void* userData)
{
    crVector3* newData = new CRCore::crVector3(coords[0],coords[1],coords[2]);
    *outData = newData;
    ((crTesselator*)userData)->combine(newData,vertex_data,weight);
}

void CALLBACK crTesselator::errorCallback(GLenum errorCode, void* userData)
{
    ((crTesselator*)userData)->error(errorCode);
}

void crTesselator::reduceArray(CRCore::Array * cold, const unsigned int nnu)
{ // shrinks size of array to N
    if (cold && cold->getNumElements()>nnu) 
	{
        CRCore::Vec2Array* v2arr = NULL;
        CRCore::Vec3Array* v3arr = NULL;
        CRCore::Vec4Array* v4arr = NULL;
        switch (cold->getType()) 
		{
        case CRCore::Array::Vec2ArrayType: 
			{
                v2arr = dynamic_cast<CRCore::Vec2Array*>(cold);
                CRCore::Vec2Array::iterator itr=v2arr->begin()+nnu;
                (*v2arr).erase(itr, v2arr->end());
             }
            break;
        case CRCore::Array::Vec3ArrayType: 
			{
                v3arr = dynamic_cast<CRCore::Vec3Array*>(cold);
                CRCore::Vec3Array::iterator itr=v3arr->begin()+nnu;
                (*v3arr).erase(itr, v3arr->end());
            }
            break;
        case CRCore::Array::Vec4ArrayType: 
			{
                v4arr = dynamic_cast<CRCore::Vec4Array*>(cold);
                CRCore::Vec4Array::iterator itr=v4arr->begin()+nnu;
                (*v4arr).erase(itr, v4arr->end());
            }
            break;
        default: // should also handle:ArrayType' ByteArrayType' ShortArrayType' IntArrayType' 
        // `UShortArrayType'  `UIntArrayType'  `UByte4ArrayType'  `FloatArrayType' 
            break;
        }
    }
}

unsigned int _computeNumberOfPrimitives(const CRCore::crGeometry& geom)
{

    unsigned int totalNumberOfPrimitives = 0;
    
    for(crGeometry::PrimitiveList::const_iterator itr=geom.getPrimitiveList().begin();
        itr!=geom.getPrimitiveList().end();
        ++itr)
    {
        const crPrimitive* primitiveset = itr->get();
        GLenum mode=primitiveset->getMode();

        unsigned int primLength;
        switch(mode)
        {
            case(GL_POINTS):    primLength=1; break;
            case(GL_LINES):     primLength=2; break;
            case(GL_TRIANGLES): primLength=3; break;
            case(GL_QUADS):     primLength=4; break;
            default:            primLength=0; break; // compute later when =0.
        }

        // draw primtives by the more flexible "slow" path,
        // sending OpenGL glBegin/glVertex.../glEnd().
        switch(primitiveset->getType())
        {
            case(crPrimitive::DrawArrayLengthsPrimitiveType):
            {

                const DrawArrayLengths* drawArrayLengths = static_cast<const DrawArrayLengths*>(primitiveset);
				for(DrawArrayLengths::PT_IndexArray::const_iterator primItr=drawArrayLengths->getIndexArray().begin();
                    primItr!=drawArrayLengths->getIndexArray().end();
                    ++primItr)
                {
                    if (primLength==0) totalNumberOfPrimitives += 1;
                    else totalNumberOfPrimitives += *primItr/primLength; // Dec 2003 - increment not set
                }
                break;
            }
            default:
            {
                if (primLength==0) totalNumberOfPrimitives += 1;
                else totalNumberOfPrimitives += primitiveset->getNumIndices()/primLength;
            }
        }
    }

    return totalNumberOfPrimitives;
}
//
void crTesselator::collectTesselation(CRCore::crGeometry &geom)
{
    CRCore::Vec3Array* vertices = dynamic_cast<CRCore::Vec3Array*>(geom.getVertexArray());
    VertexPtrToIndexMap vertexPtrToIndexMap;
    
    // populate the VertexPtrToIndexMap.
    for(unsigned int vi=0;vi<vertices->size();++vi)
    {
        vertexPtrToIndexMap[&((*vertices)[vi])] = vi;
    }
    
    handleNewVertices(geom, vertexPtrToIndexMap);
    
    // we don't properly handle per primitive and per primitive_set bindings yet
    // will need to address this soon. Robert Oct 2002.
    {
        CRCore::Vec3Array* normals = NULL; // GWM Sep 2002 - add normals for extra facets
        int iprim=0;
        if (geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
            geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET)
        {
            normals = geom.getNormalArray(); // GWM Sep 2002
        }
        // GWM Dec 2003 - needed to add colours for extra facets
        CRCore::Vec4Array* cols4 = NULL; // GWM Dec 2003 colours are vec4
        CRCore::Vec3Array* cols3 = NULL; // GWM Dec 2003 colours are vec3
        if (geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE ||
              geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET)
        {
              Array* colours = geom.getColorArray(); // GWM Dec 2003 - need to duplicate face colours
              switch (colours->getType()) 
			  {
              case CRCore::Array::Vec4ArrayType:
                  cols4=dynamic_cast<CRCore::Vec4Array *> (colours);
                  break;
              case CRCore::Array::Vec3ArrayType:
                  cols3=dynamic_cast<CRCore::Vec3Array *> (colours);
                  break;
              default:
                  break;
              }
        }
        // GWM Dec 2003 - these holders need to go outside the loop to 
        // retain the flat shaded colour &/or normal for each tesselated polygon
        CRCore::crVector3 norm(0.0f,0.0f,0.0f);
        CRCore::crVector4 primCol4(0.0f,0.0f,0.0f,1.0f);
        CRCore::crVector3 primCol3(0.0f,0.0f,0.0f);

        for(PrimList::iterator primItr=m_primList.begin();
            primItr!=m_primList.end();
            ++primItr, ++m_index)
        {
              Prim* prim = primItr->get();
              
              CRCore::DrawElementsUShort* elements = new CRCore::DrawElementsUShort(prim->m_mode);
              for(Prim::VecList::iterator vitr=prim->m_vertices.begin();
              vitr!=prim->m_vertices.end();
              ++vitr)
              {
                  elements->getIndexArray().push_back(vertexPtrToIndexMap[*vitr]);
              }
              
              // add to the drawn primitive list.
              geom.addPrimitive(elements);
              if (primItr==m_primList.begin()) 
              {   // first primitive so collect primitive normal & colour.
                  if (normals) 
				  {
                      norm=(*normals)[iprim]; // GWM Sep 2002 the flat shaded normal
                  }
                  if (cols4) 
				  {
                      primCol4=(*cols4)[iprim]; // GWM Dec 2003 the flat shaded rgba colour
                      if (m_index>=cols4->size()) 
					  {
                          cols4->push_back(primCol4); // GWM Dec 2003 add flat shaded colour for new facet
					  }
                  }
                  if (cols3) 
				  {
                      primCol3=(*cols3)[iprim]; // GWM Dec 2003 flat shaded rgb colour
                      if (m_index>=cols4->size()) 
					  {
                          cols3->push_back(primCol3); // GWM Dec 2003 add flat shaded colour for new facet
					  }
                  }
              }
              else
              { // later primitives use same colour
                  if (normals) normals->push_back(norm); // GWM Sep 2002 add flat shaded normal for new facet
                  if (cols4 && m_index>=cols4->size()) 
				  {
                      cols4->push_back(primCol4); // GWM Dec 2003 add flat shaded colour for new facet
                  }
                  if (cols3 && m_index>=cols3->size()) 
				  {
                      if (cols3) cols3->push_back(primCol3); // GWM Dec 2003 add flat shaded colour for new facet
                  }
                  if (prim->m_mode==GL_TRIANGLES) 
				  {
                      int ntris=elements->getNumIndices()/3;
                      if (geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET ||
                          geom.getNormalBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE) 
					  { // need one per triangle? Not one per set.
                          for (int ii=1; ii<ntris; ii++) 
						  {
                              if (normals) normals->push_back(norm); // GWM Sep 2002 add flat shaded normal for new facet
                          }
                      }
                      if (geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE_SET ||
                          geom.getColorBinding()==CRCore::crGeometry::BIND_PER_PRIMITIVE) 
					  { // need one per triangle? Not one per set.
                          for (int ii=1; ii<ntris; ii++) 
						  {
                              if (cols3 && m_index>=cols3->size()) 
							  {
                                  if (cols3) cols3->push_back(primCol3);
                              }
                              if (cols4 && m_index>=cols4->size()) 
							  {
                                  if (cols4) cols4->push_back(primCol4);
                              }
                              m_index++;
                          }
                      }
                  }
                  //        CRCore::notify(CRCore::WARN)<<"Add: "<< iprim << std::endl; 
              }
              iprim++; // GWM Sep 2002 count which normal we should use
        }
    }
}
