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
#include <CRCore/crNotify.h>
#include <CRCore/crCullingSet.h>
//#include <CRCore/io_utils.h>
#include <CRParticle/crConnectedParticleSystem.h>

using namespace CRParticle;

crConnectedParticleSystem::crConnectedParticleSystem():
    m_startParticle(crParticle::INVALID_INDEX),
    m_lastParticleCreated(crParticle::INVALID_INDEX)
{
}

crConnectedParticleSystem::crConnectedParticleSystem(const crConnectedParticleSystem& copy, const CRCore::crCopyOp& copyop):
    crParticleSystem(copy,copyop),
    m_startParticle(crParticle::INVALID_INDEX),
    m_lastParticleCreated(crParticle::INVALID_INDEX)
{
}

crConnectedParticleSystem::~crConnectedParticleSystem()
{
}

crParticle* crConnectedParticleSystem::createParticle(const crParticle* ptemplate)
{
    // CRCore::notify(CRCore::NOTICE)<<this<< " Creating particle "<<std::endl;

    crParticle* particle = crParticleSystem::createParticle(ptemplate);
    int particleIndex = crParticleSystem::getParticleIndex(particle);//(int)(particle - &m_particles[0]);
    
    if (particle)
    {
    
        if (m_startParticle == crParticle::INVALID_INDEX)
        {
            // we are the fisrt particle create, so start the connect particle list 
            m_startParticle = particleIndex;
        }

        if (m_lastParticleCreated != crParticle::INVALID_INDEX)
        {
            // CRCore::notify(CRCore::NOTICE)<<this<< " Connecting "<<m_lastParticleCreated<<" to "<<particleIndex<<std::endl;

            // write up the last created particle to this new particle
            m_particles[m_lastParticleCreated]->setNextParticle(particleIndex);
            particle->setPreviousParticle(m_lastParticleCreated);
        }

        // set the new particle as the last particle created.
        m_lastParticleCreated = particleIndex;
        
    }
    
    return particle;
}
        
void crConnectedParticleSystem::reuseParticle(int particleIndex)
{
    // CRCore::notify(CRCore::NOTICE)<<this<< " Reusing particle "<<particleIndex<<std::endl;

    if (particleIndex<0 || particleIndex>=(int)m_particles.size()) return;
    
    crParticle* particle = m_particles[particleIndex].get();
    int previous = particle->getPreviousParticle();
    int next = particle->getNextParticle();

    // update start and last entries
    if (m_startParticle == particleIndex)
    {
        m_startParticle = particle->getNextParticle();
    }
    
    if (m_lastParticleCreated == particleIndex)
    {
        m_lastParticleCreated = crParticle::INVALID_INDEX;
    }
    
    // join up the previous and next particles to account for
    // the deletion of the this particle
    if (previous != crParticle::INVALID_INDEX)
    {
        m_particles[previous]->setNextParticle(next);
    }
    
    if (next != crParticle::INVALID_INDEX)
    {
        m_particles[next]->setPreviousParticle(previous);
    }

    // reset the next and previous particle entries of this particle
    particle->setPreviousParticle(crParticle::INVALID_INDEX);
    particle->setNextParticle(crParticle::INVALID_INDEX);
    
    // do the actual destroy of the particle
    crParticleSystem::destroyParticle(particleIndex);
}

