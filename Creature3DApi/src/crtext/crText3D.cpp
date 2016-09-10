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
#include <CRText/crText3D.h>
#include <CRCore/crVector3.h>
//#include <CRCore/io_utils.h>

namespace CRText
{

crText3D::crText3D():
    m_renderMode(PER_GLYPH)
{
}

crText3D::crText3D(const crText3D & text3D, const CRCore::crCopyOp & copyop):
    CRText::crTextBase(text3D, copyop),
    m_renderMode(text3D.m_renderMode)
{
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

float crText3D::getCharacterDepth() const
{
    if (!m_style) return m_characterHeight*0.1f;
    else return m_characterHeight * m_style->getThicknessRatio();
}

void crText3D::setCharacterDepth(float characterDepth)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    getOrCreateStyle()->setThicknessRatio(characterDepth / m_characterHeight);

    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crText3D::accept(CRCore::crDrawable::ConstAttributeFunctor& af) const
{
    // ** for each line, do ...
    TextRenderInfo::const_iterator itLine, endLine = m_textRenderInfo.end();
    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {
            // ** apply the vertex array
            af.apply(CRCore::crDrawable::VERTICES, it->m_glyphGeometry->getVertexArray()->size(), &(it->m_glyphGeometry->getVertexArray()->front()));
        }
    }
}
void crText3D::accept(CRCore::crDrawable::PrimitiveFunctor& pf) const
{
    // ** for each line, do ...
    TextRenderInfo::const_iterator itLine, endLine = m_textRenderInfo.end();
    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {
            const CRCore::Vec3Array* vertices = it->m_glyphGeometry->getVertexArray();

            if (!vertices || vertices->empty())
              continue; //skip over spaces
          
            //pf.setVertexArray(it->_glyph->getVertexArray()->size(),&(it->_glyph->getVertexArray()->front()));
            //////////////////////////////////////////////////////////////////////////
            // now apply matrix to the glyphs.
            CRCore::ref_ptr<CRCore::Vec3Array> transformedVertices = new CRCore::Vec3Array;
            CRCore::crMatrix matrix = m_autoTransformCache[0].m_matrix;//CRCore::crMatrix();
            matrix.preMultTranslate(it->m_position);
            transformedVertices->reserve(vertices->size());
            for (CRCore::Vec3Array::const_iterator itr=vertices->begin(); itr!=vertices->end(); itr++)
            {
              transformedVertices->push_back((*itr)*matrix);
            }
            //////////////////////////////////////////////////////////////////////////
            pf.setVertexArray(transformedVertices->size(),&(transformedVertices->front()));

            // ** render the front face of the glyph
            const CRCore::crGeometry::PrimitiveList & pslFront = it->m_glyphGeometry->getFrontPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslFront.begin(), end = pslFront.end(); itr!=end; ++itr)
            {
                (*itr)->accept(pf);
            }

            // ** render the wall face of the glyph
            const CRCore::crGeometry::PrimitiveList & pslWall = it->m_glyphGeometry->getWallPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslWall.begin(), end=pslWall.end(); itr!=end; ++itr)
            {
                (*itr)->accept(pf);
            }

            // ** render the back face of the glyph
            const CRCore::crGeometry::PrimitiveList & pslBack = it->m_glyphGeometry->getBackPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslBack.begin(), end=pslBack.end(); itr!=end; ++itr)
            {
                (*itr)->accept(pf);
            }
        }
    }
}

