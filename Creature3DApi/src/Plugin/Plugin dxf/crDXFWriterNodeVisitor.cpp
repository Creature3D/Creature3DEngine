// -*-c++-*-

/*
 * Autcad DXF writer for Open Scene Graph
 *
 * Copyright (C) 2009 Martin Beckett mgb@mgbeckett.com
 *
 * Based on OBJ writer plugin by Ulrich Hertlein
 *
 * The Open Scene Graph (OSG) is a cross platform C++/OpenGL library for 
 * real-time rendering of large 3D photo-realistic models. 
 * The OSG homepage is http://www.openscenegraph.org/
 */


#include <CRCore/io_utils>
#include <iomanip>

#include "crDXFWritercrNodeVisitor.h"
 
// ROBERT - is there any need for a value visitor like this or is it just overkill?

/** writes all values of an array out to a stream, applies a matrix beforehand if necessary */

// I think this is a bit over the top for just a simple vertex array - but if anyone knwos different?
/*
class ValueVisitor : public CRCore::ValueVisitor {
    public:
        ValueVisitor(std::ostream& fout, const Layer &layer,const CRCore::crMatrix& m = CRCore::crMatrix::identity()) :    
            CRCore::ValueVisitor(), 
            _fout(fout), 
            _layer(layer),            
            _m(m)          
        { 
            //_applyMatrix = (_m != CRCore::crMatrix::identity());                
        }
                
        virtual void apply(CRCore::crVector3 & inv) 
        { 
            CRCore::crVector3 point(inv) ;
            point = point * _m;                        
            _fout << "0 \nVERTEX\n 8\n"<<_layer._name<<"\n";    
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            }

            _fout <<" 10\n"<<point.x()<<"\n 20\n"<<point.y()<<"\n 30\n"<<point.z()<<"\n";                                
        }
        
    private:

        ValueVisitor& operator = (const ValueVisitor&) { return *this; }

        std::ostream&    _fout;
        CRCore::crMatrix        _m;
        const Layer _layer;        
};
*/

/** writes all primitives of a primitive-set out to a stream, decomposes quads to triangles, line-strips to lines etc */
class DxfPrimitiveIndexWriter : public CRCore::PrimitiveIndexFunctor {
    
    public:
        DxfPrimitiveIndexWriter(std::ostream& fout,CRCore::crGeometry* geo,const Layer &layer,AcadColor &acad,const CRCore::crMatrix& m = CRCore::crMatrix::identity()) : 
            CRCore::PrimitiveIndexFunctor(), 
            _fout(fout),            
            _geo(geo),
            _layer(layer),
            _acad(acad),
            _m(m)
        {
            
        }
        
        virtual void setVertexArray(unsigned int,const CRCore::Vec2*) {}

        virtual void setVertexArray(unsigned int ,const CRCore::crVector3* ) {}

        virtual void setVertexArray(unsigned int,const CRCore::crVector4* ) {}
        
        virtual void setVertexArray(unsigned int,const CRCore::Vec2d*) {}

        virtual void setVertexArray(unsigned int ,const CRCore::crVector3d* ) {}

        virtual void setVertexArray(unsigned int,const CRCore::Vec4d* ) {}
        
        
        void write(unsigned int i,int c) 
        {            
            const CRCore::crVector3 point = ((CRCore::Vec3Array *)_geo->getVertexArray())->at(i) * _m;                        
            _fout <<c+10<<"\n "<<point.x()<<"\n"<<20+c<<"\n "<<point.y()<<"\n"<<30+c<<"\n "<<point.z()<<"\n";                        
        }

