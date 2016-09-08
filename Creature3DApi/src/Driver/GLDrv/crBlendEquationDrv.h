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
#include <CRCore/buffered_value.h>
#include <CRCore/crStreamBuf.h>
//class Extensions : public CRCore::Referenced
//{
//public:
//	Extensions(unsigned int contextID)
//	{
//		m_isBlendEquationSupported = isGLExtensionSupported(contextID,"GL_EXT_blend_equation") ||
//			strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;
//		m_isSGIXMinMaxSupported = isGLExtensionSupported(contextID, "GL_SGIX_blend_alpha_minmax");
//		m_isLogicOpSupported = isGLExtensionSupported(contextID, "GL_EXT_blend_logic_op");
//		m_glBlendEquation = getGLExtensionFuncPtr("glBlendEquation", "glBlendEquationEXT");
//	}
//	bool isBlendEquationSupported() const { return m_isBlendEquationSupported; }
//	bool isSGIXMinMaxSupported() const { return m_isSGIXMinMaxSupported; }
//	bool isLogicOpSupported() const { return m_isLogicOpSupported; }
//	void glBlendEquation(GLenum mode) const
//	{
//		if (m_glBlendEquation)
//		{
//			typedef void (APIENTRY * GLcrOglBlendEquationDrawProc) (GLenum mode);
//			((GLcrOglBlendEquationDrawProc)m_glBlendEquation)(mode);
//		}
//		else
//		{
//			notify(WARN)<<"Error: glBlendEquation not supported by OpenGL driver"<<std::endl;
//		}
//	}
//protected:
//	~Extensions() {}
//	bool    m_isBlendEquationSupported;
//	bool    m_isSGIXMinMaxSupported;
//	bool    m_isLogicOpSupported;
//	void*   m_glBlendEquation;
//
//};
//typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
//static BufferedExtensions s_extensions;
//Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
//{
//	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
//	return s_extensions[contextID].get();
//}
typedef void (APIENTRY * GLcrOglBlendEquationDrawProc) (GLenum mode);
void crBlendEquation::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();
	static bool s_isBlendEquationSupported = isGLExtensionSupported(contextID,"GL_EXT_blend_equation") ||
		strncmp((const char*)glGetString(GL_VERSION),"1.2",3)>=0;
	static bool s_isSGIXMinMaxSupported = isGLExtensionSupported(contextID, "GL_SGIX_blend_alpha_minmax");
	static bool s_isLogicOpSupported = isGLExtensionSupported(contextID, "GL_EXT_blend_logic_op");
	static GLcrOglBlendEquationDrawProc glBlendEquation = (GLcrOglBlendEquationDrawProc)getGLExtensionFuncPtr("glBlendEquation", "glBlendEquationEXT");


	if (!s_isBlendEquationSupported)
	{
		//notify(WARN)<<"Warning: crBlendEquation::apply(..) failed, crBlendEquation is not support by OpenGL driver."<<std::endl;
		return;
	}

	switch(m_equation) 
	{
	case crBlendEquation::RGBA_MIN:
		glBlendEquation(GL_MIN);
		break;
	case crBlendEquation::RGBA_MAX:
		glBlendEquation(GL_MAX);
		break;
	case crBlendEquation::ALPHA_MIN:
		if(!s_isSGIXMinMaxSupported)
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, SGIX_blend_alpha_minmax extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		glBlendEquation(GL_ALPHA_MIN_SGIX);
		break;
	case crBlendEquation::ALPHA_MAX:
		if(!s_isSGIXMinMaxSupported)
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, SGIX_blend_alpha_minmax extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		glBlendEquation(GL_ALPHA_MAX_SGIX);
		break;
	case crBlendEquation::LOGIC_OP:
		if(!s_isLogicOpSupported)
		{
			notify(WARN)<<"Warning: BlendEquation::apply(..) failed, EXT_blend_logic_op extension is not supported by OpenGL driver." << std::endl;
			return;
		}
		glBlendEquation(GL_LOGIC_OP);
		break;
	case crBlendEquation::FUNC_ADD:
		glBlendEquation(GL_FUNC_ADD);
		break;
	case crBlendEquation::FUNC_SUBTRACT:
		glBlendEquation(GL_FUNC_SUBTRACT);
		break;
	case crBlendEquation::FUNC_REVERSE_SUBTRACT:
		glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		break;
	}
#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendEquation OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}