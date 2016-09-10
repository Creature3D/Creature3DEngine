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
#include <CRCore/crPrimitive.h>
//#include <CRCore/crNotify.h>

using namespace std;
using namespace CRCore;

crPrimitive::~crPrimitive() 
{ 
	//releaseObjects(); 
}

void DrawArrays::accept(crDrawable::PrimitiveFunctor& functor) const
{
    functor.drawArrays(m_mode,m_first,m_count);
}

void DrawArrays::accept(crDrawable::PrimitiveIndexFunctor& functor) const
{
	functor.drawArrays(m_mode,m_first,m_count);
}

void DrawArrayLengths::accept(crDrawable::PrimitiveFunctor& functor) const
{
    int first = m_first;
    for(PT_IndexArray::const_iterator itr=m_indexArray.begin();
        itr!=m_indexArray.end();
        ++itr)
    {
        functor.drawArrays(m_mode,first,*itr);
        first += *itr;
    }
}

void DrawArrayLengths::accept(crDrawable::PrimitiveIndexFunctor& functor) const
{
	int first = m_first;
	for(PT_IndexArray::const_iterator itr=m_indexArray.begin();
		itr!=m_indexArray.end();
		++itr)
	{
		functor.drawArrays(m_mode,first,*itr);
		first += *itr;
	}
}

unsigned int DrawArrayLengths::getNumIndices() const
{
    unsigned int count = 0;
    for(PT_IndexArray::const_iterator itr=m_indexArray.begin();
        itr!=m_indexArray.end();
        ++itr)
    {
        count += *itr;
    }
    return count;
}

unsigned int DrawArrayLengths::getNumIndices()
{
	unsigned int count = 0;
	for(PT_IndexArray::iterator itr=m_indexArray.begin();
		itr!=m_indexArray.end();
		++itr)
	{
		count += *itr;
	}
	return count;
}

void DrawElementsUByte::accept(crDrawable::PrimitiveFunctor& functor) const
{
    if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}

void DrawElementsUByte::accept(crDrawable::PrimitiveIndexFunctor& functor) const
{
	if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}

void DrawElementsUShort::accept(crDrawable::PrimitiveFunctor& functor) const
{
    if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}

void DrawElementsUShort::accept(crDrawable::PrimitiveIndexFunctor& functor) const
{
	if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}

void DrawElementsUInt::accept(crDrawable::PrimitiveFunctor& functor) const
{
    if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}

void DrawElementsUInt::accept(crDrawable::PrimitiveIndexFunctor& functor) const
{
	if (!m_indexArray.empty()) functor.drawElements(m_mode,m_indexArray.size(),&m_indexArray.front());
}
#include <Driver/GLDrv/crPrimitiveDrv.h>