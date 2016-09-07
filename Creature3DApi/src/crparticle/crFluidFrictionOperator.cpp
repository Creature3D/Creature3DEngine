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
#include <CRParticle/crFluidFrictionOperator.h>
#include <CRParticle/crModularProgram.h>
#include <CRParticle/crOperator.h>
#include <CRParticle/crParticle.h>
#include <CRCore/crNotify.h>

CRParticle::crFluidFrictionOperator::crFluidFrictionOperator():
     crOperator(),
     m_coeff_A(0),
     m_coeff_B(0),
     m_density(0),
     m_viscosity(0),
     m_ovr_rad(0),
     m_current_program(0)
{
    setFluidToAir();
}

CRParticle::crFluidFrictionOperator::crFluidFrictionOperator(const crFluidFrictionOperator& copy, const CRCore::crCopyOp& copyop)
:    crOperator(copy, copyop),
    m_coeff_A(copy.m_coeff_A),
    m_coeff_B(copy.m_coeff_B),
    m_density(copy.m_density), 
    m_viscosity(copy.m_viscosity),
    m_ovr_rad(copy.m_ovr_rad),
    m_current_program(0)
{
}

void CRParticle::crFluidFrictionOperator::operate(crParticle* P, double dt)
{
    float r = (m_ovr_rad > 0)? m_ovr_rad : P->getRadius();
    CRCore::crVector3 v = P->getVelocity()-m_wind;

    float vm = v.length();
	v.normalize();
    float R = m_coeff_A * r * vm + m_coeff_B * r * r * vm * vm;
    
    CRCore::crVector3 Fr(-R * v.x(), -R * v.y(), -R * v.z());

    if (m_current_program->getReferenceFrame() == crModularProgram::RELATIVE_RF) {
        Fr = m_current_program->rotateLocalToWorld(Fr);
    }

    // correct unwanted velocity increments
    CRCore::crVector3 dv = Fr * P->getMassInv() * dt;
    float dvl = dv.length();
    if (dvl > vm) {
        dv *= vm / dvl;
    }

    P->addVelocity(dv);
}
