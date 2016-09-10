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
void crLogicOp::apply(crState& state) const
{
	switch(m_opcode) 
	{
	case crLogicOp::CLEAR:
		glLogicOp(GL_CLEAR);
    	break;
	case crLogicOp::SET:
		glLogicOp(GL_SET);
		break;
	case crLogicOp::COPY:
		glLogicOp(GL_COPY);
		break;
	case crLogicOp::COPY_INVERTED:
		glLogicOp(GL_COPY_INVERTED);
		break;
	case crLogicOp::NOOP:
		glLogicOp(GL_NOOP);
		break;
	case crLogicOp::INVERT:
		glLogicOp(GL_INVERT);
		break;
	case crLogicOp::AND:
		glLogicOp(GL_AND);
		break;
	case crLogicOp::NAND:
		glLogicOp(GL_NAND);
		break;
	case crLogicOp::OR:
		glLogicOp(GL_OR);
		break;
	case crLogicOp::NOR:
		glLogicOp(GL_NOR);
		break;
	case crLogicOp::XOR:
		glLogicOp(GL_XOR);
		break;
	case crLogicOp::EQUIV:
		glLogicOp(GL_EQUIV);
		break;
	case crLogicOp::AND_REVERSE:
		glLogicOp(GL_AND_REVERSE);
		break;
	case crLogicOp::AND_INVERTED:
		glLogicOp(GL_AND_INVERTED);
		break;
	case crLogicOp::OR_REVERSE:
		glLogicOp(GL_OR_REVERSE);
		break;
	case crLogicOp::OR_INVERTED:
		glLogicOp(GL_OR_INVERTED);
		break;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crLogicOp OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

