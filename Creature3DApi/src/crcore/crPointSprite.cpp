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
//Modified by 吴财华
#include <CRCore/crPointSprite.h>
#include <CRCore/crState.h>
#include <CRCore/buffered_value.h>
#include <crgl/GLExtensions.h>
#include <crgl/GL.h>
using namespace CRCore;

int crPointSprite::compare(const crStateAttribute& sa) const
{
    if (this==&sa) return 0;\
    const std::type_info* type_lhs = &typeid(*this);\
    const std::type_info* type_rhs = &typeid(sa);\
    if (type_lhs->before(*type_rhs)) return -1;\
    if (*type_lhs != *type_rhs) return 1;\

    return 0; // passed all the above comparison macro's, must be equal.
}

int crPointSprite::compare(const crStateAttribute& sa)
{
	if (this==&sa) return 0;\
		const std::type_info* type_lhs = &typeid(*this);\
		const std::type_info* type_rhs = &typeid(sa);\
		if (type_lhs->before(*type_rhs)) return -1;\
			if (*type_lhs != *type_rhs) return 1;\

				return 0; // passed all the above comparison macro's, must be equal.
}

bool crPointSprite::checkValidityOfAssociatedModes(crState& state) const
{
	bool modeValid = isPointSpriteSupported(state.getContextID());
	state.setModeValidity(GL_POINT_SPRITE_ARB, modeValid);

	return modeValid;
}

struct IntializedSupportedPair
{
	IntializedSupportedPair():
initialized(false),
supported(false) {}

bool initialized;
bool supported;
};

typedef buffered_object< IntializedSupportedPair > BufferedExtensions;
static BufferedExtensions s_extensions;
bool crPointSprite::isPointSpriteSupported(unsigned int contextID)
{
	if (!s_extensions[contextID].initialized)
	{
		s_extensions[contextID].initialized = true;
		s_extensions[contextID].supported = isGLExtensionSupported(contextID, "GL_ARB_point_sprite") || isGLExtensionSupported(contextID, "GL_NV_point_sprite");
	}

	return s_extensions[contextID].supported;
}
#include <Driver/GLDrv/crPointSpriteDrv.h>