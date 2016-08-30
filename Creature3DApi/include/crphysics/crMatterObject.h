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
#ifndef CRPHYSICS_CRMATTEROBJECT_H
#define CRPHYSICS_CRMATTEROBJECT_H 1

#include <CRPhysics/crExport.h>
#include <CRCore/crMatrixTransform.h>
#include <CRCore/crVector3.h>
#include <CRCore/crMatrix.h>
#include <CRPhysics\crOdeWorld.h>
#include <CRCore/crEventCallbackManager.h>
#include <CRCore/crEventGroup.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <ode/ode.h>

#include <vector>
#include <set>

namespace CRPhysics{

class CRPHYSICS_EXPORT crMatterObject : public CRCore::crMatrixTransform
{
public :
  crMatterObject();

  virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const crMatterObject *>(obj)!=NULL; } \
  virtual const char* className() const { return "crMatterObject"; } \
  virtual const char* libraryName() const { return "CRPhysics"; } \
  virtual void accept(CRCore::crNodeVisitor& nv) { if (nv.validNodeMask(*this)) { nv.pushOntoNodePath(this); nv.apply(*this); nv.popFromNodePath(); } } \

  crMatterObject(const crMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

  void setMass( float mass );
  float getMass(){ return m_mass; }
  float getMass()const{ return m_mass; }

  //inline void setPosition( const CRCore::crVector3f& pos ){ m_matrix.setTrans(pos); }
  void setInitPosition( const CRCore::crVector3f& pos ){ m_initPosition = pos; }
  CRCore::crVector3f& getInitPosition(){ return m_initPosition; }
  const CRCore::crVector3f& getInitPosition()const{ return m_initPosition; }

  void setBarycenter( const CRCore::crVector3f& barycenter ){ m_barycenter = barycenter; }
  CRCore::crVector3f& getBarycenter(){ return m_barycenter; }
  const CRCore::crVector3f& getBarycenter()const{ return m_barycenter; }

  void setInertiaMass( const CRCore::crMatrixf& mat ){ m_inertiaMass = mat; }
  CRCore::crMatrixf& getInertiaMass(){ return m_inertiaMass; }
  const CRCore::crMatrixf& getInertiaMass()const{ return m_inertiaMass; }

  void setGeomID(dGeomID geomID){ m_geomID = geomID; }
  dGeomID getGeomID()const{ return m_geomID; }

  void setBodyID(dBodyID bodyID){ m_bodyID = bodyID; }
  dBodyID getBodyID()const{ return m_bodyID; }

  void create(dWorldID worldID, dSpaceID spaceID);
  
  virtual void updateCreMatrix();
  virtual void updateOdeMatrix();

  virtual void releaseObjects(CRCore::crState* state);
  virtual void releaseOde();
  
  virtual CRCore::crVector3f getLengths();

  virtual double checkUpVector();

  virtual CRCore::crVector3f getDirection() const;
  virtual CRCore::crVector3f getDirectionX() const;

  //void setCollideCallback( CRCore::crCollideCallback *collidecbk );
  //inline CRCore::crCollideCallback* getCollideCallback() { return m_collideCallback.get(); }

  CRCore::crMatrix getMatterObjectRotation() const;//外部调用必须使用CRCore::crMatrix mat = 
  //CRCore::crVector3f getMatterObjectPosition() const;//外部调用必须使用CRCore::crVector3f pos = 

  enum MatrixDummyType
  {
	  MD_None = 0x0,
	  MD_RotMatrix = 0x1,
	  MD_TransMatrix = 0x2,
	  MD_FullMatrix = MD_RotMatrix | MD_TransMatrix,
	  MD_LoadMatrix = 0x4,
  };
  void setMatterObjectMatrix(CRCore::crMatrix &matrix,unsigned char matrixtype);
  void getMatterObjectMatrix(CRCore::crMatrix &matrix,unsigned char& matrixtype);

  virtual void setEnableIntersect( bool enableIntersect);

  enum BodyPhysicsType
  {
	  RayDetect,
	  RealPhysics
  };
  inline BodyPhysicsType getCurrentBodyPhysicsType(){ return m_currentBodyPhysicsType; }

  //virtual bool isInAir(){ return false; };
  void setLinearVel(const CRCore::crVector3f& vel, float interval);
  void addForce(const CRCore::crVector3f& force, float interval);
  void enablePhysics(float interval);
  void resetPhysicsSwap();

  //class Callback : public CRCore::crEventCallback
  //{
  //public:
	 // Callback(){}
	 // virtual crEventCallback* cloneType() const = 0;
	 // virtual crEventCallback* clone() const = 0;
	 // virtual bool isSameKindAs(const crEventCallback* obj) const { return true; }
	 // virtual const char* libraryName() const = 0;
	 // virtual const char* className() const = 0;
	 // virtual const char* _name() const = 0;

	 // Callback(const Callback& callback):CRCore::crEventCallback(callback){}

	 // virtual void operator()(crMatterObject &matterObject, _crInt64 param = 0){};
  //protected:
	 // virtual ~Callback(){}
  //};

  //virtual void doEvent(_crInt64 kbmsg, _crInt64 param = 0);

  inline virtual void initPhysicsBuffers() { m_matrix_buf = m_matrix; }
  inline virtual void swapPhysicsBuffers() { m_matrix = m_matrix_buf; dirtyBound(); }

  virtual void traverse(CRCore::crNodeVisitor& nv);

  void setGravityMode(int mode);

  //bool hasVolumeEvent();
  //void setCanDisablePhysics();
  //inline double getCurrentFrameTime(){ return m_previousFrameTime; }
  inline float getCurrentFrameInterval(){ return m_frameInterval; }
  void setStaticMatterObject(bool s);
  bool isStaticMatterObject() const;

  virtual CRCore::crVector3 getPhysicsCenter(){ return getBound().center(); }
protected:
  virtual ~crMatterObject();
  virtual bool computeBound() const;
  virtual void _create(dWorldID worldID, dSpaceID spaceID)=0; 
  bool physicsSwap();

  float m_mass;
  float m_density;
  CRCore::crVector3f m_barycenter;
  CRCore::crMatrixf m_inertiaMass;

  CRCore::crVector3f m_initPosition;

  mutable dGeomID   m_geomID;
  mutable dBodyID   m_bodyID;

  CRCore::crMatrix m_matrix_buf;//physics buf
  CRCore::crMatrix m_matrixDummy;//用于setMatterObjectMatrix
  unsigned char m_matrixDummyType;
  CRCore::crMutex m_matrixDummyMutex;

  int m_gravityMode;
  //CRCore::ref_ptr<CRCore::crCollideCallback>  m_collideCallback;

  ///////physics
  float m_physicsSwapInterval;
  BodyPhysicsType m_currentBodyPhysicsType;
  //double m_previousFrameTime;
  float m_frameInterval;

  CRCore::crVector3 m_linearVel;
  CRCore::crVector3 m_force;
  //bool m_needEnablePhysics;
  bool m_bStaticMatterObject;
};

class CRPHYSICS_EXPORT crBoxMatterObject : public crMatterObject
{
public:
  crBoxMatterObject(){};
  crBoxMatterObject(const crBoxMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

  META_Node(CRPhysics, crBoxMatterObject);

protected:
  virtual ~crBoxMatterObject(){}
  virtual void _create(dWorldID worldID, dSpaceID spaceID);

};

class CRPHYSICS_EXPORT crSphereMatterObject : public crMatterObject
{
public:
	crSphereMatterObject(){}
	crSphereMatterObject(const crSphereMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crSphereMatterObject);

protected:
	virtual ~crSphereMatterObject(){}
	virtual void _create(dWorldID worldID, dSpaceID spaceID);

};

class CRPHYSICS_EXPORT crCCylinderMatterObject : public crMatterObject
{
public:
	crCCylinderMatterObject(){}
	crCCylinderMatterObject(const crCCylinderMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCCylinderMatterObject);
    
protected:
	virtual ~crCCylinderMatterObject(){}
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
};

class CRPHYSICS_EXPORT crCylinderMatterObject : public crMatterObject
{
public:
	crCylinderMatterObject(){}
	crCylinderMatterObject(const crCylinderMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crCylinderMatterObject);

protected:
	virtual ~crCylinderMatterObject(){}
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
};

class CRPHYSICS_EXPORT crMeshMatterObject : public crMatterObject
{
public:
	crMeshMatterObject():m_triData(NULL){}
	crMeshMatterObject(const crMeshMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

	META_Node(CRPhysics, crMeshMatterObject);

    void triMeshMatrixUpdata();

	virtual void releaseOde();

protected:
	virtual ~crMeshMatterObject(){ }
	virtual void _create(dWorldID worldID, dSpaceID spaceID);
    
    mutable dTriMeshDataID       m_triData;
};

//class CRPHYSICS_EXPORT crRayMatterObject : public crMatterObject
//{
//public:
//	crRayMatterObject();
//	crRayMatterObject(const crRayMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
//
//	META_Node(CRPhysics, crRayMatterObject);
//    
//	void setHostMatterObject( crMatterObject *matterObject );
//	virtual double checkUpVector();
//
//	void setRayLength( float length );
//	float getRayLength();
//
//	void setOrigin( const CRCore::crVector3f &origin );
//    const CRCore::crVector3f &getOrigin();
//
//	void setDirection( const CRCore::crVector3f &direction );
//	virtual CRCore::crVector3f getDirection();
//
//	void updateDirection( const CRCore::crVector3f &dir );
//    void updateDirection( const CRCore::crMatrixf &mat );
//	void updateOrigin();
//
//	typedef std::vector<CRCore::crVector3f> RadayAngleList;
//    
//	void updateRay();
//	
//	CRCore::crVector3f getHostDirection();
//
//	const RadayAngleList &getRadayAngleList()const{ return m_angleList; }
//	int getRadayAngleListSize(){ return m_angleList.size(); }
//protected:
//	virtual ~crRayMatterObject(){}
//	virtual void _create(dWorldID worldID, dSpaceID spaceID);
//    
//	float m_length;
//	CRCore::crVector3f m_origin;
//	CRCore::crVector3f m_direction;
//
//	int    m_currentAngleIndex;
//	RadayAngleList m_angleList;
//
//	crMatterObject *m_hostMatterObject;
//
//	CRCore::crMatrixd m_hostMatrix;
//    CRCore::crVector3f m_hostep;
//	CRCore::crVector3f m_hostcenter;
//
//    CRCore::crVector3f m_offset;//雷达坐标与物体中心点坐标的偏移
//};
//
//class CRPHYSICS_EXPORT crTransformMatterObject : public crMatterObject
//{
//public:
//	crTransformMatterObject(){}
//	crTransformMatterObject(const crTransformMatterObject&,const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);
//
//	META_Node(CRPhysics, crTransformMatterObject);
//
//
//	virtual void releaseOde();
//
//protected:
//	virtual ~crTransformMatterObject(){ }
//	virtual void _create(dWorldID worldID, dSpaceID spaceID);
//
//
//};
}

#endif