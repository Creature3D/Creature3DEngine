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
#include <CRParticle/crFluidProgram.h>
using namespace CRParticle;

CRParticle::crFluidProgram::crFluidProgram():
    crProgram()
{
	setFluidType(Air);
    //setFluidToAir();
}

CRParticle::crFluidProgram::crFluidProgram(const crFluidProgram& copy, const CRCore::crCopyOp& copyop):
    crProgram(copy, copyop),
    m_acceleration(copy.m_acceleration),
    m_viscosity(copy.m_viscosity),
    m_density(copy.m_density),
    m_wind(copy.m_wind),
    m_viscosityCoefficient(copy.m_viscosityCoefficient),
    m_densityCoefficeint(copy.m_densityCoefficeint),
	m_fluidType(copy.m_fluidType)
{
}

void CRParticle::crFluidProgram::execute(double dt)
{
    const float four_over_three = 4.0f/3.0f;
    crParticleSystem* ps = getParticleSystem();
    int n = ps->numParticles();
    for (int i=0; i<n; ++i)
    {
        crParticle* particle = ps->getParticle(i);
        if (particle->isAlive())
        {
            float radius = particle->getRadius();
            float Area = CRCore::PI*radius*radius;
            float Volume = Area*radius*four_over_three;
        
            // compute force due to gravity + boyancy of displacing the fluid that the particle is emersed in.
            CRCore::crVector3 accel_gravity = m_acceleration * ((particle->getMass() - m_density*Volume) * particle->getMassInv());
            
            // compute force due to friction
            CRCore::crVector3 velBefore = particle->getVelocity();
            CRCore::crVector3 relative_wind = particle->getVelocity()-m_wind;            
            CRCore::crVector3 wind_force = - relative_wind * Area * (m_viscosityCoefficient + m_densityCoefficeint*relative_wind.length());
            CRCore::crVector3 wind_accel = wind_force * particle->getMassInv();

            double critical_dt2 = relative_wind.length2()/wind_accel.length2();
            double compenstated_dt = dt;
            if (critical_dt2 < dt*dt)
            {
                // CRCore::notify(CRCore::NOTICE)<<"** Could be critical: dt="<<dt<<" critical_dt="<<sqrtf(critical_dt2)<<std::endl;
                compenstated_dt = sqrtf(critical_dt2)*0.8f;
            }

            particle->addVelocity(accel_gravity*dt + wind_accel*compenstated_dt);
            

        }
    }
}
void crFluidProgram::setFluidType(FluidType fluidtype)
{
	switch (fluidtype)
	{
	case Air:
		setFluidToAir();
		break;
	case Water:
		setFluidToWater();
		break;
	}
	m_fluidType = fluidtype;
}
void crFluidProgram::setFluidToAir()
{
	setToGravity(1.0f);
	setFluidDensity(1.2929f);
	setFluidViscosity(1.8e-5f);
}
void crFluidProgram::setFluidToWater()
{
	setToGravity(1.0f);
	setFluidDensity(1.0f);
	setFluidViscosity(1.002e-3f);
}