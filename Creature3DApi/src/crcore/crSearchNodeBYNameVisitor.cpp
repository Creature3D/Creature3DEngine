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
#include <CRCore/crSearchNodeBYNameVisitor.h>
#include <CRCore/crNotify.h>
using namespace CRCore;

void crSearchNodeBYNameVisitor::apply(crNode& node)         
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == NODE) 
		handle(node);
	traverse(node);
}

void crSearchNodeBYNameVisitor::apply(crGroup& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == GROUP) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crObject& node)       
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == OBJECT) 
		handle((CRCore::crNode&)node);

	traverse(node);
}

void crSearchNodeBYNameVisitor::apply(crEventGroup& node)   
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == EVENTGROUP) 
		handle((CRCore::crNode&)node); 
	
	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crLightSource& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == LIGHTSOURCE) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crTransform& node)
{
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == TRANSFORM) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crMatrixTransform& node)
{
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == MATRIXTRANSFORM) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}
void crSearchNodeBYNameVisitor::apply(crMultiSwitch& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == MULTISWITCH) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}
void crSearchNodeBYNameVisitor::apply(crLod& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == LOD) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crPagedLOD& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == PAGEDLOD) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crSequence& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == SEQUENCE) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crSearchNodeBYNameVisitor::apply(crWidgetNode& node)
{ 
	if(m_result.valid()) return;

	if(m_searchNodeType == ALLNODE || m_searchNodeType == WIDGETNODE) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}
///////////////////////////
//
//crCollectNodeBYNameVisitor
//
////////////////////////////
void crCollectNodeBYNameVisitor::apply(crNode& node)         
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == NODE) 
		handle(node);
	traverse(node);
}

void crCollectNodeBYNameVisitor::apply(crGroup& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == GROUP) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crObject& node)       
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == OBJECT) 
		handle((CRCore::crNode&)node);

	traverse(node);
}

void crCollectNodeBYNameVisitor::apply(crEventGroup& node)   
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == EVENTGROUP) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crLightSource& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == LIGHTSOURCE) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crTransform& node)
{
	if(m_searchNodeType == ALLNODE || m_searchNodeType == TRANSFORM) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crMatrixTransform& node)
{
	if(m_searchNodeType == ALLNODE || m_searchNodeType == MATRIXTRANSFORM) 
		handle((CRCore::crNode&)node); 

	traverse(node); 
}
void crCollectNodeBYNameVisitor::apply(crMultiSwitch& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == MULTISWITCH) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}
void crCollectNodeBYNameVisitor::apply(crLod& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == LOD) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crPagedLOD& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == PAGEDLOD) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crSequence& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == SEQUENCE) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}

void crCollectNodeBYNameVisitor::apply(crWidgetNode& node)
{ 
	if(m_searchNodeType == ALLNODE || m_searchNodeType == WIDGETNODE) 
		handle((CRCore::crNode&)node);

	traverse(node); 
}
///////////////////////////
//
//crSearchNodeBYClassNameVisitor
//
////////////////////////////
void crSearchNodeBYClassNameVisitor::apply(crNode& node)         
{ 
	if(m_result.valid()) return;

	std::string str;
	if( m_classNameSet.find(node.className()) != m_classNameSet.end() )
	{
		m_result = &node;
		return;
	}
	traverse(node);
}
///////////////////////////
//
//crCollectNodeBYClassNameVisitor
//
////////////////////////////
void crCollectNodeBYClassNameVisitor::apply(crNode& node)         
{
	std::string str;
	if( m_classNameSet.find(node.className()) != m_classNameSet.end() )
	{
		m_result.push_back(&node);
	}
	traverse(node);
}
///////////////////////////
//
//crSearchNodeHasDataEventVisitor
//
////////////////////////////
void crSearchNodeHasDataEventVisitor::apply(crNode& node)         
{ 
	if(m_result.valid()) return;

	if( node.getDataClass() != NULL )
	{
		m_result = &node;
		return;
	}
	traverse(node);
}
///////////////////////////
//
//crCollectNodeBYCallbackVisitor
//
////////////////////////////
void crCollectNodeBYCallbackVisitor::apply(crNode& node)         
{
	if( node.getUpdateCallback(m_callback) || node.getCullCallback(m_callback) || node.getCollideCallback(m_callback) )
	{
		m_result.push_back(&node);
	}
	traverse(node);
}
///////////////////////////
//
//crContainNodeVisitor
//
////////////////////////////
void crContainNodeVisitor::apply(crNode& node)         
{ 
	if(m_result) return;

	if( &node == m_node )
	{
		m_result = true;
		return;
	}
	traverse(node);
}