crString::iterator crText3D::computeLastCharacterOnLine(CRCore::crVector2& cursor, crString::iterator first,crString::iterator last)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_font.valid() == false) return last;

    bool kerning = true;
    unsigned int previous_charcode = 0;

    crString::iterator lastChar = first;

    float maximumHeight = m_maximumHeight;
    float maximumWidth = m_maximumWidth;

    float hr = 1.0f;
    float wr = 1.0f;

    for(bool outOfSpace=false;lastChar!=last;++lastChar)
    {
        unsigned int charcode = *lastChar;

        if (charcode=='\n')
        {
            return lastChar;
        }

        crGlyph3D* glyph = m_font->getGlyph3D(charcode);
        if (glyph)
        {
            const CRCore::crBoundingBox & bb = glyph->getBoundingBox();

            if (m_layout==RIGHT_TO_LEFT)
            {
                cursor.x() -= glyph->getHorizontalAdvance() * wr;
            }

            // adjust cursor position w.r.t any kerning.
            if (kerning && previous_charcode)
            {
                switch(m_layout)
                {
                  case LEFT_TO_RIGHT:
                  {
                    CRCore::crVector2 delta(m_font->getKerning(previous_charcode,charcode,m_kerningType));
                    cursor.x() += delta.x() * wr;
                    cursor.y() += delta.y() * hr;
                    break;
                  }
                  case RIGHT_TO_LEFT:
                  {
                    CRCore::crVector2 delta(m_font->getKerning(charcode,previous_charcode,m_kerningType));
                    cursor.x() -= delta.x() * wr;
                    cursor.y() -= delta.y() * hr;
                    break;
                  }
                  case VERTICAL:
                    break; // no kerning when vertical.
                }
            }

            CRCore::crVector2 local = cursor;

            switch(m_layout)
            {
              case LEFT_TO_RIGHT:
              {
                if (maximumWidth>0.0f && local.x()+bb.xMax()>maximumWidth) outOfSpace=true;
                if(maximumHeight>0.0f && local.y()<-maximumHeight) outOfSpace=true;
                break;
              }
              case RIGHT_TO_LEFT:
              {
                if (maximumWidth>0.0f && local.x()+bb.xMin()<-maximumWidth) outOfSpace=true;
                if(maximumHeight>0.0f && local.y()<-maximumHeight) outOfSpace=true;
                break;
              }
              case VERTICAL:
                if (maximumHeight>0.0f && local.y()<-maximumHeight) outOfSpace=true;
                break;
            }

            // => word boundary detection & wrapping
            if (outOfSpace) break;

            // move the cursor onto the next character.
            switch(m_layout)
            {
              case LEFT_TO_RIGHT: cursor.x() += glyph->getHorizontalAdvance() * wr; break;
              case RIGHT_TO_LEFT: break;
              case VERTICAL:      cursor.y() -= glyph->getVerticalAdvance() * hr; break;
            }

            previous_charcode = charcode;

        }

    }

    // word boundary detection & wrapping
    if (lastChar!=last)
    {
        crString::iterator lastValidChar = lastChar;
          crString::iterator prevChar;
        while (lastValidChar != first){
            prevChar = lastValidChar - 1;

            // last char is after a hyphen
                if(*lastValidChar == '-')
                return lastValidChar + 1;

            // last char is start of whitespace
            if((*lastValidChar == ' ' || *lastValidChar == '\n') && (*prevChar != ' ' && *prevChar != '\n'))
                return lastValidChar;

            // Subtract off glyphs from the cursor position (to correctly center text)
                if(*prevChar != '-')
            {
                crGlyph3D* glyph = m_font->getGlyph3D(*prevChar);
                if (glyph)
                {
                    switch(m_layout)
                    {
                    case LEFT_TO_RIGHT: cursor.x() -= glyph->getHorizontalAdvance() * wr; break;
                    case RIGHT_TO_LEFT: cursor.x() += glyph->getHorizontalAdvance() * wr; break;
                    case VERTICAL:      cursor.y() += glyph->getVerticalAdvance() * wr; break;
                    }
                }
            }

            lastValidChar = prevChar;
          }
    }

    return lastChar;
}

