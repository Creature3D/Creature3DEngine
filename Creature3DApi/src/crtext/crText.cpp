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
#include <CRText/crText.h>
#include <CRText/crFont.h>
#include <CRText/crDefaultFont.h>
#include <CRCore/crMath.h>
#include <crgl/gl.h>
#include <CRCore/crNotify.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crTexEnv.h>

//#include <CRUtil/crCullVisitor.h>

#include <CRIOManager/crReadFile.h>

using namespace CRCore;
using namespace CRText;

//#define TREES_CODE_FOR_MAKING_SPACES_EDITABLE

crText::crText():
    m_enableDepthWrites(false),
    m_backdropType(NONE),
    m_backdropImplementation(DELAYED_DEPTH_WRITES),
    m_backdropHorizontalOffset(0.075f/*0.07f*/),
    m_backdropVerticalOffset(0.075f/*0.07f*/),
    m_backdropColor(0.0f, 0.0f, 0.0f, 1.0f),
    m_colorGradientMode(SOLID),
    m_colorGradientTopLeft(1.0f, 0.0f, 0.0f, 1.0f),
    m_colorGradientBottomLeft(0.0f, 1.0f, 0.0f, 1.0f),
    m_colorGradientBottomRight(0.0f, 0.0f, 1.0f, 1.0f),
    m_colorGradientTopRight(1.0f, 1.0f, 1.0f, 1.0f)
{}

crText::crText(const crText& text,const CRCore::crCopyOp& copyop):
    CRText::crTextBase(text,copyop),
    m_enableDepthWrites(text.m_enableDepthWrites),
    m_backdropType(text.m_backdropType),
    m_backdropImplementation(text.m_backdropImplementation),
    m_backdropHorizontalOffset(text.m_backdropHorizontalOffset),
    m_backdropVerticalOffset(text.m_backdropVerticalOffset),
    m_backdropColor(text.m_backdropColor),
    m_colorGradientMode(text.m_colorGradientMode),
    m_colorGradientTopLeft(text.m_colorGradientTopLeft),
    m_colorGradientBottomLeft(text.m_colorGradientBottomLeft),
    m_colorGradientBottomRight(text.m_colorGradientBottomRight),
    m_colorGradientTopRight(text.m_colorGradientTopRight)
{
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

crText::~crText()
{
}
//void crText::setFont(crFont* font)
//{
//    if (m_font==font) return;
//	//CRCore::crStateSet* previousFontStateSet = m_font.valid() ? m_font->getStateSet() : crDefaultFont::instance()->getStateSet();
// //   CRCore::crStateSet* newFontStateSet = font ? font->getStateSet() : crDefaultFont::instance()->getStateSet();
//
// //   if (getStateSet() == previousFontStateSet)
// //   {
// //       setStateSet( newFontStateSet );
// //   }
//
//    crTextBase::setFont(font);
//}


crFont* crText::getActiveFont()
{
    return m_font.valid() ? m_font.get() : crDefaultFont::instance();
}

const crFont* crText::getActiveFont() const
{
    return m_font.valid() ? m_font.get() : crDefaultFont::instance();
}

crString::iterator crText::computeLastCharacterOnLine(CRCore::crVector2& cursor, crString::iterator first,crString::iterator last)
{
    crFont* activefont = getActiveFont();
    if (!activefont) return last;

    float hr = m_characterHeight;
    float wr = hr/getCharacterAspectRatio();

    bool kerning = true;
    unsigned int previous_charcode = 0;

    crString::iterator lastChar = first;

    for(bool outOfSpace=false;lastChar!=last;++lastChar)
    {
        unsigned int charcode = *lastChar;
        
        if (charcode=='\n')
        {
            return lastChar;
        }

        crGlyph* glyph = activefont->getGlyph(m_fontSize, charcode);
        if (glyph)
        {

           float width = (float)(glyph->getWidth()) * wr;

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
                    CRCore::crVector2 delta(activefont->getKerning(previous_charcode,charcode,m_kerningType));
                    cursor.x() += delta.x() * wr;
                    cursor.y() += delta.y() * hr;
                    break;
                  }
                  case RIGHT_TO_LEFT:
                  {
                    CRCore::crVector2 delta(activefont->getKerning(charcode,previous_charcode,m_kerningType));
                    cursor.x() -= delta.x() * wr;
                    cursor.y() -= delta.y() * hr;
                    break;
                  }
                  case VERTICAL:
                    break; // no kerning when vertical.
                }            // check to see if we are still within line if not move to next line.
            }

            switch(m_layout)
            {
              case LEFT_TO_RIGHT:
              {
                if (m_maximumWidth>0.0f && cursor.x()+width>m_maximumWidth) outOfSpace=true;
                if(m_maximumHeight>0.0f && cursor.y()<-m_maximumHeight) outOfSpace=true;
                break;
              }
              case RIGHT_TO_LEFT:
              {
                if (m_maximumWidth>0.0f && cursor.x()<-m_maximumWidth) outOfSpace=true;
                if(m_maximumHeight>0.0f && cursor.y()<-m_maximumHeight) outOfSpace=true;
                break;
              }
              case VERTICAL:
                if (m_maximumHeight>0.0f && cursor.y()<-m_maximumHeight) outOfSpace=true;
                break;
            }
            
            // => word boundary detection & wrapping
            if (outOfSpace) break;

            // move the cursor onto the next character.
            switch(m_layout)
            {
              case LEFT_TO_RIGHT: cursor.x() += glyph->getHorizontalAdvance() * wr; break;
              case VERTICAL:      cursor.y() -= glyph->getVerticalAdvance() *hr; break;
              case RIGHT_TO_LEFT: break; // nop.
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
                crGlyph* glyph = activefont->getGlyph(m_fontSize, *prevChar);
                if (glyph)
                {
                    switch(m_layout)
                    {
                    case LEFT_TO_RIGHT: cursor.x() -= glyph->getHorizontalAdvance() * wr; break;
                    case VERTICAL:      cursor.y() += glyph->getVerticalAdvance() * hr; break;
                    case RIGHT_TO_LEFT: break; // nop.
                    }
                }
            }

            lastValidChar = prevChar;
          }
    }

    return lastChar;
}

