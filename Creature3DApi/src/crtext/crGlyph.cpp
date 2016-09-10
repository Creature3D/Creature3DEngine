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

#include <CRText/crGlyph.h>
#include <CRText/crFont.h>
#include <CRText/crText.h>

#include <CRCore/crState.h>
#include <CRCore/crNotify.h>
#include <crgl/glu.h>

#include <CRUtil/crSmoothingVisitor.h>

#include <string.h>
#include <stdlib.h>
#include <CRText/crGlyphGeometry.h>
#include <CRCore/crStreamBuf.h>
#include <CRCore/crFrameStamp.h>
using namespace CRCore;
using namespace CRText;
using namespace std;

crGlyphTexture::crGlyphTexture():
    m_margin(1),
    m_marginRatio(0.02f),
    m_usedY(0),
    m_partUsedX(0),
    m_partUsedY(0)
{
    setWrap(WRAP_S, CLAMP_TO_EDGE);
    setWrap(WRAP_T, CLAMP_TO_EDGE);
}

crGlyphTexture::~crGlyphTexture() 
{
	for( GlyphRefList::iterator itr = m_glyphs.begin();
		 itr != m_glyphs.end();
		 ++itr )
	{
		(*itr)->setTexture(NULL);
	}
	
}

// return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.
int crGlyphTexture::compare(const CRCore::crStateAttribute& rhs) const
{
    if (this<&rhs) return -1;
    else if (this>&rhs) return 1;
    return 0;
}


bool crGlyphTexture::getSpaceForGlyph(crGlyph* glyph, int& posX, int& posY)
{
    int maxAxis = std::max(glyph->s(), glyph->t());
    int margin = m_margin + (int)((float)maxAxis * m_marginRatio);
    
    int width = glyph->s()+2*margin;
    int height = glyph->t()+2*margin;

    // first check box (m_partUsedX,m_usedY) to (width,height)
    if (width <= (getTextureWidth()-m_partUsedX) &&
        height <= (getTextureHeight()-m_usedY))
    {
        // can fit in existing row.

        // record the position in which the texture will be stored.
        posX = m_partUsedX+margin;
        posY = m_usedY+margin;        

        // move used markers on.
        m_partUsedX += width;
        if (m_usedY+height>m_partUsedY) m_partUsedY = m_usedY+height;

        return true;
    }
    
    // start an new row.
    if (width <= getTextureWidth() &&
        height <= (getTextureHeight()-m_partUsedY))
    {
        // can fit next row.
        m_partUsedX = 0;
        m_usedY = m_partUsedY;

        posX = m_partUsedX+margin;
        posY = m_usedY+margin;        

        // move used markers on.
        m_partUsedX += width;
        if (m_usedY+height>m_partUsedY) m_partUsedY = m_usedY+height;

        return true;
    }

    // doesn't fit into glyph.
    return false;
}

void crGlyphTexture::addGlyph(crGlyph* glyph, int posX, int posY)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    m_glyphs.push_back(glyph);
    for(unsigned int i=0;i<m_glyphsToSubload.size();++i)
    {
        m_glyphsToSubload[i].push_back(glyph);
    }

    // set up the details of where to place glyph's image in the texture.
    glyph->setTexture(this);
    glyph->setTexturePosition(posX,posY);

    glyph->setMinTexCoord( CRCore::crVector2( static_cast<float>(posX)/static_cast<float>(getTextureWidth()),
                                      static_cast<float>(posY)/static_cast<float>(getTextureHeight()) ) );
    glyph->setMaxTexCoord( CRCore::crVector2( static_cast<float>(posX+glyph->s())/static_cast<float>(getTextureWidth()),
                                      static_cast<float>(posY+glyph->t())/static_cast<float>(getTextureHeight()) ) );
}