        // operator for facets - need to distinguish from triangles
        void writeFace(unsigned int i1, unsigned int i2, unsigned int i3)
         {                     
            _fout << "0 \n3DFACE\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1))<<"\n";                
                // Acad2000 supports 24bit color but most dxf importers don't
                //_fout << "420\n"<<crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1)<<"\n";                
            }
            write(i1,0);
            write(i2,1);
            write(i3,2);
            write(i1,0); // yes you have to write the first point again
        }

        // operator for triangles 
        void writeTriangle(unsigned int i1, unsigned int i2, unsigned int i3)
         {                    
            _fout << "0 \nLINE\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1))<<"\n";                                
            }
            write(i1,0);
            write(i2,1);

            _fout << "0 \nLINE\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i2))<<"\n";                                
            }
            write(i2,0);
            write(i3,1);

            _fout << "0 \nLINE\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i3))<<"\n";                                
            }
            write(i3,0);
            write(i1,1);

        }

        // operator for lines
        void writeLine(unsigned int i1, unsigned int i2) 
        {            
            _fout << "0 \nLINE\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1))<<"\n";                                
            }
            write(i1,0);
            write(i2,1);
        }
        
        // operator for points
        void writePoint(unsigned int i1) 
        {
            _fout << "0 \nPOINT\n 8\n"<<_layer._name<<"\n";
            if ( _layer._color ) {
                _fout << "62\n"<<_layer._color<<"\n";                
            } else {
                _fout << "62\n"<<_acad.findColor(crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1))<<"\n";                
                //_fout << "420\n"<<crDXFWritercrNodeVisitor::getcrNodeRGB(_geo,i1)<<"\n";                
                
            }
            write(i1,0);            
        }

        virtual void begin(GLenum mode)
        {
            _modeCache = mode;
            _indexCache.clear();
        }

        virtual void vertex(unsigned int vert)
        {
            _indexCache.push_back(vert);
        }

        virtual void end()
        {
            if (!_indexCache.empty())
            {
                drawElements(_modeCache,_indexCache.size(),&_indexCache.front());
            }
        }

        virtual void drawArrays(GLenum mode,GLint first,GLsizei count);                    
        
        virtual void drawElements(GLenum mode,GLsizei count,const GLubyte* indices)
        {
            drawElementsImplementation<GLubyte>(mode, count, indices);
        }
        virtual void drawElements(GLenum mode,GLsizei count,const GLushort* indices)
        {
            drawElementsImplementation<GLushort>(mode, count, indices);
        }    

        virtual void drawElements(GLenum mode,GLsizei count,const GLuint* indices)
        {
            drawElementsImplementation<GLuint>(mode, count, indices);
        }    

    protected:
        
        template<typename T>void drawElementsImplementation(GLenum mode, GLsizei count, const T* indices) 
        {
            if (indices==0 || count==0) return;

            typedef const T* IndexPointer;                    

            switch(mode)
            {
                case(GL_TRIANGLES):
                {
                    IndexPointer ilast = &indices[count];
                    for(IndexPointer  iptr=indices;iptr<ilast;iptr+=3)
                        writeTriangle(*iptr,*(iptr+1),*(iptr+2));
    
                    break;
                }
                case(GL_TRIANGLE_STRIP):
                {
                    IndexPointer iptr = indices;
                    for(GLsizei i=2;i<count;++i,++iptr)
                    {
                        if ((i%2)) writeTriangle(*(iptr),*(iptr+2),*(iptr+1));
                        else       writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                    }
                    break;
                }
                case(GL_QUADS):
                {
                    IndexPointer iptr = indices;
                    for(GLsizei i=3;i<count;i+=4,iptr+=4)
                    {
                        writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                        writeTriangle(*(iptr),*(iptr+2),*(iptr+3));
                    }
                    break;
                }
                case(GL_QUAD_STRIP):
                {
                    IndexPointer iptr = indices;
                    for(GLsizei i=3;i<count;i+=2,iptr+=2)
                    {
                        writeTriangle(*(iptr),*(iptr+1),*(iptr+2));
                        writeTriangle(*(iptr+1),*(iptr+3),*(iptr+2));
                    }
                    break;
                }
                case(GL_POLYGON): // treat polygons as GL_TRIANGLE_FAN
                case(GL_TRIANGLE_FAN):
                {
                    IndexPointer iptr = indices;
                    unsigned int first = *iptr;
                    ++iptr;
                    for(GLsizei i=2;i<count;++i,++iptr)
                    {
                        writeTriangle(first,*(iptr),*(iptr+1));
                    }
                    break;
                }
                case(GL_POINTS):
                {
                    IndexPointer ilast = &indices[count];
                    for(IndexPointer  iptr=indices;iptr<ilast;++iptr)
                    
                    {
                        writePoint(*iptr);
                    }
                    break;
                }

                case(GL_LINES):
                {
                    IndexPointer ilast = &indices[count];
                    for(IndexPointer  iptr=indices;iptr<ilast;iptr+=2)
                    {
                        writeLine(*iptr, *(iptr+1));
                    }
                    break;
                }
                case(GL_LINE_STRIP):
                {
                    
                    IndexPointer ilast = &indices[count];
                    for(IndexPointer  iptr=indices+1;iptr<ilast;iptr+=2)

                    {
                        writeLine(*(iptr-1), *iptr);
                    }
                    break;
                }
                case(GL_LINE_LOOP):
                {
                    IndexPointer ilast = &indices[count];
                    for(IndexPointer  iptr=indices+1;iptr<ilast;iptr+=2)
                    {
                        writeLine(*(iptr-1), *iptr);
                    }
                    writeLine(*ilast, *indices);
                    break;
                }

                default:
                    // uhm should never come to this point :)
                    break;
            }
        }    
    
    private:

        DxfPrimitiveIndexWriter& operator = (const DxfPrimitiveIndexWriter&) { return *this; }

        std::ostream&         _fout;
        GLenum               _modeCache;
        std::vector<GLuint>  _indexCache;        
        CRCore::crGeometry*         _geo;        
        
        Layer        _layer;
        AcadColor    _acad; // needed to lookup new colors
        CRCore::crMatrix        _m;
};


