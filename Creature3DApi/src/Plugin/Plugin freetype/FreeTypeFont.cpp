#include <Plugin Freetype/FreeTypeFont.h>
#include <Plugin Freetype/FreeTypeLibrary.h>

#include <ftoutln.h>
#include <ftbbox.h>

#include <CRCore/crNotify.h>
#include <CRIOManager/crWriteFile.h>

namespace FreeType
{

struct Char3DInfo
{
    Char3DInfo(int numSteps):
        m_verts( new CRCore::Vec3Array ),
        m_geometry( new CRCore::crGeometry ),
        m_numSteps(numSteps),
        m_maxY(-FLT_MAX),
        m_maxX(-FLT_MAX),
        m_minX(FLT_MAX),
        m_minY(FLT_MAX),
        m_coord_scale(1.0/64.0)
    {
        m_geometry->setVertexArray(m_verts.get());
    }

    ~Char3DInfo()
    {
    }

    void completeCurrentPrimitiveSet()
    {
        if (m_currentPrimitiveSet.valid() && m_currentPrimitiveSet->getNumIndices()>1)
        {
            m_geometry->addPrimitive( m_currentPrimitiveSet.get() );
        }
        m_currentPrimitiveSet = 0;
    }

    CRCore::crGeometry* get()
    {
        completeCurrentPrimitiveSet();

        return m_geometry.get();
    }

    void addVertex(CRCore::crVector3 pos)
    {
        m_previous = pos;

        pos *= m_coord_scale;

        if (!m_verts->empty() && m_verts->back()==pos)
        {
            // CRCore::notify(CRCore::NOTICE)<<"addVertex("<<pos<<") duplicate, ignoring"<<std::endl;
            return;
        }

        if (!m_currentPrimitiveSet)
        {
            m_currentPrimitiveSet = new CRCore::DrawElementsUShort( CRCore::crPrimitive::PT_POLYGON);
            m_currentPrimitiveSet->setName("boundary");
        }

        if (!(m_currentPrimitiveSet->empty()) &&
            (*m_verts)[(*m_currentPrimitiveSet)[0]] == pos)
        {
            m_currentPrimitiveSet->push_back( (*m_currentPrimitiveSet)[0] );
        }
        else
        {
            m_currentPrimitiveSet->push_back( m_verts->size() );

            m_verts->push_back( pos );

            setMinMax(pos);
        }
    }

    void moveTo(const CRCore::crVector2& pos)
    {
        completeCurrentPrimitiveSet();

        addVertex( CRCore::crVector3(pos.x(),pos.y(),0) );

    }
    void lineTo(const CRCore::crVector2& pos)
    {
        addVertex( CRCore::crVector3(pos.x(),pos.y(),0) );
    }

    void conicTo(const CRCore::crVector2& control, const CRCore::crVector2& pos)
    {
        CRCore::crVector3 p0 = m_previous;
        CRCore::crVector3 p1 = CRCore::crVector3(control.x(),control.y(),0);
        CRCore::crVector3 p2 = CRCore::crVector3(pos.x(),pos.y(),0);

        double dt = 1.0/m_numSteps;
        double u=0;
        for (int i=0; i<=m_numSteps; ++i)
        {
            double w = 1;
            double bs = 1.0/( (1-u)*(1-u)+2*(1-u)*u*w +u*u );
            CRCore::crVector3 p = (p0*((1-u)*(1-u)) + p1*(2*(1-u)*u*w) + p2*(u*u))*bs;
            addVertex( p );

            u += dt;
        }
    }

    void cubicTo(const CRCore::crVector2& control1, const CRCore::crVector2& control2, const CRCore::crVector2& pos)
    {
        CRCore::crVector3 p0 = m_previous;
        CRCore::crVector3 p1 = CRCore::crVector3(control1.x(),control1.y(),0);
        CRCore::crVector3 p2 = CRCore::crVector3(control2.x(),control2.y(),0);
        CRCore::crVector3 p3 = CRCore::crVector3(pos.x(),pos.y(),0);

        double cx = 3*(p1.x() - p0.x());
        double bx = 3*(p2.x() - p1.x()) - cx;
        double ax = p3.x() - p0.x() - cx - bx;
        double cy = 3*(p1.y() - p0.y());
        double by = 3*(p2.y() - p1.y()) - cy;
        double ay = p3.y() - p0.y() - cy - by;

        double dt = 1.0/m_numSteps;
        double u=0;
        for (int i=0; i<=m_numSteps; ++i)
        {
            CRCore::crVector3 p = CRCore::crVector3( ax*u*u*u + bx*u*u  + cx*u + p0.x(),ay*u*u*u + by*u*u  + cy*u + p0.y(),0 );
            addVertex( p );

            u += dt;
        }
    }

