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
#include <CRParticle/crModularEmitter.h>
#include <CRParticle/crEmitter.h>
#include <CRParticle/crConnectedParticleSystem.h>
#include <CRCore/crNotify.h>

CRParticle::crModularEmitter::crModularEmitter()
:    crEmitter(),
    m_numParticleToCreateMovementCompensationRatio(0.0f),
    m_counter(new crRandomRateCounter),
    m_placer(new crPointPlacer),
    m_shooter(new crRadialShooter)
{
}
    
CRParticle::crModularEmitter::crModularEmitter(const crModularEmitter& copy, const CRCore::crCopyOp& copyop):
    crEmitter(copy, copyop),
    m_numParticleToCreateMovementCompensationRatio(copy.m_numParticleToCreateMovementCompensationRatio)//,
    //m_counter(static_cast<crCounter *>(copyop(copy.m_counter.get()))), 
    //m_placer(static_cast<crPlacer *>(copyop(copy.m_placer.get()))), 
    //m_shooter(static_cast<crShooter *>(copyop(copy.m_shooter.get())))
{
	m_counter = dynamic_cast<crCounter *>(copy.m_counter->clone(copyop));
	m_placer = dynamic_cast<crPlacer *>(copy.m_placer->clone(copyop));
	m_shooter = dynamic_cast<crShooter *>(copy.m_shooter->clone(copyop));
}

void CRParticle::crModularEmitter::emit(double dt) 
{
    crConnectedParticleSystem* cps = dynamic_cast<crConnectedParticleSystem*>(getParticleSystem());

    if (getReferenceFrame() == RELATIVE_RF)
    {
        const CRCore::crMatrix& ltw = getMatrix();
        const CRCore::crMatrix& previous_ltw = getPreviousMatrix();

        int n = m_counter->numParticlesToCreate(dt);

        if (m_numParticleToCreateMovementCompensationRatio>0.0f)
        {
            // compute the distance moved between frames
            const CRCore::crVector3 controlPosition = m_placer->getControlPosition();
            CRCore::crVector3 previousPosition = controlPosition * previous_ltw;
            CRCore::crVector3 currentPosition = controlPosition * ltw;
            float distance = (currentPosition-previousPosition).length();

            float size = getUseDefaultTemplate() ? 
                        getParticleSystem()->getDefaultParticleTemplate().getSizeRange().minimum.length() :
                        getParticleTemplate().getSizeRange().minimum.length();

            float num_extra_samples = m_numParticleToCreateMovementCompensationRatio*distance/size;
            float rounded_down = floor(num_extra_samples);
            float remainder = num_extra_samples-rounded_down;

            n = CRCore::maximum(n, int(rounded_down) +  (((float) rand() < remainder * (float)RAND_MAX) ? 1 : 0));
        }
        
        for (int i=0; i<n; ++i)
        {
            crParticle* P = getParticleSystem()->createParticle(getUseDefaultTemplate()? 0: &getParticleTemplate());
            if (P)
            {
                m_placer->place(P);
                m_shooter->shoot(P);
                
                // now need to transform the position and velocity because we having a moving model.
                float r = ((float)rand()/(float)RAND_MAX);
                P->transformPositionVelocity(ltw, previous_ltw, r);
                //P->transformPositionVelocity(ltw);
                
                if (cps) P->setUpTexCoordsAsPartOfConnectedParticleSystem(cps);
                
            }
            else
            {
                CRCore::notify(CRCore::NOTICE)<<"run out of particle"<<std::endl;
            }
        }
    }
    else
    {
        int n = m_counter->numParticlesToCreate(dt);
        for (int i=0; i<n; ++i)
        {
            crParticle* P = getParticleSystem()->createParticle(getUseDefaultTemplate()? 0: &getParticleTemplate());
            if (P)
            {
                m_placer->place(P);
                m_shooter->shoot(P);

                if (cps) P->setUpTexCoordsAsPartOfConnectedParticleSystem(cps);
            }
        }
    }
}
