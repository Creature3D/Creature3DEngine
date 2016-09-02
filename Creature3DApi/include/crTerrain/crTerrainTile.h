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
//Modified by 吴财华
#ifndef CRTERRAIN_TERRAINTILE
#define CRTERRAIN_TERRAINTILE 1

#include <CRCore/crGroup.h>
#include <CRCore/crCoordinateSystemNode.h>

#include <CRIOManager/crReaderWriter.h>

#include <CRTerrain/crTerrainTechnique.h>
#include <CRTerrain/crLayer.h>
#include <CRTerrain/crLocator.h>

namespace CRTerrain {

class crTerrain;

class CRTERRAIN_EXPORT crTileID
{
    public:
    
        crTileID();

        crTileID(int in_level, int in_x, int in_y);
            
        bool operator == (const crTileID& rhs) const        
        {
            return (level==rhs.level) && (x==rhs.x) && (y==rhs.y);
        }

        bool operator != (const crTileID& rhs) const        
        {
            return (level!=rhs.level) || (x!=rhs.x) || (y!=rhs.y);
        }

        bool operator < (const crTileID& rhs) const
        {
            if (level<rhs.level) return true;
            if (level>rhs.level) return false;
            if (x<rhs.x) return true;
            if (x>rhs.x) return false;
            return y<rhs.y;
        }
        
        bool valid() const { return level>=0; }

        int level;
        int x;
        int y;
};


/** crTerrain provides a framework for loosely coupling height field data with height rendering algorithms.
  * This allows crTerrainTechnique's to be plugged in at runtime.*/
class CRTERRAIN_EXPORT crTerrainTile : public CRCore::crGroup
{
    public:

