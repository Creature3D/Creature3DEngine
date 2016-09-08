/* Creature3D - Online Game Engine, Copyright (C) 2005 Îâ²Æ»ª(26756325@qq.com)
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
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/glext.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crState.h>
#include <CRCore/crStreamBuf.h>
void crTexEnvFilter::apply(crState& state) const
{
	// note from RO, need to adapt to do testing per graphics context.
	static float glVersion = atof( (const char *)glGetString( GL_VERSION ) );
	static bool s_isTexLodBias = glVersion>=1.4 || isGLExtensionSupported(state.getContextID(),"GL_EXT_texture_lod_bias");
	if (s_isTexLodBias)
	{
		glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, m_lodBias);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crTexEnvFilter OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}
