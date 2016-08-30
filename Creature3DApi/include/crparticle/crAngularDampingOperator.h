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
#ifndef CRPARTICLE_ANGULARDAMPINGOPERATOR
#define CRPARTICLE_ANGULARDAMPINGOPERATOR

#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

namespace CRParticle
{
/** A angular damping operator applies damping constant to particle's angular velocity.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class crAngularDampingOperator : public crOperator
{
public:
    crAngularDampingOperator() : crOperator(), m_cutoffLow(0.0f), m_cutoffHigh(FLT_MAX)
    {}
    
    crAngularDampingOperator( const crAngularDampingOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    :   crOperator(copy, copyop), m_damping(copy.m_damping),
        m_cutoffLow(copy.m_cutoffLow), m_cutoffHigh(copy.m_cutoffHigh)
    {}
    
    META_Base( CRParticle, crAngularDampingOperator );
    
    /// Set the damping factors
    void setDamping( float x, float y, float z ) { m_damping.set(x, y, z); }
    void setDamping( const CRCore::crVector3& damping ) { m_damping = damping; }
    
    /// Set the damping factors to one value
    void setDamping( float x ) { m_damping.set(x, x, x); }
    
    /// Get the damping factors
    void getDamping( float& x, float& y, float& z ) const
    { x = m_damping.x(); y = m_damping.y(); z = m_damping.z(); }
    
    const CRCore::crVector3& getDamping() const { return m_damping; }
    
    /// Set the velocity cutoff factors
    void setCutoff( float low, float high ) { m_cutoffLow = low; m_cutoffHigh = high; }
    void setCutoffLow( float low ) { m_cutoffLow = low; }
    void setCutoffHigh( float low ) { m_cutoffHigh = low; }
    
    /// Get the velocity cutoff factors
    void getCutoff( float& low, float& high ) const { low = m_cutoffLow; high = m_cutoffHigh; }
    float getCutoffLow() const { return m_cutoffLow; }
    float getCutoffHigh() const { return m_cutoffHigh; }
    
    /// Apply the acceleration to a particle. Do not call this method manually.
    inline void operate( crParticle* P, double dt );
    
	inline virtual void addOperatorParam(int i, const std::string &str)
	{
		switch (i)
		{
		case 0:
			CRCore::crArgumentParser::appAtoVec(str,m_damping);
			break;
		case 1:
            m_cutoffLow = atof(str.c_str());
			break;
		case 2:
			m_cutoffHigh = atof(str.c_str());
			break;
		}		
	}
protected:
    virtual ~crAngularDampingOperator() {}
    crAngularDampingOperator& operator=( const crAngularDampingOperator& ) { return *this; }
    
    CRCore::crVector3 m_damping;
    float m_cutoffLow;
    float m_cutoffHigh;
};

// INLINE METHODS

inline void crAngularDampingOperator::operate( crParticle* P, double dt )
{
    const CRCore::crVector3& vel = P->getAngularVelocity();
    float length2 = vel.length2();
    if ( length2>=m_cutoffLow && length2<=m_cutoffHigh )
    {
        CRCore::crVector3 newvel( vel.x() * (1.0f - (1.0f - m_damping.x()) * dt),
                          vel.y() * (1.0f - (1.0f - m_damping.y()) * dt),
                          vel.z() * (1.0f - (1.0f - m_damping.z()) * dt) );
        P->setAngularVelocity( newvel );
    }
}


}

#endif
