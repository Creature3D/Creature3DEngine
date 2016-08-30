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
#ifndef CRPARTICLE_RANDOMRATE_COUNTER
#define CRPARTICLE_RANDOMRATE_COUNTER 1

#include <CRParticle/crVariableRateCounter.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector2.h>

namespace CRParticle
{

    class crRandomRateCounter: public crVariableRateCounter {
    public:
        inline crRandomRateCounter();
        inline crRandomRateCounter(const crRandomRateCounter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle, crRandomRateCounter);
    
        /// Return the number of particles to be created in this frame
        inline int numParticlesToCreate(double dt) const;
        
    protected:
        virtual ~crRandomRateCounter() {}
        
        mutable float m_np;
    };
    
    // INLINE FUNCTIONS
    
    inline crRandomRateCounter::crRandomRateCounter()
    : crVariableRateCounter(), m_np(0)
    {
    }
    
    inline crRandomRateCounter::crRandomRateCounter(const crRandomRateCounter& copy, const CRCore::crCopyOp& copyop)
    : crVariableRateCounter(copy, copyop), m_np(copy.m_np)
    {
    }
    
    inline int crRandomRateCounter::numParticlesToCreate(double dt) const
    {
        m_np += dt * getRateRange().get_random();
        int n = static_cast<int>(m_np);
        m_np -= n;
        return n;
    }    

}


#endif
