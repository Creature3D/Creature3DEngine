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
#ifndef CRPARTICLE_EMITTER
#define CRPARTICLE_EMITTER 1

#include <CRParticle/crExport.h>
#include <CRParticle/crParticleProcessor.h>
#include <CRParticle/crParticle.h>

#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crCopyOp.h>

namespace CRParticle
{

    /**    An abstract base class for particle emitters.
        Descendant classes must override the <CODE>emit()</CODE> method to generate new particles by
        calling the <CODE>crParticleSystem::createParticle()</CODE> method on the particle system associated
        to the emitter.
    */
    class CRPARTICLE_EXPORT crEmitter: public crParticleProcessor {
    public:
        crEmitter();
        crEmitter(const crEmitter& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crEmitter"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crEmitter*>(obj) != 0; }
        
        /// Get the particle template.
        inline const crParticle& getParticleTemplate() const;
        
        /// Set the particle template (particle is copied).
        inline void setParticleTemplate(const crParticle& p);
        
        /// Return whether the particle system's default template should be used.
        inline bool getUseDefaultTemplate() const;
        
        /** Set whether the default particle template should be used.
            When this flag is true, the particle template is ignored, and the
            particle system's default template is used instead.
        */
        inline void setUseDefaultTemplate(bool v);

    protected:
        virtual ~crEmitter() {}
        crEmitter& operator=(const crEmitter&) { return *this; }
        
        inline void process(double dt);
        
        virtual void emit(double dt) = 0;
        
        bool m_usedeftemp;
        crParticle m_ptemp;
    };

    // INLINE FUNCTIONS

    inline const crParticle& crEmitter::getParticleTemplate() const
    {
        return m_ptemp;
    }

    inline void crEmitter::setParticleTemplate(const crParticle& p)
    {
        m_ptemp = p;
        m_usedeftemp = false;
    }

    inline bool crEmitter::getUseDefaultTemplate() const
    {
        return m_usedeftemp;
    }

    inline void crEmitter::setUseDefaultTemplate(bool v)
    {
        m_usedeftemp = v;
    }

    inline void crEmitter::process(double dt)
    {
        emit(dt);
    }


}


#endif