void crText::computeGlyphRepresentation()
{
    crFont* activefont = getActiveFont();
    if (!activefont) return;
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);

    m_textureGlyphQuadMap.clear();
    m_lineCount = 0;
    
    if (m_text.empty()) 
    {
        m_textBB.set(0,0,0,0,0,0);//no size text
        crTextBase::computePositions(); //to reset the origin
        return;
    }
    
    //CRCore::ScopedLock<crFont::FontMutex> lock(*(activefont->getSerializeFontCallsMutex()));

    // initialize bounding box, it will be expanded during glyph position calculation
    m_textBB.init();

    CRCore::crVector2 startOfLine_coords(0.0f,m_startCoord[1]);
    CRCore::crVector2 cursor(startOfLine_coords);
    CRCore::crVector2 local(startOfLine_coords);
    
    unsigned int previous_charcode = 0;
    unsigned int linelength = 0;
    bool horizontal = m_layout!=VERTICAL;
    bool kerning = true;
    
    unsigned int lineNumber = 0;

    float hr = m_characterHeight;
    float wr = hr/getCharacterAspectRatio();
	ref_ptr<crGlyph> glyph;
	ref_ptr<crGlyphTexture> glyphtex;
	m_underLineCoords.resize(0);
    for(crString::iterator itr=m_text.begin();
        itr!=m_text.end();
        )
    {
        // record the start of the current line
        crString::iterator startOfLine_itr = itr;

		if(lineNumber == 0)
		{
			cursor[0] = m_startCoord[0];
		}
        // find the end of the current line.
        CRCore::crVector2 endOfLine_coords(cursor);
        crString::iterator endOfLine_itr = computeLastCharacterOnLine(endOfLine_coords, itr,m_text.end());

        linelength = endOfLine_itr - startOfLine_itr;
        // Set line position to correct alignment.
        switch(m_layout)
        {
			case LEFT_TO_RIGHT:
            {
				switch(m_alignment)
				{
				// nothing to be done for these
				//case LEFT_TOP:
				//case LEFT_CENTER:
				//case LEFT_BOTTOM:
				//case LEFT_BASE_LINE:
				//case LEFT_BOTTOM_BASE_LINE:
				//  break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
				case RIGHT_TOP:
				case RIGHT_CENTER:
				case RIGHT_BOTTOM:
				case RIGHT_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() - endOfLine_coords.x();
					break;
				default:
					break;
				}
				break;
            }
            case RIGHT_TO_LEFT:
            {
				switch(m_alignment)
				{
				case LEFT_TOP:
				case LEFT_CENTER:
				case LEFT_BOTTOM:
				case LEFT_BASE_LINE:
				case LEFT_BOTTOM_BASE_LINE:
					cursor.x() = 2*cursor.x() - endOfLine_coords.x();
					break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() + (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
				// nothing to be done for these
				//case RIGHT_TOP:
				//case RIGHT_CENTER:
				//case RIGHT_BOTTOM:
				//case RIGHT_BASE_LINE:
				//case RIGHT_BOTTOM_BASE_LINE:
				//  break;
				default:
					break;
				}
				break;
            }
            case VERTICAL:
            {
				switch(m_alignment)
				{
				// TODO: current behaviour top baselines lined up in both cases - need to implement
				//       top of characters alignment - Question is this necessary?
				// ... otherwise, nothing to be done for these 6 cases
				//case LEFT_TOP:
				//case CENTER_TOP:
				//case RIGHT_TOP:
				//  break;
				//case LEFT_BASE_LINE:
				//case CENTER_BASE_LINE:
				//case RIGHT_BASE_LINE:
				//  break;
				case LEFT_CENTER:
				case CENTER_CENTER:
				case RIGHT_CENTER:
					cursor.y() = cursor.y() + (cursor.y() - endOfLine_coords.y()) * 0.5f;
					break;
				case LEFT_BOTTOM_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.y() = cursor.y() - (linelength * m_characterHeight);
					break;
				case LEFT_BOTTOM:
				case CENTER_BOTTOM:
				case RIGHT_BOTTOM:
					cursor.y() = 2*cursor.y() - endOfLine_coords.y();
					break;
				default:
					break;
				}
				break;
			}
        }
		if(linelength>0)
			m_underLineCoords.push_back(crVector2(cursor[0],cursor[1]-m_underLineSpacing));
        if (itr!=endOfLine_itr)
        {

            for(;itr!=endOfLine_itr;++itr)
            {
                unsigned int charcode = *itr;

                glyph = activefont->getGlyph(m_fontSize, charcode);
                if (glyph.valid())
                {
                    float width = (float)(glyph->getWidth()) * wr;
                    float height = (float)(glyph->getHeight()) * hr;

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
                            CRCore::crVector2 delta(activefont->getKerning(previous_charcode,charcode,m_kerningType));
                            cursor.x() += delta.x() * wr;
                            cursor.y() += delta.y() * hr;
                            break;
                          }
                          case RIGHT_TO_LEFT:
                          {
                            CRCore::crVector2 delta(activefont->getKerning(charcode,previous_charcode,m_kerningType));
                            cursor.x() -= delta.x() * wr;
                            cursor.y() -= delta.y() * hr;
                            break;
                          }
                          case VERTICAL:
                            break; // no kerning when vertical.
                        }
                    }

                    local = cursor;
                    CRCore::crVector2 bearing(horizontal?glyph->getHorizontalBearing():glyph->getVerticalBearing());
                    local.x() += bearing.x() * wr;
                    local.y() += bearing.y() * hr;

					glyphtex = glyph->getTexture();
					if(!glyphtex)
						break;
					if(m_textureGlyphQuadMap.find(glyphtex) == m_textureGlyphQuadMap.end())
					{
						GlyphQuads glyphquad;
						m_textureGlyphQuadMap[glyphtex] = glyphquad;
					}
                    GlyphQuads& glyphquad = m_textureGlyphQuadMap[glyphtex];

                    glyphquad.m_glyphs.push_back(glyph.get());
                    glyphquad.m_lineNumbers.push_back(lineNumber);

                    // Adjust coordinates and texture coordinates to avoid
                    // clipping the edges of antialiased characters.
                    CRCore::crVector2 mintc = glyph->getMinTexCoord();
                    CRCore::crVector2 maxtc = glyph->getMaxTexCoord();
                    CRCore::crVector2 vDiff = maxtc - mintc;

                    float fHorizTCMargin = 1.0f / glyph->getTexture()->getTextureWidth();
                    float fVertTCMargin = 1.0f / glyph->getTexture()->getTextureHeight();
                    float fHorizQuadMargin = vDiff.x() == 0.0f ? 0.0f : width * fHorizTCMargin / vDiff.x();
                    float fVertQuadMargin = vDiff.y() == 0.0f ? 0.0f : height * fVertTCMargin / vDiff.y();

                    mintc.x() -= fHorizTCMargin;
                    mintc.y() -= fVertTCMargin;
                    maxtc.x() += fHorizTCMargin;
                    maxtc.y() += fVertTCMargin;

                    // set up the coords of the quad
                    CRCore::crVector2 upLeft = local+CRCore::crVector2(0.0f-fHorizQuadMargin,height+fVertQuadMargin);
                    CRCore::crVector2 lowLeft = local+CRCore::crVector2(0.0f-fHorizQuadMargin,0.0f-fVertQuadMargin);
                    CRCore::crVector2 lowRight = local+CRCore::crVector2(width+fHorizQuadMargin,0.0f-fVertQuadMargin);
                    CRCore::crVector2 upRight = local+CRCore::crVector2(width+fHorizQuadMargin,height+fVertQuadMargin);
                    glyphquad.m_coords.push_back(upLeft);
                    glyphquad.m_coords.push_back(lowLeft);
                    glyphquad.m_coords.push_back(lowRight);
                    glyphquad.m_coords.push_back(upRight);

                    // set up the tex coords of the quad
                    glyphquad.m_texcoords.push_back(CRCore::crVector2(mintc.x(),maxtc.y()));
                    glyphquad.m_texcoords.push_back(CRCore::crVector2(mintc.x(),mintc.y()));
                    glyphquad.m_texcoords.push_back(CRCore::crVector2(maxtc.x(),mintc.y()));
                    glyphquad.m_texcoords.push_back(CRCore::crVector2(maxtc.x(),maxtc.y()));

                    // move the cursor onto the next character.
                    // also expand bounding box
                    switch(m_layout)
                    {
                      case LEFT_TO_RIGHT:
                          cursor.x() += glyph->getHorizontalAdvance() * wr;
                          m_textBB.expandBy(CRCore::crVector3(lowLeft.x(), lowLeft.y(), 0.0f)); //lower left corner
                          m_textBB.expandBy(CRCore::crVector3(upRight.x(), upRight.y(), 0.0f)); //upper right corner
                          break;
                      case VERTICAL:
                          cursor.y() -= glyph->getVerticalAdvance() * hr;
                          m_textBB.expandBy(CRCore::crVector3(upLeft.x(),upLeft.y(),0.0f)); //upper left corner
                          m_textBB.expandBy(CRCore::crVector3(lowRight.x(),lowRight.y(),0.0f)); //lower right corner
                          break;
                      case RIGHT_TO_LEFT:
                          m_textBB.expandBy(CRCore::crVector3(lowRight.x(),lowRight.y(),0.0f)); //lower right corner
                          m_textBB.expandBy(CRCore::crVector3(upLeft.x(),upLeft.y(),0.0f)); //upper left corner
                          break;
                    }
                    previous_charcode = charcode;

                }
            }

            // skip over spaces and return.
            while (itr != m_text.end() && *itr==' ') ++itr;
            if (itr != m_text.end() && *itr=='\n') 
			{
				previous_charcode = '\n';
				++itr;
			}
        }
        else// if(*itr=='\n')///防止该行文字丢失20131203
        {
            ++itr;
        }
		if(linelength>0)
			m_underLineCoords.push_back(crVector2(cursor[0],cursor[1]-m_underLineSpacing));
        // move to new line.
        switch(m_layout)
        {
			case LEFT_TO_RIGHT:
			{
				if(previous_charcode != '\n')
					m_endCoord.set(cursor.x(),startOfLine_coords.y());
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				if(previous_charcode == '\n')
					m_endCoord.set(0.0f,startOfLine_coords.y());
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
			case RIGHT_TO_LEFT:
			{
				if(previous_charcode != '\n')
					m_endCoord.set(cursor.x(),startOfLine_coords.y());
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				if(previous_charcode == '\n')
					m_endCoord.set(0.0f,startOfLine_coords.y());
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
			case VERTICAL:
			{
				if(previous_charcode != '\n')
					m_endCoord.set(startOfLine_coords.x(),cursor.y());
				startOfLine_coords.x() += (m_characterHeight + m_lineSpacing)/getCharacterAspectRatio();//m_characterHeight/getCharacterAspectRatio() * (1.0 + m_lineSpacing);
				if(previous_charcode == '\n')
					m_endCoord.set(startOfLine_coords.x(),cursor.y());
				cursor = startOfLine_coords;
				previous_charcode = 0;
				// because m_lineCount is the max vertical no. of characters....
				m_lineCount = (m_lineCount >linelength)?m_lineCount:linelength;
			}
			break;
        }
        
        ++lineNumber;

    }

    crTextBase::computePositions();
    computeBackdropBoundingBox();
    computeBoundingBoxMargin();
    computeColorGradients();
}
bool crText::getCursorPosition(int index,CRCore::crVector2 &outPos)
{
	crFont* activefont = getActiveFont();
	if (!activefont) return false;
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);

	if (m_text.empty()) 
	{
		return false;
	}
	if(index == 0)
	{
		outPos.set(0.0f,0.0f);
		return true;
	}
	CRCore::crVector2 startOfLine_coords(0.0f,m_startCoord[1]);
	CRCore::crVector2 cursor(startOfLine_coords);
	CRCore::crVector2 local(startOfLine_coords);

	unsigned int previous_charcode = 0;
	unsigned int linelength = 0;
	bool horizontal = m_layout!=VERTICAL;
	bool kerning = true;

	unsigned int lineNumber = 0;

	float hr = m_characterHeight;
	float wr = hr/getCharacterAspectRatio();
	int id = 0;
	for(crString::iterator itr=m_text.begin();
		itr!=m_text.end();
		)
	{
		// record the start of the current line
		crString::iterator startOfLine_itr = itr;

		if(lineNumber == 0)
		{
			cursor[0] = m_startCoord[0];
		}
		// find the end of the current line.
		CRCore::crVector2 endOfLine_coords(cursor);
		crString::iterator endOfLine_itr = computeLastCharacterOnLine(endOfLine_coords, itr,m_text.end());

		linelength = endOfLine_itr - startOfLine_itr;

		// Set line position to correct alignment.
		switch(m_layout)
		{
		case LEFT_TO_RIGHT:
			{
				switch(m_alignment)
				{
					// nothing to be done for these
					//case LEFT_TOP:
					//case LEFT_CENTER:
					//case LEFT_BOTTOM:
					//case LEFT_BASE_LINE:
					//case LEFT_BOTTOM_BASE_LINE:
					//  break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
				case RIGHT_TOP:
				case RIGHT_CENTER:
				case RIGHT_BOTTOM:
				case RIGHT_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() - endOfLine_coords.x();
					break;
				default:
					break;
				}
				break;
			}
		case RIGHT_TO_LEFT:
			{
				switch(m_alignment)
				{
				case LEFT_TOP:
				case LEFT_CENTER:
				case LEFT_BOTTOM:
				case LEFT_BASE_LINE:
				case LEFT_BOTTOM_BASE_LINE:
					cursor.x() = 2*cursor.x() - endOfLine_coords.x();
					break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() + (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
					// nothing to be done for these
					//case RIGHT_TOP:
					//case RIGHT_CENTER:
					//case RIGHT_BOTTOM:
					//case RIGHT_BASE_LINE:
					//case RIGHT_BOTTOM_BASE_LINE:
					//  break;
				default:
					break;
				}
				break;
			}
		case VERTICAL:
			{
				switch(m_alignment)
				{
					// TODO: current behaviour top baselines lined up in both cases - need to implement
					//       top of characters alignment - Question is this necessary?
					// ... otherwise, nothing to be done for these 6 cases
					//case LEFT_TOP:
					//case CENTER_TOP:
					//case RIGHT_TOP:
					//  break;
					//case LEFT_BASE_LINE:
					//case CENTER_BASE_LINE:
					//case RIGHT_BASE_LINE:
					//  break;
				case LEFT_CENTER:
				case CENTER_CENTER:
				case RIGHT_CENTER:
					cursor.y() = cursor.y() + (cursor.y() - endOfLine_coords.y()) * 0.5f;
					break;
				case LEFT_BOTTOM_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.y() = cursor.y() - (linelength * m_characterHeight);
					break;
				case LEFT_BOTTOM:
				case CENTER_BOTTOM:
				case RIGHT_BOTTOM:
					cursor.y() = 2*cursor.y() - endOfLine_coords.y();
					break;
				default:
					break;
				}
				break;
			}
		}

		if (itr!=endOfLine_itr)
		{

			for(;itr!=endOfLine_itr;++itr,++id)
			{
				if(id>=index)
					break;
				unsigned int charcode = *itr;

				crGlyph* glyph = activefont->getGlyph(m_fontSize, charcode);
				if (glyph)
				{
					float width = (float)(glyph->getWidth()) * wr;
					float height = (float)(glyph->getHeight()) * hr;

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
								CRCore::crVector2 delta(activefont->getKerning(previous_charcode,charcode,m_kerningType));
								cursor.x() += delta.x() * wr;
								cursor.y() += delta.y() * hr;
								break;
							}
						case RIGHT_TO_LEFT:
							{
								CRCore::crVector2 delta(activefont->getKerning(charcode,previous_charcode,m_kerningType));
								cursor.x() -= delta.x() * wr;
								cursor.y() -= delta.y() * hr;
								break;
							}
						case VERTICAL:
							break; // no kerning when vertical.
						}
					}

					// move the cursor onto the next character.
					// also expand bounding box
					switch(m_layout)
					{
					case LEFT_TO_RIGHT:
						cursor.x() += glyph->getHorizontalAdvance() * wr;
						break;
					case VERTICAL:
						cursor.y() -= glyph->getVerticalAdvance() * hr;
						break;
					case RIGHT_TO_LEFT:
						break;
					}
					previous_charcode = charcode;
				}
			}

			if(id>=index)
				break;
			// skip over spaces and return.
			while (itr != m_text.end() && *itr==' ') 
			{
				++itr;
				++id;
			}
			if (itr != m_text.end() && *itr=='\n')
			{
				++itr;
				++id;
			}
		}
		else
		{
			++itr;
			++id;
		}
		// move to new line.
		switch(m_layout)
		{
		case LEFT_TO_RIGHT:
			{
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
		case RIGHT_TO_LEFT:
			{
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
		case VERTICAL:
			{
				startOfLine_coords.x() += (m_characterHeight + m_lineSpacing)/getCharacterAspectRatio();//m_characterHeight/getCharacterAspectRatio() * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				// because m_lineCount is the max vertical no. of characters....
				m_lineCount = (m_lineCount >linelength)?m_lineCount:linelength;
			}
			break;
		}

		++lineNumber;
	}
	outPos = cursor;
	return true;
}
int crText::getCursorIndex(const CRCore::crVector2& cursorPos)
{
	crFont* activefont = getActiveFont();
	if (!activefont) return 0;
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);

	if (m_text.empty()) 
	{
		return 0;
	}

	CRCore::crVector2 startOfLine_coords(0.0f,m_startCoord[1]);
	CRCore::crVector2 cursor(startOfLine_coords);
	CRCore::crVector2 local(startOfLine_coords);

	unsigned int previous_charcode = 0;
	unsigned int linelength = 0;
	bool horizontal = m_layout!=VERTICAL;
	bool kerning = true;

	unsigned int lineNumber = 0;

	float hr = m_characterHeight;
	float wr = hr/getCharacterAspectRatio();
	int id = 1;
	float hline = m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0f + m_lineSpacing);
	float xdist,ydist;
	bool incurline = false;
	for(crString::iterator itr=m_text.begin();
		itr!=m_text.end();
		)
	{
		// record the start of the current line
		crString::iterator startOfLine_itr = itr;

		if(lineNumber == 0)
		{
			cursor[0] = m_startCoord[0];
		}
		// find the end of the current line.
		CRCore::crVector2 endOfLine_coords(cursor);
		crString::iterator endOfLine_itr = computeLastCharacterOnLine(endOfLine_coords, itr,m_text.end());

		linelength = endOfLine_itr - startOfLine_itr;

		// Set line position to correct alignment.
		switch(m_layout)
		{
		case LEFT_TO_RIGHT:
			{
				switch(m_alignment)
				{
					// nothing to be done for these
					//case LEFT_TOP:
					//case LEFT_CENTER:
					//case LEFT_BOTTOM:
					//case LEFT_BASE_LINE:
					//case LEFT_BOTTOM_BASE_LINE:
					//  break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
				case RIGHT_TOP:
				case RIGHT_CENTER:
				case RIGHT_BOTTOM:
				case RIGHT_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() - endOfLine_coords.x();
					break;
				default:
					break;
				}
				break;
			}
		case RIGHT_TO_LEFT:
			{
				switch(m_alignment)
				{
				case LEFT_TOP:
				case LEFT_CENTER:
				case LEFT_BOTTOM:
				case LEFT_BASE_LINE:
				case LEFT_BOTTOM_BASE_LINE:
					cursor.x() = 2*cursor.x() - endOfLine_coords.x();
					break;
				case CENTER_TOP:
				case CENTER_CENTER:
				case CENTER_BOTTOM:
				case CENTER_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
					cursor.x() = cursor.x() + (cursor.x() - endOfLine_coords.x()) * 0.5f;
					break;
					// nothing to be done for these
					//case RIGHT_TOP:
					//case RIGHT_CENTER:
					//case RIGHT_BOTTOM:
					//case RIGHT_BASE_LINE:
					//case RIGHT_BOTTOM_BASE_LINE:
					//  break;
				default:
					break;
				}
				break;
			}
		case VERTICAL:
			{
				switch(m_alignment)
				{
					// TODO: current behaviour top baselines lined up in both cases - need to implement
					//       top of characters alignment - Question is this necessary?
					// ... otherwise, nothing to be done for these 6 cases
					//case LEFT_TOP:
					//case CENTER_TOP:
					//case RIGHT_TOP:
					//  break;
					//case LEFT_BASE_LINE:
					//case CENTER_BASE_LINE:
					//case RIGHT_BASE_LINE:
					//  break;
				case LEFT_CENTER:
				case CENTER_CENTER:
				case RIGHT_CENTER:
					cursor.y() = cursor.y() + (cursor.y() - endOfLine_coords.y()) * 0.5f;
					break;
				case LEFT_BOTTOM_BASE_LINE:
				case CENTER_BOTTOM_BASE_LINE:
				case RIGHT_BOTTOM_BASE_LINE:
					cursor.y() = cursor.y() - (linelength * m_characterHeight);
					break;
				case LEFT_BOTTOM:
				case CENTER_BOTTOM:
				case RIGHT_BOTTOM:
					cursor.y() = 2*cursor.y() - endOfLine_coords.y();
					break;
				default:
					break;
				}
				break;
			}
		}
		
		ydist = cursor[1] - cursorPos[1];
		if(ydist>= 0 && ydist<=hline)
		{
			incurline = true;
		}
		if (itr!=endOfLine_itr)
		{

			for(;itr!=endOfLine_itr;++itr,++id)
			{
				unsigned int charcode = *itr;

				crGlyph* glyph = activefont->getGlyph(m_fontSize, charcode);
				if (glyph)
				{
					float width = (float)(glyph->getWidth()) * wr;
					float height = (float)(glyph->getHeight()) * hr;

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
								CRCore::crVector2 delta(activefont->getKerning(previous_charcode,charcode,m_kerningType));
								cursor.x() += delta.x() * wr;
								cursor.y() += delta.y() * hr;
								break;
							}
						case RIGHT_TO_LEFT:
							{
								CRCore::crVector2 delta(activefont->getKerning(charcode,previous_charcode,m_kerningType));
								cursor.x() -= delta.x() * wr;
								cursor.y() -= delta.y() * hr;
								break;
							}
						case VERTICAL:
							break; // no kerning when vertical.
						}
					}
					if (incurline)
					{
						xdist = cursorPos[0] - cursor[0];
						if(xdist<=width * 0.5f)
						{
							return id-1;
						}
						else if(xdist<=width)
						{
							return id;
						}
					}
					// move the cursor onto the next character.
					// also expand bounding box
					switch(m_layout)
					{
					case LEFT_TO_RIGHT:
						cursor.x() += glyph->getHorizontalAdvance() * wr;
						break;
					case VERTICAL:
						cursor.y() -= glyph->getVerticalAdvance() * hr;
						break;
					case RIGHT_TO_LEFT:
						break;
					}
					previous_charcode = charcode;
				}
			}
			if (incurline)
			{
				return id-1;
			}
			// skip over spaces and return.
			while (itr != m_text.end() && *itr==' ') 
			{
				++itr;
				++id;
			}
			if (itr != m_text.end() && *itr=='\n')
			{
				++itr;
				++id;
			}
		}
		else
		{
			if (incurline)
			{
				return id;
			}

			++itr;
			++id;
		}

		// move to new line.
		switch(m_layout)
		{
		case LEFT_TO_RIGHT:
			{
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
		case RIGHT_TO_LEFT:
			{
				startOfLine_coords.y() -= m_characterHeight + m_lineSpacing;//m_characterHeight * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				m_lineCount++;
				break;
			}
		case VERTICAL:
			{
				startOfLine_coords.x() += (m_characterHeight + m_lineSpacing)/getCharacterAspectRatio();//m_characterHeight/getCharacterAspectRatio() * (1.0 + m_lineSpacing);
				cursor = startOfLine_coords;
				previous_charcode = 0;
				// because m_lineCount is the max vertical no. of characters....
				m_lineCount = (m_lineCount >linelength)?m_lineCount:linelength;
			}
			break;
		}

		++lineNumber;
	}
	return id;
}
bool crText::hitTest(const CRCore::crVector2& cursorPos)
{
	unsigned int numCoords = m_underLineCoords.size();
	crVector2 lineStart,lineEnd;
	for(unsigned int i=0;i<numCoords;)
	{
		lineStart = m_underLineCoords[i];
		i++;
		lineEnd = m_underLineCoords[i];
		i++;
		if(cursorPos.y()>lineStart.y() && cursorPos.y()<lineStart.y() + getCharacterHeight())
		{
			if(cursorPos.x()>lineStart.x() && cursorPos.x()<lineEnd.x())
				return true;
		}
	}
	return false;
}
// Returns false if there are no glyphs and the width/height values are invalid.
// Also sets avg_width and avg_height to 0.0f if the value is invalid.
// This method is used several times in a loop for the same object which will produce the same values. 
// Further optimization may try saving these values instead of recomputing them.
bool crText::computeAverageGlyphWidthAndHeight(float& avg_width, float& avg_height) const
{
    float width = 0.0f;
    float height = 0.0f;
    float running_width = 0.0f;
    float running_height = 0.0f;
    avg_width = 0.0f;
    avg_height = 0.0f;
    int counter = 0;
    unsigned int i;
    bool is_valid_size = true;
    // This section is going to try to compute the average width and height
    // for a character among the text. The reason I shift by an 
    // average amount per-character instead of shifting each character 
    // by its per-instance amount is because it may look strange to see 
    // the individual backdrop text letters not space themselves the same 
    // way the foreground text does. Using one value gives uniformity.
    // Note: This loop is repeated for each context. I think it may produce
    // the same values regardless of context. This code be optimized by moving
    // this loop outside the loop.
    for(TextureGlyphQuadMap::const_iterator const_titr=m_textureGlyphQuadMap.begin();
        const_titr!=m_textureGlyphQuadMap.end();
        ++const_titr)
    {
        const GlyphQuads& glyphquad = const_titr->second;
        const GlyphQuads::Coords2& coords2 = glyphquad.m_coords;
        for(i = 0; i < coords2.size(); i+=4)
        {
            width = coords2[i+2].x() - coords2[i].x();
            height = coords2[i].y() - coords2[i+1].y();

            running_width += width;
            running_height += height;
            counter++;
        }
    }
    if(0 == counter)
    {
        is_valid_size = false;
    }
    else
    {
        avg_width = running_width/counter;
        avg_height = running_height/counter;
    }
    return is_valid_size;
}


