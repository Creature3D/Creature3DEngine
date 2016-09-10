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
#include <CRCore/crStreamBuf.h>
void crDepth::apply(crState& state) const
{
	switch(m_func) 
	{
	case crDepth::NEVER:
		glDepthFunc(GL_NEVER);
		break;
	case crDepth::LESS:
		glDepthFunc(GL_LESS);
		break;
	case crDepth::EQUAL:
		glDepthFunc(GL_EQUAL);
		break;
	case crDepth::LEQUAL:
		glDepthFunc(GL_LEQUAL);
		break;
	case crDepth::GREATER:
		glDepthFunc(GL_GREATER);
		break;
	case crDepth::NOTEQUAL:
		glDepthFunc(GL_NOTEQUAL);
		break;
	case crDepth::GEQUAL:
		glDepthFunc(GL_GEQUAL);
		break;
	case crDepth::ALWAYS:
		glDepthFunc(GL_ALWAYS);
		break;
	}
	glDepthRange(m_zNear,m_zFar);
	glDepthMask(m_depthWriteMask);

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crDepth OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
