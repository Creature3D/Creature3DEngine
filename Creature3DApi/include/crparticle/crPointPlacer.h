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
#ifndef CRPARTICLE_POINT_PLACER
#define CRPARTICLE_POINT_PLACER 1

#include <CRParticle/crCenteredPlacer.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>

namespace CRParticle
{

    /**    A point-shaped particle placer.
        This placer class uses the center point defined in its base class <CODE>crCenteredPlacer</CODE>
        to place there all incoming particles.
    */
    class crPointPlacer: public crCenteredPlacer {
    public:
        inline crPointPlacer();
        inline crPointPlacer(const crPointPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crPointPlacer);
        
        /** Place a particle. 
            This method is called automatically by <CODE>crModularEmitter</CODE> and should not be called
            manually.
        */
        inline void place(crParticle* P) const;

        /// return the control position
        inline CRCore::crVector3 getControlPosition() const;

    protected:
        virtual ~crPointPlacer() {}
        crPointPlacer& operator=(const crPointPlacer&) { return *this; }        
    };

    // INLINE FUNCTIONS
    
    inline crPointPlacer::crPointPlacer()
    : crCenteredPlacer()
    {
    }
    
    inline crPointPlacer::crPointPlacer(const crPointPlacer& copy, const CRCore::crCopyOp& copyop)
    : crCenteredPlacer(copy, copyop)
    {
    }

    inline void crPointPlacer::place(crParticle* P) const
    {
        P->setPosition(getCenter());
    }


    inline CRCore::crVector3 crPointPlacer::getControlPosition() const
    {
        return getCenter();
    }

}


#endif