void crGlyphTexture::apply(CRCore::crState& state) const
{
    // get the contextID (user defined ID of 0 upwards) for the 
    // current OpenGL context.
    const unsigned int contextID = state.getContextID();

    if (contextID>=m_glyphsToSubload.size())
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

        // graphics context is beyond the number of glyphsToSubloads, so
        // we must now copy the glyph list across, this is a potential
        // threading issue though is multiple applies are happening the
        // same time on this object - to avoid this condition number of
        // graphics contexts should be set before create text.
        for(unsigned int i=m_glyphsToSubload.size();i<=contextID;++i)
        {
            GlyphPtrList& glyphPtrs = m_glyphsToSubload[i];
            for(GlyphRefList::const_iterator itr=m_glyphs.begin();
                itr!=m_glyphs.end();
                ++itr)
            {
                glyphPtrs.push_back(itr->get());
            }
        }
    }


    const Extensions* extensions = getExtensions(contextID,true);
    bool generateMipMapSupported = extensions->isGenerateMipMapSupported();

    // get the texture object for the current contextID.
    TextureObject* textureObject = getTextureObject(contextID);
    
    bool newTextureObject = (textureObject == 0);

    if (newTextureObject)
    {
        GLint maxTextureSize = 256;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        if (maxTextureSize < getTextureWidth() || maxTextureSize < getTextureHeight())
        {
            CRCore::notify(CRCore::WARN)<<"Warning: CRText::crFont texture size of ("<<getTextureWidth()<<", "<<getTextureHeight()<<") too large, unable to create font texture."<<std::endl;
            CRCore::notify(CRCore::WARN)<<"         Maximum supported by hardward by native OpenGL implementation is ("<<maxTextureSize<<","<<maxTextureSize<<")."<<std::endl;
            CRCore::notify(CRCore::WARN)<<"         Please set OSG_MAX_TEXTURE_SIZE lenvironment variable to "<<maxTextureSize<<" and re-run application."<<std::endl;
            return;
        }
        
        // being bound for the first time, need to allocate the texture
        m_textureObjectBuffer[contextID] = textureObject = CRCore::crTexture::generateTextureObject(
                contextID,GL_TEXTURE_2D,1,GL_ALPHA,getTextureWidth(), getTextureHeight(),1,0);

        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());


        applyTexParameters(GL_TEXTURE_2D,state);

        
        // need to look at generate mip map extension if mip mapping required.
        switch(m_min_filter)
        {
        case NEAREST_MIPMAP_NEAREST:
        case NEAREST_MIPMAP_LINEAR:
        case LINEAR_MIPMAP_NEAREST:
        case LINEAR_MIPMAP_LINEAR:
            if (generateMipMapSupported)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
            }
            else glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, LINEAR);
            break;
        default:
            // not mip mapping so no problems.
            break;
        }
        
        unsigned int imageDataSize = getTextureHeight()*getTextureWidth();
        unsigned char* imageData = new unsigned char[imageDataSize];
		memset(imageData,0,imageDataSize);
        //for(unsigned int i=0; i<imageDataSize; ++i)
        //{
        //    imageData[i] = 0;
        //}
        

        // allocate the texture memory.
        glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA,
                getTextureWidth(), getTextureHeight(), 0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                imageData );
                
        delete [] imageData;
    
    }
    else
    {
        // reuse texture by binding.
        textureObject->bind(crFrameStamp::getInstance()->getReferenceTime());
        
        if (getTextureParameterDirty(contextID))
        {
            applyTexParameters(GL_TEXTURE_2D,state);
        }


    }
    
    //static const GLubyte* s_renderer = 0;
    //static bool s_subloadAllGlyphsTogether = false;
    //if (!s_renderer)
    //{
    //    //CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    //    s_renderer = glGetString(GL_RENDERER);
    //    CRCore::notify(CRCore::INFO)<<"glGetString(GL_RENDERER)=="<<s_renderer<<std::endl;
    //    if (s_renderer && strstr((const char*)s_renderer,"IMPACT")!=0)
    //    {
    //        // we're running on an Octane, so need to work around its
    //        // subloading bugs by loading all at once.
    //        s_subloadAllGlyphsTogether = true;
    //    }
    //    
    //    if (s_renderer && 
    //        ((strstr((const char*)s_renderer,"Radeon")!=0) || 
    //        (strstr((const char*)s_renderer,"RADEON")!=0) ||
    //        (strstr((const char*)s_renderer,"ALL-IN-WONDER")!=0)))
    //    {
    //        // we're running on an ATI, so need to work around its
    //        // subloading bugs by loading all at once.
    //        s_subloadAllGlyphsTogether = true;
    //    }

    //    if (s_renderer && strstr((const char*)s_renderer,"Sun")!=0)
    //    {
    //        // we're running on an solaris x server, so need to work around its
    //        // subloading bugs by loading all at once.
    //        s_subloadAllGlyphsTogether = true;
    //    }

    //    //const char* str = getenv("OSG_TEXT_INCREMENTAL_SUBLOADING");
    //    //if (str)
    //    //{
    //    //    s_subloadAllGlyphsTogether = strcmp(str,"OFF")==0 || strcmp(str,"Off")==0 || strcmp(str,"off")==0;
    //    //}
    //}


    // now subload the glyphs that are outstanding for this graphics context.
    GlyphPtrList& glyphsWereSubloading = m_glyphsToSubload[contextID];

    if (!glyphsWereSubloading.empty() || newTextureObject)
    {
        CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

        //if (!s_subloadAllGlyphsTogether)
        //{
            if (newTextureObject)
            {
                for(GlyphRefList::const_iterator itr=m_glyphs.begin();
                    itr!=m_glyphs.end();
                    ++itr)
                {
                    (*itr)->subload();
                }
            }
            else // just subload the new entries.
            {
                // default way of subloading as required.
                //std::cout<<"subloading"<<std::endl;
                for(GlyphPtrList::iterator itr=glyphsWereSubloading.begin();
                    itr!=glyphsWereSubloading.end();
                    ++itr)
                {
                    (*itr)->subload();
                }
            }
            
            // clear the list since we have now subloaded them.
            glyphsWereSubloading.clear();
            
        //}
        //else
        //{
        //    CRCore::notify(CRCore::INFO)<<"CRText::crFont loading all glyphs as a single subload."<<std::endl;

        //    // Octane has bugs in OGL driver which mean that subloads smaller
        //    // than 32x32 produce errors, and also cannot handle general alignment,
        //    // so to get round this copy all glyphs into a temporary image and
        //    // then subload the whole lot in one go.

        //    int tsize = getTextureHeight() * getTextureWidth();
        //    unsigned char *local_data = new unsigned char[tsize];
        //    memset( local_data, 0L, tsize);

        //    for(GlyphRefList::const_iterator itr=m_glyphs.begin();
        //        itr!=m_glyphs.end();
        //        ++itr)
        //    {
        //        //(*itr)->subload();

        //        // Rather than subloading to graphics, we'll write the values
        //        // of the glyphs into some intermediate data and subload the
        //        // whole thing at the end
        //        for( int t = 0; t < (*itr)->t(); t++ )
        //        {
        //            for( int s = 0; s < (*itr)->s(); s++ )
        //            {
        //                int sindex = (t*(*itr)->s()+s);
        //                int dindex =  
        //                    ((((*itr)->getTexturePositionY()+t) * getTextureWidth()) +
        //                    ((*itr)->getTexturePositionX()+s));

        //                const unsigned char *sptr = &(*itr)->data()[sindex];
        //                unsigned char *dptr       = &local_data[dindex];

        //                (*dptr)   = (*sptr);
        //            }
        //        }
        //    }

        //    // clear the list since we have now subloaded them.
        //    glyphsWereSubloading.clear();

        //    // Subload the image once
        //    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 
        //            getTextureWidth(),
        //            getTextureHeight(),
        //            GL_ALPHA, GL_UNSIGNED_BYTE, local_data );

        //    delete [] local_data;

        //}
    }
    else
    {
//        CRCore::notify(CRCore::INFO) << "no need to subload "<<std::endl;
    }



