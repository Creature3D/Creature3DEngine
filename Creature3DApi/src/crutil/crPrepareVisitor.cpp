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
#include <CRUtil/crPrepareVisitor.h>
#include <CRUtil/crTangentSpaceGenerator.h>
#include <CRCore/crArgumentParser.h>

using namespace CRCore;
using namespace CRUtil;

void crPrepareVisitor::apply(CRCore::crNode& node)
{
	bool isNeedPrepare = false;
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"PrepareMode",str))
	{
		int mode = PrepareNone;
	    isNeedPrepare = true;
		if(str.compare("PrepareTexture")==0)
		{
            mode = PrepareTexture;
		}
		else if(str.compare("PrepareTBN")==0)
		{
			mode = PrepareTBN;
		}
		else if(str.compare("PrepareMesh")==0)
		{
			mode = PrepareMesh;
		}
		else if(str.compare("PrepareMesh|PrepareTBN")==0)
		{
			mode = PrepareMesh | PrepareTBN;
		}
		else if(str.compare("Default")==0)
		{//PrepareTexture | PrepareTBN
			mode = Default;
		}
		else if(str.compare("None")==0)
			mode = PrepareNone;
		m_prepareModeStack.push(mode);
	}

	if(isNeedPrepare)
	{
		crStateSet *ss = node.getStateSet();
	    if(ss && (m_prepareModeStack.top() & PrepareTexture)) prepareStateSet(ss);
	}

	traverse(node);

	if(isNeedPrepare)
		m_prepareModeStack.pop();

}

void crPrepareVisitor::apply(CRCore::crObject &node)
{
	bool isNeedPrepare = false;
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"PrepareMode",str))
	{
		int mode = PrepareNone;
		isNeedPrepare = true;
		if(str.compare("PrepareTexture")==0)
		{
			mode = PrepareTexture;
		}
		else if(str.compare("PrepareTBN")==0)
		{
			mode = PrepareTBN;
		}
		else if(str.compare("PrepareMesh")==0)
		{
			mode = PrepareMesh;
		}
		else if(str.compare("PrepareMesh|PrepareTBN")==0)
		{
			mode = PrepareMesh | PrepareTBN;
		}
		else if(str.compare("Default")==0)
		{//PrepareTexture | PrepareTBN
			mode = Default;
		}
		else if(str.compare("None")==0)
			mode = PrepareNone;
		m_prepareModeStack.push(mode);
	}

	if(m_prepareModeStack.empty()) return;

	crStateSet *ss = node.getStateSet();
	if(ss && (m_prepareModeStack.top() & PrepareTexture)) prepareStateSet(ss);

	for (unsigned i=0; i<node.getNumDrawables(); ++i) 
	{
		CRCore::crGeometry *geo = dynamic_cast<CRCore::crGeometry *>(node.getDrawable(i));
		if (geo) 
		{
			ss = geo->getStateSet();
			if(ss && (m_prepareModeStack.top() & PrepareTexture)) prepareStateSet(ss);
			if(m_prepareModeStack.top() & PrepareTBN) prepareTBN(geo);
			else if(m_prepareModeStack.top() & PrepareMesh) prepareMesh(geo);
		}
	}

	if(isNeedPrepare)
		m_prepareModeStack.pop();
}

void crPrepareVisitor::apply(CRCore::crBillboard &node)
{
	bool isNeedPrepare = false;
	std::string str;
	if(CRCore::crArgumentParser::readKeyValue(node.getDescriptions(),"PrepareMode",str))
	{
		int mode = PrepareNone;
		isNeedPrepare = true;
		if(str.compare("PrepareTexture")==0)
		{
			mode = PrepareTexture;
		}
		else if(str.compare("PrepareTBN")==0)
		{
			mode = PrepareTBN;
		}
		else if(str.compare("PrepareMesh")==0)
		{
			mode = PrepareMesh;
		}
		else if(str.compare("PrepareMesh|PrepareTBN")==0)
		{
			mode = PrepareMesh | PrepareTBN;
		}
		else if(str.compare("Default")==0)
		{//PrepareTexture | PrepareTBN
			mode = Default;
		}
		else if(str.compare("None")==0)
			mode = PrepareNone;
		m_prepareModeStack.push(mode);
	}

	if(m_prepareModeStack.empty()||!m_prepareModeStack.top()) return;

	crStateSet *ss = node.getStateSet();
    if(ss && (m_prepareModeStack.top() & PrepareTexture)) prepareStateSet(ss);

	for (unsigned i=0; i<node.getNumDrawables(); ++i) 
	{
		CRCore::crGeometry *geo = dynamic_cast<CRCore::crGeometry *>(node.getDrawable(i));
		if (geo) 
		{
			ss = geo->getStateSet();
			if(ss && (m_prepareModeStack.top() & PrepareTexture)) prepareStateSet(ss);
			if(m_prepareModeStack.top() & PrepareTBN) prepareTBN(geo);
			else if(m_prepareModeStack.top() & PrepareMesh) prepareMesh(geo);
		}
	}

	if(isNeedPrepare)
		m_prepareModeStack.pop();
}

