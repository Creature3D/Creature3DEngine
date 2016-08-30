/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#ifndef CRPHYSICS_CRPHYSICSINITVISITOR_H
#define CRPHYSICS_CRPHYSICSINITVISITOR_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crMatterObject.h>

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crDB.h>
#include <CRCore/crObject.h>
#include <CRCore/crTransform.h>
#include <CRCore/crArray.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crShapeDrawable.h>
#include <CRCore/crBillboard.h>
//#include <CRCore/crDepthSortedObject.h>
#include <CRCore/crSwitch.h>
#include <CRCore/crSequence.h>
#include <CRCore/crLod.h>
#include <CRCore/crEventGroup.h>
//#include <CRTerrain/crTerrainTile.h>

#include <list>
namespace CRPhysics {

class CollectMeshInfoVisitor : public CRCore::crNodeVisitor
{
public:

	CollectMeshInfoVisitor():
		CRCore::crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	virtual ~CollectMeshInfoVisitor(){}

	virtual void apply(CRCore::crBillboard& node){}
	virtual void apply(CRCore::crObject& node)
	{
		if(node.getEnableIntersect())
		{
			CRCore::crGeometry *geometry = NULL;
			for(CRCore::crObject::DrawableList::iterator itr = node.getDrawableList().begin();
				itr != node.getDrawableList().end();
				++itr)
			{
				geometry = (*itr)->asGeometry();
				if(geometry)
					m_geometryList.push_back(geometry);
			}
		}
	}

	static dTriMeshDataID createTriMeshData( CRCore::crNode* inNode );

protected:
	typedef std::list< CRCore::crGeometry* > GeometryList;

	GeometryList &getGeometryList(){ return m_geometryList; }

	static void copyIndexArray(const CRCore::crGeometry *geometry, CRCore::UIntArray &outIndexArray, int offset = 0);

	GeometryList m_geometryList;

};

class CRPHYSICS_EXPORT crPhysicsInitVisitor : public CRCore::crNodeVisitor
{
public:

    crPhysicsInitVisitor(); 
    
	virtual void apply(CRCore::crDB& node);

    virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crBillboard& node);
	virtual void apply(CRCore::crGeometry& node);
    virtual void apply(CRCore::crShapeDrawable& node);

	virtual void apply(CRCore::crGroup& node);
	virtual void apply(CRCore::crMatrixTransform& node);

	//virtual void apply(CRCore::crTransform& node);
	//virtual void apply(CRCore::crOccluderNode& node);
	virtual void apply(CRCore::crSwitch& node);                   
	virtual void apply(CRCore::crSequence& node);
	//virtual void apply(CRCore::crLod& node);
	//virtual void apply(CRCore::crEventGroup& node);

    virtual void apply(crMatterGroup& node);
	virtual void apply(crMatterObject& node);
	//virtual void apply(CRTerrain::crTerrainTile& node);

	void readJointsInfo(crMatterGroup& node);
	void preReadMatterObjectInfo(crMatterObject& node);
	void postReadMatterObjectInfo(crMatterObject& node);
protected:
	virtual ~crPhysicsInitVisitor();

	CRCore::ref_ptr<crOdeWorld> m_currentWorld;
	CRCore::ref_ptr<CRCore::crDB> m_currentDB;
	typedef std::stack<dSpaceID> SpaceStack;
	SpaceStack                  m_dynamicSpaceStack;
	dSpaceID                    m_staticSpaceID;
};

class CRPHYSICS_EXPORT crPhysicsReleaseVisitor : public CRCore::crNodeVisitor
{
public:

	crPhysicsReleaseVisitor(); 
	static crPhysicsReleaseVisitor *getInstance();

	virtual void apply(CRCore::crNode& node);
	virtual void apply(crMatterGroup& node);
	virtual void apply(crMatterObject& node);
 
protected:
	virtual ~crPhysicsReleaseVisitor(); 
};

}

#endif

