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
//Modified by Îâ²Æ»ª
#include <CRTerrain/crTerrainTile.h>
#include <CRTerrain/crTerrain.h>
#include <CRTerrain/crGeometryTechnique.h>
#include <ode/ode.h>
#include <CRCore/crClusterCullingCallback.h>

#include <CRIOManager/crReadFile.h>


using namespace CRCore;
using namespace CRTerrain;

/////////////////////////////////////////////////////////////////////////////////
//
// crTileID
//
crTileID::crTileID():
    level(-1),
    x(-1),
    y(-1)
{
}

crTileID::crTileID(int in_level, int in_x, int in_y):
    level(in_level),
    x(in_x),
    y(in_y)
{
}

/////////////////////////////////////////////////////////////////////////////////
//
// crTerrainTile
//
void crTerrainTile::setTileLoadedCallback(crTerrainTile::TileLoadedCallback* lc)
{
    getTileLoadedCallback() = lc;
}

CRCore::ref_ptr<crTerrainTile::TileLoadedCallback>& crTerrainTile::getTileLoadedCallback()
{
    static CRCore::ref_ptr<TileLoadedCallback> s_TileLoadedCallback;
    return s_TileLoadedCallback;
}

crTerrainTile::crTerrainTile():
    m_terrain(0),
    m_dirty(false),
    m_hasBeenTraversal(false),
    m_requiresNormals(true),
    m_treatBoundariesToValidDataAsDefaultValue(false)
	//m_heightid(NULL)
{
//    setThreadSafeRefUnref(true);
	setCalcShadow(false);
	//setEnableIntersect(false);
}

crTerrainTile::crTerrainTile(const crTerrainTile& terrain,const CRCore::crCopyOp& copyop):
    crGroup(terrain,copyop),
    m_terrain(0),
    m_dirty(false),
    m_hasBeenTraversal(false),
    m_elevationLayer(terrain.m_elevationLayer),
    m_colorLayers(terrain.m_colorLayers),
    m_requiresNormals(terrain.m_requiresNormals),
    m_treatBoundariesToValidDataAsDefaultValue(terrain.m_treatBoundariesToValidDataAsDefaultValue)
	//m_heightid(NULL)
{
    if (terrain.getTerrainTechnique()) 
    {
        setTerrainTechnique(dynamic_cast<crTerrainTechnique*>(terrain.getTerrainTechnique()->cloneType()));
    }
}

crTerrainTile::~crTerrainTile()
{
    if (m_terrain) setTerrain(0);
}

void crTerrainTile::setTerrain(crTerrain* ts)
{
    if (m_terrain == ts) return;
    
    if (m_terrain) m_terrain->unregisterTerrainTile(this);
    
    m_terrain = ts;

    if (m_terrain) m_terrain->registerTerrainTile(this);
}

void crTerrainTile::setTileID(const crTileID& tileID)
{
    if (m_tileID == tileID) return;

    if (m_terrain) m_terrain->unregisterTerrainTile(this);

    m_tileID = tileID;

    if (m_terrain) m_terrain->registerTerrainTile(this);
}


void crTerrainTile::traverse(CRCore::crNodeVisitor& nv)
{
    if (!m_hasBeenTraversal)
    {
        if (!m_terrain)
        {
            CRCore::NodePath& nodePath = nv.getNodePath();
            if (!nodePath.empty())
            {
                for(CRCore::NodePath::reverse_iterator itr = nodePath.rbegin();
                    itr != nodePath.rend() && !m_terrain;
                    ++itr)
                {
                    CRTerrain::crTerrain* ts = dynamic_cast<crTerrain*>(*itr);
                    if (ts) 
                    {
                        //CRCore::notify(CRCore::INFO)<<"Assigning terrain system "<<ts<<std::endl;                        
                        setTerrain(ts);
                    }
                }
            }
        }
        
        //init();
                    
        m_hasBeenTraversal = true;
    }
	CRCore::crNodeVisitor::VisitorType vt = nv.getVisitorType();
	if (getDirty())
	{
		if (vt==CRCore::crNodeVisitor::INIT_VISITOR||vt == CRCore::crNodeVisitor::UPDATE_VISITOR)
		{
			init();
		}
		else
		{
			return;
		}
	}
    if (nv.getVisitorType()==CRCore::crNodeVisitor::CULL_VISITOR)
    {
        CRCore::crClusterCullingCallback* ccc = dynamic_cast<CRCore::crClusterCullingCallback*>(getCullCallback());
        if (ccc)
        {
            if (ccc->cull(&nv,0,static_cast<crState *>(0))) return;
        }
    }

    //if (m_terrainTechnique.valid())
    //{
    //    m_terrainTechnique->traverse(nv);
    //}
    //else
    //{
        CRCore::crGroup::traverse(nv);
    //}
}

