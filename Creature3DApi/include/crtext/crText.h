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
#ifndef CRTEXT_CRTEXT
#define CRTEXT_CRTEXT 1


#include <CRCore/crDrawable.h>
#include <CRCore/crQuat.h>

#include <CRText/crTextBase.h>
#include <CRText/crFont.h>

namespace CRText {


class CRTEXT_EXPORT crText : public CRText::crTextBase
{
public:

    crText();
    crText(const crText& text,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

    virtual CRCore::crBase* cloneType() const { return new crText(); }
    virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return new crText(*this,copyop); }
    virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crText*>(obj)!=NULL; }
    virtual const char* className() const { return "crText"; }
    virtual const char* libraryName() const { return "CRText"; }

    //virtual void setFont(crFont* font=0) { setFont(CRCore::ref_ptr<crFont>(font)); };

    /** Set the crFont to use to render the text.*/
    //virtual void setFont(crFont *font);

    /** Set the font, loaded from the specified front file, to use to render the text,
      * setFont("") sets the use of the default font.
      * See the CRText::readFontFile function for how the font file will be located. */
    //virtual void setFont(const std::string& fontfile) { crTextBase::setFont(fontfile); }

    /**
     * Turns off writing to the depth buffer when rendering text. This only affects text
     * with no backdrop or text using the DELAYED_DEPTH_WRITES implementation, since
     * the other backdrop implementations are really only useful for backwards
     * compatibility and are not worth updating to utilize this flag.
     */
    void setEnableDepthWrites(bool enable) { m_enableDepthWrites = enable; }
    bool getEnableDepthWrites() const { return m_enableDepthWrites; }


    enum BackdropType
    {
        DROP_SHADOW_BOTTOM_RIGHT = 0,  // usually the type of shadow you see
        DROP_SHADOW_CENTER_RIGHT,
        DROP_SHADOW_TOP_RIGHT,
        DROP_SHADOW_BOTTOM_CENTER,
        DROP_SHADOW_TOP_CENTER,
        DROP_SHADOW_BOTTOM_LEFT,
        DROP_SHADOW_CENTER_LEFT,
        DROP_SHADOW_TOP_LEFT,
        OUTLINE,
        NONE
    };

    enum BackdropImplementation
    {
        /* POLYGON_OFFSET: 
         * This uses glPolygonOffset to draw the text multiple times to 
         * create the drop-shadow and outline effects. glPolygonOffset 
         * is used to prevent z-fighting of the overlapping text.
         * This probably should have been the best option, but all the ATI
         * cards we have encountered so far have serious problems with this.
         * We see little white holes/artifacts in the rendered glyph textures
         * which move around depending on the viewing angle. For moving text,
         * the moving holes give an extremely unpleasant flickering effect.
         * Pumping up the "units" parameter in glPolygonOffset can minimize 
         * this problem, but two other bad side-effects occur if you do this.
         * First, high values will cause problems with clipping, particularly
         * when there are objects behind the text. The drop-shadows or outline
         * may be culled because their computed offset is behind the object or 
         * z-far plane. Second, there is an additional problem associated with
         * the Z-slope. High values can make large chunks of the backdrop
         * suddenly disappear. This can be reduced by the "factor" parameter.
         * Making the "factor" value small, can help, but experimentally, we've
         * found that it creates a new, different kind of z-fighting problem.
         * So there is no perfect solution. With units, you trade off the 'holes'
         * for the large-section clipping.
         * Experimentally, we have found units values from 150-512 to be tolerable
         * to acceptable with respect to the 'holes'. A factor of .1 seems to 
         * bring down the large clipping problem without creating a new z-fighting
         * problem.
         * (You can experiment with these numbers by playing with the 
         * CRCore:crPolygonOffset multipliers which this backend tries to respect.)
         *
         * If ATI ever fixes their cards/drivers, then this might become the 
         * best option.*/
        POLYGON_OFFSET = 0,
        
        /* NO_DEPTH_BUFFER
         * Instead of using glPolygonOffset to prevent z-fighting, this mode
         * just disables the depth buffer when rendering the text. This allows
         * the text to be rendered without any z-fighting. The downside to this 
         * mode is that render order begins to matter and the text will not 
         * necessarily correctly appear above or behind other objects in the 
         * scene based on depth values.
         * This mode is best for text that only needs to be ontop and 
         * not obscured by any objects.*/
        NO_DEPTH_BUFFER,

