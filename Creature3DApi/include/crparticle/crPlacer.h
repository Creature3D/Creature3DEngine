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
#ifndef CRPARTICLE_PLACER
#define CRPARTICLE_PLACER 1

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>

namespace CRParticle
{

    class crParticle;

    /**    An abstract base class for implementing <I>particle placers</I>. A placer is an object which take
        a particle as input, and places it somewhere by setting its position vector. crPlacer objects are
        used by the <CODE>crModularEmitter</CODE> class as part of the particle emission process.
    */
    class crPlacer: public CRCore::crBase {
    public:
        inline crPlacer();
        inline crPlacer(const crPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crPlacer"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crPlacer *>(obj) != 0; }

        /// Place a particle. Must be implemented in descendant classes.
        virtual void place(crParticle* P) const = 0;

        /// Return the control position of particles that placer will generate. Must be implemented in descendant classes.
        virtual CRCore::crVector3 getControlPosition() const = 0;
		virtual void addPlacerParam(int i, const std::string &str) = 0;

		/// Volume of the placer. Can be implemented in descendant classes.
		virtual float volume() const { return 1.0f; }
    protected:
        ~crPlacer() {}
        crPlacer& operator=(const crPlacer& ) { return *this; }
    };
    
    // INLINE FUNCTIONS
    
    inline crPlacer::crPlacer()
    : CRCore::crBase()
    {
    }
    
    inline crPlacer::crPlacer(const crPlacer& copy, const CRCore::crCopyOp& copyop)
    : CRCore::crBase(copy, copyop)
    {
    }

}

#endif
