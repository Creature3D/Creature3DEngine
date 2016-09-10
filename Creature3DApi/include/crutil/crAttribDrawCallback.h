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
#ifndef CRUTIL_CRATTRIBDRAWCALLBACK_H
#define CRUTIL_CRATTRIBDRAWCALLBACK_H

#include <CRUtil/crExport.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crCopyOp.h>

namespace CRUtil{
#define crEffectDrawCallback crAttribDrawCallback
class CRUTIL_EXPORT crAttribDrawCallback : public CRCore::crDrawable::DrawCallback
{
public:
	crAttribDrawCallback(){}
	virtual void drawImplementation( const CRCore::crDrawable* pdrawable, CRCore::crState& state ) const;
};

class CRUTIL_EXPORT crBillboardDrawCallback : public CRCore::crDrawable::DrawCallback
{
public:
	crBillboardDrawCallback(){}
	virtual void drawImplementation( const CRCore::crDrawable* pdrawable, CRCore::crState& state ) const;
};

class CRUTIL_EXPORT crModelBlendDrawCallback : public CRCore::crDrawable::DrawCallback
{
public:
	crModelBlendDrawCallback(){}
	virtual void drawImplementation( const CRCore::crDrawable* pdrawable, CRCore::crState& state ) const;
};

}

#endif