void crText::computePositions(unsigned int contextID) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    switch(m_alignment)
    {
    case LEFT_TOP:      m_offset.set(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMin()); break;
    case LEFT_CENTER:   m_offset.set(m_textBB.xMin(),(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case LEFT_BOTTOM:   m_offset.set(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin()); break;

    case CENTER_TOP:    m_offset.set((m_textBB.xMax()+m_textBB.xMin()-m_maximumWidth)*0.5f,m_textBB.yMax(),m_textBB.zMin()); break;
    case CENTER_CENTER: m_offset.set((m_textBB.xMax()+m_textBB.xMin()-m_maximumWidth)*0.5f,(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case CENTER_BOTTOM: m_offset.set((m_textBB.xMax()+m_textBB.xMin()-m_maximumWidth)*0.5f,m_textBB.yMin(),m_textBB.zMin()); break;

    case RIGHT_TOP:     m_offset.set(m_textBB.xMax()-m_maximumWidth,m_textBB.yMax(),m_textBB.zMin()); break;
    case RIGHT_CENTER:  m_offset.set(m_textBB.xMax()-m_maximumWidth,(m_textBB.yMax()+m_textBB.yMin())*0.5f,m_textBB.zMin()); break;
    case RIGHT_BOTTOM:  m_offset.set(m_textBB.xMax()-m_maximumWidth,m_textBB.yMin(),m_textBB.zMin()); break;

    case LEFT_BASE_LINE:  m_offset.set(0.0f,0.0f,0.0f); break;
    case CENTER_BASE_LINE:  m_offset.set((m_textBB.xMax()+m_textBB.xMin()-m_maximumWidth)*0.5f,0.0f,0.0f); break;
    case RIGHT_BASE_LINE:  m_offset.set(m_textBB.xMax()-m_maximumWidth,0.0f,0.0f); break;
    
    case LEFT_BOTTOM_BASE_LINE:  m_offset.set(0.0f,-m_characterHeight*(m_lineCount-1),0.0f); break;
    case CENTER_BOTTOM_BASE_LINE:  m_offset.set((m_textBB.xMax()+m_textBB.xMin()-m_maximumWidth)*0.5f,-m_characterHeight*(m_lineCount-1),0.0f); break;
    case RIGHT_BOTTOM_BASE_LINE:  m_offset.set(m_textBB.xMax()-m_maximumWidth,-m_characterHeight*(m_lineCount-1),0.0f); break;
    }
    
    AutoTransformCache& atc = m_autoTransformCache[contextID];
    CRCore::crMatrix& matrix = atc.m_matrix;

    if (m_characterSizeMode!=OBJECT_COORDS || m_autoRotateToScreen)
    {

        matrix.makeTranslate(-m_offset);

        CRCore::crMatrix rotate_matrix; 
        if (m_autoRotateToScreen) 
        {
            CRCore::crVector3d trans(atc.m_modelview.getTrans());
            atc.m_modelview.setTrans(0.0f,0.0f,0.0f);

            rotate_matrix.invert(atc.m_modelview);

            atc.m_modelview.setTrans(trans);
        }

        matrix.postMultRotate(m_rotation);

        if (m_characterSizeMode!=OBJECT_COORDS)
        {

            CRCore::crMatrix M(rotate_matrix);
            M.postMultTranslate(m_position);
            M.postMult(atc.m_modelview);
            CRCore::crMatrix& P = atc.m_projection;
            
            // compute the pixel size vector.
                        
            // pre adjust P00,P20,P23,P33 by multiplying them by the viewport window matrix.
            // here we do it in short hand with the knowledge of how the window matrix is formed
            // note P23,P33 are multiplied by an implicit 1 which would come from the window matrix.
            // Robert Osfield, June 2002.

            // scaling for horizontal pixels
            float P00 = P(0,0)*atc.m_width*0.5f;
            float P20_00 = P(2,0)*atc.m_width*0.5f + P(2,3)*atc.m_width*0.5f;
            CRCore::crVector3 scale_00(M(0,0)*P00 + M(0,2)*P20_00,
                               M(1,0)*P00 + M(1,2)*P20_00,
                               M(2,0)*P00 + M(2,2)*P20_00);

            // scaling for vertical pixels
            float P10 = P(1,1)*atc.m_height*0.5f;
            float P20_10 = P(2,1)*atc.m_height*0.5f + P(2,3)*atc.m_height*0.5f;
            CRCore::crVector3 scale_10(M(0,1)*P10 + M(0,2)*P20_10,
                               M(1,1)*P10 + M(1,2)*P20_10,
                               M(2,1)*P10 + M(2,2)*P20_10);

            float P23 = P(2,3);
            float P33 = P(3,3);

            float pixelSizeVector_w = M(3,2)*P23 + M(3,3)*P33;

            float pixelSizeVert=(m_characterHeight*sqrtf(scale_10.length2()))/(pixelSizeVector_w*0.701f);
            float pixelSizeHori=(m_characterHeight/getCharacterAspectRatio()*sqrtf(scale_00.length2()))/(pixelSizeVector_w*0.701f);

            // avoid nasty math by preventing a divide by zero
            if (pixelSizeVert == 0.0f)
               pixelSizeVert= 1.0f;
            if (pixelSizeHori == 0.0f)
               pixelSizeHori= 1.0f;

            if (m_characterSizeMode==SCREEN_COORDS)
            {
                float scale_font_vert=m_characterHeight/pixelSizeVert;
                float scale_font_hori=m_characterHeight/getCharacterAspectRatio()/pixelSizeHori;

                if (P10<0)
                   scale_font_vert=-scale_font_vert;
                matrix.postMultScale(CRCore::crVector3f(scale_font_hori, scale_font_vert,1.0f));
            }
            else if (pixelSizeVert>getFontHeight())
            {
                float scale_font = getFontHeight()/pixelSizeVert;
                matrix.postMultScale(CRCore::crVector3f(scale_font, scale_font,1.0f));
            }

        }

        if (m_autoRotateToScreen) 
        {
            matrix.postMult(rotate_matrix);
        }

        matrix.postMultTranslate(m_position);
    }
    else if (!m_rotation.zeroRotation())
    {
        matrix.makeRotate(m_rotation);
        matrix.preMultTranslate(-m_offset);
        matrix.postMultTranslate(m_position);
    }
    else
    {
        matrix.makeTranslate(m_position-m_offset);
    }

    // now apply matrix to the glyphs.
    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        GlyphQuads& glyphquad = titr->second;
        GlyphQuads::Coords2& coords2 = glyphquad.m_coords;
        GlyphQuads::Coords3& transformedCoords = glyphquad.m_transformedCoords[contextID];
        
        unsigned int numCoords = coords2.size();
        if (numCoords!=transformedCoords.size())
        {
            transformedCoords.resize(numCoords);
        }
        
        for(unsigned int i=0;i<numCoords;++i)
        {
            transformedCoords[i] = CRCore::crVector3(coords2[i].x(),coords2[i].y(),0.0f)*matrix;
        }
    }
	unsigned int numCoords = m_underLineCoords.size();
	m_underLineTCoords.resize(numCoords);
	for(unsigned int i=0;i<numCoords;++i)
	{
		m_underLineTCoords[i] = CRCore::crVector3(m_underLineCoords[i].x(),m_underLineCoords[i].y(),0.0f)*matrix;
	}
    computeBackdropPositions(contextID);

    m_normal = CRCore::crMatrix::transform3x3(CRCore::crVector3(0.0f,0.0f,1.0f),matrix);
    m_normal.normalize();

    const_cast<crText*>(this)->dirtyBound();    
}

// Presumes the atc matrix is already up-to-date
void crText::computeBackdropPositions(unsigned int contextID) const
{
    if(m_backdropType == NONE)
    {
        return;
    }

    float avg_width = 0.0f;
    float avg_height = 0.0f;
    unsigned int i;
    bool is_valid_size;
    
    AutoTransformCache& atc = m_autoTransformCache[contextID];
    CRCore::crMatrix& matrix = atc.m_matrix;

    // FIXME: OPTIMIZE: This function produces the same value regardless of contextID.
    // Since we tend to loop over contextID, we should cache this value some how
    // instead of recomputing it each time.
    is_valid_size = computeAverageGlyphWidthAndHeight(avg_width, avg_height);

    if (!is_valid_size) return;
    
    // now apply matrix to the glyphs.
    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        GlyphQuads& glyphquad = titr->second;
        GlyphQuads::Coords2& coords2 = glyphquad.m_coords;

        unsigned int backdrop_index;
        unsigned int max_backdrop_index;
        if(m_backdropType == OUTLINE)
        {
            // For outline, we want to draw the in every direction
            backdrop_index = 0;
            max_backdrop_index = 8;
        }
        else
        {
            // Yes, this may seem a little strange,
            // but since the code is using references,
            // I would have to duplicate the following code twice
            // for each part of the if/else because I can't
            // declare a reference without setting it immediately
            // and it wouldn't survive the scope.
            // So it happens that the m_backdropType value matches
            // the index in the array I want to store the coordinates
            // in. So I'll just setup the for-loop so it only does
            // the one direction I'm interested in.
            backdrop_index = m_backdropType;
            max_backdrop_index = m_backdropType+1;
        }
        for( ; backdrop_index < max_backdrop_index; backdrop_index++)
        {
            GlyphQuads::Coords3& transformedCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
            unsigned int numCoords = coords2.size();
            if (numCoords!=transformedCoords.size())
            {
                transformedCoords.resize(numCoords);
            }

            for(i=0;i<numCoords;++i)
            {
                float horizontal_shift_direction;
                float vertical_shift_direction;
                switch(backdrop_index)
                {
                    case DROP_SHADOW_BOTTOM_RIGHT:
                        {
                            horizontal_shift_direction = 1.0f;
                            vertical_shift_direction = -1.0f;
                            break;
                        }
                    case DROP_SHADOW_CENTER_RIGHT:
                        {
                            horizontal_shift_direction = 1.0f;
                            vertical_shift_direction = 0.0f;
                            break;
                        }
                    case DROP_SHADOW_TOP_RIGHT:
                        {
                            horizontal_shift_direction = 1.0f;
                            vertical_shift_direction = 1.0f;
                            break;
                        }
                    case DROP_SHADOW_BOTTOM_CENTER:
                        {
                            horizontal_shift_direction = 0.0f;
                            vertical_shift_direction = -1.0f;
                            break;
                        }
                    case DROP_SHADOW_TOP_CENTER:
                        {
                            horizontal_shift_direction = 0.0f;
                            vertical_shift_direction = 1.0f;
                            break;
                        }                                
                    case DROP_SHADOW_BOTTOM_LEFT:
                        {
                            horizontal_shift_direction = -1.0f;
                            vertical_shift_direction = -1.0f;
                            break;
                        }
                    case DROP_SHADOW_CENTER_LEFT:
                        {
                            horizontal_shift_direction = -1.0f;
                            vertical_shift_direction = 0.0f;
                            break;
                        }
                    case DROP_SHADOW_TOP_LEFT:
                        {
                            horizontal_shift_direction = -1.0f;
                            vertical_shift_direction = 1.0f;
                            break;
                        }
                    default: // error
                        {
                            horizontal_shift_direction = 1.0f;
                            vertical_shift_direction = -1.0f;
                        }
                }
                transformedCoords[i] = CRCore::crVector3(horizontal_shift_direction * m_backdropHorizontalOffset * avg_width+coords2[i].x(),vertical_shift_direction * m_backdropVerticalOffset * avg_height+coords2[i].y(),0.0f)*matrix;
            }
        }
    }
}

// This method adjusts the bounding box to account for the expanded area caused by the backdrop. 
// This assumes that the bounding box has already been computed for the text without the backdrop.
void crText::computeBackdropBoundingBox() const
{
    if(m_backdropType == NONE)
    {
        return;
    }

    float avg_width = 0.0f;
    float avg_height = 0.0f;
    bool is_valid_size;
    
    // FIXME: OPTIMIZE: It is possible that this value has already been computed before
    // from previous calls to this function. This might be worth optimizing.
    is_valid_size = computeAverageGlyphWidthAndHeight(avg_width, avg_height);

    // Finally, we have one more issue to deal with.
    // Now that the text takes more space, we need
    // to adjust the size of the bounding box.
    if((!m_textBB.valid() || !is_valid_size))
    {
        return;
    }
    
    // Finally, we have one more issue to deal with.
    // Now that the text takes more space, we need
    // to adjust the size of the bounding box.
    switch(m_backdropType)
    {
        case DROP_SHADOW_BOTTOM_RIGHT:
            {
                m_textBB.set(
                    m_textBB.xMin(),
                    m_textBB.yMin() - avg_height * m_backdropVerticalOffset,
                    m_textBB.zMin(),
                    m_textBB.xMax() + avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMax(),
                    m_textBB.zMax()
                );
                break;
            }
        case DROP_SHADOW_CENTER_RIGHT:
            {
                m_textBB.set(
                    m_textBB.xMin(),
                    m_textBB.yMin(),
                    m_textBB.zMin(),
                    m_textBB.xMax() + avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMax(),
                    m_textBB.zMax()
                );
                break;
            }
        case DROP_SHADOW_TOP_RIGHT:
            {
                m_textBB.set(
                    m_textBB.xMin(),
                    m_textBB.yMin(),
                    m_textBB.zMin(),
                    m_textBB.xMax() + avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMax() + avg_height * m_backdropVerticalOffset,
                    m_textBB.zMax()
                );
                break;
            }
        case DROP_SHADOW_BOTTOM_CENTER:
            {
                m_textBB.set(
                    m_textBB.xMin(),
                    m_textBB.yMin() - avg_height * m_backdropVerticalOffset,
                    m_textBB.zMin(),
                    m_textBB.xMax(),
                    m_textBB.yMax(),
                    m_textBB.zMax()
                );
                break;
            }
        case DROP_SHADOW_TOP_CENTER:
            {
                m_textBB.set(
                    m_textBB.xMin(),
                    m_textBB.yMin(),
                    m_textBB.zMin(),
                    m_textBB.xMax(),
                    m_textBB.yMax() + avg_height * m_backdropVerticalOffset,
                    m_textBB.zMax()
                );
                break;
            }                                
        case DROP_SHADOW_BOTTOM_LEFT:
            {
                m_textBB.set(
                    m_textBB.xMin() - avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMin() - avg_height * m_backdropVerticalOffset,
                    m_textBB.zMin(),
                    m_textBB.xMax(),
                    m_textBB.yMax(),
                    m_textBB.zMax()
                );
                break;
            }
        case DROP_SHADOW_CENTER_LEFT:
            {
                m_textBB.set(
                    m_textBB.xMin() - avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMin(),
                    m_textBB.zMin(),
                    m_textBB.xMax(),
                    m_textBB.yMax(),
                    m_textBB.zMax()
                );            break;
            }
        case DROP_SHADOW_TOP_LEFT:
            {
                m_textBB.set(
                    m_textBB.xMin() - avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMin(),
                    m_textBB.zMin(),
                    m_textBB.xMax(),
                    m_textBB.yMax() + avg_height * m_backdropVerticalOffset,
                    m_textBB.zMax()
                );
                break;
            }
        case OUTLINE:
            {
                m_textBB.set(
                    m_textBB.xMin() - avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMin() - avg_height * m_backdropVerticalOffset,
                    m_textBB.zMin(),
                    m_textBB.xMax() + avg_width * m_backdropHorizontalOffset,
                    m_textBB.yMax() + avg_height * m_backdropVerticalOffset,
                    m_textBB.zMax()
                );
                break;
            }
        default: // error
            {
                break;
            }
    }
}

// This method expands the bounding box to a settable margin when a bounding box drawing mode is active. 
void crText::computeBoundingBoxMargin() const
{
    if(m_drawMode & (BOUNDINGBOX | FILLEDBOUNDINGBOX)){
        m_textBB.set(
            m_textBB.xMin() - m_textBBMargin,
            m_textBB.yMin() - m_textBBMargin,
            m_textBB.zMin(),
            m_textBB.xMax() + m_textBBMargin,
            m_textBB.yMax() + m_textBBMargin,
            m_textBB.zMax()
        );
    }
}

void crText::computeColorGradients() const
{
    switch(m_colorGradientMode)
    {
        case SOLID:
            return;
            break;
        case PER_CHARACTER:
            computeColorGradientsPerCharacter();
            break;
        case OVERALL:
            computeColorGradientsOverall();
            break;
        default:
            break;
    }
}

void crText::computeColorGradientsOverall() const
{

    float min_x = FLT_MAX;
    float min_y = FLT_MAX;
    float max_x = FLT_MIN;
    float max_y = FLT_MIN;

    unsigned int i;

    for(TextureGlyphQuadMap::const_iterator const_titr=m_textureGlyphQuadMap.begin();
        const_titr!=m_textureGlyphQuadMap.end();
        ++const_titr)
    {
        const GlyphQuads& glyphquad = const_titr->second;
        const GlyphQuads::Coords2& coords2 = glyphquad.m_coords;

        for(i=0;i<coords2.size();++i)
        {  
            // Min and Max are needed for color gradients
            if(coords2[i].x() > max_x)
            {
                max_x = coords2[i].x();
            }
            if(coords2[i].x() < min_x)
            {
                min_x = coords2[i].x();
            }
            if(coords2[i].y() > max_y)
            {
                max_y = coords2[i].y();
            }
            if(coords2[i].y() < min_y)
            {
                min_y = coords2[i].y();
            }        

        }
    }

    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        GlyphQuads& glyphquad = titr->second;
        GlyphQuads::Coords2& coords2 = glyphquad.m_coords;
        GlyphQuads::ColorCoords& colorCoords = glyphquad.m_colorCoords;

        unsigned int numCoords = coords2.size();
        if (numCoords!=colorCoords.size())
        {
            colorCoords.resize(numCoords);
        }

        for(i=0;i<numCoords;++i)
        {
            float red = bilinearInterpolate(
                min_x,
                max_x,
                min_y,
                max_y,
                coords2[i].x(),
                coords2[i].y(),
                m_colorGradientBottomLeft[0],
                m_colorGradientTopLeft[0],
                m_colorGradientBottomRight[0],
                m_colorGradientTopRight[0]
            );

            float green = bilinearInterpolate(
                min_x,
                max_x,
                min_y,
                max_y,
                coords2[i].x(),
                coords2[i].y(),
                m_colorGradientBottomLeft[1],
                m_colorGradientTopLeft[1],
                m_colorGradientBottomRight[1],
                m_colorGradientTopRight[1]
            );

            float blue = bilinearInterpolate(
                min_x,
                max_x,
                min_y,
                max_y,
                coords2[i].x(),
                coords2[i].y(),
                m_colorGradientBottomLeft[2],
                m_colorGradientTopLeft[2],
                m_colorGradientBottomRight[2],
                m_colorGradientTopRight[2]
            );
            // Alpha does not convert to HSV            
            float alpha = bilinearInterpolate(
                min_x,
                max_x,
                min_y,
                max_y,
                coords2[i].x(),
                coords2[i].y(),
                m_colorGradientBottomLeft[3],
                m_colorGradientTopLeft[3],
                m_colorGradientBottomRight[3],
                m_colorGradientTopRight[3]
            );                                    

            colorCoords[i] = CRCore::crVector4(red,green,blue,alpha);
        }
    }
}

