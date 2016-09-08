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
#include <CRUtil/crTransformAttributeFunctor.h>

using namespace CRUtil;

crTransformAttributeFunctor::crTransformAttributeFunctor(const CRCore::crMatrix& m)
{
    m_m = m;
    m_im.invert(m_m);
}

crTransformAttributeFunctor::~crTransformAttributeFunctor()
{
}

void crTransformAttributeFunctor::apply(CRCore::crDrawable::AttributeType type,unsigned int count,CRCore::crVector3* begin)
{
    if (type == CRCore::crDrawable::VERTICES)
    {
        CRCore::crVector3* end = begin+count;
        for (CRCore::crVector3* itr=begin;itr<end;++itr)
        {
            (*itr) = (*itr)*m_m;
        }
    }
    else if (type == CRCore::crDrawable::NORMALS)
    {
        CRCore::crVector3* end = begin+count;
        for (CRCore::crVector3* itr=begin;itr<end;++itr)
        {
            // note post mult by inverse for normals.
            (*itr) = CRCore::crMatrix::transform3x3(m_im,(*itr));
            (*itr).normalize();
        }
    }
}
