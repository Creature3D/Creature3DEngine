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
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crMatterGroup.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRPhysics/crWeaponNode.h>
#include <CRPhysics/crCharacterMatterObject.h>
//#include <CRPhysics/crCollideCallback.h>
//#include <CRPhysics/crRadarMatterObject.h>
#include <CRPhysics/crPhysicsInitVisitor.h>
#include <CRCore/crMath.h>
#include <CRCore/crBrain.h>
#include <CRCore/crStatistics.h>
using namespace CRPhysics;
using namespace CRCore;
//using namespace CRUtil;
crOdeWorld::WORLDMAP crOdeWorld::m_worldMap;
crOdeWorld::DynamicGeomMap crOdeWorld::m_dynamicGeomMap;
crOdeWorld::StaticGeomMap crOdeWorld::m_staticGeomMap;
crCriticalMutex crOdeWorld::s_worldMapMutex;
crCriticalMutex crOdeWorld::s_dynamicGeomMapMutex;
crCriticalMutex crOdeWorld::s_staticGeomMapMutex;
//CRCore::Timer_t crOdeWorld::m_time = 0L;
bool crOdeWorld::m_characterCollide = false;
//crOdeWorld::ConnectedExcluding crOdeWorld::m_connectedExcluding;

//void OdeMessageFunction (int errnum, const char *msg, va_list ap)
//{
//	CRCore::notify(CRCore::FATAL)<<"OdeMessageFunction : "<<msg<<std::endl;
//}

crOdeWorld::crOdeWorld()
{
	//dSetMessageHandler( &OdeMessageFunction );

	dInitODE2(0);//ode-0.11.1
	dAllocateODEDataForThread(dAllocateMaskAll);

    m_world = dWorldCreate();
	//dVector3 center = {0, 0, 0, 0};
	//dVector3 extents = {10, 0, 10, 0};
	m_worldSpace = dHashSpaceCreate(0);
	//dSpaceSetCleanup(m_worldSpace,0);
    m_staticSpace = dHashSpaceCreate(m_worldSpace);//dQuadTreeSpaceCreate(m_worldSpace, center, extents, 7);
	//dSpaceSetCleanup(m_staticSpace,0);
	//m_dynamicSpace = dQuadTreeSpaceCreate(m_worldSpace, center, extents, 7);
	m_dynamicSpace = dHashSpaceCreate(m_worldSpace);
	//dSpaceSetCleanup(m_dynamicSpace,0);
    //m_space = dHashSpaceCreate (0);
    m_contactgroup = dJointGroupCreate (0);
    dWorldSetGravity (m_world,0,0,-9.81f);
	
	//dWorldSetCFM (m_world,1e-5);
	//dWorldSetAutoDisableFlag (m_world,1);
	//dWorldSetContactMaxCorrectingVel (m_world,0.1);
	//dWorldSetContactSurfaceLayer (m_world,0.001);
 //   dWorldSetAutoDisableAverageSamplesCount( m_world, 1 );

	//dWorldSetCFM (m_world,1e-5);
	//dWorldSetAutoDisableFlag (m_world,1);
	//dWorldSetContactMaxCorrectingVel (m_world,0.1);
	//dWorldSetContactSurfaceLayer (m_world,0.001);
	//dWorldSetAutoDisableAverageSamplesCount( m_world, 10 );
	//dWorldSetLinearDamping(m_world, 0.00001);
	//dWorldSetAngularDamping(m_world, 0.005);
	//dWorldSetMaxAngularSpeed(m_world, 200);
	//dWorldSetContactMaxCorrectingVel (m_world,0.1);
	//dWorldSetContactSurfaceLayer (m_world,0.001);

	//dWorldSetCFM (m_world,0.001);//Typical values are in the range 10e-9 – 1.
	                            //The default is 10e-5 if single precision is being used, or 10e-10 if double precision is being used.
	//dWorldSetERP(m_world,0.1);//range 0.1–0.8. The default is 0.2.
	//dWorldSetAutoDisableFlag (m_world,1);
	//dWorldSetContactMaxCorrectingVel (m_world,10);
	//dWorldSetContactSurfaceLayer (m_world,0.001);

    //m_currentGroundPlane.set(0, 0, 1, 0);
    //m_groundPlaneID = dCreatePlane( m_staticSpace,
    //        m_currentGroundPlane[0],
    //        m_currentGroundPlane[1],
    //        m_currentGroundPlane[2],
    //        m_currentGroundPlane[3] );
    m_groundPlaneID = 0;
	m_maxiterations = 10;
	//m_time = 0L;
}
crOdeWorld::~crOdeWorld()
{
	//clearConnectedExcluding();
    dJointGroupDestroy (m_contactgroup);
    if(m_groundPlaneID) dGeomDestroy(m_groundPlaneID);
	dSpaceDestroy(m_dynamicSpace);
	dSpaceDestroy(m_staticSpace);
	dSpaceDestroy(m_worldSpace);
	dWorldDestroy(m_world);
	
	dCloseODE();
}

