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
#ifndef CRGLTEXT_CRGLTEXT_H
#define CRGLTEXT_CRGLTEXT_H 1

#include <CRCore/crDrawable.h>
#include <CRCore/crQuat.h>

#include <CRGLText/crFont.h>
#include <CRGLText/crString.h>
namespace CRGLText {

class crOglTextDraw;
class CRGLTEXT_EXPORT crText : public CRCore::crDrawable
{
	friend class crOglTextDraw;
public:

    crText();
    crText(const crText& text,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

    virtual CRCore::crBase* cloneType() const { return new crText(); }
    virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crText(*this,copyop); }
    virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crText*>(obj)!=NULL; }
    virtual const char* className() const { return "crText"; }
    virtual const char* libraryName() const { return "crText"; }


    /** Set the crFont to use to render the text.  
      * setFont(0) sets the use of the default font.*/
    void setFont(crFont* font=0);

    /** Set the font, loaded from the specified front file, to use to render the text,
      * setFont("") sets the use of the default font.*/
    void setFont(const std::string& fontfile);

    /** Get the font. Return 0 if default is being used.*/    
    const crFont* getFont() const { return m_font.get(); }

#ifdef USE_DEPRECATED_API

    /* deprecated */
    void setFontSize(unsigned int width, unsigned int height)
    {
        setFontResolution(width,height);
    }
#endif
    
    /** Set the crFont reference width and height resolution in texels.
      * Note, the size may not be supported by current font, 
      * the closest supported font size will be selected.*/
    void setFontResolution(unsigned int width, unsigned int height);

    unsigned int getFontWidth() const { return m_fontWidth; }
    unsigned int getFontHeight() const { return m_fontWidth; }
    
    
    /** Set the text using a crText::crString.*/
    void setText(const crString& text);

    /** Set the text using a std::string, 
      * which is converted to an internal TextString.*/
    virtual void setText(const std::string& text);

    /** Set the text using a Unicode encoded std::string, which is converted to an internal TextString.
      * The encoding parameter specificies which Unicode encodeding is used in the std::string. */
    void setText(const std::string& text,crString::Encoding encoding);

    /** Set the text using a wchar_t string, 
      * which is converted to an internal TextString.*/
    virtual void setText(const wchar_t* text);
    
    /** Get the text string. 
      * Note, if you modify the string you must call crText::update() for
      * the internal glyph reprentation to be updated.*/
    crString& getText() { return m_text; }

    /** Get the const text string.*/
    const crString& getText() const { return m_text; }
    
    /** update internal glyph respresnetation used for rendering, 
      * and bounding volume.*/
    //void update() { computeGlyphRepresentation(); }


    /** Set the rendered character size in base coordinates.*/
    void setCharacterSize(float height,float aspectRatio=1.0f);
    
    float getCharacterHeight() const { return m_characterHeight; }
    float getCharacterAspectRatio() const { return m_characterAspectRatio; }

    enum CharacterSizeMode
    {
        OBJECT_COORDS, /// default
        SCREEN_COORDS, /// internally scale the characters to be constant screen size.
        OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT /// text that behavaves like OBJECT_COORDS sized text when a long distance way, but has its screen sized capped automatically when the viewer gets near.
    };

    /** Set how the CharacterSize value relates to the final rendered character.*/
    void setCharacterSizeMode(CharacterSizeMode mode) { m_characterSizeMode = mode; }
    
    /** Get the CharacterSizeMode.*/
    CharacterSizeMode getCharacterSizeMode() const { return m_characterSizeMode; }


    /** Set the maximum width of the text box.
      * With horizontal layouts any characters which do not fit are wrapped around.
      * 0 or negative values indicate that no maximum width is set, lines can be as long as 
      * they need be to fit thre required text*/
    void setMaximumWidth(float maximumWidth);
    
    /** Get the maximim width of the text box.*/
    float getMaximumWidth() const { return m_maximumWidth; }

    /** Set the maximum height of the text box.
      * With horizontal layouts any characters which do not fit are wrapped around.
      * 0 or negative values indicate that no maximum height is set, lines can be as long as 
      * they need be to fit thre required text*/
    void setMaximumHeight(float maximumHeight);
    
