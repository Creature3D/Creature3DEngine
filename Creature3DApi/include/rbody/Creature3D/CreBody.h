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
#ifndef __CREBODY_H__
#define __CREBODY_H__

#include <rbody/Body.h>

#include <CRCore/crObject.h>
#include <CRCore/crBoundingSphere.h>
#include <CRCore/crBoundingBox.h>
#include <CRCore/crMatrixTransform.h>
#include <rbody/Creature3D/SubMeshDrawable.h>
//#include <CRPhysics/crCharacterMatterObject.h>
//#include <CRPhysics/crWeaponNode.h>
#include <CRCore/crStreamBuf.h>
#include "rbody/export.h"
#include "rbody/ActionRequest.h"
#include <map>
#include <list>
#ifdef WIN32
  #pragma warning (disable: 4355)
#endif

namespace rbody {

class CRCore::crObject;
//class CreBodyCallback;
class CRCore::crNode;
//class GeometryContainer;

class REPLICANTBODY_API CreBody : public Body 
{
    public:
        CreBody(/*CRPhysics::crCharacterMatterObject *bodyNode*/);

        CreBody(/*CRPhysics::crCharacterMatterObject *bodyNode, */const std::string& core_body_name,const std::string& fileName,const Body::MeshIDSet &meshIDSet);

        virtual RequestResult executeAction(
                ActionRequest *action, bool force = false );
        virtual RequestResult stopAction(const std::string& action);

        bool update( float elapsed_seconds );
        virtual ~CreBody();

    protected:
        void initInternalActions(const std::string& core_body_name);

        //void updateMove( double time );

  //  private:
		//CRCore::ref_ptr<CRPhysics::crCharacterMatterObject> m_bodyObject;

};

class REPLICANTBODY_API CreBodyNode : public CRCore::crGroup
{
//friend class GeometryContainer;
public:
  CreBodyNode(bool enableTangentSpace = false, bool initiate = true );

  void initiate( CreBody* body, bool enableTangentSpace ) {
      m_body = body;
      setup( enableTangentSpace );
  }

  void setup(bool enableTangentSpace,CRCore::crGroup *copyNode = NULL/*,const CRCore::crCopyOp *copyop = NULL*/);

  void initDrawables(CRCore::crGroup *copyNode = NULL/*,const CRCore::crCopyOp *copyop = NULL*/);

  virtual ~CreBodyNode();
 
  virtual void releaseObjects(CRCore::crState* state);

  //virtual void updateCharacterLod(float sqrDistance);
  //virtual void setEnableIntersect( bool enableIntersect);
public:

  /// Specifies the mode of update
  enum UpdateMode {
    UPDATE_NONE = 0, /**< means no update */
    UPDATE_ANIMATION = 1, /**< means update animation */
    UPDATE_ANIMATION_TIME = 2, /**< means update time passed in animation */
    UPDATE_CONTACT_TRANSLATION = 4, /**< means update translation according to animation (contact bones) */
    UPDATE_SPEED_TRANSLATION = 8, /**< means update translation according to set animation speed (auto turned on when body not in FOV and contact on)*/
    UPDATE_GROUND_FOLLOWING = 16, /**< means update ground following */
    UPDATE_GROUND_FOLLOWING_SIMPLE = 32, /**< means update ground following using only one ray (less calculations - auto turned on when body not in FOV and ground follwoing on) */
    UPDATE_FOV_CULLING = 64, /**< means animation => animation_time, contact => speed, ground => ground_simple when body not in FOV */
    UPDATE_DEFAULT = UPDATE_ANIMATION /*| UPDATE_SPEED_TRANSLATION | | UPDATE_CONTACT_TRANSLATION | */
    /*UPDATE_GROUND_FOLLOWING | UPDATE_FOV_CULLING *//**< means update with default values */
  };

  META_Node(rbody, CreBodyNode); 
  //virtual CRCore::crBase* cloneType() const { return new CreBodyNode(); }
  //virtual CRCore::crBase* clone(const CRCore::crCopyOp& copyop) const { return const_cast<CreBodyNode *>(this); }//不允许克隆
  //virtual bool isSameKindAs(const CRCore::crBase* obj) const { return dynamic_cast<const CreBodyNode *>(obj)!=NULL; }
  //virtual const char* libraryName() const { return "rbody"; }
  //virtual const char* className() const { return "CreBodyNode"; }

