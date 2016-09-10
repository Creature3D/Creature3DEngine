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
#include <CRCore/crNode.h>
#include <CRCore/crNodeCallback.h>
#include <CRCore/crNodeVisitor.h>

using namespace CRCore;

crNodeCallback::crNodeCallback(const crNodeCallback& nc,const crCopyOp&copyop):
crEventCallback(nc,copyop),
m_cfgFile(nc.m_cfgFile),
m_swapFrameNumber(0)
{
	if(nc.crNestedCallbackPtr.valid())
	{
		crNestedCallbackPtr = dynamic_cast<crNodeCallback *>(copyop(nc.crNestedCallbackPtr.get()));
	}
}

void crNodeCallback::traverse(crNode* node,crNodeVisitor* nv)
{
    if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node,nv);
    else nv->traverse(*node);
}