void crPrepareVisitor::prepareMesh(CRCore::crGeometry *geo)
{
	//geo->setUseVertexBufferObjects(false);
	geo->setSupportsDisplayList(false);
}

void crPrepareVisitor::prepareTBN(CRCore::crGeometry *geo)
{
	CRCore::ref_ptr<CRUtil::crTangentSpaceGenerator> tsg = new CRUtil::crTangentSpaceGenerator;
	if(tsg->generate(geo, TEXTURE_BASEMAP/*TEXTURE_BUMPMAP*/))
	{
		//if (!geo->getVertexAttribArray(ATTRIBUTE_TANGENT))
		//{
			geo->setVertexAttribData(ATTRIBUTE_TANGENT, CRCore::crGeometry::ArrayData(tsg->getTangentArray(), CRCore::crGeometry::BIND_PER_VERTEX, false));
			geo->setVertexAttribBinding(ATTRIBUTE_TANGENT,CRCore::crGeometry::BIND_PER_VERTEX);
		//}
		//if (!geo->getVertexAttribArray(ATTRIBUTE_BINORMAL))
		//{
			//geo->setVertexAttribData(ATTRIBUTE_BINORMAL, CRCore::crGeometry::ArrayData(tsg->getBinormalArray(), CRCore::crGeometry::BIND_PER_VERTEX, false));
			//geo->setVertexAttribBinding(ATTRIBUTE_BINORMAL,CRCore::crGeometry::BIND_PER_VERTEX);
		//}
		//if (!geo->getVertexAttribArray(ATTRIBUTE_NORMAL))
		//	geo->setVertexAttribData(ATTRIBUTE_NORMAL, CRCore::crGeometry::ArrayData(tsg->getNormalArray(), CRCore::crGeometry::BIND_PER_VERTEX, false));

		//geo->setUseVertexBufferObjects(false);
		geo->setSupportsDisplayList(false);
	}
}

void crPrepareVisitor::prepareStateSet(CRCore::crStateSet *stateset)
{
	for(int i = 0; i<stateset->getTextureAttributeList().size(); i++)
	{
		stateset->removeTextureAttribute(i,crStateAttribute::TEXENV);
		stateset->removeTextureAttribute(i,crStateAttribute::TEXENVFILTER);
		//stateset->removeTextureAttribute(i,crStateAttribute::TEXGEN);
	}
}
//////////////////////////////////
//
//crRemoveTransStateVisitor
//
////////////////////////////////
void crRemoveTransStateVisitor::apply(CRCore::crNode& node)
{
	crStateSet *ss = node.getStateSet();
	if(ss) apply(*ss);
	traverse(node);
}

void crRemoveTransStateVisitor::apply(CRCore::crObject &node)
{
	crStateSet *ss = node.getStateSet();
	if(ss) apply(*ss);

	for (unsigned i=0; i<node.getNumDrawables(); ++i) 
	{
		CRCore::crGeometry *geo = dynamic_cast<CRCore::crGeometry *>(node.getDrawable(i));
		if (geo) 
		{
			ss = geo->getStateSet();
			if(ss) apply(*ss);
		}
	}
}
void crRemoveTransStateVisitor::apply(CRCore::crStateSet& stateset)
{
	stateset.setRenderBinToInherit();
	stateset.removeAttribute(crStateAttribute::BLENDFUNC);
	stateset.removeAttribute(crStateAttribute::ALPHAFUNC);
}