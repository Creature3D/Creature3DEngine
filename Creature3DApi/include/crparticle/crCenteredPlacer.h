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
#ifndef CRPARTICLE_CENTERED_PLACER
#define CRPARTICLE_CENTERED_PLACER 1

#include <CRParticle/crPlacer.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crVector3.h>

namespace CRParticle
{

    /**    An abstract placer base class for placers which need a <I>center point</I>.
    */
    class crCenteredPlacer: public crPlacer {
    public:
        inline crCenteredPlacer();
        inline crCenteredPlacer(const crCenteredPlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crCenteredPlacer"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crPlacer* >(obj) != 0; }        
    
        /// Get the center point.
        inline const CRCore::crVector3& getCenter() const;
        
        /// Set the center point.
        inline void setCenter(const CRCore::crVector3& v);
        
        /// Set the center point.
        inline void setCenter(float x, float y, float z);
        
		inline virtual void addPlacerParam(int i, const std::string &str)
		{
			if(i==0) CRCore::crArgumentParser::appAtoVec(str,m_center);
		}
    protected:
        virtual ~crCenteredPlacer() {}
        
    private:
        CRCore::crVector3 m_center;
    };
    
    // INLINE FUNCTIONS
    
    inline crCenteredPlacer::crCenteredPlacer()
    : crPlacer(), m_center(0, 0, 0)
    {
    }
    
    inline crCenteredPlacer::crCenteredPlacer(const crCenteredPlacer& copy, const CRCore::crCopyOp& copyop)
    : crPlacer(copy, copyop), m_center(copy.m_center)
    {
    }
    
    inline const CRCore::crVector3& crCenteredPlacer::getCenter() const
    {
        return m_center;
    }
    
    inline void crCenteredPlacer::setCenter(const CRCore::crVector3& v)
    {
        m_center = v;
    }
    
    inline void crCenteredPlacer::setCenter(float x, float y, float z)
    {
        m_center.set(x, y, z);
    }

}


#endif
