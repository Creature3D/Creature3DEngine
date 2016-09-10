/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#include <CRCore/crCopyOp.h>

#include <CRCore/crNode.h>
#include <CRCore/crObject.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crTexture.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crArray.h>
#include <CRCore/crPrimitive.h>
#include <CRCore/crShape.h>
#include <CRCore/crEventCallbackManager.h>
using namespace CRCore;

#define COPY_OP( TYPE, FLAG ) \
	TYPE* crCopyOp::operator() (const TYPE* obj) const \
{ \
	if (obj && m_flags&FLAG) \
	return dynamic_cast<TYPE*>( obj->clone(*this) ); \
	else \
	return const_cast<TYPE*>(obj); \
} 

COPY_OP( crBase,        DEEP_COPY_BASES );
COPY_OP( crNode,        DEEP_COPY_NODES );
COPY_OP( crDrawable,    DEEP_COPY_DRAWABLES );
COPY_OP( crStateSet,    DEEP_COPY_STATESETS );
COPY_OP( crTexture,     DEEP_COPY_TEXTURES );
COPY_OP( crImage,       DEEP_COPY_IMAGES );
COPY_OP( Array,         DEEP_COPY_ARRAYS );
COPY_OP( crPrimitive,   DEEP_COPY_PRIMITIVES );
COPY_OP( crShape,       DEEP_COPY_SHAPES );
COPY_OP( crUniform,     DEEP_COPY_UNIFORMS );
COPY_OP( crData,        DEEP_COPY_DATAS );
COPY_OP( crEventCallback,        DEEP_COPY_Callbacks );
Referenced* crCopyOp::operator() (const Referenced* ref) const
{
	return const_cast<Referenced*>(ref);
}

crStateAttribute* crCopyOp::operator() (const crStateAttribute* attr) const
{
	if (attr && m_flags&DEEP_COPY_STATEATTRIBUTES)
	{
		const crTexture* textbase = dynamic_cast<const crTexture*>(attr);
		if (textbase)
		{
			return operator()(textbase);
		}
		else 
		{
			return dynamic_cast<crStateAttribute*>(attr->clone(*this));
		}
	}
	else
		return const_cast<crStateAttribute*>(attr);
}

//
//Referenced* crCopyOp::operator() (const Referenced* ref) const
//{
//  return const_cast<Referenced*>(ref);
//}
//
//crBase* crCopyOp::operator() (const crBase* obj) const
//{
//  if (obj && m_flags&DEEP_COPY_BASES)
//    return obj->clone(*this);
//  else return const_cast<crBase*>(obj);
//}
//
//
//crNode* crCopyOp::operator() (const crNode* node) const
//{
//  if (node && m_flags&DEEP_COPY_NODES)
//    return dynamic_cast<crNode*>(node->clone(*this));
//  else
//    return const_cast<crNode*>(node);
//}
//
//crDrawable* crCopyOp::operator() (const crDrawable* drawable) const
//{
//  if (drawable && m_flags&DEEP_COPY_DRAWABLES)
//    return dynamic_cast<crDrawable*>(drawable->clone(*this));
//  else
//    return const_cast<crDrawable*>(drawable);
//}
//
//crStateSet* crCopyOp::operator() (const crStateSet* stateset) const
//{
//  if (stateset && m_flags&DEEP_COPY_STATESETS)
//     return dynamic_cast<crStateSet*>(stateset->clone(*this));
//  else
//     return const_cast<crStateSet*>(stateset);
//
//  return const_cast<crStateSet*>(stateset);
//}
//
//crStateAttribute* crCopyOp::operator() (const crStateAttribute* attr) const
//{
//  if (attr && m_flags&DEEP_COPY_STATEATTRIBUTES)
//  {
//    const crTexture* textbase = dynamic_cast<const crTexture*>(attr);
//    if (textbase)
//    {
//      return operator()(textbase);
//    }
//    else 
//    {
//      return dynamic_cast<crStateAttribute*>(attr->clone(*this));
//    }
//  }
//  else
//    return const_cast<crStateAttribute*>(attr);
//
//  return const_cast<crStateAttribute*>(attr);
//}
//
//crTexture* crCopyOp::operator() (const crTexture* text) const
//{
//  if (text && m_flags&DEEP_COPY_TEXTURES)
//    return dynamic_cast<crTexture*>(text->clone(*this));
//  else
//    return const_cast<crTexture*>(text);
//}
//
//crImage* crCopyOp::operator() (const crImage* image) const
//{
//  if (image && m_flags&DEEP_COPY_IMAGES)
//    return dynamic_cast<crImage*>(image->clone(*this));
//  else return const_cast<crImage*>(image);
//}
//
//Array* crCopyOp::operator() (const Array* array) const
//{
//  if (array && m_flags&DEEP_COPY_ARRAYS)
//    return dynamic_cast<Array*>(array->clone(*this));
//  else
//    return const_cast<Array*>(array);
//}
//
//crPrimitive* crCopyOp::operator() (const crPrimitive* primitive) const
//{
//  if (primitive && m_flags&DEEP_COPY_PRIMITIVES)
//    return dynamic_cast<crPrimitive*>(primitive->clone(*this));
//  else
//    return const_cast<crPrimitive*>(primitive);
//}
//
//crShape* crCopyOp::operator() (const crShape* shape) const
//{
//  if (shape && m_flags&DEEP_COPY_SHAPES)
//    return dynamic_cast<crShape*>(shape->clone(*this));
//  else
//    return const_cast<crShape*>(shape);
//}
//
