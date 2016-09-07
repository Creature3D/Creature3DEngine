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
#include <CRParticle/crParticleProcessor.h>

#include <CRCore/crNode.h>
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crCopyOp.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crNotify.h>

//#include <CRUtil/crCullVisitor.h>


using namespace CRCore;

CRParticle::crParticleProcessor::crParticleProcessor()
:   m_rf(ABSOLUTE_RF),
    m_enabled(true),
    m_ps(0),
    m_endless(true),
    m_lifeTime(0.0),
    m_startTime(0.0),
    m_currentTime(0.0),
    m_resetTime(0.0)
{
}

CRParticle::crParticleProcessor::crParticleProcessor(const crParticleProcessor& copy, const CRCore::crCopyOp& copyop)
:    CRCore::crBase(copy, copyop),
    m_rf(copy.m_rf),
    m_enabled(copy.m_enabled),
    m_ps(static_cast<crParticleSystem* >(copyop(copy.m_ps.get()))),
	m_endless(copy.m_endless),
    m_lifeTime(copy.m_lifeTime),
    m_startTime(copy.m_startTime),
    m_currentTime(copy.m_currentTime),
    m_resetTime(copy.m_resetTime)
{
}