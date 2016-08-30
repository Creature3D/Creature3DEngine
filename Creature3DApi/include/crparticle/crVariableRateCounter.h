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
#ifndef CRPARTICLE_VARIABLERATE_COUNTER
#define CRPARTICLE_VARIABLERATE_COUNTER 1

#include <CRParticle/crCounter.h>
#include <CRCore/crRange.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>
#include <CRCore/crArgumentParser.h>

namespace CRParticle
{

    class crVariableRateCounter: public crCounter {
    public:
        inline crVariableRateCounter();
        inline crVariableRateCounter(const crVariableRateCounter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);
        
        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crVariableRateCounter"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crVariableRateCounter *>(obj) != 0; }
        
        inline const CRCore::rangef& getRateRange() const;
        inline void setRateRange(const CRCore::rangef& r);
        inline void setRateRange(float minrange, float maxrange);
        
		inline virtual void addCounterParam(int i, const std::string &str)
		{
			CRCore::crVector2f rr;
			switch(i) 
			{
			case 0:
				CRCore::crArgumentParser::appAtoVec(str,rr);
				//CRCore::notify(CRCore::FATAL)<<"crVariableRateCounter::addCounterParam(): "<<rr<<std::endl;
				setRateRange(rr[0], rr[1]);
				break;
			}
		}
    protected:
        virtual ~crVariableRateCounter() {}
        
    private:
        CRCore::rangef m_rate_range;
    };
    
    // INLINE FUNCTIONS
    
    inline crVariableRateCounter::crVariableRateCounter()
    : crCounter(), m_rate_range(1, 1)
    {
    }
    
    inline crVariableRateCounter::crVariableRateCounter(const crVariableRateCounter& copy, const CRCore::crCopyOp& copyop)
    : crCounter(copy, copyop), m_rate_range(copy.m_rate_range)
    {
    }
    
    inline const CRCore::rangef &crVariableRateCounter::getRateRange() const
    {
        return m_rate_range;
    }
    
    inline void crVariableRateCounter::setRateRange(const CRCore::rangef& r)
    {
        m_rate_range = r;
    }

    inline void crVariableRateCounter::setRateRange(float minrange, float maxrange)
    {
        m_rate_range.set(minrange, maxrange);
    }

}


#endif
