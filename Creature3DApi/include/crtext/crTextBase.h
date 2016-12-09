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

#ifndef CRTEXT_TEXTBASE
#define CRTEXT_TEXTBASE 1

#include <CRCore/crDrawable.h>

#include <CRText/crString.h>
#include <CRText/crKerningType.h>
#include <CRText/crFont.h>

namespace CRText {


class CRTEXT_EXPORT crTextBase : public CRCore::crDrawable
{
public:

    crTextBase();
    crTextBase(const crTextBase& text,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

    //virtual CRCore::crBase* cloneType() const { return new crText(); }
    //virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crText(*this,copyop); }
    virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crTextBase*>(obj)!=NULL; }
    virtual const char* className() const { return "crTextBase"; }
    virtual const char* libraryName() const { return "CRText"; }

    //void setColor(const CRCore::crVector4& color);
    //const CRCore::crVector4& getColor() const { return m_color; }

    /** Set the crFont to use to render the text.
      * setFont(0) sets the use of the default font.*/
    //virtual void setFont(crFont* font=0) { setFont(CRCore::ref_ptr<crFont>(font)); };

    /** Set the font, loaded from the specified front file, to use to render the text,
      * setFont("") sets the use of the default font.
      * See the CRText::readFontFile function for how the font file will be located. */
    void setFont(const std::string& fontfile);

    /** Get the font. Return 0 if default is being used.*/
    const crFont* getFont() const { return m_font.get(); }


    /** Set the text style.*/
    void setStyle(crStyle* style) { m_style = style; }
    /** Get the text style.*/
    crStyle* getStyle() { return m_style.get(); }
    /** Get the const text style.*/
    const crStyle* getStyle() const { return m_style.get(); }

    /** Get or create the text style.*/
    crStyle* getOrCreateStyle() { if (!m_style) m_style = new crStyle; return m_style.get(); }

    /** Set the crFont reference width and height resolution in texels.
      * Note, the size may not be supported by current font, 
      * the closest supported font size will be selected.*/
    void setFontResolution(unsigned int width, unsigned int height);

    unsigned int getFontWidth() const { return m_fontSize.first; }
    unsigned int getFontHeight() const { return m_fontSize.second; }
    
    
    /** Set the text using a CRText::crString.*/
    void setText(const crString& text);

    /** Set the text using a std::string, 
      * which is converted to an internal TextString.*/
    void setText(const std::string& text);

    /** Set the text using a Unicode encoded std::string, which is converted to an internal TextString.
      * The encoding parameter specificies which Unicode encodeding is used in the std::string. */
    void setText(const std::string& text,crString::Encoding encoding);

    /** Set the text using a wchar_t string, 
      * which is converted to an internal TextString.*/
    void setText(const wchar_t* text);
    
    /** Get the text string. 
      * Note, if you modify the string you must call crText::update() for
      * the internal glyph reprentation to be updated.*/
    crString& getText() { return m_text; }

    /** Get the const text string.*/
    const crString& getText() const { return m_text; }
    
    /** update internal glyph respresentation used for rendering, 
      * and bounding volume.*/
    //void update() { computeGlyphRepresentation(); }


    /** Set the rendered character size in object coordinates.*/
    void setCharacterSize(float height);

    /** Set the rendered character size in object coordinates.*/
    void setCharacterSize(float height, float aspectRatio);

    float getCharacterHeight() const { return m_characterHeight; }
    float getCharacterAspectRatio() const { return m_style.valid()? m_style->getWidthRatio() : 1.0f; }
	float getCharacterWidth() const { return m_characterHeight/getCharacterAspectRatio(); }

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
      * they need be to fit the required text*/
    void setMaximumHeight(float maximumHeight);
    
    /** Get the maximum height of the text box.*/
    float getMaximumHeight() const { return m_maximumHeight; }

    /** Set the line spacing of the text box, given as a percentage of
      * the character height. The default value is 0 for backward 
      * compatibility. For longer paragraphs of text, a value of at 
      * least 25% (i.e. set line spacing to 0.25) is recommended. */
    void setLineSpacing(float lineSpacing);
	void setUnderLineSpacing(float underLineSpacing);

    /** Get the line spacing of the text box. */
    float getLineSpacing() const { return m_lineSpacing; }

	float getLineHeight() const { return m_characterHeight + m_lineSpacing; }

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
    
        LEFT_BOTTOM_BASE_LINE,
        CENTER_BOTTOM_BASE_LINE,
        RIGHT_BOTTOM_BASE_LINE,
    
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
        SCREEN,
        USER_DEFINED_ROTATION
    };

    void setAxisAlignment(AxisAlignment axis);
    AxisAlignment getAxisAlignment() const { return m_axisAlignment; }
    
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


    enum DrawModeMask
    {
        TEXT              = 1, /// default
        BOUNDINGBOX       = 2,
        FILLEDBOUNDINGBOX = 4,
        ALIGNMENT         = 8,
		UNDERLINE         = 16,
    };

    void setDrawMode(unsigned int mode);
    
    unsigned int getDrawMode() const { return m_drawMode; }

    void setBoundingBoxMargin(float margin);
    
    float getBoundingBoxMargin() const { return m_textBBMargin; }

    void setBoundingBoxColor(const CRCore::crVector4& color){ m_textBBColor = color; }
    
    const CRCore::crVector4& getBoundingBoxColor() const { return m_textBBColor; }

    
    void setKerningType(KerningType kerningType) { m_kerningType = kerningType; }

