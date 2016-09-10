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
#include <CRGUI/crDrawTreeENVisitor.h>

using namespace CRCore;
using namespace CRGUI;

crDrawTreeENVisitor::crDrawTreeENVisitor():
  crElementNodeVisitor(TRAVERSE_ALL_CHILDREN)
{
}


crDrawTreeENVisitor::~crDrawTreeENVisitor()
{

}

void crDrawTreeENVisitor::apply(crElementNode& enode)
{
	if(!m_modeStack.empty()&&m_modeStack.top()==crElementNode::CONTRACT)
	{
		enode.setShow(false);
		m_modeStack.push(crElementNode::CONTRACT);
	}
	else
	{
		enode.setShow(true);
		m_modeStack.push(enode.getMode());
	}

	traverse(enode);
	m_modeStack.pop();
}