void crConnectedParticleSystem::drawImplementation(CRCore::crState& state) const
{
    const crParticle* particle = (m_startParticle_buf != crParticle::INVALID_INDEX) ? m_particles_buf[m_startParticle_buf].get() : 0;
    if (!particle) return;
    
    CRCore::crVector4 pixelSizeVector = CRCore::crCullingSet::computePixelSizeVector(*state.getCurrentViewport(),state.getProjectionMatrix(),state.getModelViewMatrix());
    float unitPixelSize = fabs(1.0/(particle->getBufPosition()*pixelSizeVector));
    float pixelSizeOfFirstParticle = unitPixelSize * particle->getBufCurrentSize().length();
    //float desiredGapBetweenDrawnParticles = 50.0f/unitPixelSize;
    //float desiredGapBetweenDrawnParticles2 = desiredGapBetweenDrawnParticles*desiredGapBetweenDrawnParticles;
    unsigned int maxNumParticlesToSkip = 200;
    float maxPixelError2 = CRCore::square(1.0f/unitPixelSize);
    
    if (pixelSizeOfFirstParticle<1.0)
    {
        // draw the connected particles as a line
        glBegin(GL_LINE_STRIP);
        while(particle != 0)
        {

            const CRCore::crVector4& color = particle->getBufCurrentColor();
            const CRCore::crVector3& pos = particle->getBufPosition();
            //glColor4f( color.r(), color.g(), color.b(), color.a() * particle->getBufCurrentAlpha());
			state.applyColor(CRCore::crVector4(color.r(), color.g(), color.b(), color.a() * particle->getBufCurrentAlpha()));
            glTexCoord2f( particle->getBufSTexCoord(), 0.5f );
            glVertex3fv(pos.ptr());

            const crParticle* nextParticle = (particle->getBufNextParticle() != crParticle::INVALID_INDEX) ? m_particles_buf[particle->getBufNextParticle()].get() : 0;
            if (nextParticle)
            {
                const CRCore::crVector3& nextPos = nextParticle->getBufPosition();
                CRCore::crVector3 startDelta = nextPos-pos;
                startDelta.normalize();
                float distance2 = 0.0;

                // now skip particles of required
                for(unsigned int i=0;
                    i<maxNumParticlesToSkip && ((distance2<maxPixelError2) && (nextParticle->getBufNextParticle()!=crParticle::INVALID_INDEX));
                    ++i)
                {
                    nextParticle = m_particles_buf[nextParticle->getBufNextParticle()].get();
                    const CRCore::crVector3& nextPos = nextParticle->getBufPosition();
                    CRCore::crVector3 delta = nextPos-pos;
                    distance2 = (delta^startDelta).length2();
                }
            }
            particle = nextParticle;
        }
        glEnd();
    }
    else
    {

        // draw the connected particles as a quad stripped aligned to be orthogonal to the eye 
        CRCore::crMatrix eyeToLocalTransform;
        eyeToLocalTransform.invert(state.getModelViewMatrix());
        CRCore::crVector3 eyeLocal = CRCore::crVector3(0.0f,0.0,0.0f)*eyeToLocalTransform;

        CRCore::crVector3 delta(0.0f,0.0f,1.0f);
        
        glBegin(GL_QUAD_STRIP);
        while(particle != 0)
        {
            const CRCore::crVector4& color = particle->getBufCurrentColor();
            const CRCore::crVector3& pos = particle->getBufPosition();

            const crParticle* nextParticle = (particle->getBufNextParticle() != crParticle::INVALID_INDEX) ? m_particles_buf[particle->getBufNextParticle()].get() : 0;
                        
            if (nextParticle)
            {
                const CRCore::crVector3& nextPos = nextParticle->getBufPosition();
                CRCore::crVector3 startDelta = nextPos-pos;
                startDelta.normalize();
                float distance2 = 0.0;

                // now skip particles of required
                for(unsigned int i=0;
                    i<maxNumParticlesToSkip && ((distance2<maxPixelError2) && (nextParticle->getBufNextParticle()!=crParticle::INVALID_INDEX));
                    ++i)
                {
                    nextParticle = m_particles_buf[nextParticle->getBufNextParticle()].get();
                    const CRCore::crVector3& nextPos = nextParticle->getBufPosition();
                    delta = nextPos-pos;
                    distance2 = (delta^startDelta).length2();
                }

                delta = nextPos-pos;
            }

            const CRCore::crVector3 &s = particle->getBufCurrentSize();
            CRCore::crVector3 normal( delta ^ (pos-eyeLocal));
            normal.normalize();
            normal[0] *= s[0];
			normal[1] *= s[1];
			normal[2] *= s[2];

            CRCore::crVector3 bottom(pos-normal);
            CRCore::crVector3 top(pos+normal);
            
            //glColor4f( color.r(), color.g(), color.b(), color.a() * particle->getBufCurrentAlpha());
			state.applyColor(CRCore::crVector4(color.r(), color.g(), color.b(), color.a() * particle->getBufCurrentAlpha()));
            
            glTexCoord2f( particle->getBufSTexCoord(), 0.0f );
            glVertex3fv(bottom.ptr());

            glTexCoord2f( particle->getBufSTexCoord(), 1.0f );
            glVertex3fv(top.ptr());

            particle = nextParticle;
        }
        glEnd();
    }

}