void crOdeWorld::odeWorldClear()
{
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_dynamicGeomMapMutex);
		m_dynamicGeomMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_staticGeomMapMutex);
		m_staticGeomMap.clear();
	}
	{
		CRCore::ScopedLock<CRCore::crCriticalMutex> lock(s_worldMapMutex);
		m_worldMap.clear();
	}
}

void crOdeWorld::setGravity( const CRCore::crVector3 &g )
{
    dWorldSetGravity (m_world,g[0], g[1], g[2]);
}

void crOdeWorld::setCFM( double cfm )
{
    dWorldSetCFM( m_world, cfm );
}

void crOdeWorld::setGroundPlane( const CRCore::crPlane& plane )
{
	return;
	//CRCore::ref_ptr<crNode> groundNode;
	//if(m_groundPlaneID)
	//{
	//	groundNode = m_staticGeomMap[m_groundPlaneID];
	//	staticGeoMapErase(m_groundPlaneID);
	//	dGeomDestroy( m_groundPlaneID );
	//}
	//m_groundPlaneID = dCreatePlane (m_staticSpace,
	//								plane[0],
	//								plane[1],
	//								plane[2],
	//								plane[3] );
 //   if( !groundNode.valid() )
 //   {
	//	groundNode = new crNode;
	//	groundNode->setVisiable(false);
	//	groundNode->setRenderable(false);
	//	groundNode->setEnableIntersect(true);
	//	//groundNode->setCollideCallback(crGroundBoxCollideCallback::getInstance());
	//	crBrain::getInstance()->getCurrentActiveDB()->addChild(groundNode.get());
 //   }
	//m_staticGeomMap[m_groundPlaneID] = groundNode.get();
}

