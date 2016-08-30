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
#ifndef CRTERRAIN_VALIDDATAOPERATOR
#define CRTERRAIN_VALIDDATAOPERATOR 1

#include <CRCore/Referenced.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRTerrain/crExport.h>

namespace CRTerrain {

struct crValidDataOperator : public CRCore::Referenced
{
    virtual bool operator() (float /*value*/) const { return true; }
    virtual bool operator() (const CRCore::crVector2f& value) const { return operator()(value.x()) && operator()(value.y()) ; }
    virtual bool operator() (const CRCore::crVector3f& value) const { return operator()(value.x()) && operator()(value.y()) && operator()(value.z());  }
    virtual bool operator() (const CRCore::crVector4f& value) const { return operator()(value.x()) && operator()(value.y()) && operator()(value.z()) && operator()(value.w()); }
};

struct crValidRange : public crValidDataOperator
{
    crValidRange(float minValue, float maxValue):
        m_minValue(minValue),
        m_maxValue(maxValue) {}
        
    void setRange(float minValue, float maxValue)
    {
        m_minValue = minValue;
        m_maxValue = maxValue;
    }

    void setMinValue(float minValue) { m_minValue = minValue; }
    float getMinValue() const { return m_minValue; }

    void setMaxValue(float maxValue) { m_maxValue = maxValue; }
    float getMaxValue() const { return m_maxValue; }

    virtual bool operator() (float value) const { return value>=m_minValue && value<=m_maxValue; }

    float m_minValue, m_maxValue;
};


struct crNoDataValue : public crValidDataOperator
{
    crNoDataValue(float value):
        m_value(value) {}
        
    void setNoDataValue(float value) { m_value = value; }
    float getValue() const { return m_value; }

    virtual bool operator() (float value) const { return value!=m_value; }
    
    float m_value;
};

}

#endif