//     if (generateMipMapTurnedOn)
//     {
//         glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS,GL_FALSE);
//     }


}

//void crGlyphTexture::setThreadSafeRefUnref(bool threadSafe)
//{
//    CRCore::crTexture2D::setThreadSafeRefUnref(threadSafe);
//}

//void crGlyphTexture::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    CRCore::crTexture2D::resizeGLObjectBuffers(maxSize);
//
//    unsigned int initialSize = m_glyphsToSubload.size();
//    m_glyphsToSubload.resize(maxSize);
//    
//    for(unsigned i=initialSize; i<m_glyphsToSubload.size(); ++i)
//    {
//        for(GlyphRefList::iterator itr = m_glyphs.begin();
//            itr != m_glyphs.end();
//            ++itr)
//        {
//            m_glyphsToSubload[i].push_back(itr->get());
//        }
//    }
//}

CRCore::crImage* crGlyphTexture::createImage()
{
	CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
    CRCore::ref_ptr<CRCore::crImage> image = new CRCore::crImage;
    image->allocateImage(getTextureWidth(), getTextureHeight(), 1, GL_ALPHA, GL_UNSIGNED_BYTE);
    memset(image->data(), 0, image->getTotalSizeInBytes());

    for(GlyphRefList::iterator itr = m_glyphs.begin();
        itr != m_glyphs.end();
        ++itr)
    {
        crGlyph* glyph = itr->get();
        image->copySubImage(glyph->getTexturePositionX(), glyph->getTexturePositionY(), 0, glyph);
    }

    return image.release();
}