    /** Get the maximum height of the text box.*/
    float getMaximumHeight() const { return m_maximumHeight; }



    /** Set the position of text.*/
    void setPosition(const CRCore::crVector3& pos);
    
    /** Get the position of text.*/
    const CRCore::crVector3& getPosition() const { return m_position; }
       

    enum AlignmentType
    {
        LEFT_TOP,
        LEFT_CENTER,
        LEFT_BOTTOM,

        CENTER_TOP,
        CENTER_CENTER,
        CENTER_BOTTOM,

        RIGHT_TOP,
        RIGHT_CENTER,
        RIGHT_BOTTOM,
        
        LEFT_BASE_LINE,
        CENTER_BASE_LINE,
        RIGHT_BASE_LINE,
        BASE_LINE = LEFT_BASE_LINE /// default.
    
    };
    
    void setAlignment(AlignmentType alignment);
    AlignmentType getAlignment() const { return m_alignment; }


    enum AxisAlignment
    {
        XY_PLANE,
        REVERSED_XY_PLANE,
        XZ_PLANE,
        REVERSED_XZ_PLANE,
        YZ_PLANE,
        REVERSED_YZ_PLANE,
        SCREEN
    };

    void setAxisAlignment(AxisAlignment axis);
    
    void setRotation(const CRCore::crQuat& quat);
    const CRCore::crQuat& getRotation() const { return m_rotation; }

    void setAutoRotateToScreen(bool autoRotateToScreen);
    bool getAutoRotateToScreen() const { return m_autoRotateToScreen; }

    enum Layout
    {
        LEFT_TO_RIGHT, /// default
        RIGHT_TO_LEFT,
        VERTICAL
    };
    
    void setLayout(Layout layout);
    
    Layout getLayout() const { return m_layout; }


    void setColor(const CRCore::crVector4& color);
    
    const CRCore::crVector4& getColor() const { return m_color; }


    enum DrawModeMask
    {
        TEXT        = 1, /// default
        BOUNDINGBOX = 2,
        ALIGNMENT   = 4
    };

    void setDrawMode(unsigned int mode);
    
    unsigned int getDrawMode() const { return m_drawMode; }

    void setKerningType(KerningType kerningType) { m_kerningType = kerningType; }

    KerningType getKerningType() const { return m_kerningType; }

    /** return false, crText::crText does not support accept(AttributeFunctor&).*/
    virtual bool supports(CRCore::crDrawable::AttributeFunctor&) const { return false; }

    /** return true, crText::crText does support accept(ConstAttributeFunctor&).*/
    virtual bool supports(CRCore::crDrawable::ConstAttributeFunctor&) const { return true; }

    /** accept an ConstAttributeFunctor and call its methods to tell it about the interal attributes that this crDrawable has.*/
    virtual void accept(CRCore::crDrawable::ConstAttributeFunctor& af) const;

    /** return true, crText::crText does support accept(PrimitiveFunctor&) .*/
    virtual bool supports(CRCore::crDrawable::PrimitiveFunctor&) const { return true; }

    /** accept a PrimtiveFunctor and call its methods to tell it about the interal primtives that this crDrawable has.*/
    virtual void accept(CRCore::crDrawable::PrimitiveFunctor& pf) const;


    // make crFont a friend to allow it set the m_font to 0 if the font is 
    // forcefully unloaded.
    friend class crFont;
    virtual void releaseObjects(CRCore::crState* state=0);
public:

    // internal structures, variable and methods used for rendering of characters.
    struct CRGLTEXT_EXPORT GlyphQuads
    {
        typedef std::vector<crFont::Glyph*> Glyphs;
        typedef std::vector<CRCore::crVector2> Coords2;
        typedef std::vector<CRCore::crVector3> Coords3;
        typedef std::vector<CRCore::crVector2> TexCoords;

        Glyphs                             m_glyphs;
        Coords2                            m_coords;
        CRCore::buffered_object<Coords3>   m_transformedCoords;
        TexCoords                          m_texcoords;

        Glyphs getGlyphs() { return m_glyphs; }
        const Glyphs getGlyphs() const { return m_glyphs; }
        

        Coords2& getCoords() { return m_coords; }
        const Coords2& getCoords() const { return m_coords; }

