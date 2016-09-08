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
#include <crgl/GLExtensions.h>
#include <CRCore/crStreamBuf.h>
#define getOperation(op)\
{\
    switch(m_sfail) \
    {\
    case crStencil::KEEP:\
	    op = GL_KEEP;\
		break;\
    case crStencil::ZERO:\
	    op = GL_ZERO;\
		break;\
    case crStencil::REPLACE:\
	    op = GL_REPLACE;\
		break;\
    case crStencil::INCR:\
	    op = GL_INCR;\
		break;\
    case crStencil::DECR:\
	    op = GL_DECR;\
		break;\
    case crStencil::INVERT:\
	    op = GL_INVERT;\
		break;\
	}\
}

#define getFunc(func)\
{\
	switch(m_func)\
	{\
	case crStencil::NEVER:\
        func = GL_NEVER;\
		break;\
	case crStencil::LESS:\
		func = GL_LESS;\
		break;\
	case crStencil::EQUAL:\
		func = GL_EQUAL;\
		break;\
	case crStencil::LEQUAL:\
		func = GL_LEQUAL;\
		break;\
	case crStencil::GREATER:\
		func = GL_GREATER;\
		break;\
	case crStencil::NOTEQUAL:\
		func = GL_NOTEQUAL;\
		break;\
	case crStencil::GEQUAL:\
		func = GL_GEQUAL;\
		break;\
	case crStencil::ALWAYS:\
		func = GL_ALWAYS;\
		break;\
	}\
}
void crStencil::apply(crState& state) const
{
    unsigned int func;
	unsigned int sfail;
	unsigned int zfail;
	unsigned int zpass;

    getFunc(func);
	getOperation(sfail);
	getOperation(zfail);
	getOperation(zpass);

	glStencilFunc( func, m_funcRef, m_funcMask );
	glStencilOp( sfail, zfail, zpass );
	glStencilMask( m_writeMask );

 //   glStencilFunc( func, m_pStencil->getFunctionRef(), m_pStencil->getFunctionMask() );
 //   glStencilOp( sfail, zfail, zpass );
	//glStencilMask( m_pStencil->getWriteMask() );

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crStencil OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}