// all the methods in crFont::crGlyph have been made non inline because VisualStudio6.0 is STUPID, STUPID, STUPID PILE OF JUNK.
crGlyph::crGlyph(crFont* font, unsigned int glyphCode):
    m_font(font),
    m_glyphCode(glyphCode),
    m_width(1.0f),
    m_height(1.0f),
    m_horizontalBearing(0.0f,0.f),
    m_horizontalAdvance(0.f),
    m_verticalBearing(0.0f,0.f),
    m_verticalAdvance(0.f),
    m_texture(0),
    m_texturePosX(0),
    m_texturePosY(0),
    m_minTexCoord(0.0f,0.0f),
    m_maxTexCoord(0.0f,0.0f)
{
    //setThreadSafeRefUnref(true);
}

crGlyph::~crGlyph()
{
	m_texture = NULL;
	m_font = NULL;
}

void crGlyph::setHorizontalBearing(const CRCore::crVector2& bearing) {  m_horizontalBearing=bearing; }
const CRCore::crVector2& crGlyph::getHorizontalBearing() const { return m_horizontalBearing; }

void crGlyph::setHorizontalAdvance(float advance) { m_horizontalAdvance=advance; }
float crGlyph::getHorizontalAdvance() const { return m_horizontalAdvance; }

void crGlyph::setVerticalBearing(const CRCore::crVector2& bearing) {  m_verticalBearing=bearing; }
const CRCore::crVector2& crGlyph::getVerticalBearing() const { return m_verticalBearing; }

void crGlyph::setVerticalAdvance(float advance) {  m_verticalAdvance=advance; }
float crGlyph::getVerticalAdvance() const { return m_verticalAdvance; }

void crGlyph::setTexture(crGlyphTexture* texture) { m_texture = texture; }
crGlyphTexture* crGlyph::getTexture() { return m_texture; }
const crGlyphTexture* crGlyph::getTexture() const { return m_texture; }

void crGlyph::setTexturePosition(int posX,int posY) { m_texturePosX = posX; m_texturePosY = posY; }
int crGlyph::getTexturePositionX() const { return m_texturePosX; }
int crGlyph::getTexturePositionY() const { return m_texturePosY; }

