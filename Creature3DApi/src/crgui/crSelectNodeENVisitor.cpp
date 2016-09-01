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
#include <CRGUI/crSelectNodeENVisitor.h>
#include <CRGUI/crElementNodeManager.h>

using namespace CRGUI;

crSelectNodeENVisitor::crSelectNodeENVisitor(int x, int y):
  crElementNodeVisitor(TRAVERSE_EXPAND_CHILDREN)
{
    m_point.set(x,y);
	m_flag = POINTSEL;
}

crSelectNodeENVisitor::crSelectNodeENVisitor(CRCore::crVector2i &point):
  crElementNodeVisitor(TRAVERSE_EXPAND_CHILDREN),
  m_point(point)
{
    m_flag = POINTSEL;
}

crSelectNodeENVisitor::crSelectNodeENVisitor(int left, int right, int top, int bottom):
  crElementNodeVisitor(TRAVERSE_EXPAND_CHILDREN)
{
	m_rect.set(left,top,right,bottom);
	m_flag = RECTSEL;
}
crSelectNodeENVisitor::crSelectNodeENVisitor(CRCore::crVector4i &rect):
  crElementNodeVisitor(TRAVERSE_EXPAND_CHILDREN),
  m_rect(rect)
{
	m_flag = RECTSEL;
}

crSelectNodeENVisitor::~crSelectNodeENVisitor()
{

}

void crSelectNodeENVisitor::apply(crElementNode& enode)
{
 /*   if(enode.isSelected())
	{		
		enode.setSelected(false);
		m_lastSelectedNodes.push_back(&enode);
	}*/

	switch(m_flag) 
	{
	case POINTSEL:
		if( m_point.x() >= enode.getRect()[0] && 
			m_point.x() <= enode.getRect()[0] + NODESIZE_X &&
			m_point.y() >= enode.getRect()[1] &&
			m_point.y() <= enode.getRect()[1] + NODESIZE_Y)
		{
			//enode.select(true);			
			crElementNodeManager::getInstance()->select(&enode);
		}
		break;
	case RECTSEL:
		if( m_rect.x() <= enode.getRect()[0] && 
			m_rect.z() >= enode.getRect()[0] + NODESIZE_X &&
			m_rect.y() <= enode.getRect()[1] &&
			m_rect.w() >= enode.getRect()[1] + NODESIZE_Y)
		{
			//enode.select(true);			
			crElementNodeManager::getInstance()->select(&enode);
			return;
		}
		break;
	case RECTINTERSECT:
		{
		    CRect rect;
			if( rect.IntersectRect( 
				    m_rect, 
				    CRCore::crVector4i(
					    enode.getRect()[0],
						enode.getRect()[1],
						enode.getRect()[0] + NODESIZE_X,
						enode.getRect()[1] + NODESIZE_Y ) ) )

                crDrawENVisitor::getInstance()->pushOntoNeedReDrawNodeList(&enode);
			else 
			{
				int num = enode.getNumChildren();
				if(num > 0 && enode.getMode()==crElementNode::EXPAND)
				{
					CPoint pt1,pt2;
					pt1.x = enode.getRect()[0] + NODESIZE_X/2;
					pt1.y = enode.getRect()[1] + NODESIZE_Y + SPACEPERLEVEL/2;
					pt2.x = enode.getChild(num-1)->getPosX() + NODESIZE_X/2;
					pt2.y = pt1.y;

					if(pt1.y >= m_rect.top && pt1.y <= m_rect.bottom)
					{
						if(!(pt2.x < m_rect.left || pt1.x > m_rect.right))
							crDrawENVisitor::getInstance()->pushOntoNeedReDrawNodeList(&enode);
					}
				}
			}
		}
		break;
	}

	traverse(enode);
}
