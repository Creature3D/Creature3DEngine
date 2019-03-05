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
#include <CRText/crTextBase.h>
#include <CRText/crDefaultFont.h>

#include <CRCore/crMath.h>
#include <crgl/gl.h>
#include <CRCore/crNotify.h>
#include <CRCore/crPolygonOffset.h>
#include <CRCore/crTexEnv.h>
#include <CRCore/crAlphaFunc.h>
#include <CRCore/crBlendFunc.h>
//#include <CRUtil/crCullVisitor.h>

#include <CRIOManager/crReadFile.h>

using namespace CRCore;
using namespace CRText;

//#define TREES_CODE_FOR_MAKING_SPACES_EDITABLE

crTextBase::crTextBase():
    //m_color(1.0f,1.0f,1.0f,1.0f),
    m_fontSize(32,32),
    m_characterHeight(32),
    m_characterSizeMode(OBJECT_COORDS),
    m_maximumWidth(0.0f),
    m_maximumHeight(0.0f),
    m_lineSpacing(3.0f),
	m_underLineSpacing(3.0f),
    m_alignment(BASE_LINE),
    m_axisAlignment(XY_PLANE),
    m_autoRotateToScreen(false),
    m_layout(LEFT_TO_RIGHT),
    m_drawMode(TEXT),
    m_textBBMargin(0.0f),
    m_textBBColor(0.0, 0.0, 0.0, 0.5),
    m_kerningType(KERNING_UNFITTED),
    m_lineCount(0),
	m_needReCompute(0)
{
    //setStateSet(crDefaultFont::instance()->getStateSet());
    setUseDisplayList(false);
    setSupportsDisplayList(false);
	m_type = TEXTDRAWABLE;
	//crAlphaFunc *alphaFunc = new crAlphaFunc;
	//alphaFunc->setFunction(crAlphaFunc::GREATER,0.3333f);
	//getOrCreateStateSet()->setAttributeAndModes(alphaFunc,crStateAttribute::ON);
	//getOrCreateStateSet()->setMode(GL_BLEND, crStateAttribute::OFF);
	//crBlendFunc *blendFunc = new crBlendFunc(crBlendFunc::SRC_ALPHA,crBlendFunc::ZERO);
	//getOrCreateStateSet()->setAttributeAndModes(blendFunc,crStateAttribute::ON);
	//newStateSet->setRenderingHint(CRCore::crStateSet::ALPHATEST_BIN);
}

crTextBase::crTextBase(const crTextBase& textBase,const CRCore::crCopyOp& copyop):
    CRCore::crDrawable(textBase,copyop),
    //m_color(textBase.m_color),
    //m_font(textBase.m_font),
    m_style(textBase.m_style),
    m_fontSize(textBase.m_fontSize),
    m_characterHeight(textBase.m_characterHeight),
    m_characterSizeMode(textBase.m_characterSizeMode),
    m_maximumWidth(textBase.m_maximumWidth),
    m_maximumHeight(textBase.m_maximumHeight),
    m_lineSpacing(textBase.m_lineSpacing),
	m_underLineSpacing(textBase.m_underLineSpacing),
    m_text(textBase.m_text),
    m_position(textBase.m_position),
    m_alignment(textBase.m_alignment),
    m_axisAlignment(textBase.m_axisAlignment),
    m_rotation(textBase.m_rotation),
    m_autoRotateToScreen(textBase.m_autoRotateToScreen),
    m_layout(textBase.m_layout),
    m_drawMode(textBase.m_drawMode),
    m_textBBMargin(textBase.m_textBBMargin),
    m_textBBColor(textBase.m_textBBColor),
    m_kerningType(textBase.m_kerningType),
    m_lineCount(textBase.m_lineCount),
	m_startCoord(textBase.m_startCoord),
	m_endCoord(textBase.m_endCoord)
{
	//computeGlyphRepresentation();
	m_needReCompute = 1;
}

crTextBase::~crTextBase()
{
}

//void crTextBase::setColor(const CRCore::crVector4& color)
//{
//    m_color = color;
//}


void crTextBase::setFont(crFont *font)
{
    //if (m_font==font) return;

	//CRCore::crStateSet* previousFontStateSet = m_font.valid() ? m_font->getStateSet() : crDefaultFont::instance()->getStateSet();
	//CRCore::crStateSet* newFontStateSet = font ? font->getStateSet() : crDefaultFont::instance()->getStateSet();

	//if (getStateSet() == previousFontStateSet)
	//{
	//    setStateSet( newFontStateSet );
	//}
	//setStateSet( newFontStateSet );
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_font = font;
	if(m_font.valid())
	{
		crFont::FontImplementation* imp = m_font->getImplementation();
		if(imp) imp->m_facade = m_font.get();
	}
    //computeGlyphRepresentation();
	//m_needReCompute = 1;
}

