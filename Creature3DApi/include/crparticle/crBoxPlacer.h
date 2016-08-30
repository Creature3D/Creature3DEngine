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
#ifndef CRPARTICLE_BOX_PLACER
#define CRPARTICLE_BOX_PLACER 1

#include <CRParticle/crCenteredPlacer.h>
#include <CRParticle/crParticle.h>
#include <CRCore/crRange.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector2.h>
#include <CRCore/crMath.h>

namespace CRParticle
{

    /** A sector-shaped particle placer.
        This placer sets the initial position of incoming particle by choosing a random position 
        within a circular sector; this sector is defined by three parameters: a <I>center point</I>, 
        which is inherited directly from <CODE>CRParticle::crCenteredPlacer</CODE>, a crRange of values 
        for <I>radius</I>, and a crRange of values for the <I>central angle</I> (sometimes called    <B>phi</B>).        
    */
    class crBoxPlacer: public crCenteredPlacer {
    public:
        inline crBoxPlacer();
        inline crBoxPlacer(const crBoxPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        /// Get the crRange of possible values for radius.
        inline const CRCore::rangef& getXRange() const;
        
        /// Set the crRange of possible values for radius.
        inline void setXRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for radius.
        inline void setXRange(float r1, float r2);
        
        /// Get the crRange of possible values for the central angle.
        inline const CRCore::rangef& getYRange() const;        
        
        /// Set the crRange of possible values for the central angle.
        inline void setYRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for the central angle.
        inline void setYRange(float r1, float r2);    

        /// Get the crRange of possible values for the height.
        inline const CRCore::rangef& getZRange() const;   

        /// Set the crRange of possible values for the height.
        inline void setZRange(const CRCore::rangef& r);
        
        /// Set the crRange of possible values for the height.
        inline void setZRange(float r1, float r2);  

        META_Base(CRParticle, crBoxPlacer);
        
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
                setXRange(rr[0],rr[1]);
				break;
			case 2:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setYRange(rr[0],rr[1]);
				break;
			case 3:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				setZRange(rr[0],rr[1]);
				break;
			}
		}
		/// return the volume of the box
		inline float volume() const;
    protected:
        virtual ~crBoxPlacer() {}
        crBoxPlacer& operator=(const crBoxPlacer&) { return *this; }        
        
    private:
        CRCore::rangef m_x_range;
        CRCore::rangef m_y_range;
        CRCore::rangef m_z_range;
    };

    // INLINE FUNCTIONS
    
    inline crBoxPlacer::crBoxPlacer()
    : crCenteredPlacer(), m_x_range(-1, 1), m_y_range(-1, 1), m_z_range(-1, 1)
    {
    }
    
    inline crBoxPlacer::crBoxPlacer(const crBoxPlacer& copy, const CRCore::crCopyOp& copyop)
    : crCenteredPlacer(copy, copyop), 
    m_x_range(copy.m_x_range), m_y_range(copy.m_y_range), m_z_range(copy.m_z_range)
    {
    }
    
    inline const CRCore::rangef& crBoxPlacer::getXRange() const
    {
        return m_x_range;
    }

    inline void crBoxPlacer::setXRange(const CRCore::rangef& r)
    {
        m_x_range = r;
    }
    
    inline void crBoxPlacer::setXRange(float r1, float r2)
    {
        m_x_range.minimum = r1;
        m_x_range.maximum = r2;
    }

    inline const CRCore::rangef& crBoxPlacer::getYRange() const
    {
        return m_y_range;
    }
    
    inline void crBoxPlacer::setYRange(const CRCore::rangef& r)
    {
        m_y_range = r;
    }
    
    inline void crBoxPlacer::setYRange(float r1, float r2)
    {
        m_y_range.minimum = r1;
        m_y_range.maximum = r2;
    }

    inline const CRCore::rangef& crBoxPlacer::getZRange() const
    {
        return m_z_range;
    }

    inline void crBoxPlacer::setZRange(const CRCore::rangef& r)
    {
        m_z_range = r;
    }
    
    inline void crBoxPlacer::setZRange(float r1, float r2)
    {
        m_z_range.minimum = r1;
        m_z_range.maximum = r2;
    }

    inline void crBoxPlacer::place(crParticle* P) const
    {
        CRCore::crVector3 pos(
            getCenter().x() + m_x_range.get_random(), 
            getCenter().y() + m_y_range.get_random(), 
            getCenter().z() + m_z_range.get_random());
        
        P->setPosition(pos);
    }

    inline CRCore::crVector3 crBoxPlacer::getControlPosition() const
    {
        return getCenter();
    }
	inline float crBoxPlacer::volume() const
	{
		return (m_x_range.maximum - m_x_range.minimum) *
			(m_y_range.maximum - m_y_range.minimum) *
			(m_z_range.maximum - m_z_range.minimum);
	}
}

#endif