        crTerrainTile();
        
        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTerrainTile(const crTerrainTile&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

        META_Node(CRTerrain, crTerrainTile);

        virtual void traverse(CRCore::crNodeVisitor& nv);

        /** Call init on any attached crTerrainTechnique.*/
        void init();


        /** Set the crTerrain that this crTerrain tile is a member of.*/
        void setTerrain(crTerrain* ts);

        /** Get the crTerrain that this crTerrain tile is a member of.*/
        crTerrain* getTerrain() { return m_terrain; }

        /** Get the const crTerrain that this crTerrain tile is a member of.*/
        const crTerrain* getTerrain() const { return m_terrain; }

        
        /** Set the crTileID (layer, x,y) of the crTerrainTile.
          * The crTileID is used so it can be located by its neighbours 
          * via the enclosing crTerrain node that manages a map of crTileID to TerraiTiles.*/
        void setTileID(const crTileID& tileID);
        
        /** Get the crTileID (layer, x,y) of the crTerrainTile.*/
        const crTileID& getTileID() const { return m_tileID; }
        

        /** Set the crTerrainTechnique*/
        void setTerrainTechnique(crTerrainTechnique* terrainTechnique);

        /** Get the crTerrainTechnique*/
        crTerrainTechnique* getTerrainTechnique() { return m_terrainTechnique.get(); }
        
        /** Get the const crTerrainTechnique*/
        const crTerrainTechnique* getTerrainTechnique() const { return m_terrainTechnique.get(); }


        /** Set the coordinate frame locator of the terrain node.
          * The locator takes non-dimensional s,t coordinates into the X,Y,Z world coords and back.*/
        void setLocator(crLocator* locator) { m_locator = locator; }

        /** Get the coordinate frame locator of the terrain node.*/
        crLocator* getLocator() { return m_locator.get(); }

        /** Get the coordinate frame locator of the terrain node.*/
        const crLocator* getLocator() const { return m_locator.get(); }

        /** Set the layer to use to define the elevations of the terrain.*/
        void setElevationLayer(crLayer* layer);

        /** Get the layer to use to define the elevations of the terrain.*/
        crLayer* getElevationLayer() { return m_elevationLayer.get(); }

        /** Get the const layer to use to define the elevations of the terrain.*/
        const crLayer* getElevationLayer() const { return m_elevationLayer.get(); }


        /** Set a color layer with specified layer number.*/
        void setColorLayer(unsigned int i, crLayer* layer);

        /** Get color layer with specified layer number.*/
        crLayer* getColorLayer(unsigned int i) { return i<m_colorLayers.size() ? m_colorLayers[i].get() : 0; }

        /** Set const color layer with specified layer number.*/
        const crLayer* getColorLayer(unsigned int i) const { return i<m_colorLayers.size() ? m_colorLayers[i].get() : 0; }

        /** Get the number of colour layers.*/
        unsigned int getNumColorLayers() const { return m_colorLayers.size(); }
        
        
        /** Set hint to whether the crTerrainTechnique should create per vertex normals for lighting purposes.*/
        void setRequiresNormals(bool flag) { m_requiresNormals = flag; }

        /** Get whether the crTerrainTechnique should create per vertex normals for lighting purposes.*/
        bool getRequiresNormals() const { return m_requiresNormals; }


        /** Set the hint to whether the crTerrainTechnique should treat the invalid crLayer entries that at are neigbours to valid entries with the default value.*/
        void setTreatBoundariesToValidDataAsDefaultValue(bool flag) { m_treatBoundariesToValidDataAsDefaultValue = flag; }

        /** Get whether the TeatBoundariesToValidDataAsDefaultValue hint.*/
        bool getTreatBoundariesToValidDataAsDefaultValue() const { return m_treatBoundariesToValidDataAsDefaultValue; }

        /** Set the dirty flag on/off.*/
        void setDirty(bool dirty);

        /** return true if the tile is dirty and needs to be updated,*/
        bool getDirty() const { return m_dirty; }

        /** Callback for post processing loaded crTerrainTile, and for filling in missing elements such as external external imagery.*/
        struct TileLoadedCallback : public CRCore::Referenced
        {
            virtual bool deferExternalLayerLoading() const = 0;
            virtual void loaded(CRTerrain::crTerrainTile* tile, const CRIOManager::crReaderWriter::Options* options) const = 0;
        };
        
        static void setTileLoadedCallback(TileLoadedCallback* lc);
        static CRCore::ref_ptr<TileLoadedCallback>& getTileLoadedCallback();

		virtual void releaseObjects(CRCore::crState* state);
		//virtual void releaseOde();
		//void setHeightfieldDataID(void *id);
    protected:

        virtual ~crTerrainTile();

		virtual bool computeBound() const;

        typedef std::vector< CRCore::ref_ptr<crLayer> > Layers;

        friend class crTerrain;

        crTerrain*                            m_terrain;
        
        bool                                m_dirty;
        bool                                m_hasBeenTraversal;
        
        crTileID                              m_tileID;

        CRCore::ref_ptr<crTerrainTechnique>      m_terrainTechnique;
        CRCore::ref_ptr<crLocator>               m_locator;
        
        CRCore::ref_ptr<crLayer>                 m_elevationLayer;

        Layers                              m_colorLayers;
        
        bool                                m_requiresNormals;
        bool                                m_treatBoundariesToValidDataAsDefaultValue;
		//mutable void*          m_heightid;
};

/** Helper callback for managing optional sets of layers, that loading of is deffered to this callback,
  * with this callback working out which layers to load, and how to create fallback versions of the layers.
*/
class CRTERRAIN_EXPORT crWhiteListTileLoadedCallback : public crTerrainTile::TileLoadedCallback
{
    public:

        crWhiteListTileLoadedCallback();

        void allow(const std::string& setname) { m_setWhiteList.insert(setname); }
        
        void setMinimumNumOfLayers(unsigned int numLayers) { m_minumumNumberOfLayers = numLayers; }
        unsigned int getMinimumNumOfLayers() const { return m_minumumNumberOfLayers; }

        void setReplaceSwitchLayer(bool replaceSwitchLayer) { m_replaceSwitchLayer = replaceSwitchLayer; }
        bool getReplaceSwitchLayer() const { return m_replaceSwitchLayer; }

        void setAllowAll(bool allowAll) { m_allowAll = allowAll; }
        bool getAllowAll() const { return m_allowAll; }

        bool layerAcceptable(const std::string& setname) const;
        bool readImageLayer(CRTerrain::crImageLayer* imageLayer, const CRIOManager::crReaderWriter::Options* options) const;

        virtual bool deferExternalLayerLoading() const;

        virtual void loaded(CRTerrain::crTerrainTile* tile, const CRIOManager::crReaderWriter::Options* options) const;
    
    protected:

        virtual ~crWhiteListTileLoadedCallback();
    
        typedef std::set<std::string> SetWhiteList;
        SetWhiteList    m_setWhiteList;
        unsigned int    m_minumumNumberOfLayers;
        bool            m_replaceSwitchLayer;
        bool            m_allowAll;

};

}

#endif
