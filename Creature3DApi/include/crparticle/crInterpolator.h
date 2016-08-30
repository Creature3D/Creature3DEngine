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
#ifndef CRPARTICLE_INTERPOLATOR
#define CRPARTICLE_INTERPOLATOR

#include <CRCore/crRange.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>

namespace CRParticle
{

    /// An abstract base class for implementing interpolators.
    class crInterpolator : public CRCore::crBase {
    public:
        crInterpolator()
        : CRCore::crBase() {}
        
        crInterpolator(const crInterpolator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY)
        : CRCore::crBase(copy, copyop) {}
        
        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crInterpolator"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crInterpolator* >(obj) != 0; }
                
        /// Interpolate between floats. Must be overriden in descendant classes.
        virtual float interpolate(float t, float y1, float y2) const = 0;    
        
        /// Interpolate between 2-dimensional vectors. Default behavior is to interpolate each component separately.
        virtual CRCore::crVector2 interpolate(float t, const CRCore::crVector2& y1, const CRCore::crVector2& y2) const
        {
            return CRCore::crVector2(
                interpolate(t, y1.x(), y2.x()),
                interpolate(t, y1.y(), y2.y())
            );
        }

        /// Interpolate between 3-dimensional vectors. Default behavior is to interpolate each component separately.
        virtual CRCore::crVector3 interpolate(float t, const CRCore::crVector3& y1, const CRCore::crVector3& y2) const
        {
            return CRCore::crVector3(
                interpolate(t, y1.x(), y2.x()),
                interpolate(t, y1.y(), y2.y()),
                interpolate(t, y1.z(), y2.z())
            );
        }
        
        /// Interpolate between 4-dimensional vectors. Default behavior is to interpolate each component separately.
        virtual CRCore::crVector4 interpolate(float t, const CRCore::crVector4& y1, const CRCore::crVector4& y2) const
        {
            return CRCore::crVector4(
                interpolate(t, y1.x(), y2.x()),
                interpolate(t, y1.y(), y2.y()),
                interpolate(t, y1.z(), y2.z()),
                interpolate(t, y1.w(), y2.w())
            );
        }
        
        template<class ValueType>
        ValueType interpolate(float t, const CRCore::crRange<ValueType>& r) const
        {
            return interpolate(t, r.minimum, r.maximum);
        }
        
    protected:
        virtual ~crInterpolator() {}
    };

}

#endif
