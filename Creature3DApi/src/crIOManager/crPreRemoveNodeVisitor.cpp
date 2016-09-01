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
#include <CRIOManager/crPreRemoveNodeVisitor.h>
using namespace CRIOManager;
using namespace CRCore;
crPreRemoveNodeVisitor::crPreRemoveNodeVisitor()
{
	setTraversalMode(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN);
}
crPreRemoveNodeVisitor::~crPreRemoveNodeVisitor()
{

}
crPreRemoveNodeVisitor* crPreRemoveNodeVisitor::getInstance()
{
	static ref_ptr<crPreRemoveNodeVisitor> m_instance = new crPreRemoveNodeVisitor;
	return m_instance.get();
}
void crPreRemoveNodeVisitor::apply(crNode& node)         
{ 
	node.preRemoveNode();
	traverse(node);
}