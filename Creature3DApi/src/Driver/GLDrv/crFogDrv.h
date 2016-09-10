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
void crFog::apply(crState& state) const
{//not use
    switch(m_mode) 
	{
    case crFog::LINEAR:
		glFogi( GL_FOG_MODE, GL_LINEAR );
    	break;
	case crFog::EXP:
		glFogi( GL_FOG_MODE, GL_EXP );
		break;
	case crFog::EXP2:
		glFogi( GL_FOG_MODE, GL_EXP2 );
		break;
    }

	glFogf( GL_FOG_DENSITY, m_density );
	glFogf( GL_FOG_START, m_start );
	glFogf( GL_FOG_END, m_end );
	glFogfv( GL_FOG_COLOR, (GLfloat*)m_color.ptr() );

	static bool fogCoordExtensionSuppoted = isGLExtensionSupported(state.getContextID(),"GL_EXT_fog_coord");
	if (fogCoordExtensionSuppoted)
	{
		switch(m_fogCoordinateSource) 
		{
		case crFog::FOG_COORDINATE:
			glFogi( GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE );
			break;
		case crFog::FRAGMENT_DEPTH:
			glFogi( GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH );
			break;
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crFog OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

