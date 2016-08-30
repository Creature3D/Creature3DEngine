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
#ifndef CRTEXT_GLYPHGEOMETRY
#define CRTEXT_GLYPHGEOMETRY 1

#include <CRText/crText3D.h>

namespace CRText
{

extern CRTEXT_EXPORT CRCore::crGeometry* computeGlyphGeometry(const CRText::crGlyph3D* glyph, float bevelThickness, float shellThickness);

extern CRTEXT_EXPORT CRCore::crGeometry* computeTextGeometry(const CRText::crGlyph3D* glyph, float width);

extern CRTEXT_EXPORT CRCore::crGeometry* computeTextGeometry(CRCore::crGeometry* glyphGeometry, const crBevel& profile, float width);

extern CRTEXT_EXPORT CRCore::crGeometry* computeShellGeometry(CRCore::crGeometry* glyphGeometry, const crBevel& profile, float width);

}

#endif