void crOdeWorld::collide()
{
	dSpaceCollide2((dGeomID)m_dynamicSpace,(dGeomID)m_staticSpace,this,s_nearCallback);
	dSpaceCollide(m_dynamicSpace,this,s_nearCallback);
}
bool crOdeWorld::isCollideValid(dGeomID o1, dGeomID o2,CRCore::ref_ptr<crNode>&node1,CRCore::ref_ptr<crNode>&node2)
{
	if(!dGeomIsEnabled(o1)||!dGeomIsEnabled(o2)) return false;

	//if(areConnectedExcluding(o1,o2)) 
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"areConnectedExcluding"<< std::endl;
	//	return;
	//}
	//if(areConnectedExcluding(o2,o1)) 
	//{
	//	CRCore::notify(CRCore::ALWAYS)<<"areConnectedExcluding"<< std::endl;
	//	return;
	//}

	crOdeWorld::getDynamicGeomNode(o1,node1);
	crOdeWorld::getDynamicGeomNode(o2,node2);

	if( (dynamic_cast<crBulletMatterObject *>(node1.get()) && dynamic_cast<crBulletMatterObject *>(node2.get())) || 
		(dynamic_cast<crBulletMatterObject *>(node1.get()) && dynamic_cast<crWeaponMatterObject *>(node2.get())) || 
		(dynamic_cast<crWeaponMatterObject *>(node1.get()) && dynamic_cast<crBulletMatterObject *>(node2.get())) ||
		(dynamic_cast<crWeaponMatterObject *>(node1.get()) && dynamic_cast<crWeaponMatterObject *>(node2.get())) )
	{//不处理子弹与子弹相撞
		return false;
	}
	if(!m_characterCollide && dynamic_cast<crCharacterMatterObject *>(node1.get()) && dynamic_cast<crCharacterMatterObject *>(node2.get())) 	
	{//不处理角色之间的碰撞
		return false;
	}
	return true;
}
void crOdeWorld::s_nearCallback( void *data, dGeomID o1, dGeomID o2 )
{
#ifdef _DEBUG
	try
	{
#endif
	ref_ptr<crNode> node1,node2;
    if(!isCollideValid(o1,o2,node1,node2)) return;

	crOdeWorld *This = static_cast<crOdeWorld *>(data);

	if( o1 == This->m_groundPlaneID ||
		o2 == This->m_groundPlaneID )
	{
		return;
	}
	//if((node1&&!node1->getEnableIntersect())||(node2&&!node2->getEnableIntersect()))
	//	return;

	crMatterGroup *mg1 = dynamic_cast<crMatterGroup *>(node1.get());
	crMatterGroup *mg2 = dynamic_cast<crMatterGroup *>(node2.get());

	if(mg1||mg2)
	{//两个crMatterGroup相撞或者是crMatterGroup与crMatterObject相撞或者是crMatterGroup与场景相撞
		if(mg1&&mg2)
		{
			mg1->collide(*mg2,o1,o2);
		}
		else if(mg1)
		{
			if(node2.valid())
			{
                //crMatterObject *matterObject2 = dynamic_cast<crMatterObject *>(node2);
				mg1->collide(dynamic_cast<crMatterObject *>(node2.get()),o1,o2);
			}
			else
				mg1->collide(o1,o2);
		}
		else 
		{
			if(node1.valid())
			    mg2->collide(dynamic_cast<crMatterObject *>(node1.get()),o1,o2);
			else
				mg2->collide(o1,o2);
		}
	}
	else
	{//两个crMatterObject相撞或者是crMatterObject与场景相撞

		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		if(b1) b1 = dBodyIsEnabled(b1)?b1:0;
		if(b2) b2 = dBodyIsEnabled(b2)?b2:0;

	    dContact contact[MAX_CONTACTS];

	    int n = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));
    
	    if(n>0)
		{
			int collideTest = 1;
			if(node1.valid()) 
			{
				node1->doEvent(WCH_COLLIDETEST,MAKECREPARAM(node2.get(),&collideTest));
			    if(collideTest == 0) return;
			}
			if(node2.valid())
			{
				collideTest = 1;
				node2->doEvent(WCH_COLLIDETEST,MAKECREPARAM(node1.get(),&collideTest));
				if(collideTest == 0) return;
			}
			//crMatterObject *mo1 = dynamic_cast<crMatterObject *>(node1);
			//crMatterObject *mo2 = dynamic_cast<crMatterObject *>(node2);

			crCollideCallback* callback;
			crVector3 contactPos(contact[0].geom.pos[0],contact[0].geom.pos[1],contact[0].geom.pos[2]);
			crVector3 contactNormal(contact[0].geom.normal[0], contact[0].geom.normal[1], contact[0].geom.normal[2]);
			if(node1.valid())
			{
				node1->doEvent(WCH_COLLIDE,MAKECREPARAM(node2.get(),&contact[0]));
				callback = node1->getCollideCallback();
				if (callback) (*callback)(node1.get(),node2.get(),o1,o2,contactPos,contactNormal);
			}

			if(node2.valid())
			{
				dGeomID g = contact[0].geom.g1;
				contact[0].geom.g1 = contact[0].geom.g2;
				contact[0].geom.g2 = g;
				contact[0].geom.normal[0] = -contact[0].geom.normal[0];
				contact[1].geom.normal[1] = -contact[1].geom.normal[1];
				contact[2].geom.normal[2] = -contact[2].geom.normal[2];
				node2->doEvent(WCH_COLLIDE,MAKECREPARAM(node1.get(),&contact[0]));
				callback = node2->getCollideCallback();
				if (callback) (*callback)(node2.get(),node1.get(),o2,o1,contactPos,-contactNormal);
			}
			
			if ((b1 || b2) && !(b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)))
			{
				for (int i=0; i<n; i++) 
				{
					//contact[i].surface.mode = /*dContactMu2 |*/ dContactSlip1 | dContactSlip2 | 
					//	/*dContactBounce |dContactSoftERP |*/ dContactSoftCFM |  dContactApprox1;
					//	contact[i].surface.mu = 0.5;
					////contact[i].surface.mu2 = 0.5;
					//contact[i].surface.slip1 = 0.001;
					//contact[i].surface.slip2 = 0.001;
					////contact[i].surface.bounce = 0.00001;
					////contact[i].surface.bounce_vel = 0.00001;
					//contact[i].surface.soft_cfm = 0.0001;
					////contact[i].surface.soft_erp = 0.1;

					contact[i].surface.mode = dContactBounce | dContactSoftCFM;
					contact[i].surface.mu = dInfinity;
					contact[i].surface.mu2 = 0;
					contact[i].surface.bounce = 0.1;
					contact[i].surface.bounce_vel = 0.1;
					contact[i].surface.soft_cfm = 0.001;
					dJointID c = dJointCreateContact(This->getWorld(),This->getContactJointGroup(),&contact[i]);
					dJointAttach (c,b1,b2);
				}
			}
		}
	}
