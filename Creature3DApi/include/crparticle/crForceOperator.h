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
#ifndef CRPARTICLE_FORCEOPERATOR
#define CRPARTICLE_FORCEOPERATOR 1

#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>

namespace CRParticle
{

    /** An operator that applies a constant force to the particles.
      * Remember that if the mass of particles is expressed in kg and the lengths are
      * expressed in meters, then the force should be expressed in Newtons.
    */
    class crForceOperator: public crOperator {
    public:
        inline crForceOperator();
        inline crForceOperator(const crForceOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRParticle, crForceOperator);
        
        /// Get the force vector.
        inline const CRCore::crVector3& getForce() const;
        
        /// Set the force vector.
        inline void setForce(const CRCore::crVector3& f);
        
        /// Apply the force to a particle. Do not call this method manually.
        inline void operate(crParticle* P, double dt);
        
        /// Perform some initialization. Do not call this method manually.
        inline void beginOperate(crProgram *prg);
        
		inline virtual void addOperatorParam(int i, const std::string &str)
		{
			if(i==0) CRCore::crArgumentParser::appAtoVec(str,m_force);
		}

    protected:
        virtual ~crForceOperator() {};
        crForceOperator& operator=(const crForceOperator&) { return *this; }        
        
    private:
        CRCore::crVector3 m_force;
        CRCore::crVector3 m_xf_force;
    };
    
    // INLINE FUNCTIONS
    
    inline crForceOperator::crForceOperator()
    : crOperator(), m_force(0, 0, 0)
    {        
    }
    
    inline crForceOperator::crForceOperator(const crForceOperator& copy, const CRCore::crCopyOp& copyop)
    : crOperator(copy, copyop), m_force(copy.m_force)
    {
    }
    
    inline const CRCore::crVector3& crForceOperator::getForce() const
    {
        return m_force;
    }
    
    inline void crForceOperator::setForce(const CRCore::crVector3& v)
    {
        m_force = v;
    }
    
    inline void crForceOperator::operate(crParticle* P, double dt)
    {
        P->addVelocity(m_xf_force * (P->getMassInv() * dt));
    }
    
    inline void crForceOperator::beginOperate(crProgram *prg)
    {
        if (prg->getReferenceFrame() == crModularProgram::RELATIVE_RF) {
            m_xf_force = prg->rotateLocalToWorld(m_force);
        } else {
            m_xf_force = m_force;
        }
    }
}

#endif
