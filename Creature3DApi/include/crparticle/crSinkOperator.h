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
#ifndef CRPARTICLE_SINKOPERATOR
#define CRPARTICLE_SINKOPERATOR

#include <CRParticle/crParticle.h>
#include <CRParticle/crDomainOperator.h>

namespace CRParticle
{
/** A sink operator kills particles if positions or velocities inside/outside the specified domain.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class CRPARTICLE_EXPORT crSinkOperator : public crDomainOperator
{
public:
    enum SinkTarget { SINK_POSITION, SINK_VELOCITY, SINK_ANGULAR_VELOCITY };
    enum SinkStrategy { SINK_INSIDE, SINK_OUTSIDE };
    
    crSinkOperator()
    : crDomainOperator(), m_sinkTarget(SINK_POSITION), m_sinkStrategy(SINK_INSIDE)
    {}
    
    crSinkOperator( const crSinkOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    : crDomainOperator(copy, copyop), m_sinkTarget(copy.m_sinkTarget), m_sinkStrategy(copy.m_sinkStrategy)
    {}
    
    META_Base( CRParticle, crSinkOperator );
    
    /// Set the sink strategy
    void setSinkTarget( SinkTarget so ) { m_sinkTarget = so; }
    
    /// Get the sink strategy
    SinkTarget getSinkTarget() const { return m_sinkTarget; }
    
    /// Set the sink strategy
    void setSinkStrategy( SinkStrategy ss ) { m_sinkStrategy = ss; }
    
    /// Get the sink strategy
    SinkStrategy getSinkStrategy() const { return m_sinkStrategy; }
    
    /// Perform some initializations. Do not call this method manually.
    void beginOperate( crProgram* prg );
    
protected:
    virtual ~crSinkOperator() {}
    crSinkOperator& operator=( const crSinkOperator& ) { return *this; }
    
    virtual void handlePoint( const Domain& domain, crParticle* P, double dt );
    virtual void handleLineSegment( const Domain& domain, crParticle* P, double dt );
    virtual void handleTriangle( const Domain& domain, crParticle* P, double dt );
    virtual void handleRectangle( const Domain& domain, crParticle* P, double dt );
    virtual void handlePlane( const Domain& domain, crParticle* P, double dt );
    virtual void handleSphere( const Domain& domain, crParticle* P, double dt );
    virtual void handleBox( const Domain& domain, crParticle* P, double dt );
    virtual void handleDisk( const Domain& domain, crParticle* P, double dt );
    
    inline const CRCore::crVector3& getValue( crParticle* P );
    inline void kill( crParticle* P, bool insideDomain );
    
    SinkTarget m_sinkTarget;
    SinkStrategy m_sinkStrategy;
};

// INLINE METHODS

inline const CRCore::crVector3& crSinkOperator::getValue( crParticle* P )
{
    switch ( m_sinkTarget )
    {
    case SINK_VELOCITY: return P->getVelocity();
    case SINK_ANGULAR_VELOCITY: return P->getAngularVelocity();
    case SINK_POSITION: default: return P->getPosition();
    }
}

inline void crSinkOperator::kill( crParticle* P, bool insideDomain )
{
    if ( !((m_sinkStrategy==SINK_INSIDE) ^ insideDomain) )
        P->kill();
}


}

#endif