void crTextBase::setFont(const std::string& fontfile)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	if (m_fontName==fontfile) return;
	m_fontName = fontfile;
    //setFont(readFontFile(fontfile));
	//CRCore::ref_ptr<crFont> font;
	//readFontFile(font, m_fontName);
	//setFont(font.get());
	m_needReCompute = 1;
}

void crTextBase::setFontResolution(unsigned int width, unsigned int height)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	//m_fontSize = FontResolution((unsigned int)((float)width+1.0f),(unsigned int)((float)height+1.0f));
	m_fontSize = FontResolution(width,height);
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setCharacterSize(float height)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	m_characterHeight = height;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setCharacterSize(float height, float aspectRatio)
{
    if (getCharacterAspectRatio()!=aspectRatio)
    {
        getOrCreateStyle()->setWidthRatio(aspectRatio);
    }
    setCharacterSize(height);
}

void crTextBase::setMaximumWidth(float maximumWidth)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_maximumWidth = maximumWidth;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void  crTextBase::setMaximumHeight(float maximumHeight)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_maximumHeight = maximumHeight;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setLineSpacing(float lineSpacing)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	m_lineSpacing = lineSpacing;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}
    
void crTextBase::setUnderLineSpacing(float underLineSpacing)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	m_underLineSpacing = underLineSpacing;
	//computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setText(const crString& text)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_text==text) return;
    m_text = text;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setText(const std::string& text)
{
    setText(crString(text));
}

void crTextBase::setText(const std::string& text,crString::Encoding encoding)
{
    setText(crString(text,encoding));
}
    

void crTextBase::setText(const wchar_t* text)
{
    setText(crString(text));
}

void crTextBase::setPosition(const CRCore::crVector3& pos)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_position==pos) return;

    m_position = pos;
    //computePositions();
	if(m_needReCompute == 0) m_needReCompute = 2;
}

void crTextBase::setAlignment(AlignmentType alignment)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_alignment==alignment) return;
    
    m_alignment = alignment;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

void crTextBase::setAxisAlignment(AxisAlignment axis)
{
    m_axisAlignment = axis;

    switch(axis)
    {
    case XZ_PLANE:
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(1.0f,0.0f,0.0f))); 
        break;
    case REVERSED_XZ_PLANE:
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat(CRCore::inDegrees(180.0f),CRCore::crVector3(0.0f,1.0f,0.0f))*
                    CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(1.0f,0.0f,0.0f))); 
        break;
    case YZ_PLANE:  
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(1.0f,0.0f,0.0f))*
                    CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(0.0f,0.0f,1.0f)));
        break;
    case REVERSED_YZ_PLANE:  
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat(CRCore::inDegrees(180.0f),CRCore::crVector3(0.0f,1.0f,0.0f))*
                    CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(1.0f,0.0f,0.0f))*
                    CRCore::crQuat(CRCore::inDegrees(90.0f),CRCore::crVector3(0.0f,0.0f,1.0f)));
        break;
    case XY_PLANE:
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat());  // nop - already on XY plane.
        break;
    case REVERSED_XY_PLANE:
        setAutoRotateToScreen(false);
        setRotation(CRCore::crQuat(CRCore::inDegrees(180.0f),CRCore::crVector3(0.0f,1.0f,0.0f)));
        break;
    case SCREEN:
        setAutoRotateToScreen(true);
        setRotation(CRCore::crQuat());  // nop - already on XY plane.
        break;
    default: break;
    }
}

void crTextBase::setRotation(const CRCore::crQuat& quat)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    m_rotation = quat;
    //computePositions();
	if(m_needReCompute == 0) m_needReCompute = 2;
}


void crTextBase::setAutoRotateToScreen(bool autoRotateToScreen)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_autoRotateToScreen==autoRotateToScreen) return;
    
    m_autoRotateToScreen = autoRotateToScreen;
    //computePositions();
	if(m_needReCompute == 0) m_needReCompute = 2;
}


void crTextBase::setLayout(Layout layout)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_layout==layout) return;

    m_layout = layout;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}


void crTextBase::setDrawMode(unsigned int mode) 
{ 
    if (m_drawMode==mode) return;

    m_drawMode=mode;
}


void crTextBase::setBoundingBoxMargin(float margin)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    if (m_textBBMargin == margin)
        return;

    m_textBBMargin = margin;
    //computeGlyphRepresentation();
	m_needReCompute = 1;
}

