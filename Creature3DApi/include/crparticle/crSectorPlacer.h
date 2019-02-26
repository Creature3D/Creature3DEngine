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
#ifndef CRPARTICLE_SECTOR_PLACER
#define CRPARTICLE_SECTOR_PLACER 1

#include <CRParticle/crCenteredPlacer.h>
#include <CRParticle/crParticle.h>
#include <CRCore/crRange.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMath.h>

namespace CRParticle
{

    /**     A sector-shaped particle placer.
        This placer sets the initial position of incoming particle by choosing a random position 
        within a circular sector; this sector is defined by three parameters: a <I>center point</I>, 
        which is inherited directly from <CODE>CRParticle::crCenteredPlacer</CODE>, a crRange of values 
        for <I>radius</I>, and a crRange of values for the <I>central angle</I> (sometimes called    <B>phi</B>).        
    */
    class crSectorPlacer: public crCenteredPlacer {
    public:
        inline crSectorPlacer();
        inline crSectorPlacer(const crSectorPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        /// Get the crRange of possible values for radius.
        inline const CRCore::rangef& getRadiusRange() const;
        
        /// Set the crRange of possible values for radius.
        inline void setRadiusRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for radius.
        inline void setRadiusRange(float r1, float r2);
        
        /// Get the crRange of possible values for the central angle.
        inline const CRCore::rangef& getPhiRange() const;        
        
        /// Set the crRange of possible values for the central angle.
        inline void setPhiRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for the central angle.
        inline void setPhiRange(float r1, float r2);    

        META_Base(CRParticle, crSectorPlacer);
        
        /// Place a particle. Do not call it manually.
        inline void place(crParticle* P) const;

        /// return the control position
        inline CRCore::crVector3 getControlPosition() const;

		inline virtual void addPlacerParam(int i, const std::string &str)
		{
			if(i==0) return crCenteredPlacer::addPlacerParam(i,str);
			CRCore::crVector2f rr;
			switch(i) 
			{
			case 1:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setRadiusRange(rr[0],rr[1]);
				break;
			case 2:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setPhiRange(rr[0],rr[1]);
				break;
			}
		}
		/// return the area of the sector
		inline float volume() const;
    protected:
        virtual ~crSectorPlacer() {}
        crSectorPlacer& operator=(const crSectorPlacer&) { return *this; }        
        
    private:
        CRCore::rangef m_rad_range;
        CRCore::rangef m_phi_range;
    };

    // INLINE FUNCTIONS
    
    inline crSectorPlacer::crSectorPlacer()
    : crCenteredPlacer(), m_rad_range(0, 1), m_phi_range(0, PI*2)
    {
    }
    
    inline crSectorPlacer::crSectorPlacer(const crSectorPlacer& copy, const CRCore::crCopyOp& copyop)
    : crCenteredPlacer(copy, copyop), m_rad_range(copy.m_rad_range), m_phi_range(copy.m_phi_range)
    {
    }
    
    inline const CRCore::rangef& crSectorPlacer::getRadiusRange() const
    {
        return m_rad_range;
    }

    inline const CRCore::rangef& crSectorPlacer::getPhiRange() const
    {
        return m_phi_range;
    }

    inline void crSectorPlacer::setRadiusRange(const CRCore::rangef& r)
    {
        m_rad_range = r;
    }
    
    inline void crSectorPlacer::setRadiusRange(float r1, float r2)
    {
        m_rad_range.minimum = r1;
        m_rad_range.maximum = r2;
    }
    
    inline void crSectorPlacer::setPhiRange(const CRCore::rangef& r)
    {
        m_phi_range = r;
    }
    
    inline void crSectorPlacer::setPhiRange(float r1, float r2)
    {
        m_phi_range.minimum = r1;
        m_phi_range.maximum = r2;
    }

    inline void crSectorPlacer::place(crParticle* P) const
    {
        float rad = m_rad_range.get_random_sqrtf();
        float phi = m_phi_range.get_random();
        
        CRCore::crVector3 pos(
            getCenter().x() + rad * cosf(phi), 
            getCenter().y() + rad * sinf(phi), 
            getCenter().z());
        
        P->setPosition(pos);
    }

    inline CRCore::crVector3 crSectorPlacer::getControlPosition() const
    {
        return getCenter();
    }

	inline float crSectorPlacer::volume() const
	{
		return 0.5f * (m_phi_range.maximum - m_phi_range.minimum) *
			(m_rad_range.maximum*m_rad_range.maximum - m_rad_range.minimum*m_rad_range.minimum);
	}
}

#endif