void crGlyph::setMinTexCoord(const CRCore::crVector2& coord) { m_minTexCoord=coord; }
const CRCore::crVector2& crGlyph::getMinTexCoord() const { return m_minTexCoord; }

void crGlyph::setMaxTexCoord(const CRCore::crVector2& coord) { m_maxTexCoord=coord; }
const CRCore::crVector2& crGlyph::getMaxTexCoord() const { return m_maxTexCoord; }

void crGlyph::subload() const
{
    //GLenum errorNo = glGetError();
    //if (errorNo!=GL_NO_ERROR)
    //{
    //    const GLubyte* msg = gluErrorString(errorNo);
    //    if (msg) { CRCore::notify(CRCore::WARN)<<"before crGlyph::subload(): detected OpenGL error: "<<msg<<std::endl; }
    //    else  { CRCore::notify(CRCore::WARN)<<"before crGlyph::subload(): detected OpenGL error number: "<<errorNo<<std::endl; }
    //}

    if(s() <= 0 || t() <= 0)
    {
       // CRCore::notify(CRCore::INFO)<<"crGlyph::subload(): texture sub-image width and/or height of 0, ignoring operation."<<std::endl;
        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT,getPacking());

    glTexSubImage2D(GL_TEXTURE_2D,0,
                    m_texturePosX,m_texturePosY,
                    s(),t(),
                    (GLenum)getPixelFormat(),
                    (GLenum)getDataType(),
                    data());
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crGlyph OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
/*    errorNo = glGetError();
    if (errorNo!=GL_NO_ERROR)
    {


        const GLubyte* msg = gluErrorString(errorNo);
        if (msg) { CRCore::notify(CRCore::WARN)<<"after crGlyph::subload() : detected OpenGL error: "<<msg<<std::endl; }
        else { CRCore::notify(CRCore::WARN)<<"after crGlyph::subload() : detected OpenGL error number: "<<errorNo<<std::endl; }

        CRCore::notify(CRCore::WARN)<< "\tglTexSubImage2D(0x"<<hex<<GL_TEXTURE_2D<<dec<<" ,"<<0<<"\t"<<std::endl<<
                                 "\t                "<<m_texturePosX<<" ,"<<m_texturePosY<<std::endl<<
                                 "\t                "<<s()<<" ,"<<t()<<std::endl<<hex<<
                                 "\t                0x"<<(GLenum)getPixelFormat()<<std::endl<<
                                 "\t                0x"<<(GLenum)getDataType()<<std::endl<<
                                 "\t                0x"<<(unsigned long)data()<<");"<<dec<<std::endl;
    }*/                    
}

crGlyph3D::crGlyph3D(crFont* font, unsigned int glyphCode):
    CRCore::Referenced(),
    m_font(font),
    m_glyphCode(glyphCode),
    m_width(1.0f),
    m_height(1.0f),
    m_horizontalBearing(0,0),
    m_horizontalAdvance(0),
    m_verticalBearing(0,0),
    m_verticalAdvance(0)
    {}

//void crGlyph3D::setThreadSafeRefUnref(bool threadSafe)
//{
//    GlyphGeometries m_glyphGeometries;
//    for(GlyphGeometries::iterator itr = m_glyphGeometries.begin();
//        itr != m_glyphGeometries.end();
//        ++itr)
//    {
//        (*itr)->setThreadSafeRefUnref(threadSafe);
//    }
//}

