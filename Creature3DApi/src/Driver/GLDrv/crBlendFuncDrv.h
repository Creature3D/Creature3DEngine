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
#include <CRCore/buffered_value.h>
#include <CRCore/crStreamBuf.h>
#include <crgl/GLExtensions.h>
//class Extensions : public CRCore::Referenced
//{
//public:
//	Extensions(unsigned int contextID)
//	{
//		m_isBlendFuncSeparateSupported = isGLExtensionSupported(contextID, "GL_EXT_blend_func_separate") ||
//			strncmp((const char*)glGetString(GL_VERSION), "1.4", 3) >= 0;
//
//		m_glBlendFuncSeparate = getGLExtensionFuncPtr("glBlendFuncSeparate", "glBlendFuncSeparateEXT");
//	}
//	bool isBlendFuncSeparateSupported() const { return m_isBlendFuncSeparateSupported; }
//	void glBlendFuncSeparate(GLenum sfactorRGB,
//		GLenum dfactorRGB,
//		GLenum sfactorAlpha,
//		GLenum dfactorAlpha) const
//	{
//		if (m_glBlendFuncSeparate)
//		{
//			typedef void (APIENTRY * GLBlendFuncSeparateProc) ( GLenum sfactorRGB,
//				GLenum dfactorRGB,
//				GLenum sfactorAlpha,
//				GLenum dfactorAlpha);
//			((GLBlendFuncSeparateProc)m_glBlendFuncSeparate)(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
//		}
//		else
//		{
//			notify(WARN)<<"Error: glBlendFuncSeparate not supported by OpenGL driver"<<std::endl;
//		}
//	}
//protected:
//	~Extensions() {}
//	bool                    m_isBlendFuncSeparateSupported;
//	void*                   m_glBlendFuncSeparate;
//};
//typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
//static BufferedExtensions s_extensions;
//Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
//{
//	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
//	return s_extensions[contextID].get();
//}

#define getFactor(factor)\
{\
	switch(factor)\
    {\
	case crBlendFunc::DST_ALPHA:\
	    factor = GL_DST_ALPHA;\
	    break;\
	case crBlendFunc::DST_COLOR:\
	    factor = GL_DST_COLOR;\
	    break;\
	case crBlendFunc::ONE:\
	    factor = GL_ONE;\
	    break;\
	case crBlendFunc::ONE_MINUS_DST_ALPHA:\
	    factor = GL_ONE_MINUS_DST_ALPHA;\
	    break;\
	case crBlendFunc::ONE_MINUS_DST_COLOR:\
    	factor = GL_ONE_MINUS_DST_COLOR;\
    	break;\
	case crBlendFunc::ONE_MINUS_SRC_ALPHA:\
		factor = GL_ONE_MINUS_SRC_ALPHA;\
		break;\
	case crBlendFunc::ONE_MINUS_SRC_COLOR:\
		factor = GL_ONE_MINUS_SRC_COLOR;\
		break;\
	case crBlendFunc::SRC_ALPHA:\
		factor = GL_SRC_ALPHA;\
		break;\
	case crBlendFunc::SRC_ALPHA_SATURATE:\
		factor = GL_SRC_ALPHA_SATURATE;\
		break;\
	case crBlendFunc::SRC_COLOR:\
		factor = GL_SRC_COLOR;\
		break;\
	case crBlendFunc::CONSTANT_COLOR:\
		factor = GL_CONSTANT_COLOR;\
		break;\
	case crBlendFunc::ONE_MINUS_CONSTANT_COLOR:\
		factor = GL_ONE_MINUS_CONSTANT_COLOR;\
		break;\
	case crBlendFunc::ONE_MINUS_CONSTANT_ALPHA:\
		factor = GL_ONE_MINUS_CONSTANT_ALPHA;\
		break;\
	case crBlendFunc::ZERO:\
		factor = GL_ZERO;\
		break;\
    }\
}

typedef void (APIENTRY * GLBlendFuncSeparateProc) ( GLenum sfactorRGB,
												   GLenum dfactorRGB,
												   GLenum sfactorAlpha,
												   GLenum dfactorAlpha);
void crBlendFunc::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();
	static bool s_isBlendFuncSeparateSupported = isGLExtensionSupported(contextID, "GL_EXT_blend_func_separate") ||
		strncmp((const char*)glGetString(GL_VERSION), "1.4", 3) >= 0;

	static GLBlendFuncSeparateProc glBlendFuncSeparate = (GLBlendFuncSeparateProc)getGLExtensionFuncPtr("glBlendFuncSeparate", "glBlendFuncSeparateEXT");

	unsigned int source_factor = m_source_factor;
	unsigned int destination_factor = m_destination_factor;
	unsigned int source_factor_alpha = m_source_factor_alpha;
	unsigned int destination_factor_alpha = m_destination_factor_alpha;
	
	getFactor(source_factor);
	getFactor(destination_factor);
	getFactor(source_factor_alpha);
	getFactor(destination_factor_alpha);

	if (source_factor != source_factor_alpha ||
		destination_factor != destination_factor_alpha)
	{
		if (!s_isBlendFuncSeparateSupported)
		{
			notify(WARN)<<"Warning: BlendFunc::apply(..) failed, BlendFuncSeparate is not support by OpenGL driver, falling back to BlendFunc."<<std::endl;
		}
		else if(glBlendFuncSeparate)
		{
			glBlendFuncSeparate(source_factor, destination_factor, source_factor_alpha, destination_factor_alpha);
			return;
		}
	}

	glBlendFunc( source_factor, destination_factor );
#ifdef OUTPUTGLERROR
	unsigned int errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crBlendFunc OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
 #endif

}