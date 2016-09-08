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
void crLightModel::apply(crState& state) const
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,m_ambient.ptr());

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,m_plm->getAmbientIntensity().ptr());
	
	////static bool s_separateSpecularSupported = strcmp((const char *)glGetString(GL_VERSION),"1.2")>=0;
	//static bool s_separateSpecularSupported = (std::string((const char *)glGetString(GL_VERSION)) == "1.2");
	//if (s_separateSpecularSupported)
	//{
	//	if (m_plm->getColorControl()==crLightModel::SEPARATE_SPECULAR_COLOR)
	//    {
	//        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	//    }
	//    else
	//    {
	//	    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
	//    }
	//}
	    
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,m_plm->getLocalViewer());
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,m_plm->getTwoSided());

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crLightModel OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}