void crTerrainTile::init()
{
    if (!m_terrainTechnique)
    {        
        if (m_terrain && m_terrain->getTerrainTechniquePrototype())
        {            
            CRCore::ref_ptr<CRCore::crBase> object = m_terrain->getTerrainTechniquePrototype()->clone(CRCore::crCopyOp::DEEP_COPY_ALL);
            setTerrainTechnique(dynamic_cast<crTerrainTechnique*>(object.get()));
        }
        else
        {
            setTerrainTechnique(new crGeometryTechnique);
        }
    }

    if (m_terrainTechnique.valid() && getDirty())
    {
        m_terrainTechnique->init();
        
        setDirty(false);
    }    
}

void crTerrainTile::setTerrainTechnique(crTerrainTechnique* terrainTechnique)
{
    if (m_terrainTechnique == terrainTechnique) return; 

    int dirtyDelta = m_dirty ? -1 : 0;

    if (m_terrainTechnique.valid()) 
    {
        m_terrainTechnique->m_terrainTile = 0;
    }

    m_terrainTechnique = terrainTechnique;
    
    if (m_terrainTechnique.valid()) 
    {
        m_terrainTechnique->m_terrainTile = this;
        ++dirtyDelta;        
    }
    
    if (dirtyDelta>0) setDirty(true);
    else if (dirtyDelta<0) setDirty(false);
}

void crTerrainTile::setDirty(bool dirty)
{
    if (m_dirty==dirty) return;

    m_dirty = dirty;

    if (m_dirty)
    {
        setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()+1);
    }
    else if (getNumChildrenRequiringUpdateTraversal()>0) 
    {
        setNumChildrenRequiringUpdateTraversal(getNumChildrenRequiringUpdateTraversal()-1);
    }
}



void crTerrainTile::setElevationLayer(crLayer* layer)
{
    m_elevationLayer = layer;
}

void crTerrainTile::setColorLayer(unsigned int i, crLayer* layer)
{
    if (m_colorLayers.size() <= i) m_colorLayers.resize(i+1);
    
    m_colorLayers[i] = layer;
}

bool crTerrainTile::computeBound() const
{
	return crGroup::computeBound();
	//m_boundSphere.init();
	//m_bbox.init();
 //   
 //   //if (m_elevationLayer.valid())
 //   //{
 //   //    m_boundSphere.expandBy(m_elevationLayer->getBound(true));
 //   //}
 //   //else
 //   //{
 //   //    for(Layers::const_iterator itr = m_colorLayers.begin();
 //   //        itr != m_colorLayers.end();
 //   //        ++itr)
 //   //    {
 //   //        if (itr->valid()) m_boundSphere.expandBy((*itr)->getBound(false));
 //   //    }
 //   //}
 //   
	////if (!m_boundSphere.valid()) 
	////{
	////	m_bBoundSphere_computed = true;
	////	return false;
	////}
	////m_bbox.expandBy(m_boundSphere);
	//m_bBoundSphere_computed = true;
	//return true;
}
void crTerrainTile::releaseObjects(crState* state)
{
	crGroup::releaseObjects(state);
	if(m_terrainTechnique.valid()) m_terrainTechnique->releaseObjects(state);
}

//void crTerrainTile::setHeightfieldDataID(void *id)
//{
//    m_heightid = id;
//}
//
//void crTerrainTile::releaseOde()
//{
//	if(m_heightid)
//	{
//		dGeomHeightfieldDataDestroy( (dHeightfieldDataID)m_heightid );
//		m_heightid = NULL;
//	}
//	crGroup::releaseOde();
//}

/////////////////////////////////////////////////////////////////////////////////
//
// crWhiteListTileLoadedCallback
//
crWhiteListTileLoadedCallback::crWhiteListTileLoadedCallback()
{
    m_minumumNumberOfLayers = 0;
    m_replaceSwitchLayer = false;
    m_allowAll = false;
}

crWhiteListTileLoadedCallback::~crWhiteListTileLoadedCallback()
{
}

bool crWhiteListTileLoadedCallback::layerAcceptable(const std::string& setname) const
{
    if (m_allowAll) return true;

    if (setname.empty()) return true;

    return m_setWhiteList.count(setname)!=0;
}

bool crWhiteListTileLoadedCallback::readImageLayer(CRTerrain::crImageLayer* imageLayer, const CRIOManager::crReaderWriter::Options* options) const
{
   if (!imageLayer->getImage() && 
        !imageLayer->getFileName().empty())
    {
        if (layerAcceptable(imageLayer->getSetName()))
        {
            CRCore::ref_ptr<CRCore::crImage> image = CRIOManager::readImageFile(imageLayer->getFileName(), options);
            imageLayer->setImage(image.get());
        }
    }
    return imageLayer->getImage()!=0;
}

bool crWhiteListTileLoadedCallback::deferExternalLayerLoading() const
{
    return true;
}