  CreBodyNode(const CreBodyNode& body, const CRCore::crCopyOp& copyop=CRCore::crCopyOp::SHALLOW_COPY);

  CreBodyNode(bool enableTangentSpace);

  CreBodyNode(const std::string& name, const std::string& fileName, const std::string& id, bool enableTangentSpace);

  void setUpdateMode(unsigned int update_mode) { m_internal_update_mode = m_update_mode = (UpdateMode)update_mode; }

  UpdateMode getUpdateMode() const { return m_update_mode; }

  //void setRotation(float inc);

  void setInternalUpdateMode(unsigned int internal_update_mode) { m_internal_update_mode = (UpdateMode)internal_update_mode; }

  UpdateMode getInternalUpdateMode() const { return m_internal_update_mode; }

  //static GeometryContainer* getGeomContainer();
  float getScale();
  void setScale(float factor);
  CRCore::crMatrixTransform *getBodyTransform();

  Body* getBody() { return m_body.get(); }

  void setBody( CreBody* body ) { m_body = body; };

  virtual void traverse(CRCore::crNodeVisitor& nv);

  const CRCore::crMatrix& getBodyScaleMatrix()const { return m_body_scale->getMatrix(); }

  //virtual CollideBody getCollideBody(const CRCore::crVector3f& collidePos);
  
 // virtual void dead();
 // virtual void renascence();
  //float getLodLevel() const{ return m_lodLevel; }
  bool getEnableTangentSpace() const { return m_enableTangentSpace; }
  bool getNeedTangentSpace() const { return m_enableTangentSpace/* && !m_noNeedTangentSpace*/; }

  void getBoneMatrix(const std::string &boneName, CRCore::crMatrix &mat);
  void rotateBoneMatrix(const std::string &boneName, const CRCore::crQuat &rot);

  //enum DetailCollide
  //{
	 // Collide_None,
	 // Collide_Head,
	 // Collide_Body,
	 // Collide_LArms,
	 // Collide_RArms,
	 // Collide_LLegs,
	 // Collide_RLegs,
	 // Collide_LFoots,
	 // Collide_RFoots
  //};
  //DetailCollide getDetailCollide(const CRCore::crVector3f& collidePos, const CRCore::crMatrix &botmatrix);
  //std::string getCollideBoneName(const CRCore::crVector3f& collidePos, const CRCore::crMatrix &botmatrix);
  //typedef std::map<std::string,DetailCollide> DetailCollideParseMap;
  //void insertDetailCollideParseMap(const std::string& boneName, const std::string &detailCollide);
  //DetailCollide parseDetailCollide(const std::string& boneName);

  int getCurrentMaterialSize();
  void addMaterial(const std::string &fileName);
  int addMaterialSet(MaterialSet *materialSet);

  void deleteMesh(const std::string &meshName);
  void attachMesh(const std::string &meshName);
  void addMesh(Mesh *mesh/*,int materialsetid*/);
  void checkAndSetBodyMeshSet(const Body::MeshIDSet &meshIDSet);

  void addAnimation(AnimationBase *anim);

  std::string getCurrentPartMeshName(int partid);
  bool findMesh(const std::string &meshName);
  std::string getMeshMaterialFileName(int meshid);

  CRCore::crDrawable *getSubMeshDrawable(const std::string &meshName);