    void setMinMax(const CRCore::crVector3& pos)
    {
        m_maxY = std::max(m_maxY, (double) pos.y());
        m_minY = std::min(m_minY, (double) pos.y());
        m_maxX = std::max(m_maxX, (double) pos.x());
        m_minX = std::min(m_minX, (double) pos.x());
    }

    CRCore::ref_ptr<CRCore::Vec3Array>    m_verts;
    CRCore::ref_ptr<CRCore::DrawElementsUShort> m_currentPrimitiveSet;
    CRCore::ref_ptr<CRCore::crGeometry>     m_geometry;
    CRCore::crVector3                       m_previous;
    int                             m_numSteps;
    double                          m_maxY;
    double                          m_maxX;
    double                          m_minX;
    double                          m_minY;
    double                          m_coord_scale;

};


int moveTo( const FT_Vector* to, void* user )
{
    Char3DInfo* char3d = (Char3DInfo*)user;
    char3d->moveTo( CRCore::crVector2(to->x,to->y) );
    return 0;
}
int lineTo( const FT_Vector* to, void* user )
{
    Char3DInfo* char3d = (Char3DInfo*)user;
    char3d->lineTo( CRCore::crVector2(to->x,to->y) );
    return 0;
}
int conicTo( const FT_Vector* control,const FT_Vector* to, void* user )
{
    Char3DInfo* char3d = (Char3DInfo*)user;
    char3d->conicTo( CRCore::crVector2(control->x,control->y), CRCore::crVector2(to->x,to->y) );
    return 0;
}
int cubicTo( const FT_Vector* control1,const FT_Vector* control2,const FT_Vector* to, void* user )
{
    Char3DInfo* char3d = (Char3DInfo*)user;
    char3d->cubicTo(
        CRCore::crVector2(control1->x,control1->y),
        CRCore::crVector2(control2->x,control2->y),
        CRCore::crVector2(to->x,to->y) );
    return 0;
}

}

FreeTypeFont::FreeTypeFont(const std::string& filename, FT_Face face, unsigned int flags):
    m_currentRes(CRText::FontResolution(0,0)),
    m_filename(filename),
    m_buffer(0),
    m_face(face),
    m_flags(flags)
{
    init();
}

FreeTypeFont::FreeTypeFont(FT_Byte* buffer, FT_Face face, unsigned int flags):
    m_currentRes(CRText::FontResolution(0,0)),
    m_filename(""),
    m_buffer(buffer),
    m_face(face),
    m_flags(flags)
{
    init();
}

FreeTypeFont::~FreeTypeFont()
{
    if(m_face)
    {
        FreeTypeLibrary* freeTypeLibrary = FreeTypeLibrary::instance();
        if (freeTypeLibrary)
        {
            // remove myself from the local registry to ensure that
            // not dangling pointers remain
            freeTypeLibrary->removeFontImplmentation(this);
        
            // free the freetype font face itself
            FT_Done_Face(m_face);
            m_face = 0;

            // release memory held for FT_Face to work
            if (m_buffer)
            {
                delete [] m_buffer;
                m_buffer = 0;
            }
        }
    }
}

void FreeTypeFont::init()
{
    FT_Error _error;
    _error = FT_Set_Pixel_Sizes(m_face, 32, 32);
    if (_error)
    {
        CRCore::notify(CRCore::NOTICE) << "FreeTypeFont3D: set pixel sizes failed ..." << std::endl;
        return;
    }
    m_currentRes.first = 32;
    m_currentRes.second = 32;
}

