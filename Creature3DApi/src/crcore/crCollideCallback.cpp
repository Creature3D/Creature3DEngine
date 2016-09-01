/* Creature3D - Online Game Engine, Copyright (C) 2005 吴财华(26756325@qq.com)
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
#include <CRCore/crNode.h>
#include <CRCore/crCollideCallback.h>

using namespace CRCore;

crCollideCallback::crCollideCallback(const crCollideCallback& cc,const crCopyOp&copyop):
CRCore::crEventCallback(cc,copyop)
{
	if(cc.crNestedCallbackPtr.valid())
	{
		crNestedCallbackPtr = dynamic_cast<crCollideCallback *>(copyop(cc.crNestedCallbackPtr.get()));
	}
}
void crCollideCallback::operator()(crNode* node1, crNode* node2, void *o1, void *o2,const CRCore::crVector3f& collidePos, const CRCore::crVector3f& collideNormal)
{//node1与node2碰撞，此处只处理node1碰撞所触发的事件
	//处理碰撞事件

	if (crNestedCallbackPtr.valid()) (*crNestedCallbackPtr)(node1,node2,o1,o2,collidePos,collideNormal);
}