void crText::computeColorGradientsPerCharacter() const
{
    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        GlyphQuads& glyphquad = titr->second;
        GlyphQuads::Coords2& coords2 = glyphquad.m_coords;
        GlyphQuads::ColorCoords& colorCoords = glyphquad.m_colorCoords;

        unsigned int numCoords = coords2.size();
        if (numCoords!=colorCoords.size())
        {
            colorCoords.resize(numCoords);
        }

        for(unsigned int i=0;i<numCoords;++i)
        {
            switch(i%4)
            {
                case 0: // top-left
                    {
                        colorCoords[i] = m_colorGradientTopLeft;
                        break;
                    }
                case 1: // bottom-left
                    {
                        colorCoords[i] = m_colorGradientBottomLeft;
                        break;
                    }
                case 2: // bottom-right
                    {
                        colorCoords[i] = m_colorGradientBottomRight;
                        break;
                    }
                case 3: // top-right
                    {
                        colorCoords[i] = m_colorGradientTopRight;
                        break;
                    }
                default: // error
                    {
                        colorCoords[i] = CRCore::crVector4(0.0f,0.0f,0.0f,1.0f);
                    }
            }
        }
    }
}

//void crText::drawImplementation(CRCore::crState& state) const
//{
//    drawImplementation(state, CRCore::crVector4(1.0f,1.0f,1.0f,1.0f));
//}

