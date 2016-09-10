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
#include <crgl/GL.h>
#include <crgl/glu.h>
#include <crgl/glext.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crState.h>
#include <CRCore/crStreamBuf.h>
PFNGLPOINTPARAMETERFEXTPROC s_PointParameterfEXT = NULL;
PFNGLPOINTPARAMETERFVEXTPROC s_PointParameterfvEXT = NULL;
void init_GL_EXT(unsigned int contextID)
{
	if (isGLExtensionSupported(contextID,"GL_EXT_point_parameters"))
	{
		s_PointParameterfEXT = (PFNGLPOINTPARAMETERFEXTPROC)
			getGLExtensionFuncPtr("glPointParameterfEXT");
		s_PointParameterfvEXT = (PFNGLPOINTPARAMETERFVEXTPROC)
			getGLExtensionFuncPtr("glPointParameterfvEXT");
	}
	else if (isGLExtensionSupported(contextID,"GL_SGIS_point_parameters"))
	{
		s_PointParameterfEXT = (PFNGLPOINTPARAMETERFEXTPROC)
			getGLExtensionFuncPtr("glPointParameterfSGIS");
		s_PointParameterfvEXT = (PFNGLPOINTPARAMETERFVEXTPROC)
			getGLExtensionFuncPtr("glPointParameterfvSGIS");
	}
}
void crPoint::apply(crState& state) const
{
	glPointSize(m_size);

	static bool s_gl_ext_init=false;

	if (!s_gl_ext_init)
	{
		s_gl_ext_init = true;
		init_GL_EXT(state.getContextID());
	}

	if (s_PointParameterfvEXT) s_PointParameterfvEXT(GL_DISTANCE_ATTENUATION_EXT, (const GLfloat*)&m_distanceAttenuation);

	if (s_PointParameterfEXT)
	{
		s_PointParameterfEXT(GL_POINT_FADE_THRESHOLD_SIZE_EXT, m_fadeThresholdSize);
		s_PointParameterfEXT(GL_POINT_SIZE_MIN_EXT, m_minSize);
		s_PointParameterfEXT(GL_POINT_SIZE_MAX_EXT, m_maxSize);
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crPoint OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}