void DxfPrimitiveIndexWriter::drawArrays(GLenum mode,GLint first,GLsizei count)
{
    switch(mode)
    {
        case(GL_TRIANGLES):
        {
            unsigned int pos=first;
            for(GLsizei i=2;i<count;i+=3,pos+=3)
            {
                writeTriangle(pos,pos+1,pos+2);
            }
            break;
        }
        case(GL_TRIANGLE_STRIP):
         {
            unsigned int pos=first;
            for(GLsizei i=2;i<count;++i,++pos)
            {
                if ((i%2)) writeTriangle(pos,pos+2,pos+1);
                else       writeTriangle(pos,pos+1,pos+2);
            }
            break;
        }
        case(GL_QUADS):
        {
            unsigned int pos=first;
            for(GLsizei i=3;i<count;i+=4,pos+=4)
            {
                writeTriangle(pos,pos+1,pos+2);
                writeTriangle(pos,pos+2,pos+3);
            }
            break;
        }
        case(GL_QUAD_STRIP):
        {
            unsigned int pos=first;
            for(GLsizei i=3;i<count;i+=2,pos+=2)
            {
                writeTriangle(pos,pos+1,pos+2);
                writeTriangle(pos+1,pos+3,pos+2);
            }
            break;
        }
        case(GL_POLYGON): // treat polygons as GL_TRIANGLE_FAN
        case(GL_TRIANGLE_FAN):
        {
            unsigned int pos=first+1;
            for(GLsizei i=2;i<count;++i,++pos)
            {
                writeTriangle(first,pos,pos+1);
            }
            break;
        }
        case(GL_POINTS):
        {
            
            for(GLsizei i=0;i<count;++i)
            {
                writePoint(i);
            }
            break;
        }

        case(GL_LINES):
        {
            for(GLsizei i=0;i<count;i+=2)
            {
                writeLine(i, i+1);
            }
            break;
        }
        case(GL_LINE_STRIP):
        {
            for(GLsizei i=1;i<count;++i)
            {
                writeLine(i-1, i);
            }
            break;
        }
        case(GL_LINE_LOOP):
        {
            for(GLsizei i=1;i<count;++i)
            {
                writeLine(i-1, i);
            }
            writeLine(count-1, 0);
            break;
        }
        default:
            CRCore::notify(CRCore::WARN) << "crDXFWritercrNodeVisitor :: can't handle mode " << mode << std::endl; 
            break;
    }
}
                    

// TODO - illegal acad characters
std::string crDXFWritercrNodeVisitor::getLayerName(const std::string& defaultvalue) 
{
    
    std::string layerName=defaultvalue;
    std::transform(layerName.begin(), layerName.end(), layerName.begin(), toupper);
        
    // remove illegal ACAD characters
    size_t found=0;
    const std::string allowed("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_-");
    while ( (found=layerName.find_first_not_of(allowed)) != std::string::npos) {
        layerName[found] = '-';
    }

    // TODO check that changed value isn't also a dupe
    for (std::vector<Layer>::iterator itr=_layers.begin();itr!=_layers.end();itr++) {
        if (itr->_name == layerName ) {
            std::stringstream ss;
            ss << defaultvalue<< "_" << _layers.size();
            layerName = ss.str();
            break;
        }
    }
    
    return layerName;

}

// Now deal with VertexArray directly
//void crDXFWritercrNodeVisitor::processArray(CRCore::Array* array, const Layer &layer,const CRCore::crMatrix& m)
//{
//    if (array == NULL)
//        return;
//    
//    ValueVisitor vv(_fout, layer,m);    
//    for(unsigned int i = 0; i < array->getNumElements(); ++i) {        
//        array->accept(i, vv);        
//    }
//    
//    CRCore::notify(CRCore::DEBUG_INFO) << "processArray "<<layer._name<<"\n";
//    CRCore::notify(CRCore::DEBUG_INFO) << "# " << array->getNumElements() << " elements written" << std::endl;
//    
//}

void crDXFWritercrNodeVisitor::processcrStateSet(CRCore::crStateSet* ss) 
{
    // anything to do if no material/texture?   
    // could detect polygon mode and output in that form?
}