void FreeTypeFont::setFontResolution(const CRText::FontResolution& fontSize)
{
    if (fontSize==m_currentRes) return;

    int width = fontSize.first;
    int height = fontSize.second;
    int maxAxis = std::max(width, height);
    int margin = m_facade->getGlyphImageMargin() + (int)((float)maxAxis * m_facade->getGlyphImageMarginRatio());

    if ((unsigned int)(width+2*margin) > m_facade->getTextureWidthHint() ||
        (unsigned int)(width+2*margin) > m_facade->getTextureHeightHint())
    {
        CRCore::notify(CRCore::WARN)<<"Warning: FreeTypeFont::setSize("<<width<<","<<height<<") sizes too large,"<<std::endl;
    
        width = m_facade->getTextureWidthHint()-2*margin;
        height = m_facade->getTextureHeightHint()-2*margin;

        CRCore::notify(CRCore::WARN)<<"         sizes capped ("<<width<<","<<height<<") to fit int current glyph texture size."<<std::endl;
    }

    FT_Error error = FT_Set_Pixel_Sizes( m_face,      /* handle to face object  */
                                         width,      /* pixel_width            */
                                         height );   /* pixel_height            */

    if (error)
    {
        CRCore::notify(CRCore::WARN)<<"FT_Set_Pixel_Sizes() - error 0x"<<std::hex<<error<<std::dec<<std::endl;
    }
    else
    {
        m_currentRes = fontSize;
    }

}

CRText::crGlyph* FreeTypeFont::getGlyph(const CRText::FontResolution& fontRes, unsigned int charcode)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(FreeTypeLibrary::instance()->getMutex());

    setFontResolution(fontRes);

    float coord_scale = 1.0f/(float(m_currentRes.second)*64.0f);

    //
    // GT: fix for symbol fonts (i.e. the Webdings font) as the wrong character are being  
    // returned, for symbol fonts in windows (FT_ENCONDING_MS_SYMBOL in freetype) the correct 
    // values are from 0xF000 to 0xF0FF not from 0x000 to 0x00FF (0 to 255) as you would expect.  
    // Microsoft uses a private field for its symbol fonts
    //
    unsigned int charindex = charcode;
    if (m_face->charmap != NULL)
    {
        if (m_face->charmap->encoding == FT_ENCODING_MS_SYMBOL)
        {
            charindex |= 0xF000;
        }
    }

    FT_Error error = FT_Load_Char( m_face, charindex, FT_LOAD_RENDER|FT_LOAD_NO_BITMAP|FT_LOAD_FORCE_AUTOHINT/*|m_flags*/ );
    if (error)
    {
        CRCore::notify(CRCore::WARN) << "FT_Load_Char(...) error 0x"<<std::hex<<error<<std::dec<<std::endl;
        return 0;
    }

	//if(m_bBold)
	//{
	//int strength = 1 << 6;
	//m_face->glyph->outline.flags
	//FT_Outline_Embolden(&m_face->glyph->outline, strength);
	//}

    FT_GlyphSlot glyphslot = m_face->glyph;

    int pitch = glyphslot->bitmap.pitch;
    unsigned char* buffer = glyphslot->bitmap.buffer;

    unsigned int sourceWidth = glyphslot->bitmap.width;;
    unsigned int sourceHeight = glyphslot->bitmap.rows;
    
    unsigned int width = sourceWidth;
    unsigned int height = sourceHeight;

    CRCore::ref_ptr<CRText::crGlyph> glyph = new CRText::crGlyph(m_facade, charcode);
    
    unsigned int dataSize = width*height;
    unsigned char* data = new unsigned char[dataSize];
    

    // clear the image to zeros.
    for(unsigned char* p=data;p<data+dataSize;) { *p++ = 0; }

    glyph->setImage(width,height,1,
                    GL_ALPHA,
                    GL_ALPHA,GL_UNSIGNED_BYTE,
                    data,
                    CRCore::crImage::USE_NEW_DELETE,
                    1);

    glyph->setInternalTextureFormat(GL_ALPHA);

    // copy image across to CRText::crGlyph image.     
    switch(glyphslot->bitmap.pixel_mode)
    {
        case FT_PIXEL_MODE_MONO:
            for(int r=sourceHeight-1;r>=0;--r)
            {
                unsigned char* ptr = buffer+r*pitch;
                for(unsigned int c=0;c<sourceWidth;++c)
                {
                    (*data++)= (ptr[c >> 3] & (1 << (~c & 7))) ? 255 : 0;
                }
            }
            break;

        
        case FT_PIXEL_MODE_GRAY:
            for(int r=sourceHeight-1;r>=0;--r)
            {
                unsigned char* ptr = buffer+r*pitch;
                for(unsigned int c=0;c<sourceWidth;++c,++ptr)
                {
                    (*data++)=*ptr;
                }
            }
            break;
            
        default:
            CRCore::notify(CRCore::WARN) << "FT_Load_Char(...) returned bitmap with unknown pixel_mode " << glyphslot->bitmap.pixel_mode << std::endl;
    }


    FT_Glyph_Metrics* metrics = &(m_face->glyph->metrics);

    glyph->setWidth((float)metrics->width * coord_scale);
    glyph->setHeight((float)metrics->height * coord_scale);
    glyph->setHorizontalBearing(CRCore::crVector2((float)metrics->horiBearingX * coord_scale,(float)(metrics->horiBearingY-metrics->height) * coord_scale)); // bottom left.
    glyph->setHorizontalAdvance((float)metrics->horiAdvance * coord_scale);
    glyph->setVerticalBearing(CRCore::crVector2((float)metrics->vertBearingX * coord_scale,(float)(metrics->vertBearingY-metrics->height) * coord_scale)); // top middle.
    glyph->setVerticalAdvance((float)metrics->vertAdvance * coord_scale);

