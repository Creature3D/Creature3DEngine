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
//		m_isMultisampleSupported = isGLExtensionSupported(contextID,"GL_ARB_multisample");
//		m_isMultisampleFilterHintSupported = isGLExtensionSupported(contextID,"GL_NV_multisample_filter_hint");
//
//		m_glSampleCoverage = getGLExtensionFuncPtr("glSampleCoverageARB");
//	}
//	bool isMultisampleSupported() const { return m_isMultisampleSupported; }
//	bool isMultisampleFilterHintSupported() const { return m_isMultisampleFilterHintSupported; }
//	void glSampleCoverage(GLclampf value, GLboolean invert) const
//	{
//		if (m_glSampleCoverage)
//		{
//			typedef void (APIENTRY * GLSampleCoverageProc) (GLclampf value, GLboolean invert);
//			((GLSampleCoverageProc)m_glSampleCoverage)(value, invert);
//		}
//		else
//		{
//			notify(WARN)<<"Error: glSampleCoverage not supported by OpenGL driver"<<std::endl;
//		}
//	}
//protected:
//	~Extensions() {}
//	bool    m_isMultisampleSupported;
//	bool    m_isMultisampleFilterHintSupported;
//	void*   m_glSampleCoverage;
//};
//typedef buffered_value< ref_ptr<Extensions> > BufferedExtensions;
//static BufferedExtensions s_extensions;
//Extensions* getExtensions(unsigned int contextID,bool createIfNotInitalized)
//{
//	if (!s_extensions[contextID] && createIfNotInitalized) s_extensions[contextID] = new Extensions(contextID);
//	return s_extensions[contextID].get();
//}

typedef void (APIENTRY * GLSampleCoverageProc) (GLclampf value, GLboolean invert);
void crMultisample::apply(crState& state) const
{
	unsigned int contextID = state.getContextID();
	//static bool s_isMultisampleSupported = isGLExtensionSupported(contextID,"GL_ARB_multisample");
	//static bool s_isMultisampleFilterHintSupported = isGLExtensionSupported(contextID,"GL_NV_multisample_filter_hint");

	static GLSampleCoverageProc glSampleCoverage = (GLSampleCoverageProc)getGLExtensionFuncPtr("glSampleCoverageARB");

	//const Extensions* extensions = getExtensions(contextID,true);

	//if (!s_isMultisampleSupported)
	//{
	//	notify(WARN)<<"Warning: crMultisample::apply(..) failed, crMultisample is not support by OpenGL driver."<<std::endl;
	//	return;
	//}

	//if(extensions->isMultisampleFilterHintSupported())
	//{
	//    switch(m_mode) 
	//	{
	//    case crMultisample::FASTEST:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_FASTEST);
	//	    break;
	//    case crMultisample::NICEST:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	//	    break;
	//    case crMultisample::DONT_CARE:
	//		glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_DONT_CARE);
	//	    break;
	//	}
	//}
	if(glSampleCoverage)
		glSampleCoverage(m_coverage, m_invert);

#ifdef OUTPUTGLERROR
	GLenum errorNo = glGetError();
	if (errorNo!=GL_NO_ERROR)
	{
		char gbuf[256];
		sprintf(gbuf,"crMultisample OpenGL error %s\n\0",gluErrorString(errorNo));
		gDebugInfo->debugInfo(CRCore::NOTICE,gbuf);
	}
#endif
}
