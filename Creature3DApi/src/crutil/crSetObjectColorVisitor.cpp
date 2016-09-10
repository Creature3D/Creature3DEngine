/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
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
#include <CRUtil/crSetObjectColorVisitor.h>

using namespace CRCore;
using namespace CRUtil;

crSetObjectColorVisitor::crSetObjectColorVisitor():
    crNodeVisitor(NODE_VISITOR,TRAVERSE_ACTIVE_CHILDREN)
{
}

crSetObjectColorVisitor::~crSetObjectColorVisitor()
{
}

void crSetObjectColorVisitor::setColor(const CRCore::crVector4 &color)
{
	m_color = color;
}

void crSetObjectColorVisitor::apply(CRCore::crObject& node)      
{ 
	for(unsigned int i=0;i<node.getNumDrawables();++i)
	{
		crDrawable* drawable = node.getDrawable(i);
		if (drawable)
		{
			apply(*drawable);
		}
	}
}

void crSetObjectColorVisitor::apply(CRCore::crDrawable& drawable) 
{ 
	drawable.setColor(m_color);
}