void crWhiteListTileLoadedCallback::loaded(CRTerrain::crTerrainTile* tile, const CRIOManager::crReaderWriter::Options* options) const
{

    // read any external layers
    for(unsigned int i=0; i<tile->getNumColorLayers(); ++i)
    {
        CRTerrain::crLayer* layer = tile->getColorLayer(i);
        CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(layer);
        if (imageLayer)
        {
            readImageLayer(imageLayer, options);
            continue;
        }

        CRTerrain::crSwitchLayer* switchLayer = dynamic_cast<CRTerrain::crSwitchLayer*>(layer);
        if (switchLayer)
        {
            for(unsigned int si=0; si<switchLayer->getNumLayers(); ++si)
            {
                CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(switchLayer->getLayer(si));
                if (imageLayer)
                {
                    if (readImageLayer(imageLayer, options))
                    {                        
                        // replace crSwitchLayer by 
                        if (m_replaceSwitchLayer) tile->setColorLayer(i, imageLayer);
                        else if (switchLayer->getActiveLayer()<0) switchLayer->setActiveLayer(si);

                        continue;
                    }
                }
            }
            continue;
        }

        CRTerrain::crCompositeLayer* compositeLayer = dynamic_cast<CRTerrain::crCompositeLayer*>(layer);
        if (compositeLayer)
        {
            for(unsigned int ci=0; ci<compositeLayer->getNumLayers(); ++ci)
            {
                CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(compositeLayer->getLayer(ci));
                if (imageLayer)
                {
                    readImageLayer(imageLayer, options);
                }
            }
            continue;
        }
    }

    // assign colour layers over missing layers
    CRTerrain::crLayer* validLayer = 0;
    for(unsigned int i=0; i<tile->getNumColorLayers(); ++i)
    {
        CRTerrain::crLayer* layer = tile->getColorLayer(i);
        CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(layer);
        if (imageLayer)
        {
            if (imageLayer->getImage()!=0)
            {
                validLayer = imageLayer;
            }
            continue;
        }

        CRTerrain::crSwitchLayer* switchLayer = dynamic_cast<CRTerrain::crSwitchLayer*>(layer);
        if (switchLayer)
        {
            for(unsigned int si=0; si<switchLayer->getNumLayers(); ++si)
            {
                CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(switchLayer->getLayer(si));
                if (imageLayer && imageLayer->getImage()!=0)
                {
                    validLayer = imageLayer;
                }
            }
            continue;
        }

        CRTerrain::crCompositeLayer* compositeLayer = dynamic_cast<CRTerrain::crCompositeLayer*>(layer);
        if (compositeLayer)
        {
            for(unsigned int ci=0; ci<compositeLayer->getNumLayers(); ++ci)
            {
                CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(switchLayer->getLayer(ci));
                if (imageLayer && imageLayer->getImage()!=0)
                {
                    validLayer = imageLayer;
                }
            }
            continue;
        }
    }

    if (validLayer)
    {
        // fill in any missing layers
        for(unsigned int i=0; i<tile->getNumColorLayers(); ++i)
        {
            CRTerrain::crLayer* layer = tile->getColorLayer(i);
            CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(layer);
            if (imageLayer)
            {
                if (imageLayer->getImage()==0)
                {
                    tile->setColorLayer(i, validLayer);
                    break;
                }
                continue;
            }

            CRTerrain::crSwitchLayer* switchLayer = dynamic_cast<CRTerrain::crSwitchLayer*>(layer);
            if (switchLayer)
            {
                for(unsigned int si=0; si<switchLayer->getNumLayers(); ++si)
                {
                    CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(switchLayer->getLayer(si));
                    if (imageLayer && imageLayer->getImage()==0)
                    {
                        if (m_replaceSwitchLayer) tile->setColorLayer(i, imageLayer);
                        else
                        {
                            switchLayer->setLayer(si, validLayer);
                            if (switchLayer->getActiveLayer()<0) switchLayer->setActiveLayer(si);
                        }
                        break;
                    }
                }
                if (switchLayer->getNumLayers()==0)
                {
                    if (m_replaceSwitchLayer) tile->setColorLayer(i, validLayer);
                    else
                    {
                        switchLayer->setLayer(0, validLayer);
                        switchLayer->setActiveLayer(0);
                    }
                }
            }

            CRTerrain::crCompositeLayer* compositeLayer = dynamic_cast<CRTerrain::crCompositeLayer*>(layer);
            if (compositeLayer)
            {
                for(unsigned int ci=0; ci<compositeLayer->getNumLayers(); ++ci)
                {
                    CRTerrain::crImageLayer* imageLayer = dynamic_cast<CRTerrain::crImageLayer*>(switchLayer->getLayer(ci));
                    if (imageLayer && imageLayer->getImage()==0)
                    {
                        tile->setColorLayer(i, validLayer);
                        break;
                    }
                }
                continue;
            }
        }

        if (m_minumumNumberOfLayers>tile->getNumColorLayers())
        {
            for(unsigned int i=tile->getNumColorLayers(); i<m_minumumNumberOfLayers; ++i)
            {
                tile->setColorLayer(i, validLayer);
            }
        }

    }
}