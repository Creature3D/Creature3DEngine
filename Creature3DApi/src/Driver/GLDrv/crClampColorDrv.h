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
#include <crgl/GLExtensions.h>
#include <CRCore/buffered_value.h>

#ifndef GL_ARB_color_buffer_float
#define GL_RGBA_FLOAT_MODE_ARB                  0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB               0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB             0x891B
#define GL_CLAMP_READ_COLOR_ARB                 0x891C
#define GL_FIXED_ONLY_ARB                       0x891D
#endif

#ifndef GL_FIXED_ONLY
#define GL_FIXED_ONLY                           GL_FIXED_ONLY_ARB
#define GL_CLAMP_VERTEX_COLOR                   GL_CLAMP_VERTEX_COLOR_ARB
#define GL_CLAMP_READ_COLOR                     GL_CLAMP_READ_COLOR_ARB
#define GL_CLAMP_FRAGMENT_COLOR                 GL_CLAMP_FRAGMENT_COLOR_ARB
#endif
//class Extensions : public CRCore::Referenced
//{
//public:
//	Extensions(unsigned int contextID)
//	{
//		m_isClampColorSupported = isGLExtensionSupported(contextID,"GL_ARB_color_buffer_float") ||
//			strncmp((const char*)glGetString(GL_VERSION),"2.0",3)>=0;
//
//		m_glClampColor = getGLExtensionFuncPtr("glClampColor", "glClampColorARB");
//	}
//	bool isClampColorSupported() const { return m_isClampColorSupported; }
//	void glClampColor(GLenum target, GLenum mode) const
//	{
//		if (m_glClampColor)
//		{
//			typedef void (APIENTRY * GLClampColorProc) (GLenum target, GLenum mode);
//			((GLClampColorProc)m_glClampColor)(target,mode);
//		}
//		else
//		{
//			notify(WARN)<<"Error: glClampColor not supported by OpenGL driver"<<std::endl;
//		}
//	}
//protected:
//	~Extensions() {}
//	bool    m_isClampColorSupported;
//	void*   m_glClampColor;
//};
//typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
//static BufferedExtensions s_extensions;
//Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
//{
//	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
//	return s_extensions[contextID].get();
//}

typedef void (APIENTRY * GLClampColorProc) (GLenum target, GLenum mode);
void crClampColor::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();
	static bool s_isClampColorSupported = isGLExtensionSupported(contextID,"GL_ARB_color_buffer_float") ||
		strncmp((const char*)glGetString(GL_VERSION),"2.0",3)>=0;

	static GLClampColorProc glClampColor = (GLClampColorProc)getGLExtensionFuncPtr("glClampColor", "glClampColorARB");

	if (!s_isClampColorSupported)
	{
		notify(WARN)<<"Warning: crClampColor::apply(..) failed, crClampColor is not support by OpenGL driver."<<std::endl;
		return;
	}
	glClampColor(GL_CLAMP_VERTEX_COLOR, m_clampVertexColor);
	glClampColor(GL_CLAMP_FRAGMENT_COLOR, m_clampFragmentColor);
	glClampColor(GL_CLAMP_READ_COLOR, m_clampReadColor);
}