#if 0
    CRCore::notify(CRCore::NOTICE)<<"getGlyph("<<charcode<<", "<<char(charcode)<<")"<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   height="<<glyph->getHeight()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   width="<<glyph->getWidth()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   horizontalBearing="<<glyph->getHorizontalBearing()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   horizontalAdvance="<<glyph->getHorizontalAdvance()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   verticalBearing="<<glyph->getHorizontalBearing()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   verticalAdvance="<<glyph->getVerticalAdvance()<<std::endl;
#endif
    
//    cout << "      in getGlyph() implementation="<<this<<"  "<<m_filename<<"  facade="<<m_facade<<endl;

    return glyph.release();

}

CRText::crGlyph3D * FreeTypeFont::getGlyph3D(unsigned int charcode)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(FreeTypeLibrary::instance()->getMutex());

    //
    // GT: fix for symbol fonts (i.e. the Webdings font) as the wrong character are being
    // returned, for symbol fonts in windows (FT_ENCONDING_MS_SYMBOL in freetype) the correct
    // values are from 0xF000 to 0xF0FF not from 0x000 to 0x00FF (0 to 255) as you would expect.
    // Microsoft uses a private field for its symbol fonts
    //
    unsigned int charindex = charcode;
    if (m_face->charmap != NULL)
    {
        if (m_face->charmap->encoding == FT_ENCODING_MS_SYMBOL)
        {
            charindex |= 0xF000;
        }
    }

    FT_Error error = FT_Load_Char( m_face, charindex, FT_LOAD_DEFAULT|m_flags );
    if (error)
    {
        CRCore::notify(CRCore::WARN) << "FT_Load_Char(...) error 0x"<<std::hex<<error<<std::dec<<std::endl;
        return 0;
    }
    if (m_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    {
        CRCore::notify(CRCore::WARN) << "FreeTypeFont3D::getGlyph : not a vector font" << std::endl;
        return 0;
    }

    //float coord_scale = _freetype_scale/64.0f;
    //float coord_scale = (1.0f/float(fontDPI))/64.0f;
    float coord_scale = 1.0f/(float(m_currentRes.second)*64.0f);

    // ** init FreeType to describe the glyph
    FreeType::Char3DInfo char3d(m_facade->getNumberCurveSamples());
    char3d.m_coord_scale = coord_scale;

    FT_Outline outline = m_face->glyph->outline;
    FT_Outline_Funcs funcs;
    funcs.conic_to = (FT_Outline_ConicToFunc)&FreeType::conicTo;
    funcs.line_to = (FT_Outline_LineToFunc)&FreeType::lineTo;
    funcs.cubic_to = (FT_Outline_CubicToFunc)&FreeType::cubicTo;
    funcs.move_to = (FT_Outline_MoveToFunc)&FreeType::moveTo;
    funcs.shift = 0;
    funcs.delta = 0;

    // ** record description
    FT_Error _error = FT_Outline_Decompose(&outline, &funcs, &char3d);
    if (_error)
    {
        CRCore::notify(CRCore::WARN) << "FreeTypeFont3D::getGlyph : - outline decompose failed ..." << std::endl;
        return 0;
    }

    // ** create geometry for each part of the glyph
    CRCore::ref_ptr<CRCore::crGeometry> frontGeo(new CRCore::crGeometry);

    CRCore::ref_ptr<CRCore::Vec3Array> rawVertices = new CRCore::Vec3Array(*(char3d.m_verts));
    CRCore::crGeometry::PrimitiveList rawPrimitives;
    for(CRCore::crGeometry::PrimitiveList::iterator itr = char3d.get()->getPrimitiveList().begin();
        itr != char3d.get()->getPrimitiveList().end();
        ++itr)
    {
        rawPrimitives.push_back(dynamic_cast<CRCore::crPrimitive*>((*itr)->clone(CRCore::crCopyOp::DEEP_COPY_ALL)));
    }

    // ** save vertices and PrimitiveList of each face in the crGlyph3D crPrimitive face list
    CRCore::ref_ptr<CRText::crGlyph3D> glyph = new CRText::crGlyph3D(m_facade, charcode);

    // copy the raw primitive set list before we tessellate it.
    glyph->getRawFacePrimitiveSetList() = rawPrimitives;
    glyph->setRawVertexArray(rawVertices.get());


    FT_Glyph_Metrics* metrics = &(m_face->glyph->metrics);

    glyph->setWidth((float)metrics->width * coord_scale);
    glyph->setHeight((float)metrics->height * coord_scale);
    glyph->setHorizontalBearing(CRCore::crVector2((float)metrics->horiBearingX * coord_scale,(float)(metrics->horiBearingY-metrics->height) * coord_scale)); // bottom left.
    glyph->setHorizontalAdvance((float)metrics->horiAdvance * coord_scale);
    glyph->setVerticalBearing(CRCore::crVector2((float)metrics->vertBearingX * coord_scale,(float)(metrics->vertBearingY-metrics->height) * coord_scale)); // top middle.
    glyph->setVerticalAdvance((float)metrics->vertAdvance * coord_scale);

#if 0
    CRCore::notify(CRCore::NOTICE)<<"getGlyph3D("<<charcode<<", "<<char(charcode)<<")"<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   height="<<glyph->getHeight()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   width="<<glyph->getWidth()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   horizontalBearing="<<glyph->getHorizontalBearing()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   horizontalAdvance="<<glyph->getHorizontalAdvance()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   verticalBearing="<<glyph->getHorizontalBearing()<<std::endl;
    CRCore::notify(CRCore::NOTICE)<<"   verticalAdvance="<<glyph->getVerticalAdvance()<<std::endl;
#endif

    FT_BBox ftbb;
    FT_Outline_Get_BBox(&outline, &ftbb);
    CRCore::crBoundingBox bb(float(ftbb.xMin) * coord_scale, float(ftbb.yMin) * coord_scale, 0.0f, float(ftbb.xMax) * coord_scale, float(ftbb.yMax) * coord_scale, 0.0f);

    glyph->setBoundingBox(bb);

    return glyph.release();
}