void crText3D::computeGlyphRepresentation()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);

	if (m_font.valid() == false) return;

    m_textRenderInfo.clear();
    m_lineCount = 0;

    if (m_text.empty())
    {
        m_textBB.set(0,0,0, 0,0,0);//no size text
        crTextBase::computePositions(); //to reset the origin
        return;
    }

    // initialize bounding box, it will be expanded during glyph position calculation
    m_textBB.init();


    float hr = 1.0f;
    float wr = 1.0f;

    CRCore::crVector2 startOfLine_coords(0.0f,0.0f);
    CRCore::crVector2 cursor(startOfLine_coords);
    CRCore::crVector2 local(0.0f,0.0f);
    CRCore::crVector2 startOffset(0.0f,0.0f);

    unsigned int previous_charcode = 0;
    unsigned int linelength = 0;
    bool kerning = true;

    unsigned int lineNumber = 0;

    for(crString::iterator itr=m_text.begin(); itr!=m_text.end(); )
    {
        m_textRenderInfo.resize(lineNumber + 1);
        LineRenderInfo & currentLineRenderInfo = m_textRenderInfo.back();

        // record the start of the current line
        crString::iterator startOfLine_itr = itr;

        // find the end of the current line.
        CRCore::crVector2 endOfLine_coords(cursor);
        crString::iterator endOfLine_itr = computeLastCharacterOnLine(endOfLine_coords, itr,m_text.end());

        // ** position the cursor function to the Layout and the alignement
        crTextBase::positionCursor(endOfLine_coords, cursor, (unsigned int) (endOfLine_itr - startOfLine_itr));


        if (itr!=endOfLine_itr)
        {
            for(;itr!=endOfLine_itr;++itr)
            {
                unsigned int charcode = *itr;

                crGlyph3D* glyph = m_font->getGlyph3D(charcode);
                if (glyph)
                {
                    const CRCore::crBoundingBox & bb = glyph->getBoundingBox();

                    if (m_layout==RIGHT_TO_LEFT)
                    {
                        cursor.x() -= glyph->getHorizontalAdvance() * wr;
                    }

                    // adjust cursor position w.r.t any kerning.
                    if (kerning && previous_charcode)
                    {
                        switch(m_layout)
                        {
                          case LEFT_TO_RIGHT:
                          {
                            CRCore::crVector2 delta(m_font->getKerning(previous_charcode,charcode,m_kerningType));
                            cursor.x() += delta.x() * wr;
                            cursor.y() += delta.y() * hr;
                            break;
                          }
                          case RIGHT_TO_LEFT:
                          {
                            CRCore::crVector2 delta(m_font->getKerning(charcode,previous_charcode,m_kerningType));
                            cursor.x() -= delta.x() * wr;
                            cursor.y() -= delta.y() * hr;
                            break;
                          }
                          case VERTICAL:
                            break; // no kerning when vertical.
                        }
                    }

                    local = cursor;

                    // move the cursor onto the next character.
                    // also expand bounding box
                    switch (m_layout)
                    {
                        case LEFT_TO_RIGHT:
                            m_textBB.expandBy(CRCore::crVector3(cursor.x() + bb.xMin()*wr, cursor.y() + bb.yMin()*hr, 0.0f)); //lower left corner
                            m_textBB.expandBy(CRCore::crVector3(cursor.x() + bb.xMax()*wr, cursor.y() + bb.yMax()*hr, 0.0f)); //upper right corner
                            cursor.x() += glyph->getHorizontalAdvance() * wr;
                            break;
                        case VERTICAL:
                            m_textBB.expandBy(CRCore::crVector3(cursor.x(), cursor.y(), 0.0f)); //upper left corner
                            m_textBB.expandBy(CRCore::crVector3(cursor.x() + glyph->getWidth()*wr, cursor.y() - glyph->getHeight()*hr, 0.0f)); //lower right corner
                            cursor.y() -= glyph->getVerticalAdvance() * hr;
                            break;
                        case RIGHT_TO_LEFT:
                            m_textBB.expandBy(CRCore::crVector3(cursor.x()+bb.xMax()*wr, cursor.y() + bb.yMax()*hr, 0.0f)); //upper right corner
                            m_textBB.expandBy(CRCore::crVector3(cursor.x()+bb.xMin()*wr, cursor.y()+bb.yMin()*hr, 0.0f)); //lower left corner

                            break;
                    }

                    CRCore::crVector3 pos = CRCore::crVector3(local.x(), local.y(), 0.0f);
                    crGlyphGeometry* glyphGeometry = glyph->getGlyphGeometry(m_style.get());
                    currentLineRenderInfo.push_back(crText3D::GlyphRenderInfo(glyphGeometry, pos));

                    previous_charcode = charcode;
                }
            }
        }
        else
        {
            ++itr;
        }

        if (itr!=m_text.end())
        {
            // skip over spaces and return.
            while (*itr==' ') ++itr;
            if (*itr=='\n') ++itr;
        }

        // move to new line.
        switch(m_layout)
        {
            case LEFT_TO_RIGHT:
            case RIGHT_TO_LEFT:
            {
                startOfLine_coords.y() -= hr+m_lineSpacing;//(1.0 + m_lineSpacing) * hr;
                ++m_lineCount;
                break;
            }
            case VERTICAL:
            {
                startOfLine_coords.x() += (m_characterHeight + m_lineSpacing)*wr;//m_characterHeight * (1.0 + m_lineSpacing) * wr;
                // because m_lineCount is the max vertical no. of characters....
                m_lineCount = (m_lineCount >linelength)?m_lineCount:linelength;
                break;
            }
            default:
                break;
        }

        cursor = startOfLine_coords;
        previous_charcode = 0;

        ++lineNumber;
    }

    float thickness = m_style.valid() ? m_style->getThicknessRatio() : 0.1f;
    m_textBB.zMin() = -thickness;

    crTextBase::computePositions();
}

