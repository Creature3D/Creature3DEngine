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

#ifndef CRPARTICLE_EXPLOSIONOPERATOR
#define CRPARTICLE_EXPLOSIONOPERATOR

#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

namespace CRParticle
{
/** An explosion operator exerts force on each particle away from the explosion center.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class crExplosionOperator : public crOperator
{
public:
    crExplosionOperator()
    :   crOperator(), m_radius(1.0f),
        m_magnitude(1.0f), m_epsilon(1e-3), m_sigma(1.0f),
        m_inexp(0.0f), m_outexp(0.0f)
    {}
    
    crExplosionOperator( const crExplosionOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    :   crOperator(copy, copyop), m_center(copy.m_center), m_radius(copy.m_radius),
        m_magnitude(copy.m_magnitude), m_epsilon(copy.m_epsilon), m_sigma(copy.m_sigma),
        m_inexp(copy.m_inexp), m_outexp(copy.m_outexp)
    {}
    
    META_Base( CRParticle, crExplosionOperator );
    
    /// Set the center of shock wave
    void setCenter( const CRCore::crVector3& c ) { m_center = c; }
    
    /// Get the center of shock wave
    const CRCore::crVector3& getCenter() const { return m_center; }
    
    /// Set the radius of wave peak
    void setRadius( float r ) { m_radius = r; }
    
    /// Get the radius of wave peak
    float getRadius() const { return m_radius; }
    
    /// Set the acceleration scale
    void setMagnitude( float mag ) { m_magnitude = mag; }
    
    /// Get the acceleration scale
    float getMagnitude() const { return m_magnitude; }
    
    /// Set the acceleration epsilon
    void setEpsilon( float eps ) { m_epsilon = eps; }
    
    /// Get the acceleration epsilon
    float getEpsilon() const { return m_epsilon; }
    
    /// Set broadness of the strength of the wave
    void setSigma( float s ) { m_sigma = s; }
    
    /// Get broadness of the strength of the wave
    float getSigma() const { return m_sigma; }
    
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
			m_radius = atof(str.c_str());
			break;
		case 2:
			m_magnitude = atof(str.c_str());
			break;
		case 3:
			m_epsilon = atof(str.c_str());
			break;
		case 4:
			m_sigma = atof(str.c_str());
			break;
		}		
	}
protected:
    virtual ~crExplosionOperator() {}
    crExplosionOperator& operator=( const crExplosionOperator& ) { return *this; }
    
    CRCore::crVector3 m_center;
    CRCore::crVector3 m_xf_center;
    float m_radius;
    float m_magnitude;
    float m_epsilon;
    float m_sigma;
    float m_inexp;
    float m_outexp;
};

// INLINE METHODS

inline void crExplosionOperator::operate( crParticle* P, double dt )
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
    float distanceFromWave2 = (m_radius - length) * (m_radius - length);
    float Gd = exp(distanceFromWave2 * m_inexp) * m_outexp;
    float factor = (m_magnitude * dt) / (length * (m_epsilon+length*length));
    P->addVelocity( dir * (Gd * factor) );
}

inline void crExplosionOperator::beginOperate( crProgram* prg )
{
    if ( prg->getReferenceFrame()==crModularProgram::RELATIVE_RF )
    {
        m_xf_center = prg->transformLocalToWorld(m_center);
    }
    else
    {
        m_xf_center = m_center;
    }
    
    float oneOverSigma = (m_sigma!=0.0f ? (1.0f / m_sigma) : 1.0f);
    m_inexp = -0.5f * oneOverSigma * oneOverSigma;
    m_outexp = oneOverSigma / sqrt(PI * 2.0f);
}


}

#endif
