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
#ifndef CRPARTICLE_COUNTER
#define CRPARTICLE_COUNTER 1

#include <CRCore/crCopyOp.h>
#include <CRCore/crBase.h>

namespace CRParticle
{

    class crCounter: public CRCore::crBase {
    public:
        inline crCounter();
        inline crCounter(const crCounter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crCounter"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crCounter* >(obj) != 0; }

        virtual int numParticlesToCreate(double dt) const = 0;

		virtual void addCounterParam(int i, const std::string &str) = 0;
		virtual void reset(){}
    protected:
        ~crCounter() {}
        crCounter &operator=(const crCounter &) { return *this; }
    };
    
    // INLINE FUNCTIONS
    
    inline crCounter::crCounter()
    : CRCore::crBase()
    {
    }
    
    inline crCounter::crCounter(const crCounter& copy, const CRCore::crCopyOp& copyop)
    : CRCore::crBase(copy, copyop)
    {
    }

}

#endif
