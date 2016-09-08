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
//Modified by Îâ²Æ»ª
#if defined(_MSC_VER)
	#pragma warning( disable : 4786 )
#endif

#include <stdio.h>
#include <list>
#include <set>

#include <CRUtil/crDisplayRequirementsVisitor.h>

using namespace CRCore;
using namespace CRUtil;

crDisplayRequirementsVisitor::crDisplayRequirementsVisitor()
{
    setTraversalMode(crNodeVisitor::TRAVERSE_ALL_CHILDREN);
}

void crDisplayRequirementsVisitor::applyStateSet(crStateSet& stateset)
{
    if (!m_ds) m_ds = new CRCore::crDisplaySettings;

   unsigned int min = 0; // assume stencil not needed by this stateset.
   
   if (stateset.getMode(GL_STENCIL_TEST) & crStateAttribute::ON)
   {
        min = 1; // number stencil bits we need at least.
   }

   if (stateset.getAttribute(crStateAttribute::STENCIL))
   {
        min = 1; // number stencil bits we need at least.
   }
   
   if (min>m_ds->getMinimumNumStencilBits())
   {
        // only update if new minimum exceeds previous minimum.
        m_ds->setMinimumNumStencilBits(min);
   }    
}

void crDisplayRequirementsVisitor::apply(crNode& node)
{
    CRCore::crStateSet* stateset = node.getStateSet();
    if (stateset) applyStateSet(*stateset);

    traverse(node);
}

void crDisplayRequirementsVisitor::apply(crObject& object)
{
    CRCore::crStateSet* object_stateset = object.getStateSet();
    if (object_stateset) applyStateSet(*object_stateset);
    
    for(unsigned int i = 0; i < object.getNumDrawables(); i++ )
    {
        CRCore::crStateSet* stateset = object.getDrawable(i)->getStateSet();
        if (stateset) applyStateSet(*stateset);
    }
}

/*
void crDisplayRequirementsVisitor::apply(Impostor& impostor)
{
    if (!m_ds) m_ds = new CRCore::crDisplaySettings;

    unsigned int min = 1; // number alpha bits we need at least.
    if (min>m_ds->getMinimumNumAlphaBits())
    {
        // only update if new minimum exceeds previous minimum.
        m_ds->setMinimumNumAlphaBits(min);
    }
    
    apply((crNode&)impostor);
}
*/