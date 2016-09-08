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
#include <CRUtil/crHighlightMapGenerator.h>

using namespace CRUtil;

crHighlightMapGenerator::crHighlightMapGenerator(const CRCore::crVector3 &light_direction,
                                             const CRCore::crVector4 &light_color,
                                             float specular_exponent,
                                             int texture_size):    
    crCubeMapGenerator(texture_size),
    m_ldir(light_direction),
    m_lcol(light_color),
    m_sexp(specular_exponent)
{
    m_ldir.normalize();
}

crHighlightMapGenerator::crHighlightMapGenerator(const crHighlightMapGenerator &copy, const CRCore::crCopyOp &copyop):    
    crCubeMapGenerator(copy, copyop),
    m_ldir(copy.m_ldir),
    m_lcol(copy.m_lcol),
    m_sexp(copy.m_sexp)
{
}
