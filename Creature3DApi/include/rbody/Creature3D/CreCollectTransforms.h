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
#ifndef __CRECOLLECTTRANSFORMS_H__
#define __CRECOLLECTTRANSFORMS_H__

#include <CRCore/crNodeVisitor.h>
#include "rbody/export.h"

namespace rbody {

/// Calculates matrix traversing all parents
class REPLICANTBODY_API CreCollectTransforms : public CRCore::crNodeVisitor
{
public:

  /// Constructor
  CreCollectTransforms() : crNodeVisitor(crNodeVisitor::TRAVERSE_PARENTS) { m_m.makeIdentity(); }

  /// Apply
  virtual void apply(CRCore::crMatrixTransform& node) { 
    m_m *= node.getMatrix();
    traverse(node);
  }

  /// Get matrix
  const CRCore::crMatrix& getMatrix() const { return m_m; }


protected:

  /// crMatrix
  CRCore::crMatrix m_m;


};

} // namespace rbody

#endif // __OSGCOLLECTTRANSFORMS_H__
