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
void crClipPlane::apply(crState& state) const
{
	//glMatrixMode( GL_MODELVIEW );
	//glPushMatrix();
	//glLoadIdentity();

	//glEnable(GL_CLIP_PLANE0 + m_pClipPlane->getClipPlaneNum());
	glClipPlane((GLenum)(GL_CLIP_PLANE0+m_clipPlaneNum),m_clipPlane.ptr());
	
	//glPopMatrix();
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crClipPlane OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}