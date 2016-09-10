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
#ifndef CRTEXT_CRFONT
#define CRTEXT_CRFONT 1

#include <string>
#include <istream>

#include <CRCore/crTexEnv.h>
#include <CRText/crGlyph.h>
#include <CRIOManager/crReaderWriter.h>

#include <CRCore/thread/crMutex.h>

namespace CRText {

// forward declare crFont
class crFont;

/** Read a font from specified file. The filename may contain a path.
  * It will search for the font file in the following places in this order: 
  * - In the current directory
  * - All paths defined in OSG_FILE_PATH or OSGFILEPATH environment variable
  * - Filename with path stripped: In the current directory
  * - Filename with path stripped: All paths defined in OSG_FILE_PATH or OSGFILEPATH
  *
  * Then the file will be searched in OS specific directories in the following order:
  * - Again in the current directory
  * - Windows: In C:/winnt/fonts
  * - Windows: In C:/windows/fonts
  * - Windows: In the fonts directory of the windows install directory
  * - Other OS: In /usr/share/fonts/ttf
  * - Other OS: In /usr/share/fonts/ttf/western
  * - Other OS: In /usr/share/fonts/ttf/decoratives
  * 
  * If the given file could not be found, the path part will be stripped and
  * the file will be searched again in the OS specific directories.
  */
extern CRTEXT_EXPORT void readFontFile(CRCore::ref_ptr<crFont> &outFont, const std::string& filename, const CRIOManager::crReaderWriter::Options* userOptions = 0);

/** read a font from specified stream.*/
//extern CRTEXT_EXPORT crFont* readFontStream(std::istream& stream, const CRIOManager::crReaderWriter::Options* userOptions = 0);

//extern CRTEXT_EXPORT CRCore::ref_ptr<crFont> readRefFontFile(const std::string& filename, const CRIOManager::Options* userOptions = 0);
//
//extern CRTEXT_EXPORT CRCore::ref_ptr<crFont> readRefFontStream(std::istream& stream, const CRIOManager::Options* userOptions = 0);

extern CRTEXT_EXPORT std::string findFontFile(const std::string& str);

/** Pure virtual base class for fonts.
  * Concrete implementation are the crDefaultFont found in src/CRText/crDefaultFont.cpp
  * and FreeTypeFont found in src/osgPlugins/freetype/FreeTypeFont.cpp*/
class CRTEXT_EXPORT crFont : public CRCore::crBase
{
// declare the interface to a font.
public:

    // forward declare nested classes.
    class FontImplementation;

public:
    crFont(FontImplementation* implementation=0);

    virtual CRCore::crBase* cloneType() const { return 0; } // cloneType() not appropriate
    virtual CRCore::crBase* clone(const CRCore::crCopyOp&) const { return 0; } // clone() not appropriate
    virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crFont*>(obj)!=NULL; }
    virtual const char* className() const { return "crFont"; }
    virtual const char* libraryName() const { return "CRText"; }

    virtual std::string getFileName() const;

    //static CRCore::ref_ptr<crFont>& getDefaultFont();

    //void setTexEnv(CRCore::crTexEnv* texenv) { if (texenv) m_texenv = texenv; }
    //inline CRCore::crTexEnv* getTexEnv() {  return m_texenv.get(); }
    //inline const CRCore::crTexEnv* getTexEnv() const {  return m_texenv.get(); }

    //void setStateSet(CRCore::crStateSet* stateset) { m_stateset = stateset; }
    //CRCore::crStateSet* getStateSet() { return m_stateset.get(); }
    //const CRCore::crStateSet* getStateSet() const { return m_stateset.get(); }

