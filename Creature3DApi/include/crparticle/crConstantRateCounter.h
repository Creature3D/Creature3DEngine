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
#ifndef CRPARTICLE_CONSTANTRATECOUNTER
#define CRPARTICLE_CONSTANTRATECOUNTER 1

#include <CRParticle/crCounter.h>

#include <CRCore/crBase.h>
#include <CRCore/crMath.h>

namespace CRParticle
{

    class crConstantRateCounter: public crCounter {
    public:
        crConstantRateCounter():
            crCounter(), 
            m_minimumNumberOfParticlesToCreate(0),
            m_numberOfParticlesPerSecondToCreate(0),
            m_carryOver(0)
        {
        }

        crConstantRateCounter(const crConstantRateCounter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY):
             crCounter(copy, copyop), 
             m_minimumNumberOfParticlesToCreate(copy.m_minimumNumberOfParticlesToCreate),
             m_numberOfParticlesPerSecondToCreate(copy.m_numberOfParticlesPerSecondToCreate),
             m_carryOver(copy.m_carryOver)
        {
        }
            
        
        META_Base(CRParticle, crConstantRateCounter);
        
        void setMinimumNumberOfParticlesToCreate(int minNumToCreate) { m_minimumNumberOfParticlesToCreate = minNumToCreate; }
        int getMinimumNumberOfParticlesToCreate() const { return m_minimumNumberOfParticlesToCreate; }
    
        void setNumberOfParticlesPerSecondToCreate(double numPerSecond) { m_numberOfParticlesPerSecondToCreate = numPerSecond; }
        double getNumberOfParticlesPerSecondToCreate() const { return m_numberOfParticlesPerSecondToCreate; }

        /// Return the number of particles to be created in this frame
        virtual int numParticlesToCreate(double dt) const
        {
            double v = (dt*m_numberOfParticlesPerSecondToCreate);
            int i = (int)(v);
            m_carryOver += (v-(double)i);
            if (m_carryOver>1.0)
            {
                ++i;
                m_carryOver -= 1.0;
            }
            return CRCore::maximum(m_minimumNumberOfParticlesToCreate, i);
        }

		inline virtual void addCounterParam(int i, const std::string &str)
		{
			switch(i) 
			{
			case 0:
				//CRCore::crArgumentParser::readKeyValue(str,"INT",m_minimumNumberOfParticlesToCreate);
				m_minimumNumberOfParticlesToCreate = atoi(str.c_str());
				break;
			case 1:
				{
					//float flt1 = 0.0f;
					//CRCore::crArgumentParser::readKeyValue(str,"FLT",flt1);
                    m_numberOfParticlesPerSecondToCreate = atof(str.c_str());
				}
				break;
			}
		}
    protected:
        virtual ~crConstantRateCounter() {}
        
        int m_minimumNumberOfParticlesToCreate;
        double m_numberOfParticlesPerSecondToCreate;
        mutable double m_carryOver;
    };
    
}


#endif
