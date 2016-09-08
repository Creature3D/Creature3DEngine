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
//class Extensions : public CRCore::Referenced
//{
//public:
//	Extensions(unsigned int contextID)
//	{
//		m_isBlendColorSupported = isGLExtensionSupported(contextID,"GL_EXT_blend_color") ||
//			strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;
//		m_glBlendColor = getGLExtensionFuncPtr("glBlendColor", "glBlendColorEXT");
//	}
//	bool isBlendColorSupported() const { return m_isBlendColorSupported; }
//	void glBlendColor(float red , float green , float blue , float alpha) const
//	{
//		if (m_glBlendColor)
//		{
//			typedef void (APIENTRY * GLBlendColorProc) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
//			((GLBlendColorProc)m_glBlendColor)(red, green, blue, alpha);
//		}
//	}
//protected:
//	~Extensions() {}
//	bool    m_isBlendColorSupported;
//	void*   m_glBlendColor;
//
//};
//typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
//static BufferedExtensions s_extensions;
//Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
//{
//	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
//	return s_extensions[contextID].get();
//}
typedef void (APIENTRY * GLBlendColorProc) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void crBlendColor::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();

	static bool s_isBlendColorSupported = isGLExtensionSupported(contextID,"GL_EXT_blend_color") ||
		strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;
	static GLBlendColorProc s_glBlendColor = (GLBlendColorProc)getGLExtensionFuncPtr("glBlendColor", "glBlendColorEXT");

	if (!s_isBlendColorSupported)
	{
		//notify(CRCore::WARN)<<"Warning: crBlendColor::apply(..) failed, crBlendColor is not support by OpenGL driver."<<std::endl;
		return;
	}

	s_glBlendColor(m_constantColor[0], m_constantColor[1],
		m_constantColor[2], m_constantColor[3]);

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendColor OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif

}