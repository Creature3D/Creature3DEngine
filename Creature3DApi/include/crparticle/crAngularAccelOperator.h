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
#ifndef CRPARTICLE_ANGULARACCELOPERATOR
#define CRPARTICLE_ANGULARACCELOPERATOR 1

#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>

namespace CRParticle
{

    /**    An operator class that applies a constant angular acceleration to 
     *     the particles.
     */
    class crAngularAccelOperator: public crOperator {
    public:
        inline crAngularAccelOperator();
        inline crAngularAccelOperator(const crAngularAccelOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crAngularAccelOperator);

        /// Get the angular acceleration vector.
        inline const CRCore::crVector3& getAngularAcceleration() const;
        
        /// Set the angular acceleration vector.
        inline void setAngularAcceleration(const CRCore::crVector3& v);
        
        /// Apply the angular acceleration to a particle. Do not call this method manually.
        inline void operate(crParticle* P, double dt);
        
        /// Perform some initializations. Do not call this method manually.
        inline void beginOperate(crProgram *prg);

		inline virtual void addOperatorParam(int i, const std::string &str)
		{
            if(i==0) CRCore::crArgumentParser::appAtoVec(str,m_angul_araccel);
		}
    protected:
        virtual ~crAngularAccelOperator() {}
        crAngularAccelOperator& operator=(const crAngularAccelOperator& ) { return *this; }        

    private:
        CRCore::crVector3 m_angul_araccel;
        CRCore::crVector3 m_xf_angul_araccel;
    };

    // INLINE FUNCTIONS
    
    inline crAngularAccelOperator::crAngularAccelOperator()
    : crOperator(), m_angul_araccel(0, 0, 0)
    {
    }
    
    inline crAngularAccelOperator::crAngularAccelOperator(const crAngularAccelOperator& copy, const CRCore::crCopyOp& copyop)
    : crOperator(copy, copyop), m_angul_araccel(copy.m_angul_araccel)
    {
    }

    inline const CRCore::crVector3& crAngularAccelOperator::getAngularAcceleration() const
    {
        return m_angul_araccel;
    }

    inline void crAngularAccelOperator::setAngularAcceleration(const CRCore::crVector3& v)
    {
        m_angul_araccel = v;
    }

    inline void crAngularAccelOperator::operate(crParticle* P, double dt)
    {
        P->addAngularVelocity(m_xf_angul_araccel * dt);
    }
    
    inline void crAngularAccelOperator::beginOperate(crProgram *prg)
    {
        if (prg->getReferenceFrame() == crModularProgram::RELATIVE_RF) {
            m_xf_angul_araccel = prg->rotateLocalToWorld(m_angul_araccel);
        } else {
            m_xf_angul_araccel = m_angul_araccel;
        }
    }

}


#endif