    KerningType getKerningType() const { return m_kerningType; }

    /** Get the number of wrapped lines - only valid after computeGlyphRepresentation() has been called, returns 0 otherwise */
    unsigned int getLineCount() const { return m_lineCount; }

    ///** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
    //virtual void setThreadSafeRefUnref(bool threadSafe);

    ///** Resize any per context GLObject buffers to specified size. */
    //virtual void resizeGLObjectBuffers(unsigned int maxSize);

    /** If crState is non-zero, this function releases OpenGL objects for
      * the specified graphics context. Otherwise, releases OpenGL objexts
      * for all graphics contexts. */
    virtual void releaseObjects(CRCore::crState* state=0);

	void computePositions();
	virtual bool getCursorPosition(int index,CRCore::crVector2 &outPos){return false;}
	virtual int getCursorIndex(const CRCore::crVector2& cursorPos){return 0;}
	inline virtual void swapBuffers(int frameNumber=0)
	{//该函数由渲染系统内部调用，外部调用有风险，只有在LOADING时刻，为获得文本正确的BoundBox才可以调用
		//CRCore::notify(CRCore::WARN)<<"crText::swapBuffers"<<getName()<<std::endl;
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
		switch (m_needReCompute)
		{
		case 1:
			{
				bool needreadfont = true;
				if (m_font.valid())
				{
					std::string fontname = m_font->getName();
					if (m_fontName.compare(fontname) == 0)
						needreadfont = false;
				}
				if (needreadfont)
				{
					CRCore::ref_ptr<crFont> font;
					readFontFile(font, m_fontName);
					setFont(font.get());
				}
				computeGlyphRepresentation();
				m_needReCompute = 0;
				//if(frameNumber > 0)
				//{
				//	m_needReCompute = 0;
				//	m_autoTransformCache_buf = m_autoTransformCache;
				//	m_offset_buf = m_offset;
				//	m_normal_buf = m_normal;
				//	m_textBB_buf = m_textBB;
				//}
				//else
				//{
				//	m_needReCompute = 3;
				//}
			}
			break;
		case 2:
			{
				computePositions();
				m_needReCompute = 0;
				//if(frameNumber > 0)
				//{
				//	m_needReCompute = 0;
				//	m_autoTransformCache_buf = m_autoTransformCache;
				//	m_offset_buf = m_offset;
				//	m_normal_buf = m_normal;
				//	m_textBB_buf = m_textBB;
				//}
				//else
				//{
				//	m_needReCompute = 3;
				//}
			}
			break;
		//case 3:
		//	if(frameNumber > 0)
		//	{
		//		m_needReCompute = 0;
		//		m_autoTransformCache_buf = m_autoTransformCache;
		//		m_offset_buf = m_offset;
		//		m_normal_buf = m_normal;
		//		m_textBB_buf = m_textBB;
		//	}
		//	break;
		}
	}
	void setStartCoord(const CRCore::crVector2& coord);
	const CRCore::crVector2& getEndCoord();
protected:
    virtual ~crTextBase();
	void setFont(crFont *font);
	char m_needReCompute;//1：computeGlyphRepresentation，2：computePositions
	mutable CRCore::crCriticalMutex m_computeGlyphMutex;
	virtual bool computeBound() const;

    void positionCursor(const CRCore::crVector2 & endOfLine_coords, CRCore::crVector2 & cursor, unsigned int linelength);
    crString::iterator computeLastCharacterOnLine(CRCore::crVector2& cursor, crString::iterator first,crString::iterator last);


    virtual void computePositions(unsigned int contextID) const = 0;
    virtual void computeGlyphRepresentation() = 0;
    
    
    // members which have public access.
    //CRCore::crVector4                               m_color;
	std::string                                     m_fontName;
	CRCore::ref_ptr<CRText::crFont>                 m_font;
    CRCore::ref_ptr<crStyle>                     m_style;
    FontResolution                          m_fontSize;
    float                                   m_characterHeight;
    CharacterSizeMode                       m_characterSizeMode;
    float                                   m_maximumWidth;
    float                                   m_maximumHeight;
    float                                   m_lineSpacing;
	float                                   m_underLineSpacing;

    crString                                  m_text;
    CRCore::crVector3                               m_position;
    AlignmentType                           m_alignment;
    AxisAlignment                           m_axisAlignment;
    CRCore::crQuat                               m_rotation;
    bool                                    m_autoRotateToScreen;
    Layout                                  m_layout;
    unsigned int                            m_drawMode;
    float                                   m_textBBMargin;
    CRCore::crVector4                               m_textBBColor;
    KerningType                             m_kerningType;
    unsigned int                            m_lineCount;
	CRCore::crVector2                       m_startCoord;
    CRCore::crVector2                       m_endCoord;

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
        CRCore::crMatrix m_modelview;
        CRCore::crMatrix m_projection;
        CRCore::crMatrix m_matrix;
    };
    
    mutable CRCore::buffered_object<AutoTransformCache>    m_autoTransformCache;
	//mutable CRCore::buffered_object<AutoTransformCache>    m_autoTransformCache_buf;
    mutable CRCore::crVector3                                   m_offset;
	//mutable CRCore::crVector3                                   m_offset_buf;
    mutable CRCore::crVector3                                   m_normal;
	//mutable CRCore::crVector3                                   m_normal_buf;
    mutable CRCore::crBoundingBox                            m_textBB;
	//mutable CRCore::crBoundingBox                            m_textBB_buf;
};

}


#endif
 
