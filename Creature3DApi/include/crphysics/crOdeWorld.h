/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#ifndef CRPHYSICS_ODEWORLD_H
#define CRPHYSICS_ODEWORLD_H

#include <CRPhysics\crExport.h>

#include <vector>
#include <map>
#include <ode/ode.h>
#include <CRCore/crVector3.h>
//#include <CRCore/crPlane.h>
#include <CRCore/Referenced.h>
#include <CRCore/crDB.h>
#include <CRCore/crTimer.h>
#include <CRCore/thread/crThread.h>
#include <CRCore/thread/crBarrier.h>
#define MAX_CONTACTS 64

namespace CRPhysics
{
	class CRPHYSICS_EXPORT crOdeWorld : public CRCore::Referenced
	{
     public:
        crOdeWorld();
        
		void setGravity( const CRCore::crVector3 & );
        void setCFM( double cfm );
        void setGroundPlane( const CRCore::crPlane& );
		void collide();
        void update(float dt);//maxiterations值越大仿真越精细可以大于（20＋）

        dWorldID getWorld() { return m_world; }

        dSpaceID getWorldSpace() { return m_worldSpace; }
		dSpaceID getStaticSpace() { return m_staticSpace; }
		dSpaceID getDynamicSpace() { return m_dynamicSpace; }

		dJointGroupID getContactJointGroup() { return m_contactgroup; }
		
		dWorldID getWorld() const { return m_world; }
		dSpaceID getWorldSpace() const { return m_worldSpace; }
		dSpaceID getStaticSpace() const { return m_staticSpace; }
		dSpaceID getDynamicSpace() const { return m_dynamicSpace; }

		dJointGroupID getContactJointGroup() const { return m_contactgroup; }

		static inline void creatureToOdeMat( const CRCore::crMatrix &R, dMatrix3 &r )
        {
            r[ 0] = R(0,0);
            r[ 1] = R(1,0);
            r[ 2] = R(2,0);
            r[ 3] = 0.0;
            r[ 4] = R(0,1);
            r[ 5] = R(1,1);
            r[ 6] = R(2,1); 
            r[ 7] = 0.0;
            r[ 8] = R(0,2);
            r[ 9] = R(1,2);
            r[10] = R(2,2);
            r[11] = 0.0;
        }

		int getMaxiterations() { return m_maxiterations; }
		void setMaxiterations( int maxiterations ) { m_maxiterations = maxiterations; }

		typedef std::map< CRCore::ref_ptr<CRCore::crDB>,CRCore::ref_ptr<crOdeWorld> > WORLDMAP;

