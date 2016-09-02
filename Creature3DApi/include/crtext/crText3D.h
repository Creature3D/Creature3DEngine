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
//Modified by 吴财华

#ifndef CRTEXT_CRTEXT3D
#define CRTEXT_CRTEXT3D 1


#include <CRText/crTextBase.h>
#include <CRText/crStyle.h>

namespace CRText {


class CRTEXT_EXPORT crText3D : public CRText::crTextBase
{
    public:

        /** Reder mode used to render the crText.
         * PER_FACE : render all front face with the default crStateSet
         *                   all wall face with the wall crStateSet
         *                   all back face with the back crStateSet (back face of the character, no the OpenGL back face)
         *
         * PER_GLYPH : render all Charactere with the default crStateSet
         */
        enum RenderMode
        {
            PER_FACE,
            PER_GLYPH
        };

        crText3D();
        crText3D(const crText3D& text,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRText,crText3D)

        /** Get the Charactere Depth of text. */
        float getCharacterDepth() const;

        /** Set the Charactere Depth of text. */
        void setCharacterDepth(float characterDepth);

        /** Get the render mode used to render the text. */
        RenderMode getRenderMode() const { return m_renderMode; }
        /** Set the render mode used to render the text. */
        void setRenderMode(RenderMode renderMode) { m_renderMode = renderMode; m_needReCompute = 1;/*computeGlyphRepresentation();*/ }


        /** Get the wall crStateSet */
        CRCore::crStateSet* getWallStateSet() { return m_wallStateSet.get(); }
        /** Get the wall crStateSet */
        const CRCore::crStateSet* getWallStateSet() const { return m_wallStateSet.get(); }
        /** Get or create the wall crStateSet */
        CRCore::crStateSet* getOrCreateWallStateSet()
        {
           if (m_wallStateSet.valid() == false) m_wallStateSet = new CRCore::crStateSet;
           return m_wallStateSet.get();
        }
        /** Set the wall crStateSet */
        void setWallStateSet(CRCore::crStateSet* wallStateSet)  { m_wallStateSet = wallStateSet; }

        /** Get the back crStateSet */
        CRCore::crStateSet* getBackStateSet() { return m_backStateSet.get(); }
        /** Get the back crStateSet */
        const CRCore::crStateSet* getBackStateSet() const { return m_backStateSet.get(); }
        /** Get or create the back crStateSet */
        CRCore::crStateSet* getOrCreateBackStateSet() { if (m_backStateSet.valid() == false) m_backStateSet = new CRCore::crStateSet; return m_backStateSet.get(); }
        /** Set the back crStateSet */
        void setBackStateSet(CRCore::crStateSet* backStateSet)  { m_backStateSet = backStateSet; }



        /** Draw the text.*/
        virtual void drawImplementation(CRCore::crState& state) const;

        /** return false, CRText::crText does not support accept(AttributeFunctor&).*/
        virtual bool supports(const CRCore::crDrawable::AttributeFunctor&) const { return false; }

        /** return true, CRText::crText does support accept(ConstAttributeFunctor&).*/
        virtual bool supports(const CRCore::crDrawable::ConstAttributeFunctor&) const { return false; }

        /** accept an ConstAttributeFunctor and call its methods to tell it about the interal attributes that this crDrawable has.*/
        virtual void accept(CRCore::crDrawable::ConstAttributeFunctor& af) const;

        /** return true, CRText::crText does support accept(PrimitiveFunctor&) .*/
		virtual bool supports(const CRCore::crDrawable::PrimitiveFunctor&) const { return false; }

        /** accept a PrimtiveFunctor and call its methods to tell it about the interal primtives that this crDrawable has.*/
		virtual void accept(CRCore::crDrawable::PrimitiveFunctor& pf) const;


        ///** Set whether to use a mutex to ensure ref() and unref() are thread safe.*/
        //virtual void setThreadSafeRefUnref(bool threadSafe);

        ///** Resize any per context GLObject buffers to specified size. */
        //virtual void resizeGLObjectBuffers(unsigned int maxSize);

        /** If crState is non-zero, this function releases OpenGL objects for
          * the specified graphics context. Otherwise, releases OpenGL objexts
          * for all graphics contexts. */
        virtual void releaseObjects(CRCore::crState* state=0);

    //    // make crFont a friend to allow it set the m_font to 0 if the font is
    //    // forcefully unloaded.
        friend class crFont;
    protected:
        virtual ~crText3D() {}
		virtual bool computeBound() const;

        void renderPerGlyph(CRCore::crState & state) const;
        void renderPerFace(CRCore::crState & state) const;

        crString::iterator computeLastCharacterOnLine(CRCore::crVector2& cursor, crString::iterator first,crString::iterator last);

        void computeGlyphRepresentation();
        void computePositions(unsigned int contextID) const;

        // ** glyph and other information to render the glyph
        struct GlyphRenderInfo
        {
            GlyphRenderInfo(crGlyphGeometry* glyphGeometry, CRCore::crVector3 & pos):
                m_glyphGeometry(glyphGeometry),
                m_position(pos) {}

            CRCore::ref_ptr<crGlyphGeometry> m_glyphGeometry;
            CRCore::crVector3 m_position;
        };

        typedef std::vector<GlyphRenderInfo> LineRenderInfo;
        typedef std::vector<LineRenderInfo> TextRenderInfo;

        TextRenderInfo m_textRenderInfo;


        RenderMode m_renderMode;

        CRCore::ref_ptr<CRCore::crStateSet> m_wallStateSet;
        CRCore::ref_ptr<CRCore::crStateSet> m_backStateSet;
};

}


#endif
