/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#include <Plugin cre/crException.h>
#include <Plugin cre/crTerrainTile.h>
#include <Plugin cre/crGroup.h>
#include <CRTerrain/crGeometryTechnique.h>
using namespace cre;

void crTerrainTile::write(crDataOutputStream* out)
{
    // Write Terrain's identification.
    out->writeInt(CRETERRAINTILE);
	CRTerrain::crGeometryTechnique *geometryTechnique = dynamic_cast<CRTerrain::crGeometryTechnique *>(getTerrainTechnique());
	if(geometryTechnique)
	{
		geometryTechnique->setShowGrid(false);
	}
    // If the osg class is inherited by any other class we should also write this to file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group)
        ((cre::crGroup*)(group))->write(out);
    else
        throw crException("crTerrainTile::write(): Could not cast this CRTerrain::crTerrainTile to an CRCore::crGroup.");

    out->writeBool(m_dirty);
	out->writeBool(m_requiresNormals);
	out->writeBool(m_treatBoundariesToValidDataAsDefaultValue);

    out->writeInt(getTileID().level);
    out->writeInt(getTileID().x);
    out->writeInt(getTileID().y);

    //out->writeLocator(getLocator());
	CRTerrain::crHeightFieldLayer *hfl = dynamic_cast<CRTerrain::crHeightFieldLayer *>(getElevationLayer());
	out->writeBool(hfl!=NULL);
    if(hfl)
	{
		out->writeShape(hfl->getHeightField());
	}
    //out->writeUInt(getNumColorLayers());
    //for(unsigned int i=0; i<getNumColorLayers(); ++i)
    //{
    //    out->writeLayer(getColorLayer(i));
    //}
    //writeTerrainTechnique(out, getTerrainTechnique());
}

void crTerrainTile::read(crDataInputStream* in)
{
    // Peek on Terrain's identification.
    int id = in->peekInt();
    if (id != CRETERRAINTILE) throw crException("crTerrainTile::read(): Expected Terrain identification.");

    // Read Terrain's identification.
    id = in->readInt();
    // If the osg class is inherited by any other class we should also read this from file.
    CRCore::crGroup*  group = dynamic_cast<CRCore::crGroup*>(this);
    if(group)
        ((cre::crGroup*)(group))->read(in);
    else
        throw crException("crTerrainTile::read(): Could not cast this CRTerrain::crTerrainTile to an CRCore::crGroup.");

	m_dirty = in->readBool();
	m_requiresNormals = in->readBool();
	m_treatBoundariesToValidDataAsDefaultValue = in->readBool();

    int level = in->readInt();
    int x = in->readInt();
    int y = in->readInt();
    setTileID(CRTerrain::crTileID(level,x,y));

    //setLocator(in->readLocator());
    if(in->readBool())
	{
		CRTerrain::crHeightFieldLayer *hfl = new CRTerrain::crHeightFieldLayer;
		hfl->setHeightField(dynamic_cast<CRCore::crHeightField *>(in->readShape()));
		setElevationLayer(hfl);
	}
    //unsigned int numColorLayers = in->readUInt();
    //for(unsigned int i=0; i<numColorLayers; ++i)
    //{
    //    setColorLayer(i, in->readLayer());
    //}    
    //setTerrainTechnique(readTerrainTechnique(in));
    //if (CRTerrain::crTerrainTile::getTileLoadedCallback().valid()) 
    //    CRTerrain::crTerrainTile::getTileLoadedCallback()->loaded(this, in->getOptions());
}