bool crTextBase::computeBound() const
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	m_bbox.init();
/*	switch (m_needReCompute)
	{
	case 1:
		const_cast<crTextBase *>(this)->computeGlyphRepresentation();
		m_needReCompute = 0;
		break;
	case 2:
		const_cast<crTextBase *>(this)->computePositions();
		m_needReCompute = 0;
		break;
	}*/ 
	//const_cast<crText *>(this)->computeGlyphRepresentation();
	if (m_textBB.valid())
	{
		for(unsigned int i=0;i<m_autoTransformCache.size();++i)
		{
			CRCore::crMatrix& matrix = m_autoTransformCache[i].m_matrix;
			m_bbox.expandBy(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin())*matrix);
			m_bbox.expandBy(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMax())*matrix);
		}
		//if (!m_bbox.valid())
		//{
		//	// Provide a fallback in cases where no bounding box has been been setup so far.
		//	// Note, assume a scaling of 1.0 for m_characterSizeMode!=OBJECT_COORDS as the
		//	// for screen space coordinates size modes we don't know what scale will be used until
		//	// the text is actually rendered, but we haven't to assume something otherwise the
		//	// text label will be culled by view or small feature culling on first frame.
		//	if (m_autoRotateToScreen)
		//	{
		//		// use bounding sphere encompassing the maximum size of the text centered on the m_position
		//		double radius = m_textBB.radius();
		//		CRCore::crVector3 diagonal(radius, radius, radius);
		//		m_bbox.set(m_position-diagonal, m_position+diagonal);
		//	}
		//	else
		//	{
		//		CRCore::crMatrix matrix;
		//		matrix.makeTranslate(m_position);
		//		matrix.preMultRotate(m_rotation);
		//		m_bbox.expandBy(CRCore::crVector3(m_textBB.xMin(),m_textBB.yMin(),m_textBB.zMin())*matrix);
		//		m_bbox.expandBy(CRCore::crVector3(m_textBB.xMax(),m_textBB.yMax(),m_textBB.zMax())*matrix);
		//	}
		//}
	}
	m_bbox_computed = true;
	return true;
}

void crTextBase::computePositions()
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
    unsigned int size = CRCore::maximum(CRCore::crDisplaySettings::instance()->getMaxNumberOfGraphicsContexts(),m_autoTransformCache.size());
    
    // FIXME: OPTIMIZE: This would be one of the ideal locations to
    // call computeAverageGlyphWidthAndHeight(). It is out of the contextID loop
    // so the value would be computed fewer times. But the code will need changes
    // to get the value down to the locations it is needed. (Either pass through parameters
    // or member variables, but we would need a system to know if the values are stale.)

    
    for(unsigned int i=0;i<size;++i)
    {
        computePositions(i);
    }
}

//void crTextBase::setThreadSafeRefUnref(bool threadSafe)
//{
//    crDrawable::setThreadSafeRefUnref(threadSafe);
//}

//void crTextBase::resizeGLObjectBuffers(unsigned int maxSize)
//{
//    crDrawable::resizeGLObjectBuffers(maxSize);
//
//    m_autoTransformCache.resize(maxSize);
//}


void crTextBase::releaseObjects(CRCore::crState* state)
{
    crDrawable::releaseObjects(state);
}
void crTextBase::setStartCoord(const CRCore::crVector2& coord)
{
	CRCore::ScopedLock<CRCore::crCriticalMutex> lock(m_computeGlyphMutex);
	if (m_startCoord==coord) return;

	m_startCoord = coord;
	m_needReCompute = 1;
}
const CRCore::crVector2& crTextBase::getStartCoord() const
{
	return m_startCoord;
}
const CRCore::crVector2& crTextBase::getEndCoord() const
{
	return m_endCoord;
}
float crTextBase::getCharacterHeight() const { return m_characterHeight; }
float crTextBase::getCharacterAspectRatio() const { return m_style.valid() ? m_style->getWidthRatio() : 1.0f; }
float crTextBase::getCharacterWidth() const { return m_characterHeight / getCharacterAspectRatio(); }
float crTextBase::getUnderLineSpacing() const { return m_underLineSpacing; }
/** Get the line spacing of the text box. */
float crTextBase::getLineSpacing() const { return m_lineSpacing; }

float crTextBase::getLineHeight() const { return m_characterHeight + m_lineSpacing; }
const CRCore::crVector3& crTextBase::getPosition() const { return m_position; }
unsigned int crTextBase::getDrawMode() const { return m_drawMode; }
const std::string &crTextBase::getFontName()const { return m_fontName; }
/** Get the font. Return 0 if default is being used.*/
const crFont* crTextBase::getFont() const { return m_font.get(); }
void crTextBase::positionCursor(const CRCore::crVector2 & endOfLine_coords, CRCore::crVector2 & cursor, unsigned int linelength)
{
    switch(m_layout)
    {
        case LEFT_TO_RIGHT:
        {
            switch (m_alignment)
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
            switch (m_alignment)
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
            switch (m_alignment)
            {
                // TODO: current behaviour top baselines lined up in both cases - need to implement
                //       top of characters aligment - Question is this neccesary?
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
}

