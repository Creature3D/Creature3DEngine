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
#include <CRCore/crStreamBuf.h>
void crPolygonMode::apply(crState& state) const
{
	if (m_frontAndBack)
	{
		switch(m_frontMode) 
		{
		case crPolygonMode::POINT:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	  		break;
		case crPolygonMode::LINE:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	  		break;
		case crPolygonMode::FILL:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		}
	}
	else
	{
		switch(m_frontMode) 
		{
		case crPolygonMode::POINT:
			glPolygonMode(GL_FRONT,GL_POINT);
			break;
		case crPolygonMode::LINE:
			glPolygonMode(GL_FRONT,GL_LINE);
			break;
		case crPolygonMode::FILL:
			glPolygonMode(GL_FRONT,GL_FILL);
			break;
		}

		switch(m_backMode) 
		{
		case crPolygonMode::POINT:
			glPolygonMode(GL_BACK,GL_POINT);
			break;
		case crPolygonMode::LINE:
			glPolygonMode(GL_BACK,GL_LINE);
			break;
		case crPolygonMode::FILL:
			glPolygonMode(GL_BACK,GL_FILL);
			break;
		}
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crPolygonMode OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}