CRCore::crVector2 FreeTypeFont::getKerning(unsigned int leftcharcode,unsigned int rightcharcode, CRText::KerningType kerningType)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(FreeTypeLibrary::instance()->getMutex());

    if (!FT_HAS_KERNING(m_face) || (kerningType == CRText::KERNING_NONE)) return CRCore::crVector2(0.0f,0.0f);

    FT_Kerning_Mode mode = (kerningType==CRText::KERNING_DEFAULT) ? ft_kerning_default : ft_kerning_unfitted;

    // convert character code to glyph index
    FT_UInt left = FT_Get_Char_Index( m_face, leftcharcode );
    FT_UInt right = FT_Get_Char_Index( m_face, rightcharcode );
    
    // get the kerning distances.   
    FT_Vector  kerning;

    FT_Error error = FT_Get_Kerning( m_face,                     // handle to face object
                                     left,                      // left glyph index
                                     right,                     // right glyph index
                                     mode,                      // kerning mode
                                     &kerning );                // target vector

    if (error)
    {
        CRCore::notify(CRCore::WARN) << "FT_Get_Kerning(...) returned error code " <<std::hex<<error<<std::dec<< std::endl;
        return CRCore::crVector2(0.0f,0.0f);
    }

    //float coord_scale = _freetype_scale/64.0f;
    //float coord_scale = 1.0f/64.0f;
    float coord_scale = 1.0f/(float(m_currentRes.second)*64.0f);

    return CRCore::crVector2((float)kerning.x*coord_scale,(float)kerning.y*coord_scale);
}

bool FreeTypeFont::hasVertical() const
{
    CRCore::ScopedLock<CRCore::crMutex> lock(FreeTypeLibrary::instance()->getMutex());
    return FT_HAS_VERTICAL(m_face)!=0;
}
