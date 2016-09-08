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
void crLight::apply(crState& state) const
{
	glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

	GLenum lightCap = (GLenum)(GL_LIGHT0 + m_lightNum);
	//CRCore::notify(WARN)<<"crOglLightDraw i= "<<m_plight->getLightNum()<< std::endl;

	switch(m_lightType) 
	{
	case CRCore::crLight::LIGHT_INFINITE:
		glLightfv( lightCap, GL_AMBIENT,               m_ambient.ptr() );
		glLightfv( lightCap, GL_DIFFUSE,               m_diffuse.ptr() );
		glLightfv( lightCap, GL_SPECULAR,              m_specular.ptr() );
		glLightfv( lightCap, GL_POSITION,              m_position.ptr() );
		break;
	case CRCore::crLight::LIGHT_LOCAL:
		glLightfv( lightCap, GL_AMBIENT,               m_ambient.ptr() );
		glLightfv( lightCap, GL_DIFFUSE,               m_diffuse.ptr() );
		glLightfv( lightCap, GL_SPECULAR,              m_specular.ptr() );
		glLightfv( lightCap, GL_POSITION,              m_position.ptr() );

		glLightf ( lightCap, GL_CONSTANT_ATTENUATION,  m_constant_Attenuation );
		glLightf ( lightCap, GL_LINEAR_ATTENUATION,    m_linear_Attenuation );
		glLightf ( lightCap, GL_QUADRATIC_ATTENUATION, m_quadratic_Attenuation );
		break;
	case CRCore::crLight::LIGHT_SPOT:
		glLightfv( lightCap, GL_AMBIENT,               m_ambient.ptr() );
		glLightfv( lightCap, GL_DIFFUSE,               m_diffuse.ptr() );
		glLightfv( lightCap, GL_SPECULAR,              m_specular.ptr() );
		glLightfv( lightCap, GL_POSITION,              m_position.ptr() );

		glLightfv( lightCap, GL_SPOT_DIRECTION,        m_direction.ptr() );
		glLightf ( lightCap, GL_SPOT_EXPONENT,         m_spot_Exponent );
		glLightf ( lightCap, GL_SPOT_CUTOFF,           m_spot_Cutoff );

		glLightf ( lightCap, GL_CONSTANT_ATTENUATION,  m_constant_Attenuation );
		glLightf ( lightCap, GL_LINEAR_ATTENUATION,    m_linear_Attenuation );
		glLightf ( lightCap, GL_QUADRATIC_ATTENUATION, m_linear_Attenuation );
		break;
	}

    glPopMatrix();

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crLight OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}

void crLight::captureLightState()
{
	GLenum lightCap = GL_LIGHT0 + m_lightNum;

	glGetLightfv( lightCap, GL_AMBIENT,        m_ambient.ptr() );
	glGetLightfv( lightCap, GL_DIFFUSE,        m_diffuse.ptr() );
	glGetLightfv( lightCap, GL_SPECULAR,       m_specular.ptr() );
	glGetLightfv( lightCap, GL_POSITION,       m_position.ptr() );
	glGetLightfv( lightCap, GL_SPOT_DIRECTION, m_direction.ptr() );
	glGetLightfv( lightCap, GL_SPOT_EXPONENT,  &m_spot_Exponent );
	glGetLightfv( lightCap, GL_SPOT_CUTOFF,    &m_spot_Cutoff );
	glGetLightfv( lightCap, GL_CONSTANT_ATTENUATION,   &m_constant_Attenuation );
	glGetLightfv( lightCap, GL_LINEAR_ATTENUATION,     &m_linear_Attenuation );
	glGetLightfv( lightCap, GL_QUADRATIC_ATTENUATION,  &m_quadratic_Attenuation );
}
