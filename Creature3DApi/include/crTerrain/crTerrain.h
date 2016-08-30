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
#ifndef CRTERRAIN_CRTerrain_H
#define CRTERRAIN_CRTerrain_H 1

#include <CRCore/crGroup.h>
#include <CRCore/crCoordinateSystemNode.h>

#include <CRTerrain/crTerrainTile.h>

namespace CRTerrain {

/** crTerrain provides a framework for loosely coupling height field data with height rendering algorithms.
  * This allows crTerrainTechnique's to be plugged in at runtime.*/
class CRTERRAIN_EXPORT crTerrain : public CRCore::crGroup
{
    public:

        crTerrain();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTerrain(const crTerrain&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Node(CRTerrain, crTerrain);

        virtual void traverse(CRCore::crNodeVisitor& nv);
        
        /** Set the sample ratio hint that crTerrainTile should use when building geometry.
          * Defaults to 1.0, which means use all original sample points.*/
        void setSampleRatio(float ratio) 
        { 
            m_sampleRatio  = ratio;
            dirtyRegisteredTiles();
        }

        /** Get the sample ratio hint.*/
        float getSampleRatio() const { return m_sampleRatio; }


        /** Set the vertical scale hint.*/
        void setVerticalScale(float scale) 
        { 
            m_verticalScale = scale;
            dirtyRegisteredTiles();
        }
    
        /** Get the vertical scale hint.*/
        float getVerticalScale() const { return m_verticalScale; }
        
        /** Get the crTerrainTile for a given crTileID.*/
        crTerrainTile* getTile(const crTileID& tileID);

        /** Get the const crTerrainTile for a given crTileID.*/
        const crTerrainTile* getTile(const crTileID& tileID) const;
        
        /** Set the crTerrainTechnique prototype from which TerrainTiles can clone the techniques from.*/
        void setTerrainTechniquePrototype(crTerrainTechnique* technique) { m_terrainTechnique = technique; }

        /** Get the crTerrainTechnique prototype */
        crTerrainTechnique* getTerrainTechniquePrototype() { return m_terrainTechnique.get(); }
        
        /** Get the const crTerrainTechnique protype*/
        const crTerrainTechnique* getTerrainTechniquePrototype() const { return m_terrainTechnique.get(); }

    protected:

        virtual ~crTerrain();
        
        friend class crTerrainTile;
        
        void dirtyRegisteredTiles();

        void registerTerrainTile(crTerrainTile* tile);
        void unregisterTerrainTile(crTerrainTile* tile);

        typedef std::map< crTileID, crTerrainTile* >    TerrainTileMap;
        typedef std::set< crTerrainTile* >            TerrainTileSet;

        float                               m_sampleRatio;
        float                               m_verticalScale;

        mutable CRCore::crMutex          m_mutex;
        TerrainTileSet                      m_terrainTileSet;
        TerrainTileMap                      m_terrainTileMap;

        CRCore::ref_ptr<crTerrainTechnique>      m_terrainTechnique;
};

}

#endif
