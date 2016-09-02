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
#ifndef CRTERRAIN_CRTERRAINTECHNIQUE_H
#define CRTERRAIN_CRTERRAINTECHNIQUE_H 1

#include <CRCore/crBase.h>

#include <CRUtil/crUpdateVisitor.h>
#include <CRUtil/crCullVisitor.h>

#include <CRTerrain/crExport.h>

namespace CRTerrain {

class crTerrainTile;

class CRTERRAIN_EXPORT crTerrainTechnique : public CRCore::crBase
{
    public:

        crTerrainTechnique();

        /** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
        crTerrainTechnique(const crTerrainTechnique&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
        
        META_Base(CRTerrain, crTerrainTechnique);

        crTerrainTile* getTerrainTile() { return m_terrainTile; }
        const crTerrainTile* getTerrainTile() const { return m_terrainTile; }

        virtual void init();

        //virtual void update(CRUtil::crUpdateVisitor* nv);

        //virtual void cull(CRUtil::crCullVisitor* nv);

        ///** Clean scene graph from any terrain technique specific nodes.*/
        //virtual void cleanSceneGraph();

        ///** Traverse the terrain subgraph.*/
        //virtual void traverse(CRCore::crNodeVisitor& nv);
		virtual void releaseObjects(CRCore::crState* state){}
    protected:
    
       // void setDirty(bool dirty);

        virtual ~crTerrainTechnique();
        
        friend class CRTerrain::crTerrainTile;

        crTerrainTile*    m_terrainTile;

};

}

#endif
