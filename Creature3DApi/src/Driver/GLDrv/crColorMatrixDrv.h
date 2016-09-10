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
#include <crgl/gl.h>
#include <crgl/glu.h>
#include <crgl/GLExtensions.h>
#include <CRCore/crStreamBuf.h>
void crColorMatrix::apply(crState& state) const
{
	static bool s_ARB_imaging = isGLExtensionSupported(state.getContextID(),"GL_ARB_imaging");
	if (s_ARB_imaging)
	{
		glMatrixMode( GL_COLOR );
		glLoadMatrix(m_matrix.ptr());
		glMatrixMode( GL_MODELVIEW );
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crColorMatrix OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}