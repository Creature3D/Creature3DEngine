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
#include <CRCore/crClipNode.h>

using namespace CRCore;

crClipNode::crClipNode()
{
    m_value = crStateAttribute::ON;
    m_stateset = new crStateSet;
}

crClipNode::crClipNode(const crClipNode& cn, const crCopyOp& copyop):crGroup(cn,copyop)
{
    for(ClipPlaneList::const_iterator itr=cn.m_planes.begin();
        itr!=cn.m_planes.end();
        ++itr)
    {
        crClipPlane* plane = dynamic_cast<crClipPlane*>(copyop(itr->get()));
        if (plane) addClipPlane(plane);
    }
}

crClipNode::~crClipNode()
{
}

// Create a 6 clip planes to create a clip box.
void crClipNode::createClipBox(const crBoundingBox& bb,unsigned int clipPlaneNumberBase)
{
    m_planes.clear();

    m_planes.push_back(new crClipPlane(clipPlaneNumberBase  ,1.0,0.0,0.0,-bb.xMin()));
    m_planes.push_back(new crClipPlane(clipPlaneNumberBase+1,-1.0,0.0,0.0,bb.xMax()));

    m_planes.push_back(new crClipPlane(clipPlaneNumberBase+2,0.0,1.0,0.0,-bb.yMin()));
    m_planes.push_back(new crClipPlane(clipPlaneNumberBase+3,0.0,-1.0,0.0,bb.yMax()));

    m_planes.push_back(new crClipPlane(clipPlaneNumberBase+4,0.0,0.0,1.0,-bb.zMin()));
    m_planes.push_back(new crClipPlane(clipPlaneNumberBase+5,0.0,0.0,-1.0,bb.zMax()));

    setLocalStateSetModes(m_value);
}

// Add a crClipPlane to a crClipNode. Return true if plane is added, 
// return false if plane already exists in crClipNode, or clipplane is false.
bool crClipNode::addClipPlane(crClipPlane* clipplane)
{
    if (!clipplane) return false;

    if (std::find(m_planes.begin(),m_planes.end(),clipplane)==m_planes.end())
    {
        // cliplane doesn't exist in list so add it.
        m_planes.push_back(clipplane);
        setLocalStateSetModes(m_value);
        return true;
    }
    else
    {
        return false;
    }
}

// Remove crClipPlane from a crClipNode. Return true if plane is removed, 
// return false if plane does not exists in crClipNode.
bool crClipNode::removeClipPlane(crClipPlane* clipplane)
{
    if (!clipplane) return false;

    ClipPlaneList::iterator itr = std::find(m_planes.begin(),m_planes.end(),clipplane);
    if (itr!=m_planes.end())
    {
        // cliplane exist in list so erase it.
        m_planes.erase(itr);
        setLocalStateSetModes(m_value);
        return true;
    }
    else
    {
        return false;
    }
}

// Remove crClipPlane, at specified index, from a crClipNode. Return true if plane is removed, 
// return false if plane does not exists in crClipNode.
bool crClipNode::removeClipPlane(unsigned int pos)
{
    if (pos<m_planes.size())
    {
        m_planes.erase(m_planes.begin()+pos);
        setLocalStateSetModes(m_value);
        return true;
    }
    else
    {
        return false;
    }
}

// Set the GLModes on crStateSet associated with the ClipPlanes.
void crClipNode::setStateSetModes(crStateSet& stateset,const crStateAttribute::VRModeValue value) const
{
    for(ClipPlaneList::const_iterator itr=m_planes.begin();
        itr!=m_planes.end();
        ++itr)
    {
		//stateset.setAttributeAndModes(const_cast<crClipPlane *>(itr->get()),value);
        stateset.setAssociatedModes(itr->get(),value);
    }
}

void crClipNode::setLocalStateSetModes(const crStateAttribute::VRModeValue value)
{
    if (!m_stateset) m_stateset = new crStateSet;
    m_stateset->setAllToInherit();
    setStateSetModes(*m_stateset,value);
}

bool crClipNode::computeBound() const
{
    return crGroup::computeBound();
}