    /** Get a kerning (adjustment of spacing of two adjacent character) for specified charcodes, w.r.t the current font size hint.*/
    virtual CRCore::crVector2 getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType);

    /** Get a crGlyph for specified charcode, and the font size nearest to the current font size hint.*/
    virtual crGlyph* getGlyph(const FontResolution& fontSize, unsigned int charcode);


    /** Get a crGlyph3D for specified charcode.*/
    virtual crGlyph3D* getGlyph3D(unsigned int charcode);

    /** Return true if this font provides vertical alignments and spacing or glyphs.*/
    virtual bool hasVertical() const;

    /** Set the margin around each glyph, 
      * to ensure that texture filtering doesn't bleed adjacent glyph's into each other.
      * Default margin is 1 texels.*/    
    void setGlyphImageMargin(unsigned int margin);
    unsigned int getGlyphImageMargin() const;

    /** Set the margin ratio around each glyph, relative to the glyph's size.
      * to ensure that texture filtering doesn't bleed adjacent glyph's into each other.
      * Default margin is 0.05.*/    
    void setGlyphImageMarginRatio(float margin);
    float getGlyphImageMarginRatio() const;


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

    unsigned int getFontDepth() const { return m_depth; }

    void setNumberCurveSamples(unsigned int numSamples) { m_numCurveSamples = numSamples; }
    unsigned int getNumberCurveSamples() const { return m_numCurveSamples; }


    // make crText a friend to allow it add and remove its entry in the crFont's _textList.
    friend class FontImplementation;

    void setImplementation(FontImplementation* implementation);

    FontImplementation* getImplementation();
    const FontImplementation* getImplementation() const;

    /** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
    //virtual void setThreadSafeRefUnref(bool threadSafe);

    /** Resize any per context GLObject buffers to specified size. */
    //virtual void resizeGLObjectBuffers(unsigned int maxSize);

     /** If crState is non-zero, this function releases OpenGL objects for
       * the specified graphics context. Otherwise, releases OpenGL objexts
       * for all graphics contexts. */
    virtual void releaseObjects(CRCore::crState* state=0);

    typedef CRCore::crMutex FontMutex;
    
    typedef std::vector< CRCore::ref_ptr<crGlyphTexture> >       GlyphTextureList;
    GlyphTextureList& getGlyphTextureList() { return m_glyphTextureList; }
    
protected:

    virtual ~crFont();

    void addGlyph(const FontResolution& fontRes, unsigned int charcode, crGlyph* glyph);

    typedef std::vector< CRCore::ref_ptr<CRCore::crStateSet> >      StateSetList;
    typedef std::map< unsigned int, CRCore::ref_ptr<crGlyph> >   GlyphMap;
    typedef std::map< unsigned int, CRCore::ref_ptr<crGlyph3D> >  Glyph3DMap;

    typedef std::map< FontResolution, GlyphMap >            FontSizeGlyphMap;

    mutable CRCore::crCriticalMutex      m_glyphMapMutex;

    //CRCore::ref_ptr<CRCore::crTexEnv>       m_texenv;
    //CRCore::ref_ptr<CRCore::crStateSet>     m_stateset;
    FontSizeGlyphMap                m_sizeGlyphMap;
    GlyphTextureList                m_glyphTextureList;


    Glyph3DMap                      m_glyph3DMap;

    // current active size of font
    FontResolution                  m_fontSize;
    unsigned int                    m_margin;
    float                           m_marginRatio;

    unsigned int                    m_textureWidthHint;
    unsigned int                    m_textureHeightHint;   
    CRCore::crTexture::FilterMode        m_minFilterHint;
    CRCore::crTexture::FilterMode        m_magFilterHint;

    unsigned int                    m_depth;
    unsigned int                    m_numCurveSamples;

    
    mutable CRCore::ref_ptr<FontImplementation> m_implementation;

    
// declare the nested classes.
public:

    class FontImplementation : public CRCore::Referenced
    {
    public:
    
        FontImplementation():
            CRCore::Referenced(),
            m_facade(0) {}
    
        virtual std::string getFileName() const = 0;

        virtual bool supportsMultipleFontResolutions() const = 0;

        /** Get a crGlyph for specified charcode, and the font size nearest to the current font size hint.*/
        virtual crGlyph* getGlyph(const FontResolution& fontRes, unsigned int charcode) = 0;

        /** Get a crGlyph3D for specified charcode.*/
        virtual crGlyph3D* getGlyph3D(unsigned int charcode) = 0;

        /** Get a kerning (adjustment of spacing of two adjacent character) for specified charcodes, w.r.t the current font size hint.*/
        virtual CRCore::crVector2 getKerning(unsigned int leftcharcode,unsigned int rightcharcode, KerningType kerningType) = 0;

        /** Return true if this font provides vertical alignments and spacing or glyphs.*/
        virtual bool hasVertical() const = 0;

        void addGlyph(const FontResolution& fontRes, unsigned int charcode, crGlyph* glyph)
        {
            m_facade->addGlyph(fontRes, charcode, glyph);
        }

        crFont* m_facade;
	protected:
		virtual ~FontImplementation(){}
    };



};

}

#endif
