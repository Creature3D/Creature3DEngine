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
#ifndef CRPARTICLE_DOMAINOPERATOR
#define CRPARTICLE_DOMAINOPERATOR

#include <CRCore/crPlane.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>

namespace CRParticle
{
/** A domain operator which accepts different domain shapes as the parameter.
    It can be derived to implement operators that require particles interacting with domains.
    Refer to David McAllister's crParticle System API (http://www.particlesystems.org)
*/
class CRPARTICLE_EXPORT crDomainOperator : public crOperator
{
public:
    struct Domain
    {
        enum Type
        {
            UNDEFINED_DOMAIN = 0,
            POINT_DOMAIN,
            LINE_DOMAIN,
            TRI_DOMAIN,
            RECT_DOMAIN,
            PLANE_DOMAIN,
            SPHERE_DOMAIN,
            BOX_DOMAIN,
            DISK_DOMAIN
        };
        
        Domain( Type t ) : r1(0.0f), r2(0.0f), type(t) {}
        CRCore::crPlane plane;
        CRCore::crVector3 v1;
        CRCore::crVector3 v2;
        CRCore::crVector3 v3;
        CRCore::crVector3 s1;
        CRCore::crVector3 s2;
        float r1;
        float r2;
        Type type;
    };
    
    crDomainOperator()
    : crOperator()
    {}
    