        /* DEPTH_RANGE
         * This mode is inspired by Paul Martz's OpenGL FAQ, item 13.050.
         * This uses glDepthRange as a substitute for glPolygonOffset.
         * Strangely, experiments on ATI cards seem to produce cleaner results
         * than when using glPolygonOffset. The trade-off for this is that the 
         * backdrop still may be placed too far back and might be culled by objects
         * directly behind the object or by the far z-plane. If ATI ever fixes
         * the glPolygonOffset problem, polygon offset is probably a slightly 
         * better solution because you can use smaller offsets. But with the 
         * current ATI problem, this option may be preferable.*/
        DEPTH_RANGE,
        /* STENCIL_BUFFER
         * (Assuming the backend is written correctly,) the Stencil Buffer is
         * the most "correct" and reliable way of producing backdrop text.
         * The stencil buffer is a multipass system that allows writing to the 
         * same z-values without needing to resort to offsets. This implementation
         * should not have any of the problems associated with the 3 previous
         * implementations. But the trade-off for this mode is that without 
         * hardware acceleration for the stencil buffer, rendering will be 
         * extremely slow. (There is also potentially more overhead for this 
         * algorithm so it could be slower than the other implementations. 
         * Benchmarking would be required to determine if the speed differences
         * are significant on your particular hardware.) This mode is best for 
         * when quality is important and stencil buffer hardware acceleration 
         * is available.*/
        STENCIL_BUFFER,

        /* DELAYED_DEPTH_WRITES
         * This mode renders all text with depth writes turned off, then
         * again with depth writes on, but with the color buffer disabled.
         * This should render text accurately for all graphics cards. The
         * only downside is the additional pass to render to the depth
         * buffer. But if you don't need the depth buffer updated for
         * your, this extra pass can be disabled by calling
         * enableDepthWrites(false).*/
        DELAYED_DEPTH_WRITES
    };

    /**
     * BackdropType gives you a background shadow text behind your regular 
     * text. This helps give text extra contrast which can be useful when 
     * placing text against noisy backgrounds. 
     * The color of the background shadow text is specified by setBackdropColor().
     * DROP_SHADOW_BOTTOM_RIGHT will draw backdrop text to the right and down of 
     * the normal text. Other DROW_SHADOW_* modes do the same for their repective directions.
     * OUTLINE will draw backdrop text so that it appears the text has an outline
     * or border around the normal text. This mode is particularly useful against
     * really noisy backgrounds that may put text on top of things that have 
     * all types of colors which you don't have control over. 
     * Some real world examples of this general technique in use that I know of 
     * are Google Earth, Sid Meier's Pirates (2004 Remake), and Star Control 2 (PC 1993).
     * The default is NONE.
     */
    void setBackdropType(BackdropType type);

    BackdropType getBackdropType() const { return m_backdropType; }

    /** 
     * Sets the amount text is offset to create the backdrop/shadow effect. 
     * Set the value too high and for example, in OUTLINE mode you will get a "Brady Bunch"
     * effect where you see duplicates of the text in a 3x3 grid.
     * Set the value too small and you won't see anything.
     * The values represent percentages. 1.0 means 100% so a value of 1.0
     * in DROW_SHADOW_LEFT_CENTER mode would cause each glyph to be echoed
     * next to it self. So the letter 'e' might look like 'ee'.
     * Good values tend to be in the 0.03 to 0.10 range (but will be subject
     * to your specific font and display characteristics).
     * Note that the text bounding boxes are updated to include backdrop offsets.
     * However, other metric information such as getCharacterHeight() are unaffected
     * by this. This means that individual glyph spacing (kerning?) are unchanged 
     * even when this mode is used.
     * The default is 0.07 (7% offset).
     */
    void setBackdropOffset(float offset = 0.07f);
    /**
     * This overloaded version lets you specify the offset for the horizontal
     * and vertical components separately.
     */
    void setBackdropOffset(float horizontal, float vertical);

    float getBackdropHorizontalOffset() const { return m_backdropHorizontalOffset; }

    float getBackdropVerticalOffset() const { return m_backdropVerticalOffset; }

    /**
     * This specifies the color of the backdrop text.
     * The default is black.
     */
    void setBackdropColor(const CRCore::crVector4& color);

    const CRCore::crVector4& getBackdropColor() const { return m_backdropColor; }

    /**
     * This specifies the underlying backdrop rendering implementation.
     * Unfortunately, at this time, there is no "perfect" rendering solution
     * so this function is provided to let you 'pick your poison'. Each
     * implementation has trade-offs. See BackdropImplementation enum
     * docs for details.*/ 
     void setBackdropImplementation(BackdropImplementation implementation);

     BackdropImplementation getBackdropImplementation() const { return m_backdropImplementation; }



