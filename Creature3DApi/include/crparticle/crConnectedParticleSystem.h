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
#ifndef CRPARTICLE_CONNECTEDPARTICLESYSTEM
#define CRPARTICLE_CONNECTEDPARTICLESYSTEM 1

#include <CRParticle/crParticleSystem.h>

namespace CRParticle
{

    /** ConnectConnectedParticleSystem is a specialise crConnectedParticleSystem for effects 
      * like missle trails, where the individual particles are rendered as
      * single ribbon. 
    */
    class CRPARTICLE_EXPORT crConnectedParticleSystem: public CRParticle::crParticleSystem
    {
    public:
    
        crConnectedParticleSystem();
        crConnectedParticleSystem(const crConnectedParticleSystem& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY);

        META_Base(CRParticle, crConnectedParticleSystem);
        
        /// Create a new particle from the specified template (or the default one if <CODE>ptemplate</CODE> is null).
        virtual crParticle* createParticle(const crParticle* ptemplate);
        
        /// Reuse the i-th particle.
        virtual void reuseParticle(int i);
        
        /// Draw the connected particles as either a line or a quad strip, depending upon viewing distance. .
        virtual void drawImplementation(CRCore::crState& state) const;

		inline virtual void swapBuffers(int frameNumber)
		{
			if(m_swapFrameNumber != frameNumber)
			{
				m_startParticle_buf = m_startParticle;
				m_lastParticleCreated_buf = m_lastParticleCreated;
				crParticleSystem::swapBuffers(frameNumber);
				m_swapFrameNumber = frameNumber;
			}
		}
    protected:

        virtual ~crConnectedParticleSystem();

        crConnectedParticleSystem& operator=(const crConnectedParticleSystem&) { return *this; }

        int m_startParticle;
        int m_lastParticleCreated;

		int m_startParticle_buf;
		int m_lastParticleCreated_buf;
    };

}

#endif
