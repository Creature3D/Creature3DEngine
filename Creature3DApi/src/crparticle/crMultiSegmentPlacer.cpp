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
#include <CRParticle/crMultiSegmentPlacer.h>
#include <CRParticle/crPlacer.h>

#include <CRCore/crCopyOp.h>
#include <CRCore/crVector3.h>

CRParticle::crMultiSegmentPlacer::crMultiSegmentPlacer()
: crPlacer(), m_total_length(0)
{
}
    
CRParticle::crMultiSegmentPlacer::crMultiSegmentPlacer(const crMultiSegmentPlacer& copy, const CRCore::crCopyOp& copyop)
: crPlacer(copy, copyop), m_vx(copy.m_vx), m_total_length(copy.m_total_length)
{
}

void CRParticle::crMultiSegmentPlacer::recompute_length()
{
    Vertex_vector::iterator i;
    Vertex_vector::iterator i0 = m_vx.begin();
        
    m_total_length = 0;
    for (i=m_vx.begin(); i!=m_vx.end(); ++i)
    {
        m_total_length += (i->first - i0->first).length();
        i->second = m_total_length;
        i0 = i;            
    }
}

void CRParticle::crMultiSegmentPlacer::place(crParticle* P) const
{
    if (m_vx.size() >= 2) {
        float x = CRCore::rangef(0, m_total_length).get_random();
        
        Vertex_vector::const_iterator i;
        Vertex_vector::const_iterator i0 = m_vx.begin();
        const Vertex_vector::const_iterator vend = m_vx.end();

        for (i=m_vx.begin(); i!=vend; ++i)
        {
            if (x <= i->second)
            {
                float t = (x - i0->second) / (i->second - i0->second);
                P->setPosition(i0->first + (i->first - i0->first) * t);
                return;
            }
            i0 = i;
        }            
    }
    else
    {
        CRCore::notify(CRCore::WARN) << "this crMultiSegmentPlacer has less than 2 vertices\n";
    }
}
void CRParticle::crMultiSegmentPlacer::reset()
{
    m_vx.clear();
	recompute_length();
}
