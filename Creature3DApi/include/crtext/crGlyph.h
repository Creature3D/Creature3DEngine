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

#ifndef CRTEXT_CRGLYPH
#define CRTEXT_CRGLYPH 1

#include <string>
#include <istream>

#include <CRCore/crVector2.h>
#include <CRCore/crImage.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crObject.h>

#include <CRText/crExport.h>
#include <CRText/crKerningType.h>
#include <CRText/crStyle.h>

#include <CRCore/thread/crMutex.h>

namespace CRText {

class crFont;
class crText;
class crGlyph3D;
class crGlyphGeometry;
class crGlyphTexture;

class CRTEXT_EXPORT crGlyph : public CRCore::crImage
{
public:

    crGlyph(crFont* font, unsigned int glyphCode);

    crFont* getFont() { return m_font; }
    const crFont* getFont() const { return m_font; }

    unsigned int getGlyphCode() const { return m_glyphCode; }

    void setWidth(float width) { m_width = width; }
    float getWidth() const { return m_width; }

    void setHeight(float height) { m_height = height; }
    float getHeight() const { return m_height; }

    void setHorizontalBearing(const CRCore::crVector2& bearing);
    const CRCore::crVector2& getHorizontalBearing() const;

    void setHorizontalAdvance(float advance);
    float getHorizontalAdvance() const;

    void setVerticalBearing(const CRCore::crVector2& bearing);
    const CRCore::crVector2& getVerticalBearing() const;

    void setVerticalAdvance(float advance);
    float getVerticalAdvance() const;

    void setTexture(crGlyphTexture* texture);
    crGlyphTexture* getTexture();
    const crGlyphTexture* getTexture() const;

    void setTexturePosition(int posX,int posY);
    int getTexturePositionX() const;
    int getTexturePositionY() const;

    void setMinTexCoord(const CRCore::crVector2& coord);
    const CRCore::crVector2& getMinTexCoord() const;

    void setMaxTexCoord(const CRCore::crVector2& coord);
    const CRCore::crVector2& getMaxTexCoord() const;

    void subload() const;

protected:

    virtual ~crGlyph();

    crFont*                       m_font;
    unsigned int                m_glyphCode;

    float                       m_width;
    float                       m_height;

    CRCore::crVector2                   m_horizontalBearing;
    float                       m_horizontalAdvance;

    CRCore::crVector2                   m_verticalBearing;
    float                       m_verticalAdvance;

    crGlyphTexture*               m_texture;
    int                         m_texturePosX;
    int                         m_texturePosY;
    CRCore::crVector2                   m_minTexCoord;
    CRCore::crVector2                   m_maxTexCoord;

    //typedef CRCore::buffered_value<GLuint> GLObjectList;
    //mutable GLObjectList m_globjList;

};

class CRTEXT_EXPORT crGlyphGeometry : public CRCore::Referenced
{
    public:

        crGlyphGeometry();

        void setup(const crGlyph3D* glyph, const crStyle* style);

        bool match(const crStyle* style) const;

        const CRCore::crObject* getGeode() const { return m_geode.get(); }
        const CRCore::crGeometry* getGeometry() const { return m_geometry.get(); }

        /** Set the VertexArray of the glyph. */
        void setVertexArray(CRCore::Vec3Array * va) { m_vertices = va; }
        /** Get the VertexArray of the glyph. */
        const CRCore::Vec3Array * getVertexArray() const { return m_vertices.get(); }

        /** Set the VertexArray of the glyph. */
        void setNormalArray(CRCore::Vec3Array* na) { m_normals = na; }
        /** Get the NormalArray for the wall face. */
        const CRCore::Vec3Array* getNormalArray() const { return m_normals.get(); }

        /** Get the PrimitiveList for the front face. */
        const CRCore::crGeometry::PrimitiveList& getFrontPrimitiveSetList()const { return m_frontPrimitiveSetList; }
        /** Get the PrimitiveList for the wall face. */
        const CRCore::crGeometry::PrimitiveList& getWallPrimitiveSetList()const { return m_wallPrimitiveSetList; }
        /** Get et the PrimitiveList for the back face. */
        const CRCore::crGeometry::PrimitiveList& getBackPrimitiveSetList()const { return m_backPrimitiveSetList; }