  inline void doUpdate(float dt) 
  {
	  if(m_animPlaySpeed == 0.0f || !m_body.valid())
	  {
		  m_timerecord = 0.0f;
		  return;
	  }
	  m_timerecord += dt;
	  if(m_needUpdate && m_timerecord>0.05f)
	  {
#if _DEBUG
		  try
		  {
#endif
			getBody()->setLodPoly(m_lodLevel);
			//CRCore::Timer_t curr_tick = m_timer.tick();
			//float elapsed = m_timer.delta_s(m_last_tick, curr_tick);
			if(m_timerecord>2.0f) m_timerecord = 2.0f;
			float t = m_timerecord * m_animPlaySpeed;
			//CRCore::notify(CRCore::ALWAYS)<<"doUpdate t="<<t<<std::endl;
			m_timerecord = 0.0f;
			//t = CRCore::clampTo(t,-0.05f,0.05f);
			bool actstop = getBody()->update(t);
			if(actstop)
			{
				m_needUpdate = false;
				//m_swapVaild = false;
				//update();
				m_swapVaild = false;
				m_animPlaySpeed = 0.0f;
				return;
			}
			//m_last_tick = curr_tick;
#if _DEBUG
		  }
		  catch (...)
		  {
			  CRCore::notify(CRCore::ALWAYS)<<"getBody()->update error "<<std::endl;
			  //sprintf(gDebugInfo->buf(),"getBody()->update error\n\0");
			  //gDebugInfo->debugInfo(CRCore::ALWAYS);
		  }
#endif
		  //if(m_needUpdate)
		  //{
			  update();
			  m_swapVaild = m_needUpdate;
			  m_needUpdate = false;
		  //}
	  }
	  //else if(m_timerecord>1.0f)
	  //{
		 // float t = m_timerecord * m_animPlaySpeed;
		 // //CRCore::notify(CRCore::ALWAYS)<<"doUpdate t="<<t<<std::endl;
		 // m_timerecord = 0.0f;
		 // //t = CRCore::clampTo(t,-0.05f,0.05f);
		 // bool actstop = getBody()->update(t);
		 // if(actstop)
		 // {
			//  m_needUpdate = false;
			//  //m_swapVaild = false;
			//  //update();
			//  m_swapVaild = false;
			//  m_animPlaySpeed = 0.0f;
		 // }
		 // update();
	  //}
  }
  inline void needUpdate() { m_needUpdate = true; }

  void parseMeshMaterial(SubMeshDrawable *subMeshDrawable);
  void setFileName(const std::string &fileName);
  const std::string &getFileName() const;

  void setAnimPlaySpeed(float speed);
  float getAnimPlaySpeed() const;
  void setLodLevel(float lod);
protected:  
	void update(/*int fn*/);
	//virtual void _create(dWorldID worldID, dSpaceID spaceID);
    //virtual void updateWeaponMatrix();
	virtual bool computeBound() const;
	void requestRemoveGeom(CRCore::crGeometry *geom);
	void deleteAnimation(AnimationBase *anim);
private:
	std::string m_fileName;
  CRCore::crMatrixTransform *m_body_scale;

  //CRCore::ref_ptr<CRCore::crObject> m_body_geode;
  CRCore::crObject::DrawableList m_subMeshDrawableList;
  CRCore::crMutex m_subMeshDrawableListMutex;

  UpdateMode m_update_mode;

  UpdateMode m_internal_update_mode;

  mutable CRCore::ref_ptr<CreBody> m_body;

  bool m_enableTangentSpace;
  typedef std::vector< CRCore::ref_ptr<CRCore::crDrawable> > DrawableVec;
  mutable DrawableVec m_requestRemoveDrawableVec;
  mutable DrawableVec m_releaseDrawableVec;
  mutable DrawableVec m_safeReleaseVec;
  mutable bool m_requestInitDrawables;

  bool m_needUpdate;
  bool m_swapVaild;
 // CRCore::Timer m_timer; 
  //CRCore::Timer_t m_last_tick;
  float m_animPlaySpeed;//动作播放速度
  float m_timerecord;
  //DetailCollideParseMap m_detailCollideParseMap;
  //bool m_noNeedTangentSpace;
  float m_lodLevel;

  //mutable dGeomID   m_characterGeomID;

	class SubMeshGeom
	{
	public:
		SubMeshGeom(int mesh, int submesh, CRCore::crGeometry *geom) : m_mesh(mesh), m_submesh(submesh), m_geom(geom) {};
		int m_mesh;
		int m_submesh;
		CRCore::ref_ptr<CRCore::crGeometry> m_geom;
		virtual ~SubMeshGeom() {};
	};

	typedef std::list<SubMeshGeom> GeomList;
	GeomList m_geomList;
	void setGeometry(int mesh, int submesh, CRCore::crGeometry *geom);
	CRCore::crGeometry* getGeometry(int mesh, int submesh);
	void requestRemoveGeom(int mesh);
	void eraseGeometry(int mesh);
};

} // namespace rbody

#endif
