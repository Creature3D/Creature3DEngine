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
#include <CRCore/crTransferFunction.h>

#include <CRCore/crNotify.h>

using namespace CRCore;

///////////////////////////////////////////////////////////////////////
//
// crTransferFunction base class
//
crTransferFunction::crTransferFunction()
{
}

crTransferFunction::crTransferFunction(const crTransferFunction& tf, const crCopyOp& copyop):
    crBase(tf,copyop)
{
}

crTransferFunction::~crTransferFunction()
{
}

///////////////////////////////////////////////////////////////////////
//
// crTransferFunction1D class
//
crTransferFunction1D::crTransferFunction1D()
{
}

crTransferFunction1D::crTransferFunction1D(const crTransferFunction1D& tf, const crCopyOp& copyop):
    crTransferFunction(tf,copyop)
{
    allocate(tf.getNumberImageCells());
    assign(m_colorMap);
}

void crTransferFunction1D::allocate(unsigned int numX)
{
    m_image = new CRCore::crImage;
    m_image->allocateImage(numX,1,1,GL_RGBA, GL_FLOAT);
    if (!m_colorMap.empty()) assign(m_colorMap);
}

void crTransferFunction1D::clear(const CRCore::crVector4& color)
{
    ColorMap newColours;
    newColours[getMinimum()] = color;
    newColours[getMaximum()] = color;
    
    assign(newColours);
}

void crTransferFunction1D::assignToImage(float lower_v, const CRCore::crVector4& lower_c, float upper_v, const CRCore::crVector4& upper_c)
{
    float minimum = m_colorMap.begin()->first;
    float maximum = m_colorMap.rbegin()->first;
    float endPos = float(getNumberImageCells()-1);
    float multiplier = endPos/(maximum - minimum);
    CRCore::crVector4* imageData = reinterpret_cast<CRCore::crVector4*>(m_image->data());
    
    float lower_iPos = (lower_v - minimum)*multiplier; 
    float upper_iPos = (upper_v - minimum)*multiplier;

    float start_iPos = ceilf(lower_iPos);
    if (start_iPos<0.0f) start_iPos=0.0f;
    if (start_iPos>endPos) return;

    float end_iPos = floorf(upper_iPos);
    if (end_iPos<0.0f) return;
    if (end_iPos>endPos) end_iPos=endPos;

    crVector4 delta_c = (upper_c-lower_c)/(upper_iPos-lower_iPos);
    unsigned int i=static_cast<unsigned int>(start_iPos);
    for(float iPos=start_iPos;
        iPos<=end_iPos; 
        ++iPos, ++i)
    {
        imageData[i] = lower_c + delta_c*(iPos-lower_v);
    }
    
    m_image->dirty();
}


void crTransferFunction1D::setColor(float v, const CRCore::crVector4& color, bool updateImage)
{
    if (!updateImage)
    {
        m_colorMap[v] = color;
        return;
    }

    if (!m_image) allocate(1024);
    
    if (m_colorMap.empty() || v<getMinimum() || v>getMaximum()) 
    {
        m_colorMap[v] = color;

        assign(m_colorMap);
        return;
    }

    m_colorMap[v] = color;

    ColorMap::iterator itr = m_colorMap.find(v);
 
    if (itr != m_colorMap.begin())
    {
        ColorMap::iterator previous_itr = itr;
        --previous_itr;
        
        assignToImage(previous_itr->first, previous_itr->second, v, color); 
    }

    ColorMap::iterator next_itr = itr;
    ++next_itr;

    if (next_itr != m_colorMap.end())
    {
        assignToImage(v, color, next_itr->first, next_itr->second); 
    }
}

CRCore::crVector4 crTransferFunction1D::getColor(float v) const
{
    if (m_colorMap.empty()) return CRCore::crVector4(1.0f,1.0f,1.0f,1.0f);
    if (m_colorMap.size()==1) return m_colorMap.begin()->second;

    if (v <= m_colorMap.begin()->first) return m_colorMap.begin()->second;
    if (v >= m_colorMap.rbegin()->first) return m_colorMap.rbegin()->second;

    // need to implement
    std::pair<ColorMap::const_iterator, ColorMap::const_iterator> range = m_colorMap.equal_range(v);
      
    // we have an identical match
    if (v == range.first->first) return range.first->second;
    
    // range.first will be at the next element after v, so move it before.
    --range.first;
    
    float vBefore = range.first->first;
    const CRCore::crVector4& cBefore = range.first->second;

    float vAfter = range.second->first;
    const CRCore::crVector4& cAfter = range.second->second;
    
    float r = (v-vBefore)/(vAfter-vBefore);

    return cBefore*(1.0f-r) + cAfter*r;
}


void crTransferFunction1D::assign(const ColorMap& newColours)
{
    m_colorMap = newColours;
    
    updateImage();
}

void crTransferFunction1D::updateImage()
{
    if (m_colorMap.empty()) return;

    if (!m_image || m_image->data()==0) allocate(1024);
    
    CRCore::crVector4* imageData = reinterpret_cast<CRCore::crVector4*>(m_image->data());

    if (m_colorMap.size()==1)
    {
        CRCore::crVector4 color = m_colorMap.begin()->second;

        for(int i=0; i<m_image->s(); ++i)
        {
            imageData[i] = color;
        }
        m_image->dirty();
        return;
    }
    
    ColorMap::const_iterator lower_itr = m_colorMap.begin();
    ColorMap::const_iterator upper_itr = lower_itr;
    ++upper_itr;
    
    for(;
        upper_itr != m_colorMap.end();
        ++upper_itr)
    {
        float lower_v = lower_itr->first;
        const CRCore::crVector4& lower_c = lower_itr->second;
        float upper_v = upper_itr->first;
        const CRCore::crVector4& upper_c = upper_itr->second;

        assignToImage(lower_v, lower_c, upper_v, upper_c);
        
        lower_itr = upper_itr;      
    }

    m_image->dirty();
}
