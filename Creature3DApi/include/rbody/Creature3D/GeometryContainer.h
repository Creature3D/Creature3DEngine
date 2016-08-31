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
#ifndef __RBODY_GEOMETRYCONTAINER_H__
#define __RBODY_GEOMETRYCONTAINER_H__

#include <CRCore/ref_ptr.h>
#include <map>
#include <list>
#include <CRCore/crGeometry.h>
#include "rbody/export.h"
#include <rbody/Creature3D/CreBody.h>

namespace rbody {

class REPLICANTBODY_API CRCore::crGeometry;
    
/// Contains CRCore geometries shared by SubMeshDrawables
class GeometryContainer : public CRCore::Referenced
{
protected:

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

  std::map<std::string, GeomList> m_geom_map;

public:
	static GeometryContainer *getInstance();

  /// Constructor
  GeometryContainer(){};

  /// Set geometry
  void set(const std::string& body, int mesh, int submesh, CRCore::crGeometry *geom);
  CRCore::crGeometry* get(const std::string& body, int mesh, int submesh);
  void erase(CreBodyNode *body, int mesh);
  void erase(const std::string& body);
  void requestRemoveGeom(CreBodyNode *body, int mesh);
  size_t size() { return m_geom_map.size(); };

  virtual void clear();
protected:
  virtual ~GeometryContainer();
  static CRCore::ref_ptr<GeometryContainer> m_instance;
};

} // namespace rbody

#endif // __RBODY_GEOMETRYCONTAINER_H__
