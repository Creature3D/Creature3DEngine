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
#ifndef CRPHYSICS_CRMATTERGROUP_H
#define CRPHYSICS_CRMATTERGROUP_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/crTransform.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crGeometry.h>
#include <CRPhysics/crJoint.h>
#include <CRPhysics/crOdeWorld.h>
#include <CRPhysics/crMatterObject.h>
#include <ode/ode.h>

#include <map>

namespace CRPhysics{

class CRPHYSICS_EXPORT crMatterGroup : public CRCore::crEventGroup
{
public :
  crMatterGroup();

  crMatterGroup(const crMatterGroup&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

  META_Node(CRPhysics, crMatterGroup);

  //virtual void traverse(CRCore::crNodeVisitor& nv);

  inline const crOdeWorld *getOdeWorld(){ return m_odeWorld.get(); }
  inline void setOdeWorld( crOdeWorld *odeWorld ){ m_odeWorld = odeWorld; }
  inline void setSpaceID( dSpaceID spaceID ){ m_spaceID = spaceID; }
  inline dSpaceID getSpaceID(){ return m_spaceID; }

  virtual void createJoints(dWorldID worldID, dJointGroupID jointGroupID);

  void collide(dGeomID o1,dGeomID o2,bool needsearchnearcb = true);
  void collide(crMatterGroup &mg,dGeomID o1,dGeomID o2,bool needsearchnearcb = true);
  void collide(crMatterObject *mo,dGeomID o1,dGeomID o2,bool needsearchnearcb = true);

  int getLastCollideSpy()const;
  
  class CRPHYSICS_EXPORT NearCallback : public virtual CRCore::Referenced
  {
  public:
	  virtual NearCallback* cloneType() const { return NULL; };
	  virtual const char* libraryName() const { return "CRPhysics"; }
	  virtual const char* className() const { return "NearCallback"; }

	  virtual void nearCallbackImplementation( void * data, dGeomID o1, dGeomID o2 ){};
  };
  
  crMatterObject *getMainMatterObject();

  typedef std::multimap<unsigned int, CRCore::ref_ptr<crJoint> > JointMap;//first: joint msg id
  typedef std::pair <unsigned int, CRCore::ref_ptr<crJoint> > JointMapPair;

  JointMap& getJointMap(){ return m_joints; }

  virtual void releaseObjects(CRCore::crState* state);
  virtual void releaseOde();
  
  static NearCallback *getMatterGroupNearCallback( const std::string &mg1, const std::string &mg2 );
  static void addNearCallback(const std::string& str, NearCallback *nc);
  virtual void setEnableIntersect( bool enableIntersect);

  void setMatterGroupMaterialString(const std::string &materialString){ m_matterGroupMaterialString = materialString; }
  const std::string &getMatterGroupMaterialString(){ return m_matterGroupMaterialString; }

  void setParentSpace(crMatterGroup *parent);
  crMatterGroup *getParentSpace();

  virtual void traverse(CRCore::crNodeVisitor& nv);

protected:
  virtual ~crMatterGroup();

  virtual bool computeBound() const;

  void resetCollideSpy();
  void collideSpy( int n );

  void dealWithCollide( crMatterGroup &mg, bool doCall );
  void dealWithCollide( crMatterObject *mo );
  void dealWithCollide();

  std::string     m_matterGroupMaterialString;//标志该matterGroup的材料属性，用于区别不同材料其表面的不同碰撞处理

  JointMap m_joints;
  mutable dSpaceID  m_spaceID;
  CRCore::ref_ptr<crOdeWorld> m_odeWorld;
  
  static void s_nearCallback (void *, dGeomID, dGeomID);

  CRCore::ref_ptr<NearCallback> m_nearCallback;

  int     m_collideSpy;

  void setNearCallback( NearCallback *nearCallback ){ m_nearCallback = nearCallback; }

  typedef std::map< std::string, CRCore::ref_ptr<NearCallback> > NearCallbackMap;//string 为matterGroup1的材料标志符＋matterGroup2的材料标志符
  static NearCallbackMap m_nearCallbackMap;

  mutable crMatterGroup* m_parentSpace;
};

}

#endif