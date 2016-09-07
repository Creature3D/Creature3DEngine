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
#include <CRParticle/crEmitter.h>
#include <CRParticle/crParticleProcessor.h>

#include <CRCore/crCopyOp.h>

CRParticle::crEmitter::crEmitter()
:    crParticleProcessor(), 
    m_usedeftemp(true)
{
}

CRParticle::crEmitter::crEmitter(const crEmitter& copy, const CRCore::crCopyOp& copyop)
:     crParticleProcessor(copy, copyop),
    m_usedeftemp(copy.m_usedeftemp), 
    m_ptemp(copy.m_ptemp)
{
}
