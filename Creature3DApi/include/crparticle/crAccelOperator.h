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
#ifndef CRPARTICLE_ACCELOPERATOR
#define CRPARTICLE_ACCELOPERATOR 1

#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crArgumentParser.h>

namespace CRParticle
{

    /**    An operator class that applies a constant acceleration to the particles.
    */
    class crAccelOperator: public crOperator {
    public:
        inline crAccelOperator();
        inline crAccelOperator(const crAccelOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crAccelOperator);

        /// Get the acceleration vector.
        inline const CRCore::crVector3& getAcceleration() const;
        
        /// Set the acceleration vector.
        inline void setAcceleration(const CRCore::crVector3& v);
        
        /** Quickly set the acceleration vector to the gravity on earth (0, 0, -9.81).
            The acceleration will be multiplied by the <CODE>scale</CODE> parameter.
        */
        inline void setToGravity(float scale = 1);
        
        /// Apply the acceleration to a particle. Do not call this method manually.
        inline void operate(crParticle* P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(crProgram *prg);

		inline virtual void addOperatorParam(int i, const std::string &str)
		{
			if(i == 0) CRCore::crArgumentParser::appAtoVec(str,m_accel);
		}
    protected:
        virtual ~crAccelOperator() {}
        crAccelOperator &operator=(const crAccelOperator &) { return *this; }        

    private:
        CRCore::crVector3 m_accel;
        CRCore::crVector3 m_xf_accel;
    };

    // INLINE FUNCTIONS
    
    inline crAccelOperator::crAccelOperator()
    : crOperator(), m_accel(0, 0, 0)
    {
    }
    
    inline crAccelOperator::crAccelOperator(const crAccelOperator& copy, const CRCore::crCopyOp& copyop)
    : crOperator(copy, copyop), m_accel(copy.m_accel)
    {
    }

    inline const CRCore::crVector3& crAccelOperator::getAcceleration() const
    {
        return m_accel;
    }

    inline void crAccelOperator::setAcceleration(const CRCore::crVector3& v)
    {
        m_accel = v;
    }
    
    inline void crAccelOperator::setToGravity(float scale)
    {
        m_accel.set(0, 0, -9.80665f * scale);
    }

    inline void crAccelOperator::operate(crParticle* P, double dt)
    {
        P->addVelocity(m_xf_accel * dt);
    }
    
    inline void crAccelOperator::beginOperate(crProgram *prg)
    {
        if (prg->getReferenceFrame() == crModularProgram::RELATIVE_RF) {
            m_xf_accel = prg->rotateLocalToWorld(m_accel);
        } else {
            m_xf_accel = m_accel;
        }
    }

}


#endif