bool crText3D::computeBound() const
{
	m_bbox.init();
	//switch (m_needReCompute)
	//{
	//case 1:
	//	const_cast<crText3D *>(this)->computeGlyphRepresentation();
	//	m_needReCompute = 0;
	//	break;
	//case 2:
	//	dynamic_cast<crTextBase *>(const_cast<crText3D *>(this))->computePositions();
	//	m_needReCompute = 0;
	//	break;
	//}
    if (m_textBB.valid())
    {
        for(unsigned int i=0;i<m_autoTransformCache.size();++i)
        {
            CRCore::crMatrix& matrix = m_autoTransformCache[i].m_matrix;
            m_bbox.expandBy(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin())*matrix);
            m_bbox.expandBy(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMax())*matrix);
        }
    }

	m_bbox_computed = true;
	return true;
}

void crText3D::computePositions(unsigned int contextID) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);

    if (m_font.valid() == false) return;

    switch(m_alignment)
    {
    case LEFT_TOP:      m_offset.set(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMin()); break;
    case LEFT_CENTER:   m_offset.set(m_textBB.xMin(),(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case LEFT_BOTTOM:   m_offset.set(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin()); break;

    case CENTER_TOP:    m_offset.set((m_textBB.xMax()+m_textBB.xMin())*0.5f,m_textBB.yMax(),m_textBB.zMin()); break;
    case CENTER_CENTER: m_offset.set((m_textBB.xMax()+m_textBB.xMin())*0.5f,(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case CENTER_BOTTOM: m_offset.set((m_textBB.xMax()+m_textBB.xMin())*0.5f,m_textBB.yMin(),m_textBB.zMin()); break;

    case RIGHT_TOP:     m_offset.set(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMin()); break;
    case RIGHT_CENTER:  m_offset.set(m_textBB.xMax(),(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case RIGHT_BOTTOM:  m_offset.set(m_textBB.xMax(),m_textBB.yMin(),m_textBB.zMin()); break;

    case LEFT_BASE_LINE:  m_offset.set(0.0f,0.0f,0.0f); break;
    case CENTER_BASE_LINE:  m_offset.set((m_textBB.xMax()+m_textBB.xMin())*0.5f,0.0f,0.0f); break;
    case RIGHT_BASE_LINE:  m_offset.set(m_textBB.xMax(),0.0f,0.0f); break;

    case LEFT_BOTTOM_BASE_LINE:  m_offset.set(0.0f,-m_characterHeight*(m_lineCount-1),0.0f); break;
    case CENTER_BOTTOM_BASE_LINE:  m_offset.set((m_textBB.xMax()+m_textBB.xMin())*0.5f,-m_characterHeight*(m_lineCount-1),0.0f); break;
    case RIGHT_BOTTOM_BASE_LINE:  m_offset.set(m_textBB.xMax(),-m_characterHeight*(m_lineCount-1),0.0f); break;
    }

    AutoTransformCache& atc = m_autoTransformCache[contextID];
    CRCore::crMatrix& matrix = atc.m_matrix;


    CRCore::crVector3 scaleVec(m_characterHeight / getCharacterAspectRatio(), m_characterHeight , m_characterHeight);

    matrix.makeTranslate(-m_offset);
    matrix.postMultScale(scaleVec);
    matrix.postMultRotate(m_rotation);
    matrix.postMultTranslate(m_position);


    m_normal = CRCore::crMatrix::transform3x3(CRCore::crVector3(0.0f,0.0f,1.0f),matrix);
    m_normal.normalize();

    const_cast<crText3D*>(this)->dirtyBound();
}

void crText3D::drawImplementation(CRCore::crState& state) const
{
    unsigned int contextID = state.getContextID();

    // ** save the previous modelview matrix
    CRCore::crMatrix previous(state.getModelViewMatrix());

    // ** get the modelview for this context
    CRCore::crMatrix modelview(m_autoTransformCache[contextID].m_matrix);

    // ** mult previous by the modelview for this context
    modelview.postMult(previous);

    // ** apply this new modelview matrix
    state.applyModelViewMatrix(new CRCore::RefMatrix(modelview));

    //CRCore::GLBeginEndAdapter& gl = (state.getGLBeginEndAdapter());

    if (m_drawMode & BOUNDINGBOX)
    {
        if (m_textBB.valid())
        {
            state.applyColor(m_textBBColor);

            CRCore::crVector3 c000(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMax()));
            CRCore::crVector3 c100(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMin(),m_textBB.zMax()));
            CRCore::crVector3 c110(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMax()));
            CRCore::crVector3 c010(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMax()));

            CRCore::crVector3 c001(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin()));
            CRCore::crVector3 c101(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMin(),m_textBB.zMin()));
            CRCore::crVector3 c111(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMin()));
            CRCore::crVector3 c011(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMin()));

            glBegin(GL_LINE_LOOP);
                glVertex3fv(c000.ptr());
                glVertex3fv(c100.ptr());
                glVertex3fv(c110.ptr());
                glVertex3fv(c010.ptr());
            glEnd();

            glBegin(GL_LINE_LOOP);
                glVertex3fv(c001.ptr());
                glVertex3fv(c011.ptr());
                glVertex3fv(c111.ptr());
                glVertex3fv(c101.ptr());
            glEnd();

            glBegin(GL_LINES);
                glVertex3fv(c000.ptr());
                glVertex3fv(c001.ptr());

                glVertex3fv(c100.ptr());
                glVertex3fv(c101.ptr());

                glVertex3fv(c110.ptr());
                glVertex3fv(c111.ptr());

                glVertex3fv(c010.ptr());
                glVertex3fv(c011.ptr());
            glEnd();
        }
    }

    if (m_drawMode & ALIGNMENT)
    {
        float cursorsize = m_characterHeight*0.5f;

        CRCore::crVector3 hl(CRCore::crVector3(m_offset.x()-cursorsize,m_offset.y(),m_offset.z()));
        CRCore::crVector3 hr(CRCore::crVector3(m_offset.x()+cursorsize,m_offset.y(),m_offset.z()));
        CRCore::crVector3 vt(CRCore::crVector3(m_offset.x(),m_offset.y()-cursorsize,m_offset.z()));
        CRCore::crVector3 vb(CRCore::crVector3(m_offset.x(),m_offset.y()+cursorsize,m_offset.z()));

        state.applyColor(m_textBBColor);

        glBegin(GL_LINES);
            glVertex3fv(hl.ptr());
            glVertex3fv(hr.ptr());
            glVertex3fv(vt.ptr());
            glVertex3fv(vb.ptr());
        glEnd();

    }

    if (m_drawMode & TEXT)
    {
        state.disableColorPointer();
        state.applyColor(m_color);

        state.disableAllVertexArrays();

        //#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
        state.applyMode(GL_NORMALIZE, true);
        //#endif

        switch(m_renderMode)
        {
            case PER_FACE:  renderPerFace(state);   break;
            case PER_GLYPH:
            default:        renderPerGlyph(state);  break;
        }
    }


    // restore the previous modelview matrix
    state.applyModelViewMatrix(new CRCore::RefMatrix(previous));
}