void crText::drawImplementation(CRCore::crState& state) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    unsigned int contextID = state.getContextID();
	m_backdropColor.a() = m_color.a();
	//state.applyMode(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB, false);
    //state.applyMode(GL_BLEND,true);
//#if 1
//    state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::ON);
//#else
//    state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);
//#endif
//#if defined(GL_FIXED_FUNCTION_AVAILABLE)
	//if(getActiveFont()->getTexEnv())
	//	state.applyTextureAttribute(0,getActiveFont()->getTexEnv());
//#endif
    //if (m_characterSizeMode!=OBJECT_COORDS || m_autoRotateToScreen)
    //{
    //    unsigned int frameNumber = state.getFrameStamp()?state.getFrameStamp()->getFrameNumber():0;
    //    AutoTransformCache& atc = m_autoTransformCache[contextID];
    //    const CRCore::crMatrix& modelview = state.getModelViewMatrix();
    //    const CRCore::crMatrix& projection = state.getProjectionMatrix();

    //    CRCore::crVector3 newTransformedPosition = m_position*modelview;

    //    int width = atc.m_width;
    //    int height = atc.m_height;

    //    const CRCore::crViewPort* viewport = state.getCurrentViewport();
    //    if (viewport)
    //    {
    //        width = static_cast<int>(viewport->width());
    //        height = static_cast<int>(viewport->height());
    //    }

    //    bool doUpdate = atc.m_traversalNumber==-1;
    //    if (atc.m_traversalNumber>=0)
    //    {
    //        if (atc.m_modelview!=modelview)
    //        {
    //            doUpdate = true;
    //        }
    //        else if (width!=atc.m_width || height!=atc.m_height)
    //        {
    //            doUpdate = true;
    //        }
    //        else if (atc.m_projection!=projection)
    //        {
    //            doUpdate = true;
    //        }
    //    }
    //    
    //    atc.m_traversalNumber = frameNumber;
    //    atc.m_width = width;
    //    atc.m_height = height;
    //    
    //    if (doUpdate)
    //    {    
    //        atc.m_transformedPosition = newTransformedPosition;
    //        atc.m_projection = projection;
    //        atc.m_modelview = modelview;

    //        computePositions(contextID);
    //    }
    //    
    //}
    
    
    // Ensure that the glyph coordinates have been transformed for
    // this context id.

    //if ( !m_textureGlyphQuadMap.empty() )
    //{
    //    const GlyphQuads& glyphquad = (m_textureGlyphQuadMap.begin())->second;
    //    if ( glyphquad.m_transformedCoords[contextID].empty() )
    //    {
    //        computePositions(contextID);
    //    }
    //}

    //CRCore::GLBeginEndAdapter& gl = (state.getGLBeginEndAdapter());

    glNormal3fv(m_normal.ptr());

    if (m_drawMode & FILLEDBOUNDINGBOX)
    {
        if (m_textBB.valid())
        {
        #if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
            state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);

            const CRCore::crMatrix& matrix = m_autoTransformCache[contextID].m_matrix;

            CRCore::crVector3 c00(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c10(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMin(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c11(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c01(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMin())*matrix);

            switch(m_backdropImplementation)
            {
                case NO_DEPTH_BUFFER:
                    // Do nothing.  The bounding box will be rendered before the text and that's all that matters.
                    break;
                case DEPTH_RANGE:
                    glPushAttrib(GL_DEPTH_BUFFER_BIT);
                    //unsigned int backdrop_index = 0;
                    //unsigned int max_backdrop_index = 8;
                    //const double offset = double(max_backdrop_index - backdrop_index) * 0.003;
                    glDepthRange(0.001, 1.001);
                    break;
                /*case STENCIL_BUFFER:
                    break;*/
                default:
                    glPushAttrib(GL_POLYGON_OFFSET_FILL);
                    glEnable(GL_POLYGON_OFFSET_FILL);
                    glPolygonOffset(0.1f, 10.0f);
            }

            //gl.Color4f(colorMultiplier.r()*m_textBBColor.r(),colorMultiplier.g()*m_textBBColor.g(),colorMultiplier.b()*m_textBBColor.b(),colorMultiplier.a()*m_textBBColor.a());
			state.applyColor(m_textBBColor);
			//unsigned int contextID = state.getContextID();
			//for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
			//	titr!=m_textureGlyphQuadMap.end();
			//	++titr)
			//{
			//	const GlyphQuads& glyphquad = titr->second;
			//	const GlyphQuads::Coords3& transformedCoords = glyphquad.m_transformedCoords[contextID];
			//	if (!transformedCoords.empty()) 
			//	{
			//		state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedCoords.front()));
			//		state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
			//		state.disableColorPointer();
			//		//state.drawQuads(0,transformedCoords.size());
			//		glDrawArrays(GL_QUADS,0,transformedCoords.size());
			//	}
			//}
			glBegin(GL_QUADS);
                glVertex3fv(c00.ptr());
                glVertex3fv(c10.ptr());
                glVertex3fv(c11.ptr());
                glVertex3fv(c01.ptr());
            glEnd();

            switch(m_backdropImplementation)
            {
                case NO_DEPTH_BUFFER:
                    // Do nothing.
                    break;
                case DEPTH_RANGE:
                    glDepthRange(0.0, 1.0);
                    glPopAttrib();
                    break;
                /*case STENCIL_BUFFER:
                    break;*/
                default:
                    glDisable(GL_POLYGON_OFFSET_FILL);
                    glPopAttrib();
            }
        #else
            CRCore::notify(CRCore::NOTICE)<<"Warning: crText::drawImplementation() fillMode FILLEDBOUNDINGBOX not supported"<<std::endl;
        #endif
        }
    }    

#if 1
    state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::ON);
#else
    state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);
