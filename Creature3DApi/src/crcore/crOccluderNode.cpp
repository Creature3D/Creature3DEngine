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
//Modified by Wucaihua

#include <CRCore/crOccluderNode.h>

using namespace CRCore;

crOccluderNode::crOccluderNode()
{
	//setNumChildrenWithOccluderNodes(1);
}

crOccluderNode::crOccluderNode(const crOccluderNode& node,const crCopyOp& copyop):
    crGroup(node,copyop),
    m_occluder(dynamic_cast<crConvexPlanarOccluder*>(copyop(node.m_occluder.get())))
{
}

bool crOccluderNode::computeBound() const
{
    bool result = crGroup::computeBound();
    
    if (getOccluder())
    {
        m_bbox.init();
        const crConvexPlanarPolygon::VertexList& vertexList = getOccluder()->getOccluder().getVertexList();
        for(crConvexPlanarPolygon::VertexList::const_iterator itr=vertexList.begin();
            itr!=vertexList.end();
            ++itr)
        {
            m_bbox.expandBy(*itr);
        }
        if (m_bbox.valid())
        {
            m_boundSphere.expandBy(m_bbox);
            m_bBoundSphere_computed=true;            
            result = true;
        }
    }
    return result;
}