void crText3D::renderPerGlyph(CRCore::crState & state) const
{
    CRCore::crMatrix original_modelview = state.getModelViewMatrix();

    const CRCore::crStateSet* frontStateSet = getStateSet();
    const CRCore::crStateSet* wallStateSet = getWallStateSet();
    const CRCore::crStateSet* backStateSet = getBackStateSet();
    bool applyMainColor = false;

    if (wallStateSet==0) wallStateSet = frontStateSet;
    else if (wallStateSet->getAttribute(CRCore::crStateAttribute::MATERIAL)!=0) applyMainColor = true;

    if (backStateSet==0) backStateSet = frontStateSet;
    else if (backStateSet->getAttribute(CRCore::crStateAttribute::MATERIAL)!=0) applyMainColor = true;

    // ** for each line, do ...
    TextRenderInfo::const_iterator itLine, endLine = m_textRenderInfo.end();
    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {

            CRCore::crMatrix matrix(original_modelview);
            matrix.preMultTranslate(CRCore::crVector3d(it->m_position.x(), it->m_position.y(), it->m_position.z()));
            state.applyModelViewMatrix(new CRCore::RefMatrix(matrix));

            //state.lazyDisablingOfVertexAttributes();

            // ** apply the vertex array
			const CRCore::Vec3Array *vtxArray = it->m_glyphGeometry->getVertexArray();
			const CRCore::Vec3Array *normalArray = it->m_glyphGeometry->getNormalArray();
            state.setVertexPointer(vtxArray->getDataSize(),vtxArray->getDataType(),0,vtxArray->getDataPointer());
            state.setNormalPointer(GL_FLOAT,0,normalArray->getDataPointer());

            //state.applyDisablingOfVertexAttributes();

            if (frontStateSet && frontStateSet!=backStateSet)
            {
                state.apply(frontStateSet);
                if (applyMainColor) state.applyColor(m_color);
            }

            const CRCore::crGeometry::PrimitiveList & pslFront = it->m_glyphGeometry->getFrontPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslFront.begin(), end = pslFront.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }

            if (wallStateSet && wallStateSet!=frontStateSet) state.apply(wallStateSet);

            // ** render the wall face of the glyph
            const CRCore::crGeometry::PrimitiveList & pslWall = it->m_glyphGeometry->getWallPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslWall.begin(), end=pslWall.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }

            if (backStateSet && backStateSet!=wallStateSet)
            {
                state.apply(backStateSet);
                if (applyMainColor) state.applyColor(m_color);
            }

            const CRCore::crGeometry::PrimitiveList & pslBack = it->m_glyphGeometry->getBackPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=pslBack.begin(), end=pslBack.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }
        }
    }
}