void crDXFWritercrNodeVisitor::processcrGeometry(CRCore::crGeometry* geo, CRCore::crMatrix& m) 
{
    

    // We only want to create a new layer for geometry with something to draw
    if (geo->getVertexArray() && geo->getVertexArray()->getNumElements() ) {

        processcrStateSet(_currentcrStateSet.get());
        
        if ( _firstPass ) {
            // Must have unique layer names
            _layer._name = getLayerName( geo->getName().empty() ? geo->getParent(0)->getName() : geo->getName() );
            //CRCore::notify(CRCore::DEBUG_INFO) << "adding Layer " << _layer._name  << std::endl;

            // if single colour include in header
            if ( CRCore::crGeometry::BIND_OVERALL == geo->getColorBinding() ) {                
                _layer._color = _acadColor.findColor(getcrNodeRGB(geo)); // per layer color
            } else if ( CRCore::crGeometry::BIND_OFF== geo->getColorBinding() ) {
                _layer._color = 0xff; // use white - or can we easily lookup in texture?
            } else {
                _layer._color = 0;  // per point color
            }            
            _layers.push_back(_layer);

        } else {
            _layer = _layers[_count++];
            //CRCore::notify(CRCore::DEBUG_INFO) << "writing Layer " << _layer._name  << std::endl;
            if ( geo->getNumPrimitiveSets() ) {                
                for(unsigned int i = 0; i < geo->getNumPrimitiveSets(); ++i) 
                {        
                    CRCore::crPrimitive* ps = geo->getPrimitiveSet(i);
                    DxfPrimitiveIndexWriter pif(_fout, geo,_layer,_acadColor,m);
                    ps->accept(pif);
                }            
            } else {                            
                // Is array visitor necessary for only dealing with vertex arrays?
                //processArray(geo->getVertexArray(),  _layer,m);                
                if ( geo->getVertexArray() ) {
                    CRCore::Vec3Array* data=static_cast<CRCore::Vec3Array*>(geo->getVertexArray());
                    for (unsigned int ii=0;ii<data->getNumElements();ii++)
                    {
                        CRCore::crVector3 point = data->at(ii) * m;
                        _fout << "0 \nVERTEX\n 8\n"<<_layer._name<<"\n";    
                        if ( _layer._color ) {
                            _fout << "62\n"<<_layer._color<<"\n";                
                        } else {                                
                            _fout << "62\n"<<_acadColor.findColor(getcrNodeRGB(geo,ii))<<"\n";                
                        }
                        _fout<<" 10\n"<<point.x()<<"\n 20\n"<<point.y()<<"\n 30\n"<<point.z()<<"\n";        
                    }
                }
            }
        }
    }
}



void crDXFWritercrNodeVisitor::apply( CRCore::crObject &node )
{

    pushcrStateSet(node.getStateSet());    
    CRCore::crMatrix m = CRCore::computeLocalToWorld(getNodePath());
    unsigned int count = node.getNumDrawables();

    for ( unsigned int i = 0; i < count; i++ )
    {
        CRCore::crGeometry *g = node.getDrawable( i )->asGeometry();
        if ( g != NULL )
        {
            pushcrStateSet(g->getStateSet());            
            processcrGeometry(g,m);            
            popcrStateSet(g->getStateSet());
        }
    }


    popcrStateSet(node.getStateSet());    
}


bool crDXFWritercrNodeVisitor::writeHeader(const CRCore::crBoundingSphere &bound)
{
    if ( _layers.empty() ) {
        return false;
    }
    _fout << "999\n written by OpenSceneGraph" << std::endl;            

    _fout << "0\nSECTION\n2\nHEADER\n";
    _fout << "9\n$ACADVER\n1\nAC1006\n"; // specify minimum autocad version AC1006=R10
    
    _fout << "9\n$EXTMIN\n10\n"<<bound.center().x()-bound.radius()<<"\n20\n"<<bound.center().y()-bound.radius()<<"\n30\n"<<bound.center().z()-bound.radius()<<"\n";
    _fout << "9\n$EXTMAX\n10\n"<<bound.center().x()+bound.radius()<<"\n20\n"<<bound.center().y()+bound.radius()<<"\n30\n"<<bound.center().z()+bound.radius()<<"\n";

    _fout << "0\nENDSEC\n0\nSECTION\n2\nTABLES\n";
    _fout << "0\nTABLE\n2\nLAYER\n";

    for (std::vector<Layer>::iterator itr=_layers.begin();itr!=_layers.end();itr++) {
        if ( itr->_color ) {
            _fout<<"0\nLAYER\n2\n"<<itr->_name<<"\n70\n0\n62\n"<<itr->_color<<"\n6\nContinuous\n"; // color by layer
        } else {
            _fout<<"0\nLAYER\n2\n"<<itr->_name<<"\n70\n0\n62\n255\n6\nContinuous\n";  // most apps won't read 24bit color without a color value in header
        }
    }

    _fout << "0\nENDTAB\n0\nENDSEC\n";

    _fout << "0\nSECTION\n2\nENTITIES\n";
    _firstPass=false; 
    _count=0;

    return true;
}

void crDXFWritercrNodeVisitor::writeFooter()
{
    _fout << "0\nENDSEC\n0\nEOF";
    _fout << std::endl;    
}

