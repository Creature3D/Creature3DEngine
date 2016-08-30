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
#ifndef CRPARTICLE_LINEARINTERPOLATOR
#define CRPARTICLE_LINEARINTERPOLATOR

#include <CRParticle/crInterpolator.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>

namespace CRParticle
{

    /// A linear interpolator.
    class crLinearInterpolator: public crInterpolator
    {
    public:
        crLinearInterpolator()
        : crInterpolator() {}
        
        crLinearInterpolator(const crLinearInterpolator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY)
        : crInterpolator(copy, copyop) {}
        
        META_Base(CRParticle, crLinearInterpolator);
        
        virtual float interpolate(float t, float y1, float y2) const
        {
            return y1 + (y2 - y1) * t;
        }
        
    protected:
        virtual ~crLinearInterpolator() {}
    };

}

#endif
