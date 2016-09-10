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
#ifndef CRUTIL_CRSETOBJECTCOLORVISITOR_H
#define CRUTIL_CRSETOBJECTCOLORVISITOR_H 1

#include <CRCore/crNodeVisitor.h>
#include <CRCore/crObject.h>
#include <CRCore/crNode.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crVector4.h>
#include <CRUtil/crExport.h>

namespace CRUtil {

class CRUTIL_EXPORT crSetObjectColorVisitor : public CRCore::crNodeVisitor
{
public:
	crSetObjectColorVisitor();
    virtual ~crSetObjectColorVisitor();
	void setColor(const CRCore::crVector4 &color);
	virtual void apply(CRCore::crObject& node);
    virtual void apply(CRCore::crDrawable& drawable);
protected:
	CRCore::crVector4 m_color;
};

}

#endif

