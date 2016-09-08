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
#include <CRTerrain/crTerrainTechnique.h>
#include <CRTerrain/crTerrainTile.h>

using namespace CRTerrain;

crTerrainTechnique::crTerrainTechnique():
    m_terrainTile(0)
{
//    setThreadSafeRefUnref(true);
}

crTerrainTechnique::crTerrainTechnique(const crTerrainTechnique& crTerrainTechnique,const CRCore::crCopyOp& copyop):
    CRCore::crBase(crTerrainTechnique,copyop),
    m_terrainTile(0)
{
}

crTerrainTechnique::~crTerrainTechnique()
{
}

void crTerrainTechnique::init()
{
    CRCore::notify(CRCore::NOTICE)<<className()<<"::initialize(..) not implementated yet"<<std::endl;
}

//void crTerrainTechnique::update(CRUtil::crUpdateVisitor* uv)
//{
//    CRCore::notify(CRCore::NOTICE)<<className()<<"::update(..) not implementated yet"<<std::endl;
//    if (m_terrainTile) m_terrainTile->CRCore::crGroup::traverse(*uv);
//}
//
//void crTerrainTechnique::cull(CRUtil::crCullVisitor* cv)
//{
//    CRCore::notify(CRCore::NOTICE)<<className()<<"::cull(..) not implementated yet"<<std::endl;
//    if (m_terrainTile) m_terrainTile->CRCore::crGroup::traverse(*cv);
//}

//void crTerrainTechnique::cleanSceneGraph()
//{
//    CRCore::notify(CRCore::NOTICE)<<className()<<"::cleanSceneGraph(..) not implementated yet"<<std::endl;
//}

//void crTerrainTechnique::traverse(CRCore::crNodeVisitor& nv)
//{
//    if (!m_terrainTile) return;
//
//    // if app traversal update the frame count.
//    if (nv.getVisitorType()==CRCore::crNodeVisitor::UPDATE_VISITOR)
//    {
//        if (m_terrainTile->getDirty()) m_terrainTile->init();
//
//        CRUtil::crUpdateVisitor* uv = dynamic_cast<CRUtil::crUpdateVisitor*>(&nv);
//        if (uv)
//        {
//            update(uv);
//            return;
//        }        
//        
//    }
//    else if (nv.getVisitorType()==CRCore::crNodeVisitor::CULL_VISITOR)
//    {
//        CRUtil::crCullVisitor* cv = dynamic_cast<CRUtil::crCullVisitor*>(&nv);
//        if (cv)
//        {
//            cull(cv);
//            return;
//        }
//    }
//
//    if (m_terrainTile->getDirty()) m_terrainTile->init();
//
//    // otherwise fallback to the crGroup::traverse()
//    m_terrainTile->CRCore::crGroup::traverse(nv);
//}