#ifdef _DEBUG
	}
	catch (...)
	{
		CRCore::notify(CRCore::ALWAYS)<<"crOdeWorld::s_nearCallback error "<<std::endl;
	}
#endif
	//insertConnectedExcluding(o1,o2);
}


void crOdeWorld::update(float dt)
{
	//CRCore::Timer_t t1 = CRCore::Timer::instance()->tick();
	//if(m_time == 0L)
	//{
	//	m_time = t1;
	//	dJointGroupEmpty (m_contactgroup);
 //       return;
	//}

	//float dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
	//if(dt<0.01f)
	//	return;
	//m_time = t1;

	//dt = clampTo(dt,0.01f,0.1f);
  // dWorldStep(m_world,dt);
	//dWorldStepFast1(m_world,dt,m_maxiterations);
	dWorldQuickStep(m_world,dt);
	//dWorldStepFast1(m_world,0.01,m_maxiterations);
	//////////////////dWorldStepFast1(m_world,0.02,m_maxiterations);//for car
    dJointGroupEmpty (m_contactgroup);
}

///////////////////////
//
//crPhysicsSystemUpdater
//
/////////////////////////////
class PhysicsToCreVisitor : public CRCore::crNodeVisitor
{
public:

	PhysicsToCreVisitor():
	  crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN) {}

	  virtual ~PhysicsToCreVisitor(){}

	  virtual void apply(crMatterGroup& node)
	  { 
		  if(node.getPhysicsInited() /*&& node.getVisiable()*/ && node.getEnableIntersect()) traverse(node);
	  }
	  virtual void apply(crMatterObject& node)
	  {
		  if(node.getPhysicsInited() /*&& node.getVisiable()*/ && node.getEnableIntersect()) 
		  {
			  node.updateCreMatrix();
			  node.swapPhysicsBuffers();
		  }
	  }
	  //virtual void apply(crRadarMatterObject& node)
	  //{
		 // if(node.getPhysicsInited()) 
		 // {
			//  if(node.getEnableIntersect())
			//      node.updateCreMatrix();
			//  else
			//	  node.unLockSwapBufMutex();
		 // }
	  //}
};

