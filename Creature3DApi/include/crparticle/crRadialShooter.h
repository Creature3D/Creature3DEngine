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
#ifndef CRPARTICLE_RADIAL_SHOOTER
#define CRPARTICLE_RADIAL_SHOOTER 1

#include <CRParticle/crShooter.h>
#include <CRParticle/crParticle.h>
#include <CRCore/crRange.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crMath.h>
#include <CRCore/crVector2.h>
namespace CRParticle
{

    /**    A shooter class that shoots particles radially.
        This shooter computes the velocity vector of incoming particles by choosing a
        random direction and a random speed. Both direction and speed are chosen within
        specified ranges. The direction is defined by two angles: <B>theta</B>, which
        is the angle between the velocity vector and the Z axis, and <B>phi</B>, which is
        the angle between the X axis and the velocity vector projected onto the X-Y plane.
    */
    class crRadialShooter: public crShooter {
    public:
        inline crRadialShooter();
        inline crRadialShooter(const crRadialShooter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crRadialShooter);

        /// Get the crRange of possible values for <B>theta</B> angle.
        inline const CRCore::rangef& getThetaRange() const;
        
        /// Set the crRange of possible values for <B>theta</B> angle.
        inline void setThetaRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for <B>theta</B> angle.
        inline void setThetaRange(float r1, float r2);
        
        /// Get the crRange of possible values for <B>phi</B> angle.
        inline const CRCore::rangef& getPhiRange() const;
        
        /// Set the crRange of possible values for <B>phi</B> angle.
        inline void setPhiRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for <B>phi</B> angle.
        inline void setPhiRange(float r1, float r2);
        
        /// Get the crRange of possible values for initial speed of particles.
        inline const CRCore::rangef& getInitialSpeedRange() const;        
        
        /// Set the crRange of possible values for initial speed of particles.
        inline void setInitialSpeedRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for initial speed of particles.
        inline void setInitialSpeedRange(float r1, float r2);

        /// Get the crRange of possible values for initial rotational speed of particles.
        inline const CRCore::rangev3& getInitialRotationalSpeedRange() const;        
        
        /// Set the crRange of possible values for initial rotational speed of particles.
        inline void setInitialRotationalSpeedRange(const CRCore::rangev3& r);
        
        /// Set the crRange of possible values for initial rotational speed of particles.
        inline void setInitialRotationalSpeedRange(const CRCore::crVector3& r1, const CRCore::crVector3& r2);

        /// Shoot a particle. Do not call this method manually.
        inline void shoot(crParticle* P) const;

		inline virtual void addShooterParam(int i, const std::string &str)
		{
			CRCore::crVector2 rr;
			switch(i) 
			{
			case 0:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setThetaRange(rr[0],rr[1]);
				break;
			case 1:
				CRCore::crArgumentParser::appAtoVec(str,rr);
                setPhiRange(rr[0],rr[1]);
				break;
			case 2:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setInitialSpeedRange(rr[0],rr[1]);
				break;
			case 3:
				CRCore::crArgumentParser::appAtoVec(str,m_rot_speed_range.minimum);
				break;
			case 4:
				CRCore::crArgumentParser::appAtoVec(str,m_rot_speed_range.maximum);
				break;
			}
		}
    protected:
        virtual ~crRadialShooter() {}
        crRadialShooter& operator=(const crRadialShooter&) { return *this; }

    private:
        CRCore::rangef m_theta_range;
        CRCore::rangef m_phi_range;
        CRCore::rangef m_speed_range;
        CRCore::rangev3 m_rot_speed_range;
    };

    // INLINE FUNCTIONS

    inline crRadialShooter::crRadialShooter()
    :    crShooter(), 
        m_theta_range(0, 0.5f*PI_4), 
        m_phi_range(0, 2*PI), 
        m_speed_range(10, 10),
        m_rot_speed_range(CRCore::crVector3(0,0,0), CRCore::crVector3(0,0,0))
    {
    }

    inline crRadialShooter::crRadialShooter(const crRadialShooter& copy, const CRCore::crCopyOp& copyop)
    :    crShooter(copy, copyop), 
        m_theta_range(copy.m_theta_range), 
        m_phi_range(copy.m_phi_range), 
        m_speed_range(copy.m_speed_range),
        m_rot_speed_range(copy.m_rot_speed_range)
    {
    }
    
    inline const CRCore::rangef& crRadialShooter::getThetaRange() const
    {
        return m_theta_range;
    }

    inline const CRCore::rangef& crRadialShooter::getPhiRange() const
    {
        return m_phi_range;
    }

    inline const CRCore::rangef& crRadialShooter::getInitialSpeedRange() const
    {
        return m_speed_range;
    }

    inline const CRCore::rangev3& crRadialShooter::getInitialRotationalSpeedRange() const
    {
        return m_rot_speed_range;
    }

    inline void crRadialShooter::setThetaRange(const CRCore::rangef& r)
    {
        m_theta_range = r;
    }

    inline void crRadialShooter::setThetaRange(float r1, float r2)
    {
        m_theta_range.minimum = r1;
        m_theta_range.maximum = r2;
    }

    inline void crRadialShooter::setPhiRange(const CRCore::rangef& r)
    {
        m_phi_range = r;
    }

    inline void crRadialShooter::setPhiRange(float r1, float r2)
    {
        m_phi_range.minimum = r1;
        m_phi_range.maximum = r2;
    }

    inline void crRadialShooter::setInitialSpeedRange(const CRCore::rangef& r)
    {
        m_speed_range = r;
    }

    inline void crRadialShooter::setInitialSpeedRange(float r1, float r2)
    {
        m_speed_range.minimum = r1;
        m_speed_range.maximum = r2;
    }

    inline void crRadialShooter::setInitialRotationalSpeedRange(const CRCore::rangev3& r)
    {
        m_rot_speed_range = r;
    }

    inline void crRadialShooter::setInitialRotationalSpeedRange(const CRCore::crVector3& r1, const CRCore::crVector3& r2)
    {
        m_rot_speed_range.minimum = r1;
        m_rot_speed_range.maximum = r2;
    }

    inline void crRadialShooter::shoot(crParticle* P) const
    {
        float theta = m_theta_range.get_random();
        float phi = m_phi_range.get_random();
        float speed = m_speed_range.get_random();
        CRCore::crVector3 rot_speed = m_rot_speed_range.get_random();
        
        P->setVelocity(CRCore::crVector3(
            speed * sinf(theta) * cosf(phi),
            speed * sinf(theta) * sinf(phi),
            speed * cosf(theta)
            ));
            
        P->setAngularVelocity(rot_speed);
    }

}


#endif