    enum ColorGradientMode
    {
        SOLID = 0, // a.k.a. ColorGradients off
        PER_CHARACTER,
        OVERALL
    };

    /**
     * This sets different types of text coloring modes.
     * When the coloring mode is not set to SOLID, the 
     * colors specified in setColorGradientCorners() determine
     * the colors for the text.
     * When the gradient mode is OVERALL, the coloring scheme
     * attempts to approximate the effect as if the entire text box/region
     * were a single polygon and you had applied colors to each of the four
     * corners with GL_SMOOTH enabled. In this mode, OpenGL interpolates
     * the colors across the polygon, and this is what OVERALL tries to 
     * emulate. This can be used to give nice embellishments on things 
     * like logos and names.
     * PER_CHARACTER is similar to OVERALL except that it applies the 
     * color interpolation to the four corners of each character instead
     * of across the overall text box.
     * The default is SOLID (a.k.a. off).
     */
    void setColorGradientMode(ColorGradientMode mode);

    ColorGradientMode getColorGradientMode() const { return m_colorGradientMode; }

    /**
     * Used only for gradient mode, let's you specify the colors of the 4 corners.
     * If ColorGradients are off, these values are ignored (and the value from setColor() 
     * is the only one that is relevant.
     */
    void setColorGradientCorners(const CRCore::crVector4& topLeft, const CRCore::crVector4& bottomLeft, const CRCore::crVector4& bottomRight, const CRCore::crVector4& topRight);

    const CRCore::crVector4& getColorGradientTopLeft() const { return m_colorGradientTopLeft; }
    const CRCore::crVector4& getColorGradientBottomLeft() const { return m_colorGradientBottomLeft; }
    const CRCore::crVector4& getColorGradientBottomRight() const { return m_colorGradientBottomRight; }
    const CRCore::crVector4& getColorGradientTopRight() const { return m_colorGradientTopRight; }


    
    /** Draw the text.*/
    virtual void drawImplementation(CRCore::crState& state) const;

    /** return false, CRText::crText does not support accept(AttributeFunctor&).*/
    virtual bool supports(const CRCore::crDrawable::AttributeFunctor&) const { return false; }

    /** return true, CRText::crText does support accept(ConstAttributeFunctor&).*/
    virtual bool supports(const CRCore::crDrawable::ConstAttributeFunctor&) const { return true; }

    /** accept an ConstAttributeFunctor and call its methods to tell it about the internal attributes that this crDrawable has.*/
    virtual void accept(CRCore::crDrawable::ConstAttributeFunctor& af) const;

    /** return true, CRText::crText does support accept(PrimitiveFunctor&) .*/
    virtual bool supports(const CRCore::crDrawable::PrimitiveFunctor&) const { return true; }

    /** accept a PrimtiveFunctor and call its methods to tell it about the internal primitives that this crDrawable has.*/
	virtual void accept(CRCore::crDrawable::PrimitiveFunctor& pf) const;


    ///** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
    //virtual void setThreadSafeRefUnref(bool threadSafe);

    ///** Resize any per context GLObject buffers to specified size. */
    //virtual void resizeGLObjectBuffers(unsigned int maxSize);

    /** If crState is non-zero, this function releases OpenGL objects for
      * the specified graphics context. Otherwise, releases OpenGL objexts
      * for all graphics contexts. */
    virtual void releaseObjects(CRCore::crState* state=0);
	virtual bool getCursorPosition(int index,CRCore::crVector2 &outPos);
	virtual int getCursorIndex(const CRCore::crVector2& cursorPos);
	bool hitTest(const CRCore::crVector2& cursorPos);
	const CRCore::Vec2Array &getUnderLineCoords() const;
	int getUnderLineCoordSize()const;
	const CRCore::crVector2& getUnderLineCoord(int index)const;
	//inline virtual void swapBuffers(int frameNumber=0)
	//{//该函数由渲染系统内部调用，外部调用有风险，只有在LOADING时刻，为获得文本正确的BoundBox才可以调用
	//	//CRCore::notify(CRCore::WARN)<<"crText::swapBuffers"<<getName()<<std::endl;
	//	int swapflg = frameNumber > 0?m_needReCompute:0;
	//	crTextBase::swapBuffers(frameNumber);
	//	if(swapflg > 0)
	//	{
	//		//m_textureGlyphQuadMap_buf = m_textureGlyphQuadMap;
	//		m_underLineTCoords_buf = m_underLineTCoords;
	//	}
	//}
public:

    // internal structures, variable and methods used for rendering of characters.
    struct CRTEXT_EXPORT GlyphQuads
    {
        typedef std::vector<crGlyph*> Glyphs;
        typedef std::vector<unsigned int> LineNumbers;
        typedef std::vector<CRCore::crVector2> Coords2;
        typedef std::vector<CRCore::crVector3> Coords3;
        typedef std::vector<CRCore::crVector2> TexCoords;
        typedef std::vector<CRCore::crVector4> ColorCoords;

        Glyphs                          m_glyphs;
        Coords2                         m_coords;
        CRCore::buffered_object<Coords3>   m_transformedCoords;
        TexCoords                       m_texcoords;
        LineNumbers                     m_lineNumbers;

        CRCore::buffered_object<Coords3>   m_transformedBackdropCoords[8];
        ColorCoords    m_colorCoords;

        Glyphs getGlyphs() { return m_glyphs; }
        const Glyphs getGlyphs() const { return m_glyphs; }

        Coords2& getCoords() { return m_coords; }
        const Coords2& getCoords() const { return m_coords; }

        Coords3& getTransformedCoords(unsigned int contexID) { return m_transformedCoords[contexID]; }
        const Coords3& getTransformedCoords(unsigned int contexID) const { return m_transformedCoords[contexID]; }

        TexCoords& getTexCoords() { return m_texcoords; }
        const TexCoords& getTexCoords() const { return m_texcoords; }

        LineNumbers& getLineNumbers() { return m_lineNumbers; }
        const LineNumbers& getLineNumbers() const { return m_lineNumbers; }
    };

    typedef std::map<CRCore::ref_ptr<crGlyphTexture>,GlyphQuads> TextureGlyphQuadMap;

    /** Direct Access to GlyphQuads */
    const GlyphQuads* getGlyphQuads(crGlyphTexture* texture) const
    {
        TextureGlyphQuadMap::iterator itGlyphQuad = m_textureGlyphQuadMap.find(texture);
        if (itGlyphQuad == m_textureGlyphQuadMap.end()) return NULL;

        return &itGlyphQuad->second;
    }

    const TextureGlyphQuadMap& getTextureGlyphQuadMap() const 
    {
        return m_textureGlyphQuadMap;
    }

	
protected:

    //virtual ~crText();

    crFont* getActiveFont();
    const crFont* getActiveFont() const;

    crString::iterator computeLastCharacterOnLine(CRCore::crVector2& cursor, crString::iterator first,crString::iterator last);
    
    // members which have public access.

    // iternal map used for rendering. Set up by the computeGlyphRepresentation() method.
    mutable TextureGlyphQuadMap            m_textureGlyphQuadMap;
    //mutable TextureGlyphQuadMap            m_textureGlyphQuadMap_buf;

    void computeGlyphRepresentation();

    // internal caches of the positioning of the text.
    
    bool computeAverageGlyphWidthAndHeight(float& avg_width, float& avg_height) const;
    
    virtual void computePositions(unsigned int contextID) const;
    
    void computeBackdropPositions(unsigned int contextID) const;
    void computeBackdropBoundingBox() const;
    void computeBoundingBoxMargin() const;

    void computeColorGradients() const;
    void computeColorGradientsOverall() const;
    void computeColorGradientsPerCharacter() const;

    void drawForegroundText(CRCore::crState& state, const GlyphQuads& glyphquad) const;
    void drawTextWithBackdrop(CRCore::crState& state) const;
    void renderOnlyForegroundText(CRCore::crState& state) const;
    void renderWithPolygonOffset(CRCore::crState& state) const;
    void renderWithNoDepthBuffer(CRCore::crState& state) const;
    void renderWithDepthRange(CRCore::crState& state) const;
    void renderWithStencilBuffer(CRCore::crState& state) const;
    void renderWithDelayedDepthWrites(CRCore::crState& state) const;
    
    bool m_enableDepthWrites;

    BackdropType m_backdropType;
    BackdropImplementation m_backdropImplementation;

    float m_backdropHorizontalOffset;
    float m_backdropVerticalOffset;
    mutable CRCore::crVector4 m_backdropColor;

    ColorGradientMode m_colorGradientMode;
    CRCore::crVector4 m_colorGradientTopLeft;
    CRCore::crVector4 m_colorGradientBottomLeft;
    CRCore::crVector4 m_colorGradientBottomRight;
    CRCore::crVector4 m_colorGradientTopRight;

	CRCore::Vec2Array m_underLineCoords;
	mutable CRCore::Vec3Array m_underLineTCoords;
	//mutable CRCore::Vec3Array m_underLineTCoords_buf;
    // Helper function for color interpolation
    float bilinearInterpolate(float x1, float x2, float y1, float y2, float x, float y, float q11, float q12, float q21, float q22) const;
};

}


#endif
