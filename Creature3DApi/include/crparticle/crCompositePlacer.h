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
#ifndef CRPARTICLE_COMPOSITEPLACER
#define CRPARTICLE_COMPOSITEPLACER

#include <CRParticle/crPlacer.h>
#include <CRParticle/crParticle.h>

namespace CRParticle
{
/** A composite particle placer which allows particles to be generated from a union of placers. */
class crCompositePlacer : public crPlacer
{
public:
    crCompositePlacer() : crPlacer() {}
    
    crCompositePlacer( const crCompositePlacer& copy, const CRCore::crCopyOp& copyop = CRCore::crCopyOp::SHALLOW_COPY )
    : crPlacer(copy, copyop), m_placers(copy.m_placers) {}
    
    META_Base( CRParticle, crCompositePlacer );
    
    // Set a child placer at specific index
    void setPlacer( unsigned int i, crPlacer* p )
    {
        if (i<m_placers.size()) m_placers[i] = p;
        else addPlacer(p);
    }
    
    /// Add a child placer
    void addPlacer( crPlacer* p ) { m_placers.push_back(p); }
    
    /// Remove a child placer
    void removePlacer( unsigned int i )
    { if (i<m_placers.size()) m_placers.erase(m_placers.begin()+i); }
    
    /// Get a child placer
    crPlacer* getPlacer( unsigned int i ) { return m_placers[i].get(); }
    const crPlacer* getPlacer( unsigned int i ) const { return m_placers[i].get(); }
    
    /// Get number of placers
    unsigned int getNumPlacers() const { return m_placers.size(); }
    
    /// Place a particle. Do not call it manually.
    inline void place( crParticle* P ) const;
    
    /// return the volume of the box
    inline float volume() const;
    
    /// return the control position
    inline CRCore::crVector3 getControlPosition() const;
    
protected:
    virtual ~crCompositePlacer() {}
    crCompositePlacer& operator=( const crCompositePlacer& ) { return *this; }
    
    typedef std::vector< CRCore::ref_ptr<crPlacer> > PlacerList;
    PlacerList m_placers;
};

// INLINE METHODS

inline void crCompositePlacer::place( crParticle* P ) const
{
    rangef sizeRange( 0.0f, volume() );
    float current = 0.0f, selected = sizeRange.get_random();
    for ( PlacerList::const_iterator itr=m_placers.begin(); itr!=m_placers.end(); ++itr )
    {
        current += (*itr)->volume();
        if ( selected<=current ) (*itr)->place( P );
    }
}

inline float crCompositePlacer::volume() const
{
    float total_size = 0.0f;
    for ( PlacerList::const_iterator itr=m_placers.begin(); itr!=m_placers.end(); ++itr )
        total_size += (*itr)->volume();
    return total_size;
}

inline CRCore::crVector3 crCompositePlacer::getControlPosition() const
{
    if ( !m_placers.size() ) return CRCore::crVector3();
    return m_placers.front()->getControlPosition();
}


}

#endif