#endif
//#if defined(GL_FIXED_FUNCTION_AVAILABLE)
	//if(getActiveFont()->getTexEnv())
	//	state.applyTextureAttribute(0,getActiveFont()->getTexEnv());
//#endif

    if (m_drawMode & TEXT)
    {

        state.disableAllVertexArrays();

        // Okay, since ATI's cards/drivers are not working correctly,
        // we need alternative solutions to glPolygonOffset.
        // So this is a pick your poison approach. Each alternative
        // backend has trade-offs associated with it, but with luck,
        // the user may find that works for them.
        if(m_backdropType != NONE && m_backdropImplementation != DELAYED_DEPTH_WRITES)
        {
            switch(m_backdropImplementation)
            {
                case POLYGON_OFFSET:
                    renderWithPolygonOffset(state);
                    break;
                case NO_DEPTH_BUFFER:
                    renderWithNoDepthBuffer(state);
                    break;
                case DEPTH_RANGE:
                    renderWithDepthRange(state);
                    break;
                case STENCIL_BUFFER:
                    renderWithStencilBuffer(state);
                    break;
                default:
                    renderWithPolygonOffset(state);
            }
        }
        else
        {
            //renderWithDelayedDepthWrites(state);
			drawTextWithBackdrop(state);
        }
		if(m_drawMode & UNDERLINE)
		{
			if (!m_underLineTCoords.empty()) 
			{
				state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);
				state.disableTexCoordPointersAboveAndIncluding(0);

				state.setVertexPointer( 3, GL_FLOAT, 0, &(m_underLineTCoords.front()));
				glDrawArrays(GL_LINES,0,m_underLineTCoords.size());
			}
		}
    }

    if (m_drawMode & BOUNDINGBOX)
    {
        if (m_textBB.valid())
        {
            state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);

            const CRCore::crMatrix& matrix = m_autoTransformCache[contextID].m_matrix;

            CRCore::crVector3 c00(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c10(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMin(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c11(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMin())*matrix);
            CRCore::crVector3 c01(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMax(),m_textBB.zMin())*matrix);

        
            //gl.Color4f(colorMultiplier.r()*m_textBBColor.r(),colorMultiplier.g()*m_textBBColor.g(),colorMultiplier.b()*m_textBBColor.b(),colorMultiplier.a()*m_textBBColor.a());
			state.applyColor(m_textBBColor);
			glBegin(GL_LINE_LOOP);
                glVertex3fv(c00.ptr());
                glVertex3fv(c10.ptr());
                glVertex3fv(c11.ptr());
                glVertex3fv(c01.ptr());
            glEnd();
        }
    }

    if (m_drawMode & ALIGNMENT)
    {
        //gl.Color4fv(colorMultiplier.ptr());
		state.applyColor(m_textBBColor);
        float cursorsize = m_characterHeight*0.5f;

        const CRCore::crMatrix& matrix = m_autoTransformCache[contextID].m_matrix;

        CRCore::crVector3 hl(CRCore::crVector3(m_offset.x()-cursorsize,m_offset.y(),m_offset.z())*matrix);
        CRCore::crVector3 hr(CRCore::crVector3(m_offset.x()+cursorsize,m_offset.y(),m_offset.z())*matrix);
        CRCore::crVector3 vt(CRCore::crVector3(m_offset.x(),m_offset.y()-cursorsize,m_offset.z())*matrix);
        CRCore::crVector3 vb(CRCore::crVector3(m_offset.x(),m_offset.y()+cursorsize,m_offset.z())*matrix);

        state.applyTextureMode(0,GL_TEXTURE_2D,CRCore::crStateAttribute::OFF);
        
        glBegin(GL_LINES);
            glVertex3fv(hl.ptr());
            glVertex3fv(hr.ptr());
            glVertex3fv(vt.ptr());
            glVertex3fv(vb.ptr());
        glEnd();
        
    }
}

