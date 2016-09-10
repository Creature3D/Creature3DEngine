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
#include <CRCore/crNotify.h>
#include <CRCore/crState.h>
#include <CRCore/crStreamBuf.h>
void crMaterial::apply(crState& state) const
{
	if (m_colorMode!=OFF)
	{
		state.setIgnoreColor(true);
		glColor4fv(m_emissionFront.ptr());
	}
	else
	{
		state.setIgnoreColor(false);
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, m_ambientFront.ptr() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuseFront.ptr() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, m_specularFront.ptr() );
		glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, m_emissionFront.ptr() );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, m_shininessFront );
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crMaterial OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
