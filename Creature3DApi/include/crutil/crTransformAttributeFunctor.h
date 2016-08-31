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
//Modified by Îâ²Æ»ª
#ifndef CRUTIL_TRANSFORMATTRIBUTEFUNCTOR
#define CRUTIL_TRANSFORMATTRIBUTEFUNCTOR 1

#include <CRCore/crDrawable.h>
#include <CRCore/crNotify.h>

#include <CRUtil/crExport.h>

namespace CRUtil {

/** Functor for transforming a drawable's vertex and normal attributes by specified matrix.
  * typically used for flattening transform down onto drawable leaves. */
class CRUTIL_EXPORT crTransformAttributeFunctor : public CRCore::crDrawable::AttributeFunctor
{
    public:
    
        /** construct a functor to transform a drawable's vertex and normal attributes by specified matrix.*/
        crTransformAttributeFunctor(const CRCore::crMatrix& m);
            
        virtual ~crTransformAttributeFunctor();

        /** do the work of transforming vertex and normal attributes. */
        virtual void apply(CRCore::crDrawable::AttributeType type,unsigned int count,CRCore::crVector3* begin);

        CRCore::crMatrix m_m;
        CRCore::crMatrix m_im;

};

}

#endif