void crText3D::renderPerFace(CRCore::crState & state) const
{
    CRCore::crMatrix original_modelview = state.getModelViewMatrix();
//#if 0
//    // ** render all front faces
//    state.Normal(0.0f,0.0f,1.0f);
//#endif

    const CRCore::crStateSet* frontStateSet = getStateSet();
    const CRCore::crStateSet* wallStateSet = getWallStateSet();
    const CRCore::crStateSet* backStateSet = getBackStateSet();
    bool applyMainColor = false;

    if (wallStateSet==0) wallStateSet = frontStateSet;
    else if (wallStateSet->getAttribute(CRCore::crStateAttribute::MATERIAL)!=0) applyMainColor = true;

    if (backStateSet==0) backStateSet = frontStateSet;
    else if (backStateSet->getAttribute(CRCore::crStateAttribute::MATERIAL)!=0) applyMainColor = true;


    TextRenderInfo::const_iterator itLine, endLine = m_textRenderInfo.end();
    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {
            CRCore::crMatrix matrix(original_modelview);
            matrix.preMultTranslate(CRCore::crVector3d(it->m_position.x(), it->m_position.y(), it->m_position.z()));
            state.applyModelViewMatrix(new CRCore::RefMatrix(matrix));

			const CRCore::Vec3Array *vtxArray = it->m_glyphGeometry->getVertexArray();
			const CRCore::Vec3Array *normalArray = it->m_glyphGeometry->getNormalArray();
			state.setVertexPointer(vtxArray->getDataSize(),vtxArray->getDataType(),0,vtxArray->getDataPointer());
			state.setNormalPointer(GL_FLOAT,0,normalArray->getDataPointer());

            // ** render the front face of the glyph
            const CRCore::crGeometry::PrimitiveList & psl = it->m_glyphGeometry->getFrontPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=psl.begin(), end=psl.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }
        }
    }

    if (wallStateSet && wallStateSet!=frontStateSet) state.apply(wallStateSet);

    // ** render all wall face of the text
    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {
            CRCore::crMatrix matrix(original_modelview);
            matrix.preMultTranslate(CRCore::crVector3d(it->m_position.x(), it->m_position.y(), it->m_position.z()));
            state.applyModelViewMatrix(new CRCore::RefMatrix(matrix));

			const CRCore::Vec3Array *vtxArray = it->m_glyphGeometry->getVertexArray();
			const CRCore::Vec3Array *normalArray = it->m_glyphGeometry->getNormalArray();
			state.setVertexPointer(vtxArray->getDataSize(),vtxArray->getDataType(),0,vtxArray->getDataPointer());
			state.setNormalPointer(GL_FLOAT,0,normalArray->getDataPointer());

            const CRCore::crGeometry::PrimitiveList & psl = it->m_glyphGeometry->getWallPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=psl.begin(), end=psl.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }
        }
    }
