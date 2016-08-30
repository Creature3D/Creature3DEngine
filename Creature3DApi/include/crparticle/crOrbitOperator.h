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

#ifndef CRPARTICLE_ORBITOPERATOR
#define CRPARTICLE_ORBITOPERATOR

#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

namespace CRParticle
{
/** An orbit operator forces particles in the orbit around a point.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class crOrbitOperator : public crOperator
{
public:
    crOrbitOperator()
    : crOperator(), m_magnitude(1.0f), m_epsilon(1e-3), m_maxRadius(FLT_MAX)
    {}
    
    crOrbitOperator( const crOrbitOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    :   crOperator(copy, copyop), m_center(copy.m_center), m_magnitude(copy.m_magnitude),
        m_epsilon(copy.m_epsilon), m_maxRadius(copy.m_maxRadius)
    {}
    
    META_Base( CRParticle, crOrbitOperator );
    
    /// Set the center of orbit
    void setCenter( const CRCore::crVector3& c ) { m_center = c; }
    
    /// Get the center of orbit
    const CRCore::crVector3& getCenter() const { return m_center; }
    
    /// Set the acceleration scale
    void setMagnitude( float mag ) { m_magnitude = mag; }
    
    /// Get the acceleration scale
    float getMagnitude() const { return m_magnitude; }
    
    /// Set the acceleration epsilon
    void setEpsilon( float eps ) { m_epsilon = eps; }
    
    /// Get the acceleration epsilon
    float getEpsilon() const { return m_epsilon; }
    
    /// Set max radius between the center and the particle
    void setMaxRadius( float max ) { m_maxRadius = max; }
    
    /// Get max radius between the center and the particle
    float getMaxRadius() const { return m_maxRadius; }
    
    /// Apply the acceleration to a particle. Do not call this method manually.
    inline void operate( crParticle* P, double dt );
    
    /// Perform some initializations. Do not call this method manually.
    inline void beginOperate( crProgram* prg );
    
	inline virtual void addOperatorParam(int i, const std::string &str)
	{
		switch (i)
		{
		case 0:
			CRCore::crArgumentParser::appAtoVec(str,m_center);
			break;
		case 1:
			m_magnitude = atof(str.c_str());
			break;
		case 2:
			m_epsilon = atof(str.c_str());
			break;
		case 3:
			m_maxRadius = atof(str.c_str());
			break;
		}		
	}
protected:
    virtual ~crOrbitOperator() {}
    crOrbitOperator& operator=( const crOrbitOperator& ) { return *this; }
    
    CRCore::crVector3 m_center;
    CRCore::crVector3 m_xf_center;
    float m_magnitude;
    float m_epsilon;
    float m_maxRadius;
};

// INLINE METHODS

inline void crOrbitOperator::operate( crParticle* P, double dt )
{
	CRCore::crVector3 dir;
	if(P->getPosition() == m_xf_center)
	{
		dir = CRCore::crVector3(0,0,1.0f);
	}
	else
	{
		dir = P->getPosition() - m_xf_center;
	}
    float length = dir.length();
    if ( length<m_maxRadius )
    {
        P->addVelocity( dir * ((m_magnitude * dt) /
                        (length * (m_epsilon+length*length))) );
    }
}

inline void crOrbitOperator::beginOperate( crProgram* prg )
{
    if ( prg->getReferenceFrame()==crModularProgram::RELATIVE_RF )
    {
        m_xf_center = prg->transformLocalToWorld(m_center);
    }
    else
    {
        m_xf_center = m_center;
    }
}


}

#endif
