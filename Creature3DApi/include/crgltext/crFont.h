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
#ifndef CRGLTEXT_CRFONT
#define CRGLTEXT_CRFONT 1

#include <CRCore/crVector2.h>
#include <CRCore/crImage.h>
#include <CRCore/crTexture2D.h>
#include <CRCore/crStateSet.h>
#include <CRCore/buffered_value.h>
#include <CRCore/crTexEnv.h>
#include <CRGLText/crExport.h>

#include <string>

#define CR_FONT_USE_LUMINANCE_ALPHA

namespace CRGLText {

class crFont;
class crText;

enum KerningType
{
    KERNING_DEFAULT,    //default locked to integer kerning values
    KERNING_UNFITTED,   //use floating point value for kerning
    KERNING_NONE        //no kerning
};

/** read a font from specified file.*/
extern CRGLTEXT_EXPORT crFont* readFontFile(const std::string& filename);


/** Pure virtual base class for fonts.
  * Concrete implementation are the crDefaultFont found in src/crText/crDefaultFont.cpp
  * and FreeTypeFont found in src/osgPlugins/freetype/FreeTypeFont.cpp*/
class CRGLTEXT_EXPORT crFont : public CRCore::crBase
{
// declare the interface to a font.
public:

    // forward declare nested classes.
    class Glyph;
    class GlyphTexture;
    class FontImplementation;

    crFont(FontImplementation* implementation=0);

    virtual CRCore::crBase* cloneType() const { return 0; } // cloneType() not appropriate
    virtual CRCore::crBase* clone(const CRCore::crCopyOp&) const { return 0; } // clone() not appropriate
    virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crFont*>(obj)!=NULL; }
    virtual const char* className() const { return "crFont"; }
    virtual const char* libraryName() const { return "CRGLText"; }


	///** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
	//crFont(const crFont& front,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY){};

    //META_Base(CRGLText,crFont);

    virtual std::string getFileName() const;

    /** Set the pixel width and height hint.*/
    virtual void setSize(unsigned int width, unsigned int height);
    
    unsigned int getWidth() const;
    unsigned int getHeight() const;

    /** Get a kerning (adjustment of spacing of two adjacent character) for specified charcodes, w.r.t the current font size hint.*/
    virtual CRCore::crVector2 getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType);

    /** Get a Glyph for specified charcode, and the font size nearest to the current font size hint.*/
    virtual Glyph* getGlyph(unsigned int charcode);
    
    /** Return true if this font provides vertical alignments and spacing or glyphs.*/
    virtual bool hasVertical() const;

    /** Set the margin around each glyph, 
      * to ensure that texture filtering doesn't bleed adjacent glyph's into each other.
      * Default margin is 2 texels.*/    
    void setGlyphImageMargin(unsigned int margin);
    unsigned int getGlyphImageMargin() const;

    /** Set the size of texture to create to store the glyph images when rendering.
      * Note, this doesn't affect already created crTexture Glhph's.*/
    void setTextureSizeHint(unsigned int width,unsigned int height);
    
    unsigned int getTextureWidthHint() const;
    unsigned int getTextureHeightHint() const;

    /** Set the minification texture filter to use when creating the texture to store the glyph images when rendering.
      * Note, this doesn't affect already created crTexture Glhph's.*/
    void setMinFilterHint(CRCore::crTexture::FilterMode mode);
    CRCore::crTexture::FilterMode getMinFilterHint() const;
    
    /** Set the magnification texture filter to use when creating the texture to store the glyph images when rendering.
      * Note, this doesn't affect already created crTexture Glhph's.*/
    void setMagFilterHint(CRCore::crTexture::FilterMode mode);
    CRCore::crTexture::FilterMode getMagFilterHint() const;

    // make crText a friend to allow it add and remove its entry in the crFont's _textList.
    friend class FontImplementation;

    void setImplementation(FontImplementation* implementation);

    FontImplementation* getImplementation();
    const FontImplementation* getImplementation() const;

	virtual void releaseObjects(CRCore::crState* state=0);
protected:

    virtual ~crFont();
    
    void addGlyph(unsigned int width, unsigned int height, unsigned int charcode, Glyph* glyph);
    
    typedef std::vector< CRCore::ref_ptr<GlyphTexture> >       GlyphTextureList;
    typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> >      StateSetList;
    typedef std::map< unsigned int, CRCore::ref_ptr<Glyph> >   GlyphMap;
    
    typedef std::pair< unsigned int, unsigned int >         SizePair;
    typedef std::map< SizePair, GlyphMap >                  SizeGlyphMap;
    
    SizeGlyphMap                    m_sizeGlyphMap;
    GlyphTextureList                m_glyphTextureList;
    StateSetList                    m_stateSetList;
    
    // current active size of font
    unsigned int                    m_width;
    unsigned int                    m_height;
    unsigned int                    m_margin;

    unsigned int                    m_textureWidthHint;
    unsigned int                    m_textureHeightHint;   
    CRCore::crTexture::FilterMode        m_minFilterHint;
    CRCore::crTexture::FilterMode        m_magFilterHint;
    
    //CRCore::ref_ptr<CRCore::crTexEnv >   m_texEnv;
    
    mutable CRCore::ref_ptr<FontImplementation>  m_implementation;

    
