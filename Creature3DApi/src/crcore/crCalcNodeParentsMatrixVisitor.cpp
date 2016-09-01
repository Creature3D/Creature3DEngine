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
#include <CRCore/crCalcNodeParentsMatrixVisitor.h>

using namespace CRCore;

void crCalcNodeParentsMatrixVisitor::apply(CRCore::crTransform& node)
{
	if( node.getReferenceFrame() == crTransform::RELATIVE_TO_PARENTS )
	{
		crMatrix mat;
		node.computeLocalToWorldMatrix(mat,this);

		m_result.postMult(mat);

		traverse(node); 
	}
}
void crCalcNodeParentsMatrixVisitor::apply(CRCore::crWidgetNode& node)
{
	if( node.getEnableMatrix())
	{
		crMatrix mat;
		node.computeLocalToWorldMatrix(mat,this);

		m_result.postMult(mat);
	}
	traverse(node); 
}
void crCalcNodeParentsMatrixVisitor::apply(CRCore::crFilterRenderManager& node)
{
	return;
}