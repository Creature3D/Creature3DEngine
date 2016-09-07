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
#include <CRParticle/crModularProgram.h>
#include <CRParticle/crProgram.h>
#include <CRParticle/crParticleSystem.h>
#include <CRParticle/crParticle.h>

CRParticle::crModularProgram::crModularProgram()
: crProgram()
{
}

CRParticle::crModularProgram::crModularProgram(const crModularProgram& copy, const CRCore::crCopyOp& copyop)
: crProgram(copy, copyop)
{
    Operator_vector::const_iterator ci;
    for (ci=copy.m_operators.begin(); ci!=copy.m_operators.end(); ++ci) {
        m_operators.push_back(dynamic_cast<crOperator *>((*ci)->clone(copyop)));// static_cast<crOperator* >(copyop(ci->get())));
    }
}

void CRParticle::crModularProgram::execute(double dt)
{
    Operator_vector::iterator ci;
    Operator_vector::iterator ci_end = m_operators.end();

    crParticleSystem* ps = getParticleSystem();
    for (ci=m_operators.begin(); ci!=ci_end; ++ci) {        
        (*ci)->beginOperate(this);
        int n = ps->numParticles();
        for (int i=0; i<n; ++i) {
            crParticle* P = ps->getParticle(i);
            if (P->isAlive() && (*ci)->isEnabled()) {
                (*ci)->operate(P, dt);
            }
        }
        (*ci)->endOperate();
    }
}
void CRParticle::crModularProgram::reset()
{
    m_operators.clear();
}
void CRParticle::crModularProgram::replaceOperator(int i, crOperator* o)
{
	m_operators[i] = o;
}