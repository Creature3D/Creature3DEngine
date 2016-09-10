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
#ifndef CRUTIL_CRACCEPTGIVISITOR_H
#define CRUTIL_CRACCEPTGIVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRUtil/crExport.h>

namespace CRUtil {

class CRUTIL_EXPORT crAcceptGIVisitor : public CRCore::crNodeVisitor
{
public:
	crAcceptGIVisitor(char acceptGI = 1);
    virtual ~crAcceptGIVisitor();

    virtual void apply(CRCore::crObject& node);
protected:
	char m_acceptGI;
};

class CRUTIL_EXPORT crSetObjectOutlineColorModeVisitor : public CRCore::crNodeVisitor
{
public:
	crSetObjectOutlineColorModeVisitor(CRCore::crObject::OutlineColorMode mode);
	virtual ~crSetObjectOutlineColorModeVisitor();
	inline void setOutlineColorMode(CRCore::crObject::OutlineColorMode mode) { m_colormode = mode; }
	virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crBillboard& node);
	virtual void apply(CRParticle::crParticleEffect& node);
protected:
	CRCore::crObject::OutlineColorMode m_colormode;
};

class CRUTIL_EXPORT crDrawOutLineVisitor : public CRCore::crNodeVisitor
{
public:
	crDrawOutLineVisitor(char outline);
	virtual ~crDrawOutLineVisitor();
	inline void setDrawOutLine(char outline) { m_outline = outline; }
	virtual void apply(CRCore::crObject& node);
	virtual void apply(CRCore::crBillboard& node);
	virtual void apply(CRParticle::crParticleEffect& node);
protected:
	char m_outline;
};

class CRUTIL_EXPORT crFinalCommandParseVisitor : public CRCore::crNodeVisitor
{
public:
	crFinalCommandParseVisitor():
		CRCore::crNodeVisitor(CRCore::crNodeVisitor::TRAVERSE_ALL_CHILDREN){}
	virtual void apply(CRCore::crGroup& node);
	virtual void apply(CRCore::crObject& node);
};
}

#endif

