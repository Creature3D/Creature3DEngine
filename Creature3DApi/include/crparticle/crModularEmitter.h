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
#ifndef CRPARTICLE_MODULAREMITTER
#define CRPARTICLE_MODULAREMITTER 1

#include <CRParticle/crExport.h>
#include <CRParticle/crEmitter.h>
#include <CRParticle/crParticle.h>
#include <CRParticle/crRandomRateCounter.h>
#include <CRParticle/crPlacer.h>
#include <CRParticle/crPointPlacer.h>
#include <CRParticle/crShooter.h>
#include <CRParticle/crRadialShooter.h>
#include <CRParticle/crParticleSystem.h>

#include <CRCore/ref_ptr.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>

namespace CRParticle
{

    /**    An emitter class that holds three objects to control the creation of particles.
        These objects are a <I>counter</I>, a <I>placer</I> and a <I>shooter</I>.
        The counter controls the number of particles to be emitted at each frame; 
        the placer must initialize the particle's position vector, while the shooter initializes 
        its velocity vector.
        You can use the predefined counter/placer/shooter classes, or you can create your own.
    */
    class CRPARTICLE_EXPORT crModularEmitter: public crEmitter {
    public:
        crModularEmitter();
        crModularEmitter(const crModularEmitter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle,crModularEmitter);
        
        /// Get the counter object.
        inline crCounter* getCounter();
        
        /// Get the const crCounter object.
        inline const crCounter* getCounter() const;
        
        /// Set the crCounter object.
        inline void setCounter(crCounter* c);
        
        /// Get the ratio between number of particle to create in compenstation for movement of the emitter
        inline float getNumParticlesToCreateMovementCompenstationRatio() const;

        /// Set the ratio between number of particle to create in compenstation for movement of the emitter
        inline void setNumParticlesToCreateMovementCompenstationRatio(float r);


        /// Get the crPlacer object.
        inline crPlacer* getPlacer();
        
        /// Get the const crPlacer object.        
        inline const crPlacer* getPlacer() const;
        
        /// Set the crPlacer object.
        inline void setPlacer(crPlacer* p);
        
        /// Get the crShooter object.
        inline crShooter *getShooter();
        
        /// Get the const crShooter object.
        inline const crShooter *getShooter() const;
        
        /// Set the crShooter object.
        inline void setShooter(crShooter *s);        
		inline virtual void reset();
    protected:
        virtual ~crModularEmitter() {}
        crModularEmitter &operator=(const crModularEmitter &) { return *this; }
        
        virtual void emit(double dt);
        
    private:
    
        float m_numParticleToCreateMovementCompensationRatio;
        CRCore::ref_ptr<crCounter> m_counter;
        CRCore::ref_ptr<crPlacer> m_placer;
        CRCore::ref_ptr<crShooter> m_shooter;
    };
    
    // INLINE FUNCTIONS
    
    inline crCounter* crModularEmitter::getCounter()
    {
        return m_counter.get();
    }
    
    inline const crCounter* crModularEmitter::getCounter() const
    {
        return m_counter.get();
    }

    inline void crModularEmitter::setCounter(crCounter* c)
    {
        m_counter = c;
    }

    inline float crModularEmitter::getNumParticlesToCreateMovementCompenstationRatio() const
    {
        return m_numParticleToCreateMovementCompensationRatio;
    }

    inline void crModularEmitter::setNumParticlesToCreateMovementCompenstationRatio(float r)
    {
        m_numParticleToCreateMovementCompensationRatio = r;
    }

    inline crPlacer* crModularEmitter::getPlacer()
    {
        return m_placer.get();
    }

    inline const crPlacer* crModularEmitter::getPlacer() const
    {
        return m_placer.get();
    }

    inline void crModularEmitter::setPlacer(crPlacer* p)
    {
        m_placer = p;
    }

    inline crShooter *crModularEmitter::getShooter()
    {
        return m_shooter.get();
    }

    inline const crShooter *crModularEmitter::getShooter() const
    {
        return m_shooter.get();
    }

    inline void crModularEmitter::setShooter(crShooter *s)
    {
        m_shooter = s;
    }
	inline void crModularEmitter::reset()
	{
		if(m_counter.valid())
			m_counter->reset();
	}
}

#endif