//static PhysicsToCreVisitor s_physicsToCreVisitor;

ref_ptr<crPhysicsSystemUpdater> crPhysicsSystemUpdater::m_instance = NULL;

crPhysicsSystemUpdater* crPhysicsSystemUpdater::getInstance()
{
	if (m_instance==NULL) 
	{
		m_instance = new crPhysicsSystemUpdater;
		crBrain::getInstance()->pushInstance(m_instance.get());
	}
	return m_instance.get();
}
void crPhysicsSystemUpdater::clear()
{
	m_instance = NULL;
}

crPhysicsSystemUpdater::crPhysicsSystemUpdater()
:m_done(true),
m_time(0L)
{
	m_physicsUpdateBarrier = new CRCore::crBarrier(2);
	m_frameBarrier = new CRCore::crBarrier(2);
}

crPhysicsSystemUpdater::~crPhysicsSystemUpdater() 
{
	m_done = true;
	if(m_physicsUpdateBarrier) m_physicsUpdateBarrier->invalidate();
	if(m_frameBarrier) m_frameBarrier->invalidate();
	while (isRunning()) 
	{
		CRCore::crThread::sleep(10);
	}

	if(m_physicsUpdateBarrier)
	{
		delete m_physicsUpdateBarrier; 
		m_physicsUpdateBarrier = NULL;
	}
	if(m_frameBarrier)
	{
		delete m_frameBarrier;
		m_frameBarrier = NULL;
	}
}

void crPhysicsSystemUpdater::done()
{
	m_done = true;
	//m_physicsUpdateBarrier->release();
	//m_frameBarrier->release();
	//m_physicsUpdateBarrier->block(1);
	//m_frameBarrier->block(1);
}

CRCore::crBarrier *crPhysicsSystemUpdater::getPhysicsUpdateBlock()
{
	return m_physicsUpdateBarrier;
}

CRCore::crBarrier *crPhysicsSystemUpdater::getFrameBlock()
{
	return m_frameBarrier;
}

void crPhysicsSystemUpdater::run()
{
#ifdef _DEBUG
	try
	{
#endif
	m_done = false;
	CRCore::Timer_t start_tick,end_tick;
	CRCore::Timer_t t1;
	float dt;
	PhysicsToCreVisitor physicsToCreVisitor;
	ref_ptr<crDB> db;
	ref_ptr<crOdeWorld> odeWorld;
	setSchedulePriority(THREAD_PRIORITY_HIGH);
	while(!m_done)
	{
		physicsUpdateBlock();
		frameBlock();
		if(!m_done)
		{
			db = crBrain::getInstance()->getCurrentActiveDB();
			if(db.valid())
			{
				t1 = CRCore::Timer::instance()->tick();
				if(m_time == 0)
				{
					m_time = t1;
					CRCore::crThread::yieldCurrentThread();
					continue;
				}
				dt = CRCore::Timer::instance()->delta_s( m_time, t1 );
				if(dt>0.05f)
				{
					if(crStatistics::getInstance()->getStat())
					{
						start_tick = CRCore::Timer::instance()->tick();
					}

					m_time = t1;
					odeWorld = crOdeWorld::getOdeWorld(db.get());
					if(odeWorld.valid())
					{
						odeWorld->collide();
						odeWorld->update(dt);
						db->accept(physicsToCreVisitor);//ode->matrixbuf
					}

					if(crStatistics::getInstance()->getStat())
					{
						end_tick = CRCore::Timer::instance()->tick();
						float ms = CRCore::Timer::instance()->delta_m(start_tick,end_tick);
						crStatistics::getInstance()->setPhyUpdTime(ms);
					}
				}
			}
		}
	}
#ifdef _DEBUG
	CRCore::notify(CRCore::ALWAYS)<<"crPhysicsSystemUpdater end"<<std::endl;
	}
	catch (...)
	{
	       CRCore::notify(CRCore::ALWAYS)<<"crPhysicsSystemUpdater::run() error "<<std::endl;
	}
#endif
}