// declare the nested classes.
public:

    class FontImplementation : public CRCore::Referenced
    {
    public:
    
        virtual std::string getFileName() const = 0;

        /** Set the pixel width and height hint.*/
        virtual void setSize(unsigned int width, unsigned int height) = 0;

        /** Get a Glyph for specified charcode, and the font size nearest to the current font size hint.*/
        virtual Glyph* getGlyph(unsigned int charcode) = 0;

        /** Get a kerning (adjustment of spacing of two adjacent character) for specified charcodes, w.r.t the current font size hint.*/
        virtual CRCore::crVector2 getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType) = 0;

        /** Return true if this font provides vertical alignments and spacing or glyphs.*/
        virtual bool hasVertical() const = 0;
        
        void setWidth(unsigned int width) { m_facade->m_width = width; }

        void setHeight(unsigned int height) { m_facade->m_height = height; }
        
        void addGlyph(unsigned int width, unsigned int height, unsigned int charcode, Glyph* glyph)
        {
            m_facade->addGlyph(width, height, charcode, glyph);
        }
		virtual ~FontImplementation(){ m_facade = NULL; }
        crFont* m_facade;
    };

    //class oglGlyphTextureDraw;
	class CRGLTEXT_EXPORT GlyphTexture : public CRCore::crTexture2D
    {
	//	friend class oglGlyphTextureDraw;
    public:

        GlyphTexture();
        
        const char* className() const { return "GlyphTexture"; }

        /** return -1 if *this < *rhs, 0 if *this==*rhs, 1 if *this>*rhs.*/
        virtual int compare(const CRCore::crStateAttribute& rhs) const;
		virtual int compare(const CRCore::crStateAttribute& rhs);

        void setStateSet(CRCore::crStateSet* stateset) { m_stateset = stateset; }
        CRCore::crStateSet* getStateSet() { return m_stateset; }
        const CRCore::crStateSet* getStateSet() const { return m_stateset; }

        /** Set the margin around each glyph, to ensure that texture filtering doesn't bleed adjacent glyph's into each other.*/    
        void setGlyphImageMargin(unsigned int margin) { m_margin = margin; }
        unsigned int getGlyphImageMargin() const { return m_margin; }

        bool getSpaceForGlyph(Glyph* glyph, int& posX, int& posY);
        
        void addGlyph(Glyph* glyph,int posX, int posY);
        
        virtual void apply(CRCore::crState& state) const;
        //virtual void releaseObjects(crState* state);
    protected:
    
        virtual ~GlyphTexture();

        CRCore::crStateSet* m_stateset;
        
        // parameter used to compute the size and position of empty space
        // in the texture which could accomodate new glyphs.
        int m_margin;
        int m_usedY;
        int m_partUsedX;
        int m_partUsedY;

        typedef std::vector< CRCore::ref_ptr<Glyph> > GlyphRefList;
        typedef std::vector< const Glyph* > GlyphPtrList;
        typedef CRCore::buffered_object< GlyphPtrList > GlyphBuffer;
        
        GlyphRefList        m_glyphs;
        mutable GlyphBuffer m_glyphsToSubload;

    };

	//class CRGLTEXT_EXPORT oglGlyphTextureDraw : public CRRender::crOglTexture2DDraw
	//{
	//public:
	//	oglGlyphTextureDraw() { m_pgt = NULL; }

	//	META_SetAttribute(CRText, oglGlyphTextureDraw, CRCore::crStateAttribute::TEXTURE);

	//	virtual void drawImplementation(  const CRCore::crStateAttribute* pgt, CRCore::crState& state ) const;
 //       
	//	static oglGlyphTextureDraw *getInstance();

	//protected:
	//	void applyGlyphTexture( GlyphTexture* pgt ) const;

	//protected:
	//	mutable GlyphTexture* m_pgt;
	//};

    class CRGLTEXT_EXPORT Glyph : public CRCore::crImage
    {
    public:

        Glyph();
        virtual ~Glyph();

        unsigned int getGlyphCode() const;

        void setHorizontalBearing(const CRCore::crVector2& bearing);
        const CRCore::crVector2& getHorizontalBearing() const;

        void setHorizontalAdvance(float advance);
        float getHorizontalAdvance() const;

        void setVerticalBearing(const CRCore::crVector2& bearing);
        const CRCore::crVector2& getVerticalBearing() const;

        void setVerticalAdvance(float advance);
        float getVerticalAdvance() const;


        void setTexture(GlyphTexture* texture);
        GlyphTexture* getTexture();
        const GlyphTexture* getTexture() const;

        CRCore::crStateSet* getStateSet();
        const CRCore::crStateSet* getStateSet() const;

        void setTexturePosition(int posX,int posY);
        int getTexturePositionX() const;
        int getTexturePositionY() const;

        void setMinTexCoord(const CRCore::crVector2& coord);
        const CRCore::crVector2& getMinTexCoord() const;

        void setMaxTexCoord(const CRCore::crVector2& coord);
        const CRCore::crVector2& getMaxTexCoord() const;
        
        void subload() const;
        
        void draw(CRCore::crState& state) const;
		virtual void releaseObjects(CRCore::crState* state=0);
    protected:

        crFont*                       m_font;
        unsigned int                  m_glyphCode;

        CRCore::crVector2             m_horizontalBearing;
        float                         m_horizontalAdvance;

        CRCore::crVector2             m_verticalBearing;
        float                         m_verticalAdvance;

        GlyphTexture*                 m_texture;
        int                           m_texturePosX;
        int                           m_texturePosY;
        CRCore::crVector2             m_minTexCoord;
        CRCore::crVector2             m_maxTexCoord;
        
        typedef CRCore::buffered_value<unsigned int> GLObjectList;
        mutable GLObjectList          m_globjList;

    };


};

}


#endif
