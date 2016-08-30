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

#ifndef CRPARTICLE_BOUNCEOPERATOR
#define CRPARTICLE_BOUNCEOPERATOR

#include <CRParticle/crParticle.h>
#include <CRParticle/crDomainOperator.h>

namespace CRParticle
{
/** A bounce operator can affect the particle's velocity to make it rebound.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class CRPARTICLE_EXPORT crBounceOperator : public crDomainOperator
{
public:
    crBounceOperator()
    : crDomainOperator(), m_friction(1.0f), m_resilience(0.0f), m_cutoff(0.0f)
    {}
    
    crBounceOperator( const crBounceOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    :   crDomainOperator(copy, copyop),
        m_friction(copy.m_friction), m_resilience(copy.m_resilience), m_cutoff(copy.m_cutoff)
    {}
    
    META_Base( CRParticle, crBounceOperator );
    
    /// Set the friction
    void setFriction( float f ) { m_friction = f; }
    
    /// Get the friction
    float getFriction() const { return m_friction; }
    
    /// Set the resilience
    void setResilience( float r ) { m_resilience = r; }
    
    /// Get the velocity cutoff factor
    float getResilience() const { return m_resilience; }
    
    /// Set the velocity cutoff factor
    void setCutoff( float v ) { m_cutoff = v; }
    
    /// Get the velocity cutoff factor
    float getCutoff() const { return m_cutoff; }
    
	inline virtual void addOperatorParam(int i, const std::string &str)
	{
		switch (i)
		{
		case 0:
			m_friction = atof(str.c_str());
			break;
		case 1:
			m_resilience = atof(str.c_str());
			break;
		case 2:
			m_cutoff = atof(str.c_str());
			break;
		}		
	}
protected:
    virtual ~crBounceOperator() {}
    crBounceOperator& operator=( const crBounceOperator& ) { return *this; }
    
    virtual void handleTriangle( const Domain& domain, crParticle* P, double dt );
    virtual void handleRectangle( const Domain& domain, crParticle* P, double dt );
    virtual void handlePlane( const Domain& domain, crParticle* P, double dt );
    virtual void handleSphere( const Domain& domain, crParticle* P, double dt );
    virtual void handleDisk( const Domain& domain, crParticle* P, double dt );
    
    float m_friction;
    float m_resilience;
    float m_cutoff;
};


}

#endif
