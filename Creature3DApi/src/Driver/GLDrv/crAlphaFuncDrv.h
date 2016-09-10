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
#include <CRCore/crStreamBuf.h>
void crAlphaFunc::apply(crState& state) const
{
	switch(m_comparisonFunc) 
	{
	case crAlphaFunc::NEVER:
		glAlphaFunc(GL_NEVER,m_referenceValue);
		break;
	case crAlphaFunc::LESS:
		glAlphaFunc(GL_LESS,m_referenceValue);
		break;
	case crAlphaFunc::EQUAL:
		glAlphaFunc(GL_EQUAL,m_referenceValue);
		break;
	case crAlphaFunc::LEQUAL:
		glAlphaFunc(GL_LEQUAL,m_referenceValue);
		break;
	case crAlphaFunc::GREATER:
		glAlphaFunc(GL_GREATER,m_referenceValue);
		break;
	case crAlphaFunc::NOTEQUAL:
		glAlphaFunc(GL_NOTEQUAL,m_referenceValue);
		break;
	case crAlphaFunc::GEQUAL:
		glAlphaFunc(GL_GEQUAL,m_referenceValue);
		break;
	case crAlphaFunc::ALWAYS:
		glAlphaFunc(GL_ALWAYS,m_referenceValue);
		break;
	}

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crAlphaFunc OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

