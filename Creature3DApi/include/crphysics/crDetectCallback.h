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
#ifndef CRPHYSICS_CRDETECTCALLBACK_H
#define CRPHYSICS_CRDETECTCALLBACK_H 1

#include <CRPhysics/crExport.h>
#include <CRPhysics/crViewMatterObject.h>
#include <CRCore/crNode.h>
#include <CRCore/crNodeCallback.h>

namespace CRPhysics{

class CRPHYSICS_EXPORT crDetectCallback : public CRCore::crNodeCallback
{//node update callback
public:
	crDetectCallback(){}
	static crDetectCallback *getInstance();
	virtual void operator()(CRCore::crNode *node, CRCore::crNodeVisitor* nv);
protected:
	virtual ~crDetectCallback(){}
};
}

#endif