    crDomainOperator( const crDomainOperator& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    : crOperator(copy, copyop), m_domains(copy.m_domains), m_backupDomains(copy.m_backupDomains)
    {}
    
    META_Base( CRParticle, crDomainOperator );
    
    /// Add a point domain
    inline void addPointDomain( const CRCore::crVector3& p );
    
    /// Add a line segment domain
    inline void addLineSegmentDomain( const CRCore::crVector3& v1, const CRCore::crVector3& v2 );
    
    /// Add a triangle domain
    inline void addTriangleDomain( const CRCore::crVector3& v1, const CRCore::crVector3& v2, const CRCore::crVector3& v3 );
    
    /// Add a rectangle domain
    inline void addRectangleDomain( const CRCore::crVector3& corner, const CRCore::crVector3& w, const CRCore::crVector3& h );
    
    /// Add a plane domain
    inline void addPlaneDomain( const CRCore::crPlane& plane );
    
    /// Add a sphere domain
    inline void addSphereDomain( const CRCore::crVector3& c, float r );
    
    /// Add a box domain
    inline void addBoxDomain( const CRCore::crVector3& min, const CRCore::crVector3& max );
    
    /// Add a disk domain
    inline void addDiskDomain( const CRCore::crVector3& c, const CRCore::crVector3& n, float r1, float r2=0.0f );
    
    /// Add a domain object directly, used by the .CRCore wrappers and serializers.
    void addDomain( const Domain& domain ) { m_domains.push_back(domain); }
    
    /// Get a domain object directly, used by the .CRCore wrappers and serializers.
    const Domain& getDomain( unsigned int i ) const { return m_domains[i]; }
    
    /// Remove a domain at specific index
    void removeDomain( unsigned int i )
    { if (i<m_domains.size()) m_domains.erase(m_domains.begin() + i); }
    
    /// Remove all existing domains
    void removeAllDomains() { m_domains.clear(); }
    
    /// Get number of domains
    unsigned int getNumDomains() const { return m_domains.size(); }
    
    /// Apply the acceleration to a particle. Do not call this method manually.
    void operate( crParticle* P, double dt );
    
    /// Perform some initializations. Do not call this method manually.
    void beginOperate( crProgram* prg );
    
    /// Perform some post-operations. Do not call this method manually.
    void endOperate();
	inline virtual void addOperatorParam(int i, const std::string &str){}
protected:
    virtual ~crDomainOperator() {}
    crDomainOperator& operator=( const crDomainOperator& ) { return *this; }
    
    virtual void handlePoint( const Domain& domain, crParticle* P, double dt ) { ignore("Point"); }
    virtual void handleLineSegment( const Domain& domain, crParticle* P, double dt ) { ignore("LineSegment"); }
    virtual void handleTriangle( const Domain& domain, crParticle* P, double dt ) { ignore("Triangle"); }
    virtual void handleRectangle( const Domain& domain, crParticle* P, double dt ) { ignore("Rectangle"); }
    virtual void handlePlane( const Domain& domain, crParticle* P, double dt ) { ignore("Plane"); }
    virtual void handleSphere( const Domain& domain, crParticle* P, double dt ) { ignore("Sphere"); }
    virtual void handleBox( const Domain& domain, crParticle* P, double dt ) { ignore("Box"); }
    virtual void handleDisk( const Domain& domain, crParticle* P, double dt ) { ignore("Disk"); }
    
    inline void computeNewBasis( const CRCore::crVector3&, const CRCore::crVector3&, CRCore::crVector3&, CRCore::crVector3& );
    inline void ignore( const std::string& func );
    
    std::vector<Domain> m_domains;
    std::vector<Domain> m_backupDomains;
};

// INLINE METHODS

inline void crDomainOperator::addPointDomain( const CRCore::crVector3& p )
{
    Domain domain( Domain::POINT_DOMAIN );
    domain.v1 = p;
    m_domains.push_back( domain );
}

inline void crDomainOperator::addLineSegmentDomain( const CRCore::crVector3& v1, const CRCore::crVector3& v2 )
{
    Domain domain( Domain::LINE_DOMAIN );
    domain.v1 = v1;
    domain.v2 = v2;
    domain.r1 = (v2 - v1).length();
    m_domains.push_back( domain );
}

inline void crDomainOperator::addTriangleDomain( const CRCore::crVector3& v1, const CRCore::crVector3& v2, const CRCore::crVector3& v3 )
{
    Domain domain( Domain::TRI_DOMAIN );
    domain.v1 = v1;
    domain.v2 = v2;
    domain.v3 = v3;
    domain.plane.set(v1, v2, v3);
    computeNewBasis( v2-v1, v3-v1, domain.s1, domain.s2 );
    m_domains.push_back( domain );
}

inline void crDomainOperator::addRectangleDomain( const CRCore::crVector3& corner, const CRCore::crVector3& w, const CRCore::crVector3& h )
{
    Domain domain( Domain::RECT_DOMAIN );
    domain.v1 = corner;
    domain.v2 = w;
    domain.v3 = h;
    domain.plane.set(corner, corner+w, corner+h);
    computeNewBasis( w, h, domain.s1, domain.s2 );
    m_domains.push_back( domain );
}

inline void crDomainOperator::addPlaneDomain( const CRCore::crPlane& plane )
{
    Domain domain( Domain::PLANE_DOMAIN );
    domain.plane.set(plane);
    m_domains.push_back( domain );
}

inline void crDomainOperator::addSphereDomain( const CRCore::crVector3& c, float r )
{
    Domain domain( Domain::SPHERE_DOMAIN );
    domain.v1 = c;
    domain.r1 = r;
    m_domains.push_back( domain );
}

inline void crDomainOperator::addBoxDomain( const CRCore::crVector3& min, const CRCore::crVector3& max )
{
    Domain domain( Domain::BOX_DOMAIN );
    domain.v1 = min;
    domain.v2 = max;
    m_domains.push_back( domain );
}

inline void crDomainOperator::addDiskDomain( const CRCore::crVector3& c, const CRCore::crVector3& n, float r1, float r2 )
{
    Domain domain( Domain::DISK_DOMAIN );
    domain.v1 = c;
    domain.v2 = n;
    domain.r1 = r1;
    domain.r2 = r2;
    domain.plane.set(n, c);
    m_domains.push_back( domain );
}

inline void crDomainOperator::computeNewBasis( const CRCore::crVector3& u, const CRCore::crVector3& v, CRCore::crVector3& s1, CRCore::crVector3& s2 )
{
    // Copied from David McAllister's crParticle System API (http://www.particlesystems.org), pDomain.h
    CRCore::crVector3 w = u ^ v;
    float det = w.z()*u.x()*v.y() - w.z()*u.y()*v.x() - u.z()*w.x()*v.y() -
                u.x()*v.z()*w.y() + v.z()*w.x()*u.y() + u.z()*v.x()*w.y();
    det = 1.0f / det;
    
    s1.set( v.y()*w.z() - v.z()*w.y(), v.z()*w.x() - v.x()*w.z(), v.x()*w.y() - v.y()*w.x() );
    s1 = s1 * det;
    s2.set( u.y()*w.z() - u.z()*w.y(), u.z()*w.x() - u.x()*w.z(), u.x()*w.y() - u.y()*w.x() );
    s2 = s2 * (-det);
}

inline void crDomainOperator::ignore( const std::string& func )
{
    //CRCore::notify(CRCore::NOTICE) << className() << ": " << func << " domain not yet implemented. " << std::endl;
}


}

#endif