void crText::accept(CRCore::crDrawable::ConstAttributeFunctor& af) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    for(TextureGlyphQuadMap::const_iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        const GlyphQuads& glyphquad = titr->second;
        af.apply(CRCore::crDrawable::VERTICES,glyphquad.m_transformedCoords[0].size(),&(glyphquad.m_transformedCoords[0].front()));
        af.apply(CRCore::crDrawable::TEXTURE_COORDS_0,glyphquad.m_texcoords.size(),&(glyphquad.m_texcoords.front()));
    }
}

void crText::accept(CRCore::crDrawable::PrimitiveFunctor& pf) const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    for(TextureGlyphQuadMap::const_iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        const GlyphQuads& glyphquad = titr->second;

        pf.setVertexArray(glyphquad.m_transformedCoords[0].size(),&(glyphquad.m_transformedCoords[0].front()));
        pf.drawArrays(GL_QUADS,0,glyphquad.m_transformedCoords[0].size());
            
    }
    
}


//void crText::setThreadSafeRefUnref(bool threadSafe)
//{
//    crTextBase::setThreadSafeRefUnref(threadSafe);
//
//    getActiveFont()->setThreadSafeRefUnref(threadSafe);
//}

//void crText::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    crTextBase::resizeGLObjectBuffers(maxSize);
//    
//    getActiveFont()->resizeGLObjectBuffers(maxSize);
//}


void crText::releaseObjects(CRCore::crState* state)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	if (m_font.valid())
	{
		m_font->releaseObjects(state);
		m_font = NULL;
	}
    crTextBase::releaseObjects(state);
 //   crFont *font = getActiveFont();
	//if(font) font->releaseObjects(state);
}


void crText::setBackdropType(BackdropType type)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_backdropType==type) return;

    m_backdropType = type;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crText::setBackdropImplementation(BackdropImplementation implementation)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_backdropImplementation==implementation) return;

    m_backdropImplementation = implementation;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}


void crText::setBackdropOffset(float offset)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_backdropHorizontalOffset = offset;
    m_backdropVerticalOffset = offset;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crText::setBackdropOffset(float horizontal, float vertical)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_backdropHorizontalOffset = horizontal;
    m_backdropVerticalOffset = vertical;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crText::setBackdropColor(const CRCore::crVector4& color)
{
    m_backdropColor = color;
}

void crText::setColorGradientMode(ColorGradientMode mode)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_colorGradientMode==mode) return;

    m_colorGradientMode = mode;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crText::setColorGradientCorners(const CRCore::crVector4& topLeft, const CRCore::crVector4& bottomLeft, const CRCore::crVector4& bottomRight, const CRCore::crVector4& topRight)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_colorGradientTopLeft = topLeft;
    m_colorGradientBottomLeft = bottomLeft;
    m_colorGradientBottomRight = bottomRight;
    m_colorGradientTopRight = topRight;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

// Formula for f(x,y) from Wikipedia "Bilinear interpolation", 2006-06-18
float crText::bilinearInterpolate(float x1, float x2, float y1, float y2, float x, float y, float q11, float q12, float q21, float q22) const
{
    return (
        ((q11 / ((x2-x1)*(y2-y1))) * (x2-x)*(y2-y))
        + ((q21 / ((x2-x1)*(y2-y1))) * (x-x1)*(y2-y))
        + ((q12 / ((x2-x1)*(y2-y1))) * (x2-x)*(y-y1))
        + ((q22 / ((x2-x1)*(y2-y1))) * (x-x1)*(y-y1))
    );
}

void crText::drawForegroundText(CRCore::crState& state, const GlyphQuads& glyphquad) const
{
    unsigned int contextID = state.getContextID();

    const GlyphQuads::Coords3& transformedCoords = glyphquad.m_transformedCoords[contextID];
    if (!transformedCoords.empty()) 
    {
        state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedCoords.front()));
        state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));

        if(m_colorGradientMode == SOLID)
        {
            state.disableColorPointer();
			state.applyColor(m_color);
        }
        else
        {
            state.setColorPointer( 4, GL_FLOAT, 0, &(glyphquad.m_colorCoords.front()));
        }

        //state.drawQuads(0,transformedCoords.size());
		glDrawArrays(GL_QUADS,0,transformedCoords.size());
		glDrawArrays(GL_QUADS,0,transformedCoords.size());
    }
}

void crText::renderOnlyForegroundText(CRCore::crState& state) const
{
    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

        drawForegroundText(state, glyphquad);
    }
}

