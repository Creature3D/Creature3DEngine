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
#ifndef CRPARTICLE_PROGRAM
#define CRPARTICLE_PROGRAM 1

#include <CRParticle/crExport.h>
#include <CRParticle/crParticleProcessor.h>

#include <CRCore/crBase.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crCopyOp.h>

namespace CRParticle
{

    /**    An abstract <CODE>crParticleProcessor</CODE> descendant for modifying particles "on the fly" 
        during the cull traversal.
        Descendants of this class must implement the <CODE>execute()</CODE> method, which should iterate 
        through all particles in the linked particle system and modify them somehow 
        (usually updating their velocity vector).
    */
    class CRPARTICLE_EXPORT crProgram: public crParticleProcessor
    {
    public:
        crProgram();
        crProgram(const crProgram& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        virtual const char* libraryName() const { return "CRParticle"; }
        virtual const char* className() const { return "crProgram"; }
        virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crProgram*>(obj) != 0; }

    protected:
        virtual ~crProgram() {}
        crProgram& operator=(const crProgram&) { return *this; }

        /// Implementation of <CODE>crParticleProcessor::process()</CODE>. Do not call this method by yourself.
        inline void process(double dt);
        
        /// Execute the program on the particle system. Must be overriden in descendant classes.
        virtual void execute(double dt) = 0;

    private:
    };

    // INLINE FUNCTIONS

    inline void crProgram::process(double dt)
    {
        execute(dt);
    }
    
}

#endif