		static crOdeWorld *getOdeWorld(CRCore::crDB *db)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_worldMapMutex);
			WORLDMAP::iterator itr = m_worldMap.find(db);
			if(itr!=m_worldMap.end())
				return itr->second.get();
			else 
				return NULL;
		}
		static void insertOdeWorld(CRCore::crDB *db,crOdeWorld *odeWorld)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_worldMapMutex);
			m_worldMap[db] = odeWorld;
		}
		//static crOdeWorld::WORLDMAP& getWorldMap(){ return m_worldMap; }

		typedef std::map< dGeomID,CRCore::ref_ptr<CRCore::crNode> > DynamicGeomMap;
		typedef std::map< dGeomID,CRCore::ref_ptr<CRCore::crNode> > StaticGeomMap;
		static int getDynamicGemoCount()
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_dynamicGeomMapMutex);
			return m_dynamicGeomMap.size();
		}
		static int getStaticGeomCount()
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_staticGeomMapMutex);
			return m_staticGeomMap.size();
		}
		static void getDynamicGeomNode(dGeomID geomID,CRCore::ref_ptr<CRCore::crNode> &outnode)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_dynamicGeomMapMutex);
			DynamicGeomMap::iterator itr = m_dynamicGeomMap.find(geomID);
			if(itr!=m_dynamicGeomMap.end())
				outnode = itr->second;
		}

		//static crOdeWorld::DynamicGeomMap& getDynamicGeomMap(){ return m_dynamicGeomMap; }
		static void dynamicGeoMapInsert( dGeomID id,CRCore::crNode *node )
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_dynamicGeomMapMutex);
			m_dynamicGeomMap[id] = node;
		}
		static void dynamicGeoMapErase( dGeomID id )
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_dynamicGeomMapMutex);
			DynamicGeomMap::iterator itr = m_dynamicGeomMap.find(id);
			if(itr != m_dynamicGeomMap.end()) 
				m_dynamicGeomMap.erase(itr); 
		}

		static void getStaticGeomNode(dGeomID geomID,CRCore::ref_ptr<CRCore::crNode> &outnode)
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_staticGeomMapMutex);
			StaticGeomMap::iterator itr = m_staticGeomMap.find(geomID);
			if(itr!=m_staticGeomMap.end())
				outnode = itr->second;
		}

		//static crOdeWorld::StaticGeomMap& getStaticGeomMap(){ return m_staticGeomMap; }
		static void staticGeoMapInsert( dGeomID id,CRCore::crNode *node )
		{
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_staticGeomMapMutex);
			m_staticGeomMap[id] = node;
		}
		static void staticGeoMapErase( dGeomID id )
		{ 
			CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_staticGeomMapMutex);
			StaticGeomMap::iterator itr = m_staticGeomMap.find(id); 
			if(itr != m_staticGeomMap.end()) 
				m_staticGeomMap.erase(itr); 
		}
        
		//typedef std::multimap<dGeomID,dGeomID> ConnectedExcluding;
		//inline static bool areConnectedExcluding(dGeomID o1, dGeomID o2)
		//{
		//	ConnectedExcluding::iterator itr = m_connectedExcluding.find(o1);
		//	if(itr != m_connectedExcluding.end())
		//	{
		//		for( ConnectedExcluding::iterator itr2 = itr;
		//			 itr2->first == o1;
		//			 ++itr2 )
		//		{
  //                  if(itr2->second == o2) return true;
		//		}
		//	}
  //          
		//	return false;
		//}
		//inline static void insertConnectedExcluding(dGeomID o1, dGeomID o2)
		//{
		//	m_connectedExcluding.insert(std::make_pair(o1,o2));
		//}
		//inline static void clearConnectedExcluding() { m_connectedExcluding.clear(); }
		static void odeWorldClear();
		static void setCharacterCollide(bool enable) { m_characterCollide = enable; }
		static bool getCharacterCollide() { return m_characterCollide; }
		static bool isCollideValid(dGeomID, dGeomID,CRCore::ref_ptr<CRCore::crNode>&outnode1,CRCore::ref_ptr<CRCore::crNode>&outnode2);
    private:
		virtual ~crOdeWorld();
		static void s_nearCallback (void *, dGeomID, dGeomID);

        dWorldID m_world;
        dSpaceID m_worldSpace;
        dSpaceID m_staticSpace;
		dSpaceID m_dynamicSpace;
        dJointGroupID m_contactgroup;
        dGeomID m_groundPlaneID;
        //CRCore::crPlane m_currentGroundPlane;
        
		static WORLDMAP m_worldMap;
		static DynamicGeomMap m_dynamicGeomMap;
		static StaticGeomMap m_staticGeomMap;
		static CRCore::crCriticalMutex s_worldMapMutex;
		static CRCore::crCriticalMutex s_dynamicGeomMapMutex;
		static CRCore::crCriticalMutex s_staticGeomMapMutex;
        //static ConnectedExcluding m_connectedExcluding;
		int m_maxiterations;

	   // static CRCore::Timer_t m_time;
		static bool m_characterCollide;
	};

	class CRPHYSICS_EXPORT crPhysicsSystemUpdater: public CRCore::crThread, public CRCore::Referenced
	{
	public:
		crPhysicsSystemUpdater();
		static crPhysicsSystemUpdater *getInstance();

		virtual void clear();

		virtual void run();

		inline void physicsUpdateBlock() { if(m_physicsUpdateBarrier) m_physicsUpdateBarrier->block(); }
		inline void frameBlock() { if(m_frameBarrier) m_frameBarrier->block(); }
		CRCore::crBarrier *getPhysicsUpdateBlock();
		CRCore::crBarrier *getFrameBlock();
		void done();
	protected:
		virtual ~crPhysicsSystemUpdater();
		static CRCore::ref_ptr<crPhysicsSystemUpdater> m_instance;
	private:
		CRCore::crBarrier *m_physicsUpdateBarrier;
		CRCore::crBarrier *m_frameBarrier;
		bool m_done;
		CRCore::Timer_t m_time;
	};	
}
#endif
