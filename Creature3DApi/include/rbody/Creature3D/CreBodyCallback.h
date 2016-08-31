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
#ifndef __CREBODYCALLBACK_H__
#define __CREBODYCALLBACK_H__

#include <CRCore/crNotify.h>
#include <CRCore/crTimer.h>

#include <rbody/Creature3D/CreBody.h>
#include <rbody/Creature3D/CreCollectTransforms.h>
#include <CRCore/crIntersectVisitor.h>
#include "rbody/export.h"

namespace rbody {

class  CreBodyNode;

class REPLICANTBODY_API CreBodyCallback : public CRCore::crNodeCallback
{
protected:
  CreBodyCallback() : m_body(0), m_geode(0), m_prev_traversal_nr(-1) {};

public:

  CreBodyCallback(CRCore::crObject *geode, CreBodyNode *body);

  virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);

protected:

  CreBodyNode *m_body;

  CRCore::crObject *m_geode;

  int m_prev_traversal_nr;

  CRCore::Timer m_timer; 
  CRCore::Timer_t m_last_tick;
  CRCore::Timer_t m_accumulated_seconds;
  double m_elapsed_time_in_air;
  float m_curElapsed;
};

}

#endif
