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
#include <CRText/crStyle.h>
#include <CRCore/crNotify.h>
#include <CRCore/crMath.h>
// #include <CRCore/io_utils.h>

using namespace CRText;

/////////////////////////////////////////////////////////////////////////////////////////
//
// crBevel
//
crBevel::crBevel()
{
    m_thickness = 0.02f;
    flatBevel();
}

crBevel::crBevel(const crBevel& bevel, const CRCore::crCopyOp& copyop):
    CRCore::crBase(bevel, copyop),
    m_thickness(bevel.m_thickness),
    m_vertices(bevel.m_vertices)
{
}

void crBevel::flatBevel(float width)
{
    m_vertices.clear();

    if (width>0.5f) width = 0.5f;

    m_vertices.push_back(CRCore::crVector2(0.0f,0.0f));

    m_vertices.push_back(CRCore::crVector2(width,1.0f));

    if (width<0.5f) m_vertices.push_back(CRCore::crVector2(1-width,1.0f));

    m_vertices.push_back(CRCore::crVector2(1.0f,0.0f));
}

void crBevel::roundedBevel(float width, unsigned int numSteps)
{
    m_vertices.clear();

    if (width>0.5f) width = 0.5f;

    unsigned int i = 0;
    for(; i<=numSteps; ++i)
    {
        float angle = float(CRCore::PI)*0.5f*(float(i)/float(numSteps));
        m_vertices.push_back( CRCore::crVector2((1.0f-cosf(angle))*width, sinf(angle)) );
    }

    // start the second half one into the curve if the width is half way across
    i = width<0.5f ? 0 : 1;
    for(; i<=numSteps; ++i)
    {
        float angle = float(CRCore::PI)*0.5f*(float(numSteps-i)/float(numSteps));
        m_vertices.push_back( CRCore::crVector2(1.0-(1.0f-cosf(angle))*width, sin(angle)) );
    }
}

void crBevel::roundedBevel2(float width, unsigned int numSteps)
{
    m_vertices.clear();

    if (width>0.5f) width = 0.5f;

    float h = 0.1f;
    float r = 1.0f-h;

    m_vertices.push_back(CRCore::crVector2(0.0,0.0));

    unsigned int i = 0;
    for(; i<=numSteps; ++i)
    {
        float angle = float(CRCore::PI)*0.5f*(float(i)/float(numSteps));
        m_vertices.push_back( CRCore::crVector2((1.0f-cosf(angle))*width, h + sinf(angle)*r) );
    }

    // start the second half one into the curve if the width is half way across
    i = width<0.5f ? 0 : 1;
    for(; i<=numSteps; ++i)
    {
        float angle = float(CRCore::PI)*0.5f*(float(numSteps-i)/float(numSteps));
        m_vertices.push_back( CRCore::crVector2(1.0-(1.0f-cosf(angle))*width, h + sin(angle)*r) );
    }

    m_vertices.push_back(CRCore::crVector2(1.0,0.0));

}

//void crBevel::print(std::ostream& fout)
//{
//    CRCore::notify(CRCore::NOTICE)<<"print bevel"<<std::endl;
//    for(Vertices::iterator itr = m_vertices.begin();
//        itr != m_vertices.end();
//        ++itr)
//    {
//        CRCore::notify(CRCore::NOTICE)<<"  "<<*itr<<std::endl;
//    }
//}


/////////////////////////////////////////////////////////////////////////////////////////
//
// crStyle
//
crStyle::crStyle():
    m_widthRatio(1.0f),
    m_thicknessRatio(0.0f),
    m_outlineRatio(0.0f),
    m_sampleDensity(1.0f)
{
}

crStyle::crStyle(const crStyle& style, const CRCore::crCopyOp& copyop):
    CRCore::crBase(style,copyop),
    m_bevel(dynamic_cast<crBevel*>(copyop(style.m_bevel.get()))),
    m_widthRatio(style.m_widthRatio),
    m_thicknessRatio(style.m_thicknessRatio),
    m_outlineRatio(style.m_outlineRatio),
    m_sampleDensity(style.m_sampleDensity)
{
}

/// default Layout implementation used if no other is specified on TextNode
CRCore::ref_ptr<crStyle>& crStyle::getDefaultStyle()
{
    static CRCore::crMutex s_DefaultStyleMutex;
    CRCore::ScopedLock<CRCore::crMutex> lock(s_DefaultStyleMutex);

    static CRCore::ref_ptr<crStyle> s_defaultStyle = new crStyle;
    return s_defaultStyle;
}


bool crStyle::operator == (const crStyle& rhs) const
{
    if (&rhs==this) return true;

    if (m_bevel.valid())
    {
        if (!rhs.m_bevel) return false;

        if (!(*m_bevel == *rhs.m_bevel)) return false;
    }
    else
    {
        if (rhs.m_bevel.valid()) return false;
    }

    if (m_widthRatio != rhs.m_widthRatio) return false;
    if (m_thicknessRatio != rhs.m_thicknessRatio) return false;
    if (m_outlineRatio != rhs.m_outlineRatio) return false;
    if (m_sampleDensity != rhs.m_sampleDensity) return false;

    return true;
}