        Coords3& getTransformedCoords(unsigned int contexID) { return m_transformedCoords[contexID]; }
        const Coords3& getTransformedCoords(unsigned int contexID) const { return m_transformedCoords[contexID]; }

        TexCoords& getTexCoords() { return m_texcoords; }
        const TexCoords& getTexCoords() const { return m_texcoords; }
    };
    
    typedef std::map<CRCore::ref_ptr<CRCore::crStateSet>,GlyphQuads> TextureGlyphQuadMap;

    /** Direct Access to GlyphQuads */
    const GlyphQuads* getGlyphQuads(CRCore::crStateSet* stateSet) const 
    {
        TextureGlyphQuadMap::iterator itGlyphQuad = m_textureGlyphQuadMap.find(stateSet);
        if (itGlyphQuad == m_textureGlyphQuadMap.end()) return NULL;

        return &itGlyphQuad->second;
    }

    const TextureGlyphQuadMap& getTextureGlyphQuadMap() const 
    {
        return m_textureGlyphQuadMap;
    }

	inline virtual void swapBuffers()
	{
		//CRCore::notify(CRCore::WARN)<<"crText::swapBuffers"<<getName()<<std::endl;
		if(m_needReCompute>0)
		{
			dirtyBound();
			getBound();
			m_needReCompute = 0;
		}
		//switch (m_needReCompute)
		//{
		//case 1:
		//	computeGlyphRepresentation();
		//	this->getBound();
		//	break;
		//case 2:
		//	computePositions();
		//	this->getBound();
		//	break;
		//} 
	}
protected:
    virtual ~crText();
    int m_needReCompute;//1£ºcomputeGlyphRepresentation£¬2£ºcomputePositions
    OpenThreads::Mutex m_computeGlyphMutex;
    virtual bool computeBound() const;

    crFont* getActiveFont();
    const crFont* getActiveFont() const;

    crString::iterator computeLastCharacterOnLine(CRCore::crVector2 cursor, crString::iterator first,crString::iterator last);
    
    // members which have public access.
    CRCore::ref_ptr<crFont>                 m_font;
    unsigned int                            m_fontWidth;
    unsigned int                            m_fontHeight;
    float                                   m_characterHeight;
    float                                   m_characterAspectRatio;
    CharacterSizeMode                       m_characterSizeMode;
    float                                   m_maximumWidth;
    float                                   m_maximumHeight;

    crString                                m_text;

    CRCore::crVector3                       m_position;
    AlignmentType                           m_alignment;
    CRCore::crQuat                          m_rotation;
    bool                                    m_autoRotateToScreen;
    Layout                                  m_layout;
    CRCore::crVector4                       m_color;
    unsigned int                            m_drawMode;
    KerningType                             m_kerningType;

    // iternal map used for rendering. Set up by the computeGlyphRepresentation() method.
    mutable TextureGlyphQuadMap             m_textureGlyphQuadMap;

    void computeGlyphRepresentation();

    // internal caches of the positioning of the text.
    
    struct AutoTransformCache
    {    
        AutoTransformCache():
            m_traversalNumber(-1),
            m_width(0),
            m_height(0) {}
    
        int         m_traversalNumber;
        int         m_width;
        int         m_height;
        CRCore::crVector3   m_transformedPosition;
        CRCore::crMatrix    m_modelview;
        CRCore::crMatrix    m_projection;
        CRCore::crMatrix    m_matrix;
    };
    
    mutable CRCore::buffered_object<AutoTransformCache>    m_autoTransformCache;
    mutable CRCore::crVector3                              m_offset;
    mutable CRCore::crVector3                              m_normal;
    mutable CRCore::crBoundingBox                          m_textBB;

    void computePositions();
    void computePositions(unsigned int contextID) const;

};

class CRGLTEXT_EXPORT crOglTextDraw : public CRRender::crOglGeoRender
{
public:
	crOglTextDraw(){}
	
	static crOglTextDraw *getInstance();
	crOglTextDraw(const crOglTextDraw&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
	META_Base(CRGLText,crOglTextDraw);
protected:
	virtual void _drawImplementation(const CRCore::crDrawable*, CRCore::crState& state ) const;
};


}


#endif
