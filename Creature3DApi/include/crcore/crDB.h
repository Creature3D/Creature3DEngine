/**********************************************************************
*
*	文件:	   crDB.h
*
*	描述:	   
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_CRDB_H
#define CRCORE_CRDB_H

#include <CRCore/crGroup.h>
#include <CRCore/crPlane.h>
#include <CRCore/crTextureCubeMap.h>

namespace CRCore{

class CR_EXPORT crDB : public crGroup
{
public:
  crDB();
  crDB(const crDB& db, const crCopyOp& copyop = crCopyOp::SHALLOW_COPY);

  META_Node(CRCore,crDB);

  inline void setMaxIterations( int maxiterations ) { m_maxiterations = maxiterations; }
  inline int getMaxIterations() const{ return m_maxiterations; }

  inline void setGravity( const crVector3f &gravity ) { m_gravity = gravity; }
  inline const crVector3f &getGravity()const { return m_gravity; }

  inline void setGroundPlane( const crPlane &plane ) { m_groundPlane = plane; }
  inline const crPlane &getGroundPlane()const { return m_groundPlane; }
  
  //typedef< std::string, ref_ptr<crTextureCubeMap> > EnvironmentMapList;

  inline void setDBEnvironmentMap(crTextureCubeMap *envmap) { m_environmentMap = envmap; }
  inline const crTextureCubeMap *getDBEnvironmentMap()const { return m_environmentMap.get(); }
  inline crTextureCubeMap *getDBEnvironmentMap() { return m_environmentMap.get(); }

protected:
  
  virtual ~crDB();

  //物理参数
  int m_maxiterations;//仿真精度
  crVector3f m_gravity;//重力
  crPlane m_groundPlane;

  ref_ptr<crTextureCubeMap> m_environmentMap;
};

}

#endif