        /** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
        //virtual void setThreadSafeRefUnref(bool threadSafe);

    protected:
		virtual ~crGlyphGeometry(){}
        CRCore::ref_ptr<crStyle>             m_style;
        CRCore::ref_ptr<CRCore::crObject>        m_geode;
        CRCore::ref_ptr<CRCore::crGeometry>     m_geometry;
        CRCore::ref_ptr<CRCore::Vec3Array>    m_vertices;
        CRCore::ref_ptr<CRCore::Vec3Array>    m_normals;

        CRCore::crGeometry::PrimitiveList m_frontPrimitiveSetList;
        CRCore::crGeometry::PrimitiveList m_wallPrimitiveSetList;
        CRCore::crGeometry::PrimitiveList m_backPrimitiveSetList;
};


class CRTEXT_EXPORT crGlyph3D : public CRCore::Referenced
{
public:

    crGlyph3D(crFont* font, unsigned int glyphCode);

    crFont* getFont() { return m_font; }
    const crFont* getFont() const { return m_font; }

    unsigned int getGlyphCode() const { return m_glyphCode; }

    void setWidth(float width) { m_width = width; }
    float getWidth() const { return m_width; }

    void setHeight(float height) { m_height = height; }
    float getHeight() const { return m_height; }

    void setHorizontalBearing(const CRCore::crVector2& bearing) { m_horizontalBearing=bearing; }
    const CRCore::crVector2 & getHorizontalBearing() const { return m_horizontalBearing; }

    void setHorizontalAdvance(float advance) { m_horizontalAdvance=advance; }
    float getHorizontalAdvance() const { return m_horizontalAdvance; }

    void setVerticalBearing(const CRCore::crVector2& bearing) {  m_verticalBearing=bearing; }
    const CRCore::crVector2& getVerticalBearing() const { return m_verticalBearing; }

    void setVerticalAdvance(float advance) { m_verticalAdvance=advance; }
    float getVerticalAdvance() const { return m_verticalAdvance; }

    void setBoundingBox(CRCore::crBoundingBox & bb) { m_bb=bb; }
    const CRCore::crBoundingBox & getBoundingBox() const { return m_bb; }


    /** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
    //virtual void setThreadSafeRefUnref(bool threadSafe);


    void setRawVertexArray(CRCore::Vec3Array* vertices) { m_rawVertexArray = vertices; }
    CRCore::Vec3Array* getRawVertexArray() { return m_rawVertexArray.get(); }
    const CRCore::Vec3Array* getRawVertexArray() const { return m_rawVertexArray.get(); }

    /** Get the PrimitiveList for the raw face which hasn't been tessellated. */
    CRCore::crGeometry::PrimitiveList & getRawFacePrimitiveSetList() { return m_rawFacePrimitiveSetList; }
    const CRCore::crGeometry::PrimitiveList & getRawFacePrimitiveSetList() const { return m_rawFacePrimitiveSetList; }

    crGlyphGeometry* getGlyphGeometry(const crStyle* style);

protected:

    virtual ~crGlyph3D() {}

    crFont*                       m_font;
    unsigned int                m_glyphCode;

    float                       m_width;
    float                       m_height;

    CRCore::crVector2                   m_horizontalBearing;
    float                       m_horizontalAdvance;

    CRCore::crVector2                   m_verticalBearing;
    float                       m_verticalAdvance;

    CRCore::crBoundingBox            m_bb;
//        CRCore::crVector2                   _advance;


    CRCore::ref_ptr<CRCore::Vec3Array>    m_rawVertexArray;
    CRCore::crGeometry::PrimitiveList m_rawFacePrimitiveSetList;

    typedef std::list< CRCore::ref_ptr<crGlyphGeometry> >  GlyphGeometries;
    GlyphGeometries m_glyphGeometries;

};


class CRTEXT_EXPORT crGlyphTexture : public CRCore::crTexture2D
{
public:

    crGlyphTexture();

    const char* className() const { return "crGlyphTexture"; }

    /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
    virtual int compare(const CRCore::crStateAttribute& rhs) const;

    /** Set the margin around each glyph, to ensure that texture filtering doesn't bleed adjacent glyph's into each other.*/
    void setGlyphImageMargin(unsigned int margin) { m_margin = margin; }
    unsigned int getGlyphImageMargin() const { return m_margin; }

    void setGlyphImageMarginRatio(float margin) { m_marginRatio = margin; }
    float getGlyphImageMarginRatio() const { return m_marginRatio; }

    bool getSpaceForGlyph(crGlyph* glyph, int& posX, int& posY);

    void addGlyph(crGlyph* glyph,int posX, int posY);

    virtual void apply(CRCore::crState& state) const;

    /** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
    //virtual void setThreadSafeRefUnref(bool threadSafe);

    /** Resize any per context GLObject buffers to specified size. */
    //virtual void resizeGLObjectBuffers(unsigned int maxSize);

    /** create an image that maps all the associated crGlyph's onto a single image, that is equivilant to what will be downloaded to the texture.*/
    CRCore::crImage* createImage();
    
protected:

    virtual ~crGlyphTexture();


    // parameter used to compute the size and position of empty space
    // in the texture which could accommodate new glyphs.
    int m_margin;
    float m_marginRatio;
    int m_usedY;
    int m_partUsedX;
    int m_partUsedY;

    typedef std::vector< CRCore::ref_ptr<crGlyph> > GlyphRefList;
    typedef std::vector< const crGlyph* > GlyphPtrList;
    typedef CRCore::buffered_object< GlyphPtrList > GlyphBuffer;

    GlyphRefList                m_glyphs;
    mutable GlyphBuffer         m_glyphsToSubload;

    mutable CRCore::crMutex  m_mutex;

};

}


#endif