crGlyphGeometry* crGlyph3D::getGlyphGeometry(const crStyle* style)
{

    for(GlyphGeometries::iterator itr = m_glyphGeometries.begin();
        itr != m_glyphGeometries.end();
        ++itr)
    {
        crGlyphGeometry* glyphGeometry = itr->get();
        if (glyphGeometry->match(style))
        {
            //CRCore::notify(CRCore::INFO)<<"crGlyph3D::getGlyphGeometry(crStyle* style) found matching crGlyphGeometry."<<std::endl;
            return glyphGeometry;
        }
    }

    //CRCore::notify(CRCore::INFO)<<"crGlyph3D::getGlyphGeometry(crStyle* style) could not find matching crGlyphGeometry, creating a new one."<<std::endl;

    CRCore::ref_ptr<crGlyphGeometry> glyphGeometry = new crGlyphGeometry();
    glyphGeometry->setup(this, style);
    m_glyphGeometries.push_back(glyphGeometry);

    return glyphGeometry.get();
}


crGlyphGeometry::crGlyphGeometry()
{
}

//void crGlyphGeometry::setThreadSafeRefUnref(bool threadSafe)
//{
//    if (m_geode.valid()) m_geode->setThreadSafeRefUnref(threadSafe);
//}

void crGlyphGeometry::setup(const crGlyph3D* glyph, const crStyle* style)
{
    float creaseAngle = 30.0f;
    bool smooth = true;
    CRCore::ref_ptr<CRCore::crGeometry> shellGeometry;

    if (!style)
    {
       // CRCore::notify(CRCore::INFO)<<"crGlyphGeometry::setup(const crGlyph* glyph, NULL) creating default glyph geometry."<<std::endl;

        float width = 0.1f;

        m_geometry = CRText::computeTextGeometry(glyph, width);
    }
    else
    {
        //CRCore::notify(CRCore::INFO)<<"crGlyphGeometry::setup(const crGlyph* glyph, NULL) create glyph geometry with custom crStyle."<<std::endl;

        // record the style
        m_style = dynamic_cast<crStyle*>(style->clone(CRCore::crCopyOp::DEEP_COPY_ALL));

        const crBevel* bevel = style ? style->getBevel() : 0;
        bool outline = style ? style->getOutlineRatio()>0.0f : false;
        float width = style->getThicknessRatio();

        if (bevel)
        {
            float thickness = bevel->getBevelThickness();

            CRCore::ref_ptr<CRCore::crGeometry> glyphGeometry = CRText::computeGlyphGeometry(glyph, thickness, width);

            m_geometry = CRText::computeTextGeometry(glyphGeometry.get(), *bevel, width);
            shellGeometry = outline ? CRText::computeShellGeometry(glyphGeometry.get(), *bevel, width) : 0;
        }
        else
        {
            m_geometry = CRText::computeTextGeometry(glyph, width);
        }
    }

    if (!m_geometry)
    {
       // CRCore::notify(CRCore::INFO)<<"Warning: crGlyphGeometry::setup(const crGlyph* glyph, const crStyle* style) failed."<<std::endl;
        return;
    }

    m_geode = new CRCore::crObject;
    m_geode->addDrawable(m_geometry.get());
    if (shellGeometry.valid()) m_geode->addDrawable(shellGeometry.get());

    // create the normals
    if (smooth)
    {
        CRUtil::crSmoothingVisitor::smooth(*m_geometry, CRCore::DegreesToRadians(creaseAngle));
    }

    m_vertices = dynamic_cast<CRCore::Vec3Array*>(m_geometry->getVertexArray());
    m_normals = dynamic_cast<CRCore::Vec3Array*>(m_geometry->getNormalArray());

	CRCore::crPrimitive* prim;
    for(CRCore::crGeometry::PrimitiveList::iterator itr = m_geometry->getPrimitiveList().begin();
        itr != m_geometry->getPrimitiveList().end();
        ++itr)
    {
        prim = itr->get();
        if (prim->getName()=="front") m_frontPrimitiveSetList.push_back(prim);
        else if (prim->getName()=="back") m_backPrimitiveSetList.push_back(prim);
        else if (prim->getName()=="wall") m_wallPrimitiveSetList.push_back(prim);
    }
}

bool crGlyphGeometry::match(const crStyle* style) const
{
    if (m_style == style) return true;
    if (!m_style || !style) return false;

    return (*m_style==*style);
}