void crText::renderWithDelayedDepthWrites(CRCore::crState& state) const
{
    //glPushAttrib( m_enableDepthWrites ? (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) : GL_DEPTH_BUFFER_BIT);
    // Render to color buffer without writing to depth buffer.
    glDepthMask(GL_FALSE);
    drawTextWithBackdrop(state);

    // Render to depth buffer if depth writes requested.
    if( m_enableDepthWrites )
    {
        glDepthMask(GL_TRUE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        drawTextWithBackdrop(state);
    }

    state.haveAppliedAttribute(CRCore::crStateAttribute::DEPTH);
    state.haveAppliedAttribute(CRCore::crStateAttribute::COLORMASK);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glPopAttrib();
}

void crText::drawTextWithBackdrop(CRCore::crState& state) const
{
    unsigned int contextID = state.getContextID();

    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;
        if(m_backdropType != NONE)
        {
            unsigned int backdrop_index;
            unsigned int max_backdrop_index;
            if(m_backdropType == OUTLINE)
            {
                backdrop_index = 0;
                max_backdrop_index = 8;
            }
            else
            {
                backdrop_index = m_backdropType;
                max_backdrop_index = m_backdropType+1;
            }

            state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
            state.disableColorPointer();
            state.applyColor(m_backdropColor);

            for( ; backdrop_index < max_backdrop_index; backdrop_index++)
            {
                const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
                if (!transformedBackdropCoords.empty()) 
                {
                    state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
                    //state.drawQuads(0,transformedBackdropCoords.size());
					glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
                }
            }
        }

		drawForegroundText(state, glyphquad);
    }
}


void crText::renderWithPolygonOffset(CRCore::crState& state) const
{
#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
    unsigned int contextID = state.getContextID();

    // Do I really need to do this for glPolygonOffset?
    glPushAttrib(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);

    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

        unsigned int backdrop_index;
        unsigned int max_backdrop_index;
        if(m_backdropType == OUTLINE)
        {
            backdrop_index = 0;
            max_backdrop_index = 8;
        }
        else
        {
            backdrop_index = m_backdropType;
            max_backdrop_index = m_backdropType+1;
        }

        state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
        state.disableColorPointer();
        state.applyColor(m_backdropColor);

        for( ; backdrop_index < max_backdrop_index; backdrop_index++)
        {
            const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
            if (!transformedBackdropCoords.empty()) 
            {
                state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
                glPolygonOffset(0.1f,max_backdrop_index-backdrop_index);
				glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
                //state.drawQuads(0,transformedBackdropCoords.size());
            }
        }

        // Reset the polygon offset so the foreground text is on top
        glPolygonOffset(0.0f,0.0f);

        drawForegroundText(state, glyphquad);
    }

    glPopAttrib();
#else
    CRCore::notify(CRCore::NOTICE)<<"Warning: crText::renderWithPolygonOffset(..) not implemented."<<std::endl;
#endif
}
    

void crText::renderWithNoDepthBuffer(CRCore::crState& state) const
{
#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
    unsigned int contextID = state.getContextID();

    glPushAttrib(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

		if(m_backdropType != NONE)
		{
			unsigned int backdrop_index;
			unsigned int max_backdrop_index;
			if(m_backdropType == OUTLINE)
			{
				backdrop_index = 0;
				max_backdrop_index = 8;
			}
			else
			{
				backdrop_index = m_backdropType;
				max_backdrop_index = m_backdropType+1;
			}

			state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
			state.disableColorPointer();
			state.applyColor(m_backdropColor);

			for( ; backdrop_index < max_backdrop_index; backdrop_index++)
			{
				const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
				if (!transformedBackdropCoords.empty()) 
				{
					state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
					//state.drawQuads(0,transformedBackdropCoords.size());
					glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
				}
			}
		}
        drawForegroundText(state, glyphquad);
    }

    glPopAttrib();
#else
    CRCore::notify(CRCore::NOTICE)<<"Warning: crText::renderWithNoDepthBuffer(..) not implemented."<<std::endl;
#endif
}

// This idea comes from Paul Martz's OpenGL FAQ: 13.050
void crText::renderWithDepthRange(CRCore::crState& state) const
{
#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
    unsigned int contextID = state.getContextID();

    // Hmmm, the man page says GL_VIEWPORT_BIT for Depth range (near and far)
    // but experimentally, GL_DEPTH_BUFFER_BIT for glDepthRange.
//    glPushAttrib(GL_VIEWPORT_BIT);
    glPushAttrib(GL_DEPTH_BUFFER_BIT);

    for(TextureGlyphQuadMap::iterator titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

        unsigned int backdrop_index;
        unsigned int max_backdrop_index;
        if(m_backdropType == OUTLINE)
        {
            backdrop_index = 0;
            max_backdrop_index = 8;
        }
        else
        {
            backdrop_index = m_backdropType;
            max_backdrop_index = m_backdropType+1;
        }

        state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
        state.disableColorPointer();
        state.applyColor(m_backdropColor);

        for( ; backdrop_index < max_backdrop_index; backdrop_index++)
        {
            const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
            if (!transformedBackdropCoords.empty()) 
            {
                state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
                double offset = double(max_backdrop_index-backdrop_index)*0.0001;
                glDepthRange( offset, 1.0+offset);
                //state.drawQuads(0,transformedBackdropCoords.size());
				glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
            }
        }

        glDepthRange(0.0, 1.0);

        drawForegroundText(state, glyphquad);
    }

    glPopAttrib();
#else
    CRCore::notify(CRCore::NOTICE)<<"Warning: crText::renderWithDepthRange(..) not implemented."<<std::endl;
#endif
}

void crText::renderWithStencilBuffer(CRCore::crState& state) const
{
#if !defined(GLES1_AVAILABLE) && !defined(GLES2_AVAILABLE) && !defined(GL3_AVAILABLE)
    /* Here are the steps:
     * 1) Disable drawing color
     * 2) Enable the stencil buffer
     * 3) Draw all the text to the stencil buffer
     * 4) Disable the stencil buffer
     * 5) Enable color
     * 6) Disable the depth buffer
     * 7) Draw all the text again.
     * 7b) Make sure the foreground text is drawn last if priority levels
     * are the same OR
     * 7c) If priority levels are different, then make sure the foreground
     * text has the higher priority.
     */
    unsigned int contextID = state.getContextID();
    TextureGlyphQuadMap::iterator titr; // Moved up here for VC6
    
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_STENCIL_TEST);

    // It seems I can get away without calling this here
    //glClear(GL_STENCIL_BUFFER_BIT);

    // enable stencil buffer
    glEnable(GL_STENCIL_TEST);

    // write a one to the stencil buffer everywhere we are about to draw
    glStencilFunc(GL_ALWAYS, 1, 1);

    // write only to the stencil buffer if we pass the depth test
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Disable writing to the color buffer so we only write to the stencil 
    // buffer and the depth buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // make sure the depth buffer is enabled
//    glEnable(GL_DEPTH_TEST);
//    glDepthMask(GL_TRUE);
//    glDepthFunc(GL_LESS);

    // Arrrgh! Why does the code only seem to work correctly if I call this?
    glDepthMask(GL_FALSE);
    

    // Draw all the text to the stencil buffer to mark out the region
    // that we can write too.
    
    for(titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

        unsigned int backdrop_index;
        unsigned int max_backdrop_index;
        if(m_backdropType == OUTLINE)
        {
            backdrop_index = 0;
            max_backdrop_index = 8;
        }
        else
        {
            backdrop_index = m_backdropType;
            max_backdrop_index = m_backdropType+1;
        }

        state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
        state.disableColorPointer();

        for( ; backdrop_index < max_backdrop_index; backdrop_index++)
        {
            const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
            if (!transformedBackdropCoords.empty()) 
            {
                state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
                //state.drawQuads(0,transformedBackdropCoords.size());
				glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
            }
        }

        // Draw the foreground text
        const GlyphQuads::Coords3& transformedCoords = glyphquad.m_transformedCoords[contextID];
        if (!transformedCoords.empty()) 
        {
            state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedCoords.front()));
            state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
            //state.drawQuads(0,transformedCoords.size());
			glDrawArrays(GL_QUADS,0,transformedCoords.size());
        }
    }


    // disable the depth buffer
//    glDisable(GL_DEPTH_TEST);
//    glDepthMask(GL_FALSE);
//    glDepthMask(GL_TRUE);
//    glDepthFunc(GL_ALWAYS);

    // Set the stencil function to pass when the stencil is 1
    // Bug: This call seems to have no effect. Try changing to NOTEQUAL
    // and see the exact same results.
    glStencilFunc(GL_EQUAL, 1, 1);

    // disable writing to the stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(GL_FALSE);

    // Re-enable writing to the color buffer so we can see the results
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    // Draw all the text again

    for(titr=m_textureGlyphQuadMap.begin();
        titr!=m_textureGlyphQuadMap.end();
        ++titr)
    {
        // need to set the texture here...
        state.applyTextureAttribute(0,titr->first.get());

        const GlyphQuads& glyphquad = titr->second;

        unsigned int backdrop_index;
        unsigned int max_backdrop_index;
        if(m_backdropType == OUTLINE)
        {
            backdrop_index = 0;
            max_backdrop_index = 8;
        }
        else
        {
            backdrop_index = m_backdropType;
            max_backdrop_index = m_backdropType+1;
        }

        state.setTexCoordPointer( 0, 2, GL_FLOAT, 0, &(glyphquad.m_texcoords.front()));
        state.disableColorPointer();
        state.applyColor(m_backdropColor);

        for( ; backdrop_index < max_backdrop_index; backdrop_index++)
        {
            const GlyphQuads::Coords3& transformedBackdropCoords = glyphquad.m_transformedBackdropCoords[backdrop_index][contextID];
            if (!transformedBackdropCoords.empty()) 
            {
                state.setVertexPointer( 3, GL_FLOAT, 0, &(transformedBackdropCoords.front()));
                //state.drawQuads(0,transformedBackdropCoords.size());
				glDrawArrays(GL_QUADS,0,transformedBackdropCoords.size());
            }
        }

        drawForegroundText(state, glyphquad);
    }

    glPopAttrib();
#else
    CRCore::notify(CRCore::NOTICE)<<"Warning: crText::renderWithStencilBuffer(..) not implemented."<<std::endl;
#endif
}