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