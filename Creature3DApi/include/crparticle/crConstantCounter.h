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
#ifndef CRPARTICLE_CONSTANTCOUNTER
#define CRPARTICLE_CONSTANTCOUNTER 1

#include <CRParticle/crCounter.h>

#include <CRCore/crBase.h>
#include <CRCore/crMath.h>

namespace CRParticle
{

    class crConstantCounter: public crCounter {
    public:
        crConstantCounter():
            crCounter(), 
            m_numToCreate(0),
            m_created(false)
        {
        }

        crConstantCounter(const crConstantCounter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY):
             crCounter(copy, copyop), 
             m_numToCreate(copy.m_numToCreate),
             m_created(copy.m_created)
        {
        }
            
        
        META_Base(CRParticle, crConstantCounter);
        
        void setNumberOfParticlesToCreate(int numToCreate) { m_numToCreate = numToCreate; }
        int getNumberOfParticlesToCreate() const { return m_numToCreate; }
    
        /// Return the number of particles to be created in this frame
        virtual int numParticlesToCreate(double dt) const
        {
            if(m_created)
				return 0;
			m_created = true;
            return m_numToCreate;
        }

		inline virtual void addCounterParam(int i, const std::string &str)
		{
			switch(i) 
			{
			case 0:
				m_numToCreate = atoi(str.c_str());
				break;
			}
		}
		inline virtual void reset(){ m_created = false; }
    protected:
        virtual ~crConstantCounter() {}
        
        int m_numToCreate;
        mutable bool m_created;
    };
    
}


#endif
