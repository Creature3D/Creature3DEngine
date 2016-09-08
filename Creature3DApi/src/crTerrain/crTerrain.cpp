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
#include <CRTerrain/crTerrain.h>
#include <CRTerrain/crGeometryTechnique.h>

#include <CRCore/thread/crScopedLock.h>

using namespace CRCore;
using namespace CRTerrain;

crTerrain::crTerrain():
    m_sampleRatio(1.0),
    m_verticalScale(1.0)
{
    m_terrainTechnique = new crGeometryTechnique;
}

crTerrain::crTerrain(const crTerrain& ts, const CRCore::crCopyOp& copyop):
    CRCore::crGroup(ts,copyop),
    m_sampleRatio(ts.m_sampleRatio),
    m_verticalScale(ts.m_verticalScale),
    m_terrainTechnique(ts.m_terrainTechnique)
{
}


crTerrain::~crTerrain()
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    for(TerrainTileSet::iterator itr = m_terrainTileSet.begin();
        itr != m_terrainTileSet.end();
        ++itr)
    {
        const_cast<crTerrainTile*>(*itr)->m_terrain = 0;
    }
    
    m_terrainTileSet.clear();
    m_terrainTileMap.clear();
}

void crTerrain::traverse(CRCore::crNodeVisitor& nv)
{
    crGroup::traverse(nv);
}

crTerrainTile* crTerrain::getTile(const crTileID& tileID)
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    TerrainTileMap::iterator itr = m_terrainTileMap.find(tileID);
    if (itr == m_terrainTileMap.end()) return 0;
    
    return itr->second;
}

const crTerrainTile* crTerrain::getTile(const crTileID& tileID) const
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    TerrainTileMap::const_iterator itr = m_terrainTileMap.find(tileID);
    if (itr == m_terrainTileMap.end()) return 0;
    
    return itr->second;
}

void crTerrain::dirtyRegisteredTiles()
{
    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    for(TerrainTileSet::iterator itr = m_terrainTileSet.begin();
        itr != m_terrainTileSet.end();
        ++itr)
    {
        (const_cast<crTerrainTile*>(*itr))->setDirty(true);
    }
}

static unsigned int s_maxNumTiles = 0;
void crTerrain::registerTerrainTile(crTerrainTile* tile)
{
    if (!tile) return;

    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);
    
    if (tile->getTileID().valid())
    {
        m_terrainTileMap[tile->getTileID()] = tile;
    }
    
    m_terrainTileSet.insert(tile);

    if (m_terrainTileSet.size() > s_maxNumTiles) s_maxNumTiles = m_terrainTileSet.size();

    // CRCore::notify(CRCore::NOTICE)<<"crTerrain::registerTerrainTile "<<tile<<" total number of tile "<<m_terrainTileSet.size()<<" max = "<<s_maxNumTiles<<std::endl;
}

void crTerrain::unregisterTerrainTile(crTerrainTile* tile)
{
    if (!tile) return;

    CRCore::ScopedLock<CRCore::crMutex> lock(m_mutex);

    if (tile->getTileID().valid())
    {
        m_terrainTileMap.erase(tile->getTileID());
    }
    
    m_terrainTileSet.erase(tile);

    // CRCore::notify(CRCore::NOTICE)<<"crTerrain::unregisterTerrainTile "<<tile<<" total number of tile "<<m_terrainTileSet.size()<<" max = "<<s_maxNumTiles<<std::endl;
}
