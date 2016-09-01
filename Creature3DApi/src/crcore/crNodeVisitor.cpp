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
#include <CRCore/crNodeVisitor.h>
#include <CRCore/crTransform.h>
#include <stdlib.h>

using namespace CRCore;

crNodeVisitor::crNodeVisitor(TraversalMode tm):
        Referenced()
{
    m_visitorType = NODE_VISITOR;
    m_traversalNumber = -1;

    m_traversalMode = tm;
    m_traversalMask = 0xffffffff;
    m_nodeMaskOverride = 0x0;
}

crNodeVisitor::crNodeVisitor(VisitorType type,TraversalMode tm):
    Referenced()
{
    m_visitorType = type;
    m_traversalNumber = -1;

    m_traversalMode = tm;
    m_traversalMask = 0xffffffff;
    m_nodeMaskOverride = 0x0;
}


crNodeVisitor::~crNodeVisitor()
{
    // if (_traversalVisitor) detach from _traversalVisitor;
}

