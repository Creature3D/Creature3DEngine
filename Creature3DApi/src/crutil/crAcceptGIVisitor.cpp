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
#include <CRUtil/crAcceptGIVisitor.h>
#include <CRCore/crArgumentParser.h>
using namespace CRCore;
using namespace CRUtil;
/////////////////////////////////////////
//
//crAcceptGIVisitor
//
/////////////////////////////////////////
crAcceptGIVisitor::crAcceptGIVisitor(char acceptGI):
	m_acceptGI(acceptGI),
    crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN)
{
}

crAcceptGIVisitor::~crAcceptGIVisitor()
{
}

void crAcceptGIVisitor::apply(CRCore::crObject& node)
{ 
	node.setAcceptGI(m_acceptGI);
}
/////////////////////////////////////////
//
//crSetObjectOutlineColorModeVisitor
//
/////////////////////////////////////////
crSetObjectOutlineColorModeVisitor::crSetObjectOutlineColorModeVisitor(CRCore::crObject::OutlineColorMode mode):
	m_colormode(mode),
	crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN)
{
}

crSetObjectOutlineColorModeVisitor::~crSetObjectOutlineColorModeVisitor()
{
}

void crSetObjectOutlineColorModeVisitor::apply(CRCore::crObject& node)
{
	if(node.getDrawOutLine()>=0)
		node.setOutlineColorMode(m_colormode);
}
void crSetObjectOutlineColorModeVisitor::apply(CRCore::crBillboard& node)
{
	return;
}
void crSetObjectOutlineColorModeVisitor::apply(CRParticle::crParticleEffect& node)
{
	return;
}
/////////////////////////////////////////
//
//crDrawOutLineVisitor
//
/////////////////////////////////////////
crDrawOutLineVisitor::crDrawOutLineVisitor(char outline):
m_outline(outline),
crNodeVisitor(NODE_VISITOR,TRAVERSE_ALL_CHILDREN)
{
}

crDrawOutLineVisitor::~crDrawOutLineVisitor()
{
}

void crDrawOutLineVisitor::apply(CRCore::crObject& node)
{ 
	node.setDrawOutLine(m_outline);
}
void crDrawOutLineVisitor::apply(CRCore::crBillboard& node)
{
	return;
}
void crDrawOutLineVisitor::apply(CRParticle::crParticleEffect& node)
{
	return;
}
/////////////////////////////////////////
//
//crFinalCommandParseVisitor
//
/////////////////////////////////////////
void crFinalCommandParseVisitor::apply(CRCore::crGroup& node)
{ 
	int int1;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"AcceptGI",int1))
	{
		crAcceptGIVisitor vistor(int1);
		node.accept(vistor);
	}
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"DrawOutLine",int1))
	{
		crDrawOutLineVisitor vistor(int1);
		node.accept(vistor);
	}
	traverse(node);
}
void crFinalCommandParseVisitor::apply(CRCore::crObject& node)
{
	int int1;
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"AcceptGI",int1))
		node.setAcceptGI(int1);
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"NoSkyLight",int1) && int1==1)
		node.setAcceptGI(-1);
	if(crArgumentParser::readKeyValue(node.getDescriptions(),"DrawOutLine",int1))
		node.setDrawOutLine(int1);
}