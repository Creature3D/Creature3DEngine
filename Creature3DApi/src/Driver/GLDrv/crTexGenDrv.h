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
void crTexGen::apply(crState& state) const
{
   switch( m_mode ) 
   {
   case crTexGen::OBJECT_LINEAR:
	 glTexGenfv(GL_S, GL_OBJECT_PLANE, m_plane_s.ptr());
	 glTexGenfv(GL_T, GL_OBJECT_PLANE, m_plane_t.ptr());
	 glTexGenfv(GL_R, GL_OBJECT_PLANE, m_plane_r.ptr());
	 glTexGenfv(GL_Q, GL_OBJECT_PLANE, m_plane_q.ptr());

	 glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	 glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	 glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	 glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );

	// note, R & Q will be disabled so R&Q settings won't
	// have an effect, see above comment in enable(). RO.
   	break;
   case crTexGen::EYE_LINEAR:
	 glTexGenfv(GL_S, GL_EYE_PLANE, m_plane_s.ptr());
	 glTexGenfv(GL_T, GL_EYE_PLANE, m_plane_t.ptr());
	 glTexGenfv(GL_R, GL_EYE_PLANE, m_plane_r.ptr());
	 glTexGenfv(GL_Q, GL_EYE_PLANE, m_plane_q.ptr());

	 glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	 glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	 glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );
	 glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR );

	 // note, R & Q will be disabled so R&Q settings won't
	 // have an effect, see above comment in enable(). RO.
   	 break;
   case crTexGen::NORMAL_MAP:
	 glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
	 glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
	 glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
	 //      glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
	 break;
   case crTexGen::REFLECTION_MAP:
     glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	 glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	 glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	 //      glTexGeni( GL_Q, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP );
	 break;
   case crTexGen::SPHERE_MAP:
	 // We ignore the planes if we are not in OBJECT_ or EYE_LINEAR mode.

	 // Also don't set the mode of GL_R & GL_Q as these will generate
	 // GL_INVALID_ENUM (See OpenGL Refrence Guide, glTexGEn.)

	 glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
	 glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
	 break;
   } 

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crTexGen OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}