#if 0
    state.disableNormalPointer();

    // ** render all back face of the text
    state.Normal(0.0f,0.0f,-1.0f);
#endif

    if (backStateSet && backStateSet!=wallStateSet)
    {
        state.apply(backStateSet);
        if (applyMainColor) state.applyColor(m_color);
    }

    for (itLine = m_textRenderInfo.begin(); itLine!=endLine; ++itLine)
    {
        // ** for each glyph in the line, do ...
        LineRenderInfo::const_iterator it, end = itLine->end();
        for (it = itLine->begin(); it!=end; ++it)
        {
            CRCore::crMatrix matrix(original_modelview);
            matrix.preMultTranslate(CRCore::crVector3d(it->m_position.x(), it->m_position.y(), it->m_position.z()));
            state.applyModelViewMatrix(new CRCore::RefMatrix(matrix));

			const CRCore::Vec3Array *vtxArray = it->m_glyphGeometry->getVertexArray();
			const CRCore::Vec3Array *normalArray = it->m_glyphGeometry->getNormalArray();
			state.setVertexPointer(vtxArray->getDataSize(),vtxArray->getDataType(),0,vtxArray->getDataPointer());
			state.setNormalPointer(GL_FLOAT,0,normalArray->getDataPointer());

            // ** render the back face of the glyph
            const CRCore::crGeometry::PrimitiveList & psl = it->m_glyphGeometry->getBackPrimitiveSetList();
            for(CRCore::crGeometry::PrimitiveList::const_iterator itr=psl.begin(), end=psl.end(); itr!=end; ++itr)
            {
                (*itr)->draw(state, false);
            }
        }
    }
}



//void crText3D::setThreadSafeRefUnref(bool threadSafe)
//{
//    crTextBase::setThreadSafeRefUnref(threadSafe);
//
//    if (m_font.valid()) m_font->setThreadSafeRefUnref(threadSafe);
//}
//
//void crText3D::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    CRCore::notify(CRCore::INFO)<<"crText3D::resizeGLObjectBuffers("<<maxSize<<")"<<std::endl;
//
//    crTextBase::resizeGLObjectBuffers(maxSize);
//
//    if (m_font.valid()) m_font->resizeGLObjectBuffers(maxSize);
//
//    crTextBase::computePositions();
//}

void crText3D::releaseObjects(CRCore::crState* state)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    crTextBase::releaseObjects(state);
    if (m_font.valid())
	{
		m_font->releaseObjects(state);
		m_font = NULL;
	}
}

}
