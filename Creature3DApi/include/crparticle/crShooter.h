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
#ifndef CRPARTICLE_SHOOTER
#define CRPARTICLE_SHOOTER 1

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>

namespace CRParticle
{

    class crParticle;

    /**     An abstract base class used by crModularEmitter to "shoot" the particles after they have been placed. 
        Descendants of this class must override the <CODE>shoot()</CODE> method.
    */
    class crShooter: public CRCore::crBase
    {
    public:
        inline crShooter();
        inline crShooter(const crShooter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crShooter"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crShooter *>(obj) != 0; }

        /**     Shoot a particle. Must be overriden by descendants.
            This method should only set the velocity vector of particle <CODE>P</CODE>, leaving other
            attributes unchanged.
        */
        virtual void shoot(crParticle* P) const = 0;
		virtual void addShooterParam(int i, const std::string &str) = 0;

    protected:
        virtual ~crShooter() {}
        crShooter &operator=(const crShooter &) { return *this; }
    };
    
    // INLINE FUNCTIONS
    
    inline crShooter::crShooter()
    : CRCore::crBase()
    {
    }
    
    inline crShooter::crShooter(const crShooter& copy, const CRCore::crCopyOp& copyop)
    : CRCore::crBase(copy, copyop)
    {
    }

}

#endif
