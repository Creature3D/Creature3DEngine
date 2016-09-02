/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by 吴财华
#include <CRCore/crBoundingBox.h>
#include <CRCore/crBoundingSphere.h>

using namespace CRCore;

void crBoundingBox::expandBy(const crBoundingBox& bb)
{
    if (!bb.valid()) return;

    if(bb.m_min.x()<m_min.x()) m_min.x() = bb.m_min.x();
    if(bb.m_max.x()>m_max.x()) m_max.x() = bb.m_max.x();

    if(bb.m_min.y()<m_min.y()) m_min.y() = bb.m_min.y();
    if(bb.m_max.y()>m_max.y()) m_max.y() = bb.m_max.y();

    if(bb.m_min.z()<m_min.z()) m_min.z() = bb.m_min.z();
    if(bb.m_max.z()>m_max.z()) m_max.z() = bb.m_max.z();
}


void crBoundingBox::expandBy(const crBoundingSphere& sh)
{
    if (!sh.valid()) return;

    if(sh.m_center.x()-sh.m_fRadius<m_min.x()) m_min.x() = sh.m_center.x()-sh.m_fRadius;
    if(sh.m_center.x()+sh.m_fRadius>m_max.x()) m_max.x() = sh.m_center.x()+sh.m_fRadius;

    if(sh.m_center.y()-sh.m_fRadius<m_min.y()) m_min.y() = sh.m_center.y()-sh.m_fRadius;
    if(sh.m_center.y()+sh.m_fRadius>m_max.y()) m_max.y() = sh.m_center.y()+sh.m_fRadius;

    if(sh.m_center.z()-sh.m_fRadius<m_min.z()) m_min.z() = sh.m_center.z()-sh.m_fRadius;
    if(sh.m_center.z()+sh.m_fRadius>m_max.z()) m_max.z() = sh.m_center.z()+sh.m_fRadius;
}
