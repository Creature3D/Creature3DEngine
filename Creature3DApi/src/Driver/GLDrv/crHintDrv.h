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
#include <CRCore/crStreamBuf.h>
void crHint::apply(crState& state) const
{
	unsigned int hintTarget;
	unsigned int hintMode;

	switch(m_hintTarget) 
	{
	case crHint::PERSPECTIVE_CORRECTION_HINT:
        hintTarget = GL_PERSPECTIVE_CORRECTION_HINT;
		break;
	case crHint::POINT_SMOOTH_HINT:
		hintTarget = GL_POINT_SMOOTH_HINT;
		break;
	case crHint::LINE_SMOOTH_HINT:
		hintTarget = GL_LINE_SMOOTH_HINT;
		break;
	case crHint::POLYGON_SMOOTH_HINT:
		hintTarget = GL_POLYGON_SMOOTH_HINT;
		break;
	case crHint::FOG_HINT:
		hintTarget = GL_FOG_HINT;
		break;
	//case crHint::PHONG_HINT:
	//	hintTarget = GL_PHONG_HINT;
	//	break;
	}

	switch(m_hintMode) 
	{
	case crHint::DONT_CARE:
		hintMode = GL_DONT_CARE;
		break;
	case crHint::FASTEST:
		hintMode = GL_FASTEST;
		break;
	case crHint::NICEST:
		hintMode = GL_NICEST;
		break;
	}

	glHint( hintTarget, hintMode );

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crHint OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}